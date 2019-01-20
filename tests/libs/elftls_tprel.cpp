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

// This shared object tests TPREL relocations in the dynamic linker. It's always
// part of static TLS.

// For accesses to these variables, the bfd and lld linkers generate a TPREL
// relocation with no symbol but a non-zero addend.
__attribute__((tls_model("initial-exec"))) static __thread int tls_var_1 = 3;
__attribute__((tls_model("initial-exec"))) static __thread int tls_var_2 = 7;

extern "C" int bump_static_tls_var_1() {
  return ++tls_var_1;
}

extern "C" int bump_static_tls_var_2() {
  return ++tls_var_2;
}

__attribute__((tls_model("initial-exec"), weak)) extern "C" __thread int missing_weak_tls;

extern "C" int* missing_weak_tls_addr() {
  // The dynamic linker should resolve a TPREL relocation to this symbol to 0,
  // which this function adds to the thread pointer.
  return &missing_weak_tls;
}
