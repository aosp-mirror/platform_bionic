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

#ifndef __LINKER_NAMESPACES_H
#define __LINKER_NAMESPACES_H

#include "linker_common_types.h"

#include <string>
#include <vector>

struct android_namespace_t {
 public:
  android_namespace_t() : name_(nullptr), is_isolated_(false) {}

  const char* get_name() const { return name_; }
  void set_name(const char* name) { name_ = name; }

  bool is_isolated() const { return is_isolated_; }
  void set_isolated(bool isolated) { is_isolated_ = isolated; }

  const std::vector<std::string>& get_ld_library_paths() const {
    return ld_library_paths_;
  }
  void set_ld_library_paths(std::vector<std::string>&& library_paths) {
    ld_library_paths_ = library_paths;
  }

  const std::vector<std::string>& get_default_library_paths() const {
    return default_library_paths_;
  }
  void set_default_library_paths(std::vector<std::string>&& library_paths) {
    default_library_paths_ = library_paths;
  }

  const std::vector<std::string>& get_permitted_paths() const {
    return permitted_paths_;
  }
  void set_permitted_paths(std::vector<std::string>&& permitted_paths) {
    permitted_paths_ = permitted_paths;
  }

  void add_soinfo(soinfo* si) {
    soinfo_list_.push_back(si);
  }

  void add_soinfos(const soinfo_list_t& soinfos) {
    for (auto si : soinfos) {
      add_soinfo(si);
    }
  }

  void remove_soinfo(soinfo* si) {
    soinfo_list_.remove_if([&](soinfo* candidate) {
      return si == candidate;
    });
  }

  const soinfo_list_t& soinfo_list() const { return soinfo_list_; }

  // For isolated namespaces - checks if the file is on the search path;
  // always returns true for not isolated namespace.
  bool is_accessible(const std::string& path);

 private:
  const char* name_;
  bool is_isolated_;
  std::vector<std::string> ld_library_paths_;
  std::vector<std::string> default_library_paths_;
  std::vector<std::string> permitted_paths_;
  soinfo_list_t soinfo_list_;

  DISALLOW_COPY_AND_ASSIGN(android_namespace_t);
};

#endif  /* __LINKER_NAMESPACES_H */
