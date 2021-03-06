#include <iostream>
#include <sstream>
#include <string>
#include "../../../../../android_wiselib/androidConcepts/debugConcept/javaEssentials.h"
//including tinyXml library
#include "../tinyXml/tinystr.h"
#include "../tinyXml/tinyxml.h"

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
	//returns an xml string equal to service struct
	string createStructXmlString(struct ZcDbBindings::service *s, string str);
	//sends a string to java side
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

/* Creates an xml string equal to service struct that will be used by
 * addService and addMyService methods. The xml is created using the
 * tinyXml library.
 */
string ZcDbBindings::createStructXmlString(struct ZcDbBindings::service *s, string str)
{
	//creating an xml document that will hold the data
	TiXmlDocument doc;

	stringstream portStr, TTLStr, advertisedStr, questionedStr;
	portStr << s->port;
	TTLStr << s->TTL;
	advertisedStr << s->advertised;
	questionedStr << s->questioned;

 	TiXmlDeclaration* decl = new TiXmlDeclaration("1.0","","");
	doc.LinkEndChild(decl);

	//adding database tag with attributes
	TiXmlElement *database = new TiXmlElement("Database");
	doc.LinkEndChild(database);
	database->SetAttribute("function",str.c_str());

		//adding data tags
			TiXmlElement *hostname = new TiXmlElement("hostname");
			hostname->LinkEndChild( new TiXmlText(s->hostname.c_str()));
			database->LinkEndChild(hostname);

			TiXmlElement *serviceType = new TiXmlElement("serviceType");
			serviceType->LinkEndChild(new TiXmlText(s->serviceType.c_str()));
			database->LinkEndChild(serviceType);

			TiXmlElement *protocol = new TiXmlElement("protocol");
			protocol->LinkEndChild( new TiXmlText(s->protocol.c_str()));
			database->LinkEndChild(protocol);

			TiXmlElement *interface = new TiXmlElement("interface");
			interface->LinkEndChild(new TiXmlText(s->interface.c_str()));
			database->LinkEndChild(interface);

			TiXmlElement *port = new TiXmlElement("port");
			port->LinkEndChild( new TiXmlText(portStr.str().c_str()));
			database->LinkEndChild(port);

			TiXmlElement *TXTDATA = new TiXmlElement("TXTDATA");
			TXTDATA->LinkEndChild(new TiXmlText(s->TXTDATA.c_str()));
			database->LinkEndChild(TXTDATA);

			TiXmlElement *TTL = new TiXmlElement("TTL");
			TTL->LinkEndChild( new TiXmlText(TTLStr.str().c_str()));
			database->LinkEndChild(TTL);

			TiXmlElement *advertised = new TiXmlElement("advertised");
			advertised->LinkEndChild(new TiXmlText(advertisedStr.str().c_str()));
			database->LinkEndChild(advertised);

			TiXmlElement *questioned = new TiXmlElement("questioned");
			questioned->LinkEndChild(new TiXmlText(questionedStr.str().c_str()));
			database->LinkEndChild(questioned);

	// Declare a printer
	TiXmlPrinter printer;
	// attach it to the document you want to convert in to a std::string
	doc.Accept(&printer);
	// Create a std::string and copy your document data in to the string
	string xmlString = printer.CStr();

	return xmlString;
}

/* Sends a string to a java method(databaseFromNative). Actually this
 * method is used to send the xml data to java side in order to store
 * them in the database.
 */
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
	//creating the xml string
	xmlString=ZcDbBindings::createStructXmlString(&s,"addService");
	//send it to java!
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
	//creating the xml string
	xmlString=ZcDbBindings::createStructXmlString(&s,"addMyService");
	//send it to java!
	ZcDbBindings::sendStringToJava(xmlString);

	return 0;
}

/* Updates TTL times from both tables services and myServices for all services
*  (TTL = TTL – t , where t is a specific value)
*/
void ZcDbBindings::refreshTTL(int t)
{
	stringstream time;
	time << t; //making integer a string
	//creating the document that will hold the xml data
	TiXmlDocument doc;
	//declaring xml 1.0
 	TiXmlDeclaration* decl = new TiXmlDeclaration("1.0","","");
	doc.LinkEndChild(decl);
	//adding database tag with attributes
	TiXmlElement *database = new TiXmlElement("Database");
	doc.LinkEndChild(database);
	database->SetAttribute("function","refreshTTL");
	//adding data tags
		TiXmlElement *newTTL = new TiXmlElement("newTTL");
		newTTL->LinkEndChild( new TiXmlText(time.str().c_str()));
		database->LinkEndChild(newTTL);

	// Declare a printer
	TiXmlPrinter printer;
	// attach it to the document you want to convert in to a std::string
	doc.Accept(&printer);
	// Create a std::string and copy your document data in to the string
	string xmlString = printer.CStr();

	//sending the xml data to java side!
	ZcDbBindings::sendStringToJava(xmlString);


}

/* Removes a record from services table.It does also a cheking in hostname and serviceType.
*  More specifically, if a hostname is ANY, it removes the services of ALL hostnames.
*  The same stands for the service types.
*/
bool ZcDbBindings::removeService(string hostname, string serviceType)
{
	//creating the document that will hold the xml data
	TiXmlDocument doc;
	//declaring xml 1.0
 	TiXmlDeclaration* decl = new TiXmlDeclaration("1.0","","");
	doc.LinkEndChild(decl);
	//adding database tag with attributes
	TiXmlElement *database = new TiXmlElement("Database");
	doc.LinkEndChild(database);
	database->SetAttribute("function","removeService");
	//adding data tags
		TiXmlElement *hostnameXml = new TiXmlElement("hostname");
		hostnameXml->LinkEndChild( new TiXmlText(hostname.c_str()));
		database->LinkEndChild(hostnameXml);

		TiXmlElement *serviceTypeXml = new TiXmlElement("serviceType");
		serviceTypeXml->LinkEndChild( new TiXmlText(serviceType.c_str()));
		database->LinkEndChild(serviceTypeXml);

	// Declare a printer
	TiXmlPrinter printer;
	// attach it to the document you want to convert in to a std::string
	doc.Accept(&printer);
	// Create a std::string and copy your document data in to the string
	string xmlString = printer.CStr();

	//sending the xml data to java side!
	ZcDbBindings::sendStringToJava(xmlString);

	return true;
}

/* Removes a record from services table. The method accepts string arguments
*  but eventually uses QStrings. It does also a cheking in serviceType. More
*  specifically, if a service type is ANY, it should remove ALL services.
*/
bool ZcDbBindings::removeMyService(string serviceType)
{
	//creating the document that will hold the xml data
	TiXmlDocument doc;
	//declaring xml 1.0
 	TiXmlDeclaration* decl = new TiXmlDeclaration("1.0","","");
	doc.LinkEndChild(decl);
	//adding database tag with attributes
	TiXmlElement *database = new TiXmlElement("Database");
	doc.LinkEndChild(database);
	database->SetAttribute("function","removeMyService");
	//adding data tags
		TiXmlElement *serviceTypeXml = new TiXmlElement("serviceType");
		serviceTypeXml->LinkEndChild( new TiXmlText(serviceType.c_str()));
		database->LinkEndChild(serviceTypeXml);

	// Declare a printer
	TiXmlPrinter printer;
	// attach it to the document you want to convert in to a std::string
	doc.Accept(&printer);
	// Create a std::string and copy your document data in to the string
	string xmlString = printer.CStr();

	//sending the xml data to java side!
	ZcDbBindings::sendStringToJava(xmlString);

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
	//creating the document that will hold the xml data
	TiXmlDocument doc;
	//declaring xml 1.0
 	TiXmlDeclaration* decl = new TiXmlDeclaration("1.0","","");
	doc.LinkEndChild(decl);
	//adding database tag with attributes
	TiXmlElement *database = new TiXmlElement("Database");
	doc.LinkEndChild(database);
	database->SetAttribute("function","addDnsRecord");
	//adding data tags
		TiXmlElement *hostnameXml = new TiXmlElement("hostname");
		hostnameXml->LinkEndChild( new TiXmlText(hostname.c_str()));
		database->LinkEndChild(hostnameXml);

		TiXmlElement *interfaceXml = new TiXmlElement("interface");
		interfaceXml->LinkEndChild( new TiXmlText(interface.c_str()));
		database->LinkEndChild(interfaceXml);

		TiXmlElement *ipXml = new TiXmlElement("ip");
		ipXml->LinkEndChild( new TiXmlText(ip.c_str()));
		database->LinkEndChild(ipXml);

	// Declare a printer
	TiXmlPrinter printer;
	// attach it to the document you want to convert in to a std::string
	doc.Accept(&printer);
	// Create a std::string and copy your document data in to the string
	string xmlString = printer.CStr();

	//sending the xml data to java side!
	ZcDbBindings::sendStringToJava(xmlString);

	return true;
}
/* Removes a record from DNSTable.
*/
bool ZcDbBindings::removeDnsRecord(string hostname, string interface)
{
	//creating the document that will hold the xml data
	TiXmlDocument doc;
	//declaring xml 1.0
 	TiXmlDeclaration* decl = new TiXmlDeclaration("1.0","","");
	doc.LinkEndChild(decl);
	//adding database tag with attributes
	TiXmlElement *database = new TiXmlElement("Database");
	doc.LinkEndChild(database);
	database->SetAttribute("function","removeDnsRecord");
	//adding data tags
		TiXmlElement *hostnameXml = new TiXmlElement("hostname");
		hostnameXml->LinkEndChild( new TiXmlText(hostname.c_str()));
		database->LinkEndChild(hostnameXml);

		TiXmlElement *interfaceXml = new TiXmlElement("interface");
		interfaceXml->LinkEndChild( new TiXmlText(interface.c_str()));
		database->LinkEndChild(interfaceXml);

	// Declare a printer
	TiXmlPrinter printer;
	// attach it to the document you want to convert in to a std::string
	doc.Accept(&printer);
	// Create a std::string and copy your document data in to the string
	string xmlString = printer.CStr();

	//sending the xml data to java side!
	ZcDbBindings::sendStringToJava(xmlString);
	return true;
}
/* Updates a record in DNSTable.
*/
bool ZcDbBindings::updateDnsRecord(string hostname, string interface, string ip)
{
	//creating the document that will hold the xml data
	TiXmlDocument doc;
	//declaring xml 1.0
 	TiXmlDeclaration* decl = new TiXmlDeclaration("1.0","","");
	doc.LinkEndChild(decl);
	//adding database tag with attributes
	TiXmlElement *database = new TiXmlElement("Database");
	doc.LinkEndChild(database);
	database->SetAttribute("function","updateDnsRecord");
	//adding data tags
		TiXmlElement *hostnameXml = new TiXmlElement("hostname");
		hostnameXml->LinkEndChild( new TiXmlText(hostname.c_str()));
		database->LinkEndChild(hostnameXml);

		TiXmlElement *interfaceXml = new TiXmlElement("interface");
		interfaceXml->LinkEndChild( new TiXmlText(interface.c_str()));
		database->LinkEndChild(interfaceXml);

		TiXmlElement *ipXml = new TiXmlElement("ip");
		ipXml->LinkEndChild( new TiXmlText(ip.c_str()));
		database->LinkEndChild(ipXml);

	// Declare a printer
	TiXmlPrinter printer;
	// attach it to the document you want to convert in to a std::string
	doc.Accept(&printer);
	// Create a std::string and copy your document data in to the string
	string xmlString = printer.CStr();

	//sending the xml data to java side!
	ZcDbBindings::sendStringToJava(xmlString);

	return true;
}

/* Function that prints in the cli the struct array s of size &size. It is ideal
*  for debugging purposes.
*/
void ZcDbBindings::printArrayOfServices(struct ZcDbBindings::service *s, int &size)
{

}

