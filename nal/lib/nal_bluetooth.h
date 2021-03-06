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
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with cbox. If not, see <http://www.gnu.org/licenses/>.
*******************************************************************************/

#ifndef NAL_BLUETOOTH_H
#define NAL_BLUETOOTH_H

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include "nal_address.h"
#include "nal_delegate.h"
#include <pthread.h>
#include <string.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include <cstdlib>

#define MAX_INSTANCES 20
#define SIZE 1024
#define PORT 1
#define TIME 8
#define MAX_DEV 255

class Bluetooth{

	public:
		Bluetooth();
		int enable();
		int disable();
		int send(char *data,int size);

		template <class T,void (T::*TMethod)(char *,int)>
		void register_receiver(T* object){

			bool found=false;
			int i=0;

			do{
   				if (valid_dels[i]==false){
					dels[i]=delegate::from_method<T,TMethod>(object);
					found=true;
					valid_dels[i]=true;
					del_id=i;
				}
   				i++;
   			}while (!found);
		}

		void unregister_receiver();
		int del_id;

		//static members following

		static void call_delegates();
		static delegate dels[MAX_INSTANCES];
		static char data_buffer[SIZE];
		static int valid_data;

		static int dev_num; //current number of scanned devices
		static address devices[MAX_DEV]; //array that scanned addresses are being saved

	private:
		static int sockfd;//socket's file descriptor
		static address addr;//used for server initialization
		static int instances;//count of instances
		static bool valid_dels[MAX_INSTANCES];//which delegates are valid in dels array (true for valid)
};

void *receive_routine(void *);//routine of receiver thread
void *scan_routine(void *);//routine of scan thread
#endif
