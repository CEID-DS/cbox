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
	//adds a record in DNSTable
	ZcDbBindings();
	~ZcDbBindings();
	char** getRequestedService(int &size);
	int getServicesNum(void);
	bool addDnsRecord(string hostname, string interface, string ip);
	bool removeDnsRecord(string hostname, string interface);
	bool updateDnsRecord(string hostname, string interface, string ip);
	int test(void);

private:
	//creates a connection to zeroconf database.It is called from every binding function
	bool createConnection();
	//destroys the connection.It is called from every binding function
	bool closeConnection();
	QSqlDatabase db;
	QString conName;
};

ZcDbBindings::ZcDbBindings()
{
	createConnection();
}
ZcDbBindings::~ZcDbBindings()
{
	closeConnection();
}
/* Creates a connection to zeroconf database.It is called from every
   binding function
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
   the reference table.It is called from every binding function
*/
bool ZcDbBindings::closeConnection()
{
	db.close();
	delete &db;
	QSqlDatabase::removeDatabase(conName);
	return true;
}

char** ZcDbBindings::getRequestedService(int &size)
{

	QSqlTableModel model;
	model.setTable("requestedServices");
	model.select();
	
	
	char **services = (char **) malloc(model.rowCount()*sizeof(char*));
	for(int i=0; i<model.rowCount(); i++ )
	{
		services[i] = (char *) malloc(16*sizeof(char));
	}
	size=model.rowCount();
	
	for(int i=0; i<model.rowCount(); i++)
	{
		QSqlRecord record = model.record(i);
		QString serviceType = record.value("serviceType").toString();
		strcpy(services[i],serviceType.toStdString().c_str());
	}
	
	for(int i=0; i<model.rowCount(); i++)
	{
		cout << services[i] << endl;
	}
	


	return services;
}


int ZcDbBindings::getServicesNum(void)
{

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

	QString Qhostname(hostname.c_str());
	QString Qinterface(interface.c_str());
	QString Qip(ip.c_str());

	QSqlQuery query;
	query.prepare("INSERT INTO DNSTable (hostname,interface,ip) values"
					"(:hostname, :interface, :ip)");
	query.bindValue(":hostname", Qhostname);
	query.bindValue(":interface", Qinterface);
	query.bindValue(":ip", Qip);
	query.exec();


	return true;
}
/* Adds a record in DNSTable.
   The method accepts string arguments but eventually uses QStrings
*/
bool ZcDbBindings::removeDnsRecord(string hostname, string interface)
{

	QString Qhostname(hostname.c_str());
	QString Qinterface(interface.c_str());

	QString mesQuery = QString("DELETE FROM DNSTable WHERE hostname='%1' AND interface='%2'").arg(Qhostname).arg(Qinterface);
	QSqlQuery query(mesQuery);

	return true;
}
/* Adds a record in DNSTable.
   The method accepts string arguments but eventually uses QStrings
*/
bool ZcDbBindings::updateDnsRecord(string hostname, string interface, string ip)
{

	QString Qhostname(hostname.c_str());
	QString Qinterface(interface.c_str());
	QString Qip(ip.c_str());

	QString mesQuery = QString("UPDATE DNSTable SET ip='%1' WHERE hostname='%2' AND interface='%3'").arg(Qip).arg(Qhostname).arg(Qinterface);
	QSqlQuery query(mesQuery);


	return true;
}


int ZcDbBindings::test(void)
{
	QSqlQuery query;
	query.exec("SELECT * FROM DNSTable");

	while(query.next())
	{
		QString title= query.value(1).toString();
		cout << "title: " << qPrintable(title) << endl;

	}

	return true;
}
