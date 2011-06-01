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

#ifndef PC_NAL_ETHERNET_H
#define PC_NAL_ETHERNET_H

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include "pc_nal_address.h"
#include "pc_nal_delegate.h"
#include <pthread.h>
#include <string.h>

#define MAX_INSTANCES 20

template<typename OsModel_P>
class Ethernet{
typedef OsModel_P OsModel;
	
	public:
		Ethernet(){

			if (instances==0){
				//initialize valid_dels boolean array
				for (int i=0;i<MAX_INSTANCES;i++){
					valid_dels[i]=false;
				}
			}
		}
		
		int enable(){

			if (instances < MAX_INSTANCES){
				//socket is initialized the first time an instance of the class is created
				if (sockfd == -2){
				  	if ((sockfd = socket(PF_INET, SOCK_DGRAM, 0) ) < 0){ //if socket failed to initialize
						sockfd=-2;
				      		return -1;
				    	}
				    	
				    	int on=1;
				    	if (setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST,  &on, sizeof(on)) < 0){
				    		sockfd=-2;
				    		return -1;
				    	}
				    	struct sockaddr_in sin;
			
					memset(&sin, 0, sizeof sin);
			
					sin.sin_family = PF_INET;
					sin.sin_addr.s_addr = INADDR_BROADCAST;
					sin.sin_port = htons(9876);
			
					if (bind(sockfd,(struct sockaddr *)&sin, sizeof(sin)) < 0 ){
						close(sockfd);
						sockfd=-2;

						return -1;
					}
			
					pthread_t tid;
			
					if(pthread_create(&tid,NULL,&receive_routine,(void*) sockfd)!=0){//create receive thread
						close(sockfd);
						sockfd=-2;
						return -1;
					}
			
					pthread_detach(tid);
				}
		
				instances++;
				return 0;
			}else{
				return -1;
			}
		}		


		int  send(char *data,int size){
	
			struct sockaddr_in sin;
			
			memset(&sin, 0, sizeof sin);
	
			sin.sin_family = PF_INET;
			sin.sin_addr.s_addr = INADDR_BROADCAST;
			sin.sin_port = htons(9876);
	
			return sendto(sockfd, data, size, 0,(struct sockaddr*)&sin, sizeof sin);
		}

		int  disable(){

			instances--;
	
			if (instances==0){//when there are no instances of the class close socket
		
				close(sockfd);
				sockfd=-2;//back to startup state
			}
			return 0;
		}

		void  unregister_receiver(){
			valid_dels[del_id] = false;
		}
		
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
			
		static void *receive_routine(void *socket){

			struct sockaddr_in sa;
			socklen_t fromlen;
	
			for (;;){
				if ( (valid_data=recvfrom((int) socket, (void *) data_buffer, 8192, 0, (struct sockaddr *)&sa, &fromlen) ) <= 0) {
					pthread_exit(NULL);
				}
	
				call_delegates();
			}
		}
		
		static void  call_delegates(){
			for (int i=0;i<MAX_INSTANCES;i++){
				if (valid_dels[i]){
					dels[i](data_buffer,valid_data);
				}
			}
		}
			
	private:
		int del_id;
		static delegate  dels[MAX_INSTANCES];
		static char  data_buffer[8192];
		static int  valid_data;
	
		static int sockfd;//socket's file descriptor
		static address addr;//used for server initialization
		static int instances;//count of instances
		
		static bool valid_dels[MAX_INSTANCES];//which delegates are valid in dels array (true for valid)
};

template<typename OsModel_P> int Ethernet<OsModel_P>::sockfd=-2;
template<typename OsModel_P> int Ethernet<OsModel_P>::instances=0;
template<typename OsModel_P> bool Ethernet<OsModel_P>::valid_dels[MAX_INSTANCES];
template<typename OsModel_P> delegate Ethernet<OsModel_P>::dels[MAX_INSTANCES];
template<typename OsModel_P> char Ethernet<OsModel_P>::data_buffer[8192];
template<typename OsModel_P> int Ethernet<OsModel_P>:: valid_data=0;

#endif

