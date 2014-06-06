/*
 * Copyright (C) 2008 The Android Open Source Project
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

#include <dirent.h>

#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "private/ErrnoRestorer.h"
#include "private/ScopedPthreadMutexLocker.h"

extern "C" int __getdents64(unsigned int, dirent*, unsigned int);

struct DIR {
  int fd_;
  size_t available_bytes_;
  dirent* next_;
  pthread_mutex_t mutex_;
  dirent buff_[15];
};

static DIR* __allocate_DIR(int fd) {
  DIR* d = reinterpret_cast<DIR*>(malloc(sizeof(DIR)));
  if (d == NULL) {
    return NULL;
  }
  d->fd_ = fd;
  d->available_bytes_ = 0;
  d->next_ = NULL;
  pthread_mutex_init(&d->mutex_, NULL);
  return d;
}

int dirfd(DIR* dirp) {
  return dirp->fd_;
}

DIR* fdopendir(int fd) {
  // Is 'fd' actually a directory?
  struct stat sb;
  if (fstat(fd, &sb) == -1) {
    return NULL;
  }
  if (!S_ISDIR(sb.st_mode)) {
    errno = ENOTDIR;
    return NULL;
  }

  return __allocate_DIR(fd);
}

DIR* opendir(const char* path) {
  int fd = open(path, O_RDONLY | O_DIRECTORY);
  return (fd != -1) ? __allocate_DIR(fd) : NULL;
}

static bool __fill_DIR(DIR* d) {
  int rc = TEMP_FAILURE_RETRY(__getdents64(d->fd_, d->buff_, sizeof(d->buff_)));
  if (rc <= 0) {
    return false;
  }
  d->available_bytes_ = rc;
  d->next_ = d->buff_;
  return true;
}

static dirent* __readdir_locked(DIR* d) {
  if (d->available_bytes_ == 0 && !__fill_DIR(d)) {
    return NULL;
  }

  dirent* entry = d->next_;
  d->next_ = reinterpret_cast<dirent*>(reinterpret_cast<char*>(entry) + entry->d_reclen);
  d->available_bytes_ -= entry->d_reclen;
  return entry;
}

dirent* readdir(DIR* d) {
  ScopedPthreadMutexLocker locker(&d->mutex_);
  return __readdir_locked(d);
}
__strong_alias(readdir64, readdir);

int readdir_r(DIR* d, dirent* entry, dirent** result) {
  ErrnoRestorer errno_restorer;

  *result = NULL;
  errno = 0;

  ScopedPthreadMutexLocker locker(&d->mutex_);

  dirent* next = __readdir_locked(d);
  if (errno != 0 && next == NULL) {
    return errno;
  }

  if (next != NULL) {
    memcpy(entry, next, next->d_reclen);
    *result = entry;
  }
  return 0;
}
__strong_alias(readdir64_r, readdir_r);

int closedir(DIR* d) {
  if (d == NULL) {
    errno = EINVAL;
    return -1;
  }

  int fd = d->fd_;
  pthread_mutex_destroy(&d->mutex_);
  free(d);
  return close(fd);
}

void rewinddir(DIR* d) {
  ScopedPthreadMutexLocker locker(&d->mutex_);
  lseek(d->fd_, 0, SEEK_SET);
  d->available_bytes_ = 0;
}

int alphasort(const dirent** a, const dirent** b) {
  return strcoll((*a)->d_name, (*b)->d_name);
}
__strong_alias(alphasort64, alphasort);
