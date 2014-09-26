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

// Prototype: void *memcpy (void *dst, const void *src, size_t count).

#include <private/bionic_asm.h>
#include <private/libc_events.h>

ENTRY(__memcpy_chk)
  cmp   x2, x3
  b.hi  __memcpy_chk_fail

  // Fall through to memcpy...
END(__memcpy_chk)

ENTRY(memcpy)
  #include "memcpy_base.S"
END(memcpy)

ENTRY_PRIVATE(__memcpy_chk_fail)
  // Preserve for accurate backtrace.
  stp  x29, x30, [sp, -16]!
  .cfi_def_cfa_offset 16
  .cfi_rel_offset x29, 0
  .cfi_rel_offset x30, 8

  adrp  x0, error_string
  add   x0, x0, :lo12:error_string
  ldr   x1, error_code
  bl    __fortify_chk_fail
error_code:
  .word   BIONIC_EVENT_MEMCPY_BUFFER_OVERFLOW
END(__memcpy_chk_fail)

  .data
  .align 2
error_string:
  .string "memcpy: prevented write past end of buffer"
