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

#define PARMS  4
#define STR1  PARMS
#define STR2  STR1+4
#define LEN   STR2+4

	.text
ENTRY (memrchr)
	mov	STR1(%esp), %ecx
	movd	STR2(%esp), %xmm1
	mov	LEN(%esp), %edx

	test	%edx, %edx
	jz	L(return_null)
	sub	$16, %edx
	jbe	L(length_less16)

	punpcklbw %xmm1, %xmm1
	add	%edx, %ecx
	punpcklbw %xmm1, %xmm1

	movdqu	(%ecx), %xmm0
	pshufd	$0, %xmm1, %xmm1
	pcmpeqb	%xmm1, %xmm0

	pmovmskb %xmm0, %eax
	test	%eax, %eax
	jnz	L(exit_dispatch)

	sub	$64, %ecx
	mov	%ecx, %eax
	and	$15, %eax
	jz	L(loop_prolog)

	add	$16, %ecx
	add	$16, %edx
	and	$-16, %ecx
	sub	%eax, %edx

	.p2align 4
/* Loop start on aligned string.  */
L(loop_prolog):
	sub	$64, %edx
	jbe	L(exit_loop)

	movdqa	48(%ecx), %xmm0
	pcmpeqb	%xmm1, %xmm0
	pmovmskb %xmm0, %eax
	test	%eax, %eax
	jnz	L(matches48)

	movdqa	32(%ecx), %xmm2
	pcmpeqb	%xmm1, %xmm2
	pmovmskb %xmm2, %eax
	test	%eax, %eax
	jnz	L(matches32)

	movdqa	16(%ecx), %xmm3
	pcmpeqb	%xmm1, %xmm3
	pmovmskb %xmm3, %eax
	test	%eax, %eax
	jnz	L(matches16)

	movdqa	(%ecx), %xmm4
	pcmpeqb	%xmm1, %xmm4
	pmovmskb %xmm4, %eax
	test	%eax, %eax
	jnz	L(exit_dispatch)

	sub	$64, %ecx
	sub	$64, %edx
	jbe	L(exit_loop)

	movdqa	48(%ecx), %xmm0
	pcmpeqb	%xmm1, %xmm0
	pmovmskb %xmm0, %eax
	test	%eax, %eax
	jnz	L(matches48)

	movdqa	32(%ecx), %xmm2
	pcmpeqb	%xmm1, %xmm2
	pmovmskb %xmm2, %eax
	test	%eax, %eax
	jnz	L(matches32)

	movdqa	16(%ecx), %xmm3
	pcmpeqb	%xmm1, %xmm3
	pmovmskb %xmm3, %eax
	test	%eax, %eax
	jnz	L(matches16)

	movdqa	(%ecx), %xmm3
	pcmpeqb	%xmm1, %xmm3
	pmovmskb %xmm3, %eax
	test	%eax, %eax
	jnz	L(exit_dispatch)

	mov	%ecx, %eax
	and	$63, %eax
	test	%eax, %eax
	jz	L(align64_loop)

	add	$64, %ecx
	add	$64, %edx
	and	$-64, %ecx
	sub	%eax, %edx

	.p2align 4
L(align64_loop):
	sub	$64, %ecx
	sub	$64, %edx
	jbe	L(exit_loop)

	movdqa	(%ecx), %xmm0
	movdqa	16(%ecx), %xmm2
	movdqa	32(%ecx), %xmm3
	movdqa	48(%ecx), %xmm4

	pcmpeqb	%xmm1, %xmm0
	pcmpeqb	%xmm1, %xmm2
	pcmpeqb	%xmm1, %xmm3
	pcmpeqb	%xmm1, %xmm4

	pmaxub	%xmm3, %xmm0
	pmaxub	%xmm4, %xmm2
	pmaxub	%xmm0, %xmm2
	pmovmskb %xmm2, %eax

	test	%eax, %eax
	jz	L(align64_loop)

	pmovmskb %xmm4, %eax
	test	%eax, %eax
	jnz	L(matches48)

	pmovmskb %xmm3, %eax
	test	%eax, %eax
	jnz	L(matches32)

	movdqa	16(%ecx), %xmm2

	pcmpeqb	%xmm1, %xmm2
	pcmpeqb	(%ecx), %xmm1

	pmovmskb %xmm2, %eax
	test	%eax, %eax
	jnz	L(matches16)

	pmovmskb %xmm1, %eax
	test	%ah, %ah
	jnz	L(exit_dispatch_high)
	mov	%al, %dl
	and	$15 << 4, %dl
	jnz	L(exit_dispatch_8)
	test	$0x08, %al
	jnz	L(exit_4)
	test	$0x04, %al
	jnz	L(exit_3)
	test	$0x02, %al
	jnz	L(exit_2)
	mov	%ecx, %eax
	ret

	.p2align 4
L(exit_loop):
	add	$64, %edx
	cmp	$32, %edx
	jbe	L(exit_loop_32)

	movdqa	48(%ecx), %xmm0
	pcmpeqb	%xmm1, %xmm0
	pmovmskb %xmm0, %eax
	test	%eax, %eax
	jnz	L(matches48)

	movdqa	32(%ecx), %xmm2
	pcmpeqb	%xmm1, %xmm2
	pmovmskb %xmm2, %eax
	test	%eax, %eax
	jnz	L(matches32)

	movdqa	16(%ecx), %xmm3
	pcmpeqb	%xmm1, %xmm3
	pmovmskb %xmm3, %eax
	test	%eax, %eax
	jnz	L(matches16_1)
	cmp	$48, %edx
	jbe	L(return_null)

	pcmpeqb	(%ecx), %xmm1
	pmovmskb %xmm1, %eax
	test	%eax, %eax
	jnz	L(matches0_1)
	xor	%eax, %eax
	ret

	.p2align 4
L(exit_loop_32):
	movdqa	48(%ecx), %xmm0
	pcmpeqb	%xmm1, %xmm0
	pmovmskb %xmm0, %eax
	test	%eax, %eax
	jnz	L(matches48_1)
	cmp	$16, %edx
	jbe	L(return_null)

	pcmpeqb	32(%ecx), %xmm1
	pmovmskb %xmm1, %eax
	test	%eax, %eax
	jnz	L(matches32_1)
	xor	%eax, %eax
	ret

	.p2align 4
L(matches16):
	lea	16(%ecx), %ecx
	test	%ah, %ah
	jnz	L(exit_dispatch_high)
	mov	%al, %dl
	and	$15 << 4, %dl
	jnz	L(exit_dispatch_8)
	test	$0x08, %al
	jnz	L(exit_4)
	test	$0x04, %al
	jnz	L(exit_3)
	test	$0x02, %al
	jnz	L(exit_2)
	mov	%ecx, %eax
	ret

	.p2align 4
L(matches32):
	lea	32(%ecx), %ecx
	test	%ah, %ah
	jnz	L(exit_dispatch_high)
	mov	%al, %dl
	and	$15 << 4, %dl
	jnz	L(exit_dispatch_8)
	test	$0x08, %al
	jnz	L(exit_4)
	test	$0x04, %al
	jnz	L(exit_3)
	test	$0x02, %al
	jnz	L(exit_2)
	mov	%ecx, %eax
	ret

	.p2align 4
L(matches48):
	lea	48(%ecx), %ecx

	.p2align 4
L(exit_dispatch):
	test	%ah, %ah
	jnz	L(exit_dispatch_high)
	mov	%al, %dl
	and	$15 << 4, %dl
	jnz	L(exit_dispatch_8)
	test	$0x08, %al
	jnz	L(exit_4)
	test	$0x04, %al
	jnz	L(exit_3)
	test	$0x02, %al
	jnz	L(exit_2)
	mov	%ecx, %eax
	ret

	.p2align 4
L(exit_dispatch_8):
	test	$0x80, %al
	jnz	L(exit_8)
	test	$0x40, %al
	jnz	L(exit_7)
	test	$0x20, %al
	jnz	L(exit_6)
	lea	4(%ecx), %eax
	ret

	.p2align 4
L(exit_dispatch_high):
	mov	%ah, %dh
	and	$15 << 4, %dh
	jnz	L(exit_dispatch_high_8)
	test	$0x08, %ah
	jnz	L(exit_12)
	test	$0x04, %ah
	jnz	L(exit_11)
	test	$0x02, %ah
	jnz	L(exit_10)
	lea	8(%ecx), %eax
	ret

	.p2align 4
L(exit_dispatch_high_8):
	test	$0x80, %ah
	jnz	L(exit_16)
	test	$0x40, %ah
	jnz	L(exit_15)
	test	$0x20, %ah
	jnz	L(exit_14)
	lea	12(%ecx), %eax
	ret

	.p2align 4
L(exit_2):
	lea	1(%ecx), %eax
	ret

	.p2align 4
L(exit_3):
	lea	2(%ecx), %eax
	ret

	.p2align 4
L(exit_4):
	lea	3(%ecx), %eax
	ret

	.p2align 4
L(exit_6):
	lea	5(%ecx), %eax
	ret

	.p2align 4
L(exit_7):
	lea	6(%ecx), %eax
	ret

	.p2align 4
L(exit_8):
	lea	7(%ecx), %eax
	ret

	.p2align 4
L(exit_10):
	lea	9(%ecx), %eax
	ret

	.p2align 4
L(exit_11):
	lea	10(%ecx), %eax
	ret

	.p2align 4
L(exit_12):
	lea	11(%ecx), %eax
	ret

	.p2align 4
L(exit_14):
	lea	13(%ecx), %eax
	ret

	.p2align 4
L(exit_15):
	lea	14(%ecx), %eax
	ret

	.p2align 4
L(exit_16):
	lea	15(%ecx), %eax
	ret

	.p2align 4
L(matches0_1):
	lea	-64(%edx), %edx

	test	%ah, %ah
	jnz	L(exit_dispatch_1_high)
	mov	%al, %ah
	and	$15 << 4, %ah
	jnz	L(exit_dispatch_1_8)
	test	$0x08, %al
	jnz	L(exit_1_4)
	test	$0x04, %al
	jnz	L(exit_1_3)
	test	$0x02, %al
	jnz	L(exit_1_2)

	add	$0, %edx
	jl	L(return_null)
	mov	%ecx, %eax
	ret

	.p2align 4
L(matches16_1):
	lea	-48(%edx), %edx
	lea	16(%ecx), %ecx

	test	%ah, %ah
	jnz	L(exit_dispatch_1_high)
	mov	%al, %ah
	and	$15 << 4, %ah
	jnz	L(exit_dispatch_1_8)
	test	$0x08, %al
	jnz	L(exit_1_4)
	test	$0x04, %al
	jnz	L(exit_1_3)
	test	$0x02, %al
	jnz	L(exit_1_2)

	add	$0, %edx
	jl	L(return_null)
	mov	%ecx, %eax
	ret

	.p2align 4
L(matches32_1):
	lea	-32(%edx), %edx
	lea	32(%ecx), %ecx

	test	%ah, %ah
	jnz	L(exit_dispatch_1_high)
	mov	%al, %ah
	and	$15 << 4, %ah
	jnz	L(exit_dispatch_1_8)
	test	$0x08, %al
	jnz	L(exit_1_4)
	test	$0x04, %al
	jnz	L(exit_1_3)
	test	$0x02, %al
	jnz	L(exit_1_2)

	add	$0, %edx
	jl	L(return_null)
	mov	%ecx, %eax
	ret

	.p2align 4
L(matches48_1):
	lea	-16(%edx), %edx
	lea	48(%ecx), %ecx

	.p2align 4
L(exit_dispatch_1):
	test	%ah, %ah
	jnz	L(exit_dispatch_1_high)
	mov	%al, %ah
	and	$15 << 4, %ah
	jnz	L(exit_dispatch_1_8)
	test	$0x08, %al
	jnz	L(exit_1_4)
	test	$0x04, %al
	jnz	L(exit_1_3)
	test	$0x02, %al
	jnz	L(exit_1_2)

	add	$0, %edx
	jl	L(return_null)
	mov	%ecx, %eax
	ret

	.p2align 4
L(exit_dispatch_1_8):
	test	$0x80, %al
	jnz	L(exit_1_8)
	test	$0x40, %al
	jnz	L(exit_1_7)
	test	$0x20, %al
	jnz	L(exit_1_6)

	add	$4, %edx
	jl	L(return_null)
	lea	4(%ecx), %eax
	ret

	.p2align 4
L(exit_dispatch_1_high):
	mov	%ah, %al
	and	$15 << 4, %al
	jnz	L(exit_dispatch_1_high_8)
	test	$0x08, %ah
	jnz	L(exit_1_12)
	test	$0x04, %ah
	jnz	L(exit_1_11)
	test	$0x02, %ah
	jnz	L(exit_1_10)

	add	$8, %edx
	jl	L(return_null)
	lea	8(%ecx), %eax
	ret

	.p2align 4
L(exit_dispatch_1_high_8):
	test	$0x80, %ah
	jnz	L(exit_1_16)
	test	$0x40, %ah
	jnz	L(exit_1_15)
	test	$0x20, %ah
	jnz	L(exit_1_14)

	add	$12, %edx
	jl	L(return_null)
	lea	12(%ecx), %eax
	ret

	.p2align 4
L(exit_1_2):
	add	$1, %edx
	jl	L(return_null)
	lea	1(%ecx), %eax
	ret

	.p2align 4
L(exit_1_3):
	add	$2, %edx
	jl	L(return_null)
	lea	2(%ecx), %eax
	ret

	.p2align 4
L(exit_1_4):
	add	$3, %edx
	jl	L(return_null)
	lea	3(%ecx), %eax
	ret

	.p2align 4
L(exit_1_6):
	add	$5, %edx
	jl	L(return_null)
	lea	5(%ecx), %eax
	ret

	.p2align 4
L(exit_1_7):
	add	$6, %edx
	jl	L(return_null)
	lea	6(%ecx), %eax
	ret

	.p2align 4
L(exit_1_8):
	add	$7, %edx
	jl	L(return_null)
	lea	7(%ecx), %eax
	ret

	.p2align 4
L(exit_1_10):
	add	$9, %edx
	jl	L(return_null)
	lea	9(%ecx), %eax
	ret

	.p2align 4
L(exit_1_11):
	add	$10, %edx
	jl	L(return_null)
	lea	10(%ecx), %eax
	ret

	.p2align 4
L(exit_1_12):
	add	$11, %edx
	jl	L(return_null)
	lea	11(%ecx), %eax
	ret

	.p2align 4
L(exit_1_14):
	add	$13, %edx
	jl	L(return_null)
	lea	13(%ecx), %eax
	ret

	.p2align 4
L(exit_1_15):
	add	$14, %edx
	jl	L(return_null)
	lea	14(%ecx), %eax
	ret

	.p2align 4
L(exit_1_16):
	add	$15, %edx
	jl	L(return_null)
	lea	15(%ecx), %eax
	ret

	.p2align 4
L(return_null):
	xor	%eax, %eax
	ret

	.p2align 4
L(length_less16_offset0):
	mov	%dl, %cl
	pcmpeqb	(%eax), %xmm1

	mov	$1, %edx
	sal	%cl, %edx
	sub	$1, %edx

	mov	%eax, %ecx
	pmovmskb %xmm1, %eax

	and	%edx, %eax
	test	%eax, %eax
	jnz	L(exit_dispatch)

	xor	%eax, %eax
	ret

	.p2align 4
L(length_less16):
	punpcklbw %xmm1, %xmm1
	add	$16, %edx
	punpcklbw %xmm1, %xmm1

	mov	%ecx, %eax
	pshufd	$0, %xmm1, %xmm1

	and	$15, %ecx
	jz	L(length_less16_offset0)

	PUSH	(%edi)

	mov	%cl, %dh
	add	%dl, %dh
	and	$-16, %eax

	sub	$16, %dh
	ja	L(length_less16_part2)

	pcmpeqb	(%eax), %xmm1
	pmovmskb %xmm1, %edi

	sar	%cl, %edi
	add	%ecx, %eax
	mov	%dl, %cl

	mov	$1, %edx
	sal	%cl, %edx
	sub	$1, %edx

	and	%edx, %edi
	test	%edi, %edi
	jz	L(ret_null)

	bsr	%edi, %edi
	add	%edi, %eax
	POP	(%edi)
	ret

	CFI_PUSH     (%edi)

	.p2align 4
L(length_less16_part2):
	movdqa	16(%eax), %xmm2
	pcmpeqb	%xmm1, %xmm2
	pmovmskb %xmm2, %edi

	mov	%cl, %ch

	mov	%dh, %cl
	mov	$1, %edx
	sal	%cl, %edx
	sub	$1, %edx

	and	%edx, %edi

	test	%edi, %edi
	jnz	L(length_less16_part2_return)

	pcmpeqb	(%eax), %xmm1
	pmovmskb %xmm1, %edi

	mov	%ch, %cl
	sar	%cl, %edi
	test	%edi, %edi
	jz	L(ret_null)

	bsr	%edi, %edi
	add	%edi, %eax
	xor	%ch, %ch
	add	%ecx, %eax
	POP	(%edi)
	ret

	CFI_PUSH     (%edi)

	.p2align 4
L(length_less16_part2_return):
	bsr	%edi, %edi
	lea	16(%eax, %edi), %eax
	POP	(%edi)
	ret

	CFI_PUSH     (%edi)

	.p2align 4
L(ret_null):
	xor	%eax, %eax
	POP	(%edi)
	ret

END (memrchr)
