/*
 * Copyright (C) 2018 The Android Open Source Project
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

#include <errno.h>
#include <fcntl.h>
#include <sys/random.h>
#include <unistd.h>

static int getentropy_urandom(void* buffer, size_t buffer_size, int saved_errno) {
  int fd = TEMP_FAILURE_RETRY(open("/dev/urandom", O_RDONLY | O_NOFOLLOW | O_CLOEXEC, 0));
  if (fd == -1) return -1;

  size_t collected = 0;
  while (collected < buffer_size) {
    ssize_t count = TEMP_FAILURE_RETRY(read(fd, static_cast<char*>(buffer) + collected,
                                            buffer_size - collected));
    if (count == -1) {
      close(fd);
      return -1;
    }
    collected += count;
  }

  close(fd);
  errno = saved_errno;
  return 0;
}

int getentropy(void* buffer, size_t buffer_size) {
  if (buffer_size > 256) {
    errno = EIO;
    return -1;
  }

  int saved_errno = errno;

  size_t collected = 0;
  while (collected < buffer_size) {
    long count = TEMP_FAILURE_RETRY(getrandom(static_cast<char*>(buffer) + collected,
                                              buffer_size - collected, GRND_NONBLOCK));
    if (count == -1) {
      // EAGAIN: there isn't enough entropy right now.
      // ENOSYS/EINVAL: getrandom(2) or GRND_NONBLOCK isn't supported.
      // EFAULT: `buffer` is invalid.
      // Try /dev/urandom regardless because it can't hurt,
      // and we don't need to optimize the EFAULT case.
      // See http://b/33059407 and http://b/67015565.
      return getentropy_urandom(buffer, buffer_size, saved_errno);
    }
    collected += count;
  }

  errno = saved_errno;
  return 0;
}
