/*
 * Copyright (C) 2014 The Android Open Source Project
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

#include <stdio_ext.h>

#include <errno.h>
#include <stdlib.h>

#include <async_safe/log.h>

#include "local.h"

size_t __fbufsize(FILE* fp) {
  return fp->_bf._size;
}

int __freading(FILE* fp) {
  return (fp->_flags & __SRD) != 0;
}

int __fwriting(FILE* fp) {
  return (fp->_flags & __SWR) != 0;
}

int __freadable(FILE* fp) {
  return (fp->_flags & (__SRD|__SRW)) != 0;
}

int __fwritable(FILE* fp) {
  return (fp->_flags & (__SWR|__SRW)) != 0;
}

int __flbf(FILE* fp) {
  return (fp->_flags & __SLBF) != 0;
}

void __fpurge(FILE* fp) {
  fpurge(fp);
}

size_t __fpending(FILE* fp) {
  return fp->_p - fp->_bf._base;
}

void _flushlbf() {
  // If we flush all streams, we know we've flushed all the line-buffered streams.
  fflush(nullptr);
}

void __fseterr(FILE* fp) {
  fp->_flags |= __SERR;
}

int __fsetlocking(FILE* fp, int type) {
  int old_state = _EXT(fp)->_caller_handles_locking ? FSETLOCKING_BYCALLER : FSETLOCKING_INTERNAL;
  if (type == FSETLOCKING_QUERY) {
    return old_state;
  }

  if (type != FSETLOCKING_INTERNAL && type != FSETLOCKING_BYCALLER) {
    // The API doesn't let us report an error, so blow up.
    async_safe_fatal("Bad type (%d) passed to __fsetlocking", type);
  }

  _EXT(fp)->_caller_handles_locking = (type == FSETLOCKING_BYCALLER);
  return old_state;
}
