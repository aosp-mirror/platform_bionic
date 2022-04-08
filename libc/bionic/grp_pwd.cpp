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

#include "private/grp_pwd.h"

#include <android/api-level.h>
#include <ctype.h>
#include <errno.h>
#include <grp.h>
#include <mntent.h>
#include <pthread.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/system_properties.h>
#include <sys/types.h>
#include <unistd.h>

#include "private/ErrnoRestorer.h"
#include "private/android_filesystem_config.h"
#include "platform/bionic/macros.h"

// Generated android_ids array
#include "generated_android_ids.h"
#include "grp_pwd_file.h"

static PasswdFile passwd_files[] = {
  { "/system/etc/passwd", "system_" },
  { "/vendor/etc/passwd", "vendor_" },
  { "/odm/etc/passwd", "odm_" },
  { "/product/etc/passwd", "product_" },
  { "/system_ext/etc/passwd", "system_ext_" },
};

static GroupFile group_files[] = {
  { "/system/etc/group", "system_" },
  { "/vendor/etc/group", "vendor_" },
  { "/odm/etc/group", "odm_" },
  { "/product/etc/group", "product_" },
  { "/system_ext/etc/group", "system_ext_" },
};

// POSIX seems to envisage an implementation where the <pwd.h> functions are
// implemented by brute-force searching with getpwent(3), and the <grp.h>
// functions are implemented similarly with getgrent(3). This means that it's
// okay for all the <grp.h> functions to share state, and all the <passwd.h>
// functions to share state, but <grp.h> functions can't clobber <passwd.h>
// functions' state and vice versa.
#include "bionic/pthread_internal.h"

static void init_group_state(group_state_t* state) {
  memset(state, 0, sizeof(group_state_t) - sizeof(state->getgrent_idx));
  state->group_.gr_name = state->group_name_buffer_;
  state->group_.gr_mem = state->group_members_;
  state->group_.gr_mem[0] = state->group_.gr_name;
}

static group_state_t* get_group_tls_buffer() {
  auto result = &__get_bionic_tls().group;
  init_group_state(result);
  return result;
}

static void init_passwd_state(passwd_state_t* state) {
  memset(state, 0, sizeof(passwd_state_t) - sizeof(state->getpwent_idx));
  state->passwd_.pw_name = state->name_buffer_;
  state->passwd_.pw_dir = state->dir_buffer_;
  state->passwd_.pw_shell = state->sh_buffer_;
}

static passwd_state_t* get_passwd_tls_buffer() {
  auto result = &__get_bionic_tls().passwd;
  init_passwd_state(result);
  return result;
}

static passwd* android_iinfo_to_passwd(passwd_state_t* state,
                                       const android_id_info* iinfo) {
  snprintf(state->name_buffer_, sizeof(state->name_buffer_), "%s", iinfo->name);
  snprintf(state->dir_buffer_, sizeof(state->dir_buffer_), "/");
  snprintf(state->sh_buffer_, sizeof(state->sh_buffer_), "/bin/sh");

  passwd* pw = &state->passwd_;
  pw->pw_uid   = iinfo->aid;
  pw->pw_gid   = iinfo->aid;
  return pw;
}

static group* android_iinfo_to_group(group_state_t* state,
                                     const android_id_info* iinfo) {
  snprintf(state->group_name_buffer_, sizeof(state->group_name_buffer_), "%s", iinfo->name);

  group* gr = &state->group_;
  gr->gr_gid = iinfo->aid;
  return gr;
}

static const android_id_info* find_android_id_info(unsigned id) {
  for (size_t n = 0; n < android_id_count; ++n) {
    if (android_ids[n].aid == id) {
      return &android_ids[n];
    }
  }
  return nullptr;
}

static const android_id_info* find_android_id_info(const char* name) {
  for (size_t n = 0; n < android_id_count; ++n) {
    if (!strcmp(android_ids[n].name, name)) {
      return &android_ids[n];
    }
  }
  return nullptr;
}

// These are a list of the reserved app ranges, and should never contain anything below
// AID_APP_START.  They exist per user, so a given uid/gid modulo AID_USER_OFFSET will map
// to these ranges.
struct IdRange {
  id_t start;
  id_t end;
};

static constexpr IdRange user_ranges[] = {
  { AID_APP_START, AID_APP_END },
  { AID_ISOLATED_START, AID_ISOLATED_END },
};

static constexpr IdRange group_ranges[] = {
  { AID_APP_START, AID_APP_END },
  { AID_CACHE_GID_START, AID_CACHE_GID_END },
  { AID_EXT_GID_START, AID_EXT_GID_END },
  { AID_EXT_CACHE_GID_START, AID_EXT_CACHE_GID_END },
  { AID_SHARED_GID_START, AID_SHARED_GID_END },
  { AID_ISOLATED_START, AID_ISOLATED_END },
};

template <class T, size_t N>
static constexpr bool verify_user_ranges_ascending(T (&ranges)[N]) {
  auto array_size = N;
  if (array_size < 2) return false;

  if (ranges[0].start > ranges[0].end) return false;

  for (size_t i = 1; i < array_size; ++i) {
    if (ranges[i].start > ranges[i].end) return false;
    if (ranges[i - 1].end > ranges[i].start) return false;
  }
  return true;
}

static_assert(verify_user_ranges_ascending(user_ranges), "user_ranges must have ascending ranges");
static_assert(verify_user_ranges_ascending(group_ranges), "user_ranges must have ascending ranges");

// This list comes from PackageManagerService.java, where platform AIDs are added to list of valid
// AIDs for packages via addSharedUserLPw().
static constexpr const id_t secondary_user_platform_ids[] = {
  AID_SYSTEM, AID_RADIO,          AID_LOG,           AID_NFC, AID_BLUETOOTH,
  AID_SHELL,  AID_SECURE_ELEMENT, AID_NETWORK_STACK,
};

static bool platform_id_secondary_user_allowed(id_t id) {
  for (const auto& allowed_id : secondary_user_platform_ids) {
    if (allowed_id == id) {
      return true;
    }
  }
  return false;
}

static bool is_valid_app_id(id_t id, bool is_group) {
  id_t appid = id % AID_USER_OFFSET;

  // AID_OVERFLOWUID is never a valid app id, so we explicitly return false to ensure this.
  // This is true across all users, as there is no reason to ever map this id into any user range.
  if (appid == AID_OVERFLOWUID) {
    return false;
  }

  auto ranges_size = is_group ? arraysize(group_ranges) : arraysize(user_ranges);
  auto ranges = is_group ? group_ranges : user_ranges;

  // If we're checking an appid that resolves below the user range, then it's a platform AID for a
  // seconary user. We only allow a reduced set of these, so we must check that it is allowed.
  if (appid < ranges[0].start && platform_id_secondary_user_allowed(appid)) {
    return true;
  }

  // The shared GID range is only valid for the first user.
  if (appid >= AID_SHARED_GID_START && appid <= AID_SHARED_GID_END && appid != id) {
    return false;
  }

  // Otherwise check that the appid is in one of the reserved ranges.
  for (size_t i = 0; i < ranges_size; ++i) {
    if (appid >= ranges[i].start && appid <= ranges[i].end) {
      return true;
    }
  }

  return false;
}

// This provides an iterater for app_ids within the first user's app id's.
static id_t get_next_app_id(id_t current_id, bool is_group) {
  auto ranges_size = is_group ? arraysize(group_ranges) : arraysize(user_ranges);
  auto ranges = is_group ? group_ranges : user_ranges;

  // If current_id is below the first of the ranges, then we're uninitialized, and return the first
  // valid id.
  if (current_id < ranges[0].start) {
    return ranges[0].start;
  }

  id_t incremented_id = current_id + 1;

  // Check to see if our incremented_id is between two ranges, and if so, return the beginning of
  // the next valid range.
  for (size_t i = 1; i < ranges_size; ++i) {
    if (incremented_id > ranges[i - 1].end && incremented_id < ranges[i].start) {
      return ranges[i].start;
    }
  }

  // Check to see if our incremented_id is above final range, and return -1 to indicate that we've
  // completed if so.
  if (incremented_id > ranges[ranges_size - 1].end) {
    return -1;
  }

  // Otherwise the incremented_id is valid, so return it.
  return incremented_id;
}

// Translate a user/group name to the corresponding user/group id.
// all_a1234 -> 0 * AID_USER_OFFSET + AID_SHARED_GID_START + 1234 (group name only)
// u0_a1234_ext_cache -> 0 * AID_USER_OFFSET + AID_EXT_CACHE_GID_START + 1234 (group name only)
// u0_a1234_ext -> 0 * AID_USER_OFFSET + AID_EXT_GID_START + 1234 (group name only)
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
      if (is_group) {
        if (!strcmp(end, "_ext_cache")) {
          end += 10;
          appid += AID_EXT_CACHE_GID_START;
        } else if (!strcmp(end, "_ext")) {
          end += 4;
          appid += AID_EXT_GID_START;
        } else if (!strcmp(end, "_cache")) {
          end += 6;
          appid += AID_CACHE_GID_START;
        } else {
          appid += AID_APP_START;
        }
      } else {
        appid += AID_APP_START;
      }
    }
  } else if (end[1] == 'i' && isdigit(end[2])) {
    // end will point to \0 if the strtoul below succeeds.
    appid = strtoul(end+2, &end, 10) + AID_ISOLATED_START;
  } else if (auto* android_id_info = find_android_id_info(end + 1); android_id_info != nullptr) {
    appid = android_id_info->aid;
    end += strlen(android_id_info->name) + 1;
    if (!platform_id_secondary_user_allowed(appid)) {
      errno = ENOENT;
      return 0;
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
    if (auto* android_id_info = find_android_id_info(appid); android_id_info != nullptr) {
      snprintf(buffer, bufferlen, "u%u_%s", userid, android_id_info->name);
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
  } else if (appid >= AID_EXT_CACHE_GID_START && appid <= AID_EXT_CACHE_GID_END) {
    snprintf(buffer, bufferlen, "u%u_a%u_ext_cache", userid, appid - AID_EXT_CACHE_GID_START);
  } else if (appid >= AID_EXT_GID_START && appid <= AID_EXT_GID_END) {
    snprintf(buffer, bufferlen, "u%u_a%u_ext", userid, appid - AID_EXT_GID_START);
  } else if (appid >= AID_CACHE_GID_START && appid <= AID_CACHE_GID_END) {
    snprintf(buffer, bufferlen, "u%u_a%u_cache", userid, appid - AID_CACHE_GID_START);
  } else if (appid < AID_APP_START) {
    if (auto* android_id_info = find_android_id_info(appid); android_id_info != nullptr) {
      snprintf(buffer, bufferlen, "u%u_%s", userid, android_id_info->name);
    }
  } else {
    snprintf(buffer, bufferlen, "u%u_a%u", userid, appid - AID_APP_START);
  }
}

static bool device_launched_before_api_29() {
  // Check if ro.product.first_api_level is set to a value > 0 and < 29, if so, this device was
  // launched before API 29 (Q). Any other value is considered to be either in development or
  // launched after.
  // Cache the value as __system_property_get() is expensive and this may be called often.
  static bool result = [] {
    char value[PROP_VALUE_MAX] = { 0 };
    if (__system_property_get("ro.product.first_api_level", value) == 0) {
      return false;
    }
    int value_int = atoi(value);
    return value_int != 0 && value_int < 29;
  }();
  return result;
}

// oem_XXXX -> uid
//  Supported ranges:
//   AID_OEM_RESERVED_START to AID_OEM_RESERVED_END (2900-2999)
//   AID_OEM_RESERVED_2_START to AID_OEM_RESERVED_2_END (5000-5999)
// Check OEM id is within range.
static bool is_oem_id(id_t id) {
  // Upgrading devices launched before API level 29 may not comply with the below check.
  // Due to the difficulty in changing uids after launch, it is waived for these devices.
  // The legacy range:
  // AID_OEM_RESERVED_START to AID_EVERYBODY (2900-9996), excluding builtin AIDs.
  if (device_launched_before_api_29() && id >= AID_OEM_RESERVED_START && id < AID_EVERYBODY &&
      find_android_id_info(id) == nullptr) {
    return true;
  }

  return (id >= AID_OEM_RESERVED_START && id <= AID_OEM_RESERVED_END) ||
         (id >= AID_OEM_RESERVED_2_START && id <= AID_OEM_RESERVED_2_END);
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
  for (auto& passwd_file : passwd_files) {
    if (passwd_file.FindById(uid, state)) {
      return &state->passwd_;
    }
  }

  if (!is_oem_id(uid)) {
    return nullptr;
  }

  snprintf(state->name_buffer_, sizeof(state->name_buffer_), "oem_%u", uid);
  snprintf(state->dir_buffer_, sizeof(state->dir_buffer_), "/");
  snprintf(state->sh_buffer_, sizeof(state->sh_buffer_), "/bin/sh");

  passwd* pw = &state->passwd_;
  pw->pw_uid   = uid;
  pw->pw_gid   = uid;
  return pw;
}

static group* oem_id_to_group(gid_t gid, group_state_t* state) {
  for (auto& group_file : group_files) {
    if (group_file.FindById(gid, state)) {
      return &state->group_;
    }
  }

  if (!is_oem_id(gid)) {
    return nullptr;
  }

  snprintf(state->group_name_buffer_, sizeof(state->group_name_buffer_),
           "oem_%u", gid);

  group* gr = &state->group_;
  gr->gr_gid = gid;
  return gr;
}

// Translate a uid into the corresponding name.
// 0 to AID_APP_START-1                    -> "system", "radio", etc.
// AID_APP_START to AID_ISOLATED_START-1   -> u0_a1234
// AID_ISOLATED_START to AID_USER_OFFSET-1 -> u0_i1234
// AID_USER_OFFSET+                        -> u1_radio, u1_a1234, u2_i1234, etc.
// returns a passwd structure (sets errno to ENOENT on failure).
static passwd* app_id_to_passwd(uid_t uid, passwd_state_t* state) {
  if (uid < AID_APP_START || !is_valid_app_id(uid, false)) {
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

  snprintf(state->sh_buffer_, sizeof(state->sh_buffer_), "/bin/sh");

  passwd* pw = &state->passwd_;
  pw->pw_uid   = uid;
  pw->pw_gid   = uid;
  return pw;
}

// Translate a gid into the corresponding app_<gid>
// group structure (sets errno to ENOENT on failure).
static group* app_id_to_group(gid_t gid, group_state_t* state) {
  if (gid < AID_APP_START || !is_valid_app_id(gid, true)) {
    errno = ENOENT;
    return nullptr;
  }

  print_app_name_from_gid(gid, state->group_name_buffer_, sizeof(state->group_name_buffer_));

  group* gr = &state->group_;
  gr->gr_gid = gid;
  return gr;
}

passwd* getpwuid_internal(uid_t uid, passwd_state_t* state) {
  if (auto* android_id_info = find_android_id_info(uid); android_id_info != nullptr) {
    return android_iinfo_to_passwd(state, android_id_info);
  }

  // Handle OEM range.
  passwd* pw = oem_id_to_passwd(uid, state);
  if (pw != nullptr) {
    return pw;
  }
  return app_id_to_passwd(uid, state);
}

passwd* getpwuid(uid_t uid) {  // NOLINT: implementing bad function.
  passwd_state_t* state = get_passwd_tls_buffer();
  return getpwuid_internal(uid, state);
}

passwd* getpwnam_internal(const char* login, passwd_state_t* state) {
  if (auto* android_id_info = find_android_id_info(login); android_id_info != nullptr) {
    return android_iinfo_to_passwd(state, android_id_info);
  }

  for (auto& passwd_file : passwd_files) {
    if (passwd_file.FindByName(login, state)) {
      return &state->passwd_;
    }
  }

  // Handle OEM range.
  passwd* pw = oem_id_to_passwd(oem_id_from_name(login), state);
  if (pw != nullptr) {
    return pw;
  }
  return app_id_to_passwd(app_id_from_name(login, false), state);
}

passwd* getpwnam(const char* login) {  // NOLINT: implementing bad function.
  passwd_state_t* state = get_passwd_tls_buffer();
  return getpwnam_internal(login, state);
}

static int getpasswd_r(bool by_name, const char* name, uid_t uid, struct passwd* pwd, char* buf,
                       size_t buflen, struct passwd** result) {
  ErrnoRestorer errno_restorer;
  *result = nullptr;
  char* p =
      reinterpret_cast<char*>(__BIONIC_ALIGN(reinterpret_cast<uintptr_t>(buf), sizeof(uintptr_t)));
  if (p + sizeof(passwd_state_t) > buf + buflen) {
    return ERANGE;
  }
  passwd_state_t* state = reinterpret_cast<passwd_state_t*>(p);
  init_passwd_state(state);
  passwd* retval = (by_name ? getpwnam_internal(name, state) : getpwuid_internal(uid, state));
  if (retval != nullptr) {
    *pwd = *retval;
    *result = pwd;
    return 0;
  }
  return errno;
}

int getpwnam_r(const char* name, passwd* pwd, char* buf, size_t byte_count, passwd** result) {
  return getpasswd_r(true, name, -1, pwd, buf, byte_count, result);
}

int getpwuid_r(uid_t uid, passwd* pwd, char* buf, size_t byte_count, passwd** result) {
  return getpasswd_r(false, nullptr, uid, pwd, buf, byte_count, result);
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

  start = end;
  end += AID_SYSTEM_EXT_RESERVED_END - AID_SYSTEM_RESERVED_START + 1;

  if (state->getpwent_idx < end) {
    // No one calls this enough to worry about how inefficient the below is.
    auto* oem_passwd =
        oem_id_to_passwd(state->getpwent_idx++ - start + AID_SYSTEM_RESERVED_START, state);
    while (oem_passwd == nullptr && state->getpwent_idx < end) {
      oem_passwd =
          oem_id_to_passwd(state->getpwent_idx++ - start + AID_SYSTEM_RESERVED_START, state);
    }
    if (oem_passwd != nullptr) {
      return oem_passwd;
    }
  }

  state->getpwent_idx = get_next_app_id(state->getpwent_idx, false);

  if (state->getpwent_idx != -1) {
    return app_id_to_passwd(state->getpwent_idx, state);
  }

  // We are not reporting u1_a* and higher or we will be here forever
  return nullptr;
}

static group* getgrgid_internal(gid_t gid, group_state_t* state) {
  if (auto* android_id_info = find_android_id_info(gid); android_id_info != nullptr) {
    return android_iinfo_to_group(state, android_id_info);
  }

  // Handle OEM range.
  group* grp = oem_id_to_group(gid, state);
  if (grp != nullptr) {
    return grp;
  }
  return app_id_to_group(gid, state);
}

group* getgrgid(gid_t gid) { // NOLINT: implementing bad function.
  group_state_t* state = get_group_tls_buffer();
  return getgrgid_internal(gid, state);
}

static group* getgrnam_internal(const char* name, group_state_t* state) {
  if (auto* android_id_info = find_android_id_info(name); android_id_info != nullptr) {
    return android_iinfo_to_group(state, android_id_info);
  }

  for (auto& group_file : group_files) {
    if (group_file.FindByName(name, state)) {
      return &state->group_;
    }
  }

  // Handle OEM range.
  group* grp = oem_id_to_group(oem_id_from_name(name), state);
  if (grp != nullptr) {
    return grp;
  }
  return app_id_to_group(app_id_from_name(name, true), state);
}

group* getgrnam(const char* name) { // NOLINT: implementing bad function.
  group_state_t* state = get_group_tls_buffer();
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
  if (state->getgrent_idx < 0) {
    return nullptr;
  }

  size_t start = 0;
  ssize_t end = android_id_count;
  if (state->getgrent_idx < end) {
    return android_iinfo_to_group(state, android_ids + state->getgrent_idx++);
  }

  start = end;
  end += AID_OEM_RESERVED_END - AID_OEM_RESERVED_START + 1;

  if (state->getgrent_idx < end) {
    return oem_id_to_group(
        state->getgrent_idx++ - start + AID_OEM_RESERVED_START, state);
  }

  start = end;
  end += AID_OEM_RESERVED_2_END - AID_OEM_RESERVED_2_START + 1;

  if (state->getgrent_idx < end) {
    return oem_id_to_group(
        state->getgrent_idx++ - start + AID_OEM_RESERVED_2_START, state);
  }

  start = end;
  end += AID_SYSTEM_EXT_RESERVED_END - AID_SYSTEM_RESERVED_START + 1;

  if (state->getgrent_idx < end) {
    // No one calls this enough to worry about how inefficient the below is.
    init_group_state(state);
    auto* oem_group =
        oem_id_to_group(state->getgrent_idx++ - start + AID_SYSTEM_RESERVED_START, state);
    while (oem_group == nullptr && state->getgrent_idx < end) {
      oem_group = oem_id_to_group(state->getgrent_idx++ - start + AID_SYSTEM_RESERVED_START, state);
    }
    if (oem_group != nullptr) {
      return oem_group;
    }
  }

  start = end;
  end += AID_USER_OFFSET - AID_APP_START; // Do not expose higher groups

  state->getgrent_idx = get_next_app_id(state->getgrent_idx, true);

  if (state->getgrent_idx != -1) {
    return app_id_to_group(state->getgrent_idx, state);
  }

  // We are not reporting u1_a* and higher or we will be here forever
  return nullptr;
}
