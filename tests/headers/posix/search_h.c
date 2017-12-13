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

#include <search.h>

#include "header_checks.h"

static void search_h() {
  TYPE(ENTRY);
  TYPE(struct entry);
  STRUCT_MEMBER(ENTRY, char*, key);
  STRUCT_MEMBER(ENTRY, void*, data);

  ACTION a;
  a = FIND;
  a = ENTER;

  VISIT v;
  v = preorder;
  v = postorder;
  v = endorder;
  v = leaf;

  TYPE(size_t);

  FUNCTION(hcreate, int (*f)(size_t));
  FUNCTION(hdestroy, void (*f)(void));
  FUNCTION(hsearch, ENTRY* (*f)(ENTRY, ACTION));
  FUNCTION(insque, void (*f)(void*, void*));
  FUNCTION(lfind, void* (*f)(const void*, const void*, size_t*, size_t,
                             int (*)(const void*, const void*)));
  FUNCTION(lsearch, void* (*f)(const void*, void*, size_t*, size_t,
                               int (*)(const void*, const void*)));
  FUNCTION(remque, void (*f)(void*));
  FUNCTION(tdelete, void* (*f)(const void*, void**, int (*)(const void*, const void*)));
  FUNCTION(tfind, void* (*f)(const void*, void* const*, int (*)(const void*, const void*)));
  FUNCTION(tsearch, void* (*f)(const void*, void**, int (*)(const void*, const void*)));
  FUNCTION(twalk, void (*f)(const void*, void (*)(const void*, VISIT, int)));
}
