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

static data_1_int_1_t<double, double> g_frexp_intel_data[] = {
  { // Entry 0
    0x1.p-1,
    (int)0x1.94p6,
    0x1.0p100
  },
  { // Entry 1
    0x1.199999999999a0p-1,
    (int)0x1.94p6,
    0x1.199999999999ap100
  },
  { // Entry 2
    0x1.33333333333340p-1,
    (int)0x1.94p6,
    0x1.3333333333334p100
  },
  { // Entry 3
    0x1.4ccccccccccce0p-1,
    (int)0x1.94p6,
    0x1.4cccccccccccep100
  },
  { // Entry 4
    0x1.66666666666680p-1,
    (int)0x1.94p6,
    0x1.6666666666668p100
  },
  { // Entry 5
    0x1.80000000000020p-1,
    (int)0x1.94p6,
    0x1.8000000000002p100
  },
  { // Entry 6
    0x1.999999999999c0p-1,
    (int)0x1.94p6,
    0x1.999999999999cp100
  },
  { // Entry 7
    0x1.b3333333333360p-1,
    (int)0x1.94p6,
    0x1.b333333333336p100
  },
  { // Entry 8
    0x1.cccccccccccdp-1,
    (int)0x1.94p6,
    0x1.cccccccccccd0p100
  },
  { // Entry 9
    0x1.e66666666666a0p-1,
    (int)0x1.94p6,
    0x1.e66666666666ap100
  },
  { // Entry 10
    0x1.p-1,
    (int)0x1.98p6,
    0x1.0p101
  },
  { // Entry 11
    0x1.p-1,
    (int)0x1.92p7,
    0x1.0p200
  },
  { // Entry 12
    0x1.199999999999a0p-1,
    (int)0x1.92p7,
    0x1.199999999999ap200
  },
  { // Entry 13
    0x1.33333333333340p-1,
    (int)0x1.92p7,
    0x1.3333333333334p200
  },
  { // Entry 14
    0x1.4ccccccccccce0p-1,
    (int)0x1.92p7,
    0x1.4cccccccccccep200
  },
  { // Entry 15
    0x1.66666666666680p-1,
    (int)0x1.92p7,
    0x1.6666666666668p200
  },
  { // Entry 16
    0x1.80000000000020p-1,
    (int)0x1.92p7,
    0x1.8000000000002p200
  },
  { // Entry 17
    0x1.999999999999c0p-1,
    (int)0x1.92p7,
    0x1.999999999999cp200
  },
  { // Entry 18
    0x1.b3333333333360p-1,
    (int)0x1.92p7,
    0x1.b333333333336p200
  },
  { // Entry 19
    0x1.cccccccccccdp-1,
    (int)0x1.92p7,
    0x1.cccccccccccd0p200
  },
  { // Entry 20
    0x1.e66666666666a0p-1,
    (int)0x1.92p7,
    0x1.e66666666666ap200
  },
  { // Entry 21
    0x1.p-1,
    (int)0x1.94p7,
    0x1.0p201
  },
  { // Entry 22
    0x1.p-1,
    (int)0x1.f480p9,
    0x1.0p1000
  },
  { // Entry 23
    0x1.199999999999a0p-1,
    (int)0x1.f480p9,
    0x1.199999999999ap1000
  },
  { // Entry 24
    0x1.33333333333340p-1,
    (int)0x1.f480p9,
    0x1.3333333333334p1000
  },
  { // Entry 25
    0x1.4ccccccccccce0p-1,
    (int)0x1.f480p9,
    0x1.4cccccccccccep1000
  },
  { // Entry 26
    0x1.66666666666680p-1,
    (int)0x1.f480p9,
    0x1.6666666666668p1000
  },
  { // Entry 27
    0x1.80000000000020p-1,
    (int)0x1.f480p9,
    0x1.8000000000002p1000
  },
  { // Entry 28
    0x1.999999999999c0p-1,
    (int)0x1.f480p9,
    0x1.999999999999cp1000
  },
  { // Entry 29
    0x1.b3333333333360p-1,
    (int)0x1.f480p9,
    0x1.b333333333336p1000
  },
  { // Entry 30
    0x1.cccccccccccdp-1,
    (int)0x1.f480p9,
    0x1.cccccccccccd0p1000
  },
  { // Entry 31
    0x1.e66666666666a0p-1,
    (int)0x1.f480p9,
    0x1.e66666666666ap1000
  },
  { // Entry 32
    0x1.p-1,
    (int)0x1.f5p9,
    0x1.0p1001
  },
  { // Entry 33
    -0x1.p-1,
    (int)0x1.98p6,
    -0x1.0p101
  },
  { // Entry 34
    -0x1.e6666666666660p-1,
    (int)0x1.94p6,
    -0x1.e666666666666p100
  },
  { // Entry 35
    -0x1.ccccccccccccc0p-1,
    (int)0x1.94p6,
    -0x1.cccccccccccccp100
  },
  { // Entry 36
    -0x1.b3333333333320p-1,
    (int)0x1.94p6,
    -0x1.b333333333332p100
  },
  { // Entry 37
    -0x1.99999999999980p-1,
    (int)0x1.94p6,
    -0x1.9999999999998p100
  },
  { // Entry 38
    -0x1.7fffffffffffe0p-1,
    (int)0x1.94p6,
    -0x1.7fffffffffffep100
  },
  { // Entry 39
    -0x1.66666666666640p-1,
    (int)0x1.94p6,
    -0x1.6666666666664p100
  },
  { // Entry 40
    -0x1.4ccccccccccca0p-1,
    (int)0x1.94p6,
    -0x1.4cccccccccccap100
  },
  { // Entry 41
    -0x1.333333333333p-1,
    (int)0x1.94p6,
    -0x1.3333333333330p100
  },
  { // Entry 42
    -0x1.19999999999960p-1,
    (int)0x1.94p6,
    -0x1.1999999999996p100
  },
  { // Entry 43
    -0x1.p-1,
    (int)0x1.94p6,
    -0x1.0p100
  },
  { // Entry 44
    -0x1.p-1,
    (int)0x1.94p7,
    -0x1.0p201
  },
  { // Entry 45
    -0x1.e6666666666660p-1,
    (int)0x1.92p7,
    -0x1.e666666666666p200
  },
  { // Entry 46
    -0x1.ccccccccccccc0p-1,
    (int)0x1.92p7,
    -0x1.cccccccccccccp200
  },
  { // Entry 47
    -0x1.b3333333333320p-1,
    (int)0x1.92p7,
    -0x1.b333333333332p200
  },
  { // Entry 48
    -0x1.99999999999980p-1,
    (int)0x1.92p7,
    -0x1.9999999999998p200
  },
  { // Entry 49
    -0x1.7fffffffffffe0p-1,
    (int)0x1.92p7,
    -0x1.7fffffffffffep200
  },
  { // Entry 50
    -0x1.66666666666640p-1,
    (int)0x1.92p7,
    -0x1.6666666666664p200
  },
  { // Entry 51
    -0x1.4ccccccccccca0p-1,
    (int)0x1.92p7,
    -0x1.4cccccccccccap200
  },
  { // Entry 52
    -0x1.333333333333p-1,
    (int)0x1.92p7,
    -0x1.3333333333330p200
  },
  { // Entry 53
    -0x1.19999999999960p-1,
    (int)0x1.92p7,
    -0x1.1999999999996p200
  },
  { // Entry 54
    -0x1.p-1,
    (int)0x1.92p7,
    -0x1.0p200
  },
  { // Entry 55
    -0x1.p-1,
    (int)0x1.f5p9,
    -0x1.0p1001
  },
  { // Entry 56
    -0x1.e6666666666660p-1,
    (int)0x1.f480p9,
    -0x1.e666666666666p1000
  },
  { // Entry 57
    -0x1.ccccccccccccc0p-1,
    (int)0x1.f480p9,
    -0x1.cccccccccccccp1000
  },
  { // Entry 58
    -0x1.b3333333333320p-1,
    (int)0x1.f480p9,
    -0x1.b333333333332p1000
  },
  { // Entry 59
    -0x1.99999999999980p-1,
    (int)0x1.f480p9,
    -0x1.9999999999998p1000
  },
  { // Entry 60
    -0x1.7fffffffffffe0p-1,
    (int)0x1.f480p9,
    -0x1.7fffffffffffep1000
  },
  { // Entry 61
    -0x1.66666666666640p-1,
    (int)0x1.f480p9,
    -0x1.6666666666664p1000
  },
  { // Entry 62
    -0x1.4ccccccccccca0p-1,
    (int)0x1.f480p9,
    -0x1.4cccccccccccap1000
  },
  { // Entry 63
    -0x1.333333333333p-1,
    (int)0x1.f480p9,
    -0x1.3333333333330p1000
  },
  { // Entry 64
    -0x1.19999999999960p-1,
    (int)0x1.f480p9,
    -0x1.1999999999996p1000
  },
  { // Entry 65
    -0x1.p-1,
    (int)0x1.f480p9,
    -0x1.0p1000
  },
  { // Entry 66
    0x1.p-1,
    (int)0x1.98p5,
    0x1.0p50
  },
  { // Entry 67
    0x1.199999999999a0p-1,
    (int)0x1.98p5,
    0x1.199999999999ap50
  },
  { // Entry 68
    0x1.33333333333340p-1,
    (int)0x1.98p5,
    0x1.3333333333334p50
  },
  { // Entry 69
    0x1.4ccccccccccce0p-1,
    (int)0x1.98p5,
    0x1.4cccccccccccep50
  },
  { // Entry 70
    0x1.66666666666680p-1,
    (int)0x1.98p5,
    0x1.6666666666668p50
  },
  { // Entry 71
    0x1.80000000000020p-1,
    (int)0x1.98p5,
    0x1.8000000000002p50
  },
  { // Entry 72
    0x1.999999999999c0p-1,
    (int)0x1.98p5,
    0x1.999999999999cp50
  },
  { // Entry 73
    0x1.b3333333333360p-1,
    (int)0x1.98p5,
    0x1.b333333333336p50
  },
  { // Entry 74
    0x1.cccccccccccdp-1,
    (int)0x1.98p5,
    0x1.cccccccccccd0p50
  },
  { // Entry 75
    0x1.e66666666666a0p-1,
    (int)0x1.98p5,
    0x1.e66666666666ap50
  },
  { // Entry 76
    0x1.p-1,
    (int)0x1.a0p5,
    0x1.0p51
  },
  { // Entry 77
    0x1.p-1,
    (int)0x1.a0p5,
    0x1.0p51
  },
  { // Entry 78
    0x1.199999999999a0p-1,
    (int)0x1.a0p5,
    0x1.199999999999ap51
  },
  { // Entry 79
    0x1.33333333333340p-1,
    (int)0x1.a0p5,
    0x1.3333333333334p51
  },
  { // Entry 80
    0x1.4ccccccccccce0p-1,
    (int)0x1.a0p5,
    0x1.4cccccccccccep51
  },
  { // Entry 81
    0x1.66666666666680p-1,
    (int)0x1.a0p5,
    0x1.6666666666668p51
  },
  { // Entry 82
    0x1.80000000000020p-1,
    (int)0x1.a0p5,
    0x1.8000000000002p51
  },
  { // Entry 83
    0x1.999999999999c0p-1,
    (int)0x1.a0p5,
    0x1.999999999999cp51
  },
  { // Entry 84
    0x1.b3333333333360p-1,
    (int)0x1.a0p5,
    0x1.b333333333336p51
  },
  { // Entry 85
    0x1.cccccccccccdp-1,
    (int)0x1.a0p5,
    0x1.cccccccccccd0p51
  },
  { // Entry 86
    0x1.e66666666666a0p-1,
    (int)0x1.a0p5,
    0x1.e66666666666ap51
  },
  { // Entry 87
    0x1.p-1,
    (int)0x1.a8p5,
    0x1.0p52
  },
  { // Entry 88
    0x1.p-1,
    (int)0x1.a8p5,
    0x1.0p52
  },
  { // Entry 89
    0x1.199999999999a0p-1,
    (int)0x1.a8p5,
    0x1.199999999999ap52
  },
  { // Entry 90
    0x1.33333333333340p-1,
    (int)0x1.a8p5,
    0x1.3333333333334p52
  },
  { // Entry 91
    0x1.4ccccccccccce0p-1,
    (int)0x1.a8p5,
    0x1.4cccccccccccep52
  },
  { // Entry 92
    0x1.66666666666680p-1,
    (int)0x1.a8p5,
    0x1.6666666666668p52
  },
  { // Entry 93
    0x1.80000000000020p-1,
    (int)0x1.a8p5,
    0x1.8000000000002p52
  },
  { // Entry 94
    0x1.999999999999c0p-1,
    (int)0x1.a8p5,
    0x1.999999999999cp52
  },
  { // Entry 95
    0x1.b3333333333360p-1,
    (int)0x1.a8p5,
    0x1.b333333333336p52
  },
  { // Entry 96
    0x1.cccccccccccdp-1,
    (int)0x1.a8p5,
    0x1.cccccccccccd0p52
  },
  { // Entry 97
    0x1.e66666666666a0p-1,
    (int)0x1.a8p5,
    0x1.e66666666666ap52
  },
  { // Entry 98
    0x1.p-1,
    (int)0x1.b0p5,
    0x1.0p53
  },
  { // Entry 99
    0x1.p-1,
    (int)0x1.b0p5,
    0x1.0p53
  },
  { // Entry 100
    0x1.199999999999a0p-1,
    (int)0x1.b0p5,
    0x1.199999999999ap53
  },
  { // Entry 101
    0x1.33333333333340p-1,
    (int)0x1.b0p5,
    0x1.3333333333334p53
  },
  { // Entry 102
    0x1.4ccccccccccce0p-1,
    (int)0x1.b0p5,
    0x1.4cccccccccccep53
  },
  { // Entry 103
    0x1.66666666666680p-1,
    (int)0x1.b0p5,
    0x1.6666666666668p53
  },
  { // Entry 104
    0x1.80000000000020p-1,
    (int)0x1.b0p5,
    0x1.8000000000002p53
  },
  { // Entry 105
    0x1.999999999999c0p-1,
    (int)0x1.b0p5,
    0x1.999999999999cp53
  },
  { // Entry 106
    0x1.b3333333333360p-1,
    (int)0x1.b0p5,
    0x1.b333333333336p53
  },
  { // Entry 107
    0x1.cccccccccccdp-1,
    (int)0x1.b0p5,
    0x1.cccccccccccd0p53
  },
  { // Entry 108
    0x1.e66666666666a0p-1,
    (int)0x1.b0p5,
    0x1.e66666666666ap53
  },
  { // Entry 109
    0x1.p-1,
    (int)0x1.b8p5,
    0x1.0p54
  },
  { // Entry 110
    0x1.p-1,
    (int)-0x1.0040p10,
    0x1.0p-1026
  },
  { // Entry 111
    0x1.d3333333333340p-1,
    (int)-0x1.ff80p9,
    0x1.d333333333334p-1024
  },
  { // Entry 112
    0x1.b3333333333340p-1,
    (int)-0x1.ffp9,
    0x1.b333333333334p-1023
  },
  { // Entry 113
    0x1.3e666666666670p-1,
    (int)-0x1.fe80p9,
    0x1.3e66666666667p-1022
  },
  { // Entry 114
    0x1.a3333333333340p-1,
    (int)-0x1.fe80p9,
    0x1.a333333333334p-1022
  },
  { // Entry 115
    0x1.04p-1,
    (int)-0x1.fep9,
    0x1.040p-1021
  },
  { // Entry 116
    0x1.36666666666660p-1,
    (int)-0x1.fep9,
    0x1.3666666666666p-1021
  },
  { // Entry 117
    0x1.68ccccccccccc0p-1,
    (int)-0x1.fep9,
    0x1.68cccccccccccp-1021
  },
  { // Entry 118
    0x1.9b333333333320p-1,
    (int)-0x1.fep9,
    0x1.9b33333333332p-1021
  },
  { // Entry 119
    0x1.cd999999999980p-1,
    (int)-0x1.fep9,
    0x1.cd99999999998p-1021
  },
  { // Entry 120
    0x1.ffffffffffffe0p-1,
    (int)-0x1.fep9,
    0x1.ffffffffffffep-1021
  },
  { // Entry 121
    0x1.fffffffffffff0p-1,
    (int)0x1.98p5,
    0x1.fffffffffffffp50
  },
  { // Entry 122
    0x1.p-1,
    (int)0x1.a0p5,
    0x1.0p51
  },
  { // Entry 123
    0x1.00000000000010p-1,
    (int)0x1.a0p5,
    0x1.0000000000001p51
  },
  { // Entry 124
    0x1.fffffffffffff0p-1,
    (int)0x1.a0p5,
    0x1.fffffffffffffp51
  },
  { // Entry 125
    0x1.p-1,
    (int)0x1.a8p5,
    0x1.0p52
  },
  { // Entry 126
    0x1.00000000000010p-1,
    (int)0x1.a8p5,
    0x1.0000000000001p52
  },
  { // Entry 127
    0x1.fffffffffffff0p-1,
    (int)0x1.a8p5,
    0x1.fffffffffffffp52
  },
  { // Entry 128
    0x1.p-1,
    (int)0x1.b0p5,
    0x1.0p53
  },
  { // Entry 129
    0x1.00000000000010p-1,
    (int)0x1.b0p5,
    0x1.0000000000001p53
  },
  { // Entry 130
    -0x1.00000000000010p-1,
    (int)0x1.a0p5,
    -0x1.0000000000001p51
  },
  { // Entry 131
    -0x1.p-1,
    (int)0x1.a0p5,
    -0x1.0p51
  },
  { // Entry 132
    -0x1.fffffffffffff0p-1,
    (int)0x1.98p5,
    -0x1.fffffffffffffp50
  },
  { // Entry 133
    -0x1.00000000000010p-1,
    (int)0x1.a8p5,
    -0x1.0000000000001p52
  },
  { // Entry 134
    -0x1.p-1,
    (int)0x1.a8p5,
    -0x1.0p52
  },
  { // Entry 135
    -0x1.fffffffffffff0p-1,
    (int)0x1.a0p5,
    -0x1.fffffffffffffp51
  },
  { // Entry 136
    -0x1.00000000000010p-1,
    (int)0x1.b0p5,
    -0x1.0000000000001p53
  },
  { // Entry 137
    -0x1.p-1,
    (int)0x1.b0p5,
    -0x1.0p53
  },
  { // Entry 138
    -0x1.fffffffffffff0p-1,
    (int)0x1.a8p5,
    -0x1.fffffffffffffp52
  },
  { // Entry 139
    0x1.fffffffffffff0p-1,
    (int)0x1.p10,
    0x1.fffffffffffffp1023
  },
  { // Entry 140
    -0x1.fffffffffffff0p-1,
    (int)0x1.p10,
    -0x1.fffffffffffffp1023
  },
  { // Entry 141
    0x1.fffffffffffff0p-1,
    (int)-0x1.80p2,
    0x1.fffffffffffffp-7
  },
  { // Entry 142
    0x1.p-1,
    (int)-0x1.40p2,
    0x1.0p-6
  },
  { // Entry 143
    0x1.00000000000010p-1,
    (int)-0x1.40p2,
    0x1.0000000000001p-6
  },
  { // Entry 144
    0x1.fffffffffffff0p-1,
    (int)-0x1.40p2,
    0x1.fffffffffffffp-6
  },
  { // Entry 145
    0x1.p-1,
    (int)-0x1.p2,
    0x1.0p-5
  },
  { // Entry 146
    0x1.00000000000010p-1,
    (int)-0x1.p2,
    0x1.0000000000001p-5
  },
  { // Entry 147
    0x1.fffffffffffff0p-1,
    (int)-0x1.p2,
    0x1.fffffffffffffp-5
  },
  { // Entry 148
    0x1.p-1,
    (int)-0x1.80p1,
    0x1.0p-4
  },
  { // Entry 149
    0x1.00000000000010p-1,
    (int)-0x1.80p1,
    0x1.0000000000001p-4
  },
  { // Entry 150
    0x1.fffffffffffff0p-1,
    (int)-0x1.80p1,
    0x1.fffffffffffffp-4
  },
  { // Entry 151
    0x1.p-1,
    (int)-0x1.p1,
    0x1.0p-3
  },
  { // Entry 152
    0x1.00000000000010p-1,
    (int)-0x1.p1,
    0x1.0000000000001p-3
  },
  { // Entry 153
    0x1.fffffffffffff0p-1,
    (int)-0x1.p1,
    0x1.fffffffffffffp-3
  },
  { // Entry 154
    0x1.p-1,
    (int)-0x1.p0,
    0x1.0p-2
  },
  { // Entry 155
    0x1.00000000000010p-1,
    (int)-0x1.p0,
    0x1.0000000000001p-2
  },
  { // Entry 156
    0x1.fffffffffffff0p-1,
    (int)-0x1.p0,
    0x1.fffffffffffffp-2
  },
  { // Entry 157
    0x1.p-1,
    (int)0.0,
    0x1.0p-1
  },
  { // Entry 158
    0x1.00000000000010p-1,
    (int)0.0,
    0x1.0000000000001p-1
  },
  { // Entry 159
    -0x1.p-1,
    (int)-0x1.0c40p10,
    -0x1.0p-1074
  },
  { // Entry 160
    -0.0,
    (int)0.0,
    -0.0
  },
  { // Entry 161
    0x1.p-1,
    (int)-0x1.0c40p10,
    0x1.0p-1074
  },
  { // Entry 162
    0x1.fffffffffffff0p-1,
    (int)0.0,
    0x1.fffffffffffffp-1
  },
  { // Entry 163
    0x1.p-1,
    (int)0x1.p0,
    0x1.0p0
  },
  { // Entry 164
    0x1.00000000000010p-1,
    (int)0x1.p0,
    0x1.0000000000001p0
  },
  { // Entry 165
    0x1.fffffffffffff0p-1,
    (int)0x1.p0,
    0x1.fffffffffffffp0
  },
  { // Entry 166
    0x1.p-1,
    (int)0x1.p1,
    0x1.0p1
  },
  { // Entry 167
    0x1.00000000000010p-1,
    (int)0x1.p1,
    0x1.0000000000001p1
  },
  { // Entry 168
    0x1.fffffffffffff0p-1,
    (int)0x1.p1,
    0x1.fffffffffffffp1
  },
  { // Entry 169
    0x1.p-1,
    (int)0x1.80p1,
    0x1.0p2
  },
  { // Entry 170
    0x1.00000000000010p-1,
    (int)0x1.80p1,
    0x1.0000000000001p2
  },
  { // Entry 171
    0x1.fffffffffffff0p-1,
    (int)0x1.80p1,
    0x1.fffffffffffffp2
  },
  { // Entry 172
    0x1.p-1,
    (int)0x1.p2,
    0x1.0p3
  },
  { // Entry 173
    0x1.00000000000010p-1,
    (int)0x1.p2,
    0x1.0000000000001p3
  },
  { // Entry 174
    0x1.fffffffffffff0p-1,
    (int)0x1.p2,
    0x1.fffffffffffffp3
  },
  { // Entry 175
    0x1.p-1,
    (int)0x1.40p2,
    0x1.0p4
  },
  { // Entry 176
    0x1.00000000000010p-1,
    (int)0x1.40p2,
    0x1.0000000000001p4
  },
  { // Entry 177
    0x1.fffffffffffff0p-1,
    (int)0x1.40p2,
    0x1.fffffffffffffp4
  },
  { // Entry 178
    0x1.p-1,
    (int)0x1.80p2,
    0x1.0p5
  },
  { // Entry 179
    0x1.00000000000010p-1,
    (int)0x1.80p2,
    0x1.0000000000001p5
  },
  { // Entry 180
    0x1.fffffffffffff0p-1,
    (int)0x1.80p2,
    0x1.fffffffffffffp5
  },
  { // Entry 181
    0x1.p-1,
    (int)0x1.c0p2,
    0x1.0p6
  },
  { // Entry 182
    0x1.00000000000010p-1,
    (int)0x1.c0p2,
    0x1.0000000000001p6
  },
  { // Entry 183
    0x1.fffffffffffff0p-1,
    (int)0x1.c0p2,
    0x1.fffffffffffffp6
  },
  { // Entry 184
    0x1.p-1,
    (int)0x1.p3,
    0x1.0p7
  },
  { // Entry 185
    0x1.00000000000010p-1,
    (int)0x1.p3,
    0x1.0000000000001p7
  },
  { // Entry 186
    HUGE_VAL,
    (int)0,
    HUGE_VAL
  },
  { // Entry 187
    -HUGE_VAL,
    (int)0,
    -HUGE_VAL
  },
  { // Entry 188
    0.0,
    (int)0.0,
    0.0
  },
  { // Entry 189
    -0.0,
    (int)0.0,
    -0.0
  },
  { // Entry 190
    0x1.fffffffffffff0p-1,
    (int)0x1.p10,
    0x1.fffffffffffffp1023
  },
  { // Entry 191
    -0x1.fffffffffffff0p-1,
    (int)0x1.p10,
    -0x1.fffffffffffffp1023
  },
  { // Entry 192
    0x1.ffffffffffffe0p-1,
    (int)0x1.p10,
    0x1.ffffffffffffep1023
  },
  { // Entry 193
    -0x1.ffffffffffffe0p-1,
    (int)0x1.p10,
    -0x1.ffffffffffffep1023
  },
  { // Entry 194
    0x1.921fb54442d180p-1,
    (int)0x1.p1,
    0x1.921fb54442d18p1
  },
  { // Entry 195
    -0x1.921fb54442d180p-1,
    (int)0x1.p1,
    -0x1.921fb54442d18p1
  },
  { // Entry 196
    0x1.921fb54442d180p-1,
    (int)0x1.p0,
    0x1.921fb54442d18p0
  },
  { // Entry 197
    -0x1.921fb54442d180p-1,
    (int)0x1.p0,
    -0x1.921fb54442d18p0
  },
  { // Entry 198
    0x1.00000000000010p-1,
    (int)0x1.p0,
    0x1.0000000000001p0
  },
  { // Entry 199
    -0x1.00000000000010p-1,
    (int)0x1.p0,
    -0x1.0000000000001p0
  },
  { // Entry 200
    0x1.p-1,
    (int)0x1.p0,
    0x1.0p0
  },
  { // Entry 201
    -0x1.p-1,
    (int)0x1.p0,
    -0x1.0p0
  },
  { // Entry 202
    0x1.fffffffffffff0p-1,
    (int)0.0,
    0x1.fffffffffffffp-1
  },
  { // Entry 203
    -0x1.fffffffffffff0p-1,
    (int)0.0,
    -0x1.fffffffffffffp-1
  },
  { // Entry 204
    0x1.921fb54442d180p-1,
    (int)0.0,
    0x1.921fb54442d18p-1
  },
  { // Entry 205
    -0x1.921fb54442d180p-1,
    (int)0.0,
    -0x1.921fb54442d18p-1
  },
  { // Entry 206
    0x1.00000000000010p-1,
    (int)-0x1.fe80p9,
    0x1.0000000000001p-1022
  },
  { // Entry 207
    -0x1.00000000000010p-1,
    (int)-0x1.fe80p9,
    -0x1.0000000000001p-1022
  },
  { // Entry 208
    0x1.p-1,
    (int)-0x1.fe80p9,
    0x1.0p-1022
  },
  { // Entry 209
    -0x1.p-1,
    (int)-0x1.fe80p9,
    -0x1.0p-1022
  },
  { // Entry 210
    0x1.ffffffffffffe0p-1,
    (int)-0x1.ffp9,
    0x1.ffffffffffffep-1023
  },
  { // Entry 211
    -0x1.ffffffffffffe0p-1,
    (int)-0x1.ffp9,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 212
    0x1.ffffffffffffc0p-1,
    (int)-0x1.ffp9,
    0x1.ffffffffffffcp-1023
  },
  { // Entry 213
    -0x1.ffffffffffffc0p-1,
    (int)-0x1.ffp9,
    -0x1.ffffffffffffcp-1023
  },
  { // Entry 214
    0x1.p-1,
    (int)-0x1.0cp10,
    0x1.0p-1073
  },
  { // Entry 215
    -0x1.p-1,
    (int)-0x1.0cp10,
    -0x1.0p-1073
  },
  { // Entry 216
    0x1.p-1,
    (int)-0x1.0c40p10,
    0x1.0p-1074
  },
  { // Entry 217
    -0x1.p-1,
    (int)-0x1.0c40p10,
    -0x1.0p-1074
  }
};
