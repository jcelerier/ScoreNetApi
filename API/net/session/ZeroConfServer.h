#pragma once

#include <QtNetwork>
#include <QDebug>
#include "../../../soft/dpetri/src/lib/zeroconf/bonjourserviceregister.h"
#include <QtNetwork/QTcpServer>

class ZeroconfServer : public QObject
{
		Q_OBJECT

		int _id;
		QString _sessionName;
		QString _machineName;
		quint16 _receiverPort;
	public:
		void setData(int id,
					 std::string sessionName,
					 std::string machineName,
					 int receiverPort)
		{
			_id = id;
			_sessionName = QString::fromStdString(sessionName);
			_machineName = QString::fromStdString(machineName);
			_receiverPort = receiverPort;
			qDebug() << _id << _sessionName << _machineName << _receiverPort;
		}

		ZeroconfServer():
			QObject()
		{

			tcpServer = new QTcpServer(this);
			bonjourRegister = new BonjourServiceRegister(this);

			if (!tcpServer->listen(QHostAddress::Any, 42591))
			{
				qDebug() << "Unable to start the server: "
						 << tcpServer->errorString();
				return;
			}

			connect(tcpServer, SIGNAL(newConnection()),
					this,	   SLOT(sendConnectionData()));

			bonjourRegister->registerService(BonjourRecord(tr("Distributed Score on %1").arg(QHostInfo::localHostName()),
														   QLatin1String("_score_net_api._tcp"),
														   QString()),
											 tcpServer->serverPort());

			qDebug() << "ZeroConf server listening on port : " << tcpServer->serverPort();
		}

		virtual ~ZeroconfServer() = default;

	private slots:
		void sendConnectionData()
		{
			QByteArray block;
			QDataStream out(&block, QIODevice::WriteOnly);
			QTcpSocket *clientConnection = tcpServer->nextPendingConnection();

			out.setVersion(QDataStream::Qt_5_2);
			// reserve space to write the size afterwards
			out << (quint16) 0;

			// write the data
			qDebug() << _id << _sessionName << _machineName;
			out << _id
				<< _sessionName
				<< _machineName
				<< clientConnection->localAddress()
				<< (quint16) _receiverPort;

			// write the size at the beginning
			out.device()->seek(0);
			out << (quint16)(block.size() - sizeof(quint16));

			connect(clientConnection, SIGNAL(disconnected()),
					clientConnection, SLOT(deleteLater()));

			clientConnection->write(block);
			clientConnection->disconnectFromHost();

			qDebug() << "Data sent";
		}

	private:
		QTcpServer *tcpServer;
		BonjourServiceRegister *bonjourRegister;
};
