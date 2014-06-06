#ifndef PNSCENARIO_H
#define PNSCENARIO_H

#include "../Scenario.h"
#include "../../soft/dpetri/src/lib/simpleNet/simplePetriNet.h"
#include "PNTimeBox.h"
#include "PNTimeNode.h"
#include <functional>


using namespace OSSIA;
class PNScenario : public Scenario
{
	public:
		PNScenario();

		void setTimeBoxPolicy(std::function<void()>);
		PNTimeBox createTimeBox(int position, int duration)
		{
			/* Créer branche qui va du début à la box et de la box à la fin */
		}

	private:
		SimplePetriNet net;
};

#endif // PNSCENARIO_H
