#ifndef NAL_ETHERNET_H
#define NAL_ETHERNET_H

int enable();
int disable();
int send(char *address,char *data,int size);
int receive(char *address,char *data,int size);
#endif

