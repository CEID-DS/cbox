/*UDP SERVER*/
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <arpa/inet.h> 
#include <strings.h>

#define PORT 5000
#define BUFFER_SIZE 1024


int main()
{
   int sock, length, n;
   struct sockaddr_in server;
   struct sockaddr_in from;
   char buf[BUFFER_SIZE];

	
   sock=socket(AF_INET, SOCK_DGRAM, 0);
   if (sock < 0) error("Opening socket");

	
   length = sizeof(server);
   bzero(&server,length);


   server.sin_family=AF_INET;
   server.sin_addr.s_addr=INADDR_ANY;
   server.sin_port=htons(PORT);


   if (bind(sock,(struct sockaddr *)&server,length)<0) perror("binding");


   while (1) {
	      printf("Ready to listen...\n");
              n = recvfrom(sock,&buf,BUFFER_SIZE,0,(struct sockaddr *)&from,&length);
              if (n < 0) error("recvfrom");
	      printf("buf: %s\n",buf);
   	     }
 } 
