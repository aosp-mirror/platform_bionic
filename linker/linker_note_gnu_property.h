/*
 * Copyright (C) 2020 The Android Open Source Project
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

#include <elf.h>
#include <link.h>

#include "linker_soinfo.h"

// The Elf* structures below are derived from the document
// Linux Extensions to gABI (https://github.com/hjl-tools/linux-abi/wiki).
// Essentially, these types would be defined in <elf.h>, but this is not
// the case at the moment.

struct Elf32_Prop {
  Elf32_Word pr_type;
  Elf32_Word pr_datasz;
  char pr_data[0];
};

// On 32-bit machines this should be 4-byte aligned.
struct Elf32_NhdrGNUProperty {
  Elf32_Nhdr nhdr;
  char n_name[4];
  char n_desc[0];
};

struct Elf64_Prop {
  Elf64_Word pr_type;
  Elf64_Word pr_datasz;
  char pr_data[0];
};

// On 64-bit machines this should be 8-byte aligned.
struct Elf64_NhdrGNUProperty {
  Elf64_Nhdr nhdr;
  char n_name[4];
  char n_desc[0];
};

struct ElfProgramProperty {
#if defined(__aarch64__)
  bool bti_compatible = false;
#endif
};

// Representation of the .note.gnu.property section found in the segment
// with p_type = PT_GNU_PROPERTY.
class GnuPropertySection {
 public:
  GnuPropertySection(){};
  explicit GnuPropertySection(const soinfo* si);
  GnuPropertySection(const ElfW(Phdr)* phdr, size_t phdr_count, const ElfW(Addr) load_bias,
                     const char* name);

#if defined(__aarch64__)
  bool IsBTICompatible() const;
#endif

 private:
  const ElfW(NhdrGNUProperty)* FindSegment(const ElfW(Phdr)* phdr, size_t phdr_count,
                                           const ElfW(Addr) load_bias, const char* name) const;
  bool SanityCheck(const ElfW(NhdrGNUProperty)* note_nhdr, const char* name) const;
  bool Parse(const ElfW(NhdrGNUProperty)* note_nhdr, const char* name);

  ElfProgramProperty properties_ __unused;
};
