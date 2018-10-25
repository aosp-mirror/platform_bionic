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

#if defined(__BIONIC__)

#include <sys/sem.h>

#include "header_checks.h"

static void sys_sem_h() {
  MACRO(SEM_UNDO);

  MACRO(GETNCNT);
  MACRO(GETPID);
  MACRO(GETVAL);
  MACRO(GETALL);
  MACRO(GETZCNT);
  MACRO(SETVAL);
  MACRO(SETALL);

  TYPE(struct semid_ds);
  STRUCT_MEMBER(struct semid_ds, struct ipc_perm, sem_perm);
#if defined(__linux__) // POSIX says short, Linux says long.
  STRUCT_MEMBER(struct semid_ds, unsigned long, sem_nsems);
#else
  STRUCT_MEMBER(struct semid_ds, unsigned short, sem_nsems);
#endif
#if defined(__LP64__)
  STRUCT_MEMBER(struct semid_ds, time_t, sem_otime);
  STRUCT_MEMBER(struct semid_ds, time_t, sem_ctime);
#else
  // Starting at kernel v4.19, 32 bit changed these to unsigned values.
  STRUCT_MEMBER(struct semid_ds, unsigned long, sem_otime);
  STRUCT_MEMBER(struct semid_ds, unsigned long, sem_ctime);
#endif

  TYPE(pid_t);
  TYPE(size_t);
  TYPE(time_t);

  TYPE(struct sembuf);
  STRUCT_MEMBER(struct sembuf, unsigned short, sem_num);
  STRUCT_MEMBER(struct sembuf, short, sem_op);
  STRUCT_MEMBER(struct sembuf, short, sem_flg);

  FUNCTION(semctl, int (*f)(int, int, int, ...));
  FUNCTION(semget, int (*f)(key_t, int, int));
  FUNCTION(semop, int (*f)(int, struct sembuf*, size_t));
}

#endif
