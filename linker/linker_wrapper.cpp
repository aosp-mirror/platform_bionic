/*
 * Copyright (C) 2017 The Android Open Source Project
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

#include "private/KernelArgumentBlock.h"

extern const char linker_code_start;
extern const char original_start;
extern const char linker_entry;

/*
 * This is the entry point for the linker wrapper, which finds
 * the real linker, then bootstraps into it.
 */
extern "C" ElfW(Addr) __linker_init(void* raw_args) {
  KernelArgumentBlock args(raw_args);

  static uintptr_t linker_offset = reinterpret_cast<uintptr_t>(&linker_code_start);
  static uintptr_t linktime_addr = reinterpret_cast<uintptr_t>(&linktime_addr);
  ElfW(Addr) my_addr = reinterpret_cast<uintptr_t>(&linktime_addr) - linktime_addr;

  // Set AT_ENTRY to the proper entry point
  for (ElfW(auxv_t)* v = args.auxv; v->a_type != AT_NULL; ++v) {
    if (v->a_type == AT_BASE) {
      v->a_un.a_val = my_addr + linker_offset;
    }
    if (v->a_type == AT_ENTRY) {
      v->a_un.a_val = my_addr + reinterpret_cast<uintptr_t>(&original_start);
    }
  }

  // Return address of linker entry point -- may need to ensure that raw_args
  // was saved.
  return my_addr + linker_offset + reinterpret_cast<uintptr_t>(&linker_entry);
}
