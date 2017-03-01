/*
 * Copyright (C) 2013 The Android Open Source Project
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

#include <gtest/gtest.h>
#include "BionicDeathTest.h"

#include <errno.h>
#include <sys/wait.h>
#include <unistd.h>

#include <string>
#include <thread>

#if defined(__BIONIC__)

#define _REALLY_INCLUDE_SYS__SYSTEM_PROPERTIES_H_
#include <sys/_system_properties.h>

struct LocalPropertyTestState {
    LocalPropertyTestState() : valid(false) {
        const char* ANDROID_DATA = getenv("ANDROID_DATA");
        char dir_template[PATH_MAX];
        snprintf(dir_template, sizeof(dir_template), "%s/local/tmp/prop-XXXXXX", ANDROID_DATA);
        char* dirname = mkdtemp(dir_template);
        if (!dirname) {
            fprintf(stderr, "making temp file for test state failed (is %s writable?): %s",
                    dir_template, strerror(errno));
            return;
        }

        pa_dirname = dirname;
        pa_filename = pa_dirname + "/__properties__";

        __system_property_set_filename(pa_filename.c_str());
        __system_property_area_init();
        valid = true;
    }

    ~LocalPropertyTestState() {
        if (!valid) {
            return;
        }

        __system_property_set_filename(PROP_FILENAME);
        __system_properties_init();
        unlink(pa_filename.c_str());
        rmdir(pa_dirname.c_str());
    }
public:
    bool valid;
private:
    std::string pa_dirname;
    std::string pa_filename;
};

static void foreach_test_callback(const prop_info *pi, void* cookie) {
    size_t *count = static_cast<size_t *>(cookie);

    ASSERT_TRUE(pi != nullptr);
    (*count)++;
}

static void hierarchical_test_callback(const prop_info *pi, void *cookie) {
    bool (*ok)[8][8] = static_cast<bool (*)[8][8]>(cookie);

    char name[PROP_NAME_MAX];
    char value[PROP_VALUE_MAX];

    __system_property_read(pi, name, value);

    int name_i, name_j, name_k;
    int value_i, value_j, value_k;
    ASSERT_EQ(3, sscanf(name, "property_%d.%d.%d", &name_i, &name_j, &name_k));
    ASSERT_EQ(3, sscanf(value, "value_%d.%d.%d", &value_i, &value_j, &value_k));
    ASSERT_EQ(name_i, value_i);
    ASSERT_GE(name_i, 0);
    ASSERT_LT(name_i, 8);
    ASSERT_EQ(name_j, value_j);
    ASSERT_GE(name_j, 0);
    ASSERT_LT(name_j, 8);
    ASSERT_EQ(name_k, value_k);
    ASSERT_GE(name_k, 0);
    ASSERT_LT(name_k, 8);

    ok[name_i][name_j][name_k] = true;
}

static void* PropertyWaitHelperFn(void* arg) {
    int* flag = static_cast<int*>(arg);
    prop_info* pi = const_cast<prop_info*>(__system_property_find("property"));
    usleep(100000);

    *flag = 1;
    __system_property_update(pi, "value3", 6);

    return nullptr;
}

#endif // __BIONIC__

TEST(properties, __system_property_add) {
#if defined(__BIONIC__)
    LocalPropertyTestState pa;
    ASSERT_TRUE(pa.valid);

    ASSERT_EQ(0, __system_property_add("property", 8, "value1", 6));
    ASSERT_EQ(0, __system_property_add("other_property", 14, "value2", 6));
    ASSERT_EQ(0, __system_property_add("property_other", 14, "value3", 6));

    // check that there is no limit on property name length
    char name[PROP_NAME_MAX + 11];
    name[0] = 'p';
    for (size_t i = 1; i < sizeof(name); i++) {
      name[i] = 'x';
    }

    name[sizeof(name)-1] = '\0';
    ASSERT_EQ(0, __system_property_add(name, strlen(name), "value", 5));

    char propvalue[PROP_VALUE_MAX];
    ASSERT_EQ(6, __system_property_get("property", propvalue));
    ASSERT_STREQ(propvalue, "value1");

    ASSERT_EQ(6, __system_property_get("other_property", propvalue));
    ASSERT_STREQ(propvalue, "value2");

    ASSERT_EQ(6, __system_property_get("property_other", propvalue));
    ASSERT_STREQ(propvalue, "value3");

    ASSERT_EQ(5, __system_property_get(name, propvalue));
    ASSERT_STREQ(propvalue, "value");
#else // __BIONIC__
    GTEST_LOG_(INFO) << "This test does nothing.\n";
#endif // __BIONIC__
}

TEST(properties, __system_property_update) {
#if defined(__BIONIC__)
    LocalPropertyTestState pa;
    ASSERT_TRUE(pa.valid);

    ASSERT_EQ(0, __system_property_add("property", 8, "oldvalue1", 9));
    ASSERT_EQ(0, __system_property_add("other_property", 14, "value2", 6));
    ASSERT_EQ(0, __system_property_add("property_other", 14, "value3", 6));

    const prop_info* pi = __system_property_find("property");
    ASSERT_TRUE(pi != nullptr);
    __system_property_update(const_cast<prop_info*>(pi), "value4", 6);

    pi = __system_property_find("other_property");
    ASSERT_TRUE(pi != nullptr);
    __system_property_update(const_cast<prop_info*>(pi), "newvalue5", 9);

    pi = __system_property_find("property_other");
    ASSERT_TRUE(pi != nullptr);
    __system_property_update(const_cast<prop_info*>(pi), "value6", 6);

    char propvalue[PROP_VALUE_MAX];
    ASSERT_EQ(6, __system_property_get("property", propvalue));
    ASSERT_STREQ(propvalue, "value4");

    ASSERT_EQ(9, __system_property_get("other_property", propvalue));
    ASSERT_STREQ(propvalue, "newvalue5");

    ASSERT_EQ(6, __system_property_get("property_other", propvalue));
    ASSERT_STREQ(propvalue, "value6");
#else // __BIONIC__
    GTEST_LOG_(INFO) << "This test does nothing.\n";
#endif // __BIONIC__
}

TEST(properties, fill) {
#if defined(__BIONIC__)
    LocalPropertyTestState pa;
    ASSERT_TRUE(pa.valid);
    char prop_name[PROP_NAME_MAX];
    char prop_value[PROP_VALUE_MAX];
    char prop_value_ret[PROP_VALUE_MAX];
    int count = 0;
    int ret;

    while (true) {
        ret = snprintf(prop_name, PROP_NAME_MAX - 1, "property_%d", count);
        memset(prop_name + ret, 'a', PROP_NAME_MAX - 1 - ret);
        ret = snprintf(prop_value, PROP_VALUE_MAX - 1, "value_%d", count);
        memset(prop_value + ret, 'b', PROP_VALUE_MAX - 1 - ret);
        prop_name[PROP_NAME_MAX - 1] = 0;
        prop_value[PROP_VALUE_MAX - 1] = 0;

        ret = __system_property_add(prop_name, PROP_NAME_MAX - 1, prop_value, PROP_VALUE_MAX - 1);
        if (ret < 0)
            break;

        count++;
    }

    // For historical reasons at least 247 properties must be supported
    ASSERT_GE(count, 247);

    for (int i = 0; i < count; i++) {
        ret = snprintf(prop_name, PROP_NAME_MAX - 1, "property_%d", i);
        memset(prop_name + ret, 'a', PROP_NAME_MAX - 1 - ret);
        ret = snprintf(prop_value, PROP_VALUE_MAX - 1, "value_%d", i);
        memset(prop_value + ret, 'b', PROP_VALUE_MAX - 1 - ret);
        prop_name[PROP_NAME_MAX - 1] = 0;
        prop_value[PROP_VALUE_MAX - 1] = 0;
        memset(prop_value_ret, '\0', PROP_VALUE_MAX);

        ASSERT_EQ(PROP_VALUE_MAX - 1, __system_property_get(prop_name, prop_value_ret));
        ASSERT_EQ(0, memcmp(prop_value, prop_value_ret, PROP_VALUE_MAX));
    }
#else // __BIONIC__
    GTEST_LOG_(INFO) << "This test does nothing.\n";
#endif // __BIONIC__
}

TEST(properties, __system_property_foreach) {
#if defined(__BIONIC__)
    LocalPropertyTestState pa;
    ASSERT_TRUE(pa.valid);

    ASSERT_EQ(0, __system_property_add("property", 8, "value1", 6));
    ASSERT_EQ(0, __system_property_add("other_property", 14, "value2", 6));
    ASSERT_EQ(0, __system_property_add("property_other", 14, "value3", 6));

    size_t count = 0;
    ASSERT_EQ(0, __system_property_foreach(foreach_test_callback, &count));
    ASSERT_EQ(3U, count);
#else // __BIONIC__
    GTEST_LOG_(INFO) << "This test does nothing.\n";
#endif // __BIONIC__
}

TEST(properties, __system_property_find_nth) {
#if defined(__BIONIC__)
    LocalPropertyTestState pa;
    ASSERT_TRUE(pa.valid);

    ASSERT_EQ(0, __system_property_add("property", 8, "value1", 6));
    ASSERT_EQ(0, __system_property_add("other_property", 14, "value2", 6));
    ASSERT_EQ(0, __system_property_add("property_other", 14, "value3", 6));

    // This method is no longer supported and should result in abort
    ASSERT_EXIT(__system_property_find_nth(0), testing::KilledBySignal(SIGABRT),
                "__system_property_find_nth is not supported since Android O,"
                " please use __system_property_foreach instead.");
#else // __BIONIC__
    GTEST_LOG_(INFO) << "This test does nothing.\n";
#endif // __BIONIC__
}

TEST(properties, fill_hierarchical) {
#if defined(__BIONIC__)
    LocalPropertyTestState pa;
    ASSERT_TRUE(pa.valid);
    char prop_name[PROP_NAME_MAX];
    char prop_value[PROP_VALUE_MAX];
    char prop_value_ret[PROP_VALUE_MAX];
    int ret;

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            for (int k = 0; k < 8; k++) {
                ret = snprintf(prop_name, PROP_NAME_MAX - 1, "property_%d.%d.%d", i, j, k);
                memset(prop_name + ret, 'a', PROP_NAME_MAX - 1 - ret);
                ret = snprintf(prop_value, PROP_VALUE_MAX - 1, "value_%d.%d.%d", i, j, k);
                memset(prop_value + ret, 'b', PROP_VALUE_MAX - 1 - ret);
                prop_name[PROP_NAME_MAX - 1] = 0;
                prop_value[PROP_VALUE_MAX - 1] = 0;

                ASSERT_EQ(0, __system_property_add(prop_name, PROP_NAME_MAX - 1, prop_value, PROP_VALUE_MAX - 1));
            }
        }
    }

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            for (int k = 0; k < 8; k++) {
                ret = snprintf(prop_name, PROP_NAME_MAX - 1, "property_%d.%d.%d", i, j, k);
                memset(prop_name + ret, 'a', PROP_NAME_MAX - 1 - ret);
                ret = snprintf(prop_value, PROP_VALUE_MAX - 1, "value_%d.%d.%d", i, j, k);
                memset(prop_value + ret, 'b', PROP_VALUE_MAX - 1 - ret);
                prop_name[PROP_NAME_MAX - 1] = 0;
                prop_value[PROP_VALUE_MAX - 1] = 0;
                memset(prop_value_ret, '\0', PROP_VALUE_MAX);

                ASSERT_EQ(PROP_VALUE_MAX - 1, __system_property_get(prop_name, prop_value_ret));
                ASSERT_EQ(0, memcmp(prop_value, prop_value_ret, PROP_VALUE_MAX));
            }
        }
    }

    bool ok[8][8][8];
    memset(ok, 0, sizeof(ok));
    __system_property_foreach(hierarchical_test_callback, ok);

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            for (int k = 0; k < 8; k++) {
                ASSERT_TRUE(ok[i][j][k]);
            }
        }
    }
#else // __BIONIC__
    GTEST_LOG_(INFO) << "This test does nothing.\n";
#endif // __BIONIC__
}

TEST(properties, errors) {
#if defined(__BIONIC__)
    LocalPropertyTestState pa;
    ASSERT_TRUE(pa.valid);
    char prop_value[PROP_NAME_MAX];

    ASSERT_EQ(0, __system_property_add("property", 8, "value1", 6));
    ASSERT_EQ(0, __system_property_add("other_property", 14, "value2", 6));
    ASSERT_EQ(0, __system_property_add("property_other", 14, "value3", 6));

    ASSERT_EQ(0, __system_property_find("property1"));
    ASSERT_EQ(0, __system_property_get("property1", prop_value));

    ASSERT_EQ(-1, __system_property_add("name", 4, "value", PROP_VALUE_MAX));
    ASSERT_EQ(-1, __system_property_update(NULL, "value", PROP_VALUE_MAX));
#else // __BIONIC__
    GTEST_LOG_(INFO) << "This test does nothing.\n";
#endif // __BIONIC__
}

TEST(properties, __system_property_serial) {
#if defined(__BIONIC__)
    LocalPropertyTestState pa;
    ASSERT_TRUE(pa.valid);

    ASSERT_EQ(0, __system_property_add("property", 8, "value1", 6));
    const prop_info* pi = __system_property_find("property");
    ASSERT_TRUE(pi != nullptr);
    unsigned serial = __system_property_serial(pi);
    ASSERT_EQ(0, __system_property_update(const_cast<prop_info*>(pi), "value2", 6));
    ASSERT_NE(serial, __system_property_serial(pi));
#else // __BIONIC__
    GTEST_LOG_(INFO) << "This test does nothing.\n";
#endif // __BIONIC__
}

TEST(properties, __system_property_wait_any) {
#if defined(__BIONIC__)
    LocalPropertyTestState pa;
    ASSERT_TRUE(pa.valid);

    ASSERT_EQ(0, __system_property_add("property", 8, "value1", 6));
    unsigned serial = __system_property_wait_any(0);

    prop_info* pi = const_cast<prop_info*>(__system_property_find("property"));
    ASSERT_TRUE(pi != nullptr);
    __system_property_update(pi, "value2", 6);
    serial = __system_property_wait_any(serial);

    int flag = 0;
    pthread_t t;
    ASSERT_EQ(0, pthread_create(&t, nullptr, PropertyWaitHelperFn, &flag));
    ASSERT_EQ(flag, 0);
    serial = __system_property_wait_any(serial);
    ASSERT_EQ(flag, 1);

    ASSERT_EQ(0, pthread_join(t, nullptr));
#else // __BIONIC__
    GTEST_LOG_(INFO) << "This test does nothing.\n";
#endif // __BIONIC__
}

TEST(properties, __system_property_wait) {
#if defined(__BIONIC__)
    LocalPropertyTestState pa;
    ASSERT_TRUE(pa.valid);

    ASSERT_EQ(0, __system_property_add("property", 8, "value1", 6));

    prop_info* pi = const_cast<prop_info*>(__system_property_find("property"));
    ASSERT_TRUE(pi != nullptr);

    unsigned serial = __system_property_serial(pi);

    std::thread thread([]() {
        prop_info* pi = const_cast<prop_info*>(__system_property_find("property"));
        ASSERT_TRUE(pi != nullptr);

        __system_property_update(pi, "value2", 6);
    });

    uint32_t new_serial;
    __system_property_wait(pi, serial, &new_serial, nullptr);
    ASSERT_GT(new_serial, serial);

    char value[PROP_VALUE_MAX];
    ASSERT_EQ(6, __system_property_get("property", value));
    ASSERT_STREQ("value2", value);

    thread.join();
#else // __BIONIC__
    GTEST_LOG_(INFO) << "This test does nothing.\n";
#endif // __BIONIC__
}

class KilledByFault {
    public:
        explicit KilledByFault() {};
        bool operator()(int exit_status) const;
};

bool KilledByFault::operator()(int exit_status) const {
    return WIFSIGNALED(exit_status) &&
        (WTERMSIG(exit_status) == SIGSEGV ||
         WTERMSIG(exit_status) == SIGBUS ||
         WTERMSIG(exit_status) == SIGABRT);
}

class properties_DeathTest : public BionicDeathTest {};

TEST_F(properties_DeathTest, read_only) {
#if defined(__BIONIC__)

  // This test only makes sense if we're talking to the real system property service.
  struct stat sb;
  ASSERT_FALSE(stat(PROP_FILENAME, &sb) == -1 && errno == ENOENT);

  ASSERT_EXIT(__system_property_add("property", 8, "value", 5), KilledByFault(), "");
#else // __BIONIC__
  GTEST_LOG_(INFO) << "This test does nothing.\n";
#endif // __BIONIC__
}
