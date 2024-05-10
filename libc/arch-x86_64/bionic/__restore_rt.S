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
#include <private/bionic_asm_dwarf_exprs.h>

// In the signal trampoline frame, rsp points to a ucontext_t struct.

// Offsets into struct ucontext_t of uc_mcontext.gregs[x].
#define OFFSET_R8 40
#define OFFSET_R9 48
#define OFFSET_R10 56
#define OFFSET_R11 64
#define OFFSET_R12 72
#define OFFSET_R13 80
#define OFFSET_R14 88
#define OFFSET_R15 96
#define OFFSET_RDI 104
#define OFFSET_RSI 112
#define OFFSET_RBP 120
#define OFFSET_RBX 128
#define OFFSET_RDX 136
#define OFFSET_RAX 144
#define OFFSET_RCX 152
#define OFFSET_RSP 160
#define OFFSET_RIP 168

// Non-standard DWARF constants for the x86-64 registers.
#define DW_x86_64_RAX 0
#define DW_x86_64_RDX 1
#define DW_x86_64_RCX 2
#define DW_x86_64_RBX 3
#define DW_x86_64_RSI 4
#define DW_x86_64_RDI 5
#define DW_x86_64_RBP 6
#define DW_x86_64_RSP 7
#define DW_x86_64_R8 8
#define DW_x86_64_R9 9
#define DW_x86_64_R10 10
#define DW_x86_64_R11 11
#define DW_x86_64_R12 12
#define DW_x86_64_R13 13
#define DW_x86_64_R14 14
#define DW_x86_64_R15 15
#define DW_x86_64_RIP 16

// Insert a nop between .cfi_startproc and the trampoline symbol so that unwinders can find the FDE.
// A function's last instruction can be a call instruction (e.g. to __cxa_throw), in which case the
// return address (e.g. from __cxa_throw to the caller) will be just after the function. This
// address may also be the start of the next function, so to avoid ambiguity, unwinders assume that
// a return address PC can refer to the address just after a function, but never to the start of a
// function. (This is implemented by subtracting 1 from the return address PC before looking it up.)
// This is fine for ordinary functions, but breaks on trampolines. Inserting a nop fixes it.
//
// N.B. Unwinders have two other strategies for recognizing the signal trampoline:
//  - Read the instructions that the return address PC points at and look for a sigreturn syscall.
//    (Hence, the instructions must not change at all.)
//  - Do a symbol table lookup and check that against the PC (e.g. LLDB looks for
//    __kernel_rt_sigreturn and __restore_rt.)
// Either way, the nop is needed to avoid ambiguity if the function before the trampoline could end
// with a call.

#define RESTORE_GPR(reg) m_cfi_breg_offset DW_x86_64_ ## reg, DW_x86_64_RSP, OFFSET_ ## reg;

  .text
  .cfi_startproc
  .cfi_signal_frame
  m_cfi_def_cfa_deref DW_x86_64_RSP, OFFSET_RSP
  RESTORE_GPR(R8)
  RESTORE_GPR(R9)
  RESTORE_GPR(R10)
  RESTORE_GPR(R11)
  RESTORE_GPR(R12)
  RESTORE_GPR(R13)
  RESTORE_GPR(R14)
  RESTORE_GPR(R15)
  RESTORE_GPR(RDI)
  RESTORE_GPR(RSI)
  RESTORE_GPR(RBP)
  RESTORE_GPR(RBX)
  RESTORE_GPR(RDX)
  RESTORE_GPR(RAX)
  RESTORE_GPR(RCX)
  // Restoring RSP is unnecessary as the unwinder simply uses the CFA value.
  RESTORE_GPR(RIP)
  nop
ENTRY_NO_DWARF_PRIVATE(__restore_rt)
  mov $__NR_rt_sigreturn, %rax
  syscall
END(__restore_rt)
