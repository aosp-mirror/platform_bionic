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
    $(LOCAL_PATH)/Android.build.testlib.target.mk \
    $(LOCAL_PATH)/Android.build.versioned_lib.mk \
    $(TEST_PATH)/Android.build.mk

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
# Library used by dlext tests - different name non-default location
# -----------------------------------------------------------------------------
module := libdlext_test_fd

libdlext_test_fd_src_files := \
    dlext_test_library.cpp \

libdlext_test_fd_shared_libraries := libtest_simple

libdlext_test_fd_relative_install_path := $(module)

libdlext_test_fd_ldflags := -Wl,--rpath,\$${ORIGIN}/.. -Wl,--enable-new-dtags

module_tag := optional
include $(LOCAL_PATH)/Android.build.testlib.target.mk


# -----------------------------------------------------------------------------
# Libraries used by dlext tests for open from a zip-file
# -----------------------------------------------------------------------------
module := libdlext_test_zip

libdlext_test_zip_src_files := \
    dlext_test_library.cpp \

libdlext_test_zip_shared_libraries := libatest_simple_zip

libdlext_test_zip_relative_install_path := $(module)
module_tag := optional
include $(LOCAL_PATH)/Android.build.testlib.target.mk

module := libatest_simple_zip

libatest_simple_zip_src_files := \
    dlopen_testlib_simple.cpp

libatest_simple_zip_relative_install_path := $(module)
module_tag := optional
include $(LOCAL_PATH)/Android.build.testlib.target.mk

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
# Library with dependency used by dlfcn tests
# -----------------------------------------------------------------------------
libtest_with_dependency_src_files := \
    dlopen_testlib_simple.cpp

libtest_with_dependency_shared_libraries := libdlext_test

module := libtest_with_dependency
include $(LOCAL_PATH)/Android.build.testlib.mk
