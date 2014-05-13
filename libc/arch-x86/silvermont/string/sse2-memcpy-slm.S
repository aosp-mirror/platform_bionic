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
# define MEMCPY	memcpy
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

#define DEST		PARMS
#define SRC		DEST+4
#define LEN		SRC+4

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
ENTRY (MEMCPY)
	ENTRANCE
	movl	LEN(%esp), %ecx
	movl	SRC(%esp), %eax
	movl	DEST(%esp), %edx

	cmp	%eax, %edx
	je	L(return)

	cmp	$16, %ecx
	jbe	L(len_0_16_bytes)

	cmp     $SHARED_CACHE_SIZE_HALF, %ecx
	jae     L(large_page)

	movdqu	(%eax), %xmm0
	movdqu	-16(%eax, %ecx), %xmm1
	cmpl    $32, %ecx
	movdqu	%xmm0, (%edx)
	movdqu	%xmm1, -16(%edx, %ecx)
	jbe	L(return)

	movdqu	16(%eax), %xmm0
	movdqu	-32(%eax, %ecx), %xmm1
	cmpl    $64, %ecx
	movdqu	%xmm0, 16(%edx)
	movdqu	%xmm1, -32(%edx, %ecx)
	jbe	L(return)

	movdqu	32(%eax), %xmm0
	movdqu	48(%eax), %xmm1
	movdqu	-48(%eax, %ecx), %xmm2
	movdqu	-64(%eax, %ecx), %xmm3
	cmpl    $128, %ecx
	movdqu	%xmm0, 32(%edx)
	movdqu	%xmm1, 48(%edx)
	movdqu	%xmm2, -48(%edx, %ecx)
	movdqu	%xmm3, -64(%edx, %ecx)
	jbe	L(return)

/* Now the main loop: we align the address of the destination.  */
	leal	64(%edx), %ebx
	andl	$-64, %ebx

	addl	%edx, %ecx
	andl	$-64, %ecx

	subl	%edx, %eax

/* We should stop two iterations before the termination
	(in order not to misprefetch).  */
	subl	$64, %ecx
	cmpl	%ebx, %ecx
	je	L(main_loop_just_one_iteration)

	subl	$64, %ecx
	cmpl	%ebx, %ecx
	je	L(main_loop_last_two_iterations)


	.p2align 4
L(main_loop_cache):

	prefetcht0 128(%ebx, %eax)

	movdqu	(%ebx, %eax), %xmm0
	movdqu	16(%ebx, %eax), %xmm1
	movdqu	32(%ebx, %eax), %xmm2
	movdqu	48(%ebx, %eax), %xmm3
	movdqa	%xmm0, (%ebx)
	movdqa	%xmm1, 16(%ebx)
	movdqa	%xmm2, 32(%ebx)
	movdqa	%xmm3, 48(%ebx)
	lea	64(%ebx), %ebx
	cmpl	%ebx, %ecx
	jne	L(main_loop_cache)

L(main_loop_last_two_iterations):
	movdqu	(%ebx, %eax), %xmm0
	movdqu	16(%ebx, %eax), %xmm1
	movdqu	32(%ebx, %eax), %xmm2
	movdqu	48(%ebx, %eax), %xmm3
	movdqu	64(%ebx, %eax), %xmm4
	movdqu	80(%ebx, %eax), %xmm5
	movdqu	96(%ebx, %eax), %xmm6
	movdqu	112(%ebx, %eax), %xmm7
	movdqa	%xmm0, (%ebx)
	movdqa	%xmm1, 16(%ebx)
	movdqa	%xmm2, 32(%ebx)
	movdqa	%xmm3, 48(%ebx)
	movdqa	%xmm4, 64(%ebx)
	movdqa	%xmm5, 80(%ebx)
	movdqa	%xmm6, 96(%ebx)
	movdqa	%xmm7, 112(%ebx)
	jmp	L(return)

L(main_loop_just_one_iteration):
	movdqu	(%ebx, %eax), %xmm0
	movdqu	16(%ebx, %eax), %xmm1
	movdqu	32(%ebx, %eax), %xmm2
	movdqu	48(%ebx, %eax), %xmm3
	movdqa	%xmm0, (%ebx)
	movdqa	%xmm1, 16(%ebx)
	movdqa	%xmm2, 32(%ebx)
	movdqa	%xmm3, 48(%ebx)
	jmp	L(return)

L(large_page):
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

	movdqu	64(%eax), %xmm0
	movdqu	80(%eax), %xmm1
	movdqu	96(%eax), %xmm2
	movdqu	112(%eax), %xmm3
	movdqu	-128(%eax, %ecx), %xmm4
	movdqu	-112(%eax, %ecx), %xmm5
	movdqu	-96(%eax, %ecx), %xmm6
	movdqu	-80(%eax, %ecx), %xmm7
	movdqu	%xmm0, 64(%edx)
	movdqu	%xmm1, 80(%edx)
	movdqu	%xmm2, 96(%edx)
	movdqu	%xmm3, 112(%edx)
	movdqu	%xmm4, -128(%edx, %ecx)
	movdqu	%xmm5, -112(%edx, %ecx)
	movdqu	%xmm6, -96(%edx, %ecx)
	movdqu	%xmm7, -80(%edx, %ecx)

/* Now the main loop with non temporal stores. We align
	the address of the destination.  */
	leal	128(%edx), %ebx
	andl	$-128, %ebx

	addl	%edx, %ecx
	andl	$-128, %ecx

	subl	%edx, %eax

	.p2align 4
L(main_loop_large_page):
	movdqu	(%ebx, %eax), %xmm0
	movdqu	16(%ebx, %eax), %xmm1
	movdqu	32(%ebx, %eax), %xmm2
	movdqu	48(%ebx, %eax), %xmm3
	movdqu	64(%ebx, %eax), %xmm4
	movdqu	80(%ebx, %eax), %xmm5
	movdqu	96(%ebx, %eax), %xmm6
	movdqu	112(%ebx, %eax), %xmm7
	movntdq	%xmm0, (%ebx)
	movntdq	%xmm1, 16(%ebx)
	movntdq	%xmm2, 32(%ebx)
	movntdq	%xmm3, 48(%ebx)
	movntdq	%xmm4, 64(%ebx)
	movntdq	%xmm5, 80(%ebx)
	movntdq	%xmm6, 96(%ebx)
	movntdq	%xmm7, 112(%ebx)
	lea	128(%ebx), %ebx
	cmpl	%ebx, %ecx
	jne	L(main_loop_large_page)
	sfence
	jmp	L(return)

L(len_0_16_bytes):
	testb	$24, %cl
	jne	L(len_9_16_bytes)
	testb	$4, %cl
	.p2align 4,,5
	jne	L(len_5_8_bytes)
	testl	%ecx, %ecx
	.p2align 4,,2
	je	L(return)
	movzbl	(%eax), %ebx
	testb	$2, %cl
	movb	%bl, (%edx)
	je	L(return)
	movzwl	-2(%eax,%ecx), %ebx
	movw	%bx, -2(%edx,%ecx)
	jmp	L(return)

L(len_9_16_bytes):
	movq	(%eax), %xmm0
	movq	-8(%eax, %ecx), %xmm1
	movq	%xmm0, (%edx)
	movq	%xmm1, -8(%edx, %ecx)
	jmp	L(return)

L(len_5_8_bytes):
	movl	(%eax), %ebx
	movl	%ebx, (%edx)
	movl	-4(%eax,%ecx), %ebx
	movl	%ebx, -4(%edx,%ecx)
	jmp	L(return)

L(return):
	movl	%edx, %eax
	RETURN

END (MEMCPY)
