#include <iostream>
#include <stdlib.h>
#include <string.h>
#include "../AndroidRadioModel.h"
#include "testClass.h"
using namespace std;

//void function1(int len, void *data);

int main(void)
{
	char *s= (char *) malloc(10*sizeof(char));
	strncpy(s,"helloo",sizeof("helloo"));

	cout << s << endl;

	AndroidRadioModel testRadio;
	testClass newTest;
	testRadio.reg_recv_callback<testClass, &testClass::charHelloWorld>(&newTest);
	testRadio.receive_message(1,(int)sizeof("aaa"),(void *)"aaa");
	
	sleep(1);
	testRadio.reg_recv_callback<testClass, &testClass::charHelloWorld2>(&newTest);
	testRadio.receive_message(1,(int)sizeof("aaa"),(void *)"bbb");
	


	return 0;
}
