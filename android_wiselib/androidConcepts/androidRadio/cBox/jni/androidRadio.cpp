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
#include "com_cbox_WiseLib.h"
#include "../../../debugConcept/javaEssentials.h"
#include "../../../debugConcept/AndrDebug.h"
#include "../../AndroidSend.h"
using namespace std;


void testSend(void)
{
	AndroidSend myAndroidSend;
	myAndroidSend.udpSend("testingSend");

}
void testReceive(void)
{
	AndrDebug testDebug;
	testDebug.debug("hell%d", 0);
}

/*
 * Class:     com_cbox_WiseLib
 * Method:    androidSend
 * Signature: (Ljava/lang/Object;)V
 */
JNIEXPORT void JNICALL Java_com_cbox_WiseLib_androidSend
  (JNIEnv *env, jobject nc, jobject thiz)
{
	//setting the global java environment variables
	setJavaENV(env);
	setJavaObject(thiz);

	//testing the debug feature
	testSend();

}


/*
 * Class:     com_cbox_WiseLib
 * Method:    androidReceive
 * Signature: (Ljava/lang/Object;)V
 */
JNIEXPORT void JNICALL Java_com_cbox_WiseLib_androidReceive
  (JNIEnv * env, jobject jc, jobject thiz)
{
	//setting the global java environment variables
	setJavaENV(env);
	setJavaObject(thiz);

	testReceive();

}
