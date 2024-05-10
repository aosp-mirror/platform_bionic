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

class MallocDebugConfigTest : public ::testing::Test {
 protected:
  void SetUp() override {
    resetLogs();
  }

  std::unique_ptr<Config> config;

  bool InitConfig(const char* options) {
    config.reset(new Config);
    return config->Init(options);
  }
};

std::string usage_string(
    "6 malloc_debug For malloc debug option descriptions go to:\n"
    "6 malloc_debug   "
    "https://android.googlesource.com/platform/bionic/+/main/libc/malloc_debug/README.md\n");

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
  ASSERT_TRUE(InitConfig("backtrace  =10")) << getFakeLogPrint();
  ASSERT_EQ(BACKTRACE | TRACK_ALLOCS, config->options());
  ASSERT_EQ(10U, config->backtrace_frames());

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  ASSERT_STREQ("", getFakeLogPrint().c_str());
}

TEST_F(MallocDebugConfigTest, space_after_equal) {
  ASSERT_TRUE(InitConfig("backtrace=  10")) << getFakeLogPrint();
  ASSERT_EQ(BACKTRACE | TRACK_ALLOCS, config->options());
  ASSERT_EQ(10U, config->backtrace_frames());

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  ASSERT_STREQ("", getFakeLogPrint().c_str());
}

TEST_F(MallocDebugConfigTest, extra_space) {
  ASSERT_TRUE(InitConfig("   backtrace=64   ")) << getFakeLogPrint();
  ASSERT_EQ(BACKTRACE | TRACK_ALLOCS, config->options());
  ASSERT_EQ(64U, config->backtrace_frames());

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  ASSERT_STREQ("", getFakeLogPrint().c_str());
}

TEST_F(MallocDebugConfigTest, multiple_options) {
  ASSERT_TRUE(InitConfig("  backtrace=64   front_guard=48")) << getFakeLogPrint();
  ASSERT_EQ(BACKTRACE | TRACK_ALLOCS | FRONT_GUARD, config->options());
  ASSERT_EQ(64U, config->backtrace_frames());
  ASSERT_EQ(48U, config->front_guard_bytes());

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  ASSERT_STREQ("", getFakeLogPrint().c_str());
}

TEST_F(MallocDebugConfigTest, front_guard) {
  ASSERT_TRUE(InitConfig("front_guard=48")) << getFakeLogPrint();
  ASSERT_EQ(FRONT_GUARD | TRACK_ALLOCS, config->options());
  ASSERT_EQ(48U, config->front_guard_bytes());

  ASSERT_TRUE(InitConfig("front_guard")) << getFakeLogPrint();
  ASSERT_EQ(FRONT_GUARD | TRACK_ALLOCS, config->options());
  ASSERT_EQ(32U, config->front_guard_bytes());

  ASSERT_TRUE(InitConfig("front_guard=39")) << getFakeLogPrint();
  ASSERT_EQ(FRONT_GUARD | TRACK_ALLOCS, config->options());
#if defined(__LP64__)
  ASSERT_EQ(48U, config->front_guard_bytes());
#else
  ASSERT_EQ(40U, config->front_guard_bytes());
#endif

  ASSERT_TRUE(InitConfig("front_guard=41")) << getFakeLogPrint();
  ASSERT_EQ(FRONT_GUARD | TRACK_ALLOCS, config->options());
  ASSERT_EQ(48U, config->front_guard_bytes());

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  ASSERT_STREQ("", getFakeLogPrint().c_str());
}

TEST_F(MallocDebugConfigTest, rear_guard) {
  ASSERT_TRUE(InitConfig("rear_guard=50")) << getFakeLogPrint();
  ASSERT_EQ(REAR_GUARD | TRACK_ALLOCS, config->options());
  ASSERT_EQ(50U, config->rear_guard_bytes());

  ASSERT_TRUE(InitConfig("rear_guard")) << getFakeLogPrint();
  ASSERT_EQ(REAR_GUARD | TRACK_ALLOCS, config->options());
  ASSERT_EQ(32U, config->rear_guard_bytes());

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  ASSERT_STREQ("", getFakeLogPrint().c_str());
}

TEST_F(MallocDebugConfigTest, guard) {
  ASSERT_TRUE(InitConfig("guard=32")) << getFakeLogPrint();
  ASSERT_EQ(FRONT_GUARD | REAR_GUARD | TRACK_ALLOCS, config->options());
  ASSERT_EQ(32U, config->front_guard_bytes());
  ASSERT_EQ(32U, config->rear_guard_bytes());

  ASSERT_TRUE(InitConfig("guard")) << getFakeLogPrint();
  ASSERT_EQ(FRONT_GUARD | REAR_GUARD | TRACK_ALLOCS, config->options());
  ASSERT_EQ(32U, config->front_guard_bytes());
  ASSERT_EQ(32U, config->rear_guard_bytes());

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  ASSERT_STREQ("", getFakeLogPrint().c_str());
}

TEST_F(MallocDebugConfigTest, backtrace) {
  ASSERT_TRUE(InitConfig("backtrace=64")) << getFakeLogPrint();
  ASSERT_EQ(BACKTRACE | TRACK_ALLOCS, config->options());
  ASSERT_EQ(64U, config->backtrace_frames());
  ASSERT_TRUE(config->backtrace_enabled());
  ASSERT_FALSE(config->backtrace_enable_on_signal());
  ASSERT_FALSE(config->backtrace_dump_on_exit());

  ASSERT_TRUE(InitConfig("bt=23")) << getFakeLogPrint();
  ASSERT_EQ(BACKTRACE | TRACK_ALLOCS, config->options());
  ASSERT_EQ(23U, config->backtrace_frames());
  ASSERT_TRUE(config->backtrace_enabled());
  ASSERT_FALSE(config->backtrace_enable_on_signal());
  ASSERT_FALSE(config->backtrace_dump_on_exit());

  ASSERT_TRUE(InitConfig("backtrace")) << getFakeLogPrint();
  ASSERT_EQ(BACKTRACE | TRACK_ALLOCS, config->options());
  ASSERT_EQ(16U, config->backtrace_frames());
  ASSERT_TRUE(config->backtrace_enabled());
  ASSERT_FALSE(config->backtrace_enable_on_signal());
  ASSERT_FALSE(config->backtrace_dump_on_exit());

  ASSERT_TRUE(InitConfig("bt")) << getFakeLogPrint();
  ASSERT_EQ(BACKTRACE | TRACK_ALLOCS, config->options());
  ASSERT_EQ(16U, config->backtrace_frames());
  ASSERT_TRUE(config->backtrace_enabled());
  ASSERT_FALSE(config->backtrace_enable_on_signal());
  ASSERT_FALSE(config->backtrace_dump_on_exit());

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  ASSERT_STREQ("", getFakeLogPrint().c_str());
}

TEST_F(MallocDebugConfigTest, backtrace_enable_on_signal) {
  ASSERT_TRUE(InitConfig("backtrace_enable_on_signal=64")) << getFakeLogPrint();
  ASSERT_EQ(BACKTRACE | TRACK_ALLOCS, config->options());
  ASSERT_EQ(64U, config->backtrace_frames());
  ASSERT_FALSE(config->backtrace_enabled());
  ASSERT_TRUE(config->backtrace_enable_on_signal());
  ASSERT_FALSE(config->backtrace_dump_on_exit());

  ASSERT_TRUE(InitConfig("bt_en_on_sig=64")) << getFakeLogPrint();
  ASSERT_EQ(BACKTRACE | TRACK_ALLOCS, config->options());
  ASSERT_EQ(64U, config->backtrace_frames());
  ASSERT_FALSE(config->backtrace_enabled());
  ASSERT_TRUE(config->backtrace_enable_on_signal());
  ASSERT_FALSE(config->backtrace_dump_on_exit());

  ASSERT_TRUE(InitConfig("backtrace_enable_on_signal")) << getFakeLogPrint();
  ASSERT_EQ(BACKTRACE | TRACK_ALLOCS, config->options());
  ASSERT_EQ(16U, config->backtrace_frames());
  ASSERT_FALSE(config->backtrace_enabled());
  ASSERT_TRUE(config->backtrace_enable_on_signal());
  ASSERT_FALSE(config->backtrace_dump_on_exit());

  ASSERT_TRUE(InitConfig("bt_en_on_sig")) << getFakeLogPrint();
  ASSERT_EQ(BACKTRACE | TRACK_ALLOCS, config->options());
  ASSERT_EQ(16U, config->backtrace_frames());
  ASSERT_FALSE(config->backtrace_enabled());
  ASSERT_TRUE(config->backtrace_enable_on_signal());
  ASSERT_FALSE(config->backtrace_dump_on_exit());

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  ASSERT_STREQ("", getFakeLogPrint().c_str());
}

TEST_F(MallocDebugConfigTest, backtrace_enable_on_signal_init) {
  ASSERT_TRUE(InitConfig("backtrace_enable_on_signal=64")) << getFakeLogPrint();
  ASSERT_EQ(BACKTRACE | TRACK_ALLOCS, config->options());
  ASSERT_EQ(64U, config->backtrace_frames());
  ASSERT_FALSE(config->backtrace_enabled());
  ASSERT_TRUE(config->backtrace_enable_on_signal());
  ASSERT_FALSE(config->backtrace_dump_on_exit());

  ASSERT_TRUE(InitConfig("backtrace")) << getFakeLogPrint();
  ASSERT_EQ(BACKTRACE | TRACK_ALLOCS, config->options());
  ASSERT_EQ(16U, config->backtrace_frames());
  ASSERT_TRUE(config->backtrace_enabled());
  ASSERT_FALSE(config->backtrace_enable_on_signal());
  ASSERT_FALSE(config->backtrace_dump_on_exit());

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  ASSERT_STREQ("", getFakeLogPrint().c_str());
}

TEST_F(MallocDebugConfigTest, backtrace_enable_and_backtrace) {
  ASSERT_TRUE(InitConfig("backtrace_enable_on_signal backtrace")) << getFakeLogPrint();
  ASSERT_EQ(BACKTRACE | TRACK_ALLOCS, config->options());
  ASSERT_EQ(16U, config->backtrace_frames());
  ASSERT_TRUE(config->backtrace_enabled());
  ASSERT_TRUE(config->backtrace_enable_on_signal());
  ASSERT_FALSE(config->backtrace_dump_on_exit());

  ASSERT_TRUE(InitConfig("backtrace backtrace_enable_on_signal")) << getFakeLogPrint();
  ASSERT_EQ(BACKTRACE | TRACK_ALLOCS, config->options());
  ASSERT_EQ(16U, config->backtrace_frames());
  ASSERT_TRUE(config->backtrace_enabled());
  ASSERT_TRUE(config->backtrace_enable_on_signal());
  ASSERT_FALSE(config->backtrace_dump_on_exit());

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  ASSERT_STREQ("", getFakeLogPrint().c_str());
}

TEST_F(MallocDebugConfigTest, backtrace_dump_on_exit) {
  ASSERT_TRUE(InitConfig("backtrace_dump_on_exit")) << getFakeLogPrint();
  ASSERT_EQ(0U, config->options());
  ASSERT_TRUE(config->backtrace_dump_on_exit());

  ASSERT_TRUE(InitConfig("bt_dmp_on_ex")) << getFakeLogPrint();
  ASSERT_EQ(0U, config->options());
  ASSERT_TRUE(config->backtrace_dump_on_exit());

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  ASSERT_STREQ("", getFakeLogPrint().c_str());
}

TEST_F(MallocDebugConfigTest, backtrace_dump_on_exit_error) {
  ASSERT_FALSE(InitConfig("backtrace_dump_on_exit=something")) << getFakeLogPrint();

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  std::string log_msg(
      "6 malloc_debug malloc_testing: value set for option 'backtrace_dump_on_exit' "
      "which does not take a value\n");
  ASSERT_STREQ((log_msg + usage_string).c_str(), getFakeLogPrint().c_str());
}

TEST_F(MallocDebugConfigTest, backtrace_dump_prefix) {
  ASSERT_TRUE(InitConfig("backtrace_dump_prefix")) << getFakeLogPrint();
  ASSERT_EQ(0U, config->options());
  ASSERT_EQ("/data/local/tmp/backtrace_heap", config->backtrace_dump_prefix());

  ASSERT_TRUE(InitConfig("bt_dmp_pre")) << getFakeLogPrint();
  ASSERT_EQ(0U, config->options());
  ASSERT_EQ("/data/local/tmp/backtrace_heap", config->backtrace_dump_prefix());

  ASSERT_TRUE(InitConfig("backtrace_dump_prefix=/fake/location")) << getFakeLogPrint();
  ASSERT_EQ(0U, config->options());
  ASSERT_EQ("/fake/location", config->backtrace_dump_prefix());

  ASSERT_TRUE(InitConfig("bt_dmp_pre=/fake/location")) << getFakeLogPrint();
  ASSERT_EQ(0U, config->options());
  ASSERT_EQ("/fake/location", config->backtrace_dump_prefix());

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  ASSERT_STREQ("", getFakeLogPrint().c_str());
}

TEST_F(MallocDebugConfigTest, backtrace_full) {
  ASSERT_TRUE(InitConfig("backtrace_full")) << getFakeLogPrint();
  ASSERT_EQ(BACKTRACE_FULL, config->options());

  ASSERT_TRUE(InitConfig("bt_full")) << getFakeLogPrint();
  ASSERT_EQ(BACKTRACE_FULL, config->options());

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  ASSERT_STREQ("", getFakeLogPrint().c_str());
}

TEST_F(MallocDebugConfigTest, backtrace_full_fail) {
  ASSERT_FALSE(InitConfig("backtrace_full=200")) << getFakeLogPrint();

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  std::string log_msg(
      "6 malloc_debug malloc_testing: value set for option 'backtrace_full' "
      "which does not take a value\n");
  ASSERT_STREQ((log_msg + usage_string).c_str(), getFakeLogPrint().c_str());
}

TEST_F(MallocDebugConfigTest, fill_on_alloc) {
  ASSERT_TRUE(InitConfig("fill_on_alloc=64")) << getFakeLogPrint();
  ASSERT_EQ(FILL_ON_ALLOC, config->options());
  ASSERT_EQ(64U, config->fill_on_alloc_bytes());

  ASSERT_TRUE(InitConfig("fill_on_alloc")) << getFakeLogPrint();
  ASSERT_EQ(FILL_ON_ALLOC, config->options());
  ASSERT_EQ(SIZE_MAX, config->fill_on_alloc_bytes());

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  ASSERT_STREQ("", getFakeLogPrint().c_str());
}

TEST_F(MallocDebugConfigTest, fill_on_free) {
  ASSERT_TRUE(InitConfig("fill_on_free=64")) << getFakeLogPrint();
  ASSERT_EQ(FILL_ON_FREE, config->options());
  ASSERT_EQ(64U, config->fill_on_free_bytes());

  ASSERT_TRUE(InitConfig("fill_on_free")) << getFakeLogPrint();
  ASSERT_EQ(FILL_ON_FREE, config->options());
  ASSERT_EQ(SIZE_MAX, config->fill_on_free_bytes());

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  ASSERT_STREQ("", getFakeLogPrint().c_str());
}

TEST_F(MallocDebugConfigTest, fill) {
  ASSERT_TRUE(InitConfig("fill=64")) << getFakeLogPrint();
  ASSERT_EQ(FILL_ON_ALLOC | FILL_ON_FREE, config->options());
  ASSERT_EQ(64U, config->fill_on_alloc_bytes());
  ASSERT_EQ(64U, config->fill_on_free_bytes());

  ASSERT_TRUE(InitConfig("fill")) << getFakeLogPrint();
  ASSERT_EQ(FILL_ON_ALLOC | FILL_ON_FREE, config->options());
  ASSERT_EQ(SIZE_MAX, config->fill_on_alloc_bytes());
  ASSERT_EQ(SIZE_MAX, config->fill_on_free_bytes());

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  ASSERT_STREQ("", getFakeLogPrint().c_str());
}

TEST_F(MallocDebugConfigTest, expand_alloc) {
  ASSERT_TRUE(InitConfig("expand_alloc=1234")) << getFakeLogPrint();
  ASSERT_EQ(EXPAND_ALLOC, config->options());
  ASSERT_EQ(1234U, config->expand_alloc_bytes());

  ASSERT_TRUE(InitConfig("expand_alloc")) << getFakeLogPrint();
  ASSERT_EQ(EXPAND_ALLOC, config->options());
  ASSERT_EQ(16U, config->expand_alloc_bytes());

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  ASSERT_STREQ("", getFakeLogPrint().c_str());
}

TEST_F(MallocDebugConfigTest, free_track) {
  ASSERT_TRUE(InitConfig("free_track=1234")) << getFakeLogPrint();
  ASSERT_EQ(FREE_TRACK | FILL_ON_FREE | TRACK_ALLOCS, config->options());
  ASSERT_EQ(1234U, config->free_track_allocations());
  ASSERT_EQ(SIZE_MAX, config->fill_on_free_bytes());
  ASSERT_EQ(16U, config->free_track_backtrace_num_frames());

  ASSERT_TRUE(InitConfig("free_track")) << getFakeLogPrint();
  ASSERT_EQ(FREE_TRACK | FILL_ON_FREE | TRACK_ALLOCS, config->options());
  ASSERT_EQ(100U, config->free_track_allocations());
  ASSERT_EQ(SIZE_MAX, config->fill_on_free_bytes());
  ASSERT_EQ(16U, config->free_track_backtrace_num_frames());

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  ASSERT_STREQ("", getFakeLogPrint().c_str());
}

TEST_F(MallocDebugConfigTest, free_track_and_fill_on_free) {
  ASSERT_TRUE(InitConfig("free_track=1234 fill_on_free=32")) << getFakeLogPrint();
  ASSERT_EQ(FREE_TRACK | FILL_ON_FREE | TRACK_ALLOCS, config->options());
  ASSERT_EQ(1234U, config->free_track_allocations());
  ASSERT_EQ(32U, config->fill_on_free_bytes());
  ASSERT_EQ(16U, config->free_track_backtrace_num_frames());

  ASSERT_TRUE(InitConfig("free_track fill_on_free=60")) << getFakeLogPrint();
  ASSERT_EQ(FREE_TRACK | FILL_ON_FREE | TRACK_ALLOCS, config->options());
  ASSERT_EQ(100U, config->free_track_allocations());
  ASSERT_EQ(60U, config->fill_on_free_bytes());
  ASSERT_EQ(16U, config->free_track_backtrace_num_frames());

  // Now reverse the arguments.
  ASSERT_TRUE(InitConfig("fill_on_free=32 free_track=1234")) << getFakeLogPrint();
  ASSERT_EQ(FREE_TRACK | FILL_ON_FREE | TRACK_ALLOCS, config->options());
  ASSERT_EQ(1234U, config->free_track_allocations());
  ASSERT_EQ(32U, config->fill_on_free_bytes());
  ASSERT_EQ(16U, config->free_track_backtrace_num_frames());

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  ASSERT_STREQ("", getFakeLogPrint().c_str());
}

TEST_F(MallocDebugConfigTest, free_track_backtrace_num_frames) {
  ASSERT_TRUE(InitConfig("free_track_backtrace_num_frames=123")) << getFakeLogPrint();

  ASSERT_EQ(0U, config->options());
  ASSERT_EQ(123U, config->free_track_backtrace_num_frames());

  ASSERT_TRUE(InitConfig("free_track_backtrace_num_frames")) << getFakeLogPrint();
  ASSERT_EQ(0U, config->options());
  ASSERT_EQ(16U, config->free_track_backtrace_num_frames());

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  ASSERT_STREQ("", getFakeLogPrint().c_str());
}

TEST_F(MallocDebugConfigTest, free_track_backtrace_num_frames_zero) {
  ASSERT_TRUE(InitConfig("free_track_backtrace_num_frames=0")) << getFakeLogPrint();

  ASSERT_EQ(0U, config->options());
  ASSERT_EQ(0U, config->free_track_backtrace_num_frames());

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  ASSERT_STREQ("", getFakeLogPrint().c_str());
}

TEST_F(MallocDebugConfigTest, free_track_backtrace_num_frames_and_free_track) {
  ASSERT_TRUE(InitConfig("free_track free_track_backtrace_num_frames=123")) << getFakeLogPrint();
  ASSERT_EQ(FREE_TRACK | FILL_ON_FREE | TRACK_ALLOCS, config->options());
  ASSERT_EQ(123U, config->free_track_backtrace_num_frames());

  ASSERT_TRUE(InitConfig("free_track free_track_backtrace_num_frames")) << getFakeLogPrint();
  ASSERT_EQ(FREE_TRACK | FILL_ON_FREE | TRACK_ALLOCS, config->options());
  ASSERT_EQ(16U, config->free_track_backtrace_num_frames());

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  ASSERT_STREQ("", getFakeLogPrint().c_str());
}

TEST_F(MallocDebugConfigTest, leak_track) {
  ASSERT_TRUE(InitConfig("leak_track")) << getFakeLogPrint();
  ASSERT_EQ(LEAK_TRACK | TRACK_ALLOCS, config->options());

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  ASSERT_STREQ("", getFakeLogPrint().c_str());
}

TEST_F(MallocDebugConfigTest, leak_track_fail) {
  ASSERT_FALSE(InitConfig("leak_track=100")) << getFakeLogPrint();

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  std::string log_msg(
      "6 malloc_debug malloc_testing: value set for option 'leak_track' "
      "which does not take a value\n");
  ASSERT_STREQ((log_msg + usage_string).c_str(), getFakeLogPrint().c_str());
}

TEST_F(MallocDebugConfigTest, verify_pointers) {
  ASSERT_TRUE(InitConfig("verify_pointers")) << getFakeLogPrint();
  ASSERT_EQ(TRACK_ALLOCS, config->options());

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  ASSERT_STREQ("", getFakeLogPrint().c_str());
}

TEST_F(MallocDebugConfigTest, verify_pointers_fail) {
  ASSERT_FALSE(InitConfig("verify_pointers=200")) << getFakeLogPrint();

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  std::string log_msg(
      "6 malloc_debug malloc_testing: value set for option 'verify_pointers' "
      "which does not take a value\n");
  ASSERT_STREQ((log_msg + usage_string).c_str(), getFakeLogPrint().c_str());
}

TEST_F(MallocDebugConfigTest, record_allocs) {
  ASSERT_TRUE(InitConfig("record_allocs=1234")) << getFakeLogPrint();
  ASSERT_EQ(RECORD_ALLOCS, config->options());
  ASSERT_EQ(1234U, config->record_allocs_num_entries());
  ASSERT_STREQ("/data/local/tmp/record_allocs.txt", config->record_allocs_file().c_str());

  ASSERT_TRUE(InitConfig("record_allocs")) << getFakeLogPrint();
  ASSERT_EQ(RECORD_ALLOCS, config->options());
  ASSERT_EQ(8000000U, config->record_allocs_num_entries());
  ASSERT_STREQ("/data/local/tmp/record_allocs.txt", config->record_allocs_file().c_str());

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  ASSERT_STREQ("", getFakeLogPrint().c_str());
}

TEST_F(MallocDebugConfigTest, record_allocs_file) {
  ASSERT_TRUE(InitConfig("record_allocs=1234 record_allocs_file=/fake/file")) << getFakeLogPrint();
  ASSERT_STREQ("/fake/file", config->record_allocs_file().c_str());

  ASSERT_TRUE(InitConfig("record_allocs_file")) << getFakeLogPrint();
  ASSERT_STREQ("/data/local/tmp/record_allocs.txt", config->record_allocs_file().c_str());

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  ASSERT_STREQ("", getFakeLogPrint().c_str());
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

TEST_F(MallocDebugConfigTest, record_alloc_min_error) {
  ASSERT_FALSE(InitConfig("record_allocs=0"));

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  std::string log_msg(
      "6 malloc_debug malloc_testing: bad value for option 'record_allocs', "
      "value must be >= 1: 0\n");
  ASSERT_STREQ((log_msg + usage_string).c_str(), getFakeLogPrint().c_str());
}

TEST_F(MallocDebugConfigTest, record_allocs_max_error) {
  ASSERT_FALSE(InitConfig("record_allocs=100000000"));

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  std::string log_msg(
      "6 malloc_debug malloc_testing: bad value for option 'record_allocs', "
      "value must be <= 50000000: 100000000\n");
  ASSERT_STREQ((log_msg + usage_string).c_str(), getFakeLogPrint().c_str());
}

TEST_F(MallocDebugConfigTest, abort_on_error) {
  ASSERT_TRUE(InitConfig("abort_on_error")) << getFakeLogPrint();
  ASSERT_EQ(ABORT_ON_ERROR, config->options());

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  ASSERT_STREQ("", getFakeLogPrint().c_str());
}

TEST_F(MallocDebugConfigTest, trigger_abort_fail) {
  ASSERT_FALSE(InitConfig("abort_on_error=200")) << getFakeLogPrint();

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  std::string log_msg(
      "6 malloc_debug malloc_testing: value set for option 'abort_on_error' "
      "which does not take a value\n");
  ASSERT_STREQ((log_msg + usage_string).c_str(), getFakeLogPrint().c_str());
}

TEST_F(MallocDebugConfigTest, verbose) {
  ASSERT_TRUE(InitConfig("verbose")) << getFakeLogPrint();
  ASSERT_EQ(VERBOSE, config->options());

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  ASSERT_STREQ("", getFakeLogPrint().c_str());
}

TEST_F(MallocDebugConfigTest, trigger_verbose_fail) {
  ASSERT_FALSE(InitConfig("verbose=200")) << getFakeLogPrint();

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  std::string log_msg(
      "6 malloc_debug malloc_testing: value set for option 'verbose' "
      "which does not take a value\n");
  ASSERT_STREQ((log_msg + usage_string).c_str(), getFakeLogPrint().c_str());
}

TEST_F(MallocDebugConfigTest, check_unreachable_on_signal) {
  ASSERT_TRUE(InitConfig("check_unreachable_on_signal")) << getFakeLogPrint();
  ASSERT_EQ(CHECK_UNREACHABLE_ON_SIGNAL, config->options());

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  ASSERT_STREQ("", getFakeLogPrint().c_str());
}

TEST_F(MallocDebugConfigTest, trigger_check_unreachable_on_signal_fail) {
  ASSERT_FALSE(InitConfig("check_unreachable_on_signal=200")) << getFakeLogPrint();

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  std::string log_msg(
      "6 malloc_debug malloc_testing: value set for option 'check_unreachable_on_signal' "
      "which does not take a value\n");
  ASSERT_STREQ((log_msg + usage_string).c_str(), getFakeLogPrint().c_str());
}

TEST_F(MallocDebugConfigTest, log_allocator_stats_on_signal) {
  ASSERT_TRUE(InitConfig("log_allocator_stats_on_signal")) << getFakeLogPrint();
  ASSERT_EQ(LOG_ALLOCATOR_STATS_ON_SIGNAL, config->options());

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  ASSERT_STREQ("", getFakeLogPrint().c_str());
}

TEST_F(MallocDebugConfigTest, trigger_log_allocator_stats_on_signal_fail) {
  ASSERT_FALSE(InitConfig("log_allocator_stats_on_signal=200")) << getFakeLogPrint();

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  std::string log_msg(
      "6 malloc_debug malloc_testing: value set for option 'log_allocator_stats_on_signal' "
      "which does not take a value\n");
  ASSERT_STREQ((log_msg + usage_string).c_str(), getFakeLogPrint().c_str());
}

TEST_F(MallocDebugConfigTest, size) {
  ASSERT_TRUE(InitConfig("backtrace_size=37")) << getFakeLogPrint();
  ASSERT_EQ(BACKTRACE_SPECIFIC_SIZES, config->options());
  ASSERT_EQ(37U, config->backtrace_min_size_bytes());
  ASSERT_EQ(37U, config->backtrace_max_size_bytes());

  ASSERT_TRUE(InitConfig("bt_sz=39")) << getFakeLogPrint();
  ASSERT_EQ(BACKTRACE_SPECIFIC_SIZES, config->options());
  ASSERT_EQ(39U, config->backtrace_min_size_bytes());
  ASSERT_EQ(39U, config->backtrace_max_size_bytes());

  ASSERT_FALSE(InitConfig("backtrace_size")) << getFakeLogPrint();
  ASSERT_FALSE(InitConfig("backtrace_size=0")) << getFakeLogPrint();
  ASSERT_FALSE(InitConfig("backtrace_size=-1")) << getFakeLogPrint();

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  std::string log_msg("6 malloc_debug malloc_testing: bad value for option 'backtrace_size'\n" +
                      usage_string +
                      "6 malloc_debug malloc_testing: bad value for option 'backtrace_size', value "
                      "must be >= 1: 0\n" +
                      usage_string +
                      "6 malloc_debug malloc_testing: bad value for option 'backtrace_size', value "
                      "cannot be negative: -1\n" +
                      usage_string);
  ASSERT_STREQ(log_msg.c_str(), getFakeLogPrint().c_str());
}

TEST_F(MallocDebugConfigTest, min_size) {
  ASSERT_TRUE(InitConfig("backtrace_min_size=9")) << getFakeLogPrint();
  ASSERT_EQ(BACKTRACE_SPECIFIC_SIZES, config->options());
  ASSERT_EQ(9U, config->backtrace_min_size_bytes());
  ASSERT_EQ(SIZE_MAX, config->backtrace_max_size_bytes());

  ASSERT_TRUE(InitConfig("bt_min_sz=11")) << getFakeLogPrint();
  ASSERT_EQ(BACKTRACE_SPECIFIC_SIZES, config->options());
  ASSERT_EQ(11U, config->backtrace_min_size_bytes());
  ASSERT_EQ(SIZE_MAX, config->backtrace_max_size_bytes());

  ASSERT_FALSE(InitConfig("backtrace_min_size")) << getFakeLogPrint();
  ASSERT_FALSE(InitConfig("backtrace_min_size=0")) << getFakeLogPrint();
  ASSERT_FALSE(InitConfig("backtrace_min_size=-1")) << getFakeLogPrint();

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  std::string log_msg("6 malloc_debug malloc_testing: bad value for option 'backtrace_min_size'\n" +
                      usage_string +
                      "6 malloc_debug malloc_testing: bad value for option 'backtrace_min_size', "
                      "value must be >= 1: 0\n" +
                      usage_string +
                      "6 malloc_debug malloc_testing: bad value for option 'backtrace_min_size', "
                      "value cannot be negative: -1\n" +
                      usage_string);
  ASSERT_STREQ(log_msg.c_str(), getFakeLogPrint().c_str());
}

TEST_F(MallocDebugConfigTest, max_size) {
  ASSERT_TRUE(InitConfig("backtrace_max_size=13")) << getFakeLogPrint();
  ASSERT_EQ(BACKTRACE_SPECIFIC_SIZES, config->options());
  ASSERT_EQ(0U, config->backtrace_min_size_bytes());
  ASSERT_EQ(13U, config->backtrace_max_size_bytes());

  ASSERT_TRUE(InitConfig("bt_max_sz=15")) << getFakeLogPrint();
  ASSERT_EQ(BACKTRACE_SPECIFIC_SIZES, config->options());
  ASSERT_EQ(0U, config->backtrace_min_size_bytes());
  ASSERT_EQ(15U, config->backtrace_max_size_bytes());

  ASSERT_FALSE(InitConfig("backtrace_max_size")) << getFakeLogPrint();
  ASSERT_FALSE(InitConfig("backtrace_max_size=0")) << getFakeLogPrint();
  ASSERT_FALSE(InitConfig("backtrace_max_size=-1")) << getFakeLogPrint();

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  std::string log_msg("6 malloc_debug malloc_testing: bad value for option 'backtrace_max_size'\n" +
                      usage_string +
                      "6 malloc_debug malloc_testing: bad value for option 'backtrace_max_size', "
                      "value must be >= 1: 0\n" +
                      usage_string +
                      "6 malloc_debug malloc_testing: bad value for option 'backtrace_max_size', "
                      "value cannot be negative: -1\n" +
                      usage_string);
  ASSERT_STREQ(log_msg.c_str(), getFakeLogPrint().c_str());
}
