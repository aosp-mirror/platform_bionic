LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
include $(CLEAR_TBLGEN_VARS)

# Only do this when Clang is available.
CLANG_ROOT_PATH := external/clang
ifneq ($(wildcard $(CLANG_ROOT_PATH)/clang.mk),)

LLVM_ROOT_PATH := external/llvm
include $(CLANG_ROOT_PATH)/clang.mk

LOCAL_MODULE := versioner
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_CLASS := EXECUTABLES

TBLGEN_TABLES := \
  AttrList.inc \
  AttrVisitor.inc \
  Attrs.inc \
  CommentCommandList.inc \
  DeclNodes.inc \
  DiagnosticCommonKinds.inc \
  StmtNodes.inc \

LOCAL_SRC_FILES := \
  versioner.cpp \
  Arch.cpp \
  DeclarationDatabase.cpp \
  Preprocessor.cpp \
  SymbolDatabase.cpp \
  Utils.cpp

LOCAL_SHARED_LIBRARIES := libclang libLLVM

include $(CLANG_HOST_BUILD_MK)
include $(CLANG_TBLGEN_RULES_MK)

# Set these after including the clang makefiles, to avoid getting CFLAGS from them.
LOCAL_CFLAGS := -Wall -Wextra -Werror -Wno-unused-parameter
LOCAL_CFLAGS += -D__STDC_CONSTANT_MACROS -D__STDC_LIMIT_MACROS
LOCAL_CPPFLAGS := -std=c++14 -fno-rtti

LOCAL_MODULE_HOST_OS := linux

include $(BUILD_HOST_EXECUTABLE)

endif
