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
#include <string>
#include "com_cbox_WiseLib.h"
#include "../../../../debugConcept/javaEssentials.h"
#include "../../../../debugConcept/AndrDebug.h"
#include "../../../AndroidRadioModel.h"
#include "testClass.h"
using namespace std;

AndroidRadioModel *myRadio;
testClass *newTest;

void testSend(void)
{
	myRadio->send("testingSend");

}
/*
void testReceive(const char *nativeString)
{
	myRadio->send(nativeString);
}
*/

/*
 * Class:     com_cbox_WiseLib
 * Method:    androidInitModel
 * Signature: (Ljava/lang/Object;)V
 */
JNIEXPORT void JNICALL Java_com_cbox_WiseLib_androidInitModel
  (JNIEnv *env, jobject nc, jobject thiz)
{
	myRadio = new AndroidRadioModel;
	newTest = new testClass;
	myRadio->enable_radio();
	myRadio->reg_recv_callback<testClass, &testClass::receive1>(newTest);

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
 * Signature: (Ljava/lang/Object;Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_cbox_WiseLib_androidReceive
  (JNIEnv *env, jobject nc, jobject thiz, jstring javaString)
{
	//setting the global java environment variables
	setJavaENV(env);
	setJavaObject(thiz);
	int size;

    //Get the native string from javaString
	//when receiving from jni a string
    const char *nativeString = env->GetStringUTFChars(javaString, 0);
    //size=(int)env->GetStringUTFLength(javaString);
	//size--;
    char *newNativeString = (char *) malloc(size*sizeof(char));

	strcpy(newNativeString,nativeString);
	//testReceive(nativeString);

    myRadio->receive_message(1,size,(void *) newNativeString);


    //RELEASING THE ALLOCATED MEMORY!!!
    env->ReleaseStringUTFChars(javaString, nativeString);
}
