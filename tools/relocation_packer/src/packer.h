// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Pack relative relocations into a more compact form.
//
//
// For relative relocations without addends (32 bit platforms)
// -----------------------------------------------------------
//
// Applies two packing strategies.  The first is run-length encoding, which
// turns a large set of relative relocations into a much smaller set
// of delta-count pairs, prefixed with a two-word header comprising the
// count of pairs and the initial relocation offset.  The second is LEB128
// encoding, which compresses the result of run-length encoding.
//
// Once packed, data is prefixed by an identifier that allows for any later
// versioning of packing strategies.
//
// A complete packed stream of relocations without addends might look
// something like:
//
//   "APR1"   pairs  init_offset count1 delta1 count2 delta2 ...
//   41505231 f2b003 b08ac716    e001   04     01     10     ...
//
//
// For relative relocations with addends (64 bit platforms)
// --------------------------------------------------------
//
// Applies two packing strategies.  The first is delta encoding, which
// turns a large set of relative relocations into a smaller set
// of offset and addend delta pairs, prefixed with a header indicating the
// count of pairs.  The second is signed LEB128 encoding, which compacts
// the result of delta encoding.
//
// Once packed, data is prefixed by an identifier that allows for any later
// versioning of packing strategies.
//
// A complete packed stream might look something like:
//
//   "APA1"   pairs  offset_d1 addend_d1 offset_d2 addend_d2 ...
//   41505232 f2b018 04        28        08        9f01      ...

#ifndef TOOLS_RELOCATION_PACKER_SRC_PACKER_H_
#define TOOLS_RELOCATION_PACKER_SRC_PACKER_H_

#include <stdint.h>
#include <vector>

#include "elf.h"

namespace relocation_packer {

// A RelocationPacker packs vectors of relocations into more
// compact forms, and unpacks them to reproduce the pre-packed data.
template <typename ELF>
class RelocationPacker {
 public:
  // Pack relocations into a more compact form.
  // |relocations| is a vector of relocation structs.
  // |packed| is the vector of packed bytes into which relocations are packed.
  static void PackRelocations(const std::vector<typename ELF::Rela>& relocations,
                              std::vector<uint8_t>* packed);

  // Unpack relocations from their more compact form.
  // |packed| is the vector of packed relocations.
  // |relocations| is a vector of unpacked relocation structs.
  static void UnpackRelocations(const std::vector<uint8_t>& packed,
                                std::vector<typename ELF::Rela>* relocations);
};

}  // namespace relocation_packer

#endif  // TOOLS_RELOCATION_PACKER_SRC_PACKER_H_
