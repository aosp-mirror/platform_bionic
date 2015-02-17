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

static data_int_1_t<double> g_ilogb_intel_data[] = {
  { // Entry 0
    (int)0x1.90p6,
    0x1.0p100
  },
  { // Entry 1
    (int)0x1.90p6,
    0x1.199999999999ap100
  },
  { // Entry 2
    (int)0x1.90p6,
    0x1.3333333333334p100
  },
  { // Entry 3
    (int)0x1.90p6,
    0x1.4cccccccccccep100
  },
  { // Entry 4
    (int)0x1.90p6,
    0x1.6666666666668p100
  },
  { // Entry 5
    (int)0x1.90p6,
    0x1.8000000000002p100
  },
  { // Entry 6
    (int)0x1.90p6,
    0x1.999999999999cp100
  },
  { // Entry 7
    (int)0x1.90p6,
    0x1.b333333333336p100
  },
  { // Entry 8
    (int)0x1.90p6,
    0x1.cccccccccccd0p100
  },
  { // Entry 9
    (int)0x1.90p6,
    0x1.e66666666666ap100
  },
  { // Entry 10
    (int)0x1.94p6,
    0x1.0p101
  },
  { // Entry 11
    (int)0x1.90p7,
    0x1.0p200
  },
  { // Entry 12
    (int)0x1.90p7,
    0x1.199999999999ap200
  },
  { // Entry 13
    (int)0x1.90p7,
    0x1.3333333333334p200
  },
  { // Entry 14
    (int)0x1.90p7,
    0x1.4cccccccccccep200
  },
  { // Entry 15
    (int)0x1.90p7,
    0x1.6666666666668p200
  },
  { // Entry 16
    (int)0x1.90p7,
    0x1.8000000000002p200
  },
  { // Entry 17
    (int)0x1.90p7,
    0x1.999999999999cp200
  },
  { // Entry 18
    (int)0x1.90p7,
    0x1.b333333333336p200
  },
  { // Entry 19
    (int)0x1.90p7,
    0x1.cccccccccccd0p200
  },
  { // Entry 20
    (int)0x1.90p7,
    0x1.e66666666666ap200
  },
  { // Entry 21
    (int)0x1.92p7,
    0x1.0p201
  },
  { // Entry 22
    (int)0x1.f4p9,
    0x1.0p1000
  },
  { // Entry 23
    (int)0x1.f4p9,
    0x1.199999999999ap1000
  },
  { // Entry 24
    (int)0x1.f4p9,
    0x1.3333333333334p1000
  },
  { // Entry 25
    (int)0x1.f4p9,
    0x1.4cccccccccccep1000
  },
  { // Entry 26
    (int)0x1.f4p9,
    0x1.6666666666668p1000
  },
  { // Entry 27
    (int)0x1.f4p9,
    0x1.8000000000002p1000
  },
  { // Entry 28
    (int)0x1.f4p9,
    0x1.999999999999cp1000
  },
  { // Entry 29
    (int)0x1.f4p9,
    0x1.b333333333336p1000
  },
  { // Entry 30
    (int)0x1.f4p9,
    0x1.cccccccccccd0p1000
  },
  { // Entry 31
    (int)0x1.f4p9,
    0x1.e66666666666ap1000
  },
  { // Entry 32
    (int)0x1.f480p9,
    0x1.0p1001
  },
  { // Entry 33
    (int)0x1.94p6,
    -0x1.0p101
  },
  { // Entry 34
    (int)0x1.90p6,
    -0x1.e666666666666p100
  },
  { // Entry 35
    (int)0x1.90p6,
    -0x1.cccccccccccccp100
  },
  { // Entry 36
    (int)0x1.90p6,
    -0x1.b333333333332p100
  },
  { // Entry 37
    (int)0x1.90p6,
    -0x1.9999999999998p100
  },
  { // Entry 38
    (int)0x1.90p6,
    -0x1.7fffffffffffep100
  },
  { // Entry 39
    (int)0x1.90p6,
    -0x1.6666666666664p100
  },
  { // Entry 40
    (int)0x1.90p6,
    -0x1.4cccccccccccap100
  },
  { // Entry 41
    (int)0x1.90p6,
    -0x1.3333333333330p100
  },
  { // Entry 42
    (int)0x1.90p6,
    -0x1.1999999999996p100
  },
  { // Entry 43
    (int)0x1.90p6,
    -0x1.0p100
  },
  { // Entry 44
    (int)0x1.92p7,
    -0x1.0p201
  },
  { // Entry 45
    (int)0x1.90p7,
    -0x1.e666666666666p200
  },
  { // Entry 46
    (int)0x1.90p7,
    -0x1.cccccccccccccp200
  },
  { // Entry 47
    (int)0x1.90p7,
    -0x1.b333333333332p200
  },
  { // Entry 48
    (int)0x1.90p7,
    -0x1.9999999999998p200
  },
  { // Entry 49
    (int)0x1.90p7,
    -0x1.7fffffffffffep200
  },
  { // Entry 50
    (int)0x1.90p7,
    -0x1.6666666666664p200
  },
  { // Entry 51
    (int)0x1.90p7,
    -0x1.4cccccccccccap200
  },
  { // Entry 52
    (int)0x1.90p7,
    -0x1.3333333333330p200
  },
  { // Entry 53
    (int)0x1.90p7,
    -0x1.1999999999996p200
  },
  { // Entry 54
    (int)0x1.90p7,
    -0x1.0p200
  },
  { // Entry 55
    (int)0x1.f480p9,
    -0x1.0p1001
  },
  { // Entry 56
    (int)0x1.f4p9,
    -0x1.e666666666666p1000
  },
  { // Entry 57
    (int)0x1.f4p9,
    -0x1.cccccccccccccp1000
  },
  { // Entry 58
    (int)0x1.f4p9,
    -0x1.b333333333332p1000
  },
  { // Entry 59
    (int)0x1.f4p9,
    -0x1.9999999999998p1000
  },
  { // Entry 60
    (int)0x1.f4p9,
    -0x1.7fffffffffffep1000
  },
  { // Entry 61
    (int)0x1.f4p9,
    -0x1.6666666666664p1000
  },
  { // Entry 62
    (int)0x1.f4p9,
    -0x1.4cccccccccccap1000
  },
  { // Entry 63
    (int)0x1.f4p9,
    -0x1.3333333333330p1000
  },
  { // Entry 64
    (int)0x1.f4p9,
    -0x1.1999999999996p1000
  },
  { // Entry 65
    (int)0x1.f4p9,
    -0x1.0p1000
  },
  { // Entry 66
    (int)0x1.90p5,
    0x1.0p50
  },
  { // Entry 67
    (int)0x1.90p5,
    0x1.199999999999ap50
  },
  { // Entry 68
    (int)0x1.90p5,
    0x1.3333333333334p50
  },
  { // Entry 69
    (int)0x1.90p5,
    0x1.4cccccccccccep50
  },
  { // Entry 70
    (int)0x1.90p5,
    0x1.6666666666668p50
  },
  { // Entry 71
    (int)0x1.90p5,
    0x1.8000000000002p50
  },
  { // Entry 72
    (int)0x1.90p5,
    0x1.999999999999cp50
  },
  { // Entry 73
    (int)0x1.90p5,
    0x1.b333333333336p50
  },
  { // Entry 74
    (int)0x1.90p5,
    0x1.cccccccccccd0p50
  },
  { // Entry 75
    (int)0x1.90p5,
    0x1.e66666666666ap50
  },
  { // Entry 76
    (int)0x1.98p5,
    0x1.0p51
  },
  { // Entry 77
    (int)0x1.98p5,
    0x1.0p51
  },
  { // Entry 78
    (int)0x1.98p5,
    0x1.199999999999ap51
  },
  { // Entry 79
    (int)0x1.98p5,
    0x1.3333333333334p51
  },
  { // Entry 80
    (int)0x1.98p5,
    0x1.4cccccccccccep51
  },
  { // Entry 81
    (int)0x1.98p5,
    0x1.6666666666668p51
  },
  { // Entry 82
    (int)0x1.98p5,
    0x1.8000000000002p51
  },
  { // Entry 83
    (int)0x1.98p5,
    0x1.999999999999cp51
  },
  { // Entry 84
    (int)0x1.98p5,
    0x1.b333333333336p51
  },
  { // Entry 85
    (int)0x1.98p5,
    0x1.cccccccccccd0p51
  },
  { // Entry 86
    (int)0x1.98p5,
    0x1.e66666666666ap51
  },
  { // Entry 87
    (int)0x1.a0p5,
    0x1.0p52
  },
  { // Entry 88
    (int)0x1.a0p5,
    0x1.0p52
  },
  { // Entry 89
    (int)0x1.a0p5,
    0x1.199999999999ap52
  },
  { // Entry 90
    (int)0x1.a0p5,
    0x1.3333333333334p52
  },
  { // Entry 91
    (int)0x1.a0p5,
    0x1.4cccccccccccep52
  },
  { // Entry 92
    (int)0x1.a0p5,
    0x1.6666666666668p52
  },
  { // Entry 93
    (int)0x1.a0p5,
    0x1.8000000000002p52
  },
  { // Entry 94
    (int)0x1.a0p5,
    0x1.999999999999cp52
  },
  { // Entry 95
    (int)0x1.a0p5,
    0x1.b333333333336p52
  },
  { // Entry 96
    (int)0x1.a0p5,
    0x1.cccccccccccd0p52
  },
  { // Entry 97
    (int)0x1.a0p5,
    0x1.e66666666666ap52
  },
  { // Entry 98
    (int)0x1.a8p5,
    0x1.0p53
  },
  { // Entry 99
    (int)0x1.a8p5,
    0x1.0p53
  },
  { // Entry 100
    (int)0x1.a8p5,
    0x1.199999999999ap53
  },
  { // Entry 101
    (int)0x1.a8p5,
    0x1.3333333333334p53
  },
  { // Entry 102
    (int)0x1.a8p5,
    0x1.4cccccccccccep53
  },
  { // Entry 103
    (int)0x1.a8p5,
    0x1.6666666666668p53
  },
  { // Entry 104
    (int)0x1.a8p5,
    0x1.8000000000002p53
  },
  { // Entry 105
    (int)0x1.a8p5,
    0x1.999999999999cp53
  },
  { // Entry 106
    (int)0x1.a8p5,
    0x1.b333333333336p53
  },
  { // Entry 107
    (int)0x1.a8p5,
    0x1.cccccccccccd0p53
  },
  { // Entry 108
    (int)0x1.a8p5,
    0x1.e66666666666ap53
  },
  { // Entry 109
    (int)0x1.b0p5,
    0x1.0p54
  },
  { // Entry 110
    (int)-0x1.0080p10,
    0x1.0p-1026
  },
  { // Entry 111
    (int)-0x1.p10,
    0x1.d333333333334p-1024
  },
  { // Entry 112
    (int)-0x1.ff80p9,
    0x1.b333333333334p-1023
  },
  { // Entry 113
    (int)-0x1.ffp9,
    0x1.3e66666666667p-1022
  },
  { // Entry 114
    (int)-0x1.ffp9,
    0x1.a333333333334p-1022
  },
  { // Entry 115
    (int)-0x1.fe80p9,
    0x1.040p-1021
  },
  { // Entry 116
    (int)-0x1.fe80p9,
    0x1.3666666666666p-1021
  },
  { // Entry 117
    (int)-0x1.fe80p9,
    0x1.68cccccccccccp-1021
  },
  { // Entry 118
    (int)-0x1.fe80p9,
    0x1.9b33333333332p-1021
  },
  { // Entry 119
    (int)-0x1.fe80p9,
    0x1.cd99999999998p-1021
  },
  { // Entry 120
    (int)-0x1.fe80p9,
    0x1.ffffffffffffep-1021
  },
  { // Entry 121
    (int)0x1.90p5,
    0x1.fffffffffffffp50
  },
  { // Entry 122
    (int)0x1.98p5,
    0x1.0p51
  },
  { // Entry 123
    (int)0x1.98p5,
    0x1.0000000000001p51
  },
  { // Entry 124
    (int)0x1.98p5,
    0x1.fffffffffffffp51
  },
  { // Entry 125
    (int)0x1.a0p5,
    0x1.0p52
  },
  { // Entry 126
    (int)0x1.a0p5,
    0x1.0000000000001p52
  },
  { // Entry 127
    (int)0x1.a0p5,
    0x1.fffffffffffffp52
  },
  { // Entry 128
    (int)0x1.a8p5,
    0x1.0p53
  },
  { // Entry 129
    (int)0x1.a8p5,
    0x1.0000000000001p53
  },
  { // Entry 130
    (int)0x1.98p5,
    -0x1.0000000000001p51
  },
  { // Entry 131
    (int)0x1.98p5,
    -0x1.0p51
  },
  { // Entry 132
    (int)0x1.90p5,
    -0x1.fffffffffffffp50
  },
  { // Entry 133
    (int)0x1.a0p5,
    -0x1.0000000000001p52
  },
  { // Entry 134
    (int)0x1.a0p5,
    -0x1.0p52
  },
  { // Entry 135
    (int)0x1.98p5,
    -0x1.fffffffffffffp51
  },
  { // Entry 136
    (int)0x1.a8p5,
    -0x1.0000000000001p53
  },
  { // Entry 137
    (int)0x1.a8p5,
    -0x1.0p53
  },
  { // Entry 138
    (int)0x1.a0p5,
    -0x1.fffffffffffffp52
  },
  { // Entry 139
    (int)0x1.ff80p9,
    0x1.fffffffffffffp1023
  },
  { // Entry 140
    (int)0x1.ff80p9,
    -0x1.fffffffffffffp1023
  },
  { // Entry 141
    (int)-0x1.c0p2,
    0x1.fffffffffffffp-7
  },
  { // Entry 142
    (int)-0x1.80p2,
    0x1.0p-6
  },
  { // Entry 143
    (int)-0x1.80p2,
    0x1.0000000000001p-6
  },
  { // Entry 144
    (int)-0x1.80p2,
    0x1.fffffffffffffp-6
  },
  { // Entry 145
    (int)-0x1.40p2,
    0x1.0p-5
  },
  { // Entry 146
    (int)-0x1.40p2,
    0x1.0000000000001p-5
  },
  { // Entry 147
    (int)-0x1.40p2,
    0x1.fffffffffffffp-5
  },
  { // Entry 148
    (int)-0x1.p2,
    0x1.0p-4
  },
  { // Entry 149
    (int)-0x1.p2,
    0x1.0000000000001p-4
  },
  { // Entry 150
    (int)-0x1.p2,
    0x1.fffffffffffffp-4
  },
  { // Entry 151
    (int)-0x1.80p1,
    0x1.0p-3
  },
  { // Entry 152
    (int)-0x1.80p1,
    0x1.0000000000001p-3
  },
  { // Entry 153
    (int)-0x1.80p1,
    0x1.fffffffffffffp-3
  },
  { // Entry 154
    (int)-0x1.p1,
    0x1.0p-2
  },
  { // Entry 155
    (int)-0x1.p1,
    0x1.0000000000001p-2
  },
  { // Entry 156
    (int)-0x1.p1,
    0x1.fffffffffffffp-2
  },
  { // Entry 157
    (int)-0x1.p0,
    0x1.0p-1
  },
  { // Entry 158
    (int)-0x1.p0,
    0x1.0000000000001p-1
  },
  { // Entry 159
    (int)-0x1.0c80p10,
    -0x1.0p-1074
  },
  { // Entry 160
    (int)-0x1.fffffffcp30,
    -0.0
  },
  { // Entry 161
    (int)-0x1.0c80p10,
    0x1.0p-1074
  },
  { // Entry 162
    (int)-0x1.p0,
    0x1.fffffffffffffp-1
  },
  { // Entry 163
    (int)0.0,
    0x1.0p0
  },
  { // Entry 164
    (int)0.0,
    0x1.0000000000001p0
  },
  { // Entry 165
    (int)0.0,
    0x1.fffffffffffffp0
  },
  { // Entry 166
    (int)0x1.p0,
    0x1.0p1
  },
  { // Entry 167
    (int)0x1.p0,
    0x1.0000000000001p1
  },
  { // Entry 168
    (int)0x1.p0,
    0x1.fffffffffffffp1
  },
  { // Entry 169
    (int)0x1.p1,
    0x1.0p2
  },
  { // Entry 170
    (int)0x1.p1,
    0x1.0000000000001p2
  },
  { // Entry 171
    (int)0x1.p1,
    0x1.fffffffffffffp2
  },
  { // Entry 172
    (int)0x1.80p1,
    0x1.0p3
  },
  { // Entry 173
    (int)0x1.80p1,
    0x1.0000000000001p3
  },
  { // Entry 174
    (int)0x1.80p1,
    0x1.fffffffffffffp3
  },
  { // Entry 175
    (int)0x1.p2,
    0x1.0p4
  },
  { // Entry 176
    (int)0x1.p2,
    0x1.0000000000001p4
  },
  { // Entry 177
    (int)0x1.p2,
    0x1.fffffffffffffp4
  },
  { // Entry 178
    (int)0x1.40p2,
    0x1.0p5
  },
  { // Entry 179
    (int)0x1.40p2,
    0x1.0000000000001p5
  },
  { // Entry 180
    (int)0x1.40p2,
    0x1.fffffffffffffp5
  },
  { // Entry 181
    (int)0x1.80p2,
    0x1.0p6
  },
  { // Entry 182
    (int)0x1.80p2,
    0x1.0000000000001p6
  },
  { // Entry 183
    (int)0x1.80p2,
    0x1.fffffffffffffp6
  },
  { // Entry 184
    (int)0x1.c0p2,
    0x1.0p7
  },
  { // Entry 185
    (int)0x1.c0p2,
    0x1.0000000000001p7
  },
  { // Entry 186
    (int)0x1.fffffffcp30,
    HUGE_VAL
  },
  { // Entry 187
    (int)0x1.fffffffcp30,
    -HUGE_VAL
  },
  { // Entry 188
    (int)-0x1.fffffffcp30,
    0.0
  },
  { // Entry 189
    (int)-0x1.fffffffcp30,
    -0.0
  },
  { // Entry 190
    (int)0x1.ff80p9,
    0x1.fffffffffffffp1023
  },
  { // Entry 191
    (int)0x1.ff80p9,
    -0x1.fffffffffffffp1023
  },
  { // Entry 192
    (int)0x1.ff80p9,
    0x1.ffffffffffffep1023
  },
  { // Entry 193
    (int)0x1.ff80p9,
    -0x1.ffffffffffffep1023
  },
  { // Entry 194
    (int)0x1.p0,
    0x1.921fb54442d18p1
  },
  { // Entry 195
    (int)0x1.p0,
    -0x1.921fb54442d18p1
  },
  { // Entry 196
    (int)0.0,
    0x1.921fb54442d18p0
  },
  { // Entry 197
    (int)0.0,
    -0x1.921fb54442d18p0
  },
  { // Entry 198
    (int)0.0,
    0x1.0000000000001p0
  },
  { // Entry 199
    (int)0.0,
    -0x1.0000000000001p0
  },
  { // Entry 200
    (int)0.0,
    0x1.0p0
  },
  { // Entry 201
    (int)0.0,
    -0x1.0p0
  },
  { // Entry 202
    (int)-0x1.p0,
    0x1.fffffffffffffp-1
  },
  { // Entry 203
    (int)-0x1.p0,
    -0x1.fffffffffffffp-1
  },
  { // Entry 204
    (int)-0x1.p0,
    0x1.921fb54442d18p-1
  },
  { // Entry 205
    (int)-0x1.p0,
    -0x1.921fb54442d18p-1
  },
  { // Entry 206
    (int)-0x1.ffp9,
    0x1.0000000000001p-1022
  },
  { // Entry 207
    (int)-0x1.ffp9,
    -0x1.0000000000001p-1022
  },
  { // Entry 208
    (int)-0x1.ffp9,
    0x1.0p-1022
  },
  { // Entry 209
    (int)-0x1.ffp9,
    -0x1.0p-1022
  },
  { // Entry 210
    (int)-0x1.ff80p9,
    0x1.ffffffffffffep-1023
  },
  { // Entry 211
    (int)-0x1.ff80p9,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 212
    (int)-0x1.ff80p9,
    0x1.ffffffffffffcp-1023
  },
  { // Entry 213
    (int)-0x1.ff80p9,
    -0x1.ffffffffffffcp-1023
  },
  { // Entry 214
    (int)-0x1.0c40p10,
    0x1.0p-1073
  },
  { // Entry 215
    (int)-0x1.0c40p10,
    -0x1.0p-1073
  },
  { // Entry 216
    (int)-0x1.0c80p10,
    0x1.0p-1074
  },
  { // Entry 217
    (int)-0x1.0c80p10,
    -0x1.0p-1074
  }
};
