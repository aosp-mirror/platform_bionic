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

# -----------------------------------------------------------------------------
# Library used by dlext tests - zipped and aligned
# -----------------------------------------------------------------------------

# TODO: It there simple way to do this?
$(bionic_2nd_arch_prefix)bionic_dlext_test_zip := \
    $($(bionic_2nd_arch_prefix)TARGET_OUT_INTERMEDIATES)/libdlext_test_fd/dlext_test_origin.zip
$(bionic_2nd_arch_prefix)bionic_dlext_test_zip_aligned := \
    $($(bionic_2nd_arch_prefix)TARGET_OUT_DATA_NATIVE_TESTS)/libdlext_test_fd/dlext_test.zip
ALL_MODULES += $($(bionic_2nd_arch_prefix)bionic_dlext_test_zip_aligned)

$(bionic_2nd_arch_prefix)bionic_dlext_built_shared_libraries := \
    $($(bionic_2nd_arch_prefix)TARGET_OUT_INTERMEDIATE_LIBRARIES)/libdlext_test_fd.so

bionic_dlext_test_zip_alignment := 4096 # PAGE_SIZE

$(bionic_2nd_arch_prefix)bionic_dlext_test_zip_tmpdir := $(dir $($(bionic_2nd_arch_prefix)bionic_dlext_test_zip))

$($(bionic_2nd_arch_prefix)bionic_dlext_test_zip)_prepare: $($(bionic_2nd_arch_prefix)bionic_dlext_built_shared_libraries)
	$(hide) mkdir -p $(dir $@)
	$(hide) cp -p $< $(dir $@)

$($(bionic_2nd_arch_prefix)bionic_dlext_test_zip): $($(bionic_2nd_arch_prefix)bionic_dlext_test_zip)_prepare
	@echo "Zip: $@"
	$(hide) (cd $(dir $@) && touch empty_file.txt && zip -rD0 $(notdir $@) empty_file.txt libdlext_test_fd.so)

$($(bionic_2nd_arch_prefix)bionic_dlext_test_zip_aligned): $($(bionic_2nd_arch_prefix)bionic_dlext_test_zip) | $(ZIPALIGN)
	$(hide) rm -rf $@
	$(hide) mkdir -p $(dir $@)
	@echo "Zipalign $(bionic_dlext_test_zip_alignment): $@"
	$(hide) zipalign $(bionic_dlext_test_zip_alignment) $< $@

