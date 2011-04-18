#ifndef TEST_CLASS_H
#define TEST_CLASS_H

#include <iostream>
using namespace std;


class testClass
{
	public:
		void helloWorld(void *pointer);
};

void testClass::helloWorld(void *pointer)
{
	//char *newP= (char *) malloc(15);

	//newP=(char *)pointer;

	cout << (char *)pointer << endl;
}

#endif

