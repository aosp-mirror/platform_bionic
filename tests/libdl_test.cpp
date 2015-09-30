/*
 * Copyright (C) 2015 The Android Open Source Project
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

#include <android/api-level.h>

extern "C" uint32_t android_get_application_target_sdk_version();
extern "C" void android_set_application_target_sdk_version(uint32_t target);

TEST(libdl, application_sdk_versions_smoke) {
  // Check initial values
  ASSERT_EQ(static_cast<uint32_t>(__ANDROID_API__), android_get_application_target_sdk_version());

  android_set_application_target_sdk_version(20U);
  ASSERT_EQ(20U, android_get_application_target_sdk_version());

  android_set_application_target_sdk_version(22U);
  ASSERT_EQ(22U, android_get_application_target_sdk_version());
}

