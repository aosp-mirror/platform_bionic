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

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <string>

#include <android-base/file.h>

using namespace std::literals;

#if defined(__BIONIC__)

#define _REALLY_INCLUDE_SYS__SYSTEM_PROPERTIES_H_
#include <sys/_system_properties.h>

#include <benchmark/benchmark.h>
#include <system_properties/system_properties.h>
#include "util.h"

struct LocalPropertyTestState {
  explicit LocalPropertyTestState(int nprops)
      : nprops(nprops), valid(false), system_properties_(false) {
    static const char prop_name_chars[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ-_.";

    valid = system_properties_.AreaInit(dir_.path, nullptr);
    if (!valid) {
      return;
    }

    names = new char* [nprops];
    name_lens = new int[nprops];
    values = new char* [nprops];
    value_lens = new int[nprops];

    srandom(nprops);

    for (int i = 0; i < nprops; i++) {
      // Make sure the name has at least 10 characters to make
      // it very unlikely to generate the same random name.
      name_lens[i] = (random() % (PROP_NAME_MAX - 10)) + 10;
      names[i] = new char[PROP_NAME_MAX + 1];
      size_t prop_name_len = sizeof(prop_name_chars) - 1;
      for (int j = 0; j < name_lens[i]; j++) {
        if (j == 0 || names[i][j-1] == '.' || j == name_lens[i] - 1) {
          // Certain values are not allowed:
          // - Don't start name with '.'
          // - Don't allow '.' to appear twice in a row
          // - Don't allow the name to end with '.'
          // This assumes that '.' is the last character in the
          // array so that decrementing the length by one removes
          // the value from the possible values.
          prop_name_len--;
        }
        names[i][j] = prop_name_chars[random() % prop_name_len];
      }
      names[i][name_lens[i]] = 0;

      // Make sure the value contains at least 1 character.
      value_lens[i] = (random() % (PROP_VALUE_MAX - 1)) + 1;
      values[i] = new char[PROP_VALUE_MAX];
      for (int j = 0; j < value_lens[i]; j++) {
        values[i][j] = prop_name_chars[random() % (sizeof(prop_name_chars) - 1)];
      }

      if (system_properties_.Add(names[i], name_lens[i], values[i], value_lens[i]) < 0) {
        printf("Failed to add a property, terminating...\n");
        printf("%s = %.*s\n", names[i], value_lens[i], values[i]);
        exit(1);
      }
    }

    valid = true;
  }

  SystemProperties& system_properties() {
    return system_properties_;
  }

  ~LocalPropertyTestState() {
    if (!valid) {
      return;
    }

    system_properties_.contexts_->FreeAndUnmap();

    for (int i = 0; i < nprops; i++) {
      delete names[i];
      delete values[i];
    }
    delete[] names;
    delete[] name_lens;
    delete[] values;
    delete[] value_lens;
  }

 public:
  const int nprops;
  char** names;
  int* name_lens;
  char** values;
  int* value_lens;
  bool valid;

 private:
  SystemProperties system_properties_;
  TemporaryDir dir_;
};

static void BM_property_get(benchmark::State& state) {
  const size_t nprops = state.range(0);

  LocalPropertyTestState pa(nprops);
  if (!pa.valid) return;

  while (state.KeepRunning()) {
    char value[PROP_VALUE_MAX];
    pa.system_properties().Get(pa.names[random() % nprops], value);
  }
}
BIONIC_BENCHMARK_WITH_ARG(BM_property_get, "NUM_PROPS");

static void BM_property_find(benchmark::State& state) {
  const size_t nprops = state.range(0);

  LocalPropertyTestState pa(nprops);
  if (!pa.valid) return;

  while (state.KeepRunning()) {
    pa.system_properties().Find(pa.names[random() % nprops]);
  }
}
BIONIC_BENCHMARK_WITH_ARG(BM_property_find, "NUM_PROPS");

static void BM_property_read(benchmark::State& state) {
  const size_t nprops = state.range(0);

  LocalPropertyTestState pa(nprops);
  if (!pa.valid) return;

  const prop_info** pinfo = new const prop_info*[nprops];
  char propvalue[PROP_VALUE_MAX];

  for (size_t i = 0; i < nprops; ++i) {
    pinfo[i] = pa.system_properties().Find(pa.names[random() % nprops]);
  }

  size_t i = 0;
  while (state.KeepRunning()) {
    pa.system_properties().Read(pinfo[i], nullptr, propvalue);
    i = (i + 1) % nprops;
  }

  delete[] pinfo;
}
BIONIC_BENCHMARK_WITH_ARG(BM_property_read, "NUM_PROPS");

static void BM_property_serial(benchmark::State& state) {
  const size_t nprops = state.range(0);

  LocalPropertyTestState pa(nprops);
  if (!pa.valid) return;

  const prop_info** pinfo = new const prop_info*[nprops];
  for (size_t i = 0; i < nprops; ++i) {
    pinfo[i] = pa.system_properties().Find(pa.names[random() % nprops]);
  }

  size_t i = 0;
  while (state.KeepRunning()) {
    pa.system_properties().Serial(pinfo[i]);
    i = (i + 1) % nprops;
  }

  delete[] pinfo;
}
BIONIC_BENCHMARK_WITH_ARG(BM_property_serial, "NUM_PROPS");

#endif  // __BIONIC__
