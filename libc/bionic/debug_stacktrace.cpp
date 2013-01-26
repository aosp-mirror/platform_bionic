/*
 * Copyright (C) 2012 The Android Open Source Project
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

#include "debug_stacktrace.h"

#include <dlfcn.h>
#include <unistd.h>
#include <unwind.h>
#include <sys/types.h>

#include "debug_format.h"
#include "debug_mapinfo.h"
#include "logd.h"

/* depends how the system includes define this */
#ifdef HAVE_UNWIND_CONTEXT_STRUCT
typedef struct _Unwind_Context __unwind_context;
#else
typedef _Unwind_Context __unwind_context;
#endif

static _Unwind_Reason_Code trace_function(__unwind_context* context, void* arg) {
  stack_crawl_state_t* state = static_cast<stack_crawl_state_t*>(arg);
  if (state->count) {
    uintptr_t ip = _Unwind_GetIP(context);
    if (ip) {
      state->addrs[0] = ip;
      state->addrs++;
      state->count--;
      return _URC_NO_REASON;
    }
  }
  // If we run out of space to record the address or 0 has been seen, stop
  // unwinding the stack.
  return _URC_END_OF_STACK;
}

__LIBC_HIDDEN__ int get_backtrace(uintptr_t* addrs, size_t max_entries) {
  stack_crawl_state_t state;
  state.count = max_entries;
  state.addrs = addrs;
  _Unwind_Backtrace(trace_function, &state);
  return max_entries - state.count;
}

__LIBC_HIDDEN__ void log_backtrace(mapinfo_t* map_info, uintptr_t* addrs, size_t c) {
  uintptr_t self_bt[16];
  if (addrs == NULL) {
    c = get_backtrace(self_bt, 16);
    addrs = self_bt;
  }

  __libc_format_log(ANDROID_LOG_ERROR, "libc",
                    "*** *** *** *** *** *** *** *** *** *** *** *** *** *** *** ***\n");

  int index = 0;
  for (size_t i = 0 ; i < c; ++i) {
    void* offset = 0;
    const char* symbol = NULL;

    Dl_info info;
    if (dladdr((void*) addrs[i], &info) != 0) {
      offset = info.dli_saddr;
      symbol = info.dli_sname;
    }

    // This test is a bit sketchy, but it allows us to skip the
    // stack trace entries due to this debugging code. it works
    // because those don't have a symbol (they're not exported).
    if (symbol != NULL || index > 0) {
      unsigned int rel_pc;
      const mapinfo_t* mi = mapinfo_find(map_info, addrs[i], &rel_pc);
      const char* soname = mi ? mi->name : info.dli_fname;
      if (soname == NULL) {
        soname = "unknown";
      }
      if (symbol) {
        __libc_format_log(ANDROID_LOG_ERROR, "libc", "          #%02d  pc %08x  %s (%s+0x%x)",
                          index, rel_pc, soname, symbol, addrs[i] - (uintptr_t) offset);
      } else {
        __libc_format_log(ANDROID_LOG_ERROR, "libc", "          #%02d  pc %08x  %s",
                          index, rel_pc, soname);
      }
      ++index;
    }
  }
}
