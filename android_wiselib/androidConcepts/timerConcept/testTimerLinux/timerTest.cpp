/******************************************************************************
* This file is part of cbox.
*
* cbox is free software: you can redistribute it and/or modify
* it under the terms of the GNU LesserGeneral Public License as published
* by the Free Software Foundation, either version 3 of the License, or
* any later version.
*
* Cbox is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with cbox.  If not, see <http://www.gnu.org/licenses/>.
*******************************************************************************/

#include <iostream>
#include <ctime>
#include <sys/time.h>
#include <stdint.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <string>
#include <unistd.h>
#include "../timer.h"
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


	uint32_t timeval=5000000;
	strncpy(pointer,"1st Call", sizeof("1st Call"));
	timer.set_timer<testClass, &testClass::charHelloWorld>(timeval, &test, pointer);
	//sleep(4);
	cout << "--------------------------------------" << endl;

	timeval=8000000;
	strncpy(pointer2,"2nd Call", sizeof("2nd Call"));
	timer.set_timer<testClass, &testClass::charHelloWorld>(timeval, &test, pointer2);
	
	//sleep(10);

	cout << "--------------------------------------" << endl;

	string *s1 = new string("3nd Call");
	timeval=1000000;
	timer.set_timer<testClass, &testClass::stringHelloWorld>(timeval, &test, s1);

	cout << "--------------------------------------" << endl;

	string *s2 = new string("4nd Call");
	timeval=12000000;
	timer.set_timer<testClass, &testClass::stringHelloWorld>(timeval, &test, s2);


	while(1) { }

	return 0;
}


