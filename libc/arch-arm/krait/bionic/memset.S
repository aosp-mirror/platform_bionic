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

#include <machine/cpu-features.h>
#include <private/bionic_asm.h>
#include <private/libc_events.h>

/*
 * This code assumes it is running on a processor that supports all arm v7
 * instructions, that supports neon instructions, and that supports
 * unaligned neon instruction accesses to memory.
 */

    .fpu    neon
    .syntax unified

ENTRY(__memset_chk)
        cmp         r2, r3
        bls         .L_done

        // Preserve lr for backtrace.
        push        {lr}
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
        mov         r3, r0
        vdup.8      q0, r1

        /* make sure we have at least 32 bytes to write */
        subs        r2, r2, #32
        blo         2f
        vmov        q1, q0

1:      /* The main loop writes 32 bytes at a time */
        subs        r2, r2, #32
        vst1.8      {d0 - d3}, [r3]!
        bhs         1b

2:      /* less than 32 left */
        add         r2, r2, #32
        tst         r2, #0x10
        beq         3f

        // writes 16 bytes, 128-bits aligned
        vst1.8      {d0, d1}, [r3]!
3:      /* write up to 15-bytes (count in r2) */
        movs        ip, r2, lsl #29
        bcc         1f
        vst1.8      {d0}, [r3]!
1:      bge         2f
        vst1.32     {d0[0]}, [r3]!
2:      movs        ip, r2, lsl #31
        strbmi      r1, [r3], #1
        strbcs      r1, [r3], #1
        strbcs      r1, [r3], #1
        bx          lr
END(memset)

        .data
error_string:
        .string     "memset: prevented write past end of buffer"
