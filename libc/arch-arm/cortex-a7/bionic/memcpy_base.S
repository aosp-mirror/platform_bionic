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
 * Copyright (c) 2013 ARM Ltd
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the company may not be used to endorse or promote
 *    products derived from this software without specific prior written
 *    permission.
 *
 * THIS SOFTWARE IS PROVIDED BY ARM LTD ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL ARM LTD BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

.L_memcpy_base:
        // Assumes that n >= 0, and dst, src are valid pointers.
        // For any sizes less than 832 use the neon code that doesn't
        // care about the src alignment. This avoids any checks
        // for src alignment, and offers the best improvement since
        // smaller sized copies are dominated by the overhead of
        // the pre and post main loop.
        // For larger copies, if src and dst cannot both be aligned to
        // word boundaries, use the neon code.
        // For all other copies, align dst to a double word boundary
        // and copy using LDRD/STRD instructions.

        cmp     r2, #16
        blo     .L_copy_less_than_16_unknown_align

.L_copy_unknown_alignment:
        // Unknown alignment of src and dst.
        // Assumes that the first few bytes have already been prefetched.

        // Align destination to 128 bits. The mainloop store instructions
        // require this alignment or they will throw an exception.
        rsb         r3, r0, #0
        ands        r3, r3, #0xF
        beq         2f

        // Copy up to 15 bytes (count in r3).
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
        // Copies 4 bytes, dst 32 bits aligned before, at least 64 bits after.
        vld4.8      {d0[0], d1[0], d2[0], d3[0]}, [r1]!
        vst4.8      {d0[0], d1[0], d2[0], d3[0]}, [r0, :32]!
1:      bcc         2f
        // Copies 8 bytes, dst 64 bits aligned before, at least 128 bits after.
        vld1.8      {d0}, [r1]!
        vst1.8      {d0}, [r0, :64]!

2:      cmp         r2, #256
        ble         .L_copy_loop

        // Make sure DST is 64 BYTE aligned.
        rsb         r3, r0, #0
        ands        r3, r3, #0x30
        beq         .L_copy_loop

        sub         r2, r2, r3
        cmp         r3, #0x10
        beq         .L_copy_16

        vld1.8      {d0  - d3},   [r1]!
        vst1.8      {d0  - d3},   [r0, :128]!
        ands        r3, r3, #0x10
        beq         .L_copy_loop

.L_copy_16:
        vld1.8      {d0, d1}, [r1]!
        vst1.8      {d0, d1}, [r0, :128]!

.L_copy_loop:
        // Make sure we have at least 64 bytes to copy.
        subs        r2, r2, #64
        blo         2f

1:      // The main loop copies 64 bytes at a time.
        vld1.8      {d0  - d3},   [r1]!
        vst1.8      {d0  - d3},   [r0, :128]!
        pld         [r1, #(64*4)]
        subs        r2, r2, #64
        vld1.8      {d4  - d7},   [r1]!
        vst1.8      {d4  - d7},   [r0, :128]!
        bhs         1b

2:      // Fix-up the remaining count and make sure we have >= 32 bytes left.
        adds        r2, r2, #32
        blo         3f

        // 32 bytes. These cache lines were already preloaded.
        vld1.8      {d0 - d3},  [r1]!
        sub         r2, r2, #32
        vst1.8      {d0 - d3},  [r0, :128]!
3:      // Less than 32 left.
        add         r2, r2, #32
        tst         r2, #0x10
        beq         .L_copy_less_than_16_unknown_align
        // Copies 16 bytes, destination 128 bits aligned.
        vld1.8      {d0, d1}, [r1]!
        vst1.8      {d0, d1}, [r0, :128]!

.L_copy_less_than_16_unknown_align:
        // Copy up to 15 bytes (count in r2).
        movs        ip, r2, lsl #29
        bcc         1f
        vld1.8      {d0}, [r1]!
        vst1.8      {d0}, [r0]!
1:      bge         2f
        vld4.8      {d0[0], d1[0], d2[0], d3[0]}, [r1]!
        vst4.8      {d0[0], d1[0], d2[0], d3[0]}, [r0]!

2:      // Copy 0 to 4 bytes.
        lsls        r2, r2, #31
        itt         ne
        ldrbne      lr, [r1], #1
        strbne      lr, [r0], #1
        itttt       cs
        ldrbcs      ip, [r1], #1
        ldrbcs      lr, [r1]
        strbcs      ip, [r0], #1
        strbcs      lr, [r0]

        pop         {r0, pc}
