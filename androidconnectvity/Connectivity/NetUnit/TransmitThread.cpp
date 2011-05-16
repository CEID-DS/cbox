/* 
 * File:   TransmitThread.cpp
 * Author: emmanouil
 * 
 * Created on May 12, 2011, 6:09 PM
 */

#include "TransmitThread.h"

TransmitThread::TransmitThread(string myip,char *mydata,int mysize) {
}

//netransmit 123.123.234.234 test.mp3 234

TransmitThread::~TransmitThread() {
}

void TransmitThread::Run(string ip,char *data,long size){
    
    cout<<"Transmiter Started"<<endl;
    
    int sock;
    int addr_len,bytes_read;
    
    char send_data[512];
    struct sockaddr_in server_addr;
 
    ofstream new_file ("new.mp3",ios::out|ios::binary);
    
    new_file.write(data,size);
    
    new_file.close();
            
    socklen_t addr_size = 0;
    
    sock = socket(AF_INET,SOCK_STREAM,0);
    
    server_addr.sin_family =  AF_INET;
    server_addr.sin_port = htons(9876);
    
    //cout<<"Running at "<<ip<<endl;
    
    server_addr.sin_addr.s_addr = inet_addr(stringtochar(ip));
            //"192.168.1.71");
            //stringtochar(ip));
    
    addr_len=sizeof(server_addr);
    
    if(connect(sock,(struct sockaddr *)&server_addr,addr_len)!=-1){
    
        int checks;
        long left=0;
        
        cout<<"Connected"<<endl;
    
        send(sock,&size,sizeof(size),0);
        
        recv(sock,&checks,sizeof(checks),0);
    
        cout<<size<<endl;
        
        for(int i=0;i<size/512;i++){
            memcpy(send_data,data+left,512);
            left=left+512;
            
            send(sock,send_data,sizeof(send_data),0);    
            recv(sock,&checks,sizeof(checks),0);
            //cout<<left<<endl;
        }
        
        cout<<left<<endl;
        
        if(size%512!=0){
            cout<<"sending"<<size-left<<endl;
            memcpy(send_data,data+left,size-left);
            send(sock,send_data,sizeof(send_data),0);
            recv(sock,&checks,sizeof(checks),0);
            cout<<"end "<<left<<endl;
        }
       
    }
    else{
        cout<<"prob with connection"<<endl;
    }
        
}