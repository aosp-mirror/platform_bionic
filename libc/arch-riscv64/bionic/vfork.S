/*
 * Copyright (C) 2020 The Android Open Source Project
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

#include <platform/bionic/tls_defines.h>
#include <private/bionic_asm.h>
#include <asm/signal.h>
#include <linux/sched.h>

ENTRY(vfork)
  // t0 = __get_tls()[TLS_SLOT_THREAD_ID]
  mv      t0, tp
  ld      t0, TLS_SLOT_THREAD_ID * 8(t0)

  // Set cached_pid_ to 0, vforked_ to 1, and stash the previous value.
  li      t1, 0x80000000
  lw      t2, 20(t0)
  sw      t1, 20(t0)

  li      a0, (CLONE_VM | CLONE_VFORK | SIGCHLD)
  li      a1, 0 //uses a duplicate of the parent's stack
  li      a2, 0
  li      a3, 0
  li      a4, 0

  li      a7, __NR_clone
  ecall

  // if (rc == 0) we're the child, and finished...
  beqz    a0, L(success)

  // else if (rc != 0): reset cached_pid_ and vforked_...
  sw      t2, 20(t0)
  // ...and work out whether we succeeded or failed.
  bltz    a0, L(failure)
L(success):
  ret

L(failure):
  neg     a0, a0
  tail    __set_errno_internal
END(vfork)
