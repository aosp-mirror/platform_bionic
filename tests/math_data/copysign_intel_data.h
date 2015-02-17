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

static data_1_2_t<double, double, double> g_copysign_intel_data[] = {
  { // Entry 0
    -0x1.p-10,
    -0x1.0p-10,
    -0x1.0p-10
  },
  { // Entry 1
    0x1.p-10,
    -0x1.0p-10,
    0x1.0p-10
  },
  { // Entry 2
    -0x1.p-10,
    0x1.0p-10,
    -0x1.0p-10
  },
  { // Entry 3
    0x1.p-10,
    0x1.0p-10,
    0x1.0p-10
  },
  { // Entry 4
    -0x1.40p3,
    -0x1.4p3,
    -0x1.4p3
  },
  { // Entry 5
    0x1.40p3,
    -0x1.4p3,
    0x1.4p3
  },
  { // Entry 6
    -0x1.40p3,
    0x1.4p3,
    -0x1.4p3
  },
  { // Entry 7
    0x1.40p3,
    0x1.4p3,
    0x1.4p3
  },
  { // Entry 8
    -0x1.p-1073,
    -0x1.0p-1073,
    -0x1.0p-1074
  },
  { // Entry 9
    -0x1.p-1073,
    -0x1.0p-1073,
    -0.0
  },
  { // Entry 10
    0x1.p-1073,
    -0x1.0p-1073,
    0x1.0p-1074
  },
  { // Entry 11
    -0x1.p-1074,
    -0x1.0p-1074,
    -0x1.0p-1074
  },
  { // Entry 12
    -0x1.p-1074,
    -0x1.0p-1074,
    -0.0
  },
  { // Entry 13
    0x1.p-1074,
    -0x1.0p-1074,
    0x1.0p-1074
  },
  { // Entry 14
    -0.0,
    -0.0,
    -0x1.0p-1074
  },
  { // Entry 15
    -0.0,
    -0.0,
    -0.0
  },
  { // Entry 16
    0.0,
    -0.0,
    0x1.0p-1074
  },
  { // Entry 17
    -0x1.p-1074,
    0x1.0p-1074,
    -0x1.0p-1074
  },
  { // Entry 18
    -0x1.p-1074,
    0x1.0p-1074,
    -0.0
  },
  { // Entry 19
    0x1.p-1074,
    0x1.0p-1074,
    0x1.0p-1074
  },
  { // Entry 20
    -0x1.p-1073,
    0x1.0p-1073,
    -0x1.0p-1074
  },
  { // Entry 21
    -0x1.p-1073,
    0x1.0p-1073,
    -0.0
  },
  { // Entry 22
    0x1.p-1073,
    0x1.0p-1073,
    0x1.0p-1074
  },
  { // Entry 23
    -0x1.fffffffffffff0p1023,
    -0x1.fffffffffffffp1023,
    -0x1.fffffffffffffp1023
  },
  { // Entry 24
    0x1.fffffffffffff0p1023,
    0x1.fffffffffffffp1023,
    0x1.fffffffffffffp1023
  },
  { // Entry 25
    0x1.fffffffffffff0p1023,
    -0x1.fffffffffffffp1023,
    0x1.fffffffffffffp1023
  },
  { // Entry 26
    -0x1.fffffffffffff0p1023,
    0x1.fffffffffffffp1023,
    -0x1.fffffffffffffp1023
  },
  { // Entry 27
    -0x1.p-1074,
    -0x1.0p-1074,
    -0x1.fffffffffffffp1023
  },
  { // Entry 28
    -0.0,
    -0.0,
    -0x1.fffffffffffffp1023
  },
  { // Entry 29
    -0x1.p-1074,
    0x1.0p-1074,
    -0x1.fffffffffffffp1023
  },
  { // Entry 30
    0x1.p-1074,
    -0x1.0p-1074,
    0x1.fffffffffffffp1023
  },
  { // Entry 31
    0.0,
    -0.0,
    0x1.fffffffffffffp1023
  },
  { // Entry 32
    0x1.p-1074,
    0x1.0p-1074,
    0x1.fffffffffffffp1023
  },
  { // Entry 33
    -0x1.fffffffffffff0p1023,
    -0x1.fffffffffffffp1023,
    -0x1.0p-1074
  },
  { // Entry 34
    -0x1.fffffffffffff0p1023,
    -0x1.fffffffffffffp1023,
    -0.0
  },
  { // Entry 35
    0x1.fffffffffffff0p1023,
    -0x1.fffffffffffffp1023,
    0x1.0p-1074
  },
  { // Entry 36
    -0x1.fffffffffffff0p1023,
    0x1.fffffffffffffp1023,
    -0x1.0p-1074
  },
  { // Entry 37
    -0x1.fffffffffffff0p1023,
    0x1.fffffffffffffp1023,
    -0.0
  },
  { // Entry 38
    0x1.fffffffffffff0p1023,
    0x1.fffffffffffffp1023,
    0x1.0p-1074
  },
  { // Entry 39
    0x1.00000000000020p-1023,
    -0x1.0000000000002p-1023,
    0x1.ffffffffffffcp-1024
  },
  { // Entry 40
    0x1.00000000000020p-1023,
    -0x1.0000000000002p-1023,
    0x1.0p-1023
  },
  { // Entry 41
    0x1.00000000000020p-1023,
    -0x1.0000000000002p-1023,
    0x1.0000000000002p-1023
  },
  { // Entry 42
    0x1.p-1023,
    -0x1.0p-1023,
    0x1.ffffffffffffcp-1024
  },
  { // Entry 43
    0x1.p-1023,
    -0x1.0p-1023,
    0x1.0p-1023
  },
  { // Entry 44
    0x1.p-1023,
    -0x1.0p-1023,
    0x1.0000000000002p-1023
  },
  { // Entry 45
    0x1.ffffffffffffc0p-1024,
    -0x1.ffffffffffffcp-1024,
    0x1.ffffffffffffcp-1024
  },
  { // Entry 46
    0x1.ffffffffffffc0p-1024,
    -0x1.ffffffffffffcp-1024,
    0x1.0p-1023
  },
  { // Entry 47
    0x1.ffffffffffffc0p-1024,
    -0x1.ffffffffffffcp-1024,
    0x1.0000000000002p-1023
  },
  { // Entry 48
    0x1.00000000000010p0,
    -0x1.0000000000001p0,
    0x1.fffffffffffffp-1
  },
  { // Entry 49
    0x1.00000000000010p0,
    -0x1.0000000000001p0,
    0x1.0p0
  },
  { // Entry 50
    0x1.00000000000010p0,
    -0x1.0000000000001p0,
    0x1.0000000000001p0
  },
  { // Entry 51
    0x1.p0,
    -0x1.0p0,
    0x1.fffffffffffffp-1
  },
  { // Entry 52
    0x1.p0,
    -0x1.0p0,
    0x1.0p0
  },
  { // Entry 53
    0x1.p0,
    -0x1.0p0,
    0x1.0000000000001p0
  },
  { // Entry 54
    0x1.fffffffffffff0p-1,
    -0x1.fffffffffffffp-1,
    0x1.fffffffffffffp-1
  },
  { // Entry 55
    0x1.fffffffffffff0p-1,
    -0x1.fffffffffffffp-1,
    0x1.0p0
  },
  { // Entry 56
    0x1.fffffffffffff0p-1,
    -0x1.fffffffffffffp-1,
    0x1.0000000000001p0
  },
  { // Entry 57
    0x1.00000000000010p1023,
    -0x1.0000000000001p1023,
    0x1.fffffffffffffp1022
  },
  { // Entry 58
    0x1.00000000000010p1023,
    -0x1.0000000000001p1023,
    0x1.0p1023
  },
  { // Entry 59
    0x1.00000000000010p1023,
    -0x1.0000000000001p1023,
    0x1.0000000000001p1023
  },
  { // Entry 60
    0x1.p1023,
    -0x1.0p1023,
    0x1.fffffffffffffp1022
  },
  { // Entry 61
    0x1.p1023,
    -0x1.0p1023,
    0x1.0p1023
  },
  { // Entry 62
    0x1.p1023,
    -0x1.0p1023,
    0x1.0000000000001p1023
  },
  { // Entry 63
    0x1.fffffffffffff0p1022,
    -0x1.fffffffffffffp1022,
    0x1.fffffffffffffp1022
  },
  { // Entry 64
    0x1.fffffffffffff0p1022,
    -0x1.fffffffffffffp1022,
    0x1.0p1023
  },
  { // Entry 65
    0x1.fffffffffffff0p1022,
    -0x1.fffffffffffffp1022,
    0x1.0000000000001p1023
  },
  { // Entry 66
    -0x1.ffffffffffffc0p-1024,
    0x1.ffffffffffffcp-1024,
    -0x1.0000000000002p-1023
  },
  { // Entry 67
    -0x1.ffffffffffffc0p-1024,
    0x1.ffffffffffffcp-1024,
    -0x1.0p-1023
  },
  { // Entry 68
    -0x1.ffffffffffffc0p-1024,
    0x1.ffffffffffffcp-1024,
    -0x1.ffffffffffffcp-1024
  },
  { // Entry 69
    -0x1.p-1023,
    0x1.0p-1023,
    -0x1.0000000000002p-1023
  },
  { // Entry 70
    -0x1.p-1023,
    0x1.0p-1023,
    -0x1.0p-1023
  },
  { // Entry 71
    -0x1.p-1023,
    0x1.0p-1023,
    -0x1.ffffffffffffcp-1024
  },
  { // Entry 72
    -0x1.00000000000020p-1023,
    0x1.0000000000002p-1023,
    -0x1.0000000000002p-1023
  },
  { // Entry 73
    -0x1.00000000000020p-1023,
    0x1.0000000000002p-1023,
    -0x1.0p-1023
  },
  { // Entry 74
    -0x1.00000000000020p-1023,
    0x1.0000000000002p-1023,
    -0x1.ffffffffffffcp-1024
  },
  { // Entry 75
    -0x1.fffffffffffff0p-1,
    0x1.fffffffffffffp-1,
    -0x1.0000000000001p0
  },
  { // Entry 76
    -0x1.fffffffffffff0p-1,
    0x1.fffffffffffffp-1,
    -0x1.0p0
  },
  { // Entry 77
    -0x1.fffffffffffff0p-1,
    0x1.fffffffffffffp-1,
    -0x1.fffffffffffffp-1
  },
  { // Entry 78
    -0x1.p0,
    0x1.0p0,
    -0x1.0000000000001p0
  },
  { // Entry 79
    -0x1.p0,
    0x1.0p0,
    -0x1.0p0
  },
  { // Entry 80
    -0x1.p0,
    0x1.0p0,
    -0x1.fffffffffffffp-1
  },
  { // Entry 81
    -0x1.00000000000010p0,
    0x1.0000000000001p0,
    -0x1.0000000000001p0
  },
  { // Entry 82
    -0x1.00000000000010p0,
    0x1.0000000000001p0,
    -0x1.0p0
  },
  { // Entry 83
    -0x1.00000000000010p0,
    0x1.0000000000001p0,
    -0x1.fffffffffffffp-1
  },
  { // Entry 84
    -0x1.fffffffffffff0p1022,
    0x1.fffffffffffffp1022,
    -0x1.0000000000001p1023
  },
  { // Entry 85
    -0x1.fffffffffffff0p1022,
    0x1.fffffffffffffp1022,
    -0x1.0p1023
  },
  { // Entry 86
    -0x1.fffffffffffff0p1022,
    0x1.fffffffffffffp1022,
    -0x1.fffffffffffffp1022
  },
  { // Entry 87
    -0x1.p1023,
    0x1.0p1023,
    -0x1.0000000000001p1023
  },
  { // Entry 88
    -0x1.p1023,
    0x1.0p1023,
    -0x1.0p1023
  },
  { // Entry 89
    -0x1.p1023,
    0x1.0p1023,
    -0x1.fffffffffffffp1022
  },
  { // Entry 90
    -0x1.00000000000010p1023,
    0x1.0000000000001p1023,
    -0x1.0000000000001p1023
  },
  { // Entry 91
    -0x1.00000000000010p1023,
    0x1.0000000000001p1023,
    -0x1.0p1023
  },
  { // Entry 92
    -0x1.00000000000010p1023,
    0x1.0000000000001p1023,
    -0x1.fffffffffffffp1022
  },
  { // Entry 93
    HUGE_VAL,
    HUGE_VAL,
    HUGE_VAL
  },
  { // Entry 94
    HUGE_VAL,
    HUGE_VAL,
    0x1.fffffffffffffp1023
  },
  { // Entry 95
    HUGE_VAL,
    HUGE_VAL,
    0x1.0p-1022
  },
  { // Entry 96
    HUGE_VAL,
    HUGE_VAL,
    0x1.ffffffffffffep-1023
  },
  { // Entry 97
    HUGE_VAL,
    HUGE_VAL,
    0x1.0p-1074
  },
  { // Entry 98
    HUGE_VAL,
    HUGE_VAL,
    0.0
  },
  { // Entry 99
    -HUGE_VAL,
    HUGE_VAL,
    -0.0
  },
  { // Entry 100
    -HUGE_VAL,
    HUGE_VAL,
    -0x1.0p-1074
  },
  { // Entry 101
    -HUGE_VAL,
    HUGE_VAL,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 102
    -HUGE_VAL,
    HUGE_VAL,
    -0x1.0p-1022
  },
  { // Entry 103
    -HUGE_VAL,
    HUGE_VAL,
    -0x1.0p0
  },
  { // Entry 104
    -HUGE_VAL,
    HUGE_VAL,
    -0x1.fffffffffffffp1023
  },
  { // Entry 105
    -HUGE_VAL,
    HUGE_VAL,
    -HUGE_VAL
  },
  { // Entry 106
    0x1.fffffffffffff0p1023,
    0x1.fffffffffffffp1023,
    HUGE_VAL
  },
  { // Entry 107
    0x1.fffffffffffff0p1023,
    0x1.fffffffffffffp1023,
    0x1.fffffffffffffp1023
  },
  { // Entry 108
    0x1.fffffffffffff0p1023,
    0x1.fffffffffffffp1023,
    0x1.0p-1022
  },
  { // Entry 109
    0x1.fffffffffffff0p1023,
    0x1.fffffffffffffp1023,
    0x1.ffffffffffffep-1023
  },
  { // Entry 110
    0x1.fffffffffffff0p1023,
    0x1.fffffffffffffp1023,
    0x1.0p-1074
  },
  { // Entry 111
    0x1.fffffffffffff0p1023,
    0x1.fffffffffffffp1023,
    0.0
  },
  { // Entry 112
    -0x1.fffffffffffff0p1023,
    0x1.fffffffffffffp1023,
    -0.0
  },
  { // Entry 113
    -0x1.fffffffffffff0p1023,
    0x1.fffffffffffffp1023,
    -0x1.0p-1074
  },
  { // Entry 114
    -0x1.fffffffffffff0p1023,
    0x1.fffffffffffffp1023,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 115
    -0x1.fffffffffffff0p1023,
    0x1.fffffffffffffp1023,
    -0x1.0p-1022
  },
  { // Entry 116
    -0x1.fffffffffffff0p1023,
    0x1.fffffffffffffp1023,
    -0x1.0p0
  },
  { // Entry 117
    -0x1.fffffffffffff0p1023,
    0x1.fffffffffffffp1023,
    -0x1.fffffffffffffp1023
  },
  { // Entry 118
    -0x1.fffffffffffff0p1023,
    0x1.fffffffffffffp1023,
    -HUGE_VAL
  },
  { // Entry 119
    0x1.p-1022,
    0x1.0p-1022,
    HUGE_VAL
  },
  { // Entry 120
    0x1.p-1022,
    0x1.0p-1022,
    0x1.fffffffffffffp1023
  },
  { // Entry 121
    0x1.p-1022,
    0x1.0p-1022,
    0x1.0p-1022
  },
  { // Entry 122
    0x1.p-1022,
    0x1.0p-1022,
    0x1.ffffffffffffep-1023
  },
  { // Entry 123
    0x1.p-1022,
    0x1.0p-1022,
    0x1.0p-1074
  },
  { // Entry 124
    0x1.p-1022,
    0x1.0p-1022,
    0.0
  },
  { // Entry 125
    -0x1.p-1022,
    0x1.0p-1022,
    -0.0
  },
  { // Entry 126
    -0x1.p-1022,
    0x1.0p-1022,
    -0x1.0p-1074
  },
  { // Entry 127
    -0x1.p-1022,
    0x1.0p-1022,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 128
    -0x1.p-1022,
    0x1.0p-1022,
    -0x1.0p-1022
  },
  { // Entry 129
    -0x1.p-1022,
    0x1.0p-1022,
    -0x1.0p0
  },
  { // Entry 130
    -0x1.p-1022,
    0x1.0p-1022,
    -0x1.fffffffffffffp1023
  },
  { // Entry 131
    -0x1.p-1022,
    0x1.0p-1022,
    -HUGE_VAL
  },
  { // Entry 132
    0x1.ffffffffffffe0p-1023,
    0x1.ffffffffffffep-1023,
    HUGE_VAL
  },
  { // Entry 133
    0x1.ffffffffffffe0p-1023,
    0x1.ffffffffffffep-1023,
    0x1.fffffffffffffp1023
  },
  { // Entry 134
    0x1.ffffffffffffe0p-1023,
    0x1.ffffffffffffep-1023,
    0x1.0p-1022
  },
  { // Entry 135
    0x1.ffffffffffffe0p-1023,
    0x1.ffffffffffffep-1023,
    0x1.ffffffffffffep-1023
  },
  { // Entry 136
    0x1.ffffffffffffe0p-1023,
    0x1.ffffffffffffep-1023,
    0x1.0p-1074
  },
  { // Entry 137
    0x1.ffffffffffffe0p-1023,
    0x1.ffffffffffffep-1023,
    0.0
  },
  { // Entry 138
    -0x1.ffffffffffffe0p-1023,
    0x1.ffffffffffffep-1023,
    -0.0
  },
  { // Entry 139
    -0x1.ffffffffffffe0p-1023,
    0x1.ffffffffffffep-1023,
    -0x1.0p-1074
  },
  { // Entry 140
    -0x1.ffffffffffffe0p-1023,
    0x1.ffffffffffffep-1023,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 141
    -0x1.ffffffffffffe0p-1023,
    0x1.ffffffffffffep-1023,
    -0x1.0p-1022
  },
  { // Entry 142
    -0x1.ffffffffffffe0p-1023,
    0x1.ffffffffffffep-1023,
    -0x1.0p0
  },
  { // Entry 143
    -0x1.ffffffffffffe0p-1023,
    0x1.ffffffffffffep-1023,
    -0x1.fffffffffffffp1023
  },
  { // Entry 144
    -0x1.ffffffffffffe0p-1023,
    0x1.ffffffffffffep-1023,
    -HUGE_VAL
  },
  { // Entry 145
    0x1.p-1074,
    0x1.0p-1074,
    HUGE_VAL
  },
  { // Entry 146
    0x1.p-1074,
    0x1.0p-1074,
    0x1.fffffffffffffp1023
  },
  { // Entry 147
    0x1.p-1074,
    0x1.0p-1074,
    0x1.0p-1022
  },
  { // Entry 148
    0x1.p-1074,
    0x1.0p-1074,
    0x1.ffffffffffffep-1023
  },
  { // Entry 149
    0x1.p-1074,
    0x1.0p-1074,
    0x1.0p-1074
  },
  { // Entry 150
    0x1.p-1074,
    0x1.0p-1074,
    0.0
  },
  { // Entry 151
    -0x1.p-1074,
    0x1.0p-1074,
    -0.0
  },
  { // Entry 152
    -0x1.p-1074,
    0x1.0p-1074,
    -0x1.0p-1074
  },
  { // Entry 153
    -0x1.p-1074,
    0x1.0p-1074,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 154
    -0x1.p-1074,
    0x1.0p-1074,
    -0x1.0p-1022
  },
  { // Entry 155
    -0x1.p-1074,
    0x1.0p-1074,
    -0x1.0p0
  },
  { // Entry 156
    -0x1.p-1074,
    0x1.0p-1074,
    -0x1.fffffffffffffp1023
  },
  { // Entry 157
    -0x1.p-1074,
    0x1.0p-1074,
    -HUGE_VAL
  },
  { // Entry 158
    0.0,
    0.0,
    HUGE_VAL
  },
  { // Entry 159
    0.0,
    0.0,
    0x1.fffffffffffffp1023
  },
  { // Entry 160
    0.0,
    0.0,
    0x1.0p-1022
  },
  { // Entry 161
    0.0,
    0.0,
    0x1.ffffffffffffep-1023
  },
  { // Entry 162
    0.0,
    0.0,
    0x1.0p-1074
  },
  { // Entry 163
    0.0,
    0.0,
    0.0
  },
  { // Entry 164
    -0.0,
    0.0,
    -0.0
  },
  { // Entry 165
    -0.0,
    0.0,
    -0x1.0p-1074
  },
  { // Entry 166
    -0.0,
    0.0,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 167
    -0.0,
    0.0,
    -0x1.0p-1022
  },
  { // Entry 168
    -0.0,
    0.0,
    -0x1.0p0
  },
  { // Entry 169
    -0.0,
    0.0,
    -0x1.fffffffffffffp1023
  },
  { // Entry 170
    -0.0,
    0.0,
    -HUGE_VAL
  },
  { // Entry 171
    0.0,
    -0.0,
    HUGE_VAL
  },
  { // Entry 172
    0.0,
    -0.0,
    0x1.fffffffffffffp1023
  },
  { // Entry 173
    0.0,
    -0.0,
    0x1.0p-1022
  },
  { // Entry 174
    0.0,
    -0.0,
    0x1.ffffffffffffep-1023
  },
  { // Entry 175
    0.0,
    -0.0,
    0x1.0p-1074
  },
  { // Entry 176
    0.0,
    -0.0,
    0.0
  },
  { // Entry 177
    -0.0,
    -0.0,
    -0.0
  },
  { // Entry 178
    -0.0,
    -0.0,
    -0x1.0p-1074
  },
  { // Entry 179
    -0.0,
    -0.0,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 180
    -0.0,
    -0.0,
    -0x1.0p-1022
  },
  { // Entry 181
    -0.0,
    -0.0,
    -0x1.0p0
  },
  { // Entry 182
    -0.0,
    -0.0,
    -0x1.fffffffffffffp1023
  },
  { // Entry 183
    -0.0,
    -0.0,
    -HUGE_VAL
  },
  { // Entry 184
    0x1.p-1074,
    -0x1.0p-1074,
    HUGE_VAL
  },
  { // Entry 185
    0x1.p-1074,
    -0x1.0p-1074,
    0x1.fffffffffffffp1023
  },
  { // Entry 186
    0x1.p-1074,
    -0x1.0p-1074,
    0x1.0p-1022
  },
  { // Entry 187
    0x1.p-1074,
    -0x1.0p-1074,
    0x1.ffffffffffffep-1023
  },
  { // Entry 188
    0x1.p-1074,
    -0x1.0p-1074,
    0x1.0p-1074
  },
  { // Entry 189
    0x1.p-1074,
    -0x1.0p-1074,
    0.0
  },
  { // Entry 190
    -0x1.p-1074,
    -0x1.0p-1074,
    -0.0
  },
  { // Entry 191
    -0x1.p-1074,
    -0x1.0p-1074,
    -0x1.0p-1074
  },
  { // Entry 192
    -0x1.p-1074,
    -0x1.0p-1074,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 193
    -0x1.p-1074,
    -0x1.0p-1074,
    -0x1.0p-1022
  },
  { // Entry 194
    -0x1.p-1074,
    -0x1.0p-1074,
    -0x1.0p0
  },
  { // Entry 195
    -0x1.p-1074,
    -0x1.0p-1074,
    -0x1.fffffffffffffp1023
  },
  { // Entry 196
    -0x1.p-1074,
    -0x1.0p-1074,
    -HUGE_VAL
  },
  { // Entry 197
    0x1.ffffffffffffe0p-1023,
    -0x1.ffffffffffffep-1023,
    HUGE_VAL
  },
  { // Entry 198
    0x1.ffffffffffffe0p-1023,
    -0x1.ffffffffffffep-1023,
    0x1.fffffffffffffp1023
  },
  { // Entry 199
    0x1.ffffffffffffe0p-1023,
    -0x1.ffffffffffffep-1023,
    0x1.0p-1022
  },
  { // Entry 200
    0x1.ffffffffffffe0p-1023,
    -0x1.ffffffffffffep-1023,
    0x1.ffffffffffffep-1023
  },
  { // Entry 201
    0x1.ffffffffffffe0p-1023,
    -0x1.ffffffffffffep-1023,
    0x1.0p-1074
  },
  { // Entry 202
    0x1.ffffffffffffe0p-1023,
    -0x1.ffffffffffffep-1023,
    0.0
  },
  { // Entry 203
    -0x1.ffffffffffffe0p-1023,
    -0x1.ffffffffffffep-1023,
    -0.0
  },
  { // Entry 204
    -0x1.ffffffffffffe0p-1023,
    -0x1.ffffffffffffep-1023,
    -0x1.0p-1074
  },
  { // Entry 205
    -0x1.ffffffffffffe0p-1023,
    -0x1.ffffffffffffep-1023,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 206
    -0x1.ffffffffffffe0p-1023,
    -0x1.ffffffffffffep-1023,
    -0x1.0p-1022
  },
  { // Entry 207
    -0x1.ffffffffffffe0p-1023,
    -0x1.ffffffffffffep-1023,
    -0x1.0p0
  },
  { // Entry 208
    -0x1.ffffffffffffe0p-1023,
    -0x1.ffffffffffffep-1023,
    -0x1.fffffffffffffp1023
  },
  { // Entry 209
    -0x1.ffffffffffffe0p-1023,
    -0x1.ffffffffffffep-1023,
    -HUGE_VAL
  },
  { // Entry 210
    0x1.p-1022,
    -0x1.0p-1022,
    HUGE_VAL
  },
  { // Entry 211
    0x1.p-1022,
    -0x1.0p-1022,
    0x1.fffffffffffffp1023
  },
  { // Entry 212
    0x1.p-1022,
    -0x1.0p-1022,
    0x1.0p-1022
  },
  { // Entry 213
    0x1.p-1022,
    -0x1.0p-1022,
    0x1.ffffffffffffep-1023
  },
  { // Entry 214
    0x1.p-1022,
    -0x1.0p-1022,
    0x1.0p-1074
  },
  { // Entry 215
    0x1.p-1022,
    -0x1.0p-1022,
    0.0
  },
  { // Entry 216
    -0x1.p-1022,
    -0x1.0p-1022,
    -0.0
  },
  { // Entry 217
    -0x1.p-1022,
    -0x1.0p-1022,
    -0x1.0p-1074
  },
  { // Entry 218
    -0x1.p-1022,
    -0x1.0p-1022,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 219
    -0x1.p-1022,
    -0x1.0p-1022,
    -0x1.0p-1022
  },
  { // Entry 220
    -0x1.p-1022,
    -0x1.0p-1022,
    -0x1.0p0
  },
  { // Entry 221
    -0x1.p-1022,
    -0x1.0p-1022,
    -0x1.fffffffffffffp1023
  },
  { // Entry 222
    -0x1.p-1022,
    -0x1.0p-1022,
    -HUGE_VAL
  },
  { // Entry 223
    0x1.fffffffffffff0p-1,
    -0x1.fffffffffffffp-1,
    HUGE_VAL
  },
  { // Entry 224
    0x1.fffffffffffff0p-1,
    -0x1.fffffffffffffp-1,
    0x1.fffffffffffffp1023
  },
  { // Entry 225
    0x1.fffffffffffff0p-1,
    -0x1.fffffffffffffp-1,
    0x1.0p-1022
  },
  { // Entry 226
    0x1.fffffffffffff0p-1,
    -0x1.fffffffffffffp-1,
    0x1.ffffffffffffep-1023
  },
  { // Entry 227
    0x1.fffffffffffff0p-1,
    -0x1.fffffffffffffp-1,
    0x1.0p-1074
  },
  { // Entry 228
    0x1.fffffffffffff0p-1,
    -0x1.fffffffffffffp-1,
    0.0
  },
  { // Entry 229
    -0x1.fffffffffffff0p-1,
    -0x1.fffffffffffffp-1,
    -0.0
  },
  { // Entry 230
    -0x1.fffffffffffff0p-1,
    -0x1.fffffffffffffp-1,
    -0x1.0p-1074
  },
  { // Entry 231
    -0x1.fffffffffffff0p-1,
    -0x1.fffffffffffffp-1,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 232
    -0x1.fffffffffffff0p-1,
    -0x1.fffffffffffffp-1,
    -0x1.0p-1022
  },
  { // Entry 233
    -0x1.fffffffffffff0p-1,
    -0x1.fffffffffffffp-1,
    -0x1.0p0
  },
  { // Entry 234
    -0x1.fffffffffffff0p-1,
    -0x1.fffffffffffffp-1,
    -0x1.fffffffffffffp1023
  },
  { // Entry 235
    -0x1.fffffffffffff0p-1,
    -0x1.fffffffffffffp-1,
    -HUGE_VAL
  },
  { // Entry 236
    0x1.p0,
    -0x1.0p0,
    HUGE_VAL
  },
  { // Entry 237
    0x1.p0,
    -0x1.0p0,
    0x1.fffffffffffffp1023
  },
  { // Entry 238
    0x1.p0,
    -0x1.0p0,
    0x1.0p-1022
  },
  { // Entry 239
    0x1.p0,
    -0x1.0p0,
    0x1.ffffffffffffep-1023
  },
  { // Entry 240
    0x1.p0,
    -0x1.0p0,
    0x1.0p-1074
  },
  { // Entry 241
    0x1.p0,
    -0x1.0p0,
    0.0
  },
  { // Entry 242
    -0x1.p0,
    -0x1.0p0,
    -0.0
  },
  { // Entry 243
    -0x1.p0,
    -0x1.0p0,
    -0x1.0p-1074
  },
  { // Entry 244
    -0x1.p0,
    -0x1.0p0,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 245
    -0x1.p0,
    -0x1.0p0,
    -0x1.0p-1022
  },
  { // Entry 246
    -0x1.p0,
    -0x1.0p0,
    -0x1.0p0
  },
  { // Entry 247
    -0x1.p0,
    -0x1.0p0,
    -0x1.fffffffffffffp1023
  },
  { // Entry 248
    -0x1.p0,
    -0x1.0p0,
    -HUGE_VAL
  },
  { // Entry 249
    0x1.00000000000010p0,
    -0x1.0000000000001p0,
    HUGE_VAL
  },
  { // Entry 250
    0x1.00000000000010p0,
    -0x1.0000000000001p0,
    0x1.fffffffffffffp1023
  },
  { // Entry 251
    0x1.00000000000010p0,
    -0x1.0000000000001p0,
    0x1.0p-1022
  },
  { // Entry 252
    0x1.00000000000010p0,
    -0x1.0000000000001p0,
    0x1.ffffffffffffep-1023
  },
  { // Entry 253
    0x1.00000000000010p0,
    -0x1.0000000000001p0,
    0x1.0p-1074
  },
  { // Entry 254
    0x1.00000000000010p0,
    -0x1.0000000000001p0,
    0.0
  },
  { // Entry 255
    -0x1.00000000000010p0,
    -0x1.0000000000001p0,
    -0.0
  },
  { // Entry 256
    -0x1.00000000000010p0,
    -0x1.0000000000001p0,
    -0x1.0p-1074
  },
  { // Entry 257
    -0x1.00000000000010p0,
    -0x1.0000000000001p0,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 258
    -0x1.00000000000010p0,
    -0x1.0000000000001p0,
    -0x1.0p-1022
  },
  { // Entry 259
    -0x1.00000000000010p0,
    -0x1.0000000000001p0,
    -0x1.0p0
  },
  { // Entry 260
    -0x1.00000000000010p0,
    -0x1.0000000000001p0,
    -0x1.fffffffffffffp1023
  },
  { // Entry 261
    -0x1.00000000000010p0,
    -0x1.0000000000001p0,
    -HUGE_VAL
  },
  { // Entry 262
    0x1.fffffffffffff0p1023,
    -0x1.fffffffffffffp1023,
    HUGE_VAL
  },
  { // Entry 263
    0x1.fffffffffffff0p1023,
    -0x1.fffffffffffffp1023,
    0x1.fffffffffffffp1023
  },
  { // Entry 264
    0x1.fffffffffffff0p1023,
    -0x1.fffffffffffffp1023,
    0x1.0p-1022
  },
  { // Entry 265
    0x1.fffffffffffff0p1023,
    -0x1.fffffffffffffp1023,
    0x1.ffffffffffffep-1023
  },
  { // Entry 266
    0x1.fffffffffffff0p1023,
    -0x1.fffffffffffffp1023,
    0x1.0p-1074
  },
  { // Entry 267
    0x1.fffffffffffff0p1023,
    -0x1.fffffffffffffp1023,
    0.0
  },
  { // Entry 268
    -0x1.fffffffffffff0p1023,
    -0x1.fffffffffffffp1023,
    -0.0
  },
  { // Entry 269
    -0x1.fffffffffffff0p1023,
    -0x1.fffffffffffffp1023,
    -0x1.0p-1074
  },
  { // Entry 270
    -0x1.fffffffffffff0p1023,
    -0x1.fffffffffffffp1023,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 271
    -0x1.fffffffffffff0p1023,
    -0x1.fffffffffffffp1023,
    -0x1.0p-1022
  },
  { // Entry 272
    -0x1.fffffffffffff0p1023,
    -0x1.fffffffffffffp1023,
    -0x1.0p0
  },
  { // Entry 273
    -0x1.fffffffffffff0p1023,
    -0x1.fffffffffffffp1023,
    -0x1.fffffffffffffp1023
  },
  { // Entry 274
    -0x1.fffffffffffff0p1023,
    -0x1.fffffffffffffp1023,
    -HUGE_VAL
  },
  { // Entry 275
    HUGE_VAL,
    -HUGE_VAL,
    HUGE_VAL
  },
  { // Entry 276
    HUGE_VAL,
    -HUGE_VAL,
    0x1.fffffffffffffp1023
  },
  { // Entry 277
    HUGE_VAL,
    -HUGE_VAL,
    0x1.0p-1022
  },
  { // Entry 278
    HUGE_VAL,
    -HUGE_VAL,
    0x1.ffffffffffffep-1023
  },
  { // Entry 279
    HUGE_VAL,
    -HUGE_VAL,
    0x1.0p-1074
  },
  { // Entry 280
    HUGE_VAL,
    -HUGE_VAL,
    0.0
  },
  { // Entry 281
    -HUGE_VAL,
    -HUGE_VAL,
    -0.0
  },
  { // Entry 282
    -HUGE_VAL,
    -HUGE_VAL,
    -0x1.0p-1074
  },
  { // Entry 283
    -HUGE_VAL,
    -HUGE_VAL,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 284
    -HUGE_VAL,
    -HUGE_VAL,
    -0x1.0p-1022
  },
  { // Entry 285
    -HUGE_VAL,
    -HUGE_VAL,
    -0x1.0p0
  },
  { // Entry 286
    -HUGE_VAL,
    -HUGE_VAL,
    -0x1.fffffffffffffp1023
  },
  { // Entry 287
    -HUGE_VAL,
    -HUGE_VAL,
    -HUGE_VAL
  }
};
