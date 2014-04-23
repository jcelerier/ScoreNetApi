#pragma once
#include "Client.h"

class LocalClient : public Client
{
	friend class ClientSessionBuilder;
	using SignalHandler = std::function<void()>;

	public:
		using Client::Client;

		int localPort() const
		{
			return _receiver.port();
		}

		void setLocalPort(unsigned int c)
		{
			_receiver.setPort(c);
		}

	protected:
		OscReceiver _receiver{0};
};
