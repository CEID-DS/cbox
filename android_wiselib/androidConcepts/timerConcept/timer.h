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
	static void signalHandler(int sgn);
	static void emptyHandler(int sgn);
private:
	//private method that is called by set_timer to add an event to the timer
	int insert(millis_t millis, timer_delegate_t callback, void *userdata);

	//struct that holds the necessary info for every event
	struct Resources{
		timer_delegate_t membFunct;
		void *data;
		bool registered;
		millis_t time;
	};
	//declaring an array of 10 Resources structs
	static struct Resources ResData[10];
	static const int MAX_EVENTS=10;
};



//constructor that initializes all the necessary variables
inline AndroidTimerModel::AndroidTimerModel(void)
{
	//registering the event handler
	signal(SIGALRM,signalHandler);

	//initializing the struct that holds all the necessary info
	for(int i=0; i<MAX_EVENTS; i++)
	{
		ResData[i].data=NULL;
		ResData[i].registered=false;
		ResData[i].time=0;
	}
}


//template method that sets a new event in the timer concept
template<typename T, void (T::*TMethod)(void*)>
int inline AndroidTimerModel:: set_timer(millis_t millis, T* obj, void *userdata)
{
	//insert the new event in the array struct
	insert(millis, timer_delegate_t::from_method<T, TMethod>(obj), userdata);

	return 0;

}
//private method that is called by set_timer to add an event to the timer
int inline AndroidTimerModel::insert(millis_t millis, timer_delegate_t callback, void *userdata)
{
	//flag that...
	bool hasEvent=false;

	//searchig the struct array to see if there exists at least one valide entry
	for(int i=0; i<MAX_EVENTS; i++)
	{
		if(ResData[i].registered == true)
		{
			hasEvent=true;
			break;
		}
	}

	//if no valide entry exists write the info to the first entry and register the event to the interval timer
	if(hasEvent==false)
	{
		ResData[0].membFunct=callback;
		ResData[0].time=millis;
		ResData[0].data=userdata;
		ResData[0].registered=true;

		//register the new event
		itimerval timerval;
		timerval.it_interval.tv_sec=0;
		timerval.it_interval.tv_usec=0;
		timerval.it_value.tv_sec = (int)(millis / 1000000);
		timerval.it_value.tv_usec = (int) (millis) % 1000000;
		setitimer(ITIMER_REAL,&timerval,NULL);
	}
	//else write the info to the struct according to time criterion
	else
	{
		
		int entry;

		//if the new event is scheduled previous than the already first known event write it in the first entry
		if(ResData[0].registered==true && ResData[0].time > millis)
		{

			itimerval timerval;
			getitimer(ITIMER_REAL, &timerval);
			//retrieving the value loss to substract it from the already first schedule
			long value = timerval.it_value.tv_sec * 1000000 + timerval.it_value.tv_usec;
			ResData[0].time -= (ResData[0].time - value);
			//initial, drag all the events one cell behind
			for(int i=MAX_EVENTS-1; i>0; i--)
			{
				if(ResData[i-1].registered == true)
				{
					ResData[i].membFunct = ResData[i-1].membFunct;
					ResData[i].data = ResData[i-1].data;
					ResData[i].registered = ResData[i-1].registered;
					ResData[i].time = ResData[i-1].time;
				}
			}
			//write the new event to the first cell
			ResData[0].membFunct=callback;
			ResData[0].time=millis;
			ResData[0].data=userdata;
			ResData[0].registered=1;

			//register the new event
			timerval.it_interval.tv_sec=0;
			timerval.it_interval.tv_usec=0;
			timerval.it_value.tv_sec = (int)(millis / 1000000);
			timerval.it_value.tv_usec = (int) (millis) % 1000000;
			setitimer(ITIMER_REAL,&timerval,NULL);
		}
		//if the new event is not scheduled previous than the already first known event write it in the first entry
		else if(ResData[0].registered==true && ResData[0].time <= millis)
		{

			bool emptyCell=false;

			itimerval timerval;
			getitimer(ITIMER_REAL, &timerval);
			//retrieving the real value to add it to this event to normalize after the first schedule event finish
			long value = timerval.it_value.tv_sec * 1000000 + timerval.it_value.tv_usec;
			millis =(millis_t)(millis + ResData[0].time - value);

			//find where the event should enter
			for(int i=0; i<MAX_EVENTS; i++)
			{
				//if it should be placed somewhere in the middle
				if(ResData[i].registered == true && ResData[i].time < millis) continue;
				else if(ResData[i].registered == true && ResData[i].time > millis)
				{
					entry=i;
					//drag all the previous events one cell behind
					for(int j=MAX_EVENTS-1; j>entry; j--)
					{
						if(ResData[j-1].registered == true)
						{
							ResData[j].membFunct = ResData[j-1].membFunct;
							ResData[j].data = ResData[j-1].data;
							ResData[j].registered = ResData[j-1].registered;
							ResData[j].time = ResData[j-1].time;
						}
					}
					//register the event
					emptyCell=true;
					ResData[entry].membFunct=callback;
					ResData[entry].time=millis;
					ResData[entry].data=userdata;
					ResData[entry].registered=1;
					break;
				}
				//if it should be placed after every register event just register it!
				else if(ResData[i].registered == false)
				{
					emptyCell=true;
					ResData[i].membFunct=callback;
					ResData[i].time=millis;
					ResData[i].data=userdata;
					ResData[i].registered=1;
					break;
				}
			}
			if(emptyCell==false)
			{
				//array is full!!!!
			}
		}
	

	}


	return 0;

}
//static method that is used as a signal handler(static member functions have the same pointers as c-like functions in most compilers)
void inline AndroidTimerModel::signalHandler(int sgn)
{
	//unregistering the event handler(actually registering it to a handler that does nothing)
	signal(SIGALRM,emptyHandler);
	if(sgn==SIGALRM)
	{
		callFirst();
	}
	//registering the event handler
	signal(SIGALRM,signalHandler);

}

//static method that is used as a signal handler(static member functions have the same pointers as c-like functions in most compilers)
void inline AndroidTimerModel::emptyHandler(int sgn)
{
	

}

//method that calls the firs scheduled event
int inline AndroidTimerModel::callFirst(void)
{

	(ResData[0].membFunct)(ResData[0].data);
	
	//substract from all the vents the elapsed time
	for(int i=1; i<MAX_EVENTS; i++)
	{
		if(ResData[i].registered!=false)
		{
			ResData[i].time -= ResData[0].time;
		}
	}

	//drag all the events one cell down
	for(int i=1; i<MAX_EVENTS; i++)
	{
		ResData[i-1].membFunct = ResData[i].membFunct;
		ResData[i-1].data = ResData[i].data;
		ResData[i-1].registered = ResData[i].registered;
		ResData[i-1].time = ResData[i].time;
	}
	
	//reset the last cell
	//ResData[MAX_EVENTS-1].membFunct = 0;
	ResData[MAX_EVENTS-1].data = NULL;
	ResData[MAX_EVENTS-1].registered = false;
	ResData[MAX_EVENTS-1].time = 0;

	itimerval timerval;
	timerval.it_interval.tv_sec=0;
	timerval.it_interval.tv_usec=0;
	timerval.it_value.tv_sec = (int)(ResData[0].time / 1000000);
	timerval.it_value.tv_usec = (int) (ResData[0].time) % 1000000;
	setitimer(ITIMER_REAL,&timerval,NULL);


	return 0;

}


#endif

