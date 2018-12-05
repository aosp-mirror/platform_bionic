/*
 * Copyright (C) 2014 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "private/bionic_globals.h"
#include "private/bionic_vdso.h"

#include <limits.h>
#include <link.h>
#include <string.h>
#include <sys/auxv.h>
#include <sys/cdefs.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

static inline int vdso_return(int result) {
  if (__predict_true(result == 0)) return 0;

  errno = -result;
  return -1;
}

int clock_gettime(int clock_id, timespec* tp) {
  auto vdso_clock_gettime = reinterpret_cast<decltype(&clock_gettime)>(
    __libc_globals->vdso[VDSO_CLOCK_GETTIME].fn);
  if (__predict_true(vdso_clock_gettime)) {
    return vdso_return(vdso_clock_gettime(clock_id, tp));
  }
  return __clock_gettime(clock_id, tp);
}

int clock_getres(int clock_id, timespec* tp) {
  auto vdso_clock_getres = reinterpret_cast<decltype(&clock_getres)>(
    __libc_globals->vdso[VDSO_CLOCK_GETRES].fn);
  if (__predict_true(vdso_clock_getres)) {
    return vdso_return(vdso_clock_getres(clock_id, tp));
  }
  return __clock_getres(clock_id, tp);
}

int gettimeofday(timeval* tv, struct timezone* tz) {
  auto vdso_gettimeofday = reinterpret_cast<decltype(&gettimeofday)>(
    __libc_globals->vdso[VDSO_GETTIMEOFDAY].fn);
  if (__predict_true(vdso_gettimeofday)) {
    return vdso_return(vdso_gettimeofday(tv, tz));
  }
  return __gettimeofday(tv, tz);
}

time_t time(time_t* t) {
  auto vdso_time = reinterpret_cast<decltype(&time)>(__libc_globals->vdso[VDSO_TIME].fn);
  if (__predict_true(vdso_time)) {
    return vdso_time(t);
  }

  // We can't fallback to the time(2) system call because it doesn't exist for most architectures.
  timeval tv;
  if (gettimeofday(&tv, nullptr) == -1) return -1;
  if (t) *t = tv.tv_sec;
  return tv.tv_sec;
}

void __libc_init_vdso(libc_globals* globals) {
  auto&& vdso = globals->vdso;
  vdso[VDSO_CLOCK_GETTIME] = { VDSO_CLOCK_GETTIME_SYMBOL, nullptr };
  vdso[VDSO_CLOCK_GETRES] = { VDSO_CLOCK_GETRES_SYMBOL, nullptr };
  vdso[VDSO_GETTIMEOFDAY] = { VDSO_GETTIMEOFDAY_SYMBOL, nullptr };
  vdso[VDSO_TIME] = { VDSO_TIME_SYMBOL, nullptr };

  // Do we have a vdso?
  uintptr_t vdso_ehdr_addr = getauxval(AT_SYSINFO_EHDR);
  ElfW(Ehdr)* vdso_ehdr = reinterpret_cast<ElfW(Ehdr)*>(vdso_ehdr_addr);
  if (vdso_ehdr == nullptr) {
    return;
  }

  // How many symbols does it have?
  size_t symbol_count = 0;
  ElfW(Shdr)* vdso_shdr = reinterpret_cast<ElfW(Shdr)*>(vdso_ehdr_addr + vdso_ehdr->e_shoff);
  for (size_t i = 0; i < vdso_ehdr->e_shnum; ++i) {
    if (vdso_shdr[i].sh_type == SHT_DYNSYM) {
      symbol_count = vdso_shdr[i].sh_size / sizeof(ElfW(Sym));
    }
  }
  if (symbol_count == 0) {
    return;
  }

  // Where's the dynamic table?
  ElfW(Addr) vdso_addr = 0;
  ElfW(Dyn)* vdso_dyn = nullptr;
  ElfW(Phdr)* vdso_phdr = reinterpret_cast<ElfW(Phdr)*>(vdso_ehdr_addr + vdso_ehdr->e_phoff);
  for (size_t i = 0; i < vdso_ehdr->e_phnum; ++i) {
    if (vdso_phdr[i].p_type == PT_DYNAMIC) {
      vdso_dyn = reinterpret_cast<ElfW(Dyn)*>(vdso_ehdr_addr + vdso_phdr[i].p_offset);
    } else if (vdso_phdr[i].p_type == PT_LOAD) {
      vdso_addr = vdso_ehdr_addr + vdso_phdr[i].p_offset - vdso_phdr[i].p_vaddr;
    }
  }
  if (vdso_addr == 0 || vdso_dyn == nullptr) {
    return;
  }

  // Where are the string and symbol tables?
  const char* strtab = nullptr;
  ElfW(Sym)* symtab = nullptr;
  for (ElfW(Dyn)* d = vdso_dyn; d->d_tag != DT_NULL; ++d) {
    if (d->d_tag == DT_STRTAB) {
      strtab = reinterpret_cast<const char*>(vdso_addr + d->d_un.d_ptr);
    } else if (d->d_tag == DT_SYMTAB) {
      symtab = reinterpret_cast<ElfW(Sym)*>(vdso_addr + d->d_un.d_ptr);
    }
  }
  if (strtab == nullptr || symtab == nullptr) {
    return;
  }

  // Are there any symbols we want?
  for (size_t i = 0; i < symbol_count; ++i) {
    for (size_t j = 0; j < VDSO_END; ++j) {
      if (strcmp(vdso[j].name, strtab + symtab[i].st_name) == 0) {
        vdso[j].fn = reinterpret_cast<void*>(vdso_addr + symtab[i].st_value);
      }
    }
  }
}
