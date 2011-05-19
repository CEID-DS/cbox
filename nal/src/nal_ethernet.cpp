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

#include "../lib/nal_ethernet.h"

int Ethernet::sockfd=-2;
int Ethernet::instances=0;
delegate Ethernet::dels[MAX_INSTANCES];
char Ethernet::data_buffer[8192];
int Ethernet::valid_data=0;
bool Ethernet::valid_dels[MAX_INSTANCES];

Ethernet::Ethernet(){
	if (instances==0){
		//initialize valid_dels boolean array
		for (int i=0;i<MAX_INSTANCES;i++){
			valid_dels[i]=false;
		}
	}
}

void Ethernet::call_delegates(){
	for (int i=0;i<MAX_INSTANCES;i++){
		if (valid_dels[i]){
			dels[i](data_buffer,valid_data);
		}
	}
}

int Ethernet::enable(){
	
	if (instances < MAX_INSTANCES){
		//socket is initialized the first time an instance of the class is created
		if (sockfd == -2){
			sockfd = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
		  	if (sockfd==-1){ //if socket failed to initialize
				sockfd=-2;
		      		return -1;
		    	}
		    	
			struct sockaddr_in sa; 
			
			memset(&sa, 0, sizeof sa);
			
			sa.sin_family = AF_INET;
			sa.sin_addr.s_addr = INADDR_ANY;
			sa.sin_port = htons(9876);

			if (bind(sockfd,(struct sockaddr *)&sa, sizeof(sa) == -1) ){
				close(sockfd);
				return -1;
			}
			
			pthread_t tid;
			
			if(pthread_create(&tid,NULL,&receive_routine,(void*) sockfd)!=0){//create receive thread
				close(sockfd);
				return -1;
			}
			
			pthread_detach(tid);
		}
		
		instances++;
	}else{
		return -1;
	}
}

int Ethernet::send(char *data,int size){
	
	

}

int Ethernet::disable(){

	instances--;
	
	if (instances==0){//when there are no instances of the class close socket
		
		close(sockfd);
		sockfd=-2;//back to startup state
	}
}

void *receive_routine(void *socket){

	struct sockaddr_in sa;
	socklen_t fromlen;
	
	for (;;){

		if ( (Ethernet::valid_data=recvfrom((int) &socket, (void *)Ethernet::data_buffer, 8192, 0, (struct sockaddr *)&sa, &fromlen) ) <= 0) {
			pthread_exit(NULL);
		}
		
		Ethernet::call_delegates();
	}
}

//just for debugging
main(){
	
}

