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

#if (_MIPS_SIM == _ABIO32) || (_MIPS_SIM == _ABI32)
FRAMESZ		=	MKFSIZ(NARGSAVE+2,0)
FRAME_A4	=	4*REGSZ
FRAME_A5	=	5*REGSZ
#else
FRAMESZ		=	0
#endif

// int __futex_wait(volatile void* ftx, int val, const struct timespec* timeout)
LEAF(__futex_wait, FRAMESZ)
#if (_MIPS_SIM == _ABIO32) || (_MIPS_SIM == _ABI32)
	PTR_SUBU sp, FRAMESZ
	REG_S	$0,FRAME_A5(sp)	/* val3 */
	REG_S	$0,FRAME_A4(sp)	/* addr2 */
#else
	move	a5,$0		/* val3 */
	move	a4,$0		/* addr2 */
#endif
	move	a3,a2		/* timespec */
	move	a2,a1		/* val */
	LI	a1,FUTEX_WAIT	/* op */
#	move	a0,a0		/* ftx */
	LI	v0,__NR_futex
	syscall
	neg	v0		/* Negate errno */
	bnez	a3,1f		/* Check for error */
	move	v0,$0		/* Return 0 if no error */
1:
#if (_MIPS_SIM == _ABIO32) || (_MIPS_SIM == _ABI32)
	PTR_ADDU sp,FRAMESZ
#endif
	j	ra
	END(__futex_wait)

// int __futex_wake(volatile void* ftx, int count)
LEAF(__futex_wake,FRAMESZ)
#if (_MIPS_SIM == _ABIO32) || (_MIPS_SIM == _ABI32)
	PTR_SUBU sp, FRAMESZ
	REG_S	$0,FRAME_A5(sp)	/* val3 */
	REG_S	$0,FRAME_A4(sp)	/* addr2 */
#else
	move	a5,$0		/* val3 */
	move	a4,$0		/* addr2 */
#endif
	move	a3,$0		/* timespec */
	move	a2,a1		/* val */
	LI	a1,FUTEX_WAKE	/* op */
#	move	a0,a0		/* ftx */
	LI	v0,__NR_futex
	syscall
	neg	v0		/* Negate errno */
	bnez	a3,1f		/* Check for error */
	move	v0,$0		/* Return 0 if no error */
1:
#if (_MIPS_SIM == _ABIO32) || (_MIPS_SIM == _ABI32)
	PTR_ADDU sp,FRAMESZ
#endif
	j	ra
	END(__futex_wake)

// int __futex_syscall3(volatile void* ftx, int op, int count)
LEAF(__futex_syscall3,FRAMESZ)
#if (_MIPS_SIM == _ABIO32) || (_MIPS_SIM == _ABI32)
	PTR_SUBU sp, FRAMESZ
	REG_S	$0,FRAME_A5(sp)	/* val3 */
	REG_S	$0,FRAME_A4(sp)	/* addr2 */
#else
	move	a5,$0		/* val3 */
	move	a4,$0		/* addr2 */
#endif
	move	a3,$0		/* timespec */
#	move	a2,a2		/* val */
#	move	a1,a1		/* op */
#	move	a0,a0		/* ftx */
	LI	v0,__NR_futex
	syscall
	neg	v0		/* Negate errno */
	bnez	a3,1f		/* Check for error */
	move	v0,$0		/* Return 0 if no error */
1:
#if (_MIPS_SIM == _ABIO32) || (_MIPS_SIM == _ABI32)
	PTR_ADDU sp,FRAMESZ
#endif
	j	ra
	END(__futex_syscall3)

// int __futex_syscall4(volatile void* ftx, int op, int val, const struct timespec* timeout)
LEAF(__futex_syscall4,FRAMESZ)
#if (_MIPS_SIM == _ABIO32) || (_MIPS_SIM == _ABI32)
	PTR_SUBU sp, FRAMESZ
	REG_S	$0,FRAME_A5(sp)	/* val3 */
	REG_S	$0,FRAME_A4(sp)	/* addr2 */
#else
	move	a5,$0		/* val3 */
	move	a4,$0		/* addr2 */
#endif
#	move	a3,a3		/* timespec */
#	move	a2,a2		/* val */
#	move	a1,a1		/* op */
#	move	a0,a0		/* ftx */
	LI	v0,__NR_futex
	syscall
	neg	v0		/* Negate errno */
	bnez	a3,1f		/* Check for error */
	move	v0,$0		/* Return 0 if no error */
1:
#if (_MIPS_SIM == _ABIO32) || (_MIPS_SIM == _ABI32)
	PTR_ADDU sp,FRAMESZ
#endif
	j	ra
	END(__futex_syscall4)
