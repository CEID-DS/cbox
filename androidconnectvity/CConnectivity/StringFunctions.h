
#ifndef STRINGFUNCTIONS_H_
#define STRINGFUNCTIONS_H_

#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>

std::string stringtoint(int number);
std::string getfromtag(std::string tag,std::string source);
char* getchecksum(char * buffer,long length);

#endif 
