#include <iostream>
#include <cmath>
using std::cout;
using std::endl;

#include <jni.h>
#include "calcsFromJNI.h" // calc class template definition
#include "ceid_ds2_cbox_templateAndroid_CalcNativeClass.h"



/* this method is a mathematical benchmarks
 * It does numerous calculations so that to
 * slow down the system more than a second.
 */
double Calculation(void)
{
	double x = 45.0 / (180.0 / M_PI);
	double result = 0.5;
	for (int i = 0; i < 100000; i++)
	{
		for(int j = 0; j < 100; j++)
		{
			result += pow(cos(x), 2.0);
			result -= pow(sin(x), 2.0);
			result = log(result);
			result = exp(result);
		}
	}
	return result;
}

/* A simple function that just tests C++ templates if they work under JNI
 * (actually, templates are nothing more than compiler declarations. After
 * the compiling, the compiler declares the appropriate types of classes/functions.
 * So they should work as they are.)
 *  */
double callFunction(int in1, int in2, double d1, double d2)
{
	double res1,res2;
	calc< int > intCalc;
	res1=intCalc.add(in1,in2);

	calc< double > doubleCalc;
	res2=doubleCalc.multiply(d1,d2);

	return res1*res2;
}

/*
 * Class:     ceid_ds2_cbox_templateAndroid_CalcNativeClass
 * Method:    calcsFromJNI
 * Signature: ()D
 *
 * This method runs the c++ benchmark and returns the result
 */
JNIEXPORT jdouble JNICALL Java_ceid_1ds2_cbox_templateAndroid_CalcNativeClass_calcsFromJNI
  (JNIEnv *env, jobject thiz)
{
	return Calculation();
}


/*
 * Class:     ceid_ds2_cbox_templateAndroid_CalcNativeClass
 * Method:    playWithTemplateJNI
 * Signature: (IIDD)D
 *
 * This method checks the c++ templates under JNI
 */
JNIEXPORT jdouble JNICALL Java_ceid_1ds2_cbox_templateAndroid_CalcNativeClass_playWithTemplateJNI
  (JNIEnv *env, jobject thiz, jint in1, jint in2, jdouble d1, jdouble d2)
{
	return callFunction(in1,in2,d1,d2);
}

/*
 * Class:     ceid_ds2_cbox_templateAndroid_CalcNativeClass
 * Method:    nativeVMPlay
 * Signature: (Ljava/lang/Object;)Ljava/lang/String;
 *
 * This method doubles the integer variable "test" of object thiz2 and runs the method
 * testNative of the same object.
 */
JNIEXPORT jstring JNICALL Java_ceid_1ds2_cbox_templateAndroid_CalcNativeClass_nativeVMPlay
  (JNIEnv *env, jobject thiz, jobject thiz1)
{
	//getting the class that represents the java objext thiz1
	jclass jcClass = env->GetObjectClass(thiz1);
	if(jcClass!=NULL)
	{
		//getting the id of variable test of type integer
		jfieldID testId = env->GetFieldID(jcClass, "test", "I");
		if(testId!=NULL)
		{
			// This way we can get and set the "test" field. Let's double it:
			jint testN = env->GetIntField(thiz1, testId);
			env->SetIntField(thiz1, testId, testN * 2);
		}
	}
	else return env->NewStringUTF("ERROR");

	//getting the id of method testNative which has no parameters and returns void
	jmethodID test = env->GetMethodID(jcClass, "testNative", "()V");
	//calling the method testNative
	if(test!=NULL)	env->CallVoidMethod(thiz1, test);
	return env->NewStringUTF("Native code doubled variable test!");
}


