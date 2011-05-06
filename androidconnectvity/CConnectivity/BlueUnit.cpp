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
 * BlueUnit.cpp
 *
 *  Created on: May 3, 2011
 *      Author: gkatzioura
 */

#include "BlueUnit.h"

BlueUnit::BlueUnit() {
	// TODO Auto-generated constructor stub

}

BlueUnit::~BlueUnit() {
	// TODO Auto-generated destructor stub
}

void BlueUnit::Linker(std::string option){
	void* t;	

	//strcpy(BlueUnit::b_device.addr,"sadasd");
	if(option.find("scanbluetooth",0)==0)	
		pthread_create(&discover_thread,NULL,Discover,(void*)t);
	else if(option.find("bluereceiver",0)==0)
		BlueReceiver();
}

void BlueUnit::BlueTransmiter(std::string mac,std::string file){

	std::cout<<mac<<" "<<file<<std::endl;
	mac +="\n";
    mac +=file;

    char *t = new char[mac.size()+1];
    std::copy(mac.begin(),mac.end(),t);
    t[mac.size()]='\0';

    std::cout<<t<<std::endl;

    pthread_create(&send_thread,NULL,Transmit,(void*)t);

}

void BlueUnit::BlueReceiver(){
	std::cout<<"bluereceiver"<<std::endl;
	
	struct sockaddr_rc loc_addr = {0},rem_addr = {0};
	char buf[1024] = {0};
	int s,client,bytes_read;
	socklen_t opt = sizeof(rem_addr);

	s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

	loc_addr.rc_family = AF_BLUETOOTH;
    loc_addr.rc_bdaddr = *BDADDR_ANY;
    loc_addr.rc_channel = (uint8_t) 1;
    bind(s, (struct sockaddr *)&loc_addr, sizeof(loc_addr));

    listen(s, 1);

	while(1){
		client = accept(s, (struct sockaddr *)&rem_addr, &opt);
	    //ba2str( &rem_addr.rc_bdaddr, buf );
	    //fprintf(stderr, "accepted connection from %s\n", buf);
	    memset(buf, 0, sizeof(buf));
	
		pthread_create(&receive_thread,NULL,Transfer,(void*)client);
	}

    close(s);

}

void* BlueUnit::Transmit(void *t){

	int pieces,connection;
 	char send_data[1024];
	struct sockaddr_rc addr = { 0 };
 
	std::cout<<"Trasmiter Started"<<std::endl;
    std::string buf = (char *)t;

    int size=buf.find('\n');
    char* mac=new char[size+1];
    mac[size]='\0';
    buf.copy(mac,size,0);

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

    std::string bufb = file ;
    std::stringstream out;
    out << filestatus.st_size;

    buf="<tosent>"+bufb+"</tosent>"+"\n"
            +"<data>"+stringtoint(filestatus.st_size)+"</data>"+"\n";
    out << pieces;
    buf+="<splits>"+stringtoint(pieces)+"</splits>"+"\n";

    buf.copy(send_data,buf.size(),0);

	connection = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
	addr.rc_family = AF_BLUETOOTH;
	addr.rc_channel = (uint8_t) 1;
	str2ba( mac, &addr.rc_bdaddr );

	connect(connection, (struct sockaddr *)&addr, sizeof(addr));

    write(connection, send_data, 1024);

    std::ifstream old_file (file,std::ios::in|std::ios::binary);

    for(int i=0;i<pieces-1;i++){
        char new_data[1024];

        unsigned int acknowledge;
        old_file.read(new_data,1024);

        write(connection,new_data,1024);
        read(connection,&acknowledge,sizeof(acknowledge));
        std::cout<<i<<" checksum "<<acknowledge<<" true checksum "<<std::endl;
    }

    int left=filestatus.st_size-1024*(pieces-1);

    old_file.read(send_data,left);
    write(connection,send_data,1024);
    old_file.close();	
	
	close(connection);
}

void* BlueUnit::Transfer(void *t){

	std::string file;	
	char recv_data[1024];
	int data,splits;
	int bytes_read;


    bytes_read = read((int)t, recv_data, sizeof(recv_data));
    //if( bytes_read > 0 ) {
    printf("received %s \n", recv_data);
    //}

	file=getfromtag("tosent>",recv_data);
    std::cout<<"file "<<file<<std::endl;
    data=atoi(getfromtag("data>",recv_data).c_str());
    std::cout<<"data "<<data<<std::endl;
    splits=atoi(getfromtag("splits>",recv_data).c_str());
    std::cout<<"splits "<<splits<<std::endl;

	std::ofstream new_file (file.c_str(),std::ios::out|std::ios::binary);

    for(int i=0;i<splits-1;i++){
        char get_data[1024];
        unsigned int acknowledge;

		read(int(t),get_data,sizeof(get_data));
        //recv(newsock,get_data,sizeof(get_data),0);
        //send(newsock,&lala,sizeof(lala),0);
		write(int(t),&acknowledge,sizeof(acknowledge));

        //std::cout<<i<<" checksum "<<getchecksum(get_data,1024)<<std::endl;
        new_file.write(get_data,1024);
    }

    int left = data-1024*(splits-1);
    std::cout<<left<<std::endl;
    read(int(t),recv_data,1024);
    new_file.write(recv_data,left);
    new_file.close();

	close(t);
}

void* BlueUnit::Discover(void *t){


	//char* a = reinterpret_cast<char*>(t);
	//(char*)t;

	//std::cout<<"thread";
	inquiry_info *ii = NULL;
	int max_rsp, num_rsp;
	int dev_id,sock,len,flags;
	int i;
	char addr[19] = { 0 };
	char name[248] = { 0 };

	dev_id = hci_get_route(NULL);
	sock = hci_open_dev(dev_id);
	if (dev_id < 0 || sock < 0) {
        perror("opening socket");
        exit(1);
    }

	    len  = 8;
    max_rsp = 255;
    flags = IREQ_CACHE_FLUSH;
    ii = (inquiry_info*)malloc(max_rsp * sizeof(inquiry_info));
    
    num_rsp = hci_inquiry(dev_id, len, max_rsp, NULL, &ii, flags);
    if( num_rsp < 0 ) perror("hci_inquiry");

	std::string devstring,devname,devaddr;

    for (i = 0; i < num_rsp; i++) {
        ba2str(&(ii+i)->bdaddr, addr);
        memset(name, 0, sizeof(name));
        if (hci_read_remote_name(sock, &(ii+i)->bdaddr, sizeof(name), 
            name, 0) < 0)
        strcpy(name, "[unknown]");
        printf("%s  %s\n", addr, name);
		devaddr = addr;
		devname = name;
		struct BlueUnit buf ;
		//t.push_back(buf);

		devstring = devaddr + "\n" + devname + "\n";

	}

	//a = new char[devstring.size()+1];
	//std::copy(devstring.begin(),devstring.end(),a);
	//t[devstring.size()]='\0';

    free( ii );
    close( sock );

}
