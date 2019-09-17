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

#include <stddef.h>

#include <private/bionic_ifuncs.h>

extern "C" {

typedef int memcmp_func(const void* __lhs, const void* __rhs, size_t __n);
DEFINE_IFUNC_FOR(memcmp) {
    __builtin_cpu_init();
    if (__builtin_cpu_is("atom")) RETURN_FUNC(memcmp_func, memcmp_atom);
    if (__builtin_cpu_supports("sse4.1")) RETURN_FUNC(memcmp_func, memcmp_sse4);
    RETURN_FUNC(memcmp_func, memcmp_generic);
}

typedef void* memset_func(void* __dst, int __ch, size_t __n);
DEFINE_IFUNC_FOR(memset) {
    __builtin_cpu_init();
    if (__builtin_cpu_is("atom")) RETURN_FUNC(memset_func, memset_atom);
    RETURN_FUNC(memset_func, memset_generic);
}

typedef void* __memset_chk_func(void *s, int c, size_t n, size_t n2);
DEFINE_IFUNC_FOR(__memset_chk) {
    __builtin_cpu_init();
    if (__builtin_cpu_is("atom")) RETURN_FUNC(__memset_chk_func, __memset_chk_atom);
    RETURN_FUNC(__memset_chk_func, __memset_chk_generic);
}

typedef void* memmove_func(void* __dst, const void* __src, size_t __n);
DEFINE_IFUNC_FOR(memmove) {
    __builtin_cpu_init();
    if (__builtin_cpu_is("atom")) RETURN_FUNC(memmove_func, memmove_atom);
    RETURN_FUNC(memmove_func, memmove_generic);
}

typedef void* memcpy_func(void*, const void*, size_t);
DEFINE_IFUNC_FOR(memcpy) {
    return memmove_resolver();
}

typedef char* strcpy_func(char* __dst, const char* __src);
DEFINE_IFUNC_FOR(strcpy) {
    __builtin_cpu_init();
    if (__builtin_cpu_is("atom")) RETURN_FUNC(strcpy_func, strcpy_atom);
    RETURN_FUNC(strcpy_func, strcpy_generic);
}

typedef char* strncpy_func(char* __dst, const char* __src, size_t __n);
DEFINE_IFUNC_FOR(strncpy) {
    __builtin_cpu_init();
    if (__builtin_cpu_is("atom")) RETURN_FUNC(strncpy_func, strncpy_atom);
    RETURN_FUNC(strncpy_func, strncpy_generic);
}

typedef size_t strlen_func(const char* __s);
DEFINE_IFUNC_FOR(strlen) {
    __builtin_cpu_init();
    if (__builtin_cpu_is("atom")) RETURN_FUNC(strlen_func, strlen_atom);
    RETURN_FUNC(strlen_func, strlen_generic);
}

typedef int wmemcmp_func(const wchar_t* __lhs, const wchar_t* __rhs, size_t __n);
DEFINE_IFUNC_FOR(wmemcmp) {
    __builtin_cpu_init();
    if (__builtin_cpu_supports("sse4.1")) RETURN_FUNC(wmemcmp_func, wmemcmp_sse4);
    if (__builtin_cpu_is("atom")) RETURN_FUNC(wmemcmp_func, wmemcmp_atom);
    RETURN_FUNC(wmemcmp_func, wmemcmp_freebsd);
}

typedef int wmemset_func(const wchar_t* __lhs, const wchar_t* __rhs, size_t __n);
DEFINE_IFUNC_FOR(wmemset) {
    __builtin_cpu_init();
    if (__builtin_cpu_supports("avx2")) RETURN_FUNC(wmemset_func, wmemset_avx2);
    RETURN_FUNC(wmemset_func, wmemset_freebsd);
}

typedef int strcmp_func(const char* __lhs, const char* __rhs);
DEFINE_IFUNC_FOR(strcmp) {
    __builtin_cpu_init();
    if (__builtin_cpu_supports("ssse3")) RETURN_FUNC(strcmp_func, strcmp_ssse3);
    RETURN_FUNC(strcmp_func, strcmp_generic);
}

typedef int strncmp_func(const char* __lhs, const char* __rhs, size_t __n);
DEFINE_IFUNC_FOR(strncmp) {
    __builtin_cpu_init();
    if (__builtin_cpu_supports("ssse3")) RETURN_FUNC(strncmp_func, strncmp_ssse3);
    RETURN_FUNC(strncmp_func, strncmp_generic);
}

typedef char* strcat_func(char* __dst, const char* __src);
DEFINE_IFUNC_FOR(strcat) {
    __builtin_cpu_init();
    if (__builtin_cpu_supports("ssse3")) RETURN_FUNC(strcat_func, strcat_ssse3);
    RETURN_FUNC(strcat_func, strcat_generic);
}

typedef char* strncat_func(char* __dst, const char* __src, size_t __n);
DEFINE_IFUNC_FOR(strncat) {
    __builtin_cpu_init();
    if (__builtin_cpu_supports("ssse3")) RETURN_FUNC(strncat_func, strncat_ssse3);
    RETURN_FUNC(strncat_func, strncat_openbsd);
}

typedef size_t strlcat_func(char *dst, const char *src, size_t dsize);
DEFINE_IFUNC_FOR(strlcat) {
    __builtin_cpu_init();
    if (__builtin_cpu_supports("ssse3")) RETURN_FUNC(strlcat_func, strlcat_ssse3);
    RETURN_FUNC(strlcat_func, strlcat_openbsd);
}

typedef size_t strlcpy_func(char *dst, const char *src, size_t dsize);
DEFINE_IFUNC_FOR(strlcpy) {
    __builtin_cpu_init();
    if (__builtin_cpu_supports("ssse3")) RETURN_FUNC(strlcpy_func, strlcpy_ssse3);
    RETURN_FUNC(strlcpy_func, strlcpy_openbsd);
}

typedef wchar_t* wcscat_func(wchar_t *s1, const wchar_t *s2);
DEFINE_IFUNC_FOR(wcscat) {
    __builtin_cpu_init();
    if (__builtin_cpu_supports("ssse3")) RETURN_FUNC(wcscat_func, wcscat_ssse3);
    RETURN_FUNC(wcscat_func, wcscat_freebsd);
}

typedef wchar_t* wcscpy_func(wchar_t *s1, const wchar_t *s2);
DEFINE_IFUNC_FOR(wcscpy) {
    __builtin_cpu_init();
    if (__builtin_cpu_supports("ssse3")) RETURN_FUNC(wcscpy_func, wcscpy_ssse3);
    RETURN_FUNC(wcscpy_func, wcscpy_freebsd);
}

}  // extern "C"
