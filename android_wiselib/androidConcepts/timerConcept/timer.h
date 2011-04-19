#ifndef TIMER_H
#define TIMER_H

#include <iostream>
#include <ctime>
#include <sys/time.h>
#include <signal.h>
#include <stdint.h>
#include "delegate.hpp"

using namespace std;

//implementation of timer concept in android platform
class AndroidTimerModel
{
public:
	typedef uint32_t millis_t;
	typedef delegate1<void, void*> timer_delegate_t;

	//constructor that initializes all the necessary variables
	AndroidTimerModel(void);

	//template method that sets a new event in the timer concept
	template<typename T, void (T::*TMethod)(void*)>
	int set_timer( millis_t millis, T *obj_pnt, void *userdata );
	//method for just testing purpose TO BE REMOVED
	static int callFirst(void);
	//static method that is used as a signal handler(static member functions have the same pointers as c-like functions in most compilers)
	static void signalHandler(int signal);
private:
	//private method that is called by set_timer to add an event to the timer
	int insert(millis_t millis, timer_delegate_t callback, void *userdata);

	//struct that holds the necessary info for every event
	struct Resources{
		timer_delegate_t membFunct;
		void *data;
		bool size;
		millis_t time;
	};
	//declaring an array of 10 Resources structs
	static struct Resources ResData[10];
	static const int MAX_EVENTS=10;
};



//constructor that initializes all the necessary variables
AndroidTimerModel::AndroidTimerModel(void)
{

	signal(SIGALRM,signalHandler);

	for(int i=0; i<MAX_EVENTS; i++)
	{
		ResData[i].data=NULL;
		ResData[i].size=false;
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

	bool flag=false;
	for(int i=0; i<MAX_EVENTS; i++)
	{
		if(ResData[i].size == true)
		{
			flag=true;
			break;
		}
	}

	if(flag==false)
	{
		ResData[0].membFunct=callback;
		ResData[0].time=millis;
		ResData[0].data=userdata;
		ResData[0].size=true;
	}
	else
	{
		flag=false;
		for(int i=0; i<MAX_EVENTS; i++)
		{
			if(ResData[i].size == true && ResData[i].time < millis) continue;
			else
			{
				flag=true;
				ResData[i].membFunct=callback;
				ResData[i].time=millis;
				ResData[i].data=userdata;
				ResData[i].size=1;
				break;
			}
		}
		if(flag==false)
		{
			//array is full!!!!
		}
	

	}

	itimerval timerval;
	timerval.it_interval.tv_sec=0;
	timerval.it_interval.tv_usec=0;
	timerval.it_value.tv_sec = (int)(millis / 1000000);
	timerval.it_value.tv_usec = (int) (millis) % 1000000;
	setitimer(ITIMER_REAL,&timerval,NULL);

	cout << timerval.it_value.tv_sec << "   " << timerval.it_value.tv_usec << "   " << endl;

	return 0;

}
//static method that is used as a signal handler(static member functions have the same pointers as c-like functions in most compilers)
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

	int counter=0;
	millis_t min=ResData[0].time;
	for(int i=0; i<MAX_EVENTS; i++)
	{
		if(ResData[i].time == min)
		{
			counter++;
		}
		else break;
	}

	for(int i=0; i<counter; i++)
	{
		(ResData[i].membFunct)(ResData[i].data);
	}

	return 0;

}


#endif

