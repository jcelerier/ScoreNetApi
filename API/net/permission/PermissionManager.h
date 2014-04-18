#pragma once
#include <vector>
#include "Permission.h"

class PermissionManager : public Iterable<Permission>
{
	private:
		using SignalHandler = std::function<void()>;

		Permission::PermissionSignalHandler permissionChanged; //Même handler pour tous les groupes
		SignalHandler changed;

	public:
		PermissionManager(Permission::PermissionSignalHandler onPermChange, SignalHandler onChange):
			permissionChanged(onPermChange),
			changed(onChange)
		{
		}

		Permission& createPermission(const Group& g,
									 const RemoteClient& c)
		{
			return create(g, c, permissionChanged);
		}

		template<typename... K>
		void remove(K&&... args)
		{
			 Iterable<Permission>::remove(std::forward<K>(args)...);
			 changed();
		}

		void changePermission(RemoteClient& client,
							  Group& group,
							  Permission::Category cat,
							  Permission::Enablement enablement)
		{
			switch(cat)
			{
				case Permission::Category::Read:
				{
					(*this)(group, client).read = static_cast<bool>(enablement);
					return;
				}
				case Permission::Category::Write:
				{
					(*this)(group, client).write = static_cast<bool>(enablement);
					return;
				}
				case Permission::Category::Execute:
				{
					(*this)(group, client).exec = static_cast<bool>(enablement);
					return;
				}
			}

			throw "Bad category.";
		}

		bool getPermission(RemoteClient& client,
						   Group& group,
						   Permission::Category cat)
		{
			switch(cat)
			{
				case Permission::Category::Read:
				{
					return (*this)(group, client).read;
				}
				case Permission::Category::Write:
				{
					return (*this)(group, client).write;
				}
				case Permission::Category::Execute:
				{
					return (*this)(group, client).exec;
				}
			}

			throw "Bad category.";
		}
};

