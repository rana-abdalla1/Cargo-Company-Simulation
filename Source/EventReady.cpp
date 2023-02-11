#include "EventReady.h"
#include "cargo.h"
void EventReady::Execute()
{
	cargo* pCargo = new cargo(cargoType, eventtime, cargoID, cargoDistance, loadingTime, cost,deadlineWait,AutoPTime); //Creatiion of Cargo
	ptrCompany->Prepare(pCargo, pCargo->getPriority());
}
EventReady::EventReady(Company* pCompany, char type, int day, int hour, int ID, int distance, int loading, int cargocost,int maxWait,int AutoP) : Event(pCompany)
{
	deadlineWait = new Time;
	AutoPTime = new Time;
	cargoID = ID;
	eventtime->setTime(day, hour);
	cargoDistance = distance;
	loadingTime = loading;
	cost = cargocost;
	cargoType = type;
	eventtime->AddWait(maxWait,deadlineWait);
	int AutoPHours = AutoP * 24;
	eventtime->AddWait(AutoPHours, AutoPTime);
}
/*
pEvent = new Event(........);
Waitingeventlist.enque(pEvent);

simulate:
eventlist.peek(pevent)
if ()
pEvent->execute(); //creating cargo, and call prepare pCompany-prepare();
*/

