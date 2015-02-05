/*
 * Copyright (C) 2015 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __LINKER_RELOC_ITERATORS_H
#define __LINKER_RELOC_ITERATORS_H

#include "linker.h"

#include <string.h>

#define RELOCATION_GROUPED_BY_INFO_FLAG 1
#define RELOCATION_GROUPED_BY_OFFSET_DELTA_FLAG 2
#define RELOCATION_GROUPED_BY_ADDEND_FLAG 4
#define RELOCATION_GROUP_HAS_ADDEND_FLAG 8

#define RELOCATION_GROUPED_BY_INFO(flags) (((flags) & RELOCATION_GROUPED_BY_INFO_FLAG) != 0)
#define RELOCATION_GROUPED_BY_OFFSET_DELTA(flags) (((flags) & RELOCATION_GROUPED_BY_OFFSET_DELTA_FLAG) != 0)
#define RELOCATION_GROUPED_BY_ADDEND(flags) (((flags) & RELOCATION_GROUPED_BY_ADDEND_FLAG) != 0)
#define RELOCATION_GROUP_HAS_ADDEND(flags) (((flags) & RELOCATION_GROUP_HAS_ADDEND_FLAG) != 0)

class plain_reloc_iterator {
#if defined(USE_RELA)
  typedef ElfW(Rela) rel_t;
#else
  typedef ElfW(Rel) rel_t;
#endif
 public:
  plain_reloc_iterator(rel_t* rel_array, size_t count)
      : begin_(rel_array), end_(begin_ + count), current_(begin_) {}

  bool has_next() {
    return current_ < end_;
  }

  rel_t* next() {
    return current_++;
  }
 private:
  rel_t* const begin_;
  rel_t* const end_;
  rel_t* current_;

  DISALLOW_COPY_AND_ASSIGN(plain_reloc_iterator);
};

template <typename decoder_t>
class packed_reloc_iterator {
#if defined(USE_RELA)
  typedef ElfW(Rela) rel_t;
#else
  typedef ElfW(Rel) rel_t;
#endif
 public:
  explicit packed_reloc_iterator(decoder_t&& decoder)
      : decoder_(decoder) {
    // initialize fields
    memset(&reloc_, 0, sizeof(reloc_));
    relocation_count_ = decoder_.pop_front();
    reloc_.r_offset = decoder_.pop_front();
    relocation_index_ = 0;
    relocation_group_index_ = 0;
    group_size_ = 0;
  }

  bool has_next() const {
    return relocation_index_ < relocation_count_;
  }

  rel_t* next() {
    if (relocation_group_index_ == group_size_) {
      if (!read_group_fields()) {
        // Iterator is inconsistent state; it should not be called again
        // but in case it is let's make sure has_next() returns false.
        relocation_index_ = relocation_count_ = 0;
        return nullptr;
      }
    }

    if (RELOCATION_GROUPED_BY_OFFSET_DELTA(group_flags_)) {
      reloc_.r_offset += group_r_offset_delta_;
    } else {
      reloc_.r_offset += decoder_.pop_front();
    }

    if (!RELOCATION_GROUPED_BY_INFO(group_flags_)) {
      reloc_.r_info = decoder_.pop_front();
    }

#if defined(USE_RELA)
    if (RELOCATION_GROUP_HAS_ADDEND(group_flags_) && !RELOCATION_GROUPED_BY_ADDEND(group_flags_)) {
      reloc_.r_addend += decoder_.pop_front();
    }
#endif

    relocation_index_++;
    relocation_group_index_++;

    return &reloc_;
  }
 private:
  bool read_group_fields() {
    group_size_ = decoder_.pop_front();
    group_flags_ = decoder_.pop_front();

    if (RELOCATION_GROUPED_BY_OFFSET_DELTA(group_flags_)) {
      group_r_offset_delta_ = decoder_.pop_front();
    }

    if (RELOCATION_GROUPED_BY_INFO(group_flags_)) {
      reloc_.r_info = decoder_.pop_front();
    }

    if (RELOCATION_GROUP_HAS_ADDEND(group_flags_) && RELOCATION_GROUPED_BY_ADDEND(group_flags_)) {
#if !defined(USE_RELA)
      // This platform does not support rela, and yet we have it encoded in android_rel section.
      DL_ERR("unexpected r_addend in android.rel section");
      return false;
#else
      reloc_.r_addend += decoder_.pop_front();
    } else if (!RELOCATION_GROUP_HAS_ADDEND(group_flags_)) {
      reloc_.r_addend = 0;
#endif
    }

    relocation_group_index_ = 0;
    return true;
  }

  decoder_t decoder_;
  size_t relocation_count_;
  size_t group_size_;
  size_t group_flags_;
  size_t group_r_offset_delta_;
  size_t relocation_index_;
  size_t relocation_group_index_;
  rel_t reloc_;
};

#endif  // __LINKER_RELOC_ITERATORS_H
