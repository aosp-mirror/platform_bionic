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

#ifndef _BIONIC_MBSTATE_H
#define _BIONIC_MBSTATE_H

#include <errno.h>
#include <wchar.h>

__BEGIN_DECLS

#define __MB_IS_ERR(rv)                              \
  (rv == BIONIC_MULTIBYTE_RESULT_ILLEGAL_SEQUENCE || \
   rv == BIONIC_MULTIBYTE_RESULT_INCOMPLETE_SEQUENCE)

static inline __wur bool mbstate_is_initial(const mbstate_t* ps) {
  return *(reinterpret_cast<const uint32_t*>(ps->__seq)) == 0;
}

static inline __wur size_t mbstate_bytes_so_far(const mbstate_t* ps) {
  return
      (ps->__seq[2] != 0) ? 3 :
      (ps->__seq[1] != 0) ? 2 :
      (ps->__seq[0] != 0) ? 1 : 0;
}

static inline void mbstate_set_byte(mbstate_t* ps, int i, char byte) {
  ps->__seq[i] = static_cast<uint8_t>(byte);
}

static inline __wur uint8_t mbstate_get_byte(const mbstate_t* ps, int n) {
  return ps->__seq[n];
}

static inline void mbstate_reset(mbstate_t* ps) {
  *(reinterpret_cast<uint32_t*>(ps->__seq)) = 0;
}

static inline __wur size_t mbstate_reset_and_return_illegal(int _errno, mbstate_t* ps) {
  errno = _errno;
  mbstate_reset(ps);
  return BIONIC_MULTIBYTE_RESULT_ILLEGAL_SEQUENCE;
}

static inline __wur size_t mbstate_reset_and_return(size_t _return, mbstate_t* ps) {
  mbstate_reset(ps);
  return _return;
}

__END_DECLS

#endif // _BIONIC_MBSTATE_H
