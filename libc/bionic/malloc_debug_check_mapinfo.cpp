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
#include "malloc_debug_check_mapinfo.h"

// 6f000000-6f01e000 rwxp 00000000 00:0c 16389419   /system/lib/libcomposer.so
// 012345678901234567890123456789012345678901234567890123456789
// 0         1         2         3         4         5

static mapinfo* parse_maps_line(char* line) {
    int len = strlen(line);

    if (len < 1) return 0;
    line[--len] = 0;

    if (len < 50) return 0;
    if (line[20] != 'x') return 0;

    mapinfo* mi = static_cast<mapinfo*>(dlmalloc(sizeof(mapinfo) + (len - 47)));
    if (mi == 0) return 0;

    mi->start = strtoul(line, 0, 16);
    mi->end = strtoul(line + 9, 0, 16);
    /* To be filled in parse_elf_info if the mapped section starts with
     * elf_header
     */
    mi->next = 0;
    strcpy(mi->name, line + 49);

    return mi;
}

__LIBC_HIDDEN__
mapinfo *init_mapinfo(int pid) {
    struct mapinfo *milist = NULL;
    char data[1024]; // Used to read lines as well as to construct the filename.
    snprintf(data, sizeof(data), "/proc/%d/maps", pid);
    FILE *fp = fopen(data, "r");
    if (fp) {
        while (fgets(data, sizeof(data), fp)) {
            mapinfo *mi = parse_maps_line(data);
            if (mi) {
                mi->next = milist;
                milist = mi;
            }
        }
        fclose(fp);
    }

    return milist;
}

__LIBC_HIDDEN__
void deinit_mapinfo(mapinfo *mi) {
    mapinfo *del;
    while (mi) {
        del = mi;
        mi = mi->next;
        dlfree(del);
    }
}

/* Map a pc address to the name of the containing ELF file */
__LIBC_HIDDEN__
const char *map_to_name(mapinfo *mi, unsigned pc, const char* def) {
    while (mi) {
        if ((pc >= mi->start) && (pc < mi->end)) {
            return mi->name;
        }
        mi = mi->next;
    }
    return def;
}

/* Find the containing map info for the pc */
__LIBC_HIDDEN__
const mapinfo *pc_to_mapinfo(mapinfo *mi, unsigned pc, unsigned *rel_pc) {
    *rel_pc = pc;
    while (mi) {
        if ((pc >= mi->start) && (pc < mi->end)) {
            // Only calculate the relative offset for shared libraries
            if (strstr(mi->name, ".so")) {
                *rel_pc -= mi->start;
            }
            return mi;
        }
        mi = mi->next;
    }
    return NULL;
}
