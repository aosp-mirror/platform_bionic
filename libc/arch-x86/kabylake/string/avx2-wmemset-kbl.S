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

ENTRY(WMEMSET)
# BB#0:
	pushl	%ebp
	pushl	%ebx
	pushl	%edi
	pushl	%esi
	pushl	%eax
	movl	32(%esp), %ecx
	movl	24(%esp), %eax
	testl	%ecx, %ecx
	je	.LBB0_12
# BB#1:
	movl	28(%esp), %edx
	xorl	%edi, %edi
	movl	%eax, %esi
	cmpl	$32, %ecx
	jb	.LBB0_10
# BB#2:
	movl	%ecx, %eax
	andl	$-32, %eax
	vmovd	%edx, %xmm0
	vpbroadcastd	%xmm0, %ymm0
	movl	%eax, (%esp)            # 4-byte Spill
	leal	-32(%eax), %esi
	movl	%esi, %eax
	shrl	$5, %eax
	leal	1(%eax), %edi
	andl	$7, %edi
	xorl	%ebx, %ebx
	cmpl	$224, %esi
	jb	.LBB0_5
# BB#3:
	movl	24(%esp), %esi
	leal	992(%esi), %ebp
	leal	-1(%edi), %esi
	subl	%eax, %esi
	xorl	%ebx, %ebx
	.p2align	4, 0x90
.LBB0_4:                                # =>This Inner Loop Header: Depth=1
	vmovdqu	%ymm0, -992(%ebp,%ebx,4)
	vmovdqu	%ymm0, -960(%ebp,%ebx,4)
	vmovdqu	%ymm0, -928(%ebp,%ebx,4)
	vmovdqu	%ymm0, -896(%ebp,%ebx,4)
	vmovdqu	%ymm0, -864(%ebp,%ebx,4)
	vmovdqu	%ymm0, -832(%ebp,%ebx,4)
	vmovdqu	%ymm0, -800(%ebp,%ebx,4)
	vmovdqu	%ymm0, -768(%ebp,%ebx,4)
	vmovdqu	%ymm0, -736(%ebp,%ebx,4)
	vmovdqu	%ymm0, -704(%ebp,%ebx,4)
	vmovdqu	%ymm0, -672(%ebp,%ebx,4)
	vmovdqu	%ymm0, -640(%ebp,%ebx,4)
	vmovdqu	%ymm0, -608(%ebp,%ebx,4)
	vmovdqu	%ymm0, -576(%ebp,%ebx,4)
	vmovdqu	%ymm0, -544(%ebp,%ebx,4)
	vmovdqu	%ymm0, -512(%ebp,%ebx,4)
	vmovdqu	%ymm0, -480(%ebp,%ebx,4)
	vmovdqu	%ymm0, -448(%ebp,%ebx,4)
	vmovdqu	%ymm0, -416(%ebp,%ebx,4)
	vmovdqu	%ymm0, -384(%ebp,%ebx,4)
	vmovdqu	%ymm0, -352(%ebp,%ebx,4)
	vmovdqu	%ymm0, -320(%ebp,%ebx,4)
	vmovdqu	%ymm0, -288(%ebp,%ebx,4)
	vmovdqu	%ymm0, -256(%ebp,%ebx,4)
	vmovdqu	%ymm0, -224(%ebp,%ebx,4)
	vmovdqu	%ymm0, -192(%ebp,%ebx,4)
	vmovdqu	%ymm0, -160(%ebp,%ebx,4)
	vmovdqu	%ymm0, -128(%ebp,%ebx,4)
	vmovdqu	%ymm0, -96(%ebp,%ebx,4)
	vmovdqu	%ymm0, -64(%ebp,%ebx,4)
	vmovdqu	%ymm0, -32(%ebp,%ebx,4)
	vmovdqu	%ymm0, (%ebp,%ebx,4)
	addl	$256, %ebx              # imm = 0x100
	addl	$8, %esi
	jne	.LBB0_4
.LBB0_5:
	testl	%edi, %edi
	movl	24(%esp), %eax
	je	.LBB0_8
# BB#6:
	leal	(%eax,%ebx,4), %esi
	addl	$96, %esi
	negl	%edi
	.p2align	4, 0x90
.LBB0_7:                                # =>This Inner Loop Header: Depth=1
	vmovdqu	%ymm0, -96(%esi)
	vmovdqu	%ymm0, -64(%esi)
	vmovdqu	%ymm0, -32(%esi)
	vmovdqu	%ymm0, (%esi)
	subl	$-128, %esi
	addl	$1, %edi
	jne	.LBB0_7
.LBB0_8:
	movl	(%esp), %edi            # 4-byte Reload
	cmpl	%ecx, %edi
	je	.LBB0_12
# BB#9:
	leal	(%eax,%edi,4), %esi
.LBB0_10:
	subl	%edi, %ecx
	.p2align	4, 0x90
.LBB0_11:                               # =>This Inner Loop Header: Depth=1
	movl	%edx, (%esi)
	addl	$4, %esi
	addl	$-1, %ecx
	jne	.LBB0_11
.LBB0_12:
	addl	$4, %esp
	popl	%esi
	popl	%edi
	popl	%ebx
	popl	%ebp
	vzeroupper
	retl
END(WMEMSET)
