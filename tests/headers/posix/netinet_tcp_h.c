// Copyright (C) 2017 The Android Open Source Project
// SPDX-License-Identifier: BSD-2-Clause

#include <netinet/tcp.h>

#include "header_checks.h"

static void netinet_tcp_h() {
  MACRO(TCP_NODELAY);
}
