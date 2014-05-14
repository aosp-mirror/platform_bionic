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
#include <inttypes.h>
#include <unistd.h>
#include <unwind.h>
#include <sys/types.h>

#include "debug_mapinfo.h"
#include "private/libc_logging.h"

#if defined(__LP64__)
#define PAD_PTR "016" PRIxPTR
#else
#define PAD_PTR "08" PRIxPTR
#endif

/* depends how the system includes define this */
#ifdef HAVE_UNWIND_CONTEXT_STRUCT
typedef struct _Unwind_Context __unwind_context;
#else
typedef _Unwind_Context __unwind_context;
#endif

static mapinfo_t* g_map_info = NULL;
static void* g_demangler;
typedef char* (*DemanglerFn)(const char*, char*, size_t*, int*);
static DemanglerFn g_demangler_fn = NULL;

__LIBC_HIDDEN__ void backtrace_startup() {
  g_map_info = mapinfo_create(getpid());
  g_demangler = dlopen("libgccdemangle.so", RTLD_NOW);
  if (g_demangler != NULL) {
    void* sym = dlsym(g_demangler, "__cxa_demangle");
    g_demangler_fn = reinterpret_cast<DemanglerFn>(sym);
  }
}

__LIBC_HIDDEN__ void backtrace_shutdown() {
  mapinfo_destroy(g_map_info);
  dlclose(g_demangler);
}

static char* demangle(const char* symbol) {
  if (g_demangler_fn == NULL) {
    return NULL;
  }
  return (*g_demangler_fn)(symbol, NULL, NULL, NULL);
}

struct stack_crawl_state_t {
  uintptr_t* frames;
  size_t frame_count;
  size_t max_depth;
  bool have_skipped_self;

  stack_crawl_state_t(uintptr_t* frames, size_t max_depth)
      : frames(frames), frame_count(0), max_depth(max_depth), have_skipped_self(false) {
  }
};

static _Unwind_Reason_Code trace_function(__unwind_context* context, void* arg) {
  stack_crawl_state_t* state = static_cast<stack_crawl_state_t*>(arg);

  uintptr_t ip = _Unwind_GetIP(context);

  // The first stack frame is get_backtrace itself. Skip it.
  if (ip != 0 && !state->have_skipped_self) {
    state->have_skipped_self = true;
    return _URC_NO_REASON;
  }

#ifdef __arm__
  /*
   * The instruction pointer is pointing at the instruction after the bl(x), and
   * the _Unwind_Backtrace routine already masks the Thumb mode indicator (LSB
   * in PC). So we need to do a quick check here to find out if the previous
   * instruction is a Thumb-mode BLX(2). If so subtract 2 otherwise 4 from PC.
   */
  if (ip != 0) {
    short* ptr = reinterpret_cast<short*>(ip);
    // Thumb BLX(2)
    if ((*(ptr-1) & 0xff80) == 0x4780) {
      ip -= 2;
    } else {
      ip -= 4;
    }
  }
#endif

  state->frames[state->frame_count++] = ip;
  return (state->frame_count >= state->max_depth) ? _URC_END_OF_STACK : _URC_NO_REASON;
}

__LIBC_HIDDEN__ int get_backtrace(uintptr_t* frames, size_t max_depth) {
  stack_crawl_state_t state(frames, max_depth);
  _Unwind_Backtrace(trace_function, &state);
  return state.frame_count;
}

__LIBC_HIDDEN__ void log_backtrace(uintptr_t* frames, size_t frame_count) {
  uintptr_t self_bt[16];
  if (frames == NULL) {
    frame_count = get_backtrace(self_bt, 16);
    frames = self_bt;
  }

  __libc_format_log(ANDROID_LOG_ERROR, "libc",
                    "*** *** *** *** *** *** *** *** *** *** *** *** *** *** *** ***\n");

  for (size_t i = 0 ; i < frame_count; ++i) {
    uintptr_t offset = 0;
    const char* symbol = NULL;

    Dl_info info;
    if (dladdr((void*) frames[i], &info) != 0) {
      offset = reinterpret_cast<uintptr_t>(info.dli_saddr);
      symbol = info.dli_sname;
    }

    uintptr_t rel_pc;
    const mapinfo_t* mi = (g_map_info != NULL) ? mapinfo_find(g_map_info, frames[i], &rel_pc) : NULL;
    const char* soname = (mi != NULL) ? mi->name : info.dli_fname;
    if (soname == NULL) {
      soname = "<unknown>";
    }
    if (symbol != NULL) {
      // TODO: we might need a flag to say whether it's safe to allocate (demangling allocates).
      char* demangled_symbol = demangle(symbol);
      const char* best_name = (demangled_symbol != NULL) ? demangled_symbol : symbol;

      __libc_format_log(ANDROID_LOG_ERROR, "libc",
                        "          #%02zd  pc %" PAD_PTR "  %s (%s+%" PRIuPTR ")",
                        i, rel_pc, soname, best_name, frames[i] - offset);

      free(demangled_symbol);
    } else {
      __libc_format_log(ANDROID_LOG_ERROR, "libc",
                        "          #%02zd  pc %" PAD_PTR "  %s",
                        i, rel_pc, soname);
    }
  }
}
