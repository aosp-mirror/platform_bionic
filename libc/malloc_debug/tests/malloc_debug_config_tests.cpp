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

#include <limits.h>

#include <memory>
#include <string>

#include <gtest/gtest.h>

#include "Config.h"

#include "log_fake.h"

extern "C" int property_set(const char*, const char*);

class MallocDebugConfigTest : public ::testing::Test {
 protected:
  void SetUp() override {
    resetLogs();
  }

  void TearDown() override {
  }

  std::unique_ptr<Config> config;

  bool InitConfig(const char* property_value) {
    config.reset(new Config);
    property_set("libc.debug.malloc.options", property_value);
    return config->SetFromProperties();
  }
};

std::string usage_string(
  "6 malloc_debug malloc debug options usage:\n"
  "6 malloc_debug \n"
  "6 malloc_debug   front_guard[=XX]\n"
  "6 malloc_debug     Enables a front guard on all allocations. If XX is set\n"
  "6 malloc_debug     it sets the number of bytes in the guard. The default is\n"
  "6 malloc_debug     32 bytes, the max bytes is 16384.\n"
  "6 malloc_debug \n"
  "6 malloc_debug   rear_guard[=XX]\n"
  "6 malloc_debug     Enables a rear guard on all allocations. If XX is set\n"
  "6 malloc_debug     it sets the number of bytes in the guard. The default is\n"
  "6 malloc_debug     32 bytes, the max bytes is 16384.\n"
  "6 malloc_debug \n"
  "6 malloc_debug   guard[=XX]\n"
  "6 malloc_debug     Enables both a front guard and a rear guard on all allocations.\n"
  "6 malloc_debug     If XX is set it sets the number of bytes in both guards.\n"
  "6 malloc_debug     The default is 32 bytes, the max bytes is 16384.\n"
  "6 malloc_debug \n"
  "6 malloc_debug   backtrace[=XX]\n"
  "6 malloc_debug     Enable capturing the backtrace at the point of allocation.\n"
  "6 malloc_debug     If XX is set it sets the number of backtrace frames.\n"
  "6 malloc_debug     The default is 16 frames, the max number of frames is 256.\n"
  "6 malloc_debug \n"
  "6 malloc_debug   backtrace_enable_on_signal[=XX]\n"
  "6 malloc_debug     Enable capturing the backtrace at the point of allocation.\n"
  "6 malloc_debug     The backtrace capture is not enabled until the process\n"
  "6 malloc_debug     receives a signal. If XX is set it sets the number of backtrace\n"
  "6 malloc_debug     frames. The default is 16 frames, the max number of frames is 256.\n"
  "6 malloc_debug \n"
  "6 malloc_debug   fill_on_alloc[=XX]\n"
  "6 malloc_debug     On first allocation, fill with the value 0xeb.\n"
  "6 malloc_debug     If XX is set it will only fill up to XX bytes of the\n"
  "6 malloc_debug     allocation. The default is to fill the entire allocation.\n"
  "6 malloc_debug \n"
  "6 malloc_debug   fill_on_free[=XX]\n"
  "6 malloc_debug     On free, fill with the value 0xef. If XX is set it will\n"
  "6 malloc_debug     only fill up to XX bytes of the allocation. The default is to\n"
  "6 malloc_debug     fill the entire allocation.\n"
  "6 malloc_debug \n"
  "6 malloc_debug   fill[=XX]\n"
  "6 malloc_debug     On both first allocation free, fill with the value 0xeb on\n"
  "6 malloc_debug     first allocation and the value 0xef. If XX is set, only fill\n"
  "6 malloc_debug     up to XX bytes. The default is to fill the entire allocation.\n"
  "6 malloc_debug \n"
  "6 malloc_debug   expand_alloc[=XX]\n"
  "6 malloc_debug     Allocate an extra number of bytes for every allocation call.\n"
  "6 malloc_debug     If XX is set, that is the number of bytes to expand the\n"
  "6 malloc_debug     allocation by. The default is 16 bytes, the max bytes is 16384.\n"
  "6 malloc_debug \n"
  "6 malloc_debug   free_track[=XX]\n"
  "6 malloc_debug     When a pointer is freed, do not free the memory right away.\n"
  "6 malloc_debug     Instead, keep XX of these allocations around and then verify\n"
  "6 malloc_debug     that they have not been modified when the total number of freed\n"
  "6 malloc_debug     allocations exceeds the XX amount. When the program terminates,\n"
  "6 malloc_debug     the rest of these allocations are verified. When this option is\n"
  "6 malloc_debug     enabled, it automatically records the backtrace at the time of the free.\n"
  "6 malloc_debug     The default is to record 100 allocations, the max allocations\n"
  "6 malloc_debug     to record is 16384.\n"
  "6 malloc_debug \n"
  "6 malloc_debug   free_track_backtrace_num_frames[=XX]\n"
  "6 malloc_debug     This option only has meaning if free_track is set. This indicates\n"
  "6 malloc_debug     how many backtrace frames to capture when an allocation is freed.\n"
  "6 malloc_debug     If XX is set, that is the number of frames to capture. If XX\n"
  "6 malloc_debug     is set to zero, then no backtrace will be captured.\n"
  "6 malloc_debug     The default is to record 16 frames, the max number of frames is 256.\n"
  "6 malloc_debug \n"
  "6 malloc_debug   leak_track\n"
  "6 malloc_debug     Enable the leak tracking of memory allocations.\n"
);

TEST_F(MallocDebugConfigTest, unknown_option) {

  ASSERT_FALSE(InitConfig("unknown_option"));

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  std::string log_msg("6 malloc_debug malloc_testing: unknown option unknown_option\n");
  ASSERT_STREQ((log_msg + usage_string).c_str(), getFakeLogPrint().c_str());
}

TEST_F(MallocDebugConfigTest, good_option_and_bad_option) {
  ASSERT_FALSE(InitConfig("backtrace unknown_option"));

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  std::string log_msg("6 malloc_debug malloc_testing: unknown option unknown_option\n");
  ASSERT_STREQ((log_msg + usage_string).c_str(), getFakeLogPrint().c_str());
}

TEST_F(MallocDebugConfigTest, unparseable_number) {
  ASSERT_FALSE(InitConfig("backtrace=XXX"));

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  std::string log_msg("6 malloc_debug malloc_testing: bad value for option 'backtrace'\n");
  ASSERT_STREQ((log_msg + usage_string).c_str(), getFakeLogPrint().c_str());
}

TEST_F(MallocDebugConfigTest, illegal_value_zero) {
  ASSERT_FALSE(InitConfig("backtrace=0"));

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  std::string log_msg(
      "6 malloc_debug malloc_testing: bad value for option 'backtrace', value must be >= 1: 0\n");
  ASSERT_STREQ((log_msg + usage_string).c_str(), getFakeLogPrint().c_str());
}

TEST_F(MallocDebugConfigTest, no_space) {
  ASSERT_FALSE(InitConfig("backtrace=10front_guard"));

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  std::string log_msg(
      "6 malloc_debug malloc_testing: bad value for option 'backtrace', "
      "non space found after option: front_guard\n");
  ASSERT_STREQ((log_msg + usage_string).c_str(), getFakeLogPrint().c_str());
}

TEST_F(MallocDebugConfigTest, illegal_value_negative) {
  ASSERT_FALSE(InitConfig("backtrace=-1"));

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  std::string log_msg(
      "6 malloc_debug malloc_testing: bad value for option 'backtrace', "
      "value cannot be negative: -1\n");
  ASSERT_STREQ((log_msg + usage_string).c_str(), getFakeLogPrint().c_str());
}

TEST_F(MallocDebugConfigTest, overflow) {
  ASSERT_FALSE(InitConfig("backtrace=99999999999999999999"));

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  std::string log_msg(
      "6 malloc_debug malloc_testing: bad value for option 'backtrace': "
      "Math result not representable\n");
  ASSERT_STREQ((log_msg + usage_string).c_str(), getFakeLogPrint().c_str());
}

TEST_F(MallocDebugConfigTest, set_value_error) {
  ASSERT_FALSE(InitConfig("leak_track=12"));

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  std::string log_msg(
      "6 malloc_debug malloc_testing: value set for option 'leak_track' "
      "which does not take a value\n");
  ASSERT_STREQ((log_msg + usage_string).c_str(), getFakeLogPrint().c_str());
}

TEST_F(MallocDebugConfigTest, space_before_equal) {
  ASSERT_TRUE(InitConfig("backtrace  =10"));
  ASSERT_EQ(BACKTRACE | TRACK_ALLOCS, config->options);
  ASSERT_EQ(10U, config->backtrace_frames);

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  ASSERT_STREQ("", getFakeLogPrint().c_str());
}

TEST_F(MallocDebugConfigTest, space_after_equal) {
  ASSERT_TRUE(InitConfig("backtrace=  10"));
  ASSERT_EQ(BACKTRACE | TRACK_ALLOCS, config->options);
  ASSERT_EQ(10U, config->backtrace_frames);

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  ASSERT_STREQ("", getFakeLogPrint().c_str());
}

TEST_F(MallocDebugConfigTest, extra_space) {
  ASSERT_TRUE(InitConfig("   backtrace=64   "));
  ASSERT_EQ(BACKTRACE | TRACK_ALLOCS, config->options);
  ASSERT_EQ(64U, config->backtrace_frames);

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  ASSERT_STREQ("", getFakeLogPrint().c_str());
}

TEST_F(MallocDebugConfigTest, multiple_options) {
  ASSERT_TRUE(InitConfig("  backtrace=64   front_guard=48"));
  ASSERT_EQ(BACKTRACE | TRACK_ALLOCS | FRONT_GUARD, config->options);
  ASSERT_EQ(64U, config->backtrace_frames);
  ASSERT_EQ(48U, config->front_guard_bytes);

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  ASSERT_STREQ("", getFakeLogPrint().c_str());
}

TEST_F(MallocDebugConfigTest, front_guard) {
  ASSERT_TRUE(InitConfig("front_guard=48"));
  ASSERT_EQ(FRONT_GUARD, config->options);
  ASSERT_EQ(48U, config->front_guard_bytes);

  ASSERT_TRUE(InitConfig("front_guard"));
  ASSERT_EQ(FRONT_GUARD, config->options);
  ASSERT_EQ(32U, config->front_guard_bytes);

  ASSERT_TRUE(InitConfig("front_guard=39"));
  ASSERT_EQ(FRONT_GUARD, config->options);
#if defined(__LP64__)
  ASSERT_EQ(48U, config->front_guard_bytes);
#else
  ASSERT_EQ(40U, config->front_guard_bytes);
#endif

  ASSERT_TRUE(InitConfig("front_guard=41"));
  ASSERT_EQ(FRONT_GUARD, config->options);
  ASSERT_EQ(48U, config->front_guard_bytes);

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  ASSERT_STREQ("", getFakeLogPrint().c_str());
}

TEST_F(MallocDebugConfigTest, rear_guard) {
  ASSERT_TRUE(InitConfig("rear_guard=50"));
  ASSERT_EQ(REAR_GUARD, config->options);
  ASSERT_EQ(50U, config->rear_guard_bytes);

  ASSERT_TRUE(InitConfig("rear_guard"));
  ASSERT_EQ(REAR_GUARD, config->options);
  ASSERT_EQ(32U, config->rear_guard_bytes);

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  ASSERT_STREQ("", getFakeLogPrint().c_str());
}

TEST_F(MallocDebugConfigTest, guard) {
  ASSERT_TRUE(InitConfig("guard=32"));
  ASSERT_EQ(FRONT_GUARD | REAR_GUARD, config->options);
  ASSERT_EQ(32U, config->front_guard_bytes);
  ASSERT_EQ(32U, config->rear_guard_bytes);

  ASSERT_TRUE(InitConfig("guard"));
  ASSERT_EQ(FRONT_GUARD | REAR_GUARD, config->options);
  ASSERT_EQ(32U, config->front_guard_bytes);
  ASSERT_EQ(32U, config->rear_guard_bytes);

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  ASSERT_STREQ("", getFakeLogPrint().c_str());
}

TEST_F(MallocDebugConfigTest, backtrace) {
  ASSERT_TRUE(InitConfig("backtrace=64"));
  ASSERT_EQ(BACKTRACE | TRACK_ALLOCS, config->options);
  ASSERT_EQ(64U, config->backtrace_frames);

  ASSERT_TRUE(InitConfig("backtrace"));
  ASSERT_EQ(BACKTRACE | TRACK_ALLOCS, config->options);
  ASSERT_EQ(16U, config->backtrace_frames);

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  ASSERT_STREQ("", getFakeLogPrint().c_str());
}

TEST_F(MallocDebugConfigTest, backtrace_enable_on_signal) {
  ASSERT_TRUE(InitConfig("backtrace_enable_on_signal=64"));
  ASSERT_EQ(BACKTRACE | TRACK_ALLOCS, config->options);
  ASSERT_EQ(64U, config->backtrace_frames);

  ASSERT_TRUE(InitConfig("backtrace_enable_on_signal"));
  ASSERT_EQ(BACKTRACE | TRACK_ALLOCS, config->options);
  ASSERT_EQ(16U, config->backtrace_frames);

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  ASSERT_STREQ("", getFakeLogPrint().c_str());
}

TEST_F(MallocDebugConfigTest, fill_on_alloc) {
  ASSERT_TRUE(InitConfig("fill_on_alloc=64"));
  ASSERT_EQ(FILL_ON_ALLOC, config->options);
  ASSERT_EQ(64U, config->fill_on_alloc_bytes);

  ASSERT_TRUE(InitConfig("fill_on_alloc"));
  ASSERT_EQ(FILL_ON_ALLOC, config->options);
  ASSERT_EQ(SIZE_MAX, config->fill_on_alloc_bytes);

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  ASSERT_STREQ("", getFakeLogPrint().c_str());
}

TEST_F(MallocDebugConfigTest, fill_on_free) {
  ASSERT_TRUE(InitConfig("fill_on_free=64"));
  ASSERT_EQ(FILL_ON_FREE, config->options);
  ASSERT_EQ(64U, config->fill_on_free_bytes);

  ASSERT_TRUE(InitConfig("fill_on_free"));
  ASSERT_EQ(FILL_ON_FREE, config->options);
  ASSERT_EQ(SIZE_MAX, config->fill_on_free_bytes);

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  ASSERT_STREQ("", getFakeLogPrint().c_str());
}

TEST_F(MallocDebugConfigTest, fill) {
  ASSERT_TRUE(InitConfig("fill=64"));
  ASSERT_EQ(FILL_ON_ALLOC | FILL_ON_FREE, config->options);
  ASSERT_EQ(64U, config->fill_on_alloc_bytes);
  ASSERT_EQ(64U, config->fill_on_free_bytes);

  ASSERT_TRUE(InitConfig("fill"));
  ASSERT_EQ(FILL_ON_ALLOC | FILL_ON_FREE, config->options);
  ASSERT_EQ(SIZE_MAX, config->fill_on_alloc_bytes);
  ASSERT_EQ(SIZE_MAX, config->fill_on_free_bytes);

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  ASSERT_STREQ("", getFakeLogPrint().c_str());
}

TEST_F(MallocDebugConfigTest, expand_alloc) {
  ASSERT_TRUE(InitConfig("expand_alloc=1234"));
  ASSERT_EQ(EXPAND_ALLOC, config->options);
  ASSERT_EQ(1234U, config->expand_alloc_bytes);

  ASSERT_TRUE(InitConfig("expand_alloc"));
  ASSERT_EQ(EXPAND_ALLOC, config->options);
  ASSERT_EQ(16U, config->expand_alloc_bytes);

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  ASSERT_STREQ("", getFakeLogPrint().c_str());
}

TEST_F(MallocDebugConfigTest, free_track) {
  ASSERT_TRUE(InitConfig("free_track=1234"));
  ASSERT_EQ(FREE_TRACK | FILL_ON_FREE, config->options);
  ASSERT_EQ(1234U, config->free_track_allocations);
  ASSERT_EQ(SIZE_MAX, config->fill_on_free_bytes);
  ASSERT_EQ(16U, config->free_track_backtrace_num_frames);

  ASSERT_TRUE(InitConfig("free_track"));
  ASSERT_EQ(FREE_TRACK | FILL_ON_FREE, config->options);
  ASSERT_EQ(100U, config->free_track_allocations);
  ASSERT_EQ(SIZE_MAX, config->fill_on_free_bytes);
  ASSERT_EQ(16U, config->free_track_backtrace_num_frames);

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  ASSERT_STREQ("", getFakeLogPrint().c_str());
}

TEST_F(MallocDebugConfigTest, free_track_and_fill_on_free) {
  ASSERT_TRUE(InitConfig("free_track=1234 fill_on_free=32"));
  ASSERT_EQ(FREE_TRACK | FILL_ON_FREE, config->options);
  ASSERT_EQ(1234U, config->free_track_allocations);
  ASSERT_EQ(32U, config->fill_on_free_bytes);
  ASSERT_EQ(16U, config->free_track_backtrace_num_frames);

  ASSERT_TRUE(InitConfig("free_track fill_on_free=60"));
  ASSERT_EQ(FREE_TRACK | FILL_ON_FREE, config->options);
  ASSERT_EQ(100U, config->free_track_allocations);
  ASSERT_EQ(60U, config->fill_on_free_bytes);
  ASSERT_EQ(16U, config->free_track_backtrace_num_frames);

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  ASSERT_STREQ("", getFakeLogPrint().c_str());
}

TEST_F(MallocDebugConfigTest, free_track_backtrace_num_frames) {
  ASSERT_TRUE(InitConfig("free_track_backtrace_num_frames=123"));

  ASSERT_EQ(0U, config->options);
  ASSERT_EQ(123U, config->free_track_backtrace_num_frames);

  ASSERT_TRUE(InitConfig("free_track_backtrace_num_frames"));
  ASSERT_EQ(0U, config->options);
  ASSERT_EQ(16U, config->free_track_backtrace_num_frames);

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  ASSERT_STREQ("", getFakeLogPrint().c_str());
}

TEST_F(MallocDebugConfigTest, free_track_backtrace_num_frames_zero) {
  ASSERT_TRUE(InitConfig("free_track_backtrace_num_frames=0"));

  ASSERT_EQ(0U, config->options);
  ASSERT_EQ(0U, config->free_track_backtrace_num_frames);

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  ASSERT_STREQ("", getFakeLogPrint().c_str());
}

TEST_F(MallocDebugConfigTest, free_track_backtrace_num_frames_and_free_track) {
  ASSERT_TRUE(InitConfig("free_track free_track_backtrace_num_frames=123"));
  ASSERT_EQ(FREE_TRACK | FILL_ON_FREE, config->options);
  ASSERT_EQ(123U, config->free_track_backtrace_num_frames);

  ASSERT_TRUE(InitConfig("free_track free_track_backtrace_num_frames"));
  ASSERT_EQ(FREE_TRACK | FILL_ON_FREE, config->options);
  ASSERT_EQ(16U, config->free_track_backtrace_num_frames);

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  ASSERT_STREQ("", getFakeLogPrint().c_str());
}

TEST_F(MallocDebugConfigTest, leak_track) {
  ASSERT_TRUE(InitConfig("leak_track"));
  ASSERT_EQ(LEAK_TRACK | TRACK_ALLOCS, config->options);

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  ASSERT_STREQ("", getFakeLogPrint().c_str());
}

TEST_F(MallocDebugConfigTest, leak_track_fail) {
  ASSERT_FALSE(InitConfig("leak_track=100"));

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  std::string log_msg(
      "6 malloc_debug malloc_testing: value set for option 'leak_track' "
      "which does not take a value\n");
  ASSERT_STREQ((log_msg + usage_string).c_str(), getFakeLogPrint().c_str());
}

TEST_F(MallocDebugConfigTest, guard_min_error) {
  ASSERT_FALSE(InitConfig("guard=0"));

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  std::string log_msg(
      "6 malloc_debug malloc_testing: bad value for option 'guard', value must be >= 1: 0\n");
  ASSERT_STREQ((log_msg + usage_string).c_str(), getFakeLogPrint().c_str());
}

TEST_F(MallocDebugConfigTest, guard_max_error) {
  ASSERT_FALSE(InitConfig("guard=20000"));

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  std::string log_msg(
      "6 malloc_debug malloc_testing: bad value for option 'guard', "
      "value must be <= 16384: 20000\n");
  ASSERT_STREQ((log_msg + usage_string).c_str(), getFakeLogPrint().c_str());
}

TEST_F(MallocDebugConfigTest, front_guard_min_error) {
  ASSERT_FALSE(InitConfig("front_guard=0"));

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  std::string log_msg(
      "6 malloc_debug malloc_testing: bad value for option 'front_guard', "
      "value must be >= 1: 0\n");
  ASSERT_STREQ((log_msg + usage_string).c_str(), getFakeLogPrint().c_str());
}

TEST_F(MallocDebugConfigTest, front_guard_max_error) {
  ASSERT_FALSE(InitConfig("front_guard=20000"));

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  std::string log_msg(
      "6 malloc_debug malloc_testing: bad value for option 'front_guard', "
      "value must be <= 16384: 20000\n");
  ASSERT_STREQ((log_msg + usage_string).c_str(), getFakeLogPrint().c_str());
}

TEST_F(MallocDebugConfigTest, rear_guard_min_error) {
  ASSERT_FALSE(InitConfig("rear_guard=0"));

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  std::string log_msg(
      "6 malloc_debug malloc_testing: bad value for option 'rear_guard', "
      "value must be >= 1: 0\n");
  ASSERT_STREQ((log_msg + usage_string).c_str(), getFakeLogPrint().c_str());
}

TEST_F(MallocDebugConfigTest, rear_guard_max_error) {
  ASSERT_FALSE(InitConfig("rear_guard=20000"));

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  std::string log_msg(
      "6 malloc_debug malloc_testing: bad value for option 'rear_guard', "
      "value must be <= 16384: 20000\n");
  ASSERT_STREQ((log_msg + usage_string).c_str(), getFakeLogPrint().c_str());
}

TEST_F(MallocDebugConfigTest, fill_min_error) {
  ASSERT_FALSE(InitConfig("fill=0"));

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  std::string log_msg(
      "6 malloc_debug malloc_testing: bad value for option 'fill', "
      "value must be >= 1: 0\n");
  ASSERT_STREQ((log_msg + usage_string).c_str(), getFakeLogPrint().c_str());
}

TEST_F(MallocDebugConfigTest, fill_on_alloc_min_error) {
  ASSERT_FALSE(InitConfig("fill_on_alloc=0"));

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  std::string log_msg(
      "6 malloc_debug malloc_testing: bad value for option 'fill_on_alloc', "
      "value must be >= 1: 0\n");
  ASSERT_STREQ((log_msg + usage_string).c_str(), getFakeLogPrint().c_str());
}

TEST_F(MallocDebugConfigTest, fill_on_free_min_error) {
  ASSERT_FALSE(InitConfig("fill_on_free=0"));

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  std::string log_msg(
      "6 malloc_debug malloc_testing: bad value for option 'fill_on_free', "
      "value must be >= 1: 0\n");
  ASSERT_STREQ((log_msg + usage_string).c_str(), getFakeLogPrint().c_str());
}

TEST_F(MallocDebugConfigTest, backtrace_min_error) {
  ASSERT_FALSE(InitConfig("backtrace=0"));

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  std::string log_msg(
      "6 malloc_debug malloc_testing: bad value for option 'backtrace', "
      "value must be >= 1: 0\n");
  ASSERT_STREQ((log_msg + usage_string).c_str(), getFakeLogPrint().c_str());
}

TEST_F(MallocDebugConfigTest, backtrace_max_error) {
  ASSERT_FALSE(InitConfig("backtrace=300"));

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  std::string log_msg(
      "6 malloc_debug malloc_testing: bad value for option 'backtrace', "
      "value must be <= 256: 300\n");
  ASSERT_STREQ((log_msg + usage_string).c_str(), getFakeLogPrint().c_str());
}

TEST_F(MallocDebugConfigTest, backtrace_enable_on_signal_min_error) {
  ASSERT_FALSE(InitConfig("backtrace_enable_on_signal=0"));

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  std::string log_msg(
      "6 malloc_debug malloc_testing: bad value for option 'backtrace_enable_on_signal', "
      "value must be >= 1: 0\n");
  ASSERT_STREQ((log_msg + usage_string).c_str(), getFakeLogPrint().c_str());
}

TEST_F(MallocDebugConfigTest, backtrace_enable_on_signal_max_error) {
  ASSERT_FALSE(InitConfig("backtrace_enable_on_signal=300"));

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  std::string log_msg(
      "6 malloc_debug malloc_testing: bad value for option 'backtrace_enable_on_signal', "
      "value must be <= 256: 300\n");
  ASSERT_STREQ((log_msg + usage_string).c_str(), getFakeLogPrint().c_str());
}

TEST_F(MallocDebugConfigTest, expand_alloc_min_error) {
  ASSERT_FALSE(InitConfig("expand_alloc=0"));

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  std::string log_msg(
      "6 malloc_debug malloc_testing: bad value for option 'expand_alloc', "
      "value must be >= 1: 0\n");
  ASSERT_STREQ((log_msg + usage_string).c_str(), getFakeLogPrint().c_str());
}

TEST_F(MallocDebugConfigTest, expand_alloc_max_error) {
  ASSERT_FALSE(InitConfig("expand_alloc=21000"));

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  std::string log_msg(
      "6 malloc_debug malloc_testing: bad value for option 'expand_alloc', "
      "value must be <= 16384: 21000\n");
  ASSERT_STREQ((log_msg + usage_string).c_str(), getFakeLogPrint().c_str());
}

TEST_F(MallocDebugConfigTest, free_track_min_error) {
  ASSERT_FALSE(InitConfig("free_track=0"));

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  std::string log_msg(
      "6 malloc_debug malloc_testing: bad value for option 'free_track', "
      "value must be >= 1: 0\n");
  ASSERT_STREQ((log_msg + usage_string).c_str(), getFakeLogPrint().c_str());
}

TEST_F(MallocDebugConfigTest, free_track_max_error) {
  ASSERT_FALSE(InitConfig("free_track=21000"));

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  std::string log_msg(
      "6 malloc_debug malloc_testing: bad value for option 'free_track', "
      "value must be <= 16384: 21000\n");
  ASSERT_STREQ((log_msg + usage_string).c_str(), getFakeLogPrint().c_str());
}

TEST_F(MallocDebugConfigTest, free_track_backtrace_num_frames_max_error) {
  ASSERT_FALSE(InitConfig("free_track_backtrace_num_frames=400"));

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  std::string log_msg(
      "6 malloc_debug malloc_testing: bad value for option 'free_track_backtrace_num_frames', "
      "value must be <= 256: 400\n");
  ASSERT_STREQ((log_msg + usage_string).c_str(), getFakeLogPrint().c_str());
}
