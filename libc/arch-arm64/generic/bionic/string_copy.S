/*
 * Copyright (C) 2014 The Android Open Source Project
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
   Copyright (c) 2014, Linaro Limited
   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:
       * Redistributions of source code must retain the above copyright
         notice, this list of conditions and the following disclaimer.
       * Redistributions in binary form must reproduce the above copyright
         notice, this list of conditions and the following disclaimer in the
         documentation and/or other materials provided with the distribution.
       * Neither the name of the Linaro nor the
         names of its contributors may be used to endorse or promote products
         derived from this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
   HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
   LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
   DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
   THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
   OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/* Assumptions:
 *
 * ARMv8-a, AArch64
 */

#if !defined(STPCPY) && !defined(STRCPY)
#error "Either STPCPY or STRCPY must be defined."
#endif

#include <private/bionic_asm.h>

/* Arguments and results.  */
#if defined(STPCPY)
#define dst         x0
#elif defined(STRCPY)
#define dstin       x0
#endif
#define src         x1

/* Locals and temporaries.  */
#if defined(STRCPY)
#define dst         x2
#endif
#define data1       x3
#define data1_w     w3
#define data2       x4
#define data2_w     w4
#define has_nul1    x5
#define has_nul1_w  w5
#define has_nul2    x6
#define tmp1        x7
#define tmp2        x8
#define tmp3        x9
#define tmp4        x10
#define zeroones    x11
#define zeroones_w  w11
#define pos         x12

#define REP8_01 0x0101010101010101
#define REP8_7f 0x7f7f7f7f7f7f7f7f
#define REP8_80 0x8080808080808080

#if defined(STPCPY)
ENTRY(stpcpy)
#elif defined(STRCPY)
ENTRY(strcpy)
#endif
    mov     zeroones, #REP8_01
#if defined(STRCPY)
    mov     dst, dstin
#endif
    ands    tmp1, src, #15
    b.ne    .Lmisaligned
    // NUL detection works on the principle that (X - 1) & (~X) & 0x80
    // (=> (X - 1) & ~(X | 0x7f)) is non-zero iff a byte is zero, and
    // can be done in parallel across the entire word.
    // The inner loop deals with two Dwords at a time.  This has a
    // slightly higher start-up cost, but we should win quite quickly,
    // especially on cores with a high number of issue slots per
    // cycle, as we get much better parallelism out of the operations.
.Lloop:
    ldp     data1, data2, [src], #16
    sub     tmp1, data1, zeroones
    orr     tmp2, data1, #REP8_7f
    bic     has_nul1, tmp1, tmp2
    cbnz    has_nul1, .Lnul_in_data1
    sub     tmp3, data2, zeroones
    orr     tmp4, data2, #REP8_7f
    bic     has_nul2, tmp3, tmp4
    cbnz    has_nul2, .Lnul_in_data2
    // No NUL in either register, copy it in a single instruction.
    stp     data1, data2, [dst], #16
    b       .Lloop

.Lnul_in_data1:
    rev     has_nul1, has_nul1
    clz     pos, has_nul1
    add     tmp1, pos, #0x8

    tbz     tmp1, #6, 1f
#if defined(STPCPY)
    str     data1, [dst], #7
#elif defined(STRCPY)
    str     data1, [dst]
#endif
    ret
1:
    tbz     tmp1, #5, 1f
    str     data1_w, [dst], #4
    lsr     data1, data1, #32
1:
    tbz     tmp1, #4, 1f
    strh    data1_w, [dst], #2
    lsr     data1, data1, #16
1:
    tbz     tmp1, #3, 1f
    strb    data1_w, [dst]
#if defined(STPCPY)
    ret
#endif
1:
#if defined(STPCPY)
    // Back up one so that dst points to the '\0' string terminator.
    sub     dst, dst, #1
#endif
    ret

.Lnul_in_data2:
    str     data1, [dst], #8
    rev     has_nul2, has_nul2
    clz     pos, has_nul2
    add     tmp1, pos, #0x8

    tbz     tmp1, #6, 1f
#if defined(STPCPY)
    str     data2, [dst], #7
#elif defined(STRCPY)
    str     data2, [dst]
#endif
    ret
1:
    tbz     tmp1, #5, 1f
    str     data2_w, [dst], #4
    lsr     data2, data2, #32
1:
    tbz     tmp1, #4, 1f
    strh    data2_w, [dst], #2
    lsr     data2, data2, #16
1:
    tbz     tmp1, #3, 1f
    strb    data2_w, [dst]
#if defined(STPCPY)
    ret
#endif
1:
#if defined(STPCPY)
    // Back up one so that dst points to the '\0' string terminator.
    sub     dst, dst, #1
#endif
    ret

.Lmisaligned:
    tbz     src, #0, 1f
    ldrb    data1_w, [src], #1
    strb    data1_w, [dst], #1
    cbnz    data1_w, 1f
#if defined(STPCPY)
    // Back up one so that dst points to the '\0' string terminator.
    sub     dst, dst, #1
#endif
    ret
1:
    tbz     src, #1, 1f
    ldrb    data1_w, [src], #1
    strb    data1_w, [dst], #1
    cbz     data1_w, .Ldone
    ldrb    data2_w, [src], #1
    strb    data2_w, [dst], #1
    cbnz    data2_w, 1f
.Ldone:
#if defined(STPCPY)
    // Back up one so that dst points to the '\0' string terminator.
    sub     dst, dst, #1
#endif
    ret
1:
    tbz     src, #2, 1f
    ldr     data1_w, [src], #4
    // Check for a zero.
    sub     has_nul1_w, data1_w, zeroones_w
    bic     has_nul1_w, has_nul1_w, data1_w
    ands    has_nul1_w, has_nul1_w, #0x80808080
    b.ne    .Lnul_in_data1
    str     data1_w, [dst], #4
1:
    tbz     src, #3, .Lloop
    ldr     data1, [src], #8
    // Check for a zero.
    sub     tmp1, data1, zeroones
    orr     tmp2, data1, #REP8_7f
    bics    has_nul1, tmp1, tmp2
    b.ne    .Lnul_in_data1
    str     data1, [dst], #8
    b       .Lloop
#if defined(STPCPY)
END(stpcpy)
#elif defined(STRCPY)
END(strcpy)
#endif
