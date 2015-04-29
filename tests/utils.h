/*
 * Copyright (C) 2012 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __TEST_UTILS_H
#define __TEST_UTILS_H
#include <inttypes.h>
#include <sys/mman.h>

#include "private/ScopeGuard.h"

struct map_record {
  uintptr_t addr_start;
  uintptr_t addr_end;

  int perms;

  size_t offset;

  dev_t device;
  ino_t inode;

  std::string pathname;
};

class Maps {
 public:
  static bool parse_maps(std::vector<map_record>* maps) {
    char path[64];
    snprintf(path, sizeof(path), "/proc/self/task/%d/maps", getpid());
    FILE* fp = fopen(path, "re");
    if (fp == nullptr) {
      return false;
    }

    auto fp_guard = make_scope_guard([&]() {
      fclose(fp);
    });

    char line[BUFSIZ];
    while (fgets(line, sizeof(line), fp) != nullptr) {
      map_record record;
      uint32_t dev_major, dev_minor;
      char pathstr[BUFSIZ];
      char prot[5]; // sizeof("rwxp")
      if (sscanf(line, "%" SCNxPTR "-%" SCNxPTR " %4s %" SCNxPTR " %x:%x %lu %s",
            &record.addr_start, &record.addr_end, prot, &record.offset,
            &dev_major, &dev_minor, &record.inode, pathstr) == 8) {
        record.perms = 0;
        if (prot[0] == 'r') {
          record.perms |= PROT_READ;
        }
        if (prot[1] == 'w') {
          record.perms |= PROT_WRITE;
        }
        if (prot[2] == 'x') {
          record.perms |= PROT_EXEC;
        }

        // TODO: parse shared/private?

        record.device = makedev(dev_major, dev_minor);
        record.pathname = pathstr;
        maps->push_back(record);
      }
    }

    return true;
  }
};

#endif
