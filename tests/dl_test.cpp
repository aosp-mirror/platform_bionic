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

#if defined(__BIONIC__)
#include <android-base/properties.h>
#endif

#include <dlfcn.h>
#include <libgen.h>
#include <limits.h>
#include <stdio.h>
#include <stdint.h>
#include <sys/stat.h>

#include <fstream>
#include <iostream>
#include <regex>
#include <string>

#include <android-base/file.h>
#include <android-base/macros.h>
#include <android-base/test_utils.h>
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

#if defined(__BIONIC__)
#if defined(__LP64__)
#define LINKER_NAME "linker64"
#else
#define LINKER_NAME "linker"
#endif
static constexpr const char* kPathToLinker = "/system/bin/" LINKER_NAME;
static constexpr const char* kAlternatePathToLinker = "/system/bin/" ABI_STRING "/" LINKER_NAME;
#undef LINKER_NAME

const char* PathToLinker() {
  // On the systems with emulated architecture linker would be of different
  // architecture. Try to use alternate paths first.
  struct stat buffer;
  if (stat(kAlternatePathToLinker, &buffer) == 0) {
    return kAlternatePathToLinker;
  }
  return kPathToLinker;
}
#endif  // defined(__BIONIC__)

TEST(dl, exec_linker) {
#if defined(__BIONIC__)
  const char* path_to_linker = PathToLinker();
  std::string usage_prefix = std::string("Usage: ") + path_to_linker;
  ExecTestHelper eth;
  eth.SetArgs({ path_to_linker, nullptr });
  eth.Run([&]() { execve(path_to_linker, eth.GetArgs(), eth.GetEnv()); }, 0, nullptr);
  ASSERT_EQ(0u, eth.GetOutput().find(usage_prefix)) << "Test output:\n" << eth.GetOutput();
#endif
}

TEST(dl, exec_linker_load_file) {
#if defined(__BIONIC__)
  const char* path_to_linker = PathToLinker();
  std::string helper = GetTestLibRoot() + "/exec_linker_helper";
  std::string expected_output =
      "ctor: argc=1 argv[0]=" + helper + "\n" +
      "main: argc=1 argv[0]=" + helper + "\n" +
      "__progname=exec_linker_helper\n" +
      "helper_func called\n";
  ExecTestHelper eth;
  eth.SetArgs({ path_to_linker, helper.c_str(), nullptr });
  eth.Run([&]() { execve(path_to_linker, eth.GetArgs(), eth.GetEnv()); }, 0, nullptr);
  ASSERT_EQ(expected_output, eth.GetOutput());
#endif
}

TEST(dl, exec_linker_load_from_zip) {
#if defined(__BIONIC__)
  const char* path_to_linker = PathToLinker();
  std::string helper = GetTestLibRoot() +
      "/libdlext_test_zip/libdlext_test_zip_zipaligned.zip!/libdir/exec_linker_helper";
  std::string expected_output =
      "ctor: argc=1 argv[0]=" + helper + "\n" +
      "main: argc=1 argv[0]=" + helper + "\n" +
      "__progname=exec_linker_helper\n" +
      "helper_func called\n";
  ExecTestHelper eth;
  eth.SetArgs({ path_to_linker, helper.c_str(), nullptr });
  eth.Run([&]() { execve(path_to_linker, eth.GetArgs(), eth.GetEnv()); }, 0, nullptr);
  ASSERT_EQ(expected_output, eth.GetOutput());
#endif
}

TEST(dl, exec_linker_load_self) {
#if defined(__BIONIC__)
  const char* path_to_linker = PathToLinker();
  std::string error_message = "error: linker cannot load itself\n";
  ExecTestHelper eth;
  eth.SetArgs({ path_to_linker, path_to_linker, nullptr });
  eth.Run([&]() { execve(path_to_linker, eth.GetArgs(), eth.GetEnv()); }, EXIT_FAILURE, error_message.c_str());
#endif
}

TEST(dl, preinit_system_calls) {
#if defined(__BIONIC__)
  SKIP_WITH_HWASAN << "hwasan not initialized in preinit_array, b/124007027";
  std::string helper = GetTestLibRoot() + "/preinit_syscall_test_helper";
  chmod(helper.c_str(), 0755); // TODO: "x" lost in CTS, b/34945607
  ExecTestHelper eth;
  eth.SetArgs({ helper.c_str(), nullptr });
  eth.Run([&]() { execve(helper.c_str(), eth.GetArgs(), eth.GetEnv()); }, 0, nullptr);
#endif
}

TEST(dl, preinit_getauxval) {
#if defined(__BIONIC__)
  SKIP_WITH_HWASAN << "hwasan not initialized in preinit_array, b/124007027";
  std::string helper = GetTestLibRoot() + "/preinit_getauxval_test_helper";
  chmod(helper.c_str(), 0755); // TODO: "x" lost in CTS, b/34945607
  ExecTestHelper eth;
  eth.SetArgs({ helper.c_str(), nullptr });
  eth.Run([&]() { execve(helper.c_str(), eth.GetArgs(), eth.GetEnv()); }, 0, nullptr);
#else
  // Force a failure when not compiled for bionic so the test is considered a pass.
  ASSERT_TRUE(false);
#endif
}


TEST(dl, exec_without_ld_preload) {
#if defined(__BIONIC__)
  std::string helper = GetTestLibRoot() + "/ld_preload_test_helper";
  chmod(helper.c_str(), 0755);
  ExecTestHelper eth;
  eth.SetArgs({ helper.c_str(), nullptr });
  eth.Run([&]() { execve(helper.c_str(), eth.GetArgs(), eth.GetEnv()); }, 0, "12345");
#endif
}

TEST(dl, exec_with_ld_preload) {
#if defined(__BIONIC__)
  std::string helper = GetTestLibRoot() + "/ld_preload_test_helper";
  std::string env = std::string("LD_PRELOAD=") + GetTestLibRoot() + "/ld_preload_test_helper_lib2.so";
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
  std::string error_message = "CANNOT LINK EXECUTABLE \"" + GetTestLibRoot() +
                              "/ld_config_test_helper\": library \"ld_config_test_helper_lib1.so\" "
                              "not found: needed by main executable\n";
  std::string helper = GetTestLibRoot() + "/ld_config_test_helper";
  chmod(helper.c_str(), 0755);
  ExecTestHelper eth;
  eth.SetArgs({ helper.c_str(), nullptr });
  eth.Run([&]() { execve(helper.c_str(), eth.GetArgs(), eth.GetEnv()); }, EXIT_FAILURE, error_message.c_str());
#endif
}

#if defined(__BIONIC__)
extern "C" void android_get_LD_LIBRARY_PATH(char*, size_t);
static void create_ld_config_file(const char* config_file) {
  char default_search_paths[PATH_MAX];
  android_get_LD_LIBRARY_PATH(default_search_paths, sizeof(default_search_paths));

  std::ofstream fout(config_file, std::ios::out);
  fout << "dir.test = " << GetTestLibRoot() << "/" << std::endl
       << "[test]" << std::endl
       << "additional.namespaces = ns2" << std::endl
       << "namespace.default.search.paths = " << GetTestLibRoot() << std::endl
       << "namespace.default.links = ns2" << std::endl
       << "namespace.default.link.ns2.shared_libs = "
          "libc.so:libm.so:libdl.so:ld_config_test_helper_lib1.so"
       << std::endl
       << "namespace.ns2.search.paths = " << default_search_paths << ":" << GetTestLibRoot()
       << "/ns2" << std::endl;
  fout.close();
}
#endif

#if defined(__BIONIC__)
// This test can't rely on ro.debuggable, because it might have been forced on
// in a user build ("Force Debuggable"). In that configuration, ro.debuggable is
// true, but Bionic's LD_CONFIG_FILE testing support is still disabled.
static bool is_user_build() {
  return android::base::GetProperty("ro.build.type", "user") == std::string("user");
}
#endif

// lib1.so and lib2.so are now searchable by having another namespace 'ns2'
// whose search paths include the 'ns2/' subdir.
//
// lib1.so is linked with DF_1_GLOBAL, so both it and the executable are added
// to every namespace.
//
// namespace configuration ('*' indicates primary ns)
//  - default: exe[*], lib1.so
//  - ns2: exe, lib1.so[*], lib2.so[*]
//
TEST(dl, exec_with_ld_config_file) {
#if defined(__BIONIC__)
  SKIP_WITH_HWASAN << "libclang_rt.hwasan is not found with custom ld config";
  if (is_user_build()) {
    GTEST_SKIP() << "LD_CONFIG_FILE is not supported on user build";
  }
  std::string helper = GetTestLibRoot() + "/ld_config_test_helper";
  TemporaryFile config_file;
  create_ld_config_file(config_file.path);
  std::string env = std::string("LD_CONFIG_FILE=") + config_file.path;
  chmod(helper.c_str(), 0755);
  ExecTestHelper eth;
  eth.SetArgs({ helper.c_str(), nullptr });
  eth.SetEnv({ env.c_str(), nullptr });
  eth.Run([&]() { execve(helper.c_str(), eth.GetArgs(), eth.GetEnv()); }, 0,
          "foo lib1\n"
          "lib1_call_funcs\n"
          "foo lib1\n"
          "bar lib2\n");
#endif
}

// lib3.so has same foo and bar symbols as lib2.so. lib3.so is LD_PRELOADed.
// This test ensures that LD_PRELOADed libs are available to all namespaces.
//
// namespace configuration ('*' indicates primary ns)
//  - default: exe[*], lib3.so[*], lib1.so
//  - ns2: exe, lib3.so, lib1.so[*], lib2.so[*]
//
// Ensure that, in both namespaces, a call to foo calls the lib3.so symbol,
// which then calls the lib1.so symbol using RTLD_NEXT. Ensure that RTLD_NEXT
// finds nothing when called from lib1.so.
//
// For the bar symbol, lib3.so's primary namespace is the default namespace, but
// lib2.so is not in the default namespace, so using RTLD_NEXT from lib3.so
// doesn't find the symbol in lib2.so.
TEST(dl, exec_with_ld_config_file_with_ld_preload) {
#if defined(__BIONIC__)
  SKIP_WITH_HWASAN << "libclang_rt.hwasan is not found with custom ld config";
  if (is_user_build()) {
    GTEST_SKIP() << "LD_CONFIG_FILE is not supported on user build";
  }
  std::string helper = GetTestLibRoot() + "/ld_config_test_helper";
  TemporaryFile config_file;
  create_ld_config_file(config_file.path);
  std::string env = std::string("LD_CONFIG_FILE=") + config_file.path;
  std::string env2 = std::string("LD_PRELOAD=") + GetTestLibRoot() + "/ld_config_test_helper_lib3.so";
  chmod(helper.c_str(), 0755);
  ExecTestHelper eth;
  eth.SetArgs({ helper.c_str(), nullptr });
  eth.SetEnv({ env.c_str(), env2.c_str(), nullptr });
  eth.Run([&]() { execve(helper.c_str(), eth.GetArgs(), eth.GetEnv()); }, 0,
          "foo lib3\n"
          "foo lib1\n"
          "lib1_call_funcs\n"
          "foo lib3\n"
          "foo lib1\n"
          "bar lib3\n"
          "lib3_call_funcs\n"
          "foo lib3\n"
          "foo lib1\n"
          "bar lib3\n");
#endif
}

// ensures that LD_CONFIG_FILE env var does not work for production builds.
// The test input is the same as exec_with_ld_config_file, but it must fail in
// this case.
TEST(dl, disable_ld_config_file) {
#if defined(__BIONIC__)
  if (getuid() == 0) {
    // when executed from the shell (e.g. not as part of CTS), skip the test.
    // This test is only for CTS.
    GTEST_SKIP() << "test is not supported with root uid";
  }
  if (!is_user_build()) {
    GTEST_SKIP() << "test requires user build";
  }

  std::string error_message =
      std::string("CANNOT LINK EXECUTABLE ") + "\"" + GetTestLibRoot() +
      "/ld_config_test_helper\": " +
      "library \"ld_config_test_helper_lib1.so\" not found: needed by main executable\n";
  std::string helper = GetTestLibRoot() + "/ld_config_test_helper";
  TemporaryFile config_file;
  create_ld_config_file(config_file.path);
  std::string env = std::string("LD_CONFIG_FILE=") + config_file.path;
  chmod(helper.c_str(), 0755);
  ExecTestHelper eth;
  eth.SetArgs({ helper.c_str(), nullptr });
  eth.SetEnv({ env.c_str(), nullptr });
  eth.Run([&]() { execve(helper.c_str(), eth.GetArgs(), eth.GetEnv()); }, EXIT_FAILURE, error_message.c_str());
#endif
}

static void RelocationsTest(const char* lib, const char* expectation) {
#if defined(__BIONIC__)
  // Does readelf think the .so file looks right?
  const std::string path = GetTestLibRoot() + "/" + lib;
  ExecTestHelper eth;
  eth.SetArgs({ "readelf", "-SW", path.c_str(), nullptr });
  eth.Run([&]() { execvpe("readelf", eth.GetArgs(), eth.GetEnv()); }, 0, nullptr);

  ASSERT_TRUE(std::regex_search(eth.GetOutput(), std::regex(expectation))) << eth.GetOutput();

  // Can we load it?
  void* handle = dlopen(lib, RTLD_NOW);
  ASSERT_TRUE(handle != nullptr) << dlerror();
#else
  UNUSED(lib);
  UNUSED(expectation);
  GTEST_SKIP() << "test is not supported on glibc";
#endif
}

TEST(dl, relocations_RELR) {
  RelocationsTest("librelocations-RELR.so", "\\.relr\\.dyn * RELR");
}

TEST(dl, relocations_ANDROID_RELR) {
  RelocationsTest("librelocations-ANDROID_RELR.so", "\\.relr\\.dyn * ANDROID_RELR");
}

TEST(dl, relocations_ANDROID_REL) {
  RelocationsTest("librelocations-ANDROID_REL.so",
#if __LP64__
                  "\\.rela\\.dyn * ANDROID_RELA"
#else
                  "\\.rel\\.dyn * ANDROID_REL"
#endif
  );
}

TEST(dl, relocations_fat) {
  RelocationsTest("librelocations-fat.so",
#if __LP64__
                  "\\.rela\\.dyn * RELA"
#else
                  "\\.rel\\.dyn * REL"
#endif
  );
}
