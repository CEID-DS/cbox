#include <iostream>
#include <string>
#include "ZcDbBindings.h"
using namespace std;

int main(int argc, char *argv[])
{

	ZcDbBindings *db = new ZcDbBindings;

	db->test();
	db->test();
	//db->addDnsRecord("koukou","lalalala","192.192.192.192");
	db->updateDnsRecord("koukou", "lalalala","19191919");
	db->getServicesNum();

	char **array;
	int size=0;

	array=db->getRequestedService(size);
	for(int i=0; i<size; i++)
	{
		cout << array[i] << endl;
	}


	return 0;
}


