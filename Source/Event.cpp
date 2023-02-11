#include "Event.h"

Event::Event(Company* pCompany)
{
	ptrCompany=pCompany;
	eventtime = new Time;
}

void Event::getEventtime(Time*& T1)
{
	T1 = eventtime;
}