# build file written to describe the C and C++ source files to the Android NDK
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := wiselib
LOCAL_SRC_FILES := androidRadio.cpp
LOCAL_SRC_FILES += ../../../AndroidRadioModel.cpp
LOCAL_SRC_FILES += ../../../../debugConcept/javaEssentials.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)
include $(BUILD_SHARED_LIBRARY)
