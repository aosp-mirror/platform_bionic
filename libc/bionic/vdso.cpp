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
#include <sys/hwprobe.h>
#include <sys/time.h>
#include <syscall.h>
#include <time.h>
#include <unistd.h>

extern "C" int __clock_gettime(int, struct timespec*);
extern "C" int __clock_getres(int, struct timespec*);
extern "C" int __gettimeofday(struct timeval*, struct timezone*);

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
  // Only x86/x86-64 actually have time() in the vdso.
#if defined(VDSO_TIME_SYMBOL)
  auto vdso_time = reinterpret_cast<decltype(&time)>(__libc_globals->vdso[VDSO_TIME].fn);
  if (__predict_true(vdso_time)) {
    return vdso_time(t);
  }
#endif

  // We can't fallback to the time(2) system call because it doesn't exist for most architectures.
  timeval tv;
  if (gettimeofday(&tv, nullptr) == -1) return -1;
  if (t) *t = tv.tv_sec;
  return tv.tv_sec;
}

#if defined(__riscv)
int __riscv_hwprobe(struct riscv_hwprobe* _Nonnull pairs, size_t pair_count, size_t cpu_count,
                    unsigned long* _Nullable cpus, unsigned flags) {
  auto vdso_riscv_hwprobe =
      reinterpret_cast<decltype(&__riscv_hwprobe)>(__libc_globals->vdso[VDSO_RISCV_HWPROBE].fn);
  if (__predict_true(vdso_riscv_hwprobe)) {
    return -vdso_riscv_hwprobe(pairs, pair_count, cpu_count, cpus, flags);
  }
  // Inline the syscall directly in case someone's calling it from an
  // ifunc resolver where we won't be able to set errno on failure.
  // (Rather than our usual trick of letting the python-generated
  // wrapper set errno but saving/restoring errno in cases where the API
  // is to return an error value rather than setting errno.)
  register long a0 __asm__("a0") = reinterpret_cast<long>(pairs);
  register long a1 __asm__("a1") = pair_count;
  register long a2 __asm__("a2") = cpu_count;
  register long a3 __asm__("a3") = reinterpret_cast<long>(cpus);
  register long a4 __asm__("a4") = flags;
  register long a7 __asm__("a7") = __NR_riscv_hwprobe;
  __asm__ volatile("ecall" : "=r"(a0) : "r"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4), "r"(a7));
  return -a0;
}
#endif

void __libc_init_vdso(libc_globals* globals) {
  auto&& vdso = globals->vdso;
  vdso[VDSO_CLOCK_GETTIME] = {VDSO_CLOCK_GETTIME_SYMBOL, nullptr};
  vdso[VDSO_CLOCK_GETRES] = {VDSO_CLOCK_GETRES_SYMBOL, nullptr};
  vdso[VDSO_GETTIMEOFDAY] = {VDSO_GETTIMEOFDAY_SYMBOL, nullptr};
#if defined(VDSO_TIME_SYMBOL)
  vdso[VDSO_TIME] = {VDSO_TIME_SYMBOL, nullptr};
#endif
#if defined(VDSO_RISCV_HWPROBE_SYMBOL)
  vdso[VDSO_RISCV_HWPROBE] = {VDSO_RISCV_HWPROBE_SYMBOL, nullptr};
#endif

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
      break;
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
    if (vdso_addr && vdso_dyn) break;
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
    if (strtab && symtab) break;
  }
  if (strtab == nullptr || symtab == nullptr) {
    return;
  }

  // Are there any symbols we want?
  for (size_t i = 0; i < VDSO_END; ++i) {
    for (size_t j = 0; j < symbol_count; ++j) {
      if (strcmp(vdso[i].name, strtab + symtab[j].st_name) == 0) {
        vdso[i].fn = reinterpret_cast<void*>(vdso_addr + symtab[j].st_value);
        break;
      }
    }
  }
}
