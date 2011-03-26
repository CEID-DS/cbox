# build file written to describe the C and C++ source files to the Android NDK


LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := calcsFromJNI
LOCAL_SRC_FILES := calcsFromJNI.cpp

include $(BUILD_SHARED_LIBRARY)



#include $(CLEAR_VARS)

#LOCAL_MODULE    := testStack
#LOCAL_SRC_FILES := testStack.cpp

#include $(BUILD_SHARED_LIBRARY)





