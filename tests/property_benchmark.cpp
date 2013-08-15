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

#include "benchmark.h"
#include <unistd.h>

#define _REALLY_INCLUDE_SYS__SYSTEM_PROPERTIES_H_
#include <sys/_system_properties.h>

#include <vector>
#include <string>

extern void *__system_property_area__;

#define TEST_NUM_PROPS \
    Arg(1)->Arg(4)->Arg(16)->Arg(64)->Arg(128)->Arg(256)->Arg(512)->Arg(1024)

struct LocalPropertyTestState {
    LocalPropertyTestState(int nprops) : nprops(nprops), valid(false) {
        static const char prop_name_chars[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ.-_";

        char dir_template[] = "/data/local/tmp/prop-XXXXXX";
        char *dirname = mkdtemp(dir_template);
        if (!dirname) {
            perror("making temp file for test state failed (is /data/local/tmp writable?)");
            return;
        }

        old_pa = __system_property_area__;
        __system_property_area__ = NULL;

        pa_dirname = dirname;
        pa_filename = pa_dirname + "/__properties__";

        __system_property_set_filename(pa_filename.c_str());
        __system_property_area_init();

        names = new char* [nprops];
        name_lens = new int[nprops];
        values = new char* [nprops];
        value_lens = new int[nprops];

        srandom(nprops);

        for (int i = 0; i < nprops; i++) {
            name_lens[i] = random() % PROP_NAME_MAX;
            names[i] = new char[PROP_NAME_MAX + 1];
            for (int j = 0; j < name_lens[i]; j++) {
                names[i][j] = prop_name_chars[random() % (sizeof(prop_name_chars) - 1)];
            }
            names[i][name_lens[i]] = 0;
            value_lens[i] = random() % PROP_VALUE_MAX;
            values[i] = new char[PROP_VALUE_MAX];
            for (int j = 0; j < value_lens[i]; j++) {
                values[i][j] = prop_name_chars[random() % (sizeof(prop_name_chars) - 1)];
            }
            __system_property_add(names[i], name_lens[i], values[i], value_lens[i]);
        }

        valid = true;
    }

    ~LocalPropertyTestState() {
        if (!valid)
            return;

        __system_property_area__ = old_pa;

        __system_property_set_filename(PROP_FILENAME);
        unlink(pa_filename.c_str());
        rmdir(pa_dirname.c_str());

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
    char **names;
    int *name_lens;
    char **values;
    int *value_lens;
    bool valid;

private:
    std::string pa_dirname;
    std::string pa_filename;
    void *old_pa;
};

static void BM_property_get(int iters, int nprops)
{
    StopBenchmarkTiming();

    LocalPropertyTestState pa(nprops);
    char value[PROP_VALUE_MAX];

    if (!pa.valid)
        return;

    srandom(iters * nprops);

    StartBenchmarkTiming();

    for (int i = 0; i < iters; i++) {
        __system_property_get(pa.names[random() % nprops], value);
    }
    StopBenchmarkTiming();
}
BENCHMARK(BM_property_get)->TEST_NUM_PROPS;

static void BM_property_find(int iters, int nprops)
{
    StopBenchmarkTiming();

    LocalPropertyTestState pa(nprops);

    if (!pa.valid)
        return;

    srandom(iters * nprops);

    StartBenchmarkTiming();

    for (int i = 0; i < iters; i++) {
        __system_property_find(pa.names[random() % nprops]);
    }
    StopBenchmarkTiming();
}
BENCHMARK(BM_property_find)->TEST_NUM_PROPS;
