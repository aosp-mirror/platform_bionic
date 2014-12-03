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
# Libraries used by dlfcn tests to verify local group ref_counting
# libtest_two_parents*.so
# -----------------------------------------------------------------------------

# -----------------------------------------------------------------------------
# ..._child.so - correct answer
# -----------------------------------------------------------------------------
libtest_two_parents_child_src_files := \
    dlopen_2_parents_reloc_answer.cpp

module := libtest_two_parents_child
include $(LOCAL_PATH)/Android.build.testlib.mk

# -----------------------------------------------------------------------------
# ..._parent1.so - correct answer
# -----------------------------------------------------------------------------
libtest_two_parents_parent1_src_files := \
    dlopen_check_order_reloc_answer_impl.cpp

libtest_two_parents_parent1_shared_libraries := libtest_two_parents_child
libtest_two_parents_parent1_cflags := -D__ANSWER=42
module := libtest_two_parents_parent1
include $(LOCAL_PATH)/Android.build.testlib.mk

# -----------------------------------------------------------------------------
# ..._parent2.so - incorrect answer
# -----------------------------------------------------------------------------
libtest_two_parents_parent2_src_files := \
    dlopen_check_order_reloc_answer_impl.cpp

libtest_two_parents_parent2_shared_libraries := libtest_two_parents_child
libtest_two_parents_parent2_cflags := -D__ANSWER=1
module := libtest_two_parents_parent2
include $(LOCAL_PATH)/Android.build.testlib.mk

