/* 
 * File:   GeneralFunctions.cpp
 * Author: emmanouil
 * 
 * Created on May 12, 2011, 7:27 PM
 */

#include <vector>

#include "GeneralFunctions.h"

GeneralFunctions::GeneralFunctions() {
}

GeneralFunctions::~GeneralFunctions() {
}


string stringtoint(int number){
    stringstream s;
    s<<number;
    return s.str();
}

string getfromtag(string tag, string source) {

    int start, end;
    start = source.find("<" + tag);
    end = source.find("</" + tag);

    return source.substr(start + 1 + tag.size(), end - 1 - tag.size());
}

vector<string> stringsplit(string tosplit){
    istringstream iss(tosplit);
    vector<string> splits;
    
    while (iss) {

        string buf;
        iss>>buf;
        //cout<<buf<<endl;
        splits.push_back(buf);
    }

    return splits; 
}

string getpath(){
    char path[1024];
    if (getcwd(path, sizeof(path)) != NULL)
        cout<<path<<endl;
    else
        cout<<"asd";
            
    return path;
}

char *stringtochar(string str){
    
    char *buf = new char[strlen(str.c_str())];
    
    strcpy(buf,str.c_str());
    
    return buf;
}