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
#include <linux/sched.h>

// TODO: mips' uapi signal.h is missing #ifndef __ASSEMBLY__.
// #include <asm/signal.h>
#define SIGCHLD 18

	.text

#if (_MIPS_SIM == _ABIO32) || (_MIPS_SIM == _ABI32)
FRAMESZ		=	MKFSIZ(5,0)
#else
FRAMESZ		=	MKFSIZ(0,0)
#endif

LEAF(vfork,FRAMESZ)
#if FRAMESZ!=0
	PTR_SUBU sp, FRAMESZ
#endif
	SETUP_GP64(a5, vfork)
	LI	a0, (CLONE_VM | CLONE_VFORK | SIGCHLD)
	move	a1, $0
	move	a2, $0
	move	a3, $0
#if (_MIPS_SIM == _ABIO32) || (_MIPS_SIM == _ABI32)
	REG_S	$0, 4*REGSZ(sp)
#else
	move	a4, $0
#endif
	LI	v0, __NR_clone
	syscall
#if FRAMESZ!=0
	PTR_ADDU sp,FRAMESZ
#endif
	move	a0, v0
	bnez	a3, 1f
	RESTORE_GP64
	j	ra
1:
	LA	t9,__set_errno
	RESTORE_GP64
	j	t9
	END(vfork)
