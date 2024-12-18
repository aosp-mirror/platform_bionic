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

#ifndef STRLEN
# define STRLEN strlen
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
# define ENTRY(name)             \
	.type name,  @function;  \
	.globl name;             \
	.p2align 4;              \
name:                            \
	cfi_startproc
#endif

#ifndef END
# define END(name)               \
	cfi_endproc;             \
	.size name,	.-name
#endif

#define CFI_PUSH(REG)                   \
	cfi_adjust_cfa_offset (4);      \
	cfi_rel_offset (REG, 0)

#define CFI_POP(REG)                    \
	cfi_adjust_cfa_offset (-4);     \
	cfi_restore (REG)

#define PUSH(REG) pushl REG; CFI_PUSH (REG)
#define POP(REG) popl REG; CFI_POP (REG)

	.section .text.sse2,"ax",@progbits
ENTRY (STRLEN)
	mov	4(%esp), %edx
	mov	%edx, %ecx
	and	$0x3f, %ecx
	pxor	%xmm0, %xmm0
	cmp	$0x30, %ecx
	ja	L(next)
	movdqu	(%edx), %xmm1
	pcmpeqb	%xmm1, %xmm0
	pmovmskb %xmm0, %ecx
	test	%ecx, %ecx
	jnz	L(exit_less16)
	mov	%edx, %eax
	and	$-16, %eax
	jmp	L(align16_start)
L(next):
	mov	%edx, %eax
	and	$-16, %eax
	PUSH	(%edi)
	pcmpeqb	(%eax), %xmm0
	mov	$-1, %edi
	sub	%eax, %ecx
	shl	%cl, %edi
	pmovmskb %xmm0, %ecx
	and	%edi, %ecx
	POP	(%edi)
	jnz	L(exit_unaligned)
	pxor	%xmm0, %xmm0
L(align16_start):
	pxor	%xmm1, %xmm1
	pxor	%xmm2, %xmm2
	pxor	%xmm3, %xmm3
	pcmpeqb	16(%eax), %xmm0
	pmovmskb %xmm0, %ecx
	test	%ecx, %ecx
	jnz	L(exit16)

	pcmpeqb	32(%eax), %xmm1
	pmovmskb %xmm1, %ecx
	test	%ecx, %ecx
	jnz	L(exit32)

	pcmpeqb	48(%eax), %xmm2
	pmovmskb %xmm2, %ecx
	test	%ecx, %ecx
	jnz	L(exit48)

	pcmpeqb	64(%eax), %xmm3
	pmovmskb %xmm3, %ecx
	test	%ecx, %ecx
	jnz	L(exit64)

	pcmpeqb	80(%eax), %xmm0
	add	$64, %eax
	pmovmskb %xmm0, %ecx
	test	%ecx, %ecx
	jnz	L(exit16)

	pcmpeqb	32(%eax), %xmm1
	pmovmskb %xmm1, %ecx
	test	%ecx, %ecx
	jnz	L(exit32)

	pcmpeqb	48(%eax), %xmm2
	pmovmskb %xmm2, %ecx
	test	%ecx, %ecx
	jnz	L(exit48)

	pcmpeqb	64(%eax), %xmm3
	pmovmskb %xmm3, %ecx
	test	%ecx, %ecx
	jnz	L(exit64)

	pcmpeqb	80(%eax), %xmm0
	add	$64, %eax
	pmovmskb %xmm0, %ecx
	test	%ecx, %ecx
	jnz	L(exit16)

	pcmpeqb	32(%eax), %xmm1
	pmovmskb %xmm1, %ecx
	test	%ecx, %ecx
	jnz	L(exit32)

	pcmpeqb	48(%eax), %xmm2
	pmovmskb %xmm2, %ecx
	test	%ecx, %ecx
	jnz	L(exit48)

	pcmpeqb	64(%eax), %xmm3
	pmovmskb %xmm3, %ecx
	test	%ecx, %ecx
	jnz	L(exit64)

	pcmpeqb	80(%eax), %xmm0
	add	$64, %eax
	pmovmskb %xmm0, %ecx
	test	%ecx, %ecx
	jnz	L(exit16)

	pcmpeqb	32(%eax), %xmm1
	pmovmskb %xmm1, %ecx
	test	%ecx, %ecx
	jnz	L(exit32)

	pcmpeqb	48(%eax), %xmm2
	pmovmskb %xmm2, %ecx
	test	%ecx, %ecx
	jnz	L(exit48)

	pcmpeqb	64(%eax), %xmm3
	pmovmskb %xmm3, %ecx
	test	%ecx, %ecx
	jnz	L(exit64)


	test	$0x3f, %eax
	jz	L(align64_loop)

	pcmpeqb	80(%eax), %xmm0
	add	$80, %eax
	pmovmskb %xmm0, %ecx
	test	%ecx, %ecx
	jnz	L(exit)

	test	$0x3f, %eax
	jz	L(align64_loop)

	pcmpeqb	16(%eax), %xmm1
	add	$16, %eax
	pmovmskb %xmm1, %ecx
	test	%ecx, %ecx
	jnz	L(exit)

	test	$0x3f, %eax
	jz	L(align64_loop)

	pcmpeqb	16(%eax), %xmm2
	add	$16, %eax
	pmovmskb %xmm2, %ecx
	test	%ecx, %ecx
	jnz	L(exit)

	test	$0x3f, %eax
	jz	L(align64_loop)

	pcmpeqb	16(%eax), %xmm3
	add	$16, %eax
	pmovmskb %xmm3, %ecx
	test	%ecx, %ecx
	jnz	L(exit)

	add	$16, %eax
	.p2align 4
L(align64_loop):
	movaps	(%eax),	%xmm4
	pminub	16(%eax), 	%xmm4
	movaps	32(%eax), 	%xmm5
	pminub	48(%eax), 	%xmm5
	add	$64, 	%eax
	pminub	%xmm4,	%xmm5
	pcmpeqb	%xmm0,	%xmm5
	pmovmskb %xmm5,	%ecx
	test	%ecx,	%ecx
	jz	L(align64_loop)


	pcmpeqb	-64(%eax), %xmm0
	sub	$80, 	%eax
	pmovmskb %xmm0, %ecx
	test	%ecx, %ecx
	jnz	L(exit16)

	pcmpeqb	32(%eax), %xmm1
	pmovmskb %xmm1, %ecx
	test	%ecx, %ecx
	jnz	L(exit32)

	pcmpeqb	48(%eax), %xmm2
	pmovmskb %xmm2, %ecx
	test	%ecx, %ecx
	jnz	L(exit48)

	pcmpeqb	64(%eax), %xmm3
	pmovmskb %xmm3, %ecx
	sub	%edx, %eax
	bsf	%ecx, %ecx
	add	%ecx, %eax
	add	$64, %eax
	ret

	.p2align 4
L(exit):
	sub	%edx, %eax
	bsf	%ecx, %ecx
	add	%ecx, %eax
	ret

L(exit_less16):
	bsf	%ecx, %eax
	ret

	.p2align 4
L(exit_unaligned):
	sub	%edx, %eax
	bsf	%ecx, %ecx
	add	%ecx, %eax
	ret

	.p2align 4
L(exit16):
	sub	%edx, %eax
	bsf	%ecx, %ecx
	add	%ecx, %eax
	add	$16, %eax
	ret

	.p2align 4
L(exit32):
	sub	%edx, %eax
	bsf	%ecx, %ecx
	add	%ecx, %eax
	add	$32, %eax
	ret

	.p2align 4
L(exit48):
	sub	%edx, %eax
	bsf	%ecx, %ecx
	add	%ecx, %eax
	add	$48, %eax
	ret

	.p2align 4
L(exit64):
	sub	%edx, %eax
	bsf	%ecx, %ecx
	add	%ecx, %eax
	add	$64, %eax
	ret

END (STRLEN)

