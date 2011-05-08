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
	//accepts an argument of type struct service and adds its contents to the table Services
	int addService(service s);
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
	//prints in the cli the struct array s of size &size
	void printArrayOfServices(struct ZcDbBindings::service *s, int &size);

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
	ZcDbBindings::createConnection();
}

/* Destructor that closes the connection of zeroconf database.
*/
ZcDbBindings::~ZcDbBindings()
{
	ZcDbBindings::closeConnection();
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

/* 	Returns a pointer to an array of structs of type service that contains
*   the contents of table services.
*/
struct ZcDbBindings::service* ZcDbBindings::getServices(int &size)
{
	QSqlTableModel model;
	model.setTable("services");
	model.select();

	//watch out here, new is used instead of malloc due to string class variables inside the struct
	struct ZcDbBindings::service* s = new struct ZcDbBindings::service[model.rowCount()];
	//storing the size of the array
	size=model.rowCount();

	//writing the mysql results into the struct array
	for(int i=0; i<model.rowCount(); i++)
	{
		QSqlRecord record = model.record(i);
		s[i].hostname = record.value("hostname").toString().toStdString();
		s[i].serviceType = record.value("serviceType").toString().toStdString();
		s[i].protocol = record.value("protocol").toString().toStdString();
		s[i].interface = record.value("interface").toString().toStdString();
		s[i].port = record.value("port").toInt();
		s[i].TXTDATA = record.value("TXTDATA").toString().toStdString();
		s[i].TTL = record.value("TTL").toInt();
		s[i].advertised = record.value("advertised").toBool();
		s[i].questioned = record.value("questioned").toBool();
	}
	//returning the pointer
	return s;
}

/* 	Returns a pointer to an array of structs of type service that contains
*   the contents of table myServices.
*/
struct ZcDbBindings::service* ZcDbBindings::getMyServices(int &size)
{
	QSqlTableModel model;
	model.setTable("myServices");
	model.select();

	//watch out here, new is used instead of malloc due to string class variables inside the struct
	struct ZcDbBindings::service* s = new struct ZcDbBindings::service[model.rowCount()];
	//storing the size of the array
	size=model.rowCount();

	//writing the mysql results into the struct array
	for(int i=0; i<model.rowCount(); i++)
	{
		QSqlRecord record = model.record(i);
		s[i].hostname = record.value("hostname").toString().toStdString();
		s[i].serviceType = record.value("serviceType").toString().toStdString();
		s[i].protocol = record.value("protocol").toString().toStdString();
		s[i].interface = record.value("interface").toString().toStdString();
		s[i].port = record.value("port").toInt();
		s[i].TXTDATA = record.value("TXTDATA").toString().toStdString();
		s[i].TTL = record.value("TTL").toInt();
		s[i].advertised = record.value("advertised").toBool();
		s[i].questioned = record.value("questioned").toBool();
	}
	//returning the pointer
	return s;
}

/* Accepts an argument of type struct service and adds its contents to
*  the table Services. If a mysql error appears during the insert, the
*  error is printed in the cli.
*/
int ZcDbBindings::addService(ZcDbBindings::service s)
{
	//declaring Qt strings
	QString Qhostname(s.hostname.c_str());
	QString QserviceType(s.serviceType.c_str());
	QString Qprotocol(s.protocol.c_str());
	QString Qinterface(s.interface.c_str());
	QString QTXTDATA(s.TXTDATA.c_str());

	//constructing the mysql query
	QSqlQuery query;

	query.prepare("INSERT INTO services (hostname, serviceType, protocol, interface, port, TXTDATA, TTL, advertised, questioned) VALUES "
					"(:hostname, :serviceType, :protocol, :interface, :port, :TXTDATA, :TTL, :advertised, :questioned)");
	query.bindValue(":hostname", Qhostname);
	query.bindValue(":serviceType", QserviceType);
	query.bindValue(":protocol", Qprotocol);
	query.bindValue(":interface", Qinterface);
	query.bindValue(":port", s.port);
	query.bindValue(":TXTDATA", QTXTDATA);
	query.bindValue(":TTL", s.TTL);
	query.bindValue(":advertised", s.advertised);
	query.bindValue(":questioned", s.questioned);

    if(!query.exec())
	{
		qDebug() << "> Query exec() error." << query.lastError();
	}
    //else qDebug() << ">Query exec() success.";

	return 0;
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
	QSqlQuery query;
    if(!query.exec(mesQuery))
	{
		qDebug() << "> Query exec() error." << query.lastError();
	}
    //else qDebug() << ">Query exec() success.";

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
	query.prepare("INSERT INTO DNSTable (hostname,interface,ip) values "
					"(:hostname, :interface, :ip)");
	query.bindValue(":hostname", Qhostname);
	query.bindValue(":interface", Qinterface);
	query.bindValue(":ip", Qip);
	//executing the query
    if(!query.exec())
	{
        qDebug() << "> Query exec() error." << query.lastError();
	}
    //else qDebug() << ">Query exec() success.";

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
	QSqlQuery query;
    if(!query.exec(mesQuery))
	{
        qDebug() << "> Query exec() error." << query.lastError();
	}
    //else qDebug() << ">Query exec() success.";

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
	QSqlQuery query;
    if(!query.exec(mesQuery))
	{
        qDebug() << "> Query exec() error." << query.lastError();
	}
    //else qDebug() << ">Query exec() success.";

	return true;
}

/* Function that prints in the cli the struct array s of size &size. It is ideal
*  for debugging purposes.
*/
void ZcDbBindings::printArrayOfServices(struct ZcDbBindings::service *s, int &size)
{
	//just printing the struct array
	for(int i=0; i<size; i++)
	{
		cout << s[i].hostname << " ";
		cout << s[i].serviceType << " ";
		cout << s[i].protocol << " ";
		cout << s[i].interface << " ";
		cout << s[i].port << " ";
		cout << s[i].TXTDATA << " ";
		cout << s[i].TTL << " ";
		cout << s[i].advertised << " ";
		cout << s[i].questioned << " ";
		cout << endl;
	}
	cout << endl;
}

