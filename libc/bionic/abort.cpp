/*
 * Copyright (c) 1985 Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <signal.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <unistd.h>

#include "private/bionic_inline_raise.h"

void abort() {
  // Don't block SIGABRT to give any signal handler a chance; we ignore
  // any errors -- X311J doesn't allow abort to return anyway.
  sigset64_t mask;
  sigfillset64(&mask);
  sigdelset64(&mask, SIGABRT);

  sigprocmask64(SIG_SETMASK, &mask, nullptr);
  inline_raise(SIGABRT);

  // If SIGABRT is ignored or it's caught and the handler returns,
  // remove the SIGABRT signal handler and raise SIGABRT again.
  struct sigaction64 sa = { .sa_handler = SIG_DFL, .sa_flags = SA_RESTART };
  sigaction64(SIGABRT, &sa, nullptr);

  sigprocmask64(SIG_SETMASK, &mask, nullptr);
  inline_raise(SIGABRT);

  // If we get this far, just exit.
  _exit(127);
}
