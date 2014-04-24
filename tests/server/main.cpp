#include <iostream>
#include <net/session/MasterSession.h>
using namespace std;

int main()
{
	MasterSession session("Session Maitre", 5678);
	session.createGroup("The Group");

	while(true)
		std::this_thread::sleep_for(std::chrono::seconds(1));

	return 0;
}
