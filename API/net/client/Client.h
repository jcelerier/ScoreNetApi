#pragma once
#include <string>
#include <memory>
#include "../osc/oscreceiver.h"

#include "../group/Group.h"

#include"../properties/hasId.h"
#include"../properties/hasName.h"
class Client : public hasId, public hasName
{
	public:
		using ClientSignalHandler = std::function<void(Client&)>;
		using SignalHandler = std::function<void()>;

		using hasName::hasSame;
		using hasId::hasSame;

		Client(const std::string& hostname):
			hasId(-1),
			hasName(hostname),
			changed{}
		{

		}

		Client(const int id,
			   const std::string& hostname,
			   ClientSignalHandler changeHandler):
			hasId(id),
			hasName(hostname),
			changed(std::bind(changeHandler, std::ref(*this)))
		{
		}

		virtual ~Client()
		{
			if(changed)
				changed();
		}

		Client(Client&&) = default;
		Client(const Client&) = default;
		Client& operator=(const Client&) = default;
		Client& operator=(Client&&) = default;

		bool operator==(const Client& c) const
		{
			return c.getId() == getId();
		}

	protected:
		SignalHandler changed;
};

using Client_p = std::unique_ptr<Client>;
