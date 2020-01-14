/*
 * Copyright (C) 2015 The Android Open Source Project
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

#include <string.h>

#include "linker.h"
#include "linker_sleb128.h"

const size_t RELOCATION_GROUPED_BY_INFO_FLAG = 1;
const size_t RELOCATION_GROUPED_BY_OFFSET_DELTA_FLAG = 2;
const size_t RELOCATION_GROUPED_BY_ADDEND_FLAG = 4;
const size_t RELOCATION_GROUP_HAS_ADDEND_FLAG = 8;

#if defined(USE_RELA)
typedef ElfW(Rela) rel_t;
#else
typedef ElfW(Rel) rel_t;
#endif

template <typename F>
inline bool for_all_packed_relocs(sleb128_decoder decoder, F&& callback) {
  const size_t num_relocs = decoder.pop_front();

  rel_t reloc = {
    .r_offset = decoder.pop_front(),
  };

  for (size_t idx = 0; idx < num_relocs; ) {
    const size_t group_size = decoder.pop_front();
    const size_t group_flags = decoder.pop_front();

    size_t group_r_offset_delta = 0;

    if (group_flags & RELOCATION_GROUPED_BY_OFFSET_DELTA_FLAG) {
      group_r_offset_delta = decoder.pop_front();
    }
    if (group_flags & RELOCATION_GROUPED_BY_INFO_FLAG) {
      reloc.r_info = decoder.pop_front();
    }

#if defined(USE_RELA)
    const size_t group_flags_reloc = group_flags & (RELOCATION_GROUP_HAS_ADDEND_FLAG |
                                                    RELOCATION_GROUPED_BY_ADDEND_FLAG);
    if (group_flags_reloc == RELOCATION_GROUP_HAS_ADDEND_FLAG) {
      // Each relocation has an addend. This is the default situation with lld's current encoder.
    } else if (group_flags_reloc == (RELOCATION_GROUP_HAS_ADDEND_FLAG |
                                     RELOCATION_GROUPED_BY_ADDEND_FLAG)) {
      reloc.r_addend += decoder.pop_front();
    } else {
      reloc.r_addend = 0;
    }
#else
    if (__predict_false(group_flags & RELOCATION_GROUP_HAS_ADDEND_FLAG)) {
      // This platform does not support rela, and yet we have it encoded in android_rel section.
      async_safe_fatal("unexpected r_addend in android.rel section");
    }
#endif

    for (size_t i = 0; i < group_size; ++i) {
      if (group_flags & RELOCATION_GROUPED_BY_OFFSET_DELTA_FLAG) {
        reloc.r_offset += group_r_offset_delta;
      } else {
        reloc.r_offset += decoder.pop_front();
      }
      if ((group_flags & RELOCATION_GROUPED_BY_INFO_FLAG) == 0) {
        reloc.r_info = decoder.pop_front();
      }
#if defined(USE_RELA)
      if (group_flags_reloc == RELOCATION_GROUP_HAS_ADDEND_FLAG) {
        reloc.r_addend += decoder.pop_front();
      }
#endif
      if (!callback(reloc)) {
        return false;
      }
    }

    idx += group_size;
  }

  return true;
}
