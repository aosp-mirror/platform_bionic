/*
Copyright (c) 2014, Intel Corporation
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice,
    * this list of conditions and the following disclaimer.

    * Redistributions in binary form must reproduce the above copyright notice,
    * this list of conditions and the following disclaimer in the documentation
    * and/or other materials provided with the distribution.

    * Neither the name of Intel Corporation nor the names of its contributors
    * may be used to endorse or promote products derived from this software
    * without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/******************************************************************************/
//                     ALGORITHM DESCRIPTION
//                     ---------------------
//
// X87 version:
// Use 80-bit FPU precision fmul, fsqrt to compute square and sqrt.
//
// SSE version:
// Swap x, y if |x|<|y|
// For x=2^k*x, get y=y*2^(-k)
// Get S ~ sqrt(x^2+y^2)  (leading 1 + leading 25 mantissa bits)
//
// Get D = ( RN(x^2+y^2) - S^2 ) + ( x^2 - RN(x^2) ) +
//                               + ( y^2 - ((RN(x^2+y^2)-RN(x^2)) )
//
// Result is 2^k*(S + Se),  where Se = S*e
//        S*e is approximated as (D/2S)*( 1 - (D/2S)^2*1.0/S )
//
// Return 2^k*(S+Se)
//
// For |y/x|<2^(-64), return x
//
// For cases where maximum biased exponent is either greater than 7fdh or
// below 32, take a special path to check for special cases (0, NaN, Inf),
// possible overflow, and more accurate computation for denormal results
//
// Special cases:
//  hypot(x,y), hypot(y,x), and hypot(x,-y) are equivalent
//  hypot(x,+-0) is equivalent to fabs(x)
//  hypot(x,y) = y if (x==NaN or x==INF) and y==INF
//  hypot(x,y) = x if (x==NaN or x==INF) and y!=INF (even if y==NaN!)
//  hypot(x,y) = y if (x!=NaN and x!=INF) and (y==NaN or y==INF)
//
/******************************************************************************/

#include <private/bionic_asm.h>
# -- Begin  hypot
ENTRY(hypot)
# parameter 1: %xmm0
# parameter 2: %xmm1
..B1.1:
..___tag_value_hypot.1:
..___tag_value_hypot.3:
..B1.2:
        subq      $64, %rsp
        movapd    static_const_table(%rip), %xmm3
        movsd     %xmm0, 48(%rsp)
        movsd     %xmm1, 56(%rsp)
        andpd     %xmm3, %xmm0
        andpd     %xmm3, %xmm1
        pextrw    $3, %xmm0, %eax
        pextrw    $3, %xmm1, %edx
        cmpl      $24528, %eax
        ja        .L_2TAG_PACKET_0.0.1
        cmpl      $24528, %edx
        ja        .L_2TAG_PACKET_0.0.1
.L_2TAG_PACKET_1.0.1:
        fldl      48(%rsp)
        fldl      56(%rsp)
        fxch      %st(1)
        fmul      %st(0), %st
        fxch      %st(1)
        nop       
        fmul      %st(0), %st
        faddp     %st, %st(1)
        fsqrt     
        jmp       .L_2TAG_PACKET_2.0.1
.L_2TAG_PACKET_0.0.1:
        cmpl      $32752, %eax
        movl      %eax, %ecx
        jae       .L_2TAG_PACKET_3.0.1
        subl      %edx, %ecx
        cmpl      $32752, %edx
        jae       .L_2TAG_PACKET_3.0.1
        addl      $928, %ecx
        addl      %edx, %eax
        cmpl      $1856, %ecx
        ja        .L_2TAG_PACKET_4.0.1
        cmpl      $49056, %eax
        jb        .L_2TAG_PACKET_1.0.1
        fldl      48(%rsp)
        fldl      56(%rsp)
        fxch      %st(1)
        fmul      %st(0), %st
        fxch      %st(1)
        nop       
        fmul      %st(0), %st
        faddp     %st, %st(1)
        fsqrt     
.L_2TAG_PACKET_5.0.1:
        fstl      (%rsp)
        fstpt     16(%rsp)
        xorl      %eax, %eax
        movw      24(%rsp), %ax
        cmpl      $17407, %eax
        jae       .L_2TAG_PACKET_6.0.1
        fldl      (%rsp)
        jmp       .L_2TAG_PACKET_7.0.1
.L_2TAG_PACKET_4.0.1:
        movsd     %xmm0, 32(%rsp)
        movsd     %xmm1, 40(%rsp)
        fldl      32(%rsp)
        faddl     40(%rsp)
        jmp       .L_2TAG_PACKET_5.0.1
.L_2TAG_PACKET_6.0.1:
        fldl      (%rsp)
        jmp       .L_2TAG_PACKET_7.0.1
.L_2TAG_PACKET_3.0.1:
        shufpd    $0, %xmm1, %xmm0
        movdqa    %xmm0, %xmm2
        movdqa    16+static_const_table(%rip), %xmm3
        movsd     %xmm0, 32(%rsp)
        movsd     %xmm1, 40(%rsp)
        cmppd     $3, %xmm0, %xmm2
        cmppd     $0, %xmm0, %xmm3
        movmskpd  %xmm2, %edx
        movmskpd  %xmm3, %rax
        testl     %edx, %edx
        je        .L_2TAG_PACKET_8.0.1
        fldl      32(%rsp)
        fmull     40(%rsp)
        testq     $1, %rax
        jne       .L_2TAG_PACKET_9.0.1
        testq     $2, %rax
        jne       .L_2TAG_PACKET_10.0.1
        jmp       .L_2TAG_PACKET_2.0.1
.L_2TAG_PACKET_8.0.1:
        fldl      32(%rsp)
        faddl     40(%rsp)
        jmp       .L_2TAG_PACKET_2.0.1
.L_2TAG_PACKET_9.0.1:
        fstpl     40(%rsp)
        fldl      32(%rsp)
        jmp       .L_2TAG_PACKET_7.0.1
.L_2TAG_PACKET_10.0.1:
        fstpl     32(%rsp)
        fldl      40(%rsp)
        jmp       .L_2TAG_PACKET_7.0.1
.L_2TAG_PACKET_2.0.1:
.L_2TAG_PACKET_7.0.1:
        fstpl     16(%rsp)
        movq      16(%rsp), %xmm0
        addq      $64, %rsp
        ret       
..B1.3:
..___tag_value_hypot.4:
END(hypot)
# -- End  hypot
	.section .rodata, "a"
	.align 16
	.align 16
static_const_table:
	.long	4294967295
	.long	2147483647
	.long	4294967295
	.long	2147483647
	.long	0
	.long	2146435072
	.long	0
	.long	2146435072
	.type	static_const_table,@object
	.size	static_const_table,32
	.data
	.section .note.GNU-stack, ""
// -- Begin DWARF2 SEGMENT .eh_frame
	.section .eh_frame,"a",@progbits
.eh_frame_seg:
	.align 1
	.4byte 0x00000014
	.8byte 0x00527a0100000000
	.8byte 0x08070c1b01107801
	.4byte 0x00000190
	.4byte 0x00000014
	.4byte 0x0000001c
	.4byte ..___tag_value_hypot.1-.
	.4byte ..___tag_value_hypot.4-..___tag_value_hypot.1
	.2byte 0x0400
	.4byte ..___tag_value_hypot.3-..___tag_value_hypot.1
	.2byte 0x100e
# End
