#include <iostream>
#include <net/session/MasterSession.h>
#include <net/session/ZeroConfServerThread.h>
#include <QCoreApplication>

#include <net/DistributedScenario.h>
using namespace std;


int main(int argc, char** argv)
{
	QCoreApplication app(argc, argv);

	MasterSession session("Session Maitre", 5678);
	auto& ag = session.createGroup("Audio Group");
	DistributedScenario sAudio(new Scenario);
	sAudio.assignToGroup(ag);
	auto& vg = session.createGroup("Video Group");
	DistributedScenario sVideo(new Scenario);
	sVideo.assignToGroup(vg);

	return app.exec();
	////////

	return 0;
}
