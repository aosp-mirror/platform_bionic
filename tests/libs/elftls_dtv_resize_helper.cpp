/*
 * Copyright (C) 2024 The Android Open Source Project
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
#include <stdio.h>
#include <stdlib.h>

#include <functional>
#include <iostream>

#include "bionic/pthread_internal.h"

constexpr bool kDumpModulesForDebugging = false;

// The old external/libcxx doesn't have operator<< for nullptr.
// TODO(b/175635923): Remove this hack after upgrading libc++.
template <class T>
T fix_nullptr(T&& arg) {
  return arg;
}
void* fix_nullptr(nullptr_t arg) {
  return static_cast<void*>(arg);
}

template <class Val1, class Val2, class Compare>
void check(int line, const char* val1_expr, Val1&& val1, const char* val2_expr, Val2&& val2,
           Compare compare) {
  if (!compare(val1, val2)) {
    std::cerr << __FILE__ << ":" << line << ": assertion failed: LHS(" << val1_expr << ") is "
              << fix_nullptr(val1) << ", RHS(" << val2_expr << ") is " << fix_nullptr(val2) << "\n"
              << std::flush;
    abort();
  }
}

#define ASSERT_EQ(val1, val2) check(__LINE__, #val1, val1, #val2, val2, std::equal_to())
#define ASSERT_NE(val1, val2) check(__LINE__, #val1, val1, #val2, val2, std::not_equal_to())
#define ASSERT_LT(val1, val2) check(__LINE__, #val1, val1, #val2, val2, std::less())
#define ASSERT_LE(val1, val2) check(__LINE__, #val1, val1, #val2, val2, std::less_equal())

static size_t highest_loaded_modid() {
  size_t result = 0;
  auto update_result = [](struct dl_phdr_info* info, size_t size __unused, void* data) {
    size_t& result = *reinterpret_cast<size_t*>(data);
    if (kDumpModulesForDebugging) {
      fprintf(stderr, "module %s: TLS modid %zu\n", info->dlpi_name, info->dlpi_tls_modid);
    }
    result = std::max(result, info->dlpi_tls_modid);
    return 0;
  };
  dl_iterate_phdr(update_result, &result);
  return result;
}

static TlsDtv* dtv() {
  return __get_tcb_dtv(__get_bionic_tcb());
}

static size_t highest_modid_in_dtv() {
  TlsDtv* current_dtv = dtv();
  size_t result = 0;
  for (size_t i = 0; i < current_dtv->count; ++i) {
    if (current_dtv->modules[i] != nullptr) {
      result = __tls_module_idx_to_id(i);
    }
  }
  return result;
}

// Unused, but ensures that the test executable has a TLS segment. With a
// new-enough libc++_static.a, the test executable will tend to has a TLS
// segment to hold the libc++ EH globals pointer.
__thread int g_tls_var_placeholder = 42;

int main() {
  // Prevent this TLS variable from being optimized away.
  ASSERT_EQ(42, g_tls_var_placeholder);

  auto load_lib = [](const char* soname) {
    void* lib = dlopen(soname, RTLD_LOCAL | RTLD_NOW);
    ASSERT_NE(nullptr, lib);
    auto func = reinterpret_cast<int (*)()>(dlsym(lib, "bump"));
    ASSERT_NE(nullptr, func);
    return func;
  };

  static_assert(sizeof(TlsDtv) == 3 * sizeof(void*),
                "This test assumes that the Dtv has a 3-word header");

  // Initially there are 2-4 modules:
  //  - 1: test executable
  //  - 2: libc
  //  - 3: libc++ (when using a new-enough libc++)
  //  - 4: libclang_rt.hwasan (when running with HWASan)
  size_t first_filler_modid = highest_loaded_modid() + 1;
  ASSERT_LE(2, highest_loaded_modid());
  ASSERT_LE(highest_loaded_modid(), 4);

  // The initial DTV is an empty DTV with no generation and a size of 0.
  TlsDtv* zero_dtv = dtv();
  ASSERT_EQ(0u, zero_dtv->count);
  ASSERT_EQ(nullptr, zero_dtv->next);
  ASSERT_EQ(kTlsGenerationNone, zero_dtv->generation);

  // Load a module. The DTV is still empty unless the TLS variable is accessed.
  auto func1 = load_lib("libtest_elftls_dynamic_filler_1.so");
  ASSERT_EQ(zero_dtv, dtv());
  ASSERT_EQ(first_filler_modid, highest_loaded_modid());

  // After accessing a TLS variable, the DTV should be initialized. It should be
  // 8 words in size, with a 5-entry capacity.
  ASSERT_EQ(101, func1());
  TlsDtv* initial_dtv = dtv();
  ASSERT_EQ(5u, dtv()->count);
  ASSERT_EQ(zero_dtv, initial_dtv->next);
  ASSERT_LT(0u, initial_dtv->generation);
  ASSERT_EQ(first_filler_modid, highest_modid_in_dtv());
  ASSERT_NE(nullptr, initial_dtv->modules[__tls_module_id_to_idx(first_filler_modid)]);

  size_t current_generation = initial_dtv->generation;

  // Fill the rest of the DTV up. (i.e. Ensure that exactly 5 modules with TLS
  // segments are loaded.)
  auto fill_entry = [&](size_t modid, const char* soname, int tls_var_value) {
    if (highest_modid_in_dtv() == modid - 1) {
      auto func = load_lib(soname);

      // Loading the module doesn't affect the DTV yet.
      ASSERT_EQ(initial_dtv, dtv());
      ASSERT_EQ(modid, highest_loaded_modid());
      ASSERT_EQ(modid - 1, highest_modid_in_dtv());
      ASSERT_EQ(current_generation, initial_dtv->generation);

      // Access the TLS variable, which will allocate it in the DTV.
      ASSERT_EQ(tls_var_value, func());

      // Verify allocation and a bumped generation.
      ASSERT_EQ(initial_dtv, dtv());
      ASSERT_EQ(modid, highest_modid_in_dtv());
      ASSERT_LT(current_generation, initial_dtv->generation);
      current_generation = initial_dtv->generation;
    }
  };

  fill_entry(4u, "libtest_elftls_dynamic_filler_2.so", 201);
  fill_entry(5u, "libtest_elftls_dynamic_filler_3.so", 301);
  ASSERT_EQ(5u, highest_modid_in_dtv());

  // Load module 6, which will require doubling the size of the DTV.
  auto func4 = load_lib("libtest_elftls_dynamic_filler_4.so");
  ASSERT_EQ(6u, highest_loaded_modid());
  ASSERT_EQ(5u, highest_modid_in_dtv());
  ASSERT_EQ(initial_dtv, dtv());

  // Access a TLS variable from the first filler module.
  ASSERT_EQ(102, func1());
  ASSERT_EQ(5u, highest_modid_in_dtv());
#if defined(__aarch64__) || defined(__riscv)
  // The arm64 and riscv64 TLSDESC resolver doesn't update the DTV if it is new enough for
  // the given access.
  ASSERT_EQ(initial_dtv, dtv());
  ASSERT_EQ(5u, dtv()->count);
  ASSERT_EQ(current_generation, dtv()->generation);
#else
  // __tls_get_addr updates the DTV anytime the generation counter changes, but
  // the highest modid in the DTV is still 5, because module 6 hasn't been
  // allocated yet.
  ASSERT_NE(initial_dtv, dtv());
  ASSERT_EQ(13u, dtv()->count);
  ASSERT_LT(current_generation, dtv()->generation);
#endif

  // Accessing the TLS variable in the latest module will always expand the DTV.
  ASSERT_EQ(401, func4());
  TlsDtv* new_dtv = dtv();
  ASSERT_NE(initial_dtv, new_dtv);
  ASSERT_EQ(initial_dtv, new_dtv->next);
  ASSERT_EQ(13u, new_dtv->count);
  ASSERT_LT(current_generation, new_dtv->generation);
  ASSERT_EQ(6u, highest_modid_in_dtv());
  current_generation = new_dtv->generation;

  // Load one more filler, module 7.
  auto func5 = load_lib("libtest_elftls_dynamic_filler_5.so");
  ASSERT_EQ(103, func1());
  ASSERT_EQ(402, func4());
  ASSERT_EQ(6u, highest_modid_in_dtv());
  ASSERT_EQ(501, func5());
  ASSERT_EQ(7u, highest_modid_in_dtv());

  // Verify that no new DTV has been allocated.
  ASSERT_EQ(new_dtv, dtv());
  ASSERT_EQ(13u, new_dtv->count);
  ASSERT_LT(current_generation, new_dtv->generation);

  return 0;
}
