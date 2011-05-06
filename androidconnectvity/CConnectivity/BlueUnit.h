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
 * BlueUnit.h
 *
 *  Created on: May 3, 2011
 *      Author: gkatzioura
 */

#ifndef BLUEUNIT_H_
#define BLUEUNIT_H_

#include <sys/types.h>
#include <sys/stat.h>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include "StringFunctions.h"

class BlueUnit {
protected:
	pthread_t discover_thread;
	pthread_t receive_thread;
	pthread_t send_thread;
public:
	BlueUnit();
	virtual ~BlueUnit();
	void Linker(std::string option);
	void BlueReceiver();
	void BlueTransmiter(std::string mac,std::string file);
	struct Bluedevice{
		char addr[19];
		char name[248];
	};

	char* buffer; 	
	
private:
	static void* Discover(void *t);
	static void* Transfer(void *t);
	static void* Transmit(void *t);
};

#endif /* BLUEUNIT_H_ */
