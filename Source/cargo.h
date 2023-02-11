#pragma once
#include "Time.h"
#include"Truck.h"
#include <fstream>
#include <stdio.h>
using namespace std;
class Truck;
class Time;
class cargo
{
private:
	Time* prepTime;
	Time* deadlineWait;
	Time* AutoPTime;
	Time* MoveTime;
	Time* loadstarttime;
	char CargoType;
	float DeliveryDistance;
	float Cost;
	int LoadingTime;
	int ID;
	float priority;
	Time* CargoDT;
	Truck* pTruck=nullptr;
public:
	cargo(char cargoType, Time *readytime, int cargo_ID, int deliveryDistance, int loadingTime, int cost,Time *deadlinewait, Time *AutoPtime);
	int getID();
	float getPriority();
	float getDistance();
	char getType();
	int getloadtime();
	bool checkmaxwait(Time*currTime);
	bool checkAutoPTime(Time* currTime);
	bool operator ==(int checkid); //overloading assignment operator in order to compare based on id
	void settype(char x);
	Time* GetMoveTime();
	void SetCargoDT(Truck* truck, Time* time, int cumulativeUnloadtime);
	Time* getCDT();
	void setTruck(Truck* currTruck);
	void Save(ofstream& myfile, float& totalwaittime);
	Time* getWaitTime();
	void setloadfinishtime(int time);
	Time* getloadstarttime();
	void incrementfinishload();
};

