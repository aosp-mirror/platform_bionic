/*
 * Copyright (C) 2016 The Android Open Source Project
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

#include "linker_namespaces.h"
#include "linker_soinfo.h"
#include "linker_utils.h"

#include <vector>

bool android_namespace_t::is_accessible(const std::string& file) {
  if (!is_isolated_) {
    return true;
  }

  for (const auto& dir : ld_library_paths_) {
    if (file_is_in_dir(file, dir)) {
      return true;
    }
  }

  for (const auto& dir : default_library_paths_) {
    if (file_is_in_dir(file, dir)) {
      return true;
    }
  }

  for (const auto& dir : permitted_paths_) {
    if (file_is_under_dir(file, dir)) {
      return true;
    }
  }

  return false;
}

bool android_namespace_t::is_accessible(soinfo* s) {
  std::vector<soinfo*> soinfos;
  soinfos.push_back(s);
  s->get_parents().for_each([&](soinfo* parent_si) {
    soinfos.push_back(parent_si);
  });

  return std::find_if(soinfos.begin(), soinfos.end(), [this](soinfo* si) {
    if (si->get_primary_namespace() == this) {
      return true;
    }

    const android_namespace_list_t& secondary_namespaces = si->get_secondary_namespaces();
    if (secondary_namespaces.find(this) != secondary_namespaces.end()) {
      return true;
    }

    return false;
  }) != soinfos.end();
}
