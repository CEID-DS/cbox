#ifndef CLOCK_H
#define CLOCK_H

#include <iostream>
#include <ctime>
#include <sys/time.h>
#include <stdint.h>
using namespace std;

//implementation of clock concept in android platform
class AndroidClockModel
{
public:
	typedef struct timeval time_t;
	int state();

	//returns a struct containing time properties
	time_t time()
	{
		time_t tv;
		gettimeofday(&tv,NULL);
		return tv;
	}
	//returns the microseconds of time struct argument
	uint64_t microseconds( time_t time )
	{
		return time.tv_usec;
	}
	//returns the milliseconds of time struct argument
	uint64_t milliseconds( time_t time )
	{
		return ((int)time.tv_usec/1000);
	}
	//returns the seconds of time struct argument
	uint64_t seconds( time_t time )
	{
		return time.tv_sec;
	}

};


#endif
