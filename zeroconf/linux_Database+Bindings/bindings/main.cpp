#include <iostream>
#include <string>
#include "ZcDbBindings.h"
using namespace std;

int main(int argc, char *argv[])
{

	ZcDbBindings *db = new ZcDbBindings;

	//adding a dns record	
	db->addDnsRecord("koukou","lalalala","192.192.192.192");
	//updating a dns record
	db->updateDnsRecord("user1@cbox", "interface1","192.168.0.1");
	//removing a dns record
	db->removeDnsRecord("user1@cbox", "interface2");
	//getting the number of services
	cout << "Number of services in our disposition: " << db->getServicesNum() << endl;
	//declaring a pointer array to hold the requested services
	char **array;
	//initializing the size of the array
	int size=0;
	//retrieving the array
	array=db->getRequestedService(size);
	//printing the array
	for(int i=0; i<size; i++)
	{
		cout << array[i] << endl;
	}

	db->removeService("user4@cbox", "service4");


	return 0;
}


