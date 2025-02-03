// Copyright (C) 2017 The Android Open Source Project
// SPDX-License-Identifier: BSD-2-Clause

#include <sys/un.h>

#include "header_checks.h"

static void sys_un_h() {
  TYPE(struct sockaddr_un);
  STRUCT_MEMBER(struct sockaddr_un, sa_family_t, sun_family);
  STRUCT_MEMBER_ARRAY(struct sockaddr_un, char/*[]*/, sun_path);

  TYPE(sa_family_t);
}
