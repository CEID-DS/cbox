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

/*
 * WifiListener.h
 *
 *  Created on: Apr 13, 2011
 *      Author: gkatzioura
 */

#ifndef WIFILISTENER_H_
#define WIFILISTENER_H_

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

namespace cconnectivity{
class WifiListener {
protected:
	pthread_t listen_thread;
public:
	WifiListener();
	virtual ~WifiListener();
	void Listener();
private:
	int status;
	static void* Working(void* t);
};
}
#endif /* WIFILISTENER_H_ */
