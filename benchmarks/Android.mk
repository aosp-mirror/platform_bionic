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
    -Wall -Wextra -Wunused \
    -Werror \
    -fno-builtin \
    -std=gnu++11 \

benchmark_src_files = \
    benchmark_main.cpp \
    math_benchmark.cpp \
    pthread_benchmark.cpp \
    semaphore_benchmark.cpp \
    stdio_benchmark.cpp \
    string_benchmark.cpp \
    time_benchmark.cpp \
    unistd_benchmark.cpp \

# Build benchmarks for the device (with bionic's .so). Run with:
#   adb shell bionic-benchmarks32
#   adb shell bionic-benchmarks64
include $(CLEAR_VARS)
LOCAL_MODULE := bionic-benchmarks
LOCAL_MODULE_STEM_32 := bionic-benchmarks32
LOCAL_MODULE_STEM_64 := bionic-benchmarks64
LOCAL_MULTILIB := both
LOCAL_ADDITIONAL_DEPENDENCIES := $(LOCAL_PATH)/Android.mk
LOCAL_CFLAGS += $(benchmark_c_flags)
LOCAL_SRC_FILES := $(benchmark_src_files) property_benchmark.cpp
LOCAL_CXX_STL := libc++
include $(BUILD_EXECUTABLE)

# We don't build a static benchmark executable because it's not usually
# useful. If you're trying to run the current benchmarks on an older
# release, it's (so far at least) always because you want to measure the
# performance of the old release's libc, and a static benchmark isn't
# going to let you do that.

# Build benchmarks for the host (against glibc!). Run with:
include $(CLEAR_VARS)
LOCAL_MODULE := bionic-benchmarks-glibc
LOCAL_MODULE_STEM_32 := bionic-benchmarks-glibc32
LOCAL_MODULE_STEM_64 := bionic-benchmarks-glibc64
LOCAL_MULTILIB := both
LOCAL_ADDITIONAL_DEPENDENCIES := $(LOCAL_PATH)/Android.mk
LOCAL_CFLAGS += $(benchmark_c_flags)
LOCAL_LDFLAGS += -lrt
LOCAL_SRC_FILES := $(benchmark_src_files)
LOCAL_CXX_STL := libc++
include $(BUILD_HOST_EXECUTABLE)

ifeq ($(HOST_OS)-$(HOST_ARCH),$(filter $(HOST_OS)-$(HOST_ARCH),linux-x86 linux-x86_64))
ifeq ($(TARGET_ARCH),x86)
LINKER = linker
NATIVE_SUFFIX=32
else
LINKER = linker64
NATIVE_SUFFIX=64
endif

bionic-benchmarks-run-on-host: bionic-benchmarks $(TARGET_OUT_EXECUTABLES)/$(LINKER) $(TARGET_OUT_EXECUTABLES)/sh
	if [ ! -d /system ]; then \
	  echo "Attempting to create /system"; \
	  sudo mkdir -p -m 0777 /system; \
	fi
	mkdir -p $(TARGET_OUT_DATA)/local/tmp
	ln -fs `realpath $(TARGET_OUT)/bin` /system/
	ln -fs `realpath $(TARGET_OUT)/etc` /system/
	ANDROID_DATA=$(TARGET_OUT_DATA) \
	ANDROID_ROOT=$(TARGET_OUT) \
	LD_LIBRARY_PATH=$(TARGET_OUT_SHARED_LIBRARIES) \
		$(TARGET_OUT_EXECUTABLES)/bionic-benchmarks$(NATIVE_SUFFIX) $(BIONIC_BENCHMARKS_FLAGS)
endif # linux-x86

endif # !BUILD_TINY_ANDROID
