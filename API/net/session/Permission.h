#pragma once
#include "../group/Group.h"
#include "../client/Client.h"

#include <iostream>
class Permission: public hasName, public hasId
{
	public:
		enum class Category { Read, Write, Execute };
		enum class Enablement : bool { Enabled = true, Disabled = false };

		using PermissionSignalHandler = std::function<void(Permission&)>;
		using SignalHandler = std::function<void()>;

		static const std::function<bool(Permission&)> hasSame(const Group& g,
															  const RemoteClient& c)
		{
			return [&g, &c] (const Permission& p)
					{
						return *p._group == g && *p._client == c;
					};
		}

		static const std::function<bool(const Permission&)> hasSame(const Group& g)
		{
			return [&g] (const Permission& p)
					{
						return *p._group == g;
					};
		}

		static const std::function<bool(const Permission&)> hasSame(const RemoteClient& c)
		{
			return [&c] (const Permission& p)
					{
						return *p._client == c;
					};
		}

		Permission(const Group& group,
				   const RemoteClient& client,
				   PermissionSignalHandler changeHandler):
			hasName(""),
			hasId(9),
			_group(&group),
			_client(&client),
			changed(std::bind(changeHandler, std::ref(*this)))
		{
		}

		Permission(Permission&&) = default;
		Permission(const Permission&) = default;
		Permission& operator=(Permission&&) = default;
		Permission& operator=(const Permission&) = default;

		~Permission()
		{
			if(changed)
				changed();
		}

		bool operator==(const Permission& p)
		{
			return *p._group == *_group && *p._client == *_client;
		}

		bool listens() const
		{
			return read | write | exec;
		}

		bool read = false;
		bool write = false;
		bool exec = false;

	private:
		const Group* _group;
		const RemoteClient* _client;

		SignalHandler changed;
};
