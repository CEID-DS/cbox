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
#include "javaEssentials.h"
#include "cbox_androidWiselib_debugConc_NativeConnection.h"
#include <jni.h>

//function that tests the debug concept
void testDebug(void)
{
	PCDebug("test test test test\n:)\n");
}



JNIEXPORT jstring JNICALL Java_cbox_androidWiselib_debugConc_NativeConnection_debugConceptJNI
  (JNIEnv *env, jobject nc, jobject thiz)
{
	//setting the global java environment variables
	setJavaENV(env);
	setJavaObject(thiz);

	//testing the debug feature
	testDebug();
	return env->NewStringUTF(ERROR.c_str());
}




