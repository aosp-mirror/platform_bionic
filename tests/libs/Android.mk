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
TEST_PATH := $(LOCAL_PATH)/..

common_cppflags += -std=gnu++11
common_additional_dependencies := \
    $(LOCAL_PATH)/Android.mk \
    $(LOCAL_PATH)/Android.build.dlext_testzip.mk \
    $(LOCAL_PATH)/Android.build.testlib.mk \
    $(TEST_PATH)/Android.build.mk

# -----------------------------------------------------------------------------
# Library used by dlfcn tests.
# -----------------------------------------------------------------------------
ifneq ($(TARGET_ARCH),$(filter $(TARGET_ARCH),mips mips64))
no-elf-hash-table-library_src_files := \
    empty.cpp \

no-elf-hash-table-library_ldflags := \
    -Wl,--hash-style=gnu \

module := no-elf-hash-table-library
module_tag := optional
include $(LOCAL_PATH)/Android.build.testlib.mk
endif

# -----------------------------------------------------------------------------
# Library used by dlext tests - with GNU RELRO program header
# -----------------------------------------------------------------------------
libdlext_test_src_files := \
    dlext_test_library.cpp \

libdlext_test_ldflags := \
    -Wl,-z,relro \

module := libdlext_test
module_tag := optional
include $(LOCAL_PATH)/Android.build.testlib.mk

# -----------------------------------------------------------------------------
# create symlink to libdlext_test.so for symlink test
# -----------------------------------------------------------------------------
# Use = instead of := to defer the evaluation of $@
$(TARGET_OUT)/lib/libdlext_test.so: PRIVATE_POST_INSTALL_CMD = \
    $(hide) cd $(dir $@) && ln -sf $(notdir $@) libdlext_test_v2.so

ifneq ($(TARGET_2ND_ARCH),)
# link 64 bit .so
$(TARGET_OUT)/lib64/libdlext_test.so: PRIVATE_POST_INSTALL_CMD = \
    $(hide) cd $(dir $@) && ln -sf $(notdir $@) libdlext_test_v2.so
endif

# host symlinks
$(HOST_OUT)/lib64/libdlext_test.so: PRIVATE_POST_INSTALL_CMD = \
    $(hide) cd $(dir $@) && ln -sf $(notdir $@) libdlext_test_v2.so

$(HOST_OUT)/lib/libdlext_test.so: PRIVATE_POST_INSTALL_CMD = \
    $(hide) cd $(dir $@) && ln -sf $(notdir $@) libdlext_test_v2.so

# -----------------------------------------------------------------------------
# Library used by dlext tests - without GNU RELRO program header
# -----------------------------------------------------------------------------
libdlext_test_norelro_src_files := \
    dlext_test_library.cpp \

libdlext_test_norelro_ldflags := \
    -Wl,-z,norelro \

module := libdlext_test_norelro
module_tag := optional
build_type := target
build_target := SHARED_LIBRARY
include $(TEST_PATH)/Android.build.mk

# -----------------------------------------------------------------------------
# Library used by dlext tests - different name non-default location
# -----------------------------------------------------------------------------
libdlext_test_fd_src_files := \
    dlext_test_library.cpp \

libdlext_test_fd_install_to_out_data := true
module := libdlext_test_fd
module_tag := optional
build_type := target
build_target := SHARED_LIBRARY
include $(TEST_PATH)/Android.build.mk

# -----------------------------------------------------------------------------
# Library used by dlext tests - zipped and aligned
# -----------------------------------------------------------------------------
include $(CLEAR_VARS)
bionic_2nd_arch_prefix :=
include $(LOCAL_PATH)/Android.build.dlext_testzip.mk
ifneq ($(TARGET_2ND_ARCH),)
  bionic_2nd_arch_prefix := $(TARGET_2ND_ARCH_VAR_PREFIX)
  include $(LOCAL_PATH)/Android.build.dlext_testzip.mk
endif

# -----------------------------------------------------------------------------
# Library used by dlfcn tests
# -----------------------------------------------------------------------------
libtest_simple_src_files := \
    dlopen_testlib_simple.cpp

module := libtest_simple
include $(LOCAL_PATH)/Android.build.testlib.mk

# -----------------------------------------------------------------------------
# Libraries used by dlfcn tests to verify correct load order:
# libtest_check_order_2_right.so
# -----------------------------------------------------------------------------
libtest_check_order_2_right_src_files := \
    dlopen_testlib_answer.cpp

libtest_check_order_2_right_cflags := -D__ANSWER=42
module := libtest_check_order_2_right
include $(LOCAL_PATH)/Android.build.testlib.mk

# -----------------------------------------------------------------------------
# libtest_check_order_a.so
# -----------------------------------------------------------------------------
libtest_check_order_a_src_files := \
    dlopen_testlib_answer.cpp

libtest_check_order_a_cflags := -D__ANSWER=1
module := libtest_check_order_a
include $(LOCAL_PATH)/Android.build.testlib.mk

# -----------------------------------------------------------------------------
# libtest_check_order_b.so
# -----------------------------------------------------------------------------
libtest_check_order_b_src_files := \
    dlopen_testlib_answer.cpp

libtest_check_order_b_cflags := -D__ANSWER=2 -D__ANSWER2=43
module := libtest_check_order_b
include $(LOCAL_PATH)/Android.build.testlib.mk

# -----------------------------------------------------------------------------
# libtest_check_order_c.so
# -----------------------------------------------------------------------------
libtest_check_order_3_c_src_files := \
    dlopen_testlib_answer.cpp

libtest_check_order_3_c_cflags := -D__ANSWER=3
module := libtest_check_order_3_c
include $(LOCAL_PATH)/Android.build.testlib.mk

# -----------------------------------------------------------------------------
# libtest_check_order_d.so
# -----------------------------------------------------------------------------
libtest_check_order_d_src_files := \
   dlopen_testlib_answer.cpp

libtest_check_order_d_shared_libraries := libtest_check_order_b
libtest_check_order_d_cflags := -D__ANSWER=4 -D__ANSWER2=4
module := libtest_check_order_d
include $(LOCAL_PATH)/Android.build.testlib.mk

# -----------------------------------------------------------------------------
# libtest_check_order_left.so
# -----------------------------------------------------------------------------
libtest_check_order_1_left_src_files := \
    empty.cpp

libtest_check_order_1_left_shared_libraries := libtest_check_order_a libtest_check_order_b

module := libtest_check_order_1_left
include $(LOCAL_PATH)/Android.build.testlib.mk

# -----------------------------------------------------------------------------
# libtest_check_order.so
# -----------------------------------------------------------------------------
libtest_check_order_src_files := \
    empty.cpp

libtest_check_order_shared_libraries := libtest_check_order_1_left \
  libtest_check_order_2_right libtest_check_order_3_c

module := libtest_check_order
include $(LOCAL_PATH)/Android.build.testlib.mk

# -----------------------------------------------------------------------------
# Library with dependency loop used by dlfcn tests
#
# libtest_with_dependency_loop -> a -> b -> c -> a
# -----------------------------------------------------------------------------
libtest_with_dependency_loop_src_files := dlopen_testlib_invalid.cpp

libtest_with_dependency_loop_shared_libraries := \
    libtest_with_dependency_loop_a

module := libtest_with_dependency_loop
include $(LOCAL_PATH)/Android.build.testlib.mk

# -----------------------------------------------------------------------------
# libtest_with_dependency_loop_a.so
# -----------------------------------------------------------------------------
libtest_with_dependency_loop_a_src_files := dlopen_testlib_invalid.cpp

libtest_with_dependency_loop_a_shared_libraries := \
    libtest_with_dependency_loop_b_tmp

module := libtest_with_dependency_loop_a
include $(LOCAL_PATH)/Android.build.testlib.mk

# -----------------------------------------------------------------------------
# libtest_with_dependency_loop_b.so
#
# this is temporary placeholder - will be removed
# -----------------------------------------------------------------------------
libtest_with_dependency_loop_b_tmp_src_files := dlopen_testlib_invalid.cpp
libtest_with_dependency_loop_b_tmp_ldflags := -Wl,-soname=libtest_with_dependency_loop_b.so

module := libtest_with_dependency_loop_b_tmp
include $(LOCAL_PATH)/Android.build.testlib.mk

# -----------------------------------------------------------------------------
# libtest_with_dependency_loop_b.so
# -----------------------------------------------------------------------------
libtest_with_dependency_loop_b_src_files := dlopen_testlib_invalid.cpp
libtest_with_dependency_loop_b_shared_libraries := libtest_with_dependency_loop_c

module := libtest_with_dependency_loop_b
include $(LOCAL_PATH)/Android.build.testlib.mk

# -----------------------------------------------------------------------------
# libtest_with_dependency_loop_c.so
# -----------------------------------------------------------------------------
libtest_with_dependency_loop_c_src_files := dlopen_testlib_invalid.cpp

libtest_with_dependency_loop_c_shared_libraries := \
    libtest_with_dependency_loop_a

module := libtest_with_dependency_loop_c
include $(LOCAL_PATH)/Android.build.testlib.mk

# -----------------------------------------------------------------------------
# libtest_relo_check_dt_needed_order.so
# |
# +-> libtest_relo_check_dt_needed_order_1.so
# |
# +-> libtest_relo_check_dt_needed_order_2.so
# -----------------------------------------------------------------------------
libtest_relo_check_dt_needed_order_shared_libraries := \
    libtest_relo_check_dt_needed_order_1 libtest_relo_check_dt_needed_order_2

libtest_relo_check_dt_needed_order_src_files := dlopen_testlib_relo_check_dt_needed_order.cpp
libtest_relo_check_dt_needed_order_1_src_files := dlopen_testlib_relo_check_dt_needed_order_1.cpp
libtest_relo_check_dt_needed_order_2_src_files := dlopen_testlib_relo_check_dt_needed_order_2.cpp

module := libtest_relo_check_dt_needed_order
include $(LOCAL_PATH)/Android.build.testlib.mk
module := libtest_relo_check_dt_needed_order_1
include $(LOCAL_PATH)/Android.build.testlib.mk
module := libtest_relo_check_dt_needed_order_2
include $(LOCAL_PATH)/Android.build.testlib.mk

# -----------------------------------------------------------------------------
# Library with dependency used by dlfcn tests
# -----------------------------------------------------------------------------
libtest_with_dependency_src_files := \
    dlopen_testlib_simple.cpp

libtest_with_dependency_shared_libraries := libdlext_test

module := libtest_with_dependency
include $(LOCAL_PATH)/Android.build.testlib.mk

# -----------------------------------------------------------------------------
# Library used by ifunc tests
# -----------------------------------------------------------------------------
libtest_ifunc_src_files := \
    dlopen_testlib_ifunc.c

libtest_ifunc_clang_host := false
module := libtest_ifunc
build_target := SHARED_LIBRARY

build_type := host
include $(TEST_PATH)/Android.build.mk

ifeq ($(TARGET_ARCH),$(filter $(TARGET_ARCH),x86 x86_64))
    ifeq ($(TARGET_ARCH),arm64)
      libtest_ifunc_multilib := 64
      # TODO: This is a workaround - remove it once gcc
      # removes its Android ifunc checks
      libtest_ifunc_cflags := -mglibc
    endif

    build_type := target
    include $(TEST_PATH)/Android.build.mk
endif

# -----------------------------------------------------------------------------
# Library used by atexit tests
# -----------------------------------------------------------------------------

libtest_atexit_src_files := \
    atexit_testlib.cpp

module := libtest_atexit
include $(LOCAL_PATH)/Android.build.testlib.mk

# -----------------------------------------------------------------------------
# Library with weak function
# -----------------------------------------------------------------------------
libtest_dlsym_weak_func_src_files := \
    dlsym_weak_function.cpp

module := libtest_dlsym_weak_func
include $(LOCAL_PATH)/Android.build.testlib.mk
