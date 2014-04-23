#pragma once
#include "RemoteClient.h"
#include "../Iterable.h"

class ClientManager : public Iterable<RemoteClient>
{
		using SignalHandler = std::function<void()>;
	private:
		unsigned int _lastId = 0;
		Client::ClientSignalHandler clientChanged; //Même handler pour tous les groupes
		SignalHandler changed;

	public:
		ClientManager(Client::ClientSignalHandler onClientChange, SignalHandler onChange):
			clientChanged(onClientChange),
			changed(onChange)
		{
		}

		RemoteClient& createConnection(std::string hostname,
									   const std::string& ip,
									   const int port)
		{
			performUniquenessCheck(hostname);
			return create(++_lastId,
						  hostname,
						  ip,
						  port,
						  clientChanged);
		}

		RemoteClient& createConnection(std::string hostname,
									   OscSender&& sender)
		{
			performUniquenessCheck(hostname);
			return create(++_lastId,
						  hostname,
						  std::move(sender),
						  clientChanged);
		}
};
