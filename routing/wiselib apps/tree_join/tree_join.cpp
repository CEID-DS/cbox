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
#include <string>
#include <sstream>
#define JOIN_REQ 0
#define JOIN_ACK 1
#define REPAIR 2
#define JOINED 3
#define LEAVE 4
#define UP 6
#define DOWN 7
#define JOIN_ME 8

#define HEADER 9



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

	void init( Os::AppMainParameter& value )
	{
	radio_ = &wiselib::FacetProvider<Os, Os::Radio>::get_facet( value );
	timer_ = &wiselib::FacetProvider<Os, Os::Timer>::get_facet( value );
	debug_ = &wiselib::FacetProvider<Os, Os::Debug>::get_facet( value );
	radio_->reg_recv_callback<TreeApp,
                                   &TreeApp::receive_radio_message>( this );

	mess = (struct message *)malloc(sizeof(struct message));
	parent_=-1;
	hops_=INT_MAX;
	isActive_=true;
	connectivity_=false;
	for (int i=0;i<100;i++)
		{
		given[i]=false;
		pending[i]=false;
		}
	if (radio_->id()==2)
		{
		timer_->set_timer<TreeApp, &TreeApp::test_mes>( 16000, this, 0 );
		debug_->debug("Test forwarding\n");
		}
	if (radio_->id()==0)
		{
		connectivity_=true;
		VersionNumber_=9;
		hops_=0;
		sprintf(RoutN_,"%.2x",0);
		mess->type=JOIN_ME;
		radio_->send( Os::Radio::BROADCAST_ADDRESS, sizeof(struct message), (unsigned char*)mess );
		}
		timer_->set_timer<TreeApp, &TreeApp::join_request>( 2000, this, 0 );
	
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
		mess->type=JOIN_ACK; 			//message type (1 is for connection answer)
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
			debug_->debug("process %d with %s received connection question from %d and replied %s\n",radio_->id(), RoutN_, from, mess->Routing);
			}
		}
		
	}


	/*The object enters the tree by setting the node that send the ACK as parent.*/
	void do_join(struct message *inbox)
	{

		if(inbox->hops+1 <hops_  && connectivity_==false)
		{
			parent_=inbox->id;
			hops_=inbox->hops+1;
			strcpy(RoutN_,inbox->Routing);
			connectivity_=true;
			VersionNumber_=inbox->version;
			debug_->debug("process %d just connected to the network with parent %d hops %d and version number %d and RoutingNumber %s\n", radio_->id(), parent_, hops_, VersionNumber_,RoutN_);
            int *message;
            message = new(int[5]);
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
		send_message("00010000");
	}
	void send_message(char *dest)
	{
		if(strlen(dest)<strlen(RoutN_))
			{debug_->debug("Going up from %s\n",RoutN_);
			strcpy(mess->Routing,dest);
			strcpy(mess->from,RoutN_);
			mess->type=UP;
			}
		else if(strncmp(RoutN_,dest,strlen(RoutN_))==0)
			{debug_->debug("Going down from %s\n",RoutN_);
			strcpy(mess->Routing,dest);
			strcpy(mess->from,RoutN_);
			mess->type=DOWN;
			}			
		else 
			{debug_->debug("Going up from %s\n",RoutN_);
			strcpy(mess->Routing,dest);
			strcpy(mess->from,RoutN_);
			mess->type=UP;
			}
		radio_->send( Os::Radio::BROADCAST_ADDRESS, sizeof(struct message),(unsigned char*) mess);
	}

	void handle_up(struct message *inbox)
	{
		if(strlen(inbox->from)==strlen(RoutN_)+2)
			{
			if (strcmp(RoutN_,inbox->Routing)==0)
				debug_->debug("Message delievered to %s!\n",RoutN_);
			else if (strncmp (inbox->from,RoutN_,strlen(RoutN_))==0)
				send_message(inbox->Routing);
			}
	}

	void handle_down(struct message *inbox)
	{
		if(strlen(inbox->from)==strlen(RoutN_)-2)
			{
			if (strcmp(RoutN_,inbox->Routing)==0)
				debug_->debug("Message delievered! to %s\n",RoutN_);
			else if (strncmp(RoutN_,inbox->Routing,strlen(RoutN_))==0)
				{debug_->debug("Down from %s\n",RoutN_);
				strcpy(mess->Routing,inbox->Routing);
				strcpy(mess->from,RoutN_);
				mess->type=DOWN;
				radio_->send( Os::Radio::BROADCAST_ADDRESS, sizeof(struct message),(unsigned char*) mess);
				}
			}
	}


      /* This method is the message handler.*/
      void receive_radio_message( Os::Radio::node_id_t from, Os::Radio::size_t len, Os::Radio::block_data_t *buf )
      {
		if(isActive_)
			{
			struct message *mess;
			mess = (struct message *) buf;
			if (mess->type==JOIN_ME)
				timer_->set_timer<TreeApp, &TreeApp::join_request>( 0, this, 0 );
			else if (mess->type==JOIN_REQ)
				join_response(from, mess);
			else if(mess->type==JOIN_ACK && mess->dest==radio_->id())
              			 do_join(mess);
            		else if(mess->type==JOINED && mess->dest==radio_->id())
            			{
                		given[mess->given]=true;
                		pending[mess->given]=false;
				}
			else if(mess->type==UP)
				handle_up(mess);
			else if(mess->type==DOWN)
				handle_down(mess);

      	}
      }
   private:
        Os::Radio::self_pointer_t radio_;
        Os::Debug::self_pointer_t debug_;
	Os::Timer::self_pointer_t timer_;
	int parent_;
	unsigned int VersionNumber_;
	char RoutN_[32];
	bool connectivity_, isActive_, given[256], pending[256];
	int hops_;



};
// --------------------------------------------------------------------------
wiselib::WiselibApplication<Os, TreeApp> tree_app;
// --------------------------------------------------------------------------
void application_main( Os::AppMainParameter& value )
{
   tree_app.init( value );
}
