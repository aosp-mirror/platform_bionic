// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "leb128.h"

#include <vector>
#include "gtest/gtest.h"

namespace relocation_packer {

TEST(Leb128, Encoder64) {
  std::vector<uint64_t> values;
  values.push_back(624485);
  values.push_back(0);
  values.push_back(1);
  values.push_back(127);
  values.push_back(128);

  Leb128Encoder<uint64_t> encoder;
  encoder.EnqueueAll(values);

  encoder.Enqueue(4294967295);
  encoder.Enqueue(18446744073709551615ul);

  std::vector<uint8_t> encoding;
  encoder.GetEncoding(&encoding);

  EXPECT_EQ(23U, encoding.size());
  // 624485
  EXPECT_EQ(0xe5, encoding[0]);
  EXPECT_EQ(0x8e, encoding[1]);
  EXPECT_EQ(0x26, encoding[2]);
  // 0
  EXPECT_EQ(0x00, encoding[3]);
  // 1
  EXPECT_EQ(0x01, encoding[4]);
  // 127
  EXPECT_EQ(0x7f, encoding[5]);
  // 128
  EXPECT_EQ(0x80, encoding[6]);
  EXPECT_EQ(0x01, encoding[7]);
  // 4294967295
  EXPECT_EQ(0xff, encoding[8]);
  EXPECT_EQ(0xff, encoding[9]);
  EXPECT_EQ(0xff, encoding[10]);
  EXPECT_EQ(0xff, encoding[11]);
  EXPECT_EQ(0x0f, encoding[12]);
  // 18446744073709551615
  EXPECT_EQ(0xff, encoding[13]);
  EXPECT_EQ(0xff, encoding[14]);
  EXPECT_EQ(0xff, encoding[15]);
  EXPECT_EQ(0xff, encoding[16]);
  EXPECT_EQ(0xff, encoding[17]);
  EXPECT_EQ(0xff, encoding[18]);
  EXPECT_EQ(0xff, encoding[19]);
  EXPECT_EQ(0xff, encoding[20]);
  EXPECT_EQ(0xff, encoding[21]);
  EXPECT_EQ(0x01, encoding[22]);
}

TEST(Leb128, Decoder64) {
  std::vector<uint8_t> encoding;
  // 624485
  encoding.push_back(0xe5);
  encoding.push_back(0x8e);
  encoding.push_back(0x26);
  // 0
  encoding.push_back(0x00);
  // 1
  encoding.push_back(0x01);
  // 127
  encoding.push_back(0x7f);
  // 128
  encoding.push_back(0x80);
  encoding.push_back(0x01);
  // 4294967295
  encoding.push_back(0xff);
  encoding.push_back(0xff);
  encoding.push_back(0xff);
  encoding.push_back(0xff);
  encoding.push_back(0x0f);
  // 18446744073709551615
  encoding.push_back(0xff);
  encoding.push_back(0xff);
  encoding.push_back(0xff);
  encoding.push_back(0xff);
  encoding.push_back(0xff);
  encoding.push_back(0xff);
  encoding.push_back(0xff);
  encoding.push_back(0xff);
  encoding.push_back(0xff);
  encoding.push_back(0x01);

  Leb128Decoder<uint64_t> decoder(encoding, 0);

  EXPECT_EQ(624485U, decoder.Dequeue());

  std::vector<uint64_t> dequeued;
  decoder.DequeueAll(&dequeued);

  EXPECT_EQ(6U, dequeued.size());
  EXPECT_EQ(0U, dequeued[0]);
  EXPECT_EQ(1U, dequeued[1]);
  EXPECT_EQ(127U, dequeued[2]);
  EXPECT_EQ(128U, dequeued[3]);
  EXPECT_EQ(4294967295U, dequeued[4]);
  EXPECT_EQ(18446744073709551615UL, dequeued[5]);
}

}  // namespace relocation_packer
