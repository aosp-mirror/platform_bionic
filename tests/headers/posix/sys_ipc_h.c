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
