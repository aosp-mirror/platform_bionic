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
	.size name,	.-name
#endif

#define CFI_PUSH(REG)	\
	cfi_adjust_cfa_offset (4);	\
	cfi_rel_offset (REG, 0)

#define CFI_POP(REG)	\
	cfi_adjust_cfa_offset (-4);	\
	cfi_restore (REG)

#define PUSH(REG) pushl REG; CFI_PUSH (REG)
#define POP(REG) popl REG; CFI_POP (REG)

#define ENTRANCE PUSH (%edi);
#define PARMS  8
#define RETURN  POP (%edi); ret; CFI_PUSH (%edi);

#define STR1  PARMS
#define STR2  STR1+4
#define LEN   STR2+4

	.text
ENTRY (memchr)
	ENTRANCE
	mov	STR1(%esp), %ecx
	movd	STR2(%esp), %xmm1
	mov	LEN(%esp), %edx
	test	%edx, %edx
	jz	L(return_null)

	punpcklbw %xmm1, %xmm1
	mov	%ecx, %edi
	punpcklbw %xmm1, %xmm1

	and	$63, %ecx
	pshufd	$0, %xmm1, %xmm1
	cmp	$48, %ecx
	ja	L(crosscache)

	movdqu	(%edi), %xmm0
	pcmpeqb	%xmm1, %xmm0
	pmovmskb %xmm0, %eax
	test	%eax, %eax
	jnz	L(match_case2_prolog)

	sub	$16, %edx
	jbe	L(return_null)
	lea	16(%edi), %edi
	and	$15, %ecx
	and	$-16, %edi
	add	%ecx, %edx
	sub	$64, %edx
	jbe	L(exit_loop)
	jmp	L(loop_prolog)

	.p2align 4
L(crosscache):
	and	$15, %ecx
	and	$-16, %edi
	movdqa	(%edi), %xmm0
	pcmpeqb	%xmm1, %xmm0
	pmovmskb %xmm0, %eax
	sar	%cl, %eax
	test	%eax, %eax

	jnz	L(match_case2_prolog1)
	lea	-16(%edx), %edx
	add	%ecx, %edx
	jle	L(return_null)
	lea	16(%edi), %edi
	sub	$64, %edx
	jbe	L(exit_loop)

	.p2align 4
L(loop_prolog):
	movdqa	(%edi), %xmm0
	pcmpeqb	%xmm1, %xmm0
	xor	%ecx, %ecx
	pmovmskb %xmm0, %eax
	test	%eax, %eax
	jnz	L(match_case1)

	movdqa	16(%edi), %xmm2
	pcmpeqb	%xmm1, %xmm2
	lea	16(%ecx), %ecx
	pmovmskb %xmm2, %eax
	test	%eax, %eax
	jnz	L(match_case1)

	movdqa	32(%edi), %xmm3
	pcmpeqb	%xmm1, %xmm3
	lea	16(%ecx), %ecx
	pmovmskb %xmm3, %eax
	test	%eax, %eax
	jnz	L(match_case1)

	movdqa	48(%edi), %xmm4
	pcmpeqb	%xmm1, %xmm4
	lea	16(%ecx), %ecx
	pmovmskb %xmm4, %eax
	test	%eax, %eax
	jnz	L(match_case1)

	lea	64(%edi), %edi
	sub	$64, %edx
	jbe	L(exit_loop)

	movdqa	(%edi), %xmm0
	pcmpeqb	%xmm1, %xmm0
	xor	%ecx, %ecx
	pmovmskb %xmm0, %eax
	test	%eax, %eax
	jnz	L(match_case1)

	movdqa	16(%edi), %xmm2
	pcmpeqb	%xmm1, %xmm2
	lea	16(%ecx), %ecx
	pmovmskb %xmm2, %eax
	test	%eax, %eax
	jnz	L(match_case1)

	movdqa	32(%edi), %xmm3
	pcmpeqb	%xmm1, %xmm3
	lea	16(%ecx), %ecx
	pmovmskb %xmm3, %eax
	test	%eax, %eax
	jnz	L(match_case1)

	movdqa	48(%edi), %xmm4
	pcmpeqb	%xmm1, %xmm4
	lea	16(%ecx), %ecx
	pmovmskb %xmm4, %eax
	test	%eax, %eax
	jnz	L(match_case1)

	lea	64(%edi), %edi
	mov	%edi, %ecx
	and	$-64, %edi
	and	$63, %ecx
	add	%ecx, %edx

	.p2align 4
L(align64_loop):
	sub	$64, %edx
	jbe	L(exit_loop)
	movdqa	(%edi), %xmm0
	movdqa	16(%edi), %xmm2
	movdqa	32(%edi), %xmm3
	movdqa	48(%edi), %xmm4
	pcmpeqb	%xmm1, %xmm0
	pcmpeqb	%xmm1, %xmm2
	pcmpeqb	%xmm1, %xmm3
	pcmpeqb	%xmm1, %xmm4

	pmaxub	%xmm0, %xmm3
	pmaxub	%xmm2, %xmm4
	pmaxub	%xmm3, %xmm4
	add	$64, %edi
	pmovmskb %xmm4, %eax

	test	%eax, %eax
	jz	L(align64_loop)

	sub	$64, %edi

	pmovmskb %xmm0, %eax
	xor	%ecx, %ecx
	test	%eax, %eax
	jnz	L(match_case1)

	pmovmskb %xmm2, %eax
	lea	16(%ecx), %ecx
	test	%eax, %eax
	jnz	L(match_case1)

	movdqa	32(%edi), %xmm3
	pcmpeqb	%xmm1, %xmm3
	pmovmskb %xmm3, %eax
	lea	16(%ecx), %ecx
	test	%eax, %eax
	jnz	L(match_case1)

	pcmpeqb	48(%edi), %xmm1
	pmovmskb %xmm1, %eax
	lea	16(%ecx), %ecx

	.p2align 4
L(match_case1):
	add	%ecx, %edi
	test	%al, %al
	jz	L(match_case1_high)
	mov	%al, %cl
	and	$15, %cl
	jz	L(match_case1_8)
	test	$0x01, %al
	jnz	L(exit_case1_1)
	test	$0x02, %al
	jnz	L(exit_case1_2)
	test	$0x04, %al
	jnz	L(exit_case1_3)
	lea	3(%edi), %eax
	RETURN

	.p2align 4
L(match_case1_8):
	test	$0x10, %al
	jnz	L(exit_case1_5)
	test	$0x20, %al
	jnz	L(exit_case1_6)
	test	$0x40, %al
	jnz	L(exit_case1_7)
	lea	7(%edi), %eax
	RETURN

	.p2align 4
L(match_case1_high):
	mov	%ah, %ch
	and	$15, %ch
	jz	L(match_case1_high_8)
	test	$0x01, %ah
	jnz	L(exit_case1_9)
	test	$0x02, %ah
	jnz	L(exit_case1_10)
	test	$0x04, %ah
	jnz	L(exit_case1_11)
	lea	11(%edi), %eax
	RETURN

	.p2align 4
L(match_case1_high_8):
	test	$0x10, %ah
	jnz	L(exit_case1_13)
	test	$0x20, %ah
	jnz	L(exit_case1_14)
	test	$0x40, %ah
	jnz	L(exit_case1_15)
	lea	15(%edi), %eax
	RETURN

	.p2align 4
L(exit_loop):
	add	$64, %edx

	movdqa	(%edi), %xmm0
	pcmpeqb	%xmm1, %xmm0
	xor	%ecx, %ecx
	pmovmskb %xmm0, %eax
	test	%eax, %eax
	jnz	L(match_case2)
	cmp	$16, %edx
	jbe	L(return_null)

	movdqa	16(%edi), %xmm2
	pcmpeqb	%xmm1, %xmm2
	lea	16(%ecx), %ecx
	pmovmskb %xmm2, %eax
	test	%eax, %eax
	jnz	L(match_case2)
	cmp	$32, %edx
	jbe	L(return_null)

	movdqa	32(%edi), %xmm3
	pcmpeqb	%xmm1, %xmm3
	lea	16(%ecx), %ecx
	pmovmskb %xmm3, %eax
	test	%eax, %eax
	jnz	L(match_case2)
	cmp	$48, %edx
	jbe	L(return_null)

	pcmpeqb	48(%edi), %xmm1
	lea	16(%ecx), %ecx
	pmovmskb %xmm1, %eax
	test	%eax, %eax
	jnz	L(match_case2)

	xor	%eax, %eax
	RETURN

	.p2align 4
L(exit_case1_1):
	mov	%edi, %eax
	RETURN

	.p2align 4
L(exit_case1_2):
	lea	1(%edi), %eax
	RETURN

	.p2align 4
L(exit_case1_3):
	lea	2(%edi), %eax
	RETURN

	.p2align 4
L(exit_case1_5):
	lea	4(%edi), %eax
	RETURN

	.p2align 4
L(exit_case1_6):
	lea	5(%edi), %eax
	RETURN

	.p2align 4
L(exit_case1_7):
	lea	6(%edi), %eax
	RETURN

	.p2align 4
L(exit_case1_9):
	lea	8(%edi), %eax
	RETURN

	.p2align 4
L(exit_case1_10):
	lea	9(%edi), %eax
	RETURN

	.p2align 4
L(exit_case1_11):
	lea	10(%edi), %eax
	RETURN

	.p2align 4
L(exit_case1_13):
	lea	12(%edi), %eax
	RETURN

	.p2align 4
L(exit_case1_14):
	lea	13(%edi), %eax
	RETURN

	.p2align 4
L(exit_case1_15):
	lea	14(%edi), %eax
	RETURN

	.p2align 4
L(match_case2):
	sub	%ecx, %edx
L(match_case2_prolog1):
	add	%ecx, %edi
L(match_case2_prolog):
	test	%al, %al
	jz	L(match_case2_high)
	mov	%al, %cl
	and	$15, %cl
	jz	L(match_case2_8)
	test	$0x01, %al
	jnz	L(exit_case2_1)
	test	$0x02, %al
	jnz	L(exit_case2_2)
	test	$0x04, %al
	jnz	L(exit_case2_3)
	sub	$4, %edx
	jb	L(return_null)
	lea	3(%edi), %eax
	RETURN

	.p2align 4
L(match_case2_8):
	test	$0x10, %al
	jnz	L(exit_case2_5)
	test	$0x20, %al
	jnz	L(exit_case2_6)
	test	$0x40, %al
	jnz	L(exit_case2_7)
	sub	$8, %edx
	jb	L(return_null)
	lea	7(%edi), %eax
	RETURN

	.p2align 4
L(match_case2_high):
	mov	%ah, %ch
	and	$15, %ch
	jz	L(match_case2_high_8)
	test	$0x01, %ah
	jnz	L(exit_case2_9)
	test	$0x02, %ah
	jnz	L(exit_case2_10)
	test	$0x04, %ah
	jnz	L(exit_case2_11)
	sub	$12, %edx
	jb	L(return_null)
	lea	11(%edi), %eax
	RETURN

	.p2align 4
L(match_case2_high_8):
	test	$0x10, %ah
	jnz	L(exit_case2_13)
	test	$0x20, %ah
	jnz	L(exit_case2_14)
	test	$0x40, %ah
	jnz	L(exit_case2_15)
	sub	$16, %edx
	jb	L(return_null)
	lea	15(%edi), %eax
	RETURN

	.p2align 4
L(exit_case2_1):
	mov	%edi, %eax
	RETURN

	.p2align 4
L(exit_case2_2):
	sub	$2, %edx
	jb	L(return_null)
	lea	1(%edi), %eax
	RETURN

	.p2align 4
L(exit_case2_3):
	sub	$3, %edx
	jb	L(return_null)
	lea	2(%edi), %eax
	RETURN

	.p2align 4
L(exit_case2_5):
	sub	$5, %edx
	jb	L(return_null)
	lea	4(%edi), %eax
	RETURN

	.p2align 4
L(exit_case2_6):
	sub	$6, %edx
	jb	L(return_null)
	lea	5(%edi), %eax
	RETURN

	.p2align 4
L(exit_case2_7):
	sub	$7, %edx
	jb	L(return_null)
	lea	6(%edi), %eax
	RETURN

	.p2align 4
L(exit_case2_9):
	sub	$9, %edx
	jb	L(return_null)
	lea	8(%edi), %eax
	RETURN

	.p2align 4
L(exit_case2_10):
	sub	$10, %edx
	jb	L(return_null)
	lea	9(%edi), %eax
	RETURN

	.p2align 4
L(exit_case2_11):
	sub	$11, %edx
	jb	L(return_null)
	lea	10(%edi), %eax
	RETURN

	.p2align 4
L(exit_case2_13):
	sub	$13, %edx
	jb	L(return_null)
	lea	12(%edi), %eax
	RETURN

	.p2align 4
L(exit_case2_14):
	sub	$14, %edx
	jb	L(return_null)
	lea	13(%edi), %eax
	RETURN

	.p2align 4
L(exit_case2_15):
	sub	$15, %edx
	jb	L(return_null)
	lea	14(%edi), %eax
	RETURN
	.p2align 4
L(return_null):
	xor	%eax, %eax
	RETURN
END (memchr)
