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
#include <private/android_filesystem_config.h>
#include <private/logd.h>
#include <pthread.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static int do_getpw_r(int by_name, const char* name, uid_t uid,
        struct passwd* dst, char* buf, size_t byte_count, struct passwd** result)
{
    /*
     * getpwnam_r and getpwuid_r don't modify errno, but library calls we
     *  make might.
     */
    int old_errno = errno;
    int rc = 0;
    *result = NULL;

    const struct passwd* src = by_name ? getpwnam(name) : getpwuid(uid);

    /*
     * POSIX allows failure to find a match to be considered a non-error.
     * Reporting success (0) but with *result NULL is glibc's behavior.
     */
    if (src == NULL) {
        rc = (errno == ENOENT) ? 0 : errno;
        goto failure;
    }

    /*
     * Work out where our strings will go in 'buf', and whether we've got
     * enough space.
     */
    size_t required_byte_count = 0;
    dst->pw_name = buf;
    required_byte_count += strlen(src->pw_name) + 1;
    dst->pw_dir = buf + required_byte_count;
    required_byte_count += strlen(src->pw_dir) + 1;
    dst->pw_shell = buf + required_byte_count;
    required_byte_count += strlen(src->pw_shell) + 1;
    if (byte_count < required_byte_count) {
        rc = ERANGE;
        goto failure;
    }

    /* Copy the strings. */
    snprintf(buf, byte_count, "%s%c%s%c%s",
            src->pw_name, 0, src->pw_dir, 0, src->pw_shell);

    /*
     * pw_passwd is non-POSIX and unused (always NULL) in bionic.
     * pw_gecos is non-POSIX and missing in bionic.
     */
    dst->pw_passwd = NULL;

    /* Copy the integral fields. */
    dst->pw_gid = src->pw_gid;
    dst->pw_uid = src->pw_uid;

success:
    rc = 0;
    *result = dst;
failure:
    errno = old_errno;
    return rc;
}

int getpwnam_r(const char* name, struct passwd* pwd,
        char* buf, size_t byte_count, struct passwd** result)
{
    return do_getpw_r(1, name, -1, pwd, buf, byte_count, result);
}

int getpwuid_r(uid_t uid, struct passwd* pwd,
        char* buf, size_t byte_count, struct passwd** result)
{
    return do_getpw_r(0, NULL, uid, pwd, buf, byte_count, result);
}

/** Thread-specific state for the stubs functions
 **/

static pthread_once_t   the_once = PTHREAD_ONCE_INIT;
static pthread_key_t    the_key;

typedef struct {
    struct passwd  passwd;
    struct group   group;
    char*          group_members[2];
    char           app_name_buffer[32];
    char           group_name_buffer[32];
} stubs_state_t;

static void
stubs_state_free( void*  _s )
{
    stubs_state_t*  s = _s;
    free(s);
}

static stubs_state_t*
stubs_state_alloc( void )
{
    stubs_state_t*  s = calloc(1, sizeof *s);

    if (s != NULL) {
        s->group.gr_mem = s->group_members;
    }
    return s;
}

static void __stubs_key_init(void)
{
    pthread_key_create( &the_key, stubs_state_free );
}

static stubs_state_t*
__stubs_state(void)
{
    stubs_state_t*  s;

    pthread_once(&the_once, __stubs_key_init);
    s = pthread_getspecific(the_key);
    if (s == NULL) {
        s = stubs_state_alloc();
        if (s == NULL) {
            errno = ENOMEM;  /* just in case */
        } else {
            if ( pthread_setspecific(the_key, s) != 0 ) {
                stubs_state_free(s);
                errno = ENOMEM;
                s     = NULL;
            }
        }
    }
    return s;
}

static struct passwd*
android_iinfo_to_passwd( struct passwd          *pw,
                         struct android_id_info *iinfo )
{
    pw->pw_name  = (char*)iinfo->name;
    pw->pw_uid   = iinfo->aid;
    pw->pw_gid   = iinfo->aid;
    pw->pw_dir   = "/";
    pw->pw_shell = "/system/bin/sh";
    return pw;
}

static struct group*
android_iinfo_to_group( struct group *gr,
                        struct android_id_info *iinfo )
{
    gr->gr_name   = (char*) iinfo->name;
    gr->gr_gid    = iinfo->aid;
    gr->gr_mem[0] = gr->gr_name;
    gr->gr_mem[1] = NULL;
    return gr;
}

static struct passwd *
android_id_to_passwd( struct passwd *pw, unsigned id)
{
    struct android_id_info *iinfo = android_ids;
    unsigned n;
    for (n = 0; n < android_id_count; n++) {
        if (iinfo[n].aid == id) {
            return android_iinfo_to_passwd(pw, iinfo + n);
        }
    }
    return NULL;
}

static struct passwd*
android_name_to_passwd(struct passwd *pw, const char *name)
{
    struct android_id_info *iinfo = android_ids;
    unsigned n;
    for (n = 0; n < android_id_count; n++) {
        if (!strcmp(iinfo[n].name, name)) {
            return android_iinfo_to_passwd(pw, iinfo + n);
        }
    }
    return NULL;
}

static struct group*
android_id_to_group( struct group *gr, unsigned id )
{
    struct android_id_info *iinfo = android_ids;
    unsigned n;
    for (n = 0; n < android_id_count; n++) {
        if (iinfo[n].aid == id) {
            return android_iinfo_to_group(gr, iinfo + n);
        }
    }
    return NULL;
}

static struct group*
android_name_to_group( struct group *gr, const char *name )
{
    struct android_id_info *iinfo = android_ids;
    unsigned n;
    for (n = 0; n < android_id_count; n++) {
        if (!strcmp(iinfo[n].name, name)) {
            return android_iinfo_to_group(gr, iinfo + n);
        }
    }
    return NULL;
}

/* translate a user/group name to the corresponding user/group id.
 * u0_a1234 -> 0 * AID_USER + AID_APP + 1234
 * u2_i1000 -> 2 * AID_USER + AID_ISOLATED_START + 1000
 * u1_system -> 1 * AID_USER + android_ids['system']
 * returns 0 and sets errno to ENOENT in case of error
 */
static unsigned
app_id_from_name( const char*  name )
{
    unsigned long  userid;
    unsigned long  appid = 0;
    char*          end;

    if (name[0] != 'u' || !isdigit(name[1]))
        goto FAIL;

    userid = strtoul(name+1, &end, 10);
    if (end[0] != '_' || end[1] == 0)
        goto FAIL;

    if (end[1] == 'a' && isdigit(end[2])) {
        /* end will point to \0 if the strtoul below succeeds. */
        appid = strtoul(end+2, &end, 10) + AID_APP;
    } else if (end[1] == 'i' && isdigit(end[2])) {
        /* end will point to \0 if the strtoul below succeeds. */
        appid = strtoul(end+2, &end, 10) + AID_ISOLATED_START;
    } else {
        for (size_t n = 0; n < android_id_count; n++) {
            if (!strcmp(android_ids[n].name, end + 1)) {
                appid = android_ids[n].aid;
                /* move the end pointer to the null terminator */
                end += 1 + strlen(android_ids[n].name);
            }
        }
    }

    /* check if the entire string was consumed by the 3 cases above */
    if (end[0] != 0)
        goto FAIL;

    /* check that user id won't overflow */
    if (userid > 1000)
        goto FAIL;

    /* check that app id is within range */
    if (appid >= AID_USER)
        goto FAIL;

    return (unsigned)(appid + userid*AID_USER);

FAIL:
    errno = ENOENT;
    return 0;
}

static void
print_app_uid_name(uid_t  uid, char* buffer, int bufferlen)
{
    uid_t appid;
    uid_t userid;

    appid = uid % AID_USER;
    userid = uid / AID_USER;

    if (appid < AID_ISOLATED_START) {
        if (appid < AID_APP) {
            for (size_t n = 0; n < android_id_count; n++) {
                if (android_ids[n].aid == appid) {
                    snprintf(buffer, bufferlen, "u%u_%s", userid, android_ids[n].name);
                    return;
                }
            }
        }
        snprintf(buffer, bufferlen, "u%u_a%u", userid, appid - AID_APP);
    } else {
        snprintf(buffer, bufferlen, "u%u_i%u", userid, appid - AID_ISOLATED_START);
    }
}

/* translate a uid into the corresponding name.
 * 0 to AID_APP-1                   -> "system", "radio", etc.
 * AID_APP to AID_ISOLATED_START-1  -> u0_a1234
 * AID_ISOLATED_START to AID_USER-1 -> u0_i1234
 * AID_USER+                        -> u1_radio, u1_a1234, u2_i1234, etc.
 * returns a passwd structure (sets errno to ENOENT on failure)
 */
static struct passwd*
app_id_to_passwd(uid_t  uid, stubs_state_t*  state)
{
    struct passwd*  pw = &state->passwd;

    if (uid < AID_APP) {
        errno = ENOENT;
        return NULL;
    }

    print_app_uid_name(uid, state->app_name_buffer, sizeof state->app_name_buffer);

    pw->pw_name  = state->app_name_buffer;
    pw->pw_dir   = "/data";
    pw->pw_shell = "/system/bin/sh";
    pw->pw_uid   = uid;
    pw->pw_gid   = uid;

    return pw;
}

/* translate a gid into the corresponding app_<gid>
 * group structure (sets errno to ENOENT on failure)
 */
static struct group*
app_id_to_group(gid_t  gid, stubs_state_t*  state)
{
    struct group*  gr = &state->group;
    int appid;
    int userid;

    if (gid < AID_APP) {
        errno = ENOENT;
        return NULL;
    }

    print_app_uid_name(gid, state->group_name_buffer, sizeof state->group_name_buffer);

    gr->gr_name   = state->group_name_buffer;
    gr->gr_gid    = gid;
    gr->gr_mem[0] = gr->gr_name;
    gr->gr_mem[1] = NULL;

    return gr;
}


struct passwd*
getpwuid(uid_t uid)
{
    stubs_state_t*  state = __stubs_state();
    struct passwd*  pw;

    if (state == NULL)
        return NULL;

    pw = &state->passwd;

    if ( android_id_to_passwd(pw, uid) != NULL )
        return pw;

    return app_id_to_passwd(uid, state);
}

struct passwd*
getpwnam(const char *login)
{
    stubs_state_t*  state = __stubs_state();

    if (state == NULL)
        return NULL;

    if (android_name_to_passwd(&state->passwd, login) != NULL)
        return &state->passwd;

    return app_id_to_passwd( app_id_from_name(login), state );
}

int
getgrouplist (const char *user, gid_t group,
              gid_t *groups, int *ngroups)
{
    if (*ngroups < 1) {
        *ngroups = 1;
        return -1;
    }
    groups[0] = group;
    return (*ngroups = 1);
}

char*
getlogin(void)
{
    struct passwd *pw = getpwuid(getuid());

    if(pw) {
        return pw->pw_name;
    } else {
        return NULL;
    }
}

struct group*
getgrgid(gid_t gid)
{
    stubs_state_t*  state = __stubs_state();
    struct group*   gr;

    if (state == NULL)
        return NULL;

    gr = android_id_to_group(&state->group, gid);
    if (gr != NULL)
        return gr;

    return app_id_to_group(gid, state);
}

struct group*
getgrnam(const char *name)
{
    stubs_state_t*  state = __stubs_state();
    unsigned        id;

    if (state == NULL)
        return NULL;

    if (android_name_to_group(&state->group, name) != 0)
        return &state->group;

    return app_id_to_group( app_id_from_name(name), state );
}

static void unimplemented_stub(const char* function) {
  const char* fmt = "%s(3) is not implemented on Android\n";
  __libc_android_log_print(ANDROID_LOG_WARN, "libc", fmt, function);
  fprintf(stderr, fmt, function);
}

#define UNIMPLEMENTED unimplemented_stub(__PRETTY_FUNCTION__)

struct netent* getnetbyname(const char* name) {
  UNIMPLEMENTED;
  return NULL;
}

void endpwent(void) {
  UNIMPLEMENTED;
}

struct mntent* getmntent(FILE* f) {
  UNIMPLEMENTED;
  return NULL;
}

char* ttyname(int fd) {
  UNIMPLEMENTED;
  return NULL;
}

int ttyname_r(int fd, char* buf, size_t buflen) {
  UNIMPLEMENTED;
  return -ERANGE;
}

struct netent* getnetbyaddr(uint32_t net, int type) {
  UNIMPLEMENTED;
  return NULL;
}

struct protoent* getprotobyname(const char* name) {
  UNIMPLEMENTED;
  return NULL;
}

struct protoent* getprotobynumber(int proto) {
  UNIMPLEMENTED;
  return NULL;
}

char* getusershell(void) {
  UNIMPLEMENTED;
  return NULL;
}

void setusershell(void) {
  UNIMPLEMENTED;
}

void endusershell(void) {
  UNIMPLEMENTED;
}
