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
#include "headers/com_cbox_MainLib.h"
#include "../../../../android_wiselib/androidConcepts/debugConcept/javaEssentials.h"
#include "../../../../android_wiselib/androidConcepts/debugConcept/AndrDebug.h"
#include "../../../../android_wiselib/androidConcepts/androidRadio/AndroidRadioModel.h"
#include "headers/testClass.h"
#include "headers/ZcDbBindings.h"
using namespace std;

AndroidRadioModel *myRadio;
testClass *newTest;

void testDatabase(void)
{
	AndrDebug test;
	test.debug("testing database connection");

	ZcDbBindings *db = new ZcDbBindings;
	//service struct that will hold the new entries we want to enter to service table
	struct ZcDbBindings::service s;

	s.hostname="user1@cbox";
	s.serviceType="liveStream";
	s.protocol="UDP";
	s.interface="wifi";
	s.port=7650;
	s.TXTDATA="streaming a live video";
	s.TTL=13;
	s.advertised=false;
	s.questioned=true;
	//adding the service
//	db->addService(s);
	s.protocol="TCP";
//	db->addMyService(s);
//	db->refreshTTL(15);
	db->removeService("ceid15@cbox.com", "IRC");
//	db->removeMyService("IRC");
//	db->addDnsRecord("ceid21@cbox.com","ethernet","192.168.0.1");
//	db->updateDnsRecord("ceid21@cbox.com", "ethernet", "192.168.0.12");
//	db->removeDnsRecord("ceid15@cbox.com", "WiFi");


}

/*
 * Class:     com_cbox_MainLib
 * Method:    initialize
 * Signature: (Ljava/lang/Object;)V
 */
JNIEXPORT void JNICALL Java_com_cbox_MainLib_initialize
  (JNIEnv *env, jobject nc, jobject thiz)
{
	//initialize here everything that routing and zeroconf needs
	/*
	myRadio = new AndroidRadioModel;
	newTest = new testClass;
	myRadio->enable_radio();
	myRadio->reg_recv_callback<testClass, &testClass::receive1>(newTest);
	*/

}

/*
 * Class:     com_cbox_MainLib
 * Method:    testMethod
 * Signature: (Ljava/lang/Object;)V
 */
JNIEXPORT void JNICALL Java_com_cbox_MainLib_testMethod
  (JNIEnv *env, jobject nc, jobject thiz)
{
	//setting the global java environment variables
	setJavaENV(env);
	setJavaObject(thiz);

	//testing something
	testDatabase();
}


/*
 * Class:     com_cbox_MainLib
 * Method:    androidReceive
 * Signature: (Ljava/lang/Object;Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_cbox_MainLib_androidReceive
  (JNIEnv *env, jobject nc, jobject thiz, jstring javaString)
{
	//setting the global java environment variables
	setJavaENV(env);
	setJavaObject(thiz);
	int size;
	/*
    //Get the native string from javaString
	//when receiving from jni a string
    const char *nativeString = env->GetStringUTFChars(javaString, 0);
    //size=(int)env->GetStringUTFLength(javaString);
    char *newNativeString = (char *) malloc(size*sizeof(char));

	strcpy(newNativeString,nativeString);

	//call every registered receiver
    myRadio->receive_message(1,size,(void *) newNativeString);
    AndrDebug test;
    test.debug("%d", size);


    //RELEASING THE ALLOCATED MEMORY!!!
    env->ReleaseStringUTFChars(javaString, nativeString);
    */
}
