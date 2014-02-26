/*
 * Copyright (C) 2008 The Android Open Source Project
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <private/bionic_asm.h>

#define FUTEX_WAIT 0
#define FUTEX_WAKE 1

// int __futex_wait(volatile void* ftx, int val, const struct timespec* timeout)
ENTRY(__futex_wait)
	subu	sp,4*6
	sw	$0,20(sp)	/* val3 */
	sw	$0,16(sp)	/* addr2 */
	move	a3,a2		/* timespec */
	move	a2,a1		/* val */
	li	a1,FUTEX_WAIT	/* op */
#	move	a0,a0		/* ftx */
	li	v0,__NR_futex
	syscall
	.set noreorder
	bnez	a3, 1f		/* Check for error */
         neg	v0		/* Negate error number if it's valid */
	move	v0,$0		/* Otherwise return 0 */
1:
	.set reorder
	addu	sp,4*6
	j	ra
END(__futex_wait)

// int __futex_wake(volatile void* ftx, int count)
ENTRY(__futex_wake)
	subu	sp,4*6
	sw	$0,20(sp)	/* val3 */
	sw	$0,16(sp)	/* addr2 */
	move	a3,$0		/* timespec */
	move	a2,a1		/* val */
	li	a1,FUTEX_WAKE	/* op */
#	move	a0,a0		/* ftx */
	li	v0,__NR_futex
	syscall
	.set noreorder
	bnez	a3, 1f		/* Check for error */
         neg	v0		/* Negate error number if it's valid */
	move	v0,$0		/* Otherwise return 0 */
1:
	.set reorder
	addu	sp,4*6
	j	ra
END(__futex_wake)

// int __futex_syscall3(volatile void* ftx, int op, int count)
ENTRY(__futex_syscall3)
	subu	sp,4*6
	sw	$0,20(sp)	/* val3 */
	sw	$0,16(sp)	/* addr2 */
	move	a3,$0		/* timespec */
#	move	a2,a2		/* val */
#	li	a1,a1		/* op */
#	move	a0,a0		/* ftx */
	li	v0,__NR_futex
	syscall
	.set noreorder
	bnez	a3, 1f		/* Check for error */
         neg	v0		/* Negate error number if it's valid */
	move	v0,$0		/* Otherwise return 0 */
1:
	.set reorder
	addu	sp,4*6
	j	ra
END(__futex_syscall3)

// int __futex_syscall4(volatile void* ftx, int op, int val, const struct timespec* timeout)
ENTRY(__futex_syscall4)
	subu	sp,4*6
	sw	$0,20(sp)	/* val3 */
	sw	$0,16(sp)	/* addr2 */
#	move	a3,a3		/* timespec */
#	move	a2,a2		/* val */
#	li	a1,a1		/* op */
#	move	a0,a0		/* ftx */
	li	v0,__NR_futex
	syscall
	.set noreorder
	bnez	a3, 1f		/* Check for error */
         neg	v0		/* Negate error number if it's valid */
	move	v0,$0		/* Otherwise return 0 */
1:
	.set reorder
	addu	sp,4*6
	j	ra
END(__futex_syscall4)
