# build file written to describe the C and C++ source files to the Android NDK

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)



LOCAL_MODULE    := debugTest
LOCAL_SRC_FILES := debugTest.cpp
LOCAL_SRC_FILES += ../../javaEssentials.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)
include $(BUILD_SHARED_LIBRARY)


