#pragma once
#include "Company.h"
#include <chrono>
#include <thread>
#include "Time.h"
#include "Truck.h"
class Truck;
Company::Company()
{
	currtime = new Time;
}
void Company::FileLoad(string fileName)
{
	int readyevent = 0;
	int cancelevent = 0;
	ifstream file;
	file.open(fileName);
	file >> normalTrucks;
	file >> specialTrucks;
	file >> VIPTrucks;
	file >> normalSpeed;
	file >> specialSpeed;
	file >> VIPSpeed;
	file >> normalCap;
	file >> specialCap;
	file >> VIPCap;
	file >> JourneyNum;
	file >> CheckNormal;
	file >> CheckSpecial;
	file >> CheckVIP;
	file >> autoP;
	file >> maxWait;
	file >> eventsNumber;
	int ID = 1;
	//Constructing Trucks and placing them in waiting lists
	for (int i = 0; i < normalTrucks; i++)
	{
		Truck* pTruck = new Truck('N', normalCap, CheckNormal, normalSpeed, ID);
		WaitingNormalTrucks.enqueue(pTruck);
		ID++;
	}
	for (int i = 0; i < VIPTrucks; i++)
	{
		Truck* pTruck = new Truck('V', VIPCap, CheckVIP, VIPSpeed, ID);
		WaitingVIPTrucks.enqueue(pTruck);
		ID++;
	}
	for (int i = 0; i < specialTrucks; i++)
	{
		Truck* pTruck = new Truck('S', specialCap, CheckSpecial, specialSpeed, ID);
		WaitingSpecialTrucks.enqueue(pTruck);
		ID++;
	}
	Event* pEvent;
	for (int i = 0; i < eventsNumber; i++)
	{
		char eventtype;
		file >> eventtype;
		if (eventtype == 'R')
		{
			readyevent++;
			char type;
			int day, hour, ID, cost, distance, LoadTime;
			file >> type;
			file >> day;
			file.ignore();
			file >> hour;
			file >> ID;
			file >> distance;
			file >> LoadTime;
			file >> cost;
			pEvent = new EventReady(this, type, day, hour, ID, distance, LoadTime, cost, maxWait, autoP);
		}
		if (eventtype == 'P')
		{
			int ID, day, hour, cost;
			file >> day;
			file.ignore();
			file >> hour;
			file >> ID;
			file >> cost;
			pEvent = new EventPromotion(this, day, hour, ID, cost);
		}
		if (eventtype == 'X')
		{
			cancelevent++;
			int ID, day, hour;
			file >> day;
			file.ignore();
			file >> hour;
			file >> ID;
			pEvent = new EventCancellation(this, day, hour, ID);
		}
		EventList.enqueue(pEvent);
	}
	endindicator = readyevent - cancelevent;
	file.close();
}

bool Company::Prepare(cargo* pCargo, float priority)
{ //placing cargo in company's list
	if (pCargo == NULL)
		return false;
	if (pCargo->getType() == 'V')
	{
		WaitingVIPCargos.enqueue(pCargo, priority);
	}
	if (pCargo->getType() == 'S')
	{
		WaitingSpecialCargos.enqueue(pCargo);
	}
	if (pCargo->getType() == 'N')
	{
		WaitingNormalCargos.insert(WaitingNormalCargos.getLength() + 1, pCargo);
	}
	return true;
}
bool Company::Cancel(int ID)
{
	cargo* pCargo;
	int pos;
	pCargo = WaitingNormalCargos.Search(ID, pos); //remove returns pCargo that needs to be cancelled
	if (pCargo == NULL)
	{
		endindicator++;
		return false;

	}
	WaitingNormalCargos.remove(pos);
	delete pCargo; //removes the cargo from the memory
	return true;
}

void Company::Simulate(string fileName)
{
	FileLoad(fileName);
	int mode = ptrUI->GetUserMode();
	Truck* pTruck = nullptr;

	int var = 0;
	while (1)
	{
		Time* eventtime;
		Event* pEvent = nullptr; //pEvent doesn't deallocate at end of while it will point to the last event
		while (1)
		{
			EventList.peek(pEvent); //peeks the evetn list t0 check if its time to be executed
			if (pEvent != nullptr)
			{
				pEvent->getEventtime(eventtime); //check its time
				if (currtime->CompareTimes(eventtime))
				{
					EventList.dequeue(pEvent); //Removed event from list
					pEvent->Execute(); //executed event, Polymorphism applies here 
					delete pEvent; //deallocates event as no longer needed after being executed
					pEvent = nullptr;
				}
				else
				{
					break;
				}
			}
			else
			{
				break;
			}
		}
		checkdelivered(); //to check if any cargos are delivered or truck has finisheded
		checkcheckup(); //to check if truck finished checkup or not and returns it back to being waiting
		MaxWait(); //placed here to make sure the cargo is assigned correctly
		AutoPromotion();
		if (currtime->getHour() >= 5 && currtime->getHour() <= 23) //Handling off hours to not assign or load out of on hours
		{
			Assign();
			loadingdecrement();
		}
		int day = currtime->getDay();
		int hour = currtime->getHour();
		if (mode == 1 || mode == 2)
		{
			ptrUI->PrintCurrTime(day, hour);
			ptrUI->PrintWaitingCargos(WaitingSpecialCargos, WaitingVIPCargos, WaitingNormalCargos);
			ptrUI->Printloadingtrucks(LoadingNormal, LoadingVIP, LoadingSpecial, currtime);
			ptrUI->PrintWaitingTrucks(WaitingNormalTrucks, WaitingVIPTrucks, WaitingSpecialTrucks);
			ptrUI->Printmovingtrucks(MovingTrucks, movingcargocount, currtime);
			ptrUI->PrintCheckUpTrucks(CheckUpNormalTrucks, CheckUpSpecialTrucks, CheckUpVIPTrucks);
			ptrUI->PrintDeliveredCargos(DeliveredCargos);
		}
		if (mode == 1) //interactive
		{
			bool checkcont = ptrUI->getKeyPress(); //waits for key press
			if (!checkcont)
			{
				return;
			}
		}
		else if (mode == 2) //step by step
		{
			chrono::milliseconds timespan(1000);
			this_thread::sleep_for(timespan);
		} //else silent mode
		currtime->increment(); 
		if (endindicator == DeliveredCargos.getCount() && EventList.isEmpty() == true && CheckUpNormalTrucks.isEmpty() == true && CheckUpSpecialTrucks.isEmpty() == true && CheckUpVIPTrucks.isEmpty() == true && MovingTrucks.isEmpty() == true) //Condition to deliver al cargs to delivered list
		{ 
			ptrUI->PrintDeliveredMessage();
			break;
		}
	}
	FileOut();
}

bool Company::Promote(int ID)
{
	cargo* pCargo;
	int pos;
	pCargo = WaitingNormalCargos.Search(ID, pos); //remove returns pCargo that needs to be promoted
	if (pCargo == NULL)
	{
		return false; //item not found
	}
	WaitingNormalCargos.remove(pos);
	pCargo->settype('V'); //promoted to vip
	WaitingVIPCargos.enqueue(pCargo, pCargo->getPriority());
	return true;
}

void Company::AutoPromotion()
{
	cargo* pCargo;
	for (int i = 0; i < WaitingNormalCargos.getLength(); i++)
	{
		pCargo = WaitingNormalCargos.getEntry(i + 1);
		if (pCargo->checkAutoPTime(currtime))
		{
			numofautop++;
			Promote(pCargo->getID());
			i--;
		}
	}
}

void Company::MaxWait()
{
	if (currtime->getHour() >= 5 && currtime->getHour() <= 23)
	{
		cargo* pCargo = nullptr;
		while (!WaitingSpecialCargos.isEmpty()) //bec if several cargos at the same time
		{
			WaitingSpecialCargos.peek(pCargo);
			if (pCargo->checkmaxwait(currtime))
			{
				if (!AssignSpecial())
				{
					break; //because no truck found
				}
			}
			else
			{
				break; // maxwait havent come yet
			}
		}
		pCargo = nullptr;
		while (!WaitingNormalCargos.isEmpty())
		{
			pCargo = WaitingNormalCargos.getEntry(1);
			if (pCargo->checkmaxwait(currtime))
			{
				maxwait = true;
				if (!AssignNormal())
				{
					break; //beecause no truck found
				}

			}
			else
			{
				break; // maxwait havent come yet

			}
		}
	}
}

void Company::Assign()
{
	int normalcargocount = WaitingNormalCargos.getLength();
	int specialcargocount = WaitingSpecialCargos.getCount();
	int vipcargocount = WaitingVIPCargos.getCount();

	while (!WaitingNormalTrucks.isEmpty() || !WaitingSpecialTrucks.isEmpty() || !WaitingVIPTrucks.isEmpty()) //as long as there is truck we can assign
	{
		bool breaker = true;
		if ((vipcargocount >= normalCap || vipcargocount >= specialCap || vipcargocount >= VIPCap) && LoadingVIP == nullptr)
		{
			bool assignvip = AssignVIP();
			if (assignvip)
				breaker = false;
		}
		if (specialcargocount >= specialCap)
		{
			bool assignspecial = AssignSpecial();
			if (assignspecial == true)
				breaker = false;
		}
		if (normalcargocount >= normalCap || normalcargocount >= VIPCap)
		{
			bool assignnormal = AssignNormal();
			if (assignnormal == true)
				breaker = false;
		}
		if (breaker)
			break;
	}
}

bool Company::AssignSpecial()
{
	Truck* pTruck = nullptr;
	cargo* pCargo = nullptr;
	if (LoadingSpecial == nullptr)
	{
		if (!WaitingSpecialTrucks.isEmpty())
		{
			WaitingSpecialTrucks.dequeue(pTruck);
			for (int i = 0; i < specialCap; i++)
			{
				if (WaitingSpecialCargos.dequeue(pCargo))
				{
					int loadtime = pCargo->getloadtime();
					pCargo->setTruck(pTruck);
					loadingtimespecial = loadingtimespecial + loadtime;
					pCargo->setloadfinishtime(loadingtimespecial + currtime->timetoint());
					pTruck->AddCargo(pCargo);
				}
				else
				{
					break;
				}
			}
			LoadingSpecial = pTruck;
			return true;
		}
		return false;
	}
	return false;
}
bool Company::AssignVIP()
{
	Truck* pTruck = nullptr;
	cargo* pCargo = nullptr;
	if (LoadingVIP == nullptr)
	{
		if (WaitingVIPCargos.getCount() >= VIPCap && !WaitingVIPTrucks.isEmpty()) //to make sure we reached cap exactly
		{
			WaitingVIPTrucks.dequeue(pTruck);
			for (int i = 0; i < VIPCap; i++)
			{
				WaitingVIPCargos.dequeue(pCargo);
				int loadtime = pCargo->getloadtime();
				pCargo->setTruck(pTruck);
				loadingtimevip = loadingtimevip + loadtime;
				pCargo->setloadfinishtime(loadingtimevip + currtime->timetoint());
				pTruck->AddCargo(pCargo);
			}
			LoadingVIP = pTruck; //transfereing truck from waiting to loading vip variable in company after being assigned
			return true;
		}
		else if (WaitingVIPCargos.getCount() >= normalCap && !WaitingNormalTrucks.isEmpty())
		{
			WaitingNormalTrucks.dequeue(pTruck);
			for (int i = 0; i < normalCap; i++)
			{
				WaitingVIPCargos.dequeue(pCargo);
				int loadtime = pCargo->getloadtime();
				pCargo->setTruck(pTruck);
				loadingtimevip = loadingtimevip + loadtime;
				pCargo->setloadfinishtime(loadingtimevip + currtime->timetoint());
				pTruck->AddCargo(pCargo);
			}
			LoadingVIP = pTruck;
			return true;
		}
		else if (WaitingVIPCargos.getCount() >= specialCap && !WaitingSpecialTrucks.isEmpty())
		{
			WaitingSpecialTrucks.dequeue(pTruck);
			for (int i = 0; i < specialCap; i++)
			{
				WaitingVIPCargos.dequeue(pCargo);
				int loadtime = pCargo->getloadtime();
				pCargo->setTruck(pTruck);
				loadingtimevip = loadingtimevip + loadtime;
				pCargo->setloadfinishtime(loadingtimevip + currtime->timetoint());
				pTruck->AddCargo(pCargo);
			}
			LoadingVIP = pTruck;
			return true;
		}
	}
	return false;
}
bool Company::AssignNormal()
{
	Truck* pTruck = nullptr;
	cargo* pCargo = nullptr;
	if (LoadingNormal == nullptr)
	{
		if ((!WaitingNormalTrucks.isEmpty() && WaitingNormalCargos.getLength() >= normalCap) || maxwait == true)
		{
			if (WaitingNormalTrucks.dequeue(pTruck))
			{
				maxwait = false;
				for (int i = 0; i < normalCap; i++)
				{
					pCargo = nullptr;
					pCargo = WaitingNormalCargos.getEntry(1);
					if (WaitingNormalCargos.remove(1))
					{
						int loadtime = pCargo->getloadtime();
						pCargo->setTruck(pTruck);
						loadingtimenormal = loadingtimenormal + loadtime;
						pCargo->setloadfinishtime(loadingtimenormal + currtime->timetoint());
						pTruck->AddCargo(pCargo);
					}
					else
					{
						break;
					}
				}
				LoadingNormal = pTruck;
				return true;
			}
		}
		else if ((!WaitingVIPTrucks.isEmpty() && WaitingNormalCargos.getLength() >= VIPCap) || maxwait == true)
		{
			if (WaitingVIPTrucks.dequeue(pTruck))
			{
				maxwait = false;
				for (int i = 0; i < VIPCap; i++)
				{
					pCargo = WaitingNormalCargos.getEntry(1);
					if (WaitingNormalCargos.remove(1))
					{
						int loadtime = pCargo->getloadtime();
						pCargo->setTruck(pTruck);
						loadingtimenormal = loadingtimenormal + loadtime;
						pCargo->setloadfinishtime(loadingtimenormal + currtime->timetoint());
						pTruck->AddCargo(pCargo);
					}
					else
					{
						break;
					}
				}
				LoadingNormal = pTruck;
				return true;
			}
		}
		return false;
	}
	return false;//loading happening
}

void Company::loadingdecrement()
{
	if (loadingtimenormal != 0)
	{
		loadingtimenormal--;

	}
	else
	{
		if (LoadingNormal != nullptr)
		{
			movingcargocount += LoadingNormal->getcargocount();
			MovingTrucks.enqueue(LoadingNormal, LoadingNormal->movingpriority(currtime)); //cargos finished loading to truck
			LoadingNormal = nullptr; // to be able to assign other normal cargos
		}
	}
	if (loadingtimespecial != 0)
	{
		loadingtimespecial--;

	}
	else
	{
		if (LoadingSpecial != nullptr)
		{
			movingcargocount += LoadingSpecial->getcargocount();
			MovingTrucks.enqueue(LoadingSpecial, LoadingSpecial->movingpriority(currtime));

			LoadingSpecial = nullptr;
		}
	}
	if (loadingtimevip != 0)
	{
		loadingtimevip--;

	}
	else
	{
		if (LoadingVIP != nullptr)
		{
			movingcargocount += LoadingVIP->getcargocount();
			MovingTrucks.enqueue(LoadingVIP, LoadingVIP->movingpriority(currtime));
			LoadingVIP = nullptr;
		}
	}
}

void Company::checkdelivered()
{
	float time = currtime->timetoint();
	Truck* ptruck = nullptr;
	cargo* pCargo = nullptr;
	PriorityQueue<Truck*>temp;

	while (!MovingTrucks.isEmpty())
	{
		MovingTrucks.dequeue(ptruck);
		if (ptruck == nullptr)
			return;
		while (1)
		{
			bool check = ptruck->checkCargoDelivery(currtime, pCargo);
			if (check)
			{
				DeliveredCargos.enqueue(pCargo); //cargo has finished its journey
				movingcargocount--;
			}
			else
				break;
		}
		if (ptruck->getmovingpriority() <= time) //check if truck has delivered all its cargos
		{
			if (ptruck->getType() == 'N')
			{
				bool checkup = maintenance(ptruck);
				if (checkup == false)
				{
					WaitingNormalTrucks.enqueue(ptruck);
				}
				else
				{
					ptruck->checkupend(currtime, CheckNormal);
					CheckUpNormalTrucks.enqueue(ptruck);
				}
				ptruck = nullptr;
			}
			else if (ptruck->getType() == 'V')
			{
				bool checkup = maintenance(ptruck);
				if (checkup == false)
				{
					WaitingVIPTrucks.enqueue(ptruck);
				}
				else
				{
					ptruck->checkupend(currtime, CheckVIP);
					CheckUpVIPTrucks.enqueue(ptruck);
				}
				ptruck = nullptr;
			}
			else
			{
				bool checkup = maintenance(ptruck);
				if (checkup == false)
				{
					WaitingSpecialTrucks.enqueue(ptruck);
				}
				else
				{
					ptruck->checkupend(currtime, CheckSpecial);
					CheckUpSpecialTrucks.enqueue(ptruck);
				}
				ptruck = nullptr;
			}
		}
		else
		{
			temp.enqueue(ptruck, ptruck->getmovingpriority());
		}
	}
	while (temp.dequeue(ptruck))
	{
		MovingTrucks.enqueue(ptruck, ptruck->getmovingpriority());
	}

}
bool Company::maintenance(Truck*& ptruck)
{
	if (ptruck->getcheckupcount() == JourneyNum) //check if now truck needs checkup
	{
		ptruck->resetcheckup();
		return true;
	}
	else
	{
		ptruck->incrementcheckup(); //truck still doesnot need checkup
		return false;
	}
}

void Company::checkcheckup() //func removes trucks tha finished its checkup duration from checkuplist to waiting trucks lists
{
	int time = currtime->timetoint();
	Truck* ptruck = nullptr;
	while (1)
	{
		bool breaker = true;
		if (CheckUpNormalTrucks.peek(ptruck))
		{
			if (time == ptruck->getcheckupend()) //check if current time reached the time the truck ends its checkup in
			{
				CheckUpNormalTrucks.dequeue(ptruck);
				WaitingNormalTrucks.enqueue(ptruck);
				breaker = false;
			}
		}
		ptruck = nullptr;
		if (CheckUpSpecialTrucks.peek(ptruck))
		{
			if (time == ptruck->getcheckupend())
			{
				CheckUpSpecialTrucks.dequeue(ptruck);
				WaitingSpecialTrucks.enqueue(ptruck);
				breaker = false;
			}
		}
		ptruck = nullptr;
		if (CheckUpVIPTrucks.peek(ptruck))
		{
			if (time == ptruck->getcheckupend())
			{
				CheckUpVIPTrucks.dequeue(ptruck);
				WaitingVIPTrucks.enqueue(ptruck);
				breaker = false;
			}
		}
		ptruck = nullptr;
		if (breaker)
		{
			break;
		}
	}
}
void Company::FileOut()
{
	ofstream fileout;
	fileout.open("Simulation Facts");
	fileout << " CDT  " << "   ID  " << "  PT    " << "  WT  " << "  TID" << endl;
	int numOfCargos = DeliveredCargos.getCount();
	int numOfTrucks = normalTrucks + specialTrucks + VIPTrucks;
	int numOfSpecial = 0;
	int numOfNormal = 0;
	int numOfVIP = 0;
	float totalwaittime = 0;
	cargo* pCargo;
	for (int i = 0;i < numOfCargos;i++)
	{
		DeliveredCargos.dequeue(pCargo);
		if (pCargo->getType() == 'S')
		{
			numOfSpecial++;
		}
		else if (pCargo->getType() == 'V')
		{
			numOfVIP++;
		}
		else if (pCargo->getType() == 'N')
		{
			numOfNormal++;
		}
		pCargo->Save(fileout, totalwaittime);
		delete pCargo; //deallocates cargos as they will no longer be used
	}
	float percentageautop;
	if ((numOfNormal + numofautop) != 0)
	{
		percentageautop = ((float)numofautop / (numOfNormal + numofautop)) * 100;
	}
	else
	{
		percentageautop = 0;
	}
	Truck* pTruck;
	float totalActiveTime = 0;
	float totalutilizationTime = 0;
	int totalsimtime = currtime->timetoint() - 1;
	while (WaitingNormalTrucks.dequeue(pTruck))
	{
		totalActiveTime += pTruck->getactiveTime();
		totalutilizationTime += pTruck->getUtilizationTime(totalsimtime);
		delete pTruck; //Deallocates trucks as it will not be used again
	}
	while (WaitingSpecialTrucks.dequeue(pTruck))
	{
		totalActiveTime += pTruck->getactiveTime();
		totalutilizationTime += pTruck->getUtilizationTime(totalsimtime);
		delete pTruck;
	}
	while (WaitingVIPTrucks.dequeue(pTruck))
	{
		totalActiveTime += pTruck->getactiveTime();
		totalutilizationTime += pTruck->getUtilizationTime(totalsimtime);
		delete pTruck;
	}
	Time* pTotalTime = new Time;
	pTotalTime->intToTime(ceil(totalwaittime / numOfCargos));
	fileout << "-----------------------------------------------" << endl;
	fileout << "-----------------------------------------------" << endl;
	fileout << "Cargos: " << numOfCargos << " [N:" << numOfNormal << ", S: " << numOfSpecial << ", V: " << numOfVIP << "]" << endl;
	fileout << "Cargo Avg Wait = " << pTotalTime->getDay() << ":" << pTotalTime->getHour() << endl;
	fileout << "Auto-promoted Cargos: " << floor(percentageautop*100)/100 << "%" << endl;
	fileout << "Trucks: " << numOfTrucks << " [N:" << normalTrucks << ", S: " << specialTrucks << ", V: " << VIPTrucks << "]" << endl;
	fileout << "Avg Active Time = " << floor(((totalActiveTime / (float)numOfTrucks)) / totalsimtime * 100 * 100) / 100.0 << "%" << endl;
	fileout << "Avg utilization = " << floor((totalutilizationTime / numOfTrucks) * 100 * 100) / 100.0 << "%" << endl;
	fileout.close();
}