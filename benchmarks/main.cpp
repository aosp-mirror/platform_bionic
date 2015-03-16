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


#include <regex.h>
#include <stdio.h>
#include <stdlib.h>

#include <vector>

#include <benchmark/Benchmark.h>

int main(int argc, char* argv[]) {
  if (::testing::Benchmark::List().empty()) {
    fprintf(stderr, "No benchmarks registered!\n");
    exit(EXIT_FAILURE);
  }

  std::vector<regex_t*> regs;
  for (int i = 1; i < argc; i++) {
    regex_t* re = new regex_t;
    int errcode = regcomp(re, argv[i], 0);
    if (errcode != 0) {
      size_t errbuf_size = regerror(errcode, re, NULL, 0);
      if (errbuf_size > 0) {
        char* errbuf = new char[errbuf_size];
        regerror(errcode, re, errbuf, errbuf_size);
        fprintf(stderr, "Couldn't compile \"%s\" as a regular expression: %s\n",
                argv[i], errbuf);
      } else {
        fprintf(stderr, "Unknown compile error for \"%s\" as a regular expression!\n", argv[i]);
      }
      exit(EXIT_FAILURE);
    }
    regs.push_back(re);
  }

  if (::testing::Benchmark::RunAll(regs) == 0) {
    fprintf(stderr, "No matching benchmarks!\n");
    fprintf(stderr, "Available benchmarks:\n");
    for (const auto& benchmark : ::testing::Benchmark::List()) {
      fprintf(stderr, "  %s\n", benchmark->Name().c_str());
    }
    exit(EXIT_FAILURE);
  }

  return 0;
}
