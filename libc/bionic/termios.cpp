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

#include <termios.h>
#include <unistd.h>

// Most of termios was missing in the platform until L, but available as inlines in the NDK.
// We share definitions with the NDK to avoid bugs (https://github.com/android-ndk/ndk/issues/441).
#define __BIONIC_TERMIOS_INLINE /* Out of line. */
#include <bits/termios_inlines.h>

// POSIX added a couple more functions much later, so do the same for them.
#define __BIONIC_TERMIOS_WINSIZE_INLINE /* Out of line. */
#include <bits/termios_winsize_inlines.h>

// Actually declared in <unistd.h>, present on all API levels.
pid_t tcgetpgrp(int fd) {
  pid_t pid;
  return (ioctl(fd, TIOCGPGRP, &pid) == -1) ? -1 : pid;
}

// Actually declared in <unistd.h>, present on all API levels.
int tcsetpgrp(int fd, pid_t pid) {
  return ioctl(fd, TIOCSPGRP, &pid);
}
