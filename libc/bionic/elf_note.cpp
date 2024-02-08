/*
 * Copyright (C) 2024 The Android Open Source Project
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

#include "platform/bionic/macros.h"
#include "private/elf_note.h"

#include <string.h>

bool __get_elf_note(unsigned note_type, const char* note_name, const ElfW(Addr) note_addr,
                    const ElfW(Phdr)* phdr_note, const ElfW(Nhdr)** note_hdr,
                    const char** note_desc) {
  if (phdr_note->p_type != PT_NOTE || !note_name || !note_addr) {
    return false;
  }

  ElfW(Addr) p = note_addr;
  ElfW(Addr) note_end = p + phdr_note->p_memsz;

  while (p + sizeof(ElfW(Nhdr)) <= note_end) {
    const ElfW(Nhdr)* note = reinterpret_cast<const ElfW(Nhdr)*>(p);
    p += sizeof(ElfW(Nhdr));
    const char* name = reinterpret_cast<const char*>(p);
    p += align_up(note->n_namesz, 4);
    const char* desc = reinterpret_cast<const char*>(p);
    p += align_up(note->n_descsz, 4);
    if (p > note_end) {
      break;
    }
    if (note->n_type != note_type) {
      continue;
    }
    size_t note_name_len = strlen(note_name) + 1;
    if (note->n_namesz != note_name_len || strncmp(note_name, name, note_name_len) != 0) {
      break;
    }

    *note_hdr = note;
    *note_desc = desc;

    return true;
  }
  return false;
}

bool __find_elf_note(unsigned int note_type, const char* note_name, const ElfW(Phdr)* phdr_start,
                     size_t phdr_ct, const ElfW(Nhdr)** note_hdr, const char** note_desc,
                     const ElfW(Addr) load_bias) {
  for (size_t i = 0; i < phdr_ct; ++i) {
    const ElfW(Phdr)* phdr = &phdr_start[i];

    ElfW(Addr) note_addr = load_bias + phdr->p_vaddr;
    if (__get_elf_note(note_type, note_name, note_addr, phdr, note_hdr, note_desc)) {
      return true;
    }
  }

  return false;
}
