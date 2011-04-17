
#include <iostream> 
using namespace std;
 


#include "util/serialization/simple_types.h"

#include "util/pstl/list_static.h"
#include "util/pstl/map_list.h"
#include "util/pstl/map_static_vector.h"
//#include <string.h>


//#define TYPE_LEN 20;
//#define IP_LEN 20;
//#define MAC_LEN 20;




namespace wiselib
{

template<typename OsModel_P,
            typename Radio_P>
   class ZeroConfMessage
   {
   public:
      typedef OsModel_P OsModel;
      typedef Radio_P Radio;
      typedef uint16_t seq_nr_t;
      typedef typename Radio::node_id_t node_id_t;
      typedef typename Radio::block_data_t block_data_t;
      typedef typename Radio::size_t size_t;
      typedef typename Radio::message_id_t message_id_t;


      // --------------------------------------------------------------------
      inline ZeroConfMessage();
      // --------------------------------------------------------------------
      inline block_data_t* type()
      { return buffer; }
      // --------------------------------------------------------------------
      inline void set_type( size_t len, block_data_t *type )
      { //write<OsModel, block_data_t, message_id_t>( buffer, id );
	memcpy( buffer , type, len  );		 }
      // --------------------------------------------------------------------
      inline block_data_t* source()
      { return buffer + SOURCE_IP_POS; }
      // --------------------------------------------------------------------
      inline void set_source( size_t len, block_data_t *source_ip )
      {
	 memcpy(buffer + SOURCE_IP_POS, source_ip, len);}
      // --------------------------------------------------------------------
      inline block_data_t* destination()
      { return buffer + DEST_IP_POS; }
      // --------------------------------------------------------------------
      inline void set_destination( size_t len, block_data_t *dest_ip )
      { 
	 memcpy(buffer + DEST_IP_POS, dest_ip, len);}
      // --------------------------------------------------------------------
      inline block_data_t* source_mac()
      { return buffer + SOURCE_MAC_POS; }
      // --------------------------------------------------------------------
      inline void set_source_mac( size_t len, block_data_t *source_mac )
      {
	 memcpy(buffer + SOURCE_MAC_POS, source_mac, len);}
      // --------------------------------------------------------------------
      inline size_t msg_id()
      { return read<OsModel, block_data_t, size_t>(buffer + MSG_ID_POS); }
      // --------------------------------------------------------------------
      inline void set_msg_id( size_t msg_id )
      { write<OsModel, block_data_t, size_t>(buffer + MSG_ID_POS, msg_id); }
      // --------------------------------------------------------------------
      inline void set_payload( size_t len, block_data_t *buf )
      {
         write<OsModel, block_data_t, size_t>(buffer + PAYLOAD_POS, len);
         memcpy( buffer + PAYLOAD_POS + sizeof(size_t), buf, len);
      }
      inline size_t payload_size()
      { return read<OsModel, block_data_t, size_t>(buffer + PAYLOAD_POS); }
      // --------------------------------------------------------------------
      inline block_data_t* payload()
      { return buffer + PAYLOAD_POS + sizeof(size_t); }
      // --------------------------------------------------------------------
      inline size_t buffer_size()
      { return PAYLOAD_POS + sizeof(size_t) + payload_size(); }


	enum data_positions
      {
	 TYPE_LEN = 15,
	 IP_LEN = 20,
	 MAC_LEN = 20,
         SOURCE_IP_POS = TYPE_LEN*sizeof(char),
         DEST_IP_POS = SOURCE_IP_POS + IP_LEN*sizeof(char),
	 SOURCE_MAC_POS = DEST_IP_POS + IP_LEN*sizeof(char),
	 MSG_ID_POS = SOURCE_MAC_POS + MAC_LEN*sizeof(char),
	 PAYLOAD_POS = MSG_ID_POS + sizeof(size_t)
      };	
      

   private:    
      block_data_t buffer[Radio::MAX_MESSAGE_LENGTH];
   };
   // -----------------------------------------------------------------------
   template<typename OsModel_P,
            typename Radio_P>
   ZeroConfMessage<OsModel_P, Radio_P>::
   ZeroConfMessage()
   {

	bzero(buffer, Radio::MAX_MESSAGE_LENGTH);
	
      //set_msg_id( 0 );
      //set_node_id( 0 );
      //size_t len = 0;
      //write<OsModel, block_data_t, size_t>(buffer + PAYLOAD_POS, len);
   }

}
