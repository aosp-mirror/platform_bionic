/*
 * Copyright (C) 2010 The Android Open Source Project
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

#include <netinet/ether.h>

#include <stdio.h>

ether_addr* ether_aton_r(const char* asc, ether_addr* addr) {
  int bytes[ETHER_ADDR_LEN], end;
  int n = sscanf(asc, "%x:%x:%x:%x:%x:%x%n",
                 &bytes[0], &bytes[1], &bytes[2],
                 &bytes[3], &bytes[4], &bytes[5], &end);
  if (n != ETHER_ADDR_LEN || asc[end] != '\0') return NULL;
  for (int i = 0; i < ETHER_ADDR_LEN; i++) {
    if (bytes[i] > 0xff) return NULL;
    addr->ether_addr_octet[i] = bytes[i];
  }
  return addr;
}

struct ether_addr* ether_aton(const char* asc) {
  static ether_addr addr;
  return ether_aton_r(asc, &addr);
}

char* ether_ntoa_r(const ether_addr* addr, char* buf) {
  snprintf(buf, 18, "%02x:%02x:%02x:%02x:%02x:%02x",
           addr->ether_addr_octet[0], addr->ether_addr_octet[1],
           addr->ether_addr_octet[2], addr->ether_addr_octet[3],
           addr->ether_addr_octet[4], addr->ether_addr_octet[5]);
  return buf;
}

char* ether_ntoa(const ether_addr* addr) {
  static char buf[18];
  return ether_ntoa_r(addr, buf);
}
