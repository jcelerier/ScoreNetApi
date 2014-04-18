#pragma once
#include "Client.h"

class RemoteClient : public Client
{
	using SignalHandler = std::function<void()>;

	public:
		using Client::Client;
		RemoteClient(RemoteClient&&) = default;
		RemoteClient(const RemoteClient&) = default;
		RemoteClient& operator=(const RemoteClient&) = default;
		RemoteClient& operator=(RemoteClient&&) = default;

		void pollDelay();
		int getDelay();

	private:
		int delayInMs; // ns ?

};
