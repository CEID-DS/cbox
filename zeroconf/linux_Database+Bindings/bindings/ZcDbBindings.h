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
*  the table services. If the entry already exists then it updates its
*  contents. If a mysql error appears during the insert, the error is
*  printed in the cli.
*/
int ZcDbBindings::addService(ZcDbBindings::service s)
{
	//declaring Qt strings
	QString Qhostname(s.hostname.c_str());
	QString QserviceType(s.serviceType.c_str());
	QString Qprotocol(s.protocol.c_str());
	QString Qinterface(s.interface.c_str());
	QString QTXTDATA(s.TXTDATA.c_str());

	//creating a QSqlTableModel to handle the select statement in order to see if we have
	//to update the value or we need to insert a new row
	QSqlTableModel model;
	model.setTable("services");
	//all changes to the model will be applied immediately to the database.
	model.setEditStrategy(QSqlTableModel::OnFieldChange);
	model.setFilter(QString("hostname='%1' AND serviceType='%2'").arg(Qhostname).arg(QserviceType));
	//executing the query
    if(!model.select())
	{
		qDebug() << "> Query model.select() error." << model.lastError();
	}
	//checking the number of rows that were returned
	if(model.rowCount()==1)
	{
		QSqlRecord record = model.record(0);
		record.setValue("hostname", Qhostname);
		record.setValue("serviceType", QserviceType);
		record.setValue("protocol", Qprotocol);
		record.setValue("interface", Qinterface);
		record.setValue("port", s.port);
		record.setValue("TXTDATA", QTXTDATA);
		record.setValue("TTL", s.TTL);
		record.setValue("advertised", s.advertised);
		record.setValue("questioned", s.questioned);
		model.setRecord(0, record);
	}
	//if no rows were returned then this means that no such value exists..
	else if(model.rowCount()==0)
	{
		//constructing the mysql query
		QSqlQuery insQuery;

		insQuery.prepare("INSERT INTO services (hostname, serviceType, protocol, interface, port, TXTDATA, TTL, advertised, questioned) VALUES "
						"(:hostname, :serviceType, :protocol, :interface, :port, :TXTDATA, :TTL, :advertised, :questioned)");
		insQuery.bindValue(":hostname", Qhostname);
		insQuery.bindValue(":serviceType", QserviceType);
		insQuery.bindValue(":protocol", Qprotocol);
		insQuery.bindValue(":interface", Qinterface);
		insQuery.bindValue(":port", s.port);
		insQuery.bindValue(":TXTDATA", QTXTDATA);
		insQuery.bindValue(":TTL", s.TTL);
		insQuery.bindValue(":advertised", s.advertised);
		insQuery.bindValue(":questioned", s.questioned);

		if(!insQuery.exec())
		{
			qDebug() << "> Query exec() error." << insQuery.lastError();
		}
		//else qDebug() << ">Query exec() success.";
	}
	//normally this should never be executed!
	else qDebug() << "> Error: Duplicate entry in database.";

	return 0;
}

/* Accepts an argument of type struct service and adds its contents to
*  the table myServices. If the entry already exists then it updates its
*  contents. If a mysql error appears during the insert, the error is
*  printed in the cli.
*/
int ZcDbBindings::addMyService(ZcDbBindings::service s)
{
	//declaring Qt strings
	QString Qhostname(s.hostname.c_str());
	QString QserviceType(s.serviceType.c_str());
	QString Qprotocol(s.protocol.c_str());
	QString Qinterface(s.interface.c_str());
	QString QTXTDATA(s.TXTDATA.c_str());

	//creating a QSqlTableModel to handle the select statement in order to see if we have
	//to update the value or we need to insert a new row
	QSqlTableModel model;
	model.setTable("myServices");
	//all changes to the model will be applied immediately to the database.
	model.setEditStrategy(QSqlTableModel::OnFieldChange);
	model.setFilter(QString("hostname='%1' AND serviceType='%2'").arg(Qhostname).arg(QserviceType));
	//executing the query
    if(!model.select())
	{
		qDebug() << "> Query model.select() error." << model.lastError();
	}
	//checking the number of rows that were returned
	if(model.rowCount()==1)
	{
		QSqlRecord record = model.record(0);
		record.setValue("hostname", Qhostname);
		record.setValue("serviceType", QserviceType);
		record.setValue("protocol", Qprotocol);
		record.setValue("interface", Qinterface);
		record.setValue("port", s.port);
		record.setValue("TXTDATA", QTXTDATA);
		record.setValue("TTL", s.TTL);
		record.setValue("advertised", s.advertised);
		record.setValue("questioned", s.questioned);
		model.setRecord(0, record);
	}
	//if no rows were returned then this means that no such value exists..
	else if(model.rowCount()==0)
	{
		//constructing the mysql query
		QSqlQuery insQuery;

		insQuery.prepare("INSERT INTO myServices (hostname, serviceType, protocol, interface, port, TXTDATA, TTL, advertised, questioned) VALUES "
						"(:hostname, :serviceType, :protocol, :interface, :port, :TXTDATA, :TTL, :advertised, :questioned)");
		insQuery.bindValue(":hostname", Qhostname);
		insQuery.bindValue(":serviceType", QserviceType);
		insQuery.bindValue(":protocol", Qprotocol);
		insQuery.bindValue(":interface", Qinterface);
		insQuery.bindValue(":port", s.port);
		insQuery.bindValue(":TXTDATA", QTXTDATA);
		insQuery.bindValue(":TTL", s.TTL);
		insQuery.bindValue(":advertised", s.advertised);
		insQuery.bindValue(":questioned", s.questioned);

		if(!insQuery.exec())
		{
			qDebug() << "> Query exec() error." << insQuery.lastError();
		}
		//else qDebug() << ">Query exec() success.";
	}
	//normally this should never be executed!
	else qDebug() << "> Error: Duplicate entry in database.";

	return 0;
}

/* Updates TTL times from both tables services and myServices for all services
*  (TTL = TTL – t , where t is a specific value)
*/
void ZcDbBindings::refreshTTL(int t)
{
	//creating a QSqlTableModel to handle the select statement
	QSqlTableModel model;
	//setting the database table
	model.setTable("services");
	//all changes to the model will be applied immediately to the database.
	model.setEditStrategy(QSqlTableModel::OnFieldChange);
	//executing the query
    if(!model.select())
	{
		qDebug() << "> Query model.select() error." << model.lastError();
	}
	//updating all the TTL values
	for(int i=0; i<model.rowCount(); i++)
	{
		QSqlRecord record = model.record(i);
		int TTL = record.value("TTL").toInt();
		if(TTL>=t)
		{
			record.setValue("TTL", TTL-t);
			model.setRecord(i, record);
		}
	}
	//resetting the database table
	model.setTable("myServices");
	//re-executing the query
    if(!model.select())
	{
		qDebug() << "> Query model.select() error." << model.lastError();
	}
	//updating all the TTL values
	for(int i=0; i<model.rowCount(); i++)
	{
		QSqlRecord record = model.record(i);
		int TTL = record.value("TTL").toInt();
		if(TTL>=t)
		{
			record.setValue("TTL", TTL-t);
			model.setRecord(i, record);
		}
	}
}

/* Removes a record from services table. The method accepts string arguments
*  but eventually uses QStrings. It does also a cheking in hostname and serviceType.
*  More specifically, if a hostname is ANY, it removes the services of ALL hostnames.
*  The same stands for the service types.
*/
bool ZcDbBindings::removeService(string hostname, string serviceType)
{
	//declaring Qt strings
	QString Qhostname(hostname.c_str());
	QString QserviceType(serviceType.c_str());

	//checking hostname string to see if we must delete all hostnames with service serviceType
	if(QString::compare(Qhostname, "ANY", Qt::CaseSensitive)==0)
	{
		QString delQuery = QString("DELETE FROM services where serviceType='%1'").arg(QserviceType);
		QSqlQuery query;
    	if(!query.exec(delQuery))
		{
			qDebug() << "> Query exec() error." << query.lastError();
		}
	}
	//checking serviceType string to see if we must delete all services of user hostname
	//(actually removes the user hostname from table services
	else if(QString::compare(QserviceType, "ANY", Qt::CaseSensitive)==0)
	{
		QString delQuery = QString("DELETE FROM services WHERE hostname='%1'").arg(Qhostname);
		QSqlQuery query;
    	if(!query.exec(delQuery))
		{
			qDebug() << "> Query exec() error." << query.lastError();
		}
	}
	else
	{
		//constructing and executing the mysql query
		QString mesQuery = QString("DELETE FROM services WHERE hostname='%1' AND serviceType='%2'").arg(Qhostname).arg(QserviceType);
		QSqlQuery query;
    	if(!query.exec(mesQuery))
		{
			qDebug() << "> Query exec() error." << query.lastError();
		}
    	//else qDebug() << ">Query exec() success.";
	}
	return true;
}

/* Removes a record from services table. The method accepts string arguments
*  but eventually uses QStrings. It does also a cheking in serviceType. More
*  specifically, if a service type is ANY, it should remove ALL services.
*/
bool ZcDbBindings::removeMyService(string serviceType)
{
	//declaring Qt strings
	QString QserviceType(serviceType.c_str());


	//checking serviceType string to see if we must delete all services the user
	if(QString::compare(QserviceType, "ANY", Qt::CaseSensitive)==0)
	{
		QString delQuery = QString("DELETE FROM myServices");
		QSqlQuery query;
    	if(!query.exec(delQuery))
		{
			qDebug() << "> Query exec() error." << query.lastError();
		}
	}
	else
	{
		//constructing and executing the mysql query
		QString mesQuery = QString("DELETE FROM myServices where serviceType='%1'").arg(QserviceType);
		QSqlQuery query;
    	if(!query.exec(mesQuery))
		{
			qDebug() << "> Query exec() error." << query.lastError();
		}
    	//else qDebug() << ">Query exec() success.";
	}
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
*  The method accepts string arguments but eventually uses QStrings
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
*  The method accepts string arguments but eventually uses QStrings
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
*  The method accepts string arguments but eventually uses QStrings
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

