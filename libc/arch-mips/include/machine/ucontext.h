/*
 * Copyright (C) 2012 The Android Open Source Project
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
#ifndef _ARCH_MIPS_SYS_UCONTEXT_H_
#define _ARCH_MIPS_SYS_UCONTEXT_H_

#include <signal.h>
#include <stdint.h>

__BEGIN_DECLS

/* Technical note:
 *
 * On MIPS, there are differences in the way GLibc and the Linux kernel declare
 * ucontext_t:
 *
 *  - The kernel implements 'uc_mcontext' with a 'struct sigcontext',
 *    while GLibc defines 'mcontext_t' in a binary-compatible way
 *    (same size, same binary layout), but with different field naming/access
 *    convention.
 *
 *    The biggest difference is the naming of the fields. The kernel uses
 *    a 'sc_' prefix (e.g. "sc_gregs[]"), while GLibc does not ("gregs[]").
 *
 *    For maximum portability of existing client code, this header follows
 *    the GLibc convention. Client code, and the C library, should never
 *    try to include <asm/ucontext.h>
 *
 *  Apart from that, all floating point state is stored in uc_mcontext
 *  in the same way, which is much simpler than ARM and x86 schemes for it.
 *  As such, there is no _libc_fpstate.
 *
 *  Reference source files:
 *    $KERNEL/arch/mips/include/asm/sigcontext.h
 *    $KERNEL/arch/mips/include/asm/ucontext.h
 *
 *    $GLIBC/sysdeps/unix/sysv/linux/mips/getcontext.S
 *    $GLIBC/sysdeps/unix/sysv/linux/mips/setcontext.S
 *    $GLIBC/sysdeps/unix/sysv/linux/mips/sys/ucontext.h
 */

/* First, the kernel-compatible version, for reference. */
typedef struct __kernel_ucontext {
  unsigned long              uc_flags;
  struct __kernel_ucontext*  uc_link;
  stack_t                    uc_stack;
  struct sigcontext          uc_mcontext;
  __kernel_sigset_t          uc_sigmask;
} __kernel_ucontext_t;

/* Second, the GLibc-compatible version */

__extension__ /* Required to build in ANSI C mode */
typedef unsigned long long greg_t;

#define NGREG   32
#define NFPREG  32

typedef greg_t  gregset_t[NGREG];

typedef struct fpregset {
  union {
    double fp_dregs[NFPREG];
    struct {
      float     _fp_fregs;
      unsigned  _fp_pad;
    } fp_fregs[NFPREG];
  } fp_r;
} fpregset_t;

#ifndef _MIPS_SIM
#error "_MIPS_SIM should be defined by your Android MIPS toolchain!"
#endif

#ifndef _OABI32
#error "_OABI32 should be defined by your Android MIPS toolchain!"
#endif

#if _MIPS_SIM != _OABI32
#error "Only _OABI32 is supported on Android MIPS!"
#endif

typedef struct {
  unsigned   regmask;
  unsigned   status;
  greg_t     pc;
  gregset_t  gregs;
  fpregset_t fpregs;
  unsigned   fp_owned;
  unsigned   fpc_csr;
  unsigned   fpc_eir;
  unsigned   used_math;
  unsigned   dsp;
  greg_t     mdhi;
  greg_t     mdlo;
  unsigned   hi1;
  unsigned   lo1;
  unsigned   hi2;
  unsigned   lo2;
  unsigned   hi3;
  unsigned   lo3;
} mcontext_t;

typedef struct ucontext {
  uint32_t          uc_flags;
  struct ucontext*  uc_link;
  stack_t           uc_stack;
  mcontext_t        uc_mcontext;
  /* Only expose the signals in Bionic's 32-bit sigset_t.
   * The _unused field is required padding from the kernel. */
  sigset_t          uc_sigmask;
  char _unused[sizeof(__kernel_sigset_t) - sizeof(sigset_t)];
} ucontext_t;

__END_DECLS

#endif /* _ARCH_MIPS_SYS_UCONTEXT_H_ */
