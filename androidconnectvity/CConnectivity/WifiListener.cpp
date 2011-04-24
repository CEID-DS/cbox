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

/*
 * WifiListener.cpp
 *
 *  Created on: Apr 13, 2011
 *      Author: gkatzioura
 */

#include "WifiListener.h"


namespace cconnectivity{
WifiListener::WifiListener() {
	status=-1;	
}

WifiListener::~WifiListener() {
	// TODO Auto-generated destructor stub
}

void* WifiListener::Working(void* t){

	int sock;
	int addr_len,bytes_read;
	char recv_data[1024];

	struct sockaddr_in server_addr,client_addr;

	socklen_t addr_size = 0;

	sock= socket(AF_INET,SOCK_DGRAM,0);

	server_addr.sin_family= AF_INET;

	server_addr.sin_port = htons(9876);
	server_addr.sin_addr.s_addr = INADDR_ANY;
	bzero(&(server_addr.sin_zero),8);

	bind(sock,(struct sockaddr *)&server_addr,sizeof(struct sockaddr));

	addr_len = sizeof(struct sockaddr);

	std::cout<<"Listening Devices"<<std::endl;

	while(1){
		bytes_read = recvfrom(sock,recv_data,1024,0,(struct sockaddr *)&client_addr,(socklen_t*)&addr_len);

		recv_data[1024]='\0';
		printf("%s\n",recv_data);
	pthread_exit(0);
	}

}

void WifiListener::Listener(){
	long t;
	
	status=pthread_create(&listen_thread,NULL,Working,(void*)t);
	//std::cout<<"status"<<status<<std::endl;
	//Working();
}
}
