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


//compile using g++ scan.cpp -lbluetooth

#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include <sys/socket.h>
#include <string>
#include <iostream>
#include <cstdlib>
#include "../lib/nal_address.h"

#define TIME 8
#define MAX_DEV 255

using namespace std;
int scan(address* array);

int main(){
	
	int i,num;

	address* array;
	array=new address[MAX_DEV];
	
	while(1){
		num=-1; //init num
		
		//scan devices
		num=scan(array);
	
		//# of available devices
		cout << "Number of available devices: " << num <<endl;
	
		//print available devices
		for(i=0;i<num;i++) cout << array[i].mac_addr << endl;
		
		sleep(60); //sleep for 60 secs
		
	}
		
	
	delete[] array;

}

int scan(address* array){
	
int int_id=-1; //id of device-interface
int num_dev=-1; //# of returned devices
int flag,i,socket;
inquiry_info *table= NULL;

int_id=hci_get_route(NULL); //first available adapter

socket=hci_open_dev(int_id);  //opens a socket

if(int_id<0 || socket<0){
	
	perror("Opening the Socket");
	return -1;
}

flag=IREQ_CACHE_FLUSH; //flush from previous use

//allocate the max size
table=(inquiry_info*)malloc(MAX_DEV * sizeof(inquiry_info));

if (table==NULL){
	 perror("Memory allocation");
	 return -1;
 }

//scanning for devices
num_dev=hci_inquiry(int_id, TIME, MAX_DEV, NULL, &table, flag);

if(num_dev<0){
	 perror("scanning problem");
	 free(table);
	 return -1;
	 }

//for every possible device
for(i=0;i<num_dev;i++){
	
	ba2str(&(table+i)->bdaddr, array[i].mac_addr); //get the hex address

	}
	
free(table);
close(socket);

return num_dev;
}
