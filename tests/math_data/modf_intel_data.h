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

static data_2_1_t<double, double, double> g_modf_intel_data[] = {
  { // Entry 0
    -0x1.p-1074,
    -0.0,
    -0x1.0p-1074
  },
  { // Entry 1
    -0.0,
    -0.0,
    -0.0
  },
  { // Entry 2
    0x1.p-1074,
    0.0,
    0x1.0p-1074
  },
  { // Entry 3
    0x1.fffffffffffff0p-2,
    0.0,
    0x1.fffffffffffffp-2
  },
  { // Entry 4
    0x1.p-1,
    0.0,
    0x1.0p-1
  },
  { // Entry 5
    0x1.00000000000010p-1,
    0.0,
    0x1.0000000000001p-1
  },
  { // Entry 6
    0x1.fffffffffffff0p-1,
    0.0,
    0x1.fffffffffffffp-1
  },
  { // Entry 7
    0.0,
    0x1.p0,
    0x1.0p0
  },
  { // Entry 8
    0x1.p-52,
    0x1.p0,
    0x1.0000000000001p0
  },
  { // Entry 9
    0x1.ffffffffffffc0p-2,
    0x1.p0,
    0x1.7ffffffffffffp0
  },
  { // Entry 10
    0x1.p-1,
    0x1.p0,
    0x1.8p0
  },
  { // Entry 11
    0x1.00000000000020p-1,
    0x1.p0,
    0x1.8000000000001p0
  },
  { // Entry 12
    0x1.ffffffffffffe0p-1,
    0x1.p0,
    0x1.fffffffffffffp0
  },
  { // Entry 13
    0.0,
    0x1.p1,
    0x1.0p1
  },
  { // Entry 14
    0x1.p-51,
    0x1.p1,
    0x1.0000000000001p1
  },
  { // Entry 15
    0x1.ffffffffffff80p-2,
    0x1.p1,
    0x1.3ffffffffffffp1
  },
  { // Entry 16
    0x1.p-1,
    0x1.p1,
    0x1.4p1
  },
  { // Entry 17
    0x1.00000000000040p-1,
    0x1.p1,
    0x1.4000000000001p1
  },
  { // Entry 18
    0x1.fffffffffff8p-1,
    0x1.8cp6,
    0x1.8ffffffffffffp6
  },
  { // Entry 19
    0.0,
    0x1.90p6,
    0x1.9p6
  },
  { // Entry 20
    0x1.p-46,
    0x1.90p6,
    0x1.9000000000001p6
  },
  { // Entry 21
    0x1.fffffffffff0p-2,
    0x1.90p6,
    0x1.91fffffffffffp6
  },
  { // Entry 22
    0x1.p-1,
    0x1.90p6,
    0x1.920p6
  },
  { // Entry 23
    0x1.000000000008p-1,
    0x1.90p6,
    0x1.9200000000001p6
  },
  { // Entry 24
    0x1.ffffffffffc0p-1,
    0x1.f380p9,
    0x1.f3fffffffffffp9
  },
  { // Entry 25
    0.0,
    0x1.f4p9,
    0x1.f40p9
  },
  { // Entry 26
    0x1.p-43,
    0x1.f4p9,
    0x1.f400000000001p9
  },
  { // Entry 27
    0x1.ffffffffff80p-2,
    0x1.f4p9,
    0x1.f43ffffffffffp9
  },
  { // Entry 28
    0x1.p-1,
    0x1.f4p9,
    0x1.f44p9
  },
  { // Entry 29
    0x1.000000000040p-1,
    0x1.f4p9,
    0x1.f440000000001p9
  },
  { // Entry 30
    0x1.c0p-1,
    0x1.ffffffffffff80p49,
    0x1.fffffffffffffp49
  },
  { // Entry 31
    0.0,
    0x1.p50,
    0x1.0p50
  },
  { // Entry 32
    0x1.p-2,
    0x1.p50,
    0x1.0000000000001p50
  },
  { // Entry 33
    0x1.80p-1,
    0x1.ffffffffffffc0p50,
    0x1.fffffffffffffp50
  },
  { // Entry 34
    0.0,
    0x1.p51,
    0x1.0p51
  },
  { // Entry 35
    0x1.p-1,
    0x1.p51,
    0x1.0000000000001p51
  },
  { // Entry 36
    0x1.p-1,
    0x1.ffffffffffffe0p51,
    0x1.fffffffffffffp51
  },
  { // Entry 37
    0.0,
    0x1.p52,
    0x1.0p52
  },
  { // Entry 38
    0.0,
    0x1.00000000000010p52,
    0x1.0000000000001p52
  },
  { // Entry 39
    0.0,
    0x1.fffffffffffff0p52,
    0x1.fffffffffffffp52
  },
  { // Entry 40
    0.0,
    0x1.p53,
    0x1.0p53
  },
  { // Entry 41
    0.0,
    0x1.00000000000010p53,
    0x1.0000000000001p53
  },
  { // Entry 42
    0.0,
    0x1.fffffffffffff0p53,
    0x1.fffffffffffffp53
  },
  { // Entry 43
    0.0,
    0x1.p54,
    0x1.0p54
  },
  { // Entry 44
    0.0,
    0x1.00000000000010p54,
    0x1.0000000000001p54
  },
  { // Entry 45
    0.0,
    0x1.fffffffffffff0p1023,
    0x1.fffffffffffffp1023
  },
  { // Entry 46
    -0x1.00000000000010p-1,
    -0.0,
    -0x1.0000000000001p-1
  },
  { // Entry 47
    -0x1.p-1,
    -0.0,
    -0x1.0p-1
  },
  { // Entry 48
    -0x1.fffffffffffff0p-2,
    -0.0,
    -0x1.fffffffffffffp-2
  },
  { // Entry 49
    -0x1.p-52,
    -0x1.p0,
    -0x1.0000000000001p0
  },
  { // Entry 50
    -0.0,
    -0x1.p0,
    -0x1.0p0
  },
  { // Entry 51
    -0x1.fffffffffffff0p-1,
    -0.0,
    -0x1.fffffffffffffp-1
  },
  { // Entry 52
    -0x1.00000000000020p-1,
    -0x1.p0,
    -0x1.8000000000001p0
  },
  { // Entry 53
    -0x1.p-1,
    -0x1.p0,
    -0x1.8p0
  },
  { // Entry 54
    -0x1.ffffffffffffc0p-2,
    -0x1.p0,
    -0x1.7ffffffffffffp0
  },
  { // Entry 55
    -0x1.p-51,
    -0x1.p1,
    -0x1.0000000000001p1
  },
  { // Entry 56
    -0.0,
    -0x1.p1,
    -0x1.0p1
  },
  { // Entry 57
    -0x1.ffffffffffffe0p-1,
    -0x1.p0,
    -0x1.fffffffffffffp0
  },
  { // Entry 58
    -0x1.00000000000040p-1,
    -0x1.p1,
    -0x1.4000000000001p1
  },
  { // Entry 59
    -0x1.p-1,
    -0x1.p1,
    -0x1.4p1
  },
  { // Entry 60
    -0x1.ffffffffffff80p-2,
    -0x1.p1,
    -0x1.3ffffffffffffp1
  },
  { // Entry 61
    -0x1.p-46,
    -0x1.90p6,
    -0x1.9000000000001p6
  },
  { // Entry 62
    -0.0,
    -0x1.90p6,
    -0x1.9p6
  },
  { // Entry 63
    -0x1.fffffffffff8p-1,
    -0x1.8cp6,
    -0x1.8ffffffffffffp6
  },
  { // Entry 64
    -0x1.000000000008p-1,
    -0x1.90p6,
    -0x1.9200000000001p6
  },
  { // Entry 65
    -0x1.p-1,
    -0x1.90p6,
    -0x1.920p6
  },
  { // Entry 66
    -0x1.fffffffffff0p-2,
    -0x1.90p6,
    -0x1.91fffffffffffp6
  },
  { // Entry 67
    -0x1.p-43,
    -0x1.f4p9,
    -0x1.f400000000001p9
  },
  { // Entry 68
    -0.0,
    -0x1.f4p9,
    -0x1.f40p9
  },
  { // Entry 69
    -0x1.ffffffffffc0p-1,
    -0x1.f380p9,
    -0x1.f3fffffffffffp9
  },
  { // Entry 70
    -0x1.000000000040p-1,
    -0x1.f4p9,
    -0x1.f440000000001p9
  },
  { // Entry 71
    -0x1.p-1,
    -0x1.f4p9,
    -0x1.f44p9
  },
  { // Entry 72
    -0x1.ffffffffff80p-2,
    -0x1.f4p9,
    -0x1.f43ffffffffffp9
  },
  { // Entry 73
    -0x1.p-2,
    -0x1.p50,
    -0x1.0000000000001p50
  },
  { // Entry 74
    -0.0,
    -0x1.p50,
    -0x1.0p50
  },
  { // Entry 75
    -0x1.c0p-1,
    -0x1.ffffffffffff80p49,
    -0x1.fffffffffffffp49
  },
  { // Entry 76
    -0x1.p-1,
    -0x1.p51,
    -0x1.0000000000001p51
  },
  { // Entry 77
    -0.0,
    -0x1.p51,
    -0x1.0p51
  },
  { // Entry 78
    -0x1.80p-1,
    -0x1.ffffffffffffc0p50,
    -0x1.fffffffffffffp50
  },
  { // Entry 79
    -0.0,
    -0x1.00000000000010p52,
    -0x1.0000000000001p52
  },
  { // Entry 80
    -0.0,
    -0x1.p52,
    -0x1.0p52
  },
  { // Entry 81
    -0x1.p-1,
    -0x1.ffffffffffffe0p51,
    -0x1.fffffffffffffp51
  },
  { // Entry 82
    -0.0,
    -0x1.00000000000010p53,
    -0x1.0000000000001p53
  },
  { // Entry 83
    -0.0,
    -0x1.p53,
    -0x1.0p53
  },
  { // Entry 84
    -0.0,
    -0x1.fffffffffffff0p52,
    -0x1.fffffffffffffp52
  },
  { // Entry 85
    -0.0,
    -0x1.00000000000010p54,
    -0x1.0000000000001p54
  },
  { // Entry 86
    -0.0,
    -0x1.p54,
    -0x1.0p54
  },
  { // Entry 87
    -0.0,
    -0x1.fffffffffffff0p53,
    -0x1.fffffffffffffp53
  },
  { // Entry 88
    -0.0,
    -0x1.fffffffffffff0p1023,
    -0x1.fffffffffffffp1023
  },
  { // Entry 89
    0x1.fffffcp-1,
    0x1.fffffff8p29,
    0x1.fffffffffffffp29
  },
  { // Entry 90
    0.0,
    0x1.p30,
    0x1.0p30
  },
  { // Entry 91
    0x1.p-22,
    0x1.p30,
    0x1.0000000000001p30
  },
  { // Entry 92
    0x1.ffffe8p-1,
    0x1.fffffff4p30,
    0x1.fffffff7ffffdp30
  },
  { // Entry 93
    0x1.fffff0p-1,
    0x1.fffffff4p30,
    0x1.fffffff7ffffep30
  },
  { // Entry 94
    0x1.fffff8p-1,
    0x1.fffffff4p30,
    0x1.fffffff7fffffp30
  },
  { // Entry 95
    0.0,
    0x1.fffffff8p30,
    0x1.fffffff80p30
  },
  { // Entry 96
    0x1.p-22,
    0x1.fffffff8p30,
    0x1.fffffff800001p30
  },
  { // Entry 97
    0x1.p-21,
    0x1.fffffff8p30,
    0x1.fffffff800002p30
  },
  { // Entry 98
    0x1.80p-21,
    0x1.fffffff8p30,
    0x1.fffffff800003p30
  },
  { // Entry 99
    0x1.ffffd0p-2,
    0x1.fffffff8p30,
    0x1.fffffff9ffffdp30
  },
  { // Entry 100
    0x1.ffffe0p-2,
    0x1.fffffff8p30,
    0x1.fffffff9ffffep30
  },
  { // Entry 101
    0x1.fffff0p-2,
    0x1.fffffff8p30,
    0x1.fffffff9fffffp30
  },
  { // Entry 102
    0x1.p-1,
    0x1.fffffff8p30,
    0x1.fffffffa0p30
  },
  { // Entry 103
    0x1.000008p-1,
    0x1.fffffff8p30,
    0x1.fffffffa00001p30
  },
  { // Entry 104
    0x1.000010p-1,
    0x1.fffffff8p30,
    0x1.fffffffa00002p30
  },
  { // Entry 105
    0x1.000018p-1,
    0x1.fffffff8p30,
    0x1.fffffffa00003p30
  },
  { // Entry 106
    0x1.ffffe8p-1,
    0x1.fffffff8p30,
    0x1.fffffffbffffdp30
  },
  { // Entry 107
    0x1.fffff0p-1,
    0x1.fffffff8p30,
    0x1.fffffffbffffep30
  },
  { // Entry 108
    0x1.fffff8p-1,
    0x1.fffffff8p30,
    0x1.fffffffbfffffp30
  },
  { // Entry 109
    0.0,
    0x1.fffffffcp30,
    0x1.fffffffc0p30
  },
  { // Entry 110
    0x1.p-22,
    0x1.fffffffcp30,
    0x1.fffffffc00001p30
  },
  { // Entry 111
    0x1.p-21,
    0x1.fffffffcp30,
    0x1.fffffffc00002p30
  },
  { // Entry 112
    0x1.80p-21,
    0x1.fffffffcp30,
    0x1.fffffffc00003p30
  },
  { // Entry 113
    0x1.ffffd0p-2,
    0x1.fffffffcp30,
    0x1.fffffffdffffdp30
  },
  { // Entry 114
    0x1.ffffe0p-2,
    0x1.fffffffcp30,
    0x1.fffffffdffffep30
  },
  { // Entry 115
    0x1.fffff0p-2,
    0x1.fffffffcp30,
    0x1.fffffffdfffffp30
  },
  { // Entry 116
    0x1.p-1,
    0x1.fffffffcp30,
    0x1.fffffffe0p30
  },
  { // Entry 117
    0x1.000008p-1,
    0x1.fffffffcp30,
    0x1.fffffffe00001p30
  },
  { // Entry 118
    0x1.000010p-1,
    0x1.fffffffcp30,
    0x1.fffffffe00002p30
  },
  { // Entry 119
    0x1.000018p-1,
    0x1.fffffffcp30,
    0x1.fffffffe00003p30
  },
  { // Entry 120
    0x1.ffffe8p-1,
    0x1.fffffffcp30,
    0x1.ffffffffffffdp30
  },
  { // Entry 121
    0x1.fffff0p-1,
    0x1.fffffffcp30,
    0x1.ffffffffffffep30
  },
  { // Entry 122
    0x1.fffff8p-1,
    0x1.fffffffcp30,
    0x1.fffffffffffffp30
  },
  { // Entry 123
    0.0,
    0x1.p31,
    0x1.0p31
  },
  { // Entry 124
    0x1.p-21,
    0x1.p31,
    0x1.0000000000001p31
  },
  { // Entry 125
    0x1.p-20,
    0x1.p31,
    0x1.0000000000002p31
  },
  { // Entry 126
    0x1.80p-20,
    0x1.p31,
    0x1.0000000000003p31
  },
  { // Entry 127
    0x1.ffffa0p-2,
    0x1.p31,
    0x1.00000000ffffdp31
  },
  { // Entry 128
    0x1.ffffc0p-2,
    0x1.p31,
    0x1.00000000ffffep31
  },
  { // Entry 129
    0x1.ffffe0p-2,
    0x1.p31,
    0x1.00000000fffffp31
  },
  { // Entry 130
    0x1.p-1,
    0x1.p31,
    0x1.000000010p31
  },
  { // Entry 131
    0x1.000010p-1,
    0x1.p31,
    0x1.0000000100001p31
  },
  { // Entry 132
    0x1.000020p-1,
    0x1.p31,
    0x1.0000000100002p31
  },
  { // Entry 133
    0x1.000030p-1,
    0x1.p31,
    0x1.0000000100003p31
  },
  { // Entry 134
    0.0,
    0x1.ffffffe0p30,
    0x1.ffffffep30
  },
  { // Entry 135
    0.0,
    0x1.ffffffe4p30,
    0x1.ffffffe40p30
  },
  { // Entry 136
    0.0,
    0x1.ffffffe8p30,
    0x1.ffffffe80p30
  },
  { // Entry 137
    0.0,
    0x1.ffffffecp30,
    0x1.ffffffec0p30
  },
  { // Entry 138
    0.0,
    0x1.fffffff0p30,
    0x1.fffffffp30
  },
  { // Entry 139
    0.0,
    0x1.fffffff4p30,
    0x1.fffffff40p30
  },
  { // Entry 140
    0.0,
    0x1.fffffff8p30,
    0x1.fffffff80p30
  },
  { // Entry 141
    0.0,
    0x1.fffffffcp30,
    0x1.fffffffc0p30
  },
  { // Entry 142
    0.0,
    0x1.p31,
    0x1.0p31
  },
  { // Entry 143
    0.0,
    0x1.00000002p31,
    0x1.000000020p31
  },
  { // Entry 144
    -0x1.p-22,
    -0x1.p30,
    -0x1.0000000000001p30
  },
  { // Entry 145
    -0.0,
    -0x1.p30,
    -0x1.0p30
  },
  { // Entry 146
    -0x1.fffffcp-1,
    -0x1.fffffff8p29,
    -0x1.fffffffffffffp29
  },
  { // Entry 147
    -0x1.80p-21,
    -0x1.fffffff8p30,
    -0x1.fffffff800003p30
  },
  { // Entry 148
    -0x1.p-21,
    -0x1.fffffff8p30,
    -0x1.fffffff800002p30
  },
  { // Entry 149
    -0x1.p-22,
    -0x1.fffffff8p30,
    -0x1.fffffff800001p30
  },
  { // Entry 150
    -0.0,
    -0x1.fffffff8p30,
    -0x1.fffffff80p30
  },
  { // Entry 151
    -0x1.fffff8p-1,
    -0x1.fffffff4p30,
    -0x1.fffffff7fffffp30
  },
  { // Entry 152
    -0x1.fffff0p-1,
    -0x1.fffffff4p30,
    -0x1.fffffff7ffffep30
  },
  { // Entry 153
    -0x1.ffffe8p-1,
    -0x1.fffffff4p30,
    -0x1.fffffff7ffffdp30
  },
  { // Entry 154
    -0x1.000018p-1,
    -0x1.fffffff8p30,
    -0x1.fffffffa00003p30
  },
  { // Entry 155
    -0x1.000010p-1,
    -0x1.fffffff8p30,
    -0x1.fffffffa00002p30
  },
  { // Entry 156
    -0x1.000008p-1,
    -0x1.fffffff8p30,
    -0x1.fffffffa00001p30
  },
  { // Entry 157
    -0x1.p-1,
    -0x1.fffffff8p30,
    -0x1.fffffffa0p30
  },
  { // Entry 158
    -0x1.fffff0p-2,
    -0x1.fffffff8p30,
    -0x1.fffffff9fffffp30
  },
  { // Entry 159
    -0x1.ffffe0p-2,
    -0x1.fffffff8p30,
    -0x1.fffffff9ffffep30
  },
  { // Entry 160
    -0x1.ffffd0p-2,
    -0x1.fffffff8p30,
    -0x1.fffffff9ffffdp30
  },
  { // Entry 161
    -0x1.80p-21,
    -0x1.fffffffcp30,
    -0x1.fffffffc00003p30
  },
  { // Entry 162
    -0x1.p-21,
    -0x1.fffffffcp30,
    -0x1.fffffffc00002p30
  },
  { // Entry 163
    -0x1.p-22,
    -0x1.fffffffcp30,
    -0x1.fffffffc00001p30
  },
  { // Entry 164
    -0.0,
    -0x1.fffffffcp30,
    -0x1.fffffffc0p30
  },
  { // Entry 165
    -0x1.fffff8p-1,
    -0x1.fffffff8p30,
    -0x1.fffffffbfffffp30
  },
  { // Entry 166
    -0x1.fffff0p-1,
    -0x1.fffffff8p30,
    -0x1.fffffffbffffep30
  },
  { // Entry 167
    -0x1.ffffe8p-1,
    -0x1.fffffff8p30,
    -0x1.fffffffbffffdp30
  },
  { // Entry 168
    -0x1.000018p-1,
    -0x1.fffffffcp30,
    -0x1.fffffffe00003p30
  },
  { // Entry 169
    -0x1.000010p-1,
    -0x1.fffffffcp30,
    -0x1.fffffffe00002p30
  },
  { // Entry 170
    -0x1.000008p-1,
    -0x1.fffffffcp30,
    -0x1.fffffffe00001p30
  },
  { // Entry 171
    -0x1.p-1,
    -0x1.fffffffcp30,
    -0x1.fffffffe0p30
  },
  { // Entry 172
    -0x1.fffff0p-2,
    -0x1.fffffffcp30,
    -0x1.fffffffdfffffp30
  },
  { // Entry 173
    -0x1.ffffe0p-2,
    -0x1.fffffffcp30,
    -0x1.fffffffdffffep30
  },
  { // Entry 174
    -0x1.ffffd0p-2,
    -0x1.fffffffcp30,
    -0x1.fffffffdffffdp30
  },
  { // Entry 175
    -0x1.80p-20,
    -0x1.p31,
    -0x1.0000000000003p31
  },
  { // Entry 176
    -0x1.p-20,
    -0x1.p31,
    -0x1.0000000000002p31
  },
  { // Entry 177
    -0x1.p-21,
    -0x1.p31,
    -0x1.0000000000001p31
  },
  { // Entry 178
    -0.0,
    -0x1.p31,
    -0x1.0p31
  },
  { // Entry 179
    -0x1.fffff8p-1,
    -0x1.fffffffcp30,
    -0x1.fffffffffffffp30
  },
  { // Entry 180
    -0x1.fffff0p-1,
    -0x1.fffffffcp30,
    -0x1.ffffffffffffep30
  },
  { // Entry 181
    -0x1.ffffe8p-1,
    -0x1.fffffffcp30,
    -0x1.ffffffffffffdp30
  },
  { // Entry 182
    -0x1.000030p-1,
    -0x1.p31,
    -0x1.0000000100003p31
  },
  { // Entry 183
    -0x1.000020p-1,
    -0x1.p31,
    -0x1.0000000100002p31
  },
  { // Entry 184
    -0x1.000010p-1,
    -0x1.p31,
    -0x1.0000000100001p31
  },
  { // Entry 185
    -0x1.p-1,
    -0x1.p31,
    -0x1.000000010p31
  },
  { // Entry 186
    -0x1.ffffe0p-2,
    -0x1.p31,
    -0x1.00000000fffffp31
  },
  { // Entry 187
    -0x1.ffffc0p-2,
    -0x1.p31,
    -0x1.00000000ffffep31
  },
  { // Entry 188
    -0x1.ffffa0p-2,
    -0x1.p31,
    -0x1.00000000ffffdp31
  },
  { // Entry 189
    -0.0,
    -0x1.ffffffe0p30,
    -0x1.ffffffep30
  },
  { // Entry 190
    -0.0,
    -0x1.ffffffe0p30,
    -0x1.ffffffep30
  },
  { // Entry 191
    -0.0,
    -0x1.ffffffe0p30,
    -0x1.ffffffep30
  },
  { // Entry 192
    -0.0,
    -0x1.ffffffe0p30,
    -0x1.ffffffep30
  },
  { // Entry 193
    -0.0,
    -0x1.ffffffe0p30,
    -0x1.ffffffep30
  },
  { // Entry 194
    -0.0,
    -0x1.ffffffe0p30,
    -0x1.ffffffep30
  },
  { // Entry 195
    -0.0,
    -0x1.ffffffe0p30,
    -0x1.ffffffep30
  },
  { // Entry 196
    -0.0,
    -0x1.ffffffe0p30,
    -0x1.ffffffep30
  },
  { // Entry 197
    -0.0,
    -0x1.ffffffe0p30,
    -0x1.ffffffep30
  },
  { // Entry 198
    -0.0,
    -0x1.ffffffe0p30,
    -0x1.ffffffep30
  },
  { // Entry 199
    0.0,
    0x1.ffffffffffffd0p61,
    0x1.ffffffffffffdp61
  },
  { // Entry 200
    0.0,
    0x1.ffffffffffffe0p61,
    0x1.ffffffffffffep61
  },
  { // Entry 201
    0.0,
    0x1.fffffffffffff0p61,
    0x1.fffffffffffffp61
  },
  { // Entry 202
    0.0,
    0x1.p62,
    0x1.0p62
  },
  { // Entry 203
    0.0,
    0x1.00000000000010p62,
    0x1.0000000000001p62
  },
  { // Entry 204
    0.0,
    0x1.00000000000020p62,
    0x1.0000000000002p62
  },
  { // Entry 205
    0.0,
    0x1.00000000000030p62,
    0x1.0000000000003p62
  },
  { // Entry 206
    0.0,
    0x1.ffffffffffffd0p62,
    0x1.ffffffffffffdp62
  },
  { // Entry 207
    0.0,
    0x1.ffffffffffffe0p62,
    0x1.ffffffffffffep62
  },
  { // Entry 208
    0.0,
    0x1.fffffffffffff0p62,
    0x1.fffffffffffffp62
  },
  { // Entry 209
    0.0,
    0x1.p63,
    0x1.0p63
  },
  { // Entry 210
    0.0,
    0x1.00000000000010p63,
    0x1.0000000000001p63
  },
  { // Entry 211
    0.0,
    0x1.00000000000020p63,
    0x1.0000000000002p63
  },
  { // Entry 212
    0.0,
    0x1.00000000000030p63,
    0x1.0000000000003p63
  },
  { // Entry 213
    0.0,
    0x1.ffffffffffffd0p63,
    0x1.ffffffffffffdp63
  },
  { // Entry 214
    0.0,
    0x1.ffffffffffffe0p63,
    0x1.ffffffffffffep63
  },
  { // Entry 215
    0.0,
    0x1.fffffffffffff0p63,
    0x1.fffffffffffffp63
  },
  { // Entry 216
    0.0,
    0x1.p64,
    0x1.0p64
  },
  { // Entry 217
    0.0,
    0x1.00000000000010p64,
    0x1.0000000000001p64
  },
  { // Entry 218
    0.0,
    0x1.00000000000020p64,
    0x1.0000000000002p64
  },
  { // Entry 219
    0.0,
    0x1.00000000000030p64,
    0x1.0000000000003p64
  },
  { // Entry 220
    -0.0,
    -0x1.00000000000030p62,
    -0x1.0000000000003p62
  },
  { // Entry 221
    -0.0,
    -0x1.00000000000020p62,
    -0x1.0000000000002p62
  },
  { // Entry 222
    -0.0,
    -0x1.00000000000010p62,
    -0x1.0000000000001p62
  },
  { // Entry 223
    -0.0,
    -0x1.p62,
    -0x1.0p62
  },
  { // Entry 224
    -0.0,
    -0x1.fffffffffffff0p61,
    -0x1.fffffffffffffp61
  },
  { // Entry 225
    -0.0,
    -0x1.ffffffffffffe0p61,
    -0x1.ffffffffffffep61
  },
  { // Entry 226
    -0.0,
    -0x1.ffffffffffffd0p61,
    -0x1.ffffffffffffdp61
  },
  { // Entry 227
    -0.0,
    -0x1.00000000000030p63,
    -0x1.0000000000003p63
  },
  { // Entry 228
    -0.0,
    -0x1.00000000000020p63,
    -0x1.0000000000002p63
  },
  { // Entry 229
    -0.0,
    -0x1.00000000000010p63,
    -0x1.0000000000001p63
  },
  { // Entry 230
    -0.0,
    -0x1.p63,
    -0x1.0p63
  },
  { // Entry 231
    -0.0,
    -0x1.fffffffffffff0p62,
    -0x1.fffffffffffffp62
  },
  { // Entry 232
    -0.0,
    -0x1.ffffffffffffe0p62,
    -0x1.ffffffffffffep62
  },
  { // Entry 233
    -0.0,
    -0x1.ffffffffffffd0p62,
    -0x1.ffffffffffffdp62
  },
  { // Entry 234
    -0.0,
    -0x1.00000000000030p64,
    -0x1.0000000000003p64
  },
  { // Entry 235
    -0.0,
    -0x1.00000000000020p64,
    -0x1.0000000000002p64
  },
  { // Entry 236
    -0.0,
    -0x1.00000000000010p64,
    -0x1.0000000000001p64
  },
  { // Entry 237
    -0.0,
    -0x1.p64,
    -0x1.0p64
  },
  { // Entry 238
    -0.0,
    -0x1.fffffffffffff0p63,
    -0x1.fffffffffffffp63
  },
  { // Entry 239
    -0.0,
    -0x1.ffffffffffffe0p63,
    -0x1.ffffffffffffep63
  },
  { // Entry 240
    -0.0,
    -0x1.ffffffffffffd0p63,
    -0x1.ffffffffffffdp63
  },
  { // Entry 241
    0.0,
    0x1.p62,
    0x1.0p62
  },
  { // Entry 242
    0.0,
    0x1.40p62,
    0x1.4p62
  },
  { // Entry 243
    0.0,
    0x1.80p62,
    0x1.8p62
  },
  { // Entry 244
    0.0,
    0x1.c0p62,
    0x1.cp62
  },
  { // Entry 245
    0.0,
    0x1.p63,
    0x1.0p63
  },
  { // Entry 246
    0.0,
    0x1.p63,
    0x1.0p63
  },
  { // Entry 247
    0.0,
    0x1.40p63,
    0x1.4p63
  },
  { // Entry 248
    0.0,
    0x1.80p63,
    0x1.8p63
  },
  { // Entry 249
    0.0,
    0x1.c0p63,
    0x1.cp63
  },
  { // Entry 250
    0.0,
    0x1.p64,
    0x1.0p64
  },
  { // Entry 251
    -0.0,
    -0x1.p62,
    -0x1.0p62
  },
  { // Entry 252
    -0.0,
    -0x1.40p62,
    -0x1.4p62
  },
  { // Entry 253
    -0.0,
    -0x1.80p62,
    -0x1.8p62
  },
  { // Entry 254
    -0.0,
    -0x1.c0p62,
    -0x1.cp62
  },
  { // Entry 255
    -0.0,
    -0x1.p63,
    -0x1.0p63
  },
  { // Entry 256
    -0.0,
    -0x1.p63,
    -0x1.0p63
  },
  { // Entry 257
    -0.0,
    -0x1.40p63,
    -0x1.4p63
  },
  { // Entry 258
    -0.0,
    -0x1.80p63,
    -0x1.8p63
  },
  { // Entry 259
    -0.0,
    -0x1.c0p63,
    -0x1.cp63
  },
  { // Entry 260
    -0.0,
    -0x1.p64,
    -0x1.0p64
  },
  { // Entry 261
    0x1.fffff8p-1,
    0x1.fffffff8p30,
    0x1.fffffffbfffffp30
  },
  { // Entry 262
    0.0,
    0x1.fffffffcp30,
    0x1.fffffffc0p30
  },
  { // Entry 263
    0x1.p-22,
    0x1.fffffffcp30,
    0x1.fffffffc00001p30
  },
  { // Entry 264
    -0x1.p-21,
    -0x1.p31,
    -0x1.0000000000001p31
  },
  { // Entry 265
    -0.0,
    -0x1.p31,
    -0x1.0p31
  },
  { // Entry 266
    -0x1.fffff8p-1,
    -0x1.fffffffcp30,
    -0x1.fffffffffffffp30
  },
  { // Entry 267
    0x1.ffffffffffffc0p-1,
    0x1.80p1,
    0x1.fffffffffffffp1
  },
  { // Entry 268
    0.0,
    0x1.p2,
    0x1.0p2
  },
  { // Entry 269
    0x1.p-50,
    0x1.p2,
    0x1.0000000000001p2
  },
  { // Entry 270
    0x1.ffffffffffff80p-1,
    0x1.c0p2,
    0x1.fffffffffffffp2
  },
  { // Entry 271
    0.0,
    0x1.p3,
    0x1.0p3
  },
  { // Entry 272
    0x1.p-49,
    0x1.p3,
    0x1.0000000000001p3
  },
  { // Entry 273
    0x1.ffffffffffffp-1,
    0x1.e0p3,
    0x1.fffffffffffffp3
  },
  { // Entry 274
    0.0,
    0x1.p4,
    0x1.0p4
  },
  { // Entry 275
    0x1.p-48,
    0x1.p4,
    0x1.0000000000001p4
  },
  { // Entry 276
    0x1.fffffffffffep-1,
    0x1.f0p4,
    0x1.fffffffffffffp4
  },
  { // Entry 277
    0.0,
    0x1.p5,
    0x1.0p5
  },
  { // Entry 278
    0x1.p-47,
    0x1.p5,
    0x1.0000000000001p5
  },
  { // Entry 279
    0x1.fffffffffffcp-1,
    0x1.f8p5,
    0x1.fffffffffffffp5
  },
  { // Entry 280
    0.0,
    0x1.p6,
    0x1.0p6
  },
  { // Entry 281
    0x1.p-46,
    0x1.p6,
    0x1.0000000000001p6
  },
  { // Entry 282
    0x1.fffffffffff8p-1,
    0x1.fcp6,
    0x1.fffffffffffffp6
  },
  { // Entry 283
    0.0,
    0x1.p7,
    0x1.0p7
  },
  { // Entry 284
    0x1.p-45,
    0x1.p7,
    0x1.0000000000001p7
  },
  { // Entry 285
    0x1.fffffffffff0p-1,
    0x1.fep7,
    0x1.fffffffffffffp7
  },
  { // Entry 286
    0.0,
    0x1.p8,
    0x1.0p8
  },
  { // Entry 287
    0x1.p-44,
    0x1.p8,
    0x1.0000000000001p8
  },
  { // Entry 288
    0x1.ffffffffffe0p-1,
    0x1.ffp8,
    0x1.fffffffffffffp8
  },
  { // Entry 289
    0.0,
    0x1.p9,
    0x1.0p9
  },
  { // Entry 290
    0x1.p-43,
    0x1.p9,
    0x1.0000000000001p9
  },
  { // Entry 291
    0x1.ffffffffffc0p-1,
    0x1.ff80p9,
    0x1.fffffffffffffp9
  },
  { // Entry 292
    0.0,
    0x1.p10,
    0x1.0p10
  },
  { // Entry 293
    0x1.p-42,
    0x1.p10,
    0x1.0000000000001p10
  },
  { // Entry 294
    0x1.ffffffffff80p-1,
    0x1.ffc0p10,
    0x1.fffffffffffffp10
  },
  { // Entry 295
    0.0,
    0x1.p11,
    0x1.0p11
  },
  { // Entry 296
    0x1.p-41,
    0x1.p11,
    0x1.0000000000001p11
  },
  { // Entry 297
    0x1.ffffffffffp-1,
    0x1.ffe0p11,
    0x1.fffffffffffffp11
  },
  { // Entry 298
    0.0,
    0x1.p12,
    0x1.0p12
  },
  { // Entry 299
    0x1.p-40,
    0x1.p12,
    0x1.0000000000001p12
  },
  { // Entry 300
    0x1.ffffffffffffp-2,
    0x1.p2,
    0x1.1ffffffffffffp2
  },
  { // Entry 301
    0x1.p-1,
    0x1.p2,
    0x1.2p2
  },
  { // Entry 302
    0x1.00000000000080p-1,
    0x1.p2,
    0x1.2000000000001p2
  },
  { // Entry 303
    0x1.fffffffffffep-2,
    0x1.p3,
    0x1.0ffffffffffffp3
  },
  { // Entry 304
    0x1.p-1,
    0x1.p3,
    0x1.1p3
  },
  { // Entry 305
    0x1.000000000001p-1,
    0x1.p3,
    0x1.1000000000001p3
  },
  { // Entry 306
    0x1.fffffffffffcp-2,
    0x1.p4,
    0x1.07fffffffffffp4
  },
  { // Entry 307
    0x1.p-1,
    0x1.p4,
    0x1.080p4
  },
  { // Entry 308
    0x1.000000000002p-1,
    0x1.p4,
    0x1.0800000000001p4
  },
  { // Entry 309
    0x1.fffffffffff8p-2,
    0x1.p5,
    0x1.03fffffffffffp5
  },
  { // Entry 310
    0x1.p-1,
    0x1.p5,
    0x1.040p5
  },
  { // Entry 311
    0x1.000000000004p-1,
    0x1.p5,
    0x1.0400000000001p5
  },
  { // Entry 312
    0x1.fffffffffff0p-2,
    0x1.p6,
    0x1.01fffffffffffp6
  },
  { // Entry 313
    0x1.p-1,
    0x1.p6,
    0x1.020p6
  },
  { // Entry 314
    0x1.000000000008p-1,
    0x1.p6,
    0x1.0200000000001p6
  },
  { // Entry 315
    0x1.ffffffffffe0p-2,
    0x1.p7,
    0x1.00fffffffffffp7
  },
  { // Entry 316
    0x1.p-1,
    0x1.p7,
    0x1.010p7
  },
  { // Entry 317
    0x1.000000000010p-1,
    0x1.p7,
    0x1.0100000000001p7
  },
  { // Entry 318
    0x1.ffffffffffc0p-2,
    0x1.p8,
    0x1.007ffffffffffp8
  },
  { // Entry 319
    0x1.p-1,
    0x1.p8,
    0x1.008p8
  },
  { // Entry 320
    0x1.000000000020p-1,
    0x1.p8,
    0x1.0080000000001p8
  },
  { // Entry 321
    0x1.ffffffffff80p-2,
    0x1.p9,
    0x1.003ffffffffffp9
  },
  { // Entry 322
    0x1.p-1,
    0x1.p9,
    0x1.004p9
  },
  { // Entry 323
    0x1.000000000040p-1,
    0x1.p9,
    0x1.0040000000001p9
  },
  { // Entry 324
    0x1.ffffffffffp-2,
    0x1.p10,
    0x1.001ffffffffffp10
  },
  { // Entry 325
    0x1.p-1,
    0x1.p10,
    0x1.002p10
  },
  { // Entry 326
    0x1.000000000080p-1,
    0x1.p10,
    0x1.0020000000001p10
  },
  { // Entry 327
    0x1.ffffffffffp-2,
    0x1.0040p10,
    0x1.005ffffffffffp10
  },
  { // Entry 328
    0x1.p-1,
    0x1.0040p10,
    0x1.006p10
  },
  { // Entry 329
    0x1.000000000080p-1,
    0x1.0040p10,
    0x1.0060000000001p10
  },
  { // Entry 330
    0x1.fffffffffep-2,
    0x1.p11,
    0x1.000ffffffffffp11
  },
  { // Entry 331
    0x1.p-1,
    0x1.p11,
    0x1.001p11
  },
  { // Entry 332
    0x1.0000000001p-1,
    0x1.p11,
    0x1.0010000000001p11
  },
  { // Entry 333
    0x1.fffffffffcp-2,
    0x1.p12,
    0x1.0007fffffffffp12
  },
  { // Entry 334
    0x1.p-1,
    0x1.p12,
    0x1.00080p12
  },
  { // Entry 335
    0x1.0000000002p-1,
    0x1.p12,
    0x1.0008000000001p12
  },
  { // Entry 336
    0.0,
    HUGE_VAL,
    HUGE_VAL
  },
  { // Entry 337
    -0.0,
    -HUGE_VAL,
    -HUGE_VAL
  },
  { // Entry 338
    0.0,
    0x1.fffffffffffff0p1023,
    0x1.fffffffffffffp1023
  },
  { // Entry 339
    -0.0,
    -0x1.fffffffffffff0p1023,
    -0x1.fffffffffffffp1023
  },
  { // Entry 340
    0.0,
    0x1.ffffffffffffe0p1023,
    0x1.ffffffffffffep1023
  },
  { // Entry 341
    -0.0,
    -0x1.ffffffffffffe0p1023,
    -0x1.ffffffffffffep1023
  },
  { // Entry 342
    0x1.21fb54442d18p-3,
    0x1.80p1,
    0x1.921fb54442d18p1
  },
  { // Entry 343
    -0x1.21fb54442d18p-3,
    -0x1.80p1,
    -0x1.921fb54442d18p1
  },
  { // Entry 344
    0x1.243f6a8885a3p-1,
    0x1.p0,
    0x1.921fb54442d18p0
  },
  { // Entry 345
    -0x1.243f6a8885a3p-1,
    -0x1.p0,
    -0x1.921fb54442d18p0
  },
  { // Entry 346
    0x1.p-52,
    0x1.p0,
    0x1.0000000000001p0
  },
  { // Entry 347
    -0x1.p-52,
    -0x1.p0,
    -0x1.0000000000001p0
  },
  { // Entry 348
    0.0,
    0x1.p0,
    0x1.0p0
  },
  { // Entry 349
    -0.0,
    -0x1.p0,
    -0x1.0p0
  },
  { // Entry 350
    0x1.fffffffffffff0p-1,
    0.0,
    0x1.fffffffffffffp-1
  },
  { // Entry 351
    -0x1.fffffffffffff0p-1,
    -0.0,
    -0x1.fffffffffffffp-1
  },
  { // Entry 352
    0x1.921fb54442d180p-1,
    0.0,
    0x1.921fb54442d18p-1
  },
  { // Entry 353
    -0x1.921fb54442d180p-1,
    -0.0,
    -0x1.921fb54442d18p-1
  },
  { // Entry 354
    0x1.00000000000010p-1022,
    0.0,
    0x1.0000000000001p-1022
  },
  { // Entry 355
    -0x1.00000000000010p-1022,
    -0.0,
    -0x1.0000000000001p-1022
  },
  { // Entry 356
    0x1.p-1022,
    0.0,
    0x1.0p-1022
  },
  { // Entry 357
    -0x1.p-1022,
    -0.0,
    -0x1.0p-1022
  },
  { // Entry 358
    0x1.ffffffffffffe0p-1023,
    0.0,
    0x1.ffffffffffffep-1023
  },
  { // Entry 359
    -0x1.ffffffffffffe0p-1023,
    -0.0,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 360
    0x1.ffffffffffffc0p-1023,
    0.0,
    0x1.ffffffffffffcp-1023
  },
  { // Entry 361
    -0x1.ffffffffffffc0p-1023,
    -0.0,
    -0x1.ffffffffffffcp-1023
  },
  { // Entry 362
    0x1.p-1073,
    0.0,
    0x1.0p-1073
  },
  { // Entry 363
    -0x1.p-1073,
    -0.0,
    -0x1.0p-1073
  },
  { // Entry 364
    0x1.p-1074,
    0.0,
    0x1.0p-1074
  },
  { // Entry 365
    -0x1.p-1074,
    -0.0,
    -0x1.0p-1074
  },
  { // Entry 366
    0.0,
    0.0,
    0.0
  },
  { // Entry 367
    -0.0,
    -0.0,
    -0.0
  }
};
