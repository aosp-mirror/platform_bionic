/*
 * Copyright (C) 2008 The Android Open Source Project
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

#include <pthread.h>

#include <fcntl.h>
#include <stdio.h> // For snprintf.
#include <string.h>
#include <sys/prctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "private/ErrnoRestorer.h"
#include "pthread_internal.h"

// This value is not exported by kernel headers.
#define MAX_TASK_COMM_LEN 16

static int __open_task_comm_fd(pthread_t t, int flags) {
  char comm_name[64];
  snprintf(comm_name, sizeof(comm_name), "/proc/self/task/%d/comm", pthread_gettid_np(t));
  return open(comm_name, O_CLOEXEC | flags);
}

int pthread_getname_np(pthread_t t, char* buf, size_t buf_size) {
  ErrnoRestorer errno_restorer;

  if (buf_size < MAX_TASK_COMM_LEN) return ERANGE;

  // Getting our own name is an easy special case.
  if (t == pthread_self()) {
    return prctl(PR_GET_NAME, buf) ? errno : 0;
  }

  // We have to get another thread's name.
  int fd = __open_task_comm_fd(t, O_RDONLY);
  if (fd == -1) return errno;

  ssize_t n = TEMP_FAILURE_RETRY(read(fd, buf, buf_size));
  close(fd);

  if (n == -1) return errno;

  // The kernel adds a trailing '\n' to the /proc file,
  // so this is actually the normal case for short names.
  if (n > 0 && buf[n - 1] == '\n') {
    buf[n - 1] = '\0';
    return 0;
  }

  if (n == static_cast<ssize_t>(buf_size)) return ERANGE;
  buf[n] = '\0';
  return 0;
}

int pthread_setname_np(pthread_t t, const char* thread_name) {
  ErrnoRestorer errno_restorer;

  size_t thread_name_len = strlen(thread_name);
  if (thread_name_len >= MAX_TASK_COMM_LEN) return ERANGE;

  // Setting our own name is an easy special case.
  if (t == pthread_self()) {
    return prctl(PR_SET_NAME, thread_name) ? errno : 0;
  }

  // We have to set another thread's name.
  int fd = __open_task_comm_fd(t, O_WRONLY);
  if (fd == -1) return errno;

  ssize_t n = TEMP_FAILURE_RETRY(write(fd, thread_name, thread_name_len));
  close(fd);

  if (n == -1) return errno;
  if (n != static_cast<ssize_t>(thread_name_len)) return EIO;
  return 0;
}
