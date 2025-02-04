// Copyright (C) 2017 The Android Open Source Project
// SPDX-License-Identifier: BSD-2-Clause

#include <poll.h>

#include "header_checks.h"

static void poll_h() {
  TYPE(struct pollfd);
  STRUCT_MEMBER(struct pollfd, int, fd);
  STRUCT_MEMBER(struct pollfd, short, events);
  STRUCT_MEMBER(struct pollfd, short, revents);

#if !defined(__GLIBC__)  // Our glibc is too old.
  TYPE(sigset_t);
  TYPE(struct timespec);
#endif

  TYPE(nfds_t);

  MACRO(POLLIN);
  MACRO(POLLRDNORM);
  MACRO(POLLRDBAND);
  MACRO(POLLPRI);
  MACRO(POLLOUT);
  MACRO(POLLWRNORM);
  MACRO(POLLWRBAND);
  MACRO(POLLERR);
  MACRO(POLLHUP);
  MACRO(POLLNVAL);

  FUNCTION(poll, int (*f)(struct pollfd[], nfds_t, int));
#if !defined(__GLIBC__)  // Our glibc is too old.
  FUNCTION(ppoll, int (*f)(struct pollfd[], nfds_t, const struct timespec*, const sigset_t*));
#endif
}
