/*
 * Copyright (C) 2023 The Android Open Source Project
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

#include "linker_auxv.h"

#include <elf.h>
#include <stdio.h>
#include <sys/auxv.h>
#include <unistd.h>

#include <async_safe/log.h>

static const char* auxv_name(int at) {
  switch (at) {
  case AT_NULL: return "AT_NULL";
  case AT_IGNORE: return "AT_IGNORE";
  case AT_EXECFD: return "AT_EXECFD";
  case AT_PHDR: return "AT_PHDR";
  case AT_PHENT: return "AT_PHENT";
  case AT_PHNUM: return "AT_PHNUM";
  case AT_PAGESZ: return "AT_PAGESZ";
  case AT_BASE: return "AT_BASE";
  case AT_FLAGS: return "AT_FLAGS";
  case AT_ENTRY: return "AT_ENTRY";
  case AT_NOTELF: return "AT_NOTELF";
  case AT_UID: return "AT_UID";
  case AT_EUID: return "AT_EUID";
  case AT_GID: return "AT_GID";
  case AT_EGID: return "AT_EGID";
  case AT_PLATFORM: return "AT_PLATFORM";
  case AT_HWCAP: return "AT_HWCAP";
  case AT_CLKTCK: return "AT_CLKTCK";
  case AT_SECURE: return "AT_SECURE";
  case AT_BASE_PLATFORM: return "AT_BASE_PLATFORM";
  case AT_RANDOM: return "AT_RANDOM";
  case AT_HWCAP2: return "AT_HWCAP2";
  case AT_RSEQ_FEATURE_SIZE: return "AT_RSEQ_FEATURE_SIZE";
  case AT_RSEQ_ALIGN: return "AT_RSEQ_ALIGN";
  case AT_EXECFN: return "AT_EXECFN";
  case AT_SYSINFO_EHDR: return "AT_SYSINFO_EHDR";
#if defined(AT_MINSIGSTKSZ)
  case AT_MINSIGSTKSZ: return "AT_MINSIGSTKSZ";
#endif
#if defined(AT_SYSINFO)
  case AT_SYSINFO: return "AT_SYSINFO";
#endif
#if defined(AT_L1I_CACHESIZE)
  case AT_L1I_CACHESIZE: return "AT_L1I_CACHESIZE";
#endif
#if defined(AT_L1I_CACHEGEOMETRY)
  case AT_L1I_CACHEGEOMETRY: return "AT_L1I_CACHEGEOMETRY";
#endif
#if defined(AT_L1D_CACHESIZE)
  case AT_L1D_CACHESIZE: return "AT_L1D_CACHESIZE";
#endif
#if defined(AT_L1D_CACHEGEOMETRY)
  case AT_L1D_CACHEGEOMETRY: return "AT_L1D_CACHEGEOMETRY";
#endif
#if defined(AT_L2_CACHESIZE)
  case AT_L2_CACHESIZE: return "AT_L2_CACHESIZE";
#endif
#if defined(AT_L2_CACHEGEOMETRY)
  case AT_L2_CACHEGEOMETRY: return "AT_L2_CACHEGEOMETRY";
#endif
#if defined(AT_L3_CACHESIZE)
  case AT_L3_CACHESIZE: return "AT_L3_CACHESIZE";
#endif
#if defined(AT_L3_CACHEGEOMETRY)
  case AT_L3_CACHEGEOMETRY: return "AT_L3_CACHEGEOMETRY";
#endif
  }
  static char name[32];
  snprintf(name, sizeof(name), "AT_??? (%d)", at);
  return name;
}

void ld_show_auxv(ElfW(auxv_t)* auxv) {
  for (ElfW(auxv_t)* v = auxv; v->a_type != AT_NULL; ++v) {
    const char* name = auxv_name(v->a_type);
    long value = v->a_un.a_val;
    switch (v->a_type) {
    case AT_SYSINFO_EHDR:
    case AT_PHDR:
    case AT_BASE:
    case AT_ENTRY:
    case AT_RANDOM:
      async_safe_format_fd(STDOUT_FILENO, "%-20s %#lx\n", name, value);
      break;
    case AT_FLAGS:
    case AT_HWCAP:
    case AT_HWCAP2:
      async_safe_format_fd(STDOUT_FILENO, "%-20s %#lb\n", name, value);
      break;
    case AT_EXECFN:
    case AT_PLATFORM:
      async_safe_format_fd(STDOUT_FILENO, "%-20s \"%s\"\n", name, reinterpret_cast<char*>(value));
      break;
    default:
      async_safe_format_fd(STDOUT_FILENO, "%-20s %ld\n", name, value);
      break;
    }
  }
}
