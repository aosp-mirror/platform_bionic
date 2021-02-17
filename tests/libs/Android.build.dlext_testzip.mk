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

LOCAL_MODULE_CLASS := NATIVE_TESTS
LOCAL_MODULE := libdlext_test_zip_zipaligned
LOCAL_MODULE_SUFFIX := .zip
LOCAL_MODULE_PATH := $($(bionic_2nd_arch_prefix)TARGET_OUT_DATA_NATIVE_TESTS)/bionic-loader-test-libs/libdlext_test_zip
LOCAL_2ND_ARCH_VAR_PREFIX := $(bionic_2nd_arch_prefix)

include $(BUILD_SYSTEM)/base_rules.mk

my_shared_libs := \
  $(call intermediates-dir-for,SHARED_LIBRARIES,libdlext_test_zip,,,$(bionic_2nd_arch_prefix))/libdlext_test_zip.so \
  $(call intermediates-dir-for,SHARED_LIBRARIES,libatest_simple_zip,,,$(bionic_2nd_arch_prefix))/libatest_simple_zip.so \
  $(call intermediates-dir-for,NATIVE_TESTS,exec_linker_helper,,,$(bionic_2nd_arch_prefix))/exec_linker_helper \
  $(call intermediates-dir-for,SHARED_LIBRARIES,exec_linker_helper_lib,,,$(bionic_2nd_arch_prefix))/exec_linker_helper_lib.so

$(LOCAL_BUILT_MODULE): PRIVATE_SHARED_LIBS := $(my_shared_libs)
$(LOCAL_BUILT_MODULE): $(my_shared_libs) $(BIONIC_TESTS_ZIPALIGN)
	@echo "Aligning zip: $@"
	$(hide) rm -rf $@.unaligned $@ $(dir $@)/zipdir && mkdir -p $(dir $@)/zipdir/libdir
	$(hide) cp $(PRIVATE_SHARED_LIBS) $(dir $@)/zipdir/libdir
	$(hide) touch $(dir $@)/zipdir/empty_file.txt
	$(hide) (cd $(dir $@)/zipdir && zip -qrD0 ../$(notdir $@).unaligned .)
	$(hide) $(BIONIC_TESTS_ZIPALIGN) 4096 $@.unaligned $@

include $(CLEAR_VARS)

LOCAL_MODULE_CLASS := NATIVE_TESTS
LOCAL_MODULE := libdlext_test_runpath_zip_zipaligned
LOCAL_MODULE_SUFFIX := .zip
LOCAL_MODULE_PATH := $($(bionic_2nd_arch_prefix)TARGET_OUT_DATA_NATIVE_TESTS)/bionic-loader-test-libs/libdlext_test_runpath_zip
LOCAL_2ND_ARCH_VAR_PREFIX := $(bionic_2nd_arch_prefix)

include $(BUILD_SYSTEM)/base_rules.mk
lib_d := $(call intermediates-dir-for,SHARED_LIBRARIES,libtest_dt_runpath_d_zip,,,$(bionic_2nd_arch_prefix))/libtest_dt_runpath_d_zip.so
lib_a := $(call intermediates-dir-for,SHARED_LIBRARIES,libtest_dt_runpath_a,,,$(bionic_2nd_arch_prefix))/libtest_dt_runpath_a.so
lib_b := $(call intermediates-dir-for,SHARED_LIBRARIES,libtest_dt_runpath_b,,,$(bionic_2nd_arch_prefix))/libtest_dt_runpath_b.so
lib_c := $(call intermediates-dir-for,SHARED_LIBRARIES,libtest_dt_runpath_c,,,$(bionic_2nd_arch_prefix))/libtest_dt_runpath_c.so
lib_x := $(call intermediates-dir-for,SHARED_LIBRARIES,libtest_dt_runpath_x,,,$(bionic_2nd_arch_prefix))/libtest_dt_runpath_x.so
lib_y := $(call intermediates-dir-for,SHARED_LIBRARIES,libtest_dt_runpath_y,,,$(bionic_2nd_arch_prefix))/libtest_dt_runpath_y.so

$(LOCAL_BUILT_MODULE) : PRIVATE_LIB_D := $(lib_d)
$(LOCAL_BUILT_MODULE) : PRIVATE_LIB_A := $(lib_a)
$(LOCAL_BUILT_MODULE) : PRIVATE_LIB_B := $(lib_b)
$(LOCAL_BUILT_MODULE) : PRIVATE_LIB_C := $(lib_c)
$(LOCAL_BUILT_MODULE) : PRIVATE_LIB_X := $(lib_x)
$(LOCAL_BUILT_MODULE) : PRIVATE_LIB_Y := $(lib_y)
ifeq ($(TARGET_IS_64_BIT),true)
  ifeq ($(TARGET_TRANSLATE_2ND_ARCH),true)
    $(LOCAL_BUILT_MODULE) : PRIVATE_LIB_OR_LIB64 := $(if $(LOCAL_2ND_ARCH_VAR_PREFIX),lib/$(TARGET_2ND_ARCH),lib64)
  else
    $(LOCAL_BUILT_MODULE) : PRIVATE_LIB_OR_LIB64 := $(if $(LOCAL_2ND_ARCH_VAR_PREFIX),lib,lib64)
  endif
else
  ifeq ($(TARGET_TRANSLATE_2ND_ARCH),true)
    $(LOCAL_BUILT_MODULE) : PRIVATE_LIB_OR_LIB64 := $(if $(LOCAL_2ND_ARCH_VAR_PREFIX),lib/$(TARGET_2ND_ARCH),lib)
  else
    $(LOCAL_BUILT_MODULE) : PRIVATE_LIB_OR_LIB64 := lib
  endif
endif
$(LOCAL_BUILT_MODULE) : $(lib_d) $(lib_a) $(lib_b) $(lib_c) $(lib_x) $(lib_y) $(BIONIC_TESTS_ZIPALIGN)
	@echo "Aligning zip: $@"
	$(hide) rm -rf $@.unaligned $@ $(dir $@)/zipdir && mkdir -p $(dir $@)/zipdir/libdir && \
    mkdir -p $(dir $@)/zipdir/libdir/dt_runpath_a && mkdir -p $(dir $@)/zipdir/libdir/dt_runpath_b_c_x && \
    mkdir -p $(dir $@)/zipdir/libdir/dt_runpath_y/$(PRIVATE_LIB_OR_LIB64)
	$(hide) cp $(PRIVATE_LIB_D) $(dir $@)/zipdir/libdir
	$(hide) cp $(PRIVATE_LIB_A) $(dir $@)/zipdir/libdir/dt_runpath_a
	$(hide) cp $(PRIVATE_LIB_B) $(dir $@)/zipdir/libdir/dt_runpath_b_c_x
	$(hide) cp $(PRIVATE_LIB_C) $(dir $@)/zipdir/libdir/dt_runpath_b_c_x
	$(hide) cp $(PRIVATE_LIB_X) $(dir $@)/zipdir/libdir/dt_runpath_b_c_x
	$(hide) cp $(PRIVATE_LIB_Y) $(dir $@)/zipdir/libdir/dt_runpath_y/$(PRIVATE_LIB_OR_LIB64)
	$(hide) touch $(dir $@)/zipdir/empty_file.txt
	$(hide) (cd $(dir $@)/zipdir && zip -qrD0 ../$(notdir $@).unaligned .)
	$(hide) $(BIONIC_TESTS_ZIPALIGN) 4096 $@.unaligned $@

