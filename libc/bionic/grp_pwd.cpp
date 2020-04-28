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

#include <ctype.h>
#include <errno.h>
#include <grp.h>
#include <mntent.h>
#include <pthread.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "private/android_filesystem_config.h"
#include "private/bionic_macros.h"
#include "private/grp_pwd.h"
#include "private/ErrnoRestorer.h"

// Generated android_ids array
#include "generated_android_ids.h"
#include "grp_pwd_file.h"

static PasswdFile vendor_passwd("/vendor/etc/passwd", "vendor_");
static GroupFile vendor_group("/vendor/etc/group", "vendor_");

// POSIX seems to envisage an implementation where the <pwd.h> functions are
// implemented by brute-force searching with getpwent(3), and the <grp.h>
// functions are implemented similarly with getgrent(3). This means that it's
// okay for all the <grp.h> functions to share state, and all the <passwd.h>
// functions to share state, but <grp.h> functions can't clobber <passwd.h>
// functions' state and vice versa.
#include "bionic/pthread_internal.h"
static group_state_t* get_group_tls_buffer() {
  return &__get_bionic_tls().group;
}

static passwd_state_t* get_passwd_tls_buffer() {
  return &__get_bionic_tls().passwd;
}

static void init_group_state(group_state_t* state) {
  memset(state, 0, sizeof(group_state_t) - sizeof(state->getgrent_idx));
  state->group_.gr_mem = state->group_members_;
}

static group_state_t* __group_state() {
  group_state_t* result = get_group_tls_buffer();
  if (result != nullptr) {
    init_group_state(result);
  }
  return result;
}

static int do_getpw_r(int by_name, const char* name, uid_t uid,
                      passwd* dst, char* buf, size_t byte_count,
                      passwd** result) {
  // getpwnam_r and getpwuid_r don't modify errno, but library calls we
  // make might.
  ErrnoRestorer errno_restorer;
  *result = nullptr;

  // Our implementation of getpwnam(3) and getpwuid(3) use thread-local
  // storage, so we can call them as long as we copy everything out
  // before returning.
  const passwd* src = by_name ? getpwnam(name) : getpwuid(uid); // NOLINT: see above.

  // POSIX allows failure to find a match to be considered a non-error.
  // Reporting success (0) but with *result NULL is glibc's behavior.
  if (src == nullptr) {
    return (errno == ENOENT) ? 0 : errno;
  }

  // Work out where our strings will go in 'buf', and whether we've got
  // enough space.
  size_t required_byte_count = 0;
  dst->pw_name = buf;
  required_byte_count += strlen(src->pw_name) + 1;
  dst->pw_dir = buf + required_byte_count;
  required_byte_count += strlen(src->pw_dir) + 1;
  dst->pw_shell = buf + required_byte_count;
  required_byte_count += strlen(src->pw_shell) + 1;
  if (byte_count < required_byte_count) {
    return ERANGE;
  }

  // Copy the strings.
  snprintf(buf, byte_count, "%s%c%s%c%s", src->pw_name, 0, src->pw_dir, 0, src->pw_shell);

  // pw_passwd and pw_gecos are non-POSIX and unused (always NULL) in bionic.
  // Note: On LP32, we define pw_gecos to pw_passwd since they're both NULL.
  dst->pw_passwd = nullptr;
#if defined(__LP64__)
  dst->pw_gecos = nullptr;
#endif

  // Copy the integral fields.
  dst->pw_gid = src->pw_gid;
  dst->pw_uid = src->pw_uid;

  *result = dst;
  return 0;
}

int getpwnam_r(const char* name, passwd* pwd,
               char* buf, size_t byte_count, passwd** result) {
  return do_getpw_r(1, name, -1, pwd, buf, byte_count, result);
}

int getpwuid_r(uid_t uid, passwd* pwd,
               char* buf, size_t byte_count, passwd** result) {
  return do_getpw_r(0, nullptr, uid, pwd, buf, byte_count, result);
}

static passwd* android_iinfo_to_passwd(passwd_state_t* state,
                                       const android_id_info* iinfo) {
  snprintf(state->name_buffer_, sizeof(state->name_buffer_), "%s", iinfo->name);
  snprintf(state->dir_buffer_, sizeof(state->dir_buffer_), "/");
  snprintf(state->sh_buffer_, sizeof(state->sh_buffer_), "/system/bin/sh");

  passwd* pw = &state->passwd_;
  pw->pw_name  = state->name_buffer_;
  pw->pw_uid   = iinfo->aid;
  pw->pw_gid   = iinfo->aid;
  pw->pw_dir   = state->dir_buffer_;
  pw->pw_shell = state->sh_buffer_;
  return pw;
}

static group* android_iinfo_to_group(group_state_t* state,
                                     const android_id_info* iinfo) {
  snprintf(state->group_name_buffer_, sizeof(state->group_name_buffer_), "%s", iinfo->name);

  group* gr = &state->group_;
  gr->gr_name   = state->group_name_buffer_;
  gr->gr_gid    = iinfo->aid;
  gr->gr_mem[0] = gr->gr_name;
  return gr;
}

static passwd* android_id_to_passwd(passwd_state_t* state, unsigned id) {
  for (size_t n = 0; n < android_id_count; ++n) {
    if (android_ids[n].aid == id) {
      return android_iinfo_to_passwd(state, android_ids + n);
    }
  }
  return nullptr;
}

static passwd* android_name_to_passwd(passwd_state_t* state, const char* name) {
  for (size_t n = 0; n < android_id_count; ++n) {
    if (!strcmp(android_ids[n].name, name)) {
      return android_iinfo_to_passwd(state, android_ids + n);
    }
  }
  return nullptr;
}

static group* android_id_to_group(group_state_t* state, unsigned id) {
  for (size_t n = 0; n < android_id_count; ++n) {
    if (android_ids[n].aid == id) {
      return android_iinfo_to_group(state, android_ids + n);
    }
  }
  return nullptr;
}

static group* android_name_to_group(group_state_t* state, const char* name) {
  for (size_t n = 0; n < android_id_count; ++n) {
    if (!strcmp(android_ids[n].name, name)) {
      return android_iinfo_to_group(state, android_ids + n);
    }
  }
  return nullptr;
}

// These are a list of the reserved app ranges, and should never contain anything below
// AID_APP_START.  They exist per user, so a given uid/gid modulo AID_USER_OFFSET will map
// to these ranges.
static constexpr struct {
  uid_t start;
  uid_t end;
} user_ranges[] = {
  { AID_APP_START, AID_APP_END },
  { AID_CACHE_GID_START, AID_CACHE_GID_END },
  { AID_EXT_GID_START, AID_EXT_GID_END },
  { AID_EXT_CACHE_GID_START, AID_EXT_CACHE_GID_END },
  { AID_SHARED_GID_START, AID_SHARED_GID_END },
  { AID_ISOLATED_START, AID_ISOLATED_END },
};

static constexpr bool verify_user_ranges_ascending() {
  auto array_size = arraysize(user_ranges);
  if (array_size < 2) return false;

  if (user_ranges[0].start > user_ranges[0].end) return false;

  for (size_t i = 1; i < array_size; ++i) {
    if (user_ranges[i].start > user_ranges[i].end) return false;
    if (user_ranges[i - 1].end > user_ranges[i].start) return false;
  }
  return true;
}

static_assert(verify_user_ranges_ascending(), "user_ranges must have ascending ranges");

static bool is_valid_app_id(id_t id) {
  id_t appid = id % AID_USER_OFFSET;

  // AID_OVERFLOWUID is never a valid app id, so we explicitly return false to ensure this.
  // This is true across all users, as there is no reason to ever map this id into any user range.
  if (appid == AID_OVERFLOWUID) {
    return false;
  }

  // If we've resolved to something before app_start, we have already checked against
  // android_ids, so no need to check again.
  if (appid < user_ranges[0].start) {
    return true;
  }

  // Otherwise check that the appid is in one of the reserved ranges.
  for (size_t i = 0; i < arraysize(user_ranges); ++i) {
    if (appid >= user_ranges[i].start && appid <= user_ranges[i].end) {
      return true;
    }
  }

  return false;
}

// This provides an iterater for app_ids within the first user's app id's.
static uid_t get_next_app_id(uid_t current_id) {
  // If current_id is below the first of the user_ranges, then we're uninitialized, and return the
  // first valid id.
  if (current_id < user_ranges[0].start) {
    return user_ranges[0].start;
  }

  uid_t incremented_id = current_id + 1;

  // Check to see if our incremented_id is between two ranges, and if so, return the beginning of
  // the next valid range.
  for (size_t i = 1; i < arraysize(user_ranges); ++i) {
    if (incremented_id > user_ranges[i - 1].end && incremented_id < user_ranges[i].start) {
      return user_ranges[i].start;
    }
  }

  // Check to see if our incremented_id is above final range, and return -1 to indicate that we've
  // completed if so.
  if (incremented_id > user_ranges[arraysize(user_ranges) - 1].end) {
    return -1;
  }

  // Otherwise the incremented_id is valid, so return it.
  return incremented_id;
}

// Translate a user/group name to the corresponding user/group id.
// all_a1234 -> 0 * AID_USER_OFFSET + AID_SHARED_GID_START + 1234 (group name only)
// u0_a1234_cache -> 0 * AID_USER_OFFSET + AID_CACHE_GID_START + 1234 (group name only)
// u0_a1234 -> 0 * AID_USER_OFFSET + AID_APP_START + 1234
// u2_i1000 -> 2 * AID_USER_OFFSET + AID_ISOLATED_START + 1000
// u1_system -> 1 * AID_USER_OFFSET + android_ids['system']
// returns 0 and sets errno to ENOENT in case of error.
static id_t app_id_from_name(const char* name, bool is_group) {
  char* end;
  unsigned long userid;
  bool is_shared_gid = false;

  if (is_group && name[0] == 'a' && name[1] == 'l' && name[2] == 'l') {
    end = const_cast<char*>(name+3);
    userid = 0;
    is_shared_gid = true;
  } else if (name[0] == 'u' && isdigit(name[1])) {
    userid = strtoul(name+1, &end, 10);
  } else {
    errno = ENOENT;
    return 0;
  }

  if (end[0] != '_' || end[1] == 0) {
    errno = ENOENT;
    return 0;
  }

  unsigned long appid = 0;
  if (end[1] == 'a' && isdigit(end[2])) {
    if (is_shared_gid) {
      // end will point to \0 if the strtoul below succeeds.
      appid = strtoul(end+2, &end, 10) + AID_SHARED_GID_START;
      if (appid > AID_SHARED_GID_END) {
        errno = ENOENT;
        return 0;
      }
    } else {
      // end will point to \0 if the strtoul below succeeds.
      appid = strtoul(end+2, &end, 10);
      if (is_group && !strcmp(end, "_cache")) {
        end += 6;
        appid += AID_CACHE_GID_START;
      } else {
        appid += AID_APP_START;
      }
    }
  } else if (end[1] == 'i' && isdigit(end[2])) {
    // end will point to \0 if the strtoul below succeeds.
    appid = strtoul(end+2, &end, 10) + AID_ISOLATED_START;
  } else {
    for (size_t n = 0; n < android_id_count; n++) {
      if (!strcmp(android_ids[n].name, end + 1)) {
        appid = android_ids[n].aid;
        // Move the end pointer to the null terminator.
        end += strlen(android_ids[n].name) + 1;
        break;
      }
    }
  }

  // Check that the entire string was consumed by one of the 3 cases above.
  if (end[0] != 0) {
    errno = ENOENT;
    return 0;
  }

  // Check that user id won't overflow.
  if (userid > 1000) {
    errno = ENOENT;
    return 0;
  }

  // Check that app id is within range.
  if (appid >= AID_USER_OFFSET) {
    errno = ENOENT;
    return 0;
  }

  return (appid + userid*AID_USER_OFFSET);
}

static void print_app_name_from_uid(const uid_t uid, char* buffer, const int bufferlen) {
  const uid_t appid = uid % AID_USER_OFFSET;
  const uid_t userid = uid / AID_USER_OFFSET;
  if (appid >= AID_ISOLATED_START) {
    snprintf(buffer, bufferlen, "u%u_i%u", userid, appid - AID_ISOLATED_START);
  } else if (appid < AID_APP_START) {
    for (size_t n = 0; n < android_id_count; n++) {
      if (android_ids[n].aid == appid) {
        snprintf(buffer, bufferlen, "u%u_%s", userid, android_ids[n].name);
        return;
      }
    }
  } else {
    snprintf(buffer, bufferlen, "u%u_a%u", userid, appid - AID_APP_START);
  }
}

static void print_app_name_from_gid(const gid_t gid, char* buffer, const int bufferlen) {
  const uid_t appid = gid % AID_USER_OFFSET;
  const uid_t userid = gid / AID_USER_OFFSET;
  if (appid >= AID_ISOLATED_START) {
    snprintf(buffer, bufferlen, "u%u_i%u", userid, appid - AID_ISOLATED_START);
  } else if (userid == 0 && appid >= AID_SHARED_GID_START && appid <= AID_SHARED_GID_END) {
    snprintf(buffer, bufferlen, "all_a%u", appid - AID_SHARED_GID_START);
  } else if (appid >= AID_CACHE_GID_START && appid <= AID_CACHE_GID_END) {
    snprintf(buffer, bufferlen, "u%u_a%u_cache", userid, appid - AID_CACHE_GID_START);
  } else if (appid < AID_APP_START) {
    for (size_t n = 0; n < android_id_count; n++) {
      if (android_ids[n].aid == appid) {
        snprintf(buffer, bufferlen, "u%u_%s", userid, android_ids[n].name);
        return;
      }
    }
  } else {
    snprintf(buffer, bufferlen, "u%u_a%u", userid, appid - AID_APP_START);
  }
}

// oem_XXXX -> uid
//  Supported ranges:
//   AID_OEM_RESERVED_START to AID_OEM_RESERVED_END (2900-2999)
//   AID_OEM_RESERVED_2_START to AID_OEM_RESERVED_2_END (5000-5999)
// Check OEM id is within range.
static bool is_oem_id(id_t id) {
  return (((id >= AID_OEM_RESERVED_START) && (id <= AID_OEM_RESERVED_END)) ||
      ((id >= AID_OEM_RESERVED_2_START) && (id <= AID_OEM_RESERVED_2_END)));
}

// Translate an OEM name to the corresponding user/group id.
static id_t oem_id_from_name(const char* name) {
  unsigned int id;
  if (sscanf(name, "oem_%u", &id) != 1) {
    return 0;
  }
  if (!is_oem_id(id)) {
    return 0;
  }
  return static_cast<id_t>(id);
}

static passwd* oem_id_to_passwd(uid_t uid, passwd_state_t* state) {
  if (!is_oem_id(uid)) {
    return nullptr;
  }

  if (vendor_passwd.FindById(uid, state)) {
    return &state->passwd_;
  }

  snprintf(state->name_buffer_, sizeof(state->name_buffer_), "oem_%u", uid);
  snprintf(state->dir_buffer_, sizeof(state->dir_buffer_), "/");
  snprintf(state->sh_buffer_, sizeof(state->sh_buffer_), "/vendor/bin/sh");

  passwd* pw = &state->passwd_;
  pw->pw_name  = state->name_buffer_;
  pw->pw_dir   = state->dir_buffer_;
  pw->pw_shell = state->sh_buffer_;
  pw->pw_uid   = uid;
  pw->pw_gid   = uid;
  return pw;
}

static group* oem_id_to_group(gid_t gid, group_state_t* state) {
  if (!is_oem_id(gid)) {
    return nullptr;
  }

  if (vendor_group.FindById(gid, state)) {
    return &state->group_;
  }

  snprintf(state->group_name_buffer_, sizeof(state->group_name_buffer_),
           "oem_%u", gid);

  group* gr = &state->group_;
  gr->gr_name   = state->group_name_buffer_;
  gr->gr_gid    = gid;
  gr->gr_mem[0] = gr->gr_name;
  return gr;
}

// Translate a uid into the corresponding name.
// 0 to AID_APP_START-1                    -> "system", "radio", etc.
// AID_APP_START to AID_ISOLATED_START-1   -> u0_a1234
// AID_ISOLATED_START to AID_USER_OFFSET-1 -> u0_i1234
// AID_USER_OFFSET+                        -> u1_radio, u1_a1234, u2_i1234, etc.
// returns a passwd structure (sets errno to ENOENT on failure).
static passwd* app_id_to_passwd(uid_t uid, passwd_state_t* state) {
  if (uid < AID_APP_START || !is_valid_app_id(uid)) {
    errno = ENOENT;
    return nullptr;
  }

  print_app_name_from_uid(uid, state->name_buffer_, sizeof(state->name_buffer_));

  const uid_t appid = uid % AID_USER_OFFSET;
  if (appid < AID_APP_START) {
      snprintf(state->dir_buffer_, sizeof(state->dir_buffer_), "/");
  } else {
      snprintf(state->dir_buffer_, sizeof(state->dir_buffer_), "/data");
  }

  snprintf(state->sh_buffer_, sizeof(state->sh_buffer_), "/system/bin/sh");

  passwd* pw = &state->passwd_;
  pw->pw_name  = state->name_buffer_;
  pw->pw_dir   = state->dir_buffer_;
  pw->pw_shell = state->sh_buffer_;
  pw->pw_uid   = uid;
  pw->pw_gid   = uid;
  return pw;
}

// Translate a gid into the corresponding app_<gid>
// group structure (sets errno to ENOENT on failure).
static group* app_id_to_group(gid_t gid, group_state_t* state) {
  if (gid < AID_APP_START || !is_valid_app_id(gid)) {
    errno = ENOENT;
    return nullptr;
  }

  print_app_name_from_gid(gid, state->group_name_buffer_, sizeof(state->group_name_buffer_));

  group* gr = &state->group_;
  gr->gr_name   = state->group_name_buffer_;
  gr->gr_gid    = gid;
  gr->gr_mem[0] = gr->gr_name;
  return gr;
}

passwd* getpwuid(uid_t uid) { // NOLINT: implementing bad function.
  passwd_state_t* state = get_passwd_tls_buffer();
  if (state == nullptr) {
    return nullptr;
  }

  passwd* pw = android_id_to_passwd(state, uid);
  if (pw != nullptr) {
    return pw;
  }
  // Handle OEM range.
  pw = oem_id_to_passwd(uid, state);
  if (pw != nullptr) {
    return pw;
  }
  return app_id_to_passwd(uid, state);
}

passwd* getpwnam(const char* login) { // NOLINT: implementing bad function.
  passwd_state_t* state = get_passwd_tls_buffer();
  if (state == nullptr) {
    return nullptr;
  }

  passwd* pw = android_name_to_passwd(state, login);
  if (pw != nullptr) {
    return pw;
  }

  if (vendor_passwd.FindByName(login, state)) {
    if (is_oem_id(state->passwd_.pw_uid)) {
      return &state->passwd_;
    }
  }

  // Handle OEM range.
  pw = oem_id_to_passwd(oem_id_from_name(login), state);
  if (pw != nullptr) {
    return pw;
  }
  return app_id_to_passwd(app_id_from_name(login, false), state);
}

// All users are in just one group, the one passed in.
int getgrouplist(const char* /*user*/, gid_t group, gid_t* groups, int* ngroups) {
  if (*ngroups < 1) {
    *ngroups = 1;
    return -1;
  }
  groups[0] = group;
  return (*ngroups = 1);
}

char* getlogin() { // NOLINT: implementing bad function.
  passwd *pw = getpwuid(getuid()); // NOLINT: implementing bad function in terms of bad function.
  return pw ? pw->pw_name : nullptr;
}

int getlogin_r(char* buf, size_t size) {
  char* login = getlogin();
  if (login == nullptr) return errno;
  size_t login_length = strlen(login) + 1;
  if (login_length > size) return ERANGE;
  memcpy(buf, login, login_length);
  return 0;
}

void setpwent() {
  passwd_state_t* state = get_passwd_tls_buffer();
  if (state) {
    state->getpwent_idx = 0;
  }
}

void endpwent() {
  setpwent();
}

passwd* getpwent() {
  passwd_state_t* state = get_passwd_tls_buffer();
  if (state == nullptr) {
    return nullptr;
  }
  if (state->getpwent_idx < 0) {
    return nullptr;
  }

  size_t start = 0;
  ssize_t end = android_id_count;
  if (state->getpwent_idx < end) {
    return android_iinfo_to_passwd(state, android_ids + state->getpwent_idx++);
  }

  start = end;
  end += AID_OEM_RESERVED_END - AID_OEM_RESERVED_START + 1;

  if (state->getpwent_idx < end) {
    return oem_id_to_passwd(
        state->getpwent_idx++ - start + AID_OEM_RESERVED_START, state);
  }

  start = end;
  end += AID_OEM_RESERVED_2_END - AID_OEM_RESERVED_2_START + 1;

  if (state->getpwent_idx < end) {
    return oem_id_to_passwd(
        state->getpwent_idx++ - start + AID_OEM_RESERVED_2_START, state);
  }

  state->getpwent_idx = get_next_app_id(state->getpwent_idx);

  if (state->getpwent_idx != -1) {
    return app_id_to_passwd(state->getpwent_idx, state);
  }

  // We are not reporting u1_a* and higher or we will be here forever
  return nullptr;
}

static group* getgrgid_internal(gid_t gid, group_state_t* state) {
  group* grp = android_id_to_group(state, gid);
  if (grp != nullptr) {
    return grp;
  }
  // Handle OEM range.
  grp = oem_id_to_group(gid, state);
  if (grp != nullptr) {
    return grp;
  }
  return app_id_to_group(gid, state);
}

group* getgrgid(gid_t gid) { // NOLINT: implementing bad function.
  group_state_t* state = __group_state();
  if (state == nullptr) {
    return nullptr;
  }
  return getgrgid_internal(gid, state);
}

static group* getgrnam_internal(const char* name, group_state_t* state) {
  group* grp = android_name_to_group(state, name);
  if (grp != nullptr) {
    return grp;
  }

  if (vendor_group.FindByName(name, state)) {
    if (is_oem_id(state->group_.gr_gid)) {
      return &state->group_;
    }
  }

  // Handle OEM range.
  grp = oem_id_to_group(oem_id_from_name(name), state);
  if (grp != nullptr) {
    return grp;
  }
  return app_id_to_group(app_id_from_name(name, true), state);
}

group* getgrnam(const char* name) { // NOLINT: implementing bad function.
  group_state_t* state = __group_state();
  if (state == nullptr) {
    return nullptr;
  }
  return getgrnam_internal(name, state);
}

static int getgroup_r(bool by_name, const char* name, gid_t gid, struct group* grp, char* buf,
                      size_t buflen, struct group** result) {
  ErrnoRestorer errno_restorer;
  *result = nullptr;
  char* p = reinterpret_cast<char*>(
      __BIONIC_ALIGN(reinterpret_cast<uintptr_t>(buf), sizeof(uintptr_t)));
  if (p + sizeof(group_state_t) > buf + buflen) {
    return ERANGE;
  }
  group_state_t* state = reinterpret_cast<group_state_t*>(p);
  init_group_state(state);
  group* retval = (by_name ? getgrnam_internal(name, state) : getgrgid_internal(gid, state));
  if (retval != nullptr) {
    *grp = *retval;
    *result = grp;
    return 0;
  }
  return errno;
}

int getgrgid_r(gid_t gid, struct group* grp, char* buf, size_t buflen, struct group** result) {
  return getgroup_r(false, nullptr, gid, grp, buf, buflen, result);
}

int getgrnam_r(const char* name, struct group* grp, char* buf, size_t buflen,
               struct group **result) {
  return getgroup_r(true, name, 0, grp, buf, buflen, result);
}

void setgrent() {
  group_state_t* state = get_group_tls_buffer();
  if (state) {
    state->getgrent_idx = 0;
  }
}

void endgrent() {
  setgrent();
}

group* getgrent() {
  group_state_t* state = get_group_tls_buffer();
  if (state == nullptr) {
    return nullptr;
  }
  if (state->getgrent_idx < 0) {
    return nullptr;
  }

  size_t start = 0;
  ssize_t end = android_id_count;
  if (state->getgrent_idx < end) {
    init_group_state(state);
    return android_iinfo_to_group(state, android_ids + state->getgrent_idx++);
  }

  start = end;
  end += AID_OEM_RESERVED_END - AID_OEM_RESERVED_START + 1;

  if (state->getgrent_idx < end) {
    init_group_state(state);
    return oem_id_to_group(
        state->getgrent_idx++ - start + AID_OEM_RESERVED_START, state);
  }

  start = end;
  end += AID_OEM_RESERVED_2_END - AID_OEM_RESERVED_2_START + 1;

  if (state->getgrent_idx < end) {
    init_group_state(state);
    return oem_id_to_group(
        state->getgrent_idx++ - start + AID_OEM_RESERVED_2_START, state);
  }

  start = end;
  end += AID_USER_OFFSET - AID_APP_START; // Do not expose higher groups

  state->getgrent_idx = get_next_app_id(state->getgrent_idx);

  if (state->getgrent_idx != -1) {
    return app_id_to_group(state->getgrent_idx, state);
  }

  // We are not reporting u1_a* and higher or we will be here forever
  return nullptr;
}
