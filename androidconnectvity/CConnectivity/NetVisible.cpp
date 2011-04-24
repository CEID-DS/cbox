/*
 * NetVisible.cpp
 *
 *  Created on: Apr 23, 2011
 *      Author: gkatzioura
 */

#include "NetVisible.h"

NetVisible::NetVisible() {
	status = -1;
	Getlocalip();
}

NetVisible::~NetVisible() {
	// TODO Auto-generated destructor stub
}

void* NetVisible::Working(void* t){

	(char*) t;

	int sock;
	char ip[15];
	int addr_len,bytes_write;
	char send_data[1024];

	struct sockaddr_in server_addr,client_addr;

	sock = socket(AF_INET,SOCK_DGRAM,0);

	addr_len = sizeof(struct sockaddr);
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(9876);

	sprintf(send_data,"<ipv4_addr>%s</ipv4_addr>\n<blue_addr>___________</blue_addr>\n<ipv6_addr>______________________________________</ipv6_addr>\n",t);

	for(int i=0;i<255;i++){
		sprintf(ip,"192.168.1.%d",i);
		server_addr.sin_addr.s_addr = inet_addr(ip);	
		sendto(sock,send_data,1024,0,(struct sockaddr *)&server_addr,addr_len);
		
	}
	std::cout<<"send"<<std::endl;
	
}

void NetVisible::Broadcast(){
	long t;
	status=pthread_create(&broadcast_thread,NULL,Working,(void*)my_ip);
}

void NetVisible::Getlocalip(){
	struct ifaddrs * ifAddrStruct = NULL;
	struct ifaddrs * ifa = NULL;
	void * tmpAddrPtr=NULL;

	getifaddrs(&ifAddrStruct);

	ifa = ifAddrStruct;

	for(int i=0;i<4;i++){
		ifa=ifa->ifa_next;
		tmpAddrPtr=&((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;
       	inet_ntop(AF_INET, tmpAddrPtr, my_ip, INET_ADDRSTRLEN);
	}
	//std::cout<<my_ip<<std::endl;
}

void NetVisible::GetBluemac(){

}
