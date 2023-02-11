#include "Time.h"
#include"iostream"
using namespace std;
class cargo;

Time::Time(int Day, int Hour)
{
	day = Day;
	hour = Hour;
}

Time::Time()
{
}

void Time::increment()
{
	if (hour == 23)
	{
		hour = 0;
		day++;
	}
	else
	{
		hour++;

	}
}

int Time::getHour()
{
	return hour;
}

int Time::getDay()
{
	return day;
}

void Time::setTime(int Day,int Hour)
{
	day = Day;
	hour = Hour;
}

bool Time::CompareTimes(Time *eventtime)
{
	if (day == eventtime->getDay() && hour == eventtime->getHour())
		return true;
	else
		return false;
}

bool Time::CompareGreaterTimes(Time* time)
{
	if ((day*24+hour) <= ((time->getDay()*24)+time->getHour()))
		return true;
	else
		return false;
}

void Time::AddWait(int maxwait, Time*& deadlineWait)
{
	int totalhours = hour + maxwait + (day * 24);
	int deadlineday = totalhours / 24;
	int deadlinehour = totalhours % 24;
	deadlineWait->setTime(deadlineday, deadlinehour);
}

int Time::timetoint()
{
	return day*24+hour;
}

void Time::intToTime(int time)
{
	day = time / 24;
	hour = time % 24;
}

Time* Time::subtractTimes(Time* rValue)
{
	Time* result=new Time;
	int lValue=timetoint();
	int rvalue=rValue->timetoint();
	result->intToTime(rvalue - lValue);
	return result;
}
