/*-
 * Copyright (c) 1991, 1993
 *	The Regents of the University of California.  All rights reserved.
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

#include <sys/types.h>
#include <sys/uio.h>

#include <errno.h>
#include <limits.h>
#include <paths.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

extern "C" char** environ;

enum ExecVariant { kIsExecL, kIsExecLE, kIsExecLP };

static int __execl(const char* name, const char* argv0, ExecVariant variant, va_list ap) {
  // Count the arguments.
  va_list count_ap;
  va_copy(count_ap, ap);
  size_t n = 1;
  while (va_arg(count_ap, char*) != nullptr) {
    ++n;
  }
  va_end(count_ap);

  // Construct the new argv.
  char* argv[n + 1];
  argv[0] = const_cast<char*>(argv0);
  n = 1;
  while ((argv[n] = va_arg(ap, char*)) != nullptr) {
    ++n;
  }

  // Collect the argp too.
  char** argp = (variant == kIsExecLE) ? va_arg(ap, char**) : environ;

  va_end(ap);

  return (variant == kIsExecLP) ? execvp(name, argv) : execve(name, argv, argp);
}

int execl(const char* name, const char* arg, ...) {
  va_list ap;
  va_start(ap, arg);
  return __execl(name, arg, kIsExecL, ap);
}

int execle(const char* name, const char* arg, ...) {
  va_list ap;
  va_start(ap, arg);
  return __execl(name, arg, kIsExecLE, ap);
}

int execlp(const char* name, const char* arg, ...) {
  va_list ap;
  va_start(ap, arg);
  return __execl(name, arg, kIsExecLP, ap);
}

int execv(const char* name, char* const* argv) {
  return execve(name, argv, environ);
}

int execvp(const char* name, char* const* argv) {
  return execvpe(name, argv, environ);
}

static int __exec_as_script(const char* buf, char* const* argv, char* const* envp) {
  size_t arg_count = 1;
  while (argv[arg_count] != nullptr) ++arg_count;

  const char* script_argv[arg_count + 2];
  script_argv[0] = "sh";
  script_argv[1] = buf;
  memcpy(script_argv + 2, argv + 1, arg_count * sizeof(char*));
  return execve(_PATH_BSHELL, const_cast<char**>(script_argv), envp);
}

int execvpe(const char* name, char* const* argv, char* const* envp) {
  // Do not allow null name.
  if (name == nullptr || *name == '\0') {
    errno = ENOENT;
    return -1;
  }

  // If it's an absolute or relative path name, it's easy.
  if (strchr(name, '/') && execve(name, argv, envp) == -1) {
    if (errno == ENOEXEC) return __exec_as_script(name, argv, envp);
    return -1;
  }

  // Get the path we're searching.
  const char* path = getenv("PATH");
  if (path == nullptr) path = _PATH_DEFPATH;

  // Make a writable copy.
  size_t len = strlen(path) + 1;
  char writable_path[len];
  memcpy(writable_path, path, len);

  bool saw_EACCES = false;

  // Try each element of $PATH in turn...
  char* strsep_buf = writable_path;
  const char* dir;
  while ((dir = strsep(&strsep_buf, ":"))) {
    // It's a shell path: double, leading and trailing colons
    // mean the current directory.
    if (*dir == '\0') dir = const_cast<char*>(".");

    size_t dir_len = strlen(dir);
    size_t name_len = strlen(name);

    char buf[dir_len + 1 + name_len + 1];
    mempcpy(mempcpy(mempcpy(buf, dir, dir_len), "/", 1), name, name_len + 1);

    execve(buf, argv, envp);
    switch (errno) {
    case EISDIR:
    case ELOOP:
    case ENAMETOOLONG:
    case ENOENT:
    case ENOTDIR:
      break;
    case ENOEXEC:
      return __exec_as_script(buf, argv, envp);
    case EACCES:
      saw_EACCES = true;
      break;
    default:
      return -1;
    }
  }
  if (saw_EACCES) errno = EACCES;
  return -1;
}
