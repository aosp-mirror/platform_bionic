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

#include <fcntl.h>
#include <private/bionic_ifuncs.h>
#include <stddef.h>
#include <sys/syscall.h>
#include <unistd.h>

extern "C" {

static inline __always_inline int ifunc_faccessat(int dir_fd, const char* path, int mode) {
  register long a0 __asm__("a0") = dir_fd;
  register long a1 __asm__("a1") = reinterpret_cast<long>(path);
  register long a2 __asm__("a2") = mode;
  register long a7 __asm__("a7") = __NR_faccessat;
  __asm__("ecall" : "=r"(a0) : "r"(a0), "r"(a1), "r"(a2), "r"(a7) : "memory");
  return a0;
}

static bool have_fast_v() {
  static bool result = []() {
    // We don't want to do a full "bogomips" test, so just check for the
    // presence of a file that would indicate that we're running in qemu.
    return ifunc_faccessat(AT_FDCWD, "/dev/hvc0", F_OK) != 0;
  }();
  return result;
}

DEFINE_IFUNC_FOR(memchr) {
  if (have_fast_v()) RETURN_FUNC(memchr_func_t, memchr_v);
  RETURN_FUNC(memchr_func_t, memchr_gc);
}
MEMCHR_SHIM()

DEFINE_IFUNC_FOR(memcmp) {
  if (have_fast_v()) RETURN_FUNC(memcmp_func_t, memcmp_v);
  RETURN_FUNC(memcmp_func_t, memcmp_gc);
}
MEMCMP_SHIM()

DEFINE_IFUNC_FOR(memcpy) {
  if (have_fast_v()) RETURN_FUNC(memcpy_func_t, memcpy_v);
  RETURN_FUNC(memcpy_func_t, memcpy_gc);
}
MEMCPY_SHIM()

DEFINE_IFUNC_FOR(memmove) {
  if (have_fast_v()) RETURN_FUNC(memmove_func_t, memmove_v);
  RETURN_FUNC(memmove_func_t, memmove_gc);
}
MEMMOVE_SHIM()

DEFINE_IFUNC_FOR(memset) {
  if (have_fast_v()) RETURN_FUNC(memset_func_t, memset_v);
  RETURN_FUNC(memset_func_t, memset_gc);
}
MEMSET_SHIM()

DEFINE_IFUNC_FOR(stpcpy) {
  if (have_fast_v()) RETURN_FUNC(stpcpy_func_t, stpcpy_v);
  RETURN_FUNC(stpcpy_func_t, stpcpy_gc);
}
STPCPY_SHIM()

DEFINE_IFUNC_FOR(strcat) {
  if (have_fast_v()) RETURN_FUNC(strcat_func_t, strcat_v);
  RETURN_FUNC(strcat_func_t, strcat_gc);
}
STRCAT_SHIM()

DEFINE_IFUNC_FOR(strchr) {
  if (have_fast_v()) RETURN_FUNC(strchr_func_t, strchr_v);
  RETURN_FUNC(strchr_func_t, strchr_gc);
}
STRCHR_SHIM()

DEFINE_IFUNC_FOR(strcmp) {
  if (have_fast_v()) RETURN_FUNC(strcmp_func_t, strcmp_v);
  RETURN_FUNC(strcmp_func_t, strcmp_gc);
}
STRCMP_SHIM()

DEFINE_IFUNC_FOR(strcpy) {
  if (have_fast_v()) RETURN_FUNC(strcpy_func_t, strcpy_v);
  RETURN_FUNC(strcpy_func_t, strcpy_gc);
}
STRCPY_SHIM()

DEFINE_IFUNC_FOR(strlen) {
  if (have_fast_v()) RETURN_FUNC(strlen_func_t, strlen_v);
  RETURN_FUNC(strlen_func_t, strlen_gc);
}
STRLEN_SHIM()

DEFINE_IFUNC_FOR(strncat) {
  if (have_fast_v()) RETURN_FUNC(strncat_func_t, strncat_v);
  RETURN_FUNC(strncat_func_t, strncat_gc);
}
STRNCAT_SHIM()

DEFINE_IFUNC_FOR(strncmp) {
  if (have_fast_v()) RETURN_FUNC(strncmp_func_t, strncmp_v);
  RETURN_FUNC(strncmp_func_t, strncmp_gc);
}
STRNCMP_SHIM()

DEFINE_IFUNC_FOR(strncpy) {
  if (have_fast_v()) RETURN_FUNC(strncpy_func_t, strncpy_v);
  RETURN_FUNC(strncpy_func_t, strncpy_gc);
}
STRNCPY_SHIM()

DEFINE_IFUNC_FOR(strnlen) {
  if (have_fast_v()) RETURN_FUNC(strnlen_func_t, strnlen_v);
  RETURN_FUNC(strnlen_func_t, strnlen_gc);
}
STRNLEN_SHIM()

}  // extern "C"
