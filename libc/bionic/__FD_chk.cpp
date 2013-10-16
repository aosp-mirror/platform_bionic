/*
 * Copyright (C) 2013 The Android Open Source Project
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
#include <sys/select.h>
#include "private/libc_logging.h"

extern "C" int __FD_ISSET_chk(int fd, fd_set* set, size_t set_size) {
  if (__predict_false(fd < 0)) {
    __fortify_chk_fail("FD_ISSET: file descriptor < 0", 0);
  }
  if (__predict_false(fd >= FD_SETSIZE)) {
    __fortify_chk_fail("FD_ISSET: file descriptor >= FD_SETSIZE", 0);
  }
  if (__predict_false(set_size < sizeof(fd_set))) {
    __fortify_chk_fail("FD_ISSET: set is too small", 0);
  }
  return FD_ISSET(fd, set);
}

extern "C" void __FD_CLR_chk(int fd, fd_set* set, size_t set_size) {
  if (__predict_false(fd < 0)) {
    __fortify_chk_fail("FD_CLR: file descriptor < 0", 0);
  }
  if (__predict_false(fd >= FD_SETSIZE)) {
    __fortify_chk_fail("FD_CLR: file descriptor >= FD_SETSIZE", 0);
  }
  if (__predict_false(set_size < sizeof(fd_set))) {
    __fortify_chk_fail("FD_CLR: set is too small", 0);
  }
  FD_CLR(fd, set);
}

extern "C" void __FD_SET_chk(int fd, fd_set* set, size_t set_size) {
  if (__predict_false(fd < 0)) {
    __fortify_chk_fail("FD_SET: file descriptor < 0", 0);
  }
  if (__predict_false(fd >= FD_SETSIZE)) {
    __fortify_chk_fail("FD_SET: file descriptor >= FD_SETSIZE", 0);
  }
  if (__predict_false(set_size < sizeof(fd_set))) {
    __fortify_chk_fail("FD_SET: set is too small", 0);
  }
  FD_SET(fd, set);
}
