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
# define MEMMOVE	memmove
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

#ifdef USE_AS_BCOPY
# define SRC		PARMS
# define DEST		SRC+4
# define LEN		DEST+4
#else
# define DEST		PARMS
# define SRC		DEST+4
# define LEN		SRC+4
#endif

#define CFI_PUSH(REG)		\
  cfi_adjust_cfa_offset (4);		\
  cfi_rel_offset (REG, 0)

#define CFI_POP(REG)		\
  cfi_adjust_cfa_offset (-4);		\
  cfi_restore (REG)

#define PUSH(REG)	pushl REG; CFI_PUSH (REG)
#define POP(REG)	popl REG; CFI_POP (REG)

#define PARMS		8		/* Preserve EBX.  */
#define ENTRANCE	PUSH (%ebx);
#define RETURN_END	POP (%ebx); ret
#define RETURN		RETURN_END; CFI_PUSH (%ebx)

	.section .text.sse2,"ax",@progbits
ENTRY (MEMMOVE)
	ENTRANCE
	movl	LEN(%esp), %ecx
	movl	SRC(%esp), %eax
	movl	DEST(%esp), %edx

/* Check whether we should copy backward or forward.  */
	cmp	%eax, %edx
	je	L(mm_return)
	jg	L(mm_len_0_or_more_backward)

/* Now do checks for lengths. We do [0..16], [0..32], [0..64], [0..128]
	separately.  */
	cmp	$16, %ecx
	jbe	L(mm_len_0_16_bytes_forward)

	cmpl	$32, %ecx
	ja	L(mm_len_32_or_more_forward)

/* Copy [0..32] and return.  */
	movdqu	(%eax), %xmm0
	movdqu	-16(%eax, %ecx), %xmm1
	movdqu	%xmm0, (%edx)
	movdqu	%xmm1, -16(%edx, %ecx)
	jmp	L(mm_return)

L(mm_len_32_or_more_forward):
	cmpl	$64, %ecx
	ja	L(mm_len_64_or_more_forward)

/* Copy [0..64] and return.  */
	movdqu	(%eax), %xmm0
	movdqu	16(%eax), %xmm1
	movdqu	-16(%eax, %ecx), %xmm2
	movdqu	-32(%eax, %ecx), %xmm3
	movdqu	%xmm0, (%edx)
	movdqu	%xmm1, 16(%edx)
	movdqu	%xmm2, -16(%edx, %ecx)
	movdqu	%xmm3, -32(%edx, %ecx)
	jmp	L(mm_return)

L(mm_len_64_or_more_forward):
	cmpl	$128, %ecx
	ja	L(mm_len_128_or_more_forward)

/* Copy [0..128] and return.  */
	movdqu	(%eax), %xmm0
	movdqu	16(%eax), %xmm1
	movdqu	32(%eax), %xmm2
	movdqu	48(%eax), %xmm3
	movdqu	-64(%eax, %ecx), %xmm4
	movdqu	-48(%eax, %ecx), %xmm5
	movdqu	-32(%eax, %ecx), %xmm6
	movdqu	-16(%eax, %ecx), %xmm7
	movdqu	%xmm0, (%edx)
	movdqu	%xmm1, 16(%edx)
	movdqu	%xmm2, 32(%edx)
	movdqu	%xmm3, 48(%edx)
	movdqu	%xmm4, -64(%edx, %ecx)
	movdqu	%xmm5, -48(%edx, %ecx)
	movdqu	%xmm6, -32(%edx, %ecx)
	movdqu	%xmm7, -16(%edx, %ecx)
	jmp	L(mm_return)

L(mm_len_128_or_more_forward):
	PUSH (%esi)
	PUSH (%edi)

/* Aligning the address of destination.  */
	movdqu	(%eax), %xmm0
	movdqu	16(%eax), %xmm1
	movdqu	32(%eax), %xmm2
	movdqu	48(%eax), %xmm3

	leal	64(%edx), %edi
	andl	$-64, %edi
	subl	%edx, %eax

	movdqu	(%eax, %edi), %xmm4
	movdqu	16(%eax, %edi), %xmm5
	movdqu	32(%eax, %edi), %xmm6
	movdqu	48(%eax, %edi), %xmm7

	movdqu	%xmm0, (%edx)
	movdqu	%xmm1, 16(%edx)
	movdqu	%xmm2, 32(%edx)
	movdqu	%xmm3, 48(%edx)
	movdqa	%xmm4, (%edi)
	movaps	%xmm5, 16(%edi)
	movaps	%xmm6, 32(%edi)
	movaps	%xmm7, 48(%edi)
	addl	$64, %edi

	leal	(%edx, %ecx), %ebx
	andl	$-64, %ebx
	cmp	%edi, %ebx
	jbe	L(mm_copy_remaining_forward)

	cmp	$SHARED_CACHE_SIZE_HALF, %ecx
	jae	L(mm_large_page_loop_forward)

	.p2align 4
L(mm_main_loop_forward):

	prefetcht0 128(%eax, %edi)

	movdqu	(%eax, %edi), %xmm0
	movdqu	16(%eax, %edi), %xmm1
	movdqu	32(%eax, %edi), %xmm2
	movdqu	48(%eax, %edi), %xmm3
	movdqa	%xmm0, (%edi)
	movaps	%xmm1, 16(%edi)
	movaps	%xmm2, 32(%edi)
	movaps	%xmm3, 48(%edi)
	leal	64(%edi), %edi
	cmp	%edi, %ebx
	ja	L(mm_main_loop_forward)

L(mm_copy_remaining_forward):
	addl	%edx, %ecx
	subl	%edi, %ecx
/* We copied all up till %edi position in the dst.
	In %ecx now is how many bytes are left to copy.
	Now we need to advance %esi. */
	leal	(%edi, %eax), %esi

L(mm_remaining_0_64_bytes_forward):
	cmp	$32, %ecx
	ja	L(mm_remaining_33_64_bytes_forward)
	cmp	$16, %ecx
	ja	L(mm_remaining_17_32_bytes_forward)
	testl	%ecx, %ecx
	.p2align 4,,2
	je	L(mm_return_pop_all)

	cmpb	$8, %cl
	ja	L(mm_remaining_9_16_bytes_forward)
	cmpb	$4, %cl
	.p2align 4,,5
	ja	L(mm_remaining_5_8_bytes_forward)
	cmpb	$2, %cl
	.p2align 4,,1
	ja	L(mm_remaining_3_4_bytes_forward)
	movzbl	-1(%esi,%ecx), %eax
	movzbl	(%esi), %ebx
	movb	%al, -1(%edi,%ecx)
	movb	%bl, (%edi)
	jmp	L(mm_return_pop_all)

L(mm_remaining_33_64_bytes_forward):
	movdqu	(%esi), %xmm0
	movdqu	16(%esi), %xmm1
	movdqu	-32(%esi, %ecx), %xmm2
	movdqu	-16(%esi, %ecx), %xmm3
	movdqu	%xmm0, (%edi)
	movdqu	%xmm1, 16(%edi)
	movdqu	%xmm2, -32(%edi, %ecx)
	movdqu	%xmm3, -16(%edi, %ecx)
	jmp	L(mm_return_pop_all)

L(mm_remaining_17_32_bytes_forward):
	movdqu	(%esi), %xmm0
	movdqu	-16(%esi, %ecx), %xmm1
	movdqu	%xmm0, (%edi)
	movdqu	%xmm1, -16(%edi, %ecx)
	jmp	L(mm_return_pop_all)

L(mm_remaining_9_16_bytes_forward):
	movq	(%esi), %xmm0
	movq	-8(%esi, %ecx), %xmm1
	movq	%xmm0, (%edi)
	movq	%xmm1, -8(%edi, %ecx)
	jmp	L(mm_return_pop_all)

L(mm_remaining_5_8_bytes_forward):
	movl	(%esi), %eax
	movl	-4(%esi,%ecx), %ebx
	movl	%eax, (%edi)
	movl	%ebx, -4(%edi,%ecx)
	jmp	L(mm_return_pop_all)

L(mm_remaining_3_4_bytes_forward):
	movzwl	-2(%esi,%ecx), %eax
	movzwl	(%esi), %ebx
	movw	%ax, -2(%edi,%ecx)
	movw	%bx, (%edi)
	jmp	L(mm_return_pop_all)

L(mm_len_0_16_bytes_forward):
	testb	$24, %cl
	jne	L(mm_len_9_16_bytes_forward)
	testb	$4, %cl
	.p2align 4,,5
	jne	L(mm_len_5_8_bytes_forward)
	testl	%ecx, %ecx
	.p2align 4,,2
	je	L(mm_return)
	testb	$2, %cl
	.p2align 4,,1
	jne	L(mm_len_2_4_bytes_forward)
	movzbl	-1(%eax,%ecx), %ebx
	movzbl	(%eax), %eax
	movb	%bl, -1(%edx,%ecx)
	movb	%al, (%edx)
	jmp	L(mm_return)

L(mm_len_2_4_bytes_forward):
	movzwl	-2(%eax,%ecx), %ebx
	movzwl	(%eax), %eax
	movw	%bx, -2(%edx,%ecx)
	movw	%ax, (%edx)
	jmp	L(mm_return)

L(mm_len_5_8_bytes_forward):
	movl	(%eax), %ebx
	movl	-4(%eax,%ecx), %eax
	movl	%ebx, (%edx)
	movl	%eax, -4(%edx,%ecx)
	jmp	L(mm_return)

L(mm_len_9_16_bytes_forward):
	movq	(%eax), %xmm0
	movq	-8(%eax, %ecx), %xmm1
	movq	%xmm0, (%edx)
	movq	%xmm1, -8(%edx, %ecx)
	jmp	L(mm_return)

L(mm_recalc_len):
/* Compute in %ecx how many bytes are left to copy after
	the main loop stops.  */
	movl	%ebx, %ecx
	subl	%edx, %ecx
/* The code for copying backwards.  */
L(mm_len_0_or_more_backward):

/* Now do checks for lengths. We do [0..16], [16..32], [32..64], [64..128]
	separately.  */
	cmp	$16, %ecx
	jbe	L(mm_len_0_16_bytes_backward)

	cmpl	$32, %ecx
	jg	L(mm_len_32_or_more_backward)

/* Copy [0..32] and return.  */
	movdqu	(%eax), %xmm0
	movdqu	-16(%eax, %ecx), %xmm1
	movdqu	%xmm0, (%edx)
	movdqu	%xmm1, -16(%edx, %ecx)
	jmp	L(mm_return)

L(mm_len_32_or_more_backward):
	cmpl	$64, %ecx
	jg	L(mm_len_64_or_more_backward)

/* Copy [0..64] and return.  */
	movdqu	(%eax), %xmm0
	movdqu	16(%eax), %xmm1
	movdqu	-16(%eax, %ecx), %xmm2
	movdqu	-32(%eax, %ecx), %xmm3
	movdqu	%xmm0, (%edx)
	movdqu	%xmm1, 16(%edx)
	movdqu	%xmm2, -16(%edx, %ecx)
	movdqu	%xmm3, -32(%edx, %ecx)
	jmp	L(mm_return)

L(mm_len_64_or_more_backward):
	cmpl	$128, %ecx
	jg	L(mm_len_128_or_more_backward)

/* Copy [0..128] and return.  */
	movdqu	(%eax), %xmm0
	movdqu	16(%eax), %xmm1
	movdqu	32(%eax), %xmm2
	movdqu	48(%eax), %xmm3
	movdqu	-64(%eax, %ecx), %xmm4
	movdqu	-48(%eax, %ecx), %xmm5
	movdqu	-32(%eax, %ecx), %xmm6
	movdqu	-16(%eax, %ecx), %xmm7
	movdqu	%xmm0, (%edx)
	movdqu	%xmm1, 16(%edx)
	movdqu	%xmm2, 32(%edx)
	movdqu	%xmm3, 48(%edx)
	movdqu	%xmm4, -64(%edx, %ecx)
	movdqu	%xmm5, -48(%edx, %ecx)
	movdqu	%xmm6, -32(%edx, %ecx)
	movdqu	%xmm7, -16(%edx, %ecx)
	jmp	L(mm_return)

L(mm_len_128_or_more_backward):
	PUSH (%esi)
	PUSH (%edi)

/* Aligning the address of destination. We need to save
	16 bits from the source in order not to overwrite them.  */
	movdqu	-16(%eax, %ecx), %xmm0
	movdqu	-32(%eax, %ecx), %xmm1
	movdqu	-48(%eax, %ecx), %xmm2
	movdqu	-64(%eax, %ecx), %xmm3

	leal	(%edx, %ecx), %edi
	andl	$-64, %edi

	movl	%eax, %esi
	subl	%edx, %esi

	movdqu	-16(%edi, %esi), %xmm4
	movdqu	-32(%edi, %esi), %xmm5
	movdqu	-48(%edi, %esi), %xmm6
	movdqu	-64(%edi, %esi), %xmm7

	movdqu	%xmm0, -16(%edx, %ecx)
	movdqu	%xmm1, -32(%edx, %ecx)
	movdqu	%xmm2, -48(%edx, %ecx)
	movdqu	%xmm3, -64(%edx, %ecx)
	movdqa	%xmm4, -16(%edi)
	movdqa	%xmm5, -32(%edi)
	movdqa	%xmm6, -48(%edi)
	movdqa	%xmm7, -64(%edi)
	leal	-64(%edi), %edi

	leal	64(%edx), %ebx
	andl	$-64, %ebx

	cmp	%edi, %ebx
	jae	L(mm_main_loop_backward_end)

	cmp	$SHARED_CACHE_SIZE_HALF, %ecx
	jae	L(mm_large_page_loop_backward)

	.p2align 4
L(mm_main_loop_backward):

	prefetcht0 -128(%edi, %esi)

	movdqu	-64(%edi, %esi), %xmm0
	movdqu	-48(%edi, %esi), %xmm1
	movdqu	-32(%edi, %esi), %xmm2
	movdqu	-16(%edi, %esi), %xmm3
	movdqa	%xmm0, -64(%edi)
	movdqa	%xmm1, -48(%edi)
	movdqa	%xmm2, -32(%edi)
	movdqa	%xmm3, -16(%edi)
	leal	-64(%edi), %edi
	cmp	%edi, %ebx
	jb	L(mm_main_loop_backward)
L(mm_main_loop_backward_end):
	POP (%edi)
	POP (%esi)
	jmp	L(mm_recalc_len)

/* Copy [0..16] and return.  */
L(mm_len_0_16_bytes_backward):
	testb	$24, %cl
	jnz	L(mm_len_9_16_bytes_backward)
	testb	$4, %cl
	.p2align 4,,5
	jnz	L(mm_len_5_8_bytes_backward)
	testl	%ecx, %ecx
	.p2align 4,,2
	je	L(mm_return)
	testb	$2, %cl
	.p2align 4,,1
	jne	L(mm_len_3_4_bytes_backward)
	movzbl	-1(%eax,%ecx), %ebx
	movzbl	(%eax), %eax
	movb	%bl, -1(%edx,%ecx)
	movb	%al, (%edx)
	jmp	L(mm_return)

L(mm_len_3_4_bytes_backward):
	movzwl	-2(%eax,%ecx), %ebx
	movzwl	(%eax), %eax
	movw	%bx, -2(%edx,%ecx)
	movw	%ax, (%edx)
	jmp	L(mm_return)

L(mm_len_9_16_bytes_backward):
	PUSH (%esi)
	movl	-4(%eax,%ecx), %ebx
	movl	-8(%eax,%ecx), %esi
	movl	%ebx, -4(%edx,%ecx)
	movl	%esi, -8(%edx,%ecx)
	subl	$8, %ecx
	POP (%esi)
	jmp	L(mm_len_0_16_bytes_backward)

L(mm_len_5_8_bytes_backward):
	movl	(%eax), %ebx
	movl	-4(%eax,%ecx), %eax
	movl	%ebx, (%edx)
	movl	%eax, -4(%edx,%ecx)

L(mm_return):
	movl	%edx, %eax
	RETURN

L(mm_return_pop_all):
	movl	%edx, %eax
	POP (%edi)
	POP (%esi)
	RETURN

/* Big length copy forward part.  */

	.p2align 4
L(mm_large_page_loop_forward):
	movdqu	(%eax, %edi), %xmm0
	movdqu	16(%eax, %edi), %xmm1
	movdqu	32(%eax, %edi), %xmm2
	movdqu	48(%eax, %edi), %xmm3
	movntdq	%xmm0, (%edi)
	movntdq	%xmm1, 16(%edi)
	movntdq	%xmm2, 32(%edi)
	movntdq	%xmm3, 48(%edi)
	leal	64(%edi), %edi
	cmp	%edi, %ebx
	ja	L(mm_large_page_loop_forward)
	sfence
	jmp	L(mm_copy_remaining_forward)

/* Big length copy backward part.  */
	.p2align 4
L(mm_large_page_loop_backward):
	movdqu	-64(%edi, %esi), %xmm0
	movdqu	-48(%edi, %esi), %xmm1
	movdqu	-32(%edi, %esi), %xmm2
	movdqu	-16(%edi, %esi), %xmm3
	movntdq	%xmm0, -64(%edi)
	movntdq	%xmm1, -48(%edi)
	movntdq	%xmm2, -32(%edi)
	movntdq	%xmm3, -16(%edi)
	leal	-64(%edi), %edi
	cmp	%edi, %ebx
	jb	L(mm_large_page_loop_backward)
	sfence
	POP (%edi)
	POP (%esi)
	jmp	L(mm_recalc_len)

END (MEMMOVE)
