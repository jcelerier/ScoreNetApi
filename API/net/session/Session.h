#pragma once

#include "../permission/PermissionManager.h"
#include "../group/GroupManager.h"
#include "../client/ClientManager.h"
#include "../client/RemoteMaster.h"
#include <vector>
#include <utility>

#include <cstdint>
#include <limits>
#include <random>

inline int32_t generateRandom64()
{
	using namespace std;
	static random_device rd;
	static mt19937 gen(rd());
	static uniform_int_distribution<int32_t>
			dist(numeric_limits<int32_t>::min(),
				 numeric_limits<int32_t>::max());

	return dist(gen);
}

class ClientSessionBuilder;

class Session : public hasName, public hasId
{
		friend class ClientSessionBuilder;
	public:
		Session(std::string name):
			hasName(name),
			hasId(generateRandom64()),
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
		std::unique_ptr<RemoteMaster> _remoteMaster;

		GroupManager _groups;
		PermissionManager _permissions;
		ClientManager _clients;

		// Mettre à jour tous les Clients qui listen() ce groupe (incohérence lors de sauvegarde?)
		void groupChanged(Group& ) // Mettre la mise-à-jour des scénarios qui y sont reliés ici ?
		{

		}

		// Mettre à jour tout le monde
		void clientChanged(Client& )
		{

		}

		// Mettre à jour :
		// - ceux qui lui envoyaient et qui ne lui envoient plus
		// - ceux qui ne lui envoyaient pas et qui lui envoient
		void permissionChanged(Permission& )
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

	private: // Accessible uniquement à masterSession et ClientSessionBuilder
		template<typename... K>
		RemoteClient& private__createClient(K&&... args)
		{
			auto& client = _clients.createConnection(std::forward<K>(args)...);

			for(Group& group : _groups)
			{
				_permissions.createPermission(group, client);
			}

			return client;
		}

		template<typename... K>
		void private__removeClient(K&&... args)
		{
			RemoteClient& client = _clients(std::forward<K>(args)...); // Faire has()
			for(Group& group : _groups)
			{
				_permissions.remove(group, client);
			}

			_clients.remove(std::forward<K>(args)...);
		}

		template<typename... K>
		Group& private__createGroup(K&&... args)
		{
			Group& group = _groups.createGroup(std::forward<K>(args)...);
			for(RemoteClient& client : _clients)
			{
				_permissions.createPermission(group, client);
			}

			return group;
		}

		template<typename... K>
		void private__removeGroup(K&&... args)
		{
			Group& group = _groups(std::forward<K>(args)...); // Faire has()
			for(RemoteClient& client : _clients)
			{
				_permissions.remove(group, client);
			}

			_groups.remove(std::forward<K>(args)...);
		}
};

using Session_p = std::unique_ptr<Session>;


// Chargé de construire une session en fonction des données reçues petit à petit
class RemoteSessionBuilder
{

};
