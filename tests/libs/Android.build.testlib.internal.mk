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

# 1. Install test libraries to $ANDROID_DATA/nativetests../bionic-loader-test-libs/
#    by default.
ifeq ($($(module)_relative_install_path),)
  $(module)_install_to_native_tests_dir := bionic-loader-test-libs
else
  $(module)_install_to_native_tests_dir := bionic-loader-test-libs/$($(module)_relative_install_path)
endif
# 2. Set dt_runpath to origin to resolve dependencies
$(module)_ldflags += -Wl,--rpath,\$${ORIGIN} -Wl,--enable-new-dtags

include $(TEST_PATH)/Android.build.mk
