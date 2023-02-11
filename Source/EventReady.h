#pragma once
#include "Event.h"
#include "Company.h"

class Company;
class Event;
class EventReady : public Event
{
	char cargoType;
	int cargoDistance;
	int loadingTime;
	int cost;
	Time* deadlineWait;
	Time* AutoPTime;
public:
	EventReady(Company * pCompany, char type, int day, int hour, int ID, int distance, int loading, int cargocost,int maxWait,int AutoP);
	virtual void Execute();

};

