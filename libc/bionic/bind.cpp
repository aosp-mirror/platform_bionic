/*
 * Copyright (C) 2020 The Android Open Source Project
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

// b/170214442: Intercept bind calls on NETLINK_ROUTE sockets.
// This entire file will be reverted before release.

#include <async_safe/log.h>
#include <errno.h>
#include <linux/rtnetlink.h>
#include <string.h>
#include <sys/socket.h>

#include "bionic_appcompat.h"

extern "C" int __bind(int fd, const struct sockaddr* addr, socklen_t addr_length);

int bind(int fd, const struct sockaddr* addr, socklen_t addr_length) {
  if (should_apply_soft_mac_bind_restrictions()) {
    int sock_protocol;
    socklen_t sock_length = sizeof(sock_protocol);
    if (getsockopt(fd, SOL_SOCKET, SO_PROTOCOL, &sock_protocol, &sock_length) < 0) {
      async_safe_format_log(ANDROID_LOG_ERROR, "mac-restrictions",
                            "Could not get socket protocol: %s", strerror(errno));
    } else if (NETLINK_ROUTE == sock_protocol) {
      char package_name[MAX_PACKAGE_NAME_LENGTH + 1];
      get_package_name(package_name, sizeof(package_name));
      async_safe_format_log(
          ANDROID_LOG_ERROR, "mac-restrictions",
          "%s tried to call bind() on a NETLINK_ROUTE socket, which is not allowed. Please follow "
          "instructions at go/netlink-bug if this app behaves incorrectly.",
          package_name);
      errno = EACCES;
      return -1;
    }
  }

  return __bind(fd, addr, addr_length);
}
