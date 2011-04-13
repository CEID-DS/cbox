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

#include "PCDebug.h"

//function that implements the debug concept
void PCDebug(const char* msg, ...)
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
	if(jcClass==NULL) { ERROR="GetObjectClass error"; goto ABORT; }
	{
	//getting the id of method testNative which has no parameters and returns void
	jmethodID debug = env->GetMethodID(jcClass, "testNative", "(Ljava/lang/String;)V");
	//calling the method testNative

	if(debug==NULL)	{ ERROR="GetMethodID error"; goto ABORT; }
	jstring _jstring = env->NewStringUTF ((const char *) buffer);
	env->CallVoidMethod(thiz, debug, _jstring);
	ERROR="OK";
	}

	ABORT:;


}
