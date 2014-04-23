#include "net/Iterable.h"
#include "net/client/ClientManager.h"
#include "net/permission/PermissionManager.h"
#include "net/session/Session.h"
#include "net/session/ClientSessionBuilder.h"
#include "net/session/MasterSession.h"
#include "net/DistributedScenario.h"
#include <vector>
#include <iostream>

using namespace std;
int main()
{
	auto sessionList = ClientSessionBuilder::list("localClt");
	sessionList[0].join();

	MasterSession s("Ma Session");
	std::cerr << s.getId() << std::endl;

	s.createGroup("Mon groupe");
	s.createGroup("Mon autre groupe");

	s.createClient("ordi1", "1.1.1.1", 111);
	s.createClient("ordi2", "2.2.2.2", 222);

	s.changePermission(s.client("ordi1"),
					   s.group("Mon groupe"),
					   Permission::Category::Read, Permission::Enablement::Enabled);

	DistributedScenario myScenar(new Scenario);
	myScenar.assignToGroup(s.group("Mon groupe"));
	myScenar.removeFromGroup();


	std::cerr << s.getPermission(s.client(1),
								 s.group(0),
								 Permission::Category::Read) << std::endl;
	std::cerr << s.getPermission(s.client(1),
								 s.group(0),
								 Permission::Category::Execute) << std::endl;

	for(auto& g : s.groups()) std::cerr << g.getName() << std::endl;

	s.removeGroup(0);
	for(auto& g : s.groups()) std::cerr << g.getName() << std::endl;
	s.removeClient("ordi1");

	s.createClient("ordi1", "1.1.1.1", 111);

	return 0;
}
