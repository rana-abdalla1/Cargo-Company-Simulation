#pragma once
#include "Event.h"
#include "Company.h"
class Company;
class Event;
class EventPromotion : public Event
{
	int extraMoney;
public:
	virtual void Execute();
	EventPromotion(Company* pCompany, int ID, int day, int hour, int promotecost);
};

