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

#include <limits.h>
#include <pthread.h>
#include <stdbool.h>

#include <asm/ldt.h>

extern int __set_thread_area(struct user_desc*);

__LIBC_HIDDEN__ void __init_user_desc(struct user_desc* result, bool allocate, void* base_addr) {
  if (allocate) {
    // Let the kernel choose.
    result->entry_number = -1;
  } else {
    // Get the existing entry number from %gs.
    uint32_t gs;
    __asm__ __volatile__("movw %%gs, %w0" : "=q"(gs) /*output*/);
    result->entry_number = (gs & 0xffff) >> 3;
  }

  result->base_addr = (uintptr_t) base_addr;

  result->limit = PAGE_SIZE;

  result->seg_32bit = 1;
  result->contents = MODIFY_LDT_CONTENTS_DATA;
  result->read_exec_only = 0;
  result->limit_in_pages = 1;
  result->seg_not_present = 0;
  result->useable = 1;
}

int __set_tls(void* ptr) {
  struct user_desc tls_descriptor;
  __init_user_desc(&tls_descriptor, true, ptr);

  int rc = __set_thread_area(&tls_descriptor);
  if (rc != -1) {
    // Change %gs to be new GDT entry.
    uint16_t table_indicator = 0;  // GDT
    uint16_t rpl = 3;  // Requested privilege level
    uint16_t selector = (tls_descriptor.entry_number << 3) | table_indicator | rpl;
    __asm__ __volatile__("movw %w0, %%gs" : /*output*/ : "q"(selector) /*input*/ : /*clobber*/);
  }

  return rc;
}
