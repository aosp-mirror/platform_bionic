include $(CLEAR_VARS)

LOCAL_ADDITIONAL_DEPENDENCIES := \
    $(LOCAL_PATH)/Android.mk \
    $(LOCAL_PATH)/touch-obj-on-success

LOCAL_CXX := $(LOCAL_PATH)/touch-obj-on-success \
    $(LLVM_PREBUILTS_PATH)/clang++ \

LOCAL_CLANG := true
LOCAL_MODULE := bionic-compile-time-tests$(FORTIFY_LEVEL)-clang++
LOCAL_CPPFLAGS := -Wall -Wno-error
LOCAL_CPPFLAGS += -fno-color-diagnostics -ferror-limit=10000 -Xclang -verify
LOCAL_CPPFLAGS += -DCOMPILATION_TESTS=1 -Wformat-nonliteral
LOCAL_CPPFLAGS += -U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=$(FORTIFY_LEVEL)
LOCAL_SRC_FILES := clang_fortify_tests.cpp

include $(BUILD_STATIC_LIBRARY)

FORTIFY_LEVEL :=
