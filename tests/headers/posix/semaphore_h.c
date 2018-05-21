/*
 * Copyright (C) 2017 The Android Open Source Project
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

#include <semaphore.h>

#include "header_checks.h"

static void semaphore_h() {
  TYPE(sem_t);

  MACRO(SEM_FAILED);

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
