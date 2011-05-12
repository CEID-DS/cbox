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
    ReceiveThread receivethread;
    
    
    
    boost::thread thr(boost::bind(&ReceiveThread::Run,&receivethread));
    
}