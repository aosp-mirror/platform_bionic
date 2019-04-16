/*
 * Copyright (C) 2018 The Android Open Source Project
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
#include <stdlib.h>
#include <spawn.h>
#include <sys/wait.h>
#include <unistd.h>

#include "private/__bionic_get_shell_path.h"
#include "private/ScopedSignalBlocker.h"
#include "private/ScopedSignalHandler.h"

int system(const char* command) {
  // "The system() function shall always return non-zero when command is NULL."
  // http://pubs.opengroup.org/onlinepubs/9699919799/functions/system.html
  if (command == nullptr) return 1;

  ScopedSignalBlocker sigchld_blocker(SIGCHLD);
  ScopedSignalHandler sigint_ignorer(SIGINT, SIG_IGN);
  ScopedSignalHandler sigquit_ignorer(SIGQUIT, SIG_IGN);

  sigset64_t default_mask = {};
  if (sigint_ignorer.old_action_.sa_handler != SIG_IGN) sigaddset64(&default_mask, SIGINT);
  if (sigquit_ignorer.old_action_.sa_handler != SIG_IGN) sigaddset64(&default_mask, SIGQUIT);

  static constexpr int flags = POSIX_SPAWN_SETSIGDEF | POSIX_SPAWN_SETSIGMASK;
  posix_spawnattr_t attributes;
  if ((errno = posix_spawnattr_init(&attributes))) return -1;
  if ((errno = posix_spawnattr_setsigdefault64(&attributes, &default_mask))) return -1;
  if ((errno = posix_spawnattr_setsigmask64(&attributes, &sigchld_blocker.old_set_))) return -1;
  if ((errno = posix_spawnattr_setflags(&attributes, flags))) return -1;

  const char* argv[] = { "sh", "-c", command, nullptr };
  pid_t child;
  if ((errno = posix_spawn(&child, __bionic_get_shell_path(), nullptr, &attributes,
                           const_cast<char**>(argv), environ)) != 0) {
    return -1;
  }

  posix_spawnattr_destroy(&attributes);

  int status;
  pid_t pid = TEMP_FAILURE_RETRY(waitpid(child, &status, 0));
  return (pid == -1 ? -1 : status);
}
