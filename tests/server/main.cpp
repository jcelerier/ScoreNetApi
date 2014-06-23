#include <iostream>
#include <net/session/MasterSession.h>
#include <net/session/ZeroConfServerThread.h>
#include <QCoreApplication>
using namespace std;


int main(int argc, char** argv)
{
	QCoreApplication app(argc, argv);
	MasterSession zcsession("Session Maitre", 5678);

	return app.exec();
	////////

	MasterSession session("Session Maitre", 5678);
	session.createGroup("The Group");

	while(true)
		std::this_thread::sleep_for(std::chrono::seconds(1));

	return 0;
}
