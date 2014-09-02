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

static data_1_2_t<float, float, float> g_copysignf_intel_data[] = {
  { // Entry 0
    -0x1.p-10,
    -0x1.p-10,
    -0x1.p-10
  },
  { // Entry 1
    0x1.p-10,
    -0x1.p-10,
    0x1.p-10
  },
  { // Entry 2
    -0x1.p-10,
    0x1.p-10,
    -0x1.p-10
  },
  { // Entry 3
    0x1.p-10,
    0x1.p-10,
    0x1.p-10
  },
  { // Entry 4
    -0x1.40p3,
    -0x1.40p3,
    -0x1.40p3
  },
  { // Entry 5
    0x1.40p3,
    -0x1.40p3,
    0x1.40p3
  },
  { // Entry 6
    -0x1.40p3,
    0x1.40p3,
    -0x1.40p3
  },
  { // Entry 7
    0x1.40p3,
    0x1.40p3,
    0x1.40p3
  },
  { // Entry 8
    -0x1.p-148,
    -0x1.p-148,
    -0x1.p-149
  },
  { // Entry 9
    0x1.p-148,
    -0x1.p-148,
    0.0
  },
  { // Entry 10
    0x1.p-148,
    -0x1.p-148,
    0x1.p-149
  },
  { // Entry 11
    -0x1.p-149,
    -0x1.p-149,
    -0x1.p-149
  },
  { // Entry 12
    0x1.p-149,
    -0x1.p-149,
    0.0
  },
  { // Entry 13
    0x1.p-149,
    -0x1.p-149,
    0x1.p-149
  },
  { // Entry 14
    -0.0,
    0.0,
    -0x1.p-149
  },
  { // Entry 15
    0.0,
    0.0,
    0.0
  },
  { // Entry 16
    0.0,
    0.0,
    0x1.p-149
  },
  { // Entry 17
    -0x1.p-149,
    0x1.p-149,
    -0x1.p-149
  },
  { // Entry 18
    0x1.p-149,
    0x1.p-149,
    0.0
  },
  { // Entry 19
    0x1.p-149,
    0x1.p-149,
    0x1.p-149
  },
  { // Entry 20
    -0x1.p-148,
    0x1.p-148,
    -0x1.p-149
  },
  { // Entry 21
    0x1.p-148,
    0x1.p-148,
    0.0
  },
  { // Entry 22
    0x1.p-148,
    0x1.p-148,
    0x1.p-149
  },
  { // Entry 23
    -0x1.fffffep127,
    -0x1.fffffep127,
    -0x1.fffffep127
  },
  { // Entry 24
    0x1.fffffep127,
    0x1.fffffep127,
    0x1.fffffep127
  },
  { // Entry 25
    0x1.fffffep127,
    -0x1.fffffep127,
    0x1.fffffep127
  },
  { // Entry 26
    -0x1.fffffep127,
    0x1.fffffep127,
    -0x1.fffffep127
  },
  { // Entry 27
    -0x1.p-149,
    -0x1.p-149,
    -0x1.fffffep127
  },
  { // Entry 28
    -0.0,
    0.0,
    -0x1.fffffep127
  },
  { // Entry 29
    -0x1.p-149,
    0x1.p-149,
    -0x1.fffffep127
  },
  { // Entry 30
    0x1.p-149,
    -0x1.p-149,
    0x1.fffffep127
  },
  { // Entry 31
    0.0,
    0.0,
    0x1.fffffep127
  },
  { // Entry 32
    0x1.p-149,
    0x1.p-149,
    0x1.fffffep127
  },
  { // Entry 33
    -0x1.fffffep127,
    -0x1.fffffep127,
    -0x1.p-149
  },
  { // Entry 34
    0x1.fffffep127,
    -0x1.fffffep127,
    0.0
  },
  { // Entry 35
    0x1.fffffep127,
    -0x1.fffffep127,
    0x1.p-149
  },
  { // Entry 36
    -0x1.fffffep127,
    0x1.fffffep127,
    -0x1.p-149
  },
  { // Entry 37
    0x1.fffffep127,
    0x1.fffffep127,
    0.0
  },
  { // Entry 38
    0x1.fffffep127,
    0x1.fffffep127,
    0x1.p-149
  },
  { // Entry 39
    0x1.000004p-127,
    -0x1.000004p-127,
    0x1.fffff8p-128
  },
  { // Entry 40
    0x1.000004p-127,
    -0x1.000004p-127,
    0x1.p-127
  },
  { // Entry 41
    0x1.000004p-127,
    -0x1.000004p-127,
    0x1.000004p-127
  },
  { // Entry 42
    0x1.p-127,
    -0x1.p-127,
    0x1.fffff8p-128
  },
  { // Entry 43
    0x1.p-127,
    -0x1.p-127,
    0x1.p-127
  },
  { // Entry 44
    0x1.p-127,
    -0x1.p-127,
    0x1.000004p-127
  },
  { // Entry 45
    0x1.fffff8p-128,
    -0x1.fffff8p-128,
    0x1.fffff8p-128
  },
  { // Entry 46
    0x1.fffff8p-128,
    -0x1.fffff8p-128,
    0x1.p-127
  },
  { // Entry 47
    0x1.fffff8p-128,
    -0x1.fffff8p-128,
    0x1.000004p-127
  },
  { // Entry 48
    0x1.000002p0,
    -0x1.000002p0,
    0x1.fffffep-1
  },
  { // Entry 49
    0x1.000002p0,
    -0x1.000002p0,
    0x1.p0
  },
  { // Entry 50
    0x1.000002p0,
    -0x1.000002p0,
    0x1.000002p0
  },
  { // Entry 51
    0x1.p0,
    -0x1.p0,
    0x1.fffffep-1
  },
  { // Entry 52
    0x1.p0,
    -0x1.p0,
    0x1.p0
  },
  { // Entry 53
    0x1.p0,
    -0x1.p0,
    0x1.000002p0
  },
  { // Entry 54
    0x1.fffffep-1,
    -0x1.fffffep-1,
    0x1.fffffep-1
  },
  { // Entry 55
    0x1.fffffep-1,
    -0x1.fffffep-1,
    0x1.p0
  },
  { // Entry 56
    0x1.fffffep-1,
    -0x1.fffffep-1,
    0x1.000002p0
  },
  { // Entry 57
    0x1.000002p127,
    -0x1.000002p127,
    0x1.fffffep126
  },
  { // Entry 58
    0x1.000002p127,
    -0x1.000002p127,
    0x1.p127
  },
  { // Entry 59
    0x1.000002p127,
    -0x1.000002p127,
    0x1.000002p127
  },
  { // Entry 60
    0x1.p127,
    -0x1.p127,
    0x1.fffffep126
  },
  { // Entry 61
    0x1.p127,
    -0x1.p127,
    0x1.p127
  },
  { // Entry 62
    0x1.p127,
    -0x1.p127,
    0x1.000002p127
  },
  { // Entry 63
    0x1.fffffep126,
    -0x1.fffffep126,
    0x1.fffffep126
  },
  { // Entry 64
    0x1.fffffep126,
    -0x1.fffffep126,
    0x1.p127
  },
  { // Entry 65
    0x1.fffffep126,
    -0x1.fffffep126,
    0x1.000002p127
  },
  { // Entry 66
    -0x1.fffff8p-128,
    0x1.fffff8p-128,
    -0x1.000004p-127
  },
  { // Entry 67
    -0x1.fffff8p-128,
    0x1.fffff8p-128,
    -0x1.p-127
  },
  { // Entry 68
    -0x1.fffff8p-128,
    0x1.fffff8p-128,
    -0x1.fffff8p-128
  },
  { // Entry 69
    -0x1.p-127,
    0x1.p-127,
    -0x1.000004p-127
  },
  { // Entry 70
    -0x1.p-127,
    0x1.p-127,
    -0x1.p-127
  },
  { // Entry 71
    -0x1.p-127,
    0x1.p-127,
    -0x1.fffff8p-128
  },
  { // Entry 72
    -0x1.000004p-127,
    0x1.000004p-127,
    -0x1.000004p-127
  },
  { // Entry 73
    -0x1.000004p-127,
    0x1.000004p-127,
    -0x1.p-127
  },
  { // Entry 74
    -0x1.000004p-127,
    0x1.000004p-127,
    -0x1.fffff8p-128
  },
  { // Entry 75
    -0x1.fffffep-1,
    0x1.fffffep-1,
    -0x1.000002p0
  },
  { // Entry 76
    -0x1.fffffep-1,
    0x1.fffffep-1,
    -0x1.p0
  },
  { // Entry 77
    -0x1.fffffep-1,
    0x1.fffffep-1,
    -0x1.fffffep-1
  },
  { // Entry 78
    -0x1.p0,
    0x1.p0,
    -0x1.000002p0
  },
  { // Entry 79
    -0x1.p0,
    0x1.p0,
    -0x1.p0
  },
  { // Entry 80
    -0x1.p0,
    0x1.p0,
    -0x1.fffffep-1
  },
  { // Entry 81
    -0x1.000002p0,
    0x1.000002p0,
    -0x1.000002p0
  },
  { // Entry 82
    -0x1.000002p0,
    0x1.000002p0,
    -0x1.p0
  },
  { // Entry 83
    -0x1.000002p0,
    0x1.000002p0,
    -0x1.fffffep-1
  },
  { // Entry 84
    -0x1.fffffep126,
    0x1.fffffep126,
    -0x1.000002p127
  },
  { // Entry 85
    -0x1.fffffep126,
    0x1.fffffep126,
    -0x1.p127
  },
  { // Entry 86
    -0x1.fffffep126,
    0x1.fffffep126,
    -0x1.fffffep126
  },
  { // Entry 87
    -0x1.p127,
    0x1.p127,
    -0x1.000002p127
  },
  { // Entry 88
    -0x1.p127,
    0x1.p127,
    -0x1.p127
  },
  { // Entry 89
    -0x1.p127,
    0x1.p127,
    -0x1.fffffep126
  },
  { // Entry 90
    -0x1.000002p127,
    0x1.000002p127,
    -0x1.000002p127
  },
  { // Entry 91
    -0x1.000002p127,
    0x1.000002p127,
    -0x1.p127
  },
  { // Entry 92
    -0x1.000002p127,
    0x1.000002p127,
    -0x1.fffffep126
  },
  { // Entry 93
    HUGE_VALF,
    HUGE_VALF,
    HUGE_VALF
  },
  { // Entry 94
    HUGE_VALF,
    HUGE_VALF,
    0x1.fffffep127
  },
  { // Entry 95
    HUGE_VALF,
    HUGE_VALF,
    0x1.p-126
  },
  { // Entry 96
    HUGE_VALF,
    HUGE_VALF,
    0x1.fffffcp-127
  },
  { // Entry 97
    HUGE_VALF,
    HUGE_VALF,
    0x1.p-149
  },
  { // Entry 98
    HUGE_VALF,
    HUGE_VALF,
    0.0f
  },
  { // Entry 99
    -HUGE_VALF,
    HUGE_VALF,
    -0.0f
  },
  { // Entry 100
    -HUGE_VALF,
    HUGE_VALF,
    -0x1.p-149
  },
  { // Entry 101
    -HUGE_VALF,
    HUGE_VALF,
    -0x1.fffffcp-127
  },
  { // Entry 102
    -HUGE_VALF,
    HUGE_VALF,
    -0x1.p-126
  },
  { // Entry 103
    -HUGE_VALF,
    HUGE_VALF,
    -0x1.p0
  },
  { // Entry 104
    -HUGE_VALF,
    HUGE_VALF,
    -0x1.fffffep127
  },
  { // Entry 105
    -HUGE_VALF,
    HUGE_VALF,
    -HUGE_VALF
  },
  { // Entry 106
    0x1.fffffep127,
    0x1.fffffep127,
    HUGE_VALF
  },
  { // Entry 107
    0x1.fffffep127,
    0x1.fffffep127,
    0x1.fffffep127
  },
  { // Entry 108
    0x1.fffffep127,
    0x1.fffffep127,
    0x1.p-126
  },
  { // Entry 109
    0x1.fffffep127,
    0x1.fffffep127,
    0x1.fffffcp-127
  },
  { // Entry 110
    0x1.fffffep127,
    0x1.fffffep127,
    0x1.p-149
  },
  { // Entry 111
    0x1.fffffep127,
    0x1.fffffep127,
    0.0f
  },
  { // Entry 112
    -0x1.fffffep127,
    0x1.fffffep127,
    -0.0f
  },
  { // Entry 113
    -0x1.fffffep127,
    0x1.fffffep127,
    -0x1.p-149
  },
  { // Entry 114
    -0x1.fffffep127,
    0x1.fffffep127,
    -0x1.fffffcp-127
  },
  { // Entry 115
    -0x1.fffffep127,
    0x1.fffffep127,
    -0x1.p-126
  },
  { // Entry 116
    -0x1.fffffep127,
    0x1.fffffep127,
    -0x1.p0
  },
  { // Entry 117
    -0x1.fffffep127,
    0x1.fffffep127,
    -0x1.fffffep127
  },
  { // Entry 118
    -0x1.fffffep127,
    0x1.fffffep127,
    -HUGE_VALF
  },
  { // Entry 119
    0x1.p-126,
    0x1.p-126,
    HUGE_VALF
  },
  { // Entry 120
    0x1.p-126,
    0x1.p-126,
    0x1.fffffep127
  },
  { // Entry 121
    0x1.p-126,
    0x1.p-126,
    0x1.p-126
  },
  { // Entry 122
    0x1.p-126,
    0x1.p-126,
    0x1.fffffcp-127
  },
  { // Entry 123
    0x1.p-126,
    0x1.p-126,
    0x1.p-149
  },
  { // Entry 124
    0x1.p-126,
    0x1.p-126,
    0.0f
  },
  { // Entry 125
    -0x1.p-126,
    0x1.p-126,
    -0.0f
  },
  { // Entry 126
    -0x1.p-126,
    0x1.p-126,
    -0x1.p-149
  },
  { // Entry 127
    -0x1.p-126,
    0x1.p-126,
    -0x1.fffffcp-127
  },
  { // Entry 128
    -0x1.p-126,
    0x1.p-126,
    -0x1.p-126
  },
  { // Entry 129
    -0x1.p-126,
    0x1.p-126,
    -0x1.p0
  },
  { // Entry 130
    -0x1.p-126,
    0x1.p-126,
    -0x1.fffffep127
  },
  { // Entry 131
    -0x1.p-126,
    0x1.p-126,
    -HUGE_VALF
  },
  { // Entry 132
    0x1.fffffcp-127,
    0x1.fffffcp-127,
    HUGE_VALF
  },
  { // Entry 133
    0x1.fffffcp-127,
    0x1.fffffcp-127,
    0x1.fffffep127
  },
  { // Entry 134
    0x1.fffffcp-127,
    0x1.fffffcp-127,
    0x1.p-126
  },
  { // Entry 135
    0x1.fffffcp-127,
    0x1.fffffcp-127,
    0x1.fffffcp-127
  },
  { // Entry 136
    0x1.fffffcp-127,
    0x1.fffffcp-127,
    0x1.p-149
  },
  { // Entry 137
    0x1.fffffcp-127,
    0x1.fffffcp-127,
    0.0f
  },
  { // Entry 138
    -0x1.fffffcp-127,
    0x1.fffffcp-127,
    -0.0f
  },
  { // Entry 139
    -0x1.fffffcp-127,
    0x1.fffffcp-127,
    -0x1.p-149
  },
  { // Entry 140
    -0x1.fffffcp-127,
    0x1.fffffcp-127,
    -0x1.fffffcp-127
  },
  { // Entry 141
    -0x1.fffffcp-127,
    0x1.fffffcp-127,
    -0x1.p-126
  },
  { // Entry 142
    -0x1.fffffcp-127,
    0x1.fffffcp-127,
    -0x1.p0
  },
  { // Entry 143
    -0x1.fffffcp-127,
    0x1.fffffcp-127,
    -0x1.fffffep127
  },
  { // Entry 144
    -0x1.fffffcp-127,
    0x1.fffffcp-127,
    -HUGE_VALF
  },
  { // Entry 145
    0x1.p-149,
    0x1.p-149,
    HUGE_VALF
  },
  { // Entry 146
    0x1.p-149,
    0x1.p-149,
    0x1.fffffep127
  },
  { // Entry 147
    0x1.p-149,
    0x1.p-149,
    0x1.p-126
  },
  { // Entry 148
    0x1.p-149,
    0x1.p-149,
    0x1.fffffcp-127
  },
  { // Entry 149
    0x1.p-149,
    0x1.p-149,
    0x1.p-149
  },
  { // Entry 150
    0x1.p-149,
    0x1.p-149,
    0.0f
  },
  { // Entry 151
    -0x1.p-149,
    0x1.p-149,
    -0.0f
  },
  { // Entry 152
    -0x1.p-149,
    0x1.p-149,
    -0x1.p-149
  },
  { // Entry 153
    -0x1.p-149,
    0x1.p-149,
    -0x1.fffffcp-127
  },
  { // Entry 154
    -0x1.p-149,
    0x1.p-149,
    -0x1.p-126
  },
  { // Entry 155
    -0x1.p-149,
    0x1.p-149,
    -0x1.p0
  },
  { // Entry 156
    -0x1.p-149,
    0x1.p-149,
    -0x1.fffffep127
  },
  { // Entry 157
    -0x1.p-149,
    0x1.p-149,
    -HUGE_VALF
  },
  { // Entry 158
    0.0,
    0.0f,
    HUGE_VALF
  },
  { // Entry 159
    0.0,
    0.0f,
    0x1.fffffep127
  },
  { // Entry 160
    0.0,
    0.0f,
    0x1.p-126
  },
  { // Entry 161
    0.0,
    0.0f,
    0x1.fffffcp-127
  },
  { // Entry 162
    0.0,
    0.0f,
    0x1.p-149
  },
  { // Entry 163
    0.0,
    0.0f,
    0.0f
  },
  { // Entry 164
    -0.0,
    0.0f,
    -0.0f
  },
  { // Entry 165
    -0.0,
    0.0f,
    -0x1.p-149
  },
  { // Entry 166
    -0.0,
    0.0f,
    -0x1.fffffcp-127
  },
  { // Entry 167
    -0.0,
    0.0f,
    -0x1.p-126
  },
  { // Entry 168
    -0.0,
    0.0f,
    -0x1.p0
  },
  { // Entry 169
    -0.0,
    0.0f,
    -0x1.fffffep127
  },
  { // Entry 170
    -0.0,
    0.0f,
    -HUGE_VALF
  },
  { // Entry 171
    0.0,
    -0.0f,
    HUGE_VALF
  },
  { // Entry 172
    0.0,
    -0.0f,
    0x1.fffffep127
  },
  { // Entry 173
    0.0,
    -0.0f,
    0x1.p-126
  },
  { // Entry 174
    0.0,
    -0.0f,
    0x1.fffffcp-127
  },
  { // Entry 175
    0.0,
    -0.0f,
    0x1.p-149
  },
  { // Entry 176
    0.0,
    -0.0f,
    0.0f
  },
  { // Entry 177
    -0.0,
    -0.0f,
    -0.0f
  },
  { // Entry 178
    -0.0,
    -0.0f,
    -0x1.p-149
  },
  { // Entry 179
    -0.0,
    -0.0f,
    -0x1.fffffcp-127
  },
  { // Entry 180
    -0.0,
    -0.0f,
    -0x1.p-126
  },
  { // Entry 181
    -0.0,
    -0.0f,
    -0x1.p0
  },
  { // Entry 182
    -0.0,
    -0.0f,
    -0x1.fffffep127
  },
  { // Entry 183
    -0.0,
    -0.0f,
    -HUGE_VALF
  },
  { // Entry 184
    0x1.p-149,
    -0x1.p-149,
    HUGE_VALF
  },
  { // Entry 185
    0x1.p-149,
    -0x1.p-149,
    0x1.fffffep127
  },
  { // Entry 186
    0x1.p-149,
    -0x1.p-149,
    0x1.p-126
  },
  { // Entry 187
    0x1.p-149,
    -0x1.p-149,
    0x1.fffffcp-127
  },
  { // Entry 188
    0x1.p-149,
    -0x1.p-149,
    0x1.p-149
  },
  { // Entry 189
    0x1.p-149,
    -0x1.p-149,
    0.0f
  },
  { // Entry 190
    -0x1.p-149,
    -0x1.p-149,
    -0.0f
  },
  { // Entry 191
    -0x1.p-149,
    -0x1.p-149,
    -0x1.p-149
  },
  { // Entry 192
    -0x1.p-149,
    -0x1.p-149,
    -0x1.fffffcp-127
  },
  { // Entry 193
    -0x1.p-149,
    -0x1.p-149,
    -0x1.p-126
  },
  { // Entry 194
    -0x1.p-149,
    -0x1.p-149,
    -0x1.p0
  },
  { // Entry 195
    -0x1.p-149,
    -0x1.p-149,
    -0x1.fffffep127
  },
  { // Entry 196
    -0x1.p-149,
    -0x1.p-149,
    -HUGE_VALF
  },
  { // Entry 197
    0x1.fffffcp-127,
    -0x1.fffffcp-127,
    HUGE_VALF
  },
  { // Entry 198
    0x1.fffffcp-127,
    -0x1.fffffcp-127,
    0x1.fffffep127
  },
  { // Entry 199
    0x1.fffffcp-127,
    -0x1.fffffcp-127,
    0x1.p-126
  },
  { // Entry 200
    0x1.fffffcp-127,
    -0x1.fffffcp-127,
    0x1.fffffcp-127
  },
  { // Entry 201
    0x1.fffffcp-127,
    -0x1.fffffcp-127,
    0x1.p-149
  },
  { // Entry 202
    0x1.fffffcp-127,
    -0x1.fffffcp-127,
    0.0f
  },
  { // Entry 203
    -0x1.fffffcp-127,
    -0x1.fffffcp-127,
    -0.0f
  },
  { // Entry 204
    -0x1.fffffcp-127,
    -0x1.fffffcp-127,
    -0x1.p-149
  },
  { // Entry 205
    -0x1.fffffcp-127,
    -0x1.fffffcp-127,
    -0x1.fffffcp-127
  },
  { // Entry 206
    -0x1.fffffcp-127,
    -0x1.fffffcp-127,
    -0x1.p-126
  },
  { // Entry 207
    -0x1.fffffcp-127,
    -0x1.fffffcp-127,
    -0x1.p0
  },
  { // Entry 208
    -0x1.fffffcp-127,
    -0x1.fffffcp-127,
    -0x1.fffffep127
  },
  { // Entry 209
    -0x1.fffffcp-127,
    -0x1.fffffcp-127,
    -HUGE_VALF
  },
  { // Entry 210
    0x1.p-126,
    -0x1.p-126,
    HUGE_VALF
  },
  { // Entry 211
    0x1.p-126,
    -0x1.p-126,
    0x1.fffffep127
  },
  { // Entry 212
    0x1.p-126,
    -0x1.p-126,
    0x1.p-126
  },
  { // Entry 213
    0x1.p-126,
    -0x1.p-126,
    0x1.fffffcp-127
  },
  { // Entry 214
    0x1.p-126,
    -0x1.p-126,
    0x1.p-149
  },
  { // Entry 215
    0x1.p-126,
    -0x1.p-126,
    0.0f
  },
  { // Entry 216
    -0x1.p-126,
    -0x1.p-126,
    -0.0f
  },
  { // Entry 217
    -0x1.p-126,
    -0x1.p-126,
    -0x1.p-149
  },
  { // Entry 218
    -0x1.p-126,
    -0x1.p-126,
    -0x1.fffffcp-127
  },
  { // Entry 219
    -0x1.p-126,
    -0x1.p-126,
    -0x1.p-126
  },
  { // Entry 220
    -0x1.p-126,
    -0x1.p-126,
    -0x1.p0
  },
  { // Entry 221
    -0x1.p-126,
    -0x1.p-126,
    -0x1.fffffep127
  },
  { // Entry 222
    -0x1.p-126,
    -0x1.p-126,
    -HUGE_VALF
  },
  { // Entry 223
    0x1.fffffep-1,
    -0x1.fffffep-1,
    HUGE_VALF
  },
  { // Entry 224
    0x1.fffffep-1,
    -0x1.fffffep-1,
    0x1.fffffep127
  },
  { // Entry 225
    0x1.fffffep-1,
    -0x1.fffffep-1,
    0x1.p-126
  },
  { // Entry 226
    0x1.fffffep-1,
    -0x1.fffffep-1,
    0x1.fffffcp-127
  },
  { // Entry 227
    0x1.fffffep-1,
    -0x1.fffffep-1,
    0x1.p-149
  },
  { // Entry 228
    0x1.fffffep-1,
    -0x1.fffffep-1,
    0.0f
  },
  { // Entry 229
    -0x1.fffffep-1,
    -0x1.fffffep-1,
    -0.0f
  },
  { // Entry 230
    -0x1.fffffep-1,
    -0x1.fffffep-1,
    -0x1.p-149
  },
  { // Entry 231
    -0x1.fffffep-1,
    -0x1.fffffep-1,
    -0x1.fffffcp-127
  },
  { // Entry 232
    -0x1.fffffep-1,
    -0x1.fffffep-1,
    -0x1.p-126
  },
  { // Entry 233
    -0x1.fffffep-1,
    -0x1.fffffep-1,
    -0x1.p0
  },
  { // Entry 234
    -0x1.fffffep-1,
    -0x1.fffffep-1,
    -0x1.fffffep127
  },
  { // Entry 235
    -0x1.fffffep-1,
    -0x1.fffffep-1,
    -HUGE_VALF
  },
  { // Entry 236
    0x1.p0,
    -0x1.p0,
    HUGE_VALF
  },
  { // Entry 237
    0x1.p0,
    -0x1.p0,
    0x1.fffffep127
  },
  { // Entry 238
    0x1.p0,
    -0x1.p0,
    0x1.p-126
  },
  { // Entry 239
    0x1.p0,
    -0x1.p0,
    0x1.fffffcp-127
  },
  { // Entry 240
    0x1.p0,
    -0x1.p0,
    0x1.p-149
  },
  { // Entry 241
    0x1.p0,
    -0x1.p0,
    0.0f
  },
  { // Entry 242
    -0x1.p0,
    -0x1.p0,
    -0.0f
  },
  { // Entry 243
    -0x1.p0,
    -0x1.p0,
    -0x1.p-149
  },
  { // Entry 244
    -0x1.p0,
    -0x1.p0,
    -0x1.fffffcp-127
  },
  { // Entry 245
    -0x1.p0,
    -0x1.p0,
    -0x1.p-126
  },
  { // Entry 246
    -0x1.p0,
    -0x1.p0,
    -0x1.p0
  },
  { // Entry 247
    -0x1.p0,
    -0x1.p0,
    -0x1.fffffep127
  },
  { // Entry 248
    -0x1.p0,
    -0x1.p0,
    -HUGE_VALF
  },
  { // Entry 249
    0x1.000002p0,
    -0x1.000002p0,
    HUGE_VALF
  },
  { // Entry 250
    0x1.000002p0,
    -0x1.000002p0,
    0x1.fffffep127
  },
  { // Entry 251
    0x1.000002p0,
    -0x1.000002p0,
    0x1.p-126
  },
  { // Entry 252
    0x1.000002p0,
    -0x1.000002p0,
    0x1.fffffcp-127
  },
  { // Entry 253
    0x1.000002p0,
    -0x1.000002p0,
    0x1.p-149
  },
  { // Entry 254
    0x1.000002p0,
    -0x1.000002p0,
    0.0f
  },
  { // Entry 255
    -0x1.000002p0,
    -0x1.000002p0,
    -0.0f
  },
  { // Entry 256
    -0x1.000002p0,
    -0x1.000002p0,
    -0x1.p-149
  },
  { // Entry 257
    -0x1.000002p0,
    -0x1.000002p0,
    -0x1.fffffcp-127
  },
  { // Entry 258
    -0x1.000002p0,
    -0x1.000002p0,
    -0x1.p-126
  },
  { // Entry 259
    -0x1.000002p0,
    -0x1.000002p0,
    -0x1.p0
  },
  { // Entry 260
    -0x1.000002p0,
    -0x1.000002p0,
    -0x1.fffffep127
  },
  { // Entry 261
    -0x1.000002p0,
    -0x1.000002p0,
    -HUGE_VALF
  },
  { // Entry 262
    0x1.fffffep127,
    -0x1.fffffep127,
    HUGE_VALF
  },
  { // Entry 263
    0x1.fffffep127,
    -0x1.fffffep127,
    0x1.fffffep127
  },
  { // Entry 264
    0x1.fffffep127,
    -0x1.fffffep127,
    0x1.p-126
  },
  { // Entry 265
    0x1.fffffep127,
    -0x1.fffffep127,
    0x1.fffffcp-127
  },
  { // Entry 266
    0x1.fffffep127,
    -0x1.fffffep127,
    0x1.p-149
  },
  { // Entry 267
    0x1.fffffep127,
    -0x1.fffffep127,
    0.0f
  },
  { // Entry 268
    -0x1.fffffep127,
    -0x1.fffffep127,
    -0.0f
  },
  { // Entry 269
    -0x1.fffffep127,
    -0x1.fffffep127,
    -0x1.p-149
  },
  { // Entry 270
    -0x1.fffffep127,
    -0x1.fffffep127,
    -0x1.fffffcp-127
  },
  { // Entry 271
    -0x1.fffffep127,
    -0x1.fffffep127,
    -0x1.p-126
  },
  { // Entry 272
    -0x1.fffffep127,
    -0x1.fffffep127,
    -0x1.p0
  },
  { // Entry 273
    -0x1.fffffep127,
    -0x1.fffffep127,
    -0x1.fffffep127
  },
  { // Entry 274
    -0x1.fffffep127,
    -0x1.fffffep127,
    -HUGE_VALF
  },
  { // Entry 275
    HUGE_VALF,
    -HUGE_VALF,
    HUGE_VALF
  },
  { // Entry 276
    HUGE_VALF,
    -HUGE_VALF,
    0x1.fffffep127
  },
  { // Entry 277
    HUGE_VALF,
    -HUGE_VALF,
    0x1.p-126
  },
  { // Entry 278
    HUGE_VALF,
    -HUGE_VALF,
    0x1.fffffcp-127
  },
  { // Entry 279
    HUGE_VALF,
    -HUGE_VALF,
    0x1.p-149
  },
  { // Entry 280
    HUGE_VALF,
    -HUGE_VALF,
    0.0f
  },
  { // Entry 281
    -HUGE_VALF,
    -HUGE_VALF,
    -0.0f
  },
  { // Entry 282
    -HUGE_VALF,
    -HUGE_VALF,
    -0x1.p-149
  },
  { // Entry 283
    -HUGE_VALF,
    -HUGE_VALF,
    -0x1.fffffcp-127
  },
  { // Entry 284
    -HUGE_VALF,
    -HUGE_VALF,
    -0x1.p-126
  },
  { // Entry 285
    -HUGE_VALF,
    -HUGE_VALF,
    -0x1.p0
  },
  { // Entry 286
    -HUGE_VALF,
    -HUGE_VALF,
    -0x1.fffffep127
  },
  { // Entry 287
    -HUGE_VALF,
    -HUGE_VALF,
    -HUGE_VALF
  }
};
