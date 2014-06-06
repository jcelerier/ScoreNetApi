#pragma once
#include <thread>
#include <QThread>

#include <net/session/ZeroConfServer.h>
class MyThread : public QThread
{
	Q_OBJECT

protected:
	void run()
	{
		ZeroconfServer serv;

		while(1)
			std::this_thread::sleep_for(std::chrono::seconds(1));
	}
};
