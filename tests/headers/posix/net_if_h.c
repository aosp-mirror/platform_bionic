// Copyright (C) 2017 The Android Open Source Project
// SPDX-License-Identifier: BSD-2-Clause

#include <net/if.h>

#include "header_checks.h"

static void net_if_h() {
  TYPE(struct if_nameindex);
  STRUCT_MEMBER(struct if_nameindex, unsigned, if_index);
  STRUCT_MEMBER(struct if_nameindex, char*, if_name);

  MACRO(IF_NAMESIZE);

  FUNCTION(if_freenameindex, void (*f)(struct if_nameindex*));
  FUNCTION(if_indextoname, char* (*f)(unsigned, char*));
  FUNCTION(if_nameindex, struct if_nameindex* (*f)(void));
  FUNCTION(if_nametoindex, unsigned (*f)(const char*));
}
