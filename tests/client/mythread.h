#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QThread>

#include <net/session/ZeroConfClientSessionBuilder.h>
#include <thread>
class MyThread : public QThread
{
		Q_OBJECT
	public:
		virtual ~MyThread()
		{
			delete _cb;
		}

	protected:
		void run()
		{
			_cb = new ZeroConfClientSessionBuilder;
			connect(_cb, &ZeroConfClientSessionBuilder::recordsChanged,
					this, &MyThread::tryConnect);

			exec();
		}

	private slots:
		void tryConnect()
		{
			auto lst = _cb->getRecords();
			if(lst.size() > 0)
			{
				_cb->connectTo(lst[0]);
			}
		}

	private:
		ZeroConfClientSessionBuilder* _cb;
};

#endif // MYTHREAD_H
