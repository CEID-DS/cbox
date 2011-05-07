#include <iostream>
#include <vector>
#include <string>
#include <QString>
#include <QtSql>
#include <QSqlTableModel>
using namespace std;

/*The class contains all the binding functions to zeroconf database*/
class ZcDbBindings
{
public:
	//constructor that creates a connection to zeroconf database
	ZcDbBindings();
	//destructor that closes the connection of zeroconf database
	~ZcDbBindings();
	//removes a service record from the database
	bool removeService(string hostname, string serviceType);
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

private:
	//creates a connection to zeroconf database.It is called from every binding function
	bool createConnection();
	//destroys the connection.It is called from every binding function
	bool closeConnection();
	//variable that holds database descriptor
	QSqlDatabase db;
	//variable that holds the database connection name(may be different from the actual database name)
	QString conName;
};

/* Constructor that creates a connection to zeroconf database
*/
ZcDbBindings::ZcDbBindings()
{
	createConnection();
}

/* Destructor that closes the connection of zeroconf database.
*/
ZcDbBindings::~ZcDbBindings()
{
	closeConnection();
}

/* Creates a connection to zeroconf database.It is called once in the
   constructor.
*/
bool ZcDbBindings::createConnection()
{
	//declaring a mysql driver
	db = QSqlDatabase::addDatabase("QMYSQL");
	//declaring of the database
	db.setHostName("localhost");
	db.setDatabaseName("zeroconf");
	db.setUserName("zeroUser");
	db.setPassword("password");
	if(!db.open())
	{
		return false;
	}
	//storing the connection name to be used with closeConnection() method
	conName=db.connectionName();

	return true;
}

/* Destroys the connection.Actually removes its reference from
   the reference table.It is called only once from the destructor
*/
bool ZcDbBindings::closeConnection()
{
	db.close();
	delete &db;
	QSqlDatabase::removeDatabase(conName);
	return true;
}

/* Removes a record from services table.
   The method accepts string arguments but eventually uses QStrings
*/
bool ZcDbBindings::removeService(string hostname, string serviceType)
{
	//declaring Qt strings
	QString Qhostname(hostname.c_str());
	QString QserviceType(serviceType.c_str());
	//constructing and executing the mysql query
	QString mesQuery = QString("DELETE FROM services WHERE hostname='%1' AND serviceType='%2'").arg(Qhostname).arg(QserviceType);
	QSqlQuery query(mesQuery);

	return true;
}
/* Returns a c-like array of all the requested services.
*/
char** ZcDbBindings::getRequestedService(int &size)
{
	//selecting from table requestedServices
	QSqlTableModel model;
	model.setTable("requestedServices");
	model.select();
	//allocating the array in the heap
	char **services = (char **) malloc(model.rowCount()*sizeof(char*));
	for(int i=0; i<model.rowCount(); i++ )
	{
		services[i] = (char *) malloc(16*sizeof(char));
	}
	//storing the size of the array
	size=model.rowCount();
	//storing the database values into the array
	for(int i=0; i<model.rowCount(); i++)
	{
		QSqlRecord record = model.record(i);
		QString serviceType = record.value("serviceType").toString();
		strcpy(services[i],serviceType.toStdString().c_str());
	}
	//return the array pointer
	return services;
}

/* Returns the number of services the user has in its disposition
*  Note: the services are modified by a higher layer (connectivity
*  layer or application layer).
*/
int ZcDbBindings::getServicesNum(void)
{
	//selecting from table myServices
	QSqlTableModel model;
	model.setTable("myServices");
	model.select();

	return model.rowCount();
}

/* Adds a record in DNSTable.
   The method accepts string arguments but eventually uses QStrings
*/
bool ZcDbBindings::addDnsRecord(string hostname, string interface, string ip)
{
	//declaring Qt strings
	QString Qhostname(hostname.c_str());
	QString Qinterface(interface.c_str());
	QString Qip(ip.c_str());
	//constructing the mysql query
	QSqlQuery query;
	query.prepare("INSERT INTO DNSTable (hostname,interface,ip) values"
					"(:hostname, :interface, :ip)");
	query.bindValue(":hostname", Qhostname);
	query.bindValue(":interface", Qinterface);
	query.bindValue(":ip", Qip);
	//executing the query
	query.exec();

	return true;
}
/* Removes a record from DNSTable.
   The method accepts string arguments but eventually uses QStrings
*/
bool ZcDbBindings::removeDnsRecord(string hostname, string interface)
{
	//declaring Qt strings
	QString Qhostname(hostname.c_str());
	QString Qinterface(interface.c_str());
	//constructing and executing the mysql query
	QString mesQuery = QString("DELETE FROM DNSTable WHERE hostname='%1' AND interface='%2'").arg(Qhostname).arg(Qinterface);
	QSqlQuery query(mesQuery);

	return true;
}
/* Updates a record in DNSTable.
   The method accepts string arguments but eventually uses QStrings
*/
bool ZcDbBindings::updateDnsRecord(string hostname, string interface, string ip)
{
	//declaring Qt strings
	QString Qhostname(hostname.c_str());
	QString Qinterface(interface.c_str());
	QString Qip(ip.c_str());
	//constructing and executing the mysql query
	QString mesQuery = QString("UPDATE DNSTable SET ip='%1' WHERE hostname='%2' AND interface='%3'").arg(Qip).arg(Qhostname).arg(Qinterface);
	QSqlQuery query(mesQuery);

	return true;
}

