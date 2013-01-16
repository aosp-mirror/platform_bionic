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

#ifndef _PRIVATE_SSP_H
#define _PRIVATE_SSP_H

#include <errno.h>
#include <sys/cdefs.h>

__BEGIN_DECLS

/** WARNING WARNING WARNING
 **
 ** This header file is *NOT* part of the public Bionic ABI/API
 ** and should not be used/included by user-serviceable parts of
 ** the system (e.g. applications).
 **/

/* GCC uses this on ARM and MIPS. */
extern void* __stack_chk_guard;

/* GCC calls this if a stack guard check fails. */
extern void __stack_chk_fail();

__inline__ static void* __attribute__((always_inline)) __generate_stack_chk_guard(void) {
  union {
    uintptr_t value;
    char bytes[sizeof(uintptr_t)];
  } u;

  /* Try pulling random bytes from /dev/urandom. */
  int fd = TEMP_FAILURE_RETRY(open("/dev/urandom", O_RDONLY));
  if (fd != -1) {
    ssize_t byte_count = TEMP_FAILURE_RETRY(read(fd, &u.bytes, sizeof(u)));
    close(fd);
    if (byte_count == sizeof(u)) {
      return (void*) u.value;
    }
  }

  /* If that failed, switch to 'terminator canary'. */
  u.bytes[0] = 0;
  u.bytes[1] = 0;
  u.bytes[2] = '\n';
  u.bytes[3] = 255;
  return (void*) u.value;
}

__END_DECLS

#endif
