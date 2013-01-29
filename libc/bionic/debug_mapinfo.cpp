/*
 * Copyright (C) 2012 The Android Open Source Project
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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "dlmalloc.h"
#include "debug_mapinfo.h"

// 6f000000-6f01e000 rwxp 00000000 00:0c 16389419   /system/lib/libcomposer.so
// 012345678901234567890123456789012345678901234567890123456789
// 0         1         2         3         4         5

static mapinfo_t* parse_maps_line(char* line) {
  int len = strlen(line);

  if (len < 1) return 0;
  line[--len] = 0;

  if (len < 50) return 0;
  if (line[20] != 'x') return 0;

  mapinfo_t* mi = static_cast<mapinfo_t*>(dlmalloc(sizeof(mapinfo_t) + (len - 47)));
  if (mi == 0) return 0;

  mi->start = strtoul(line, 0, 16);
  mi->end = strtoul(line + 9, 0, 16);
  mi->next = 0;
  strcpy(mi->name, line + 49);

  return mi;
}

__LIBC_HIDDEN__ mapinfo_t* mapinfo_create(pid_t pid) {
  struct mapinfo_t* milist = NULL;
  char data[1024]; // Used to read lines as well as to construct the filename.
  snprintf(data, sizeof(data), "/proc/%d/maps", pid);
  FILE* fp = fopen(data, "r");
  if (fp != NULL) {
    while (fgets(data, sizeof(data), fp) != NULL) {
      mapinfo_t* mi = parse_maps_line(data);
      if (mi) {
        mi->next = milist;
        milist = mi;
      }
    }
    fclose(fp);
  }
  return milist;
}

__LIBC_HIDDEN__ void mapinfo_destroy(mapinfo_t* mi) {
  while (mi != NULL) {
    mapinfo_t* del = mi;
    mi = mi->next;
    dlfree(del);
  }
}

// Find the containing map info for the PC.
__LIBC_HIDDEN__ const mapinfo_t* mapinfo_find(mapinfo_t* mi, uintptr_t pc, uintptr_t* rel_pc) {
  for (; mi != NULL; mi = mi->next) {
    if ((pc >= mi->start) && (pc < mi->end)) {
      *rel_pc = pc - mi->start;
      return mi;
    }
  }
  *rel_pc = pc;
  return NULL;
}
