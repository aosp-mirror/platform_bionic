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

#define ASSERT_SUBSTR(needle, haystack) \
    ASSERT_PRED_FORMAT2(::testing::IsSubstring, needle, haystack)

static bool gCalled = false;
extern "C" void DlSymTestFunction() {
  gCalled = true;
}

TEST(dlfcn, dlsym_in_self) {
  dlerror(); // Clear any pending errors.
  void* self = dlopen(NULL, RTLD_NOW);
  ASSERT_TRUE(self != NULL);
  ASSERT_TRUE(dlerror() == NULL);

  void* sym = dlsym(self, "DlSymTestFunction");
  ASSERT_TRUE(sym != NULL);

  void (*function)() = reinterpret_cast<void(*)()>(sym);

  gCalled = false;
  function();
  ASSERT_TRUE(gCalled);

  ASSERT_EQ(0, dlclose(self));
}

TEST(dlfcn, dlopen_failure) {
  void* self = dlopen("/does/not/exist", RTLD_NOW);
  ASSERT_TRUE(self == NULL);
#if __BIONIC__
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

  // NULL handle.
  sym = dlsym(NULL, "test");
  ASSERT_TRUE(sym == NULL);
#if __BIONIC__
  ASSERT_SUBSTR("dlsym library handle is null", dlerror());
#else
  ASSERT_SUBSTR("undefined symbol: test", dlerror()); // glibc isn't specific about the failure.
#endif

  // NULL symbol name.
#if __BIONIC__
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
  char path[PATH_MAX];
  snprintf(path, sizeof(path), "/proc/%d/maps", getpid());
  char line[BUFSIZ];
  FILE* fp = fopen(path, "r");
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

#ifdef __GLIBC__
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
