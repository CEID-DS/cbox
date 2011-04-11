/*UDP CLIENT*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
/* defines STDIN_FILENO, system calls,etc */
#include <sys/types.h> /* system data type definitions */
#include <sys/socket.h> /* socket specific definitions */
#include <netinet/in.h> /* INET constants and stuff */
#include <arpa/inet.h> /* IP address conversion stuff */
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

	//socket
if ((sk = socket( PF_INET, SOCK_DGRAM, 0 )) < 0){
	printf("Problem creating socket\n");
	exit(1);
	}

	//connection's conf
server.sin_family=AF_INET;
server.sin_addr.s_addr=inet_addr(HOST);
server.sin_port = htons(PORT);/* establish the server port number - we must use network byte order! */


strcpy(buf, "testing\0");

/* send it to the echo server */
n_sent = sendto(sk,buf,BUFFER_SIZE,0,
(struct sockaddr*) &server,sizeof(server));

if (n_sent<0) {
perror("Problem sending data");
exit(1);
}

return(0);
}

