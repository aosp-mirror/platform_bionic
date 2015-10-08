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
# Libraries used by dt_runpath tests.
# -----------------------------------------------------------------------------

#
# Dependencies
#
# libtest_dt_runpath_d.so                       runpath: ${ORIGIN}/dt_runpath_b_c_x
# |-> dt_runpath_b_c_x/libtest_dt_runpath_b.so  runpath: ${ORIGIN}/../dt_runpath_a
# |   |-> dt_runpath_a/libtest_dt_runpath_a.so
# |-> dt_runpath_b_c_x/libtest_dt_runpath_c.so  runpath: ${ORIGIN}/invalid_dt_runpath
# |   |-> libtest_dt_runpath_a.so (soname)
#
# This one is used to test dlopen
# dt_runpath_b_c_x/libtest_dt_runpath_x.so
#

# A leaf library in a non-standard directory.
libtest_dt_runpath_a_src_files := \
    empty.cpp

libtest_dt_runpath_a_relative_path := dt_runpath_a
module := libtest_dt_runpath_a
include $(LOCAL_PATH)/Android.build.testlib.mk

# Depends on library A with a DT_RUNPATH
libtest_dt_runpath_b_src_files := \
    empty.cpp

libtest_dt_runpath_b_shared_libraries := libtest_dt_runpath_a
libtest_dt_runpath_b_ldflags := -Wl,--rpath,\$${ORIGIN}/../dt_runpath_a -Wl,--enable-new-dtags
libtest_dt_runpath_b_relative_path := dt_runpath_b_c_x
module := libtest_dt_runpath_b
include $(LOCAL_PATH)/Android.build.testlib.mk

# Depends on library A with an incorrect DT_RUNPATH. This does not matter
# because B is the first in the D (below) dependency order, and library A
# is already loaded using the correct DT_RUNPATH from library B.
libtest_dt_runpath_c_src_files := \
    empty.cpp

libtest_dt_runpath_c_shared_libraries := libtest_dt_runpath_a
libtest_dt_runpath_c_ldflags := -Wl,--rpath,\$${ORIGIN}/invalid_dt_runpath -Wl,--enable-new-dtags
libtest_dt_runpath_c_relative_path := dt_runpath_b_c_x
module := libtest_dt_runpath_c
include $(LOCAL_PATH)/Android.build.testlib.mk

# D depends on B and C with DT_RUNPATH.
libtest_dt_runpath_d_src_files := \
    dlopen_b.cpp

libtest_dt_runpath_d_shared_libraries := libtest_dt_runpath_b libtest_dt_runpath_c
libtest_dt_runpath_d_ldflags := -Wl,--rpath,\$${ORIGIN}/dt_runpath_b_c_x -Wl,--enable-new-dtags
module := libtest_dt_runpath_d
include $(LOCAL_PATH)/Android.build.testlib.mk

# D version for open-from-zip test with runpath
libtest_dt_runpath_d_zip_src_files := \
    dlopen_b.cpp

libtest_dt_runpath_d_zip_shared_libraries := libtest_dt_runpath_b libtest_dt_runpath_c
libtest_dt_runpath_d_zip_ldflags := -Wl,--rpath,\$${ORIGIN}/dt_runpath_b_c_x -Wl,--enable-new-dtags
libtest_dt_runpath_d_zip_install_to_out_data := true
module := libtest_dt_runpath_d_zip
module_tag := optional
build_type := target
build_target := SHARED_LIBRARY
include $(TEST_PATH)/Android.build.mk


# A leaf library in a directory library D has DT_RUNPATH for.
libtest_dt_runpath_x_src_files := \
    empty.cpp

libtest_dt_runpath_x_relative_path := dt_runpath_b_c_x
module := libtest_dt_runpath_x
include $(LOCAL_PATH)/Android.build.testlib.mk

