#pragma once
#include "Session.h"
#include "../client/LocalMaster.h"


// Utiliser templates à la place ?
class MasterSession : public Session
{
	public:
		using Session::Session;

		void send__session_update_group(RemoteClient& client)
		{

		}


		void handle__session_connect(osc::ReceivedMessageArgumentStream args, std::string ip)
		{
			osc::int32 idSession, clientListenPort;
			const char* cname;

			args >> idSession >> cname >> clientListenPort >> osc::EndMessage;
			std::string clientName(cname);
			OscSender tempSender(ip, clientListenPort);

			// Test si bonne session
			if(idSession != getId()) return;

			// Recherche si nom existe déjà
			if(std::any_of(begin(_clients), end(_clients),
						   [&clientName] (RemoteClient& c)
							{ return c.getName() == clientName;	}))
			{

				tempSender.send(osc::MessageGenerator()("/session/clientNameIsTaken",
														getId(),
														cname));
				return;
			}

			createClient(clientName, std::move(tempSender));
		}

		virtual Client& getLocalClient() override
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
						(osc::int32) getId(),
						true);

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
		std::unique_ptr<LocalMaster> _localMaster;
};
