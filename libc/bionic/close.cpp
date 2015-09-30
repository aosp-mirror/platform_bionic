/*
 * Copyright (C) 2015 The Android Open Source Project
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
#include <unistd.h>

extern "C" int ___close(int);

int close(int fd) {
  int rc = ___close(fd);
  if (rc == -1 && errno == EINTR) {
    // POSIX says that if close returns with EINTR, the fd must not be closed.
    // Linus disagrees: http://lkml.indiana.edu/hypermail/linux/kernel/0509.1/0877.html
    // The future POSIX solution is posix_close (http://austingroupbugs.net/view.php?id=529),
    // with the state after EINTR being undefined, and EINPROGRESS for the case where close
    // was interrupted by a signal but the file descriptor was actually closed.
    // My concern with that future behavior is that it breaks existing code that assumes
    // that close only returns -1 if it failed. Unlike other system calls, I have real
    // difficulty even imagining a caller that would need to know that close was interrupted
    // but succeeded. So returning EINTR is wrong (because Linux always closes) and EINPROGRESS
    // is harmful because callers need to be rewritten to understand that EINPROGRESS isn't
    // actually a failure, but will be reported as one.

    // We don't restore errno because that would incur a cost (the TLS read) for every caller.
    // Since callers don't know ahead of time whether close will legitimately fail, they need
    // to have stashed the old errno value anyway if they plan on using it afterwards, so
    // us clobbering errno here doesn't change anything in that respect.
    return 0;
  }
  return rc;
}
