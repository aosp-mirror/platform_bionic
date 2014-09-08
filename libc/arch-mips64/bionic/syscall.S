/*
 * Copyright (C) 2013 The Android Open Source Project
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

#if (_MIPS_SIM == _ABIO32) || (_MIPS_SIM == _ABI32)
FRAMESZ		=	MKFSIZ(6,0)
#else
FRAMESZ		=	MKFSIZ(0,1)
FRAME_GP	=	FRAMESZ-1*REGSZ
#endif

LEAF(syscall,FRAMESZ)
	PTR_SUBU sp, FRAMESZ	# allocate stack frame
	SETUP_GP64(FRAME_GP,syscall)
	SAVE_GP(FRAME_GP)
	move	v0, a0		# syscall number to v0
	move	a0, a1		# shift args down
	move	a1, a2
	move	a2, a3
#if (_MIPS_SIM == _ABIO32) || (_MIPS_SIM == _ABI32)
	REG_L	a3, FRAMESZ+4*REGSZ(sp)
	REG_L	t0, FRAMESZ+5*REGSZ(sp)
	REG_L	t1, FRAMESZ+6*REGSZ(sp)
	REG_S	t0, 4*REGSZ(sp)
	REG_S	t1, 5*REGSZ(sp)
#else
	move	a3, a4
	move	a4, a5
	move	a5, a6
#endif
	syscall
	move	a0, v0
	bnez	a3, 1f
	RESTORE_GP64
	PTR_ADDU sp, FRAMESZ
	j	ra
1:
	LA	t9,__set_errno_internal
	RESTORE_GP64
	PTR_ADDU sp, FRAMESZ
	j	t9
	END(syscall)
