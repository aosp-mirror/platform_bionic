/*
 * Copyright (C) 2021 The Android Open Source Project
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

#include <gtest/gtest.h>

#include <dlfcn.h>
#include <execinfo.h>
#include <string.h>

#include <fstream>
#include <regex>
#include <string>
#include <vector>

#include <android-base/file.h>
#include <android-base/stringprintf.h>
#include <android-base/strings.h>
#include <android-base/test_utils.h>

TEST(execinfo, backtrace_errors) {
  void* frames[20];
  ASSERT_EQ(0, backtrace(frames, 0));
  ASSERT_EQ(0, backtrace(frames, -1));
}

static constexpr int kMaxFrames = 50;

// Disable optimizations so that these functions show up properly in
// the backtrace.
#pragma clang optimize off
extern "C" __attribute__((__noinline__)) void CallTwo(std::vector<void*>& frames) {
  int num_frames = backtrace(frames.data(), static_cast<int>(frames.size()));
  ASSERT_LT(0, num_frames);
  frames.resize(static_cast<size_t>(num_frames));
}

extern "C" __attribute__((__noinline__)) void CallOne(std::vector<void*>& frames) {
  CallTwo(frames);
}
#pragma clang optimize on

static std::string DumpFrames(std::vector<void*>& frames) {
  std::string frame_data;
  for (auto frame : frames) {
    frame_data += android::base::StringPrintf("[%p]", frame);
    Dl_info info;
    if (dladdr(frame, &info) != 0 && info.dli_sname != nullptr) {
      frame_data += ' ';
      frame_data += info.dli_sname;
    }
    frame_data += '\n';
  }
  return frame_data;
}

static size_t FindFunction(std::vector<void*>& frames, uintptr_t func_addr) {
  Dl_info func_info;
  if (!dladdr(reinterpret_cast<void*>(func_addr), &func_info)) {
    return 0;
  }
  for (size_t i = 0; i < frames.size(); i++) {
    Dl_info frame_info;
    if (dladdr(frames[i], &frame_info) && func_info.dli_saddr == frame_info.dli_saddr) {
      return i + 1;
    }
  }
  return 0;
}

static void VerifyCalls(std::vector<void*>& frames, size_t* one_idx = nullptr,
                        size_t* two_idx = nullptr) {
  // Try and find the CallOne and CallTwo function addresses.
  size_t call_one_idx = FindFunction(frames, reinterpret_cast<uintptr_t>(&CallOne));
  ASSERT_TRUE(call_one_idx != 0) << DumpFrames(frames);
  size_t call_two_idx = FindFunction(frames, reinterpret_cast<uintptr_t>(&CallTwo));
  ASSERT_TRUE(call_two_idx != 0) << DumpFrames(frames);

  ASSERT_LT(call_two_idx, call_one_idx) << "CallTwo function found after CallOne\n"
                                        << DumpFrames(frames);

  if (one_idx != nullptr) *one_idx = call_one_idx;
  if (two_idx != nullptr) *two_idx = call_two_idx;
}

TEST(execinfo, backtrace) {
  std::vector<void*> frames(kMaxFrames);
  ASSERT_NO_FATAL_FAILURE(CallOne(frames));

  // Verfiy that there are at least two frames.
  ASSERT_LT(3U, frames.size()) << DumpFrames(frames);

  VerifyCalls(frames);
}

TEST(execinfo, backtrace_cutoff_frames) {
  // Verify the max frames is handled properly
  std::vector<void*> frames(1);
  ASSERT_NO_FATAL_FAILURE(CallOne(frames));
  ASSERT_EQ(1U, frames.size()) << DumpFrames(frames);
}

TEST(execinfo, backtrace_symbols_errors) {
  void* frames[kMaxFrames];
  // glibc incorrectly returns memory when a zero is passed in.
  // Since we know this works properly on bionic, only verify
  // this there.
#if defined(__BIONIC__)
  ASSERT_EQ(nullptr, backtrace_symbols(frames, 0));
#endif
  ASSERT_EQ(nullptr, backtrace_symbols(frames, -1));
}

static void VerifyLineFormat(std::string& line) {
  // Verify that the format of the line is one of these:
  //   elf_file(FuncName+0xFuncAddr) [0xAddress]
  //   elf_file(+0xRelAddress) [0xAddress]
  //   elf_file [0xAddress]
  //   [0xAddress]
#if defined(__GLIBC__)
  // For some reason, glibc will print a space before [0xAddress] for
  // backtrace symbols, and no space for backtrace_symbols_fd. Allow this
  // only for glibc.
  std::regex format1("[^\\(\\s]+\\([^\\+]+\\+0x[0-9a-fA-F]+\\) ?\\[0x[0-9a-fA-F]+\\]");
  std::regex format2("[^\\(\\s]+\\(+\\+0x[0-9a-fA-F]+\\) ?\\[0x[0-9a-fA-F]+\\]");
  std::regex format3("[^\\(\\s]+ ?\\[0x[0-9a-fA-F]+\\]");
#else
  std::regex format1("[^\\(\\s]+\\([^\\+]+\\+0x[0-9a-fA-F]+\\) \\[0x[0-9a-fA-F]+\\]");
  std::regex format2("[^\\(\\s]+\\(+\\+0x[0-9a-fA-F]+\\) \\[0x[0-9a-fA-F]+\\]");
  std::regex format3("[^\\(\\s]+ \\[0x[0-9a-fA-F]+\\]");
#endif
  std::regex format4("\\[0x[0-9a-fA-F]+\\]");

  EXPECT_TRUE(std::regex_match(line, format1) || std::regex_match(line, format2) ||
              std::regex_match(line, format3) || std::regex_match(line, format4))
      << "Unknown format of line:\n"
      << line;
}

static void VerifyLineFormat(char* raw_line, size_t length) {
  std::string line(raw_line, length);
  VerifyLineFormat(line);
}

TEST(execinfo, backtrace_symbols) {
  std::vector<void*> frames(kMaxFrames);
  ASSERT_NO_FATAL_FAILURE(CallOne(frames));
  ASSERT_LT(3U, frames.size()) << DumpFrames(frames);

  char** symbols = backtrace_symbols(frames.data(), static_cast<int>(frames.size()));
  ASSERT_TRUE(symbols != nullptr);
  for (size_t i = 0; i < frames.size(); i++) {
    ASSERT_TRUE(frames[i] != nullptr);
    VerifyLineFormat(symbols[i], strlen(symbols[i]));
  }

  size_t call_one_idx;
  size_t call_two_idx;
  ASSERT_NO_FATAL_FAILURE(VerifyCalls(frames, &call_one_idx, &call_two_idx));
  // Now verify that those frames contain the function names we expect.
  SCOPED_TRACE(DumpFrames(frames));
  ASSERT_MATCH(symbols[call_one_idx - 1], "\\(CallOne+");
  ASSERT_MATCH(symbols[call_two_idx - 1], "\\(CallTwo+");
  free(symbols);
}

TEST(execinfo, backtrace_symbols_fd_errors) {
  void* frames[kMaxFrames];
  frames[0] = reinterpret_cast<void*>(&backtrace_symbols);

  {
    TemporaryFile tf;
    backtrace_symbols_fd(frames, 0, tf.fd);
    close(tf.fd);
    std::string content;
    ASSERT_TRUE(android::base::ReadFileToString(tf.path, &content));
    // Verify that no data is written to the file.
    ASSERT_TRUE(content.empty());
  }

  {
    TemporaryFile tf;
    backtrace_symbols_fd(frames, -1, tf.fd);
    close(tf.fd);
    std::string content;
    ASSERT_TRUE(android::base::ReadFileToString(tf.path, &content));
    // Verify that no data is written to the file.
    ASSERT_TRUE(content.empty());
  }

  // Verify that there isn't a crash.
  backtrace_symbols_fd(frames, 0, -1);
}

TEST(execinfo, backtrace_symbols_fd) {
  std::vector<void*> frames(kMaxFrames);
  ASSERT_NO_FATAL_FAILURE(CallOne(frames));
  ASSERT_LT(3U, frames.size()) << DumpFrames(frames);

  TemporaryFile tf;
  backtrace_symbols_fd(frames.data(), static_cast<int>(frames.size()), tf.fd);
  close(tf.fd);

  size_t call_one_idx;
  size_t call_two_idx;
  ASSERT_NO_FATAL_FAILURE(VerifyCalls(frames, &call_one_idx, &call_two_idx));

  std::ifstream frame_stream(tf.path);
  ASSERT_TRUE(frame_stream.is_open());
  size_t num_lines = 0;
  std::string line;
  while (std::getline(frame_stream, line)) {
    ASSERT_FALSE(line.empty());
    VerifyLineFormat(line);
    num_lines++;

    if (num_lines == call_one_idx) {
      EXPECT_MATCH(line, "\\(CallOne+");
    } else if (num_lines == call_two_idx) {
      EXPECT_MATCH(line, "\\(CallTwo+");
    }
  }
  ASSERT_EQ(num_lines, frames.size()) << "Number of lines in file does not match number of frames.";
}
