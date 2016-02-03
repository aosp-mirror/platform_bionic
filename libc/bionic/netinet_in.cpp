/*
 * Copyright (C) 2008 The Android Open Source Project
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

#include <netinet/in.h>

#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>

constexpr int START_PORT = 600;
constexpr int END_PORT = IPPORT_RESERVED;
constexpr int NUM_PORTS = (END_PORT - START_PORT);

int bindresvport(int sd, struct sockaddr_in* sin) {
  sockaddr_in sin0;
  if (sin == nullptr) {
    memset(&sin0, 0, sizeof(sin0));
    sin = &sin0;
    sin->sin_family = AF_INET;
  }

  if (sin->sin_family != AF_INET) {
    errno = EPFNOSUPPORT;
    return -1;
  }

  // TODO: thread safety!
  static short port;
  if (port == 0) {
    port = START_PORT + (getpid() % NUM_PORTS);
  }

  for (size_t i = NUM_PORTS; i > 0; i--, port++) {
    if (port == END_PORT) port = START_PORT;
    sin->sin_port = htons(port);
    int rc = TEMP_FAILURE_RETRY(bind(sd, reinterpret_cast<sockaddr*>(sin), sizeof(*sin)));
    if (rc >= 0) return rc;
  }
  return -1;
}

const in6_addr in6addr_any = IN6ADDR_ANY_INIT;
const in6_addr in6addr_loopback = IN6ADDR_LOOPBACK_INIT;
