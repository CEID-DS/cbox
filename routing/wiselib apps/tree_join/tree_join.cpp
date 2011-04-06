#include "external_interface/external_interface.h"
#include "algorithms/routing/tree/tree_routing.h"
#include <string.h>
#include <stdlib.h>

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
	hops_=9999999;
	connectivity_=0;
	if (radio_->id()==0)
		{
		connectivity_=1;
		VersionNumber_=rand();
		debug_->debug("hello\n");
		hops_=0;
		timer_->set_timer<TreeApp, &TreeApp::self_repair>( 15000, this, 0 );
		
		}
	

		if (connectivity_==0){
		timer_->set_timer<TreeApp, &TreeApp::connect>( 2000, this, 0 );
	
		}
	 }
	
	//-------------------------------------------------------------------
	void connect( void* )
	{
		if (connectivity_==0)
		{
		int *message;
		
		message = (int*)malloc(4*sizeof(int));
		debug_->debug("process %d tries to connect\n", radio_->id());
		message[0]=0;			//message type (0 is for connection question)
		message[1]=radio_->id();
		radio_->send( Os::Radio::BROADCAST_ADDRESS, 4*sizeof(int), (unsigned char*) message );
		timer_->set_timer<TreeApp, &TreeApp::connect>( 2000, this, 0 );
		}
	}
	//-------------------------------------------------------------------
	void self_repair( void* )
	{	int *message;
		message = (int*)malloc(4*sizeof(int));
		VersionNumber_++;
		message[0]=2;
		message[1]=radio_->id();
		message[2]=hops_;
		message[3]=VersionNumber_;
		radio_->send( Os::Radio::BROADCAST_ADDRESS, 4*sizeof(int),(unsigned char*) message);
		debug_->debug("initiated self repair mechanism\n");
	}
	
	
      // --------------------------------------------------------------------
      void receive_radio_message( Os::Radio::node_id_t from, Os::Radio::size_t len, Os::Radio::block_data_t *buf )
      {
	int *mess;
	mess = (int*) buf;
	if (mess[0]==0){
	 if (connectivity_==1)
		{
		int *message;
		message = new(int[4]);
		message[1]=hops_;
		message[0]=1; 			//message type (1 is for connection answer)
		message[2]=radio_->id();
		message[3]=VersionNumber_;
		radio_->send( Os::Radio::BROADCAST_ADDRESS, 4*sizeof(int),(unsigned char*) message);
		debug_->debug("process %d received connection question from %d and replied\n",radio_->id(), mess[1]);
		}
	}
	else if(mess[0]==1){
		if(mess[1]<hops_ && connectivity_==0){
		parent_=mess[2];
		hops_=mess[1]+1;
		connectivity_=1;
		VersionNumber_=mess[3];
		debug_->debug("process %d just connected to the network with parent %d hops %d and version number %d\n", radio_->id(), parent_, hops_, VersionNumber_);
		}	
	}
	else if(mess[0]==2 && VersionNumber_ < mess[3])
	{		
		int *message;
		message = new(int[4]);
		hops_=mess[1]+1;
		parent_=mess[2];
		VersionNumber_= mess[3];
		message[1]=hops_;
		message[0]=2; 			
		message[2]=radio_->id();
		message[3]=VersionNumber_;
		radio_->send( Os::Radio::BROADCAST_ADDRESS, 4*sizeof(int),(unsigned char*) message);
		debug_->debug("process %d repaired with parent %d hops %d and version number %d\n", radio_->id(), parent_, hops_, VersionNumber_);
	}
	
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
     
};
// --------------------------------------------------------------------------
wiselib::WiselibApplication<Os, TreeApp> tree_app;
// --------------------------------------------------------------------------
void application_main( Os::AppMainParameter& value )
{
   tree_app.init( value );
}
