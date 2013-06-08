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

#define CFI_PUSH(REG)	\
	cfi_adjust_cfa_offset (4);	\
	cfi_rel_offset (REG, 0)

#define CFI_POP(REG)	\
	cfi_adjust_cfa_offset (-4);	\
	cfi_restore (REG)

#define PUSH(REG) pushl REG; CFI_PUSH (REG)
#define POP(REG) popl REG; CFI_POP (REG)

#define ENTRANCE PUSH(%esi); PUSH(%edi)
#define RETURN  POP(%edi); POP(%esi); ret; CFI_PUSH(%esi); CFI_PUSH(%edi);
#define PARMS  4
#define STR1  PARMS
#define STR2  STR1+4

	.text
ENTRY (wcscmp)
/*
	* This implementation uses SSE to compare up to 16 bytes at a time.
*/
	mov	STR1(%esp), %edx
	mov	STR2(%esp), %eax

	mov	(%eax), %ecx
	cmp	%ecx, (%edx)
	jne	L(neq)
	test	%ecx, %ecx
	jz	L(eq)

	mov	4(%eax), %ecx
	cmp	%ecx, 4(%edx)
	jne	L(neq)
	test	%ecx, %ecx
	jz	L(eq)

	mov	8(%eax), %ecx
	cmp	%ecx, 8(%edx)
	jne	L(neq)
	test	%ecx, %ecx
	jz	L(eq)

	mov	12(%eax), %ecx
	cmp	%ecx, 12(%edx)
	jne	L(neq)
	test	%ecx, %ecx
	jz	L(eq)

	ENTRANCE
	add	$16, %eax
	add	$16, %edx

	mov	%eax, %esi
	mov	%edx, %edi
	pxor	%xmm0, %xmm0		/* clear %xmm0 for null char checks */
	mov	%al, %ch
	mov	%dl, %cl
	and	$63, %eax		/* esi alignment in cache line */
	and	$63, %edx		/* edi alignment in cache line */
	and	$15, %cl
	jz	L(continue_00)
	cmp	$16, %edx
	jb	L(continue_0)
	cmp	$32, %edx
	jb	L(continue_16)
	cmp	$48, %edx
	jb	L(continue_32)

L(continue_48):
	and	$15, %ch
	jz	L(continue_48_00)
	cmp	$16, %eax
	jb	L(continue_0_48)
	cmp	$32, %eax
	jb	L(continue_16_48)
	cmp	$48, %eax
	jb	L(continue_32_48)

	.p2align 4
L(continue_48_48):
	mov	(%esi), %ecx
	cmp	%ecx, (%edi)
	jne	L(nequal)
	test	%ecx, %ecx
	jz	L(equal)

	mov	4(%esi), %ecx
	cmp	%ecx, 4(%edi)
	jne	L(nequal)
	test	%ecx, %ecx
	jz	L(equal)

	mov	8(%esi), %ecx
	cmp	%ecx, 8(%edi)
	jne	L(nequal)
	test	%ecx, %ecx
	jz	L(equal)

	mov	12(%esi), %ecx
	cmp	%ecx, 12(%edi)
	jne	L(nequal)
	test	%ecx, %ecx
	jz	L(equal)

	movdqu	16(%edi), %xmm1
	movdqu	16(%esi), %xmm2
	pcmpeqd	%xmm1, %xmm0		/* Any null double_word? */
	pcmpeqd	%xmm2, %xmm1		/* compare first 4 double_words for equality */
	psubb	%xmm0, %xmm1		/* packed sub of comparison results*/
	pmovmskb %xmm1, %edx
	sub	$0xffff, %edx		/* if first 4 double_words are same, edx == 0xffff */
	jnz	L(less4_double_words_16)

	movdqu	32(%edi), %xmm1
	movdqu	32(%esi), %xmm2
	pcmpeqd	%xmm1, %xmm0		/* Any null double_word? */
	pcmpeqd	%xmm2, %xmm1		/* compare first 4 double_words for equality */
	psubb	%xmm0, %xmm1		/* packed sub of comparison results*/
	pmovmskb %xmm1, %edx
	sub	$0xffff, %edx		/* if first 4 double_words are same, edx == 0xffff */
	jnz	L(less4_double_words_32)

	movdqu	48(%edi), %xmm1
	movdqu	48(%esi), %xmm2
	pcmpeqd	%xmm1, %xmm0		/* Any null double_word? */
	pcmpeqd	%xmm2, %xmm1		/* compare first 4 double_words for equality */
	psubb	%xmm0, %xmm1		/* packed sub of comparison results*/
	pmovmskb %xmm1, %edx
	sub	$0xffff, %edx		/* if first 4 double_words are same, edx == 0xffff */
	jnz	L(less4_double_words_48)

	add	$64, %esi
	add	$64, %edi
	jmp	L(continue_48_48)

L(continue_0):
	and	$15, %ch
	jz	L(continue_0_00)
	cmp	$16, %eax
	jb	L(continue_0_0)
	cmp	$32, %eax
	jb	L(continue_0_16)
	cmp	$48, %eax
	jb	L(continue_0_32)

	.p2align 4
L(continue_0_48):
	mov	(%esi), %ecx
	cmp	%ecx, (%edi)
	jne	L(nequal)
	test	%ecx, %ecx
	jz	L(equal)

	mov	4(%esi), %ecx
	cmp	%ecx, 4(%edi)
	jne	L(nequal)
	test	%ecx, %ecx
	jz	L(equal)

	mov	8(%esi), %ecx
	cmp	%ecx, 8(%edi)
	jne	L(nequal)
	test	%ecx, %ecx
	jz	L(equal)

	mov	12(%esi), %ecx
	cmp	%ecx, 12(%edi)
	jne	L(nequal)
	test	%ecx, %ecx
	jz	L(equal)

	movdqu	16(%edi), %xmm1
	movdqu	16(%esi), %xmm2
	pcmpeqd	%xmm1, %xmm0		/* Any null double_word? */
	pcmpeqd	%xmm2, %xmm1		/* compare first 4 double_words for equality */
	psubb	%xmm0, %xmm1		/* packed sub of comparison results*/
	pmovmskb %xmm1, %edx
	sub	$0xffff, %edx		/* if first 4 double_words are same, edx == 0xffff */
	jnz	L(less4_double_words_16)

	movdqu	32(%edi), %xmm1
	movdqu	32(%esi), %xmm2
	pcmpeqd	%xmm1, %xmm0		/* Any null double_word? */
	pcmpeqd	%xmm2, %xmm1		/* compare first 4 double_words for equality */
	psubb	%xmm0, %xmm1		/* packed sub of comparison results*/
	pmovmskb %xmm1, %edx
	sub	$0xffff, %edx		/* if first 4 double_words are same, edx == 0xffff */
	jnz	L(less4_double_words_32)

	mov	48(%esi), %ecx
	cmp	%ecx, 48(%edi)
	jne	L(nequal)
	test	%ecx, %ecx
	jz	L(equal)

	mov	52(%esi), %ecx
	cmp	%ecx, 52(%edi)
	jne	L(nequal)
	test	%ecx, %ecx
	jz	L(equal)

	mov	56(%esi), %ecx
	cmp	%ecx, 56(%edi)
	jne	L(nequal)
	test	%ecx, %ecx
	jz	L(equal)

	mov	60(%esi), %ecx
	cmp	%ecx, 60(%edi)
	jne	L(nequal)
	test	%ecx, %ecx
	jz	L(equal)

	add	$64, %esi
	add	$64, %edi
	jmp	L(continue_0_48)

	.p2align 4
L(continue_00):
	and	$15, %ch
	jz	L(continue_00_00)
	cmp	$16, %eax
	jb	L(continue_00_0)
	cmp	$32, %eax
	jb	L(continue_00_16)
	cmp	$48, %eax
	jb	L(continue_00_32)

	.p2align 4
L(continue_00_48):
	pcmpeqd	(%edi), %xmm0
	mov	(%edi), %eax
	pmovmskb %xmm0, %ecx
	test	%ecx, %ecx
	jnz	L(less4_double_words1)

	cmp	(%esi), %eax
	jne	L(nequal)

	mov	4(%edi), %eax
	cmp	4(%esi), %eax
	jne	L(nequal)

	mov	8(%edi), %eax
	cmp	8(%esi), %eax
	jne	L(nequal)

	mov	12(%edi), %eax
	cmp	12(%esi), %eax
	jne	L(nequal)

	movdqu	16(%esi), %xmm2
	pcmpeqd	%xmm2, %xmm0		/* Any null double_word? */
	pcmpeqd	16(%edi), %xmm2		/* compare first 4 double_words for equality */
	psubb	%xmm0, %xmm2		/* packed sub of comparison results*/
	pmovmskb %xmm2, %edx
	sub	$0xffff, %edx		/* if first 4 double_words are same, edx == 0xffff */
	jnz	L(less4_double_words_16)

	movdqu	32(%esi), %xmm2
	pcmpeqd	%xmm2, %xmm0		/* Any null double_word? */
	pcmpeqd	32(%edi), %xmm2		/* compare first 4 double_words for equality */
	psubb	%xmm0, %xmm2		/* packed sub of comparison results*/
	pmovmskb %xmm2, %edx
	sub	$0xffff, %edx		/* if first 4 double_words are same, edx == 0xffff */
	jnz	L(less4_double_words_32)

	movdqu	48(%esi), %xmm2
	pcmpeqd	%xmm2, %xmm0		/* Any null double_word? */
	pcmpeqd	48(%edi), %xmm2		/* compare first 4 double_words for equality */
	psubb	%xmm0, %xmm2		/* packed sub of comparison results*/
	pmovmskb %xmm2, %edx
	sub	$0xffff, %edx		/* if first 4 double_words are same, edx == 0xffff */
	jnz	L(less4_double_words_48)

	add	$64, %esi
	add	$64, %edi
	jmp	L(continue_00_48)

	.p2align 4
L(continue_32):
	and	$15, %ch
	jz	L(continue_32_00)
	cmp	$16, %eax
	jb	L(continue_0_32)
	cmp	$32, %eax
	jb	L(continue_16_32)
	cmp	$48, %eax
	jb	L(continue_32_32)

	.p2align 4
L(continue_32_48):
	mov	(%esi), %ecx
	cmp	%ecx, (%edi)
	jne	L(nequal)
	test	%ecx, %ecx
	jz	L(equal)

	mov	4(%esi), %ecx
	cmp	%ecx, 4(%edi)
	jne	L(nequal)
	test	%ecx, %ecx
	jz	L(equal)

	mov	8(%esi), %ecx
	cmp	%ecx, 8(%edi)
	jne	L(nequal)
	test	%ecx, %ecx
	jz	L(equal)

	mov	12(%esi), %ecx
	cmp	%ecx, 12(%edi)
	jne	L(nequal)
	test	%ecx, %ecx
	jz	L(equal)

	mov	16(%esi), %ecx
	cmp	%ecx, 16(%edi)
	jne	L(nequal)
	test	%ecx, %ecx
	jz	L(equal)

	mov	20(%esi), %ecx
	cmp	%ecx, 20(%edi)
	jne	L(nequal)
	test	%ecx, %ecx
	jz	L(equal)

	mov	24(%esi), %ecx
	cmp	%ecx, 24(%edi)
	jne	L(nequal)
	test	%ecx, %ecx
	jz	L(equal)

	mov	28(%esi), %ecx
	cmp	%ecx, 28(%edi)
	jne	L(nequal)
	test	%ecx, %ecx
	jz	L(equal)

	movdqu	32(%edi), %xmm1
	movdqu	32(%esi), %xmm2
	pcmpeqd	%xmm1, %xmm0		/* Any null double_word? */
	pcmpeqd	%xmm2, %xmm1		/* compare first 4 double_words for equality */
	psubb	%xmm0, %xmm1		/* packed sub of comparison results*/
	pmovmskb %xmm1, %edx
	sub	$0xffff, %edx		/* if first 4 double_words are same, edx == 0xffff */
	jnz	L(less4_double_words_32)

	movdqu	48(%edi), %xmm1
	movdqu	48(%esi), %xmm2
	pcmpeqd	%xmm1, %xmm0		/* Any null double_word? */
	pcmpeqd	%xmm2, %xmm1		/* compare first 4 double_words for equality */
	psubb	%xmm0, %xmm1		/* packed sub of comparison results*/
	pmovmskb %xmm1, %edx
	sub	$0xffff, %edx		/* if first 4 double_words are same, edx == 0xffff */
	jnz	L(less4_double_words_48)

	add	$64, %esi
	add	$64, %edi
	jmp	L(continue_32_48)

	.p2align 4
L(continue_16):
	and	$15, %ch
	jz	L(continue_16_00)
	cmp	$16, %eax
	jb	L(continue_0_16)
	cmp	$32, %eax
	jb	L(continue_16_16)
	cmp	$48, %eax
	jb	L(continue_16_32)

	.p2align 4
L(continue_16_48):
	mov	(%esi), %ecx
	cmp	%ecx, (%edi)
	jne	L(nequal)
	test	%ecx, %ecx
	jz	L(equal)

	mov	4(%esi), %ecx
	cmp	%ecx, 4(%edi)
	jne	L(nequal)
	test	%ecx, %ecx
	jz	L(equal)

	mov	8(%esi), %ecx
	cmp	%ecx, 8(%edi)
	jne	L(nequal)
	test	%ecx, %ecx
	jz	L(equal)

	mov	12(%esi), %ecx
	cmp	%ecx, 12(%edi)
	jne	L(nequal)
	test	%ecx, %ecx
	jz	L(equal)

	movdqu	16(%edi), %xmm1
	movdqu	16(%esi), %xmm2
	pcmpeqd	%xmm1, %xmm0		/* Any null double_word? */
	pcmpeqd	%xmm2, %xmm1		/* compare first 4 double_words for equality */
	psubb	%xmm0, %xmm1		/* packed sub of comparison results*/
	pmovmskb %xmm1, %edx
	sub	$0xffff, %edx		/* if first 4 double_words are same, edx == 0xffff */
	jnz	L(less4_double_words_16)

	mov	32(%esi), %ecx
	cmp	%ecx, 32(%edi)
	jne	L(nequal)
	test	%ecx, %ecx
	jz	L(equal)

	mov	36(%esi), %ecx
	cmp	%ecx, 36(%edi)
	jne	L(nequal)
	test	%ecx, %ecx
	jz	L(equal)

	mov	40(%esi), %ecx
	cmp	%ecx, 40(%edi)
	jne	L(nequal)
	test	%ecx, %ecx
	jz	L(equal)

	mov	44(%esi), %ecx
	cmp	%ecx, 44(%edi)
	jne	L(nequal)
	test	%ecx, %ecx
	jz	L(equal)

	movdqu	48(%edi), %xmm1
	movdqu	48(%esi), %xmm2
	pcmpeqd	%xmm1, %xmm0		/* Any null double_word? */
	pcmpeqd	%xmm2, %xmm1		/* compare first 4 double_words for equality */
	psubb	%xmm0, %xmm1		/* packed sub of comparison results*/
	pmovmskb %xmm1, %edx
	sub	$0xffff, %edx		/* if first 4 double_words are same, edx == 0xffff */
	jnz	L(less4_double_words_48)

	add	$64, %esi
	add	$64, %edi
	jmp	L(continue_16_48)

	.p2align 4
L(continue_00_00):
	movdqa	(%edi), %xmm1
	pcmpeqd	%xmm1, %xmm0		/* Any null double_word? */
	pcmpeqd	(%esi), %xmm1		/* compare first 4 double_words for equality */
	psubb	%xmm0, %xmm1		/* packed sub of comparison results*/
	pmovmskb %xmm1, %edx
	sub	$0xffff, %edx		/* if first 4 double_words are same, edx == 0xffff */
	jnz	L(less4_double_words)

	movdqa	16(%edi), %xmm3
	pcmpeqd	%xmm3, %xmm0		/* Any null double_word? */
	pcmpeqd	16(%esi), %xmm3		/* compare first 4 double_words for equality */
	psubb	%xmm0, %xmm3		/* packed sub of comparison results*/
	pmovmskb %xmm3, %edx
	sub	$0xffff, %edx		/* if first 4 double_words are same, edx == 0xffff */
	jnz	L(less4_double_words_16)

	movdqa	32(%edi), %xmm5
	pcmpeqd	%xmm5, %xmm0		/* Any null double_word? */
	pcmpeqd	32(%esi), %xmm5		/* compare first 4 double_words for equality */
	psubb	%xmm0, %xmm5		/* packed sub of comparison results*/
	pmovmskb %xmm5, %edx
	sub	$0xffff, %edx		/* if first 4 double_words are same, edx == 0xffff */
	jnz	L(less4_double_words_32)

	movdqa	48(%edi), %xmm1
	pcmpeqd	%xmm1, %xmm0		/* Any null double_word? */
	pcmpeqd	48(%esi), %xmm1		/* compare first 4 double_words for equality */
	psubb	%xmm0, %xmm1		/* packed sub of comparison results*/
	pmovmskb %xmm1, %edx
	sub	$0xffff, %edx		/* if first 4 double_words are same, edx == 0xffff */
	jnz	L(less4_double_words_48)

	add	$64, %esi
	add	$64, %edi
	jmp	L(continue_00_00)

	.p2align 4
L(continue_00_32):
	movdqu	(%esi), %xmm2
	pcmpeqd	%xmm2, %xmm0		/* Any null double_word? */
	pcmpeqd	(%edi), %xmm2		/* compare first 4 double_words for equality */
	psubb	%xmm0, %xmm2		/* packed sub of comparison results*/
	pmovmskb %xmm2, %edx
	sub	$0xffff, %edx		/* if first 4 double_words are same, edx == 0xffff */
	jnz	L(less4_double_words)

	add	$16, %esi
	add	$16, %edi
	jmp	L(continue_00_48)

	.p2align 4
L(continue_00_16):
	movdqu	(%esi), %xmm2
	pcmpeqd	%xmm2, %xmm0		/* Any null double_word? */
	pcmpeqd	(%edi), %xmm2		/* compare first 4 double_words for equality */
	psubb	%xmm0, %xmm2		/* packed sub of comparison results*/
	pmovmskb %xmm2, %edx
	sub	$0xffff, %edx		/* if first 4 double_words are same, edx == 0xffff */
	jnz	L(less4_double_words)

	movdqu	16(%esi), %xmm2
	pcmpeqd	%xmm2, %xmm0		/* Any null double_word? */
	pcmpeqd	16(%edi), %xmm2		/* compare first 4 double_words for equality */
	psubb	%xmm0, %xmm2		/* packed sub of comparison results*/
	pmovmskb %xmm2, %edx
	sub	$0xffff, %edx		/* if first 4 double_words are same, edx == 0xffff */
	jnz	L(less4_double_words_16)

	add	$32, %esi
	add	$32, %edi
	jmp	L(continue_00_48)

	.p2align 4
L(continue_00_0):
	movdqu	(%esi), %xmm2
	pcmpeqd	%xmm2, %xmm0		/* Any null double_word? */
	pcmpeqd	(%edi), %xmm2		/* compare first 4 double_words for equality */
	psubb	%xmm0, %xmm2		/* packed sub of comparison results*/
	pmovmskb %xmm2, %edx
	sub	$0xffff, %edx		/* if first 4 double_words are same, edx == 0xffff */
	jnz	L(less4_double_words)

	movdqu	16(%esi), %xmm2
	pcmpeqd	%xmm2, %xmm0		/* Any null double_word? */
	pcmpeqd	16(%edi), %xmm2		/* compare first 4 double_words for equality */
	psubb	%xmm0, %xmm2		/* packed sub of comparison results*/
	pmovmskb %xmm2, %edx
	sub	$0xffff, %edx		/* if first 4 double_words are same, edx == 0xffff */
	jnz	L(less4_double_words_16)

	movdqu	32(%esi), %xmm2
	pcmpeqd	%xmm2, %xmm0		/* Any null double_word? */
	pcmpeqd	32(%edi), %xmm2		/* compare first 4 double_words for equality */
	psubb	%xmm0, %xmm2		/* packed sub of comparison results*/
	pmovmskb %xmm2, %edx
	sub	$0xffff, %edx		/* if first 4 double_words are same, edx == 0xffff */
	jnz	L(less4_double_words_32)

	add	$48, %esi
	add	$48, %edi
	jmp	L(continue_00_48)

	.p2align 4
L(continue_48_00):
	pcmpeqd	(%esi), %xmm0
	mov	(%edi), %eax
	pmovmskb %xmm0, %ecx
	test	%ecx, %ecx
	jnz	L(less4_double_words1)

	cmp	(%esi), %eax
	jne	L(nequal)

	mov	4(%edi), %eax
	cmp	4(%esi), %eax
	jne	L(nequal)

	mov	8(%edi), %eax
	cmp	8(%esi), %eax
	jne	L(nequal)

	mov	12(%edi), %eax
	cmp	12(%esi), %eax
	jne	L(nequal)

	movdqu	16(%edi), %xmm1
	pcmpeqd	%xmm1, %xmm0		/* Any null double_word? */
	pcmpeqd	16(%esi), %xmm1		/* compare first 4 double_words for equality */
	psubb	%xmm0, %xmm1		/* packed sub of comparison results*/
	pmovmskb %xmm1, %edx
	sub	$0xffff, %edx		/* if first 4 double_words are same, edx == 0xffff */
	jnz	L(less4_double_words_16)

	movdqu	32(%edi), %xmm1
	pcmpeqd	%xmm1, %xmm0		/* Any null double_word? */
	pcmpeqd	32(%esi), %xmm1		/* compare first 4 double_words for equality */
	psubb	%xmm0, %xmm1		/* packed sub of comparison results*/
	pmovmskb %xmm1, %edx
	sub	$0xffff, %edx		/* if first 4 double_words are same, edx == 0xffff */
	jnz	L(less4_double_words_32)

	movdqu	48(%edi), %xmm1
	pcmpeqd	%xmm1, %xmm0		/* Any null double_word? */
	pcmpeqd	48(%esi), %xmm1		/* compare first 4 double_words for equality */
	psubb	%xmm0, %xmm1		/* packed sub of comparison results*/
	pmovmskb %xmm1, %edx
	sub	$0xffff, %edx		/* if first 4 double_words are same, edx == 0xffff */
	jnz	L(less4_double_words_48)

	add	$64, %esi
	add	$64, %edi
	jmp	L(continue_48_00)

	.p2align 4
L(continue_32_00):
	movdqu	(%edi), %xmm1
	pcmpeqd	%xmm1, %xmm0		/* Any null double_word? */
	pcmpeqd	(%esi), %xmm1		/* compare first 4 double_words for equality */
	psubb	%xmm0, %xmm1		/* packed sub of comparison results*/
	pmovmskb %xmm1, %edx
	sub	$0xffff, %edx		/* if first 4 double_words are same, edx == 0xffff */
	jnz	L(less4_double_words)

	add	$16, %esi
	add	$16, %edi
	jmp	L(continue_48_00)

	.p2align 4
L(continue_16_00):
	movdqu	(%edi), %xmm1
	pcmpeqd	%xmm1, %xmm0		/* Any null double_word? */
	pcmpeqd	(%esi), %xmm1		/* compare first 4 double_words for equality */
	psubb	%xmm0, %xmm1		/* packed sub of comparison results*/
	pmovmskb %xmm1, %edx
	sub	$0xffff, %edx		/* if first 4 double_words are same, edx == 0xffff */
	jnz	L(less4_double_words)

	movdqu	16(%edi), %xmm1
	pcmpeqd	%xmm1, %xmm0		/* Any null double_word? */
	pcmpeqd	16(%esi), %xmm1		/* compare first 4 double_words for equality */
	psubb	%xmm0, %xmm1		/* packed sub of comparison results*/
	pmovmskb %xmm1, %edx
	sub	$0xffff, %edx		/* if first 4 double_words are same, edx == 0xffff */
	jnz	L(less4_double_words_16)

	add	$32, %esi
	add	$32, %edi
	jmp	L(continue_48_00)

	.p2align 4
L(continue_0_00):
	movdqu	(%edi), %xmm1
	pcmpeqd	%xmm1, %xmm0		/* Any null double_word? */
	pcmpeqd	(%esi), %xmm1		/* compare first 4 double_words for equality */
	psubb	%xmm0, %xmm1		/* packed sub of comparison results*/
	pmovmskb %xmm1, %edx
	sub	$0xffff, %edx		/* if first 4 double_words are same, edx == 0xffff */
	jnz	L(less4_double_words)

	movdqu	16(%edi), %xmm1
	pcmpeqd	%xmm1, %xmm0		/* Any null double_word? */
	pcmpeqd	16(%esi), %xmm1		/* compare first 4 double_words for equality */
	psubb	%xmm0, %xmm1		/* packed sub of comparison results*/
	pmovmskb %xmm1, %edx
	sub	$0xffff, %edx		/* if first 4 double_words are same, edx == 0xffff */
	jnz	L(less4_double_words_16)

	movdqu	32(%edi), %xmm1
	pcmpeqd	%xmm1, %xmm0		/* Any null double_word? */
	pcmpeqd	32(%esi), %xmm1		/* compare first 4 double_words for equality */
	psubb	%xmm0, %xmm1		/* packed sub of comparison results*/
	pmovmskb %xmm1, %edx
	sub	$0xffff, %edx		/* if first 4 double_words are same, edx == 0xffff */
	jnz	L(less4_double_words_32)

	add	$48, %esi
	add	$48, %edi
	jmp	L(continue_48_00)

	.p2align 4
L(continue_32_32):
	movdqu	(%edi), %xmm1
	movdqu	(%esi), %xmm2
	pcmpeqd	%xmm1, %xmm0		/* Any null double_word? */
	pcmpeqd	%xmm2, %xmm1		/* compare first 4 double_words for equality */
	psubb	%xmm0, %xmm1		/* packed sub of comparison results*/
	pmovmskb %xmm1, %edx
	sub	$0xffff, %edx		/* if first 4 double_words are same, edx == 0xffff */
	jnz	L(less4_double_words)

	add	$16, %esi
	add	$16, %edi
	jmp	L(continue_48_48)

	.p2align 4
L(continue_16_16):
	movdqu	(%edi), %xmm1
	movdqu	(%esi), %xmm2
	pcmpeqd	%xmm1, %xmm0		/* Any null double_word? */
	pcmpeqd	%xmm2, %xmm1		/* compare first 4 double_words for equality */
	psubb	%xmm0, %xmm1		/* packed sub of comparison results*/
	pmovmskb %xmm1, %edx
	sub	$0xffff, %edx		/* if first 4 double_words are same, edx == 0xffff */
	jnz	L(less4_double_words)

	movdqu	16(%edi), %xmm3
	movdqu	16(%esi), %xmm4
	pcmpeqd	%xmm3, %xmm0		/* Any null double_word? */
	pcmpeqd	%xmm4, %xmm3		/* compare first 4 double_words for equality */
	psubb	%xmm0, %xmm3		/* packed sub of comparison results*/
	pmovmskb %xmm3, %edx
	sub	$0xffff, %edx		/* if first 4 double_words are same, edx == 0xffff */
	jnz	L(less4_double_words_16)

	add	$32, %esi
	add	$32, %edi
	jmp	L(continue_48_48)

	.p2align 4
L(continue_0_0):
	movdqu	(%edi), %xmm1
	movdqu	(%esi), %xmm2
	pcmpeqd	%xmm1, %xmm0		/* Any null double_word? */
	pcmpeqd	%xmm2, %xmm1		/* compare first 4 double_words for equality */
	psubb	%xmm0, %xmm1		/* packed sub of comparison results*/
	pmovmskb %xmm1, %edx
	sub	$0xffff, %edx		/* if first 4 double_words are same, edx == 0xffff */
	jnz	L(less4_double_words)

	movdqu	16(%edi), %xmm3
	movdqu	16(%esi), %xmm4
	pcmpeqd	%xmm3, %xmm0		/* Any null double_word? */
	pcmpeqd	%xmm4, %xmm3		/* compare first 4 double_words for equality */
	psubb	%xmm0, %xmm3		/* packed sub of comparison results*/
	pmovmskb %xmm3, %edx
	sub	$0xffff, %edx		/* if first 4 double_words are same, edx == 0xffff */
	jnz	L(less4_double_words_16)

	movdqu	32(%edi), %xmm1
	movdqu	32(%esi), %xmm2
	pcmpeqd	%xmm1, %xmm0		/* Any null double_word? */
	pcmpeqd	%xmm2, %xmm1		/* compare first 4 double_words for equality */
	psubb	%xmm0, %xmm1		/* packed sub of comparison results*/
	pmovmskb %xmm1, %edx
	sub	$0xffff, %edx		/* if first 4 double_words are same, edx == 0xffff */
	jnz	L(less4_double_words_32)

	add	$48, %esi
	add	$48, %edi
	jmp	L(continue_48_48)

	.p2align 4
L(continue_0_16):
	movdqu	(%edi), %xmm1
	movdqu	(%esi), %xmm2
	pcmpeqd	%xmm1, %xmm0		/* Any null double_word? */
	pcmpeqd	%xmm2, %xmm1		/* compare first 4 double_words for equality */
	psubb	%xmm0, %xmm1		/* packed sub of comparison results*/
	pmovmskb %xmm1, %edx
	sub	$0xffff, %edx		/* if first 4 double_words are same, edx == 0xffff */
	jnz	L(less4_double_words)

	movdqu	16(%edi), %xmm1
	movdqu	16(%esi), %xmm2
	pcmpeqd	%xmm1, %xmm0		/* Any null double_word? */
	pcmpeqd	%xmm2, %xmm1		/* compare first 4 double_words for equality */
	psubb	%xmm0, %xmm1		/* packed sub of comparison results*/
	pmovmskb %xmm1, %edx
	sub	$0xffff, %edx		/* if first 4 double_words are same, edx == 0xffff */
	jnz	L(less4_double_words_16)

	add	$32, %esi
	add	$32, %edi
	jmp	L(continue_32_48)

	.p2align 4
L(continue_0_32):
	movdqu	(%edi), %xmm1
	movdqu	(%esi), %xmm2
	pcmpeqd	%xmm1, %xmm0		/* Any null double_word? */
	pcmpeqd	%xmm2, %xmm1		/* compare first 4 double_words for equality */
	psubb	%xmm0, %xmm1		/* packed sub of comparison results*/
	pmovmskb %xmm1, %edx
	sub	$0xffff, %edx		/* if first 4 double_words are same, edx == 0xffff */
	jnz	L(less4_double_words)

	add	$16, %esi
	add	$16, %edi
	jmp	L(continue_16_48)

	.p2align 4
L(continue_16_32):
	movdqu	(%edi), %xmm1
	movdqu	(%esi), %xmm2
	pcmpeqd	%xmm1, %xmm0		/* Any null double_word? */
	pcmpeqd	%xmm2, %xmm1		/* compare first 4 double_words for equality */
	psubb	%xmm0, %xmm1		/* packed sub of comparison results*/
	pmovmskb %xmm1, %edx
	sub	$0xffff, %edx		/* if first 4 double_words are same, edx == 0xffff */
	jnz	L(less4_double_words)

	add	$16, %esi
	add	$16, %edi
	jmp	L(continue_32_48)

	.p2align 4
L(less4_double_words1):
	cmp	(%esi), %eax
	jne	L(nequal)
	test	%eax, %eax
	jz	L(equal)

	mov	4(%esi), %ecx
	cmp	%ecx, 4(%edi)
	jne	L(nequal)
	test	%ecx, %ecx
	jz	L(equal)

	mov	8(%esi), %ecx
	cmp	%ecx, 8(%edi)
	jne	L(nequal)
	test	%ecx, %ecx
	jz	L(equal)

	mov	12(%esi), %ecx
	cmp	%ecx, 12(%edi)
	jne	L(nequal)
	xor	%eax, %eax
	RETURN

	.p2align 4
L(less4_double_words):
	xor	%eax, %eax
	test	%dl, %dl
	jz	L(next_two_double_words)
	and	$15, %dl
	jz	L(second_double_word)
	mov	(%esi), %ecx
	cmp	%ecx, (%edi)
	jne	L(nequal)
	RETURN

	.p2align 4
L(second_double_word):
	mov	4(%esi), %ecx
	cmp	%ecx, 4(%edi)
	jne	L(nequal)
	RETURN

	.p2align 4
L(next_two_double_words):
	and	$15, %dh
	jz	L(fourth_double_word)
	mov	8(%esi), %ecx
	cmp	%ecx, 8(%edi)
	jne	L(nequal)
	RETURN

	.p2align 4
L(fourth_double_word):
	mov	12(%esi), %ecx
	cmp	%ecx, 12(%edi)
	jne	L(nequal)
	RETURN

	.p2align 4
L(less4_double_words_16):
	xor	%eax, %eax
	test	%dl, %dl
	jz	L(next_two_double_words_16)
	and	$15, %dl
	jz	L(second_double_word_16)
	mov	16(%esi), %ecx
	cmp	%ecx, 16(%edi)
	jne	L(nequal)
	RETURN

	.p2align 4
L(second_double_word_16):
	mov	20(%esi), %ecx
	cmp	%ecx, 20(%edi)
	jne	L(nequal)
	RETURN

	.p2align 4
L(next_two_double_words_16):
	and	$15, %dh
	jz	L(fourth_double_word_16)
	mov	24(%esi), %ecx
	cmp	%ecx, 24(%edi)
	jne	L(nequal)
	RETURN

	.p2align 4
L(fourth_double_word_16):
	mov	28(%esi), %ecx
	cmp	%ecx, 28(%edi)
	jne	L(nequal)
	RETURN

	.p2align 4
L(less4_double_words_32):
	xor	%eax, %eax
	test	%dl, %dl
	jz	L(next_two_double_words_32)
	and	$15, %dl
	jz	L(second_double_word_32)
	mov	32(%esi), %ecx
	cmp	%ecx, 32(%edi)
	jne	L(nequal)
	RETURN

	.p2align 4
L(second_double_word_32):
	mov	36(%esi), %ecx
	cmp	%ecx, 36(%edi)
	jne	L(nequal)
	RETURN

	.p2align 4
L(next_two_double_words_32):
	and	$15, %dh
	jz	L(fourth_double_word_32)
	mov	40(%esi), %ecx
	cmp	%ecx, 40(%edi)
	jne	L(nequal)
	RETURN

	.p2align 4
L(fourth_double_word_32):
	mov	44(%esi), %ecx
	cmp	%ecx, 44(%edi)
	jne	L(nequal)
	RETURN

	.p2align 4
L(less4_double_words_48):
	xor	%eax, %eax
	test	%dl, %dl
	jz	L(next_two_double_words_48)
	and	$15, %dl
	jz	L(second_double_word_48)
	mov	48(%esi), %ecx
	cmp	%ecx, 48(%edi)
	jne	L(nequal)
	RETURN

	.p2align 4
L(second_double_word_48):
	mov	52(%esi), %ecx
	cmp	%ecx, 52(%edi)
	jne	L(nequal)
	RETURN

	.p2align 4
L(next_two_double_words_48):
	and	$15, %dh
	jz	L(fourth_double_word_48)
	mov	56(%esi), %ecx
	cmp	%ecx, 56(%edi)
	jne	L(nequal)
	RETURN

	.p2align 4
L(fourth_double_word_48):
	mov	60(%esi), %ecx
	cmp	%ecx, 60(%edi)
	jne	L(nequal)
	RETURN

	.p2align 4
L(nequal):
	mov	$1, %eax
	jg	L(return)
	neg	%eax
	RETURN

	.p2align 4
L(return):
	RETURN

	.p2align 4
L(equal):
	xorl	%eax, %eax
	RETURN

	CFI_POP (%edi)
	CFI_POP (%esi)

	.p2align 4
L(neq):
	mov	$1, %eax
	jg	L(neq_bigger)
	neg	%eax

L(neq_bigger):
	ret

	.p2align 4
L(eq):
	xorl	%eax, %eax
	ret

END (wcscmp)

