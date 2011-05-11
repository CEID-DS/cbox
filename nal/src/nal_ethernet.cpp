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

Ethernet::sockfd=-2;
Ethernet::instances=0;
Ethernet::max_instances=20;

//note: some operations must use mutexes. for example changing instances_values;

Ethernet::Ethernet(){
	//do some standard initialization
	//for example initialize mutex
}

int Ethernet::enable(){
	
	if (instances<max_instances){
		//socket is initialized the first time an instance of the class is created
		if (sockfd == -2){
			sockfd = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
		  	if (sockfd==-1){ //if socket failed to initialize
				sockfd=-2;
		      		return -1;
		    	}
		    	
		    	//spawn thread that will wait for data and call delegates when data is available
		}
		
		instances++;
	}else{
		return -1;
	}
}

int Ethernet::send(address addr,char *data,int size){
	
	

}

int Ethernet::disable(){

	instances--;
	
	if (instances==0){//when there are no instances of the class close socket
		
		//mutex should be used to "communicate" with receiver thread
		
		close(sockfd);
		sockfd=-2;	
	}
}

