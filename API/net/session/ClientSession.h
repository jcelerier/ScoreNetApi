#pragma once
#include "Session.h"
#include "../client/LocalClient.h"

class ClientSession : public Session
{
		friend class ClientSessionBuilder;
	public:
		using Session::Session;
		virtual ~ClientSession() = default;
		ClientSession(ClientSession&&) = default;

		virtual Client& getClient() override
		{
			return *_localClient;
		}

		LocalClient& getLocalClient()
		{
			return *_localClient;
		}

	private:
		std::unique_ptr<LocalClient> _localClient{nullptr};
};

