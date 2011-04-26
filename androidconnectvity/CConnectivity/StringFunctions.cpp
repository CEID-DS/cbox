#include "StringFunctions.h"

std::string stringtoint(int number){

	std::stringstream s;
	s << number ;
	return s.str();
}

std::string getfromtag(std::string tag,std::string source){

	int start,end;
	start=source.find("<"+tag);
	end=source.find("</"+tag);
	
	return source.substr(start+1+tag.size(),end-1-tag.size());
}
