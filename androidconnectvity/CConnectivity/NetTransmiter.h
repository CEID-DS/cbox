/*
 * NetTransmiter.h
 *
 *  Created on: Apr 24, 2011
 *      Author: Emmanouil Gkatziouras
 */

#ifndef NETTRANSMITER_H_
#define NETTRANSMITER_H_

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
#include <sstream>
#include <fstream>
#include <sys/stat.h>

class NetTransmiter {
protected:
	pthread_t transmit_thread;
public:
	NetTransmiter();
	virtual ~NetTransmiter();
	void Transmiter(std::string ip,std::string file);
private:
	int status;
	static void* Working(void *t);
};

#endif /* NETTRANSMITER_H_ */
