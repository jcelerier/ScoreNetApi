#pragma once
#include "Session.h"


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
