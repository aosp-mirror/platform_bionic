// Copyright (C) 2017 The Android Open Source Project
// SPDX-License-Identifier: BSD-2-Clause

#if defined(__BIONIC__)

#include <sys/msg.h>

#include "header_checks.h"

static void sys_msg_h() {
  TYPE(msgqnum_t);
  TYPE(msglen_t);

  MACRO(MSG_NOERROR);

  TYPE(struct msqid_ds);
  STRUCT_MEMBER(struct msqid_ds, struct ipc_perm, msg_perm);
  STRUCT_MEMBER(struct msqid_ds, msgqnum_t, msg_qnum);
  STRUCT_MEMBER(struct msqid_ds, msglen_t, msg_qbytes);
  STRUCT_MEMBER(struct msqid_ds, pid_t, msg_lspid);
  STRUCT_MEMBER(struct msqid_ds, pid_t, msg_lrpid);
#if defined(__LP64__)
  STRUCT_MEMBER(struct msqid_ds, time_t, msg_stime);
  STRUCT_MEMBER(struct msqid_ds, time_t, msg_rtime);
  STRUCT_MEMBER(struct msqid_ds, time_t, msg_ctime);
#else
  // Starting at kernel v4.19, 32 bit changed these to unsigned values.
  STRUCT_MEMBER(struct msqid_ds, unsigned long, msg_stime);
  STRUCT_MEMBER(struct msqid_ds, unsigned long, msg_rtime);
  STRUCT_MEMBER(struct msqid_ds, unsigned long, msg_ctime);
#endif

  TYPE(pid_t);
  TYPE(size_t);
  TYPE(ssize_t);
  TYPE(time_t);

  FUNCTION(msgctl, int (*f)(int, int, struct msqid_ds*));
  FUNCTION(msgget, int (*f)(key_t, int));
  FUNCTION(msgrcv, ssize_t (*f)(int, void*, size_t, long, int));
  FUNCTION(msgsnd, int (*f)(int, const void*, size_t, int));
}
#endif
