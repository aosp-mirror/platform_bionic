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

// DWARF constants.
#define DW_CFA_def_cfa_expression 0x0f
#define DW_CFA_expression 0x10
#define DW_EH_PE_pcrel 0x10
#define DW_EH_PE_sdata4 0x0b
#define DW_OP_breg4 0x74
#define DW_OP_breg7 0x77
#define DW_OP_deref 0x06

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
#define OFFSET_RSP 160
#define OFFSET_RBX 128
#define OFFSET_RDX 136
#define OFFSET_RAX 144
#define OFFSET_RCX 152
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

#define cfi_signal_frame_start(f) \
.section .eh_frame,"a",@progbits; \
.L ## f ## _START_EH_FRAME: \
  .long 2f - 1f; /* CIE length. */ \
1:.long 0;       /* CIE ID. */ \
  .byte 1;       /* Version. */ \
  .string "zRS"; /* Augmentation string. */ \
  .uleb128 1;    /* Code alignment factor. */ \
  .sleb128 -8;   /* Data alignment factor. */ \
  .uleb128 DW_x86_64_RIP;   /* Return address register. */ \
  .uleb128 1;    /* 1 byte of augmentation data. */ \
  .byte (DW_EH_PE_pcrel | DW_EH_PE_sdata4); /* FDE encoding. */ \
  .align 8; \
2: \
  .long .L ## f ## _END_FDE - .L ## f ## _START_FDE;   /* FDE length. */ \
.L ## f ## _START_FDE: \
  .long .L ## f ## _START_FDE - .L ## f ## _START_EH_FRAME; /* CIE location. */ \
  .long (.L ## f ## _START - 1) - .;                   /* pcrel start address (see FDE encoding above). */ \
  .long .L ## f ## _END - (.L ## f ## _START - 1);     /* Function this FDE applies to. */ \
  .uleb128 0;                                          /* FDE augmentation length. */ \

#define cfi_signal_frame_end(f) \
.L ## f ## _END_FDE: \

#define cfi_def_cfa(offset) \
  .byte DW_CFA_def_cfa_expression; \
  .uleb128 2f-1f; \
1:.byte DW_OP_breg7; \
  .sleb128 offset; \
  .byte DW_OP_deref; \
2: \

#define cfi_offset(reg_number,offset) \
  .byte DW_CFA_expression; \
  .uleb128 reg_number; \
  .uleb128 2f-1f; \
1:.byte DW_OP_breg7; \
  .sleb128 offset; \
2: \

ENTRY_PRIVATE(__restore_rt)
.L__restore_rt_START:
  mov $__NR_rt_sigreturn, %rax
  syscall
.L__restore_rt_END:
END(__restore_rt)
cfi_signal_frame_start(__restore_rt)
  cfi_def_cfa(OFFSET_RSP)
  cfi_offset(DW_x86_64_R8, OFFSET_R8)
  cfi_offset(DW_x86_64_R9, OFFSET_R9)
  cfi_offset(DW_x86_64_R10, OFFSET_R10)
  cfi_offset(DW_x86_64_R11, OFFSET_R11)
  cfi_offset(DW_x86_64_R12, OFFSET_R12)
  cfi_offset(DW_x86_64_R13, OFFSET_R13)
  cfi_offset(DW_x86_64_R14, OFFSET_R14)
  cfi_offset(DW_x86_64_R15, OFFSET_R15)
  cfi_offset(DW_x86_64_RDI, OFFSET_RDI)
  cfi_offset(DW_x86_64_RSI, OFFSET_RSI)
  cfi_offset(DW_x86_64_RBP, OFFSET_RBP)
  cfi_offset(DW_x86_64_RSP, OFFSET_RSP)
  cfi_offset(DW_x86_64_RBX, OFFSET_RBX)
  cfi_offset(DW_x86_64_RDX, OFFSET_RDX)
  cfi_offset(DW_x86_64_RAX, OFFSET_RAX)
  cfi_offset(DW_x86_64_RCX, OFFSET_RCX)
  cfi_offset(DW_x86_64_RIP, OFFSET_RIP)
cfi_signal_frame_end(__restore_rt)
