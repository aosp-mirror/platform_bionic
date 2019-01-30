#
# Copyright (C) 2014 The Android Open Source Project
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

include $(CLEAR_VARS)
LOCAL_ADDITIONAL_DEPENDENCIES := $(common_additional_dependencies)

LOCAL_MODULE := $(module)
LOCAL_MODULE_TAGS := $(module_tag)
ifeq ($(build_type),host)
  # Always make host multilib
  LOCAL_MULTILIB := both
endif

ifneq ($(findstring LIBRARY, $(build_target)),LIBRARY)
  LOCAL_MODULE_STEM_32 := $(module)32
  LOCAL_MODULE_STEM_64 := $(module)64
else
ifneq ($($(module)_install_to_native_tests_dir),)
  ifeq ($(build_type),host)
    native_tests_var := HOST_OUT_NATIVE_TESTS
  else
    native_tests_var := TARGET_OUT_DATA_NATIVE_TESTS
  endif

  ifneq ($($(module)_install_to_native_tests_dir_32),)
    tests_dir_32 := $($(module)_install_to_native_tests_dir_32)
  else
    tests_dir_32 := $($(module)_install_to_native_tests_dir)
  endif

  ifneq ($($(module)_install_to_native_tests_dir_64),)
    tests_dir_64 := $($(module)_install_to_native_tests_dir_64)
  else
    tests_dir_64 := $($(module)_install_to_native_tests_dir)
  endif

  LOCAL_MODULE_PATH_32 := $($(TARGET_2ND_ARCH_VAR_PREFIX)$(native_tests_var))/$(tests_dir_32)
  LOCAL_MODULE_PATH_64 := $($(native_tests_var))/$(tests_dir_64)
endif
endif

LOCAL_CLANG := $($(module)_clang_$(build_type))

ifneq ($($(module)_allow_asan),true)
  LOCAL_SANITIZE := never
endif

LOCAL_FORCE_STATIC_EXECUTABLE := $($(module)_force_static_executable)

LOCAL_ALLOW_UNDEFINED_SYMBOLS := $($(module)_allow_undefined_symbols)

ifneq ($($(module)_multilib),)
  LOCAL_MULTILIB := $($(module)_multilib)
endif

ifneq ($($(module)_relative_path),)
  LOCAL_MODULE_RELATIVE_PATH := $($(module)_relative_path)
endif

LOCAL_CFLAGS := \
    $(common_cflags) \
    $($(module)_cflags) \
    $($(module)_cflags_$(build_type)) \

LOCAL_CONLYFLAGS += \
    $(common_conlyflags) \
    $($(module)_conlyflags) \
    $($(module)_conlyflags_$(build_type)) \

LOCAL_CPPFLAGS += \
    $(common_cppflags) \
    $($(module)_cppflags) \
    $($(module)_cppflags_$(build_type)) \

LOCAL_C_INCLUDES := \
    $(common_c_includes) \
    $($(module)_c_includes) \
    $($(module)_c_includes_$(build_type)) \

LOCAL_SRC_FILES := \
    $($(module)_src_files) \
    $($(module)_src_files_$(build_type)) \

LOCAL_STATIC_LIBRARIES := \
    $($(module)_static_libraries) \
    $($(module)_static_libraries_$(build_type)) \

LOCAL_SHARED_LIBRARIES := \
    $($(module)_shared_libraries) \
    $($(module)_shared_libraries_$(build_type)) \

LOCAL_WHOLE_STATIC_LIBRARIES := \
    $($(module)_whole_static_libraries) \
    $($(module)_whole_static_libraries_$(build_type)) \

LOCAL_LDFLAGS := \
    $($(module)_ldflags) \
    $($(module)_ldflags_$(build_type)) \

LOCAL_LDLIBS := \
    $($(module)_ldlibs) \
    $($(module)_ldlibs_$(build_type)) \

LOCAL_CXX_STL := libc++_static

ifeq ($(build_type),target)
  include $(BUILD_$(build_target))
endif

ifeq ($(build_type),host)
  # Only build if host builds are supported.
  ifeq ($(build_host),true)
    include $(BUILD_HOST_$(build_target))
  endif
endif
