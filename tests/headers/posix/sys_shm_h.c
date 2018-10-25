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

#include <sys/shm.h>

#include "header_checks.h"

static void sys_shm_h() {
  MACRO(SHM_RDONLY);
  MACRO(SHM_RND);
  MACRO(SHMLBA);

  TYPE(shmatt_t);

  TYPE(struct shmid_ds);
  STRUCT_MEMBER(struct shmid_ds, struct ipc_perm, shm_perm);
  STRUCT_MEMBER(struct shmid_ds, size_t, shm_segsz);
  STRUCT_MEMBER(struct shmid_ds, pid_t, shm_lpid);
  STRUCT_MEMBER(struct shmid_ds, pid_t, shm_cpid);
  STRUCT_MEMBER(struct shmid_ds, shmatt_t, shm_nattch);
#if defined(__LP64__)
  STRUCT_MEMBER(struct shmid_ds, time_t, shm_atime);
  STRUCT_MEMBER(struct shmid_ds, time_t, shm_dtime);
  STRUCT_MEMBER(struct shmid_ds, time_t, shm_ctime);
#else
  // Starting at kernel v4.19, 32 bit changed these to unsigned values.
  STRUCT_MEMBER(struct shmid_ds, unsigned long, shm_atime);
  STRUCT_MEMBER(struct shmid_ds, unsigned long, shm_dtime);
  STRUCT_MEMBER(struct shmid_ds, unsigned long, shm_ctime);
#endif

  TYPE(pid_t);
  TYPE(size_t);
  TYPE(time_t);

  FUNCTION(shmat, void* (*f)(int, const void*, int));
  FUNCTION(shmctl, int (*f)(int, int, struct shmid_ds*));
  FUNCTION(shmdt, int (*f)(const void*));
  FUNCTION(shmget, int (*f)(key_t, size_t, int));
}
#endif
