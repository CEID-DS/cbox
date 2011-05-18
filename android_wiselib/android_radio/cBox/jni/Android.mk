LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := wiselib
LOCAL_SRC_FILES := wiselib.cpp

include $(BUILD_SHARED_LIBRARY)

