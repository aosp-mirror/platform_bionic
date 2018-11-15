/*
Copyright (c) 2011, Intel Corporation
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

/* Optimized strlcat with SSSE3 */

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
	.type name,  @function;	\
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

#define PUSH(REG)	pushl	REG;	CFI_PUSH (REG)
#define POP(REG)	popl	REG;	CFI_POP (REG)
#define L(label)	.L##Prolog_##label

#define DST	4
#define SRC	DST+8
#define LEN	SRC+4

	.text
ENTRY (strlcat_ssse3)
	mov	DST(%esp), %edx
	PUSH	(%ebx)
	mov	LEN(%esp), %ebx
	sub	$4, %ebx
	jbe	L(len_less4_prolog)

#define RETURN	jmp	L(StrcpyStep)
#define edi	ebx

#define USE_AS_STRNLEN
#define USE_AS_STRCAT
#define USE_AS_STRLCAT

#include "sse2-strlen-atom.S"

	.p2align 4
L(StrcpyStep):

#undef edi
#undef L
#define L(label) .L##label
#undef RETURN
#define RETURN	POP (%ebx); ret; CFI_PUSH (%ebx);
#define RETURN1	POP (%edi); POP (%ebx); ret; CFI_PUSH (%ebx); CFI_PUSH (%edi)

        movl	SRC(%esp), %ecx
	movl	LEN(%esp), %ebx

	cmp	%eax, %ebx
	je	L(CalculateLengthOfSrcProlog)
	sub	%eax, %ebx

	test	%ebx, %ebx
	jz	L(CalculateLengthOfSrcProlog)

	mov	DST + 4(%esp), %edx

	PUSH	(%edi)
	add	%eax, %edx
	mov	%ecx, %edi
	sub	%eax, %edi

	cmp	$8, %ebx
	jbe	L(StrncpyExit8Bytes)

	cmpb	$0, (%ecx)
	jz	L(Exit1)
	cmpb	$0, 1(%ecx)
	jz	L(Exit2)
	cmpb	$0, 2(%ecx)
	jz	L(Exit3)
	cmpb	$0, 3(%ecx)
	jz	L(Exit4)
	cmpb	$0, 4(%ecx)
	jz	L(Exit5)
	cmpb	$0, 5(%ecx)
	jz	L(Exit6)
	cmpb	$0, 6(%ecx)
	jz	L(Exit7)
	cmpb	$0, 7(%ecx)
	jz	L(Exit8)
	cmp	$16, %ebx
	jb	L(StrncpyExit15Bytes)
	cmpb	$0, 8(%ecx)
	jz	L(Exit9)
	cmpb	$0, 9(%ecx)
	jz	L(Exit10)
	cmpb	$0, 10(%ecx)
	jz	L(Exit11)
	cmpb	$0, 11(%ecx)
	jz	L(Exit12)
	cmpb	$0, 12(%ecx)
	jz	L(Exit13)
	cmpb	$0, 13(%ecx)
	jz	L(Exit14)
	cmpb	$0, 14(%ecx)
	jz	L(Exit15)
	cmpb	$0, 15(%ecx)
	jz	L(Exit16)
	cmp	$16, %ebx
	je	L(StrlcpyExit16)

#define USE_AS_STRNCPY
#include "ssse3-strcpy-atom.S"

	.p2align 4
L(CopyFrom1To16Bytes):
	add	%esi, %edx
	add	%esi, %ecx

	POP	(%esi)
	test	%al, %al
	jz	L(ExitHigh8)

L(CopyFrom1To16BytesLess8):
	mov	%al, %ah
	and	$15, %ah
	jz	L(ExitHigh4)

	test	$0x01, %al
	jnz	L(Exit1)
	test	$0x02, %al
	jnz	L(Exit2)
	test	$0x04, %al
	jnz	L(Exit3)
L(Exit4):
	movl	(%ecx), %eax
	movl	%eax, (%edx)

	lea	3(%ecx), %eax
	sub	%edi, %eax
	RETURN1

	.p2align 4
L(ExitHigh4):
	test	$0x10, %al
	jnz	L(Exit5)
	test	$0x20, %al
	jnz	L(Exit6)
	test	$0x40, %al
	jnz	L(Exit7)
L(Exit8):
	movlpd	(%ecx), %xmm0
	movlpd	%xmm0, (%edx)

	lea	7(%ecx), %eax
	sub	%edi, %eax
	RETURN1

	.p2align 4
L(ExitHigh8):
	mov	%ah, %al
	and	$15, %al
	jz	L(ExitHigh12)

	test	$0x01, %ah
	jnz	L(Exit9)
	test	$0x02, %ah
	jnz	L(Exit10)
	test	$0x04, %ah
	jnz	L(Exit11)
L(Exit12):
	movlpd	(%ecx), %xmm0
	movlpd	%xmm0, (%edx)
	movl	8(%ecx), %eax
	movl	%eax, 8(%edx)

	lea	11(%ecx), %eax
	sub	%edi, %eax
	RETURN1

	.p2align 4
L(ExitHigh12):
	test	$0x10, %ah
	jnz	L(Exit13)
	test	$0x20, %ah
	jnz	L(Exit14)
	test	$0x40, %ah
	jnz	L(Exit15)
L(Exit16):
	movlpd	(%ecx), %xmm0
	movlpd	8(%ecx), %xmm1
	movlpd	%xmm0, (%edx)
	movlpd	%xmm1, 8(%edx)

	lea	15(%ecx), %eax
	sub	%edi, %eax
	RETURN1

	CFI_PUSH(%esi)

	.p2align 4
L(CopyFrom1To16BytesCase2):
	add	$16, %ebx
	add	%esi, %ecx
	add	%esi, %edx

	POP	(%esi)

	test	%al, %al
	jz	L(ExitHighCase2)

	cmp	$8, %ebx
	ja	L(CopyFrom1To16BytesLess8)

	test	$0x01, %al
	jnz	L(Exit1)
	cmp	$1, %ebx
	je	L(StrlcpyExit1)
	test	$0x02, %al
	jnz	L(Exit2)
	cmp	$2, %ebx
	je	L(StrlcpyExit2)
	test	$0x04, %al
	jnz	L(Exit3)
	cmp	$3, %ebx
	je	L(StrlcpyExit3)
	test	$0x08, %al
	jnz	L(Exit4)
	cmp	$4, %ebx
	je	L(StrlcpyExit4)
	test	$0x10, %al
	jnz	L(Exit5)
	cmp	$5, %ebx
	je	L(StrlcpyExit5)
	test	$0x20, %al
	jnz	L(Exit6)
	cmp	$6, %ebx
	je	L(StrlcpyExit6)
	test	$0x40, %al
	jnz	L(Exit7)
	cmp	$7, %ebx
	je	L(StrlcpyExit7)
	test	$0x80, %al
	jnz	L(Exit8)
	jmp	L(StrlcpyExit8)

	.p2align 4
L(ExitHighCase2):
	cmp	$8, %ebx
	jbe	L(CopyFrom1To16BytesLess8Case3)

	test	$0x01, %ah
	jnz	L(Exit9)
	cmp	$9, %ebx
	je	L(StrlcpyExit9)
	test	$0x02, %ah
	jnz	L(Exit10)
	cmp	$10, %ebx
	je	L(StrlcpyExit10)
	test	$0x04, %ah
	jnz	L(Exit11)
	cmp	$11, %ebx
	je	L(StrlcpyExit11)
	test	$0x8, %ah
	jnz	L(Exit12)
	cmp	$12, %ebx
	je	L(StrlcpyExit12)
	test	$0x10, %ah
	jnz	L(Exit13)
	cmp	$13, %ebx
	je	L(StrlcpyExit13)
	test	$0x20, %ah
	jnz	L(Exit14)
	cmp	$14, %ebx
	je	L(StrlcpyExit14)
	test	$0x40, %ah
	jnz	L(Exit15)
	cmp	$15, %ebx
	je	L(StrlcpyExit15)
	test	$0x80, %ah
	jnz	L(Exit16)
	jmp	L(StrlcpyExit16)

	CFI_PUSH(%esi)

	.p2align 4
L(CopyFrom1To16BytesCase2OrCase3):
	test	%eax, %eax
	jnz	L(CopyFrom1To16BytesCase2)

	.p2align 4
L(CopyFrom1To16BytesCase3):
	add	$16, %ebx
	add	%esi, %edx
	add	%esi, %ecx

	POP	(%esi)

	cmp	$8, %ebx
	ja	L(ExitHigh8Case3)

L(CopyFrom1To16BytesLess8Case3):
	cmp	$4, %ebx
	ja	L(ExitHigh4Case3)

	cmp	$1, %ebx
	je	L(StrlcpyExit1)
	cmp	$2, %ebx
	je	L(StrlcpyExit2)
	cmp	$3, %ebx
	je	L(StrlcpyExit3)
L(StrlcpyExit4):
	movb	%bh, 3(%edx)
	movw	(%ecx), %ax
	movw	%ax, (%edx)
	movb	2(%ecx), %al
	movb	%al, 2(%edx)

	lea	4(%ecx), %edx
	mov	%edi, %ecx
	POP	(%edi)
	jmp	L(CalculateLengthOfSrc)
        CFI_PUSH     (%edi)

	.p2align 4
L(ExitHigh4Case3):
	cmp	$5, %ebx
	je	L(StrlcpyExit5)
	cmp	$6, %ebx
	je	L(StrlcpyExit6)
	cmp	$7, %ebx
	je	L(StrlcpyExit7)
L(StrlcpyExit8):
	movb	%bh, 7(%edx)
	movl	(%ecx), %eax
	movl	%eax, (%edx)
	movl	3(%ecx), %eax
	movl	%eax, 3(%edx)

	lea	8(%ecx), %edx
	mov	%edi, %ecx
	POP	(%edi)
	jmp	L(CalculateLengthOfSrc)
        CFI_PUSH     (%edi)

	.p2align 4
L(ExitHigh8Case3):
	cmp	$12, %ebx
	ja	L(ExitHigh12Case3)

	cmp	$9, %ebx
	je	L(StrlcpyExit9)
	cmp	$10, %ebx
	je	L(StrlcpyExit10)
	cmp	$11, %ebx
	je	L(StrlcpyExit11)
L(StrlcpyExit12):
	movb	%bh, 11(%edx)
	movlpd	(%ecx), %xmm0
	movlpd	%xmm0, (%edx)
	movl	7(%ecx), %eax
	movl	%eax, 7(%edx)

	lea	12(%ecx), %edx
	mov	%edi, %ecx
	POP	(%edi)
	jmp	L(CalculateLengthOfSrc)
        CFI_PUSH     (%edi)

	.p2align 4
L(ExitHigh12Case3):
	cmp	$13, %ebx
	je	L(StrlcpyExit13)
	cmp	$14, %ebx
	je	L(StrlcpyExit14)
	cmp	$15, %ebx
	je	L(StrlcpyExit15)
L(StrlcpyExit16):
	movb	%bh, 15(%edx)
	movlpd	(%ecx), %xmm0
	movlpd	%xmm0, (%edx)
	movlpd	7(%ecx), %xmm0
	movlpd	%xmm0, 7(%edx)

	lea	16(%ecx), %edx
	mov	%edi, %ecx
	POP	(%edi)
	jmp	L(CalculateLengthOfSrc)
        CFI_PUSH     (%edi)

	.p2align 4
L(StrlcpyExit1):
	movb	%bh, (%edx)

	lea	1(%ecx), %edx
	mov	%edi, %ecx
	POP	(%edi)
	jmp	L(CalculateLengthOfSrc)
        CFI_PUSH     (%edi)

	.p2align 4
L(Exit1):
	movb	(%ecx), %al
	movb	%al, (%edx)

	mov	%ecx, %eax
	sub	%edi, %eax
	RETURN1

	.p2align 4
L(StrlcpyExit2):
	movb	%bh, 1(%edx)
	movb	(%ecx), %al
	movb	%al, (%edx)

	lea	2(%ecx), %edx
	mov	%edi, %ecx
	POP	(%edi)
	jmp	L(CalculateLengthOfSrc)
        CFI_PUSH     (%edi)

	.p2align 4
L(Exit2):
	movw	(%ecx), %ax
	movw	%ax, (%edx)
	movl	%edi, %eax

	lea	1(%ecx), %eax
	sub	%edi, %eax
	RETURN1

	.p2align 4
L(StrlcpyExit3):
	movb	%bh, 2(%edx)
	movw	(%ecx), %ax
	movw	%ax, (%edx)

	lea	3(%ecx), %edx
	mov	%edi, %ecx
	POP	(%edi)
	jmp	L(CalculateLengthOfSrc)
        CFI_PUSH     (%edi)

	.p2align 4
L(Exit3):
	movw	(%ecx), %ax
	movw	%ax, (%edx)
	movb	2(%ecx), %al
	movb	%al, 2(%edx)

	lea	2(%ecx), %eax
	sub	%edi, %eax
	RETURN1

	.p2align 4
L(StrlcpyExit5):
	movb	%bh, 4(%edx)
	movl	(%ecx), %eax
	movl	%eax, (%edx)
	movl	%edi, %eax

	lea	5(%ecx), %edx
	mov	%edi, %ecx
	POP	(%edi)
	jmp	L(CalculateLengthOfSrc)
        CFI_PUSH     (%edi)

	.p2align 4
L(Exit5):
	movl	(%ecx), %eax
	movl	%eax, (%edx)
	movb	4(%ecx), %al
	movb	%al, 4(%edx)

	lea	4(%ecx), %eax
	sub	%edi, %eax
	RETURN1

	.p2align 4
L(StrlcpyExit6):
	movb	%bh, 5(%edx)
	movl	(%ecx), %eax
	movl	%eax, (%edx)
	movb	4(%ecx), %al
	movb	%al, 4(%edx)

	lea	6(%ecx), %edx
	mov	%edi, %ecx
	POP	(%edi)
	jmp	L(CalculateLengthOfSrc)
        CFI_PUSH     (%edi)

	.p2align 4
L(Exit6):
	movl	(%ecx), %eax
	movl	%eax, (%edx)
	movw	4(%ecx), %ax
	movw	%ax, 4(%edx)

	lea	5(%ecx), %eax
	sub	%edi, %eax
	RETURN1

	.p2align 4
L(StrlcpyExit7):
	movb	%bh, 6(%edx)
	movl	(%ecx), %eax
	movl	%eax, (%edx)
	movw	4(%ecx), %ax
	movw	%ax, 4(%edx)

	lea	7(%ecx), %edx
	mov	%edi, %ecx
	POP	(%edi)
	jmp	L(CalculateLengthOfSrc)
        CFI_PUSH     (%edi)

	.p2align 4
L(Exit7):
	movl	(%ecx), %eax
	movl	%eax, (%edx)
	movl	3(%ecx), %eax
	movl	%eax, 3(%edx)

	lea	6(%ecx), %eax
	sub	%edi, %eax
	RETURN1

	.p2align 4
L(StrlcpyExit9):
	movb	%bh, 8(%edx)
	movlpd	(%ecx), %xmm0
	movlpd	%xmm0, (%edx)

	lea	9(%ecx), %edx
	mov	%edi, %ecx
	POP	(%edi)
	jmp	L(CalculateLengthOfSrc)
        CFI_PUSH     (%edi)

	.p2align 4
L(Exit9):
	movlpd	(%ecx), %xmm0
	movlpd	%xmm0, (%edx)
	movb	8(%ecx), %al
	movb	%al, 8(%edx)

	lea	8(%ecx), %eax
	sub	%edi, %eax
	RETURN1

	.p2align 4
L(StrlcpyExit10):
	movb	%bh, 9(%edx)
	movlpd	(%ecx), %xmm0
	movlpd	%xmm0, (%edx)
	movb	8(%ecx), %al
	movb	%al, 8(%edx)

	lea	10(%ecx), %edx
	mov	%edi, %ecx
	POP	(%edi)
	jmp	L(CalculateLengthOfSrc)
        CFI_PUSH     (%edi)

	.p2align 4
L(Exit10):
	movlpd	(%ecx), %xmm0
	movlpd	%xmm0, (%edx)
	movw	8(%ecx), %ax
	movw	%ax, 8(%edx)

	lea	9(%ecx), %eax
	sub	%edi, %eax
	RETURN1

	.p2align 4
L(StrlcpyExit11):
	movb	%bh, 10(%edx)
	movlpd	(%ecx), %xmm0
	movlpd	%xmm0, (%edx)
	movw	8(%ecx), %ax
	movw	%ax, 8(%edx)

	lea	11(%ecx), %edx
	mov	%edi, %ecx
	POP	(%edi)
	jmp	L(CalculateLengthOfSrc)
        CFI_PUSH     (%edi)

	.p2align 4
L(Exit11):
	movlpd	(%ecx), %xmm0
	movlpd	%xmm0, (%edx)
	movl	7(%ecx), %eax
	movl	%eax, 7(%edx)

	lea	10(%ecx), %eax
	sub	%edi, %eax
	RETURN1

	.p2align 4
L(StrlcpyExit13):
	movb	%bh, 12(%edx)
	movlpd	(%ecx), %xmm0
	movlpd	%xmm0, (%edx)
	movl	8(%ecx), %eax
	movl	%eax, 8(%edx)

	lea	13(%ecx), %edx
	mov	%edi, %ecx
	POP	(%edi)
	jmp	L(CalculateLengthOfSrc)
        CFI_PUSH     (%edi)

	.p2align 4
L(Exit13):
	movlpd	(%ecx), %xmm0
	movlpd	%xmm0, (%edx)
	movlpd	5(%ecx), %xmm0
	movlpd	%xmm0, 5(%edx)

	lea	12(%ecx), %eax
	sub	%edi, %eax
	RETURN1

	.p2align 4
L(StrlcpyExit14):
	movb	%bh, 13(%edx)
	movlpd	(%ecx), %xmm0
	movlpd	%xmm0, (%edx)
	movlpd	5(%ecx), %xmm0
	movlpd	%xmm0, 5(%edx)

	lea	14(%ecx), %edx
	mov	%edi, %ecx
	POP	(%edi)
	jmp	L(CalculateLengthOfSrc)
        CFI_PUSH     (%edi)

	.p2align 4
L(Exit14):
	movlpd	(%ecx), %xmm0
	movlpd	%xmm0, (%edx)
	movlpd	6(%ecx), %xmm0
	movlpd	%xmm0, 6(%edx)

	lea	13(%ecx), %eax
	sub	%edi, %eax
	RETURN1

	.p2align 4
L(StrlcpyExit15):
	movb	%bh, 14(%edx)
	movlpd	(%ecx), %xmm0
	movlpd	%xmm0, (%edx)
	movlpd	6(%ecx), %xmm0
	movlpd	%xmm0, 6(%edx)

	lea	15(%ecx), %edx
	mov	%edi, %ecx
	POP	(%edi)
	jmp	L(CalculateLengthOfSrc)
        CFI_PUSH     (%edi)

	.p2align 4
L(Exit15):
	movlpd	(%ecx), %xmm0
	movlpd	%xmm0, (%edx)
	movlpd	7(%ecx), %xmm0
	movlpd	%xmm0, 7(%edx)

	lea	14(%ecx), %eax
	sub	%edi, %eax
	RETURN1

	.p2align 4
L(StrncpyExit15Bytes):
	cmp	$12, %ebx
	ja	L(StrncpyExit15Bytes1)

	cmpb	$0, 8(%ecx)
	jz	L(Exit9)
	cmp	$9, %ebx
	je	L(StrlcpyExit9)

	cmpb	$0, 9(%ecx)
	jz	L(Exit10)
	cmp	$10, %ebx
	je	L(StrlcpyExit10)

	cmpb	$0, 10(%ecx)
	jz	L(Exit11)
	cmp	$11, %ebx
	je	L(StrlcpyExit11)

	cmpb	$0, 11(%ecx)
	jz	L(Exit12)
	jmp	L(StrlcpyExit12)

	.p2align 4
L(StrncpyExit15Bytes1):
	cmpb	$0, 8(%ecx)
	jz	L(Exit9)
	cmpb	$0, 9(%ecx)
	jz	L(Exit10)
	cmpb	$0, 10(%ecx)
	jz	L(Exit11)
	cmpb	$0, 11(%ecx)
	jz	L(Exit12)

	cmpb	$0, 12(%ecx)
	jz	L(Exit13)
	cmp	$13, %ebx
	je	L(StrlcpyExit13)

	cmpb	$0, 13(%ecx)
	jz	L(Exit14)
	cmp	$14, %ebx
	je	L(StrlcpyExit14)

	cmpb	$0, 14(%ecx)
	jz	L(Exit15)
	jmp	L(StrlcpyExit15)

	.p2align 4
L(StrncpyExit8Bytes):
	cmp	$4, %ebx
	ja	L(StrncpyExit8Bytes1)

	cmpb	$0, (%ecx)
	jz	L(Exit1)
	cmp	$1, %ebx
	je	L(StrlcpyExit1)

	cmpb	$0, 1(%ecx)
	jz	L(Exit2)
	cmp	$2, %ebx
	je	L(StrlcpyExit2)

	cmpb	$0, 2(%ecx)
	jz	L(Exit3)
	cmp	$3, %ebx
	je	L(StrlcpyExit3)

	cmpb	$0, 3(%ecx)
	jz	L(Exit4)
	jmp	L(StrlcpyExit4)

	.p2align 4
L(StrncpyExit8Bytes1):
	cmpb	$0, (%ecx)
	jz	L(Exit1)
	cmpb	$0, 1(%ecx)
	jz	L(Exit2)
	cmpb	$0, 2(%ecx)
	jz	L(Exit3)
	cmpb	$0, 3(%ecx)
	jz	L(Exit4)

	cmpb	$0, 4(%ecx)
	jz	L(Exit5)
	cmp	$5, %ebx
	je	L(StrlcpyExit5)

	cmpb	$0, 5(%ecx)
	jz	L(Exit6)
	cmp	$6, %ebx
	je	L(StrlcpyExit6)

	cmpb	$0, 6(%ecx)
	jz	L(Exit7)
	cmp	$7, %ebx
	je	L(StrlcpyExit7)

	cmpb	$0, 7(%ecx)
	jz	L(Exit8)
	jmp	L(StrlcpyExit8)

	CFI_POP	(%edi)


	.p2align 4
L(Prolog_return_start_len):
	movl	LEN(%esp), %ebx
        movl	SRC(%esp), %ecx
L(CalculateLengthOfSrcProlog):
	mov	%ecx, %edx
	sub	%ebx, %ecx

	.p2align 4
L(CalculateLengthOfSrc):
	cmpb	$0, (%edx)
	jz	L(exit_tail0)
	cmpb	$0, 1(%edx)
	jz	L(exit_tail1)
	cmpb	$0, 2(%edx)
	jz	L(exit_tail2)
	cmpb	$0, 3(%edx)
	jz	L(exit_tail3)

	cmpb	$0, 4(%edx)
	jz	L(exit_tail4)
	cmpb	$0, 5(%edx)
	jz	L(exit_tail5)
	cmpb	$0, 6(%edx)
	jz	L(exit_tail6)
	cmpb	$0, 7(%edx)
	jz	L(exit_tail7)

	cmpb	$0, 8(%edx)
	jz	L(exit_tail8)
	cmpb	$0, 9(%edx)
	jz	L(exit_tail9)
	cmpb	$0, 10(%edx)
	jz	L(exit_tail10)
	cmpb	$0, 11(%edx)
	jz	L(exit_tail11)

	cmpb	$0, 12(%edx)
	jz	L(exit_tail12)
	cmpb	$0, 13(%edx)
	jz	L(exit_tail13)
	cmpb	$0, 14(%edx)
	jz	L(exit_tail14)
	cmpb	$0, 15(%edx)
	jz	L(exit_tail15)

	pxor	%xmm0, %xmm0
	lea	16(%edx), %eax
	add	$16, %ecx
	and	$-16, %eax

	pcmpeqb	(%eax), %xmm0
	pmovmskb %xmm0, %edx
	pxor	%xmm1, %xmm1
	lea	16(%eax), %eax
	test	%edx, %edx
	jnz	L(exit)

	pcmpeqb	(%eax), %xmm1
	pmovmskb %xmm1, %edx
	pxor	%xmm2, %xmm2
	lea	16(%eax), %eax
	test	%edx, %edx
	jnz	L(exit)

	pcmpeqb	(%eax), %xmm2
	pmovmskb %xmm2, %edx
	pxor	%xmm3, %xmm3
	lea	16(%eax), %eax
	test	%edx, %edx
	jnz	L(exit)

	pcmpeqb	(%eax), %xmm3
	pmovmskb %xmm3, %edx
	lea	16(%eax), %eax
	test	%edx, %edx
	jnz	L(exit)

	pcmpeqb	(%eax), %xmm0
	pmovmskb %xmm0, %edx
	lea	16(%eax), %eax
	test	%edx, %edx
	jnz	L(exit)

	pcmpeqb	(%eax), %xmm1
	pmovmskb %xmm1, %edx
	lea	16(%eax), %eax
	test	%edx, %edx
	jnz	L(exit)

	pcmpeqb	(%eax), %xmm2
	pmovmskb %xmm2, %edx
	lea	16(%eax), %eax
	test	%edx, %edx
	jnz	L(exit)

	pcmpeqb	(%eax), %xmm3
	pmovmskb %xmm3, %edx
	lea	16(%eax), %eax
	test	%edx, %edx
	jnz	L(exit)

	pcmpeqb	(%eax), %xmm0
	pmovmskb %xmm0, %edx
	lea	16(%eax), %eax
	test	%edx, %edx
	jnz	L(exit)

	pcmpeqb	(%eax), %xmm1
	pmovmskb %xmm1, %edx
	lea	16(%eax), %eax
	test	%edx, %edx
	jnz	L(exit)

	pcmpeqb	(%eax), %xmm2
	pmovmskb %xmm2, %edx
	lea	16(%eax), %eax
	test	%edx, %edx
	jnz	L(exit)

	pcmpeqb	(%eax), %xmm3
	pmovmskb %xmm3, %edx
	lea	16(%eax), %eax
	test	%edx, %edx
	jnz	L(exit)

	pcmpeqb	(%eax), %xmm0
	pmovmskb %xmm0, %edx
	lea	16(%eax), %eax
	test	%edx, %edx
	jnz	L(exit)

	pcmpeqb	(%eax), %xmm1
	pmovmskb %xmm1, %edx
	lea	16(%eax), %eax
	test	%edx, %edx
	jnz	L(exit)

	pcmpeqb	(%eax), %xmm2
	pmovmskb %xmm2, %edx
	lea	16(%eax), %eax
	test	%edx, %edx
	jnz	L(exit)

	pcmpeqb	(%eax), %xmm3
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
	pcmpeqb	%xmm3, %xmm2
	pmovmskb %xmm2, %edx
	lea	64(%eax), %eax
	test	%edx, %edx
	jz	L(aligned_64_loop)

	pcmpeqb	-64(%eax), %xmm3
	pmovmskb %xmm3, %edx
	lea	48(%ecx), %ecx
	test	%edx, %edx
	jnz	L(exit)

	pcmpeqb	%xmm1, %xmm3
	pmovmskb %xmm3, %edx
	lea	-16(%ecx), %ecx
	test	%edx, %edx
	jnz	L(exit)

	pcmpeqb	-32(%eax), %xmm3
	pmovmskb %xmm3, %edx
	lea	-16(%ecx), %ecx
	test	%edx, %edx
	jnz	L(exit)

	pcmpeqb	%xmm6, %xmm3
	pmovmskb %xmm3, %edx
	lea	-16(%ecx), %ecx

	.p2align 4
L(exit):
	sub	%ecx, %eax
	test	%dl, %dl
	jz	L(exit_more_8)

	mov	%dl, %cl
	and	$15, %cl
	jz	L(exit_more_4)
	test	$0x01, %dl
	jnz	L(exit_0)
	test	$0x02, %dl
	jnz	L(exit_1)
	test	$0x04, %dl
	jnz	L(exit_2)
	add	$3, %eax
	RETURN

	.p2align 4
L(exit_more_4):
	test	$0x10, %dl
	jnz	L(exit_4)
	test	$0x20, %dl
	jnz	L(exit_5)
	test	$0x40, %dl
	jnz	L(exit_6)
	add	$7, %eax
	RETURN

	.p2align 4
L(exit_more_8):
	mov	%dh, %ch
	and	$15, %ch
	jz	L(exit_more_12)
	test	$0x01, %dh
	jnz	L(exit_8)
	test	$0x02, %dh
	jnz	L(exit_9)
	test	$0x04, %dh
	jnz	L(exit_10)
	add	$11, %eax
	RETURN

	.p2align 4
L(exit_more_12):
	test	$0x10, %dh
	jnz	L(exit_12)
	test	$0x20, %dh
	jnz	L(exit_13)
	test	$0x40, %dh
	jnz	L(exit_14)
	add	$15, %eax
L(exit_0):
	RETURN

	.p2align 4
L(exit_1):
	add	$1, %eax
	RETURN

L(exit_2):
	add	$2, %eax
	RETURN

L(exit_3):
	add	$3, %eax
	RETURN

L(exit_4):
	add	$4, %eax
	RETURN

L(exit_5):
	add	$5, %eax
	RETURN

L(exit_6):
	add	$6, %eax
	RETURN

L(exit_7):
	add	$7, %eax
	RETURN

L(exit_8):
	add	$8, %eax
	RETURN

L(exit_9):
	add	$9, %eax
	RETURN

L(exit_10):
	add	$10, %eax
	RETURN

L(exit_11):
	add	$11, %eax
	RETURN

L(exit_12):
	add	$12, %eax
	RETURN

L(exit_13):
	add	$13, %eax
	RETURN

L(exit_14):
	add	$14, %eax
	RETURN

L(exit_15):
	add	$15, %eax
	RETURN

L(exit_tail0):
	mov	%edx, %eax
	sub	%ecx, %eax
	RETURN

	.p2align 4
L(exit_tail1):
	lea	1(%edx), %eax
	sub	%ecx, %eax
	RETURN

L(exit_tail2):
	lea	2(%edx), %eax
	sub	%ecx, %eax
	RETURN

L(exit_tail3):
	lea	3(%edx), %eax
	sub	%ecx, %eax
	RETURN

L(exit_tail4):
	lea	4(%edx), %eax
	sub	%ecx, %eax
	RETURN

L(exit_tail5):
	lea	5(%edx), %eax
	sub	%ecx, %eax
	RETURN

L(exit_tail6):
	lea	6(%edx), %eax
	sub	%ecx, %eax
	RETURN

L(exit_tail7):
	lea	7(%edx), %eax
	sub	%ecx, %eax
	RETURN

L(exit_tail8):
	lea	8(%edx), %eax
	sub	%ecx, %eax
	RETURN

L(exit_tail9):
	lea	9(%edx), %eax
	sub	%ecx, %eax
	RETURN

L(exit_tail10):
	lea	10(%edx), %eax
	sub	%ecx, %eax
	RETURN

L(exit_tail11):
	lea	11(%edx), %eax
	sub	%ecx, %eax
	RETURN

L(exit_tail12):
	lea	12(%edx), %eax
	sub	%ecx, %eax
	RETURN

L(exit_tail13):
	lea	13(%edx), %eax
	sub	%ecx, %eax
	RETURN

L(exit_tail14):
	lea	14(%edx), %eax
	sub	%ecx, %eax
	RETURN

L(exit_tail15):
	lea	15(%edx), %eax
	sub	%ecx, %eax
	RETURN

END (strlcat)
