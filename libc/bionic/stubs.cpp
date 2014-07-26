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
#include <netdb.h>
#include <pthread.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "private/android_filesystem_config.h"
#include "private/ErrnoRestorer.h"
#include "private/libc_logging.h"

// Thread-specific state for the non-reentrant functions.
static pthread_once_t stubs_once = PTHREAD_ONCE_INIT;
static pthread_key_t stubs_key;
struct stubs_state_t {
  passwd passwd_;
  group group_;
  char* group_members_[2];
  char app_name_buffer_[32];
  char group_name_buffer_[32];
  char dir_buffer_[32];
  char sh_buffer_[32];
};

static int do_getpw_r(int by_name, const char* name, uid_t uid,
                      passwd* dst, char* buf, size_t byte_count,
                      passwd** result) {
  // getpwnam_r and getpwuid_r don't modify errno, but library calls we
  // make might.
  ErrnoRestorer errno_restorer;
  *result = NULL;

  // Our implementation of getpwnam(3) and getpwuid(3) use thread-local
  // storage, so we can call them as long as we copy everything out
  // before returning.
  const passwd* src = by_name ? getpwnam(name) : getpwuid(uid); // NOLINT: see above.

  // POSIX allows failure to find a match to be considered a non-error.
  // Reporting success (0) but with *result NULL is glibc's behavior.
  if (src == NULL) {
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
  dst->pw_passwd = NULL;
#ifdef __LP64__
  dst->pw_gecos = NULL;
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
  return do_getpw_r(0, NULL, uid, pwd, buf, byte_count, result);
}

static stubs_state_t* stubs_state_alloc() {
  stubs_state_t*  s = static_cast<stubs_state_t*>(calloc(1, sizeof(*s)));
  if (s != NULL) {
    s->group_.gr_mem = s->group_members_;
  }
  return s;
}

static void stubs_state_free(void* ptr) {
  stubs_state_t* state = static_cast<stubs_state_t*>(ptr);
  free(state);
}

static void __stubs_key_init() {
  pthread_key_create(&stubs_key, stubs_state_free);
}

static stubs_state_t* __stubs_state() {
  pthread_once(&stubs_once, __stubs_key_init);
  stubs_state_t* s = static_cast<stubs_state_t*>(pthread_getspecific(stubs_key));
  if (s == NULL) {
    s = stubs_state_alloc();
    if (s == NULL) {
      errno = ENOMEM;  // Just in case.
    } else {
      if (pthread_setspecific(stubs_key, s) != 0) {
        stubs_state_free(s);
        errno = ENOMEM;
        s = NULL;
      }
    }
  }
  return s;
}

static passwd* android_iinfo_to_passwd(stubs_state_t* state,
                                       const android_id_info* iinfo) {
  snprintf(state->dir_buffer_, sizeof(state->dir_buffer_), "/");
  snprintf(state->sh_buffer_, sizeof(state->sh_buffer_), "/system/bin/sh");

  passwd* pw = &state->passwd_;
  pw->pw_name  = (char*) iinfo->name;
  pw->pw_uid   = iinfo->aid;
  pw->pw_gid   = iinfo->aid;
  pw->pw_dir   = state->dir_buffer_;
  pw->pw_shell = state->sh_buffer_;
  return pw;
}

static group* android_iinfo_to_group(group* gr,
                                     const android_id_info* iinfo) {
  gr->gr_name   = (char*) iinfo->name;
  gr->gr_gid    = iinfo->aid;
  gr->gr_mem[0] = gr->gr_name;
  gr->gr_mem[1] = NULL;
  return gr;
}

static passwd* android_id_to_passwd(stubs_state_t* state, unsigned id) {
  for (size_t n = 0; n < android_id_count; ++n) {
    if (android_ids[n].aid == id) {
      return android_iinfo_to_passwd(state, android_ids + n);
    }
  }
  return NULL;
}

static passwd* android_name_to_passwd(stubs_state_t* state, const char* name) {
  for (size_t n = 0; n < android_id_count; ++n) {
    if (!strcmp(android_ids[n].name, name)) {
      return android_iinfo_to_passwd(state, android_ids + n);
    }
  }
  return NULL;
}

static group* android_id_to_group(group* gr, unsigned id) {
  for (size_t n = 0; n < android_id_count; ++n) {
    if (android_ids[n].aid == id) {
      return android_iinfo_to_group(gr, android_ids + n);
    }
  }
  return NULL;
}

static group* android_name_to_group(group* gr, const char* name) {
  for (size_t n = 0; n < android_id_count; ++n) {
    if (!strcmp(android_ids[n].name, name)) {
      return android_iinfo_to_group(gr, android_ids + n);
    }
  }
  return NULL;
}

// Translate a user/group name to the corresponding user/group id.
// u0_a1234 -> 0 * AID_USER + AID_APP + 1234
// u2_i1000 -> 2 * AID_USER + AID_ISOLATED_START + 1000
// u1_system -> 1 * AID_USER + android_ids['system']
// returns 0 and sets errno to ENOENT in case of error
static unsigned app_id_from_name(const char* name) {
  if (name[0] != 'u' || !isdigit(name[1])) {
    errno = ENOENT;
    return 0;
  }

  char* end;
  unsigned long userid = strtoul(name+1, &end, 10);
  if (end[0] != '_' || end[1] == 0) {
    errno = ENOENT;
    return 0;
  }

  unsigned long appid = 0;
  if (end[1] == 'a' && isdigit(end[2])) {
    // end will point to \0 if the strtoul below succeeds.
    appid = strtoul(end+2, &end, 10) + AID_APP;
  } else if (end[1] == 'i' && isdigit(end[2])) {
    // end will point to \0 if the strtoul below succeeds.
    appid = strtoul(end+2, &end, 10) + AID_ISOLATED_START;
  } else {
    for (size_t n = 0; n < android_id_count; n++) {
      if (!strcmp(android_ids[n].name, end + 1)) {
        appid = android_ids[n].aid;
        // Move the end pointer to the null terminator.
        end += strlen(android_ids[n].name) + 1;
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
  if (appid >= AID_USER) {
    errno = ENOENT;
    return 0;
  }

  return (unsigned)(appid + userid*AID_USER);
}

static void print_app_name_from_appid_userid(const uid_t appid,
    const uid_t userid, char* buffer, const int bufferlen) {
  if (appid >= AID_ISOLATED_START) {
    snprintf(buffer, bufferlen, "u%u_i%u", userid, appid - AID_ISOLATED_START);
  } else if (userid == 0 && appid >= AID_SHARED_GID_START) {
    snprintf(buffer, bufferlen, "all_a%u", appid - AID_SHARED_GID_START);
  } else if (appid < AID_APP) {
    for (size_t n = 0; n < android_id_count; n++) {
      if (android_ids[n].aid == appid) {
        snprintf(buffer, bufferlen, "u%u_%s", userid, android_ids[n].name);
        return;
      }
    }
  } else {
    snprintf(buffer, bufferlen, "u%u_a%u", userid, appid - AID_APP);
  }
}

static void print_app_name_from_uid(const uid_t uid, char* buffer, const int bufferlen) {
  const uid_t appid = uid % AID_USER;
  const uid_t userid = uid / AID_USER;
  return print_app_name_from_appid_userid(appid, userid, buffer, bufferlen);
}

// Translate a uid into the corresponding name.
// 0 to AID_APP-1                   -> "system", "radio", etc.
// AID_APP to AID_ISOLATED_START-1  -> u0_a1234
// AID_ISOLATED_START to AID_USER-1 -> u0_i1234
// AID_USER+                        -> u1_radio, u1_a1234, u2_i1234, etc.
// returns a passwd structure (sets errno to ENOENT on failure).
static passwd* app_id_to_passwd(uid_t uid, stubs_state_t* state) {
  passwd* pw = &state->passwd_;

  if (uid < AID_APP) {
    errno = ENOENT;
    return NULL;
  }

  const uid_t appid = uid % AID_USER;
  const uid_t userid = uid / AID_USER;

  print_app_name_from_appid_userid(appid, userid, state->app_name_buffer_,
                                   sizeof(state->app_name_buffer_));

  if (appid < AID_APP) {
      snprintf(state->dir_buffer_, sizeof(state->dir_buffer_), "/");
  } else {
      snprintf(state->dir_buffer_, sizeof(state->dir_buffer_), "/data");
  }

  snprintf(state->sh_buffer_, sizeof(state->sh_buffer_), "/system/bin/sh");

  pw->pw_name  = state->app_name_buffer_;
  pw->pw_dir   = state->dir_buffer_;
  pw->pw_shell = state->sh_buffer_;
  pw->pw_uid   = uid;
  pw->pw_gid   = uid;

  return pw;
}

// Translate a gid into the corresponding app_<gid>
// group structure (sets errno to ENOENT on failure).
static group* app_id_to_group(gid_t gid, stubs_state_t* state) {
  if (gid < AID_APP) {
    errno = ENOENT;
    return NULL;
  }

  print_app_name_from_uid(gid, state->group_name_buffer_,
                          sizeof(state->group_name_buffer_));

  group* gr = &state->group_;
  gr->gr_name   = state->group_name_buffer_;
  gr->gr_gid    = gid;
  gr->gr_mem[0] = gr->gr_name;
  gr->gr_mem[1] = NULL;
  return gr;
}


passwd* getpwuid(uid_t uid) { // NOLINT: implementing bad function.
  stubs_state_t* state = __stubs_state();
  if (state == NULL) {
    return NULL;
  }

  passwd* pw = android_id_to_passwd(state, uid);
  if (pw != NULL) {
    return pw;
  }
  return app_id_to_passwd(uid, state);
}

passwd* getpwnam(const char* login) { // NOLINT: implementing bad function.
  stubs_state_t* state = __stubs_state();
  if (state == NULL) {
    return NULL;
  }

  passwd* pw = android_name_to_passwd(state, login);
  if (pw != NULL) {
    return pw;
  }
  return app_id_to_passwd(app_id_from_name(login), state);
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
  return (pw != NULL) ? pw->pw_name : NULL;
}

group* getgrgid(gid_t gid) { // NOLINT: implementing bad function.
  stubs_state_t* state = __stubs_state();
  if (state == NULL) {
    return NULL;
  }

  group* gr = android_id_to_group(&state->group_, gid);
  if (gr != NULL) {
    return gr;
  }

  return app_id_to_group(gid, state);
}

group* getgrnam(const char* name) { // NOLINT: implementing bad function.
  stubs_state_t* state = __stubs_state();
  if (state == NULL) {
    return NULL;
  }

  if (android_name_to_group(&state->group_, name) != 0) {
    return &state->group_;
  }

  return app_id_to_group(app_id_from_name(name), state);
}

// We don't have an /etc/networks, so all inputs return NULL.
netent* getnetbyname(const char* /*name*/) {
  return NULL;
}

// We don't have an /etc/networks, so all inputs return NULL.
netent* getnetbyaddr(uint32_t /*net*/, int /*type*/) {
  return NULL;
}

// We don't have an /etc/protocols, so all inputs return NULL.
protoent* getprotobyname(const char* /*name*/) {
  return NULL;
}

// We don't have an /etc/protocols, so all inputs return NULL.
protoent* getprotobynumber(int /*proto*/) {
  return NULL;
}

static void unimplemented_stub(const char* function) {
  const char* fmt = "%s(3) is not implemented on Android\n";
  __libc_format_log(ANDROID_LOG_WARN, "libc", fmt, function);
  fprintf(stderr, fmt, function);
}

#define UNIMPLEMENTED unimplemented_stub(__PRETTY_FUNCTION__)

void endpwent() {
  UNIMPLEMENTED;
}

char* getusershell() {
  UNIMPLEMENTED;
  return NULL;
}

void setusershell() {
  UNIMPLEMENTED;
}

void endusershell() {
  UNIMPLEMENTED;
}

// Portable code should use sysconf(_SC_PAGE_SIZE) directly instead.
int getpagesize() {
  // We dont use sysconf(3) here because that drags in stdio, which makes static binaries fat.
  return PAGE_SIZE;
}
