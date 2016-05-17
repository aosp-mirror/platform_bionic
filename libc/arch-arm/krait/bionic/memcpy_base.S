/***************************************************************************
 Copyright (c) 2009-2013 The Linux Foundation. All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:
     * Redistributions of source code must retain the above copyright
       notice, this list of conditions and the following disclaimer.
     * Redistributions in binary form must reproduce the above copyright
       notice, this list of conditions and the following disclaimer in the
       documentation and/or other materials provided with the distribution.
     * Neither the name of The Linux Foundation nor the names of its contributors may
       be used to endorse or promote products derived from this software
       without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
  ***************************************************************************/

/* Assumes neon instructions and a cache line size of 64 bytes. */

#include <machine/asm.h>

#define PLDOFFS	(10)
#define PLDTHRESH (PLDOFFS)
#define BBTHRESH (4096/64)
#define PLDSIZE (64)

#if (PLDOFFS < 1)
#error Routine does not support offsets less than 1
#endif

#if (PLDTHRESH < PLDOFFS)
#error PLD threshold must be greater than or equal to the PLD offset
#endif

	.text
	.fpu    neon

.L_memcpy_base:
	cmp	r2, #4
	blt	.L_neon_lt4
	cmp	r2, #16
	blt	.L_neon_lt16
	cmp	r2, #32
	blt	.L_neon_16
	cmp	r2, #64
	blt	.L_neon_copy_32_a

	mov	r12, r2, lsr #6
	cmp	r12, #PLDTHRESH
	ble	.L_neon_copy_64_loop_nopld

	push	{r9, r10}
	.cfi_adjust_cfa_offset 8
	.cfi_rel_offset r9, 0
	.cfi_rel_offset r10, 4

	cmp	r12, #BBTHRESH
	ble	.L_neon_prime_pump

	add	lr, r0, #0x400
	add	r9, r1, #(PLDOFFS*PLDSIZE)
	sub	lr, lr, r9
	lsl	lr, lr, #21
	lsr	lr, lr, #21
	add	lr, lr, #(PLDOFFS*PLDSIZE)
	cmp	r12, lr, lsr #6
	ble	.L_neon_prime_pump

	itt	gt
	movgt	r9, #(PLDOFFS)
	rsbsgt	r9, r9, lr, lsr #6
	ble	.L_neon_prime_pump

	add	r10, r1, lr
	bic	r10, #0x3F

	sub	r12, r12, lr, lsr #6

	cmp	r9, r12
	itee	le
	suble	r12, r12, r9
	movgt	r9, r12
	movgt	r12, #0

	pld	[r1, #((PLDOFFS-1)*PLDSIZE)]
.L_neon_copy_64_loop_outer_doublepld:
	pld	[r1, #((PLDOFFS)*PLDSIZE)]
	vld1.32	{q0, q1}, [r1]!
	vld1.32	{q2, q3}, [r1]!
	ldr	r3, [r10]
	subs	r9, r9, #1
	vst1.32	{q0, q1}, [r0]!
	vst1.32	{q2, q3}, [r0]!
	add	r10, #64
	bne	.L_neon_copy_64_loop_outer_doublepld
	cmp	r12, #0
	beq	.L_neon_pop_before_nopld

	cmp	r12, #(512*1024/64)
	blt	.L_neon_copy_64_loop_outer

.L_neon_copy_64_loop_ddr:
	vld1.32	{q0, q1}, [r1]!
	vld1.32	{q2, q3}, [r1]!
	pld	[r10]
	subs	r12, r12, #1
	vst1.32	{q0, q1}, [r0]!
	vst1.32	{q2, q3}, [r0]!
	add	r10, #64
	bne	.L_neon_copy_64_loop_ddr
	b	.L_neon_pop_before_nopld

.L_neon_prime_pump:
	mov	lr, #(PLDOFFS*PLDSIZE)
	add	r10, r1, #(PLDOFFS*PLDSIZE)
	bic	r10, #0x3F
	sub	r12, r12, #PLDOFFS
	ldr	r3, [r10, #(-1*PLDSIZE)]

.L_neon_copy_64_loop_outer:
	vld1.32	{q0, q1}, [r1]!
	vld1.32	{q2, q3}, [r1]!
	ldr	r3, [r10]
	subs	r12, r12, #1
	vst1.32	{q0, q1}, [r0]!
	vst1.32	{q2, q3}, [r0]!
	add	r10, #64
	bne	.L_neon_copy_64_loop_outer

.L_neon_pop_before_nopld:
	mov	r12, lr, lsr #6
	pop	{r9, r10}
	.cfi_adjust_cfa_offset -8
	.cfi_restore r9
	.cfi_restore r10

.L_neon_copy_64_loop_nopld:
	vld1.32	{q8, q9}, [r1]!
	vld1.32	{q10, q11}, [r1]!
	subs	r12, r12, #1
	vst1.32	{q8, q9}, [r0]!
	vst1.32	{q10, q11}, [r0]!
	bne	.L_neon_copy_64_loop_nopld
	ands	r2, r2, #0x3f
	beq	.L_neon_exit

.L_neon_copy_32_a:
	movs	r3, r2, lsl #27
	bcc	.L_neon_16
	vld1.32	{q0,q1}, [r1]!
	vst1.32	{q0,q1}, [r0]!

.L_neon_16:
	bpl	.L_neon_lt16
	vld1.32	{q8}, [r1]!
	vst1.32	{q8}, [r0]!
	ands	r2, r2, #0x0f
	beq	.L_neon_exit

.L_neon_lt16:
	movs	r3, r2, lsl #29
	bcc	1f
	vld1.8	{d0}, [r1]!
	vst1.8	{d0}, [r0]!
1:
	bge	.L_neon_lt4
	vld4.8	{d0[0], d1[0], d2[0], d3[0]}, [r1]!
	vst4.8	{d0[0], d1[0], d2[0], d3[0]}, [r0]!

.L_neon_lt4:
	movs	r2, r2, lsl #31
	itt	cs
	ldrhcs	r3, [r1], #2
	strhcs	r3, [r0], #2
	itt	mi
	ldrbmi	r3, [r1]
	strbmi	r3, [r0]

.L_neon_exit:
	pop	{r0, pc}
