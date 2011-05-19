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
*******************************************************************************/

#ifndef NAL_ETHERNET_H
#define NAL_ETHERNET_H

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include "nal_address.h"
#include "nal_delegate.h"
#include <pthread.h>
#include <string.h>

#define MAX_INSTANCES 20

class Ethernet{

	public:
		Ethernet();
		int enable();
		int disable();
		int send(char *data,int size);
		
		static void call_delegates();
		static delegate dels[MAX_INSTANCES];
		
		static char data_buffer[8192];
		static int valid_data;
		
	private:
		static int sockfd;//socket's file descriptor
		static address addr;//used for server initialization
		static int instances;//count of instances
		
		static bool valid_dels[MAX_INSTANCES];//which delegates are valid in dels array (true for valid)
};

void *receive_routine(void *);//routine of receiver thread
		
#endif

