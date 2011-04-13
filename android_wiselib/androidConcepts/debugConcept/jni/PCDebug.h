#ifndef PCDEBUG_H
#define PCDEBUG_H

#include <jni.h>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
#include <cstdarg>
#include "javaEssentials.h"
using namespace std;

class AndrDebug{
	public:
		//function that implements the debug concept
		void debug(const char* msg, ...)
		{
			va_list fmtargs;
			char buffer[1024];
			va_start(fmtargs, msg);
			vsnprintf(buffer, sizeof(buffer) - 1, msg, fmtargs);
			va_end(fmtargs);

			jobject thiz = getJavaObject();
			JNIEnv *env;
			env=getJavaENV();

			//getting the class that represents the java object thiz
			jclass jcClass = env->GetObjectClass(thiz);
			if(jcClass==NULL) { myError="GetObjectClass error"; goto ABORT; }
			{
			//getting the id of method testNative which has no parameters and returns void
			jmethodID debug = env->GetMethodID(jcClass, "testNative", "(Ljava/lang/String;)V");
			//calling the method testNative

			if(debug==NULL)	{ myError="GetMethodID error"; goto ABORT; }
			jstring _jstring = env->NewStringUTF ((const char *) buffer);
			env->CallVoidMethod(thiz, debug, _jstring);
			myError="OK";
			}

			ABORT:;

		}
};

#endif
