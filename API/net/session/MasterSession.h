#pragma once
#include "Session.h"
#include "../client/LocalMaster.h"


// Utiliser templates à la place ?
class MasterSession : public Session
{
	public:
		template<typename... K>
		MasterSession(K&&... args):
			Session(std::forward<K>(args)...),
			_localMaster(new LocalMaster(9000, 0, "master"))
		{
			OscReceiver::connection_handler h = std::bind(&MasterSession::handle__session_connect,
														  this,
														  std::placeholders::_1,
														  std::placeholders::_2);

			_localMaster->receiver().setConnectionHandler("/session/connect", h);
			_localMaster->receiver().addHandler("/session/permission/update",
												&MasterSession::handle__session_permission_update,
												this);

			_localMaster->receiver().run();
		}

		virtual ~MasterSession() = default;

		void handle__session_connect(osc::ReceivedMessageArgumentStream args, std::string ip)
		{
			osc::int32 idSession, clientListenPort;
			const char* cname;

			args >> idSession >> cname >> clientListenPort >> osc::EndMessage;
			std::string clientName(cname);
			OscSender clientSender(ip, clientListenPort);

			// Test si bonne session
			if(idSession != getId()) return;

			// Recherche si nom existe déjà
			if(std::any_of(begin(_clients), end(_clients),
						   [&clientName] (RemoteClient& c)
							{ return c.getName() == clientName;	}))
			{

				clientSender.send(osc::MessageGenerator()("/session/clientNameIsTaken",
														  getId(),
														  cname));
				return;
			}

			createClient(clientName, std::move(clientSender));
		}

		// /session/permission/update sessionId clientId groupId category enablement
		void handle__session_permission_update(osc::ReceivedMessageArgumentStream args)
		{
			osc::int32 sessionId, clientId, groupId;
			osc::int32 cat;
			bool enablement;

			args >> sessionId >> clientId >> groupId >> cat >> enablement >> osc::EndMessage;

			if(sessionId != getId()) return;

			auto& client = _clients(clientId);
			auto& group  = _groups(groupId);
			auto& perm   = _permissions(client,
										group);

			bool prevListens = perm.listens(),
				 prevWrites  = perm.writes();

			perm.setPermission(static_cast<Permission::Category>(cat),
							   static_cast<Permission::Enablement>(enablement));


			// Cas de changements :
			//	- !listens() devient  listens()
			//		=> ceux qui ont W sur group prennent connaissance de moi.
			//	-  listens() devient !listens()
			//		=> ceux qui ont W sur group m'oublient.
			//  - !writes()  devient  writes()
			//		=> je prends connaissance de ceux qui ont R / W / X sur groupe.
			//  -  writes()  devient !writes()
			//		=> j'oublie tout le monde sur ce groupe.
			//
			// Dans tous les cas on informe le maître, c'est lui qui informe les uns et les autres.
			//
			// Faire un RemotePermission avec juste listens() et writes()
			if(!prevListens && perm.listens())
			{
				for(RemoteClient& rclt : _clients)
				{
					if(_permissions(rclt, group).writes())
					{
						rclt.initConnectionTo(client);
						rclt.send("/session/permission/update",
								  getId(),
								  client.getId(),
								  group.getId(),
								  cat,
								  enablement);
					}
				}
			}
			else if(prevListens && !perm.listens())
			{
				for(RemoteClient& rclt : _clients)
				{
					if(_permissions(rclt, group).writes())
					{
						rclt.send("/session/permission/update",
								  getId(),
								  client.getId(),
								  group.getId(),
								  cat,
								  enablement);
					}
				}
			}

			if(!prevWrites && perm.writes())
			{
				for(RemoteClient& rclt : _clients)
				{
					if(_permissions(rclt, group).listens())
					{
						client.initConnectionTo(rclt);
						client.send("/session/permission/update",
									getId(),
									rclt.getId(),
									group.getId(),
									cat,
									enablement);

					}
				}
			}
			else if(prevWrites && !perm.writes())
			{
				// Celui-là peut être fait en local.
			}


		}

		virtual Client& getClient() override
		{
			return *_localMaster;
		}

		LocalMaster& getLocalMaster()
		{
			return *_localMaster;
		}

		template<typename... K>
		RemoteClient& createClient(K&&... args)
		{
			auto& client = private__createClient(std::forward<K>(args)...);

			// Send the id.
			client.send("/session/setClientId",
						getId(),
						client.getId());

			// Send the groups information.
			for(auto& group : _groups)
			{
				client.send("/session/update/group",
							(osc::int32) getId(),
							(osc::int32) group.getId(),
							group.getName().c_str(),
							group.isMuted());
			}

			// Send isReady
			client.send("/session/isReady",
						(osc::int32) getId());

			// Send information to others clients only when required.
			// (i. e. when acquiring write permission on a group)

			return client;
		}

		/** Après ceux-là, informer sur réseau **/
		template<typename... K>
		void removeClient(K&&... args)
		{
			private__removeClient(std::forward<K>(args)...);
		}

		template<typename... K>
		Group& createGroup(K&&... args)
		{
			return private__createGroup(std::forward<K>(args)...);
		}

		template<typename... K>
		void removeGroup(K&&... args)
		{
			private__removeGroup(std::forward<K>(args)...);
		}



		template<typename... K>
		void muteGroup(K&&... args)
		{
			_groups(std::forward<K>(args)...).mute();
		}

		template<typename... K>
		void unmuteGroup(K&&... args)
		{
			_groups(std::forward<K>(args)...).unmute();
		}

		// Mute & unmute
		// Permettre de tout voir ?
	private:
		std::unique_ptr<LocalMaster> _localMaster{nullptr};
};
