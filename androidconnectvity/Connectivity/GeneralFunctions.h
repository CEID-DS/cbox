/* 
 * File:   GeneralFunctions.h
 * Author: emmanouil
 *
 * Created on May 12, 2011, 7:27 PM
 */

#ifndef GENERALFUNCTIONS_H
#define	GENERALFUNCTIONS_H

#include <iostream>
#include <vector>
#include <sstream>
#include <istream>
#include <stdlib.h>
#include <string.h>

using namespace std;

class GeneralFunctions {
public:
    GeneralFunctions();
    virtual ~GeneralFunctions();
    
private:

};

string stringtoint(int number);
string getfromtag(string tag, string source);
vector<string> stringsplit(string tosplit);
string getpath();
char *stringtochar(string str);

#endif	/* GENERALFUNCTIONS_H */

