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
#include <limits.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#if __has_include(<sys/auxv.h>)
#include <sys/auxv.h>
#endif
#include <sys/user.h>

#include <string>
#include <thread>

#include <android-base/file.h>
#include <android-base/scopeguard.h>

#include "gtest_globals.h"
#include "gtest_utils.h"
#include "dlfcn_symlink_support.h"
#include "utils.h"

#if defined(__BIONIC__) && (defined(__arm__) || defined(__i386__))
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"

#include <llvm/ADT/StringRef.h>
#include <llvm/Object/Binary.h>
#include <llvm/Object/ELFObjectFile.h>
#include <llvm/Object/ObjectFile.h>

#pragma clang diagnostic pop
#endif //  defined(__ANDROID__) && (defined(__arm__) || defined(__i386__))

#define ASSERT_SUBSTR(needle, haystack) \
    ASSERT_PRED_FORMAT2(::testing::IsSubstring, needle, haystack)


static bool g_called = false;
extern "C" void DlSymTestFunction() {
  g_called = true;
}

static int g_ctor_function_called = 0;
static int g_ctor_argc = 0;
static char** g_ctor_argv = reinterpret_cast<char**>(0xDEADBEEF);
static char** g_ctor_envp = g_ctor_envp;

extern "C" void ctor_function(int argc, char** argv, char** envp) __attribute__ ((constructor));

extern "C" void ctor_function(int argc, char** argv, char** envp) {
  g_ctor_function_called = 17;
  g_ctor_argc = argc;
  g_ctor_argv = argv;
  g_ctor_envp = envp;
}

TEST(dlfcn, ctor_function_call) {
  ASSERT_EQ(17, g_ctor_function_called);
  ASSERT_TRUE(g_ctor_argc = GetArgc());
  ASSERT_TRUE(g_ctor_argv = GetArgv());
  ASSERT_TRUE(g_ctor_envp = GetEnvp());
}

TEST(dlfcn, dlsym_in_executable) {
  dlerror(); // Clear any pending errors.
  void* self = dlopen(nullptr, RTLD_NOW);
  ASSERT_TRUE(self != nullptr);
  ASSERT_TRUE(dlerror() == nullptr);

  void* sym = dlsym(self, "DlSymTestFunction");
  ASSERT_TRUE(sym != nullptr);

  void (*function)() = reinterpret_cast<void(*)()>(sym);

  g_called = false;
  function();
  ASSERT_TRUE(g_called);

  ASSERT_EQ(0, dlclose(self));
}

TEST(dlfcn, dlsym_from_sofile) {
  void* handle = dlopen("libtest_dlsym_from_this.so", RTLD_LAZY | RTLD_LOCAL);
  ASSERT_TRUE(handle != nullptr) << dlerror();

  // check that we can't find '_test_dlsym_symbol' via dlsym(RTLD_DEFAULT)
  void* symbol = dlsym(RTLD_DEFAULT, "test_dlsym_symbol");
  ASSERT_TRUE(symbol == nullptr);
  ASSERT_SUBSTR("undefined symbol: test_dlsym_symbol", dlerror());

  typedef int* (*fn_t)();
  fn_t lookup_dlsym_symbol_using_RTLD_DEFAULT =
      reinterpret_cast<fn_t>(dlsym(handle, "lookup_dlsym_symbol_using_RTLD_DEFAULT"));
  ASSERT_TRUE(lookup_dlsym_symbol_using_RTLD_DEFAULT != nullptr) << dlerror();

  int* ptr = lookup_dlsym_symbol_using_RTLD_DEFAULT();
  ASSERT_TRUE(ptr != nullptr) << dlerror();
  ASSERT_EQ(42, *ptr);

  fn_t lookup_dlsym_symbol2_using_RTLD_DEFAULT =
      reinterpret_cast<fn_t>(dlsym(handle, "lookup_dlsym_symbol2_using_RTLD_DEFAULT"));
  ASSERT_TRUE(lookup_dlsym_symbol2_using_RTLD_DEFAULT != nullptr) << dlerror();

  ptr = lookup_dlsym_symbol2_using_RTLD_DEFAULT();
  ASSERT_TRUE(ptr != nullptr) << dlerror();
  ASSERT_EQ(44, *ptr);

  fn_t lookup_dlsym_symbol_using_RTLD_NEXT =
      reinterpret_cast<fn_t>(dlsym(handle, "lookup_dlsym_symbol_using_RTLD_NEXT"));
  ASSERT_TRUE(lookup_dlsym_symbol_using_RTLD_NEXT != nullptr) << dlerror();

  ptr = lookup_dlsym_symbol_using_RTLD_NEXT();
  ASSERT_TRUE(ptr != nullptr) << dlerror();
  ASSERT_EQ(43, *ptr);

  dlclose(handle);
}

TEST(dlfcn, dlsym_from_sofile_with_preload) {
  void* preload = dlopen("libtest_dlsym_from_this_grandchild.so", RTLD_NOW | RTLD_LOCAL);
  ASSERT_TRUE(preload != nullptr) << dlerror();

  void* handle = dlopen("libtest_dlsym_from_this.so", RTLD_NOW | RTLD_LOCAL);
  ASSERT_TRUE(handle != nullptr) << dlerror();

  // check that we can't find '_test_dlsym_symbol' via dlsym(RTLD_DEFAULT)
  void* symbol = dlsym(RTLD_DEFAULT, "test_dlsym_symbol");
  ASSERT_TRUE(symbol == nullptr);
  ASSERT_SUBSTR("undefined symbol: test_dlsym_symbol", dlerror());

  typedef int* (*fn_t)();
  fn_t lookup_dlsym_symbol_using_RTLD_DEFAULT =
      reinterpret_cast<fn_t>(dlsym(handle, "lookup_dlsym_symbol_using_RTLD_DEFAULT"));
  ASSERT_TRUE(lookup_dlsym_symbol_using_RTLD_DEFAULT != nullptr) << dlerror();

  int* ptr = lookup_dlsym_symbol_using_RTLD_DEFAULT();
  ASSERT_TRUE(ptr != nullptr) << dlerror();
  ASSERT_EQ(42, *ptr);

  fn_t lookup_dlsym_symbol2_using_RTLD_DEFAULT =
      reinterpret_cast<fn_t>(dlsym(handle, "lookup_dlsym_symbol2_using_RTLD_DEFAULT"));
  ASSERT_TRUE(lookup_dlsym_symbol2_using_RTLD_DEFAULT != nullptr) << dlerror();

  ptr = lookup_dlsym_symbol2_using_RTLD_DEFAULT();
  ASSERT_TRUE(ptr != nullptr) << dlerror();
  ASSERT_EQ(44, *ptr);

  fn_t lookup_dlsym_symbol_using_RTLD_NEXT =
      reinterpret_cast<fn_t>(dlsym(handle, "lookup_dlsym_symbol_using_RTLD_NEXT"));
  ASSERT_TRUE(lookup_dlsym_symbol_using_RTLD_NEXT != nullptr) << dlerror();

  ptr = lookup_dlsym_symbol_using_RTLD_NEXT();
  ASSERT_TRUE(ptr != nullptr) << dlerror();
  ASSERT_EQ(43, *ptr);

  dlclose(handle);
  dlclose(preload);
}

TEST(dlfcn, dlsym_handle_global_sym) {
  // check that we do not look into global group
  // when looking up symbol by handle
  void* handle = dlopen("libtest_empty.so", RTLD_NOW);
  dlopen("libtest_with_dependency.so", RTLD_NOW | RTLD_GLOBAL);
  void* sym = dlsym(handle, "getRandomNumber");
  ASSERT_TRUE(sym == nullptr);
  ASSERT_SUBSTR("undefined symbol: getRandomNumber", dlerror());

  sym = dlsym(handle, "DlSymTestFunction");
  ASSERT_TRUE(sym == nullptr);
  ASSERT_SUBSTR("undefined symbol: DlSymTestFunction", dlerror());
  dlclose(handle);
}

TEST(dlfcn, dlsym_handle_empty_symbol) {
  // check that dlsym of an empty symbol fails (see http://b/33530622)
  void* handle = dlopen("libtest_dlsym_from_this.so", RTLD_NOW);
  ASSERT_TRUE(handle != nullptr) << dlerror();
  void* sym = dlsym(handle, "");
  ASSERT_TRUE(sym == nullptr);
  ASSERT_SUBSTR("undefined symbol: ", dlerror());
  dlclose(handle);
}

TEST(dlfcn, dlsym_with_dependencies) {
  void* handle = dlopen("libtest_with_dependency.so", RTLD_NOW);
  ASSERT_TRUE(handle != nullptr);
  dlerror();
  // This symbol is in DT_NEEDED library.
  void* sym = dlsym(handle, "getRandomNumber");
  ASSERT_TRUE(sym != nullptr) << dlerror();
  int (*fn)(void);
  fn = reinterpret_cast<int (*)(void)>(sym);
  EXPECT_EQ(4, fn());
  dlclose(handle);
}

TEST(dlfcn, dlopen_noload) {
  void* handle = dlopen("libtest_simple.so", RTLD_NOW | RTLD_NOLOAD);
  ASSERT_TRUE(handle == nullptr);
  handle = dlopen("libtest_simple.so", RTLD_NOW);
  void* handle2 = dlopen("libtest_simple.so", RTLD_NOW | RTLD_NOLOAD);
  ASSERT_TRUE(handle != nullptr);
  ASSERT_TRUE(handle2 != nullptr);
  ASSERT_TRUE(handle == handle2);
  ASSERT_EQ(0, dlclose(handle));
  ASSERT_EQ(0, dlclose(handle2));
}

TEST(dlfcn, dlopen_by_soname) {
  static const char* soname = "libdlext_test_soname.so";
  static const char* filename = "libdlext_test_different_soname.so";
  // 1. Make sure there is no library with soname in default search path
  void* handle = dlopen(soname, RTLD_NOW);
  ASSERT_TRUE(handle == nullptr);

  // 2. Load a library using filename
  handle = dlopen(filename, RTLD_NOW);
  ASSERT_TRUE(handle != nullptr) << dlerror();

  // 3. Find library by soname
  void* handle_soname = dlopen(soname, RTLD_NOW | RTLD_NOLOAD);
  ASSERT_TRUE(handle_soname != nullptr) << dlerror();
  ASSERT_EQ(handle, handle_soname);

  // 4. RTLD_NOLOAD should still work with filename
  void* handle_filename = dlopen(filename, RTLD_NOW | RTLD_NOLOAD);
  ASSERT_TRUE(handle_filename != nullptr) << dlerror();
  ASSERT_EQ(handle, handle_filename);

  dlclose(handle_filename);
  dlclose(handle_soname);
  dlclose(handle);
}

TEST(dlfcn, dlopen_vdso) {
#if __has_include(<sys/auxv.h>)
  if (getauxval(AT_SYSINFO_EHDR) == 0) {
    GTEST_LOG_(INFO) << "getauxval(AT_SYSINFO_EHDR) == 0, skipping this test.";
    return;
  }
#endif

  const char* vdso_name = "linux-vdso.so.1";
#if defined(__i386__)
  vdso_name = "linux-gate.so.1";
#endif
  void* handle = dlopen(vdso_name, RTLD_NOW);
  ASSERT_TRUE(handle != nullptr) << dlerror();
  dlclose(handle);
}

// mips doesn't support ifuncs
#if !defined(__mips__)
TEST(dlfcn, ifunc_variable) {
  typedef const char* (*fn_ptr)();

  // ifunc's choice depends on whether IFUNC_CHOICE has a value
  // first check the set case
  setenv("IFUNC_CHOICE", "set", 1);
  // preload libtest_ifunc_variable_impl.so
  void* handle_impl = dlopen("libtest_ifunc_variable_impl.so", RTLD_NOW);
  void* handle = dlopen("libtest_ifunc_variable.so", RTLD_NOW);
  ASSERT_TRUE(handle != nullptr) << dlerror();
  const char** foo_ptr = reinterpret_cast<const char**>(dlsym(handle, "foo"));
  fn_ptr foo_library_ptr = reinterpret_cast<fn_ptr>(dlsym(handle, "foo_library"));
  ASSERT_TRUE(foo_ptr != nullptr) << dlerror();
  ASSERT_TRUE(foo_library_ptr != nullptr) << dlerror();
  ASSERT_EQ(strncmp("set", *foo_ptr, 3), 0);
  ASSERT_EQ(strncmp("set", foo_library_ptr(), 3), 0);
  dlclose(handle);
  dlclose(handle_impl);

  // then check the unset case
  unsetenv("IFUNC_CHOICE");
  handle_impl = dlopen("libtest_ifunc_variable_impl.so", RTLD_NOW);
  handle = dlopen("libtest_ifunc_variable.so", RTLD_NOW);
  ASSERT_TRUE(handle != nullptr) << dlerror();
  foo_ptr = reinterpret_cast<const char**>(dlsym(handle, "foo"));
  foo_library_ptr = reinterpret_cast<fn_ptr>(dlsym(handle, "foo_library"));
  ASSERT_TRUE(foo_ptr != nullptr) << dlerror();
  ASSERT_TRUE(foo_library_ptr != nullptr) << dlerror();
  ASSERT_EQ(strncmp("unset", *foo_ptr, 5), 0);
  ASSERT_EQ(strncmp("unset", foo_library_ptr(), 5), 0);
  dlclose(handle);
  dlclose(handle_impl);
}

TEST(dlfcn, ifunc) {
  typedef const char* (*fn_ptr)();

  // ifunc's choice depends on whether IFUNC_CHOICE has a value
  // first check the set case
  setenv("IFUNC_CHOICE", "set", 1);
  void* handle = dlopen("libtest_ifunc.so", RTLD_NOW);
  ASSERT_TRUE(handle != nullptr) << dlerror();
  fn_ptr foo_ptr = reinterpret_cast<fn_ptr>(dlsym(handle, "foo"));
  fn_ptr foo_library_ptr = reinterpret_cast<fn_ptr>(dlsym(handle, "foo_library"));
  ASSERT_TRUE(foo_ptr != nullptr) << dlerror();
  ASSERT_TRUE(foo_library_ptr != nullptr) << dlerror();
  ASSERT_EQ(strncmp("set", foo_ptr(), 3), 0);
  ASSERT_EQ(strncmp("set", foo_library_ptr(), 3), 0);
  dlclose(handle);

  // then check the unset case
  unsetenv("IFUNC_CHOICE");
  handle = dlopen("libtest_ifunc.so", RTLD_NOW);
  ASSERT_TRUE(handle != nullptr) << dlerror();
  foo_ptr = reinterpret_cast<fn_ptr>(dlsym(handle, "foo"));
  foo_library_ptr = reinterpret_cast<fn_ptr>(dlsym(handle, "foo_library"));
  ASSERT_TRUE(foo_ptr != nullptr) << dlerror();
  ASSERT_TRUE(foo_library_ptr != nullptr) << dlerror();
  ASSERT_EQ(strncmp("unset", foo_ptr(), 5), 0);
  ASSERT_EQ(strncmp("unset", foo_library_ptr(), 5), 0);
  dlclose(handle);
}

TEST(dlfcn, ifunc_ctor_call) {
  typedef const char* (*fn_ptr)();

  void* handle = dlopen("libtest_ifunc.so", RTLD_NOW);
  ASSERT_TRUE(handle != nullptr) << dlerror();
  fn_ptr is_ctor_called =  reinterpret_cast<fn_ptr>(dlsym(handle, "is_ctor_called_irelative"));
  ASSERT_TRUE(is_ctor_called != nullptr) << dlerror();
  ASSERT_STREQ("false", is_ctor_called());

  is_ctor_called =  reinterpret_cast<fn_ptr>(dlsym(handle, "is_ctor_called_jump_slot"));
  ASSERT_TRUE(is_ctor_called != nullptr) << dlerror();
  ASSERT_STREQ("true", is_ctor_called());
  dlclose(handle);
}

TEST(dlfcn, ifunc_ctor_call_rtld_lazy) {
  typedef const char* (*fn_ptr)();

  void* handle = dlopen("libtest_ifunc.so", RTLD_LAZY);
  ASSERT_TRUE(handle != nullptr) << dlerror();
  fn_ptr is_ctor_called =  reinterpret_cast<fn_ptr>(dlsym(handle, "is_ctor_called_irelative"));
  ASSERT_TRUE(is_ctor_called != nullptr) << dlerror();
  ASSERT_STREQ("false", is_ctor_called());

  is_ctor_called =  reinterpret_cast<fn_ptr>(dlsym(handle, "is_ctor_called_jump_slot"));
  ASSERT_TRUE(is_ctor_called != nullptr) << dlerror();
  ASSERT_STREQ("true", is_ctor_called());
  dlclose(handle);
}
#endif

TEST(dlfcn, dlopen_check_relocation_dt_needed_order) {
  // This is the structure of the test library and
  // its dt_needed libraries
  // libtest_relo_check_dt_needed_order.so
  // |
  // +-> libtest_relo_check_dt_needed_order_1.so
  // |
  // +-> libtest_relo_check_dt_needed_order_2.so
  //
  // The root library references relo_test_get_answer_lib - which is defined
  // in both dt_needed libraries, the correct relocation should
  // use the function defined in libtest_relo_check_dt_needed_order_1.so
  void* handle = nullptr;
  auto guard = android::base::make_scope_guard([&]() { dlclose(handle); });

  handle = dlopen("libtest_relo_check_dt_needed_order.so", RTLD_NOW);
  ASSERT_TRUE(handle != nullptr) << dlerror();

  typedef int (*fn_t) (void);
  fn_t fn = reinterpret_cast<fn_t>(dlsym(handle, "relo_test_get_answer"));
  ASSERT_TRUE(fn != nullptr) << dlerror();
  ASSERT_EQ(1, fn());
}

TEST(dlfcn, dlopen_check_order_dlsym) {
  // Here is how the test library and its dt_needed
  // libraries are arranged
  //
  //  libtest_check_order_children.so
  //  |
  //  +-> ..._1_left.so
  //  |   |
  //  |   +-> ..._a.so
  //  |   |
  //  |   +-> ...r_b.so
  //  |
  //  +-> ..._2_right.so
  //  |   |
  //  |   +-> ..._d.so
  //  |       |
  //  |       +-> ..._b.so
  //  |
  //  +-> ..._3_c.so
  //
  //  load order should be (1, 2, 3, a, b, d)
  //
  // get_answer() is defined in (2, 3, a, b, c)
  // get_answer2() is defined in (b, d)
  void* sym = dlsym(RTLD_DEFAULT, "check_order_dlsym_get_answer");
  ASSERT_TRUE(sym == nullptr);
  void* handle = dlopen("libtest_check_order_dlsym.so", RTLD_NOW | RTLD_GLOBAL);
  ASSERT_TRUE(handle != nullptr) << dlerror();
  typedef int (*fn_t) (void);
  fn_t fn, fn2;
  fn = reinterpret_cast<fn_t>(dlsym(RTLD_DEFAULT, "check_order_dlsym_get_answer"));
  ASSERT_TRUE(fn != nullptr) << dlerror();
  fn2 = reinterpret_cast<fn_t>(dlsym(RTLD_DEFAULT, "check_order_dlsym_get_answer2"));
  ASSERT_TRUE(fn2 != nullptr) << dlerror();

  ASSERT_EQ(42, fn());
  ASSERT_EQ(43, fn2());
  dlclose(handle);
}

TEST(dlfcn, dlopen_check_order_reloc_siblings) {
  // This is how this one works:
  // we lookup and call get_answer which is defined in '_2.so'
  // and in turn calls external get_answer_impl() defined in _1.so and in '_[a-f].so'
  // the correct _impl() is implemented by '_a.so';
  //
  // Note that this is test for RTLD_LOCAL (TODO: test for GLOBAL?)
  //
  // Here is the picture:
  //
  // libtest_check_order_reloc_siblings.so
  // |
  // +-> ..._1.so <- empty
  // |   |
  // |   +-> ..._a.so <- exports correct answer_impl()
  // |   |
  // |   +-> ..._b.so <- every other letter exporting incorrect one.
  // |
  // +-> ..._2.so <- empty
  // |   |
  // |   +-> ..._c.so
  // |   |
  // |   +-> ..._d.so
  // |
  // +-> ..._3.so <- empty
  //     |
  //     +-> ..._e.so
  //     |
  //     +-> ..._f.so <- exports get_answer() that calls get_anser_impl();
  //                     implements incorrect get_answer_impl()

  void* handle = dlopen("libtest_check_order_reloc_siblings.so", RTLD_NOW | RTLD_NOLOAD);
  ASSERT_TRUE(handle == nullptr);
#ifdef __BIONIC__
  // TODO: glibc returns nullptr on dlerror() here. Is it bug?
  ASSERT_STREQ("dlopen failed: library \"libtest_check_order_reloc_siblings.so\" wasn't loaded and RTLD_NOLOAD prevented it", dlerror());
#endif

  handle = dlopen("libtest_check_order_reloc_siblings.so", RTLD_NOW | RTLD_LOCAL);
  ASSERT_TRUE(handle != nullptr) << dlerror();

  typedef int (*fn_t) (void);
  fn_t fn = reinterpret_cast<fn_t>(dlsym(handle, "check_order_reloc_get_answer"));
  ASSERT_TRUE(fn != nullptr) << dlerror();
  ASSERT_EQ(42, fn());

  ASSERT_EQ(0, dlclose(handle));
}

TEST(dlfcn, dlopen_check_order_reloc_siblings_with_preload) {
  // This test uses the same library as dlopen_check_order_reloc_siblings.
  // Unlike dlopen_check_order_reloc_siblings it preloads
  // libtest_check_order_reloc_siblings_1.so (first dependency) prior to
  // dlopen(libtest_check_order_reloc_siblings.so)

  void* handle = dlopen("libtest_check_order_reloc_siblings.so", RTLD_NOW | RTLD_NOLOAD);
  ASSERT_TRUE(handle == nullptr);
  handle = dlopen("libtest_check_order_reloc_siblings_1.so", RTLD_NOW | RTLD_NOLOAD);
  ASSERT_TRUE(handle == nullptr);

  void* handle_for_1 = dlopen("libtest_check_order_reloc_siblings_1.so", RTLD_NOW | RTLD_LOCAL);
  ASSERT_TRUE(handle_for_1 != nullptr) << dlerror();

  handle = dlopen("libtest_check_order_reloc_siblings.so", RTLD_NOW | RTLD_LOCAL);
  ASSERT_TRUE(handle != nullptr) << dlerror();

  ASSERT_EQ(0, dlclose(handle_for_1));

  typedef int (*fn_t) (void);
  fn_t fn = reinterpret_cast<fn_t>(dlsym(handle, "check_order_reloc_get_answer"));
  ASSERT_TRUE(fn != nullptr) << dlerror();
  ASSERT_EQ(42, fn());

  ASSERT_EQ(0, dlclose(handle));
}

TEST(dlfcn, dlopen_check_order_reloc_grandchild) {
  // This is how this one works:
  // we lookup and call grandchild_get_answer which is defined in '_2.so'
  // and in turn calls external get_answer_impl() defined in '_c_1.so and _c_2.so'
  // the correct _impl() is implemented by '_c_1.so';
  //
  // Here is the picture of subtree:
  //
  // libtest_check_order_reloc_siblings.so
  // |
  // +-> ..._2.so <- grandchild_get_answer()
  //     |
  //     +-> ..._c.so <- empty
  //     |   |
  //     |   +-> _c_1.so <- exports correct answer_impl()
  //     |   |
  //     |   +-> _c_2.so <- exports incorrect answer_impl()
  //     |
  //     +-> ..._d.so <- empty

  void* handle = dlopen("libtest_check_order_reloc_siblings.so", RTLD_NOW | RTLD_NOLOAD);
  ASSERT_TRUE(handle == nullptr);
#ifdef __BIONIC__
  // TODO: glibc returns nullptr on dlerror() here. Is it bug?
  ASSERT_STREQ("dlopen failed: library \"libtest_check_order_reloc_siblings.so\" wasn't loaded and RTLD_NOLOAD prevented it", dlerror());
#endif

  handle = dlopen("libtest_check_order_reloc_siblings.so", RTLD_NOW | RTLD_LOCAL);
  ASSERT_TRUE(handle != nullptr) << dlerror();

  typedef int (*fn_t) (void);
  fn_t fn = reinterpret_cast<fn_t>(dlsym(handle, "check_order_reloc_grandchild_get_answer"));
  ASSERT_TRUE(fn != nullptr) << dlerror();
  ASSERT_EQ(42, fn());

  ASSERT_EQ(0, dlclose(handle));
}

TEST(dlfcn, dlopen_check_order_reloc_nephew) {
  // This is how this one works:
  // we lookup and call nephew_get_answer which is defined in '_2.so'
  // and in turn calls external get_answer_impl() defined in '_[a-f].so'
  // the correct _impl() is implemented by '_a.so';
  //
  // Here is the picture:
  //
  // libtest_check_order_reloc_siblings.so
  // |
  // +-> ..._1.so <- empty
  // |   |
  // |   +-> ..._a.so <- exports correct answer_impl()
  // |   |
  // |   +-> ..._b.so <- every other letter exporting incorrect one.
  // |
  // +-> ..._2.so <- empty
  // |   |
  // |   +-> ..._c.so
  // |   |
  // |   +-> ..._d.so
  // |
  // +-> ..._3.so <- nephew_get_answer() that calls get_answer_impl();
  //     |
  //     +-> ..._e.so
  //     |
  //     +-> ..._f.so

  void* handle = dlopen("libtest_check_order_reloc_siblings.so", RTLD_NOW | RTLD_NOLOAD);
  ASSERT_TRUE(handle == nullptr);
#ifdef __BIONIC__
  // TODO: glibc returns nullptr on dlerror() here. Is it bug?
  ASSERT_STREQ("dlopen failed: library \"libtest_check_order_reloc_siblings.so\" wasn't loaded and RTLD_NOLOAD prevented it", dlerror());
#endif

  handle = dlopen("libtest_check_order_reloc_siblings.so", RTLD_NOW | RTLD_LOCAL);
  ASSERT_TRUE(handle != nullptr) << dlerror();

  typedef int (*fn_t) (void);
  fn_t fn = reinterpret_cast<fn_t>(dlsym(handle, "check_order_reloc_nephew_get_answer"));
  ASSERT_TRUE(fn != nullptr) << dlerror();
  ASSERT_EQ(42, fn());

  ASSERT_EQ(0, dlclose(handle));
}

TEST(dlfcn, check_unload_after_reloc) {
  // This is how this one works:
  // libtest_two_parents_parent1 <- answer_impl() used by libtest_two_parents_child
  // |
  // +-> libtest_two_parents_child
  //
  // libtest_two_parents_parent2 <- answer_impl() not used by libtest_two_parents_child
  // |
  // +-> libtest_two_parents_child
  //
  // Test dlopens parent1 which loads and relocates libtest_two_parents_child.so
  // as a second step it dlopens parent2 and dlcloses parent1...

  void* handle = dlopen("libtest_two_parents_parent1.so", RTLD_NOW | RTLD_LOCAL);
  ASSERT_TRUE(handle != nullptr) << dlerror();

  void* handle2 = dlopen("libtest_two_parents_parent2.so", RTLD_NOW | RTLD_LOCAL);
  ASSERT_TRUE(handle2 != nullptr) << dlerror();

  typedef int (*fn_t) (void);
  fn_t fn = reinterpret_cast<fn_t>(dlsym(handle2, "check_order_reloc_get_answer"));
  ASSERT_TRUE(fn != nullptr) << dlerror();
  ASSERT_EQ(42, fn());

  ASSERT_EQ(0, dlclose(handle));

  handle = dlopen("libtest_two_parents_parent1.so", RTLD_NOW | RTLD_LOCAL | RTLD_NOLOAD);
  ASSERT_TRUE(handle != nullptr);
  ASSERT_EQ(0, dlclose(handle));

  fn = reinterpret_cast<fn_t>(dlsym(handle2, "check_order_reloc_get_answer"));
  ASSERT_TRUE(fn != nullptr) << dlerror();
  ASSERT_EQ(42, fn());

  ASSERT_EQ(0, dlclose(handle2));

  handle = dlopen("libtest_two_parents_parent1.so", RTLD_NOW | RTLD_LOCAL | RTLD_NOLOAD);
  ASSERT_TRUE(handle == nullptr);
}

extern "C" int check_order_reloc_root_get_answer_impl() {
  return 42;
}

TEST(dlfcn, dlopen_check_order_reloc_main_executable) {
  // This is how this one works:
  // we lookup and call get_answer3 which is defined in 'root.so'
  // and in turn calls external root_get_answer_impl() defined in _2.so and
  // above the correct _impl() is one in the executable.
  //
  // libtest_check_order_reloc_root.so
  // |
  // +-> ..._1.so <- empty
  // |
  // +-> ..._2.so <- gives incorrect answer for answer_main_impl()
  //

  void* handle = dlopen("libtest_check_order_reloc_root.so", RTLD_NOW | RTLD_NOLOAD);
  ASSERT_TRUE(handle == nullptr);
#ifdef __BIONIC__
  // TODO: glibc returns nullptr on dlerror() here. Is it bug?
  ASSERT_STREQ("dlopen failed: library \"libtest_check_order_reloc_root.so\" wasn't loaded and RTLD_NOLOAD prevented it", dlerror());
#endif

  handle = dlopen("libtest_check_order_reloc_root.so", RTLD_NOW | RTLD_LOCAL);
  ASSERT_TRUE(handle != nullptr) << dlerror();

  typedef int (*fn_t) (void);
  fn_t fn = reinterpret_cast<fn_t>(dlsym(handle, "check_order_reloc_root_get_answer"));
  ASSERT_TRUE(fn != nullptr) << dlerror();
  ASSERT_EQ(42, fn());

  ASSERT_EQ(0, dlclose(handle));
}

TEST(dlfcn, dlopen_check_rtld_local) {
  void* sym = dlsym(RTLD_DEFAULT, "dlopen_testlib_simple_func");
  ASSERT_TRUE(sym == nullptr);

  // implicit RTLD_LOCAL
  void* handle = dlopen("libtest_simple.so", RTLD_NOW);
  sym = dlsym(RTLD_DEFAULT, "dlopen_testlib_simple_func");
  ASSERT_TRUE(sym == nullptr);
  ASSERT_SUBSTR("undefined symbol: dlopen_testlib_simple_func", dlerror());
  sym = dlsym(handle, "dlopen_testlib_simple_func");
  ASSERT_TRUE(sym != nullptr);
  ASSERT_TRUE(reinterpret_cast<bool (*)(void)>(sym)());
  dlclose(handle);

  // explicit RTLD_LOCAL
  handle = dlopen("libtest_simple.so", RTLD_NOW | RTLD_LOCAL);
  sym = dlsym(RTLD_DEFAULT, "dlopen_testlib_simple_func");
  ASSERT_TRUE(sym == nullptr);
  ASSERT_SUBSTR("undefined symbol: dlopen_testlib_simple_func", dlerror());
  sym = dlsym(handle, "dlopen_testlib_simple_func");
  ASSERT_TRUE(sym != nullptr);
  ASSERT_TRUE(reinterpret_cast<bool (*)(void)>(sym)());
  dlclose(handle);
}

TEST(dlfcn, dlopen_check_rtld_global) {
  void* sym = dlsym(RTLD_DEFAULT, "dlopen_testlib_simple_func");
  ASSERT_TRUE(sym == nullptr);

  void* handle = dlopen("libtest_simple.so", RTLD_NOW | RTLD_GLOBAL);
  ASSERT_TRUE(handle != nullptr) << dlerror();
  sym = dlsym(RTLD_DEFAULT, "dlopen_testlib_simple_func");
  ASSERT_TRUE(sym != nullptr) << dlerror();
  ASSERT_TRUE(reinterpret_cast<bool (*)(void)>(sym)());
  dlclose(handle);

  // RTLD_GLOBAL implies RTLD_NODELETE, let's check that
  void* sym_after_dlclose = dlsym(RTLD_DEFAULT, "dlopen_testlib_simple_func");
  ASSERT_EQ(sym, sym_after_dlclose);

  // Check if dlsym() for main program's handle searches RTLD_GLOBAL
  // shared libraries after symbol was not found in the main executable
  // and dependent libraries.
  void* handle_for_main_executable = dlopen(nullptr, RTLD_NOW);
  sym = dlsym(handle_for_main_executable, "dlopen_testlib_simple_func");
  ASSERT_TRUE(sym != nullptr) << dlerror();

  dlclose(handle_for_main_executable);
}

// libtest_with_dependency_loop.so -> libtest_with_dependency_loop_a.so ->
// libtest_with_dependency_loop_b.so -> libtest_with_dependency_loop_c.so ->
// libtest_with_dependency_loop_a.so
TEST(dlfcn, dlopen_check_loop) {
  void* handle = dlopen("libtest_with_dependency_loop.so", RTLD_NOW);
  ASSERT_TRUE(handle != nullptr) << dlerror();
  void* f = dlsym(handle, "dlopen_test_loopy_function");
  ASSERT_TRUE(f != nullptr) << dlerror();
  EXPECT_TRUE(reinterpret_cast<bool (*)(void)>(f)());
  ASSERT_EQ(0, dlclose(handle));

  // dlopen second time to make sure that the library was unloaded correctly
  handle = dlopen("libtest_with_dependency_loop.so", RTLD_NOW | RTLD_NOLOAD);
  ASSERT_TRUE(handle == nullptr);
#ifdef __BIONIC__
  ASSERT_STREQ("dlopen failed: library \"libtest_with_dependency_loop.so\" wasn't loaded and RTLD_NOLOAD prevented it", dlerror());
#else
  // TODO: glibc returns nullptr on dlerror() here. Is it bug?
  ASSERT_TRUE(dlerror() == nullptr);
#endif

  handle = dlopen("libtest_with_dependency_a.so", RTLD_NOW | RTLD_NOLOAD);
  ASSERT_TRUE(handle == nullptr);
}

TEST(dlfcn, dlopen_nodelete) {
  static bool is_unloaded = false;

  void* handle = dlopen("libtest_nodelete_1.so", RTLD_NOW | RTLD_NODELETE);
  ASSERT_TRUE(handle != nullptr) << dlerror();
  void (*set_unload_flag_ptr)(bool*);
  set_unload_flag_ptr = reinterpret_cast<void (*)(bool*)>(dlsym(handle, "dlopen_nodelete_1_set_unload_flag_ptr"));
  ASSERT_TRUE(set_unload_flag_ptr != nullptr) << dlerror();
  set_unload_flag_ptr(&is_unloaded);

  uint32_t* taxicab_number = reinterpret_cast<uint32_t*>(dlsym(handle, "dlopen_nodelete_1_taxicab_number"));
  ASSERT_TRUE(taxicab_number != nullptr) << dlerror();
  ASSERT_EQ(1729U, *taxicab_number);
  *taxicab_number = 2;

  dlclose(handle);
  ASSERT_TRUE(!is_unloaded);

  uint32_t* taxicab_number_after_dlclose = reinterpret_cast<uint32_t*>(dlsym(handle, "dlopen_nodelete_1_taxicab_number"));
  ASSERT_EQ(taxicab_number_after_dlclose, taxicab_number);
  ASSERT_EQ(2U, *taxicab_number_after_dlclose);


  handle = dlopen("libtest_nodelete_1.so", RTLD_NOW);
  uint32_t* taxicab_number2 = reinterpret_cast<uint32_t*>(dlsym(handle, "dlopen_nodelete_1_taxicab_number"));
  ASSERT_EQ(taxicab_number2, taxicab_number);

  ASSERT_EQ(2U, *taxicab_number2);

  dlclose(handle);
  ASSERT_TRUE(!is_unloaded);
}

TEST(dlfcn, dlopen_nodelete_on_second_dlopen) {
  static bool is_unloaded = false;

  void* handle = dlopen("libtest_nodelete_2.so", RTLD_NOW);
  ASSERT_TRUE(handle != nullptr) << dlerror();
  void (*set_unload_flag_ptr)(bool*);
  set_unload_flag_ptr = reinterpret_cast<void (*)(bool*)>(dlsym(handle, "dlopen_nodelete_2_set_unload_flag_ptr"));
  ASSERT_TRUE(set_unload_flag_ptr != nullptr) << dlerror();
  set_unload_flag_ptr(&is_unloaded);

  uint32_t* taxicab_number = reinterpret_cast<uint32_t*>(dlsym(handle, "dlopen_nodelete_2_taxicab_number"));
  ASSERT_TRUE(taxicab_number != nullptr) << dlerror();

  ASSERT_EQ(1729U, *taxicab_number);
  *taxicab_number = 2;

  // This RTLD_NODELETE should be ignored
  void* handle1 = dlopen("libtest_nodelete_2.so", RTLD_NOW | RTLD_NODELETE);
  ASSERT_TRUE(handle1 != nullptr) << dlerror();
  ASSERT_EQ(handle, handle1);

  dlclose(handle1);
  dlclose(handle);

  ASSERT_TRUE(is_unloaded);
}

TEST(dlfcn, dlopen_nodelete_dt_flags_1) {
  static bool is_unloaded = false;

  void* handle = dlopen("libtest_nodelete_dt_flags_1.so", RTLD_NOW);
  ASSERT_TRUE(handle != nullptr) << dlerror();
  void (*set_unload_flag_ptr)(bool*);
  set_unload_flag_ptr = reinterpret_cast<void (*)(bool*)>(dlsym(handle, "dlopen_nodelete_dt_flags_1_set_unload_flag_ptr"));
  ASSERT_TRUE(set_unload_flag_ptr != nullptr) << dlerror();
  set_unload_flag_ptr(&is_unloaded);

  dlclose(handle);
  ASSERT_TRUE(!is_unloaded);
}

TEST(dlfcn, dlsym_df_1_global) {
  void* handle = dlopen("libtest_dlsym_df_1_global.so", RTLD_NOW);
  ASSERT_TRUE(handle != nullptr) << dlerror();
  int (*get_answer)();
  get_answer = reinterpret_cast<int (*)()>(dlsym(handle, "dl_df_1_global_get_answer"));
  ASSERT_TRUE(get_answer != nullptr) << dlerror();
  ASSERT_EQ(42, get_answer());
  ASSERT_EQ(0, dlclose(handle));
}

TEST(dlfcn, dlopen_failure) {
  void* self = dlopen("/does/not/exist", RTLD_NOW);
  ASSERT_TRUE(self == nullptr);
#if defined(__BIONIC__)
  ASSERT_STREQ("dlopen failed: library \"/does/not/exist\" not found", dlerror());
#else
  ASSERT_STREQ("/does/not/exist: cannot open shared object file: No such file or directory", dlerror());
#endif
}

TEST(dlfcn, dlclose_unload) {
  void* handle = dlopen("libtest_simple.so", RTLD_NOW);
  ASSERT_TRUE(handle != nullptr) << dlerror();
  uint32_t* taxicab_number = static_cast<uint32_t*>(dlsym(handle, "dlopen_testlib_taxicab_number"));
  ASSERT_TRUE(taxicab_number != nullptr) << dlerror();
  EXPECT_EQ(1729U, *taxicab_number);
  dlclose(handle);
  // Making sure that the library has been unmapped as part of library unload
  // process. Note that mprotect somewhat counter-intuitively returns ENOMEM in
  // this case.
  uintptr_t page_start = reinterpret_cast<uintptr_t>(taxicab_number) & ~(PAGE_SIZE - 1);
  ASSERT_TRUE(mprotect(reinterpret_cast<void*>(page_start), PAGE_SIZE, PROT_NONE) != 0);
  ASSERT_EQ(ENOMEM, errno) << strerror(errno);
}

static void ConcurrentDlErrorFn(std::string& error) {
  ASSERT_TRUE(dlerror() == nullptr);

  void* handle = dlopen("/child/thread", RTLD_NOW);
  ASSERT_TRUE(handle == nullptr);

  const char* err = dlerror();
  ASSERT_TRUE(err != nullptr);

  error = err;
}

TEST(dlfcn, dlerror_concurrent_buffer) {
  void* handle = dlopen("/main/thread", RTLD_NOW);
  ASSERT_TRUE(handle == nullptr);
  const char* main_thread_error = dlerror();
  ASSERT_TRUE(main_thread_error != nullptr);
  ASSERT_SUBSTR("/main/thread", main_thread_error);

  std::string child_thread_error;
  std::thread t(ConcurrentDlErrorFn, std::ref(child_thread_error));
  t.join();
  ASSERT_SUBSTR("/child/thread", child_thread_error.c_str());

  // Check that main thread local buffer was not modified.
  ASSERT_SUBSTR("/main/thread", main_thread_error);
}

TEST(dlfcn, dlerror_concurrent) {
  void* handle = dlopen("/main/thread", RTLD_NOW);
  ASSERT_TRUE(handle == nullptr);

  std::string child_thread_error;
  std::thread t(ConcurrentDlErrorFn, std::ref(child_thread_error));
  t.join();
  ASSERT_SUBSTR("/child/thread", child_thread_error.c_str());

  const char* main_thread_error = dlerror();
  ASSERT_TRUE(main_thread_error != nullptr);
  ASSERT_SUBSTR("/main/thread", main_thread_error);
}

TEST(dlfcn, dlsym_failures) {
  dlerror(); // Clear any pending errors.
  void* self = dlopen(nullptr, RTLD_NOW);
  ASSERT_TRUE(self != nullptr);
  ASSERT_TRUE(dlerror() == nullptr);

  void* sym;

#if defined(__BIONIC__) && !defined(__LP64__)
  // RTLD_DEFAULT in lp32 bionic is not (void*)0
  // so it can be distinguished from the NULL handle.
  sym = dlsym(nullptr, "test");
  ASSERT_TRUE(sym == nullptr);
  ASSERT_STREQ("dlsym failed: library handle is null", dlerror());
#endif

  // Symbol that doesn't exist.
  sym = dlsym(self, "ThisSymbolDoesNotExist");
  ASSERT_TRUE(sym == nullptr);
  ASSERT_SUBSTR("undefined symbol: ThisSymbolDoesNotExist", dlerror());

  ASSERT_EQ(0, dlclose(self));
}

TEST(dlfcn, dladdr_executable) {
  dlerror(); // Clear any pending errors.
  void* self = dlopen(nullptr, RTLD_NOW);
  ASSERT_TRUE(self != nullptr);
  ASSERT_TRUE(dlerror() == nullptr);

  void* sym = dlsym(self, "DlSymTestFunction");
  ASSERT_TRUE(sym != nullptr);

  // Deliberately ask dladdr for an address inside a symbol, rather than the symbol base address.
  void* addr = reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(sym) + 2);

  Dl_info info;
  int rc = dladdr(addr, &info);
  ASSERT_NE(rc, 0); // Zero on error, non-zero on success.

  // Get the name of this executable.
  const std::string executable_path = android::base::GetExecutablePath();

  // The filename should be that of this executable.
  char dli_realpath[PATH_MAX];
  ASSERT_TRUE(realpath(info.dli_fname, dli_realpath) != nullptr);
  ASSERT_STREQ(executable_path.c_str(), dli_realpath);

  // The symbol name should be the symbol we looked up.
  ASSERT_STREQ(info.dli_sname, "DlSymTestFunction");

  // The address should be the exact address of the symbol.
  ASSERT_EQ(info.dli_saddr, sym);

  std::vector<map_record> maps;
  ASSERT_TRUE(Maps::parse_maps(&maps));

  void* base_address = nullptr;
  for (const map_record& rec : maps) {
    if (executable_path == rec.pathname) {
      base_address = reinterpret_cast<void*>(rec.addr_start);
      break;
    }
  }

  // The base address should be the address we were loaded at.
  ASSERT_EQ(info.dli_fbase, base_address);

  ASSERT_EQ(0, dlclose(self));
}

TEST(dlfcn, dlopen_executable_by_absolute_path) {
  void* handle1 = dlopen(nullptr, RTLD_NOW);
  ASSERT_TRUE(handle1 != nullptr) << dlerror();

  void* handle2 = dlopen(android::base::GetExecutablePath().c_str(), RTLD_NOW);
  ASSERT_TRUE(handle2 != nullptr) << dlerror();

#if defined(__BIONIC__)
  ASSERT_EQ(handle1, handle2);
#else
  GTEST_LOG_(INFO) << "Skipping ASSERT_EQ(handle1, handle2) for glibc: "
                      "it loads a separate copy of the main executable "
                      "on dlopen by absolute path.";
#endif
}

#if defined (__aarch64__)
#define ALTERNATE_PATH_TO_SYSTEM_LIB "/system/lib/arm64/"
#elif defined (__arm__)
#define ALTERNATE_PATH_TO_SYSTEM_LIB "/system/lib/arm/"
#elif defined (__i386__)
#define ALTERNATE_PATH_TO_SYSTEM_LIB "/system/lib/x86/"
#elif defined (__x86_64__)
#define ALTERNATE_PATH_TO_SYSTEM_LIB "/system/lib/x86_64/"
#elif defined (__mips__)
#if defined(__LP64__)
#define ALTERNATE_PATH_TO_SYSTEM_LIB "/system/lib/mips64/"
#else
#define ALTERNATE_PATH_TO_SYSTEM_LIB "/system/lib/mips/"
#endif
#else
#error "Unknown architecture"
#endif
#define PATH_TO_LIBC PATH_TO_SYSTEM_LIB "libc.so"
#define ALTERNATE_PATH_TO_LIBC ALTERNATE_PATH_TO_SYSTEM_LIB "libc.so"

TEST(dlfcn, dladdr_libc) {
#if defined(__BIONIC__)
  Dl_info info;
  void* addr = reinterpret_cast<void*>(puts); // well-known libc function
  ASSERT_TRUE(dladdr(addr, &info) != 0);

  char libc_realpath[PATH_MAX];

  // Check if libc is in canonical path or in alternate path.
  if (strncmp(ALTERNATE_PATH_TO_SYSTEM_LIB,
              info.dli_fname,
              sizeof(ALTERNATE_PATH_TO_SYSTEM_LIB) - 1) == 0) {
    // Platform with emulated architecture.  Symlink on ARC++.
    ASSERT_TRUE(realpath(ALTERNATE_PATH_TO_LIBC, libc_realpath) == libc_realpath);
  } else {
    // /system/lib is symlink when this test is executed on host.
    ASSERT_TRUE(realpath(PATH_TO_LIBC, libc_realpath) == libc_realpath);
  }

  ASSERT_STREQ(libc_realpath, info.dli_fname);
  // TODO: add check for dfi_fbase
  ASSERT_STREQ("puts", info.dli_sname);
  ASSERT_EQ(addr, info.dli_saddr);
#else
  GTEST_LOG_(INFO) << "This test does nothing for glibc. Glibc returns path from ldconfig "
      "for libc.so, which is symlink itself (not a realpath).\n";
#endif
}

TEST(dlfcn, dladdr_invalid) {
  Dl_info info;

  dlerror(); // Clear any pending errors.

  // No symbol corresponding to NULL.
  ASSERT_EQ(dladdr(nullptr, &info), 0); // Zero on error, non-zero on success.
  ASSERT_TRUE(dlerror() == nullptr); // dladdr(3) doesn't set dlerror(3).

  // No symbol corresponding to a stack address.
  ASSERT_EQ(dladdr(&info, &info), 0); // Zero on error, non-zero on success.
  ASSERT_TRUE(dlerror() == nullptr); // dladdr(3) doesn't set dlerror(3).
}

// GNU-style ELF hash tables are incompatible with the MIPS ABI.
// MIPS requires .dynsym to be sorted to match the GOT but GNU-style requires sorting by hash code.
TEST(dlfcn, dlopen_library_with_only_gnu_hash) {
#if !defined(__mips__)
  dlerror(); // Clear any pending errors.
  void* handle = dlopen("libgnu-hash-table-library.so", RTLD_NOW);
  ASSERT_TRUE(handle != nullptr) << dlerror();
  auto guard = android::base::make_scope_guard([&]() { dlclose(handle); });
  void* sym = dlsym(handle, "getRandomNumber");
  ASSERT_TRUE(sym != nullptr) << dlerror();
  int (*fn)(void);
  fn = reinterpret_cast<int (*)(void)>(sym);
  EXPECT_EQ(4, fn());

  Dl_info dlinfo;
  ASSERT_TRUE(0 != dladdr(reinterpret_cast<void*>(fn), &dlinfo));

  ASSERT_TRUE(fn == dlinfo.dli_saddr);
  ASSERT_STREQ("getRandomNumber", dlinfo.dli_sname);
  ASSERT_SUBSTR("libgnu-hash-table-library.so", dlinfo.dli_fname);
#else
  GTEST_LOG_(INFO) << "This test does nothing for mips/mips64; mips toolchain does not support '--hash-style=gnu'\n";
#endif
}

TEST(dlfcn, dlopen_library_with_only_sysv_hash) {
  void* handle = dlopen("libsysv-hash-table-library.so", RTLD_NOW);
  ASSERT_TRUE(handle != nullptr) << dlerror();
  auto guard = android::base::make_scope_guard([&]() { dlclose(handle); });
  void* sym = dlsym(handle, "getRandomNumber");
  ASSERT_TRUE(sym != nullptr) << dlerror();
  int (*fn)(void);
  fn = reinterpret_cast<int (*)(void)>(sym);
  EXPECT_EQ(4, fn());

  Dl_info dlinfo;
  ASSERT_TRUE(0 != dladdr(reinterpret_cast<void*>(fn), &dlinfo));

  ASSERT_TRUE(fn == dlinfo.dli_saddr);
  ASSERT_STREQ("getRandomNumber", dlinfo.dli_sname);
  ASSERT_SUBSTR("libsysv-hash-table-library.so", dlinfo.dli_fname);
}

TEST(dlfcn, dlopen_bad_flags) {
  dlerror(); // Clear any pending errors.
  void* handle;

#if defined(__GLIBC__)
  // glibc was smart enough not to define RTLD_NOW as 0, so it can detect missing flags.
  handle = dlopen(nullptr, 0);
  ASSERT_TRUE(handle == nullptr);
  ASSERT_SUBSTR("invalid", dlerror());
#endif

  handle = dlopen(nullptr, 0xffffffff);
  ASSERT_TRUE(handle == nullptr);
  ASSERT_SUBSTR("invalid", dlerror());

  // glibc actually allows you to choose both RTLD_NOW and RTLD_LAZY at the same time, and so do we.
  handle = dlopen(nullptr, RTLD_NOW|RTLD_LAZY);
  ASSERT_TRUE(handle != nullptr);
  ASSERT_SUBSTR(nullptr, dlerror());
}

TEST(dlfcn, rtld_default_unknown_symbol) {
  void* addr = dlsym(RTLD_DEFAULT, "ANY_UNKNOWN_SYMBOL_NAME");
  ASSERT_TRUE(addr == nullptr);
}

TEST(dlfcn, rtld_default_known_symbol) {
  void* addr = dlsym(RTLD_DEFAULT, "fopen");
  ASSERT_TRUE(addr != nullptr);
}

TEST(dlfcn, rtld_next_unknown_symbol) {
  void* addr = dlsym(RTLD_NEXT, "ANY_UNKNOWN_SYMBOL_NAME");
  ASSERT_TRUE(addr == nullptr);
}

TEST(dlfcn, rtld_next_known_symbol) {
  void* addr = dlsym(RTLD_NEXT, "fopen");
  ASSERT_TRUE(addr != nullptr);
}

// Check that RTLD_NEXT of a libc symbol works in dlopened library
TEST(dlfcn, rtld_next_from_library) {
  void* library_with_fclose = dlopen("libtest_check_rtld_next_from_library.so", RTLD_NOW | RTLD_GLOBAL);
  ASSERT_TRUE(library_with_fclose != nullptr) << dlerror();
  void* expected_addr = dlsym(RTLD_DEFAULT, "fclose");
  ASSERT_TRUE(expected_addr != nullptr) << dlerror();
  typedef void* (*get_libc_fclose_ptr_fn_t)();
  get_libc_fclose_ptr_fn_t get_libc_fclose_ptr =
      reinterpret_cast<get_libc_fclose_ptr_fn_t>(dlsym(library_with_fclose, "get_libc_fclose_ptr"));
  ASSERT_TRUE(get_libc_fclose_ptr != nullptr) << dlerror();
  ASSERT_EQ(expected_addr, get_libc_fclose_ptr());

  dlclose(library_with_fclose);
}


TEST(dlfcn, dlsym_weak_func) {
  dlerror();
  void* handle = dlopen("libtest_dlsym_weak_func.so", RTLD_NOW);
  ASSERT_TRUE(handle != nullptr);

  int (*weak_func)();
  weak_func = reinterpret_cast<int (*)()>(dlsym(handle, "weak_func"));
  ASSERT_TRUE(weak_func != nullptr) << "dlerror: " << dlerror();
  EXPECT_EQ(42, weak_func());
  dlclose(handle);
}

TEST(dlfcn, dlopen_undefined_weak_func) {
  void* handle = dlopen("libtest_dlopen_weak_undefined_func.so", RTLD_NOW);
  ASSERT_TRUE(handle != nullptr) << dlerror();
  int (*weak_func)();
  weak_func = reinterpret_cast<int (*)()>(dlsym(handle, "use_weak_undefined_func"));
  ASSERT_TRUE(weak_func != nullptr) << dlerror();
  EXPECT_EQ(6551, weak_func());
  dlclose(handle);
}

TEST(dlfcn, dlopen_symlink) {
  DlfcnSymlink symlink("dlopen_symlink");
  const std::string symlink_name = basename(symlink.get_symlink_path().c_str());
  void* handle1 = dlopen("libdlext_test.so", RTLD_NOW);
  void* handle2 = dlopen(symlink_name.c_str(), RTLD_NOW);
  ASSERT_TRUE(handle1 != nullptr);
  ASSERT_TRUE(handle2 != nullptr);
  ASSERT_EQ(handle1, handle2);
  dlclose(handle1);
  dlclose(handle2);
}

// libtest_dlopen_from_ctor_main.so depends on
// libtest_dlopen_from_ctor.so which has a constructor
// that calls dlopen(libc...). This is to test the situation
// described in b/7941716.
TEST(dlfcn, dlopen_dlopen_from_ctor) {
#if defined(__BIONIC__)
  void* handle = dlopen("libtest_dlopen_from_ctor_main.so", RTLD_NOW);
  ASSERT_TRUE(handle != nullptr) << dlerror();
  dlclose(handle);
#else
  GTEST_LOG_(INFO) << "This test is disabled for glibc (glibc segfaults if you try to call dlopen from a constructor).\n";
#endif
}

static std::string g_fini_call_order_str;

static void register_fini_call(const char* s) {
  g_fini_call_order_str += s;
}

static void test_init_fini_call_order_for(const char* libname) {
  g_fini_call_order_str.clear();
  void* handle = dlopen(libname, RTLD_NOW);
  ASSERT_TRUE(handle != nullptr) << dlerror();
  typedef int (*get_init_order_number_t)();
  get_init_order_number_t get_init_order_number =
          reinterpret_cast<get_init_order_number_t>(dlsym(handle, "get_init_order_number"));
  ASSERT_EQ(321, get_init_order_number());

  typedef void (*set_fini_callback_t)(void (*f)(const char*));
  set_fini_callback_t set_fini_callback =
          reinterpret_cast<set_fini_callback_t>(dlsym(handle, "set_fini_callback"));
  set_fini_callback(register_fini_call);
  dlclose(handle);
  ASSERT_EQ("(root)(child)(grandchild)", g_fini_call_order_str);
}

TEST(dlfcn, init_fini_call_order) {
  test_init_fini_call_order_for("libtest_init_fini_order_root.so");
  test_init_fini_call_order_for("libtest_init_fini_order_root2.so");
}

TEST(dlfcn, symbol_versioning_use_v1) {
  void* handle = dlopen("libtest_versioned_uselibv1.so", RTLD_NOW);
  ASSERT_TRUE(handle != nullptr) << dlerror();
  typedef int (*fn_t)();
  fn_t fn = reinterpret_cast<fn_t>(dlsym(handle, "get_function_version"));
  ASSERT_TRUE(fn != nullptr) << dlerror();
  ASSERT_EQ(1, fn());
  dlclose(handle);
}

TEST(dlfcn, symbol_versioning_use_v2) {
  void* handle = dlopen("libtest_versioned_uselibv2.so", RTLD_NOW);
  ASSERT_TRUE(handle != nullptr) << dlerror();
  typedef int (*fn_t)();
  fn_t fn = reinterpret_cast<fn_t>(dlsym(handle, "get_function_version"));
  ASSERT_TRUE(fn != nullptr) << dlerror();
  ASSERT_EQ(2, fn());
  dlclose(handle);
}

TEST(dlfcn, symbol_versioning_use_other_v2) {
  void* handle = dlopen("libtest_versioned_uselibv2_other.so", RTLD_NOW);
  ASSERT_TRUE(handle != nullptr) << dlerror();
  typedef int (*fn_t)();
  fn_t fn = reinterpret_cast<fn_t>(dlsym(handle, "get_function_version"));
  ASSERT_TRUE(fn != nullptr) << dlerror();
  ASSERT_EQ(20, fn());
  dlclose(handle);
}

TEST(dlfcn, symbol_versioning_use_other_v3) {
  void* handle = dlopen("libtest_versioned_uselibv3_other.so", RTLD_NOW);
  ASSERT_TRUE(handle != nullptr) << dlerror();
  typedef int (*fn_t)();
  fn_t fn = reinterpret_cast<fn_t>(dlsym(handle, "get_function_version"));
  ASSERT_TRUE(fn != nullptr) << dlerror();
  ASSERT_EQ(3, fn());
  dlclose(handle);
}

TEST(dlfcn, symbol_versioning_default_via_dlsym) {
  void* handle = dlopen("libtest_versioned_lib.so", RTLD_NOW);
  ASSERT_TRUE(handle != nullptr) << dlerror();
  typedef int (*fn_t)();
  fn_t fn = reinterpret_cast<fn_t>(dlsym(handle, "versioned_function"));
  ASSERT_TRUE(fn != nullptr) << dlerror();
  ASSERT_EQ(3, fn()); // the default version is 3
  dlclose(handle);
}

TEST(dlfcn, dlvsym_smoke) {
  void* handle = dlopen("libtest_versioned_lib.so", RTLD_NOW);
  ASSERT_TRUE(handle != nullptr) << dlerror();
  typedef int (*fn_t)();

  {
    fn_t fn = reinterpret_cast<fn_t>(dlvsym(handle, "versioned_function", "nonversion"));
    ASSERT_TRUE(fn == nullptr);
    ASSERT_SUBSTR("undefined symbol: versioned_function, version nonversion", dlerror());
  }

  {
    fn_t fn = reinterpret_cast<fn_t>(dlvsym(handle, "versioned_function", "TESTLIB_V2"));
    ASSERT_TRUE(fn != nullptr) << dlerror();
    ASSERT_EQ(2, fn());
  }

  dlclose(handle);
}

// This preempts the implementation from libtest_versioned_lib.so
extern "C" int version_zero_function() {
  return 0;
}

// This preempts the implementation from libtest_versioned_uselibv*.so
extern "C" int version_zero_function2() {
  return 0;
}

TEST(dlfcn, dt_runpath_smoke) {
  void* handle = dlopen("libtest_dt_runpath_d.so", RTLD_NOW);
  ASSERT_TRUE(handle != nullptr) << dlerror();

  typedef void *(* dlopen_b_fn)();
  dlopen_b_fn fn = (dlopen_b_fn)dlsym(handle, "dlopen_b");
  ASSERT_TRUE(fn != nullptr) << dlerror();

  void *p = fn();
  ASSERT_TRUE(p != nullptr);

  dlclose(handle);
}

TEST(dlfcn, dt_runpath_absolute_path) {
  std::string libpath = GetTestlibRoot() + "/libtest_dt_runpath_d.so";
  void* handle = dlopen(libpath.c_str(), RTLD_NOW);
  ASSERT_TRUE(handle != nullptr) << dlerror();

  typedef void *(* dlopen_b_fn)();
  dlopen_b_fn fn = (dlopen_b_fn)dlsym(handle, "dlopen_b");
  ASSERT_TRUE(fn != nullptr) << dlerror();

  void *p = fn();
  ASSERT_TRUE(p != nullptr);

  dlclose(handle);
}

static void test_dlclose_after_thread_local_dtor(const char* library_name) {
  bool is_dtor_triggered = false;

  auto f = [](void* handle, bool* is_dtor_triggered) {
    typedef void (*fn_t)(bool*);
    fn_t fn = reinterpret_cast<fn_t>(dlsym(handle, "init_thread_local_variable"));
    ASSERT_TRUE(fn != nullptr) << dlerror();

    fn(is_dtor_triggered);

    ASSERT_TRUE(!*is_dtor_triggered);
  };

  void* handle = dlopen(library_name, RTLD_NOW | RTLD_NOLOAD);
  ASSERT_TRUE(handle == nullptr);

  handle = dlopen(library_name, RTLD_NOW);
  ASSERT_TRUE(handle != nullptr) << dlerror();

  std::thread t(f, handle, &is_dtor_triggered);
  t.join();

  ASSERT_TRUE(is_dtor_triggered);
  dlclose(handle);

  handle = dlopen(library_name, RTLD_NOW | RTLD_NOLOAD);
  ASSERT_TRUE(handle == nullptr);
}

TEST(dlfcn, dlclose_after_thread_local_dtor) {
  test_dlclose_after_thread_local_dtor("libtest_thread_local_dtor.so");
}

TEST(dlfcn, dlclose_after_thread_local_dtor_indirect) {
  test_dlclose_after_thread_local_dtor("libtest_indirect_thread_local_dtor.so");
}

static void test_dlclose_before_thread_local_dtor(const char* library_name) {
  bool is_dtor_triggered = false;

  auto f = [library_name](bool* is_dtor_triggered) {
    void* handle = dlopen(library_name, RTLD_NOW | RTLD_NOLOAD);
    ASSERT_TRUE(handle == nullptr);

    handle = dlopen(library_name, RTLD_NOW);
    ASSERT_TRUE(handle != nullptr) << dlerror();

    typedef void (*fn_t)(bool*);
    fn_t fn = reinterpret_cast<fn_t>(dlsym(handle, "init_thread_local_variable"));
    ASSERT_TRUE(fn != nullptr) << dlerror();

    fn(is_dtor_triggered);

    dlclose(handle);

    ASSERT_TRUE(!*is_dtor_triggered);

    // Since we have thread_atexit dtors associated with handle - the library should
    // still be availabe.
    handle = dlopen(library_name, RTLD_NOW | RTLD_NOLOAD);
    ASSERT_TRUE(handle != nullptr) << dlerror();
    dlclose(handle);
  };

  void* handle = dlopen(library_name, RTLD_NOW);
  ASSERT_TRUE(handle != nullptr) << dlerror();
  dlclose(handle);

  handle = dlopen(library_name, RTLD_NOW | RTLD_NOLOAD);
  ASSERT_TRUE(handle == nullptr);

  std::thread t(f, &is_dtor_triggered);
  t.join();
#if defined(__BIONIC__)
  // ld-android.so unloads unreferenced libraries on pthread_exit()
  ASSERT_TRUE(is_dtor_triggered);
  handle = dlopen(library_name, RTLD_NOW | RTLD_NOLOAD);
  ASSERT_TRUE(handle == nullptr);
#else
  // GLIBC does not unload libraries with ref_count = 0 on pthread_exit
  ASSERT_TRUE(is_dtor_triggered);
  handle = dlopen(library_name, RTLD_NOW | RTLD_NOLOAD);
  ASSERT_TRUE(handle != nullptr) << dlerror();
#endif
}

TEST(dlfcn, dlclose_before_thread_local_dtor) {
  test_dlclose_before_thread_local_dtor("libtest_thread_local_dtor.so");
}

TEST(dlfcn, dlclose_before_thread_local_dtor_indirect) {
  test_dlclose_before_thread_local_dtor("libtest_indirect_thread_local_dtor.so");
}

TEST(dlfcn, dlclose_before_thread_local_dtor_multiple_dsos) {
  const constexpr char* library_name = "libtest_indirect_thread_local_dtor.so";

  bool is_dtor1_triggered = false;
  bool is_dtor2_triggered = false;

  std::mutex mtx;
  std::condition_variable cv;
  void* library_handle = nullptr;
  bool thread1_dlopen_complete = false;
  bool thread2_thread_local_dtor_initialized = false;
  bool thread1_complete = false;

  auto f1 = [&]() {
    void* handle = dlopen(library_name, RTLD_NOW | RTLD_NOLOAD);
    ASSERT_TRUE(handle == nullptr);

    handle = dlopen(library_name, RTLD_NOW);
    ASSERT_TRUE(handle != nullptr) << dlerror();
    std::unique_lock<std::mutex> lock(mtx);
    thread1_dlopen_complete = true;
    library_handle = handle;
    lock.unlock();
    cv.notify_one();

    typedef void (*fn_t)(bool*);
    fn_t fn = reinterpret_cast<fn_t>(dlsym(handle, "init_thread_local_variable"));
    ASSERT_TRUE(fn != nullptr) << dlerror();

    fn(&is_dtor1_triggered);

    lock.lock();
    cv.wait(lock, [&] { return thread2_thread_local_dtor_initialized; });
    lock.unlock();

    dlclose(handle);

    ASSERT_TRUE(!is_dtor1_triggered);

    // Since we have thread_atexit dtors associated with handle - the library should
    // still be availabe.
    handle = dlopen(library_name, RTLD_NOW | RTLD_NOLOAD);
    ASSERT_TRUE(handle != nullptr) << dlerror();
    dlclose(handle);
  };

  auto f2 = [&]() {
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, [&] { return thread1_dlopen_complete; });
    void* handle = library_handle;
    lock.unlock();

    typedef void (*fn_t)(bool*);
    fn_t fn = reinterpret_cast<fn_t>(dlsym(handle, "init_thread_local_variable2"));
    ASSERT_TRUE(fn != nullptr) << dlerror();

    fn(&is_dtor2_triggered);

    lock.lock();
    thread2_thread_local_dtor_initialized = true;
    lock.unlock();
    cv.notify_one();

    lock.lock();
    cv.wait(lock, [&] { return thread1_complete; });
    lock.unlock();

    ASSERT_TRUE(!is_dtor2_triggered);
  };

  void* handle = dlopen(library_name, RTLD_NOW);
  ASSERT_TRUE(handle != nullptr) << dlerror();
  dlclose(handle);

  handle = dlopen(library_name, RTLD_NOW | RTLD_NOLOAD);
  ASSERT_TRUE(handle == nullptr);

  std::thread t1(f1);
  std::thread t2(f2);
  t1.join();
  ASSERT_TRUE(is_dtor1_triggered);
  ASSERT_TRUE(!is_dtor2_triggered);

  handle = dlopen(library_name, RTLD_NOW | RTLD_NOLOAD);
  ASSERT_TRUE(handle != nullptr) << dlerror();
  dlclose(handle);

  std::unique_lock<std::mutex> lock(mtx);
  thread1_complete = true;
  lock.unlock();
  cv.notify_one();

  t2.join();
  ASSERT_TRUE(is_dtor2_triggered);

#if defined(__BIONIC__)
  // ld-android.so unloads unreferenced libraries on pthread_exit()
  handle = dlopen(library_name, RTLD_NOW | RTLD_NOLOAD);
  ASSERT_TRUE(handle == nullptr);
#else
  // GLIBC does not unload libraries with ref_count = 0 on pthread_exit
  handle = dlopen(library_name, RTLD_NOW | RTLD_NOLOAD);
  ASSERT_TRUE(handle != nullptr) << dlerror();
#endif
}

TEST(dlfcn, RTLD_macros) {
#if !defined(RTLD_LOCAL)
#error no RTLD_LOCAL
#elif !defined(RTLD_LAZY)
#error no RTLD_LAZY
#elif !defined(RTLD_NOW)
#error no RTLD_NOW
#elif !defined(RTLD_NOLOAD)
#error no RTLD_NOLOAD
#elif !defined(RTLD_GLOBAL)
#error no RTLD_GLOBAL
#elif !defined(RTLD_NODELETE)
#error no RTLD_NODELETE
#endif
}

// Bionic specific tests
#if defined(__BIONIC__)

#if defined(__arm__)
const llvm::ELF::Elf32_Dyn* to_dynamic_table(const char* p) {
  return reinterpret_cast<const llvm::ELF::Elf32_Dyn*>(p);
}

// Duplicate these definitions here because they are android specific
//  - note that we cannot include <elf.h> because #defines conflict with
//    enum names provided by LLVM.
//  - we also don't use llvm::ELF::DT_LOOS because its value is 0x60000000
//    rather than the 0x6000000d we expect
#define DT_LOOS 0x6000000d
#define DT_ANDROID_REL (DT_LOOS + 2)
#define DT_ANDROID_RELA (DT_LOOS + 4)

template<typename ELFT>
void validate_compatibility_of_native_library(const std::string& soname,
                                              const std::string& path, ELFT* elf) {
  bool has_elf_hash = false;
  bool has_android_rel = false;
  bool has_rel = false;
  // Find dynamic section and check that DT_HASH and there is no DT_ANDROID_REL
  for (auto it = elf->section_begin(); it != elf->section_end(); ++it) {
    const llvm::object::ELFSectionRef& section_ref = *it;
    if (section_ref.getType() == llvm::ELF::SHT_DYNAMIC) {
      llvm::StringRef data;
      ASSERT_TRUE(!it->getContents(data)) << "unable to get SHT_DYNAMIC section data";
      for (auto d = to_dynamic_table(data.data()); d->d_tag != llvm::ELF::DT_NULL; ++d) {
        if (d->d_tag == llvm::ELF::DT_HASH) {
          has_elf_hash = true;
        } else if (d->d_tag == DT_ANDROID_REL || d->d_tag == DT_ANDROID_RELA) {
          has_android_rel = true;
        } else if (d->d_tag == llvm::ELF::DT_REL || d->d_tag == llvm::ELF::DT_RELA) {
          has_rel = true;
        }
      }

      break;
    }
  }

  ASSERT_TRUE(has_elf_hash) << path.c_str() << ": missing elf hash (DT_HASH)";
  ASSERT_TRUE(!has_android_rel) << path.c_str() << ": has packed relocations";
  // libdl.so is simple enough that it might not have any relocations, so
  // exempt it from the DT_REL/DT_RELA check.
  if (soname != "libdl.so") {
    ASSERT_TRUE(has_rel) << path.c_str() << ": missing DT_REL/DT_RELA";
  }
}

void validate_compatibility_of_native_library(const std::string& soname) {
  // On the systems with emulation system libraries would be of different
  // architecture.  Try to use alternate paths first.
  std::string path = std::string(ALTERNATE_PATH_TO_SYSTEM_LIB) + soname;
  auto binary_or_error = llvm::object::createBinary(path);
  if (!binary_or_error) {
    path = std::string(PATH_TO_SYSTEM_LIB) + soname;
    binary_or_error = llvm::object::createBinary(path);
  }
  ASSERT_FALSE(!binary_or_error);

  llvm::object::Binary* binary = binary_or_error.get().getBinary();

  auto obj = llvm::dyn_cast<llvm::object::ObjectFile>(binary);
  ASSERT_TRUE(obj != nullptr);

  auto elf = llvm::dyn_cast<llvm::object::ELF32LEObjectFile>(obj);

  ASSERT_TRUE(elf != nullptr);

  validate_compatibility_of_native_library(soname, path, elf);
}

// This is a test for app compatibility workaround for arm apps
// affected by http://b/24465209
TEST(dlext, compat_elf_hash_and_relocation_tables) {
  validate_compatibility_of_native_library("libc.so");
  validate_compatibility_of_native_library("liblog.so");
  validate_compatibility_of_native_library("libstdc++.so");
  validate_compatibility_of_native_library("libdl.so");
  validate_compatibility_of_native_library("libm.so");
  validate_compatibility_of_native_library("libz.so");
  validate_compatibility_of_native_library("libjnigraphics.so");
}

#endif //  defined(__arm__)

TEST(dlfcn, dlopen_invalid_rw_load_segment) {
  const std::string libpath = GetTestlibRoot() +
                              "/" + kPrebuiltElfDir +
                              "/libtest_invalid-rw_load_segment.so";
  void* handle = dlopen(libpath.c_str(), RTLD_NOW);
  ASSERT_TRUE(handle == nullptr);
  std::string expected_dlerror = std::string("dlopen failed: \"") + libpath + "\": W+E load segments are not allowed";
  ASSERT_STREQ(expected_dlerror.c_str(), dlerror());
}

TEST(dlfcn, dlopen_invalid_unaligned_shdr_offset) {
  const std::string libpath = GetTestlibRoot() +
                              "/" + kPrebuiltElfDir +
                              "/libtest_invalid-unaligned_shdr_offset.so";

  void* handle = dlopen(libpath.c_str(), RTLD_NOW);
  ASSERT_TRUE(handle == nullptr);
  std::string expected_dlerror = std::string("dlopen failed: \"") + libpath + "\" has invalid shdr offset/size: ";
  ASSERT_SUBSTR(expected_dlerror.c_str(), dlerror());
}

TEST(dlfcn, dlopen_invalid_zero_shentsize) {
  const std::string libpath = GetTestlibRoot() +
                              "/" + kPrebuiltElfDir +
                              "/libtest_invalid-zero_shentsize.so";

  void* handle = dlopen(libpath.c_str(), RTLD_NOW);
  ASSERT_TRUE(handle == nullptr);
  std::string expected_dlerror = std::string("dlopen failed: \"") + libpath + "\" has unsupported e_shentsize: 0x0 (expected 0x";
  ASSERT_SUBSTR(expected_dlerror.c_str(), dlerror());
}

TEST(dlfcn, dlopen_invalid_zero_shstrndx) {
  const std::string libpath = GetTestlibRoot() +
                              "/" + kPrebuiltElfDir +
                              "/libtest_invalid-zero_shstrndx.so";

  void* handle = dlopen(libpath.c_str(), RTLD_NOW);
  ASSERT_TRUE(handle == nullptr);
  std::string expected_dlerror = std::string("dlopen failed: \"") + libpath + "\" has invalid e_shstrndx";
  ASSERT_STREQ(expected_dlerror.c_str(), dlerror());
}

TEST(dlfcn, dlopen_invalid_empty_shdr_table) {
  const std::string libpath = GetTestlibRoot() +
                              "/" + kPrebuiltElfDir +
                              "/libtest_invalid-empty_shdr_table.so";

  void* handle = dlopen(libpath.c_str(), RTLD_NOW);
  ASSERT_TRUE(handle == nullptr);
  std::string expected_dlerror = std::string("dlopen failed: \"") + libpath + "\" has no section headers";
  ASSERT_STREQ(expected_dlerror.c_str(), dlerror());
}

TEST(dlfcn, dlopen_invalid_zero_shdr_table_offset) {
  const std::string libpath = GetTestlibRoot() +
                              "/" + kPrebuiltElfDir +
                              "/libtest_invalid-zero_shdr_table_offset.so";

  void* handle = dlopen(libpath.c_str(), RTLD_NOW);
  ASSERT_TRUE(handle == nullptr);
  std::string expected_dlerror = std::string("dlopen failed: \"") + libpath + "\" has invalid shdr offset/size: 0/";
  ASSERT_SUBSTR(expected_dlerror.c_str(), dlerror());
}

TEST(dlfcn, dlopen_invalid_zero_shdr_table_content) {
  const std::string libpath = GetTestlibRoot() +
                              "/" + kPrebuiltElfDir +
                              "/libtest_invalid-zero_shdr_table_content.so";

  void* handle = dlopen(libpath.c_str(), RTLD_NOW);
  ASSERT_TRUE(handle == nullptr);
  std::string expected_dlerror = std::string("dlopen failed: \"") + libpath + "\" .dynamic section header was not found";
  ASSERT_SUBSTR(expected_dlerror.c_str(), dlerror());
}

TEST(dlfcn, dlopen_invalid_textrels) {
  const std::string libpath = GetTestlibRoot() +
                              "/" + kPrebuiltElfDir +
                              "/libtest_invalid-textrels.so";

  void* handle = dlopen(libpath.c_str(), RTLD_NOW);
  ASSERT_TRUE(handle == nullptr);
  std::string expected_dlerror = std::string("dlopen failed: \"") + libpath + "\" has text relocations";
  ASSERT_SUBSTR(expected_dlerror.c_str(), dlerror());
}

TEST(dlfcn, dlopen_invalid_textrels2) {
  const std::string libpath = GetTestlibRoot() +
                              "/" + kPrebuiltElfDir +
                              "/libtest_invalid-textrels2.so";

  void* handle = dlopen(libpath.c_str(), RTLD_NOW);
  ASSERT_TRUE(handle == nullptr);
  std::string expected_dlerror = std::string("dlopen failed: \"") + libpath + "\" has text relocations";
  ASSERT_SUBSTR(expected_dlerror.c_str(), dlerror());
}

TEST(dlfcn, dlopen_df_1_global) {
  void* handle = dlopen("libtest_dlopen_df_1_global.so", RTLD_NOW);
  ASSERT_TRUE(handle != nullptr) << dlerror();
}

#endif
