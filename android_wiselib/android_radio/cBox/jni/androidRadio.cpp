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

#include <iostream>
#include <jni.h>
#include "com_cbox_WiseLib.h"
#include "../../../androidConcepts/debugConcept/javaEssentials.h"
#include "../../../androidConcepts/debugConcept/javaEssentials.cpp"
#include "AndroidSend.h"
using namespace std;

void testSend(void)
{
	AndroidSend myAndroidSend;

}

/*
 * Class:     com_cbox_WiseLib
 * Method:    androidSend
 * Signature: (Ljava/lang/Object;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_cbox_WiseLib_androidSend
  (JNIEnv *env, jobject nc, jobject thiz)
{
	//setting the global java environment variables
	setJavaENV(env);
	setJavaObject(thiz);

	//testing the debug feature
	testSend();
	return 0;
}
