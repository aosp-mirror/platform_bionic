// Copyright (C) 2017 The Android Open Source Project
// SPDX-License-Identifier: BSD-2-Clause

#include <semaphore.h>

#include "header_checks.h"

static void semaphore_h() {
  TYPE(sem_t);

  MACRO(SEM_FAILED);

#if !defined(__GLIBC__)  // Our glibc is too old.
  FUNCTION(sem_clockwait, int (*f)(sem_t*, clockid_t, const struct timespec*));
#endif
  FUNCTION(sem_close, int (*f)(sem_t*));
  FUNCTION(sem_destroy, int (*f)(sem_t*));
  FUNCTION(sem_getvalue, int (*f)(sem_t*, int*));
  FUNCTION(sem_init, int (*f)(sem_t*, int, unsigned));
  FUNCTION(sem_open, sem_t* (*f)(const char*, int, ...));
  FUNCTION(sem_post, int (*f)(sem_t*));
  FUNCTION(sem_timedwait, int (*f)(sem_t*, const struct timespec*));
  FUNCTION(sem_trywait, int (*f)(sem_t*));
  FUNCTION(sem_unlink, int (*f)(const char*));
  FUNCTION(sem_wait, int (*f)(sem_t*));
}
