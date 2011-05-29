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

#include "external_interface/external_interface.h"
#include "algorithms/routing/tree/tree_routing.h"
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <definitions.h>
#define JOIN_REQ 0
#define JOIN_ACK 1
#define REPAIR 2
#define JOINED 3
#define LEAVE 4
#define UP 6
#define DOWN 7
#define JOIN_ME 8
#define PING 9
#define PONG 10
#define ALL 11
#define REJOIN_REQ 12
#define REJOIN_ACK 13
#include <dirent.h>





typedef wiselib::OSMODEL Os;
using namespace std;

class TreeApp
{
   public:

struct message
{
short int parent;
short int type;
short int hops;
short int id;
short int version;
short int dest;
short int given;
char from[32];
char Routing[32];
} *mess;

//char *payload;
char *message;









	/**
	 * Struct that contains info about services
	 **/
	struct service
    {
      char service_type[15];
      char protocol[5];
      char interface[10];
      //char ip[20];
      char hostname[20];
      int port;
      char TXT_DATA[20];
      int TTL;
      int TOTAL_TTL;
      bool advertised;
      bool questioned;
    };

   service services[NUM_STRUCTS]; //services from other nodes
   service myServices[NUM_STRUCTS]; //services from this node

	/**
	 * struct for ZeroConf answers
	 **/
   struct ZeroAnswer
   {
      int _TTL;
      char _service_type[15];
      char _protocol[5];
      char _hostname[HOSTNAME_LEN];
      char _ip[32];
      char _data[10];
      size_t _port;
   };
	/**
	 * the main zeroconf message
	 **/
   struct ZeroMessage
   {
	   
	  char source_host[HOSTNAME_LEN];
      char dest_host[HOSTNAME_LEN];
      char type[TYPE_LEN];
	  char source_ip[32];
      char source_MAC[32];
      char questions[2][QUERY_LEN];
      ZeroAnswer answers[NUM_ANSWERS];
      int k;
   };

   struct piggy_query
   {
      char about[QUERY_LEN];
      char _service_type[15];
      char _hostname[20];
   };

   /**
    * initialise service tables
    **/

   void init_service_arrays (void)
   {
      for (int i=0; i<NUM_STRUCTS; i++)
      {
         strcpy(myServices[i].hostname, "null");
         strcpy(services[i].hostname, "null");
         //free_service_index.push_front(i);
      }
   }
   
   
      /**
    * Insert a service that i have in the services table.
    **/
   void insert_my_service (struct service& serv)
   {
      debug_->debug("in insert my service\n");
      int index=-1;

      for (int i=0; i<NUM_STRUCTS; i++)
      {
         if (!strcmp(myServices[i].hostname, "null"))
         {
            index = i;
            break;
         }
      }

      if (index == -1)
      {
         debug_->debug("Den uparxei keni thesi sta myservices\n");
         return;
      }

      myServices[index] = serv;
   }

   /**
    * Insert a service that is being advertised
    **/
   void insert_service (struct ZeroAnswer& answer, char *host)
   {
      //debug_->debug("(%u)inserting service (%s)(%s)\n",radio_->id(),host,answer._service_type);
      //debug_->debug("(%u)Mesa stin insert_service:I answer einai ip:(%s), service_type(%s)\n",radio_->id(),answer._ip,answer._service_type);
      int index = -1;
      //number_of_services++;

      for (int i=0; i<NUM_STRUCTS; i++)
      {
         if (!strcmp(services[i].service_type, answer._service_type) &&
		  !strcmp(services[i].hostname, answer._hostname))
         {
            index = i;
            break;
         }
      }

      if (index == -1)
      {
         for (int i=0; i<NUM_STRUCTS; i++)
         {
            if (!strcmp(services[i].hostname, "null"))
            {
               index = i;
               break;
            }
         }
      }

      if (index == -1)
      {
         //debug_->debug("Den uparxei keni thesi sta services\n");
         return;
      }

      strcpy(services[index].service_type, answer._service_type);
      strcpy(services[index].protocol, answer._protocol);
      //strcpy(services[index].ip, answer._ip);
      strcpy(services[index].hostname, host);
      strcpy(services[index].TXT_DATA, answer._data);
      services[index].port = answer._port;
      services[index].TTL = answer._TTL;
      services[index].TOTAL_TTL = answer._TTL;
      services[index].advertised = true;
      services[index].questioned = false;
//      debug_->debug("(%u)Mesa stin insert_service:I answer einai ip:(%s), service_type(%s) mpike stin %d\n",radio_->id(),answer._ip,answer._service_type,index);
   }


   /**
    * Remove a service from my services or services
    * because it cannot be trusted as valid
    **/
   void remove_services (char *host_serv)
   {
      struct service *array_serv = NULL;

      if (!strcmp(host_serv, my_host))
      {
         array_serv = myServices;
      }
      else
      {
         array_serv = services;
      }

      for (int i=0; i<NUM_STRUCTS; i++)
      {
         if (!strcmp(array_serv[i].hostname, host_serv))
         {
            strcpy(array_serv[i].hostname, "null");
         }
      }
   }

   //remove a single service known for a spesific ip
   void remove_service (char *host_serv,int index)
   {
//    debug_->debug("(%u)i am removing service %s from %s\n",radio_->id(),services[index].service_type,ip_serv);
      //under construction.
      struct service *array_serv = NULL;
      if (!strcmp(host_serv, my_host))
      {
         array_serv = myServices;
      }
      else
      {
         array_serv = services;
      }
      strcpy(array_serv[index].hostname, "null");
   }


       //if a service is not available unregister it
      void unregister_service(int i)
      {
	 //int *i=(int *)pos;
         debug_->debug("(%u) unregister service sti thesi %d \n", radio_->id(),i);

	 ZeroMessage unreg;
     	 //strcpy(unregister_data.source_host, my_host);
         strcpy(unreg.answers[0]._service_type, myServices[i].service_type);

	 strcpy(unreg.type,(char *)UNREGISTER);
	 strcpy(unreg.source_host,my_host);
	 strcpy(unreg.source_ip,RoutN_);

	sendToAll((char *)&unreg,sizeof(unreg));
      }

void unregister_service2(void *){unregister_service(1);}

//TODO: this should change so we load services from the database.
	/**
	 * Loads all services that want to register
	 **/
      void load_services()
      {
		  debug_->debug("%u is loading services\n",radio_->id());
         FILE *fs;
         char command[50];
         char input[50];
         DIR *pDIR;
         struct dirent *entry;

         if (radio_->id()==0 || radio_->id()==1)
         {
            if ((pDIR = opendir("./services")))
            {
               while ((entry = readdir(pDIR)))
               {
                  if (strcmp(entry->d_name, ".") && strcmp(entry->d_name, "..") &&
                     !strcmp(strrchr(entry->d_name, '.'), ".service"))
                  {
                     //debug_->debug("file name: %s\n ", entry->d_name);
                     strcpy(input, "./services/");
                     strcat(input, entry->d_name);
                     //debug_->debug("file destination: %s\n ", input);
                     fs = fopen(input, "r");
                     if (fs == NULL)
                     {
                        //debug_->debug ( "cannot open file" ) ;
                        //exit() ;
                     }
                     else
                     {
                        struct service temp;
                        //debug_->debug ( "(%u)file read(%s)!\n", radio_->id(), entry->d_name) ;
                        //number_of_myServices++;

                        for (int i=0; i<NUMBER_OF_ROWS; i++)
                        {
                           fscanf(fs, "%s", command);
                           fscanf(fs, "%s", input);
                           if (!strcmp(command, "service_type"))
                           {
                              strcpy(temp.service_type, input);
                           }
                           else if (!strcmp(command, "protocol"))
                           {
                              strcpy(temp.protocol, input);
                           }
                           else if (!strcmp(command, "interface"))
                           {
                              strcpy(temp.interface, input);
                           }
                           else if (!strcmp(command, "port"))
                           {
                              temp.port = atoi(input);
                           }
                           else if (!strcmp(command, "TXT_DATA"))
                           {
                              strcpy(temp.TXT_DATA, input);
                           }
                           else if (!strcmp(command, "TTL"))
                           {
                              temp.TTL = atoi(input);
                              temp.TOTAL_TTL = atoi(input);
                              //debug_->debug("(TTL:%d)\n ", temp.TTL);
                           }
                        }
                        temp.advertised = false;
                        strcpy(temp.hostname, my_host);
                        strcpy(temp.hostname, my_host);
                        fclose(fs);
                        insert_my_service(temp);
                     }
                  }
               }
               closedir(pDIR);
            }
         }//end if radio
      }




      //TODO : this will have to change.... load services from database...
      /**
       * This updates the registered services in this machine.
       * (almost the same as above function... they should be merged)
       **/
      void update_myServices()
      {
         debug_->debug("%u in update my services\n", radio_->id());
         FILE *fs ;
         char command[50];
         char input[50];
         /*
         if (radio_->id() == 0)
            fs = fopen ( "input","r" ) ;
         else
            fs = fopen ( "input1","r" ) ;
         */
         DIR *pDIR;
         struct dirent *entry;

         bool check[NUM_STRUCTS];
         for (int i=0; i<NUM_STRUCTS; i++)
         {
            check[i] = false;
         }

         if (radio_->id()==0 || radio_->id()==1)
         {
            if ( (pDIR=opendir("./services")) )
            {
               for(int i=0; i<NUM_STRUCTS; i++)
               {
                  if (strcmp(myServices[i].hostname, "null"))
                     printf("first %u: service %d = %s\n", radio_->id(), i, myServices[i].service_type);
               }
               //number_of_myServices = 0;
               while ((entry = readdir(pDIR)))
               {
                  if ( strcmp(entry->d_name, ".") && strcmp(entry->d_name, "..") &&
                     !strcmp(strrchr(entry->d_name, '.'),".service"))
                  {
                     //debug_->debug("file name: %s\n ", entry->d_name);
                     strcpy(input, "./services/");
                     strcat(input, entry->d_name);
                     //debug_->debug("file destination: %s\n ", input);
                     fs = fopen ( input, "r" ) ;

                     if ( fs == NULL )
                     {
                        //debug_->debug ( "cannot open file" ) ;
                        //exit() ;
                     }
                     else
                     {
                        bool added = false;
                        fscanf(fs, "%s", command);
                        fscanf(fs, "%s", input);
                        for (int i=0; i<NUM_STRUCTS; i++)
                        {
                           if (!strcmp(myServices[i].service_type, input) && (!strcmp(myServices[i].hostname, my_host)))
                           {
                              for (int j=0; j<NUMBER_OF_ROWS-1; j++)
                              {
                                 fscanf(fs, "%s", command);
                                 fscanf(fs, "%s", input);

                                 if (!strcmp(command, "TTL"))
                                 {
                                    myServices[i].TTL = atoi(input);
                                    myServices[i].TOTAL_TTL = atoi(input);
                                 }
                              }
                              check[i] = true;
                              added = true;
                           }
                        }
                        for (int i=0; i<NUM_STRUCTS; i++)
                        {
                           if (!strcmp(myServices[i].hostname, "null") && (added == false))
                           {
                              printf("adding in %d\n", i);
                              printf("ip = %s\n added = %s\n", myServices[i].hostname, (added)?"true":"false");
                              for (int m=0; m<NUMBER_OF_ROWS-1; m++)
                              {
                                 if (!strcmp(command, "service_type"))
                                 {
                                    strcpy(myServices[i].service_type, input);
                                 }
                                 else if (!strcmp(command, "protocol"))
                                 {
                                    strcpy(myServices[i].protocol, input);
                                 }
                                 else if (!strcmp(command, "interface"))
                                 {
                                    strcpy(myServices[i].interface, input);
                                 }
                                 else if (!strcmp(command, "port"))
                                 {
                                    myServices[i].port = atoi(input);
                                 }
                                 else if (!strcmp(command, "TXT_DATA"))
                                 {
                                    strcpy(myServices[i].TXT_DATA, input);
                                 }
                                 else if (!strcmp(command, "TTL"))
                                 {
                                    myServices[i].TTL = atoi(input);
                                    myServices[i].TOTAL_TTL = atoi(input);
                                 }
                                 fscanf(fs, "%s", command);
                                 fscanf(fs, "%s", input);
                              }
                              myServices[i].advertised = false;
                              strcpy(myServices[i].hostname, my_host);
                              check[i] = true;
                              added = true;
                           }
                        }
                        fclose(fs);
                     }
                  }
               }
               for (int l=0; l<NUM_STRUCTS; l++)
               {
                  if ((check[l]==false) && (strcmp(myServices[l].hostname, "null")))
                  {
                     unregister_service(l);
                  }
               }
            }
            closedir(pDIR);
         }
	 }



void print_my_services() {
			int count=0;
		for (int i=0; i<NUM_STRUCTS; i++) //for all other services you know
         {
            if (!strcmp(services[i].hostname, "null"))
            {
               continue;
		    }
		    count++;
		    debug_->debug("****(%u)service %d ******\n",radio_->id(),count);
		    debug_->debug("service type: %s\n",myServices[i].service_type);
		    debug_->debug("protocol: %s\n",myServices[i].protocol);
		    debug_->debug("interface: %s\n",myServices[i].interface);
		    debug_->debug("hostname: %s\n",myServices[i].hostname);
		    debug_->debug("TXT DATA: %s\n",myServices[i].TXT_DATA);
		    debug_->debug("Total TTL: %d\n",myServices[i].TOTAL_TTL);
		    debug_->debug("remaining TTL: %d\n",myServices[i].TTL);
	}
	
}


void print_known_services() {
		int count=0;
		for (int i=0; i<NUM_STRUCTS; i++) //for all other services you know
         {
            if (!strcmp(services[i].hostname, "null"))
            {
               continue;
		    }
		    count++;
		    debug_->debug("****(%u)service %d ******\n",radio_->id(),count);
		    debug_->debug("service type: %s\n",services[i].service_type);
		    debug_->debug("protocol: %s\n",services[i].protocol);
		    debug_->debug("interface: %s\n",services[i].interface);
		    debug_->debug("hostname: %s\n",services[i].hostname);
		    debug_->debug("TXT DATA: %s\n",services[i].TXT_DATA);
		    debug_->debug("Total TTL: %d\n",services[i].TOTAL_TTL);
		    debug_->debug("remaining TTL: %d\n",services[i].TTL);
	}
}



		/**
		 * Check the TTL value of known services
		 * if it is too low then take action
		 **/
	  void checkTTLforServices()
      {
		  print_known_services();
         piggy_query queries[NUM_ANSWERS]; //NUM_ANSWERS is the number of answers a message has
         int num_queries = 0;

         for (int i=0; i<NUM_STRUCTS; i++) //for all other services you know
         {
            if (!strcmp(services[i].hostname, "null"))
            {
               continue;
            }
            services[i].TTL = services[i].TTL - recallTime; //update TTL
            
            //debug_->debug("(%u)(%s)service(%s) expired, sending query %d , %f >= %f ?\n", radio_->id(), services[i].hostname, services[i].service_type, services[i].TTL,(float)services[i].TOTAL_TTL - (float)services[i].TTL,3*(float)services[i].TOTAL_TTL/4);

            if ((float)services[i].TOTAL_TTL - (float)services[i].TTL >=3*(float)services[i].TOTAL_TTL/4)//if TTL is equal or less than 1/4 of original TTL
            {
               if (strcmp(services[i].hostname, my_host) && !services[i].questioned) //if query has not been sent yet
               {
                  if (num_queries < NUM_ANSWERS)
                  {
					  debug_->debug("sending query for %s %s\n",services[i].service_type,services[i].hostname);
                     strcpy(queries[num_queries].about, "service"); //fill query parameters with the given
                     strcpy(queries[num_queries]._service_type, services[i].service_type);
                     strcpy(queries[num_queries]._hostname, services[i].hostname);
                     num_queries++;
                     //send_question_message("service", services[i].service_type, services[i].ip); //send the query
                     services[i].questioned = true;//query is sented
                  }
                  else
                  {
                     break;
                  }
               }
            }
            if ((float)services[i].TOTAL_TTL - (float)services[i].TTL ==(float)services[i].TOTAL_TTL)
            {
//             debug_->debug("(removing) TTL for %s -> %d",services[i].service_type,services[i].TTL);
               remove_service(services[i].hostname, i);
            }
         }

         if (num_queries>0)
         {
            send_question_message(queries, num_queries); //send the query
         }

         for (int i=0; i<NUM_STRUCTS; i++) //for all your services
         {
            if (!strcmp(myServices[i].hostname, "null"))
            {
               continue;
            }
            myServices[i].TTL = myServices[i].TTL - recallTime; //update TTL

            if ((float)myServices[i].TOTAL_TTL - (float)myServices[i].TTL >= 3*(float)myServices[i].TOTAL_TTL/4) //if TTL is equal or less than zero
            {
               update_myServices(); //update your services
             //  debug_->debug("(%u)(%s)myService expired %f\n", radio_->id(), myServices[i].ip, services[i].TTL);
            }
         }
      }

 
  
  
  
  
        //resolve hostname conflict
      void resolve_hostname(char my_host[HOSTNAME_LEN], int trial)
      {
         char temp_host[HOSTNAME_LEN];
         char *n;
         char tr[3];
         int l = strlen(my_host);
         strcpy(temp_host, my_host);
         srand(time(NULL)+radio_->id());    
         snprintf(tr, 3, "%d", (rand() % 100));
         //sprintf(n, "%s", char(trial));
         if (l < HOSTNAME_LEN - 3 - strlen(tr))
         {
            if (trial)
            {
               n = strrchr(temp_host, '_');
            }
            else
            {
               n = strrchr(temp_host, '.');
            }
            int y = (int)(n-temp_host+1);
            snprintf(temp_host, y, "%s", my_host);
            bzero(my_host, HOSTNAME_LEN);
            snprintf(my_host, HOSTNAME_LEN, "%s_%s.loc", temp_host, tr);
         }
         else
         {

         }
      }





/**
 * this function is used to advertise the services that needs to be advertised
 **/
      void advertise_services() 
      {//advertise services (who are not advertised yet or need to advertise because of a query)
         
         ZeroMessage advertise;
         memset(&advertise, 0, sizeof(advertise));
         strcpy(advertise.type,(char *)&(ADVERTISE));
         strcpy(advertise.source_host,my_host);
         strcpy(advertise.source_MAC,my_MACC);
         
         for(int i=0; i<NUM_ANSWERS; i++) //initialization, it is useful for the handling of message
         {
            strcpy(advertise.answers[i]._service_type,"null");
         }

         //strcpy(hello.dest_host,my_host);
         int services_to_advertise = 0;

         for (int i=0; i<NUM_STRUCTS;i++) //for all your services
         {
            if (!strcmp(myServices[i].hostname, "null"))
            { 
               continue;
            }
				
            if (!myServices[i].advertised) //if it needs to advertise it
            {//fill the service struct
               if (services_to_advertise<NUM_ANSWERS)
               {
//                  debug_->debug("(%s)advertising app: %s\n", my_ip,myServices[i].service_type);
                  strcpy(advertise.answers[services_to_advertise]._service_type, myServices[i].service_type);
                  strcpy(advertise.answers[services_to_advertise]._protocol, myServices[i].protocol);
                  advertise.answers[services_to_advertise]._port = myServices[i].port;
                  strcpy(advertise.answers[services_to_advertise]._data, myServices[i].TXT_DATA);
                  advertise.answers[services_to_advertise]._TTL = myServices[i].TTL;
                  
                  strcpy(advertise.answers[services_to_advertise]._hostname, my_host);
                  //advertise_data.answers[services_to_advertise]._k = 5;
                  services_to_advertise++;
                  myServices[i].advertised = true; //this service marked as advertised
                  //advertise_service(services[i]);
               }
            }
         }
         /*
         for (int i=0; i<NUM_ANSWERS && strcmp(advertise_data.answers[i]._service_type, "null"); i++)
            debug_->debug("(%u)myService_%d:%s TTL:%d PORT:%u \n", radio_->id(), i, advertise_data.answers[i]._service_type, advertise_data.answers[i]._TTL, advertise_data.answers[i]._port); 
         */
		
	     if (services_to_advertise > 0) //if you have services to advertise, advertise them
         {
			debug_->debug("%s advertising its services , %u\n",my_host,radio_->id());
            sendToAll((char *)&advertise,sizeof(advertise));
         }
      }




/**
 * zeroconf type of messages
 **/
	  /**
	   * With this message a process tries to get a hostname so 
	   * it can participate in the network.
	   **/
	  void send_participate_message() 
      {
		  debug_->debug("send participate message %u\n",radio_->id());
        ZeroMessage partic;
	    memset(&partic, 0, sizeof(partic));
        strcpy(partic.type,(char *) &(PARTICIPATE));
        strcpy(partic.source_host,(char *)"unregistered");
        strcpy(partic.dest_host,(char *)my_host);
        debug_->debug("\nsending %s , %s, %s\n",partic.type,partic.source_host,partic.dest_host);
        sendToAll((char *)&partic,sizeof(partic));
      }

	void send_hello_message() 
    {
		debug_->debug("%s,%u,%s sending hello message",RoutN_,radio_->id(),my_host);
			ZeroMessage hello;
			
			strcpy(hello.source_ip,RoutN_);
			strcpy(hello.type,(char *)HELLO);
			strcpy(hello.source_MAC,my_MACC);
			strcpy(hello.source_host,my_host);
			sendToAll((char *)&hello,sizeof(hello));
    }	
    
    void send_welcome_message(char* dest_ip) 
    {
         ZeroMessage welcome;
         
         strcpy(welcome.type,(char *)WELCOME);
         strcpy(welcome.source_host,my_host);
         send_message(dest_ip, (char *)&welcome,sizeof(welcome));
    }


	void send_question_message(piggy_query piggy_questions[], int num_queries) 
      {
		 debug_->debug("%u,%s sending query message\n",radio_->id(),my_host);
		ZeroMessage query;
		strcpy(query.type,(char *)QUERY);
		strcpy(query.source_host,my_host);
		strcpy(query.source_ip,RoutN_);
		strcpy(query.source_MAC,my_MACC);
	
	     for (int i=0; i<NUM_ANSWERS; i++)
         {
            strcpy(query.questions[i], "null");
         }

         for (int i=0; i<num_queries; i++)
         {
            //debug_->debug("(%u)(%s) stelnw service(%s) request\n", radio_->id(), piggy_questions[i]._ip, piggy_questions[i]._service_type);	
            strcpy(query.questions[i], piggy_questions[i].about); //fill query parameters with the given
            strcpy(query.answers[i]._service_type, piggy_questions[i]._service_type);
            strcpy(query.answers[i]._hostname, piggy_questions[i]._hostname);
         }
         strcpy(query.source_host, my_host);
         sendToAll((char *)&query,sizeof(query));
         //radio_->send( Os::Radio::BROADCAST_ADDRESS, sizeof(zeroconf_msg), (Os::Radio::block_data_t *)&queryMessage);
      }


	void send_leaving_message(void *)
      {

	 //set_node_message_id(my_MACC,++msgID);
         ZeroMessage leaving;
         //memset(&leaving,0,sizeof(leaving));
	 strcpy(leaving.type,(char *)LEAVING);
	 strcpy(leaving.source_host,my_host);
	 strcpy(leaving.source_ip,RoutN_);
	 //leaving.set_type(TYPE_LEN * sizeof(char), (Os::Radio::block_data_t *) &LEAVING);
	 //leaving.set_source(IP_LEN * sizeof(char),(Os::Radio::block_data_t *) &my_ip);
	 //leaving.set_destination(IP_LEN * sizeof(char),(Os::Radio::block_data_t *) &("ANY"));
         //leaving.set_source_mac(MAC_LEN * sizeof(char),(Os::Radio::block_data_t *) &my_MACC );
	 //leaving.set_msg_id(msgID);

	 //msg_data leaving_data;
	 //strcpy(leaving_data.source_host,my_host);
	 //strcpy(hello.dest_host,my_host);

	 //leaving.set_payload(sizeof(msg_data),(Os::Radio::block_data_t *) &leaving_data);

	 //radio_->send( Os::Radio::BROADCAST_ADDRESS, sizeof(zeroconf_msg), (Os::Radio::block_data_t *)&leaving);
	sendToAll((char *)&leaving,sizeof(leaving));

	init_service_arrays();
	//number_of_myServices=0;
	//number_of_services=0;
	//mode=1;
	//counter=0;

      }
      
      
      
      
      

/**
 * handlers
 **/
 

      void handle_participate_message(ZeroMessage* message)
      {
		  if (radio_->id()==0)
			debug_->debug("handle participate message\n");
		if (mode == 0) //if node is in the network
        {//if you receive an other message
		if (radio_->id()==0)
			//debug_->debug("participate, received %s,%s,%s == %s,%u\n",message->type,message->source_host,message->dest_host,my_host,radio_->id());
            if (!strcmp(message->dest_host, my_host))
            {
			      ZeroMessage reply;
			      memset(&reply, 0, sizeof(reply));
				  strcpy(reply.type,(char *)PARTICIPATE);
				  strcpy(reply.source_host,my_host);
				  sendToAll((char *)&reply,sizeof(reply));
				  debug_->debug("sending reply\n");
			} 
        } else {
			if (!strcmp(message->source_host, my_host))
            {
			   //debug_->debug("participate, has my ip %s,%s,%s == %s,%u\n",message->type,message->source_host,message->dest_host,my_host,radio_->id());
			   counter = 0;
               resolve_hostname(my_host, trial_num);
               debug_->debug("my new hostname is %s\n",my_host);
               trial_num = 1;
            }
		}
      }

      void handle_hello_message(ZeroMessage* message)
      {
		debug_->debug("handling hello message %s,%s,%u\n",message->type,message->source_host,radio_->id());
		if (mode==0)
		send_welcome_message(message->source_ip); //and send welcome message
      }
      
	  void handle_welcome_message(ZeroMessage* message)
      {
		  debug_->debug("received welcome from %s,%s,%u\n",message->source_host,my_host,radio_->id());
	  }


      void handle_advertise_message(ZeroMessage* message)
      {
		  debug_->debug("handling advertise message %s,%u\n",message->source_host,radio_->id());
		for(int i=0; i<NUM_ANSWERS && strcmp(message->answers[i]._service_type, "null"); i++)
		{
            insert_service(message->answers[i], message->source_host);
        }
	  }
	  
	  
	  void handle_query_message(ZeroMessage* message)
      {
		  if (radio_->id()==1)
		  debug_->debug("handling query message %s,%u\n",message->source_host,radio_->id());
			
         //debug_->debug("(%u) mpainw stin handle_query apo ton %s\n", radio_->id(), (char *) message.source_mac());
		 for (int i=0; i<NUM_ANSWERS && strcmp(message->questions[i], "null"); i++)
         {
            if (!strcmp(message->questions[i], "service")) //this will go to a separate handle especially for question "service"
            {
               for (int j=0; j<NUM_STRUCTS; j++)
               {
                  if (!strcmp(services[i].hostname, "null"))
                  { 
                     continue;
                  }
                  if (!strcmp(message->answers[i]._service_type, services[j].service_type) && !strcmp(message->answers[i]._hostname,services[j].hostname))
                  {
                     services[j].questioned = true;
                  }
               }
               for (int j=0; j<NUM_STRUCTS; j++)
               {
                  if (!strcmp(myServices[j].hostname, "null"))
                  { 
                     continue;
                  }
				  if (!strcmp((char *)message->answers[i]._service_type, myServices[j].service_type) && !strcmp((char *)message->answers[i]._hostname, myServices[j].hostname))
                  {
					  debug_->debug("%u wow mine!!\n",radio_->id());
                     myServices[j].advertised = false;
                  }
               }
            }
         }
      }


	void handle_leaving_message(ZeroMessage* message)
	{
		debug_->debug("(%u)receive leaving from (%s)\n", radio_->id(),message->source_host);
		 
		//debug_->debug("mpainw sto exist\n");
		remove_services(message->source_host);

	}


	void handle_unregister_service_message(ZeroMessage* message)
      {
         debug_->debug("(%u) in handle unregister message\n",radio_->id());
        // msg_data m_data;
         //memcpy(&m_data, message.payload(), message.payload_size() );
         
         for (int i=0; i<NUM_STRUCTS; i++)
         {
            if (!strcmp(services[i].service_type, (message->answers[0])._service_type) && (!strcmp(services[i].hostname, message->source_host)) )
            {
               strcpy(services[i].hostname, "null");
               debug_->debug("%u service %s from %s deleted\n", radio_->id(), services[i].service_type, message->source_host); 
               break;
            }
         }
      }






    /**
     * handler for zeroconf messages.
     **/
    void handle_ZeroMessage(ZeroMessage *zeroMessage) {
		
		if (!strcmp(zeroMessage->type, PARTICIPATE))
		{//if it is a participate message
		   handle_participate_message(zeroMessage);
		}//advertise	
		else if (!strcmp(zeroMessage->type, HELLO))
        {//if it is a hello message then welcome the new device
			handle_hello_message(zeroMessage);
        }
        else if (!strcmp(zeroMessage->type, WELCOME))
        {//if it is a welcome message
			handle_welcome_message(zeroMessage);
        } 
        else if (!strcmp(zeroMessage->type, ADVERTISE))
        {//if it is an advertisement message
               		handle_advertise_message(zeroMessage);
        }//advertise
        else if (!strcmp(zeroMessage->type, QUERY))
        {//if it is a participate message
			handle_query_message(zeroMessage);
        }
		else if (!strcmp(zeroMessage->type,LEAVING))
        { //if it is a leaving message
			handle_leaving_message(zeroMessage);
        }
		else if (!strcmp(zeroMessage->type, UNREGISTER))
        {//if it is an unregister service message
               		handle_unregister_service_message(zeroMessage);
        }
	}









    int mode; //if 0 then node in network, if 1 then not in network
	int counter; //counter for messages we send before we participate in the network
	int trial_num; //if we have already tried to get in the network and failed, so we now have a name in the format alekos_xx.loc
	bool sent;

   
   














void init( Os::AppMainParameter& value )
{
	radio_ = &wiselib::FacetProvider<Os, Os::Radio>::get_facet( value );
	timer_ = &wiselib::FacetProvider<Os, Os::Timer>::get_facet( value );
	debug_ = &wiselib::FacetProvider<Os, Os::Debug>::get_facet( value );
	radio_->reg_recv_callback<TreeApp,
									   &TreeApp::receive_radio_message>( this );

	mess = (struct message *)malloc(sizeof(struct message));
	/** revision 1 **/
	bzero(mess, sizeof(struct message));

	parent_=-1;
	hops_=INT_MAX;
	isActive_=true;
	connectivity_=false;
	ponged_=false;
	for (int i=0;i<100;i++)
	{
	given[i]=false;
	pending[i]=false;
	}
	
	
/**
 * ZeroConf variables initialization
 **/
 ////////////////////////////////////////////////////
	mode = 1; // if mode = 1 then node is not in the network, if 0 then it is.
    counter = 0; //we are not in the network at the beginning
	trial_num = 0;
	sprintf(my_MACC,"%d",radio_->id());
    recallTime = 2000;
    init_service_arrays();
    sprintf(my_MACC, "%u", radio_->id());
    services_loaded = false;
    haveToAdvertise = false;
    //if (radio_->id()==0 || radio_->id()==1)
    //{
        mode = 1;
        strcpy(my_host, "alekos.loc");
	//}
    //else
    //{
		//sprintf(my_ip, "192.168.1.10%u", radio_->id());
      //  sprintf(my_host, "alekos_%u.loc", radio_->id());
    //}
	//if (radio_->id() == 0)
   // {
		//debug_->debug("\nsetting mode 1 size:%u\n\n", Os::Radio::MAX_MESSAGE_LENGTH);
    //    mode = 0; //if mode = 1 then node is not in the network, if 0 then it is.
        //debug_->debug("services we have: %i, %s(TTL:%f)\n ",number_of_services,services[0].service_type,services[0].TTL);

   // }
////////////////////////////////////////////////

	
	
	
	//if (radio_->id()==2)
	// {
	// timer_->set_timer<TreeApp, &TreeApp::sendToAll>( 16000, this, 0 );
	// }
	if (radio_->id()==0)
	{
	connectivity_=true;
	VersionNumber_=9;
	hops_=0;
	sprintf(RoutN_,"%.2x",0);
	mess->type=JOIN_ME;
	radio_->send( Os::Radio::BROADCAST_ADDRESS, sizeof(struct message), (unsigned char*)mess );
	//timer_->set_timer<TreeApp, &TreeApp::test_mes>( 20000, this, 0 );
	}
	timer_->set_timer<TreeApp, &TreeApp::join_request>( 2000, this, 0 );
	
	
	
	
	
	
	
	run(this);
	
	
	
	
	
	/**
	if(radio_->id()!=0)
	timer_->set_timer<TreeApp, &TreeApp::pingToFather>( 12000, this, 0 );
	if(radio_->id()==4)
	timer_->set_timer<TreeApp, &TreeApp::deactivate>( 15000, this, 0 );
	**/
			 //timer_->set_timer<TreeApp, &TreeApp::messagedebug>( 1000, this, 0 );
}

 /** under review
void send_message (int k, char* mess )
{

}
**/

void ZeroMessageToString(ZeroMessage *zero)
{
	debug_->debug("type: %s",zero->type);
	debug_->debug("source_host: %s",zero->source_host);
}


/**
 * runnable function for zeroconf needs
 **/
	void run(void*)
	{
		if (connectivity_)
		{
	     if (mode == 1)
         {//if you are not in the network 
            if (counter >= 0)
            {
               counter++;
               if (counter < 4)
               {
				   	debug_->debug("send part %u\n",radio_->id());
					send_participate_message();				  
               }
               else if (counter == 4)
               {
				   debug_->debug("im going in with hostname %s\n",my_host);
                  send_hello_message();   //send hello message
                  mode = 0; //change your mode, now you can participate in the network
		 if (radio_->id()==0 || radio_->id()==1)
		  timer_->set_timer<TreeApp, &TreeApp::send_leaving_message>( 50000, this, 0 );
		// if (radio_->id()==0)
		  timer_->set_timer<TreeApp, &TreeApp::unregister_service2>( 50000, this, 0 );
               }
               
            }
            
         }
		 else if (mode == 0)
         {//if you are in the network...
            if ((radio_->id()==0 || radio_->id()==1 ) && !services_loaded)
            {
               load_services();
               services_loaded = true;
            }
            //if (number_of_myServices > 0)
            for (int i=0; i<NUM_STRUCTS; i++)			
            {
               if (!strcmp(myServices[i].hostname, "null"))
               {
                  continue;
               }
               advertise_services();
               break;
            }
            checkTTLforServices();
            //timer_->set_timer<Zeroconf, &Zeroconf::run>( recallTime, this, 0 ); //recall run function after 5000 miliseconds
         }
		}
		timer_->set_timer<TreeApp, &TreeApp::run>( recallTime, this, 0 ); //recall run function after 5000miliseconds
	}





/*Join_request broadcasts a request*/
void join_request( void *)
{
if (connectivity_==false && isActive_)
{
debug_->debug("process %d tries to connect\n", radio_->id());
mess->type=JOIN_REQ;
mess->id=radio_->id();
radio_->send( Os::Radio::BROADCAST_ADDRESS, sizeof(struct message), (unsigned char*)mess );
timer_->set_timer<TreeApp, &TreeApp::join_request>( 2000, this, 0 );
}
}

int RoutingNumGenerator (void)
{
int i;
for (i=0;i<256;i++)
{
if(given[i]==false && pending[i]==false)
{
pending[i]=true;
return(i);

}
}
return (-1);
}

void RemovePending(int which)
{
pending[which]=false;
}

/*Join_response sends a response to a node previously sent a request. It actually gives the other node the freedom to
use the object that calls the method as a parent.*/

void join_response(Os::Radio::node_id_t from, struct message *inbox)
{
	if (connectivity_==true)
	{
	if(inbox->type==JOIN_REQ)
	{mess->type=JOIN_ACK;}
	else if (inbox->type==REJOIN_REQ)
	{
	if(strlen(inbox->from)<strlen(RoutN_) && strncmp(inbox->from,RoutN_,strlen(inbox->from))==0)
	{return;}
	else
	{mess->type=REJOIN_ACK;}
	}

	mess->hops=hops_;
	mess->id=radio_->id();
	mess->version=VersionNumber_;
	mess->dest=inbox->id;
	int temp = RoutingNumGenerator();
	if(temp!=-1)
	{
		mess->given=temp;
		char next[5];
		sprintf(next,"%.2x",temp);
		strcpy(mess->Routing,RoutN_);
		strcat(mess->Routing,next);
		radio_->send( Os::Radio::BROADCAST_ADDRESS, sizeof(struct message),(unsigned char*) mess);
	//	debug_->debug("process %d with %s received connection question from %d and replied %s\n",radio_->id(), RoutN_, from, mess->Routing);
	}
	}

}


/*The object enters the tree by setting the node that send the ACK as parent.*/
void do_join(struct message *inbox)
{

if(inbox->hops+1 <hops_ && connectivity_==false)
{
parent_=inbox->id;
hops_=inbox->hops+1;
strcpy(RoutN_,inbox->Routing);
connectivity_=true;
VersionNumber_=inbox->version;
//debug_->debug("process %d just connected to the network with parent %d hops %d and version number %d and RoutingNumber %s\n", radio_->id(), parent_, hops_, VersionNumber_,RoutN_);
            mess->type=JOINED;
            mess->dest=parent_;
            mess->given=inbox->given;
            radio_->send( Os::Radio::BROADCAST_ADDRESS, sizeof(struct message),(unsigned char*) mess);

}
}

/*Deactivate method deactivates the object, by setting isActive_=false, and sending a LEAVE message.*/
void deactivate(void *)
{
mess->type=LEAVE;
mess->id=radio_->id();
radio_->send( Os::Radio::BROADCAST_ADDRESS, sizeof(struct message), (unsigned char*) mess );
isActive_=false;
}
/*Activate method activates the object, and calls join_request, so that the object joins a tree.*/
void activate(void*)
{
isActive_=true;
parent_=-1;
hops_=INT_MAX;
connectivity_=false;
timer_->set_timer<TreeApp, &TreeApp::join_request>( 2000, this, 0 );
}

void test_mes(void *)
{
//send_message("0000", "kaos", sizeof("kaos"));
}

void send_message(char *dest, char *payload, int len)
{

if(strlen(dest)<strlen(RoutN_))
{//debug_->debug("Going up from %s\n",RoutN_);
strcpy(mess->Routing,dest);
strcpy(mess->from,RoutN_);
mess->type=UP;
}
else if(strncmp(RoutN_,dest,strlen(RoutN_))==0)
{//debug_->debug("Going down from %s\n",RoutN_);
strcpy(mess->Routing,dest);
strcpy(mess->from,RoutN_);
mess->type=DOWN;
}
else
{//debug_->debug("Going up from %s\n",RoutN_);
strcpy(mess->Routing,dest);
strcpy(mess->from,RoutN_);
mess->type=UP;
}

if(payload==NULL)
{
/** revision 1 **/

radio_->send(Os::Radio::BROADCAST_ADDRESS, sizeof(struct message),(unsigned char*) mess);
}
else
{
message = (char *)malloc(sizeof(struct  message)+len);
memcpy(message, mess, sizeof(struct message));
memcpy(message+sizeof(struct message), payload, len);
radio_->send( Os::Radio::BROADCAST_ADDRESS, sizeof(struct message)+len,(unsigned char*) message);
free(message);
}



}

void handle_up(struct message *inbox , char *buf, int len)
{
if(strlen(inbox->from)==strlen(RoutN_)+2)
{
if (strcmp(RoutN_,inbox->Routing)==0)
{
//debug_->debug("Message delievered to %s!\n",RoutN_);
/** revision 1 **/
payload_handler(buf);
}
else if (strncmp (inbox->from,RoutN_,strlen(RoutN_))==0)
{
                    if (len == 0)
                    {
                        send_message(inbox->Routing, NULL, 0);
                    }
                    else
                    {
                        send_message(inbox->Routing, buf, len);
                    }
}
}
}

void handle_down(struct message *inbox, char *buf, int len )
{
if(strlen(inbox->from)==strlen(RoutN_)-2)
{
if (strcmp(RoutN_,inbox->Routing)==0)
{
//debug_->debug("Message delievered! to %s\n",RoutN_);
payload_handler(buf);
}
else if (strncmp(RoutN_,inbox->Routing,strlen(RoutN_))==0)
{//debug_->debug("Down from %s\n",RoutN_);
strcpy(mess->Routing,inbox->Routing);
strcpy(mess->from,RoutN_);
mess->type=DOWN;

                    if(len==0)
                    {
                      
                        radio_->send(Os::Radio::BROADCAST_ADDRESS, sizeof(struct message),(unsigned char*) mess);
                    }
                    else
                    {
                       	message = (char *)malloc(sizeof(struct  message)+len);
			memcpy(message, mess, sizeof(struct message));
			memcpy(message+sizeof(struct message), buf, len);
			radio_->send( Os::Radio::BROADCAST_ADDRESS, sizeof(struct message)+len,(unsigned char*) message);
free(message);
                    }
}
}
}

    void payload_handler(char *payload)
    {
        if(payload!=NULL)
        {
			ZeroMessage zeroMessage;
			memcpy(&zeroMessage,payload,sizeof(zeroMessage));
			handle_ZeroMessage(&zeroMessage);

       // debug_->debug("Message payload: %s\n",payload);
        }
    }


void sendToAll(char *payload, int len)
{
mess->type=ALL;
strcpy(mess->from,RoutN_);
strcpy(mess->Routing,"_");
//debug_->debug("message sent from %s\n",RoutN_);

if(payload==NULL)
{
/** revision 2 **/

radio_->send(Os::Radio::BROADCAST_ADDRESS, sizeof(struct message),(unsigned char*) mess);
}
else
{
message = (char *)malloc(sizeof(struct  message)+len);
memcpy(message, mess, sizeof(struct message));
memcpy(message+sizeof(struct message), payload, len);
radio_->send( Os::Radio::BROADCAST_ADDRESS, sizeof(struct message)+len,(unsigned char*) message);
free(message);
}

}




void forwardToAll(struct message *inbox, char *payload, int len)
{
if((strlen(inbox->from)==strlen(RoutN_)-2 && strncmp(inbox->from,RoutN_,strlen(RoutN_)-2)==0) ||
(strlen(inbox->from)==strlen(RoutN_)+2 && strncmp(inbox->from,RoutN_,strlen(RoutN_))==0))
{
if(strcmp(RoutN_,inbox->Routing)!=0)
{
mess->type=ALL;
strcpy(mess->from,RoutN_);
strcpy(mess->Routing,inbox->from);

if(len==0)
{
/** revision 2 **/

radio_->send(Os::Radio::BROADCAST_ADDRESS, sizeof(struct message),(unsigned char*) mess);
}
else
{
payload_handler(payload);
message = (char *)malloc(sizeof(struct  message)+len);
memcpy(message, mess, sizeof(struct message));
memcpy(message+sizeof(struct message), payload, len);
radio_->send( Os::Radio::BROADCAST_ADDRESS, sizeof(struct message)+len,(unsigned char*) message);
free(message);
}

//debug_->debug("message passed from %s\n",RoutN_);
}
}
}



 
       /* This method is the message handler.*/
void receive_radio_message( Os::Radio::node_id_t from, Os::Radio::size_t len, Os::Radio::block_data_t *buf )
{

if(isActive_)
{
struct message *mess;
char *temp;
mess=(struct message *)malloc(sizeof(struct message));
if(len > sizeof(struct message))
{
temp=(char *)malloc(len-sizeof(struct message));
memcpy(mess, buf, sizeof(struct message));
memcpy(temp, buf+sizeof(struct message), len-sizeof(struct message));
}
else
{
mess = (struct message *) buf;
}

if (mess->type==JOIN_ME)
timer_->set_timer<TreeApp, &TreeApp::join_request>( 0, this, 0 );
else if (mess->type==JOIN_REQ || mess->type==REJOIN_REQ)
join_response(from, mess);
else if(mess->type==JOIN_ACK && mess->dest==radio_->id())
               do_join(mess);
else if(mess->type==REJOIN_ACK && mess->dest==radio_->id())
               do_rejoin(mess);
             else if(mess->type==JOINED && mess->dest==radio_->id())
             {
                 given[mess->given]=true;
                 pending[mess->given]=false;
}
else if(mess->type==UP)
handle_up(mess, temp, len-sizeof(struct message));
else if(mess->type==DOWN)
handle_down(mess, temp, len-sizeof(struct message));
else if(mess->type==ALL)
forwardToAll(mess, temp, len-sizeof(struct message));
else if(mess->type==PING)
pongToSon(mess);
else if(mess->type==PONG)
getPonged(mess);
else if(mess->type==REPAIR)
repair(mess);

            { /*
debug_->debug("parent: %d\n", mess->parent);
debug_->debug("type: %d\n", mess->type);
debug_->debug("hops: %d\n", mess->hops);
debug_->debug("id: %d\n", mess->id);
debug_->debug("version: %d\n", mess->version);
debug_->debug("dest: %d\n", mess->dest);
debug_->debug("given: %d\n", mess->given);
debug_->debug("from: %s\n", mess->from);
debug_->debug("Routing: %s\n", mess->Routing);
debug_->debug("payload: %s\n", buf+sizeof(struct message));
debug_->debug("len: %d\n", len);
debug_->debug("---\n");
*/
                }
}
}
 
 

void pingToFather(void *)
{
if(isActive_ && connectivity_)
{
mess->type=PING;
ponged_=false;
strcpy(mess->from,RoutN_);
strncpy(mess->Routing,RoutN_,(strlen(RoutN_)-2));
mess->Routing[strlen(RoutN_)-2]='\0';
radio_->send( Os::Radio::BROADCAST_ADDRESS, sizeof(struct message),(unsigned char*) mess);
timer_->set_timer<TreeApp, &TreeApp::checkIfPonged>( 2000, this, 0 );
debug_->debug("Sent ping from %s to %s\n",RoutN_,mess->Routing);
}
}
void pongToSon(struct message *inbox)
{
if(strcmp(inbox->Routing,RoutN_)==0)
{
mess->type=PONG;
strcpy(mess->from,RoutN_);
strcpy(mess->Routing,inbox->from);
radio_->send( Os::Radio::BROADCAST_ADDRESS, sizeof(struct message),(unsigned char*) mess);
debug_->debug("Pong back from %s to %s\n",RoutN_, mess->Routing);
}
}
void getPonged(struct message *inbox)
{
if(strcmp(inbox->Routing,RoutN_)==0)
ponged_=true;
}
void checkIfPonged(void *)
{
if(ponged_)
{
ponged_=false;
debug_->debug("Ping-pong ok at %s\n",RoutN_);
if(radio_->id()!=0)
timer_->set_timer<TreeApp, &TreeApp::pingToFather>( 5000, this, 0 );

}
else
{
debug_->debug("Ping-pong error at %s\n",RoutN_);
connectivity_=false;
reJoin_request(NULL);
}
}

void reJoin_request( void *)
{
if (connectivity_==false && isActive_)
{
debug_->debug("process %d tries to reconnect\n", radio_->id());
mess->type=REJOIN_REQ;
mess->id=radio_->id();
strcpy(mess->from,RoutN_);
radio_->send( Os::Radio::BROADCAST_ADDRESS, sizeof(struct message), (unsigned char*)mess );
timer_->set_timer<TreeApp, &TreeApp::reJoin_request>( 2000, this, 0 );
}
}

void do_rejoin(struct message *inbox)
{

if(connectivity_==false)
{
parent_=inbox->id;
hops_=inbox->hops+1;
strcpy(mess->from,RoutN_);
strcpy(mess->Routing,inbox->Routing);
mess->hops=hops_;
strcpy(RoutN_,inbox->Routing);
connectivity_=true;
VersionNumber_=inbox->version;
debug_->debug("process %d just connected to the network with parent %d hops %d and version number %d and RoutingNumber %s\n", radio_->id(), parent_, hops_, VersionNumber_,RoutN_);
mess->type=JOINED;
mess->dest=parent_;
mess->given=inbox->given;
radio_->send( Os::Radio::BROADCAST_ADDRESS, sizeof(struct message),(unsigned char*) mess);
mess->type=REPAIR;
radio_->send( Os::Radio::BROADCAST_ADDRESS, sizeof(struct message),(unsigned char*) mess);
timer_->set_timer<TreeApp, &TreeApp::pingToFather>( 5000, this, 0 );

}
}

void repair(struct message *inbox)
{
if(strlen(inbox->from)==strlen(RoutN_)-2 && strncmp(inbox->from,RoutN_,strlen(inbox->from))==0)
{
char temp[2];
temp[0]=RoutN_[strlen(RoutN_)-2];
temp[1]=RoutN_[strlen(RoutN_)-1];
temp[2]='\0';
strcpy(mess->from,RoutN_);
strcpy(RoutN_,inbox->Routing);
strcat(RoutN_,temp);
strcpy(mess->Routing,RoutN_);
hops_=inbox->hops+1;
mess->hops=hops_;
debug_->debug("process %d repaired with parent %d hops %d and version number %d and RoutingNumber %s\n", radio_->id(), parent_, hops_, VersionNumber_,RoutN_);
mess->type=REPAIR;
radio_->send( Os::Radio::BROADCAST_ADDRESS, sizeof(struct message),(unsigned char*) mess);
timer_->set_timer<TreeApp, &TreeApp::pingToFather>( 5000, this, 0 );
}
}

   private:
        Os::Radio::self_pointer_t radio_;
        Os::Debug::self_pointer_t debug_;
Os::Timer::self_pointer_t timer_;
int parent_;
unsigned int VersionNumber_;
char RoutN_[32];
bool connectivity_, isActive_, given[256], pending[256], ponged_;
int hops_;
//zeroconf implementation
	char my_host[40];
	char my_MACC[20];
	bool services_loaded;
	bool haveToAdvertise;
	int recallTime;
	char dnsTable[25][255];



};
// --------------------------------------------------------------------------
wiselib::WiselibApplication<Os, TreeApp> tree_app;
// --------------------------------------------------------------------------
void application_main( Os::AppMainParameter& value )
{
   tree_app.init( value );
}
