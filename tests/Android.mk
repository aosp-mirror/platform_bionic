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

# -----------------------------------------------------------------------------
# Unit tests.
# -----------------------------------------------------------------------------

ifeq ($(HOST_OS)-$(HOST_ARCH),$(filter $(HOST_OS)-$(HOST_ARCH),linux-x86 linux-x86_64))
build_host := true
else
build_host := false
endif

common_additional_dependencies := $(LOCAL_PATH)/Android.mk $(LOCAL_PATH)/Android.build.mk

# -----------------------------------------------------------------------------
# All standard tests.
# -----------------------------------------------------------------------------
test_cflags = \
    -fstack-protector-all \
    -g \
    -Wall -Wextra -Wunused \
    -Werror \
    -fno-builtin \

test_cflags += -D__STDC_LIMIT_MACROS  # For glibc.

ifeq ($(MALLOC_IMPL),dlmalloc)
test_cflags += -DUSE_DLMALLOC
else
test_cflags += -DUSE_JEMALLOC
endif

test_cppflags = \
    -std=gnu++11 \

libBionicStandardTests_src_files := \
    arpa_inet_test.cpp \
    buffer_tests.cpp \
    complex_test.cpp \
    ctype_test.cpp \
    dirent_test.cpp \
    error_test.cpp \
    eventfd_test.cpp \
    fcntl_test.cpp \
    fenv_test.cpp \
    ftw_test.cpp \
    getauxval_test.cpp \
    getcwd_test.cpp \
    inttypes_test.cpp \
    libc_logging_test.cpp \
    libgen_test.cpp \
    locale_test.cpp \
    malloc_test.cpp \
    math_test.cpp \
    mntent_test.cpp \
    netdb_test.cpp \
    pthread_test.cpp \
    pty_test.cpp \
    regex_test.cpp \
    sched_test.cpp \
    search_test.cpp \
    semaphore_test.cpp \
    setjmp_test.cpp \
    signal_test.cpp \
    stack_protector_test.cpp \
    stack_unwinding_test.cpp \
    stdatomic_test.cpp \
    stdint_test.cpp \
    stdio_test.cpp \
    stdio_ext_test.cpp \
    stdlib_test.cpp \
    string_test.cpp \
    string_posix_strerror_r_test.cpp \
    strings_test.cpp \
    stubs_test.cpp \
    sstream_test.cpp \
    sys_epoll_test.cpp \
    sys_mman_test.cpp \
    sys_personality_test.cpp \
    sys_procfs_test.cpp \
    sys_resource_test.cpp \
    sys_select_test.cpp \
    sys_sendfile_test.cpp \
    sys_socket_test.cpp \
    sys_stat_test.cpp \
    sys_statvfs_test.cpp \
    sys_syscall_test.cpp \
    sys_sysinfo_test.cpp \
    sys_time_test.cpp \
    sys_types_test.cpp \
    sys_uio_test.cpp \
    sys_vfs_test.cpp \
    sys_xattr_test.cpp \
    system_properties_test.cpp \
    time_test.cpp \
    uchar_test.cpp \
    uniqueptr_test.cpp \
    unistd_test.cpp \
    utmp_test.cpp \
    wchar_test.cpp \

libBionicStandardTests_cflags := \
    $(test_cflags) \

ifeq ($(MALLOC_IMPL),dlmalloc)
  libBionicStandardTests_cflags += -DUSE_DLMALLOC
else
  libBionicStandardTests_cflags += -DUSE_JEMALLOC
endif

libBionicStandardTests_cppflags := \
    $(test_cppflags) \

libBionicStandardTests_c_includes := \
    bionic/libc \
    external/tinyxml2 \

libBionicStandardTests_static_libraries := \
    libbase \

libBionicStandardTests_ldlibs_host := \
    -lrt \

module := libBionicStandardTests
module_tag := optional
build_type := target
build_target := STATIC_TEST_LIBRARY
include $(LOCAL_PATH)/Android.build.mk
build_type := host
include $(LOCAL_PATH)/Android.build.mk

# -----------------------------------------------------------------------------
# Fortify tests.
# -----------------------------------------------------------------------------
$(foreach compiler,gcc clang, \
  $(foreach test,1 2, \
    $(eval fortify$(test)-tests-$(compiler)_cflags := \
      $(test_cflags) \
      -Wno-error \
      -U_FORTIFY_SOURCE \
      -D_FORTIFY_SOURCE=$(test) \
      -DTEST_NAME=Fortify$(test)_$(compiler)); \
    $(eval fortify$(test)-tests-$(compiler)_src_files := \
      fortify_test.cpp); \
    $(eval fortify_libs += fortify$(test)-tests-$(compiler)); \
  ) \
)

module := fortify1-tests-gcc
module_tag := optional
build_type := target
build_target := STATIC_TEST_LIBRARY
include $(LOCAL_PATH)/Android.build.mk
build_type := host
include $(LOCAL_PATH)/Android.build.mk

module := fortify2-tests-gcc
module_tag := optional
build_type := target
build_target := STATIC_TEST_LIBRARY
include $(LOCAL_PATH)/Android.build.mk
build_type := host
include $(LOCAL_PATH)/Android.build.mk

fortify1-tests-clang_clang_target := true
fortify1-tests-clang_cflags_host := -D__clang__

module := fortify1-tests-clang
module_tag := optional
build_type := target
build_target := STATIC_TEST_LIBRARY
include $(LOCAL_PATH)/Android.build.mk
build_type := host
include $(LOCAL_PATH)/Android.build.mk

fortify2-tests-clang_clang_target := true

fortify2-tests-clang_cflags_host := -D__clang__

module := fortify2-tests-clang
module_tag := optional
build_type := target
build_target := STATIC_TEST_LIBRARY
include $(LOCAL_PATH)/Android.build.mk
build_type := host
include $(LOCAL_PATH)/Android.build.mk

# -----------------------------------------------------------------------------
# Library of all tests (excluding the dynamic linker tests).
# -----------------------------------------------------------------------------
libBionicTests_whole_static_libraries := \
    libBionicStandardTests \
    $(fortify_libs) \

module := libBionicTests
module_tag := optional
build_type := target
build_target := STATIC_TEST_LIBRARY
include $(LOCAL_PATH)/Android.build.mk
build_type := host
include $(LOCAL_PATH)/Android.build.mk

# -----------------------------------------------------------------------------
# Library of bionic customized gtest main function, with simplified output format.
# -----------------------------------------------------------------------------
libBionicGtestMain_src_files := gtest_main.cpp

libBionicGtestMain_cflags := $(test_cflags)

libBionicGtestMain_cppflags := $(test_cppflags)

module := libBionicGtestMain
module_tag := optional
build_type := target
build_target := STATIC_TEST_LIBRARY
include $(LOCAL_PATH)/Android.build.mk
build_type := host
include $(LOCAL_PATH)/Android.build.mk

# -----------------------------------------------------------------------------
# Library of bionic customized gtest main function, with normal gtest output format,
# which is needed by bionic cts test.
# -----------------------------------------------------------------------------
libBionicCtsGtestMain_src_files := gtest_main.cpp

libBionicCtsGtestMain_cflags := $(test_cflags)

libBionicCtsGtestMain_cppflags := $(test_cppflags) -DUSING_GTEST_OUTPUT_FORMAT \

# Temporarily fix the job count to 1 for CTS since on some devices the
# number of online cores is incorrectly read as the total number of cores
# in the system. When b/24376925 is fixed, this should be removed.
libBionicCtsGtestMain_cppflags += -DJOB_COUNT_FIXED=1

module := libBionicCtsGtestMain
module_tag := optional
build_type := target
build_target := STATIC_TEST_LIBRARY
include $(LOCAL_PATH)/Android.build.mk
build_type := host
include $(LOCAL_PATH)/Android.build.mk

# -----------------------------------------------------------------------------
# Tests for the device using bionic's .so. Run with:
#   adb shell /data/nativetest/bionic-unit-tests/bionic-unit-tests32
#   adb shell /data/nativetest/bionic-unit-tests/bionic-unit-tests64
# -----------------------------------------------------------------------------
bionic-unit-tests_whole_static_libraries := \
    libBionicTests \
    libBionicGtestMain \

bionic-unit-tests_static_libraries := \
    libtinyxml2 \
    liblog \
    libbase \

# TODO: Include __cxa_thread_atexit_test.cpp to glibc tests once it is upgraded (glibc 2.18+)
bionic-unit-tests_src_files := \
    atexit_test.cpp \
    dl_test.cpp \
    dlext_test.cpp \
    __cxa_thread_atexit_test.cpp \
    dlfcn_test.cpp \
    libdl_test.cpp \
    pthread_dlfcn_test.cpp \

bionic-unit-tests_cflags := $(test_cflags)

bionic-unit-tests_conlyflags := \
    -fexceptions \
    -fnon-call-exceptions \

bionic-unit-tests_cppflags := $(test_cppflags)

bionic-unit-tests_ldflags := \
    -Wl,--export-dynamic

bionic-unit-tests_c_includes := \
    bionic/libc \
    $(call include-path-for, libpagemap) \

bionic-unit-tests_shared_libraries_target := \
    libdl \
    libpagemap \
    libdl_preempt_test_1 \
    libdl_preempt_test_2

# TODO: clang support for thread_local on arm is done via __aeabi_read_tp()
# which bionic does not support. Reenable this once this question is resolved.
bionic-unit-tests_clang_target := false

bionic-unit-tests_shared_libraries_target += libdl_test_df_1_global

module := bionic-unit-tests
module_tag := optional
build_type := target
build_target := NATIVE_TEST
include $(LOCAL_PATH)/Android.build.mk

# -----------------------------------------------------------------------------
# Tests for the device linked against bionic's static library. Run with:
#   adb shell /data/nativetest/bionic-unit-tests-static/bionic-unit-tests-static32
#   adb shell /data/nativetest/bionic-unit-tests-static/bionic-unit-tests-static64
# -----------------------------------------------------------------------------
bionic-unit-tests-static_whole_static_libraries := \
    libBionicTests \
    libBionicGtestMain \

bionic-unit-tests-static_static_libraries := \
    libm \
    libc \
    libc++_static \
    libdl \
    libtinyxml2 \
    liblog \
    libbase \

bionic-unit-tests-static_force_static_executable := true

# libc and libc++ both define std::nothrow. libc's is a private symbol, but this
# still causes issues when linking libc.a and libc++.a, since private isn't
# effective until it has been linked. To fix this, just allow multiple symbol
# definitions for the static tests.
bionic-unit-tests-static_ldflags := -Wl,--allow-multiple-definition

module := bionic-unit-tests-static
module_tag := optional
build_type := target
build_target := NATIVE_TEST
include $(LOCAL_PATH)/Android.build.mk

# -----------------------------------------------------------------------------
# Tests to run on the host and linked against glibc. Run with:
#   cd bionic/tests; mm bionic-unit-tests-glibc-run
# -----------------------------------------------------------------------------

ifeq ($(HOST_OS)-$(HOST_ARCH),$(filter $(HOST_OS)-$(HOST_ARCH),linux-x86 linux-x86_64))

bionic-unit-tests-glibc_src_files := \
    atexit_test.cpp \
    dlfcn_test.cpp \
    dl_test.cpp \
    pthread_dlfcn_test.cpp \

bionic-unit-tests-glibc_shared_libraries := \
    libdl_preempt_test_1 \
    libdl_preempt_test_2

bionic-unit-tests-glibc_shared_libraries += libdl_test_df_1_global

bionic-unit-tests-glibc_whole_static_libraries := \
    libBionicStandardTests \
    libBionicGtestMain \
    $(fortify_libs) \

bionic-unit-tests-glibc_static_libraries := \
    libbase \
    liblog \
    libcutils \

bionic-unit-tests-glibc_ldlibs := \
    -lrt -ldl -lutil \

bionic-unit-tests-glibc_c_includes := \
    bionic/libc \

bionic-unit-tests-glibc_cflags := $(test_cflags)
bionic-unit-tests-glibc_cppflags := $(test_cppflags)
bionic-unit-tests-glibc_ldflags := -Wl,--export-dynamic

bionic-unit-tests-glibc_allow_asan := true

module := bionic-unit-tests-glibc
module_tag := optional
build_type := host
build_target := NATIVE_TEST
include $(LOCAL_PATH)/Android.build.mk

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

LOCAL_CXX = $(LOCAL_PATH)/file-check-cxx \
    $(HOST_OUT_EXECUTABLES)/FileCheck \
    $($(LOCAL_2ND_ARCH_VAR_PREFIX)TARGET_CXX) \
    GCC \

LOCAL_CLANG := false
LOCAL_MODULE := bionic-compile-time-tests-g++
LOCAL_CPPFLAGS := -Wall
LOCAL_SRC_FILES := fortify_compilation_test.cpp
include $(BUILD_STATIC_LIBRARY)

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
LOCAL_CPPFLAGS := -Wall
# FileCheck will error if there aren't any CLANG: lines in the file, but there
# don't appear to be any cases where clang _does_ emit warnings for sn?printf :(
LOCAL_SRC_FILES :=
include $(BUILD_STATIC_LIBRARY)

# -----------------------------------------------------------------------------
# Host glibc tests.
# -----------------------------------------------------------------------------

# gtest needs ANDROID_DATA/local/tmp for death test output.
# Make sure to create ANDROID_DATA/local/tmp if doesn't exist.
# Use the current target out directory as ANDROID_DATA.
# BIONIC_TEST_FLAGS is either empty or it comes from the user.
bionic-unit-tests-glibc-run: bionic-unit-tests-glibc
	mkdir -p $(TARGET_OUT_DATA)/local/tmp
	ANDROID_DATA=$(TARGET_OUT_DATA) \
	ANDROID_ROOT=$(TARGET_OUT) \
		$(HOST_OUT_EXECUTABLES)/bionic-unit-tests-glibc64 $(BIONIC_TEST_FLAGS)

# -----------------------------------------------------------------------------
# Run the unit tests built against x86 bionic on an x86 host.
# -----------------------------------------------------------------------------

include $(LOCAL_PATH)/../build/run-on-host.mk

ifeq ($(TARGET_ARCH),$(filter $(TARGET_ARCH),x86 x86_64))

TEST_TIMEOUT := 0

# BIONIC_TEST_FLAGS is either empty or it comes from the user.
bionic-unit-tests-run-on-host32: bionic-unit-tests bionic-prepare-run-on-host
	ANDROID_DATA=$(TARGET_OUT_DATA) \
	ANDROID_DNS_MODE=local \
	ANDROID_ROOT=$(TARGET_OUT) \
		timeout $(TEST_TIMEOUT) \
		$(TARGET_OUT_DATA)/nativetest/bionic-unit-tests/bionic-unit-tests32 $(BIONIC_TEST_FLAGS)

ifeq ($(TARGET_IS_64_BIT),true)
# add target to run lp64 tests
bionic-unit-tests-run-on-host64: bionic-unit-tests bionic-prepare-run-on-host
	ANDROID_DATA=$(TARGET_OUT_DATA) \
	ANDROID_DNS_MODE=local \
	ANDROID_ROOT=$(TARGET_OUT) \
		timeout $(TEST_TIMEOUT) \
		$(TARGET_OUT_DATA)/nativetest64/bionic-unit-tests/bionic-unit-tests64 $(BIONIC_TEST_FLAGS)
endif

endif # x86 x86_64
endif # linux-x86

include $(call first-makefiles-under,$(LOCAL_PATH))
