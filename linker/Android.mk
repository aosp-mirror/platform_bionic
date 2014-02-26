LOCAL_PATH:= $(call my-dir)

linker_2nd_arch_var_prefix :=
include $(LOCAL_PATH)/linker.mk

ifdef TARGET_2ND_ARCH
linker_2nd_arch_var_prefix := $(TARGET_2ND_ARCH_VAR_PREFIX)
include $(LOCAL_PATH)/linker.mk
endif
