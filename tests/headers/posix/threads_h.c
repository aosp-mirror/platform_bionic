/*
 * Copyright (C) 2019 The Android Open Source Project
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

#if __has_include(<threads.h>)

#include <threads.h>

#include "header_checks.h"

thread_local int t;

static void threads_h() {
  MACRO(ONCE_FLAG_INIT);
  MACRO(TSS_DTOR_ITERATIONS);

  TYPE(cnd_t);
  TYPE(thrd_t);
  TYPE(tss_t);
  TYPE(mtx_t);

  TYPE(tss_dtor_t);
  TYPE(thrd_start_t);

  TYPE(once_flag);

  int enumeration_constants = mtx_plain | mtx_recursive | mtx_timed |
      thrd_timedout | thrd_success | thrd_busy | thrd_error | thrd_nomem;

  FUNCTION(call_once, void (*f)(once_flag*, void (*)(void)));

  FUNCTION(cnd_broadcast, int (*f)(cnd_t*));
  FUNCTION(cnd_destroy, void (*f)(cnd_t*));
  FUNCTION(cnd_init, int (*f)(cnd_t*));
  FUNCTION(cnd_signal, int (*f)(cnd_t*));
  FUNCTION(cnd_timedwait, int (*f)(cnd_t*, mtx_t*, const struct timespec*));
  FUNCTION(cnd_wait, int (*f)(cnd_t*, mtx_t*));

  FUNCTION(mtx_destroy, void (*f)(mtx_t*));
  FUNCTION(mtx_init, int (*f)(mtx_t*, int));
  FUNCTION(mtx_lock, int (*f)(mtx_t*));
  FUNCTION(mtx_timedlock, int (*f)(mtx_t*, const struct timespec*));
  FUNCTION(mtx_trylock, int (*f)(mtx_t*));
  FUNCTION(mtx_unlock, int (*f)(mtx_t*));

  FUNCTION(thrd_create, int (*f)(thrd_t*, thrd_start_t, void*));
  FUNCTION(thrd_current, thrd_t (*f)(void));
  FUNCTION(thrd_detach, int (*f)(thrd_t));
  FUNCTION(thrd_equal, int (*f)(thrd_t, thrd_t));
  FUNCTION(thrd_exit, void (*f)(int));
  FUNCTION(thrd_join, int (*f)(thrd_t, int*));
  FUNCTION(thrd_sleep, int (*f)(const struct timespec*, struct timespec*));
  FUNCTION(thrd_yield, void (*f)(void));

  FUNCTION(tss_create, int (*f)(tss_t*, tss_dtor_t));
  FUNCTION(tss_delete, void (*f)(tss_t));
  FUNCTION(tss_get, void* (*f)(tss_t));
  FUNCTION(tss_set, int (*f)(tss_t, void*));
}

#define DO_NOT_INCLUDE_TIME_H
#include "time_h.c"

#endif
