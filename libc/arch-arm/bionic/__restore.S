/*
 * Copyright (C) 2014 The Android Open Source Project
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

// gdb is smart enough to unwind through signal frames with just the regular
// CFI information but libgcc and libunwind both need extra help. We do this
// by using .fnstart/.fnend and inserting a nop before both __restore and
// __restore_rt (but covered by the .fnstart/.fnend) so that although they're
// not inside the functions from objdump's point of view, an unwinder that
// blindly looks at the previous instruction (but is then smart enough to check
// the DWARF information to find out where it landed) gets the right answer.

// We need to place .fnstart ourselves (but we may as well keep the free .fnend).
#undef __bionic_asm_custom_entry
#define __bionic_asm_custom_entry(f)

  .fnstart
  .save {r0-r15}
  .pad #32
  nop
ENTRY_PRIVATE(__restore)
  // This function must have exactly this instruction sequence.
  mov r7, #__NR_sigreturn
  swi #0
END(__restore)

  .fnstart
  .save {r0-r15}
  .pad #160
  nop
ENTRY_PRIVATE(__restore_rt)
  // This function must have exactly this instruction sequence.
  mov r7, #__NR_rt_sigreturn
  swi #0
END(__restore_rt)
