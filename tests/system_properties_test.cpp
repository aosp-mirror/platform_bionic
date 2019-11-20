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

#include <android-base/file.h>

using namespace std::literals;

#if defined(__BIONIC__)

#define _REALLY_INCLUDE_SYS__SYSTEM_PROPERTIES_H_
#include <sys/_system_properties.h>

#include <system_properties/system_properties.h>

class SystemPropertiesTest : public SystemProperties {
 public:
  SystemPropertiesTest() : SystemProperties(false) {
    valid_ = AreaInit(dir_.path, nullptr);
  }
  ~SystemPropertiesTest() {
    if (valid_) {
      contexts_->FreeAndUnmap();
    }
  }

  bool valid() const {
    return valid_;
  }

 private:
  TemporaryDir dir_;
  bool valid_;
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

#endif // __BIONIC__

TEST(properties, __system_property_add) {
#if defined(__BIONIC__)
    SystemPropertiesTest system_properties;
    ASSERT_TRUE(system_properties.valid());

    ASSERT_EQ(0, system_properties.Add("property", 8, "value1", 6));
    ASSERT_EQ(0, system_properties.Add("other_property", 14, "value2", 6));
    ASSERT_EQ(0, system_properties.Add("property_other", 14, "value3", 6));

    // check that there is no limit on property name length
    char name[PROP_NAME_MAX + 11];
    name[0] = 'p';
    for (size_t i = 1; i < sizeof(name); i++) {
      name[i] = 'x';
    }

    name[sizeof(name)-1] = '\0';
    ASSERT_EQ(0, system_properties.Add(name, strlen(name), "value", 5));

    char propvalue[PROP_VALUE_MAX];
    ASSERT_EQ(6, system_properties.Get("property", propvalue));
    ASSERT_STREQ(propvalue, "value1");

    ASSERT_EQ(6, system_properties.Get("other_property", propvalue));
    ASSERT_STREQ(propvalue, "value2");

    ASSERT_EQ(6, system_properties.Get("property_other", propvalue));
    ASSERT_STREQ(propvalue, "value3");

    ASSERT_EQ(5, system_properties.Get(name, propvalue));
    ASSERT_STREQ(propvalue, "value");
#else // __BIONIC__
    GTEST_SKIP() << "bionic-only test";
#endif // __BIONIC__
}

TEST(properties, __system_property_update) {
#if defined(__BIONIC__)
    SystemPropertiesTest system_properties;
    ASSERT_TRUE(system_properties.valid());

    ASSERT_EQ(0, system_properties.Add("property", 8, "oldvalue1", 9));
    ASSERT_EQ(0, system_properties.Add("other_property", 14, "value2", 6));
    ASSERT_EQ(0, system_properties.Add("property_other", 14, "value3", 6));

    const prop_info* pi = system_properties.Find("property");
    ASSERT_TRUE(pi != nullptr);
    system_properties.Update(const_cast<prop_info*>(pi), "value4", 6);

    pi = system_properties.Find("other_property");
    ASSERT_TRUE(pi != nullptr);
    system_properties.Update(const_cast<prop_info*>(pi), "newvalue5", 9);

    pi = system_properties.Find("property_other");
    ASSERT_TRUE(pi != nullptr);
    system_properties.Update(const_cast<prop_info*>(pi), "value6", 6);

    char propvalue[PROP_VALUE_MAX];
    ASSERT_EQ(6, system_properties.Get("property", propvalue));
    ASSERT_STREQ(propvalue, "value4");

    ASSERT_EQ(9, system_properties.Get("other_property", propvalue));
    ASSERT_STREQ(propvalue, "newvalue5");

    ASSERT_EQ(6, system_properties.Get("property_other", propvalue));
    ASSERT_STREQ(propvalue, "value6");
#else // __BIONIC__
    GTEST_SKIP() << "bionic-only test";
#endif // __BIONIC__
}

TEST(properties, fill) {
#if defined(__BIONIC__)
    SystemPropertiesTest system_properties;
    ASSERT_TRUE(system_properties.valid());

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

        ret = system_properties.Add(prop_name, PROP_NAME_MAX - 1, prop_value, PROP_VALUE_MAX - 1);
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

        ASSERT_EQ(PROP_VALUE_MAX - 1, system_properties.Get(prop_name, prop_value_ret));
        ASSERT_EQ(0, memcmp(prop_value, prop_value_ret, PROP_VALUE_MAX));
    }
#else // __BIONIC__
    GTEST_SKIP() << "bionic-only test";
#endif // __BIONIC__
}

TEST(properties, __system_property_foreach) {
#if defined(__BIONIC__)
    SystemPropertiesTest system_properties;
    ASSERT_TRUE(system_properties.valid());

    ASSERT_EQ(0, system_properties.Add("property", 8, "value1", 6));
    ASSERT_EQ(0, system_properties.Add("other_property", 14, "value2", 6));
    ASSERT_EQ(0, system_properties.Add("property_other", 14, "value3", 6));

    size_t count = 0;
    ASSERT_EQ(0, system_properties.Foreach(foreach_test_callback, &count));
    ASSERT_EQ(3U, count);
#else // __BIONIC__
    GTEST_SKIP() << "bionic-only test";
#endif // __BIONIC__
}

TEST(properties, __system_property_find_nth) {
#if defined(__BIONIC__)
    SystemPropertiesTest system_properties;
    ASSERT_TRUE(system_properties.valid());

    ASSERT_EQ(0, system_properties.Add("property", 8, "value1", 6));
    ASSERT_EQ(0, system_properties.Add("other_property", 14, "value2", 6));
    ASSERT_EQ(0, system_properties.Add("property_other", 14, "value3", 6));

    char name[PROP_NAME_MAX];
    char value[PROP_VALUE_MAX];
    EXPECT_EQ(6, system_properties.Read(system_properties.FindNth(0), name, value));
    EXPECT_STREQ("property", name);
    EXPECT_STREQ("value1", value);
    EXPECT_EQ(6, system_properties.Read(system_properties.FindNth(1), name, value));
    EXPECT_STREQ("other_property", name);
    EXPECT_STREQ("value2", value);
    EXPECT_EQ(6, system_properties.Read(system_properties.FindNth(2), name, value));
    EXPECT_STREQ("property_other", name);
    EXPECT_STREQ("value3", value);

    for (unsigned i = 3; i < 1024; ++i) {
      ASSERT_TRUE(system_properties.FindNth(i) == nullptr);
    }
#else // __BIONIC__
    GTEST_SKIP() << "bionic-only test";
#endif // __BIONIC__
}

TEST(properties, fill_hierarchical) {
#if defined(__BIONIC__)
    SystemPropertiesTest system_properties;
    ASSERT_TRUE(system_properties.valid());

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

                ASSERT_EQ(0, system_properties.Add(
                    prop_name, PROP_NAME_MAX - 1, prop_value, PROP_VALUE_MAX - 1));
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

                ASSERT_EQ(PROP_VALUE_MAX - 1, system_properties.Get(prop_name, prop_value_ret));
                ASSERT_EQ(0, memcmp(prop_value, prop_value_ret, PROP_VALUE_MAX));
            }
        }
    }

    bool ok[8][8][8];
    memset(ok, 0, sizeof(ok));
    system_properties.Foreach(hierarchical_test_callback, ok);

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            for (int k = 0; k < 8; k++) {
                ASSERT_TRUE(ok[i][j][k]);
            }
        }
    }
#else // __BIONIC__
    GTEST_SKIP() << "bionic-only test";
#endif // __BIONIC__
}

TEST(properties, errors) {
#if defined(__BIONIC__)
    SystemPropertiesTest system_properties;
    ASSERT_TRUE(system_properties.valid());

    char prop_value[PROP_NAME_MAX];

    ASSERT_EQ(0, system_properties.Add("property", 8, "value1", 6));
    ASSERT_EQ(0, system_properties.Add("other_property", 14, "value2", 6));
    ASSERT_EQ(0, system_properties.Add("property_other", 14, "value3", 6));

    ASSERT_EQ(0, system_properties.Find("property1"));
    ASSERT_EQ(0, system_properties.Get("property1", prop_value));

    ASSERT_EQ(-1, system_properties.Add("name", 4, "value", PROP_VALUE_MAX));
    ASSERT_EQ(-1, system_properties.Update(NULL, "value", PROP_VALUE_MAX));
#else // __BIONIC__
    GTEST_SKIP() << "bionic-only test";
#endif // __BIONIC__
}

TEST(properties, __system_property_serial) {
#if defined(__BIONIC__)
    SystemPropertiesTest system_properties;
    ASSERT_TRUE(system_properties.valid());

    ASSERT_EQ(0, system_properties.Add("property", 8, "value1", 6));
    const prop_info* pi = system_properties.Find("property");
    ASSERT_TRUE(pi != nullptr);
    unsigned serial = __system_property_serial(pi);
    ASSERT_EQ(0, system_properties.Update(const_cast<prop_info*>(pi), "value2", 6));
    ASSERT_NE(serial, __system_property_serial(pi));
#else // __BIONIC__
    GTEST_SKIP() << "bionic-only test";
#endif // __BIONIC__
}

TEST(properties, __system_property_wait_any) {
#if defined(__BIONIC__)
    SystemPropertiesTest system_properties;
    ASSERT_TRUE(system_properties.valid());

    ASSERT_EQ(0, system_properties.Add("property", 8, "value1", 6));
    unsigned serial = system_properties.WaitAny(0);

    prop_info* pi = const_cast<prop_info*>(system_properties.Find("property"));
    ASSERT_TRUE(pi != nullptr);
    system_properties.Update(pi, "value2", 6);
    serial = system_properties.WaitAny(serial);

    int flag = 0;
    std::thread thread([&system_properties, &flag]() {
        prop_info* pi = const_cast<prop_info*>(system_properties.Find("property"));
        usleep(100000);

        flag = 1;
        system_properties.Update(pi, "value3", 6);
    });
    ASSERT_EQ(flag, 0);
    serial = system_properties.WaitAny(serial);
    ASSERT_EQ(flag, 1);

    thread.join();
#else // __BIONIC__
    GTEST_SKIP() << "bionic-only test";
#endif // __BIONIC__
}

TEST(properties, __system_property_wait) {
#if defined(__BIONIC__)
    SystemPropertiesTest system_properties;
    ASSERT_TRUE(system_properties.valid());

    ASSERT_EQ(0, system_properties.Add("property", 8, "value1", 6));

    prop_info* pi = const_cast<prop_info*>(system_properties.Find("property"));
    ASSERT_TRUE(pi != nullptr);

    unsigned serial = __system_property_serial(pi);

    std::thread thread([&system_properties]() {
        prop_info* pi = const_cast<prop_info*>(system_properties.Find("property"));
        ASSERT_TRUE(pi != nullptr);

        system_properties.Update(pi, "value2", 6);
    });

    uint32_t new_serial;
    system_properties.Wait(pi, serial, &new_serial, nullptr);
    ASSERT_GT(new_serial, serial);

    char value[PROP_VALUE_MAX];
    ASSERT_EQ(6, system_properties.Get("property", value));
    ASSERT_STREQ("value2", value);

    thread.join();
#else // __BIONIC__
    GTEST_SKIP() << "bionic-only test";
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
  GTEST_SKIP() << "bionic-only test";
#endif // __BIONIC__
}

TEST(properties, __system_property_extra_long_read_only) {
#if defined(__BIONIC__)
  SystemPropertiesTest system_properties;
  ASSERT_TRUE(system_properties.valid());

  std::vector<std::pair<std::string, std::string>> short_properties = {
    { "ro.0char", std::string() },
    { "ro.50char", std::string(50, 'x') },
    { "ro.91char", std::string(91, 'x') },
  };

  std::vector<std::pair<std::string, std::string>> long_properties = {
    { "ro.92char", std::string(92, 'x') },
    { "ro.93char", std::string(93, 'x') },
    { "ro.1000char", std::string(1000, 'x') },
  };

  for (const auto& property : short_properties) {
    const std::string& name = property.first;
    const std::string& value = property.second;
    ASSERT_EQ(0, system_properties.Add(name.c_str(), name.size(), value.c_str(), value.size()));
  }

  for (const auto& property : long_properties) {
    const std::string& name = property.first;
    const std::string& value = property.second;
    ASSERT_EQ(0, system_properties.Add(name.c_str(), name.size(), value.c_str(), value.size()));
  }

  auto check_with_legacy_read = [&system_properties](const std::string& name,
                                                     const std::string& expected_value) {
    char value[PROP_VALUE_MAX];
    EXPECT_EQ(static_cast<int>(expected_value.size()), system_properties.Get(name.c_str(), value))
        << name;
    EXPECT_EQ(expected_value, value) << name;
  };

  auto check_with_read_callback = [&system_properties](const std::string& name,
                                                       const std::string& expected_value) {
    const prop_info* pi = system_properties.Find(name.c_str());
    ASSERT_NE(nullptr, pi);
    std::string value;
    system_properties.ReadCallback(pi,
                                   [](void* cookie, const char*, const char* value, uint32_t) {
                                     auto* out_value = reinterpret_cast<std::string*>(cookie);
                                     *out_value = value;
                                   },
                                   &value);
    EXPECT_EQ(expected_value, value) << name;
  };

  for (const auto& property : short_properties) {
    const std::string& name = property.first;
    const std::string& value = property.second;
    check_with_legacy_read(name, value);
    check_with_read_callback(name, value);
  }

  constexpr static const char* kExtraLongLegacyError =
      "Must use __system_property_read_callback() to read";
  for (const auto& property : long_properties) {
    const std::string& name = property.first;
    const std::string& value = property.second;
    check_with_legacy_read(name, kExtraLongLegacyError);
    check_with_read_callback(name, value);
  }

#else   // __BIONIC__
  GTEST_SKIP() << "bionic-only test";
#endif  // __BIONIC__
}

// pa_size is 128 * 1024 currently, if a property is longer then we expect it to fail gracefully.
TEST(properties, __system_property_extra_long_read_only_too_long) {
#if defined(__BIONIC__)
  SystemPropertiesTest system_properties;
  ASSERT_TRUE(system_properties.valid());

  auto name = "ro.super_long_property"s;
  auto value = std::string(128 * 1024 + 1, 'x');
  ASSERT_NE(0, system_properties.Add(name.c_str(), name.size(), value.c_str(), value.size()));

#else   // __BIONIC__
  GTEST_SKIP() << "bionic-only test";
#endif  // __BIONIC__
}
