#include <iostream>
#include <string>
#include "ZcDbBindings.h"
using namespace std;

int main(void)
{

	ZcDbBindings *db = new ZcDbBindings;
	//service struct that will hold the new entries we want to enter to service table
	struct ZcDbBindings::service s;
	//service struct that may even be an array that's why we declare it as a pointer
	struct ZcDbBindings::service *s1;
	//initializing the size of the array
	int size=0;

	//let's see the services and myServices tables
	//retrieving service struct pointer and size
	s1=db->getServices(size);
	//printing the whole struct array
	cout << "Services Table:" << endl;
	db->printArrayOfServices(s1,size);
	//retrieving service struct pointer and size
	s1=db->getMyServices(size);
	//printing the whole struct array
	cout << "myServices Table:" << endl;
	db->printArrayOfServices(s1,size);

	//adding a dns record	
	db->addDnsRecord("ceid13@cbox","WiFi","192.192.13.2");
	//updating a dns record
	db->updateDnsRecord("user1@cbox", "bluetooth","192.168.2.100");
	//removing a dns record
	db->removeDnsRecord("user1@cbox", "ethernet");
	//getting the number of services
	cout << "Number of services in our disposition: " << db->getServicesNum() << endl;
	//declaring a pointer array to hold the requested services
	char **array;
	//retrieving the array
	array=db->getRequestedService(size);
	//printing the array
	cout << "Requested Services: " << endl;
	for(int i=0; i<size; i++)
	{
		cout << array[i] << endl;
	}
	cout << endl;

	//remove a service
	db->removeService("user1@cbox", "printing");

	//let's add a new service in user1(to add a service, the user/node has to appear also in DNSTable
	//otherwise due to foreign key sql will return an error
	s.hostname="user1@cbox";
	s.serviceType="liveStream";
	s.protocol="UDP";
	s.interface="wifi";
	s.port=7650;
	s.TXTDATA="streaming a live video";
	s.TTL=13;
	s.advertised=false;
	s.questioned=true;
	//adding the service
	db->addService(s);

	//let's see the services table again
	//retrieving service struct pointer and size
	s1=db->getServices(size);
	//printing the whole struct array
	cout << "Services Table:" << endl;
	db->printArrayOfServices(s1,size);

	//int size;



	return 0;
}


