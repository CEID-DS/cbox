#include "external_interface/external_interface.h"
#include "algorithms/routing/tree/tree_routing.h"
#include "ZeroConfMessage.h"
//#include <map>
#include "util/pstl/list_static.h"
#include "util/pstl/map_list.h"
#include "util/pstl/map_static_vector.h"

//#include <string.h>
//#include <stdlib.h>
//#include <time.h>
//#include "ZeroConfMessage.h"
//#include "../example_app/example_message.h"

//for struct dirent, manages directory
#include<dirent.h>


/**
 * please all #define lines add them to definitions.h file, not here!!!!!!
 * */
#include "definitions.h"



typedef wiselib::OSMODEL Os;
typedef wiselib::MapStaticVector<Os,const char*,int,250> map_t;
typedef wiselib::ZeroConfMessage<Os, Os::Radio> zeroconf_msg;
//typedef zeroconf_msg::TYPE_LEN TYPE_LEN;
//typedef char** ZeroConfMsg[10][3];

class Zeroconf
{

   enum lengths 
   {
      TYPE_LEN = 5,
      IP_LEN = 20,
      MAC_LEN = 20,
      QUERY_LEN = 10,
      NUM_ANSWERS = 2
   };

   //map_t node_message_mapping; //we don't use this...
   int mode; //if 0 then node in network, if 1 then not in network
   int counter; //counter for messages we send before we participate in the network
   char ips[250][20]; //important::: we associate an ip with a msgId. if we see a message from an ip
   int msg_Ids[250]; //we keep the id here so we will know that we handled this message and we avoid loops
		     //we keep only the last id we saw so we will not handle a message from the same
		     //ip with the same OR smaller message id	
   int number_of_nodes_known; //the number of the ips we know
   int trial_num; //if we have already tried to get in the network and failed, so we now have a name in the format alekos_xx.loc
   int recallTime;
   bool services_loaded;
   bool haveToAdvertise;

   struct service
   {
      char service_type[15];
      char protocol[5];
      char interface[10];
      char ip[20];
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

   struct ZeroAnswer
   {//
      int _TTL; 
      char _service_type[15];
      char _protocol[5];
      char _hostname[HOSTNAME_LEN];
      char _ip[20];
      char _data[10];
      size_t _port;
      //int _TTL;
      //size_t _k;
      //int temp;
      //float k;
      //char _domain[5];
   };
	 
   //struct zeroconfMsg
   struct msg_data
   {
      char dest_host[HOSTNAME_LEN];  
      char source_host[HOSTNAME_LEN];
      int source_MAC;
      char questions[2][QUERY_LEN];
      ZeroAnswer answers[NUM_ANSWERS];
      int k;
      //int msgID;
      //char type[20];
      //int hops;
      //int destination;
      //char dest_ip[20];
      //char not_ip[20];
      //char source_ip[20];
      //char source_MACC[20];
   };

   struct piggy_query
   {
      char about[QUERY_LEN];
      char _service_type[15];
      char _ip[20];
   };

   void init_service_arrays (void)
   {
      for (int i=0; i<NUM_STRUCTS; i++)
      {
         strcpy(myServices[i].ip, "null");		
         strcpy(services[i].ip, "null");
         //free_service_index.push_front(i);
      }
   }

   void insert_my_service (struct service& serv)
   {
      debug_->debug("in insert my service\n");
      int index=-1;

      for (int i=0; i<NUM_STRUCTS; i++)
      {
         if (!strcmp(myServices[i].ip, "null"))
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


   void insert_service (struct ZeroAnswer& answer, char *host)
   {
      //debug_->debug("(%u)inserting service (%s)(%s)\n",radio_->id(),host,answer._service_type);
      //debug_->debug("(%u)Mesa stin insert_service:I answer einai ip:(%s), service_type(%s)\n",radio_->id(),answer._ip,answer._service_type);
      int index = -1;
      //number_of_services++;

      for (int i=0; i<NUM_STRUCTS; i++)
      {
         if (!strcmp(services[i].service_type, answer._service_type) &&
		  !strcmp(services[i].ip, answer._ip))
         {
            index = i;
            break;
         }
      }

      if (index == -1)
      {
         for (int i=0; i<NUM_STRUCTS; i++)
         {
            if (!strcmp(services[i].ip, "null"))
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
      strcpy(services[index].ip, answer._ip);
      strcpy(services[index].hostname, host);
      strcpy(services[index].TXT_DATA, answer._data);
      services[index].port = answer._port;
      services[index].TTL = answer._TTL;
      services[index].TOTAL_TTL = answer._TTL;
      services[index].advertised = true;
      services[index].questioned = false;
//      debug_->debug("(%u)Mesa stin insert_service:I answer einai ip:(%s), service_type(%s) mpike stin %d\n",radio_->id(),answer._ip,answer._service_type,index);
   }

   void remove_services (char *ip_serv)
   {
      struct service *array_serv = NULL;

      if (!strcmp(ip_serv, my_ip))
      {
         array_serv = myServices;
      }
      else
      {
         array_serv = services;
      }

      for (int i=0; i<NUM_STRUCTS; i++)
      {
         if (!strcmp(array_serv[i].ip, ip_serv))
         {
            strcpy(array_serv[i].ip, "null");
         }    
      }
   }
   
   //remove a single service known for a spesific ip
   void remove_service (char *ip_serv,int index)
   {
//    debug_->debug("(%u)i am removing service %s from %s\n",radio_->id(),services[index].service_type,ip_serv);
      //under construction.
      struct service *array_serv = NULL;
      if (!strcmp(ip_serv, my_ip))
      {
         array_serv = myServices;
      }
      else
      {
         array_serv = services;
      }
      strcpy(array_serv[index].ip, "null");	   
   }
	
   //get the message_id from the input ip
   int get_node_message_id (const char* ip)
   {
      //bool found = false;
      for (int i=0; i<=number_of_nodes_known; i++)
      {
         if (!strcmp(ip, ips[i]))
         {
            return msg_Ids[i];
         }
      }
      return -1;//if not in the list then return -1
   }
	
   //set the message_id for the given ip
   void set_node_message_id (const char* ip, int id)
   {
      bool found = false;
      for (int i=0; i<=number_of_nodes_known; i++)
      {
         if (!strcmp(ip, ips[i]))
         {//update the message_id of the ip if you already have it
            msg_Ids[i] = id;
            found = true;
            break;
         }
      }

      if (!found) 
      {//or create new entry
         number_of_nodes_known++;
         strcpy(ips[number_of_nodes_known], ip);
         msg_Ids[number_of_nodes_known] = id;
      }
   }
	
   void print_node_message_id()
   {
      for (int i=0; i<=number_of_nodes_known; i++)
      {
         //debug_->debug("%s = > %d\n", ips[i], msg_Ids[i]);
      }
   }
	
	
   public:
      void init( Os::AppMainParameter& value )
      {
         radio_ = &wiselib::FacetProvider<Os, Os::Radio>::get_facet( value );
         timer_ = &wiselib::FacetProvider<Os, Os::Timer>::get_facet( value );
         debug_ = &wiselib::FacetProvider<Os, Os::Debug>::get_facet( value );
         radio_->reg_recv_callback<Zeroconf,
                                   &Zeroconf::receive_radio_message>( this );
         mode = 0; // if mode = 1 then node is not in the network, if 0 then it is.
         counter = -1; //we are not in the network at the beginning
         msgID = 0;
         recallTime = 1000;
         number_of_nodes_known = -1;
         //number_of_services = 0;
         //number_of_myServices = 0;
         trial_num = 0;
         init_service_arrays();

         sprintf(my_MACC, "%u", radio_->id());
         //debug_->debug("%s\n", my_MACC);
         services_loaded = false;
         haveToAdvertise = false;
         
         if (radio_->id()==0 || radio_->id()==1)
         {
            mode = 1;
            strcpy(my_host, "alekos.loc");
            strcpy(my_ip, "192.168.1.100");
         }
         else 
         {
            sprintf(my_ip, "192.168.1.10%u", radio_->id());
            sprintf(my_host, "alekos_%u.loc", radio_->id());
         }
		    
         if (radio_->id() == 0)
         {
            //debug_->debug("\nsetting mode 1 size:%u\n\n", Os::Radio::MAX_MESSAGE_LENGTH);
            mode = 1; //if mode = 1 then node is not in the network, if 0 then it is.
            //debug_->debug("services we have: %i, %s(TTL:%f)\n ",number_of_services,services[0].service_type,services[0].TTL);
         }
         
         run(this);
      }
      
      
      //TODO: this should change so we load services from the database.
      void load_services()
      {
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
                        strcpy(temp.ip, my_ip);
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


      //this keeps the program alive.
      void run(void *)
      {
         if (mode == 1)
         {//if you are not in the network 
            if(counter == -1)
            {
               send_ping_message();
               timer_->set_timer<Zeroconf, &Zeroconf::run>( 5000, this, 0 );
            }
            else if (counter >= 0)
            {
               counter++;
               if (counter < 4)
               {
                  //debug_->debug("(%u - %s - %s)sending participation message %i\n", radio_->id(), my_ip, my_host, counter);
                  send_participate_message();
               }
               else if (counter == 4)
               {//if the counter is 4 then you can come in the network
                  //debug_->debug("%u:: i'm going in!!\n", radio_->id());
                  send_hello_message();   //send hello message
                  mode = 0; //change your mode, now you are in the network
                  //timer_->set_timer<Zeroconf, &Zeroconf::send_leaving_message>( 50000, this, 0 );
               }
               timer_->set_timer<Zeroconf, &Zeroconf::run>( 5000, this, 0 ); //recall run function after 5000miliseconds
            }
            //timer_->set_timer<Zeroconf, &Zeroconf::run>( 5000, this, 0 ); //recall run function after 5000miliseconds
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
               if (!strcmp(myServices[i].ip, "null"))
               {
                  continue;
               }
               advertise_services();
               break;
            }
            checkTTLforServices();
            timer_->set_timer<Zeroconf, &Zeroconf::run>( recallTime, this, 0 ); //recall run function after 5000 miliseconds
         }
      }
           
      //TODO : this will have to change.... load services from database...
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
                  if (strcmp(myServices[i].ip, "null"))
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
                           if (!strcmp(myServices[i].service_type, input) && (!strcmp(myServices[i].ip, my_ip)))
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
                           if (!strcmp(myServices[i].ip, "null") && (added == false))
                           {
                              printf("adding in %d\n", i);
                              printf("ip = %s\n added = %s\n", myServices[i].ip, (added)?"true":"false");
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
                              strcpy(myServices[i].ip, my_ip);
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
                  if ((check[l]==false) && (strcmp(myServices[l].ip, "null")))
                  {
                     unregister_service(l);
                  }
               }
            }
            closedir(pDIR);			
         }
         
         /*
         if (radio_->id() == 0 || radio_->id() == 1)
         { 
            for(int i=0; i<NUM_STRUCTS; i++)
            {
               if (strcmp(services[i].ip, "null"))
                  printf("check 1 %u: service %d = %s ip = %s\n", radio_->id(), i, services[i].service_type, services[i].ip); 	
            }
         }
         if (radio_->id() == 0)
         { 
            printf("calling unregister\n");
            unregister_service(1);
            for(int i=0; i<NUM_STRUCTS; i++)
            {
               if (strcmp(myServices[i].ip, "null"))
                  printf("sec %u: service %d = %s ip = %s\n", radio_->id(), i, myServices[i].service_type, myServices[i].ip); 	
            }
         }
         if (radio_->id() == 0 || radio_->id() == 1)
         { 
            for(int i=0; i<NUM_STRUCTS; i++)
            {
               if (strcmp(services[i].ip, "null"))
                  printf("check 2 %u: service %d = %s ip = %s\n", radio_->id(), i, services[i].service_type, services[i].ip); 	
            }
         }
         */
         
      }
    
      //if a service is not available unregister it
      void unregister_service(int i) 
      {
         debug_->debug("(%u) unregister service \n", radio_->id());       
         msg_data unregister_data;
         strcpy(unregister_data.source_host, my_host);
         strcpy(unregister_data.answers[0]._service_type, myServices[i].service_type);
         strcpy(myServices[i].ip, "null");
               
         set_node_message_id(my_MACC, ++msgID);
         zeroconf_msg unregister;
         memset(&unregister, 0, sizeof(unregister));
         unregister.set_type(TYPE_LEN * sizeof(char), (Os::Radio::block_data_t *) &(UNREGISTER));
         unregister.set_source(IP_LEN * sizeof(char), (Os::Radio::block_data_t *) &my_ip);
         unregister.set_destination(IP_LEN * sizeof(char), (Os::Radio::block_data_t *) &("ANY"));
         unregister.set_source_mac(MAC_LEN * sizeof(char), (Os::Radio::block_data_t *) &my_MACC );
         unregister.set_msg_id(msgID);
         unregister.set_payload(sizeof(msg_data), (Os::Radio::block_data_t *) &unregister_data);
         radio_->send( Os::Radio::BROADCAST_ADDRESS, sizeof(zeroconf_msg), (Os::Radio::block_data_t *)&unregister);  
      }
      
      //check TTL for your services and other services you know
      void checkTTLforServices() 
      {
         piggy_query queries[NUM_ANSWERS]; //NUM_ANSWERS is the number of answers a message has
         int num_queries = 0;			
         
         for (int i=0; i<NUM_STRUCTS; i++) //for all other services you know
         {
            if (!strcmp(services[i].ip, "null"))
            {
               continue;
            }
            services[i].TTL = services[i].TTL - recallTime; //update TTL
//          debug_->debug("(%u)TTL for %s -> %d\n",radio_->id(),services[i].service_type,services[i].TTL);
            if (services[i].TOTAL_TTL - services[i].TTL >=3*services[i].TOTAL_TTL/4)//if TTL is equal or less than zero
            {
               if (strcmp(services[i].ip, my_ip) && !services[i].questioned) //if query has not been sent yet
               {	
                  if (num_queries < NUM_ANSWERS)
                  {
                     //debug_->debug("(%u)(%s)service(%s) expired, sending query %f\n", radio_->id(), services[i].ip, services[i].service_type, services[i].TTL);
                     strcpy(queries[num_queries].about, "service"); //fill query parameters with the given
                     strcpy(queries[num_queries]._service_type, services[i].service_type);
                     strcpy(queries[num_queries]._ip, services[i].ip);
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
               remove_service(services[i].ip, i);
            }    
         }

         if (num_queries)
         {
            send_question_message(queries, num_queries); //send the query
         }

         for (int i=0; i<NUM_STRUCTS; i++) //for all your services
         {
            if (!strcmp(myServices[i].ip, "null"))
            {
               continue;
            }
            myServices[i].TTL = myServices[i].TTL - recallTime; //update TTL
           
            if ((float)myServices[i].TOTAL_TTL - (float)myServices[i].TTL >= 3*myServices[i].TOTAL_TTL/4) //if TTL is equal or less than zero
            {
               update_myServices(); //update your services
              // debug_->debug("(%u)meta to upload\n", radio_->id());	
               /*
               for (int j=0; j<NUM_STRUCTS; j++)
               {
	          debug_->debug("(%u) koitaw ta myServices, service_type:(%s) kai myServ_ip(%s)\n", radio_->id(), myServices[j].service_type, myServices[j].ip);
               }
               */
             //  debug_->debug("(%u)(%s)myService expired %f\n", radio_->id(), myServices[i].ip, services[i].TTL);
            }
         }
      }

      void send_ping_message()
      {
//         debug_->debug("(%u) sending ping\n", radio_->id());
         set_node_message_id(my_MACC, ++msgID);
         zeroconf_msg ping;
         memset(&ping, 0, sizeof(ping));
         ping.set_type(TYPE_LEN * sizeof(char), (Os::Radio::block_data_t *) &(PING));
         ping.set_source(IP_LEN * sizeof(char),(Os::Radio::block_data_t *) &("0.0.0.0"));
         ping.set_destination(IP_LEN * sizeof(char),(Os::Radio::block_data_t *) &("ANY"));
         ping.set_source_mac(MAC_LEN * sizeof(char),(Os::Radio::block_data_t *) &my_MACC );
         ping.set_msg_id(msgID);
         radio_->send( Os::Radio::BROADCAST_ADDRESS, sizeof(zeroconf_msg), (Os::Radio::block_data_t *)&ping);
      }

      void send_question_message(piggy_query piggy_questions[], int num_queries) 
      {//send query message
         //char destin[IP_LEN];
         set_node_message_id(my_MACC, ++msgID);
         zeroconf_msg queryMessage;
         memset(&queryMessage, 0, sizeof(queryMessage));
         queryMessage.set_type(TYPE_LEN * sizeof(char), (Os::Radio::block_data_t *) &(QUERY));
         queryMessage.set_source(IP_LEN * sizeof(char), (Os::Radio::block_data_t *) &my_ip);
         queryMessage.set_destination(IP_LEN * sizeof(char), (Os::Radio::block_data_t *) &("ANY"));
         queryMessage.set_source_mac(MAC_LEN * sizeof(char), (Os::Radio::block_data_t *) &my_MACC );
         queryMessage.set_msg_id(msgID);
         msg_data question_data;
         
         for (int i=0; i<NUM_ANSWERS; i++)
         {
            strcpy(question_data.questions[i], "null");
         }

         for (int i=0; i<num_queries; i++)
         {
            //debug_->debug("(%u)(%s) stelnw service(%s) request\n", radio_->id(), piggy_questions[i]._ip, piggy_questions[i]._service_type);	
            strcpy(question_data.questions[i], piggy_questions[i].about); //fill query parameters with the given
            strcpy(question_data.answers[i]._service_type, piggy_questions[i]._service_type);
            strcpy(question_data.answers[i]._ip, piggy_questions[i]._ip);
         }
         strcpy(question_data.source_host, my_host);
         //strcpy(hello.dest_host, my_host);
         queryMessage.set_payload(sizeof(msg_data), (Os::Radio::block_data_t *) &question_data);
         radio_->send( Os::Radio::BROADCAST_ADDRESS, sizeof(zeroconf_msg), (Os::Radio::block_data_t *)&queryMessage);
      }
		
      void advertise_services() 
      {//advertise services (who are not advertised yet or need to advertise because of a query)
         //char destin[IP_LEN];
         set_node_message_id(my_MACC, ++msgID);
         zeroconf_msg advertise;
         memset(&advertise, 0, sizeof(advertise));
         advertise.set_type(TYPE_LEN * sizeof(char), (Os::Radio::block_data_t *) &(ADVERTISE));
         advertise.set_source(IP_LEN * sizeof(char), (Os::Radio::block_data_t *) &my_ip);
         advertise.set_destination(IP_LEN * sizeof(char), (Os::Radio::block_data_t *) &("ANY"));
         advertise.set_source_mac(MAC_LEN * sizeof(char), (Os::Radio::block_data_t *) &my_MACC );
         advertise.set_msg_id(msgID);
         msg_data advertise_data;
         strcpy(advertise_data.source_host, my_host);

         for(int i=0; i<NUM_ANSWERS; i++) //initialization, it is useful for the handling of message
         {
            strcpy(advertise_data.answers[i]._service_type,"null");
         }

         //strcpy(hello.dest_host,my_host);
         int services_to_advertise = 0;

         for (int i=0; i<NUM_STRUCTS;i++) //for all your services
         {
            if (!strcmp(myServices[i].ip, "null"))
            { 
               continue;
            }
				
            if (!myServices[i].advertised) //if it needs to advertise it
            {//fill the service struct
               if (services_to_advertise<NUM_ANSWERS)
               {
//                  debug_->debug("(%s)advertising app: %s\n", my_ip,myServices[i].service_type);
                  strcpy(advertise_data.answers[services_to_advertise]._service_type, myServices[i].service_type);
                  strcpy(advertise_data.answers[services_to_advertise]._protocol, myServices[i].protocol);
                  advertise_data.answers[services_to_advertise]._port = myServices[i].port;
                  strcpy(advertise_data.answers[services_to_advertise]._data, myServices[i].TXT_DATA);
                  advertise_data.answers[services_to_advertise]._TTL = myServices[i].TTL;
                  strcpy(advertise_data.answers[services_to_advertise]._ip, my_ip);
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
			
         advertise.set_payload(sizeof(msg_data), (Os::Radio::block_data_t *) &advertise_data);
         //debug_->debug("payload_size einai:%d kai to sizeof(msg_data)=%d \n", advertise.payload_size(), sizeof(msg_data));
			
         msg_data m_data;
         memcpy(&m_data, advertise.payload(), advertise.payload_size() );
	
         /*
         for (int i=0; i<NUM_ANSWERS && strcmp(m_data.answers[i]._service_type, "null"); i++)
            debug_->debug("(%u)myService2_%d:%s TTL:%d PORT:%u \n", radio_->id(), i, m_data.answers[i]._service_type, m_data.answers[i]._TTL, m_data.answers[i]._port);
         */

         if (services_to_advertise > 0) //if you have services to advertise, advertise them
         {
            radio_->send( Os::Radio::BROADCAST_ADDRESS, sizeof(zeroconf_msg), (Os::Radio::block_data_t *)&advertise);
         }
      }
		
      void send_hello_message() 
      {//send hello message to tell everybody you are in the network
//         debug_->debug("(%u)sending hello to all\n", radio_->id());
         set_node_message_id(my_MACC, ++msgID);
         zeroconf_msg hello;
         memset(&hello, 0, sizeof(hello));
	 hello.set_type(TYPE_LEN * sizeof(char), (Os::Radio::block_data_t *) &(HELLO));
	 hello.set_source(IP_LEN * sizeof(char), (Os::Radio::block_data_t *) &my_ip);
	 hello.set_destination(IP_LEN * sizeof(char), (Os::Radio::block_data_t *) &("ANY"));
	 hello.set_source_mac(MAC_LEN * sizeof(char), (Os::Radio::block_data_t *) &my_MACC );
	 hello.set_msg_id(msgID);
         msg_data hello_data;
	 strcpy(hello_data.source_host, my_host);
	 strcpy(hello_data.dest_host, "ANY");
         hello.set_payload(sizeof(msg_data), (Os::Radio::block_data_t *) &hello_data);
         radio_->send( Os::Radio::BROADCAST_ADDRESS, sizeof(zeroconf_msg), (Os::Radio::block_data_t *)&hello);
      }	
		
      void send_participate_message() 
      {//send participation message to see if you can come in the network
         set_node_message_id(my_MACC, ++msgID);
         zeroconf_msg partic;
         memset(&partic, 0, sizeof(partic));
         partic.set_type(TYPE_LEN * sizeof(char), (Os::Radio::block_data_t *) &(PARTICIPATE));
         partic.set_source(IP_LEN * sizeof(char), (Os::Radio::block_data_t *) &("0.0.0.0"));
         partic.set_destination(IP_LEN * sizeof(char), (Os::Radio::block_data_t *) &my_ip);
         partic.set_source_mac(MAC_LEN * sizeof(char), (Os::Radio::block_data_t *) &my_MACC );
         partic.set_msg_id(msgID);
         msg_data partic_data;
         strcpy(partic_data.source_host, "unregistered");
         strcpy(partic_data.dest_host, my_host);
         partic.set_payload(sizeof(msg_data), (Os::Radio::block_data_t *) &partic_data);
         //debug_->debug("to megethos einai %u\n", partic.buffer_size());
         radio_->send( Os::Radio::BROADCAST_ADDRESS, sizeof(zeroconf_msg), (Os::Radio::block_data_t *)&partic);
      }

      void send_welcome_message(const char* dest_ip) 
      {//response to the hello message.
         //debug_->debug("(%u)sending welcome to %s\n", radio_->id(), dest_ip);
         char destin[IP_LEN];
         strcpy(destin, dest_ip);
         set_node_message_id(my_MACC, ++msgID);
         zeroconf_msg welcome;
         memset(&welcome, 0, sizeof(welcome));
	 welcome.set_type(TYPE_LEN * sizeof(char), (Os::Radio::block_data_t *) &(WELCOME));
	 welcome.set_source(IP_LEN * sizeof(char), (Os::Radio::block_data_t *) &my_ip);
	 welcome.set_destination(IP_LEN * sizeof(char), (Os::Radio::block_data_t *) &destin);
         welcome.set_source_mac(MAC_LEN * sizeof(char), (Os::Radio::block_data_t *) &my_MACC );
	 welcome.set_msg_id(msgID);
         msg_data welcome_data;
	 strcpy(welcome_data.source_host, my_host);
	 //strcpy(hello.dest_host, my_host);
         welcome.set_payload(sizeof(msg_data), (Os::Radio::block_data_t *) &welcome_data);
         radio_->send( Os::Radio::BROADCAST_ADDRESS, sizeof(zeroconf_msg), (Os::Radio::block_data_t *)&welcome);
      }

      void send_leaving_message(void *)
      {
         set_node_message_id(my_MACC, ++msgID);
         zeroconf_msg leaving;
         memset(&leaving, 0, sizeof(leaving));
	 leaving.set_type(TYPE_LEN * sizeof(char), (Os::Radio::block_data_t *) &LEAVING);
	 leaving.set_source(IP_LEN * sizeof(char), (Os::Radio::block_data_t *) &my_ip);
	 leaving.set_destination(IP_LEN * sizeof(char), (Os::Radio::block_data_t *) &("ANY"));
         leaving.set_source_mac(MAC_LEN * sizeof(char), (Os::Radio::block_data_t *) &my_MACC );
	 leaving.set_msg_id(msgID);
         msg_data leaving_data;
	 strcpy(leaving_data.source_host, my_host);
	 //strcpy(hello.dest_host, my_host);
         leaving.set_payload(sizeof(msg_data), (Os::Radio::block_data_t *) &leaving_data);
         radio_->send( Os::Radio::BROADCAST_ADDRESS, sizeof(zeroconf_msg), (Os::Radio::block_data_t *)&leaving);
         init_service_arrays();
         //number_of_myServices = 0;
         //number_of_services = 0;
         //mode = 1;
         //counter = 0;
      }

// --------------------------------------------------------------------
      void receive_radio_message( Os::Radio::node_id_t from, Os::Radio::size_t len, Os::Radio::block_data_t *buf )
      {
         //debug_->debug("222222222222222222222222s\n");
         //i know that is a zeroconf message
         zeroconf_msg *msg_p = (zeroconf_msg *) buf;

         if (get_node_message_id((char *)msg_p->source_mac())==-1 || get_node_message_id((char *)msg_p->source_mac())<msg_p->msg_id())
         {
            set_node_message_id((char *)msg_p->source_mac(), msg_p->msg_id());

            if (!strcmp((char *)msg_p->type(), PING))
            {//if it is a ping message then a device is nearby and searches for network
               handle_ping_message(*msg_p);
            }
            else if (!strcmp((char *)msg_p->type(), HELLO))
            {//if it is a hello message then welcome the new device
               handle_hello_message(*msg_p);
            }
            else if (!strcmp((char *)msg_p->type(), WELCOME))
            {//if it is a welcome message
               handle_welcome_message(*msg_p);
            } 
            else if (!strcmp((char *)msg_p->type(), PARTICIPATE))
            {//if it is a participate message
               handle_participate_message(*msg_p);
            }//advertise
            else if (!strcmp((char *)msg_p->type(), ADVERTISE))
            {//if it is an advertisement message
               //if (radio_->id() == 0)
                  //debug_->debug("0 received message from %s\n", (char *)msg_p->source());
               handle_advertise_message(*msg_p);
            }
            else if (!strcmp((char *)msg_p->type(), QUERY))
            {//if it is a participate message
               handle_query_message(*msg_p);
            }
            else if (!strcmp((char *)msg_p->type(), LEAVING))
            {//if it is a participate message
               handle_leaving_message(*msg_p);
            }
            else if (!strcmp((char *)msg_p->type(),PONG))
            {//if it is a pong message
               handle_pong_message(*msg_p);
            }
            else if (!strcmp((char *)msg_p->type(), UNREGISTER))
            {//if it is an unregister service message
               handle_unregister_service_message(*msg_p);
            }
         }
      }

      void handle_ping_message(zeroconf_msg& message)
      {   
//         debug_->debug("(%u)receive ping from (%s)\n", radio_->id(), (char *)message.source_mac());
			//TODO: THIS IS NOT ABSOLUTELY CLEAR RIGHT NOW...
//         if (mode==1 && counter==-1) //if I search for network too, i found one (another node)
//         {
//            counter = 0;
//         }
		//send pong only if you belong in a network... we will see about this...
         if (mode == 0)
	 {
            set_node_message_id(my_MACC, ++msgID);
            zeroconf_msg pong;
            memset(&pong, 0, sizeof(pong));
            pong.set_type(TYPE_LEN * sizeof(char), (Os::Radio::block_data_t *) &(PONG));
            pong.set_source(IP_LEN * sizeof(char), (Os::Radio::block_data_t *) &("0.0.0.0"));
            pong.set_destination(IP_LEN * sizeof(char), (Os::Radio::block_data_t *) &("ANY"));
            pong.set_source_mac(MAC_LEN * sizeof(char), (Os::Radio::block_data_t *) &my_MACC );
            pong.set_msg_id(msgID);
            radio_->send( Os::Radio::BROADCAST_ADDRESS, sizeof(zeroconf_msg), (Os::Radio::block_data_t *)&pong);
         }
      }

      void handle_pong_message(zeroconf_msg& message) 
      {
         if(mode==1 && counter==-1) //i am waiting for a pong
         {
//			debug_->debug("(%u)receive pong from (%s)\n", radio_->id(),(char *)message.source_mac());
            counter = 0;
         }
      }

      /**
        * Handler for leaving messages.
        * 
        **/
      void handle_leaving_message(zeroconf_msg& message)
      {
//         debug_->debug("(%u)receive leaving from (%s)\n", radio_->id(), (char *)message.source());
         
         /*
         for(int i=0; i<number_of_services; i++)
         {
            debug_->debug("(%u)service_ip (%s) at %d-th position\n", radio_->id(), services[i].ip, i);	
         }
         */
         //debug_->debug("(%u)ta number_of_nodes_known=%d kai number_of_services=%d\n", radio_->id(), number_of_nodes_known, number_of_services);

         //int count = 0;
         bool exist = false;
         for (int i=0; i<=number_of_nodes_known; i++)
         {
            if (!strcmp((char *)message.source_mac(), ips[i]))
            {
               //debug_->debug("(%u)delete ip (%s) from %d-th position\n", radio_->id(), (char *)message.source(), i);
               msg_Ids[i] = 0;
               bzero(ips[i], IP_LEN);
               //number_of_nodes_known--;
               exist = true;
               break;
            }
         }

         if (exist)
         {   
            //debug_->debug("mpainw sto exist\n");
            remove_services((char *)message.source());

            /*
            debug_->debug("mpainw sto exist\n");
            number_of_nodes_known--;
            for(int i=0; i<NUM_STRUCTS; i++)
            {
               if(!strcmp(services[i].ip, "null")) 
                  continue;
			
               //debug_->debug("mpainw sto for me msg_source(%s) kai serv_ip(%s)\n", (char *) message.source(), services[i].ip);
               if (!strcmp((char *)message.source(), services[i].ip))
               {
                  debug_->debug("(%u)delete service (%s) from %d-th position\n", radio_->id(), (char *) message.source(), i);
                  strcpy(services[i].ip, "0.0.0.0");
                  //debug_->debug("(%u)i ip tou service twra einai (%s)\n", radio_->id(), services[i].ip);
                  count++;
               }
            }
            number_of_services -= count; 
            */
	 }
         //radio_->send( Os::Radio::BROADCAST_ADDRESS, sizeof(zeroconf_msg), (Os::Radio::block_data_t *)&message);
      }

      /**
       * Handler for query messages.
       * 
       * */

      void handle_query_message(zeroconf_msg& message)
      {
         //debug_->debug("(%u) mpainw stin handle_query apo ton %s\n", radio_->id(), (char *) message.source_mac());
         msg_data m_data;
         memcpy(&m_data, message.payload(), message.payload_size() );
         int not_author_services = 0; //i will forward the message if eventually is positive
	
         for (int i=0; i<NUM_ANSWERS && strcmp(m_data.questions[i], "null"); i++)
         {
            not_author_services++;
            if (!strcmp(m_data.questions[i], "service")) //this will go to a separate handle especially for question "service"
            {
               for (int j=0; j<NUM_STRUCTS; j++)
               {
                  if (!strcmp(services[i].ip, "null"))
                  { 
                     continue;
                  }
                  if (!strcmp(m_data.answers[i]._service_type, services[j].service_type))
                  {
                     services[j].questioned = true;
                  }
               }
               for (int j=0; j<NUM_STRUCTS; j++)
               {
                  if (!strcmp(myServices[j].ip, "null"))
                  { 
                     continue;
                  }

                  //debug_->debug("(%u) koitaw ta myServices gia ton %s, m_data:(%s)(%s) kai myServ(%s)(%s)\n", radio_->id(), (char *) message.source_mac(), (char *)m_data.answers[i]._service_type, (char *)m_data.answers[i]._ip,myServices[j].service_type, myServices[j].ip);
                  
                  if (!strcmp((char *)m_data.answers[i]._service_type, myServices[j].service_type) && !strcmp((char *)m_data.answers[i]._ip, myServices[j].ip))
                  {
                     myServices[j].advertised = false;
                     //advertise_services();
                     //send_response_message(myServices[i]);
//                     debug_->debug("(%u)(%s) i have the answer(%s)(%s)\n", radio_->id(),my_ip, (char *) message.source_mac(),myServices[j].service_type);
                     not_author_services--;
                  }
               }
            }
         }
         if (not_author_services)
         {
            radio_->send( Os::Radio::BROADCAST_ADDRESS, sizeof(zeroconf_msg), (Os::Radio::block_data_t *)&message);
         }
      }
	
      void handle_advertise_message(zeroconf_msg& message)
      {
         msg_data m_data;
         memcpy(&m_data, message.payload(), message.payload_size() );
         //debug_->debug("(%u)advertisement(%s)(TTL:%d)(PORT:%d)\n", radio_->id(), (char *)message.source(), m_data.answers[0]._TTL, m_data.answers[0]._port);
         //for (int i=0; i<NUM_ANSWERS && strcmp(m_data.answers[i]._service_type, "null"); i++)
            //debug_->debug("(%u)Service_%d:%s TTL:%f \n", radio_->id(), i, m_data.answers[i]._service_type, m_data.answers[i]._TTL);
//		debug_->debug("(%u)(%s)receive advertise message(%s)",radio_->id(),my_ip,(char *)message.source());
	 for(int i=0; i<NUM_ANSWERS && strcmp(m_data.answers[i]._service_type, "null"); i++)
	 {
            insert_service(m_data.answers[i], (char *)message.source());

            /*
            //debug_->debug("(%u)ADVERTISEMENT(%s)(TTL:%d)(PORT:%d)\n", radio_->id(), (char *)message.source(), m_data.answers[i]._TTL, m_data.answers[i]._port);
            int write_to = number_of_services;
            for (int j=0; j<number_of_services; j++)
            {
               if (!strcmp(services[j].service_type, m_data.answers[i]._service_type) &&
                  !strcmp(services[j].ip, m_data.answers[i]._ip)) //maybe we check the port too in case we have two same services in one ip
               {
                  write_to = j;
               }
            }
            strcpy(services[write_to].service_type, m_data.answers[i]._service_type);
            strcpy(services[write_to].protocol, m_data.answers[i]._protocol);
            strcpy(services[write_to].ip, m_data.answers[i]._ip);
            strcpy(services[write_to].hostname, m_data.source_host);
            strcpy(services[write_to].TXT_DATA, m_data.answers[i]._data);
            services[write_to].port = m_data.answers[i]._port;
            services[write_to].TTL = m_data.answers[i]._TTL;
            services[write_to].advertised = true;
            services[write_to].questioned = false;
		
            if (write_to == number_of_services)
               number_of_services++;     
            */
         }
		
         radio_->send( Os::Radio::BROADCAST_ADDRESS, sizeof(zeroconf_msg), (Os::Radio::block_data_t *)&message);
      }

      void handle_hello_message(zeroconf_msg& message)
      {
         if (strcmp((char *)message.source(), my_ip))
         {
//            debug_->debug("(%u)(%s) receive hello (%s)\n", radio_->id(),my_ip,(char *)message.source());
            radio_->send( Os::Radio::BROADCAST_ADDRESS, sizeof(zeroconf_msg), (Os::Radio::block_data_t *)&message); //forward the message
            send_welcome_message((char *)message.source()); //and send welcome message
         }
      }

      void handle_welcome_message(zeroconf_msg& message)
      {
         if (!strcmp((char *)message.destination(), my_ip))
         {//and you are the destintation, accept it.
//            debug_->debug("receive welcome(%u) from (%s)\n", radio_->id(), (char *)message.source());
         }
         else 
         {//else forward it
            radio_->send( Os::Radio::BROADCAST_ADDRESS, sizeof(zeroconf_msg), (Os::Radio::block_data_t *)&message); //forward the message
         }
      }

      void handle_participate_message(zeroconf_msg& message)
      {
		 
         msg_data m_data;
         memcpy(&m_data, message.payload(), message.payload_size() );


         if (mode == 0) //if node is in the network
         {//if you receive an other message
//         debug_->debug("(%s)receive participate from (%s)\n",my_ip,(char *)message.source());
            if (!strcmp((char *)message.destination(), my_ip) || !strcmp(m_data.dest_host, my_host))
            {//you are the destination
               //debug_->debug("%s got a message wow!:)(%s)\n", my_ip,message.source_ip);
               if (!strcmp((char *)message.source(), "0.0.0.0")) //someone is trying to connect with my ip
               {//if the sender is one whos trying to come in
                  //char type_reply[] = "participate";
                  //char any[] = "ANY";                        
                  //debug_->debug("found an outsider, i must reply to him\n");
                  set_node_message_id(my_MACC, ++msgID);
                  zeroconf_msg reply;
                  memset(&reply,0,sizeof(reply));
                  reply.set_type(TYPE_LEN * sizeof(char), (Os::Radio::block_data_t *) &("participate"));
                  reply.set_source(IP_LEN * sizeof(char), (Os::Radio::block_data_t *) &my_ip);
                  reply.set_destination(IP_LEN * sizeof(char), (Os::Radio::block_data_t *) &("ANY"));
                  reply.set_source_mac(MAC_LEN * sizeof(char), (Os::Radio::block_data_t *) &my_MACC );
                  reply.set_msg_id(msgID);
                  msg_data reply_data;
                  strcpy(reply_data.source_host, my_host);
                  reply.set_payload(sizeof(msg_data), (Os::Radio::block_data_t *) &reply_data);
                  radio_->send( Os::Radio::BROADCAST_ADDRESS, sizeof(zeroconf_msg), (Os::Radio::block_data_t *)&reply);
               }
            } 
            else 
            {//else forward the message
               //strcpy(message.not_ip, my_ip);
               radio_->send( Os::Radio::BROADCAST_ADDRESS, sizeof(zeroconf_msg), (Os::Radio::block_data_t *)&message);
            }
         } 
         else if (mode == 1) 
         {
//         debug_->debug("(%s)(%s)receive participate from (%s)(%s)\n",my_ip,my_host,(char *)message.source(),m_data.source_host);

		//if node is not in the network
            //debug_->debug("eimai o 0:m_data.source_host=%s\n", m_data.source_host);	
            if (!strcmp((char *)message.source(), my_ip)) 
            {//if you receive message from someone with your ip
               counter = 0;
               //debug_->debug("(%u)WOW someone has my ip.... i must change it?\n", radio_->id());
               generate_ip(); //generate new ip
            }
						
            if (!strcmp(m_data.source_host, my_host))
            {//if you receive message from someone with your hostname
               //debug_->debug("%u === my_host = %s & source_host = %s \n", radio_->id(), my_host, m_data.source_host);
               counter = 0;
               //generate_hostname(); //generate new hostname
               //debug_->debug("(%u)WOW someone has my hostname.... i must change it?\n", radio_->id());
               resolve_hostname(my_host, trial_num);
               trial_num = 1;
            }

            //someone is claiming the hostname i want at the same time  
            if (!strcmp(m_data.dest_host, my_host) && strcmp((char *)message.source_mac(), my_MACC))
            {
               counter = 0;
               resolve_hostname(my_host, trial_num);
               trial_num = 1;
            }

            if (!strcmp((char *)message.destination(), my_ip) && strcmp((char *)message.source_mac(), my_MACC))
            {
               counter=0;
               generate_ip();
            }
            
            //will forward the message in case that the only node which listens is this one(node_mode=1->node_mode=1->network)
            radio_->send( Os::Radio::BROADCAST_ADDRESS, sizeof(zeroconf_msg), (Os::Radio::block_data_t *)&message);
         }
      }

      void handle_unregister_service_message(zeroconf_msg& message)
      {
         debug_->debug("(%u) in handle unregister message\n",radio_->id());
         msg_data m_data;
         memcpy(&m_data, message.payload(), message.payload_size() );
         
         for (int i=0; i<NUM_STRUCTS; i++)
         {
            if (!strcmp(services[i].service_type, m_data.answers[0]._service_type) && (!strcmp(services[i].ip, ((char *)message.source()))))
            {
               strcpy(services[i].ip, "null");
               debug_->debug("%u service %s from %s deleted\n", radio_->id(), services[i].service_type, (char *)message.source()); 
               break;
            }
         }
      }

      //generate new ip
      void generate_ip()
      {
         srand(time(NULL)*radio_->id()+radio_->id()*5);
         int rm = rand() % 255;
         bzero(&my_ip,sizeof(my_ip));
         sprintf(my_ip,"192.168.1.%d",rm);
//         debug_->debug("(%u)my new ip is %s\n",radio_->id(),my_ip);
      }
		 
      //resolve hostname conflict
      void resolve_hostname(char my_host[HOSTNAME_LEN], int trial)
      {
//         debug_ ->debug("%u: resolving hostname %s\n", radio_->id(), my_host);
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
//         debug_ ->debug("%u: my new hostname is %s\n", radio_->id(), my_host);
      }
      
      private:
         Os::Radio::self_pointer_t radio_;
         Os::Debug::self_pointer_t debug_;
         Os::Timer::self_pointer_t timer_;
	 int parent_; 
	 unsigned int VersionNumber_;
	 bool connectivity_, init_SR_;
	 int hops_;
	 unsigned char* mes;
	 char my_host[40];
	 char my_ip[20];
	 int msgID;    
	 char my_MACC[20];
};

// --------------------------------------------------------------------------
      wiselib::WiselibApplication<Os, Zeroconf> zeroconf;
// --------------------------------------------------------------------------
      void application_main( Os::AppMainParameter& value )
      {
         zeroconf.init( value );
      }   

         
