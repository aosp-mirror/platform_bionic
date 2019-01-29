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

// This shared object test library is dlopen'ed by the main test executable.
// This variable comes from libtest_elftls_shared_var.so, which is part of
// static TLS. Verify that a GD-model access can access the variable.
//
// Accessing the static TLS variable from an solib prevents the static linker
// from relaxing the GD access to IE and lets us test that __tls_get_addr and
// the tlsdesc resolver handle a static TLS variable.
extern "C" __thread int elftls_shared_var;

extern "C" int bump_shared_var() {
  return ++elftls_shared_var;
}

// The static linker denotes the current module by omitting the symbol from
// the DTPMOD/TLSDESC relocations.
static __thread int local_var_1 = 15;
static __thread int local_var_2 = 25;

extern "C" int bump_local_vars() {
  return ++local_var_1 + ++local_var_2;
}

__attribute__((weak)) extern "C" __thread int missing_weak_dyn_tls;

extern "C" int* missing_weak_dyn_tls_addr() {
  return &missing_weak_dyn_tls;
}
