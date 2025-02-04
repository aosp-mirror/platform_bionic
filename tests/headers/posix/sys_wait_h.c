// Copyright (C) 2017 The Android Open Source Project
// SPDX-License-Identifier: BSD-2-Clause

#include <sys/wait.h>

#include "header_checks.h"

static void sys_wait_h() {
  MACRO(WCONTINUED);
  MACRO(WNOHANG);
  MACRO(WUNTRACED);

#if !defined(WEXITSTATUS)
#error WEXITSTATUS
#endif
#if !defined(WIFCONTINUED)
#error WIFCONTINUED
#endif
#if !defined(WIFEXITED)
#error WIFEXITED
#endif
#if !defined(WIFSIGNALED)
#error WIFSIGNALED
#endif
#if !defined(WIFSTOPPED)
#error WIFSTOPPED
#endif
#if !defined(WSTOPSIG)
#error WSTOPSIG
#endif
#if !defined(WTERMSIG)
#error WTERMSIG
#endif

  MACRO(WEXITED);
  MACRO(WNOWAIT);
  MACRO(WSTOPPED);

  TYPE(idtype_t);
  MACRO(P_ALL);
  MACRO(P_PGID);
  MACRO(P_PID);

  TYPE(id_t);
  TYPE(pid_t);
  TYPE(siginfo_t);
  TYPE(union sigval);

  FUNCTION(wait, pid_t (*f)(int*));
  FUNCTION(waitid, int (*f)(idtype_t, id_t, siginfo_t*, int));
  FUNCTION(waitpid, pid_t (*f)(pid_t, int*, int));
}
