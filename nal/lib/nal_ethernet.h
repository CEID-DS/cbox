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

class Ethernet{

	public:
		Ethernet();
		int enable();
		int disable();
		int send(address addr,char *data,int size);
		
	private:
		static int sockfd;//socket's file descriptor
		static address addr;//used for server initialization
		static char data_buffer[1024];//buffer of the data received
		static int instances;//count of instances
		static const int max_instances;//max number of instances that can be created
};
		
#endif

