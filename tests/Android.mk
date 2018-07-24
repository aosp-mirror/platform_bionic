#
# Copyright (C) 2012 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

LOCAL_PATH := $(call my-dir)

# TODO(dimitry): replace with define once https://android-review.googlesource.com/247466 is reverted
# https://github.com/google/kati/issues/83 is currently blocking it.

# Move prebuilt test elf-files to $(TARGET_OUT_NATIVE_TESTS)
bionic_tests_module := libtest_invalid-rw_load_segment.so
include $(LOCAL_PATH)/Android.build.prebuilt.mk

bionic_tests_module := libtest_invalid-unaligned_shdr_offset.so
include $(LOCAL_PATH)/Android.build.prebuilt.mk

bionic_tests_module := libtest_invalid-zero_shentsize.so
include $(LOCAL_PATH)/Android.build.prebuilt.mk

bionic_tests_module := libtest_invalid-zero_shstrndx.so
include $(LOCAL_PATH)/Android.build.prebuilt.mk

bionic_tests_module := libtest_invalid-empty_shdr_table.so
include $(LOCAL_PATH)/Android.build.prebuilt.mk

bionic_tests_module := libtest_invalid-zero_shdr_table_offset.so
include $(LOCAL_PATH)/Android.build.prebuilt.mk

bionic_tests_module := libtest_invalid-zero_shdr_table_content.so
include $(LOCAL_PATH)/Android.build.prebuilt.mk

bionic_tests_module := libtest_invalid-textrels.so
include $(LOCAL_PATH)/Android.build.prebuilt.mk

bionic_tests_module := libtest_invalid-textrels2.so
include $(LOCAL_PATH)/Android.build.prebuilt.mk

ifeq ($(HOST_OS)-$(HOST_ARCH),$(filter $(HOST_OS)-$(HOST_ARCH),linux-x86 linux-x86_64))
build_host := true
else
build_host := false
endif

ifeq ($(HOST_OS)-$(HOST_ARCH),$(filter $(HOST_OS)-$(HOST_ARCH),linux-x86 linux-x86_64))

# -----------------------------------------------------------------------------
# Compile time tests.
# -----------------------------------------------------------------------------

# Some of these are intentionally using = instead of := since we need access to
# some variables not initialtized until we're in the build system.

include $(CLEAR_VARS)
LOCAL_ADDITIONAL_DEPENDENCIES := \
    $(LOCAL_PATH)/Android.mk \
    $(LOCAL_PATH)/file-check-cxx \
    | $(HOST_OUT_EXECUTABLES)/FileCheck$(HOST_EXECUTABLE_SUFFIX) \

LOCAL_CXX := $(LOCAL_PATH)/file-check-cxx \
    $(HOST_OUT_EXECUTABLES)/FileCheck \
    $(LLVM_PREBUILTS_PATH)/clang++ \
    CLANG \

LOCAL_CLANG := true
LOCAL_MODULE := bionic-compile-time-tests-clang++
LOCAL_CPPFLAGS := -Wall -Werror
LOCAL_CPPFLAGS += -fno-color-diagnostics -ferror-limit=10000
LOCAL_SRC_FILES := fortify_filecheck_diagnostics_test.cpp
include $(BUILD_STATIC_LIBRARY)

endif # linux-x86

include $(call first-makefiles-under,$(LOCAL_PATH))
