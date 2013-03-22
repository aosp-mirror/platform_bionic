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

#include <asm/unistd.h>
#include <linux/errno.h>
#include <linux/sched.h>

	.text
	.type __pthread_clone, @function
	.global __pthread_clone
	.align 4
        .ent __pthread_clone

/*
 * int __pthread_clone(void* (*fn)(void*), void *child_stack,
 *			 int flags, void *arg);
 */

__pthread_clone:
        .set	noreorder
        .cpload $t9
        .set	reorder

	# set up child stack
	subu	$a1,16
	sw	$a0,0($a1)	# fn
	sw	$a3,4($a1)	# arg
#	sw	$a1+16,8($a1)	# tls

	/*
	 * int sys_clone(int flags, void *child_stack, int *parent_tidptr,
	 *	 struct user_desc *newtls, int *child_tidptr);
	 */

	move	$a0,$a2		# flags
#	move	$a1,$a1		# child_stack
	move	$a2,$0		# parent_tidptr
	move	$a3,$0		# user_desc
	and	$a0,~(CLONE_CHILD_SETTID | CLONE_CHILD_CLEARTID)
				# make sure the kernel doesn't access child_tidptr

        li	$v0,__NR_clone
        syscall

        bnez	$a3,.L__error

        beqz	$v0,.L__thread_start

        j $ra

.L__thread_start:
        lw	$a0,0($sp)	#  fn
        lw	$a1,4($sp)	#  arg
        addu	$a2,$sp,16	#  tls

	# void __thread_entry(void* (*func)(void*), void *arg, void *tls)
        la	$t9, __thread_entry
        j	$t9

.L__error:
	move	$a0,$v0
	la	$t9,__set_errno
	j	$t9

        .end __pthread_clone


    #
    # This function is defined as:
    #
    #   pid_t  __bionic_clone( int  flags, void *child_stack,
    #                          pid_t *pid, void *tls, pid_t *ctid,
    #                          int  (*fn)(void *), void* arg );
    #
    # NOTE: This is not the same signature than the GLibc
    #       __clone function here !! Placing 'fn' and 'arg'
    #       at the end of the parameter list makes the
    #       implementation much simpler.
    #
	.text
	.type __bionic_clone, @function
	.global __bionic_clone
	.align 4
        .ent __bionic_clone
__bionic_clone:
        .set	noreorder
        .cpload $t9
        .set	reorder

	# set up child stack
	subu	$a1,16
	lw	$t0,20($sp)     # fn
	lw	$t1,24($sp)     # arg
	sw	$t0,0($a1)	# fn
	sw	$t1,4($a1)	# arg

	# remainder of arguments are correct for clone system call
        li	$v0,__NR_clone
        syscall

        bnez	$a3,.L__error_bc

        beqz	$v0,.L__thread_start_bc

        j $ra

.L__thread_start_bc:
        lw	$a0,0($sp)	#  fn
        lw	$a1,4($sp)	#  arg

	# void __bionic_clone_entry(int (*func)(void*), void *arg)
        la	$t9,__bionic_clone_entry
        j	$t9

.L__error_bc:
	move	$a0,$v0
	la	$t9,__set_errno
	j	$t9

        .end __bionic_clone
