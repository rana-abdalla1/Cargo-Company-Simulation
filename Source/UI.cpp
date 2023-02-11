#include "UI.h"
using namespace std;
#include <iostream>
class Truck;
bool UI::getKeyPress()
{
	cout << "Press Enter to continue or any key to terminate" << endl;
	int x = cin.get();
	while (1)
	{
		if (x == '\n')
		{
			return true;
		}
		else

		{
			cout << "Simulation Ended, Thank You!" << endl;
			return false;
		}
	}
}
int UI::GetUserMode()
{
	int num;
	cout << "Welcome to the Simulator!" << endl;
	cout << "Please enter 1 for Interactive Mode OR 2 for Step-By-Step Mode OR 3 for Silent Mode" << endl;
	cin >> num;
	if (num == 3)
	{
		cout << "Silent Mode" << endl;
		cout << "Simulation Starts..." << endl;
	}
	return num;
}

void UI::PrintDeliveredMessage()
{
	cout << "All cargos have been delivered succesfully, Simulation Ended, Thank You!" << endl;
	cout << "Ouput file created." << endl;
}

void UI::PrintCurrTime(int day, int hour)
{
	cout << "Current Time (Day:Hour):";
	cout << day << ":" << hour << endl;
}
void UI::Printloadingtrucks(Truck* LoadingNormal, Truck* LoadingVIP, Truck* LoadingSpecial, Time* time)
{
	int count = 0;
	if (LoadingNormal)
	{
		count++;
	}
	if (LoadingVIP)
	{
		count++;
	}
	if (LoadingSpecial)
	{
		count++;
	}
	cout << count << " Loading trucks:";
	if (LoadingNormal)
	{
		cout << LoadingNormal->getID();
		//print cargos
		int countarr = 0;
		int* cargosids = LoadingNormal->getcargosid(time, countarr);
		cout << "[";
		int count = 0;
		for (int i = 0; i < countarr; i++)
		{
			cout << cargosids[i];
			if ((i != countarr - 1))
				cout << ",";
		}
		cout << "] ";
	}
	if (LoadingVIP)
	{
		cout << LoadingVIP->getID();
		//print cargos
		int countarr = 0;
		int* cargosids = LoadingVIP->getcargosid(time, countarr);
		cout << "{";
		int count = 0;
		for (int i = 0; i < countarr; i++)
		{
			cout << cargosids[i];
			if ((i != countarr - 1))
				cout << ",";
		}
		cout << "} ";
	}
	if (LoadingSpecial)
	{
		cout << LoadingSpecial->getID();
		//print cargos
		int countarr = 0;
		int* cargosids = LoadingSpecial->getcargosid(time, countarr);
		cout << "(";
		int count = 0;
		for (int i = 0; i < countarr; i++)
		{
			cout << cargosids[i];
			if ((i != countarr - 1))
				cout << ",";
		}
		cout << ") ";
	}
	cout << endl << "----------------------------------------------------" << endl;
}