#include <iostream>
#include <ctime>
#include <sys/time.h>
#include <stdint.h>

using namespace std;

#include "timer.h"


/*
int AndroidClockModel::state()
{
	return 0;
}
*/

//returns a struct containing time properties
AndroidClockModel::time_t AndroidClockModel::time()
{
	time_t tv;
	gettimeofday(tv,NULL);
	return tv;
}
//returns the total microseconds of time struct argument
uint32_t AndroidClockModel::microseconds(time_t time)
{
	return (time->tv_sec + time->tv_usec);
}
//returns the total milliseconds of time struct argument
uint32_t AndroidClockModel::milliseconds(time_t time)
{
	return (time->tv_sec + ((int)time->tv_usec/1000));
}
//returns the total seconds of time struct argument
uint32_t AndroidClockModel::seconds(time_t time)
{
	return time->tv_sec;
}



