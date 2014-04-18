LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

include /Users/xiaohang/Documents/BTDownload/OpenCV-2.4.8-android-sdk/sdk/native/jni/OpenCV.mk

LOCAL_MODULE    := whichbkg
LOCAL_SRC_FILES := whichbkg.cpp

LOCAL_CPPFLAGS := -std=gnu++0x -Wall

include $(BUILD_EXECUTABLE) 
