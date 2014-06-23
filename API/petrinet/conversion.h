#pragma once
#include "../../soft/dpetri/src/lib/simpleNet/simplePetriNet.h"
#include "../Scenario.h"
#include "../TimeBox.h"
#include "../Event.h"
#include "../TimeValue.h"
#include <utility>
// Converts a Scenario to a Petri Net.
using namespace OSSIA;
inline SimplePetriNet convertScenario(const Scenario& s)
{
	SimplePetriNet net;

	auto& init_trans = net.createTransition("init_t");
	net.createArc(net.createPlace("init"), init_trans);
	auto& end_trans = net.createTransition("end_t");
	net.createArc(end_trans, net.createPlace("end"));
	// Pour chaque timebox, créer une transition.
	// Puis pour chaque evt. créer une transition.
	// Puis relier : quand un evt est au début / fin d'une timebox, créer une place commune
	// aux deux, et créer arcs entre place et transitions.
	// Enfin, relier transitions sans entrée / sortie au début et à la fin.

	// Puis rajouter cas récursif de la hiérarchie
	std::vector<std::pair<SimpleTransition*, TimeNode*>> trans_events;
	std::vector<std::pair<SimpleTransition*, TimeBox*>> trans_boxes;
	int nodeid{}, timeboxid{}, placeid{};

	for(TimeNode* node : s.getTimeNodes())
	{
		auto& trans = net.createTransition(std::string("node_t__") + std::to_string(nodeid++));
		trans_events.emplace_back(&trans, node);
	}
	for(TimeBox* box : s.getTimeBoxes())
	{
		auto& trans = net.createTransition(std::string("box_t__") + std::to_string(timeboxid++));
		trans_boxes.emplace_back(&trans, box);

		trans.setCost(box->getLength());

		for(auto& tr_evt_pair : trans_events)
		{
			if(tr_evt_pair.second == &box->getStartEvent())
			{
				auto& p = net.createPlace(std::string("place__") + std::to_string(placeid++));
				net.createArc(*tr_evt_pair.first, p);
				net.createArc(p, trans);
			}
			else if(tr_evt_pair.second == &box->getEndEvent())
			{
				auto& p = net.createPlace(std::string("place__") + std::to_string(placeid++));
				net.createArc(trans, p);
				net.createArc(p, *tr_evt_pair.first);
			}
		}
	}

	for(auto& tr_evt_pair : trans_events)
	{
		if(tr_evt_pair.first->getPreset().empty())
		{
			auto& p = net.createPlace(std::string("place__") + std::to_string(placeid++));
			net.createArc(init_trans, p);
			net.createArc(p, *tr_evt_pair.first);
		}
		if(tr_evt_pair.first->getPostset().empty())
		{
			auto& p = net.createPlace(std::string("place__") + std::to_string(placeid++));
			net.createArc(*tr_evt_pair.first, p);
			net.createArc(p, end_trans);
		}
	}

	return net;
}
