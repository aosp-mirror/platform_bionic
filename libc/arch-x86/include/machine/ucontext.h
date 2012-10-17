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
#ifndef _ARCH_X86_SYS_UCONTEXT_H_
#define _ARCH_X86_SYS_UCONTEXT_H_

#include <signal.h>
#include <stdint.h>

__BEGIN_DECLS

/* Technical note:
 *
 * On x86, there are differences in the way GLibc and the Linux kernel declare
 * ucontext_t. Here are the details (they differ from ARM significantly):
 *
 *  - The kernel implements 'uc_mcontext' with a 'struct sigcontext',
 *    while GLibc defines 'mcontext_t' in a binary-compatible way
 *    (same size, same binary layout), but with different field naming/access
 *    conventions.
 *
 *    The biggest difference is that 'struct sigcontext' requires named fields
 *    like "eax", or "ebp" to access the state of register values, while GLibc
 *    provides an array (gregs[]) of 32-bit values, that can be accessed
 *    through indexing, plus some enum/macros like REG_EAX or REG_ESP to get
 *    to specific register values.
 *
 *    For maximum portability of existing client code, this header follows
 *    the GLibc convention. Client code, and the C library, should never
 *    try to include <asm/ucontext.h>
 *
 *  - The floating-point state is not stored in the mcontext_t. For historical
 *    reasons, the 'fpregs' field of 'mcontext_t' is really a pointer to a
 *    different block of memory. More specifically:
 *
 *    * When passed to a signal handler, the pointer points to some memory
 *      on the signal stack, populated by the kernel before calling the
 *      handler in user space.
 *
 *    * In GLibc's getcontext()/setcontext() implementation, the pointer will
 *      point to the ucontext_t structure (more specifically the __fpregs_mem
 *      field after uc_sigmask).
 *
 *    * Note that in both cases, the pointer can be NULL if no floating-point
 *      operation has been performed on the current thread yet.
 *
 * - The 'struct _libc_fpstate' structure declared by GLibc corresponds to
 *   both the FP state managed by getcontext()/setcontext(), and the start
 *   of the FP state stored by the kernel and passed to a signal handler.
 *
 *   This matches the data saved by the FNSTENV IA-32 instruction.
 *
 *   IMPORTANT: GLibc does not save the MMX/SSE state, i.e. it does not use
 *              FXSAVE / FXRSTOR instructions in *context() functions.
 *
 *   It is used by client code to retrieve the state of the FPU registers
 *   (e.g. Google Breakpad does that).
 *
 *  Reference source files:
 *    $KERNEL/arch/x86/include/asm/sigcontext_32.h
 *    $KERNEL/arch/x86/include/asm/sigcontext.h
 *    $KERNEL/arch/x86/include/asm/ucontext.h
 *
 *    $GLIBC/sysdeps/unix/sysv/linux/i386/getcontext.S
 *    $GLIBC/sysdeps/unix/sysv/linux/i386/setcontext.S
 *    $GLIBC/sysdeps/unix/sysv/linux/x86/sys/ucontext.h
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

#define NGREG   19
typedef int     greg_t;
typedef greg_t  gregset_t[NGREG];

enum {
  REG_GS = 0, REG_FS, REG_ES, REG_DS,
  REG_EDI, REG_ESI, REG_EBP, REG_ESP,
  REG_EBX, REG_EDX, REG_ECX, REG_EAX,
  REG_TRAPNO, REG_ERR, REG_EIP, REG_CS,
  REG_EFL, REG_UESP, REG_SS
};

/* GLibc defines both macros and enums. Probably to let client do
 *  #ifdef REG_XXXX ... #endif. Do the same here. */
#define REG_GS     REG_GS
#define REG_FS     REG_FS
#define REG_ES     REG_ES
#define REG_DS     REG_DS

#define REG_EDI    REG_EDI
#define REG_ESI    REG_ESI
#define REG_EBP    REG_EBP
#define REG_ESP    REG_ESP

#define REG_EBX    REG_EBX
#define REG_EDX    REG_EDX
#define REG_ECX    REG_ECX
#define REG_EAX    REG_EAX

#define REG_TRAPNO REG_TRAPNO
#define REG_ERR    REG_ERR
#define REG_EIP    REG_EIP
#define REG_CS     REG_CS
#define REG_EFL    REG_EFL
#define REG_UESP   REG_UESP
#define REG_SS     REG_SS

/* 80-bit floating-point register */
struct _libc_fpreg {
  unsigned short significand[4];
  unsigned short exponent;
};

/* Simple floating-point state, see FNSTENV instruction */
struct _libc_fpstate {
  unsigned long cw;
  unsigned long sw;
  unsigned long tag;
  unsigned long ipoff;
  unsigned long cssel;
  unsigned long dataoff;
  unsigned long datasel;
  struct _libc_fpreg _st[8];
  unsigned long status;
};

typedef struct _libc_fpstate* fpregset_t;

typedef struct {
  gregset_t   gregs;
  fpregset_t  fpregs;   /* Really a pointer to _libc_fpstate! */
} mcontext_t;

typedef struct ucontext {
  uint32_t          uc_flags;
  struct ucontext*  uc_link;
  stack_t           uc_stack;
  mcontext_t        uc_mcontext;
  /* Only expose the 32 non-realtime signals in Bionic's 32-bit sigset_t
   * The _unused field is required padding from the kernel. */
  sigset_t          uc_sigmask;
  char _unused[sizeof(__kernel_sigset_t) - sizeof(sigset_t)];
  /* Storage area for *context() functions only - private, don't use. */
  struct _libc_fpstate __fpregs_mem;
  /* Growth opportunity - in case we want to save MMX/SSE stuff there too */
  int _unused2[128 - sizeof(struct _libc_fpstate)/sizeof(int)];
} ucontext_t;

__END_DECLS

#endif /* _ARCH_X86_SYS_UCONTEXT_H_ */
