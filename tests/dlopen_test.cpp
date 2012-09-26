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

static bool gCalled = false;
extern "C" void DlSymTestFunction() {
  gCalled = true;
}

TEST(dlopen, dlsym_in_self) {
  void* self = dlopen(NULL, RTLD_NOW);
  ASSERT_TRUE(self != NULL);

  void* sym = dlsym(self, "DlSymTestFunction");
  ASSERT_TRUE(sym != NULL);

  void (*function)() = reinterpret_cast<void(*)()>(sym);

  gCalled = false;
  function();
  ASSERT_TRUE(gCalled);
}

TEST(dlopen, dladdr) {
  void* self = dlopen(NULL, RTLD_NOW);
  ASSERT_TRUE(self != NULL);

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
    if (strcmp(executable_path, path) == 0) {
      base_address = reinterpret_cast<void*>(start);
      break;
    }
  }
  fclose(fp);

  // The base address should be the address we were loaded at.
  ASSERT_EQ(info.dli_fbase, base_address);
}

TEST(dlopen, dladdr_invalid) {
  Dl_info info;

  // No symbol corresponding to NULL.
  ASSERT_EQ(dladdr(NULL, &info), 0); // Zero on error, non-zero on success.

  // No symbol corresponding to a stack address.
  ASSERT_EQ(dladdr(&info, &info), 0); // Zero on error, non-zero on success.
}
