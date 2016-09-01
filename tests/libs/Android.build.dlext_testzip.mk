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

BIONIC_TESTS_ZIPALIGN := $(HOST_OUT_EXECUTABLES)/bionic_tests_zipalign

include $(CLEAR_VARS)

LOCAL_MODULE_CLASS := SHARED_LIBRARIES
LOCAL_MODULE := libdlext_test_zip_zipaligned
LOCAL_MODULE_SUFFIX := .zip
LOCAL_MODULE_TAGS := tests
LOCAL_MODULE_PATH := $($(bionic_2nd_arch_prefix)TARGET_OUT_DATA_NATIVE_TESTS)/bionic-loader-test-libs/libdlext_test_zip
LOCAL_2ND_ARCH_VAR_PREFIX := $(bionic_2nd_arch_prefix)

include $(BUILD_SYSTEM)/base_rules.mk

my_shared_libs := \
  $($(bionic_2nd_arch_prefix)TARGET_OUT_INTERMEDIATE_LIBRARIES)/libdlext_test_zip.so \
  $($(bionic_2nd_arch_prefix)TARGET_OUT_INTERMEDIATE_LIBRARIES)/libatest_simple_zip.so

$(LOCAL_BUILT_MODULE) : $(my_shared_libs) | $(BIONIC_TESTS_ZIPALIGN)
	@echo "Aligning zip: $@"
	$(hide) rm -rf $(dir $@) && mkdir -p $(dir $@)/libdir
	$(hide) cp $^ $(dir $@)/libdir
	$(hide) (cd $(dir $@) && touch empty_file.txt && zip -qrD0 $(notdir $@).unaligned empty_file.txt libdir/*.so)
	$(hide) $(BIONIC_TESTS_ZIPALIGN) 4096 $@.unaligned $@

include $(CLEAR_VARS)

LOCAL_MODULE_CLASS := SHARED_LIBRARIES
LOCAL_MODULE := libdlext_test_runpath_zip_zipaligned
LOCAL_MODULE_SUFFIX := .zip
LOCAL_MODULE_TAGS := tests
LOCAL_MODULE_PATH := $($(bionic_2nd_arch_prefix)TARGET_OUT_DATA_NATIVE_TESTS)/bionic-loader-test-libs/libdlext_test_runpath_zip
LOCAL_2ND_ARCH_VAR_PREFIX := $(bionic_2nd_arch_prefix)

include $(BUILD_SYSTEM)/base_rules.mk
my_shared_libs := \
  $($(bionic_2nd_arch_prefix)TARGET_OUT_INTERMEDIATE_LIBRARIES)/libtest_dt_runpath_d_zip.so \
  $($(bionic_2nd_arch_prefix)TARGET_OUT_INTERMEDIATE_LIBRARIES)/libtest_dt_runpath_b.so \
  $($(bionic_2nd_arch_prefix)TARGET_OUT_INTERMEDIATE_LIBRARIES)/libtest_dt_runpath_a.so \
  $($(bionic_2nd_arch_prefix)TARGET_OUT_INTERMEDIATE_LIBRARIES)/libtest_dt_runpath_c.so \
  $($(bionic_2nd_arch_prefix)TARGET_OUT_INTERMEDIATE_LIBRARIES)/libtest_dt_runpath_x.so


$(LOCAL_BUILT_MODULE) : PRIVATE_LIB_D := \
  $($(bionic_2nd_arch_prefix)TARGET_OUT_INTERMEDIATE_LIBRARIES)/libtest_dt_runpath_d_zip.so
$(LOCAL_BUILT_MODULE) : PRIVATE_LIB_A := \
  $($(bionic_2nd_arch_prefix)TARGET_OUT_INTERMEDIATE_LIBRARIES)/libtest_dt_runpath_a.so
$(LOCAL_BUILT_MODULE) : PRIVATE_LIB_B := \
  $($(bionic_2nd_arch_prefix)TARGET_OUT_INTERMEDIATE_LIBRARIES)/libtest_dt_runpath_b.so
$(LOCAL_BUILT_MODULE) : PRIVATE_LIB_C := \
  $($(bionic_2nd_arch_prefix)TARGET_OUT_INTERMEDIATE_LIBRARIES)/libtest_dt_runpath_c.so
$(LOCAL_BUILT_MODULE) : PRIVATE_LIB_X := \
  $($(bionic_2nd_arch_prefix)TARGET_OUT_INTERMEDIATE_LIBRARIES)/libtest_dt_runpath_x.so
$(LOCAL_BUILT_MODULE) : $(my_shared_libs) | $(BIONIC_TESTS_ZIPALIGN)
	@echo "Aligning zip: $@"
	$(hide) rm -rf $(dir $@) && mkdir -p $(dir $@)/libdir && \
    mkdir -p $(dir $@)/libdir/dt_runpath_a && mkdir -p $(dir $@)/libdir/dt_runpath_b_c_x
	$(hide) cp $(PRIVATE_LIB_D) $(dir $@)/libdir
	$(hide) cp $(PRIVATE_LIB_A) $(dir $@)/libdir/dt_runpath_a
	$(hide) cp $(PRIVATE_LIB_B) $(dir $@)/libdir/dt_runpath_b_c_x
	$(hide) cp $(PRIVATE_LIB_C) $(dir $@)/libdir/dt_runpath_b_c_x
	$(hide) cp $(PRIVATE_LIB_X) $(dir $@)/libdir/dt_runpath_b_c_x
	$(hide) (cd $(dir $@) && touch empty_file.txt && zip -qrD0 $(notdir $@).unaligned empty_file.txt libdir)
	$(hide) $(BIONIC_TESTS_ZIPALIGN) 4096 $@.unaligned $@

