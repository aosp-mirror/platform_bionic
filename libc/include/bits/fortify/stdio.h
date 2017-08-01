/*
 * Copyright (C) 2017 The Android Open Source Project
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

#ifndef _STDIO_H_
#error "Never include this file directly; instead, include <stdio.h>"
#endif

char* __fgets_chk(char*, int, FILE*, size_t) __INTRODUCED_IN(17);
size_t __fread_chk(void* __restrict, size_t, size_t, FILE* __restrict, size_t)
    __INTRODUCED_IN(24);
size_t __fwrite_chk(const void* __restrict, size_t, size_t, FILE* __restrict, size_t)
    __INTRODUCED_IN(24);

#if defined(__BIONIC_FORTIFY) && !defined(__BIONIC_NO_STDIO_FORTIFY)

#if __ANDROID_API__ >= __ANDROID_API_J_MR1__
__BIONIC_FORTIFY_INLINE __printflike(3, 0)
int vsnprintf(char *const __pass_object_size dest, size_t size,
              const char *_Nonnull format, __va_list ap) __overloadable {
    return __builtin___vsnprintf_chk(dest, size, 0, __bos(dest), format, ap);
}

__BIONIC_FORTIFY_INLINE __printflike(2, 0)
int vsprintf(char *const __pass_object_size dest, const char *_Nonnull format,
             __va_list ap) __overloadable {
    return __builtin___vsprintf_chk(dest, 0, __bos(dest), format, ap);
}
#endif /* __ANDROID_API__ >= __ANDROID_API_J_MR1__ */

#if defined(__clang__)
#if __ANDROID_API__ >= __ANDROID_API_J_MR1__
/*
 * Simple case: `format` can't have format specifiers, so we can just compare
 * its length to the length of `dest`
 */
__BIONIC_ERROR_FUNCTION_VISIBILITY
int snprintf(char *__restrict dest, size_t size, const char *__restrict format)
    __overloadable
    __enable_if(__bos(dest) != __BIONIC_FORTIFY_UNKNOWN_SIZE &&
                __bos(dest) < __builtin_strlen(format),
                "format string will always overflow destination buffer")
    __errorattr("format string will always overflow destination buffer");

__BIONIC_FORTIFY_INLINE
__printflike(3, 4)
int snprintf(char *__restrict const __pass_object_size dest,
             size_t size, const char *__restrict format, ...) __overloadable {
    va_list va;
    va_start(va, format);
    int result = __builtin___vsnprintf_chk(dest, size, 0, __bos(dest), format, va);
    va_end(va);
    return result;
}

__BIONIC_ERROR_FUNCTION_VISIBILITY
int sprintf(char *__restrict dest, const char *__restrict format) __overloadable
    __enable_if(__bos(dest) != __BIONIC_FORTIFY_UNKNOWN_SIZE &&
                __bos(dest) < __builtin_strlen(format),
                "format string will always overflow destination buffer")
    __errorattr("format string will always overflow destination buffer");

__BIONIC_FORTIFY_INLINE
__printflike(2, 3)
int sprintf(char *__restrict const __pass_object_size dest,
        const char *__restrict format, ...) __overloadable {
    va_list va;
    va_start(va, format);
    int result = __builtin___vsprintf_chk(dest, 0, __bos(dest), format, va);
    va_end(va);
    return result;
}
#endif /* __ANDROID_API__ >= __ANDROID_API_J_MR1__ */

#if __ANDROID_API__ >= __ANDROID_API_N__
__BIONIC_FORTIFY_INLINE
size_t fread(void *__restrict buf, size_t size, size_t count,
             FILE *__restrict stream) __overloadable
        __enable_if(__unsafe_check_mul_overflow(size, count), "size * count overflows")
        __errorattr("size * count overflows");

__BIONIC_FORTIFY_INLINE
size_t fread(void *__restrict buf, size_t size, size_t count,
             FILE *__restrict stream) __overloadable
    __enable_if(!__unsafe_check_mul_overflow(size, count), "no overflow")
    __enable_if(__bos(buf) != __BIONIC_FORTIFY_UNKNOWN_SIZE &&
                size * count > __bos(buf), "size * count is too large")
    __errorattr("size * count is too large");

__BIONIC_FORTIFY_INLINE
size_t fread(void *__restrict const __pass_object_size0 buf, size_t size,
             size_t count, FILE *__restrict stream) __overloadable {
    size_t bos = __bos0(buf);

    if (bos == __BIONIC_FORTIFY_UNKNOWN_SIZE) {
        return __call_bypassing_fortify(fread)(buf, size, count, stream);
    }

    return __fread_chk(buf, size, count, stream, bos);
}

size_t fwrite(const void * __restrict buf, size_t size,
              size_t count, FILE * __restrict stream) __overloadable
    __enable_if(__unsafe_check_mul_overflow(size, count),
                "size * count overflows")
    __errorattr("size * count overflows");

size_t fwrite(const void * __restrict buf, size_t size,
              size_t count, FILE * __restrict stream) __overloadable
    __enable_if(!__unsafe_check_mul_overflow(size, count), "no overflow")
    __enable_if(__bos(buf) != __BIONIC_FORTIFY_UNKNOWN_SIZE &&
                size * count > __bos(buf), "size * count is too large")
    __errorattr("size * count is too large");

__BIONIC_FORTIFY_INLINE
size_t fwrite(const void * __restrict const __pass_object_size0 buf,
              size_t size, size_t count, FILE * __restrict stream)
        __overloadable {
    size_t bos = __bos0(buf);

    if (bos == __BIONIC_FORTIFY_UNKNOWN_SIZE) {
        return __call_bypassing_fortify(fwrite)(buf, size, count, stream);
    }

    return __fwrite_chk(buf, size, count, stream, bos);
}
#endif /* __ANDROID_API__ >= __ANDROID_API_N__ */

#if __ANDROID_API__ >= __ANDROID_API_J_MR1__
__BIONIC_ERROR_FUNCTION_VISIBILITY
char *fgets(char* __restrict dest, int size, FILE* stream) __overloadable
    __enable_if(size < 0, "size is negative")
    __errorattr("size is negative");

__BIONIC_ERROR_FUNCTION_VISIBILITY
char *fgets(char* dest, int size, FILE* stream) __overloadable
    __enable_if(size >= 0 && size > __bos(dest),
                "size is larger than the destination buffer")
    __errorattr("size is larger than the destination buffer");

__BIONIC_FORTIFY_INLINE
char *fgets(char* __restrict const __pass_object_size dest,
        int size, FILE* stream) __overloadable {
    size_t bos = __bos(dest);

    if (bos == __BIONIC_FORTIFY_UNKNOWN_SIZE) {
        return __call_bypassing_fortify(fgets)(dest, size, stream);
    }

    return __fgets_chk(dest, size, stream, bos);
}
#endif /* __ANDROID_API__ >= __ANDROID_API_J_MR1__ */

#else /* defined(__clang__) */

size_t __fread_real(void * __restrict, size_t, size_t, FILE * __restrict) __RENAME(fread);
__errordecl(__fread_too_big_error, "fread called with size * count bigger than buffer");
__errordecl(__fread_overflow, "fread called with overflowing size * count");

char* __fgets_real(char*, int, FILE*) __RENAME(fgets);
__errordecl(__fgets_too_big_error, "fgets called with size bigger than buffer");
__errordecl(__fgets_too_small_error, "fgets called with size less than zero");

size_t __fwrite_real(const void * __restrict, size_t, size_t, FILE * __restrict) __RENAME(fwrite);
__errordecl(__fwrite_too_big_error, "fwrite called with size * count bigger than buffer");
__errordecl(__fwrite_overflow, "fwrite called with overflowing size * count");


#if __ANDROID_API__ >= __ANDROID_API_J_MR1__
__BIONIC_FORTIFY_INLINE __printflike(3, 4)
int snprintf(char *__restrict dest, size_t size, const char* _Nonnull format, ...)
{
    return __builtin___snprintf_chk(dest, size, 0, __bos(dest), format,
                                    __builtin_va_arg_pack());
}

__BIONIC_FORTIFY_INLINE __printflike(2, 3)
int sprintf(char *__restrict dest, const char* _Nonnull format, ...) {
    return __builtin___sprintf_chk(dest, 0, __bos(dest), format,
                                   __builtin_va_arg_pack());
}
#endif /* __ANDROID_API__ >= __ANDROID_API_J_MR1__ */

#if __ANDROID_API__ >= __ANDROID_API_N__
__BIONIC_FORTIFY_INLINE
size_t fread(void *__restrict buf, size_t size, size_t count, FILE * __restrict stream) {
    size_t bos = __bos0(buf);

    if (bos == __BIONIC_FORTIFY_UNKNOWN_SIZE) {
        return __fread_real(buf, size, count, stream);
    }

    if (__builtin_constant_p(size) && __builtin_constant_p(count)) {
        size_t total;
        if (__size_mul_overflow(size, count, &total)) {
            __fread_overflow();
        }

        if (total > bos) {
            __fread_too_big_error();
        }

        return __fread_real(buf, size, count, stream);
    }

    return __fread_chk(buf, size, count, stream, bos);
}

__BIONIC_FORTIFY_INLINE
size_t fwrite(const void * __restrict buf, size_t size, size_t count, FILE * __restrict stream) {
    size_t bos = __bos0(buf);

    if (bos == __BIONIC_FORTIFY_UNKNOWN_SIZE) {
        return __fwrite_real(buf, size, count, stream);
    }

    if (__builtin_constant_p(size) && __builtin_constant_p(count)) {
        size_t total;
        if (__size_mul_overflow(size, count, &total)) {
            __fwrite_overflow();
        }

        if (total > bos) {
            __fwrite_too_big_error();
        }

        return __fwrite_real(buf, size, count, stream);
    }

    return __fwrite_chk(buf, size, count, stream, bos);
}
#endif /* __ANDROID_API__ >= __ANDROID_API_N__ */

#if __ANDROID_API__ >= __ANDROID_API_J_MR1__
__BIONIC_FORTIFY_INLINE
char *fgets(char* dest, int size, FILE* stream) {
    size_t bos = __bos(dest);

    // Compiler can prove, at compile time, that the passed in size
    // is always negative. Force a compiler error.
    if (__builtin_constant_p(size) && (size < 0)) {
        __fgets_too_small_error();
    }

    // Compiler doesn't know destination size. Don't call __fgets_chk
    if (bos == __BIONIC_FORTIFY_UNKNOWN_SIZE) {
        return __fgets_real(dest, size, stream);
    }

    // Compiler can prove, at compile time, that the passed in size
    // is always <= the actual object size. Don't call __fgets_chk
    if (__builtin_constant_p(size) && (size <= (int) bos)) {
        return __fgets_real(dest, size, stream);
    }

    // Compiler can prove, at compile time, that the passed in size
    // is always > the actual object size. Force a compiler error.
    if (__builtin_constant_p(size) && (size > (int) bos)) {
        __fgets_too_big_error();
    }

    return __fgets_chk(dest, size, stream, bos);
}
#endif /* __ANDROID_API__ >= __ANDROID_API_J_MR1__ */

#endif /* defined(__clang__) */
#endif /* defined(__BIONIC_FORTIFY) */
