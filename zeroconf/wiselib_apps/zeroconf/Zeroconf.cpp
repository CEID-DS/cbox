#include "external_interface/external_interface.h"
#include "algorithms/routing/tree/tree_routing.h"
#include "util/pstl/map_static_vector.h"

#include <string.h>
#include <stdlib.h>
#include <time.h>

#define TYPE_QUESTION 1;
#define TYPE_RESPONSE 2;
#define TYPE_NET 3;
#define OUTSIDER_IP "0.0.0.0";
#define IN_NETWORK 0;
#define WANT_NETWORK 1;
#define HOSTNAME_LEN 20
#define TYPE_PROB_HOSTNAME 4;

typedef wiselib::OSMODEL Os;
typedef wiselib::MapStaticVector<Os,const char*,int,250> map_t;
//typedef char** ZeroConfMsg[10][3];

class Zeroconf
{
   map_t node_message_mapping; //we don't use this...
   int mode;//if 0 then node in network, if 1 then not in network
   int counter;//counter for messages we send before we participate in the network
   char ips[250][20];//important::: we associate an ip with a msgId. if we see a message from an ip
   int msg_Ids[250];//we keep the id here so we will know that we handle this message and we avoid loops
		    //we keep only the last id we saw so we will not handle a message from the same
		    //the same ip with the same OR smaller message id	
   int number_of_nodes_known;//the number of the ips we know
   int trial_num;

   struct ZeroAnswer
   {//
      char _app_name[5];
      char _protocol[5];
      char _domain[5];
      char _hostname[HOSTNAME_LEN];
      char _ip[20];
      char _data[40];
      float TTL;
   };
	  
   struct zeroconfMsg 
   {
      int msgID;
      char type[20];
      int hops;
      int destination;
      char dest_host[HOSTNAME_LEN];
      char dest_ip[20];
      char not_ip[20];
      char source_host[HOSTNAME_LEN];
      char source_ip[20];
      int source_MAC;
      char source_MACC[20];
      char questions[10][5];
      ZeroAnswer answers[10];
   };
	
	
   //get the message_id from the input ip
   int get_node_message_id(const char* ip)
   {
      bool found = false;
      for (int i=0;i<=number_of_nodes_known;i++)
      {
         if (!strcmp(ip,ips[i]))
         {
            return msg_Ids[i];
         }
      }
      return -1;//if not in the list then return -1
   }
	
   //set the message_id for the given ip
   void set_node_message_id(const char* ip, int id)
   {
      bool found = false;
      for (int i=0;i<=number_of_nodes_known;i++)
      {
         if (!strcmp(ip,ips[i]))
         {//update the message_id of the ip if you already have it
            msg_Ids[i] = id;
            found=true;
            break;
         }
      }

      if (!found) 
      {//or create new entry
         number_of_nodes_known++;
         strcpy(ips[number_of_nodes_known],ip);
         msg_Ids[number_of_nodes_known]=id;
      }
   }
	
   void print_node_message_id()
   {
      for (int i=0;i<=number_of_nodes_known;i++)
      {
         debug_->debug("%s = > %d\n",ips[i],msg_Ids[i]);
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
         counter=0;
         msgID=0;
         number_of_nodes_known=-1;
         trial_num = 0;
		 sprintf(my_MACC, "%u",radio_->id());
		 debug_->debug("%s\n",my_MACC);
         if (radio_->id()==0 || radio_->id()==1)
         {
			 mode=1;
            strcpy(my_host,"alekos.loc");
            strcpy(my_ip,"192.168.1.100");
         }
         else 
         {
            sprintf(my_ip,"192.168.1.10%u",radio_->id());
            //sprintf(my_host,"alekos_%u",radio_->id());
            //generate_hostname();
            sprintf(my_host,"alekos_%u.loc",radio_->id());
//            strcpy(my_host,"alekos.loc");
         }
		    
         if (radio_->id()==0)
         {
            debug_->debug("\nsetting mode 1\n\n");
            mode=1; //if mode = 1 then node is not in the network, if 0 then it is.
         }
			
         run(this);

//         debug_->debug("process %d tries to connect\n", radio_->id());
      }

      void run(void *)
      {//this keeps the program alive
      	
		 //debug_->debug("%u --- counter = %d\n", radio_->id(), counter);         
         if (mode==1)
         {//if you are not in the network 
            if (counter>=0)
            {//if the counter is positive
               counter++;
               if (counter==4)
               {//if the counter is 4 then you can come in the network
                  debug_->debug("%u:: i'm going in!!\n", radio_->id());
                  send_hello_message();//send hello message
                  mode=0;//change your mode, now you are in the network
               }
               else if (counter!=-1)
               {//if counter is not -1 send participation message (it will not be ofcourse)
                  debug_->debug("(%u - %s - %s)sending participation message %i\n",radio_->id(),my_ip,my_host,counter);
                  send_participate_message();
               }
            }
            else 
            {//else
               if (counter==-1)//if counter is -1, well.. we fix this in other position doesnot need here.. anyway.
               {
                  debug_->debug("oops must resolve it!!!\n");
               }
               generate_ip();
            }
            timer_->set_timer<Zeroconf, &Zeroconf::run>( 5000, this, 0 );//recall run function after 5000miliseconds
         }
         else if (mode==0)
         {//if you are in the network...
            timer_->set_timer<Zeroconf, &Zeroconf::run>( 1000, this, 0 );//recall run function after 5000 miliseconds
			//here we will add code for searching for services if needed, and advertise new services if any.
         }	
      }
		
      void send_hello_message() 
      {//send hello message to tell everybody you are in the network
         debug_->debug("(%u)sending hello to all\n",radio_->id());

         msgID++;
         //node_message_mapping[my_ip]=msgID;
         set_node_message_id(my_MACC,msgID);
         zeroconfMsg hello;
         memset(&hello,0,sizeof(hello));
         hello.msgID=msgID;
         strcpy(hello.source_ip,my_ip);
         strcpy(hello.source_host,my_host);
         strcpy(hello.type,"hello");
         sprintf(hello.source_MACC,"%s",my_MACC);
         radio_->send( Os::Radio::BROADCAST_ADDRESS, sizeof(hello),(unsigned char*) &hello);
      }	
		
      void send_participate_message() 
      {//send participation message to see if you can come in the network
         zeroconfMsg zcmsg;
         memset(&zcmsg,0,sizeof(zcmsg));
         strcpy(zcmsg.dest_ip,my_ip);
         strcpy(zcmsg.dest_host,my_host);
         strcpy(zcmsg.source_ip,"0.0.0.0");
         strcpy(zcmsg.source_host, "unregistered");
         msgID++;
         zcmsg.msgID=msgID;
         sprintf(zcmsg.source_MACC,"%s",my_MACC);
         //node_message_mapping[my_ip]=msgID;
         set_node_message_id(my_MACC,msgID);
         radio_->send( Os::Radio::BROADCAST_ADDRESS, sizeof(zcmsg),(unsigned char*) &zcmsg);
      }

      //claim a hostname
      void prob_hostname()
      {
         generate_hostname();
         zeroconfMsg zcmsg;
         memset(&zcmsg, 0, sizeof(zcmsg));		
         msgID++;
         zcmsg.msgID = msgID;
         strcpy(zcmsg.type, "prob_host");
         strcpy(zcmsg.dest_host, my_host);
         strcpy(zcmsg.dest_ip, "BROADCAST_ADDRESS");
         strcpy(zcmsg.source_ip, my_ip);
         set_node_message_id(my_MACC,msgID);
         sprintf(zcmsg.source_MACC,"%s",my_MACC);

         debug_->debug("host %u probing hostname\n",radio_->id());
         radio_->send( Os::Radio::BROADCAST_ADDRESS, sizeof(zcmsg),(unsigned char*) &zcmsg);
      }

      void send_welcome_message(const char* dest_ip) 
      {//response to the hello message.
         //debug_->debug("(%u)sending welcome to %s\n",radio_->id(),dest_ip);
         zeroconfMsg welcome;
         memset(&welcome,0,sizeof(welcome));
         strcpy(welcome.source_host,my_host);
         strcpy(welcome.source_ip,my_ip);
         strcpy(welcome.dest_ip,dest_ip);
         strcpy(welcome.type,"welcome");
         //debug_->debug("(%s , %u) = W > (%s)\n",welcome.source_ip,radio_->id(),welcome.dest_ip);
         msgID++;
         welcome.msgID = msgID;
         //node_message_mapping[my_ip]=msgID;
         sprintf(welcome.source_MACC,"%s",my_MACC);

         set_node_message_id(my_MACC,msgID);
         //debug_->debug("(%u)welcome(%i)\n",radio_->id(),node_message_mapping[my_ip]);
         radio_->send( Os::Radio::BROADCAST_ADDRESS, sizeof(welcome),(unsigned char*) &welcome);
      }

		
      // --------------------------------------------------------------------
      void receive_radio_message( Os::Radio::node_id_t from, Os::Radio::size_t len, Os::Radio::block_data_t *buf )
      {
         zeroconfMsg message;
         memcpy(&message,buf,len);
         char from_source_ip[20];
         strcpy(from_source_ip,message.source_ip);
 
         char temp_host[HOSTNAME_LEN];
         strcpy(temp_host, message.source_host);

         if (get_node_message_id(message.source_MACC)==-1 || get_node_message_id(message.source_MACC)<message.msgID)
         {
	         set_node_message_id(message.source_MACC,message.msgID);

			 //if you haven't already handle this message
            if (!strcmp(message.type,"hello"))
            {//if it is a hello message then welcome the new device
               if (strcmp(message.source_ip,my_ip))
               {
                  debug_->debug("receive hello(%u)",radio_->id());
                 // set_node_message_id(message.source_MACC,message.msgID);
                  //node_message_mapping[message.source_ip]=message.msgID;
                  radio_->send( Os::Radio::BROADCAST_ADDRESS, sizeof(message),(unsigned char*) &message);//forward the message for the other nodes
                  send_welcome_message(message.source_ip);//and send welcome message
               }
            }
            else if (!strcmp(message.type,"welcome"))
            {//if it is a welcome message
               if (!strcmp(message.dest_ip,my_ip))
               {//and you are the destintation, accept it.
                  debug_->debug("receive welcome(%u) from (%s)\n",radio_->id(),message.source_ip);
                //  set_node_message_id(message.source_MACC,message.msgID);
               }
               else 
               {//else forward it
                //  set_node_message_id(message.source_MACC,message.msgID);
                  radio_->send( Os::Radio::BROADCAST_ADDRESS, sizeof(message),(unsigned char*) &message);
               }
            } 
            else 
            {
//debug_->debug("my_host = %s & source_host = %s \n", my_host, message.source_host);
               if (mode==0)//if node is in the network
	       {//if you receive an other message
                  if (!strcmp(message.dest_ip ,my_ip) || !strcmp(message.dest_host,my_host))
                  {//you are the destination
                     //debug_->debug("%s got a message wow!:)(%s)\n",my_ip,message.source_ip);
                     if (!strcmp(message.source_ip,"0.0.0.0")) //someone is trying to connect with my ip
                     {//if the sender is one whos trying to come in
                        debug_->debug("found an outsider, i must reply to him\n");
                        zeroconfMsg reply;
                        strcpy(reply.source_host,my_host);
                        strcpy(reply.source_ip,my_ip);
                        strcpy(reply.not_ip,my_ip);
                        strcpy(reply.type, "reply");
                        reply.source_MAC = radio_->id();
						sprintf(reply.source_MACC,"%s",my_MACC);
						msgID++;
						set_node_message_id(my_MACC,msgID);
                        radio_->send( from, sizeof(reply),(unsigned char*) &reply);
                     }
                  } 
                  else 
                  {//else forward the message
                     //strcpy(message.not_ip,my_ip);
                     radio_->send( Os::Radio::BROADCAST_ADDRESS, len,(unsigned char*) &message);
                  }
               } 
               else if (mode == 1) 
               {//if node is not in the network
				debug_->debug(".\n");
                  //debug_->debug("%s got a message wow!:)(%s)\n",my_ip,message.source_ip);
		  //debug_->debug("%u: my_host = %s & source_host = %s \n", radio_->id(), my_host, message.source_host);	
                  if (!strcmp(my_ip,message.source_ip)) 
                  {//if you receive message from someone with your ip
                     counter=0;
                     debug_->debug("(%u)WOW someone has my ip.... i must change it?\n",radio_->id());
                     generate_ip();//generate new ip
                  }
						
                  if (!strcmp(my_host,message.source_host))
                  {//if you receive message from someone with your hostname
                     debug_->debug("%u === my_host = %s & source_host = %s \n", radio_->id(), my_host, message.source_host);
                     counter=0;
                     //generate_hostname();//generate new hostname
					 debug_->debug("(%u)WOW someone has my hostname.... i must change it?\n",radio_->id());
                     resolve_hostname(my_host, trial_num);
                     trial_num = 1;
                  }

                  //someone is claiming the hostname i want at the same time  
                  if (!strcmp(my_host,message.dest_host) && strcmp(message.source_MACC,my_MACC))
                  {
                     counter=0;

					resolve_hostname(my_host, trial_num);
                  }
                  if (!strcmp(my_ip,message.dest_ip) && strcmp(message.source_MACC,my_MACC))
                  {
                     counter=0;

					generate_ip();
                  }
                  
               }
            }
         }
      }

      //generate new ip
      void generate_ip()
      {
         srand(time(NULL));
         int rm = rand() % 255;
         bzero(&my_ip,sizeof(my_ip));
         sprintf(my_ip,"192.168.1.%i",rm+radio_->id());
         debug_->debug("my new ip is %s",my_ip);
      }
		 
      //generate new random hostname
      void generate_hostname()
      {
         char my_host[HOSTNAME_LEN];
         srand(time(NULL));
         int i;
         for (i=0; i<10; i++)
         {
            int t = rand() % 2;
            int x;
            if (t)
            {
               x = rand() % 26 + 65; //kefalaia
            }
            else
            {
               x = rand() % 25 + 97; //mikra
            }
            //debug_->debug("char num = %d\n",x);
            my_host[i] = (char)x;
            //debug_->debug("char = %c\n",(char)x);
         }
         my_host[i] = '\0';
         strcat(my_host, ".loc");
         debug_ ->debug("%u: my hostname is %s\n", radio_->id(), my_host);
         /*int number;
         try 
         {
            char *p;
            int position;
            p = strchr(my_host, 'l');
            while (p != NULL) 
            {
               position = p - my_host;
               break;
            }
            char *v = &my_host[position];
            number = atoi(v);
         } 
         catch (char * str) 
         {
             number=0;
         }
         sprintf(my_host,"%s_%i",my_host,number+1);
         debug_->debug("my new name is %s\n",my_host);*/
      }

      //resolve hostname conflict
      void resolve_hostname(char my_host[HOSTNAME_LEN], int trial)
      {
         debug_ ->debug("%u: resolving hostname %s\n", radio_->id(), my_host);
         char temp_host[HOSTNAME_LEN];
         char *n;
         char tr[3];
         int l = strlen(my_host);
         strcpy(temp_host, my_host);
         srand(time(NULL));    
         snprintf(tr, 3, "%d", (rand()+radio_->id()) % 100);
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
//debug_ ->debug("%u: my y %d\n", radio_->id(), y);
            snprintf(temp_host, y, "%s", my_host);
//debug_ ->debug("%u: my temp hostname %s\n", radio_->id(), temp_host);
            bzero(my_host, HOSTNAME_LEN);
            snprintf(my_host, HOSTNAME_LEN, "%s_%s.loc", temp_host, tr);
         }
         else
         {
            
         }
         debug_ ->debug("%u: my new hostname is %s\n", radio_->id(), my_host);
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
