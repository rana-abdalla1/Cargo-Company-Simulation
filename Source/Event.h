#pragma once
//#include "Company.h"
#include "Time.h"
class Company;
class Time;
class Event
{
protected:
	Time* eventtime;
	int cargoID;
	Company* ptrCompany;
public:
	Event(Company* pCompany);
	virtual void Execute() = 0;
	void getEventtime(Time* &T1);
};

