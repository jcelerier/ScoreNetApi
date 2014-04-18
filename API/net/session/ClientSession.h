#pragma once
#include "Session.h"
class ClientSession : public Session
{
	public:
		ClientSession() = default;
		ClientSession(ClientSession&&) = default;
		ClientSession(const ClientSession&) = default;



		virtual Client& getLocalClient() override
		{
			return *_localClient;
		}

	private:
		std::unique_ptr<LocalClient> _localClient;
};

