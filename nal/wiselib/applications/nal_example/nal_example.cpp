/*
 * Simple Wiselib Example
 */
#include "external_interface/external_interface_testing.h"

using namespace std;
typedef wiselib::OSMODEL Os;
typedef Ethernet<Os> ether;

class ExampleApplication
{
   public:
      void init( Os::AppMainParameter& value )
      {      	
	cout << e.enable() << endl;
	e.register_receiver<ExampleApplication,&ExampleApplication::getData>(this);

	e.send("nikos",5);
	e.send("kap",3);
	
	sleep (1);
	
	e.unregister_receiver();
	
	e.register_receiver<ExampleApplication,&ExampleApplication::getData>(this);
	e.send("s",2);

	e.disable();

      }
   private:
	ether e;
	
	void getData(char *data,int size){
      
	      	for (int i=0;i<size;i++){
	      		cout << data[i];
	      	}
	      	cout << endl;
      }
};



// --------------------------------------------------------------------------
wiselib::WiselibApplication<Os, ExampleApplication> example_app;
// --------------------------------------------------------------------------
void application_main( Os::AppMainParameter& value )
{
   example_app.init( value );
}
