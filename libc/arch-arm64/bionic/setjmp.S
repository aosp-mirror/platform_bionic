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

// According to AARCH64 PCS document we need to save the following
// registers:
//
// Core     x19 - x30, sp (see section 5.1.1)
// VFP      d8 - d15 (see section 5.1.2)
//
// NOTE: All the registers saved here will have 64 bit vales.
//       AAPCS mandates that the higher part of q registers do not need to
//       be saved by the callee.

#define _JB_SIGFLAG     0
#define _JB_SIGMASK     (_JB_SIGFLAG + 1)
#define _JB_X30_SP      (_JB_SIGMASK + 1)
#define _JB_X28_X29     (_JB_X30_SP  + 2)
#define _JB_X26_X27     (_JB_X28_X29 + 2)
#define _JB_X24_X25     (_JB_X26_X27 + 2)
#define _JB_X22_X23     (_JB_X24_X25 + 2)
#define _JB_X20_X21     (_JB_X22_X23 + 2)
#define _JB_X19         (_JB_X20_X21 + 2)
#define _JB_D14_D15     (_JB_X19 + 1)
#define _JB_D12_D13     (_JB_D14_D15 + 2)
#define _JB_D10_D11     (_JB_D12_D13 + 2)
#define _JB_D8_D9       (_JB_D10_D11 + 2)

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
  // Record whether or not we're saving the signal mask.
  str w1, [x0, #(_JB_SIGFLAG * 8)]

  // Do we need to save the signal mask?
  cbz w1, 1f

  // Save current signal mask.
  stp x0, x30, [sp, #-16]!
  // The 'how' argument is ignored if new_mask is NULL.
  mov x1, #0 // NULL.
  add x2, x0, #(_JB_SIGMASK * 8) // old_mask.
  bl sigprocmask
  ldp x0, x30, [sp], #16

1:
  // Save core registers.
  mov x10, sp
  stp x30, x10, [x0, #(_JB_X30_SP  * 8)]
  stp x28, x29, [x0, #(_JB_X28_X29 * 8)]
  stp x26, x27, [x0, #(_JB_X26_X27 * 8)]
  stp x24, x25, [x0, #(_JB_X24_X25 * 8)]
  stp x22, x23, [x0, #(_JB_X22_X23 * 8)]
  stp x20, x21, [x0, #(_JB_X20_X21 * 8)]
  str x19,      [x0, #(_JB_X19     * 8)]

  // Save floating point registers.
  stp d14, d15, [x0, #(_JB_D14_D15 * 8)]
  stp d12, d13, [x0, #(_JB_D12_D13 * 8)]
  stp d10, d11, [x0, #(_JB_D10_D11 * 8)]
  stp d8,  d9,  [x0, #(_JB_D8_D9   * 8)]

  mov w0, #0
  ret
END(sigsetjmp)

// void siglongjmp(sigjmp_buf env, int value);
ENTRY(siglongjmp)
  // Do we need to restore the signal mask?
  ldr w9, [x0, #(_JB_SIGFLAG * 8)]
  cbz w9, 1f

  // Restore signal mask.
  stp x0, x30, [sp, #-16]!
  mov x19, x1 // Save 'value'.
  mov x2, x0
  mov x0, #2 // SIG_SETMASK
  add x1, x2, #(_JB_SIGMASK * 8) // new_mask.
  mov x2, #0 // NULL.
  bl sigprocmask
  mov x1, x19 // Restore 'value'.
  ldp x0, x30, [sp], #16

1:
  // Restore core registers.
  ldp x30, x10, [x0, #(_JB_X30_SP  * 8)]
  mov sp, x10
  ldp x28, x29, [x0, #(_JB_X28_X29 * 8)]
  ldp x26, x27, [x0, #(_JB_X26_X27 * 8)]
  ldp x24, x25, [x0, #(_JB_X24_X25 * 8)]
  ldp x22, x23, [x0, #(_JB_X22_X23 * 8)]
  ldp x20, x21, [x0, #(_JB_X20_X21 * 8)]
  ldr x19,      [x0, #(_JB_X19     * 8)]

  // Restore floating point registers.
  ldp d14, d15, [x0, #(_JB_D14_D15 * 8)]
  ldp d12, d13, [x0, #(_JB_D12_D13 * 8)]
  ldp d10, d11, [x0, #(_JB_D10_D11 * 8)]
  ldp d8,  d9,  [x0, #(_JB_D8_D9   * 8)]

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

ALIAS_SYMBOL(longjmp, siglongjmp)
ALIAS_SYMBOL(_longjmp, siglongjmp)
