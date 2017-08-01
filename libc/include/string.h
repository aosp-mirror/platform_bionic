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

#ifndef _STRING_H
#define _STRING_H

#include <sys/cdefs.h>
#include <stddef.h>
#include <xlocale.h>

#include <bits/strcasecmp.h>

__BEGIN_DECLS

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wnullability-completeness"
#endif

#if defined(__USE_BSD)
#include <strings.h>
#endif

void* memccpy(void* _Nonnull __restrict, const void* _Nonnull __restrict, int, size_t);
void* memchr(const void* _Nonnull, int, size_t) __attribute_pure__ __overloadable
        __RENAME_CLANG(memchr);
void* memrchr(const void* _Nonnull, int, size_t) __attribute_pure__ __overloadable
        __RENAME_CLANG(memrchr);
int memcmp(const void* _Nonnull, const void* _Nonnull, size_t) __attribute_pure__;
void* memcpy(void* _Nonnull __restrict, const void* _Nonnull __restrict, size_t)
        __overloadable __RENAME_CLANG(memcpy);
#if defined(__USE_GNU)
void* mempcpy(void* _Nonnull __restrict, const void* _Nonnull __restrict, size_t) __INTRODUCED_IN(23);
#endif
void* memmove(void* _Nonnull, const void* _Nonnull, size_t) __overloadable
        __RENAME_CLANG(memmove);
void* memset(void* _Nonnull, int, size_t) __overloadable __RENAME_CLANG(memset);
void* memmem(const void* _Nonnull, size_t, const void* _Nonnull, size_t) __attribute_pure__;

char* strchr(const char* _Nonnull, int) __attribute_pure__ __overloadable
        __RENAME_CLANG(strchr);
char* __strchr_chk(const char* _Nonnull, int, size_t) __INTRODUCED_IN(18);
#if defined(__USE_GNU)
#if defined(__cplusplus)
extern "C++" char* strchrnul(char* _Nonnull, int) __RENAME(strchrnul) __attribute_pure__ __INTRODUCED_IN(24);
extern "C++" const char* strchrnul(const char* _Nonnull, int) __RENAME(strchrnul) __attribute_pure__ __INTRODUCED_IN(24);
#else
char* strchrnul(const char* _Nonnull, int) __attribute_pure__ __INTRODUCED_IN(24);
#endif
#endif

char* strrchr(const char* _Nonnull, int) __attribute_pure__ __overloadable
        __RENAME_CLANG(strrchr);
char* __strrchr_chk(const char* _Nonnull, int, size_t) __INTRODUCED_IN(18);

size_t strlen(const char* _Nonnull) __attribute_pure__ __overloadable
        __RENAME_CLANG(strlen);
size_t __strlen_chk(const char* _Nonnull, size_t) __INTRODUCED_IN(17);

int strcmp(const char* _Nonnull, const char* _Nonnull) __attribute_pure__;
char* stpcpy(char* _Nonnull __restrict, const char* _Nonnull __restrict)
        __overloadable __RENAME_CLANG(stpcpy) __INTRODUCED_IN(21);
char* strcpy(char* _Nonnull __restrict, const char* _Nonnull __restrict)
        __overloadable __RENAME_CLANG(strcpy);
char* strcat(char* _Nonnull __restrict, const char* _Nonnull __restrict)
        __overloadable __RENAME_CLANG(strcat);
char* strdup(const char* _Nonnull);

char* strstr(const char* _Nonnull, const char* _Nonnull) __attribute_pure__;
char* strcasestr(const char* _Nonnull, const char* _Nonnull) __attribute_pure__;
char* strtok(char* __restrict, const char* _Nonnull __restrict);
char* strtok_r(char* __restrict, const char* _Nonnull __restrict, char** _Nonnull __restrict);

char* strerror(int);
char* strerror_l(int, locale_t) __INTRODUCED_IN(23);
#if defined(__USE_GNU) && __ANDROID_API__ >= 23
char* strerror_r(int, char*, size_t) __RENAME(__gnu_strerror_r) __INTRODUCED_IN(23);
#else /* POSIX */
int strerror_r(int, char*, size_t);
#endif

size_t strnlen(const char* _Nonnull, size_t) __attribute_pure__;
char* strncat(char* _Nonnull __restrict, const char* _Nonnull __restrict, size_t)
        __overloadable __RENAME_CLANG(strncat);
char* strndup(const char* _Nonnull, size_t);
int strncmp(const char* _Nonnull, const char* _Nonnull, size_t) __attribute_pure__;
char* stpncpy(char* _Nonnull __restrict, const char* _Nonnull __restrict, size_t)
        __overloadable __RENAME_CLANG(stpncpy) __INTRODUCED_IN(21);
char* strncpy(char* _Nonnull __restrict, const char* _Nonnull __restrict, size_t)
        __overloadable __RENAME_CLANG(strncpy);

size_t strlcat(char* _Nonnull __restrict, const char* _Nonnull __restrict, size_t)
        __overloadable __RENAME_CLANG(strlcat);
size_t strlcpy(char* _Nonnull __restrict, const char* _Nonnull __restrict, size_t)
        __overloadable __RENAME_CLANG(strlcpy);

size_t strcspn(const char* _Nonnull, const char* _Nonnull) __attribute_pure__;
char* strpbrk(const char* _Nonnull, const char* _Nonnull) __attribute_pure__;
char* strsep(char** _Nonnull __restrict, const char* _Nonnull __restrict);
size_t strspn(const char* _Nonnull, const char* _Nonnull);

char* strsignal(int);

int strcoll(const char* _Nonnull, const char* _Nonnull) __attribute_pure__;
size_t strxfrm(char* __restrict, const char* _Nonnull __restrict, size_t);

#if __ANDROID_API__ >= __ANDROID_API_L__
int strcoll_l(const char* _Nonnull, const char* _Nonnull, locale_t) __attribute_pure__ __INTRODUCED_IN(21);
size_t strxfrm_l(char* __restrict, const char* _Nonnull __restrict, size_t, locale_t) __INTRODUCED_IN(21);
#else
// Implemented as static inlines before 21.
#endif

#if defined(__USE_GNU) && !defined(basename)
/*
 * glibc has a basename in <string.h> that's different to the POSIX one in <libgen.h>.
 * It doesn't modify its argument, and in C++ it's const-correct.
 */
#if defined(__cplusplus)
extern "C++" char* basename(char* _Nonnull) __RENAME(__gnu_basename) __INTRODUCED_IN(23);
extern "C++" const char* basename(const char* _Nonnull) __RENAME(__gnu_basename) __INTRODUCED_IN(23);
#else
char* basename(const char* _Nonnull) __RENAME(__gnu_basename) __INTRODUCED_IN(23);
#endif
#endif

#if defined(__BIONIC_INCLUDE_FORTIFY_HEADERS)
#include <bits/fortify/string.h>
#endif

/* Const-correct overloads. Placed after FORTIFY so we call those functions, if possible. */
#if defined(__cplusplus) && defined(__clang__)
/*
 * Use two enable_ifs so these overloads don't conflict with + are preferred over libcxx's. This can
 * be reduced to 1 after libcxx recognizes that we have const-correct overloads.
 */
#define __prefer_this_overload __enable_if(true, "preferred overload") __enable_if(true, "")
extern "C++" {
inline __always_inline
void* __bionic_memchr(const void* const _Nonnull s __pass_object_size, int c, size_t n) {
    return memchr(s, c, n);
}

inline __always_inline
const void* memchr(const void* const _Nonnull s __pass_object_size, int c, size_t n)
        __prefer_this_overload {
    return __bionic_memchr(s, c, n);
}

inline __always_inline
void* memchr(void* const _Nonnull s __pass_object_size, int c, size_t n) __prefer_this_overload {
    return __bionic_memchr(s, c, n);
}

inline __always_inline
char* __bionic_strchr(const char* const _Nonnull s __pass_object_size, int c) {
    return strchr(s, c);
}

inline __always_inline
const char* strchr(const char* const _Nonnull s __pass_object_size, int c)
        __prefer_this_overload {
    return __bionic_strchr(s, c);
}

inline __always_inline
char* strchr(char* const _Nonnull s __pass_object_size, int c)
        __prefer_this_overload {
    return __bionic_strchr(s, c);
}

inline __always_inline
char* __bionic_strrchr(const char* const _Nonnull s __pass_object_size, int c) {
    return strrchr(s, c);
}

inline __always_inline
const char* strrchr(const char* const _Nonnull s __pass_object_size, int c) __prefer_this_overload {
    return __bionic_strrchr(s, c);
}

inline __always_inline
char* strrchr(char* const _Nonnull s __pass_object_size, int c) __prefer_this_overload {
    return __bionic_strrchr(s, c);
}

/* Functions with no FORTIFY counterpart. */
inline __always_inline
char* __bionic_strstr(const char* _Nonnull h, const char* _Nonnull n) { return strstr(h, n); }

inline __always_inline
const char* strstr(const char* _Nonnull h, const char* _Nonnull n) __prefer_this_overload {
    return __bionic_strstr(h, n);
}

inline __always_inline
char* strstr(char* _Nonnull h, const char* _Nonnull n) __prefer_this_overload {
    return __bionic_strstr(h, n);
}

inline __always_inline
char* __bionic_strpbrk(const char* _Nonnull h, const char* _Nonnull n) { return strpbrk(h, n); }

inline __always_inline
char* strpbrk(char* _Nonnull h, const char* _Nonnull n) __prefer_this_overload {
    return __bionic_strpbrk(h, n);
}

inline __always_inline
const char* strpbrk(const char* _Nonnull h, const char* _Nonnull n) __prefer_this_overload {
    return __bionic_strpbrk(h, n);
}
}
#undef __prefer_this_overload
#endif

#if defined(__clang__)
#pragma clang diagnostic pop
#endif

__END_DECLS

#endif /* _STRING_H */
