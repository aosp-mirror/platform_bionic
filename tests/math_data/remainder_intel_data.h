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

static data_1_2_t<double, double, double> g_remainder_intel_data[] = {
  { // Entry 0
    -0x1.p-51,
    -0x1.4p1,
    -0x1.3ffffffffffffp1
  },
  { // Entry 1
    0x1.c0p46,
    -0x1.8888888888888p100,
    -0x1.1111111111111p95
  },
  { // Entry 2
    0x1.0c6f7a20p-16,
    -0x1.b155555555555p9,
    -0x1.b15555db8d126p9
  },
  { // Entry 3
    -0.0,
    -0x1.0p-117,
    -0x1.0p-117
  },
  { // Entry 4
    -0.0,
    -0x1.0p-117,
    0x1.0p-117
  },
  { // Entry 5
    0.0,
    0x1.0p-117,
    -0x1.0p-117
  },
  { // Entry 6
    0.0,
    0x1.0p-117,
    0x1.0p-117
  },
  { // Entry 7
    -0x1.p-117,
    -0x1.0p-117,
    0x1.0p15
  },
  { // Entry 8
    -0x1.p-117,
    -0x1.0p-117,
    0x1.0p16
  },
  { // Entry 9
    0x1.p-117,
    0x1.0p-117,
    0x1.0p15
  },
  { // Entry 10
    0x1.p-117,
    0x1.0p-117,
    0x1.0p16
  },
  { // Entry 11
    -0x1.p-117,
    -0x1.0p-117,
    0x1.0p117
  },
  { // Entry 12
    -0x1.p-117,
    -0x1.0p-117,
    0x1.0p118
  },
  { // Entry 13
    0x1.p-117,
    0x1.0p-117,
    0x1.0p117
  },
  { // Entry 14
    0x1.p-117,
    0x1.0p-117,
    0x1.0p118
  },
  { // Entry 15
    0.0,
    0x1.0p15,
    -0x1.0p-117
  },
  { // Entry 16
    0.0,
    0x1.0p15,
    0x1.0p-117
  },
  { // Entry 17
    0.0,
    0x1.0p16,
    -0x1.0p-117
  },
  { // Entry 18
    0.0,
    0x1.0p16,
    0x1.0p-117
  },
  { // Entry 19
    0.0,
    0x1.0p15,
    0x1.0p15
  },
  { // Entry 20
    0x1.p15,
    0x1.0p15,
    0x1.0p16
  },
  { // Entry 21
    0.0,
    0x1.0p16,
    0x1.0p15
  },
  { // Entry 22
    0.0,
    0x1.0p16,
    0x1.0p16
  },
  { // Entry 23
    0x1.p15,
    0x1.0p15,
    0x1.0p117
  },
  { // Entry 24
    0x1.p15,
    0x1.0p15,
    0x1.0p118
  },
  { // Entry 25
    0x1.p16,
    0x1.0p16,
    0x1.0p117
  },
  { // Entry 26
    0x1.p16,
    0x1.0p16,
    0x1.0p118
  },
  { // Entry 27
    0.0,
    0x1.0p117,
    -0x1.0p-117
  },
  { // Entry 28
    0.0,
    0x1.0p117,
    0x1.0p-117
  },
  { // Entry 29
    0.0,
    0x1.0p118,
    -0x1.0p-117
  },
  { // Entry 30
    0.0,
    0x1.0p118,
    0x1.0p-117
  },
  { // Entry 31
    0.0,
    0x1.0p117,
    0x1.0p15
  },
  { // Entry 32
    0.0,
    0x1.0p117,
    0x1.0p16
  },
  { // Entry 33
    0.0,
    0x1.0p118,
    0x1.0p15
  },
  { // Entry 34
    0.0,
    0x1.0p118,
    0x1.0p16
  },
  { // Entry 35
    0.0,
    0x1.0p117,
    0x1.0p117
  },
  { // Entry 36
    0x1.p117,
    0x1.0p117,
    0x1.0p118
  },
  { // Entry 37
    0.0,
    0x1.0p118,
    0x1.0p117
  },
  { // Entry 38
    0.0,
    0x1.0p118,
    0x1.0p118
  },
  { // Entry 39
    0.0,
    0x1.9p6,
    0x1.4p3
  },
  { // Entry 40
    0x1.p0,
    0x1.9p6,
    0x1.6p3
  },
  { // Entry 41
    0x1.p2,
    0x1.9p6,
    0x1.8p3
  },
  { // Entry 42
    0x1.p0,
    0x1.940p6,
    0x1.4p3
  },
  { // Entry 43
    0x1.p1,
    0x1.940p6,
    0x1.6p3
  },
  { // Entry 44
    0x1.40p2,
    0x1.940p6,
    0x1.8p3
  },
  { // Entry 45
    0x1.p1,
    0x1.980p6,
    0x1.4p3
  },
  { // Entry 46
    0x1.80p1,
    0x1.980p6,
    0x1.6p3
  },
  { // Entry 47
    0x1.80p2,
    0x1.980p6,
    0x1.8p3
  },
  { // Entry 48
    0x1.80p1,
    0x1.9c0p6,
    0x1.4p3
  },
  { // Entry 49
    0x1.p2,
    0x1.9c0p6,
    0x1.6p3
  },
  { // Entry 50
    -0x1.40p2,
    0x1.9c0p6,
    0x1.8p3
  },
  { // Entry 51
    0x1.p2,
    0x1.ap6,
    0x1.4p3
  },
  { // Entry 52
    0x1.40p2,
    0x1.ap6,
    0x1.6p3
  },
  { // Entry 53
    -0x1.p2,
    0x1.ap6,
    0x1.8p3
  },
  { // Entry 54
    0x1.40p2,
    0x1.a40p6,
    0x1.4p3
  },
  { // Entry 55
    -0x1.40p2,
    0x1.a40p6,
    0x1.6p3
  },
  { // Entry 56
    -0x1.80p1,
    0x1.a40p6,
    0x1.8p3
  },
  { // Entry 57
    -0x1.p2,
    0x1.a80p6,
    0x1.4p3
  },
  { // Entry 58
    -0x1.p2,
    0x1.a80p6,
    0x1.6p3
  },
  { // Entry 59
    -0x1.p1,
    0x1.a80p6,
    0x1.8p3
  },
  { // Entry 60
    -0x1.80p1,
    0x1.ac0p6,
    0x1.4p3
  },
  { // Entry 61
    -0x1.80p1,
    0x1.ac0p6,
    0x1.6p3
  },
  { // Entry 62
    -0x1.p0,
    0x1.ac0p6,
    0x1.8p3
  },
  { // Entry 63
    -0x1.p1,
    0x1.bp6,
    0x1.4p3
  },
  { // Entry 64
    -0x1.p1,
    0x1.bp6,
    0x1.6p3
  },
  { // Entry 65
    0.0,
    0x1.bp6,
    0x1.8p3
  },
  { // Entry 66
    -0x1.p0,
    0x1.b40p6,
    0x1.4p3
  },
  { // Entry 67
    -0x1.p0,
    0x1.b40p6,
    0x1.6p3
  },
  { // Entry 68
    0x1.p0,
    0x1.b40p6,
    0x1.8p3
  },
  { // Entry 69
    0.0,
    0x1.b80p6,
    0x1.4p3
  },
  { // Entry 70
    0.0,
    0x1.b80p6,
    0x1.6p3
  },
  { // Entry 71
    0x1.p1,
    0x1.b80p6,
    0x1.8p3
  },
  { // Entry 72
    -0.0,
    -0x1.0000000000001p0,
    -0x1.0000000000001p0
  },
  { // Entry 73
    -0x1.p-52,
    -0x1.0000000000001p0,
    -0x1.0p0
  },
  { // Entry 74
    -0x1.80p-52,
    -0x1.0000000000001p0,
    -0x1.fffffffffffffp-1
  },
  { // Entry 75
    0x1.p-52,
    -0x1.0p0,
    -0x1.0000000000001p0
  },
  { // Entry 76
    -0.0,
    -0x1.0p0,
    -0x1.0p0
  },
  { // Entry 77
    -0x1.p-53,
    -0x1.0p0,
    -0x1.fffffffffffffp-1
  },
  { // Entry 78
    0x1.80p-52,
    -0x1.fffffffffffffp-1,
    -0x1.0000000000001p0
  },
  { // Entry 79
    0x1.p-53,
    -0x1.fffffffffffffp-1,
    -0x1.0p0
  },
  { // Entry 80
    -0.0,
    -0x1.fffffffffffffp-1,
    -0x1.fffffffffffffp-1
  },
  { // Entry 81
    -0x1.80p-52,
    -0x1.0000000000001p0,
    0x1.fffffffffffffp-1
  },
  { // Entry 82
    -0x1.p-52,
    -0x1.0000000000001p0,
    0x1.0p0
  },
  { // Entry 83
    -0.0,
    -0x1.0000000000001p0,
    0x1.0000000000001p0
  },
  { // Entry 84
    -0x1.p-53,
    -0x1.0p0,
    0x1.fffffffffffffp-1
  },
  { // Entry 85
    -0.0,
    -0x1.0p0,
    0x1.0p0
  },
  { // Entry 86
    0x1.p-52,
    -0x1.0p0,
    0x1.0000000000001p0
  },
  { // Entry 87
    -0.0,
    -0x1.fffffffffffffp-1,
    0x1.fffffffffffffp-1
  },
  { // Entry 88
    0x1.p-53,
    -0x1.fffffffffffffp-1,
    0x1.0p0
  },
  { // Entry 89
    0x1.80p-52,
    -0x1.fffffffffffffp-1,
    0x1.0000000000001p0
  },
  { // Entry 90
    -0x1.80p-52,
    0x1.fffffffffffffp-1,
    -0x1.0000000000001p0
  },
  { // Entry 91
    -0x1.p-53,
    0x1.fffffffffffffp-1,
    -0x1.0p0
  },
  { // Entry 92
    0.0,
    0x1.fffffffffffffp-1,
    -0x1.fffffffffffffp-1
  },
  { // Entry 93
    -0x1.p-52,
    0x1.0p0,
    -0x1.0000000000001p0
  },
  { // Entry 94
    0.0,
    0x1.0p0,
    -0x1.0p0
  },
  { // Entry 95
    0x1.p-53,
    0x1.0p0,
    -0x1.fffffffffffffp-1
  },
  { // Entry 96
    0.0,
    0x1.0000000000001p0,
    -0x1.0000000000001p0
  },
  { // Entry 97
    0x1.p-52,
    0x1.0000000000001p0,
    -0x1.0p0
  },
  { // Entry 98
    0x1.80p-52,
    0x1.0000000000001p0,
    -0x1.fffffffffffffp-1
  },
  { // Entry 99
    0.0,
    0x1.fffffffffffffp-1,
    0x1.fffffffffffffp-1
  },
  { // Entry 100
    -0x1.p-53,
    0x1.fffffffffffffp-1,
    0x1.0p0
  },
  { // Entry 101
    -0x1.80p-52,
    0x1.fffffffffffffp-1,
    0x1.0000000000001p0
  },
  { // Entry 102
    0x1.p-53,
    0x1.0p0,
    0x1.fffffffffffffp-1
  },
  { // Entry 103
    0.0,
    0x1.0p0,
    0x1.0p0
  },
  { // Entry 104
    -0x1.p-52,
    0x1.0p0,
    0x1.0000000000001p0
  },
  { // Entry 105
    0x1.80p-52,
    0x1.0000000000001p0,
    0x1.fffffffffffffp-1
  },
  { // Entry 106
    0x1.p-52,
    0x1.0000000000001p0,
    0x1.0p0
  },
  { // Entry 107
    0.0,
    0x1.0000000000001p0,
    0x1.0000000000001p0
  },
  { // Entry 108
    -0.0,
    -0x1.0p-1074,
    0x1.0p-1074
  },
  { // Entry 109
    -0.0,
    -0.0,
    0x1.0p-1074
  },
  { // Entry 110
    0.0,
    0x1.0p-1074,
    0x1.0p-1074
  },
  { // Entry 111
    -0.0,
    -0x1.0p-1074,
    -0x1.0p-1074
  },
  { // Entry 112
    -0.0,
    -0.0,
    -0x1.0p-1074
  },
  { // Entry 113
    0.0,
    0x1.0p-1074,
    -0x1.0p-1074
  },
  { // Entry 114
    -0x1.p-1074,
    -0x1.0p-1074,
    0x1.fffffffffffffp1023
  },
  { // Entry 115
    -0.0,
    -0.0,
    0x1.fffffffffffffp1023
  },
  { // Entry 116
    0x1.p-1074,
    0x1.0p-1074,
    0x1.fffffffffffffp1023
  },
  { // Entry 117
    -0x1.p-1074,
    -0x1.0p-1074,
    -0x1.fffffffffffffp1023
  },
  { // Entry 118
    -0.0,
    -0.0,
    -0x1.fffffffffffffp1023
  },
  { // Entry 119
    0x1.p-1074,
    0x1.0p-1074,
    -0x1.fffffffffffffp1023
  },
  { // Entry 120
    0x1.p-1074,
    0x1.0p-1074,
    0x1.fffffffffffffp1023
  },
  { // Entry 121
    -0x1.p-1074,
    -0x1.0p-1074,
    -0x1.fffffffffffffp1023
  },
  { // Entry 122
    -0x1.p-1074,
    -0x1.0p-1074,
    0x1.fffffffffffffp1023
  },
  { // Entry 123
    0x1.p-1074,
    0x1.0p-1074,
    -0x1.fffffffffffffp1023
  },
  { // Entry 124
    0.0,
    0x1.fffffffffffffp1023,
    0x1.0p-1074
  },
  { // Entry 125
    -0.0,
    -0x1.fffffffffffffp1023,
    -0x1.0p-1074
  },
  { // Entry 126
    -0.0,
    -0x1.fffffffffffffp1023,
    0x1.0p-1074
  },
  { // Entry 127
    0.0,
    0x1.fffffffffffffp1023,
    -0x1.0p-1074
  },
  { // Entry 128
    0.0,
    0x1.fffffffffffffp1023,
    0x1.fffffffffffffp1023
  },
  { // Entry 129
    0.0,
    0x1.fffffffffffffp1023,
    -0x1.fffffffffffffp1023
  },
  { // Entry 130
    -0.0,
    -0x1.fffffffffffffp1023,
    0x1.fffffffffffffp1023
  },
  { // Entry 131
    -0.0,
    -0x1.fffffffffffffp1023,
    -0x1.fffffffffffffp1023
  },
  { // Entry 132
    0x1.ffffffffffffc0p-3,
    -0x1.0000000000001p51,
    0x1.fffffffffffffp-1
  },
  { // Entry 133
    -0x1.p-1,
    -0x1.0000000000001p51,
    0x1.0p0
  },
  { // Entry 134
    -0.0,
    -0x1.0000000000001p51,
    0x1.0000000000001p0
  },
  { // Entry 135
    -0x1.p-2,
    -0x1.0p51,
    0x1.fffffffffffffp-1
  },
  { // Entry 136
    -0.0,
    -0x1.0p51,
    0x1.0p0
  },
  { // Entry 137
    0x1.p-1,
    -0x1.0p51,
    0x1.0000000000001p0
  },
  { // Entry 138
    -0.0,
    -0x1.fffffffffffffp50,
    0x1.fffffffffffffp-1
  },
  { // Entry 139
    0x1.p-2,
    -0x1.fffffffffffffp50,
    0x1.0p0
  },
  { // Entry 140
    -0x1.00000000000040p-2,
    -0x1.fffffffffffffp50,
    0x1.0000000000001p0
  },
  { // Entry 141
    0.0,
    0x1.fffffffffffffp51,
    0x1.fffffffffffffp-1
  },
  { // Entry 142
    -0x1.p-1,
    0x1.fffffffffffffp51,
    0x1.0p0
  },
  { // Entry 143
    -0x1.ffffffffffffc0p-2,
    0x1.fffffffffffffp51,
    0x1.0000000000001p0
  },
  { // Entry 144
    -0x1.ffffffffffffe0p-2,
    0x1.0p52,
    0x1.fffffffffffffp-1
  },
  { // Entry 145
    0.0,
    0x1.0p52,
    0x1.0p0
  },
  { // Entry 146
    0x1.p-52,
    0x1.0p52,
    0x1.0000000000001p0
  },
  { // Entry 147
    -0x1.ffffffffffffc0p-2,
    0x1.0000000000001p52,
    0x1.fffffffffffffp-1
  },
  { // Entry 148
    0.0,
    0x1.0000000000001p52,
    0x1.0p0
  },
  { // Entry 149
    0.0,
    0x1.0000000000001p52,
    0x1.0000000000001p0
  },
  { // Entry 150
    -0x1.80p-52,
    -0x1.0000000000001p53,
    0x1.fffffffffffffp-1
  },
  { // Entry 151
    -0.0,
    -0x1.0000000000001p53,
    0x1.0p0
  },
  { // Entry 152
    -0.0,
    -0x1.0000000000001p53,
    0x1.0000000000001p0
  },
  { // Entry 153
    -0x1.p-53,
    -0x1.0p53,
    0x1.fffffffffffffp-1
  },
  { // Entry 154
    -0.0,
    -0x1.0p53,
    0x1.0p0
  },
  { // Entry 155
    -0x1.p-51,
    -0x1.0p53,
    0x1.0000000000001p0
  },
  { // Entry 156
    -0.0,
    -0x1.fffffffffffffp52,
    0x1.fffffffffffffp-1
  },
  { // Entry 157
    -0.0,
    -0x1.fffffffffffffp52,
    0x1.0p0
  },
  { // Entry 158
    -0x1.80p-51,
    -0x1.fffffffffffffp52,
    0x1.0000000000001p0
  },
  { // Entry 159
    0.0,
    0x1.fffffffffffffp50,
    0x1.fffffffffffffp-1
  },
  { // Entry 160
    -0x1.p-2,
    0x1.fffffffffffffp50,
    0x1.0p0
  },
  { // Entry 161
    0x1.00000000000040p-2,
    0x1.fffffffffffffp50,
    0x1.0000000000001p0
  },
  { // Entry 162
    0x1.p-2,
    0x1.0p51,
    0x1.fffffffffffffp-1
  },
  { // Entry 163
    0.0,
    0x1.0p51,
    0x1.0p0
  },
  { // Entry 164
    -0x1.p-1,
    0x1.0p51,
    0x1.0000000000001p0
  },
  { // Entry 165
    -0x1.ffffffffffffc0p-3,
    0x1.0000000000001p51,
    0x1.fffffffffffffp-1
  },
  { // Entry 166
    0x1.p-1,
    0x1.0000000000001p51,
    0x1.0p0
  },
  { // Entry 167
    0.0,
    0x1.0000000000001p51,
    0x1.0000000000001p0
  },
  { // Entry 168
    0.0,
    0x1.fffffffffffffp51,
    0x1.fffffffffffffp-1
  },
  { // Entry 169
    -0x1.p-1,
    0x1.fffffffffffffp51,
    0x1.0p0
  },
  { // Entry 170
    -0x1.ffffffffffffc0p-2,
    0x1.fffffffffffffp51,
    0x1.0000000000001p0
  },
  { // Entry 171
    -0x1.ffffffffffffe0p-2,
    0x1.0p52,
    0x1.fffffffffffffp-1
  },
  { // Entry 172
    0.0,
    0x1.0p52,
    0x1.0p0
  },
  { // Entry 173
    0x1.p-52,
    0x1.0p52,
    0x1.0000000000001p0
  },
  { // Entry 174
    -0x1.ffffffffffffc0p-2,
    0x1.0000000000001p52,
    0x1.fffffffffffffp-1
  },
  { // Entry 175
    0.0,
    0x1.0000000000001p52,
    0x1.0p0
  },
  { // Entry 176
    0.0,
    0x1.0000000000001p52,
    0x1.0000000000001p0
  },
  { // Entry 177
    -0.0,
    -0x1.0000000000001p53,
    -0x1.0000000000001p0
  },
  { // Entry 178
    -0.0,
    -0x1.0000000000001p53,
    -0x1.0p0
  },
  { // Entry 179
    -0x1.80p-52,
    -0x1.0000000000001p53,
    -0x1.fffffffffffffp-1
  },
  { // Entry 180
    -0x1.p-51,
    -0x1.0p53,
    -0x1.0000000000001p0
  },
  { // Entry 181
    -0.0,
    -0x1.0p53,
    -0x1.0p0
  },
  { // Entry 182
    -0x1.p-53,
    -0x1.0p53,
    -0x1.fffffffffffffp-1
  },
  { // Entry 183
    -0x1.80p-51,
    -0x1.fffffffffffffp52,
    -0x1.0000000000001p0
  },
  { // Entry 184
    -0.0,
    -0x1.fffffffffffffp52,
    -0x1.0p0
  },
  { // Entry 185
    -0.0,
    -0x1.fffffffffffffp52,
    -0x1.fffffffffffffp-1
  },
  { // Entry 186
    0x1.fffffffffffff0p1023,
    0x1.fffffffffffffp1023,
    HUGE_VAL
  },
  { // Entry 187
    -0x1.fffffffffffff0p1023,
    -0x1.fffffffffffffp1023,
    HUGE_VAL
  },
  { // Entry 188
    0x1.fffffffffffff0p1023,
    0x1.fffffffffffffp1023,
    -HUGE_VAL
  },
  { // Entry 189
    -0x1.fffffffffffff0p1023,
    -0x1.fffffffffffffp1023,
    -HUGE_VAL
  },
  { // Entry 190
    0x1.p-1022,
    0x1.0p-1022,
    HUGE_VAL
  },
  { // Entry 191
    -0x1.p-1022,
    -0x1.0p-1022,
    HUGE_VAL
  },
  { // Entry 192
    0x1.p-1022,
    0x1.0p-1022,
    -HUGE_VAL
  },
  { // Entry 193
    -0x1.p-1022,
    -0x1.0p-1022,
    -HUGE_VAL
  },
  { // Entry 194
    0x1.p-1074,
    0x1.0p-1074,
    HUGE_VAL
  },
  { // Entry 195
    -0x1.p-1074,
    -0x1.0p-1074,
    HUGE_VAL
  },
  { // Entry 196
    0x1.p-1074,
    0x1.0p-1074,
    -HUGE_VAL
  },
  { // Entry 197
    -0x1.p-1074,
    -0x1.0p-1074,
    -HUGE_VAL
  },
  { // Entry 198
    0.0,
    0.0,
    HUGE_VAL
  },
  { // Entry 199
    -0.0,
    -0.0,
    HUGE_VAL
  },
  { // Entry 200
    0.0,
    0.0,
    -HUGE_VAL
  },
  { // Entry 201
    -0.0,
    -0.0,
    -HUGE_VAL
  },
  { // Entry 202
    0.0,
    0x1.fffffffffffffp1023,
    0x1.fffffffffffffp1023
  },
  { // Entry 203
    0.0,
    0x1.fffffffffffffp1023,
    -0x1.fffffffffffffp1023
  },
  { // Entry 204
    -0.0,
    -0x1.fffffffffffffp1023,
    0x1.fffffffffffffp1023
  },
  { // Entry 205
    -0.0,
    -0x1.fffffffffffffp1023,
    -0x1.fffffffffffffp1023
  },
  { // Entry 206
    0.0,
    0x1.fffffffffffffp1023,
    0x1.0p-1022
  },
  { // Entry 207
    0.0,
    0x1.fffffffffffffp1023,
    -0x1.0p-1022
  },
  { // Entry 208
    -0.0,
    -0x1.fffffffffffffp1023,
    0x1.0p-1022
  },
  { // Entry 209
    -0.0,
    -0x1.fffffffffffffp1023,
    -0x1.0p-1022
  },
  { // Entry 210
    0.0,
    0x1.fffffffffffffp1023,
    0x1.0p-1074
  },
  { // Entry 211
    0.0,
    0x1.fffffffffffffp1023,
    -0x1.0p-1074
  },
  { // Entry 212
    -0.0,
    -0x1.fffffffffffffp1023,
    0x1.0p-1074
  },
  { // Entry 213
    -0.0,
    -0x1.fffffffffffffp1023,
    -0x1.0p-1074
  },
  { // Entry 214
    0x1.p-1022,
    0x1.0p-1022,
    0x1.fffffffffffffp1023
  },
  { // Entry 215
    -0x1.p-1022,
    -0x1.0p-1022,
    0x1.fffffffffffffp1023
  },
  { // Entry 216
    0x1.p-1022,
    0x1.0p-1022,
    -0x1.fffffffffffffp1023
  },
  { // Entry 217
    -0x1.p-1022,
    -0x1.0p-1022,
    -0x1.fffffffffffffp1023
  },
  { // Entry 218
    0x1.p-1074,
    0x1.0p-1074,
    0x1.fffffffffffffp1023
  },
  { // Entry 219
    -0x1.p-1074,
    -0x1.0p-1074,
    0x1.fffffffffffffp1023
  },
  { // Entry 220
    0x1.p-1074,
    0x1.0p-1074,
    -0x1.fffffffffffffp1023
  },
  { // Entry 221
    -0x1.p-1074,
    -0x1.0p-1074,
    -0x1.fffffffffffffp1023
  },
  { // Entry 222
    0.0,
    0.0,
    0x1.fffffffffffffp1023
  },
  { // Entry 223
    -0.0,
    -0.0,
    0x1.fffffffffffffp1023
  },
  { // Entry 224
    0.0,
    0.0,
    -0x1.fffffffffffffp1023
  },
  { // Entry 225
    -0.0,
    -0.0,
    -0x1.fffffffffffffp1023
  },
  { // Entry 226
    0.0,
    0x1.0p-1022,
    0x1.0p-1022
  },
  { // Entry 227
    0.0,
    0x1.0p-1022,
    -0x1.0p-1022
  },
  { // Entry 228
    -0.0,
    -0x1.0p-1022,
    0x1.0p-1022
  },
  { // Entry 229
    -0.0,
    -0x1.0p-1022,
    -0x1.0p-1022
  },
  { // Entry 230
    0.0,
    0x1.0p-1022,
    0x1.0p-1074
  },
  { // Entry 231
    0.0,
    0x1.0p-1022,
    -0x1.0p-1074
  },
  { // Entry 232
    -0.0,
    -0x1.0p-1022,
    0x1.0p-1074
  },
  { // Entry 233
    -0.0,
    -0x1.0p-1022,
    -0x1.0p-1074
  },
  { // Entry 234
    0x1.p-1074,
    0x1.0p-1074,
    0x1.0p-1022
  },
  { // Entry 235
    -0x1.p-1074,
    -0x1.0p-1074,
    0x1.0p-1022
  },
  { // Entry 236
    0x1.p-1074,
    0x1.0p-1074,
    -0x1.0p-1022
  },
  { // Entry 237
    -0x1.p-1074,
    -0x1.0p-1074,
    -0x1.0p-1022
  },
  { // Entry 238
    0.0,
    0.0,
    0x1.0p-1022
  },
  { // Entry 239
    -0.0,
    -0.0,
    0x1.0p-1022
  },
  { // Entry 240
    0.0,
    0.0,
    -0x1.0p-1022
  },
  { // Entry 241
    -0.0,
    -0.0,
    -0x1.0p-1022
  },
  { // Entry 242
    0.0,
    0x1.0p-1074,
    0x1.0p-1074
  },
  { // Entry 243
    -0.0,
    -0x1.0p-1074,
    0x1.0p-1074
  },
  { // Entry 244
    0.0,
    0x1.0p-1074,
    -0x1.0p-1074
  },
  { // Entry 245
    -0.0,
    -0x1.0p-1074,
    -0x1.0p-1074
  },
  { // Entry 246
    0.0,
    0.0,
    0x1.0p-1074
  },
  { // Entry 247
    -0.0,
    -0.0,
    0x1.0p-1074
  },
  { // Entry 248
    0.0,
    0.0,
    -0x1.0p-1074
  },
  { // Entry 249
    -0.0,
    -0.0,
    -0x1.0p-1074
  },
  { // Entry 250
    -0x1.p0,
    0x1.8p1,
    0x1.0p1
  },
  { // Entry 251
    0x1.p0,
    -0x1.8p1,
    0x1.0p1
  },
  { // Entry 252
    -0x1.p0,
    0x1.8p1,
    -0x1.0p1
  },
  { // Entry 253
    0x1.p0,
    -0x1.8p1,
    -0x1.0p1
  },
  { // Entry 254
    0x1.p0,
    0x1.4p2,
    0x1.0p1
  },
  { // Entry 255
    -0x1.p0,
    -0x1.4p2,
    0x1.0p1
  },
  { // Entry 256
    0x1.p0,
    0x1.4p2,
    -0x1.0p1
  },
  { // Entry 257
    -0x1.p0,
    -0x1.4p2,
    -0x1.0p1
  }
};
