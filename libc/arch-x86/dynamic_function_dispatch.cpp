/*
 * Copyright (C) 2008 The Android Open Source Project
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

#include <private/bionic_ifuncs.h>
#include <stddef.h>

extern "C" {

DEFINE_IFUNC_FOR(memcmp) {
  __builtin_cpu_init();
  if (__builtin_cpu_supports("sse4.1")) RETURN_FUNC(memcmp_func_t, memcmp_sse4);
  RETURN_FUNC(memcmp_func_t, memcmp_atom);
}
MEMCMP_SHIM()

typedef int wmemcmp_func_t(const wchar_t*, const wchar_t*, size_t);
DEFINE_IFUNC_FOR(wmemcmp) {
  __builtin_cpu_init();
  if (__builtin_cpu_supports("sse4.1")) RETURN_FUNC(wmemcmp_func_t, wmemcmp_sse4);
  RETURN_FUNC(wmemcmp_func_t, wmemcmp_atom);
}
DEFINE_STATIC_SHIM(int wmemcmp(const wchar_t* lhs, const wchar_t* rhs, size_t n) {
  FORWARD(wmemcmp)(lhs, rhs, n);
})

}  // extern "C"
