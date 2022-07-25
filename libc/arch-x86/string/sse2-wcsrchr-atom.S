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

#define PUSH(REG)	pushl REG;	CFI_PUSH (REG)
#define POP(REG)	popl REG;	CFI_POP (REG)

#define PARMS  8
#define ENTRANCE PUSH(%edi);
#define RETURN  POP(%edi);	ret;	CFI_PUSH(%edi);

#define STR1  PARMS
#define STR2  STR1+4

	.text
ENTRY (wcsrchr)

	ENTRANCE
	mov	STR1(%esp), %ecx
	movd	STR2(%esp), %xmm1

	mov	%ecx, %edi
	punpckldq %xmm1, %xmm1
	pxor	%xmm2, %xmm2
	punpckldq %xmm1, %xmm1

/* ECX has OFFSET. */
	and	$63, %ecx
	cmp	$48, %ecx
	ja	L(crosscache)

/* unaligned string. */
	movdqu	(%edi), %xmm0
	pcmpeqd	%xmm0, %xmm2
	pcmpeqd	%xmm1, %xmm0
/* Find where NULL is.  */
	pmovmskb %xmm2, %ecx
/* Check if there is a match.  */
	pmovmskb %xmm0, %eax
	add	$16, %edi

	test	%eax, %eax
	jnz	L(unaligned_match1)

	test	%ecx, %ecx
	jnz	L(return_null)

	and	$-16, %edi

	PUSH	(%esi)

	xor	%edx, %edx
	jmp	L(loop)

	CFI_POP	(%esi)

	.p2align 4
L(unaligned_match1):
	test	%ecx, %ecx
	jnz	L(prolog_find_zero_1)

	PUSH	(%esi)

/* Save current match */
	mov	%eax, %edx
	mov	%edi, %esi
	and	$-16, %edi
	jmp	L(loop)

	CFI_POP	(%esi)

	.p2align 4
L(crosscache):
/* Hancle unaligned string.  */
	and	$15, %ecx
	and	$-16, %edi
	pxor	%xmm3, %xmm3
	movdqa	(%edi), %xmm0
	pcmpeqd	%xmm0, %xmm3
	pcmpeqd	%xmm1, %xmm0
/* Find where NULL is.  */
	pmovmskb %xmm3, %edx
/* Check if there is a match.  */
	pmovmskb %xmm0, %eax
/* Remove the leading bytes.  */
	shr	%cl, %edx
	shr	%cl, %eax
	add	$16, %edi

	test	%eax, %eax
	jnz	L(unaligned_match)

	test	%edx, %edx
	jnz	L(return_null)

	PUSH	(%esi)

	xor	%edx, %edx
	jmp	L(loop)

	CFI_POP	(%esi)

	.p2align 4
L(unaligned_match):
	test	%edx, %edx
	jnz	L(prolog_find_zero)

	PUSH	(%esi)

	mov	%eax, %edx
	lea	(%edi, %ecx), %esi

/* Loop start on aligned string.  */
	.p2align 4
L(loop):
	movdqa	(%edi), %xmm0
	pcmpeqd	%xmm0, %xmm2
	add	$16, %edi
	pcmpeqd	%xmm1, %xmm0
	pmovmskb %xmm2, %ecx
	pmovmskb %xmm0, %eax
	or	%eax, %ecx
	jnz	L(matches)

	movdqa	(%edi), %xmm3
	pcmpeqd	%xmm3, %xmm2
	add	$16, %edi
	pcmpeqd	%xmm1, %xmm3
	pmovmskb %xmm2, %ecx
	pmovmskb %xmm3, %eax
	or	%eax, %ecx
	jnz	L(matches)

	movdqa	(%edi), %xmm4
	pcmpeqd	%xmm4, %xmm2
	add	$16, %edi
	pcmpeqd	%xmm1, %xmm4
	pmovmskb %xmm2, %ecx
	pmovmskb %xmm4, %eax
	or	%eax, %ecx
	jnz	L(matches)

	movdqa	(%edi), %xmm5
	pcmpeqd	%xmm5, %xmm2
	add	$16, %edi
	pcmpeqd	%xmm1, %xmm5
	pmovmskb %xmm2, %ecx
	pmovmskb %xmm5, %eax
	or	%eax, %ecx
	jz	L(loop)

	.p2align 4
L(matches):
	test	%eax, %eax
	jnz	L(match)
L(return_value):
	test	%edx, %edx
	jz	L(return_null_1)
	mov	%edx, %eax
	mov	%esi, %edi

	POP	(%esi)

	test	%ah, %ah
	jnz	L(match_third_or_fourth_wchar)
	test	$15 << 4, %al
	jnz	L(match_second_wchar)
	lea	-16(%edi), %eax
	RETURN

	CFI_PUSH	(%esi)

	.p2align 4
L(return_null_1):
	POP	(%esi)

	xor	%eax, %eax
	RETURN

	CFI_PUSH	(%esi)

	.p2align 4
L(match):
	pmovmskb %xmm2, %ecx
	test	%ecx, %ecx
	jnz	L(find_zero)
/* save match info */
	mov	%eax, %edx
	mov	%edi, %esi
	jmp	L(loop)

	.p2align 4
L(find_zero):
	test	%cl, %cl
	jz	L(find_zero_in_third_or_fourth_wchar)
	test	$15, %cl
	jz	L(find_zero_in_second_wchar)
	and	$1, %eax
	jz	L(return_value)

	POP	(%esi)

	lea	-16(%edi), %eax
	RETURN

	CFI_PUSH	(%esi)

	.p2align 4
L(find_zero_in_second_wchar):
	and	$(1 << 5) - 1, %eax
	jz	L(return_value)

	POP	(%esi)

	test	$15 << 4, %al
	jnz	L(match_second_wchar)
	lea	-16(%edi), %eax
	RETURN

	CFI_PUSH	(%esi)

	.p2align 4
L(find_zero_in_third_or_fourth_wchar):
	test	$15, %ch
	jz	L(find_zero_in_fourth_wchar)
	and	$(1 << 9) - 1, %eax
	jz	L(return_value)

	POP	(%esi)

	test	%ah, %ah
	jnz	L(match_third_wchar)
	test	$15 << 4, %al
	jnz	L(match_second_wchar)
	lea	-16(%edi), %eax
	RETURN

	CFI_PUSH	(%esi)

	.p2align 4
L(find_zero_in_fourth_wchar):

	POP	(%esi)

	test	%ah, %ah
	jnz	L(match_third_or_fourth_wchar)
	test	$15 << 4, %al
	jnz	L(match_second_wchar)
	lea	-16(%edi), %eax
	RETURN

	CFI_PUSH	(%esi)

	.p2align 4
L(match_second_wchar):
	lea	-12(%edi), %eax
	RETURN

	.p2align 4
L(match_third_or_fourth_wchar):
	test	$15 << 4, %ah
	jnz	L(match_fourth_wchar)
	lea	-8(%edi), %eax
	RETURN

	.p2align 4
L(match_third_wchar):
	lea	-8(%edi), %eax
	RETURN

	.p2align 4
L(match_fourth_wchar):
	lea	-4(%edi), %eax
	RETURN

	.p2align 4
L(return_null):
	xor	%eax, %eax
	RETURN

	.p2align 4
L(prolog_find_zero):
	add	%ecx, %edi
	mov     %edx, %ecx
L(prolog_find_zero_1):
	test	%cl, %cl
	jz	L(prolog_find_zero_in_third_or_fourth_wchar)
	test	$15, %cl
	jz	L(prolog_find_zero_in_second_wchar)
	and	$1, %eax
	jz	L(return_null)

	lea	-16(%edi), %eax
	RETURN

	.p2align 4
L(prolog_find_zero_in_second_wchar):
	and	$(1 << 5) - 1, %eax
	jz	L(return_null)

	test	$15 << 4, %al
	jnz	L(match_second_wchar)
	lea	-16(%edi), %eax
	RETURN

	.p2align 4
L(prolog_find_zero_in_third_or_fourth_wchar):
	test	$15, %ch
	jz	L(prolog_find_zero_in_fourth_wchar)
	and	$(1 << 9) - 1, %eax
	jz	L(return_null)

	test	%ah, %ah
	jnz	L(match_third_wchar)
	test	$15 << 4, %al
	jnz	L(match_second_wchar)
	lea	-16(%edi), %eax
	RETURN

	.p2align 4
L(prolog_find_zero_in_fourth_wchar):
	test	%ah, %ah
	jnz	L(match_third_or_fourth_wchar)
	test	$15 << 4, %al
	jnz	L(match_second_wchar)
	lea	-16(%edi), %eax
	RETURN

END (wcsrchr)
