/*
 * Copyright (C) 2013 The Android Open Source Project
 * Copyright (c) 2014, NVIDIA CORPORATION.  All rights reserved.
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
         * Optimized memset() for ARM.
         *
         * memset() returns its first argument.
         */

        .cpu        cortex-a15
        .fpu        neon
        .syntax     unified

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
        mov         r2, r1
        mov         r1, #0
.L_done:
        // Fall through to memset...
END(bzero)

ENTRY(memset)
        pldw        [r0]
        mov         r3, r0

        // Duplicate the low byte of r1
        mov         r1, r1, lsl #24
        orr         r1, r1, r1, lsr #8
        orr         r1, r1, r1, lsr #16

        cmp         r2, #16
        blo         .L_less_than_16

        // This section handles regions 16 bytes or larger
        //
        // Use aligned vst1.8 and vstm when possible.  Register values will be:
        //   ip is scratch
        //   q0, q1, and r1 contain the memset value
        //   r2 is the number of bytes to set
        //   r3 is the advancing destination pointer
        vdup.32     q0, r1

        ands        ip, r3, 0xF
        beq         .L_memset_aligned

        // Align dest pointer to 16-byte boundary.
        pldw        [r0, #64]
        rsb         ip, ip, #16

        // Pre-adjust the byte count to reflect post-aligment value.  Expecting
        // 8-byte alignment to be rather common so we special case that one.
        sub         r2, r2, ip

        /* set 1 byte */
        tst         ip, #1
        it          ne
        strbne      r1, [r3], #1
        /* set 2 bytes */
        tst         ip, #2
        it          ne
        strhne      r1, [r3], #2
        /* set 4 bytes */
        movs        ip, ip, lsl #29
        it          mi
        strmi       r1, [r3], #4
        /* set 8 bytes */
        itt         cs
        strcs       r1, [r3], #4
        strcs       r1, [r3], #4

.L_memset_aligned:
        // Destination is now 16-byte aligned.  Determine how to handle
        // remaining bytes.
        vmov        q1, q0
        cmp         r2, #128
        blo         .L_less_than_128

        // We need to set a larger block of memory.  Use four Q regs to
        // set a full cache line in one instruction.  Pre-decrement
        // r2 to simplify end-of-loop detection
        vmov        q2, q0
        vmov        q3, q0
        pldw        [r0, #128]
        sub         r2, r2, #128
        .align 4
.L_memset_loop_128:
        pldw        [r3, #192]
        vstm        r3!, {q0, q1, q2, q3}
        vstm        r3!, {q0, q1, q2, q3}
        subs        r2, r2, #128
        bhs         .L_memset_loop_128

        // Un-bias r2 so it contains the number of bytes left.  Early
        // exit if we are done.
        adds        r2, r2, #128
        beq         2f

        .align 4
.L_less_than_128:
        // set 64 bytes
        movs        ip, r2, lsl #26
        bcc         1f
        vst1.8      {q0, q1}, [r3, :128]!
        vst1.8      {q0, q1}, [r3, :128]!
        beq         2f
1:
        // set 32 bytes
        bpl         1f
        vst1.8      {q0, q1}, [r3, :128]!
1:
        // set 16 bytes
        movs        ip, r2, lsl #28
        bcc         1f
        vst1.8      {q0}, [r3, :128]!
        beq         2f
1:
        // set 8 bytes
        bpl         1f
        vst1.8      {d0}, [r3, :64]!
1:
        // set 4 bytes
        tst         r2, #4
        it          ne
        strne       r1, [r3], #4
1:
        // set 2 bytes
        movs        ip, r2, lsl #31
        it          cs
        strhcs      r1, [r3], #2
        // set 1 byte
        it          mi
        strbmi      r1, [r3]
2:
        bx          lr

.L_less_than_16:
        // Store up to 15 bytes without worrying about byte alignment
        movs        ip, r2, lsl #29
        bcc         1f
        str         r1, [r3], #4
        str         r1, [r3], #4
        beq         2f
1:
        it          mi
        strmi       r1, [r3], #4
        movs        ip, r2, lsl #31
        it          mi
        strbmi      r1, [r3], #1
        itt         cs
        strbcs      r1, [r3], #1
        strbcs      r1, [r3]
2:
        bx          lr
END(memset)

        .data
error_string:
        .string     "memset: prevented write past end of buffer"
