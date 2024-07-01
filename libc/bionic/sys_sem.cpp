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

#include <sys/sem.h>

#include <stdarg.h>
#include <sys/syscall.h>
#include <unistd.h>

int semctl(int id, int num, int cmd, ...) {
#if !defined(__LP64__)
  // Annoyingly, the kernel requires this for 32-bit but rejects it for 64-bit.
  cmd |= IPC_64;
#endif
  va_list ap;
  va_start(ap, cmd);
  semun arg = va_arg(ap, semun);
  va_end(ap);
  return syscall(SYS_semctl, id, num, cmd, arg);
}

int semget(key_t key, int n, int flags) {
  return syscall(SYS_semget, key, n, flags);
}

int semop(int id, sembuf* ops, size_t op_count) {
  return semtimedop(id, ops, op_count, nullptr);
}

int semtimedop(int id, sembuf* ops, size_t op_count, const timespec* ts) {
#if defined(SYS_semtimedop)
  return syscall(SYS_semtimedop, id, ops, op_count, ts);
#else
  // 32-bit x86 -- the only architecture without semtimedop(2) -- only has
  // semtimedop_time64(2), but since we don't have any timespec64 stuff,
  // it's less painful for us to just stick with the legacy ipc(2) here.
  return syscall(SYS_ipc, SEMTIMEDOP, id, op_count, 0, ops, ts);
#endif
}
