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

static data_1_1_t<double, double> g_sqrt_intel_data[] = {
  { // Entry 0
    0x1.00000000000007ffffffffffffe0p-1,
    0x1.0000000000001p-2
  },
  { // Entry 1
    0x1.00000000000007ffffffffffffe0p-5,
    0x1.0000000000001p-10
  },
  { // Entry 2
    0x1.00000000000007ffffffffffffe0p-20,
    0x1.0000000000001p-40
  },
  { // Entry 3
    0x1.6a09e667f3bcd459022e5304d0b08199p-511,
    0x1.0000000000001p-1021
  },
  { // Entry 4
    0x1.00000000000007ffffffffffffe0p-511,
    0x1.0000000000001p-1022
  },
  { // Entry 5
    0x1.00000000000007ffffffffffffe0p0,
    0x1.0000000000001p0
  },
  { // Entry 6
    0x1.00000000000007ffffffffffffe0p1,
    0x1.0000000000001p2
  },
  { // Entry 7
    0x1.0000000000000fffffffffffff80p0,
    0x1.0000000000002p0
  },
  { // Entry 8
    0x1.6a09e667f3bceaf9a094d2409bd702e2p-3,
    0x1.0000000000003p-5
  },
  { // Entry 9
    0x1.6a09e667f3bceaf9a094d2409bd702e2p-511,
    0x1.0000000000003p-1021
  },
  { // Entry 10
    0x1.00000000000017fffffffffffee0p-511,
    0x1.0000000000003p-1022
  },
  { // Entry 11
    0x1.6a09e667f3bceaf9a094d2409bd702e2p0,
    0x1.0000000000003p1
  },
  { // Entry 12
    0x1.00000000000037fffffffffff9e0p-3,
    0x1.0000000000007p-6
  },
  { // Entry 13
    0x1.00000000000037fffffffffff9e0p-511,
    0x1.0000000000007p-1022
  },
  { // Entry 14
    0x1.00000000000077ffffffffffe3e0p-1,
    0x1.000000000000fp-2
  },
  { // Entry 15
    0x1.6a09e667f40bfb3319b85c0967d96777p-21,
    0x1.00000000007p-41
  },
  { // Entry 16
    0x1.0000003ffffff8000001ffffff60p-10,
    0x1.0000008p-20
  },
  { // Entry 17
    0x1.0000007fffffe000000ffffff6000006p-20,
    0x1.0000010p-40
  },
  { // Entry 18
    0x1.000000ffffef78001087ff66d3e1aa74p-503,
    0x1.000001ffffdffp-1006
  },
  { // Entry 19
    0x1.000000ffffef8000107fff676001a8dfp-509,
    0x1.000001ffffep-1018
  },
  { // Entry 20
    0x1.000001fffffe000003fffff600001bffp50,
    0x1.0000040p100
  },
  { // Entry 21
    0x1.000001fffffe880002effff90be01238p-10,
    0x1.0000040000011p-20
  },
  { // Entry 22
    0x1.6a0a40db7d51f00038bb4d171626c123p-500,
    0x1.00007feafp-999
  },
  { // Entry 23
    0x1.6a2bd5be688300293f34c09a864348a7p-511,
    0x1.003p-1021
  },
  { // Entry 24
    0x1.6a6521b171386b3e9c9708c18094f81ep1,
    0x1.0081159eb7531p3
  },
  { // Entry 25
    0x1.6b20018577e83a548e15ae72516a45e5p1,
    0x1.0189e42871b67p3
  },
  { // Entry 26
    0x1.030dc4eb8784b800006b31393def4b78p-5,
    0x1.0624dd322b9bdp-10
  },
  { // Entry 27
    0x1.030dc4f1684a3000002a2118b9ff4fc3p-5,
    0x1.0624dd3e110d4p-10
  },
  { // Entry 28
    0x1.030dc4f1696f97ffffdbf85c70af7a32p-5,
    0x1.0624dd3e135f1p-10
  },
  { // Entry 29
    0x1.030dc4fce267800000008a6d267d447fp-5,
    0x1.0624dd554b60ap-10
  },
  { // Entry 30
    0x1.6e9b2675a66267ffc501a2a352d80cadp-11,
    0x1.068p-21
  },
  { // Entry 31
    0x1.6e9b2675a66267ffc501a2a352d80cadp-18,
    0x1.068p-35
  },
  { // Entry 32
    0x1.6ede29b025aaf0011c319ebac8dce9fap-11,
    0x1.06ep-21
  },
  { // Entry 33
    0x1.717983890b6a97fffff1c9e6db43dc37p48,
    0x1.0a9fc36f5705dp97
  },
  { // Entry 34
    0x1.752deb01e1aa48002dcf5a4f55adabf7p-4,
    0x1.0fff6b87f90p-7
  },
  { // Entry 35
    0x1.76356020885cca53989372a8049c6ccbp-11,
    0x1.118p-21
  },
  { // Entry 36
    0x1.7b63945a7c4cb40027ac4d7964bdfdffp-11,
    0x1.192p-21
  },
  { // Entry 37
    0x1.83821c9ec9b2a8003b649ec5754fa2e6p-6,
    0x1.2549525495251p-11
  },
  { // Entry 38
    0x1.8ac40868f92c17ff0ecf9e6c802c000ap-11,
    0x1.306p-21
  },
  { // Entry 39
    0x1.1a9dc8f6df10380eb98f9c8f8ada2dc3p-10,
    0x1.380p-20
  },
  { // Entry 40
    0x1.1a9dc8f6df10380eb98f9c8f8ada2dc3p-20,
    0x1.380p-40
  },
  { // Entry 41
    0x1.1d43ad1c267397ff000366e504ec0904p0,
    0x1.3ddfc154bf689p0
  },
  { // Entry 42
    0x1.2d4d2aa66779740440a7ac683ca92be7p0,
    0x1.629e8d8dfe88ep0
  },
  { // Entry 43
    0x1.ae89f995ad3ab3fed29f3cdde669565cp-1,
    0x1.6a09e667f3bc9p-1
  },
  { // Entry 44
    0x1.bb67ae8584caa73b25742d7078b83b89p-537,
    0x1.8p-1073
  },
  { // Entry 45
    0x1.bb67ae86abb307ffff9450222403ce3fp0,
    0x1.80000001fecb9p1
  },
  { // Entry 46
    0x1.404b92fd6a8120001cc2a21eb82dc383p-1,
    0x1.90bd05c8ff254p-2
  },
  { // Entry 47
    0x1.43d1363d61aec800006b07fe0fefdfa2p-2,
    0x1.999999d880368p-4
  },
  { // Entry 48
    0x1.4e78ac22c6f5e800ffc121b0def932c0p5,
    0x1.b4ff1a0c9382fp10
  },
  { // Entry 49
    0x1.50144b1c72dd17ff0012ebc586f4e10fp-519,
    0x1.b93546c68p-1038
  },
  { // Entry 50
    0x1.dfd052dbe76857ff0b1587ce79a18a73p7,
    0x1.c1a69fccd6111p15
  },
  { // Entry 51
    0x1.e4826468545d31f66cbd21db9f6249b7p-1,
    0x1.ca7ea70a502bep-1
  },
  { // Entry 52
    0x1.e96948d224f0585c0f7a85d30932126dp1,
    0x1.d3d1b1bfd11bbp3
  },
  { // Entry 53
    0x1.ee51da20312bfe8a4722b6c085901297p-512,
    0x1.dd3fffffffffep-1023
  },
  { // Entry 54
    0x1.5f744159f7e5efff106d1c5d5d64aa8dp-10,
    0x1.e28p-20
  },
  { // Entry 55
    0x1.fdcecc6f3d49e79e3d375b22e0b9f4b1p-11,
    0x1.fbap-21
  },
  { // Entry 56
    0x1.fe35a055fc4be7fc5f7d57d28e7bd1a7p0,
    0x1.fc6cdb0930a24p1
  },
  { // Entry 57
    0x1.feadd22799ac4801f8159ee8a6db5ef2p-1,
    0x1.fd5c83adbf2a9p-1
  },
  { // Entry 58
    0x1.ffbffbff80080000000200300a02205cp-5,
    0x1.ff80000000380p-9
  },
  { // Entry 59
    0x1.ffefffbffdffdbfe9fef7f2bf4ab6197p-512,
    0x1.ffdfffffffffep-1023
  },
  { // Entry 60
    0x1.ffff7fffeffff3fffcbffecfff75ffc0p-21,
    0x1.fffefffffffffp-41
  },
  { // Entry 61
    0x1.fffff3ffffdbf7ff27cff9aa4fbac71dp-21,
    0x1.ffffe7fffffffp-41
  },
  { // Entry 62
    0x1.fffff3ffffdbf7ff27cff9aa4fbac71dp-23,
    0x1.ffffe7fffffffp-45
  },
  { // Entry 63
    0x1.ffffff800000e0000037ffffdcffffdep-488,
    0x1.ffffff000001ep-975
  },
  { // Entry 64
    0x1.ffffff8000010000003fffffcfffffd4p-26,
    0x1.ffffff0000022p-51
  },
  { // Entry 65
    0x1.fffffff9fffff7f6ffffe7e4ffef939ap-21,
    0x1.fffffff3fffffp-41
  },
  { // Entry 66
    0x1.fffffffe000017ff000017feff7023fep-5,
    0x1.fffffffc00003p-9
  },
  { // Entry 67
    0x1.fffffffe000017ff000017feff7023fep-6,
    0x1.fffffffc00003p-11
  },
  { // Entry 68
    0x1.fffffffe000017ff000017feff7023fep-21,
    0x1.fffffffc00003p-41
  },
  { // Entry 69
    0x1.fffffffe000017ff000017feff7023fep-156,
    0x1.fffffffc00003p-311
  },
  { // Entry 70
    0x1.fffffffe000017ff000017feff7023fep-511,
    0x1.fffffffc00003p-1021
  },
  { // Entry 71
    0x1.fffffffe3ffff7ff3bfff8ff546ff6cfp-21,
    0x1.fffffffc7ffffp-41
  },
  { // Entry 72
    0x1.6a09e667f3b858019b5c99e309b9080ap-3,
    0x1.fffffffffff37p-6
  },
  { // Entry 73
    0x1.fffffffffffd77fffffffffe65efffffp-1,
    0x1.fffffffffffafp-1
  },
  { // Entry 74
    0x1.fffffffffffd77fffffffffe65efffffp-5,
    0x1.fffffffffffafp-9
  },
  { // Entry 75
    0x1.fffffffffffd77fffffffffe65efffffp-511,
    0x1.fffffffffffafp-1021
  },
  { // Entry 76
    0x1.ffffffffffff37ffffffffffd8efffffp1,
    0x1.fffffffffffe7p3
  },
  { // Entry 77
    0x1.ffffffffffffefffffffffffffbfffffp-4,
    0x1.ffffffffffffep-7
  },
  { // Entry 78
    0x1.ffffffffffffefffffffffffffbfffffp-21,
    0x1.ffffffffffffep-41
  },
  { // Entry 79
    0x1.ffffffffffffefffffffffffffbfffffp-511,
    0x1.ffffffffffffep-1021
  },
  { // Entry 80
    0x1.fffffffffffff7ffffffffffffefffffp-1,
    0x1.fffffffffffffp-1
  },
  { // Entry 81
    0x1.fffffffffffff7ffffffffffffefffffp-4,
    0x1.fffffffffffffp-7
  },
  { // Entry 82
    0x1.fffffffffffff7ffffffffffffefffffp-21,
    0x1.fffffffffffffp-41
  },
  { // Entry 83
    0x1.fffffffffffff7ffffffffffffefffffp0,
    0x1.fffffffffffffp1
  },
  { // Entry 84
    0x1.306fe0a31b71419ddec788789fb4580ap-1,
    0x1.6a09e667f3bcap-2
  },
  { // Entry 85
    0x1.306fe0a31b71485806addf2d8b5a8b60p-1,
    0x1.6a09e667f3bcbp-2
  },
  { // Entry 86
    0x1.306fe0a31b714f122e9435e276dab0b9p-1,
    0x1.6a09e667f3bccp-2
  },
  { // Entry 87
    0x1.306fe0a31b7155cc567a8c976234c817p-1,
    0x1.6a09e667f3bcdp-2
  },
  { // Entry 88
    0x1.306fe0a31b715c867e60e34c4d68d179p-1,
    0x1.6a09e667f3bcep-2
  },
  { // Entry 89
    0x1.ae89f995ad3abd8251a455b971a538dbp-1,
    0x1.6a09e667f3bcap-1
  },
  { // Entry 90
    0x1.ae89f995ad3ac705d0a96e94fcab4a1ap-1,
    0x1.6a09e667f3bcbp-1
  },
  { // Entry 91
    0x1.ae89f995ad3ad0894fae8770877b8a1bp-1,
    0x1.6a09e667f3bccp-1
  },
  { // Entry 92
    0x1.ae89f995ad3ada0cceb3a04c1215f8ddp-1,
    0x1.6a09e667f3bcdp-1
  },
  { // Entry 93
    0x1.ae89f995ad3ae3904db8b9279c7a965fp-1,
    0x1.6a09e667f3bcep-1
  },
  { // Entry 94
    0x1.306fe0a31b71419ddec788789fb4580ap0,
    0x1.6a09e667f3bcap0
  },
  { // Entry 95
    0x1.306fe0a31b71485806addf2d8b5a8b60p0,
    0x1.6a09e667f3bcbp0
  },
  { // Entry 96
    0x1.306fe0a31b714f122e9435e276dab0b9p0,
    0x1.6a09e667f3bccp0
  },
  { // Entry 97
    0x1.306fe0a31b7155cc567a8c976234c817p0,
    0x1.6a09e667f3bcdp0
  },
  { // Entry 98
    0x1.306fe0a31b715c867e60e34c4d68d179p0,
    0x1.6a09e667f3bcep0
  },
  { // Entry 99
    0x1.ae89f995ad3abd8251a455b971a538dbp0,
    0x1.6a09e667f3bcap1
  },
  { // Entry 100
    0x1.ae89f995ad3ac705d0a96e94fcab4a1ap0,
    0x1.6a09e667f3bcbp1
  },
  { // Entry 101
    0x1.ae89f995ad3ad0894fae8770877b8a1bp0,
    0x1.6a09e667f3bccp1
  },
  { // Entry 102
    0x1.ae89f995ad3ada0cceb3a04c1215f8ddp0,
    0x1.6a09e667f3bcdp1
  },
  { // Entry 103
    0x1.ae89f995ad3ae3904db8b9279c7a965fp0,
    0x1.6a09e667f3bcep1
  },
  { // Entry 104
    0x1.fffffffffffff7ffffffffffffefffffp-4,
    0x1.fffffffffffffp-7
  },
  { // Entry 105
    0x1.p-3,
    0x1.0p-6
  },
  { // Entry 106
    0x1.00000000000007ffffffffffffe0p-3,
    0x1.0000000000001p-6
  },
  { // Entry 107
    0x1.6a09e667f3bcc3608b617397f7660a23p-3,
    0x1.fffffffffffffp-6
  },
  { // Entry 108
    0x1.6a09e667f3bcc908b2fb1366ea957d3ep-3,
    0x1.0p-5
  },
  { // Entry 109
    0x1.6a09e667f3bcd459022e5304d0b08199p-3,
    0x1.0000000000001p-5
  },
  { // Entry 110
    0x1.fffffffffffff7ffffffffffffefffffp-3,
    0x1.fffffffffffffp-5
  },
  { // Entry 111
    0x1.p-2,
    0x1.0p-4
  },
  { // Entry 112
    0x1.00000000000007ffffffffffffe0p-2,
    0x1.0000000000001p-4
  },
  { // Entry 113
    0x1.6a09e667f3bcc3608b617397f7660a23p-2,
    0x1.fffffffffffffp-4
  },
  { // Entry 114
    0x1.6a09e667f3bcc908b2fb1366ea957d3ep-2,
    0x1.0p-3
  },
  { // Entry 115
    0x1.6a09e667f3bcd459022e5304d0b08199p-2,
    0x1.0000000000001p-3
  },
  { // Entry 116
    0x1.fffffffffffff7ffffffffffffefffffp-2,
    0x1.fffffffffffffp-3
  },
  { // Entry 117
    0x1.p-1,
    0x1.0p-2
  },
  { // Entry 118
    0x1.00000000000007ffffffffffffe0p-1,
    0x1.0000000000001p-2
  },
  { // Entry 119
    0x1.6a09e667f3bcc3608b617397f7660a23p-1,
    0x1.fffffffffffffp-2
  },
  { // Entry 120
    0x1.6a09e667f3bcc908b2fb1366ea957d3ep-1,
    0x1.0p-1
  },
  { // Entry 121
    0x1.6a09e667f3bcd459022e5304d0b08199p-1,
    0x1.0000000000001p-1
  },
  { // Entry 122
    0x1.fffffffffffff7ffffffffffffefffffp-1,
    0x1.fffffffffffffp-1
  },
  { // Entry 123
    0x1.p0,
    0x1.0p0
  },
  { // Entry 124
    0x1.00000000000007ffffffffffffe0p0,
    0x1.0000000000001p0
  },
  { // Entry 125
    0x1.6a09e667f3bcc3608b617397f7660a23p0,
    0x1.fffffffffffffp0
  },
  { // Entry 126
    0x1.6a09e667f3bcc908b2fb1366ea957d3ep0,
    0x1.0p1
  },
  { // Entry 127
    0x1.6a09e667f3bcd459022e5304d0b08199p0,
    0x1.0000000000001p1
  },
  { // Entry 128
    0x1.fffffffffffff7ffffffffffffefffffp0,
    0x1.fffffffffffffp1
  },
  { // Entry 129
    0x1.p1,
    0x1.0p2
  },
  { // Entry 130
    0x1.00000000000007ffffffffffffe0p1,
    0x1.0000000000001p2
  },
  { // Entry 131
    -0.0,
    -0.0
  },
  { // Entry 132
    0x1.p-537,
    0x1.0p-1074
  },
  { // Entry 133
    0x1.ffffffffffffefffffffffffffbfffffp-512,
    0x1.ffffffffffffep-1023
  },
  { // Entry 134
    0x1.p-511,
    0x1.0p-1022
  },
  { // Entry 135
    0x1.00000000000007ffffffffffffe0p-511,
    0x1.0000000000001p-1022
  },
  { // Entry 136
    0x1.p-537,
    0x1.0p-1074
  },
  { // Entry 137
    0x1.279a74590331d74bc03dae7e16ded15bp-512,
    0x1.5555555555558p-1024
  },
  { // Entry 138
    0x1.a20bd700c2c3e64872281df887e3cbf1p-512,
    0x1.5555555555556p-1023
  },
  { // Entry 139
    0x1.ffffffffffffefffffffffffffbfffffp-512,
    0x1.ffffffffffffep-1023
  },
  { // Entry 140
    0x1.ffffffffece09fffffa494f9e6fc95edp-1,
    0x1.ffffffffd9c14p-1
  },
  { // Entry 141
    0x1.ffffffffece0a7ffffa49546646c95f1p-1,
    0x1.ffffffffd9c15p-1
  },
  { // Entry 142
    0x1.ffffffffece0afffffa49592e1bc95f5p-1,
    0x1.ffffffffd9c16p-1
  },
  { // Entry 143
    0x1.ffffffffece0b7ffffa495df5eec95fap-1,
    0x1.ffffffffd9c17p-1
  },
  { // Entry 144
    0x1.ffffffffece0bfffffa4962bdbfc95fep-1,
    0x1.ffffffffd9c18p-1
  },
  { // Entry 145
    0x1.ffffffffeae35fffff9092a326fb67c8p-1,
    0x1.ffffffffd5c6cp-1
  },
  { // Entry 146
    0x1.ffffffffeae367ffff9092f7996b67cep-1,
    0x1.ffffffffd5c6dp-1
  },
  { // Entry 147
    0x1.ffffffffeae36fffff90934c0bbb67d3p-1,
    0x1.ffffffffd5c6ep-1
  },
  { // Entry 148
    0x1.ffffffffeae377ffff9093a07deb67d8p-1,
    0x1.ffffffffd5c6fp-1
  },
  { // Entry 149
    0x1.ffffffffeae37fffff9093f4effb67ddp-1,
    0x1.ffffffffd5c70p-1
  },
  { // Entry 150
    0x1.fffffffffffff7ffffffffffffefffffp511,
    0x1.fffffffffffffp1023
  },
  { // Entry 151
    HUGE_VAL,
    HUGE_VAL
  },
  { // Entry 152
    0x1.fffffffffffff7ffffffffffffefffffp511,
    0x1.fffffffffffffp1023
  },
  { // Entry 153
    0x1.ffffffffffffefffffffffffffbfffffp511,
    0x1.ffffffffffffep1023
  },
  { // Entry 154
    0x1.c5bf891b4ef6a7fc7dc11ccf9559536ep0,
    0x1.921fb54442d18p1
  },
  { // Entry 155
    0x1.40d931ff627057a2dddf7c87edb63664p0,
    0x1.921fb54442d18p0
  },
  { // Entry 156
    0x1.00000000000007ffffffffffffe0p0,
    0x1.0000000000001p0
  },
  { // Entry 157
    0x1.p0,
    0x1.0p0
  },
  { // Entry 158
    0x1.fffffffffffff7ffffffffffffefffffp-1,
    0x1.fffffffffffffp-1
  },
  { // Entry 159
    0x1.c5bf891b4ef6a7fc7dc11ccf9559536ep-1,
    0x1.921fb54442d18p-1
  },
  { // Entry 160
    0x1.00000000000007ffffffffffffe0p-511,
    0x1.0000000000001p-1022
  },
  { // Entry 161
    0x1.p-511,
    0x1.0p-1022
  },
  { // Entry 162
    0x1.ffffffffffffefffffffffffffbfffffp-512,
    0x1.ffffffffffffep-1023
  },
  { // Entry 163
    0x1.ffffffffffffdffffffffffffeffffffp-512,
    0x1.ffffffffffffcp-1023
  },
  { // Entry 164
    0x1.6a09e667f3bcc908b2fb1366ea957d3ep-537,
    0x1.0p-1073
  },
  { // Entry 165
    0x1.p-537,
    0x1.0p-1074
  },
  { // Entry 166
    0.0,
    0.0
  },
  { // Entry 167
    -0.0,
    -0.0
  },
  { // Entry 168
    0x1.p1,
    0x1.0p2
  },
  { // Entry 169
    0x1.p2,
    0x1.0p4
  },
  { // Entry 170
    0x1.80p1,
    0x1.2p3
  },
  { // Entry 171
    0x1.40p2,
    0x1.9p4
  },
  { // Entry 172
    0x1.p-1,
    0x1.0p-2
  },
  { // Entry 173
    0x1.c0p2,
    0x1.880p5
  },
  { // Entry 174
    0x1.40p3,
    0x1.9p6
  }
};
