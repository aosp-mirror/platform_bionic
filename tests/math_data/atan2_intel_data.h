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

static data_1_2_t<double, double, double> g_atan2_intel_data[] = {
  { // Entry 0
    -0x1.ffffffffffff20000000000061fffd55p-60,
    -0x1.0p-100,
    0x1.0000000000007p-41
  },
  { // Entry 1
    0x1.ffffffffffff20000000000061fffd55p-60,
    0x1.0p-100,
    0x1.0000000000007p-41
  },
  { // Entry 2
    -0.0,
    -0x1.0p-1073,
    0x1.0000000000001p1
  },
  { // Entry 3
    0.0,
    0x1.0p-1073,
    0x1.0000000000001p1
  },
  { // Entry 4
    -0x1.cd648010e76317fd25f197c89894a747p-1,
    -0x1.0p10,
    0x1.955555555555ep9
  },
  { // Entry 5
    0x1.cd648010e76317fd25f197c89894a747p-1,
    0x1.0p10,
    0x1.955555555555ep9
  },
  { // Entry 6
    -0x1.0000000000000800000000000040p-924,
    -0x1.0p100,
    0x1.fffffffffffffp1023
  },
  { // Entry 7
    0x1.0000000000000800000000000040p-924,
    0x1.0p100,
    0x1.fffffffffffffp1023
  },
  { // Entry 8
    -0x1.0945ca475762680110c86c82f4007bdap1,
    -0x1.0000000000001p0,
    -0x1.18cd584e6112bp-1
  },
  { // Entry 9
    0x1.0945ca475762680110c86c82f4007bdap1,
    0x1.0000000000001p0,
    -0x1.18cd584e6112bp-1
  },
  { // Entry 10
    -0x1.f9ca0e1dd954324b96732f0ae9c1c8ffp-3,
    -0x1.0222222222222p0,
    0x1.ffeffffffffffp1
  },
  { // Entry 11
    0x1.f9ca0e1dd954324b96732f0ae9c1c8ffp-3,
    0x1.0222222222222p0,
    0x1.ffeffffffffffp1
  },
  { // Entry 12
    -0x1.fff9653e6201f888937cfc2d716b4d44p-2,
    -0x1.14171f06bfb89p-2,
    0x1.f96902dccd29ap-2
  },
  { // Entry 13
    0x1.fff9653e6201f888937cfc2d716b4d44p-2,
    0x1.14171f06bfb89p-2,
    0x1.f96902dccd29ap-2
  },
  { // Entry 14
    -0x1.ff572aded0be932feeb4707dcb65336dp0,
    -0x1.1999999999998p-2,
    -0x1.0000000000001p-3
  },
  { // Entry 15
    0x1.ff572aded0be932feeb4707dcb65336dp0,
    0x1.1999999999998p-2,
    -0x1.0000000000001p-3
  },
  { // Entry 16
    -0x1.ff542758ef05b8e7de0d70e2d341ed67p0,
    -0x1.1999999999999p-1,
    -0x1.ffeffffffffffp-3
  },
  { // Entry 17
    0x1.ff542758ef05b8e7de0d70e2d341ed67p0,
    0x1.1999999999999p-1,
    -0x1.ffeffffffffffp-3
  },
  { // Entry 18
    -0x1.2fffffffffffffffffffffffffffffffp-1071,
    -0x1.3p-1070,
    0x1.0p1
  },
  { // Entry 19
    0x1.2fffffffffffffffffffffffffffffffp-1071,
    0x1.3p-1070,
    0x1.0p1
  },
  { // Entry 20
    -0x1.85539729ef1727fed15784b60b91b2ecp-1,
    -0x1.3cf3cf3cf3cf4p9,
    0x1.4d34d34d34d34p9
  },
  { // Entry 21
    0x1.85539729ef1727fed15784b60b91b2ecp-1,
    0x1.3cf3cf3cf3cf4p9,
    0x1.4d34d34d34d34p9
  },
  { // Entry 22
    -0x1.40000000000008000000000000fffd65p-59,
    -0x1.3fffffffffffep42,
    0x1.ffffffffffffcp100
  },
  { // Entry 23
    0x1.40000000000008000000000000fffd65p-59,
    0x1.3fffffffffffep42,
    0x1.ffffffffffffcp100
  },
  { // Entry 24
    -0x1.921fb54442d18469898cc51701b839a2p1,
    -0x1.4p-1072,
    -0x1.fffffffffffffp1023
  },
  { // Entry 25
    0x1.921fb54442d18469898cc51701b839a2p1,
    0x1.4p-1072,
    -0x1.fffffffffffffp1023
  },
  { // Entry 26
    -0x1.3fffffffffffffffffffffffffffffffp-1073,
    -0x1.4p-1072,
    0x1.0p1
  },
  { // Entry 27
    0x1.3fffffffffffffffffffffffffffffffp-1073,
    0x1.4p-1072,
    0x1.0p1
  },
  { // Entry 28
    -0x1.5ffe7c27a5cf37fef15668ed8bfdc92cp-1,
    -0x1.6477c84a032cep-1,
    0x1.b21f69ae030b0p-1
  },
  { // Entry 29
    0x1.5ffe7c27a5cf37fef15668ed8bfdc92cp-1,
    0x1.6477c84a032cep-1,
    0x1.b21f69ae030b0p-1
  },
  { // Entry 30
    -0x1.66719908f7c3b796d84184977c923894p-12,
    -0x1.6666666665b64p-1,
    0x1.ffeffffffffffp10
  },
  { // Entry 31
    0x1.66719908f7c3b796d84184977c923894p-12,
    0x1.6666666665b64p-1,
    0x1.ffeffffffffffp10
  },
  { // Entry 32
    -0x1.667199f33acd08010011a82e9838500dp-52,
    -0x1.6666666666668p-1,
    0x1.ffeffffff924fp50
  },
  { // Entry 33
    0x1.667199f33acd08010011a82e9838500dp-52,
    0x1.6666666666668p-1,
    0x1.ffeffffff924fp50
  },
  { // Entry 34
    -0x1.48ef86a5d674e7fe626345caa6dea1adp0,
    -0x1.6e589292a58a8p3,
    0x1.aebd9564499f0p1
  },
  { // Entry 35
    0x1.48ef86a5d674e7fe626345caa6dea1adp0,
    0x1.6e589292a58a8p3,
    0x1.aebd9564499f0p1
  },
  { // Entry 36
    -0x1.69412651b663880102057ffe2b6916e7p-2,
    -0x1.7906fe92593dcp-2,
    0x1.0p0
  },
  { // Entry 37
    0x1.69412651b663880102057ffe2b6916e7p-2,
    0x1.7906fe92593dcp-2,
    0x1.0p0
  },
  { // Entry 38
    -0x1.7fffffffffff97ffb800000013803a80p-33,
    -0x1.7fffffffffffep0,
    0x1.0000000000003p33
  },
  { // Entry 39
    0x1.7fffffffffff97ffb800000013803a80p-33,
    0x1.7fffffffffffep0,
    0x1.0000000000003p33
  },
  { // Entry 40
    -0x1.7fffffffee0020000184c84cd0a5bfb7p-18,
    -0x1.7ffffffffffffp0,
    0x1.ffffffffffffcp17
  },
  { // Entry 41
    0x1.7fffffffee0020000184c84cd0a5bfb7p-18,
    0x1.7ffffffffffffp0,
    0x1.ffffffffffffcp17
  },
  { // Entry 42
    -0x1.01b7ead625912801099d55f3bb6d9b74p0,
    -0x1.8e38e38e38e39p9,
    0x1.f7df7df7df7dep8
  },
  { // Entry 43
    0x1.01b7ead625912801099d55f3bb6d9b74p0,
    0x1.8e38e38e38e39p9,
    0x1.f7df7df7df7dep8
  },
  { // Entry 44
    -0x1.119e0f7084d96bc18bbf2e7a08cfe5adp1,
    -0x1.9249249249246p-2,
    -0x1.001p-2
  },
  { // Entry 45
    0x1.119e0f7084d96bc18bbf2e7a08cfe5adp1,
    0x1.9249249249246p-2,
    -0x1.001p-2
  },
  { // Entry 46
    -0x1.921fb54442d18469898cc51701b839a2p0,
    -0x1.99999999999a8p-4,
    0x1.0p-1074
  },
  { // Entry 47
    0x1.921fb54442d18469898cc51701b839a2p0,
    0x1.99999999999a8p-4,
    0x1.0p-1074
  },
  { // Entry 48
    -0x1.37626c23803aec7d7a70f700585852f4p-1,
    -0x1.a77569dd5a776p8,
    0x1.301ecc07b301ep9
  },
  { // Entry 49
    0x1.37626c23803aec7d7a70f700585852f4p-1,
    0x1.a77569dd5a776p8,
    0x1.301ecc07b301ep9
  },
  { // Entry 50
    -0x1.0ca7cc2d0d7fd03164ee3af269e6bf79p1,
    -0x1.b6db6db6db6e0p-1,
    -0x1.0000000000003p-1
  },
  { // Entry 51
    0x1.0ca7cc2d0d7fd03164ee3af269e6bf79p1,
    0x1.b6db6db6db6e0p-1,
    -0x1.0000000000003p-1
  },
  { // Entry 52
    -0x1.a271f63e34fb2fff42b98e7a5ab17eafp-2,
    -0x1.bb67ae8584c96p-1,
    0x1.0000000000008p1
  },
  { // Entry 53
    0x1.a271f63e34fb2fff42b98e7a5ab17eafp-2,
    0x1.bb67ae8584c96p-1,
    0x1.0000000000008p1
  },
  { // Entry 54
    -0x1.f0845de317dae782ac3e8a7eb1fadd63p-2,
    -0x1.bed61bed61be4p7,
    0x1.a814afd6a053bp8
  },
  { // Entry 55
    0x1.f0845de317dae782ac3e8a7eb1fadd63p-2,
    0x1.bed61bed61be4p7,
    0x1.a814afd6a053bp8
  },
  { // Entry 56
    -0x1.3a51f5f0cb5d33de07ac24a32621878dp-1,
    -0x1.c18f9c18f9c3ep7,
    0x1.3ef368eb04334p8
  },
  { // Entry 57
    0x1.3a51f5f0cb5d33de07ac24a32621878dp-1,
    0x1.c18f9c18f9c3ep7,
    0x1.3ef368eb04334p8
  },
  { // Entry 58
    -0x1.fcb510cd5b6bbb8cde13f46dbeeb3110p-3,
    -0x1.d26a2bad98d68p-2,
    0x1.cbbd407a7a5b0p0
  },
  { // Entry 59
    0x1.fcb510cd5b6bbb8cde13f46dbeeb3110p-3,
    0x1.d26a2bad98d68p-2,
    0x1.cbbd407a7a5b0p0
  },
  { // Entry 60
    -0x1.cd5de97a2e3e1859fc3e2517de7a0880p-3,
    -0x1.d555555555555p0,
    0x1.0000000000003p3
  },
  { // Entry 61
    0x1.cd5de97a2e3e1859fc3e2517de7a0880p-3,
    0x1.d555555555555p0,
    0x1.0000000000003p3
  },
  { // Entry 62
    -0x1.337d175e088fb7fa32fafca382768a15p-3,
    -0x1.db8a874640569p-3,
    0x1.88eed10e75135p0
  },
  { // Entry 63
    0x1.337d175e088fb7fa32fafca382768a15p-3,
    0x1.db8a874640569p-3,
    0x1.88eed10e75135p0
  },
  { // Entry 64
    -0x1.f9d28f3da09c8864390cd924ac658d33p0,
    -0x1.ddddddddddde0p-2,
    -0x1.99ce075f6fd27p-3
  },
  { // Entry 65
    0x1.f9d28f3da09c8864390cd924ac658d33p0,
    0x1.ddddddddddde0p-2,
    -0x1.99ce075f6fd27p-3
  },
  { // Entry 66
    -0x1.ae127b4fb5a7e81cc14c8d0627d18c73p-8,
    -0x1.eccd7fdf96454p10,
    0x1.255608e135d80p18
  },
  { // Entry 67
    0x1.ae127b4fb5a7e81cc14c8d0627d18c73p-8,
    0x1.eccd7fdf96454p10,
    0x1.255608e135d80p18
  },
  { // Entry 68
    -0x1.09121b4b0fb15403f902f2d06a8f1034p1,
    -0x1.f5a814afd6a05p9,
    -0x1.1219dbcc48673p9
  },
  { // Entry 69
    0x1.09121b4b0fb15403f902f2d06a8f1034p1,
    0x1.f5a814afd6a05p9,
    -0x1.1219dbcc48673p9
  },
  { // Entry 70
    -0x1.ffd55bba97625a80f03aaeebb3192417p-6,
    -0x1.ffffffffffffdp-1,
    0x1.ffffffffffffcp4
  },
  { // Entry 71
    0x1.ffd55bba97625a80f03aaeebb3192417p-6,
    0x1.ffffffffffffdp-1,
    0x1.ffffffffffffcp4
  },
  { // Entry 72
    0x1.ffffffffffffe0000000000001fffffdp-64,
    0x1.0p-53,
    0x1.0000000000001p10
  },
  { // Entry 73
    -0x1.ffffffffffffe0000000000001fffffdp-64,
    -0x1.0p-53,
    0x1.0000000000001p10
  },
  { // Entry 74
    0x1.921fb54442d18469898cc51701b839a2p1,
    0x1.0p-1074,
    -0x1.745d1745d173cp-3
  },
  { // Entry 75
    -0x1.921fb54442d18469898cc51701b839a2p1,
    -0x1.0p-1074,
    -0x1.745d1745d173cp-3
  },
  { // Entry 76
    0x1.921fb54442d18469898cc51701b839a2p1,
    0x1.0p-1074,
    -0x1.99999999999a8p-4
  },
  { // Entry 77
    -0x1.921fb54442d18469898cc51701b839a2p1,
    -0x1.0p-1074,
    -0x1.99999999999a8p-4
  },
  { // Entry 78
    0.0,
    0x1.0p-1074,
    0x1.0p1
  },
  { // Entry 79
    -0.0,
    -0x1.0p-1074,
    0x1.0p1
  },
  { // Entry 80
    0x1.ffffffffffffe0000000000001ffffffp-1074,
    0x1.0p-1074,
    0x1.0000000000001p-1
  },
  { // Entry 81
    -0x1.ffffffffffffe0000000000001ffffffp-1074,
    -0x1.0p-1074,
    0x1.0000000000001p-1
  },
  { // Entry 82
    0.0,
    0x1.0p-1074,
    0x1.0222222222223p0
  },
  { // Entry 83
    -0.0,
    -0x1.0p-1074,
    0x1.0222222222223p0
  },
  { // Entry 84
    0.0,
    0x1.0p-1074,
    0x1.03126e978d4fep0
  },
  { // Entry 85
    -0.0,
    -0x1.0p-1074,
    0x1.03126e978d4fep0
  },
  { // Entry 86
    0.0,
    0x1.0p-1074,
    0x1.0a3d70a3d70a3p0
  },
  { // Entry 87
    -0.0,
    -0x1.0p-1074,
    0x1.0a3d70a3d70a3p0
  },
  { // Entry 88
    0x1.0b833be165ccd3f3660d385792d30b1fp1,
    0x1.0000000000001p-2,
    -0x1.24924924924aap-3
  },
  { // Entry 89
    -0x1.0b833be165ccd3f3660d385792d30b1fp1,
    -0x1.0000000000001p-2,
    -0x1.24924924924aap-3
  },
  { // Entry 90
    0x1.5522d16b2f5a7d52fbf6dd4ea12734c4p-5,
    0x1.0000000000001p-4,
    0x1.8000000000001p0
  },
  { // Entry 91
    -0x1.5522d16b2f5a7d52fbf6dd4ea12734c4p-5,
    -0x1.0000000000001p-4,
    0x1.8000000000001p0
  },
  { // Entry 92
    0x1.7ffffffedfffe80184cd02ca5ef0e59ap-16,
    0x1.0000000000001p-17,
    0x1.5555555555558p-2
  },
  { // Entry 93
    -0x1.7ffffffedfffe80184cd02ca5ef0e59ap-16,
    -0x1.0000000000001p-17,
    0x1.5555555555558p-2
  },
  { // Entry 94
    0x1.00000000000017ffaaaaaaaaab6a92aap-32,
    0x1.0000000000001p-31,
    0x1.fffffffffffffp0
  },
  { // Entry 95
    -0x1.00000000000017ffaaaaaaaaab6a92aap-32,
    -0x1.0000000000001p-31,
    0x1.fffffffffffffp0
  },
  { // Entry 96
    0x1.00000000000027ffffffaaaaae6aaaaap-40,
    0x1.0000000000001p-41,
    0x1.ffffffffffffdp-2
  },
  { // Entry 97
    -0x1.00000000000027ffffffaaaaae6aaaaap-40,
    -0x1.0000000000001p-41,
    0x1.ffffffffffffdp-2
  },
  { // Entry 98
    0x1.000000000000680000000000103faaaap-56,
    0x1.0000000000004p1,
    0x1.ffffffffffffbp56
  },
  { // Entry 99
    -0x1.000000000000680000000000103faaaap-56,
    -0x1.0000000000004p1,
    0x1.ffffffffffffbp56
  },
  { // Entry 100
    0x1.fd5ba9aac2f7c8b4561b80036f0e165cp-4,
    0x1.0000000000007p3,
    0x1.fffffffffffffp5
  },
  { // Entry 101
    -0x1.fd5ba9aac2f7c8b4561b80036f0e165cp-4,
    -0x1.0000000000007p3,
    0x1.fffffffffffffp5
  },
  { // Entry 102
    0x1.ff55bb72cfe2e821e203716e1d97a257p-5,
    0x1.000000000001cp-3,
    0x1.ffffffffffff3p0
  },
  { // Entry 103
    -0x1.ff55bb72cfe2e821e203716e1d97a257p-5,
    -0x1.000000000001cp-3,
    0x1.ffffffffffff3p0
  },
  { // Entry 104
    0x1.38e36745aef6d7ab02058b0c0c876fc8p-9,
    0x1.00000000004d6p-8,
    0x1.a2e8ba2e8ba30p0
  },
  { // Entry 105
    -0x1.38e36745aef6d7ab02058b0c0c876fc8p-9,
    -0x1.00000000004d6p-8,
    0x1.a2e8ba2e8ba30p0
  },
  { // Entry 106
    0x1.3ff4325a8437500000286dff86bc02adp-1,
    0x1.00000009f0205p-1,
    0x1.62e42fefa39efp-1
  },
  { // Entry 107
    -0x1.3ff4325a8437500000286dff86bc02adp-1,
    -0x1.00000009f0205p-1,
    0x1.62e42fefa39efp-1
  },
  { // Entry 108
    0x1.b4f5f236308b48037fe229608c1d81fbp-2,
    0x1.001p-1,
    0x1.199999999999ap0
  },
  { // Entry 109
    -0x1.b4f5f236308b48037fe229608c1d81fbp-2,
    -0x1.001p-1,
    0x1.199999999999ap0
  },
  { // Entry 110
    0x1.9242e6442d4317ff2531ceafca8af0f4p0,
    0x1.001p10,
    -0x1.1999999999999p-1
  },
  { // Entry 111
    -0x1.9242e6442d4317ff2531ceafca8af0f4p0,
    -0x1.001p10,
    -0x1.1999999999999p-1
  },
  { // Entry 112
    0x1.046862a40cbe6ab9070021df9e1e411bp1,
    0x1.0175fcd4ab261p1,
    -0x1.01749ca942943p0
  },
  { // Entry 113
    -0x1.046862a40cbe6ab9070021df9e1e411bp1,
    -0x1.0175fcd4ab261p1,
    -0x1.01749ca942943p0
  },
  { // Entry 114
    0x1.9b2bb7e10b2677febb378df81a94d587p-1,
    0x1.066b3f39ae7a1p0,
    0x1.fa9c4b9f46842p-1
  },
  { // Entry 115
    -0x1.9b2bb7e10b2677febb378df81a94d587p-1,
    -0x1.066b3f39ae7a1p0,
    0x1.fa9c4b9f46842p-1
  },
  { // Entry 116
    0x1.e897850a716e4889a143afe6cdae77d1p-2,
    0x1.08b1d3b97c955p-2,
    0x1.0p-1
  },
  { // Entry 117
    -0x1.e897850a716e4889a143afe6cdae77d1p-2,
    -0x1.08b1d3b97c955p-2,
    0x1.0p-1
  },
  { // Entry 118
    0x1.13e7bb06113d680135e98e8c2e9c9628p-92,
    0x1.09d89d89d89d8p9,
    0x1.ed55555555573p100
  },
  { // Entry 119
    -0x1.13e7bb06113d680135e98e8c2e9c9628p-92,
    -0x1.09d89d89d89d8p9,
    0x1.ed55555555573p100
  },
  { // Entry 120
    0x1.ff572aded0be7136f236315e3c9eccb7p0,
    0x1.199999999999cp-1,
    -0x1.ffffffffffffep-3
  },
  { // Entry 121
    -0x1.ff572aded0be7136f236315e3c9eccb7p0,
    -0x1.199999999999cp-1,
    -0x1.ffffffffffffep-3
  },
  { // Entry 122
    0x1.196ba6878b92680ebe4a5666ff18a384p-5,
    0x1.199999999999cp-2,
    0x1.001p3
  },
  { // Entry 123
    -0x1.196ba6878b92680ebe4a5666ff18a384p-5,
    -0x1.199999999999cp-2,
    0x1.001p3
  },
  { // Entry 124
    0x1.1b4a29a02a9c87fffdb48e539399967ap-3,
    0x1.1adec7d06a010p-2,
    0x1.fbfa204c8234cp0
  },
  { // Entry 125
    -0x1.1b4a29a02a9c87fffdb48e539399967ap-3,
    -0x1.1adec7d06a010p-2,
    0x1.fbfa204c8234cp0
  },
  { // Entry 126
    0x1.19dcd054169247fffd4cc05900e64848p-2,
    0x1.213422ec61f53p-3,
    0x1.0p-1
  },
  { // Entry 127
    -0x1.19dcd054169247fffd4cc05900e64848p-2,
    -0x1.213422ec61f53p-3,
    0x1.0p-1
  },
  { // Entry 128
    0x1.b3b95bdcb30277fec23bb4be90b63531p-1,
    0x1.2776fe2145bd5p0,
    0x1.0306216790738p0
  },
  { // Entry 129
    -0x1.b3b95bdcb30277fec23bb4be90b63531p-1,
    -0x1.2776fe2145bd5p0,
    0x1.0306216790738p0
  },
  { // Entry 130
    0x1.ee0c54984cb15edcdcb239dbfffd57dep-4,
    0x1.27fb7de0e57c8p12,
    0x1.313f9061390p15
  },
  { // Entry 131
    -0x1.ee0c54984cb15edcdcb239dbfffd57dep-4,
    -0x1.27fb7de0e57c8p12,
    0x1.313f9061390p15
  },
  { // Entry 132
    0x1.27ff4834766d779860765d14b68788cep-8,
    0x1.27fb7de0e57c8p12,
    0x1.fff88d6e2d934p19
  },
  { // Entry 133
    -0x1.27ff4834766d779860765d14b68788cep-8,
    -0x1.27fb7de0e57c8p12,
    0x1.fff88d6e2d934p19
  },
  { // Entry 134
    0x1.f9c6b238c6435777790ced0df81049e2p0,
    0x1.2aaaaaaaaaaabp0,
    -0x1.0000000000003p-1
  },
  { // Entry 135
    -0x1.f9c6b238c6435777790ced0df81049e2p0,
    -0x1.2aaaaaaaaaaabp0,
    -0x1.0000000000003p-1
  },
  { // Entry 136
    0x1.2aaaaaaaaaaa77ff787e6b74f9b2d658p-32,
    0x1.2aaaaaaaaaaabp0,
    0x1.0000000000003p32
  },
  { // Entry 137
    -0x1.2aaaaaaaaaaa77ff787e6b74f9b2d658p-32,
    -0x1.2aaaaaaaaaaabp0,
    0x1.0000000000003p32
  },
  { // Entry 138
    0x1.edae91ebbfb8780006f7e9144583c7b3p0,
    0x1.2d66ca857bf9ap0,
    -0x1.c28f5c28f5c28p-2
  },
  { // Entry 139
    -0x1.edae91ebbfb8780006f7e9144583c7b3p0,
    -0x1.2d66ca857bf9ap0,
    -0x1.c28f5c28f5c28p-2
  },
  { // Entry 140
    0x1.9d5a77d67cf1d7febab338e68f258f5ap-1,
    0x1.2e12530a85951p2,
    0x1.211a7b9611a7bp2
  },
  { // Entry 141
    -0x1.9d5a77d67cf1d7febab338e68f258f5ap-1,
    -0x1.2e12530a85951p2,
    0x1.211a7b9611a7bp2
  },
  { // Entry 142
    0x1.a244e21ebefa8fffffbfabeaba9e67acp-2,
    0x1.3333333d813abp-2,
    0x1.62e42fefa39efp-1
  },
  { // Entry 143
    -0x1.a244e21ebefa8fffffbfabeaba9e67acp-2,
    -0x1.3333333d813abp-2,
    0x1.62e42fefa39efp-1
  },
  { // Entry 144
    0x1.90a08b292067f00094284270c3b75547p-4,
    0x1.3deb308a9c960p-4,
    0x1.9500a27c6a82ep-1
  },
  { // Entry 145
    -0x1.90a08b292067f00094284270c3b75547p-4,
    -0x1.3deb308a9c960p-4,
    0x1.9500a27c6a82ep-1
  },
  { // Entry 146
    0x1.a896592d6fdb57b63fa6ed728b91fe47p-5,
    0x1.420bc59c42c7cp2,
    0x1.83fffffffffffp6
  },
  { // Entry 147
    -0x1.a896592d6fdb57b63fa6ed728b91fe47p-5,
    -0x1.420bc59c42c7cp2,
    0x1.83fffffffffffp6
  },
  { // Entry 148
    0x1.25e3010ff1ed37fe769fa76dea43608cp1,
    0x1.494b48acbe5b0p-9,
    -0x1.23da61f087530p-9
  },
  { // Entry 149
    -0x1.25e3010ff1ed37fe769fa76dea43608cp1,
    -0x1.494b48acbe5b0p-9,
    -0x1.23da61f087530p-9
  },
  { // Entry 150
    0x1.6d0d1984633eb80e098b6dc91f083a06p-3,
    0x1.51ff85f2ba468p0,
    0x1.d50692986b95dp2
  },
  { // Entry 151
    -0x1.6d0d1984633eb80e098b6dc91f083a06p-3,
    -0x1.51ff85f2ba468p0,
    0x1.d50692986b95dp2
  },
  { // Entry 152
    0x1.ccadda48d08027ff92d1bd814812ce8cp-1,
    0x1.5412e00233d75p-1,
    0x1.0dff2d1714940p-1
  },
  { // Entry 153
    -0x1.ccadda48d08027ff92d1bd814812ce8cp-1,
    -0x1.5412e00233d75p-1,
    0x1.0dff2d1714940p-1
  },
  { // Entry 154
    0x1.3f2496d84ac34801117f6f830c0fb201p-90,
    0x1.5555555554c2ep8,
    0x1.11ccccccccccdp98
  },
  { // Entry 155
    -0x1.3f2496d84ac34801117f6f830c0fb201p-90,
    -0x1.5555555554c2ep8,
    0x1.11ccccccccccdp98
  },
  { // Entry 156
    0x1.555555555555aaaaaaaaaaaab5555555p-1021,
    0x1.5555555555558p-2,
    0x1.ffffffffffffcp1018
  },
  { // Entry 157
    -0x1.555555555555aaaaaaaaaaaab5555555p-1021,
    -0x1.5555555555558p-2,
    0x1.ffffffffffffcp1018
  },
  { // Entry 158
    0x1.88134cb8d04e88985007b92a62b1fd1ap-8,
    0x1.5711ef5ee1eecp-5,
    0x1.c000000000302p2
  },
  { // Entry 159
    -0x1.88134cb8d04e88985007b92a62b1fd1ap-8,
    -0x1.5711ef5ee1eecp-5,
    0x1.c000000000302p2
  },
  { // Entry 160
    0x1.94a470782907f800006a4822bc94bc23p-1,
    0x1.666666688d411p-1,
    0x1.62e42fefa39efp-1
  },
  { // Entry 161
    -0x1.94a470782907f800006a4822bc94bc23p-1,
    -0x1.666666688d411p-1,
    0x1.62e42fefa39efp-1
  },
  { // Entry 162
    0x1.d7d7b672ee30c85d01819b25305f3230p-10,
    0x1.6c9b26c9b26cap0,
    0x1.8ba2e8ba2e8cap9
  },
  { // Entry 163
    -0x1.d7d7b672ee30c85d01819b25305f3230p-10,
    -0x1.6c9b26c9b26cap0,
    0x1.8ba2e8ba2e8cap9
  },
  { // Entry 164
    0x1.7fffffffee0030000184c60cd2a5c008p-18,
    0x1.8p-53,
    0x1.ffffffffffffcp-36
  },
  { // Entry 165
    -0x1.7fffffffee0030000184c60cd2a5c008p-18,
    -0x1.8p-53,
    0x1.ffffffffffffcp-36
  },
  { // Entry 166
    0x1.e7c8952cb26158012b54b9a61c08f431p-2,
    0x1.8c46231188cp0,
    0x1.8p1
  },
  { // Entry 167
    -0x1.e7c8952cb26158012b54b9a61c08f431p-2,
    -0x1.8c46231188cp0,
    0x1.8p1
  },
  { // Entry 168
    0x1.2b854f022de7a93cb621cb2462f86074p0,
    0x1.8d79435e50d71p2,
    0x1.50d79435e50d9p1
  },
  { // Entry 169
    -0x1.2b854f022de7a93cb621cb2462f86074p0,
    -0x1.8d79435e50d71p2,
    0x1.50d79435e50d9p1
  },
  { // Entry 170
    0x1.42a76a164c39c800e4405027c490bdfbp-1,
    0x1.8dd3d2235ad60p-1,
    0x1.10b5d1e78459cp0
  },
  { // Entry 171
    -0x1.42a76a164c39c800e4405027c490bdfbp-1,
    -0x1.8dd3d2235ad60p-1,
    0x1.10b5d1e78459cp0
  },
  { // Entry 172
    0x1.e3240e993ab957f9d76dde4a50896826p-3,
    0x1.9p0,
    0x1.9fffffffffffbp2
  },
  { // Entry 173
    -0x1.e3240e993ab957f9d76dde4a50896826p-3,
    -0x1.9p0,
    0x1.9fffffffffffbp2
  },
  { // Entry 174
    0x1.a335efd4da90a804f7a6dad4434f5ba0p-2,
    0x1.920d799fda713p-3,
    0x1.cf4cdc48f3536p-2
  },
  { // Entry 175
    -0x1.a335efd4da90a804f7a6dad4434f5ba0p-2,
    -0x1.920d799fda713p-3,
    0x1.cf4cdc48f3536p-2
  },
  { // Entry 176
    0x1.ed87f9c729d17ffe9d2f47e2fe9ecb40p-1,
    0x1.95fad40a57ec6p9,
    0x1.19dbcc48676f6p9
  },
  { // Entry 177
    -0x1.ed87f9c729d17ffe9d2f47e2fe9ecb40p-1,
    -0x1.95fad40a57ec6p9,
    0x1.19dbcc48676f6p9
  },
  { // Entry 178
    0x1.0bfa5f3f099e68000068d82232dc4cc7p-1,
    0x1.9999999e37c24p-2,
    0x1.62e42fefa39efp-1
  },
  { // Entry 179
    -0x1.0bfa5f3f099e68000068d82232dc4cc7p-1,
    -0x1.9999999e37c24p-2,
    0x1.62e42fefa39efp-1
  },
  { // Entry 180
    0x1.1fa6ac30d066d800006a5c239e5188f2p-2,
    0x1.999999bb09140p-3,
    0x1.62e42fefa39efp-1
  },
  { // Entry 181
    -0x1.1fa6ac30d066d800006a5c239e5188f2p-2,
    -0x1.999999bb09140p-3,
    0x1.62e42fefa39efp-1
  },
  { // Entry 182
    0x1.2570742fa4989fffff9e946c986117d9p-3,
    0x1.999999c2f3b55p-4,
    0x1.62e42fefa39efp-1
  },
  { // Entry 183
    -0x1.2570742fa4989fffff9e946c986117d9p-3,
    -0x1.999999c2f3b55p-4,
    0x1.62e42fefa39efp-1
  },
  { // Entry 184
    0x1.257074378653a7ffffffb78db995aafcp-3,
    0x1.999999ce1b18ap-4,
    0x1.62e42fefa39efp-1
  },
  { // Entry 185
    -0x1.257074378653a7ffffffb78db995aafcp-3,
    -0x1.999999ce1b18ap-4,
    0x1.62e42fefa39efp-1
  },
  { // Entry 186
    0x1.25707437a1476000006169ddb5dabdd7p-3,
    0x1.999999ce413ccp-4,
    0x1.62e42fefa39efp-1
  },
  { // Entry 187
    -0x1.25707437a1476000006169ddb5dabdd7p-3,
    -0x1.999999ce413ccp-4,
    0x1.62e42fefa39efp-1
  },
  { // Entry 188
    0x1.b6a03b0f1ff0d7fd08e9f5846ec5a75bp-1,
    0x1.9c5c97530cc21p0,
    0x1.6563fa1d6f518p0
  },
  { // Entry 189
    -0x1.b6a03b0f1ff0d7fd08e9f5846ec5a75bp-1,
    -0x1.9c5c97530cc21p0,
    0x1.6563fa1d6f518p0
  },
  { // Entry 190
    0x1.b80aa068167f97558972ecfed6777774p-19,
    0x1.9f5aeae03799dp-18,
    0x1.e346d9b2ad73ep0
  },
  { // Entry 191
    -0x1.b80aa068167f97558972ecfed6777774p-19,
    -0x1.9f5aeae03799dp-18,
    0x1.e346d9b2ad73ep0
  },
  { // Entry 192
    0x1.e5ef7b22c83b27ad34bbfda6c2383d23p-2,
    0x1.a4d269349b66cp-3,
    0x1.999999999999ap-2
  },
  { // Entry 193
    -0x1.e5ef7b22c83b27ad34bbfda6c2383d23p-2,
    -0x1.a4d269349b66cp-3,
    0x1.999999999999ap-2
  },
  { // Entry 194
    0x1.5e82cb51676728011e1c6ba75f3339a7p-91,
    0x1.ad5aa6ff6335ep9,
    0x1.3995880de757ap100
  },
  { // Entry 195
    -0x1.5e82cb51676728011e1c6ba75f3339a7p-91,
    -0x1.ad5aa6ff6335ep9,
    0x1.3995880de757ap100
  },
  { // Entry 196
    0x1.95ac93504f319fefb1b5148792dab412p-1,
    0x1.b1427cd988b8cp-2,
    0x1.ab4adeaf1a3eap-2
  },
  { // Entry 197
    -0x1.95ac93504f319fefb1b5148792dab412p-1,
    -0x1.b1427cd988b8cp-2,
    0x1.ab4adeaf1a3eap-2
  },
  { // Entry 198
    0x1.921fd1f09f928801088a93fc7dbba1cap0,
    0x1.bbd49acc58d98p10,
    -0x1.8db0a4ab22e7ep-9
  },
  { // Entry 199
    -0x1.921fd1f09f928801088a93fc7dbba1cap0,
    -0x1.bbd49acc58d98p10,
    -0x1.8db0a4ab22e7ep-9
  },
  { // Entry 200
    0x1.43e54975fb8bc8012953e9ef023f67b3p-100,
    0x1.c37dac37dac3cp0,
    0x1.64d9364d93659p100
  },
  { // Entry 201
    -0x1.43e54975fb8bc8012953e9ef023f67b3p-100,
    -0x1.c37dac37dac3cp0,
    0x1.64d9364d93659p100
  },
  { // Entry 202
    0x1.ab78c13521cfc80117f7fae57836356ep-98,
    0x1.c9b26c9b26ca0p2,
    0x1.1219dbcc48679p100
  },
  { // Entry 203
    -0x1.ab78c13521cfc80117f7fae57836356ep-98,
    -0x1.c9b26c9b26ca0p2,
    0x1.1219dbcc48679p100
  },
  { // Entry 204
    0x1.77e467d5ff6337f84f880eb86f426f87p-1,
    0x1.ce0d5078ae3d0p0,
    0x1.0p1
  },
  { // Entry 205
    -0x1.77e467d5ff6337f84f880eb86f426f87p-1,
    -0x1.ce0d5078ae3d0p0,
    0x1.0p1
  },
  { // Entry 206
    0x1.7ccd882d8fdbe8010d0be61f023186a5p-1,
    0x1.dbcc48676f32ap7,
    0x1.0295fad40a58bp8
  },
  { // Entry 207
    -0x1.7ccd882d8fdbe8010d0be61f023186a5p-1,
    -0x1.dbcc48676f32ap7,
    0x1.0295fad40a58bp8
  },
  { // Entry 208
    0x1.2d3a87e24eb319156ef615caa7abe128p0,
    0x1.e052bf5a814b6p2,
    0x1.8f83e0f83e0f1p1
  },
  { // Entry 209
    -0x1.2d3a87e24eb319156ef615caa7abe128p0,
    -0x1.e052bf5a814b6p2,
    0x1.8f83e0f83e0f1p1
  },
  { // Entry 210
    0x1.eb0df42c36a5f7fe1df8c86bed0a28a0p-1,
    0x1.e0547e40e4cc8p-2,
    0x1.50eebc195bb24p-2
  },
  { // Entry 211
    -0x1.eb0df42c36a5f7fe1df8c86bed0a28a0p-1,
    -0x1.e0547e40e4cc8p-2,
    0x1.50eebc195bb24p-2
  },
  { // Entry 212
    0x1.c7fe1dbd95349778458697fe195e4a58p-8,
    0x1.e666666666668p1,
    0x1.111111111196dp9
  },
  { // Entry 213
    -0x1.c7fe1dbd95349778458697fe195e4a58p-8,
    -0x1.e666666666668p1,
    0x1.111111111196dp9
  },
  { // Entry 214
    0x1.38927ede67216800006a39bb49e9c0f4p0,
    0x1.e666666b987f5p0,
    0x1.62e42fefa39efp-1
  },
  { // Entry 215
    -0x1.38927ede67216800006a39bb49e9c0f4p0,
    -0x1.e666666b987f5p0,
    0x1.62e42fefa39efp-1
  },
  { // Entry 216
    0x1.39fed5699428b3b69a7339d36bb044abp-1,
    0x1.e91ee78afd1e6p-3,
    0x1.5b7c32b32bde0p-2
  },
  { // Entry 217
    -0x1.39fed5699428b3b69a7339d36bb044abp-1,
    -0x1.e91ee78afd1e6p-3,
    0x1.5b7c32b32bde0p-2
  },
  { // Entry 218
    0x1.a127224010cba803945f315e1d0ee4b5p-1,
    0x1.f166e1dc4499bp2,
    0x1.d50692986b98fp2
  },
  { // Entry 219
    -0x1.a127224010cba803945f315e1d0ee4b5p-1,
    -0x1.f166e1dc4499bp2,
    0x1.d50692986b98fp2
  },
  { // Entry 220
    0x1.f1f32aa6acb70801dd4349d43d3d4c25p-3,
    0x1.fbfffffffffffp-2,
    0x1.ffffffffe7fffp0
  },
  { // Entry 221
    -0x1.f1f32aa6acb70801dd4349d43d3d4c25p-3,
    -0x1.fbfffffffffffp-2,
    0x1.ffffffffe7fffp0
  },
  { // Entry 222
    0x1.079c41361b6ab8115015e4f32dcfe4b3p1,
    0x1.ff4b7c848cde6p-1,
    -0x1.0fd6c7f44f588p-1
  },
  { // Entry 223
    -0x1.079c41361b6ab8115015e4f32dcfe4b3p1,
    -0x1.ff4b7c848cde6p-1,
    -0x1.0fd6c7f44f588p-1
  },
  { // Entry 224
    0x1.da4c6912789968011f1b516f595d868ep-2,
    0x1.ff677ffffffffp-6,
    0x1.ffffffff19fffp-5
  },
  { // Entry 225
    -0x1.da4c6912789968011f1b516f595d868ep-2,
    -0x1.ff677ffffffffp-6,
    0x1.ffffffff19fffp-5
  },
  { // Entry 226
    0x1.fffc80021ffc480225fc1d822a9bc5e0p-58,
    0x1.fffc7ffffffffp-50,
    0x1.fffffffddffffp7
  },
  { // Entry 227
    -0x1.fffc80021ffc480225fc1d822a9bc5e0p-58,
    -0x1.fffc7ffffffffp-50,
    0x1.fffffffddffffp7
  },
  { // Entry 228
    0x1.66666666666617fffffffffff8b72015p-50,
    0x1.ffffffffffffcp50,
    0x1.6db6db6db6db9p100
  },
  { // Entry 229
    -0x1.66666666666617fffffffffff8b72015p-50,
    -0x1.ffffffffffffcp50,
    0x1.6db6db6db6db9p100
  },
  { // Entry 230
    0x1.ff55d35ae8e467ce77407069ad013ab5p-5,
    0x1.ffffffffffffep-3,
    0x1.ffffe7fffffffp1
  },
  { // Entry 231
    -0x1.ff55d35ae8e467ce77407069ad013ab5p-5,
    -0x1.ffffffffffffep-3,
    0x1.ffffe7fffffffp1
  },
  { // Entry 232
    0x1.b4ddd66a37b3b335a2a5b11ceb9a4c56p-2,
    0x1.ffffffffffffep-4,
    0x1.199999999999cp-2
  },
  { // Entry 233
    -0x1.b4ddd66a37b3b335a2a5b11ceb9a4c56p-2,
    -0x1.ffffffffffffep-4,
    0x1.199999999999cp-2
  },
  { // Entry 234
    0x1.90e6d4253517c8010321aeae887990a9p1,
    0x1.ffffffffffffep-7,
    -0x1.a2e8ba2e97a22p0
  },
  { // Entry 235
    -0x1.90e6d4253517c8010321aeae887990a9p1,
    -0x1.ffffffffffffep-7,
    -0x1.a2e8ba2e97a22p0
  },
  { // Entry 236
    0x1.b6db6db6db6d281ddaaea5b12cced2a1p-25,
    0x1.ffffffffffffep-25,
    0x1.2aaaaaaaaaaabp0
  },
  { // Entry 237
    -0x1.b6db6db6db6d281ddaaea5b12cced2a1p-25,
    -0x1.ffffffffffffep-25,
    0x1.2aaaaaaaaaaabp0
  },
  { // Entry 238
    0x1.921fb54442d18469898cc51701b839a2p1,
    0x1.ffffffffffffep-807,
    -0x1.745d1745d173cp-3
  },
  { // Entry 239
    -0x1.921fb54442d18469898cc51701b839a2p1,
    -0x1.ffffffffffffep-807,
    -0x1.745d1745d173cp-3
  },
  { // Entry 240
    0x1.096d05371b1c54c40f9a06c6cf2db981p1,
    0x1.ffffffffffffep0,
    -0x1.199999999999ap0
  },
  { // Entry 241
    -0x1.096d05371b1c54c40f9a06c6cf2db981p1,
    -0x1.ffffffffffffep0,
    -0x1.199999999999ap0
  },
  { // Entry 242
    0x1.921fb54442d18469898cc51701b839a2p0,
    0x1.fffffffffffffp1023,
    0x1.4p-1072
  },
  { // Entry 243
    -0x1.921fb54442d18469898cc51701b839a2p0,
    -0x1.fffffffffffffp1023,
    0x1.4p-1072
  },
  { // Entry 244
    0x1.921fb54442d18469898cc51701b839a2p-1,
    0x1.fffffffffffffp1023,
    0x1.fffffffffffffp1023
  },
  { // Entry 245
    -0x1.921fb54442d18469898cc51701b839a2p-1,
    -0x1.fffffffffffffp1023,
    0x1.fffffffffffffp1023
  },
  { // Entry 246
    -0x1.f4f3b198c0f168030e9490be1ea559e8p-4,
    -0x1.08de20fafe4a2p0,
    0x1.0d5ba77adf969p3
  },
  { // Entry 247
    0x1.f4f3b198c0f168030e9490be1ea559e8p-4,
    0x1.08de20fafe4a2p0,
    0x1.0d5ba77adf969p3
  },
  { // Entry 248
    -0x1.2d97c7f3321d234f272993d1414a2b39p1,
    -0x1.0p0,
    -0x1.0p0
  },
  { // Entry 249
    0x1.2d97c7f3321d234f272993d1414a2b39p1,
    0x1.0p0,
    -0x1.0p0
  },
  { // Entry 250
    -0x1.921fb54442d18469898cc51701b839a2p-1,
    -0x1.0p0,
    0x1.0p0
  },
  { // Entry 251
    0x1.921fb54442d18469898cc51701b839a2p-1,
    0x1.0p0,
    0x1.0p0
  },
  { // Entry 252
    0x1.2d97c7f3321d234f272993d1414a2b39p1,
    0x1.0p0,
    -0x1.0p0
  },
  { // Entry 253
    -0x1.2d97c7f3321d234f272993d1414a2b39p1,
    -0x1.0p0,
    -0x1.0p0
  },
  { // Entry 254
    0x1.921fb54442d18469898cc51701b839a2p-1,
    0x1.0p0,
    0x1.0p0
  },
  { // Entry 255
    -0x1.921fb54442d18469898cc51701b839a2p-1,
    -0x1.0p0,
    0x1.0p0
  },
  { // Entry 256
    0x1.921fb54442d18469898cc51701b839a2p-1,
    0x1.0p0,
    0x1.0p0
  },
  { // Entry 257
    -0x1.921fb54442d18469898cc51701b839a2p-1,
    -0x1.0p0,
    0x1.0p0
  },
  { // Entry 258
    0x1.fd5ba9aac2f6dc65912f313e7d111defp-4,
    0x1.0p0,
    0x1.0p3
  },
  { // Entry 259
    -0x1.fd5ba9aac2f6dc65912f313e7d111defp-4,
    -0x1.0p0,
    0x1.0p3
  },
  { // Entry 260
    0x1.7249faa996a216a33079d20319e727c3p0,
    0x1.0p3,
    0x1.0p0
  },
  { // Entry 261
    -0x1.7249faa996a216a33079d20319e727c3p0,
    -0x1.0p3,
    0x1.0p0
  },
  { // Entry 262
    0x1.921fb54442d18469898cc51701b839a2p-1,
    0x1.0p3,
    0x1.0p3
  },
  { // Entry 263
    -0x1.921fb54442d18469898cc51701b839a2p-1,
    -0x1.0p3,
    0x1.0p3
  },
  { // Entry 264
    0x1.ffffd5555bbbba9729ab7aac089473a3p-10,
    0x1.0p0,
    0x1.0p9
  },
  { // Entry 265
    -0x1.ffffd5555bbbba9729ab7aac089473a3p-10,
    -0x1.0p0,
    0x1.0p9
  },
  { // Entry 266
    0x1.fffff55555bbbbb72972d00cfde752f9p-11,
    0x1.0p0,
    0x1.0p10
  },
  { // Entry 267
    -0x1.fffff55555bbbbb72972d00cfde752f9p-11,
    -0x1.0p0,
    0x1.0p10
  },
  { // Entry 268
    0x1.fff555bbb729ab77cf18ac802beec090p-7,
    0x1.0p3,
    0x1.0p9
  },
  { // Entry 269
    -0x1.fff555bbb729ab77cf18ac802beec090p-7,
    -0x1.0p3,
    0x1.0p9
  },
  { // Entry 270
    0x1.fffd555bbba972d00c46a3f77cc15e8ep-8,
    0x1.0p3,
    0x1.0p10
  },
  { // Entry 271
    -0x1.fffd555bbba972d00c46a3f77cc15e8ep-8,
    -0x1.0p3,
    0x1.0p10
  },
  { // Entry 272
    0x1.ffffffffffffffffffffffffffffffffp-101,
    0x1.0p0,
    0x1.0p100
  },
  { // Entry 273
    -0x1.ffffffffffffffffffffffffffffffffp-101,
    -0x1.0p0,
    0x1.0p100
  },
  { // Entry 274
    0x1.ffffffffffffffffffffffffffffffffp-102,
    0x1.0p0,
    0x1.0p101
  },
  { // Entry 275
    -0x1.ffffffffffffffffffffffffffffffffp-102,
    -0x1.0p0,
    0x1.0p101
  },
  { // Entry 276
    0x1.ffffffffffffffffffffffffffffffffp-98,
    0x1.0p3,
    0x1.0p100
  },
  { // Entry 277
    -0x1.ffffffffffffffffffffffffffffffffp-98,
    -0x1.0p3,
    0x1.0p100
  },
  { // Entry 278
    0x1.ffffffffffffffffffffffffffffffffp-99,
    0x1.0p3,
    0x1.0p101
  },
  { // Entry 279
    -0x1.ffffffffffffffffffffffffffffffffp-99,
    -0x1.0p3,
    0x1.0p101
  },
  { // Entry 280
    0x1.919fb54eed7a957ae3c25a3856b61485p0,
    0x1.0p9,
    0x1.0p0
  },
  { // Entry 281
    -0x1.919fb54eed7a957ae3c25a3856b61485p0,
    -0x1.0p9,
    0x1.0p0
  },
  { // Entry 282
    0x1.8e1fca98cb63311299ee93be01605c21p0,
    0x1.0p9,
    0x1.0p3
  },
  { // Entry 283
    -0x1.8e1fca98cb63311299ee93be01605c21p0,
    -0x1.0p9,
    0x1.0p3
  },
  { // Entry 284
    0x1.91dfb5459826ccf212a796bd00187cb7p0,
    0x1.0p10,
    0x1.0p0
  },
  { // Entry 285
    -0x1.91dfb5459826ccf212a796bd00187cb7p0,
    -0x1.0p10,
    0x1.0p0
  },
  { // Entry 286
    0x1.901fb7eee715daf6b9807e730a3b7843p0,
    0x1.0p10,
    0x1.0p3
  },
  { // Entry 287
    -0x1.901fb7eee715daf6b9807e730a3b7843p0,
    -0x1.0p10,
    0x1.0p3
  },
  { // Entry 288
    0x1.921fb54442d18469898cc51701b839a2p-1,
    0x1.0p9,
    0x1.0p9
  },
  { // Entry 289
    -0x1.921fb54442d18469898cc51701b839a2p-1,
    -0x1.0p9,
    0x1.0p9
  },
  { // Entry 290
    0x1.dac670561bb4f68adfc88bd978751a06p-2,
    0x1.0p9,
    0x1.0p10
  },
  { // Entry 291
    -0x1.dac670561bb4f68adfc88bd978751a06p-2,
    -0x1.0p9,
    0x1.0p10
  },
  { // Entry 292
    0x1.1b6e192ebbe446c6d19aa220a39af320p0,
    0x1.0p10,
    0x1.0p9
  },
  { // Entry 293
    -0x1.1b6e192ebbe446c6d19aa220a39af320p0,
    -0x1.0p10,
    0x1.0p9
  },
  { // Entry 294
    0x1.921fb54442d18469898cc51701b839a2p-1,
    0x1.0p10,
    0x1.0p10
  },
  { // Entry 295
    -0x1.921fb54442d18469898cc51701b839a2p-1,
    -0x1.0p10,
    0x1.0p10
  },
  { // Entry 296
    0x1.ffffffffffffffffffffffffffffffffp-92,
    0x1.0p9,
    0x1.0p100
  },
  { // Entry 297
    -0x1.ffffffffffffffffffffffffffffffffp-92,
    -0x1.0p9,
    0x1.0p100
  },
  { // Entry 298
    0x1.ffffffffffffffffffffffffffffffffp-93,
    0x1.0p9,
    0x1.0p101
  },
  { // Entry 299
    -0x1.ffffffffffffffffffffffffffffffffp-93,
    -0x1.0p9,
    0x1.0p101
  },
  { // Entry 300
    0x1.ffffffffffffffffffffffffffffffffp-91,
    0x1.0p10,
    0x1.0p100
  },
  { // Entry 301
    -0x1.ffffffffffffffffffffffffffffffffp-91,
    -0x1.0p10,
    0x1.0p100
  },
  { // Entry 302
    0x1.ffffffffffffffffffffffffffffffffp-92,
    0x1.0p10,
    0x1.0p101
  },
  { // Entry 303
    -0x1.ffffffffffffffffffffffffffffffffp-92,
    -0x1.0p10,
    0x1.0p101
  },
  { // Entry 304
    0x1.921fb54442d18469898cc516f1b839a2p0,
    0x1.0p100,
    0x1.0p0
  },
  { // Entry 305
    -0x1.921fb54442d18469898cc516f1b839a2p0,
    -0x1.0p100,
    0x1.0p0
  },
  { // Entry 306
    0x1.921fb54442d18469898cc51681b839a2p0,
    0x1.0p100,
    0x1.0p3
  },
  { // Entry 307
    -0x1.921fb54442d18469898cc51681b839a2p0,
    -0x1.0p100,
    0x1.0p3
  },
  { // Entry 308
    0x1.921fb54442d18469898cc516f9b839a2p0,
    0x1.0p101,
    0x1.0p0
  },
  { // Entry 309
    -0x1.921fb54442d18469898cc516f9b839a2p0,
    -0x1.0p101,
    0x1.0p0
  },
  { // Entry 310
    0x1.921fb54442d18469898cc516c1b839a2p0,
    0x1.0p101,
    0x1.0p3
  },
  { // Entry 311
    -0x1.921fb54442d18469898cc516c1b839a2p0,
    -0x1.0p101,
    0x1.0p3
  },
  { // Entry 312
    0x1.921fb54442d18469898cc4f701b839a2p0,
    0x1.0p100,
    0x1.0p9
  },
  { // Entry 313
    -0x1.921fb54442d18469898cc4f701b839a2p0,
    -0x1.0p100,
    0x1.0p9
  },
  { // Entry 314
    0x1.921fb54442d18469898cc4d701b839a2p0,
    0x1.0p100,
    0x1.0p10
  },
  { // Entry 315
    -0x1.921fb54442d18469898cc4d701b839a2p0,
    -0x1.0p100,
    0x1.0p10
  },
  { // Entry 316
    0x1.921fb54442d18469898cc50701b839a2p0,
    0x1.0p101,
    0x1.0p9
  },
  { // Entry 317
    -0x1.921fb54442d18469898cc50701b839a2p0,
    -0x1.0p101,
    0x1.0p9
  },
  { // Entry 318
    0x1.921fb54442d18469898cc4f701b839a2p0,
    0x1.0p101,
    0x1.0p10
  },
  { // Entry 319
    -0x1.921fb54442d18469898cc4f701b839a2p0,
    -0x1.0p101,
    0x1.0p10
  },
  { // Entry 320
    0x1.921fb54442d18469898cc51701b839a2p-1,
    0x1.0p100,
    0x1.0p100
  },
  { // Entry 321
    -0x1.921fb54442d18469898cc51701b839a2p-1,
    -0x1.0p100,
    0x1.0p100
  },
  { // Entry 322
    0x1.dac670561bb4f68adfc88bd978751a06p-2,
    0x1.0p100,
    0x1.0p101
  },
  { // Entry 323
    -0x1.dac670561bb4f68adfc88bd978751a06p-2,
    -0x1.0p100,
    0x1.0p101
  },
  { // Entry 324
    0x1.1b6e192ebbe446c6d19aa220a39af320p0,
    0x1.0p101,
    0x1.0p100
  },
  { // Entry 325
    -0x1.1b6e192ebbe446c6d19aa220a39af320p0,
    -0x1.0p101,
    0x1.0p100
  },
  { // Entry 326
    0x1.921fb54442d18469898cc51701b839a2p-1,
    0x1.0p101,
    0x1.0p101
  },
  { // Entry 327
    -0x1.921fb54442d18469898cc51701b839a2p-1,
    -0x1.0p101,
    0x1.0p101
  },
  { // Entry 328
    -0.0,
    -0x1.0p-1074,
    0x1.fffffffffffffp0
  },
  { // Entry 329
    0.0,
    0x1.0p-1074,
    0x1.fffffffffffffp0
  },
  { // Entry 330
    -0.0,
    -0x1.0p-1074,
    0x1.0p1
  },
  { // Entry 331
    0.0,
    0x1.0p-1074,
    0x1.0p1
  },
  { // Entry 332
    -0.0,
    -0x1.0p-1074,
    0x1.0000000000001p1
  },
  { // Entry 333
    0.0,
    0x1.0p-1074,
    0x1.0000000000001p1
  },
  { // Entry 334
    -0.0,
    -0.0,
    0x1.fffffffffffffp0
  },
  { // Entry 335
    -0.0,
    -0.0,
    0x1.0p1
  },
  { // Entry 336
    -0.0,
    -0.0,
    0x1.0000000000001p1
  },
  { // Entry 337
    0.0,
    0x1.0p-1074,
    0x1.fffffffffffffp0
  },
  { // Entry 338
    -0.0,
    -0x1.0p-1074,
    0x1.fffffffffffffp0
  },
  { // Entry 339
    0.0,
    0x1.0p-1074,
    0x1.0p1
  },
  { // Entry 340
    -0.0,
    -0x1.0p-1074,
    0x1.0p1
  },
  { // Entry 341
    0.0,
    0x1.0p-1074,
    0x1.0000000000001p1
  },
  { // Entry 342
    -0.0,
    -0x1.0p-1074,
    0x1.0000000000001p1
  },
  { // Entry 343
    0x1.ff55bb72cfde9c6d964f25b81c5c1aa2p-5,
    0x1.fffffffffffffp-4,
    0x1.fffffffffffffp0
  },
  { // Entry 344
    -0x1.ff55bb72cfde9c6d964f25b81c5c1aa2p-5,
    -0x1.fffffffffffffp-4,
    0x1.fffffffffffffp0
  },
  { // Entry 345
    0x1.ff55bb72cfde8c7d865f15c80c6b8bb0p-5,
    0x1.fffffffffffffp-4,
    0x1.0p1
  },
  { // Entry 346
    -0x1.ff55bb72cfde8c7d865f15c80c6b8bb0p-5,
    -0x1.fffffffffffffp-4,
    0x1.0p1
  },
  { // Entry 347
    0x1.ff55bb72cfde6c9d667ef5e7ef8476c1p-5,
    0x1.fffffffffffffp-4,
    0x1.0000000000001p1
  },
  { // Entry 348
    -0x1.ff55bb72cfde6c9d667ef5e7ef8476c1p-5,
    -0x1.fffffffffffffp-4,
    0x1.0000000000001p1
  },
  { // Entry 349
    0x1.ff55bb72cfdeac5da63f35a82ccb2c10p-5,
    0x1.0p-3,
    0x1.fffffffffffffp0
  },
  { // Entry 350
    -0x1.ff55bb72cfdeac5da63f35a82ccb2c10p-5,
    -0x1.0p-3,
    0x1.fffffffffffffp0
  },
  { // Entry 351
    0x1.ff55bb72cfde9c6d964f25b81c5c1aa2p-5,
    0x1.0p-3,
    0x1.0p1
  },
  { // Entry 352
    -0x1.ff55bb72cfde9c6d964f25b81c5c1aa2p-5,
    -0x1.0p-3,
    0x1.0p1
  },
  { // Entry 353
    0x1.ff55bb72cfde7c8d766f05d7fe7800bap-5,
    0x1.0p-3,
    0x1.0000000000001p1
  },
  { // Entry 354
    -0x1.ff55bb72cfde7c8d766f05d7fe7800bap-5,
    -0x1.0p-3,
    0x1.0000000000001p1
  },
  { // Entry 355
    0x1.ff55bb72cfdecc3dc61f55884da654e3p-5,
    0x1.0000000000001p-3,
    0x1.fffffffffffffp0
  },
  { // Entry 356
    -0x1.ff55bb72cfdecc3dc61f55884da654e3p-5,
    -0x1.0000000000001p-3,
    0x1.fffffffffffffp0
  },
  { // Entry 357
    0x1.ff55bb72cfdebc4db62f45983c3a3e7cp-5,
    0x1.0000000000001p-3,
    0x1.0p1
  },
  { // Entry 358
    -0x1.ff55bb72cfdebc4db62f45983c3a3e7cp-5,
    -0x1.0000000000001p-3,
    0x1.0p1
  },
  { // Entry 359
    0x1.ff55bb72cfde9c6d964f25b81c5c1aa2p-5,
    0x1.0000000000001p-3,
    0x1.0000000000001p1
  },
  { // Entry 360
    -0x1.ff55bb72cfde9c6d964f25b81c5c1aa2p-5,
    -0x1.0000000000001p-3,
    0x1.0000000000001p1
  },
  { // Entry 361
    -0x1.ff55bb72cfdecc3dc61f55884da654e3p-5,
    -0x1.0000000000001p-3,
    0x1.fffffffffffffp0
  },
  { // Entry 362
    0x1.ff55bb72cfdecc3dc61f55884da654e3p-5,
    0x1.0000000000001p-3,
    0x1.fffffffffffffp0
  },
  { // Entry 363
    -0x1.ff55bb72cfdebc4db62f45983c3a3e7cp-5,
    -0x1.0000000000001p-3,
    0x1.0p1
  },
  { // Entry 364
    0x1.ff55bb72cfdebc4db62f45983c3a3e7cp-5,
    0x1.0000000000001p-3,
    0x1.0p1
  },
  { // Entry 365
    -0x1.ff55bb72cfde9c6d964f25b81c5c1aa2p-5,
    -0x1.0000000000001p-3,
    0x1.0000000000001p1
  },
  { // Entry 366
    0x1.ff55bb72cfde9c6d964f25b81c5c1aa2p-5,
    0x1.0000000000001p-3,
    0x1.0000000000001p1
  },
  { // Entry 367
    -0x1.ff55bb72cfdeac5da63f35a82ccb2c10p-5,
    -0x1.0p-3,
    0x1.fffffffffffffp0
  },
  { // Entry 368
    0x1.ff55bb72cfdeac5da63f35a82ccb2c10p-5,
    0x1.0p-3,
    0x1.fffffffffffffp0
  },
  { // Entry 369
    -0x1.ff55bb72cfde9c6d964f25b81c5c1aa2p-5,
    -0x1.0p-3,
    0x1.0p1
  },
  { // Entry 370
    0x1.ff55bb72cfde9c6d964f25b81c5c1aa2p-5,
    0x1.0p-3,
    0x1.0p1
  },
  { // Entry 371
    -0x1.ff55bb72cfde7c8d766f05d7fe7800bap-5,
    -0x1.0p-3,
    0x1.0000000000001p1
  },
  { // Entry 372
    0x1.ff55bb72cfde7c8d766f05d7fe7800bap-5,
    0x1.0p-3,
    0x1.0000000000001p1
  },
  { // Entry 373
    -0x1.ff55bb72cfde9c6d964f25b81c5c1aa2p-5,
    -0x1.fffffffffffffp-4,
    0x1.fffffffffffffp0
  },
  { // Entry 374
    0x1.ff55bb72cfde9c6d964f25b81c5c1aa2p-5,
    0x1.fffffffffffffp-4,
    0x1.fffffffffffffp0
  },
  { // Entry 375
    -0x1.ff55bb72cfde8c7d865f15c80c6b8bb0p-5,
    -0x1.fffffffffffffp-4,
    0x1.0p1
  },
  { // Entry 376
    0x1.ff55bb72cfde8c7d865f15c80c6b8bb0p-5,
    0x1.fffffffffffffp-4,
    0x1.0p1
  },
  { // Entry 377
    -0x1.ff55bb72cfde6c9d667ef5e7ef8476c1p-5,
    -0x1.fffffffffffffp-4,
    0x1.0000000000001p1
  },
  { // Entry 378
    0x1.ff55bb72cfde6c9d667ef5e7ef8476c1p-5,
    0x1.fffffffffffffp-4,
    0x1.0000000000001p1
  },
  { // Entry 379
    0x1.a271f63e34fcf65ccd6e8d4040e2778cp-2,
    0x1.bb67ae8584ca9p-1,
    0x1.fffffffffffffp0
  },
  { // Entry 380
    -0x1.a271f63e34fcf65ccd6e8d4040e2778cp-2,
    -0x1.bb67ae8584ca9p-1,
    0x1.fffffffffffffp0
  },
  { // Entry 381
    0x1.a271f63e34fceab1a727ab70d0ce3998p-2,
    0x1.bb67ae8584ca9p-1,
    0x1.0p1
  },
  { // Entry 382
    -0x1.a271f63e34fceab1a727ab70d0ce3998p-2,
    -0x1.bb67ae8584ca9p-1,
    0x1.0p1
  },
  { // Entry 383
    0x1.a271f63e34fcd35b5a99e7d1f27d656cp-2,
    0x1.bb67ae8584ca9p-1,
    0x1.0000000000001p1
  },
  { // Entry 384
    -0x1.a271f63e34fcd35b5a99e7d1f27d656cp-2,
    -0x1.bb67ae8584ca9p-1,
    0x1.0000000000001p1
  },
  { // Entry 385
    0x1.a271f63e34fd03d610ccde17d587872dp-2,
    0x1.bb67ae8584caap-1,
    0x1.fffffffffffffp0
  },
  { // Entry 386
    -0x1.a271f63e34fd03d610ccde17d587872dp-2,
    -0x1.bb67ae8584caap-1,
    0x1.fffffffffffffp0
  },
  { // Entry 387
    0x1.a271f63e34fcf82aea85fc486529890cp-2,
    0x1.bb67ae8584caap-1,
    0x1.0p1
  },
  { // Entry 388
    -0x1.a271f63e34fcf82aea85fc486529890cp-2,
    -0x1.bb67ae8584caap-1,
    0x1.0p1
  },
  { // Entry 389
    0x1.a271f63e34fce0d49df838a986453485p-2,
    0x1.bb67ae8584caap-1,
    0x1.0000000000001p1
  },
  { // Entry 390
    -0x1.a271f63e34fce0d49df838a986453485p-2,
    -0x1.bb67ae8584caap-1,
    0x1.0000000000001p1
  },
  { // Entry 391
    0x1.a271f63e34fd114f542b2eef6a0548d3p-2,
    0x1.bb67ae8584cabp-1,
    0x1.fffffffffffffp0
  },
  { // Entry 392
    -0x1.a271f63e34fd114f542b2eef6a0548d3p-2,
    -0x1.bb67ae8584cabp-1,
    0x1.fffffffffffffp0
  },
  { // Entry 393
    0x1.a271f63e34fd05a42de44d1ff95d8a85p-2,
    0x1.bb67ae8584cabp-1,
    0x1.0p1
  },
  { // Entry 394
    -0x1.a271f63e34fd05a42de44d1ff95d8a85p-2,
    -0x1.bb67ae8584cabp-1,
    0x1.0p1
  },
  { // Entry 395
    0x1.a271f63e34fcee4de156898119e5b5a4p-2,
    0x1.bb67ae8584cabp-1,
    0x1.0000000000001p1
  },
  { // Entry 396
    -0x1.a271f63e34fcee4de156898119e5b5a4p-2,
    -0x1.bb67ae8584cabp-1,
    0x1.0000000000001p1
  },
  { // Entry 397
    -0x1.a271f63e34fd114f542b2eef6a0548d3p-2,
    -0x1.bb67ae8584cabp-1,
    0x1.fffffffffffffp0
  },
  { // Entry 398
    0x1.a271f63e34fd114f542b2eef6a0548d3p-2,
    0x1.bb67ae8584cabp-1,
    0x1.fffffffffffffp0
  },
  { // Entry 399
    -0x1.a271f63e34fd05a42de44d1ff95d8a85p-2,
    -0x1.bb67ae8584cabp-1,
    0x1.0p1
  },
  { // Entry 400
    0x1.a271f63e34fd05a42de44d1ff95d8a85p-2,
    0x1.bb67ae8584cabp-1,
    0x1.0p1
  },
  { // Entry 401
    -0x1.a271f63e34fcee4de156898119e5b5a4p-2,
    -0x1.bb67ae8584cabp-1,
    0x1.0000000000001p1
  },
  { // Entry 402
    0x1.a271f63e34fcee4de156898119e5b5a4p-2,
    0x1.bb67ae8584cabp-1,
    0x1.0000000000001p1
  },
  { // Entry 403
    -0x1.a271f63e34fd03d610ccde17d587872dp-2,
    -0x1.bb67ae8584caap-1,
    0x1.fffffffffffffp0
  },
  { // Entry 404
    0x1.a271f63e34fd03d610ccde17d587872dp-2,
    0x1.bb67ae8584caap-1,
    0x1.fffffffffffffp0
  },
  { // Entry 405
    -0x1.a271f63e34fcf82aea85fc486529890cp-2,
    -0x1.bb67ae8584caap-1,
    0x1.0p1
  },
  { // Entry 406
    0x1.a271f63e34fcf82aea85fc486529890cp-2,
    0x1.bb67ae8584caap-1,
    0x1.0p1
  },
  { // Entry 407
    -0x1.a271f63e34fce0d49df838a986453485p-2,
    -0x1.bb67ae8584caap-1,
    0x1.0000000000001p1
  },
  { // Entry 408
    0x1.a271f63e34fce0d49df838a986453485p-2,
    0x1.bb67ae8584caap-1,
    0x1.0000000000001p1
  },
  { // Entry 409
    -0x1.a271f63e34fcf65ccd6e8d4040e2778cp-2,
    -0x1.bb67ae8584ca9p-1,
    0x1.fffffffffffffp0
  },
  { // Entry 410
    0x1.a271f63e34fcf65ccd6e8d4040e2778cp-2,
    0x1.bb67ae8584ca9p-1,
    0x1.fffffffffffffp0
  },
  { // Entry 411
    -0x1.a271f63e34fceab1a727ab70d0ce3998p-2,
    -0x1.bb67ae8584ca9p-1,
    0x1.0p1
  },
  { // Entry 412
    0x1.a271f63e34fceab1a727ab70d0ce3998p-2,
    0x1.bb67ae8584ca9p-1,
    0x1.0p1
  },
  { // Entry 413
    -0x1.a271f63e34fcd35b5a99e7d1f27d656cp-2,
    -0x1.bb67ae8584ca9p-1,
    0x1.0000000000001p1
  },
  { // Entry 414
    0x1.a271f63e34fcd35b5a99e7d1f27d656cp-2,
    0x1.bb67ae8584ca9p-1,
    0x1.0000000000001p1
  },
  { // Entry 415
    0x1.e1fc084cd761a3aa77e521b9a803b2f2p-1,
    0x1.5f89e11a0441cp1,
    0x1.fffffffffffffp0
  },
  { // Entry 416
    -0x1.e1fc084cd761a3aa77e521b9a803b2f2p-1,
    -0x1.5f89e11a0441cp1,
    0x1.fffffffffffffp0
  },
  { // Entry 417
    0x1.e1fc084cd7619c0d50916d35af40b669p-1,
    0x1.5f89e11a0441cp1,
    0x1.0p1
  },
  { // Entry 418
    -0x1.e1fc084cd7619c0d50916d35af40b669p-1,
    -0x1.5f89e11a0441cp1,
    0x1.0p1
  },
  { // Entry 419
    0x1.e1fc084cd7618cd301ea042dbe396361p-1,
    0x1.5f89e11a0441cp1,
    0x1.0000000000001p1
  },
  { // Entry 420
    -0x1.e1fc084cd7618cd301ea042dbe396361p-1,
    -0x1.5f89e11a0441cp1,
    0x1.0000000000001p1
  },
  { // Entry 421
    0x1.e1fc084cd761aec15275b5d29ba21a09p-1,
    0x1.5f89e11a0441dp1,
    0x1.fffffffffffffp0
  },
  { // Entry 422
    -0x1.e1fc084cd761aec15275b5d29ba21a09p-1,
    -0x1.5f89e11a0441dp1,
    0x1.fffffffffffffp0
  },
  { // Entry 423
    0x1.e1fc084cd761a7242b22014ea2fa57ebp-1,
    0x1.5f89e11a0441dp1,
    0x1.0p1
  },
  { // Entry 424
    -0x1.e1fc084cd761a7242b22014ea2fa57ebp-1,
    -0x1.5f89e11a0441dp1,
    0x1.0p1
  },
  { // Entry 425
    0x1.e1fc084cd76197e9dc7a9846b22979b9p-1,
    0x1.5f89e11a0441dp1,
    0x1.0000000000001p1
  },
  { // Entry 426
    -0x1.e1fc084cd76197e9dc7a9846b22979b9p-1,
    -0x1.5f89e11a0441dp1,
    0x1.0000000000001p1
  },
  { // Entry 427
    0x1.e1fc084cd761b9d82d0649eb8e97a3bcp-1,
    0x1.5f89e11a0441ep1,
    0x1.fffffffffffffp0
  },
  { // Entry 428
    -0x1.e1fc084cd761b9d82d0649eb8e97a3bcp-1,
    -0x1.5f89e11a0441ep1,
    0x1.fffffffffffffp0
  },
  { // Entry 429
    0x1.e1fc084cd761b23b05b29567960b1c09p-1,
    0x1.5f89e11a0441ep1,
    0x1.0p1
  },
  { // Entry 430
    -0x1.e1fc084cd761b23b05b29567960b1c09p-1,
    -0x1.5f89e11a0441ep1,
    0x1.0p1
  },
  { // Entry 431
    0x1.e1fc084cd761a300b70b2c5fa570b2aep-1,
    0x1.5f89e11a0441ep1,
    0x1.0000000000001p1
  },
  { // Entry 432
    -0x1.e1fc084cd761a300b70b2c5fa570b2aep-1,
    -0x1.5f89e11a0441ep1,
    0x1.0000000000001p1
  },
  { // Entry 433
    -0x1.e1fc084cd761b9d82d0649eb8e97a3bcp-1,
    -0x1.5f89e11a0441ep1,
    0x1.fffffffffffffp0
  },
  { // Entry 434
    0x1.e1fc084cd761b9d82d0649eb8e97a3bcp-1,
    0x1.5f89e11a0441ep1,
    0x1.fffffffffffffp0
  },
  { // Entry 435
    -0x1.e1fc084cd761b23b05b29567960b1c09p-1,
    -0x1.5f89e11a0441ep1,
    0x1.0p1
  },
  { // Entry 436
    0x1.e1fc084cd761b23b05b29567960b1c09p-1,
    0x1.5f89e11a0441ep1,
    0x1.0p1
  },
  { // Entry 437
    -0x1.e1fc084cd761a300b70b2c5fa570b2aep-1,
    -0x1.5f89e11a0441ep1,
    0x1.0000000000001p1
  },
  { // Entry 438
    0x1.e1fc084cd761a300b70b2c5fa570b2aep-1,
    0x1.5f89e11a0441ep1,
    0x1.0000000000001p1
  },
  { // Entry 439
    -0x1.e1fc084cd761aec15275b5d29ba21a09p-1,
    -0x1.5f89e11a0441dp1,
    0x1.fffffffffffffp0
  },
  { // Entry 440
    0x1.e1fc084cd761aec15275b5d29ba21a09p-1,
    0x1.5f89e11a0441dp1,
    0x1.fffffffffffffp0
  },
  { // Entry 441
    -0x1.e1fc084cd761a7242b22014ea2fa57ebp-1,
    -0x1.5f89e11a0441dp1,
    0x1.0p1
  },
  { // Entry 442
    0x1.e1fc084cd761a7242b22014ea2fa57ebp-1,
    0x1.5f89e11a0441dp1,
    0x1.0p1
  },
  { // Entry 443
    -0x1.e1fc084cd76197e9dc7a9846b22979b9p-1,
    -0x1.5f89e11a0441dp1,
    0x1.0000000000001p1
  },
  { // Entry 444
    0x1.e1fc084cd76197e9dc7a9846b22979b9p-1,
    0x1.5f89e11a0441dp1,
    0x1.0000000000001p1
  },
  { // Entry 445
    -0x1.e1fc084cd761a3aa77e521b9a803b2f2p-1,
    -0x1.5f89e11a0441cp1,
    0x1.fffffffffffffp0
  },
  { // Entry 446
    0x1.e1fc084cd761a3aa77e521b9a803b2f2p-1,
    0x1.5f89e11a0441cp1,
    0x1.fffffffffffffp0
  },
  { // Entry 447
    -0x1.e1fc084cd7619c0d50916d35af40b669p-1,
    -0x1.5f89e11a0441cp1,
    0x1.0p1
  },
  { // Entry 448
    0x1.e1fc084cd7619c0d50916d35af40b669p-1,
    0x1.5f89e11a0441cp1,
    0x1.0p1
  },
  { // Entry 449
    -0x1.e1fc084cd7618cd301ea042dbe396361p-1,
    -0x1.5f89e11a0441cp1,
    0x1.0000000000001p1
  },
  { // Entry 450
    0x1.e1fc084cd7618cd301ea042dbe396361p-1,
    0x1.5f89e11a0441cp1,
    0x1.0000000000001p1
  },
  { // Entry 451
    0x1.921fb54442d18469898cc51701b839a2p-1,
    0x1.fffffffffffffp0,
    0x1.fffffffffffffp0
  },
  { // Entry 452
    -0x1.921fb54442d18469898cc51701b839a2p-1,
    -0x1.fffffffffffffp0,
    0x1.fffffffffffffp0
  },
  { // Entry 453
    0x1.921fb54442d17c69898cc517019839a2p-1,
    0x1.fffffffffffffp0,
    0x1.0p1
  },
  { // Entry 454
    -0x1.921fb54442d17c69898cc517019839a2p-1,
    -0x1.fffffffffffffp0,
    0x1.0p1
  },
  { // Entry 455
    0x1.921fb54442d16c69898cc517021839a2p-1,
    0x1.fffffffffffffp0,
    0x1.0000000000001p1
  },
  { // Entry 456
    -0x1.921fb54442d16c69898cc517021839a2p-1,
    -0x1.fffffffffffffp0,
    0x1.0000000000001p1
  },
  { // Entry 457
    0x1.921fb54442d18c69898cc51701d839a2p-1,
    0x1.0p1,
    0x1.fffffffffffffp0
  },
  { // Entry 458
    -0x1.921fb54442d18c69898cc51701d839a2p-1,
    -0x1.0p1,
    0x1.fffffffffffffp0
  },
  { // Entry 459
    0x1.921fb54442d18469898cc51701b839a2p-1,
    0x1.0p1,
    0x1.0p1
  },
  { // Entry 460
    -0x1.921fb54442d18469898cc51701b839a2p-1,
    -0x1.0p1,
    0x1.0p1
  },
  { // Entry 461
    0x1.921fb54442d17469898cc517023839a2p-1,
    0x1.0p1,
    0x1.0000000000001p1
  },
  { // Entry 462
    -0x1.921fb54442d17469898cc517023839a2p-1,
    -0x1.0p1,
    0x1.0000000000001p1
  },
  { // Entry 463
    0x1.921fb54442d19c69898cc517015839a2p-1,
    0x1.0000000000001p1,
    0x1.fffffffffffffp0
  },
  { // Entry 464
    -0x1.921fb54442d19c69898cc517015839a2p-1,
    -0x1.0000000000001p1,
    0x1.fffffffffffffp0
  },
  { // Entry 465
    0x1.921fb54442d19469898cc517013839a2p-1,
    0x1.0000000000001p1,
    0x1.0p1
  },
  { // Entry 466
    -0x1.921fb54442d19469898cc517013839a2p-1,
    -0x1.0000000000001p1,
    0x1.0p1
  },
  { // Entry 467
    0x1.921fb54442d18469898cc51701b839a2p-1,
    0x1.0000000000001p1,
    0x1.0000000000001p1
  },
  { // Entry 468
    -0x1.921fb54442d18469898cc51701b839a2p-1,
    -0x1.0000000000001p1,
    0x1.0000000000001p1
  },
  { // Entry 469
    0x1.fffffffffffffd5555555555555bbbbbp-28,
    0x1.fffffffffffffp-27,
    0x1.fffffffffffffp0
  },
  { // Entry 470
    -0x1.fffffffffffffd5555555555555bbbbbp-28,
    -0x1.fffffffffffffp-27,
    0x1.fffffffffffffp0
  },
  { // Entry 471
    0x1.ffffffffffffed5555555555559bbbbbp-28,
    0x1.fffffffffffffp-27,
    0x1.0p1
  },
  { // Entry 472
    -0x1.ffffffffffffed5555555555559bbbbbp-28,
    -0x1.fffffffffffffp-27,
    0x1.0p1
  },
  { // Entry 473
    0x1.ffffffffffffcd5555555555591bbbbbp-28,
    0x1.fffffffffffffp-27,
    0x1.0000000000001p1
  },
  { // Entry 474
    -0x1.ffffffffffffcd5555555555591bbbbbp-28,
    -0x1.fffffffffffffp-27,
    0x1.0000000000001p1
  },
  { // Entry 475
    0x1.00000000000006aaaaaaaaaaaacdddddp-27,
    0x1.0p-26,
    0x1.fffffffffffffp0
  },
  { // Entry 476
    -0x1.00000000000006aaaaaaaaaaaacdddddp-27,
    -0x1.0p-26,
    0x1.fffffffffffffp0
  },
  { // Entry 477
    0x1.fffffffffffffd5555555555555bbbbbp-28,
    0x1.0p-26,
    0x1.0p1
  },
  { // Entry 478
    -0x1.fffffffffffffd5555555555555bbbbbp-28,
    -0x1.0p-26,
    0x1.0p1
  },
  { // Entry 479
    0x1.ffffffffffffdd555555555557dbbbbbp-28,
    0x1.0p-26,
    0x1.0000000000001p1
  },
  { // Entry 480
    -0x1.ffffffffffffdd555555555557dbbbbbp-28,
    -0x1.0p-26,
    0x1.0000000000001p1
  },
  { // Entry 481
    0x1.00000000000016aaaaaaaaaaab0dddddp-27,
    0x1.0000000000001p-26,
    0x1.fffffffffffffp0
  },
  { // Entry 482
    -0x1.00000000000016aaaaaaaaaaab0dddddp-27,
    -0x1.0000000000001p-26,
    0x1.fffffffffffffp0
  },
  { // Entry 483
    0x1.0000000000000eaaaaaaaaaaaa6dddddp-27,
    0x1.0000000000001p-26,
    0x1.0p1
  },
  { // Entry 484
    -0x1.0000000000000eaaaaaaaaaaaa6dddddp-27,
    -0x1.0000000000001p-26,
    0x1.0p1
  },
  { // Entry 485
    0x1.fffffffffffffd5555555555555bbbbbp-28,
    0x1.0000000000001p-26,
    0x1.0000000000001p1
  },
  { // Entry 486
    -0x1.fffffffffffffd5555555555555bbbbbp-28,
    -0x1.0000000000001p-26,
    0x1.0000000000001p1
  },
  { // Entry 487
    -0x1.2d97c7f3321d234f272993d1414a2b39p1,
    -0x1.0p-1074,
    -0x1.0p-1074
  },
  { // Entry 488
    0x1.2d97c7f3321d234f272993d1414a2b39p1,
    0x1.0p-1074,
    -0x1.0p-1074
  },
  { // Entry 489
    -0x1.921fb54442d18469898cc51701b839a2p0,
    -0x1.0p-1074,
    -0.0
  },
  { // Entry 490
    0x1.921fb54442d18469898cc51701b839a2p0,
    0x1.0p-1074,
    -0.0
  },
  { // Entry 491
    -0x1.921fb54442d18469898cc51701b839a2p-1,
    -0x1.0p-1074,
    0x1.0p-1074
  },
  { // Entry 492
    0x1.921fb54442d18469898cc51701b839a2p-1,
    0x1.0p-1074,
    0x1.0p-1074
  },
  { // Entry 493
    -0x1.921fb54442d18469898cc51701b839a2p1,
    -0.0,
    -0x1.0p-1074
  },
  { // Entry 494
    -0x1.921fb54442d18469898cc51701b839a2p1,
    -0.0,
    -0.0
  },
  { // Entry 495
    -0.0,
    -0.0,
    0x1.0p-1074
  },
  { // Entry 496
    0x1.2d97c7f3321d234f272993d1414a2b39p1,
    0x1.0p-1074,
    -0x1.0p-1074
  },
  { // Entry 497
    -0x1.2d97c7f3321d234f272993d1414a2b39p1,
    -0x1.0p-1074,
    -0x1.0p-1074
  },
  { // Entry 498
    0x1.921fb54442d18469898cc51701b839a2p0,
    0x1.0p-1074,
    -0.0
  },
  { // Entry 499
    -0x1.921fb54442d18469898cc51701b839a2p0,
    -0x1.0p-1074,
    -0.0
  },
  { // Entry 500
    0x1.921fb54442d18469898cc51701b839a2p-1,
    0x1.0p-1074,
    0x1.0p-1074
  },
  { // Entry 501
    -0x1.921fb54442d18469898cc51701b839a2p-1,
    -0x1.0p-1074,
    0x1.0p-1074
  },
  { // Entry 502
    -0.0,
    -0x1.0p-1074,
    0x1.fffffffffffffp1023
  },
  { // Entry 503
    0.0,
    0x1.0p-1074,
    0x1.fffffffffffffp1023
  },
  { // Entry 504
    -0.0,
    -0.0,
    0x1.fffffffffffffp1023
  },
  { // Entry 505
    0.0,
    0x1.0p-1074,
    0x1.fffffffffffffp1023
  },
  { // Entry 506
    -0.0,
    -0x1.0p-1074,
    0x1.fffffffffffffp1023
  },
  { // Entry 507
    -0x1.921fb54442d18469898cc51701b839a2p1,
    -0x1.0p-1074,
    -0x1.fffffffffffffp1023
  },
  { // Entry 508
    0x1.921fb54442d18469898cc51701b839a2p1,
    0x1.0p-1074,
    -0x1.fffffffffffffp1023
  },
  { // Entry 509
    -0x1.921fb54442d18469898cc51701b839a2p1,
    -0.0,
    -0x1.fffffffffffffp1023
  },
  { // Entry 510
    0x1.921fb54442d18469898cc51701b839a2p1,
    0x1.0p-1074,
    -0x1.fffffffffffffp1023
  },
  { // Entry 511
    -0x1.921fb54442d18469898cc51701b839a2p1,
    -0x1.0p-1074,
    -0x1.fffffffffffffp1023
  },
  { // Entry 512
    0x1.921fb54442d18469898cc51701b839a2p0,
    0x1.fffffffffffffp1023,
    -0x1.0p-1074
  },
  { // Entry 513
    -0x1.921fb54442d18469898cc51701b839a2p0,
    -0x1.fffffffffffffp1023,
    -0x1.0p-1074
  },
  { // Entry 514
    0x1.921fb54442d18469898cc51701b839a2p0,
    0x1.fffffffffffffp1023,
    -0.0
  },
  { // Entry 515
    -0x1.921fb54442d18469898cc51701b839a2p0,
    -0x1.fffffffffffffp1023,
    -0.0
  },
  { // Entry 516
    0x1.921fb54442d18469898cc51701b839a2p0,
    0x1.fffffffffffffp1023,
    0x1.0p-1074
  },
  { // Entry 517
    -0x1.921fb54442d18469898cc51701b839a2p0,
    -0x1.fffffffffffffp1023,
    0x1.0p-1074
  },
  { // Entry 518
    -0x1.921fb54442d18469898cc51701b839a2p0,
    -0x1.fffffffffffffp1023,
    -0x1.0p-1074
  },
  { // Entry 519
    0x1.921fb54442d18469898cc51701b839a2p0,
    0x1.fffffffffffffp1023,
    -0x1.0p-1074
  },
  { // Entry 520
    -0x1.921fb54442d18469898cc51701b839a2p0,
    -0x1.fffffffffffffp1023,
    -0.0
  },
  { // Entry 521
    0x1.921fb54442d18469898cc51701b839a2p0,
    0x1.fffffffffffffp1023,
    -0.0
  },
  { // Entry 522
    -0x1.921fb54442d18469898cc51701b839a2p0,
    -0x1.fffffffffffffp1023,
    0x1.0p-1074
  },
  { // Entry 523
    0x1.921fb54442d18469898cc51701b839a2p0,
    0x1.fffffffffffffp1023,
    0x1.0p-1074
  },
  { // Entry 524
    0x1.921fb54442d18469898cc51701b839a2p-1,
    0x1.fffffffffffffp1023,
    0x1.fffffffffffffp1023
  },
  { // Entry 525
    -0x1.921fb54442d18469898cc51701b839a2p-1,
    -0x1.fffffffffffffp1023,
    0x1.fffffffffffffp1023
  },
  { // Entry 526
    -0x1.2d97c7f3321d234f272993d1414a2b39p1,
    -0x1.fffffffffffffp1023,
    -0x1.fffffffffffffp1023
  },
  { // Entry 527
    0x1.2d97c7f3321d234f272993d1414a2b39p1,
    0x1.fffffffffffffp1023,
    -0x1.fffffffffffffp1023
  },
  { // Entry 528
    0x1.ffffffffffffffffffffffffffff5555p-57,
    0x1.fffffffffffffp0,
    0x1.fffffffffffffp56
  },
  { // Entry 529
    -0x1.ffffffffffffffffffffffffffff5555p-57,
    -0x1.fffffffffffffp0,
    0x1.fffffffffffffp56
  },
  { // Entry 530
    0x1.ffffffffffffefffffffffffffff5555p-57,
    0x1.fffffffffffffp0,
    0x1.0p57
  },
  { // Entry 531
    -0x1.ffffffffffffefffffffffffffff5555p-57,
    -0x1.fffffffffffffp0,
    0x1.0p57
  },
  { // Entry 532
    0x1.ffffffffffffd0000000000002ff5555p-57,
    0x1.fffffffffffffp0,
    0x1.0000000000001p57
  },
  { // Entry 533
    -0x1.ffffffffffffd0000000000002ff5555p-57,
    -0x1.fffffffffffffp0,
    0x1.0000000000001p57
  },
  { // Entry 534
    0x1.000000000000080000000000003faaaap-56,
    0x1.0p1,
    0x1.fffffffffffffp56
  },
  { // Entry 535
    -0x1.000000000000080000000000003faaaap-56,
    -0x1.0p1,
    0x1.fffffffffffffp56
  },
  { // Entry 536
    0x1.ffffffffffffffffffffffffffff5555p-57,
    0x1.0p1,
    0x1.0p57
  },
  { // Entry 537
    -0x1.ffffffffffffffffffffffffffff5555p-57,
    -0x1.0p1,
    0x1.0p57
  },
  { // Entry 538
    0x1.ffffffffffffe0000000000001ff5555p-57,
    0x1.0p1,
    0x1.0000000000001p57
  },
  { // Entry 539
    -0x1.ffffffffffffe0000000000001ff5555p-57,
    -0x1.0p1,
    0x1.0000000000001p57
  },
  { // Entry 540
    0x1.00000000000018000000000000bfaaaap-56,
    0x1.0000000000001p1,
    0x1.fffffffffffffp56
  },
  { // Entry 541
    -0x1.00000000000018000000000000bfaaaap-56,
    -0x1.0000000000001p1,
    0x1.fffffffffffffp56
  },
  { // Entry 542
    0x1.0000000000000fffffffffffffffaaaap-56,
    0x1.0000000000001p1,
    0x1.0p57
  },
  { // Entry 543
    -0x1.0000000000000fffffffffffffffaaaap-56,
    -0x1.0000000000001p1,
    0x1.0p57
  },
  { // Entry 544
    0x1.ffffffffffffffffffffffffffff5555p-57,
    0x1.0000000000001p1,
    0x1.0000000000001p57
  },
  { // Entry 545
    -0x1.ffffffffffffffffffffffffffff5555p-57,
    -0x1.0000000000001p1,
    0x1.0000000000001p57
  },
  { // Entry 546
    -0x1.00000000000018000000000000bfaaaap-56,
    -0x1.0000000000001p1,
    0x1.fffffffffffffp56
  },
  { // Entry 547
    0x1.00000000000018000000000000bfaaaap-56,
    0x1.0000000000001p1,
    0x1.fffffffffffffp56
  },
  { // Entry 548
    -0x1.0000000000000fffffffffffffffaaaap-56,
    -0x1.0000000000001p1,
    0x1.0p57
  },
  { // Entry 549
    0x1.0000000000000fffffffffffffffaaaap-56,
    0x1.0000000000001p1,
    0x1.0p57
  },
  { // Entry 550
    -0x1.ffffffffffffffffffffffffffff5555p-57,
    -0x1.0000000000001p1,
    0x1.0000000000001p57
  },
  { // Entry 551
    0x1.ffffffffffffffffffffffffffff5555p-57,
    0x1.0000000000001p1,
    0x1.0000000000001p57
  },
  { // Entry 552
    -0x1.000000000000080000000000003faaaap-56,
    -0x1.0p1,
    0x1.fffffffffffffp56
  },
  { // Entry 553
    0x1.000000000000080000000000003faaaap-56,
    0x1.0p1,
    0x1.fffffffffffffp56
  },
  { // Entry 554
    -0x1.ffffffffffffffffffffffffffff5555p-57,
    -0x1.0p1,
    0x1.0p57
  },
  { // Entry 555
    0x1.ffffffffffffffffffffffffffff5555p-57,
    0x1.0p1,
    0x1.0p57
  },
  { // Entry 556
    -0x1.ffffffffffffe0000000000001ff5555p-57,
    -0x1.0p1,
    0x1.0000000000001p57
  },
  { // Entry 557
    0x1.ffffffffffffe0000000000001ff5555p-57,
    0x1.0p1,
    0x1.0000000000001p57
  },
  { // Entry 558
    -0x1.ffffffffffffffffffffffffffff5555p-57,
    -0x1.fffffffffffffp0,
    0x1.fffffffffffffp56
  },
  { // Entry 559
    0x1.ffffffffffffffffffffffffffff5555p-57,
    0x1.fffffffffffffp0,
    0x1.fffffffffffffp56
  },
  { // Entry 560
    -0x1.ffffffffffffefffffffffffffff5555p-57,
    -0x1.fffffffffffffp0,
    0x1.0p57
  },
  { // Entry 561
    0x1.ffffffffffffefffffffffffffff5555p-57,
    0x1.fffffffffffffp0,
    0x1.0p57
  },
  { // Entry 562
    -0x1.ffffffffffffd0000000000002ff5555p-57,
    -0x1.fffffffffffffp0,
    0x1.0000000000001p57
  },
  { // Entry 563
    0x1.ffffffffffffd0000000000002ff5555p-57,
    0x1.fffffffffffffp0,
    0x1.0000000000001p57
  },
  { // Entry 564
    0x1.fffffffffffffff555555555555555bbp-31,
    0x1.fffffffffffffp1,
    0x1.fffffffffffffp31
  },
  { // Entry 565
    -0x1.fffffffffffffff555555555555555bbp-31,
    -0x1.fffffffffffffp1,
    0x1.fffffffffffffp31
  },
  { // Entry 566
    0x1.ffffffffffffeff555555555555655bbp-31,
    0x1.fffffffffffffp1,
    0x1.0p32
  },
  { // Entry 567
    -0x1.ffffffffffffeff555555555555655bbp-31,
    -0x1.fffffffffffffp1,
    0x1.0p32
  },
  { // Entry 568
    0x1.ffffffffffffcff555555555585855bbp-31,
    0x1.fffffffffffffp1,
    0x1.0000000000001p32
  },
  { // Entry 569
    -0x1.ffffffffffffcff555555555585855bbp-31,
    -0x1.fffffffffffffp1,
    0x1.0000000000001p32
  },
  { // Entry 570
    0x1.00000000000007faaaaaaaaaaaea2addp-30,
    0x1.0p2,
    0x1.fffffffffffffp31
  },
  { // Entry 571
    -0x1.00000000000007faaaaaaaaaaaea2addp-30,
    -0x1.0p2,
    0x1.fffffffffffffp31
  },
  { // Entry 572
    0x1.fffffffffffffff555555555555555bbp-31,
    0x1.0p2,
    0x1.0p32
  },
  { // Entry 573
    -0x1.fffffffffffffff555555555555555bbp-31,
    -0x1.0p2,
    0x1.0p32
  },
  { // Entry 574
    0x1.ffffffffffffdff555555555575755bbp-31,
    0x1.0p2,
    0x1.0000000000001p32
  },
  { // Entry 575
    -0x1.ffffffffffffdff555555555575755bbp-31,
    -0x1.0p2,
    0x1.0000000000001p32
  },
  { // Entry 576
    0x1.00000000000017faaaaaaaaaab692addp-30,
    0x1.0000000000001p2,
    0x1.fffffffffffffp31
  },
  { // Entry 577
    -0x1.00000000000017faaaaaaaaaab692addp-30,
    -0x1.0000000000001p2,
    0x1.fffffffffffffp31
  },
  { // Entry 578
    0x1.0000000000000ffaaaaaaaaaaaa9aaddp-30,
    0x1.0000000000001p2,
    0x1.0p32
  },
  { // Entry 579
    -0x1.0000000000000ffaaaaaaaaaaaa9aaddp-30,
    -0x1.0000000000001p2,
    0x1.0p32
  },
  { // Entry 580
    0x1.fffffffffffffff555555555555555bbp-31,
    0x1.0000000000001p2,
    0x1.0000000000001p32
  },
  { // Entry 581
    -0x1.fffffffffffffff555555555555555bbp-31,
    -0x1.0000000000001p2,
    0x1.0000000000001p32
  },
  { // Entry 582
    -0x1.00000000000017faaaaaaaaaab692addp-30,
    -0x1.0000000000001p-2,
    0x1.fffffffffffffp27
  },
  { // Entry 583
    0x1.00000000000017faaaaaaaaaab692addp-30,
    0x1.0000000000001p-2,
    0x1.fffffffffffffp27
  },
  { // Entry 584
    -0x1.0000000000000ffaaaaaaaaaaaa9aaddp-30,
    -0x1.0000000000001p-2,
    0x1.0p28
  },
  { // Entry 585
    0x1.0000000000000ffaaaaaaaaaaaa9aaddp-30,
    0x1.0000000000001p-2,
    0x1.0p28
  },
  { // Entry 586
    -0x1.fffffffffffffff555555555555555bbp-31,
    -0x1.0000000000001p-2,
    0x1.0000000000001p28
  },
  { // Entry 587
    0x1.fffffffffffffff555555555555555bbp-31,
    0x1.0000000000001p-2,
    0x1.0000000000001p28
  },
  { // Entry 588
    -0x1.00000000000007faaaaaaaaaaaea2addp-30,
    -0x1.0p-2,
    0x1.fffffffffffffp27
  },
  { // Entry 589
    0x1.00000000000007faaaaaaaaaaaea2addp-30,
    0x1.0p-2,
    0x1.fffffffffffffp27
  },
  { // Entry 590
    -0x1.fffffffffffffff555555555555555bbp-31,
    -0x1.0p-2,
    0x1.0p28
  },
  { // Entry 591
    0x1.fffffffffffffff555555555555555bbp-31,
    0x1.0p-2,
    0x1.0p28
  },
  { // Entry 592
    -0x1.ffffffffffffdff555555555575755bbp-31,
    -0x1.0p-2,
    0x1.0000000000001p28
  },
  { // Entry 593
    0x1.ffffffffffffdff555555555575755bbp-31,
    0x1.0p-2,
    0x1.0000000000001p28
  },
  { // Entry 594
    -0x1.fffffffffffffff555555555555555bbp-31,
    -0x1.fffffffffffffp-3,
    0x1.fffffffffffffp27
  },
  { // Entry 595
    0x1.fffffffffffffff555555555555555bbp-31,
    0x1.fffffffffffffp-3,
    0x1.fffffffffffffp27
  },
  { // Entry 596
    -0x1.ffffffffffffeff555555555555655bbp-31,
    -0x1.fffffffffffffp-3,
    0x1.0p28
  },
  { // Entry 597
    0x1.ffffffffffffeff555555555555655bbp-31,
    0x1.fffffffffffffp-3,
    0x1.0p28
  },
  { // Entry 598
    -0x1.ffffffffffffcff555555555585855bbp-31,
    -0x1.fffffffffffffp-3,
    0x1.0000000000001p28
  },
  { // Entry 599
    0x1.ffffffffffffcff555555555585855bbp-31,
    0x1.fffffffffffffp-3,
    0x1.0000000000001p28
  },
  { // Entry 600
    0x1.921fb54442d18469898cc51701b839a2p-1,
    0x1.fffffffffffffp2,
    0x1.fffffffffffffp2
  },
  { // Entry 601
    -0x1.921fb54442d18469898cc51701b839a2p-1,
    -0x1.fffffffffffffp2,
    0x1.fffffffffffffp2
  },
  { // Entry 602
    0x1.921fb54442d17c69898cc517019839a2p-1,
    0x1.fffffffffffffp2,
    0x1.0p3
  },
  { // Entry 603
    -0x1.921fb54442d17c69898cc517019839a2p-1,
    -0x1.fffffffffffffp2,
    0x1.0p3
  },
  { // Entry 604
    0x1.921fb54442d16c69898cc517021839a2p-1,
    0x1.fffffffffffffp2,
    0x1.0000000000001p3
  },
  { // Entry 605
    -0x1.921fb54442d16c69898cc517021839a2p-1,
    -0x1.fffffffffffffp2,
    0x1.0000000000001p3
  },
  { // Entry 606
    0x1.921fb54442d18c69898cc51701d839a2p-1,
    0x1.0p3,
    0x1.fffffffffffffp2
  },
  { // Entry 607
    -0x1.921fb54442d18c69898cc51701d839a2p-1,
    -0x1.0p3,
    0x1.fffffffffffffp2
  },
  { // Entry 608
    0x1.921fb54442d18469898cc51701b839a2p-1,
    0x1.0p3,
    0x1.0p3
  },
  { // Entry 609
    -0x1.921fb54442d18469898cc51701b839a2p-1,
    -0x1.0p3,
    0x1.0p3
  },
  { // Entry 610
    0x1.921fb54442d17469898cc517023839a2p-1,
    0x1.0p3,
    0x1.0000000000001p3
  },
  { // Entry 611
    -0x1.921fb54442d17469898cc517023839a2p-1,
    -0x1.0p3,
    0x1.0000000000001p3
  },
  { // Entry 612
    0x1.921fb54442d19c69898cc517015839a2p-1,
    0x1.0000000000001p3,
    0x1.fffffffffffffp2
  },
  { // Entry 613
    -0x1.921fb54442d19c69898cc517015839a2p-1,
    -0x1.0000000000001p3,
    0x1.fffffffffffffp2
  },
  { // Entry 614
    0x1.921fb54442d19469898cc517013839a2p-1,
    0x1.0000000000001p3,
    0x1.0p3
  },
  { // Entry 615
    -0x1.921fb54442d19469898cc517013839a2p-1,
    -0x1.0000000000001p3,
    0x1.0p3
  },
  { // Entry 616
    0x1.921fb54442d18469898cc51701b839a2p-1,
    0x1.0000000000001p3,
    0x1.0000000000001p3
  },
  { // Entry 617
    -0x1.921fb54442d18469898cc51701b839a2p-1,
    -0x1.0000000000001p3,
    0x1.0000000000001p3
  },
  { // Entry 618
    -0x1.dac670561bb51cf1462ef23fdf5661b4p-2,
    -0x1.0000000000001p3,
    0x1.fffffffffffffp3
  },
  { // Entry 619
    0x1.dac670561bb51cf1462ef23fdf5661b4p-2,
    0x1.0000000000001p3,
    0x1.fffffffffffffp3
  },
  { // Entry 620
    -0x1.dac670561bb510247962257311bcc81bp-2,
    -0x1.0000000000001p3,
    0x1.0p4
  },
  { // Entry 621
    0x1.dac670561bb510247962257311bcc81bp-2,
    0x1.0000000000001p3,
    0x1.0p4
  },
  { // Entry 622
    -0x1.dac670561bb4f68adfc88bd978751a06p-2,
    -0x1.0000000000001p3,
    0x1.0000000000001p4
  },
  { // Entry 623
    0x1.dac670561bb4f68adfc88bd978751a06p-2,
    0x1.0000000000001p3,
    0x1.0000000000001p4
  },
  { // Entry 624
    -0x1.dac670561bb50357ac9558a64593d258p-2,
    -0x1.0p3,
    0x1.fffffffffffffp3
  },
  { // Entry 625
    0x1.dac670561bb50357ac9558a64593d258p-2,
    0x1.0p3,
    0x1.fffffffffffffp3
  },
  { // Entry 626
    -0x1.dac670561bb4f68adfc88bd978751a06p-2,
    -0x1.0p3,
    0x1.0p4
  },
  { // Entry 627
    0x1.dac670561bb4f68adfc88bd978751a06p-2,
    0x1.0p3,
    0x1.0p4
  },
  { // Entry 628
    -0x1.dac670561bb4dcf1462ef23fe0232e81p-2,
    -0x1.0p3,
    0x1.0000000000001p4
  },
  { // Entry 629
    0x1.dac670561bb4dcf1462ef23fe0232e81p-2,
    0x1.0p3,
    0x1.0000000000001p4
  },
  { // Entry 630
    -0x1.dac670561bb4f68adfc88bd978751a06p-2,
    -0x1.fffffffffffffp2,
    0x1.fffffffffffffp3
  },
  { // Entry 631
    0x1.dac670561bb4f68adfc88bd978751a06p-2,
    0x1.fffffffffffffp2,
    0x1.fffffffffffffp3
  },
  { // Entry 632
    -0x1.dac670561bb4e9be12fbbf0cab93d258p-2,
    -0x1.fffffffffffffp2,
    0x1.0p4
  },
  { // Entry 633
    0x1.dac670561bb4e9be12fbbf0cab93d258p-2,
    0x1.fffffffffffffp2,
    0x1.0p4
  },
  { // Entry 634
    -0x1.dac670561bb4d0247962257313bcc81bp-2,
    -0x1.fffffffffffffp2,
    0x1.0000000000001p4
  },
  { // Entry 635
    0x1.dac670561bb4d0247962257313bcc81bp-2,
    0x1.fffffffffffffp2,
    0x1.0000000000001p4
  },
  { // Entry 636
    0x1.72c43f4b1650a9d9aea6a40b156d98c0p1,
    0x1.fffffffffffffp2,
    -0x1.0000000000001p5
  },
  { // Entry 637
    -0x1.72c43f4b1650a9d9aea6a40b156d98c0p1,
    -0x1.fffffffffffffp2,
    -0x1.0000000000001p5
  },
  { // Entry 638
    0x1.72c43f4b1650a7f7ccc4c22933b558f8p1,
    0x1.fffffffffffffp2,
    -0x1.0p5
  },
  { // Entry 639
    -0x1.72c43f4b1650a7f7ccc4c22933b558f8p1,
    -0x1.fffffffffffffp2,
    -0x1.0p5
  },
  { // Entry 640
    0x1.72c43f4b1650a706dbd3d13842c3f6a5p1,
    0x1.fffffffffffffp2,
    -0x1.fffffffffffffp4
  },
  { // Entry 641
    -0x1.72c43f4b1650a706dbd3d13842c3f6a5p1,
    -0x1.fffffffffffffp2,
    -0x1.fffffffffffffp4
  },
  { // Entry 642
    0x1.72c43f4b1650a8e8bdb5b31a24897ff2p1,
    0x1.0p3,
    -0x1.0000000000001p5
  },
  { // Entry 643
    -0x1.72c43f4b1650a8e8bdb5b31a24897ff2p1,
    -0x1.0p3,
    -0x1.0000000000001p5
  },
  { // Entry 644
    0x1.72c43f4b1650a706dbd3d13842c3f6a5p1,
    0x1.0p3,
    -0x1.0p5
  },
  { // Entry 645
    -0x1.72c43f4b1650a706dbd3d13842c3f6a5p1,
    -0x1.0p3,
    -0x1.0p5
  },
  { // Entry 646
    0x1.72c43f4b1650a615eae2e04751cbef8fp1,
    0x1.0p3,
    -0x1.fffffffffffffp4
  },
  { // Entry 647
    -0x1.72c43f4b1650a615eae2e04751cbef8fp1,
    -0x1.0p3,
    -0x1.fffffffffffffp4
  },
  { // Entry 648
    0x1.72c43f4b1650a706dbd3d13842c3f6a5p1,
    0x1.0000000000001p3,
    -0x1.0000000000001p5
  },
  { // Entry 649
    -0x1.72c43f4b1650a706dbd3d13842c3f6a5p1,
    -0x1.0000000000001p3,
    -0x1.0000000000001p5
  },
  { // Entry 650
    0x1.72c43f4b1650a524f9f1ef5660e3da4dp1,
    0x1.0000000000001p3,
    -0x1.0p5
  },
  { // Entry 651
    -0x1.72c43f4b1650a524f9f1ef5660e3da4dp1,
    -0x1.0000000000001p3,
    -0x1.0p5
  },
  { // Entry 652
    0x1.72c43f4b1650a4340900fe656fde89b1p1,
    0x1.0000000000001p3,
    -0x1.fffffffffffffp4
  },
  { // Entry 653
    -0x1.72c43f4b1650a4340900fe656fde89b1p1,
    -0x1.0000000000001p3,
    -0x1.fffffffffffffp4
  },
  { // Entry 654
    0x1.fd5ba9aac2f6dc65912f313e7d111defp-4,
    0x1.fffffffffffffp2,
    0x1.fffffffffffffp5
  },
  { // Entry 655
    -0x1.fd5ba9aac2f6dc65912f313e7d111defp-4,
    -0x1.fffffffffffffp2,
    0x1.fffffffffffffp5
  },
  { // Entry 656
    0x1.fd5ba9aac2f6cca4951f70426d4e3181p-4,
    0x1.fffffffffffffp2,
    0x1.0p6
  },
  { // Entry 657
    -0x1.fd5ba9aac2f6cca4951f70426d4e3181p-4,
    -0x1.fffffffffffffp2,
    0x1.0p6
  },
  { // Entry 658
    0x1.fd5ba9aac2f6ad229cffee4a50b0e5b4p-4,
    0x1.fffffffffffffp2,
    0x1.0000000000001p6
  },
  { // Entry 659
    -0x1.fd5ba9aac2f6ad229cffee4a50b0e5b4p-4,
    -0x1.fffffffffffffp2,
    0x1.0000000000001p6
  },
  { // Entry 660
    0x1.fd5ba9aac2f6ec268d3ef23a8d4e3181p-4,
    0x1.0p3,
    0x1.fffffffffffffp5
  },
  { // Entry 661
    -0x1.fd5ba9aac2f6ec268d3ef23a8d4e3181p-4,
    -0x1.0p3,
    0x1.fffffffffffffp5
  },
  { // Entry 662
    0x1.fd5ba9aac2f6dc65912f313e7d111defp-4,
    0x1.0p3,
    0x1.0p6
  },
  { // Entry 663
    -0x1.fd5ba9aac2f6dc65912f313e7d111defp-4,
    -0x1.0p3,
    0x1.0p6
  },
  { // Entry 664
    0x1.fd5ba9aac2f6bce3990faf465f7f83d9p-4,
    0x1.0p3,
    0x1.0000000000001p6
  },
  { // Entry 665
    -0x1.fd5ba9aac2f6bce3990faf465f7f83d9p-4,
    -0x1.0p3,
    0x1.0000000000001p6
  },
  { // Entry 666
    0x1.fd5ba9aac2f70ba8855e7432adbcb671p-4,
    0x1.0000000000001p3,
    0x1.fffffffffffffp5
  },
  { // Entry 667
    -0x1.fd5ba9aac2f70ba8855e7432adbcb671p-4,
    -0x1.0000000000001p3,
    0x1.fffffffffffffp5
  },
  { // Entry 668
    0x1.fd5ba9aac2f6fbe7894eb3369c8b5496p-4,
    0x1.0000000000001p3,
    0x1.0p6
  },
  { // Entry 669
    -0x1.fd5ba9aac2f6fbe7894eb3369c8b5496p-4,
    -0x1.0000000000001p3,
    0x1.0p6
  },
  { // Entry 670
    0x1.fd5ba9aac2f6dc65912f313e7d111defp-4,
    0x1.0000000000001p3,
    0x1.0000000000001p6
  },
  { // Entry 671
    -0x1.fd5ba9aac2f6dc65912f313e7d111defp-4,
    -0x1.0000000000001p3,
    0x1.0000000000001p6
  },
  { // Entry 672
    -0x1.ff55bb72cfdecc3dc61f55884da654e3p-5,
    -0x1.0000000000001p3,
    0x1.fffffffffffffp6
  },
  { // Entry 673
    0x1.ff55bb72cfdecc3dc61f55884da654e3p-5,
    0x1.0000000000001p3,
    0x1.fffffffffffffp6
  },
  { // Entry 674
    -0x1.ff55bb72cfdebc4db62f45983c3a3e7cp-5,
    -0x1.0000000000001p3,
    0x1.0p7
  },
  { // Entry 675
    0x1.ff55bb72cfdebc4db62f45983c3a3e7cp-5,
    0x1.0000000000001p3,
    0x1.0p7
  },
  { // Entry 676
    -0x1.ff55bb72cfde9c6d964f25b81c5c1aa2p-5,
    -0x1.0000000000001p3,
    0x1.0000000000001p7
  },
  { // Entry 677
    0x1.ff55bb72cfde9c6d964f25b81c5c1aa2p-5,
    0x1.0000000000001p3,
    0x1.0000000000001p7
  },
  { // Entry 678
    -0x1.ff55bb72cfdeac5da63f35a82ccb2c10p-5,
    -0x1.0p3,
    0x1.fffffffffffffp6
  },
  { // Entry 679
    0x1.ff55bb72cfdeac5da63f35a82ccb2c10p-5,
    0x1.0p3,
    0x1.fffffffffffffp6
  },
  { // Entry 680
    -0x1.ff55bb72cfde9c6d964f25b81c5c1aa2p-5,
    -0x1.0p3,
    0x1.0p7
  },
  { // Entry 681
    0x1.ff55bb72cfde9c6d964f25b81c5c1aa2p-5,
    0x1.0p3,
    0x1.0p7
  },
  { // Entry 682
    -0x1.ff55bb72cfde7c8d766f05d7fe7800bap-5,
    -0x1.0p3,
    0x1.0000000000001p7
  },
  { // Entry 683
    0x1.ff55bb72cfde7c8d766f05d7fe7800bap-5,
    0x1.0p3,
    0x1.0000000000001p7
  },
  { // Entry 684
    -0x1.ff55bb72cfde9c6d964f25b81c5c1aa2p-5,
    -0x1.fffffffffffffp2,
    0x1.fffffffffffffp6
  },
  { // Entry 685
    0x1.ff55bb72cfde9c6d964f25b81c5c1aa2p-5,
    0x1.fffffffffffffp2,
    0x1.fffffffffffffp6
  },
  { // Entry 686
    -0x1.ff55bb72cfde8c7d865f15c80c6b8bb0p-5,
    -0x1.fffffffffffffp2,
    0x1.0p7
  },
  { // Entry 687
    0x1.ff55bb72cfde8c7d865f15c80c6b8bb0p-5,
    0x1.fffffffffffffp2,
    0x1.0p7
  },
  { // Entry 688
    -0x1.ff55bb72cfde6c9d667ef5e7ef8476c1p-5,
    -0x1.fffffffffffffp2,
    0x1.0000000000001p7
  },
  { // Entry 689
    0x1.ff55bb72cfde6c9d667ef5e7ef8476c1p-5,
    0x1.fffffffffffffp2,
    0x1.0000000000001p7
  },
  { // Entry 690
    0x1.0468a8ace4df65d2ed8c40d37cc6e907p1,
    0x1.fffffffffffffp2,
    -0x1.0000000000001p2
  },
  { // Entry 691
    -0x1.0468a8ace4df65d2ed8c40d37cc6e907p1,
    -0x1.fffffffffffffp2,
    -0x1.0000000000001p2
  },
  { // Entry 692
    0x1.0468a8ace4df629fba590da0498e971cp1,
    0x1.fffffffffffffp2,
    -0x1.0p2
  },
  { // Entry 693
    -0x1.0468a8ace4df629fba590da0498e971cp1,
    -0x1.fffffffffffffp2,
    -0x1.0p2
  },
  { // Entry 694
    0x1.0468a8ace4df610620bf7406afeac012p1,
    0x1.fffffffffffffp2,
    -0x1.fffffffffffffp1
  },
  { // Entry 695
    -0x1.0468a8ace4df610620bf7406afeac012p1,
    -0x1.fffffffffffffp2,
    -0x1.fffffffffffffp1
  },
  { // Entry 696
    0x1.0468a8ace4df643953f2a739e313b5d4p1,
    0x1.0p3,
    -0x1.0000000000001p2
  },
  { // Entry 697
    -0x1.0468a8ace4df643953f2a739e313b5d4p1,
    -0x1.0p3,
    -0x1.0000000000001p2
  },
  { // Entry 698
    0x1.0468a8ace4df610620bf7406afeac012p1,
    0x1.0p3,
    -0x1.0p2
  },
  { // Entry 699
    -0x1.0468a8ace4df610620bf7406afeac012p1,
    -0x1.0p3,
    -0x1.0p2
  },
  { // Entry 700
    0x1.0468a8ace4df5f6c8725da6d164e971cp1,
    0x1.0p3,
    -0x1.fffffffffffffp1
  },
  { // Entry 701
    -0x1.0468a8ace4df5f6c8725da6d164e971cp1,
    -0x1.0p3,
    -0x1.fffffffffffffp1
  },
  { // Entry 702
    0x1.0468a8ace4df610620bf7406afeac012p1,
    0x1.0000000000001p3,
    -0x1.0000000000001p2
  },
  { // Entry 703
    -0x1.0468a8ace4df610620bf7406afeac012p1,
    -0x1.0000000000001p3,
    -0x1.0000000000001p2
  },
  { // Entry 704
    0x1.0468a8ace4df5dd2ed8c40d37ce082a1p1,
    0x1.0000000000001p3,
    -0x1.0p2
  },
  { // Entry 705
    -0x1.0468a8ace4df5dd2ed8c40d37ce082a1p1,
    -0x1.0000000000001p3,
    -0x1.0p2
  },
  { // Entry 706
    0x1.0468a8ace4df5c3953f2a739e353b5d4p1,
    0x1.0000000000001p3,
    -0x1.fffffffffffffp1
  },
  { // Entry 707
    -0x1.0468a8ace4df5c3953f2a739e353b5d4p1,
    -0x1.0000000000001p3,
    -0x1.fffffffffffffp1
  },
  { // Entry 708
    0x1.ffffffffffffffffffffffffffffffffp-101,
    0x1.fffffffffffffp-103,
    0x1.fffffffffffffp-3
  },
  { // Entry 709
    -0x1.ffffffffffffffffffffffffffffffffp-101,
    -0x1.fffffffffffffp-103,
    0x1.fffffffffffffp-3
  },
  { // Entry 710
    0x1.ffffffffffffefffffffffffffffffffp-101,
    0x1.fffffffffffffp-103,
    0x1.0p-2
  },
  { // Entry 711
    -0x1.ffffffffffffefffffffffffffffffffp-101,
    -0x1.fffffffffffffp-103,
    0x1.0p-2
  },
  { // Entry 712
    0x1.ffffffffffffd0000000000002ffffffp-101,
    0x1.fffffffffffffp-103,
    0x1.0000000000001p-2
  },
  { // Entry 713
    -0x1.ffffffffffffd0000000000002ffffffp-101,
    -0x1.fffffffffffffp-103,
    0x1.0000000000001p-2
  },
  { // Entry 714
    0x1.0000000000000800000000000040p-100,
    0x1.0p-102,
    0x1.fffffffffffffp-3
  },
  { // Entry 715
    -0x1.0000000000000800000000000040p-100,
    -0x1.0p-102,
    0x1.fffffffffffffp-3
  },
  { // Entry 716
    0x1.ffffffffffffffffffffffffffffffffp-101,
    0x1.0p-102,
    0x1.0p-2
  },
  { // Entry 717
    -0x1.ffffffffffffffffffffffffffffffffp-101,
    -0x1.0p-102,
    0x1.0p-2
  },
  { // Entry 718
    0x1.ffffffffffffe0000000000001ffffffp-101,
    0x1.0p-102,
    0x1.0000000000001p-2
  },
  { // Entry 719
    -0x1.ffffffffffffe0000000000001ffffffp-101,
    -0x1.0p-102,
    0x1.0000000000001p-2
  },
  { // Entry 720
    0x1.00000000000018000000000000c0p-100,
    0x1.0000000000001p-102,
    0x1.fffffffffffffp-3
  },
  { // Entry 721
    -0x1.00000000000018000000000000c0p-100,
    -0x1.0000000000001p-102,
    0x1.fffffffffffffp-3
  },
  { // Entry 722
    0x1.0000000000000fffffffffffffffffffp-100,
    0x1.0000000000001p-102,
    0x1.0p-2
  },
  { // Entry 723
    -0x1.0000000000000fffffffffffffffffffp-100,
    -0x1.0000000000001p-102,
    0x1.0p-2
  },
  { // Entry 724
    0x1.ffffffffffffffffffffffffffffffffp-101,
    0x1.0000000000001p-102,
    0x1.0000000000001p-2
  },
  { // Entry 725
    -0x1.ffffffffffffffffffffffffffffffffp-101,
    -0x1.0000000000001p-102,
    0x1.0000000000001p-2
  },
  { // Entry 726
    0x1.ffffffffffffffffffffffffffffffffp-201,
    0x1.fffffffffffffp-203,
    0x1.fffffffffffffp-3
  },
  { // Entry 727
    -0x1.ffffffffffffffffffffffffffffffffp-201,
    -0x1.fffffffffffffp-203,
    0x1.fffffffffffffp-3
  },
  { // Entry 728
    0x1.ffffffffffffefffffffffffffffffffp-201,
    0x1.fffffffffffffp-203,
    0x1.0p-2
  },
  { // Entry 729
    -0x1.ffffffffffffefffffffffffffffffffp-201,
    -0x1.fffffffffffffp-203,
    0x1.0p-2
  },
  { // Entry 730
    0x1.ffffffffffffd0000000000002ffffffp-201,
    0x1.fffffffffffffp-203,
    0x1.0000000000001p-2
  },
  { // Entry 731
    -0x1.ffffffffffffd0000000000002ffffffp-201,
    -0x1.fffffffffffffp-203,
    0x1.0000000000001p-2
  },
  { // Entry 732
    0x1.0000000000000800000000000040p-200,
    0x1.0p-202,
    0x1.fffffffffffffp-3
  },
  { // Entry 733
    -0x1.0000000000000800000000000040p-200,
    -0x1.0p-202,
    0x1.fffffffffffffp-3
  },
  { // Entry 734
    0x1.ffffffffffffffffffffffffffffffffp-201,
    0x1.0p-202,
    0x1.0p-2
  },
  { // Entry 735
    -0x1.ffffffffffffffffffffffffffffffffp-201,
    -0x1.0p-202,
    0x1.0p-2
  },
  { // Entry 736
    0x1.ffffffffffffe0000000000001ffffffp-201,
    0x1.0p-202,
    0x1.0000000000001p-2
  },
  { // Entry 737
    -0x1.ffffffffffffe0000000000001ffffffp-201,
    -0x1.0p-202,
    0x1.0000000000001p-2
  },
  { // Entry 738
    0x1.00000000000018000000000000c0p-200,
    0x1.0000000000001p-202,
    0x1.fffffffffffffp-3
  },
  { // Entry 739
    -0x1.00000000000018000000000000c0p-200,
    -0x1.0000000000001p-202,
    0x1.fffffffffffffp-3
  },
  { // Entry 740
    0x1.0000000000000fffffffffffffffffffp-200,
    0x1.0000000000001p-202,
    0x1.0p-2
  },
  { // Entry 741
    -0x1.0000000000000fffffffffffffffffffp-200,
    -0x1.0000000000001p-202,
    0x1.0p-2
  },
  { // Entry 742
    0x1.ffffffffffffffffffffffffffffffffp-201,
    0x1.0000000000001p-202,
    0x1.0000000000001p-2
  },
  { // Entry 743
    -0x1.ffffffffffffffffffffffffffffffffp-201,
    -0x1.0000000000001p-202,
    0x1.0000000000001p-2
  },
  { // Entry 744
    0x1.ffffffffffffffffffffffffffffffffp-1001,
    0x1.fffffffffffffp-1003,
    0x1.fffffffffffffp-3
  },
  { // Entry 745
    -0x1.ffffffffffffffffffffffffffffffffp-1001,
    -0x1.fffffffffffffp-1003,
    0x1.fffffffffffffp-3
  },
  { // Entry 746
    0x1.ffffffffffffefffffffffffffffffffp-1001,
    0x1.fffffffffffffp-1003,
    0x1.0p-2
  },
  { // Entry 747
    -0x1.ffffffffffffefffffffffffffffffffp-1001,
    -0x1.fffffffffffffp-1003,
    0x1.0p-2
  },
  { // Entry 748
    0x1.ffffffffffffd0000000000002ffffffp-1001,
    0x1.fffffffffffffp-1003,
    0x1.0000000000001p-2
  },
  { // Entry 749
    -0x1.ffffffffffffd0000000000002ffffffp-1001,
    -0x1.fffffffffffffp-1003,
    0x1.0000000000001p-2
  },
  { // Entry 750
    0x1.0000000000000800000000000040p-1000,
    0x1.0p-1002,
    0x1.fffffffffffffp-3
  },
  { // Entry 751
    -0x1.0000000000000800000000000040p-1000,
    -0x1.0p-1002,
    0x1.fffffffffffffp-3
  },
  { // Entry 752
    0x1.ffffffffffffffffffffffffffffffffp-1001,
    0x1.0p-1002,
    0x1.0p-2
  },
  { // Entry 753
    -0x1.ffffffffffffffffffffffffffffffffp-1001,
    -0x1.0p-1002,
    0x1.0p-2
  },
  { // Entry 754
    0x1.ffffffffffffe0000000000001ffffffp-1001,
    0x1.0p-1002,
    0x1.0000000000001p-2
  },
  { // Entry 755
    -0x1.ffffffffffffe0000000000001ffffffp-1001,
    -0x1.0p-1002,
    0x1.0000000000001p-2
  },
  { // Entry 756
    0x1.00000000000018000000000000c0p-1000,
    0x1.0000000000001p-1002,
    0x1.fffffffffffffp-3
  },
  { // Entry 757
    -0x1.00000000000018000000000000c0p-1000,
    -0x1.0000000000001p-1002,
    0x1.fffffffffffffp-3
  },
  { // Entry 758
    0x1.0000000000000fffffffffffffffffffp-1000,
    0x1.0000000000001p-1002,
    0x1.0p-2
  },
  { // Entry 759
    -0x1.0000000000000fffffffffffffffffffp-1000,
    -0x1.0000000000001p-1002,
    0x1.0p-2
  },
  { // Entry 760
    0x1.ffffffffffffffffffffffffffffffffp-1001,
    0x1.0000000000001p-1002,
    0x1.0000000000001p-2
  },
  { // Entry 761
    -0x1.ffffffffffffffffffffffffffffffffp-1001,
    -0x1.0000000000001p-1002,
    0x1.0000000000001p-2
  },
  { // Entry 762
    0x1.ffffffffffffffffffffffffffffffffp-101,
    0x1.ffffffffffffep2,
    0x1.ffffffffffffep102
  },
  { // Entry 763
    -0x1.ffffffffffffffffffffffffffffffffp-101,
    -0x1.ffffffffffffep2,
    0x1.ffffffffffffep102
  },
  { // Entry 764
    0x1.ffffffffffffefffffffffffff7fffffp-101,
    0x1.ffffffffffffep2,
    0x1.fffffffffffffp102
  },
  { // Entry 765
    -0x1.ffffffffffffefffffffffffff7fffffp-101,
    -0x1.ffffffffffffep2,
    0x1.fffffffffffffp102
  },
  { // Entry 766
    0x1.ffffffffffffdfffffffffffffffffffp-101,
    0x1.ffffffffffffep2,
    0x1.0p103
  },
  { // Entry 767
    -0x1.ffffffffffffdfffffffffffffffffffp-101,
    -0x1.ffffffffffffep2,
    0x1.0p103
  },
  { // Entry 768
    0x1.ffffffffffffc0000000000003ffffffp-101,
    0x1.ffffffffffffep2,
    0x1.0000000000001p103
  },
  { // Entry 769
    -0x1.ffffffffffffc0000000000003ffffffp-101,
    -0x1.ffffffffffffep2,
    0x1.0000000000001p103
  },
  { // Entry 770
    0x1.ffffffffffffa000000000000bffffffp-101,
    0x1.ffffffffffffep2,
    0x1.0000000000002p103
  },
  { // Entry 771
    -0x1.ffffffffffffa000000000000bffffffp-101,
    -0x1.ffffffffffffep2,
    0x1.0000000000002p103
  },
  { // Entry 772
    0x1.0000000000000800000000000080p-100,
    0x1.fffffffffffffp2,
    0x1.ffffffffffffep102
  },
  { // Entry 773
    -0x1.0000000000000800000000000080p-100,
    -0x1.fffffffffffffp2,
    0x1.ffffffffffffep102
  },
  { // Entry 774
    0x1.ffffffffffffffffffffffffffffffffp-101,
    0x1.fffffffffffffp2,
    0x1.fffffffffffffp102
  },
  { // Entry 775
    -0x1.ffffffffffffffffffffffffffffffffp-101,
    -0x1.fffffffffffffp2,
    0x1.fffffffffffffp102
  },
  { // Entry 776
    0x1.ffffffffffffefffffffffffffffffffp-101,
    0x1.fffffffffffffp2,
    0x1.0p103
  },
  { // Entry 777
    -0x1.ffffffffffffefffffffffffffffffffp-101,
    -0x1.fffffffffffffp2,
    0x1.0p103
  },
  { // Entry 778
    0x1.ffffffffffffd0000000000002ffffffp-101,
    0x1.fffffffffffffp2,
    0x1.0000000000001p103
  },
  { // Entry 779
    -0x1.ffffffffffffd0000000000002ffffffp-101,
    -0x1.fffffffffffffp2,
    0x1.0000000000001p103
  },
  { // Entry 780
    0x1.ffffffffffffb0000000000009ffffffp-101,
    0x1.fffffffffffffp2,
    0x1.0000000000002p103
  },
  { // Entry 781
    -0x1.ffffffffffffb0000000000009ffffffp-101,
    -0x1.fffffffffffffp2,
    0x1.0000000000002p103
  },
  { // Entry 782
    0x1.00000000000010000000000001p-100,
    0x1.0p3,
    0x1.ffffffffffffep102
  },
  { // Entry 783
    -0x1.00000000000010000000000001p-100,
    -0x1.0p3,
    0x1.ffffffffffffep102
  },
  { // Entry 784
    0x1.0000000000000800000000000040p-100,
    0x1.0p3,
    0x1.fffffffffffffp102
  },
  { // Entry 785
    -0x1.0000000000000800000000000040p-100,
    -0x1.0p3,
    0x1.fffffffffffffp102
  },
  { // Entry 786
    0x1.ffffffffffffffffffffffffffffffffp-101,
    0x1.0p3,
    0x1.0p103
  },
  { // Entry 787
    -0x1.ffffffffffffffffffffffffffffffffp-101,
    -0x1.0p3,
    0x1.0p103
  },
  { // Entry 788
    0x1.ffffffffffffe0000000000001ffffffp-101,
    0x1.0p3,
    0x1.0000000000001p103
  },
  { // Entry 789
    -0x1.ffffffffffffe0000000000001ffffffp-101,
    -0x1.0p3,
    0x1.0000000000001p103
  },
  { // Entry 790
    0x1.ffffffffffffc0000000000007ffffffp-101,
    0x1.0p3,
    0x1.0000000000002p103
  },
  { // Entry 791
    -0x1.ffffffffffffc0000000000007ffffffp-101,
    -0x1.0p3,
    0x1.0000000000002p103
  },
  { // Entry 792
    0x1.00000000000020000000000002p-100,
    0x1.0000000000001p3,
    0x1.ffffffffffffep102
  },
  { // Entry 793
    -0x1.00000000000020000000000002p-100,
    -0x1.0000000000001p3,
    0x1.ffffffffffffep102
  },
  { // Entry 794
    0x1.00000000000018000000000000c0p-100,
    0x1.0000000000001p3,
    0x1.fffffffffffffp102
  },
  { // Entry 795
    -0x1.00000000000018000000000000c0p-100,
    -0x1.0000000000001p3,
    0x1.fffffffffffffp102
  },
  { // Entry 796
    0x1.0000000000000fffffffffffffffffffp-100,
    0x1.0000000000001p3,
    0x1.0p103
  },
  { // Entry 797
    -0x1.0000000000000fffffffffffffffffffp-100,
    -0x1.0000000000001p3,
    0x1.0p103
  },
  { // Entry 798
    0x1.ffffffffffffffffffffffffffffffffp-101,
    0x1.0000000000001p3,
    0x1.0000000000001p103
  },
  { // Entry 799
    -0x1.ffffffffffffffffffffffffffffffffp-101,
    -0x1.0000000000001p3,
    0x1.0000000000001p103
  },
  { // Entry 800
    0x1.ffffffffffffe0000000000003ffffffp-101,
    0x1.0000000000001p3,
    0x1.0000000000002p103
  },
  { // Entry 801
    -0x1.ffffffffffffe0000000000003ffffffp-101,
    -0x1.0000000000001p3,
    0x1.0000000000002p103
  },
  { // Entry 802
    0x1.00000000000030000000000003p-100,
    0x1.0000000000002p3,
    0x1.ffffffffffffep102
  },
  { // Entry 803
    -0x1.00000000000030000000000003p-100,
    -0x1.0000000000002p3,
    0x1.ffffffffffffep102
  },
  { // Entry 804
    0x1.0000000000002800000000000140p-100,
    0x1.0000000000002p3,
    0x1.fffffffffffffp102
  },
  { // Entry 805
    -0x1.0000000000002800000000000140p-100,
    -0x1.0000000000002p3,
    0x1.fffffffffffffp102
  },
  { // Entry 806
    0x1.0000000000001fffffffffffffffffffp-100,
    0x1.0000000000002p3,
    0x1.0p103
  },
  { // Entry 807
    -0x1.0000000000001fffffffffffffffffffp-100,
    -0x1.0000000000002p3,
    0x1.0p103
  },
  { // Entry 808
    0x1.0000000000000fffffffffffffp-100,
    0x1.0000000000002p3,
    0x1.0000000000001p103
  },
  { // Entry 809
    -0x1.0000000000000fffffffffffffp-100,
    -0x1.0000000000002p3,
    0x1.0000000000001p103
  },
  { // Entry 810
    0x1.ffffffffffffffffffffffffffffffffp-101,
    0x1.0000000000002p3,
    0x1.0000000000002p103
  },
  { // Entry 811
    -0x1.ffffffffffffffffffffffffffffffffp-101,
    -0x1.0000000000002p3,
    0x1.0000000000002p103
  },
  { // Entry 812
    0x1.ffffffffffffffffffffffffffffffffp-1024,
    0x1.ffffffffffffep0,
    0x1.ffffffffffffep1023
  },
  { // Entry 813
    -0x1.ffffffffffffffffffffffffffffffffp-1024,
    -0x1.ffffffffffffep0,
    0x1.ffffffffffffep1023
  },
  { // Entry 814
    0x1.ffffffffffffefffffffffffff7fffffp-1024,
    0x1.ffffffffffffep0,
    0x1.fffffffffffffp1023
  },
  { // Entry 815
    -0x1.ffffffffffffefffffffffffff7fffffp-1024,
    -0x1.ffffffffffffep0,
    0x1.fffffffffffffp1023
  },
  { // Entry 816
    0x1.ffffffffffffefffffffffffff7fffffp-1024,
    0x1.ffffffffffffep0,
    0x1.fffffffffffffp1023
  },
  { // Entry 817
    -0x1.ffffffffffffefffffffffffff7fffffp-1024,
    -0x1.ffffffffffffep0,
    0x1.fffffffffffffp1023
  },
  { // Entry 818
    0x1.ffffffffffffefffffffffffff7fffffp-1024,
    0x1.ffffffffffffep0,
    0x1.fffffffffffffp1023
  },
  { // Entry 819
    -0x1.ffffffffffffefffffffffffff7fffffp-1024,
    -0x1.ffffffffffffep0,
    0x1.fffffffffffffp1023
  },
  { // Entry 820
    0x1.ffffffffffffefffffffffffff7fffffp-1024,
    0x1.ffffffffffffep0,
    0x1.fffffffffffffp1023
  },
  { // Entry 821
    -0x1.ffffffffffffefffffffffffff7fffffp-1024,
    -0x1.ffffffffffffep0,
    0x1.fffffffffffffp1023
  },
  { // Entry 822
    0x1.0000000000000800000000000080p-1023,
    0x1.fffffffffffffp0,
    0x1.ffffffffffffep1023
  },
  { // Entry 823
    -0x1.0000000000000800000000000080p-1023,
    -0x1.fffffffffffffp0,
    0x1.ffffffffffffep1023
  },
  { // Entry 824
    0x1.ffffffffffffffffffffffffffffffffp-1024,
    0x1.fffffffffffffp0,
    0x1.fffffffffffffp1023
  },
  { // Entry 825
    -0x1.ffffffffffffffffffffffffffffffffp-1024,
    -0x1.fffffffffffffp0,
    0x1.fffffffffffffp1023
  },
  { // Entry 826
    0x1.ffffffffffffffffffffffffffffffffp-1024,
    0x1.fffffffffffffp0,
    0x1.fffffffffffffp1023
  },
  { // Entry 827
    -0x1.ffffffffffffffffffffffffffffffffp-1024,
    -0x1.fffffffffffffp0,
    0x1.fffffffffffffp1023
  },
  { // Entry 828
    0x1.ffffffffffffffffffffffffffffffffp-1024,
    0x1.fffffffffffffp0,
    0x1.fffffffffffffp1023
  },
  { // Entry 829
    -0x1.ffffffffffffffffffffffffffffffffp-1024,
    -0x1.fffffffffffffp0,
    0x1.fffffffffffffp1023
  },
  { // Entry 830
    0x1.ffffffffffffffffffffffffffffffffp-1024,
    0x1.fffffffffffffp0,
    0x1.fffffffffffffp1023
  },
  { // Entry 831
    -0x1.ffffffffffffffffffffffffffffffffp-1024,
    -0x1.fffffffffffffp0,
    0x1.fffffffffffffp1023
  },
  { // Entry 832
    0x1.00000000000010000000000001p-1023,
    0x1.0p1,
    0x1.ffffffffffffep1023
  },
  { // Entry 833
    -0x1.00000000000010000000000001p-1023,
    -0x1.0p1,
    0x1.ffffffffffffep1023
  },
  { // Entry 834
    0x1.0000000000000800000000000040p-1023,
    0x1.0p1,
    0x1.fffffffffffffp1023
  },
  { // Entry 835
    -0x1.0000000000000800000000000040p-1023,
    -0x1.0p1,
    0x1.fffffffffffffp1023
  },
  { // Entry 836
    0x1.0000000000000800000000000040p-1023,
    0x1.0p1,
    0x1.fffffffffffffp1023
  },
  { // Entry 837
    -0x1.0000000000000800000000000040p-1023,
    -0x1.0p1,
    0x1.fffffffffffffp1023
  },
  { // Entry 838
    0x1.0000000000000800000000000040p-1023,
    0x1.0p1,
    0x1.fffffffffffffp1023
  },
  { // Entry 839
    -0x1.0000000000000800000000000040p-1023,
    -0x1.0p1,
    0x1.fffffffffffffp1023
  },
  { // Entry 840
    0x1.0000000000000800000000000040p-1023,
    0x1.0p1,
    0x1.fffffffffffffp1023
  },
  { // Entry 841
    -0x1.0000000000000800000000000040p-1023,
    -0x1.0p1,
    0x1.fffffffffffffp1023
  },
  { // Entry 842
    0x1.00000000000020000000000002p-1023,
    0x1.0000000000001p1,
    0x1.ffffffffffffep1023
  },
  { // Entry 843
    -0x1.00000000000020000000000002p-1023,
    -0x1.0000000000001p1,
    0x1.ffffffffffffep1023
  },
  { // Entry 844
    0x1.00000000000018000000000000c0p-1023,
    0x1.0000000000001p1,
    0x1.fffffffffffffp1023
  },
  { // Entry 845
    -0x1.00000000000018000000000000c0p-1023,
    -0x1.0000000000001p1,
    0x1.fffffffffffffp1023
  },
  { // Entry 846
    0x1.00000000000018000000000000c0p-1023,
    0x1.0000000000001p1,
    0x1.fffffffffffffp1023
  },
  { // Entry 847
    -0x1.00000000000018000000000000c0p-1023,
    -0x1.0000000000001p1,
    0x1.fffffffffffffp1023
  },
  { // Entry 848
    0x1.00000000000018000000000000c0p-1023,
    0x1.0000000000001p1,
    0x1.fffffffffffffp1023
  },
  { // Entry 849
    -0x1.00000000000018000000000000c0p-1023,
    -0x1.0000000000001p1,
    0x1.fffffffffffffp1023
  },
  { // Entry 850
    0x1.00000000000018000000000000c0p-1023,
    0x1.0000000000001p1,
    0x1.fffffffffffffp1023
  },
  { // Entry 851
    -0x1.00000000000018000000000000c0p-1023,
    -0x1.0000000000001p1,
    0x1.fffffffffffffp1023
  },
  { // Entry 852
    0x1.00000000000030000000000003p-1023,
    0x1.0000000000002p1,
    0x1.ffffffffffffep1023
  },
  { // Entry 853
    -0x1.00000000000030000000000003p-1023,
    -0x1.0000000000002p1,
    0x1.ffffffffffffep1023
  },
  { // Entry 854
    0x1.0000000000002800000000000140p-1023,
    0x1.0000000000002p1,
    0x1.fffffffffffffp1023
  },
  { // Entry 855
    -0x1.0000000000002800000000000140p-1023,
    -0x1.0000000000002p1,
    0x1.fffffffffffffp1023
  },
  { // Entry 856
    0x1.0000000000002800000000000140p-1023,
    0x1.0000000000002p1,
    0x1.fffffffffffffp1023
  },
  { // Entry 857
    -0x1.0000000000002800000000000140p-1023,
    -0x1.0000000000002p1,
    0x1.fffffffffffffp1023
  },
  { // Entry 858
    0x1.0000000000002800000000000140p-1023,
    0x1.0000000000002p1,
    0x1.fffffffffffffp1023
  },
  { // Entry 859
    -0x1.0000000000002800000000000140p-1023,
    -0x1.0000000000002p1,
    0x1.fffffffffffffp1023
  },
  { // Entry 860
    0x1.0000000000002800000000000140p-1023,
    0x1.0000000000002p1,
    0x1.fffffffffffffp1023
  },
  { // Entry 861
    -0x1.0000000000002800000000000140p-1023,
    -0x1.0000000000002p1,
    0x1.fffffffffffffp1023
  },
  { // Entry 862
    0x1.ffffffffffffffffffffffffffffffffp-1023,
    0x1.ffffffffffffep1,
    0x1.ffffffffffffep1023
  },
  { // Entry 863
    -0x1.ffffffffffffffffffffffffffffffffp-1023,
    -0x1.ffffffffffffep1,
    0x1.ffffffffffffep1023
  },
  { // Entry 864
    0x1.ffffffffffffefffffffffffff7fffffp-1023,
    0x1.ffffffffffffep1,
    0x1.fffffffffffffp1023
  },
  { // Entry 865
    -0x1.ffffffffffffefffffffffffff7fffffp-1023,
    -0x1.ffffffffffffep1,
    0x1.fffffffffffffp1023
  },
  { // Entry 866
    0x1.ffffffffffffefffffffffffff7fffffp-1023,
    0x1.ffffffffffffep1,
    0x1.fffffffffffffp1023
  },
  { // Entry 867
    -0x1.ffffffffffffefffffffffffff7fffffp-1023,
    -0x1.ffffffffffffep1,
    0x1.fffffffffffffp1023
  },
  { // Entry 868
    0x1.ffffffffffffefffffffffffff7fffffp-1023,
    0x1.ffffffffffffep1,
    0x1.fffffffffffffp1023
  },
  { // Entry 869
    -0x1.ffffffffffffefffffffffffff7fffffp-1023,
    -0x1.ffffffffffffep1,
    0x1.fffffffffffffp1023
  },
  { // Entry 870
    0x1.ffffffffffffefffffffffffff7fffffp-1023,
    0x1.ffffffffffffep1,
    0x1.fffffffffffffp1023
  },
  { // Entry 871
    -0x1.ffffffffffffefffffffffffff7fffffp-1023,
    -0x1.ffffffffffffep1,
    0x1.fffffffffffffp1023
  },
  { // Entry 872
    0x1.0000000000000800000000000080p-1022,
    0x1.fffffffffffffp1,
    0x1.ffffffffffffep1023
  },
  { // Entry 873
    -0x1.0000000000000800000000000080p-1022,
    -0x1.fffffffffffffp1,
    0x1.ffffffffffffep1023
  },
  { // Entry 874
    0x1.ffffffffffffffffffffffffffffffffp-1023,
    0x1.fffffffffffffp1,
    0x1.fffffffffffffp1023
  },
  { // Entry 875
    -0x1.ffffffffffffffffffffffffffffffffp-1023,
    -0x1.fffffffffffffp1,
    0x1.fffffffffffffp1023
  },
  { // Entry 876
    0x1.ffffffffffffffffffffffffffffffffp-1023,
    0x1.fffffffffffffp1,
    0x1.fffffffffffffp1023
  },
  { // Entry 877
    -0x1.ffffffffffffffffffffffffffffffffp-1023,
    -0x1.fffffffffffffp1,
    0x1.fffffffffffffp1023
  },
  { // Entry 878
    0x1.ffffffffffffffffffffffffffffffffp-1023,
    0x1.fffffffffffffp1,
    0x1.fffffffffffffp1023
  },
  { // Entry 879
    -0x1.ffffffffffffffffffffffffffffffffp-1023,
    -0x1.fffffffffffffp1,
    0x1.fffffffffffffp1023
  },
  { // Entry 880
    0x1.ffffffffffffffffffffffffffffffffp-1023,
    0x1.fffffffffffffp1,
    0x1.fffffffffffffp1023
  },
  { // Entry 881
    -0x1.ffffffffffffffffffffffffffffffffp-1023,
    -0x1.fffffffffffffp1,
    0x1.fffffffffffffp1023
  },
  { // Entry 882
    0x1.00000000000010000000000001p-1022,
    0x1.0p2,
    0x1.ffffffffffffep1023
  },
  { // Entry 883
    -0x1.00000000000010000000000001p-1022,
    -0x1.0p2,
    0x1.ffffffffffffep1023
  },
  { // Entry 884
    0x1.0000000000000800000000000040p-1022,
    0x1.0p2,
    0x1.fffffffffffffp1023
  },
  { // Entry 885
    -0x1.0000000000000800000000000040p-1022,
    -0x1.0p2,
    0x1.fffffffffffffp1023
  },
  { // Entry 886
    0x1.0000000000000800000000000040p-1022,
    0x1.0p2,
    0x1.fffffffffffffp1023
  },
  { // Entry 887
    -0x1.0000000000000800000000000040p-1022,
    -0x1.0p2,
    0x1.fffffffffffffp1023
  },
  { // Entry 888
    0x1.0000000000000800000000000040p-1022,
    0x1.0p2,
    0x1.fffffffffffffp1023
  },
  { // Entry 889
    -0x1.0000000000000800000000000040p-1022,
    -0x1.0p2,
    0x1.fffffffffffffp1023
  },
  { // Entry 890
    0x1.0000000000000800000000000040p-1022,
    0x1.0p2,
    0x1.fffffffffffffp1023
  },
  { // Entry 891
    -0x1.0000000000000800000000000040p-1022,
    -0x1.0p2,
    0x1.fffffffffffffp1023
  },
  { // Entry 892
    0x1.00000000000020000000000002p-1022,
    0x1.0000000000001p2,
    0x1.ffffffffffffep1023
  },
  { // Entry 893
    -0x1.00000000000020000000000002p-1022,
    -0x1.0000000000001p2,
    0x1.ffffffffffffep1023
  },
  { // Entry 894
    0x1.00000000000018000000000000c0p-1022,
    0x1.0000000000001p2,
    0x1.fffffffffffffp1023
  },
  { // Entry 895
    -0x1.00000000000018000000000000c0p-1022,
    -0x1.0000000000001p2,
    0x1.fffffffffffffp1023
  },
  { // Entry 896
    0x1.00000000000018000000000000c0p-1022,
    0x1.0000000000001p2,
    0x1.fffffffffffffp1023
  },
  { // Entry 897
    -0x1.00000000000018000000000000c0p-1022,
    -0x1.0000000000001p2,
    0x1.fffffffffffffp1023
  },
  { // Entry 898
    0x1.00000000000018000000000000c0p-1022,
    0x1.0000000000001p2,
    0x1.fffffffffffffp1023
  },
  { // Entry 899
    -0x1.00000000000018000000000000c0p-1022,
    -0x1.0000000000001p2,
    0x1.fffffffffffffp1023
  },
  { // Entry 900
    0x1.00000000000018000000000000c0p-1022,
    0x1.0000000000001p2,
    0x1.fffffffffffffp1023
  },
  { // Entry 901
    -0x1.00000000000018000000000000c0p-1022,
    -0x1.0000000000001p2,
    0x1.fffffffffffffp1023
  },
  { // Entry 902
    0x1.00000000000030000000000003p-1022,
    0x1.0000000000002p2,
    0x1.ffffffffffffep1023
  },
  { // Entry 903
    -0x1.00000000000030000000000003p-1022,
    -0x1.0000000000002p2,
    0x1.ffffffffffffep1023
  },
  { // Entry 904
    0x1.0000000000002800000000000140p-1022,
    0x1.0000000000002p2,
    0x1.fffffffffffffp1023
  },
  { // Entry 905
    -0x1.0000000000002800000000000140p-1022,
    -0x1.0000000000002p2,
    0x1.fffffffffffffp1023
  },
  { // Entry 906
    0x1.0000000000002800000000000140p-1022,
    0x1.0000000000002p2,
    0x1.fffffffffffffp1023
  },
  { // Entry 907
    -0x1.0000000000002800000000000140p-1022,
    -0x1.0000000000002p2,
    0x1.fffffffffffffp1023
  },
  { // Entry 908
    0x1.0000000000002800000000000140p-1022,
    0x1.0000000000002p2,
    0x1.fffffffffffffp1023
  },
  { // Entry 909
    -0x1.0000000000002800000000000140p-1022,
    -0x1.0000000000002p2,
    0x1.fffffffffffffp1023
  },
  { // Entry 910
    0x1.0000000000002800000000000140p-1022,
    0x1.0000000000002p2,
    0x1.fffffffffffffp1023
  },
  { // Entry 911
    -0x1.0000000000002800000000000140p-1022,
    -0x1.0000000000002p2,
    0x1.fffffffffffffp1023
  },
  { // Entry 912
    -0x1.921fb54442d18469898cc51701b839a2p0,
    -0x1.0000000000002p1023,
    -0x1.0000000000002p0
  },
  { // Entry 913
    0x1.921fb54442d18469898cc51701b839a2p0,
    0x1.0000000000002p1023,
    -0x1.0000000000002p0
  },
  { // Entry 914
    -0x1.921fb54442d18469898cc51701b839a2p0,
    -0x1.0000000000002p1023,
    -0x1.0000000000001p0
  },
  { // Entry 915
    0x1.921fb54442d18469898cc51701b839a2p0,
    0x1.0000000000002p1023,
    -0x1.0000000000001p0
  },
  { // Entry 916
    -0x1.921fb54442d18469898cc51701b839a2p0,
    -0x1.0000000000002p1023,
    -0x1.0p0
  },
  { // Entry 917
    0x1.921fb54442d18469898cc51701b839a2p0,
    0x1.0000000000002p1023,
    -0x1.0p0
  },
  { // Entry 918
    -0x1.921fb54442d18469898cc51701b839a2p0,
    -0x1.0000000000002p1023,
    -0x1.fffffffffffffp-1
  },
  { // Entry 919
    0x1.921fb54442d18469898cc51701b839a2p0,
    0x1.0000000000002p1023,
    -0x1.fffffffffffffp-1
  },
  { // Entry 920
    -0x1.921fb54442d18469898cc51701b839a2p0,
    -0x1.0000000000002p1023,
    -0x1.ffffffffffffep-1
  },
  { // Entry 921
    0x1.921fb54442d18469898cc51701b839a2p0,
    0x1.0000000000002p1023,
    -0x1.ffffffffffffep-1
  },
  { // Entry 922
    -0x1.921fb54442d18469898cc51701b839a2p0,
    -0x1.0000000000001p1023,
    -0x1.0000000000002p0
  },
  { // Entry 923
    0x1.921fb54442d18469898cc51701b839a2p0,
    0x1.0000000000001p1023,
    -0x1.0000000000002p0
  },
  { // Entry 924
    -0x1.921fb54442d18469898cc51701b839a2p0,
    -0x1.0000000000001p1023,
    -0x1.0000000000001p0
  },
  { // Entry 925
    0x1.921fb54442d18469898cc51701b839a2p0,
    0x1.0000000000001p1023,
    -0x1.0000000000001p0
  },
  { // Entry 926
    -0x1.921fb54442d18469898cc51701b839a2p0,
    -0x1.0000000000001p1023,
    -0x1.0p0
  },
  { // Entry 927
    0x1.921fb54442d18469898cc51701b839a2p0,
    0x1.0000000000001p1023,
    -0x1.0p0
  },
  { // Entry 928
    -0x1.921fb54442d18469898cc51701b839a2p0,
    -0x1.0000000000001p1023,
    -0x1.fffffffffffffp-1
  },
  { // Entry 929
    0x1.921fb54442d18469898cc51701b839a2p0,
    0x1.0000000000001p1023,
    -0x1.fffffffffffffp-1
  },
  { // Entry 930
    -0x1.921fb54442d18469898cc51701b839a2p0,
    -0x1.0000000000001p1023,
    -0x1.ffffffffffffep-1
  },
  { // Entry 931
    0x1.921fb54442d18469898cc51701b839a2p0,
    0x1.0000000000001p1023,
    -0x1.ffffffffffffep-1
  },
  { // Entry 932
    -0x1.921fb54442d18469898cc51701b839a2p0,
    -0x1.0p1023,
    -0x1.0000000000002p0
  },
  { // Entry 933
    0x1.921fb54442d18469898cc51701b839a2p0,
    0x1.0p1023,
    -0x1.0000000000002p0
  },
  { // Entry 934
    -0x1.921fb54442d18469898cc51701b839a2p0,
    -0x1.0p1023,
    -0x1.0000000000001p0
  },
  { // Entry 935
    0x1.921fb54442d18469898cc51701b839a2p0,
    0x1.0p1023,
    -0x1.0000000000001p0
  },
  { // Entry 936
    -0x1.921fb54442d18469898cc51701b839a2p0,
    -0x1.0p1023,
    -0x1.0p0
  },
  { // Entry 937
    0x1.921fb54442d18469898cc51701b839a2p0,
    0x1.0p1023,
    -0x1.0p0
  },
  { // Entry 938
    -0x1.921fb54442d18469898cc51701b839a2p0,
    -0x1.0p1023,
    -0x1.fffffffffffffp-1
  },
  { // Entry 939
    0x1.921fb54442d18469898cc51701b839a2p0,
    0x1.0p1023,
    -0x1.fffffffffffffp-1
  },
  { // Entry 940
    -0x1.921fb54442d18469898cc51701b839a2p0,
    -0x1.0p1023,
    -0x1.ffffffffffffep-1
  },
  { // Entry 941
    0x1.921fb54442d18469898cc51701b839a2p0,
    0x1.0p1023,
    -0x1.ffffffffffffep-1
  },
  { // Entry 942
    -0x1.921fb54442d18469898cc51701b839a2p0,
    -0x1.fffffffffffffp1022,
    -0x1.0000000000002p0
  },
  { // Entry 943
    0x1.921fb54442d18469898cc51701b839a2p0,
    0x1.fffffffffffffp1022,
    -0x1.0000000000002p0
  },
  { // Entry 944
    -0x1.921fb54442d18469898cc51701b839a2p0,
    -0x1.fffffffffffffp1022,
    -0x1.0000000000001p0
  },
  { // Entry 945
    0x1.921fb54442d18469898cc51701b839a2p0,
    0x1.fffffffffffffp1022,
    -0x1.0000000000001p0
  },
  { // Entry 946
    -0x1.921fb54442d18469898cc51701b839a2p0,
    -0x1.fffffffffffffp1022,
    -0x1.0p0
  },
  { // Entry 947
    0x1.921fb54442d18469898cc51701b839a2p0,
    0x1.fffffffffffffp1022,
    -0x1.0p0
  },
  { // Entry 948
    -0x1.921fb54442d18469898cc51701b839a2p0,
    -0x1.fffffffffffffp1022,
    -0x1.fffffffffffffp-1
  },
  { // Entry 949
    0x1.921fb54442d18469898cc51701b839a2p0,
    0x1.fffffffffffffp1022,
    -0x1.fffffffffffffp-1
  },
  { // Entry 950
    -0x1.921fb54442d18469898cc51701b839a2p0,
    -0x1.fffffffffffffp1022,
    -0x1.ffffffffffffep-1
  },
  { // Entry 951
    0x1.921fb54442d18469898cc51701b839a2p0,
    0x1.fffffffffffffp1022,
    -0x1.ffffffffffffep-1
  },
  { // Entry 952
    -0x1.921fb54442d18469898cc51701b839a2p0,
    -0x1.ffffffffffffep1022,
    -0x1.0000000000002p0
  },
  { // Entry 953
    0x1.921fb54442d18469898cc51701b839a2p0,
    0x1.ffffffffffffep1022,
    -0x1.0000000000002p0
  },
  { // Entry 954
    -0x1.921fb54442d18469898cc51701b839a2p0,
    -0x1.ffffffffffffep1022,
    -0x1.0000000000001p0
  },
  { // Entry 955
    0x1.921fb54442d18469898cc51701b839a2p0,
    0x1.ffffffffffffep1022,
    -0x1.0000000000001p0
  },
  { // Entry 956
    -0x1.921fb54442d18469898cc51701b839a2p0,
    -0x1.ffffffffffffep1022,
    -0x1.0p0
  },
  { // Entry 957
    0x1.921fb54442d18469898cc51701b839a2p0,
    0x1.ffffffffffffep1022,
    -0x1.0p0
  },
  { // Entry 958
    -0x1.921fb54442d18469898cc51701b839a2p0,
    -0x1.ffffffffffffep1022,
    -0x1.fffffffffffffp-1
  },
  { // Entry 959
    0x1.921fb54442d18469898cc51701b839a2p0,
    0x1.ffffffffffffep1022,
    -0x1.fffffffffffffp-1
  },
  { // Entry 960
    -0x1.921fb54442d18469898cc51701b839a2p0,
    -0x1.ffffffffffffep1022,
    -0x1.ffffffffffffep-1
  },
  { // Entry 961
    0x1.921fb54442d18469898cc51701b839a2p0,
    0x1.ffffffffffffep1022,
    -0x1.ffffffffffffep-1
  },
  { // Entry 962
    0x1.921fb54442d18469898cc51701b839a2p1,
    0.0,
    -0.0
  },
  { // Entry 963
    -0x1.921fb54442d18469898cc51701b839a2p1,
    -0.0,
    -0.0
  },
  { // Entry 964
    0.0,
    0.0,
    0.0
  },
  { // Entry 965
    -0.0,
    -0.0,
    0.0
  },
  { // Entry 966
    0x1.921fb54442d18469898cc51701b839a2p1,
    0.0,
    -0x1.0p-1074
  },
  { // Entry 967
    0x1.921fb54442d18469898cc51701b839a2p1,
    0.0,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 968
    0x1.921fb54442d18469898cc51701b839a2p1,
    0.0,
    -0x1.0p-1022
  },
  { // Entry 969
    0x1.921fb54442d18469898cc51701b839a2p1,
    0.0,
    -0x1.fffffffffffffp1023
  },
  { // Entry 970
    0x1.921fb54442d18469898cc51701b839a2p1,
    0.0,
    -HUGE_VAL
  },
  { // Entry 971
    -0x1.921fb54442d18469898cc51701b839a2p1,
    -0.0,
    -0x1.0p-1074
  },
  { // Entry 972
    -0x1.921fb54442d18469898cc51701b839a2p1,
    -0.0,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 973
    -0x1.921fb54442d18469898cc51701b839a2p1,
    -0.0,
    -0x1.0p-1022
  },
  { // Entry 974
    -0x1.921fb54442d18469898cc51701b839a2p1,
    -0.0,
    -0x1.fffffffffffffp1023
  },
  { // Entry 975
    -0x1.921fb54442d18469898cc51701b839a2p1,
    -0.0,
    -HUGE_VAL
  },
  { // Entry 976
    0.0,
    0.0,
    HUGE_VAL
  },
  { // Entry 977
    0.0,
    0.0,
    0x1.fffffffffffffp1023
  },
  { // Entry 978
    0.0,
    0.0,
    0x1.0p-1022
  },
  { // Entry 979
    0.0,
    0.0,
    0x1.ffffffffffffep-1023
  },
  { // Entry 980
    0.0,
    0.0,
    0x1.0p-1074
  },
  { // Entry 981
    -0.0,
    -0.0,
    HUGE_VAL
  },
  { // Entry 982
    -0.0,
    -0.0,
    0x1.fffffffffffffp1023
  },
  { // Entry 983
    -0.0,
    -0.0,
    0x1.0p-1022
  },
  { // Entry 984
    -0.0,
    -0.0,
    0x1.ffffffffffffep-1023
  },
  { // Entry 985
    -0.0,
    -0.0,
    0x1.0p-1074
  },
  { // Entry 986
    -0x1.921fb54442d18469898cc51701b839a2p0,
    -HUGE_VAL,
    0.0
  },
  { // Entry 987
    -0x1.921fb54442d18469898cc51701b839a2p0,
    -0x1.fffffffffffffp1023,
    0.0
  },
  { // Entry 988
    -0x1.921fb54442d18469898cc51701b839a2p0,
    -0x1.0p-1022,
    0.0
  },
  { // Entry 989
    -0x1.921fb54442d18469898cc51701b839a2p0,
    -0x1.0p-1074,
    0.0
  },
  { // Entry 990
    -0x1.921fb54442d18469898cc51701b839a2p0,
    -HUGE_VAL,
    -0.0
  },
  { // Entry 991
    -0x1.921fb54442d18469898cc51701b839a2p0,
    -0x1.fffffffffffffp1023,
    -0.0
  },
  { // Entry 992
    -0x1.921fb54442d18469898cc51701b839a2p0,
    -0x1.0p-1022,
    -0.0
  },
  { // Entry 993
    -0x1.921fb54442d18469898cc51701b839a2p0,
    -0x1.0p-1074,
    -0.0
  },
  { // Entry 994
    0x1.921fb54442d18469898cc51701b839a2p0,
    HUGE_VAL,
    0.0
  },
  { // Entry 995
    0x1.921fb54442d18469898cc51701b839a2p0,
    0x1.fffffffffffffp1023,
    0.0
  },
  { // Entry 996
    0x1.921fb54442d18469898cc51701b839a2p0,
    0x1.0p-1022,
    0.0
  },
  { // Entry 997
    0x1.921fb54442d18469898cc51701b839a2p0,
    0x1.0p-1074,
    0.0
  },
  { // Entry 998
    0x1.921fb54442d18469898cc51701b839a2p0,
    HUGE_VAL,
    -0.0
  },
  { // Entry 999
    0x1.921fb54442d18469898cc51701b839a2p0,
    0x1.fffffffffffffp1023,
    -0.0
  },
  { // Entry 1000
    0x1.921fb54442d18469898cc51701b839a2p0,
    0x1.0p-1022,
    -0.0
  },
  { // Entry 1001
    0x1.921fb54442d18469898cc51701b839a2p0,
    0x1.0p-1074,
    -0.0
  },
  { // Entry 1002
    0x1.921fb54442d18469898cc51701b839a2p1,
    0x1.fffffffffffffp1023,
    -HUGE_VAL
  },
  { // Entry 1003
    0x1.921fb54442d18469898cc51701b839a2p1,
    0x1.0p-1022,
    -HUGE_VAL
  },
  { // Entry 1004
    0x1.921fb54442d18469898cc51701b839a2p1,
    0x1.0p-1074,
    -HUGE_VAL
  },
  { // Entry 1005
    -0x1.921fb54442d18469898cc51701b839a2p1,
    -0x1.fffffffffffffp1023,
    -HUGE_VAL
  },
  { // Entry 1006
    -0x1.921fb54442d18469898cc51701b839a2p1,
    -0x1.0p-1022,
    -HUGE_VAL
  },
  { // Entry 1007
    -0x1.921fb54442d18469898cc51701b839a2p1,
    -0x1.0p-1074,
    -HUGE_VAL
  },
  { // Entry 1008
    0.0,
    0x1.fffffffffffffp1023,
    HUGE_VAL
  },
  { // Entry 1009
    0.0,
    0x1.0p-1022,
    HUGE_VAL
  },
  { // Entry 1010
    0.0,
    0x1.0p-1074,
    HUGE_VAL
  },
  { // Entry 1011
    -0.0,
    -0x1.fffffffffffffp1023,
    HUGE_VAL
  },
  { // Entry 1012
    -0.0,
    -0x1.0p-1022,
    HUGE_VAL
  },
  { // Entry 1013
    -0.0,
    -0x1.0p-1074,
    HUGE_VAL
  },
  { // Entry 1014
    0x1.921fb54442d18469898cc51701b839a2p0,
    HUGE_VAL,
    0x1.fffffffffffffp1023
  },
  { // Entry 1015
    0x1.921fb54442d18469898cc51701b839a2p0,
    HUGE_VAL,
    0x1.0p-1022
  },
  { // Entry 1016
    0x1.921fb54442d18469898cc51701b839a2p0,
    HUGE_VAL,
    0x1.0p-1074
  },
  { // Entry 1017
    0x1.921fb54442d18469898cc51701b839a2p0,
    HUGE_VAL,
    -0x1.fffffffffffffp1023
  },
  { // Entry 1018
    0x1.921fb54442d18469898cc51701b839a2p0,
    HUGE_VAL,
    -0x1.0p-1022
  },
  { // Entry 1019
    0x1.921fb54442d18469898cc51701b839a2p0,
    HUGE_VAL,
    -0x1.0p-1074
  },
  { // Entry 1020
    -0x1.921fb54442d18469898cc51701b839a2p0,
    -HUGE_VAL,
    0x1.fffffffffffffp1023
  },
  { // Entry 1021
    -0x1.921fb54442d18469898cc51701b839a2p0,
    -HUGE_VAL,
    0x1.0p-1022
  },
  { // Entry 1022
    -0x1.921fb54442d18469898cc51701b839a2p0,
    -HUGE_VAL,
    0x1.0p-1074
  },
  { // Entry 1023
    -0x1.921fb54442d18469898cc51701b839a2p0,
    -HUGE_VAL,
    -0x1.fffffffffffffp1023
  },
  { // Entry 1024
    -0x1.921fb54442d18469898cc51701b839a2p0,
    -HUGE_VAL,
    -0x1.0p-1022
  },
  { // Entry 1025
    -0x1.921fb54442d18469898cc51701b839a2p0,
    -HUGE_VAL,
    -0x1.0p-1074
  },
  { // Entry 1026
    0x1.2d97c7f3321d234f272993d1414a2b39p1,
    HUGE_VAL,
    -HUGE_VAL
  },
  { // Entry 1027
    -0x1.2d97c7f3321d234f272993d1414a2b39p1,
    -HUGE_VAL,
    -HUGE_VAL
  },
  { // Entry 1028
    0x1.921fb54442d18469898cc51701b839a2p-1,
    HUGE_VAL,
    HUGE_VAL
  },
  { // Entry 1029
    -0x1.921fb54442d18469898cc51701b839a2p-1,
    -HUGE_VAL,
    HUGE_VAL
  },
  { // Entry 1030
    0x1.921fb54442d18469898cc51701b839a2p-1,
    0x1.fffffffffffffp1023,
    0x1.fffffffffffffp1023
  },
  { // Entry 1031
    0x1.921fb54442d18469898cc51701b839a2p0,
    0x1.fffffffffffffp1023,
    0x1.0p-1022
  },
  { // Entry 1032
    0x1.921fb54442d18469898cc51701b839a2p0,
    0x1.fffffffffffffp1023,
    0x1.0p-1074
  },
  { // Entry 1033
    0x1.921fb54442d18469898cc51701b839a2p0,
    0x1.fffffffffffffp1023,
    -0x1.0p-1074
  },
  { // Entry 1034
    0x1.921fb54442d18469898cc51701b839a2p0,
    0x1.fffffffffffffp1023,
    -0x1.0p-1022
  },
  { // Entry 1035
    0x1.2d97c7f3321d234f272993d1414a2b39p1,
    0x1.fffffffffffffp1023,
    -0x1.fffffffffffffp1023
  },
  { // Entry 1036
    0.0,
    0x1.0p-1022,
    0x1.fffffffffffffp1023
  },
  { // Entry 1037
    0.0,
    0x1.0p-1074,
    0x1.fffffffffffffp1023
  },
  { // Entry 1038
    -0.0,
    -0x1.0p-1074,
    0x1.fffffffffffffp1023
  },
  { // Entry 1039
    -0.0,
    -0x1.0p-1022,
    0x1.fffffffffffffp1023
  },
  { // Entry 1040
    -0x1.921fb54442d18469898cc51701b839a2p-1,
    -0x1.fffffffffffffp1023,
    0x1.fffffffffffffp1023
  },
  { // Entry 1041
    0x1.921fb54442d18469898cc51701b839a2p-1,
    0x1.0p-1022,
    0x1.0p-1022
  },
  { // Entry 1042
    0x1.921fb54442d17469898cc51701b839a2p0,
    0x1.0p-1022,
    0x1.0p-1074
  },
  { // Entry 1043
    0x1.921fb54442d19469898cc51701b839a2p0,
    0x1.0p-1022,
    -0x1.0p-1074
  },
  { // Entry 1044
    0x1.2d97c7f3321d234f272993d1414a2b39p1,
    0x1.0p-1022,
    -0x1.0p-1022
  },
  { // Entry 1045
    0x1.921fb54442d18469898cc51701b839a2p1,
    0x1.0p-1022,
    -0x1.fffffffffffffp1023
  },
  { // Entry 1046
    0x1.ffffffffffffffffffffffffff555555p-53,
    0x1.0p-1074,
    0x1.0p-1022
  },
  { // Entry 1047
    -0x1.ffffffffffffffffffffffffff555555p-53,
    -0x1.0p-1074,
    0x1.0p-1022
  },
  { // Entry 1048
    -0x1.921fb54442d18469898cc51701b839a2p-1,
    -0x1.0p-1022,
    0x1.0p-1022
  },
  { // Entry 1049
    -0x1.921fb54442d18469898cc51701b839a2p0,
    -0x1.fffffffffffffp1023,
    0x1.0p-1022
  },
  { // Entry 1050
    0x1.921fb54442d18469898cc51701b839a2p-1,
    0x1.0p-1074,
    0x1.0p-1074
  },
  { // Entry 1051
    0x1.2d97c7f3321d234f272993d1414a2b39p1,
    0x1.0p-1074,
    -0x1.0p-1074
  },
  { // Entry 1052
    0x1.921fb54442d17c69898cc51701b839a2p1,
    0x1.0p-1074,
    -0x1.0p-1022
  },
  { // Entry 1053
    0x1.921fb54442d18469898cc51701b839a2p1,
    0x1.0p-1074,
    -0x1.fffffffffffffp1023
  },
  { // Entry 1054
    -0x1.921fb54442d18469898cc51701b839a2p-1,
    -0x1.0p-1074,
    0x1.0p-1074
  },
  { // Entry 1055
    -0x1.921fb54442d17469898cc51701b839a2p0,
    -0x1.0p-1022,
    0x1.0p-1074
  },
  { // Entry 1056
    -0x1.921fb54442d18469898cc51701b839a2p0,
    -0x1.fffffffffffffp1023,
    0x1.0p-1074
  },
  { // Entry 1057
    -0x1.2d97c7f3321d234f272993d1414a2b39p1,
    -0x1.0p-1074,
    -0x1.0p-1074
  },
  { // Entry 1058
    -0x1.921fb54442d17c69898cc51701b839a2p1,
    -0x1.0p-1074,
    -0x1.0p-1022
  },
  { // Entry 1059
    -0x1.921fb54442d18469898cc51701b839a2p1,
    -0x1.0p-1074,
    -0x1.fffffffffffffp1023
  },
  { // Entry 1060
    -0x1.921fb54442d19469898cc51701b839a2p0,
    -0x1.0p-1022,
    -0x1.0p-1074
  },
  { // Entry 1061
    -0x1.921fb54442d18469898cc51701b839a2p0,
    -0x1.fffffffffffffp1023,
    -0x1.0p-1074
  },
  { // Entry 1062
    -0x1.2d97c7f3321d234f272993d1414a2b39p1,
    -0x1.0p-1022,
    -0x1.0p-1022
  },
  { // Entry 1063
    -0x1.921fb54442d18469898cc51701b839a2p1,
    -0x1.0p-1022,
    -0x1.fffffffffffffp1023
  },
  { // Entry 1064
    -0x1.921fb54442d18469898cc51701b839a2p0,
    -0x1.fffffffffffffp1023,
    -0x1.0p-1022
  },
  { // Entry 1065
    -0x1.2d97c7f3321d234f272993d1414a2b39p1,
    -0x1.fffffffffffffp1023,
    -0x1.fffffffffffffp1023
  }
};
