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

static data_1_2_t<float, float, float> g_fmodf_intel_data[] = {
  { // Entry 0
    0x1.fbp-11,
    0x1.8e77b6p12,
    -0x1.0140p-10
  },
  { // Entry 1
    -0.0,
    -0x1.p-117,
    -0x1.p-117
  },
  { // Entry 2
    -0.0,
    -0x1.p-117,
    0x1.p-117
  },
  { // Entry 3
    0.0,
    0x1.p-117,
    -0x1.p-117
  },
  { // Entry 4
    0.0,
    0x1.p-117,
    0x1.p-117
  },
  { // Entry 5
    -0x1.p-117,
    -0x1.p-117,
    0x1.p15
  },
  { // Entry 6
    -0x1.p-117,
    -0x1.p-117,
    0x1.p16
  },
  { // Entry 7
    0x1.p-117,
    0x1.p-117,
    0x1.p15
  },
  { // Entry 8
    0x1.p-117,
    0x1.p-117,
    0x1.p16
  },
  { // Entry 9
    -0x1.p-117,
    -0x1.p-117,
    0x1.p117
  },
  { // Entry 10
    -0x1.p-117,
    -0x1.p-117,
    0x1.p118
  },
  { // Entry 11
    0x1.p-117,
    0x1.p-117,
    0x1.p117
  },
  { // Entry 12
    0x1.p-117,
    0x1.p-117,
    0x1.p118
  },
  { // Entry 13
    0.0,
    0x1.p15,
    -0x1.p-117
  },
  { // Entry 14
    0.0,
    0x1.p15,
    0x1.p-117
  },
  { // Entry 15
    0.0,
    0x1.p16,
    -0x1.p-117
  },
  { // Entry 16
    0.0,
    0x1.p16,
    0x1.p-117
  },
  { // Entry 17
    0.0,
    0x1.p15,
    0x1.p15
  },
  { // Entry 18
    0x1.p15,
    0x1.p15,
    0x1.p16
  },
  { // Entry 19
    0.0,
    0x1.p16,
    0x1.p15
  },
  { // Entry 20
    0.0,
    0x1.p16,
    0x1.p16
  },
  { // Entry 21
    0x1.p15,
    0x1.p15,
    0x1.p117
  },
  { // Entry 22
    0x1.p15,
    0x1.p15,
    0x1.p118
  },
  { // Entry 23
    0x1.p16,
    0x1.p16,
    0x1.p117
  },
  { // Entry 24
    0x1.p16,
    0x1.p16,
    0x1.p118
  },
  { // Entry 25
    0.0,
    0x1.p117,
    -0x1.p-117
  },
  { // Entry 26
    0.0,
    0x1.p117,
    0x1.p-117
  },
  { // Entry 27
    0.0,
    0x1.p118,
    -0x1.p-117
  },
  { // Entry 28
    0.0,
    0x1.p118,
    0x1.p-117
  },
  { // Entry 29
    0.0,
    0x1.p117,
    0x1.p15
  },
  { // Entry 30
    0.0,
    0x1.p117,
    0x1.p16
  },
  { // Entry 31
    0.0,
    0x1.p118,
    0x1.p15
  },
  { // Entry 32
    0.0,
    0x1.p118,
    0x1.p16
  },
  { // Entry 33
    0.0,
    0x1.p117,
    0x1.p117
  },
  { // Entry 34
    0x1.p117,
    0x1.p117,
    0x1.p118
  },
  { // Entry 35
    0.0,
    0x1.p118,
    0x1.p117
  },
  { // Entry 36
    0.0,
    0x1.p118,
    0x1.p118
  },
  { // Entry 37
    0.0,
    0x1.90p6,
    0x1.40p3
  },
  { // Entry 38
    0x1.p0,
    0x1.90p6,
    0x1.60p3
  },
  { // Entry 39
    0x1.p2,
    0x1.90p6,
    0x1.80p3
  },
  { // Entry 40
    0x1.p0,
    0x1.94p6,
    0x1.40p3
  },
  { // Entry 41
    0x1.p1,
    0x1.94p6,
    0x1.60p3
  },
  { // Entry 42
    0x1.40p2,
    0x1.94p6,
    0x1.80p3
  },
  { // Entry 43
    0x1.p1,
    0x1.98p6,
    0x1.40p3
  },
  { // Entry 44
    0x1.80p1,
    0x1.98p6,
    0x1.60p3
  },
  { // Entry 45
    0x1.80p2,
    0x1.98p6,
    0x1.80p3
  },
  { // Entry 46
    0x1.80p1,
    0x1.9cp6,
    0x1.40p3
  },
  { // Entry 47
    0x1.p2,
    0x1.9cp6,
    0x1.60p3
  },
  { // Entry 48
    0x1.c0p2,
    0x1.9cp6,
    0x1.80p3
  },
  { // Entry 49
    0x1.p2,
    0x1.a0p6,
    0x1.40p3
  },
  { // Entry 50
    0x1.40p2,
    0x1.a0p6,
    0x1.60p3
  },
  { // Entry 51
    0x1.p3,
    0x1.a0p6,
    0x1.80p3
  },
  { // Entry 52
    0x1.40p2,
    0x1.a4p6,
    0x1.40p3
  },
  { // Entry 53
    0x1.80p2,
    0x1.a4p6,
    0x1.60p3
  },
  { // Entry 54
    0x1.20p3,
    0x1.a4p6,
    0x1.80p3
  },
  { // Entry 55
    0x1.80p2,
    0x1.a8p6,
    0x1.40p3
  },
  { // Entry 56
    0x1.c0p2,
    0x1.a8p6,
    0x1.60p3
  },
  { // Entry 57
    0x1.40p3,
    0x1.a8p6,
    0x1.80p3
  },
  { // Entry 58
    0x1.c0p2,
    0x1.acp6,
    0x1.40p3
  },
  { // Entry 59
    0x1.p3,
    0x1.acp6,
    0x1.60p3
  },
  { // Entry 60
    0x1.60p3,
    0x1.acp6,
    0x1.80p3
  },
  { // Entry 61
    0x1.p3,
    0x1.b0p6,
    0x1.40p3
  },
  { // Entry 62
    0x1.20p3,
    0x1.b0p6,
    0x1.60p3
  },
  { // Entry 63
    0.0,
    0x1.b0p6,
    0x1.80p3
  },
  { // Entry 64
    0x1.20p3,
    0x1.b4p6,
    0x1.40p3
  },
  { // Entry 65
    0x1.40p3,
    0x1.b4p6,
    0x1.60p3
  },
  { // Entry 66
    0x1.p0,
    0x1.b4p6,
    0x1.80p3
  },
  { // Entry 67
    0.0,
    0x1.b8p6,
    0x1.40p3
  },
  { // Entry 68
    0.0,
    0x1.b8p6,
    0x1.60p3
  },
  { // Entry 69
    0x1.p1,
    0x1.b8p6,
    0x1.80p3
  },
  { // Entry 70
    -0.0,
    -0x1.000002p0,
    -0x1.000002p0
  },
  { // Entry 71
    -0x1.p-23,
    -0x1.000002p0,
    -0x1.p0
  },
  { // Entry 72
    -0x1.80p-23,
    -0x1.000002p0,
    -0x1.fffffep-1
  },
  { // Entry 73
    -0x1.p0,
    -0x1.p0,
    -0x1.000002p0
  },
  { // Entry 74
    -0.0,
    -0x1.p0,
    -0x1.p0
  },
  { // Entry 75
    -0x1.p-24,
    -0x1.p0,
    -0x1.fffffep-1
  },
  { // Entry 76
    -0x1.fffffep-1,
    -0x1.fffffep-1,
    -0x1.000002p0
  },
  { // Entry 77
    -0x1.fffffep-1,
    -0x1.fffffep-1,
    -0x1.p0
  },
  { // Entry 78
    -0.0,
    -0x1.fffffep-1,
    -0x1.fffffep-1
  },
  { // Entry 79
    -0x1.80p-23,
    -0x1.000002p0,
    0x1.fffffep-1
  },
  { // Entry 80
    -0x1.p-23,
    -0x1.000002p0,
    0x1.p0
  },
  { // Entry 81
    -0.0,
    -0x1.000002p0,
    0x1.000002p0
  },
  { // Entry 82
    -0x1.p-24,
    -0x1.p0,
    0x1.fffffep-1
  },
  { // Entry 83
    -0.0,
    -0x1.p0,
    0x1.p0
  },
  { // Entry 84
    -0x1.p0,
    -0x1.p0,
    0x1.000002p0
  },
  { // Entry 85
    -0.0,
    -0x1.fffffep-1,
    0x1.fffffep-1
  },
  { // Entry 86
    -0x1.fffffep-1,
    -0x1.fffffep-1,
    0x1.p0
  },
  { // Entry 87
    -0x1.fffffep-1,
    -0x1.fffffep-1,
    0x1.000002p0
  },
  { // Entry 88
    0x1.fffffep-1,
    0x1.fffffep-1,
    -0x1.000002p0
  },
  { // Entry 89
    0x1.fffffep-1,
    0x1.fffffep-1,
    -0x1.p0
  },
  { // Entry 90
    0.0,
    0x1.fffffep-1,
    -0x1.fffffep-1
  },
  { // Entry 91
    0x1.p0,
    0x1.p0,
    -0x1.000002p0
  },
  { // Entry 92
    0.0,
    0x1.p0,
    -0x1.p0
  },
  { // Entry 93
    0x1.p-24,
    0x1.p0,
    -0x1.fffffep-1
  },
  { // Entry 94
    0.0,
    0x1.000002p0,
    -0x1.000002p0
  },
  { // Entry 95
    0x1.p-23,
    0x1.000002p0,
    -0x1.p0
  },
  { // Entry 96
    0x1.80p-23,
    0x1.000002p0,
    -0x1.fffffep-1
  },
  { // Entry 97
    0.0,
    0x1.fffffep-1,
    0x1.fffffep-1
  },
  { // Entry 98
    0x1.fffffep-1,
    0x1.fffffep-1,
    0x1.p0
  },
  { // Entry 99
    0x1.fffffep-1,
    0x1.fffffep-1,
    0x1.000002p0
  },
  { // Entry 100
    0x1.p-24,
    0x1.p0,
    0x1.fffffep-1
  },
  { // Entry 101
    0.0,
    0x1.p0,
    0x1.p0
  },
  { // Entry 102
    0x1.p0,
    0x1.p0,
    0x1.000002p0
  },
  { // Entry 103
    0x1.80p-23,
    0x1.000002p0,
    0x1.fffffep-1
  },
  { // Entry 104
    0x1.p-23,
    0x1.000002p0,
    0x1.p0
  },
  { // Entry 105
    0.0,
    0x1.000002p0,
    0x1.000002p0
  },
  { // Entry 106
    -0.0,
    -0x1.p-149,
    0x1.p-149
  },
  { // Entry 107
    0.0,
    0.0,
    0x1.p-149
  },
  { // Entry 108
    0.0,
    0x1.p-149,
    0x1.p-149
  },
  { // Entry 109
    -0.0,
    -0x1.p-149,
    -0x1.p-149
  },
  { // Entry 110
    0.0,
    0.0,
    -0x1.p-149
  },
  { // Entry 111
    0.0,
    0x1.p-149,
    -0x1.p-149
  },
  { // Entry 112
    -0x1.p-149,
    -0x1.p-149,
    0x1.fffffep127
  },
  { // Entry 113
    0.0,
    0.0,
    0x1.fffffep127
  },
  { // Entry 114
    0x1.p-149,
    0x1.p-149,
    0x1.fffffep127
  },
  { // Entry 115
    -0x1.p-149,
    -0x1.p-149,
    -0x1.fffffep127
  },
  { // Entry 116
    0.0,
    0.0,
    -0x1.fffffep127
  },
  { // Entry 117
    0x1.p-149,
    0x1.p-149,
    -0x1.fffffep127
  },
  { // Entry 118
    0x1.p-149,
    0x1.p-149,
    0x1.fffffep127
  },
  { // Entry 119
    -0x1.p-149,
    -0x1.p-149,
    -0x1.fffffep127
  },
  { // Entry 120
    -0x1.p-149,
    -0x1.p-149,
    0x1.fffffep127
  },
  { // Entry 121
    0x1.p-149,
    0x1.p-149,
    -0x1.fffffep127
  },
  { // Entry 122
    0.0,
    0x1.fffffep127,
    0x1.p-149
  },
  { // Entry 123
    -0.0,
    -0x1.fffffep127,
    -0x1.p-149
  },
  { // Entry 124
    -0.0,
    -0x1.fffffep127,
    0x1.p-149
  },
  { // Entry 125
    0.0,
    0x1.fffffep127,
    -0x1.p-149
  },
  { // Entry 126
    0.0,
    0x1.fffffep127,
    0x1.fffffep127
  },
  { // Entry 127
    0.0,
    0x1.fffffep127,
    -0x1.fffffep127
  },
  { // Entry 128
    -0.0,
    -0x1.fffffep127,
    0x1.fffffep127
  },
  { // Entry 129
    -0.0,
    -0x1.fffffep127,
    -0x1.fffffep127
  },
  { // Entry 130
    -0x1.80p-1,
    -0x1.000002p22,
    0x1.fffffep-1
  },
  { // Entry 131
    -0x1.p-1,
    -0x1.000002p22,
    0x1.p0
  },
  { // Entry 132
    -0.0,
    -0x1.000002p22,
    0x1.000002p0
  },
  { // Entry 133
    -0x1.p-2,
    -0x1.p22,
    0x1.fffffep-1
  },
  { // Entry 134
    -0.0,
    -0x1.p22,
    0x1.p0
  },
  { // Entry 135
    -0x1.000004p-1,
    -0x1.p22,
    0x1.000002p0
  },
  { // Entry 136
    -0.0,
    -0x1.fffffep21,
    0x1.fffffep-1
  },
  { // Entry 137
    -0x1.80p-1,
    -0x1.fffffep21,
    0x1.p0
  },
  { // Entry 138
    -0x1.000008p-2,
    -0x1.fffffep21,
    0x1.000002p0
  },
  { // Entry 139
    0.0,
    0x1.fffffep22,
    0x1.fffffep-1
  },
  { // Entry 140
    0x1.p-1,
    0x1.fffffep22,
    0x1.p0
  },
  { // Entry 141
    0x1.000008p-1,
    0x1.fffffep22,
    0x1.000002p0
  },
  { // Entry 142
    0x1.p-1,
    0x1.p23,
    0x1.fffffep-1
  },
  { // Entry 143
    0.0,
    0x1.p23,
    0x1.p0
  },
  { // Entry 144
    0x1.p-23,
    0x1.p23,
    0x1.000002p0
  },
  { // Entry 145
    0x1.000002p-1,
    0x1.000002p23,
    0x1.fffffep-1
  },
  { // Entry 146
    0.0,
    0x1.000002p23,
    0x1.p0
  },
  { // Entry 147
    0.0,
    0x1.000002p23,
    0x1.000002p0
  },
  { // Entry 148
    -0x1.80p-23,
    -0x1.000002p24,
    0x1.fffffep-1
  },
  { // Entry 149
    -0.0,
    -0x1.000002p24,
    0x1.p0
  },
  { // Entry 150
    -0.0,
    -0x1.000002p24,
    0x1.000002p0
  },
  { // Entry 151
    -0x1.p-24,
    -0x1.p24,
    0x1.fffffep-1
  },
  { // Entry 152
    -0.0,
    -0x1.p24,
    0x1.p0
  },
  { // Entry 153
    -0x1.p-22,
    -0x1.p24,
    0x1.000002p0
  },
  { // Entry 154
    -0.0,
    -0x1.fffffep23,
    0x1.fffffep-1
  },
  { // Entry 155
    -0.0,
    -0x1.fffffep23,
    0x1.p0
  },
  { // Entry 156
    -0x1.80p-22,
    -0x1.fffffep23,
    0x1.000002p0
  },
  { // Entry 157
    0.0,
    0x1.fffffep21,
    0x1.fffffep-1
  },
  { // Entry 158
    0x1.80p-1,
    0x1.fffffep21,
    0x1.p0
  },
  { // Entry 159
    0x1.000008p-2,
    0x1.fffffep21,
    0x1.000002p0
  },
  { // Entry 160
    0x1.p-2,
    0x1.p22,
    0x1.fffffep-1
  },
  { // Entry 161
    0.0,
    0x1.p22,
    0x1.p0
  },
  { // Entry 162
    0x1.000004p-1,
    0x1.p22,
    0x1.000002p0
  },
  { // Entry 163
    0x1.80p-1,
    0x1.000002p22,
    0x1.fffffep-1
  },
  { // Entry 164
    0x1.p-1,
    0x1.000002p22,
    0x1.p0
  },
  { // Entry 165
    0.0,
    0x1.000002p22,
    0x1.000002p0
  },
  { // Entry 166
    0.0,
    0x1.fffffep22,
    0x1.fffffep-1
  },
  { // Entry 167
    0x1.p-1,
    0x1.fffffep22,
    0x1.p0
  },
  { // Entry 168
    0x1.000008p-1,
    0x1.fffffep22,
    0x1.000002p0
  },
  { // Entry 169
    0x1.p-1,
    0x1.p23,
    0x1.fffffep-1
  },
  { // Entry 170
    0.0,
    0x1.p23,
    0x1.p0
  },
  { // Entry 171
    0x1.p-23,
    0x1.p23,
    0x1.000002p0
  },
  { // Entry 172
    0x1.000002p-1,
    0x1.000002p23,
    0x1.fffffep-1
  },
  { // Entry 173
    0.0,
    0x1.000002p23,
    0x1.p0
  },
  { // Entry 174
    0.0,
    0x1.000002p23,
    0x1.000002p0
  },
  { // Entry 175
    -0.0,
    -0x1.000002p24,
    -0x1.000002p0
  },
  { // Entry 176
    -0.0,
    -0x1.000002p24,
    -0x1.p0
  },
  { // Entry 177
    -0x1.80p-23,
    -0x1.000002p24,
    -0x1.fffffep-1
  },
  { // Entry 178
    -0x1.p-22,
    -0x1.p24,
    -0x1.000002p0
  },
  { // Entry 179
    -0.0,
    -0x1.p24,
    -0x1.p0
  },
  { // Entry 180
    -0x1.p-24,
    -0x1.p24,
    -0x1.fffffep-1
  },
  { // Entry 181
    -0x1.80p-22,
    -0x1.fffffep23,
    -0x1.000002p0
  },
  { // Entry 182
    -0.0,
    -0x1.fffffep23,
    -0x1.p0
  },
  { // Entry 183
    -0.0,
    -0x1.fffffep23,
    -0x1.fffffep-1
  },
  { // Entry 184
    0x1.fffffep127,
    0x1.fffffep127,
    HUGE_VALF
  },
  { // Entry 185
    0x1.fffffep127,
    0x1.fffffep127,
    -HUGE_VALF
  },
  { // Entry 186
    -0x1.fffffep127,
    -0x1.fffffep127,
    HUGE_VALF
  },
  { // Entry 187
    -0x1.fffffep127,
    -0x1.fffffep127,
    -HUGE_VALF
  },
  { // Entry 188
    0x1.p-126,
    0x1.p-126,
    HUGE_VALF
  },
  { // Entry 189
    -0x1.p-126,
    -0x1.p-126,
    HUGE_VALF
  },
  { // Entry 190
    0x1.p-126,
    0x1.p-126,
    -HUGE_VALF
  },
  { // Entry 191
    -0x1.p-126,
    -0x1.p-126,
    -HUGE_VALF
  },
  { // Entry 192
    0x1.p-149,
    0x1.p-149,
    HUGE_VALF
  },
  { // Entry 193
    -0x1.p-149,
    -0x1.p-149,
    HUGE_VALF
  },
  { // Entry 194
    0x1.p-149,
    0x1.p-149,
    -HUGE_VALF
  },
  { // Entry 195
    -0x1.p-149,
    -0x1.p-149,
    -HUGE_VALF
  },
  { // Entry 196
    0.0,
    0.0f,
    HUGE_VALF
  },
  { // Entry 197
    -0.0,
    -0.0f,
    HUGE_VALF
  },
  { // Entry 198
    0.0,
    0.0f,
    -HUGE_VALF
  },
  { // Entry 199
    -0.0,
    -0.0f,
    -HUGE_VALF
  },
  { // Entry 200
    0.0,
    0x1.fffffep127,
    0x1.fffffep127
  },
  { // Entry 201
    0.0,
    0x1.fffffep127,
    -0x1.fffffep127
  },
  { // Entry 202
    -0.0,
    -0x1.fffffep127,
    0x1.fffffep127
  },
  { // Entry 203
    -0.0,
    -0x1.fffffep127,
    -0x1.fffffep127
  },
  { // Entry 204
    0.0,
    0x1.fffffep127,
    0x1.p-126
  },
  { // Entry 205
    0.0,
    0x1.fffffep127,
    -0x1.p-126
  },
  { // Entry 206
    -0.0,
    -0x1.fffffep127,
    0x1.p-126
  },
  { // Entry 207
    -0.0,
    -0x1.fffffep127,
    -0x1.p-126
  },
  { // Entry 208
    0.0,
    0x1.fffffep127,
    0x1.p-149
  },
  { // Entry 209
    0.0,
    0x1.fffffep127,
    -0x1.p-149
  },
  { // Entry 210
    -0.0,
    -0x1.fffffep127,
    0x1.p-149
  },
  { // Entry 211
    -0.0,
    -0x1.fffffep127,
    -0x1.p-149
  },
  { // Entry 212
    0x1.p-126,
    0x1.p-126,
    0x1.fffffep127
  },
  { // Entry 213
    -0x1.p-126,
    -0x1.p-126,
    0x1.fffffep127
  },
  { // Entry 214
    0x1.p-126,
    0x1.p-126,
    -0x1.fffffep127
  },
  { // Entry 215
    -0x1.p-126,
    -0x1.p-126,
    -0x1.fffffep127
  },
  { // Entry 216
    0x1.p-149,
    0x1.p-149,
    0x1.fffffep127
  },
  { // Entry 217
    -0x1.p-149,
    -0x1.p-149,
    0x1.fffffep127
  },
  { // Entry 218
    0x1.p-149,
    0x1.p-149,
    -0x1.fffffep127
  },
  { // Entry 219
    -0x1.p-149,
    -0x1.p-149,
    -0x1.fffffep127
  },
  { // Entry 220
    0.0,
    0.0f,
    0x1.fffffep127
  },
  { // Entry 221
    -0.0,
    -0.0f,
    0x1.fffffep127
  },
  { // Entry 222
    0.0,
    0.0f,
    -0x1.fffffep127
  },
  { // Entry 223
    -0.0,
    -0.0f,
    -0x1.fffffep127
  },
  { // Entry 224
    0.0,
    0x1.p-126,
    0x1.p-126
  },
  { // Entry 225
    0.0,
    0x1.p-126,
    -0x1.p-126
  },
  { // Entry 226
    -0.0,
    -0x1.p-126,
    0x1.p-126
  },
  { // Entry 227
    -0.0,
    -0x1.p-126,
    -0x1.p-126
  },
  { // Entry 228
    0x1.p-149,
    0x1.p-126,
    0x1.fffffcp-127
  },
  { // Entry 229
    0x1.p-149,
    0x1.p-126,
    -0x1.fffffcp-127
  },
  { // Entry 230
    -0x1.p-149,
    -0x1.p-126,
    0x1.fffffcp-127
  },
  { // Entry 231
    -0x1.p-149,
    -0x1.p-126,
    -0x1.fffffcp-127
  },
  { // Entry 232
    0.0,
    0x1.p-126,
    0x1.p-149
  },
  { // Entry 233
    0.0,
    0x1.p-126,
    -0x1.p-149
  },
  { // Entry 234
    -0.0,
    -0x1.p-126,
    0x1.p-149
  },
  { // Entry 235
    -0.0,
    -0x1.p-126,
    -0x1.p-149
  },
  { // Entry 236
    0x1.fffffcp-127,
    0x1.fffffcp-127,
    0x1.p-126
  },
  { // Entry 237
    0x1.fffffcp-127,
    0x1.fffffcp-127,
    -0x1.p-126
  },
  { // Entry 238
    -0x1.fffffcp-127,
    -0x1.fffffcp-127,
    0x1.p-126
  },
  { // Entry 239
    -0x1.fffffcp-127,
    -0x1.fffffcp-127,
    -0x1.p-126
  },
  { // Entry 240
    0x1.p-149,
    0x1.p-149,
    0x1.p-126
  },
  { // Entry 241
    0x1.p-149,
    0x1.p-149,
    -0x1.p-126
  },
  { // Entry 242
    -0x1.p-149,
    -0x1.p-149,
    0x1.p-126
  },
  { // Entry 243
    -0x1.p-149,
    -0x1.p-149,
    -0x1.p-126
  },
  { // Entry 244
    0.0,
    0.0f,
    0x1.p-126
  },
  { // Entry 245
    0.0,
    0.0f,
    -0x1.p-126
  },
  { // Entry 246
    -0.0,
    -0.0f,
    0x1.p-126
  },
  { // Entry 247
    -0.0,
    -0.0f,
    -0x1.p-126
  },
  { // Entry 248
    0.0,
    0x1.p-149,
    0x1.p-149
  },
  { // Entry 249
    0.0,
    0x1.p-149,
    -0x1.p-149
  },
  { // Entry 250
    -0.0,
    -0x1.p-149,
    0x1.p-149
  },
  { // Entry 251
    -0.0,
    -0x1.p-149,
    -0x1.p-149
  },
  { // Entry 252
    0.0,
    0.0f,
    0x1.p-149
  },
  { // Entry 253
    0.0,
    0.0f,
    -0x1.p-149
  },
  { // Entry 254
    -0.0,
    -0.0f,
    0x1.p-149
  },
  { // Entry 255
    -0.0,
    -0.0f,
    -0x1.p-149
  }
};
