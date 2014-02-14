LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES:= \
    debugger.cpp \
    dlfcn.cpp \
    linker.cpp \
    linker_environ.cpp \
    linker_phdr.cpp \
    rt.cpp \

LOCAL_SRC_FILES_arm     := arch/arm/begin.S
LOCAL_SRC_FILES_arm64   := arch/arm64/begin.S
LOCAL_SRC_FILES_x86     := arch/x86/begin.c
LOCAL_SRC_FILES_x86_64  := arch/x86_64/begin.S
LOCAL_SRC_FILES_mips    := arch/mips/begin.S

LOCAL_LDFLAGS := \
    -shared \
    -Wl,-Bsymbolic \
    -Wl,--exclude-libs,ALL \

LOCAL_CFLAGS += \
    -fno-stack-protector \
    -Wstrict-overflow=5 \
    -fvisibility=hidden \
    -Wall -Wextra -Werror \

LOCAL_CONLYFLAGS += \
    -std=gnu99 \

LOCAL_CPPFLAGS += \
    -std=gnu++11 \

# We need to access Bionic private headers in the linker.
LOCAL_CFLAGS += -I$(LOCAL_PATH)/../libc/

# we don't want crtbegin.o (because we have begin.o), so unset it
# just for this module
LOCAL_NO_CRT := true
# TODO: split out the asflags.
LOCAL_ASFLAGS := $(LOCAL_CFLAGS)

LOCAL_ADDITIONAL_DEPENDENCIES := $(LOCAL_PATH)/Android.mk $(LOCAL_PATH)/linker.mk $(LOCAL_PATH)/linker_executable.mk

LOCAL_STATIC_LIBRARIES := libc_nomalloc

LOCAL_FORCE_STATIC_EXECUTABLE := true # not necessary when not including BUILD_EXECUTABLE

LOCAL_2ND_ARCH_VAR_PREFIX := $(linker_2nd_arch_var_prefix)

ifeq ($(TARGET_IS_64_BIT)|$(linker_2nd_arch_var_prefix),true|)
LOCAL_MODULE := linker64
else
LOCAL_MODULE := linker
endif

include $(LOCAL_PATH)/linker_executable.mk
