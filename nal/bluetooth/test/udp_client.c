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

