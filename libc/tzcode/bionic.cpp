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
#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

#include "private/CachedProperty.h"

extern "C" void tzset_unlocked(void);
extern "C" void __bionic_get_system_tz(char* buf, size_t n);
extern "C" int __bionic_open_tzdata(const char*, int32_t*);

extern "C" void tzsetlcl(char const*);

void __bionic_get_system_tz(char* buf, size_t n) {
  static CachedProperty persist_sys_timezone("persist.sys.timezone");
  const char* name = persist_sys_timezone.Get();

  // If the system property is not set, perhaps because this is called
  // before the default value has been set (the recovery image being a
  // classic example), fall back to GMT.
  if (name == nullptr) name = "GMT";

  strlcpy(buf, name, n);

  if (!strcmp(buf, "GMT")) {
    // Typically we'll set the system property to an Olson ID, but
    // java.util.TimeZone also supports the "GMT+xxxx" style, and at
    // least historically (see http://b/25463955) some Android-based set
    // top boxes would get the timezone from the TV network in this format
    // and use it directly in the system property. This caused trouble
    // for native code because POSIX and Java disagree about the sign in
    // a timezone string. For POSIX, "GMT+3" means "3 hours west/behind",
    // but for Java it means "3 hours east/ahead". Since (a) Java is the
    // one that matches human expectations and (b) this system property is
    // used directly by Java, we flip the sign here to translate from Java
    // to POSIX. We only need to worry about the "GMT+xxxx" case because
    // the expectation is that these are valid java.util.TimeZone ids,
    // not general POSIX custom timezone specifications (which is why this
    // code only applies to the system property, and not to the environment
    // variable).
    char sign = buf[3];
    if (sign == '-' || sign == '+') {
      buf[3] = (sign == '-') ? '+' : '-';
    }
  }
}

void tzset_unlocked() {
  // The TZ environment variable is meant to override the system-wide setting.
  const char* name = getenv("TZ");
  char buf[PROP_VALUE_MAX];

  // If that's not set, look at the "persist.sys.timezone" system property.
  if (name == nullptr) {
    __bionic_get_system_tz(buf, sizeof(buf));
    name = buf;
  }

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
// int final_offset
struct bionic_tzdata_header_t {
  char tzdata_version[12];
  int32_t index_offset;
  int32_t data_offset;
  int32_t final_offset;
};
static constexpr size_t NAME_LENGTH = 40;
struct index_entry_t {
  char buf[NAME_LENGTH];
  int32_t start;
  int32_t length;
  int32_t unused; // Was raw GMT offset; always 0 since tzdata2014f (L).
};

// Returns -2 for a soft failure (where the caller should try another file),
// -1 for a hard failure (where the caller should give up), and >= 0 is a
// file descriptor whose offset points to the data for the given olson id in
// the given file (and *entry_length is the size of the data).
static int __bionic_open_tzdata_path(const char* path,
                                     const char* olson_id,
                                     int32_t* entry_length) {
  int fd = TEMP_FAILURE_RETRY(open(path, O_RDONLY | O_CLOEXEC));
  if (fd == -1) {
    // We don't log here, because this is quite common --- current devices
    // aren't expected to have the old APK tzdata, for example.
    return -2;
  }

  bionic_tzdata_header_t header = {};
  ssize_t bytes_read = TEMP_FAILURE_RETRY(read(fd, &header, sizeof(header)));
  if (bytes_read != sizeof(header)) {
    fprintf(stderr, "%s: could not read header of \"%s\": %s\n",
            __FUNCTION__, path, (bytes_read == -1) ? strerror(errno) : "short read");
    close(fd);
    return -2;
  }

  if (strncmp(header.tzdata_version, "tzdata", 6) != 0 || header.tzdata_version[11] != 0) {
    fprintf(stderr, "%s: bad magic in \"%s\": \"%.6s\"\n", __FUNCTION__, path, header.tzdata_version);
    close(fd);
    return -2;
  }

  if (TEMP_FAILURE_RETRY(lseek(fd, ntohl(header.index_offset), SEEK_SET)) == -1) {
    fprintf(stderr, "%s: couldn't seek to index in \"%s\": %s\n", __FUNCTION__, path, strerror(errno));
    close(fd);
    return -2;
  }

  if (ntohl(header.index_offset) > ntohl(header.data_offset)) {
    fprintf(stderr, "%s: invalid data and index offsets in \"%s\": %u %u\n",
            __FUNCTION__, path, ntohl(header.data_offset), ntohl(header.index_offset));
    close(fd);
    return -2;
  }
  const size_t index_size = ntohl(header.data_offset) - ntohl(header.index_offset);
  if ((index_size % sizeof(index_entry_t)) != 0) {
    fprintf(stderr, "%s: invalid index size in \"%s\": %zd\n", __FUNCTION__, path, index_size);
    close(fd);
    return -2;
  }

  char* index = reinterpret_cast<char*>(malloc(index_size));
  if (index == nullptr) {
    fprintf(stderr, "%s: couldn't allocate %zd-byte index for \"%s\"\n", __FUNCTION__, index_size, path);
    close(fd);
    return -2;
  }
  if (TEMP_FAILURE_RETRY(read(fd, index, index_size)) != static_cast<ssize_t>(index_size)) {
    fprintf(stderr, "%s: could not read index of \"%s\": %s\n",
            __FUNCTION__, path, (bytes_read == -1) ? strerror(errno) : "short read");
    free(index);
    close(fd);
    return -2;
  }

  off_t specific_zone_offset = -1;
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
    // We found a valid tzdata file, but didn't find the requested id in it.
    // Give up now, and don't try fallback tzdata files. We don't log here
    // because for all we know the given olson id was nonsense.
    close(fd);
    // This file descriptor (-1) is passed to localtime.c. In invalid fd case
    // upstream passes errno value around methods and having 0 there will
    // indicate that timezone was found and read successfully and localtime's
    // internal state was properly initialized (which wasn't as we couldn't find
    // requested timezone in the tzdata file).
    // If we reached this point errno is unlikely to be touched. It is only
    // close(fd) which can do it, but that is very unlikely to happen. And
    // even if it happens we can't extract any useful insights from it.
    // We are overriding it to ENOENT as it matches upstream expectations -
    // timezone is absent in the tzdata file == there is no TZif file in
    // /usr/share/zoneinfo.
    errno = ENOENT;
    return -1;
  }

  if (TEMP_FAILURE_RETRY(lseek(fd, specific_zone_offset, SEEK_SET)) == -1) {
    fprintf(stderr, "%s: could not seek to %ld in \"%s\": %s\n",
            __FUNCTION__, specific_zone_offset, path, strerror(errno));
    close(fd);
    return -2;
  }

  return fd;
}

int __bionic_open_tzdata(const char* olson_id, int32_t* entry_length) {
  int fd;

  // Try the two locations for the tzdata file in a strict order:
  // 1: The timezone data module which contains the main copy. This is the
  //    common case for current devices.
  // 2: The ultimate fallback: the non-updatable copy in /system.

#if defined(__ANDROID__)
  // On Android devices, bionic has to work even if exec takes place without
  // environment variables set. So, all paths are hardcoded here.
  fd = __bionic_open_tzdata_path("/apex/com.android.tzdata/etc/tz/tzdata",
                                 olson_id, entry_length);
  if (fd >= -1) return fd;

  fd = __bionic_open_tzdata_path("/system/usr/share/zoneinfo/tzdata",
                                 olson_id, entry_length);
  if (fd >= -1) return fd;
#else
  // On the host, we don't expect the hard-coded locations above to exist, and
  // we're not worried about security so we trust $ANDROID_TZDATA_ROOT, and
  // $ANDROID_ROOT to point us in the right direction instead.

  char* path = make_path("ANDROID_TZDATA_ROOT", "/etc/tz/tzdata");
  fd = __bionic_open_tzdata_path(path, olson_id, entry_length);
  free(path);
  if (fd >= -1) return fd;

  path = make_path("ANDROID_ROOT", "/usr/share/zoneinfo/tzdata");
  fd = __bionic_open_tzdata_path(path, olson_id, entry_length);
  free(path);
  if (fd >= -1) return fd;
#endif

  // Not finding any tzdata is more serious that not finding a specific zone,
  // and worth logging.
  if (fd == -2) {
    // The first thing that 'recovery' does is try to format the current time. It doesn't have
    // any tzdata available, so we must not abort here --- doing so breaks the recovery image!
    fprintf(stderr, "%s: couldn't find any tzdata when looking for %s!\n", __FUNCTION__, olson_id);
  }

  // Otherwise we were successful.
  return fd;
}
