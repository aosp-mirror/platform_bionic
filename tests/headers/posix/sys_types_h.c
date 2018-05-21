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

#include <sys/types.h>

#include "header_checks.h"

static void sys_types_h() {
  TYPE(blkcnt_t);
  TYPE(blksize_t);
  TYPE(clock_t);
  TYPE(clockid_t);
  TYPE(dev_t);
  TYPE(fsblkcnt_t);
  TYPE(fsfilcnt_t);
  TYPE(gid_t);
  TYPE(id_t);
  TYPE(ino_t);
  TYPE(key_t);
  TYPE(mode_t);
  TYPE(nlink_t);
  TYPE(off_t);
  TYPE(pid_t);
  TYPE(pthread_attr_t);
  TYPE(pthread_barrier_t);
  TYPE(pthread_barrierattr_t);
  TYPE(pthread_cond_t);
  TYPE(pthread_condattr_t);
  TYPE(pthread_key_t);
  TYPE(pthread_mutex_t);
  TYPE(pthread_mutexattr_t);
  TYPE(pthread_once_t);
  TYPE(pthread_rwlock_t);
  TYPE(pthread_rwlockattr_t);
  TYPE(pthread_spinlock_t);
  TYPE(pthread_t);
  TYPE(size_t);
  TYPE(ssize_t);
  TYPE(suseconds_t);
  TYPE(time_t);
  TYPE(timer_t);
  TYPE(uid_t);
}
