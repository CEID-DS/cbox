//basic source http://people.csail.mit.edu/albert/bluez-intro/c404.html#simplescan.c
//sudo apt-get install gnome-common libbluetooth-dev

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>

#define len 8 //total scan time: len*1.28 sec
#define max_rsp 255 //max devices returned


int main(int argc, char **argv)
{
    inquiry_info *ii = NULL;  //structure to save the devices' info
    int num_rsp; //number of detected devices   
    int dev_id, sock, flags;
    int i;
    char addr[19] = { 0 };  //address of the device
    char name[248] = { 0 }; //name of the device

    dev_id = hci_get_route(NULL); //finds the first available bluetooth adapter
    sock = hci_open_dev( dev_id ); //opens a socket to the specific adapter
    if (dev_id < 0 || sock < 0) {
        		perror("opening socket");
        		exit(1);
    			}

    flags = IREQ_CACHE_FLUSH; //flush output devices (in case of previous use)

    ii = (inquiry_info*)malloc(max_rsp * sizeof(inquiry_info));  //allocate the max size devices' structure
    
    num_rsp = hci_inquiry(dev_id, len, max_rsp, NULL, &ii, flags); //scan for nearby devices 

    if( num_rsp < 0 ) perror("hci_inquiry");
    else if (num_rsp==0) printf("no devices found\n");

    // for all available devices
    for (i = 0; i < num_rsp; i++) {

        ba2str(&(ii+i)->bdaddr, addr); //convert to hex address
        memset(name, 0, sizeof(name));
        if (hci_read_remote_name(sock, &(ii+i)->bdaddr, sizeof(name), 
            name, 0) < 0){  //get the name of the device
        		  strcpy(name, "[unknown]");
			  }

        printf("%s  %s\n", addr, name);
    }

    free( ii );
    close( sock );
    return 0;
}

