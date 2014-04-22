#
# Copyright (C) 2013 The Android Open Source Project
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

# -----------------------------------------------------------------------------
# Benchmarks.
# -----------------------------------------------------------------------------

benchmark_c_flags = \
    -O2 \
    -Wall -Wextra \
    -Werror \
    -fno-builtin \
    -std=gnu++11 \

benchmark_src_files = \
    benchmark_main.cpp \
    math_benchmark.cpp \
    property_benchmark.cpp \
    pthread_benchmark.cpp \
    semaphore_benchmark.cpp \
    stdio_benchmark.cpp \
    string_benchmark.cpp \
    time_benchmark.cpp \
    unistd_benchmark.cpp \

# Build benchmarks for the device (with bionic's .so). Run with:
#   adb shell bionic-benchmarks
include $(CLEAR_VARS)
LOCAL_MODULE := bionic-benchmarks
LOCAL_MODULE_STEM_32 := bionic-benchmarks32
LOCAL_MODULE_STEM_64 := bionic-benchmarks64
LOCAL_MULTILIB := both
LOCAL_ADDITIONAL_DEPENDENCIES := $(LOCAL_PATH)/Android.mk
LOCAL_CFLAGS += $(benchmark_c_flags)
LOCAL_C_INCLUDES += external/stlport/stlport bionic/ bionic/libstdc++/include
LOCAL_SHARED_LIBRARIES += libstlport
LOCAL_SRC_FILES := $(benchmark_src_files)
include $(BUILD_EXECUTABLE)

endif # !BUILD_TINY_ANDROID
