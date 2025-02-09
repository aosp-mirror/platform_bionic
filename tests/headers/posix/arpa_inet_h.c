// Copyright (C) 2017 The Android Open Source Project
// SPDX-License-Identifier: BSD-2-Clause

#include <arpa/inet.h>

#include "header_checks.h"

static void arpa_inet_h() {
  TYPE(in_port_t);
  TYPE(in_addr_t);
  TYPE(struct in_addr);

  MACRO(INET_ADDRSTRLEN);
  MACRO(INET6_ADDRSTRLEN);

  FUNCTION(htonl, uint32_t (*f)(uint32_t));
  FUNCTION(htons, uint16_t (*f)(uint16_t));
  FUNCTION(ntohl, uint32_t (*f)(uint32_t));
  FUNCTION(ntohs, uint16_t (*f)(uint16_t));

  TYPE(uint32_t);
  TYPE(uint16_t);

  FUNCTION(inet_addr, in_addr_t (*f)(const char*));
}
