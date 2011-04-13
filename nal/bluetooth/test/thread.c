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






