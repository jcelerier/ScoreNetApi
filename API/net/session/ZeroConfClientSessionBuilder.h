#pragma once

#include <QTcpSocket>
#include <../../../soft/dpetri/src/lib/zeroconf/bonjourrecord.h>
#include <../../../soft/dpetri/src/lib/zeroconf/bonjourservicebrowser.h>
#include <../../../soft/dpetri/src/lib/zeroconf/bonjourserviceresolver.h>
#include <QtNetwork>
#include <QtNetwork/QHostAddress>
#include <QString>
#include <iostream>
class ZeroConfClientSessionBuilder: public QObject
{
	Q_OBJECT

public:
		virtual ~ZeroConfClientSessionBuilder() = default;
		ZeroConfClientSessionBuilder():
			QObject()
		{
			// Chargé de recevoir les messages ensuite
			tcpSocket = new QTcpSocket(this);

			connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(readConnectionData()));
			connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
					this, SLOT(displayError(QAbstractSocket::SocketError)));

			// Chargé de regarder les sessions ouvertes
			bonjourBrowser = new BonjourServiceBrowser(this);

			connect(bonjourBrowser, SIGNAL(currentBonjourRecordsChanged(const QList<BonjourRecord> &)),
					this, SLOT(updateRecords(const QList<BonjourRecord> &)));
			qDebug() << "1";
			bonjourBrowser->browseForServiceType(QLatin1String("_dpetriserver._tcp"));
			qDebug() << "2";
		}

	signals:
	void setLocalAddress(QHostAddress);
	void connectedTo(QHostAddress, quint16);

private slots:
	void updateRecords(const QList<BonjourRecord> &list)
	{
		qDebug() << "Hallo!";
//		for(const BonjourRecord& record : list)
		{
//			qDebug() << "Detected a record!: " ;//<< record.serviceName() << std::endl;
		}
	}
	/*
	void connectTo()
	{
		blockSize = 0;
		tcpSocket->abort();

		QList<QTreeWidgetItem *> selectedItems = treeWidget->selectedItems();

		if (selectedItems.isEmpty())
			return;

		if (!bonjourResolver)
		{
			bonjourResolver = new BonjourServiceResolver(this);
			connect(bonjourResolver, SIGNAL(bonjourRecordResolved(const QHostInfo &, int)),
					this, SLOT(connectToServer(const QHostInfo &, int)));
		}

		QTreeWidgetItem *item = selectedItems.at(0);
		QVariant variant = item->data(0, Qt::UserRole);
		bonjourResolver->resolveBonjourRecord(variant.value<BonjourRecord>());

		close();
	}
	*/
	void readConnectionData()
	{
		QDataStream in(tcpSocket);
		in.setVersion(QDataStream::Qt_5_2);

		if (blockSize == 0)
		{
			if (tcpSocket->bytesAvailable() < (int)sizeof(quint16))
				return;

			in >> blockSize;
		}

		if (tcpSocket->bytesAvailable() < blockSize)
			return;

		quint16 port;
		QHostAddress ip;
		in >> ip >> port;


	}
	void displayError(QAbstractSocket::SocketError socketError)
	{
		switch (socketError)
		{
			case QAbstractSocket::RemoteHostClosedError:
				break;
			case QAbstractSocket::HostNotFoundError:
				std::cerr << "Host not found" << std::endl;
				break;
			case QAbstractSocket::ConnectionRefusedError:
				std::cerr << "Connection refused" << std::endl;
				break;
			default:
				qDebug() << tcpSocket->errorString();
		}
	}
	void connectToServer(const QHostInfo &hostInfo, int port)
	{
		const QList<QHostAddress> &addresses = hostInfo.addresses();
		if (!addresses.isEmpty())
			tcpSocket->connectToHost(addresses.first(), port);
	}

private:
	QTcpSocket *tcpSocket = nullptr;
	quint16 blockSize;
	BonjourServiceBrowser *bonjourBrowser = nullptr;
	BonjourServiceResolver *bonjourResolver = nullptr;
};