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

#ifndef ANDROIDSEND_H
#define ANDROIDSEND_H

#include <jni.h>
#include <iostream>
#include <string>
#include "../../../androidConcepts/debugConcept/javaEssentials.h"
using namespace std;

class AndroidSend{
	public:

		AndroidSend(){
		}
		//function that implements the send concept
		int udpSend(string s)
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
};

#endif
