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


// The internal structure of a jmp_buf is totally private.
// Current layout (changes from release to release):
//
// word   name            description
// 0      rbx             registers
// 1      rbp
// 2      r12
// 3      r13
// 4      r14
// 5      r15
// 6      rsp
// 7      pc
// 8      sigflag/cookie  setjmp cookie in top 31 bits, signal mask flag in low bit
// 9      sigmask         signal mask (includes rt signals as well)
// 10     checksum        checksum of the core registers, to give better error messages.

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
#define _JB_CHECKSUM 10

#define MANGLE_REGISTERS 1

.macro m_mangle_registers reg
#if MANGLE_REGISTERS
  xorq \reg,%rbx
  xorq \reg,%rbp
  xorq \reg,%r12
  xorq \reg,%r13
  xorq \reg,%r14
  xorq \reg,%r15
  xorq \reg,%rsp
  xorq \reg,%r11
#endif
.endm

.macro m_unmangle_registers reg
  m_mangle_registers \reg
.endm

.macro m_calculate_checksum dst, src
  movq $0, \dst
  .irp i,0,1,2,3,4,5,6,7
    xorq (\i*8)(\src), \dst
  .endr
.endm

ENTRY(setjmp)
__BIONIC_WEAK_ASM_FOR_NATIVE_BRIDGE(setjmp)
  movl $1,%esi
  jmp PIC_PLT(sigsetjmp)
END(setjmp)

ENTRY(_setjmp)
__BIONIC_WEAK_ASM_FOR_NATIVE_BRIDGE(_setjmp)
  movl $0,%esi
  jmp PIC_PLT(sigsetjmp)
END(_setjmp)

// int sigsetjmp(sigjmp_buf env, int save_signal_mask);
ENTRY(sigsetjmp)
__BIONIC_WEAK_ASM_FOR_NATIVE_BRIDGE(sigsetjmp)
  pushq %rdi
  movq %rsi,%rdi
  call PIC_PLT(__bionic_setjmp_cookie_get)
  popq %rdi

  // Record setjmp cookie and whether or not we're saving the signal mask.
  movq %rax,(_JB_SIGFLAG * 8)(%rdi)
  pushq %rax

  // Do we need to save the signal mask?
  testq $1,%rax
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
  popq %rax
  andq $-2,%rax
  movq (%rsp),%r11
  m_mangle_registers %rax
  movq %rbx,(_JB_RBX * 8)(%rdi)
  movq %rbp,(_JB_RBP * 8)(%rdi)
  movq %r12,(_JB_R12 * 8)(%rdi)
  movq %r13,(_JB_R13 * 8)(%rdi)
  movq %r14,(_JB_R14 * 8)(%rdi)
  movq %r15,(_JB_R15 * 8)(%rdi)
  movq %rsp,(_JB_RSP * 8)(%rdi)
  movq %r11,(_JB_PC  * 8)(%rdi)
  m_unmangle_registers %rax

  m_calculate_checksum %rax, %rdi
  movq %rax, (_JB_CHECKSUM * 8)(%rdi)

  xorl %eax,%eax
  ret
END(sigsetjmp)

// void siglongjmp(sigjmp_buf env, int value);
ENTRY(siglongjmp)
__BIONIC_WEAK_ASM_FOR_NATIVE_BRIDGE(siglongjmp)
  movq %rdi,%r12
  pushq %rsi // Push 'value'.

  m_calculate_checksum %rax, %rdi
  xorq (_JB_CHECKSUM * 8)(%rdi), %rax
  jnz 3f

  // Do we need to restore the signal mask?
  movq (_JB_SIGFLAG * 8)(%rdi), %rdi
  pushq %rdi // Push cookie
  testq $1, %rdi
  jz 2f

  // Restore the signal mask.
  movq $2,%rdi // SIG_SETMASK.
  leaq (_JB_SIGMASK * 8)(%r12),%rsi // new_mask.
  xorq %rdx,%rdx // NULL.
  call PIC_PLT(sigprocmask)

2:
  // Fetch the setjmp cookie and clear the signal flag bit.
  popq %rcx
  andq $-2, %rcx

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
  m_unmangle_registers %rcx

  // Check the cookie.
  pushq %rax
  pushq %r11
  movq %rcx, %rdi
  call PIC_PLT(__bionic_setjmp_cookie_check)
  popq %r11
  popq %rax

  // Return 1 if value is 0.
  testl %eax,%eax
  jnz 1f
  incl %eax
1:
  movq %r11,0(%rsp)
  ret

3:
  call PIC_PLT(__bionic_setjmp_checksum_mismatch)
END(siglongjmp)

ALIAS_SYMBOL(longjmp, siglongjmp)
__BIONIC_WEAK_ASM_FOR_NATIVE_BRIDGE(longjmp)
ALIAS_SYMBOL(_longjmp, siglongjmp)
__BIONIC_WEAK_ASM_FOR_NATIVE_BRIDGE(_longjmp)
