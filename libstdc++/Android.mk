LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES:= \
	src/one_time_construction.cpp \
	src/new.cpp \
	src/pure_virtual.cpp \
	src/typeinfo.cpp

LOCAL_MODULE:= libstdc++

LOCAL_SYSTEM_SHARED_LIBRARIES := libc

include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)

LOCAL_SRC_FILES:= \
	src/one_time_construction.cpp \
	src/new.cpp \
	src/pure_virtual.cpp \
	src/typeinfo.cpp

LOCAL_MODULE:= libstdc++

LOCAL_SYSTEM_SHARED_LIBRARIES := libc

include $(BUILD_STATIC_LIBRARY)
