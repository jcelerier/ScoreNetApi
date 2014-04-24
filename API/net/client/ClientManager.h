#pragma once
#include "RemoteClient.h"
#include "../Iterable.h"

class ClientManager : public Iterable<RemoteClient>
{
	private:
		unsigned int _lastId = 0;

	public:
		RemoteClient& createConnection(std::string hostname,
									   const std::string& ip,
									   const int port)
		{
			performUniquenessCheck(hostname);
			return create(++_lastId,
						  hostname,
						  ip,
						  port);
		}

		RemoteClient& createConnection(std::string hostname,
									   OscSender&& sender)
		{
			performUniquenessCheck(hostname);
			return create(++_lastId,
						  hostname,
						  std::move(sender));
		}
};
