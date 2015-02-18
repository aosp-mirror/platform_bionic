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

static data_1_1_t<float, float> g_sqrtf_intel_data[] = {
  { // Entry 0
    0x1.p-1,
    0x1.p-2
  },
  { // Entry 1
    0x1.p-10,
    0x1.p-20
  },
  { // Entry 2
    0x1.000000ffffff8000007fffff600000dfp-1,
    0x1.000002p-2
  },
  { // Entry 3
    0x1.000000ffffff8000007fffff600000dfp-20,
    0x1.000002p-40
  },
  { // Entry 4
    0x1.000000ffffff8000007fffff600000dfp-63,
    0x1.000002p-126
  },
  { // Entry 5
    0x1.6a09eaa61169a3b76f99ea0d364efd91p-63,
    0x1.000006p-125
  },
  { // Entry 6
    0x1.6a0a0584cc337abc8740253db45a12f6p-3,
    0x1.00002cp-5
  },
  { // Entry 7
    0x1.6a0a1ef97b51a8def43dbb5dc7f37ff0p-11,
    0x1.000050p-21
  },
  { // Entry 8
    0x1.6a0a1ef97b51a8def43dbb5dc7f37ff0p-18,
    0x1.000050p-35
  },
  { // Entry 9
    0x1.6a0a359a157a0b9bd4ff286a2eab2edfp-21,
    0x1.000070p-41
  },
  { // Entry 10
    0x1.6a0a9898a74a019a7cc18157814e39d9p-6,
    0x1.0000fcp-11
  },
  { // Entry 11
    0x1.6a0a9898a74a019a7cc18157814e39d9p-59,
    0x1.0000fcp-117
  },
  { // Entry 12
    0x1.6a0a9898a74a019a7cc18157814e39d9p-61,
    0x1.0000fcp-121
  },
  { // Entry 13
    0x1.6a0a9898a74a019a7cc18157814e39d9p1,
    0x1.0000fcp3
  },
  { // Entry 14
    0x1.6a0ab377552adf71413bdc5fdf2d3e0dp63,
    0x1.000122p127
  },
  { // Entry 15
    0x1.6a0b1d87f00e99851a9dcb6fb9c2d56ep-3,
    0x1.0001b8p-5
  },
  { // Entry 16
    0x1.6a0ba98930c5cb58722f06e15c330f95p-1,
    0x1.00027ep-1
  },
  { // Entry 17
    0x1.6a0bea96a144bf2a9c899380f11039d1p0,
    0x1.0002dap1
  },
  { // Entry 18
    0x1.6a0d3c935fb77764b4cc3f34e117a891p-49,
    0x1.0004b8p-97
  },
  { // Entry 19
    0x1.6a0d6700222327f1053e4a429adb015ep-3,
    0x1.0004f4p-5
  },
  { // Entry 20
    0x1.6a0dc893ce705ac35b85bb49e3aa1badp0,
    0x1.00057ep1
  },
  { // Entry 21
    0x1.001ffe003ff601bfac107ca6b29a0c31p-20,
    0x1.0040p-40
  },
  { // Entry 22
    0x1.6a3724d10762c86a71fe557d13336111p-21,
    0x1.0040p-41
  },
  { // Entry 23
    0x1.6a38a9884557da551f3ec21f785779d7p6,
    0x1.004226p13
  },
  { // Entry 24
    0x1.6abbeb82dbfd8f20f5923ab389eef7f3p-11,
    0x1.00fcp-21
  },
  { // Entry 25
    0x1.0085dd004f71f5362dd6a5e09a9cee74p-10,
    0x1.010cp-20
  },
  { // Entry 26
    0x1.0085dd004f71f5362dd6a5e09a9cee74p-20,
    0x1.010cp-40
  },
  { // Entry 27
    0x1.6cdb707e0273cc8e2a1d99aa3ad67b66p0,
    0x1.040062p1
  },
  { // Entry 28
    0x1.6e14fb33af5d199451a44c592d18f9e1p-11,
    0x1.05c0p-21
  },
  { // Entry 29
    0x1.6e3eedfff2f9d88cc837d36b17bce548p-11,
    0x1.05fcp-21
  },
  { // Entry 30
    0x1.708713bb31c17627489983a6397ff529p-1,
    0x1.094250p-1
  },
  { // Entry 31
    0x1.7214c125cb8b2284459daa230a47b9dfp-11,
    0x1.0b80p-21
  },
  { // Entry 32
    0x1.7528ce694c692ce6ecf340f96e7ac410p-9,
    0x1.0ff7f8p-17
  },
  { // Entry 33
    0x1.752b9263582a2e3ab2a36ee98ed34a19p-11,
    0x1.0ffcp-21
  },
  { // Entry 34
    0x1.752b9263582a2e3ab2a36ee98ed34a19p-49,
    0x1.0ffcp-97
  },
  { // Entry 35
    0x1.752b9263582a2e3ab2a36ee98ed34a19p-63,
    0x1.0ffcp-125
  },
  { // Entry 36
    0x1.752b9263582a2e3ab2a36ee98ed34a19p-68,
    0x1.0ffcp-135
  },
  { // Entry 37
    0x1.752bcd629c83e4378e77e8fb09e0dfb8p0,
    0x1.0ffc56p1
  },
  { // Entry 38
    0x1.752cdbac2ec6fddb69cc2e00bdf1fd02p4,
    0x1.0ffde0p9
  },
  { // Entry 39
    0x1.752cf45e665c37b7d77c8c9fb50cd678p-4,
    0x1.0ffe04p-7
  },
  { // Entry 40
    0x1.752cf45e665c37b7d77c8c9fb50cd678p-63,
    0x1.0ffe04p-125
  },
  { // Entry 41
    0x1.752cf45e665c37b7d77c8c9fb50cd678p1,
    0x1.0ffe04p3
  },
  { // Entry 42
    0x1.752d6a5c75cf3b16e5de9a228f3ef4dap1,
    0x1.0ffeb0p3
  },
  { // Entry 43
    0x1.752d6a5c75cf3b16e5de9a228f3ef4dap28,
    0x1.0ffeb0p57
  },
  { // Entry 44
    0x1.752e105f70189628b0d2d6e17ca2b9c5p1,
    0x1.0fffa2p3
  },
  { // Entry 45
    0x1.752e4b5e4f95f6a45bcfd8721ccbf950p-4,
    0x1.0ffff8p-7
  },
  { // Entry 46
    0x1.752e4b5e4f95f6a45bcfd8721ccbf950p-53,
    0x1.0ffff8p-105
  },
  { // Entry 47
    0x1.752e4b5e4f95f6a45bcfd8721ccbf950p-61,
    0x1.0ffff8p-121
  },
  { // Entry 48
    0x1.752e4b5e4f95f6a45bcfd8721ccbf950p-62,
    0x1.0ffff8p-123
  },
  { // Entry 49
    0x1.752e4b5e4f95f6a45bcfd8721ccbf950p1,
    0x1.0ffff8p3
  },
  { // Entry 50
    0x1.784220501c1fc95d7e2043339d354588p-1,
    0x1.148128p-1
  },
  { // Entry 51
    0x1.78cc0a00054d7683165ea2815b6b8d14p-11,
    0x1.154cp-21
  },
  { // Entry 52
    0x1.7ea8336e0f268f6d05f0934a67b4840ap-32,
    0x1.1dfd34p-63
  },
  { // Entry 53
    0x1.0ec1270014d42e5424a7780b67b4974cp-10,
    0x1.1e5cp-20
  },
  { // Entry 54
    0x1.0f7a3974c0a036ead45a0017d4782e1ap-10,
    0x1.1fe4p-20
  },
  { // Entry 55
    0x1.10207f000077f1ab909fb7b5314f837fp-1,
    0x1.214512p-2
  },
  { // Entry 56
    0x1.8c4487000042dd2813dd730d1a7f3cabp1,
    0x1.32b20ap3
  },
  { // Entry 57
    0x1.8dc41537f0b639cb37aedfa8d531d4cfp-51,
    0x1.3504e0p-101
  },
  { // Entry 58
    0x1.8fae0c15ad389e24852497e80935e4b5p-63,
    0x1.38p-125
  },
  { // Entry 59
    0x1.95b8c0ffff868994f9ce14b11aa1cfd5p-2,
    0x1.41810cp-3
  },
  { // Entry 60
    0x1.a644fe00060ec83a5393c3c3cd39fc95p-1,
    0x1.5c43c4p-1
  },
  { // Entry 61
    0x1.b7070406a4527543e7e1a93c34a2d6f7p-1,
    0x1.787488p-1
  },
  { // Entry 62
    0x1.3836b2ffff863cbb58a0fb2bcad85474p-1,
    0x1.7cc560p-2
  },
  { // Entry 63
    0x1.bb67ad5dea55ebaf21faa9bb10eabdf1p-21,
    0x1.7ffffep-41
  },
  { // Entry 64
    0x1.ce7e96000007bfd0ed97130360c298ddp-1,
    0x1.a1c692p-1
  },
  { // Entry 65
    0x1.d10d73f80594d3d73fae1f2d6a130247p0,
    0x1.a668f0p1
  },
  { // Entry 66
    0x1.d94c090afeb02b92e741bb236186bc86p-1,
    0x1.b58508p-1
  },
  { // Entry 67
    0x1.de4315000013008bf12e454d672ffbdfp-2,
    0x1.bebf4ap-3
  },
  { // Entry 68
    0x1.e768d28cf1e3981570e875af113546c7p-21,
    0x1.cffffep-41
  },
  { // Entry 69
    0x1.e7f0cc5a77a783be336f9b5127709acep-1,
    0x1.d10306p-1
  },
  { // Entry 70
    0x1.f3169029c9867f10142750dc51d5843dp-1,
    0x1.e6807cp-1
  },
  { // Entry 71
    0x1.f6eb62d27730caff89a8d78c7314934ap-60,
    0x1.eep-119
  },
  { // Entry 72
    0x1.f8f3935d2ecc7256a14d7c8816275c60p-21,
    0x1.f1fffep-41
  },
  { // Entry 73
    0x1.f94a8e8524ece8623432ace75571785ep-1,
    0x1.f2ab9ep-1
  },
  { // Entry 74
    0x1.faa0p-1,
    0x1.f54e72p-1
  },
  { // Entry 75
    0x1.feefb698fc02e71c2377deb225837a87p-63,
    0x1.fddffep-125
  },
  { // Entry 76
    0x1.fffffcfffffdbffffc9ffff9abfff2b5p-1,
    0x1.fffffap-1
  },
  { // Entry 77
    0x1.fffffdfffffefffffefffffebffffe3fp-4,
    0x1.fffffcp-7
  },
  { // Entry 78
    0x1.fffffdfffffefffffefffffebffffe3fp-21,
    0x1.fffffcp-41
  },
  { // Entry 79
    0x1.fffffdfffffefffffefffffebffffe3fp-63,
    0x1.fffffcp-125
  },
  { // Entry 80
    0x1.fffffeffffffbfffffdfffffebfffff1p-4,
    0x1.fffffep-7
  },
  { // Entry 81
    0x1.fffffeffffffbfffffdfffffebfffff1p0,
    0x1.fffffep1
  },
  { // Entry 82
    0x1.306fdec8dc9ad32b551e92585b7094f3p-1,
    0x1.6a09e2p-2
  },
  { // Entry 83
    0x1.306fdfa02198a13d946ad818ec615547p-1,
    0x1.6a09e4p-2
  },
  { // Entry 84
    0x1.306fe0776695d717e1e11958d56541cfp-1,
    0x1.6a09e6p-2
  },
  { // Entry 85
    0x1.306fe14eab9274ba3ec43d966959f47cp-1,
    0x1.6a09e8p-2
  },
  { // Entry 86
    0x1.306fe225f08e7a24ac572c4b8579785fp-1,
    0x1.6a09eap-2
  },
  { // Entry 87
    0x1.ae89f6f6fe087ac302131f3840da7a90p-1,
    0x1.6a09e2p-1
  },
  { // Entry 88
    0x1.ae89f8276dea8c7accb82339973f2af9p-1,
    0x1.6a09e4p-1
  },
  { // Entry 89
    0x1.ae89f957ddcbc6ed986cf1a0e754d170p-1,
    0x1.6a09e6p-1
  },
  { // Entry 90
    0x1.ae89fa884dac2a1b66fa324394d3c590p-1,
    0x1.6a09e8p-1
  },
  { // Entry 91
    0x1.ae89fbb8bd8bb6043a288cf0b4eef0aep-1,
    0x1.6a09eap-1
  },
  { // Entry 92
    0x1.306fdec8dc9ad32b551e92585b7094f3p0,
    0x1.6a09e2p0
  },
  { // Entry 93
    0x1.306fdfa02198a13d946ad818ec615547p0,
    0x1.6a09e4p0
  },
  { // Entry 94
    0x1.306fe0776695d717e1e11958d56541cfp0,
    0x1.6a09e6p0
  },
  { // Entry 95
    0x1.306fe14eab9274ba3ec43d966959f47cp0,
    0x1.6a09e8p0
  },
  { // Entry 96
    0x1.306fe225f08e7a24ac572c4b8579785fp0,
    0x1.6a09eap0
  },
  { // Entry 97
    0x1.ae89f6f6fe087ac302131f3840da7a90p0,
    0x1.6a09e2p1
  },
  { // Entry 98
    0x1.ae89f8276dea8c7accb82339973f2af9p0,
    0x1.6a09e4p1
  },
  { // Entry 99
    0x1.ae89f957ddcbc6ed986cf1a0e754d170p0,
    0x1.6a09e6p1
  },
  { // Entry 100
    0x1.ae89fa884dac2a1b66fa324394d3c590p0,
    0x1.6a09e8p1
  },
  { // Entry 101
    0x1.ae89fbb8bd8bb6043a288cf0b4eef0aep0,
    0x1.6a09eap1
  },
  { // Entry 102
    0x1.fffffeffffffbfffffdfffffebfffff1p-4,
    0x1.fffffep-7
  },
  { // Entry 103
    0x1.p-3,
    0x1.p-6
  },
  { // Entry 104
    0x1.000000ffffff8000007fffff600000dfp-3,
    0x1.000002p-6
  },
  { // Entry 105
    0x1.6a09e5b2eec967cd97b2eff75f471493p-3,
    0x1.fffffep-6
  },
  { // Entry 106
    0x1.6a09e667f3bcc908b2fb1366ea957d3ep-3,
    0x1.p-5
  },
  { // Entry 107
    0x1.6a09e7d1fda27bf77d45272dd2d83a4bp-3,
    0x1.000002p-5
  },
  { // Entry 108
    0x1.fffffeffffffbfffffdfffffebfffff1p-3,
    0x1.fffffep-5
  },
  { // Entry 109
    0x1.p-2,
    0x1.p-4
  },
  { // Entry 110
    0x1.000000ffffff8000007fffff600000dfp-2,
    0x1.000002p-4
  },
  { // Entry 111
    0x1.6a09e5b2eec967cd97b2eff75f471493p-2,
    0x1.fffffep-4
  },
  { // Entry 112
    0x1.6a09e667f3bcc908b2fb1366ea957d3ep-2,
    0x1.p-3
  },
  { // Entry 113
    0x1.6a09e7d1fda27bf77d45272dd2d83a4bp-2,
    0x1.000002p-3
  },
  { // Entry 114
    0x1.fffffeffffffbfffffdfffffebfffff1p-2,
    0x1.fffffep-3
  },
  { // Entry 115
    0x1.p-1,
    0x1.p-2
  },
  { // Entry 116
    0x1.000000ffffff8000007fffff600000dfp-1,
    0x1.000002p-2
  },
  { // Entry 117
    0x1.6a09e5b2eec967cd97b2eff75f471493p-1,
    0x1.fffffep-2
  },
  { // Entry 118
    0x1.6a09e667f3bcc908b2fb1366ea957d3ep-1,
    0x1.p-1
  },
  { // Entry 119
    0x1.6a09e7d1fda27bf77d45272dd2d83a4bp-1,
    0x1.000002p-1
  },
  { // Entry 120
    0x1.fffffeffffffbfffffdfffffebfffff1p-1,
    0x1.fffffep-1
  },
  { // Entry 121
    0x1.p0,
    0x1.p0
  },
  { // Entry 122
    0x1.000000ffffff8000007fffff600000dfp0,
    0x1.000002p0
  },
  { // Entry 123
    0x1.6a09e5b2eec967cd97b2eff75f471493p0,
    0x1.fffffep0
  },
  { // Entry 124
    0x1.6a09e667f3bcc908b2fb1366ea957d3ep0,
    0x1.p1
  },
  { // Entry 125
    0x1.6a09e7d1fda27bf77d45272dd2d83a4bp0,
    0x1.000002p1
  },
  { // Entry 126
    0x1.fffffeffffffbfffffdfffffebfffff1p0,
    0x1.fffffep1
  },
  { // Entry 127
    0x1.p1,
    0x1.p2
  },
  { // Entry 128
    0x1.000000ffffff8000007fffff600000dfp1,
    0x1.000002p2
  },
  { // Entry 129
    0.0,
    0.0
  },
  { // Entry 130
    0x1.6a09e667f3bcc908b2fb1366ea957d3ep-75,
    0x1.p-149
  },
  { // Entry 131
    0x1.fffffdfffffefffffefffffebffffe3fp-64,
    0x1.fffffcp-127
  },
  { // Entry 132
    0x1.p-63,
    0x1.p-126
  },
  { // Entry 133
    0x1.000000ffffff8000007fffff600000dfp-63,
    0x1.000002p-126
  },
  { // Entry 134
    0x1.6a09e667f3bcc908b2fb1366ea957d3ep-75,
    0x1.p-149
  },
  { // Entry 135
    0x1.279a75809da58a0811243c04849bccb2p-64,
    0x1.555558p-128
  },
  { // Entry 136
    0x1.a20bd62fbcd82b1d65e201b6160bb97fp-64,
    0x1.555554p-127
  },
  { // Entry 137
    0x1.fffffdfffffefffffefffffebffffe3fp-64,
    0x1.fffffcp-127
  },
  { // Entry 138
    0x1.fffffcfffffdbffffc9ffff9abfff2b5p-1,
    0x1.fffffap-1
  },
  { // Entry 139
    0x1.fffffdfffffefffffefffffebffffe3fp-1,
    0x1.fffffcp-1
  },
  { // Entry 140
    0x1.fffffeffffffbfffffdfffffebfffff1p-1,
    0x1.fffffep-1
  },
  { // Entry 141
    0x1.p0,
    0x1.p0
  },
  { // Entry 142
    0x1.000000ffffff8000007fffff600000dfp0,
    0x1.000002p0
  },
  { // Entry 143
    0x1.fffffcfffffdbffffc9ffff9abfff2b5p-1,
    0x1.fffffap-1
  },
  { // Entry 144
    0x1.fffffdfffffefffffefffffebffffe3fp-1,
    0x1.fffffcp-1
  },
  { // Entry 145
    0x1.fffffeffffffbfffffdfffffebfffff1p-1,
    0x1.fffffep-1
  },
  { // Entry 146
    0x1.p0,
    0x1.p0
  },
  { // Entry 147
    0x1.000000ffffff8000007fffff600000dfp0,
    0x1.000002p0
  },
  { // Entry 148
    0x1.fffffeffffffbfffffdfffffebfffff1p63,
    0x1.fffffep127
  },
  { // Entry 149
    HUGE_VALF,
    HUGE_VALF
  },
  { // Entry 150
    0x1.fffffeffffffbfffffdfffffebfffff1p63,
    0x1.fffffep127
  },
  { // Entry 151
    0x1.fffffdfffffefffffefffffebffffe3fp63,
    0x1.fffffcp127
  },
  { // Entry 152
    0x1.c5bf89853a94d473c88f0dc85f187a6ep0,
    0x1.921fb6p1
  },
  { // Entry 153
    0x1.40d9324a48137bb45e891e1bdffe64c2p0,
    0x1.921fb6p0
  },
  { // Entry 154
    0x1.000000ffffff8000007fffff600000dfp0,
    0x1.000002p0
  },
  { // Entry 155
    0x1.p0,
    0x1.p0
  },
  { // Entry 156
    0x1.fffffeffffffbfffffdfffffebfffff1p-1,
    0x1.fffffep-1
  },
  { // Entry 157
    0x1.c5bf89853a94d473c88f0dc85f187a6ep-1,
    0x1.921fb6p-1
  },
  { // Entry 158
    0x1.000000ffffff8000007fffff600000dfp-63,
    0x1.000002p-126
  },
  { // Entry 159
    0x1.p-63,
    0x1.p-126
  },
  { // Entry 160
    0x1.fffffdfffffefffffefffffebffffe3fp-64,
    0x1.fffffcp-127
  },
  { // Entry 161
    0x1.fffffbfffffbfffff7ffffebffffc7ffp-64,
    0x1.fffff8p-127
  },
  { // Entry 162
    0x1.p-74,
    0x1.p-148
  },
  { // Entry 163
    0x1.6a09e667f3bcc908b2fb1366ea957d3ep-75,
    0x1.p-149
  },
  { // Entry 164
    0.0,
    0.0f
  },
  { // Entry 165
    -0.0,
    -0.0f
  },
  { // Entry 166
    0x1.p1,
    0x1.p2
  },
  { // Entry 167
    0x1.p2,
    0x1.p4
  },
  { // Entry 168
    0x1.80p1,
    0x1.20p3
  },
  { // Entry 169
    0x1.40p2,
    0x1.90p4
  },
  { // Entry 170
    0x1.p-1,
    0x1.p-2
  },
  { // Entry 171
    0x1.c0p2,
    0x1.88p5
  },
  { // Entry 172
    0x1.40p3,
    0x1.90p6
  }
};
