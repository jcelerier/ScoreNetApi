#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QThread>

#include <net/session/ZeroConfClientSessionBuilder.h>
#include <thread>
class MyThread : public QThread
{
	Q_OBJECT

protected:
	void run()
	{
		ZeroConfClientSessionBuilder cb;

		while(1)
			std::this_thread::sleep_for(std::chrono::seconds(1));
	}
};

#endif // MYTHREAD_H
