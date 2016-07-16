ifeq (true,$(FORCE_BUILD_LLVM_COMPONENTS))

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := versioner
LOCAL_MODULE_HOST_OS := linux

LOCAL_CLANG := true
LOCAL_CFLAGS := -Wall -Wextra -Werror -Wno-unused-parameter
LOCAL_CFLAGS += -D__STDC_CONSTANT_MACROS -D__STDC_LIMIT_MACROS
LOCAL_CPPFLAGS := $(LOCAL_CFLAGS) -std=c++14 -fno-rtti

LOCAL_SRC_FILES := \
  src/versioner.cpp \
  src/Arch.cpp \
  src/DeclarationDatabase.cpp \
  src/SymbolDatabase.cpp \
  src/Utils.cpp

LOCAL_SHARED_LIBRARIES := libclang libLLVM

include $(BUILD_HOST_EXECUTABLE)

endif
