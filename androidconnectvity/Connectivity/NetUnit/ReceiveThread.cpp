/* *
 * File:   ReceiveThread.cpp
 * Author: carnage
 * 
 * Created on May 12, 2011, 1:55 PM
 */

#include "ReceiveThread.h"

ReceiveThread::ReceiveThread(int mynewsock) {
    newsock=mynewsock;
}

ReceiveThread::~ReceiveThread() {
}

void ReceiveThread::Run(){
    cout<<"Accepted";
    
    char get_data[512];
    long size;
    long checks;
    long left=0;
    
    recv(newsock,&size,sizeof(size),0);

    cout<<"recv "<<size<<endl;

    send(newsock,&checks,sizeof(checks),0);

    cout<<"send"<<endl;

    char data[size];
    
    cout<<size<<endl;

    for (int i = 0; i < size / 512; i++) {
        recv(newsock, get_data, sizeof (get_data), 0);
        memcpy(data + left, get_data, 512);
        left = left + 512;

        send(newsock, &checks, sizeof (checks), 0);
    }
    
        cout<<left<<endl;

    if(size%512!=0){
        cout<<"receiving "<<size-left<<endl;
        ///recv(newsock,get_data,sizeof(get_data),0);
        //memcpy(data+left,get_data,size-left);
        //send(newsock,&checks,sizeof(checks),0);
        //cout<<"end "+left<<endl;
    }


    close(newsock);
       
}
