#include "Truck.h"
#include"iostream"
class cargo;
using namespace std;
Truck::Truck(char type, int Capacity, int time, float speed, int ID)
{
	TruckType = type;
	TruckCapacity = Capacity;
	MaintenanceTime = time;
	Speed = speed;
	truckID = ID;

}
void Truck::SetTruckType(char type)
{
	TruckType = type;
}
void Truck::SetTruckCapacity(int cap)
{
	TruckCapacity = cap;
}
void Truck::setMaintenanceTime(float time)
{
	MaintenanceTime = time;
}
void Truck::setSpeed(float s)
{
	Speed = s;
}
void Truck::setDeliveryInterval(float interval)
{
	DeliveryInterval = interval;
}
void Truck::AddCargo(cargo* pCargo) //Adds one cargo at a time
{
	if (pCargo != nullptr)
	{
		Cargos.enqueue(pCargo, pCargo->getDistance());
		totalCargosCount++;
	}
}

void Truck::DeleteCargo(cargo*& pCargo)
{
	Cargos.dequeue(pCargo);
}

char Truck::getType()
{
	return TruckType;
}
int Truck::getID()
{
	return truckID;
}
int Truck::getTruckCapacity()
{
	return TruckCapacity;
}
float Truck::getMaintenanceTime()
{
	return MaintenanceTime;
}
float Truck::getSpeed()
{
	return Speed;
}
float Truck::getDeliveryInterval()
{
	return DeliveryInterval;
}

void Truck::setpriority(int x)
{
	checkuppriority = x;
	loadingpriority = x;
}

int* Truck::getcargosid(Time* time, int& countarr)
{
	int count = Cargos.getCount();
	cargo* pcargo = nullptr;
	int* arr = new int[count];
	PriorityQueue<cargo*>temp;
	for (int i = 0; i < count; i++)
	{
		Cargos.dequeue(pcargo);
		if (time->getHour() < 5)
		{
			pcargo->incrementfinishload();
		}
		if (pcargo->getloadstarttime()->CompareGreaterTimes(time))
		{
			arr[countarr] = pcargo->getID();
			countarr++;
		}
		temp.enqueue(pcargo, pcargo->getDistance());
	}
	for (int i = 0; i < count; i++)
	{
		temp.dequeue(pcargo);
		Cargos.enqueue(pcargo, pcargo->getDistance());
	}
	return arr;
}

int Truck::getcargocount()
{
	return Cargos.getCount();
}

char Truck::getcargotype()
{
	cargo* pcargo = nullptr;
	Cargos.peek(pcargo);
	if (pcargo)
	{
		return pcargo->getType();
	}
	return 'X';
}

int Truck::getcheckuppriority()
{
	return checkuppriority;
}

int Truck::getloadingpriority()
{
	return loadingpriority;
}
float Truck::movingpriority(Time* time)
{
	int unloadtimetotal = 0;
	cargo* pcargo;
	Cargos.peek(pcargo);
	long int maxdistance = pcargo->getDistance();
	int count = Cargos.getCount();
	PriorityQueue<cargo*>temp;
	for (int i = 0; i < count; i++)
	{
		Cargos.dequeue(pcargo);
		unloadtimetotal = unloadtimetotal + pcargo->getloadtime();
		Truck* ptruck = this;
		pcargo->SetCargoDT(ptruck, time, unloadtimetotal);
		if (pcargo->getDistance() > maxdistance)
		{
			maxdistance = pcargo->getDistance();
		}
		temp.enqueue(pcargo, pcargo->getDistance());
	}
	for (int i = 0; i < count; i++)
	{
		temp.dequeue(pcargo);
		Cargos.enqueue(pcargo, pcargo->getDistance());
	}
	movingPriority = (2 * (float(maxdistance) / float(Speed))) + unloadtimetotal + time->timetoint();
	activetime = activetime + (float(maxdistance) / float(Speed)) + float(unloadtimetotal * 2);
	return movingPriority;
}

float Truck::getmovingpriority()
{
	return movingPriority;
}

void Truck::incrementcheckup()
{
	totalnumjourney++;
	checkupcount++;
}

int Truck::getcheckupcount()
{
	return  checkupcount;
}

void Truck::resetcheckup()
{
	totalnumjourney++;
	checkupcount = 1;
}

int Truck::getcheckupend()
{
	return Checkupend;
}

bool Truck::checkCargoDelivery(Time* time, cargo*& pCargo)
{
	pCargo = nullptr;
	Cargos.peek(pCargo); //to check if the top of the list is ready to be delivered
	if (pCargo != nullptr)
	{
		Time* CDT = pCargo->getCDT();
		if (CDT->CompareGreaterTimes(time))
		{
			Cargos.dequeue(pCargo); //to return the cargo to delivered list in company
			return true;
		}
		return false;
	}
	return false;
}

void Truck::checkupend(Time* time, int checkupduration)
{
	Checkupend = time->getDay() * 24 + time->getHour() + checkupduration;
}


float Truck::getactiveTime()
{
	return activetime;
}

int Truck::getTotalCargosCount()
{
	return totalCargosCount;
}

float Truck::getUtilizationTime(int totalsimtime)
{
	if (totalnumjourney == 0 || activetime == 0)
		return 0;
	float utilizationtime = ((float)totalCargosCount / ((float)TruckCapacity * totalnumjourney)) * ((float)activetime / totalsimtime);
	return utilizationtime; //Truck Utilization Equation
}
