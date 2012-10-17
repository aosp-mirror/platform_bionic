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
#ifndef _ARCH_ARM_SYS_UCONTEXT_H_
#define _ARCH_ARM_SYS_UCONTEXT_H_

#include <stdint.h>
#include <sys/cdefs.h>
#include <sys/types.h>
#include <machine/signal.h>

__BEGIN_DECLS

/* Technical note:
 *
 * There are similarities and differences in the way GLibc and the ARM Linux
 * kernel declare ucontext_t. Here are the details:
 *
 *  - Both declare mcontext_t as a typedef for 'struct sigcontext'. This goes
 *    against the GLibc convention of providing a custom mcontext_t declaration
 *    for all other platforms.
 *
 *    One notable difference on ARM is that to access register values, one
 *    has to use named fields like "arm_r0", "arm_sp", "arm_pc".
 *
 *    On other platforms, GLibc mcontext_t provides a gregs[] array of
 *    32-bit values to access the value of core registers instead. Plus
 *    some helper enums + macros to index it (e.g. REG_EBP on i386).
 *
 *    Note that some client code, when ported to Android, provide custom
 *    mcontext_t declaration that use the gregs[] convention. For the sake
 *    of portability, this header follows the ARM GLibc convention of using
 *    explicit field names. This means that such client code will need to be
 *    adapted. Possibly by checking against __BIONIC_HAVE_UCONTEXT_T defined
 *    in <sys/cdefs.h>
 *
 * - On ARM, the floating-point state is not stored in mcontext_t.
 *   Instead, it is stored in the ucontext_t structure, in the '_uc_regspace'
 *   field at the end, but both the Kernel and GLibc use a different
 *   convention for exact layout:
 *
 *   * The state saved by the kernel (which would be accessible from the
 *     ucontext_t received from a SA_SIGINGO handler) has a layout that
 *     depends on the exact kernel configuration. For more details see
 *     the "struct aux_sigframe" declaration in the kernel's ucontext.h
 *     which defines the format. It is *not* exposed by this header.
 *
 *   * The FP state saved by GLibc's getcontext()/setcontext() functions
 *     uses a different custom storage scheme that depends on the reported
 *     ELF HWCAPs. If VFP is enabled, only registers d8-d15 are saved to
 *     the storage space, in addition to some status/exception registers.
 *     This is not surprising because d0-d7 are callee-saved in the
 *     ARM EABI.
 *
 *     It also looks like this implementation cannot handle VFPv3-D32/NEON
 *     by saving/restoring the state of d16-d31 available on these CPUs.
 *
 *   Note that client code that needs to access the FP state typically
 *   does so through ptrace() using the special PTRACE_GETVFPREGS op.
 *   This is what gdbserver and debuggerd use to extract that information.
 *
 *   In conclusion, there is no reliable way to extract the FP state from
 *   a ucontext_t on ARM.
 *
 * - The ARM GLibc ucontext.h defines a 'struct _libc_fpstate' structure
 *   that is never used by GLibc source code, and which doesn't seem to
 *   match either the kernel of getcontext/setcontext() layout.
 *
 *   It was introduced by the following patch, reason is unclear but may be
 *   related to the GLibc test suite:
 *
 *      http://www.cygwin.com/ml/libc-ports/2011-12/msg00032.html
 *
 *   This header does *not* define this type, or fpregset_t either.
 *
 *  Reference source files:
 *    $KERNEL/arch/arm/include/asm/fpstate.h
 *    $KERNEL/arch/arm/include/asm/sigcontext.h
 *    $KERNEL/arch/arm/include/asm/ucontext.h
 *    $KERNEL/arch/arm/include/asm/user.h
 *
 *    $GLIBC/ports/sysdeps/unix/sysv/linux/arm/getcontext.S
 *    $GLIBC/ports/sysdeps/unix/sysv/linux/arm/setcontext.S
 *    $GLIBC/ports/sysdeps/unix/sysv/linux/arm/sys/ucontext.h
 */

/* First, the kernel-compatible version, for reference. */
typedef struct __kernel_ucontext {
  unsigned long              uc_flags;
  struct __kernel_ucontext*  uc_link;
  stack_t                    uc_stack;
  struct sigcontext          uc_mcontext;
  __kernel_sigset_t          uc_sigmask;
  /* The kernel reserves 1024 bits for the signal mask, even if it only
   * uses the first 64 ones. */
  int _unused[32 - (sizeof(__kernel_sigset_t) / sizeof(int))];
  /* Used to store coprocessor + VFP register state on ARMv7-A */
  unsigned long              uc_regspace[128] __attribute__((__aligned__(8)));
} __kernel_ucontext_t;

/* Second, the GLibc-compatible version */

/* On ARM, GLibc mcontext_t is a simple typedef to 'struct sigcontext',
 * just like the kernel does. This means that to access register values,
 * you will need to use the specific named field (e.g. 'arm_r0', 'arm_pc').
 *
 * Note that on all other platforms, GLibc uses a gregs[] array instead.
 * Follow the GLibc logic, or lack of, for ease of portability.
 *
 * Curiously, GLibc also defines enums and macros for REG_R0 .. REG_R15,
 * plus the gregset_t type, NREGS macro, fpregset_t types, etc.. while they
 * are never used by any code. They are *not* declared here.
 *
 * This also means there is no 'official' or 'documented' way to access
 * the state of the FP registers (while the GLibc implementation does
 * save/restore them in getcontext()/swapcontext()).
 */
typedef struct sigcontext mcontext_t;

/* Technical note:
 *
 * GLibc defines an ARM-specific 'struct _libc_fpstate' that seems to match
 * the FPU state stored in uc_regspace[] below. However, its declaration does
 * not match either the kernel declaration, or the actual GLibc
 * implementation of *context() functions.
 *
 * It is suspected that this is a remnant of the obsolete VFPv1 era that
 * was never updated. As such, client code shall *not* rely on this and
 * there is no definition for it provided here.
 */

typedef struct ucontext {
  uint32_t          uc_flags;
  struct ucontext*  uc_link;
  stack_t           uc_stack;
  mcontext_t        uc_mcontext;
  /* Only expose the 32 non-realtime signals in Bionic's 32-bit sigset_t
   * The _unused field is required padding from the kernel. */
  sigset_t          uc_sigmask;
  int               _unused[32 - sizeof(sigset_t)/sizeof(int)];
  uint32_t          uc_regspace[128] __attribute__((__aligned__(8)));
} ucontext_t;

__END_DECLS

#endif /* _ARCH_ARM_SYS_UCONTEXT_H_ */
