/*
 * Copyright (C) 2024 The Android Open Source Project
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
#include <private/bionic_elf_dtv_offset.h>

#ifndef TLS_DTV_OFFSET
  #error "TLS_DTV_OFFSET not defined"
#endif

.globl __tls_get_addr

// spill a register onto the stack
.macro spill reg, idx, f=
  \f\()sd \reg, \idx*8(sp)
  .cfi_rel_offset \reg, (\idx)*8
.endm

// reload a value from the stack
.macro reload reg, idx, f=
  \f\()ld \reg, \idx*8(sp)
  .cfi_same_value \reg
.endm

.macro spill_vector_regs
  csrr a3, vlenb
  slli a3, a3, 3
  sub sp, sp, a3
  vs8r.v v0, (sp)
  sub sp, sp, a3
  vs8r.v v8, (sp)
  sub sp, sp, a3
  vs8r.v v16, (sp)
  sub sp, sp, a3
  vs8r.v v24, (sp)
.endm

.macro reload_vector_regs
  csrr a3, vlenb
  slli a3, a3, 3
  vl8r.v v24, (sp)
  add sp, sp, a3
  vl8r.v v16, (sp)
  add sp, sp, a3
  vl8r.v v8, (sp)
  add sp, sp, a3
  vl8r.v v0, (sp)
  add sp, sp, a3
.endm

// We save a total of 35 registers
.macro for_each_saved_reg op max
  \op ra, 1
  \op a1, 2
  \op a2, 3
  \op a3, 4
  \op a4, 5
  \op a5, 6
  \op a6, 7
  \op a7, 8
  \op t0, 9
  \op t1, 10
  \op t2, 11
  \op t3, 12
  \op t4, 13
  \op t5, 14
  \op t6, 15
  // save floating point regs
  \op ft0, 16, f
  \op ft1, 17, f
  \op ft2, 18, f
  \op ft3, 19, f
  \op ft4, 20, f
  \op ft5, 21, f
  \op ft6, 22, f
  \op ft7, 23, f
  \op ft8, 24, f
  \op ft9, 25, f
  \op ft10, 26, f
  \op ft11, 27, f
  \op fa0, 28, f
  \op fa1, 29, f
  \op fa2, 30, f
  \op fa3, 31, f
  \op fa4, 32, f
  \op fa5, 33, f
  \op fa6, 34, f
  \op fa7, 35, f
.endm

// These resolver functions must preserve every register except a0. They set a0
// to the offset of the TLS symbol relative to the thread pointer.

ENTRY_PRIVATE(tlsdesc_resolver_static)
  ld a0, 8(a0)
  jr t0
END(tlsdesc_resolver_static)

ENTRY_PRIVATE(tlsdesc_resolver_dynamic)
  // We only need 3 stack slots, but still require a 4th slot for alignment
  addi sp, sp, -4*8
  .cfi_def_cfa_offset 4*8
  spill a1, 1
  spill a2, 2
  spill a3, 3

  ld a2, (TLS_SLOT_DTV * 8)(tp) // a2 = &DTV
  ld a1, (a2)                   // a1 = TlsDtv::generation (DTV[0])

  ld a0, 8(a0)                  // a0 = TlsDynamicResolverArg*
  ld a3, (a0)                   // a3 = TlsDynamicResolverArg::generation

  // Fallback if TlsDtv::generation < TlsDynamicResolverArg::generation
  // since we need to call __tls_get_addr
  blt a1, a3, L(fallback)

  // We can't modify a0 yet, since tlsdesc_resolver_dynamic_slow_path requires
  // a pointer to the TlsIndex, which is the second field of the
  // TlsDynamicResolverArg. As a result, we can't modify a0 until we will no
  // longer fallback.
  ld a1, 8(a0)                  // a1 = TlsIndex::module_id
  slli a1, a1, 3                // a1 = module_id*8 -- scale the idx
  add a1, a2, a1                // a1 = &TlsDtv::modules[module_id]
  ld a1, (a1)                   // a1 = TlsDtv::modules[module_id]
  beqz a1, L(fallback)
  ld a3, 16(a0)                 // a3 = TlsIndex::offset
  add a0, a1, a3                // a0 = TlsDtv::modules[module_id] + offset
  sub a0, a0, tp                // a0 = TlsDtv::modules[module_id] + offset - tp

  .cfi_remember_state
  reload a3, 3
  reload a2, 2
  reload a1, 1
  addi sp, sp, 4*8
  .cfi_adjust_cfa_offset -4*8
  jr t0

L(fallback):
  reload a3, 3
  reload a2, 2
  reload a1, 1
  addi sp, sp, 4*8
  .cfi_adjust_cfa_offset -4*8
  j tlsdesc_resolver_dynamic_slow_path
END(tlsdesc_resolver_dynamic)

// On entry, a0 is the address of a TlsDynamicResolverArg object rather than
// the TlsDescriptor address passed to the original resolver function.
ENTRY_PRIVATE(tlsdesc_resolver_dynamic_slow_path)
  // We save a total of 35 registers, but vector spills require an alignment
  // of 16, so use an extra slot to align it correctly.
  addi sp, sp, (-8*36)
  .cfi_def_cfa_offset (8 * 36)
  for_each_saved_reg spill, 36
  spill_vector_regs

  add a0, a0, 8
  call __tls_get_addr
  addi a0, a0, (-1 * TLS_DTV_OFFSET)  // Correct the address by TLS_DTV_OFFSET
  sub a0, a0, tp

  reload_vector_regs
  for_each_saved_reg reload, 36
  addi sp, sp, 8*36
  .cfi_def_cfa_offset 0
  jr t0
END(tlsdesc_resolver_dynamic_slow_path)

// The address of an unresolved weak TLS symbol evaluates to NULL with TLSDESC.
// The value returned by this function is added to the thread pointer, so return
// a negated thread pointer to cancel it out.
ENTRY_PRIVATE(tlsdesc_resolver_unresolved_weak)
  sub a0, zero, tp
  jr t0
END(tlsdesc_resolver_unresolved_weak)
