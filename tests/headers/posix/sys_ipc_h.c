// Copyright (C) 2017 The Android Open Source Project
// SPDX-License-Identifier: BSD-2-Clause

#include <sys/ipc.h>

#include "header_checks.h"

static void sys_ipc_h() {
  TYPE(struct ipc_perm);
  STRUCT_MEMBER(struct ipc_perm, uid_t, uid);
  STRUCT_MEMBER(struct ipc_perm, gid_t, gid);
  STRUCT_MEMBER(struct ipc_perm, uid_t, cuid);
  STRUCT_MEMBER(struct ipc_perm, gid_t, cgid);
#if defined(__GLIBC__)
  STRUCT_MEMBER(struct ipc_perm, unsigned short, mode);
#else
  STRUCT_MEMBER(struct ipc_perm, mode_t, mode);
#endif

  TYPE(uid_t);
  TYPE(gid_t);
  TYPE(mode_t);
  TYPE(key_t);

  MACRO(IPC_CREAT);
  MACRO(IPC_EXCL);
  MACRO(IPC_NOWAIT);

  MACRO(IPC_PRIVATE);

  MACRO(IPC_RMID);
  MACRO(IPC_SET);
  MACRO(IPC_STAT);

  FUNCTION(ftok, key_t (*f)(const char*, int));
}
