/*
 * Copyright (C) 2012 The Android Open Source Project
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

#undef _FORTIFY_SOURCE

#include <string.h>
#include <stdlib.h>
#include "private/libc_logging.h"

/*
 * Runtime implementation of __builtin____strncpy_chk.
 *
 * See
 *   http://gcc.gnu.org/onlinedocs/gcc/Object-Size-Checking.html
 *   http://gcc.gnu.org/ml/gcc-patches/2004-09/msg02055.html
 * for details.
 *
 * This strncpy check is called if _FORTIFY_SOURCE is defined and
 * greater than 0.
 */
extern "C" char* __strncpy_chk(char* __restrict dest, const char* __restrict src,
                               size_t len, size_t dest_len) {
  if (__predict_false(len > dest_len)) {
    __fortify_chk_fail("strncpy: prevented write past end of buffer",
                       BIONIC_EVENT_STRNCPY_BUFFER_OVERFLOW);
  }

  return strncpy(dest, src, len);
}

/*
 * __strncpy_chk2
 *
 * This is a variant of __strncpy_chk, but it also checks to make
 * sure we don't read beyond the end of "src". The code for this is
 * based on the original version of strncpy, but modified to check
 * how much we read from "src" at the end of the copy operation.
 */
extern "C" char* __strncpy_chk2(char* __restrict dst, const char* __restrict src,
              size_t n, size_t dest_len, size_t src_len)
{
  if (__predict_false(n > dest_len)) {
    __fortify_chk_fail("strncpy: prevented write past end of buffer",
                       BIONIC_EVENT_STRNCPY_BUFFER_OVERFLOW);
  }
  if (n != 0) {
    char* d = dst;
    const char* s = src;

    do {
      if ((*d++ = *s++) == 0) {
        /* NUL pad the remaining n-1 bytes */
        while (--n != 0) {
          *d++ = 0;
        }
        break;
      }
    } while (--n != 0);

    size_t s_copy_len = static_cast<size_t>(s - src);
    if (__predict_false(s_copy_len > src_len)) {
      __fortify_chk_fail("strncpy: prevented read past end of buffer", 0);
    }
  }

  return dst;
}
