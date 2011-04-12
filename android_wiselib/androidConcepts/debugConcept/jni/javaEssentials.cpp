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

//method that returns the Java object
jobject getJavaObject(void)
{
	return jthiz;
}
//method that sets the Java object to a global variable
void setJavaObject(jobject thiz)
{
	jthiz=thiz;
}
//method that returns the Java environment
JNIEnv* getJavaENV(void)
{
	return env;
}
//method that sets the Java environment to a global variable
void setJavaENV(JNIEnv *envir)
{
	env=envir;
}
