#pragma once
#include "../group/Group.h"
#include "../client/Client.h"

#include <iostream>
class Permission
{
	public:
		enum class Category   : int  { Read = 0, Write = 1, Execute = 2 };
		enum class Enablement : bool { Enabled = true, Disabled = false };

		static const std::function<bool(Permission&)> hasSame(const Group& g,
															  const Client& c)
		{
			return [&g, &c] (const Permission& p)
					{ return *p._group == g && *p._client == c; };
		}

		static const std::function<bool(Permission&)> hasSame(const Client& c,
															  const Group& g)
		{
			return [&g, &c] (const Permission& p)
					{ return *p._group == g && *p._client == c; };
		}

		static const std::function<bool(const Permission&)> hasSame(const Group& g)
		{
			return [&g] (const Permission& p)
					{ return *p._group == g; };
		}

		static const std::function<bool(const Permission&)> hasSame(const Client& c)
		{
			return [&c] (const Permission& p)
					{ return *p._client == c; };
		}

		Permission(const Group& group,
				   const Client& client):
			_group(&group),
			_client(&client)
		{
		}

		Permission(Permission&&) = default;
		Permission(const Permission&) = default;
		Permission& operator=(Permission&&) = default;
		Permission& operator=(const Permission&) = default;
		~Permission() = default;

		bool operator==(const Permission& p)
		{
			return *p._group == *_group && *p._client == *_client;
		}

		bool listens() const
		{
			return read or write or exec;
		}

		bool writes() const
		{
			return write;
		}

		void setPermission(Permission::Category cat,
							  Permission::Enablement enablement)
		{
			switch(cat)
			{
				case Category::Read:
					read = static_cast<bool>(enablement);
					break;
				case Category::Write:
					write = static_cast<bool>(enablement);
					break;
				case Category::Execute:
					exec = static_cast<bool>(enablement);
					break;
				default:
					throw;
			}
		}

		bool getPermission(Permission::Category cat)
		{
			switch(cat)
			{
				case Category::Read:
					return read;
				case Category::Write:
					return write;
				case Category::Execute:
					return exec;
				default:
					throw;
			}
		}

	private:
		bool read = false;
		bool write = false;
		bool exec = false;

		const Group* _group;
		const Client* _client;
};
