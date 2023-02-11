#pragma once
#include "NormalQueue.h"
#include "Event.h"
#include "Truck.h"
#include "cargo.h"
#include "Priority.h"
#include "LinkedList.h"
#include "EventCancellation.h"
#include "EventPromotion.h"
#include "EventReady.h"
#include <string>
class Truck;
class Event;

//LinkedQueue<T>& Normal, LinkedQueue<T>& VIP, LinkedQueue<T>& Special)
class UI
{
public:
	bool getKeyPress();
	int GetUserMode();
	template <typename T>
	void PrintWaitingTrucks(LinkedQueue<T>& NormalT, LinkedQueue<T>& VIPT, LinkedQueue<T>& SpecialT);
	template <typename T>
	void PrintWaitingCargos(LinkedQueue<T>& Special, PriorityQueue<T>& VIP, LinkedList<T>& Normal);
	template <typename T>
	void PrintDeliveredCargos(LinkedQueue<T>& Delivered);
	template <typename T>
	void PrintCheckUpTrucks(LinkedQueue<T>& Normal, LinkedQueue<T>& Special, LinkedQueue<T>& VIP);
	void Printloadingtrucks(Truck* LoadingNormal, Truck* LoadingVIP, Truck* LoadingSpecial, Time* time);
	template <typename t>
	void Printmovingtrucks(PriorityQueue<t>& moving,int movingcargocount, Time* time);
	void PrintDeliveredMessage();
	void PrintCurrTime(int day, int hour);
};

template<typename T>
void UI::PrintWaitingTrucks(LinkedQueue<T>& NormalT, LinkedQueue<T>& VIPT, LinkedQueue<T>& SpecialT)
{
	int SpecialCount = SpecialT.getCount();
	int VIPCount = VIPT.getCount();
	int NormalCount = NormalT.getCount();
	cout << SpecialCount + VIPCount + NormalCount;
	cout << " Empty Trucks: ";
	Truck* pTruck;
	cout << "[";
	for (int i = 0;i < NormalCount;i++)
	{
		NormalT.dequeue(pTruck);
		cout << pTruck->getID();
		if (i != NormalCount - 1)
			cout << ",";
		NormalT.enqueue(pTruck);
	}
	cout << "]" << " ";
	cout << "(";
	for (int i = 0;i < SpecialCount;i++)
	{
		SpecialT.dequeue(pTruck);
		cout << pTruck->getID();
		if (i != SpecialCount - 1)
			cout << ",";
		SpecialT.enqueue(pTruck);
	}
	cout << ")" << " ";
	cout << "{";
	for (int i = 0;i < VIPCount;i++)
	{
		VIPT.dequeue(pTruck);
		cout << pTruck->getID();
		if (i != VIPCount - 1)
			cout << ",";
		VIPT.enqueue(pTruck);
	}
	cout << "}" << " " << endl;
	cout << "----------------------------------------------------" << endl;
}

template <typename T>
void UI::PrintWaitingCargos(LinkedQueue<T>& LQ, PriorityQueue<T>& PQ, LinkedList<T>& List)
{

	PriorityQueue<T>temp;
	int SpecialCount = LQ.getCount();
	int VIPCount = PQ.getCount();
	int NormalCount = List.getLength();
	int totalCargos = SpecialCount + VIPCount + NormalCount;
	cout << totalCargos << " ";
	cout << "Waiting Cargos: ";
	cargo* pCargo;
	cout << "[";
	for (int i = 0;i < NormalCount;i++)
	{
		pCargo = List.getEntry(i + 1);
		cout << pCargo->getID();
		if (i != NormalCount - 1)
			cout << ",";
	}
	cout << "]" << " ";
	cout << "(";
	for (int i = 0;i < SpecialCount;i++)
	{
		LQ.dequeue(pCargo);
		cout << pCargo->getID();
		if (i != SpecialCount - 1)
			cout << ",";
		LQ.enqueue(pCargo);
	}
	cout << ")" << " ";
	cout << "{";
	for (int i = 0;i < VIPCount;i++)
	{
		PQ.dequeue(pCargo);
		cout << pCargo->getID();
		if (i != VIPCount - 1)
			cout << ",";
		temp.enqueue(pCargo, pCargo->getPriority());
	}
	//PQ is now empty, temp  is full with vip cargos
	for (int i = 0;i < VIPCount;i++)
	{
		temp.dequeue(pCargo);
		PQ.enqueue(pCargo, pCargo->getPriority());
	}
	cout << "}" << " " << endl;
	cout << "----------------------------------------------------" << endl;
}
template<typename T>
void UI::PrintDeliveredCargos(LinkedQueue<T>& Delivered)
{
	int DeliveredCount = Delivered.getCount();
	cout << DeliveredCount << " ";
	cout << "Delivered Cargos: ";
	cargo* pCargo;
	cout << "{";
	bool first = true;
	for (int i = 0;i < DeliveredCount;i++)
	{
		Delivered.dequeue(pCargo);
		if (pCargo->getType() == 'V')
		{
			if (first)
			{
				cout << pCargo->getID();
				first = false;
				Delivered.enqueue(pCargo);
				continue;
			}
			cout << ",";
			cout << pCargo->getID();
		}
		Delivered.enqueue(pCargo);
	}
	first = true;
	cout << "}" << " ";

	cout << "[";
	for (int i = 0;i < DeliveredCount;i++)
	{
		Delivered.dequeue(pCargo);
		if (pCargo->getType() == 'N')
		{
			if (first)
			{
				cout << pCargo->getID();
				first = false;
				Delivered.enqueue(pCargo);
				continue;
			}
			cout << ",";
			cout << pCargo->getID();
		}
		Delivered.enqueue(pCargo);
	}
	first = true;
	cout << "]" << " ";

	cout << "(";
	for (int i = 0;i < DeliveredCount;i++)
	{
		Delivered.dequeue(pCargo);
		if (pCargo->getType() == 'S')
		{
			if (first)
			{
				cout << pCargo->getID();
				first = false;
				Delivered.enqueue(pCargo);
				continue;
			}
			cout << ",";
			cout << pCargo->getID();
		}
		Delivered.enqueue(pCargo);
	}
	cout << ")" << " " << endl;
	cout << "----------------------------------------------------" << endl;
}

template<typename T>
void UI::PrintCheckUpTrucks(LinkedQueue<T>& Normal, LinkedQueue<T>& Special, LinkedQueue<T>& VIP)
{
	int SpecialCount = Special.getCount();
	int VIPCount = VIP.getCount();
	int NormalCount = Normal.getCount();
	int totalCheckupTrucks = SpecialCount + VIPCount + NormalCount;
	cout << totalCheckupTrucks << " ";
	cout << "In-Checkup Trucks: ";
	Truck* pTruck;
	cout << "[";
	for (int i = 0;i < NormalCount;i++)
	{
		Normal.dequeue(pTruck);
		cout << pTruck->getID();
		if (i != NormalCount - 1)
			cout << ",";
		Normal.enqueue(pTruck);
	}
	cout << "]" << " ";
	cout << "(";
	for (int i = 0;i < SpecialCount;i++)
	{
		Special.dequeue(pTruck);
		cout << pTruck->getID();
		if (i != SpecialCount - 1)
			cout << ",";
		Special.enqueue(pTruck);
	}
	cout << ")" << " ";
	cout << "{";
	for (int i = 0;i < VIPCount;i++)
	{
		VIP.dequeue(pTruck);
		cout << pTruck->getID();
		if (i != VIPCount - 1)
			cout << ",";
		VIP.enqueue(pTruck);
	}
	cout << "}" << " " << endl;
	cout << "----------------------------------------------------" << endl;
}

template<typename t>
void UI::Printmovingtrucks(PriorityQueue<t>& moving,int movingcargocount,Time*time)
{
	int movecount = moving.getCount();
	cout << movingcargocount;
	cout << " Moving Cargos: ";
	Truck* pTruck;
	PriorityQueue<Truck*>temp;
	for (int i = 0; i < movecount; i++)
	{
		moving.dequeue(pTruck);
		cout << pTruck->getID();
		int count = 0;
		int* cargosids = pTruck->getcargosid(time, count);
		char cargotype = pTruck->getcargotype();    //NO ACCESS VIOLATION (NULLPTR ACCESING)BECAUSE IF 
													//NULL WONT ENTER THE FOR LOOP BECAUSE LIST WILL BE EMPTY
		int cargoscount = pTruck->getcargocount();
		if (cargotype == 'X')
		{
			cout << " ";
		}
		else if (cargotype == 'N')
		{
			cout << "[";
		}
		else if (cargotype == 'V')
		{
			cout << "{";
		}
		else {
			cout << '(';
		}
		for (int j = 0; j < cargoscount; j++)
		{
			cout << cargosids[j];
			if (j != cargoscount - 1)
				cout << ",";
		}
		if (cargotype == 'X')
		{
		}
		else if (cargotype == 'N')
		{
			cout << "] ";
		}
		else if (cargotype == 'V')
		{
			cout << "} ";
		}
		else {
			cout << ") ";
		}
		temp.enqueue(pTruck, pTruck->getmovingpriority());
	}
	while (temp.dequeue(pTruck))
	{
		moving.enqueue(pTruck, pTruck->getmovingpriority());
	}
	cout << endl;
	cout << "----------------------------------------------------" << endl;
}