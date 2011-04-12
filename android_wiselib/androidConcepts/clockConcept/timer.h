#ifndef TIMER_H
#define TIMER_H

#include <iostream>
#include <ctime>
#include <sys/time.h>
#include <stdint.h>
using namespace std;

//implementation of clock concept in android platform
class AndroidClockModel
{
public:
	typedef struct timeval* time_t;

	int state();

	//returns a struct containing time properties
	time_t time();
	//returns the total microseconds of time struct argument
	uint32_t microseconds( time_t time );
	//returns the total milliseconds of time struct argument
	uint32_t milliseconds( time_t time );
	//returns the total seconds of time struct argument
	uint32_t seconds( time_t time );

};


#endif
