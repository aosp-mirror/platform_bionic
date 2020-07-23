/*
 * Copyright (C) 2017 The Android Open Source Project
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

#include <dlfcn.h>
#include <gtest/gtest.h>
#include <sys/stat.h>

#include <vector>

#include "BionicDeathTest.h"
#include "gtest_globals.h"
#include "utils.h"

#if defined(__BIONIC__)
#include "private/CFIShadow.h"
#endif

// Private libdl interface.
extern "C" {
void __cfi_slowpath(uint64_t CallSiteTypeId, void* Ptr);
void __cfi_slowpath_diag(uint64_t CallSiteTypeId, void* Ptr, void* DiagData);
size_t __cfi_shadow_size();
}

static void f() {}

static void test_cfi_slowpath_with_alloc() {
  std::vector<void*> allocs;
  for (size_t i = 0; i < 1000; i++) {
    allocs.push_back(malloc(4096));
    __cfi_slowpath(46, allocs.back());
  }
}

TEST(cfi_test, basic) {
#if defined(__BIONIC__)
  void* handle;
  handle = dlopen("libcfi-test.so", RTLD_NOW | RTLD_LOCAL);
  ASSERT_TRUE(handle != nullptr) << dlerror();

  EXPECT_NE(0U, __cfi_shadow_size());

#define SYM(type, name) auto name = reinterpret_cast<type>(dlsym(handle, #name))
  SYM(size_t (*)(), get_count);
  SYM(uint64_t(*)(), get_last_type_id);
  SYM(void* (*)(), get_last_address);
  SYM(void* (*)(), get_last_diag);
  SYM(void* (*)(), get_global_address);
  SYM(void (*)(uint64_t, void*, void*), __cfi_check);
  SYM(char*, bss);
#undef SYM

  size_t c = get_count();

  // CFI check for code inside the DSO. Can't use just any function address - this is only
  // guaranteed to work for code addresses above __cfi_check.
  void* code_ptr = reinterpret_cast<char*>(__cfi_check) + 1234;
  void* diag_ptr = reinterpret_cast<void*>(5678);
  __cfi_slowpath_diag(42, code_ptr, diag_ptr);
  EXPECT_EQ(42U, get_last_type_id());
  EXPECT_EQ(code_ptr, get_last_address());
  EXPECT_EQ(diag_ptr, get_last_diag());
  EXPECT_EQ(++c, get_count());

  // __cfi_slowpath passes nullptr for the Diag argument.
  __cfi_slowpath(42, code_ptr);
  EXPECT_EQ(42U, get_last_type_id());
  EXPECT_EQ(code_ptr, get_last_address());
  EXPECT_EQ(nullptr, get_last_diag());
  EXPECT_EQ(++c, get_count());

  // CFI check for a data address inside the DSO.
  __cfi_slowpath(43, get_global_address());
  EXPECT_EQ(43U, get_last_type_id());
  EXPECT_EQ(get_global_address(), get_last_address());
  EXPECT_EQ(++c, get_count());

  // CFI check for a function inside _this_ DSO. It either goes to this DSO's __cfi_check,
  // or (if missing) is simply ignored. Any way, it does not affect the test lib's counters.
  __cfi_slowpath(44, reinterpret_cast<void*>(&f));
  EXPECT_EQ(43U, get_last_type_id());
  EXPECT_EQ(get_global_address(), get_last_address());
  EXPECT_EQ(c, get_count());

  // CFI check for a heap address.
  // It's possible that this allocation could wind up in the same CFI granule as
  // an unchecked library, which means the below might not crash. To force a
  // crash keep allocating up to a max until there is a crash.
  EXPECT_DEATH(test_cfi_slowpath_with_alloc(), "");

  // Check all the addresses.
  const size_t bss_size = 1024 * 1024;
  static_assert(bss_size >= kLibraryAlignment * 2, "test range not big enough");
  for (size_t i = 0; i < bss_size; ++i) {
    __cfi_slowpath(47, bss + i);
    EXPECT_EQ(++c, get_count());
  }

  // Load the same library again.
  void* handle2 = dlopen("libcfi-test.so", RTLD_NOW | RTLD_LOCAL);
  ASSERT_TRUE(handle2 != nullptr) << dlerror();
  EXPECT_EQ(handle2, handle);

  // Check that it is still there.
  __cfi_slowpath(43, get_global_address());
  EXPECT_EQ(43U, get_last_type_id());
  EXPECT_EQ(get_global_address(), get_last_address());
  EXPECT_EQ(++c, get_count());

  dlclose(handle);
  dlclose(handle2);

  // CFI check for a function inside the unloaded DSO. This is always invalid and gets the process
  // killed.
  EXPECT_DEATH(__cfi_slowpath(45, reinterpret_cast<void*>(code_ptr)), "");
#endif
}

TEST(cfi_test, invalid) {
#if defined(__BIONIC__)
  void* handle;
  handle = dlopen("libcfi-test-bad.so", RTLD_NOW | RTLD_LOCAL);
  ASSERT_FALSE(handle != nullptr) << dlerror();

  handle = dlopen("libcfi-test-bad.so", RTLD_NOW | RTLD_LOCAL);
  ASSERT_FALSE(handle != nullptr) << dlerror();
#endif
}

// cfi_test_helper exports __cfi_check, which triggers CFI initialization at startup.
TEST(cfi_test, early_init) {
#if defined(__BIONIC__)
  std::string helper = GetTestlibRoot() + "/cfi_test_helper/cfi_test_helper";
  chmod(helper.c_str(), 0755); // TODO: "x" lost in CTS, b/34945607
  ExecTestHelper eth;
  eth.SetArgs({ helper.c_str(), nullptr });
  eth.Run([&]() { execve(helper.c_str(), eth.GetArgs(), eth.GetEnv()); }, 0, nullptr);
#endif
}

// cfi_test_helper2 depends on a library that exports __cfi_check, which triggers CFI initialization
// at startup.
TEST(cfi_test, early_init2) {
#if defined(__BIONIC__)
  std::string helper = GetTestlibRoot() + "/cfi_test_helper2/cfi_test_helper2";
  chmod(helper.c_str(), 0755); // TODO: "x" lost in CTS, b/34945607
  ExecTestHelper eth;
  eth.SetArgs({ helper.c_str(), nullptr });
  eth.Run([&]() { execve(helper.c_str(), eth.GetArgs(), eth.GetEnv()); }, 0, nullptr);
#endif
}
