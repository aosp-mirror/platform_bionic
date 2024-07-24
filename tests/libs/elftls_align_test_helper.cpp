/*
 * Copyright (C) 2024 The Android Open Source Project
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

#include <stdint.h>

#include "CHECK.h"

struct AlignedVar {
  int field;
  char buffer[0x1000 - sizeof(int)];
} __attribute__((aligned(0x400)));

struct SmallVar {
  int field;
  char buffer[0xeee - sizeof(int)];
};

// The single .tdata section should have a size that isn't a multiple of its
// alignment.
__thread struct AlignedVar var1 = {13};
__thread struct AlignedVar var2 = {17};
__thread struct SmallVar var3 = {19};

static uintptr_t var_addr(void* value) {
  // Maybe the optimizer would assume that the variable has the alignment it is
  // declared with.
  asm volatile("" : "+r,m"(value) : : "memory");
  return reinterpret_cast<uintptr_t>(value);
}

int main() {
  CHECK((var_addr(&var1) & 0x3ff) == 0);
  CHECK((var_addr(&var2) & 0x3ff) == 0);
  CHECK(var1.field == 13);
  CHECK(var2.field == 17);
  CHECK(var3.field == 19);
  return 0;
}
