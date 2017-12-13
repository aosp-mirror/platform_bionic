/*
 * Copyright (C) 2017 The Android Open Source Project
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

#include <errno.h>
#include <string.h>
#include <sys/cdefs.h>
#include <unistd.h>

#define VENDOR_PREFIX "/vendor/"

static const char* init_sh_path() {
  /* If the device is not treble enabled, return the path to the system shell.
   * Vendor code, on non-treble enabled devices could use system() / popen()
   * with relative paths for executables on /system. Since /system will not be
   * in $PATH for the vendor shell, simply return the system shell.
   */

#ifdef TREBLE_LINKER_NAMESPACES
  /* look for /system or /vendor prefix */
  char exe_path[strlen(VENDOR_PREFIX)];
  ssize_t len = readlink("/proc/self/exe", exe_path, sizeof(exe_path));
  if (len != -1 && !strncmp(exe_path, VENDOR_PREFIX, strlen(VENDOR_PREFIX))) {
    return "/vendor/bin/sh";
  }
#endif
  return "/system/bin/sh";
}

__LIBC_HIDDEN__ extern "C" const char* __bionic_get_shell_path() {
  static const char* sh_path = init_sh_path();
  return sh_path;
}
