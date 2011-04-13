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

#include <jni.h>
#include "javaEssentials.h"

//string that holds the error(not used yet)
string myError;
//global variable that holds the Java environment
JNIEnv *myEnv;
//global variable that holds the Java object
jobject myJthiz;

//method that returns the Java object
jobject getJavaObject(void)
{
	return myJthiz;
}
//method that sets the Java object to a global variable
void setJavaObject(jobject thiz)
{
	myJthiz=thiz;
}
//method that returns the Java environment
JNIEnv* getJavaENV(void)
{
	return myEnv;
}
//method that sets the Java environment to a global variable
void setJavaENV(JNIEnv *envir)
{
	myEnv=envir;
}
