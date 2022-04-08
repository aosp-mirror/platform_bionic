/*
 * Copyright (C) 2019 The Android Open Source Project
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

#include <string.h>
#include <sys/socket.h>

#include <async_safe/log.h>

#include "private/bionic_fdtrack.h"

extern "C" ssize_t __recvmsg(int __fd, struct msghdr* __msg, int __flags);
extern "C" int __recvmmsg(int __fd, struct mmsghdr* __msgs, unsigned int __msg_count, int __flags,
                          const struct timespec* __timeout);

static inline __attribute__((artificial)) __attribute__((always_inline)) void track_fds(
    struct msghdr* msg, const char* function_name) {
  if (!__android_fdtrack_hook) {
    return;
  }

  for (struct cmsghdr* cmsg = CMSG_FIRSTHDR(msg); cmsg; cmsg = CMSG_NXTHDR(msg, cmsg)) {
    if (cmsg->cmsg_type != SCM_RIGHTS) {
      continue;
    }

    if (cmsg->cmsg_len <= sizeof(struct cmsghdr)) {
      continue;
    }

    size_t data_length = cmsg->cmsg_len - sizeof(struct cmsghdr);
    if (data_length % sizeof(int) != 0) {
      async_safe_fatal("invalid cmsg length: %zu", data_length);
    }

    for (size_t offset = 0; offset < data_length; offset += sizeof(int)) {
      int fd;
      memcpy(&fd, CMSG_DATA(cmsg) + offset, sizeof(int));
      FDTRACK_CREATE_NAME(function_name, fd);
    }
  }
}

ssize_t recvmsg(int __fd, struct msghdr* __msg, int __flags) {
  ssize_t rc = __recvmsg(__fd, __msg, __flags);
  if (rc == -1) {
    return -1;
  }
  track_fds(__msg, "recvmsg");
  return rc;
}

int recvmmsg(int __fd, struct mmsghdr* __msgs, unsigned int __msg_count, int __flags,
             const struct timespec* __timeout) {
  int rc = __recvmmsg(__fd, __msgs, __msg_count, __flags, __timeout);
  if (rc == -1) {
    return -1;
  }
  for (int i = 0; i < rc; ++i) {
    track_fds(&__msgs[i].msg_hdr, "recvmmsg");
  }
  return rc;
}
