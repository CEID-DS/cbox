#include "com_cbox_WiseLib.h"
#include <jni.h>

void wiselib(void)
{

}

JNIEXPORT jint JNICALL Java_com_cbox_WiseLib_add
  (JNIEnv * env, jobject obj, jint value1, jint value2) {
		return (value1 + value2);
}
