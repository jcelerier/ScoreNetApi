#include <iostream>
#include <net/session/ClientSessionBuilder.h>
#include <mythread.h>

// Client sans Zeroconf



using namespace std;
int main()
{
	MyThread t;
	t.start();
	while(1)
		std::this_thread::sleep_for(std::chrono::seconds(1));

	return 0;

	//////////////////////////////////////////////////////

	ClientSessionBuilder sessionBuilder(5678,
										"Session Maitre",
										"master",
										"127.0.0.1",
										9000,
										"DatClient",
										8900);

	sessionBuilder.join();

	while(!sessionBuilder.isReady())
		std::this_thread::sleep_for(std::chrono::seconds(1));

	std::unique_ptr<ClientSession> session{sessionBuilder.getBuiltSession()};
	auto& g = *begin(session->groups());
	session->changePermission(session->getLocalClient(),
							 g,
							 Permission::Category::Write,
							 Permission::Enablement::Enabled);

	std::this_thread::sleep_for(std::chrono::seconds(1));

	///////////////////////////////////////////////////////

	ClientSessionBuilder sessionBuilder2(5678,
										"Session Maitre",
										"master",
										"127.0.0.1",
										9000,
										"OtherClient",
										 8901);

	sessionBuilder2.join();

	while(!sessionBuilder2.isReady())
		std::this_thread::sleep_for(std::chrono::seconds(1));

	std::unique_ptr<ClientSession> session2{sessionBuilder2.getBuiltSession()};
	auto& g2 = *begin(session2->groups());
	session2->changePermission(session2->getLocalClient(),
							 g2,
							 Permission::Category::Read,
							 Permission::Enablement::Enabled);

	std::this_thread::sleep_for(std::chrono::seconds(2));
	session2.reset();
	std::this_thread::sleep_for(std::chrono::seconds(2));

	return 0;
}
