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


/*
 * This code assumes it is running on a processor that supports all arm v7
 * instructions, that supports neon instructions, and that has a 32 byte
 * cache line.
 */

// Assumes neon instructions and a cache line size of 32 bytes.

ENTRY_PRIVATE(MEMCPY_BASE)
        .cfi_def_cfa_offset 8
        .cfi_rel_offset r0, 0
        .cfi_rel_offset lr, 4

        /* do we have at least 16-bytes to copy (needed for alignment below) */
        cmp         r2, #16
        blo         5f

        /* align destination to cache-line for the write-buffer */
        rsb         r3, r0, #0
        ands        r3, r3, #0xF
        beq         2f

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
        vld4.8      {d0[0], d1[0], d2[0], d3[0]}, [r1]!
        vst4.8      {d0[0], d1[0], d2[0], d3[0]}, [r0, :32]!
1:      bcc         2f
        // copies 8 bytes, destination 64-bits aligned
        vld1.8      {d0}, [r1]!
        vst1.8      {d0}, [r0, :64]!

2:      /* make sure we have at least 64 bytes to copy */
        subs        r2, r2, #64
        blo         2f

1:      /* The main loop copies 64 bytes at a time */
        vld1.8      {d0  - d3},   [r1]!
        vld1.8      {d4  - d7},   [r1]!
        pld         [r1, #(32*8)]
        subs        r2, r2, #64
        vst1.8      {d0  - d3},   [r0, :128]!
        vst1.8      {d4  - d7},   [r0, :128]!
        bhs         1b

2:      /* fix-up the remaining count and make sure we have >= 32 bytes left */
        adds        r2, r2, #32
        blo         4f

        /* Copy 32 bytes. These cache lines were already preloaded */
        vld1.8      {d0 - d3},  [r1]!
        sub         r2, r2, #32
        vst1.8      {d0 - d3},  [r0, :128]!

4:      /* less than 32 left */
        add         r2, r2, #32
        tst         r2, #0x10
        beq         5f
        // copies 16 bytes, 128-bits aligned
        vld1.8      {d0, d1}, [r1]!
        vst1.8      {d0, d1}, [r0, :128]!

5:      /* copy up to 15-bytes (count in r2) */
        movs        ip, r2, lsl #29
        bcc         1f
        vld1.8      {d0}, [r1]!
        vst1.8      {d0}, [r0]!
1:      bge         2f
        vld4.8      {d0[0], d1[0], d2[0], d3[0]}, [r1]!
        vst4.8      {d0[0], d1[0], d2[0], d3[0]}, [r0]!
2:      movs        ip, r2, lsl #31
        itt         mi
        ldrbmi      r3, [r1], #1
        strbmi      r3, [r0], #1
        itttt       cs
        ldrbcs      ip, [r1], #1
        ldrbcs      lr, [r1], #1
        strbcs      ip, [r0], #1
        strbcs      lr, [r0], #1

        ldmfd       sp!, {r0, pc}
END(MEMCPY_BASE)
