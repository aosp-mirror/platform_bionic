/*
 * Copyright (C) 2020 The Android Open Source Project
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

#pragma once

#include <stddef.h>
#include <sys/prctl.h>

inline uintptr_t __bionic_clear_pac_bits(uintptr_t ptr) {
#if defined(__aarch64__)
  register uintptr_t x30 __asm("x30") = ptr;
  // This is a NOP on pre-Armv8.3-A architectures.
  asm("xpaclri" : "+r"(x30));
  return x30;
#else
  return ptr;
#endif
}

#ifdef __aarch64__
// The default setting for branch-protection enables both PAC and BTI, so by
// overriding it to only enable BTI we disable PAC.
#define __BIONIC_DISABLE_PAUTH __attribute__((target("branch-protection=bti")))
#else
#define __BIONIC_DISABLE_PAUTH
#endif

#ifdef __aarch64__
// Disable PAC (i.e. make the signing and authentication instructions into no-ops) for the lifetime
// of this object.
class ScopedDisablePAC {
  int prev_enabled_keys_;

 public:
  // Disabling IA will invalidate the return address in this function if it is signed, so we need to
  // make sure that this function does not sign its return address. Likewise for the destructor.
  __BIONIC_DISABLE_PAUTH
  ScopedDisablePAC() {
    // These prctls will fail (resulting in a no-op, the intended behavior) if PAC is not supported.
    prev_enabled_keys_ = prctl(PR_PAC_GET_ENABLED_KEYS, 0, 0, 0, 0);
    prctl(PR_PAC_SET_ENABLED_KEYS, prev_enabled_keys_, 0, 0, 0);
  }

  __BIONIC_DISABLE_PAUTH
  ~ScopedDisablePAC() {
    prctl(PR_PAC_SET_ENABLED_KEYS, prev_enabled_keys_, prev_enabled_keys_, 0, 0);
  }
};
#else
struct ScopedDisablePAC {
  // Silence unused variable warnings in non-aarch64 builds.
  ScopedDisablePAC() {}
};
#endif
