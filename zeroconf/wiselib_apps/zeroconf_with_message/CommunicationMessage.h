	#ifndef __ALGORITHMS_COMMUNICATION_MSG_H__
	#define __ALGORITHMS_COMMUNICATION_MSG_H__
	
	#include "util/serialization/simple_types.h"
	enum msg_id {
		END_TO_END_MESSAGE = 245,
		NODE_IN_CLUSTER = 246,
		};
 	
	namespace wiselib
	{
	
	   template<typename OsModel_P,
	            typename Radio_P>
	   class CommunicationMessage
	   {
	   public:
	      typedef OsModel_P OsModel;
	      typedef Radio_P Radio;
	      typedef typename Radio::block_data_t block_data_t;
     
	      // --------------------------------------------------------------------
	      inline CommunicationMessage();
	      inline CommunicationMessage( uint8_t id, uint16_t src, uint16_t dest );
	      // --------------------------------------------------------------------
	      inline uint8_t msg_id()
	      { return read<OsModel, block_data_t, uint8_t>( buffer ); };
	      // --------------------------------------------------------------------
      		inline void set_msg_id( uint8_t id )
	      { write<OsModel, block_data_t, uint8_t>( buffer, id ); }
	      // --------------------------------------------------------------------
	      inline uint8_t payload_size()
	      { return read<OsModel, block_data_t, uint8_t>(buffer + PAYLOAD_POS); }
	      // --------------------------------------------------------------------
	      inline uint16_t source()
	      { return read<OsModel, block_data_t, uint16_t>(buffer + SOURCE_POS); }
	      // --------------------------------------------------------------------
      	      inline void set_source( uint16_t src )
	      { write<OsModel, block_data_t, uint16_t>(buffer + SOURCE_POS, src); }
	      // -----------------------------------------------------------------------
	      inline uint16_t dest()
	      { return read<OsModel, block_data_t, uint16_t>(buffer + RECEIVER_POS); }
	      // --------------------------------------------------------------------
	      inline void set_dest( uint16_t dest )
	      { write<OsModel, block_data_t, uint16_t>(buffer + RECEIVER_POS, dest); }
	      // -----------------------------------------------------------------------
	
	      inline void set_payload( uint8_t len, block_data_t* data )
	      {
	         write<OsModel, block_data_t, uint8_t>(buffer + PAYLOAD_POS, len);
	         if (len == 0) return;
	         memcpy( buffer + PAYLOAD_POS + 1, data, len );
	      }
	      // -----------------------------------------------------------------------
	      inline block_data_t* payload( void )
	      { return buffer + PAYLOAD_POS + 1; }
	      // --------------------------------------------------------------------
	      inline size_t buffer_size()
	      { return PAYLOAD_POS + 1 + payload_size(); };

	   private:
	      inline void set_payload_size( uint8_t len )
	      { write<OsModel, block_data_t, uint8_t>(buffer + PAYLOAD_POS, len); }
	
	      enum data_positions
	      {
	         MSG_ID_POS  = 0,
	         SOURCE_POS  = 1,
	         RECEIVER_POS  = 3,
	         PAYLOAD_POS = 5
	      };
	
	      block_data_t buffer[Radio::MAX_MESSAGE_LENGTH];
	   };
	   // -----------------------------------------------------------------------
	   template<typename OsModel_P,
	            typename Radio_P>
	   CommunicationMessage<OsModel_P, Radio_P>::
	   CommunicationMessage()
	   {
      set_msg_id( 0 );
	      set_payload_size( 0 );
	      set_source( Radio::NULL_NODE_ID );
      set_dest( 0 );
	   }
	   // -----------------------------------------------------------------------
	   template<typename OsModel_P,
	            typename Radio_P>
	   CommunicationMessage<OsModel_P, Radio_P>::
	   CommunicationMessage( uint8_t id, uint16_t src, uint16_t dest )
	   {
	      set_msg_id( id );
	      set_payload_size( 0 );
	      set_source( src );
	      set_dest( dest );
	   }
	
	}
	#endif

