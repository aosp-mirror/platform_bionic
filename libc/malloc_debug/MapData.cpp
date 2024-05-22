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
#include <link.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/uio.h>
#include <unistd.h>

#include <vector>

#include "MapData.h"

// Format of /proc/<PID>/maps:
//   6f000000-6f01e000 rwxp 00000000 00:0c 16389419   /system/lib/libcomposer.so
static MapEntry* parse_line(char* line) {
  uintptr_t start;
  uintptr_t end;
  uintptr_t offset;
  int flags;
  char permissions[5];
  int name_pos;
  if (sscanf(line, "%" PRIxPTR "-%" PRIxPTR " %4s %" PRIxPTR " %*x:%*x %*d %n", &start, &end,
             permissions, &offset, &name_pos) < 2) {
    return nullptr;
  }

  const char* name = line + name_pos;
  size_t name_len = strlen(name);
  if (name_len && name[name_len - 1] == '\n') {
    name_len -= 1;
  }

  flags = 0;
  if (permissions[0] == 'r') {
    flags |= PROT_READ;
  }
  if (permissions[2] == 'x') {
    flags |= PROT_EXEC;
  }

  MapEntry* entry = new MapEntry(start, end, offset, name, name_len, flags);
  if (!(flags & PROT_READ)) {
    // This will make sure that an unreadable map will prevent attempts to read
    // elf data from the map.
    entry->SetInvalid();
  }
  return entry;
}

void MapEntry::Init() {
  if (init_) {
    return;
  }
  init_ = true;

  uintptr_t end_addr;
  if (__builtin_add_overflow(start_, SELFMAG, &end_addr) || end_addr >= end_) {
    return;
  }

  ElfW(Ehdr) ehdr;
  struct iovec src_io = {.iov_base = reinterpret_cast<void*>(start_), .iov_len = SELFMAG};
  struct iovec dst_io = {.iov_base = ehdr.e_ident, .iov_len = SELFMAG};
  ssize_t rc = process_vm_readv(getpid(), &dst_io, 1, &src_io, 1, 0);
  valid_ = rc == SELFMAG && IS_ELF(ehdr);
}

uintptr_t MapEntry::GetLoadBias() {
  if (!valid_) {
    return 0;
  }

  if (load_bias_read_) {
    return load_bias_;
  }

  load_bias_read_ = true;

  ElfW(Ehdr) ehdr;
  struct iovec src_io = {.iov_base = reinterpret_cast<void*>(start_), .iov_len = sizeof(ehdr)};
  struct iovec dst_io = {.iov_base = &ehdr, .iov_len = sizeof(ehdr)};
  ssize_t rc = process_vm_readv(getpid(), &dst_io, 1, &src_io, 1, 0);
  if (rc != sizeof(ehdr)) {
    return 0;
  }

  uintptr_t addr = start_ + ehdr.e_phoff;
  for (size_t i = 0; i < ehdr.e_phnum; i++) {
    ElfW(Phdr) phdr;

    src_io.iov_base = reinterpret_cast<void*>(addr);
    src_io.iov_len = sizeof(phdr);
    dst_io.iov_base = &phdr;
    dst_io.iov_len = sizeof(phdr);
    rc = process_vm_readv(getpid(), &dst_io, 1, &src_io, 1, 0);
    if (rc != sizeof(phdr)) {
      return 0;
    }
    if ((phdr.p_type == PT_LOAD) && (phdr.p_flags & PF_X) ) {
      load_bias_ = phdr.p_vaddr - phdr.p_offset;
      return load_bias_;
    }
    addr += sizeof(phdr);
  }
  return 0;
}

void MapData::ReadMaps() {
  std::lock_guard<std::mutex> lock(m_);
  FILE* fp = fopen("/proc/self/maps", "re");
  if (fp == nullptr) {
    return;
  }

  ClearEntries();

  std::vector<char> buffer(1024);
  while (fgets(buffer.data(), buffer.size(), fp) != nullptr) {
    MapEntry* entry = parse_line(buffer.data());
    if (entry == nullptr) {
      break;
    }
    entries_.insert(entry);
  }
  fclose(fp);
}

void MapData::ClearEntries() {
  for (auto* entry : entries_) {
    delete entry;
  }
  entries_.clear();
}

MapData::~MapData() {
  ClearEntries();
}

// Find the containing map info for the PC.
const MapEntry* MapData::find(uintptr_t pc, uintptr_t* rel_pc) {
  MapEntry pc_entry(pc);

  std::lock_guard<std::mutex> lock(m_);
  auto it = entries_.find(&pc_entry);
  if (it == entries_.end()) {
    return nullptr;
  }

  MapEntry* entry = *it;
  entry->Init();

  if (rel_pc != nullptr) {
    // Need to check to see if this is a read-execute map and the read-only
    // map is the previous one.
    if (!entry->valid() && it != entries_.begin()) {
      MapEntry* prev_entry = *--it;
      if (prev_entry->flags() == PROT_READ && prev_entry->offset() < entry->offset() &&
          prev_entry->name() == entry->name()) {
        prev_entry->Init();

        if (prev_entry->valid()) {
          entry->set_elf_start_offset(prev_entry->offset());
          *rel_pc = pc - entry->start() + entry->offset() + prev_entry->GetLoadBias();
          return entry;
        }
      }
    }
    *rel_pc = pc - entry->start() + entry->offset() + entry->GetLoadBias();
  }
  return entry;
}
