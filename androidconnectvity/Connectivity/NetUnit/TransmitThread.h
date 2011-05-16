/* 
 * File:   TransmitThread.h
 * Author: emmanouil
 *
 * Created on May 12, 2011, 6:09 PM
 */

#ifndef TRANSMITTHREAD_H
#define	TRANSMITTHREAD_H

#include <iostream>
#include <fcntl.h>
#include <fstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include "../GeneralFunctions.h"

using namespace std;

class TransmitThread {
public:
    TransmitThread(string myip,char *mydata,int mysize);
    virtual ~TransmitThread();
    void Run(string ip,char *data,long size);
private:
    //string ip;
    //char *data;
    //int size;
};

#endif	/* TRANSMITTHREAD_H */

