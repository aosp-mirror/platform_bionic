/*
 * Copyright (C) 2014 The Android Open Source Project
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
#include <private/bionic_asm_dwarf_exprs.h>

// Offsets into struct sigcontext.
#define OFFSET_EDI 16
#define OFFSET_ESI 20
#define OFFSET_EBP 24
#define OFFSET_ESP 28
#define OFFSET_EBX 32
#define OFFSET_EDX 36
#define OFFSET_ECX 40
#define OFFSET_EAX 44
#define OFFSET_EIP 56

// Non-standard DWARF constants for the x86 registers.
#define DW_x86_REG_EAX 0
#define DW_x86_REG_ECX 1
#define DW_x86_REG_EDX 2
#define DW_x86_REG_EBX 3
#define DW_x86_REG_ESP 4
#define DW_x86_REG_EBP 5
#define DW_x86_REG_ESI 6
#define DW_x86_REG_EDI 7
#define DW_x86_REG_EIP 8

#define RESTORE_GPR(reg, extra_offset)                    \
    m_cfi_breg_offset DW_x86_REG_ ## reg,                 \
                      DW_x86_REG_ESP,                     \
                      (OFFSET_ ## reg + (extra_offset));

// Restoring ESP is unnecessary as the unwinder simply uses the CFA value.
#define RESTORE_GPRS(extra_offset)                                      \
    m_cfi_def_cfa_deref DW_x86_REG_ESP, (OFFSET_ESP + (extra_offset));  \
    RESTORE_GPR(EDI, extra_offset)                                      \
    RESTORE_GPR(ESI, extra_offset)                                      \
    RESTORE_GPR(EBP, extra_offset)                                      \
    RESTORE_GPR(EBX, extra_offset)                                      \
    RESTORE_GPR(EDX, extra_offset)                                      \
    RESTORE_GPR(ECX, extra_offset)                                      \
    RESTORE_GPR(EAX, extra_offset)                                      \
    RESTORE_GPR(EIP, extra_offset)                                      \

  .text

  .cfi_startproc
  .cfi_signal_frame
  RESTORE_GPRS(4)
  nop   // See comment in libc/arch-x86_64/bionic/__restore_rt.S about this nop.
ENTRY_PRIVATE_NO_DWARF(__restore)
  popl %eax
  RESTORE_GPRS(0)
  movl $__NR_sigreturn, %eax
  int $0x80
END(__restore)

  .cfi_startproc
  .cfi_signal_frame
  RESTORE_GPRS(160)
  nop   // See comment in libc/arch-x86_64/bionic/__restore_rt.S about this nop.
ENTRY_PRIVATE_NO_DWARF(__restore_rt)
  movl $__NR_rt_sigreturn, %eax
  int $0x80
END(__restore_rt)
