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

#include "private/ScopeGuard.h"

#include <string>

#define ASSERT_SUBSTR(needle, haystack) \
    ASSERT_PRED_FORMAT2(::testing::IsSubstring, needle, haystack)

#if defined(__LP64__)
#define LIBPATH_PREFIX "/nativetest64/libdlext_test_fd/"
#else
#define LIBPATH_PREFIX "/nativetest/libdlext_test_fd/"
#endif

#define LIBZIPPATH LIBPATH_PREFIX "libdlext_test_fd_zipaligned.zip"

static bool g_called = false;
extern "C" void DlSymTestFunction() {
  g_called = true;
}

static int g_ctor_function_called = 0;

extern "C" void ctor_function() __attribute__ ((constructor));

extern "C" void ctor_function() {
  g_ctor_function_called = 17;
}

TEST(dlfcn, ctor_function_call) {
  ASSERT_EQ(17, g_ctor_function_called);
}

TEST(dlfcn, dlsym_in_self) {
  dlerror(); // Clear any pending errors.
  void* self = dlopen(NULL, RTLD_NOW);
  ASSERT_TRUE(self != NULL);
  ASSERT_TRUE(dlerror() == NULL);

  void* sym = dlsym(self, "DlSymTestFunction");
  ASSERT_TRUE(sym != NULL);

  void (*function)() = reinterpret_cast<void(*)()>(sym);

  g_called = false;
  function();
  ASSERT_TRUE(g_called);

  ASSERT_EQ(0, dlclose(self));
}

TEST(dlfcn, dlsym_with_dependencies) {
  void* handle = dlopen("libtest_with_dependency.so", RTLD_NOW);
  ASSERT_TRUE(handle != NULL);
  dlerror();
  // This symbol is in DT_NEEDED library.
  void* sym = dlsym(handle, "getRandomNumber");
  ASSERT_TRUE(sym != NULL);
  int (*fn)(void);
  fn = reinterpret_cast<int (*)(void)>(sym);
  EXPECT_EQ(4, fn());
  dlclose(handle);
}

TEST(dlfcn, dlopen_noload) {
  void* handle = dlopen("libtest_simple.so", RTLD_NOW | RTLD_NOLOAD);
  ASSERT_TRUE(handle == NULL);
  handle = dlopen("libtest_simple.so", RTLD_NOW);
  void* handle2 = dlopen("libtest_simple.so", RTLD_NOW | RTLD_NOLOAD);
  ASSERT_TRUE(handle != NULL);
  ASSERT_TRUE(handle2 != NULL);
  ASSERT_TRUE(handle == handle2);
  ASSERT_EQ(0, dlclose(handle));
  ASSERT_EQ(0, dlclose(handle2));
}

// ifuncs are only supported on intel and arm64 for now
#if defined (__aarch64__) || defined(__i386__) || defined(__x86_64__)
TEST(dlfcn, ifunc) {
  typedef const char* (*fn_ptr)();

  // ifunc's choice depends on whether IFUNC_CHOICE has a value
  // first check the set case
  setenv("IFUNC_CHOICE", "set", 1);
  void* handle = dlopen("libtest_ifunc.so", RTLD_NOW);
  ASSERT_TRUE(handle != NULL);
  fn_ptr foo_ptr = reinterpret_cast<fn_ptr>(dlsym(handle, "foo"));
  fn_ptr foo_library_ptr = reinterpret_cast<fn_ptr>(dlsym(handle, "foo_library"));
  ASSERT_TRUE(foo_ptr != NULL);
  ASSERT_TRUE(foo_library_ptr != NULL);
  ASSERT_EQ(strncmp("set", foo_ptr(), 3), 0);
  ASSERT_EQ(strncmp("set", foo_library_ptr(), 3), 0);
  dlclose(handle);

  // then check the unset case
  unsetenv("IFUNC_CHOICE");
  handle = dlopen("libtest_ifunc.so", RTLD_NOW);
  ASSERT_TRUE(handle != NULL);
  foo_ptr = reinterpret_cast<fn_ptr>(dlsym(handle, "foo"));
  foo_library_ptr = reinterpret_cast<fn_ptr>(dlsym(handle, "foo_library"));
  ASSERT_TRUE(foo_ptr != NULL);
  ASSERT_TRUE(foo_library_ptr != NULL);
  ASSERT_EQ(strncmp("unset", foo_ptr(), 5), 0);
  ASSERT_EQ(strncmp("unset", foo_library_ptr(), 3), 0);
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
  auto guard = make_scope_guard([&]() {
    dlclose(handle);
  });

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
  ASSERT_TRUE(fn != NULL) << dlerror();
  fn2 = reinterpret_cast<fn_t>(dlsym(RTLD_DEFAULT, "check_order_dlsym_get_answer2"));
  ASSERT_TRUE(fn2 != NULL) << dlerror();

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
  // TODO: glibc returns nullptr on dlerror() here. Is it bug?
  ASSERT_STREQ("dlopen failed: library \"libtest_with_dependency_loop.so\" wasn't loaded and RTLD_NOLOAD prevented it", dlerror());
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
#if !defined(__arm__) && !defined(__aarch64__)
  void* handle = dlopen("libtest_dlsym_df_1_global.so", RTLD_NOW);
  ASSERT_TRUE(handle != nullptr) << dlerror();
  int (*get_answer)();
  get_answer = reinterpret_cast<int (*)()>(dlsym(handle, "dl_df_1_global_get_answer"));
  ASSERT_TRUE(get_answer != nullptr) << dlerror();
  ASSERT_EQ(42, get_answer());
  ASSERT_EQ(0, dlclose(handle));
#else
  GTEST_LOG_(INFO) << "This test does nothing on arm/arm64 (to be reenabled once b/18137520 or b/18130452 are fixed).\n";
#endif
}

TEST(dlfcn, dlopen_failure) {
  void* self = dlopen("/does/not/exist", RTLD_NOW);
  ASSERT_TRUE(self == NULL);
#if defined(__BIONIC__)
  ASSERT_STREQ("dlopen failed: library \"/does/not/exist\" not found", dlerror());
#else
  ASSERT_STREQ("/does/not/exist: cannot open shared object file: No such file or directory", dlerror());
#endif
}

static void* ConcurrentDlErrorFn(void*) {
  dlopen("/child/thread", RTLD_NOW);
  return reinterpret_cast<void*>(strdup(dlerror()));
}

TEST(dlfcn, dlerror_concurrent) {
  dlopen("/main/thread", RTLD_NOW);
  const char* main_thread_error = dlerror();
  ASSERT_SUBSTR("/main/thread", main_thread_error);

  pthread_t t;
  ASSERT_EQ(0, pthread_create(&t, NULL, ConcurrentDlErrorFn, NULL));
  void* result;
  ASSERT_EQ(0, pthread_join(t, &result));
  char* child_thread_error = static_cast<char*>(result);
  ASSERT_SUBSTR("/child/thread", child_thread_error);
  free(child_thread_error);

  ASSERT_SUBSTR("/main/thread", main_thread_error);
}

TEST(dlfcn, dlsym_failures) {
  dlerror(); // Clear any pending errors.
  void* self = dlopen(NULL, RTLD_NOW);
  ASSERT_TRUE(self != NULL);
  ASSERT_TRUE(dlerror() == NULL);

  void* sym;

#if defined(__BIONIC__) && !defined(__LP64__)
  // RTLD_DEFAULT in lp32 bionic is not (void*)0
  // so it can be distinguished from the NULL handle.
  sym = dlsym(NULL, "test");
  ASSERT_TRUE(sym == NULL);
  ASSERT_SUBSTR("dlsym library handle is null", dlerror());
#endif

  // NULL symbol name.
#if defined(__BIONIC__)
  // glibc marks this parameter non-null and SEGVs if you cheat.
  sym = dlsym(self, NULL);
  ASSERT_TRUE(sym == NULL);
  ASSERT_SUBSTR("", dlerror());
#endif

  // Symbol that doesn't exist.
  sym = dlsym(self, "ThisSymbolDoesNotExist");
  ASSERT_TRUE(sym == NULL);
  ASSERT_SUBSTR("undefined symbol: ThisSymbolDoesNotExist", dlerror());

  ASSERT_EQ(0, dlclose(self));
}

TEST(dlfcn, dladdr) {
  dlerror(); // Clear any pending errors.
  void* self = dlopen(NULL, RTLD_NOW);
  ASSERT_TRUE(self != NULL);
  ASSERT_TRUE(dlerror() == NULL);

  void* sym = dlsym(self, "DlSymTestFunction");
  ASSERT_TRUE(sym != NULL);

  // Deliberately ask dladdr for an address inside a symbol, rather than the symbol base address.
  void* addr = reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(sym) + 2);

  Dl_info info;
  int rc = dladdr(addr, &info);
  ASSERT_NE(rc, 0); // Zero on error, non-zero on success.

  // Get the name of this executable.
  char executable_path[PATH_MAX];
  rc = readlink("/proc/self/exe", executable_path, sizeof(executable_path));
  ASSERT_NE(rc, -1);
  executable_path[rc] = '\0';
  std::string executable_name(basename(executable_path));

  // The filename should be that of this executable.
  // Note that we don't know whether or not we have the full path, so we want an "ends_with" test.
  std::string dli_fname(info.dli_fname);
  dli_fname = basename(&dli_fname[0]);
  ASSERT_EQ(dli_fname, executable_name);

  // The symbol name should be the symbol we looked up.
  ASSERT_STREQ(info.dli_sname, "DlSymTestFunction");

  // The address should be the exact address of the symbol.
  ASSERT_EQ(info.dli_saddr, sym);

  // Look in /proc/pid/maps to find out what address we were loaded at.
  // TODO: factor /proc/pid/maps parsing out into a class and reuse all over bionic.
  void* base_address = NULL;
  char line[BUFSIZ];
  FILE* fp = fopen("/proc/self/maps", "r");
  ASSERT_TRUE(fp != NULL);
  while (fgets(line, sizeof(line), fp) != NULL) {
    uintptr_t start = strtoul(line, 0, 16);
    line[strlen(line) - 1] = '\0'; // Chomp the '\n'.
    char* path = strchr(line, '/');
    if (path != NULL && strcmp(executable_path, path) == 0) {
      base_address = reinterpret_cast<void*>(start);
      break;
    }
  }
  fclose(fp);

  // The base address should be the address we were loaded at.
  ASSERT_EQ(info.dli_fbase, base_address);

  ASSERT_EQ(0, dlclose(self));
}

TEST(dlfcn, dladdr_invalid) {
  Dl_info info;

  dlerror(); // Clear any pending errors.

  // No symbol corresponding to NULL.
  ASSERT_EQ(dladdr(NULL, &info), 0); // Zero on error, non-zero on success.
  ASSERT_TRUE(dlerror() == NULL); // dladdr(3) doesn't set dlerror(3).

  // No symbol corresponding to a stack address.
  ASSERT_EQ(dladdr(&info, &info), 0); // Zero on error, non-zero on success.
  ASSERT_TRUE(dlerror() == NULL); // dladdr(3) doesn't set dlerror(3).
}

// GNU-style ELF hash tables are incompatible with the MIPS ABI.
// MIPS requires .dynsym to be sorted to match the GOT but GNU-style requires sorting by hash code.
TEST(dlfcn, dlopen_library_with_only_gnu_hash) {
#if !defined(__mips__)
  dlerror(); // Clear any pending errors.
  void* handle = dlopen("libgnu-hash-table-library.so", RTLD_NOW);
  ASSERT_TRUE(handle != nullptr) << dlerror();
  auto guard = make_scope_guard([&]() {
    dlclose(handle);
  });
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
  auto guard = make_scope_guard([&]() {
    dlclose(handle);
  });
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
  handle = dlopen(NULL, 0);
  ASSERT_TRUE(handle == NULL);
  ASSERT_SUBSTR("invalid", dlerror());
#endif

  handle = dlopen(NULL, 0xffffffff);
  ASSERT_TRUE(handle == NULL);
  ASSERT_SUBSTR("invalid", dlerror());

  // glibc actually allows you to choose both RTLD_NOW and RTLD_LAZY at the same time, and so do we.
  handle = dlopen(NULL, RTLD_NOW|RTLD_LAZY);
  ASSERT_TRUE(handle != NULL);
  ASSERT_SUBSTR(NULL, dlerror());
}

TEST(dlfcn, rtld_default_unknown_symbol) {
  void* addr = dlsym(RTLD_DEFAULT, "ANY_UNKNOWN_SYMBOL_NAME");
  ASSERT_TRUE(addr == NULL);
}

TEST(dlfcn, rtld_default_known_symbol) {
  void* addr = dlsym(RTLD_DEFAULT, "fopen");
  ASSERT_TRUE(addr != NULL);
}

TEST(dlfcn, rtld_next_unknown_symbol) {
  void* addr = dlsym(RTLD_NEXT, "ANY_UNKNOWN_SYMBOL_NAME");
  ASSERT_TRUE(addr == NULL);
}

TEST(dlfcn, rtld_next_known_symbol) {
  void* addr = dlsym(RTLD_NEXT, "fopen");
  ASSERT_TRUE(addr != NULL);
}

TEST(dlfcn, dlsym_weak_func) {
  dlerror();
  void* handle = dlopen("libtest_dlsym_weak_func.so", RTLD_NOW);
  ASSERT_TRUE(handle != NULL);

  int (*weak_func)();
  weak_func = reinterpret_cast<int (*)()>(dlsym(handle, "weak_func"));
  ASSERT_TRUE(weak_func != NULL) << "dlerror: " << dlerror();
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
  void* handle1 = dlopen("libdlext_test.so", RTLD_NOW);
  void* handle2 = dlopen("libdlext_test_v2.so", RTLD_NOW);
  ASSERT_TRUE(handle1 != NULL);
  ASSERT_TRUE(handle2 != NULL);
  ASSERT_EQ(handle1, handle2);
  dlclose(handle1);
  dlclose(handle2);
}

TEST(dlfcn, dlopen_from_zip_absolute_path) {
  const std::string lib_path = std::string(getenv("ANDROID_DATA")) + LIBZIPPATH;

  void* handle = dlopen((lib_path + "!libdir/libdlext_test_fd.so").c_str(), RTLD_NOW);
  ASSERT_TRUE(handle != nullptr) << dlerror();

  int (*fn)(void);
  fn = reinterpret_cast<int (*)(void)>(dlsym(handle, "getRandomNumber"));
  ASSERT_TRUE(fn != nullptr);
  EXPECT_EQ(4, fn());

  dlclose(handle);
}

TEST(dlfcn, dlopen_from_zip_ld_library_path) {
  const std::string lib_path = std::string(getenv("ANDROID_DATA")) + LIBZIPPATH + "!libdir";

  typedef void (*fn_t)(const char*);
  fn_t android_update_LD_LIBRARY_PATH =
      reinterpret_cast<fn_t>(dlsym(RTLD_DEFAULT, "android_update_LD_LIBRARY_PATH"));

  ASSERT_TRUE(android_update_LD_LIBRARY_PATH != nullptr) << dlerror();

  void* handle = dlopen("libdlext_test_fd.so", RTLD_NOW);
  ASSERT_TRUE(handle == nullptr);

  android_update_LD_LIBRARY_PATH(lib_path.c_str());

  handle = dlopen("libdlext_test_fd.so", RTLD_NOW);
  ASSERT_TRUE(handle != nullptr) << dlerror();

  int (*fn)(void);
  fn = reinterpret_cast<int (*)(void)>(dlsym(handle, "getRandomNumber"));
  ASSERT_TRUE(fn != nullptr);
  EXPECT_EQ(4, fn());

  dlclose(handle);
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
