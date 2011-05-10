	void send_question_message(char* about,char* who,char* ip) {
		  	char destin[IP_LEN];
			set_node_message_id(my_MACC,++msgID);
			zeroconf_msg queryMessage;
			memset(&queryMessage,0,sizeof(queryMessage));
			queryMessage.set_type(TYPE_LEN * sizeof(char), (Os::Radio::block_data_t *) &("query"));
			queryMessage.set_source(IP_LEN * sizeof(char),(Os::Radio::block_data_t *) &my_ip);
			queryMessage.set_destination(IP_LEN * sizeof(char),(Os::Radio::block_data_t *) &destin);
			queryMessage.set_source_mac(MAC_LEN * sizeof(char),(Os::Radio::block_data_t *) &my_MACC );
			queryMessage.set_msg_id(msgID);

			msg_data question_data;
			strcpy(question_data.questions[0],about);
			strcpy(question_data.answers[0]._app_name,who);
			strcpy(question_data.answers[0]._ip,ip);
			strcpy(question_data.source_host,my_host);
			//strcpy(hello.dest_host,my_host);
			queryMessage.set_payload(sizeof(msg_data),(Os::Radio::block_data_t *) &question_data);
			radio_->send( Os::Radio::BROADCAST_ADDRESS, sizeof(zeroconf_msg), (Os::Radio::block_data_t *)&queryMessage);
	}
		
	  void advertise_services() {
		  	char destin[IP_LEN];
			set_node_message_id(my_MACC,++msgID);
			zeroconf_msg advertise;
			memset(&advertise,0,sizeof(advertise));
			advertise.set_type(TYPE_LEN * sizeof(char), (Os::Radio::block_data_t *) &("advertise"));
			advertise.set_source(IP_LEN * sizeof(char),(Os::Radio::block_data_t *) &my_ip);
			advertise.set_destination(IP_LEN * sizeof(char),(Os::Radio::block_data_t *) &destin);
			advertise.set_source_mac(MAC_LEN * sizeof(char),(Os::Radio::block_data_t *) &my_MACC );
			advertise.set_msg_id(msgID);

			msg_data advertise_data;
			strcpy(advertise_data.source_host,my_host);
			//strcpy(hello.dest_host,my_host);
			int services_to_advertise=0;
			for (int i=0;i<number_of_myServices;i++)
			{
				if (!myServices[i].advertised)
				{
					debug_->debug("advertising app: %s\n",myServices[i].app_name);
					strcpy(advertise_data.answers[services_to_advertise]._app_name,myServices[i].app_name);
					strcpy(advertise_data.answers[services_to_advertise]._protocol,myServices[i].protocol);
					advertise_data.answers[services_to_advertise]._port = myServices[i].port;
					strcpy(advertise_data.answers[services_to_advertise]._data,myServices[i].TXT_DATA);
					advertise_data.answers[services_to_advertise]._TTL = myServices[i].TTL;
					
					services_to_advertise++;
				
					myServices[i].advertised=true;
					//advertise_service(services[i]);
				}
			}
			advertise.set_payload(sizeof(msg_data),(Os::Radio::block_data_t *) &advertise_data);

			if (services_to_advertise>0)
				radio_->send( Os::Radio::BROADCAST_ADDRESS, sizeof(zeroconf_msg), (Os::Radio::block_data_t *)&advertise);
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
      {//send participation message to see if you can come in the network
         //char participate[]="participate";
	// char null_ip[]="0.0.0.0";
	//debug_->debug("to type einai:%s\n",participate);

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
