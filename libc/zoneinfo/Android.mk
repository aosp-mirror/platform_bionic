LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

ALL_PREBUILT += $(TARGET_OUT)/usr/share/zoneinfo/zoneinfo.dat
$(TARGET_OUT)/usr/share/zoneinfo/zoneinfo.dat : $(LOCAL_PATH)/zoneinfo.dat | $(ACP)
	$(transform-prebuilt-to-target)

ALL_PREBUILT += $(TARGET_OUT)/usr/share/zoneinfo/zoneinfo.idx
$(TARGET_OUT)/usr/share/zoneinfo/zoneinfo.idx : $(LOCAL_PATH)/zoneinfo.idx | $(ACP)
	$(transform-prebuilt-to-target)

ALL_PREBUILT += $(TARGET_OUT)/usr/share/zoneinfo/zoneinfo.version
$(TARGET_OUT)/usr/share/zoneinfo/zoneinfo.version : $(LOCAL_PATH)/zoneinfo.version | $(ACP)
	$(transform-prebuilt-to-target)

# The host build doesn't use bionic, but it does use bionic's zoneinfo data
ifeq ($(WITH_HOST_DALVIK),true)
    ALL_PREBUILT += $(HOST_OUT)/usr/share/zoneinfo/zoneinfo.dat
    $(eval $(call copy-one-file,$(LOCAL_PATH)/zoneinfo.dat,$(HOST_OUT)/usr/share/zoneinfo/zoneinfo.dat))

    ALL_PREBUILT += $(HOST_OUT)/usr/share/zoneinfo/zoneinfo.idx
    $(eval $(call copy-one-file,$(LOCAL_PATH)/zoneinfo.idx,$(HOST_OUT)/usr/share/zoneinfo/zoneinfo.idx))

    ALL_PREBUILT += $(HOST_OUT)/usr/share/zoneinfo/zoneinfo.version
    $(eval $(call copy-one-file,$(LOCAL_PATH)/zoneinfo.version,$(HOST_OUT)/usr/share/zoneinfo/zoneinfo.version))
endif
