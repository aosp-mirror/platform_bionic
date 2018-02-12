/*
 * Copyright (C) 2018 The Android Open Source Project
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

#ifndef GRP_PWD_FILE_H
#define GRP_PWD_FILE_H

#include <grp.h>
#include <pwd.h>

#include "private/bionic_lock.h"
#include "private/bionic_macros.h"
#include "private/grp_pwd.h"

class MmapFile {
 public:
  MmapFile(const char* filename);
  ~MmapFile();

  template <typename Line>
  bool FindById(uid_t uid, Line* line);
  template <typename Line>
  bool FindByName(const char* name, Line* line);

  DISALLOW_COPY_AND_ASSIGN(MmapFile);

 private:
  enum class FileStatus {
    Uninitialized,
    Initialized,
    Error,
  };

  bool GetFile(const char** start, const char** end);
  bool DoMmap();

  template <typename Line, typename Predicate>
  bool Find(Line* line, Predicate predicate);

  FileStatus status_ = FileStatus::Uninitialized;
  Lock lock_;
  const char* filename_ = nullptr;
  const char* start_ = nullptr;
  const char* end_ = nullptr;
};

class PasswdFile {
 public:
  PasswdFile(const char* filename);

  bool FindById(uid_t id, passwd_state_t* passwd_state);
  bool FindByName(const char* name, passwd_state_t* passwd_state);

  DISALLOW_COPY_AND_ASSIGN(PasswdFile);

 private:
  MmapFile mmap_file_;
};

class GroupFile {
 public:
  GroupFile(const char* filename);

  bool FindById(gid_t id, group_state_t* group_state);
  bool FindByName(const char* name, group_state_t* group_state);

  DISALLOW_COPY_AND_ASSIGN(GroupFile);

 private:
  MmapFile mmap_file_;
};

#endif
