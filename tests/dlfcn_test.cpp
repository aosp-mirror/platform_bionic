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
#if defined(__i386__) || defined(__x86_64__)
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

TEST(dlfcn, dlopen_check_order) {
  // Here is how the test library and its dt_needed
  // libraries are arranged
  //
  //  libtest_check_order.so
  //  |
  //  +-> libtest_check_order_1_left.so
  //  |   |
  //  |   +-> libtest_check_order_a.so
  //  |   |
  //  |   +-> libtest_check_order_b.so
  //  |
  //  +-> libtest_check_order_2_right.so
  //  |   |
  //  |   +-> libtest_check_order_d.so
  //  |       |
  //  |       +-> libtest_check_order_b.so
  //  |
  //  +-> libtest_check_order_3_c.so
  //
  //  load order should be (1, 2, 3, a, b, d)
  //
  // get_answer() is defined in (2, 3, a, b, c)
  // get_answer2() is defined in (b, d)
  void* sym = dlsym(RTLD_DEFAULT, "dlopen_test_get_answer");
  ASSERT_TRUE(sym == nullptr);
  void* handle = dlopen("libtest_check_order.so", RTLD_NOW | RTLD_GLOBAL);
  ASSERT_TRUE(handle != nullptr);
  typedef int (*fn_t) (void);
  fn_t fn, fn2;
  fn = reinterpret_cast<fn_t>(dlsym(RTLD_DEFAULT, "dlopen_test_get_answer"));
  ASSERT_TRUE(fn != NULL) << dlerror();
  fn2 = reinterpret_cast<fn_t>(dlsym(RTLD_DEFAULT, "dlopen_test_get_answer2"));
  ASSERT_TRUE(fn2 != NULL) << dlerror();

  ASSERT_EQ(42, fn());
  ASSERT_EQ(43, fn2());
  dlclose(handle);
}

// libtest_with_dependency_loop.so -> libtest_with_dependency_loop_a.so ->
// libtest_with_dependency_loop_b.so -> libtest_with_dependency_loop_c.so ->
// libtest_with_dependency_loop_a.so
TEST(dlfcn, dlopen_check_loop) {
  void* handle = dlopen("libtest_with_dependency_loop.so", RTLD_NOW);
#if defined(__BIONIC__)
  ASSERT_TRUE(handle == nullptr);
  ASSERT_STREQ("dlopen failed: recursive link to \"libtest_with_dependency_loop_a.so\"", dlerror());
  // This symbol should never be exposed
  void* f = dlsym(RTLD_DEFAULT, "dlopen_test_invalid_function");
  ASSERT_TRUE(f == nullptr);
  ASSERT_SUBSTR("undefined symbol: dlopen_test_invalid_function", dlerror());

  // dlopen second time to make sure that the library wasn't loaded even though dlopen returned null.
  // This may happen if during cleanup the root library or one of the depended libs were not removed
  // from soinfo list.
  handle = dlopen("libtest_with_dependency_loop.so", RTLD_NOW | RTLD_NOLOAD);
  ASSERT_TRUE(handle == nullptr);
  ASSERT_STREQ("dlopen failed: library \"libtest_with_dependency_loop.so\" wasn't loaded and RTLD_NOLOAD prevented it", dlerror());
#else // glibc allows recursive links
  ASSERT_TRUE(handle != nullptr);
  dlclose(handle);
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

// Our dynamic linker doesn't support GNU hash tables.
#if defined(__BIONIC__)
// GNU-style ELF hash tables are incompatible with the MIPS ABI.
// MIPS requires .dynsym to be sorted to match the GOT but GNU-style requires sorting by hash code.
#if !defined(__mips__)
TEST(dlfcn, dlopen_library_with_only_gnu_hash) {
  dlerror(); // Clear any pending errors.
  void* handle = dlopen("no-elf-hash-table-library.so", RTLD_NOW);
  ASSERT_TRUE(handle == NULL);
  ASSERT_STREQ("dlopen failed: empty/missing DT_HASH in \"no-elf-hash-table-library.so\" (built with --hash-style=gnu?)", dlerror());
}
#endif
#endif

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
  void* handle = dlopen("libtest_dlsym_weak_func.so",RTLD_NOW);
  ASSERT_TRUE(handle != NULL);

  int (*weak_func)();
  weak_func = reinterpret_cast<int (*)()>(dlsym(handle, "weak_func"));
  ASSERT_TRUE(weak_func != NULL) << "dlerror: " << dlerror();
  EXPECT_EQ(42, weak_func());
  dlclose(handle);
}

TEST(dlfcn, dlopen_symlink) {
  void* handle1 = dlopen("libdlext_test.so", RTLD_NOW);
  void* handle2 = dlopen("libdlext_test_v2.so", RTLD_NOW);
  ASSERT_TRUE(handle1 != NULL);
  ASSERT_TRUE(handle2 != NULL);
  ASSERT_EQ(handle1, handle2);
}
