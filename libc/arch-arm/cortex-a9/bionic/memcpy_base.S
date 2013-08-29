/*
 * Copyright (C) 2008 The Android Open Source Project
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

/*
 * This code assumes it is running on a processor that supports all arm v7
 * instructions, that supports neon instructions, and that has a 32 byte
 * cache line.
 */

ENTRY(MEMCPY_BASE)
        .cfi_startproc
        .save       {r0, lr}
        .cfi_def_cfa_offset 8
        .cfi_rel_offset r0, 0
        .cfi_rel_offset lr, 4

        // Check so divider is at least 16 bytes, needed for alignment code.
        cmp         r2, #16
        blo         5f

        /* check if buffers are aligned. If so, run arm-only version */
        eor         r3, r0, r1
        ands        r3, r3, #0x3
        beq         __memcpy_base_aligned

        /* Check the upper size limit for Neon unaligned memory access in memcpy */
        cmp         r2, #224
        blo         3f

        /* align destination to 16 bytes for the write-buffer */
        rsb         r3, r0, #0
        ands        r3, r3, #0xF
        beq         3f

        /* copy up to 15-bytes (count in r3) */
        sub         r2, r2, r3
        movs        ip, r3, lsl #31
        itt         mi
        ldrbmi      lr, [r1], #1
        strbmi      lr, [r0], #1
        itttt       cs
        ldrbcs      ip, [r1], #1
        ldrbcs      lr, [r1], #1
        strbcs      ip, [r0], #1
        strbcs      lr, [r0], #1
        movs        ip, r3, lsl #29
        bge         1f
        // copies 4 bytes, destination 32-bits aligned
        vld1.32     {d0[0]}, [r1]!
        vst1.32     {d0[0]}, [r0, :32]!
1:      bcc         2f
        // copies 8 bytes, destination 64-bits aligned
        vld1.8      {d0}, [r1]!
        vst1.8      {d0}, [r0, :64]!
2:
        /* preload immediately the next cache line, which we may need */
        pld         [r1, #0]
        pld         [r1, #(32 * 2)]
3:
        /* make sure we have at least 64 bytes to copy */
        subs        r2, r2, #64
        blo         2f

        /* preload all the cache lines we need */
        pld         [r1, #(32 * 4)]
        pld         [r1, #(32 * 6)]

1:      /* The main loop copies 64 bytes at a time */
        vld1.8      {d0 - d3}, [r1]!
        vld1.8      {d4 - d7}, [r1]!
        pld         [r1, #(32 * 6)]
        subs        r2, r2, #64
        vst1.8      {d0 - d3}, [r0]!
        vst1.8      {d4 - d7}, [r0]!
        bhs         1b

2:      /* fix-up the remaining count and make sure we have >= 32 bytes left */
        add         r2, r2, #64
        subs        r2, r2, #32
        blo         4f

3:      /* 32 bytes at a time. These cache lines were already preloaded */
        vld1.8      {d0 - d3}, [r1]!
        subs        r2, r2, #32
        vst1.8      {d0 - d3}, [r0]!
        bhs         3b

4:      /* less than 32 left */
        add         r2, r2, #32
        tst         r2, #0x10
        beq         5f
        // copies 16 bytes, 128-bits aligned
        vld1.8      {d0, d1}, [r1]!
        vst1.8      {d0, d1}, [r0]!
5:      /* copy up to 15-bytes (count in r2) */
        movs        ip, r2, lsl #29
        bcc         1f
        vld1.8      {d0}, [r1]!
        vst1.8      {d0}, [r0]!
1:      bge         2f
        vld1.32     {d0[0]}, [r1]!
        vst1.32     {d0[0]}, [r0]!
2:      movs        ip, r2, lsl #31
        itt         mi
        ldrbmi      r3, [r1], #1
        strbmi      r3, [r0], #1
        itttt       cs
        ldrbcs      ip, [r1], #1
        ldrbcs      lr, [r1], #1
        strbcs      ip, [r0], #1
        strbcs      lr, [r0], #1

        ldmfd       sp!, {r0, lr}
        bx          lr

        .cfi_endproc
END(MEMCPY_BASE)

ENTRY(MEMCPY_BASE_ALIGNED)
        .cfi_startproc

        .save       {r0, lr}
        .cfi_def_cfa_offset 8
        .cfi_rel_offset r0, 0
        .cfi_rel_offset lr, 4

        /* Simple arm-only copy loop to handle aligned copy operations */
        stmfd       sp!, {r4-r8}
        .save       {r4-r8}
        .cfi_adjust_cfa_offset 20
        .cfi_rel_offset r4, 0
        .cfi_rel_offset r5, 4
        .cfi_rel_offset r6, 8
        .cfi_rel_offset r7, 12
        .cfi_rel_offset r8, 16
        pld         [r1, #(32 * 4)]

        /* Check alignment */
        rsb         r3, r1, #0
        ands        r3, #3
        beq         2f

        /* align source to 32 bits. We need to insert 2 instructions between
         * a ldr[b|h] and str[b|h] because byte and half-word instructions
         * stall 2 cycles.
         */
        movs        r12, r3, lsl #31
        sub         r2, r2, r3      /* we know that r3 <= r2 because r2 >= 4 */
        itt         mi
        ldrbmi      r3, [r1], #1
        strbmi      r3, [r0], #1
        itttt       cs
        ldrbcs      r4, [r1], #1
        ldrbcs      r5, [r1], #1
        strbcs      r4, [r0], #1
        strbcs      r5, [r0], #1

2:
        subs        r2, r2, #64
        blt         4f

3:      /* Main copy loop, copying 64 bytes at a time */
        pld         [r1, #(32 * 8)]
        ldmia       r1!, {r3, r4, r5, r6, r7, r8, r12, lr}
        stmia       r0!, {r3, r4, r5, r6, r7, r8, r12, lr}
        ldmia       r1!, {r3, r4, r5, r6, r7, r8, r12, lr}
        stmia       r0!, {r3, r4, r5, r6, r7, r8, r12, lr}
        subs        r2, r2, #64
        bge         3b

4:      /* Check if there are > 32 bytes left */
        adds        r2, r2, #64
        subs        r2, r2, #32
        blt         5f

        /* Copy 32 bytes */
        ldmia       r1!, {r3, r4, r5, r6, r7, r8, r12, lr}
        stmia       r0!, {r3, r4, r5, r6, r7, r8, r12, lr}
        subs        r2, #32

5:      /* Handle any remaining bytes */
        adds        r2, #32
        beq         6f

        movs        r12, r2, lsl #28
        itt         cs
        ldmiacs     r1!, {r3, r4, r5, r6}   /* 16 bytes */
        stmiacs     r0!, {r3, r4, r5, r6}
        itt         mi
        ldmiami     r1!, {r7, r8}           /*  8 bytes */
        stmiami     r0!, {r7, r8}
        movs        r12, r2, lsl #30
        itt         cs
        ldrcs       r3, [r1], #4            /*  4 bytes */
        strcs       r3, [r0], #4
        itt         mi
        ldrhmi      r4, [r1], #2            /*  2 bytes */
        strhmi      r4, [r0], #2
        tst         r2, #0x1
        itt         ne
        ldrbne      r3, [r1]                /*  last byte  */
        strbne      r3, [r0]
6:
        ldmfd       sp!, {r4-r8}
        ldmfd       sp!, {r0, pc}

        .cfi_endproc
END(MEMCPY_BASE_ALIGNED)
