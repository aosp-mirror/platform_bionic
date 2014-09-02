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

static data_1_1_t<double, double> g_fabs_intel_data[] = {
  { // Entry 0
    0x1.p-10,
    -0x1.0p-10
  },
  { // Entry 1
    0x1.p-1074,
    -0x1.0p-1074
  },
  { // Entry 2
    0.0,
    -0.0
  },
  { // Entry 3
    0x1.p-1074,
    0x1.0p-1074
  },
  { // Entry 4
    0x1.fffffffffffff0p999,
    0x1.fffffffffffffp999
  },
  { // Entry 5
    0x1.p1000,
    0x1.0p1000
  },
  { // Entry 6
    0x1.00000000000010p1000,
    0x1.0000000000001p1000
  },
  { // Entry 7
    0x1.fffffffffffff0p199,
    0x1.fffffffffffffp199
  },
  { // Entry 8
    0x1.p200,
    0x1.0p200
  },
  { // Entry 9
    0x1.00000000000010p200,
    0x1.0000000000001p200
  },
  { // Entry 10
    0x1.fffffffffffff0p99,
    0x1.fffffffffffffp99
  },
  { // Entry 11
    0x1.p100,
    0x1.0p100
  },
  { // Entry 12
    0x1.00000000000010p100,
    0x1.0000000000001p100
  },
  { // Entry 13
    0x1.fffffffffffff0p19,
    0x1.fffffffffffffp19
  },
  { // Entry 14
    0x1.p20,
    0x1.0p20
  },
  { // Entry 15
    0x1.00000000000010p20,
    0x1.0000000000001p20
  },
  { // Entry 16
    0x1.fffffffffffff0p14,
    0x1.fffffffffffffp14
  },
  { // Entry 17
    0x1.p15,
    0x1.0p15
  },
  { // Entry 18
    0x1.00000000000010p15,
    0x1.0000000000001p15
  },
  { // Entry 19
    0x1.fffffffffffff0p9,
    0x1.fffffffffffffp9
  },
  { // Entry 20
    0x1.p10,
    0x1.0p10
  },
  { // Entry 21
    0x1.00000000000010p10,
    0x1.0000000000001p10
  },
  { // Entry 22
    0x1.fffffffffffff0p8,
    0x1.fffffffffffffp8
  },
  { // Entry 23
    0x1.p9,
    0x1.0p9
  },
  { // Entry 24
    0x1.00000000000010p9,
    0x1.0000000000001p9
  },
  { // Entry 25
    0x1.fffffffffffff0p6,
    0x1.fffffffffffffp6
  },
  { // Entry 26
    0x1.p7,
    0x1.0p7
  },
  { // Entry 27
    0x1.00000000000010p7,
    0x1.0000000000001p7
  },
  { // Entry 28
    0x1.fffffffffffff0p4,
    0x1.fffffffffffffp4
  },
  { // Entry 29
    0x1.p5,
    0x1.0p5
  },
  { // Entry 30
    0x1.00000000000010p5,
    0x1.0000000000001p5
  },
  { // Entry 31
    0x1.fffffffffffff0p3,
    0x1.fffffffffffffp3
  },
  { // Entry 32
    0x1.p4,
    0x1.0p4
  },
  { // Entry 33
    0x1.00000000000010p4,
    0x1.0000000000001p4
  },
  { // Entry 34
    0x1.fffffffffffff0p2,
    0x1.fffffffffffffp2
  },
  { // Entry 35
    0x1.p3,
    0x1.0p3
  },
  { // Entry 36
    0x1.00000000000010p3,
    0x1.0000000000001p3
  },
  { // Entry 37
    0x1.fffffffffffff0p1,
    0x1.fffffffffffffp1
  },
  { // Entry 38
    0x1.p2,
    0x1.0p2
  },
  { // Entry 39
    0x1.00000000000010p2,
    0x1.0000000000001p2
  },
  { // Entry 40
    0x1.fffffffffffff0p0,
    0x1.fffffffffffffp0
  },
  { // Entry 41
    0x1.p1,
    0x1.0p1
  },
  { // Entry 42
    0x1.00000000000010p1,
    0x1.0000000000001p1
  },
  { // Entry 43
    0x1.fffffffffffff0p-1,
    0x1.fffffffffffffp-1
  },
  { // Entry 44
    0x1.p0,
    0x1.0p0
  },
  { // Entry 45
    0x1.00000000000010p0,
    0x1.0000000000001p0
  },
  { // Entry 46
    0x1.fffffffffffff0p-2,
    0x1.fffffffffffffp-2
  },
  { // Entry 47
    0x1.p-1,
    0x1.0p-1
  },
  { // Entry 48
    0x1.00000000000010p-1,
    0x1.0000000000001p-1
  },
  { // Entry 49
    0x1.fffffffffffff0p-3,
    0x1.fffffffffffffp-3
  },
  { // Entry 50
    0x1.p-2,
    0x1.0p-2
  },
  { // Entry 51
    0x1.00000000000010p-2,
    0x1.0000000000001p-2
  },
  { // Entry 52
    0x1.fffffffffffff0p-4,
    0x1.fffffffffffffp-4
  },
  { // Entry 53
    0x1.p-3,
    0x1.0p-3
  },
  { // Entry 54
    0x1.00000000000010p-3,
    0x1.0000000000001p-3
  },
  { // Entry 55
    0x1.fffffffffffff0p-5,
    0x1.fffffffffffffp-5
  },
  { // Entry 56
    0x1.p-4,
    0x1.0p-4
  },
  { // Entry 57
    0x1.00000000000010p-4,
    0x1.0000000000001p-4
  },
  { // Entry 58
    0x1.fffffffffffff0p-6,
    0x1.fffffffffffffp-6
  },
  { // Entry 59
    0x1.p-5,
    0x1.0p-5
  },
  { // Entry 60
    0x1.00000000000010p-5,
    0x1.0000000000001p-5
  },
  { // Entry 61
    0x1.fffffffffffff0p-8,
    0x1.fffffffffffffp-8
  },
  { // Entry 62
    0x1.p-7,
    0x1.0p-7
  },
  { // Entry 63
    0x1.00000000000010p-7,
    0x1.0000000000001p-7
  },
  { // Entry 64
    0x1.fffffffffffff0p-10,
    0x1.fffffffffffffp-10
  },
  { // Entry 65
    0x1.p-9,
    0x1.0p-9
  },
  { // Entry 66
    0x1.00000000000010p-9,
    0x1.0000000000001p-9
  },
  { // Entry 67
    0x1.fffffffffffff0p-11,
    0x1.fffffffffffffp-11
  },
  { // Entry 68
    0x1.p-10,
    0x1.0p-10
  },
  { // Entry 69
    0x1.00000000000010p-10,
    0x1.0000000000001p-10
  },
  { // Entry 70
    0x1.fffffffffffff0p-16,
    0x1.fffffffffffffp-16
  },
  { // Entry 71
    0x1.p-15,
    0x1.0p-15
  },
  { // Entry 72
    0x1.00000000000010p-15,
    0x1.0000000000001p-15
  },
  { // Entry 73
    0x1.fffffffffffff0p-21,
    0x1.fffffffffffffp-21
  },
  { // Entry 74
    0x1.p-20,
    0x1.0p-20
  },
  { // Entry 75
    0x1.00000000000010p-20,
    0x1.0000000000001p-20
  },
  { // Entry 76
    0x1.fffffffffffff0p-101,
    0x1.fffffffffffffp-101
  },
  { // Entry 77
    0x1.p-100,
    0x1.0p-100
  },
  { // Entry 78
    0x1.00000000000010p-100,
    0x1.0000000000001p-100
  },
  { // Entry 79
    0x1.fffffffffffff0p-201,
    0x1.fffffffffffffp-201
  },
  { // Entry 80
    0x1.p-200,
    0x1.0p-200
  },
  { // Entry 81
    0x1.00000000000010p-200,
    0x1.0000000000001p-200
  },
  { // Entry 82
    0x1.fffffffffffff0p-1001,
    0x1.fffffffffffffp-1001
  },
  { // Entry 83
    0x1.p-1000,
    0x1.0p-1000
  },
  { // Entry 84
    0x1.00000000000010p-1000,
    0x1.0000000000001p-1000
  },
  { // Entry 85
    0x1.fffffffffffff0p1023,
    -0x1.fffffffffffffp1023
  },
  { // Entry 86
    0x1.fffffffffffff0p1023,
    0x1.fffffffffffffp1023
  },
  { // Entry 87
    HUGE_VAL,
    HUGE_VAL
  },
  { // Entry 88
    HUGE_VAL,
    -HUGE_VAL
  },
  { // Entry 89
    0x1.fffffffffffff0p1023,
    0x1.fffffffffffffp1023
  },
  { // Entry 90
    0x1.fffffffffffff0p1023,
    -0x1.fffffffffffffp1023
  },
  { // Entry 91
    0x1.ffffffffffffe0p1023,
    0x1.ffffffffffffep1023
  },
  { // Entry 92
    0x1.ffffffffffffe0p1023,
    -0x1.ffffffffffffep1023
  },
  { // Entry 93
    0x1.921fb54442d180p1,
    0x1.921fb54442d18p1
  },
  { // Entry 94
    0x1.921fb54442d180p1,
    -0x1.921fb54442d18p1
  },
  { // Entry 95
    0x1.921fb54442d180p0,
    0x1.921fb54442d18p0
  },
  { // Entry 96
    0x1.921fb54442d180p0,
    -0x1.921fb54442d18p0
  },
  { // Entry 97
    0x1.00000000000010p0,
    0x1.0000000000001p0
  },
  { // Entry 98
    0x1.00000000000010p0,
    -0x1.0000000000001p0
  },
  { // Entry 99
    0x1.p0,
    0x1.0p0
  },
  { // Entry 100
    0x1.p0,
    -0x1.0p0
  },
  { // Entry 101
    0x1.fffffffffffff0p-1,
    0x1.fffffffffffffp-1
  },
  { // Entry 102
    0x1.fffffffffffff0p-1,
    -0x1.fffffffffffffp-1
  },
  { // Entry 103
    0x1.921fb54442d180p-1,
    0x1.921fb54442d18p-1
  },
  { // Entry 104
    0x1.921fb54442d180p-1,
    -0x1.921fb54442d18p-1
  },
  { // Entry 105
    0x1.00000000000010p-1022,
    0x1.0000000000001p-1022
  },
  { // Entry 106
    0x1.00000000000010p-1022,
    -0x1.0000000000001p-1022
  },
  { // Entry 107
    0x1.p-1022,
    0x1.0p-1022
  },
  { // Entry 108
    0x1.p-1022,
    -0x1.0p-1022
  },
  { // Entry 109
    0x1.ffffffffffffe0p-1023,
    0x1.ffffffffffffep-1023
  },
  { // Entry 110
    0x1.ffffffffffffe0p-1023,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 111
    0x1.ffffffffffffc0p-1023,
    0x1.ffffffffffffcp-1023
  },
  { // Entry 112
    0x1.ffffffffffffc0p-1023,
    -0x1.ffffffffffffcp-1023
  },
  { // Entry 113
    0x1.p-1073,
    0x1.0p-1073
  },
  { // Entry 114
    0x1.p-1073,
    -0x1.0p-1073
  },
  { // Entry 115
    0x1.p-1074,
    0x1.0p-1074
  },
  { // Entry 116
    0x1.p-1074,
    -0x1.0p-1074
  },
  { // Entry 117
    0.0,
    0.0
  },
  { // Entry 118
    0.0,
    -0.0
  }
};
