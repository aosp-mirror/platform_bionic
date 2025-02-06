// Copyright (C) 2017 The Android Open Source Project
// SPDX-License-Identifier: BSD-2-Clause

#include <sys/uio.h>

#include "header_checks.h"

static void sys_uio_h() {
  TYPE(struct iovec);
  STRUCT_MEMBER(struct iovec, void*, iov_base);
  STRUCT_MEMBER(struct iovec, size_t, iov_len);

  TYPE(ssize_t);
  TYPE(size_t);

  FUNCTION(readv, ssize_t (*f)(int, const struct iovec*, int));
  FUNCTION(writev, ssize_t (*f)(int, const struct iovec*, int));
}
