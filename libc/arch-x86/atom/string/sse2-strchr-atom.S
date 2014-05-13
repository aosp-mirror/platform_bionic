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

#define PUSH(REG)	pushl REG;	CFI_PUSH (REG)
#define POP(REG)	popl REG;	CFI_POP (REG)

#define PARMS	8
#define ENTRANCE	PUSH(%edi)
#define RETURN	POP (%edi); ret; CFI_PUSH (%edi);


#define STR1	PARMS
#define STR2	STR1+4

	.text
ENTRY (strchr)

	ENTRANCE
	mov	STR1(%esp), %ecx
	movd	STR2(%esp), %xmm1

	pxor	%xmm2, %xmm2
	mov	%ecx, %edi
	punpcklbw %xmm1, %xmm1
	punpcklbw %xmm1, %xmm1
	/* ECX has OFFSET. */
	and	$15, %ecx
	pshufd	$0, %xmm1, %xmm1
	je	L(loop)

/* Handle unaligned string.  */
	and	$-16, %edi
	movdqa	(%edi), %xmm0
	pcmpeqb	%xmm0, %xmm2
	pcmpeqb	%xmm1, %xmm0
	/* Find where NULL is.  */
	pmovmskb %xmm2, %edx
	/* Check if there is a match.  */
	pmovmskb %xmm0, %eax
	/* Remove the leading bytes.  */
	sarl	%cl, %edx
	sarl	%cl, %eax
	test	%eax, %eax
	jz	L(unaligned_no_match)
	add	%ecx, %edi
	test	%edx, %edx
	jz	L(match_case1)
	jmp	L(match_case2)

	.p2align 4
L(unaligned_no_match):
	test	%edx, %edx
	jne	L(return_null)

	pxor	%xmm2, %xmm2
	add	$16, %edi

	.p2align 4
/* Loop start on aligned string.  */
L(loop):
	movdqa	(%edi), %xmm0
	pcmpeqb	%xmm0, %xmm2
	pcmpeqb	%xmm1, %xmm0
	pmovmskb %xmm2, %edx
	pmovmskb %xmm0, %eax
	test	%eax, %eax
	jnz	L(matches)
	test	%edx, %edx
	jnz	L(return_null)
	add	$16, %edi

	movdqa	(%edi), %xmm0
	pcmpeqb	%xmm0, %xmm2
	pcmpeqb	%xmm1, %xmm0
	pmovmskb %xmm2, %edx
	pmovmskb %xmm0, %eax
	test	%eax, %eax
	jnz	L(matches)
	test	%edx, %edx
	jnz	L(return_null)
	add	$16, %edi

	movdqa	(%edi), %xmm0
	pcmpeqb	%xmm0, %xmm2
	pcmpeqb	%xmm1, %xmm0
	pmovmskb %xmm2, %edx
	pmovmskb %xmm0, %eax
	test	%eax, %eax
	jnz	L(matches)
	test	%edx, %edx
	jnz	L(return_null)
	add	$16, %edi

	movdqa	(%edi), %xmm0
	pcmpeqb	%xmm0, %xmm2
	pcmpeqb	%xmm1, %xmm0
	pmovmskb %xmm2, %edx
	pmovmskb %xmm0, %eax
	test	%eax, %eax
	jnz	L(matches)
	test	%edx, %edx
	jnz	L(return_null)
	add	$16, %edi
	jmp	L(loop)

L(matches):
	/* There is a match.  First find where NULL is.  */
	test	%edx, %edx
	jz	L(match_case1)

	.p2align 4
L(match_case2):
	test	%al, %al
	jz	L(match_higth_case2)

	mov	%al, %cl
	and	$15, %cl
	jnz	L(match_case2_4)

	mov	%dl, %ch
	and	$15, %ch
	jnz	L(return_null)

	test	$0x10, %al
	jnz	L(Exit5)
	test	$0x10, %dl
	jnz	L(return_null)
	test	$0x20, %al
	jnz	L(Exit6)
	test	$0x20, %dl
	jnz	L(return_null)
	test	$0x40, %al
	jnz	L(Exit7)
	test	$0x40, %dl
	jnz	L(return_null)
	lea	7(%edi), %eax
	RETURN

	.p2align 4
L(match_case2_4):
	test	$0x01, %al
	jnz	L(Exit1)
	test	$0x01, %dl
	jnz	L(return_null)
	test	$0x02, %al
	jnz	L(Exit2)
	test	$0x02, %dl
	jnz	L(return_null)
	test	$0x04, %al
	jnz	L(Exit3)
	test	$0x04, %dl
	jnz	L(return_null)
	lea	3(%edi), %eax
	RETURN

	.p2align 4
L(match_higth_case2):
	test	%dl, %dl
	jnz	L(return_null)

	mov	%ah, %cl
	and	$15, %cl
	jnz	L(match_case2_12)

	mov	%dh, %ch
	and	$15, %ch
	jnz	L(return_null)

	test	$0x10, %ah
	jnz	L(Exit13)
	test	$0x10, %dh
	jnz	L(return_null)
	test	$0x20, %ah
	jnz	L(Exit14)
	test	$0x20, %dh
	jnz	L(return_null)
	test	$0x40, %ah
	jnz	L(Exit15)
	test	$0x40, %dh
	jnz	L(return_null)
	lea	15(%edi), %eax
	RETURN

	.p2align 4
L(match_case2_12):
	test	$0x01, %ah
	jnz	L(Exit9)
	test	$0x01, %dh
	jnz	L(return_null)
	test	$0x02, %ah
	jnz	L(Exit10)
	test	$0x02, %dh
	jnz	L(return_null)
	test	$0x04, %ah
	jnz	L(Exit11)
	test	$0x04, %dh
	jnz	L(return_null)
	lea	11(%edi), %eax
	RETURN

	.p2align 4
L(match_case1):
	test	%al, %al
	jz	L(match_higth_case1)

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
	lea	7(%edi), %eax
	RETURN

	.p2align 4
L(match_higth_case1):
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
	lea	15(%edi), %eax
	RETURN

	.p2align 4
L(Exit1):
	lea	(%edi), %eax
	RETURN

	.p2align 4
L(Exit2):
	lea	1(%edi), %eax
	RETURN

	.p2align 4
L(Exit3):
	lea	2(%edi), %eax
	RETURN

	.p2align 4
L(Exit4):
	lea	3(%edi), %eax
	RETURN

	.p2align 4
L(Exit5):
	lea	4(%edi), %eax
	RETURN

	.p2align 4
L(Exit6):
	lea	5(%edi), %eax
	RETURN

	.p2align 4
L(Exit7):
	lea	6(%edi), %eax
	RETURN

	.p2align 4
L(Exit9):
	lea	8(%edi), %eax
	RETURN

	.p2align 4
L(Exit10):
	lea	9(%edi), %eax
	RETURN

	.p2align 4
L(Exit11):
	lea	10(%edi), %eax
	RETURN

	.p2align 4
L(Exit12):
	lea	11(%edi), %eax
	RETURN

	.p2align 4
L(Exit13):
	lea	12(%edi), %eax
	RETURN

	.p2align 4
L(Exit14):
	lea	13(%edi), %eax
	RETURN

	.p2align 4
L(Exit15):
	lea	14(%edi), %eax
	RETURN

	.p2align 4
L(return_null):
	xor	%eax, %eax
	RETURN

END (strchr)
