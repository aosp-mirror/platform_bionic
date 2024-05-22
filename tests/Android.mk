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

ifeq ($(HOST_OS)-$(HOST_ARCH),$(filter $(HOST_OS)-$(HOST_ARCH),linux-x86 linux-x86_64))

# -----------------------------------------------------------------------------
# Compile time tests.
# -----------------------------------------------------------------------------

FORTIFY_LEVEL := 1
include $(LOCAL_PATH)/make_fortify_compile_test.mk

FORTIFY_LEVEL := 2
include $(LOCAL_PATH)/make_fortify_compile_test.mk

endif # linux-x86

include $(call first-makefiles-under,$(LOCAL_PATH))
