LOCAL_PATH:= $(call my-dir)

############################################
include $(CLEAR_VARS)
LOCAL_MODULE := zoneinfo.dat
LOCAL_ADDITIONAL_DEPENDENCIES := $(LOCAL_PATH)/Android.mk
LOCAL_SRC_FILES := $(LOCAL_MODULE)
LOCAL_MODULE_CLASS := ETC
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_PATH := $(TARGET_OUT)/usr/share/zoneinfo
include $(BUILD_PREBUILT)

############################################
include $(CLEAR_VARS)
LOCAL_MODULE := zoneinfo.idx
LOCAL_ADDITIONAL_DEPENDENCIES := $(LOCAL_PATH)/Android.mk
LOCAL_SRC_FILES := $(LOCAL_MODULE)
LOCAL_MODULE_CLASS := ETC
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_PATH := $(TARGET_OUT)/usr/share/zoneinfo
include $(BUILD_PREBUILT)

############################################
include $(CLEAR_VARS)
LOCAL_MODULE := zoneinfo.version
LOCAL_ADDITIONAL_DEPENDENCIES := $(LOCAL_PATH)/Android.mk
LOCAL_SRC_FILES := $(LOCAL_MODULE)
LOCAL_MODULE_CLASS := ETC
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_PATH := $(TARGET_OUT)/usr/share/zoneinfo
include $(BUILD_PREBUILT)


# The host build doesn't use bionic, but it does use bionic's zoneinfo data
ifeq ($(WITH_HOST_DALVIK),true)

############################################
include $(CLEAR_VARS)
LOCAL_MODULE := zoneinfo-host.dat
LOCAL_ADDITIONAL_DEPENDENCIES := $(LOCAL_PATH)/Android.mk
LOCAL_IS_HOST_MODULE := true
LOCAL_SRC_FILES := zoneinfo.dat
LOCAL_MODULE_CLASS := ETC
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_STEM := $(LOCAL_SRC_FILES)
LOCAL_MODULE_PATH := $(HOST_OUT)/usr/share/zoneinfo
include $(BUILD_PREBUILT)

############################################
include $(CLEAR_VARS)
LOCAL_MODULE := zoneinfo-host.idx
LOCAL_ADDITIONAL_DEPENDENCIES := $(LOCAL_PATH)/Android.mk
LOCAL_IS_HOST_MODULE := true
LOCAL_SRC_FILES := zoneinfo.idx
LOCAL_MODULE_CLASS := ETC
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_STEM := $(LOCAL_SRC_FILES)
LOCAL_MODULE_PATH := $(HOST_OUT)/usr/share/zoneinfo
include $(BUILD_PREBUILT)

############################################
include $(CLEAR_VARS)
LOCAL_MODULE := zoneinfo-host.version
LOCAL_ADDITIONAL_DEPENDENCIES := $(LOCAL_PATH)/Android.mk
LOCAL_IS_HOST_MODULE := true
LOCAL_SRC_FILES := zoneinfo.version
LOCAL_MODULE_CLASS := ETC
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_STEM := $(LOCAL_SRC_FILES)
LOCAL_MODULE_PATH := $(HOST_OUT)/usr/share/zoneinfo
include $(BUILD_PREBUILT)

endif
