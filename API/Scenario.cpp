#include "Scenario.h"
using namespace OSSIA;

Scenario::Scenario()
{

}

Scenario::Scenario(const Scenario&)
{

}

Scenario::~Scenario()
{

}

Scenario&Scenario::operator=(const Scenario&)
{

}

void Scenario::play() const
{

}

std::set<TimeBox*> Scenario::getTimeBoxes() const
{
	return {};
}

std::set<TimeNode*> Scenario::getTimeNodes() const
{
	return {};
}

void Scenario::addTimeBox(const TimeBox&, const Event& startEvent)
{

}

void Scenario::addTimeBox(const TimeBox&, const Event& startEvent, const Event& endEvent)
{

}

Event&Scenario::getStartEvent() const
{

}

void Scenario::setStartEvent(const Event&)
{

}

Event&Scenario::getEndEvent() const
{

}

void Scenario::setEndEvent(const Event&)
{

}
