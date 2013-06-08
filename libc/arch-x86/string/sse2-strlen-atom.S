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

#ifndef USE_AS_STRCAT

# ifndef STRLEN
#  define STRLEN strlen
# endif

# ifndef L
#  define L(label)	.L##label
# endif

# ifndef cfi_startproc
#  define cfi_startproc	.cfi_startproc
# endif

# ifndef cfi_endproc
#  define cfi_endproc	.cfi_endproc
# endif

/* calee safe register only for strnlen is required */

# ifdef USE_AS_STRNLEN
#  ifndef cfi_rel_offset
#   define cfi_rel_offset(reg, off)	.cfi_rel_offset reg, off
#  endif

#  ifndef cfi_restore
#   define cfi_restore(reg)	.cfi_restore reg
#  endif

#  ifndef cfi_adjust_cfa_offset
#   define cfi_adjust_cfa_offset(off)	.cfi_adjust_cfa_offset off
#  endif
# endif

# ifndef ENTRY
#  define ENTRY(name)	\
	.type name,  @function;	\
	.globl name;	\
	.p2align 4;	\
name:	\
	cfi_startproc
# endif

# ifndef END
#  define END(name)	\
	cfi_endproc;	\
	.size name, .-name
# endif

# define PARMS	4
# define STR	PARMS
# define RETURN	ret

# ifdef USE_AS_STRNLEN
#  define LEN	PARMS + 8
#  define CFI_PUSH(REG)	\
	cfi_adjust_cfa_offset (4);	\
	cfi_rel_offset (REG, 0)

#  define CFI_POP(REG)	\
	cfi_adjust_cfa_offset (-4);	\
	cfi_restore (REG)

#  define PUSH(REG)	pushl	REG;	CFI_PUSH (REG)
#  define POP(REG)	popl	REG;	CFI_POP (REG)
#  undef RETURN
#  define RETURN	POP (%edi); ret; CFI_PUSH(%edi);
# endif

	.text
ENTRY (STRLEN)
	mov	STR(%esp), %edx
# ifdef USE_AS_STRNLEN
	PUSH	(%edi)
	movl	LEN(%esp), %edi
	sub	$4, %edi
	jbe	L(len_less4_prolog)
# endif
#endif
	xor	%eax, %eax
	cmpb	$0, (%edx)
	jz	L(exit_tail0)
	cmpb	$0, 1(%edx)
	jz	L(exit_tail1)
	cmpb	$0, 2(%edx)
	jz	L(exit_tail2)
	cmpb	$0, 3(%edx)
	jz	L(exit_tail3)

#ifdef USE_AS_STRNLEN
	sub	$4, %edi
	jbe	L(len_less8_prolog)
#endif

	cmpb	$0, 4(%edx)
	jz	L(exit_tail4)
	cmpb	$0, 5(%edx)
	jz	L(exit_tail5)
	cmpb	$0, 6(%edx)
	jz	L(exit_tail6)
	cmpb	$0, 7(%edx)
	jz	L(exit_tail7)

#ifdef USE_AS_STRNLEN
	sub	$4, %edi
	jbe	L(len_less12_prolog)
#endif

	cmpb	$0, 8(%edx)
	jz	L(exit_tail8)
	cmpb	$0, 9(%edx)
	jz	L(exit_tail9)
	cmpb	$0, 10(%edx)
	jz	L(exit_tail10)
	cmpb	$0, 11(%edx)
	jz	L(exit_tail11)

#ifdef USE_AS_STRNLEN
	sub	$4, %edi
	jbe	L(len_less16_prolog)
#endif

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
	mov	%eax, %ecx
	and	$-16, %eax

#ifdef USE_AS_STRNLEN
	and	$15, %edx
	add	%edx, %edi
	sub	$64, %edi
	jbe	L(len_less64)
#endif

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

#ifdef USE_AS_STRNLEN
	sub	$64, %edi
	jbe	L(len_less64)
#endif

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

#ifdef USE_AS_STRNLEN
	sub	$64, %edi
	jbe	L(len_less64)
#endif

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

#ifdef USE_AS_STRNLEN
	sub	$64, %edi
	jbe	L(len_less64)
#endif

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

#ifdef USE_AS_STRNLEN
	mov	%eax, %edx
	and	$63, %edx
	add	%edx, %edi
#endif

	and	$-0x40, %eax

	.p2align 4
L(aligned_64_loop):
#ifdef USE_AS_STRNLEN
	sub	$64, %edi
	jbe	L(len_less64)
#endif
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
L(exit):
	sub	%ecx, %eax
	test	%dl, %dl
	jz	L(exit_high)

	mov	%dl, %cl
	and	$15, %cl
	jz	L(exit_8)
	test	$0x01, %dl
	jnz	L(exit_tail0)
	test	$0x02, %dl
	jnz	L(exit_tail1)
	test	$0x04, %dl
	jnz	L(exit_tail2)
	add	$3, %eax
	RETURN

	.p2align 4
L(exit_8):
	test	$0x10, %dl
	jnz	L(exit_tail4)
	test	$0x20, %dl
	jnz	L(exit_tail5)
	test	$0x40, %dl
	jnz	L(exit_tail6)
	add	$7, %eax
	RETURN

	.p2align 4
L(exit_high):
	mov	%dh, %ch
	and	$15, %ch
	jz	L(exit_high_8)
	test	$0x01, %dh
	jnz	L(exit_tail8)
	test	$0x02, %dh
	jnz	L(exit_tail9)
	test	$0x04, %dh
	jnz	L(exit_tail10)
	add	$11, %eax
	RETURN

	.p2align 4
L(exit_high_8):
	test	$0x10, %dh
	jnz	L(exit_tail12)
	test	$0x20, %dh
	jnz	L(exit_tail13)
	test	$0x40, %dh
	jnz	L(exit_tail14)
	add	$15, %eax
L(exit_tail0):
	RETURN

#ifdef USE_AS_STRNLEN

	.p2align 4
L(len_less64):
	pxor	%xmm0, %xmm0
	add	$64, %edi

	pcmpeqb	(%eax), %xmm0
	pmovmskb %xmm0, %edx
	pxor	%xmm1, %xmm1
	lea	16(%eax), %eax
	test	%edx, %edx
	jnz	L(strnlen_exit)

	sub	$16, %edi
	jbe	L(return_start_len)

	pcmpeqb	(%eax), %xmm1
	pmovmskb %xmm1, %edx
	lea	16(%eax), %eax
	test	%edx, %edx
	jnz	L(strnlen_exit)

	sub	$16, %edi
	jbe	L(return_start_len)

	pcmpeqb	(%eax), %xmm0
	pmovmskb %xmm0, %edx
	lea	16(%eax), %eax
	test	%edx, %edx
	jnz	L(strnlen_exit)

	sub	$16, %edi
	jbe	L(return_start_len)

	pcmpeqb	(%eax), %xmm1
	pmovmskb %xmm1, %edx
	lea	16(%eax), %eax
	test	%edx, %edx
	jnz	L(strnlen_exit)

#ifndef USE_AS_STRLCAT
	movl	LEN(%esp), %eax
	RETURN
#else
	jmp	L(return_start_len)
#endif

	.p2align 4
L(strnlen_exit):
	sub	%ecx, %eax

	test	%dl, %dl
	jz	L(strnlen_exit_high)
	mov	%dl, %cl
	and	$15, %cl
	jz	L(strnlen_exit_8)
	test	$0x01, %dl
	jnz	L(exit_tail0)
	test	$0x02, %dl
	jnz	L(strnlen_exit_tail1)
	test	$0x04, %dl
	jnz	L(strnlen_exit_tail2)
	sub	$4, %edi
	jb	L(return_start_len)
	lea	3(%eax), %eax
	RETURN

	.p2align 4
L(strnlen_exit_8):
	test	$0x10, %dl
	jnz	L(strnlen_exit_tail4)
	test	$0x20, %dl
	jnz	L(strnlen_exit_tail5)
	test	$0x40, %dl
	jnz	L(strnlen_exit_tail6)
	sub	$8, %edi
	jb	L(return_start_len)
	lea	7(%eax), %eax
	RETURN

	.p2align 4
L(strnlen_exit_high):
	mov	%dh, %ch
	and	$15, %ch
	jz	L(strnlen_exit_high_8)
	test	$0x01, %dh
	jnz	L(strnlen_exit_tail8)
	test	$0x02, %dh
	jnz	L(strnlen_exit_tail9)
	test	$0x04, %dh
	jnz	L(strnlen_exit_tail10)
	sub	$12, %edi
	jb	L(return_start_len)
	lea	11(%eax), %eax
	RETURN

	.p2align 4
L(strnlen_exit_high_8):
	test	$0x10, %dh
	jnz	L(strnlen_exit_tail12)
	test	$0x20, %dh
	jnz	L(strnlen_exit_tail13)
	test	$0x40, %dh
	jnz	L(strnlen_exit_tail14)
	sub	$16, %edi
	jb	L(return_start_len)
	lea	15(%eax), %eax
	RETURN

	.p2align 4
L(strnlen_exit_tail1):
	sub	$2, %edi
	jb	L(return_start_len)
	lea	1(%eax), %eax
	RETURN

	.p2align 4
L(strnlen_exit_tail2):
	sub	$3, %edi
	jb	L(return_start_len)
	lea	2(%eax), %eax
	RETURN

	.p2align 4
L(strnlen_exit_tail4):
	sub	$5, %edi
	jb	L(return_start_len)
	lea	4(%eax), %eax
	RETURN

	.p2align 4
L(strnlen_exit_tail5):
	sub	$6, %edi
	jb	L(return_start_len)
	lea	5(%eax), %eax
	RETURN

	.p2align 4
L(strnlen_exit_tail6):
	sub	$7, %edi
	jb	L(return_start_len)
	lea	6(%eax), %eax
	RETURN

	.p2align 4
L(strnlen_exit_tail8):
	sub	$9, %edi
	jb	L(return_start_len)
	lea	8(%eax), %eax
	RETURN

	.p2align 4
L(strnlen_exit_tail9):
	sub	$10, %edi
	jb	L(return_start_len)
	lea	9(%eax), %eax
	RETURN

	.p2align 4
L(strnlen_exit_tail10):
	sub	$11, %edi
	jb	L(return_start_len)
	lea	10(%eax), %eax
	RETURN

	.p2align 4
L(strnlen_exit_tail12):
	sub	$13, %edi
	jb	L(return_start_len)
	lea	12(%eax), %eax
	RETURN

	.p2align 4
L(strnlen_exit_tail13):
	sub	$14, %edi
	jb	L(return_start_len)
	lea	13(%eax), %eax
	RETURN

	.p2align 4
L(strnlen_exit_tail14):
	sub	$15, %edi
	jb	L(return_start_len)
	lea	14(%eax), %eax
	RETURN

#ifndef USE_AS_STRLCAT
	.p2align 4
L(return_start_len):
	movl	LEN(%esp), %eax
	RETURN
#endif

/* for prolog only */

	.p2align 4
L(len_less4_prolog):
	xor	%eax, %eax

	add	$4, %edi
	jz	L(exit_tail0)

	cmpb	$0, (%edx)
	jz	L(exit_tail0)
	cmp	$1, %edi
	je	L(exit_tail1)

	cmpb	$0, 1(%edx)
	jz	L(exit_tail1)
	cmp	$2, %edi
	je	L(exit_tail2)

	cmpb	$0, 2(%edx)
	jz	L(exit_tail2)
	cmp	$3, %edi
	je	L(exit_tail3)

	cmpb	$0, 3(%edx)
	jz	L(exit_tail3)
	mov	%edi, %eax
	RETURN

	.p2align 4
L(len_less8_prolog):
	add	$4, %edi

	cmpb	$0, 4(%edx)
	jz	L(exit_tail4)
	cmp	$1, %edi
	je	L(exit_tail5)

	cmpb	$0, 5(%edx)
	jz	L(exit_tail5)
	cmp	$2, %edi
	je	L(exit_tail6)

	cmpb	$0, 6(%edx)
	jz	L(exit_tail6)
	cmp	$3, %edi
	je	L(exit_tail7)

	cmpb	$0, 7(%edx)
	jz	L(exit_tail7)
	mov	$8, %eax
	RETURN


	.p2align 4
L(len_less12_prolog):
	add	$4, %edi

	cmpb	$0, 8(%edx)
	jz	L(exit_tail8)
	cmp	$1, %edi
	je	L(exit_tail9)

	cmpb	$0, 9(%edx)
	jz	L(exit_tail9)
	cmp	$2, %edi
	je	L(exit_tail10)

	cmpb	$0, 10(%edx)
	jz	L(exit_tail10)
	cmp	$3, %edi
	je	L(exit_tail11)

	cmpb	$0, 11(%edx)
	jz	L(exit_tail11)
	mov	$12, %eax
	RETURN

	.p2align 4
L(len_less16_prolog):
	add	$4, %edi

	cmpb	$0, 12(%edx)
	jz	L(exit_tail12)
	cmp	$1, %edi
	je	L(exit_tail13)

	cmpb	$0, 13(%edx)
	jz	L(exit_tail13)
	cmp	$2, %edi
	je	L(exit_tail14)

	cmpb	$0, 14(%edx)
	jz	L(exit_tail14)
	cmp	$3, %edi
	je	L(exit_tail15)

	cmpb	$0, 15(%edx)
	jz	L(exit_tail15)
	mov	$16, %eax
	RETURN
#endif

	.p2align 4
L(exit_tail1):
	add	$1, %eax
	RETURN

L(exit_tail2):
	add	$2, %eax
	RETURN

L(exit_tail3):
	add	$3, %eax
	RETURN

L(exit_tail4):
	add	$4, %eax
	RETURN

L(exit_tail5):
	add	$5, %eax
	RETURN

L(exit_tail6):
	add	$6, %eax
	RETURN

L(exit_tail7):
	add	$7, %eax
	RETURN

L(exit_tail8):
	add	$8, %eax
	RETURN

L(exit_tail9):
	add	$9, %eax
	RETURN

L(exit_tail10):
	add	$10, %eax
	RETURN

L(exit_tail11):
	add	$11, %eax
	RETURN

L(exit_tail12):
	add	$12, %eax
	RETURN

L(exit_tail13):
	add	$13, %eax
	RETURN

L(exit_tail14):
	add	$14, %eax
	RETURN

L(exit_tail15):
	add	$15, %eax
#ifndef USE_AS_STRCAT
	RETURN
END (STRLEN)
#endif
