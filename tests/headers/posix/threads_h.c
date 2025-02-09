// Copyright (C) 2019 The Android Open Source Project
// SPDX-License-Identifier: BSD-2-Clause

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
