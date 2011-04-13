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

/* This is a trivial JNI example where we use a native method
 * to return a new VM String.
 */

#include <string.h>
#include <jni.h>
#include <iostream>
#include "helloFromJNI.h"


JNIEXPORT jstring JNICALL Java_ceid_1ds2_cbox_helloandroid_NativeConnection_stringFromJNI(JNIEnv *env, jobject thiz)
{
    return env->NewStringUTF("Hello from native C++ code using NJI :-D");
}
