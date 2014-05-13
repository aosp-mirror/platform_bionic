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

#include "cache.h"

#ifndef MEMCPY
# define MEMCPY		memcpy
#endif

#ifndef L
# define L(label)	.L##label
#endif

#ifndef cfi_startproc
# define cfi_startproc	.cfi_startproc
#endif

#ifndef cfi_endproc
# define cfi_endproc	.cfi_endproc
#endif

#ifndef cfi_rel_offset
# define cfi_rel_offset(reg, off)	.cfi_rel_offset reg, off
#endif

#ifndef cfi_restore
# define cfi_restore(reg)	.cfi_restore reg
#endif

#ifndef cfi_adjust_cfa_offset
# define cfi_adjust_cfa_offset(off)	.cfi_adjust_cfa_offset off
#endif

#ifndef ENTRY
# define ENTRY(name)		\
	.type name,  @function;		\
	.globl name;		\
	.p2align 4;		\
name:		\
	cfi_startproc
#endif

#ifndef END
# define END(name)		\
	cfi_endproc;		\
	.size name, .-name
#endif

#define CFI_PUSH(REG)		\
	cfi_adjust_cfa_offset (4);		\
	cfi_rel_offset (REG, 0)

#define CFI_POP(REG)		\
	cfi_adjust_cfa_offset (-4);		\
	cfi_restore (REG)

#define PUSH(REG)	push REG;
#define POP(REG)	pop REG;

#define ENTRANCE	PUSH (%rbx);
#define RETURN_END	POP (%rbx); ret
#define RETURN		RETURN_END;

	.section .text.sse2,"ax",@progbits
ENTRY (MEMCPY)
	ENTRANCE
	cmp	%rsi, %rdi
	je	L(return)

	cmp	$16, %rdx
	jbe	L(len_0_16_bytes)

	cmp	$SHARED_CACHE_SIZE_HALF, %rdx
	jae	L(large_page)

	movdqu	(%rsi), %xmm0
	movdqu	-16(%rsi, %rdx), %xmm1
	cmp	$32, %rdx
	movdqu	%xmm0, (%rdi)
	movdqu	%xmm1, -16(%rdi, %rdx)
	jbe	L(return)

	movdqu	16(%rsi), %xmm0
	movdqu	-32(%rsi, %rdx), %xmm1
	cmp	$64, %rdx
	movdqu	%xmm0, 16(%rdi)
	movdqu	%xmm1, -32(%rdi, %rdx)
	jbe	L(return)

	movdqu	32(%rsi), %xmm0
	movdqu	48(%rsi), %xmm1
	movdqu	-48(%rsi, %rdx), %xmm2
	movdqu	-64(%rsi, %rdx), %xmm3
	cmp	$128, %rdx
	movdqu	%xmm0, 32(%rdi)
	movdqu	%xmm1, 48(%rdi)
	movdqu	%xmm2, -48(%rdi, %rdx)
	movdqu	%xmm3, -64(%rdi, %rdx)
	jbe	L(return)

/* Now the main loop: we align the address of the destination.  */
	lea	64(%rdi), %r8
	and	$-64, %r8

	add	%rdi, %rdx
	and	$-64, %rdx

	sub	%rdi, %rsi

/* We should stop two iterations before the termination
	(in order not to misprefetch).  */
	sub	$64, %rdx
	cmp	%r8, %rdx
	je	L(main_loop_just_one_iteration)

	sub	$64, %rdx
	cmp	%r8, %rdx
	je	L(main_loop_last_two_iterations)


	.p2align 4
L(main_loop_cache):

	prefetcht0 128(%r8, %rsi)

	movdqu	(%r8, %rsi), %xmm0
	movdqu	16(%r8, %rsi), %xmm1
	movdqu	32(%r8, %rsi), %xmm2
	movdqu	48(%r8, %rsi), %xmm3
	movdqa	%xmm0, (%r8)
	movdqa	%xmm1, 16(%r8)
	movdqa	%xmm2, 32(%r8)
	movdqa	%xmm3, 48(%r8)
	lea	64(%r8), %r8
	cmp	%r8, %rdx
	jne	L(main_loop_cache)

L(main_loop_last_two_iterations):
	movdqu	(%r8, %rsi), %xmm0
	movdqu	16(%r8, %rsi), %xmm1
	movdqu	32(%r8, %rsi), %xmm2
	movdqu	48(%r8, %rsi), %xmm3
	movdqu	64(%r8, %rsi), %xmm4
	movdqu	80(%r8, %rsi), %xmm5
	movdqu	96(%r8, %rsi), %xmm6
	movdqu	112(%r8, %rsi), %xmm7
	movdqa	%xmm0, (%r8)
	movdqa	%xmm1, 16(%r8)
	movdqa	%xmm2, 32(%r8)
	movdqa	%xmm3, 48(%r8)
	movdqa	%xmm4, 64(%r8)
	movdqa	%xmm5, 80(%r8)
	movdqa	%xmm6, 96(%r8)
	movdqa	%xmm7, 112(%r8)
	jmp	L(return)

L(main_loop_just_one_iteration):
	movdqu	(%r8, %rsi), %xmm0
	movdqu	16(%r8, %rsi), %xmm1
	movdqu	32(%r8, %rsi), %xmm2
	movdqu	48(%r8, %rsi), %xmm3
	movdqa	%xmm0, (%r8)
	movdqa	%xmm1, 16(%r8)
	movdqa	%xmm2, 32(%r8)
	movdqa	%xmm3, 48(%r8)
	jmp	L(return)

L(large_page):
	movdqu	(%rsi), %xmm0
	movdqu	16(%rsi), %xmm1
	movdqu	32(%rsi), %xmm2
	movdqu	48(%rsi), %xmm3
	movdqu	-64(%rsi, %rdx), %xmm4
	movdqu	-48(%rsi, %rdx), %xmm5
	movdqu	-32(%rsi, %rdx), %xmm6
	movdqu	-16(%rsi, %rdx), %xmm7
	movdqu	%xmm0, (%rdi)
	movdqu	%xmm1, 16(%rdi)
	movdqu	%xmm2, 32(%rdi)
	movdqu	%xmm3, 48(%rdi)
	movdqu	%xmm4, -64(%rdi, %rdx)
	movdqu	%xmm5, -48(%rdi, %rdx)
	movdqu	%xmm6, -32(%rdi, %rdx)
	movdqu	%xmm7, -16(%rdi, %rdx)

	movdqu	64(%rsi), %xmm0
	movdqu	80(%rsi), %xmm1
	movdqu	96(%rsi), %xmm2
	movdqu	112(%rsi), %xmm3
	movdqu	-128(%rsi, %rdx), %xmm4
	movdqu	-112(%rsi, %rdx), %xmm5
	movdqu	-96(%rsi, %rdx), %xmm6
	movdqu	-80(%rsi, %rdx), %xmm7
	movdqu	%xmm0, 64(%rdi)
	movdqu	%xmm1, 80(%rdi)
	movdqu	%xmm2, 96(%rdi)
	movdqu	%xmm3, 112(%rdi)
	movdqu	%xmm4, -128(%rdi, %rdx)
	movdqu	%xmm5, -112(%rdi, %rdx)
	movdqu	%xmm6, -96(%rdi, %rdx)
	movdqu	%xmm7, -80(%rdi, %rdx)

/* Now the main loop with non temporal stores. We align
	the address of the destination.  */
	lea	128(%rdi), %r8
	and	$-128, %r8

	add	%rdi, %rdx
	and	$-128, %rdx

	sub	%rdi, %rsi

	.p2align 4
L(main_loop_large_page):
	movdqu	(%r8, %rsi), %xmm0
	movdqu	16(%r8, %rsi), %xmm1
	movdqu	32(%r8, %rsi), %xmm2
	movdqu	48(%r8, %rsi), %xmm3
	movdqu	64(%r8, %rsi), %xmm4
	movdqu	80(%r8, %rsi), %xmm5
	movdqu	96(%r8, %rsi), %xmm6
	movdqu	112(%r8, %rsi), %xmm7
	movntdq	%xmm0, (%r8)
	movntdq	%xmm1, 16(%r8)
	movntdq	%xmm2, 32(%r8)
	movntdq	%xmm3, 48(%r8)
	movntdq	%xmm4, 64(%r8)
	movntdq	%xmm5, 80(%r8)
	movntdq	%xmm6, 96(%r8)
	movntdq	%xmm7, 112(%r8)
	lea	128(%r8), %r8
	cmp	%r8, %rdx
	jne	L(main_loop_large_page)
	sfence
	jmp	L(return)

L(len_0_16_bytes):
	testb	$24, %dl
	jne	L(len_9_16_bytes)
	testb	$4, %dl
	.p2align 4,,5
	jne	L(len_5_8_bytes)
	test	%rdx, %rdx
	.p2align 4,,2
	je	L(return)
	movzbl	(%rsi), %ebx
	testb	$2, %dl
	movb	%bl, (%rdi)
	je	L(return)
	movzwl	-2(%rsi,%rdx), %ebx
	movw	%bx, -2(%rdi,%rdx)
	jmp	L(return)

L(len_9_16_bytes):
	movq	(%rsi), %xmm0
	movq	-8(%rsi, %rdx), %xmm1
	movq	%xmm0, (%rdi)
	movq	%xmm1, -8(%rdi, %rdx)
	jmp	L(return)

L(len_5_8_bytes):
	movl	(%rsi), %ebx
	movl	%ebx, (%rdi)
	movl	-4(%rsi,%rdx), %ebx
	movl	%ebx, -4(%rdi,%rdx)
	jmp	L(return)

L(return):
	mov 	%rdi, %rax
	RETURN

END (MEMCPY)
