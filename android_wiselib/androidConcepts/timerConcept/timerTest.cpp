#include <iostream>
#include <ctime>
#include <sys/time.h>
#include <stdint.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include "timer.h"
//#include "timer.cpp"
#include "testClass.h"
using namespace std;



int main()
{

	typedef uint32_t millis_t;
	AndroidTimerModel timer;
	testClass test;
	char *pointer = (char *) malloc(sizeof(50));
	strncpy(pointer,":)", sizeof(":)"));
	uint32_t timeval=1500303;

	test.helloWorld(pointer);

	strncpy(pointer,":) This time the function was called by timer :)", sizeof(":) This time the function was called by timer :)"));

	timer.set_timer<testClass, &testClass::helloWorld>(timeval, &test, pointer);

	timeval=1900303;

	strncpy(pointer,":) This time the function 2 was called by timer :)", sizeof(":) This time the function 2 was called by timer :)"));
	timer.set_timer<testClass, &testClass::helloWorld>(timeval, &test, pointer);

	while(1) { }



	return 0;
}


