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
#ifndef __LINKER_UTILS_H
#define __LINKER_UTILS_H

#include <string>
#include <vector>

extern const char* const kZipFileSeparator;

bool file_is_in_dir(const std::string& file, const std::string& dir);
bool file_is_under_dir(const std::string& file, const std::string& dir);
bool normalize_path(const char* path, std::string* normalized_path);
bool parse_zip_path(const char* input_path, std::string* zip_path, std::string* entry_path);

// For every path element this function checks of it exists, and is a directory,
// and normalizes it:
// 1. For regular path it converts it to realpath()
// 2. For path in a zip file it uses realpath on the zipfile
//    normalizes entry name by calling normalize_path function.
void resolve_paths(std::vector<std::string>& paths, std::vector<std::string>* resolved_paths);
void split_path(const char* path, const char* delimiters, std::vector<std::string>* paths);

std::string dirname(const char* path);

off64_t page_start(off64_t offset);
size_t page_offset(off64_t offset);
bool safe_add(off64_t* out, off64_t a, size_t b);

#endif
