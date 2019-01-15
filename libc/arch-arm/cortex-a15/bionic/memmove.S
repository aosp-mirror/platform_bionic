/*
 * Copyright (C) 2013 The Android Open Source Project
 * All rights reserved.
 * Copyright (c) 2013-2014 NVIDIA Corporation.  All rights reserved.
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

        .text
        .syntax unified
        .fpu    neon

#define CACHE_LINE_SIZE         (64)
#define MEMCPY_BLOCK_SIZE_SMALL (32768)
#define MEMCPY_BLOCK_SIZE_MID   (1048576)
#define PREFETCH_DISTANCE_NEAR  (CACHE_LINE_SIZE*4)
#define PREFETCH_DISTANCE_MID   (CACHE_LINE_SIZE*4)
#define PREFETCH_DISTANCE_FAR   (CACHE_LINE_SIZE*16)

ENTRY(memmove_a15)
        cmp         r2, #0
        cmpne       r0, r1
        bxeq        lr
        subs        r3, r0, r1
        bls         .L_jump_to_memcpy
        cmp         r2, r3
        bhi         .L_reversed_memcpy

.L_jump_to_memcpy:
        b           __memcpy

.L_reversed_memcpy:
        push        {r0, lr}
        .cfi_def_cfa_offset 8
        .cfi_rel_offset r0, 0
        .cfi_rel_offset lr, 4

        add         r0, r0, r2
        add         r1, r1, r2

        /* preload next cache line */
        pld         [r1, #-CACHE_LINE_SIZE]
        pld         [r1, #-CACHE_LINE_SIZE*2]

.L_reversed_memcpy_align_dest:
        /* Deal with very small blocks (< 32bytes) asap */
        cmp         r2, #32
        blo         .L_reversed_memcpy_lt_32bytes
        /* no need to align if len < 128 bytes */
        cmp         r2, #128
        blo         .L_reversed_memcpy_lt_128bytes
        /* align destination to 64 bytes (1 cache line) */
        ands        r3, r0, #0x3f
        beq         .L_reversed_memcpy_dispatch
        sub         r2, r2, r3
0:      /* copy 1 byte */
        movs        ip, r3, lsl #31
        ldrbmi      ip, [r1, #-1]!
        strbmi      ip, [r0, #-1]!
1:      /* copy 2 bytes */
        ldrbcs      ip, [r1, #-1]!
        strbcs      ip, [r0, #-1]!
        ldrbcs      ip, [r1, #-1]!
        strbcs      ip, [r0, #-1]!
2:      /* copy 4 bytes */
        movs        ip, r3, lsl #29
        bpl         3f
        sub         r1, r1, #4
        sub         r0, r0, #4
        vld4.8      {d0[0], d1[0], d2[0], d3[0]}, [r1]
        vst4.8      {d0[0], d1[0], d2[0], d3[0]}, [r0, :32]
3:      /* copy 8 bytes */
        bcc         4f
        sub         r1, r1, #8
        sub         r0, r0, #8
        vld1.8      {d0}, [r1]
        vst1.8      {d0}, [r0, :64]
4:      /* copy 16 bytes */
        movs        ip, r3, lsl #27
        bpl         5f
        sub         r1, r1, #16
        sub         r0, r0, #16
        vld1.8      {q0}, [r1]
        vst1.8      {q0}, [r0, :128]
5:      /* copy 32 bytes */
        bcc         .L_reversed_memcpy_dispatch
        sub         r1, r1, #32
        sub         r0, r0, #32
        vld1.8      {q0, q1}, [r1]
        vst1.8      {q0, q1}, [r0, :256]

.L_reversed_memcpy_dispatch:
        /* preload more cache lines */
        pld         [r1, #-CACHE_LINE_SIZE*3]
        pld         [r1, #-CACHE_LINE_SIZE*4]

        cmp         r2, #MEMCPY_BLOCK_SIZE_SMALL
        blo         .L_reversed_memcpy_neon_pld_near
        cmp         r2, #MEMCPY_BLOCK_SIZE_MID
        blo         .L_reversed_memcpy_neon_pld_mid
        b           .L_reversed_memcpy_neon_pld_far

.L_reversed_memcpy_neon_pld_near:
        /* less than 128 bytes? */
        subs        r2, r2, #128
        blo         1f
        sub         r1, r1, #32
        sub         r0, r0, #32
        mov         r3, #-32
        .align      4
0:
        /* copy 128 bytes in each loop */
        subs        r2, r2, #128

        /* preload to cache */
        pld         [r1, #-(PREFETCH_DISTANCE_NEAR+CACHE_LINE_SIZE*2)+32]
        /* copy a cache line */
        vld1.8      {q0, q1}, [r1], r3
        vst1.8      {q0, q1}, [r0, :256], r3
        vld1.8      {q0, q1}, [r1], r3
        vst1.8      {q0, q1}, [r0, :256], r3

        /* preload to cache */
        pld         [r1, #-(PREFETCH_DISTANCE_NEAR+CACHE_LINE_SIZE*2)+32]
        /* copy a cache line */
        vld1.8      {q0, q1}, [r1], r3
        vst1.8      {q0, q1}, [r0, :256], r3
        vld1.8      {q0, q1}, [r1], r3
        vst1.8      {q0, q1}, [r0, :256], r3

        bhs         0b
        add         r1, r1, #32
        add         r0, r0, #32
1:
        adds        r2, r2, #128
        bne         .L_reversed_memcpy_lt_128bytes
        pop         {r0, pc}

.L_reversed_memcpy_neon_pld_mid:
        subs        r2, r2, #128
        sub         r1, r1, #32
        sub         r0, r0, #32
        mov         r3, #-32
        .align      4
0:
        /* copy 128 bytes in each loop */
        subs        r2, r2, #128

        /* preload to cache */
        pld         [r1, #-(PREFETCH_DISTANCE_MID+CACHE_LINE_SIZE)+32]
        /* copy a cache line */
        vld1.8      {q0, q1}, [r1], r3
        vst1.8      {q0, q1}, [r0, :256], r3
        vld1.8      {q0, q1}, [r1], r3
        vst1.8      {q0, q1}, [r0, :256], r3

        /* preload to cache */
        pld         [r1, #-(PREFETCH_DISTANCE_MID+CACHE_LINE_SIZE)+32]
        /* copy a cache line */
        vld1.8      {q0, q1}, [r1], r3
        vst1.8      {q0, q1}, [r0, :256], r3
        vld1.8      {q0, q1}, [r1], r3
        vst1.8      {q0, q1}, [r0, :256], r3

        bhs         0b
        add         r1, r1, #32
        add         r0, r0, #32
1:
        adds        r2, r2, #128
        bne         .L_reversed_memcpy_lt_128bytes
        pop         {r0, pc}

.L_reversed_memcpy_neon_pld_far:
        sub         r2, r2, #128
        sub         r0, r0, #128
        sub         r1, r1, #128
        .align      4
0:
        /* copy 128 bytes in each loop */
        subs        r2, r2, #128

        /* preload to cache */
        pld         [r1, #-(PREFETCH_DISTANCE_FAR+CACHE_LINE_SIZE*2)+128]
        pld         [r1, #-(PREFETCH_DISTANCE_FAR+CACHE_LINE_SIZE)+128]
        /* read */
        vld1.8      {q0, q1}, [r1]!
        vld1.8      {q2, q3}, [r1]!
        vld1.8      {q8, q9}, [r1]!
        vld1.8      {q10, q11}, [r1]!
        /* write */
        vst1.8      {q0, q1}, [r0, :256]!
        vst1.8      {q2, q3}, [r0, :256]!
        vst1.8      {q8, q9}, [r0, :256]!
        vst1.8      {q10, q11}, [r0, :256]!

        sub         r0, r0, #256
        sub         r1, r1, #256
        bhs         0b
        add         r0, r0, #128
        add         r1, r1, #128
1:
        adds        r2, r2, #128
        bne         .L_reversed_memcpy_lt_128bytes
        pop         {r0, pc}

.L_reversed_memcpy_lt_128bytes:
6:      /* copy 64 bytes */
        movs        ip, r2, lsl #26
        bcc         5f
        sub         r1, r1, #32
        sub         r0, r0, #32
        vld1.8      {q0, q1}, [r1]
        vst1.8      {q0, q1}, [r0]
        sub         r1, r1, #32
        sub         r0, r0, #32
        vld1.8      {q0, q1}, [r1]
        vst1.8      {q0, q1}, [r0]
5:      /* copy 32 bytes */
        bpl         4f
        sub         r1, r1, #32
        sub         r0, r0, #32
        vld1.8      {q0, q1}, [r1]
        vst1.8      {q0, q1}, [r0]
.L_reversed_memcpy_lt_32bytes:
4:      /* copy 16 bytes */
        movs        ip, r2, lsl #28
        bcc         3f
        sub         r1, r1, #16
        sub         r0, r0, #16
        vld1.8      {q0}, [r1]
        vst1.8      {q0}, [r0]
3:      /* copy 8 bytes */
        bpl         2f
        sub         r1, r1, #8
        sub         r0, r0, #8
        vld1.8      {d0}, [r1]
        vst1.8      {d0}, [r0]
2:      /* copy 4 bytes */
        ands        ip, r2, #0x4
        beq         1f
        sub         r1, r1, #4
        sub         r0, r0, #4
        vld4.8      {d0[0], d1[0], d2[0], d3[0]}, [r1]
        vst4.8      {d0[0], d1[0], d2[0], d3[0]}, [r0]
1:      /* copy 2 bytes */
        movs        ip, r2, lsl #31
        ldrbcs      ip, [r1, #-1]!
        strbcs      ip, [r0, #-1]!
        ldrbcs      ip, [r1, #-1]!
        strbcs      ip, [r0, #-1]!
0:      /* copy 1 byte */
        ldrbmi      ip, [r1, #-1]!
        strbmi      ip, [r0, #-1]!

        pop         {r0, pc}

END(memmove_a15)
