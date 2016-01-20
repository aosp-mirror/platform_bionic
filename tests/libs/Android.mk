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

common_cppflags :=
common_additional_dependencies := \
    $(LOCAL_PATH)/Android.mk \
    $(LOCAL_PATH)/Android.build.dt_runpath.mk \
    $(LOCAL_PATH)/Android.build.dlext_testzip.mk \
    $(LOCAL_PATH)/Android.build.dlopen_2_parents_reloc.mk \
    $(LOCAL_PATH)/Android.build.dlopen_check_order_dlsym.mk \
    $(LOCAL_PATH)/Android.build.dlopen_check_order_reloc_siblings.mk \
    $(LOCAL_PATH)/Android.build.dlopen_check_order_reloc_main_executable.mk \
    $(LOCAL_PATH)/Android.build.linker_namespaces.mk \
    $(LOCAL_PATH)/Android.build.pthread_atfork.mk \
    $(LOCAL_PATH)/Android.build.testlib.mk \
    $(LOCAL_PATH)/Android.build.versioned_lib.mk \
    $(TEST_PATH)/Android.build.mk

# -----------------------------------------------------------------------------
# Library to test gnu-styled hash
# -----------------------------------------------------------------------------
ifneq ($(TARGET_ARCH),$(filter $(TARGET_ARCH),mips mips64))
libgnu-hash-table-library_src_files := \
    dlext_test_library.cpp \

libgnu-hash-table-library_ldflags := \
    -Wl,--hash-style=gnu \

module := libgnu-hash-table-library
module_tag := optional
include $(LOCAL_PATH)/Android.build.testlib.mk
endif

# -----------------------------------------------------------------------------
# Library to test sysv-styled hash
# -----------------------------------------------------------------------------
libsysv-hash-table-library_src_files := \
    dlext_test_library.cpp \

libsysv-hash-table-library_ldflags := \
    -Wl,--hash-style=sysv \

module := libsysv-hash-table-library
module_tag := optional
include $(LOCAL_PATH)/Android.build.testlib.mk

# -----------------------------------------------------------------------------
# Library used by dlext tests - with GNU RELRO program header
# -----------------------------------------------------------------------------
libdlext_test_src_files := \
    dlext_test_library.cpp \

libdlext_test_ldflags := \
    -Wl,-z,relro \

libdlext_test_shared_libraries := libtest_simple

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

libdlext_test_norelro_shared_libraries := libtest_simple

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

libdlext_test_fd_shared_libraries := libtest_simple

libdlext_test_fd_install_to_out_data := true
module := libdlext_test_fd
module_tag := optional
build_type := target
build_target := SHARED_LIBRARY
include $(TEST_PATH)/Android.build.mk


# -----------------------------------------------------------------------------
# Libraries used by dlext tests for open from a zip-file
# -----------------------------------------------------------------------------
libdlext_test_zip_src_files := \
    dlext_test_library.cpp \

libdlext_test_zip_shared_libraries := libatest_simple_zip

libdlext_test_zip_install_to_out_data := true
module := libdlext_test_zip
module_tag := optional
build_type := target
build_target := SHARED_LIBRARY
include $(TEST_PATH)/Android.build.mk

libatest_simple_zip_src_files := \
    dlopen_testlib_simple.cpp

libatest_simple_zip_install_to_out_data := true
module := libatest_simple_zip
module_tag := optional
build_type := target
build_target := SHARED_LIBRARY
include $(TEST_PATH)/Android.build.mk

# ----------------------------------------------------------------------------
# Library with soname which does not match filename
# ----------------------------------------------------------------------------
libdlext_test_different_soname_src_files := \
    dlext_test_library.cpp \

module := libdlext_test_different_soname
module_tag := optional
libdlext_test_different_soname_ldflags := -Wl,-soname=libdlext_test_soname.so
include $(LOCAL_PATH)/Android.build.testlib.mk

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
# Library used by dlfcn nodelete tests
# -----------------------------------------------------------------------------
libtest_nodelete_1_src_files := \
    dlopen_nodelete_1.cpp

module := libtest_nodelete_1
include $(LOCAL_PATH)/Android.build.testlib.mk

# -----------------------------------------------------------------------------
# Library used by dlfcn nodelete tests
# -----------------------------------------------------------------------------
libtest_nodelete_2_src_files := \
    dlopen_nodelete_2.cpp

module := libtest_nodelete_2
include $(LOCAL_PATH)/Android.build.testlib.mk

# -----------------------------------------------------------------------------
# Library used by dlfcn nodelete tests
# -----------------------------------------------------------------------------
libtest_nodelete_dt_flags_1_src_files := \
    dlopen_nodelete_dt_flags_1.cpp

libtest_nodelete_dt_flags_1_ldflags := -Wl,-z,nodelete

module := libtest_nodelete_dt_flags_1
include $(LOCAL_PATH)/Android.build.testlib.mk

# -----------------------------------------------------------------------------
# Build test helper libraries for linker namespaces
# -----------------------------------------------------------------------------
include $(LOCAL_PATH)/Android.build.linker_namespaces.mk

# -----------------------------------------------------------------------------
# Build DT_RUNPATH test helper libraries
# -----------------------------------------------------------------------------
include $(LOCAL_PATH)/Android.build.dt_runpath.mk

# -----------------------------------------------------------------------------
# Build library with two parents
# -----------------------------------------------------------------------------
include $(LOCAL_PATH)/Android.build.dlopen_2_parents_reloc.mk

# -----------------------------------------------------------------------------
# Build libtest_check_order_dlsym.so with its dependencies.
# -----------------------------------------------------------------------------
include $(LOCAL_PATH)/Android.build.dlopen_check_order_dlsym.mk

# -----------------------------------------------------------------------------
# Build libtest_check_order_siblings.so with its dependencies.
# -----------------------------------------------------------------------------
include $(LOCAL_PATH)/Android.build.dlopen_check_order_reloc_siblings.mk

# -----------------------------------------------------------------------------
# Build libtest_check_order_root.so with its dependencies.
# -----------------------------------------------------------------------------
include $(LOCAL_PATH)/Android.build.dlopen_check_order_reloc_main_executable.mk

# -----------------------------------------------------------------------------
# Build libtest_versioned_lib.so with its dependencies.
# -----------------------------------------------------------------------------
include $(LOCAL_PATH)/Android.build.versioned_lib.mk

# -----------------------------------------------------------------------------
# Build libraries needed by pthread_atfork tests
# -----------------------------------------------------------------------------
include $(LOCAL_PATH)/Android.build.pthread_atfork.mk

# -----------------------------------------------------------------------------
# Library with dependency loop used by dlfcn tests
#
# libtest_with_dependency_loop -> a -> b -> c -> a
# -----------------------------------------------------------------------------
libtest_with_dependency_loop_src_files := dlopen_testlib_loopy_root.cpp

libtest_with_dependency_loop_shared_libraries := \
    libtest_with_dependency_loop_a

module := libtest_with_dependency_loop
include $(LOCAL_PATH)/Android.build.testlib.mk

# -----------------------------------------------------------------------------
# libtest_with_dependency_loop_a.so
# -----------------------------------------------------------------------------
libtest_with_dependency_loop_a_src_files := dlopen_testlib_loopy_a.cpp

libtest_with_dependency_loop_a_shared_libraries := \
    libtest_with_dependency_loop_b_tmp

module := libtest_with_dependency_loop_a
include $(LOCAL_PATH)/Android.build.testlib.mk

# -----------------------------------------------------------------------------
# libtest_with_dependency_loop_b.so
#
# this is temporary placeholder - will be removed
# -----------------------------------------------------------------------------
libtest_with_dependency_loop_b_tmp_src_files := dlopen_testlib_loopy_invalid.cpp
libtest_with_dependency_loop_b_tmp_ldflags := -Wl,-soname=libtest_with_dependency_loop_b.so

module := libtest_with_dependency_loop_b_tmp
include $(LOCAL_PATH)/Android.build.testlib.mk

# -----------------------------------------------------------------------------
# libtest_with_dependency_loop_b.so
# -----------------------------------------------------------------------------
libtest_with_dependency_loop_b_src_files := dlopen_testlib_loopy_b.cpp
libtest_with_dependency_loop_b_shared_libraries := libtest_with_dependency_loop_c

module := libtest_with_dependency_loop_b
include $(LOCAL_PATH)/Android.build.testlib.mk

# -----------------------------------------------------------------------------
# libtest_with_dependency_loop_c.so
# -----------------------------------------------------------------------------
libtest_with_dependency_loop_c_src_files := dlopen_testlib_loopy_c.cpp

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

ifeq ($(TARGET_ARCH),$(filter $(TARGET_ARCH),arm64 x86 x86_64))
    ifeq ($(TARGET_ARCH),arm64)
      libtest_ifunc_multilib := 64
      # TODO: This is a workaround - remove it once gcc
      # removes its Android ifunc checks
      libtest_ifunc_cflags := -mglibc
    endif

    build_type := target
    libtest_ifunc_clang_target := false
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
# This library is used by dl_load test to check symbol preempting
# by main executable
# -----------------------------------------------------------------------------
libdl_preempt_test_1_src_files := dl_preempt_library_1.cpp

module := libdl_preempt_test_1
include $(LOCAL_PATH)/Android.build.testlib.mk

# -----------------------------------------------------------------------------
# This library is used by dl_load test to check symbol preempting
# by libdl_preempt_test_1.so
# -----------------------------------------------------------------------------
libdl_preempt_test_2_src_files := dl_preempt_library_2.cpp

module := libdl_preempt_test_2
include $(LOCAL_PATH)/Android.build.testlib.mk

# -----------------------------------------------------------------------------
# Library with DF_1_GLOBAL
# -----------------------------------------------------------------------------
libdl_test_df_1_global_src_files := dl_df_1_global.cpp
libdl_test_df_1_global_ldflags := -Wl,-z,global

# TODO (dimitry): host ld.gold does not yet support -z global
# remove this line once it is updated.
libdl_test_df_1_global_ldflags_host := -fuse-ld=bfd

module := libdl_test_df_1_global
include $(LOCAL_PATH)/Android.build.testlib.mk

# -----------------------------------------------------------------------------
# Library using symbol from libdl_test_df_1_global
# -----------------------------------------------------------------------------
libtest_dlsym_df_1_global_src_files := dl_df_1_use_global.cpp
module := libtest_dlsym_df_1_global
include $(LOCAL_PATH)/Android.build.testlib.mk

# -----------------------------------------------------------------------------
# Library with weak function
# -----------------------------------------------------------------------------
libtest_dlsym_weak_func_src_files := \
    dlsym_weak_function.cpp

module := libtest_dlsym_weak_func
include $(LOCAL_PATH)/Android.build.testlib.mk

# -----------------------------------------------------------------------------
# Library to check RTLD_LOCAL with dlsym in 'this'
# -----------------------------------------------------------------------------
libtest_dlsym_from_this_src_files := dlsym_from_this_symbol.cpp

libtest_dlsym_from_this_shared_libraries_target := libdl
libtest_dlsym_from_this_shared_libraries := libtest_dlsym_from_this_child

module := libtest_dlsym_from_this
include $(LOCAL_PATH)/Android.build.testlib.mk

# -----------------------------------------------------------------------------
libtest_dlsym_from_this_child_src_files := dlsym_from_this_functions.cpp

libtest_dlsym_from_this_child_shared_libraries := libtest_dlsym_from_this_grandchild

module := libtest_dlsym_from_this_child
include $(LOCAL_PATH)/Android.build.testlib.mk

# -----------------------------------------------------------------------------
libtest_dlsym_from_this_grandchild_src_files := dlsym_from_this_symbol2.cpp

module := libtest_dlsym_from_this_grandchild
include $(LOCAL_PATH)/Android.build.testlib.mk

# -----------------------------------------------------------------------------
# Empty library
# -----------------------------------------------------------------------------
libtest_empty_src_files := empty.cpp

module := libtest_empty
include $(LOCAL_PATH)/Android.build.testlib.mk

# -----------------------------------------------------------------------------
# Library with weak undefined function
# -----------------------------------------------------------------------------
libtest_dlopen_weak_undefined_func_src_files := \
    dlopen_weak_undefined.cpp

module := libtest_dlopen_weak_undefined_func
include $(LOCAL_PATH)/Android.build.testlib.mk

# -----------------------------------------------------------------------------
# Library with constructor that calls dlopen() b/7941716
# -----------------------------------------------------------------------------
libtest_dlopen_from_ctor_src_files := \
   dlopen_testlib_dlopen_from_ctor.cpp

module := libtest_dlopen_from_ctor

libtest_dlopen_from_ctor_shared_libraries_target := libdl

include $(LOCAL_PATH)/Android.build.testlib.mk

# -----------------------------------------------------------------------------
# Library that depends on the library with constructor that calls dlopen() b/7941716
# -----------------------------------------------------------------------------

libtest_dlopen_from_ctor_main_src_files := empty.cpp
libtest_dlopen_from_ctor_main_shared_libraries := libtest_dlopen_from_ctor

module := libtest_dlopen_from_ctor_main
include $(LOCAL_PATH)/Android.build.testlib.mk

# -----------------------------------------------------------------------------
# Tool to use to align the shared libraries in a zip file.
# -----------------------------------------------------------------------------
include $(CLEAR_VARS)

LOCAL_SRC_FILES := bionic_tests_zipalign.cpp
LOCAL_MODULE := bionic_tests_zipalign
LOCAL_CFLAGS := -Wall -Werror

LOCAL_STATIC_LIBRARIES := libziparchive-host liblog libbase libz libutils

LOCAL_MODULE_HOST_OS := darwin linux windows

include $(BUILD_HOST_EXECUTABLE)
