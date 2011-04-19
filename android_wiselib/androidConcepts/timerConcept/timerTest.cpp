#include <iostream>
#include <ctime>
#include <sys/time.h>
#include <stdint.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <string>
#include "timer.h"
#include "testClass.h"



int main()
{

	typedef uint32_t millis_t;
	//declaring a new AndroidTimerMode object
	AndroidTimerModel timer;
	//declaring a new testClass object
	testClass test;
	//declaring char pointer to use the as the data to testClass functions
	char *pointer = (char *) malloc(sizeof(50));
	char *pointer2 = (char *) malloc(sizeof(50));
	strncpy(pointer,":)", sizeof(":)"));
	uint32_t timeval=2000303;

	test.charHelloWorld(pointer);

	
	strncpy(pointer,"1st Call", sizeof("1st Call"));
	
	timer.set_timer<testClass, &testClass::charHelloWorld>(timeval, &test, pointer);
	
	timeval=4000303;
	
	strncpy(pointer2,"2nd Call", sizeof("2nd Call"));



	timer.set_timer<testClass, &testClass::charHelloWorld>(timeval, &test, pointer2);

	string *s1 = new string("3nd Call");

	//cout << *s << endl;

	timeval=2500303;
	timer.set_timer<testClass, &testClass::stringHelloWorld>(timeval, &test, s1);

	string *s2 = new string("4nd Call");

	timeval=1500303;
	timer.set_timer<testClass, &testClass::stringHelloWorld>(timeval, &test, s2);

	

	while(1) { }

	return 0;
}


