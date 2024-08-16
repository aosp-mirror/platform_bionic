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
  if (__builtin_cpu_is("atom")) RETURN_FUNC(memcmp_func_t, memcmp_atom);
  if (__builtin_cpu_supports("sse4.1")) RETURN_FUNC(memcmp_func_t, memcmp_sse4);
  RETURN_FUNC(memcmp_func_t, memcmp_generic);
}
MEMCMP_SHIM()

DEFINE_IFUNC_FOR(memset) {
  __builtin_cpu_init();
  if (__builtin_cpu_is("atom")) RETURN_FUNC(memset_func_t, memset_atom);
  RETURN_FUNC(memset_func_t, memset_generic);
}
MEMSET_SHIM()

DEFINE_IFUNC_FOR(__memset_chk) {
  __builtin_cpu_init();
  if (__builtin_cpu_is("atom")) RETURN_FUNC(__memset_chk_func_t, __memset_chk_atom);
  RETURN_FUNC(__memset_chk_func_t, __memset_chk_generic);
}
__MEMSET_CHK_SHIM()

DEFINE_IFUNC_FOR(memmove) {
  __builtin_cpu_init();
  if (__builtin_cpu_is("atom")) RETURN_FUNC(memmove_func_t, memmove_atom);
  RETURN_FUNC(memmove_func_t, memmove_generic);
}
MEMMOVE_SHIM()

DEFINE_IFUNC_FOR(memcpy) { return memmove_resolver(); }
MEMCPY_SHIM()

DEFINE_IFUNC_FOR(strcpy) {
  __builtin_cpu_init();
  if (__builtin_cpu_is("atom")) RETURN_FUNC(strcpy_func_t, strcpy_atom);
  RETURN_FUNC(strcpy_func_t, strcpy_generic);
}
STRCPY_SHIM()

DEFINE_IFUNC_FOR(strncpy) {
  __builtin_cpu_init();
  if (__builtin_cpu_is("atom")) RETURN_FUNC(strncpy_func_t, strncpy_atom);
  RETURN_FUNC(strncpy_func_t, strncpy_generic);
}
STRNCPY_SHIM()

DEFINE_IFUNC_FOR(strlen) {
  __builtin_cpu_init();
  if (__builtin_cpu_is("atom")) RETURN_FUNC(strlen_func_t, strlen_atom);
  RETURN_FUNC(strlen_func_t, strlen_generic);
}
STRLEN_SHIM()

DEFINE_IFUNC_FOR(strcmp) {
  __builtin_cpu_init();
  // TODO: ssse3 is required by our x86 abi!
  if (__builtin_cpu_supports("ssse3")) RETURN_FUNC(strcmp_func_t, strcmp_ssse3);
  RETURN_FUNC(strcmp_func_t, strcmp_generic);
}
STRCMP_SHIM()

DEFINE_IFUNC_FOR(strncmp) {
  __builtin_cpu_init();
  // TODO: ssse3 is required by our x86 abi!
  if (__builtin_cpu_supports("ssse3"))
    RETURN_FUNC(strncmp_func_t, strncmp_ssse3);
  RETURN_FUNC(strncmp_func_t, strncmp_generic);
}
STRNCMP_SHIM()

DEFINE_IFUNC_FOR(strcat) {
  __builtin_cpu_init();
  // TODO: ssse3 is required by our x86 abi!
  if (__builtin_cpu_supports("ssse3")) RETURN_FUNC(strcat_func_t, strcat_ssse3);
  RETURN_FUNC(strcat_func_t, strcat_generic);
}
STRCAT_SHIM()

DEFINE_IFUNC_FOR(strncat) {
  __builtin_cpu_init();
  // TODO: ssse3 is required by our x86 abi!
  if (__builtin_cpu_supports("ssse3")) RETURN_FUNC(strncat_func_t, strncat_ssse3);
  RETURN_FUNC(strncat_func_t, strncat_openbsd);
}
STRNCAT_SHIM()

typedef size_t strlcat_func_t(char*, const char*, size_t);
DEFINE_IFUNC_FOR(strlcat) {
  __builtin_cpu_init();
  // TODO: ssse3 is required by our x86 abi!
  if (__builtin_cpu_supports("ssse3")) RETURN_FUNC(strlcat_func_t, strlcat_ssse3);
  RETURN_FUNC(strlcat_func_t, strlcat_openbsd);
}
DEFINE_STATIC_SHIM(size_t strlcat(char* dst, const char* src, size_t n) {
  FORWARD(strlcat)(dst, src, n);
})

typedef size_t strlcpy_func_t(char*, const char*, size_t);
DEFINE_IFUNC_FOR(strlcpy) {
  __builtin_cpu_init();
  // TODO: ssse3 is required by our x86 abi!
  if (__builtin_cpu_supports("ssse3")) RETURN_FUNC(strlcpy_func_t, strlcpy_ssse3);
  RETURN_FUNC(strlcpy_func_t, strlcpy_openbsd);
}
DEFINE_STATIC_SHIM(size_t strlcpy(char* dst, const char* src, size_t n) {
  FORWARD(strlcpy)(dst, src, n);
})

typedef wchar_t* wcscat_func_t(wchar_t*, const wchar_t*);
DEFINE_IFUNC_FOR(wcscat) {
  __builtin_cpu_init();
  // TODO: ssse3 is required by our x86 abi!
  if (__builtin_cpu_supports("ssse3")) RETURN_FUNC(wcscat_func_t, wcscat_ssse3);
  RETURN_FUNC(wcscat_func_t, wcscat_freebsd);
}
DEFINE_STATIC_SHIM(wchar_t* wcscat(wchar_t* dst, const wchar_t* src) {
  FORWARD(wcscat)(dst, src);
})

typedef wchar_t* wcscpy_func_t(wchar_t*, const wchar_t*);
DEFINE_IFUNC_FOR(wcscpy) {
  __builtin_cpu_init();
  // TODO: ssse3 is required by our x86 abi!
  if (__builtin_cpu_supports("ssse3")) RETURN_FUNC(wcscpy_func_t, wcscpy_ssse3);
  RETURN_FUNC(wcscpy_func_t, wcscpy_freebsd);
}
DEFINE_STATIC_SHIM(wchar_t* wcscpy(wchar_t* dst, const wchar_t* src) {
  FORWARD(wcscpy)(dst, src);
})

typedef int wmemcmp_func_t(const wchar_t*, const wchar_t*, size_t);
DEFINE_IFUNC_FOR(wmemcmp) {
  __builtin_cpu_init();
  if (__builtin_cpu_supports("sse4.1")) RETURN_FUNC(wmemcmp_func_t, wmemcmp_sse4);
  if (__builtin_cpu_is("atom")) RETURN_FUNC(wmemcmp_func_t, wmemcmp_atom);
  RETURN_FUNC(wmemcmp_func_t, wmemcmp_freebsd);
}
DEFINE_STATIC_SHIM(int wmemcmp(const wchar_t* lhs, const wchar_t* rhs, size_t n) {
  FORWARD(wmemcmp)(lhs, rhs, n);
})

}  // extern "C"
