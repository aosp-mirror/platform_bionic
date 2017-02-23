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

#include <mntent.h>
#include <string.h>

#include "bionic/pthread_internal.h"

mntent* getmntent(FILE* fp) {
  auto& tls = __get_bionic_tls();
  return getmntent_r(fp, &tls.mntent_buf, tls.mntent_strings, sizeof(tls.mntent_strings));
}

mntent* getmntent_r(FILE* fp, struct mntent* e, char* buf, int buf_len) {
  memset(e, 0, sizeof(*e));
  while (fgets(buf, buf_len, fp) != NULL) {
    // Entries look like "proc /proc proc rw,nosuid,nodev,noexec,relatime 0 0".
    // That is: mnt_fsname mnt_dir mnt_type mnt_opts 0 0.
    int fsname0, fsname1, dir0, dir1, type0, type1, opts0, opts1;
    if (sscanf(buf, " %n%*s%n %n%*s%n %n%*s%n %n%*s%n %d %d",
               &fsname0, &fsname1, &dir0, &dir1, &type0, &type1, &opts0, &opts1,
               &e->mnt_freq, &e->mnt_passno) == 2) {
      e->mnt_fsname = &buf[fsname0];
      buf[fsname1] = '\0';

      e->mnt_dir = &buf[dir0];
      buf[dir1] = '\0';

      e->mnt_type = &buf[type0];
      buf[type1] = '\0';

      e->mnt_opts = &buf[opts0];
      buf[opts1] = '\0';

      return e;
    }
  }
  return NULL;
}

FILE* setmntent(const char* path, const char* mode) {
  return fopen(path, mode);
}

int endmntent(FILE* fp) {
  if (fp != NULL) {
    fclose(fp);
  }
  return 1;
}

char* hasmntopt(const struct mntent* mnt, const char* opt) {
  char* token = mnt->mnt_opts;
  char* const end = mnt->mnt_opts + strlen(mnt->mnt_opts);
  const size_t optLen = strlen(opt);

  while (token) {
    char* const tokenEnd = token + optLen;
    if (tokenEnd > end) break;

    if (memcmp(token, opt, optLen) == 0 &&
        (*tokenEnd == '\0' || *tokenEnd == ',' || *tokenEnd == '=')) {
      return token;
    }

    token = strchr(token, ',');
    if (token) token++;
  }

  return nullptr;
}
