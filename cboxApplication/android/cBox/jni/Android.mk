# build file written to describe the C and C++ source files to the Android NDK
LOCAL_PATH := $(call my-dir)
MY_CONCEPTS := ../../../../android_wiselib/androidConcepts




# first lib, which will be built statically
#
include $(CLEAR_VARS)
#BE CAREFUL HERE
LOCAL_PATH := $(LOCAL_PATH)/tinyXml
#BE CAREFUL HERE
LOCAL_MODULE    := tinyXml
LOCAL_SRC_FILES := tinystr.cpp tinyxml.cpp
LOCAL_SRC_FILES += tinyxmlerror.cpp tinyxmlparser.cpp
LOCAL_C_INCLUDES := $(LOCAL_PATH)
include $(BUILD_STATIC_LIBRARY)

LOCAL_PATH := $(LOCAL_PATH)/..
include $(CLEAR_VARS)
LOCAL_MODULE    := mainLib
LOCAL_SRC_FILES := androidMain.cpp
LOCAL_SRC_FILES += $(MY_CONCEPTS)/androidRadio/AndroidRadioModel.cpp
LOCAL_SRC_FILES += $(MY_CONCEPTS)/debugConcept/javaEssentials.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/headers/

LOCAL_STATIC_LIBRARIES := tinyXml
include $(BUILD_SHARED_LIBRARY)
