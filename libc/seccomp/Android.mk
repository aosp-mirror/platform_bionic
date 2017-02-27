LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_ADDITIONAL_DEPENDENCIES := $(LOCAL_PATH)/Android.mk
LOCAL_MODULE := libseccomp_policy
LOCAL_CLANG := true
LOCAL_SRC_FILES := seccomp_policy.cpp arm_policy.cpp arm64_policy.cpp
LOCAL_EXPORT_C_INCLUDE_DIRS := $(LOCAL_PATH)
LOCAL_SHARED_LIBRARIES := libbase

include $(BUILD_STATIC_LIBRARY)

