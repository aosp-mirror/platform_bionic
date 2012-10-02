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

#include <string.h>
#include <stdlib.h>
#include <private/logd.h>
#include <safe_iop.h>

/*
 * Runtime implementation of __builtin____strncat_chk.
 *
 * See
 *   http://gcc.gnu.org/onlinedocs/gcc/Object-Size-Checking.html
 *   http://gcc.gnu.org/ml/gcc-patches/2004-09/msg02055.html
 * for details.
 *
 * This strncat check is called if _FORTIFY_SOURCE is defined and
 * greater than 0.
 */
char *__strncat_chk (char *dest, const char *src,
              size_t len, size_t dest_buf_size)
{
    // TODO: optimize so we don't scan src/dest twice.
    size_t dest_len = strlen(dest);
    size_t src_len = strlen(src);
    if (src_len > len) {
        src_len = len;
    }

    size_t sum;
    // sum = src_len + dest_len + 1 (with overflow protection)
    if (!safe_add3(&sum, src_len, dest_len, 1U)) {
        __libc_android_log_print(ANDROID_LOG_FATAL, "libc",
            "*** strncat integer overflow detected ***\n");
        __libc_android_log_event_uid(BIONIC_EVENT_STRNCAT_INTEGER_OVERFLOW);
        abort();
    }

    if (sum > dest_buf_size) {
        __libc_android_log_print(ANDROID_LOG_FATAL, "libc",
            "*** strncat buffer overflow detected ***\n");
        __libc_android_log_event_uid(BIONIC_EVENT_STRNCAT_BUFFER_OVERFLOW);
        abort();
    }

    return strncat(dest, src, len);
}
