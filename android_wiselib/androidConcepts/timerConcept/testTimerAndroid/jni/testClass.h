#ifndef TEST_CLASS_H
#define TEST_CLASS_H

#include <iostream>
#include <string.h>
#include <string>
#include "AndrDebug.h"

using namespace std;


class testClass
{
	public:
		void charHelloWorld(void *pointer);
		void stringHelloWorld(void *pointer);
	private:
		AndrDebug testDebug;
};

void testClass::charHelloWorld(void *pointer)
{

	testDebug.debug("%s", pointer);
}

void testClass::stringHelloWorld(void *pointer)
{
	string p;
	p=*static_cast<std::string*>(pointer);

	//cout << *static_cast<std::string*>(pointer) << endl;
	//testDebug.debug("%s", p.c_str());
	testDebug.debug("%s", p.c_str());
}

#endif

