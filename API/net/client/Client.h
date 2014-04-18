#pragma once
#include <string>
#include <memory>
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

		Client(const int id,
			   const std::string& hostname,
			   const std::string& ip,
			   const int port,
			   ClientSignalHandler changeHandler):
			hasId(id),
			hasName(hostname),
			_ip(ip),
			_port(port),
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

		const std::string& ip() const
		{
			return _ip;
		}

		void setIp(std::string ip)
		{
			_ip = ip;
		}

		int port() const
		{
			return _port;
		}

	protected:
		std::string _ip;
		int _port;

		SignalHandler changed;
		std::vector<Group_p> _groups;
};

using Client_p = std::unique_ptr<Client>;
