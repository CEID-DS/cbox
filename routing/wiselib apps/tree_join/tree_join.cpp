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

#include "external_interface/external_interface.h"
#include "algorithms/routing/tree/tree_routing.h"
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#define JOIN_REQ  0
#define JOIN_ACK  1
#define REPAIR    2
#define TREE_INIT 3
#define LEAVE     4

/*SCENARIO: Node 0 starts to build an MST by sending a TREE_INIT message. Node 4 is activated at iteration 6.
  Node 1 is deactivated in iteration 10. A new Tree (not always an MST) is now formed. Therefore node 0 calls
  self_repair every 15 iterations to reform the tree back to an MST. For different topologies, different trees
  are formed.*/
typedef wiselib::OSMODEL Os;

class TreeApp
{
   public:
	
      void init( Os::AppMainParameter& value )
      {
         radio_ = &wiselib::FacetProvider<Os, Os::Radio>::get_facet( value );
         timer_ = &wiselib::FacetProvider<Os, Os::Timer>::get_facet( value );
         debug_ = &wiselib::FacetProvider<Os, Os::Debug>::get_facet( value );
         radio_->reg_recv_callback<TreeApp,
                                   &TreeApp::receive_radio_message>( this );

	int *message;
	message = (int*)malloc(4*sizeof(int));
	parent_=-1;
	hops_=INT_MAX;
	isActive_=true;
	connectivity_=false;

	if(radio_->id()==4)
		{
		deactivate((void *) message);
		timer_->set_timer<TreeApp, &TreeApp::activate>( 6000,this,0);
		}
	 if (radio_->id()==0)
		{
		connectivity_=true;
		VersionNumber_=0;
		debug_->debug("hello\n");	
		hops_=0;
		message[0]=TREE_INIT;
		message[1]=hops_;
		message[2]=radio_->id();
		message[3]=VersionNumber_;
		radio_->send( Os::Radio::BROADCAST_ADDRESS, 4*sizeof(int), (unsigned char*) message );
		timer_->set_timer<TreeApp, &TreeApp::self_repair_init>( 15000, this, 0 );
		
		}

	if(radio_->id()==1)
		{
		timer_->set_timer<TreeApp, &TreeApp::deactivate>(10000,this,0);
		}
	}
	
	/*Deactivate method deactivates the object, by setting isActive_=false, and sending a LEAVE message.*/
	void deactivate(void *)
	{
		int *message;
		message = new(int[4]);
		message[0]=LEAVE;
		message[2]=radio_->id();
		radio_->send( Os::Radio::BROADCAST_ADDRESS, 4*sizeof(int), (unsigned char*) message );
		free(message);
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
		
	/*Join_request broadcasts a request*/
	void join_request( void* )
	{
		if (connectivity_==false && isActive_)
		{
		int *message;
		message = (int*)malloc(4*sizeof(int));
		debug_->debug("process %d tries to connect\n", radio_->id());
		message[0]=0;			//message type (0 is for connection question)
		message[2]=radio_->id();
		radio_->send( Os::Radio::BROADCAST_ADDRESS, 4*sizeof(int), (unsigned char*) message );
		free(message);
		timer_->set_timer<TreeApp, &TreeApp::join_request>( 2000, this, 0 );
		
		}
	}
	/*Join_response sends a response to a node previously sent a request. It actually gives the other node the freedom to 
	  use the object that calls the method as a parent.*/
	void join_response(Os::Radio::node_id_t from)
	{
		if (connectivity_==true)
		{
		int *message;
		message = new(int[4]);
		message[0]=JOIN_ACK; 			//message type (1 is for connection answer)
		message[1]=hops_;
		message[2]=radio_->id();
		message[3]=VersionNumber_;
		radio_->send( Os::Radio::BROADCAST_ADDRESS, 4*sizeof(int),(unsigned char*) message);
		debug_->debug("process %d received connection question from %d and replied\n",radio_->id(), from);
		free(message);
		}
	}

	/*The object enters the tree by setting the node that send the ACK as parent.*/
	void do_join(int *mess)
	{
		if((mess[1]+1) <hops_  && connectivity_==false)
		{
			parent_=mess[2];
			hops_=mess[1]+1;
			connectivity_=true;
			VersionNumber_=mess[3];
			debug_->debug("process %d just connected to the network with parent %d hops %d and version number %d\n", radio_->id(), parent_, hops_, VersionNumber_);
		}
	}	
	
	//------------------------------------------------------------------

	void self_repair_init( void*)
	{	
		int *message;
		message = (int*)malloc(4*sizeof(int));
		VersionNumber_++;
		message[0]=2;
		message[1]=hops_;
		message[2]=radio_->id();
		message[3]=VersionNumber_;
		radio_->send( Os::Radio::BROADCAST_ADDRESS, 4*sizeof(int),(unsigned char*) message);
		debug_->debug("initiated self repair mechanism\n");
		timer_->set_timer<TreeApp, &TreeApp::self_repair_init>( 15000, this, 0 );
		free(message);
	}
	
	
	/*This method is called by an object that receives a TREE_INIT message. What it does is that allowes the node that sent the
	  message to be the parent, and forwards a similar one.*/
	void tree_init( int *mess)
	{
		if(mess[1] +1 <hops_)
			{
			int *message;
			connectivity_=true;
			message = new(int[4]);
			hops_=mess[1]+1;
			parent_=mess[2];
			VersionNumber_= mess[3];
			message[0]=TREE_INIT; 	
			message[1]=hops_;	
			message[2]=radio_->id();
			message[3]=VersionNumber_;
			radio_->send( Os::Radio::BROADCAST_ADDRESS, 4*sizeof(int),(unsigned char*) message);
			debug_->debug("process %d connected to  %d with %d hops and version number %d\n", radio_->id(), parent_, hops_, VersionNumber_);
			free(message);
			}
	}

      /* This method is the message handler.*/
      void receive_radio_message( Os::Radio::node_id_t from, Os::Radio::size_t len, Os::Radio::block_data_t *buf )
      {
		if(isActive_)
			{
			int *mess;
			mess = (int*) buf;
			if (mess[0]==JOIN_REQ)
				join_response(from);
				
			else if(mess[0]==JOIN_ACK)
				do_join(mess);	
		
			else if(mess[0]==REPAIR && VersionNumber_ < mess[3])
			{		
				int *message;
				message = new(int[4]);
				hops_=mess[1]+1;
				parent_=mess[2];
				VersionNumber_= mess[3];
				message[0]=2; 	
				message[1]=hops_;		
				message[2]=radio_->id();
				message[3]=VersionNumber_;
				radio_->send( Os::Radio::BROADCAST_ADDRESS, 4*sizeof(int),(unsigned char*) message);
				debug_->debug("process %d repaired with parent %d hops %d and version number %d\n", radio_->id(), parent_, hops_, VersionNumber_);
				free(message);
	
			}
			else if(mess[0]==TREE_INIT && hops_>mess[1])
				tree_init(mess);
			
			else if(mess[0]==LEAVE && mess[2]==parent_ )
				{
				connectivity_=false;
				hops_=INT_MAX;
				parent_=-1;
				timer_->set_timer<TreeApp, &TreeApp::join_request>( 1000, this, 0 );}
			}
      	}
   private:
        Os::Radio::self_pointer_t radio_;
        Os::Debug::self_pointer_t debug_;
	Os::Timer::self_pointer_t timer_;
	int parent_; 
	unsigned int VersionNumber_;
	bool connectivity_, isActive_;
	int hops_;
	
	
     
};
// --------------------------------------------------------------------------
wiselib::WiselibApplication<Os, TreeApp> tree_app;
// --------------------------------------------------------------------------
void application_main( Os::AppMainParameter& value )
{
   tree_app.init( value );
}
