#include <iostream>
#include <net/session/ClientSessionBuilder.h>

// Client sans Zeroconf
using namespace std;
int main()
{
	ClientSessionBuilder sessionBuilder(5678,
										"Session Maitre",
										"master",
										"127.0.0.1",
										9000,
										"DatClient");

	sessionBuilder.join();

	while(!sessionBuilder.isReady())
		std::this_thread::sleep_for(std::chrono::seconds(1));

	ClientSession session{sessionBuilder.getBuiltSession()};
	auto& g = *begin(session.groups());
	session.changePermission(session.getLocalClient(),
							 g,
							 Permission::Category::Write,
							 Permission::Enablement::Enabled);

	std::this_thread::sleep_for(std::chrono::seconds(1));
	return 0;
}
