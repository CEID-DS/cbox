/******************************************************************************
* This file is part of cbox.
*
* cbox is free software: you can redistribute it and/or modify
* it under the terms of the GNU LesserGeneral Public License as published
* by the Free Software Foundation, either version 3 of the License, or
* any later version.
*
* Cbox is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with cbox.  If not, see <http://www.gnu.org/licenses/>.
*******************************************************************************/

#ifndef ANDROIDRADIOMODEL_H
#define ANDROIDRADIOMODEL_H

#include <jni.h>
#include <iostream>
#include <string>
#include "delegate.hpp"
#include "../debugConcept/javaEssentials.h"
using namespace std;

class AndroidRadioModel{
public:
	//specifying the member function prototype that will be called by the receive
	typedef delegate2<void,int, void*> radio_delegate_t;

	AndroidRadioModel(void);
	~AndroidRadioModel();
	int enable_radio();
	int disable_radio();
	//function that implements the send concept
	int send(string s);
	//template method that inserts a function callback in the static callback array
	template<class T, void (T::*TMethod)(int, void*)>
	int reg_recv_callback( T *obj_pnt );
	//function that calls all the functions when receiving a new message
	void receive_message( int from, int len, void *buf );
private:
	//private method that is called by set_timer to add an event to the timer
	int insert(radio_delegate_t callback);
	//private function that does the actual work of receive_message
	void notify_receivers( int from, int len, void *data );

	//struct that holds the necessary info for every callback
	struct Resources{
		radio_delegate_t membFunct;
		bool registered;
	};
	//declaring an array of 10 Resources structs
	static const int MAX_EVENTS=10;
	static struct Resources ResData[MAX_EVENTS];

};

//constructor that initializes all the necessary variables
inline AndroidRadioModel::AndroidRadioModel(void)
{

}

inline AndroidRadioModel::~AndroidRadioModel()
{

}

inline int AndroidRadioModel::enable_radio()
{
	//initializing the struct that holds all the necessary info
	for(int i=0; i<MAX_EVENTS; i++)
	{
		ResData[i].registered=false;
	}
	
	return true;
}

//function that implements the send concept
inline int AndroidRadioModel::send(string s)
{
	jobject thiz = getJavaObject();
	JNIEnv *env;
	env=getJavaENV();

	//getting the class that represents the java object thiz
	jclass jcClass = env->GetObjectClass(thiz);
	if(jcClass==NULL) { myError="GetObjectClass error"; goto ABORT; }
	{
		//getting the id of method testNative which has one string parameter and returns void
		jmethodID debug = env->GetMethodID(jcClass, "UdpSendNative", "(Ljava/lang/String;)V");
		//calling the method UdpSend
		if(debug==NULL)	{ myError="GetMethodID error"; goto ABORT; }
		jstring _jstring = env->NewStringUTF (s.c_str());
		env->CallVoidMethod(thiz, debug, _jstring);
		myError="OK";
	}

	ABORT:;

}

template<class T, void (T::*TMethod)(int, void*)>
inline int AndroidRadioModel::reg_recv_callback( T *obj_pnt )
{
	//insert the new event in the array struct
	insert(radio_delegate_t::from_method<T, TMethod>(obj_pnt));

	return 0;

}
//private method that is called by reg_recv_callback to register(add) a new callback
inline int AndroidRadioModel::insert(radio_delegate_t callback)
{

	bool hasCell=false;

	for(int i=0; i<MAX_EVENTS; i++)
	{
		if(ResData[i].registered==false)
		{
			ResData[i].membFunct=callback;
			ResData[i].registered=true;
			hasCell=true;
			break;
		}
	}
}
//member function that receives a new message and sends it to the registered receivers
inline void AndroidRadioModel::receive_message( int from, int len, void *buf )
{
	//caling all the registered receivers
	AndroidRadioModel::notify_receivers(from,len,buf);

}
//private member that calls back all the registered functions
inline void AndroidRadioModel::notify_receivers( int from, int len, void *data )
{
	for(int i=0; i<MAX_EVENTS; i++)
	{
		if(ResData[i].registered==true)
		{
			(ResData[i].membFunct)(len,data);
		}
	}
}


#endif
