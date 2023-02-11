#pragma once
class Time
{
	int day = 0;
	int hour = 0;
public:
	Time(int Day, int Hour);
	Time();
	void increment();
	int getHour();
	int getDay();
	void setTime(int Day,int Hour);
	bool CompareTimes(Time* eventtime);
	bool CompareGreaterTimes(Time* time);
	void AddWait(int maxwait, Time*& deadlineWait);
	int timetoint();
	void intToTime(int time);
	Time* subtractTimes(Time* rValue);
};

