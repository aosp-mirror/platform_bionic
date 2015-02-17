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

static data_1_3_t<double, double, double, double> g_fma_intel_data[] = {
  { // Entry 0
    -0x1.e6666666666660p0,
    0x1.0p-1074,
    -0x1.ccccccccccccdp-1,
    -0x1.e666666666666p0
  },
  { // Entry 1
    0x1.15f15f15f15edfffffffffffffffffffp-2,
    0x1.0p-1074,
    -0x1.ccccccccccccdp-1,
    0x1.15f15f15f15eep-2
  },
  { // Entry 2
    0x1.0000000000002fffffffffffffffffffp-41,
    0x1.0p-1074,
    -0x1.e666666666666p-1,
    0x1.0000000000003p-41
  },
  { // Entry 3
    -0x1.e666666666665fffffffffffffffffffp0,
    0x1.0p-1074,
    0x1.0750750750756p-3,
    -0x1.e666666666666p0
  },
  { // Entry 4
    0x1.00000000000030p-41,
    0x1.0p-1074,
    0x1.4444444444430p-4,
    0x1.0000000000003p-41
  },
  { // Entry 5
    -0x1.f4ccccccccccc766666666666668p0,
    0x1.0000000000001p-4,
    -0x1.ccccccccccccdp-1,
    -0x1.e666666666666p0
  },
  { // Entry 6
    0x1.1be9c07bef3aa00000000ca3acc0p0,
    0x1.333334be90b7dp-1,
    0x1.62e42fefa39efp-1,
    0x1.62e42fefa39efp-1
  },
  { // Entry 7
    0x1.2da85c2c93416ffffffffd1b42c0p0,
    0x1.6666670f24aa5p-1,
    0x1.62e42fefa39efp-1,
    0x1.62e42fefa39efp-1
  },
  { // Entry 8
    0x1.6ce999999999c802333333333480p16,
    0x1.8000000000001p4,
    0x1.e666666666669p11,
    0x1.ccccccccccccfp4
  },
  { // Entry 9
    0x1.62b83c4461cc280000000001bc40p0,
    0x1.ff812e8bc2d1fp-1,
    0x1.62e42fefa39efp-1,
    0x1.62e42fefa39efp-1
  },
  { // Entry 10
    0x1.0942b0df6a30e7ff3586fb5fb5p39,
    0x1.ffe7fffffffffp40,
    0x1.094f2094f2096p-2,
    -0x1.ba2e8ba2e8ba2p-2
  },
  { // Entry 11
    0x1.06fb586fb586f8p-51,
    0x1.ffffffffffffcp-1,
    -0x1.8df6b0df6b0dfp-1,
    0x1.8df6b0df6b0e0p-1
  },
  { // Entry 12
    0x1.ffffffffffff70000000000004p-1,
    0x1.fffffffffffffp1023,
    -0x1.0p-1074,
    0x1.fffffffffffffp-1
  },
  { // Entry 13
    0x1.ffffffffffff80000000000004p-1,
    0x1.fffffffffffffp1023,
    -0x1.0p-1074,
    0x1.0p0
  },
  { // Entry 14
    0x1.ffffffffffffa0000000000004p-1,
    0x1.fffffffffffffp1023,
    -0x1.0p-1074,
    0x1.0000000000001p0
  },
  { // Entry 15
    0x1.fffffffffffff0p-1,
    0x1.fffffffffffffp1023,
    -0.0,
    0x1.fffffffffffffp-1
  },
  { // Entry 16
    0x1.p0,
    0x1.fffffffffffffp1023,
    -0.0,
    0x1.0p0
  },
  { // Entry 17
    0x1.00000000000010p0,
    0x1.fffffffffffffp1023,
    -0.0,
    0x1.0000000000001p0
  },
  { // Entry 18
    0x1.00000000000037fffffffffffep0,
    0x1.fffffffffffffp1023,
    0x1.0p-1074,
    0x1.fffffffffffffp-1
  },
  { // Entry 19
    0x1.0000000000003ffffffffffffep0,
    0x1.fffffffffffffp1023,
    0x1.0p-1074,
    0x1.0p0
  },
  { // Entry 20
    0x1.0000000000004ffffffffffffep0,
    0x1.fffffffffffffp1023,
    0x1.0p-1074,
    0x1.0000000000001p0
  },
  { // Entry 21
    0x1.ffffffffffff70000000000008p-2,
    0x1.ffffffffffffep1022,
    -0x1.0p-1074,
    0x1.fffffffffffffp-2
  },
  { // Entry 22
    0x1.ffffffffffff80000000000008p-2,
    0x1.ffffffffffffep1022,
    -0x1.0p-1074,
    0x1.0p-1
  },
  { // Entry 23
    0x1.ffffffffffffa0000000000008p-2,
    0x1.ffffffffffffep1022,
    -0x1.0p-1074,
    0x1.0000000000001p-1
  },
  { // Entry 24
    0x1.fffffffffffff0p-2,
    0x1.ffffffffffffep1022,
    -0.0,
    0x1.fffffffffffffp-2
  },
  { // Entry 25
    0x1.p-1,
    0x1.ffffffffffffep1022,
    -0.0,
    0x1.0p-1
  },
  { // Entry 26
    0x1.00000000000010p-1,
    0x1.ffffffffffffep1022,
    -0.0,
    0x1.0000000000001p-1
  },
  { // Entry 27
    0x1.00000000000037fffffffffffcp-1,
    0x1.ffffffffffffep1022,
    0x1.0p-1074,
    0x1.fffffffffffffp-2
  },
  { // Entry 28
    0x1.0000000000003ffffffffffffcp-1,
    0x1.ffffffffffffep1022,
    0x1.0p-1074,
    0x1.0p-1
  },
  { // Entry 29
    0x1.0000000000004ffffffffffffcp-1,
    0x1.ffffffffffffep1022,
    0x1.0p-1074,
    0x1.0000000000001p-1
  },
  { // Entry 30
    0x1.ffffffffffff70000000000004p-2,
    0x1.fffffffffffffp1022,
    -0x1.0p-1074,
    0x1.fffffffffffffp-2
  },
  { // Entry 31
    0x1.ffffffffffff80000000000004p-2,
    0x1.fffffffffffffp1022,
    -0x1.0p-1074,
    0x1.0p-1
  },
  { // Entry 32
    0x1.ffffffffffffa0000000000004p-2,
    0x1.fffffffffffffp1022,
    -0x1.0p-1074,
    0x1.0000000000001p-1
  },
  { // Entry 33
    0x1.fffffffffffff0p-2,
    0x1.fffffffffffffp1022,
    -0.0,
    0x1.fffffffffffffp-2
  },
  { // Entry 34
    0x1.p-1,
    0x1.fffffffffffffp1022,
    -0.0,
    0x1.0p-1
  },
  { // Entry 35
    0x1.00000000000010p-1,
    0x1.fffffffffffffp1022,
    -0.0,
    0x1.0000000000001p-1
  },
  { // Entry 36
    0x1.00000000000037fffffffffffep-1,
    0x1.fffffffffffffp1022,
    0x1.0p-1074,
    0x1.fffffffffffffp-2
  },
  { // Entry 37
    0x1.0000000000003ffffffffffffep-1,
    0x1.fffffffffffffp1022,
    0x1.0p-1074,
    0x1.0p-1
  },
  { // Entry 38
    0x1.0000000000004ffffffffffffep-1,
    0x1.fffffffffffffp1022,
    0x1.0p-1074,
    0x1.0000000000001p-1
  },
  { // Entry 39
    0x1.ffffffffffff70p-2,
    0x1.0p1023,
    -0x1.0p-1074,
    0x1.fffffffffffffp-2
  },
  { // Entry 40
    0x1.ffffffffffff80p-2,
    0x1.0p1023,
    -0x1.0p-1074,
    0x1.0p-1
  },
  { // Entry 41
    0x1.ffffffffffffa0p-2,
    0x1.0p1023,
    -0x1.0p-1074,
    0x1.0000000000001p-1
  },
  { // Entry 42
    0x1.fffffffffffff0p-2,
    0x1.0p1023,
    -0.0,
    0x1.fffffffffffffp-2
  },
  { // Entry 43
    0x1.p-1,
    0x1.0p1023,
    -0.0,
    0x1.0p-1
  },
  { // Entry 44
    0x1.00000000000010p-1,
    0x1.0p1023,
    -0.0,
    0x1.0000000000001p-1
  },
  { // Entry 45
    0x1.00000000000038p-1,
    0x1.0p1023,
    0x1.0p-1074,
    0x1.fffffffffffffp-2
  },
  { // Entry 46
    0x1.00000000000040p-1,
    0x1.0p1023,
    0x1.0p-1074,
    0x1.0p-1
  },
  { // Entry 47
    0x1.00000000000050p-1,
    0x1.0p1023,
    0x1.0p-1074,
    0x1.0000000000001p-1
  },
  { // Entry 48
    0x1.ffffffffffffb0000000000004p-1,
    0x1.ffffffffffffep1022,
    -0x1.0p-1074,
    0x1.fffffffffffffp-1
  },
  { // Entry 49
    0x1.ffffffffffffc0000000000004p-1,
    0x1.ffffffffffffep1022,
    -0x1.0p-1074,
    0x1.0p0
  },
  { // Entry 50
    0x1.ffffffffffffe0000000000004p-1,
    0x1.ffffffffffffep1022,
    -0x1.0p-1074,
    0x1.0000000000001p0
  },
  { // Entry 51
    0x1.fffffffffffff0p-1,
    0x1.ffffffffffffep1022,
    -0.0,
    0x1.fffffffffffffp-1
  },
  { // Entry 52
    0x1.p0,
    0x1.ffffffffffffep1022,
    -0.0,
    0x1.0p0
  },
  { // Entry 53
    0x1.00000000000010p0,
    0x1.ffffffffffffep1022,
    -0.0,
    0x1.0000000000001p0
  },
  { // Entry 54
    0x1.00000000000017fffffffffffep0,
    0x1.ffffffffffffep1022,
    0x1.0p-1074,
    0x1.fffffffffffffp-1
  },
  { // Entry 55
    0x1.0000000000001ffffffffffffep0,
    0x1.ffffffffffffep1022,
    0x1.0p-1074,
    0x1.0p0
  },
  { // Entry 56
    0x1.0000000000002ffffffffffffep0,
    0x1.ffffffffffffep1022,
    0x1.0p-1074,
    0x1.0000000000001p0
  },
  { // Entry 57
    0x1.ffffffffffffb0000000000002p-1,
    0x1.fffffffffffffp1022,
    -0x1.0p-1074,
    0x1.fffffffffffffp-1
  },
  { // Entry 58
    0x1.ffffffffffffc0000000000002p-1,
    0x1.fffffffffffffp1022,
    -0x1.0p-1074,
    0x1.0p0
  },
  { // Entry 59
    0x1.ffffffffffffe0000000000002p-1,
    0x1.fffffffffffffp1022,
    -0x1.0p-1074,
    0x1.0000000000001p0
  },
  { // Entry 60
    0x1.fffffffffffff0p-1,
    0x1.fffffffffffffp1022,
    -0.0,
    0x1.fffffffffffffp-1
  },
  { // Entry 61
    0x1.p0,
    0x1.fffffffffffffp1022,
    -0.0,
    0x1.0p0
  },
  { // Entry 62
    0x1.00000000000010p0,
    0x1.fffffffffffffp1022,
    -0.0,
    0x1.0000000000001p0
  },
  { // Entry 63
    0x1.00000000000017ffffffffffffp0,
    0x1.fffffffffffffp1022,
    0x1.0p-1074,
    0x1.fffffffffffffp-1
  },
  { // Entry 64
    0x1.0000000000001fffffffffffffp0,
    0x1.fffffffffffffp1022,
    0x1.0p-1074,
    0x1.0p0
  },
  { // Entry 65
    0x1.0000000000002fffffffffffffp0,
    0x1.fffffffffffffp1022,
    0x1.0p-1074,
    0x1.0000000000001p0
  },
  { // Entry 66
    0x1.ffffffffffffb0p-1,
    0x1.0p1023,
    -0x1.0p-1074,
    0x1.fffffffffffffp-1
  },
  { // Entry 67
    0x1.ffffffffffffc0p-1,
    0x1.0p1023,
    -0x1.0p-1074,
    0x1.0p0
  },
  { // Entry 68
    0x1.ffffffffffffe0p-1,
    0x1.0p1023,
    -0x1.0p-1074,
    0x1.0000000000001p0
  },
  { // Entry 69
    0x1.fffffffffffff0p-1,
    0x1.0p1023,
    -0.0,
    0x1.fffffffffffffp-1
  },
  { // Entry 70
    0x1.p0,
    0x1.0p1023,
    -0.0,
    0x1.0p0
  },
  { // Entry 71
    0x1.00000000000010p0,
    0x1.0p1023,
    -0.0,
    0x1.0000000000001p0
  },
  { // Entry 72
    0x1.00000000000018p0,
    0x1.0p1023,
    0x1.0p-1074,
    0x1.fffffffffffffp-1
  },
  { // Entry 73
    0x1.00000000000020p0,
    0x1.0p1023,
    0x1.0p-1074,
    0x1.0p0
  },
  { // Entry 74
    0x1.00000000000030p0,
    0x1.0p1023,
    0x1.0p-1074,
    0x1.0000000000001p0
  },
  { // Entry 75
    0x1.ffffffffffffe00000000000007fffffp-2,
    0x1.fffffffffffffp-2,
    0x1.fffffffffffffp-1,
    -0x1.0p-1074
  },
  { // Entry 76
    0x1.ffffffffffffe000000000000080p-2,
    0x1.fffffffffffffp-2,
    0x1.fffffffffffffp-1,
    -0.0
  },
  { // Entry 77
    0x1.ffffffffffffe000000000000080p-2,
    0x1.fffffffffffffp-2,
    0x1.fffffffffffffp-1,
    0x1.0p-1074
  },
  { // Entry 78
    0x1.ffffffffffffefffffffffffffffffffp-2,
    0x1.fffffffffffffp-2,
    0x1.0p0,
    -0x1.0p-1074
  },
  { // Entry 79
    0x1.fffffffffffff0p-2,
    0x1.fffffffffffffp-2,
    0x1.0p0,
    -0.0
  },
  { // Entry 80
    0x1.fffffffffffff0p-2,
    0x1.fffffffffffffp-2,
    0x1.0p0,
    0x1.0p-1074
  },
  { // Entry 81
    0x1.00000000000007ffffffffffff7fffffp-1,
    0x1.fffffffffffffp-2,
    0x1.0000000000001p0,
    -0x1.0p-1074
  },
  { // Entry 82
    0x1.00000000000007ffffffffffff80p-1,
    0x1.fffffffffffffp-2,
    0x1.0000000000001p0,
    -0.0
  },
  { // Entry 83
    0x1.00000000000007ffffffffffff80p-1,
    0x1.fffffffffffffp-2,
    0x1.0000000000001p0,
    0x1.0p-1074
  },
  { // Entry 84
    0x1.ffffffffffffefffffffffffffffffffp-2,
    0x1.0p-1,
    0x1.fffffffffffffp-1,
    -0x1.0p-1074
  },
  { // Entry 85
    0x1.fffffffffffff0p-2,
    0x1.0p-1,
    0x1.fffffffffffffp-1,
    -0.0
  },
  { // Entry 86
    0x1.fffffffffffff0p-2,
    0x1.0p-1,
    0x1.fffffffffffffp-1,
    0x1.0p-1074
  },
  { // Entry 87
    0x1.ffffffffffffffffffffffffffffffffp-2,
    0x1.0p-1,
    0x1.0p0,
    -0x1.0p-1074
  },
  { // Entry 88
    0x1.p-1,
    0x1.0p-1,
    0x1.0p0,
    -0.0
  },
  { // Entry 89
    0x1.p-1,
    0x1.0p-1,
    0x1.0p0,
    0x1.0p-1074
  },
  { // Entry 90
    0x1.0000000000000fffffffffffffffffffp-1,
    0x1.0p-1,
    0x1.0000000000001p0,
    -0x1.0p-1074
  },
  { // Entry 91
    0x1.00000000000010p-1,
    0x1.0p-1,
    0x1.0000000000001p0,
    -0.0
  },
  { // Entry 92
    0x1.00000000000010p-1,
    0x1.0p-1,
    0x1.0000000000001p0,
    0x1.0p-1074
  },
  { // Entry 93
    0x1.00000000000007ffffffffffff7fffffp-1,
    0x1.0000000000001p-1,
    0x1.fffffffffffffp-1,
    -0x1.0p-1074
  },
  { // Entry 94
    0x1.00000000000007ffffffffffff80p-1,
    0x1.0000000000001p-1,
    0x1.fffffffffffffp-1,
    -0.0
  },
  { // Entry 95
    0x1.00000000000007ffffffffffff80p-1,
    0x1.0000000000001p-1,
    0x1.fffffffffffffp-1,
    0x1.0p-1074
  },
  { // Entry 96
    0x1.0000000000000fffffffffffffffffffp-1,
    0x1.0000000000001p-1,
    0x1.0p0,
    -0x1.0p-1074
  },
  { // Entry 97
    0x1.00000000000010p-1,
    0x1.0000000000001p-1,
    0x1.0p0,
    -0.0
  },
  { // Entry 98
    0x1.00000000000010p-1,
    0x1.0000000000001p-1,
    0x1.0p0,
    0x1.0p-1074
  },
  { // Entry 99
    0x1.00000000000020000000000000ffffffp-1,
    0x1.0000000000001p-1,
    0x1.0000000000001p0,
    -0x1.0p-1074
  },
  { // Entry 100
    0x1.00000000000020000000000001p-1,
    0x1.0000000000001p-1,
    0x1.0000000000001p0,
    -0.0
  },
  { // Entry 101
    0x1.00000000000020000000000001p-1,
    0x1.0000000000001p-1,
    0x1.0000000000001p0,
    0x1.0p-1074
  },
  { // Entry 102
    0x1.0000000000000fffffffffffff40p-1,
    0x1.fffffffffffffp-1,
    0x1.fffffffffffffp-2,
    0x1.fffffffffffffp-53
  },
  { // Entry 103
    0x1.0000000000001000000000000040p-1,
    0x1.fffffffffffffp-1,
    0x1.fffffffffffffp-2,
    0x1.0p-52
  },
  { // Entry 104
    0x1.0000000000001000000000000240p-1,
    0x1.fffffffffffffp-1,
    0x1.fffffffffffffp-2,
    0x1.0000000000001p-52
  },
  { // Entry 105
    0x1.00000000000017ffffffffffffp-1,
    0x1.fffffffffffffp-1,
    0x1.0p-1,
    0x1.fffffffffffffp-53
  },
  { // Entry 106
    0x1.00000000000018p-1,
    0x1.fffffffffffffp-1,
    0x1.0p-1,
    0x1.0p-52
  },
  { // Entry 107
    0x1.00000000000018000000000002p-1,
    0x1.fffffffffffffp-1,
    0x1.0p-1,
    0x1.0000000000001p-52
  },
  { // Entry 108
    0x1.00000000000027fffffffffffe80p-1,
    0x1.fffffffffffffp-1,
    0x1.0000000000001p-1,
    0x1.fffffffffffffp-53
  },
  { // Entry 109
    0x1.00000000000027ffffffffffff80p-1,
    0x1.fffffffffffffp-1,
    0x1.0000000000001p-1,
    0x1.0p-52
  },
  { // Entry 110
    0x1.0000000000002800000000000180p-1,
    0x1.fffffffffffffp-1,
    0x1.0000000000001p-1,
    0x1.0000000000001p-52
  },
  { // Entry 111
    0x1.00000000000017ffffffffffffp-1,
    0x1.0p0,
    0x1.fffffffffffffp-2,
    0x1.fffffffffffffp-53
  },
  { // Entry 112
    0x1.00000000000018p-1,
    0x1.0p0,
    0x1.fffffffffffffp-2,
    0x1.0p-52
  },
  { // Entry 113
    0x1.00000000000018000000000002p-1,
    0x1.0p0,
    0x1.fffffffffffffp-2,
    0x1.0000000000001p-52
  },
  { // Entry 114
    0x1.0000000000001fffffffffffffp-1,
    0x1.0p0,
    0x1.0p-1,
    0x1.fffffffffffffp-53
  },
  { // Entry 115
    0x1.00000000000020p-1,
    0x1.0p0,
    0x1.0p-1,
    0x1.0p-52
  },
  { // Entry 116
    0x1.00000000000020000000000002p-1,
    0x1.0p0,
    0x1.0p-1,
    0x1.0000000000001p-52
  },
  { // Entry 117
    0x1.0000000000002fffffffffffffp-1,
    0x1.0p0,
    0x1.0000000000001p-1,
    0x1.fffffffffffffp-53
  },
  { // Entry 118
    0x1.00000000000030p-1,
    0x1.0p0,
    0x1.0000000000001p-1,
    0x1.0p-52
  },
  { // Entry 119
    0x1.00000000000030000000000002p-1,
    0x1.0p0,
    0x1.0000000000001p-1,
    0x1.0000000000001p-52
  },
  { // Entry 120
    0x1.00000000000027fffffffffffe80p-1,
    0x1.0000000000001p0,
    0x1.fffffffffffffp-2,
    0x1.fffffffffffffp-53
  },
  { // Entry 121
    0x1.00000000000027ffffffffffff80p-1,
    0x1.0000000000001p0,
    0x1.fffffffffffffp-2,
    0x1.0p-52
  },
  { // Entry 122
    0x1.0000000000002800000000000180p-1,
    0x1.0000000000001p0,
    0x1.fffffffffffffp-2,
    0x1.0000000000001p-52
  },
  { // Entry 123
    0x1.0000000000002fffffffffffffp-1,
    0x1.0000000000001p0,
    0x1.0p-1,
    0x1.fffffffffffffp-53
  },
  { // Entry 124
    0x1.00000000000030p-1,
    0x1.0000000000001p0,
    0x1.0p-1,
    0x1.0p-52
  },
  { // Entry 125
    0x1.00000000000030000000000002p-1,
    0x1.0000000000001p0,
    0x1.0p-1,
    0x1.0000000000001p-52
  },
  { // Entry 126
    0x1.00000000000040p-1,
    0x1.0000000000001p0,
    0x1.0000000000001p-1,
    0x1.fffffffffffffp-53
  },
  { // Entry 127
    0x1.00000000000040000000000001p-1,
    0x1.0000000000001p0,
    0x1.0000000000001p-1,
    0x1.0p-52
  },
  { // Entry 128
    0x1.00000000000040000000000003p-1,
    0x1.0000000000001p0,
    0x1.0000000000001p-1,
    0x1.0000000000001p-52
  },
  { // Entry 129
    0x1.0000000000000fffffffffffff40p-1,
    0x1.fffffffffffffp-1,
    0x1.fffffffffffffp-2,
    0x1.fffffffffffffp-53
  },
  { // Entry 130
    0x1.0000000000001000000000000040p-1,
    0x1.fffffffffffffp-1,
    0x1.fffffffffffffp-2,
    0x1.0p-52
  },
  { // Entry 131
    0x1.0000000000001000000000000240p-1,
    0x1.fffffffffffffp-1,
    0x1.fffffffffffffp-2,
    0x1.0000000000001p-52
  },
  { // Entry 132
    0x1.00000000000017ffffffffffffp-1,
    0x1.fffffffffffffp-1,
    0x1.0p-1,
    0x1.fffffffffffffp-53
  },
  { // Entry 133
    0x1.00000000000018p-1,
    0x1.fffffffffffffp-1,
    0x1.0p-1,
    0x1.0p-52
  },
  { // Entry 134
    0x1.00000000000018000000000002p-1,
    0x1.fffffffffffffp-1,
    0x1.0p-1,
    0x1.0000000000001p-52
  },
  { // Entry 135
    0x1.00000000000027fffffffffffe80p-1,
    0x1.fffffffffffffp-1,
    0x1.0000000000001p-1,
    0x1.fffffffffffffp-53
  },
  { // Entry 136
    0x1.00000000000027ffffffffffff80p-1,
    0x1.fffffffffffffp-1,
    0x1.0000000000001p-1,
    0x1.0p-52
  },
  { // Entry 137
    0x1.0000000000002800000000000180p-1,
    0x1.fffffffffffffp-1,
    0x1.0000000000001p-1,
    0x1.0000000000001p-52
  },
  { // Entry 138
    0x1.00000000000017ffffffffffffp-1,
    0x1.0p0,
    0x1.fffffffffffffp-2,
    0x1.fffffffffffffp-53
  },
  { // Entry 139
    0x1.00000000000018p-1,
    0x1.0p0,
    0x1.fffffffffffffp-2,
    0x1.0p-52
  },
  { // Entry 140
    0x1.00000000000018000000000002p-1,
    0x1.0p0,
    0x1.fffffffffffffp-2,
    0x1.0000000000001p-52
  },
  { // Entry 141
    0x1.0000000000001fffffffffffffp-1,
    0x1.0p0,
    0x1.0p-1,
    0x1.fffffffffffffp-53
  },
  { // Entry 142
    0x1.00000000000020p-1,
    0x1.0p0,
    0x1.0p-1,
    0x1.0p-52
  },
  { // Entry 143
    0x1.00000000000020000000000002p-1,
    0x1.0p0,
    0x1.0p-1,
    0x1.0000000000001p-52
  },
  { // Entry 144
    0x1.0000000000002fffffffffffffp-1,
    0x1.0p0,
    0x1.0000000000001p-1,
    0x1.fffffffffffffp-53
  },
  { // Entry 145
    0x1.00000000000030p-1,
    0x1.0p0,
    0x1.0000000000001p-1,
    0x1.0p-52
  },
  { // Entry 146
    0x1.00000000000030000000000002p-1,
    0x1.0p0,
    0x1.0000000000001p-1,
    0x1.0000000000001p-52
  },
  { // Entry 147
    0x1.00000000000027fffffffffffe80p-1,
    0x1.0000000000001p0,
    0x1.fffffffffffffp-2,
    0x1.fffffffffffffp-53
  },
  { // Entry 148
    0x1.00000000000027ffffffffffff80p-1,
    0x1.0000000000001p0,
    0x1.fffffffffffffp-2,
    0x1.0p-52
  },
  { // Entry 149
    0x1.0000000000002800000000000180p-1,
    0x1.0000000000001p0,
    0x1.fffffffffffffp-2,
    0x1.0000000000001p-52
  },
  { // Entry 150
    0x1.0000000000002fffffffffffffp-1,
    0x1.0000000000001p0,
    0x1.0p-1,
    0x1.fffffffffffffp-53
  },
  { // Entry 151
    0x1.00000000000030p-1,
    0x1.0000000000001p0,
    0x1.0p-1,
    0x1.0p-52
  },
  { // Entry 152
    0x1.00000000000030000000000002p-1,
    0x1.0000000000001p0,
    0x1.0p-1,
    0x1.0000000000001p-52
  },
  { // Entry 153
    0x1.00000000000040p-1,
    0x1.0000000000001p0,
    0x1.0000000000001p-1,
    0x1.fffffffffffffp-53
  },
  { // Entry 154
    0x1.00000000000040000000000001p-1,
    0x1.0000000000001p0,
    0x1.0000000000001p-1,
    0x1.0p-52
  },
  { // Entry 155
    0x1.00000000000040000000000003p-1,
    0x1.0000000000001p0,
    0x1.0000000000001p-1,
    0x1.0000000000001p-52
  },
  { // Entry 156
    0x1.0000000000006ffffffffffffc40p-1,
    0x1.fffffffffffffp-1,
    0x1.fffffffffffffp-2,
    0x1.fffffffffffffp-51
  },
  { // Entry 157
    0x1.0000000000007000000000000040p-1,
    0x1.fffffffffffffp-1,
    0x1.fffffffffffffp-2,
    0x1.0p-50
  },
  { // Entry 158
    0x1.0000000000007000000000000840p-1,
    0x1.fffffffffffffp-1,
    0x1.fffffffffffffp-2,
    0x1.0000000000001p-50
  },
  { // Entry 159
    0x1.00000000000077fffffffffffcp-1,
    0x1.fffffffffffffp-1,
    0x1.0p-1,
    0x1.fffffffffffffp-51
  },
  { // Entry 160
    0x1.00000000000078p-1,
    0x1.fffffffffffffp-1,
    0x1.0p-1,
    0x1.0p-50
  },
  { // Entry 161
    0x1.00000000000078000000000008p-1,
    0x1.fffffffffffffp-1,
    0x1.0p-1,
    0x1.0000000000001p-50
  },
  { // Entry 162
    0x1.00000000000087fffffffffffb80p-1,
    0x1.fffffffffffffp-1,
    0x1.0000000000001p-1,
    0x1.fffffffffffffp-51
  },
  { // Entry 163
    0x1.00000000000087ffffffffffff80p-1,
    0x1.fffffffffffffp-1,
    0x1.0000000000001p-1,
    0x1.0p-50
  },
  { // Entry 164
    0x1.0000000000008800000000000780p-1,
    0x1.fffffffffffffp-1,
    0x1.0000000000001p-1,
    0x1.0000000000001p-50
  },
  { // Entry 165
    0x1.00000000000077fffffffffffcp-1,
    0x1.0p0,
    0x1.fffffffffffffp-2,
    0x1.fffffffffffffp-51
  },
  { // Entry 166
    0x1.00000000000078p-1,
    0x1.0p0,
    0x1.fffffffffffffp-2,
    0x1.0p-50
  },
  { // Entry 167
    0x1.00000000000078000000000008p-1,
    0x1.0p0,
    0x1.fffffffffffffp-2,
    0x1.0000000000001p-50
  },
  { // Entry 168
    0x1.0000000000007ffffffffffffcp-1,
    0x1.0p0,
    0x1.0p-1,
    0x1.fffffffffffffp-51
  },
  { // Entry 169
    0x1.00000000000080p-1,
    0x1.0p0,
    0x1.0p-1,
    0x1.0p-50
  },
  { // Entry 170
    0x1.00000000000080000000000008p-1,
    0x1.0p0,
    0x1.0p-1,
    0x1.0000000000001p-50
  },
  { // Entry 171
    0x1.0000000000008ffffffffffffcp-1,
    0x1.0p0,
    0x1.0000000000001p-1,
    0x1.fffffffffffffp-51
  },
  { // Entry 172
    0x1.00000000000090p-1,
    0x1.0p0,
    0x1.0000000000001p-1,
    0x1.0p-50
  },
  { // Entry 173
    0x1.00000000000090000000000008p-1,
    0x1.0p0,
    0x1.0000000000001p-1,
    0x1.0000000000001p-50
  },
  { // Entry 174
    0x1.00000000000087fffffffffffb80p-1,
    0x1.0000000000001p0,
    0x1.fffffffffffffp-2,
    0x1.fffffffffffffp-51
  },
  { // Entry 175
    0x1.00000000000087ffffffffffff80p-1,
    0x1.0000000000001p0,
    0x1.fffffffffffffp-2,
    0x1.0p-50
  },
  { // Entry 176
    0x1.0000000000008800000000000780p-1,
    0x1.0000000000001p0,
    0x1.fffffffffffffp-2,
    0x1.0000000000001p-50
  },
  { // Entry 177
    0x1.0000000000008ffffffffffffcp-1,
    0x1.0000000000001p0,
    0x1.0p-1,
    0x1.fffffffffffffp-51
  },
  { // Entry 178
    0x1.00000000000090p-1,
    0x1.0000000000001p0,
    0x1.0p-1,
    0x1.0p-50
  },
  { // Entry 179
    0x1.00000000000090000000000008p-1,
    0x1.0000000000001p0,
    0x1.0p-1,
    0x1.0000000000001p-50
  },
  { // Entry 180
    0x1.0000000000009ffffffffffffdp-1,
    0x1.0000000000001p0,
    0x1.0000000000001p-1,
    0x1.fffffffffffffp-51
  },
  { // Entry 181
    0x1.000000000000a0000000000001p-1,
    0x1.0000000000001p0,
    0x1.0000000000001p-1,
    0x1.0p-50
  },
  { // Entry 182
    0x1.000000000000a0000000000009p-1,
    0x1.0000000000001p0,
    0x1.0000000000001p-1,
    0x1.0000000000001p-50
  },
  { // Entry 183
    0x1.fffffffffffff0p-2,
    0x1.fffffffffffffp-1,
    0x1.fffffffffffffp-2,
    0x1.fffffffffffffp-55
  },
  { // Entry 184
    0x1.fffffffffffff000000000000080p-2,
    0x1.fffffffffffffp-1,
    0x1.fffffffffffffp-2,
    0x1.0p-54
  },
  { // Entry 185
    0x1.fffffffffffff000000000000180p-2,
    0x1.fffffffffffffp-1,
    0x1.fffffffffffffp-2,
    0x1.0000000000001p-54
  },
  { // Entry 186
    0x1.ffffffffffffffffffffffffff80p-2,
    0x1.fffffffffffffp-1,
    0x1.0p-1,
    0x1.fffffffffffffp-55
  },
  { // Entry 187
    0x1.p-1,
    0x1.fffffffffffffp-1,
    0x1.0p-1,
    0x1.0p-54
  },
  { // Entry 188
    0x1.0000000000000000000000000080p-1,
    0x1.fffffffffffffp-1,
    0x1.0p-1,
    0x1.0000000000001p-54
  },
  { // Entry 189
    0x1.0000000000000fffffffffffff40p-1,
    0x1.fffffffffffffp-1,
    0x1.0000000000001p-1,
    0x1.fffffffffffffp-55
  },
  { // Entry 190
    0x1.0000000000000fffffffffffff80p-1,
    0x1.fffffffffffffp-1,
    0x1.0000000000001p-1,
    0x1.0p-54
  },
  { // Entry 191
    0x1.00000000000010p-1,
    0x1.fffffffffffffp-1,
    0x1.0000000000001p-1,
    0x1.0000000000001p-54
  },
  { // Entry 192
    0x1.ffffffffffffffffffffffffff80p-2,
    0x1.0p0,
    0x1.fffffffffffffp-2,
    0x1.fffffffffffffp-55
  },
  { // Entry 193
    0x1.p-1,
    0x1.0p0,
    0x1.fffffffffffffp-2,
    0x1.0p-54
  },
  { // Entry 194
    0x1.0000000000000000000000000080p-1,
    0x1.0p0,
    0x1.fffffffffffffp-2,
    0x1.0000000000001p-54
  },
  { // Entry 195
    0x1.00000000000007ffffffffffffc0p-1,
    0x1.0p0,
    0x1.0p-1,
    0x1.fffffffffffffp-55
  },
  { // Entry 196
    0x1.00000000000008p-1,
    0x1.0p0,
    0x1.0p-1,
    0x1.0p-54
  },
  { // Entry 197
    0x1.0000000000000800000000000080p-1,
    0x1.0p0,
    0x1.0p-1,
    0x1.0000000000001p-54
  },
  { // Entry 198
    0x1.00000000000017ffffffffffffc0p-1,
    0x1.0p0,
    0x1.0000000000001p-1,
    0x1.fffffffffffffp-55
  },
  { // Entry 199
    0x1.00000000000018p-1,
    0x1.0p0,
    0x1.0000000000001p-1,
    0x1.0p-54
  },
  { // Entry 200
    0x1.0000000000001800000000000080p-1,
    0x1.0p0,
    0x1.0000000000001p-1,
    0x1.0000000000001p-54
  },
  { // Entry 201
    0x1.0000000000000fffffffffffff40p-1,
    0x1.0000000000001p0,
    0x1.fffffffffffffp-2,
    0x1.fffffffffffffp-55
  },
  { // Entry 202
    0x1.0000000000000fffffffffffff80p-1,
    0x1.0000000000001p0,
    0x1.fffffffffffffp-2,
    0x1.0p-54
  },
  { // Entry 203
    0x1.00000000000010p-1,
    0x1.0000000000001p0,
    0x1.fffffffffffffp-2,
    0x1.0000000000001p-54
  },
  { // Entry 204
    0x1.00000000000017ffffffffffffc0p-1,
    0x1.0000000000001p0,
    0x1.0p-1,
    0x1.fffffffffffffp-55
  },
  { // Entry 205
    0x1.00000000000018p-1,
    0x1.0000000000001p0,
    0x1.0p-1,
    0x1.0p-54
  },
  { // Entry 206
    0x1.0000000000001800000000000080p-1,
    0x1.0000000000001p0,
    0x1.0p-1,
    0x1.0000000000001p-54
  },
  { // Entry 207
    0x1.00000000000028000000000000c0p-1,
    0x1.0000000000001p0,
    0x1.0000000000001p-1,
    0x1.fffffffffffffp-55
  },
  { // Entry 208
    0x1.00000000000028000000000001p-1,
    0x1.0000000000001p0,
    0x1.0000000000001p-1,
    0x1.0p-54
  },
  { // Entry 209
    0x1.0000000000002800000000000180p-1,
    0x1.0000000000001p0,
    0x1.0000000000001p-1,
    0x1.0000000000001p-54
  },
  { // Entry 210
    0x1.ffffffffffffe00000000000007fffffp1023,
    0x1.fffffffffffffp1023,
    0x1.fffffffffffffp-1,
    -0x1.0p-1074
  },
  { // Entry 211
    0x1.ffffffffffffe000000000000080p1023,
    0x1.fffffffffffffp1023,
    0x1.fffffffffffffp-1,
    -0.0
  },
  { // Entry 212
    0x1.ffffffffffffe000000000000080p1023,
    0x1.fffffffffffffp1023,
    0x1.fffffffffffffp-1,
    0x1.0p-1074
  },
  { // Entry 213
    0x1.ffffffffffffefffffffffffffffffffp1023,
    0x1.fffffffffffffp1023,
    0x1.0p0,
    -0x1.0p-1074
  },
  { // Entry 214
    0x1.fffffffffffff0p1023,
    0x1.fffffffffffffp1023,
    0x1.0p0,
    -0.0
  },
  { // Entry 215
    0x1.fffffffffffff0p1023,
    0x1.fffffffffffffp1023,
    0x1.0p0,
    0x1.0p-1074
  },
  { // Entry 216
    HUGE_VAL,
    0x1.fffffffffffffp1023,
    0x1.0000000000001p0,
    -0x1.0p-1074
  },
  { // Entry 217
    HUGE_VAL,
    0x1.fffffffffffffp1023,
    0x1.0000000000001p0,
    -0.0
  },
  { // Entry 218
    HUGE_VAL,
    0x1.fffffffffffffp1023,
    0x1.0000000000001p0,
    0x1.0p-1074
  },
  { // Entry 219
    -0x1.fffffffffffff0p970,
    0x1.fffffffffffffp1023,
    0x1.fffffffffffffp-1,
    -0x1.fffffffffffffp1023
  },
  { // Entry 220
    0.0,
    0x1.fffffffffffffp1023,
    0x1.0p0,
    -0x1.fffffffffffffp1023
  },
  { // Entry 221
    0x1.fffffffffffff0p971,
    0x1.fffffffffffffp1023,
    0x1.0000000000001p0,
    -0x1.fffffffffffffp1023
  },
  { // Entry 222
    0x1.ffffffffffffd0000000000000ffffffp1023,
    0x1.fffffffffffffp511,
    0x1.ffffffffffffep511,
    -0x1.0p-1074
  },
  { // Entry 223
    0x1.ffffffffffffd0000000000001p1023,
    0x1.fffffffffffffp511,
    0x1.ffffffffffffep511,
    -0.0
  },
  { // Entry 224
    0x1.ffffffffffffd0000000000001p1023,
    0x1.fffffffffffffp511,
    0x1.ffffffffffffep511,
    0x1.0p-1074
  },
  { // Entry 225
    0x1.ffffffffffffe00000000000007fffffp1023,
    0x1.fffffffffffffp511,
    0x1.fffffffffffffp511,
    -0x1.0p-1074
  },
  { // Entry 226
    0x1.ffffffffffffe000000000000080p1023,
    0x1.fffffffffffffp511,
    0x1.fffffffffffffp511,
    -0.0
  },
  { // Entry 227
    0x1.ffffffffffffe000000000000080p1023,
    0x1.fffffffffffffp511,
    0x1.fffffffffffffp511,
    0x1.0p-1074
  },
  { // Entry 228
    0x1.ffffffffffffefffffffffffffffffffp1023,
    0x1.fffffffffffffp511,
    0x1.0p512,
    -0x1.0p-1074
  },
  { // Entry 229
    0x1.fffffffffffff0p1023,
    0x1.fffffffffffffp511,
    0x1.0p512,
    -0.0
  },
  { // Entry 230
    0x1.fffffffffffff0p1023,
    0x1.fffffffffffffp511,
    0x1.0p512,
    0x1.0p-1074
  },
  { // Entry 231
    -0x1.7ffffffffffff0p972,
    0x1.ffffffffffffep511,
    0x1.ffffffffffffep511,
    -0x1.fffffffffffffp1023
  },
  { // Entry 232
    -0x1.fffffffffffff0p971,
    0x1.ffffffffffffep511,
    0x1.fffffffffffffp511,
    -0x1.fffffffffffffp1023
  },
  { // Entry 233
    -0x1.p971,
    0x1.ffffffffffffep511,
    0x1.0p512,
    -0x1.fffffffffffffp1023
  },
  { // Entry 234
    -0x1.fffffffffffff0p971,
    0x1.fffffffffffffp511,
    0x1.ffffffffffffep511,
    -0x1.fffffffffffffp1023
  },
  { // Entry 235
    -0x1.fffffffffffff0p970,
    0x1.fffffffffffffp511,
    0x1.fffffffffffffp511,
    -0x1.fffffffffffffp1023
  },
  { // Entry 236
    0.0,
    0x1.fffffffffffffp511,
    0x1.0p512,
    -0x1.fffffffffffffp1023
  },
  { // Entry 237
    -0x1.p971,
    0x1.0p512,
    0x1.ffffffffffffep511,
    -0x1.fffffffffffffp1023
  },
  { // Entry 238
    0.0,
    0x1.0p512,
    0x1.fffffffffffffp511,
    -0x1.fffffffffffffp1023
  },
  { // Entry 239
    0x1.p971,
    0x1.0p512,
    0x1.0p512,
    -0x1.fffffffffffffp1023
  },
  { // Entry 240
    -0x1.ffffffffffffc0p970,
    0x1.ffffffffffffep1022,
    -0x1.0000000000001p1,
    0x1.fffffffffffffp1023
  },
  { // Entry 241
    0x1.p971,
    0x1.ffffffffffffep1022,
    -0x1.0p1,
    0x1.fffffffffffffp1023
  },
  { // Entry 242
    0x1.fffffffffffff0p971,
    0x1.ffffffffffffep1022,
    -0x1.fffffffffffffp0,
    0x1.fffffffffffffp1023
  },
  { // Entry 243
    -0x1.fffffffffffff0p971,
    0x1.fffffffffffffp1022,
    -0x1.0000000000001p1,
    0x1.fffffffffffffp1023
  },
  { // Entry 244
    0.0,
    0x1.fffffffffffffp1022,
    -0x1.0p1,
    0x1.fffffffffffffp1023
  },
  { // Entry 245
    0x1.fffffffffffff0p970,
    0x1.fffffffffffffp1022,
    -0x1.fffffffffffffp0,
    0x1.fffffffffffffp1023
  },
  { // Entry 246
    -0x1.80p972,
    0x1.0p1023,
    -0x1.0000000000001p1,
    0x1.fffffffffffffp1023
  },
  { // Entry 247
    -0x1.p971,
    0x1.0p1023,
    -0x1.0p1,
    0x1.fffffffffffffp1023
  },
  { // Entry 248
    0.0,
    0x1.0p1023,
    -0x1.fffffffffffffp0,
    0x1.fffffffffffffp1023
  },
  { // Entry 249
    -HUGE_VAL,
    0x1.ffffffffffffep1022,
    -0x1.0000000000001p1,
    -0x1.0p-1074
  },
  { // Entry 250
    -HUGE_VAL,
    0x1.ffffffffffffep1022,
    -0x1.0000000000001p1,
    -0.0
  },
  { // Entry 251
    -HUGE_VAL,
    0x1.ffffffffffffep1022,
    -0x1.0000000000001p1,
    0x1.0p-1074
  },
  { // Entry 252
    -0x1.ffffffffffffe0p1023,
    0x1.ffffffffffffep1022,
    -0x1.0p1,
    -0x1.0p-1074
  },
  { // Entry 253
    -0x1.ffffffffffffe0p1023,
    0x1.ffffffffffffep1022,
    -0x1.0p1,
    -0.0
  },
  { // Entry 254
    -0x1.ffffffffffffdfffffffffffffffffffp1023,
    0x1.ffffffffffffep1022,
    -0x1.0p1,
    0x1.0p-1074
  },
  { // Entry 255
    -0x1.ffffffffffffd0000000000001p1023,
    0x1.ffffffffffffep1022,
    -0x1.fffffffffffffp0,
    -0x1.0p-1074
  },
  { // Entry 256
    -0x1.ffffffffffffd0000000000001p1023,
    0x1.ffffffffffffep1022,
    -0x1.fffffffffffffp0,
    -0.0
  },
  { // Entry 257
    -0x1.ffffffffffffd0000000000000ffffffp1023,
    0x1.ffffffffffffep1022,
    -0x1.fffffffffffffp0,
    0x1.0p-1074
  },
  { // Entry 258
    -HUGE_VAL,
    0x1.fffffffffffffp1022,
    -0x1.0000000000001p1,
    -0x1.0p-1074
  },
  { // Entry 259
    -HUGE_VAL,
    0x1.fffffffffffffp1022,
    -0x1.0000000000001p1,
    -0.0
  },
  { // Entry 260
    -HUGE_VAL,
    0x1.fffffffffffffp1022,
    -0x1.0000000000001p1,
    0x1.0p-1074
  },
  { // Entry 261
    -0x1.fffffffffffff0p1023,
    0x1.fffffffffffffp1022,
    -0x1.0p1,
    -0x1.0p-1074
  },
  { // Entry 262
    -0x1.fffffffffffff0p1023,
    0x1.fffffffffffffp1022,
    -0x1.0p1,
    -0.0
  },
  { // Entry 263
    -0x1.ffffffffffffefffffffffffffffffffp1023,
    0x1.fffffffffffffp1022,
    -0x1.0p1,
    0x1.0p-1074
  },
  { // Entry 264
    -0x1.ffffffffffffe000000000000080p1023,
    0x1.fffffffffffffp1022,
    -0x1.fffffffffffffp0,
    -0x1.0p-1074
  },
  { // Entry 265
    -0x1.ffffffffffffe000000000000080p1023,
    0x1.fffffffffffffp1022,
    -0x1.fffffffffffffp0,
    -0.0
  },
  { // Entry 266
    -0x1.ffffffffffffe00000000000007fffffp1023,
    0x1.fffffffffffffp1022,
    -0x1.fffffffffffffp0,
    0x1.0p-1074
  },
  { // Entry 267
    -HUGE_VAL,
    0x1.0p1023,
    -0x1.0000000000001p1,
    -0x1.0p-1074
  },
  { // Entry 268
    -HUGE_VAL,
    0x1.0p1023,
    -0x1.0000000000001p1,
    -0.0
  },
  { // Entry 269
    -HUGE_VAL,
    0x1.0p1023,
    -0x1.0000000000001p1,
    0x1.0p-1074
  },
  { // Entry 270
    -HUGE_VAL,
    0x1.0p1023,
    -0x1.0p1,
    -0x1.0p-1074
  },
  { // Entry 271
    -HUGE_VAL,
    0x1.0p1023,
    -0x1.0p1,
    -0.0
  },
  { // Entry 272
    -HUGE_VAL,
    0x1.0p1023,
    -0x1.0p1,
    0x1.0p-1074
  },
  { // Entry 273
    -0x1.fffffffffffff0p1023,
    0x1.0p1023,
    -0x1.fffffffffffffp0,
    -0x1.0p-1074
  },
  { // Entry 274
    -0x1.fffffffffffff0p1023,
    0x1.0p1023,
    -0x1.fffffffffffffp0,
    -0.0
  },
  { // Entry 275
    -0x1.ffffffffffffefffffffffffffffffffp1023,
    0x1.0p1023,
    -0x1.fffffffffffffp0,
    0x1.0p-1074
  },
  { // Entry 276
    0x1.7fffffffffffe400000000000040p1023,
    0x1.ffffffffffffep1022,
    0x1.fffffffffffffp-2,
    0x1.ffffffffffffep1022
  },
  { // Entry 277
    0x1.7fffffffffffec00000000000040p1023,
    0x1.ffffffffffffep1022,
    0x1.fffffffffffffp-2,
    0x1.fffffffffffffp1022
  },
  { // Entry 278
    0x1.7ffffffffffff400000000000040p1023,
    0x1.ffffffffffffep1022,
    0x1.fffffffffffffp-2,
    0x1.0p1023
  },
  { // Entry 279
    0x1.7fffffffffffe8p1023,
    0x1.ffffffffffffep1022,
    0x1.0p-1,
    0x1.ffffffffffffep1022
  },
  { // Entry 280
    0x1.7ffffffffffff0p1023,
    0x1.ffffffffffffep1022,
    0x1.0p-1,
    0x1.fffffffffffffp1022
  },
  { // Entry 281
    0x1.7ffffffffffff8p1023,
    0x1.ffffffffffffep1022,
    0x1.0p-1,
    0x1.0p1023
  },
  { // Entry 282
    0x1.7fffffffffffefffffffffffff80p1023,
    0x1.ffffffffffffep1022,
    0x1.0000000000001p-1,
    0x1.ffffffffffffep1022
  },
  { // Entry 283
    0x1.7ffffffffffff7ffffffffffff80p1023,
    0x1.ffffffffffffep1022,
    0x1.0000000000001p-1,
    0x1.fffffffffffffp1022
  },
  { // Entry 284
    0x1.7fffffffffffffffffffffffff80p1023,
    0x1.ffffffffffffep1022,
    0x1.0000000000001p-1,
    0x1.0p1023
  },
  { // Entry 285
    0x1.7fffffffffffe800000000000020p1023,
    0x1.fffffffffffffp1022,
    0x1.fffffffffffffp-2,
    0x1.ffffffffffffep1022
  },
  { // Entry 286
    0x1.7ffffffffffff000000000000020p1023,
    0x1.fffffffffffffp1022,
    0x1.fffffffffffffp-2,
    0x1.fffffffffffffp1022
  },
  { // Entry 287
    0x1.7ffffffffffff800000000000020p1023,
    0x1.fffffffffffffp1022,
    0x1.fffffffffffffp-2,
    0x1.0p1023
  },
  { // Entry 288
    0x1.7fffffffffffecp1023,
    0x1.fffffffffffffp1022,
    0x1.0p-1,
    0x1.ffffffffffffep1022
  },
  { // Entry 289
    0x1.7ffffffffffff4p1023,
    0x1.fffffffffffffp1022,
    0x1.0p-1,
    0x1.fffffffffffffp1022
  },
  { // Entry 290
    0x1.7ffffffffffffcp1023,
    0x1.fffffffffffffp1022,
    0x1.0p-1,
    0x1.0p1023
  },
  { // Entry 291
    0x1.7ffffffffffff3ffffffffffffc0p1023,
    0x1.fffffffffffffp1022,
    0x1.0000000000001p-1,
    0x1.ffffffffffffep1022
  },
  { // Entry 292
    0x1.7ffffffffffffbffffffffffffc0p1023,
    0x1.fffffffffffffp1022,
    0x1.0000000000001p-1,
    0x1.fffffffffffffp1022
  },
  { // Entry 293
    0x1.80000000000003ffffffffffffc0p1023,
    0x1.fffffffffffffp1022,
    0x1.0000000000001p-1,
    0x1.0p1023
  },
  { // Entry 294
    0x1.7fffffffffffecp1023,
    0x1.0p1023,
    0x1.fffffffffffffp-2,
    0x1.ffffffffffffep1022
  },
  { // Entry 295
    0x1.7ffffffffffff4p1023,
    0x1.0p1023,
    0x1.fffffffffffffp-2,
    0x1.fffffffffffffp1022
  },
  { // Entry 296
    0x1.7ffffffffffffcp1023,
    0x1.0p1023,
    0x1.fffffffffffffp-2,
    0x1.0p1023
  },
  { // Entry 297
    0x1.7ffffffffffff0p1023,
    0x1.0p1023,
    0x1.0p-1,
    0x1.ffffffffffffep1022
  },
  { // Entry 298
    0x1.7ffffffffffff8p1023,
    0x1.0p1023,
    0x1.0p-1,
    0x1.fffffffffffffp1022
  },
  { // Entry 299
    0x1.80p1023,
    0x1.0p1023,
    0x1.0p-1,
    0x1.0p1023
  },
  { // Entry 300
    0x1.7ffffffffffff8p1023,
    0x1.0p1023,
    0x1.0000000000001p-1,
    0x1.ffffffffffffep1022
  },
  { // Entry 301
    0x1.80p1023,
    0x1.0p1023,
    0x1.0000000000001p-1,
    0x1.fffffffffffffp1022
  },
  { // Entry 302
    0x1.80000000000008p1023,
    0x1.0p1023,
    0x1.0000000000001p-1,
    0x1.0p1023
  },
  { // Entry 303
    0x1.ffffffffffffd800000000000080p1023,
    0x1.ffffffffffffep1022,
    0x1.fffffffffffffp-1,
    0x1.ffffffffffffep1022
  },
  { // Entry 304
    0x1.ffffffffffffe000000000000080p1023,
    0x1.ffffffffffffep1022,
    0x1.fffffffffffffp-1,
    0x1.fffffffffffffp1022
  },
  { // Entry 305
    0x1.ffffffffffffe800000000000080p1023,
    0x1.ffffffffffffep1022,
    0x1.fffffffffffffp-1,
    0x1.0p1023
  },
  { // Entry 306
    0x1.ffffffffffffe0p1023,
    0x1.ffffffffffffep1022,
    0x1.0p0,
    0x1.ffffffffffffep1022
  },
  { // Entry 307
    0x1.ffffffffffffe8p1023,
    0x1.ffffffffffffep1022,
    0x1.0p0,
    0x1.fffffffffffffp1022
  },
  { // Entry 308
    0x1.fffffffffffff0p1023,
    0x1.ffffffffffffep1022,
    0x1.0p0,
    0x1.0p1023
  },
  { // Entry 309
    0x1.ffffffffffffefffffffffffffp1023,
    0x1.ffffffffffffep1022,
    0x1.0000000000001p0,
    0x1.ffffffffffffep1022
  },
  { // Entry 310
    0x1.fffffffffffff7ffffffffffffp1023,
    0x1.ffffffffffffep1022,
    0x1.0000000000001p0,
    0x1.fffffffffffffp1022
  },
  { // Entry 311
    HUGE_VAL,
    0x1.ffffffffffffep1022,
    0x1.0000000000001p0,
    0x1.0p1023
  },
  { // Entry 312
    0x1.ffffffffffffe000000000000040p1023,
    0x1.fffffffffffffp1022,
    0x1.fffffffffffffp-1,
    0x1.ffffffffffffep1022
  },
  { // Entry 313
    0x1.ffffffffffffe800000000000040p1023,
    0x1.fffffffffffffp1022,
    0x1.fffffffffffffp-1,
    0x1.fffffffffffffp1022
  },
  { // Entry 314
    0x1.fffffffffffff000000000000040p1023,
    0x1.fffffffffffffp1022,
    0x1.fffffffffffffp-1,
    0x1.0p1023
  },
  { // Entry 315
    0x1.ffffffffffffe8p1023,
    0x1.fffffffffffffp1022,
    0x1.0p0,
    0x1.ffffffffffffep1022
  },
  { // Entry 316
    0x1.fffffffffffff0p1023,
    0x1.fffffffffffffp1022,
    0x1.0p0,
    0x1.fffffffffffffp1022
  },
  { // Entry 317
    HUGE_VAL,
    0x1.fffffffffffffp1022,
    0x1.0p0,
    0x1.0p1023
  },
  { // Entry 318
    0x1.fffffffffffff7ffffffffffff80p1023,
    0x1.fffffffffffffp1022,
    0x1.0000000000001p0,
    0x1.ffffffffffffep1022
  },
  { // Entry 319
    HUGE_VAL,
    0x1.fffffffffffffp1022,
    0x1.0000000000001p0,
    0x1.fffffffffffffp1022
  },
  { // Entry 320
    HUGE_VAL,
    0x1.fffffffffffffp1022,
    0x1.0000000000001p0,
    0x1.0p1023
  },
  { // Entry 321
    0x1.ffffffffffffe8p1023,
    0x1.0p1023,
    0x1.fffffffffffffp-1,
    0x1.ffffffffffffep1022
  },
  { // Entry 322
    0x1.fffffffffffff0p1023,
    0x1.0p1023,
    0x1.fffffffffffffp-1,
    0x1.fffffffffffffp1022
  },
  { // Entry 323
    HUGE_VAL,
    0x1.0p1023,
    0x1.fffffffffffffp-1,
    0x1.0p1023
  },
  { // Entry 324
    0x1.fffffffffffff0p1023,
    0x1.0p1023,
    0x1.0p0,
    0x1.ffffffffffffep1022
  },
  { // Entry 325
    HUGE_VAL,
    0x1.0p1023,
    0x1.0p0,
    0x1.fffffffffffffp1022
  },
  { // Entry 326
    HUGE_VAL,
    0x1.0p1023,
    0x1.0p0,
    0x1.0p1023
  },
  { // Entry 327
    HUGE_VAL,
    0x1.0p1023,
    0x1.0000000000001p0,
    0x1.ffffffffffffep1022
  },
  { // Entry 328
    HUGE_VAL,
    0x1.0p1023,
    0x1.0000000000001p0,
    0x1.fffffffffffffp1022
  },
  { // Entry 329
    HUGE_VAL,
    0x1.0p1023,
    0x1.0000000000001p0,
    0x1.0p1023
  },
  { // Entry 330
    0x1.ffffffffffffd800000000000080p1023,
    0x1.ffffffffffffep1022,
    0x1.fffffffffffffp-1,
    0x1.ffffffffffffep1022
  },
  { // Entry 331
    0x1.ffffffffffffe000000000000080p1023,
    0x1.ffffffffffffep1022,
    0x1.fffffffffffffp-1,
    0x1.fffffffffffffp1022
  },
  { // Entry 332
    0x1.ffffffffffffe800000000000080p1023,
    0x1.ffffffffffffep1022,
    0x1.fffffffffffffp-1,
    0x1.0p1023
  },
  { // Entry 333
    0x1.ffffffffffffe0p1023,
    0x1.ffffffffffffep1022,
    0x1.0p0,
    0x1.ffffffffffffep1022
  },
  { // Entry 334
    0x1.ffffffffffffe8p1023,
    0x1.ffffffffffffep1022,
    0x1.0p0,
    0x1.fffffffffffffp1022
  },
  { // Entry 335
    0x1.fffffffffffff0p1023,
    0x1.ffffffffffffep1022,
    0x1.0p0,
    0x1.0p1023
  },
  { // Entry 336
    0x1.ffffffffffffefffffffffffffp1023,
    0x1.ffffffffffffep1022,
    0x1.0000000000001p0,
    0x1.ffffffffffffep1022
  },
  { // Entry 337
    0x1.fffffffffffff7ffffffffffffp1023,
    0x1.ffffffffffffep1022,
    0x1.0000000000001p0,
    0x1.fffffffffffffp1022
  },
  { // Entry 338
    HUGE_VAL,
    0x1.ffffffffffffep1022,
    0x1.0000000000001p0,
    0x1.0p1023
  },
  { // Entry 339
    0x1.ffffffffffffe000000000000040p1023,
    0x1.fffffffffffffp1022,
    0x1.fffffffffffffp-1,
    0x1.ffffffffffffep1022
  },
  { // Entry 340
    0x1.ffffffffffffe800000000000040p1023,
    0x1.fffffffffffffp1022,
    0x1.fffffffffffffp-1,
    0x1.fffffffffffffp1022
  },
  { // Entry 341
    0x1.fffffffffffff000000000000040p1023,
    0x1.fffffffffffffp1022,
    0x1.fffffffffffffp-1,
    0x1.0p1023
  },
  { // Entry 342
    0x1.ffffffffffffe8p1023,
    0x1.fffffffffffffp1022,
    0x1.0p0,
    0x1.ffffffffffffep1022
  },
  { // Entry 343
    0x1.fffffffffffff0p1023,
    0x1.fffffffffffffp1022,
    0x1.0p0,
    0x1.fffffffffffffp1022
  },
  { // Entry 344
    HUGE_VAL,
    0x1.fffffffffffffp1022,
    0x1.0p0,
    0x1.0p1023
  },
  { // Entry 345
    0x1.fffffffffffff7ffffffffffff80p1023,
    0x1.fffffffffffffp1022,
    0x1.0000000000001p0,
    0x1.ffffffffffffep1022
  },
  { // Entry 346
    HUGE_VAL,
    0x1.fffffffffffffp1022,
    0x1.0000000000001p0,
    0x1.fffffffffffffp1022
  },
  { // Entry 347
    HUGE_VAL,
    0x1.fffffffffffffp1022,
    0x1.0000000000001p0,
    0x1.0p1023
  },
  { // Entry 348
    0x1.ffffffffffffe8p1023,
    0x1.0p1023,
    0x1.fffffffffffffp-1,
    0x1.ffffffffffffep1022
  },
  { // Entry 349
    0x1.fffffffffffff0p1023,
    0x1.0p1023,
    0x1.fffffffffffffp-1,
    0x1.fffffffffffffp1022
  },
  { // Entry 350
    HUGE_VAL,
    0x1.0p1023,
    0x1.fffffffffffffp-1,
    0x1.0p1023
  },
  { // Entry 351
    0x1.fffffffffffff0p1023,
    0x1.0p1023,
    0x1.0p0,
    0x1.ffffffffffffep1022
  },
  { // Entry 352
    HUGE_VAL,
    0x1.0p1023,
    0x1.0p0,
    0x1.fffffffffffffp1022
  },
  { // Entry 353
    HUGE_VAL,
    0x1.0p1023,
    0x1.0p0,
    0x1.0p1023
  },
  { // Entry 354
    HUGE_VAL,
    0x1.0p1023,
    0x1.0000000000001p0,
    0x1.ffffffffffffep1022
  },
  { // Entry 355
    HUGE_VAL,
    0x1.0p1023,
    0x1.0000000000001p0,
    0x1.fffffffffffffp1022
  },
  { // Entry 356
    HUGE_VAL,
    0x1.0p1023,
    0x1.0000000000001p0,
    0x1.0p1023
  },
  { // Entry 357
    0x1.ffffffffffffe800000000000040p0,
    0x1.fffffffffffffp-1,
    0x1.fffffffffffffp-1,
    0x1.fffffffffffffp-1
  },
  { // Entry 358
    0x1.fffffffffffff000000000000040p0,
    0x1.fffffffffffffp-1,
    0x1.fffffffffffffp-1,
    0x1.0p0
  },
  { // Entry 359
    0x1.0000000000000000000000000020p1,
    0x1.fffffffffffffp-1,
    0x1.fffffffffffffp-1,
    0x1.0000000000001p0
  },
  { // Entry 360
    0x1.fffffffffffff0p0,
    0x1.fffffffffffffp-1,
    0x1.0p0,
    0x1.fffffffffffffp-1
  },
  { // Entry 361
    0x1.fffffffffffff8p0,
    0x1.fffffffffffffp-1,
    0x1.0p0,
    0x1.0p0
  },
  { // Entry 362
    0x1.00000000000004p1,
    0x1.fffffffffffffp-1,
    0x1.0p0,
    0x1.0000000000001p0
  },
  { // Entry 363
    0x1.ffffffffffffffffffffffffff80p0,
    0x1.fffffffffffffp-1,
    0x1.0000000000001p0,
    0x1.fffffffffffffp-1
  },
  { // Entry 364
    0x1.00000000000003ffffffffffffc0p1,
    0x1.fffffffffffffp-1,
    0x1.0000000000001p0,
    0x1.0p0
  },
  { // Entry 365
    0x1.0000000000000bffffffffffffc0p1,
    0x1.fffffffffffffp-1,
    0x1.0000000000001p0,
    0x1.0000000000001p0
  },
  { // Entry 366
    0x1.fffffffffffff0p0,
    0x1.0p0,
    0x1.fffffffffffffp-1,
    0x1.fffffffffffffp-1
  },
  { // Entry 367
    0x1.fffffffffffff8p0,
    0x1.0p0,
    0x1.fffffffffffffp-1,
    0x1.0p0
  },
  { // Entry 368
    0x1.00000000000004p1,
    0x1.0p0,
    0x1.fffffffffffffp-1,
    0x1.0000000000001p0
  },
  { // Entry 369
    0x1.fffffffffffff8p0,
    0x1.0p0,
    0x1.0p0,
    0x1.fffffffffffffp-1
  },
  { // Entry 370
    0x1.p1,
    0x1.0p0,
    0x1.0p0,
    0x1.0p0
  },
  { // Entry 371
    0x1.00000000000008p1,
    0x1.0p0,
    0x1.0p0,
    0x1.0000000000001p0
  },
  { // Entry 372
    0x1.00000000000004p1,
    0x1.0p0,
    0x1.0000000000001p0,
    0x1.fffffffffffffp-1
  },
  { // Entry 373
    0x1.00000000000008p1,
    0x1.0p0,
    0x1.0000000000001p0,
    0x1.0p0
  },
  { // Entry 374
    0x1.00000000000010p1,
    0x1.0p0,
    0x1.0000000000001p0,
    0x1.0000000000001p0
  },
  { // Entry 375
    0x1.ffffffffffffffffffffffffff80p0,
    0x1.0000000000001p0,
    0x1.fffffffffffffp-1,
    0x1.fffffffffffffp-1
  },
  { // Entry 376
    0x1.00000000000003ffffffffffffc0p1,
    0x1.0000000000001p0,
    0x1.fffffffffffffp-1,
    0x1.0p0
  },
  { // Entry 377
    0x1.0000000000000bffffffffffffc0p1,
    0x1.0000000000001p0,
    0x1.fffffffffffffp-1,
    0x1.0000000000001p0
  },
  { // Entry 378
    0x1.00000000000004p1,
    0x1.0000000000001p0,
    0x1.0p0,
    0x1.fffffffffffffp-1
  },
  { // Entry 379
    0x1.00000000000008p1,
    0x1.0000000000001p0,
    0x1.0p0,
    0x1.0p0
  },
  { // Entry 380
    0x1.00000000000010p1,
    0x1.0000000000001p0,
    0x1.0p0,
    0x1.0000000000001p0
  },
  { // Entry 381
    0x1.0000000000000c00000000000080p1,
    0x1.0000000000001p0,
    0x1.0000000000001p0,
    0x1.fffffffffffffp-1
  },
  { // Entry 382
    0x1.0000000000001000000000000080p1,
    0x1.0000000000001p0,
    0x1.0000000000001p0,
    0x1.0p0
  },
  { // Entry 383
    0x1.0000000000001800000000000080p1,
    0x1.0000000000001p0,
    0x1.0000000000001p0,
    0x1.0000000000001p0
  },
  { // Entry 384
    -0x1.fffffffffffff0p-53,
    0x1.fffffffffffffp-1,
    -0x1.0000000000001p0,
    0x1.fffffffffffffp-1
  },
  { // Entry 385
    -0x1.ffffffffffffe0p-54,
    0x1.fffffffffffffp-1,
    -0x1.0000000000001p0,
    0x1.0p0
  },
  { // Entry 386
    0x1.00000000000010p-53,
    0x1.fffffffffffffp-1,
    -0x1.0000000000001p0,
    0x1.0000000000001p0
  },
  { // Entry 387
    0.0,
    0x1.fffffffffffffp-1,
    -0x1.0p0,
    0x1.fffffffffffffp-1
  },
  { // Entry 388
    0x1.p-53,
    0x1.fffffffffffffp-1,
    -0x1.0p0,
    0x1.0p0
  },
  { // Entry 389
    0x1.80p-52,
    0x1.fffffffffffffp-1,
    -0x1.0p0,
    0x1.0000000000001p0
  },
  { // Entry 390
    0x1.fffffffffffff0p-54,
    0x1.fffffffffffffp-1,
    -0x1.fffffffffffffp-1,
    0x1.fffffffffffffp-1
  },
  { // Entry 391
    0x1.fffffffffffff8p-53,
    0x1.fffffffffffffp-1,
    -0x1.fffffffffffffp-1,
    0x1.0p0
  },
  { // Entry 392
    0x1.fffffffffffffcp-52,
    0x1.fffffffffffffp-1,
    -0x1.fffffffffffffp-1,
    0x1.0000000000001p0
  },
  { // Entry 393
    -0x1.80p-52,
    0x1.0p0,
    -0x1.0000000000001p0,
    0x1.fffffffffffffp-1
  },
  { // Entry 394
    -0x1.p-52,
    0x1.0p0,
    -0x1.0000000000001p0,
    0x1.0p0
  },
  { // Entry 395
    0.0,
    0x1.0p0,
    -0x1.0000000000001p0,
    0x1.0000000000001p0
  },
  { // Entry 396
    -0x1.p-53,
    0x1.0p0,
    -0x1.0p0,
    0x1.fffffffffffffp-1
  },
  { // Entry 397
    0.0,
    0x1.0p0,
    -0x1.0p0,
    0x1.0p0
  },
  { // Entry 398
    0x1.p-52,
    0x1.0p0,
    -0x1.0p0,
    0x1.0000000000001p0
  },
  { // Entry 399
    0.0,
    0x1.0p0,
    -0x1.fffffffffffffp-1,
    0x1.fffffffffffffp-1
  },
  { // Entry 400
    0x1.p-53,
    0x1.0p0,
    -0x1.fffffffffffffp-1,
    0x1.0p0
  },
  { // Entry 401
    0x1.80p-52,
    0x1.0p0,
    -0x1.fffffffffffffp-1,
    0x1.0000000000001p0
  },
  { // Entry 402
    -0x1.40000000000008p-51,
    0x1.0000000000001p0,
    -0x1.0000000000001p0,
    0x1.fffffffffffffp-1
  },
  { // Entry 403
    -0x1.00000000000008p-51,
    0x1.0000000000001p0,
    -0x1.0000000000001p0,
    0x1.0p0
  },
  { // Entry 404
    -0x1.00000000000010p-52,
    0x1.0000000000001p0,
    -0x1.0000000000001p0,
    0x1.0000000000001p0
  },
  { // Entry 405
    -0x1.80p-52,
    0x1.0000000000001p0,
    -0x1.0p0,
    0x1.fffffffffffffp-1
  },
  { // Entry 406
    -0x1.p-52,
    0x1.0000000000001p0,
    -0x1.0p0,
    0x1.0p0
  },
  { // Entry 407
    0.0,
    0x1.0000000000001p0,
    -0x1.0p0,
    0x1.0000000000001p0
  },
  { // Entry 408
    -0x1.fffffffffffff0p-53,
    0x1.0000000000001p0,
    -0x1.fffffffffffffp-1,
    0x1.fffffffffffffp-1
  },
  { // Entry 409
    -0x1.ffffffffffffe0p-54,
    0x1.0000000000001p0,
    -0x1.fffffffffffffp-1,
    0x1.0p0
  },
  { // Entry 410
    0x1.00000000000010p-53,
    0x1.0000000000001p0,
    -0x1.fffffffffffffp-1,
    0x1.0000000000001p0
  },
  { // Entry 411
    0x1.ffffffffffffe00000000000007fffffp-1,
    0x1.fffffffffffffp-1,
    0x1.fffffffffffffp-1,
    -0x1.0p-1074
  },
  { // Entry 412
    0x1.ffffffffffffe000000000000080p-1,
    0x1.fffffffffffffp-1,
    0x1.fffffffffffffp-1,
    -0.0
  },
  { // Entry 413
    0x1.ffffffffffffe000000000000080p-1,
    0x1.fffffffffffffp-1,
    0x1.fffffffffffffp-1,
    0x1.0p-1074
  },
  { // Entry 414
    0x1.ffffffffffffefffffffffffffffffffp-1,
    0x1.fffffffffffffp-1,
    0x1.0p0,
    -0x1.0p-1074
  },
  { // Entry 415
    0x1.fffffffffffff0p-1,
    0x1.fffffffffffffp-1,
    0x1.0p0,
    -0.0
  },
  { // Entry 416
    0x1.fffffffffffff0p-1,
    0x1.fffffffffffffp-1,
    0x1.0p0,
    0x1.0p-1074
  },
  { // Entry 417
    0x1.00000000000007ffffffffffff7fffffp0,
    0x1.fffffffffffffp-1,
    0x1.0000000000001p0,
    -0x1.0p-1074
  },
  { // Entry 418
    0x1.00000000000007ffffffffffff80p0,
    0x1.fffffffffffffp-1,
    0x1.0000000000001p0,
    -0.0
  },
  { // Entry 419
    0x1.00000000000007ffffffffffff80p0,
    0x1.fffffffffffffp-1,
    0x1.0000000000001p0,
    0x1.0p-1074
  },
  { // Entry 420
    0x1.ffffffffffffefffffffffffffffffffp-1,
    0x1.0p0,
    0x1.fffffffffffffp-1,
    -0x1.0p-1074
  },
  { // Entry 421
    0x1.fffffffffffff0p-1,
    0x1.0p0,
    0x1.fffffffffffffp-1,
    -0.0
  },
  { // Entry 422
    0x1.fffffffffffff0p-1,
    0x1.0p0,
    0x1.fffffffffffffp-1,
    0x1.0p-1074
  },
  { // Entry 423
    0x1.ffffffffffffffffffffffffffffffffp-1,
    0x1.0p0,
    0x1.0p0,
    -0x1.0p-1074
  },
  { // Entry 424
    0x1.p0,
    0x1.0p0,
    0x1.0p0,
    -0.0
  },
  { // Entry 425
    0x1.p0,
    0x1.0p0,
    0x1.0p0,
    0x1.0p-1074
  },
  { // Entry 426
    0x1.0000000000000fffffffffffffffffffp0,
    0x1.0p0,
    0x1.0000000000001p0,
    -0x1.0p-1074
  },
  { // Entry 427
    0x1.00000000000010p0,
    0x1.0p0,
    0x1.0000000000001p0,
    -0.0
  },
  { // Entry 428
    0x1.00000000000010p0,
    0x1.0p0,
    0x1.0000000000001p0,
    0x1.0p-1074
  },
  { // Entry 429
    0x1.00000000000007ffffffffffff7fffffp0,
    0x1.0000000000001p0,
    0x1.fffffffffffffp-1,
    -0x1.0p-1074
  },
  { // Entry 430
    0x1.00000000000007ffffffffffff80p0,
    0x1.0000000000001p0,
    0x1.fffffffffffffp-1,
    -0.0
  },
  { // Entry 431
    0x1.00000000000007ffffffffffff80p0,
    0x1.0000000000001p0,
    0x1.fffffffffffffp-1,
    0x1.0p-1074
  },
  { // Entry 432
    0x1.0000000000000fffffffffffffffffffp0,
    0x1.0000000000001p0,
    0x1.0p0,
    -0x1.0p-1074
  },
  { // Entry 433
    0x1.00000000000010p0,
    0x1.0000000000001p0,
    0x1.0p0,
    -0.0
  },
  { // Entry 434
    0x1.00000000000010p0,
    0x1.0000000000001p0,
    0x1.0p0,
    0x1.0p-1074
  },
  { // Entry 435
    0x1.00000000000020000000000000ffffffp0,
    0x1.0000000000001p0,
    0x1.0000000000001p0,
    -0x1.0p-1074
  },
  { // Entry 436
    0x1.00000000000020000000000001p0,
    0x1.0000000000001p0,
    0x1.0000000000001p0,
    -0.0
  },
  { // Entry 437
    0x1.00000000000020000000000001p0,
    0x1.0000000000001p0,
    0x1.0000000000001p0,
    0x1.0p-1074
  },
  { // Entry 438
    -0x1.00000000000007ffffffffffff80p0,
    0x1.fffffffffffffp-1,
    -0x1.0000000000001p0,
    -0x1.0p-1074
  },
  { // Entry 439
    -0x1.00000000000007ffffffffffff80p0,
    0x1.fffffffffffffp-1,
    -0x1.0000000000001p0,
    -0.0
  },
  { // Entry 440
    -0x1.00000000000007ffffffffffff7fffffp0,
    0x1.fffffffffffffp-1,
    -0x1.0000000000001p0,
    0x1.0p-1074
  },
  { // Entry 441
    -0x1.fffffffffffff0p-1,
    0x1.fffffffffffffp-1,
    -0x1.0p0,
    -0x1.0p-1074
  },
  { // Entry 442
    -0x1.fffffffffffff0p-1,
    0x1.fffffffffffffp-1,
    -0x1.0p0,
    -0.0
  },
  { // Entry 443
    -0x1.ffffffffffffefffffffffffffffffffp-1,
    0x1.fffffffffffffp-1,
    -0x1.0p0,
    0x1.0p-1074
  },
  { // Entry 444
    -0x1.ffffffffffffe000000000000080p-1,
    0x1.fffffffffffffp-1,
    -0x1.fffffffffffffp-1,
    -0x1.0p-1074
  },
  { // Entry 445
    -0x1.ffffffffffffe000000000000080p-1,
    0x1.fffffffffffffp-1,
    -0x1.fffffffffffffp-1,
    -0.0
  },
  { // Entry 446
    -0x1.ffffffffffffe00000000000007fffffp-1,
    0x1.fffffffffffffp-1,
    -0x1.fffffffffffffp-1,
    0x1.0p-1074
  },
  { // Entry 447
    -0x1.00000000000010p0,
    0x1.0p0,
    -0x1.0000000000001p0,
    -0x1.0p-1074
  },
  { // Entry 448
    -0x1.00000000000010p0,
    0x1.0p0,
    -0x1.0000000000001p0,
    -0.0
  },
  { // Entry 449
    -0x1.0000000000000fffffffffffffffffffp0,
    0x1.0p0,
    -0x1.0000000000001p0,
    0x1.0p-1074
  },
  { // Entry 450
    -0x1.p0,
    0x1.0p0,
    -0x1.0p0,
    -0x1.0p-1074
  },
  { // Entry 451
    -0x1.p0,
    0x1.0p0,
    -0x1.0p0,
    -0.0
  },
  { // Entry 452
    -0x1.ffffffffffffffffffffffffffffffffp-1,
    0x1.0p0,
    -0x1.0p0,
    0x1.0p-1074
  },
  { // Entry 453
    -0x1.fffffffffffff0p-1,
    0x1.0p0,
    -0x1.fffffffffffffp-1,
    -0x1.0p-1074
  },
  { // Entry 454
    -0x1.fffffffffffff0p-1,
    0x1.0p0,
    -0x1.fffffffffffffp-1,
    -0.0
  },
  { // Entry 455
    -0x1.ffffffffffffefffffffffffffffffffp-1,
    0x1.0p0,
    -0x1.fffffffffffffp-1,
    0x1.0p-1074
  },
  { // Entry 456
    -0x1.00000000000020000000000001p0,
    0x1.0000000000001p0,
    -0x1.0000000000001p0,
    -0x1.0p-1074
  },
  { // Entry 457
    -0x1.00000000000020000000000001p0,
    0x1.0000000000001p0,
    -0x1.0000000000001p0,
    -0.0
  },
  { // Entry 458
    -0x1.00000000000020000000000000ffffffp0,
    0x1.0000000000001p0,
    -0x1.0000000000001p0,
    0x1.0p-1074
  },
  { // Entry 459
    -0x1.00000000000010p0,
    0x1.0000000000001p0,
    -0x1.0p0,
    -0x1.0p-1074
  },
  { // Entry 460
    -0x1.00000000000010p0,
    0x1.0000000000001p0,
    -0x1.0p0,
    -0.0
  },
  { // Entry 461
    -0x1.0000000000000fffffffffffffffffffp0,
    0x1.0000000000001p0,
    -0x1.0p0,
    0x1.0p-1074
  },
  { // Entry 462
    -0x1.00000000000007ffffffffffff80p0,
    0x1.0000000000001p0,
    -0x1.fffffffffffffp-1,
    -0x1.0p-1074
  },
  { // Entry 463
    -0x1.00000000000007ffffffffffff80p0,
    0x1.0000000000001p0,
    -0x1.fffffffffffffp-1,
    -0.0
  },
  { // Entry 464
    -0x1.00000000000007ffffffffffff7fffffp0,
    0x1.0000000000001p0,
    -0x1.fffffffffffffp-1,
    0x1.0p-1074
  },
  { // Entry 465
    0x1.0000003fffffeffffffe00000040p0,
    0x1.fffffffffffffp-1,
    0x1.fffffffffffffp-1,
    0x1.fffffffffffffp-27
  },
  { // Entry 466
    0x1.0000003ffffff000000000000040p0,
    0x1.fffffffffffffp-1,
    0x1.fffffffffffffp-1,
    0x1.0p-26
  },
  { // Entry 467
    0x1.0000003ffffff000000400000040p0,
    0x1.fffffffffffffp-1,
    0x1.fffffffffffffp-1,
    0x1.0000000000001p-26
  },
  { // Entry 468
    0x1.0000003ffffff7fffffep0,
    0x1.fffffffffffffp-1,
    0x1.0p0,
    0x1.fffffffffffffp-27
  },
  { // Entry 469
    0x1.0000003ffffff8p0,
    0x1.fffffffffffffp-1,
    0x1.0p0,
    0x1.0p-26
  },
  { // Entry 470
    0x1.0000003ffffff8000004p0,
    0x1.fffffffffffffp-1,
    0x1.0p0,
    0x1.0000000000001p-26
  },
  { // Entry 471
    0x1.00000040000007fffffdffffff80p0,
    0x1.fffffffffffffp-1,
    0x1.0000000000001p0,
    0x1.fffffffffffffp-27
  },
  { // Entry 472
    0x1.00000040000007ffffffffffff80p0,
    0x1.fffffffffffffp-1,
    0x1.0000000000001p0,
    0x1.0p-26
  },
  { // Entry 473
    0x1.00000040000008000003ffffff80p0,
    0x1.fffffffffffffp-1,
    0x1.0000000000001p0,
    0x1.0000000000001p-26
  },
  { // Entry 474
    0x1.0000003ffffff7fffffep0,
    0x1.0p0,
    0x1.fffffffffffffp-1,
    0x1.fffffffffffffp-27
  },
  { // Entry 475
    0x1.0000003ffffff8p0,
    0x1.0p0,
    0x1.fffffffffffffp-1,
    0x1.0p-26
  },
  { // Entry 476
    0x1.0000003ffffff8000004p0,
    0x1.0p0,
    0x1.fffffffffffffp-1,
    0x1.0000000000001p-26
  },
  { // Entry 477
    0x1.0000003ffffffffffffep0,
    0x1.0p0,
    0x1.0p0,
    0x1.fffffffffffffp-27
  },
  { // Entry 478
    0x1.00000040p0,
    0x1.0p0,
    0x1.0p0,
    0x1.0p-26
  },
  { // Entry 479
    0x1.00000040000000000004p0,
    0x1.0p0,
    0x1.0p0,
    0x1.0000000000001p-26
  },
  { // Entry 480
    0x1.0000004000000ffffffep0,
    0x1.0p0,
    0x1.0000000000001p0,
    0x1.fffffffffffffp-27
  },
  { // Entry 481
    0x1.00000040000010p0,
    0x1.0p0,
    0x1.0000000000001p0,
    0x1.0p-26
  },
  { // Entry 482
    0x1.00000040000010000004p0,
    0x1.0p0,
    0x1.0000000000001p0,
    0x1.0000000000001p-26
  },
  { // Entry 483
    0x1.00000040000007fffffdffffff80p0,
    0x1.0000000000001p0,
    0x1.fffffffffffffp-1,
    0x1.fffffffffffffp-27
  },
  { // Entry 484
    0x1.00000040000007ffffffffffff80p0,
    0x1.0000000000001p0,
    0x1.fffffffffffffp-1,
    0x1.0p-26
  },
  { // Entry 485
    0x1.00000040000008000003ffffff80p0,
    0x1.0000000000001p0,
    0x1.fffffffffffffp-1,
    0x1.0000000000001p-26
  },
  { // Entry 486
    0x1.0000004000000ffffffep0,
    0x1.0000000000001p0,
    0x1.0p0,
    0x1.fffffffffffffp-27
  },
  { // Entry 487
    0x1.00000040000010p0,
    0x1.0000000000001p0,
    0x1.0p0,
    0x1.0p-26
  },
  { // Entry 488
    0x1.00000040000010000004p0,
    0x1.0000000000001p0,
    0x1.0p0,
    0x1.0000000000001p-26
  },
  { // Entry 489
    0x1.0000004000001ffffffe000001p0,
    0x1.0000000000001p0,
    0x1.0000000000001p0,
    0x1.fffffffffffffp-27
  },
  { // Entry 490
    0x1.00000040000020000000000001p0,
    0x1.0000000000001p0,
    0x1.0000000000001p0,
    0x1.0p-26
  },
  { // Entry 491
    0x1.00000040000020000004000001p0,
    0x1.0000000000001p0,
    0x1.0000000000001p0,
    0x1.0000000000001p-26
  },
  { // Entry 492
    -0x1.ffffff80000010000003ffffffp-1,
    0x1.fffffffffffffp-1,
    -0x1.0000000000001p0,
    0x1.fffffffffffffp-27
  },
  { // Entry 493
    -0x1.ffffff8000000fffffffffffffp-1,
    0x1.fffffffffffffp-1,
    -0x1.0000000000001p0,
    0x1.0p-26
  },
  { // Entry 494
    -0x1.ffffff8000000ffffff7ffffffp-1,
    0x1.fffffffffffffp-1,
    -0x1.0000000000001p0,
    0x1.0000000000001p-26
  },
  { // Entry 495
    -0x1.ffffff7ffffff0000004p-1,
    0x1.fffffffffffffp-1,
    -0x1.0p0,
    0x1.fffffffffffffp-27
  },
  { // Entry 496
    -0x1.ffffff7ffffff0p-1,
    0x1.fffffffffffffp-1,
    -0x1.0p0,
    0x1.0p-26
  },
  { // Entry 497
    -0x1.ffffff7fffffeffffff8p-1,
    0x1.fffffffffffffp-1,
    -0x1.0p0,
    0x1.0000000000001p-26
  },
  { // Entry 498
    -0x1.ffffff7fffffe000000400000080p-1,
    0x1.fffffffffffffp-1,
    -0x1.fffffffffffffp-1,
    0x1.fffffffffffffp-27
  },
  { // Entry 499
    -0x1.ffffff7fffffe000000000000080p-1,
    0x1.fffffffffffffp-1,
    -0x1.fffffffffffffp-1,
    0x1.0p-26
  },
  { // Entry 500
    -0x1.ffffff7fffffdffffff800000080p-1,
    0x1.fffffffffffffp-1,
    -0x1.fffffffffffffp-1,
    0x1.0000000000001p-26
  },
  { // Entry 501
    -0x1.ffffff80000020000004p-1,
    0x1.0p0,
    -0x1.0000000000001p0,
    0x1.fffffffffffffp-27
  },
  { // Entry 502
    -0x1.ffffff80000020p-1,
    0x1.0p0,
    -0x1.0000000000001p0,
    0x1.0p-26
  },
  { // Entry 503
    -0x1.ffffff8000001ffffff8p-1,
    0x1.0p0,
    -0x1.0000000000001p0,
    0x1.0000000000001p-26
  },
  { // Entry 504
    -0x1.ffffff80000000000004p-1,
    0x1.0p0,
    -0x1.0p0,
    0x1.fffffffffffffp-27
  },
  { // Entry 505
    -0x1.ffffff80p-1,
    0x1.0p0,
    -0x1.0p0,
    0x1.0p-26
  },
  { // Entry 506
    -0x1.ffffff7ffffffffffff8p-1,
    0x1.0p0,
    -0x1.0p0,
    0x1.0000000000001p-26
  },
  { // Entry 507
    -0x1.ffffff7ffffff0000004p-1,
    0x1.0p0,
    -0x1.fffffffffffffp-1,
    0x1.fffffffffffffp-27
  },
  { // Entry 508
    -0x1.ffffff7ffffff0p-1,
    0x1.0p0,
    -0x1.fffffffffffffp-1,
    0x1.0p-26
  },
  { // Entry 509
    -0x1.ffffff7fffffeffffff8p-1,
    0x1.0p0,
    -0x1.fffffffffffffp-1,
    0x1.0000000000001p-26
  },
  { // Entry 510
    -0x1.ffffff80000040000004000002p-1,
    0x1.0000000000001p0,
    -0x1.0000000000001p0,
    0x1.fffffffffffffp-27
  },
  { // Entry 511
    -0x1.ffffff80000040000000000002p-1,
    0x1.0000000000001p0,
    -0x1.0000000000001p0,
    0x1.0p-26
  },
  { // Entry 512
    -0x1.ffffff8000003ffffff8000002p-1,
    0x1.0000000000001p0,
    -0x1.0000000000001p0,
    0x1.0000000000001p-26
  },
  { // Entry 513
    -0x1.ffffff80000020000004p-1,
    0x1.0000000000001p0,
    -0x1.0p0,
    0x1.fffffffffffffp-27
  },
  { // Entry 514
    -0x1.ffffff80000020p-1,
    0x1.0000000000001p0,
    -0x1.0p0,
    0x1.0p-26
  },
  { // Entry 515
    -0x1.ffffff8000001ffffff8p-1,
    0x1.0000000000001p0,
    -0x1.0p0,
    0x1.0000000000001p-26
  },
  { // Entry 516
    -0x1.ffffff80000010000003ffffffp-1,
    0x1.0000000000001p0,
    -0x1.fffffffffffffp-1,
    0x1.fffffffffffffp-27
  },
  { // Entry 517
    -0x1.ffffff8000000fffffffffffffp-1,
    0x1.0000000000001p0,
    -0x1.fffffffffffffp-1,
    0x1.0p-26
  },
  { // Entry 518
    -0x1.ffffff8000000ffffff7ffffffp-1,
    0x1.0000000000001p0,
    -0x1.fffffffffffffp-1,
    0x1.0000000000001p-26
  },
  { // Entry 519
    -0x1.fffffffffffffcp0,
    0x1.fffffffffffffp51,
    0x1.fffffffffffffp-1,
    -0x1.0000000000001p52
  },
  { // Entry 520
    -0x1.fffffffffffff8p-1,
    0x1.fffffffffffffp51,
    0x1.fffffffffffffp-1,
    -0x1.0p52
  },
  { // Entry 521
    -0x1.fffffffffffff0p-2,
    0x1.fffffffffffffp51,
    0x1.fffffffffffffp-1,
    -0x1.fffffffffffffp51
  },
  { // Entry 522
    -0x1.80p0,
    0x1.fffffffffffffp51,
    0x1.0p0,
    -0x1.0000000000001p52
  },
  { // Entry 523
    -0x1.p-1,
    0x1.fffffffffffffp51,
    0x1.0p0,
    -0x1.0p52
  },
  { // Entry 524
    0.0,
    0x1.fffffffffffffp51,
    0x1.0p0,
    -0x1.fffffffffffffp51
  },
  { // Entry 525
    -0x1.00000000000010p-1,
    0x1.fffffffffffffp51,
    0x1.0000000000001p0,
    -0x1.0000000000001p52
  },
  { // Entry 526
    0x1.ffffffffffffe0p-2,
    0x1.fffffffffffffp51,
    0x1.0000000000001p0,
    -0x1.0p52
  },
  { // Entry 527
    0x1.fffffffffffff0p-1,
    0x1.fffffffffffffp51,
    0x1.0000000000001p0,
    -0x1.fffffffffffffp51
  },
  { // Entry 528
    -0x1.80p0,
    0x1.0p52,
    0x1.fffffffffffffp-1,
    -0x1.0000000000001p52
  },
  { // Entry 529
    -0x1.p-1,
    0x1.0p52,
    0x1.fffffffffffffp-1,
    -0x1.0p52
  },
  { // Entry 530
    0.0,
    0x1.0p52,
    0x1.fffffffffffffp-1,
    -0x1.fffffffffffffp51
  },
  { // Entry 531
    -0x1.p0,
    0x1.0p52,
    0x1.0p0,
    -0x1.0000000000001p52
  },
  { // Entry 532
    0.0,
    0x1.0p52,
    0x1.0p0,
    -0x1.0p52
  },
  { // Entry 533
    0x1.p-1,
    0x1.0p52,
    0x1.0p0,
    -0x1.fffffffffffffp51
  },
  { // Entry 534
    0.0,
    0x1.0p52,
    0x1.0000000000001p0,
    -0x1.0000000000001p52
  },
  { // Entry 535
    0x1.p0,
    0x1.0p52,
    0x1.0000000000001p0,
    -0x1.0p52
  },
  { // Entry 536
    0x1.80p0,
    0x1.0p52,
    0x1.0000000000001p0,
    -0x1.fffffffffffffp51
  },
  { // Entry 537
    -0x1.00000000000010p-1,
    0x1.0000000000001p52,
    0x1.fffffffffffffp-1,
    -0x1.0000000000001p52
  },
  { // Entry 538
    0x1.ffffffffffffe0p-2,
    0x1.0000000000001p52,
    0x1.fffffffffffffp-1,
    -0x1.0p52
  },
  { // Entry 539
    0x1.fffffffffffff0p-1,
    0x1.0000000000001p52,
    0x1.fffffffffffffp-1,
    -0x1.fffffffffffffp51
  },
  { // Entry 540
    0.0,
    0x1.0000000000001p52,
    0x1.0p0,
    -0x1.0000000000001p52
  },
  { // Entry 541
    0x1.p0,
    0x1.0000000000001p52,
    0x1.0p0,
    -0x1.0p52
  },
  { // Entry 542
    0x1.80p0,
    0x1.0000000000001p52,
    0x1.0p0,
    -0x1.fffffffffffffp51
  },
  { // Entry 543
    0x1.00000000000010p0,
    0x1.0000000000001p52,
    0x1.0000000000001p0,
    -0x1.0000000000001p52
  },
  { // Entry 544
    0x1.00000000000008p1,
    0x1.0000000000001p52,
    0x1.0000000000001p0,
    -0x1.0p52
  },
  { // Entry 545
    0x1.40000000000008p1,
    0x1.0000000000001p52,
    0x1.0000000000001p0,
    -0x1.fffffffffffffp51
  },
  { // Entry 546
    0x1.08p-5,
    0x1.0p-5,
    0x1.0p-5,
    0x1.0p-5
  },
  { // Entry 547
    0x1.0040p0,
    0x1.0p-5,
    0x1.0p-5,
    0x1.0p0
  },
  { // Entry 548
    0x1.p-4,
    0x1.0p-5,
    0x1.0p0,
    0x1.0p-5
  },
  { // Entry 549
    0x1.08p0,
    0x1.0p-5,
    0x1.0p0,
    0x1.0p0
  },
  { // Entry 550
    0x1.p-4,
    0x1.0p0,
    0x1.0p-5,
    0x1.0p-5
  },
  { // Entry 551
    0x1.08p0,
    0x1.0p0,
    0x1.0p-5,
    0x1.0p0
  },
  { // Entry 552
    0x1.08p0,
    0x1.0p0,
    0x1.0p0,
    0x1.0p-5
  },
  { // Entry 553
    0x1.p1,
    0x1.0p0,
    0x1.0p0,
    0x1.0p0
  },
  { // Entry 554
    0x1.20p-2,
    0x1.0p-5,
    0x1.0p3,
    0x1.0p-5
  },
  { // Entry 555
    0x1.40p0,
    0x1.0p-5,
    0x1.0p3,
    0x1.0p0
  },
  { // Entry 556
    0x1.08p0,
    0x1.0p-5,
    0x1.0p5,
    0x1.0p-5
  },
  { // Entry 557
    0x1.p1,
    0x1.0p-5,
    0x1.0p5,
    0x1.0p0
  },
  { // Entry 558
    0x1.01p3,
    0x1.0p0,
    0x1.0p3,
    0x1.0p-5
  },
  { // Entry 559
    0x1.20p3,
    0x1.0p0,
    0x1.0p3,
    0x1.0p0
  },
  { // Entry 560
    0x1.0040p5,
    0x1.0p0,
    0x1.0p5,
    0x1.0p-5
  },
  { // Entry 561
    0x1.08p5,
    0x1.0p0,
    0x1.0p5,
    0x1.0p0
  },
  { // Entry 562
    0x1.0040p5,
    0x1.0p-5,
    0x1.0p10,
    0x1.0p-5
  },
  { // Entry 563
    0x1.08p5,
    0x1.0p-5,
    0x1.0p10,
    0x1.0p0
  },
  { // Entry 564
    0x1.0010p7,
    0x1.0p-5,
    0x1.0p12,
    0x1.0p-5
  },
  { // Entry 565
    0x1.02p7,
    0x1.0p-5,
    0x1.0p12,
    0x1.0p0
  },
  { // Entry 566
    0x1.0002p10,
    0x1.0p0,
    0x1.0p10,
    0x1.0p-5
  },
  { // Entry 567
    0x1.0040p10,
    0x1.0p0,
    0x1.0p10,
    0x1.0p0
  },
  { // Entry 568
    0x1.000080p12,
    0x1.0p0,
    0x1.0p12,
    0x1.0p-5
  },
  { // Entry 569
    0x1.0010p12,
    0x1.0p0,
    0x1.0p12,
    0x1.0p0
  },
  { // Entry 570
    0x1.0008p3,
    0x1.0p-5,
    0x1.0p-5,
    0x1.0p3
  },
  { // Entry 571
    0x1.0002p5,
    0x1.0p-5,
    0x1.0p-5,
    0x1.0p5
  },
  { // Entry 572
    0x1.01p3,
    0x1.0p-5,
    0x1.0p0,
    0x1.0p3
  },
  { // Entry 573
    0x1.0040p5,
    0x1.0p-5,
    0x1.0p0,
    0x1.0p5
  },
  { // Entry 574
    0x1.01p3,
    0x1.0p0,
    0x1.0p-5,
    0x1.0p3
  },
  { // Entry 575
    0x1.0040p5,
    0x1.0p0,
    0x1.0p-5,
    0x1.0p5
  },
  { // Entry 576
    0x1.20p3,
    0x1.0p0,
    0x1.0p0,
    0x1.0p3
  },
  { // Entry 577
    0x1.08p5,
    0x1.0p0,
    0x1.0p0,
    0x1.0p5
  },
  { // Entry 578
    0x1.08p3,
    0x1.0p-5,
    0x1.0p3,
    0x1.0p3
  },
  { // Entry 579
    0x1.02p5,
    0x1.0p-5,
    0x1.0p3,
    0x1.0p5
  },
  { // Entry 580
    0x1.20p3,
    0x1.0p-5,
    0x1.0p5,
    0x1.0p3
  },
  { // Entry 581
    0x1.08p5,
    0x1.0p-5,
    0x1.0p5,
    0x1.0p5
  },
  { // Entry 582
    0x1.p4,
    0x1.0p0,
    0x1.0p3,
    0x1.0p3
  },
  { // Entry 583
    0x1.40p5,
    0x1.0p0,
    0x1.0p3,
    0x1.0p5
  },
  { // Entry 584
    0x1.40p5,
    0x1.0p0,
    0x1.0p5,
    0x1.0p3
  },
  { // Entry 585
    0x1.p6,
    0x1.0p0,
    0x1.0p5,
    0x1.0p5
  },
  { // Entry 586
    0x1.40p5,
    0x1.0p-5,
    0x1.0p10,
    0x1.0p3
  },
  { // Entry 587
    0x1.p6,
    0x1.0p-5,
    0x1.0p10,
    0x1.0p5
  },
  { // Entry 588
    0x1.10p7,
    0x1.0p-5,
    0x1.0p12,
    0x1.0p3
  },
  { // Entry 589
    0x1.40p7,
    0x1.0p-5,
    0x1.0p12,
    0x1.0p5
  },
  { // Entry 590
    0x1.02p10,
    0x1.0p0,
    0x1.0p10,
    0x1.0p3
  },
  { // Entry 591
    0x1.08p10,
    0x1.0p0,
    0x1.0p10,
    0x1.0p5
  },
  { // Entry 592
    0x1.0080p12,
    0x1.0p0,
    0x1.0p12,
    0x1.0p3
  },
  { // Entry 593
    0x1.02p12,
    0x1.0p0,
    0x1.0p12,
    0x1.0p5
  },
  { // Entry 594
    0x1.000010p10,
    0x1.0p-5,
    0x1.0p-5,
    0x1.0p10
  },
  { // Entry 595
    0x1.000004p12,
    0x1.0p-5,
    0x1.0p-5,
    0x1.0p12
  },
  { // Entry 596
    0x1.0002p10,
    0x1.0p-5,
    0x1.0p0,
    0x1.0p10
  },
  { // Entry 597
    0x1.000080p12,
    0x1.0p-5,
    0x1.0p0,
    0x1.0p12
  },
  { // Entry 598
    0x1.0002p10,
    0x1.0p0,
    0x1.0p-5,
    0x1.0p10
  },
  { // Entry 599
    0x1.000080p12,
    0x1.0p0,
    0x1.0p-5,
    0x1.0p12
  },
  { // Entry 600
    0x1.0040p10,
    0x1.0p0,
    0x1.0p0,
    0x1.0p10
  },
  { // Entry 601
    0x1.0010p12,
    0x1.0p0,
    0x1.0p0,
    0x1.0p12
  },
  { // Entry 602
    0x1.0010p10,
    0x1.0p-5,
    0x1.0p3,
    0x1.0p10
  },
  { // Entry 603
    0x1.0004p12,
    0x1.0p-5,
    0x1.0p3,
    0x1.0p12
  },
  { // Entry 604
    0x1.0040p10,
    0x1.0p-5,
    0x1.0p5,
    0x1.0p10
  },
  { // Entry 605
    0x1.0010p12,
    0x1.0p-5,
    0x1.0p5,
    0x1.0p12
  },
  { // Entry 606
    0x1.02p10,
    0x1.0p0,
    0x1.0p3,
    0x1.0p10
  },
  { // Entry 607
    0x1.0080p12,
    0x1.0p0,
    0x1.0p3,
    0x1.0p12
  },
  { // Entry 608
    0x1.08p10,
    0x1.0p0,
    0x1.0p5,
    0x1.0p10
  },
  { // Entry 609
    0x1.02p12,
    0x1.0p0,
    0x1.0p5,
    0x1.0p12
  },
  { // Entry 610
    0x1.08p10,
    0x1.0p-5,
    0x1.0p10,
    0x1.0p10
  },
  { // Entry 611
    0x1.02p12,
    0x1.0p-5,
    0x1.0p10,
    0x1.0p12
  },
  { // Entry 612
    0x1.20p10,
    0x1.0p-5,
    0x1.0p12,
    0x1.0p10
  },
  { // Entry 613
    0x1.08p12,
    0x1.0p-5,
    0x1.0p12,
    0x1.0p12
  },
  { // Entry 614
    0x1.p11,
    0x1.0p0,
    0x1.0p10,
    0x1.0p10
  },
  { // Entry 615
    0x1.40p12,
    0x1.0p0,
    0x1.0p10,
    0x1.0p12
  },
  { // Entry 616
    0x1.40p12,
    0x1.0p0,
    0x1.0p12,
    0x1.0p10
  },
  { // Entry 617
    0x1.p13,
    0x1.0p0,
    0x1.0p12,
    0x1.0p12
  },
  { // Entry 618
    0x1.0020p6,
    0x1.0p3,
    0x1.0p3,
    0x1.0p-5
  },
  { // Entry 619
    0x1.04p6,
    0x1.0p3,
    0x1.0p3,
    0x1.0p0
  },
  { // Entry 620
    0x1.0008p8,
    0x1.0p3,
    0x1.0p5,
    0x1.0p-5
  },
  { // Entry 621
    0x1.01p8,
    0x1.0p3,
    0x1.0p5,
    0x1.0p0
  },
  { // Entry 622
    0x1.0008p8,
    0x1.0p5,
    0x1.0p3,
    0x1.0p-5
  },
  { // Entry 623
    0x1.01p8,
    0x1.0p5,
    0x1.0p3,
    0x1.0p0
  },
  { // Entry 624
    0x1.0002p10,
    0x1.0p5,
    0x1.0p5,
    0x1.0p-5
  },
  { // Entry 625
    0x1.0040p10,
    0x1.0p5,
    0x1.0p5,
    0x1.0p0
  },
  { // Entry 626
    0x1.20p6,
    0x1.0p3,
    0x1.0p3,
    0x1.0p3
  },
  { // Entry 627
    0x1.80p6,
    0x1.0p3,
    0x1.0p3,
    0x1.0p5
  },
  { // Entry 628
    0x1.08p8,
    0x1.0p3,
    0x1.0p5,
    0x1.0p3
  },
  { // Entry 629
    0x1.20p8,
    0x1.0p3,
    0x1.0p5,
    0x1.0p5
  },
  { // Entry 630
    0x1.08p8,
    0x1.0p5,
    0x1.0p3,
    0x1.0p3
  },
  { // Entry 631
    0x1.20p8,
    0x1.0p5,
    0x1.0p3,
    0x1.0p5
  },
  { // Entry 632
    0x1.02p10,
    0x1.0p5,
    0x1.0p5,
    0x1.0p3
  },
  { // Entry 633
    0x1.08p10,
    0x1.0p5,
    0x1.0p5,
    0x1.0p5
  },
  { // Entry 634
    0x1.10p10,
    0x1.0p3,
    0x1.0p3,
    0x1.0p10
  },
  { // Entry 635
    0x1.04p12,
    0x1.0p3,
    0x1.0p3,
    0x1.0p12
  },
  { // Entry 636
    0x1.40p10,
    0x1.0p3,
    0x1.0p5,
    0x1.0p10
  },
  { // Entry 637
    0x1.10p12,
    0x1.0p3,
    0x1.0p5,
    0x1.0p12
  },
  { // Entry 638
    0x1.40p10,
    0x1.0p5,
    0x1.0p3,
    0x1.0p10
  },
  { // Entry 639
    0x1.10p12,
    0x1.0p5,
    0x1.0p3,
    0x1.0p12
  },
  { // Entry 640
    0x1.p11,
    0x1.0p5,
    0x1.0p5,
    0x1.0p10
  },
  { // Entry 641
    0x1.40p12,
    0x1.0p5,
    0x1.0p5,
    0x1.0p12
  },
  { // Entry 642
    0x1.000040p13,
    0x1.0p3,
    0x1.0p10,
    0x1.0p-5
  },
  { // Entry 643
    0x1.0008p13,
    0x1.0p3,
    0x1.0p10,
    0x1.0p0
  },
  { // Entry 644
    0x1.000010p15,
    0x1.0p3,
    0x1.0p12,
    0x1.0p-5
  },
  { // Entry 645
    0x1.0002p15,
    0x1.0p3,
    0x1.0p12,
    0x1.0p0
  },
  { // Entry 646
    0x1.000010p15,
    0x1.0p5,
    0x1.0p10,
    0x1.0p-5
  },
  { // Entry 647
    0x1.0002p15,
    0x1.0p5,
    0x1.0p10,
    0x1.0p0
  },
  { // Entry 648
    0x1.000004p17,
    0x1.0p5,
    0x1.0p12,
    0x1.0p-5
  },
  { // Entry 649
    0x1.000080p17,
    0x1.0p5,
    0x1.0p12,
    0x1.0p0
  },
  { // Entry 650
    0x1.0040p13,
    0x1.0p3,
    0x1.0p10,
    0x1.0p3
  },
  { // Entry 651
    0x1.01p13,
    0x1.0p3,
    0x1.0p10,
    0x1.0p5
  },
  { // Entry 652
    0x1.0010p15,
    0x1.0p3,
    0x1.0p12,
    0x1.0p3
  },
  { // Entry 653
    0x1.0040p15,
    0x1.0p3,
    0x1.0p12,
    0x1.0p5
  },
  { // Entry 654
    0x1.0010p15,
    0x1.0p5,
    0x1.0p10,
    0x1.0p3
  },
  { // Entry 655
    0x1.0040p15,
    0x1.0p5,
    0x1.0p10,
    0x1.0p5
  },
  { // Entry 656
    0x1.0004p17,
    0x1.0p5,
    0x1.0p12,
    0x1.0p3
  },
  { // Entry 657
    0x1.0010p17,
    0x1.0p5,
    0x1.0p12,
    0x1.0p5
  },
  { // Entry 658
    0x1.20p13,
    0x1.0p3,
    0x1.0p10,
    0x1.0p10
  },
  { // Entry 659
    0x1.80p13,
    0x1.0p3,
    0x1.0p10,
    0x1.0p12
  },
  { // Entry 660
    0x1.08p15,
    0x1.0p3,
    0x1.0p12,
    0x1.0p10
  },
  { // Entry 661
    0x1.20p15,
    0x1.0p3,
    0x1.0p12,
    0x1.0p12
  },
  { // Entry 662
    0x1.08p15,
    0x1.0p5,
    0x1.0p10,
    0x1.0p10
  },
  { // Entry 663
    0x1.20p15,
    0x1.0p5,
    0x1.0p10,
    0x1.0p12
  },
  { // Entry 664
    0x1.02p17,
    0x1.0p5,
    0x1.0p12,
    0x1.0p10
  },
  { // Entry 665
    0x1.08p17,
    0x1.0p5,
    0x1.0p12,
    0x1.0p12
  },
  { // Entry 666
    -0x1.ffffe0p-21,
    -0x1.0p-20,
    -0x1.0p-20,
    -0x1.0p-20
  },
  { // Entry 667
    0x1.000010p-20,
    -0x1.0p-20,
    -0x1.0p-20,
    0x1.0p-20
  },
  { // Entry 668
    -0x1.000010p-20,
    -0x1.0p-20,
    0x1.0p-20,
    -0x1.0p-20
  },
  { // Entry 669
    0x1.ffffe0p-21,
    -0x1.0p-20,
    0x1.0p-20,
    0x1.0p-20
  },
  { // Entry 670
    -0x1.000010p-20,
    0x1.0p-20,
    -0x1.0p-20,
    -0x1.0p-20
  },
  { // Entry 671
    0x1.ffffe0p-21,
    0x1.0p-20,
    -0x1.0p-20,
    0x1.0p-20
  },
  { // Entry 672
    -0x1.ffffe0p-21,
    0x1.0p-20,
    0x1.0p-20,
    -0x1.0p-20
  },
  { // Entry 673
    0x1.000010p-20,
    0x1.0p-20,
    0x1.0p-20,
    0x1.0p-20
  },
  { // Entry 674
    0x1.fffffffffffffffffffep-21,
    -0x1.0p-10,
    -0x1.0p-10,
    -0x1.0p-100
  },
  { // Entry 675
    0x1.00000000000000000001p-20,
    -0x1.0p-10,
    -0x1.0p-10,
    0x1.0p-100
  },
  { // Entry 676
    -0x1.00000000000000000001p-20,
    -0x1.0p-10,
    0x1.0p-10,
    -0x1.0p-100
  },
  { // Entry 677
    -0x1.fffffffffffffffffffep-21,
    -0x1.0p-10,
    0x1.0p-10,
    0x1.0p-100
  },
  { // Entry 678
    -0x1.00000000000000000001p-20,
    0x1.0p-10,
    -0x1.0p-10,
    -0x1.0p-100
  },
  { // Entry 679
    -0x1.fffffffffffffffffffep-21,
    0x1.0p-10,
    -0x1.0p-10,
    0x1.0p-100
  },
  { // Entry 680
    0x1.fffffffffffffffffffep-21,
    0x1.0p-10,
    0x1.0p-10,
    -0x1.0p-100
  },
  { // Entry 681
    0x1.00000000000000000001p-20,
    0x1.0p-10,
    0x1.0p-10,
    0x1.0p-100
  },
  { // Entry 682
    0x1.f0p-11,
    -0x1.0p-5,
    -0x1.0p-5,
    -0x1.0p-15
  },
  { // Entry 683
    0x1.08p-10,
    -0x1.0p-5,
    -0x1.0p-5,
    0x1.0p-15
  },
  { // Entry 684
    -0x1.08p-10,
    -0x1.0p-5,
    0x1.0p-5,
    -0x1.0p-15
  },
  { // Entry 685
    -0x1.f0p-11,
    -0x1.0p-5,
    0x1.0p-5,
    0x1.0p-15
  },
  { // Entry 686
    -0x1.08p-10,
    0x1.0p-5,
    -0x1.0p-5,
    -0x1.0p-15
  },
  { // Entry 687
    -0x1.f0p-11,
    0x1.0p-5,
    -0x1.0p-5,
    0x1.0p-15
  },
  { // Entry 688
    0x1.f0p-11,
    0x1.0p-5,
    0x1.0p-5,
    -0x1.0p-15
  },
  { // Entry 689
    0x1.08p-10,
    0x1.0p-5,
    0x1.0p-5,
    0x1.0p-15
  },
  { // Entry 690
    0x1.68p6,
    -0x1.4p3,
    -0x1.4p3,
    -0x1.4p3
  },
  { // Entry 691
    0x1.b8p6,
    -0x1.4p3,
    -0x1.4p3,
    0x1.4p3
  },
  { // Entry 692
    -0x1.b8p6,
    -0x1.4p3,
    0x1.4p3,
    -0x1.4p3
  },
  { // Entry 693
    -0x1.68p6,
    -0x1.4p3,
    0x1.4p3,
    0x1.4p3
  },
  { // Entry 694
    -0x1.b8p6,
    0x1.4p3,
    -0x1.4p3,
    -0x1.4p3
  },
  { // Entry 695
    -0x1.68p6,
    0x1.4p3,
    -0x1.4p3,
    0x1.4p3
  },
  { // Entry 696
    0x1.68p6,
    0x1.4p3,
    0x1.4p3,
    -0x1.4p3
  },
  { // Entry 697
    0x1.b8p6,
    0x1.4p3,
    0x1.4p3,
    0x1.4p3
  },
  { // Entry 698
    0.0,
    -0x1.0p0,
    -0x1.0p0,
    -0x1.0p0
  },
  { // Entry 699
    0x1.p1,
    -0x1.0p0,
    -0x1.0p0,
    0x1.0p0
  },
  { // Entry 700
    -0x1.p1,
    -0x1.0p0,
    0x1.0p0,
    -0x1.0p0
  },
  { // Entry 701
    0.0,
    -0x1.0p0,
    0x1.0p0,
    0x1.0p0
  },
  { // Entry 702
    -0x1.p1,
    0x1.0p0,
    -0x1.0p0,
    -0x1.0p0
  },
  { // Entry 703
    0.0,
    0x1.0p0,
    -0x1.0p0,
    0x1.0p0
  },
  { // Entry 704
    0.0,
    0x1.0p0,
    0x1.0p0,
    -0x1.0p0
  },
  { // Entry 705
    0x1.p1,
    0x1.0p0,
    0x1.0p0,
    0x1.0p0
  },
  { // Entry 706
    HUGE_VAL,
    0x1.fffffffffffffp1023,
    HUGE_VAL,
    HUGE_VAL
  },
  { // Entry 707
    HUGE_VAL,
    0x1.fffffffffffffp1023,
    HUGE_VAL,
    0x1.fffffffffffffp1023
  },
  { // Entry 708
    HUGE_VAL,
    0x1.fffffffffffffp1023,
    HUGE_VAL,
    -0x1.fffffffffffffp1023
  },
  { // Entry 709
    HUGE_VAL,
    0x1.fffffffffffffp1023,
    HUGE_VAL,
    0x1.0p-1022
  },
  { // Entry 710
    HUGE_VAL,
    0x1.fffffffffffffp1023,
    HUGE_VAL,
    -0x1.0p-1022
  },
  { // Entry 711
    HUGE_VAL,
    0x1.fffffffffffffp1023,
    HUGE_VAL,
    0x1.ffffffffffffep-1023
  },
  { // Entry 712
    HUGE_VAL,
    0x1.fffffffffffffp1023,
    HUGE_VAL,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 713
    HUGE_VAL,
    0x1.fffffffffffffp1023,
    HUGE_VAL,
    0x1.0p-1074
  },
  { // Entry 714
    HUGE_VAL,
    0x1.fffffffffffffp1023,
    HUGE_VAL,
    -0x1.0p-1074
  },
  { // Entry 715
    HUGE_VAL,
    0x1.fffffffffffffp1023,
    HUGE_VAL,
    0.0
  },
  { // Entry 716
    HUGE_VAL,
    0x1.fffffffffffffp1023,
    HUGE_VAL,
    -0.0
  },
  { // Entry 717
    -HUGE_VAL,
    0x1.fffffffffffffp1023,
    -HUGE_VAL,
    -HUGE_VAL
  },
  { // Entry 718
    -HUGE_VAL,
    0x1.fffffffffffffp1023,
    -HUGE_VAL,
    0x1.fffffffffffffp1023
  },
  { // Entry 719
    -HUGE_VAL,
    0x1.fffffffffffffp1023,
    -HUGE_VAL,
    -0x1.fffffffffffffp1023
  },
  { // Entry 720
    -HUGE_VAL,
    0x1.fffffffffffffp1023,
    -HUGE_VAL,
    0x1.0p-1022
  },
  { // Entry 721
    -HUGE_VAL,
    0x1.fffffffffffffp1023,
    -HUGE_VAL,
    -0x1.0p-1022
  },
  { // Entry 722
    -HUGE_VAL,
    0x1.fffffffffffffp1023,
    -HUGE_VAL,
    0x1.ffffffffffffep-1023
  },
  { // Entry 723
    -HUGE_VAL,
    0x1.fffffffffffffp1023,
    -HUGE_VAL,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 724
    -HUGE_VAL,
    0x1.fffffffffffffp1023,
    -HUGE_VAL,
    0x1.0p-1074
  },
  { // Entry 725
    -HUGE_VAL,
    0x1.fffffffffffffp1023,
    -HUGE_VAL,
    -0x1.0p-1074
  },
  { // Entry 726
    -HUGE_VAL,
    0x1.fffffffffffffp1023,
    -HUGE_VAL,
    0.0
  },
  { // Entry 727
    -HUGE_VAL,
    0x1.fffffffffffffp1023,
    -HUGE_VAL,
    -0.0
  },
  { // Entry 728
    HUGE_VAL,
    0x1.fffffffffffffp1023,
    0x1.fffffffffffffp1023,
    HUGE_VAL
  },
  { // Entry 729
    -HUGE_VAL,
    0x1.fffffffffffffp1023,
    0x1.fffffffffffffp1023,
    -HUGE_VAL
  },
  { // Entry 730
    HUGE_VAL,
    0x1.fffffffffffffp1023,
    0x1.fffffffffffffp1023,
    0x1.fffffffffffffp1023
  },
  { // Entry 731
    HUGE_VAL,
    0x1.fffffffffffffp1023,
    0x1.fffffffffffffp1023,
    -0x1.fffffffffffffp1023
  },
  { // Entry 732
    HUGE_VAL,
    0x1.fffffffffffffp1023,
    0x1.fffffffffffffp1023,
    0x1.0p-1022
  },
  { // Entry 733
    HUGE_VAL,
    0x1.fffffffffffffp1023,
    0x1.fffffffffffffp1023,
    -0x1.0p-1022
  },
  { // Entry 734
    HUGE_VAL,
    0x1.fffffffffffffp1023,
    0x1.fffffffffffffp1023,
    0x1.ffffffffffffep-1023
  },
  { // Entry 735
    HUGE_VAL,
    0x1.fffffffffffffp1023,
    0x1.fffffffffffffp1023,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 736
    HUGE_VAL,
    0x1.fffffffffffffp1023,
    0x1.fffffffffffffp1023,
    0x1.0p-1074
  },
  { // Entry 737
    HUGE_VAL,
    0x1.fffffffffffffp1023,
    0x1.fffffffffffffp1023,
    -0x1.0p-1074
  },
  { // Entry 738
    HUGE_VAL,
    0x1.fffffffffffffp1023,
    0x1.fffffffffffffp1023,
    0.0
  },
  { // Entry 739
    HUGE_VAL,
    0x1.fffffffffffffp1023,
    0x1.fffffffffffffp1023,
    -0.0
  },
  { // Entry 740
    HUGE_VAL,
    0x1.fffffffffffffp1023,
    -0x1.fffffffffffffp1023,
    HUGE_VAL
  },
  { // Entry 741
    -HUGE_VAL,
    0x1.fffffffffffffp1023,
    -0x1.fffffffffffffp1023,
    -HUGE_VAL
  },
  { // Entry 742
    -HUGE_VAL,
    0x1.fffffffffffffp1023,
    -0x1.fffffffffffffp1023,
    0x1.fffffffffffffp1023
  },
  { // Entry 743
    -HUGE_VAL,
    0x1.fffffffffffffp1023,
    -0x1.fffffffffffffp1023,
    -0x1.fffffffffffffp1023
  },
  { // Entry 744
    -HUGE_VAL,
    0x1.fffffffffffffp1023,
    -0x1.fffffffffffffp1023,
    0x1.0p-1022
  },
  { // Entry 745
    -HUGE_VAL,
    0x1.fffffffffffffp1023,
    -0x1.fffffffffffffp1023,
    -0x1.0p-1022
  },
  { // Entry 746
    -HUGE_VAL,
    0x1.fffffffffffffp1023,
    -0x1.fffffffffffffp1023,
    0x1.ffffffffffffep-1023
  },
  { // Entry 747
    -HUGE_VAL,
    0x1.fffffffffffffp1023,
    -0x1.fffffffffffffp1023,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 748
    -HUGE_VAL,
    0x1.fffffffffffffp1023,
    -0x1.fffffffffffffp1023,
    0x1.0p-1074
  },
  { // Entry 749
    -HUGE_VAL,
    0x1.fffffffffffffp1023,
    -0x1.fffffffffffffp1023,
    -0x1.0p-1074
  },
  { // Entry 750
    -HUGE_VAL,
    0x1.fffffffffffffp1023,
    -0x1.fffffffffffffp1023,
    0.0
  },
  { // Entry 751
    -HUGE_VAL,
    0x1.fffffffffffffp1023,
    -0x1.fffffffffffffp1023,
    -0.0
  },
  { // Entry 752
    HUGE_VAL,
    0x1.fffffffffffffp1023,
    0x1.0p-1022,
    HUGE_VAL
  },
  { // Entry 753
    -HUGE_VAL,
    0x1.fffffffffffffp1023,
    0x1.0p-1022,
    -HUGE_VAL
  },
  { // Entry 754
    0x1.fffffffffffff0p1023,
    0x1.fffffffffffffp1023,
    0x1.0p-1022,
    0x1.fffffffffffffp1023
  },
  { // Entry 755
    -0x1.ffffffffffffefffffffffffffffffffp1023,
    0x1.fffffffffffffp1023,
    0x1.0p-1022,
    -0x1.fffffffffffffp1023
  },
  { // Entry 756
    0x1.fffffffffffff0p1,
    0x1.fffffffffffffp1023,
    0x1.0p-1022,
    0x1.0p-1022
  },
  { // Entry 757
    0x1.ffffffffffffefffffffffffffffffffp1,
    0x1.fffffffffffffp1023,
    0x1.0p-1022,
    -0x1.0p-1022
  },
  { // Entry 758
    0x1.fffffffffffff0p1,
    0x1.fffffffffffffp1023,
    0x1.0p-1022,
    0x1.ffffffffffffep-1023
  },
  { // Entry 759
    0x1.ffffffffffffefffffffffffffffffffp1,
    0x1.fffffffffffffp1023,
    0x1.0p-1022,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 760
    0x1.fffffffffffff0p1,
    0x1.fffffffffffffp1023,
    0x1.0p-1022,
    0x1.0p-1074
  },
  { // Entry 761
    0x1.ffffffffffffefffffffffffffffffffp1,
    0x1.fffffffffffffp1023,
    0x1.0p-1022,
    -0x1.0p-1074
  },
  { // Entry 762
    0x1.fffffffffffff0p1,
    0x1.fffffffffffffp1023,
    0x1.0p-1022,
    0.0
  },
  { // Entry 763
    0x1.fffffffffffff0p1,
    0x1.fffffffffffffp1023,
    0x1.0p-1022,
    -0.0
  },
  { // Entry 764
    HUGE_VAL,
    0x1.fffffffffffffp1023,
    -0x1.0p-1022,
    HUGE_VAL
  },
  { // Entry 765
    -HUGE_VAL,
    0x1.fffffffffffffp1023,
    -0x1.0p-1022,
    -HUGE_VAL
  },
  { // Entry 766
    0x1.ffffffffffffefffffffffffffffffffp1023,
    0x1.fffffffffffffp1023,
    -0x1.0p-1022,
    0x1.fffffffffffffp1023
  },
  { // Entry 767
    -0x1.fffffffffffff0p1023,
    0x1.fffffffffffffp1023,
    -0x1.0p-1022,
    -0x1.fffffffffffffp1023
  },
  { // Entry 768
    -0x1.ffffffffffffefffffffffffffffffffp1,
    0x1.fffffffffffffp1023,
    -0x1.0p-1022,
    0x1.0p-1022
  },
  { // Entry 769
    -0x1.fffffffffffff0p1,
    0x1.fffffffffffffp1023,
    -0x1.0p-1022,
    -0x1.0p-1022
  },
  { // Entry 770
    -0x1.ffffffffffffefffffffffffffffffffp1,
    0x1.fffffffffffffp1023,
    -0x1.0p-1022,
    0x1.ffffffffffffep-1023
  },
  { // Entry 771
    -0x1.fffffffffffff0p1,
    0x1.fffffffffffffp1023,
    -0x1.0p-1022,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 772
    -0x1.ffffffffffffefffffffffffffffffffp1,
    0x1.fffffffffffffp1023,
    -0x1.0p-1022,
    0x1.0p-1074
  },
  { // Entry 773
    -0x1.fffffffffffff0p1,
    0x1.fffffffffffffp1023,
    -0x1.0p-1022,
    -0x1.0p-1074
  },
  { // Entry 774
    -0x1.fffffffffffff0p1,
    0x1.fffffffffffffp1023,
    -0x1.0p-1022,
    0.0
  },
  { // Entry 775
    -0x1.fffffffffffff0p1,
    0x1.fffffffffffffp1023,
    -0x1.0p-1022,
    -0.0
  },
  { // Entry 776
    HUGE_VAL,
    0x1.fffffffffffffp1023,
    0x1.ffffffffffffep-1023,
    HUGE_VAL
  },
  { // Entry 777
    -HUGE_VAL,
    0x1.fffffffffffffp1023,
    0x1.ffffffffffffep-1023,
    -HUGE_VAL
  },
  { // Entry 778
    0x1.fffffffffffff0p1023,
    0x1.fffffffffffffp1023,
    0x1.ffffffffffffep-1023,
    0x1.fffffffffffffp1023
  },
  { // Entry 779
    -0x1.ffffffffffffefffffffffffffffffffp1023,
    0x1.fffffffffffffp1023,
    0x1.ffffffffffffep-1023,
    -0x1.fffffffffffffp1023
  },
  { // Entry 780
    0x1.ffffffffffffd0000000000001p1,
    0x1.fffffffffffffp1023,
    0x1.ffffffffffffep-1023,
    0x1.0p-1022
  },
  { // Entry 781
    0x1.ffffffffffffd0000000000000ffffffp1,
    0x1.fffffffffffffp1023,
    0x1.ffffffffffffep-1023,
    -0x1.0p-1022
  },
  { // Entry 782
    0x1.ffffffffffffd0000000000001p1,
    0x1.fffffffffffffp1023,
    0x1.ffffffffffffep-1023,
    0x1.ffffffffffffep-1023
  },
  { // Entry 783
    0x1.ffffffffffffd0000000000000ffffffp1,
    0x1.fffffffffffffp1023,
    0x1.ffffffffffffep-1023,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 784
    0x1.ffffffffffffd0000000000001p1,
    0x1.fffffffffffffp1023,
    0x1.ffffffffffffep-1023,
    0x1.0p-1074
  },
  { // Entry 785
    0x1.ffffffffffffd0000000000000ffffffp1,
    0x1.fffffffffffffp1023,
    0x1.ffffffffffffep-1023,
    -0x1.0p-1074
  },
  { // Entry 786
    0x1.ffffffffffffd0000000000001p1,
    0x1.fffffffffffffp1023,
    0x1.ffffffffffffep-1023,
    0.0
  },
  { // Entry 787
    0x1.ffffffffffffd0000000000001p1,
    0x1.fffffffffffffp1023,
    0x1.ffffffffffffep-1023,
    -0.0
  },
  { // Entry 788
    HUGE_VAL,
    0x1.fffffffffffffp1023,
    -0x1.ffffffffffffep-1023,
    HUGE_VAL
  },
  { // Entry 789
    -HUGE_VAL,
    0x1.fffffffffffffp1023,
    -0x1.ffffffffffffep-1023,
    -HUGE_VAL
  },
  { // Entry 790
    0x1.ffffffffffffefffffffffffffffffffp1023,
    0x1.fffffffffffffp1023,
    -0x1.ffffffffffffep-1023,
    0x1.fffffffffffffp1023
  },
  { // Entry 791
    -0x1.fffffffffffff0p1023,
    0x1.fffffffffffffp1023,
    -0x1.ffffffffffffep-1023,
    -0x1.fffffffffffffp1023
  },
  { // Entry 792
    -0x1.ffffffffffffd0000000000000ffffffp1,
    0x1.fffffffffffffp1023,
    -0x1.ffffffffffffep-1023,
    0x1.0p-1022
  },
  { // Entry 793
    -0x1.ffffffffffffd0000000000001p1,
    0x1.fffffffffffffp1023,
    -0x1.ffffffffffffep-1023,
    -0x1.0p-1022
  },
  { // Entry 794
    -0x1.ffffffffffffd0000000000000ffffffp1,
    0x1.fffffffffffffp1023,
    -0x1.ffffffffffffep-1023,
    0x1.ffffffffffffep-1023
  },
  { // Entry 795
    -0x1.ffffffffffffd0000000000001p1,
    0x1.fffffffffffffp1023,
    -0x1.ffffffffffffep-1023,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 796
    -0x1.ffffffffffffd0000000000000ffffffp1,
    0x1.fffffffffffffp1023,
    -0x1.ffffffffffffep-1023,
    0x1.0p-1074
  },
  { // Entry 797
    -0x1.ffffffffffffd0000000000001p1,
    0x1.fffffffffffffp1023,
    -0x1.ffffffffffffep-1023,
    -0x1.0p-1074
  },
  { // Entry 798
    -0x1.ffffffffffffd0000000000001p1,
    0x1.fffffffffffffp1023,
    -0x1.ffffffffffffep-1023,
    0.0
  },
  { // Entry 799
    -0x1.ffffffffffffd0000000000001p1,
    0x1.fffffffffffffp1023,
    -0x1.ffffffffffffep-1023,
    -0.0
  },
  { // Entry 800
    HUGE_VAL,
    0x1.fffffffffffffp1023,
    0x1.0p-1074,
    HUGE_VAL
  },
  { // Entry 801
    -HUGE_VAL,
    0x1.fffffffffffffp1023,
    0x1.0p-1074,
    -HUGE_VAL
  },
  { // Entry 802
    0x1.fffffffffffff0p1023,
    0x1.fffffffffffffp1023,
    0x1.0p-1074,
    0x1.fffffffffffffp1023
  },
  { // Entry 803
    -0x1.ffffffffffffefffffffffffffffffffp1023,
    0x1.fffffffffffffp1023,
    0x1.0p-1074,
    -0x1.fffffffffffffp1023
  },
  { // Entry 804
    0x1.fffffffffffff0p-51,
    0x1.fffffffffffffp1023,
    0x1.0p-1074,
    0x1.0p-1022
  },
  { // Entry 805
    0x1.ffffffffffffefffffffffffffffffffp-51,
    0x1.fffffffffffffp1023,
    0x1.0p-1074,
    -0x1.0p-1022
  },
  { // Entry 806
    0x1.fffffffffffff0p-51,
    0x1.fffffffffffffp1023,
    0x1.0p-1074,
    0x1.ffffffffffffep-1023
  },
  { // Entry 807
    0x1.ffffffffffffefffffffffffffffffffp-51,
    0x1.fffffffffffffp1023,
    0x1.0p-1074,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 808
    0x1.fffffffffffff0p-51,
    0x1.fffffffffffffp1023,
    0x1.0p-1074,
    0x1.0p-1074
  },
  { // Entry 809
    0x1.ffffffffffffefffffffffffffffffffp-51,
    0x1.fffffffffffffp1023,
    0x1.0p-1074,
    -0x1.0p-1074
  },
  { // Entry 810
    0x1.fffffffffffff0p-51,
    0x1.fffffffffffffp1023,
    0x1.0p-1074,
    0.0
  },
  { // Entry 811
    0x1.fffffffffffff0p-51,
    0x1.fffffffffffffp1023,
    0x1.0p-1074,
    -0.0
  },
  { // Entry 812
    HUGE_VAL,
    0x1.fffffffffffffp1023,
    -0x1.0p-1074,
    HUGE_VAL
  },
  { // Entry 813
    -HUGE_VAL,
    0x1.fffffffffffffp1023,
    -0x1.0p-1074,
    -HUGE_VAL
  },
  { // Entry 814
    0x1.ffffffffffffefffffffffffffffffffp1023,
    0x1.fffffffffffffp1023,
    -0x1.0p-1074,
    0x1.fffffffffffffp1023
  },
  { // Entry 815
    -0x1.fffffffffffff0p1023,
    0x1.fffffffffffffp1023,
    -0x1.0p-1074,
    -0x1.fffffffffffffp1023
  },
  { // Entry 816
    -0x1.ffffffffffffefffffffffffffffffffp-51,
    0x1.fffffffffffffp1023,
    -0x1.0p-1074,
    0x1.0p-1022
  },
  { // Entry 817
    -0x1.fffffffffffff0p-51,
    0x1.fffffffffffffp1023,
    -0x1.0p-1074,
    -0x1.0p-1022
  },
  { // Entry 818
    -0x1.ffffffffffffefffffffffffffffffffp-51,
    0x1.fffffffffffffp1023,
    -0x1.0p-1074,
    0x1.ffffffffffffep-1023
  },
  { // Entry 819
    -0x1.fffffffffffff0p-51,
    0x1.fffffffffffffp1023,
    -0x1.0p-1074,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 820
    -0x1.ffffffffffffefffffffffffffffffffp-51,
    0x1.fffffffffffffp1023,
    -0x1.0p-1074,
    0x1.0p-1074
  },
  { // Entry 821
    -0x1.fffffffffffff0p-51,
    0x1.fffffffffffffp1023,
    -0x1.0p-1074,
    -0x1.0p-1074
  },
  { // Entry 822
    -0x1.fffffffffffff0p-51,
    0x1.fffffffffffffp1023,
    -0x1.0p-1074,
    0.0
  },
  { // Entry 823
    -0x1.fffffffffffff0p-51,
    0x1.fffffffffffffp1023,
    -0x1.0p-1074,
    -0.0
  },
  { // Entry 824
    HUGE_VAL,
    0x1.fffffffffffffp1023,
    0.0,
    HUGE_VAL
  },
  { // Entry 825
    -HUGE_VAL,
    0x1.fffffffffffffp1023,
    0.0,
    -HUGE_VAL
  },
  { // Entry 826
    0x1.fffffffffffff0p1023,
    0x1.fffffffffffffp1023,
    0.0,
    0x1.fffffffffffffp1023
  },
  { // Entry 827
    -0x1.fffffffffffff0p1023,
    0x1.fffffffffffffp1023,
    0.0,
    -0x1.fffffffffffffp1023
  },
  { // Entry 828
    0x1.p-1022,
    0x1.fffffffffffffp1023,
    0.0,
    0x1.0p-1022
  },
  { // Entry 829
    -0x1.p-1022,
    0x1.fffffffffffffp1023,
    0.0,
    -0x1.0p-1022
  },
  { // Entry 830
    0x1.ffffffffffffe0p-1023,
    0x1.fffffffffffffp1023,
    0.0,
    0x1.ffffffffffffep-1023
  },
  { // Entry 831
    -0x1.ffffffffffffe0p-1023,
    0x1.fffffffffffffp1023,
    0.0,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 832
    0x1.p-1074,
    0x1.fffffffffffffp1023,
    0.0,
    0x1.0p-1074
  },
  { // Entry 833
    -0x1.p-1074,
    0x1.fffffffffffffp1023,
    0.0,
    -0x1.0p-1074
  },
  { // Entry 834
    0.0,
    0x1.fffffffffffffp1023,
    0.0,
    0.0
  },
  { // Entry 835
    0.0,
    0x1.fffffffffffffp1023,
    0.0,
    -0.0
  },
  { // Entry 836
    HUGE_VAL,
    0x1.fffffffffffffp1023,
    -0.0,
    HUGE_VAL
  },
  { // Entry 837
    -HUGE_VAL,
    0x1.fffffffffffffp1023,
    -0.0,
    -HUGE_VAL
  },
  { // Entry 838
    0x1.fffffffffffff0p1023,
    0x1.fffffffffffffp1023,
    -0.0,
    0x1.fffffffffffffp1023
  },
  { // Entry 839
    -0x1.fffffffffffff0p1023,
    0x1.fffffffffffffp1023,
    -0.0,
    -0x1.fffffffffffffp1023
  },
  { // Entry 840
    0x1.p-1022,
    0x1.fffffffffffffp1023,
    -0.0,
    0x1.0p-1022
  },
  { // Entry 841
    -0x1.p-1022,
    0x1.fffffffffffffp1023,
    -0.0,
    -0x1.0p-1022
  },
  { // Entry 842
    0x1.ffffffffffffe0p-1023,
    0x1.fffffffffffffp1023,
    -0.0,
    0x1.ffffffffffffep-1023
  },
  { // Entry 843
    -0x1.ffffffffffffe0p-1023,
    0x1.fffffffffffffp1023,
    -0.0,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 844
    0x1.p-1074,
    0x1.fffffffffffffp1023,
    -0.0,
    0x1.0p-1074
  },
  { // Entry 845
    -0x1.p-1074,
    0x1.fffffffffffffp1023,
    -0.0,
    -0x1.0p-1074
  },
  { // Entry 846
    0.0,
    0x1.fffffffffffffp1023,
    -0.0,
    0.0
  },
  { // Entry 847
    -0.0,
    0x1.fffffffffffffp1023,
    -0.0,
    -0.0
  },
  { // Entry 848
    -HUGE_VAL,
    -0x1.fffffffffffffp1023,
    HUGE_VAL,
    -HUGE_VAL
  },
  { // Entry 849
    -HUGE_VAL,
    -0x1.fffffffffffffp1023,
    HUGE_VAL,
    0x1.fffffffffffffp1023
  },
  { // Entry 850
    -HUGE_VAL,
    -0x1.fffffffffffffp1023,
    HUGE_VAL,
    -0x1.fffffffffffffp1023
  },
  { // Entry 851
    -HUGE_VAL,
    -0x1.fffffffffffffp1023,
    HUGE_VAL,
    0x1.0p-1022
  },
  { // Entry 852
    -HUGE_VAL,
    -0x1.fffffffffffffp1023,
    HUGE_VAL,
    -0x1.0p-1022
  },
  { // Entry 853
    -HUGE_VAL,
    -0x1.fffffffffffffp1023,
    HUGE_VAL,
    0x1.ffffffffffffep-1023
  },
  { // Entry 854
    -HUGE_VAL,
    -0x1.fffffffffffffp1023,
    HUGE_VAL,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 855
    -HUGE_VAL,
    -0x1.fffffffffffffp1023,
    HUGE_VAL,
    0x1.0p-1074
  },
  { // Entry 856
    -HUGE_VAL,
    -0x1.fffffffffffffp1023,
    HUGE_VAL,
    -0x1.0p-1074
  },
  { // Entry 857
    -HUGE_VAL,
    -0x1.fffffffffffffp1023,
    HUGE_VAL,
    0.0
  },
  { // Entry 858
    -HUGE_VAL,
    -0x1.fffffffffffffp1023,
    HUGE_VAL,
    -0.0
  },
  { // Entry 859
    HUGE_VAL,
    -0x1.fffffffffffffp1023,
    -HUGE_VAL,
    HUGE_VAL
  },
  { // Entry 860
    HUGE_VAL,
    -0x1.fffffffffffffp1023,
    -HUGE_VAL,
    0x1.fffffffffffffp1023
  },
  { // Entry 861
    HUGE_VAL,
    -0x1.fffffffffffffp1023,
    -HUGE_VAL,
    -0x1.fffffffffffffp1023
  },
  { // Entry 862
    HUGE_VAL,
    -0x1.fffffffffffffp1023,
    -HUGE_VAL,
    0x1.0p-1022
  },
  { // Entry 863
    HUGE_VAL,
    -0x1.fffffffffffffp1023,
    -HUGE_VAL,
    -0x1.0p-1022
  },
  { // Entry 864
    HUGE_VAL,
    -0x1.fffffffffffffp1023,
    -HUGE_VAL,
    0x1.ffffffffffffep-1023
  },
  { // Entry 865
    HUGE_VAL,
    -0x1.fffffffffffffp1023,
    -HUGE_VAL,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 866
    HUGE_VAL,
    -0x1.fffffffffffffp1023,
    -HUGE_VAL,
    0x1.0p-1074
  },
  { // Entry 867
    HUGE_VAL,
    -0x1.fffffffffffffp1023,
    -HUGE_VAL,
    -0x1.0p-1074
  },
  { // Entry 868
    HUGE_VAL,
    -0x1.fffffffffffffp1023,
    -HUGE_VAL,
    0.0
  },
  { // Entry 869
    HUGE_VAL,
    -0x1.fffffffffffffp1023,
    -HUGE_VAL,
    -0.0
  },
  { // Entry 870
    HUGE_VAL,
    -0x1.fffffffffffffp1023,
    0x1.fffffffffffffp1023,
    HUGE_VAL
  },
  { // Entry 871
    -HUGE_VAL,
    -0x1.fffffffffffffp1023,
    0x1.fffffffffffffp1023,
    -HUGE_VAL
  },
  { // Entry 872
    -HUGE_VAL,
    -0x1.fffffffffffffp1023,
    0x1.fffffffffffffp1023,
    0x1.fffffffffffffp1023
  },
  { // Entry 873
    -HUGE_VAL,
    -0x1.fffffffffffffp1023,
    0x1.fffffffffffffp1023,
    -0x1.fffffffffffffp1023
  },
  { // Entry 874
    -HUGE_VAL,
    -0x1.fffffffffffffp1023,
    0x1.fffffffffffffp1023,
    0x1.0p-1022
  },
  { // Entry 875
    -HUGE_VAL,
    -0x1.fffffffffffffp1023,
    0x1.fffffffffffffp1023,
    -0x1.0p-1022
  },
  { // Entry 876
    -HUGE_VAL,
    -0x1.fffffffffffffp1023,
    0x1.fffffffffffffp1023,
    0x1.ffffffffffffep-1023
  },
  { // Entry 877
    -HUGE_VAL,
    -0x1.fffffffffffffp1023,
    0x1.fffffffffffffp1023,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 878
    -HUGE_VAL,
    -0x1.fffffffffffffp1023,
    0x1.fffffffffffffp1023,
    0x1.0p-1074
  },
  { // Entry 879
    -HUGE_VAL,
    -0x1.fffffffffffffp1023,
    0x1.fffffffffffffp1023,
    -0x1.0p-1074
  },
  { // Entry 880
    -HUGE_VAL,
    -0x1.fffffffffffffp1023,
    0x1.fffffffffffffp1023,
    0.0
  },
  { // Entry 881
    -HUGE_VAL,
    -0x1.fffffffffffffp1023,
    0x1.fffffffffffffp1023,
    -0.0
  },
  { // Entry 882
    HUGE_VAL,
    -0x1.fffffffffffffp1023,
    -0x1.fffffffffffffp1023,
    HUGE_VAL
  },
  { // Entry 883
    -HUGE_VAL,
    -0x1.fffffffffffffp1023,
    -0x1.fffffffffffffp1023,
    -HUGE_VAL
  },
  { // Entry 884
    HUGE_VAL,
    -0x1.fffffffffffffp1023,
    -0x1.fffffffffffffp1023,
    0x1.fffffffffffffp1023
  },
  { // Entry 885
    HUGE_VAL,
    -0x1.fffffffffffffp1023,
    -0x1.fffffffffffffp1023,
    -0x1.fffffffffffffp1023
  },
  { // Entry 886
    HUGE_VAL,
    -0x1.fffffffffffffp1023,
    -0x1.fffffffffffffp1023,
    0x1.0p-1022
  },
  { // Entry 887
    HUGE_VAL,
    -0x1.fffffffffffffp1023,
    -0x1.fffffffffffffp1023,
    -0x1.0p-1022
  },
  { // Entry 888
    HUGE_VAL,
    -0x1.fffffffffffffp1023,
    -0x1.fffffffffffffp1023,
    0x1.ffffffffffffep-1023
  },
  { // Entry 889
    HUGE_VAL,
    -0x1.fffffffffffffp1023,
    -0x1.fffffffffffffp1023,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 890
    HUGE_VAL,
    -0x1.fffffffffffffp1023,
    -0x1.fffffffffffffp1023,
    0x1.0p-1074
  },
  { // Entry 891
    HUGE_VAL,
    -0x1.fffffffffffffp1023,
    -0x1.fffffffffffffp1023,
    -0x1.0p-1074
  },
  { // Entry 892
    HUGE_VAL,
    -0x1.fffffffffffffp1023,
    -0x1.fffffffffffffp1023,
    0.0
  },
  { // Entry 893
    HUGE_VAL,
    -0x1.fffffffffffffp1023,
    -0x1.fffffffffffffp1023,
    -0.0
  },
  { // Entry 894
    HUGE_VAL,
    -0x1.fffffffffffffp1023,
    0x1.0p-1022,
    HUGE_VAL
  },
  { // Entry 895
    -HUGE_VAL,
    -0x1.fffffffffffffp1023,
    0x1.0p-1022,
    -HUGE_VAL
  },
  { // Entry 896
    0x1.ffffffffffffefffffffffffffffffffp1023,
    -0x1.fffffffffffffp1023,
    0x1.0p-1022,
    0x1.fffffffffffffp1023
  },
  { // Entry 897
    -0x1.fffffffffffff0p1023,
    -0x1.fffffffffffffp1023,
    0x1.0p-1022,
    -0x1.fffffffffffffp1023
  },
  { // Entry 898
    -0x1.ffffffffffffefffffffffffffffffffp1,
    -0x1.fffffffffffffp1023,
    0x1.0p-1022,
    0x1.0p-1022
  },
  { // Entry 899
    -0x1.fffffffffffff0p1,
    -0x1.fffffffffffffp1023,
    0x1.0p-1022,
    -0x1.0p-1022
  },
  { // Entry 900
    -0x1.ffffffffffffefffffffffffffffffffp1,
    -0x1.fffffffffffffp1023,
    0x1.0p-1022,
    0x1.ffffffffffffep-1023
  },
  { // Entry 901
    -0x1.fffffffffffff0p1,
    -0x1.fffffffffffffp1023,
    0x1.0p-1022,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 902
    -0x1.ffffffffffffefffffffffffffffffffp1,
    -0x1.fffffffffffffp1023,
    0x1.0p-1022,
    0x1.0p-1074
  },
  { // Entry 903
    -0x1.fffffffffffff0p1,
    -0x1.fffffffffffffp1023,
    0x1.0p-1022,
    -0x1.0p-1074
  },
  { // Entry 904
    -0x1.fffffffffffff0p1,
    -0x1.fffffffffffffp1023,
    0x1.0p-1022,
    0.0
  },
  { // Entry 905
    -0x1.fffffffffffff0p1,
    -0x1.fffffffffffffp1023,
    0x1.0p-1022,
    -0.0
  },
  { // Entry 906
    HUGE_VAL,
    -0x1.fffffffffffffp1023,
    -0x1.0p-1022,
    HUGE_VAL
  },
  { // Entry 907
    -HUGE_VAL,
    -0x1.fffffffffffffp1023,
    -0x1.0p-1022,
    -HUGE_VAL
  },
  { // Entry 908
    0x1.fffffffffffff0p1023,
    -0x1.fffffffffffffp1023,
    -0x1.0p-1022,
    0x1.fffffffffffffp1023
  },
  { // Entry 909
    -0x1.ffffffffffffefffffffffffffffffffp1023,
    -0x1.fffffffffffffp1023,
    -0x1.0p-1022,
    -0x1.fffffffffffffp1023
  },
  { // Entry 910
    0x1.fffffffffffff0p1,
    -0x1.fffffffffffffp1023,
    -0x1.0p-1022,
    0x1.0p-1022
  },
  { // Entry 911
    0x1.ffffffffffffefffffffffffffffffffp1,
    -0x1.fffffffffffffp1023,
    -0x1.0p-1022,
    -0x1.0p-1022
  },
  { // Entry 912
    0x1.fffffffffffff0p1,
    -0x1.fffffffffffffp1023,
    -0x1.0p-1022,
    0x1.ffffffffffffep-1023
  },
  { // Entry 913
    0x1.ffffffffffffefffffffffffffffffffp1,
    -0x1.fffffffffffffp1023,
    -0x1.0p-1022,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 914
    0x1.fffffffffffff0p1,
    -0x1.fffffffffffffp1023,
    -0x1.0p-1022,
    0x1.0p-1074
  },
  { // Entry 915
    0x1.ffffffffffffefffffffffffffffffffp1,
    -0x1.fffffffffffffp1023,
    -0x1.0p-1022,
    -0x1.0p-1074
  },
  { // Entry 916
    0x1.fffffffffffff0p1,
    -0x1.fffffffffffffp1023,
    -0x1.0p-1022,
    0.0
  },
  { // Entry 917
    0x1.fffffffffffff0p1,
    -0x1.fffffffffffffp1023,
    -0x1.0p-1022,
    -0.0
  },
  { // Entry 918
    HUGE_VAL,
    -0x1.fffffffffffffp1023,
    0x1.ffffffffffffep-1023,
    HUGE_VAL
  },
  { // Entry 919
    -HUGE_VAL,
    -0x1.fffffffffffffp1023,
    0x1.ffffffffffffep-1023,
    -HUGE_VAL
  },
  { // Entry 920
    0x1.ffffffffffffefffffffffffffffffffp1023,
    -0x1.fffffffffffffp1023,
    0x1.ffffffffffffep-1023,
    0x1.fffffffffffffp1023
  },
  { // Entry 921
    -0x1.fffffffffffff0p1023,
    -0x1.fffffffffffffp1023,
    0x1.ffffffffffffep-1023,
    -0x1.fffffffffffffp1023
  },
  { // Entry 922
    -0x1.ffffffffffffd0000000000000ffffffp1,
    -0x1.fffffffffffffp1023,
    0x1.ffffffffffffep-1023,
    0x1.0p-1022
  },
  { // Entry 923
    -0x1.ffffffffffffd0000000000001p1,
    -0x1.fffffffffffffp1023,
    0x1.ffffffffffffep-1023,
    -0x1.0p-1022
  },
  { // Entry 924
    -0x1.ffffffffffffd0000000000000ffffffp1,
    -0x1.fffffffffffffp1023,
    0x1.ffffffffffffep-1023,
    0x1.ffffffffffffep-1023
  },
  { // Entry 925
    -0x1.ffffffffffffd0000000000001p1,
    -0x1.fffffffffffffp1023,
    0x1.ffffffffffffep-1023,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 926
    -0x1.ffffffffffffd0000000000000ffffffp1,
    -0x1.fffffffffffffp1023,
    0x1.ffffffffffffep-1023,
    0x1.0p-1074
  },
  { // Entry 927
    -0x1.ffffffffffffd0000000000001p1,
    -0x1.fffffffffffffp1023,
    0x1.ffffffffffffep-1023,
    -0x1.0p-1074
  },
  { // Entry 928
    -0x1.ffffffffffffd0000000000001p1,
    -0x1.fffffffffffffp1023,
    0x1.ffffffffffffep-1023,
    0.0
  },
  { // Entry 929
    -0x1.ffffffffffffd0000000000001p1,
    -0x1.fffffffffffffp1023,
    0x1.ffffffffffffep-1023,
    -0.0
  },
  { // Entry 930
    HUGE_VAL,
    -0x1.fffffffffffffp1023,
    -0x1.ffffffffffffep-1023,
    HUGE_VAL
  },
  { // Entry 931
    -HUGE_VAL,
    -0x1.fffffffffffffp1023,
    -0x1.ffffffffffffep-1023,
    -HUGE_VAL
  },
  { // Entry 932
    0x1.fffffffffffff0p1023,
    -0x1.fffffffffffffp1023,
    -0x1.ffffffffffffep-1023,
    0x1.fffffffffffffp1023
  },
  { // Entry 933
    -0x1.ffffffffffffefffffffffffffffffffp1023,
    -0x1.fffffffffffffp1023,
    -0x1.ffffffffffffep-1023,
    -0x1.fffffffffffffp1023
  },
  { // Entry 934
    0x1.ffffffffffffd0000000000001p1,
    -0x1.fffffffffffffp1023,
    -0x1.ffffffffffffep-1023,
    0x1.0p-1022
  },
  { // Entry 935
    0x1.ffffffffffffd0000000000000ffffffp1,
    -0x1.fffffffffffffp1023,
    -0x1.ffffffffffffep-1023,
    -0x1.0p-1022
  },
  { // Entry 936
    0x1.ffffffffffffd0000000000001p1,
    -0x1.fffffffffffffp1023,
    -0x1.ffffffffffffep-1023,
    0x1.ffffffffffffep-1023
  },
  { // Entry 937
    0x1.ffffffffffffd0000000000000ffffffp1,
    -0x1.fffffffffffffp1023,
    -0x1.ffffffffffffep-1023,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 938
    0x1.ffffffffffffd0000000000001p1,
    -0x1.fffffffffffffp1023,
    -0x1.ffffffffffffep-1023,
    0x1.0p-1074
  },
  { // Entry 939
    0x1.ffffffffffffd0000000000000ffffffp1,
    -0x1.fffffffffffffp1023,
    -0x1.ffffffffffffep-1023,
    -0x1.0p-1074
  },
  { // Entry 940
    0x1.ffffffffffffd0000000000001p1,
    -0x1.fffffffffffffp1023,
    -0x1.ffffffffffffep-1023,
    0.0
  },
  { // Entry 941
    0x1.ffffffffffffd0000000000001p1,
    -0x1.fffffffffffffp1023,
    -0x1.ffffffffffffep-1023,
    -0.0
  },
  { // Entry 942
    HUGE_VAL,
    -0x1.fffffffffffffp1023,
    0x1.0p-1074,
    HUGE_VAL
  },
  { // Entry 943
    -HUGE_VAL,
    -0x1.fffffffffffffp1023,
    0x1.0p-1074,
    -HUGE_VAL
  },
  { // Entry 944
    0x1.ffffffffffffefffffffffffffffffffp1023,
    -0x1.fffffffffffffp1023,
    0x1.0p-1074,
    0x1.fffffffffffffp1023
  },
  { // Entry 945
    -0x1.fffffffffffff0p1023,
    -0x1.fffffffffffffp1023,
    0x1.0p-1074,
    -0x1.fffffffffffffp1023
  },
  { // Entry 946
    -0x1.ffffffffffffefffffffffffffffffffp-51,
    -0x1.fffffffffffffp1023,
    0x1.0p-1074,
    0x1.0p-1022
  },
  { // Entry 947
    -0x1.fffffffffffff0p-51,
    -0x1.fffffffffffffp1023,
    0x1.0p-1074,
    -0x1.0p-1022
  },
  { // Entry 948
    -0x1.ffffffffffffefffffffffffffffffffp-51,
    -0x1.fffffffffffffp1023,
    0x1.0p-1074,
    0x1.ffffffffffffep-1023
  },
  { // Entry 949
    -0x1.fffffffffffff0p-51,
    -0x1.fffffffffffffp1023,
    0x1.0p-1074,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 950
    -0x1.ffffffffffffefffffffffffffffffffp-51,
    -0x1.fffffffffffffp1023,
    0x1.0p-1074,
    0x1.0p-1074
  },
  { // Entry 951
    -0x1.fffffffffffff0p-51,
    -0x1.fffffffffffffp1023,
    0x1.0p-1074,
    -0x1.0p-1074
  },
  { // Entry 952
    -0x1.fffffffffffff0p-51,
    -0x1.fffffffffffffp1023,
    0x1.0p-1074,
    0.0
  },
  { // Entry 953
    -0x1.fffffffffffff0p-51,
    -0x1.fffffffffffffp1023,
    0x1.0p-1074,
    -0.0
  },
  { // Entry 954
    HUGE_VAL,
    -0x1.fffffffffffffp1023,
    -0x1.0p-1074,
    HUGE_VAL
  },
  { // Entry 955
    -HUGE_VAL,
    -0x1.fffffffffffffp1023,
    -0x1.0p-1074,
    -HUGE_VAL
  },
  { // Entry 956
    0x1.fffffffffffff0p1023,
    -0x1.fffffffffffffp1023,
    -0x1.0p-1074,
    0x1.fffffffffffffp1023
  },
  { // Entry 957
    -0x1.ffffffffffffefffffffffffffffffffp1023,
    -0x1.fffffffffffffp1023,
    -0x1.0p-1074,
    -0x1.fffffffffffffp1023
  },
  { // Entry 958
    0x1.fffffffffffff0p-51,
    -0x1.fffffffffffffp1023,
    -0x1.0p-1074,
    0x1.0p-1022
  },
  { // Entry 959
    0x1.ffffffffffffefffffffffffffffffffp-51,
    -0x1.fffffffffffffp1023,
    -0x1.0p-1074,
    -0x1.0p-1022
  },
  { // Entry 960
    0x1.fffffffffffff0p-51,
    -0x1.fffffffffffffp1023,
    -0x1.0p-1074,
    0x1.ffffffffffffep-1023
  },
  { // Entry 961
    0x1.ffffffffffffefffffffffffffffffffp-51,
    -0x1.fffffffffffffp1023,
    -0x1.0p-1074,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 962
    0x1.fffffffffffff0p-51,
    -0x1.fffffffffffffp1023,
    -0x1.0p-1074,
    0x1.0p-1074
  },
  { // Entry 963
    0x1.ffffffffffffefffffffffffffffffffp-51,
    -0x1.fffffffffffffp1023,
    -0x1.0p-1074,
    -0x1.0p-1074
  },
  { // Entry 964
    0x1.fffffffffffff0p-51,
    -0x1.fffffffffffffp1023,
    -0x1.0p-1074,
    0.0
  },
  { // Entry 965
    0x1.fffffffffffff0p-51,
    -0x1.fffffffffffffp1023,
    -0x1.0p-1074,
    -0.0
  },
  { // Entry 966
    HUGE_VAL,
    -0x1.fffffffffffffp1023,
    0.0,
    HUGE_VAL
  },
  { // Entry 967
    -HUGE_VAL,
    -0x1.fffffffffffffp1023,
    0.0,
    -HUGE_VAL
  },
  { // Entry 968
    0x1.fffffffffffff0p1023,
    -0x1.fffffffffffffp1023,
    0.0,
    0x1.fffffffffffffp1023
  },
  { // Entry 969
    -0x1.fffffffffffff0p1023,
    -0x1.fffffffffffffp1023,
    0.0,
    -0x1.fffffffffffffp1023
  },
  { // Entry 970
    0x1.p-1022,
    -0x1.fffffffffffffp1023,
    0.0,
    0x1.0p-1022
  },
  { // Entry 971
    -0x1.p-1022,
    -0x1.fffffffffffffp1023,
    0.0,
    -0x1.0p-1022
  },
  { // Entry 972
    0x1.ffffffffffffe0p-1023,
    -0x1.fffffffffffffp1023,
    0.0,
    0x1.ffffffffffffep-1023
  },
  { // Entry 973
    -0x1.ffffffffffffe0p-1023,
    -0x1.fffffffffffffp1023,
    0.0,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 974
    0x1.p-1074,
    -0x1.fffffffffffffp1023,
    0.0,
    0x1.0p-1074
  },
  { // Entry 975
    -0x1.p-1074,
    -0x1.fffffffffffffp1023,
    0.0,
    -0x1.0p-1074
  },
  { // Entry 976
    0.0,
    -0x1.fffffffffffffp1023,
    0.0,
    0.0
  },
  { // Entry 977
    -0.0,
    -0x1.fffffffffffffp1023,
    0.0,
    -0.0
  },
  { // Entry 978
    HUGE_VAL,
    -0x1.fffffffffffffp1023,
    -0.0,
    HUGE_VAL
  },
  { // Entry 979
    -HUGE_VAL,
    -0x1.fffffffffffffp1023,
    -0.0,
    -HUGE_VAL
  },
  { // Entry 980
    0x1.fffffffffffff0p1023,
    -0x1.fffffffffffffp1023,
    -0.0,
    0x1.fffffffffffffp1023
  },
  { // Entry 981
    -0x1.fffffffffffff0p1023,
    -0x1.fffffffffffffp1023,
    -0.0,
    -0x1.fffffffffffffp1023
  },
  { // Entry 982
    0x1.p-1022,
    -0x1.fffffffffffffp1023,
    -0.0,
    0x1.0p-1022
  },
  { // Entry 983
    -0x1.p-1022,
    -0x1.fffffffffffffp1023,
    -0.0,
    -0x1.0p-1022
  },
  { // Entry 984
    0x1.ffffffffffffe0p-1023,
    -0x1.fffffffffffffp1023,
    -0.0,
    0x1.ffffffffffffep-1023
  },
  { // Entry 985
    -0x1.ffffffffffffe0p-1023,
    -0x1.fffffffffffffp1023,
    -0.0,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 986
    0x1.p-1074,
    -0x1.fffffffffffffp1023,
    -0.0,
    0x1.0p-1074
  },
  { // Entry 987
    -0x1.p-1074,
    -0x1.fffffffffffffp1023,
    -0.0,
    -0x1.0p-1074
  },
  { // Entry 988
    0.0,
    -0x1.fffffffffffffp1023,
    -0.0,
    0.0
  },
  { // Entry 989
    0.0,
    -0x1.fffffffffffffp1023,
    -0.0,
    -0.0
  },
  { // Entry 990
    HUGE_VAL,
    0x1.0p-1022,
    HUGE_VAL,
    HUGE_VAL
  },
  { // Entry 991
    HUGE_VAL,
    0x1.0p-1022,
    HUGE_VAL,
    0x1.fffffffffffffp1023
  },
  { // Entry 992
    HUGE_VAL,
    0x1.0p-1022,
    HUGE_VAL,
    -0x1.fffffffffffffp1023
  },
  { // Entry 993
    HUGE_VAL,
    0x1.0p-1022,
    HUGE_VAL,
    0x1.0p-1022
  },
  { // Entry 994
    HUGE_VAL,
    0x1.0p-1022,
    HUGE_VAL,
    -0x1.0p-1022
  },
  { // Entry 995
    HUGE_VAL,
    0x1.0p-1022,
    HUGE_VAL,
    0x1.ffffffffffffep-1023
  },
  { // Entry 996
    HUGE_VAL,
    0x1.0p-1022,
    HUGE_VAL,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 997
    HUGE_VAL,
    0x1.0p-1022,
    HUGE_VAL,
    0x1.0p-1074
  },
  { // Entry 998
    HUGE_VAL,
    0x1.0p-1022,
    HUGE_VAL,
    -0x1.0p-1074
  },
  { // Entry 999
    HUGE_VAL,
    0x1.0p-1022,
    HUGE_VAL,
    0.0
  },
  { // Entry 1000
    HUGE_VAL,
    0x1.0p-1022,
    HUGE_VAL,
    -0.0
  },
  { // Entry 1001
    -HUGE_VAL,
    0x1.0p-1022,
    -HUGE_VAL,
    -HUGE_VAL
  },
  { // Entry 1002
    -HUGE_VAL,
    0x1.0p-1022,
    -HUGE_VAL,
    0x1.fffffffffffffp1023
  },
  { // Entry 1003
    -HUGE_VAL,
    0x1.0p-1022,
    -HUGE_VAL,
    -0x1.fffffffffffffp1023
  },
  { // Entry 1004
    -HUGE_VAL,
    0x1.0p-1022,
    -HUGE_VAL,
    0x1.0p-1022
  },
  { // Entry 1005
    -HUGE_VAL,
    0x1.0p-1022,
    -HUGE_VAL,
    -0x1.0p-1022
  },
  { // Entry 1006
    -HUGE_VAL,
    0x1.0p-1022,
    -HUGE_VAL,
    0x1.ffffffffffffep-1023
  },
  { // Entry 1007
    -HUGE_VAL,
    0x1.0p-1022,
    -HUGE_VAL,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 1008
    -HUGE_VAL,
    0x1.0p-1022,
    -HUGE_VAL,
    0x1.0p-1074
  },
  { // Entry 1009
    -HUGE_VAL,
    0x1.0p-1022,
    -HUGE_VAL,
    -0x1.0p-1074
  },
  { // Entry 1010
    -HUGE_VAL,
    0x1.0p-1022,
    -HUGE_VAL,
    0.0
  },
  { // Entry 1011
    -HUGE_VAL,
    0x1.0p-1022,
    -HUGE_VAL,
    -0.0
  },
  { // Entry 1012
    HUGE_VAL,
    0x1.0p-1022,
    0x1.fffffffffffffp1023,
    HUGE_VAL
  },
  { // Entry 1013
    -HUGE_VAL,
    0x1.0p-1022,
    0x1.fffffffffffffp1023,
    -HUGE_VAL
  },
  { // Entry 1014
    0x1.fffffffffffff0p1023,
    0x1.0p-1022,
    0x1.fffffffffffffp1023,
    0x1.fffffffffffffp1023
  },
  { // Entry 1015
    -0x1.ffffffffffffefffffffffffffffffffp1023,
    0x1.0p-1022,
    0x1.fffffffffffffp1023,
    -0x1.fffffffffffffp1023
  },
  { // Entry 1016
    0x1.fffffffffffff0p1,
    0x1.0p-1022,
    0x1.fffffffffffffp1023,
    0x1.0p-1022
  },
  { // Entry 1017
    0x1.ffffffffffffefffffffffffffffffffp1,
    0x1.0p-1022,
    0x1.fffffffffffffp1023,
    -0x1.0p-1022
  },
  { // Entry 1018
    0x1.fffffffffffff0p1,
    0x1.0p-1022,
    0x1.fffffffffffffp1023,
    0x1.ffffffffffffep-1023
  },
  { // Entry 1019
    0x1.ffffffffffffefffffffffffffffffffp1,
    0x1.0p-1022,
    0x1.fffffffffffffp1023,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 1020
    0x1.fffffffffffff0p1,
    0x1.0p-1022,
    0x1.fffffffffffffp1023,
    0x1.0p-1074
  },
  { // Entry 1021
    0x1.ffffffffffffefffffffffffffffffffp1,
    0x1.0p-1022,
    0x1.fffffffffffffp1023,
    -0x1.0p-1074
  },
  { // Entry 1022
    0x1.fffffffffffff0p1,
    0x1.0p-1022,
    0x1.fffffffffffffp1023,
    0.0
  },
  { // Entry 1023
    0x1.fffffffffffff0p1,
    0x1.0p-1022,
    0x1.fffffffffffffp1023,
    -0.0
  },
  { // Entry 1024
    HUGE_VAL,
    0x1.0p-1022,
    -0x1.fffffffffffffp1023,
    HUGE_VAL
  },
  { // Entry 1025
    -HUGE_VAL,
    0x1.0p-1022,
    -0x1.fffffffffffffp1023,
    -HUGE_VAL
  },
  { // Entry 1026
    0x1.ffffffffffffefffffffffffffffffffp1023,
    0x1.0p-1022,
    -0x1.fffffffffffffp1023,
    0x1.fffffffffffffp1023
  },
  { // Entry 1027
    -0x1.fffffffffffff0p1023,
    0x1.0p-1022,
    -0x1.fffffffffffffp1023,
    -0x1.fffffffffffffp1023
  },
  { // Entry 1028
    -0x1.ffffffffffffefffffffffffffffffffp1,
    0x1.0p-1022,
    -0x1.fffffffffffffp1023,
    0x1.0p-1022
  },
  { // Entry 1029
    -0x1.fffffffffffff0p1,
    0x1.0p-1022,
    -0x1.fffffffffffffp1023,
    -0x1.0p-1022
  },
  { // Entry 1030
    -0x1.ffffffffffffefffffffffffffffffffp1,
    0x1.0p-1022,
    -0x1.fffffffffffffp1023,
    0x1.ffffffffffffep-1023
  },
  { // Entry 1031
    -0x1.fffffffffffff0p1,
    0x1.0p-1022,
    -0x1.fffffffffffffp1023,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 1032
    -0x1.ffffffffffffefffffffffffffffffffp1,
    0x1.0p-1022,
    -0x1.fffffffffffffp1023,
    0x1.0p-1074
  },
  { // Entry 1033
    -0x1.fffffffffffff0p1,
    0x1.0p-1022,
    -0x1.fffffffffffffp1023,
    -0x1.0p-1074
  },
  { // Entry 1034
    -0x1.fffffffffffff0p1,
    0x1.0p-1022,
    -0x1.fffffffffffffp1023,
    0.0
  },
  { // Entry 1035
    -0x1.fffffffffffff0p1,
    0x1.0p-1022,
    -0x1.fffffffffffffp1023,
    -0.0
  },
  { // Entry 1036
    HUGE_VAL,
    0x1.0p-1022,
    0x1.0p-1022,
    HUGE_VAL
  },
  { // Entry 1037
    -HUGE_VAL,
    0x1.0p-1022,
    0x1.0p-1022,
    -HUGE_VAL
  },
  { // Entry 1038
    0x1.fffffffffffff0p1023,
    0x1.0p-1022,
    0x1.0p-1022,
    0x1.fffffffffffffp1023
  },
  { // Entry 1039
    -0x1.ffffffffffffefffffffffffffffffffp1023,
    0x1.0p-1022,
    0x1.0p-1022,
    -0x1.fffffffffffffp1023
  },
  { // Entry 1040
    0x1.p-1022,
    0x1.0p-1022,
    0x1.0p-1022,
    0x1.0p-1022
  },
  { // Entry 1041
    -0x1.ffffffffffffffffffffffffffffffffp-1023,
    0x1.0p-1022,
    0x1.0p-1022,
    -0x1.0p-1022
  },
  { // Entry 1042
    0x1.ffffffffffffe0p-1023,
    0x1.0p-1022,
    0x1.0p-1022,
    0x1.ffffffffffffep-1023
  },
  { // Entry 1043
    -0x1.ffffffffffffdfffffffffffffffffffp-1023,
    0x1.0p-1022,
    0x1.0p-1022,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 1044
    0x1.p-1074,
    0x1.0p-1022,
    0x1.0p-1022,
    0x1.0p-1074
  },
  { // Entry 1045
    -0.0,
    0x1.0p-1022,
    0x1.0p-1022,
    -0x1.0p-1074
  },
  { // Entry 1046
    0.0,
    0x1.0p-1022,
    0x1.0p-1022,
    0.0
  },
  { // Entry 1047
    0.0,
    0x1.0p-1022,
    0x1.0p-1022,
    -0.0
  },
  { // Entry 1048
    HUGE_VAL,
    0x1.0p-1022,
    -0x1.0p-1022,
    HUGE_VAL
  },
  { // Entry 1049
    -HUGE_VAL,
    0x1.0p-1022,
    -0x1.0p-1022,
    -HUGE_VAL
  },
  { // Entry 1050
    0x1.ffffffffffffefffffffffffffffffffp1023,
    0x1.0p-1022,
    -0x1.0p-1022,
    0x1.fffffffffffffp1023
  },
  { // Entry 1051
    -0x1.fffffffffffff0p1023,
    0x1.0p-1022,
    -0x1.0p-1022,
    -0x1.fffffffffffffp1023
  },
  { // Entry 1052
    0x1.ffffffffffffffffffffffffffffffffp-1023,
    0x1.0p-1022,
    -0x1.0p-1022,
    0x1.0p-1022
  },
  { // Entry 1053
    -0x1.p-1022,
    0x1.0p-1022,
    -0x1.0p-1022,
    -0x1.0p-1022
  },
  { // Entry 1054
    0x1.ffffffffffffdfffffffffffffffffffp-1023,
    0x1.0p-1022,
    -0x1.0p-1022,
    0x1.ffffffffffffep-1023
  },
  { // Entry 1055
    -0x1.ffffffffffffe0p-1023,
    0x1.0p-1022,
    -0x1.0p-1022,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 1056
    0.0,
    0x1.0p-1022,
    -0x1.0p-1022,
    0x1.0p-1074
  },
  { // Entry 1057
    -0x1.p-1074,
    0x1.0p-1022,
    -0x1.0p-1022,
    -0x1.0p-1074
  },
  { // Entry 1058
    -0.0,
    0x1.0p-1022,
    -0x1.0p-1022,
    0.0
  },
  { // Entry 1059
    -0.0,
    0x1.0p-1022,
    -0x1.0p-1022,
    -0.0
  },
  { // Entry 1060
    HUGE_VAL,
    0x1.0p-1022,
    0x1.ffffffffffffep-1023,
    HUGE_VAL
  },
  { // Entry 1061
    -HUGE_VAL,
    0x1.0p-1022,
    0x1.ffffffffffffep-1023,
    -HUGE_VAL
  },
  { // Entry 1062
    0x1.fffffffffffff0p1023,
    0x1.0p-1022,
    0x1.ffffffffffffep-1023,
    0x1.fffffffffffffp1023
  },
  { // Entry 1063
    -0x1.ffffffffffffefffffffffffffffffffp1023,
    0x1.0p-1022,
    0x1.ffffffffffffep-1023,
    -0x1.fffffffffffffp1023
  },
  { // Entry 1064
    0x1.p-1022,
    0x1.0p-1022,
    0x1.ffffffffffffep-1023,
    0x1.0p-1022
  },
  { // Entry 1065
    -0x1.ffffffffffffffffffffffffffffffffp-1023,
    0x1.0p-1022,
    0x1.ffffffffffffep-1023,
    -0x1.0p-1022
  },
  { // Entry 1066
    0x1.ffffffffffffe0p-1023,
    0x1.0p-1022,
    0x1.ffffffffffffep-1023,
    0x1.ffffffffffffep-1023
  },
  { // Entry 1067
    -0x1.ffffffffffffdfffffffffffffffffffp-1023,
    0x1.0p-1022,
    0x1.ffffffffffffep-1023,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 1068
    0x1.p-1074,
    0x1.0p-1022,
    0x1.ffffffffffffep-1023,
    0x1.0p-1074
  },
  { // Entry 1069
    -0.0,
    0x1.0p-1022,
    0x1.ffffffffffffep-1023,
    -0x1.0p-1074
  },
  { // Entry 1070
    0.0,
    0x1.0p-1022,
    0x1.ffffffffffffep-1023,
    0.0
  },
  { // Entry 1071
    0.0,
    0x1.0p-1022,
    0x1.ffffffffffffep-1023,
    -0.0
  },
  { // Entry 1072
    HUGE_VAL,
    0x1.0p-1022,
    -0x1.ffffffffffffep-1023,
    HUGE_VAL
  },
  { // Entry 1073
    -HUGE_VAL,
    0x1.0p-1022,
    -0x1.ffffffffffffep-1023,
    -HUGE_VAL
  },
  { // Entry 1074
    0x1.ffffffffffffefffffffffffffffffffp1023,
    0x1.0p-1022,
    -0x1.ffffffffffffep-1023,
    0x1.fffffffffffffp1023
  },
  { // Entry 1075
    -0x1.fffffffffffff0p1023,
    0x1.0p-1022,
    -0x1.ffffffffffffep-1023,
    -0x1.fffffffffffffp1023
  },
  { // Entry 1076
    0x1.ffffffffffffffffffffffffffffffffp-1023,
    0x1.0p-1022,
    -0x1.ffffffffffffep-1023,
    0x1.0p-1022
  },
  { // Entry 1077
    -0x1.p-1022,
    0x1.0p-1022,
    -0x1.ffffffffffffep-1023,
    -0x1.0p-1022
  },
  { // Entry 1078
    0x1.ffffffffffffdfffffffffffffffffffp-1023,
    0x1.0p-1022,
    -0x1.ffffffffffffep-1023,
    0x1.ffffffffffffep-1023
  },
  { // Entry 1079
    -0x1.ffffffffffffe0p-1023,
    0x1.0p-1022,
    -0x1.ffffffffffffep-1023,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 1080
    0.0,
    0x1.0p-1022,
    -0x1.ffffffffffffep-1023,
    0x1.0p-1074
  },
  { // Entry 1081
    -0x1.p-1074,
    0x1.0p-1022,
    -0x1.ffffffffffffep-1023,
    -0x1.0p-1074
  },
  { // Entry 1082
    -0.0,
    0x1.0p-1022,
    -0x1.ffffffffffffep-1023,
    0.0
  },
  { // Entry 1083
    -0.0,
    0x1.0p-1022,
    -0x1.ffffffffffffep-1023,
    -0.0
  },
  { // Entry 1084
    HUGE_VAL,
    0x1.0p-1022,
    0x1.0p-1074,
    HUGE_VAL
  },
  { // Entry 1085
    -HUGE_VAL,
    0x1.0p-1022,
    0x1.0p-1074,
    -HUGE_VAL
  },
  { // Entry 1086
    0x1.fffffffffffff0p1023,
    0x1.0p-1022,
    0x1.0p-1074,
    0x1.fffffffffffffp1023
  },
  { // Entry 1087
    -0x1.ffffffffffffefffffffffffffffffffp1023,
    0x1.0p-1022,
    0x1.0p-1074,
    -0x1.fffffffffffffp1023
  },
  { // Entry 1088
    0x1.p-1022,
    0x1.0p-1022,
    0x1.0p-1074,
    0x1.0p-1022
  },
  { // Entry 1089
    -0x1.ffffffffffffffffffffffffffffffffp-1023,
    0x1.0p-1022,
    0x1.0p-1074,
    -0x1.0p-1022
  },
  { // Entry 1090
    0x1.ffffffffffffe0p-1023,
    0x1.0p-1022,
    0x1.0p-1074,
    0x1.ffffffffffffep-1023
  },
  { // Entry 1091
    -0x1.ffffffffffffdfffffffffffffffffffp-1023,
    0x1.0p-1022,
    0x1.0p-1074,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 1092
    0x1.p-1074,
    0x1.0p-1022,
    0x1.0p-1074,
    0x1.0p-1074
  },
  { // Entry 1093
    -0.0,
    0x1.0p-1022,
    0x1.0p-1074,
    -0x1.0p-1074
  },
  { // Entry 1094
    0.0,
    0x1.0p-1022,
    0x1.0p-1074,
    0.0
  },
  { // Entry 1095
    0.0,
    0x1.0p-1022,
    0x1.0p-1074,
    -0.0
  },
  { // Entry 1096
    HUGE_VAL,
    0x1.0p-1022,
    -0x1.0p-1074,
    HUGE_VAL
  },
  { // Entry 1097
    -HUGE_VAL,
    0x1.0p-1022,
    -0x1.0p-1074,
    -HUGE_VAL
  },
  { // Entry 1098
    0x1.ffffffffffffefffffffffffffffffffp1023,
    0x1.0p-1022,
    -0x1.0p-1074,
    0x1.fffffffffffffp1023
  },
  { // Entry 1099
    -0x1.fffffffffffff0p1023,
    0x1.0p-1022,
    -0x1.0p-1074,
    -0x1.fffffffffffffp1023
  },
  { // Entry 1100
    0x1.ffffffffffffffffffffffffffffffffp-1023,
    0x1.0p-1022,
    -0x1.0p-1074,
    0x1.0p-1022
  },
  { // Entry 1101
    -0x1.p-1022,
    0x1.0p-1022,
    -0x1.0p-1074,
    -0x1.0p-1022
  },
  { // Entry 1102
    0x1.ffffffffffffdfffffffffffffffffffp-1023,
    0x1.0p-1022,
    -0x1.0p-1074,
    0x1.ffffffffffffep-1023
  },
  { // Entry 1103
    -0x1.ffffffffffffe0p-1023,
    0x1.0p-1022,
    -0x1.0p-1074,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 1104
    0.0,
    0x1.0p-1022,
    -0x1.0p-1074,
    0x1.0p-1074
  },
  { // Entry 1105
    -0x1.p-1074,
    0x1.0p-1022,
    -0x1.0p-1074,
    -0x1.0p-1074
  },
  { // Entry 1106
    -0.0,
    0x1.0p-1022,
    -0x1.0p-1074,
    0.0
  },
  { // Entry 1107
    -0.0,
    0x1.0p-1022,
    -0x1.0p-1074,
    -0.0
  },
  { // Entry 1108
    HUGE_VAL,
    0x1.0p-1022,
    0.0,
    HUGE_VAL
  },
  { // Entry 1109
    -HUGE_VAL,
    0x1.0p-1022,
    0.0,
    -HUGE_VAL
  },
  { // Entry 1110
    0x1.fffffffffffff0p1023,
    0x1.0p-1022,
    0.0,
    0x1.fffffffffffffp1023
  },
  { // Entry 1111
    -0x1.fffffffffffff0p1023,
    0x1.0p-1022,
    0.0,
    -0x1.fffffffffffffp1023
  },
  { // Entry 1112
    0x1.p-1022,
    0x1.0p-1022,
    0.0,
    0x1.0p-1022
  },
  { // Entry 1113
    -0x1.p-1022,
    0x1.0p-1022,
    0.0,
    -0x1.0p-1022
  },
  { // Entry 1114
    0x1.ffffffffffffe0p-1023,
    0x1.0p-1022,
    0.0,
    0x1.ffffffffffffep-1023
  },
  { // Entry 1115
    -0x1.ffffffffffffe0p-1023,
    0x1.0p-1022,
    0.0,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 1116
    0x1.p-1074,
    0x1.0p-1022,
    0.0,
    0x1.0p-1074
  },
  { // Entry 1117
    -0x1.p-1074,
    0x1.0p-1022,
    0.0,
    -0x1.0p-1074
  },
  { // Entry 1118
    0.0,
    0x1.0p-1022,
    0.0,
    0.0
  },
  { // Entry 1119
    0.0,
    0x1.0p-1022,
    0.0,
    -0.0
  },
  { // Entry 1120
    HUGE_VAL,
    0x1.0p-1022,
    -0.0,
    HUGE_VAL
  },
  { // Entry 1121
    -HUGE_VAL,
    0x1.0p-1022,
    -0.0,
    -HUGE_VAL
  },
  { // Entry 1122
    0x1.fffffffffffff0p1023,
    0x1.0p-1022,
    -0.0,
    0x1.fffffffffffffp1023
  },
  { // Entry 1123
    -0x1.fffffffffffff0p1023,
    0x1.0p-1022,
    -0.0,
    -0x1.fffffffffffffp1023
  },
  { // Entry 1124
    0x1.p-1022,
    0x1.0p-1022,
    -0.0,
    0x1.0p-1022
  },
  { // Entry 1125
    -0x1.p-1022,
    0x1.0p-1022,
    -0.0,
    -0x1.0p-1022
  },
  { // Entry 1126
    0x1.ffffffffffffe0p-1023,
    0x1.0p-1022,
    -0.0,
    0x1.ffffffffffffep-1023
  },
  { // Entry 1127
    -0x1.ffffffffffffe0p-1023,
    0x1.0p-1022,
    -0.0,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 1128
    0x1.p-1074,
    0x1.0p-1022,
    -0.0,
    0x1.0p-1074
  },
  { // Entry 1129
    -0x1.p-1074,
    0x1.0p-1022,
    -0.0,
    -0x1.0p-1074
  },
  { // Entry 1130
    0.0,
    0x1.0p-1022,
    -0.0,
    0.0
  },
  { // Entry 1131
    -0.0,
    0x1.0p-1022,
    -0.0,
    -0.0
  },
  { // Entry 1132
    -HUGE_VAL,
    -0x1.0p-1022,
    HUGE_VAL,
    -HUGE_VAL
  },
  { // Entry 1133
    -HUGE_VAL,
    -0x1.0p-1022,
    HUGE_VAL,
    0x1.fffffffffffffp1023
  },
  { // Entry 1134
    -HUGE_VAL,
    -0x1.0p-1022,
    HUGE_VAL,
    -0x1.fffffffffffffp1023
  },
  { // Entry 1135
    -HUGE_VAL,
    -0x1.0p-1022,
    HUGE_VAL,
    0x1.0p-1022
  },
  { // Entry 1136
    -HUGE_VAL,
    -0x1.0p-1022,
    HUGE_VAL,
    -0x1.0p-1022
  },
  { // Entry 1137
    -HUGE_VAL,
    -0x1.0p-1022,
    HUGE_VAL,
    0x1.ffffffffffffep-1023
  },
  { // Entry 1138
    -HUGE_VAL,
    -0x1.0p-1022,
    HUGE_VAL,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 1139
    -HUGE_VAL,
    -0x1.0p-1022,
    HUGE_VAL,
    0x1.0p-1074
  },
  { // Entry 1140
    -HUGE_VAL,
    -0x1.0p-1022,
    HUGE_VAL,
    -0x1.0p-1074
  },
  { // Entry 1141
    -HUGE_VAL,
    -0x1.0p-1022,
    HUGE_VAL,
    0.0
  },
  { // Entry 1142
    -HUGE_VAL,
    -0x1.0p-1022,
    HUGE_VAL,
    -0.0
  },
  { // Entry 1143
    HUGE_VAL,
    -0x1.0p-1022,
    -HUGE_VAL,
    HUGE_VAL
  },
  { // Entry 1144
    HUGE_VAL,
    -0x1.0p-1022,
    -HUGE_VAL,
    0x1.fffffffffffffp1023
  },
  { // Entry 1145
    HUGE_VAL,
    -0x1.0p-1022,
    -HUGE_VAL,
    -0x1.fffffffffffffp1023
  },
  { // Entry 1146
    HUGE_VAL,
    -0x1.0p-1022,
    -HUGE_VAL,
    0x1.0p-1022
  },
  { // Entry 1147
    HUGE_VAL,
    -0x1.0p-1022,
    -HUGE_VAL,
    -0x1.0p-1022
  },
  { // Entry 1148
    HUGE_VAL,
    -0x1.0p-1022,
    -HUGE_VAL,
    0x1.ffffffffffffep-1023
  },
  { // Entry 1149
    HUGE_VAL,
    -0x1.0p-1022,
    -HUGE_VAL,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 1150
    HUGE_VAL,
    -0x1.0p-1022,
    -HUGE_VAL,
    0x1.0p-1074
  },
  { // Entry 1151
    HUGE_VAL,
    -0x1.0p-1022,
    -HUGE_VAL,
    -0x1.0p-1074
  },
  { // Entry 1152
    HUGE_VAL,
    -0x1.0p-1022,
    -HUGE_VAL,
    0.0
  },
  { // Entry 1153
    HUGE_VAL,
    -0x1.0p-1022,
    -HUGE_VAL,
    -0.0
  },
  { // Entry 1154
    HUGE_VAL,
    -0x1.0p-1022,
    0x1.fffffffffffffp1023,
    HUGE_VAL
  },
  { // Entry 1155
    -HUGE_VAL,
    -0x1.0p-1022,
    0x1.fffffffffffffp1023,
    -HUGE_VAL
  },
  { // Entry 1156
    0x1.ffffffffffffefffffffffffffffffffp1023,
    -0x1.0p-1022,
    0x1.fffffffffffffp1023,
    0x1.fffffffffffffp1023
  },
  { // Entry 1157
    -0x1.fffffffffffff0p1023,
    -0x1.0p-1022,
    0x1.fffffffffffffp1023,
    -0x1.fffffffffffffp1023
  },
  { // Entry 1158
    -0x1.ffffffffffffefffffffffffffffffffp1,
    -0x1.0p-1022,
    0x1.fffffffffffffp1023,
    0x1.0p-1022
  },
  { // Entry 1159
    -0x1.fffffffffffff0p1,
    -0x1.0p-1022,
    0x1.fffffffffffffp1023,
    -0x1.0p-1022
  },
  { // Entry 1160
    -0x1.ffffffffffffefffffffffffffffffffp1,
    -0x1.0p-1022,
    0x1.fffffffffffffp1023,
    0x1.ffffffffffffep-1023
  },
  { // Entry 1161
    -0x1.fffffffffffff0p1,
    -0x1.0p-1022,
    0x1.fffffffffffffp1023,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 1162
    -0x1.ffffffffffffefffffffffffffffffffp1,
    -0x1.0p-1022,
    0x1.fffffffffffffp1023,
    0x1.0p-1074
  },
  { // Entry 1163
    -0x1.fffffffffffff0p1,
    -0x1.0p-1022,
    0x1.fffffffffffffp1023,
    -0x1.0p-1074
  },
  { // Entry 1164
    -0x1.fffffffffffff0p1,
    -0x1.0p-1022,
    0x1.fffffffffffffp1023,
    0.0
  },
  { // Entry 1165
    -0x1.fffffffffffff0p1,
    -0x1.0p-1022,
    0x1.fffffffffffffp1023,
    -0.0
  },
  { // Entry 1166
    HUGE_VAL,
    -0x1.0p-1022,
    -0x1.fffffffffffffp1023,
    HUGE_VAL
  },
  { // Entry 1167
    -HUGE_VAL,
    -0x1.0p-1022,
    -0x1.fffffffffffffp1023,
    -HUGE_VAL
  },
  { // Entry 1168
    0x1.fffffffffffff0p1023,
    -0x1.0p-1022,
    -0x1.fffffffffffffp1023,
    0x1.fffffffffffffp1023
  },
  { // Entry 1169
    -0x1.ffffffffffffefffffffffffffffffffp1023,
    -0x1.0p-1022,
    -0x1.fffffffffffffp1023,
    -0x1.fffffffffffffp1023
  },
  { // Entry 1170
    0x1.fffffffffffff0p1,
    -0x1.0p-1022,
    -0x1.fffffffffffffp1023,
    0x1.0p-1022
  },
  { // Entry 1171
    0x1.ffffffffffffefffffffffffffffffffp1,
    -0x1.0p-1022,
    -0x1.fffffffffffffp1023,
    -0x1.0p-1022
  },
  { // Entry 1172
    0x1.fffffffffffff0p1,
    -0x1.0p-1022,
    -0x1.fffffffffffffp1023,
    0x1.ffffffffffffep-1023
  },
  { // Entry 1173
    0x1.ffffffffffffefffffffffffffffffffp1,
    -0x1.0p-1022,
    -0x1.fffffffffffffp1023,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 1174
    0x1.fffffffffffff0p1,
    -0x1.0p-1022,
    -0x1.fffffffffffffp1023,
    0x1.0p-1074
  },
  { // Entry 1175
    0x1.ffffffffffffefffffffffffffffffffp1,
    -0x1.0p-1022,
    -0x1.fffffffffffffp1023,
    -0x1.0p-1074
  },
  { // Entry 1176
    0x1.fffffffffffff0p1,
    -0x1.0p-1022,
    -0x1.fffffffffffffp1023,
    0.0
  },
  { // Entry 1177
    0x1.fffffffffffff0p1,
    -0x1.0p-1022,
    -0x1.fffffffffffffp1023,
    -0.0
  },
  { // Entry 1178
    HUGE_VAL,
    -0x1.0p-1022,
    0x1.0p-1022,
    HUGE_VAL
  },
  { // Entry 1179
    -HUGE_VAL,
    -0x1.0p-1022,
    0x1.0p-1022,
    -HUGE_VAL
  },
  { // Entry 1180
    0x1.ffffffffffffefffffffffffffffffffp1023,
    -0x1.0p-1022,
    0x1.0p-1022,
    0x1.fffffffffffffp1023
  },
  { // Entry 1181
    -0x1.fffffffffffff0p1023,
    -0x1.0p-1022,
    0x1.0p-1022,
    -0x1.fffffffffffffp1023
  },
  { // Entry 1182
    0x1.ffffffffffffffffffffffffffffffffp-1023,
    -0x1.0p-1022,
    0x1.0p-1022,
    0x1.0p-1022
  },
  { // Entry 1183
    -0x1.p-1022,
    -0x1.0p-1022,
    0x1.0p-1022,
    -0x1.0p-1022
  },
  { // Entry 1184
    0x1.ffffffffffffdfffffffffffffffffffp-1023,
    -0x1.0p-1022,
    0x1.0p-1022,
    0x1.ffffffffffffep-1023
  },
  { // Entry 1185
    -0x1.ffffffffffffe0p-1023,
    -0x1.0p-1022,
    0x1.0p-1022,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 1186
    0.0,
    -0x1.0p-1022,
    0x1.0p-1022,
    0x1.0p-1074
  },
  { // Entry 1187
    -0x1.p-1074,
    -0x1.0p-1022,
    0x1.0p-1022,
    -0x1.0p-1074
  },
  { // Entry 1188
    -0.0,
    -0x1.0p-1022,
    0x1.0p-1022,
    0.0
  },
  { // Entry 1189
    -0.0,
    -0x1.0p-1022,
    0x1.0p-1022,
    -0.0
  },
  { // Entry 1190
    HUGE_VAL,
    -0x1.0p-1022,
    -0x1.0p-1022,
    HUGE_VAL
  },
  { // Entry 1191
    -HUGE_VAL,
    -0x1.0p-1022,
    -0x1.0p-1022,
    -HUGE_VAL
  },
  { // Entry 1192
    0x1.fffffffffffff0p1023,
    -0x1.0p-1022,
    -0x1.0p-1022,
    0x1.fffffffffffffp1023
  },
  { // Entry 1193
    -0x1.ffffffffffffefffffffffffffffffffp1023,
    -0x1.0p-1022,
    -0x1.0p-1022,
    -0x1.fffffffffffffp1023
  },
  { // Entry 1194
    0x1.p-1022,
    -0x1.0p-1022,
    -0x1.0p-1022,
    0x1.0p-1022
  },
  { // Entry 1195
    -0x1.ffffffffffffffffffffffffffffffffp-1023,
    -0x1.0p-1022,
    -0x1.0p-1022,
    -0x1.0p-1022
  },
  { // Entry 1196
    0x1.ffffffffffffe0p-1023,
    -0x1.0p-1022,
    -0x1.0p-1022,
    0x1.ffffffffffffep-1023
  },
  { // Entry 1197
    -0x1.ffffffffffffdfffffffffffffffffffp-1023,
    -0x1.0p-1022,
    -0x1.0p-1022,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 1198
    0x1.p-1074,
    -0x1.0p-1022,
    -0x1.0p-1022,
    0x1.0p-1074
  },
  { // Entry 1199
    -0.0,
    -0x1.0p-1022,
    -0x1.0p-1022,
    -0x1.0p-1074
  },
  { // Entry 1200
    0.0,
    -0x1.0p-1022,
    -0x1.0p-1022,
    0.0
  },
  { // Entry 1201
    0.0,
    -0x1.0p-1022,
    -0x1.0p-1022,
    -0.0
  },
  { // Entry 1202
    HUGE_VAL,
    -0x1.0p-1022,
    0x1.ffffffffffffep-1023,
    HUGE_VAL
  },
  { // Entry 1203
    -HUGE_VAL,
    -0x1.0p-1022,
    0x1.ffffffffffffep-1023,
    -HUGE_VAL
  },
  { // Entry 1204
    0x1.ffffffffffffefffffffffffffffffffp1023,
    -0x1.0p-1022,
    0x1.ffffffffffffep-1023,
    0x1.fffffffffffffp1023
  },
  { // Entry 1205
    -0x1.fffffffffffff0p1023,
    -0x1.0p-1022,
    0x1.ffffffffffffep-1023,
    -0x1.fffffffffffffp1023
  },
  { // Entry 1206
    0x1.ffffffffffffffffffffffffffffffffp-1023,
    -0x1.0p-1022,
    0x1.ffffffffffffep-1023,
    0x1.0p-1022
  },
  { // Entry 1207
    -0x1.p-1022,
    -0x1.0p-1022,
    0x1.ffffffffffffep-1023,
    -0x1.0p-1022
  },
  { // Entry 1208
    0x1.ffffffffffffdfffffffffffffffffffp-1023,
    -0x1.0p-1022,
    0x1.ffffffffffffep-1023,
    0x1.ffffffffffffep-1023
  },
  { // Entry 1209
    -0x1.ffffffffffffe0p-1023,
    -0x1.0p-1022,
    0x1.ffffffffffffep-1023,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 1210
    0.0,
    -0x1.0p-1022,
    0x1.ffffffffffffep-1023,
    0x1.0p-1074
  },
  { // Entry 1211
    -0x1.p-1074,
    -0x1.0p-1022,
    0x1.ffffffffffffep-1023,
    -0x1.0p-1074
  },
  { // Entry 1212
    -0.0,
    -0x1.0p-1022,
    0x1.ffffffffffffep-1023,
    0.0
  },
  { // Entry 1213
    -0.0,
    -0x1.0p-1022,
    0x1.ffffffffffffep-1023,
    -0.0
  },
  { // Entry 1214
    HUGE_VAL,
    -0x1.0p-1022,
    -0x1.ffffffffffffep-1023,
    HUGE_VAL
  },
  { // Entry 1215
    -HUGE_VAL,
    -0x1.0p-1022,
    -0x1.ffffffffffffep-1023,
    -HUGE_VAL
  },
  { // Entry 1216
    0x1.fffffffffffff0p1023,
    -0x1.0p-1022,
    -0x1.ffffffffffffep-1023,
    0x1.fffffffffffffp1023
  },
  { // Entry 1217
    -0x1.ffffffffffffefffffffffffffffffffp1023,
    -0x1.0p-1022,
    -0x1.ffffffffffffep-1023,
    -0x1.fffffffffffffp1023
  },
  { // Entry 1218
    0x1.p-1022,
    -0x1.0p-1022,
    -0x1.ffffffffffffep-1023,
    0x1.0p-1022
  },
  { // Entry 1219
    -0x1.ffffffffffffffffffffffffffffffffp-1023,
    -0x1.0p-1022,
    -0x1.ffffffffffffep-1023,
    -0x1.0p-1022
  },
  { // Entry 1220
    0x1.ffffffffffffe0p-1023,
    -0x1.0p-1022,
    -0x1.ffffffffffffep-1023,
    0x1.ffffffffffffep-1023
  },
  { // Entry 1221
    -0x1.ffffffffffffdfffffffffffffffffffp-1023,
    -0x1.0p-1022,
    -0x1.ffffffffffffep-1023,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 1222
    0x1.p-1074,
    -0x1.0p-1022,
    -0x1.ffffffffffffep-1023,
    0x1.0p-1074
  },
  { // Entry 1223
    -0.0,
    -0x1.0p-1022,
    -0x1.ffffffffffffep-1023,
    -0x1.0p-1074
  },
  { // Entry 1224
    0.0,
    -0x1.0p-1022,
    -0x1.ffffffffffffep-1023,
    0.0
  },
  { // Entry 1225
    0.0,
    -0x1.0p-1022,
    -0x1.ffffffffffffep-1023,
    -0.0
  },
  { // Entry 1226
    HUGE_VAL,
    -0x1.0p-1022,
    0x1.0p-1074,
    HUGE_VAL
  },
  { // Entry 1227
    -HUGE_VAL,
    -0x1.0p-1022,
    0x1.0p-1074,
    -HUGE_VAL
  },
  { // Entry 1228
    0x1.ffffffffffffefffffffffffffffffffp1023,
    -0x1.0p-1022,
    0x1.0p-1074,
    0x1.fffffffffffffp1023
  },
  { // Entry 1229
    -0x1.fffffffffffff0p1023,
    -0x1.0p-1022,
    0x1.0p-1074,
    -0x1.fffffffffffffp1023
  },
  { // Entry 1230
    0x1.ffffffffffffffffffffffffffffffffp-1023,
    -0x1.0p-1022,
    0x1.0p-1074,
    0x1.0p-1022
  },
  { // Entry 1231
    -0x1.p-1022,
    -0x1.0p-1022,
    0x1.0p-1074,
    -0x1.0p-1022
  },
  { // Entry 1232
    0x1.ffffffffffffdfffffffffffffffffffp-1023,
    -0x1.0p-1022,
    0x1.0p-1074,
    0x1.ffffffffffffep-1023
  },
  { // Entry 1233
    -0x1.ffffffffffffe0p-1023,
    -0x1.0p-1022,
    0x1.0p-1074,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 1234
    0.0,
    -0x1.0p-1022,
    0x1.0p-1074,
    0x1.0p-1074
  },
  { // Entry 1235
    -0x1.p-1074,
    -0x1.0p-1022,
    0x1.0p-1074,
    -0x1.0p-1074
  },
  { // Entry 1236
    -0.0,
    -0x1.0p-1022,
    0x1.0p-1074,
    0.0
  },
  { // Entry 1237
    -0.0,
    -0x1.0p-1022,
    0x1.0p-1074,
    -0.0
  },
  { // Entry 1238
    HUGE_VAL,
    -0x1.0p-1022,
    -0x1.0p-1074,
    HUGE_VAL
  },
  { // Entry 1239
    -HUGE_VAL,
    -0x1.0p-1022,
    -0x1.0p-1074,
    -HUGE_VAL
  },
  { // Entry 1240
    0x1.fffffffffffff0p1023,
    -0x1.0p-1022,
    -0x1.0p-1074,
    0x1.fffffffffffffp1023
  },
  { // Entry 1241
    -0x1.ffffffffffffefffffffffffffffffffp1023,
    -0x1.0p-1022,
    -0x1.0p-1074,
    -0x1.fffffffffffffp1023
  },
  { // Entry 1242
    0x1.p-1022,
    -0x1.0p-1022,
    -0x1.0p-1074,
    0x1.0p-1022
  },
  { // Entry 1243
    -0x1.ffffffffffffffffffffffffffffffffp-1023,
    -0x1.0p-1022,
    -0x1.0p-1074,
    -0x1.0p-1022
  },
  { // Entry 1244
    0x1.ffffffffffffe0p-1023,
    -0x1.0p-1022,
    -0x1.0p-1074,
    0x1.ffffffffffffep-1023
  },
  { // Entry 1245
    -0x1.ffffffffffffdfffffffffffffffffffp-1023,
    -0x1.0p-1022,
    -0x1.0p-1074,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 1246
    0x1.p-1074,
    -0x1.0p-1022,
    -0x1.0p-1074,
    0x1.0p-1074
  },
  { // Entry 1247
    -0.0,
    -0x1.0p-1022,
    -0x1.0p-1074,
    -0x1.0p-1074
  },
  { // Entry 1248
    0.0,
    -0x1.0p-1022,
    -0x1.0p-1074,
    0.0
  },
  { // Entry 1249
    0.0,
    -0x1.0p-1022,
    -0x1.0p-1074,
    -0.0
  },
  { // Entry 1250
    HUGE_VAL,
    -0x1.0p-1022,
    0.0,
    HUGE_VAL
  },
  { // Entry 1251
    -HUGE_VAL,
    -0x1.0p-1022,
    0.0,
    -HUGE_VAL
  },
  { // Entry 1252
    0x1.fffffffffffff0p1023,
    -0x1.0p-1022,
    0.0,
    0x1.fffffffffffffp1023
  },
  { // Entry 1253
    -0x1.fffffffffffff0p1023,
    -0x1.0p-1022,
    0.0,
    -0x1.fffffffffffffp1023
  },
  { // Entry 1254
    0x1.p-1022,
    -0x1.0p-1022,
    0.0,
    0x1.0p-1022
  },
  { // Entry 1255
    -0x1.p-1022,
    -0x1.0p-1022,
    0.0,
    -0x1.0p-1022
  },
  { // Entry 1256
    0x1.ffffffffffffe0p-1023,
    -0x1.0p-1022,
    0.0,
    0x1.ffffffffffffep-1023
  },
  { // Entry 1257
    -0x1.ffffffffffffe0p-1023,
    -0x1.0p-1022,
    0.0,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 1258
    0x1.p-1074,
    -0x1.0p-1022,
    0.0,
    0x1.0p-1074
  },
  { // Entry 1259
    -0x1.p-1074,
    -0x1.0p-1022,
    0.0,
    -0x1.0p-1074
  },
  { // Entry 1260
    0.0,
    -0x1.0p-1022,
    0.0,
    0.0
  },
  { // Entry 1261
    -0.0,
    -0x1.0p-1022,
    0.0,
    -0.0
  },
  { // Entry 1262
    HUGE_VAL,
    -0x1.0p-1022,
    -0.0,
    HUGE_VAL
  },
  { // Entry 1263
    -HUGE_VAL,
    -0x1.0p-1022,
    -0.0,
    -HUGE_VAL
  },
  { // Entry 1264
    0x1.fffffffffffff0p1023,
    -0x1.0p-1022,
    -0.0,
    0x1.fffffffffffffp1023
  },
  { // Entry 1265
    -0x1.fffffffffffff0p1023,
    -0x1.0p-1022,
    -0.0,
    -0x1.fffffffffffffp1023
  },
  { // Entry 1266
    0x1.p-1022,
    -0x1.0p-1022,
    -0.0,
    0x1.0p-1022
  },
  { // Entry 1267
    -0x1.p-1022,
    -0x1.0p-1022,
    -0.0,
    -0x1.0p-1022
  },
  { // Entry 1268
    0x1.ffffffffffffe0p-1023,
    -0x1.0p-1022,
    -0.0,
    0x1.ffffffffffffep-1023
  },
  { // Entry 1269
    -0x1.ffffffffffffe0p-1023,
    -0x1.0p-1022,
    -0.0,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 1270
    0x1.p-1074,
    -0x1.0p-1022,
    -0.0,
    0x1.0p-1074
  },
  { // Entry 1271
    -0x1.p-1074,
    -0x1.0p-1022,
    -0.0,
    -0x1.0p-1074
  },
  { // Entry 1272
    0.0,
    -0x1.0p-1022,
    -0.0,
    0.0
  },
  { // Entry 1273
    0.0,
    -0x1.0p-1022,
    -0.0,
    -0.0
  },
  { // Entry 1274
    HUGE_VAL,
    0x1.ffffffffffffep-1023,
    HUGE_VAL,
    HUGE_VAL
  },
  { // Entry 1275
    HUGE_VAL,
    0x1.ffffffffffffep-1023,
    HUGE_VAL,
    0x1.fffffffffffffp1023
  },
  { // Entry 1276
    HUGE_VAL,
    0x1.ffffffffffffep-1023,
    HUGE_VAL,
    -0x1.fffffffffffffp1023
  },
  { // Entry 1277
    HUGE_VAL,
    0x1.ffffffffffffep-1023,
    HUGE_VAL,
    0x1.0p-1022
  },
  { // Entry 1278
    HUGE_VAL,
    0x1.ffffffffffffep-1023,
    HUGE_VAL,
    -0x1.0p-1022
  },
  { // Entry 1279
    HUGE_VAL,
    0x1.ffffffffffffep-1023,
    HUGE_VAL,
    0x1.ffffffffffffep-1023
  },
  { // Entry 1280
    HUGE_VAL,
    0x1.ffffffffffffep-1023,
    HUGE_VAL,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 1281
    HUGE_VAL,
    0x1.ffffffffffffep-1023,
    HUGE_VAL,
    0x1.0p-1074
  },
  { // Entry 1282
    HUGE_VAL,
    0x1.ffffffffffffep-1023,
    HUGE_VAL,
    -0x1.0p-1074
  },
  { // Entry 1283
    HUGE_VAL,
    0x1.ffffffffffffep-1023,
    HUGE_VAL,
    0.0
  },
  { // Entry 1284
    HUGE_VAL,
    0x1.ffffffffffffep-1023,
    HUGE_VAL,
    -0.0
  },
  { // Entry 1285
    -HUGE_VAL,
    0x1.ffffffffffffep-1023,
    -HUGE_VAL,
    -HUGE_VAL
  },
  { // Entry 1286
    -HUGE_VAL,
    0x1.ffffffffffffep-1023,
    -HUGE_VAL,
    0x1.fffffffffffffp1023
  },
  { // Entry 1287
    -HUGE_VAL,
    0x1.ffffffffffffep-1023,
    -HUGE_VAL,
    -0x1.fffffffffffffp1023
  },
  { // Entry 1288
    -HUGE_VAL,
    0x1.ffffffffffffep-1023,
    -HUGE_VAL,
    0x1.0p-1022
  },
  { // Entry 1289
    -HUGE_VAL,
    0x1.ffffffffffffep-1023,
    -HUGE_VAL,
    -0x1.0p-1022
  },
  { // Entry 1290
    -HUGE_VAL,
    0x1.ffffffffffffep-1023,
    -HUGE_VAL,
    0x1.ffffffffffffep-1023
  },
  { // Entry 1291
    -HUGE_VAL,
    0x1.ffffffffffffep-1023,
    -HUGE_VAL,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 1292
    -HUGE_VAL,
    0x1.ffffffffffffep-1023,
    -HUGE_VAL,
    0x1.0p-1074
  },
  { // Entry 1293
    -HUGE_VAL,
    0x1.ffffffffffffep-1023,
    -HUGE_VAL,
    -0x1.0p-1074
  },
  { // Entry 1294
    -HUGE_VAL,
    0x1.ffffffffffffep-1023,
    -HUGE_VAL,
    0.0
  },
  { // Entry 1295
    -HUGE_VAL,
    0x1.ffffffffffffep-1023,
    -HUGE_VAL,
    -0.0
  },
  { // Entry 1296
    HUGE_VAL,
    0x1.ffffffffffffep-1023,
    0x1.fffffffffffffp1023,
    HUGE_VAL
  },
  { // Entry 1297
    -HUGE_VAL,
    0x1.ffffffffffffep-1023,
    0x1.fffffffffffffp1023,
    -HUGE_VAL
  },
  { // Entry 1298
    0x1.fffffffffffff0p1023,
    0x1.ffffffffffffep-1023,
    0x1.fffffffffffffp1023,
    0x1.fffffffffffffp1023
  },
  { // Entry 1299
    -0x1.ffffffffffffefffffffffffffffffffp1023,
    0x1.ffffffffffffep-1023,
    0x1.fffffffffffffp1023,
    -0x1.fffffffffffffp1023
  },
  { // Entry 1300
    0x1.ffffffffffffd0000000000001p1,
    0x1.ffffffffffffep-1023,
    0x1.fffffffffffffp1023,
    0x1.0p-1022
  },
  { // Entry 1301
    0x1.ffffffffffffd0000000000000ffffffp1,
    0x1.ffffffffffffep-1023,
    0x1.fffffffffffffp1023,
    -0x1.0p-1022
  },
  { // Entry 1302
    0x1.ffffffffffffd0000000000001p1,
    0x1.ffffffffffffep-1023,
    0x1.fffffffffffffp1023,
    0x1.ffffffffffffep-1023
  },
  { // Entry 1303
    0x1.ffffffffffffd0000000000000ffffffp1,
    0x1.ffffffffffffep-1023,
    0x1.fffffffffffffp1023,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 1304
    0x1.ffffffffffffd0000000000001p1,
    0x1.ffffffffffffep-1023,
    0x1.fffffffffffffp1023,
    0x1.0p-1074
  },
  { // Entry 1305
    0x1.ffffffffffffd0000000000000ffffffp1,
    0x1.ffffffffffffep-1023,
    0x1.fffffffffffffp1023,
    -0x1.0p-1074
  },
  { // Entry 1306
    0x1.ffffffffffffd0000000000001p1,
    0x1.ffffffffffffep-1023,
    0x1.fffffffffffffp1023,
    0.0
  },
  { // Entry 1307
    0x1.ffffffffffffd0000000000001p1,
    0x1.ffffffffffffep-1023,
    0x1.fffffffffffffp1023,
    -0.0
  },
  { // Entry 1308
    HUGE_VAL,
    0x1.ffffffffffffep-1023,
    -0x1.fffffffffffffp1023,
    HUGE_VAL
  },
  { // Entry 1309
    -HUGE_VAL,
    0x1.ffffffffffffep-1023,
    -0x1.fffffffffffffp1023,
    -HUGE_VAL
  },
  { // Entry 1310
    0x1.ffffffffffffefffffffffffffffffffp1023,
    0x1.ffffffffffffep-1023,
    -0x1.fffffffffffffp1023,
    0x1.fffffffffffffp1023
  },
  { // Entry 1311
    -0x1.fffffffffffff0p1023,
    0x1.ffffffffffffep-1023,
    -0x1.fffffffffffffp1023,
    -0x1.fffffffffffffp1023
  },
  { // Entry 1312
    -0x1.ffffffffffffd0000000000000ffffffp1,
    0x1.ffffffffffffep-1023,
    -0x1.fffffffffffffp1023,
    0x1.0p-1022
  },
  { // Entry 1313
    -0x1.ffffffffffffd0000000000001p1,
    0x1.ffffffffffffep-1023,
    -0x1.fffffffffffffp1023,
    -0x1.0p-1022
  },
  { // Entry 1314
    -0x1.ffffffffffffd0000000000000ffffffp1,
    0x1.ffffffffffffep-1023,
    -0x1.fffffffffffffp1023,
    0x1.ffffffffffffep-1023
  },
  { // Entry 1315
    -0x1.ffffffffffffd0000000000001p1,
    0x1.ffffffffffffep-1023,
    -0x1.fffffffffffffp1023,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 1316
    -0x1.ffffffffffffd0000000000000ffffffp1,
    0x1.ffffffffffffep-1023,
    -0x1.fffffffffffffp1023,
    0x1.0p-1074
  },
  { // Entry 1317
    -0x1.ffffffffffffd0000000000001p1,
    0x1.ffffffffffffep-1023,
    -0x1.fffffffffffffp1023,
    -0x1.0p-1074
  },
  { // Entry 1318
    -0x1.ffffffffffffd0000000000001p1,
    0x1.ffffffffffffep-1023,
    -0x1.fffffffffffffp1023,
    0.0
  },
  { // Entry 1319
    -0x1.ffffffffffffd0000000000001p1,
    0x1.ffffffffffffep-1023,
    -0x1.fffffffffffffp1023,
    -0.0
  },
  { // Entry 1320
    HUGE_VAL,
    0x1.ffffffffffffep-1023,
    0x1.0p-1022,
    HUGE_VAL
  },
  { // Entry 1321
    -HUGE_VAL,
    0x1.ffffffffffffep-1023,
    0x1.0p-1022,
    -HUGE_VAL
  },
  { // Entry 1322
    0x1.fffffffffffff0p1023,
    0x1.ffffffffffffep-1023,
    0x1.0p-1022,
    0x1.fffffffffffffp1023
  },
  { // Entry 1323
    -0x1.ffffffffffffefffffffffffffffffffp1023,
    0x1.ffffffffffffep-1023,
    0x1.0p-1022,
    -0x1.fffffffffffffp1023
  },
  { // Entry 1324
    0x1.p-1022,
    0x1.ffffffffffffep-1023,
    0x1.0p-1022,
    0x1.0p-1022
  },
  { // Entry 1325
    -0x1.ffffffffffffffffffffffffffffffffp-1023,
    0x1.ffffffffffffep-1023,
    0x1.0p-1022,
    -0x1.0p-1022
  },
  { // Entry 1326
    0x1.ffffffffffffe0p-1023,
    0x1.ffffffffffffep-1023,
    0x1.0p-1022,
    0x1.ffffffffffffep-1023
  },
  { // Entry 1327
    -0x1.ffffffffffffdfffffffffffffffffffp-1023,
    0x1.ffffffffffffep-1023,
    0x1.0p-1022,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 1328
    0x1.p-1074,
    0x1.ffffffffffffep-1023,
    0x1.0p-1022,
    0x1.0p-1074
  },
  { // Entry 1329
    -0.0,
    0x1.ffffffffffffep-1023,
    0x1.0p-1022,
    -0x1.0p-1074
  },
  { // Entry 1330
    0.0,
    0x1.ffffffffffffep-1023,
    0x1.0p-1022,
    0.0
  },
  { // Entry 1331
    0.0,
    0x1.ffffffffffffep-1023,
    0x1.0p-1022,
    -0.0
  },
  { // Entry 1332
    HUGE_VAL,
    0x1.ffffffffffffep-1023,
    -0x1.0p-1022,
    HUGE_VAL
  },
  { // Entry 1333
    -HUGE_VAL,
    0x1.ffffffffffffep-1023,
    -0x1.0p-1022,
    -HUGE_VAL
  },
  { // Entry 1334
    0x1.ffffffffffffefffffffffffffffffffp1023,
    0x1.ffffffffffffep-1023,
    -0x1.0p-1022,
    0x1.fffffffffffffp1023
  },
  { // Entry 1335
    -0x1.fffffffffffff0p1023,
    0x1.ffffffffffffep-1023,
    -0x1.0p-1022,
    -0x1.fffffffffffffp1023
  },
  { // Entry 1336
    0x1.ffffffffffffffffffffffffffffffffp-1023,
    0x1.ffffffffffffep-1023,
    -0x1.0p-1022,
    0x1.0p-1022
  },
  { // Entry 1337
    -0x1.p-1022,
    0x1.ffffffffffffep-1023,
    -0x1.0p-1022,
    -0x1.0p-1022
  },
  { // Entry 1338
    0x1.ffffffffffffdfffffffffffffffffffp-1023,
    0x1.ffffffffffffep-1023,
    -0x1.0p-1022,
    0x1.ffffffffffffep-1023
  },
  { // Entry 1339
    -0x1.ffffffffffffe0p-1023,
    0x1.ffffffffffffep-1023,
    -0x1.0p-1022,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 1340
    0.0,
    0x1.ffffffffffffep-1023,
    -0x1.0p-1022,
    0x1.0p-1074
  },
  { // Entry 1341
    -0x1.p-1074,
    0x1.ffffffffffffep-1023,
    -0x1.0p-1022,
    -0x1.0p-1074
  },
  { // Entry 1342
    -0.0,
    0x1.ffffffffffffep-1023,
    -0x1.0p-1022,
    0.0
  },
  { // Entry 1343
    -0.0,
    0x1.ffffffffffffep-1023,
    -0x1.0p-1022,
    -0.0
  },
  { // Entry 1344
    HUGE_VAL,
    0x1.ffffffffffffep-1023,
    0x1.ffffffffffffep-1023,
    HUGE_VAL
  },
  { // Entry 1345
    -HUGE_VAL,
    0x1.ffffffffffffep-1023,
    0x1.ffffffffffffep-1023,
    -HUGE_VAL
  },
  { // Entry 1346
    0x1.fffffffffffff0p1023,
    0x1.ffffffffffffep-1023,
    0x1.ffffffffffffep-1023,
    0x1.fffffffffffffp1023
  },
  { // Entry 1347
    -0x1.ffffffffffffefffffffffffffffffffp1023,
    0x1.ffffffffffffep-1023,
    0x1.ffffffffffffep-1023,
    -0x1.fffffffffffffp1023
  },
  { // Entry 1348
    0x1.p-1022,
    0x1.ffffffffffffep-1023,
    0x1.ffffffffffffep-1023,
    0x1.0p-1022
  },
  { // Entry 1349
    -0x1.ffffffffffffffffffffffffffffffffp-1023,
    0x1.ffffffffffffep-1023,
    0x1.ffffffffffffep-1023,
    -0x1.0p-1022
  },
  { // Entry 1350
    0x1.ffffffffffffe0p-1023,
    0x1.ffffffffffffep-1023,
    0x1.ffffffffffffep-1023,
    0x1.ffffffffffffep-1023
  },
  { // Entry 1351
    -0x1.ffffffffffffdfffffffffffffffffffp-1023,
    0x1.ffffffffffffep-1023,
    0x1.ffffffffffffep-1023,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 1352
    0x1.p-1074,
    0x1.ffffffffffffep-1023,
    0x1.ffffffffffffep-1023,
    0x1.0p-1074
  },
  { // Entry 1353
    -0.0,
    0x1.ffffffffffffep-1023,
    0x1.ffffffffffffep-1023,
    -0x1.0p-1074
  },
  { // Entry 1354
    0.0,
    0x1.ffffffffffffep-1023,
    0x1.ffffffffffffep-1023,
    0.0
  },
  { // Entry 1355
    0.0,
    0x1.ffffffffffffep-1023,
    0x1.ffffffffffffep-1023,
    -0.0
  },
  { // Entry 1356
    HUGE_VAL,
    0x1.ffffffffffffep-1023,
    -0x1.ffffffffffffep-1023,
    HUGE_VAL
  },
  { // Entry 1357
    -HUGE_VAL,
    0x1.ffffffffffffep-1023,
    -0x1.ffffffffffffep-1023,
    -HUGE_VAL
  },
  { // Entry 1358
    0x1.ffffffffffffefffffffffffffffffffp1023,
    0x1.ffffffffffffep-1023,
    -0x1.ffffffffffffep-1023,
    0x1.fffffffffffffp1023
  },
  { // Entry 1359
    -0x1.fffffffffffff0p1023,
    0x1.ffffffffffffep-1023,
    -0x1.ffffffffffffep-1023,
    -0x1.fffffffffffffp1023
  },
  { // Entry 1360
    0x1.ffffffffffffffffffffffffffffffffp-1023,
    0x1.ffffffffffffep-1023,
    -0x1.ffffffffffffep-1023,
    0x1.0p-1022
  },
  { // Entry 1361
    -0x1.p-1022,
    0x1.ffffffffffffep-1023,
    -0x1.ffffffffffffep-1023,
    -0x1.0p-1022
  },
  { // Entry 1362
    0x1.ffffffffffffdfffffffffffffffffffp-1023,
    0x1.ffffffffffffep-1023,
    -0x1.ffffffffffffep-1023,
    0x1.ffffffffffffep-1023
  },
  { // Entry 1363
    -0x1.ffffffffffffe0p-1023,
    0x1.ffffffffffffep-1023,
    -0x1.ffffffffffffep-1023,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 1364
    0.0,
    0x1.ffffffffffffep-1023,
    -0x1.ffffffffffffep-1023,
    0x1.0p-1074
  },
  { // Entry 1365
    -0x1.p-1074,
    0x1.ffffffffffffep-1023,
    -0x1.ffffffffffffep-1023,
    -0x1.0p-1074
  },
  { // Entry 1366
    -0.0,
    0x1.ffffffffffffep-1023,
    -0x1.ffffffffffffep-1023,
    0.0
  },
  { // Entry 1367
    -0.0,
    0x1.ffffffffffffep-1023,
    -0x1.ffffffffffffep-1023,
    -0.0
  },
  { // Entry 1368
    HUGE_VAL,
    0x1.ffffffffffffep-1023,
    0x1.0p-1074,
    HUGE_VAL
  },
  { // Entry 1369
    -HUGE_VAL,
    0x1.ffffffffffffep-1023,
    0x1.0p-1074,
    -HUGE_VAL
  },
  { // Entry 1370
    0x1.fffffffffffff0p1023,
    0x1.ffffffffffffep-1023,
    0x1.0p-1074,
    0x1.fffffffffffffp1023
  },
  { // Entry 1371
    -0x1.ffffffffffffefffffffffffffffffffp1023,
    0x1.ffffffffffffep-1023,
    0x1.0p-1074,
    -0x1.fffffffffffffp1023
  },
  { // Entry 1372
    0x1.p-1022,
    0x1.ffffffffffffep-1023,
    0x1.0p-1074,
    0x1.0p-1022
  },
  { // Entry 1373
    -0x1.ffffffffffffffffffffffffffffffffp-1023,
    0x1.ffffffffffffep-1023,
    0x1.0p-1074,
    -0x1.0p-1022
  },
  { // Entry 1374
    0x1.ffffffffffffe0p-1023,
    0x1.ffffffffffffep-1023,
    0x1.0p-1074,
    0x1.ffffffffffffep-1023
  },
  { // Entry 1375
    -0x1.ffffffffffffdfffffffffffffffffffp-1023,
    0x1.ffffffffffffep-1023,
    0x1.0p-1074,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 1376
    0x1.p-1074,
    0x1.ffffffffffffep-1023,
    0x1.0p-1074,
    0x1.0p-1074
  },
  { // Entry 1377
    -0.0,
    0x1.ffffffffffffep-1023,
    0x1.0p-1074,
    -0x1.0p-1074
  },
  { // Entry 1378
    0.0,
    0x1.ffffffffffffep-1023,
    0x1.0p-1074,
    0.0
  },
  { // Entry 1379
    0.0,
    0x1.ffffffffffffep-1023,
    0x1.0p-1074,
    -0.0
  },
  { // Entry 1380
    HUGE_VAL,
    0x1.ffffffffffffep-1023,
    -0x1.0p-1074,
    HUGE_VAL
  },
  { // Entry 1381
    -HUGE_VAL,
    0x1.ffffffffffffep-1023,
    -0x1.0p-1074,
    -HUGE_VAL
  },
  { // Entry 1382
    0x1.ffffffffffffefffffffffffffffffffp1023,
    0x1.ffffffffffffep-1023,
    -0x1.0p-1074,
    0x1.fffffffffffffp1023
  },
  { // Entry 1383
    -0x1.fffffffffffff0p1023,
    0x1.ffffffffffffep-1023,
    -0x1.0p-1074,
    -0x1.fffffffffffffp1023
  },
  { // Entry 1384
    0x1.ffffffffffffffffffffffffffffffffp-1023,
    0x1.ffffffffffffep-1023,
    -0x1.0p-1074,
    0x1.0p-1022
  },
  { // Entry 1385
    -0x1.p-1022,
    0x1.ffffffffffffep-1023,
    -0x1.0p-1074,
    -0x1.0p-1022
  },
  { // Entry 1386
    0x1.ffffffffffffdfffffffffffffffffffp-1023,
    0x1.ffffffffffffep-1023,
    -0x1.0p-1074,
    0x1.ffffffffffffep-1023
  },
  { // Entry 1387
    -0x1.ffffffffffffe0p-1023,
    0x1.ffffffffffffep-1023,
    -0x1.0p-1074,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 1388
    0.0,
    0x1.ffffffffffffep-1023,
    -0x1.0p-1074,
    0x1.0p-1074
  },
  { // Entry 1389
    -0x1.p-1074,
    0x1.ffffffffffffep-1023,
    -0x1.0p-1074,
    -0x1.0p-1074
  },
  { // Entry 1390
    -0.0,
    0x1.ffffffffffffep-1023,
    -0x1.0p-1074,
    0.0
  },
  { // Entry 1391
    -0.0,
    0x1.ffffffffffffep-1023,
    -0x1.0p-1074,
    -0.0
  },
  { // Entry 1392
    HUGE_VAL,
    0x1.ffffffffffffep-1023,
    0.0,
    HUGE_VAL
  },
  { // Entry 1393
    -HUGE_VAL,
    0x1.ffffffffffffep-1023,
    0.0,
    -HUGE_VAL
  },
  { // Entry 1394
    0x1.fffffffffffff0p1023,
    0x1.ffffffffffffep-1023,
    0.0,
    0x1.fffffffffffffp1023
  },
  { // Entry 1395
    -0x1.fffffffffffff0p1023,
    0x1.ffffffffffffep-1023,
    0.0,
    -0x1.fffffffffffffp1023
  },
  { // Entry 1396
    0x1.p-1022,
    0x1.ffffffffffffep-1023,
    0.0,
    0x1.0p-1022
  },
  { // Entry 1397
    -0x1.p-1022,
    0x1.ffffffffffffep-1023,
    0.0,
    -0x1.0p-1022
  },
  { // Entry 1398
    0x1.ffffffffffffe0p-1023,
    0x1.ffffffffffffep-1023,
    0.0,
    0x1.ffffffffffffep-1023
  },
  { // Entry 1399
    -0x1.ffffffffffffe0p-1023,
    0x1.ffffffffffffep-1023,
    0.0,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 1400
    0x1.p-1074,
    0x1.ffffffffffffep-1023,
    0.0,
    0x1.0p-1074
  },
  { // Entry 1401
    -0x1.p-1074,
    0x1.ffffffffffffep-1023,
    0.0,
    -0x1.0p-1074
  },
  { // Entry 1402
    0.0,
    0x1.ffffffffffffep-1023,
    0.0,
    0.0
  },
  { // Entry 1403
    0.0,
    0x1.ffffffffffffep-1023,
    0.0,
    -0.0
  },
  { // Entry 1404
    HUGE_VAL,
    0x1.ffffffffffffep-1023,
    -0.0,
    HUGE_VAL
  },
  { // Entry 1405
    -HUGE_VAL,
    0x1.ffffffffffffep-1023,
    -0.0,
    -HUGE_VAL
  },
  { // Entry 1406
    0x1.fffffffffffff0p1023,
    0x1.ffffffffffffep-1023,
    -0.0,
    0x1.fffffffffffffp1023
  },
  { // Entry 1407
    -0x1.fffffffffffff0p1023,
    0x1.ffffffffffffep-1023,
    -0.0,
    -0x1.fffffffffffffp1023
  },
  { // Entry 1408
    0x1.p-1022,
    0x1.ffffffffffffep-1023,
    -0.0,
    0x1.0p-1022
  },
  { // Entry 1409
    -0x1.p-1022,
    0x1.ffffffffffffep-1023,
    -0.0,
    -0x1.0p-1022
  },
  { // Entry 1410
    0x1.ffffffffffffe0p-1023,
    0x1.ffffffffffffep-1023,
    -0.0,
    0x1.ffffffffffffep-1023
  },
  { // Entry 1411
    -0x1.ffffffffffffe0p-1023,
    0x1.ffffffffffffep-1023,
    -0.0,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 1412
    0x1.p-1074,
    0x1.ffffffffffffep-1023,
    -0.0,
    0x1.0p-1074
  },
  { // Entry 1413
    -0x1.p-1074,
    0x1.ffffffffffffep-1023,
    -0.0,
    -0x1.0p-1074
  },
  { // Entry 1414
    0.0,
    0x1.ffffffffffffep-1023,
    -0.0,
    0.0
  },
  { // Entry 1415
    -0.0,
    0x1.ffffffffffffep-1023,
    -0.0,
    -0.0
  },
  { // Entry 1416
    -HUGE_VAL,
    -0x1.ffffffffffffep-1023,
    HUGE_VAL,
    -HUGE_VAL
  },
  { // Entry 1417
    -HUGE_VAL,
    -0x1.ffffffffffffep-1023,
    HUGE_VAL,
    0x1.fffffffffffffp1023
  },
  { // Entry 1418
    -HUGE_VAL,
    -0x1.ffffffffffffep-1023,
    HUGE_VAL,
    -0x1.fffffffffffffp1023
  },
  { // Entry 1419
    -HUGE_VAL,
    -0x1.ffffffffffffep-1023,
    HUGE_VAL,
    0x1.0p-1022
  },
  { // Entry 1420
    -HUGE_VAL,
    -0x1.ffffffffffffep-1023,
    HUGE_VAL,
    -0x1.0p-1022
  },
  { // Entry 1421
    -HUGE_VAL,
    -0x1.ffffffffffffep-1023,
    HUGE_VAL,
    0x1.ffffffffffffep-1023
  },
  { // Entry 1422
    -HUGE_VAL,
    -0x1.ffffffffffffep-1023,
    HUGE_VAL,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 1423
    -HUGE_VAL,
    -0x1.ffffffffffffep-1023,
    HUGE_VAL,
    0x1.0p-1074
  },
  { // Entry 1424
    -HUGE_VAL,
    -0x1.ffffffffffffep-1023,
    HUGE_VAL,
    -0x1.0p-1074
  },
  { // Entry 1425
    -HUGE_VAL,
    -0x1.ffffffffffffep-1023,
    HUGE_VAL,
    0.0
  },
  { // Entry 1426
    -HUGE_VAL,
    -0x1.ffffffffffffep-1023,
    HUGE_VAL,
    -0.0
  },
  { // Entry 1427
    HUGE_VAL,
    -0x1.ffffffffffffep-1023,
    -HUGE_VAL,
    HUGE_VAL
  },
  { // Entry 1428
    HUGE_VAL,
    -0x1.ffffffffffffep-1023,
    -HUGE_VAL,
    0x1.fffffffffffffp1023
  },
  { // Entry 1429
    HUGE_VAL,
    -0x1.ffffffffffffep-1023,
    -HUGE_VAL,
    -0x1.fffffffffffffp1023
  },
  { // Entry 1430
    HUGE_VAL,
    -0x1.ffffffffffffep-1023,
    -HUGE_VAL,
    0x1.0p-1022
  },
  { // Entry 1431
    HUGE_VAL,
    -0x1.ffffffffffffep-1023,
    -HUGE_VAL,
    -0x1.0p-1022
  },
  { // Entry 1432
    HUGE_VAL,
    -0x1.ffffffffffffep-1023,
    -HUGE_VAL,
    0x1.ffffffffffffep-1023
  },
  { // Entry 1433
    HUGE_VAL,
    -0x1.ffffffffffffep-1023,
    -HUGE_VAL,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 1434
    HUGE_VAL,
    -0x1.ffffffffffffep-1023,
    -HUGE_VAL,
    0x1.0p-1074
  },
  { // Entry 1435
    HUGE_VAL,
    -0x1.ffffffffffffep-1023,
    -HUGE_VAL,
    -0x1.0p-1074
  },
  { // Entry 1436
    HUGE_VAL,
    -0x1.ffffffffffffep-1023,
    -HUGE_VAL,
    0.0
  },
  { // Entry 1437
    HUGE_VAL,
    -0x1.ffffffffffffep-1023,
    -HUGE_VAL,
    -0.0
  },
  { // Entry 1438
    HUGE_VAL,
    -0x1.ffffffffffffep-1023,
    0x1.fffffffffffffp1023,
    HUGE_VAL
  },
  { // Entry 1439
    -HUGE_VAL,
    -0x1.ffffffffffffep-1023,
    0x1.fffffffffffffp1023,
    -HUGE_VAL
  },
  { // Entry 1440
    0x1.ffffffffffffefffffffffffffffffffp1023,
    -0x1.ffffffffffffep-1023,
    0x1.fffffffffffffp1023,
    0x1.fffffffffffffp1023
  },
  { // Entry 1441
    -0x1.fffffffffffff0p1023,
    -0x1.ffffffffffffep-1023,
    0x1.fffffffffffffp1023,
    -0x1.fffffffffffffp1023
  },
  { // Entry 1442
    -0x1.ffffffffffffd0000000000000ffffffp1,
    -0x1.ffffffffffffep-1023,
    0x1.fffffffffffffp1023,
    0x1.0p-1022
  },
  { // Entry 1443
    -0x1.ffffffffffffd0000000000001p1,
    -0x1.ffffffffffffep-1023,
    0x1.fffffffffffffp1023,
    -0x1.0p-1022
  },
  { // Entry 1444
    -0x1.ffffffffffffd0000000000000ffffffp1,
    -0x1.ffffffffffffep-1023,
    0x1.fffffffffffffp1023,
    0x1.ffffffffffffep-1023
  },
  { // Entry 1445
    -0x1.ffffffffffffd0000000000001p1,
    -0x1.ffffffffffffep-1023,
    0x1.fffffffffffffp1023,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 1446
    -0x1.ffffffffffffd0000000000000ffffffp1,
    -0x1.ffffffffffffep-1023,
    0x1.fffffffffffffp1023,
    0x1.0p-1074
  },
  { // Entry 1447
    -0x1.ffffffffffffd0000000000001p1,
    -0x1.ffffffffffffep-1023,
    0x1.fffffffffffffp1023,
    -0x1.0p-1074
  },
  { // Entry 1448
    -0x1.ffffffffffffd0000000000001p1,
    -0x1.ffffffffffffep-1023,
    0x1.fffffffffffffp1023,
    0.0
  },
  { // Entry 1449
    -0x1.ffffffffffffd0000000000001p1,
    -0x1.ffffffffffffep-1023,
    0x1.fffffffffffffp1023,
    -0.0
  },
  { // Entry 1450
    HUGE_VAL,
    -0x1.ffffffffffffep-1023,
    -0x1.fffffffffffffp1023,
    HUGE_VAL
  },
  { // Entry 1451
    -HUGE_VAL,
    -0x1.ffffffffffffep-1023,
    -0x1.fffffffffffffp1023,
    -HUGE_VAL
  },
  { // Entry 1452
    0x1.fffffffffffff0p1023,
    -0x1.ffffffffffffep-1023,
    -0x1.fffffffffffffp1023,
    0x1.fffffffffffffp1023
  },
  { // Entry 1453
    -0x1.ffffffffffffefffffffffffffffffffp1023,
    -0x1.ffffffffffffep-1023,
    -0x1.fffffffffffffp1023,
    -0x1.fffffffffffffp1023
  },
  { // Entry 1454
    0x1.ffffffffffffd0000000000001p1,
    -0x1.ffffffffffffep-1023,
    -0x1.fffffffffffffp1023,
    0x1.0p-1022
  },
  { // Entry 1455
    0x1.ffffffffffffd0000000000000ffffffp1,
    -0x1.ffffffffffffep-1023,
    -0x1.fffffffffffffp1023,
    -0x1.0p-1022
  },
  { // Entry 1456
    0x1.ffffffffffffd0000000000001p1,
    -0x1.ffffffffffffep-1023,
    -0x1.fffffffffffffp1023,
    0x1.ffffffffffffep-1023
  },
  { // Entry 1457
    0x1.ffffffffffffd0000000000000ffffffp1,
    -0x1.ffffffffffffep-1023,
    -0x1.fffffffffffffp1023,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 1458
    0x1.ffffffffffffd0000000000001p1,
    -0x1.ffffffffffffep-1023,
    -0x1.fffffffffffffp1023,
    0x1.0p-1074
  },
  { // Entry 1459
    0x1.ffffffffffffd0000000000000ffffffp1,
    -0x1.ffffffffffffep-1023,
    -0x1.fffffffffffffp1023,
    -0x1.0p-1074
  },
  { // Entry 1460
    0x1.ffffffffffffd0000000000001p1,
    -0x1.ffffffffffffep-1023,
    -0x1.fffffffffffffp1023,
    0.0
  },
  { // Entry 1461
    0x1.ffffffffffffd0000000000001p1,
    -0x1.ffffffffffffep-1023,
    -0x1.fffffffffffffp1023,
    -0.0
  },
  { // Entry 1462
    HUGE_VAL,
    -0x1.ffffffffffffep-1023,
    0x1.0p-1022,
    HUGE_VAL
  },
  { // Entry 1463
    -HUGE_VAL,
    -0x1.ffffffffffffep-1023,
    0x1.0p-1022,
    -HUGE_VAL
  },
  { // Entry 1464
    0x1.ffffffffffffefffffffffffffffffffp1023,
    -0x1.ffffffffffffep-1023,
    0x1.0p-1022,
    0x1.fffffffffffffp1023
  },
  { // Entry 1465
    -0x1.fffffffffffff0p1023,
    -0x1.ffffffffffffep-1023,
    0x1.0p-1022,
    -0x1.fffffffffffffp1023
  },
  { // Entry 1466
    0x1.ffffffffffffffffffffffffffffffffp-1023,
    -0x1.ffffffffffffep-1023,
    0x1.0p-1022,
    0x1.0p-1022
  },
  { // Entry 1467
    -0x1.p-1022,
    -0x1.ffffffffffffep-1023,
    0x1.0p-1022,
    -0x1.0p-1022
  },
  { // Entry 1468
    0x1.ffffffffffffdfffffffffffffffffffp-1023,
    -0x1.ffffffffffffep-1023,
    0x1.0p-1022,
    0x1.ffffffffffffep-1023
  },
  { // Entry 1469
    -0x1.ffffffffffffe0p-1023,
    -0x1.ffffffffffffep-1023,
    0x1.0p-1022,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 1470
    0.0,
    -0x1.ffffffffffffep-1023,
    0x1.0p-1022,
    0x1.0p-1074
  },
  { // Entry 1471
    -0x1.p-1074,
    -0x1.ffffffffffffep-1023,
    0x1.0p-1022,
    -0x1.0p-1074
  },
  { // Entry 1472
    -0.0,
    -0x1.ffffffffffffep-1023,
    0x1.0p-1022,
    0.0
  },
  { // Entry 1473
    -0.0,
    -0x1.ffffffffffffep-1023,
    0x1.0p-1022,
    -0.0
  },
  { // Entry 1474
    HUGE_VAL,
    -0x1.ffffffffffffep-1023,
    -0x1.0p-1022,
    HUGE_VAL
  },
  { // Entry 1475
    -HUGE_VAL,
    -0x1.ffffffffffffep-1023,
    -0x1.0p-1022,
    -HUGE_VAL
  },
  { // Entry 1476
    0x1.fffffffffffff0p1023,
    -0x1.ffffffffffffep-1023,
    -0x1.0p-1022,
    0x1.fffffffffffffp1023
  },
  { // Entry 1477
    -0x1.ffffffffffffefffffffffffffffffffp1023,
    -0x1.ffffffffffffep-1023,
    -0x1.0p-1022,
    -0x1.fffffffffffffp1023
  },
  { // Entry 1478
    0x1.p-1022,
    -0x1.ffffffffffffep-1023,
    -0x1.0p-1022,
    0x1.0p-1022
  },
  { // Entry 1479
    -0x1.ffffffffffffffffffffffffffffffffp-1023,
    -0x1.ffffffffffffep-1023,
    -0x1.0p-1022,
    -0x1.0p-1022
  },
  { // Entry 1480
    0x1.ffffffffffffe0p-1023,
    -0x1.ffffffffffffep-1023,
    -0x1.0p-1022,
    0x1.ffffffffffffep-1023
  },
  { // Entry 1481
    -0x1.ffffffffffffdfffffffffffffffffffp-1023,
    -0x1.ffffffffffffep-1023,
    -0x1.0p-1022,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 1482
    0x1.p-1074,
    -0x1.ffffffffffffep-1023,
    -0x1.0p-1022,
    0x1.0p-1074
  },
  { // Entry 1483
    -0.0,
    -0x1.ffffffffffffep-1023,
    -0x1.0p-1022,
    -0x1.0p-1074
  },
  { // Entry 1484
    0.0,
    -0x1.ffffffffffffep-1023,
    -0x1.0p-1022,
    0.0
  },
  { // Entry 1485
    0.0,
    -0x1.ffffffffffffep-1023,
    -0x1.0p-1022,
    -0.0
  },
  { // Entry 1486
    HUGE_VAL,
    -0x1.ffffffffffffep-1023,
    0x1.ffffffffffffep-1023,
    HUGE_VAL
  },
  { // Entry 1487
    -HUGE_VAL,
    -0x1.ffffffffffffep-1023,
    0x1.ffffffffffffep-1023,
    -HUGE_VAL
  },
  { // Entry 1488
    0x1.ffffffffffffefffffffffffffffffffp1023,
    -0x1.ffffffffffffep-1023,
    0x1.ffffffffffffep-1023,
    0x1.fffffffffffffp1023
  },
  { // Entry 1489
    -0x1.fffffffffffff0p1023,
    -0x1.ffffffffffffep-1023,
    0x1.ffffffffffffep-1023,
    -0x1.fffffffffffffp1023
  },
  { // Entry 1490
    0x1.ffffffffffffffffffffffffffffffffp-1023,
    -0x1.ffffffffffffep-1023,
    0x1.ffffffffffffep-1023,
    0x1.0p-1022
  },
  { // Entry 1491
    -0x1.p-1022,
    -0x1.ffffffffffffep-1023,
    0x1.ffffffffffffep-1023,
    -0x1.0p-1022
  },
  { // Entry 1492
    0x1.ffffffffffffdfffffffffffffffffffp-1023,
    -0x1.ffffffffffffep-1023,
    0x1.ffffffffffffep-1023,
    0x1.ffffffffffffep-1023
  },
  { // Entry 1493
    -0x1.ffffffffffffe0p-1023,
    -0x1.ffffffffffffep-1023,
    0x1.ffffffffffffep-1023,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 1494
    0.0,
    -0x1.ffffffffffffep-1023,
    0x1.ffffffffffffep-1023,
    0x1.0p-1074
  },
  { // Entry 1495
    -0x1.p-1074,
    -0x1.ffffffffffffep-1023,
    0x1.ffffffffffffep-1023,
    -0x1.0p-1074
  },
  { // Entry 1496
    -0.0,
    -0x1.ffffffffffffep-1023,
    0x1.ffffffffffffep-1023,
    0.0
  },
  { // Entry 1497
    -0.0,
    -0x1.ffffffffffffep-1023,
    0x1.ffffffffffffep-1023,
    -0.0
  },
  { // Entry 1498
    HUGE_VAL,
    -0x1.ffffffffffffep-1023,
    -0x1.ffffffffffffep-1023,
    HUGE_VAL
  },
  { // Entry 1499
    -HUGE_VAL,
    -0x1.ffffffffffffep-1023,
    -0x1.ffffffffffffep-1023,
    -HUGE_VAL
  },
  { // Entry 1500
    0x1.fffffffffffff0p1023,
    -0x1.ffffffffffffep-1023,
    -0x1.ffffffffffffep-1023,
    0x1.fffffffffffffp1023
  },
  { // Entry 1501
    -0x1.ffffffffffffefffffffffffffffffffp1023,
    -0x1.ffffffffffffep-1023,
    -0x1.ffffffffffffep-1023,
    -0x1.fffffffffffffp1023
  },
  { // Entry 1502
    0x1.p-1022,
    -0x1.ffffffffffffep-1023,
    -0x1.ffffffffffffep-1023,
    0x1.0p-1022
  },
  { // Entry 1503
    -0x1.ffffffffffffffffffffffffffffffffp-1023,
    -0x1.ffffffffffffep-1023,
    -0x1.ffffffffffffep-1023,
    -0x1.0p-1022
  },
  { // Entry 1504
    0x1.ffffffffffffe0p-1023,
    -0x1.ffffffffffffep-1023,
    -0x1.ffffffffffffep-1023,
    0x1.ffffffffffffep-1023
  },
  { // Entry 1505
    -0x1.ffffffffffffdfffffffffffffffffffp-1023,
    -0x1.ffffffffffffep-1023,
    -0x1.ffffffffffffep-1023,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 1506
    0x1.p-1074,
    -0x1.ffffffffffffep-1023,
    -0x1.ffffffffffffep-1023,
    0x1.0p-1074
  },
  { // Entry 1507
    -0.0,
    -0x1.ffffffffffffep-1023,
    -0x1.ffffffffffffep-1023,
    -0x1.0p-1074
  },
  { // Entry 1508
    0.0,
    -0x1.ffffffffffffep-1023,
    -0x1.ffffffffffffep-1023,
    0.0
  },
  { // Entry 1509
    0.0,
    -0x1.ffffffffffffep-1023,
    -0x1.ffffffffffffep-1023,
    -0.0
  },
  { // Entry 1510
    HUGE_VAL,
    -0x1.ffffffffffffep-1023,
    0x1.0p-1074,
    HUGE_VAL
  },
  { // Entry 1511
    -HUGE_VAL,
    -0x1.ffffffffffffep-1023,
    0x1.0p-1074,
    -HUGE_VAL
  },
  { // Entry 1512
    0x1.ffffffffffffefffffffffffffffffffp1023,
    -0x1.ffffffffffffep-1023,
    0x1.0p-1074,
    0x1.fffffffffffffp1023
  },
  { // Entry 1513
    -0x1.fffffffffffff0p1023,
    -0x1.ffffffffffffep-1023,
    0x1.0p-1074,
    -0x1.fffffffffffffp1023
  },
  { // Entry 1514
    0x1.ffffffffffffffffffffffffffffffffp-1023,
    -0x1.ffffffffffffep-1023,
    0x1.0p-1074,
    0x1.0p-1022
  },
  { // Entry 1515
    -0x1.p-1022,
    -0x1.ffffffffffffep-1023,
    0x1.0p-1074,
    -0x1.0p-1022
  },
  { // Entry 1516
    0x1.ffffffffffffdfffffffffffffffffffp-1023,
    -0x1.ffffffffffffep-1023,
    0x1.0p-1074,
    0x1.ffffffffffffep-1023
  },
  { // Entry 1517
    -0x1.ffffffffffffe0p-1023,
    -0x1.ffffffffffffep-1023,
    0x1.0p-1074,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 1518
    0.0,
    -0x1.ffffffffffffep-1023,
    0x1.0p-1074,
    0x1.0p-1074
  },
  { // Entry 1519
    -0x1.p-1074,
    -0x1.ffffffffffffep-1023,
    0x1.0p-1074,
    -0x1.0p-1074
  },
  { // Entry 1520
    -0.0,
    -0x1.ffffffffffffep-1023,
    0x1.0p-1074,
    0.0
  },
  { // Entry 1521
    -0.0,
    -0x1.ffffffffffffep-1023,
    0x1.0p-1074,
    -0.0
  },
  { // Entry 1522
    HUGE_VAL,
    -0x1.ffffffffffffep-1023,
    -0x1.0p-1074,
    HUGE_VAL
  },
  { // Entry 1523
    -HUGE_VAL,
    -0x1.ffffffffffffep-1023,
    -0x1.0p-1074,
    -HUGE_VAL
  },
  { // Entry 1524
    0x1.fffffffffffff0p1023,
    -0x1.ffffffffffffep-1023,
    -0x1.0p-1074,
    0x1.fffffffffffffp1023
  },
  { // Entry 1525
    -0x1.ffffffffffffefffffffffffffffffffp1023,
    -0x1.ffffffffffffep-1023,
    -0x1.0p-1074,
    -0x1.fffffffffffffp1023
  },
  { // Entry 1526
    0x1.p-1022,
    -0x1.ffffffffffffep-1023,
    -0x1.0p-1074,
    0x1.0p-1022
  },
  { // Entry 1527
    -0x1.ffffffffffffffffffffffffffffffffp-1023,
    -0x1.ffffffffffffep-1023,
    -0x1.0p-1074,
    -0x1.0p-1022
  },
  { // Entry 1528
    0x1.ffffffffffffe0p-1023,
    -0x1.ffffffffffffep-1023,
    -0x1.0p-1074,
    0x1.ffffffffffffep-1023
  },
  { // Entry 1529
    -0x1.ffffffffffffdfffffffffffffffffffp-1023,
    -0x1.ffffffffffffep-1023,
    -0x1.0p-1074,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 1530
    0x1.p-1074,
    -0x1.ffffffffffffep-1023,
    -0x1.0p-1074,
    0x1.0p-1074
  },
  { // Entry 1531
    -0.0,
    -0x1.ffffffffffffep-1023,
    -0x1.0p-1074,
    -0x1.0p-1074
  },
  { // Entry 1532
    0.0,
    -0x1.ffffffffffffep-1023,
    -0x1.0p-1074,
    0.0
  },
  { // Entry 1533
    0.0,
    -0x1.ffffffffffffep-1023,
    -0x1.0p-1074,
    -0.0
  },
  { // Entry 1534
    HUGE_VAL,
    -0x1.ffffffffffffep-1023,
    0.0,
    HUGE_VAL
  },
  { // Entry 1535
    -HUGE_VAL,
    -0x1.ffffffffffffep-1023,
    0.0,
    -HUGE_VAL
  },
  { // Entry 1536
    0x1.fffffffffffff0p1023,
    -0x1.ffffffffffffep-1023,
    0.0,
    0x1.fffffffffffffp1023
  },
  { // Entry 1537
    -0x1.fffffffffffff0p1023,
    -0x1.ffffffffffffep-1023,
    0.0,
    -0x1.fffffffffffffp1023
  },
  { // Entry 1538
    0x1.p-1022,
    -0x1.ffffffffffffep-1023,
    0.0,
    0x1.0p-1022
  },
  { // Entry 1539
    -0x1.p-1022,
    -0x1.ffffffffffffep-1023,
    0.0,
    -0x1.0p-1022
  },
  { // Entry 1540
    0x1.ffffffffffffe0p-1023,
    -0x1.ffffffffffffep-1023,
    0.0,
    0x1.ffffffffffffep-1023
  },
  { // Entry 1541
    -0x1.ffffffffffffe0p-1023,
    -0x1.ffffffffffffep-1023,
    0.0,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 1542
    0x1.p-1074,
    -0x1.ffffffffffffep-1023,
    0.0,
    0x1.0p-1074
  },
  { // Entry 1543
    -0x1.p-1074,
    -0x1.ffffffffffffep-1023,
    0.0,
    -0x1.0p-1074
  },
  { // Entry 1544
    0.0,
    -0x1.ffffffffffffep-1023,
    0.0,
    0.0
  },
  { // Entry 1545
    -0.0,
    -0x1.ffffffffffffep-1023,
    0.0,
    -0.0
  },
  { // Entry 1546
    HUGE_VAL,
    -0x1.ffffffffffffep-1023,
    -0.0,
    HUGE_VAL
  },
  { // Entry 1547
    -HUGE_VAL,
    -0x1.ffffffffffffep-1023,
    -0.0,
    -HUGE_VAL
  },
  { // Entry 1548
    0x1.fffffffffffff0p1023,
    -0x1.ffffffffffffep-1023,
    -0.0,
    0x1.fffffffffffffp1023
  },
  { // Entry 1549
    -0x1.fffffffffffff0p1023,
    -0x1.ffffffffffffep-1023,
    -0.0,
    -0x1.fffffffffffffp1023
  },
  { // Entry 1550
    0x1.p-1022,
    -0x1.ffffffffffffep-1023,
    -0.0,
    0x1.0p-1022
  },
  { // Entry 1551
    -0x1.p-1022,
    -0x1.ffffffffffffep-1023,
    -0.0,
    -0x1.0p-1022
  },
  { // Entry 1552
    0x1.ffffffffffffe0p-1023,
    -0x1.ffffffffffffep-1023,
    -0.0,
    0x1.ffffffffffffep-1023
  },
  { // Entry 1553
    -0x1.ffffffffffffe0p-1023,
    -0x1.ffffffffffffep-1023,
    -0.0,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 1554
    0x1.p-1074,
    -0x1.ffffffffffffep-1023,
    -0.0,
    0x1.0p-1074
  },
  { // Entry 1555
    -0x1.p-1074,
    -0x1.ffffffffffffep-1023,
    -0.0,
    -0x1.0p-1074
  },
  { // Entry 1556
    0.0,
    -0x1.ffffffffffffep-1023,
    -0.0,
    0.0
  },
  { // Entry 1557
    0.0,
    -0x1.ffffffffffffep-1023,
    -0.0,
    -0.0
  },
  { // Entry 1558
    HUGE_VAL,
    0x1.0p-1074,
    HUGE_VAL,
    HUGE_VAL
  },
  { // Entry 1559
    HUGE_VAL,
    0x1.0p-1074,
    HUGE_VAL,
    0x1.fffffffffffffp1023
  },
  { // Entry 1560
    HUGE_VAL,
    0x1.0p-1074,
    HUGE_VAL,
    -0x1.fffffffffffffp1023
  },
  { // Entry 1561
    HUGE_VAL,
    0x1.0p-1074,
    HUGE_VAL,
    0x1.0p-1022
  },
  { // Entry 1562
    HUGE_VAL,
    0x1.0p-1074,
    HUGE_VAL,
    -0x1.0p-1022
  },
  { // Entry 1563
    HUGE_VAL,
    0x1.0p-1074,
    HUGE_VAL,
    0x1.ffffffffffffep-1023
  },
  { // Entry 1564
    HUGE_VAL,
    0x1.0p-1074,
    HUGE_VAL,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 1565
    HUGE_VAL,
    0x1.0p-1074,
    HUGE_VAL,
    0x1.0p-1074
  },
  { // Entry 1566
    HUGE_VAL,
    0x1.0p-1074,
    HUGE_VAL,
    -0x1.0p-1074
  },
  { // Entry 1567
    HUGE_VAL,
    0x1.0p-1074,
    HUGE_VAL,
    0.0
  },
  { // Entry 1568
    HUGE_VAL,
    0x1.0p-1074,
    HUGE_VAL,
    -0.0
  },
  { // Entry 1569
    -HUGE_VAL,
    0x1.0p-1074,
    -HUGE_VAL,
    -HUGE_VAL
  },
  { // Entry 1570
    -HUGE_VAL,
    0x1.0p-1074,
    -HUGE_VAL,
    0x1.fffffffffffffp1023
  },
  { // Entry 1571
    -HUGE_VAL,
    0x1.0p-1074,
    -HUGE_VAL,
    -0x1.fffffffffffffp1023
  },
  { // Entry 1572
    -HUGE_VAL,
    0x1.0p-1074,
    -HUGE_VAL,
    0x1.0p-1022
  },
  { // Entry 1573
    -HUGE_VAL,
    0x1.0p-1074,
    -HUGE_VAL,
    -0x1.0p-1022
  },
  { // Entry 1574
    -HUGE_VAL,
    0x1.0p-1074,
    -HUGE_VAL,
    0x1.ffffffffffffep-1023
  },
  { // Entry 1575
    -HUGE_VAL,
    0x1.0p-1074,
    -HUGE_VAL,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 1576
    -HUGE_VAL,
    0x1.0p-1074,
    -HUGE_VAL,
    0x1.0p-1074
  },
  { // Entry 1577
    -HUGE_VAL,
    0x1.0p-1074,
    -HUGE_VAL,
    -0x1.0p-1074
  },
  { // Entry 1578
    -HUGE_VAL,
    0x1.0p-1074,
    -HUGE_VAL,
    0.0
  },
  { // Entry 1579
    -HUGE_VAL,
    0x1.0p-1074,
    -HUGE_VAL,
    -0.0
  },
  { // Entry 1580
    HUGE_VAL,
    0x1.0p-1074,
    0x1.fffffffffffffp1023,
    HUGE_VAL
  },
  { // Entry 1581
    -HUGE_VAL,
    0x1.0p-1074,
    0x1.fffffffffffffp1023,
    -HUGE_VAL
  },
  { // Entry 1582
    0x1.fffffffffffff0p1023,
    0x1.0p-1074,
    0x1.fffffffffffffp1023,
    0x1.fffffffffffffp1023
  },
  { // Entry 1583
    -0x1.ffffffffffffefffffffffffffffffffp1023,
    0x1.0p-1074,
    0x1.fffffffffffffp1023,
    -0x1.fffffffffffffp1023
  },
  { // Entry 1584
    0x1.fffffffffffff0p-51,
    0x1.0p-1074,
    0x1.fffffffffffffp1023,
    0x1.0p-1022
  },
  { // Entry 1585
    0x1.ffffffffffffefffffffffffffffffffp-51,
    0x1.0p-1074,
    0x1.fffffffffffffp1023,
    -0x1.0p-1022
  },
  { // Entry 1586
    0x1.fffffffffffff0p-51,
    0x1.0p-1074,
    0x1.fffffffffffffp1023,
    0x1.ffffffffffffep-1023
  },
  { // Entry 1587
    0x1.ffffffffffffefffffffffffffffffffp-51,
    0x1.0p-1074,
    0x1.fffffffffffffp1023,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 1588
    0x1.fffffffffffff0p-51,
    0x1.0p-1074,
    0x1.fffffffffffffp1023,
    0x1.0p-1074
  },
  { // Entry 1589
    0x1.ffffffffffffefffffffffffffffffffp-51,
    0x1.0p-1074,
    0x1.fffffffffffffp1023,
    -0x1.0p-1074
  },
  { // Entry 1590
    0x1.fffffffffffff0p-51,
    0x1.0p-1074,
    0x1.fffffffffffffp1023,
    0.0
  },
  { // Entry 1591
    0x1.fffffffffffff0p-51,
    0x1.0p-1074,
    0x1.fffffffffffffp1023,
    -0.0
  },
  { // Entry 1592
    HUGE_VAL,
    0x1.0p-1074,
    -0x1.fffffffffffffp1023,
    HUGE_VAL
  },
  { // Entry 1593
    -HUGE_VAL,
    0x1.0p-1074,
    -0x1.fffffffffffffp1023,
    -HUGE_VAL
  },
  { // Entry 1594
    0x1.ffffffffffffefffffffffffffffffffp1023,
    0x1.0p-1074,
    -0x1.fffffffffffffp1023,
    0x1.fffffffffffffp1023
  },
  { // Entry 1595
    -0x1.fffffffffffff0p1023,
    0x1.0p-1074,
    -0x1.fffffffffffffp1023,
    -0x1.fffffffffffffp1023
  },
  { // Entry 1596
    -0x1.ffffffffffffefffffffffffffffffffp-51,
    0x1.0p-1074,
    -0x1.fffffffffffffp1023,
    0x1.0p-1022
  },
  { // Entry 1597
    -0x1.fffffffffffff0p-51,
    0x1.0p-1074,
    -0x1.fffffffffffffp1023,
    -0x1.0p-1022
  },
  { // Entry 1598
    -0x1.ffffffffffffefffffffffffffffffffp-51,
    0x1.0p-1074,
    -0x1.fffffffffffffp1023,
    0x1.ffffffffffffep-1023
  },
  { // Entry 1599
    -0x1.fffffffffffff0p-51,
    0x1.0p-1074,
    -0x1.fffffffffffffp1023,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 1600
    -0x1.ffffffffffffefffffffffffffffffffp-51,
    0x1.0p-1074,
    -0x1.fffffffffffffp1023,
    0x1.0p-1074
  },
  { // Entry 1601
    -0x1.fffffffffffff0p-51,
    0x1.0p-1074,
    -0x1.fffffffffffffp1023,
    -0x1.0p-1074
  },
  { // Entry 1602
    -0x1.fffffffffffff0p-51,
    0x1.0p-1074,
    -0x1.fffffffffffffp1023,
    0.0
  },
  { // Entry 1603
    -0x1.fffffffffffff0p-51,
    0x1.0p-1074,
    -0x1.fffffffffffffp1023,
    -0.0
  },
  { // Entry 1604
    HUGE_VAL,
    0x1.0p-1074,
    0x1.0p-1022,
    HUGE_VAL
  },
  { // Entry 1605
    -HUGE_VAL,
    0x1.0p-1074,
    0x1.0p-1022,
    -HUGE_VAL
  },
  { // Entry 1606
    0x1.fffffffffffff0p1023,
    0x1.0p-1074,
    0x1.0p-1022,
    0x1.fffffffffffffp1023
  },
  { // Entry 1607
    -0x1.ffffffffffffefffffffffffffffffffp1023,
    0x1.0p-1074,
    0x1.0p-1022,
    -0x1.fffffffffffffp1023
  },
  { // Entry 1608
    0x1.p-1022,
    0x1.0p-1074,
    0x1.0p-1022,
    0x1.0p-1022
  },
  { // Entry 1609
    -0x1.ffffffffffffffffffffffffffffffffp-1023,
    0x1.0p-1074,
    0x1.0p-1022,
    -0x1.0p-1022
  },
  { // Entry 1610
    0x1.ffffffffffffe0p-1023,
    0x1.0p-1074,
    0x1.0p-1022,
    0x1.ffffffffffffep-1023
  },
  { // Entry 1611
    -0x1.ffffffffffffdfffffffffffffffffffp-1023,
    0x1.0p-1074,
    0x1.0p-1022,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 1612
    0x1.p-1074,
    0x1.0p-1074,
    0x1.0p-1022,
    0x1.0p-1074
  },
  { // Entry 1613
    -0.0,
    0x1.0p-1074,
    0x1.0p-1022,
    -0x1.0p-1074
  },
  { // Entry 1614
    0.0,
    0x1.0p-1074,
    0x1.0p-1022,
    0.0
  },
  { // Entry 1615
    0.0,
    0x1.0p-1074,
    0x1.0p-1022,
    -0.0
  },
  { // Entry 1616
    HUGE_VAL,
    0x1.0p-1074,
    -0x1.0p-1022,
    HUGE_VAL
  },
  { // Entry 1617
    -HUGE_VAL,
    0x1.0p-1074,
    -0x1.0p-1022,
    -HUGE_VAL
  },
  { // Entry 1618
    0x1.ffffffffffffefffffffffffffffffffp1023,
    0x1.0p-1074,
    -0x1.0p-1022,
    0x1.fffffffffffffp1023
  },
  { // Entry 1619
    -0x1.fffffffffffff0p1023,
    0x1.0p-1074,
    -0x1.0p-1022,
    -0x1.fffffffffffffp1023
  },
  { // Entry 1620
    0x1.ffffffffffffffffffffffffffffffffp-1023,
    0x1.0p-1074,
    -0x1.0p-1022,
    0x1.0p-1022
  },
  { // Entry 1621
    -0x1.p-1022,
    0x1.0p-1074,
    -0x1.0p-1022,
    -0x1.0p-1022
  },
  { // Entry 1622
    0x1.ffffffffffffdfffffffffffffffffffp-1023,
    0x1.0p-1074,
    -0x1.0p-1022,
    0x1.ffffffffffffep-1023
  },
  { // Entry 1623
    -0x1.ffffffffffffe0p-1023,
    0x1.0p-1074,
    -0x1.0p-1022,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 1624
    0.0,
    0x1.0p-1074,
    -0x1.0p-1022,
    0x1.0p-1074
  },
  { // Entry 1625
    -0x1.p-1074,
    0x1.0p-1074,
    -0x1.0p-1022,
    -0x1.0p-1074
  },
  { // Entry 1626
    -0.0,
    0x1.0p-1074,
    -0x1.0p-1022,
    0.0
  },
  { // Entry 1627
    -0.0,
    0x1.0p-1074,
    -0x1.0p-1022,
    -0.0
  },
  { // Entry 1628
    HUGE_VAL,
    0x1.0p-1074,
    0x1.ffffffffffffep-1023,
    HUGE_VAL
  },
  { // Entry 1629
    -HUGE_VAL,
    0x1.0p-1074,
    0x1.ffffffffffffep-1023,
    -HUGE_VAL
  },
  { // Entry 1630
    0x1.fffffffffffff0p1023,
    0x1.0p-1074,
    0x1.ffffffffffffep-1023,
    0x1.fffffffffffffp1023
  },
  { // Entry 1631
    -0x1.ffffffffffffefffffffffffffffffffp1023,
    0x1.0p-1074,
    0x1.ffffffffffffep-1023,
    -0x1.fffffffffffffp1023
  },
  { // Entry 1632
    0x1.p-1022,
    0x1.0p-1074,
    0x1.ffffffffffffep-1023,
    0x1.0p-1022
  },
  { // Entry 1633
    -0x1.ffffffffffffffffffffffffffffffffp-1023,
    0x1.0p-1074,
    0x1.ffffffffffffep-1023,
    -0x1.0p-1022
  },
  { // Entry 1634
    0x1.ffffffffffffe0p-1023,
    0x1.0p-1074,
    0x1.ffffffffffffep-1023,
    0x1.ffffffffffffep-1023
  },
  { // Entry 1635
    -0x1.ffffffffffffdfffffffffffffffffffp-1023,
    0x1.0p-1074,
    0x1.ffffffffffffep-1023,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 1636
    0x1.p-1074,
    0x1.0p-1074,
    0x1.ffffffffffffep-1023,
    0x1.0p-1074
  },
  { // Entry 1637
    -0.0,
    0x1.0p-1074,
    0x1.ffffffffffffep-1023,
    -0x1.0p-1074
  },
  { // Entry 1638
    0.0,
    0x1.0p-1074,
    0x1.ffffffffffffep-1023,
    0.0
  },
  { // Entry 1639
    0.0,
    0x1.0p-1074,
    0x1.ffffffffffffep-1023,
    -0.0
  },
  { // Entry 1640
    HUGE_VAL,
    0x1.0p-1074,
    -0x1.ffffffffffffep-1023,
    HUGE_VAL
  },
  { // Entry 1641
    -HUGE_VAL,
    0x1.0p-1074,
    -0x1.ffffffffffffep-1023,
    -HUGE_VAL
  },
  { // Entry 1642
    0x1.ffffffffffffefffffffffffffffffffp1023,
    0x1.0p-1074,
    -0x1.ffffffffffffep-1023,
    0x1.fffffffffffffp1023
  },
  { // Entry 1643
    -0x1.fffffffffffff0p1023,
    0x1.0p-1074,
    -0x1.ffffffffffffep-1023,
    -0x1.fffffffffffffp1023
  },
  { // Entry 1644
    0x1.ffffffffffffffffffffffffffffffffp-1023,
    0x1.0p-1074,
    -0x1.ffffffffffffep-1023,
    0x1.0p-1022
  },
  { // Entry 1645
    -0x1.p-1022,
    0x1.0p-1074,
    -0x1.ffffffffffffep-1023,
    -0x1.0p-1022
  },
  { // Entry 1646
    0x1.ffffffffffffdfffffffffffffffffffp-1023,
    0x1.0p-1074,
    -0x1.ffffffffffffep-1023,
    0x1.ffffffffffffep-1023
  },
  { // Entry 1647
    -0x1.ffffffffffffe0p-1023,
    0x1.0p-1074,
    -0x1.ffffffffffffep-1023,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 1648
    0.0,
    0x1.0p-1074,
    -0x1.ffffffffffffep-1023,
    0x1.0p-1074
  },
  { // Entry 1649
    -0x1.p-1074,
    0x1.0p-1074,
    -0x1.ffffffffffffep-1023,
    -0x1.0p-1074
  },
  { // Entry 1650
    -0.0,
    0x1.0p-1074,
    -0x1.ffffffffffffep-1023,
    0.0
  },
  { // Entry 1651
    -0.0,
    0x1.0p-1074,
    -0x1.ffffffffffffep-1023,
    -0.0
  },
  { // Entry 1652
    HUGE_VAL,
    0x1.0p-1074,
    0x1.0p-1074,
    HUGE_VAL
  },
  { // Entry 1653
    -HUGE_VAL,
    0x1.0p-1074,
    0x1.0p-1074,
    -HUGE_VAL
  },
  { // Entry 1654
    0x1.fffffffffffff0p1023,
    0x1.0p-1074,
    0x1.0p-1074,
    0x1.fffffffffffffp1023
  },
  { // Entry 1655
    -0x1.ffffffffffffefffffffffffffffffffp1023,
    0x1.0p-1074,
    0x1.0p-1074,
    -0x1.fffffffffffffp1023
  },
  { // Entry 1656
    0x1.p-1022,
    0x1.0p-1074,
    0x1.0p-1074,
    0x1.0p-1022
  },
  { // Entry 1657
    -0x1.ffffffffffffffffffffffffffffffffp-1023,
    0x1.0p-1074,
    0x1.0p-1074,
    -0x1.0p-1022
  },
  { // Entry 1658
    0x1.ffffffffffffe0p-1023,
    0x1.0p-1074,
    0x1.0p-1074,
    0x1.ffffffffffffep-1023
  },
  { // Entry 1659
    -0x1.ffffffffffffdfffffffffffffffffffp-1023,
    0x1.0p-1074,
    0x1.0p-1074,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 1660
    0x1.p-1074,
    0x1.0p-1074,
    0x1.0p-1074,
    0x1.0p-1074
  },
  { // Entry 1661
    -0.0,
    0x1.0p-1074,
    0x1.0p-1074,
    -0x1.0p-1074
  },
  { // Entry 1662
    0.0,
    0x1.0p-1074,
    0x1.0p-1074,
    0.0
  },
  { // Entry 1663
    0.0,
    0x1.0p-1074,
    0x1.0p-1074,
    -0.0
  },
  { // Entry 1664
    HUGE_VAL,
    0x1.0p-1074,
    -0x1.0p-1074,
    HUGE_VAL
  },
  { // Entry 1665
    -HUGE_VAL,
    0x1.0p-1074,
    -0x1.0p-1074,
    -HUGE_VAL
  },
  { // Entry 1666
    0x1.ffffffffffffefffffffffffffffffffp1023,
    0x1.0p-1074,
    -0x1.0p-1074,
    0x1.fffffffffffffp1023
  },
  { // Entry 1667
    -0x1.fffffffffffff0p1023,
    0x1.0p-1074,
    -0x1.0p-1074,
    -0x1.fffffffffffffp1023
  },
  { // Entry 1668
    0x1.ffffffffffffffffffffffffffffffffp-1023,
    0x1.0p-1074,
    -0x1.0p-1074,
    0x1.0p-1022
  },
  { // Entry 1669
    -0x1.p-1022,
    0x1.0p-1074,
    -0x1.0p-1074,
    -0x1.0p-1022
  },
  { // Entry 1670
    0x1.ffffffffffffdfffffffffffffffffffp-1023,
    0x1.0p-1074,
    -0x1.0p-1074,
    0x1.ffffffffffffep-1023
  },
  { // Entry 1671
    -0x1.ffffffffffffe0p-1023,
    0x1.0p-1074,
    -0x1.0p-1074,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 1672
    0.0,
    0x1.0p-1074,
    -0x1.0p-1074,
    0x1.0p-1074
  },
  { // Entry 1673
    -0x1.p-1074,
    0x1.0p-1074,
    -0x1.0p-1074,
    -0x1.0p-1074
  },
  { // Entry 1674
    -0.0,
    0x1.0p-1074,
    -0x1.0p-1074,
    0.0
  },
  { // Entry 1675
    -0.0,
    0x1.0p-1074,
    -0x1.0p-1074,
    -0.0
  },
  { // Entry 1676
    HUGE_VAL,
    0x1.0p-1074,
    0.0,
    HUGE_VAL
  },
  { // Entry 1677
    -HUGE_VAL,
    0x1.0p-1074,
    0.0,
    -HUGE_VAL
  },
  { // Entry 1678
    0x1.fffffffffffff0p1023,
    0x1.0p-1074,
    0.0,
    0x1.fffffffffffffp1023
  },
  { // Entry 1679
    -0x1.fffffffffffff0p1023,
    0x1.0p-1074,
    0.0,
    -0x1.fffffffffffffp1023
  },
  { // Entry 1680
    0x1.p-1022,
    0x1.0p-1074,
    0.0,
    0x1.0p-1022
  },
  { // Entry 1681
    -0x1.p-1022,
    0x1.0p-1074,
    0.0,
    -0x1.0p-1022
  },
  { // Entry 1682
    0x1.ffffffffffffe0p-1023,
    0x1.0p-1074,
    0.0,
    0x1.ffffffffffffep-1023
  },
  { // Entry 1683
    -0x1.ffffffffffffe0p-1023,
    0x1.0p-1074,
    0.0,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 1684
    0x1.p-1074,
    0x1.0p-1074,
    0.0,
    0x1.0p-1074
  },
  { // Entry 1685
    -0x1.p-1074,
    0x1.0p-1074,
    0.0,
    -0x1.0p-1074
  },
  { // Entry 1686
    0.0,
    0x1.0p-1074,
    0.0,
    0.0
  },
  { // Entry 1687
    0.0,
    0x1.0p-1074,
    0.0,
    -0.0
  },
  { // Entry 1688
    HUGE_VAL,
    0x1.0p-1074,
    -0.0,
    HUGE_VAL
  },
  { // Entry 1689
    -HUGE_VAL,
    0x1.0p-1074,
    -0.0,
    -HUGE_VAL
  },
  { // Entry 1690
    0x1.fffffffffffff0p1023,
    0x1.0p-1074,
    -0.0,
    0x1.fffffffffffffp1023
  },
  { // Entry 1691
    -0x1.fffffffffffff0p1023,
    0x1.0p-1074,
    -0.0,
    -0x1.fffffffffffffp1023
  },
  { // Entry 1692
    0x1.p-1022,
    0x1.0p-1074,
    -0.0,
    0x1.0p-1022
  },
  { // Entry 1693
    -0x1.p-1022,
    0x1.0p-1074,
    -0.0,
    -0x1.0p-1022
  },
  { // Entry 1694
    0x1.ffffffffffffe0p-1023,
    0x1.0p-1074,
    -0.0,
    0x1.ffffffffffffep-1023
  },
  { // Entry 1695
    -0x1.ffffffffffffe0p-1023,
    0x1.0p-1074,
    -0.0,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 1696
    0x1.p-1074,
    0x1.0p-1074,
    -0.0,
    0x1.0p-1074
  },
  { // Entry 1697
    -0x1.p-1074,
    0x1.0p-1074,
    -0.0,
    -0x1.0p-1074
  },
  { // Entry 1698
    0.0,
    0x1.0p-1074,
    -0.0,
    0.0
  },
  { // Entry 1699
    -0.0,
    0x1.0p-1074,
    -0.0,
    -0.0
  },
  { // Entry 1700
    -HUGE_VAL,
    -0x1.0p-1074,
    HUGE_VAL,
    -HUGE_VAL
  },
  { // Entry 1701
    -HUGE_VAL,
    -0x1.0p-1074,
    HUGE_VAL,
    0x1.fffffffffffffp1023
  },
  { // Entry 1702
    -HUGE_VAL,
    -0x1.0p-1074,
    HUGE_VAL,
    -0x1.fffffffffffffp1023
  },
  { // Entry 1703
    -HUGE_VAL,
    -0x1.0p-1074,
    HUGE_VAL,
    0x1.0p-1022
  },
  { // Entry 1704
    -HUGE_VAL,
    -0x1.0p-1074,
    HUGE_VAL,
    -0x1.0p-1022
  },
  { // Entry 1705
    -HUGE_VAL,
    -0x1.0p-1074,
    HUGE_VAL,
    0x1.ffffffffffffep-1023
  },
  { // Entry 1706
    -HUGE_VAL,
    -0x1.0p-1074,
    HUGE_VAL,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 1707
    -HUGE_VAL,
    -0x1.0p-1074,
    HUGE_VAL,
    0x1.0p-1074
  },
  { // Entry 1708
    -HUGE_VAL,
    -0x1.0p-1074,
    HUGE_VAL,
    -0x1.0p-1074
  },
  { // Entry 1709
    -HUGE_VAL,
    -0x1.0p-1074,
    HUGE_VAL,
    0.0
  },
  { // Entry 1710
    -HUGE_VAL,
    -0x1.0p-1074,
    HUGE_VAL,
    -0.0
  },
  { // Entry 1711
    HUGE_VAL,
    -0x1.0p-1074,
    -HUGE_VAL,
    HUGE_VAL
  },
  { // Entry 1712
    HUGE_VAL,
    -0x1.0p-1074,
    -HUGE_VAL,
    0x1.fffffffffffffp1023
  },
  { // Entry 1713
    HUGE_VAL,
    -0x1.0p-1074,
    -HUGE_VAL,
    -0x1.fffffffffffffp1023
  },
  { // Entry 1714
    HUGE_VAL,
    -0x1.0p-1074,
    -HUGE_VAL,
    0x1.0p-1022
  },
  { // Entry 1715
    HUGE_VAL,
    -0x1.0p-1074,
    -HUGE_VAL,
    -0x1.0p-1022
  },
  { // Entry 1716
    HUGE_VAL,
    -0x1.0p-1074,
    -HUGE_VAL,
    0x1.ffffffffffffep-1023
  },
  { // Entry 1717
    HUGE_VAL,
    -0x1.0p-1074,
    -HUGE_VAL,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 1718
    HUGE_VAL,
    -0x1.0p-1074,
    -HUGE_VAL,
    0x1.0p-1074
  },
  { // Entry 1719
    HUGE_VAL,
    -0x1.0p-1074,
    -HUGE_VAL,
    -0x1.0p-1074
  },
  { // Entry 1720
    HUGE_VAL,
    -0x1.0p-1074,
    -HUGE_VAL,
    0.0
  },
  { // Entry 1721
    HUGE_VAL,
    -0x1.0p-1074,
    -HUGE_VAL,
    -0.0
  },
  { // Entry 1722
    HUGE_VAL,
    -0x1.0p-1074,
    0x1.fffffffffffffp1023,
    HUGE_VAL
  },
  { // Entry 1723
    -HUGE_VAL,
    -0x1.0p-1074,
    0x1.fffffffffffffp1023,
    -HUGE_VAL
  },
  { // Entry 1724
    0x1.ffffffffffffefffffffffffffffffffp1023,
    -0x1.0p-1074,
    0x1.fffffffffffffp1023,
    0x1.fffffffffffffp1023
  },
  { // Entry 1725
    -0x1.fffffffffffff0p1023,
    -0x1.0p-1074,
    0x1.fffffffffffffp1023,
    -0x1.fffffffffffffp1023
  },
  { // Entry 1726
    -0x1.ffffffffffffefffffffffffffffffffp-51,
    -0x1.0p-1074,
    0x1.fffffffffffffp1023,
    0x1.0p-1022
  },
  { // Entry 1727
    -0x1.fffffffffffff0p-51,
    -0x1.0p-1074,
    0x1.fffffffffffffp1023,
    -0x1.0p-1022
  },
  { // Entry 1728
    -0x1.ffffffffffffefffffffffffffffffffp-51,
    -0x1.0p-1074,
    0x1.fffffffffffffp1023,
    0x1.ffffffffffffep-1023
  },
  { // Entry 1729
    -0x1.fffffffffffff0p-51,
    -0x1.0p-1074,
    0x1.fffffffffffffp1023,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 1730
    -0x1.ffffffffffffefffffffffffffffffffp-51,
    -0x1.0p-1074,
    0x1.fffffffffffffp1023,
    0x1.0p-1074
  },
  { // Entry 1731
    -0x1.fffffffffffff0p-51,
    -0x1.0p-1074,
    0x1.fffffffffffffp1023,
    -0x1.0p-1074
  },
  { // Entry 1732
    -0x1.fffffffffffff0p-51,
    -0x1.0p-1074,
    0x1.fffffffffffffp1023,
    0.0
  },
  { // Entry 1733
    -0x1.fffffffffffff0p-51,
    -0x1.0p-1074,
    0x1.fffffffffffffp1023,
    -0.0
  },
  { // Entry 1734
    HUGE_VAL,
    -0x1.0p-1074,
    -0x1.fffffffffffffp1023,
    HUGE_VAL
  },
  { // Entry 1735
    -HUGE_VAL,
    -0x1.0p-1074,
    -0x1.fffffffffffffp1023,
    -HUGE_VAL
  },
  { // Entry 1736
    0x1.fffffffffffff0p1023,
    -0x1.0p-1074,
    -0x1.fffffffffffffp1023,
    0x1.fffffffffffffp1023
  },
  { // Entry 1737
    -0x1.ffffffffffffefffffffffffffffffffp1023,
    -0x1.0p-1074,
    -0x1.fffffffffffffp1023,
    -0x1.fffffffffffffp1023
  },
  { // Entry 1738
    0x1.fffffffffffff0p-51,
    -0x1.0p-1074,
    -0x1.fffffffffffffp1023,
    0x1.0p-1022
  },
  { // Entry 1739
    0x1.ffffffffffffefffffffffffffffffffp-51,
    -0x1.0p-1074,
    -0x1.fffffffffffffp1023,
    -0x1.0p-1022
  },
  { // Entry 1740
    0x1.fffffffffffff0p-51,
    -0x1.0p-1074,
    -0x1.fffffffffffffp1023,
    0x1.ffffffffffffep-1023
  },
  { // Entry 1741
    0x1.ffffffffffffefffffffffffffffffffp-51,
    -0x1.0p-1074,
    -0x1.fffffffffffffp1023,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 1742
    0x1.fffffffffffff0p-51,
    -0x1.0p-1074,
    -0x1.fffffffffffffp1023,
    0x1.0p-1074
  },
  { // Entry 1743
    0x1.ffffffffffffefffffffffffffffffffp-51,
    -0x1.0p-1074,
    -0x1.fffffffffffffp1023,
    -0x1.0p-1074
  },
  { // Entry 1744
    0x1.fffffffffffff0p-51,
    -0x1.0p-1074,
    -0x1.fffffffffffffp1023,
    0.0
  },
  { // Entry 1745
    0x1.fffffffffffff0p-51,
    -0x1.0p-1074,
    -0x1.fffffffffffffp1023,
    -0.0
  },
  { // Entry 1746
    HUGE_VAL,
    -0x1.0p-1074,
    0x1.0p-1022,
    HUGE_VAL
  },
  { // Entry 1747
    -HUGE_VAL,
    -0x1.0p-1074,
    0x1.0p-1022,
    -HUGE_VAL
  },
  { // Entry 1748
    0x1.ffffffffffffefffffffffffffffffffp1023,
    -0x1.0p-1074,
    0x1.0p-1022,
    0x1.fffffffffffffp1023
  },
  { // Entry 1749
    -0x1.fffffffffffff0p1023,
    -0x1.0p-1074,
    0x1.0p-1022,
    -0x1.fffffffffffffp1023
  },
  { // Entry 1750
    0x1.ffffffffffffffffffffffffffffffffp-1023,
    -0x1.0p-1074,
    0x1.0p-1022,
    0x1.0p-1022
  },
  { // Entry 1751
    -0x1.p-1022,
    -0x1.0p-1074,
    0x1.0p-1022,
    -0x1.0p-1022
  },
  { // Entry 1752
    0x1.ffffffffffffdfffffffffffffffffffp-1023,
    -0x1.0p-1074,
    0x1.0p-1022,
    0x1.ffffffffffffep-1023
  },
  { // Entry 1753
    -0x1.ffffffffffffe0p-1023,
    -0x1.0p-1074,
    0x1.0p-1022,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 1754
    0.0,
    -0x1.0p-1074,
    0x1.0p-1022,
    0x1.0p-1074
  },
  { // Entry 1755
    -0x1.p-1074,
    -0x1.0p-1074,
    0x1.0p-1022,
    -0x1.0p-1074
  },
  { // Entry 1756
    -0.0,
    -0x1.0p-1074,
    0x1.0p-1022,
    0.0
  },
  { // Entry 1757
    -0.0,
    -0x1.0p-1074,
    0x1.0p-1022,
    -0.0
  },
  { // Entry 1758
    HUGE_VAL,
    -0x1.0p-1074,
    -0x1.0p-1022,
    HUGE_VAL
  },
  { // Entry 1759
    -HUGE_VAL,
    -0x1.0p-1074,
    -0x1.0p-1022,
    -HUGE_VAL
  },
  { // Entry 1760
    0x1.fffffffffffff0p1023,
    -0x1.0p-1074,
    -0x1.0p-1022,
    0x1.fffffffffffffp1023
  },
  { // Entry 1761
    -0x1.ffffffffffffefffffffffffffffffffp1023,
    -0x1.0p-1074,
    -0x1.0p-1022,
    -0x1.fffffffffffffp1023
  },
  { // Entry 1762
    0x1.p-1022,
    -0x1.0p-1074,
    -0x1.0p-1022,
    0x1.0p-1022
  },
  { // Entry 1763
    -0x1.ffffffffffffffffffffffffffffffffp-1023,
    -0x1.0p-1074,
    -0x1.0p-1022,
    -0x1.0p-1022
  },
  { // Entry 1764
    0x1.ffffffffffffe0p-1023,
    -0x1.0p-1074,
    -0x1.0p-1022,
    0x1.ffffffffffffep-1023
  },
  { // Entry 1765
    -0x1.ffffffffffffdfffffffffffffffffffp-1023,
    -0x1.0p-1074,
    -0x1.0p-1022,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 1766
    0x1.p-1074,
    -0x1.0p-1074,
    -0x1.0p-1022,
    0x1.0p-1074
  },
  { // Entry 1767
    -0.0,
    -0x1.0p-1074,
    -0x1.0p-1022,
    -0x1.0p-1074
  },
  { // Entry 1768
    0.0,
    -0x1.0p-1074,
    -0x1.0p-1022,
    0.0
  },
  { // Entry 1769
    0.0,
    -0x1.0p-1074,
    -0x1.0p-1022,
    -0.0
  },
  { // Entry 1770
    HUGE_VAL,
    -0x1.0p-1074,
    0x1.ffffffffffffep-1023,
    HUGE_VAL
  },
  { // Entry 1771
    -HUGE_VAL,
    -0x1.0p-1074,
    0x1.ffffffffffffep-1023,
    -HUGE_VAL
  },
  { // Entry 1772
    0x1.ffffffffffffefffffffffffffffffffp1023,
    -0x1.0p-1074,
    0x1.ffffffffffffep-1023,
    0x1.fffffffffffffp1023
  },
  { // Entry 1773
    -0x1.fffffffffffff0p1023,
    -0x1.0p-1074,
    0x1.ffffffffffffep-1023,
    -0x1.fffffffffffffp1023
  },
  { // Entry 1774
    0x1.ffffffffffffffffffffffffffffffffp-1023,
    -0x1.0p-1074,
    0x1.ffffffffffffep-1023,
    0x1.0p-1022
  },
  { // Entry 1775
    -0x1.p-1022,
    -0x1.0p-1074,
    0x1.ffffffffffffep-1023,
    -0x1.0p-1022
  },
  { // Entry 1776
    0x1.ffffffffffffdfffffffffffffffffffp-1023,
    -0x1.0p-1074,
    0x1.ffffffffffffep-1023,
    0x1.ffffffffffffep-1023
  },
  { // Entry 1777
    -0x1.ffffffffffffe0p-1023,
    -0x1.0p-1074,
    0x1.ffffffffffffep-1023,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 1778
    0.0,
    -0x1.0p-1074,
    0x1.ffffffffffffep-1023,
    0x1.0p-1074
  },
  { // Entry 1779
    -0x1.p-1074,
    -0x1.0p-1074,
    0x1.ffffffffffffep-1023,
    -0x1.0p-1074
  },
  { // Entry 1780
    -0.0,
    -0x1.0p-1074,
    0x1.ffffffffffffep-1023,
    0.0
  },
  { // Entry 1781
    -0.0,
    -0x1.0p-1074,
    0x1.ffffffffffffep-1023,
    -0.0
  },
  { // Entry 1782
    HUGE_VAL,
    -0x1.0p-1074,
    -0x1.ffffffffffffep-1023,
    HUGE_VAL
  },
  { // Entry 1783
    -HUGE_VAL,
    -0x1.0p-1074,
    -0x1.ffffffffffffep-1023,
    -HUGE_VAL
  },
  { // Entry 1784
    0x1.fffffffffffff0p1023,
    -0x1.0p-1074,
    -0x1.ffffffffffffep-1023,
    0x1.fffffffffffffp1023
  },
  { // Entry 1785
    -0x1.ffffffffffffefffffffffffffffffffp1023,
    -0x1.0p-1074,
    -0x1.ffffffffffffep-1023,
    -0x1.fffffffffffffp1023
  },
  { // Entry 1786
    0x1.p-1022,
    -0x1.0p-1074,
    -0x1.ffffffffffffep-1023,
    0x1.0p-1022
  },
  { // Entry 1787
    -0x1.ffffffffffffffffffffffffffffffffp-1023,
    -0x1.0p-1074,
    -0x1.ffffffffffffep-1023,
    -0x1.0p-1022
  },
  { // Entry 1788
    0x1.ffffffffffffe0p-1023,
    -0x1.0p-1074,
    -0x1.ffffffffffffep-1023,
    0x1.ffffffffffffep-1023
  },
  { // Entry 1789
    -0x1.ffffffffffffdfffffffffffffffffffp-1023,
    -0x1.0p-1074,
    -0x1.ffffffffffffep-1023,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 1790
    0x1.p-1074,
    -0x1.0p-1074,
    -0x1.ffffffffffffep-1023,
    0x1.0p-1074
  },
  { // Entry 1791
    -0.0,
    -0x1.0p-1074,
    -0x1.ffffffffffffep-1023,
    -0x1.0p-1074
  },
  { // Entry 1792
    0.0,
    -0x1.0p-1074,
    -0x1.ffffffffffffep-1023,
    0.0
  },
  { // Entry 1793
    0.0,
    -0x1.0p-1074,
    -0x1.ffffffffffffep-1023,
    -0.0
  },
  { // Entry 1794
    HUGE_VAL,
    -0x1.0p-1074,
    0x1.0p-1074,
    HUGE_VAL
  },
  { // Entry 1795
    -HUGE_VAL,
    -0x1.0p-1074,
    0x1.0p-1074,
    -HUGE_VAL
  },
  { // Entry 1796
    0x1.ffffffffffffefffffffffffffffffffp1023,
    -0x1.0p-1074,
    0x1.0p-1074,
    0x1.fffffffffffffp1023
  },
  { // Entry 1797
    -0x1.fffffffffffff0p1023,
    -0x1.0p-1074,
    0x1.0p-1074,
    -0x1.fffffffffffffp1023
  },
  { // Entry 1798
    0x1.ffffffffffffffffffffffffffffffffp-1023,
    -0x1.0p-1074,
    0x1.0p-1074,
    0x1.0p-1022
  },
  { // Entry 1799
    -0x1.p-1022,
    -0x1.0p-1074,
    0x1.0p-1074,
    -0x1.0p-1022
  },
  { // Entry 1800
    0x1.ffffffffffffdfffffffffffffffffffp-1023,
    -0x1.0p-1074,
    0x1.0p-1074,
    0x1.ffffffffffffep-1023
  },
  { // Entry 1801
    -0x1.ffffffffffffe0p-1023,
    -0x1.0p-1074,
    0x1.0p-1074,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 1802
    0.0,
    -0x1.0p-1074,
    0x1.0p-1074,
    0x1.0p-1074
  },
  { // Entry 1803
    -0x1.p-1074,
    -0x1.0p-1074,
    0x1.0p-1074,
    -0x1.0p-1074
  },
  { // Entry 1804
    -0.0,
    -0x1.0p-1074,
    0x1.0p-1074,
    0.0
  },
  { // Entry 1805
    -0.0,
    -0x1.0p-1074,
    0x1.0p-1074,
    -0.0
  },
  { // Entry 1806
    HUGE_VAL,
    -0x1.0p-1074,
    -0x1.0p-1074,
    HUGE_VAL
  },
  { // Entry 1807
    -HUGE_VAL,
    -0x1.0p-1074,
    -0x1.0p-1074,
    -HUGE_VAL
  },
  { // Entry 1808
    0x1.fffffffffffff0p1023,
    -0x1.0p-1074,
    -0x1.0p-1074,
    0x1.fffffffffffffp1023
  },
  { // Entry 1809
    -0x1.ffffffffffffefffffffffffffffffffp1023,
    -0x1.0p-1074,
    -0x1.0p-1074,
    -0x1.fffffffffffffp1023
  },
  { // Entry 1810
    0x1.p-1022,
    -0x1.0p-1074,
    -0x1.0p-1074,
    0x1.0p-1022
  },
  { // Entry 1811
    -0x1.ffffffffffffffffffffffffffffffffp-1023,
    -0x1.0p-1074,
    -0x1.0p-1074,
    -0x1.0p-1022
  },
  { // Entry 1812
    0x1.ffffffffffffe0p-1023,
    -0x1.0p-1074,
    -0x1.0p-1074,
    0x1.ffffffffffffep-1023
  },
  { // Entry 1813
    -0x1.ffffffffffffdfffffffffffffffffffp-1023,
    -0x1.0p-1074,
    -0x1.0p-1074,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 1814
    0x1.p-1074,
    -0x1.0p-1074,
    -0x1.0p-1074,
    0x1.0p-1074
  },
  { // Entry 1815
    -0.0,
    -0x1.0p-1074,
    -0x1.0p-1074,
    -0x1.0p-1074
  },
  { // Entry 1816
    0.0,
    -0x1.0p-1074,
    -0x1.0p-1074,
    0.0
  },
  { // Entry 1817
    0.0,
    -0x1.0p-1074,
    -0x1.0p-1074,
    -0.0
  },
  { // Entry 1818
    HUGE_VAL,
    -0x1.0p-1074,
    0.0,
    HUGE_VAL
  },
  { // Entry 1819
    -HUGE_VAL,
    -0x1.0p-1074,
    0.0,
    -HUGE_VAL
  },
  { // Entry 1820
    0x1.fffffffffffff0p1023,
    -0x1.0p-1074,
    0.0,
    0x1.fffffffffffffp1023
  },
  { // Entry 1821
    -0x1.fffffffffffff0p1023,
    -0x1.0p-1074,
    0.0,
    -0x1.fffffffffffffp1023
  },
  { // Entry 1822
    0x1.p-1022,
    -0x1.0p-1074,
    0.0,
    0x1.0p-1022
  },
  { // Entry 1823
    -0x1.p-1022,
    -0x1.0p-1074,
    0.0,
    -0x1.0p-1022
  },
  { // Entry 1824
    0x1.ffffffffffffe0p-1023,
    -0x1.0p-1074,
    0.0,
    0x1.ffffffffffffep-1023
  },
  { // Entry 1825
    -0x1.ffffffffffffe0p-1023,
    -0x1.0p-1074,
    0.0,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 1826
    0x1.p-1074,
    -0x1.0p-1074,
    0.0,
    0x1.0p-1074
  },
  { // Entry 1827
    -0x1.p-1074,
    -0x1.0p-1074,
    0.0,
    -0x1.0p-1074
  },
  { // Entry 1828
    0.0,
    -0x1.0p-1074,
    0.0,
    0.0
  },
  { // Entry 1829
    -0.0,
    -0x1.0p-1074,
    0.0,
    -0.0
  },
  { // Entry 1830
    HUGE_VAL,
    -0x1.0p-1074,
    -0.0,
    HUGE_VAL
  },
  { // Entry 1831
    -HUGE_VAL,
    -0x1.0p-1074,
    -0.0,
    -HUGE_VAL
  },
  { // Entry 1832
    0x1.fffffffffffff0p1023,
    -0x1.0p-1074,
    -0.0,
    0x1.fffffffffffffp1023
  },
  { // Entry 1833
    -0x1.fffffffffffff0p1023,
    -0x1.0p-1074,
    -0.0,
    -0x1.fffffffffffffp1023
  },
  { // Entry 1834
    0x1.p-1022,
    -0x1.0p-1074,
    -0.0,
    0x1.0p-1022
  },
  { // Entry 1835
    -0x1.p-1022,
    -0x1.0p-1074,
    -0.0,
    -0x1.0p-1022
  },
  { // Entry 1836
    0x1.ffffffffffffe0p-1023,
    -0x1.0p-1074,
    -0.0,
    0x1.ffffffffffffep-1023
  },
  { // Entry 1837
    -0x1.ffffffffffffe0p-1023,
    -0x1.0p-1074,
    -0.0,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 1838
    0x1.p-1074,
    -0x1.0p-1074,
    -0.0,
    0x1.0p-1074
  },
  { // Entry 1839
    -0x1.p-1074,
    -0x1.0p-1074,
    -0.0,
    -0x1.0p-1074
  },
  { // Entry 1840
    0.0,
    -0x1.0p-1074,
    -0.0,
    0.0
  },
  { // Entry 1841
    0.0,
    -0x1.0p-1074,
    -0.0,
    -0.0
  },
  { // Entry 1842
    HUGE_VAL,
    0.0,
    0x1.fffffffffffffp1023,
    HUGE_VAL
  },
  { // Entry 1843
    -HUGE_VAL,
    0.0,
    0x1.fffffffffffffp1023,
    -HUGE_VAL
  },
  { // Entry 1844
    0x1.fffffffffffff0p1023,
    0.0,
    0x1.fffffffffffffp1023,
    0x1.fffffffffffffp1023
  },
  { // Entry 1845
    -0x1.fffffffffffff0p1023,
    0.0,
    0x1.fffffffffffffp1023,
    -0x1.fffffffffffffp1023
  },
  { // Entry 1846
    0x1.p-1022,
    0.0,
    0x1.fffffffffffffp1023,
    0x1.0p-1022
  },
  { // Entry 1847
    -0x1.p-1022,
    0.0,
    0x1.fffffffffffffp1023,
    -0x1.0p-1022
  },
  { // Entry 1848
    0x1.ffffffffffffe0p-1023,
    0.0,
    0x1.fffffffffffffp1023,
    0x1.ffffffffffffep-1023
  },
  { // Entry 1849
    -0x1.ffffffffffffe0p-1023,
    0.0,
    0x1.fffffffffffffp1023,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 1850
    0x1.p-1074,
    0.0,
    0x1.fffffffffffffp1023,
    0x1.0p-1074
  },
  { // Entry 1851
    -0x1.p-1074,
    0.0,
    0x1.fffffffffffffp1023,
    -0x1.0p-1074
  },
  { // Entry 1852
    0.0,
    0.0,
    0x1.fffffffffffffp1023,
    0.0
  },
  { // Entry 1853
    0.0,
    0.0,
    0x1.fffffffffffffp1023,
    -0.0
  },
  { // Entry 1854
    HUGE_VAL,
    0.0,
    -0x1.fffffffffffffp1023,
    HUGE_VAL
  },
  { // Entry 1855
    -HUGE_VAL,
    0.0,
    -0x1.fffffffffffffp1023,
    -HUGE_VAL
  },
  { // Entry 1856
    0x1.fffffffffffff0p1023,
    0.0,
    -0x1.fffffffffffffp1023,
    0x1.fffffffffffffp1023
  },
  { // Entry 1857
    -0x1.fffffffffffff0p1023,
    0.0,
    -0x1.fffffffffffffp1023,
    -0x1.fffffffffffffp1023
  },
  { // Entry 1858
    0x1.p-1022,
    0.0,
    -0x1.fffffffffffffp1023,
    0x1.0p-1022
  },
  { // Entry 1859
    -0x1.p-1022,
    0.0,
    -0x1.fffffffffffffp1023,
    -0x1.0p-1022
  },
  { // Entry 1860
    0x1.ffffffffffffe0p-1023,
    0.0,
    -0x1.fffffffffffffp1023,
    0x1.ffffffffffffep-1023
  },
  { // Entry 1861
    -0x1.ffffffffffffe0p-1023,
    0.0,
    -0x1.fffffffffffffp1023,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 1862
    0x1.p-1074,
    0.0,
    -0x1.fffffffffffffp1023,
    0x1.0p-1074
  },
  { // Entry 1863
    -0x1.p-1074,
    0.0,
    -0x1.fffffffffffffp1023,
    -0x1.0p-1074
  },
  { // Entry 1864
    0.0,
    0.0,
    -0x1.fffffffffffffp1023,
    0.0
  },
  { // Entry 1865
    -0.0,
    0.0,
    -0x1.fffffffffffffp1023,
    -0.0
  },
  { // Entry 1866
    HUGE_VAL,
    0.0,
    0x1.0p-1022,
    HUGE_VAL
  },
  { // Entry 1867
    -HUGE_VAL,
    0.0,
    0x1.0p-1022,
    -HUGE_VAL
  },
  { // Entry 1868
    0x1.fffffffffffff0p1023,
    0.0,
    0x1.0p-1022,
    0x1.fffffffffffffp1023
  },
  { // Entry 1869
    -0x1.fffffffffffff0p1023,
    0.0,
    0x1.0p-1022,
    -0x1.fffffffffffffp1023
  },
  { // Entry 1870
    0x1.p-1022,
    0.0,
    0x1.0p-1022,
    0x1.0p-1022
  },
  { // Entry 1871
    -0x1.p-1022,
    0.0,
    0x1.0p-1022,
    -0x1.0p-1022
  },
  { // Entry 1872
    0x1.ffffffffffffe0p-1023,
    0.0,
    0x1.0p-1022,
    0x1.ffffffffffffep-1023
  },
  { // Entry 1873
    -0x1.ffffffffffffe0p-1023,
    0.0,
    0x1.0p-1022,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 1874
    0x1.p-1074,
    0.0,
    0x1.0p-1022,
    0x1.0p-1074
  },
  { // Entry 1875
    -0x1.p-1074,
    0.0,
    0x1.0p-1022,
    -0x1.0p-1074
  },
  { // Entry 1876
    0.0,
    0.0,
    0x1.0p-1022,
    0.0
  },
  { // Entry 1877
    0.0,
    0.0,
    0x1.0p-1022,
    -0.0
  },
  { // Entry 1878
    HUGE_VAL,
    0.0,
    -0x1.0p-1022,
    HUGE_VAL
  },
  { // Entry 1879
    -HUGE_VAL,
    0.0,
    -0x1.0p-1022,
    -HUGE_VAL
  },
  { // Entry 1880
    0x1.fffffffffffff0p1023,
    0.0,
    -0x1.0p-1022,
    0x1.fffffffffffffp1023
  },
  { // Entry 1881
    -0x1.fffffffffffff0p1023,
    0.0,
    -0x1.0p-1022,
    -0x1.fffffffffffffp1023
  },
  { // Entry 1882
    0x1.p-1022,
    0.0,
    -0x1.0p-1022,
    0x1.0p-1022
  },
  { // Entry 1883
    -0x1.p-1022,
    0.0,
    -0x1.0p-1022,
    -0x1.0p-1022
  },
  { // Entry 1884
    0x1.ffffffffffffe0p-1023,
    0.0,
    -0x1.0p-1022,
    0x1.ffffffffffffep-1023
  },
  { // Entry 1885
    -0x1.ffffffffffffe0p-1023,
    0.0,
    -0x1.0p-1022,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 1886
    0x1.p-1074,
    0.0,
    -0x1.0p-1022,
    0x1.0p-1074
  },
  { // Entry 1887
    -0x1.p-1074,
    0.0,
    -0x1.0p-1022,
    -0x1.0p-1074
  },
  { // Entry 1888
    0.0,
    0.0,
    -0x1.0p-1022,
    0.0
  },
  { // Entry 1889
    -0.0,
    0.0,
    -0x1.0p-1022,
    -0.0
  },
  { // Entry 1890
    HUGE_VAL,
    0.0,
    0x1.ffffffffffffep-1023,
    HUGE_VAL
  },
  { // Entry 1891
    -HUGE_VAL,
    0.0,
    0x1.ffffffffffffep-1023,
    -HUGE_VAL
  },
  { // Entry 1892
    0x1.fffffffffffff0p1023,
    0.0,
    0x1.ffffffffffffep-1023,
    0x1.fffffffffffffp1023
  },
  { // Entry 1893
    -0x1.fffffffffffff0p1023,
    0.0,
    0x1.ffffffffffffep-1023,
    -0x1.fffffffffffffp1023
  },
  { // Entry 1894
    0x1.p-1022,
    0.0,
    0x1.ffffffffffffep-1023,
    0x1.0p-1022
  },
  { // Entry 1895
    -0x1.p-1022,
    0.0,
    0x1.ffffffffffffep-1023,
    -0x1.0p-1022
  },
  { // Entry 1896
    0x1.ffffffffffffe0p-1023,
    0.0,
    0x1.ffffffffffffep-1023,
    0x1.ffffffffffffep-1023
  },
  { // Entry 1897
    -0x1.ffffffffffffe0p-1023,
    0.0,
    0x1.ffffffffffffep-1023,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 1898
    0x1.p-1074,
    0.0,
    0x1.ffffffffffffep-1023,
    0x1.0p-1074
  },
  { // Entry 1899
    -0x1.p-1074,
    0.0,
    0x1.ffffffffffffep-1023,
    -0x1.0p-1074
  },
  { // Entry 1900
    0.0,
    0.0,
    0x1.ffffffffffffep-1023,
    0.0
  },
  { // Entry 1901
    0.0,
    0.0,
    0x1.ffffffffffffep-1023,
    -0.0
  },
  { // Entry 1902
    HUGE_VAL,
    0.0,
    -0x1.ffffffffffffep-1023,
    HUGE_VAL
  },
  { // Entry 1903
    -HUGE_VAL,
    0.0,
    -0x1.ffffffffffffep-1023,
    -HUGE_VAL
  },
  { // Entry 1904
    0x1.fffffffffffff0p1023,
    0.0,
    -0x1.ffffffffffffep-1023,
    0x1.fffffffffffffp1023
  },
  { // Entry 1905
    -0x1.fffffffffffff0p1023,
    0.0,
    -0x1.ffffffffffffep-1023,
    -0x1.fffffffffffffp1023
  },
  { // Entry 1906
    0x1.p-1022,
    0.0,
    -0x1.ffffffffffffep-1023,
    0x1.0p-1022
  },
  { // Entry 1907
    -0x1.p-1022,
    0.0,
    -0x1.ffffffffffffep-1023,
    -0x1.0p-1022
  },
  { // Entry 1908
    0x1.ffffffffffffe0p-1023,
    0.0,
    -0x1.ffffffffffffep-1023,
    0x1.ffffffffffffep-1023
  },
  { // Entry 1909
    -0x1.ffffffffffffe0p-1023,
    0.0,
    -0x1.ffffffffffffep-1023,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 1910
    0x1.p-1074,
    0.0,
    -0x1.ffffffffffffep-1023,
    0x1.0p-1074
  },
  { // Entry 1911
    -0x1.p-1074,
    0.0,
    -0x1.ffffffffffffep-1023,
    -0x1.0p-1074
  },
  { // Entry 1912
    0.0,
    0.0,
    -0x1.ffffffffffffep-1023,
    0.0
  },
  { // Entry 1913
    -0.0,
    0.0,
    -0x1.ffffffffffffep-1023,
    -0.0
  },
  { // Entry 1914
    HUGE_VAL,
    0.0,
    0x1.0p-1074,
    HUGE_VAL
  },
  { // Entry 1915
    -HUGE_VAL,
    0.0,
    0x1.0p-1074,
    -HUGE_VAL
  },
  { // Entry 1916
    0x1.fffffffffffff0p1023,
    0.0,
    0x1.0p-1074,
    0x1.fffffffffffffp1023
  },
  { // Entry 1917
    -0x1.fffffffffffff0p1023,
    0.0,
    0x1.0p-1074,
    -0x1.fffffffffffffp1023
  },
  { // Entry 1918
    0x1.p-1022,
    0.0,
    0x1.0p-1074,
    0x1.0p-1022
  },
  { // Entry 1919
    -0x1.p-1022,
    0.0,
    0x1.0p-1074,
    -0x1.0p-1022
  },
  { // Entry 1920
    0x1.ffffffffffffe0p-1023,
    0.0,
    0x1.0p-1074,
    0x1.ffffffffffffep-1023
  },
  { // Entry 1921
    -0x1.ffffffffffffe0p-1023,
    0.0,
    0x1.0p-1074,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 1922
    0x1.p-1074,
    0.0,
    0x1.0p-1074,
    0x1.0p-1074
  },
  { // Entry 1923
    -0x1.p-1074,
    0.0,
    0x1.0p-1074,
    -0x1.0p-1074
  },
  { // Entry 1924
    0.0,
    0.0,
    0x1.0p-1074,
    0.0
  },
  { // Entry 1925
    0.0,
    0.0,
    0x1.0p-1074,
    -0.0
  },
  { // Entry 1926
    HUGE_VAL,
    0.0,
    -0x1.0p-1074,
    HUGE_VAL
  },
  { // Entry 1927
    -HUGE_VAL,
    0.0,
    -0x1.0p-1074,
    -HUGE_VAL
  },
  { // Entry 1928
    0x1.fffffffffffff0p1023,
    0.0,
    -0x1.0p-1074,
    0x1.fffffffffffffp1023
  },
  { // Entry 1929
    -0x1.fffffffffffff0p1023,
    0.0,
    -0x1.0p-1074,
    -0x1.fffffffffffffp1023
  },
  { // Entry 1930
    0x1.p-1022,
    0.0,
    -0x1.0p-1074,
    0x1.0p-1022
  },
  { // Entry 1931
    -0x1.p-1022,
    0.0,
    -0x1.0p-1074,
    -0x1.0p-1022
  },
  { // Entry 1932
    0x1.ffffffffffffe0p-1023,
    0.0,
    -0x1.0p-1074,
    0x1.ffffffffffffep-1023
  },
  { // Entry 1933
    -0x1.ffffffffffffe0p-1023,
    0.0,
    -0x1.0p-1074,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 1934
    0x1.p-1074,
    0.0,
    -0x1.0p-1074,
    0x1.0p-1074
  },
  { // Entry 1935
    -0x1.p-1074,
    0.0,
    -0x1.0p-1074,
    -0x1.0p-1074
  },
  { // Entry 1936
    0.0,
    0.0,
    -0x1.0p-1074,
    0.0
  },
  { // Entry 1937
    -0.0,
    0.0,
    -0x1.0p-1074,
    -0.0
  },
  { // Entry 1938
    HUGE_VAL,
    0.0,
    0.0,
    HUGE_VAL
  },
  { // Entry 1939
    -HUGE_VAL,
    0.0,
    0.0,
    -HUGE_VAL
  },
  { // Entry 1940
    0x1.fffffffffffff0p1023,
    0.0,
    0.0,
    0x1.fffffffffffffp1023
  },
  { // Entry 1941
    -0x1.fffffffffffff0p1023,
    0.0,
    0.0,
    -0x1.fffffffffffffp1023
  },
  { // Entry 1942
    0x1.p-1022,
    0.0,
    0.0,
    0x1.0p-1022
  },
  { // Entry 1943
    -0x1.p-1022,
    0.0,
    0.0,
    -0x1.0p-1022
  },
  { // Entry 1944
    0x1.ffffffffffffe0p-1023,
    0.0,
    0.0,
    0x1.ffffffffffffep-1023
  },
  { // Entry 1945
    -0x1.ffffffffffffe0p-1023,
    0.0,
    0.0,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 1946
    0x1.p-1074,
    0.0,
    0.0,
    0x1.0p-1074
  },
  { // Entry 1947
    -0x1.p-1074,
    0.0,
    0.0,
    -0x1.0p-1074
  },
  { // Entry 1948
    0.0,
    0.0,
    0.0,
    0.0
  },
  { // Entry 1949
    0.0,
    0.0,
    0.0,
    -0.0
  },
  { // Entry 1950
    HUGE_VAL,
    0.0,
    -0.0,
    HUGE_VAL
  },
  { // Entry 1951
    -HUGE_VAL,
    0.0,
    -0.0,
    -HUGE_VAL
  },
  { // Entry 1952
    0x1.fffffffffffff0p1023,
    0.0,
    -0.0,
    0x1.fffffffffffffp1023
  },
  { // Entry 1953
    -0x1.fffffffffffff0p1023,
    0.0,
    -0.0,
    -0x1.fffffffffffffp1023
  },
  { // Entry 1954
    0x1.p-1022,
    0.0,
    -0.0,
    0x1.0p-1022
  },
  { // Entry 1955
    -0x1.p-1022,
    0.0,
    -0.0,
    -0x1.0p-1022
  },
  { // Entry 1956
    0x1.ffffffffffffe0p-1023,
    0.0,
    -0.0,
    0x1.ffffffffffffep-1023
  },
  { // Entry 1957
    -0x1.ffffffffffffe0p-1023,
    0.0,
    -0.0,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 1958
    0x1.p-1074,
    0.0,
    -0.0,
    0x1.0p-1074
  },
  { // Entry 1959
    -0x1.p-1074,
    0.0,
    -0.0,
    -0x1.0p-1074
  },
  { // Entry 1960
    0.0,
    0.0,
    -0.0,
    0.0
  },
  { // Entry 1961
    -0.0,
    0.0,
    -0.0,
    -0.0
  },
  { // Entry 1962
    HUGE_VAL,
    -0.0,
    0x1.fffffffffffffp1023,
    HUGE_VAL
  },
  { // Entry 1963
    -HUGE_VAL,
    -0.0,
    0x1.fffffffffffffp1023,
    -HUGE_VAL
  },
  { // Entry 1964
    0x1.fffffffffffff0p1023,
    -0.0,
    0x1.fffffffffffffp1023,
    0x1.fffffffffffffp1023
  },
  { // Entry 1965
    -0x1.fffffffffffff0p1023,
    -0.0,
    0x1.fffffffffffffp1023,
    -0x1.fffffffffffffp1023
  },
  { // Entry 1966
    0x1.p-1022,
    -0.0,
    0x1.fffffffffffffp1023,
    0x1.0p-1022
  },
  { // Entry 1967
    -0x1.p-1022,
    -0.0,
    0x1.fffffffffffffp1023,
    -0x1.0p-1022
  },
  { // Entry 1968
    0x1.ffffffffffffe0p-1023,
    -0.0,
    0x1.fffffffffffffp1023,
    0x1.ffffffffffffep-1023
  },
  { // Entry 1969
    -0x1.ffffffffffffe0p-1023,
    -0.0,
    0x1.fffffffffffffp1023,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 1970
    0x1.p-1074,
    -0.0,
    0x1.fffffffffffffp1023,
    0x1.0p-1074
  },
  { // Entry 1971
    -0x1.p-1074,
    -0.0,
    0x1.fffffffffffffp1023,
    -0x1.0p-1074
  },
  { // Entry 1972
    0.0,
    -0.0,
    0x1.fffffffffffffp1023,
    0.0
  },
  { // Entry 1973
    -0.0,
    -0.0,
    0x1.fffffffffffffp1023,
    -0.0
  },
  { // Entry 1974
    HUGE_VAL,
    -0.0,
    -0x1.fffffffffffffp1023,
    HUGE_VAL
  },
  { // Entry 1975
    -HUGE_VAL,
    -0.0,
    -0x1.fffffffffffffp1023,
    -HUGE_VAL
  },
  { // Entry 1976
    0x1.fffffffffffff0p1023,
    -0.0,
    -0x1.fffffffffffffp1023,
    0x1.fffffffffffffp1023
  },
  { // Entry 1977
    -0x1.fffffffffffff0p1023,
    -0.0,
    -0x1.fffffffffffffp1023,
    -0x1.fffffffffffffp1023
  },
  { // Entry 1978
    0x1.p-1022,
    -0.0,
    -0x1.fffffffffffffp1023,
    0x1.0p-1022
  },
  { // Entry 1979
    -0x1.p-1022,
    -0.0,
    -0x1.fffffffffffffp1023,
    -0x1.0p-1022
  },
  { // Entry 1980
    0x1.ffffffffffffe0p-1023,
    -0.0,
    -0x1.fffffffffffffp1023,
    0x1.ffffffffffffep-1023
  },
  { // Entry 1981
    -0x1.ffffffffffffe0p-1023,
    -0.0,
    -0x1.fffffffffffffp1023,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 1982
    0x1.p-1074,
    -0.0,
    -0x1.fffffffffffffp1023,
    0x1.0p-1074
  },
  { // Entry 1983
    -0x1.p-1074,
    -0.0,
    -0x1.fffffffffffffp1023,
    -0x1.0p-1074
  },
  { // Entry 1984
    0.0,
    -0.0,
    -0x1.fffffffffffffp1023,
    0.0
  },
  { // Entry 1985
    0.0,
    -0.0,
    -0x1.fffffffffffffp1023,
    -0.0
  },
  { // Entry 1986
    HUGE_VAL,
    -0.0,
    0x1.0p-1022,
    HUGE_VAL
  },
  { // Entry 1987
    -HUGE_VAL,
    -0.0,
    0x1.0p-1022,
    -HUGE_VAL
  },
  { // Entry 1988
    0x1.fffffffffffff0p1023,
    -0.0,
    0x1.0p-1022,
    0x1.fffffffffffffp1023
  },
  { // Entry 1989
    -0x1.fffffffffffff0p1023,
    -0.0,
    0x1.0p-1022,
    -0x1.fffffffffffffp1023
  },
  { // Entry 1990
    0x1.p-1022,
    -0.0,
    0x1.0p-1022,
    0x1.0p-1022
  },
  { // Entry 1991
    -0x1.p-1022,
    -0.0,
    0x1.0p-1022,
    -0x1.0p-1022
  },
  { // Entry 1992
    0x1.ffffffffffffe0p-1023,
    -0.0,
    0x1.0p-1022,
    0x1.ffffffffffffep-1023
  },
  { // Entry 1993
    -0x1.ffffffffffffe0p-1023,
    -0.0,
    0x1.0p-1022,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 1994
    0x1.p-1074,
    -0.0,
    0x1.0p-1022,
    0x1.0p-1074
  },
  { // Entry 1995
    -0x1.p-1074,
    -0.0,
    0x1.0p-1022,
    -0x1.0p-1074
  },
  { // Entry 1996
    0.0,
    -0.0,
    0x1.0p-1022,
    0.0
  },
  { // Entry 1997
    -0.0,
    -0.0,
    0x1.0p-1022,
    -0.0
  },
  { // Entry 1998
    HUGE_VAL,
    -0.0,
    -0x1.0p-1022,
    HUGE_VAL
  },
  { // Entry 1999
    -HUGE_VAL,
    -0.0,
    -0x1.0p-1022,
    -HUGE_VAL
  },
  { // Entry 2000
    0x1.fffffffffffff0p1023,
    -0.0,
    -0x1.0p-1022,
    0x1.fffffffffffffp1023
  },
  { // Entry 2001
    -0x1.fffffffffffff0p1023,
    -0.0,
    -0x1.0p-1022,
    -0x1.fffffffffffffp1023
  },
  { // Entry 2002
    0x1.p-1022,
    -0.0,
    -0x1.0p-1022,
    0x1.0p-1022
  },
  { // Entry 2003
    -0x1.p-1022,
    -0.0,
    -0x1.0p-1022,
    -0x1.0p-1022
  },
  { // Entry 2004
    0x1.ffffffffffffe0p-1023,
    -0.0,
    -0x1.0p-1022,
    0x1.ffffffffffffep-1023
  },
  { // Entry 2005
    -0x1.ffffffffffffe0p-1023,
    -0.0,
    -0x1.0p-1022,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 2006
    0x1.p-1074,
    -0.0,
    -0x1.0p-1022,
    0x1.0p-1074
  },
  { // Entry 2007
    -0x1.p-1074,
    -0.0,
    -0x1.0p-1022,
    -0x1.0p-1074
  },
  { // Entry 2008
    0.0,
    -0.0,
    -0x1.0p-1022,
    0.0
  },
  { // Entry 2009
    0.0,
    -0.0,
    -0x1.0p-1022,
    -0.0
  },
  { // Entry 2010
    HUGE_VAL,
    -0.0,
    0x1.ffffffffffffep-1023,
    HUGE_VAL
  },
  { // Entry 2011
    -HUGE_VAL,
    -0.0,
    0x1.ffffffffffffep-1023,
    -HUGE_VAL
  },
  { // Entry 2012
    0x1.fffffffffffff0p1023,
    -0.0,
    0x1.ffffffffffffep-1023,
    0x1.fffffffffffffp1023
  },
  { // Entry 2013
    -0x1.fffffffffffff0p1023,
    -0.0,
    0x1.ffffffffffffep-1023,
    -0x1.fffffffffffffp1023
  },
  { // Entry 2014
    0x1.p-1022,
    -0.0,
    0x1.ffffffffffffep-1023,
    0x1.0p-1022
  },
  { // Entry 2015
    -0x1.p-1022,
    -0.0,
    0x1.ffffffffffffep-1023,
    -0x1.0p-1022
  },
  { // Entry 2016
    0x1.ffffffffffffe0p-1023,
    -0.0,
    0x1.ffffffffffffep-1023,
    0x1.ffffffffffffep-1023
  },
  { // Entry 2017
    -0x1.ffffffffffffe0p-1023,
    -0.0,
    0x1.ffffffffffffep-1023,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 2018
    0x1.p-1074,
    -0.0,
    0x1.ffffffffffffep-1023,
    0x1.0p-1074
  },
  { // Entry 2019
    -0x1.p-1074,
    -0.0,
    0x1.ffffffffffffep-1023,
    -0x1.0p-1074
  },
  { // Entry 2020
    0.0,
    -0.0,
    0x1.ffffffffffffep-1023,
    0.0
  },
  { // Entry 2021
    -0.0,
    -0.0,
    0x1.ffffffffffffep-1023,
    -0.0
  },
  { // Entry 2022
    HUGE_VAL,
    -0.0,
    -0x1.ffffffffffffep-1023,
    HUGE_VAL
  },
  { // Entry 2023
    -HUGE_VAL,
    -0.0,
    -0x1.ffffffffffffep-1023,
    -HUGE_VAL
  },
  { // Entry 2024
    0x1.fffffffffffff0p1023,
    -0.0,
    -0x1.ffffffffffffep-1023,
    0x1.fffffffffffffp1023
  },
  { // Entry 2025
    -0x1.fffffffffffff0p1023,
    -0.0,
    -0x1.ffffffffffffep-1023,
    -0x1.fffffffffffffp1023
  },
  { // Entry 2026
    0x1.p-1022,
    -0.0,
    -0x1.ffffffffffffep-1023,
    0x1.0p-1022
  },
  { // Entry 2027
    -0x1.p-1022,
    -0.0,
    -0x1.ffffffffffffep-1023,
    -0x1.0p-1022
  },
  { // Entry 2028
    0x1.ffffffffffffe0p-1023,
    -0.0,
    -0x1.ffffffffffffep-1023,
    0x1.ffffffffffffep-1023
  },
  { // Entry 2029
    -0x1.ffffffffffffe0p-1023,
    -0.0,
    -0x1.ffffffffffffep-1023,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 2030
    0x1.p-1074,
    -0.0,
    -0x1.ffffffffffffep-1023,
    0x1.0p-1074
  },
  { // Entry 2031
    -0x1.p-1074,
    -0.0,
    -0x1.ffffffffffffep-1023,
    -0x1.0p-1074
  },
  { // Entry 2032
    0.0,
    -0.0,
    -0x1.ffffffffffffep-1023,
    0.0
  },
  { // Entry 2033
    0.0,
    -0.0,
    -0x1.ffffffffffffep-1023,
    -0.0
  },
  { // Entry 2034
    HUGE_VAL,
    -0.0,
    0x1.0p-1074,
    HUGE_VAL
  },
  { // Entry 2035
    -HUGE_VAL,
    -0.0,
    0x1.0p-1074,
    -HUGE_VAL
  },
  { // Entry 2036
    0x1.fffffffffffff0p1023,
    -0.0,
    0x1.0p-1074,
    0x1.fffffffffffffp1023
  },
  { // Entry 2037
    -0x1.fffffffffffff0p1023,
    -0.0,
    0x1.0p-1074,
    -0x1.fffffffffffffp1023
  },
  { // Entry 2038
    0x1.p-1022,
    -0.0,
    0x1.0p-1074,
    0x1.0p-1022
  },
  { // Entry 2039
    -0x1.p-1022,
    -0.0,
    0x1.0p-1074,
    -0x1.0p-1022
  },
  { // Entry 2040
    0x1.ffffffffffffe0p-1023,
    -0.0,
    0x1.0p-1074,
    0x1.ffffffffffffep-1023
  },
  { // Entry 2041
    -0x1.ffffffffffffe0p-1023,
    -0.0,
    0x1.0p-1074,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 2042
    0x1.p-1074,
    -0.0,
    0x1.0p-1074,
    0x1.0p-1074
  },
  { // Entry 2043
    -0x1.p-1074,
    -0.0,
    0x1.0p-1074,
    -0x1.0p-1074
  },
  { // Entry 2044
    0.0,
    -0.0,
    0x1.0p-1074,
    0.0
  },
  { // Entry 2045
    -0.0,
    -0.0,
    0x1.0p-1074,
    -0.0
  },
  { // Entry 2046
    HUGE_VAL,
    -0.0,
    -0x1.0p-1074,
    HUGE_VAL
  },
  { // Entry 2047
    -HUGE_VAL,
    -0.0,
    -0x1.0p-1074,
    -HUGE_VAL
  },
  { // Entry 2048
    0x1.fffffffffffff0p1023,
    -0.0,
    -0x1.0p-1074,
    0x1.fffffffffffffp1023
  },
  { // Entry 2049
    -0x1.fffffffffffff0p1023,
    -0.0,
    -0x1.0p-1074,
    -0x1.fffffffffffffp1023
  },
  { // Entry 2050
    0x1.p-1022,
    -0.0,
    -0x1.0p-1074,
    0x1.0p-1022
  },
  { // Entry 2051
    -0x1.p-1022,
    -0.0,
    -0x1.0p-1074,
    -0x1.0p-1022
  },
  { // Entry 2052
    0x1.ffffffffffffe0p-1023,
    -0.0,
    -0x1.0p-1074,
    0x1.ffffffffffffep-1023
  },
  { // Entry 2053
    -0x1.ffffffffffffe0p-1023,
    -0.0,
    -0x1.0p-1074,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 2054
    0x1.p-1074,
    -0.0,
    -0x1.0p-1074,
    0x1.0p-1074
  },
  { // Entry 2055
    -0x1.p-1074,
    -0.0,
    -0x1.0p-1074,
    -0x1.0p-1074
  },
  { // Entry 2056
    0.0,
    -0.0,
    -0x1.0p-1074,
    0.0
  },
  { // Entry 2057
    0.0,
    -0.0,
    -0x1.0p-1074,
    -0.0
  },
  { // Entry 2058
    HUGE_VAL,
    -0.0,
    0.0,
    HUGE_VAL
  },
  { // Entry 2059
    -HUGE_VAL,
    -0.0,
    0.0,
    -HUGE_VAL
  },
  { // Entry 2060
    0x1.fffffffffffff0p1023,
    -0.0,
    0.0,
    0x1.fffffffffffffp1023
  },
  { // Entry 2061
    -0x1.fffffffffffff0p1023,
    -0.0,
    0.0,
    -0x1.fffffffffffffp1023
  },
  { // Entry 2062
    0x1.p-1022,
    -0.0,
    0.0,
    0x1.0p-1022
  },
  { // Entry 2063
    -0x1.p-1022,
    -0.0,
    0.0,
    -0x1.0p-1022
  },
  { // Entry 2064
    0x1.ffffffffffffe0p-1023,
    -0.0,
    0.0,
    0x1.ffffffffffffep-1023
  },
  { // Entry 2065
    -0x1.ffffffffffffe0p-1023,
    -0.0,
    0.0,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 2066
    0x1.p-1074,
    -0.0,
    0.0,
    0x1.0p-1074
  },
  { // Entry 2067
    -0x1.p-1074,
    -0.0,
    0.0,
    -0x1.0p-1074
  },
  { // Entry 2068
    0.0,
    -0.0,
    0.0,
    0.0
  },
  { // Entry 2069
    -0.0,
    -0.0,
    0.0,
    -0.0
  },
  { // Entry 2070
    HUGE_VAL,
    -0.0,
    -0.0,
    HUGE_VAL
  },
  { // Entry 2071
    -HUGE_VAL,
    -0.0,
    -0.0,
    -HUGE_VAL
  },
  { // Entry 2072
    0x1.fffffffffffff0p1023,
    -0.0,
    -0.0,
    0x1.fffffffffffffp1023
  },
  { // Entry 2073
    -0x1.fffffffffffff0p1023,
    -0.0,
    -0.0,
    -0x1.fffffffffffffp1023
  },
  { // Entry 2074
    0x1.p-1022,
    -0.0,
    -0.0,
    0x1.0p-1022
  },
  { // Entry 2075
    -0x1.p-1022,
    -0.0,
    -0.0,
    -0x1.0p-1022
  },
  { // Entry 2076
    0x1.ffffffffffffe0p-1023,
    -0.0,
    -0.0,
    0x1.ffffffffffffep-1023
  },
  { // Entry 2077
    -0x1.ffffffffffffe0p-1023,
    -0.0,
    -0.0,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 2078
    0x1.p-1074,
    -0.0,
    -0.0,
    0x1.0p-1074
  },
  { // Entry 2079
    -0x1.p-1074,
    -0.0,
    -0.0,
    -0x1.0p-1074
  },
  { // Entry 2080
    0.0,
    -0.0,
    -0.0,
    0.0
  },
  { // Entry 2081
    0.0,
    -0.0,
    -0.0,
    -0.0
  },
  { // Entry 2082
    HUGE_VAL,
    HUGE_VAL,
    HUGE_VAL,
    HUGE_VAL
  },
  { // Entry 2083
    HUGE_VAL,
    HUGE_VAL,
    HUGE_VAL,
    0x1.fffffffffffffp1023
  },
  { // Entry 2084
    HUGE_VAL,
    HUGE_VAL,
    HUGE_VAL,
    -0x1.fffffffffffffp1023
  },
  { // Entry 2085
    HUGE_VAL,
    HUGE_VAL,
    HUGE_VAL,
    0x1.0p-1022
  },
  { // Entry 2086
    HUGE_VAL,
    HUGE_VAL,
    HUGE_VAL,
    -0x1.0p-1022
  },
  { // Entry 2087
    HUGE_VAL,
    HUGE_VAL,
    HUGE_VAL,
    0x1.ffffffffffffep-1023
  },
  { // Entry 2088
    HUGE_VAL,
    HUGE_VAL,
    HUGE_VAL,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 2089
    HUGE_VAL,
    HUGE_VAL,
    HUGE_VAL,
    0x1.0p-1074
  },
  { // Entry 2090
    HUGE_VAL,
    HUGE_VAL,
    HUGE_VAL,
    -0x1.0p-1074
  },
  { // Entry 2091
    HUGE_VAL,
    HUGE_VAL,
    HUGE_VAL,
    0.0
  },
  { // Entry 2092
    HUGE_VAL,
    HUGE_VAL,
    HUGE_VAL,
    -0.0
  },
  { // Entry 2093
    -HUGE_VAL,
    HUGE_VAL,
    -HUGE_VAL,
    -HUGE_VAL
  },
  { // Entry 2094
    -HUGE_VAL,
    HUGE_VAL,
    -HUGE_VAL,
    0x1.fffffffffffffp1023
  },
  { // Entry 2095
    -HUGE_VAL,
    HUGE_VAL,
    -HUGE_VAL,
    -0x1.fffffffffffffp1023
  },
  { // Entry 2096
    -HUGE_VAL,
    HUGE_VAL,
    -HUGE_VAL,
    0x1.0p-1022
  },
  { // Entry 2097
    -HUGE_VAL,
    HUGE_VAL,
    -HUGE_VAL,
    -0x1.0p-1022
  },
  { // Entry 2098
    -HUGE_VAL,
    HUGE_VAL,
    -HUGE_VAL,
    0x1.ffffffffffffep-1023
  },
  { // Entry 2099
    -HUGE_VAL,
    HUGE_VAL,
    -HUGE_VAL,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 2100
    -HUGE_VAL,
    HUGE_VAL,
    -HUGE_VAL,
    0x1.0p-1074
  },
  { // Entry 2101
    -HUGE_VAL,
    HUGE_VAL,
    -HUGE_VAL,
    -0x1.0p-1074
  },
  { // Entry 2102
    -HUGE_VAL,
    HUGE_VAL,
    -HUGE_VAL,
    0.0
  },
  { // Entry 2103
    -HUGE_VAL,
    HUGE_VAL,
    -HUGE_VAL,
    -0.0
  },
  { // Entry 2104
    HUGE_VAL,
    HUGE_VAL,
    0x1.fffffffffffffp1023,
    HUGE_VAL
  },
  { // Entry 2105
    HUGE_VAL,
    HUGE_VAL,
    0x1.fffffffffffffp1023,
    0x1.fffffffffffffp1023
  },
  { // Entry 2106
    HUGE_VAL,
    HUGE_VAL,
    0x1.fffffffffffffp1023,
    -0x1.fffffffffffffp1023
  },
  { // Entry 2107
    HUGE_VAL,
    HUGE_VAL,
    0x1.fffffffffffffp1023,
    0x1.0p-1022
  },
  { // Entry 2108
    HUGE_VAL,
    HUGE_VAL,
    0x1.fffffffffffffp1023,
    -0x1.0p-1022
  },
  { // Entry 2109
    HUGE_VAL,
    HUGE_VAL,
    0x1.fffffffffffffp1023,
    0x1.ffffffffffffep-1023
  },
  { // Entry 2110
    HUGE_VAL,
    HUGE_VAL,
    0x1.fffffffffffffp1023,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 2111
    HUGE_VAL,
    HUGE_VAL,
    0x1.fffffffffffffp1023,
    0x1.0p-1074
  },
  { // Entry 2112
    HUGE_VAL,
    HUGE_VAL,
    0x1.fffffffffffffp1023,
    -0x1.0p-1074
  },
  { // Entry 2113
    HUGE_VAL,
    HUGE_VAL,
    0x1.fffffffffffffp1023,
    0.0
  },
  { // Entry 2114
    HUGE_VAL,
    HUGE_VAL,
    0x1.fffffffffffffp1023,
    -0.0
  },
  { // Entry 2115
    -HUGE_VAL,
    HUGE_VAL,
    -0x1.fffffffffffffp1023,
    -HUGE_VAL
  },
  { // Entry 2116
    -HUGE_VAL,
    HUGE_VAL,
    -0x1.fffffffffffffp1023,
    0x1.fffffffffffffp1023
  },
  { // Entry 2117
    -HUGE_VAL,
    HUGE_VAL,
    -0x1.fffffffffffffp1023,
    -0x1.fffffffffffffp1023
  },
  { // Entry 2118
    -HUGE_VAL,
    HUGE_VAL,
    -0x1.fffffffffffffp1023,
    0x1.0p-1022
  },
  { // Entry 2119
    -HUGE_VAL,
    HUGE_VAL,
    -0x1.fffffffffffffp1023,
    -0x1.0p-1022
  },
  { // Entry 2120
    -HUGE_VAL,
    HUGE_VAL,
    -0x1.fffffffffffffp1023,
    0x1.ffffffffffffep-1023
  },
  { // Entry 2121
    -HUGE_VAL,
    HUGE_VAL,
    -0x1.fffffffffffffp1023,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 2122
    -HUGE_VAL,
    HUGE_VAL,
    -0x1.fffffffffffffp1023,
    0x1.0p-1074
  },
  { // Entry 2123
    -HUGE_VAL,
    HUGE_VAL,
    -0x1.fffffffffffffp1023,
    -0x1.0p-1074
  },
  { // Entry 2124
    -HUGE_VAL,
    HUGE_VAL,
    -0x1.fffffffffffffp1023,
    0.0
  },
  { // Entry 2125
    -HUGE_VAL,
    HUGE_VAL,
    -0x1.fffffffffffffp1023,
    -0.0
  },
  { // Entry 2126
    HUGE_VAL,
    HUGE_VAL,
    0x1.0p-1022,
    HUGE_VAL
  },
  { // Entry 2127
    HUGE_VAL,
    HUGE_VAL,
    0x1.0p-1022,
    0x1.fffffffffffffp1023
  },
  { // Entry 2128
    HUGE_VAL,
    HUGE_VAL,
    0x1.0p-1022,
    -0x1.fffffffffffffp1023
  },
  { // Entry 2129
    HUGE_VAL,
    HUGE_VAL,
    0x1.0p-1022,
    0x1.0p-1022
  },
  { // Entry 2130
    HUGE_VAL,
    HUGE_VAL,
    0x1.0p-1022,
    -0x1.0p-1022
  },
  { // Entry 2131
    HUGE_VAL,
    HUGE_VAL,
    0x1.0p-1022,
    0x1.ffffffffffffep-1023
  },
  { // Entry 2132
    HUGE_VAL,
    HUGE_VAL,
    0x1.0p-1022,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 2133
    HUGE_VAL,
    HUGE_VAL,
    0x1.0p-1022,
    0x1.0p-1074
  },
  { // Entry 2134
    HUGE_VAL,
    HUGE_VAL,
    0x1.0p-1022,
    -0x1.0p-1074
  },
  { // Entry 2135
    HUGE_VAL,
    HUGE_VAL,
    0x1.0p-1022,
    0.0
  },
  { // Entry 2136
    HUGE_VAL,
    HUGE_VAL,
    0x1.0p-1022,
    -0.0
  },
  { // Entry 2137
    -HUGE_VAL,
    HUGE_VAL,
    -0x1.0p-1022,
    -HUGE_VAL
  },
  { // Entry 2138
    -HUGE_VAL,
    HUGE_VAL,
    -0x1.0p-1022,
    0x1.fffffffffffffp1023
  },
  { // Entry 2139
    -HUGE_VAL,
    HUGE_VAL,
    -0x1.0p-1022,
    -0x1.fffffffffffffp1023
  },
  { // Entry 2140
    -HUGE_VAL,
    HUGE_VAL,
    -0x1.0p-1022,
    0x1.0p-1022
  },
  { // Entry 2141
    -HUGE_VAL,
    HUGE_VAL,
    -0x1.0p-1022,
    -0x1.0p-1022
  },
  { // Entry 2142
    -HUGE_VAL,
    HUGE_VAL,
    -0x1.0p-1022,
    0x1.ffffffffffffep-1023
  },
  { // Entry 2143
    -HUGE_VAL,
    HUGE_VAL,
    -0x1.0p-1022,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 2144
    -HUGE_VAL,
    HUGE_VAL,
    -0x1.0p-1022,
    0x1.0p-1074
  },
  { // Entry 2145
    -HUGE_VAL,
    HUGE_VAL,
    -0x1.0p-1022,
    -0x1.0p-1074
  },
  { // Entry 2146
    -HUGE_VAL,
    HUGE_VAL,
    -0x1.0p-1022,
    0.0
  },
  { // Entry 2147
    -HUGE_VAL,
    HUGE_VAL,
    -0x1.0p-1022,
    -0.0
  },
  { // Entry 2148
    HUGE_VAL,
    HUGE_VAL,
    0x1.ffffffffffffep-1023,
    HUGE_VAL
  },
  { // Entry 2149
    HUGE_VAL,
    HUGE_VAL,
    0x1.ffffffffffffep-1023,
    0x1.fffffffffffffp1023
  },
  { // Entry 2150
    HUGE_VAL,
    HUGE_VAL,
    0x1.ffffffffffffep-1023,
    -0x1.fffffffffffffp1023
  },
  { // Entry 2151
    HUGE_VAL,
    HUGE_VAL,
    0x1.ffffffffffffep-1023,
    0x1.0p-1022
  },
  { // Entry 2152
    HUGE_VAL,
    HUGE_VAL,
    0x1.ffffffffffffep-1023,
    -0x1.0p-1022
  },
  { // Entry 2153
    HUGE_VAL,
    HUGE_VAL,
    0x1.ffffffffffffep-1023,
    0x1.ffffffffffffep-1023
  },
  { // Entry 2154
    HUGE_VAL,
    HUGE_VAL,
    0x1.ffffffffffffep-1023,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 2155
    HUGE_VAL,
    HUGE_VAL,
    0x1.ffffffffffffep-1023,
    0x1.0p-1074
  },
  { // Entry 2156
    HUGE_VAL,
    HUGE_VAL,
    0x1.ffffffffffffep-1023,
    -0x1.0p-1074
  },
  { // Entry 2157
    HUGE_VAL,
    HUGE_VAL,
    0x1.ffffffffffffep-1023,
    0.0
  },
  { // Entry 2158
    HUGE_VAL,
    HUGE_VAL,
    0x1.ffffffffffffep-1023,
    -0.0
  },
  { // Entry 2159
    -HUGE_VAL,
    HUGE_VAL,
    -0x1.ffffffffffffep-1023,
    -HUGE_VAL
  },
  { // Entry 2160
    -HUGE_VAL,
    HUGE_VAL,
    -0x1.ffffffffffffep-1023,
    0x1.fffffffffffffp1023
  },
  { // Entry 2161
    -HUGE_VAL,
    HUGE_VAL,
    -0x1.ffffffffffffep-1023,
    -0x1.fffffffffffffp1023
  },
  { // Entry 2162
    -HUGE_VAL,
    HUGE_VAL,
    -0x1.ffffffffffffep-1023,
    0x1.0p-1022
  },
  { // Entry 2163
    -HUGE_VAL,
    HUGE_VAL,
    -0x1.ffffffffffffep-1023,
    -0x1.0p-1022
  },
  { // Entry 2164
    -HUGE_VAL,
    HUGE_VAL,
    -0x1.ffffffffffffep-1023,
    0x1.ffffffffffffep-1023
  },
  { // Entry 2165
    -HUGE_VAL,
    HUGE_VAL,
    -0x1.ffffffffffffep-1023,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 2166
    -HUGE_VAL,
    HUGE_VAL,
    -0x1.ffffffffffffep-1023,
    0x1.0p-1074
  },
  { // Entry 2167
    -HUGE_VAL,
    HUGE_VAL,
    -0x1.ffffffffffffep-1023,
    -0x1.0p-1074
  },
  { // Entry 2168
    -HUGE_VAL,
    HUGE_VAL,
    -0x1.ffffffffffffep-1023,
    0.0
  },
  { // Entry 2169
    -HUGE_VAL,
    HUGE_VAL,
    -0x1.ffffffffffffep-1023,
    -0.0
  },
  { // Entry 2170
    HUGE_VAL,
    HUGE_VAL,
    0x1.0p-1074,
    HUGE_VAL
  },
  { // Entry 2171
    HUGE_VAL,
    HUGE_VAL,
    0x1.0p-1074,
    0x1.fffffffffffffp1023
  },
  { // Entry 2172
    HUGE_VAL,
    HUGE_VAL,
    0x1.0p-1074,
    -0x1.fffffffffffffp1023
  },
  { // Entry 2173
    HUGE_VAL,
    HUGE_VAL,
    0x1.0p-1074,
    0x1.0p-1022
  },
  { // Entry 2174
    HUGE_VAL,
    HUGE_VAL,
    0x1.0p-1074,
    -0x1.0p-1022
  },
  { // Entry 2175
    HUGE_VAL,
    HUGE_VAL,
    0x1.0p-1074,
    0x1.ffffffffffffep-1023
  },
  { // Entry 2176
    HUGE_VAL,
    HUGE_VAL,
    0x1.0p-1074,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 2177
    HUGE_VAL,
    HUGE_VAL,
    0x1.0p-1074,
    0x1.0p-1074
  },
  { // Entry 2178
    HUGE_VAL,
    HUGE_VAL,
    0x1.0p-1074,
    -0x1.0p-1074
  },
  { // Entry 2179
    HUGE_VAL,
    HUGE_VAL,
    0x1.0p-1074,
    0.0
  },
  { // Entry 2180
    HUGE_VAL,
    HUGE_VAL,
    0x1.0p-1074,
    -0.0
  },
  { // Entry 2181
    -HUGE_VAL,
    HUGE_VAL,
    -0x1.0p-1074,
    -HUGE_VAL
  },
  { // Entry 2182
    -HUGE_VAL,
    HUGE_VAL,
    -0x1.0p-1074,
    0x1.fffffffffffffp1023
  },
  { // Entry 2183
    -HUGE_VAL,
    HUGE_VAL,
    -0x1.0p-1074,
    -0x1.fffffffffffffp1023
  },
  { // Entry 2184
    -HUGE_VAL,
    HUGE_VAL,
    -0x1.0p-1074,
    0x1.0p-1022
  },
  { // Entry 2185
    -HUGE_VAL,
    HUGE_VAL,
    -0x1.0p-1074,
    -0x1.0p-1022
  },
  { // Entry 2186
    -HUGE_VAL,
    HUGE_VAL,
    -0x1.0p-1074,
    0x1.ffffffffffffep-1023
  },
  { // Entry 2187
    -HUGE_VAL,
    HUGE_VAL,
    -0x1.0p-1074,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 2188
    -HUGE_VAL,
    HUGE_VAL,
    -0x1.0p-1074,
    0x1.0p-1074
  },
  { // Entry 2189
    -HUGE_VAL,
    HUGE_VAL,
    -0x1.0p-1074,
    -0x1.0p-1074
  },
  { // Entry 2190
    -HUGE_VAL,
    HUGE_VAL,
    -0x1.0p-1074,
    0.0
  },
  { // Entry 2191
    -HUGE_VAL,
    HUGE_VAL,
    -0x1.0p-1074,
    -0.0
  },
  { // Entry 2192
    -HUGE_VAL,
    -HUGE_VAL,
    HUGE_VAL,
    -HUGE_VAL
  },
  { // Entry 2193
    -HUGE_VAL,
    -HUGE_VAL,
    HUGE_VAL,
    0x1.fffffffffffffp1023
  },
  { // Entry 2194
    -HUGE_VAL,
    -HUGE_VAL,
    HUGE_VAL,
    -0x1.fffffffffffffp1023
  },
  { // Entry 2195
    -HUGE_VAL,
    -HUGE_VAL,
    HUGE_VAL,
    0x1.0p-1022
  },
  { // Entry 2196
    -HUGE_VAL,
    -HUGE_VAL,
    HUGE_VAL,
    -0x1.0p-1022
  },
  { // Entry 2197
    -HUGE_VAL,
    -HUGE_VAL,
    HUGE_VAL,
    0x1.ffffffffffffep-1023
  },
  { // Entry 2198
    -HUGE_VAL,
    -HUGE_VAL,
    HUGE_VAL,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 2199
    -HUGE_VAL,
    -HUGE_VAL,
    HUGE_VAL,
    0x1.0p-1074
  },
  { // Entry 2200
    -HUGE_VAL,
    -HUGE_VAL,
    HUGE_VAL,
    -0x1.0p-1074
  },
  { // Entry 2201
    -HUGE_VAL,
    -HUGE_VAL,
    HUGE_VAL,
    0.0
  },
  { // Entry 2202
    -HUGE_VAL,
    -HUGE_VAL,
    HUGE_VAL,
    -0.0
  },
  { // Entry 2203
    HUGE_VAL,
    -HUGE_VAL,
    -HUGE_VAL,
    HUGE_VAL
  },
  { // Entry 2204
    HUGE_VAL,
    -HUGE_VAL,
    -HUGE_VAL,
    0x1.fffffffffffffp1023
  },
  { // Entry 2205
    HUGE_VAL,
    -HUGE_VAL,
    -HUGE_VAL,
    -0x1.fffffffffffffp1023
  },
  { // Entry 2206
    HUGE_VAL,
    -HUGE_VAL,
    -HUGE_VAL,
    0x1.0p-1022
  },
  { // Entry 2207
    HUGE_VAL,
    -HUGE_VAL,
    -HUGE_VAL,
    -0x1.0p-1022
  },
  { // Entry 2208
    HUGE_VAL,
    -HUGE_VAL,
    -HUGE_VAL,
    0x1.ffffffffffffep-1023
  },
  { // Entry 2209
    HUGE_VAL,
    -HUGE_VAL,
    -HUGE_VAL,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 2210
    HUGE_VAL,
    -HUGE_VAL,
    -HUGE_VAL,
    0x1.0p-1074
  },
  { // Entry 2211
    HUGE_VAL,
    -HUGE_VAL,
    -HUGE_VAL,
    -0x1.0p-1074
  },
  { // Entry 2212
    HUGE_VAL,
    -HUGE_VAL,
    -HUGE_VAL,
    0.0
  },
  { // Entry 2213
    HUGE_VAL,
    -HUGE_VAL,
    -HUGE_VAL,
    -0.0
  },
  { // Entry 2214
    -HUGE_VAL,
    -HUGE_VAL,
    0x1.fffffffffffffp1023,
    -HUGE_VAL
  },
  { // Entry 2215
    -HUGE_VAL,
    -HUGE_VAL,
    0x1.fffffffffffffp1023,
    0x1.fffffffffffffp1023
  },
  { // Entry 2216
    -HUGE_VAL,
    -HUGE_VAL,
    0x1.fffffffffffffp1023,
    -0x1.fffffffffffffp1023
  },
  { // Entry 2217
    -HUGE_VAL,
    -HUGE_VAL,
    0x1.fffffffffffffp1023,
    0x1.0p-1022
  },
  { // Entry 2218
    -HUGE_VAL,
    -HUGE_VAL,
    0x1.fffffffffffffp1023,
    -0x1.0p-1022
  },
  { // Entry 2219
    -HUGE_VAL,
    -HUGE_VAL,
    0x1.fffffffffffffp1023,
    0x1.ffffffffffffep-1023
  },
  { // Entry 2220
    -HUGE_VAL,
    -HUGE_VAL,
    0x1.fffffffffffffp1023,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 2221
    -HUGE_VAL,
    -HUGE_VAL,
    0x1.fffffffffffffp1023,
    0x1.0p-1074
  },
  { // Entry 2222
    -HUGE_VAL,
    -HUGE_VAL,
    0x1.fffffffffffffp1023,
    -0x1.0p-1074
  },
  { // Entry 2223
    -HUGE_VAL,
    -HUGE_VAL,
    0x1.fffffffffffffp1023,
    0.0
  },
  { // Entry 2224
    -HUGE_VAL,
    -HUGE_VAL,
    0x1.fffffffffffffp1023,
    -0.0
  },
  { // Entry 2225
    HUGE_VAL,
    -HUGE_VAL,
    -0x1.fffffffffffffp1023,
    HUGE_VAL
  },
  { // Entry 2226
    HUGE_VAL,
    -HUGE_VAL,
    -0x1.fffffffffffffp1023,
    0x1.fffffffffffffp1023
  },
  { // Entry 2227
    HUGE_VAL,
    -HUGE_VAL,
    -0x1.fffffffffffffp1023,
    -0x1.fffffffffffffp1023
  },
  { // Entry 2228
    HUGE_VAL,
    -HUGE_VAL,
    -0x1.fffffffffffffp1023,
    0x1.0p-1022
  },
  { // Entry 2229
    HUGE_VAL,
    -HUGE_VAL,
    -0x1.fffffffffffffp1023,
    -0x1.0p-1022
  },
  { // Entry 2230
    HUGE_VAL,
    -HUGE_VAL,
    -0x1.fffffffffffffp1023,
    0x1.ffffffffffffep-1023
  },
  { // Entry 2231
    HUGE_VAL,
    -HUGE_VAL,
    -0x1.fffffffffffffp1023,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 2232
    HUGE_VAL,
    -HUGE_VAL,
    -0x1.fffffffffffffp1023,
    0x1.0p-1074
  },
  { // Entry 2233
    HUGE_VAL,
    -HUGE_VAL,
    -0x1.fffffffffffffp1023,
    -0x1.0p-1074
  },
  { // Entry 2234
    HUGE_VAL,
    -HUGE_VAL,
    -0x1.fffffffffffffp1023,
    0.0
  },
  { // Entry 2235
    HUGE_VAL,
    -HUGE_VAL,
    -0x1.fffffffffffffp1023,
    -0.0
  },
  { // Entry 2236
    -HUGE_VAL,
    -HUGE_VAL,
    0x1.0p-1022,
    -HUGE_VAL
  },
  { // Entry 2237
    -HUGE_VAL,
    -HUGE_VAL,
    0x1.0p-1022,
    0x1.fffffffffffffp1023
  },
  { // Entry 2238
    -HUGE_VAL,
    -HUGE_VAL,
    0x1.0p-1022,
    -0x1.fffffffffffffp1023
  },
  { // Entry 2239
    -HUGE_VAL,
    -HUGE_VAL,
    0x1.0p-1022,
    0x1.0p-1022
  },
  { // Entry 2240
    -HUGE_VAL,
    -HUGE_VAL,
    0x1.0p-1022,
    -0x1.0p-1022
  },
  { // Entry 2241
    -HUGE_VAL,
    -HUGE_VAL,
    0x1.0p-1022,
    0x1.ffffffffffffep-1023
  },
  { // Entry 2242
    -HUGE_VAL,
    -HUGE_VAL,
    0x1.0p-1022,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 2243
    -HUGE_VAL,
    -HUGE_VAL,
    0x1.0p-1022,
    0x1.0p-1074
  },
  { // Entry 2244
    -HUGE_VAL,
    -HUGE_VAL,
    0x1.0p-1022,
    -0x1.0p-1074
  },
  { // Entry 2245
    -HUGE_VAL,
    -HUGE_VAL,
    0x1.0p-1022,
    0.0
  },
  { // Entry 2246
    -HUGE_VAL,
    -HUGE_VAL,
    0x1.0p-1022,
    -0.0
  },
  { // Entry 2247
    HUGE_VAL,
    -HUGE_VAL,
    -0x1.0p-1022,
    HUGE_VAL
  },
  { // Entry 2248
    HUGE_VAL,
    -HUGE_VAL,
    -0x1.0p-1022,
    0x1.fffffffffffffp1023
  },
  { // Entry 2249
    HUGE_VAL,
    -HUGE_VAL,
    -0x1.0p-1022,
    -0x1.fffffffffffffp1023
  },
  { // Entry 2250
    HUGE_VAL,
    -HUGE_VAL,
    -0x1.0p-1022,
    0x1.0p-1022
  },
  { // Entry 2251
    HUGE_VAL,
    -HUGE_VAL,
    -0x1.0p-1022,
    -0x1.0p-1022
  },
  { // Entry 2252
    HUGE_VAL,
    -HUGE_VAL,
    -0x1.0p-1022,
    0x1.ffffffffffffep-1023
  },
  { // Entry 2253
    HUGE_VAL,
    -HUGE_VAL,
    -0x1.0p-1022,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 2254
    HUGE_VAL,
    -HUGE_VAL,
    -0x1.0p-1022,
    0x1.0p-1074
  },
  { // Entry 2255
    HUGE_VAL,
    -HUGE_VAL,
    -0x1.0p-1022,
    -0x1.0p-1074
  },
  { // Entry 2256
    HUGE_VAL,
    -HUGE_VAL,
    -0x1.0p-1022,
    0.0
  },
  { // Entry 2257
    HUGE_VAL,
    -HUGE_VAL,
    -0x1.0p-1022,
    -0.0
  },
  { // Entry 2258
    -HUGE_VAL,
    -HUGE_VAL,
    0x1.ffffffffffffep-1023,
    -HUGE_VAL
  },
  { // Entry 2259
    -HUGE_VAL,
    -HUGE_VAL,
    0x1.ffffffffffffep-1023,
    0x1.fffffffffffffp1023
  },
  { // Entry 2260
    -HUGE_VAL,
    -HUGE_VAL,
    0x1.ffffffffffffep-1023,
    -0x1.fffffffffffffp1023
  },
  { // Entry 2261
    -HUGE_VAL,
    -HUGE_VAL,
    0x1.ffffffffffffep-1023,
    0x1.0p-1022
  },
  { // Entry 2262
    -HUGE_VAL,
    -HUGE_VAL,
    0x1.ffffffffffffep-1023,
    -0x1.0p-1022
  },
  { // Entry 2263
    -HUGE_VAL,
    -HUGE_VAL,
    0x1.ffffffffffffep-1023,
    0x1.ffffffffffffep-1023
  },
  { // Entry 2264
    -HUGE_VAL,
    -HUGE_VAL,
    0x1.ffffffffffffep-1023,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 2265
    -HUGE_VAL,
    -HUGE_VAL,
    0x1.ffffffffffffep-1023,
    0x1.0p-1074
  },
  { // Entry 2266
    -HUGE_VAL,
    -HUGE_VAL,
    0x1.ffffffffffffep-1023,
    -0x1.0p-1074
  },
  { // Entry 2267
    -HUGE_VAL,
    -HUGE_VAL,
    0x1.ffffffffffffep-1023,
    0.0
  },
  { // Entry 2268
    -HUGE_VAL,
    -HUGE_VAL,
    0x1.ffffffffffffep-1023,
    -0.0
  },
  { // Entry 2269
    HUGE_VAL,
    -HUGE_VAL,
    -0x1.ffffffffffffep-1023,
    HUGE_VAL
  },
  { // Entry 2270
    HUGE_VAL,
    -HUGE_VAL,
    -0x1.ffffffffffffep-1023,
    0x1.fffffffffffffp1023
  },
  { // Entry 2271
    HUGE_VAL,
    -HUGE_VAL,
    -0x1.ffffffffffffep-1023,
    -0x1.fffffffffffffp1023
  },
  { // Entry 2272
    HUGE_VAL,
    -HUGE_VAL,
    -0x1.ffffffffffffep-1023,
    0x1.0p-1022
  },
  { // Entry 2273
    HUGE_VAL,
    -HUGE_VAL,
    -0x1.ffffffffffffep-1023,
    -0x1.0p-1022
  },
  { // Entry 2274
    HUGE_VAL,
    -HUGE_VAL,
    -0x1.ffffffffffffep-1023,
    0x1.ffffffffffffep-1023
  },
  { // Entry 2275
    HUGE_VAL,
    -HUGE_VAL,
    -0x1.ffffffffffffep-1023,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 2276
    HUGE_VAL,
    -HUGE_VAL,
    -0x1.ffffffffffffep-1023,
    0x1.0p-1074
  },
  { // Entry 2277
    HUGE_VAL,
    -HUGE_VAL,
    -0x1.ffffffffffffep-1023,
    -0x1.0p-1074
  },
  { // Entry 2278
    HUGE_VAL,
    -HUGE_VAL,
    -0x1.ffffffffffffep-1023,
    0.0
  },
  { // Entry 2279
    HUGE_VAL,
    -HUGE_VAL,
    -0x1.ffffffffffffep-1023,
    -0.0
  },
  { // Entry 2280
    -HUGE_VAL,
    -HUGE_VAL,
    0x1.0p-1074,
    -HUGE_VAL
  },
  { // Entry 2281
    -HUGE_VAL,
    -HUGE_VAL,
    0x1.0p-1074,
    0x1.fffffffffffffp1023
  },
  { // Entry 2282
    -HUGE_VAL,
    -HUGE_VAL,
    0x1.0p-1074,
    -0x1.fffffffffffffp1023
  },
  { // Entry 2283
    -HUGE_VAL,
    -HUGE_VAL,
    0x1.0p-1074,
    0x1.0p-1022
  },
  { // Entry 2284
    -HUGE_VAL,
    -HUGE_VAL,
    0x1.0p-1074,
    -0x1.0p-1022
  },
  { // Entry 2285
    -HUGE_VAL,
    -HUGE_VAL,
    0x1.0p-1074,
    0x1.ffffffffffffep-1023
  },
  { // Entry 2286
    -HUGE_VAL,
    -HUGE_VAL,
    0x1.0p-1074,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 2287
    -HUGE_VAL,
    -HUGE_VAL,
    0x1.0p-1074,
    0x1.0p-1074
  },
  { // Entry 2288
    -HUGE_VAL,
    -HUGE_VAL,
    0x1.0p-1074,
    -0x1.0p-1074
  },
  { // Entry 2289
    -HUGE_VAL,
    -HUGE_VAL,
    0x1.0p-1074,
    0.0
  },
  { // Entry 2290
    -HUGE_VAL,
    -HUGE_VAL,
    0x1.0p-1074,
    -0.0
  },
  { // Entry 2291
    HUGE_VAL,
    -HUGE_VAL,
    -0x1.0p-1074,
    HUGE_VAL
  },
  { // Entry 2292
    HUGE_VAL,
    -HUGE_VAL,
    -0x1.0p-1074,
    0x1.fffffffffffffp1023
  },
  { // Entry 2293
    HUGE_VAL,
    -HUGE_VAL,
    -0x1.0p-1074,
    -0x1.fffffffffffffp1023
  },
  { // Entry 2294
    HUGE_VAL,
    -HUGE_VAL,
    -0x1.0p-1074,
    0x1.0p-1022
  },
  { // Entry 2295
    HUGE_VAL,
    -HUGE_VAL,
    -0x1.0p-1074,
    -0x1.0p-1022
  },
  { // Entry 2296
    HUGE_VAL,
    -HUGE_VAL,
    -0x1.0p-1074,
    0x1.ffffffffffffep-1023
  },
  { // Entry 2297
    HUGE_VAL,
    -HUGE_VAL,
    -0x1.0p-1074,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 2298
    HUGE_VAL,
    -HUGE_VAL,
    -0x1.0p-1074,
    0x1.0p-1074
  },
  { // Entry 2299
    HUGE_VAL,
    -HUGE_VAL,
    -0x1.0p-1074,
    -0x1.0p-1074
  },
  { // Entry 2300
    HUGE_VAL,
    -HUGE_VAL,
    -0x1.0p-1074,
    0.0
  },
  { // Entry 2301
    HUGE_VAL,
    -HUGE_VAL,
    -0x1.0p-1074,
    -0.0
  }
};
