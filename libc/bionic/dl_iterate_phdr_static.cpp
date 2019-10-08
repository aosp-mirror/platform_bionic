/*
 * Copyright (C) 2006 The Android Open Source Project
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

#include <elf.h>
#include <string.h>
#include <sys/auxv.h>
#include <sys/types.h>
#include <link.h>

#include "private/bionic_elf_tls.h"
#include "private/bionic_globals.h"
#include "pthread_internal.h"

/* ld provides this to us in the default link script */
extern "C" void* __executable_start;

int dl_iterate_phdr(int (*cb)(struct dl_phdr_info* info, size_t size, void* data), void* data) {
  ElfW(Ehdr)* ehdr = reinterpret_cast<ElfW(Ehdr)*>(&__executable_start);

  if (memcmp(ehdr->e_ident, ELFMAG, SELFMAG) != 0) {
    return -1;
  }

  // Dynamic binaries get their dl_iterate_phdr from the dynamic linker, but
  // static binaries get this. We don't have a list of shared objects to
  // iterate over, since there's really only a single monolithic blob of
  // code/data, plus optionally a VDSO.

  struct dl_phdr_info exe_info;
  exe_info.dlpi_addr = 0;
  exe_info.dlpi_name = NULL;
  exe_info.dlpi_phdr = reinterpret_cast<ElfW(Phdr)*>(reinterpret_cast<uintptr_t>(ehdr) + ehdr->e_phoff);
  exe_info.dlpi_phnum = ehdr->e_phnum;
  exe_info.dlpi_adds = 0;
  exe_info.dlpi_subs = 0;

  const TlsModules& tls_modules = __libc_shared_globals()->tls_modules;
  if (tls_modules.module_count == 0) {
    exe_info.dlpi_tls_modid = 0;
    exe_info.dlpi_tls_data = nullptr;
  } else {
    const size_t kExeModuleId = 1;
    const StaticTlsLayout& layout = __libc_shared_globals()->static_tls_layout;
    const TlsModule& tls_module = tls_modules.module_table[__tls_module_id_to_idx(kExeModuleId)];
    char* static_tls = reinterpret_cast<char*>(__get_bionic_tcb()) - layout.offset_bionic_tcb();
    exe_info.dlpi_tls_modid = kExeModuleId;
    exe_info.dlpi_tls_data = static_tls + tls_module.static_offset;
  }

  // Try the executable first.
  int rc = cb(&exe_info, sizeof(exe_info), data);
  if (rc != 0) {
    return rc;
  }

  // Try the VDSO if that didn't work.
  ElfW(Ehdr)* ehdr_vdso = reinterpret_cast<ElfW(Ehdr)*>(getauxval(AT_SYSINFO_EHDR));
  if (ehdr_vdso == nullptr) {
    // There is no VDSO, so there's nowhere left to look.
    return rc;
  }

  struct dl_phdr_info vdso_info;
  vdso_info.dlpi_addr = 0;
  vdso_info.dlpi_name = NULL;
  vdso_info.dlpi_phdr = reinterpret_cast<ElfW(Phdr)*>(reinterpret_cast<char*>(ehdr_vdso) + ehdr_vdso->e_phoff);
  vdso_info.dlpi_phnum = ehdr_vdso->e_phnum;
  vdso_info.dlpi_adds = 0;
  vdso_info.dlpi_subs = 0;
  vdso_info.dlpi_tls_modid = 0;
  vdso_info.dlpi_tls_data = nullptr;
  for (size_t i = 0; i < vdso_info.dlpi_phnum; ++i) {
    if (vdso_info.dlpi_phdr[i].p_type == PT_LOAD) {
      vdso_info.dlpi_addr = (ElfW(Addr)) ehdr_vdso - vdso_info.dlpi_phdr[i].p_vaddr;
      break;
    }
  }
  return cb(&vdso_info, sizeof(vdso_info), data);
}
