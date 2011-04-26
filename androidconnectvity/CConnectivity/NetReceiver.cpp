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

void* NetReceiver::Working(void *t){

	int* sad = reinterpret_cast<int*>(t);
    int  newsock    = *sad;

	//int newsock = (int)t;
	char recv_data[1024];
	std::string file;
	int data,splits;

	std::cout<<"Accepted"<<std::endl;

	recv(newsock,recv_data,1024,0);

	//std::cout<<"Received: "<<recv_data<<std::endl;

	file=getfromtag("tosent>",recv_data);
	std::cout<<"file "<<file<<std::endl;
	data=atoi(getfromtag("data>",recv_data).c_str());
	std::cout<<"data "<<data<<std::endl;	
	splits=atoi(getfromtag("splits>",recv_data).c_str());
	std::cout<<"splits "<<splits<<std::endl;

	std::ofstream new_file (file.c_str(),std::ios::out|std::ios::binary);

	for(int i=0;i<splits-1;i++){
		recv(newsock,recv_data,1024,0);
		new_file.write(recv_data,1024);
	}

	int left = data-1024*(splits-1);
	recv(newsock,recv_data,1024,0);
	new_file.write(recv_data,left);
	new_file.close();
		
	close(newsock);
}

void NetReceiver::Receiver(){

	std::cout<<"Receiver started"<<std::endl;	

	int sock,newsock;
	int addr_len,bytes_read;
	//char recv_data[1024];

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
	while(1){
		newsock=accept(sock,(struct sockaddr *) &client_addr,(socklen_t*)&chilen);

		pthread_create(&receive_thread,NULL,Working,(void*)newsock);
	}
	
	close(sock);
}
