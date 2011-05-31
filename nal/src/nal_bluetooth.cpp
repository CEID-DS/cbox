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

#include "../lib/nal_bluetooth.h"


int Bluetooth::sockfd=-2;
int Bluetooth::instances=0;
delegate Bluetooth::dels[MAX_INSTANCES];
char Bluetooth::data_buffer[SIZE];
int Bluetooth::valid_data=0;
bool Bluetooth::valid_dels[MAX_INSTANCES];
address Bluetooth::devices[MAX_DEV];
int Bluetooth::dev_num=-1;
int scan(address* array);

Bluetooth::Bluetooth(){
	if (instances==0){
		//initialize valid_dels boolean array
		for (int i=0;i<MAX_INSTANCES;i++){
			valid_dels[i]=false;
		}
	}
}

void Bluetooth::call_delegates(){
	for (int i=0;i<MAX_INSTANCES;i++){
		if (valid_dels[i]){
			dels[i](data_buffer,valid_data);
		}
	}
}

int Bluetooth::enable(){
	
	if (instances < MAX_INSTANCES){
		//socket is initialized the first time an instance of the class is created
		if (sockfd == -2){
		  	if ((sockfd = socket(AF_BLUETOOTH, SOCK_DGRAM, BTPROTO_RFCOMM) ) < 0){ //if socket failed to initialize
				sockfd=-2;
		      		return -1;
		    	}
		    	
		    int on=1;
		    	
		    /*	if (setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST,  &on, sizeof(on)) < 0){
		    		sockfd=-2;
		    		return -1;
		    	} */
		    	
		    struct sockaddr_rc rc;
			
			memset(&rc, 0, sizeof rc);
			
			rc.rc_family = AF_BLUETOOTH;
			rc.rc_bdaddr = *BDADDR_ANY; //test!! (?)
			rc.rc_channel = (uint8_t) PORT;
			
			if (bind(sockfd,(struct sockaddr *)&rc, sizeof(rc)) < 0 ){
				close(sockfd);
				sockfd=-2;

				return -1;
			}
			
			pthread_t tid;
			
			if(pthread_create(&tid,NULL,&receive_routine,(void*) sockfd)!=0){//create receive thread
				close(sockfd);
				sockfd=-2;
				return -1;
			}
			
			pthread_detach(tid);
			
			pthread_t scan;
			
			if(pthread_create(&scan,NULL,&scan_routine,(void*) sockfd)!=0){//create receive thread
				close(sockfd);
				sockfd=-2;
				return -1;
			}
			
			pthread_detach(scan);
		}
		
		instances++;
		return 0;
	}else{
		return -1;
	}
}

int Bluetooth::send(char *data,int size){
	
	struct sockaddr_rc rc;
	int i;
	
	//send to all available devices
	for (i=0;i<dev_num;i++){
					
		memset(&rc, 0, sizeof rc);
	
		rc.rc_family = AF_BLUETOOTH;
		str2ba(devices[i].mac_addr, &rc.rc_bdaddr); //test!! (?)
		rc.rc_channel = (uint8_t) PORT;
	
		sendto(sockfd, data, size, 0,(struct sockaddr*)&rc, sizeof rc);
		}
}

int Bluetooth::disable(){

	instances--;
	
	if (instances==0){//when there are no instances of the class close socket
		
		close(sockfd);
		sockfd=-2;//back to startup state
	}
}

void Bluetooth::unregister_receiver(){
	valid_dels[del_id] = false;
}

void *receive_routine(void *socket){

	struct sockaddr_in sa;
	socklen_t fromlen;
	
	for (;;){
		if ( (Bluetooth::valid_data=recvfrom((int) socket, (void *)Bluetooth::data_buffer, SIZE, 0, (struct sockaddr *)&sa, &fromlen) ) <= 0) {
			pthread_exit(NULL);
		}
		
		Bluetooth::call_delegates();
	}
}

void *scan_routine(void *socket){

	struct sockaddr_rc rc;
	socklen_t fromlen;
	
	for (;;){
		if ( (Bluetooth::valid_data=recvfrom((int) socket, (void *)Bluetooth::data_buffer, SIZE, 0, (struct sockaddr *)&rc, &fromlen) ) <= 0) {
			pthread_exit(NULL);
		}
		
		//	Bluetooth::call_delegates();
		Bluetooth::dev_num=scan(Bluetooth::devices);
		sleep(60); //stops for 60 secs
	}
}

//inner function (scan)
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
