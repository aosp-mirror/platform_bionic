/*
 * Copyright (C) 2017 The Android Open Source Project
 * All rights reserved.
 *
 * Copyright (c) 2009-2011, Code Aurora Forum. All rights reserved.
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

#define PLDOFFS (16)
#define PLDSIZE (128) /* L2 cache line size */

        .syntax unified

        // To avoid warning about deprecated instructions, add an explicit
        // arch. The code generated is exactly the same.
        .arch armv7-a

        .code 32
ENTRY(__memcpy_kryo)
        push            {r0}
        .cfi_def_cfa_offset 4
        .cfi_rel_offset r0, 0
        cmp             r2, #4
        blt             .Lneon_lt4
        cmp             r2, #16
        blt             .Lneon_lt16
        cmp             r2, #32
        blt             .Lneon_16
        cmp              r2, #128
        blt              .Lneon_copy_32_a
        /* Copy blocks of 128-bytes (word-aligned) at a time*/
        /* Code below is optimized for PLDSIZE=128 only */
        mov             r12, r2, lsr #7
        cmp             r12, #PLDOFFS
        ble             .Lneon_copy_128_loop_nopld
        sub             r12, #PLDOFFS
        pld             [r1, #(PLDOFFS-1)*PLDSIZE]
.Lneon_copy_128_loop_outer:
        pld             [r1, #(PLDOFFS*PLDSIZE)]
        pld             [r1, #(PLDOFFS)*(PLDSIZE)+64]
        vld1.32         {q0, q1}, [r1]!
        vld1.32         {q2, q3}, [r1]!
        vld1.32         {q8, q9}, [r1]!
        vld1.32         {q10, q11}, [r1]!
        subs            r12, r12, #1
        vst1.32         {q0, q1}, [r0]!
        vst1.32         {q2, q3}, [r0]!
        vst1.32         {q8, q9}, [r0]!
        vst1.32         {q10, q11}, [r0]!
        bne             .Lneon_copy_128_loop_outer
        mov             r12, #PLDOFFS
.Lneon_copy_128_loop_nopld:
        vld1.32         {q0, q1}, [r1]!
        vld1.32         {q2, q3}, [r1]!
        vld1.32         {q8, q9}, [r1]!
        vld1.32         {q10, q11}, [r1]!
        subs            r12, r12, #1
        vst1.32         {q0, q1}, [r0]!
        vst1.32         {q2, q3}, [r0]!
        vst1.32         {q8, q9}, [r0]!
        vst1.32         {q10, q11}, [r0]!
        bne             .Lneon_copy_128_loop_nopld
        ands            r2, r2, #0x7f
        beq             .Lneon_exit
        cmp             r2, #32
        blt             .Lneon_16
        nop
        /* Copy blocks of 32-bytes (word aligned) at a time*/
.Lneon_copy_32_a:
        mov             r12, r2, lsr #5
.Lneon_copy_32_loop_a:
        vld1.32         {q0,q1}, [r1]!
        subs            r12, r12, #1
        vst1.32         {q0,q1}, [r0]!
        bne             .Lneon_copy_32_loop_a
        ands            r2, r2, #0x1f
        beq             .Lneon_exit
.Lneon_16:
        subs            r2, r2, #16
        blt             .Lneon_lt16
        vld1.32         {q8}, [r1]!
        vst1.32         {q8}, [r0]!
        beq             .Lneon_exit
.Lneon_lt16:
        movs            r12, r2, lsl #29
        bcc             .Lneon_skip8
        ldr             r3, [r1], #4
        ldr             r12, [r1], #4
        str             r3, [r0], #4
        str             r12, [r0], #4
.Lneon_skip8:
        bpl             .Lneon_lt4
        ldr             r3, [r1], #4
        str             r3, [r0], #4
.Lneon_lt4:
        movs            r2, r2, lsl #31
        bcc             .Lneon_lt2
        ldrh            r3, [r1], #2
        strh            r3, [r0], #2
.Lneon_lt2:
        bpl             .Lneon_exit
        ldrb            r12, [r1]
        strb            r12, [r0]
.Lneon_exit:
        pop             {r0}
        bx              lr

END(__memcpy_kryo)
