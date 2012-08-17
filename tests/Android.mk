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
# Copyright The Android Open Source Project

LOCAL_PATH := $(call my-dir)

test_module = bionic-unit-tests
test_tags = eng tests

test_src_files = \
    getcwd_test.cpp \
    regex_test.cpp \

# Build for the device (with bionic). Run with:
#   adb shell /data/nativetest/bionic-unit-tests/bionic-unit-tests
include $(CLEAR_VARS)
LOCAL_MODULE := $(test_module)
LOCAL_ADDITIONAL_DEPENDENCIES := $(LOCAL_PATH)/Android.mk
LOCAL_MODULE_TAGS := $(test_tags)
LOCAL_SRC_FILES := $(test_src_files)
include $(BUILD_NATIVE_TEST)

# Build for the host (with glibc).
# Note that this will build against glibc, so it's not useful for testing
# bionic's implementation, but it does let you use glibc as a reference
# implementation for testing the tests themselves.
include $(CLEAR_VARS)
LOCAL_MODULE := $(test_module)-glibc
LOCAL_ADDITIONAL_DEPENDENCIES := $(LOCAL_PATH)/Android.mk
LOCAL_MODULE_TAGS := $(test_tags)
LOCAL_SRC_FILES := $(test_src_files)
include $(BUILD_HOST_NATIVE_TEST)
