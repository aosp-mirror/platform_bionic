/*
 * Copyright (C) 2023 The Android Open Source Project
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
#include <sys/auxv.h>

#if defined(__riscv_v)
extern "C" {

typedef void* memchr_func(const void*, int, size_t);
DEFINE_IFUNC_FOR(memchr) {
  RETURN_FUNC(memchr_func, memchr_vext);
}

typedef int memcmp_func(const void*, const void*, size_t);
DEFINE_IFUNC_FOR(memcmp) {
  RETURN_FUNC(memcmp_func, memcmp_vext);
}

typedef void* memcpy_func(void*, const void*, size_t);
DEFINE_IFUNC_FOR(memcpy) {
  RETURN_FUNC(memcpy_func, memcpy_vext);
}

typedef void* memmove_func(void*, const void*, size_t);
DEFINE_IFUNC_FOR(memmove) {
  RETURN_FUNC(memmove_func, memmove_vext);
}

typedef void* memset_func(void*, int, size_t);
DEFINE_IFUNC_FOR(memset) {
  RETURN_FUNC(memset_func, memset_vext);
}

typedef char* stpcpy_func(char*, const char*);
DEFINE_IFUNC_FOR(stpcpy) {
  RETURN_FUNC(stpcpy_func, stpcpy_vext);
}

typedef char* strcat_func(char*, const char*);
DEFINE_IFUNC_FOR(strcat) {
  RETURN_FUNC(strcat_func, strcat_vext);
}

typedef char* strchr_func(const char*, int);
DEFINE_IFUNC_FOR(strchr) {
  RETURN_FUNC(strchr_func, strchr_vext);
}

typedef int strcmp_func(const char*, const char*);
DEFINE_IFUNC_FOR(strcmp) {
  RETURN_FUNC(strcmp_func, strcmp_vext);
}

typedef char* strcpy_func(char*, const char*);
DEFINE_IFUNC_FOR(strcpy) {
  RETURN_FUNC(strcpy_func, strcpy_vext);
}

typedef size_t strlen_func(const char*);
DEFINE_IFUNC_FOR(strlen) {
  RETURN_FUNC(strlen_func, strlen_vext);
}

typedef char* strncat_func(char*, const char*, size_t);
DEFINE_IFUNC_FOR(strncat) {
  RETURN_FUNC(strncat_func, strncat_vext);
}

typedef int strncmp_func(const char*, const char*, size_t);
DEFINE_IFUNC_FOR(strncmp) {
  RETURN_FUNC(strncmp_func, strncmp_vext);
}

typedef char* strncpy_func(char*, const char*, size_t);
DEFINE_IFUNC_FOR(strncpy) {
  RETURN_FUNC(strncpy_func, strncpy_vext);
}

typedef size_t strnlen_func(const char*, size_t);
DEFINE_IFUNC_FOR(strnlen) {
  RETURN_FUNC(strnlen_func, strnlen_vext);
}

}  // extern "C"
#endif
