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
