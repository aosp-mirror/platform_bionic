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

# Include once
ifneq ($(bionic_run_on_host_mk_included),true)
bionic_run_on_host_mk_included:=true

ifneq ($(TARGET_ARCH),$(filter $(TARGET_ARCH),arm mips x86))
LINKER = linker64
else
LINKER = linker
endif

ifeq ($(TARGET_ARCH),$(filter $(TARGET_ARCH),x86 x86_64))
# gtest needs ANDROID_DATA/local/tmp for death test output.
# Make sure to create ANDROID_DATA/local/tmp if doesn't exist.
# bionic itself should always work relative to ANDROID_DATA or ANDROID_ROOT.
bionic-prepare-run-on-host: $(TARGET_OUT_EXECUTABLES)/$(LINKER) $(TARGET_OUT)/etc/hosts $(TARGET_OUT_EXECUTABLES)/sh
	if [ ! -d /system ]; then \
	  echo "Attempting to create /system"; \
	  sudo mkdir -p -m 0777 /system; \
	fi
	mkdir -p $(TARGET_OUT_DATA)/local/tmp
	ln -fs `realpath $(TARGET_OUT)/bin` /system/
	ln -fs `realpath $(TARGET_OUT)/etc` /system/
	ln -fs `realpath $(TARGET_OUT)/lib` /system/
	if [ -d "$(TARGET_OUT)/lib64" ]; then \
	  ln -fs `realpath $(TARGET_OUT)/lib64` /system/; \
	fi
endif
endif
