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
#include <private/bionic_constants.h>

// The internal structure of a jmp_buf is totally private.
// Current layout (changes from release to release):
//
// word   name            description
// 0      sigflag/cookie  setjmp cookie in top 31 bits, signal mask flag in low bit
// 1      sigmask         64-bit signal mask
// 2      ra
// 3      sp
// 4      gp
// 5      s0
// ......
// 16     s11
// 17     fs0
// ......
// 28     fs11
// 29     checksum
// _JBLEN: defined in bionic/libc/include/setjmp.h

#define _JB_SIGFLAG   0
#define _JB_SIGMASK   1 * 8
#define _JB_RA        2 * 8
#define _JB_SP        3 * 8
#define _JB_GP        4 * 8
#define _JB_S0        5 * 8
#define _JB_S1        6 * 8
#define _JB_S2        7 * 8
#define _JB_S3        8 * 8
#define _JB_S4        9 * 8
#define _JB_S5       10 * 8
#define _JB_S6       11 * 8
#define _JB_S7       12 * 8
#define _JB_S8       13 * 8
#define _JB_S9       14 * 8
#define _JB_S10      15 * 8
#define _JB_S11      16 * 8
#define _JB_FS0      17 * 8
#define _JB_FS1      18 * 8
#define _JB_FS2      19 * 8
#define _JB_FS3      20 * 8
#define _JB_FS4      21 * 8
#define _JB_FS5      22 * 8
#define _JB_FS6      23 * 8
#define _JB_FS7      24 * 8
#define _JB_FS8      25 * 8
#define _JB_FS9      26 * 8
#define _JB_FS10     27 * 8
#define _JB_FS11     28 * 8
#define _JB_CHECKSUM 29 * 8

.macro m_mangle_registers reg, sp_reg
  xor s0, s0, \reg
  xor s1, s1, \reg
  xor s2, s2, \reg
  xor s3, s3, \reg
  xor s4, s4, \reg
  xor s5, s5, \reg
  xor s6, s6, \reg
  xor s7, s7, \reg
  xor s8, s8, \reg
  xor s9, s9, \reg
  xor s10, s10, \reg
  xor s11, s11, \reg
  xor a4, a4, \reg  // a4 is the masked gp (x3) for SCS.
  xor \sp_reg, \sp_reg, \reg
.endm

.macro m_calculate_checksum dst, src, scratch
  li \dst, 0
  .irp i,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28
    ld \scratch, (\i * 8)(\src)
    xor \dst, \dst, \scratch
  .endr
.endm

.macro m_unmangle_registers reg, sp_reg
  m_mangle_registers \reg, sp_reg=\sp_reg
.endm

ENTRY_WEAK_FOR_NATIVE_BRIDGE(setjmp)
  li a1, 1
  tail sigsetjmp
END(setjmp)

ENTRY_WEAK_FOR_NATIVE_BRIDGE(_setjmp)
  li a1, 0
  tail sigsetjmp
END(_setjmp)

// int sigsetjmp(sigjmp_buf env, int save_signal_mask);
ENTRY_WEAK_FOR_NATIVE_BRIDGE(sigsetjmp)
  addi sp, sp, -24
  sd a0, 0(sp)
  sd a1, 8(sp)
  sd ra, 16(sp)

  // Get the cookie and store it along with the signal flag.
  mv a0, a1
  call __bionic_setjmp_cookie_get
  mv a1, a0
  ld a0, 0(sp)
  sd a1, _JB_SIGFLAG(a0)

  // Do we need to save the signal mask?
  andi a1, a1, 1
  beqz a1, 1f

  // Save current signal mask.
  // The 'how'/a0 argument is ignored if set is NULL.
  li a1, 0 // NULL
  addi a2, a0, _JB_SIGMASK // old_mask.
  call sigprocmask

1:
  // Restore original a0/ra.
  ld a0, 0(sp)
  ld ra, 16(sp)
  addi sp, sp, 24

  // Get the cookie.
  ld a1, _JB_SIGFLAG(a0)
  andi a1, a1, -2

  // Mask off the high bits of the shadow call stack pointer.
  // We only store the low bits of gp to avoid leaking the
  // shadow call stack address into memory.
  // See the SCS commentary in pthread_internal.h for more detail.
  li a4, SCS_MASK
  and a4, a4, gp

  // Save core registers.
  mv a2, sp
  m_mangle_registers a1, sp_reg=a2
  sd ra,  _JB_RA(a0)
  sd a4,  _JB_GP(a0)  // a4 is the masked gp (x3) for SCS.
  sd a2,  _JB_SP(a0)
  sd s0,  _JB_S0(a0)
  sd s1,  _JB_S1(a0)
  sd s2,  _JB_S2(a0)
  sd s3,  _JB_S3(a0)
  sd s4,  _JB_S4(a0)
  sd s5,  _JB_S5(a0)
  sd s6,  _JB_S6(a0)
  sd s7,  _JB_S7(a0)
  sd s8,  _JB_S8(a0)
  sd s9,  _JB_S9(a0)
  sd s10, _JB_S10(a0)
  sd s11, _JB_S11(a0)
  m_unmangle_registers a1, sp_reg=a2

  // Save floating point registers.
  fsd fs0,  _JB_FS0(a0)
  fsd fs1,  _JB_FS1(a0)
  fsd fs2,  _JB_FS2(a0)
  fsd fs3,  _JB_FS3(a0)
  fsd fs4,  _JB_FS4(a0)
  fsd fs5,  _JB_FS5(a0)
  fsd fs6,  _JB_FS6(a0)
  fsd fs7,  _JB_FS7(a0)
  fsd fs8,  _JB_FS8(a0)
  fsd fs9,  _JB_FS9(a0)
  fsd fs10, _JB_FS10(a0)
  fsd fs11, _JB_FS11(a0)

  // Calculate the checksum and save it.
  m_calculate_checksum t0, a0, t1
  sd t0, _JB_CHECKSUM(a0)

  li a0, 0
  ret
END(sigsetjmp)

// void siglongjmp(sigjmp_buf env, int value);
ENTRY_WEAK_FOR_NATIVE_BRIDGE(siglongjmp)
  // Check the checksum before doing anything.
  m_calculate_checksum t0, a0, t1
  ld t1, _JB_CHECKSUM(a0)
  bne t0, t1, L(checksum_failure)

  // Do we need to restore the signal mask?
  ld a2, _JB_SIGFLAG(a0)
  andi a3, a2, 1
  beqz a3, 1f

  addi sp, sp, -16
  sd a0, 0(sp)
  sd ra, 8(sp)

  // Restore the signal mask.
  mv t0, a1 // Save 'value'.

  mv a2, a0
  li a0, 2 // SIG_SETMASK
  addi a1, a2, _JB_SIGMASK // new_mask
  li a2, 0 // NULL
  call sigprocmask
  mv a1, t0 // Restore 'value'.

  // Restore original a0 and ra.
  ld a0, 0(sp)
  ld ra, 8(sp)
  addi sp, sp, 16

  ld a2, _JB_SIGFLAG(a0)
1:
  // Restore core registers.
  andi a2, a2, -2
  ld ra,  _JB_RA(a0)
  ld a4,  _JB_GP(a0)  // Don't clobber the upper bits of gp (x3) used for SCS yet.
  ld s0,  _JB_S0(a0)
  ld s1,  _JB_S1(a0)
  ld s2,  _JB_S2(a0)
  ld s3,  _JB_S3(a0)
  ld s4,  _JB_S4(a0)
  ld s5,  _JB_S5(a0)
  ld s6,  _JB_S6(a0)
  ld s7,  _JB_S7(a0)
  ld s8,  _JB_S8(a0)
  ld s9,  _JB_S9(a0)
  ld s10, _JB_S10(a0)
  ld s11, _JB_S11(a0)
  ld a3,  _JB_SP(a0)
  m_unmangle_registers a2, sp_reg=a3
  mv sp, a3

  // Restore the low bits of the shadow call stack pointer.
  li a5, ~SCS_MASK
  and gp, gp, a5
  or gp, gp, a4

  addi sp, sp, -24
  sd   ra, 0(sp)
  sd   a0, 8(sp)
  sd   a1, 16(sp)
  ld   a0, _JB_SIGFLAG(a0)
  call __bionic_setjmp_cookie_check
  ld   ra, 0(sp)
  ld   a0, 8(sp)
  ld   a1, 16(sp)
  addi sp, sp, 24

  // Restore floating point registers.
  fld fs0,  _JB_FS0(a0)
  fld fs1,  _JB_FS1(a0)
  fld fs2,  _JB_FS2(a0)
  fld fs3,  _JB_FS3(a0)
  fld fs4,  _JB_FS4(a0)
  fld fs5,  _JB_FS5(a0)
  fld fs6,  _JB_FS6(a0)
  fld fs7,  _JB_FS7(a0)
  fld fs8,  _JB_FS8(a0)
  fld fs9,  _JB_FS9(a0)
  fld fs10, _JB_FS10(a0)
  fld fs11, _JB_FS11(a0)

  // Set return value.
  beqz a1, 2f
  li a0, 1
2:
  mv a0, a1
  ret

L(checksum_failure):
  call __bionic_setjmp_checksum_mismatch
END(siglongjmp)

ALIAS_SYMBOL_WEAK_FOR_NATIVE_BRIDGE(longjmp, siglongjmp)
ALIAS_SYMBOL_WEAK_FOR_NATIVE_BRIDGE(_longjmp, siglongjmp)
