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

#include <signal.h>

#include "header_checks.h"

static void signal_h() {
  void (*h)(int);
  h = SIG_DFL;
  h = SIG_ERR;
  h = SIG_HOLD;
  h = SIG_IGN;

  TYPE(pthread_t);
  TYPE(size_t);
  TYPE(uid_t);

  TYPE(struct timespec);

  TYPE(sig_atomic_t);
  TYPE(pid_t);

  TYPE(pthread_attr_t);

  TYPE(struct sigevent);
  STRUCT_MEMBER(struct sigevent, int, sigev_notify);
  STRUCT_MEMBER(struct sigevent, int, sigev_signo);
  STRUCT_MEMBER(struct sigevent, union sigval, sigev_value);
  STRUCT_MEMBER_FUNCTION_POINTER(struct sigevent, void (*f)(union sigval), sigev_notify_function);
#if defined(__BIONIC__)
  STRUCT_MEMBER(struct sigevent, void*, sigev_notify_attributes);
#else
  STRUCT_MEMBER(struct sigevent, pthread_attr_t*, sigev_notify_attributes);
#endif

  MACRO(SIGEV_NONE);
  MACRO(SIGEV_SIGNAL);
  MACRO(SIGEV_THREAD);

  TYPE(union sigval);
  STRUCT_MEMBER(union sigval, int, sival_int);
  STRUCT_MEMBER(union sigval, void*, sival_ptr);

  int i;
  i = SIGRTMIN;
  i = SIGRTMAX;

  MACRO(SIGABRT);
  MACRO(SIGALRM);
  MACRO(SIGBUS);
  MACRO(SIGCHLD);
  MACRO(SIGCONT);
  MACRO(SIGFPE);
  MACRO(SIGHUP);
  MACRO(SIGILL);
  MACRO(SIGINT);
  MACRO(SIGKILL);
  MACRO(SIGPIPE);
  MACRO(SIGQUIT);
  MACRO(SIGSEGV);
  MACRO(SIGSTOP);
  MACRO(SIGTERM);
  MACRO(SIGTSTP);
  MACRO(SIGTTIN);
  MACRO(SIGTTOU);
  MACRO(SIGUSR1);
  MACRO(SIGUSR2);
  MACRO(SIGPOLL);
  MACRO(SIGPROF);
  MACRO(SIGSYS);
  MACRO(SIGTRAP);
  MACRO(SIGURG);
  MACRO(SIGVTALRM);
  MACRO(SIGXCPU);
  MACRO(SIGXFSZ);

  TYPE(struct sigaction);
  STRUCT_MEMBER_FUNCTION_POINTER(struct sigaction, void (*f)(int), sa_handler);
  STRUCT_MEMBER(struct sigaction, sigset_t, sa_mask);
  STRUCT_MEMBER(struct sigaction, int, sa_flags);
  STRUCT_MEMBER_FUNCTION_POINTER(struct sigaction, void (*f)(int, siginfo_t*, void*), sa_sigaction);

  i = SIG_BLOCK;
  i = SIG_UNBLOCK;
  i = SIG_SETMASK;

  MACRO(SA_NOCLDSTOP);
  MACRO(SA_ONSTACK);
  MACRO(SA_RESETHAND);
  MACRO(SA_RESTART);
  MACRO(SA_SIGINFO);
  MACRO(SA_NOCLDWAIT);
  MACRO(SA_NODEFER);

  MACRO(SS_ONSTACK);
  MACRO(SS_DISABLE);

  MACRO(MINSIGSTKSZ);
  MACRO(SIGSTKSZ);

  TYPE(mcontext_t);

  TYPE(ucontext_t);
  STRUCT_MEMBER(ucontext_t, ucontext_t*, uc_link);
  STRUCT_MEMBER(ucontext_t, sigset_t, uc_sigmask);
  STRUCT_MEMBER(ucontext_t, stack_t, uc_stack);
  STRUCT_MEMBER(ucontext_t, mcontext_t, uc_mcontext);

  TYPE(stack_t);
  STRUCT_MEMBER(stack_t, void*, ss_sp);
  STRUCT_MEMBER(stack_t, size_t, ss_size);
  STRUCT_MEMBER(stack_t, int, ss_flags);

  TYPE(siginfo_t);
  STRUCT_MEMBER(siginfo_t, int, si_signo);
  STRUCT_MEMBER(siginfo_t, int, si_code);
  STRUCT_MEMBER(siginfo_t, int, si_errno);
  STRUCT_MEMBER(siginfo_t, pid_t, si_pid);
  STRUCT_MEMBER(siginfo_t, uid_t, si_uid);
  STRUCT_MEMBER(siginfo_t, void*, si_addr);
  STRUCT_MEMBER(siginfo_t, int, si_status);
  STRUCT_MEMBER(siginfo_t, long, si_band);
  STRUCT_MEMBER(siginfo_t, union sigval, si_value);

  i = ILL_ILLOPC;
  i = ILL_ILLOPN;
  i = ILL_ILLADR;
  i = ILL_ILLTRP;
  i = ILL_PRVOPC;
  i = ILL_PRVREG;
  i = ILL_COPROC;
  i = ILL_BADSTK;

  i = FPE_INTDIV;
  i = FPE_INTOVF;
  i = FPE_FLTDIV;
  i = FPE_FLTOVF;
  i = FPE_FLTUND;
  i = FPE_FLTRES;
  i = FPE_FLTINV;
  i = FPE_FLTSUB;

  i = SEGV_MAPERR;
  i = SEGV_ACCERR;

  i = BUS_ADRALN;
  i = BUS_ADRERR;
  i = BUS_OBJERR;

  i = TRAP_BRKPT;
  i = TRAP_TRACE;

  i = CLD_EXITED;
  i = CLD_KILLED;
  i = CLD_DUMPED;
  i = CLD_TRAPPED;
  i = CLD_STOPPED;
  i = CLD_CONTINUED;

  i = POLL_IN;
  i = POLL_OUT;
  i = POLL_MSG;
  i = POLL_ERR;
  i = POLL_PRI;
  i = POLL_HUP;

  i = SI_USER;
  i = SI_QUEUE;
  i = SI_TIMER;
  i = SI_ASYNCIO;
  i = SI_MESGQ;

  typedef void (*signal_handler_type)(int);

  FUNCTION(kill, int (*f)(pid_t, int));
  FUNCTION(killpg, int (*f)(pid_t, int));
  FUNCTION(psiginfo, void (*f)(const siginfo_t*, const char*));
  FUNCTION(psignal, void (*f)(int, const char*));
  FUNCTION(pthread_kill, int (*f)(pthread_t, int));
  FUNCTION(pthread_sigmask, int (*f)(int, const sigset_t*, sigset_t*));
  FUNCTION(raise, int (*f)(int));
  FUNCTION(sigaction, int (*f)(int, const struct sigaction*, struct sigaction*));
  FUNCTION(sigaddset, int (*f)(sigset_t*, int));
  FUNCTION(sigaltstack, int (*f)(const stack_t*, stack_t*));
  FUNCTION(sigdelset, int (*f)(sigset_t*, int));
  FUNCTION(sigemptyset, int (*f)(sigset_t*));
  FUNCTION(sigfillset, int (*f)(sigset_t*));
  FUNCTION(sighold, int (*f)(int));
  FUNCTION(sigignore, int (*f)(int));
  FUNCTION(siginterrupt, int (*f)(int, int));
  FUNCTION(sigismember, int (*f)(const sigset_t*, int));
  FUNCTION(signal, signal_handler_type (*f)(int, signal_handler_type));
  FUNCTION(sigpause, int (*f)(int));
  FUNCTION(sigpending, int (*f)(sigset_t*));
  FUNCTION(sigprocmask, int (*f)(int, const sigset_t*, sigset_t*));
  FUNCTION(sigqueue, int (*f)(pid_t, int, union sigval));
  FUNCTION(sigrelse, int (*f)(int));
  FUNCTION(sigset, signal_handler_type (*f)(int, signal_handler_type));
  FUNCTION(sigsuspend, int (*f)(const sigset_t*));
  FUNCTION(sigtimedwait, int (*f)(const sigset_t*, siginfo_t*, const struct timespec*));
  FUNCTION(sigwait, int (*f)(const sigset_t*, int*));
  FUNCTION(sigwaitinfo, int (*f)(const sigset_t*, siginfo_t*));
}
