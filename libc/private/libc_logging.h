/*
 * Copyright (C) 2010 The Android Open Source Project
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

#ifndef _LIBC_LOGGING_H
#define _LIBC_LOGGING_H

#include <sys/cdefs.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

__BEGIN_DECLS

enum {
  BIONIC_EVENT_MEMCPY_BUFFER_OVERFLOW = 80100,
  BIONIC_EVENT_STRCAT_BUFFER_OVERFLOW = 80105,
  BIONIC_EVENT_MEMMOVE_BUFFER_OVERFLOW = 80110,
  BIONIC_EVENT_STRNCAT_BUFFER_OVERFLOW = 80115,
  BIONIC_EVENT_STRNCPY_BUFFER_OVERFLOW = 80120,
  BIONIC_EVENT_MEMSET_BUFFER_OVERFLOW = 80125,
  BIONIC_EVENT_STRCPY_BUFFER_OVERFLOW = 80130,

  BIONIC_EVENT_STRCAT_INTEGER_OVERFLOW = 80200,
  BIONIC_EVENT_STRNCAT_INTEGER_OVERFLOW = 80205,

  BIONIC_EVENT_RESOLVER_OLD_RESPONSE = 80300,
  BIONIC_EVENT_RESOLVER_WRONG_SERVER = 80305,
  BIONIC_EVENT_RESOLVER_WRONG_QUERY = 80310,
};

enum {
  ANDROID_LOG_UNKNOWN = 0,
  ANDROID_LOG_DEFAULT,    /* only for SetMinPriority() */

  ANDROID_LOG_VERBOSE,
  ANDROID_LOG_DEBUG,
  ANDROID_LOG_INFO,
  ANDROID_LOG_WARN,
  ANDROID_LOG_ERROR,
  ANDROID_LOG_FATAL,

  ANDROID_LOG_SILENT,     /* only for SetMinPriority(); must be last */
};

struct abort_msg_t {
  size_t size;
  char msg[0];
};

__LIBC_HIDDEN__ void __libc_set_abort_message(const char* msg);

//
// Formats a message to the log (priority 'fatal'), then aborts.
//

__LIBC_HIDDEN__ __noreturn void __libc_fatal(const char* format, ...)
    __attribute__((__format__(printf, 1, 2)));

//
// Formatting routines for the C library's internal debugging.
// Unlike the usual alternatives, these don't allocate.
//

__LIBC_HIDDEN__ int __libc_format_buffer(char* buffer, size_t buffer_size, const char* format, ...)
    __attribute__((__format__(printf, 3, 4)));

__LIBC_HIDDEN__ int __libc_format_fd(int fd, const char* format, ...)
    __attribute__((__format__(printf, 2, 3)));

__LIBC_HIDDEN__ int __libc_format_log(int priority, const char* tag, const char* format, ...)
    __attribute__((__format__(printf, 3, 4)));

__LIBC_HIDDEN__ int __libc_format_log_va_list(int priority, const char* tag, const char* format,
                                              va_list ap);

//
// Event logging.
//

__LIBC_HIDDEN__ void __libc_android_log_event_int(int32_t tag, int value);
__LIBC_HIDDEN__ void __libc_android_log_event_uid(int32_t tag);

__LIBC_HIDDEN__ __noreturn void __fortify_chk_fail(const char* msg, uint32_t event_tag);

__END_DECLS

#endif
