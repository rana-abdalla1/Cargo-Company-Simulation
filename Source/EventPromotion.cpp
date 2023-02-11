#include "EventPromotion.h"
void EventPromotion::Execute()
{
	ptrCompany->Promote(cargoID);
}

EventPromotion::EventPromotion(Company* pCompany, int day, int hour, int ID, int promotecost) : Event(pCompany)
{
	eventtime->setTime(day, hour);
	cargoID = ID;
	extraMoney = promotecost;
}
