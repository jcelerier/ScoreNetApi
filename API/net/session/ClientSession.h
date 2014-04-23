#pragma once
#include "Session.h"
#include "../client/LocalClient.h"

class ClientSession : public Session
{
		friend class ClientSessionBuilder;
	public:
		using Session::Session;

		virtual Client& getLocalClient() override
		{
			return *_localClient;
		}

	private:
		std::unique_ptr<LocalClient> _localClient;
};

