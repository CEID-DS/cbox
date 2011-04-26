/*
 * NetReceiver.h
 *
 *  Created on: Apr 25, 2011
 *      Author: kurgan
 */

#ifndef NETRECEIVER_H_
#define NETRECEIVER_H_

#include <iostream>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <netinet/in.h>
#include <resolv.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

class NetReceiver {
protected:
	pthread_t receive_thread;
public:
	NetReceiver();
	virtual ~NetReceiver();
	void Receiver();
private:
	int status;
	static void* Working(void *t);
};

#endif /* NETRECEIVER_H_ */
