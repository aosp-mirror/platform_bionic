// Copyright (C) 2017 The Android Open Source Project
// SPDX-License-Identifier: BSD-2-Clause

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
