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
#include <sys/stat.h>

#include <vector>

#include <gtest/gtest.h>

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

// Disables debuggerd stack traces to speed up death tests, make them less
// noisy in logcat, and avoid expected deaths from showing up in stability
// metrics.
// We don't use the usual libbase class because (a) we don't care about most
// of the signals it blocks but (b) we do need to block SIGILL, which normal
// death tests shouldn't ever hit. (It's possible that a design where a
// deathtest always declares its expected signals up front is a better one,
// and maybe that's an interesting future direction for libbase.)
//
// We include SIGSEGV because there's a test that passes heap addresses to
// __cfi_slowpath and we only map the executable code shadow as readable.
// We don't always get SIGSEGV there though: if the heap allocation happens
// to be close enough to an executable mapping that its shadow is in the
// same page as the executable shadow, we'll get SIGILL/SIGTRAP.
class cfi_test_DeathTest : public testing::Test {
 protected:
  void SetUp() override {
    struct sigaction64 action = {.sa_handler = SIG_DFL};
    sigaction64(SIGILL, &action, &previous_sigill_);
    sigaction64(SIGSEGV, &action, &previous_sigsegv_);
    sigaction64(SIGTRAP, &action, &previous_sigtrap_);
  }

  void TearDown() override {
    sigaction64(SIGTRAP, &previous_sigtrap_, nullptr);
    sigaction64(SIGSEGV, &previous_sigsegv_, nullptr);
    sigaction64(SIGILL, &previous_sigill_, nullptr);
  }

 private:
  struct sigaction64 previous_sigill_;
  struct sigaction64 previous_sigsegv_;
  struct sigaction64 previous_sigtrap_;
};

static void f() {}

TEST_F(cfi_test_DeathTest, basic) {
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
  std::string helper = GetTestlibRoot() + "/cfi_test_helper";
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
  std::string helper = GetTestlibRoot() + "/cfi_test_helper2";
  chmod(helper.c_str(), 0755); // TODO: "x" lost in CTS, b/34945607
  ExecTestHelper eth;
  eth.SetArgs({ helper.c_str(), nullptr });
  eth.Run([&]() { execve(helper.c_str(), eth.GetArgs(), eth.GetEnv()); }, 0, nullptr);
#endif
}
