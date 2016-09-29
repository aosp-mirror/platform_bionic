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
void* memchr(const void* _Nonnull, int, size_t) __attribute_pure__;
void* memrchr(const void* _Nonnull, int, size_t) __attribute_pure__;
int memcmp(const void* _Nonnull, const void* _Nonnull, size_t) __attribute_pure__;
void* memcpy(void* _Nonnull __restrict, const void* _Nonnull __restrict, size_t);
#if defined(__USE_GNU)
void* mempcpy(void* _Nonnull __restrict, const void* _Nonnull __restrict, size_t) __INTRODUCED_IN(23);
#endif
void* memmove(void* _Nonnull, const void* _Nonnull, size_t);
void* memset(void* _Nonnull, int, size_t);
void* memmem(const void* _Nonnull, size_t, const void* _Nonnull, size_t) __attribute_pure__;

char* strchr(const char* _Nonnull, int) __attribute_pure__;
char* __strchr_chk(const char* _Nonnull, int, size_t) __INTRODUCED_IN(18);
#if defined(__USE_GNU)
#if defined(__cplusplus)
extern "C++" char* strchrnul(char* _Nonnull, int) __RENAME(strchrnul) __attribute_pure__;
extern "C++" const char* strchrnul(const char* _Nonnull, int) __RENAME(strchrnul) __attribute_pure__;
#else
char* strchrnul(const char* _Nonnull, int) __attribute_pure__ __INTRODUCED_IN(24);
#endif
#endif

char* strrchr(const char* _Nonnull, int) __attribute_pure__;
char* __strrchr_chk(const char* _Nonnull, int, size_t) __INTRODUCED_IN(18);

size_t strlen(const char* _Nonnull) __attribute_pure__;
size_t __strlen_chk(const char* _Nonnull, size_t) __INTRODUCED_IN(17);
int strcmp(const char* _Nonnull, const char* _Nonnull) __attribute_pure__;
char* stpcpy(char* _Nonnull __restrict, const char* _Nonnull__restrict) __INTRODUCED_IN(21);
char* strcpy(char* _Nonnull __restrict, const char* _Nonnull __restrict);
char* strcat(char* _Nonnull __restrict, const char* _Nonnull __restrict);

char* strdup(const char* _Nonnull);

char* strstr(const char* _Nonnull, const char* _Nonnull) __attribute_pure__;
char* strcasestr(const char* _Nonnull, const char* _Nonnull) __attribute_pure__;
char* strtok(char* __restrict, const char* _Nonnull __restrict);
char* strtok_r(char* __restrict, const char* _Nonnull __restrict, char** _Nonnull __restrict);

char* strerror(int);
char* strerror_l(int, locale_t) __INTRODUCED_IN(23);
#if defined(__USE_GNU)
char* strerror_r(int, char*, size_t) __RENAME(__gnu_strerror_r) __INTRODUCED_IN(23);
#else /* POSIX */
int strerror_r(int, char*, size_t);
#endif

size_t strnlen(const char* _Nonnull, size_t) __attribute_pure__;
char* strncat(char* _Nonnull __restrict, const char* _Nonnull __restrict, size_t);
char* strndup(const char* _Nonnull, size_t);
int strncmp(const char* _Nonnull, const char* _Nonnull, size_t) __attribute_pure__;
char* stpncpy(char* _Nonnull __restrict, const char* _Nonnull __restrict, size_t) __INTRODUCED_IN(21);
char* strncpy(char* _Nonnull __restrict, const char* _Nonnull __restrict, size_t);

size_t strlcat(char* _Nonnull __restrict, const char* _Nonnull __restrict, size_t);
size_t strlcpy(char* _Nonnull __restrict, const char* _Nonnull __restrict, size_t);

size_t strcspn(const char* _Nonnull, const char* _Nonnull) __attribute_pure__;
char* strpbrk(const char* _Nonnull, const char* _Nonnull) __attribute_pure__;
char* strsep(char** _Nonnull __restrict, const char* _Nonnull __restrict);
size_t strspn(const char* _Nonnull, const char* _Nonnull);

char* strsignal(int);

int strcoll(const char* _Nonnull, const char* _Nonnull) __attribute_pure__;
size_t strxfrm(char* __restrict, const char* _Nonnull __restrict, size_t);

#if __ANDROID_API__ >= 21
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
extern "C++" char* basename(char* _Nonnull) __RENAME(__gnu_basename);
extern "C++" const char* basename(const char* _Nonnull) __RENAME(__gnu_basename);
#else
char* basename(const char* _Nonnull) __RENAME(__gnu_basename) __INTRODUCED_IN(23);
#endif
#endif

void* __memchr_chk(const void* _Nonnull, int, size_t, size_t) __INTRODUCED_IN(23);
__errordecl(__memchr_buf_size_error, "memchr called with size bigger than buffer");

void* __memrchr_chk(const void* _Nonnull, int, size_t, size_t) __INTRODUCED_IN(23);
__errordecl(__memrchr_buf_size_error, "memrchr called with size bigger than buffer");
void* __memrchr_real(const void* _Nonnull, int, size_t) __RENAME(memrchr);

char* __stpncpy_chk2(char* _Nonnull __restrict, const char* _Nonnull __restrict, size_t, size_t, size_t)
  __INTRODUCED_IN(21);
char* __strncpy_chk2(char* _Nonnull __restrict, const char* _Nonnull __restrict, size_t, size_t, size_t)
  __INTRODUCED_IN(21);
size_t __strlcpy_real(char* _Nonnull __restrict, const char* _Nonnull __restrict, size_t) __RENAME(strlcpy);
size_t __strlcpy_chk(char* _Nonnull __restrict, const char* _Nonnull __restrict, size_t, size_t) __INTRODUCED_IN(17);
size_t __strlcat_real(char* _Nonnull __restrict, const char* _Nonnull __restrict, size_t) __RENAME(strlcat);
size_t __strlcat_chk(char* _Nonnull __restrict, const char* _Nonnull __restrict, size_t, size_t) __INTRODUCED_IN(17);

#if defined(__BIONIC_FORTIFY)

#if __ANDROID_API__ >= 23
__BIONIC_FORTIFY_INLINE
void* memchr(const void* s, int c, size_t n) {
    size_t bos = __bos(s);

#if !defined(__clang__)
    if (bos == __BIONIC_FORTIFY_UNKNOWN_SIZE) {
        return __builtin_memchr(s, c, n);
    }

    if (__builtin_constant_p(n) && (n > bos)) {
        __memchr_buf_size_error();
    }

    if (__builtin_constant_p(n) && (n <= bos)) {
        return __builtin_memchr(s, c, n);
    }
#endif

    return __memchr_chk(s, c, n, bos);
}

__BIONIC_FORTIFY_INLINE
void* memrchr(const void* s, int c, size_t n) {
    size_t bos = __bos(s);

#if !defined(__clang__)
    if (bos == __BIONIC_FORTIFY_UNKNOWN_SIZE) {
        return __memrchr_real(s, c, n);
    }

    if (__builtin_constant_p(n) && (n > bos)) {
        __memrchr_buf_size_error();
    }

    if (__builtin_constant_p(n) && (n <= bos)) {
        return __memrchr_real(s, c, n);
    }
#endif

    return __memrchr_chk(s, c, n, bos);
}
#endif /* __ANDROID_API__ >= 23 */

#if __ANDROID_API__ >= 17
__BIONIC_FORTIFY_INLINE
void* memcpy(void* _Nonnull __restrict dst, const void* _Nonnull __restrict src, size_t copy_amount) {
    return __builtin___memcpy_chk(dst, src, copy_amount, __bos0(dst));
}

__BIONIC_FORTIFY_INLINE
void* memmove(void* _Nonnull dst, const void* _Nonnull src, size_t len) {
    return __builtin___memmove_chk(dst, src, len, __bos0(dst));
}
#endif /* __ANDROID_API__ >= 17 */

#if __ANDROID_API__ >= 21
__BIONIC_FORTIFY_INLINE
char* stpcpy(char* _Nonnull __restrict dst, const char* _Nonnull __restrict src) {
    return __builtin___stpcpy_chk(dst, src, __bos(dst));
}
#endif /* __ANDROID_API__ >= 21 */

#if __ANDROID_API__ >= 17
__BIONIC_FORTIFY_INLINE
char* strcpy(char* _Nonnull __restrict dst, const char* _Nonnull __restrict src) {
    return __builtin___strcpy_chk(dst, src, __bos(dst));
}
#endif /* __ANDROID_API__ >= 17 */

#if __ANDROID_API__ >= 21
__BIONIC_FORTIFY_INLINE
char* stpncpy(char* _Nonnull __restrict dst, const char* _Nonnull __restrict src, size_t n) {
    size_t bos_dst = __bos(dst);
    size_t bos_src = __bos(src);

    if (bos_src == __BIONIC_FORTIFY_UNKNOWN_SIZE) {
        return __builtin___stpncpy_chk(dst, src, n, bos_dst);
    }

    if (__builtin_constant_p(n) && (n <= bos_src)) {
        return __builtin___stpncpy_chk(dst, src, n, bos_dst);
    }

    size_t slen = __builtin_strlen(src);
    if (__builtin_constant_p(slen)) {
        return __builtin___stpncpy_chk(dst, src, n, bos_dst);
    }

    return __stpncpy_chk2(dst, src, n, bos_dst, bos_src);
}

__BIONIC_FORTIFY_INLINE
char* strncpy(char* _Nonnull __restrict dst, const char* _Nonnull __restrict src, size_t n) {
    size_t bos_dst = __bos(dst);
    size_t bos_src = __bos(src);

    if (bos_src == __BIONIC_FORTIFY_UNKNOWN_SIZE) {
        return __builtin___strncpy_chk(dst, src, n, bos_dst);
    }

    if (__builtin_constant_p(n) && (n <= bos_src)) {
        return __builtin___strncpy_chk(dst, src, n, bos_dst);
    }

    size_t slen = __builtin_strlen(src);
    if (__builtin_constant_p(slen)) {
        return __builtin___strncpy_chk(dst, src, n, bos_dst);
    }

    return __strncpy_chk2(dst, src, n, bos_dst, bos_src);
}
#endif /* __ANDROID_API__ >= 21 */

#if __ANDROID_API__ >= 17
__BIONIC_FORTIFY_INLINE
char* strcat(char* _Nonnull __restrict dst, const char* _Nonnull __restrict src) {
    return __builtin___strcat_chk(dst, src, __bos(dst));
}

__BIONIC_FORTIFY_INLINE
char *strncat(char* _Nonnull __restrict dst, const char* _Nonnull __restrict src, size_t n) {
    return __builtin___strncat_chk(dst, src, n, __bos(dst));
}

__BIONIC_FORTIFY_INLINE
void* memset(void* _Nonnull s, int c, size_t n) {
    return __builtin___memset_chk(s, c, n, __bos0(s));
}

__BIONIC_FORTIFY_INLINE
size_t strlcpy(char* _Nonnull __restrict dst, const char* _Nonnull __restrict src, size_t size) {
    size_t bos = __bos(dst);

#if !defined(__clang__)
    // Compiler doesn't know destination size. Don't call __strlcpy_chk
    if (bos == __BIONIC_FORTIFY_UNKNOWN_SIZE) {
        return __strlcpy_real(dst, src, size);
    }

    // Compiler can prove, at compile time, that the passed in size
    // is always <= the actual object size. Don't call __strlcpy_chk
    if (__builtin_constant_p(size) && (size <= bos)) {
        return __strlcpy_real(dst, src, size);
    }
#endif /* !defined(__clang__) */

    return __strlcpy_chk(dst, src, size, bos);
}


__BIONIC_FORTIFY_INLINE
size_t strlcat(char* _Nonnull __restrict dst, const char* _Nonnull __restrict src, size_t size) {
    size_t bos = __bos(dst);

#if !defined(__clang__)
    // Compiler doesn't know destination size. Don't call __strlcat_chk
    if (bos == __BIONIC_FORTIFY_UNKNOWN_SIZE) {
        return __strlcat_real(dst, src, size);
    }

    // Compiler can prove, at compile time, that the passed in size
    // is always <= the actual object size. Don't call __strlcat_chk
    if (__builtin_constant_p(size) && (size <= bos)) {
        return __strlcat_real(dst, src, size);
    }
#endif /* !defined(__clang__) */

    return __strlcat_chk(dst, src, size, bos);
}

__BIONIC_FORTIFY_INLINE
size_t strlen(const char* _Nonnull s) {
    size_t bos = __bos(s);

#if !defined(__clang__)
    // Compiler doesn't know destination size. Don't call __strlen_chk
    if (bos == __BIONIC_FORTIFY_UNKNOWN_SIZE) {
        return __builtin_strlen(s);
    }

    size_t slen = __builtin_strlen(s);
    if (__builtin_constant_p(slen)) {
        return slen;
    }
#endif /* !defined(__clang__) */

    return __strlen_chk(s, bos);
}
#endif /* __ANDROID_API__ >= 17 */

#if  __ANDROID_API__ >= 18
__BIONIC_FORTIFY_INLINE
char* strchr(const char* _Nonnull s, int c) {
    size_t bos = __bos(s);

#if !defined(__clang__)
    // Compiler doesn't know destination size. Don't call __strchr_chk
    if (bos == __BIONIC_FORTIFY_UNKNOWN_SIZE) {
        return __builtin_strchr(s, c);
    }

    size_t slen = __builtin_strlen(s);
    if (__builtin_constant_p(slen) && (slen < bos)) {
        return __builtin_strchr(s, c);
    }
#endif /* !defined(__clang__) */

    return __strchr_chk(s, c, bos);
}

__BIONIC_FORTIFY_INLINE
char* strrchr(const char* _Nonnull s, int c) {
    size_t bos = __bos(s);

#if !defined(__clang__)
    // Compiler doesn't know destination size. Don't call __strrchr_chk
    if (bos == __BIONIC_FORTIFY_UNKNOWN_SIZE) {
        return __builtin_strrchr(s, c);
    }

    size_t slen = __builtin_strlen(s);
    if (__builtin_constant_p(slen) && (slen < bos)) {
        return __builtin_strrchr(s, c);
    }
#endif /* !defined(__clang__) */

    return __strrchr_chk(s, c, bos);
}
#endif /* __ANDROID_API__ >= 18 */

#endif /* defined(__BIONIC_FORTIFY) */

#if defined(__clang__)
#pragma clang diagnostic pop
#endif

__END_DECLS

#include <android/legacy_string_inlines.h>

#endif /* _STRING_H */
