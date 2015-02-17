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

static data_int_1_t<float> g_ilogbf_intel_data[] = {
  { // Entry 0
    (int)0x1.90p6,
    0x1.p100
  },
  { // Entry 1
    (int)0x1.90p6,
    0x1.19999ap100
  },
  { // Entry 2
    (int)0x1.90p6,
    0x1.333334p100
  },
  { // Entry 3
    (int)0x1.90p6,
    0x1.4ccccep100
  },
  { // Entry 4
    (int)0x1.90p6,
    0x1.666668p100
  },
  { // Entry 5
    (int)0x1.90p6,
    0x1.800002p100
  },
  { // Entry 6
    (int)0x1.90p6,
    0x1.99999cp100
  },
  { // Entry 7
    (int)0x1.90p6,
    0x1.b33336p100
  },
  { // Entry 8
    (int)0x1.90p6,
    0x1.ccccd0p100
  },
  { // Entry 9
    (int)0x1.90p6,
    0x1.e6666ap100
  },
  { // Entry 10
    (int)0x1.94p6,
    0x1.p101
  },
  { // Entry 11
    (int)0x1.94p6,
    -0x1.p101
  },
  { // Entry 12
    (int)0x1.90p6,
    -0x1.e66666p100
  },
  { // Entry 13
    (int)0x1.90p6,
    -0x1.ccccccp100
  },
  { // Entry 14
    (int)0x1.90p6,
    -0x1.b33332p100
  },
  { // Entry 15
    (int)0x1.90p6,
    -0x1.999998p100
  },
  { // Entry 16
    (int)0x1.90p6,
    -0x1.7ffffep100
  },
  { // Entry 17
    (int)0x1.90p6,
    -0x1.666664p100
  },
  { // Entry 18
    (int)0x1.90p6,
    -0x1.4ccccap100
  },
  { // Entry 19
    (int)0x1.90p6,
    -0x1.333330p100
  },
  { // Entry 20
    (int)0x1.90p6,
    -0x1.199996p100
  },
  { // Entry 21
    (int)0x1.90p6,
    -0x1.p100
  },
  { // Entry 22
    (int)0x1.50p4,
    0x1.p21
  },
  { // Entry 23
    (int)0x1.50p4,
    0x1.19999ap21
  },
  { // Entry 24
    (int)0x1.50p4,
    0x1.333334p21
  },
  { // Entry 25
    (int)0x1.50p4,
    0x1.4ccccep21
  },
  { // Entry 26
    (int)0x1.50p4,
    0x1.666668p21
  },
  { // Entry 27
    (int)0x1.50p4,
    0x1.800002p21
  },
  { // Entry 28
    (int)0x1.50p4,
    0x1.99999cp21
  },
  { // Entry 29
    (int)0x1.50p4,
    0x1.b33336p21
  },
  { // Entry 30
    (int)0x1.50p4,
    0x1.ccccd0p21
  },
  { // Entry 31
    (int)0x1.50p4,
    0x1.e6666ap21
  },
  { // Entry 32
    (int)0x1.60p4,
    0x1.p22
  },
  { // Entry 33
    (int)0x1.60p4,
    0x1.p22
  },
  { // Entry 34
    (int)0x1.60p4,
    0x1.19999ap22
  },
  { // Entry 35
    (int)0x1.60p4,
    0x1.333334p22
  },
  { // Entry 36
    (int)0x1.60p4,
    0x1.4ccccep22
  },
  { // Entry 37
    (int)0x1.60p4,
    0x1.666668p22
  },
  { // Entry 38
    (int)0x1.60p4,
    0x1.800002p22
  },
  { // Entry 39
    (int)0x1.60p4,
    0x1.99999cp22
  },
  { // Entry 40
    (int)0x1.60p4,
    0x1.b33336p22
  },
  { // Entry 41
    (int)0x1.60p4,
    0x1.ccccd0p22
  },
  { // Entry 42
    (int)0x1.60p4,
    0x1.e6666ap22
  },
  { // Entry 43
    (int)0x1.70p4,
    0x1.p23
  },
  { // Entry 44
    (int)0x1.70p4,
    0x1.p23
  },
  { // Entry 45
    (int)0x1.70p4,
    0x1.19999ap23
  },
  { // Entry 46
    (int)0x1.70p4,
    0x1.333334p23
  },
  { // Entry 47
    (int)0x1.70p4,
    0x1.4ccccep23
  },
  { // Entry 48
    (int)0x1.70p4,
    0x1.666668p23
  },
  { // Entry 49
    (int)0x1.70p4,
    0x1.800002p23
  },
  { // Entry 50
    (int)0x1.70p4,
    0x1.99999cp23
  },
  { // Entry 51
    (int)0x1.70p4,
    0x1.b33336p23
  },
  { // Entry 52
    (int)0x1.70p4,
    0x1.ccccd0p23
  },
  { // Entry 53
    (int)0x1.70p4,
    0x1.e6666ap23
  },
  { // Entry 54
    (int)0x1.80p4,
    0x1.p24
  },
  { // Entry 55
    (int)0x1.80p4,
    0x1.p24
  },
  { // Entry 56
    (int)0x1.80p4,
    0x1.19999ap24
  },
  { // Entry 57
    (int)0x1.80p4,
    0x1.333334p24
  },
  { // Entry 58
    (int)0x1.80p4,
    0x1.4ccccep24
  },
  { // Entry 59
    (int)0x1.80p4,
    0x1.666668p24
  },
  { // Entry 60
    (int)0x1.80p4,
    0x1.800002p24
  },
  { // Entry 61
    (int)0x1.80p4,
    0x1.99999cp24
  },
  { // Entry 62
    (int)0x1.80p4,
    0x1.b33336p24
  },
  { // Entry 63
    (int)0x1.80p4,
    0x1.ccccd0p24
  },
  { // Entry 64
    (int)0x1.80p4,
    0x1.e6666ap24
  },
  { // Entry 65
    (int)0x1.90p4,
    0x1.p25
  },
  { // Entry 66
    (int)-0x1.04p7,
    0x1.p-130
  },
  { // Entry 67
    (int)-0x1.p7,
    0x1.d33330p-128
  },
  { // Entry 68
    (int)-0x1.fcp6,
    0x1.b33330p-127
  },
  { // Entry 69
    (int)-0x1.f8p6,
    0x1.3e6664p-126
  },
  { // Entry 70
    (int)-0x1.f8p6,
    0x1.a33330p-126
  },
  { // Entry 71
    (int)-0x1.f4p6,
    0x1.03fffep-125
  },
  { // Entry 72
    (int)-0x1.f4p6,
    0x1.366664p-125
  },
  { // Entry 73
    (int)-0x1.f4p6,
    0x1.68cccap-125
  },
  { // Entry 74
    (int)-0x1.f4p6,
    0x1.9b3330p-125
  },
  { // Entry 75
    (int)-0x1.f4p6,
    0x1.cd9996p-125
  },
  { // Entry 76
    (int)-0x1.f4p6,
    0x1.fffffcp-125
  },
  { // Entry 77
    (int)0x1.50p4,
    0x1.fffffep21
  },
  { // Entry 78
    (int)0x1.60p4,
    0x1.p22
  },
  { // Entry 79
    (int)0x1.60p4,
    0x1.000002p22
  },
  { // Entry 80
    (int)0x1.60p4,
    0x1.fffffep22
  },
  { // Entry 81
    (int)0x1.70p4,
    0x1.p23
  },
  { // Entry 82
    (int)0x1.70p4,
    0x1.000002p23
  },
  { // Entry 83
    (int)0x1.70p4,
    0x1.fffffep23
  },
  { // Entry 84
    (int)0x1.80p4,
    0x1.p24
  },
  { // Entry 85
    (int)0x1.80p4,
    0x1.000002p24
  },
  { // Entry 86
    (int)0x1.60p4,
    -0x1.000002p22
  },
  { // Entry 87
    (int)0x1.60p4,
    -0x1.p22
  },
  { // Entry 88
    (int)0x1.50p4,
    -0x1.fffffep21
  },
  { // Entry 89
    (int)0x1.70p4,
    -0x1.000002p23
  },
  { // Entry 90
    (int)0x1.70p4,
    -0x1.p23
  },
  { // Entry 91
    (int)0x1.60p4,
    -0x1.fffffep22
  },
  { // Entry 92
    (int)0x1.80p4,
    -0x1.000002p24
  },
  { // Entry 93
    (int)0x1.80p4,
    -0x1.p24
  },
  { // Entry 94
    (int)0x1.70p4,
    -0x1.fffffep23
  },
  { // Entry 95
    (int)0x1.fcp6,
    0x1.fffffep127
  },
  { // Entry 96
    (int)0x1.fcp6,
    -0x1.fffffep127
  },
  { // Entry 97
    (int)-0x1.c0p2,
    0x1.fffffep-7
  },
  { // Entry 98
    (int)-0x1.80p2,
    0x1.p-6
  },
  { // Entry 99
    (int)-0x1.80p2,
    0x1.000002p-6
  },
  { // Entry 100
    (int)-0x1.80p2,
    0x1.fffffep-6
  },
  { // Entry 101
    (int)-0x1.40p2,
    0x1.p-5
  },
  { // Entry 102
    (int)-0x1.40p2,
    0x1.000002p-5
  },
  { // Entry 103
    (int)-0x1.40p2,
    0x1.fffffep-5
  },
  { // Entry 104
    (int)-0x1.p2,
    0x1.p-4
  },
  { // Entry 105
    (int)-0x1.p2,
    0x1.000002p-4
  },
  { // Entry 106
    (int)-0x1.p2,
    0x1.fffffep-4
  },
  { // Entry 107
    (int)-0x1.80p1,
    0x1.p-3
  },
  { // Entry 108
    (int)-0x1.80p1,
    0x1.000002p-3
  },
  { // Entry 109
    (int)-0x1.80p1,
    0x1.fffffep-3
  },
  { // Entry 110
    (int)-0x1.p1,
    0x1.p-2
  },
  { // Entry 111
    (int)-0x1.p1,
    0x1.000002p-2
  },
  { // Entry 112
    (int)-0x1.p1,
    0x1.fffffep-2
  },
  { // Entry 113
    (int)-0x1.p0,
    0x1.p-1
  },
  { // Entry 114
    (int)-0x1.p0,
    0x1.000002p-1
  },
  { // Entry 115
    (int)-0x1.2ap7,
    -0x1.p-149
  },
  { // Entry 116
    (int)-0x1.fffffffcp30,
    0.0
  },
  { // Entry 117
    (int)-0x1.2ap7,
    0x1.p-149
  },
  { // Entry 118
    (int)-0x1.p0,
    0x1.fffffep-1
  },
  { // Entry 119
    (int)0.0,
    0x1.p0
  },
  { // Entry 120
    (int)0.0,
    0x1.000002p0
  },
  { // Entry 121
    (int)0.0,
    0x1.fffffep0
  },
  { // Entry 122
    (int)0x1.p0,
    0x1.p1
  },
  { // Entry 123
    (int)0x1.p0,
    0x1.000002p1
  },
  { // Entry 124
    (int)0x1.p0,
    0x1.fffffep1
  },
  { // Entry 125
    (int)0x1.p1,
    0x1.p2
  },
  { // Entry 126
    (int)0x1.p1,
    0x1.000002p2
  },
  { // Entry 127
    (int)0x1.p1,
    0x1.fffffep2
  },
  { // Entry 128
    (int)0x1.80p1,
    0x1.p3
  },
  { // Entry 129
    (int)0x1.80p1,
    0x1.000002p3
  },
  { // Entry 130
    (int)0x1.80p1,
    0x1.fffffep3
  },
  { // Entry 131
    (int)0x1.p2,
    0x1.p4
  },
  { // Entry 132
    (int)0x1.p2,
    0x1.000002p4
  },
  { // Entry 133
    (int)0x1.p2,
    0x1.fffffep4
  },
  { // Entry 134
    (int)0x1.40p2,
    0x1.p5
  },
  { // Entry 135
    (int)0x1.40p2,
    0x1.000002p5
  },
  { // Entry 136
    (int)0x1.40p2,
    0x1.fffffep5
  },
  { // Entry 137
    (int)0x1.80p2,
    0x1.p6
  },
  { // Entry 138
    (int)0x1.80p2,
    0x1.000002p6
  },
  { // Entry 139
    (int)0x1.80p2,
    0x1.fffffep6
  },
  { // Entry 140
    (int)0x1.c0p2,
    0x1.p7
  },
  { // Entry 141
    (int)0x1.c0p2,
    0x1.000002p7
  },
  { // Entry 142
    (int)0x1.fffffffcp30,
    HUGE_VALF
  },
  { // Entry 143
    (int)0x1.fffffffcp30,
    -HUGE_VALF
  },
  { // Entry 144
    (int)-0x1.fffffffcp30,
    0.0f
  },
  { // Entry 145
    (int)-0x1.fffffffcp30,
    -0.0f
  },
  { // Entry 146
    (int)0x1.fcp6,
    0x1.fffffep127
  },
  { // Entry 147
    (int)0x1.fcp6,
    -0x1.fffffep127
  },
  { // Entry 148
    (int)0x1.fcp6,
    0x1.fffffcp127
  },
  { // Entry 149
    (int)0x1.fcp6,
    -0x1.fffffcp127
  },
  { // Entry 150
    (int)0x1.p0,
    0x1.921fb6p1
  },
  { // Entry 151
    (int)0x1.p0,
    -0x1.921fb6p1
  },
  { // Entry 152
    (int)0.0,
    0x1.921fb6p0
  },
  { // Entry 153
    (int)0.0,
    -0x1.921fb6p0
  },
  { // Entry 154
    (int)0.0,
    0x1.000002p0
  },
  { // Entry 155
    (int)0.0,
    -0x1.000002p0
  },
  { // Entry 156
    (int)0.0,
    0x1.p0
  },
  { // Entry 157
    (int)0.0,
    -0x1.p0
  },
  { // Entry 158
    (int)-0x1.p0,
    0x1.fffffep-1
  },
  { // Entry 159
    (int)-0x1.p0,
    -0x1.fffffep-1
  },
  { // Entry 160
    (int)-0x1.p0,
    0x1.921fb6p-1
  },
  { // Entry 161
    (int)-0x1.p0,
    -0x1.921fb6p-1
  },
  { // Entry 162
    (int)-0x1.f8p6,
    0x1.000002p-126
  },
  { // Entry 163
    (int)-0x1.f8p6,
    -0x1.000002p-126
  },
  { // Entry 164
    (int)-0x1.f8p6,
    0x1.p-126
  },
  { // Entry 165
    (int)-0x1.f8p6,
    -0x1.p-126
  },
  { // Entry 166
    (int)-0x1.fcp6,
    0x1.fffffcp-127
  },
  { // Entry 167
    (int)-0x1.fcp6,
    -0x1.fffffcp-127
  },
  { // Entry 168
    (int)-0x1.fcp6,
    0x1.fffff8p-127
  },
  { // Entry 169
    (int)-0x1.fcp6,
    -0x1.fffff8p-127
  },
  { // Entry 170
    (int)-0x1.28p7,
    0x1.p-148
  },
  { // Entry 171
    (int)-0x1.28p7,
    -0x1.p-148
  },
  { // Entry 172
    (int)-0x1.2ap7,
    0x1.p-149
  },
  { // Entry 173
    (int)-0x1.2ap7,
    -0x1.p-149
  }
};
