/*
 * Copyright (C) 2017 The Android Open Source Project
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

#include <arpa/inet.h> // For ntohl(3).
#include <fcntl.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

#include "private/CachedProperty.h"

extern "C" void tzset_unlocked(void);
extern "C" int __bionic_open_tzdata(const char*, int32_t*);

extern "C" void tzsetlcl(char const*);

void tzset_unlocked() {
  // The TZ environment variable is meant to override the system-wide setting.
  const char* name = getenv("TZ");
  char buf[PROP_VALUE_MAX];

  // If that's not set, look at the "persist.sys.timezone" system property.
  if (name == nullptr) {
    static CachedProperty persist_sys_timezone("persist.sys.timezone");

    if ((name = persist_sys_timezone.Get()) != nullptr && strlen(name) > 3) {
      // POSIX and Java disagree about the sign in a timezone string. For POSIX, "GMT+3" means
      // "3 hours west/behind", but for Java it means "3 hours east/ahead". Since (a) Java is
      // the one that matches human expectations and (b) this system property is used directly
      // by Java, we flip the sign here to translate from Java to POSIX. http://b/25463955.
      char sign = name[3];
      if (sign == '-' || sign == '+') {
        strlcpy(buf, name, sizeof(buf));
        buf[3] = (sign == '-') ? '+' : '-';
        name = buf;
      }
    }
  }

  // If the system property is also not available (because you're running AOSP on a WiFi-only
  // device, say), fall back to GMT.
  if (name == nullptr) name = "GMT";

  tzsetlcl(name);
}

#if !defined(__ANDROID__)
static char* make_path(const char* path_prefix_variable,
                       const char* path_suffix) {
  const char* path_prefix = getenv(path_prefix_variable);
  if (path_prefix == nullptr) {
    fprintf(stderr, "%s: %s not set!\n", __FUNCTION__, path_prefix_variable);
    abort();
  }
  char* path;
  if (asprintf(&path, "%s/%s", path_prefix, path_suffix) == -1) {
    fprintf(stderr, "%s: couldn't allocate \"%s/%s\"\n", __FUNCTION__, path_prefix, path_suffix);
    abort();
  }
  return path;
}
#endif

// byte[12] tzdata_version  -- "tzdata2012f\0"
// int index_offset
// int data_offset
// int zonetab_offset
struct bionic_tzdata_header_t {
  char tzdata_version[12];
  int32_t index_offset;
  int32_t data_offset;
  int32_t zonetab_offset;
};
static constexpr size_t NAME_LENGTH = 40;
struct index_entry_t {
  char buf[NAME_LENGTH];
  int32_t start;
  int32_t length;
  int32_t unused; // Was raw GMT offset; always 0 since tzdata2014f (L).
};

static int __bionic_open_tzdata_path(const char* path,
                                     const char* olson_id,
                                     int32_t* entry_length) {
  int fd = TEMP_FAILURE_RETRY(open(path, O_RDONLY | O_CLOEXEC));
  if (fd == -1) {
    return -2; // Distinguish failure to find any data from failure to find a specific id.
  }

  bionic_tzdata_header_t header = {};
  ssize_t bytes_read = TEMP_FAILURE_RETRY(read(fd, &header, sizeof(header)));
  if (bytes_read != sizeof(header)) {
    fprintf(stderr, "%s: could not read header of \"%s\": %s\n",
            __FUNCTION__, path, (bytes_read == -1) ? strerror(errno) : "short read");
    close(fd);
    return -1;
  }

  if (strncmp(header.tzdata_version, "tzdata", 6) != 0 || header.tzdata_version[11] != 0) {
    fprintf(stderr, "%s: bad magic in \"%s\": \"%.6s\"\n", __FUNCTION__, path, header.tzdata_version);
    close(fd);
    return -1;
  }

  if (TEMP_FAILURE_RETRY(lseek(fd, ntohl(header.index_offset), SEEK_SET)) == -1) {
    fprintf(stderr, "%s: couldn't seek to index in \"%s\": %s\n", __FUNCTION__, path, strerror(errno));
    close(fd);
    return -1;
  }

  if (ntohl(header.data_offset) > ntohl(header.index_offset)) {
    fprintf(stderr, "%s: invalid data and index offsets in \"%s\": %u %u\n",
            __FUNCTION__, path, ntohl(header.data_offset), ntohl(header.index_offset));
    close(fd);
    return -1;
  }
  const size_t index_size = ntohl(header.data_offset) - ntohl(header.index_offset);
  if ((index_size % sizeof(index_entry_t)) != 0) {
    fprintf(stderr, "%s: invalid index size in \"%s\": %zd\n", __FUNCTION__, path, index_size);
    close(fd);
    return -1;
  }

  off_t specific_zone_offset = -1;
  char* index = reinterpret_cast<char*>(malloc(index_size));
  if (index == nullptr) {
    fprintf(stderr, "%s: couldn't allocate %zd-byte index for \"%s\"\n", __FUNCTION__, index_size, path);
    close(fd);
    return -1;
  }
  if (TEMP_FAILURE_RETRY(read(fd, index, index_size)) != static_cast<ssize_t>(index_size)) {
    fprintf(stderr, "%s: could not read index of \"%s\": %s\n",
            __FUNCTION__, path, (bytes_read == -1) ? strerror(errno) : "short read");
    free(index);
    close(fd);
    return -1;
  }

  size_t id_count = index_size / sizeof(index_entry_t);
  index_entry_t* entry = reinterpret_cast<index_entry_t*>(index);
  for (size_t i = 0; i < id_count; ++i) {
    char this_id[NAME_LENGTH + 1];
    memcpy(this_id, entry->buf, NAME_LENGTH);
    this_id[NAME_LENGTH] = '\0';

    if (strcmp(this_id, olson_id) == 0) {
      specific_zone_offset = ntohl(entry->start) + ntohl(header.data_offset);
      *entry_length = ntohl(entry->length);
      break;
    }

    ++entry;
  }
  free(index);

  if (specific_zone_offset == -1) {
    close(fd);
    return -1;
  }

  if (TEMP_FAILURE_RETRY(lseek(fd, specific_zone_offset, SEEK_SET)) == -1) {
    fprintf(stderr, "%s: could not seek to %ld in \"%s\": %s\n",
            __FUNCTION__, specific_zone_offset, path, strerror(errno));
    close(fd);
    return -1;
  }

  // TODO: check that there's TZ_MAGIC at this offset, so we can fall back to the other file if not.

  return fd;
}

int __bionic_open_tzdata(const char* olson_id, int32_t* entry_length) {
  int fd;

#if defined(__ANDROID__)
  // On Android, try the two hard-coded locations.
  fd = __bionic_open_tzdata_path("/data/misc/zoneinfo/current/tzdata",
                                 olson_id, entry_length);
  if (fd >= 0) return fd;

  fd = __bionic_open_tzdata_path("/system/usr/share/zoneinfo/tzdata",
                                 olson_id, entry_length);
  if (fd >= 0) return fd;
#else
  // On the host, we don't expect those locations to exist, and we're not
  // worried about security so we trust $ANDROID_DATA and $ANDROID_ROOT to
  // point us in the right direction.
  char* path = make_path("ANDROID_DATA", "/misc/zoneinfo/current/tzdata");
  fd = __bionic_open_tzdata_path(path, olson_id, entry_length);
  free(path);
  if (fd >= 0) return fd;

  path = make_path("ANDROID_ROOT", "/usr/share/zoneinfo/tzdata");
  fd = __bionic_open_tzdata_path(path, olson_id, entry_length);
  free(path);
  if (fd >= 0) return fd;
#endif

  // Not finding any tzdata is more serious that not finding a specific zone,
  // and worth logging.
  if (fd == -2) {
    // The first thing that 'recovery' does is try to format the current time. It doesn't have
    // any tzdata available, so we must not abort here --- doing so breaks the recovery image!
    fprintf(stderr, "%s: couldn't find any tzdata when looking for %s!\n", __FUNCTION__, olson_id);
  }

  return fd;
}
