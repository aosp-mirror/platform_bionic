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

static data_1_1_t<float, float> g_tanhf_intel_data[] = {
  { // Entry 0
    -0x1.ff5717ff015e292b197aa6a5bd8ead11p-5,
    -0x1.0000c0p-4
  },
  { // Entry 1
    0x1.ff5717ff015e292b197aa6a5bd8ead11p-5,
    0x1.0000c0p-4
  },
  { // Entry 2
    -0x1.fffffc0001871106009b5aaf55c49116p-1,
    -0x1.0a2b2ap3
  },
  { // Entry 3
    0x1.fffffc0001871106009b5aaf55c49116p-1,
    0x1.0a2b2ap3
  },
  { // Entry 4
    -0x1.f2c4e0c5399bd4c3248a88570584900dp-2,
    -0x1.107fa4p-1
  },
  { // Entry 5
    0x1.f2c4e0c5399bd4c3248a88570584900dp-2,
    0x1.107fa4p-1
  },
  { // Entry 6
    -0x1.968428ffd872ecb6c9fd8e70df8a8bfep-1,
    -0x1.150498p0
  },
  { // Entry 7
    0x1.968428ffd872ecb6c9fd8e70df8a8bfep-1,
    0x1.150498p0
  },
  { // Entry 8
    -0x1.fffffdfd7b7e46383ce8fadc5f8ea7eap-1,
    -0x1.152e2ep3
  },
  { // Entry 9
    0x1.fffffdfd7b7e46383ce8fadc5f8ea7eap-1,
    0x1.152e2ep3
  },
  { // Entry 10
    -0x1.189751ff578effbe5ec0f53fb816c705p-4,
    -0x1.1908p-4
  },
  { // Entry 11
    0x1.189751ff578effbe5ec0f53fb816c705p-4,
    0x1.1908p-4
  },
  { // Entry 12
    -0x1.fffffef20b998e5904f90043100cd5d3p-1,
    -0x1.1f80p3
  },
  { // Entry 13
    0x1.fffffef20b998e5904f90043100cd5d3p-1,
    0x1.1f80p3
  },
  { // Entry 14
    -0x1.fff9f601c71a50ea8042b6e535d99676p-1,
    -0x1.55b54ep2
  },
  { // Entry 15
    0x1.fff9f601c71a50ea8042b6e535d99676p-1,
    0x1.55b54ep2
  },
  { // Entry 16
    -0x1.35fec061664b9cf1b7b1484307f84259p-1,
    -0x1.674804p-1
  },
  { // Entry 17
    0x1.35fec061664b9cf1b7b1484307f84259p-1,
    0x1.674804p-1
  },
  { // Entry 18
    -0x1.8d9b1b0128196f89d9531df123a40ff4p-11,
    -0x1.8d9b20p-11
  },
  { // Entry 19
    0x1.8d9b1b0128196f89d9531df123a40ff4p-11,
    0x1.8d9b20p-11
  },
  { // Entry 20
    -0x1.e3b21701a09ce46bedfb3f6e4f5f9a30p-11,
    -0x1.e3b220p-11
  },
  { // Entry 21
    0x1.e3b21701a09ce46bedfb3f6e4f5f9a30p-11,
    0x1.e3b220p-11
  },
  { // Entry 22
    -0x1.fb070301e5b46bfdaf1d2a7bd9573b3cp-8,
    -0x1.fb099ap-8
  },
  { // Entry 23
    0x1.fb070301e5b46bfdaf1d2a7bd9573b3cp-8,
    0x1.fb099ap-8
  },
  { // Entry 24
    -0x1.ff85d977c45b8a463709fc31915c04bap-6,
    -0x1.ffb06cp-6
  },
  { // Entry 25
    0x1.ff85d977c45b8a463709fc31915c04bap-6,
    0x1.ffb06cp-6
  },
  { // Entry 26
    -0x1.ff5517ffac72f276baf86702efd7f0e4p-5,
    -0x1.ffff7ep-5
  },
  { // Entry 27
    0x1.ff5517ffac72f276baf86702efd7f0e4p-5,
    0x1.ffff7ep-5
  },
  { // Entry 28
    0x1.85efacff5cf7afdba442be92190b551bp-1,
    0x1.000002p0
  },
  { // Entry 29
    -0x1.85efacff5cf7afdba442be92190b551bp-1,
    -0x1.000002p0
  },
  { // Entry 30
    0x1.ffffffffffffffffffffffffffffffffp-1,
    0x1.000002p8
  },
  { // Entry 31
    -0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.000002p8
  },
  { // Entry 32
    0x1.ffffffffffffffffffffffffffffffffp-1,
    0x1.000002p40
  },
  { // Entry 33
    -0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.000002p40
  },
  { // Entry 34
    0x1.ff5717ff015e292b197aa6a5bd8ead11p-5,
    0x1.0000c0p-4
  },
  { // Entry 35
    -0x1.ff5717ff015e292b197aa6a5bd8ead11p-5,
    -0x1.0000c0p-4
  },
  { // Entry 36
    0x1.ffd7b90143bb5d654465fbf6eb08db94p-6,
    0x1.000130p-5
  },
  { // Entry 37
    -0x1.ffd7b90143bb5d654465fbf6eb08db94p-6,
    -0x1.000130p-5
  },
  { // Entry 38
    0x1.ff5816ffa9aaf77b52ddf52d862ee085p-5,
    0x1.000140p-4
  },
  { // Entry 39
    -0x1.ff5816ffa9aaf77b52ddf52d862ee085p-5,
    -0x1.000140p-4
  },
  { // Entry 40
    0x1.fd5c86dbf1ef7c0b7c17b5a5fc9384bap-4,
    0x1.000180p-3
  },
  { // Entry 41
    -0x1.fd5c86dbf1ef7c0b7c17b5a5fc9384bap-4,
    -0x1.000180p-3
  },
  { // Entry 42
    0x1.fff8d56199d2d1496393498af57a65c5p-7,
    0x1.0001c0p-6
  },
  { // Entry 43
    -0x1.fff8d56199d2d1496393498af57a65c5p-7,
    -0x1.0001c0p-6
  },
  { // Entry 44
    0x1.ff5a1500f7488c88a28967ef3279598bp-5,
    0x1.000240p-4
  },
  { // Entry 45
    -0x1.ff5a1500f7488c88a28967ef3279598bp-5,
    -0x1.000240p-4
  },
  { // Entry 46
    0x1.ffdd57997869c223859da246115830a8p-6,
    0x1.0004p-5
  },
  { // Entry 47
    -0x1.ffdd57997869c223859da246115830a8p-6,
    -0x1.0004p-5
  },
  { // Entry 48
    0x1.0003ffffffeaa9aaa6aaa777a223777cp-21,
    0x1.0004p-21
  },
  { // Entry 49
    -0x1.0003ffffffeaa9aaa6aaa777a223777cp-21,
    -0x1.0004p-21
  },
  { // Entry 50
    0x1.fffed101db6291871ee5b1f184662c14p-7,
    0x1.0004bep-6
  },
  { // Entry 51
    -0x1.fffed101db6291871ee5b1f184662c14p-7,
    -0x1.0004bep-6
  },
  { // Entry 52
    0x1.ffa82cfe3d3102474919a811a60807b3p-1,
    0x1.0008p2
  },
  { // Entry 53
    -0x1.ffa82cfe3d3102474919a811a60807b3p-1,
    -0x1.0008p2
  },
  { // Entry 54
    0x1.00718b006b52c2ab11702de3389d77dep-5,
    0x1.0087p-5
  },
  { // Entry 55
    -0x1.00718b006b52c2ab11702de3389d77dep-5,
    -0x1.0087p-5
  },
  { // Entry 56
    0x1.ffaacb36b2dfa869b4fddee9c98dc067p-1,
    0x1.01p2
  },
  { // Entry 57
    -0x1.ffaacb36b2dfa869b4fddee9c98dc067p-1,
    -0x1.01p2
  },
  { // Entry 58
    0x1.fffff900052e44ea345da5965f4d110ep-1,
    0x1.0137p3
  },
  { // Entry 59
    -0x1.fffff900052e44ea345da5965f4d110ep-1,
    -0x1.0137p3
  },
  { // Entry 60
    0x1.fc32f80000dd7b300750a0078a9054c1p-3,
    0x1.03843ep-2
  },
  { // Entry 61
    -0x1.fc32f80000dd7b300750a0078a9054c1p-3,
    -0x1.03843ep-2
  },
  { // Entry 62
    0x1.fd19570002093df94b65683f9a00917fp-3,
    0x1.03ffp-2
  },
  { // Entry 63
    -0x1.fd19570002093df94b65683f9a00917fp-3,
    -0x1.03ffp-2
  },
  { // Entry 64
    0x1.fde5d8ffff80b927d4d76ae1dce6f59bp-3,
    0x1.046cp-2
  },
  { // Entry 65
    -0x1.fde5d8ffff80b927d4d76ae1dce6f59bp-3,
    -0x1.046cp-2
  },
  { // Entry 66
    0x1.feb06fff85b879ae19eb2bc5cf70e031p-3,
    0x1.04d8p-2
  },
  { // Entry 67
    -0x1.feb06fff85b879ae19eb2bc5cf70e031p-3,
    -0x1.04d8p-2
  },
  { // Entry 68
    0x1.e6c426ed700f503e6583df7694a4979bp-2,
    0x1.08b038p-1
  },
  { // Entry 69
    -0x1.e6c426ed700f503e6583df7694a4979bp-2,
    -0x1.08b038p-1
  },
  { // Entry 70
    0x1.fffffc0001871106009b5aaf55c49116p-1,
    0x1.0a2b2ap3
  },
  { // Entry 71
    -0x1.fffffc0001871106009b5aaf55c49116p-1,
    -0x1.0a2b2ap3
  },
  { // Entry 72
    0x1.ffc392f4ab8d534aa69f7c5166cf0366p-1,
    0x1.0cp2
  },
  { // Entry 73
    -0x1.ffc392f4ab8d534aa69f7c5166cf0366p-1,
    -0x1.0cp2
  },
  { // Entry 74
    0x1.0acd33000032a880878690d403afbcbep-3,
    0x1.0c5392p-3
  },
  { // Entry 75
    -0x1.0acd33000032a880878690d403afbcbep-3,
    -0x1.0c5392p-3
  },
  { // Entry 76
    0x1.fffffcfb1bf37b0f791d7a17eb6b0a15p-1,
    0x1.0eab7cp3
  },
  { // Entry 77
    -0x1.fffffcfb1bf37b0f791d7a17eb6b0a15p-1,
    -0x1.0eab7cp3
  },
  { // Entry 78
    0x1.fffffcfb1e37253dc76193d1e1aed817p-1,
    0x1.0eab88p3
  },
  { // Entry 79
    -0x1.fffffcfb1e37253dc76193d1e1aed817p-1,
    -0x1.0eab88p3
  },
  { // Entry 80
    0x1.fffffcfb3902021b406b1ea187844f4bp-1,
    0x1.0eac16p3
  },
  { // Entry 81
    -0x1.fffffcfb3902021b406b1ea187844f4bp-1,
    -0x1.0eac16p3
  },
  { // Entry 82
    0x1.f5443f00c7ad11ba27b4800bc8cff692p-2,
    0x1.12236ap-1
  },
  { // Entry 83
    -0x1.f5443f00c7ad11ba27b4800bc8cff692p-2,
    -0x1.12236ap-1
  },
  { // Entry 84
    0x1.0db18affff41c0dc320851eef3614bffp-2,
    0x1.1433d2p-2
  },
  { // Entry 85
    -0x1.0db18affff41c0dc320851eef3614bffp-2,
    -0x1.1433d2p-2
  },
  { // Entry 86
    0x1.fffffdfdabb86f8672c461c08d04046fp-1,
    0x1.152faep3
  },
  { // Entry 87
    -0x1.fffffdfdabb86f8672c461c08d04046fp-1,
    -0x1.152faep3
  },
  { // Entry 88
    0x1.fa265d240155e5013f12a3b41500189ep-2,
    0x1.155cp-1
  },
  { // Entry 89
    -0x1.fa265d240155e5013f12a3b41500189ep-2,
    -0x1.155cp-1
  },
  { // Entry 90
    0x1.fc9a1a5f9c32905aaf97cba3f8aafaf0p-2,
    0x1.16fcp-1
  },
  { // Entry 91
    -0x1.fc9a1a5f9c32905aaf97cba3f8aafaf0p-2,
    -0x1.16fcp-1
  },
  { // Entry 92
    0x1.155868ffff4152df457a24ad6f413bdfp-3,
    0x1.170f48p-3
  },
  { // Entry 93
    -0x1.155868ffff4152df457a24ad6f413bdfp-3,
    -0x1.170f48p-3
  },
  { // Entry 94
    0x1.177d0f00f641d1e96c79c7abd75269b1p-6,
    0x1.1784p-6
  },
  { // Entry 95
    -0x1.177d0f00f641d1e96c79c7abd75269b1p-6,
    -0x1.1784p-6
  },
  { // Entry 96
    0x1.189751ff578effbe5ec0f53fb816c705p-4,
    0x1.1908p-4
  },
  { // Entry 97
    -0x1.189751ff578effbe5ec0f53fb816c705p-4,
    -0x1.1908p-4
  },
  { // Entry 98
    0x1.1dce250138c3f920efea001b064975a3p-7,
    0x1.1dd0p-7
  },
  { // Entry 99
    -0x1.1dce250138c3f920efea001b064975a3p-7,
    -0x1.1dd0p-7
  },
  { // Entry 100
    0x1.1dda24c562621a0640fe0b86641cd64bp-7,
    0x1.1ddcp-7
  },
  { // Entry 101
    -0x1.1dda24c562621a0640fe0b86641cd64bp-7,
    -0x1.1ddcp-7
  },
  { // Entry 102
    0x1.1fa86e8f695d32c4d8d874744814444dp-6,
    0x1.1fb0p-6
  },
  { // Entry 103
    -0x1.1fa86e8f695d32c4d8d874744814444dp-6,
    -0x1.1fb0p-6
  },
  { // Entry 104
    0x1.a01401001045019528db07ebeec35ef9p-1,
    0x1.2281aap0
  },
  { // Entry 105
    -0x1.a01401001045019528db07ebeec35ef9p-1,
    -0x1.2281aap0
  },
  { // Entry 106
    0x1.aeea270075d91da2ac1928b5d795a866p-1,
    0x1.3a0b6cp0
  },
  { // Entry 107
    -0x1.aeea270075d91da2ac1928b5d795a866p-1,
    -0x1.3a0b6cp0
  },
  { // Entry 108
    0x1.58840500489a56042910d82b4425107cp-10,
    0x1.588412p-10
  },
  { // Entry 109
    -0x1.58840500489a56042910d82b4425107cp-10,
    -0x1.588412p-10
  },
  { // Entry 110
    0x1.fffc48bea6ea7bca9e3cc76637bfbfcep-1,
    0x1.653fbcp2
  },
  { // Entry 111
    -0x1.fffc48bea6ea7bca9e3cc76637bfbfcep-1,
    -0x1.653fbcp2
  },
  { // Entry 112
    0x1.fffc48d5dfe4c3c0ebf5fc4e969507bap-1,
    0x1.654084p2
  },
  { // Entry 113
    -0x1.fffc48d5dfe4c3c0ebf5fc4e969507bap-1,
    -0x1.654084p2
  },
  { // Entry 114
    0x1.fffc5933b716ebf93b846ed2d0629240p-1,
    0x1.65ceb4p2
  },
  { // Entry 115
    -0x1.fffc5933b716ebf93b846ed2d0629240p-1,
    -0x1.65ceb4p2
  },
  { // Entry 116
    0x1.6cc3070142cab25a8a3556c9810da1e8p-5,
    0x1.6d00d0p-5
  },
  { // Entry 117
    -0x1.6cc3070142cab25a8a3556c9810da1e8p-5,
    -0x1.6d00d0p-5
  },
  { // Entry 118
    0x1.6cc309003eed18aedf499413fadb462fp-5,
    0x1.6d00d2p-5
  },
  { // Entry 119
    -0x1.6cc309003eed18aedf499413fadb462fp-5,
    -0x1.6d00d2p-5
  },
  { // Entry 120
    0x1.fd3f72ffe427753610423980d1d05fedp-1,
    0x1.7aa642p1
  },
  { // Entry 121
    -0x1.fd3f72ffe427753610423980d1d05fedp-1,
    -0x1.7aa642p1
  },
  { // Entry 122
    0x1.7c2f60ffff4224b41b98aa2e87d40e93p-3,
    0x1.80a516p-3
  },
  { // Entry 123
    -0x1.7c2f60ffff4224b41b98aa2e87d40e93p-3,
    -0x1.80a516p-3
  },
  { // Entry 124
    0x1.fffec68f7d9cae8cb2022d2f6e1cb483p-1,
    0x1.88c660p2
  },
  { // Entry 125
    -0x1.fffec68f7d9cae8cb2022d2f6e1cb483p-1,
    -0x1.88c660p2
  },
  { // Entry 126
    0x1.8959449ca3adfa7d322370460455a902p-9,
    0x1.895992p-9
  },
  { // Entry 127
    -0x1.8959449ca3adfa7d322370460455a902p-9,
    -0x1.895992p-9
  },
  { // Entry 128
    0x1.fffecc86683d8ef8a1fb2cafaea1545fp-1,
    0x1.8963c4p2
  },
  { // Entry 129
    -0x1.fffecc86683d8ef8a1fb2cafaea1545fp-1,
    -0x1.8963c4p2
  },
  { // Entry 130
    0x1.79b6110000baefbe3999a6dff4c4acdbp-2,
    0x1.8c6448p-2
  },
  { // Entry 131
    -0x1.79b6110000baefbe3999a6dff4c4acdbp-2,
    -0x1.8c6448p-2
  },
  { // Entry 132
    0x1.9556a2f5d933a6a7c7cf78568bb7249ap-7,
    0x1.955beep-7
  },
  { // Entry 133
    -0x1.9556a2f5d933a6a7c7cf78568bb7249ap-7,
    -0x1.955beep-7
  },
  { // Entry 134
    0x1.a6460901ad2d3a62ccc87319783ddde8p-8,
    0x1.a64788p-8
  },
  { // Entry 135
    -0x1.a6460901ad2d3a62ccc87319783ddde8p-8,
    -0x1.a64788p-8
  },
  { // Entry 136
    0x1.62faf5001c03bf4b64fe9125b3ecbd72p-1,
    0x1.b569c2p-1
  },
  { // Entry 137
    -0x1.62faf5001c03bf4b64fe9125b3ecbd72p-1,
    -0x1.b569c2p-1
  },
  { // Entry 138
    0x1.ffffb2f40c1a3456b2931f183789727dp-1,
    0x1.b5ad60p2
  },
  { // Entry 139
    -0x1.ffffb2f40c1a3456b2931f183789727dp-1,
    -0x1.b5ad60p2
  },
  { // Entry 140
    0x1.b855a8c8313c4cd13e0f00b884857ca3p-7,
    0x1.b85c72p-7
  },
  { // Entry 141
    -0x1.b855a8c8313c4cd13e0f00b884857ca3p-7,
    -0x1.b85c72p-7
  },
  { // Entry 142
    0x1.af0793000035655bbe779d897c9f5d4ep-2,
    0x1.cb9714p-2
  },
  { // Entry 143
    -0x1.af0793000035655bbe779d897c9f5d4ep-2,
    -0x1.cb9714p-2
  },
  { // Entry 144
    0x1.cf812104022313677123e2625c6cb00bp-5,
    0x1.cffffep-5
  },
  { // Entry 145
    -0x1.cf812104022313677123e2625c6cb00bp-5,
    -0x1.cffffep-5
  },
  { // Entry 146
    0x1.d5132b00e44b3c951c3d25000110a656p-4,
    0x1.d7244cp-4
  },
  { // Entry 147
    -0x1.d5132b00e44b3c951c3d25000110a656p-4,
    -0x1.d7244cp-4
  },
  { // Entry 148
    0x1.bc797cffffff6db7359d1c595930bc63p-2,
    0x1.dc0accp-2
  },
  { // Entry 149
    -0x1.bc797cffffff6db7359d1c595930bc63p-2,
    -0x1.dc0accp-2
  },
  { // Entry 150
    0x1.dd556501c8476b8826b0d4995a3e0054p-7,
    0x1.dd5e0ap-7
  },
  { // Entry 151
    -0x1.dd556501c8476b8826b0d4995a3e0054p-7,
    -0x1.dd5e0ap-7
  },
  { // Entry 152
    0x1.dfe86501ca363c2f1da356d0632fe6c8p-7,
    0x1.dff12ep-7
  },
  { // Entry 153
    -0x1.dfe86501ca363c2f1da356d0632fe6c8p-7,
    -0x1.dff12ep-7
  },
  { // Entry 154
    0x1.d9d7000000930fc88ef47c6e1ada9a2ep-3,
    0x1.e293c6p-3
  },
  { // Entry 155
    -0x1.d9d7000000930fc88ef47c6e1ada9a2ep-3,
    -0x1.e293c6p-3
  },
  { // Entry 156
    0x1.e3871b0406361c77ff3262df09e8c737p-11,
    0x1.e38724p-11
  },
  { // Entry 157
    -0x1.e3871b0406361c77ff3262df09e8c737p-11,
    -0x1.e38724p-11
  },
  { // Entry 158
    0x1.e7f6c1d323d9985457500cf721006947p-7,
    0x1.e7fffep-7
  },
  { // Entry 159
    -0x1.e7f6c1d323d9985457500cf721006947p-7,
    -0x1.e7fffep-7
  },
  { // Entry 160
    0x1.7c54ecfffa1859038ca17969c97fe5dep-1,
    0x1.e9de92p-1
  },
  { // Entry 161
    -0x1.7c54ecfffa1859038ca17969c97fe5dep-1,
    -0x1.e9de92p-1
  },
  { // Entry 162
    0x1.f34e86fff858026738196409a11a9d77p-4,
    0x1.f5cd60p-4
  },
  { // Entry 163
    -0x1.f34e86fff858026738196409a11a9d77p-4,
    -0x1.f5cd60p-4
  },
  { // Entry 164
    0x1.f650cd01dd1dd3b74d4170b58162d604p-7,
    0x1.f65ae0p-7
  },
  { // Entry 165
    -0x1.f650cd01dd1dd3b74d4170b58162d604p-7,
    -0x1.f65ae0p-7
  },
  { // Entry 166
    0x1.f7f5d19736651657bb4a908bb6ce6f3cp-7,
    0x1.f7fffep-7
  },
  { // Entry 167
    -0x1.f7f5d19736651657bb4a908bb6ce6f3cp-7,
    -0x1.f7fffep-7
  },
  { // Entry 168
    0x1.f936b301e75e5596ad5b43deb4061c2cp-10,
    0x1.f936dcp-10
  },
  { // Entry 169
    -0x1.f936b301e75e5596ad5b43deb4061c2cp-10,
    -0x1.f936dcp-10
  },
  { // Entry 170
    0x1.fb5c247b97361255c41f765d4cbab28ep-10,
    0x1.fb5c4ep-10
  },
  { // Entry 171
    -0x1.fb5c247b97361255c41f765d4cbab28ep-10,
    -0x1.fb5c4ep-10
  },
  { // Entry 172
    0x1.fb60ee7a6a3af79f5ce57fd612d65a22p-10,
    0x1.fb6118p-10
  },
  { // Entry 173
    -0x1.fb60ee7a6a3af79f5ce57fd612d65a22p-10,
    -0x1.fb6118p-10
  },
  { // Entry 174
    0x1.fb61507a522ba9573e82098c0792ba25p-10,
    0x1.fb617ap-10
  },
  { // Entry 175
    -0x1.fb61507a522ba9573e82098c0792ba25p-10,
    -0x1.fb617ap-10
  },
  { // Entry 176
    0x1.fb896701df3450ee4bc970f28fb75445p-8,
    0x1.fb8cp-8
  },
  { // Entry 177
    -0x1.fb896701df3450ee4bc970f28fb75445p-8,
    -0x1.fb8cp-8
  },
  { // Entry 178
    0x1.fc3c1bffffc9f2bffbd43471548806fcp-4,
    0x1.fede10p-4
  },
  { // Entry 179
    -0x1.fc3c1bffffc9f2bffbd43471548806fcp-4,
    -0x1.fede10p-4
  },
  { // Entry 180
    0x1.fed5976f11593f5d7e4b3836da802317p-6,
    0x1.fefffep-6
  },
  { // Entry 181
    -0x1.fed5976f11593f5d7e4b3836da802317p-6,
    -0x1.fefffep-6
  },
  { // Entry 182
    0x1.ff1d56d81077af007425dda32d1f0031p-8,
    0x1.ff1ffep-8
  },
  { // Entry 183
    -0x1.ff1d56d81077af007425dda32d1f0031p-8,
    -0x1.ff1ffep-8
  },
  { // Entry 184
    0x1.fff1d3d1b5268bffb0a21ac78411370bp-7,
    0x1.fffc7ep-7
  },
  { // Entry 185
    -0x1.fff1d3d1b5268bffb0a21ac78411370bp-7,
    -0x1.fffc7ep-7
  },
  { // Entry 186
    0x1.fd580cdbebec6f60e3365d17ed60414ap-4,
    0x1.fffe74p-4
  },
  { // Entry 187
    -0x1.fd580cdbebec6f60e3365d17ed60414ap-4,
    -0x1.fffe74p-4
  },
  { // Entry 188
    0x1.f596b7f7b4c13c155c3efd93f5dfe6d2p-3,
    0x1.fffebap-3
  },
  { // Entry 189
    -0x1.f596b7f7b4c13c155c3efd93f5dfe6d2p-3,
    -0x1.fffebap-3
  },
  { // Entry 190
    0x1.f597340cb7cde9ab8349aed17bf234d0p-3,
    0x1.ffff3ep-3
  },
  { // Entry 191
    -0x1.f597340cb7cde9ab8349aed17bf234d0p-3,
    -0x1.ffff3ep-3
  },
  { // Entry 192
    0x1.ff5517ffac72f276baf86702efd7f0e4p-5,
    0x1.ffff7ep-5
  },
  { // Entry 193
    -0x1.ff5517ffac72f276baf86702efd7f0e4p-5,
    -0x1.ffff7ep-5
  },
  { // Entry 194
    0x1.85ef82ffff600101847e4b0f9e445356p-1,
    0x1.ffffa0p-1
  },
  { // Entry 195
    -0x1.85ef82ffff600101847e4b0f9e445356p-1,
    -0x1.ffffa0p-1
  },
  { // Entry 196
    0x1.d9352125813bde3aa9ea505b2809fca3p-2,
    0x1.ffffdcp-2
  },
  { // Entry 197
    -0x1.d9352125813bde3aa9ea505b2809fca3p-2,
    -0x1.ffffdcp-2
  },
  { // Entry 198
    0x1.ffa817006391aeb5707dac7212cf73d5p-1,
    0x1.fffffap1
  },
  { // Entry 199
    -0x1.ffa817006391aeb5707dac7212cf73d5p-1,
    -0x1.fffffap1
  },
  { // Entry 200
    0x1.a86c170000be8e7f4d04a063da24860ep-3,
    0x1.aea8e2p-3
  },
  { // Entry 201
    -0x1.a86c170000be8e7f4d04a063da24860ep-3,
    -0x1.aea8e2p-3
  },
  { // Entry 202
    0.0,
    0.0
  },
  { // Entry 203
    0x1.1a5eefe0da6da46ea6f171425810e4dfp-5,
    0x1.1a7b96p-5
  },
  { // Entry 204
    -0x1.1a5eefe0da6da46ea6f171425810e4dfp-5,
    -0x1.1a7b96p-5
  },
  { // Entry 205
    0x1.1a09274ac6f7b6d4b727690be74eb0ddp-4,
    0x1.1a7b96p-4
  },
  { // Entry 206
    -0x1.1a09274ac6f7b6d4b727690be74eb0ddp-4,
    -0x1.1a7b96p-4
  },
  { // Entry 207
    0x1.a6381479e10e322568c693ec4f279989p-4,
    0x1.a7b960p-4
  },
  { // Entry 208
    -0x1.a6381479e10e322568c693ec4f279989p-4,
    -0x1.a7b960p-4
  },
  { // Entry 209
    0x1.18b472d6fbe7dee4499ec16056fea055p-3,
    0x1.1a7b96p-3
  },
  { // Entry 210
    -0x1.18b472d6fbe7dee4499ec16056fea055p-3,
    -0x1.1a7b96p-3
  },
  { // Entry 211
    0x1.5da54dc77f05440705ed9d5be3f9e9d8p-3,
    0x1.611a7cp-3
  },
  { // Entry 212
    -0x1.5da54dc77f05440705ed9d5be3f9e9d8p-3,
    -0x1.611a7cp-3
  },
  { // Entry 213
    0x1.a1c7a7ee254eb0b79612d120102ce9cdp-3,
    0x1.a7b962p-3
  },
  { // Entry 214
    -0x1.a1c7a7ee254eb0b79612d120102ce9cdp-3,
    -0x1.a7b962p-3
  },
  { // Entry 215
    0x1.e4f66de638560e22d3726a77da8f160fp-3,
    0x1.ee5848p-3
  },
  { // Entry 216
    -0x1.e4f66de638560e22d3726a77da8f160fp-3,
    -0x1.ee5848p-3
  },
  { // Entry 217
    0x1.13875aa36e2d8920626dbd412fff0a19p-2,
    0x1.1a7b96p-2
  },
  { // Entry 218
    -0x1.13875aa36e2d8920626dbd412fff0a19p-2,
    -0x1.1a7b96p-2
  },
  { // Entry 219
    0x1.33f8019585f3cc502e91eecf50e70e1ep-2,
    0x1.3dcb08p-2
  },
  { // Entry 220
    -0x1.33f8019585f3cc502e91eecf50e70e1ep-2,
    -0x1.3dcb08p-2
  },
  { // Entry 221
    0x1.53be3e2cd98b021967b9bd31e58b5176p-2,
    0x1.611a7ap-2
  },
  { // Entry 222
    -0x1.53be3e2cd98b021967b9bd31e58b5176p-2,
    -0x1.611a7ap-2
  },
  { // Entry 223
    0x1.72cc88c146572445dafcbe755d41bcabp-2,
    0x1.8469ecp-2
  },
  { // Entry 224
    -0x1.72cc88c146572445dafcbe755d41bcabp-2,
    -0x1.8469ecp-2
  },
  { // Entry 225
    0x1.9116ceec77a9f298abf6b13437746b97p-2,
    0x1.a7b95ep-2
  },
  { // Entry 226
    -0x1.9116ceec77a9f298abf6b13437746b97p-2,
    -0x1.a7b95ep-2
  },
  { // Entry 227
    0x1.ae927d0b74198b988ccf700a98369717p-2,
    0x1.cb08d0p-2
  },
  { // Entry 228
    -0x1.ae927d0b74198b988ccf700a98369717p-2,
    -0x1.cb08d0p-2
  },
  { // Entry 229
    0x1.cb3682279dc978565aefc3dbd264dc6bp-2,
    0x1.ee5842p-2
  },
  { // Entry 230
    -0x1.cb3682279dc978565aefc3dbd264dc6bp-2,
    -0x1.ee5842p-2
  },
  { // Entry 231
    0x1.e6fb4e9962e192a0d5bde52d580d91p-2,
    0x1.08d3dap-1
  },
  { // Entry 232
    -0x1.e6fb4e9962e192a0d5bde52d580d91p-2,
    -0x1.08d3dap-1
  },
  { // Entry 233
    0x1.00ed67a7ca644147f7f84f1f3f68eb4ep-1,
    0x1.1a7b94p-1
  },
  { // Entry 234
    -0x1.00ed67a7ca644147f7f84f1f3f68eb4ep-1,
    -0x1.1a7b94p-1
  },
  { // Entry 235
    0x1.0de82f529333a223ff587747d148653cp-1,
    0x1.2c234ep-1
  },
  { // Entry 236
    -0x1.0de82f529333a223ff587747d148653cp-1,
    -0x1.2c234ep-1
  },
  { // Entry 237
    0x1.1a6c5d5a29120e952b1038e517a86a7ap-1,
    0x1.3dcb08p-1
  },
  { // Entry 238
    -0x1.1a6c5d5a29120e952b1038e517a86a7ap-1,
    -0x1.3dcb08p-1
  },
  { // Entry 239
    0x1.2678f914054d8f29392b6e43daa2fe82p-1,
    0x1.4f72c2p-1
  },
  { // Entry 240
    -0x1.2678f914054d8f29392b6e43daa2fe82p-1,
    -0x1.4f72c2p-1
  },
  { // Entry 241
    0x1.320da804a66b5aea0e2572fe3978eac8p-1,
    0x1.611a7cp-1
  },
  { // Entry 242
    -0x1.320da804a66b5aea0e2572fe3978eac8p-1,
    -0x1.611a7cp-1
  },
  { // Entry 243
    0x1.3d2aa2c374c14e0b68b69e1f2aad8daep-1,
    0x1.72c236p-1
  },
  { // Entry 244
    -0x1.3d2aa2c374c14e0b68b69e1f2aad8daep-1,
    -0x1.72c236p-1
  },
  { // Entry 245
    0x1.47d0a924d14e34db0ec4761b0df7646dp-1,
    0x1.8469f0p-1
  },
  { // Entry 246
    -0x1.47d0a924d14e34db0ec4761b0df7646dp-1,
    -0x1.8469f0p-1
  },
  { // Entry 247
    0x1.5200f5eeb74275fa79657708d5b078b6p-1,
    0x1.9611aap-1
  },
  { // Entry 248
    -0x1.5200f5eeb74275fa79657708d5b078b6p-1,
    -0x1.9611aap-1
  },
  { // Entry 249
    0x1.5bbd32569013f13f86d8a0f1ef604c1dp-1,
    0x1.a7b964p-1
  },
  { // Entry 250
    -0x1.5bbd32569013f13f86d8a0f1ef604c1dp-1,
    -0x1.a7b964p-1
  },
  { // Entry 251
    0x1.650769803034e7d79ffaa44b0cc39437p-1,
    0x1.b9611ep-1
  },
  { // Entry 252
    -0x1.650769803034e7d79ffaa44b0cc39437p-1,
    -0x1.b9611ep-1
  },
  { // Entry 253
    0x1.6de1fc2ec1c6722f9de045bb1a94b919p-1,
    0x1.cb08d8p-1
  },
  { // Entry 254
    -0x1.6de1fc2ec1c6722f9de045bb1a94b919p-1,
    -0x1.cb08d8p-1
  },
  { // Entry 255
    0x1.764f94d0fb2866129f770f308fa0ba85p-1,
    0x1.dcb092p-1
  },
  { // Entry 256
    -0x1.764f94d0fb2866129f770f308fa0ba85p-1,
    -0x1.dcb092p-1
  },
  { // Entry 257
    0x1.7e531c0aa594c275df30c5b8451bba53p-1,
    0x1.ee584cp-1
  },
  { // Entry 258
    -0x1.7e531c0aa594c275df30c5b8451bba53p-1,
    -0x1.ee584cp-1
  },
  { // Entry 259
    0x1.85efab514f394558632be293c4274fe6p-1,
    0x1.p0
  },
  { // Entry 260
    -0x1.85efab514f394558632be293c4274fe6p-1,
    -0x1.p0
  },
  { // Entry 261
    0x1.ffffffffffffffffffffffffffffffffp-1,
    0x1.p100
  },
  { // Entry 262
    -0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.p100
  },
  { // Entry 263
    0x1.ffffffffffffffffffffffffffffffffp-1,
    0x1.0d7944p100
  },
  { // Entry 264
    -0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.0d7944p100
  },
  { // Entry 265
    0x1.ffffffffffffffffffffffffffffffffp-1,
    0x1.1af288p100
  },
  { // Entry 266
    -0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.1af288p100
  },
  { // Entry 267
    0x1.ffffffffffffffffffffffffffffffffp-1,
    0x1.286bccp100
  },
  { // Entry 268
    -0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.286bccp100
  },
  { // Entry 269
    0x1.ffffffffffffffffffffffffffffffffp-1,
    0x1.35e510p100
  },
  { // Entry 270
    -0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.35e510p100
  },
  { // Entry 271
    0x1.ffffffffffffffffffffffffffffffffp-1,
    0x1.435e54p100
  },
  { // Entry 272
    -0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.435e54p100
  },
  { // Entry 273
    0x1.ffffffffffffffffffffffffffffffffp-1,
    0x1.50d798p100
  },
  { // Entry 274
    -0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.50d798p100
  },
  { // Entry 275
    0x1.ffffffffffffffffffffffffffffffffp-1,
    0x1.5e50dcp100
  },
  { // Entry 276
    -0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.5e50dcp100
  },
  { // Entry 277
    0x1.ffffffffffffffffffffffffffffffffp-1,
    0x1.6bca20p100
  },
  { // Entry 278
    -0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.6bca20p100
  },
  { // Entry 279
    0x1.ffffffffffffffffffffffffffffffffp-1,
    0x1.794364p100
  },
  { // Entry 280
    -0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.794364p100
  },
  { // Entry 281
    0x1.ffffffffffffffffffffffffffffffffp-1,
    0x1.86bca8p100
  },
  { // Entry 282
    -0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.86bca8p100
  },
  { // Entry 283
    0x1.ffffffffffffffffffffffffffffffffp-1,
    0x1.9435ecp100
  },
  { // Entry 284
    -0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.9435ecp100
  },
  { // Entry 285
    0x1.ffffffffffffffffffffffffffffffffp-1,
    0x1.a1af30p100
  },
  { // Entry 286
    -0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.a1af30p100
  },
  { // Entry 287
    0x1.ffffffffffffffffffffffffffffffffp-1,
    0x1.af2874p100
  },
  { // Entry 288
    -0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.af2874p100
  },
  { // Entry 289
    0x1.ffffffffffffffffffffffffffffffffp-1,
    0x1.bca1b8p100
  },
  { // Entry 290
    -0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.bca1b8p100
  },
  { // Entry 291
    0x1.ffffffffffffffffffffffffffffffffp-1,
    0x1.ca1afcp100
  },
  { // Entry 292
    -0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.ca1afcp100
  },
  { // Entry 293
    0x1.ffffffffffffffffffffffffffffffffp-1,
    0x1.d79440p100
  },
  { // Entry 294
    -0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.d79440p100
  },
  { // Entry 295
    0x1.ffffffffffffffffffffffffffffffffp-1,
    0x1.e50d84p100
  },
  { // Entry 296
    -0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.e50d84p100
  },
  { // Entry 297
    0x1.ffffffffffffffffffffffffffffffffp-1,
    0x1.f286c8p100
  },
  { // Entry 298
    -0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.f286c8p100
  },
  { // Entry 299
    0x1.ffffffffffffffffffffffffffffffffp-1,
    0x1.p101
  },
  { // Entry 300
    -0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.p101
  },
  { // Entry 301
    -0.0f,
    -0x1.p-149
  },
  { // Entry 302
    0.0f,
    0x1.p-149
  },
  { // Entry 303
    0.0,
    0.0
  },
  { // Entry 304
    0.0f,
    0x1.p-149
  },
  { // Entry 305
    -0.0f,
    -0x1.p-149
  },
  { // Entry 306
    0x1.d9353be2bf67df131f7df0e337af4ca9p-2,
    0x1.fffffep-2
  },
  { // Entry 307
    -0x1.d9353be2bf67df131f7df0e337af4ca9p-2,
    -0x1.fffffep-2
  },
  { // Entry 308
    0x1.d9353d7568af365128ee21c65b08d3a7p-2,
    0x1.p-1
  },
  { // Entry 309
    -0x1.d9353d7568af365128ee21c65b08d3a7p-2,
    -0x1.p-1
  },
  { // Entry 310
    0x1.d935409abb3bb6925a21ec1ab4945211p-2,
    0x1.000002p-1
  },
  { // Entry 311
    -0x1.d935409abb3bb6925a21ec1ab4945211p-2,
    -0x1.000002p-1
  },
  { // Entry 312
    0x1.fffffa7fc7b1f6bc2e2cdde1540f0470p-2,
    0x1.193ea4p-1
  },
  { // Entry 313
    -0x1.fffffa7fc7b1f6bc2e2cdde1540f0470p-2,
    -0x1.193ea4p-1
  },
  { // Entry 314
    0x1.fffffd7fc7b5f6f475c594f45adc9be5p-2,
    0x1.193ea6p-1
  },
  { // Entry 315
    -0x1.fffffd7fc7b5f6f475c594f45adc9be5p-2,
    -0x1.193ea6p-1
  },
  { // Entry 316
    0x1.0000003fe3db7b965f4f34192b818b64p-1,
    0x1.193ea8p-1
  },
  { // Entry 317
    -0x1.0000003fe3db7b965f4f34192b818b64p-1,
    -0x1.193ea8p-1
  },
  { // Entry 318
    0x1.fffffffffffffffa422e1905e1f508eep-1,
    0x1.5ffffep4
  },
  { // Entry 319
    -0x1.fffffffffffffffa422e1905e1f508eep-1,
    -0x1.5ffffep4
  },
  { // Entry 320
    0x1.fffffffffffffffa422f887a2dc5050cp-1,
    0x1.60p4
  },
  { // Entry 321
    -0x1.fffffffffffffffa422f887a2dc5050cp-1,
    -0x1.60p4
  },
  { // Entry 322
    0x1.fffffffffffffffa4230f7ee1db7f9b3p-1,
    0x1.600002p4
  },
  { // Entry 323
    -0x1.fffffffffffffffa4230f7ee1db7f9b3p-1,
    -0x1.600002p4
  },
  { // Entry 324
    -0x1.3333333dabc33b19ad2c008a3f7d4144p-1,
    -0x1.62e430p-1
  },
  { // Entry 325
    0x1.3333333dabc33b19ad2c008a3f7d4144p-1,
    0x1.62e430p-1
  },
  { // Entry 326
    -0x1.333331f5fdae082d6c69302af70f1ab2p-1,
    -0x1.62e42ep-1
  },
  { // Entry 327
    0x1.333331f5fdae082d6c69302af70f1ab2p-1,
    0x1.62e42ep-1
  },
  { // Entry 328
    -0x1.333330ae4f974c09dfacf6fd31a6a22ap-1,
    -0x1.62e42cp-1
  },
  { // Entry 329
    0x1.333330ae4f974c09dfacf6fd31a6a22ap-1,
    0x1.62e42cp-1
  },
  { // Entry 330
    -0x1.55555563e05644101a754f1b989bf5e0p-2,
    -0x1.62e430p-2
  },
  { // Entry 331
    0x1.55555563e05644101a754f1b989bf5e0p-2,
    0x1.62e430p-2
  },
  { // Entry 332
    -0x1.5555539cc3e435f2961e38240bc73aa4p-2,
    -0x1.62e42ep-2
  },
  { // Entry 333
    0x1.5555539cc3e435f2961e38240bc73aa4p-2,
    0x1.62e42ep-2
  },
  { // Entry 334
    -0x1.555551d5a7719020ec6cf2d7658d0ac8p-2,
    -0x1.62e42cp-2
  },
  { // Entry 335
    0x1.555551d5a7719020ec6cf2d7658d0ac8p-2,
    0x1.62e42cp-2
  },
  { // Entry 336
    -0x1.5f619990a5492052ffe57497a9abd298p-3,
    -0x1.62e430p-3
  },
  { // Entry 337
    0x1.5f619990a5492052ffe57497a9abd298p-3,
    0x1.62e430p-3
  },
  { // Entry 338
    -0x1.5f61979fb7af4d856def98ede8520596p-3,
    -0x1.62e42ep-3
  },
  { // Entry 339
    0x1.5f61979fb7af4d856def98ede8520596p-3,
    0x1.62e42ep-3
  },
  { // Entry 340
    -0x1.5f6195aeca155016a893d14088fd4ba5p-3,
    -0x1.62e42cp-3
  },
  { // Entry 341
    0x1.5f6195aeca155016a893d14088fd4ba5p-3,
    0x1.62e42cp-3
  },
  { // Entry 342
    -0x1.620185f59f2beaefe5f1ff532c6185e9p-4,
    -0x1.62e430p-4
  },
  { // Entry 343
    0x1.620185f59f2beaefe5f1ff532c6185e9p-4,
    0x1.62e430p-4
  },
  { // Entry 344
    -0x1.620183f9723c526db01581b03289e0f8p-4,
    -0x1.62e42ep-4
  },
  { // Entry 345
    0x1.620183f9723c526db01581b03289e0f8p-4,
    0x1.62e42ep-4
  },
  { // Entry 346
    -0x1.620181fd454caef095a305bc4bfa5f8cp-4,
    -0x1.62e42cp-4
  },
  { // Entry 347
    0x1.620181fd454caef095a305bc4bfa5f8cp-4,
    0x1.62e42cp-4
  },
  { // Entry 348
    -0x1.62ab64d86ab1c41d432598f1a7016c16p-5,
    -0x1.62e430p-5
  },
  { // Entry 349
    0x1.62ab64d86ab1c41d432598f1a7016c16p-5,
    0x1.62e430p-5
  },
  { // Entry 350
    -0x1.62ab62d96060fd8e75e7cb16c3cd0f7bp-5,
    -0x1.62e42ep-5
  },
  { // Entry 351
    0x1.62ab62d96060fd8e75e7cb16c3cd0f7bp-5,
    0x1.62e42ep-5
  },
  { // Entry 352
    -0x1.62ab60da5610343ba64510e844c87ed4p-5,
    -0x1.62e42cp-5
  },
  { // Entry 353
    0x1.62ab60da5610343ba64510e844c87ed4p-5,
    0x1.62e42cp-5
  },
  { // Entry 354
    -0x1.62d5fb2a4e0746b264bbd3e451c465d4p-6,
    -0x1.62e430p-6
  },
  { // Entry 355
    0x1.62d5fb2a4e0746b264bbd3e451c465d4p-6,
    0x1.62e430p-6
  },
  { // Entry 356
    -0x1.62d5f92a8b81d6095737ce1a949ac4bep-6,
    -0x1.62e42ep-6
  },
  { // Entry 357
    0x1.62d5f92a8b81d6095737ce1a949ac4bep-6,
    0x1.62e42ep-6
  },
  { // Entry 358
    -0x1.62d5f72ac8fc64aef404ea2dd7db57a6p-6,
    -0x1.62e42cp-6
  },
  { // Entry 359
    0x1.62d5f72ac8fc64aef404ea2dd7db57a6p-6,
    0x1.62e42cp-6
  },
  { // Entry 360
    0x1.62d5f72ac8fc64aef404ea2dd7db57a6p-6,
    0x1.62e42cp-6
  },
  { // Entry 361
    -0x1.62d5f72ac8fc64aef404ea2dd7db57a6p-6,
    -0x1.62e42cp-6
  },
  { // Entry 362
    0x1.62d5f92a8b81d6095737ce1a949ac4bep-6,
    0x1.62e42ep-6
  },
  { // Entry 363
    -0x1.62d5f92a8b81d6095737ce1a949ac4bep-6,
    -0x1.62e42ep-6
  },
  { // Entry 364
    0x1.62d5fb2a4e0746b264bbd3e451c465d4p-6,
    0x1.62e430p-6
  },
  { // Entry 365
    -0x1.62d5fb2a4e0746b264bbd3e451c465d4p-6,
    -0x1.62e430p-6
  },
  { // Entry 366
    0x1.62ab60da5610343ba64510e844c87ed4p-5,
    0x1.62e42cp-5
  },
  { // Entry 367
    -0x1.62ab60da5610343ba64510e844c87ed4p-5,
    -0x1.62e42cp-5
  },
  { // Entry 368
    0x1.62ab62d96060fd8e75e7cb16c3cd0f7bp-5,
    0x1.62e42ep-5
  },
  { // Entry 369
    -0x1.62ab62d96060fd8e75e7cb16c3cd0f7bp-5,
    -0x1.62e42ep-5
  },
  { // Entry 370
    0x1.62ab64d86ab1c41d432598f1a7016c16p-5,
    0x1.62e430p-5
  },
  { // Entry 371
    -0x1.62ab64d86ab1c41d432598f1a7016c16p-5,
    -0x1.62e430p-5
  },
  { // Entry 372
    0x1.620181fd454caef095a305bc4bfa5f8cp-4,
    0x1.62e42cp-4
  },
  { // Entry 373
    -0x1.620181fd454caef095a305bc4bfa5f8cp-4,
    -0x1.62e42cp-4
  },
  { // Entry 374
    0x1.620183f9723c526db01581b03289e0f8p-4,
    0x1.62e42ep-4
  },
  { // Entry 375
    -0x1.620183f9723c526db01581b03289e0f8p-4,
    -0x1.62e42ep-4
  },
  { // Entry 376
    0x1.620185f59f2beaefe5f1ff532c6185e9p-4,
    0x1.62e430p-4
  },
  { // Entry 377
    -0x1.620185f59f2beaefe5f1ff532c6185e9p-4,
    -0x1.62e430p-4
  },
  { // Entry 378
    0x1.5f6195aeca155016a893d14088fd4ba5p-3,
    0x1.62e42cp-3
  },
  { // Entry 379
    -0x1.5f6195aeca155016a893d14088fd4ba5p-3,
    -0x1.62e42cp-3
  },
  { // Entry 380
    0x1.5f61979fb7af4d856def98ede8520596p-3,
    0x1.62e42ep-3
  },
  { // Entry 381
    -0x1.5f61979fb7af4d856def98ede8520596p-3,
    -0x1.62e42ep-3
  },
  { // Entry 382
    0x1.5f619990a5492052ffe57497a9abd298p-3,
    0x1.62e430p-3
  },
  { // Entry 383
    -0x1.5f619990a5492052ffe57497a9abd298p-3,
    -0x1.62e430p-3
  },
  { // Entry 384
    0x1.555551d5a7719020ec6cf2d7658d0ac8p-2,
    0x1.62e42cp-2
  },
  { // Entry 385
    -0x1.555551d5a7719020ec6cf2d7658d0ac8p-2,
    -0x1.62e42cp-2
  },
  { // Entry 386
    0x1.5555539cc3e435f2961e38240bc73aa4p-2,
    0x1.62e42ep-2
  },
  { // Entry 387
    -0x1.5555539cc3e435f2961e38240bc73aa4p-2,
    -0x1.62e42ep-2
  },
  { // Entry 388
    0x1.55555563e05644101a754f1b989bf5e0p-2,
    0x1.62e430p-2
  },
  { // Entry 389
    -0x1.55555563e05644101a754f1b989bf5e0p-2,
    -0x1.62e430p-2
  },
  { // Entry 390
    0x1.333330ae4f974c09dfacf6fd31a6a22ap-1,
    0x1.62e42cp-1
  },
  { // Entry 391
    -0x1.333330ae4f974c09dfacf6fd31a6a22ap-1,
    -0x1.62e42cp-1
  },
  { // Entry 392
    0x1.333331f5fdae082d6c69302af70f1ab2p-1,
    0x1.62e42ep-1
  },
  { // Entry 393
    -0x1.333331f5fdae082d6c69302af70f1ab2p-1,
    -0x1.62e42ep-1
  },
  { // Entry 394
    0x1.3333333dabc33b19ad2c008a3f7d4144p-1,
    0x1.62e430p-1
  },
  { // Entry 395
    -0x1.3333333dabc33b19ad2c008a3f7d4144p-1,
    -0x1.62e430p-1
  },
  { // Entry 396
    0x1.ffffffffffffffffffffffffffffffffp-1,
    0x1.62e42cp6
  },
  { // Entry 397
    -0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.62e42cp6
  },
  { // Entry 398
    0x1.ffffffffffffffffffffffffffffffffp-1,
    0x1.62e42ep6
  },
  { // Entry 399
    -0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.62e42ep6
  },
  { // Entry 400
    0x1.ffffffffffffffffffffffffffffffffp-1,
    0x1.62e430p6
  },
  { // Entry 401
    -0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.62e430p6
  },
  { // Entry 402
    -0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.9d1da2p6
  },
  { // Entry 403
    0x1.ffffffffffffffffffffffffffffffffp-1,
    0x1.9d1da2p6
  },
  { // Entry 404
    -0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.9d1da0p6
  },
  { // Entry 405
    0x1.ffffffffffffffffffffffffffffffffp-1,
    0x1.9d1da0p6
  },
  { // Entry 406
    -0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.9d1d9ep6
  },
  { // Entry 407
    0x1.ffffffffffffffffffffffffffffffffp-1,
    0x1.9d1d9ep6
  },
  { // Entry 408
    0x1.ffffffffffffffffffffffffffffffffp-1,
    0x1.65a9f6p6
  },
  { // Entry 409
    -0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.65a9f6p6
  },
  { // Entry 410
    0x1.ffffffffffffffffffffffffffffffffp-1,
    0x1.65a9f8p6
  },
  { // Entry 411
    -0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.65a9f8p6
  },
  { // Entry 412
    0x1.ffffffffffffffffffffffffffffffffp-1,
    0x1.65a9fap6
  },
  { // Entry 413
    -0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.65a9fap6
  },
  { // Entry 414
    -0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.62e430p6
  },
  { // Entry 415
    0x1.ffffffffffffffffffffffffffffffffp-1,
    0x1.62e430p6
  },
  { // Entry 416
    -0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.62e42ep6
  },
  { // Entry 417
    0x1.ffffffffffffffffffffffffffffffffp-1,
    0x1.62e42ep6
  },
  { // Entry 418
    -0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.62e42cp6
  },
  { // Entry 419
    0x1.ffffffffffffffffffffffffffffffffp-1,
    0x1.62e42cp6
  },
  { // Entry 420
    0x1.ffffffffffffffffffffffffffffffffp-1,
    0x1.fffffep62
  },
  { // Entry 421
    -0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.fffffep62
  },
  { // Entry 422
    0x1.ffffffffffffffffffffffffffffffffp-1,
    0x1.p63
  },
  { // Entry 423
    -0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.p63
  },
  { // Entry 424
    0x1.ffffffffffffffffffffffffffffffffp-1,
    0x1.000002p63
  },
  { // Entry 425
    -0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.000002p63
  },
  { // Entry 426
    0x1.ffffffffffffffffffffffffffffffffp-1,
    0x1.fffffep26
  },
  { // Entry 427
    -0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.fffffep26
  },
  { // Entry 428
    0x1.ffffffffffffffffffffffffffffffffp-1,
    0x1.p27
  },
  { // Entry 429
    -0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.p27
  },
  { // Entry 430
    0x1.ffffffffffffffffffffffffffffffffp-1,
    0x1.000002p27
  },
  { // Entry 431
    -0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.000002p27
  },
  { // Entry 432
    0x1.ffffffffffffffffffffffffffffffffp-1,
    0x1.fffffep23
  },
  { // Entry 433
    -0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.fffffep23
  },
  { // Entry 434
    0x1.ffffffffffffffffffffffffffffffffp-1,
    0x1.p24
  },
  { // Entry 435
    -0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.p24
  },
  { // Entry 436
    0x1.ffffffffffffffffffffffffffffffffp-1,
    0x1.000002p24
  },
  { // Entry 437
    -0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.000002p24
  },
  { // Entry 438
    0x1.ffffffffffffffffffffffcd2c4a64d0p-1,
    0x1.fffffep4
  },
  { // Entry 439
    -0x1.ffffffffffffffffffffffcd2c4a64d0p-1,
    -0x1.fffffep4
  },
  { // Entry 440
    0x1.ffffffffffffffffffffffcd2c5719bcp-1,
    0x1.p5
  },
  { // Entry 441
    -0x1.ffffffffffffffffffffffcd2c5719bcp-1,
    -0x1.p5
  },
  { // Entry 442
    0x1.ffffffffffffffffffffffcd2c70838ap-1,
    0x1.000002p5
  },
  { // Entry 443
    -0x1.ffffffffffffffffffffffcd2c70838ap-1,
    -0x1.000002p5
  },
  { // Entry 444
    0x1.fffffffffff1bdcbbc08e2044832bbfep-1,
    0x1.fffffep3
  },
  { // Entry 445
    -0x1.fffffffffff1bdcbbc08e2044832bbfep-1,
    -0x1.fffffep3
  },
  { // Entry 446
    0x1.fffffffffff1bdcd844f4dfec4941943p-1,
    0x1.p4
  },
  { // Entry 447
    -0x1.fffffffffff1bdcd844f4dfec4941943p-1,
    -0x1.p4
  },
  { // Entry 448
    0x1.fffffffffff1bdd114db7ad966aba40dp-1,
    0x1.000002p4
  },
  { // Entry 449
    -0x1.fffffffffff1bdd114db7ad966aba40dp-1,
    -0x1.000002p4
  },
  { // Entry 450
    0x1.fffff872a8a6b12003ef317c57617676p-1,
    0x1.fffffep2
  },
  { // Entry 451
    -0x1.fffff872a8a6b12003ef317c57617676p-1,
    -0x1.fffffep2
  },
  { // Entry 452
    0x1.fffff872a91f8690ee0e6d3ad8aee46bp-1,
    0x1.p3
  },
  { // Entry 453
    -0x1.fffff872a91f8690ee0e6d3ad8aee46bp-1,
    -0x1.p3
  },
  { // Entry 454
    0x1.fffff872aa11315c1a493c74b407aa6ep-1,
    0x1.000002p3
  },
  { // Entry 455
    -0x1.fffff872aa11315c1a493c74b407aa6ep-1,
    -0x1.000002p3
  },
  { // Entry 456
    0x1.ffa81705e1a8bbcbf5a3dcf7cb937ef6p-1,
    0x1.fffffep1
  },
  { // Entry 457
    -0x1.ffa81705e1a8bbcbf5a3dcf7cb937ef6p-1,
    -0x1.fffffep1
  },
  { // Entry 458
    0x1.ffa81708a0b4216857246c19dc60acb8p-1,
    0x1.p2
  },
  { // Entry 459
    -0x1.ffa81708a0b4216857246c19dc60acb8p-1,
    -0x1.p2
  },
  { // Entry 460
    0x1.ffa8170e1ecaaac35b6d81d682891126p-1,
    0x1.000002p2
  },
  { // Entry 461
    -0x1.ffa8170e1ecaaac35b6d81d682891126p-1,
    -0x1.000002p2
  },
  { // Entry 462
    0x1.ed950599638c18fec5bd8135b3976fafp-1,
    0x1.fffffep0
  },
  { // Entry 463
    -0x1.ed950599638c18fec5bd8135b3976fafp-1,
    -0x1.fffffep0
  },
  { // Entry 464
    0x1.ed9505e1bc3d3d33c432fc3e8255c8b5p-1,
    0x1.p1
  },
  { // Entry 465
    -0x1.ed9505e1bc3d3d33c432fc3e8255c8b5p-1,
    -0x1.p1
  },
  { // Entry 466
    0x1.ed9506726d9c40b04cf2556073e90aecp-1,
    0x1.000002p1
  },
  { // Entry 467
    -0x1.ed9506726d9c40b04cf2556073e90aecp-1,
    -0x1.000002p1
  },
  { // Entry 468
    0x1.85efaa7a485824cc9f98f88674c08b83p-1,
    0x1.fffffep-1
  },
  { // Entry 469
    -0x1.85efaa7a485824cc9f98f88674c08b83p-1,
    -0x1.fffffep-1
  },
  { // Entry 470
    0x1.85efab514f394558632be293c4274fe6p-1,
    0x1.p0
  },
  { // Entry 471
    -0x1.85efab514f394558632be293c4274fe6p-1,
    -0x1.p0
  },
  { // Entry 472
    0x1.85efacff5cf7afdba442be92190b551bp-1,
    0x1.000002p0
  },
  { // Entry 473
    -0x1.85efacff5cf7afdba442be92190b551bp-1,
    -0x1.000002p0
  },
  { // Entry 474
    0x1.d9353be2bf67df131f7df0e337af4ca9p-2,
    0x1.fffffep-2
  },
  { // Entry 475
    -0x1.d9353be2bf67df131f7df0e337af4ca9p-2,
    -0x1.fffffep-2
  },
  { // Entry 476
    0x1.d9353d7568af365128ee21c65b08d3a7p-2,
    0x1.p-1
  },
  { // Entry 477
    -0x1.d9353d7568af365128ee21c65b08d3a7p-2,
    -0x1.p-1
  },
  { // Entry 478
    0x1.d935409abb3bb6925a21ec1ab4945211p-2,
    0x1.000002p-1
  },
  { // Entry 479
    -0x1.d935409abb3bb6925a21ec1ab4945211p-2,
    -0x1.000002p-1
  },
  { // Entry 480
    0x1.f597e8885827eed9d73369feec84841dp-3,
    0x1.fffffep-3
  },
  { // Entry 481
    -0x1.f597e8885827eed9d73369feec84841dp-3,
    -0x1.fffffep-3
  },
  { // Entry 482
    0x1.f597ea69a1c85f1358d71d84729c80c8p-3,
    0x1.p-2
  },
  { // Entry 483
    -0x1.f597ea69a1c85f1358d71d84729c80c8p-3,
    -0x1.p-2
  },
  { // Entry 484
    0x1.f597ee2c35088eb5da928b278522fdc0p-3,
    0x1.000002p-2
  },
  { // Entry 485
    -0x1.f597ee2c35088eb5da928b278522fdc0p-3,
    -0x1.000002p-2
  },
  { // Entry 486
    0x1.fd5990c4365de99b093619573aed5eefp-4,
    0x1.fffffep-4
  },
  { // Entry 487
    -0x1.fd5990c4365de99b093619573aed5eefp-4,
    -0x1.fffffep-4
  },
  { // Entry 488
    0x1.fd5992bc4b834fc0af6ac8eff7d81040p-4,
    0x1.p-3
  },
  { // Entry 489
    -0x1.fd5992bc4b834fc0af6ac8eff7d81040p-4,
    -0x1.p-3
  },
  { // Entry 490
    0x1.fd5996ac75cded089eba2285d0035a24p-4,
    0x1.000002p-3
  },
  { // Entry 491
    -0x1.fd5996ac75cded089eba2285d0035a24p-4,
    -0x1.000002p-3
  },
  { // Entry 492
    0x1.ff55978001b8da0e0ab4904fa64b8d32p-5,
    0x1.fffffep-5
  },
  { // Entry 493
    -0x1.ff55978001b8da0e0ab4904fa64b8d32p-5,
    -0x1.fffffep-5
  },
  { // Entry 494
    0x1.ff55997e030d705935592a366a8a66d4p-5,
    0x1.p-4
  },
  { // Entry 495
    -0x1.ff55997e030d705935592a366a8a66d4p-5,
    -0x1.p-4
  },
  { // Entry 496
    0x1.ff559d7a05b690ff7d0e4114c0eb72c1p-5,
    0x1.000002p-4
  },
  { // Entry 497
    -0x1.ff559d7a05b690ff7d0e4114c0eb72c1p-5,
    -0x1.000002p-4
  },
  { // Entry 498
    0x1.ffd55799ab088fb326e9ba18d0997203p-6,
    0x1.fffffep-6
  },
  { // Entry 499
    -0x1.ffd55799ab088fb326e9ba18d0997203p-6,
    -0x1.fffffep-6
  },
  { // Entry 500
    0x1.ffd559992b1de28305fc17382205392ep-6,
    0x1.p-5
  },
  { // Entry 501
    -0x1.ffd559992b1de28305fc17382205392ep-6,
    -0x1.p-5
  },
  { // Entry 502
    0x1.ffd55d982b488523c3e9758124e0628bp-6,
    0x1.000002p-5
  },
  { // Entry 503
    -0x1.ffd55d982b488523c3e9758124e0628bp-6,
    -0x1.000002p-5
  },
  { // Entry 504
    0x1.fff55399b7de33c0d4da3bfbdc23a5d4p-7,
    0x1.fffffep-7
  },
  { // Entry 505
    -0x1.fff55399b7de33c0d4da3bfbdc23a5d4p-7,
    -0x1.fffffep-7
  },
  { // Entry 506
    0x1.fff5559997df892a1128575843fc0d52p-7,
    0x1.p-6
  },
  { // Entry 507
    -0x1.fff5559997df892a1128575843fc0d52p-7,
    -0x1.p-6
  },
  { // Entry 508
    0x1.fff5599957e2333c99c37490eae25a5ap-7,
    0x1.000002p-6
  },
  { // Entry 509
    -0x1.fff5599957e2333c99c37490eae25a5ap-7,
    -0x1.000002p-6
  },
  { // Entry 510
    0x1.fffffdf5555575999978428a3604016fp-15,
    0x1.fffffep-15
  },
  { // Entry 511
    -0x1.fffffdf5555575999978428a3604016fp-15,
    -0x1.fffffep-15
  },
  { // Entry 512
    0x1.fffffff555555599999997df7df7eab0p-15,
    0x1.p-14
  },
  { // Entry 513
    -0x1.fffffff555555599999997df7df7eab0p-15,
    -0x1.p-14
  },
  { // Entry 514
    0x1.000001faaaaa8acccc8e2144eeefdea1p-14,
    0x1.000002p-14
  },
  { // Entry 515
    -0x1.000001faaaaa8acccc8e2144eeefdea1p-14,
    -0x1.000002p-14
  },
  { // Entry 516
    0x1.fffffdfffffffff55555755555355599p-31,
    0x1.fffffep-31
  },
  { // Entry 517
    -0x1.fffffdfffffffff55555755555355599p-31,
    -0x1.fffffep-31
  },
  { // Entry 518
    0x1.fffffffffffffff55555555555555599p-31,
    0x1.p-30
  },
  { // Entry 519
    -0x1.fffffffffffffff55555555555555599p-31,
    -0x1.p-30
  },
  { // Entry 520
    0x1.000001fffffffffaaaaa8aaaaa6aaaccp-30,
    0x1.000002p-30
  },
  { // Entry 521
    -0x1.000001fffffffffaaaaa8aaaaa6aaaccp-30,
    -0x1.000002p-30
  },
  { // Entry 522
    0x1.fffffdfffffffffffffffffffffd5555p-56,
    0x1.fffffep-56
  },
  { // Entry 523
    -0x1.fffffdfffffffffffffffffffffd5555p-56,
    -0x1.fffffep-56
  },
  { // Entry 524
    0x1.fffffffffffffffffffffffffffd5555p-56,
    0x1.p-55
  },
  { // Entry 525
    -0x1.fffffffffffffffffffffffffffd5555p-56,
    -0x1.p-55
  },
  { // Entry 526
    0x1.000001fffffffffffffffffffffeaaaap-55,
    0x1.000002p-55
  },
  { // Entry 527
    -0x1.000001fffffffffffffffffffffeaaaap-55,
    -0x1.000002p-55
  },
  { // Entry 528
    -0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.fffffep127
  },
  { // Entry 529
    0x1.ffffffffffffffffffffffffffffffffp-1,
    0x1.fffffep127
  },
  { // Entry 530
    0x1.ffffffffffffffffffffffffffffffffp-1,
    0x1.fffffep127
  },
  { // Entry 531
    -0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.fffffep127
  },
  { // Entry 532
    0x1.p0,
    HUGE_VALF
  },
  { // Entry 533
    -0x1.p0,
    -HUGE_VALF
  },
  { // Entry 534
    0x1.ffffffffffffffffffffffffffffffffp-1,
    0x1.fffffep127
  },
  { // Entry 535
    -0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.fffffep127
  },
  { // Entry 536
    0x1.ffffffffffffffffffffffffffffffffp-1,
    0x1.fffffcp127
  },
  { // Entry 537
    -0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.fffffcp127
  },
  { // Entry 538
    0x1.fe175fa8292deb3d8c41deec7c2ee47cp-1,
    0x1.921fb6p1
  },
  { // Entry 539
    -0x1.fe175fa8292deb3d8c41deec7c2ee47cp-1,
    -0x1.921fb6p1
  },
  { // Entry 540
    0x1.d594fde9eb7012c121b429007ea7884ap-1,
    0x1.921fb6p0
  },
  { // Entry 541
    -0x1.d594fde9eb7012c121b429007ea7884ap-1,
    -0x1.921fb6p0
  },
  { // Entry 542
    0x1.85efacff5cf7afdba442be92190b551bp-1,
    0x1.000002p0
  },
  { // Entry 543
    -0x1.85efacff5cf7afdba442be92190b551bp-1,
    -0x1.000002p0
  },
  { // Entry 544
    0x1.85efab514f394558632be293c4274fe6p-1,
    0x1.p0
  },
  { // Entry 545
    -0x1.85efab514f394558632be293c4274fe6p-1,
    -0x1.p0
  },
  { // Entry 546
    0x1.85efaa7a485824cc9f98f88674c08b83p-1,
    0x1.fffffep-1
  },
  { // Entry 547
    -0x1.85efaa7a485824cc9f98f88674c08b83p-1,
    -0x1.fffffep-1
  },
  { // Entry 548
    0x1.4fc442656d206b21f6dcd108d6a88ad7p-1,
    0x1.921fb6p-1
  },
  { // Entry 549
    -0x1.4fc442656d206b21f6dcd108d6a88ad7p-1,
    -0x1.921fb6p-1
  },
  { // Entry 550
    0x1.000001ffffffffffffffffffffffffffp-126,
    0x1.000002p-126
  },
  { // Entry 551
    -0x1.000001ffffffffffffffffffffffffffp-126,
    -0x1.000002p-126
  },
  { // Entry 552
    0x1.ffffffffffffffffffffffffffffffffp-127,
    0x1.p-126
  },
  { // Entry 553
    -0x1.ffffffffffffffffffffffffffffffffp-127,
    -0x1.p-126
  },
  { // Entry 554
    0x1.fffffbffffffffffffffffffffffffffp-127,
    0x1.fffffcp-127
  },
  { // Entry 555
    -0x1.fffffbffffffffffffffffffffffffffp-127,
    -0x1.fffffcp-127
  },
  { // Entry 556
    0x1.fffff7ffffffffffffffffffffffffffp-127,
    0x1.fffff8p-127
  },
  { // Entry 557
    -0x1.fffff7ffffffffffffffffffffffffffp-127,
    -0x1.fffff8p-127
  },
  { // Entry 558
    0x1.ffffffffffffffffffffffffffffffffp-149,
    0x1.p-148
  },
  { // Entry 559
    -0x1.ffffffffffffffffffffffffffffffffp-149,
    -0x1.p-148
  },
  { // Entry 560
    0.0f,
    0x1.p-149
  },
  { // Entry 561
    -0.0f,
    -0x1.p-149
  },
  { // Entry 562
    0.0,
    0.0f
  },
  { // Entry 563
    -0.0,
    -0.0f
  }
};
