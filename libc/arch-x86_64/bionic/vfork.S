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

#include <platform/bionic/tls_defines.h>
#include <private/bionic_asm.h>

// This custom code preserves the return address across the system call.

ENTRY(vfork)
__BIONIC_WEAK_ASM_FOR_NATIVE_BRIDGE(vfork)
  popq    %rdi  // Grab the return address.

  // Set cached_pid_ to 0, vforked_ to 1, and stash the previous value.
  mov     %fs:0, %r8
  mov     (TLS_SLOT_THREAD_ID * 8)(%r8), %r8
  movl    20(%r8), %r9d
  movl    $0x80000000, 20(%r8)

  movl    $__NR_vfork, %eax
  syscall
  pushq   %rdi  // Restore the return address.

  test    %eax, %eax
  jz      1f

  // rc != 0: restore the previous cached_pid_/vforked_ values.
  movl    %r9d, 20(%r8)

  cmpq    $-MAX_ERRNO, %rax
  jb      1f
  negl    %eax
  movl    %eax, %edi
  call    __set_errno_internal
1:
  ret
END(vfork)
