/*
 * Copyright (C) 2016 The Android Open Source Project
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

#include <sys/msg.h>

#include <sys/syscall.h>
#include <unistd.h>

int msgctl(int id, int cmd, msqid_ds* buf) {
#if !defined(__LP64__)
  // Annoyingly, the kernel requires this for 32-bit but rejects it for 64-bit.
  cmd |= IPC_64;
#endif
#if defined(SYS_msgctl)
  return syscall(SYS_msgctl, id, cmd, buf);
#else
  return syscall(SYS_ipc, MSGCTL, id, cmd, 0, buf, 0);
#endif
}

int msgget(key_t key, int flags) {
#if defined(SYS_msgget)
  return syscall(SYS_msgget, key, flags);
#else
  return syscall(SYS_ipc, MSGGET, key, flags, 0, 0, 0);
#endif
}

ssize_t msgrcv(int id, void* msg, size_t n, long type, int flags) {
#if defined(SYS_msgrcv)
  return syscall(SYS_msgrcv, id, msg, n, type, flags);
#else
  return syscall(SYS_ipc, IPCCALL(1, MSGRCV), id, n, flags, msg, type);
#endif
}

int msgsnd(int id, const void* msg, size_t n, int flags) {
#if defined(SYS_msgsnd)
  return syscall(SYS_msgsnd, id, msg, n, flags);
#else
  return syscall(SYS_ipc, MSGSND, id, n, flags, msg, 0);
#endif
}
