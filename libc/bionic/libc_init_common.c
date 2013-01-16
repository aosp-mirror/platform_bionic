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

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <elf.h>
#include <asm/page.h>
#include "pthread_internal.h"
#include "atexit.h"
#include "libc_init_common.h"

#include <bionic_tls.h>
#include <errno.h>
#include <private/bionic_auxv.h>

extern unsigned __get_sp(void);
extern pid_t gettid(void);

char*  __progname;
char** environ;

/* from asm/page.h */
unsigned int __page_size = PAGE_SIZE;
unsigned int __page_shift = PAGE_SHIFT;


int __system_properties_init(void);

/* Init TLS for the initial thread. Called by the linker _before_ libc is mapped
 * in memory. Beware: all writes to libc globals from this function will
 * apply to linker-private copies and will not be visible from libc later on.
 *
 * Note: this function creates a pthread_internal_t for the initial thread and
 * stores the pointer in TLS, but does not add it to pthread's gThreadList. This
 * has to be done later from libc itself (see __libc_init_common).
 *
 * This function also stores the elf_data argument in a specific TLS slot to be later
 * picked up by the libc constructor.
 */
void __libc_init_tls(unsigned** elf_data) {
  unsigned stack_top = (__get_sp() & ~(PAGE_SIZE - 1)) + PAGE_SIZE;
  unsigned stack_size = 128 * 1024;
  unsigned stack_bottom = stack_top - stack_size;

  pthread_attr_t thread_attr;
  pthread_attr_init(&thread_attr);
  pthread_attr_setstack(&thread_attr, (void*) stack_bottom, stack_size);

  static pthread_internal_t thread;
  _init_thread(&thread, gettid(), &thread_attr, (void*) stack_bottom, false);

  static void* tls_area[BIONIC_TLS_SLOTS];
  __init_tls(tls_area, &thread);
  tls_area[TLS_SLOT_BIONIC_PREINIT] = elf_data;
}

void __libc_init_common(uintptr_t* elf_data) {
  int argc = *elf_data;
  char** argv = (char**) (elf_data + 1);
  char** envp = argv + argc + 1;

  // Get the main thread from TLS and add it to the thread list.
  pthread_internal_t* main_thread = __get_thread();
  main_thread->allocated_on_heap = false;
  _pthread_internal_add(main_thread);

  // Set various globals.
  errno = 0;
  __progname = argv[0] ? argv[0] : "<unknown>";
  environ = envp;

  // The auxiliary vector is at the end of the environment block
  while(*envp != NULL) {
    envp++;
  }
  /* The end of the environment block is marked by two NULL pointers */
  envp++;

  __libc_auxv = (Elf32_auxv_t*) envp;

  __system_properties_init(); // Requires 'environ'.
}

/* This function will be called during normal program termination
 * to run the destructors that are listed in the .fini_array section
 * of the executable, if any.
 *
 * 'fini_array' points to a list of function addresses. The first
 * entry in the list has value -1, the last one has value 0.
 */
void __libc_fini(void* array) {
  void** fini_array = array;
  const size_t minus1 = ~(size_t)0; /* ensure proper sign extension */

  /* Sanity check - first entry must be -1 */
  if (array == NULL || (size_t)fini_array[0] != minus1) {
    return;
  }

  /* skip over it */
  fini_array += 1;

  /* Count the number of destructors. */
  int count = 0;
  while (fini_array[count] != NULL) {
    ++count;
  }

  /* Now call each destructor in reverse order. */
  while (count > 0) {
    void (*func)() = (void (*)) fini_array[--count];

    /* Sanity check, any -1 in the list is ignored */
    if ((size_t)func == minus1) {
      continue;
    }

    func();
  }

#ifndef LIBC_STATIC
  {
    extern void __libc_postfini(void) __attribute__((weak));
    if (__libc_postfini) {
      __libc_postfini();
    }
  }
#endif
}
