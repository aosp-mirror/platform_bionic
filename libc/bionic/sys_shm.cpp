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

#include <sys/shm.h>

#include <sys/syscall.h>
#include <unistd.h>

void* shmat(int id, const void* address, int flags) {
#if defined(SYS_shmat)
  return reinterpret_cast<void*>(syscall(SYS_shmat, id, address, flags));
#else
  // See the kernel's ipc/syscall.c for the other side of this dance.
  void* result = nullptr;
  if (syscall(SYS_ipc, SHMAT, id, flags, &result, address, 0) == -1) {
    return reinterpret_cast<void*>(-1);
  }
  return result;
#endif
}

int shmctl(int id, int cmd, struct shmid_ds* buf) {
#if !defined(__LP64__)
  // Annoyingly, the kernel requires this for 32-bit but rejects it for 64-bit.
  cmd |= IPC_64;
#endif
#if defined(SYS_shmctl)
  return syscall(SYS_shmctl, id, cmd, buf);
#else
  return syscall(SYS_ipc, SHMCTL, id, cmd, 0, buf, 0);
#endif
}

int shmdt(const void* address) {
#if defined(SYS_shmdt)
  return syscall(SYS_shmdt, address);
#else
  return syscall(SYS_ipc, SHMDT, 0, 0, 0, address, 0);
#endif
}

int shmget(key_t key, size_t size, int flags) {
#if defined(SYS_shmget)
  return syscall(SYS_shmget, key, size, flags);
#else
  return syscall(SYS_ipc, SHMGET, key, size, flags, 0, 0);
#endif
}
