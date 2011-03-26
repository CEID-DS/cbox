#include <iostream>
#include <cmath>
using std::cout;
using std::endl;

#include <jni.h>
#include "calcsFromJNI.h" // calc class template definition
#include "ceid_ds2_cbox_templateAndroid_CalcNativeClass.h"



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
 */
JNIEXPORT jdouble JNICALL Java_ceid_1ds2_cbox_templateAndroid_CalcNativeClass_playWithTemplateJNI
  (JNIEnv *env, jobject thiz, jint in1, jint in2, jdouble d1, jdouble d2)
{
	return callFunction(in1,in2,d1,d2);
}


