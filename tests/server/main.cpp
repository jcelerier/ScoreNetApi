#include <iostream>
#include <net/session/MasterSession.h>
#include <serverthread.h>
using namespace std;


int main()
{
	MyThread t;
	t.start();

	while(true)
		std::this_thread::sleep_for(std::chrono::seconds(1));

	return 0;
	////////

	MasterSession session("Session Maitre", 5678);
	session.createGroup("The Group");

	while(true)
		std::this_thread::sleep_for(std::chrono::seconds(1));

	return 0;
}
