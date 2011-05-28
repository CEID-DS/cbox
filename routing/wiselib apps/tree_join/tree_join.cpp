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
timer_->set_timer<TreeApp, &TreeApp::test_mes>( 20000, this, 0 );
}
timer_->set_timer<TreeApp, &TreeApp::join_request>( 2000, this, 0 );
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
debug_->debug("process %d with %s received connection question from %d and replied %s\n",radio_->id(), RoutN_, from, mess->Routing);
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
debug_->debug("process %d just connected to the network with parent %d hops %d and version number %d and RoutingNumber %s\n", radio_->id(), parent_, hops_, VersionNumber_,RoutN_);
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
send_message("0000", "kaos", sizeof("kaos"));
}

void send_message(char *dest, char *payload, int len)
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
debug_->debug("Message delievered to %s!\n",RoutN_);
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
debug_->debug("Message delievered! to %s\n",RoutN_);
payload_handler(buf);
}
else if (strncmp(RoutN_,inbox->Routing,strlen(RoutN_))==0)
{debug_->debug("Down from %s\n",RoutN_);
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
        debug_->debug("Message payload: %s\n",payload);
        }
    }


void sendToAll(char *payload, int len)
{
mess->type=ALL;
strcpy(mess->from,RoutN_);
strcpy(mess->Routing,"_");
debug_->debug("message sent from %s\n",RoutN_);

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

debug_->debug("message passed from %s\n",RoutN_);
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



};
// --------------------------------------------------------------------------
wiselib::WiselibApplication<Os, TreeApp> tree_app;
// --------------------------------------------------------------------------
void application_main( Os::AppMainParameter& value )
{
   tree_app.init( value );
}
