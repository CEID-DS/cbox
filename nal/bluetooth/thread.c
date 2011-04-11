
// compile needs lpthread lib

#include <pthread.h>
#include <stdio.h>


int main ()
{

enable();

}


void *receiver()
{
printf("Receiver is up and running...\n");

/* receiver supposed to open 
   a UDP socket connection and listen */
connection();

}


// initialize node/machine
int enable(){

	int status;
	long t;
	pthread_t thread;

        status = pthread_create(&thread, NULL, receiver, (void *)t);
        if (status) return -1;

	printf("Main is up and running...\n");

	pthread_join(thread,NULL);
}



/* socket: 
 +parameters
 +create
 +bind
 +listen */
int connection(){


}






