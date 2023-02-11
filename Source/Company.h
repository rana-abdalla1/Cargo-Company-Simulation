#pragma once

#include "NormalQueue.h"
#include "Event.h"
#include "Truck.h"
#include "cargo.h"
#include "Priority.h"
#include "LinkedListCargo.h"
#include "EventCancellation.h"
#include "EventPromotion.h"
#include "EventReady.h"
#include "UI.h"
#include <string>
#include <fstream>
#include"Time.h"
class Truck;
class Event;
class UI;
class Company
{
	UI* ptrUI;
	LinkedQueue <Event*> EventList;
	LinkedQueue <Truck*> WaitingVIPTrucks;//waiting truckss for each type
	LinkedQueue <Truck*> WaitingNormalTrucks;
	LinkedQueue <Truck*> WaitingSpecialTrucks;
	//	PriorityQueue<Truck*>LoadingTrucks;
	PriorityQueue<Truck*>MovingTrucks; 	//Moving trucks list for all
	LinkedQueue <Truck*> CheckUpSpecialTrucks;	//Checkup trucks list for each type
	LinkedQueue <Truck*> CheckUpNormalTrucks;
	LinkedQueue <Truck*> CheckUpVIPTrucks;
	LinkedQueue<cargo*>WaitingSpecialCargos;//Waiting cargos for each type 
	PriorityQueue<cargo*>WaitingVIPCargos;
	LinkedListCargo WaitingNormalCargos;
	LinkedQueue<cargo*>DeliveredCargos;
	int normalTrucks;
	int VIPTrucks;
	int specialTrucks;
	int VIPSpeed;
	int specialSpeed;
	int normalSpeed;
	int normalCap;
	int VIPCap;
	int specialCap;
	int CheckNormal;
	int CheckSpecial;
	int CheckVIP;
	int JourneyNum;
	int eventsNumber;
	int autoP;
	int maxWait;
	int endindicator = 0;
	bool maxwait=false;
	Time* currtime;
	Truck* LoadingVIP = nullptr;   //max loading trucks=3
	Truck* LoadingSpecial = nullptr; //point to truck loading normal cargos
	Truck* LoadingNormal = nullptr;
	int loadingtimenormal;
	int loadingtimespecial;
	int loadingtimevip;
	int movingcargocount=0;
	int numofautop = 0; //to calculate autop percentage
public:
	Company();
	void FileLoad(string fileName);
	bool Prepare(cargo* pCargo, float priority);
	void Simulate(string fileName);
	bool Cancel(int ID);
	bool Promote(int ID);
	void AutoPromotion();
	void MaxWait();
	void Assign();
	bool AssignSpecial();
	bool AssignVIP();
	bool AssignNormal();
	void loadingdecrement();
	void checkdelivered();
	bool maintenance(Truck*& ptruck);
	void checkcheckup();
	void FileOut();
	
	//int checkuppriority(char x);
};
/*
*
*
*
*
* delete all trucks
* delete all cargo
* delete time ely bel new
*all events
* USE CONTROL F TO FIND ALL INSTANCES OF NEW FILE BY FILE TO MAKE SURE EVERYTHINGS IS DELETED*/

