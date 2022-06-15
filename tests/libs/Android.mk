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
