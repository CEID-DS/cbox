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
 * *******************************************************************************/

/*
 * NetVisible.h
 *
 *  Created on: Apr 23, 2011
 *      Author: gkatzioura
 */

#ifndef NETVISIBLE_H_
#define NETVISIBLE_H_

#include <iostream>
#include <fcntl.h>
#include <string>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <netinet/in.h>
#include <resolv.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <ifaddrs.h>

class NetVisible {
protected:
	pthread_t broadcast_thread;
public:
	NetVisible();
	virtual ~NetVisible();
	void Broadcast();
	char my_ip[INET_ADDRSTRLEN];
private:
    int status;
	static void* Working(void* t);
	void Getlocalip();
	void GetBluemac();
	
};

#endif /* NETVISIBLE_H_ */
