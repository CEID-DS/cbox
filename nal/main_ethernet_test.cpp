#include "lib/nal_ethernet.h"
#include <iostream>

using namespace std;

class Test{
	public:
	void call_me(char *data,int size){
		for (int i=0;i<size;i++){
			cout << data[i];
		}
		
		cout << " test" << endl;
	}
};

class Test1{
	public:
	void call_me_1(char *data,int size){
		for (int i=0;i<size;i++){
			cout << data[i];
		}
		
		cout << " test1" << endl;
	}
};

int main(void){

	Ethernet e;
	Test test;
	
	cout << e.enable() << endl;
	e.register_receiver<Test,&Test::call_me>(&test);

	while(1){}
	
	
	e.disable();
	pthread_exit(NULL);
}

