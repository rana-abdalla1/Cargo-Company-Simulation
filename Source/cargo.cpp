#include "cargo.h"
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <iomanip>
using namespace std;
class Truck;
using namespace std;
cargo::cargo(char cargoType, Time* readytime, int cargo_ID, int deliveryDistance, int loadingTime, int cost, Time* deadlinewait, Time* AutoPtime)
{
	prepTime = readytime;
	deadlineWait = deadlinewait;
	if (cargoType == 'N')
	{
		AutoPTime = AutoPtime;
	}
	else
	{
		AutoPTime = nullptr;
	}
	CargoType = cargoType;
	ID = cargo_ID;
	DeliveryDistance = deliveryDistance;
	Cost = cost;
	LoadingTime = loadingTime;
}

int cargo::getID()
{
	return ID;
}

float cargo::getPriority()
{
	if (priority == 0)
	{
		return priority;
	}
	priority = pow((prepTime->getDay() * 24 + prepTime->getHour()), 0.8) / (pow(Cost, 2) * pow(DeliveryDistance, 1.3));
	return priority;
}

float cargo::getDistance()
{
	return DeliveryDistance;
}

char cargo::getType()
{
	return CargoType;
}

int cargo::getloadtime()
{
	return LoadingTime;
}

bool cargo::checkmaxwait(Time* currTime)
{
	if (deadlineWait->CompareGreaterTimes(currTime))
	{
		if (CargoType == 'V')
		{

			return false;
		}
		return true;
	}
	return false;
}
bool cargo::checkAutoPTime(Time* currTime)
{
	if (AutoPTime->CompareGreaterTimes(currTime))
	{
		return true;
	}
	return false;
}

bool cargo::operator==(int checkid)
{

	if (ID == checkid)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void cargo::settype(char x)
{
	CargoType = x;
}

Time* cargo::GetMoveTime()
{
	return MoveTime;
}

void cargo::SetCargoDT(Truck* truck, Time* time, int cumulativeUnloadtime)
{
	Time* temp = new Time(time->getDay(), time->getHour());
	pTruck = truck;
	MoveTime = temp;
	int speed = pTruck->getSpeed();
	int totaltime = ceil(float(DeliveryDistance) / float(speed)) + cumulativeUnloadtime + MoveTime->timetoint();
	CargoDT = new Time;
	CargoDT->intToTime(totaltime);
}

Time* cargo::getCDT()
{
	return CargoDT;
}
void cargo::setTruck(Truck* currTruck)
{
	pTruck = currTruck;
}
Time* cargo::getWaitTime()
{
	Time* waitTime;
	waitTime = prepTime->subtractTimes(MoveTime);
	return waitTime;
}

void cargo::setloadfinishtime(int time) //to be able to show cargos one by one
{
	loadstarttime = new Time;
	loadstarttime->intToTime(time);
}

Time* cargo::getloadstarttime()
{
	return loadstarttime;
}
void cargo::incrementfinishload()
{
	int time = loadstarttime->timetoint() + 1;
	loadstarttime->intToTime(time);
}

void cargo::Save(ofstream& myfile, float& totalwaittime)
{
	Time* waitTime;
	myfile << setw(2) << (CargoDT->getDay()) << setw(1) << ":" << setw(2) << (CargoDT->getHour()) << setw(5);
	myfile << ID << setw(5) << prepTime->getDay() << setw(1) << ":" << setw(2) << prepTime->getHour() << setw(5);
	waitTime = getWaitTime();
	totalwaittime += waitTime->timetoint();
	myfile << waitTime->getDay() << setw(1) << ":" << setw(2) << waitTime->getHour() << setw(5);
	myfile << pTruck->getID() << endl;
	delete waitTime;
}



