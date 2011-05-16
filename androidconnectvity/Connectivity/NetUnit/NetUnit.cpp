/* 
 * File:   NetUnit.cpp
 * Author: carnage
 * 
 * Created on May 12, 2011, 1:54 PM
 */

#include "NetUnit.h"

NetUnit::NetUnit() {
}

NetUnit::~NetUnit() {
}

void NetUnit::Receive(){
    cout<<"Receiver Started"<<endl;
    
    int sock,newsock;
    int addr_len,bytes_read;

    struct sockaddr_in server_addr,client_addr;
    socklen_t addr_size;
    sock=socket(AF_INET,SOCK_STREAM,0);

    server_addr.sin_family =AF_INET;
    server_addr.sin_port = htons(9876);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if(bind(sock,(sockaddr*)&server_addr,sizeof(server_addr))!=-1){
        if(listen(sock,248)!=-1){
            while(1){

                int chilen = sizeof(client_addr);

                cout<<"Listening";
                newsock = accept(sock,(struct sockaddr *) &client_addr,(socklen_t*)&chilen);

                cout<<"Received";

                ReceiveThread receivethread(newsock);
                boost::thread thr(boost::bind(&ReceiveThread::Run,&receivethread));
            }
        }
    }else{
        cout<<"Can't bind"<<endl;
    }
    
    ReceiveThread receivethread(newsock);   
    boost::thread thread(boost::bind(&ReceiveThread::Run,&receivethread));
}

void NetUnit::Transmit(string ip,char *data,long size){
    //cout<<"Transmiter Started"<<endl;
    
    TransmitThread transmitThread(ip,data,size);
    
    boost::thread thread(boost::bind(&TransmitThread::Run,&transmitThread,ip,data,size));    
}
