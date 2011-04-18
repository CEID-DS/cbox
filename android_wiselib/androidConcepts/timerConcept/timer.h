#ifndef TIMER_H
#define TIMER_H

#include <iostream>
#include <ctime>
#include <sys/time.h>
#include <signal.h>
#include <stdint.h>
#include "delegate.hpp"

#define MAX_EVENTS 3

using namespace std;



//implementation of timer concept in android platform
class AndroidTimerModel
{
public:
	typedef uint32_t millis_t;
	typedef delegate1<void, void*> timer_delegate_t;

	//constructor that initializes all the necessary variables
	AndroidTimerModel();

	//template method that sets a new event in the timer concept
	template<typename T, void (T::*TMethod)(void*)>
	int set_timer( millis_t millis, T *obj_pnt, void *userdata );
	//method for just testing purpose TO BE REMOVED
	static int callFirst(void);
	static void signalHandler(int signal);
private:
	//private method that is called by set_timer to add an event to the timer
	int insert(millis_t millis, timer_delegate_t callback, void *userdata);

	//struct that holds the necessary info for every event
	struct Resources{
		timer_delegate_t membFunct;
		void *data;
		int size;
		millis_t time;
	};
	//declaring an array of 10 Resources structs
	static struct Resources ResData[10];
	static int best;
};

AndroidTimerModel::AndroidTimerModel()
{
	signal(SIGALRM,signalHandler);

	for(int i=0; i<MAX_EVENTS; i++)
	{
		ResData[i].data=NULL;
		ResData[i].size=-1;
		ResData[i].time=-1;
	}

}


//template method that sets a new event in the timer concept
template<typename T, void (T::*TMethod)(void*)>
int AndroidTimerModel:: set_timer(millis_t millis, T* obj, void *userdata)
{

	insert(millis, timer_delegate_t::from_method<T, TMethod>(obj), userdata);

	return 0;

}
//private method that is called by set_timer to add an event to the timer
int AndroidTimerModel::insert(millis_t millis, timer_delegate_t callback, void *userdata)
{
	for(int i=0; i<MAX_EVENTS; i++)
	{
		if(ResData[i].size == -1)
		{
			ResData[i].membFunct=callback;
			ResData[i].time=millis;
			ResData[i].data=userdata;
			break;
		}
	}

	itimerval timerval;
	timerval.it_interval.tv_sec=0;
	timerval.it_interval.tv_usec=0;
	timerval.it_value.tv_sec = millis / 1000;
	timerval.it_value.tv_usec = (millis % 1000) * 1000000;
	setitimer(ITIMER_REAL,&timerval,NULL);

	return 0;

}

void AndroidTimerModel::signalHandler(int signal)
{
	if(signal==SIGALRM)
	{
		callFirst();
	}

}

//method for just testing purpose TO BE REMOVED
int AndroidTimerModel::callFirst(void)
{
	int temp=33;

	for(int i=0; i<MAX_EVENTS; i++)
	{
		if(ResData[i].time == 15)
		{
			(ResData[i].membFunct)(ResData[i].data);
		}
	}


	return temp;

}

#endif

