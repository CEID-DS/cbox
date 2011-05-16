/* 
 * File:   NetUnit.h
 * Author: carnage
 *
 * Created on May 12, 2011, 1:54 PM
 */

#ifndef NETUNIT_H
#define	NETUNIT_H

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <resolv.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>
#include <boost/thread.hpp>
#include "ReceiveThread.h" 
#include "TransmitThread.h"

using namespace std;

class NetUnit {
public:
    NetUnit();
    virtual ~NetUnit();
    void Receive();
    void Transmit(string ip,char *data,long size);
private:
};

#endif	/* NETUNIT_H */

