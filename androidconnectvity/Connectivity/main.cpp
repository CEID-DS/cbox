/* 
 * File:   main.cpp
 * Author: carnage
 *
 * Created on May 12, 2011, 1:53 PM
 */

#include <cstdlib>
#include <iostream>
#include <cstring>
#include <fstream>
#include <sys/stat.h>
#include "GeneralFunctions.h"
#include "NetUnit/NetUnit.h"

using namespace std;

/*
 * 
 */
NetUnit netUnit;
GeneralFunctions generalFunctions;

int main(int argc, char** argv) {

    string command;
    vector<string> commandvector;
     
    while(1){
        cout<<"debugshell$ ";
        getline(cin,command);
        commandvector=stringsplit(command);         
        
        if(command.find("netreceive",0)==0){
            
            netUnit.Receive();
        }
        else if(command.find("netransmit",0)==0){
            
            
           
           //char *data=new char[filestatus.st_size];
           
           //with the data you want
           //netUnit.Transmit(commandvector[1],data,filestatus.st_size);

           
        }
        else if(command.find("exit",0)==0){
            return 1;
        }
        else{
            cout<<"Unknown Command"<<endl;
        }
    }
    
    return 0;
}

