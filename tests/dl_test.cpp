/*
 * Copyright (C) 2012 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <gtest/gtest.h>

#include <dlfcn.h>
#include <libgen.h>
#include <limits.h>
#include <stdio.h>
#include <stdint.h>

#include <string>
#include <iostream>
#include <fstream>

#include "gtest_globals.h"
#include "utils.h"

extern "C" int main_global_default_serial() {
  return 3370318;
}

extern "C" int main_global_protected_serial() {
  return 2716057;
}

// The following functions are defined in DT_NEEDED
// libdl_preempt_test.so library.

// This one calls main_global_default_serial
extern "C" int main_global_default_get_serial();

// This one calls main_global_protected_serial
extern "C" int main_global_protected_get_serial();

// This one calls lib_global_default_serial
extern "C" int lib_global_default_get_serial();

// This one calls lib_global_protected_serial
extern "C" int lib_global_protected_get_serial();

// This test verifies that the global default function
// main_global_default_serial() is preempted by
// the function defined above.
TEST(dl, main_preempts_global_default) {
  ASSERT_EQ(3370318, main_global_default_get_serial());
}

// This one makes sure that the global protected
// symbols do not get preempted
TEST(dl, main_does_not_preempt_global_protected) {
  ASSERT_EQ(3370318, main_global_protected_get_serial());
}

// check same things for lib
TEST(dl, lib_preempts_global_default) {
  ASSERT_EQ(3370318, lib_global_default_get_serial());
}

TEST(dl, lib_does_not_preempt_global_protected) {
  ASSERT_EQ(3370318, lib_global_protected_get_serial());
}

TEST(dl, exec_linker) {
#if defined(__BIONIC__)
#if defined(__LP64__)
  static constexpr const char* kPathToLinker = "/system/bin/linker64";
#else
  static constexpr const char* kPathToLinker = "/system/bin/linker";
#endif
  ExecTestHelper eth;
  std::string expected_output = std::string("This is ") + kPathToLinker +
                                ", the helper program for dynamic executables.\n";
  eth.SetArgs( { kPathToLinker, nullptr });
  eth.Run([&]() { execve(kPathToLinker, eth.GetArgs(), eth.GetEnv()); }, 0, expected_output.c_str());
#endif
}

TEST(dl, preinit_system_calls) {
#if defined(__BIONIC__)
  std::string helper = get_testlib_root() +
      "/preinit_syscall_test_helper/preinit_syscall_test_helper";
  chmod(helper.c_str(), 0755); // TODO: "x" lost in CTS, b/34945607
  ExecTestHelper eth;
  eth.SetArgs({ helper.c_str(), nullptr });
  eth.Run([&]() { execve(helper.c_str(), eth.GetArgs(), eth.GetEnv()); }, 0, nullptr);
#endif
}

TEST(dl, xfail_preinit_getauxval) {
#if defined(__BIONIC__)
  std::string helper = get_testlib_root() +
      "/preinit_getauxval_test_helper/preinit_getauxval_test_helper";
  chmod(helper.c_str(), 0755); // TODO: "x" lost in CTS, b/34945607
  ExecTestHelper eth;
  eth.SetArgs({ helper.c_str(), nullptr });
  eth.Run([&]() { execve(helper.c_str(), eth.GetArgs(), eth.GetEnv()); }, 0, nullptr);
#endif
}


TEST(dl, exec_without_ld_preload) {
#if defined(__BIONIC__)
  std::string helper = get_testlib_root() +
      "/ld_preload_test_helper/ld_preload_test_helper";
  chmod(helper.c_str(), 0755);
  ExecTestHelper eth;
  eth.SetArgs({ helper.c_str(), nullptr });
  eth.Run([&]() { execve(helper.c_str(), eth.GetArgs(), eth.GetEnv()); }, 0, "12345");
#endif
}

TEST(dl, exec_with_ld_preload) {
#if defined(__BIONIC__)
  std::string helper = get_testlib_root() +
      "/ld_preload_test_helper/ld_preload_test_helper";
  std::string env = std::string("LD_PRELOAD=") + get_testlib_root() + "/ld_preload_test_helper_lib2.so";
  chmod(helper.c_str(), 0755);
  ExecTestHelper eth;
  eth.SetArgs({ helper.c_str(), nullptr });
  eth.SetEnv({ env.c_str(), nullptr });
  // ld_preload_test_helper calls get_value_from_lib() and returns the value.
  // The symbol is defined by two libs: ld_preload_test_helper_lib.so and
  // ld_preloaded_lib.so. The former is DT_NEEDED and the latter is LD_PRELOADED
  // via this execution. The main executable is linked to the LD_PRELOADED lib
  // and the value given from the lib is returned.
  eth.Run([&]() { execve(helper.c_str(), eth.GetArgs(), eth.GetEnv()); }, 0, "54321");
#endif
}


// ld_config_test_helper must fail because it is depending on a lib which is not
// in the search path
//
// Call sequence is...
// _helper -- (get_value_from_lib()) -->
//     _lib1.so -- (get_value_from_another_lib()) -->
//       _lib2.so (returns 12345)
// The two libs are in ns2/ subdir.
TEST(dl, exec_without_ld_config_file) {
#if defined(__BIONIC__)
  std::string error_message = "CANNOT LINK EXECUTABLE \"" + get_testlib_root() + "/ld_config_test_helper/ld_config_test_helper\": library \"ld_config_test_helper_lib1.so\" not found\n";
  std::string helper = get_testlib_root() +
      "/ld_config_test_helper/ld_config_test_helper";
  chmod(helper.c_str(), 0755);
  ExecTestHelper eth;
  eth.SetArgs({ helper.c_str(), nullptr });
  eth.Run([&]() { execve(helper.c_str(), eth.GetArgs(), eth.GetEnv()); }, -6, error_message.c_str());
#endif
}

#if defined(__BIONIC__)
static void create_ld_config_file(std::string& config_file) {
  std::ofstream fout(config_file.c_str(), std::ios::out);
  fout << "dir.test = " << get_testlib_root() << "/ld_config_test_helper/" << std::endl
       << "[test]" << std::endl
       << "additional.namespaces = ns2" << std::endl
       << "namespace.default.search.paths = " << get_testlib_root() << std::endl
       << "namespace.default.links = ns2" << std::endl
       << "namespace.default.link.ns2.shared_libs = libc.so:libm.so:libdl.so:ld_config_test_helper_lib1.so" << std::endl
       << "namespace.ns2.search.paths = /system/${LIB}:" << get_testlib_root() << "/ns2" << std::endl;
  fout.close();
}
#endif

#ifdef USE_LD_CONFIG_FILE

// _lib1.so and _lib2.so are now searchable by having another namespace 'ns2'
// whose search paths include the 'ns2/' subdir.
TEST(dl, exec_with_ld_config_file) {
#if defined(__BIONIC__)
  std::string helper = get_testlib_root() +
      "/ld_config_test_helper/ld_config_test_helper";
  std::string config_file = get_testlib_root() + "/ld.config.txt";
  create_ld_config_file(config_file);
  std::string env = std::string("LD_CONFIG_FILE=") + config_file;
  chmod(helper.c_str(), 0755);
  ExecTestHelper eth;
  eth.SetArgs({ helper.c_str(), nullptr });
  eth.SetEnv({ env.c_str(), nullptr });
  eth.Run([&]() { execve(helper.c_str(), eth.GetArgs(), eth.GetEnv()); }, 0, "12345");
#endif
}

// _lib3.so has same symbol as lib2.so but returns 54321. _lib3.so is
// LD_PRELOADed. This test is to ensure LD_PRELOADed libs are available to
// additional namespaces other than the default namespace.
TEST(dl, exec_with_ld_config_file_with_ld_preload) {
#if defined(__BIONIC__)
  std::string helper = get_testlib_root() +
      "/ld_config_test_helper/ld_config_test_helper";
  std::string config_file = get_testlib_root() + "/ld.config.txt";
  create_ld_config_file(config_file);
  std::string env = std::string("LD_CONFIG_FILE=") + config_file;
  std::string env2 = std::string("LD_PRELOAD=") + get_testlib_root() + "/ld_config_test_helper_lib3.so";
  chmod(helper.c_str(), 0755);
  ExecTestHelper eth;
  eth.SetArgs({ helper.c_str(), nullptr });
  eth.SetEnv({ env.c_str(), env2.c_str(), nullptr });
  eth.Run([&]() { execve(helper.c_str(), eth.GetArgs(), eth.GetEnv()); }, 0, "54321");
#endif
}

#endif // USE_LD_CONFIG_FILE

// ensures that LD_CONFIG_FILE env var does not work for production builds.
// The test input is the same as exec_with_ld_config_file, but it must fail in
// this case.
TEST(dl, disable_ld_config_file) {
#if defined(__BIONIC__)
  if (getuid() == 0) {
    // when executed from the shell (e.g. not as part of CTS), skip the test.
    // This test is only for CTS.
    return;
  }
  std::string error_message = "CANNOT LINK EXECUTABLE \"" + get_testlib_root() + "/ld_config_test_helper/ld_config_test_helper\": library \"ld_config_test_helper_lib1.so\" not found\n";
  std::string helper = get_testlib_root() +
      "/ld_config_test_helper/ld_config_test_helper";
  std::string config_file = get_testlib_root() + "/ld.config.txt";
  create_ld_config_file(config_file);
  std::string env = std::string("LD_CONFIG_FILE=") + config_file;
  chmod(helper.c_str(), 0755);
  ExecTestHelper eth;
  eth.SetArgs({ helper.c_str(), nullptr });
  eth.SetEnv({ env.c_str(), nullptr });
  eth.Run([&]() { execve(helper.c_str(), eth.GetArgs(), eth.GetEnv()); }, -6, error_message.c_str());
#endif
}
