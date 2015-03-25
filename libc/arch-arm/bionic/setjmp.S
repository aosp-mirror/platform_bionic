/*
 * Copyright (c) 1997 Mark Brinicombe
 * Copyright (c) 2010 Android Open Source Project.
 * All rights reserved.
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
 *	This product includes software developed by Mark Brinicombe
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <private/bionic_asm.h>

// According to the ARM AAPCS document, we only need to save
// the following registers:
//
//  Core   r4-r14
//
//  VFP    d8-d15  (see section 5.1.2.1)
//
//      Registers s16-s31 (d8-d15, q4-q7) must be preserved across subroutine
//      calls; registers s0-s15 (d0-d7, q0-q3) do not need to be preserved
//      (and can be used for passing arguments or returning results in standard
//      procedure-call variants). Registers d16-d31 (q8-q15), if present, do
//      not need to be preserved.
//
//  FPSCR  saved because glibc does.

// The internal structure of a jmp_buf is totally private.
// Current layout (may change in the future):
//
// word   name         description
// 0      magic        magic number
// 1      sigmask      signal mask (not used with _setjmp / _longjmp)
// 2      float_base   base of float registers (d8 to d15)
// 18     float_state  floating-point status and control register
// 19     core_base    base of core registers (r4 to r14)
// 30     reserved     reserved entries (room to grow)
// 64
//
// NOTE: float_base must be at an even word index, since the
//       FP registers will be loaded/stored with instructions
//       that expect 8-byte alignment.

#define _JB_SIGFLAG     0
#define _JB_SIGMASK     (_JB_SIGFLAG+1)
#define _JB_FLOAT_BASE  (_JB_SIGMASK+1)
#define _JB_FLOAT_STATE (_JB_FLOAT_BASE + (15-8+1)*2)
#define _JB_CORE_BASE   (_JB_FLOAT_STATE+1)

ENTRY(setjmp)
  mov r1, #1
  b sigsetjmp
END(setjmp)

ENTRY(_setjmp)
  mov r1, #0
  b sigsetjmp
END(_setjmp)

// int sigsetjmp(sigjmp_buf env, int save_signal_mask);
ENTRY(sigsetjmp)
  // Record whether or not we're saving the signal mask.
  str r1, [r0, #(_JB_SIGFLAG * 4)]

  // Do we need to save the signal mask?
  teq r1, #0
  beq 1f

  // Get current signal mask.
  stmfd sp!, {r0, r14}
  .cfi_def_cfa_offset 8
  .cfi_rel_offset r0, 0
  .cfi_rel_offset r14, 4
  mov r0, #0
  bl sigblock
  mov r1, r0
  ldmfd sp!, {r0, r14}
  .cfi_def_cfa_offset 0

  // Save the signal mask.
  str r1, [r0, #(_JB_SIGMASK * 4)]

1:
  // Save core registers.
  add r1, r0, #(_JB_CORE_BASE * 4)
  stmia r1, {r4-r14}

  // Save floating-point registers.
  add r1, r0, #(_JB_FLOAT_BASE * 4)
  vstmia  r1, {d8-d15}

  // Save floating-point state.
  fmrx r1, fpscr
  str r1, [r0, #(_JB_FLOAT_STATE * 4)]

  mov r0, #0
  bx lr
END(sigsetjmp)

// void siglongjmp(sigjmp_buf env, int value);
ENTRY(siglongjmp)
  // Do we need to restore the signal mask?
  ldr r2, [r0, #(_JB_SIGFLAG * 4)]
  teq r2, #0
  beq 1f

  // Restore the signal mask.
  stmfd sp!, {r0, r1, r14}
  .cfi_def_cfa_offset 12
  .cfi_rel_offset r0, 0
  .cfi_rel_offset r1, 4
  .cfi_rel_offset r14, 8
  sub sp, sp, #4 // Align the stack.
  .cfi_adjust_cfa_offset 4

  ldr r0, [r0, #(_JB_SIGMASK * 4)]
  bl sigsetmask

  add sp, sp, #4 // Unalign the stack.
  .cfi_adjust_cfa_offset -4
  ldmfd sp!, {r0, r1, r14}
  .cfi_def_cfa_offset 0

1:
  // Restore floating-point registers.
  add r2, r0, #(_JB_FLOAT_BASE * 4)
  vldmia r2, {d8-d15}

  // Restore floating-point state.
  ldr r2, [r0, #(_JB_FLOAT_STATE * 4)]
  fmxr fpscr, r2

  // Restore core registers.
  add r2, r0, #(_JB_CORE_BASE * 4)
  ldmia r2, {r4-r14}

  // Validate sp and r14.
  teq sp, #0
  teqne r14, #0
  bleq longjmperror

  // Set return value.
  mov r0, r1
  teq r0, #0
  moveq r0, #1
  bx lr
END(siglongjmp)

ALIAS_SYMBOL(longjmp, siglongjmp)
ALIAS_SYMBOL(_longjmp, siglongjmp)
