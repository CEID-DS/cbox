/* 
 * File:   main.cpp
 * Author: carnage
 *
 * Created on May 12, 2011, 1:53 PM
 */

#include <cstdlib>
#include <iostream>
#include <cstring>
#include "NetUnit/NetUnit.h"

using namespace std;

/*
 * 
 */
NetUnit netUnit;

int main(int argc, char** argv) {

    string command;
    
    cout<<"Helper for the android connectivity"<<endl;

    while(1){
        cout<<"debugshell ";
        cin>>command;
        
        if(command.find("netreceiver",0)==0){
            
            netUnit.Receive();
        }
        else if(command.find("exit",0)==0){
            return 1;
        }
    }
    
    return 0;
}

