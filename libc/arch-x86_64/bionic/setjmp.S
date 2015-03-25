/*
 * Copyright (c) 2001 Wasabi Systems, Inc.
 * All rights reserved.
 *
 * Written by Frank van der Linden for Wasabi Systems, Inc.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *      This product includes software developed for the NetBSD Project by
 *      Wasabi Systems, Inc.
 * 4. The name of Wasabi Systems, Inc. may not be used to endorse
 *    or promote products derived from this software without specific prior
 *    written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY WASABI SYSTEMS, INC. ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL WASABI SYSTEMS, INC
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <private/bionic_asm.h>

// These are only the callee-saved registers. Code calling setjmp
// will expect the rest to be clobbered anyway.

#define _JB_RBX 0
#define _JB_RBP 1
#define _JB_R12 2
#define _JB_R13 3
#define _JB_R14 4
#define _JB_R15 5
#define _JB_RSP 6
#define _JB_PC 7
#define _JB_SIGFLAG 8
#define _JB_SIGMASK 9
#define _JB_SIGMASK_RT 10 // sigprocmask will write here too.

ENTRY(setjmp)
  movl $1,%esi
  jmp PIC_PLT(sigsetjmp)
END(setjmp)

ENTRY(_setjmp)
  movl $0,%esi
  jmp PIC_PLT(sigsetjmp)
END(_setjmp)

// int sigsetjmp(sigjmp_buf env, int save_signal_mask);
ENTRY(sigsetjmp)
  // Record whether or not we're saving the signal mask.
  movl %esi,(_JB_SIGFLAG * 8)(%rdi)

  // Do we need to save the signal mask?
  testl %esi,%esi
  jz 2f

  // Save current signal mask.
  pushq %rdi // Push 'env'.
  // The 'how' argument is ignored if new_mask is NULL.
  xorq %rsi,%rsi // NULL.
  leaq (_JB_SIGMASK * 8)(%rdi),%rdx // old_mask.
  call PIC_PLT(sigprocmask)
  popq %rdi // Pop 'env'.

2:
  // Save the callee-save registers.
  movq (%rsp),%r11
  movq %rbx,(_JB_RBX * 8)(%rdi)
  movq %rbp,(_JB_RBP * 8)(%rdi)
  movq %r12,(_JB_R12 * 8)(%rdi)
  movq %r13,(_JB_R13 * 8)(%rdi)
  movq %r14,(_JB_R14 * 8)(%rdi)
  movq %r15,(_JB_R15 * 8)(%rdi)
  movq %rsp,(_JB_RSP * 8)(%rdi)
  movq %r11,(_JB_PC  * 8)(%rdi)

  xorl %eax,%eax
  ret
END(sigsetjmp)

// void siglongjmp(sigjmp_buf env, int value);
ENTRY(siglongjmp)
  movq %rdi,%r12
  pushq %rsi // Push 'value'.

  // Do we need to restore the signal mask?
  cmpl $0,(_JB_SIGFLAG * 8)(%rdi)
  jz 2f

  // Restore the signal mask.
  movq $2,%rdi // SIG_SETMASK.
  leaq (_JB_SIGMASK * 8)(%r12),%rsi // new_mask.
  xorq %rdx,%rdx // NULL.
  call PIC_PLT(sigprocmask)

2:
  popq %rax // Pop 'value'.

  // Restore the callee-save registers.
  movq (_JB_RBX * 8)(%r12),%rbx
  movq (_JB_RBP * 8)(%r12),%rbp
  movq (_JB_R13 * 8)(%r12),%r13
  movq (_JB_R14 * 8)(%r12),%r14
  movq (_JB_R15 * 8)(%r12),%r15
  movq (_JB_RSP * 8)(%r12),%rsp
  movq (_JB_PC  * 8)(%r12),%r11
  movq (_JB_R12 * 8)(%r12),%r12

  testl %eax,%eax
  jnz 1f
  incl %eax
1:
  movq %r11,0(%rsp)
  ret
END(siglongjmp)

ALIAS_SYMBOL(longjmp, siglongjmp)
ALIAS_SYMBOL(_longjmp, siglongjmp)
