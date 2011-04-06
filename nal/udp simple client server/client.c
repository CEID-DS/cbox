#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
 
 
int main(int argc, char *argv[])
{
  int sock;
  struct sockaddr_in sa;
  int bytes_sent;
  char buffer[200];
 
  strcpy(buffer, "hello world!");
 
  sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if (-1 == sock) /* if socket failed to initialize, exit */
    {
      printf("Error Creating Socket");
      exit(EXIT_FAILURE);
    }
 
  memset(&sa, 0, sizeof sa);
  sa.sin_family = AF_INET;
  sa.sin_addr.s_addr = inet_addr("127.0.0.1");
  sa.sin_port = htons(7654);
 
  bytes_sent = sendto(sock, buffer, strlen(buffer), 0,(struct sockaddr*)&sa, sizeof sa);
  if (bytes_sent < 0) {
    printf("Error sending packet: %s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }
  int temp;
  
  recvfrom(sock,buffer,strlen(buffer),0,(struct sockaddr*)&sa, &temp);
  
  printf("Answer: %s\n",buffer);
 
  close(sock); /* close the socket */
  return 0;
}

