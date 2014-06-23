#pragma once

#include <pnapi/pnapi.h>
#include <set>
#include <../../soft/dpetri/src/lib/simpleNet/simplePetriNet.h>


class NetworkNode { };
class DeportedTransition : public SimpleTransition
{
	public:
		using SimpleTransition::SimpleTransition;

		NetworkNode node;
		bool isAlpha{false};
		bool isBeta{false};

		DeportedTransition* creeAlpha()
		{
			DeportedTransition * alpha;
			return alpha;
		}
};

using namespace pnapi;

std::set<DeportedTransition*> previousTransitions(DeportedTransition* t)
{
	std::set<DeportedTransition*> prev_transitions;
	for(Place * p : t->getPreset())
	{
		for(DeportedTransition* t : p->getPreset())
		{
			prev_transitions.insert(t);
		}
	}

	return prev_transitions;
}

void displace(SimplePetriNet* net, DeportedTransition* s, NetworkNode nd)
{
	for(DeportedTransition* p : previousTransitions(t))
	{
		if(p->node == s->node && p->node == nd)
		{
			continue;
		}
		else if(p->node == s->node && p->node != nd)
		{
			auto pp{previousTransitions(p)};
			DeportedTransition* alphatrans{nullptr};
			if(std::any_of(pp.begin(), pp.end(), [] (DeportedTransition* dt){return dt->isAlpha;}))
			{
				alphatrans = *std::find_if(pp.begin(), pp.end(), [] (DeportedTransition* dt){return dt->isAlpha;});
			}
			else
			{
				alphatrans = p->creeAlpha();
			}

			DeportedTransition* betatrans{alphatrans->creeBeta()};
			for(SimplePlace* pl : s->getPreset())
			{
				for(SimpleArc* arc : net->getArcs())
				{
					if(arc->getTargetNode() == pl) arc->clear();
				}

				net->createArc(*betatrans, *s);
			}


		}
		else if(p->node != s->node && p->node == nd)
		{

		}
		else if(p->node != s->node)
		{

		}
	}

}
