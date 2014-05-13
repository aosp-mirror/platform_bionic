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

#ifndef MEMMOVE
# define MEMMOVE		memmove
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
ENTRY (MEMMOVE)
	ENTRANCE
#ifdef USE_AS_BCOPY
	xchg	%rsi, %rdi
#endif
	mov	%rdi, %rax

/* Check whether we should copy backward or forward.  */
	cmp	%rsi, %rdi
	je	L(mm_return)
	ja	L(mm_len_0_or_more_backward)

/* Now do checks for lengths. We do [0..16], [0..32], [0..64], [0..128]
	separately.  */
	cmp	$16, %rdx
	jbe	L(mm_len_0_16_bytes_forward)

	cmp	$32, %rdx
	jg	L(mm_len_32_or_more_forward)

/* Copy [0..32] and return.  */
	movdqu	(%rsi), %xmm0
	movdqu	-16(%rsi, %rdx), %xmm1
	movdqu	%xmm0, (%rdi)
	movdqu	%xmm1, -16(%rdi, %rdx)
	jmp	L(mm_return)

L(mm_len_32_or_more_forward):
	cmp	$64, %rdx
	jg	L(mm_len_64_or_more_forward)

/* Copy [0..64] and return.  */
	movdqu	(%rsi), %xmm0
	movdqu	16(%rsi), %xmm1
	movdqu	-16(%rsi, %rdx), %xmm2
	movdqu	-32(%rsi, %rdx), %xmm3
	movdqu	%xmm0, (%rdi)
	movdqu	%xmm1, 16(%rdi)
	movdqu	%xmm2, -16(%rdi, %rdx)
	movdqu	%xmm3, -32(%rdi, %rdx)
	jmp	L(mm_return)

L(mm_len_64_or_more_forward):
	cmp	$128, %rdx
	jg	L(mm_len_128_or_more_forward)

/* Copy [0..128] and return.  */
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
	jmp	L(mm_return)

L(mm_len_128_or_more_forward):

	cmp	$SHARED_CACHE_SIZE_HALF, %rdx
	jae	L(mm_large_page_forward)

	mov	%rsi, %r8  // copy src to r8
	mov	%rdi, %r9  // copy dst to r9

/* Aligning the address of destination.  */
/*  save first unaligned 64 bytes */
	movdqu	(%rsi), %xmm0
	movdqu	16(%rsi), %xmm1
	movdqu	32(%rsi), %xmm2
	movdqu	48(%rsi), %xmm3

	lea	64(%r9), %rdi
	and	$-64, %rdi  /* rdi now aligned to next 64 byte boundary */

	sub	%r9, %rsi /* rsi = src - dst = diff */

	movdqu	(%rdi, %rsi), %xmm4
	movdqu	16(%rdi, %rsi), %xmm5
	movdqu	32(%rdi, %rsi), %xmm6
	movdqu	48(%rdi, %rsi), %xmm7

	movdqu	%xmm0, (%r9)
	movdqu	%xmm1, 16(%r9)
	movdqu	%xmm2, 32(%r9)
	movdqu	%xmm3, 48(%r9)
	movdqa	%xmm4, (%rdi)
	movdqa	%xmm5, 16(%rdi)
	movdqa	%xmm6, 32(%rdi)
	movdqa	%xmm7, 48(%rdi)
	add	$64, %rdi

	lea	(%r9, %rdx), %rbx
	and	$-64, %rbx

	cmp	%rdi, %rbx
	jbe	L(mm_copy_remaining_forward)

	.p2align 4
L(mm_main_loop_forward):

	prefetcht0 128(%rdi, %rsi)

	movdqu	(%rdi, %rsi), %xmm0
	movdqu	16(%rdi, %rsi), %xmm1
	movdqu	32(%rdi, %rsi), %xmm2
	movdqu	48(%rdi, %rsi), %xmm3
	movdqa	%xmm0, (%rdi)
	movdqa	%xmm1, 16(%rdi)
	movdqa	%xmm2, 32(%rdi)
	movdqa	%xmm3, 48(%rdi)
	lea	64(%rdi), %rdi
	cmp	%rdi, %rbx
	ja	L(mm_main_loop_forward)

L(mm_copy_remaining_forward):
	add	%r9, %rdx
	sub	%rdi, %rdx
/* We copied all up till %rdi position in the dst.
	In %rdx now is how many bytes are left to copy.
	Now we need to advance %r8. */
	lea	(%rdi, %rsi), %r8

L(mm_remaining_0_64_bytes_forward):
	cmp	$32, %rdx
	ja	L(mm_remaining_33_64_bytes_forward)
	cmp	$16, %rdx
	ja	L(mm_remaining_17_32_bytes_forward)
	test	%rdx, %rdx
	.p2align 4,,2
	je	L(mm_return)

	cmpb	$8, %dl
	ja	L(mm_remaining_9_16_bytes_forward)
	cmpb	$4, %dl
	.p2align 4,,5
	ja	L(mm_remaining_5_8_bytes_forward)
	cmpb	$2, %dl
	.p2align 4,,1
	ja	L(mm_remaining_3_4_bytes_forward)
	movzbl	-1(%r8,%rdx), %esi
	movzbl	(%r8), %ebx
	movb	%sil, -1(%rdi,%rdx)
	movb	%bl, (%rdi)
	jmp	L(mm_return)

L(mm_remaining_33_64_bytes_forward):
	movdqu	(%r8), %xmm0
	movdqu	16(%r8), %xmm1
	movdqu	-32(%r8, %rdx), %xmm2
	movdqu	-16(%r8, %rdx), %xmm3
	movdqu	%xmm0, (%rdi)
	movdqu	%xmm1, 16(%rdi)
	movdqu	%xmm2, -32(%rdi, %rdx)
	movdqu	%xmm3, -16(%rdi, %rdx)
	jmp	L(mm_return)

L(mm_remaining_17_32_bytes_forward):
	movdqu	(%r8), %xmm0
	movdqu	-16(%r8, %rdx), %xmm1
	movdqu	%xmm0, (%rdi)
	movdqu	%xmm1, -16(%rdi, %rdx)
	jmp	L(mm_return)

L(mm_remaining_3_4_bytes_forward):
	movzwl	-2(%r8,%rdx), %esi
	movzwl	(%r8), %ebx
	movw	%si, -2(%rdi,%rdx)
	movw	%bx, (%rdi)
	jmp	L(mm_return)

L(mm_remaining_5_8_bytes_forward):
	movl	(%r8), %esi
	movl	-4(%r8,%rdx), %ebx
	movl	%esi, (%rdi)
	movl	%ebx, -4(%rdi,%rdx)
	jmp	L(mm_return)

L(mm_remaining_9_16_bytes_forward):
	mov	(%r8), %rsi
	mov	-8(%r8, %rdx), %rbx
	mov	%rsi, (%rdi)
	mov	%rbx, -8(%rdi, %rdx)
	jmp	L(mm_return)

L(mm_len_0_16_bytes_forward):
	testb	$24, %dl
	jne	L(mm_len_9_16_bytes_forward)
	testb	$4, %dl
	.p2align 4,,5
	jne	L(mm_len_5_8_bytes_forward)
	test	%rdx, %rdx
	.p2align 4,,2
	je	L(mm_return)
	testb	$2, %dl
	.p2align 4,,1
	jne	L(mm_len_2_4_bytes_forward)
	movzbl	-1(%rsi,%rdx), %ebx
	movzbl	(%rsi), %esi
	movb	%bl, -1(%rdi,%rdx)
	movb	%sil, (%rdi)
	jmp	L(mm_return)

L(mm_len_2_4_bytes_forward):
	movzwl	-2(%rsi,%rdx), %ebx
	movzwl	(%rsi), %esi
	movw	%bx, -2(%rdi,%rdx)
	movw	%si, (%rdi)
	jmp	L(mm_return)

L(mm_len_5_8_bytes_forward):
	movl	(%rsi), %ebx
	movl	-4(%rsi,%rdx), %esi
	movl	%ebx, (%rdi)
	movl	%esi, -4(%rdi,%rdx)
	jmp	L(mm_return)

L(mm_len_9_16_bytes_forward):
	mov	(%rsi), %rbx
	mov	-8(%rsi, %rdx), %rsi
	mov	%rbx, (%rdi)
	mov	%rsi, -8(%rdi, %rdx)
	jmp	L(mm_return)

/* The code for copying backwards.  */
L(mm_len_0_or_more_backward):

/* Now do checks for lengths. We do [0..16], [0..32], [0..64], [0..128]
	separately.  */
	cmp	$16, %rdx
	jbe	L(mm_len_0_16_bytes_backward)

	cmp	$32, %rdx
	jg	L(mm_len_32_or_more_backward)

/* Copy [0..32] and return.  */
	movdqu	(%rsi), %xmm0
	movdqu	-16(%rsi, %rdx), %xmm1
	movdqu	%xmm0, (%rdi)
	movdqu	%xmm1, -16(%rdi, %rdx)
	jmp	L(mm_return)

L(mm_len_32_or_more_backward):
	cmp	$64, %rdx
	jg	L(mm_len_64_or_more_backward)

/* Copy [0..64] and return.  */
	movdqu	(%rsi), %xmm0
	movdqu	16(%rsi), %xmm1
	movdqu	-16(%rsi, %rdx), %xmm2
	movdqu	-32(%rsi, %rdx), %xmm3
	movdqu	%xmm0, (%rdi)
	movdqu	%xmm1, 16(%rdi)
	movdqu	%xmm2, -16(%rdi, %rdx)
	movdqu	%xmm3, -32(%rdi, %rdx)
	jmp	L(mm_return)

L(mm_len_64_or_more_backward):
	cmp	$128, %rdx
	jg	L(mm_len_128_or_more_backward)

/* Copy [0..128] and return.  */
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
	jmp	L(mm_return)

L(mm_len_128_or_more_backward):

	cmp	$SHARED_CACHE_SIZE_HALF, %rdx
	jae	L(mm_large_page_backward)

/* Aligning the address of destination. We need to save
	16 bits from the source in order not to overwrite them.  */
	movdqu	-16(%rsi, %rdx), %xmm0
	movdqu	-32(%rsi, %rdx), %xmm1
	movdqu	-48(%rsi, %rdx), %xmm2
	movdqu	-64(%rsi, %rdx), %xmm3

	lea	(%rdi, %rdx), %r9
	and	$-64, %r9 /* r9 = aligned dst */

	mov	%rsi, %r8
	sub	%rdi, %r8 /* r8 = src - dst, diff */

	movdqu	-16(%r9, %r8), %xmm4
	movdqu	-32(%r9, %r8), %xmm5
	movdqu	-48(%r9, %r8), %xmm6
	movdqu	-64(%r9, %r8), %xmm7

	movdqu	%xmm0, -16(%rdi, %rdx)
	movdqu	%xmm1, -32(%rdi, %rdx)
	movdqu	%xmm2, -48(%rdi, %rdx)
	movdqu	%xmm3, -64(%rdi, %rdx)
	movdqa	%xmm4, -16(%r9)
	movdqa	%xmm5, -32(%r9)
	movdqa	%xmm6, -48(%r9)
	movdqa	%xmm7, -64(%r9)
	lea	-64(%r9), %r9

	lea	64(%rdi), %rbx
	and	$-64, %rbx

/* Compute in %rdx how many bytes are left to copy after
	the main loop stops.  */
	mov 	%rbx, %rdx
	sub 	%rdi, %rdx

	cmp	%r9, %rbx
	jb	L(mm_main_loop_backward)
	jmp	L(mm_len_0_or_more_backward)

	.p2align 4
L(mm_main_loop_backward):

	prefetcht0 -128(%r9, %r8)

	movdqu	-64(%r9, %r8), %xmm0
	movdqu	-48(%r9, %r8), %xmm1
	movdqu	-32(%r9, %r8), %xmm2
	movdqu	-16(%r9, %r8), %xmm3
	movdqa	%xmm0, -64(%r9)
	movdqa	%xmm1, -48(%r9)
	movdqa	%xmm2, -32(%r9)
	movdqa	%xmm3, -16(%r9)
	lea	-64(%r9), %r9
	cmp	%r9, %rbx
	jb	L(mm_main_loop_backward)
	jmp	L(mm_len_0_or_more_backward)

/* Copy [0..16] and return.  */
L(mm_len_0_16_bytes_backward):
	testb	$24, %dl
	jnz	L(mm_len_9_16_bytes_backward)
	testb	$4, %dl
	.p2align 4,,5
	jnz	L(mm_len_5_8_bytes_backward)
	test	%rdx, %rdx
	.p2align 4,,2
	je	L(mm_return)
	testb	$2, %dl
	.p2align 4,,1
	jne	L(mm_len_3_4_bytes_backward)
	movzbl	-1(%rsi,%rdx), %ebx
	movzbl	(%rsi), %ecx
	movb	%bl, -1(%rdi,%rdx)
	movb	%cl, (%rdi)
	jmp	L(mm_return)

L(mm_len_3_4_bytes_backward):
	movzwl	-2(%rsi,%rdx), %ebx
	movzwl	(%rsi), %ecx
	movw	%bx, -2(%rdi,%rdx)
	movw	%cx, (%rdi)
	jmp	L(mm_return)

L(mm_len_9_16_bytes_backward):
	movl	-4(%rsi,%rdx), %ebx
	movl	-8(%rsi,%rdx), %ecx
	movl	%ebx, -4(%rdi,%rdx)
	movl	%ecx, -8(%rdi,%rdx)
	sub	$8, %rdx
	jmp	L(mm_len_0_16_bytes_backward)

L(mm_len_5_8_bytes_backward):
	movl	(%rsi), %ebx
	movl	-4(%rsi,%rdx), %ecx
	movl	%ebx, (%rdi)
	movl	%ecx, -4(%rdi,%rdx)

L(mm_return):
	RETURN

/* Big length copy forward part.  */

L(mm_large_page_forward):
/* Aligning the address of destination. We need to save
	16 bits from the source in order not to overwrite them.  */

	mov	%rsi, %r8
	mov	%rdi, %r9

	movdqu	(%rsi), %xmm0
	movdqu	16(%rsi), %xmm1
	movdqu	32(%rsi), %xmm2
	movdqu	48(%rsi), %xmm3

	lea	64(%r9), %rdi
	and	$-64, %rdi      /* rdi = aligned dst */

	sub	%r9, %rsi        /* rsi = diff */

	movdqu	(%rdi, %rsi), %xmm4
	movdqu	16(%rdi, %rsi), %xmm5
	movdqu	32(%rdi, %rsi), %xmm6
	movdqu	48(%rdi, %rsi), %xmm7

	movdqu	%xmm0, (%r9)
	movdqu	%xmm1, 16(%r9)
	movdqu	%xmm2, 32(%r9)
	movdqu	%xmm3, 48(%r9)
	movntdq	%xmm4, (%rdi)
	movntdq	%xmm5, 16(%rdi)
	movntdq	%xmm6, 32(%rdi)
	movntdq	%xmm7, 48(%rdi)
	add	$64, %rdi

	lea	(%r9, %rdx), %rbx
	and	$-128, %rbx

	cmp	%rdi, %rbx
	jbe	L(mm_copy_remaining_forward)

	.p2align 4
L(mm_large_page_loop_forward):
	movdqu	(%rdi, %rsi), %xmm0
	movdqu	16(%rdi, %rsi), %xmm1
	movdqu	32(%rdi, %rsi), %xmm2
	movdqu	48(%rdi, %rsi), %xmm3
	movdqu	64(%rdi, %rsi), %xmm4
	movdqu	80(%rdi, %rsi), %xmm5
	movdqu	96(%rdi, %rsi), %xmm6
	movdqu	112(%rdi, %rsi), %xmm7
	movntdq	%xmm0, (%rdi)
	movntdq	%xmm1, 16(%rdi)
	movntdq	%xmm2, 32(%rdi)
	movntdq	%xmm3, 48(%rdi)
	movntdq	%xmm4, 64(%rdi)
	movntdq	%xmm5, 80(%rdi)
	movntdq	%xmm6, 96(%rdi)
	movntdq	%xmm7, 112(%rdi)
	lea 	128(%rdi), %rdi
	cmp	%rdi, %rbx
	ja	L(mm_large_page_loop_forward)
	sfence

	add 	%r9, %rdx
	sub 	%rdi, %rdx
/* We copied all up till %rdi position in the dst.
	In %rdx now is how many bytes are left to copy.
	Now we need to advance %r8. */
	lea 	(%rdi, %rsi), %r8

	cmp	$64, %rdx
	jb	L(mm_remaining_0_64_bytes_forward)

	movdqu	(%r8), %xmm0
	movdqu	16(%r8), %xmm1
	movdqu	32(%r8), %xmm2
	movdqu	48(%r8), %xmm3
	movdqu	-64(%r8, %rdx), %xmm4
	movdqu	-48(%r8, %rdx), %xmm5
	movdqu	-32(%r8, %rdx), %xmm6
	movdqu	-16(%r8, %rdx), %xmm7
	movdqu	%xmm0, (%rdi)
	movdqu	%xmm1, 16(%rdi)
	movdqu	%xmm2, 32(%rdi)
	movdqu	%xmm3, 48(%rdi)
	movdqu	%xmm4, -64(%rdi, %rdx)
	movdqu	%xmm5, -48(%rdi, %rdx)
	movdqu	%xmm6, -32(%rdi, %rdx)
	movdqu	%xmm7, -16(%rdi, %rdx)
	jmp	L(mm_return)


/* Big length copy backward part.  */
L(mm_large_page_backward):
/* Aligning the address of destination. We need to save
	16 bits from the source in order not to overwrite them.  */

	movdqu	-16(%rsi, %rdx), %xmm0
	movdqu	-32(%rsi, %rdx), %xmm1
	movdqu	-48(%rsi, %rdx), %xmm2
	movdqu	-64(%rsi, %rdx), %xmm3

	lea	(%rdi, %rdx), %r9
	and	$-64, %r9

	mov 	%rsi, %r8
	sub 	%rdi, %r8

	movdqu	-16(%r9, %r8), %xmm4
	movdqu	-32(%r9, %r8), %xmm5
	movdqu	-48(%r9, %r8), %xmm6
	movdqu	-64(%r9, %r8), %xmm7

	movdqu	%xmm0, -16(%rdi, %rdx)
	movdqu	%xmm1, -32(%rdi, %rdx)
	movdqu	%xmm2, -48(%rdi, %rdx)
	movdqu	%xmm3, -64(%rdi, %rdx)
	movntdq	%xmm4, -16(%r9)
	movntdq	%xmm5, -32(%r9)
	movntdq	%xmm6, -48(%r9)
	movntdq	%xmm7, -64(%r9)
	lea 	-64(%r9), %r9

	lea 	128(%rdi), %rbx
	and 	$-64, %rbx

/* Compute in %rdx how many bytes are left to copy after
	the main loop stops.  */
	mov 	%rbx, %rdx
	sub 	%rdi, %rdx

	cmp	%r9, %rbx
	jae	L(mm_len_0_or_more_backward)

	.p2align 4
L(mm_large_page_loop_backward):
	movdqu	-64(%r9, %r8), %xmm0
	movdqu	-48(%r9, %r8), %xmm1
	movdqu	-32(%r9, %r8), %xmm2
	movdqu	-16(%r9, %r8), %xmm3
	movntdq	%xmm0, -64(%r9)
	movntdq	%xmm1, -48(%r9)
	movntdq	%xmm2, -32(%r9)
	movntdq	%xmm3, -16(%r9)
	lea 	-64(%r9), %r9
	cmp	%r9, %rbx
	jb	L(mm_large_page_loop_backward)
	jmp	L(mm_len_0_or_more_backward)

END (MEMMOVE)
