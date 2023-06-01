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
#include <link.h>

#include <android-base/file.h>
#include <android-base/test_utils.h>
#include <gtest/gtest.h>

#include <thread>

#include "gtest_globals.h"
#include "platform/bionic/tls.h"
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
  std::string helper = GetTestlibRoot() + "/elftls_dlopen_ie_error_helper";
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
  GTEST_SKIP() << "This test is only run on TLSDESC-based targets";
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

  // Initially there are 4 modules (5 w/ hwasan):
  //  - the main test executable
  //  - libc
  //  - libtest_elftls_shared_var
  //  - libtest_elftls_tprel
  //  - w/ hwasan: libclang_rt.hwasan

  // The initial DTV is an empty DTV with no generation and a size of 0.
  TlsDtv* zero_dtv = dtv();
  ASSERT_EQ(0u, zero_dtv->count);
  ASSERT_EQ(nullptr, zero_dtv->next);
  ASSERT_EQ(kTlsGenerationNone, zero_dtv->generation);

  // Load module 5 (6 w/ hwasan).
  auto func1 = LOAD_LIB("libtest_elftls_dynamic_filler_1.so");
  ASSERT_EQ(101, func1());

  // After loading one module, the DTV should be initialized to the next
  // power-of-2 size (including the header).
  TlsDtv* initial_dtv = dtv();
  ASSERT_EQ(running_with_hwasan() ? 13u : 5u, dtv()->count);
  ASSERT_EQ(zero_dtv, initial_dtv->next);
  ASSERT_LT(0u, initial_dtv->generation);

  // Load module 6 (7 w/ hwasan).
  auto func2 = LOAD_LIB("libtest_elftls_dynamic_filler_2.so");
  ASSERT_EQ(102, func1());

#if defined(__aarch64__)
  // The arm64 TLSDESC resolver doesn't update the DTV if it is new enough for
  // the given access.
  ASSERT_EQ(running_with_hwasan() ? 13u : 5u, dtv()->count);
#else
  // __tls_get_addr updates the DTV anytime the generation counter changes.
  ASSERT_EQ(13u, dtv()->count);
#endif

  ASSERT_EQ(201, func2());
  TlsDtv* new_dtv = dtv();
  if (!running_with_hwasan()) {
    ASSERT_NE(initial_dtv, new_dtv);
    ASSERT_EQ(initial_dtv, new_dtv->next);
  }
  ASSERT_EQ(13u, new_dtv->count);

  // Load module 7 (8 w/ hwasan).
  auto func3 = LOAD_LIB("libtest_elftls_dynamic_filler_3.so");
  ASSERT_EQ(103, func1());
  ASSERT_EQ(202, func2());
  ASSERT_EQ(301, func3());

  ASSERT_EQ(new_dtv, dtv());

#undef LOAD_LIB
#else
  GTEST_SKIP() << "test doesn't apply to glibc";
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
  GTEST_SKIP() << "test doesn't apply to glibc";
#endif
}

// Use dlsym to get the address of a TLS variable in static TLS and compare it
// against the ordinary address of the variable.
TEST(elftls_dl, dlsym_static_tls) {
  void* lib = dlopen("libtest_elftls_shared_var.so", RTLD_LOCAL | RTLD_NOW);
  ASSERT_NE(nullptr, lib);

  int* var_addr = static_cast<int*>(dlsym(lib, "elftls_shared_var"));
  ASSERT_EQ(&elftls_shared_var, var_addr);

  std::thread([lib] {
    int* var_addr = static_cast<int*>(dlsym(lib, "elftls_shared_var"));
    ASSERT_EQ(&elftls_shared_var, var_addr);
  }).join();
}

// Use dlsym to get the address of a TLS variable in dynamic TLS and compare it
// against the ordinary address of the variable.
TEST(elftls_dl, dlsym_dynamic_tls) {
  void* lib = dlopen("libtest_elftls_dynamic.so", RTLD_LOCAL | RTLD_NOW);
  ASSERT_NE(nullptr, lib);
  auto get_var_addr = reinterpret_cast<int*(*)()>(dlsym(lib, "get_large_tls_var_addr"));
  ASSERT_NE(nullptr, get_var_addr);

  int* var_addr = static_cast<int*>(dlsym(lib, "large_tls_var"));
  ASSERT_EQ(get_var_addr(), var_addr);

  std::thread([lib, get_var_addr] {
    int* var_addr = static_cast<int*>(dlsym(lib, "large_tls_var"));
    ASSERT_EQ(get_var_addr(), var_addr);
  }).join();
}

// Calling dladdr on a TLS variable's address doesn't find anything.
TEST(elftls_dl, dladdr_on_tls_var) {
  Dl_info info;

  // Static TLS variable
  ASSERT_EQ(0, dladdr(&elftls_shared_var, &info));

  // Dynamic TLS variable
  void* lib = dlopen("libtest_elftls_dynamic.so", RTLD_LOCAL | RTLD_NOW);
  ASSERT_NE(nullptr, lib);
  int* var_addr = static_cast<int*>(dlsym(lib, "large_tls_var"));
  ASSERT_EQ(0, dladdr(var_addr, &info));
}

// Verify that dladdr does not misinterpret a TLS symbol's value as a virtual
// address.
TEST(elftls_dl, dladdr_skip_tls_symbol) {
  void* lib = dlopen("libtest_elftls_dynamic.so", RTLD_LOCAL | RTLD_NOW);

  auto get_local_addr = reinterpret_cast<void*(*)()>(dlsym(lib, "get_local_addr"));
  ASSERT_NE(nullptr, get_local_addr);
  void* local_addr = get_local_addr();

  Dl_info info;
  ASSERT_NE(0, dladdr(local_addr, &info));

  std::string libpath = GetTestlibRoot() + "/libtest_elftls_dynamic.so";
  char dli_realpath[PATH_MAX];
  ASSERT_TRUE(realpath(info.dli_fname, dli_realpath));
  ASSERT_STREQ(libpath.c_str(), dli_realpath);
  ASSERT_STREQ(nullptr, info.dli_sname);
  ASSERT_EQ(nullptr, info.dli_saddr);
}

TEST(elftls_dl, dl_iterate_phdr) {
  void* lib = dlopen("libtest_elftls_dynamic.so", RTLD_LOCAL | RTLD_NOW);

  auto get_var_addr = reinterpret_cast<void*(*)()>(dlsym(lib, "get_large_tls_var_addr"));
  ASSERT_NE(nullptr, get_var_addr);

  struct TlsInfo {
    bool found;
    size_t modid;
    void* data;
    size_t memsz;
  };

  auto get_tls_info = []() {
    auto callback = [](dl_phdr_info* info, size_t, void* data) {
      TlsInfo& tls_info = *static_cast<TlsInfo*>(data);

      // This test is also run with glibc, where dlpi_name may have relative path components, so
      // examine just the basename when searching for the library.
      if (strcmp(android::base::Basename(info->dlpi_name).c_str(), "libtest_elftls_dynamic.so") != 0) return 0;

      tls_info.found = true;
      tls_info.modid = info->dlpi_tls_modid;
      tls_info.data = info->dlpi_tls_data;
      for (ElfW(Half) i = 0; i < info->dlpi_phnum; ++i) {
        if (info->dlpi_phdr[i].p_type == PT_TLS) {
          tls_info.memsz = info->dlpi_phdr[i].p_memsz;
        }
      }
      EXPECT_NE(static_cast<size_t>(0), tls_info.memsz);
      return 1;
    };

    TlsInfo result {};
    dl_iterate_phdr(callback, &result);
    return result;
  };

  // The executable has a TLS segment, so it will use module ID #1, and the DSO's ID will be larger
  // than 1. Initially, the data field is nullptr, because this thread's instance hasn't been
  // allocated yet.
  TlsInfo tls_info = get_tls_info();
  ASSERT_TRUE(tls_info.found);
  ASSERT_GT(tls_info.modid, static_cast<size_t>(1));
  ASSERT_EQ(nullptr, tls_info.data);

  void* var_addr = get_var_addr();

  // Verify that dl_iterate_phdr returns a range of memory covering the allocated TLS variable.
  tls_info = get_tls_info();
  ASSERT_TRUE(tls_info.found);
  ASSERT_GE(var_addr, tls_info.data);
  ASSERT_LT(var_addr, static_cast<char*>(tls_info.data) + tls_info.memsz);
}
