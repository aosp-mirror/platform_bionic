/*
 * Copyright (c) 2013-2014, NVIDIA Corporation.  All rights reserved.
 * Johnny Qiu <joqiu@nvidia.com>
 * Shu Zhang <chazhang@nvidia.com>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer in the documentation and/or other materials provided
 *       with the distribution.
 *     * Neither the name of The Linux Foundation nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 * IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <float.h>
#include <private/bionic_asm.h>

ENTRY(floor)    /* x in r0, r1 */

        and             r3, r1, #0x80000000     /* sign(x) */
        bic             r1, r1, #0x80000000     /* x = abs(x) */

        /* extract exp of x */
        lsr             r2, r1, #20
        sub             r2, r2, #0x3fc
        subs            r2, r2, #0x3            /* r2 <- exp */

        /* |x| < 1.0? */
        blt             .Lx_lt_one

        /* x < 0? */
        cmp             r3, #0
        bne             .Lclr_frac_neg

        /* |x| <= 2^20? */
        cmp             r2, #20
        ble             .Lclr_frac_r1

        /* |x| < 2^52? */
        cmp             r2, #52
        blt             .Lclr_frac_r0

        /* return x */
        bx              lr

.Lclr_frac_r1:
        rsb             r2, r2, #20
        lsr             r1, r1, r2
        lsl             r1, r1, r2
        mov             r0, #0
        bx              lr

.Lclr_frac_r0:
        rsb             r2, r2, #52
        lsr             r0, r0, r2
        lsl             r0, r0, r2
        bx              lr

.Lclr_frac_neg:
        /* |x| <= 2^20? */
        cmp             r2, #20
        ble             .Lclr_frac_r1_neg

        /* |x| < 2^52? */
        cmp             r2, #52
        blt             .Lclr_frac_r0_neg

        /* return x */
        orr             r1, r1, #0x80000000
        bx              lr

.Lclr_frac_r1_neg:
        rsb             r2, r2, #20
        mov             r3, #1
        lsl             r3, r3, r2
        sub             r3, r3, #1
        and             r3, r1, r3
        orr             r3, r3, r0
        lsr             r1, r1, r2
        lsl             r1, r1, r2
        mov             r0, #0
        b               .Lreturn_x_neg

.Lclr_frac_r0_neg:
        rsb             r2, r2, #52
        mov             r3, #1
        lsl             r3, r3, r2
        sub             r3, r3, #1
        and             r3, r0, r3
        lsr             r0, r0, r2
        lsl             r0, r0, r2
        b               .Lreturn_x_neg

.Lx_lt_one:
        /* x == +-0? */
        cmp             r0, #0
        cmpeq           r1, #0
        orreq           r1, r1, r3
        bxeq            lr

        /* (x > 0) ? 0 : -1 */
        mov             r1, #0x00100000
        mov             r0, #0
        cmp             r3, #0
        movne           r1, #0xc0000000
        sub             r1, r1, #0x00100000
        bx              lr

.Lreturn_x_neg:
        cmp             r3, #0
        orr             r1, r1, #0x80000000
        bxeq            lr

        vmov            d16, r0, r1
        vmov.f64        d18, #1.0
        vsub.f64        d16, d16, d18
        vmov            r0, r1, d16
        bx              lr

END(floor)

#if LDBL_MANT_DIG == 53
        .weak           floorl
        .equ            floorl,floor
#endif
