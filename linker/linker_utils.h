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

#include <string>
#include <vector>

extern const char* const kZipFileSeparator;

void format_string(std::string* str, const std::vector<std::pair<std::string, std::string>>& params);

bool file_is_in_dir(const std::string& file, const std::string& dir);
bool file_is_under_dir(const std::string& file, const std::string& dir);
bool normalize_path(const char* path, std::string* normalized_path);
bool parse_zip_path(const char* input_path, std::string* zip_path, std::string* entry_path);

// For every path element this function checks of it exists, and is a directory,
// and normalizes it:
// 1. For regular path it converts it to realpath()
// 2. For path in a zip file it uses realpath on the zipfile
//    normalizes entry name by calling normalize_path function.
void resolve_paths(std::vector<std::string>& paths,
                   std::vector<std::string>* resolved_paths);
// Resolve a single path. Return empty string when the path is invalid or can't
// be resolved.
std::string resolve_path(const std::string& path);

void split_path(const char* path, const char* delimiters, std::vector<std::string>* paths);

std::string dirname(const char* path);

bool safe_add(off64_t* out, off64_t a, size_t b);
bool is_first_stage_init();
