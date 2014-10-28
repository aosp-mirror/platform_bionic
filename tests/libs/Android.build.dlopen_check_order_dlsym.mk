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
# Libraries used by dlfcn tests to verify correct load order:
# libtest_check_order_2_right.so
# -----------------------------------------------------------------------------
libtest_check_order_dlsym_2_right_src_files := \
    dlopen_check_order_dlsym_answer.cpp

libtest_check_order_dlsym_2_right_cflags := -D__ANSWER=42
module := libtest_check_order_dlsym_2_right
include $(LOCAL_PATH)/Android.build.testlib.mk

# -----------------------------------------------------------------------------
# libtest_check_order_a.so
# -----------------------------------------------------------------------------
libtest_check_order_dlsym_a_src_files := \
    dlopen_check_order_dlsym_answer.cpp

libtest_check_order_dlsym_a_cflags := -D__ANSWER=1
module := libtest_check_order_dlsym_a
include $(LOCAL_PATH)/Android.build.testlib.mk

# -----------------------------------------------------------------------------
# libtest_check_order_b.so
# -----------------------------------------------------------------------------
libtest_check_order_dlsym_b_src_files := \
    dlopen_check_order_dlsym_answer.cpp

libtest_check_order_dlsym_b_cflags := -D__ANSWER=2 -D__ANSWER2=43
module := libtest_check_order_dlsym_b
include $(LOCAL_PATH)/Android.build.testlib.mk

# -----------------------------------------------------------------------------
# libtest_check_order_c.so
# -----------------------------------------------------------------------------
libtest_check_order_dlsym_3_c_src_files := \
    dlopen_check_order_dlsym_answer.cpp

libtest_check_order_dlsym_3_c_cflags := -D__ANSWER=3
module := libtest_check_order_dlsym_3_c
include $(LOCAL_PATH)/Android.build.testlib.mk

# -----------------------------------------------------------------------------
# libtest_check_order_d.so
# -----------------------------------------------------------------------------
libtest_check_order_dlsym_d_src_files := \
   dlopen_check_order_dlsym_answer.cpp

libtest_check_order_dlsym_d_shared_libraries := libtest_check_order_dlsym_b
libtest_check_order_dlsym_d_cflags := -D__ANSWER=4 -D__ANSWER2=4
module := libtest_check_order_dlsym_d
include $(LOCAL_PATH)/Android.build.testlib.mk

# -----------------------------------------------------------------------------
# libtest_check_order_left.so
# -----------------------------------------------------------------------------
libtest_check_order_dlsym_1_left_src_files := \
    empty.cpp

libtest_check_order_dlsym_1_left_shared_libraries := libtest_check_order_dlsym_a libtest_check_order_dlsym_b

module := libtest_check_order_dlsym_1_left
include $(LOCAL_PATH)/Android.build.testlib.mk

# -----------------------------------------------------------------------------
# libtest_check_order.so
# -----------------------------------------------------------------------------
libtest_check_order_dlsym_src_files := \
    empty.cpp

libtest_check_order_dlsym_shared_libraries := libtest_check_order_dlsym_1_left \
  libtest_check_order_dlsym_2_right libtest_check_order_dlsym_3_c

module := libtest_check_order_dlsym
include $(LOCAL_PATH)/Android.build.testlib.mk
