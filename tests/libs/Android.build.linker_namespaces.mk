#
# Copyright (C) 2015 The Android Open Source Project
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
# This set of libraries are used to verify linker namespaces.
# -----------------------------------------------------------------------------

# -----------------------------------------------------------------------------
# Test cases
# 1. Check that private libraries loaded in different namespaces are
#    different. Check that dlsym does not confuse them.
# 2. Check that public libraries loaded in different namespaces are shared
#    between them.
# 3. Check that namespace sticks on dlopen
#
# Dependency tree (visibility)
# libnstest_root.so (this should be local to the namespace)
# +-> libnstest_public.so
# +-> libnstest_private.so
#
# libnstest_dlopened.so (library in private namespace dlopened from libnstest_root.so)
# -----------------------------------------------------------------------------
libnstest_root_src_files := namespaces_root.cpp
libnstest_root_shared_libraries := libnstest_public libnstest_private
libnstest_root_install_to_out_data_dir := private_namespace_libs
module := libnstest_root
include $(LOCAL_PATH)/Android.build.target.testlib.mk

libnstest_public_src_files := namespaces_public.cpp
module := libnstest_public
libnstest_public_install_to_out_data_dir := public_namespace_libs
include $(LOCAL_PATH)/Android.build.target.testlib.mk

libnstest_private_src_files := namespaces_private.cpp
libnstest_private_install_to_out_data_dir := private_namespace_libs
module := libnstest_private
include $(LOCAL_PATH)/Android.build.target.testlib.mk

libnstest_dlopened_src_files := namespaces_dlopened.cpp
libnstest_dlopened_install_to_out_data_dir := private_namespace_libs
module := libnstest_dlopened
include $(LOCAL_PATH)/Android.build.target.testlib.mk

# -----------------------------------------------------------------------------
# This set of libraries is to test isolated namespaces
#
# Isolated namespaces do not allow loading of the library outside of
# the search paths.
#
# This library cannot be loaded in isolated namespace because one of DT_NEEDED
# libraries is outside of the search paths.
#
# libnstest_root_not_isolated.so (DT_RUNPATH = $ORIGIN/../private_namespace_libs_external/)
# +-> libnstest_public.so
# +-> libnstest_private_external.so (located in $ORIGIN/../private_namespace_libs_external/)
#
# Search path: $NATIVE_TESTS/private_namespace_libs/
# -----------------------------------------------------------------------------
libnstest_root_not_isolated_src_files := namespaces_root.cpp
libnstest_root_not_isolated_shared_libraries := libnstest_public libnstest_private_external
libnstest_root_not_isolated_install_to_out_data_dir := private_namespace_libs
libnstest_root_not_isolated_ldflags := -Wl,--rpath,\$$ORIGIN/../private_namespace_libs_external \
                                       -Wl,--enable-new-dtags

module := libnstest_root_not_isolated
include $(LOCAL_PATH)/Android.build.target.testlib.mk

libnstest_private_external_src_files := namespaces_private.cpp
libnstest_private_external_install_to_out_data_dir := private_namespace_libs_external
module := libnstest_private_external
include $(LOCAL_PATH)/Android.build.target.testlib.mk
