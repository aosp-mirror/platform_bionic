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
#ifndef _SIGNAL_H_
#define _SIGNAL_H_

#include <sys/cdefs.h>
#include <sys/types.h>
#include <machine/signal.h>
#include <machine/ucontext.h>

#include <limits.h>             /* For LONG_BIT */
#include <string.h>             /* For memset() */

__BEGIN_DECLS

/* Previous versions of <signal.h> didn't define 'struct sigcontext'. This
 * has led client code to redefine the structure, sometimes in a custom and
 * incompatible way.
 *
 * The __BIONIC_HAVE_STRUCT_SIGCONTEXT macro is defined in <sys/cdefs.h>
 * to indicate that <signal.h> does indeed define the structure. For
 * maximum source compatibility, client code should follow these guidelines:
 *
 *  - If client code is guaranteed to build against a recent NDK/platform
 *    version, just include <signal.h> as usual, and use the
 *    'struct sigcontext'.
 *
 *  - If client code wants to build against older NDK/platform versions,
 *    and only uses 'struct sigcontext' without accessing _any_
 *    of its fields, just always include <asm/sigcontext.h> on Android
 *    to ensure maximum portability (i.e. to old platform/NDK releases).
 *    As in:
 *
 *        #include <signal.h>
 *        #ifdef __BIONIC__
 *        #include <asm/sigcontext.h>
 *        #endif
 *
 *  - Otherwise, if the client has a custom sigcontext definition and wants to
 *    keep it to minimize source changes, use a macro trick as below to avoid
 *    naming conflicts:
 *
 *        #include <signal.h>
 *        #ifdef __BIONIC__
 *        #  include <asm/sigcontext.h>
 *        #  ifdef __BIONIC_HAVE_STRUCT_SIGCONTEXT
 *        #    undef sigcontext
 *        #    define sigcontext  my_custom_sigcontext
 *        #  endif
 *        #endif
 */

/* Similarly, previous versions of the C library didn't define ucontext_t
 * and mcontext_t in <signal.h>, or <sys/ucontext.h>. Client code can check
 * against the macro __BIONIC_HAVE_UCONTEXT_T to determine if it does.
 *
 * Beware, that does _not_ mean that <ucontext.h> and related functions are
 * available.
 */

typedef int sig_atomic_t;

extern const char * const sys_siglist[];
extern const char * const sys_signame[];

static __inline__ int sigismember(sigset_t *set, int signum)
{
    unsigned long *local_set = (unsigned long *)set;
    signum--;
    return (int)((local_set[signum/LONG_BIT] >> (signum%LONG_BIT)) & 1);
}


static __inline__ int sigaddset(sigset_t *set, int signum)
{
    unsigned long *local_set = (unsigned long *)set;
    signum--;
    local_set[signum/LONG_BIT] |= 1UL << (signum%LONG_BIT);
    return 0;
}


static __inline__ int sigdelset(sigset_t *set, int signum)
{
    unsigned long *local_set = (unsigned long *)set;
    signum--;
    local_set[signum/LONG_BIT] &= ~(1UL << (signum%LONG_BIT));
    return 0;
}


static __inline__ int sigemptyset(sigset_t *set)
{
    memset(set, 0, sizeof *set);
    return 0;
}

static __inline__ int sigfillset(sigset_t *set)
{
    memset(set, ~0, sizeof *set);
    return 0;
}


/* compatibility types */
typedef void  (*sig_t)(int);
typedef sig_t sighandler_t;

/* differentiater between sysv and bsd behaviour 8*/
extern __sighandler_t sysv_signal(int, __sighandler_t);
extern __sighandler_t bsd_signal(int, __sighandler_t);

/* the default is bsd */
static __inline__ __sighandler_t signal(int s, __sighandler_t f)
{
    return bsd_signal(s,f);
}

/* the syscall itself */
extern __sighandler_t __signal(int, __sighandler_t, int);

extern int sigprocmask(int, const sigset_t *, sigset_t *);
extern int sigaction(int, const struct sigaction *, struct sigaction *);

extern int sigpending(sigset_t *);
extern int sigsuspend(const sigset_t *);
extern int sigwait(const sigset_t *set, int *sig);
extern int siginterrupt(int  sig, int  flag);

extern int raise(int);
extern int kill(pid_t, int);
extern int killpg(int pgrp, int sig);
extern int sigaltstack(const stack_t *ss, stack_t *oss);

extern void psiginfo(const siginfo_t* si, const char* message);
extern void psignal(int signal_number, const char* message);

__END_DECLS

#endif /* _SIGNAL_H_ */
