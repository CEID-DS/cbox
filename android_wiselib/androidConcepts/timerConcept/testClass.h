#ifndef TEST_CLASS_H
#define TEST_CLASS_H

#include <iostream>
#include <string.h>
using namespace std;


class testClass
{
	public:
		void charHelloWorld(void *pointer);
		void stringHelloWorld(void *pointer);
};

void testClass::charHelloWorld(void *pointer)
{

	cout << (char *)pointer << endl;
}

void testClass::stringHelloWorld(void *pointer)
{

	cout << *static_cast<std::string*>(pointer) << endl;
}

#endif

