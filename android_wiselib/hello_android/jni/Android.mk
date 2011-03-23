# build file written to describe the C and C++ source files to the Android NDK

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := helloFromJNI
LOCAL_SRC_FILES := helloFromJNI.cpp

include $(BUILD_SHARED_LIBRARY)
