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

#if defined(__USE_BSD)
#include <strings.h>
#endif

void* memccpy(void*, const void*, int, size_t);
void* memchr(const void*, int, size_t) __attribute_pure__ __overloadable __RENAME_CLANG(memchr);
void* memrchr(const void*, int, size_t) __attribute_pure__ __overloadable __RENAME_CLANG(memrchr);
int memcmp(const void*, const void*, size_t) __attribute_pure__;
void* memcpy(void*, const void*, size_t)
        __overloadable __RENAME_CLANG(memcpy);
#if defined(__USE_GNU)
void* mempcpy(void*, const void*, size_t) __INTRODUCED_IN(23);
#endif
void* memmove(void*, const void*, size_t) __overloadable __RENAME_CLANG(memmove);
void* memset(void*, int, size_t) __overloadable __RENAME_CLANG(memset);
void* memmem(const void*, size_t, const void*, size_t) __attribute_pure__;

char* strchr(const char*, int) __attribute_pure__ __overloadable __RENAME_CLANG(strchr);
char* __strchr_chk(const char*, int, size_t) __INTRODUCED_IN(18);
#if defined(__USE_GNU)
#if defined(__cplusplus)
extern "C++" char* strchrnul(char*, int) __RENAME(strchrnul) __attribute_pure__ __INTRODUCED_IN(24);
extern "C++" const char* strchrnul(const char*, int) __RENAME(strchrnul) __attribute_pure__ __INTRODUCED_IN(24);
#else
char* strchrnul(const char*, int) __attribute_pure__ __INTRODUCED_IN(24);
#endif
#endif

char* strrchr(const char*, int) __attribute_pure__ __overloadable __RENAME_CLANG(strrchr);
char* __strrchr_chk(const char*, int, size_t) __INTRODUCED_IN(18);

size_t strlen(const char*) __attribute_pure__ __overloadable
        __RENAME_CLANG(strlen);
size_t __strlen_chk(const char*, size_t) __INTRODUCED_IN(17);

int strcmp(const char*, const char*) __attribute_pure__;
char* stpcpy(char*, const char*) __overloadable __RENAME_CLANG(stpcpy) __INTRODUCED_IN(21);
char* strcpy(char*, const char*)
        __overloadable __RENAME_CLANG(strcpy);
char* strcat(char*, const char*) __overloadable __RENAME_CLANG(strcat);
char* strdup(const char*);

char* strstr(const char*, const char*) __attribute_pure__;
char* strcasestr(const char*, const char*) __attribute_pure__;
char* strtok(char*, const char*);
char* strtok_r(char*, const char*, char**);

char* strerror(int);
char* strerror_l(int, locale_t) __INTRODUCED_IN(23);
#if defined(__USE_GNU) && __ANDROID_API__ >= 23
char* strerror_r(int, char*, size_t) __RENAME(__gnu_strerror_r) __INTRODUCED_IN(23);
#else /* POSIX */
int strerror_r(int, char*, size_t);
#endif

size_t strnlen(const char*, size_t) __attribute_pure__;
char* strncat(char*, const char*, size_t) __overloadable __RENAME_CLANG(strncat);
char* strndup(const char*, size_t);
int strncmp(const char*, const char*, size_t) __attribute_pure__;
char* stpncpy(char*, const char*, size_t) __overloadable __RENAME_CLANG(stpncpy) __INTRODUCED_IN(21);
char* strncpy(char*, const char*, size_t) __overloadable __RENAME_CLANG(strncpy);

size_t strlcat(char*, const char*, size_t) __overloadable __RENAME_CLANG(strlcat);
size_t strlcpy(char*, const char*, size_t) __overloadable __RENAME_CLANG(strlcpy);

size_t strcspn(const char*, const char*) __attribute_pure__;
char* strpbrk(const char*, const char*) __attribute_pure__;
char* strsep(char**, const char*);
size_t strspn(const char*, const char*);

char* strsignal(int);

int strcoll(const char*, const char*) __attribute_pure__;
size_t strxfrm(char*, const char*, size_t);

#if __ANDROID_API__ >= __ANDROID_API_L__
int strcoll_l(const char*, const char*, locale_t) __attribute_pure__ __INTRODUCED_IN(21);
size_t strxfrm_l(char*, const char*, size_t, locale_t) __INTRODUCED_IN(21);
#else
// Implemented as static inlines before 21.
#endif

#if defined(__USE_GNU) && !defined(basename)
/*
 * glibc has a basename in <string.h> that's different to the POSIX one in <libgen.h>.
 * It doesn't modify its argument, and in C++ it's const-correct.
 */
#if defined(__cplusplus)
extern "C++" char* basename(char*) __RENAME(__gnu_basename) __INTRODUCED_IN(23);
extern "C++" const char* basename(const char*) __RENAME(__gnu_basename) __INTRODUCED_IN(23);
#else
char* basename(const char*) __RENAME(__gnu_basename) __INTRODUCED_IN(23);
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
void* __bionic_memchr(const void* const s __pass_object_size, int c, size_t n) {
    return memchr(s, c, n);
}

inline __always_inline
const void* memchr(const void* const s __pass_object_size, int c, size_t n)
        __prefer_this_overload {
    return __bionic_memchr(s, c, n);
}

inline __always_inline
void* memchr(void* const s __pass_object_size, int c, size_t n) __prefer_this_overload {
    return __bionic_memchr(s, c, n);
}

inline __always_inline
char* __bionic_strchr(const char* const s __pass_object_size, int c) {
    return strchr(s, c);
}

inline __always_inline
const char* strchr(const char* const s __pass_object_size, int c)
        __prefer_this_overload {
    return __bionic_strchr(s, c);
}

inline __always_inline
char* strchr(char* const s __pass_object_size, int c)
        __prefer_this_overload {
    return __bionic_strchr(s, c);
}

inline __always_inline
char* __bionic_strrchr(const char* const s __pass_object_size, int c) {
    return strrchr(s, c);
}

inline __always_inline
const char* strrchr(const char* const s __pass_object_size, int c) __prefer_this_overload {
    return __bionic_strrchr(s, c);
}

inline __always_inline
char* strrchr(char* const s __pass_object_size, int c) __prefer_this_overload {
    return __bionic_strrchr(s, c);
}

/* Functions with no FORTIFY counterpart. */
inline __always_inline
char* __bionic_strstr(const char* h, const char* n) { return strstr(h, n); }

inline __always_inline
const char* strstr(const char* h, const char* n) __prefer_this_overload {
    return __bionic_strstr(h, n);
}

inline __always_inline
char* strstr(char* h, const char* n) __prefer_this_overload {
    return __bionic_strstr(h, n);
}

inline __always_inline
char* __bionic_strpbrk(const char* h, const char* n) { return strpbrk(h, n); }

inline __always_inline
char* strpbrk(char* h, const char* n) __prefer_this_overload {
    return __bionic_strpbrk(h, n);
}

inline __always_inline
const char* strpbrk(const char* h, const char* n) __prefer_this_overload {
    return __bionic_strpbrk(h, n);
}
}
#undef __prefer_this_overload
#endif

__END_DECLS

#endif /* _STRING_H */
