/*
 * Copyright (C) 2017 The Android Open Source Project
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
#include <sys/wait.h>
#include <unistd.h>

#include <chrono>
#include <sstream>
#include <string>

#include <gtest/gtest.h>

#include "utils.h"

#if defined(__BIONIC__)
#include <sys/system_properties.h>
int64_t NanoTime() {
  auto t = std::chrono::time_point_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now());
  return t.time_since_epoch().count();
}
#endif

// Note that this test affects global state of the system
// this tests tries to mitigate this by using utime+pid
// prefix for the property name. It is still results in
// pollution of property service since properties cannot
// be removed.
//
// Note that there is also possibility to run into "out-of-memory"
// if this test if it is executed often enough without reboot.
TEST(properties, smoke) {
#if defined(__BIONIC__)
    char propvalue[PROP_VALUE_MAX];

    std::stringstream ss;
    ss << "debug.test." << getpid() << "." << NanoTime() << ".";
    const std::string property_prefix = ss.str();
    const std::string property_name = property_prefix + "property1";

    // Set brand new property
    ASSERT_EQ(0, __system_property_set(property_name.c_str(), "value1"));
    ASSERT_EQ(6, __system_property_get(property_name.c_str(), propvalue));
    ASSERT_STREQ("value1", propvalue);

    std::string long_value = "property-";
    for (size_t i = 0; i < PROP_VALUE_MAX; i++) {
      long_value += "y";
    }

    // Make sure that attempts to set invalid property value fails and preserves
    // previous value.
    propvalue[0] = '\0';
    ASSERT_EQ(-1, __system_property_set(property_name.c_str(), long_value.c_str()));
    ASSERT_EQ(6, __system_property_get(property_name.c_str(), propvalue));
    ASSERT_STREQ("value1", propvalue);

    // Update property
    ASSERT_EQ(0, __system_property_set(property_name.c_str(), "value1-1"));
    ASSERT_EQ(8, __system_property_get(property_name.c_str(), propvalue));
    ASSERT_STREQ("value1-1", propvalue);


    // check that there is no limit on property name length
    char suffix[1024];
    for (size_t i = 0; i < sizeof(suffix); i++) {
      suffix[i] = 'x';
    }

    suffix[sizeof(suffix)-1] = '\0';
    const std::string long_property_name = property_prefix + suffix;

    ASSERT_EQ(0, __system_property_set(long_property_name.c_str(), "value2"));
    ASSERT_EQ(6, __system_property_get(long_property_name.c_str(), propvalue));
    ASSERT_STREQ("value2", propvalue);

    // test find and read_callback
    const prop_info* pi = __system_property_find(property_name.c_str());
    ASSERT_TRUE(pi != nullptr);

    std::string expected_name = property_name;
    __system_property_read_callback(pi,
      [](void* cookie, const char* name, const char* value, unsigned /*serial*/) {
        const std::string* expected_name = static_cast<const std::string*>(cookie);
        ASSERT_EQ(*expected_name, name);
        ASSERT_STREQ("value1-1", value);
    }, &expected_name);

    pi = __system_property_find(long_property_name.c_str());
    ASSERT_TRUE(pi != nullptr);

    expected_name = long_property_name;
    __system_property_read_callback(pi,
      [](void* cookie, const char* name, const char* value, unsigned /*serial*/) {
        const std::string* expected_name = static_cast<const std::string*>(cookie);
        ASSERT_EQ(*expected_name, name);
        ASSERT_STREQ("value2", value);
    }, &expected_name);

    // Check that read() for long names still works but returns truncated version of the name
    pi = __system_property_find(property_name.c_str());
    ASSERT_TRUE(pi != nullptr);
    char legacy_name[PROP_NAME_MAX];
    expected_name = std::string(property_name.c_str(), PROP_NAME_MAX-1);
    ASSERT_EQ(8, __system_property_read(pi, &legacy_name[0], propvalue));
    ASSERT_EQ(expected_name, legacy_name);
    ASSERT_STREQ("value1-1", propvalue);

    const prop_info* pi_long = __system_property_find(long_property_name.c_str());
    ASSERT_TRUE(pi != nullptr);
    expected_name = std::string(long_property_name.c_str(), PROP_NAME_MAX-1);
    ASSERT_EQ(6, __system_property_read(pi_long, &legacy_name[0], propvalue));
    ASSERT_EQ(expected_name, legacy_name);
    ASSERT_STREQ("value2", propvalue);
#else // __BIONIC__
    GTEST_SKIP() << "bionic-only test";
#endif // __BIONIC__
}

TEST(properties, no_fd_leaks) {
#if defined(__BIONIC__)
  FdLeakChecker leak_checker;
  std::stringstream ss;
  ss << "debug.test." << getpid() << "." << NanoTime() << ".";
  const std::string property_prefix = ss.str();
  const std::string property_name = property_prefix + "property1";

  for (size_t i = 0; i < 100; ++i) {
    char propvalue[PROP_VALUE_MAX];
    ASSERT_EQ(0, __system_property_set(property_name.c_str(), "value1"));
    ASSERT_EQ(6, __system_property_get(property_name.c_str(), propvalue));
    ASSERT_STREQ("value1", propvalue);

    ASSERT_EQ(0, __system_property_set(property_name.c_str(), "value2"));
    ASSERT_EQ(6, __system_property_get(property_name.c_str(), propvalue));
    ASSERT_STREQ("value2", propvalue);
  }
#else   // __BIONIC__
  GTEST_SKIP() << "bionic-only test";
#endif  // __BIONIC__
}

TEST(properties, empty_value) {
#if defined(__BIONIC__)
    char propvalue[PROP_VALUE_MAX];

    std::stringstream ss;
    ss << "debug.test." << getpid() << "." << NanoTime() << "." << "property_empty";
    const std::string property_name = ss.str();

    for (size_t i = 0; i < 1000; ++i) {
      ASSERT_EQ(0, __system_property_set(property_name.c_str(), ""));
      ASSERT_EQ(0, __system_property_get(property_name.c_str(), propvalue));
      ASSERT_STREQ("", propvalue);
    }

#else  // __BIONIC__
  GTEST_SKIP() << "bionic-only test";
#endif // __BIONIC__
}
