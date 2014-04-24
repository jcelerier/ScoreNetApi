#pragma once
#include "ClientSession.h"


class ClientSessionBuilder : public hasId, public hasName
{
	public:
		// à mettre en privé, devrait être appelé uniquement par ::list()
		ClientSessionBuilder(int32_t id,
							 std::string sessionName,
							 std::string masterName,
							 std::string masterIp,
							 int masterInputPort,
							 std::string localName,
							 int localPort):
			hasId(id),
			hasName(sessionName),
			_session(new ClientSession(localName,
									   new RemoteMaster(0,
													   masterName,
													   masterIp,
													   masterInputPort),
									   new LocalClient(std::make_unique<OscReceiver>(localPort),
													   -1,
													   localName)))
		{
			_session->getLocalClient().receiver().addHandler("/session/clientNameIsTaken",
								 &ClientSessionBuilder::handle__session_clientNameIsTaken, this);

			_session->getLocalClient().receiver().addHandler("/session/setClientId",
								 &ClientSessionBuilder::handle__session_setClientId, this);
			_session->getLocalClient().receiver().addHandler("/session/update/group",
								 &ClientSessionBuilder::handle__session_update_group, this);

			_session->getLocalClient().receiver().addHandler("/session/isReady",
								 &ClientSessionBuilder::handle__session_isReady, this);

			_session->getLocalClient().receiver().run();
		}

		virtual ~ClientSessionBuilder() = default;
		ClientSessionBuilder(const ClientSessionBuilder& s) = default;
		ClientSessionBuilder(ClientSessionBuilder&& s) = default;

		/**** Handlers ****/
		// /session/update/group idSession idGroupe nomGroupe isMute
		void handle__session_update_group(osc::ReceivedMessageArgumentStream args)
		{
			osc::int32 idSession, idGroupe;
			bool isMute;
			const char* s;

			args >> idSession >> idGroupe >> s >> isMute >> osc::EndMessage;

			if(idSession == getId())
			{
				auto& g = _session->private__createGroup(idGroupe, std::string(s));
				isMute? g.mute() : g.unmute();
			}
		}

		// /session/isReady idSession
		void handle__session_isReady(osc::ReceivedMessageArgumentStream args)
		{
			osc::int32 idSession;

			args >> idSession >> osc::EndMessage;

			if(getId() == idSession) _isReady = true;
		}

		// /session/clientNameIsTaken idSession nom
		void handle__session_clientNameIsTaken(osc::ReceivedMessageArgumentStream args)
		{
			osc::int32 idSession;
			const char* cname;

			args >> idSession >> cname >> osc::EndMessage;
			std::string name(cname);

			if(idSession == getId() && _session->getLocalClient().getName() == name)
			{
				_session->getLocalClient().setName(_session->getLocalClient().getName() + "_");
				join();
			}
		}

		// /session/setClientId idSession idClient
		void handle__session_setClientId(osc::ReceivedMessageArgumentStream args)
		{
			osc::int32 idSession, idClient;

			args >> idSession >> idClient >> osc::EndMessage;

			if(idSession == getId())
			{
				LocalClient& lc = _session->getLocalClient();
				lc.setId(idClient);
			}
		}


		/**** Connection ****/
		// Si on veut, on récupère la liste des sessions dispo sur le réseau
		static std::vector<ClientSessionBuilder> list(std::string localName, int localPort)
		{
			std::vector<ClientSessionBuilder> v;

			v.emplace_back(3453525,
						   "Session Electro-pop",
						   "Machine du compositeur",
						   "127.0.0.1",
						   12345,
						   localName,
						   localPort);

			v.emplace_back(21145,
						   "Session test",
						   "Machine drôle",
						   "92.91.90.89",
						   8908,
						   localName,
						   localPort);

			return v;
		}


		// 2. On appelle "join" sur celle qu'on désire rejoindre.
		void join()
		{
			_session->_remoteMaster->send("/session/connect" ,
								getId(),
								_session->getLocalClient().getName().c_str(),
								_session->getLocalClient().localPort());

			// Envoyer message de connection au serveur.
			// Il va construire peu à peu session.
			//
			// Eventuellement :
			//	/session/clientNameIsTaken
			//
			// Puis :
			//	/session/setClientId
			//	/session/update/group
			//
			// Enfin :
			//	/session/isReady
		}

		// 3. On teste si la construction est terminée
		// Construction : Envoyer tous les groupes.
		bool isReady()
		{
			return _isReady;
		}

		// 4. A appeler uniquement quand isReady
		std::unique_ptr<ClientSession>&& getBuiltSession()
		{
			if(!_isReady) throw "Is not ready";

			// Construction de l'objet Session.
			_session->setId(getId());
			_session->setName(getName());

			return std::move(_session);
		}

	private:
		std::unique_ptr<ClientSession> _session{nullptr};

		bool _isReady = false;
};
