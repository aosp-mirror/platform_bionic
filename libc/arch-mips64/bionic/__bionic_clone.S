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
#include <linux/errno.h>
#include <linux/sched.h>

#if (_MIPS_SIM == _ABIO32) || (_MIPS_SIM == _ABI32)
FRAMESZ		=	MKFSIZ(NARGSAVE,0)
FRAME_ARG	=	0*REGSZ
FRAME_FN	=	1*REGSZ
#else
FRAMESZ		=	MKFSIZ(0,3)
FRAME_GP	=	FRAMESZ-1*REGSZ
FRAME_ARG	=	FRAMESZ-2*REGSZ
FRAME_FN	=	FRAMESZ-3*REGSZ
#endif

// pid_t __bionic_clone(int flags, void* child_stack, pid_t* parent_tid, void* tls, pid_t* child_tid, int (*fn)(void*), void* arg);
LEAF(__bionic_clone, FRAMESZ)
	PTR_SUBU sp, FRAMESZ			# allocate stack frame
	SETUP_GP64(FRAME_GP,__bionic_clone)
	SAVE_GP(FRAME_GP)

	# set up child stack
	PTR_SUBU a1,FRAMESZ
#if (_MIPS_SIM == _ABIO32) || (_MIPS_SIM == _ABI32)
	PTR_L	t0,FRAMESZ+5*REGSZ(sp)	# fn
	PRL_L	t1,FRAMESZ+6*REGSZ(sp)	# arg
	PTR_S	t0,FRAME_FN(a1)		# fn
	PTR_S	t1,FRAME_ARG(a1)	# arg
#else
	PTR_L	t0,FRAME_GP(sp)		# copy gp to child stack
	PTR_S	t0,FRAME_GP(a1)
	PTR_S	a5,FRAME_FN(a1)		# fn
	PTR_S	a6,FRAME_ARG(a1)	# arg
# endif

	# remainder of arguments are correct for clone system call
	LI	v0,__NR_clone
	syscall

	move    a0,v0
	bnez	a3,.L__error_bc

	beqz	v0,.L__thread_start_bc

	RESTORE_GP64
	PTR_ADDU sp,FRAMESZ
	j	ra

.L__thread_start_bc:
	# void __bionic_clone_entry(int (*func)(void*), void *arg)
	PTR_L	a0,FRAME_FN(sp)		#  fn
	PTR_L	a1,FRAME_ARG(sp)	#  arg
	LA	t9,__bionic_clone_entry
	RESTORE_GP64
	/*
	 * For O32 etc the child stack must have space for a0..a3 to be stored
	 * For N64 etc, the child stack can be restored to the original value
	 */
#if !((_MIPS_SIM == _ABIO32) || (_MIPS_SIM == _ABI32))
	PTR_ADDU sp,FRAMESZ
#endif
	j	t9

.L__error_bc:
	LA	t9,__set_errno
	RESTORE_GP64
	PTR_ADDU sp,FRAMESZ
	j	t9
	END(__bionic_clone)
.hidden __bionic_clone
