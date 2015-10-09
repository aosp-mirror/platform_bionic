/*
 * Copyright (C) 2015 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdlib.h>

#include "private/ErrnoRestorer.h"

// The difference between inet_network(3) and inet_addr(3) is that
// inet_network uses host order and inet_addr network order.
in_addr_t inet_network(const char* cp) {
  in_addr_t network_order = inet_addr(cp);
  return ntohl(network_order);
}

in_addr_t inet_addr(const char* cp) {
  in_addr addr;
  return inet_aton(cp, &addr) ? addr.s_addr : INADDR_NONE;
}

int inet_aton(const char* cp, in_addr* addr) {
  ErrnoRestorer errno_restorer;

  unsigned long parts[4];
  size_t i;
  for (i = 0; i < 4; ++i) {
    char* end;
    parts[i] = strtoul(cp, &end, 0);
    if (end == cp || (*end != '.' && *end != '\0')) return 0;
    if (*end == '\0') break;
    cp = end + 1;
  }

  uint32_t result = 0;
  if (i == 0) {
    // a (a 32-bit).
    if (parts[0] > 0xffffffff) return 0;
    result = parts[0];
  } else if (i == 1) {
    // a.b (b 24-bit).
    if (parts[0] > 0xff || parts[1] > 0xffffff) return 0;
    result = (parts[0] << 24) | parts[1];
  } else if (i == 2) {
    // a.b.c (c 16-bit).
    if (parts[0] > 0xff || parts[1] > 0xff || parts[2] > 0xffff) return 0;
    result = (parts[0] << 24) | (parts[1] << 16) | parts[2];
  } else if (i == 3) {
    // a.b.c.d (d 8-bit).
    if (parts[0] > 0xff || parts[1] > 0xff || parts[2] > 0xff || parts[3] > 0xff) return 0;
    result = (parts[0] << 24) | (parts[1] << 16) | (parts[2] << 8) | parts[3];
  } else {
    return 0;
  }

  if (addr != nullptr) addr->s_addr = htonl(result);
  return 1;
}
