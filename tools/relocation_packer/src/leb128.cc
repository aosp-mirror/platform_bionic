// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "leb128.h"

#include <stdint.h>
#include <vector>

#include "elf_traits.h"

namespace relocation_packer {

// Empty constructor and destructor to silence chromium-style.
template <typename uint_t>
Leb128Encoder<uint_t>::Leb128Encoder() { }

template <typename uint_t>
Leb128Encoder<uint_t>::~Leb128Encoder() { }

// Add a single value to the encoding.  Values are encoded with variable
// length.  The least significant 7 bits of each byte hold 7 bits of data,
// and the most significant bit is set on each byte except the last.
template <typename uint_t>
void Leb128Encoder<uint_t>::Enqueue(uint_t value) {
  uint_t uvalue = static_cast<uint_t>(value);
  do {
    const uint8_t byte = uvalue & 127;
    uvalue >>= 7;
    encoding_.push_back((uvalue ? 128 : 0) | byte);
  } while (uvalue);
}

// Add a vector of values to the encoding.
template <typename uint_t>
void Leb128Encoder<uint_t>::EnqueueAll(const std::vector<uint_t>& values) {
  for (size_t i = 0; i < values.size(); ++i) {
    Enqueue(values[i]);
  }
}

// Create a new decoder for the given encoded stream.
template <typename uint_t>
Leb128Decoder<uint_t>::Leb128Decoder(const std::vector<uint8_t>& encoding, size_t start_with) {
  encoding_ = encoding;
  cursor_ = start_with;
}

// Empty destructor to silence chromium-style.
template <typename uint_t>
Leb128Decoder<uint_t>::~Leb128Decoder() { }

// Decode and retrieve a single value from the encoding.  Read forwards until
// a byte without its most significant bit is found, then read the 7 bit
// fields of the bytes spanned to re-form the value.
template <typename uint_t>
uint_t Leb128Decoder<uint_t>::Dequeue() {
  uint_t value = 0;

  size_t shift = 0;
  uint8_t byte;

  // Loop until we reach a byte with its high order bit clear.
  do {
    byte = encoding_[cursor_++];
    value |= static_cast<uint_t>(byte & 127) << shift;
    shift += 7;
  } while (byte & 128);

  return value;
}

// Decode and retrieve all remaining values from the encoding.
template <typename uint_t>
void Leb128Decoder<uint_t>::DequeueAll(std::vector<uint_t>* values) {
  while (cursor_ < encoding_.size()) {
    values->push_back(Dequeue());
  }
}

template class Leb128Encoder<uint32_t>;
template class Leb128Encoder<uint64_t>;

template class Leb128Decoder<uint32_t>;
template class Leb128Decoder<uint64_t>;

}  // namespace relocation_packer
