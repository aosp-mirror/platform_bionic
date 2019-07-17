/*
Copyright (C) 2019 The Android Open Source Project
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:
 * Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in
   the documentation and/or other materials provided with the
   distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
SUCH DAMAGE.
*/

#include <private/bionic_asm.h>

#ifndef WMEMSET
 #define WMEMSET wmemset_avx2
#endif

        .section .text.avx2,"ax",@progbits

ENTRY (WMEMSET)
# BB#0:
	testq	%rdx, %rdx
	je	.LBB0_14
# BB#1:
	cmpq	$32, %rdx
	jae	.LBB0_3
# BB#2:
	xorl	%r8d, %r8d
	movq	%rdi, %rax
	jmp	.LBB0_12
.LBB0_3:
	movq	%rdx, %r8
	andq	$-32, %r8
	vmovd	%esi, %xmm0
	vpbroadcastd	%xmm0, %ymm0
	leaq	-32(%r8), %rcx
	movq	%rcx, %rax
	shrq	$5, %rax
	leal	1(%rax), %r9d
	andl	$7, %r9d
	cmpq	$224, %rcx
	jae	.LBB0_5
# BB#4:
	xorl	%eax, %eax
	testq	%r9, %r9
	jne	.LBB0_8
	jmp	.LBB0_10
.LBB0_5:
	leaq	992(%rdi), %rcx
	leaq	-1(%r9), %r10
	subq	%rax, %r10
	xorl	%eax, %eax
	.p2align	4, 0x90
.LBB0_6:                                # =>This Inner Loop Header: Depth=1
	vmovdqu	%ymm0, -992(%rcx,%rax,4)
	vmovdqu	%ymm0, -960(%rcx,%rax,4)
	vmovdqu	%ymm0, -928(%rcx,%rax,4)
	vmovdqu	%ymm0, -896(%rcx,%rax,4)
	vmovdqu	%ymm0, -864(%rcx,%rax,4)
	vmovdqu	%ymm0, -832(%rcx,%rax,4)
	vmovdqu	%ymm0, -800(%rcx,%rax,4)
	vmovdqu	%ymm0, -768(%rcx,%rax,4)
	vmovdqu	%ymm0, -736(%rcx,%rax,4)
	vmovdqu	%ymm0, -704(%rcx,%rax,4)
	vmovdqu	%ymm0, -672(%rcx,%rax,4)
	vmovdqu	%ymm0, -640(%rcx,%rax,4)
	vmovdqu	%ymm0, -608(%rcx,%rax,4)
	vmovdqu	%ymm0, -576(%rcx,%rax,4)
	vmovdqu	%ymm0, -544(%rcx,%rax,4)
	vmovdqu	%ymm0, -512(%rcx,%rax,4)
	vmovdqu	%ymm0, -480(%rcx,%rax,4)
	vmovdqu	%ymm0, -448(%rcx,%rax,4)
	vmovdqu	%ymm0, -416(%rcx,%rax,4)
	vmovdqu	%ymm0, -384(%rcx,%rax,4)
	vmovdqu	%ymm0, -352(%rcx,%rax,4)
	vmovdqu	%ymm0, -320(%rcx,%rax,4)
	vmovdqu	%ymm0, -288(%rcx,%rax,4)
	vmovdqu	%ymm0, -256(%rcx,%rax,4)
	vmovdqu	%ymm0, -224(%rcx,%rax,4)
	vmovdqu	%ymm0, -192(%rcx,%rax,4)
	vmovdqu	%ymm0, -160(%rcx,%rax,4)
	vmovdqu	%ymm0, -128(%rcx,%rax,4)
	vmovdqu	%ymm0, -96(%rcx,%rax,4)
	vmovdqu	%ymm0, -64(%rcx,%rax,4)
	vmovdqu	%ymm0, -32(%rcx,%rax,4)
	vmovdqu	%ymm0, (%rcx,%rax,4)
	addq	$256, %rax              # imm = 0x100
	addq	$8, %r10
	jne	.LBB0_6
# BB#7:
	testq	%r9, %r9
	je	.LBB0_10
.LBB0_8:
	leaq	(%rdi,%rax,4), %rax
	addq	$96, %rax
	negq	%r9
	.p2align	4, 0x90
.LBB0_9:                                # =>This Inner Loop Header: Depth=1
	vmovdqu	%ymm0, -96(%rax)
	vmovdqu	%ymm0, -64(%rax)
	vmovdqu	%ymm0, -32(%rax)
	vmovdqu	%ymm0, (%rax)
	subq	$-128, %rax
	addq	$1, %r9
	jne	.LBB0_9
.LBB0_10:
	cmpq	%rdx, %r8
	je	.LBB0_14
# BB#11:
	leaq	(%rdi,%r8,4), %rax
.LBB0_12:
	subq	%r8, %rdx
	.p2align	4, 0x90
.LBB0_13:                               # =>This Inner Loop Header: Depth=1
	movl	%esi, (%rax)
	addq	$4, %rax
	addq	$-1, %rdx
	jne	.LBB0_13
.LBB0_14:
	movq	%rdi, %rax
	vzeroupper
	retq
END(WMEMSET)
