/*
 * Copyright (C) 2020 The Android Open Source Project
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
#include <stdlib.h>

#include <string>

extern "C" int __cxa_atexit(void (*func)(void*), void* arg, void* dso);

extern void* __dso_handle;

static void dtor_func(void*) {}

// Prevent the compiler from optimizing out the __cxa_atexit call.
void (*volatile g_pdtor_func)(void*) = dtor_func;

int main(int argc, char* argv[]) {
  auto usage = [&argv]() {
    fprintf(stderr, "usage: %s COUNT MODE\n", argv[0]);
    fprintf(stderr, "MODE is one of '_Exit' or 'exit'.\n");
    exit(1);
  };

  if (argc != 3) usage();

  int count = atoi(argv[1]);

  // Two modes: "_Exit" ==> exit early w/o calling dtors, "exit" ==> call dtors on exit.
  std::string mode = argv[2];
  if (mode != "_Exit" && mode != "exit") usage();

  for (int i = 0; i < count; ++i) {
    __cxa_atexit(g_pdtor_func, nullptr, &__dso_handle);
  }

  if (mode == "_Exit") {
    _Exit(0);
  } else {
    exit(0);
  }
}
