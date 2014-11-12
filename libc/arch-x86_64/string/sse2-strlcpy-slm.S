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

#ifndef L
# define L(label)	.L##label
#endif

#ifndef cfi_startproc
# define cfi_startproc	.cfi_startproc
#endif

#ifndef cfi_endproc
# define cfi_endproc	.cfi_endproc
#endif

#ifndef ENTRY
# define ENTRY(name)	\
	.type name, @function;	\
	.globl name;	\
	.p2align 4;	\
name:	\
	cfi_startproc
#endif

#ifndef END
# define END(name)	\
       cfi_endproc;	\
       .size name, .-name
#endif


#ifndef STRLCPY
# define STRLCPY	strlcpy
#endif

#define JMPTBL(I, B)	I - B
#define BRANCH_TO_JMPTBL_ENTRY(TABLE, INDEX, SCALE)	\
	lea	TABLE(%rip), %r11;	\
	movslq	(%r11, INDEX, SCALE), %rcx;	\
	lea	(%r11, %rcx), %rcx;	\
	jmp	*%rcx

#define RETURN	\
	add	%r9, %rax;	\
	ret

.text
ENTRY (STRLCPY)
	xor	%rax, %rax
	xor	%r9, %r9
	mov	%rdx, %r8
	cmp	$0, %r8
	jz	L(CalculateSrcLen)

#ifdef USE_AS_STRLCAT
	xor	%rcx, %rcx
	pxor	%xmm0, %xmm0

	movdqu	(%rdi), %xmm1
	pcmpeqb %xmm1, %xmm0
	pmovmskb %xmm0, %rdx

	cmp	$17, %r8
	jb	L(SizeEndCase1)
	test	%rdx, %rdx
	jnz	L(StringEndCase1)

	add	$16, %rax
	movdqu	16(%rdi), %xmm1
	pcmpeqb %xmm1, %xmm0
	pmovmskb %xmm0, %rdx

	cmp	$33, %r8
	jb	L(SizeEndCase1)
	test	%rdx, %rdx
	jnz	L(StringEndCase1)

	mov	%rdi, %rcx
	and	$15, %rcx
	and	$-16, %rdi

	add	%rcx, %r8
	sub	$16, %r8

L(DstLenLoop):
	movdqa	(%rdi, %rax), %xmm1
	pcmpeqb %xmm1, %xmm0
	pmovmskb %xmm0, %rdx
	sub	$16, %r8
	jbe	L(SizeEndCase2)
	test	%rdx, %rdx
	jnz	L(StringEndCase2)
	add	$16, %rax
	jmp	L(DstLenLoop)

L(StringEndCase2):
	add	$16, %r8
	bsf	%rdx, %rdx
	sub	%rdx, %r8
	add	%rdx, %rax
	sub	%rcx, %r9
	add	%rax, %rdi
	jmp	 L(CopySrcString)

L(SizeEndCase1):
	test	%rdx, %rdx
	jz	L(SizeEnd)
	bsf	%rdx, %rdx
	add	%rdx, %rax
	cmp	%r8, %rax
	jb	L(StringEnd)
L(SizeEnd):
	mov	%r8, %r9
	jmp	L(CalculateSrcLenCase1)

L(SizeEndCase2):
	add	$16, %r8
	test	%rdx, %rdx
	jz	L(StringEndCase4)
	bsf	%rdx, %rdx
	cmp	%r8, %rdx
	jb	L(StringEndCase3)
L(StringEndCase4):
	add	%r8, %rax
	sub	%rcx, %rax
	mov	%rax, %r9
	jmp	L(CalculateSrcLenCase1)

L(StringEndCase3):
	add	%rdx, %rax
	sub	%rcx, %r9
	add	%rax, %rdi
	sub	%rdx, %r8
	jmp	L(CopySrcString)

L(StringEndCase1):
	bsf	%rdx, %rdx
	add	%rdx, %rax
	sub	%rcx, %rax
L(StringEnd):
	add	%rax, %rdi
	sub	%rax, %r8
#endif

	mov	%rsi, %rcx
	and	$63, %rcx
	cmp	$32, %rcx
	jbe	L(CopySrcString)

	and	$-16, %rsi
	and	$15, %rcx
	pxor	%xmm0, %xmm0
	pxor	%xmm1, %xmm1

	pcmpeqb	(%rsi), %xmm1
	pmovmskb %xmm1, %rdx
	shr	%cl, %rdx
	mov	$16, %r10
	sub	%rcx, %r10
	cmp	%r10, %r8
	jbe	L(CopyFrom1To16BytesTailCase2OrCase3)
	test	%rdx, %rdx
	jnz	L(CopyFrom1To16BytesTail)

	pcmpeqb	16(%rsi), %xmm0
	pmovmskb %xmm0, %rdx
	add	$16, %r10
	cmp	%r10, %r8
	jbe	L(CopyFrom1To32BytesCase2OrCase3)
	test	%rdx, %rdx
	jnz	L(CopyFrom1To32Bytes)

	movdqu	(%rsi, %rcx), %xmm1
	movdqu	%xmm1, (%rdi)
#ifdef USE_AS_STRLCAT
	add	%rax, %r9
#endif
	jmp	L(LoopStart)

	.p2align 4
L(CopySrcString):
#ifdef USE_AS_STRLCAT
	add	%rax, %r9
	xor	%rax, %rax
#endif
	pxor	%xmm0, %xmm0
	movdqu	(%rsi), %xmm1
	pcmpeqb	%xmm1, %xmm0
	pmovmskb %xmm0, %rdx

	cmp	$17, %r8
	jb	L(CopyFrom1To16BytesTail1Case2OrCase3)
	test	%rdx, %rdx
	jnz	L(CopyFrom1To16BytesTail1)

	movdqu	16(%rsi), %xmm2
	pcmpeqb	%xmm2, %xmm0
	movdqu	%xmm1, (%rdi)
	pmovmskb %xmm0, %rdx
	add	$16, %rax

	cmp	$33, %r8
	jb	L(CopyFrom1To32Bytes1Case2OrCase3)
	test	%rdx, %rdx
	jnz	L(CopyFrom1To32Bytes1)

	mov	%rsi, %rcx
	and	$15, %rcx
	and	$-16, %rsi

L(LoopStart):
	sub	%rcx, %rdi
	add	%rcx, %r8
	sub	$16, %r8
	mov	$16, %rax

L(16Loop):
	movdqa	(%rsi, %rax), %xmm1
	pcmpeqb	%xmm1, %xmm0
	pmovmskb %xmm0, %rdx
	sub	$16, %r8
	jbe	L(CopyFrom1To16BytesCase2OrCase3)
	test	%rdx, %rdx
	jnz	L(CopyFrom1To16BytesXmmExit)
	movdqu	%xmm1, (%rdi, %rax)
	add	$16, %rax
	jmp	L(16Loop)

/*------End of main part with loops---------------------*/

/* Case1 */
	.p2align 4
L(CopyFrom1To16Bytes):
	add	%rcx, %rdi
	add	%rcx, %rsi
	bsf	%rdx, %rdx
	add	%rdx, %rax
	BRANCH_TO_JMPTBL_ENTRY (L(ExitStringTailTable), %rdx, 4)

	.p2align 4
L(CopyFrom1To16BytesTail):
	add	%rcx, %rsi
	bsf	%rdx, %rdx
	add	%rdx, %rax
	BRANCH_TO_JMPTBL_ENTRY (L(ExitStringTailTable), %rdx, 4)

	.p2align 4
L(CopyFrom1To32Bytes1):
	add	$16, %rsi
	add	$16, %rdi
	sub	$16, %r8
L(CopyFrom1To16BytesTail1):
	bsf	%rdx, %rdx
	add	%rdx, %rax
	BRANCH_TO_JMPTBL_ENTRY (L(ExitStringTailTable), %rdx, 4)

	.p2align 4
L(CopyFrom1To32Bytes):
	bsf	%rdx, %rdx
	add	%rcx, %rsi
	add	$16, %rdx
	sub	%rcx, %rdx
	add	%rdx, %rax
	BRANCH_TO_JMPTBL_ENTRY (L(ExitStringTailTable), %rdx, 4)

	.p2align 4
L(CopyFrom1To16BytesExit):
	add	%rdx, %rax
	BRANCH_TO_JMPTBL_ENTRY (L(ExitStringTailTable), %rdx, 4)

/* Case2 */

	.p2align 4
L(CopyFrom1To16BytesCase2):
	add	$16, %r8
	add	%rax, %rdi
	add	%rax, %rsi
	bsf	%rdx, %rdx
	sub	%rcx, %rax
	cmp	%r8, %rdx
	jb	L(CopyFrom1To16BytesExit)
	add	%r8, %rax
	BRANCH_TO_JMPTBL_ENTRY (L(ExitTable), %r8, 4)

	.p2align 4
L(CopyFrom1To32BytesCase2):
	add	%rcx, %rsi
	bsf	%rdx, %rdx
	add	$16, %rdx
	sub	%rcx, %rdx
	cmp	%r8, %rdx
	jb	L(CopyFrom1To16BytesExit)
	add	%r8, %rax
	BRANCH_TO_JMPTBL_ENTRY (L(ExitTable), %r8, 4)

L(CopyFrom1To16BytesTailCase2):
	add	%rcx, %rsi
	bsf	%rdx, %rdx
	cmp	%r8, %rdx
	jb	L(CopyFrom1To16BytesExit)
	add	%r8, %rax
	BRANCH_TO_JMPTBL_ENTRY (L(ExitTable), %r8, 4)

	.p2align 4
L(CopyFrom1To16BytesTail1Case2):
	bsf	%rdx, %rdx
	cmp	%r8, %rdx
	jb	L(CopyFrom1To16BytesExit)
	add	%r8, %rax
	BRANCH_TO_JMPTBL_ENTRY (L(ExitTable), %r8, 4)

/* Case2 or Case3,  Case3 */

	.p2align 4
L(CopyFrom1To16BytesCase2OrCase3):
	test	%rdx, %rdx
	jnz	L(CopyFrom1To16BytesCase2)
	add	$16, %r8
	add	%rax, %rdi
	add	%rax, %rsi
	add	%r8, %rax
	sub	%rcx, %rax
	BRANCH_TO_JMPTBL_ENTRY (L(ExitTable), %r8, 4)

	.p2align 4
L(CopyFrom1To32BytesCase2OrCase3):
	test	%rdx, %rdx
	jnz	L(CopyFrom1To32BytesCase2)
	add	%rcx, %rsi
	add	%r8, %rax
	BRANCH_TO_JMPTBL_ENTRY (L(ExitTable), %r8, 4)

	.p2align 4
L(CopyFrom1To16BytesTailCase2OrCase3):
	test	%rdx, %rdx
	jnz	L(CopyFrom1To16BytesTailCase2)
	add	%rcx, %rsi
	add	%r8, %rax
	BRANCH_TO_JMPTBL_ENTRY (L(ExitTable), %r8, 4)

	.p2align 4
L(CopyFrom1To32Bytes1Case2OrCase3):
	add	$16, %rdi
	add	$16, %rsi
	sub	$16, %r8
L(CopyFrom1To16BytesTail1Case2OrCase3):
	test	%rdx, %rdx
	jnz	L(CopyFrom1To16BytesTail1Case2)
	add	%r8, %rax
	BRANCH_TO_JMPTBL_ENTRY (L(ExitTable), %r8, 4)

	.p2align 4
L(CopyFrom1To16BytesXmmExit):
	bsf	%rdx, %rdx
	add	%rax, %rdi
	add	%rax, %rsi
	add	%rdx, %rax
	sub	%rcx, %rax
	BRANCH_TO_JMPTBL_ENTRY (L(ExitStringTailTable), %rdx, 4)

/*------------End labels regarding with copying 1-16 bytes--and 1-32 bytes----*/


	.p2align 4
L(Exit0):
	RETURN

	.p2align 4
L(Exit1):
	movb	$0, (%rdi)
	jmp	L(CalculateSrcLen)

	.p2align 4
L(Exit2):
	movb	(%rsi), %dh
	movb	%dh, (%rdi)
	movb	$0, 1(%rdi)
	jmp	L(CalculateSrcLen)

	.p2align 4
L(Exit3):
	movw	(%rsi), %dx
	movw	%dx, (%rdi)
	movb	$0, 2(%rdi)
	jmp	L(CalculateSrcLen)

	.p2align 4
L(Exit4):
	movw	(%rsi), %cx
	movb	2(%rsi), %dh
	movw	%cx, (%rdi)
	movb	%dh, 2(%rdi)
	movb	$0, 3(%rdi)
	jmp	L(CalculateSrcLen)

	.p2align 4
L(Exit5):
	movl	(%rsi), %edx
	movl	%edx, (%rdi)
	movb	$0, 4(%rdi)
	jmp	L(CalculateSrcLen)

	.p2align 4
L(Exit6):
	movl	(%rsi), %ecx
	movb	4(%rsi), %dh
	movl	%ecx, (%rdi)
	movb	%dh, 4(%rdi)
	movb	$0, 5(%rdi)
	jmp	L(CalculateSrcLen)

	.p2align 4
L(Exit7):
	movl	(%rsi), %ecx
	movw	4(%rsi), %dx
	movl	%ecx, (%rdi)
	movw	%dx, 4(%rdi)
	movb	$0, 6(%rdi)
	jmp	L(CalculateSrcLen)

	.p2align 4
L(Exit8):
	movl	(%rsi), %ecx
	movl	3(%rsi), %edx
	movl	%ecx, (%rdi)
	movl	%edx, 3(%rdi)
	movb	$0, 7(%rdi)
	jmp	L(CalculateSrcLen)

	.p2align 4
L(Exit9):
	movq	(%rsi), %rdx
	movq	%rdx, (%rdi)
	movb	$0, 8(%rdi)
	jmp	L(CalculateSrcLen)

	.p2align 4
L(Exit10):
	movq	(%rsi), %rcx
	movb	8(%rsi), %dh
	movq	%rcx, (%rdi)
	movb	%dh, 8(%rdi)
	movb	$0, 9(%rdi)
	jmp	L(CalculateSrcLen)

	.p2align 4
L(Exit11):
	movq	(%rsi), %rcx
	movw	8(%rsi), %dx
	movq	%rcx, (%rdi)
	movw	%dx, 8(%rdi)
	movb	$0, 10(%rdi)
	jmp	L(CalculateSrcLen)

	.p2align 4
L(Exit12):
	movq	(%rsi), %rcx
	movl	7(%rsi), %edx
	movq	%rcx, (%rdi)
	movl	%edx, 7(%rdi)
	movb	$0, 11(%rdi)
	jmp	L(CalculateSrcLen)

	.p2align 4
L(Exit13):
	movq	(%rsi), %rcx
	movl	8(%rsi), %edx
	movq	%rcx, (%rdi)
	movl	%edx, 8(%rdi)
	movb	$0, 12(%rdi)
	jmp	L(CalculateSrcLen)

	.p2align 4
L(Exit14):
	movq	(%rsi), %rcx
	movq	5(%rsi), %rdx
	movq	%rcx, (%rdi)
	movq	%rdx, 5(%rdi)
	movb	$0, 13(%rdi)
	jmp	L(CalculateSrcLen)

	.p2align 4
L(Exit15):
	movq	(%rsi), %rcx
	movq	6(%rsi), %rdx
	movq	%rcx, (%rdi)
	movq	%rdx, 6(%rdi)
	movb	$0, 14(%rdi)
	jmp	L(CalculateSrcLen)

	.p2align 4
L(Exit16):
	movq	(%rsi), %rcx
	movq	7(%rsi), %rdx
	movq	%rcx, (%rdi)
	movq	%rdx, 7(%rdi)
	movb	$0, 15(%rdi)
	jmp	L(CalculateSrcLen)

	.p2align 4
L(Exit17):
	movdqu	(%rsi), %xmm0
	movdqu	%xmm0, (%rdi)
	movb	$0, 16(%rdi)
	jmp	L(CalculateSrcLen)

	.p2align 4
L(Exit18):
	movdqu	(%rsi), %xmm0
	movb	16(%rsi), %dh
	movdqu	%xmm0, (%rdi)
	movb	%dh, 16(%rdi)
	movb	$0, 17(%rdi)
	jmp	L(CalculateSrcLen)

	.p2align 4
L(Exit19):
	movdqu	(%rsi), %xmm0
	movw	16(%rsi), %cx
	movdqu	%xmm0, (%rdi)
	movw	%cx, 16(%rdi)
	movb	$0, 18(%rdi)
	jmp	L(CalculateSrcLen)

	.p2align 4
L(Exit20):
	movdqu	(%rsi), %xmm0
	movl	15(%rsi), %ecx
	movdqu	%xmm0, (%rdi)
	movl	%ecx, 15(%rdi)
	movb	$0, 19(%rdi)
	jmp	L(CalculateSrcLen)

	.p2align 4
L(Exit21):
	movdqu	(%rsi), %xmm0
	movl	16(%rsi), %ecx
	movdqu	%xmm0, (%rdi)
	movl	%ecx, 16(%rdi)
	movb	$0, 20(%rdi)
	jmp	L(CalculateSrcLen)

	.p2align 4
L(Exit22):
	movdqu	(%rsi), %xmm0
	movl	16(%rsi), %ecx
	movb	20(%rsi), %dh
	movdqu	%xmm0, (%rdi)
	movl	%ecx, 16(%rdi)
	movb	%dh, 20(%rdi)
	movb	$0, 21(%rdi)
	jmp	L(CalculateSrcLen)

	.p2align 4
L(Exit23):
	movdqu	(%rsi), %xmm0
	movq	14(%rsi), %rcx
	movdqu	%xmm0, (%rdi)
	movq	%rcx, 14(%rdi)
	movb	$0, 22(%rdi)
	jmp	L(CalculateSrcLen)

	.p2align 4
L(Exit24):
	movdqu	(%rsi), %xmm0
	movq	15(%rsi), %rcx
	movdqu	%xmm0, (%rdi)
	movq	%rcx, 15(%rdi)
	movb	$0, 23(%rdi)
	jmp	L(CalculateSrcLen)

	.p2align 4
L(Exit25):
	movdqu	(%rsi), %xmm0
	movq	16(%rsi), %rcx
	movdqu	%xmm0, (%rdi)
	movq	%rcx, 16(%rdi)
	movb	$0, 24(%rdi)
	jmp	L(CalculateSrcLen)

	.p2align 4
L(Exit26):
	movdqu	(%rsi), %xmm0
	movq	16(%rsi), %rcx
	movb	24(%rsi), %dh
	movdqu	%xmm0, (%rdi)
	movq	%rcx, 16(%rdi)
	mov	%dh, 24(%rdi)
	movb	$0, 25(%rdi)
	jmp	L(CalculateSrcLen)

	.p2align 4
L(Exit27):
	movdqu	(%rsi), %xmm0
	movq	16(%rsi), %rdx
	movw	24(%rsi), %cx
	movdqu	%xmm0, (%rdi)
	movq	%rdx, 16(%rdi)
	movw	%cx, 24(%rdi)
	movb	$0, 26(%rdi)
	jmp	L(CalculateSrcLen)

	.p2align 4
L(Exit28):
	movdqu	(%rsi), %xmm0
	movq	16(%rsi), %rdx
	movl	23(%rsi), %ecx
	movdqu	%xmm0, (%rdi)
	movq	%rdx, 16(%rdi)
	movl	%ecx, 23(%rdi)
	movb	$0, 27(%rdi)
	jmp	L(CalculateSrcLen)

	.p2align 4
L(Exit29):
	movdqu	(%rsi), %xmm0
	movq	16(%rsi), %rdx
	movl	24(%rsi), %ecx
	movdqu	%xmm0, (%rdi)
	movq	%rdx, 16(%rdi)
	movl	%ecx, 24(%rdi)
	movb	$0, 28(%rdi)
	jmp	L(CalculateSrcLen)

	.p2align 4
L(Exit30):
	movdqu	(%rsi), %xmm0
	movdqu	13(%rsi), %xmm2
	movdqu	%xmm0, (%rdi)
	movdqu	%xmm2, 13(%rdi)
	movb	$0, 29(%rdi)
	jmp	L(CalculateSrcLen)

	.p2align 4
L(Exit31):
	movdqu	(%rsi), %xmm0
	movdqu	14(%rsi), %xmm2
	movdqu	%xmm0, (%rdi)
	movdqu	%xmm2, 14(%rdi)
	movb	$0, 30(%rdi)
	jmp	L(CalculateSrcLen)

	.p2align 4
L(Exit32):
	movdqu	(%rsi), %xmm0
	movdqu	15(%rsi), %xmm2
	movdqu	%xmm0, (%rdi)
	movdqu	%xmm2, 15(%rdi)
	movb	$0, 31(%rdi)
	jmp	L(CalculateSrcLen)

	.p2align 4
L(StringTail0):
	mov	(%rsi), %dl
	mov	%dl, (%rdi)
	RETURN

	.p2align 4
L(StringTail1):
	mov	(%rsi), %dx
	mov	%dx, (%rdi)
	RETURN

	.p2align 4
L(StringTail2):
	mov	(%rsi), %cx
	mov	2(%rsi), %dl
	mov	%cx, (%rdi)
	mov	%dl, 2(%rdi)
	RETURN

	.p2align 4
L(StringTail3):
	mov	(%rsi), %edx
	mov	%edx, (%rdi)
	RETURN

	.p2align 4
L(StringTail4):
	mov	(%rsi), %ecx
	mov	4(%rsi), %dl
	mov	%ecx, (%rdi)
	mov	%dl, 4(%rdi)
	RETURN

	.p2align 4
L(StringTail5):
	mov	(%rsi), %ecx
	mov	4(%rsi), %dx
	mov	%ecx, (%rdi)
	mov	%dx, 4(%rdi)
	RETURN

	.p2align 4
L(StringTail6):
	mov	(%rsi), %ecx
	mov	3(%rsi), %edx
	mov	%ecx, (%rdi)
	mov	%edx, 3(%rdi)
	RETURN

	.p2align 4
L(StringTail7):
	mov	(%rsi), %rdx
	mov	%rdx, (%rdi)
	RETURN

	.p2align 4
L(StringTail8):
	mov	(%rsi), %rcx
	mov	8(%rsi), %dl
	mov	%rcx, (%rdi)
	mov	%dl, 8(%rdi)
	RETURN

	.p2align 4
L(StringTail9):
	mov	(%rsi), %rcx
	mov	8(%rsi), %dx
	mov	%rcx, (%rdi)
	mov	%dx, 8(%rdi)
	RETURN

	.p2align 4
L(StringTail10):
	mov	(%rsi), %rcx
	mov	7(%rsi), %edx
	mov	%rcx, (%rdi)
	mov	%edx, 7(%rdi)
	RETURN

	.p2align 4
L(StringTail11):
	mov	(%rsi), %rcx
	mov	8(%rsi), %edx
	mov	%rcx, (%rdi)
	mov	%edx, 8(%rdi)
	RETURN

	.p2align 4
L(StringTail12):
	mov	(%rsi), %rcx
	mov	5(%rsi), %rdx
	mov	%rcx, (%rdi)
	mov	%rdx, 5(%rdi)
	RETURN

	.p2align 4
L(StringTail13):
	mov	(%rsi), %rcx
	mov	6(%rsi), %rdx
	mov	%rcx, (%rdi)
	mov	%rdx, 6(%rdi)
	RETURN

	.p2align 4
L(StringTail14):
	mov	(%rsi), %rcx
	mov	7(%rsi), %rdx
	mov	%rcx, (%rdi)
	mov	%rdx, 7(%rdi)
	RETURN

	.p2align 4
L(StringTail15):
	movdqu	(%rsi), %xmm0
	movdqu	%xmm0, (%rdi)
	RETURN

	.p2align 4
L(StringTail16):
	movdqu	(%rsi), %xmm0
	mov	16(%rsi), %cl
	movdqu	%xmm0, (%rdi)
	mov	%cl, 16(%rdi)
	RETURN

	.p2align 4
L(StringTail17):
	movdqu	(%rsi), %xmm0
	mov	16(%rsi), %cx
	movdqu	%xmm0, (%rdi)
	mov	%cx, 16(%rdi)
	RETURN

	.p2align 4
L(StringTail18):
	movdqu	(%rsi), %xmm0
	mov	15(%rsi), %ecx
	movdqu	%xmm0, (%rdi)
	mov	%ecx, 15(%rdi)
	RETURN

	.p2align 4
L(StringTail19):
	movdqu	(%rsi), %xmm0
	mov	16(%rsi), %ecx
	movdqu	%xmm0, (%rdi)
	mov	%ecx, 16(%rdi)
	RETURN

	.p2align 4
L(StringTail20):
	movdqu	(%rsi), %xmm0
	mov	16(%rsi), %ecx
	mov	20(%rsi), %dl
	movdqu	%xmm0, (%rdi)
	mov	%ecx, 16(%rdi)
	mov	%dl, 20(%rdi)
	RETURN

	.p2align 4
L(StringTail21):
	movdqu	(%rsi), %xmm0
	mov	14(%rsi), %rcx
	movdqu	%xmm0, (%rdi)
	mov	%rcx, 14(%rdi)
	RETURN

	.p2align 4
L(StringTail22):
	movdqu	(%rsi), %xmm0
	mov	15(%rsi), %rcx
	movdqu	%xmm0, (%rdi)
	mov	%rcx, 15(%rdi)
	RETURN

	.p2align 4
L(StringTail23):
	movdqu	(%rsi), %xmm0
	mov	16(%rsi), %rcx
	movdqu	%xmm0, (%rdi)
	mov	%rcx, 16(%rdi)
	RETURN

	.p2align 4
L(StringTail24):
	movdqu	(%rsi), %xmm0
	mov	16(%rsi), %rdx
	mov	24(%rsi), %cl
	movdqu	%xmm0, (%rdi)
	mov	%rdx, 16(%rdi)
	mov	%cl, 24(%rdi)
	RETURN

	.p2align 4
L(StringTail25):
	movdqu	(%rsi), %xmm0
	mov	16(%rsi), %rdx
	mov	24(%rsi), %cx
	movdqu	%xmm0, (%rdi)
	mov	%rdx, 16(%rdi)
	mov	%cx, 24(%rdi)
	RETURN

	.p2align 4
L(StringTail26):
	movdqu	(%rsi), %xmm0
	mov	16(%rsi), %rdx
	mov	23(%rsi), %ecx
	movdqu	%xmm0, (%rdi)
	mov	%rdx, 16(%rdi)
	mov	%ecx, 23(%rdi)
	RETURN

	.p2align 4
L(StringTail27):
	movdqu	(%rsi), %xmm0
	mov	16(%rsi), %rdx
	mov	24(%rsi), %ecx
	movdqu	%xmm0, (%rdi)
	mov	%rdx, 16(%rdi)
	mov	%ecx, 24(%rdi)
	RETURN

	.p2align 4
L(StringTail28):
	movdqu	(%rsi), %xmm0
	movdqu	13(%rsi), %xmm2
	movdqu	%xmm0, (%rdi)
	movdqu	%xmm2, 13(%rdi)
	RETURN

	.p2align 4
L(StringTail29):
	movdqu	(%rsi), %xmm0
	movdqu	14(%rsi), %xmm2
	movdqu	%xmm0, (%rdi)
	movdqu	%xmm2, 14(%rdi)
	RETURN

	.p2align 4
L(StringTail30):
	movdqu	(%rsi), %xmm0
	movdqu	15(%rsi), %xmm2
	movdqu	%xmm0, (%rdi)
	movdqu	%xmm2, 15(%rdi)
	RETURN

	.p2align 4
L(StringTail31):
	movdqu	(%rsi), %xmm0
	movdqu	16(%rsi), %xmm2
	movdqu	%xmm0, (%rdi)
	movdqu	%xmm2, 16(%rdi)
	RETURN

	.p2align 4
L(StringTail32):
	movdqu	(%rsi), %xmm0
	movdqu	16(%rsi), %xmm2
	mov	32(%rsi), %cl
	movdqu	%xmm0, (%rdi)
	movdqu	%xmm2, 16(%rdi)
	mov	%cl, 32(%rdi)
	RETURN

	.p2align 4
L(StringTail33):
	movdqu	(%rsi), %xmm0
	movdqu	16(%rsi), %xmm2
	mov	32(%rsi), %cl
	movdqu	%xmm0, (%rdi)
	movdqu	%xmm2, 16(%rdi)
	mov	%cl, 32(%rdi)
	RETURN

	.p2align 4
L(CalculateSrcLenCase1):
	xor	%r8, %r8
	xor	%rax, %rax
L(CalculateSrcLen):
	pxor	%xmm0, %xmm0
	xor	%rcx, %rcx
	add	%r8, %rsi
	movdqu	(%rsi), %xmm1
	pcmpeqb	%xmm1, %xmm0
	pmovmskb %xmm0, %rdx
	test	%rdx, %rdx
	jnz	L(SrcLenLoopEnd)

	add	%rax, %r9
	mov	$16, %rax
	mov	%rsi, %rcx
	and	$15, %rcx
	and	$-16, %rsi
L(SrcLenLoop):
	movdqa	(%rsi, %rax), %xmm1
	pcmpeqb	%xmm1, %xmm0
	pmovmskb %xmm0, %rdx
	test	%rdx, %rdx
	jnz	L(SrcLenLoopEnd)
	add	$16, %rax
	jmp	L(SrcLenLoop)

	.p2align 4
L(SrcLenLoopEnd):
	bsf	%rdx, %rdx
	add	%rdx, %rax
	sub	%rcx, %rax
	RETURN

END (STRLCPY)

	.p2align 4
	.section .rodata
L(ExitTable):
	.int	JMPTBL(L(Exit0), L(ExitTable))
	.int	JMPTBL(L(Exit1), L(ExitTable))
	.int	JMPTBL(L(Exit2), L(ExitTable))
	.int	JMPTBL(L(Exit3), L(ExitTable))
	.int	JMPTBL(L(Exit4), L(ExitTable))
	.int	JMPTBL(L(Exit5), L(ExitTable))
	.int	JMPTBL(L(Exit6), L(ExitTable))
	.int	JMPTBL(L(Exit7), L(ExitTable))
	.int	JMPTBL(L(Exit8), L(ExitTable))
	.int	JMPTBL(L(Exit9), L(ExitTable))
	.int	JMPTBL(L(Exit10), L(ExitTable))
	.int	JMPTBL(L(Exit11), L(ExitTable))
	.int	JMPTBL(L(Exit12), L(ExitTable))
	.int	JMPTBL(L(Exit13), L(ExitTable))
	.int	JMPTBL(L(Exit14), L(ExitTable))
	.int	JMPTBL(L(Exit15), L(ExitTable))
	.int	JMPTBL(L(Exit16), L(ExitTable))
	.int	JMPTBL(L(Exit17), L(ExitTable))
	.int	JMPTBL(L(Exit18), L(ExitTable))
	.int	JMPTBL(L(Exit19), L(ExitTable))
	.int	JMPTBL(L(Exit20), L(ExitTable))
	.int	JMPTBL(L(Exit21), L(ExitTable))
	.int	JMPTBL(L(Exit22), L(ExitTable))
	.int	JMPTBL(L(Exit23), L(ExitTable))
	.int	JMPTBL(L(Exit24), L(ExitTable))
	.int	JMPTBL(L(Exit25), L(ExitTable))
	.int	JMPTBL(L(Exit26), L(ExitTable))
	.int	JMPTBL(L(Exit27), L(ExitTable))
	.int	JMPTBL(L(Exit28), L(ExitTable))
	.int	JMPTBL(L(Exit29), L(ExitTable))
	.int	JMPTBL(L(Exit30), L(ExitTable))
	.int	JMPTBL(L(Exit31), L(ExitTable))
	.int	JMPTBL(L(Exit32), L(ExitTable))
L(ExitStringTailTable):
	.int	JMPTBL(L(StringTail0), L(ExitStringTailTable))
	.int	JMPTBL(L(StringTail1), L(ExitStringTailTable))
	.int	JMPTBL(L(StringTail2), L(ExitStringTailTable))
	.int	JMPTBL(L(StringTail3), L(ExitStringTailTable))
	.int	JMPTBL(L(StringTail4), L(ExitStringTailTable))
	.int	JMPTBL(L(StringTail5), L(ExitStringTailTable))
	.int	JMPTBL(L(StringTail6), L(ExitStringTailTable))
	.int	JMPTBL(L(StringTail7), L(ExitStringTailTable))
	.int	JMPTBL(L(StringTail8), L(ExitStringTailTable))
	.int	JMPTBL(L(StringTail9), L(ExitStringTailTable))
	.int	JMPTBL(L(StringTail10), L(ExitStringTailTable))
	.int	JMPTBL(L(StringTail11), L(ExitStringTailTable))
	.int	JMPTBL(L(StringTail12), L(ExitStringTailTable))
	.int	JMPTBL(L(StringTail13), L(ExitStringTailTable))
	.int	JMPTBL(L(StringTail14), L(ExitStringTailTable))
	.int	JMPTBL(L(StringTail15), L(ExitStringTailTable))
	.int	JMPTBL(L(StringTail16), L(ExitStringTailTable))
	.int	JMPTBL(L(StringTail17), L(ExitStringTailTable))
	.int	JMPTBL(L(StringTail18), L(ExitStringTailTable))
	.int	JMPTBL(L(StringTail19), L(ExitStringTailTable))
	.int	JMPTBL(L(StringTail20), L(ExitStringTailTable))
	.int	JMPTBL(L(StringTail21), L(ExitStringTailTable))
	.int	JMPTBL(L(StringTail22), L(ExitStringTailTable))
	.int	JMPTBL(L(StringTail23), L(ExitStringTailTable))
	.int	JMPTBL(L(StringTail24), L(ExitStringTailTable))
	.int	JMPTBL(L(StringTail25), L(ExitStringTailTable))
	.int	JMPTBL(L(StringTail26), L(ExitStringTailTable))
	.int	JMPTBL(L(StringTail27), L(ExitStringTailTable))
	.int	JMPTBL(L(StringTail28), L(ExitStringTailTable))
	.int	JMPTBL(L(StringTail29), L(ExitStringTailTable))
	.int	JMPTBL(L(StringTail30), L(ExitStringTailTable))
	.int	JMPTBL(L(StringTail31), L(ExitStringTailTable))
	.int	JMPTBL(L(StringTail32), L(ExitStringTailTable))
	.int	JMPTBL(L(StringTail33), L(ExitStringTailTable))
