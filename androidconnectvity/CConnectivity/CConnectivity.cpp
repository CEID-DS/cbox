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
 * CConnectivity.cpp
 *
 *  Created on: Apr 13, 2011
 *      Author: gkatzioura
 */

#include "WifiListener.h"
#include "NetVisible.h"
#include "NetReceiver.h"
#include "NetTransmiter.h"
#include "BlueUnit.h"
#include <cstring>
#include <iostream>
#include <strings.h>
#include <pthread.h>
#include <unistd.h>


WifiListener* wifiListener = new WifiListener();
NetVisible* netVisible = new NetVisible();
NetReceiver* netReceiver = new NetReceiver();
NetTransmiter* netTransmiter = new NetTransmiter();
BlueUnit* blueUnit = new BlueUnit();

//void listener(void);

int main()
{
	char t;

	std::string command;

	std::cout<<"Helper App for android debugging"<<std::endl;

	//WifiListener* wifiListener = new WifiListener();

	//wifiListener->Listener();

	for(;;)
	{
		std::cout<<"debugshell$ ";
		std::cin>>command;

		if(command.find("netlistener",0)==0){
			//std::cout<<command<<std::endl;
			wifiListener->Listener();	

		}
		else if(command.find("exit",0)==0){
			exit(1);
		}
		else if(command.find("netvisible",0)==0){
			//std::cout<<"asd"<<std::endl;
			netVisible->Broadcast();
		}
		else if(command.find("netreceiver",0)==0){
			netReceiver->Receiver();
		}
		else if(command.find("nettransmiter",0)==0){
			netTransmiter->Transmiter("192.168.1.9","testa.mp3");
		}
		else if(command.find("nettosend",0)==0){
			netVisible->Tosent("192.168.1.9",0);
		}
		else if(command.find("netrequest",0)==0){
			netVisible->Tosent("192.168.1.9",1);
		}
		else if(command.find("scanbluetooth",0)==0){
			//std::cout<<" Sacn bluetooth"<<std::endl;
			blueUnit->Linker("scanbluetooth");
		}
		else if(command.find("bluereceiver")==0){
			blueUnit->Linker("bluereceiver");	
		}
		else
			std::cout<<"No such command"<<std::endl;
	}
	return 0;
}

