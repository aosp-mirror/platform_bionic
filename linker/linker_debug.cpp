/*
 * Copyright (C) 2019 The Android Open Source Project
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

#include "linker_debug.h"

#include <unistd.h>

#include <android-base/strings.h>

LinkerDebugConfig g_linker_debug_config;

void init_LD_DEBUG(const std::string& value) {
  if (value.empty()) return;
  std::vector<std::string> options = android::base::Split(value, ",");
  for (const auto& o : options) {
    if (o == "calls") g_linker_debug_config.calls = true;
    else if (o == "cfi") g_linker_debug_config.cfi = true;
    else if (o == "dynamic") g_linker_debug_config.dynamic = true;
    else if (o == "lookup") g_linker_debug_config.lookup = true;
    else if (o == "props") g_linker_debug_config.props = true;
    else if (o == "reloc") g_linker_debug_config.reloc = true;
    else if (o == "statistics") g_linker_debug_config.statistics = true;
    else if (o == "timing") g_linker_debug_config.timing = true;
    else if (o == "all") {
      g_linker_debug_config.calls = true;
      g_linker_debug_config.cfi = true;
      g_linker_debug_config.dynamic = true;
      g_linker_debug_config.lookup = true;
      g_linker_debug_config.props = true;
      g_linker_debug_config.reloc = true;
      g_linker_debug_config.statistics = true;
      g_linker_debug_config.timing = true;
    } else {
      __linker_error("$LD_DEBUG is a comma-separated list of:\n"
                     "\n"
                     "  calls       ctors/dtors/ifuncs\n"
                     "  cfi         control flow integrity messages\n"
                     "  dynamic     dynamic section processing\n"
                     "  lookup      symbol lookup\n"
                     "  props       ELF property processing\n"
                     "  reloc       relocation resolution\n"
                     "  statistics  relocation statistics\n"
                     "  timing      timing information\n"
                     "\n"
                     "or 'all' for all of the above.\n");
    }
  }
  if (g_linker_debug_config.calls || g_linker_debug_config.cfi ||
      g_linker_debug_config.dynamic || g_linker_debug_config.lookup ||
      g_linker_debug_config.props || g_linker_debug_config.reloc ||
      g_linker_debug_config.statistics || g_linker_debug_config.timing) {
    g_linker_debug_config.any = true;
  }
}

static void linker_log_va_list(int prio, const char* fmt, va_list ap) {
  va_list ap2;
  va_copy(ap2, ap);
  async_safe_format_log_va_list(prio, "linker", fmt, ap2);
  va_end(ap2);

  async_safe_format_fd_va_list(STDERR_FILENO, fmt, ap);
  write(STDERR_FILENO, "\n", 1);
}

void __linker_log(int prio, const char* fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  linker_log_va_list(prio, fmt, ap);
  va_end(ap);
}

void __linker_error(const char* fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  linker_log_va_list(ANDROID_LOG_FATAL, fmt, ap);
  va_end(ap);

  _exit(EXIT_FAILURE);
}
