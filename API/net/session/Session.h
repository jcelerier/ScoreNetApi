#pragma once

#include "../permission/PermissionManager.h"
#include "../group/GroupManager.h"
#include "../client/ClientManager.h"
#include "../client/RemoteMaster.h"

#include <type_traits>

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
			Session(name, generateRandom64())
		{
		}

		Session(std::string name, int id):
			hasName(name),
			hasId(id)
		{
		}

		Session(Session&&) = default;
		Session(const Session&) = default;
		virtual ~Session() = default;

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

		void changePermission(Client& client,
							  Group& group,
							  Permission::Category cat,
							  Permission::Enablement enablement)
		{
			auto& perm = _permissions(client, group);
			perm.setPermission(cat, enablement);

			// /session/permission/update cat enablement
			_remoteMaster->send("/session/permission/update",
								getId(),
								client.getId(),
								group.getId(),
								static_cast<std::underlying_type<Permission::Category>::type>(cat),
								static_cast<std::underlying_type<Permission::Enablement>::type>(enablement));
		}

		template<typename... K>
		bool getPermission(K&&... args)
		{
			return _permissions.getPermission(std::forward<K>(args)...);
		}

		virtual Client& getClient() = 0;

	protected:
		std::unique_ptr<RemoteMaster> _remoteMaster{nullptr};

		GroupManager _groups;
		PermissionManager _permissions;
		ClientManager _clients;

	private: // Accessible uniquement à masterSession et ClientSessionBuilder
		template<typename... K>
		RemoteClient& private__createClient(K&&... args)
		{
			auto& client = _clients.createConnection(std::forward<K>(args)...);

			for(Group& group : _groups)
			{
				_permissions.create(group, client);
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
				_permissions.create(group, client);
			}

			_permissions.create(group, getClient());
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

			_permissions.remove(group, getClient());
			_groups.remove(std::forward<K>(args)...);
		}
};

using Session_p = std::unique_ptr<Session>;


// Chargé de construire une session en fonction des données reçues petit à petit
class RemoteSessionBuilder
{

};
