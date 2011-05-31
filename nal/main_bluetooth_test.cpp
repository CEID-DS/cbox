//compile using g++ main.cpp src/bluetooth.cpp -lbluetooth -lpthread

#include "lib/nal_bluetooth.h"
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


int main(void){

	Bluetooth b;
	Test test;
	
	cout << b.enable() << endl;
	b.register_receiver<Test,&Test::call_me>(&test);

	while(1){}
	
	
	b.disable();
	pthread_exit(NULL);
}

