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

#include <platform/bionic/mte_kernel.h>
#include <private/bionic_ifuncs.h>
#include <stddef.h>
#include <sys/auxv.h>

extern "C" {

static bool supports_mte(unsigned long hwcap2) {
#ifdef ANDROID_EXPERIMENTAL_MTE
    return hwcap2 & HWCAP2_MTE;
#else
    (void)hwcap2;
    return false;
#endif
}

typedef void* memchr_func(const void*, int, size_t);
DEFINE_IFUNC_FOR(memchr) {
    if (supports_mte(arg->_hwcap2)) {
        RETURN_FUNC(memchr_func, memchr_mte);
    } else {
        RETURN_FUNC(memchr_func, memchr_default);
    }
}

typedef char* strchr_func(const char*, int);
DEFINE_IFUNC_FOR(strchr) {
    if (supports_mte(arg->_hwcap2)) {
        RETURN_FUNC(strchr_func, strchr_mte);
    } else {
        RETURN_FUNC(strchr_func, strchr_default);
    }
}

typedef int strcmp_func(const char*, const char*);
DEFINE_IFUNC_FOR(strcmp) {
    if (supports_mte(arg->_hwcap2)) {
        RETURN_FUNC(strcmp_func, strcmp_mte);
    } else {
        RETURN_FUNC(strcmp_func, strcmp_default);
    }
}

typedef size_t strlen_func(const char*);
DEFINE_IFUNC_FOR(strlen) {
    if (supports_mte(arg->_hwcap2)) {
        RETURN_FUNC(strlen_func, strlen_mte);
    } else {
        RETURN_FUNC(strlen_func, strlen_default);
    }
}

typedef int strncmp_func(const char*, const char*, int);
DEFINE_IFUNC_FOR(strncmp) {
    if (supports_mte(arg->_hwcap2)) {
        RETURN_FUNC(strncmp_func, strncmp_mte);
    } else {
        RETURN_FUNC(strncmp_func, strncmp_default);
    }
}

typedef size_t strnlen_func(const char*, int);
DEFINE_IFUNC_FOR(strnlen) {
    if (supports_mte(arg->_hwcap2)) {
        RETURN_FUNC(strnlen_func, strnlen_mte);
    } else {
        RETURN_FUNC(strnlen_func, strnlen_default);
    }
}

}  // extern "C"
