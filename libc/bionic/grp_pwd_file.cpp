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

#include "grp_pwd_file.h"

#include <fcntl.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>

#include "private/ErrnoRestorer.h"

// This file mmap's /*/etc/passwd and /*/etc/group in order to return their contents without any
// allocations.  Note that these files and the strings contained within them are explicitly not
// null-terminated.  ':'s are used to deliminate fields and '\n's are used to deliminate lines.
// There is a check that the file ends with '\n', such that terminating loops at '\n' ensures that
// memory will be not ready before the mmap region.

namespace {

void CopyFieldToString(char* dest, const char* source, size_t max) {
  while (*source != ':' && *source != '\n' && max > 1) {
    *dest++ = *source++;
    --max;
  }
  *dest = '\0';
}

bool FieldToUid(const char* field, uid_t* uid) {
  if (field == nullptr) {
    return false;
  }

  char* end = nullptr;
  errno = 0;
  uid_t result = strtoul(field, &end, 0);
  if (errno != 0 || field == end || *end != ':') {
    return false;
  }
  *uid = result;
  return true;
}

// Returns a pointer to one past the end of line.
const char* ParseLine(const char* begin, const char* end, const char** fields, size_t num_fields) {
  size_t fields_written = 0;
  const char* position = begin;
  fields[fields_written++] = position;

  while (position < end && fields_written < num_fields) {
    if (*position == '\n') {
      return position + 1;
    }
    if (*position == ':') {
      fields[fields_written++] = position + 1;
    }
    position++;
  }

  while (position < end && *position != '\n') {
    position++;
  }

  return position + 1;
}

struct PasswdLine {
  const char* name() const {
    return fields[0];
  }
  // Password is not supported.
  const char* uid() const {
    return fields[2];
  }
  const char* gid() const {
    return fields[3];
  }
  // User Info is not supported
  const char* dir() const {
    return fields[5];
  }
  const char* shell() const {
    return fields[6];
  }

  bool ToPasswdState(passwd_state_t* passwd_state) {
    if (name() == nullptr || dir() == nullptr || shell() == nullptr) {
      return false;
    }

    uid_t uid;
    if (!FieldToUid(this->uid(), &uid)) {
      return false;
    }

    gid_t gid;
    if (!FieldToUid(this->gid(), &gid)) {
      return false;
    }

    passwd_state->passwd_.pw_uid = uid;
    passwd_state->passwd_.pw_gid = gid;

    CopyFieldToString(passwd_state->name_buffer_, name(), sizeof(passwd_state->name_buffer_));
    passwd_state->passwd_.pw_name = passwd_state->name_buffer_;

    passwd_state->passwd_.pw_passwd = nullptr;

#ifdef __LP64__
    passwd_state->passwd_.pw_gecos = nullptr;
#endif

    CopyFieldToString(passwd_state->dir_buffer_, dir(), sizeof(passwd_state->dir_buffer_));
    passwd_state->passwd_.pw_dir = passwd_state->dir_buffer_;

    CopyFieldToString(passwd_state->sh_buffer_, shell(), sizeof(passwd_state->sh_buffer_));
    passwd_state->passwd_.pw_shell = passwd_state->sh_buffer_;

    return true;
  }

  static constexpr size_t kNumFields = 7;
  const char* fields[kNumFields] = {};
};

struct GroupLine {
  const char* name() const {
    return fields[0];
  }
  // Password is not supported.
  const char* gid() const {
    return fields[2];
  }
  // User list is not supported (returns simply name)

  bool ToGroupState(group_state_t* group_state) {
    if (name() == nullptr || gid() == nullptr) {
      return false;
    }

    gid_t gid;
    if (!FieldToUid(this->gid(), &gid)) {
      return false;
    }

    group_state->group_.gr_gid = gid;

    CopyFieldToString(group_state->group_name_buffer_, name(),
                      sizeof(group_state->group_name_buffer_));
    group_state->group_.gr_name = group_state->group_name_buffer_;

    group_state->group_.gr_passwd = nullptr;

    group_state->group_.gr_mem = group_state->group_members_;
    group_state->group_.gr_mem[0] = group_state->group_.gr_name;
    group_state->group_.gr_mem[1] = nullptr;

    return true;
  }

  static constexpr size_t kNumFields = 4;
  const char* fields[kNumFields] = {};
};

}  // namespace

MmapFile::MmapFile(const char* filename) : filename_(filename) {
  lock_.init(false);
}

MmapFile::~MmapFile() {
  if (status_ == FileStatus::Initialized) {
    size_t size = end_ - start_ + 1;
    munmap(const_cast<char*>(start_), size);
  }
}

bool MmapFile::GetFile(const char** start, const char** end) {
  LockGuard guard(lock_);
  if (status_ == FileStatus::Initialized) {
    *start = start_;
    *end = end_;
    return true;
  }
  if (status_ == FileStatus::Error) {
    return false;
  }

  if (!DoMmap()) {
    status_ = FileStatus::Error;
    return false;
  }

  status_ = FileStatus::Initialized;
  *start = start_;
  *end = end_;
  return true;
}

bool MmapFile::DoMmap() {
  int fd = open(filename_, O_CLOEXEC | O_NOFOLLOW | O_RDONLY);

  struct stat fd_stat;
  if (fstat(fd, &fd_stat) == -1) {
    close(fd);
    return false;
  }

  auto mmap_size = fd_stat.st_size;

  const void* map_result = mmap(nullptr, mmap_size, PROT_READ, MAP_SHARED, fd, 0);
  close(fd);

  if (map_result == MAP_FAILED) {
    return false;
  }

  start_ = static_cast<const char*>(map_result);
  end_ = start_ + mmap_size - 1;

  return *end_ == '\n';
}

template <typename Line, typename Predicate>
bool MmapFile::Find(Line* line, Predicate predicate) {
  const char* start;
  const char* end;
  if (!GetFile(&start, &end)) {
    return false;
  }

  const char* line_beginning = start;

  while (line_beginning < end) {
    line_beginning = ParseLine(line_beginning, end, line->fields, line->kNumFields);
    if (predicate(line)) return true;
  }

  return false;
}

template <typename Line>
bool MmapFile::FindById(uid_t uid, Line* line) {
  return Find(line, [uid](const auto& line) {
    uid_t line_id;
    if (!FieldToUid(line->fields[2], &line_id)) {
      return false;
    }

    return line_id == uid;
  });
}

template <typename Line>
bool MmapFile::FindByName(const char* name, Line* line) {
  return Find(line, [name](const auto& line) {
    const char* line_name = line->fields[0];
    if (line_name == nullptr) {
      return false;
    }

    const char* match_name = name;
    while (*line_name != '\n' && *line_name != ':' && *match_name != '\0') {
      if (*line_name++ != *match_name++) {
        return false;
      }
    }

    return *line_name == ':' && *match_name == '\0';
  });
}

PasswdFile::PasswdFile(const char* filename) : mmap_file_(filename) {
}

bool PasswdFile::FindById(uid_t id, passwd_state_t* passwd_state) {
  ErrnoRestorer errno_restorer;
  PasswdLine passwd_line;
  return mmap_file_.FindById(id, &passwd_line) && passwd_line.ToPasswdState(passwd_state);
}

bool PasswdFile::FindByName(const char* name, passwd_state_t* passwd_state) {
  ErrnoRestorer errno_restorer;
  PasswdLine passwd_line;
  return mmap_file_.FindByName(name, &passwd_line) && passwd_line.ToPasswdState(passwd_state);
}

GroupFile::GroupFile(const char* filename) : mmap_file_(filename) {
}

bool GroupFile::FindById(gid_t id, group_state_t* group_state) {
  ErrnoRestorer errno_restorer;
  GroupLine group_line;
  return mmap_file_.FindById(id, &group_line) && group_line.ToGroupState(group_state);
}

bool GroupFile::FindByName(const char* name, group_state_t* group_state) {
  ErrnoRestorer errno_restorer;
  GroupLine group_line;
  return mmap_file_.FindByName(name, &group_line) && group_line.ToGroupState(group_state);
}
