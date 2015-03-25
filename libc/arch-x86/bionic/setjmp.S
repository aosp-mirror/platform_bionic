/*-
 * Copyright (c) 1990 The Regents of the University of California.
 * All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * William Jolitz.
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

#include <private/bionic_asm.h>

#define _JB_EDX 0
#define _JB_EBX 1
#define _JB_ESP 2
#define _JB_EBP 3
#define _JB_ESI 4
#define _JB_EDI 5
#define _JB_SIGMASK 6
#define _JB_SIGFLAG 7

ENTRY(setjmp)
  movl 4(%esp),%ecx
  movl $1,(_JB_SIGFLAG * 4)(%ecx)
  jmp .L_sigsetjmp_signal_mask
END(setjmp)

ENTRY(_setjmp)
  movl 4(%esp),%ecx
  movl $0,(_JB_SIGFLAG * 4)(%ecx)
  jmp .L_sigsetjmp_no_signal_mask
END(_setjmp)

ENTRY(sigsetjmp)
  movl 4(%esp),%ecx
  movl 8(%esp),%eax

  // Record whether or not the signal mask is valid.
  movl %eax,(_JB_SIGFLAG * 4)(%ecx)

  // Do we need to save the signal mask?
  testl %eax,%eax
  jz 1f

.L_sigsetjmp_signal_mask:
  // Get the current signal mask.
  PIC_PROLOGUE
  pushl $0
  call PIC_PLT(sigblock)
  addl $4,%esp
  PIC_EPILOGUE

  // Save the signal mask.
  movl 4(%esp),%ecx
  movl %eax,(_JB_SIGMASK * 4)(%ecx)

.L_sigsetjmp_no_signal_mask:
1:
  // Save the callee-save registers.
  movl 0(%esp),%edx
  movl %edx,(_JB_EDX * 4)(%ecx)
  movl %ebx,(_JB_EBX * 4)(%ecx)
  movl %esp,(_JB_ESP * 4)(%ecx)
  movl %ebp,(_JB_EBP * 4)(%ecx)
  movl %esi,(_JB_ESI * 4)(%ecx)
  movl %edi,(_JB_EDI * 4)(%ecx)

  xorl %eax,%eax
  ret
END(sigsetjmp)

ENTRY(siglongjmp)
  // Do we have a signal mask to restore?
  movl 4(%esp),%edx
  cmpl $0,(_JB_SIGFLAG * 4)(%edx)
  jz 1f

  // Restore the signal mask.
  PIC_PROLOGUE
  pushl (_JB_SIGMASK * 4)(%edx)
  call PIC_PLT(sigsetmask)
  addl $4,%esp
  PIC_EPILOGUE

1:
  // Restore the callee-save registers.
  movl 4(%esp),%edx
  movl 8(%esp),%eax
  movl (_JB_EDX * 4)(%edx),%ecx
  movl (_JB_EBX * 4)(%edx),%ebx
  movl (_JB_ESP * 4)(%edx),%esp
  movl (_JB_EBP * 4)(%edx),%ebp
  movl (_JB_ESI * 4)(%edx),%esi
  movl (_JB_EDI * 4)(%edx),%edi

  testl %eax,%eax
  jnz 2f
  incl %eax
2:
  movl %ecx,0(%esp)
  ret
END(siglongjmp)

ALIAS_SYMBOL(longjmp, siglongjmp)
ALIAS_SYMBOL(_longjmp, siglongjmp)
