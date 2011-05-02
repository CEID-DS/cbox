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

char* getchecksum(char* buffer,long length){

	static char tBuf[4];
	long index;
	unsigned int checksum;

	for(index = 0L,checksum = 0;index <length;checksum +=(unsigned int) buffer[index++]);
	sprintf(tBuf,"%03d",(unsigned int)(checksum%256));
	return(tBuf);

	//return checksum;
}
