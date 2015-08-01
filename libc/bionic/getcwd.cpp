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

#undef _FORTIFY_SOURCE
#include <errno.h>
#include <malloc.h>
#include <string.h>
#include <unistd.h>

extern "C" int __getcwd(char* buf, size_t size);

char* getcwd(char* buf, size_t size) {
  // You can't specify size 0 unless you're asking us to allocate for you.
  if (buf != NULL && size == 0) {
    errno = EINVAL;
    return NULL;
  }

  // Allocate a buffer if necessary.
  char* allocated_buf = NULL;
  size_t allocated_size = size;
  if (buf == NULL) {
    if (size == 0) {
      // The Linux kernel won't return more than a page, so translate size 0 to 4KiB.
      // TODO: if we need to support paths longer than that, we'll have to walk the tree ourselves.
      allocated_size = getpagesize();
    }
    buf = allocated_buf = static_cast<char*>(malloc(allocated_size));
    if (buf == NULL) {
      // malloc should set errno, but valgrind's malloc wrapper doesn't.
      errno = ENOMEM;
      return NULL;
    }
  }

  // Ask the kernel to fill our buffer.
  int rc = __getcwd(buf, allocated_size);
  if (rc == -1) {
    free(allocated_buf);
    // __getcwd set errno.
    return NULL;
  }

  // If we allocated a whole page, only return as large an allocation as necessary.
  if (allocated_buf != NULL) {
    if (size == 0) {
      buf = strdup(allocated_buf);
      free(allocated_buf);
    } else {
      buf = allocated_buf;
    }
  }

  return buf;
}
