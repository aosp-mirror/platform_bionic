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

ifneq ($(BUILD_TINY_ANDROID), true)

LOCAL_PATH := $(call my-dir)

test_src_files = \
    getcwd_test.cpp \
    pthread_test.cpp \
    regex_test.cpp \
    string_test.cpp \
    stubs_test.cpp \

test_dynamic_ldflags = -Wl,--export-dynamic -Wl,-u,DlSymTestFunction
test_dynamic_src_files = \
    dlopen_test.cpp \

# Build for the device (with bionic's .so). Run with:
#   adb shell /data/nativetest/bionic-unit-tests/bionic-unit-tests
include $(CLEAR_VARS)
LOCAL_MODULE := bionic-unit-tests
LOCAL_ADDITIONAL_DEPENDENCIES := $(LOCAL_PATH)/Android.mk
LOCAL_LDFLAGS += $(test_dynamic_ldflags)
LOCAL_SHARED_LIBRARIES += libdl
LOCAL_SRC_FILES := $(test_src_files) $(test_dynamic_src_files)
include $(BUILD_NATIVE_TEST)

# Build for the device (with bionic's .a). Run with:
#   adb shell /data/nativetest/bionic-unit-tests-static/bionic-unit-tests-static
include $(CLEAR_VARS)
LOCAL_MODULE := bionic-unit-tests-static
LOCAL_ADDITIONAL_DEPENDENCIES := $(LOCAL_PATH)/Android.mk
LOCAL_FORCE_STATIC_EXECUTABLE := true
LOCAL_SRC_FILES := $(test_src_files)
LOCAL_STATIC_LIBRARIES += libstlport_static libstdc++ libm libc
include $(BUILD_NATIVE_TEST)

# Build for the host (with glibc).
# Note that this will build against glibc, so it's not useful for testing
# bionic's implementation, but it does let you use glibc as a reference
# implementation for testing the tests themselves.
ifeq ($(HOST_OS)-$(HOST_ARCH),linux-x86)
include $(CLEAR_VARS)
LOCAL_MODULE := bionic-unit-tests-glibc
LOCAL_ADDITIONAL_DEPENDENCIES := $(LOCAL_PATH)/Android.mk
LOCAL_LDFLAGS += -lpthread -ldl
LOCAL_LDFLAGS += $(test_dynamic_ldflags)
LOCAL_SRC_FILES := $(test_src_files) $(test_dynamic_src_files)
include $(BUILD_HOST_NATIVE_TEST)
endif

endif # !BUILD_TINY_ANDROID
