/*UDP CLIENT*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h> 
#include <arpa/inet.h> 
#include <netdb.h>

#define HOST "127.0.0.1"
#define PORT 5000
#define BUFFER_SIZE 1024


int main( int argc, char **argv ) {
int sk;
struct sockaddr_in server;
char buf[BUFFER_SIZE];
int n_sent;
int n_read;

	
if ((sk = socket( PF_INET, SOCK_DGRAM, 0 )) < 0){
	printf("Problem creating socket\n");
	exit(1);
	}

	
server.sin_family=AF_INET;
server.sin_addr.s_addr=inet_addr(HOST);
server.sin_port = htons(PORT);


strcpy(buf, "testing\0");


n_sent = sendto(sk,buf,BUFFER_SIZE,0,
(struct sockaddr*) &server,sizeof(server));

if (n_sent<0) {
perror("Problem sending data");
exit(1);
}

return(0);
}

