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

#include <platform/bionic/tls_defines.h>
#include <private/bionic_asm.h>

// This custom code preserves the return address across the system call.

ENTRY(vfork)
__BIONIC_WEAK_ASM_FOR_NATIVE_BRIDGE(vfork)
  popl    %ecx  // Grab the return address.
  .cfi_adjust_cfa_offset 4
  .cfi_rel_offset ecx, 0

  // Set cached_pid_ to 0, vforked_ to 1, and stash the previous value.
  movl    %gs:0, %eax
  movl    (TLS_SLOT_THREAD_ID * 4)(%eax), %eax
  movl    12(%eax), %edx
  movl    $0x80000000, 12(%eax)

  movl    $__NR_vfork, %eax
  int     $0x80

  test    %eax, %eax
  jz      1f

  // rc != 0: restore the previous cached_pid_/vforked_ values.
  pushl   %ecx
  movl    %gs:0, %ecx
  movl    (TLS_SLOT_THREAD_ID * 4)(%ecx), %ecx
  movl    %edx, 12(%ecx)
  popl    %ecx

  cmpl    $-MAX_ERRNO, %eax
  jb      1f
  negl    %eax
  pushl   %eax
  call    __set_errno_internal
1:
  jmp     *%ecx  // Jump to the stored return address.
END(vfork)
