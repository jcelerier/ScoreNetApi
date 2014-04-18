#pragma once
#include "Client.h"

class LocalClient : public Client
{
	using SignalHandler = std::function<void()>;

	public:
		using Client::Client;
};

class MasterClient : public LocalClient
{
	public:
		using LocalClient::LocalClient;

};
