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

#include <libgen.h>

#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/cdefs.h>
#include <sys/param.h>

#include "bionic/pthread_internal.h"

static int __basename_r(const char* path, char* buffer, size_t buffer_size) {
  const char* startp = nullptr;
  const char* endp = nullptr;
  int len;
  int result;

  // Empty or NULL string gets treated as ".".
  if (path == nullptr || *path == '\0') {
    startp = ".";
    len = 1;
    goto Exit;
  }

  // Strip trailing slashes.
  endp = path + strlen(path) - 1;
  while (endp > path && *endp == '/') {
    endp--;
  }

  // All slashes becomes "/".
  if (endp == path && *endp == '/') {
    startp = "/";
    len = 1;
    goto Exit;
  }

  // Find the start of the base.
  startp = endp;
  while (startp > path && *(startp - 1) != '/') {
    startp--;
  }

  len = endp - startp +1;

 Exit:
  result = len;
  if (buffer == nullptr) {
    return result;
  }
  if (len > static_cast<int>(buffer_size) - 1) {
    len = buffer_size - 1;
    result = -1;
    errno = ERANGE;
  }

  if (len >= 0) {
    memcpy(buffer, startp, len);
    buffer[len] = 0;
  }
  return result;
}

// Since this is a non-standard symbol, it might be hijacked by a basename_r in the executable.
__LIBC32_LEGACY_PUBLIC__ int basename_r(const char* path, char* buffer, size_t buffer_size) {
  return __basename_r(path, buffer, buffer_size);
}

static int __dirname_r(const char* path, char* buffer, size_t buffer_size) {
  const char* endp = nullptr;
  int len;
  int result;

  // Empty or NULL string gets treated as ".".
  if (path == nullptr || *path == '\0') {
    path = ".";
    len = 1;
    goto Exit;
  }

  // Strip trailing slashes.
  endp = path + strlen(path) - 1;
  while (endp > path && *endp == '/') {
    endp--;
  }

  // Find the start of the dir.
  while (endp > path && *endp != '/') {
    endp--;
  }

  // Either the dir is "/" or there are no slashes.
  if (endp == path) {
    path = (*endp == '/') ? "/" : ".";
    len = 1;
    goto Exit;
  }

  do {
    endp--;
  } while (endp > path && *endp == '/');

  len = endp - path + 1;

 Exit:
  result = len;
  if (len + 1 > MAXPATHLEN) {
    errno = ENAMETOOLONG;
    return -1;
  }
  if (buffer == nullptr) {
    return result;
  }

  if (len > static_cast<int>(buffer_size) - 1) {
    len = buffer_size - 1;
    result = -1;
    errno = ERANGE;
  }

  if (len >= 0) {
    memcpy(buffer, path, len);
    buffer[len] = 0;
  }
  return result;
}

// Since this is a non-standard symbol, it might be hijacked by a basename_r in the executable.
__LIBC32_LEGACY_PUBLIC__ int dirname_r(const char* path, char* buffer, size_t buffer_size) {
  return __dirname_r(path, buffer, buffer_size);
}

char* basename(const char* path) {
  char* buf = __get_bionic_tls().basename_buf;
  int rc = __basename_r(path, buf, sizeof(__get_bionic_tls().basename_buf));
  return (rc < 0) ? nullptr : buf;
}

char* dirname(const char* path) {
  char* buf = __get_bionic_tls().dirname_buf;
  int rc = __dirname_r(path, buf, sizeof(__get_bionic_tls().dirname_buf));
  return (rc < 0) ? nullptr : buf;
}
