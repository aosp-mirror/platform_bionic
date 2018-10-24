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

        /*
         * Optimized memset() for ARM.
         *
         * memset() returns its first argument.
         */

        .fpu        neon
        .syntax     unified

        // To avoid warning about deprecated instructions, add an explicit
        // arch. The code generated is exactly the same.
        .arch armv7-a

ENTRY(__memset_chk_a15)
        cmp         r2, r3
        bls         memset

        // Preserve lr for backtrace.
        push        {lr}
        .cfi_def_cfa_offset 4
        .cfi_rel_offset lr, 0

        bl          __memset_chk_fail
END(__memset_chk_a15)

ENTRY(memset_a15)
        stmfd       sp!, {r0}
        .cfi_def_cfa_offset 4
        .cfi_rel_offset r0, 0

        // The new algorithm is slower for copies < 16 so use the old
        // neon code in that case.
        cmp         r2, #16
        blo         .L_set_less_than_16_unknown_align

        // Use strd which requires an even and odd register so move the
        // values so that:
        //   r0 and r1 contain the memset value
        //   r2 is the number of bytes to set
        //   r3 is the destination pointer
        mov         r3, r0

        // Copy the byte value in every byte of r1.
        mov         r1, r1, lsl #24
        orr         r1, r1, r1, lsr #8
        orr         r1, r1, r1, lsr #16

.L_check_alignment:
        // Align destination to a double word to avoid the strd crossing
        // a cache line boundary.
        ands        ip, r3, #7
        bne         .L_do_double_word_align

.L_double_word_aligned:
        mov         r0, r1

        subs        r2, #64
        blo         .L_set_less_than_64

1:      // Main loop sets 64 bytes at a time.
        .irp        offset, #0, #8, #16, #24, #32, #40, #48, #56
        strd        r0, r1, [r3, \offset]
        .endr

        add         r3, #64
        subs        r2, #64
        bge         1b

.L_set_less_than_64:
        // Restore r2 to the count of bytes left to set.
        add         r2, #64
        lsls        ip, r2, #27
        bcc         .L_set_less_than_32
        // Set 32 bytes.
        .irp        offset, #0, #8, #16, #24
        strd        r0, r1, [r3, \offset]
        .endr
        add         r3, #32

.L_set_less_than_32:
        bpl         .L_set_less_than_16
        // Set 16 bytes.
        .irp        offset, #0, #8
        strd        r0, r1, [r3, \offset]
        .endr
        add         r3, #16

.L_set_less_than_16:
        // Less than 16 bytes to set.
        lsls        ip, r2, #29
        bcc         .L_set_less_than_8

        // Set 8 bytes.
        strd        r0, r1, [r3], #8

.L_set_less_than_8:
        bpl         .L_set_less_than_4
        // Set 4 bytes
        str         r1, [r3], #4

.L_set_less_than_4:
        lsls        ip, r2, #31
        it          ne
        strbne      r1, [r3], #1
        itt         cs
        strbcs      r1, [r3], #1
        strbcs      r1, [r3]

        ldmfd       sp!, {r0}
        bx          lr

.L_do_double_word_align:
        rsb         ip, ip, #8
        sub         r2, r2, ip
        movs        r0, ip, lsl #31
        it          mi
        strbmi      r1, [r3], #1
        itt         cs
        strbcs      r1, [r3], #1
        strbcs      r1, [r3], #1

        // Dst is at least word aligned by this point.
        cmp         ip, #4
        blo         .L_double_word_aligned
        str         r1, [r3], #4
        b           .L_double_word_aligned

.L_set_less_than_16_unknown_align:
        // Set up to 15 bytes.
        vdup.8      d0, r1
        movs        ip, r2, lsl #29
        bcc         1f
        vst1.8      {d0}, [r0]!
1:      bge         2f
        vst1.32     {d0[0]}, [r0]!
2:      movs        ip, r2, lsl #31
        it          mi
        strbmi      r1, [r0], #1
        itt         cs
        strbcs      r1, [r0], #1
        strbcs      r1, [r0], #1
        ldmfd       sp!, {r0}
        bx          lr
END(memset_a15)
