#include "EventCancellation.h"

void EventCancellation::Execute()
{
	ptrCompany->Cancel(cargoID);
}
EventCancellation::EventCancellation(Company* pCompany, int day, int hour, int ID) : Event(pCompany)
{
	cargoID = ID;
	eventtime->setTime(day, hour);
}
