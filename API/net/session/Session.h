#pragma once

#include "PermissionManager.h"
#include "../group/GroupManager.h"
#include "../client/ClientManager.h"
#include "../client/LocalClient.h"
#include <vector>
#include <utility>



class Session : public hasName
{
	public:
		Session():
			hasName("Ma Session"),
			_groups(std::bind(&Session::groupChanged, this, std::placeholders::_1),
					std::bind(&Session::groupManagerChanged, this)),
			_permissions(std::bind(&Session::permissionChanged, this, std::placeholders::_1),
						 std::bind(&Session::permissionManagerChanged, this)),
			_clients(std::bind(&Session::clientChanged, this, std::placeholders::_1),
					 std::bind(&Session::clientManagerChanged, this))
		{

		}

		Session(Session&&) = default;
		Session(const Session&) = default;

		template<typename... K>
		Group& group(K&&... args)
		{
			return _groups(std::forward<K>(args)...);
		}

		GroupManager& groups()
		{
			return _groups;
		}


		template<typename... K>
		RemoteClient& client(K&&... args)
		{
			return _clients(std::forward<K>(args)...);
		}

		ClientManager& clients()
		{
			return _clients;
		}

		template<typename... K>
		void changePermission(K&&... args)
		{
			_permissions.changePermission(std::forward<K>(args)...);
		}

		template<typename... K>
		bool getPermission(K&&... args)
		{
			return _permissions.getPermission(std::forward<K>(args)...);
		}

		virtual Client& getLocalClient() = 0;

	protected:
		std::unique_ptr<MasterClient> _masterClient;

		GroupManager _groups;
		PermissionManager _permissions;
		ClientManager _clients;

		// Mettre à jour tous les Clients qui listen() ce groupe (incohérence lors de sauvegarde?)
		void groupChanged(Group& g) // Mettre la mise-à-jour des scénarios qui y sont reliés ici ?
		{

		}

		// Mettre à jour tout le monde
		void clientChanged(Client& c)
		{

		}

		// Mettre à jour :
		// - ceux qui lui envoyaient et qui ne lui envoient plus
		// - ceux qui ne lui envoyaient pas et qui lui envoient
		void permissionChanged(Permission& p)
		{
			// Envoyer à tout le monde
		}

		// Mettre à jour les permissions
		void groupManagerChanged()
		{
			// Envoyer à tout le monde
		}

		void clientManagerChanged()
		{

		}

		// Mettre à jour tout le monde
		void permissionManagerChanged()
		{

		}
};

using Session_p = std::unique_ptr<Session>;

// Utiliser templates à la place ?
class MasterSession : public Session
{
	public:
		MasterSession() {}

		virtual Client& getLocalClient() override
		{
			return *_localClient;
		}

		template<typename... K>
		RemoteClient& createClient(K&&... args)
		{
			auto& client = _clients.createConnection(std::forward<K>(args)...);

			for(Group& group : _groups)
			{
				_permissions.createPermission(group, client);
			}
			// informer sur réseau

			return client;
		}

		template<typename... K>
		void removeClient(K&&... args)
		{
			RemoteClient& client = _clients(std::forward<K>(args)...); // Faire has()
			for(Group& group : _groups)
			{
				_permissions.remove(group, client);
			}

			_clients.remove(std::forward<K>(args)...);
		}


		template<typename... K>
		Group& createGroup(K&&... args)
		{
			Group& group = _groups.createGroup(std::forward<K>(args)...);
			for(RemoteClient& client : _clients)
			{
				_permissions.createPermission(group, client);
			}

			// informer sur réseau
			return group;
		}

		template<typename... K>
		void removeGroup(K&&... args)
		{
			Group& group = _groups(std::forward<K>(args)...); // Faire has()
			for(RemoteClient& client : _clients)
			{
				_permissions.remove(group, client);
			}

			_groups.remove(std::forward<K>(args)...);

			// Informer sur réseau
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
		std::unique_ptr<MasterClient> _localClient;
};

class ClientSession : public Session
{
	public:
		ClientSession() = default;
		ClientSession(ClientSession&&) = default;
		ClientSession(const ClientSession&) = default;



		virtual Client& getLocalClient() override
		{
			return *_localClient;
		}

	private:
		std::unique_ptr<LocalClient> _localClient;
};


class SessionInformation : public hasName
{
	public:
		virtual ~SessionInformation() = default;
		SessionInformation(const SessionInformation& s) = default;
		SessionInformation(SessionInformation&& s) = default;
		// 1. On récupère la liste des sessions dispo sur le réseau
		static std::vector<SessionInformation> list()
		{
			std::vector<SessionInformation> v;

			v.emplace_back("Session Electro-pop",
						   "Machine du compositeur",
						   "127.0.0.1",
						   12345);

			v.emplace_back("Session test",
						   "Machine drôle",
						   "0.1.2.3",
						   8908);

			return v;
		}

		// 2. On appelle "join" sur celle qu'on désire rejoindre.
		std::unique_ptr<ClientSession> join()
		{
			return std::make_unique<ClientSession>();
		}// Attente ici ? ou dynamique ?

		void clientChanged(Client& c) { }
		std::unique_ptr<MasterClient> _masterClient;

		SessionInformation(std::string sessionName,
						   std::string masterName,
						   std::string masterIp,
						   int masterInputPort):
			hasName(sessionName),
			_masterClient(new MasterClient(0, masterName, masterIp, masterInputPort, std::bind(&SessionInformation::clientChanged, this, std::placeholders::_1)))
		{

		}
};


// Chargé de construire une session en fonction des données reçues petit à petit
class RemoteSessionBuilder
{
	public:
		bool isReady();
		ClientSession builtSession() { return std::move(session); }

	private:
		ClientSession session;
};
