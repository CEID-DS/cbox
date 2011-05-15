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

	//let's add a new service in user1(to add a service, the user/node has to appear also in DNSTable
	//otherwise due to foreign key sql will return an error)
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
	cout << "Services Table(added service liveStream in user1@cbox):" << endl;
	db->printArrayOfServices(s1,size);


	//let's change the service in user1 with different port and protocol
	s.hostname="user1@cbox";
	s.serviceType="liveStream";
	s.protocol="TCP";
	s.interface="wifi";
	s.port=5555;
	s.TXTDATA="streaming a live video";
	s.TTL=13;
	s.advertised=false;
	s.questioned=true;
	//adding the service
	db->addService(s);

	//let's update also all the TTLs
	db->refreshTTL(10);


	//let's see the services table again
	//retrieving service struct pointer and size
	s1=db->getServices(size);
	//printing the whole struct array
	cout << "Services Table(updated all TTLs + edited port and protocol in service livestream):" << endl;
	db->printArrayOfServices(s1,size);


	//remove a service
	db->removeService("user1@cbox", "printing");
	//let's see the services table again
	//retrieving service struct pointer and size
	s1=db->getServices(size);
	//printing the whole struct array
	cout << "Services Table(removed user1@cbox printing service):" << endl;
	db->printArrayOfServices(s1,size);

	db->removeService("ANY", "RemoteDesktop");
	//let's see the services table again
	//retrieving service struct pointer and size
	s1=db->getServices(size);
	//printing the whole struct array
	cout << "Services Table(removed all users with RemoteDesktop service):" << endl;
	db->printArrayOfServices(s1,size);

	db->removeService("user4@cbox", "ANY");
	//let's see the services table again
	//retrieving service struct pointer and size
	s1=db->getServices(size);
	//printing the whole struct array
	cout << "Services Table(removed all services of user4@cbox):" << endl;
	db->printArrayOfServices(s1,size);

	//let's see the user's services now
	//retrieving service struct pointer and size
	s1=db->getMyServices(size);
	//printing the whole struct array
	cout << "User's myServices Table:" << endl;
	db->printArrayOfServices(s1,size);

	db->removeMyService("music");
	//let's see the user's services again
	//retrieving service struct pointer and size
	s1=db->getMyServices(size);
	//printing the whole struct array
	cout << "User's myServices Table(removed usrer's music service:" << endl;
	db->printArrayOfServices(s1,size);

	db->removeMyService("ANY");
	//let's see the user's services again
	//retrieving service struct pointer and size
	s1=db->getMyServices(size);
	//printing the whole struct array
	cout << "User's myServices Table(removed all user's services):" << endl;
	db->printArrayOfServices(s1,size);

	return 0;
}


