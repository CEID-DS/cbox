/* 
 * File:   ReceiveThread.h
 * Author: carnage
 *
 * Created on May 12, 2011, 1:55 PM
 */

#ifndef RECEIVETHREAD_H
#define	RECEIVETHREAD_H

#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

using namespace std;

class ReceiveThread {
public:
    ReceiveThread(int mynewsock);
    virtual ~ReceiveThread();
    void Run();
private:
    int newsock;
};

#endif	/* RECEIVETHREAD_H */

