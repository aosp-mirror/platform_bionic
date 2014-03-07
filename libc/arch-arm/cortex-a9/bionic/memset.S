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

#include <private/bionic_asm.h>
#include <private/libc_events.h>

/*
 * This code assumes it is running on a processor that supports all arm v7
 * instructions and that supports neon instructions.
 */

    .fpu    neon

ENTRY(__memset_chk)
        cmp         r2, r3
        bls         .L_done

        // Preserve lr for backtrace.
        push        {lr}
        .save       {lr}
        .cfi_def_cfa_offset 4
        .cfi_rel_offset lr, 0

        ldr         r0, error_message
        ldr         r1, error_code
1:
        add         r0, pc
        bl          __fortify_chk_fail
error_code:
        .word       BIONIC_EVENT_MEMSET_BUFFER_OVERFLOW
error_message:
        .word       error_string-(1b+8)
END(__memset_chk)

ENTRY(bzero)
        mov     r2, r1
        mov     r1, #0

.L_done:
        // Fall through to memset...
END(bzero)

/* memset() returns its first argument.  */
ENTRY(memset)
        // The neon memset only wins for less than 132.
        cmp         r2, #132
        bhi         __memset_large_copy

        stmfd       sp!, {r0}
        .save       {r0}
        .cfi_def_cfa_offset 4
        .cfi_rel_offset r0, 0

        vdup.8      q0, r1

        /* make sure we have at least 32 bytes to write */
        subs        r2, r2, #32
        blo         2f
        vmov        q1, q0

1:      /* The main loop writes 32 bytes at a time */
        subs        r2, r2, #32
        vst1.8      {d0 - d3}, [r0]!
        bhs         1b

2:      /* less than 32 left */
        add         r2, r2, #32
        tst         r2, #0x10
        beq         3f

        // writes 16 bytes, 128-bits aligned
        vst1.8      {d0, d1}, [r0]!
3:      /* write up to 15-bytes (count in r2) */
        movs        ip, r2, lsl #29
        bcc         1f
        vst1.8      {d0}, [r0]!
1:      bge         2f
        vst1.32     {d0[0]}, [r0]!
2:      movs        ip, r2, lsl #31
        strmib      r1, [r0], #1
        strcsb      r1, [r0], #1
        strcsb      r1, [r0], #1
        ldmfd       sp!, {r0}
        bx          lr
END(memset)

ENTRY_PRIVATE(__memset_large_copy)
        /* compute the offset to align the destination
         * offset = (4-(src&3))&3 = -src & 3
         */
        stmfd       sp!, {r0, r4-r7, lr}
        .save       {r0, r4-r7, lr}
        .cfi_def_cfa_offset 24
        .cfi_rel_offset r0, 0
        .cfi_rel_offset r4, 4
        .cfi_rel_offset r5, 8
        .cfi_rel_offset r6, 12
        .cfi_rel_offset r7, 16
        .cfi_rel_offset lr, 20

        rsb         r3, r0, #0
        ands        r3, r3, #3
        cmp         r3, r2
        movhi       r3, r2

        /* splat r1 */
        mov         r1, r1, lsl #24
        orr         r1, r1, r1, lsr #8
        orr         r1, r1, r1, lsr #16

        movs        r12, r3, lsl #31
        strcsb      r1, [r0], #1    /* can't use strh (alignment unknown) */
        strcsb      r1, [r0], #1
        strmib      r1, [r0], #1
        subs        r2, r2, r3
        ldmlsfd     sp!, {r0, r4-r7, lr}   /* return */
        bxls        lr

        /* align the destination to a cache-line */
        mov         r12, r1
        mov         lr, r1
        mov         r4, r1
        mov         r5, r1
        mov         r6, r1
        mov         r7, r1

        rsb         r3, r0, #0
        ands        r3, r3, #0x1C
        beq         3f
        cmp         r3, r2
        andhi       r3, r2, #0x1C
        sub         r2, r2, r3

        /* conditionally writes 0 to 7 words (length in r3) */
        movs        r3, r3, lsl #28
        stmcsia     r0!, {r1, lr}
        stmcsia     r0!, {r1, lr}
        stmmiia     r0!, {r1, lr}
        movs        r3, r3, lsl #2
        strcs       r1, [r0], #4

3:
        subs        r2, r2, #32
        mov         r3, r1
        bmi         2f
1:      subs        r2, r2, #32
        stmia       r0!, {r1,r3,r4,r5,r6,r7,r12,lr}
        bhs         1b
2:      add         r2, r2, #32

        /* conditionally stores 0 to 31 bytes */
        movs        r2, r2, lsl #28
        stmcsia     r0!, {r1,r3,r12,lr}
        stmmiia     r0!, {r1, lr}
        movs        r2, r2, lsl #2
        strcs       r1, [r0], #4
        strmih      r1, [r0], #2
        movs        r2, r2, lsl #2
        strcsb      r1, [r0]
        ldmfd       sp!, {r0, r4-r7, lr}
        bx          lr
END(__memset_large_copy)

        .data
error_string:
        .string     "memset: prevented write past end of buffer"
