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

#include <errno.h>

// This function is called from our assembler syscall stubs.
// C/C++ code should just assign 'errno' instead.

// The return type is 'long' because we use the same routine in calls
// that return an int as in ones that return a ssize_t. On a 32-bit
// system these are the same size, but on a 64-bit system they're not.
// 'long' gives us 32-bit on 32-bit systems, 64-bit on 64-bit systems.

// __set_errno was mistakenly exposed in <errno.h> in the 32-bit NDK.
// We need the extra level of indirection so that the .hidden directives
// in the system call stubs don't cause __set_errno to be hidden, breaking
// old NDK apps.

// This one is for internal use only and used by both LP32 and LP64 assembler.
extern "C" __LIBC_HIDDEN__ long __set_errno_internal(int n) {
  errno = n;
  return -1;
}

// This one exists for the LP32 NDK and is not present at all in LP64.
#if !defined(__LP64__)
extern "C" long __set_errno(int n) {
  return __set_errno_internal(n);
}
#endif
