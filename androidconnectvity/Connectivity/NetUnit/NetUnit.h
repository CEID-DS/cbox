/* 
 * File:   NetUnit.h
 * Author: carnage
 *
 * Created on May 12, 2011, 1:54 PM
 */

#ifndef NETUNIT_H
#define	NETUNIT_H

#include <iostream>
#include <boost/thread.hpp>
//#include <boost/thread/thread.hpp>
//#include <boost/bind/bind.hpp>
#include "ReceiveThread.h" 


using namespace std;

class NetUnit {
public:
    NetUnit();
    virtual ~NetUnit();
    void Receive();
private:

};

#endif	/* NETUNIT_H */

