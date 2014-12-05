/*
 * Copyright (C) 2013 The Android Open Source Project
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

#include <private/bionic_asm.h>

// _JBLEN is the size of a jmp_buf in longs(64bit on AArch64).
#define _JBLEN 32

// According to AARCH64 PCS document we need to save the following
// registers:
//
// Core     x19 - x30, sp (see section 5.1.1)
// VFP      d8 - d15 (see section 5.1.2)
//
// NOTE: All the registers saved here will have 64bit vales (except FPSR).
//       AAPCS mandates that the higher part of q registers do not need to
//       be saved by the callee.
//

// The structure of jmp_buf for AArch64:
//
// NOTE: _JBLEN is the size of jmp_buf in longs(64bit on AArch64)! The table
//      below computes the offsets in words(32bit).
//
//  word        name            description
//  0       magic           magic number
//  1       sigmask         signal mask (not used with _setjmp / _longjmp)
//  2       core_base       base of core registers (x19-x30, sp)
//  28      float_base      base of float registers (d8-d15)
//  44      reserved        reserved entries (room to grow)
//  64
//
//
//  NOTE: The instructions that load/store core/vfp registers expect 8-byte
//        alignment. Contrary to the previous setjmp header for ARM we do not
//        need to save status/control registers for VFP (it is not a
//        requirement for setjmp).
//

#define _JB_MAGIC       0
#define _JB_SIGMASK     (_JB_MAGIC+1)
#define _JB_CORE_BASE   (_JB_SIGMASK+1)
#define _JB_FLOAT_BASE  (_JB_CORE_BASE + (31-19+1)*2)

.L_setjmp_magic_signal_mask_n: .word 0x53657200
.L_setjmp_magic_signal_mask_y: .word 0x53657201

ENTRY(setjmp)
  mov w1, #1
  b sigsetjmp
END(setjmp)

ENTRY(_setjmp)
  mov w1, #0
  b sigsetjmp
END(_setjmp)

// int sigsetjmp(sigjmp_buf env, int save_signal_mask);
ENTRY(sigsetjmp)
  // Do we need to save the signal mask?
  ldr w9, .L_setjmp_magic_signal_mask_n
  cbz w1, 1f

  // Get current signal mask.
  stp x0, x30, [sp, #-16]!
  mov x0, xzr
  bl sigblock
  mov w1, w0
  ldp x0, x30, [sp], #16

  // Save signal mask.
  str w1, [x0, #(_JB_SIGMASK * 4)]

  ldr w9, .L_setjmp_magic_signal_mask_y

1:
  // Save magic number.
  str w9, [x0, #(_JB_MAGIC * 4)]

  // Save core registers.
  mov x10, sp
  stp x30, x10, [x0, #(_JB_CORE_BASE * 4 + 16 * 0)]
  stp x28, x29, [x0, #(_JB_CORE_BASE * 4 + 16 * 1)]
  stp x26, x27, [x0, #(_JB_CORE_BASE * 4 + 16 * 2)]
  stp x24, x25, [x0, #(_JB_CORE_BASE * 4 + 16 * 3)]
  stp x22, x23, [x0, #(_JB_CORE_BASE * 4 + 16 * 4)]
  stp x20, x21, [x0, #(_JB_CORE_BASE * 4 + 16 * 5)]
  str x19,      [x0, #(_JB_CORE_BASE * 4 + 16 * 6)]

  // Save floating point registers.
  stp d14, d15, [x0, #(_JB_FLOAT_BASE * 4 + 16 * 0)]
  stp d12, d13, [x0, #(_JB_FLOAT_BASE * 4 + 16 * 1)]
  stp d10, d11, [x0, #(_JB_FLOAT_BASE * 4 + 16 * 2)]
  stp d8,  d9,  [x0, #(_JB_FLOAT_BASE * 4 + 16 * 3)]

  mov w0, wzr
  ret
END(sigsetjmp)

// void siglongjmp(sigjmp_buf env, int value);
ENTRY(siglongjmp)
  // Check magic.
  ldr w10, [x0, #(_JB_MAGIC * 4)]
  ldr w9, .L_setjmp_magic_signal_mask_n
  cmp w9, w10
  b.eq 1f
  ldr w9, .L_setjmp_magic_signal_mask_y
  cmp w9, w10
  b.ne longjmperror

  // Restore signal mask.
  stp x0, x30, [sp, #-16]!
  mov x19, x1
  ldr w0, [x0, #(_JB_SIGMASK * 4)]
  bl sigsetmask
  ldp x0, x30, [sp], #16
  mov x1, x19

1:
  // Restore core registers.
  ldp x30, x10, [x0, #(_JB_CORE_BASE * 4 + 16 * 0)]
  mov sp, x10
  ldp x28, x29, [x0, #(_JB_CORE_BASE * 4 + 16 * 1)]
  ldp x26, x27, [x0, #(_JB_CORE_BASE * 4 + 16 * 2)]
  ldp x24, x25, [x0, #(_JB_CORE_BASE * 4 + 16 * 3)]
  ldp x22, x23, [x0, #(_JB_CORE_BASE * 4 + 16 * 4)]
  ldp x20, x21, [x0, #(_JB_CORE_BASE * 4 + 16 * 5)]
  ldr x19,      [x0, #(_JB_CORE_BASE * 4 + 16 * 6)]

  // Restore floating point registers.
  ldp d14, d15, [x0, #(_JB_FLOAT_BASE * 4 + 16 * 0)]
  ldp d12, d13, [x0, #(_JB_FLOAT_BASE * 4 + 16 * 1)]
  ldp d10, d11, [x0, #(_JB_FLOAT_BASE * 4 + 16 * 2)]
  ldp d8,  d9,  [x0, #(_JB_FLOAT_BASE * 4 + 16 * 3)]

  // Validate sp (sp mod 16 = 0) and lr (lr mod 4 = 0).
  tst x30, #3
  b.ne longjmperror
  mov x10, sp
  tst x10, #15
  b.ne longjmperror

  // Set return value.
  cmp w1, wzr
  csinc w0, w1, wzr, ne
  ret
END(siglongjmp)

  .globl longjmp
  .equ longjmp, siglongjmp
  .globl _longjmp
  .equ _longjmp, siglongjmp
