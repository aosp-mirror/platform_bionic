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

#include "linker_note_gnu_property.h"

#include <elf.h>
#include <link.h>

#include "linker.h"
#include "linker_debug.h"
#include "linker_globals.h"
#include "linker_soinfo.h"

GnuPropertySection::GnuPropertySection(const soinfo* si)
    : GnuPropertySection(si->phdr, si->phnum, si->load_bias, si->get_realpath()) {}

GnuPropertySection::GnuPropertySection(const ElfW(Phdr)* phdr, size_t phdr_count,
                                       const ElfW(Addr) load_bias, const char* name) {
  // Try to find PT_GNU_PROPERTY segment.
  auto note_gnu_property = FindSegment(phdr, phdr_count, load_bias, name);
  // Perform some validity checks.
  if (note_gnu_property && SanityCheck(note_gnu_property, name)) {
    // Parse section.
    Parse(note_gnu_property, name);
  }
}

const ElfW(NhdrGNUProperty)* GnuPropertySection::FindSegment(const ElfW(Phdr)* phdr,
                                                             size_t phdr_count,
                                                             const ElfW(Addr) load_bias,
                                                             const char* name) const {
  // According to Linux gABI extension this segment should contain
  // .note.gnu.property section only.
  if (phdr != nullptr) {
    for (size_t i = 0; i < phdr_count; ++i) {
      if (phdr[i].p_type != PT_GNU_PROPERTY) {
        continue;
      }

      TRACE("\"%s\" PT_GNU_PROPERTY: found at segment index %zu", name, i);

      // Check segment size.
      if (phdr[i].p_memsz < sizeof(ElfW(NhdrGNUProperty))) {
        DL_ERR_AND_LOG(
            "\"%s\" PT_GNU_PROPERTY segment is too small. Segment "
            "size is %zu, minimum is %zu.",
            name, static_cast<size_t>(phdr[i].p_memsz), sizeof(ElfW(NhdrGNUProperty)));
        return nullptr;
      }

      // PT_GNU_PROPERTY contains .note.gnu.property which has SHF_ALLOC
      // attribute, therefore it is loaded.
      auto note_nhdr = reinterpret_cast<ElfW(NhdrGNUProperty)*>(load_bias + phdr[i].p_vaddr);

      // Check that the n_descsz <= p_memsz
      if ((phdr[i].p_memsz - sizeof(ElfW(NhdrGNUProperty))) < note_nhdr->nhdr.n_descsz) {
        DL_ERR_AND_LOG(
            "\"%s\" PT_GNU_PROPERTY segment p_memsz (%zu) is too small for note n_descsz (%zu).",
            name, static_cast<size_t>(phdr[i].p_memsz),
            static_cast<size_t>(note_nhdr->nhdr.n_descsz));
        return nullptr;
      }

      return note_nhdr;
    }
  }

  TRACE("\"%s\" PT_GNU_PROPERTY: not found", name);
  return nullptr;
}

bool GnuPropertySection::SanityCheck(const ElfW(NhdrGNUProperty)* note_nhdr,
                                     const char* name) const {
  // Check .note section type
  if (note_nhdr->nhdr.n_type != NT_GNU_PROPERTY_TYPE_0) {
    DL_ERR_AND_LOG("\"%s\" .note.gnu.property: unexpected note type. Expected %u, got %u.", name,
                   NT_GNU_PROPERTY_TYPE_0, note_nhdr->nhdr.n_type);
    return false;
  }

  if (note_nhdr->nhdr.n_namesz != 4) {
    DL_ERR_AND_LOG("\"%s\" .note.gnu.property: unexpected name size. Expected 4, got %u.", name,
                   note_nhdr->nhdr.n_namesz);
    return false;
  }

  if (strncmp(note_nhdr->n_name, "GNU", 4) != 0) {
    DL_ERR_AND_LOG("\"%s\" .note.gnu.property: unexpected name. Expected 'GNU', got '%s'.", name,
                   note_nhdr->n_name);
    return false;
  }

  return true;
}

bool GnuPropertySection::Parse(const ElfW(NhdrGNUProperty)* note_nhdr, const char* name) {
  // The total length of the program property array is in _bytes_.
  ElfW(Word) offset = 0;
  while (offset < note_nhdr->nhdr.n_descsz) {
    DEBUG("\"%s\" .note.gnu.property: processing at offset 0x%x", name, offset);

    // At least the "header" part must fit.
    // The ABI doesn't say that pr_datasz can't be 0.
    if ((note_nhdr->nhdr.n_descsz - offset) < sizeof(ElfW(Prop))) {
      DL_ERR_AND_LOG(
          "\"%s\" .note.gnu.property: no more space left for a "
          "Program Property Note header.",
          name);
      return false;
    }

    // Loop on program property array.
    const ElfW(Prop)* property = reinterpret_cast<const ElfW(Prop)*>(&note_nhdr->n_desc[offset]);
    const ElfW(Word) property_size =
        align_up(sizeof(ElfW(Prop)) + property->pr_datasz, sizeof(ElfW(Addr)));
    if ((note_nhdr->nhdr.n_descsz - offset) < property_size) {
      DL_ERR_AND_LOG(
          "\"%s\" .note.gnu.property: property descriptor size is "
          "invalid. Expected at least %u bytes, got %u.",
          name, property_size, note_nhdr->nhdr.n_descsz - offset);
      return false;
    }

    // Cache found properties.
    switch (property->pr_type) {
#if defined(__aarch64__)
      case GNU_PROPERTY_AARCH64_FEATURE_1_AND: {
        if (property->pr_datasz != 4) {
          DL_ERR_AND_LOG(
              "\"%s\" .note.gnu.property: property descriptor size is "
              "invalid. Expected %u bytes for GNU_PROPERTY_AARCH64_FEATURE_1_AND, got %u.",
              name, 4, property->pr_datasz);
          return false;
        }

        const ElfW(Word) flags = *reinterpret_cast<const ElfW(Word)*>(&property->pr_data[0]);
        properties_.bti_compatible = (flags & GNU_PROPERTY_AARCH64_FEATURE_1_BTI) != 0;
        if (properties_.bti_compatible) {
          INFO("[ BTI compatible: \"%s\" ]", name);
        }
        break;
      }
#endif
      default:
        DEBUG("\"%s\" .note.gnu.property: found property pr_type %u pr_datasz 0x%x", name,
              property->pr_type, property->pr_datasz);
        break;
    }

    // Move offset, this should be safe to add because of previous checks.
    offset += property_size;
  }

  return true;
}

#if defined(__aarch64__)
bool GnuPropertySection::IsBTICompatible() const {
  return (g_platform_properties.bti_supported && properties_.bti_compatible);
}
#endif
