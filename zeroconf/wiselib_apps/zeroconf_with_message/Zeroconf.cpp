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
typedef wiselib::ZeroConfMessage<Os, Os::Radio> zeroconf_msg;
//typedef zeroconf_msg::TYPE_LEN TYPE_LEN;
//typedef char** ZeroConfMsg[10][3];

class Zeroconf
{

enum lengths {
 	TYPE_LEN = 15,
	IP_LEN = 20,
	MAC_LEN = 20
	};

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
      char _data[30];
      float TTL;
   };
	  
   //struct zeroconfMsg
   struct msg_data
   {
      int hops;
      int destination;
      char dest_host[HOSTNAME_LEN];
      char source_host[HOSTNAME_LEN];
      int source_MAC;
      char questions[2][5];
      ZeroAnswer answers[2];
   };
	
	
   //get the message_id from the input ip
   int get_node_message_id(const char* ip)
   {
      //bool found = false;
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
	    //if(radio_->id()==0) mode=1;
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

         set_node_message_id(my_MACC,++msgID);
         zeroconf_msg hello;
         memset(&hello,0,sizeof(hello));
	 hello.set_type(TYPE_LEN * sizeof(char), (Os::Radio::block_data_t *) &("hello"));
	 hello.set_source(IP_LEN * sizeof(char),(Os::Radio::block_data_t *) &my_ip);
	 hello.set_destination(IP_LEN * sizeof(char),(Os::Radio::block_data_t *) &("ANY"));
         hello.set_source_mac(MAC_LEN * sizeof(char),(Os::Radio::block_data_t *) &my_MACC );
	 hello.set_msg_id(msgID);

	 msg_data hello_data;
	 strcpy(hello_data.source_host,my_host);
	 strcpy(hello_data.dest_host,"ANY");

	 hello.set_payload(sizeof(msg_data),(Os::Radio::block_data_t *) &hello_data);
	 
	 
	 radio_->send( Os::Radio::BROADCAST_ADDRESS, sizeof(zeroconf_msg), (Os::Radio::block_data_t *)&hello);
      }	
		
      void send_participate_message() 
      {
	 set_node_message_id(my_MACC,++msgID);

         zeroconf_msg partic;
         memset(&partic,0,sizeof(partic));

	 partic.set_type(TYPE_LEN * sizeof(char), (Os::Radio::block_data_t *) &("participate"));
	 partic.set_source(IP_LEN * sizeof(char),(Os::Radio::block_data_t *) &("0.0.0.0"));
	 partic.set_destination(IP_LEN * sizeof(char),(Os::Radio::block_data_t *) &my_ip);
         partic.set_source_mac(MAC_LEN * sizeof(char),(Os::Radio::block_data_t *) &my_MACC );
	 partic.set_msg_id(msgID);

	 msg_data partic_data;
	 strcpy(partic_data.source_host,"unregistered");
	 strcpy(partic_data.dest_host,my_host);

	 partic.set_payload(sizeof(msg_data),(Os::Radio::block_data_t *) &partic_data);
	 debug_->debug("to megethos einai %u\n",partic.buffer_size());

	 radio_->send( Os::Radio::BROADCAST_ADDRESS, sizeof(zeroconf_msg), (Os::Radio::block_data_t *)&partic);
      }



      void send_welcome_message(const char* dest_ip) 
      {//response to the hello message.
         //debug_->debug("(%u)sending welcome to %s\n",radio_->id(),dest_ip);

	char destin[IP_LEN];
	strcpy(destin,dest_ip);

	 set_node_message_id(my_MACC,++msgID);
         zeroconf_msg welcome;
         memset(&welcome,0,sizeof(welcome));
	 welcome.set_type(TYPE_LEN * sizeof(char), (Os::Radio::block_data_t *) &("welcome"));
	 welcome.set_source(IP_LEN * sizeof(char),(Os::Radio::block_data_t *) &my_ip);
	 welcome.set_destination(IP_LEN * sizeof(char),(Os::Radio::block_data_t *) &destin);
         welcome.set_source_mac(MAC_LEN * sizeof(char),(Os::Radio::block_data_t *) &my_MACC );
	 welcome.set_msg_id(msgID);

	 msg_data welcome_data;
	 strcpy(welcome_data.source_host,my_host);
	 //strcpy(hello.dest_host,my_host);

	 welcome.set_payload(sizeof(msg_data),(Os::Radio::block_data_t *) &welcome_data);

	 radio_->send( Os::Radio::BROADCAST_ADDRESS, sizeof(zeroconf_msg), (Os::Radio::block_data_t *)&welcome);

      }

		
      // --------------------------------------------------------------------
      void receive_radio_message( Os::Radio::node_id_t from, Os::Radio::size_t len, Os::Radio::block_data_t *buf )
      {
	  //i know that is a zeroconf message
	 zeroconf_msg *msg_p= (zeroconf_msg *) buf;

         if (get_node_message_id((char *)msg_p->source_mac())==-1 || get_node_message_id((char *)msg_p->source_mac())<msg_p->msg_id())
         {
	         set_node_message_id((char *)msg_p->source_mac(),msg_p->msg_id());

            if (!strcmp((char *)msg_p->type(),"hello"))
            {//if it is a hello message then welcome the new device
               
		handle_hello_message(*msg_p);

            }
            else if (!strcmp((char *)msg_p->type(),"welcome"))
            {	//if it is a welcome message
        
		handle_welcome_message(*msg_p);

            } 
            else if (!strcmp((char *)msg_p->type(),"participate"))
            { //if it is a participate message

		handle_participate_message(*msg_p);

	
            }
         }
      }



	void handle_hello_message(zeroconf_msg& message)
	{
	    if (strcmp((char *)message.source(),my_ip))
               {
                  debug_->debug("receive hello(%u)",radio_->id());
                  radio_->send( Os::Radio::BROADCAST_ADDRESS, sizeof(zeroconf_msg), (Os::Radio::block_data_t *)&message);//forward the message
                  send_welcome_message((char *)message.source());//and send welcome message
               }


	}



	void handle_welcome_message(zeroconf_msg& message)
	{
	      if (!strcmp((char *)message.destination(),my_ip))
               {//and you are the destintation, accept it.
                  debug_->debug("receive welcome(%u) from (%s)\n",radio_->id(),(char *)message.source());
               }
               else 
               {//else forward it
                  radio_->send( Os::Radio::BROADCAST_ADDRESS, sizeof(zeroconf_msg), (Os::Radio::block_data_t *)&message);//forward the message
               }
	}




	void handle_participate_message(zeroconf_msg& message)
	{
		msg_data m_data;
		
		memcpy(&m_data, message.payload(), message.payload_size() );


		debug_->debug("receive participate(%u) from (%s)\n",radio_->id(),(char *)message.source());
                  
               if (mode==0)//if node is in the network
	       {//if you receive an other message
                  if (!strcmp((char *)message.destination(),my_ip) || !strcmp(m_data.dest_host,my_host))
                  {//you are the destination
                     //debug_->debug("%s got a message wow!:)(%s)\n",my_ip,message.source_ip);
                     if (!strcmp((char *)message.source(),"0.0.0.0")) //someone is trying to connect with my ip
                     {//if the sender is one whos trying to come in                        
			debug_->debug("found an outsider, i must reply to him\n");
			set_node_message_id(my_MACC,++msgID);
			zeroconf_msg reply;
 			memset(&reply,0,sizeof(reply));
			reply.set_type(TYPE_LEN * sizeof(char), (Os::Radio::block_data_t *) &("participate"));
			reply.set_source(IP_LEN * sizeof(char),(Os::Radio::block_data_t *) &my_ip);
	 		reply.set_destination(IP_LEN * sizeof(char),(Os::Radio::block_data_t *) &("ANY"));
         		reply.set_source_mac(MAC_LEN * sizeof(char),(Os::Radio::block_data_t *) &my_MACC );
	 		reply.set_msg_id(msgID);

			msg_data reply_data;
			strcpy(reply_data.source_host,my_host);
	 		
			reply.set_payload(sizeof(msg_data),(Os::Radio::block_data_t *) &reply_data);
			radio_->send( Os::Radio::BROADCAST_ADDRESS, sizeof(zeroconf_msg), (Os::Radio::block_data_t *)&reply);
                     }
                  } 
                  else 
                  {//else forward the message
                     //strcpy(message.not_ip,my_ip);
                     radio_->send( Os::Radio::BROADCAST_ADDRESS, sizeof(zeroconf_msg), (Os::Radio::block_data_t *)&message);
                  }
               } 
               else if (mode == 1) 
               {//if node is not in the network
				debug_->debug("eimai o 0:m_data.source_host=%s\n",m_data.source_host);	
                  if (!strcmp((char *)message.source(),my_ip)) 
                  {//if you receive message from someone with your ip
                     counter=0;
                     debug_->debug("(%u)WOW someone has my ip.... i must change it?\n",radio_->id());
                     generate_ip();//generate new ip
                  }
						
                  if (!strcmp(m_data.source_host,my_host))
                  {//if you receive message from someone with your hostname
                     debug_->debug("%u === my_host = %s & source_host = %s \n", radio_->id(), my_host, m_data.source_host);
                     counter=0;
                     //generate_hostname();//generate new hostname
					 debug_->debug("(%u)WOW someone has my hostname.... i must change it?\n",radio_->id());
                     resolve_hostname(my_host, trial_num);
                     trial_num = 1;
                  }

                  //someone is claiming the hostname i want at the same time  
                  if (!strcmp(m_data.dest_host,my_host) && strcmp((char *)message.source_mac(),my_MACC))
                  {
                     counter=0;

		     resolve_hostname(my_host, trial_num);
                  }
                  if (!strcmp((char *)message.destination(),my_ip) && strcmp((char *)message.source_mac(),my_MACC))
                  {
                     counter=0;
		     generate_ip();
                  }
                  
               }

	}


      //generate new ip
      void generate_ip()
      {
         srand(time(NULL));
         int rm = rand() % 255;
         bzero(&my_ip,sizeof(my_ip));
         sprintf(my_ip,"192.168.1.%d",rm);
         debug_->debug("my new ip is %s",my_ip);
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
            snprintf(temp_host, y, "%s", my_host);
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

         
