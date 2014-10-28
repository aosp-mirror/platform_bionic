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

# -----------------------------------------------------------------------------
# Libraries used by dlfcn tests to verify correct relocation order:
# libtest_check_order_reloc_root*.so
# -----------------------------------------------------------------------------


# -----------------------------------------------------------------------------
# ..._1.so - empty
# -----------------------------------------------------------------------------
libtest_check_order_reloc_root_1_src_files := \
    empty.cpp


module := libtest_check_order_reloc_root_1
include $(LOCAL_PATH)/Android.build.testlib.mk


# -----------------------------------------------------------------------------
# ..._2.so - this one has the incorrect answer
# -----------------------------------------------------------------------------
libtest_check_order_reloc_root_2_src_files := \
    dlopen_check_order_reloc_root_answer_impl.cpp

libtest_check_order_reloc_root_2_cflags := -D__ANSWER=2

module := libtest_check_order_reloc_root_2
include $(LOCAL_PATH)/Android.build.testlib.mk

# -----------------------------------------------------------------------------
# libtest_check_order_reloc_root.so <- implements get_answer3()
# -----------------------------------------------------------------------------
libtest_check_order_reloc_root_src_files := \
    dlopen_check_order_reloc_root_answer.cpp

libtest_check_order_reloc_root_shared_libraries := \
    libtest_check_order_reloc_root_1 \
    libtest_check_order_reloc_root_2

module := libtest_check_order_reloc_root
include $(LOCAL_PATH)/Android.build.testlib.mk
