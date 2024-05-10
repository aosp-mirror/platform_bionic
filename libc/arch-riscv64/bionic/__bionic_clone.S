/*
 * Copyright (C) 2022 The Android Open Source Project
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

// pid_t __bionic_clone(int flags, void* child_stack, pid_t* parent_tid, void* tls, pid_t* child_tid, int (*fn)(void*), void* arg);

ENTRY_PRIVATE(__bionic_clone)
  # Push 'fn' and 'arg' onto the child stack.
  addi a1, a1, -16
  sd a5, 0(a1)
  sd a6, 8(a1)

  # Make the system call.
  li a7, __NR_clone
  ecall

  # Are we the child?
  beqz a0, L(child)

  # Did the clone(2) fail?
  bltz a0, L(failure)
  # Nope, we're the parent, and our work here is done.
  ret

L(failure):
  # Set errno if something went wrong.
  neg a0, a0
  tail __set_errno_internal

L(child):
  # We're in the child now. Set the end of the frame record chain.
  li fp, 0
  # Setting ra to 0 will make the unwinder stop at __start_thread.
  li ra, 0
  # Call __start_thread with the 'fn' and 'arg' we stored on the child stack.
  ld a0, 0(sp)
  ld a1, 8(sp)
  addi sp, sp, 16
  tail __start_thread
END(__bionic_clone)
