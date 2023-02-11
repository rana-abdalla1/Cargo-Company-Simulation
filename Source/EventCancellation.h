#pragma once
#include "Event.h"
#include "Company.h"
class Company;
class Event;
class EventCancellation :  public Event
{
public:
	EventCancellation(Company* pCompany,int day,int hour,int ID);
	virtual void Execute();
};

