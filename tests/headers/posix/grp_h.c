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

#include <grp.h>

#include "header_checks.h"

static void grp_h() {
  TYPE(struct group);
  STRUCT_MEMBER(struct group, char*, gr_name);
  STRUCT_MEMBER(struct group, gid_t, gr_gid);
  STRUCT_MEMBER(struct group, char**, gr_mem);

  TYPE(gid_t);
  TYPE(size_t);

  FUNCTION(endgrent, void (*f)(void));
  FUNCTION(getgrent, struct group* (*f)(void));
  FUNCTION(getgrgid, struct group* (*f)(gid_t));
  FUNCTION(getgrgid_r, int (*f)(gid_t, struct group*, char*, size_t, struct group**));
  FUNCTION(getgrnam, struct group* (*f)(const char*));
  FUNCTION(getgrnam_r, int (*f)(const char*, struct group*, char*, size_t, struct group**));
  FUNCTION(setgrent, void (*f)(void));
}
