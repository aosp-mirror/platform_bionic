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

#include <link.h>
#include <sys/auxv.h>
#include <unistd.h>

// x86 has a vdso, but there's nothing useful to us in it.
#if defined(__aarch64__) || defined(__x86_64__)

#if defined(__aarch64__)
#define VDSO_CLOCK_GETTIME_SYMBOL "__kernel_clock_gettime"
#define VDSO_GETTIMEOFDAY_SYMBOL  "__kernel_gettimeofday"
#elif defined(__x86_64__)
#define VDSO_CLOCK_GETTIME_SYMBOL "__vdso_clock_gettime"
#define VDSO_GETTIMEOFDAY_SYMBOL  "__vdso_gettimeofday"
#endif

#include <time.h>

extern "C" int __clock_gettime(int, timespec*);
extern "C" int __gettimeofday(timeval*, struct timezone*);

struct vdso_entry {
  const char* name;
  void* fn;
};

enum {
  VDSO_CLOCK_GETTIME = 0,
  VDSO_GETTIMEOFDAY,
  VDSO_END
};

static vdso_entry vdso_entries[] = {
  [VDSO_CLOCK_GETTIME] = { VDSO_CLOCK_GETTIME_SYMBOL, reinterpret_cast<void*>(__clock_gettime) },
  [VDSO_GETTIMEOFDAY] = { VDSO_GETTIMEOFDAY_SYMBOL, reinterpret_cast<void*>(__gettimeofday) },
};

int clock_gettime(int clock_id, timespec* tp) {
  static int (*vdso_clock_gettime)(int, timespec*) =
      (int (*)(int, timespec*)) vdso_entries[VDSO_CLOCK_GETTIME].fn;
  return vdso_clock_gettime(clock_id, tp);
}

int gettimeofday(timeval* tv, struct timezone* tz) {
  static int (*vdso_gettimeofday)(timeval*, struct timezone*) =
      (int (*)(timeval*, struct timezone*)) vdso_entries[VDSO_GETTIMEOFDAY].fn;
  return vdso_gettimeofday(tv, tz);
}

void __libc_init_vdso() {
  // Do we have a vdso?
  uintptr_t vdso_ehdr_addr = getauxval(AT_SYSINFO_EHDR);
  ElfW(Ehdr)* vdso_ehdr = reinterpret_cast<ElfW(Ehdr)*>(vdso_ehdr_addr);
  if (vdso_ehdr == NULL) {
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
  ElfW(Dyn)* vdso_dyn = NULL;
  ElfW(Phdr)* vdso_phdr = reinterpret_cast<ElfW(Phdr)*>(vdso_ehdr_addr + vdso_ehdr->e_phoff);
  for (size_t i = 0; i < vdso_ehdr->e_phnum; ++i) {
    if (vdso_phdr[i].p_type == PT_DYNAMIC) {
      vdso_dyn = reinterpret_cast<ElfW(Dyn)*>(vdso_ehdr_addr + vdso_phdr[i].p_offset);
    } else if (vdso_phdr[i].p_type == PT_LOAD) {
      vdso_addr = vdso_ehdr_addr + vdso_phdr[i].p_offset - vdso_phdr[i].p_vaddr;
    }
  }
  if (vdso_addr == 0 || vdso_dyn == NULL) {
    return;
  }

  // Where are the string and symbol tables?
  const char* strtab = NULL;
  ElfW(Sym)* symtab = NULL;
  for (ElfW(Dyn)* d = vdso_dyn; d->d_tag != DT_NULL; ++d) {
    if (d->d_tag == DT_STRTAB) {
      strtab = reinterpret_cast<const char*>(vdso_addr + d->d_un.d_ptr);
    } else if (d->d_tag == DT_SYMTAB) {
      symtab = reinterpret_cast<ElfW(Sym)*>(vdso_addr + d->d_un.d_ptr);
    }
  }
  if (strtab == NULL || symtab == NULL) {
    return;
  }

  // Are there any symbols we want?
  for (size_t i = 0; i < symbol_count; ++i) {
    for (size_t j = 0; j < VDSO_END; ++j) {
      if (strcmp(vdso_entries[j].name, strtab + symtab[i].st_name) == 0) {
        vdso_entries[j].fn = reinterpret_cast<void*>(vdso_addr + symtab[i].st_value);
      }
    }
  }
}

#else

void __libc_init_vdso() {
}

#endif
