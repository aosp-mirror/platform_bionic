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

#pragma once

#include <stdarg.h>
#include <unistd.h>

#include <string>

#include <async_safe/log.h>
#include <async_safe/CHECK.h>

struct LinkerDebugConfig {
  // Set automatically if any of the more specific options are set.
  bool any;

  // Messages relating to calling ctors/dtors/ifuncs.
  bool calls;
  // Messages relating to CFI.
  bool cfi;
  // Messages relating to the dynamic section.
  bool dynamic;
  // Messages relating to symbol lookup.
  bool lookup;
  // Messages relating to relocation processing.
  bool reloc;
  // Messages relating to ELF properties.
  bool props;
  // TODO: "config" and "zip" seem likely to want to be separate?

  bool timing;
  bool statistics;
};

extern LinkerDebugConfig g_linker_debug_config;

__LIBC_HIDDEN__ void init_LD_DEBUG(const std::string& value);
__LIBC_HIDDEN__ void __linker_log(int prio, const char* fmt, ...) __printflike(2, 3);
__LIBC_HIDDEN__ void __linker_error(const char* fmt, ...) __printflike(1, 2);

#define LD_DEBUG(what, x...) \
  do { \
    if (g_linker_debug_config.what) { \
      __linker_log(ANDROID_LOG_INFO, x); \
    } \
  } while (false)
