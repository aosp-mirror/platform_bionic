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

#ifndef L
# define L(label)	.L##label
#endif

#ifndef cfi_startproc
# define cfi_startproc			.cfi_startproc
#endif

#ifndef cfi_endproc
# define cfi_endproc			.cfi_endproc
#endif

#ifndef cfi_rel_offset
# define cfi_rel_offset(reg, off)	.cfi_rel_offset reg, off
#endif

#ifndef cfi_restore
# define cfi_restore(reg)		.cfi_restore reg
#endif

#ifndef cfi_adjust_cfa_offset
# define cfi_adjust_cfa_offset(off)	.cfi_adjust_cfa_offset off
#endif

#ifndef cfi_remember_state
# define cfi_remember_state		.cfi_remember_state
#endif

#ifndef cfi_restore_state
# define cfi_restore_state		.cfi_restore_state
#endif

#ifndef ENTRY
# define ENTRY(name)			\
	.type name,  @function; 	\
	.globl name;			\
	.p2align 4;			\
name:					\
	cfi_startproc
#endif

#ifndef END
# define END(name)			\
	cfi_endproc;			\
	.size name, .-name
#endif

#define CFI_PUSH(REG)			\
  cfi_adjust_cfa_offset (4);		\
  cfi_rel_offset (REG, 0)

#define CFI_POP(REG)			\
  cfi_adjust_cfa_offset (-4);		\
  cfi_restore (REG)

#define PUSH(REG)	pushl REG; CFI_PUSH (REG)
#define POP(REG)	popl REG; CFI_POP (REG)

#ifndef STRCAT
# define STRCAT	strcat
#endif

#define PARMS	4
#define STR1	PARMS+4
#define STR2	STR1+4

#ifdef USE_AS_STRNCAT
# define LEN	STR2+8
#endif

#define USE_AS_STRCAT

	.section .text.ssse3,"ax",@progbits
ENTRY (STRCAT)
	PUSH	(%edi)
	mov	STR1(%esp), %edi
	mov	%edi, %edx

#define RETURN	jmp	L(StrcpyAtom)
#include "sse2-strlen-atom.S"

L(StrcpyAtom):
	mov	STR2(%esp), %ecx
	lea	(%edi, %eax), %edx
#ifdef USE_AS_STRNCAT
	PUSH	(%ebx)
	mov	LEN(%esp), %ebx
	test	%ebx, %ebx
	jz	L(StrncatExit0)
	cmp	$8, %ebx
	jbe	L(StrncpyExit8Bytes)
#endif
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
	cmpb	$0, 8(%ecx)
	jz	L(Exit9)
#ifdef USE_AS_STRNCAT
	cmp	$16, %ebx
	jb	L(StrncpyExit15Bytes)
#endif
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
#ifdef USE_AS_STRNCAT
	cmp	$16, %ebx
	je	L(StrncatExit16)

# define RETURN1	POP (%ebx); POP (%edi);	ret; \
	CFI_PUSH (%ebx); CFI_PUSH (%edi)
# define USE_AS_STRNCPY
#else
# define RETURN1	POP(%edi); ret; CFI_PUSH(%edi)
#endif
#include "ssse3-strcpy-atom.S"

	.p2align 4
L(CopyFrom1To16Bytes):
	add	%esi, %edx
	add	%esi, %ecx

	POP	(%esi)
	test	%al, %al
	jz	L(ExitHigh)
	test	$0x01, %al
	jnz	L(Exit1)
	test	$0x02, %al
	jnz	L(Exit2)
	test	$0x04, %al
	jnz	L(Exit3)
	test	$0x08, %al
	jnz	L(Exit4)
	test	$0x10, %al
	jnz	L(Exit5)
	test	$0x20, %al
	jnz	L(Exit6)
	test	$0x40, %al
	jnz	L(Exit7)
	movlpd	(%ecx), %xmm0
	movlpd	%xmm0, (%edx)
	movl	%edi, %eax
	RETURN1

	.p2align 4
L(ExitHigh):
	test	$0x01, %ah
	jnz	L(Exit9)
	test	$0x02, %ah
	jnz	L(Exit10)
	test	$0x04, %ah
	jnz	L(Exit11)
	test	$0x08, %ah
	jnz	L(Exit12)
	test	$0x10, %ah
	jnz	L(Exit13)
	test	$0x20, %ah
	jnz	L(Exit14)
	test	$0x40, %ah
	jnz	L(Exit15)
	movlpd	(%ecx), %xmm0
	movlpd	8(%ecx), %xmm1
	movlpd	%xmm0, (%edx)
	movlpd	%xmm1, 8(%edx)
	movl	%edi, %eax
	RETURN1

	.p2align 4
L(StrncatExit1):
	movb	%bh, 1(%edx)
L(Exit1):
	movb	(%ecx), %al
	movb	%al, (%edx)
	movl	%edi, %eax
	RETURN1

	.p2align 4
L(StrncatExit2):
	movb	%bh, 2(%edx)
L(Exit2):
	movw	(%ecx), %ax
	movw	%ax, (%edx)
	movl	%edi, %eax
	RETURN1

	.p2align 4
L(StrncatExit3):
	movb	%bh, 3(%edx)
L(Exit3):
	movw	(%ecx), %ax
	movw	%ax, (%edx)
	movb	2(%ecx), %al
	movb	%al, 2(%edx)
	movl	%edi, %eax
	RETURN1

	.p2align 4
L(StrncatExit4):
	movb	%bh, 4(%edx)
L(Exit4):
	movl	(%ecx), %eax
	movl	%eax, (%edx)
	movl	%edi, %eax
	RETURN1

	.p2align 4
L(StrncatExit5):
	movb	%bh, 5(%edx)
L(Exit5):
	movl	(%ecx), %eax
	movl	%eax, (%edx)
	movb	4(%ecx), %al
	movb	%al, 4(%edx)
	movl	%edi, %eax
	RETURN1

	.p2align 4
L(StrncatExit6):
	movb	%bh, 6(%edx)
L(Exit6):
	movl	(%ecx), %eax
	movl	%eax, (%edx)
	movw	4(%ecx), %ax
	movw	%ax, 4(%edx)
	movl	%edi, %eax
	RETURN1

	.p2align 4
L(StrncatExit7):
	movb	%bh, 7(%edx)
L(Exit7):
	movl	(%ecx), %eax
	movl	%eax, (%edx)
	movl	3(%ecx), %eax
	movl	%eax, 3(%edx)
	movl	%edi, %eax
	RETURN1

	.p2align 4
L(StrncatExit8):
	movb	%bh, 8(%edx)
L(Exit8):
	movlpd	(%ecx), %xmm0
	movlpd	%xmm0, (%edx)
	movl	%edi, %eax
	RETURN1

	.p2align 4
L(StrncatExit9):
	movb	%bh, 9(%edx)
L(Exit9):
	movlpd	(%ecx), %xmm0
	movlpd	%xmm0, (%edx)
	movb	8(%ecx), %al
	movb	%al, 8(%edx)
	movl	%edi, %eax
	RETURN1

	.p2align 4
L(StrncatExit10):
	movb	%bh, 10(%edx)
L(Exit10):
	movlpd	(%ecx), %xmm0
	movlpd	%xmm0, (%edx)
	movw	8(%ecx), %ax
	movw	%ax, 8(%edx)
	movl	%edi, %eax
	RETURN1

	.p2align 4
L(StrncatExit11):
	movb	%bh, 11(%edx)
L(Exit11):
	movlpd	(%ecx), %xmm0
	movlpd	%xmm0, (%edx)
	movl	7(%ecx), %eax
	movl	%eax, 7(%edx)
	movl	%edi, %eax
	RETURN1

	.p2align 4
L(StrncatExit12):
	movb	%bh, 12(%edx)
L(Exit12):
	movlpd	(%ecx), %xmm0
	movlpd	%xmm0, (%edx)
	movl	8(%ecx), %eax
	movl	%eax, 8(%edx)
	movl	%edi, %eax
	RETURN1

	.p2align 4
L(StrncatExit13):
	movb	%bh, 13(%edx)
L(Exit13):
	movlpd	(%ecx), %xmm0
	movlpd	%xmm0, (%edx)
	movlpd	5(%ecx), %xmm0
	movlpd	%xmm0, 5(%edx)
	movl	%edi, %eax
	RETURN1

	.p2align 4
L(StrncatExit14):
	movb	%bh, 14(%edx)
L(Exit14):
	movlpd	(%ecx), %xmm0
	movlpd	%xmm0, (%edx)
	movlpd	6(%ecx), %xmm0
	movlpd	%xmm0, 6(%edx)
	movl	%edi, %eax
	RETURN1

	.p2align 4
L(StrncatExit15):
	movb	%bh, 15(%edx)
L(Exit15):
	movlpd	(%ecx), %xmm0
	movlpd	%xmm0, (%edx)
	movlpd	7(%ecx), %xmm0
	movlpd	%xmm0, 7(%edx)
	movl	%edi, %eax
	RETURN1

	.p2align 4
L(StrncatExit16):
	movb	%bh, 16(%edx)
L(Exit16):
	movlpd	(%ecx), %xmm0
	movlpd	8(%ecx), %xmm1
	movlpd	%xmm0, (%edx)
	movlpd	%xmm1, 8(%edx)
	movl	%edi, %eax
	RETURN1

#ifdef USE_AS_STRNCPY

	CFI_PUSH(%esi)

	.p2align 4
L(CopyFrom1To16BytesCase2):
	add	$16, %ebx
	add	%esi, %ecx
	lea	(%esi, %edx), %esi
	lea	-9(%ebx), %edx
	and	$1<<7, %dh
	or	%al, %dh
	lea	(%esi), %edx
	POP	(%esi)
	jz	L(ExitHighCase2)

	test	$0x01, %al
	jnz	L(Exit1)
	cmp	$1, %ebx
	je	L(StrncatExit1)
	test	$0x02, %al
	jnz	L(Exit2)
	cmp	$2, %ebx
	je	L(StrncatExit2)
	test	$0x04, %al
	jnz	L(Exit3)
	cmp	$3, %ebx
	je	L(StrncatExit3)
	test	$0x08, %al
	jnz	L(Exit4)
	cmp	$4, %ebx
	je	L(StrncatExit4)
	test	$0x10, %al
	jnz	L(Exit5)
	cmp	$5, %ebx
	je	L(StrncatExit5)
	test	$0x20, %al
	jnz	L(Exit6)
	cmp	$6, %ebx
	je	L(StrncatExit6)
	test	$0x40, %al
	jnz	L(Exit7)
	cmp	$7, %ebx
	je	L(StrncatExit7)
	movlpd	(%ecx), %xmm0
	movlpd	%xmm0, (%edx)
	lea	7(%edx), %eax
	cmpb	$1, (%eax)
	sbb	$-1, %eax
	xor	%cl, %cl
	movb	%cl, (%eax)
	movl	%edi, %eax
	RETURN1

	.p2align 4
L(ExitHighCase2):
	test	$0x01, %ah
	jnz	L(Exit9)
	cmp	$9, %ebx
	je	L(StrncatExit9)
	test	$0x02, %ah
	jnz	L(Exit10)
	cmp	$10, %ebx
	je	L(StrncatExit10)
	test	$0x04, %ah
	jnz	L(Exit11)
	cmp	$11, %ebx
	je	L(StrncatExit11)
	test	$0x8, %ah
	jnz	L(Exit12)
	cmp	$12, %ebx
	je	L(StrncatExit12)
	test	$0x10, %ah
	jnz	L(Exit13)
	cmp	$13, %ebx
	je	L(StrncatExit13)
	test	$0x20, %ah
	jnz	L(Exit14)
	cmp	$14, %ebx
	je	L(StrncatExit14)
	test	$0x40, %ah
	jnz	L(Exit15)
	cmp	$15, %ebx
	je	L(StrncatExit15)
	movlpd	(%ecx), %xmm0
	movlpd	%xmm0, (%edx)
	movlpd	8(%ecx), %xmm1
	movlpd	%xmm1, 8(%edx)
	movl	%edi, %eax
	RETURN1

	CFI_PUSH(%esi)

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
	ja	L(ExitHighCase3)
	cmp	$1, %ebx
	je	L(StrncatExit1)
	cmp	$2, %ebx
	je	L(StrncatExit2)
	cmp	$3, %ebx
	je	L(StrncatExit3)
	cmp	$4, %ebx
	je	L(StrncatExit4)
	cmp	$5, %ebx
	je	L(StrncatExit5)
	cmp	$6, %ebx
	je	L(StrncatExit6)
	cmp	$7, %ebx
	je	L(StrncatExit7)
	movlpd	(%ecx), %xmm0
	movlpd	%xmm0, (%edx)
	movb	%bh, 8(%edx)
	movl	%edi, %eax
	RETURN1

	.p2align 4
L(ExitHighCase3):
	cmp	$9, %ebx
	je	L(StrncatExit9)
	cmp	$10, %ebx
	je	L(StrncatExit10)
	cmp	$11, %ebx
	je	L(StrncatExit11)
	cmp	$12, %ebx
	je	L(StrncatExit12)
	cmp	$13, %ebx
	je	L(StrncatExit13)
	cmp	$14, %ebx
	je	L(StrncatExit14)
	cmp	$15, %ebx
	je	L(StrncatExit15)
	movlpd	(%ecx), %xmm0
	movlpd	%xmm0, (%edx)
	movlpd	8(%ecx), %xmm1
	movlpd	%xmm1, 8(%edx)
	movb	%bh, 16(%edx)
	movl	%edi, %eax
	RETURN1

	.p2align 4
L(StrncatExit0):
	movl	%edi, %eax
	RETURN1

	.p2align 4
L(StrncpyExit15Bytes):
	cmp	$9, %ebx
	je	L(StrncatExit9)
	cmpb	$0, 9(%ecx)
	jz	L(Exit10)
	cmp	$10, %ebx
	je	L(StrncatExit10)
	cmpb	$0, 10(%ecx)
	jz	L(Exit11)
	cmp	$11, %ebx
	je	L(StrncatExit11)
	cmpb	$0, 11(%ecx)
	jz	L(Exit12)
	cmp	$12, %ebx
	je	L(StrncatExit12)
	cmpb	$0, 12(%ecx)
	jz	L(Exit13)
	cmp	$13, %ebx
	je	L(StrncatExit13)
	cmpb	$0, 13(%ecx)
	jz	L(Exit14)
	cmp	$14, %ebx
	je	L(StrncatExit14)
	movlpd	(%ecx), %xmm0
	movlpd	%xmm0, (%edx)
	movlpd	7(%ecx), %xmm0
	movlpd	%xmm0, 7(%edx)
	lea	14(%edx), %eax
	cmpb	$1, (%eax)
	sbb	$-1, %eax
	movb	%bh, (%eax)
	movl	%edi, %eax
	RETURN1

	.p2align 4
L(StrncpyExit8Bytes):
	cmpb	$0, (%ecx)
	jz	L(Exit1)
	cmp	$1, %ebx
	je	L(StrncatExit1)
	cmpb	$0, 1(%ecx)
	jz	L(Exit2)
	cmp	$2, %ebx
	je	L(StrncatExit2)
	cmpb	$0, 2(%ecx)
	jz	L(Exit3)
	cmp	$3, %ebx
	je	L(StrncatExit3)
	cmpb	$0, 3(%ecx)
	jz	L(Exit4)
	cmp	$4, %ebx
	je	L(StrncatExit4)
	cmpb	$0, 4(%ecx)
	jz	L(Exit5)
	cmp	$5, %ebx
	je	L(StrncatExit5)
	cmpb	$0, 5(%ecx)
	jz	L(Exit6)
	cmp	$6, %ebx
	je	L(StrncatExit6)
	cmpb	$0, 6(%ecx)
	jz	L(Exit7)
	cmp	$7, %ebx
	je	L(StrncatExit7)
	movlpd	(%ecx), %xmm0
	movlpd	%xmm0, (%edx)
	lea	7(%edx), %eax
	cmpb	$1, (%eax)
	sbb	$-1, %eax
	movb	%bh, (%eax)
	movl	%edi, %eax
	RETURN1

#endif
END (STRCAT)
