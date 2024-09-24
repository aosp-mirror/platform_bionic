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
# define ENTRY(name)             \
	.type name, @function;   \
	.globl name;             \
	.p2align 4;              \
name:                            \
	cfi_startproc
#endif

#ifndef END
# define END(name)               \
	cfi_endproc;             \
	.size name, .-name
#endif

#define CFI_PUSH(REG)                  \
	cfi_adjust_cfa_offset (4);     \
	cfi_rel_offset (REG, 0)

#define CFI_POP(REG)                   \
	cfi_adjust_cfa_offset (-4);    \
	cfi_restore (REG)

#define PUSH(REG) pushl REG; CFI_PUSH (REG)
#define POP(REG) popl REG; CFI_POP (REG)

#ifndef STRCPY
# define STRCPY  strcpy
#endif

#ifdef USE_AS_STPNCPY
# define USE_AS_STRNCPY
# define USE_AS_STPCPY
#endif

#ifdef USE_AS_STRNCPY
# define PARMS  16
# define ENTRANCE PUSH(%ebx); PUSH(%esi); PUSH(%edi)
# define RETURN  POP(%edi); POP(%esi); POP(%ebx); ret; CFI_PUSH(%ebx); CFI_PUSH(%edi); CFI_PUSH(%edi);
#else
# define PARMS  12
# define ENTRANCE PUSH(%esi); PUSH(%edi)
# define RETURN  POP(%edi); POP(%esi); ret; CFI_PUSH(%esi); CFI_PUSH(%edi);
#endif

#define STR1  PARMS
#define STR2  STR1+4
#define LEN  STR2+4


#if (defined SHARED || defined __PIC__)
# define JMPTBL(I, B)	I - B

/* Load an entry in a jump table into ECX and branch to it.  TABLE is a
	jump	table with relative offsets.  INDEX is a register contains the
	index	into the jump table.   SCALE is the scale of INDEX. */

# define BRANCH_TO_JMPTBL_ENTRY(TABLE, INDEX, SCALE)            \
	/* We first load PC into ECX.  */                       \
	call	__x86.get_pc_thunk.cx;                         \
	/* Get the address of the jump table.  */               \
	addl	$(TABLE - .), %ecx;                             \
	/* Get the entry and convert the relative offset to the \
	absolute	address.  */                            \
	addl	(%ecx,INDEX,SCALE), %ecx;                       \
	/* We loaded the jump table and adjuested ECX. Go.  */  \
	jmp	*%ecx
#else
# define JMPTBL(I, B)	I

/* Branch to an entry in a jump table.  TABLE is a jump table with
	absolute	offsets.  INDEX is a register contains the index into the
	jump	table.  SCALE is the scale of INDEX. */

# define BRANCH_TO_JMPTBL_ENTRY(TABLE, INDEX, SCALE)		\
	jmp	*TABLE(,INDEX,SCALE)
#endif

.text
ENTRY (STRCPY)
	ENTRANCE
	mov	STR1(%esp), %edi
	mov	STR2(%esp), %esi
#ifdef USE_AS_STRNCPY
	movl	LEN(%esp), %ebx
	test	%ebx, %ebx
	jz	L(ExitZero)
#endif

	mov	%esi, %ecx
#ifndef USE_AS_STPCPY
	mov	%edi, %eax      /* save result */
#endif
	and	$15, %ecx
	jz	L(SourceStringAlignmentZero)

	and	$-16, %esi
	pxor	%xmm0, %xmm0
	pxor	%xmm1, %xmm1

	pcmpeqb	(%esi), %xmm1
#ifdef USE_AS_STRNCPY
	add	%ecx, %ebx
#endif
	pmovmskb %xmm1, %edx
	shr	%cl, %edx
#ifdef USE_AS_STRNCPY
#ifdef USE_AS_STPCPY
	cmp	$16, %ebx
	jbe	L(CopyFrom1To16BytesTailCase2OrCase3)
#else
	cmp	$17, %ebx
	jbe	L(CopyFrom1To16BytesTailCase2OrCase3)
#endif
#endif
	test	%edx, %edx
	jnz	L(CopyFrom1To16BytesTail)

	pcmpeqb	16(%esi), %xmm0
	pmovmskb %xmm0, %edx
#ifdef USE_AS_STRNCPY
#ifdef USE_AS_STPCPY
	cmp	$32, %ebx
	jbe	L(CopyFrom1To32BytesCase2OrCase3)
#else
	cmp	$33, %ebx
	jbe	L(CopyFrom1To32BytesCase2OrCase3)
#endif
#endif
	test	%edx, %edx
	jnz	L(CopyFrom1To32Bytes)

	movdqu	(%esi, %ecx), %xmm1   /* copy 16 bytes */
	movdqu	%xmm1, (%edi)

	sub	%ecx, %edi
	mov	%edi, %edx
	mov	$16, %ecx
	and	$15, %edx
	jz	L(Align16Both)

/* If source adress alignment != destination adress alignment */
	.p2align 4
L(Unalign16Both):
	movdqa	(%esi, %ecx), %xmm1
	movaps	16(%esi, %ecx), %xmm2
	movdqu	%xmm1, (%edi, %ecx)
	pcmpeqb	%xmm2, %xmm0
	pmovmskb %xmm0, %edx
	add	$16, %ecx
#ifdef USE_AS_STRNCPY
	sub	$48, %ebx
	jbe	L(CopyFrom1To16BytesCase2OrCase3)
	test	%edx, %edx
	jnz	L(CopyFrom1To16BytesUnalignedXmm2)
#else
	test	%edx, %edx
	jnz	L(CopyFrom1To16Bytes)
#endif

	movaps	16(%esi, %ecx), %xmm3
	movdqu	%xmm2, (%edi, %ecx)
	pcmpeqb	%xmm3, %xmm0
	pmovmskb %xmm0, %edx
	add	$16, %ecx
#ifdef USE_AS_STRNCPY
	sub	$16, %ebx
	jbe	L(CopyFrom1To16BytesCase2OrCase3)
	test	%edx, %edx
	jnz	L(CopyFrom1To16BytesUnalignedXmm3)
#else
	test	%edx, %edx
	jnz	L(CopyFrom1To16Bytes)
#endif

	movaps	16(%esi, %ecx), %xmm4
	movdqu	%xmm3, (%edi, %ecx)
	pcmpeqb	%xmm4, %xmm0
	pmovmskb %xmm0, %edx
	add	$16, %ecx
#ifdef USE_AS_STRNCPY
	sub	$16, %ebx
	jbe	L(CopyFrom1To16BytesCase2OrCase3)
	test	%edx, %edx
	jnz	L(CopyFrom1To16BytesUnalignedXmm4)
#else
	test	%edx, %edx
	jnz	L(CopyFrom1To16Bytes)
#endif

	movaps	16(%esi, %ecx), %xmm1
	movdqu	%xmm4, (%edi, %ecx)
	pcmpeqb	%xmm1, %xmm0
	pmovmskb %xmm0, %edx
	add	$16, %ecx
#ifdef USE_AS_STRNCPY
	sub	$16, %ebx
	jbe	L(CopyFrom1To16BytesCase2OrCase3)
	test	%edx, %edx
	jnz	L(CopyFrom1To16BytesUnalignedXmm1)
#else
	test	%edx, %edx
	jnz	L(CopyFrom1To16Bytes)
#endif

	movaps	16(%esi, %ecx), %xmm2
	movdqu	%xmm1, (%edi, %ecx)
	pcmpeqb	%xmm2, %xmm0
	pmovmskb %xmm0, %edx
	add	$16, %ecx
#ifdef USE_AS_STRNCPY
	sub	$16, %ebx
	jbe	L(CopyFrom1To16BytesCase2OrCase3)
	test	%edx, %edx
	jnz	L(CopyFrom1To16BytesUnalignedXmm2)
#else
	test	%edx, %edx
	jnz	L(CopyFrom1To16Bytes)
#endif

	movaps	16(%esi, %ecx), %xmm3
	movdqu	%xmm2, (%edi, %ecx)
	pcmpeqb	%xmm3, %xmm0
	pmovmskb %xmm0, %edx
	add	$16, %ecx
#ifdef USE_AS_STRNCPY
	sub	$16, %ebx
	jbe	L(CopyFrom1To16BytesCase2OrCase3)
	test	%edx, %edx
	jnz	L(CopyFrom1To16BytesUnalignedXmm3)
#else
	test	%edx, %edx
	jnz	L(CopyFrom1To16Bytes)
#endif

	movdqu	%xmm3, (%edi, %ecx)
	mov	%esi, %edx
	lea	16(%esi, %ecx), %esi
	and	$-0x40, %esi
	sub	%esi, %edx
	sub	%edx, %edi
#ifdef USE_AS_STRNCPY
	lea	64+64(%ebx, %edx), %ebx
#endif
L(Unaligned64Loop):
	movaps	(%esi), %xmm2
	movaps	%xmm2, %xmm4
	movaps	16(%esi), %xmm5
	movaps	32(%esi), %xmm3
	movaps	%xmm3, %xmm6
	movaps	48(%esi), %xmm7
	pminub	%xmm5, %xmm2
	pminub	%xmm7, %xmm3
	pminub	%xmm2, %xmm3
	pcmpeqb	%xmm0, %xmm3
	pmovmskb %xmm3, %edx
#ifdef USE_AS_STRNCPY
	sub	$64, %ebx
	jbe	L(UnalignedLeaveCase2OrCase3)
#endif
	test	%edx, %edx
	jnz	L(Unaligned64Leave)

L(Unaligned64Loop_start):
	add	$64, %edi
	add	$64, %esi
	movdqu	%xmm4, -64(%edi)
	movaps	(%esi), %xmm2
	movdqa	%xmm2, %xmm4
	movdqu	%xmm5, -48(%edi)
	movaps	16(%esi), %xmm5
	pminub	%xmm5, %xmm2
	movaps	32(%esi), %xmm3
	movdqu	%xmm6, -32(%edi)
	movaps	%xmm3, %xmm6
	movdqu	%xmm7, -16(%edi)
	movaps	48(%esi), %xmm7
	pminub	%xmm7, %xmm3
	pminub	%xmm2, %xmm3
	pcmpeqb	%xmm3, %xmm0
	pmovmskb %xmm0, %edx
#ifdef USE_AS_STRNCPY
	sub	$64, %ebx
	jbe	L(UnalignedLeaveCase2OrCase3)
#endif
	test	%edx, %edx
	jz	L(Unaligned64Loop_start)

L(Unaligned64Leave):
	pxor	%xmm0, %xmm0
	pxor	%xmm1, %xmm1

	pcmpeqb	%xmm4, %xmm0
	pcmpeqb	%xmm5, %xmm1
	pmovmskb %xmm0, %edx
	pmovmskb %xmm1, %ecx
	test	%edx, %edx
	jnz	L(CopyFrom1To16BytesUnaligned_0)
	test	%ecx, %ecx
	jnz	L(CopyFrom1To16BytesUnaligned_16)

	pcmpeqb	%xmm6, %xmm0
	pcmpeqb	%xmm7, %xmm1
	pmovmskb %xmm0, %edx
	pmovmskb %xmm1, %ecx
	test	%edx, %edx
	jnz	L(CopyFrom1To16BytesUnaligned_32)

	bsf	%ecx, %edx
	movdqu	%xmm4, (%edi)
	movdqu	%xmm5, 16(%edi)
	movdqu	%xmm6, 32(%edi)
#ifdef USE_AS_STRNCPY
#ifdef USE_AS_STPCPY
	lea	48(%edi, %edx), %eax
#endif
	movdqu	%xmm7, 48(%edi)
	add	$15, %ebx
	sub	%edx, %ebx
	lea	49(%edi, %edx), %edi
	jmp	L(StrncpyFillTailWithZero)
#else
	add	$48, %esi
	add	$48, %edi
	BRANCH_TO_JMPTBL_ENTRY (L(ExitTable), %edx, 4)
#endif

/* If source adress alignment == destination adress alignment */

L(SourceStringAlignmentZero):
	pxor	%xmm0, %xmm0
	movdqa	(%esi), %xmm1
	pcmpeqb	%xmm1, %xmm0
	pmovmskb %xmm0, %edx

#ifdef USE_AS_STRNCPY
#ifdef USE_AS_STPCPY
	cmp	$16, %ebx
	jbe	L(CopyFrom1To16BytesTail1Case2OrCase3)
#else
	cmp	$17, %ebx
	jbe	L(CopyFrom1To16BytesTail1Case2OrCase3)
#endif
#endif
	test	%edx, %edx
	jnz	L(CopyFrom1To16BytesTail1)

	pcmpeqb	16(%esi), %xmm0
	movdqu	%xmm1, (%edi)
	pmovmskb %xmm0, %edx

#ifdef USE_AS_STRNCPY
#ifdef USE_AS_STPCPY
	cmp	$32, %ebx
	jbe	L(CopyFrom1To32Bytes1Case2OrCase3)
#else
	cmp	$33, %ebx
	jbe	L(CopyFrom1To32Bytes1Case2OrCase3)
#endif
#endif
	test	%edx, %edx
	jnz	L(CopyFrom1To32Bytes1)

	mov	%edi, %edx
	mov	$16, %ecx
	and	$15, %edx
	jnz	L(Unalign16Both)

L(Align16Both):
	movdqa	(%esi, %ecx), %xmm1
	movdqa	16(%esi, %ecx), %xmm2
	movdqa	%xmm1, (%edi, %ecx)
	pcmpeqb	%xmm2, %xmm0
	pmovmskb %xmm0, %edx
	add	$16, %ecx
#ifdef USE_AS_STRNCPY
	sub	$48, %ebx
	jbe	L(CopyFrom1To16BytesCase2OrCase3)
	test	%edx, %edx
	jnz	L(CopyFrom1To16BytesXmm2)
#else
	test	%edx, %edx
	jnz	L(CopyFrom1To16Bytes)
#endif

	movdqa	16(%esi, %ecx), %xmm3
	movdqa	%xmm2, (%edi, %ecx)
	pcmpeqb	%xmm3, %xmm0
	pmovmskb %xmm0, %edx
	lea	16(%ecx), %ecx
#ifdef USE_AS_STRNCPY
	sub	$16, %ebx
	jbe	L(CopyFrom1To16BytesCase2OrCase3)
	test	%edx, %edx
	jnz	L(CopyFrom1To16BytesXmm3)
#else
	test	%edx, %edx
	jnz	L(CopyFrom1To16Bytes)
#endif

	movdqa	16(%esi, %ecx), %xmm4
	movdqa	%xmm3, (%edi, %ecx)
	pcmpeqb	%xmm4, %xmm0
	pmovmskb %xmm0, %edx
	lea	16(%ecx), %ecx
#ifdef USE_AS_STRNCPY
	sub	$16, %ebx
	jbe	L(CopyFrom1To16BytesCase2OrCase3)
	test	%edx, %edx
	jnz	L(CopyFrom1To16BytesXmm4)
#else
	test	%edx, %edx
	jnz	L(CopyFrom1To16Bytes)
#endif

	movdqa	16(%esi, %ecx), %xmm1
	movdqa	%xmm4, (%edi, %ecx)
	pcmpeqb	%xmm1, %xmm0
	pmovmskb %xmm0, %edx
	lea	16(%ecx), %ecx
#ifdef USE_AS_STRNCPY
	sub	$16, %ebx
	jbe	L(CopyFrom1To16BytesCase2OrCase3)
	test	%edx, %edx
	jnz	L(CopyFrom1To16BytesXmm1)
#else
	test	%edx, %edx
	jnz	L(CopyFrom1To16Bytes)
#endif

	movdqa	16(%esi, %ecx), %xmm2
	movdqa	%xmm1, (%edi, %ecx)
	pcmpeqb	%xmm2, %xmm0
	pmovmskb %xmm0, %edx
	lea	16(%ecx), %ecx
#ifdef USE_AS_STRNCPY
	sub	$16, %ebx
	jbe	L(CopyFrom1To16BytesCase2OrCase3)
	test	%edx, %edx
	jnz	L(CopyFrom1To16BytesXmm2)
#else
	test	%edx, %edx
	jnz	L(CopyFrom1To16Bytes)
#endif

	movdqa	16(%esi, %ecx), %xmm3
	movdqa	%xmm2, (%edi, %ecx)
	pcmpeqb	%xmm3, %xmm0
	pmovmskb %xmm0, %edx
	lea	16(%ecx), %ecx
#ifdef USE_AS_STRNCPY
	sub	$16, %ebx
	jbe	L(CopyFrom1To16BytesCase2OrCase3)
	test	%edx, %edx
	jnz	L(CopyFrom1To16BytesXmm3)
#else
	test	%edx, %edx
	jnz	L(CopyFrom1To16Bytes)
#endif

	movdqa	%xmm3, (%edi, %ecx)
	mov	%esi, %edx
	lea	16(%esi, %ecx), %esi
	and	$-0x40, %esi
	sub	%esi, %edx
	sub	%edx, %edi
#ifdef USE_AS_STRNCPY
	lea	64+64(%ebx, %edx), %ebx
#endif
L(Aligned64Loop):
	movdqa	(%esi), %xmm2
	movdqa	%xmm2, %xmm4
	movaps	16(%esi), %xmm5
	movdqa	32(%esi), %xmm3
	movdqa	%xmm3, %xmm6
	movaps	48(%esi), %xmm7
	pminub	%xmm5, %xmm2
	pminub	%xmm7, %xmm3
	pminub	%xmm2, %xmm3
	pcmpeqb	%xmm0, %xmm3
	pmovmskb %xmm3, %edx
#ifdef USE_AS_STRNCPY
	sub	$64, %ebx
	jbe	L(AlignedLeaveCase2OrCase3)
#endif
	test	%edx, %edx
	jnz	L(Aligned64Leave)

L(Aligned64Loop_start):
	add	$64, %esi
	add	$64, %edi
	movaps	%xmm4, -64(%edi)
	movdqa	(%esi), %xmm2
	movdqa	%xmm2, %xmm4
	movaps	%xmm5, -48(%edi)
	movaps	16(%esi), %xmm5
	pminub	%xmm5, %xmm2
	movaps	32(%esi), %xmm3
	movaps	%xmm6, -32(%edi)
	movdqa	%xmm3, %xmm6
	movaps	%xmm7, -16(%edi)
	movaps	48(%esi), %xmm7
	pminub	%xmm7, %xmm3
	pminub	%xmm2, %xmm3
	pcmpeqb	%xmm3, %xmm0
	pmovmskb %xmm0, %edx
#ifdef USE_AS_STRNCPY
	sub	$64, %ebx
	jbe	L(AlignedLeaveCase2OrCase3)
#endif
	test	%edx, %edx
	jz	L(Aligned64Loop_start)

L(Aligned64Leave):
	pxor	%xmm0, %xmm0
	pxor	%xmm1, %xmm1

	pcmpeqb	%xmm4, %xmm0
	pcmpeqb	%xmm5, %xmm1
	pmovmskb %xmm0, %edx
	pmovmskb %xmm1, %ecx
	test	%edx, %edx
	jnz	L(CopyFrom1To16Bytes_0)
	test	%ecx, %ecx
	jnz	L(CopyFrom1To16Bytes_16)

	pcmpeqb	%xmm6, %xmm0
	pcmpeqb	%xmm7, %xmm1
	pmovmskb %xmm0, %edx
	pmovmskb %xmm1, %ecx
	test	%edx, %edx
	jnz	L(CopyFrom1To16Bytes_32)

	bsf	%ecx, %edx
	movdqa	%xmm4, (%edi)
	movdqa	%xmm5, 16(%edi)
	movdqa	%xmm6, 32(%edi)
#ifdef USE_AS_STRNCPY
#ifdef USE_AS_STPCPY
	lea	48(%edi, %edx), %eax
#endif
	movdqa	%xmm7, 48(%edi)
	add	$15, %ebx
	sub	%edx, %ebx
	lea	49(%edi, %edx), %edi
	jmp	L(StrncpyFillTailWithZero)
#else
	add	$48, %esi
	add	$48, %edi
	BRANCH_TO_JMPTBL_ENTRY (L(ExitTable), %edx, 4)
#endif

/*----------------------------------------------------*/

/* Case1 */
#ifndef USE_AS_STRNCPY
	.p2align 4
L(CopyFrom1To16Bytes):
	add	%ecx, %edi
	add	%ecx, %esi
	bsf	%edx, %edx
	BRANCH_TO_JMPTBL_ENTRY (L(ExitTable), %edx, 4)
#endif
	.p2align 4
L(CopyFrom1To16BytesTail):
#ifdef USE_AS_STRNCPY
	sub	%ecx, %ebx
#endif
	add	%ecx, %esi
	bsf	%edx, %edx
	BRANCH_TO_JMPTBL_ENTRY (L(ExitTable), %edx, 4)

	.p2align 4
L(CopyFrom1To32Bytes1):
	add	$16, %esi
	add	$16, %edi
#ifdef USE_AS_STRNCPY
	sub	$16, %ebx
#endif
L(CopyFrom1To16BytesTail1):
	bsf	%edx, %edx
	BRANCH_TO_JMPTBL_ENTRY (L(ExitTable), %edx, 4)

	.p2align 4
L(CopyFrom1To32Bytes):
#ifdef USE_AS_STRNCPY
	sub	%ecx, %ebx
#endif
	bsf	%edx, %edx
	add	%ecx, %esi
	add	$16, %edx
	sub	%ecx, %edx
	BRANCH_TO_JMPTBL_ENTRY (L(ExitTable), %edx, 4)

	.p2align 4
L(CopyFrom1To16Bytes_0):
	bsf	%edx, %edx
#ifdef USE_AS_STRNCPY
#ifdef USE_AS_STPCPY
	lea	(%edi, %edx), %eax
#endif
	movdqa	%xmm4, (%edi)
	add	$63, %ebx
	sub	%edx, %ebx
	lea	1(%edi, %edx), %edi
	jmp	L(StrncpyFillTailWithZero)
#else
	BRANCH_TO_JMPTBL_ENTRY (L(ExitTable), %edx, 4)
#endif

	.p2align 4
L(CopyFrom1To16Bytes_16):
	bsf	%ecx, %edx
	movdqa	%xmm4, (%edi)
#ifdef USE_AS_STRNCPY
#ifdef USE_AS_STPCPY
	lea	16(%edi, %edx), %eax
#endif
	movdqa	%xmm5, 16(%edi)
	add	$47, %ebx
	sub	%edx, %ebx
	lea	17(%edi, %edx), %edi
	jmp	L(StrncpyFillTailWithZero)
#else
	add	$16, %esi
	add	$16, %edi
	BRANCH_TO_JMPTBL_ENTRY (L(ExitTable), %edx, 4)
#endif

	.p2align 4
L(CopyFrom1To16Bytes_32):
	bsf	%edx, %edx
	movdqa	%xmm4, (%edi)
	movdqa	%xmm5, 16(%edi)
#ifdef USE_AS_STRNCPY
#ifdef USE_AS_STPCPY
	lea	32(%edi, %edx), %eax
#endif
	movdqa	%xmm6, 32(%edi)
	add	$31, %ebx
	sub	%edx, %ebx
	lea	33(%edi, %edx), %edi
	jmp	L(StrncpyFillTailWithZero)
#else
	add	$32, %esi
	add	$32, %edi
	BRANCH_TO_JMPTBL_ENTRY (L(ExitTable), %edx, 4)
#endif

	.p2align 4
L(CopyFrom1To16BytesUnaligned_0):
	bsf	%edx, %edx
#ifdef USE_AS_STRNCPY
#ifdef USE_AS_STPCPY
	lea	(%edi, %edx), %eax
#endif
	movdqu	%xmm4, (%edi)
	add	$63, %ebx
	sub	%edx, %ebx
	lea	1(%edi, %edx), %edi
	jmp	L(StrncpyFillTailWithZero)
#else
	BRANCH_TO_JMPTBL_ENTRY (L(ExitTable), %edx, 4)
#endif

	.p2align 4
L(CopyFrom1To16BytesUnaligned_16):
	bsf	%ecx, %edx
	movdqu	%xmm4, (%edi)
#ifdef USE_AS_STRNCPY
#ifdef USE_AS_STPCPY
	lea	16(%edi, %edx), %eax
#endif
	movdqu	%xmm5, 16(%edi)
	add	$47, %ebx
	sub	%edx, %ebx
	lea	17(%edi, %edx), %edi
	jmp	L(StrncpyFillTailWithZero)
#else
	add	$16, %esi
	add	$16, %edi
	BRANCH_TO_JMPTBL_ENTRY (L(ExitTable), %edx, 4)
#endif

	.p2align 4
L(CopyFrom1To16BytesUnaligned_32):
	bsf	%edx, %edx
	movdqu	%xmm4, (%edi)
	movdqu	%xmm5, 16(%edi)
#ifdef USE_AS_STRNCPY
#ifdef USE_AS_STPCPY
	lea	32(%edi, %edx), %eax
#endif
	movdqu	%xmm6, 32(%edi)
	add	$31, %ebx
	sub	%edx, %ebx
	lea	33(%edi, %edx), %edi
	jmp	L(StrncpyFillTailWithZero)
#else
	add	$32, %esi
	add	$32, %edi
	BRANCH_TO_JMPTBL_ENTRY (L(ExitTable), %edx, 4)
#endif

#ifdef USE_AS_STRNCPY
	.p2align 4
L(CopyFrom1To16BytesXmm6):
	movdqa	%xmm6, (%edi, %ecx)
	jmp	L(CopyFrom1To16BytesXmmExit)

	.p2align 4
L(CopyFrom1To16BytesXmm5):
	movdqa	%xmm5, (%edi, %ecx)
	jmp	L(CopyFrom1To16BytesXmmExit)

	.p2align 4
L(CopyFrom1To16BytesXmm4):
	movdqa	%xmm4, (%edi, %ecx)
	jmp	L(CopyFrom1To16BytesXmmExit)

	.p2align 4
L(CopyFrom1To16BytesXmm3):
	movdqa	%xmm3, (%edi, %ecx)
	jmp	L(CopyFrom1To16BytesXmmExit)

	.p2align 4
L(CopyFrom1To16BytesXmm2):
	movdqa	%xmm2, (%edi, %ecx)
	jmp	L(CopyFrom1To16BytesXmmExit)

	.p2align 4
L(CopyFrom1To16BytesXmm1):
	movdqa	%xmm1, (%edi, %ecx)
	jmp	L(CopyFrom1To16BytesXmmExit)

	.p2align 4
L(CopyFrom1To16BytesUnalignedXmm6):
	movdqu	%xmm6, (%edi, %ecx)
	jmp	L(CopyFrom1To16BytesXmmExit)

	.p2align 4
L(CopyFrom1To16BytesUnalignedXmm5):
	movdqu	%xmm5, (%edi, %ecx)
	jmp	L(CopyFrom1To16BytesXmmExit)

	.p2align 4
L(CopyFrom1To16BytesUnalignedXmm4):
	movdqu	%xmm4, (%edi, %ecx)
	jmp	L(CopyFrom1To16BytesXmmExit)

	.p2align 4
L(CopyFrom1To16BytesUnalignedXmm3):
	movdqu	%xmm3, (%edi, %ecx)
	jmp	L(CopyFrom1To16BytesXmmExit)

	.p2align 4
L(CopyFrom1To16BytesUnalignedXmm1):
	movdqu	%xmm1, (%edi, %ecx)
	jmp	L(CopyFrom1To16BytesXmmExit)

	.p2align 4
L(CopyFrom1To16BytesExit):
	BRANCH_TO_JMPTBL_ENTRY (L(ExitTable), %edx, 4)

/* Case2 */

	.p2align 4
L(CopyFrom1To16BytesCase2):
	add	$16, %ebx
	add	%ecx, %edi
	add	%ecx, %esi
	bsf	%edx, %edx
	cmp	%ebx, %edx
	jb	L(CopyFrom1To16BytesExit)
	BRANCH_TO_JMPTBL_ENTRY (L(ExitStrncpyTable), %ebx, 4)

	.p2align 4
L(CopyFrom1To32BytesCase2):
	sub	%ecx, %ebx
	add	%ecx, %esi
	bsf	%edx, %edx
	add	$16, %edx
	sub	%ecx, %edx
	cmp	%ebx, %edx
	jb	L(CopyFrom1To16BytesExit)
	BRANCH_TO_JMPTBL_ENTRY (L(ExitStrncpyTable), %ebx, 4)

L(CopyFrom1To16BytesTailCase2):
	sub	%ecx, %ebx
	add	%ecx, %esi
	bsf	%edx, %edx
	cmp	%ebx, %edx
	jb	L(CopyFrom1To16BytesExit)
	BRANCH_TO_JMPTBL_ENTRY (L(ExitStrncpyTable), %ebx, 4)

L(CopyFrom1To16BytesTail1Case2):
	bsf	%edx, %edx
	cmp	%ebx, %edx
	jb	L(CopyFrom1To16BytesExit)
	BRANCH_TO_JMPTBL_ENTRY (L(ExitStrncpyTable), %ebx, 4)

/* Case2 or Case3,  Case3 */

	.p2align 4
L(CopyFrom1To16BytesCase2OrCase3):
	test	%edx, %edx
	jnz	L(CopyFrom1To16BytesCase2)
L(CopyFrom1To16BytesCase3):
	add	$16, %ebx
	add	%ecx, %edi
	add	%ecx, %esi
	BRANCH_TO_JMPTBL_ENTRY (L(ExitStrncpyTable), %ebx, 4)

	.p2align 4
L(CopyFrom1To32BytesCase2OrCase3):
	test	%edx, %edx
	jnz	L(CopyFrom1To32BytesCase2)
	sub	%ecx, %ebx
	add	%ecx, %esi
	BRANCH_TO_JMPTBL_ENTRY (L(ExitStrncpyTable), %ebx, 4)

	.p2align 4
L(CopyFrom1To16BytesTailCase2OrCase3):
	test	%edx, %edx
	jnz	L(CopyFrom1To16BytesTailCase2)
	sub	%ecx, %ebx
	add	%ecx, %esi
	BRANCH_TO_JMPTBL_ENTRY (L(ExitStrncpyTable), %ebx, 4)

	.p2align 4
L(CopyFrom1To32Bytes1Case2OrCase3):
	add	$16, %edi
	add	$16, %esi
	sub	$16, %ebx
L(CopyFrom1To16BytesTail1Case2OrCase3):
	test	%edx, %edx
	jnz	L(CopyFrom1To16BytesTail1Case2)
	BRANCH_TO_JMPTBL_ENTRY (L(ExitStrncpyTable), %ebx, 4)

#endif

/*-----------------------------------------------------------------*/
	.p2align 4
L(Exit0):
#ifdef USE_AS_STPCPY
	mov	%edi, %eax
#endif
	RETURN

	.p2align 4
L(Exit1):
	movb	%dh, (%edi)
#ifdef USE_AS_STPCPY
	lea	(%edi), %eax
#endif
#ifdef USE_AS_STRNCPY
	sub	$1, %ebx
	lea	1(%edi), %edi
	jnz	L(StrncpyFillTailWithZero)
#endif
	RETURN

	.p2align 4
L(Exit2):
	movw	(%esi), %dx
	movw	%dx, (%edi)
#ifdef USE_AS_STPCPY
	lea	1(%edi), %eax
#endif
#ifdef USE_AS_STRNCPY
	sub	$2, %ebx
	lea	2(%edi), %edi
	jnz	L(StrncpyFillTailWithZero)
#endif
	RETURN

	.p2align 4
L(Exit3):
	movw	(%esi), %cx
	movw	%cx, (%edi)
	movb	%dh, 2(%edi)
#ifdef USE_AS_STPCPY
	lea	2(%edi), %eax
#endif
#ifdef USE_AS_STRNCPY
	sub	$3, %ebx
	lea	3(%edi), %edi
	jnz	L(StrncpyFillTailWithZero)
#endif
	RETURN

	.p2align 4
L(Exit4):
	movl	(%esi), %edx
	movl	%edx, (%edi)
#ifdef USE_AS_STPCPY
	lea	3(%edi), %eax
#endif
#ifdef USE_AS_STRNCPY
	sub	$4, %ebx
	lea	4(%edi), %edi
	jnz	L(StrncpyFillTailWithZero)
#endif
	RETURN

	.p2align 4
L(Exit5):
	movl	(%esi), %ecx
	movb	%dh, 4(%edi)
	movl	%ecx, (%edi)
#ifdef USE_AS_STPCPY
	lea	4(%edi), %eax
#endif
#ifdef USE_AS_STRNCPY
	sub	$5, %ebx
	lea	5(%edi), %edi
	jnz	L(StrncpyFillTailWithZero)
#endif
	RETURN

	.p2align 4
L(Exit6):
	movl	(%esi), %ecx
	movw	4(%esi), %dx
	movl	%ecx, (%edi)
	movw	%dx, 4(%edi)
#ifdef USE_AS_STPCPY
	lea	5(%edi), %eax
#endif
#ifdef USE_AS_STRNCPY
	sub	$6, %ebx
	lea	6(%edi), %edi
	jnz	L(StrncpyFillTailWithZero)
#endif
	RETURN

	.p2align 4
L(Exit7):
	movl	(%esi), %ecx
	movl	3(%esi), %edx
	movl	%ecx, (%edi)
	movl	%edx, 3(%edi)
#ifdef USE_AS_STPCPY
	lea	6(%edi), %eax
#endif
#ifdef USE_AS_STRNCPY
	sub	$7, %ebx
	lea	7(%edi), %edi
	jnz	L(StrncpyFillTailWithZero)
#endif
	RETURN

	.p2align 4
L(Exit8):
	movlpd	(%esi), %xmm0
	movlpd	%xmm0, (%edi)
#ifdef USE_AS_STPCPY
	lea	7(%edi), %eax
#endif
#ifdef USE_AS_STRNCPY
	sub	$8, %ebx
	lea	8(%edi), %edi
	jnz	L(StrncpyFillTailWithZero)
#endif
	RETURN

	.p2align 4
L(Exit9):
	movlpd	(%esi), %xmm0
	movb	%dh, 8(%edi)
	movlpd	%xmm0, (%edi)
#ifdef USE_AS_STPCPY
	lea	8(%edi), %eax
#endif
#ifdef USE_AS_STRNCPY
	sub	$9, %ebx
	lea	9(%edi), %edi
	jnz	L(StrncpyFillTailWithZero)
#endif
	RETURN

	.p2align 4
L(Exit10):
	movlpd	(%esi), %xmm0
	movw	8(%esi), %dx
	movlpd	%xmm0, (%edi)
	movw	%dx, 8(%edi)
#ifdef USE_AS_STPCPY
	lea	9(%edi), %eax
#endif
#ifdef USE_AS_STRNCPY
	sub	$10, %ebx
	lea	10(%edi), %edi
	jnz	L(StrncpyFillTailWithZero)
#endif
	RETURN

	.p2align 4
L(Exit11):
	movlpd	(%esi), %xmm0
	movl	7(%esi), %edx
	movlpd	%xmm0, (%edi)
	movl	%edx, 7(%edi)
#ifdef USE_AS_STPCPY
	lea	10(%edi), %eax
#endif
#ifdef USE_AS_STRNCPY
	sub	$11, %ebx
	lea	11(%edi), %edi
	jnz	L(StrncpyFillTailWithZero)
#endif
	RETURN

	.p2align 4
L(Exit12):
	movlpd	(%esi), %xmm0
	movl	8(%esi), %edx
	movlpd	%xmm0, (%edi)
	movl	%edx, 8(%edi)
#ifdef USE_AS_STPCPY
	lea	11(%edi), %eax
#endif
#ifdef USE_AS_STRNCPY
	sub	$12, %ebx
	lea	12(%edi), %edi
	jnz	L(StrncpyFillTailWithZero)
#endif
	RETURN

	.p2align 4
L(Exit13):
	movlpd	(%esi), %xmm0
	movlpd	5(%esi), %xmm1
	movlpd	%xmm0, (%edi)
	movlpd	%xmm1, 5(%edi)
#ifdef USE_AS_STPCPY
	lea	12(%edi), %eax
#endif
#ifdef USE_AS_STRNCPY
	sub	$13, %ebx
	lea	13(%edi), %edi
	jnz	L(StrncpyFillTailWithZero)
#endif
	RETURN

	.p2align 4
L(Exit14):
	movlpd	(%esi), %xmm0
	movlpd	6(%esi), %xmm1
	movlpd	%xmm0, (%edi)
	movlpd	%xmm1, 6(%edi)
#ifdef USE_AS_STPCPY
	lea	13(%edi), %eax
#endif
#ifdef USE_AS_STRNCPY
	sub	$14, %ebx
	lea	14(%edi), %edi
	jnz	L(StrncpyFillTailWithZero)
#endif
	RETURN

	.p2align 4
L(Exit15):
	movlpd	(%esi), %xmm0
	movlpd	7(%esi), %xmm1
	movlpd	%xmm0, (%edi)
	movlpd	%xmm1, 7(%edi)
#ifdef USE_AS_STPCPY
	lea	14(%edi), %eax
#endif
#ifdef USE_AS_STRNCPY
	sub	$15, %ebx
	lea	15(%edi), %edi
	jnz	L(StrncpyFillTailWithZero)
#endif
	RETURN

	.p2align 4
L(Exit16):
	movdqu	(%esi), %xmm0
	movdqu	%xmm0, (%edi)
#ifdef USE_AS_STPCPY
	lea	15(%edi), %eax
#endif
#ifdef USE_AS_STRNCPY
	sub	$16, %ebx
	lea	16(%edi), %edi
	jnz	L(StrncpyFillTailWithZero)
#endif
	RETURN

	.p2align 4
L(Exit17):
	movdqu	(%esi), %xmm0
	xor	%cl, %cl
	movdqu	%xmm0, (%edi)
	movb	%cl, 16(%edi)
#ifdef USE_AS_STPCPY
	lea	16(%edi), %eax
#endif
#ifdef USE_AS_STRNCPY
	sub	$17, %ebx
	lea	17(%edi), %edi
	jnz	L(StrncpyFillTailWithZero)
#endif
	RETURN

	.p2align 4
L(Exit18):
	movdqu	(%esi), %xmm0
	movw	16(%esi), %cx
	movdqu	%xmm0, (%edi)
	movw	%cx, 16(%edi)
#ifdef USE_AS_STPCPY
	lea	17(%edi), %eax
#endif
#ifdef USE_AS_STRNCPY
	sub	$18, %ebx
	lea	18(%edi), %edi
	jnz	L(StrncpyFillTailWithZero)
#endif
	RETURN

	.p2align 4
L(Exit19):
	movdqu	(%esi), %xmm0
	movl	15(%esi), %ecx
	movdqu	%xmm0, (%edi)
	movl	%ecx, 15(%edi)
#ifdef USE_AS_STPCPY
	lea	18(%edi), %eax
#endif
#ifdef USE_AS_STRNCPY
	sub	$19, %ebx
	lea	19(%edi), %edi
	jnz	L(StrncpyFillTailWithZero)
#endif
	RETURN

	.p2align 4
L(Exit20):
	movdqu	(%esi), %xmm0
	movl	16(%esi), %ecx
	movdqu	%xmm0, (%edi)
	movl	%ecx, 16(%edi)
#ifdef USE_AS_STPCPY
	lea	19(%edi), %eax
#endif
#ifdef USE_AS_STRNCPY
	sub	$20, %ebx
	lea	20(%edi), %edi
	jnz	L(StrncpyFillTailWithZero)
#endif
	RETURN

	.p2align 4
L(Exit21):
	movdqu	(%esi), %xmm0
	movl	16(%esi), %ecx
	xor	%dl, %dl
	movdqu	%xmm0, (%edi)
	movl	%ecx, 16(%edi)
	movb	%dl, 20(%edi)
#ifdef USE_AS_STPCPY
	lea	20(%edi), %eax
#endif
#ifdef USE_AS_STRNCPY
	sub	$21, %ebx
	lea	21(%edi), %edi
	jnz	L(StrncpyFillTailWithZero)
#endif
	RETURN

	.p2align 4
L(Exit22):
	movdqu	(%esi), %xmm0
	movlpd	14(%esi), %xmm3
	movdqu	%xmm0, (%edi)
	movlpd	%xmm3, 14(%edi)
#ifdef USE_AS_STPCPY
	lea	21(%edi), %eax
#endif
#ifdef USE_AS_STRNCPY
	sub	$22, %ebx
	lea	22(%edi), %edi
	jnz	L(StrncpyFillTailWithZero)
#endif
	RETURN

	.p2align 4
L(Exit23):
	movdqu	(%esi), %xmm0
	movlpd	15(%esi), %xmm3
	movdqu	%xmm0, (%edi)
	movlpd	%xmm3, 15(%edi)
#ifdef USE_AS_STPCPY
	lea	22(%edi), %eax
#endif
#ifdef USE_AS_STRNCPY
	sub	$23, %ebx
	lea	23(%edi), %edi
	jnz	L(StrncpyFillTailWithZero)
#endif
	RETURN

	.p2align 4
L(Exit24):
	movdqu	(%esi), %xmm0
	movlpd	16(%esi), %xmm2
	movdqu	%xmm0, (%edi)
	movlpd	%xmm2, 16(%edi)
#ifdef USE_AS_STPCPY
	lea	23(%edi), %eax
#endif
#ifdef USE_AS_STRNCPY
	sub	$24, %ebx
	lea	24(%edi), %edi
	jnz	L(StrncpyFillTailWithZero)
#endif
	RETURN

	.p2align 4
L(Exit25):
	movdqu	(%esi), %xmm0
	movlpd	16(%esi), %xmm2
	xor	%cl, %cl
	movdqu	%xmm0, (%edi)
	movlpd	%xmm2, 16(%edi)
	movb	%cl, 24(%edi)
#ifdef USE_AS_STPCPY
	lea	24(%edi), %eax
#endif
#ifdef USE_AS_STRNCPY
	sub	$25, %ebx
	lea	25(%edi), %edi
	jnz	L(StrncpyFillTailWithZero)
#endif
	RETURN

	.p2align 4
L(Exit26):
	movdqu	(%esi), %xmm0
	movlpd	16(%esi), %xmm2
	movw	24(%esi), %cx
	movdqu	%xmm0, (%edi)
	movlpd	%xmm2, 16(%edi)
	movw	%cx, 24(%edi)
#ifdef USE_AS_STPCPY
	lea	25(%edi), %eax
#endif
#ifdef USE_AS_STRNCPY
	sub	$26, %ebx
	lea	26(%edi), %edi
	jnz	L(StrncpyFillTailWithZero)
#endif
	RETURN

	.p2align 4
L(Exit27):
	movdqu	(%esi), %xmm0
	movlpd	16(%esi), %xmm2
	movl	23(%esi), %ecx
	movdqu	%xmm0, (%edi)
	movlpd	%xmm2, 16(%edi)
	movl	%ecx, 23(%edi)
#ifdef USE_AS_STPCPY
	lea	26(%edi), %eax
#endif
#ifdef USE_AS_STRNCPY
	sub	$27, %ebx
	lea	27(%edi), %edi
	jnz	L(StrncpyFillTailWithZero)
#endif
	RETURN

	.p2align 4
L(Exit28):
	movdqu	(%esi), %xmm0
	movlpd	16(%esi), %xmm2
	movl	24(%esi), %ecx
	movdqu	%xmm0, (%edi)
	movlpd	%xmm2, 16(%edi)
	movl	%ecx, 24(%edi)
#ifdef USE_AS_STPCPY
	lea	27(%edi), %eax
#endif
#ifdef USE_AS_STRNCPY
	sub	$28, %ebx
	lea	28(%edi), %edi
	jnz	L(StrncpyFillTailWithZero)
#endif
	RETURN

	.p2align 4
L(Exit29):
	movdqu	(%esi), %xmm0
	movdqu	13(%esi), %xmm2
	movdqu	%xmm0, (%edi)
	movdqu	%xmm2, 13(%edi)
#ifdef USE_AS_STPCPY
	lea	28(%edi), %eax
#endif
#ifdef USE_AS_STRNCPY
	sub	$29, %ebx
	lea	29(%edi), %edi
	jnz	L(StrncpyFillTailWithZero)
#endif
	RETURN

	.p2align 4
L(Exit30):
	movdqu	(%esi), %xmm0
	movdqu	14(%esi), %xmm2
	movdqu	%xmm0, (%edi)
	movdqu	%xmm2, 14(%edi)
#ifdef USE_AS_STPCPY
	lea	29(%edi), %eax
#endif
#ifdef USE_AS_STRNCPY
	sub	$30, %ebx
	lea	30(%edi), %edi
	jnz	L(StrncpyFillTailWithZero)
#endif
	RETURN


	.p2align 4
L(Exit31):
	movdqu	(%esi), %xmm0
	movdqu	15(%esi), %xmm2
	movdqu	%xmm0, (%edi)
	movdqu	%xmm2, 15(%edi)
#ifdef USE_AS_STPCPY
	lea	30(%edi), %eax
#endif
#ifdef USE_AS_STRNCPY
	sub	$31, %ebx
	lea	31(%edi), %edi
	jnz	L(StrncpyFillTailWithZero)
#endif
	RETURN

	.p2align 4
L(Exit32):
	movdqu	(%esi), %xmm0
	movdqu	16(%esi), %xmm2
	movdqu	%xmm0, (%edi)
	movdqu	%xmm2, 16(%edi)
#ifdef USE_AS_STPCPY
	lea	31(%edi), %eax
#endif
#ifdef USE_AS_STRNCPY
	sub	$32, %ebx
	lea	32(%edi), %edi
	jnz	L(StrncpyFillTailWithZero)
#endif
	RETURN

#ifdef USE_AS_STRNCPY

	.p2align 4
L(StrncpyExit1):
	movb	(%esi), %dl
	movb	%dl, (%edi)
#ifdef USE_AS_STPCPY
	lea	1(%edi), %eax
#endif
	RETURN

	.p2align 4
L(StrncpyExit2):
	movw	(%esi), %dx
	movw	%dx, (%edi)
#ifdef USE_AS_STPCPY
	lea	2(%edi), %eax
#endif
	RETURN
	.p2align 4
L(StrncpyExit3):
	movw	(%esi), %cx
	movb	2(%esi), %dl
	movw	%cx, (%edi)
	movb	%dl, 2(%edi)
#ifdef USE_AS_STPCPY
	lea	3(%edi), %eax
#endif
	RETURN

	.p2align 4
L(StrncpyExit4):
	movl	(%esi), %edx
	movl	%edx, (%edi)
#ifdef USE_AS_STPCPY
	lea	4(%edi), %eax
#endif
	RETURN

	.p2align 4
L(StrncpyExit5):
	movl	(%esi), %ecx
	movb	4(%esi), %dl
	movl	%ecx, (%edi)
	movb	%dl, 4(%edi)
#ifdef USE_AS_STPCPY
	lea	5(%edi), %eax
#endif
	RETURN

	.p2align 4
L(StrncpyExit6):
	movl	(%esi), %ecx
	movw	4(%esi), %dx
	movl	%ecx, (%edi)
	movw	%dx, 4(%edi)
#ifdef USE_AS_STPCPY
	lea	6(%edi), %eax
#endif
	RETURN

	.p2align 4
L(StrncpyExit7):
	movl	(%esi), %ecx
	movl	3(%esi), %edx
	movl	%ecx, (%edi)
	movl	%edx, 3(%edi)
#ifdef USE_AS_STPCPY
	lea	7(%edi), %eax
#endif
	RETURN

	.p2align 4
L(StrncpyExit8):
	movlpd	(%esi), %xmm0
	movlpd	%xmm0, (%edi)
#ifdef USE_AS_STPCPY
	lea	8(%edi), %eax
#endif
	RETURN

	.p2align 4
L(StrncpyExit9):
	movlpd	(%esi), %xmm0
	movb	8(%esi), %dl
	movlpd	%xmm0, (%edi)
	movb	%dl, 8(%edi)
#ifdef USE_AS_STPCPY
	lea	9(%edi), %eax
#endif
	RETURN

	.p2align 4
L(StrncpyExit10):
	movlpd	(%esi), %xmm0
	movw	8(%esi), %dx
	movlpd	%xmm0, (%edi)
	movw	%dx, 8(%edi)
#ifdef USE_AS_STPCPY
	lea	10(%edi), %eax
#endif
	RETURN

	.p2align 4
L(StrncpyExit11):
	movlpd	(%esi), %xmm0
	movl	7(%esi), %edx
	movlpd	%xmm0, (%edi)
	movl	%edx, 7(%edi)
#ifdef USE_AS_STPCPY
	lea	11(%edi), %eax
#endif
	RETURN

	.p2align 4
L(StrncpyExit12):
	movlpd	(%esi), %xmm0
	movl	8(%esi), %edx
	movlpd	%xmm0, (%edi)
	movl	%edx, 8(%edi)
#ifdef USE_AS_STPCPY
	lea	12(%edi), %eax
#endif
	RETURN

	.p2align 4
L(StrncpyExit13):
	movlpd	(%esi), %xmm0
	movlpd	5(%esi), %xmm1
	movlpd	%xmm0, (%edi)
	movlpd	%xmm1, 5(%edi)
#ifdef USE_AS_STPCPY
	lea	13(%edi), %eax
#endif
	RETURN

	.p2align 4
L(StrncpyExit14):
	movlpd	(%esi), %xmm0
	movlpd	6(%esi), %xmm1
	movlpd	%xmm0, (%edi)
	movlpd	%xmm1, 6(%edi)
#ifdef USE_AS_STPCPY
	lea	14(%edi), %eax
#endif
	RETURN

	.p2align 4
L(StrncpyExit15):
	movlpd	(%esi), %xmm0
	movlpd	7(%esi), %xmm1
	movlpd	%xmm0, (%edi)
	movlpd	%xmm1, 7(%edi)
#ifdef USE_AS_STPCPY
	lea	15(%edi), %eax
#endif
	RETURN

	.p2align 4
L(StrncpyExit16):
	movdqu	(%esi), %xmm0
	movdqu	%xmm0, (%edi)
#ifdef USE_AS_STPCPY
	lea	16(%edi), %eax
#endif
	RETURN

	.p2align 4
L(StrncpyExit17):
	movdqu	(%esi), %xmm0
	movb	16(%esi), %cl
	movdqu	%xmm0, (%edi)
	movb	%cl, 16(%edi)
#ifdef USE_AS_STPCPY
	lea	17(%edi), %eax
#endif
	RETURN

	.p2align 4
L(StrncpyExit18):
	movdqu	(%esi), %xmm0
	movw	16(%esi), %cx
	movdqu	%xmm0, (%edi)
	movw	%cx, 16(%edi)
#ifdef USE_AS_STPCPY
	lea	18(%edi), %eax
#endif
	RETURN

	.p2align 4
L(StrncpyExit19):
	movdqu	(%esi), %xmm0
	movl	15(%esi), %ecx
	movdqu	%xmm0, (%edi)
	movl	%ecx, 15(%edi)
#ifdef USE_AS_STPCPY
	lea	19(%edi), %eax
#endif
	RETURN

	.p2align 4
L(StrncpyExit20):
	movdqu	(%esi), %xmm0
	movl	16(%esi), %ecx
	movdqu	%xmm0, (%edi)
	movl	%ecx, 16(%edi)
#ifdef USE_AS_STPCPY
	lea	20(%edi), %eax
#endif
	RETURN

	.p2align 4
L(StrncpyExit21):
	movdqu	(%esi), %xmm0
	movl	16(%esi), %ecx
	movb	20(%esi), %dl
	movdqu	%xmm0, (%edi)
	movl	%ecx, 16(%edi)
	movb	%dl, 20(%edi)
#ifdef USE_AS_STPCPY
	lea	21(%edi), %eax
#endif
	RETURN

	.p2align 4
L(StrncpyExit22):
	movdqu	(%esi), %xmm0
	movlpd	14(%esi), %xmm3
	movdqu	%xmm0, (%edi)
	movlpd	%xmm3, 14(%edi)
#ifdef USE_AS_STPCPY
	lea	22(%edi), %eax
#endif
	RETURN

	.p2align 4
L(StrncpyExit23):
	movdqu	(%esi), %xmm0
	movlpd	15(%esi), %xmm3
	movdqu	%xmm0, (%edi)
	movlpd	%xmm3, 15(%edi)
#ifdef USE_AS_STPCPY
	lea	23(%edi), %eax
#endif
	RETURN

	.p2align 4
L(StrncpyExit24):
	movdqu	(%esi), %xmm0
	movlpd	16(%esi), %xmm2
	movdqu	%xmm0, (%edi)
	movlpd	%xmm2, 16(%edi)
#ifdef USE_AS_STPCPY
	lea	24(%edi), %eax
#endif
	RETURN

	.p2align 4
L(StrncpyExit25):
	movdqu	(%esi), %xmm0
	movlpd	16(%esi), %xmm2
	movb	24(%esi), %cl
	movdqu	%xmm0, (%edi)
	movlpd	%xmm2, 16(%edi)
	movb	%cl, 24(%edi)
#ifdef USE_AS_STPCPY
	lea	25(%edi), %eax
#endif
	RETURN

	.p2align 4
L(StrncpyExit26):
	movdqu	(%esi), %xmm0
	movlpd	16(%esi), %xmm2
	movw	24(%esi), %cx
	movdqu	%xmm0, (%edi)
	movlpd	%xmm2, 16(%edi)
	movw	%cx, 24(%edi)
#ifdef USE_AS_STPCPY
	lea	26(%edi), %eax
#endif
	RETURN

	.p2align 4
L(StrncpyExit27):
	movdqu	(%esi), %xmm0
	movlpd	16(%esi), %xmm2
	movl	23(%esi), %ecx
	movdqu	%xmm0, (%edi)
	movlpd	%xmm2, 16(%edi)
	movl	%ecx, 23(%edi)
#ifdef USE_AS_STPCPY
	lea	27(%edi), %eax
#endif
	RETURN

	.p2align 4
L(StrncpyExit28):
	movdqu	(%esi), %xmm0
	movlpd	16(%esi), %xmm2
	movl	24(%esi), %ecx
	movdqu	%xmm0, (%edi)
	movlpd	%xmm2, 16(%edi)
	movl	%ecx, 24(%edi)
#ifdef USE_AS_STPCPY
	lea	28(%edi), %eax
#endif
	RETURN

	.p2align 4
L(StrncpyExit29):
	movdqu	(%esi), %xmm0
	movdqu	13(%esi), %xmm2
	movdqu	%xmm0, (%edi)
	movdqu	%xmm2, 13(%edi)
#ifdef USE_AS_STPCPY
	lea	29(%edi), %eax
#endif
	RETURN

	.p2align 4
L(StrncpyExit30):
	movdqu	(%esi), %xmm0
	movdqu	14(%esi), %xmm2
	movdqu	%xmm0, (%edi)
	movdqu	%xmm2, 14(%edi)
#ifdef USE_AS_STPCPY
	lea	30(%edi), %eax
#endif
	RETURN

	.p2align 4
L(StrncpyExit31):
	movdqu	(%esi), %xmm0
	movdqu	15(%esi), %xmm2
	movdqu	%xmm0, (%edi)
	movdqu	%xmm2, 15(%edi)
#ifdef USE_AS_STPCPY
	lea	31(%edi), %eax
#endif
	RETURN

	.p2align 4
L(StrncpyExit32):
	movdqu	(%esi), %xmm0
	movdqu	16(%esi), %xmm2
	movdqu	%xmm0, (%edi)
	movdqu	%xmm2, 16(%edi)
#ifdef USE_AS_STPCPY
	lea	32(%edi), %eax
#endif
	RETURN

	.p2align 4
L(StrncpyExit33):
	movdqu	(%esi), %xmm0
	movdqu	16(%esi), %xmm2
	movb	32(%esi), %cl
	movdqu	%xmm0, (%edi)
	movdqu	%xmm2, 16(%edi)
	movb	%cl, 32(%edi)
	RETURN

	.p2align 4
L(Fill0):
	RETURN

	.p2align 4
L(Fill1):
	movb	%dl, (%edi)
	RETURN

	.p2align 4
L(Fill2):
	movw	%dx, (%edi)
	RETURN

	.p2align 4
L(Fill3):
	movl	%edx, -1(%edi)
	RETURN

	.p2align 4
L(Fill4):
	movl	%edx, (%edi)
	RETURN

	.p2align 4
L(Fill5):
	movl	%edx, (%edi)
	movb	%dl, 4(%edi)
	RETURN

	.p2align 4
L(Fill6):
	movl	%edx, (%edi)
	movw	%dx, 4(%edi)
	RETURN

	.p2align 4
L(Fill7):
	movlpd	%xmm0, -1(%edi)
	RETURN

	.p2align 4
L(Fill8):
	movlpd	%xmm0, (%edi)
	RETURN

	.p2align 4
L(Fill9):
	movlpd	%xmm0, (%edi)
	movb	%dl, 8(%edi)
	RETURN

	.p2align 4
L(Fill10):
	movlpd	%xmm0, (%edi)
	movw	%dx, 8(%edi)
	RETURN

	.p2align 4
L(Fill11):
	movlpd	%xmm0, (%edi)
	movl	%edx, 7(%edi)
	RETURN

	.p2align 4
L(Fill12):
	movlpd	%xmm0, (%edi)
	movl	%edx, 8(%edi)
	RETURN

	.p2align 4
L(Fill13):
	movlpd	%xmm0, (%edi)
	movlpd	%xmm0, 5(%edi)
	RETURN

	.p2align 4
L(Fill14):
	movlpd	%xmm0, (%edi)
	movlpd	%xmm0, 6(%edi)
	RETURN

	.p2align 4
L(Fill15):
	movdqu	%xmm0, -1(%edi)
	RETURN

	.p2align 4
L(Fill16):
	movdqu	%xmm0, (%edi)
	RETURN

	.p2align 4
L(CopyFrom1To16BytesUnalignedXmm2):
	movdqu	%xmm2, (%edi, %ecx)

	.p2align 4
L(CopyFrom1To16BytesXmmExit):
	bsf	%edx, %edx
	add	$15, %ebx
	add	%ecx, %edi
#ifdef USE_AS_STPCPY
	lea	(%edi, %edx), %eax
#endif
	sub	%edx, %ebx
	lea	1(%edi, %edx), %edi

	.p2align 4
L(StrncpyFillTailWithZero):
	pxor	%xmm0, %xmm0
	xor	%edx, %edx
	sub	$16, %ebx
	jbe	L(StrncpyFillExit)

	movdqu	%xmm0, (%edi)
	add	$16, %edi

	mov	%edi, %esi
	and	$0xf, %esi
	sub	%esi, %edi
	add	%esi, %ebx
	sub	$64, %ebx
	jb	L(StrncpyFillLess64)

L(StrncpyFillLoopMovdqa):
	movdqa	%xmm0, (%edi)
	movdqa	%xmm0, 16(%edi)
	movdqa	%xmm0, 32(%edi)
	movdqa	%xmm0, 48(%edi)
	add	$64, %edi
	sub	$64, %ebx
	jae	L(StrncpyFillLoopMovdqa)

L(StrncpyFillLess64):
	add	$32, %ebx
	jl	L(StrncpyFillLess32)
	movdqa	%xmm0, (%edi)
	movdqa	%xmm0, 16(%edi)
	add	$32, %edi
	sub	$16, %ebx
	jl	L(StrncpyFillExit)
	movdqa	%xmm0, (%edi)
	add	$16, %edi
	BRANCH_TO_JMPTBL_ENTRY (L(FillTable), %ebx, 4)

L(StrncpyFillLess32):
	add	$16, %ebx
	jl	L(StrncpyFillExit)
	movdqa	%xmm0, (%edi)
	add	$16, %edi
	BRANCH_TO_JMPTBL_ENTRY (L(FillTable), %ebx, 4)

L(StrncpyFillExit):
	add	$16, %ebx
	BRANCH_TO_JMPTBL_ENTRY (L(FillTable), %ebx, 4)

	.p2align 4
L(AlignedLeaveCase2OrCase3):
	test	%edx, %edx
	jnz	L(Aligned64LeaveCase2)
L(Aligned64LeaveCase3):
	lea	64(%ebx), %ecx
	and	$-16, %ecx
	add	$48, %ebx
	jl	L(CopyFrom1To16BytesCase3)
	movdqa	%xmm4, (%edi)
	sub	$16, %ebx
	jb	L(CopyFrom1To16BytesCase3)
	movdqa	%xmm5, 16(%edi)
	sub	$16, %ebx
	jb	L(CopyFrom1To16BytesCase3)
	movdqa	%xmm6, 32(%edi)
	sub	$16, %ebx
	jb	L(CopyFrom1To16BytesCase3)
	movdqa	%xmm7, 48(%edi)
#ifdef USE_AS_STPCPY
	lea	64(%edi), %eax
#endif
	RETURN

	.p2align 4
L(Aligned64LeaveCase2):
	pxor	%xmm0, %xmm0
	xor	%ecx, %ecx
	pcmpeqb	%xmm4, %xmm0
	pmovmskb %xmm0, %edx
	add	$48, %ebx
	jle	L(CopyFrom1To16BytesCase2OrCase3)
	test	%edx, %edx
	jnz	L(CopyFrom1To16BytesXmm4)

	pcmpeqb	%xmm5, %xmm0
	pmovmskb %xmm0, %edx
	movdqa	%xmm4, (%edi)
	add	$16, %ecx
	sub	$16, %ebx
	jbe	L(CopyFrom1To16BytesCase2OrCase3)
	test	%edx, %edx
	jnz	L(CopyFrom1To16BytesXmm5)

	pcmpeqb	%xmm6, %xmm0
	pmovmskb %xmm0, %edx
	movdqa	%xmm5, 16(%edi)
	add	$16, %ecx
	sub	$16, %ebx
	jbe	L(CopyFrom1To16BytesCase2OrCase3)
	test	%edx, %edx
	jnz	L(CopyFrom1To16BytesXmm6)

	pcmpeqb	%xmm7, %xmm0
	pmovmskb %xmm0, %edx
	movdqa	%xmm6, 32(%edi)
	lea	16(%edi, %ecx), %edi
	lea	16(%esi, %ecx), %esi
	bsf	%edx, %edx
	cmp	%ebx, %edx
	jb	L(CopyFrom1To16BytesExit)
	BRANCH_TO_JMPTBL_ENTRY (L(ExitStrncpyTable), %ebx, 4)

	.p2align 4
L(UnalignedLeaveCase2OrCase3):
	test	%edx, %edx
	jnz	L(Unaligned64LeaveCase2)
L(Unaligned64LeaveCase3):
	lea	64(%ebx), %ecx
	and	$-16, %ecx
	add	$48, %ebx
	jl	L(CopyFrom1To16BytesCase3)
	movdqu	%xmm4, (%edi)
	sub	$16, %ebx
	jb	L(CopyFrom1To16BytesCase3)
	movdqu	%xmm5, 16(%edi)
	sub	$16, %ebx
	jb	L(CopyFrom1To16BytesCase3)
	movdqu	%xmm6, 32(%edi)
	sub	$16, %ebx
	jb	L(CopyFrom1To16BytesCase3)
	movdqu	%xmm7, 48(%edi)
#ifdef USE_AS_STPCPY
	lea	64(%edi), %eax
#endif
	RETURN

	.p2align 4
L(Unaligned64LeaveCase2):
	pxor	%xmm0, %xmm0
	xor	%ecx, %ecx
	pcmpeqb	%xmm4, %xmm0
	pmovmskb %xmm0, %edx
	add	$48, %ebx
	jle	L(CopyFrom1To16BytesCase2OrCase3)
	test	%edx, %edx
	jnz	L(CopyFrom1To16BytesUnalignedXmm4)

	pcmpeqb	%xmm5, %xmm0
	pmovmskb %xmm0, %edx
	movdqu	%xmm4, (%edi)
	add	$16, %ecx
	sub	$16, %ebx
	jbe	L(CopyFrom1To16BytesCase2OrCase3)
	test	%edx, %edx
	jnz	L(CopyFrom1To16BytesUnalignedXmm5)

	pcmpeqb	%xmm6, %xmm0
	pmovmskb %xmm0, %edx
	movdqu	%xmm5, 16(%edi)
	add	$16, %ecx
	sub	$16, %ebx
	jbe	L(CopyFrom1To16BytesCase2OrCase3)
	test	%edx, %edx
	jnz	L(CopyFrom1To16BytesUnalignedXmm6)

	pcmpeqb	%xmm7, %xmm0
	pmovmskb %xmm0, %edx
	movdqu	%xmm6, 32(%edi)
	lea	16(%edi, %ecx), %edi
	lea	16(%esi, %ecx), %esi
	bsf	%edx, %edx
	cmp	%ebx, %edx
	jb	L(CopyFrom1To16BytesExit)
	BRANCH_TO_JMPTBL_ENTRY (L(ExitStrncpyTable), %ebx, 4)

	.p2align 4
L(ExitZero):
	movl	%edi, %eax
	RETURN
#endif

END (STRCPY)

	.p2align 4
	.section .rodata
L(ExitTable):
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
	.int    JMPTBL(L(Exit23), L(ExitTable))
	.int	JMPTBL(L(Exit24), L(ExitTable))
	.int	JMPTBL(L(Exit25), L(ExitTable))
	.int	JMPTBL(L(Exit26), L(ExitTable))
	.int	JMPTBL(L(Exit27), L(ExitTable))
	.int	JMPTBL(L(Exit28), L(ExitTable))
	.int	JMPTBL(L(Exit29), L(ExitTable))
	.int	JMPTBL(L(Exit30), L(ExitTable))
	.int	JMPTBL(L(Exit31), L(ExitTable))
	.int	JMPTBL(L(Exit32), L(ExitTable))
#ifdef USE_AS_STRNCPY
L(ExitStrncpyTable):
	.int	JMPTBL(L(Exit0), L(ExitStrncpyTable))
	.int	JMPTBL(L(StrncpyExit1), L(ExitStrncpyTable))
	.int	JMPTBL(L(StrncpyExit2), L(ExitStrncpyTable))
	.int	JMPTBL(L(StrncpyExit3), L(ExitStrncpyTable))
	.int	JMPTBL(L(StrncpyExit4), L(ExitStrncpyTable))
	.int	JMPTBL(L(StrncpyExit5), L(ExitStrncpyTable))
	.int	JMPTBL(L(StrncpyExit6), L(ExitStrncpyTable))
	.int	JMPTBL(L(StrncpyExit7), L(ExitStrncpyTable))
	.int	JMPTBL(L(StrncpyExit8), L(ExitStrncpyTable))
	.int	JMPTBL(L(StrncpyExit9), L(ExitStrncpyTable))
	.int	JMPTBL(L(StrncpyExit10), L(ExitStrncpyTable))
	.int	JMPTBL(L(StrncpyExit11), L(ExitStrncpyTable))
	.int	JMPTBL(L(StrncpyExit12), L(ExitStrncpyTable))
	.int	JMPTBL(L(StrncpyExit13), L(ExitStrncpyTable))
	.int	JMPTBL(L(StrncpyExit14), L(ExitStrncpyTable))
	.int	JMPTBL(L(StrncpyExit15), L(ExitStrncpyTable))
	.int	JMPTBL(L(StrncpyExit16), L(ExitStrncpyTable))
	.int	JMPTBL(L(StrncpyExit17), L(ExitStrncpyTable))
	.int	JMPTBL(L(StrncpyExit18), L(ExitStrncpyTable))
	.int	JMPTBL(L(StrncpyExit19), L(ExitStrncpyTable))
	.int	JMPTBL(L(StrncpyExit20), L(ExitStrncpyTable))
	.int	JMPTBL(L(StrncpyExit21), L(ExitStrncpyTable))
	.int	JMPTBL(L(StrncpyExit22), L(ExitStrncpyTable))
	.int    JMPTBL(L(StrncpyExit23), L(ExitStrncpyTable))
	.int	JMPTBL(L(StrncpyExit24), L(ExitStrncpyTable))
	.int	JMPTBL(L(StrncpyExit25), L(ExitStrncpyTable))
	.int	JMPTBL(L(StrncpyExit26), L(ExitStrncpyTable))
	.int	JMPTBL(L(StrncpyExit27), L(ExitStrncpyTable))
	.int	JMPTBL(L(StrncpyExit28), L(ExitStrncpyTable))
	.int	JMPTBL(L(StrncpyExit29), L(ExitStrncpyTable))
	.int	JMPTBL(L(StrncpyExit30), L(ExitStrncpyTable))
	.int	JMPTBL(L(StrncpyExit31), L(ExitStrncpyTable))
	.int	JMPTBL(L(StrncpyExit32), L(ExitStrncpyTable))
	.int	JMPTBL(L(StrncpyExit33), L(ExitStrncpyTable))

	.p2align 4
L(FillTable):
	.int	JMPTBL(L(Fill0), L(FillTable))
	.int	JMPTBL(L(Fill1), L(FillTable))
	.int	JMPTBL(L(Fill2), L(FillTable))
	.int	JMPTBL(L(Fill3), L(FillTable))
	.int	JMPTBL(L(Fill4), L(FillTable))
	.int	JMPTBL(L(Fill5), L(FillTable))
	.int	JMPTBL(L(Fill6), L(FillTable))
	.int	JMPTBL(L(Fill7), L(FillTable))
	.int	JMPTBL(L(Fill8), L(FillTable))
	.int	JMPTBL(L(Fill9), L(FillTable))
	.int	JMPTBL(L(Fill10), L(FillTable))
	.int	JMPTBL(L(Fill11), L(FillTable))
	.int	JMPTBL(L(Fill12), L(FillTable))
	.int	JMPTBL(L(Fill13), L(FillTable))
	.int	JMPTBL(L(Fill14), L(FillTable))
	.int	JMPTBL(L(Fill15), L(FillTable))
	.int	JMPTBL(L(Fill16), L(FillTable))
#endif
