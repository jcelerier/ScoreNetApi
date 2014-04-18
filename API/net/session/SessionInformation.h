#pragma once
#include "Session.h"


class ClientSessionBuilder : public hasName
{
	public:
		// à mettre en privé, devrait être appelé uniquement par ::list()
		ClientSessionBuilder(std::string sessionName,
						   std::string masterName,
						   std::string masterIp,
						   int masterInputPort):
			hasName(sessionName),
			_masterClient(new MasterClient(0, masterName, masterIp, masterInputPort, std::bind(&ClientSessionBuilder::clientChanged, this, std::placeholders::_1)))
		{

		}

		virtual ~ClientSessionBuilder() = default;
		ClientSessionBuilder(const ClientSessionBuilder& s) = default;
		ClientSessionBuilder(ClientSessionBuilder&& s) = default;

		// 1. On récupère la liste des sessions dispo sur le réseau
		static std::vector<ClientSessionBuilder> list()
		{
			std::vector<ClientSessionBuilder> v;

			v.emplace_back("Session Electro-pop",
						   "Machine du compositeur",
						   "127.0.0.1",
						   12345);

			v.emplace_back("Session test",
						   "Machine drôle",
						   "0.1.2.3",
						   8908);

			return v;
		}

		// 2. On appelle "join" sur celle qu'on désire rejoindre.
		void join()
		{
			// Envoyer message de connection au serveur.
			// Il va construire peu à peu session.
			// D'abord : /session/update/addGroup *infos du groupe*
			// Puis : /session/isReady numSession
		}

		// 3. On teste si la construction est terminée
		// Construction : Envoyer tous les groupes.
		bool isReady()
		{
			return _isReady;
		}

		// 4. A appeler uniquement quand isReady
		ClientSession builtSession()
		{
			if(!_isReady) throw "Is not ready";
			session._masterClient = std::move(_masterClient);
			return std::move(session);
		}

	private:
		std::unique_ptr<MasterClient> _masterClient;
		ClientSession session;
		bool _isReady = false;
};
