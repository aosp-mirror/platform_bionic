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
build_type := target
build_target := SHARED_LIBRARY
include $(TEST_PATH)/Android.build.mk
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
build_type := target
build_target := SHARED_LIBRARY
include $(TEST_PATH)/Android.build.mk

# -----------------------------------------------------------------------------
# create symlink to libdlext_test.so for symlink test
# -----------------------------------------------------------------------------
# Use = instead of := to defer the evaluation of $@
$(LOCAL_INSTALLED_MODULE): PRIVATE_POST_INSTALL_CMD = \
    $(hide) cd $(dir $@) && ln -sf $(notdir $@) libdlext_test_v2.so

ifneq ($(TARGET_2ND_ARCH),)
# link 64 bit .so
$(TARGET_OUT)/lib64/libdlext_test.so: PRIVATE_POST_INSTALL_CMD = \
    $(hide) cd $(dir $@) && ln -sf $(notdir $@) libdlext_test_v2.so
endif

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
# Library used by dlfcn tests
# -----------------------------------------------------------------------------
libtest_simple_src_files := \
    dlopen_testlib_simple.cpp

module := libtest_simple
build_type := target
build_target := SHARED_LIBRARY
include $(TEST_PATH)/Android.build.mk

# -----------------------------------------------------------------------------
# Library with dependency used by dlfcn tests
# -----------------------------------------------------------------------------
libtest_with_dependency_src_files := \
    dlopen_testlib_simple.cpp

libtest_with_dependency_shared_libraries := libdlext_test

module := libtest_with_dependency
build_type := target
build_target := SHARED_LIBRARY
include $(TEST_PATH)/Android.build.mk

# -----------------------------------------------------------------------------
# Library used by atexit tests
# -----------------------------------------------------------------------------

libtest_atexit_src_files := \
    atexit_testlib.cpp

module := libtest_atexit
build_target := SHARED_LIBRARY
build_type := target
include $(TEST_PATH)/Android.build.mk
build_type := host
include $(TEST_PATH)/Android.build.mk

# -----------------------------------------------------------------------------
# Library with weak function
# -----------------------------------------------------------------------------
libtest_dlsym_weak_func_src_files := \
    dlsym_weak_function.cpp

module := libtest_dlsym_weak_func
build_target := SHARED_LIBRARY
build_type := target
include $(TEST_PATH)/Android.build.mk
build_type := host
include $(TEST_PATH)/Android.build.mk
