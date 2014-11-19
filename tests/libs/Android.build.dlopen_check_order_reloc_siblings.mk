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
# Libraries used by dlfcn tests to verify correct relocation order:
# libtest_check_order_reloc_siblings*.so
# -----------------------------------------------------------------------------

# -----------------------------------------------------------------------------
# ..._1.so - empty
# -----------------------------------------------------------------------------
libtest_check_order_reloc_siblings_1_src_files := \
    empty.cpp

libtest_check_order_reloc_siblings_1_shared_libraries := \
    libtest_check_order_reloc_siblings_a \
    libtest_check_order_reloc_siblings_b

module := libtest_check_order_reloc_siblings_1
include $(LOCAL_PATH)/Android.build.testlib.mk


# -----------------------------------------------------------------------------
# ..._2.so - empty
# -----------------------------------------------------------------------------
libtest_check_order_reloc_siblings_2_src_files := \
    dlopen_check_order_reloc_grandchild_answer.cpp

libtest_check_order_reloc_siblings_2_shared_libraries := \
    libtest_check_order_reloc_siblings_c \
    libtest_check_order_reloc_siblings_d

libtest_check_order_reloc_siblings_2_allow_undefined_symbols := true
module := libtest_check_order_reloc_siblings_2
include $(LOCAL_PATH)/Android.build.testlib.mk

# -----------------------------------------------------------------------------
# ..._3.so - get_answer2();
# -----------------------------------------------------------------------------
libtest_check_order_reloc_siblings_3_src_files := \
    dlopen_check_order_reloc_nephew_answer.cpp

libtest_check_order_reloc_siblings_3_shared_libraries := \
    libtest_check_order_reloc_siblings_e \
    libtest_check_order_reloc_siblings_f

module := libtest_check_order_reloc_siblings_3
include $(LOCAL_PATH)/Android.build.testlib.mk

# -----------------------------------------------------------------------------
# ..._a.so <- correct impl
# -----------------------------------------------------------------------------
libtest_check_order_reloc_siblings_a_src_files := \
    dlopen_check_order_reloc_answer_impl.cpp

libtest_check_order_reloc_siblings_a_cflags := -D__ANSWER=42
module := libtest_check_order_reloc_siblings_a
include $(LOCAL_PATH)/Android.build.testlib.mk

# -----------------------------------------------------------------------------
# ..._b.so
# -----------------------------------------------------------------------------
libtest_check_order_reloc_siblings_b_src_files := \
    dlopen_check_order_reloc_answer_impl.cpp

libtest_check_order_reloc_siblings_b_cflags := -D__ANSWER=1
module := libtest_check_order_reloc_siblings_b
include $(LOCAL_PATH)/Android.build.testlib.mk

# -----------------------------------------------------------------------------
# ..._c.so
# -----------------------------------------------------------------------------
libtest_check_order_reloc_siblings_c_src_files := \
    dlopen_check_order_reloc_answer_impl.cpp

libtest_check_order_reloc_siblings_c_cflags := -D__ANSWER=2
libtest_check_order_reloc_siblings_c_shared_libraries := \
    libtest_check_order_reloc_siblings_c_1 \
    libtest_check_order_reloc_siblings_c_2

module := libtest_check_order_reloc_siblings_c
include $(LOCAL_PATH)/Android.build.testlib.mk

# -----------------------------------------------------------------------------
# ..._d.so
# -----------------------------------------------------------------------------
libtest_check_order_reloc_siblings_d_src_files := \
    dlopen_check_order_reloc_answer_impl.cpp

libtest_check_order_reloc_siblings_d_cflags := -D__ANSWER=3
module := libtest_check_order_reloc_siblings_d
include $(LOCAL_PATH)/Android.build.testlib.mk

# -----------------------------------------------------------------------------
# ..._e.so
# -----------------------------------------------------------------------------
libtest_check_order_reloc_siblings_e_src_files := \
    dlopen_check_order_reloc_answer_impl.cpp

libtest_check_order_reloc_siblings_e_cflags := -D__ANSWER=4
module := libtest_check_order_reloc_siblings_e
include $(LOCAL_PATH)/Android.build.testlib.mk

# -----------------------------------------------------------------------------
# ..._f.so <- get_answer()
# -----------------------------------------------------------------------------
libtest_check_order_reloc_siblings_f_src_files := \
    dlopen_check_order_reloc_answer.cpp

module := libtest_check_order_reloc_siblings_f
include $(LOCAL_PATH)/Android.build.testlib.mk

# -----------------------------------------------------------------------------
# ..._c_1.so
# -----------------------------------------------------------------------------
libtest_check_order_reloc_siblings_c_1_src_files := \
    dlopen_check_order_reloc_grandchild_answer_impl.cpp

libtest_check_order_reloc_siblings_c_1_cflags := -D__ANSWER=42
module := libtest_check_order_reloc_siblings_c_1
include $(LOCAL_PATH)/Android.build.testlib.mk

# -----------------------------------------------------------------------------
# ..._c_2.so
# -----------------------------------------------------------------------------
libtest_check_order_reloc_siblings_c_2_src_files := \
    dlopen_check_order_reloc_grandchild_answer_impl.cpp

libtest_check_order_reloc_siblings_c_2_cflags := -D__ANSWER=0
module := libtest_check_order_reloc_siblings_c_2
include $(LOCAL_PATH)/Android.build.testlib.mk

# -----------------------------------------------------------------------------
# libtest_check_order_reloc_siblings.so
# -----------------------------------------------------------------------------
libtest_check_order_reloc_siblings_src_files := \
    empty.cpp

libtest_check_order_reloc_siblings_shared_libraries := \
    libtest_check_order_reloc_siblings_1 \
    libtest_check_order_reloc_siblings_2 \
    libtest_check_order_reloc_siblings_3

module := libtest_check_order_reloc_siblings
include $(LOCAL_PATH)/Android.build.testlib.mk
