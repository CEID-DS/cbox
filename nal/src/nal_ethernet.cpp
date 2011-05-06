#include "nal_ethernet.h"

int Ethernet::sockfd=-2;

int Ethernet::enable(){
	
	if (sockfd == -2){
		sockfd = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	  	if (sockfd==-1){ //if socket failed to initialize

	      		return -1;
	    	}
	}
	
	return 0;
}

int Ethernet::disable(){

	if (sockfd>0){
		close(sockfd);
		sockfd=-2;
	}
}
