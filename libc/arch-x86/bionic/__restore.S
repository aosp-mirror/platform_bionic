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

// DWARF constants.
#define DW_CFA_def_cfa_expression 0x0f
#define DW_CFA_expression 0x10
#define DW_EH_PE_pcrel 0x10
#define DW_EH_PE_sdata4 0x0b
#define DW_OP_breg4 0x74
#define DW_OP_deref 0x06

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
#define DW_x86_REG_EBP 5
#define DW_x86_REG_ESI 6
#define DW_x86_REG_EDI 7
#define DW_x86_REG_EIP 8

#define cfi_signal_frame_start(f) \
.section .eh_frame,"a",@progbits; \
.L ## f ## _START_EH_FRAME: \
  .long 2f - 1f; /* CIE length. */ \
1:.long 0;       /* CIE ID. */ \
  .byte 1;       /* Version. */ \
  .string "zRS"; /* Augmentation string. */ \
  .uleb128 1;    /* Code alignment factor. */ \
  .sleb128 -4;   /* Data alignment factor. */ \
  .uleb128 DW_x86_REG_EIP;    /* Return address register. */ \
  .uleb128 1;    /* 1 byte of augmentation data. */ \
  .byte (DW_EH_PE_pcrel|DW_EH_PE_sdata4); /* FDE encoding. */ \
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
1:.byte DW_OP_breg4; \
  .sleb128 offset; \
  .byte DW_OP_deref; \
2: \

#define cfi_offset(reg_number,offset) \
  .byte DW_CFA_expression; \
  .uleb128 reg_number; \
  .uleb128 2f-1f; \
1:.byte DW_OP_breg4; \
  .sleb128 offset; \
2: \

ENTRY_PRIVATE(__restore)
.L__restore_START:
  popl %eax
  movl $__NR_sigreturn, %eax
  int $0x80
.L__restore_END:
END(__restore)
cfi_signal_frame_start(__restore)
  cfi_def_cfa(OFFSET_ESP + 4)
  cfi_offset(DW_x86_REG_EDI, OFFSET_EDI + 4)
  cfi_offset(DW_x86_REG_ESI, OFFSET_ESI + 4)
  cfi_offset(DW_x86_REG_EBP, OFFSET_EBP + 4)
  cfi_offset(DW_x86_REG_EBX, OFFSET_EBX + 4)
  cfi_offset(DW_x86_REG_EDX, OFFSET_EDX + 4)
  cfi_offset(DW_x86_REG_ECX, OFFSET_ECX + 4)
  cfi_offset(DW_x86_REG_EAX, OFFSET_EAX + 4)
  cfi_offset(DW_x86_REG_EIP, OFFSET_EIP + 4)
cfi_signal_frame_end(__restore)

ENTRY_PRIVATE(__restore_rt)
.L__restore_rt_START:
  movl $__NR_rt_sigreturn, %eax
  int $0x80
.L__restore_rt_END:
END(__restore_rt)
cfi_signal_frame_start(__restore_rt)
  cfi_def_cfa(OFFSET_ESP + 160)
  cfi_offset(DW_x86_REG_EDI, OFFSET_EDI + 160)
  cfi_offset(DW_x86_REG_ESI, OFFSET_ESI + 160)
  cfi_offset(DW_x86_REG_EBP, OFFSET_EBP + 160)
  cfi_offset(DW_x86_REG_EBX, OFFSET_EBX + 160)
  cfi_offset(DW_x86_REG_EDX, OFFSET_EDX + 160)
  cfi_offset(DW_x86_REG_ECX, OFFSET_ECX + 160)
  cfi_offset(DW_x86_REG_EAX, OFFSET_EAX + 160)
  cfi_offset(DW_x86_REG_EIP, OFFSET_EIP + 160)
cfi_signal_frame_end(__restore_rt)
