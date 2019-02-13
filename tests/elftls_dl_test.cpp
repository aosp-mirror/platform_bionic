/*
 * Copyright (C) 2019 The Android Open Source Project
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <dlfcn.h>
#include <gtest/gtest.h>

#include <thread>

#include "gtest_globals.h"
#include "private/__get_tls.h"
#include "utils.h"

#if defined(__BIONIC__)
#include "bionic/pthread_internal.h"
#endif

// Access libtest_elftls_shared_var.so's TLS variable using an IE access.
__attribute__((tls_model("initial-exec"))) extern "C" __thread int elftls_shared_var;

TEST(elftls_dl, dlopen_shared_var_ie) {
  // libtest_elftls_shared_var_ie.so can be dlopen'ed, even though it contains a
  // TLS IE access, because its IE access references a TLS variable from
  // libtest_elftls_shared_var.so, which is DT_NEEDED by the executable. This
  // pattern appears in sanitizers, which use TLS IE instrumentation in shared
  // objects to access special variables exported from the executable or from a
  // preloaded solib.
  void* lib = dlopen("libtest_elftls_shared_var_ie.so", RTLD_LOCAL | RTLD_NOW);
  ASSERT_NE(nullptr, lib);

  auto bump_shared_var = reinterpret_cast<int(*)()>(dlsym(lib, "bump_shared_var"));
  ASSERT_NE(nullptr, bump_shared_var);

  ASSERT_EQ(21, ++elftls_shared_var);
  ASSERT_EQ(22, bump_shared_var());

  std::thread([bump_shared_var] {
    ASSERT_EQ(21, ++elftls_shared_var);
    ASSERT_EQ(22, bump_shared_var());
  }).join();
}

TEST(elftls_dl, dlopen_ie_error) {
  std::string helper = GetTestlibRoot() +
      "/elftls_dlopen_ie_error_helper/elftls_dlopen_ie_error_helper";
  std::string src_path = GetTestlibRoot() + "/libtest_elftls_shared_var_ie.so";
  std::string dst_path = GetTestlibRoot() + "/libtest_elftls_shared_var.so";
#if defined(__BIONIC__)
  std::string error =
      "dlerror: dlopen failed: TLS symbol \"elftls_shared_var\" in dlopened \"" + dst_path + "\" " +
      "referenced from \"" + src_path + "\" using IE access model\n";
#else
  // glibc will reserve some surplus static TLS memory, allowing this test to pass.
  std::string error = "success\n";
#endif

  chmod(helper.c_str(), 0755); // TODO: "x" lost in CTS, b/34945607
  ExecTestHelper eth;
  eth.SetArgs({ helper.c_str(), nullptr });
  eth.Run([&]() { execve(helper.c_str(), eth.GetArgs(), eth.GetEnv()); }, 0, error.c_str());
}

// Use a GD access (__tls_get_addr or TLSDESC) to modify a variable in static
// TLS memory.
TEST(elftls_dl, access_static_tls) {
  void* lib = dlopen("libtest_elftls_dynamic.so", RTLD_LOCAL | RTLD_NOW);
  ASSERT_NE(nullptr, lib);

  auto bump_shared_var = reinterpret_cast<int(*)()>(dlsym(lib, "bump_shared_var"));
  ASSERT_NE(nullptr, bump_shared_var);

  ASSERT_EQ(21, ++elftls_shared_var);
  ASSERT_EQ(22, bump_shared_var());

  std::thread([bump_shared_var] {
    ASSERT_EQ(21, ++elftls_shared_var);
    ASSERT_EQ(22, bump_shared_var());
  }).join();
}

TEST(elftls_dl, bump_local_vars) {
  void* lib = dlopen("libtest_elftls_dynamic.so", RTLD_LOCAL | RTLD_NOW);
  ASSERT_NE(nullptr, lib);

  auto bump_local_vars = reinterpret_cast<int(*)()>(dlsym(lib, "bump_local_vars"));
  ASSERT_NE(nullptr, bump_local_vars);

  ASSERT_EQ(42, bump_local_vars());
  std::thread([bump_local_vars] {
    ASSERT_EQ(42, bump_local_vars());
  }).join();
}

extern "C" int* missing_weak_tls_addr();

// The Bionic linker resolves a TPREL relocation to an unresolved weak TLS
// symbol to 0, which is added to the thread pointer. N.B.: A TPREL relocation
// in a static executable is resolved by the static linker instead, and static
// linker behavior varies (especially with bfd and gold). See
// https://bugs.llvm.org/show_bug.cgi?id=40570.
TEST(elftls_dl, tprel_missing_weak) {
  ASSERT_EQ(static_cast<void*>(__get_tls()), missing_weak_tls_addr());
  std::thread([] {
    ASSERT_EQ(static_cast<void*>(__get_tls()), missing_weak_tls_addr());
  }).join();
}

// The behavior of accessing an unresolved weak TLS symbol using a dynamic TLS
// relocation depends on which kind of implementation the target uses. With
// TLSDESC, the result is NULL. With __tls_get_addr, the result is the
// generation count (or maybe undefined behavior)? This test only tests TLSDESC.
TEST(elftls_dl, tlsdesc_missing_weak) {
#if defined(__aarch64__)
  void* lib = dlopen("libtest_elftls_dynamic.so", RTLD_LOCAL | RTLD_NOW);
  ASSERT_NE(nullptr, lib);

  auto missing_weak_dyn_tls_addr = reinterpret_cast<int*(*)()>(dlsym(lib, "missing_weak_dyn_tls_addr"));
  ASSERT_NE(nullptr, missing_weak_dyn_tls_addr);

  ASSERT_EQ(nullptr, missing_weak_dyn_tls_addr());
  std::thread([missing_weak_dyn_tls_addr] {
    ASSERT_EQ(nullptr, missing_weak_dyn_tls_addr());
  }).join();
#else
  GTEST_LOG_(INFO) << "This test is only run on TLSDESC-based targets.\n";
#endif
}

TEST(elftls_dl, dtv_resize) {
#if defined(__BIONIC__)
#define LOAD_LIB(soname) ({                           \
    auto lib = dlopen(soname, RTLD_LOCAL | RTLD_NOW); \
    ASSERT_NE(nullptr, lib);                          \
    reinterpret_cast<int(*)()>(dlsym(lib, "bump"));   \
  })

  auto dtv = []() -> TlsDtv* { return __get_tcb_dtv(__get_bionic_tcb()); };

  static_assert(sizeof(TlsDtv) == 3 * sizeof(void*),
                "This test assumes that the Dtv has a 3-word header");

  // Initially there are 3 modules:
  //  - the main test executable
  //  - libtest_elftls_shared_var
  //  - libtest_elftls_tprel

  // The initial DTV is an empty DTV with no generation and a size of 0.
  TlsDtv* zero_dtv = dtv();
  ASSERT_EQ(0u, zero_dtv->count);
  ASSERT_EQ(nullptr, zero_dtv->next);
  ASSERT_EQ(kTlsGenerationNone, zero_dtv->generation);

  // Load the fourth module.
  auto func1 = LOAD_LIB("libtest_elftls_dynamic_filler_1.so");
  ASSERT_EQ(101, func1());

  // After loading one module, the DTV should be initialized to the next
  // power-of-2 size (including the header).
  TlsDtv* initial_dtv = dtv();
  ASSERT_EQ(5u, initial_dtv->count);
  ASSERT_EQ(zero_dtv, initial_dtv->next);
  ASSERT_LT(0u, initial_dtv->generation);

  // Load module 5.
  auto func2 = LOAD_LIB("libtest_elftls_dynamic_filler_2.so");
  ASSERT_EQ(102, func1());
  ASSERT_EQ(201, func2());
  ASSERT_EQ(initial_dtv, dtv());
  ASSERT_EQ(5u, initial_dtv->count);

  // Load module 6.
  auto func3 = LOAD_LIB("libtest_elftls_dynamic_filler_3.so");
  ASSERT_EQ(103, func1());
  ASSERT_EQ(202, func2());

#if defined(__aarch64__)
  // The arm64 TLSDESC resolver doesn't update the DTV if it is new enough for
  // the given access.
  ASSERT_EQ(5u, dtv()->count);
#else
  // __tls_get_addr updates the DTV anytime the generation counter changes.
  ASSERT_EQ(13u, dtv()->count);
#endif

  ASSERT_EQ(301, func3());

  TlsDtv* new_dtv = dtv();
  ASSERT_EQ(13u, new_dtv->count);
  ASSERT_NE(initial_dtv, new_dtv);
  ASSERT_EQ(initial_dtv, new_dtv->next);

#undef LOAD_LIB
#else
  GTEST_LOG_(INFO) << "This test is skipped for glibc because it tests Bionic internals.";
#endif
}

// Verify that variables are reset to their initial values after the library
// containing them is closed.
TEST(elftls_dl, dlclose_resets_values) {
  for (int round = 0; round < 2; ++round) {
    void* lib = dlopen("libtest_elftls_dynamic.so", RTLD_LOCAL | RTLD_NOW);
    ASSERT_NE(nullptr, lib);

    auto bump_local_vars = reinterpret_cast<int(*)()>(dlsym(lib, "bump_local_vars"));
    ASSERT_NE(nullptr, bump_local_vars);

    ASSERT_EQ(42, bump_local_vars());
    ASSERT_EQ(44, bump_local_vars());

    ASSERT_EQ(0, dlclose(lib));
  }
}

// Calling dlclose should remove the entry for the solib from the global list of
// ELF TLS modules. Test that repeatedly loading and unloading a library doesn't
// increase the DTV size.
TEST(elftls_dl, dlclose_removes_entry) {
#if defined(__BIONIC__)
  auto dtv = []() -> TlsDtv* { return __get_tcb_dtv(__get_bionic_tcb()); };

  bool first = true;
  size_t count = 0;

  // Use a large number of rounds in case the DTV is initially larger than
  // expected.
  for (int round = 0; round < 32; ++round) {
    void* lib = dlopen("libtest_elftls_dynamic.so", RTLD_LOCAL | RTLD_NOW);
    ASSERT_NE(nullptr, lib);

    auto bump_local_vars = reinterpret_cast<int(*)()>(dlsym(lib, "bump_local_vars"));
    ASSERT_NE(nullptr, bump_local_vars);

    ASSERT_EQ(42, bump_local_vars());
    if (first) {
      first = false;
      count = dtv()->count;
    } else {
      ASSERT_EQ(count, dtv()->count);
    }

    dlclose(lib);
  }
#else
  GTEST_LOG_(INFO) << "This test is skipped for glibc because it tests Bionic internals.";
#endif
}
