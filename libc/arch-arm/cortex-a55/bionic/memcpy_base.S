/*
 * Copyright (C) 2008 The Android Open Source Project
 * All rights reserved.
 * Copyright (c) 2013-2014, NVIDIA Corporation.  All rights reserved.
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

#define CACHE_LINE_SIZE         (64)
#define PREFETCH_DISTANCE       (CACHE_LINE_SIZE*6)

ENTRY_PRIVATE(MEMCPY_BASE)
        .cfi_def_cfa_offset 8
        .cfi_rel_offset r0, 0
        .cfi_rel_offset lr, 4

        cmp         r2, #0
        beq         .L_memcpy_done
        cmp         r0, r1
        beq         .L_memcpy_done

        /* preload next cache line */
        pld         [r1, #CACHE_LINE_SIZE*1]

        /* Deal with very small blocks (< 32bytes) asap */
        cmp         r2, #32
        blo         .L_memcpy_lt_32bytes
        /* no need to align if len < 128 bytes */
        cmp         r2, #128
        blo         .L_memcpy_lt_128bytes

        /* large copy, align dest to 64 byte boundry */
        pld         [r1, #CACHE_LINE_SIZE*2]
        rsb         r3, r0, #0
        ands        r3, r3, #0x3F
        pld         [r1, #CACHE_LINE_SIZE*3]
        beq         .L_memcpy_dispatch
        sub         r2, r2, r3
        /* copy 1 byte */
        movs        ip, r3, lsl #31
        itt         mi
        ldrbmi      ip, [r1], #1
        strbmi      ip, [r0], #1
        /* copy 2 bytes */
        itt         cs
        ldrhcs      ip, [r1], #2
        strhcs      ip, [r0], #2
        /* copy 4 bytes */
        movs        ip, r3, lsl #29
        itt         mi
        ldrmi       ip, [r1], #4
        strmi       ip, [r0], #4
        /* copy 8 bytes */
        bcc         1f
        vld1.8      {d0}, [r1]!
        vst1.8      {d0}, [r0, :64]!
1:      /* copy 16 bytes */
        movs        ip, r3, lsl #27
        bpl         1f
        vld1.8      {q0}, [r1]!
        vst1.8      {q0}, [r0, :128]!
1:      /* copy 32 bytes */
        bcc         .L_memcpy_dispatch
        vld1.8      {q0, q1}, [r1]!
        vst1.8      {q0, q1}, [r0, :256]!

.L_memcpy_dispatch:
        // pre-decrement by 128 to detect nearly-done condition easily, but
        // also need to check if we have less than 128 bytes left at this
        // point due to alignment code above
        subs        r2, r2, #128
        blo         .L_memcpy_lt_128presub

        // Denver does better if both source and dest are aligned so
        // we'll special-case that even though the code is virually identical
        tst         r1, #0xF
        bne         .L_memcpy_neon_unalign_src_pld

        // DRAM memcpy should be throttled slightly to get full bandwidth
        //
        cmp         r2, #32768
        bhi         .L_memcpy_neon_unalign_src_pld
        .align      4
1:
        /* copy 128 bytes in each loop */
        subs        r2, r2, #128

        /* preload a cache line */
        pld         [r1, #PREFETCH_DISTANCE]
        /* copy a cache line */
        vld1.8      {q0, q1}, [r1, :128]!
        vst1.8      {q0, q1}, [r0, :256]!
        vld1.8      {q0, q1}, [r1, :128]!
        vst1.8      {q0, q1}, [r0, :256]!
        /* preload a cache line */
        pld         [r1, #PREFETCH_DISTANCE]
        /* copy a cache line */
        vld1.8      {q0, q1}, [r1, :128]!
        vst1.8      {q0, q1}, [r0, :256]!
        vld1.8      {q0, q1}, [r1, :128]!
        vst1.8      {q0, q1}, [r0, :256]!

        bhs         1b
        adds        r2, r2, #128
        bne         .L_memcpy_lt_128bytes_align
        pop         {r0, pc}

        .align      4
.L_memcpy_neon_unalign_src_pld:
1:
        /* copy 128 bytes in each loop */
        subs        r2, r2, #128

        /* preload a cache line */
        pld         [r1, #PREFETCH_DISTANCE]
        /* copy a cache line */
        vld1.8      {q0, q1}, [r1]!
        vst1.8      {q0, q1}, [r0, :256]!
        vld1.8      {q0, q1}, [r1]!
        vst1.8      {q0, q1}, [r0, :256]!
        /* preload a cache line */
        pld         [r1, #PREFETCH_DISTANCE]
        /* copy a cache line */
        vld1.8      {q0, q1}, [r1]!
        vst1.8      {q0, q1}, [r0, :256]!
        vld1.8      {q0, q1}, [r1]!
        vst1.8      {q0, q1}, [r0, :256]!

        bhs         1b
        adds        r2, r2, #128
        bne         .L_memcpy_lt_128bytes_align
        pop         {r0, pc}

.L_memcpy_lt_128presub:
        add         r2, r2, #128
.L_memcpy_lt_128bytes_align:
        /* copy 64 bytes */
        movs        ip, r2, lsl #26
        bcc         1f
        vld1.8      {q0, q1}, [r1]!
        vst1.8      {q0, q1}, [r0, :256]!
        vld1.8      {q0, q1}, [r1]!
        vst1.8      {q0, q1}, [r0, :256]!
1:      /* copy 32 bytes */
        bpl         1f
        vld1.8      {q0, q1}, [r1]!
        vst1.8      {q0, q1}, [r0, :256]!
1:      /* copy 16 bytes */
        movs        ip, r2, lsl #28
        bcc         1f
        vld1.8      {q0}, [r1]!
        vst1.8      {q0}, [r0, :128]!
1:      /* copy 8 bytes */
        bpl         1f
        vld1.8      {d0}, [r1]!
        vst1.8      {d0}, [r0, :64]!
1:      /* copy 4 bytes */
        tst         r2, #4
        itt         ne
        ldrne       ip, [r1], #4
        strne       ip, [r0], #4
        /* copy 2 bytes */
        movs        ip, r2, lsl #31
        itt         cs
        ldrhcs      ip, [r1], #2
        strhcs      ip, [r0], #2
        /* copy 1 byte */
        itt         mi
        ldrbmi      ip, [r1]
        strbmi      ip, [r0]

        pop         {r0, pc}

.L_memcpy_lt_128bytes:
        /* copy 64 bytes */
        movs        ip, r2, lsl #26
        bcc         1f
        vld1.8      {q0, q1}, [r1]!
        vst1.8      {q0, q1}, [r0]!
        vld1.8      {q0, q1}, [r1]!
        vst1.8      {q0, q1}, [r0]!
1:      /* copy 32 bytes */
        bpl	    .L_memcpy_lt_32bytes
        vld1.8      {q0, q1}, [r1]!
        vst1.8      {q0, q1}, [r0]!
.L_memcpy_lt_32bytes:
        /* copy 16 bytes */
        movs        ip, r2, lsl #28
        bcc         1f
        vld1.8      {q0}, [r1]!
        vst1.8      {q0}, [r0]!
1:      /* copy 8 bytes */
        bpl         1f
        vld1.8      {d0}, [r1]!
        vst1.8      {d0}, [r0]!
1:      /* copy 4 bytes */
        tst         r2, #4
        itt         ne
        ldrne       ip, [r1], #4
        strne       ip, [r0], #4
        /* copy 2 bytes */
        movs        ip, r2, lsl #31
        itt         cs
        ldrhcs      ip, [r1], #2
        strhcs      ip, [r0], #2
        /* copy 1 byte */
        itt         mi
        ldrbmi      ip, [r1]
        strbmi      ip, [r0]

.L_memcpy_done:
        pop         {r0, pc}
END(MEMCPY_BASE)
