/*
Copyright (c) 2011 Intel Corporation
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

#ifndef USE_AS_WCSCAT

# ifndef L
#  define L(label)	.L##label
# endif

# ifndef cfi_startproc
#  define cfi_startproc	.cfi_startproc
# endif

# ifndef cfi_endproc
#  define cfi_endproc	.cfi_endproc
# endif

# ifndef ENTRY
#  define ENTRY(name)	\
	.type name,  @function;	\
	.globl name;	\
	.p2align 4;	\
name:	\
	cfi_startproc
# endif

# ifndef END
#  define END(name)	\
	cfi_endproc;	\
	.size name, .-name
# endif

# define PARMS	4
# define STR	PARMS
# define RETURN ret

	.text
ENTRY (wcslen)
	mov	STR(%esp), %edx
#endif
	cmp	$0, (%edx)
	jz	L(exit_tail0)
	cmp	$0, 4(%edx)
	jz	L(exit_tail1)
	cmp	$0, 8(%edx)
	jz	L(exit_tail2)
	cmp	$0, 12(%edx)
	jz	L(exit_tail3)
	cmp	$0, 16(%edx)
	jz	L(exit_tail4)
	cmp	$0, 20(%edx)
	jz	L(exit_tail5)
	cmp	$0, 24(%edx)
	jz	L(exit_tail6)
	cmp	$0, 28(%edx)
	jz	L(exit_tail7)

	pxor	%xmm0, %xmm0

	lea	32(%edx), %eax
	lea	-16(%eax), %ecx
	and	$-16, %eax

	pcmpeqd	(%eax), %xmm0
	pmovmskb %xmm0, %edx
	pxor	%xmm1, %xmm1
	lea	16(%eax), %eax
	test	%edx, %edx
	jnz	L(exit)

	pcmpeqd	(%eax), %xmm1
	pmovmskb %xmm1, %edx
	pxor	%xmm2, %xmm2
	lea	16(%eax), %eax
	test	%edx, %edx
	jnz	L(exit)

	pcmpeqd	(%eax), %xmm2
	pmovmskb %xmm2, %edx
	pxor	%xmm3, %xmm3
	lea	16(%eax), %eax
	test	%edx, %edx
	jnz	L(exit)

	pcmpeqd	(%eax), %xmm3
	pmovmskb %xmm3, %edx
	lea	16(%eax), %eax
	test	%edx, %edx
	jnz	L(exit)

	pcmpeqd	(%eax), %xmm0
	pmovmskb %xmm0, %edx
	lea	16(%eax), %eax
	test	%edx, %edx
	jnz	L(exit)

	pcmpeqd	(%eax), %xmm1
	pmovmskb %xmm1, %edx
	lea	16(%eax), %eax
	test	%edx, %edx
	jnz	L(exit)

	pcmpeqd	(%eax), %xmm2
	pmovmskb %xmm2, %edx
	lea	16(%eax), %eax
	test	%edx, %edx
	jnz	L(exit)

	pcmpeqd	(%eax), %xmm3
	pmovmskb %xmm3, %edx
	lea	16(%eax), %eax
	test	%edx, %edx
	jnz	L(exit)

	pcmpeqd	(%eax), %xmm0
	pmovmskb %xmm0, %edx
	lea	16(%eax), %eax
	test	%edx, %edx
	jnz	L(exit)

	pcmpeqd	(%eax), %xmm1
	pmovmskb %xmm1, %edx
	lea	16(%eax), %eax
	test	%edx, %edx
	jnz	L(exit)

	pcmpeqd	(%eax), %xmm2
	pmovmskb %xmm2, %edx
	lea	16(%eax), %eax
	test	%edx, %edx
	jnz	L(exit)

	pcmpeqd	(%eax), %xmm3
	pmovmskb %xmm3, %edx
	lea	16(%eax), %eax
	test	%edx, %edx
	jnz	L(exit)

	pcmpeqd	(%eax), %xmm0
	pmovmskb %xmm0, %edx
	lea	16(%eax), %eax
	test	%edx, %edx
	jnz	L(exit)

	pcmpeqd	(%eax), %xmm1
	pmovmskb %xmm1, %edx
	lea	16(%eax), %eax
	test	%edx, %edx
	jnz	L(exit)

	pcmpeqd	(%eax), %xmm2
	pmovmskb %xmm2, %edx
	lea	16(%eax), %eax
	test	%edx, %edx
	jnz	L(exit)

	pcmpeqd	(%eax), %xmm3
	pmovmskb %xmm3, %edx
	lea	16(%eax), %eax
	test	%edx, %edx
	jnz	L(exit)

	and	$-0x40, %eax

	.p2align 4
L(aligned_64_loop):
	movaps	(%eax), %xmm0
	movaps	16(%eax), %xmm1
	movaps	32(%eax), %xmm2
	movaps	48(%eax), %xmm6

	pminub	%xmm1, %xmm0
	pminub	%xmm6, %xmm2
	pminub	%xmm0, %xmm2
	pcmpeqd	%xmm3, %xmm2
	pmovmskb %xmm2, %edx
	lea	64(%eax), %eax
	test	%edx, %edx
	jz	L(aligned_64_loop)

	pcmpeqd	-64(%eax), %xmm3
	pmovmskb %xmm3, %edx
	lea	48(%ecx), %ecx
	test	%edx, %edx
	jnz	L(exit)

	pcmpeqd	%xmm1, %xmm3
	pmovmskb %xmm3, %edx
	lea	-16(%ecx), %ecx
	test	%edx, %edx
	jnz	L(exit)

	pcmpeqd	-32(%eax), %xmm3
	pmovmskb %xmm3, %edx
	lea	-16(%ecx), %ecx
	test	%edx, %edx
	jnz	L(exit)

	pcmpeqd	%xmm6, %xmm3
	pmovmskb %xmm3, %edx
	lea	-16(%ecx), %ecx
	test	%edx, %edx
	jnz	L(exit)

	jmp	L(aligned_64_loop)

	.p2align 4
L(exit):
	sub	%ecx, %eax
	shr	$2, %eax
	test	%dl, %dl
	jz	L(exit_high)

	mov	%dl, %cl
	and	$15, %cl
	jz	L(exit_1)
	RETURN

	.p2align 4
L(exit_high):
	mov	%dh, %ch
	and	$15, %ch
	jz	L(exit_3)
	add	$2, %eax
	RETURN

	.p2align 4
L(exit_1):
	add	$1, %eax
	RETURN

	.p2align 4
L(exit_3):
	add	$3, %eax
	RETURN

	.p2align 4
L(exit_tail0):
	xor	%eax, %eax
	RETURN

	.p2align 4
L(exit_tail1):
	mov	$1, %eax
	RETURN

	.p2align 4
L(exit_tail2):
	mov	$2, %eax
	RETURN

	.p2align 4
L(exit_tail3):
	mov	$3, %eax
	RETURN

	.p2align 4
L(exit_tail4):
	mov	$4, %eax
	RETURN

	.p2align 4
L(exit_tail5):
	mov	$5, %eax
	RETURN

	.p2align 4
L(exit_tail6):
	mov	$6, %eax
	RETURN

	.p2align 4
L(exit_tail7):
	mov	$7, %eax
#ifndef USE_AS_WCSCAT
	RETURN

END (wcslen)
#endif
