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

#include <asm/unistd.h>
#include <machine/asm.h>
/*
 * void _exit_with_stack_teardown(void *stackBase, int stackSize, int *retCode)
 */

ENTRY(_exit_with_stack_teardown)
    /* we can trash %rbx here since this call should never return. */
    /* We can also take advantage of the fact that the linux syscall trap
     * handler saves all the registers, so we don't need a stack to keep
     * the retCode argument for exit while doing the munmap */

    /* TODO(dmtriyz): No one expects this code to return, so even if
     * munmap fails, we have to exit. This should probably be fixed, but
     * since ARM side does the same thing, leave it as is.
     */

    /* args passed through registers */
    mov     $__NR_munmap, %eax        /* shouldn't change %rdx (retCode) */
    syscall
    mov     %rdx, %rdi                /* retrieve the retCode */
    mov     $__NR_exit, %eax
    syscall
    /* exit does not return */
    /* can't have a ret here since we no longer have a usable stack. Seems
     * that presently, 'hlt' will cause the program to segfault.. but this
     * should never happen :) */
    hlt
