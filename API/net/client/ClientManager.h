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

		RemoteClient& createConnection(int id,
									   std::string hostname,
									   std::string ip,
									   int port)
		{
			performUniquenessCheck(hostname);

			// Les ID partent de 1 (server = 0)
			return create(id,
						  hostname,
						  ip,
						  port,
						  clientChanged);
		}

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
			return createConnection((hostname == "server")? 0 : ++_lastId,
									hostname,
									ip,
									port);
		}
};
