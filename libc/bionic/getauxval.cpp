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

#include <stddef.h>
#include <sys/cdefs.h>
#include <sys/auxv.h>
#include <private/bionic_auxv.h>
#include <private/bionic_globals.h>
#include <private/bionic_ifuncs.h>
#include <elf.h>
#include <errno.h>

// This function needs to be safe to call before TLS is set up, so it can't
// access errno or the stack protector.
__LIBC_HIDDEN__ unsigned long __bionic_getauxval(unsigned long type, bool& exists) {
  for (ElfW(auxv_t)* v = __libc_shared_globals()->auxv; v->a_type != AT_NULL; ++v) {
    if (v->a_type == type) {
      exists = true;
      return v->a_un.a_val;
    }
  }
  exists = false;
  return 0;
}

extern "C" unsigned long getauxval(unsigned long type) {
  bool exists;
  unsigned long result = __bionic_getauxval(type, exists);
  if (!exists) errno = ENOENT;
  return result;
}
