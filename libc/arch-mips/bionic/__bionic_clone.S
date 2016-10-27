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

// pid_t __bionic_clone(int flags, void* child_stack, pid_t* parent_tid, void* tls, pid_t* child_tid, int (*fn)(void*), void* arg);
ENTRY_PRIVATE(__bionic_clone)
        .set	noreorder
        .cpload t9
        .set	reorder

	# set up child stack
	subu	a1,16
	lw	t0,20(sp)     # fn
	lw	t1,24(sp)     # arg
	sw	t0,0(a1)	# fn
	sw	t1,4(a1)	# arg

	# remainder of arguments are correct for clone system call
        li	v0,__NR_clone
        syscall

        bnez	a3,.L__error_bc

        beqz	v0,.L__thread_start_bc

        j ra

.L__thread_start_bc:
        # Clear return address in child so we don't unwind further.
        li      ra,0

        lw	a0,0(sp)	#  fn
        lw	a1,4(sp)	#  arg

	# void __start_thread(int (*func)(void*), void *arg)
        la	t9,__start_thread
        j	t9

.L__error_bc:
	move	a0,v0
	la	t9,__set_errno_internal
	j	t9
END(__bionic_clone)
