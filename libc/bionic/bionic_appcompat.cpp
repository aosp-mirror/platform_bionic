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

// b/170214442: Intercept bind calls on NETLINK_ROUTE sockets and getifaddrs() calls.
// This entire file will be reverted before release.

#include <async_safe/log.h>
#include <cutils/misc.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#include "bionic_appcompat.h"

bool should_apply_soft_mac_restrictions(const char* const allowlist[]) {
  if (getuid() < FIRST_APPLICATION_UID) {
    // System app. No restrictions should be applied.
    return false;
  }
  if (android_get_application_target_sdk_version() >= __ANDROID_API_R__) {
    // Restrictions already applied by SELinux. Behave as normally.
    return false;
  }

  char package_name[MAX_PACKAGE_NAME_LENGTH + 1];
  if (get_package_name(package_name, sizeof(package_name)) < 0) {
    // Error in getting own package name. Apply restrictions by default.
    async_safe_format_log(ANDROID_LOG_ERROR, "mac-restrictions",
                          "Could not determine own package name for uid %d", getuid());
    return true;
  }
  for (int i = 0; allowlist[i] != nullptr; i++) {
    if (strcmp(package_name, allowlist[i]) == 0) {
      async_safe_format_log(ANDROID_LOG_WARN, "mac-restrictions",
                            "Temporarily allowing %s to bypass MAC address restrictions.",
                            package_name);
      return false;
    }
  }
  return true;
}

bool should_apply_soft_mac_bind_restrictions() {
  return should_apply_soft_mac_restrictions(soft_mac_bind_allowlist);
}

bool should_apply_soft_mac_getlink_restrictions() {
  return should_apply_soft_mac_restrictions(soft_mac_getlink_allowlist);
}

int get_package_name(char* buffer, const int bufferlen) {
  int file = open("/proc/self/cmdline", O_RDONLY | O_CLOEXEC);
  if (file < 0) {
    return file;
  }

  ssize_t len = read(file, buffer, bufferlen - 1);
  if (len < 0) {
    close(file);
    return -1;
  }
  buffer[len] = 0;

  close(file);
  return 0;
}
