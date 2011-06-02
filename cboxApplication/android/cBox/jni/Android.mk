# build file written to describe the C and C++ source files to the Android NDK
LOCAL_PATH := $(call my-dir)
MY_CONCEPTS := ../../../../android_wiselib/androidConcepts
include $(CLEAR_VARS)

LOCAL_MODULE    := mainLib
LOCAL_SRC_FILES := androidMain.cpp
LOCAL_SRC_FILES += $(MY_CONCEPTS)/androidRadio/AndroidRadioModel.cpp
LOCAL_SRC_FILES += $(MY_CONCEPTS)/debugConcept/javaEssentials.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/headers/
include $(BUILD_SHARED_LIBRARY)
