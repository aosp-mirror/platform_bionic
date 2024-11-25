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

#include "linker_phdr.h"

#include <linux/prctl.h>
#include <sys/mman.h>
#include <sys/prctl.h>
#include <unistd.h>

#include "linker_debug.h"
#include "linker_dlwarning.h"
#include "linker_globals.h"

#include "platform/bionic/macros.h"
#include "platform/bionic/page.h"

#include <string>

static bool g_enable_16kb_app_compat;

static inline bool segment_contains_prefix(const ElfW(Phdr)* segment, const ElfW(Phdr)* prefix) {
  return segment && prefix && segment->p_vaddr == prefix->p_vaddr;
}

void set_16kb_appcompat_mode(bool enable_app_compat) {
  g_enable_16kb_app_compat = enable_app_compat;
}

bool get_16kb_appcompat_mode() {
  return g_enable_16kb_app_compat;
}

/*
 * Returns true if the ELF contains at most 1 RELRO segment; and populates @relro_phdr
 * with the relro phdr or nullptr if none.
 *
 * Returns false if more than 1 RELRO segments are found.
 */
bool ElfReader::HasAtMostOneRelroSegment(const ElfW(Phdr)** relro_phdr) {
  const ElfW(Phdr)* relro = nullptr;
  for (size_t i = 0; i < phdr_num_; ++i) {
    const ElfW(Phdr)* phdr = &phdr_table_[i];

    if (phdr->p_type != PT_GNU_RELRO) {
      continue;
    }

    if (relro == nullptr) {
      relro = phdr;
    } else {
      return false;
    }
  }

  *relro_phdr = relro;

  return true;
}

/*
 * In 16KiB compatibility mode ELFs with the following segment layout
 * can be loaded successfully:
 *
 *         ┌────────────┬─────────────────────────┬────────────┐
 *         │            │                         │            │
 *         │  (RO|RX)*  │   (RW - RELRO prefix)?  │    (RW)*   │
 *         │            │                         │            │
 *         └────────────┴─────────────────────────┴────────────┘
 *
 * In other words, compatible layouts have:
 *         - zero or more RO or RX segments;
 *         - followed by zero or one RELRO prefix;
 *         - followed by zero or more RW segments (this can include the RW
 *           suffix from the segment containing the RELRO prefix, if any)
 *
 * In 16KiB compat mode, after relocation, the ELF is layout in virtual
 * memory is as shown below:
 *         ┌──────────────────────────────────────┬────────────┐
 *         │                                      │            │
 *         │                (RX)?                 │    (RW)?   │
 *         │                                      │            │
 *         └──────────────────────────────────────┴────────────┘
 *
 * In compat mode:
 *         - the RO and RX segments along with the RELRO prefix are protected
 *           as RX;
 *         - and the RW segments along with RW suffix from the relro segment,
 *           if any; are RW protected.
 *
 * This allows for the single RX|RW permission boundary to be aligned with
 * a 16KiB page boundary; since a single page cannot share multiple
 * permissions.
 *
 * IsEligibleFor16KiBAppCompat() identifies compatible ELFs and populates @vaddr
 * with the boundary between RX|RW portions.
 *
 * Returns true if the ELF can be loaded in compat mode, else false.
 */
bool ElfReader::IsEligibleFor16KiBAppCompat(ElfW(Addr)* vaddr) {
  const ElfW(Phdr)* relro_phdr = nullptr;
  if (!HasAtMostOneRelroSegment(&relro_phdr)) {
    DL_WARN("\"%s\": Compat loading failed: Multiple RELRO segments found", name_.c_str());
    return false;
  }

  const ElfW(Phdr)* last_rw = nullptr;
  const ElfW(Phdr)* first_rw = nullptr;

  for (size_t i = 0; i < phdr_num_; ++i) {
    const ElfW(Phdr)* curr = &phdr_table_[i];
    const ElfW(Phdr)* prev = (i > 0) ? &phdr_table_[i - 1] : nullptr;

    if (curr->p_type != PT_LOAD) {
      continue;
    }

    int prot = PFLAGS_TO_PROT(curr->p_flags);

    if ((prot & PROT_WRITE) && (prot & PROT_READ)) {
      if (!first_rw) {
        first_rw = curr;
      }

      if (last_rw && last_rw != prev) {
        DL_WARN("\"%s\": Compat loading failed: ELF contains multiple non-adjacent RW segments",
                name_.c_str());
        return false;
      }

      last_rw = curr;
    }
  }

  if (!relro_phdr) {
    *vaddr = align_down(first_rw->p_vaddr, kCompatPageSize);
    return true;
  }

  // The RELRO segment is present, it must be the prefix of the first RW segment.
  if (!segment_contains_prefix(first_rw, relro_phdr)) {
    DL_WARN("\"%s\": Compat loading failed: RELRO is not in the first RW segment",
            name_.c_str());
    return false;
  }

  uint64_t end;
  if (__builtin_add_overflow(relro_phdr->p_vaddr, relro_phdr->p_memsz, &end)) {
    DL_WARN("\"%s\": Compat loading failed: relro vaddr + memsz overflowed", name_.c_str());
    return false;
  }

  *vaddr = align_up(end, kCompatPageSize);
  return true;
}

/*
 * Returns the offset/shift needed to align @vaddr to a page boundary.
 */
static inline ElfW(Addr) perm_boundary_offset(const ElfW(Addr) addr) {
  ElfW(Addr) offset = page_offset(addr);

  return offset ? page_size() - offset : 0;
}

bool ElfReader::Setup16KiBAppCompat() {
  if (!should_use_16kib_app_compat_) {
    return true;
  }

  ElfW(Addr) rx_rw_boundary;  // Permission bounadry for compat mode
  if (!IsEligibleFor16KiBAppCompat(&rx_rw_boundary)) {
    return false;
  }

  // Adjust the load_bias to position the RX|RW boundary on a page boundary
  load_bias_ += perm_boundary_offset(rx_rw_boundary);

  // RW region (.data, .bss ...)
  ElfW(Addr) rw_start = load_bias_ + rx_rw_boundary;
  ElfW(Addr) rw_size = load_size_ - (rw_start - reinterpret_cast<ElfW(Addr)>(load_start_));

  CHECK(rw_start % getpagesize() == 0);
  CHECK(rw_size % getpagesize() == 0);

  // Compat RELRO (RX) region (.text, .data.relro, ...)
  compat_relro_start_ = reinterpret_cast<ElfW(Addr)>(load_start_);
  compat_relro_size_ = load_size_ - rw_size;

  // Label the ELF VMA, since compat mode uses anonymous mappings.
  std::string compat_name = name_ + " (compat loaded)";
  prctl(PR_SET_VMA, PR_SET_VMA_ANON_NAME, load_start_, load_size_, compat_name.c_str());

  return true;
}

bool ElfReader::CompatMapSegment(size_t seg_idx, size_t len) {
  const ElfW(Phdr)* phdr = &phdr_table_[seg_idx];

  // NOTE: The compat(legacy) page size (4096) must be used when aligning
  // the 4KiB segments for loading (reading). The larger 16KiB page size
  // will lead to overwriting adjacent segments since the ELF's segment(s)
  // are not 16KiB aligned.

  void* start = reinterpret_cast<void*>(align_down(phdr->p_vaddr + load_bias_, kCompatPageSize));

  // The ELF could be being loaded directly from a zipped APK,
  // the zip offset must be added to find the segment offset.
  const ElfW(Addr) offset = file_offset_ + align_down(phdr->p_offset, kCompatPageSize);

  CHECK(should_use_16kib_app_compat_);

  // Since the 4KiB max-page-size ELF is not properly aligned, loading it by
  // directly mmapping the ELF file is not feasible.
  // Instead, read the ELF contents into the anonymous RW mapping.
  if (TEMP_FAILURE_RETRY(pread64(fd_, start, len, offset)) == -1) {
    DL_ERR("Compat loading: \"%s\" failed to read LOAD segment %zu: %m", name_.c_str(), seg_idx);
    return false;
  }

  return true;
}
