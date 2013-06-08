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

#define PUSH(REG)	pushl REG; CFI_PUSH (REG)
#define POP(REG)	popl REG; CFI_POP (REG)

#define PARMS	4


#define STR1  PARMS
#define STR2  STR1+4

	.text
ENTRY (wcschr)

	mov	STR1(%esp), %ecx
	movd	STR2(%esp), %xmm1

	mov	%ecx, %eax
	punpckldq %xmm1, %xmm1
	pxor	%xmm2, %xmm2
	punpckldq %xmm1, %xmm1

	and	$63, %eax
	cmp	$48, %eax
	ja	L(cross_cache)

	movdqu	(%ecx), %xmm0
	pcmpeqd	%xmm0, %xmm2
	pcmpeqd	%xmm1, %xmm0
	pmovmskb %xmm2, %edx
	pmovmskb %xmm0, %eax
	or	%eax, %edx
	jnz	L(matches)
	and	$-16, %ecx
	jmp	L(loop)

	.p2align 4
L(cross_cache):
	PUSH	(%edi)
	mov	%ecx, %edi
	mov	%eax, %ecx
	and	$-16, %edi
	and	$15, %ecx
	movdqa	(%edi), %xmm0
	pcmpeqd	%xmm0, %xmm2
	pcmpeqd	%xmm1, %xmm0
	pmovmskb %xmm2, %edx
	pmovmskb %xmm0, %eax

	sarl	%cl, %edx
	sarl	%cl, %eax
	test	%eax, %eax
	jz	L(unaligned_no_match)

	add	%edi, %ecx
	POP	(%edi)

	test	%edx, %edx
	jz	L(match_case1)
	test	%al, %al
	jz	L(match_higth_case2)
	test	$15, %al
	jnz	L(match_case2_4)
	test	$15, %dl
	jnz	L(return_null)
	lea	4(%ecx), %eax
	ret

	CFI_PUSH (%edi)

	.p2align 4
L(unaligned_no_match):
	mov	%edi, %ecx
	POP	(%edi)

	test	%edx, %edx
	jnz	L(return_null)

	pxor	%xmm2, %xmm2

/* Loop start on aligned string.  */
	.p2align 4
L(loop):
	add	$16, %ecx
	movdqa	(%ecx), %xmm0
	pcmpeqd	%xmm0, %xmm2
	pcmpeqd	%xmm1, %xmm0
	pmovmskb %xmm2, %edx
	pmovmskb %xmm0, %eax
	or	%eax, %edx
	jnz	L(matches)
	add	$16, %ecx

	movdqa	(%ecx), %xmm0
	pcmpeqd	%xmm0, %xmm2
	pcmpeqd	%xmm1, %xmm0
	pmovmskb %xmm2, %edx
	pmovmskb %xmm0, %eax
	or	%eax, %edx
	jnz	L(matches)
	add	$16, %ecx

	movdqa	(%ecx), %xmm0
	pcmpeqd	%xmm0, %xmm2
	pcmpeqd	%xmm1, %xmm0
	pmovmskb %xmm2, %edx
	pmovmskb %xmm0, %eax
	or	%eax, %edx
	jnz	L(matches)
	add	$16, %ecx

	movdqa	(%ecx), %xmm0
	pcmpeqd	%xmm0, %xmm2
	pcmpeqd	%xmm1, %xmm0
	pmovmskb %xmm2, %edx
	pmovmskb %xmm0, %eax
	or	%eax, %edx
	jz	L(loop)

	.p2align 4
L(matches):
	pmovmskb %xmm2, %edx
	test	%eax, %eax
	jz	L(return_null)
	test	%edx, %edx
	jz	L(match_case1)

	.p2align 4
L(match_case2):
	test	%al, %al
	jz	L(match_higth_case2)
	test	$15, %al
	jnz	L(match_case2_4)
	test	$15, %dl
	jnz	L(return_null)
	lea	4(%ecx), %eax
	ret

	.p2align 4
L(match_case2_4):
	mov	%ecx, %eax
	ret

	.p2align 4
L(match_higth_case2):
	test	%dl, %dl
	jnz	L(return_null)
	test	$15, %ah
	jnz	L(match_case2_12)
	test	$15, %dh
	jnz	L(return_null)
	lea	12(%ecx), %eax
	ret

	.p2align 4
L(match_case2_12):
	lea	8(%ecx), %eax
	ret

	.p2align 4
L(match_case1):
	test	%al, %al
	jz	L(match_higth_case1)

	test	$0x01, %al
	jnz	L(exit0)
	lea	4(%ecx), %eax
	ret

	.p2align 4
L(match_higth_case1):
	test	$0x01, %ah
	jnz	L(exit3)
	lea	12(%ecx), %eax
	ret

	.p2align 4
L(exit0):
	mov	%ecx, %eax
	ret

	.p2align 4
L(exit3):
	lea	8(%ecx), %eax
	ret

	.p2align 4
L(return_null):
	xor	%eax, %eax
	ret

END (wcschr)
