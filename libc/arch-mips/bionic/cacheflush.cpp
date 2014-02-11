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

#include <unistd.h>
#include <sys/cachectl.h>

#include "private/libc_logging.h"

// Linux historically defines a cacheflush(3) routine for MIPS
// with this signature:

//   int cacheflush(char *addr, int nbytes, int cache);

// Android defines an alternate cacheflush routine which exposes the
// ARM system call interface:

//   int cacheflush (long start, long end, long flags)

// This is an attempt to maintain compatibility between the historical MIPS
// usage for software previously ported to MIPS and Android specific
// uses of cacheflush().

int cacheflush(long start, long end, long /*flags*/) {
  if (end < start) {
    // It looks like this is really a MIPS-style cacheflush call.
    static bool warned = false;
    if (!warned) {
      __libc_format_log(ANDROID_LOG_WARN, "libc", "cacheflush called with (start,len) instead of (start,end)");
      warned = true;
    }
    end += start;
  }

  // Use the GCC builtin. This will generate inline synci instructions if available,
  // or call _flush_cache(start, len, BCACHE) directly.
  __builtin___clear_cache(reinterpret_cast<char*>(start), reinterpret_cast<char*>(end));
  return 0;
}
