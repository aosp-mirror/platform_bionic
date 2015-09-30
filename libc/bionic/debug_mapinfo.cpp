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

#include <ctype.h>
#include <elf.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "debug_mapinfo.h"
#include "malloc_debug_disable.h"

#if defined(__LP64__)
#define Elf_W(x) Elf64_##x
#else
#define Elf_W(x) Elf32_##x
#endif

// Format of /proc/<PID>/maps:
//   6f000000-6f01e000 rwxp 00000000 00:0c 16389419   /system/lib/libcomposer.so
static mapinfo_t* parse_maps_line(char* line) {
  uintptr_t start;
  uintptr_t end;
  uintptr_t offset;
  char permissions[4];
  int name_pos;
  if (sscanf(line, "%" PRIxPTR "-%" PRIxPTR " %4s %" PRIxPTR " %*x:%*x %*d %n", &start,
             &end, permissions, &offset, &name_pos) < 2) {
    return NULL;
  }

  const char* name = line + name_pos;
  size_t name_len = strlen(name);
  if (name_len && name[name_len - 1] == '\n') {
    name_len -= 1;
  }

  mapinfo_t* mi = reinterpret_cast<mapinfo_t*>(calloc(1, sizeof(mapinfo_t) + name_len + 1));
  if (mi) {
    mi->start = start;
    mi->end = end;
    mi->offset = offset;
    if (permissions[0] != 'r') {
      // Any unreadable map will just get a zero load base.
      mi->load_base = 0;
      mi->load_base_read = true;
    } else {
      mi->load_base_read = false;
    }
    memcpy(mi->name, name, name_len);
    mi->name[name_len] = '\0';
  }
  return mi;
}

__LIBC_HIDDEN__ mapinfo_t* mapinfo_create(pid_t pid) {
  ScopedDisableDebugCalls disable;

  struct mapinfo_t* milist = NULL;
  char data[1024]; // Used to read lines as well as to construct the filename.
  snprintf(data, sizeof(data), "/proc/%d/maps", pid);
  FILE* fp = fopen(data, "re");
  if (fp != NULL) {
    while (fgets(data, sizeof(data), fp) != NULL) {
      mapinfo_t* mi = parse_maps_line(data);
      if (mi) {
        mi->next = milist;
        milist = mi;
      }
    }
    fclose(fp);
  }
  return milist;
}

__LIBC_HIDDEN__ void mapinfo_destroy(mapinfo_t* mi) {
  ScopedDisableDebugCalls disable;

  while (mi != NULL) {
    mapinfo_t* del = mi;
    mi = mi->next;
    free(del);
  }
}

template<typename T>
static inline bool get_val(mapinfo_t* mi, uintptr_t addr, T* store) {
  if (addr < mi->start || addr + sizeof(T) > mi->end) {
    return false;
  }
  // Make sure the address is aligned properly.
  if (addr & (sizeof(T)-1)) {
    return false;
  }
  *store = *reinterpret_cast<T*>(addr);
  return true;
}

__LIBC_HIDDEN__ void mapinfo_read_loadbase(mapinfo_t* mi) {
  mi->load_base = 0;
  mi->load_base_read = true;
  uintptr_t addr = mi->start;
  Elf_W(Ehdr) ehdr;
  if (!get_val<Elf_W(Half)>(mi, addr + offsetof(Elf_W(Ehdr), e_phnum), &ehdr.e_phnum)) {
    return;
  }
  if (!get_val<Elf_W(Off)>(mi, addr + offsetof(Elf_W(Ehdr), e_phoff), &ehdr.e_phoff)) {
    return;
  }
  addr += ehdr.e_phoff;
  for (size_t i = 0; i < ehdr.e_phnum; i++) {
    Elf_W(Phdr) phdr;
    if (!get_val<Elf_W(Word)>(mi, addr + offsetof(Elf_W(Phdr), p_type), &phdr.p_type)) {
      return;
    }
    if (!get_val<Elf_W(Off)>(mi, addr + offsetof(Elf_W(Phdr), p_offset), &phdr.p_offset)) {
      return;
    }
    if (phdr.p_type == PT_LOAD && phdr.p_offset == mi->offset) {
      if (!get_val<Elf_W(Addr)>(mi, addr + offsetof(Elf_W(Phdr), p_vaddr), &phdr.p_vaddr)) {
        return;
      }
      mi->load_base = phdr.p_vaddr;
      return;
    }
    addr += sizeof(phdr);
  }
}

// Find the containing map info for the PC.
__LIBC_HIDDEN__ const mapinfo_t* mapinfo_find(mapinfo_t* mi, uintptr_t pc, uintptr_t* rel_pc) {
  for (; mi != NULL; mi = mi->next) {
    if ((pc >= mi->start) && (pc < mi->end)) {
      if (!mi->load_base_read) {
        mapinfo_read_loadbase(mi);
      }
      *rel_pc = pc - mi->start + mi->load_base;
      return mi;
    }
  }
  *rel_pc = pc;
  return NULL;
}
