/*
 * NetTransmiter.cpp
 *
 *  Created on: Apr 24, 2011
 *      Author: Emmanouil Gkatziouras
 */

#include "NetTransmiter.h"

NetTransmiter::NetTransmiter() {
	// TODO Auto-generated constructor stub
	status=-1;
}

NetTransmiter::~NetTransmiter() {
	// TODO Auto-generated destructor stub
}

void* NetTransmiter::Working(void *t){

	int pieces;
	std::cout<<"Trasmiter Started"<<std::endl;
	std::string buf = (char *)t;

	int size=buf.find('\n');
	char* ip=new char[size+1];
	ip[size]='\0';
	buf.copy(ip,size,0);

	int sizeb = buf.size()-buf.find('\n');
	char* file=new char[sizeb];
	buf.copy(file,sizeb-1,size+1);
	file[sizeb-1]='\0';
	std::cout<<"file "<<file<<std::endl;

	struct stat filestatus;
	stat(file,&filestatus);	

	std::cout<<filestatus.st_size<<" "<<filestatus.st_size/1024<<std::endl;

    if(filestatus.st_size>=1024){
        pieces = filestatus.st_size/1024;
        if(filestatus.st_size%1024!=0){
            pieces++;
        }
    }
    else{
        pieces=1;
    }

	//buf="";

	std::cout<<"The buf "<<buf<<" was"<<std::endl;

	std::string bufb = file;
	std::stringstream out;
	out << filestatus.st_size;

	buf="<tosent>"+bufb+"</tosent>"+"\n"
            +"<data>"+out.str()+"<data>"+"\n";
	out << pieces;
    buf+="<splits>"+out.str()+"</splits>"+"\n";

	

	int sock,newsock;
	int addr_len,bytes_read;
	char send_data[1024];

	buf.copy(send_data,buf.size(),0);	

	struct sockaddr_in server_addr,client_addr;

	socklen_t addr_size = 0;

	sock = socket(AF_INET,SOCK_STREAM,0);

	server_addr.sin_family =  AF_INET;
	server_addr.sin_port = htons(9876);	
	server_addr.sin_addr.s_addr = inet_addr(ip);

	addr_len=sizeof(server_addr);
	connect(sock,(struct sockaddr *) &server_addr,addr_len);

	send(sock,send_data,1024,0);

	close(sock);
	
}

void NetTransmiter::Transmiter(std::string ip,std::string file){
	ip +="\n";
	ip +=file;

	char *t = new char[ip.size()+1];
	std::copy(ip.begin(),ip.end(),t);
	t[ip.size()]='\0';

	std::cout<<t<<std::endl;
	
	status=pthread_create(&transmit_thread,NULL,Working,(void*)t);
}
