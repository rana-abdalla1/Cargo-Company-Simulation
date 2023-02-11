#pragma once
#include "cargo.h"
#include "Priority.h"
class cargo;

class Truck
{
	float activetime=0;

	char TruckType;
	int truckID;
	int totalnumjourney=0;
	int checkupcount=1;
	int TruckCapacity;
	int MaintenanceTime;
	float Speed;
	float DeliveryInterval;
	int CurrentJourneyNum;
	int checkuppriority=0;
	int loadingpriority = 0;
	float movingPriority;
	int Checkupend;
	int totalCargosCount = 0;
	PriorityQueue<cargo*>Cargos;
	//   <c1,c2,c3,c4>
	//   
	//List of Cargos in each truck;
public:
	Truck(char type, int Capacity, int time, float speed,int ID);
	void SetTruckType(char type);
	void SetTruckCapacity(int cap);
	void setMaintenanceTime(float time);
	void setSpeed(float s);
	void setDeliveryInterval(float interval);
	void AddCargo(cargo* pCargo);
	void DeleteCargo(cargo*& pCargo);
	char getType();
	int getID();
	int getTruckCapacity();
	float getMaintenanceTime();
	float getSpeed();
	float getDeliveryInterval();
	void setpriority(int x);
	int* getcargosid(Time* time, int& countarr);
	int getcargocount();
	char getcargotype();
	int getcheckuppriority();
	int getloadingpriority();
	float movingpriority(Time* time);
	float getmovingpriority();
	void incrementcheckup();
	int getcheckupcount();
	void resetcheckup();
	int getcheckupend();
	bool checkCargoDelivery(Time*time,cargo*&pCargo);
	void checkupend(Time* time, int checkupduration);
	float getactiveTime();
	int getTotalCargosCount();
	float getUtilizationTime(int totalsimtime);
};


