#
# Copyright (C) 2016 The Android Open Source Project
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
LOCAL_MULTILIB := both
LOCAL_MODULE := $(bionic_tests_module)
LOCAL_MODULE_PATH_32 := $($(TARGET_2ND_ARCH_VAR_PREFIX)TARGET_OUT_DATA_NATIVE_TESTS)/bionic-loader-test-libs/prebuilt-elf-files
LOCAL_MODULE_PATH_64 := $(TARGET_OUT_DATA_NATIVE_TESTS)/bionic-loader-test-libs/prebuilt-elf-files
LOCAL_MODULE_CLASS := EXECUTABLES

LOCAL_SRC_FILES_arm := prebuilt-elf-files/arm/$(bionic_tests_module)
LOCAL_SRC_FILES_arm64 := prebuilt-elf-files/arm64/$(bionic_tests_module)
LOCAL_SRC_FILES_x86 := prebuilt-elf-files/x86/$(bionic_tests_module)
LOCAL_SRC_FILES_x86_64 := prebuilt-elf-files/x86_64/$(bionic_tests_module)
include $(BUILD_PREBUILT)
bionic-loader-test-libs-target: $(LOCAL_MODULE)
.PHONY: bionic-loader-test-libs-target
