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

#include <math.h>

#include <gtest/gtest.h>

#if defined(__BIONIC__)
typedef struct {
  double expected;
  double call_data;
} sin_intel_data_t;

static sin_intel_data_t g_sin_intel_data[] = {
  { // Entry 0
    0x1.9259e3708bd39ffffffffffffff1bdbep-5,
    0x1.9283586503fe0p-5
  },
  { // Entry 1
    -0x1.9259e3708bd39ffffffffffffff1bdbep-5,
    -0x1.9283586503fe0p-5
  },
  { // Entry 2
    0x1.d77b117f230d5ffffffffffffff2adc7p-5,
    0x1.d7bdcd778049fp-5
  },
  { // Entry 3
    -0x1.d77b117f230d5ffffffffffffff2adc7p-5,
    -0x1.d7bdcd778049fp-5
  },
  { // Entry 4
    0x1.a1490c8c06ba6fffffffffffffa98611p-4,
    0x1.a202b3fb84788p-4
  },
  { // Entry 5
    -0x1.a1490c8c06ba6fffffffffffffa98611p-4,
    -0x1.a202b3fb84788p-4
  },
  { // Entry 6
    0x1.cc40c3805229a7ffffffffffff83e76bp-3,
    0x1.d037cb27ee6dfp-3
  },
  { // Entry 7
    -0x1.cc40c3805229a7ffffffffffff83e76bp-3,
    -0x1.d037cb27ee6dfp-3
  },
  { // Entry 8
    0x1.d0ef799001ba900000000000005ea4e9p-3,
    0x1.d5064e6fe82c5p-3
  },
  { // Entry 9
    -0x1.d0ef799001ba900000000000005ea4e9p-3,
    -0x1.d5064e6fe82c5p-3
  },
  { // Entry 10
    0x1.e9950730c4695ffffffffffffffffc2ep-2,
    0x1.fe767739d0f6dp-2
  },
  { // Entry 11
    -0x1.e9950730c4695ffffffffffffffffc2ep-2,
    -0x1.fe767739d0f6dp-2
  },
  { // Entry 12
    0x1.98dcd093377928000000000000167dc7p-1,
    0x1.d98c4c612718dp-1
  },
  { // Entry 13
    -0x1.98dcd093377928000000000000167dc7p-1,
    -0x1.d98c4c612718dp-1
  },
  { // Entry 14
    0x1.ffffffffffffffffffffffffffec8831p-1,
    0x1.921fb54442d18p0
  },
  { // Entry 15
    -0x1.ffffffffffffffffffffffffffec8831p-1,
    -0x1.921fb54442d18p0
  },
  { // Entry 16
    0x1.70a9d825b506400422155aecd519d54cp-1,
    -0x1.0000001f8p500
  },
  { // Entry 17
    -0x1.70a9d825b506400422155aecd519d54cp-1,
    0x1.0000001f8p500
  },
  { // Entry 18
    0x1.bf3980c6c1e9f4496795f4ce535016b6p-1,
    -0x1.00c0bf8p700
  },
  { // Entry 19
    -0x1.bf3980c6c1e9f4496795f4ce535016b6p-1,
    0x1.00c0bf8p700
  },
  { // Entry 20
    0x1.d62899d48b4397fb275ba44095971364p-4,
    -0x1.13fffffffff80p6
  },
  { // Entry 21
    -0x1.d62899d48b4397fb275ba44095971364p-4,
    0x1.13fffffffff80p6
  },
  { // Entry 22
    -0x1.17b7a60ce1f1485824711523ab0fd6d1p-5,
    -0x1.17c5920767dfcp-5
  },
  { // Entry 23
    0x1.17b7a60ce1f1485824711523ab0fd6d1p-5,
    0x1.17c5920767dfcp-5
  },
  { // Entry 24
    0x1.f0192b794fbbe030fe25edf65a5db9bfp-1,
    -0x1.1d99be08713ccp2
  },
  { // Entry 25
    -0x1.f0192b794fbbe030fe25edf65a5db9bfp-1,
    0x1.1d99be08713ccp2
  },
  { // Entry 26
    -0x1.5e61328c0034ec3a05d053b72e2d5945p-3,
    -0x1.1ddbfd64fc0d3p81
  },
  { // Entry 27
    0x1.5e61328c0034ec3a05d053b72e2d5945p-3,
    0x1.1ddbfd64fc0d3p81
  },
  { // Entry 28
    0x1.fb028c5df1db3eb32b0062c6cd4dea0bp-1,
    -0x1.1e2a1563e068ep7
  },
  { // Entry 29
    -0x1.fb028c5df1db3eb32b0062c6cd4dea0bp-1,
    0x1.1e2a1563e068ep7
  },
  { // Entry 30
    -0x1.2cefb196ba2077fffffd4bd650722bfbp-3,
    -0x1.2e07a91314dp-3
  },
  { // Entry 31
    0x1.2cefb196ba2077fffffd4bd650722bfbp-3,
    0x1.2e07a91314dp-3
  },
  { // Entry 32
    0x1.b80f489d3edf4835de7deeeb5bb38ad9p-2,
    -0x1.3bcec270444e2p3
  },
  { // Entry 33
    -0x1.b80f489d3edf4835de7deeeb5bb38ad9p-2,
    0x1.3bcec270444e2p3
  },
  { // Entry 34
    -0x1.4fffffffffa38800000004dd22ccccccp-20,
    -0x1.5000000000040p-20
  },
  { // Entry 35
    0x1.4fffffffffa38800000004dd22ccccccp-20,
    0x1.5000000000040p-20
  },
  { // Entry 36
    -0x1.d29da5b44f51b7fc019f183e1d3bd511p-2,
    -0x1.559001a42d90cp1
  },
  { // Entry 37
    0x1.d29da5b44f51b7fc019f183e1d3bd511p-2,
    0x1.559001a42d90cp1
  },
  { // Entry 38
    -0x1.f85f526147f787fffff15241e719734bp-1,
    -0x1.597bf3e9776b7p99
  },
  { // Entry 39
    0x1.f85f526147f787fffff15241e719734bp-1,
    0x1.597bf3e9776b7p99
  },
  { // Entry 40
    -0x1.6d61b58c99c42f1396af4a42148f73c6p-59,
    -0x1.6c6cbc45dc8dep7
  },
  { // Entry 41
    0x1.6d61b58c99c42f1396af4a42148f73c6p-59,
    0x1.6c6cbc45dc8dep7
  },
  { // Entry 42
    0x1.e5c3c08a258a77f1d12efa8c30166709p-1,
    -0x1.73d8d173f90d0p4
  },
  { // Entry 43
    -0x1.e5c3c08a258a77f1d12efa8c30166709p-1,
    0x1.73d8d173f90d0p4
  },
  { // Entry 44
    0x1.feb36806ca5fb7ea442119af41be0f4fp-1,
    -0x1.8c202d3a31802p6
  },
  { // Entry 45
    -0x1.feb36806ca5fb7ea442119af41be0f4fp-1,
    0x1.8c202d3a31802p6
  },
  { // Entry 46
    -0x1.7c6c7b01b98d947cd8add5e5d1ae11cap-1,
    -0x1.acd538b1a6d5dp-1
  },
  { // Entry 47
    0x1.7c6c7b01b98d947cd8add5e5d1ae11cap-1,
    0x1.acd538b1a6d5dp-1
  },
  { // Entry 48
    -0x1.191be2059dcb57fed86dd2cc62eb1a9fp-1,
    -0x1.b7525ac97e0d2p2
  },
  { // Entry 49
    0x1.191be2059dcb57fed86dd2cc62eb1a9fp-1,
    0x1.b7525ac97e0d2p2
  },
  { // Entry 50
    -0x1.f8305993a212c7fffffc15c2afa5e57ep-1,
    -0x1.bee5fa8a84b02p0
  },
  { // Entry 51
    0x1.f8305993a212c7fffffc15c2afa5e57ep-1,
    0x1.bee5fa8a84b02p0
  },
  { // Entry 52
    0x1.ff3b13530fd70e72b77f7c6721da1131p-1,
    -0x1.c393979fe5921p9
  },
  { // Entry 53
    -0x1.ff3b13530fd70e72b77f7c6721da1131p-1,
    0x1.c393979fe5921p9
  },
  { // Entry 54
    -0x1.f119da81a4da57ffd8bcc5a1247f5590p-1,
    -0x1.c48ffc72563c8p18
  },
  { // Entry 55
    0x1.f119da81a4da57ffd8bcc5a1247f5590p-1,
    0x1.c48ffc72563c8p18
  },
  { // Entry 56
    -0x1.fd73b81e04ccc7e9d764eca8bb085109p-1,
    -0x1.c79548bc31856p3
  },
  { // Entry 57
    0x1.fd73b81e04ccc7e9d764eca8bb085109p-1,
    0x1.c79548bc31856p3
  },
  { // Entry 58
    -0x1.c7885aef33a94ffc5ae06be9444efad5p-3,
    -0x1.cb6p-3
  },
  { // Entry 59
    0x1.c7885aef33a94ffc5ae06be9444efad5p-3,
    0x1.cb6p-3
  },
  { // Entry 60
    -0x1.e6494911eedd08d1650a4b5136c3e6d6p-7,
    -0x1.e64ddaf7bd72fp-7
  },
  { // Entry 61
    0x1.e6494911eedd08d1650a4b5136c3e6d6p-7,
    0x1.e64ddaf7bd72fp-7
  },
  { // Entry 62
    0x1.e180eef5b1c887fff585594949b46e6cp-1,
    -0x1.ecdd0fbf07942p5
  },
  { // Entry 63
    -0x1.e180eef5b1c887fff585594949b46e6cp-1,
    0x1.ecdd0fbf07942p5
  },
  { // Entry 64
    -0x1.fd98d20c1be4380000001ee51562d7a8p-1,
    -0x1.f073a23292337p2
  },
  { // Entry 65
    0x1.fd98d20c1be4380000001ee51562d7a8p-1,
    0x1.f073a23292337p2
  },
  { // Entry 66
    -0x1.7268c112297c87d17633bab722f31a3fp-5,
    -0x1.f5e4c410f4ef8p15
  },
  { // Entry 67
    0x1.7268c112297c87d17633bab722f31a3fp-5,
    0x1.f5e4c410f4ef8p15
  },
  { // Entry 68
    0x1.420796146070e628c19d38d1e90df228p-18,
    -0x1.f8000000002p95
  },
  { // Entry 69
    -0x1.420796146070e628c19d38d1e90df228p-18,
    0x1.f8000000002p95
  },
  { // Entry 70
    -0x1.e4f6dc499d9cc8035607fa3d76f9f4c5p-2,
    -0x1.f9365d79546e1p-2
  },
  { // Entry 71
    0x1.e4f6dc499d9cc8035607fa3d76f9f4c5p-2,
    0x1.f9365d79546e1p-2
  },
  { // Entry 72
    0x1.b2ef99b140d650468aed82fe0d442adap-14,
    -0x1.ffffffffffe7ep1023
  },
  { // Entry 73
    -0x1.b2ef99b140d650468aed82fe0d442adap-14,
    0x1.ffffffffffe7ep1023
  },
  { // Entry 74
    0x1.db0ffc3ecc6e3b8e37357ca002d5625bp-1,
    0x1.0p15
  },
  { // Entry 75
    -0x1.db0ffc3ecc6e3b8e37357ca002d5625bp-1,
    -0x1.0p15
  },
  { // Entry 76
    -0x1.e98f87098b62699ad16e0259b0e85b24p-1,
    0x1.0000000000001p13
  },
  { // Entry 77
    0x1.e98f87098b62699ad16e0259b0e85b24p-1,
    -0x1.0000000000001p13
  },
  { // Entry 78
    0x1.053c35068e10d45ee50f2d1ddcabcf3dp-4,
    0x1.0000000000001p52
  },
  { // Entry 79
    -0x1.053c35068e10d45ee50f2d1ddcabcf3dp-4,
    -0x1.0000000000001p52
  },
  { // Entry 80
    0x1.72d421b6884e500bbd3a9efc0f0ced92p-1,
    0x1.0000000000001p228
  },
  { // Entry 81
    -0x1.72d421b6884e500bbd3a9efc0f0ced92p-1,
    -0x1.0000000000001p228
  },
  { // Entry 82
    0x1.77fba987c56533a665e88e5a592ca10cp-1,
    0x1.0000000000001p491
  },
  { // Entry 83
    -0x1.77fba987c56533a665e88e5a592ca10cp-1,
    -0x1.0000000000001p491
  },
  { // Entry 84
    -0x1.723b2625331af2e79a9f07145a1e5731p-1,
    0x1.0000000000003p215
  },
  { // Entry 85
    0x1.723b2625331af2e79a9f07145a1e5731p-1,
    -0x1.0000000000003p215
  },
  { // Entry 86
    0x1.aed548f090cf47fe7e32103ab140caf8p-1,
    0x1.0000000000006p0
  },
  { // Entry 87
    -0x1.aed548f090cf47fe7e32103ab140caf8p-1,
    -0x1.0000000000006p0
  },
  { // Entry 88
    -0x1.ff983208c7dc978079174b461b3809f0p-1,
    0x1.0000000000007p8
  },
  { // Entry 89
    0x1.ff983208c7dc978079174b461b3809f0p-1,
    -0x1.0000000000007p8
  },
  { // Entry 90
    0x1.ffef29dc38452ffef4a779a1f86707dap-1,
    0x1.0000000000007p275
  },
  { // Entry 91
    -0x1.ffef29dc38452ffef4a779a1f86707dap-1,
    -0x1.0000000000007p275
  },
  { // Entry 92
    -0x1.fa88c375723c0d7ea1f2d08ebb2f9407p-8,
    0x1.0000000000007p449
  },
  { // Entry 93
    0x1.fa88c375723c0d7ea1f2d08ebb2f9407p-8,
    -0x1.0000000000007p449
  },
  { // Entry 94
    0x1.fff5322c94eae1b0b51086c08faa585cp-1,
    0x1.0000000000011p644
  },
  { // Entry 95
    -0x1.fff5322c94eae1b0b51086c08faa585cp-1,
    -0x1.0000000000011p644
  },
  { // Entry 96
    -0x1.a73630af8f15b8000054ddd950bba107p-1,
    0x1.000000000001fp164
  },
  { // Entry 97
    0x1.a73630af8f15b8000054ddd950bba107p-1,
    -0x1.000000000001fp164
  },
  { // Entry 98
    0x1.1c548f9249e4474988646b86894e9581p-2,
    0x1.0000000000038p380
  },
  { // Entry 99
    -0x1.1c548f9249e4474988646b86894e9581p-2,
    -0x1.0000000000038p380
  },
  { // Entry 100
    0x1.ca965bd2c4dfee4779ca8c797806292dp-3,
    0x1.0000000000118p380
  },
  { // Entry 101
    -0x1.ca965bd2c4dfee4779ca8c797806292dp-3,
    -0x1.0000000000118p380
  },
  { // Entry 102
    -0x1.837b9dddc24cf2f729a38147795b18d2p-1,
    0x1.000000000012cp2
  },
  { // Entry 103
    0x1.837b9dddc24cf2f729a38147795b18d2p-1,
    -0x1.000000000012cp2
  },
  { // Entry 104
    0x1.d82c1784c3ecbfefb02d7d6b585f73c4p-2,
    0x1.00000000001f8p700
  },
  { // Entry 105
    -0x1.d82c1784c3ecbfefb02d7d6b585f73c4p-2,
    -0x1.00000000001f8p700
  },
  { // Entry 106
    0x1.fffeaaaaef2ee7ee876f90b70b512475p-8,
    0x1.00000000002p-7
  },
  { // Entry 107
    -0x1.fffeaaaaef2ee7ee876f90b70b512475p-8,
    -0x1.00000000002p-7
  },
  { // Entry 108
    -0x1.0871bddd90fc5834034ee3f6f86b43a3p-1,
    0x1.00000000002p40
  },
  { // Entry 109
    0x1.0871bddd90fc5834034ee3f6f86b43a3p-1,
    -0x1.00000000002p40
  },
  { // Entry 110
    0x1.fffeaaaaef2f07ee476fa60c5dbe4886p-8,
    0x1.0000000000201p-7
  },
  { // Entry 111
    -0x1.fffeaaaaef2f07ee476fa60c5dbe4886p-8,
    -0x1.0000000000201p-7
  },
  { // Entry 112
    0x1.fffeaaaaef3307e6477250b6ab524ab7p-8,
    0x1.0000000000221p-7
  },
  { // Entry 113
    -0x1.fffeaaaaef3307e6477250b6ab524ab7p-8,
    -0x1.0000000000221p-7
  },
  { // Entry 114
    0x1.fffeaaaaef3627e00774660bb7d7a87cp-8,
    0x1.000000000023ap-7
  },
  { // Entry 115
    -0x1.fffeaaaaef3627e00774660bb7d7a87cp-8,
    -0x1.000000000023ap-7
  },
  { // Entry 116
    0x1.e0c6edfa9360159534def713925f126fp-9,
    0x1.00000000040p45
  },
  { // Entry 117
    -0x1.e0c6edfa9360159534def713925f126fp-9,
    -0x1.00000000040p45
  },
  { // Entry 118
    0x1.ea1f618356db0d3ab66d0ef6493bc93dp-5,
    0x1.0000000c0p40
  },
  { // Entry 119
    -0x1.ea1f618356db0d3ab66d0ef6493bc93dp-5,
    -0x1.0000000c0p40
  },
  { // Entry 120
    0x1.faaeed7587541fffff9922ba6690b8bap-3,
    0x1.00000013c86f4p-2
  },
  { // Entry 121
    -0x1.faaeed7587541fffff9922ba6690b8bap-3,
    -0x1.00000013c86f4p-2
  },
  { // Entry 122
    0x1.540bc7785680ac2ca169ff9e3cc4b152p-1,
    0x1.001p13
  },
  { // Entry 123
    -0x1.540bc7785680ac2ca169ff9e3cc4b152p-1,
    -0x1.001p13
  },
  { // Entry 124
    -0x1.37a7cb907a2e500106b04d9e3f0b1b8cp-1,
    0x1.003p699
  },
  { // Entry 125
    0x1.37a7cb907a2e500106b04d9e3f0b1b8cp-1,
    -0x1.003p699
  },
  { // Entry 126
    -0x1.29e5845fc54b580145273f2cafe73ab5p-1,
    0x1.00380p40
  },
  { // Entry 127
    0x1.29e5845fc54b580145273f2cafe73ab5p-1,
    -0x1.00380p40
  },
  { // Entry 128
    0x1.ffe5ca46564917a1e812aedd48fe107cp-1,
    0x1.007p10
  },
  { // Entry 129
    -0x1.ffe5ca46564917a1e812aedd48fe107cp-1,
    -0x1.007p10
  },
  { // Entry 130
    0x1.ea4df82db014ac0c913465da57b6a643p-1,
    0x1.007p25
  },
  { // Entry 131
    -0x1.ea4df82db014ac0c913465da57b6a643p-1,
    -0x1.007p25
  },
  { // Entry 132
    0x1.fe757aef1c80c7ec16003d3ae5f0ce15p-1,
    0x1.007p41
  },
  { // Entry 133
    -0x1.fe757aef1c80c7ec16003d3ae5f0ce15p-1,
    -0x1.007p41
  },
  { // Entry 134
    0x1.e9b71805ec0685d484f6270f6411dc36p-7,
    0x1.00cp41
  },
  { // Entry 135
    -0x1.e9b71805ec0685d484f6270f6411dc36p-7,
    -0x1.00cp41
  },
  { // Entry 136
    0x1.b0b6d0a54058280008e80488f8ab1bb7p-1,
    0x1.01c00000001p0
  },
  { // Entry 137
    -0x1.b0b6d0a54058280008e80488f8ab1bb7p-1,
    -0x1.01c00000001p0
  },
  { // Entry 138
    0x1.fef009262701280b92aaf925dda63bbap-3,
    0x1.02322e46da919p-2
  },
  { // Entry 139
    -0x1.fef009262701280b92aaf925dda63bbap-3,
    -0x1.02322e46da919p-2
  },
  { // Entry 140
    0x1.ffc90059804a082d9b3f46b77ee81932p-3,
    0x1.02a236478p-2
  },
  { // Entry 141
    -0x1.ffc90059804a082d9b3f46b77ee81932p-3,
    -0x1.02a236478p-2
  },
  { // Entry 142
    0x1.ffd10a6b5429e828bef4d9413eb9f6c0p-3,
    0x1.02a65d08ca5e5p-2
  },
  { // Entry 143
    -0x1.ffd10a6b5429e828bef4d9413eb9f6c0p-3,
    -0x1.02a65d08ca5e5p-2
  },
  { // Entry 144
    0x1.ffd10ab302a3f7e0a048db5861e7b20bp-3,
    0x1.02a65d2dce49ap-2
  },
  { // Entry 145
    -0x1.ffd10ab302a3f7e0a048db5861e7b20bp-3,
    -0x1.02a65d2dce49ap-2
  },
  { // Entry 146
    0x1.ffe0b1764ca4c7d2329433828cdc901cp-3,
    0x1.02ae7238ap-2
  },
  { // Entry 147
    -0x1.ffe0b1764ca4c7d2329433828cdc901cp-3,
    -0x1.02ae7238ap-2
  },
  { // Entry 148
    -0x1.f68f0e26c0f6ad44f7184d56be95e1aep-3,
    0x1.0501d22221dacp621
  },
  { // Entry 149
    0x1.f68f0e26c0f6ad44f7184d56be95e1aep-3,
    -0x1.0501d22221dacp621
  },
  { // Entry 150
    0x1.b63c41f09eb747f30c1af5f254340a4cp-1,
    0x1.06ffffffffff8p0
  },
  { // Entry 151
    -0x1.b63c41f09eb747f30c1af5f254340a4cp-1,
    -0x1.06ffffffffff8p0
  },
  { // Entry 152
    -0x1.ffdc173adabb1afd58fcb5643cd86562p-1,
    0x1.07023d3d44215p12
  },
  { // Entry 153
    0x1.ffdc173adabb1afd58fcb5643cd86562p-1,
    -0x1.07023d3d44215p12
  },
  { // Entry 154
    0x1.0889e11bef1357a88742ab3a7f180786p-5,
    0x1.0895a7a3e8ae6p-5
  },
  { // Entry 155
    -0x1.0889e11bef1357a88742ab3a7f180786p-5,
    -0x1.0895a7a3e8ae6p-5
  },
  { // Entry 156
    0x1.08ca077c764457aa6d968afc1d1b17b3p-5,
    0x1.08d5d69840601p-5
  },
  { // Entry 157
    -0x1.08ca077c764457aa6d968afc1d1b17b3p-5,
    -0x1.08d5d69840601p-5
  },
  { // Entry 158
    -0x1.ff7fbe518023e90b3f28f81ab93e872fp-1,
    0x1.0e0p6
  },
  { // Entry 159
    0x1.ff7fbe518023e90b3f28f81ab93e872fp-1,
    -0x1.0e0p6
  },
  { // Entry 160
    -0x1.fd6c68b877afe803aaab2bd30124ecdfp-1,
    0x1.107ba49c346e4p9
  },
  { // Entry 161
    0x1.fd6c68b877afe803aaab2bd30124ecdfp-1,
    -0x1.107ba49c346e4p9
  },
  { // Entry 162
    -0x1.a2ba6bc70bce4801ea4d1c85f116193dp-1,
    0x1.1491544774440p745
  },
  { // Entry 163
    0x1.a2ba6bc70bce4801ea4d1c85f116193dp-1,
    -0x1.1491544774440p745
  },
  { // Entry 164
    0x1.165609790f23484a4701b4439d4fe51fp-5,
    0x1.1663c0e518180p-5
  },
  { // Entry 165
    -0x1.165609790f23484a4701b4439d4fe51fp-5,
    -0x1.1663c0e518180p-5
  },
  { // Entry 166
    -0x1.fc0523ff94e44c01731e45963034cba8p-1,
    0x1.1745d1745d176p238
  },
  { // Entry 167
    0x1.fc0523ff94e44c01731e45963034cba8p-1,
    -0x1.1745d1745d176p238
  },
  { // Entry 168
    0x1.f34a729c584bcc9666402cfdb48839ffp-1,
    0x1.17472a408a3e0p97
  },
  { // Entry 169
    -0x1.f34a729c584bcc9666402cfdb48839ffp-1,
    -0x1.17472a408a3e0p97
  },
  { // Entry 170
    0x1.177fae169fdf0858b1d4e7bad69981dcp-5,
    0x1.178d91b6b992dp-5
  },
  { // Entry 171
    -0x1.177fae169fdf0858b1d4e7bad69981dcp-5,
    -0x1.178d91b6b992dp-5
  },
  { // Entry 172
    0x1.177fae16a120e85843d1dfef36726cecp-5,
    0x1.178d91b6bad4ep-5
  },
  { // Entry 173
    -0x1.177fae16a120e85843d1dfef36726cecp-5,
    -0x1.178d91b6bad4ep-5
  },
  { // Entry 174
    0x1.177fae16a1f78856637cc9da8aaf6439p-5,
    0x1.178d91b6bbabap-5
  },
  { // Entry 175
    -0x1.177fae16a1f78856637cc9da8aaf6439p-5,
    -0x1.178d91b6bbabap-5
  },
  { // Entry 176
    0x1.177fae16a40fe858da1ddcb583bb9f3bp-5,
    0x1.178d91b6bdc45p-5
  },
  { // Entry 177
    -0x1.177fae16a40fe858da1ddcb583bb9f3bp-5,
    -0x1.178d91b6bdc45p-5
  },
  { // Entry 178
    0x1.297c768f2413080002edcb9a905946afp-1,
    0x1.19752dbee5f6ap933
  },
  { // Entry 179
    -0x1.297c768f2413080002edcb9a905946afp-1,
    -0x1.19752dbee5f6ap933
  },
  { // Entry 180
    0x1.b826df5cafafa824779bb808d64de6fbp-2,
    0x1.1b3009cfe4dbcp8
  },
  { // Entry 181
    -0x1.b826df5cafafa824779bb808d64de6fbp-2,
    -0x1.1b3009cfe4dbcp8
  },
  { // Entry 182
    0x1.b7a5956250b6a83882133bbe14c847ccp-2,
    0x1.1f6475d95bf18p3
  },
  { // Entry 183
    -0x1.b7a5956250b6a83882133bbe14c847ccp-2,
    -0x1.1f6475d95bf18p3
  },
  { // Entry 184
    0x1.4db6566b645477ffea9faaf8bf463a1ep-1,
    0x1.229148a452291p118
  },
  { // Entry 185
    -0x1.4db6566b645477ffea9faaf8bf463a1ep-1,
    -0x1.229148a452291p118
  },
  { // Entry 186
    0x1.1686fee2c49a7fffb68df681b2da7e49p-1,
    0x1.268p-1
  },
  { // Entry 187
    -0x1.1686fee2c49a7fffb68df681b2da7e49p-1,
    -0x1.268p-1
  },
  { // Entry 188
    0x1.22eb21a44d62780000031ae43c448511p-2,
    0x1.26fb3844dd190p-2
  },
  { // Entry 189
    -0x1.22eb21a44d62780000031ae43c448511p-2,
    -0x1.26fb3844dd190p-2
  },
  { // Entry 190
    0x1.d4a216d89b2b37f40ff08753ed600e74p-1,
    0x1.27fffffffe6b0p0
  },
  { // Entry 191
    -0x1.d4a216d89b2b37f40ff08753ed600e74p-1,
    -0x1.27fffffffe6b0p0
  },
  { // Entry 192
    -0x1.76c9b0f3a22f73cfd4453fcf7b21e358p-1,
    0x1.284b84048d481p204
  },
  { // Entry 193
    0x1.76c9b0f3a22f73cfd4453fcf7b21e358p-1,
    -0x1.284b84048d481p204
  },
  { // Entry 194
    -0x1.ff01226f97d32d6b5c0d5e4a3a2f53c3p-1,
    0x1.2999e3109cad4p2
  },
  { // Entry 195
    0x1.ff01226f97d32d6b5c0d5e4a3a2f53c3p-1,
    -0x1.2999e3109cad4p2
  },
  { // Entry 196
    0x1.2a8f11e7ae82c0a374855b53b3605d3bp-5,
    0x1.2aap-5
  },
  { // Entry 197
    -0x1.2a8f11e7ae82c0a374855b53b3605d3bp-5,
    -0x1.2aap-5
  },
  { // Entry 198
    0x1.2b03d1bf773df7b697d8a97eccefec50p-5,
    0x1.2b14d3be0c230p-5
  },
  { // Entry 199
    -0x1.2b03d1bf773df7b697d8a97eccefec50p-5,
    -0x1.2b14d3be0c230p-5
  },
  { // Entry 200
    -0x1.ffb90ee641791e699aa7fc7139bf58b4p-1,
    0x1.2b7cb44849981p2
  },
  { // Entry 201
    0x1.ffb90ee641791e699aa7fc7139bf58b4p-1,
    -0x1.2b7cb44849981p2
  },
  { // Entry 202
    -0x1.ffffff79e71a3db22109c6bb87f9c237p-1,
    0x1.2becc86852580p200
  },
  { // Entry 203
    0x1.ffffff79e71a3db22109c6bb87f9c237p-1,
    -0x1.2becc86852580p200
  },
  { // Entry 204
    -0x1.fff9edaf85b76f6cbb2f5b7e384cbbc9p-1,
    0x1.2cfa14ce27cd5p2
  },
  { // Entry 205
    0x1.fff9edaf85b76f6cbb2f5b7e384cbbc9p-1,
    -0x1.2cfa14ce27cd5p2
  },
  { // Entry 206
    0x1.2cbaaa4cebb517fe7bd14f062afec390p-4,
    0x1.2d0p-4
  },
  { // Entry 207
    -0x1.2cbaaa4cebb517fe7bd14f062afec390p-4,
    -0x1.2d0p-4
  },
  { // Entry 208
    -0x1.ffffbc177e00ff89cbb4f49a0643da78p-1,
    0x1.2d76d18721be8p2
  },
  { // Entry 209
    0x1.ffffbc177e00ff89cbb4f49a0643da78p-1,
    -0x1.2d76d18721be8p2
  },
  { // Entry 210
    0x1.745843dfafefd1bf9656896a8fb5b0e3p-18,
    0x1.302a494e09090p97
  },
  { // Entry 211
    -0x1.745843dfafefd1bf9656896a8fb5b0e3p-18,
    -0x1.302a494e09090p97
  },
  { // Entry 212
    0x1.ffcc568d423766c92e9c175f9210f1f4p-1,
    0x1.31cc731cc731cp1000
  },
  { // Entry 213
    -0x1.ffcc568d423766c92e9c175f9210f1f4p-1,
    -0x1.31cc731cc731cp1000
  },
  { // Entry 214
    0x1.b676077d4faf7927da1f858539fad1ffp-1,
    0x1.328463d4f8ca6p441
  },
  { // Entry 215
    -0x1.b676077d4faf7927da1f858539fad1ffp-1,
    -0x1.328463d4f8ca6p441
  },
  { // Entry 216
    -0x1.fffffffffffffffff3471d8dc070d5e6p-1,
    0x1.32ce90b32171ep18
  },
  { // Entry 217
    0x1.fffffffffffffffff3471d8dc070d5e6p-1,
    -0x1.32ce90b32171ep18
  },
  { // Entry 218
    0x1.35cbd3240d148845d39ebd54b1556779p-5,
    0x1.35debd7f020ecp-5
  },
  { // Entry 219
    -0x1.35cbd3240d148845d39ebd54b1556779p-5,
    -0x1.35debd7f020ecp-5
  },
  { // Entry 220
    0x1.3bb2086559fa982e5c6da10d8d9ac931p-7,
    0x1.3bb3487893405p-7
  },
  { // Entry 221
    -0x1.3bb2086559fa982e5c6da10d8d9ac931p-7,
    -0x1.3bb3487893405p-7
  },
  { // Entry 222
    0x1.3bb2086559fab82dfb1925eac2ab8bc1p-7,
    0x1.3bb3487893407p-7
  },
  { // Entry 223
    -0x1.3bb2086559fab82dfb1925eac2ab8bc1p-7,
    -0x1.3bb3487893407p-7
  },
  { // Entry 224
    0x1.dff197edc51d235e4a4cc6e34bd1fd10p-16,
    0x1.3bb681d65aa60p100
  },
  { // Entry 225
    -0x1.dff197edc51d235e4a4cc6e34bd1fd10p-16,
    -0x1.3bb681d65aa60p100
  },
  { // Entry 226
    -0x1.5d08d3dbb41af80000053ba30e95e709p-3,
    0x1.3f9aa8626042fp83
  },
  { // Entry 227
    0x1.5d08d3dbb41af80000053ba30e95e709p-3,
    -0x1.3f9aa8626042fp83
  },
  { // Entry 228
    0x1.fb503983f94bb749fce9ccca56a35ec6p-3,
    0x1.3fep19
  },
  { // Entry 229
    -0x1.fb503983f94bb749fce9ccca56a35ec6p-3,
    -0x1.3fep19
  },
  { // Entry 230
    -0x1.d3876eacc9ee6ec5a93ea14e032f9b02p-1,
    0x1.4285478f1e3c8p58
  },
  { // Entry 231
    0x1.d3876eacc9ee6ec5a93ea14e032f9b02p-1,
    -0x1.4285478f1e3c8p58
  },
  { // Entry 232
    0x1.42b66d54f69c0fabe371d703a1d7d7c9p-5,
    0x1.42cbcf45a169ep-5
  },
  { // Entry 233
    -0x1.42b66d54f69c0fabe371d703a1d7d7c9p-5,
    -0x1.42cbcf45a169ep-5
  },
  { // Entry 234
    0x1.b45e9e942755380000015e1db0ce188dp-1,
    0x1.43fffffffff6ap557
  },
  { // Entry 235
    -0x1.b45e9e942755380000015e1db0ce188dp-1,
    -0x1.43fffffffff6ap557
  },
  { // Entry 236
    0x1.43ffffffea602800006ed7862b3224a6p-17,
    0x1.440p-17
  },
  { // Entry 237
    -0x1.43ffffffea602800006ed7862b3224a6p-17,
    -0x1.440p-17
  },
  { // Entry 238
    -0x1.6a4e98d2d8b1b7fb43fef1bc4e3245bbp-1,
    0x1.4748c08dc0976p200
  },
  { // Entry 239
    0x1.6a4e98d2d8b1b7fb43fef1bc4e3245bbp-1,
    -0x1.4748c08dc0976p200
  },
  { // Entry 240
    -0x1.b57ca8aacf2a937a269ccdfa8b38c8c1p-1,
    0x1.478fc08p43
  },
  { // Entry 241
    0x1.b57ca8aacf2a937a269ccdfa8b38c8c1p-1,
    -0x1.478fc08p43
  },
  { // Entry 242
    0x1.ffe38008ef6b4956b290202d414d59c3p-1,
    0x1.4cf36d17c596ep200
  },
  { // Entry 243
    -0x1.ffe38008ef6b4956b290202d414d59c3p-1,
    -0x1.4cf36d17c596ep200
  },
  { // Entry 244
    0x1.d6457a3f12e6c527a853470401d35d2dp-1,
    0x1.4f0f308p488
  },
  { // Entry 245
    -0x1.d6457a3f12e6c527a853470401d35d2dp-1,
    -0x1.4f0f308p488
  },
  { // Entry 246
    0x1.4fffffffff9f88000000084f22ccccccp-20,
    0x1.5p-20
  },
  { // Entry 247
    -0x1.4fffffffff9f88000000084f22ccccccp-20,
    -0x1.5p-20
  },
  { // Entry 248
    -0x1.cbad095f503a18019a7b27170f6a4acfp-1,
    0x1.5143e25a488f1p3
  },
  { // Entry 249
    0x1.cbad095f503a18019a7b27170f6a4acfp-1,
    -0x1.5143e25a488f1p3
  },
  { // Entry 250
    -0x1.f942d6262e82dd6834ded8c90d0f8212p-5,
    0x1.51f0f44da4df4p200
  },
  { // Entry 251
    0x1.f942d6262e82dd6834ded8c90d0f8212p-5,
    -0x1.51f0f44da4df4p200
  },
  { // Entry 252
    -0x1.fc466ccaece804aac14e79aaba4b5c1ep-3,
    0x1.52ad6c5a3602fp16
  },
  { // Entry 253
    0x1.fc466ccaece804aac14e79aaba4b5c1ep-3,
    -0x1.52ad6c5a3602fp16
  },
  { // Entry 254
    0x1.d69c3cf4eecdd9b43726a2a7fa405c31p-1,
    0x1.52f00e0p793
  },
  { // Entry 255
    -0x1.d69c3cf4eecdd9b43726a2a7fa405c31p-1,
    -0x1.52f00e0p793
  },
  { // Entry 256
    0x1.e120292f3d4956579f9c3164cd94bad6p-1,
    0x1.5555555555556p239
  },
  { // Entry 257
    -0x1.e120292f3d4956579f9c3164cd94bad6p-1,
    -0x1.5555555555556p239
  },
  { // Entry 258
    -0x1.fd1d85b7ef0037fc4b1d1763882085f1p-1,
    0x1.5a0000008p6
  },
  { // Entry 259
    0x1.fd1d85b7ef0037fc4b1d1763882085f1p-1,
    -0x1.5a0000008p6
  },
  { // Entry 260
    0x1.5aff9664b07e1a2e0f24b2572ddbff64p-6,
    0x1.5b063ad2dd08fp-6
  },
  { // Entry 261
    -0x1.5aff9664b07e1a2e0f24b2572ddbff64p-6,
    -0x1.5b063ad2dd08fp-6
  },
  { // Entry 262
    -0x1.83f8bbb59f2f8cd8ebbd44c459e4c493p-1,
    0x1.5b179d75fa285p2
  },
  { // Entry 263
    0x1.83f8bbb59f2f8cd8ebbd44c459e4c493p-1,
    -0x1.5b179d75fa285p2
  },
  { // Entry 264
    0x1.fa865b0d994968390f9480c9caf49cb1p-1,
    0x1.5bb5967402f9cp79
  },
  { // Entry 265
    -0x1.fa865b0d994968390f9480c9caf49cb1p-1,
    -0x1.5bb5967402f9cp79
  },
  { // Entry 266
    0x1.e8a523fce884d189bdb87c20ed615944p-2,
    0x1.5bea010p468
  },
  { // Entry 267
    -0x1.e8a523fce884d189bdb87c20ed615944p-2,
    -0x1.5bea010p468
  },
  { // Entry 268
    -0x1.ff2ad941f0a40ea3bc29e6dd9c339b63p-1,
    0x1.5f19fbc507af6p9
  },
  { // Entry 269
    0x1.ff2ad941f0a40ea3bc29e6dd9c339b63p-1,
    -0x1.5f19fbc507af6p9
  },
  { // Entry 270
    -0x1.75ce4a0d0bd037fd53042cab82404953p-1,
    0x1.60a610a658da9p889
  },
  { // Entry 271
    0x1.75ce4a0d0bd037fd53042cab82404953p-1,
    -0x1.60a610a658da9p889
  },
  { // Entry 272
    -0x1.721586594ab4818acd4cb41fafc99484p-1,
    0x1.62ad7ce17143dp62
  },
  { // Entry 273
    0x1.721586594ab4818acd4cb41fafc99484p-1,
    -0x1.62ad7ce17143dp62
  },
  { // Entry 274
    0x1.b8d27019d1b9e836b953613c87acf3a9p-2,
    0x1.645926cc1132cp9
  },
  { // Entry 275
    -0x1.b8d27019d1b9e836b953613c87acf3a9p-2,
    -0x1.645926cc1132cp9
  },
  { // Entry 276
    0x1.647e09059c1e98000013714cd0c6a43dp-9,
    0x1.647e25d391f17p-9
  },
  { // Entry 277
    -0x1.647e09059c1e98000013714cd0c6a43dp-9,
    -0x1.647e25d391f17p-9
  },
  { // Entry 278
    -0x1.8d3b53ff85a823d06537413901229e45p-1,
    0x1.64ef438p142
  },
  { // Entry 279
    0x1.8d3b53ff85a823d06537413901229e45p-1,
    -0x1.64ef438p142
  },
  { // Entry 280
    -0x1.f7c8630e62a01ffd4e02577956e5523ep-1,
    0x1.6599665996658p3
  },
  { // Entry 281
    0x1.f7c8630e62a01ffd4e02577956e5523ep-1,
    -0x1.6599665996658p3
  },
  { // Entry 282
    0x1.67028e3602034800f45a9158d20a340cp-5,
    0x1.672p-5
  },
  { // Entry 283
    -0x1.67028e3602034800f45a9158d20a340cp-5,
    -0x1.672p-5
  },
  { // Entry 284
    0x1.bc60c8c33cb5ebf693d71650cbf166f9p-2,
    0x1.688ae6c138ea8p299
  },
  { // Entry 285
    -0x1.bc60c8c33cb5ebf693d71650cbf166f9p-2,
    -0x1.688ae6c138ea8p299
  },
  { // Entry 286
    -0x1.fc3b4bb8b012e8042d3ebced12def0c9p-1,
    0x1.6aa78p17
  },
  { // Entry 287
    0x1.fc3b4bb8b012e8042d3ebced12def0c9p-1,
    -0x1.6aa78p17
  },
  { // Entry 288
    0x1.ffffffffffffffffffffffffffffffb5p-1,
    0x1.6ac5b262ca1ffp849
  },
  { // Entry 289
    -0x1.ffffffffffffffffffffffffffffffb5p-1,
    -0x1.6ac5b262ca1ffp849
  },
  { // Entry 290
    -0x1.82317836a97c8cea1cd2e3a4d0d48f29p-1,
    0x1.6d88083749412p4
  },
  { // Entry 291
    0x1.82317836a97c8cea1cd2e3a4d0d48f29p-1,
    -0x1.6d88083749412p4
  },
  { // Entry 292
    0x1.6f781c78cc82a7ffcd20ba801e6691b1p-6,
    0x1.6f8p-6
  },
  { // Entry 293
    -0x1.6f781c78cc82a7ffcd20ba801e6691b1p-6,
    -0x1.6f8p-6
  },
  { // Entry 294
    -0x1.fdbe5085494ae7fe47fa067aaea7fdf9p-1,
    0x1.729aa6859d1f4p396
  },
  { // Entry 295
    0x1.fdbe5085494ae7fe47fa067aaea7fdf9p-1,
    -0x1.729aa6859d1f4p396
  },
  { // Entry 296
    -0x1.fffffae862b4fec1c2f1712aa165807cp-1,
    0x1.73e2dbe9a2f80p10
  },
  { // Entry 297
    0x1.fffffae862b4fec1c2f1712aa165807cp-1,
    -0x1.73e2dbe9a2f80p10
  },
  { // Entry 298
    0x1.769ac74459b058456b333238833f1d1ap-7,
    0x1.769cde0b90b80p-7
  },
  { // Entry 299
    -0x1.769ac74459b058456b333238833f1d1ap-7,
    -0x1.769cde0b90b80p-7
  },
  { // Entry 300
    0x1.769e8afb6a4ebf99ade465d19be86ac0p-5,
    0x1.76cp-5
  },
  { // Entry 301
    -0x1.769e8afb6a4ebf99ade465d19be86ac0p-5,
    -0x1.76cp-5
  },
  { // Entry 302
    0x1.fd562611f5bd4800de503e4210cc7df4p-1,
    0x1.78001p0
  },
  { // Entry 303
    -0x1.fd562611f5bd4800de503e4210cc7df4p-1,
    -0x1.78001p0
  },
  { // Entry 304
    0x1.fdba784ca00f17ebcc2b5b1098e83bffp-1,
    0x1.7a0p0
  },
  { // Entry 305
    -0x1.fdba784ca00f17ebcc2b5b1098e83bffp-1,
    -0x1.7a0p0
  },
  { // Entry 306
    0x1.f930c222a8682d7d1920a12b68870c36p-5,
    0x1.7abd870381c2dp38
  },
  { // Entry 307
    -0x1.f930c222a8682d7d1920a12b68870c36p-5,
    -0x1.7abd870381c2dp38
  },
  { // Entry 308
    0x1.ffeb2ff2b692367bc5660dab6960b57dp-1,
    0x1.7dc945c212480p95
  },
  { // Entry 309
    -0x1.ffeb2ff2b692367bc5660dab6960b57dp-1,
    -0x1.7dc945c212480p95
  },
  { // Entry 310
    0x1.b279153c23fb180000ce3a0d47d8c476p-2,
    0x1.7f73e1594b70cp98
  },
  { // Entry 311
    -0x1.b279153c23fb180000ce3a0d47d8c476p-2,
    -0x1.7f73e1594b70cp98
  },
  { // Entry 312
    -0x1.599fad35cf60ad43760cf3a9da3d9c30p-41,
    0x1.7f7ef77e83f1ap21
  },
  { // Entry 313
    0x1.599fad35cf60ad43760cf3a9da3d9c30p-41,
    -0x1.7f7ef77e83f1ap21
  },
  { // Entry 314
    0x1.feb7a9b2c6d8ade6e17fdacaef4fd96cp-1,
    0x1.8p0
  },
  { // Entry 315
    -0x1.feb7a9b2c6d8ade6e17fdacaef4fd96cp-1,
    -0x1.8p0
  },
  { // Entry 316
    0x1.f798d01ec615b8ededa474e3c6cd0c92p-1,
    0x1.8p6
  },
  { // Entry 317
    -0x1.f798d01ec615b8ededa474e3c6cd0c92p-1,
    -0x1.8p6
  },
  { // Entry 318
    0x1.fee1a2a977bced179f8ab8455ada525fp-1,
    0x1.8132ceb1c4f39p0
  },
  { // Entry 319
    -0x1.fee1a2a977bced179f8ab8455ada525fp-1,
    -0x1.8132ceb1c4f39p0
  },
  { // Entry 320
    -0x1.24245af4cd994e9b3bba992d0f57fc3fp-52,
    0x1.81ae0dffa3b33p959
  },
  { // Entry 321
    0x1.24245af4cd994e9b3bba992d0f57fc3fp-52,
    -0x1.81ae0dffa3b33p959
  },
  { // Entry 322
    0x1.85d41b0bf30907fffff1913a5ced7645p-4,
    0x1.85ec5a399a2e6p1
  },
  { // Entry 323
    -0x1.85d41b0bf30907fffff1913a5ced7645p-4,
    -0x1.85ec5a399a2e6p1
  },
  { // Entry 324
    0x1.1e42ae3cfbdc60976884087f97a0fda9p-24,
    0x1.86a0092754022p16
  },
  { // Entry 325
    -0x1.1e42ae3cfbdc60976884087f97a0fda9p-24,
    -0x1.86a0092754022p16
  },
  { // Entry 326
    -0x1.dbf4e594cefe1548efc24098d631e950p-1,
    0x1.8720588p392
  },
  { // Entry 327
    0x1.dbf4e594cefe1548efc24098d631e950p-1,
    -0x1.8720588p392
  },
  { // Entry 328
    0x1.44302d6a82d403badf1f9b71420ccdcap-9,
    0x1.8929354ebc6aap43
  },
  { // Entry 329
    -0x1.44302d6a82d403badf1f9b71420ccdcap-9,
    -0x1.8929354ebc6aap43
  },
  { // Entry 330
    0x1.8a52189ec34877bb5944413e00ad52edp-5,
    0x1.8a791e4791e75p-5
  },
  { // Entry 331
    -0x1.8a52189ec34877bb5944413e00ad52edp-5,
    -0x1.8a791e4791e75p-5
  },
  { // Entry 332
    -0x1.fe8566e538122db33c17f13b83474a04p-1,
    0x1.8ba761438f5edp11
  },
  { // Entry 333
    0x1.fe8566e538122db33c17f13b83474a04p-1,
    -0x1.8ba761438f5edp11
  },
  { // Entry 334
    0x1.fff42aca4cb5a80000057f1a48af65dep-1,
    0x1.8eaf16de63920p0
  },
  { // Entry 335
    -0x1.fff42aca4cb5a80000057f1a48af65dep-1,
    -0x1.8eaf16de63920p0
  },
  { // Entry 336
    0x1.fffb7d3f3a252da56d96327c18627f57p-1,
    0x1.9p0
  },
  { // Entry 337
    -0x1.fffb7d3f3a252da56d96327c18627f57p-1,
    -0x1.9p0
  },
  { // Entry 338
    -0x1.e815770667fd87f9525154edc867605fp-4,
    0x1.91a5657fb6a9ap6
  },
  { // Entry 339
    0x1.e815770667fd87f9525154edc867605fp-4,
    -0x1.91a5657fb6a9ap6
  },
  { // Entry 340
    -0x1.fffffffd311dc7ec5d0ae5728a7960fdp-1,
    0x1.921fb54468847p37
  },
  { // Entry 341
    0x1.fffffffd311dc7ec5d0ae5728a7960fdp-1,
    -0x1.921fb54468847p37
  },
  { // Entry 342
    0x1.ffffffff875e5e4497dd0760336e63f3p-17,
    0x1.921ff54442d18p2
  },
  { // Entry 343
    -0x1.ffffffff875e5e4497dd0760336e63f3p-17,
    -0x1.921ff54442d18p2
  },
  { // Entry 344
    0x1.812a5da3777cd7f6e17288638b40aa09p-8,
    0x1.928p2
  },
  { // Entry 345
    -0x1.812a5da3777cd7f6e17288638b40aa09p-8,
    -0x1.928p2
  },
  { // Entry 346
    0x1.fff9be8d82572c746f944934bbf1b697p-1,
    0x1.94ap0
  },
  { // Entry 347
    -0x1.fff9be8d82572c746f944934bbf1b697p-1,
    -0x1.94ap0
  },
  { // Entry 348
    0x1.947b0ace235f2850b27b164c49102a49p-5,
    0x1.94a5294a51bdep-5
  },
  { // Entry 349
    -0x1.947b0ace235f2850b27b164c49102a49p-5,
    -0x1.94a5294a51bdep-5
  },
  { // Entry 350
    0x1.c34f70e55a707bbd053a70e749105174p-2,
    0x1.94a5294a52948p100
  },
  { // Entry 351
    -0x1.c34f70e55a707bbd053a70e749105174p-2,
    -0x1.94a5294a52948p100
  },
  { // Entry 352
    0x1.950bcfc0f3d50850941a41d66cfa3721p-5,
    0x1.95361b8f7697dp-5
  },
  { // Entry 353
    -0x1.950bcfc0f3d50850941a41d66cfa3721p-5,
    -0x1.95361b8f7697dp-5
  },
  { // Entry 354
    0x1.6c548bfcce6953f19a8d2dd480d9320cp-1,
    0x1.956p-1
  },
  { // Entry 355
    -0x1.6c548bfcce6953f19a8d2dd480d9320cp-1,
    -0x1.956p-1
  },
  { // Entry 356
    0x1.ffeffdbf67ca6711a57b7d82140093c6p-1,
    0x1.962p0
  },
  { // Entry 357
    -0x1.ffeffdbf67ca6711a57b7d82140093c6p-1,
    -0x1.962p0
  },
  { // Entry 358
    0x1.9708213bf67f4857135077d50826f588p-5,
    0x1.97330d2ea16d9p-5
  },
  { // Entry 359
    -0x1.9708213bf67f4857135077d50826f588p-5,
    -0x1.97330d2ea16d9p-5
  },
  { // Entry 360
    0x1.972bf92713d50858e64c1b9b26aeecd0p-5,
    0x1.9756f073b6b61p-5
  },
  { // Entry 361
    -0x1.972bf92713d50858e64c1b9b26aeecd0p-5,
    -0x1.9756f073b6b61p-5
  },
  { // Entry 362
    0x1.976845ebe71188580e468bad00ce23ecp-5,
    0x1.97935055cec1bp-5
  },
  { // Entry 363
    -0x1.976845ebe71188580e468bad00ce23ecp-5,
    -0x1.97935055cec1bp-5
  },
  { // Entry 364
    0x1.97535cee51a42806e15f6f878524e5b7p-4,
    0x1.980p-4
  },
  { // Entry 365
    -0x1.97535cee51a42806e15f6f878524e5b7p-4,
    -0x1.980p-4
  },
  { // Entry 366
    0x1.6f494c3356177000006502cdff975b08p-1,
    0x1.999999a42160cp-1
  },
  { // Entry 367
    -0x1.6f494c3356177000006502cdff975b08p-1,
    -0x1.999999a42160cp-1
  },
  { // Entry 368
    0x1.6f494c37edd6d800006dc76aee4cc8e4p-1,
    0x1.999999aab8f50p-1
  },
  { // Entry 369
    -0x1.6f494c37edd6d800006dc76aee4cc8e4p-1,
    -0x1.999999aab8f50p-1
  },
  { // Entry 370
    0x1.6fa912bdeaab280000164f16dd649356p-1,
    0x1.9a2324b9c6326p-1
  },
  { // Entry 371
    -0x1.6fa912bdeaab280000164f16dd649356p-1,
    -0x1.9a2324b9c6326p-1
  },
  { // Entry 372
    0x1.70c7ef4ef9b347ed234ca6131be1b786p-1,
    0x1.9bcp-1
  },
  { // Entry 373
    -0x1.70c7ef4ef9b347ed234ca6131be1b786p-1,
    -0x1.9bcp-1
  },
  { // Entry 374
    0x1.ff28176ad3163ea168c9991f155f875bp-1,
    0x1.a0d1d817d6c4ap0
  },
  { // Entry 375
    -0x1.ff28176ad3163ea168c9991f155f875bp-1,
    -0x1.a0d1d817d6c4ap0
  },
  { // Entry 376
    0x1.749468a7248dd26161e93d78928c66dep-1,
    0x1.a141c9de12fdfp-1
  },
  { // Entry 377
    -0x1.749468a7248dd26161e93d78928c66dep-1,
    -0x1.a141c9de12fdfp-1
  },
  { // Entry 378
    0x1.754ebb7e73f452ab49c615f0e8300d5cp-1,
    0x1.a251bc6766f20p-1
  },
  { // Entry 379
    -0x1.754ebb7e73f452ab49c615f0e8300d5cp-1,
    -0x1.a251bc6766f20p-1
  },
  { // Entry 380
    -0x1.7c3bfefa74bd07fe21336caac372a73ep-1,
    0x1.a2689ae1b86ddp62
  },
  { // Entry 381
    0x1.7c3bfefa74bd07fe21336caac372a73ep-1,
    -0x1.a2689ae1b86ddp62
  },
  { // Entry 382
    -0x1.ffff4f3648e02f9f494f42eb822f262bp-1,
    0x1.a3f66180c4550p100
  },
  { // Entry 383
    0x1.ffff4f3648e02f9f494f42eb822f262bp-1,
    -0x1.a3f66180c4550p100
  },
  { // Entry 384
    -0x1.1cf463983c0e28023161b7d0d2938e37p-3,
    0x1.a3fdd2a5286c3p1
  },
  { // Entry 385
    0x1.1cf463983c0e28023161b7d0d2938e37p-3,
    -0x1.a3fdd2a5286c3p1
  },
  { // Entry 386
    0x1.feb7948d224d7e759a7c9dd768ded727p-1,
    0x1.a44p0
  },
  { // Entry 387
    -0x1.feb7948d224d7e759a7c9dd768ded727p-1,
    -0x1.a44p0
  },
  { // Entry 388
    0x1.78801e3e1166482dce6208e98d3cc142p-1,
    0x1.a701ef3c7d54bp-1
  },
  { // Entry 389
    -0x1.78801e3e1166482dce6208e98d3cc142p-1,
    -0x1.a701ef3c7d54bp-1
  },
  { // Entry 390
    -0x1.fff11e871d59c3e1333db5475bb57e27p-1,
    0x1.a8c01fd43c0p537
  },
  { // Entry 391
    0x1.fff11e871d59c3e1333db5475bb57e27p-1,
    -0x1.a8c01fd43c0p537
  },
  { // Entry 392
    0x1.fdfa4366eb7337e6d973c4cb8f1a244ap-1,
    0x1.a8e29b7602f3bp0
  },
  { // Entry 393
    -0x1.fdfa4366eb7337e6d973c4cb8f1a244ap-1,
    -0x1.a8e29b7602f3bp0
  },
  { // Entry 394
    0x1.fde98b94e7947a04229d26eeb2bb4280p-1,
    0x1.a94p0
  },
  { // Entry 395
    -0x1.fde98b94e7947a04229d26eeb2bb4280p-1,
    -0x1.a94p0
  },
  { // Entry 396
    0x1.7931cba10000880000068f94098018a7p-2,
    0x1.aa445fce93b82p2
  },
  { // Entry 397
    -0x1.7931cba10000880000068f94098018a7p-2,
    -0x1.aa445fce93b82p2
  },
  { // Entry 398
    0x1.7af3f76c7a708834b4072f59ac71a0bap-1,
    0x1.aaa3fbc359fbep-1
  },
  { // Entry 399
    -0x1.7af3f76c7a708834b4072f59ac71a0bap-1,
    -0x1.aaa3fbc359fbep-1
  },
  { // Entry 400
    0x1.fd74e53ae32fcd8467bcf50da1d0f563p-6,
    0x1.abdd3dbd4d860p119
  },
  { // Entry 401
    -0x1.fd74e53ae32fcd8467bcf50da1d0f563p-6,
    -0x1.abdd3dbd4d860p119
  },
  { // Entry 402
    0x1.7d4a7bf183a3329ed434c0c4dd78b1d9p-1,
    0x1.ae2165a0c9f8ep-1
  },
  { // Entry 403
    -0x1.7d4a7bf183a3329ed434c0c4dd78b1d9p-1,
    -0x1.ae2165a0c9f8ep-1
  },
  { // Entry 404
    0x1.b81410edc79e0837507a3eef8d553bd6p-2,
    0x1.ae8dfefcfe13bp2
  },
  { // Entry 405
    -0x1.b81410edc79e0837507a3eef8d553bd6p-2,
    -0x1.ae8dfefcfe13bp2
  },
  { // Entry 406
    -0x1.ff751561dc5097fefe499301d034a0edp-2,
    0x1.b5597f950ee8cp29
  },
  { // Entry 407
    0x1.ff751561dc5097fefe499301d034a0edp-2,
    -0x1.b5597f950ee8cp29
  },
  { // Entry 408
    0x1.027d184afb1984ca1d21b1ac928d5391p-52,
    0x1.bab62ed655019p970
  },
  { // Entry 409
    -0x1.027d184afb1984ca1d21b1ac928d5391p-52,
    -0x1.bab62ed655019p970
  },
  { // Entry 410
    0x1.bc572e5e413e17fcb2246d40249a005fp-10,
    0x1.bc573c4ffffffp-10
  },
  { // Entry 411
    -0x1.bc572e5e413e17fcb2246d40249a005fp-10,
    -0x1.bc573c4ffffffp-10
  },
  { // Entry 412
    0x1.fb300f1e39afe80471f993abb8cfa428p-1,
    0x1.bef5cd25ab1adp9
  },
  { // Entry 413
    -0x1.fb300f1e39afe80471f993abb8cfa428p-1,
    -0x1.bef5cd25ab1adp9
  },
  { // Entry 414
    0x1.b1baaf622d3a27fd27d0716f7d08d992p-2,
    0x1.bfdf6df2a24c1p-2
  },
  { // Entry 415
    -0x1.b1baaf622d3a27fd27d0716f7d08d992p-2,
    -0x1.bfdf6df2a24c1p-2
  },
  { // Entry 416
    0x1.88fb762c35ce37fecbb4748d01feffcep-1,
    0x1.bfffffdffffffp-1
  },
  { // Entry 417
    -0x1.88fb762c35ce37fecbb4748d01feffcep-1,
    -0x1.bfffffdffffffp-1
  },
  { // Entry 418
    0x1.fe0ebff99ab8c80ce5939f1f072c2e91p-1,
    0x1.c2b489520e376p920
  },
  { // Entry 419
    -0x1.fe0ebff99ab8c80ce5939f1f072c2e91p-1,
    -0x1.c2b489520e376p920
  },
  { // Entry 420
    0x1.cf7f749f2a8357f7feab685ff6f8b624p-4,
    0x1.c54beb0085470p5
  },
  { // Entry 421
    -0x1.cf7f749f2a8357f7feab685ff6f8b624p-4,
    -0x1.c54beb0085470p5
  },
  { // Entry 422
    0x1.b6facf665891482ea8c61f5ca32f280dp-2,
    0x1.c5ad34f5f472ap-2
  },
  { // Entry 423
    -0x1.b6facf665891482ea8c61f5ca32f280dp-2,
    -0x1.c5ad34f5f472ap-2
  },
  { // Entry 424
    0x1.b851cd9b84ee683dc6bd3899ea81dbf9p-2,
    0x1.c728fc2f34bd6p-2
  },
  { // Entry 425
    -0x1.b851cd9b84ee683dc6bd3899ea81dbf9p-2,
    -0x1.c728fc2f34bd6p-2
  },
  { // Entry 426
    0x1.ba21b53cf2ff2832606f65eeeb4e3aafp-2,
    0x1.c92b0f6105089p-2
  },
  { // Entry 427
    -0x1.ba21b53cf2ff2832606f65eeeb4e3aafp-2,
    -0x1.c92b0f6105089p-2
  },
  { // Entry 428
    0x1.c9a2b68e30ec77c4e76c889704394a50p-5,
    0x1.c9dfbbe9ec704p-5
  },
  { // Entry 429
    -0x1.c9a2b68e30ec77c4e76c889704394a50p-5,
    -0x1.c9dfbbe9ec704p-5
  },
  { // Entry 430
    0x1.f370115c9ab35743e12217c6122e1a28p-1,
    0x1.caf31bd7ee217p0
  },
  { // Entry 431
    -0x1.f370115c9ab35743e12217c6122e1a28p-1,
    -0x1.caf31bd7ee217p0
  },
  { // Entry 432
    -0x1.dd38a1f1d289b6173115721bc58e298dp-54,
    0x1.cb44e86bc192bp648
  },
  { // Entry 433
    0x1.dd38a1f1d289b6173115721bc58e298dp-54,
    -0x1.cb44e86bc192bp648
  },
  { // Entry 434
    0x1.dd38a1f1d289b6173115721bc55a56a8p-53,
    0x1.cb44e86bc192bp649
  },
  { // Entry 435
    -0x1.dd38a1f1d289b6173115721bc55a56a8p-53,
    -0x1.cb44e86bc192bp649
  },
  { // Entry 436
    0x1.c7885aef33a94ffc5ae06be9444efad5p-3,
    0x1.cb6p-3
  },
  { // Entry 437
    -0x1.c7885aef33a94ffc5ae06be9444efad5p-3,
    -0x1.cb6p-3
  },
  { // Entry 438
    0x1.cd279aa6196b57f9be71cdffdd9f1919p-4,
    0x1.ce2271d2f662fp-4
  },
  { // Entry 439
    -0x1.cd279aa6196b57f9be71cdffdd9f1919p-4,
    -0x1.ce2271d2f662fp-4
  },
  { // Entry 440
    0x1.930b705f9fad17fb56813a45973a8d72p-1,
    0x1.d0000000004p-1
  },
  { // Entry 441
    -0x1.930b705f9fad17fb56813a45973a8d72p-1,
    -0x1.d0000000004p-1
  },
  { // Entry 442
    0x1.7ef24c8e67d9a8017a031f38e24cce06p-1,
    0x1.d01p199
  },
  { // Entry 443
    -0x1.7ef24c8e67d9a8017a031f38e24cce06p-1,
    -0x1.d01p199
  },
  { // Entry 444
    0x1.ffff124c001aafb1f175ccb531c053b0p-1,
    0x1.d024ba6f953cfp1000
  },
  { // Entry 445
    -0x1.ffff124c001aafb1f175ccb531c053b0p-1,
    -0x1.d024ba6f953cfp1000
  },
  { // Entry 446
    -0x1.f83a0983dd15d00301e2df21e3bee635p-2,
    0x1.d4067c60f471ep1
  },
  { // Entry 447
    0x1.f83a0983dd15d00301e2df21e3bee635p-2,
    -0x1.d4067c60f471ep1
  },
  { // Entry 448
    0x1.d79b9896ff55484d2591b677d54ea46fp-5,
    0x1.d7de6263bcaabp-5
  },
  { // Entry 449
    -0x1.d79b9896ff55484d2591b677d54ea46fp-5,
    -0x1.d7de6263bcaabp-5
  },
  { // Entry 450
    0x1.ed0b908a2982f7fd9c80bc1051dd4080p-1,
    0x1.d800000002274p0
  },
  { // Entry 451
    -0x1.ed0b908a2982f7fd9c80bc1051dd4080p-1,
    -0x1.d800000002274p0
  },
  { // Entry 452
    -0x1.f2c217cbc7dcc919243a7c434a209f9ap-1,
    0x1.d96e058p488
  },
  { // Entry 453
    0x1.f2c217cbc7dcc919243a7c434a209f9ap-1,
    -0x1.d96e058p488
  },
  { // Entry 454
    0x1.98dcd093377928000000000000167dc7p-1,
    0x1.d98c4c612718dp-1
  },
  { // Entry 455
    -0x1.98dcd093377928000000000000167dc7p-1,
    -0x1.d98c4c612718dp-1
  },
  { // Entry 456
    0x1.db3ba8775ca257da3bd5449d396a66e2p-5,
    0x1.db8p-5
  },
  { // Entry 457
    -0x1.db3ba8775ca257da3bd5449d396a66e2p-5,
    -0x1.db8p-5
  },
  { // Entry 458
    -0x1.9fee37697d5828031a6a2679c1433457p-2,
    0x1.de386d6090303p200
  },
  { // Entry 459
    0x1.9fee37697d5828031a6a2679c1433457p-2,
    -0x1.de386d6090303p200
  },
  { // Entry 460
    -0x1.5361ee6553188036e9dfcab428deb4e1p-53,
    0x1.de5e5054e921bp35
  },
  { // Entry 461
    0x1.5361ee6553188036e9dfcab428deb4e1p-53,
    -0x1.de5e5054e921bp35
  },
  { // Entry 462
    0x1.fec48d5e769ebe37448c10e0a7a0ecb1p-1,
    0x1.df77ddf77ddf4p10
  },
  { // Entry 463
    -0x1.fec48d5e769ebe37448c10e0a7a0ecb1p-1,
    -0x1.df77ddf77ddf4p10
  },
  { // Entry 464
    -0x1.2902a83d72632800003d1149b7e6c0fbp-1,
    0x1.e1562b0448a86p1
  },
  { // Entry 465
    0x1.2902a83d72632800003d1149b7e6c0fbp-1,
    -0x1.e1562b0448a86p1
  },
  { // Entry 466
    0x1.9e26c7bc96b68800000d4736ff132035p-1,
    0x1.e2700cdc86635p-1
  },
  { // Entry 467
    -0x1.9e26c7bc96b68800000d4736ff132035p-1,
    -0x1.e2700cdc86635p-1
  },
  { // Entry 468
    0x1.e6494911eedd18d0f1915fd4f6495469p-7,
    0x1.e64ddaf7bd730p-7
  },
  { // Entry 469
    -0x1.e6494911eedd18d0f1915fd4f6495469p-7,
    -0x1.e64ddaf7bd730p-7
  },
  { // Entry 470
    0x1.eb26c690bda2484a6c020a9c052ea846p-5,
    0x1.eb7239bca8afap-5
  },
  { // Entry 471
    -0x1.eb26c690bda2484a6c020a9c052ea846p-5,
    -0x1.eb7239bca8afap-5
  },
  { // Entry 472
    0x1.c73238790a4ce801e04107b2546b4ae3p-3,
    0x1.ef7b83f7bdef4p3
  },
  { // Entry 473
    -0x1.c73238790a4ce801e04107b2546b4ae3p-3,
    -0x1.ef7b83f7bdef4p3
  },
  { // Entry 474
    0x1.ed1b575acb8c881082ce315074a8a42ap-3,
    0x1.f20000000109bp-3
  },
  { // Entry 475
    -0x1.ed1b575acb8c881082ce315074a8a42ap-3,
    -0x1.f20000000109bp-3
  },
  { // Entry 476
    0x1.c1b50a56c8809800000282a516c13e25p-1,
    0x1.f40ca67a9e8d7p9
  },
  { // Entry 477
    -0x1.c1b50a56c8809800000282a516c13e25p-1,
    -0x1.f40ca67a9e8d7p9
  },
  { // Entry 478
    0x1.e321fea643a968083208768e9e2cd28ep-2,
    0x1.f7224d2c77540p-2
  },
  { // Entry 479
    -0x1.e321fea643a968083208768e9e2cd28ep-2,
    -0x1.f7224d2c77540p-2
  },
  { // Entry 480
    0x1.c1269b020a107ffc00612a563a919686p-3,
    0x1.f78a0d05e60e2p6
  },
  { // Entry 481
    -0x1.c1269b020a107ffc00612a563a919686p-3,
    -0x1.f78a0d05e60e2p6
  },
  { // Entry 482
    0x1.f76cae28a07747fffcd453a0142c2ec1p-5,
    0x1.f7bdef7bdf073p-5
  },
  { // Entry 483
    -0x1.f76cae28a07747fffcd453a0142c2ec1p-5,
    -0x1.f7bdef7bdf073p-5
  },
  { // Entry 484
    0x1.e42c139dc2053807b9440f1a28f7c9b9p-2,
    0x1.f8502d5955443p-2
  },
  { // Entry 485
    -0x1.e42c139dc2053807b9440f1a28f7c9b9p-2,
    -0x1.f8502d5955443p-2
  },
  { // Entry 486
    0x1.0fa749e07f63fc9d01c834de47654acbp-9,
    0x1.f8fc824d2693bp61
  },
  { // Entry 487
    -0x1.0fa749e07f63fc9d01c834de47654acbp-9,
    -0x1.f8fc824d2693bp61
  },
  { // Entry 488
    0x1.ffa80324e2d8ebc02f323ed49eb97a6cp-1,
    0x1.f8fffffffffffp2
  },
  { // Entry 489
    -0x1.ffa80324e2d8ebc02f323ed49eb97a6cp-1,
    -0x1.f8fffffffffffp2
  },
  { // Entry 490
    -0x1.7cdf79d5e37b8b212eff86d2ffe2044bp-1,
    0x1.fd8p1
  },
  { // Entry 491
    0x1.7cdf79d5e37b8b212eff86d2ffe2044bp-1,
    -0x1.fd8p1
  },
  { // Entry 492
    0x1.fd3f48847a1d10a8e5ff1d4db84ed26dp-5,
    0x1.fd9364d936596p-5
  },
  { // Entry 493
    -0x1.fd3f48847a1d10a8e5ff1d4db84ed26dp-5,
    -0x1.fd9364d936596p-5
  },
  { // Entry 494
    0x1.f93ad471d262f8032e92d596797f9d41p-3,
    0x1.fe8p-3
  },
  { // Entry 495
    -0x1.f93ad471d262f8032e92d596797f9d41p-3,
    -0x1.fe8p-3
  },
  { // Entry 496
    0x1.83b3062414973d92c4278507f0474d67p-1,
    0x1.febb646e2ee57p13
  },
  { // Entry 497
    -0x1.83b3062414973d92c4278507f0474d67p-1,
    -0x1.febb646e2ee57p13
  },
  { // Entry 498
    0x1.3b45bd744977547fa4673e42dfc99402p-1,
    0x1.feeffffffffc6p995
  },
  { // Entry 499
    -0x1.3b45bd744977547fa4673e42dfc99402p-1,
    -0x1.feeffffffffc6p995
  },
  { // Entry 500
    -0x1.eefb59d143645a40041bf726dc6f2fa0p-1,
    0x1.ff8ffffffffffp7
  },
  { // Entry 501
    0x1.eefb59d143645a40041bf726dc6f2fa0p-1,
    -0x1.ff8ffffffffffp7
  },
  { // Entry 502
    -0x1.56433f0c6bcee9848751611c6f5ade1fp-1,
    0x1.ff8ffffffffffp870
  },
  { // Entry 503
    0x1.56433f0c6bcee9848751611c6f5ade1fp-1,
    -0x1.ff8ffffffffffp870
  },
  { // Entry 504
    -0x1.930006246a6c05dab72ea697daaba69dp-2,
    0x1.ffcfff8p19
  },
  { // Entry 505
    0x1.930006246a6c05dab72ea697daaba69dp-2,
    -0x1.ffcfff8p19
  },
  { // Entry 506
    0x1.ded37a1f0aa6d05d782d6aab40cb8670p-1,
    0x1.ffcfff8p365
  },
  { // Entry 507
    -0x1.ded37a1f0aa6d05d782d6aab40cb8670p-1,
    -0x1.ffcfff8p365
  },
  { // Entry 508
    -0x1.93e4d96b621e07bca866c1a2e771881cp-1,
    0x1.ffcffffffff6cp720
  },
  { // Entry 509
    0x1.93e4d96b621e07bca866c1a2e771881cp-1,
    -0x1.ffcffffffff6cp720
  },
  { // Entry 510
    0x1.9068b90e42605e05010e147110688e52p-1,
    0x1.ffcfffffffff9p320
  },
  { // Entry 511
    -0x1.9068b90e42605e05010e147110688e52p-1,
    -0x1.ffcfffffffff9p320
  },
  { // Entry 512
    0x1.cf81642e7421c56507eb8187611c84dbp-1,
    0x1.ffcffffffffffp12
  },
  { // Entry 513
    -0x1.cf81642e7421c56507eb8187611c84dbp-1,
    -0x1.ffcffffffffffp12
  },
  { // Entry 514
    0x1.ffffffe61fe616b282c3a69ca225c2d2p-1,
    0x1.ffcffffffffffp404
  },
  { // Entry 515
    -0x1.ffffffe61fe616b282c3a69ca225c2d2p-1,
    -0x1.ffcffffffffffp404
  },
  { // Entry 516
    -0x1.406ee9ae91e16f1d58c96eb7165f595bp-1,
    0x1.ffeffffffffccp995
  },
  { // Entry 517
    0x1.406ee9ae91e16f1d58c96eb7165f595bp-1,
    -0x1.ffeffffffffccp995
  },
  { // Entry 518
    0x1.fa9f6ca0ec44e0010026f385c0ab8690p-3,
    0x1.ffeffffffffffp-3
  },
  { // Entry 519
    -0x1.fa9f6ca0ec44e0010026f385c0ab8690p-3,
    -0x1.ffeffffffffffp-3
  },
  { // Entry 520
    0x1.6b491db8b66d880121f830d8c755ae54p-4,
    0x1.ffeffffffffffp55
  },
  { // Entry 521
    -0x1.6b491db8b66d880121f830d8c755ae54p-4,
    -0x1.ffeffffffffffp55
  },
  { // Entry 522
    0x1.fb0ab102cb12fef9a4e4a869a299ed46p-1,
    0x1.ffeffffffffffp180
  },
  { // Entry 523
    -0x1.fb0ab102cb12fef9a4e4a869a299ed46p-1,
    -0x1.ffeffffffffffp180
  },
  { // Entry 524
    0x1.e4315ec04635cd34b3ef60370511281fp-3,
    0x1.ffeffffffffffp706
  },
  { // Entry 525
    -0x1.e4315ec04635cd34b3ef60370511281fp-3,
    -0x1.ffeffffffffffp706
  },
  { // Entry 526
    0x1.ffffc39997ef67d2b9c7e4efa8b77e50p-1,
    0x1.fff1fffffffffp41
  },
  { // Entry 527
    -0x1.ffffc39997ef67d2b9c7e4efa8b77e50p-1,
    -0x1.fff1fffffffffp41
  },
  { // Entry 528
    0x1.fff163992831e8c68b0234b42efa1d0ap-7,
    0x1.fff6b89ffffffp-7
  },
  { // Entry 529
    -0x1.fff163992831e8c68b0234b42efa1d0ap-7,
    -0x1.fff6b89ffffffp-7
  },
  { // Entry 530
    -0x1.d9757a05fcc42c664f3a66b5576a98e4p-1,
    0x1.fffdffff0001fp105
  },
  { // Entry 531
    0x1.d9757a05fcc42c664f3a66b5576a98e4p-1,
    -0x1.fffdffff0001fp105
  },
  { // Entry 532
    -0x1.83791fe63a17a80258f2c479c7e1d339p-1,
    0x1.ffff0c0000002p1
  },
  { // Entry 533
    0x1.83791fe63a17a80258f2c479c7e1d339p-1,
    -0x1.ffff0c0000002p1
  },
  { // Entry 534
    -0x1.d9d3a85acc50c8000005d13dd82f61b5p-1,
    0x1.ffffc00000055p150
  },
  { // Entry 535
    0x1.d9d3a85acc50c8000005d13dd82f61b5p-1,
    -0x1.ffffc00000055p150
  },
  { // Entry 536
    -0x1.f25d858dcdee7412cf0760e955cb8390p-3,
    0x1.ffffe3fffffffp40
  },
  { // Entry 537
    0x1.f25d858dcdee7412cf0760e955cb8390p-3,
    -0x1.ffffe3fffffffp40
  },
  { // Entry 538
    0x1.d18f7bfe557eb80001557142bda18baep-1,
    0x1.ffffefffcffaep0
  },
  { // Entry 539
    -0x1.d18f7bfe557eb80001557142bda18baep-1,
    -0x1.ffffefffcffaep0
  },
  { // Entry 540
    -0x1.bc14ebf6bfb51ffa5e6f5b0b0bf9e899p-4,
    0x1.fffffbfffffffp228
  },
  { // Entry 541
    0x1.bc14ebf6bfb51ffa5e6f5b0b0bf9e899p-4,
    -0x1.fffffbfffffffp228
  },
  { // Entry 542
    0x1.bb887a06f6c50fef0654d303c82f1a78p-3,
    0x1.fffffbfffffffp735
  },
  { // Entry 543
    -0x1.bb887a06f6c50fef0654d303c82f1a78p-3,
    -0x1.fffffbfffffffp735
  },
  { // Entry 544
    0x1.ffaaadef54e2f0c4081880b7b9e794dfp-5,
    0x1.fffffefffffffp-5
  },
  { // Entry 545
    -0x1.ffaaadef54e2f0c4081880b7b9e794dfp-5,
    -0x1.fffffefffffffp-5
  },
  { // Entry 546
    0x1.d4a3c62c5be08b123868e8a467eff0cep-1,
    0x1.ffffff8p119
  },
  { // Entry 547
    -0x1.d4a3c62c5be08b123868e8a467eff0cep-1,
    -0x1.ffffff8p119
  },
  { // Entry 548
    0x1.cec20f197703f4fd42048ddf889fe84ap-3,
    0x1.ffffff8p192
  },
  { // Entry 549
    -0x1.cec20f197703f4fd42048ddf889fe84ap-3,
    -0x1.ffffff8p192
  },
  { // Entry 550
    0x1.d37aadc7c8662501751dbf4e9e5701abp-2,
    0x1.ffffff8p543
  },
  { // Entry 551
    -0x1.d37aadc7c8662501751dbf4e9e5701abp-2,
    -0x1.ffffff8p543
  },
  { // Entry 552
    0x1.fa8d2a4d0a2027f0f74df3136cb78f72p-1,
    0x1.ffffffc0018ffp2
  },
  { // Entry 553
    -0x1.fa8d2a4d0a2027f0f74df3136cb78f72p-1,
    -0x1.ffffffc0018ffp2
  },
  { // Entry 554
    0x1.fa8d2a029f977ffe74cb14cb09df176dp-1,
    0x1.ffffffffeffffp2
  },
  { // Entry 555
    -0x1.fa8d2a029f977ffe74cb14cb09df176dp-1,
    -0x1.ffffffffeffffp2
  },
  { // Entry 556
    -0x1.2763f02a2d1e980000015dbf95d89ab2p-4,
    0x1.fffffffff8250p943
  },
  { // Entry 557
    0x1.2763f02a2d1e980000015dbf95d89ab2p-4,
    -0x1.fffffffff8250p943
  },
  { // Entry 558
    0x1.fcff128f77dda81c2a0ec03e4ed80422p-1,
    0x1.fffffffffe09dp320
  },
  { // Entry 559
    -0x1.fcff128f77dda81c2a0ec03e4ed80422p-1,
    -0x1.fffffffffe09dp320
  },
  { // Entry 560
    -0x1.fcc0bfedd84a5816d9596b8f9fbb9b41p-1,
    0x1.fffffffffe6e3p720
  },
  { // Entry 561
    0x1.fcc0bfedd84a5816d9596b8f9fbb9b41p-1,
    -0x1.fffffffffe6e3p720
  },
  { // Entry 562
    0x1.aed548f090c1dffe6e04322dc8e8cbfap-1,
    0x1.ffffffffffe7fp-1
  },
  { // Entry 563
    -0x1.aed548f090c1dffe6e04322dc8e8cbfap-1,
    -0x1.ffffffffffe7fp-1
  },
  { // Entry 564
    -0x1.f5e11def99d2b22098242ac9fb756adbp-1,
    0x1.ffffffffffeffp250
  },
  { // Entry 565
    0x1.f5e11def99d2b22098242ac9fb756adbp-1,
    -0x1.ffffffffffeffp250
  },
  { // Entry 566
    0x1.8a9cbf48fec9f2a0fde951f313b918ccp-1,
    0x1.fffffffffff78p920
  },
  { // Entry 567
    -0x1.8a9cbf48fec9f2a0fde951f313b918ccp-1,
    -0x1.fffffffffff78p920
  },
  { // Entry 568
    -0x1.7eba5894844cc00c88f39bd3d4197d14p-3,
    0x1.fffffffffff83p150
  },
  { // Entry 569
    0x1.7eba5894844cc00c88f39bd3d4197d14p-3,
    -0x1.fffffffffff83p150
  },
  { // Entry 570
    0x1.92c4f06d2cdd15d302609caa87882afap-1,
    0x1.fffffffffffd5p995
  },
  { // Entry 571
    -0x1.92c4f06d2cdd15d302609caa87882afap-1,
    -0x1.fffffffffffd5p995
  },
  { // Entry 572
    -0x1.3d5f7deb1d3bad63eb173b73e84110a0p-1,
    0x1.fffffffffffe8p720
  },
  { // Entry 573
    0x1.3d5f7deb1d3bad63eb173b73e84110a0p-1,
    -0x1.fffffffffffe8p720
  },
  { // Entry 574
    -0x1.91349b0ae90e5316539c07eea9b4e6b0p-1,
    0x1.fffffffffffebp920
  },
  { // Entry 575
    0x1.91349b0ae90e5316539c07eea9b4e6b0p-1,
    -0x1.fffffffffffebp920
  },
  { // Entry 576
    -0x1.837b9dddc1e8738d74b2a74fa42d802dp-1,
    0x1.ffffffffffff1p1
  },
  { // Entry 577
    0x1.837b9dddc1e8738d74b2a74fa42d802dp-1,
    -0x1.ffffffffffff1p1
  },
  { // Entry 578
    0x1.510e062e7fa2037d3842de993c9c5090p-1,
    0x1.ffffffffffff1p245
  },
  { // Entry 579
    -0x1.510e062e7fa2037d3842de993c9c5090p-1,
    -0x1.ffffffffffff1p245
  },
  { // Entry 580
    0x1.eaee8744b05e47fde092e3a14895d067p-2,
    0x1.ffffffffffff3p-2
  },
  { // Entry 581
    -0x1.eaee8744b05e47fde092e3a14895d067p-2,
    -0x1.ffffffffffff3p-2
  },
  { // Entry 582
    0x1.8a4dee8f4062791078766b0a01de360dp-1,
    0x1.ffffffffffff4p845
  },
  { // Entry 583
    -0x1.8a4dee8f4062791078766b0a01de360dp-1,
    -0x1.ffffffffffff4p845
  },
  { // Entry 584
    0x1.5118d6bbde07e4eebf2a11df4dc74d54p-1,
    0x1.ffffffffffff4p1020
  },
  { // Entry 585
    -0x1.5118d6bbde07e4eebf2a11df4dc74d54p-1,
    -0x1.ffffffffffff4p1020
  },
  { // Entry 586
    -0x1.5cd5c53cf30a97ff0e471d149c40955dp-1,
    0x1.ffffffffffff8p616
  },
  { // Entry 587
    0x1.5cd5c53cf30a97ff0e471d149c40955dp-1,
    -0x1.ffffffffffff8p616
  },
  { // Entry 588
    0x1.ffffa1f0d7daf693a80a2b817b16f277p-1,
    0x1.ffffffffffffcp475
  },
  { // Entry 589
    -0x1.ffffa1f0d7daf693a80a2b817b16f277p-1,
    -0x1.ffffffffffffcp475
  },
  { // Entry 590
    0x1.51e9d840106d73484d44a86f74c3d41dp-1,
    0x1.ffffffffffffep970
  },
  { // Entry 591
    -0x1.51e9d840106d73484d44a86f74c3d41dp-1,
    -0x1.ffffffffffffep970
  },
  { // Entry 592
    -0.0,
    -0x1.0p-1074
  },
  { // Entry 593
    0.0,
    0x1.0p-1074
  },
  { // Entry 594
    -0.0,
    -0.0
  },
  { // Entry 595
    0.0,
    0x1.0p-1074
  },
  { // Entry 596
    -0.0,
    -0x1.0p-1074
  },
  { // Entry 597
    -0x1.0000000000000fffffffffffffffffffp-1022,
    -0x1.0000000000001p-1022
  },
  { // Entry 598
    0x1.0000000000000fffffffffffffffffffp-1022,
    0x1.0000000000001p-1022
  },
  { // Entry 599
    -0x1.ffffffffffffffffffffffffffffffffp-1023,
    -0x1.0p-1022
  },
  { // Entry 600
    0x1.ffffffffffffffffffffffffffffffffp-1023,
    0x1.0p-1022
  },
  { // Entry 601
    -0x1.ffffffffffffdfffffffffffffffffffp-1023,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 602
    0x1.ffffffffffffdfffffffffffffffffffp-1023,
    0x1.ffffffffffffep-1023
  },
  { // Entry 603
    0x1.ffffffffffffdfffffffffffffffffffp-1023,
    0x1.ffffffffffffep-1023
  },
  { // Entry 604
    -0x1.ffffffffffffdfffffffffffffffffffp-1023,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 605
    0x1.ffffffffffffffffffffffffffffffffp-1023,
    0x1.0p-1022
  },
  { // Entry 606
    -0x1.ffffffffffffffffffffffffffffffffp-1023,
    -0x1.0p-1022
  },
  { // Entry 607
    0x1.0000000000000fffffffffffffffffffp-1022,
    0x1.0000000000001p-1022
  },
  { // Entry 608
    -0x1.0000000000000fffffffffffffffffffp-1022,
    -0x1.0000000000001p-1022
  },
  { // Entry 609
    0x1.9999996de8ca198c24ab9449beee16d5p-13,
    0x1.999999999999ap-13
  },
  { // Entry 610
    -0x1.9999996de8ca198c24ab9449beee16d5p-13,
    -0x1.999999999999ap-13
  },
  { // Entry 611
    0x1.999998ead65b96f78a4dbfd839c7ef2ep-12,
    0x1.999999999999ap-12
  },
  { // Entry 612
    -0x1.999998ead65b96f78a4dbfd839c7ef2ep-12,
    -0x1.999999999999ap-12
  },
  { // Entry 613
    0x1.3333320c49babff151b6d04290e2c3a2p-11,
    0x1.3333333333334p-11
  },
  { // Entry 614
    -0x1.3333320c49babff151b6d04290e2c3a2p-11,
    -0x1.3333333333334p-11
  },
  { // Entry 615
    0x1.999996de8ca2884da2f08f25bb024d08p-11,
    0x1.999999999999ap-11
  },
  { // Entry 616
    -0x1.999996de8ca2884da2f08f25bb024d08p-11,
    -0x1.999999999999ap-11
  },
  { // Entry 617
    0x1.fffffaaaaaaeeeeeed4ed4edab4c7bd6p-11,
    0x1.0p-10
  },
  { // Entry 618
    -0x1.fffffaaaaaaeeeeeed4ed4edab4c7bd6p-11,
    -0x1.0p-10
  },
  { // Entry 619
    0x1.33332e978d552afc883bdb04751e3835p-10,
    0x1.3333333333333p-10
  },
  { // Entry 620
    -0x1.33332e978d552afc883bdb04751e3835p-10,
    -0x1.3333333333333p-10
  },
  { // Entry 621
    0x1.66665f1529aff8a3809246670a436c3cp-10,
    0x1.6666666666666p-10
  },
  { // Entry 622
    -0x1.66665f1529aff8a3809246670a436c3cp-10,
    -0x1.6666666666666p-10
  },
  { // Entry 623
    0x1.99998ead65cdf82e194c133997f2fb68p-10,
    0x1.9999999999999p-10
  },
  { // Entry 624
    -0x1.99998ead65cdf82e194c133997f2fb68p-10,
    -0x1.9999999999999p-10
  },
  { // Entry 625
    0x1.ccccbd3f7d15d42affb9f02bf1dc257bp-10,
    0x1.cccccccccccccp-10
  },
  { // Entry 626
    -0x1.ccccbd3f7d15d42affb9f02bf1dc257bp-10,
    -0x1.cccccccccccccp-10
  },
  { // Entry 627
    0x1.0665ae9c7b44ed280216be2104f28f02p-7,
    0x1.0666666666666p-7
  },
  { // Entry 628
    -0x1.0665ae9c7b44ed280216be2104f28f02p-7,
    -0x1.0666666666666p-7
  },
  { // Entry 629
    0x1.ccc8e97b59f618898c4ac3a0aeddf709p-7,
    0x1.cccccccccccccp-7
  },
  { // Entry 630
    -0x1.ccc8e97b59f618898c4ac3a0aeddf709p-7,
    -0x1.cccccccccccccp-7
  },
  { // Entry 631
    0x1.4993e8a8ff79b132046efa7856a97538p-6,
    0x1.4999999999999p-6
  },
  { // Entry 632
    -0x1.4993e8a8ff79b132046efa7856a97538p-6,
    -0x1.4999999999999p-6
  },
  { // Entry 633
    0x1.acc044c56db0e19f82c9c3cff246e201p-6,
    0x1.accccccccccccp-6
  },
  { // Entry 634
    -0x1.acc044c56db0e19f82c9c3cff246e201p-6,
    -0x1.accccccccccccp-6
  },
  { // Entry 635
    0x1.07f44d67cf41afbc0c95108b99f91b01p-5,
    0x1.080p-5
  },
  { // Entry 636
    -0x1.07f44d67cf41afbc0c95108b99f91b01p-5,
    -0x1.080p-5
  },
  { // Entry 637
    0x1.3985fe46f1c8714eaa1418561963e89bp-5,
    0x1.399999999999ap-5
  },
  { // Entry 638
    -0x1.3985fe46f1c8714eaa1418561963e89bp-5,
    -0x1.399999999999ap-5
  },
  { // Entry 639
    0x1.6b14bde93ac5f7d24544d0ecf8be7aeep-5,
    0x1.6b33333333334p-5
  },
  { // Entry 640
    -0x1.6b14bde93ac5f7d24544d0ecf8be7aeep-5,
    -0x1.6b33333333334p-5
  },
  { // Entry 641
    0x1.9ca0153ed8396b02f8605219a5fe5917p-5,
    0x1.9cccccccccccep-5
  },
  { // Entry 642
    -0x1.9ca0153ed8396b02f8605219a5fe5917p-5,
    -0x1.9cccccccccccep-5
  },
  { // Entry 643
    0x1.ce278d4027d34387f184d4ab2aaf545fp-5,
    0x1.ce66666666666p-5
  },
  { // Entry 644
    -0x1.ce278d4027d34387f184d4ab2aaf545fp-5,
    -0x1.ce66666666666p-5
  },
  { // Entry 645
    0x1.43c1e9c171a667a0b92519a04fa5a91cp-1,
    0x1.5e7fc4369bdadp-1
  },
  { // Entry 646
    -0x1.43c1e9c171a667a0b92519a04fa5a91cp-1,
    -0x1.5e7fc4369bdadp-1
  },
  { // Entry 647
    0x1.ee3d6bcea09ca18b1d1ce7ee04fd886fp-1,
    0x1.4e7fc4369bdadp0
  },
  { // Entry 648
    -0x1.ee3d6bcea09ca18b1d1ce7ee04fd886fp-1,
    -0x1.4e7fc4369bdadp0
  },
  { // Entry 649
    0x1.df8e22ea809d65c6a69b96aca60be432p-1,
    0x1.edbfa651e9c84p0
  },
  { // Entry 650
    -0x1.df8e22ea809d65c6a69b96aca60be432p-1,
    -0x1.edbfa651e9c84p0
  },
  { // Entry 651
    0x1.1d3479eac7ae35e2fbea0ae696434692p-1,
    0x1.467fc4369bdadp1
  },
  { // Entry 652
    -0x1.1d3479eac7ae35e2fbea0ae696434692p-1,
    -0x1.467fc4369bdadp1
  },
  { // Entry 653
    -0x1.ffeaaaeeee84b44ccefef832254d28c0p-6,
    0x1.961fb54442d18p1
  },
  { // Entry 654
    0x1.ffeaaaeeee84b44ccefef832254d28c0p-6,
    -0x1.961fb54442d18p1
  },
  { // Entry 655
    -0x1.3734d32d49bd0b942772a7567d514140p-1,
    0x1.e5bfa651e9c83p1
  },
  { // Entry 656
    0x1.3734d32d49bd0b942772a7567d514140p-1,
    -0x1.e5bfa651e9c83p1
  },
  { // Entry 657
    -0x1.e9d25d19911e205b653521f42b9b864fp-1,
    0x1.1aafcbafc85f7p2
  },
  { // Entry 658
    0x1.e9d25d19911e205b653521f42b9b864fp-1,
    -0x1.1aafcbafc85f7p2
  },
  { // Entry 659
    -0x1.e4ecdc5a4e465899928eb9fc95829d48p-1,
    0x1.427fc4369bdadp2
  },
  { // Entry 660
    0x1.e4ecdc5a4e465899928eb9fc95829d48p-1,
    -0x1.427fc4369bdadp2
  },
  { // Entry 661
    -0x1.2a59f1034426197fa6eee22762967f25p-1,
    0x1.6a4fbcbd6f562p2
  },
  { // Entry 662
    0x1.2a59f1034426197fa6eee22762967f25p-1,
    -0x1.6a4fbcbd6f562p2
  },
  { // Entry 663
    -0x1.26312443bd35f19312eac0a1a6b5659ep-1,
    0x1.6af2eff0a2896p2
  },
  { // Entry 664
    0x1.26312443bd35f19312eac0a1a6b5659ep-1,
    -0x1.6af2eff0a2896p2
  },
  { // Entry 665
    -0x1.e18e660a5e2fb316ecbb9ed70122eff5p-1,
    0x1.43c62a9d02414p2
  },
  { // Entry 666
    0x1.e18e660a5e2fb316ecbb9ed70122eff5p-1,
    -0x1.43c62a9d02414p2
  },
  { // Entry 667
    -0x1.ee0e83a0198b6e2ef7c48e6625291a0ap-1,
    0x1.1c99654961f92p2
  },
  { // Entry 668
    0x1.ee0e83a0198b6e2ef7c48e6625291a0ap-1,
    -0x1.1c99654961f92p2
  },
  { // Entry 669
    -0x1.4727747338e4653616eadbd7ec3d02d3p-1,
    0x1.ead93feb8361fp1
  },
  { // Entry 670
    0x1.4727747338e4653616eadbd7ec3d02d3p-1,
    -0x1.ead93feb8361fp1
  },
  { // Entry 671
    -0x1.4ba2f75dda5fe434320905a7184ff1afp-4,
    0x1.9c7fb54442d1ap1
  },
  { // Entry 672
    0x1.4ba2f75dda5fe434320905a7184ff1afp-4,
    -0x1.9c7fb54442d1ap1
  },
  { // Entry 673
    0x1.034c4d633b4ef0a9089b43892a462a26p-1,
    0x1.4e262a9d02415p1
  },
  { // Entry 674
    -0x1.034c4d633b4ef0a9089b43892a462a26p-1,
    -0x1.4e262a9d02415p1
  },
  { // Entry 675
    0x1.d1e4cde2f3944f4c134c05cc4e5339a3p-1,
    0x1.ff993feb83620p0
  },
  { // Entry 676
    -0x1.d1e4cde2f3944f4c134c05cc4e5339a3p-1,
    -0x1.ff993feb83620p0
  },
  { // Entry 677
    0x1.f750235c949926c48c90e41a91474c06p-1,
    0x1.62e62a9d02416p0
  },
  { // Entry 678
    -0x1.f750235c949926c48c90e41a91474c06p-1,
    -0x1.62e62a9d02416p0
  },
  { // Entry 679
    0x1.65f7d571279b0b8005552fd47a2e77aep-1,
    0x1.8c662a9d02419p-1
  },
  { // Entry 680
    -0x1.65f7d571279b0b8005552fd47a2e77aep-1,
    -0x1.8c662a9d02419p-1
  },
  { // Entry 681
    -0x1.fe043f57369d6a52fa33f0119ec4da19p-1,
    -0x1.a8aa1d11c44ffp0
  },
  { // Entry 682
    0x1.fe043f57369d6a52fa33f0119ec4da19p-1,
    0x1.a8aa1d11c44ffp0
  },
  { // Entry 683
    -0x1.fff18f24f3e4b87bf8c3762cb44f46d6p-1,
    -0x1.95ec8b9e03d54p0
  },
  { // Entry 684
    0x1.fff18f24f3e4b87bf8c3762cb44f46d6p-1,
    0x1.95ec8b9e03d54p0
  },
  { // Entry 685
    -0x1.ff20d961624e7063a78203b811f579cap-1,
    -0x1.832efa2a435a9p0
  },
  { // Entry 686
    0x1.ff20d961624e7063a78203b811f579cap-1,
    0x1.832efa2a435a9p0
  },
  { // Entry 687
    -0x1.fb933c40107fd775185ac14918c8fbafp-1,
    -0x1.707168b682dfep0
  },
  { // Entry 688
    0x1.fb933c40107fd775185ac14918c8fbafp-1,
    0x1.707168b682dfep0
  },
  { // Entry 689
    -0x1.f54d971881ad685b782ef88e6350f7cdp-1,
    -0x1.5db3d742c2653p0
  },
  { // Entry 690
    0x1.f54d971881ad685b782ef88e6350f7cdp-1,
    0x1.5db3d742c2653p0
  },
  { // Entry 691
    -0x1.ec5883b7b6cf4d859ab04e15d53698c9p-1,
    -0x1.4af645cf01ea8p0
  },
  { // Entry 692
    0x1.ec5883b7b6cf4d859ab04e15d53698c9p-1,
    0x1.4af645cf01ea8p0
  },
  { // Entry 693
    -0x1.e0c04a94e17309c806c1c78bddc1d607p-1,
    -0x1.3838b45b416fdp0
  },
  { // Entry 694
    0x1.e0c04a94e17309c806c1c78bddc1d607p-1,
    0x1.3838b45b416fdp0
  },
  { // Entry 695
    -0x1.d294d1f96c7ebdb9869dd97cf574ddb9p-1,
    -0x1.257b22e780f52p0
  },
  { // Entry 696
    0x1.d294d1f96c7ebdb9869dd97cf574ddb9p-1,
    0x1.257b22e780f52p0
  },
  { // Entry 697
    -0x1.c1e9883373d7ecc48c92dc8875505f7ep-1,
    -0x1.12bd9173c07abp0
  },
  { // Entry 698
    0x1.c1e9883373d7ecc48c92dc8875505f7ep-1,
    0x1.12bd9173c07abp0
  },
  { // Entry 699
    -0x1.a2c289d9d055ac377f67d7a54a0b3005p-1,
    -0x1.ea5c3ed5b3850p-1
  },
  { // Entry 700
    0x1.a2c289d9d055ac377f67d7a54a0b3005p-1,
    0x1.ea5c3ed5b3850p-1
  },
  { // Entry 701
    -0x1.95f05257dbcb5f4b12636c5878ea405ap-1,
    -0x1.d4b87dab670a0p-1
  },
  { // Entry 702
    0x1.95f05257dbcb5f4b12636c5878ea405ap-1,
    0x1.d4b87dab670a0p-1
  },
  { // Entry 703
    -0x1.88647f26a6e0f6b2715a6c3797ec11f5p-1,
    -0x1.bf14bc811a8f0p-1
  },
  { // Entry 704
    0x1.88647f26a6e0f6b2715a6c3797ec11f5p-1,
    0x1.bf14bc811a8f0p-1
  },
  { // Entry 705
    -0x1.7a2541dfd4e752de38f04aba21fc9d9fp-1,
    -0x1.a970fb56ce140p-1
  },
  { // Entry 706
    0x1.7a2541dfd4e752de38f04aba21fc9d9fp-1,
    0x1.a970fb56ce140p-1
  },
  { // Entry 707
    -0x1.6b391e25bc26cbbcf7a0184070af9c39p-1,
    -0x1.93cd3a2c81990p-1
  },
  { // Entry 708
    0x1.6b391e25bc26cbbcf7a0184070af9c39p-1,
    0x1.93cd3a2c81990p-1
  },
  { // Entry 709
    -0x1.5ba6e6a8e706535b98fc99dfaef824f1p-1,
    -0x1.7e297902351e0p-1
  },
  { // Entry 710
    0x1.5ba6e6a8e706535b98fc99dfaef824f1p-1,
    0x1.7e297902351e0p-1
  },
  { // Entry 711
    -0x1.4b75ba096fa549eb93595d8194ab917fp-1,
    -0x1.6885b7d7e8a30p-1
  },
  { // Entry 712
    0x1.4b75ba096fa549eb93595d8194ab917fp-1,
    0x1.6885b7d7e8a30p-1
  },
  { // Entry 713
    -0x1.3aacff95a3122b15f372bfd2fdf9a75fp-1,
    -0x1.52e1f6ad9c280p-1
  },
  { // Entry 714
    0x1.3aacff95a3122b15f372bfd2fdf9a75fp-1,
    0x1.52e1f6ad9c280p-1
  },
  { // Entry 715
    -0x1.295463e769284a5aed17a443392f38f3p-1,
    -0x1.3d3e35834fad0p-1
  },
  { // Entry 716
    0x1.295463e769284a5aed17a443392f38f3p-1,
    0x1.3d3e35834fad0p-1
  },
  { // Entry 717
    -0x1.fc769b77e588495a6f642ca24e4ed3fcp-2,
    -0x1.0a0b02501c799p-1
  },
  { // Entry 718
    0x1.fc769b77e588495a6f642ca24e4ed3fcp-2,
    0x1.0a0b02501c799p-1
  },
  { // Entry 719
    -0x1.c853c78462de46b5743315612f8b5a7cp-2,
    -0x1.d8f7208e6b82cp-2
  },
  { // Entry 720
    0x1.c853c78462de46b5743315612f8b5a7cp-2,
    0x1.d8f7208e6b82cp-2
  },
  { // Entry 721
    -0x1.92aba90aaf27249de49c78fc643c8b72p-2,
    -0x1.9dd83c7c9e126p-2
  },
  { // Entry 722
    0x1.92aba90aaf27249de49c78fc643c8b72p-2,
    0x1.9dd83c7c9e126p-2
  },
  { // Entry 723
    -0x1.5bac064658f39460c83113c0a0097a0cp-2,
    -0x1.62b9586ad0a20p-2
  },
  { // Entry 724
    0x1.5bac064658f39460c83113c0a0097a0cp-2,
    0x1.62b9586ad0a20p-2
  },
  { // Entry 725
    -0x1.2383ca8078e58477cd5fb1d9de031dcep-2,
    -0x1.279a74590331ap-2
  },
  { // Entry 726
    0x1.2383ca8078e58477cd5fb1d9de031dcep-2,
    0x1.279a74590331ap-2
  },
  { // Entry 727
    -0x1.d4c5bc11d2371af2fe25ef5ede2766a3p-3,
    -0x1.d8f7208e6b829p-3
  },
  { // Entry 728
    0x1.d4c5bc11d2371af2fe25ef5ede2766a3p-3,
    0x1.d8f7208e6b829p-3
  },
  { // Entry 729
    -0x1.60f3faaf43023d3c7863ae06d4d59774p-3,
    -0x1.62b9586ad0a1ep-3
  },
  { // Entry 730
    0x1.60f3faaf43023d3c7863ae06d4d59774p-3,
    0x1.62b9586ad0a1ep-3
  },
  { // Entry 731
    -0x1.d7ea3de45a9d6563ac005c0c5bad8c50p-4,
    -0x1.d8f7208e6b826p-4
  },
  { // Entry 732
    0x1.d7ea3de45a9d6563ac005c0c5bad8c50p-4,
    0x1.d8f7208e6b826p-4
  },
  { // Entry 733
    -0x1.d8b3df489987a6fe0eead008e720aa22p-5,
    -0x1.d8f7208e6b82dp-5
  },
  { // Entry 734
    0x1.d8b3df489987a6fe0eead008e720aa22p-5,
    0x1.d8f7208e6b82dp-5
  },
  { // Entry 735
    0x1.d8b3df489987a6fe0eead008e720aa22p-5,
    0x1.d8f7208e6b82dp-5
  },
  { // Entry 736
    -0x1.d8b3df489987a6fe0eead008e720aa22p-5,
    -0x1.d8f7208e6b82dp-5
  },
  { // Entry 737
    0x1.d7ea3de45a9dd4a4bccd1a8c048faf4cp-4,
    0x1.d8f7208e6b82dp-4
  },
  { // Entry 738
    -0x1.d7ea3de45a9dd4a4bccd1a8c048faf4cp-4,
    -0x1.d8f7208e6b82dp-4
  },
  { // Entry 739
    0x1.60f3faaf43027c4752f564f9d0818fe8p-3,
    0x1.62b9586ad0a22p-3
  },
  { // Entry 740
    -0x1.60f3faaf43027c4752f564f9d0818fe8p-3,
    -0x1.62b9586ad0a22p-3
  },
  { // Entry 741
    0x1.d4c5bc11d23759400642e5a1efdc0f85p-3,
    0x1.d8f7208e6b82dp-3
  },
  { // Entry 742
    -0x1.d4c5bc11d23759400642e5a1efdc0f85p-3,
    -0x1.d8f7208e6b82dp-3
  },
  { // Entry 743
    0x1.2383ca8078e5a324d52c1530742cd4f5p-2,
    0x1.279a74590331cp-2
  },
  { // Entry 744
    -0x1.2383ca8078e5a324d52c1530742cd4f5p-2,
    -0x1.279a74590331cp-2
  },
  { // Entry 745
    0x1.5bac064658f3b27a28572bea256195efp-2,
    0x1.62b9586ad0a22p-2
  },
  { // Entry 746
    -0x1.5bac064658f3b27a28572bea256195efp-2,
    -0x1.62b9586ad0a22p-2
  },
  { // Entry 747
    0x1.92aba90aaf274209efaed08e34071e3bp-2,
    0x1.9dd83c7c9e128p-2
  },
  { // Entry 748
    -0x1.92aba90aaf274209efaed08e34071e3bp-2,
    -0x1.9dd83c7c9e128p-2
  },
  { // Entry 749
    0x1.c853c78462de635b10a2b93afd75da26p-2,
    0x1.d8f7208e6b82ep-2
  },
  { // Entry 750
    -0x1.c853c78462de635b10a2b93afd75da26p-2,
    -0x1.d8f7208e6b82ep-2
  },
  { // Entry 751
    0x1.fc769b77e588495a6f642ca24e4ed3fcp-2,
    0x1.0a0b02501c799p-1
  },
  { // Entry 752
    -0x1.fc769b77e588495a6f642ca24e4ed3fcp-2,
    -0x1.0a0b02501c799p-1
  },
  { // Entry 753
    0x1.295463e769281640ae026f50fc45e301p-1,
    0x1.3d3e35834faccp-1
  },
  { // Entry 754
    -0x1.295463e769281640ae026f50fc45e301p-1,
    -0x1.3d3e35834faccp-1
  },
  { // Entry 755
    0x1.3aacff95a311f899a0e279535e81c4ecp-1,
    0x1.52e1f6ad9c27cp-1
  },
  { // Entry 756
    -0x1.3aacff95a311f899a0e279535e81c4ecp-1,
    -0x1.52e1f6ad9c27cp-1
  },
  { // Entry 757
    0x1.4b75ba096fa5192442b7950f960f8006p-1,
    0x1.6885b7d7e8a2cp-1
  },
  { // Entry 758
    -0x1.4b75ba096fa5192442b7950f960f8006p-1,
    -0x1.6885b7d7e8a2cp-1
  },
  { // Entry 759
    0x1.5ba6e6a8e706245f97e28af3ddb700f6p-1,
    0x1.7e297902351dcp-1
  },
  { // Entry 760
    -0x1.5ba6e6a8e706245f97e28af3ddb700f6p-1,
    -0x1.7e297902351dcp-1
  },
  { // Entry 761
    0x1.6b391e25bc269ea1c1a40de62fbc03b4p-1,
    0x1.93cd3a2c8198cp-1
  },
  { // Entry 762
    -0x1.6b391e25bc269ea1c1a40de62fbc03b4p-1,
    -0x1.93cd3a2c8198cp-1
  },
  { // Entry 763
    0x1.7a2541dfd4e727b86dd309664186ec6bp-1,
    0x1.a970fb56ce13cp-1
  },
  { // Entry 764
    -0x1.7a2541dfd4e727b86dd309664186ec6bp-1,
    -0x1.a970fb56ce13cp-1
  },
  { // Entry 765
    0x1.88647f26a6e0cd95cb991f7ffe61a02ep-1,
    0x1.bf14bc811a8ecp-1
  },
  { // Entry 766
    -0x1.88647f26a6e0cd95cb991f7ffe61a02ep-1,
    -0x1.bf14bc811a8ecp-1
  },
  { // Entry 767
    0x1.95f05257dbcb384a5e326857376dd801p-1,
    0x1.d4b87dab6709cp-1
  },
  { // Entry 768
    -0x1.95f05257dbcb384a5e326857376dd801p-1,
    -0x1.d4b87dab6709cp-1
  },
  { // Entry 769
    0x1.a2c289d9d0558764921a4de355f9448cp-1,
    0x1.ea5c3ed5b384cp-1
  },
  { // Entry 770
    -0x1.a2c289d9d0558764921a4de355f9448cp-1,
    -0x1.ea5c3ed5b384cp-1
  },
  { // Entry 771
    0x1.c1e9883373d7ecc48c92dc8875505f7ep-1,
    0x1.12bd9173c07abp0
  },
  { // Entry 772
    -0x1.c1e9883373d7ecc48c92dc8875505f7ep-1,
    -0x1.12bd9173c07abp0
  },
  { // Entry 773
    0x1.d294d1f96c7ef26e203c5b309a55671fp-1,
    0x1.257b22e780f56p0
  },
  { // Entry 774
    -0x1.d294d1f96c7ef26e203c5b309a55671fp-1,
    -0x1.257b22e780f56p0
  },
  { // Entry 775
    0x1.e0c04a94e17335d073052a0394b9e1c3p-1,
    0x1.3838b45b41701p0
  },
  { // Entry 776
    -0x1.e0c04a94e17335d073052a0394b9e1c3p-1,
    -0x1.3838b45b41701p0
  },
  { // Entry 777
    0x1.ec5883b7b6cf70a577dd9160d0f8e9d5p-1,
    0x1.4af645cf01eacp0
  },
  { // Entry 778
    -0x1.ec5883b7b6cf70a577dd9160d0f8e9d5p-1,
    -0x1.4af645cf01eacp0
  },
  { // Entry 779
    0x1.f54d971881ad82629bd84d214194e8ddp-1,
    0x1.5db3d742c2657p0
  },
  { // Entry 780
    -0x1.f54d971881ad82629bd84d214194e8ddp-1,
    -0x1.5db3d742c2657p0
  },
  { // Entry 781
    0x1.fb933c40107fe83fd16c1789e27f69f7p-1,
    0x1.707168b682e02p0
  },
  { // Entry 782
    -0x1.fb933c40107fe83fd16c1789e27f69f7p-1,
    -0x1.707168b682e02p0
  },
  { // Entry 783
    0x1.ff20d961624e77daef329b4029c362dep-1,
    0x1.832efa2a435adp0
  },
  { // Entry 784
    -0x1.ff20d961624e77daef329b4029c362dep-1,
    -0x1.832efa2a435adp0
  },
  { // Entry 785
    0x1.fff18f24f3e4b69592294f206d7b32c2p-1,
    0x1.95ec8b9e03d58p0
  },
  { // Entry 786
    -0x1.fff18f24f3e4b69592294f206d7b32c2p-1,
    -0x1.95ec8b9e03d58p0
  },
  { // Entry 787
    0x1.fe043f57369d6a52fa33f0119ec4da19p-1,
    0x1.a8aa1d11c44ffp0
  },
  { // Entry 788
    -0x1.fe043f57369d6a52fa33f0119ec4da19p-1,
    -0x1.a8aa1d11c44ffp0
  },
  { // Entry 789
    0x1.b3d3695acc4136b2d44714f9b38419b4p-1,
    0x1.04aff6d330942p0
  },
  { // Entry 790
    -0x1.b3d3695acc4136b2d44714f9b38419b4p-1,
    -0x1.04aff6d330942p0
  },
  { // Entry 791
    0x1.b3d41972dc8063994f63413d5e4d8e4bp-1,
    0x1.04b09e98dcdb4p0
  },
  { // Entry 792
    -0x1.b3d41972dc8063994f63413d5e4d8e4bp-1,
    -0x1.04b09e98dcdb4p0
  },
  { // Entry 793
    0x1.b3d4c98a318fb66f821d7286ae7dce7bp-1,
    0x1.04b1465e89226p0
  },
  { // Entry 794
    -0x1.b3d4c98a318fb66f821d7286ae7dce7bp-1,
    -0x1.04b1465e89226p0
  },
  { // Entry 795
    0x1.b3d579a0cb6ee393ff75b58ffe16d13fp-1,
    0x1.04b1ee2435698p0
  },
  { // Entry 796
    -0x1.b3d579a0cb6ee393ff75b58ffe16d13fp-1,
    -0x1.04b1ee2435698p0
  },
  { // Entry 797
    0x1.b3d629b6aa1d9f65aad1a2fc932c8bcbp-1,
    0x1.04b295e9e1b0ap0
  },
  { // Entry 798
    -0x1.b3d629b6aa1d9f65aad1a2fc932c8bcbp-1,
    -0x1.04b295e9e1b0ap0
  },
  { // Entry 799
    0x1.b3d6d9cbcd9b9e43b7fc7fd428a44dd8p-1,
    0x1.04b33daf8df7cp0
  },
  { // Entry 800
    -0x1.b3d6d9cbcd9b9e43b7fc7fd428a44dd8p-1,
    -0x1.04b33daf8df7cp0
  },
  { // Entry 801
    0x1.b3d789e035e8948dab275dfe546c5b08p-1,
    0x1.04b3e5753a3eep0
  },
  { // Entry 802
    -0x1.b3d789e035e8948dab275dfe546c5b08p-1,
    -0x1.04b3e5753a3eep0
  },
  { // Entry 803
    0x1.b3d839f3e30436a358e93cbdcb2bb367p-1,
    0x1.04b48d3ae6860p0
  },
  { // Entry 804
    -0x1.b3d839f3e30436a358e93cbdcb2bb367p-1,
    -0x1.04b48d3ae6860p0
  },
  { // Entry 805
    0x1.b3d8ea06d4ee0684f5741ec777ed88e0p-1,
    0x1.04b5350092ccfp0
  },
  { // Entry 806
    -0x1.b3d8ea06d4ee0684f5741ec777ed88e0p-1,
    -0x1.04b5350092ccfp0
  },
  { // Entry 807
    -0.0,
    -0x1.0p-1074
  },
  { // Entry 808
    0.0,
    0x1.0p-1074
  },
  { // Entry 809
    -0.0,
    -0.0
  },
  { // Entry 810
    0.0,
    0x1.0p-1074
  },
  { // Entry 811
    -0.0,
    -0x1.0p-1074
  },
  { // Entry 812
    0x1.1773d561fd5065d0e9607a728a39eed2p-1,
    0x1.279a74590331bp-1
  },
  { // Entry 813
    -0x1.1773d561fd5065d0e9607a728a39eed2p-1,
    -0x1.279a74590331bp-1
  },
  { // Entry 814
    0x1.1773d561fd507338ff9c088d80c680dbp-1,
    0x1.279a74590331cp-1
  },
  { // Entry 815
    -0x1.1773d561fd507338ff9c088d80c680dbp-1,
    -0x1.279a74590331cp-1
  },
  { // Entry 816
    0x1.1773d561fd5080a115d796a8770d35efp-1,
    0x1.279a74590331dp-1
  },
  { // Entry 817
    -0x1.1773d561fd5080a115d796a8770d35efp-1,
    -0x1.279a74590331dp-1
  },
  { // Entry 818
    0x1.f95b8e7107418c11c94d4a54a9da9b7ap-1,
    0x1.bb67ae8584ca9p0
  },
  { // Entry 819
    -0x1.f95b8e7107418c11c94d4a54a9da9b7ap-1,
    -0x1.bb67ae8584ca9p0
  },
  { // Entry 820
    0x1.f95b8e71074186ee81d5ff89d8fae545p-1,
    0x1.bb67ae8584caap0
  },
  { // Entry 821
    -0x1.f95b8e71074186ee81d5ff89d8fae545p-1,
    -0x1.bb67ae8584caap0
  },
  { // Entry 822
    0x1.f95b8e71074181cb3a5eb4bf0621d381p-1,
    0x1.bb67ae8584cabp0
  },
  { // Entry 823
    -0x1.f95b8e71074181cb3a5eb4bf0621d381p-1,
    -0x1.bb67ae8584cabp0
  },
  { // Entry 824
    0x1.b1d8305321615ac938cff02be9f25085p-2,
    0x1.bffffffffffffp-2
  },
  { // Entry 825
    -0x1.b1d8305321615ac938cff02be9f25085p-2,
    -0x1.bffffffffffffp-2
  },
  { // Entry 826
    0x1.b1d83053216169476f4d1982b9b14ab1p-2,
    0x1.cp-2
  },
  { // Entry 827
    -0x1.b1d83053216169476f4d1982b9b14ab1p-2,
    -0x1.cp-2
  },
  { // Entry 828
    0x1.b1d83053216177c5a5ca42d98955275ap-2,
    0x1.c000000000001p-2
  },
  { // Entry 829
    -0x1.b1d83053216177c5a5ca42d98955275ap-2,
    -0x1.c000000000001p-2
  },
  { // Entry 830
    0x1.44eb381cf3869ea71ccb36863e4ea65bp-1,
    0x1.5ffffffffffffp-1
  },
  { // Entry 831
    -0x1.44eb381cf3869ea71ccb36863e4ea65bp-1,
    -0x1.5ffffffffffffp-1
  },
  { // Entry 832
    0x1.44eb381cf386ab04a4f8656abea80b83p-1,
    0x1.6p-1
  },
  { // Entry 833
    -0x1.44eb381cf386ab04a4f8656abea80b83p-1,
    -0x1.6p-1
  },
  { // Entry 834
    0x1.44eb381cf386b7622d25944f3eb035dcp-1,
    0x1.6000000000001p-1
  },
  { // Entry 835
    -0x1.44eb381cf386b7622d25944f3eb035dcp-1,
    -0x1.6000000000001p-1
  },
  { // Entry 836
    0x1.dad902fa8ac864fd8afa0bdc609ded19p-1,
    0x1.2ffffffffffffp0
  },
  { // Entry 837
    -0x1.dad902fa8ac864fd8afa0bdc609ded19p-1,
    -0x1.2ffffffffffffp0
  },
  { // Entry 838
    0x1.dad902fa8ac870f52f1b843ac83bc3edp-1,
    0x1.3p0
  },
  { // Entry 839
    -0x1.dad902fa8ac870f52f1b843ac83bc3edp-1,
    -0x1.3p0
  },
  { // Entry 840
    0x1.dad902fa8ac87cecd33cfc992dfec1bep-1,
    0x1.3000000000001p0
  },
  { // Entry 841
    -0x1.dad902fa8ac87cecd33cfc992dfec1bep-1,
    -0x1.3000000000001p0
  },
  { // Entry 842
    0x1.4b707a7acdecf90a188d0230fad3ad58p-1,
    0x1.37fffffffffffp1
  },
  { // Entry 843
    -0x1.4b707a7acdecf90a188d0230fad3ad58p-1,
    -0x1.37fffffffffffp1
  },
  { // Entry 844
    0x1.4b707a7acdecc84239463e78b312fa10p-1,
    0x1.380p1
  },
  { // Entry 845
    -0x1.4b707a7acdecc84239463e78b312fa10p-1,
    -0x1.380p1
  },
  { // Entry 846
    0x1.4b707a7acdec977a59ff7ac0662484ddp-1,
    0x1.3800000000001p1
  },
  { // Entry 847
    -0x1.4b707a7acdec977a59ff7ac0662484ddp-1,
    -0x1.3800000000001p1
  },
  { // Entry 848
    0x1.066e7eb76f5c6678fd8325a81f1925c6p-4,
    0x1.069c8b46b3792p-4
  },
  { // Entry 849
    -0x1.066e7eb76f5c6678fd8325a81f1925c6p-4,
    -0x1.069c8b46b3792p-4
  },
  { // Entry 850
    0x1.05e4761ab8d8f0a7dba834000f236650p-3,
    0x1.069c8b46b3792p-3
  },
  { // Entry 851
    -0x1.05e4761ab8d8f0a7dba834000f236650p-3,
    -0x1.069c8b46b3792p-3
  },
  { // Entry 852
    0x1.877e2cd4f6fd9ba498e327053032734fp-3,
    0x1.89ead0ea0d35bp-3
  },
  { // Entry 853
    -0x1.877e2cd4f6fd9ba498e327053032734fp-3,
    -0x1.89ead0ea0d35bp-3
  },
  { // Entry 854
    0x1.03be06f97cbee47698539f977cadbe7ep-2,
    0x1.069c8b46b3792p-2
  },
  { // Entry 855
    -0x1.03be06f97cbee47698539f977cadbe7ep-2,
    -0x1.069c8b46b3792p-2
  },
  { // Entry 856
    0x1.42abba8c72fbb8ca96f79aa4bb03584ep-2,
    0x1.4843ae1860576p-2
  },
  { // Entry 857
    -0x1.42abba8c72fbb8ca96f79aa4bb03584ep-2,
    -0x1.4843ae1860576p-2
  },
  { // Entry 858
    0x1.8045fe64e62dc3d686d976d7d5a7c689p-2,
    0x1.89ead0ea0d35ap-2
  },
  { // Entry 859
    -0x1.8045fe64e62dc3d686d976d7d5a7c689p-2,
    -0x1.89ead0ea0d35ap-2
  },
  { // Entry 860
    0x1.bc4c04d71abbeea5ab064ecfbf54c613p-2,
    0x1.cb91f3bbba13ep-2
  },
  { // Entry 861
    -0x1.bc4c04d71abbeea5ab064ecfbf54c613p-2,
    -0x1.cb91f3bbba13ep-2
  },
  { // Entry 862
    0x1.f67ea975b86a01510e6bde3778138934p-2,
    0x1.069c8b46b3791p-1
  },
  { // Entry 863
    -0x1.f67ea975b86a01510e6bde3778138934p-2,
    -0x1.069c8b46b3791p-1
  },
  { // Entry 864
    0x1.175059bf0d42524ecb0bf4243b55973dp-1,
    0x1.27701caf89e83p-1
  },
  { // Entry 865
    -0x1.175059bf0d42524ecb0bf4243b55973dp-1,
    -0x1.27701caf89e83p-1
  },
  { // Entry 866
    0x1.323b8b1fb4ba21dd12cce820e156a4fcp-1,
    0x1.4843ae1860575p-1
  },
  { // Entry 867
    -0x1.323b8b1fb4ba21dd12cce820e156a4fcp-1,
    -0x1.4843ae1860575p-1
  },
  { // Entry 868
    0x1.4be4979c5efb306c1a77024032849b52p-1,
    0x1.69173f8136c67p-1
  },
  { // Entry 869
    -0x1.4be4979c5efb306c1a77024032849b52p-1,
    -0x1.69173f8136c67p-1
  },
  { // Entry 870
    0x1.643080d67acc1332c64a85612cacafb9p-1,
    0x1.89ead0ea0d359p-1
  },
  { // Entry 871
    -0x1.643080d67acc1332c64a85612cacafb9p-1,
    -0x1.89ead0ea0d359p-1
  },
  { // Entry 872
    0x1.7b05b7b6c612e5b08d5efe49a46e21a1p-1,
    0x1.aabe6252e3a4bp-1
  },
  { // Entry 873
    -0x1.7b05b7b6c612e5b08d5efe49a46e21a1p-1,
    -0x1.aabe6252e3a4bp-1
  },
  { // Entry 874
    0x1.904c37505de48fa8e76287960fd44594p-1,
    0x1.cb91f3bbba13dp-1
  },
  { // Entry 875
    -0x1.904c37505de48fa8e76287960fd44594p-1,
    -0x1.cb91f3bbba13dp-1
  },
  { // Entry 876
    0x1.a3ed9e252938a14c79c575639c15a91dp-1,
    0x1.ec6585249082fp-1
  },
  { // Entry 877
    -0x1.a3ed9e252938a14c79c575639c15a91dp-1,
    -0x1.ec6585249082fp-1
  },
  { // Entry 878
    0x1.b5d545b109bf935594036798cf40c9b0p-1,
    0x1.069c8b46b3791p0
  },
  { // Entry 879
    -0x1.b5d545b109bf935594036798cf40c9b0p-1,
    -0x1.069c8b46b3791p0
  },
  { // Entry 880
    0x1.c5f058230e7fd14d3e5e315349f699efp-1,
    0x1.170653fb1eb0ap0
  },
  { // Entry 881
    -0x1.c5f058230e7fd14d3e5e315349f699efp-1,
    -0x1.170653fb1eb0ap0
  },
  { // Entry 882
    0x1.d42de42dce1346a03d1f6abf0eba9022p-1,
    0x1.27701caf89e83p0
  },
  { // Entry 883
    -0x1.d42de42dce1346a03d1f6abf0eba9022p-1,
    -0x1.27701caf89e83p0
  },
  { // Entry 884
    0x1.e07eeeda109cb504afcca860d4b5dd32p-1,
    0x1.37d9e563f51fcp0
  },
  { // Entry 885
    -0x1.e07eeeda109cb504afcca860d4b5dd32p-1,
    -0x1.37d9e563f51fcp0
  },
  { // Entry 886
    0x1.ead6834909b93371faf3beaddbd60eddp-1,
    0x1.4843ae1860575p0
  },
  { // Entry 887
    -0x1.ead6834909b93371faf3beaddbd60eddp-1,
    -0x1.4843ae1860575p0
  },
  { // Entry 888
    0x1.f329c0558e967e4cab58d0fa572d62d2p-1,
    0x1.58ad76cccb8eep0
  },
  { // Entry 889
    -0x1.f329c0558e967e4cab58d0fa572d62d2p-1,
    -0x1.58ad76cccb8eep0
  },
  { // Entry 890
    0x1.f96fe405f1ac5dc9cf343508067bfcaep-1,
    0x1.69173f8136c67p0
  },
  { // Entry 891
    -0x1.f96fe405f1ac5dc9cf343508067bfcaep-1,
    -0x1.69173f8136c67p0
  },
  { // Entry 892
    0x1.fda254c27a01f4786c149d6a7779cc3ap-1,
    0x1.79810835a1fe0p0
  },
  { // Entry 893
    -0x1.fda254c27a01f4786c149d6a7779cc3ap-1,
    -0x1.79810835a1fe0p0
  },
  { // Entry 894
    0x1.ffbca846c4fc997f1a381420208884e0p-1,
    0x1.89ead0ea0d359p0
  },
  { // Entry 895
    -0x1.ffbca846c4fc997f1a381420208884e0p-1,
    -0x1.89ead0ea0d359p0
  },
  { // Entry 896
    0x1.ffbca846c4fc9f30bfb458ef2091c8eep-1,
    0x1.9a54999e786d2p0
  },
  { // Entry 897
    -0x1.ffbca846c4fc9f30bfb458ef2091c8eep-1,
    -0x1.9a54999e786d2p0
  },
  { // Entry 898
    0x1.fda254c27a0205875f271435f827160cp-1,
    0x1.aabe6252e3a4bp0
  },
  { // Entry 899
    -0x1.fda254c27a0205875f271435f827160cp-1,
    -0x1.aabe6252e3a4bp0
  },
  { // Entry 900
    0x1.f96fe405f1ac7a241e02e58b0cbf3ae7p-1,
    0x1.bb282b074edc4p0
  },
  { // Entry 901
    -0x1.f96fe405f1ac7a241e02e58b0cbf3ae7p-1,
    -0x1.bb282b074edc4p0
  },
  { // Entry 902
    0x1.f329c0558e96a5d48272ad4c49ec53b8p-1,
    0x1.cb91f3bbba13dp0
  },
  { // Entry 903
    -0x1.f329c0558e96a5d48272ad4c49ec53b8p-1,
    -0x1.cb91f3bbba13dp0
  },
  { // Entry 904
    0x1.ead6834909b965fdc4b0ceffc0f285c6p-1,
    0x1.dbfbbc70254b6p0
  },
  { // Entry 905
    -0x1.ead6834909b965fdc4b0ceffc0f285c6p-1,
    -0x1.dbfbbc70254b6p0
  },
  { // Entry 906
    0x1.e07eeeda109cf25f400cd5f46acec887p-1,
    0x1.ec6585249082fp0
  },
  { // Entry 907
    -0x1.e07eeeda109cf25f400cd5f46acec887p-1,
    -0x1.ec6585249082fp0
  },
  { // Entry 908
    0x1.d42de42dce138e890939e56c439ded90p-1,
    0x1.fccf4dd8fbba8p0
  },
  { // Entry 909
    -0x1.d42de42dce138e890939e56c439ded90p-1,
    -0x1.fccf4dd8fbba8p0
  },
  { // Entry 910
    0x1.c5f058230e8014ab83ece0c3a638c079p-1,
    0x1.069c8b46b3791p1
  },
  { // Entry 911
    -0x1.c5f058230e8014ab83ece0c3a638c079p-1,
    -0x1.069c8b46b3791p1
  },
  { // Entry 912
    0x1.b5d545b109bfce3fc4d77001afe2f2b6p-1,
    0x1.0ed16fa0e914ep1
  },
  { // Entry 913
    -0x1.b5d545b109bfce3fc4d77001afe2f2b6p-1,
    -0x1.0ed16fa0e914ep1
  },
  { // Entry 914
    0x1.a3ed9e252938d92a5553b3c09d2bddd3p-1,
    0x1.170653fb1eb0bp1
  },
  { // Entry 915
    -0x1.a3ed9e252938d92a5553b3c09d2bddd3p-1,
    -0x1.170653fb1eb0bp1
  },
  { // Entry 916
    0x1.904c37505de4b8975dd2730e196ddfc3p-1,
    0x1.1f3b3855544c8p1
  },
  { // Entry 917
    -0x1.904c37505de4b8975dd2730e196ddfc3p-1,
    -0x1.1f3b3855544c8p1
  },
  { // Entry 918
    0x1.7b05b7b6c612fc4fda3812b1f1348389p-1,
    0x1.27701caf89e85p1
  },
  { // Entry 919
    -0x1.7b05b7b6c612fc4fda3812b1f1348389p-1,
    -0x1.27701caf89e85p1
  },
  { // Entry 920
    0x1.643080d67acc14620672dda6241ea305p-1,
    0x1.2fa50109bf842p1
  },
  { // Entry 921
    -0x1.643080d67acc14620672dda6241ea305p-1,
    -0x1.2fa50109bf842p1
  },
  { // Entry 922
    0x1.4be4979c5efb194fc82ac367fedf93bcp-1,
    0x1.37d9e563f51ffp1
  },
  { // Entry 923
    -0x1.4be4979c5efb194fc82ac367fedf93bcp-1,
    -0x1.37d9e563f51ffp1
  },
  { // Entry 924
    0x1.323b8b1fb4b9efe5075ede8049a85c3dp-1,
    0x1.400ec9be2abbcp1
  },
  { // Entry 925
    -0x1.323b8b1fb4b9efe5075ede8049a85c3dp-1,
    -0x1.400ec9be2abbcp1
  },
  { // Entry 926
    0x1.175059bf0d42033bbcf598c88b176e61p-1,
    0x1.4843ae1860579p1
  },
  { // Entry 927
    -0x1.175059bf0d42033bbcf598c88b176e61p-1,
    -0x1.4843ae1860579p1
  },
  { // Entry 928
    0x1.f67ea975b8692521f77d6754b302c5c4p-2,
    0x1.5078927295f36p1
  },
  { // Entry 929
    -0x1.f67ea975b8692521f77d6754b302c5c4p-2,
    -0x1.5078927295f36p1
  },
  { // Entry 930
    0x1.bc4c04d71abad14efc29a66342ada723p-2,
    0x1.58ad76cccb8f3p1
  },
  { // Entry 931
    -0x1.bc4c04d71abad14efc29a66342ada723p-2,
    -0x1.58ad76cccb8f3p1
  },
  { // Entry 932
    0x1.8045fe64e62c62f57f077ea251e2f2dcp-2,
    0x1.60e25b27012b0p1
  },
  { // Entry 933
    -0x1.8045fe64e62c62f57f077ea251e2f2dcp-2,
    -0x1.60e25b27012b0p1
  },
  { // Entry 934
    0x1.42abba8c72fa12be920b316627512e41p-2,
    0x1.69173f8136c6dp1
  },
  { // Entry 935
    -0x1.42abba8c72fa12be920b316627512e41p-2,
    -0x1.69173f8136c6dp1
  },
  { // Entry 936
    0x1.03be06f97cbcf866021e5a5c62c6b07ep-2,
    0x1.714c23db6c62ap1
  },
  { // Entry 937
    -0x1.03be06f97cbcf866021e5a5c62c6b07ep-2,
    -0x1.714c23db6c62ap1
  },
  { // Entry 938
    0x1.877e2cd4f6f94710f2776775b01c73dbp-3,
    0x1.79810835a1fe7p1
  },
  { // Entry 939
    -0x1.877e2cd4f6f94710f2776775b01c73dbp-3,
    -0x1.79810835a1fe7p1
  },
  { // Entry 940
    0x1.05e4761ab8d421719567717f76712867p-3,
    0x1.81b5ec8fd79a4p1
  },
  { // Entry 941
    -0x1.05e4761ab8d421719567717f76712867p-3,
    -0x1.81b5ec8fd79a4p1
  },
  { // Entry 942
    0x1.066e7eb76f5dd2ea19b6991e8a1a3634p-4,
    0x1.89ead0ea0d35bp1
  },
  { // Entry 943
    -0x1.066e7eb76f5dd2ea19b6991e8a1a3634p-4,
    -0x1.89ead0ea0d35bp1
  },
  { // Entry 944
    0x1.03be06f97cbf09cc0badbdae803d7b4ep-2,
    -0x1.81b5ec8fd799fp2
  },
  { // Entry 945
    -0x1.03be06f97cbf09cc0badbdae803d7b4ep-2,
    0x1.81b5ec8fd799fp2
  },
  { // Entry 946
    0x1.f67ea975b86a22f2348778824f95d84ap-2,
    -0x1.714c23db6c626p2
  },
  { // Entry 947
    -0x1.f67ea975b86a22f2348778824f95d84ap-2,
    0x1.714c23db6c626p2
  },
  { // Entry 948
    0x1.643080d67acc210fa27e9247a8286220p-1,
    -0x1.60e25b27012adp2
  },
  { // Entry 949
    -0x1.643080d67acc210fa27e9247a8286220p-1,
    0x1.60e25b27012adp2
  },
  { // Entry 950
    0x1.b5d545b109bf950b419702972b94f8fap-1,
    -0x1.5078927295f34p2
  },
  { // Entry 951
    -0x1.b5d545b109bf950b419702972b94f8fap-1,
    0x1.5078927295f34p2
  },
  { // Entry 952
    0x1.ead6834909b9346234dbb601d0486cf2p-1,
    -0x1.400ec9be2abbbp2
  },
  { // Entry 953
    -0x1.ead6834909b9346234dbb601d0486cf2p-1,
    0x1.400ec9be2abbbp2
  },
  { // Entry 954
    0x1.ffbca846c4fc999a29dc1d6b2d7cb413p-1,
    -0x1.2fa50109bf842p2
  },
  { // Entry 955
    -0x1.ffbca846c4fc999a29dc1d6b2d7cb413p-1,
    0x1.2fa50109bf842p2
  },
  { // Entry 956
    0x1.f329c0558e96a518a2af3ae7800a5b65p-1,
    -0x1.1f3b3855544c9p2
  },
  { // Entry 957
    -0x1.f329c0558e96a518a2af3ae7800a5b65p-1,
    0x1.1f3b3855544c9p2
  },
  { // Entry 958
    0x1.c5f058230e8021f21bd0ac2c0f6809a9p-1,
    -0x1.0ed16fa0e9150p2
  },
  { // Entry 959
    -0x1.c5f058230e8021f21bd0ac2c0f6809a9p-1,
    0x1.0ed16fa0e9150p2
  },
  { // Entry 960
    0x1.7b05b7b6c61365a9ac9e908b8e5d3ce4p-1,
    -0x1.fccf4dd8fbbaep1
  },
  { // Entry 961
    -0x1.7b05b7b6c61365a9ac9e908b8e5d3ce4p-1,
    0x1.fccf4dd8fbbaep1
  },
  { // Entry 962
    0x1.175059bf0d42f1d6b391f07f96f2353dp-1,
    -0x1.dbfbbc70254bcp1
  },
  { // Entry 963
    -0x1.175059bf0d42f1d6b391f07f96f2353dp-1,
    0x1.dbfbbc70254bcp1
  },
  { // Entry 964
    0x1.42abba8c72fd22194793246b8d19960ap-2,
    -0x1.bb282b074edcap1
  },
  { // Entry 965
    -0x1.42abba8c72fd22194793246b8d19960ap-2,
    0x1.bb282b074edcap1
  },
  { // Entry 966
    0x1.066e7eb76f62b5f4563de26dca890017p-4,
    -0x1.9a54999e786d8p1
  },
  { // Entry 967
    -0x1.066e7eb76f62b5f4563de26dca890017p-4,
    0x1.9a54999e786d8p1
  },
  { // Entry 968
    -0x1.877e2cd4f6fa42586875c5250a169e48p-3,
    -0x1.79810835a1fe6p1
  },
  { // Entry 969
    0x1.877e2cd4f6fa42586875c5250a169e48p-3,
    0x1.79810835a1fe6p1
  },
  { // Entry 970
    -0x1.bc4c04d71aba5dfc098278f168bbd962p-2,
    -0x1.58ad76cccb8f4p1
  },
  { // Entry 971
    0x1.bc4c04d71aba5dfc098278f168bbd962p-2,
    0x1.58ad76cccb8f4p1
  },
  { // Entry 972
    -0x1.4be4979c5efa871d30ae1cfa66389199p-1,
    -0x1.37d9e563f5202p1
  },
  { // Entry 973
    0x1.4be4979c5efa871d30ae1cfa66389199p-1,
    0x1.37d9e563f5202p1
  },
  { // Entry 974
    -0x1.a3ed9e25293822168958cce1e09f7c11p-1,
    -0x1.170653fb1eb10p1
  },
  { // Entry 975
    0x1.a3ed9e25293822168958cce1e09f7c11p-1,
    0x1.170653fb1eb10p1
  },
  { // Entry 976
    -0x1.e07eeeda109c62b340dc36e92169648dp-1,
    -0x1.ec6585249083cp0
  },
  { // Entry 977
    0x1.e07eeeda109c62b340dc36e92169648dp-1,
    0x1.ec6585249083cp0
  },
  { // Entry 978
    -0x1.fda254c27a01dd954db3aea505e49453p-1,
    -0x1.aabe6252e3a58p0
  },
  { // Entry 979
    0x1.fda254c27a01dd954db3aea505e49453p-1,
    0x1.aabe6252e3a58p0
  },
  { // Entry 980
    -0x1.f96fe405f1aca02e8f4fd433e59aa973p-1,
    -0x1.69173f8136c74p0
  },
  { // Entry 981
    0x1.f96fe405f1aca02e8f4fd433e59aa973p-1,
    0x1.69173f8136c74p0
  },
  { // Entry 982
    -0x1.d42de42dce13ef040bb1040e3148d7dep-1,
    -0x1.27701caf89e90p0
  },
  { // Entry 983
    0x1.d42de42dce13ef040bb1040e3148d7dep-1,
    0x1.27701caf89e90p0
  },
  { // Entry 984
    -0x1.904c37505de5930812e3a2a94feaa51bp-1,
    -0x1.cb91f3bbba157p-1
  },
  { // Entry 985
    0x1.904c37505de5930812e3a2a94feaa51bp-1,
    0x1.cb91f3bbba157p-1
  },
  { // Entry 986
    -0x1.323b8b1fb4bb626dd40cacd74963ac6cp-1,
    -0x1.4843ae186058ep-1
  },
  { // Entry 987
    0x1.323b8b1fb4bb626dd40cacd74963ac6cp-1,
    0x1.4843ae186058ep-1
  },
  { // Entry 988
    -0x1.8045fe64e6308bb5c6ce35f834b93c63p-2,
    -0x1.89ead0ea0d38ap-2
  },
  { // Entry 989
    0x1.8045fe64e6308bb5c6ce35f834b93c63p-2,
    0x1.89ead0ea0d38ap-2
  },
  { // Entry 990
    -0x1.05e4761ab8dec44ed0fa30d335049c40p-3,
    -0x1.069c8b46b37f0p-3
  },
  { // Entry 991
    0x1.05e4761ab8dec44ed0fa30d335049c40p-3,
    0x1.069c8b46b37f0p-3
  },
  { // Entry 992
    0x1.05e4761ab8d31d00e656372c5c04aa6ep-3,
    0x1.069c8b46b3734p-3
  },
  { // Entry 993
    -0x1.05e4761ab8d31d00e656372c5c04aa6ep-3,
    -0x1.069c8b46b3734p-3
  },
  { // Entry 994
    0x1.8045fe64e62b19a094399502afb76e5cp-2,
    0x1.89ead0ea0d32cp-2
  },
  { // Entry 995
    -0x1.8045fe64e62b19a094399502afb76e5cp-2,
    -0x1.89ead0ea0d32cp-2
  },
  { // Entry 996
    0x1.323b8b1fb4b907c416d23b04e0ec0e72p-1,
    0x1.4843ae186055fp-1
  },
  { // Entry 997
    -0x1.323b8b1fb4b907c416d23b04e0ec0e72p-1,
    -0x1.4843ae186055fp-1
  },
  { // Entry 998
    0x1.904c37505de3be2ace17ca5487750231p-1,
    0x1.cb91f3bbba128p-1
  },
  { // Entry 999
    -0x1.904c37505de3be2ace17ca5487750231p-1,
    -0x1.cb91f3bbba128p-1
  },
  { // Entry 1000
    0x1.d42de42dce12b82466f2fcb63b294751p-1,
    0x1.27701caf89e78p0
  },
  { // Entry 1001
    -0x1.d42de42dce12b82466f2fcb63b294751p-1,
    -0x1.27701caf89e78p0
  },
  { // Entry 1002
    0x1.f96fe405f1ac259bf192fd1cf64e2f12p-1,
    0x1.69173f8136c5cp0
  },
  { // Entry 1003
    -0x1.f96fe405f1ac259bf192fd1cf64e2f12p-1,
    -0x1.69173f8136c5cp0
  },
  { // Entry 1004
    0x1.fda254c27a02275432d77dd6f9704644p-1,
    0x1.aabe6252e3a40p0
  },
  { // Entry 1005
    -0x1.fda254c27a02275432d77dd6f9704644p-1,
    -0x1.aabe6252e3a40p0
  },
  { // Entry 1006
    0x1.e07eeeda109d6bf0c935fa10b1280c6dp-1,
    0x1.ec65852490824p0
  },
  { // Entry 1007
    -0x1.e07eeeda109d6bf0c935fa10b1280c6dp-1,
    -0x1.ec65852490824p0
  },
  { // Entry 1008
    0x1.a3ed9e252939d9793fb2f6f75e5c76e7p-1,
    0x1.170653fb1eb04p1
  },
  { // Entry 1009
    -0x1.a3ed9e252939d9793fb2f6f75e5c76e7p-1,
    -0x1.170653fb1eb04p1
  },
  { // Entry 1010
    0x1.4be4979c5efccfe78ea0b6afb0cbba37p-1,
    0x1.37d9e563f51f6p1
  },
  { // Entry 1011
    -0x1.4be4979c5efccfe78ea0b6afb0cbba37p-1,
    -0x1.37d9e563f51f6p1
  },
  { // Entry 1012
    0x1.bc4c04d71abfc5df69589a45d5e3196ep-2,
    0x1.58ad76cccb8e8p1
  },
  { // Entry 1013
    -0x1.bc4c04d71abfc5df69589a45d5e3196ep-2,
    -0x1.58ad76cccb8e8p1
  },
  { // Entry 1014
    0x1.877e2cd4f70609b1f062295b64aed4bdp-3,
    0x1.79810835a1fdap1
  },
  { // Entry 1015
    -0x1.877e2cd4f70609b1f062295b64aed4bdp-3,
    -0x1.79810835a1fdap1
  },
  { // Entry 1016
    -0x1.066e7eb76f4ac293f46486dc328d450bp-4,
    0x1.9a54999e786ccp1
  },
  { // Entry 1017
    0x1.066e7eb76f4ac293f46486dc328d450bp-4,
    -0x1.9a54999e786ccp1
  },
  { // Entry 1018
    -0x1.42abba8c72f770595ffe3135a0e0ad83p-2,
    0x1.bb282b074edbep1
  },
  { // Entry 1019
    0x1.42abba8c72f770595ffe3135a0e0ad83p-2,
    -0x1.bb282b074edbep1
  },
  { // Entry 1020
    -0x1.175059bf0d406e2fe014e880dd29cfacp-1,
    0x1.dbfbbc70254b0p1
  },
  { // Entry 1021
    0x1.175059bf0d406e2fe014e880dd29cfacp-1,
    -0x1.dbfbbc70254b0p1
  },
  { // Entry 1022
    -0x1.7b05b7b6c6116155f0dc551e316e1e0bp-1,
    0x1.fccf4dd8fbba2p1
  },
  { // Entry 1023
    0x1.7b05b7b6c6116155f0dc551e316e1e0bp-1,
    -0x1.fccf4dd8fbba2p1
  },
  { // Entry 1024
    -0x1.c5f058230e7ebeb7616779e16fa9b537p-1,
    0x1.0ed16fa0e914ap2
  },
  { // Entry 1025
    0x1.c5f058230e7ebeb7616779e16fa9b537p-1,
    -0x1.0ed16fa0e914ap2
  },
  { // Entry 1026
    -0x1.f329c0558e95fa333d5d2d44d654777cp-1,
    0x1.1f3b3855544c3p2
  },
  { // Entry 1027
    0x1.f329c0558e95fa333d5d2d44d654777cp-1,
    -0x1.1f3b3855544c3p2
  },
  { // Entry 1028
    -0x1.ffbca846c4fcb237c2947b35b037a2p-1,
    0x1.2fa50109bf83cp2
  },
  { // Entry 1029
    0x1.ffbca846c4fcb237c2947b35b037a2p-1,
    -0x1.2fa50109bf83cp2
  },
  { // Entry 1030
    -0x1.ead6834909ba0ee69b31e1970df1bb8bp-1,
    0x1.400ec9be2abb5p2
  },
  { // Entry 1031
    0x1.ead6834909ba0ee69b31e1970df1bb8bp-1,
    -0x1.400ec9be2abb5p2
  },
  { // Entry 1032
    -0x1.b5d545b109c1232b61dd28d8035d95cbp-1,
    0x1.5078927295f2ep2
  },
  { // Entry 1033
    0x1.b5d545b109c1232b61dd28d8035d95cbp-1,
    -0x1.5078927295f2ep2
  },
  { // Entry 1034
    -0x1.643080d67ace48c0dd1fe3a06bbc4bf5p-1,
    0x1.60e25b27012a7p2
  },
  { // Entry 1035
    0x1.643080d67ace48c0dd1fe3a06bbc4bf5p-1,
    -0x1.60e25b27012a7p2
  },
  { // Entry 1036
    -0x1.f67ea975b86f5d4aa92716cc077473a7p-2,
    0x1.714c23db6c620p2
  },
  { // Entry 1037
    0x1.f67ea975b86f5d4aa92716cc077473a7p-2,
    -0x1.714c23db6c620p2
  },
  { // Entry 1038
    -0x1.03be06f97cc4d78fdccbca1d40e86011p-2,
    0x1.81b5ec8fd7999p2
  },
  { // Entry 1039
    0x1.03be06f97cc4d78fdccbca1d40e86011p-2,
    -0x1.81b5ec8fd7999p2
  },
  { // Entry 1040
    0x1.efb26ef930c4c3fa3245963c1dcec0a6p-5,
    0x1.effffffffffffp-5
  },
  { // Entry 1041
    -0x1.efb26ef930c4c3fa3245963c1dcec0a6p-5,
    -0x1.effffffffffffp-5
  },
  { // Entry 1042
    0x1.efb26ef930c4d3f2b0dbe1931ba5ae64p-5,
    0x1.fp-5
  },
  { // Entry 1043
    -0x1.efb26ef930c4d3f2b0dbe1931ba5ae64p-5,
    -0x1.fp-5
  },
  { // Entry 1044
    0x1.efb26ef930c4e3eb2f722cea197c2036p-5,
    0x1.f000000000001p-5
  },
  { // Entry 1045
    -0x1.efb26ef930c4e3eb2f722cea197c2036p-5,
    -0x1.f000000000001p-5
  },
  { // Entry 1046
    0x1.f6baaa131de633ad4e0e7d6465d12a05p-4,
    0x1.f7fffffffffffp-4
  },
  { // Entry 1047
    -0x1.f6baaa131de633ad4e0e7d6465d12a05p-4,
    -0x1.f7fffffffffffp-4
  },
  { // Entry 1048
    0x1.f6baaa131de6438e5611279864fe7663p-4,
    0x1.f80p-4
  },
  { // Entry 1049
    -0x1.f6baaa131de6438e5611279864fe7663p-4,
    -0x1.f80p-4
  },
  { // Entry 1050
    0x1.f6baaa131de6536f5e13d1cc6429cc07p-4,
    0x1.f800000000001p-4
  },
  { // Entry 1051
    -0x1.f6baaa131de6536f5e13d1cc6429cc07p-4,
    -0x1.f800000000001p-4
  },
  { // Entry 1052
    0x1.4a8c3b4e9c7ff00a36e061a0d2295093p-3,
    0x1.4bfffffffffffp-3
  },
  { // Entry 1053
    -0x1.4a8c3b4e9c7ff00a36e061a0d2295093p-3,
    -0x1.4bfffffffffffp-3
  },
  { // Entry 1054
    0x1.4a8c3b4e9c7fffd48305f44a42f5f50fp-3,
    0x1.4c0p-3
  },
  { // Entry 1055
    -0x1.4a8c3b4e9c7fffd48305f44a42f5f50fp-3,
    -0x1.4c0p-3
  },
  { // Entry 1056
    0x1.4a8c3b4e9c800f9ecf2b86f3b3bd6f5ap-3,
    0x1.4c00000000001p-3
  },
  { // Entry 1057
    -0x1.4a8c3b4e9c800f9ecf2b86f3b3bd6f5ap-3,
    -0x1.4c00000000001p-3
  },
  { // Entry 1058
    0x1.2e9cd95baba325fe6067233d4496aaacp-2,
    0x1.3333333333332p-2
  },
  { // Entry 1059
    -0x1.2e9cd95baba325fe6067233d4496aaacp-2,
    -0x1.3333333333332p-2
  },
  { // Entry 1060
    0x1.2e9cd95baba335476f513ac221d078c7p-2,
    0x1.3333333333333p-2
  },
  { // Entry 1061
    -0x1.2e9cd95baba335476f513ac221d078c7p-2,
    -0x1.3333333333333p-2
  },
  { // Entry 1062
    0x1.2e9cd95baba344907e3b5246fef75d15p-2,
    0x1.3333333333334p-2
  },
  { // Entry 1063
    -0x1.2e9cd95baba344907e3b5246fef75d15p-2,
    -0x1.3333333333334p-2
  },
  { // Entry 1064
    0x1.3faefc7a5466ef3045c3f1be716ad568p-1,
    0x1.594317acc4ef8p-1
  },
  { // Entry 1065
    -0x1.3faefc7a5466ef3045c3f1be716ad568p-1,
    -0x1.594317acc4ef8p-1
  },
  { // Entry 1066
    0x1.3faefc7a5466fbafbca027b6e8db8c04p-1,
    0x1.594317acc4ef9p-1
  },
  { // Entry 1067
    -0x1.3faefc7a5466fbafbca027b6e8db8c04p-1,
    -0x1.594317acc4ef9p-1
  },
  { // Entry 1068
    0x1.3faefc7a5467082f337c5daf5ffc56e2p-1,
    0x1.594317acc4efap-1
  },
  { // Entry 1069
    -0x1.3faefc7a5467082f337c5daf5ffc56e2p-1,
    -0x1.594317acc4efap-1
  },
  { // Entry 1070
    0x1.6888a4e134b2def5bea51f4cd7d647c6p-1,
    0x1.8ffffffffffffp-1
  },
  { // Entry 1071
    -0x1.6888a4e134b2def5bea51f4cd7d647c6p-1,
    -0x1.8ffffffffffffp-1
  },
  { // Entry 1072
    0x1.6888a4e134b2ea520b226eca8694b3a2p-1,
    0x1.9p-1
  },
  { // Entry 1073
    -0x1.6888a4e134b2ea520b226eca8694b3a2p-1,
    -0x1.9p-1
  },
  { // Entry 1074
    0x1.6888a4e134b2f5ae579fbe4834f8fd55p-1,
    0x1.9000000000001p-1
  },
  { // Entry 1075
    -0x1.6888a4e134b2f5ae579fbe4834f8fd55p-1,
    -0x1.9000000000001p-1
  },
  { // Entry 1076
    -0.0,
    -0x1.0p-1074
  },
  { // Entry 1077
    0.0,
    0x1.0p-1074
  },
  { // Entry 1078
    -0.0,
    -0.0
  },
  { // Entry 1079
    0.0,
    0x1.0p-1074
  },
  { // Entry 1080
    -0.0,
    -0x1.0p-1074
  },
  { // Entry 1081
    0x1.91f65f10dd812a0b95ff71871ab48207p-5,
    0x1.921fb54442d17p-5
  },
  { // Entry 1082
    -0x1.91f65f10dd812a0b95ff71871ab48207p-5,
    -0x1.921fb54442d17p-5
  },
  { // Entry 1083
    0x1.91f65f10dd813a06a6f12e3dd9a6a41ep-5,
    0x1.921fb54442d18p-5
  },
  { // Entry 1084
    -0x1.91f65f10dd813a06a6f12e3dd9a6a41ep-5,
    -0x1.921fb54442d18p-5
  },
  { // Entry 1085
    0x1.91f65f10dd814a01b7e2eaf4989861b7p-5,
    0x1.921fb54442d19p-5
  },
  { // Entry 1086
    -0x1.91f65f10dd814a01b7e2eaf4989861b7p-5,
    -0x1.921fb54442d19p-5
  },
  { // Entry 1087
    0x1.917a6bc29b42a9cd2e787ee00ca8f8e3p-4,
    0x1.921fb54442d17p-4
  },
  { // Entry 1088
    -0x1.917a6bc29b42a9cd2e787ee00ca8f8e3p-4,
    -0x1.921fb54442d17p-4
  },
  { // Entry 1089
    0x1.917a6bc29b42b9b9754a67729f79346bp-4,
    0x1.921fb54442d18p-4
  },
  { // Entry 1090
    -0x1.917a6bc29b42b9b9754a67729f79346bp-4,
    -0x1.921fb54442d18p-4
  },
  { // Entry 1091
    0x1.917a6bc29b42c9a5bc1c50053247de78p-4,
    0x1.921fb54442d19p-4
  },
  { // Entry 1092
    -0x1.917a6bc29b42c9a5bc1c50053247de78p-4,
    -0x1.921fb54442d19p-4
  },
  { // Entry 1093
    0x1.8f8b83c69a60975f98453886f953a0e5p-3,
    0x1.921fb54442d17p-3
  },
  { // Entry 1094
    -0x1.8f8b83c69a60975f98453886f953a0e5p-3,
    -0x1.921fb54442d17p-3
  },
  { // Entry 1095
    0x1.8f8b83c69a60a710e42d3435516ddaccp-3,
    0x1.921fb54442d18p-3
  },
  { // Entry 1096
    -0x1.8f8b83c69a60a710e42d3435516ddaccp-3,
    -0x1.921fb54442d18p-3
  },
  { // Entry 1097
    0x1.8f8b83c69a60b6c230152fe3a981d686p-3,
    0x1.921fb54442d19p-3
  },
  { // Entry 1098
    -0x1.8f8b83c69a60b6c230152fe3a981d686p-3,
    -0x1.921fb54442d19p-3
  },
  { // Entry 1099
    0x1.87de2a6aea961a3e9dedea57fe7e6514p-2,
    0x1.921fb54442d17p-2
  },
  { // Entry 1100
    -0x1.87de2a6aea961a3e9dedea57fe7e6514p-2,
    -0x1.921fb54442d17p-2
  },
  { // Entry 1101
    0x1.87de2a6aea962906d3d5839ea1a5bcf0p-2,
    0x1.921fb54442d18p-2
  },
  { // Entry 1102
    -0x1.87de2a6aea962906d3d5839ea1a5bcf0p-2,
    -0x1.921fb54442d18p-2
  },
  { // Entry 1103
    0x1.87de2a6aea9637cf09bd1ce544b496eap-2,
    0x1.921fb54442d19p-2
  },
  { // Entry 1104
    -0x1.87de2a6aea9637cf09bd1ce544b496eap-2,
    -0x1.921fb54442d19p-2
  },
  { // Entry 1105
    0x1.6a09e667f3bcba99afb0135729457874p-1,
    0x1.921fb54442d17p-1
  },
  { // Entry 1106
    -0x1.6a09e667f3bcba99afb0135729457874p-1,
    -0x1.921fb54442d17p-1
  },
  { // Entry 1107
    0x1.6a09e667f3bcc5e9fee352f50fd3f4e9p-1,
    0x1.921fb54442d18p-1
  },
  { // Entry 1108
    -0x1.6a09e667f3bcc5e9fee352f50fd3f4e9p-1,
    -0x1.921fb54442d18p-1
  },
  { // Entry 1109
    0x1.6a09e667f3bcd13a4e169292f607eee5p-1,
    0x1.921fb54442d19p-1
  },
  { // Entry 1110
    -0x1.6a09e667f3bcd13a4e169292f607eee5p-1,
    -0x1.921fb54442d19p-1
  },
  { // Entry 1111
    0x1.fffffffffffffffffffffffffe5f56ffp-1,
    0x1.921fb54442d17p0
  },
  { // Entry 1112
    -0x1.fffffffffffffffffffffffffe5f56ffp-1,
    -0x1.921fb54442d17p0
  },
  { // Entry 1113
    0x1.ffffffffffffffffffffffffffec8831p-1,
    0x1.921fb54442d18p0
  },
  { // Entry 1114
    -0x1.ffffffffffffffffffffffffffec8831p-1,
    -0x1.921fb54442d18p0
  },
  { // Entry 1115
    0x1.ffffffffffffffffffffffffff79b963p-1,
    0x1.921fb54442d19p0
  },
  { // Entry 1116
    -0x1.ffffffffffffffffffffffffff79b963p-1,
    -0x1.921fb54442d19p0
  },
  { // Entry 1117
    0x1.469898cc51701b839a252049bfaeed42p-51,
    0x1.921fb54442d17p1
  },
  { // Entry 1118
    -0x1.469898cc51701b839a252049bfaeed42p-51,
    -0x1.921fb54442d17p1
  },
  { // Entry 1119
    0x1.1a62633145c06e0e689481270436e2edp-53,
    0x1.921fb54442d18p1
  },
  { // Entry 1120
    -0x1.1a62633145c06e0e689481270436e2edp-53,
    -0x1.921fb54442d18p1
  },
  { // Entry 1121
    -0x1.72cece675d1fc8f8cbb5bf6c7d5bbc5cp-52,
    0x1.921fb54442d19p1
  },
  { // Entry 1122
    0x1.72cece675d1fc8f8cbb5bf6c7d5bbc5cp-52,
    -0x1.921fb54442d19p1
  },
  { // Entry 1123
    -0x1.469898cc51701b839a252049bb87ce1bp-50,
    0x1.921fb54442d17p2
  },
  { // Entry 1124
    0x1.469898cc51701b839a252049bb87ce1bp-50,
    -0x1.921fb54442d17p2
  },
  { // Entry 1125
    -0x1.1a62633145c06e0e68948127040bf003p-52,
    0x1.921fb54442d18p2
  },
  { // Entry 1126
    0x1.1a62633145c06e0e68948127040bf003p-52,
    -0x1.921fb54442d18p2
  },
  { // Entry 1127
    0x1.72cece675d1fc8f8cbb5bf6c7bd6bf4dp-51,
    0x1.921fb54442d19p2
  },
  { // Entry 1128
    -0x1.72cece675d1fc8f8cbb5bf6c7bd6bf4dp-51,
    -0x1.921fb54442d19p2
  },
  { // Entry 1129
    -0x1.469898cc51701b839a252049aaeb5182p-49,
    0x1.921fb54442d17p3
  },
  { // Entry 1130
    0x1.469898cc51701b839a252049aaeb5182p-49,
    -0x1.921fb54442d17p3
  },
  { // Entry 1131
    -0x1.1a62633145c06e0e689481270360245cp-51,
    0x1.921fb54442d18p3
  },
  { // Entry 1132
    0x1.1a62633145c06e0e689481270360245cp-51,
    -0x1.921fb54442d18p3
  },
  { // Entry 1133
    0x1.72cece675d1fc8f8cbb5bf6c75c2cb0ep-50,
    0x1.921fb54442d19p3
  },
  { // Entry 1134
    -0x1.72cece675d1fc8f8cbb5bf6c75c2cb0ep-50,
    -0x1.921fb54442d19p3
  },
  { // Entry 1135
    -0x1.469898cc51701b839a25204968795f1cp-48,
    0x1.921fb54442d17p4
  },
  { // Entry 1136
    0x1.469898cc51701b839a25204968795f1cp-48,
    -0x1.921fb54442d17p4
  },
  { // Entry 1137
    -0x1.1a62633145c06e0e6894812700b0f5bfp-50,
    0x1.921fb54442d18p4
  },
  { // Entry 1138
    0x1.1a62633145c06e0e6894812700b0f5bfp-50,
    -0x1.921fb54442d18p4
  },
  { // Entry 1139
    0x1.72cece675d1fc8f8cbb5bf6c5d72fa11p-49,
    0x1.921fb54442d19p4
  },
  { // Entry 1140
    -0x1.72cece675d1fc8f8cbb5bf6c5d72fa11p-49,
    -0x1.921fb54442d19p4
  },
  { // Entry 1141
    -0x1.469898cc51701b839a2520485eb19584p-47,
    0x1.921fb54442d17p5
  },
  { // Entry 1142
    0x1.469898cc51701b839a2520485eb19584p-47,
    -0x1.921fb54442d17p5
  },
  { // Entry 1143
    -0x1.1a62633145c06e0e68948126f5f43b4cp-49,
    0x1.921fb54442d18p5
  },
  { // Entry 1144
    0x1.1a62633145c06e0e68948126f5f43b4cp-49,
    -0x1.921fb54442d18p5
  },
  { // Entry 1145
    0x1.72cece675d1fc8f8cbb5bf6bfc33b61fp-48,
    0x1.921fb54442d19p5
  },
  { // Entry 1146
    -0x1.72cece675d1fc8f8cbb5bf6bfc33b61fp-48,
    -0x1.921fb54442d19p5
  },
  { // Entry 1147
    -0x1.469898cc51701b839a25204437926f25p-46,
    0x1.921fb54442d17p6
  },
  { // Entry 1148
    0x1.469898cc51701b839a25204437926f25p-46,
    -0x1.921fb54442d17p6
  },
  { // Entry 1149
    -0x1.1a62633145c06e0e68948126cb01517dp-48,
    0x1.921fb54442d18p6
  },
  { // Entry 1150
    0x1.1a62633145c06e0e68948126cb01517dp-48,
    -0x1.921fb54442d18p6
  },
  { // Entry 1151
    0x1.72cece675d1fc8f8cbb5bf6a7736a658p-47,
    0x1.921fb54442d19p6
  },
  { // Entry 1152
    -0x1.72cece675d1fc8f8cbb5bf6a7736a658p-47,
    -0x1.921fb54442d19p6
  },
  { // Entry 1153
    -0x1.469898cc51701b839a2520339b15d5aap-45,
    0x1.921fb54442d17p7
  },
  { // Entry 1154
    0x1.469898cc51701b839a2520339b15d5aap-45,
    -0x1.921fb54442d17p7
  },
  { // Entry 1155
    -0x1.1a62633145c06e0e689481261f35aa43p-47,
    0x1.921fb54442d18p7
  },
  { // Entry 1156
    0x1.1a62633145c06e0e689481261f35aa43p-47,
    -0x1.921fb54442d18p7
  },
  { // Entry 1157
    0x1.72cece675d1fc8f8cbb5bf646342673ap-46,
    0x1.921fb54442d19p7
  },
  { // Entry 1158
    -0x1.72cece675d1fc8f8cbb5bf646342673ap-46,
    -0x1.921fb54442d19p7
  },
  { // Entry 1159
    0x1.6a09e667f3bcffa60c0f53340fd24904p-1,
    0x1.2d97c7f3321d1p1
  },
  { // Entry 1160
    -0x1.6a09e667f3bcffa60c0f53340fd24904p-1,
    -0x1.2d97c7f3321d1p1
  },
  { // Entry 1161
    0x1.6a09e667f3bcd264cf4254bc7ab0c9fap-1,
    0x1.2d97c7f3321d2p1
  },
  { // Entry 1162
    -0x1.6a09e667f3bcd264cf4254bc7ab0c9fap-1,
    -0x1.2d97c7f3321d2p1
  },
  { // Entry 1163
    0x1.6a09e667f3bca52392755644dfe72357p-1,
    0x1.2d97c7f3321d3p1
  },
  { // Entry 1164
    -0x1.6a09e667f3bca52392755644dfe72357p-1,
    -0x1.2d97c7f3321d3p1
  },
  { // Entry 1165
    -0x1.6a09e667f3bc8c2df1b752b606a0a472p-1,
    0x1.f6a7a2955385dp1
  },
  { // Entry 1166
    0x1.6a09e667f3bc8c2df1b752b606a0a472p-1,
    -0x1.f6a7a2955385dp1
  },
  { // Entry 1167
    -0x1.6a09e667f3bcb96f2e84512da488ff2dp-1,
    0x1.f6a7a2955385ep1
  },
  { // Entry 1168
    0x1.6a09e667f3bcb96f2e84512da488ff2dp-1,
    -0x1.f6a7a2955385ep1
  },
  { // Entry 1169
    -0x1.6a09e667f3bce6b06b514fa53cc9324fp-1,
    0x1.f6a7a2955385fp1
  },
  { // Entry 1170
    0x1.6a09e667f3bce6b06b514fa53cc9324fp-1,
    -0x1.f6a7a2955385fp1
  },
  { // Entry 1171
    -0x1.ffffffffffffffffffffffffe8b27b6ap-1,
    0x1.2d97c7f3321d1p2
  },
  { // Entry 1172
    0x1.ffffffffffffffffffffffffe8b27b6ap-1,
    -0x1.2d97c7f3321d1p2
  },
  { // Entry 1173
    -0x1.ffffffffffffffffffffffffff50c9bdp-1,
    0x1.2d97c7f3321d2p2
  },
  { // Entry 1174
    0x1.ffffffffffffffffffffffffff50c9bdp-1,
    -0x1.2d97c7f3321d2p2
  },
  { // Entry 1175
    -0x1.fffffffffffffffffffffffff5ef1810p-1,
    0x1.2d97c7f3321d3p2
  },
  { // Entry 1176
    0x1.fffffffffffffffffffffffff5ef1810p-1,
    -0x1.2d97c7f3321d3p2
  },
  { // Entry 1177
    -0x1.6a09e667f3bd3962193b5373069ba0c2p-1,
    0x1.5fdbbe9bba774p2
  },
  { // Entry 1178
    0x1.6a09e667f3bd3962193b5373069ba0c2p-1,
    -0x1.5fdbbe9bba774p2
  },
  { // Entry 1179
    -0x1.6a09e667f3bcdedf9fa15683e51f7e60p-1,
    0x1.5fdbbe9bba775p2
  },
  { // Entry 1180
    0x1.6a09e667f3bcdedf9fa15683e51f7e60p-1,
    -0x1.5fdbbe9bba775p2
  },
  { // Entry 1181
    -0x1.6a09e667f3bc845d26075994ad02bd98p-1,
    0x1.5fdbbe9bba776p2
  },
  { // Entry 1182
    0x1.6a09e667f3bc845d26075994ad02bd98p-1,
    -0x1.5fdbbe9bba776p2
  },
  { // Entry 1183
    0x1.6a09e667f3bc5271e48b5276f438579fp-1,
    0x1.c463abeccb2bap2
  },
  { // Entry 1184
    -0x1.6a09e667f3bc5271e48b5276f438579fp-1,
    -0x1.c463abeccb2bap2
  },
  { // Entry 1185
    0x1.6a09e667f3bcacf45e254f6638cfe8c6p-1,
    0x1.c463abeccb2bbp2
  },
  { // Entry 1186
    -0x1.6a09e667f3bcacf45e254f6638cfe8c6p-1,
    -0x1.c463abeccb2bbp2
  },
  { // Entry 1187
    0x1.6a09e667f3bd0776d7bf4c5566c6db87p-1,
    0x1.c463abeccb2bcp2
  },
  { // Entry 1188
    -0x1.6a09e667f3bd0776d7bf4c5566c6db87p-1,
    -0x1.c463abeccb2bcp2
  },
  { // Entry 1189
    0x1.ffffffffffffffffffffffffe31174f5p-1,
    0x1.f6a7a2955385dp2
  },
  { // Entry 1190
    -0x1.ffffffffffffffffffffffffe31174f5p-1,
    -0x1.f6a7a2955385dp2
  },
  { // Entry 1191
    0x1.fffffffffffffffffffffffffe194cd4p-1,
    0x1.f6a7a2955385ep2
  },
  { // Entry 1192
    -0x1.fffffffffffffffffffffffffe194cd4p-1,
    -0x1.f6a7a2955385ep2
  },
  { // Entry 1193
    0x1.fffffffffffffffffffffffff92124b4p-1,
    0x1.f6a7a2955385fp2
  },
  { // Entry 1194
    -0x1.fffffffffffffffffffffffff92124b4p-1,
    -0x1.f6a7a2955385fp2
  },
  { // Entry 1195
    0x1.6a09e667f3bdfae1dcce4f18665681c6p-1,
    0x1.1475cc9eedeffp3
  },
  { // Entry 1196
    -0x1.6a09e667f3bdfae1dcce4f18665681c6p-1,
    -0x1.1475cc9eedeffp3
  },
  { // Entry 1197
    0x1.6a09e667f3bd45dce99a553a6d7d8065p-1,
    0x1.1475cc9eedfp3
  },
  { // Entry 1198
    -0x1.6a09e667f3bd45dce99a553a6d7d8065p-1,
    -0x1.1475cc9eedfp3
  },
  { // Entry 1199
    0x1.6a09e667f3bc90d7f6665b5c1a22056ap-1,
    0x1.1475cc9eedf01p3
  },
  { // Entry 1200
    -0x1.6a09e667f3bc90d7f6665b5c1a22056ap-1,
    -0x1.1475cc9eedf01p3
  },
  { // Entry 1201
    0x1.34f272993d1414a2b39bd8373e0d6b94p-49,
    0x1.2d97c7f3321d1p3
  },
  { // Entry 1202
    -0x1.34f272993d1414a2b39bd8373e0d6b94p-49,
    -0x1.2d97c7f3321d1p3
  },
  { // Entry 1203
    0x1.a79394c9e8a0a5159cdec1ba85a688bdp-52,
    0x1.2d97c7f3321d2p3
  },
  { // Entry 1204
    -0x1.a79394c9e8a0a5159cdec1ba85a688bdp-52,
    -0x1.2d97c7f3321d2p3
  },
  { // Entry 1205
    -0x1.961b1acd85d7d6ba98c84f9153c0cc16p-50,
    0x1.2d97c7f3321d3p3
  },
  { // Entry 1206
    0x1.961b1acd85d7d6ba98c84f9153c0cc16p-50,
    -0x1.2d97c7f3321d3p3
  },
  { // Entry 1207
    -0x1.6a09e667f3bb90f220f856d0b1a406c3p-1,
    0x1.46b9c347764a2p3
  },
  { // Entry 1208
    0x1.6a09e667f3bb90f220f856d0b1a406c3p-1,
    -0x1.46b9c347764a2p3
  },
  { // Entry 1209
    -0x1.6a09e667f3bc45f7142c50af84f26c75p-1,
    0x1.46b9c347764a3p3
  },
  { // Entry 1210
    0x1.6a09e667f3bc45f7142c50af84f26c75p-1,
    -0x1.46b9c347764a3p3
  },
  { // Entry 1211
    -0x1.6a09e667f3bcfafc07604a8dfdbe588dp-1,
    0x1.46b9c347764a4p3
  },
  { // Entry 1212
    0x1.6a09e667f3bcfafc07604a8dfdbe588dp-1,
    -0x1.46b9c347764a4p3
  },
  { // Entry 1213
    -0x1.ffffffffffffffffffffffff9d634e9fp-1,
    0x1.5fdbbe9bba774p3
  },
  { // Entry 1214
    0x1.ffffffffffffffffffffffff9d634e9fp-1,
    -0x1.5fdbbe9bba774p3
  },
  { // Entry 1215
    -0x1.fffffffffffffffffffffffffc461178p-1,
    0x1.5fdbbe9bba775p3
  },
  { // Entry 1216
    0x1.fffffffffffffffffffffffffc461178p-1,
    -0x1.5fdbbe9bba775p3
  },
  { // Entry 1217
    -0x1.ffffffffffffffffffffffffdb28d451p-1,
    0x1.5fdbbe9bba776p3
  },
  { // Entry 1218
    0x1.ffffffffffffffffffffffffdb28d451p-1,
    -0x1.5fdbbe9bba776p3
  },
  { // Entry 1219
    -0x1.6a09e667f3be075cad2d50dfc68cd88fp-1,
    0x1.78fdb9effea45p3
  },
  { // Entry 1220
    0x1.6a09e667f3be075cad2d50dfc68cd88fp-1,
    -0x1.78fdb9effea45p3
  },
  { // Entry 1221
    -0x1.6a09e667f3bd5257b9f95701d3f13f5dp-1,
    0x1.78fdb9effea46p3
  },
  { // Entry 1222
    0x1.6a09e667f3bd5257b9f95701d3f13f5dp-1,
    -0x1.78fdb9effea46p3
  },
  { // Entry 1223
    -0x1.6a09e667f3bc9d52c6c55d2386d32c92p-1,
    0x1.78fdb9effea47p3
  },
  { // Entry 1224
    0x1.6a09e667f3bc9d52c6c55d2386d32c92p-1,
    -0x1.78fdb9effea47p3
  },
  { // Entry 1225
    0x1.6a09e667f3bb8477509955093bb292bep-1,
    0x1.ab41b09886fe8p3
  },
  { // Entry 1226
    -0x1.6a09e667f3bb8477509955093bb292bep-1,
    -0x1.ab41b09886fe8p3
  },
  { // Entry 1227
    0x1.6a09e667f3bc397c43cd4ee8153e60a0p-1,
    0x1.ab41b09886fe9p3
  },
  { // Entry 1228
    -0x1.6a09e667f3bc397c43cd4ee8153e60a0p-1,
    -0x1.ab41b09886fe9p3
  },
  { // Entry 1229
    0x1.6a09e667f3bcee81370148c69447b4e7p-1,
    0x1.ab41b09886feap3
  },
  { // Entry 1230
    -0x1.6a09e667f3bcee81370148c69447b4e7p-1,
    -0x1.ab41b09886feap3
  },
  { // Entry 1231
    0x1.ffffffffffffffffffffffff922141b4p-1,
    0x1.c463abeccb2bap3
  },
  { // Entry 1232
    -0x1.ffffffffffffffffffffffff922141b4p-1,
    -0x1.c463abeccb2bap3
  },
  { // Entry 1233
    0x1.fffffffffffffffffffffffff9d717a7p-1,
    0x1.c463abeccb2bbp3
  },
  { // Entry 1234
    -0x1.fffffffffffffffffffffffff9d717a7p-1,
    -0x1.c463abeccb2bbp3
  },
  { // Entry 1235
    0x1.ffffffffffffffffffffffffe18ced9ap-1,
    0x1.c463abeccb2bcp3
  },
  { // Entry 1236
    -0x1.ffffffffffffffffffffffffe18ced9ap-1,
    -0x1.c463abeccb2bcp3
  },
  { // Entry 1237
    0x1.6a09e667f3be13d77d8c52a726550eacp-1,
    0x1.dd85a7410f58bp3
  },
  { // Entry 1238
    -0x1.6a09e667f3be13d77d8c52a726550eacp-1,
    -0x1.dd85a7410f58bp3
  },
  { // Entry 1239
    0x1.6a09e667f3bd5ed28a5858c939f6ddaap-1,
    0x1.dd85a7410f58cp3
  },
  { // Entry 1240
    -0x1.6a09e667f3bd5ed28a5858c939f6ddaap-1,
    -0x1.dd85a7410f58cp3
  },
  { // Entry 1241
    0x1.6a09e667f3bca9cd97245eeaf316330ep-1,
    0x1.dd85a7410f58dp3
  },
  { // Entry 1242
    -0x1.6a09e667f3bca9cd97245eeaf316330ep-1,
    -0x1.dd85a7410f58dp3
  },
  { // Entry 1243
    0x1.583ebeff65cc226480ae685c1765dec1p-49,
    0x1.f6a7a2955385dp3
  },
  { // Entry 1244
    -0x1.583ebeff65cc226480ae685c1765dec1p-49,
    -0x1.f6a7a2955385dp3
  },
  { // Entry 1245
    0x1.60fafbfd9730899202b9a170c3971e86p-51,
    0x1.f6a7a2955385ep3
  },
  { // Entry 1246
    -0x1.60fafbfd9730899202b9a170c3971e86p-51,
    -0x1.f6a7a2955385ep3
  },
  { // Entry 1247
    -0x1.4f8282013467bb36fea32f479753fe7ap-50,
    0x1.f6a7a2955385fp3
  },
  { // Entry 1248
    0x1.4f8282013467bb36fea32f479753fe7ap-50,
    -0x1.f6a7a2955385fp3
  },
  { // Entry 1249
    -0x1.6a09e667f3ba0df299d25f82f639251fp-1,
    0x1.07e4cef4cbd96p4
  },
  { // Entry 1250
    0x1.6a09e667f3ba0df299d25f82f639251fp-1,
    -0x1.07e4cef4cbd96p4
  },
  { // Entry 1251
    -0x1.6a09e667f3bb77fc803a5341c552fe0fp-1,
    0x1.07e4cef4cbd97p4
  },
  { // Entry 1252
    0x1.6a09e667f3bb77fc803a5341c552fe0fp-1,
    -0x1.07e4cef4cbd97p4
  },
  { // Entry 1253
    -0x1.6a09e667f3bce20666a246ff2a62f097p-1,
    0x1.07e4cef4cbd98p4
  },
  { // Entry 1254
    0x1.6a09e667f3bce20666a246ff2a62f097p-1,
    -0x1.07e4cef4cbd98p4
  },
  { // Entry 1255
    -0x1.fffffffffffffffffffffffd2531a43dp-1,
    0x1.1475cc9eedeffp4
  },
  { // Entry 1256
    0x1.fffffffffffffffffffffffd2531a43dp-1,
    -0x1.1475cc9eedeffp4
  },
  { // Entry 1257
    -0x1.ffffffffffffffffffffffff86437656p-1,
    0x1.1475cc9eedfp4
  },
  { // Entry 1258
    0x1.ffffffffffffffffffffffff86437656p-1,
    -0x1.1475cc9eedfp4
  },
  { // Entry 1259
    -0x1.ffffffffffffffffffffffffe755486fp-1,
    0x1.1475cc9eedf01p4
  },
  { // Entry 1260
    0x1.ffffffffffffffffffffffffe755486fp-1,
    -0x1.1475cc9eedf01p4
  },
  { // Entry 1261
    -0x1.6a09e667f3bed557411f4e4c114d7357p-1,
    0x1.2106ca4910068p4
  },
  { // Entry 1262
    0x1.6a09e667f3bed557411f4e4c114d7357p-1,
    -0x1.2106ca4910068p4
  },
  { // Entry 1263
    -0x1.6a09e667f3bd6b4d5ab75a909f8e5b4cp-1,
    0x1.2106ca4910069p4
  },
  { // Entry 1264
    0x1.6a09e667f3bd6b4d5ab75a909f8e5b4cp-1,
    -0x1.2106ca4910069p4
  },
  { // Entry 1265
    -0x1.6a09e667f3bc0143744f66d3c3c55cd9p-1,
    0x1.2106ca491006ap4
  },
  { // Entry 1266
    0x1.6a09e667f3bc0143744f66d3c3c55cd9p-1,
    -0x1.2106ca491006ap4
  },
  { // Entry 1267
    -0x1.34f272993d1414a2b39bd83705cec120p-48,
    0x1.2d97c7f3321d1p4
  },
  { // Entry 1268
    0x1.34f272993d1414a2b39bd83705cec120p-48,
    -0x1.2d97c7f3321d1p4
  },
  { // Entry 1269
    -0x1.a79394c9e8a0a5159cdec1ba8362b968p-51,
    0x1.2d97c7f3321d2p4
  },
  { // Entry 1270
    0x1.a79394c9e8a0a5159cdec1ba8362b968p-51,
    -0x1.2d97c7f3321d2p4
  },
  { // Entry 1271
    0x1.961b1acd85d7d6ba98c84f9133d10abep-49,
    0x1.2d97c7f3321d3p4
  },
  { // Entry 1272
    -0x1.961b1acd85d7d6ba98c84f9133d10abep-49,
    -0x1.2d97c7f3321d3p4
  },
  { // Entry 1273
    0x1.6a09e667f3ba0177c9735dbb72f09f65p-1,
    0x1.3a28c59d54339p4
  },
  { // Entry 1274
    -0x1.6a09e667f3ba0177c9735dbb72f09f65p-1,
    -0x1.3a28c59d54339p4
  },
  { // Entry 1275
    0x1.6a09e667f3bb6b81afdb517a4e8548b4p-1,
    0x1.3a28c59d5433ap4
  },
  { // Entry 1276
    -0x1.6a09e667f3bb6b81afdb517a4e8548b4p-1,
    -0x1.3a28c59d5433ap4
  },
  { // Entry 1277
    0x1.6a09e667f3bcd58b96434537c0100b9bp-1,
    0x1.3a28c59d5433bp4
  },
  { // Entry 1278
    -0x1.6a09e667f3bcd58b96434537c0100b9bp-1,
    -0x1.3a28c59d5433bp4
  },
  { // Entry 1279
    0x1.fffffffffffffffffffffffd0711f437p-1,
    0x1.46b9c347764a2p4
  },
  { // Entry 1280
    -0x1.fffffffffffffffffffffffd0711f437p-1,
    -0x1.46b9c347764a2p4
  },
  { // Entry 1281
    0x1.ffffffffffffffffffffffff79c9ec83p-1,
    0x1.46b9c347764a3p4
  },
  { // Entry 1282
    -0x1.ffffffffffffffffffffffff79c9ec83p-1,
    -0x1.46b9c347764a3p4
  },
  { // Entry 1283
    0x1.ffffffffffffffffffffffffec81e4cfp-1,
    0x1.46b9c347764a4p4
  },
  { // Entry 1284
    -0x1.ffffffffffffffffffffffffec81e4cfp-1,
    -0x1.46b9c347764a4p4
  },
  { // Entry 1285
    0x1.6a09e667f3bee1d2117e501369fbffefp-1,
    0x1.534ac0f19860bp4
  },
  { // Entry 1286
    -0x1.6a09e667f3bee1d2117e501369fbffefp-1,
    -0x1.534ac0f19860bp4
  },
  { // Entry 1287
    0x1.6a09e667f3bd77c82b165c5804b7b843p-1,
    0x1.534ac0f19860cp4
  },
  { // Entry 1288
    -0x1.6a09e667f3bd77c82b165c5804b7b843p-1,
    -0x1.534ac0f19860cp4
  },
  { // Entry 1289
    0x1.6a09e667f3bc0dbe44ae689b35698a2fp-1,
    0x1.534ac0f19860dp4
  },
  { // Entry 1290
    -0x1.6a09e667f3bc0dbe44ae689b35698a2fp-1,
    -0x1.534ac0f19860dp4
  },
  { // Entry 1291
    0x1.3dc585b2c742181326e07c40375464dep-48,
    0x1.5fdbbe9bba774p4
  },
  { // Entry 1292
    -0x1.3dc585b2c742181326e07c40375464dep-48,
    -0x1.5fdbbe9bba774p4
  },
  { // Entry 1293
    0x1.ee2c2d963a10c0993703e20442ad7b8dp-51,
    0x1.5fdbbe9bba775p4
  },
  { // Entry 1294
    -0x1.ee2c2d963a10c0993703e20442ad7b8dp-51,
    -0x1.5fdbbe9bba775p4
  },
  { // Entry 1295
    -0x1.8474f49a717bcfd9b23f077ec8dd175bp-49,
    0x1.5fdbbe9bba776p4
  },
  { // Entry 1296
    0x1.8474f49a717bcfd9b23f077ec8dd175bp-49,
    -0x1.5fdbbe9bba776p4
  },
  { // Entry 1297
    -0x1.6a09e667f3b9f4fcf9145bf3ef39f901p-1,
    0x1.6c6cbc45dc8dcp4
  },
  { // Entry 1298
    0x1.6a09e667f3b9f4fcf9145bf3ef39f901p-1,
    -0x1.6c6cbc45dc8dcp4
  },
  { // Entry 1299
    -0x1.6a09e667f3bb5f06df7c4fb2d74972aep-1,
    0x1.6c6cbc45dc8ddp4
  },
  { // Entry 1300
    0x1.6a09e667f3bb5f06df7c4fb2d74972aep-1,
    -0x1.6c6cbc45dc8ddp4
  },
  { // Entry 1301
    -0x1.6a09e667f3bcc910c5e44370554f05f4p-1,
    0x1.6c6cbc45dc8dep4
  },
  { // Entry 1302
    0x1.6a09e667f3bcc910c5e44370554f05f4p-1,
    -0x1.6c6cbc45dc8dep4
  },
  { // Entry 1303
    -0x1.fffffffffffffffffffffffce85685bdp-1,
    0x1.78fdb9effea45p4
  },
  { // Entry 1304
    0x1.fffffffffffffffffffffffce85685bdp-1,
    -0x1.78fdb9effea45p4
  },
  { // Entry 1305
    -0x1.ffffffffffffffffffffffff6cb4a43dp-1,
    0x1.78fdb9effea46p4
  },
  { // Entry 1306
    0x1.ffffffffffffffffffffffff6cb4a43dp-1,
    -0x1.78fdb9effea46p4
  },
  { // Entry 1307
    -0x1.fffffffffffffffffffffffff112c2bcp-1,
    0x1.78fdb9effea47p4
  },
  { // Entry 1308
    0x1.fffffffffffffffffffffffff112c2bcp-1,
    -0x1.78fdb9effea47p4
  },
  { // Entry 1309
    -0x1.6a09e667f3beee4ce1dd51dac23c6bdcp-1,
    0x1.858eb79a20baep4
  },
  { // Entry 1310
    0x1.6a09e667f3beee4ce1dd51dac23c6bdcp-1,
    -0x1.858eb79a20baep4
  },
  { // Entry 1311
    -0x1.6a09e667f3bd8442fb755e1f6972f48fp-1,
    0x1.858eb79a20bafp4
  },
  { // Entry 1312
    0x1.6a09e667f3bd8442fb755e1f6972f48fp-1,
    -0x1.858eb79a20bafp4
  },
  { // Entry 1313
    -0x1.6a09e667f3bc1a39150d6a62a69f96dap-1,
    0x1.858eb79a20bb0p4
  },
  { // Entry 1314
    0x1.6a09e667f3bc1a39150d6a62a69f96dap-1,
    -0x1.858eb79a20bb0p4
  },
  { // Entry 1315
    0x1.fa7299b17573d373c615096572a0de04p-1,
    0x1.fffffffffffffp62
  },
  { // Entry 1316
    -0x1.fa7299b17573d373c615096572a0de04p-1,
    -0x1.fffffffffffffp62
  },
  { // Entry 1317
    0x1.fff6dfd42dc54430bc0576b00a88bd94p-1,
    0x1.0p63
  },
  { // Entry 1318
    -0x1.fff6dfd42dc54430bc0576b00a88bd94p-1,
    -0x1.0p63
  },
  { // Entry 1319
    0x1.e456b818e7396b45989978d4b7ebf7b3p-1,
    0x1.0000000000001p63
  },
  { // Entry 1320
    -0x1.e456b818e7396b45989978d4b7ebf7b3p-1,
    -0x1.0000000000001p63
  },
  { // Entry 1321
    -0x1.86dcca0d689e7b56ef79481be4a645c6p-1,
    0x1.fffffffffffffp26
  },
  { // Entry 1322
    0x1.86dcca0d689e7b56ef79481be4a645c6p-1,
    -0x1.fffffffffffffp26
  },
  { // Entry 1323
    -0x1.86dcc9babb0a40ee875cab3b9e892757p-1,
    0x1.0p27
  },
  { // Entry 1324
    0x1.86dcc9babb0a40ee875cab3b9e892757p-1,
    -0x1.0p27
  },
  { // Entry 1325
    -0x1.86dcc9155fe182d4515599426bb14b94p-1,
    0x1.0000000000001p27
  },
  { // Entry 1326
    0x1.86dcc9155fe182d4515599426bb14b94p-1,
    -0x1.0000000000001p27
  },
  { // Entry 1327
    -0x1.8f22f84d42da1f57469dfaac44a8b73ap-1,
    0x1.fffffffffffffp23
  },
  { // Entry 1328
    0x1.8f22f84d42da1f57469dfaac44a8b73ap-1,
    -0x1.fffffffffffffp23
  },
  { // Entry 1329
    -0x1.8f22f8433d6edfe9a4aff9622517caa9p-1,
    0x1.0p24
  },
  { // Entry 1330
    0x1.8f22f8433d6edfe9a4aff9622517caa9p-1,
    -0x1.0p24
  },
  { // Entry 1331
    -0x1.8f22f82f32985fe30699c6e12d9dfce2p-1,
    0x1.0000000000001p24
  },
  { // Entry 1332
    0x1.8f22f82f32985fe30699c6e12d9dfce2p-1,
    -0x1.0000000000001p24
  },
  { // Entry 1333
    -0x1.837b9dddc1eabd379d729f575fc1144cp-1,
    0x1.fffffffffffffp1
  },
  { // Entry 1334
    0x1.837b9dddc1eabd379d729f575fc1144cp-1,
    -0x1.fffffffffffffp1
  },
  { // Entry 1335
    -0x1.837b9dddc1eae70ce98055a0e450d93cp-1,
    0x1.0p2
  },
  { // Entry 1336
    0x1.837b9dddc1eae70ce98055a0e450d93cp-1,
    -0x1.0p2
  },
  { // Entry 1337
    -0x1.837b9dddc1eb3ab7819bc233db4697b5p-1,
    0x1.0000000000001p2
  },
  { // Entry 1338
    0x1.837b9dddc1eb3ab7819bc233db4697b5p-1,
    -0x1.0000000000001p2
  },
  { // Entry 1339
    0x1.d18f6ead1b446b4bcb73c2390b330d75p-1,
    0x1.fffffffffffffp0
  },
  { // Entry 1340
    -0x1.d18f6ead1b446b4bcb73c2390b330d75p-1,
    -0x1.fffffffffffffp0
  },
  { // Entry 1341
    0x1.d18f6ead1b445dfab848188009c9bb95p-1,
    0x1.0p1
  },
  { // Entry 1342
    -0x1.d18f6ead1b445dfab848188009c9bb95p-1,
    -0x1.0p1
  },
  { // Entry 1343
    0x1.d18f6ead1b44435891f0c50e01826988p-1,
    0x1.0000000000001p1
  },
  { // Entry 1344
    -0x1.d18f6ead1b44435891f0c50e01826988p-1,
    -0x1.0000000000001p1
  },
  { // Entry 1345
    0x1.aed548f090ced79c79cbf790441f8197p-1,
    0x1.fffffffffffffp-1
  },
  { // Entry 1346
    -0x1.aed548f090ced79c79cbf790441f8197p-1,
    -0x1.fffffffffffffp-1
  },
  { // Entry 1347
    0x1.aed548f090cee0418dd3d2138a1e7865p-1,
    0x1.0p0
  },
  { // Entry 1348
    -0x1.aed548f090cee0418dd3d2138a1e7865p-1,
    -0x1.0p0
  },
  { // Entry 1349
    0x1.aed548f090cef18bb5e3871a14d94609p-1,
    0x1.0000000000001p0
  },
  { // Entry 1350
    -0x1.aed548f090cef18bb5e3871a14d94609p-1,
    -0x1.0000000000001p0
  },
  { // Entry 1351
    0x1.eaee8744b05ef07cd0b9089130598314p-2,
    0x1.fffffffffffffp-2
  },
  { // Entry 1352
    -0x1.eaee8744b05ef07cd0b9089130598314p-2,
    -0x1.fffffffffffffp-2
  },
  { // Entry 1353
    0x1.eaee8744b05efe8764bc364fd837b666p-2,
    0x1.0p-1
  },
  { // Entry 1354
    -0x1.eaee8744b05efe8764bc364fd837b666p-2,
    -0x1.0p-1
  },
  { // Entry 1355
    0x1.eaee8744b05f1a9c8cc291cd27981051p-2,
    0x1.0000000000001p-1
  },
  { // Entry 1356
    -0x1.eaee8744b05f1a9c8cc291cd27981051p-2,
    -0x1.0000000000001p-1
  },
  { // Entry 1357
    0x1.faaeed4f31575c27f39c1d7c012a4413p-3,
    0x1.fffffffffffffp-3
  },
  { // Entry 1358
    -0x1.faaeed4f31575c27f39c1d7c012a4413p-3,
    -0x1.fffffffffffffp-3
  },
  { // Entry 1359
    0x1.faaeed4f31576ba89debdc7351e8b1aep-3,
    0x1.0p-2
  },
  { // Entry 1360
    -0x1.faaeed4f31576ba89debdc7351e8b1aep-3,
    -0x1.0p-2
  },
  { // Entry 1361
    0x1.faaeed4f31578aa9f28b5a61f34dccb1p-3,
    0x1.0000000000001p-2
  },
  { // Entry 1362
    -0x1.faaeed4f31578aa9f28b5a61f34dccb1p-3,
    -0x1.0000000000001p-2
  },
  { // Entry 1363
    0x1.feaaeee86ee34cc05eff28740ee7b469p-4,
    0x1.fffffffffffffp-4
  },
  { // Entry 1364
    -0x1.feaaeee86ee34cc05eff28740ee7b469p-4,
    -0x1.fffffffffffffp-4
  },
  { // Entry 1365
    0x1.feaaeee86ee35ca069a86721f89f85a5p-4,
    0x1.0p-3
  },
  { // Entry 1366
    -0x1.feaaeee86ee35ca069a86721f89f85a5p-4,
    -0x1.0p-3
  },
  { // Entry 1367
    0x1.feaaeee86ee37c607efae47dcc092c1cp-4,
    0x1.0000000000001p-3
  },
  { // Entry 1368
    -0x1.feaaeee86ee37c607efae47dcc092c1cp-4,
    -0x1.0000000000001p-3
  },
  { // Entry 1369
    0x1.ffaaaeeed4ed9b53a408c0f2bc02c8f9p-5,
    0x1.fffffffffffffp-5
  },
  { // Entry 1370
    -0x1.ffaaaeeed4ed9b53a408c0f2bc02c8f9p-5,
    -0x1.fffffffffffffp-5
  },
  { // Entry 1371
    0x1.ffaaaeeed4edab4ba4b365ed25a9595fp-5,
    0x1.0p-4
  },
  { // Entry 1372
    -0x1.ffaaaeeed4edab4ba4b365ed25a9595fp-5,
    -0x1.0p-4
  },
  { // Entry 1373
    0x1.ffaaaeeed4edcb3ba608afe1f8f4fa6bp-5,
    0x1.0000000000001p-4
  },
  { // Entry 1374
    -0x1.ffaaaeeed4edcb3ba608afe1f8f4fa6bp-5,
    -0x1.0000000000001p-4
  },
  { // Entry 1375
    0x1.ffeaaaeeee86d8ccfe368cd95e38f003p-6,
    0x1.fffffffffffffp-6
  },
  { // Entry 1376
    -0x1.ffeaaaeeee86d8ccfe368cd95e38f003p-6,
    -0x1.fffffffffffffp-6
  },
  { // Entry 1377
    0x1.ffeaaaeeee86e8cafe41376d47919579p-6,
    0x1.0p-5
  },
  { // Entry 1378
    -0x1.ffeaaaeeee86e8cafe41376d47919579p-6,
    -0x1.0p-5
  },
  { // Entry 1379
    0x1.ffeaaaeeee8708c6fe568c951a428069p-6,
    0x1.0000000000001p-5
  },
  { // Entry 1380
    -0x1.ffeaaaeeee8708c6fe568c951a428069p-6,
    -0x1.0000000000001p-5
  },
  { // Entry 1381
    0x1.fffaaaaeeeed3ed5c9c5ab6538f9cce0p-7,
    0x1.fffffffffffffp-7
  },
  { // Entry 1382
    -0x1.fffaaaaeeeed3ed5c9c5ab6538f9cce0p-7,
    -0x1.fffffffffffffp-7
  },
  { // Entry 1383
    0x1.fffaaaaeeeed4ed549c6560f889ee531p-7,
    0x1.0p-6
  },
  { // Entry 1384
    -0x1.fffaaaaeeeed4ed549c6560f889ee531p-7,
    -0x1.0p-6
  },
  { // Entry 1385
    0x1.fffaaaaeeeed6ed449c7ab6427e8fdd4p-7,
    0x1.0000000000001p-6
  },
  { // Entry 1386
    -0x1.fffaaaaeeeed6ed449c7ab6427e8fdd4p-7,
    -0x1.0000000000001p-6
  },
  { // Entry 1387
    0x1.fffffffaaaaa9aaeeeef6eed4ed442a4p-15,
    0x1.fffffffffffffp-15
  },
  { // Entry 1388
    -0x1.fffffffaaaaa9aaeeeef6eed4ed442a4p-15,
    -0x1.fffffffffffffp-15
  },
  { // Entry 1389
    0x1.fffffffaaaaaaaaeeeeeeeed4ed4ed4fp-15,
    0x1.0p-14
  },
  { // Entry 1390
    -0x1.fffffffaaaaaaaaeeeeeeeed4ed4ed4fp-15,
    -0x1.0p-14
  },
  { // Entry 1391
    0x1.fffffffaaaaacaaeeeedeeed4ed642a4p-15,
    0x1.0000000000001p-14
  },
  { // Entry 1392
    -0x1.fffffffaaaaacaaeeeedeeed4ed642a4p-15,
    -0x1.0000000000001p-14
  },
  { // Entry 1393
    0x1.ffffffffffffeeaaaaaaaaaaaacaeeeep-28,
    0x1.fffffffffffffp-28
  },
  { // Entry 1394
    -0x1.ffffffffffffeeaaaaaaaaaaaacaeeeep-28,
    -0x1.fffffffffffffp-28
  },
  { // Entry 1395
    0x1.fffffffffffffeaaaaaaaaaaaaaaeeeep-28,
    0x1.0p-27
  },
  { // Entry 1396
    -0x1.fffffffffffffeaaaaaaaaaaaaaaeeeep-28,
    -0x1.0p-27
  },
  { // Entry 1397
    0x1.0000000000000f555555555555357777p-27,
    0x1.0000000000001p-27
  },
  { // Entry 1398
    -0x1.0000000000000f555555555555357777p-27,
    -0x1.0000000000001p-27
  },
  { // Entry 1399
    0x1.ffffffffffffeffaaaaaaaaaaaab2aaep-31,
    0x1.fffffffffffffp-31
  },
  { // Entry 1400
    -0x1.ffffffffffffeffaaaaaaaaaaaab2aaep-31,
    -0x1.fffffffffffffp-31
  },
  { // Entry 1401
    0x1.fffffffffffffffaaaaaaaaaaaaaaaaep-31,
    0x1.0p-30
  },
  { // Entry 1402
    -0x1.fffffffffffffffaaaaaaaaaaaaaaaaep-31,
    -0x1.0p-30
  },
  { // Entry 1403
    0x1.0000000000000ffd555555555554d557p-30,
    0x1.0000000000001p-30
  },
  { // Entry 1404
    -0x1.0000000000000ffd555555555554d557p-30,
    -0x1.0000000000001p-30
  },
  { // Entry 1405
    -0x1.452fc98b34e96b61139b09a7c84a44bdp-8,
    -0x1.fffffffffffffp1023
  },
  { // Entry 1406
    0x1.452fc98b34e96b61139b09a7c84a44bdp-8,
    0x1.fffffffffffffp1023
  },
  { // Entry 1407
    0x1.452fc98b34e96b61139b09a7c84a44bdp-8,
    0x1.fffffffffffffp1023
  },
  { // Entry 1408
    -0x1.452fc98b34e96b61139b09a7c84a44bdp-8,
    -0x1.fffffffffffffp1023
  },
  { // Entry 1409
    0x1.452fc98b34e96b61139b09a7c84a44bdp-8,
    0x1.fffffffffffffp1023
  },
  { // Entry 1410
    -0x1.452fc98b34e96b61139b09a7c84a44bdp-8,
    -0x1.fffffffffffffp1023
  },
  { // Entry 1411
    0x1.daa3677c6ee8a22eb6c4b12ca10ce021p-1,
    0x1.ffffffffffffep1023
  },
  { // Entry 1412
    -0x1.daa3677c6ee8a22eb6c4b12ca10ce021p-1,
    -0x1.ffffffffffffep1023
  },
  { // Entry 1413
    0x1.1a62633145c06e0e689481270436e2edp-53,
    0x1.921fb54442d18p1
  },
  { // Entry 1414
    -0x1.1a62633145c06e0e689481270436e2edp-53,
    -0x1.921fb54442d18p1
  },
  { // Entry 1415
    0x1.ffffffffffffffffffffffffffec8831p-1,
    0x1.921fb54442d18p0
  },
  { // Entry 1416
    -0x1.ffffffffffffffffffffffffffec8831p-1,
    -0x1.921fb54442d18p0
  },
  { // Entry 1417
    0x1.aed548f090cef18bb5e3871a14d94609p-1,
    0x1.0000000000001p0
  },
  { // Entry 1418
    -0x1.aed548f090cef18bb5e3871a14d94609p-1,
    -0x1.0000000000001p0
  },
  { // Entry 1419
    0x1.aed548f090cee0418dd3d2138a1e7865p-1,
    0x1.0p0
  },
  { // Entry 1420
    -0x1.aed548f090cee0418dd3d2138a1e7865p-1,
    -0x1.0p0
  },
  { // Entry 1421
    0x1.aed548f090ced79c79cbf790441f8197p-1,
    0x1.fffffffffffffp-1
  },
  { // Entry 1422
    -0x1.aed548f090ced79c79cbf790441f8197p-1,
    -0x1.fffffffffffffp-1
  },
  { // Entry 1423
    0x1.6a09e667f3bcc5e9fee352f50fd3f4e9p-1,
    0x1.921fb54442d18p-1
  },
  { // Entry 1424
    -0x1.6a09e667f3bcc5e9fee352f50fd3f4e9p-1,
    -0x1.921fb54442d18p-1
  },
  { // Entry 1425
    0x1.0000000000000fffffffffffffffffffp-1022,
    0x1.0000000000001p-1022
  },
  { // Entry 1426
    -0x1.0000000000000fffffffffffffffffffp-1022,
    -0x1.0000000000001p-1022
  },
  { // Entry 1427
    0x1.ffffffffffffffffffffffffffffffffp-1023,
    0x1.0p-1022
  },
  { // Entry 1428
    -0x1.ffffffffffffffffffffffffffffffffp-1023,
    -0x1.0p-1022
  },
  { // Entry 1429
    0x1.ffffffffffffdfffffffffffffffffffp-1023,
    0x1.ffffffffffffep-1023
  },
  { // Entry 1430
    -0x1.ffffffffffffdfffffffffffffffffffp-1023,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 1431
    0x1.ffffffffffffbfffffffffffffffffffp-1023,
    0x1.ffffffffffffcp-1023
  },
  { // Entry 1432
    -0x1.ffffffffffffbfffffffffffffffffffp-1023,
    -0x1.ffffffffffffcp-1023
  },
  { // Entry 1433
    0x1.ffffffffffffffffffffffffffffffffp-1074,
    0x1.0p-1073
  },
  { // Entry 1434
    -0x1.ffffffffffffffffffffffffffffffffp-1074,
    -0x1.0p-1073
  },
  { // Entry 1435
    0.0,
    0x1.0p-1074
  },
  { // Entry 1436
    -0.0,
    -0x1.0p-1074
  },
  { // Entry 1437
    0.0,
    0.0
  },
  { // Entry 1438
    -0.0,
    -0.0
  },
};
#endif // __BIONIC__

TEST(math_sin, sin_intel) {
#if defined(__BIONIC__)
  for (size_t i = 0; i < sizeof(g_sin_intel_data)/sizeof(sin_intel_data_t); i++) {
    EXPECT_DOUBLE_EQ(g_sin_intel_data[i].expected, sin(g_sin_intel_data[i].call_data)) << "Failed on element " << i;
  }
#else // __BIONIC__
  GTEST_LOG_(INFO) << "This test does nothing.";
#endif // __BIONIC__
}
