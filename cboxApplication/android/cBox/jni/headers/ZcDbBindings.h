#include <iostream>
#include <sstream>
#include <string>
#include "../../../../../android_wiselib/androidConcepts/debugConcept/javaEssentials.h"

using namespace std;

/*The class contains all the binding functions to zeroconf database*/
class ZcDbBindings
{
public:
	//struct that contains all the recors of the mysql table services
	struct service{
		string hostname;
		string serviceType;
		string protocol;
		string interface;
		int port;
		string TXTDATA;
		int TTL;
		bool advertised;
		bool questioned;
	};
	//constructor that creates a connection to zeroconf database
	ZcDbBindings();
	//destructor that closes the connection of zeroconf database
	~ZcDbBindings();
	//returns a pointer to an array of structs of type service that contains the contents of table services
	struct ZcDbBindings::service* getServices(int &size);
	//returns a pointer to an array of structs of type service that contains the contents of table myServices
	struct ZcDbBindings::service* getMyServices(int &size);
	//accepts an argument of type struct service and adds or updates its contents to the table services
	int addService(service s);
	//accepts an argument of type struct service and adds or updates its contents to the table myServices
	int addMyService(service s);
	//updates TTL times
	void refreshTTL(int t);
	//removes one or more services records from the database table services
	bool removeService(string hostname, string serviceType);
	//removes one or more services records from the database table myServices
	bool removeMyService(string serviceType);
	//returns an array that holds every requestedService
	char** getRequestedService(int &size);
	//returns the number of services the user has in its disposition
	int getServicesNum(void);
	//adds a dns record in the database
	bool addDnsRecord(string hostname, string interface, string ip);
	//removes a dns record from the database
	bool removeDnsRecord(string hostname, string interface);
	//updates the ip of a dns record in the database
	bool updateDnsRecord(string hostname, string interface, string ip);
	//prints in the cli the struct array s of size &size
	void printArrayOfServices(struct ZcDbBindings::service *s, int &size);

private:
	string createStructXmlString(struct ZcDbBindings::service *s, string str);
	void sendStringToJava(string s);
	//creates a connection to zeroconf database.It is called from every binding function
	bool createConnection();
	//destroys the connection.It is called from every binding function
	bool closeConnection();
};

/* Constructor that creates a connection to zeroconf database
*/
ZcDbBindings::ZcDbBindings()
{

}

/* Destructor that closes the connection of zeroconf database.
*/
ZcDbBindings::~ZcDbBindings()
{

}

/* Creates a connection to zeroconf database.It is called once in the
   constructor.
*/
bool ZcDbBindings::createConnection()
{


	return true;
}

/* Destroys the connection.Actually removes its reference from
   the reference table.It is called only once from the destructor
*/
bool ZcDbBindings::closeConnection()
{

	return true;
}

string ZcDbBindings::createStructXmlString(struct ZcDbBindings::service *s, string str)
{
	stringstream port, TTL, advertised, questioned;
	port << s->port;
	TTL << s->TTL;
	advertised << s->advertised;
	questioned << s->questioned;
	string xmlString="";
	xmlString.append(
	"<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>""\n"
		"<database>\n"
			"<"+str+">\n"
			"<hostname>"+s->hostname+"</hostname>\n"
			"<serviceType>"+s->serviceType+"</serviceType>\n"
			"<protocol>"+s->protocol+"</protocol>\n"
			"<interface>"+s->interface+"</interface>\n"
			"<port>"+port.str()+"</port>\n"
			"<TXTDATA>"+s->TXTDATA+"</TXTDATA>\n"
			"<TTL>"+TTL.str()+"</TTL>\n"
			"<advertised>"+advertised.str()+"</advertised>\n"
			"<questioned>"+questioned.str()+"</questioned>\n"
			"</"+str+">\n"
		"</database>");
	return xmlString;

}
void ZcDbBindings::sendStringToJava(string s)
{
	jobject thiz = getJavaObject();
	JNIEnv *env;
	env=getJavaENV();

	//getting the class that represents the java object thiz
	jclass jcClass = env->GetObjectClass(thiz);
	//getting the id of method testNative which has no parameters and returns void
	jmethodID debug = env->GetMethodID(jcClass, "databaseFromNative", "(Ljava/lang/String;)V");
	//calling the method testNative
	jstring _jstring = env->NewStringUTF ((const char *) s.c_str());
	env->CallVoidMethod(thiz, debug, _jstring);

}

/* 	Returns a pointer to an array of structs of type service that contains
*   the contents of table services.
*/
struct ZcDbBindings::service* ZcDbBindings::getServices(int &size)
{


}

/* 	Returns a pointer to an array of structs of type service that contains
*   the contents of table myServices.
*/
struct ZcDbBindings::service* ZcDbBindings::getMyServices(int &size)
{


}

/* Accepts an argument of type struct service and adds its contents to
*  the table services. If the entry already exists then it updates its
*  contents.
*/
int ZcDbBindings::addService(ZcDbBindings::service s)
{
	string xmlString;
	xmlString=ZcDbBindings::createStructXmlString(&s,"addService");
	ZcDbBindings::sendStringToJava(xmlString);

	return 0;
}

/* Accepts an argument of type struct service and adds its contents to
*  the table myServices. If the entry already exists then it updates its
*  contents.
*/
int ZcDbBindings::addMyService(ZcDbBindings::service s)
{
	string xmlString;
	xmlString=ZcDbBindings::createStructXmlString(&s,"addMyService");
	ZcDbBindings::sendStringToJava(xmlString);

	return 0;
}

/* Updates TTL times from both tables services and myServices for all services
*  (TTL = TTL – t , where t is a specific value)
*/
void ZcDbBindings::refreshTTL(int t)
{
	stringstream time;
	time << t;
	string xmlString="";
	xmlString.append(
	"<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>""\n"
		"<database>\n"
			"<refreshTTL>"+t.str()+"</refreshTTL>"
		"</database>\n");
	ZcDbBindings::sendStringToJava(xmlString);

}

/* Removes a record from services table.It does also a cheking in hostname and serviceType.
*  More specifically, if a hostname is ANY, it removes the services of ALL hostnames.
*  The same stands for the service types.
*/
bool ZcDbBindings::removeService(string hostname, string serviceType)
{

	return true;
}

/* Removes a record from services table. The method accepts string arguments
*  but eventually uses QStrings. It does also a cheking in serviceType. More
*  specifically, if a service type is ANY, it should remove ALL services.
*/
bool ZcDbBindings::removeMyService(string serviceType)
{

	return true;
}


/* Returns a c-like array of all the requested services.
*/
char** ZcDbBindings::getRequestedService(int &size)
{


}

/* Returns the number of services the user has in its disposition
*  Note: the services are modified by a higher layer (connectivity
*  layer or application layer).
*/
int ZcDbBindings::getServicesNum(void)
{


}

/* Adds a record in DNSTable.
*/
bool ZcDbBindings::addDnsRecord(string hostname, string interface, string ip)
{

	return true;
}
/* Removes a record from DNSTable.
*/
bool ZcDbBindings::removeDnsRecord(string hostname, string interface)
{
	return true;
}
/* Updates a record in DNSTable.
*/
bool ZcDbBindings::updateDnsRecord(string hostname, string interface, string ip)
{

	return true;
}

/* Function that prints in the cli the struct array s of size &size. It is ideal
*  for debugging purposes.
*/
void ZcDbBindings::printArrayOfServices(struct ZcDbBindings::service *s, int &size)
{

}

