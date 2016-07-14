#
# Copyright (C) 2015 The Android Open Source Project
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

ifeq ($(HOST_OS),linux)

# $(1) library name
define copy-test-library
include $(CLEAR_VARS)
LOCAL_IS_HOST_MODULE := true
LOCAL_MODULE := $(1)
LOCAL_MODULE_CLASS := SHARED_LIBRARIES
LOCAL_MODULE_PATH := $(HOST_OUT_EXECUTABLES)
LOCAL_STRIP_MODULE := false
LOCAL_SRC_FILES := test_data/$(1)
include $(BUILD_PREBUILT)
endef

$(eval $(call copy-test-library,elf_file_unittest_relocs_arm32.so))
$(eval $(call copy-test-library,elf_file_unittest_relocs_arm32_packed.so))
$(eval $(call copy-test-library,elf_file_unittest_relocs_arm64.so))
$(eval $(call copy-test-library,elf_file_unittest_relocs_arm64_packed.so))
$(eval $(call copy-test-library,elf_file_unittest_relocs_ia32.so))
$(eval $(call copy-test-library,elf_file_unittest_relocs_ia32_packed.so))
$(eval $(call copy-test-library,elf_file_unittest_relocs_x64.so))
$(eval $(call copy-test-library,elf_file_unittest_relocs_x64_packed.so))
$(eval $(call copy-test-library,elf_file_unittest_relocs_mips32.so))
$(eval $(call copy-test-library,elf_file_unittest_relocs_mips32_packed.so))

endif
