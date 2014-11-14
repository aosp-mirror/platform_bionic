/*
 * Copyright (C) 2014 The Android Open Source Project
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

#include <sys/sysinfo.h>

TEST(sys_sysinfo, smoke) {
  int nprocessor = get_nprocs();
  ASSERT_GT(nprocessor, 0);

  int nprocessor_conf = get_nprocs_conf();
  ASSERT_GE(nprocessor_conf, nprocessor);

  long avail_phys_pages = get_avphys_pages();
  ASSERT_GT(avail_phys_pages, 0);

  long phys_pages = get_phys_pages();
  ASSERT_GE(phys_pages, avail_phys_pages);
}
