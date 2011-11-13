LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

# Here we give our module name and sourcefile(s)
LOCAL_CFLAGS += -Wno-write-strings -DENV_ANDROID
LOCAL_CPPFLAGS += -Wno-write-strings -fexceptions -DENV_ANDROID
LOCAL_LDFLAGS += -Wno-write-strings -DENV_ANDROID

LOCAL_C_INCLUDES += ../../../../../../include

#--------source file
MODULE_CXXSRCS := AddFunction.cpp

LOCAL_SRC_FILES := ${MODULE_CXXSRCS}
LOCAL_LDLIBS := ../../../../../../libs/armeabi/libstarlib.a

LOCAL_MODULE  := AddFunction

include $(BUILD_SHARED_LIBRARY)  