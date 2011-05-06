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

class Ethernet{

	public:
		int enable();
		int disable();
		int send(address addr,char *data,int size);
		
	private:
		static int sockfd;//socket's file descriptor
		static sockaddr_in address;//used for server initialization and they used for communication with others
		static char data_buffer[1024];//buffer of the data received
};
		
#endif

