/*
 * Copyright (C) 2019 The Android Open Source Project
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

#include <link.h>
#include <stdint.h>
#include <stdlib.h>

#include <utility>
#include <vector>

#include "linker_common_types.h"
#include "linker_globals.h"
#include "linker_soinfo.h"

static constexpr ElfW(Versym) kVersymHiddenBit = 0x8000;

enum RelocationKind {
  kRelocAbsolute = 0,
  kRelocRelative,
  kRelocSymbol,
  kRelocSymbolCached,
  kRelocMax
};

void count_relocation(RelocationKind kind);

template <bool Enabled> void count_relocation_if(RelocationKind kind) {
  if (Enabled) count_relocation(kind);
}

void print_linker_stats();

inline bool is_symbol_global_and_defined(const soinfo* si, const ElfW(Sym)* s) {
  if (__predict_true(ELF_ST_BIND(s->st_info) == STB_GLOBAL ||
                     ELF_ST_BIND(s->st_info) == STB_WEAK)) {
    return s->st_shndx != SHN_UNDEF;
  } else if (__predict_false(ELF_ST_BIND(s->st_info) != STB_LOCAL)) {
    DL_WARN("Warning: unexpected ST_BIND value: %d for \"%s\" in \"%s\" (ignoring)",
            ELF_ST_BIND(s->st_info), si->get_string(s->st_name), si->get_realpath());
  }
  return false;
}
