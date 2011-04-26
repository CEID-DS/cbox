/*
 * NetReceiver.cpp
 *
 *  Created on: Apr 25, 2011
 *      Author: kurgan
 */

#include "NetReceiver.h"

NetReceiver::NetReceiver() {
	// TODO Auto-generated constructor stub

}

NetReceiver::~NetReceiver() {
	// TODO Auto-generated destructor stub
}

void* Working(void *t){

}

void NetReceiver::Receiver(){

	std::cout<<"Receiver started"<<std::endl;	

	int sock,newsock;
	int addr_len,bytes_read;
	char recv_data[1024];

	struct sockaddr_in server_addr,client_addr;
	socklen_t addr_size = 0;

	sock = socket(AF_INET,SOCK_STREAM,0);
	
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(9876);
	server_addr.sin_addr.s_addr = INADDR_ANY;

	if(bind(sock,(sockaddr*)&server_addr,sizeof(server_addr))==-1){
		std::cout<<"Unable to bind"<<std::endl;
	}

	if(listen(sock,248)==-1){
		std::cout<<"Cannot listen"<<std::endl;
	}

	int chilen = sizeof(client_addr);
	newsock=accept(sock,(struct sockaddr *) &client_addr,(socklen_t*)&chilen);
	//accept(sock,(),sizeof);
	std::cout<<"Accepted"<<std::endl;

	recv(newsock,recv_data,1024,0);

	std::cout<<"Received: "<<recv_data<<std::endl;
	
	close(newsock);
	close(sock);
}
