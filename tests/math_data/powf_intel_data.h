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

static data_1_2_t<float, float, float> g_powf_intel_data[] = {
  { // Entry 0
    HUGE_VALF,
    -0.0, -0x1.000002p-1
  },
  { // Entry 1
    0.0f,
    -0x1.p-5, 0x1.e0p4
  },
  { // Entry 2
    -0.0f,
    -0x1.p-30, 0x1.40p2
  },
  { // Entry 3
    0x1.p0,
    -0x1.p0, 0x1.000002p32
  },
  { // Entry 4
    0x1.p0,
    -0x1.000002p-41, 0.0
  },
  { // Entry 5
    0x1.d1a029128778fca3f9a261be1cb86be7p-121,
    -0x1.000006p0, -0x1.bc1ee2p27
  },
  { // Entry 6
    0x1.da6e3ff202da752de523f9846303c0b5p-124,
    -0x1.00000ap0, -0x1.111112p27
  },
  { // Entry 7
    0x1.eb70a2fbb8b2489b8d838eb65ed676acp-91,
    -0x1.00000ap0, -0x1.8f83e4p26
  },
  { // Entry 8
    HUGE_VALF,
    -0x1.000028p0, 0x1.20p44
  },
  { // Entry 9
    0x1.002001p-4,
    -0x1.0010p-2, 0x1.p1
  },
  { // Entry 10
    0x1.002001p-82,
    -0x1.0010p-41, 0x1.p1
  },
  { // Entry 11
    0x1.004004p-82,
    -0x1.0020p-41, 0x1.p1
  },
  { // Entry 12
    0x1.006009p-12,
    -0x1.0030p-6, 0x1.p1
  },
  { // Entry 13
    -0x1.fb859adbdb7df6974c5c9a5489e6972ap53,
    -0x1.0040p-6, -0x1.20p3
  },
  { // Entry 14
    0x1.008010p-82,
    -0x1.0040p-41, 0x1.p1
  },
  { // Entry 15
    0x1.00e031p-40,
    -0x1.0070p-20, 0x1.p1
  },
  { // Entry 16
    0x1.31e452ffffec96a3d5a882fe244f8c63p-1,
    -0x1.046ef4p0, -0x1.e0p4
  },
  { // Entry 17
    0x1.33e8f304p-36,
    -0x1.08p-6, 0x1.80p2
  },
  { // Entry 18
    0x1.d82001fe9d6bdbba98638def8d37e50bp-124,
    -0x1.1ec38cp0, -0x1.78p9
  },
  { // Entry 19
    -0x1.45f3bdeaa5f60d121c3fa751dbd758adp36,
    -0x1.bffffep-6, -0x1.c0p2
  },
  { // Entry 20
    0x1.df41ae7ef4e15e8ad45c7293ddc3fe7dp61,
    -0x1.fffffap-1, -0x1.c9b244p27
  },
  { // Entry 21
    0x1.df46f26f1f129a54922022f9b653a99fp61,
    -0x1.fffffap-1, -0x1.c9b262p27
  },
  { // Entry 22
    -0x1.00000300000900001b0000510000f3p21,
    -0x1.fffffap-22, -0x1.p0
  },
  { // Entry 23
    -0x1.00000300000900001b0000510000f3p-99,
    -0x1.fffffap98, -0x1.p0
  },
  { // Entry 24
    -0x1.78b55ef8aecb0b7c5b8865e27157d824p-2,
    -0x1.fffffcp-1, 0x1.000002p23
  },
  { // Entry 25
    -0x1.fffffep-41,
    -0x1.fffffep-41, 0x1.p0
  },
  { // Entry 26
    0x1.p71,
    0x1.p-2, -0x1.1cp5
  },
  { // Entry 27
    0x1.d580710e38463c3dd62fce98f203b471p-1,
    0x1.p-2, 0x1.0007p-4
  },
  { // Entry 28
    0x1.p-15,
    0x1.p-2, 0x1.e0p2
  },
  { // Entry 29
    0.0f,
    0x1.p-3, 0x1.8ffffep5
  },
  { // Entry 30
    0x1.p-40,
    0x1.p-5, 0x1.p3
  },
  { // Entry 31
    0x1.p40,
    0x1.p-10, -0x1.p2
  },
  { // Entry 32
    0x1.ecfff0b449d7c9a5d494c884c717f9cdp-88,
    0x1.p-144, 0x1.3586fep-1
  },
  { // Entry 33
    0x1.ecfff0b449d7c9a5d494c884c717f9cdp-106,
    0x1.p-144, 0x1.7586fep-1
  },
  { // Entry 34
    HUGE_VALF,
    0x1.p-149, -0x1.ccacccp-1
  },
  { // Entry 35
    HUGE_VALF,
    0x1.p-149, -0x1.e6e666p-1
  },
  { // Entry 36
    0x1.e8e101355bd975bfec3fb5ed3757777dp-1,
    0x1.000002p-2, 0x1.1111p-5
  },
  { // Entry 37
    0x1.ddb64347a55e452ed04d6a173ca5b56cp99,
    0x1.000002p-111, -0x1.ccccccp-1
  },
  { // Entry 38
    0x1.558e990004a8ebb3e8176275ba9f1052p52,
    0x1.000002p-112, -0x1.df3b5ap-2
  },
  { // Entry 39
    0x1.fffffd0000053ffff66000120bffddb6p92,
    0x1.000002p-124, -0x1.80p-1
  },
  { // Entry 40
    0x1.fffffffffff800000800000555554555p-1,
    0x1.000002p0, -0x1.p-23
  },
  { // Entry 41
    0x1.fffffffffff9000007000002eaaaa02ap-1,
    0x1.000002p0, -0x1.c0p-24
  },
  { // Entry 42
    0x1.ddb6530d485b7badb441a4460ca54c62p9,
    0x1.000002p11, 0x1.ccccc6p-1
  },
  { // Entry 43
    0x1.d901790cd9d1b9d8cdf616296479022fp-14,
    0x1.000002p17, -0x1.8af8b0p-1
  },
  { // Entry 44
    0x1.e6d3f90d414447b2f2467d4c214496bdp30,
    0x1.000002p42, 0x1.7904a4p-1
  },
  { // Entry 45
    0x1.000012ffffd68004fa7fb159108ec97dp0,
    0x1.00001cp0, 0x1.5b6dbap-1
  },
  { // Entry 46
    0x1.d174810e1e4527f011547dfc4dc6b48cp-3,
    0x1.00001cp3, -0x1.6ccccep-1
  },
  { // Entry 47
    0x1.d581970e8b4ccc9dbc28899bd1848e24p-1,
    0x1.00002ep-2, 0x1.ffffcep-5
  },
  { // Entry 48
    0x1.000455000312cc6e79ced653c38d7e2ap0,
    0x1.000038p-50, -0x1.fff77ep-20
  },
  { // Entry 49
    0x1.00480900a807e03f01500480090008p-81,
    0x1.0008p-9, 0x1.20p3
  },
  { // Entry 50
    0x1.ffc004ffb0045fc8029fe20149f2408ep39,
    0x1.0008p-10, -0x1.p2
  },
  { // Entry 51
    0x1.fec1bb35b5a826526101adab0695d1d3p-1,
    0x1.00e0p0, -0x1.6ccccep-1
  },
  { // Entry 52
    0x1.fadbde187acba5b3a6c4cde78e1bbb4ap45,
    0x1.01fffcp0, 0x1.p12
  },
  { // Entry 53
    0x1.ff1fb6ff79a5e0391b1d7dfb14de7de6p22,
    0x1.01fffep0, 0x1.000cb6p11
  },
  { // Entry 54
    0x1.6cbbc2fff64c73aab0033df757f3808dp11,
    0x1.0220p0, 0x1.e295f2p9
  },
  { // Entry 55
    0x1.f81f5312ba449421bd9393ad8df53aaep-1,
    0x1.04p0, -0x1.0006p0
  },
  { // Entry 56
    0x1.ffe88affffff33e933cfaad5f0ee2678p-1,
    0x1.08c7eep-16, 0x1.0f94b2p-16
  },
  { // Entry 57
    0x1.0000dcffffc6fee7e4aac09a3d9fb9a1p0,
    0x1.0ep-20, -0x1.000cdcp-20
  },
  { // Entry 58
    0x1.70ce05e629803c0ca47482392a882debp-3,
    0x1.0ep3, -0x1.9b91bap-1
  },
  { // Entry 59
    0x1.5a8926e473f6148a5a383bfa1ed0b335p-90,
    0x1.0ffffep0, -0x1.0000fep10
  },
  { // Entry 60
    0x1.8ec5b2e1606728f21cf2c90c2e4d2a9bp-93,
    0x1.0ffffep0, -0x1.07fffep10
  },
  { // Entry 61
    0x1.a47dd4ffffe25486314351413837b2fdp4,
    0x1.1624p-4, -0x1.3720c0p0
  },
  { // Entry 62
    0x1.442401p0,
    0x1.2010p0, 0x1.p1
  },
  { // Entry 63
    0x1.e7aaf2ffffbce86a4fd2cf9cb53d7e55p-1,
    0x1.253264p0, -0x1.6f826ep-2
  },
  { // Entry 64
    0x1.4d63290052d4d2d894b4635cb9b98130p-39,
    0x1.2711c8p-6, 0x1.aa804ep2
  },
  { // Entry 65
    0x1.b20168da0fc1fca6d3c1b8c23fdcaf39p49,
    0x1.2aaaaap-1, -0x1.fff1fep5
  },
  { // Entry 66
    0x1.ee26c12ebf5b649bef95484ece113007p2,
    0x1.2f7dc0p-23, -0x1.0967c0p-3
  },
  { // Entry 67
    0x1.bc90590000002ee11763c6fe2418730cp-2,
    0x1.334478p-2, 0x1.62e42ep-1
  },
  { // Entry 68
    0x1.8c8c8300308cee7c1a41b09294323cfbp-82,
    0x1.3ffffep-1, 0x1.e0p6
  },
  { // Entry 69
    0x1.643d4efffe606e056e4035b0becc20fdp-5,
    0x1.3ffffep-40, 0x1.d2f190p-4
  },
  { // Entry 70
    0x1.f8148914d4ea3af94ee724572f2ee8ffp-1,
    0x1.41d420p20, -0x1.22p-10
  },
  { // Entry 71
    0x1.cd6e9100038c93a7dce72a113ca56c70p-2,
    0x1.443a42p-2, 0x1.62e42ep-1
  },
  { // Entry 72
    0x1.f895910f392f3b8fcc641ae87164d684p-1,
    0x1.45a2a8p1, -0x1.p-6
  },
  { // Entry 73
    0x1.e3dff8fff6e9efd4f167a7b91eb882afp-1,
    0x1.45d174p-1, 0x1.00001cp-3
  },
  { // Entry 74
    0x1.d332f0fffc83128a7fc7bd56be27e755p-2,
    0x1.4a1704p-2, 0x1.62e42ep-1
  },
  { // Entry 75
    0x1.d5ae790003cb17cf83deb5e2cc0ea01cp-2,
    0x1.4c9f94p-2, 0x1.62e42ep-1
  },
  { // Entry 76
    0x1.ca8ec6ed5df39f991f808d94dd5c8834p88,
    0x1.4e9cc2p-30, -0x1.80p1
  },
  { // Entry 77
    0x1.d9b648fffc167ed8b917b64f747e6270p-2,
    0x1.50bfc8p-2, 0x1.62e42ep-1
  },
  { // Entry 78
    0x1.da95070001a3799fee02ea034357a8c2p-2,
    0x1.51a450p-2, 0x1.62e42ep-1
  },
  { // Entry 79
    0x1.fe957b38c5b6959bb0ea80e43c709ecap-1,
    0x1.679286p-11, 0x1.8ea824p-12
  },
  { // Entry 80
    0x1.aca91b5f3882f36dcdab2a8d641c0ab5p-56,
    0x1.745d18p-4, 0x1.ff1ffep3
  },
  { // Entry 81
    0x1.f82eb711ff0066ee591658258b692331p-1,
    0x1.77fffep-120, 0x1.85bc7ap-13
  },
  { // Entry 82
    0x1.d98c8300003e83fd25b95381f702161cp-1,
    0x1.7a3d0ep0, -0x1.99999ap-3
  },
  { // Entry 83
    0x1.f82cef13a11a5f5a0562fe52c88207bdp-1,
    0x1.7c9a16p-2, 0x1.fddffep-7
  },
  { // Entry 84
    0x1.d0d014fffe715e2732b6d1ced96adb76p19,
    0x1.7e9bb0p-8, -0x1.569828p1
  },
  { // Entry 85
    0x1.8518e2fffdea301062ac6a29c6e53df6p-1,
    0x1.851ebap-1, 0x1.000ep0
  },
  { // Entry 86
    0x1.2da1e8fffe1350e4daf5553e75dca020p-4,
    0x1.861862p-4, 0x1.1cp0
  },
  { // Entry 87
    0x1.dd037a2c561bfe1824p-11,
    0x1.8ce632p-4, 0x1.80p1
  },
  { // Entry 88
    0x1.f81fa713d2b23eac52a36f4b3a33023ep-1,
    0x1.8f86aap-1, 0x1.0000e0p-4
  },
  { // Entry 89
    0x1.03f14b095ae687525a7e377e3505e587p0,
    0x1.90p5, 0x1.0008p-8
  },
  { // Entry 90
    0x1.36395100005cc4113b220d6ce672e165p0,
    0x1.95578ep1, 0x1.555556p-3
  },
  { // Entry 91
    0x1.f83249134e77a21bf811350c6a931beep-1,
    0x1.98p5, -0x1.000002p-8
  },
  { // Entry 92
    0x1.9f628b3cfd06f417f86e1ca8edc1469ep117,
    0x1.99999cp3, 0x1.p5
  },
  { // Entry 93
    0x1.d4851ccedafdd1cbc79a6a6b3dbb1cbep119,
    0x1.9a66d0p-14, -0x1.20p3
  },
  { // Entry 94
    0x1.2536270001fab70a29d68e60feb11211p-11,
    0x1.9ffffep41, -0x1.094f1cp-2
  },
  { // Entry 95
    0x1.d08ae8fffdc7029e0bd02c871606a01cp0,
    0x1.a57becp1, 0x1.p-1
  },
  { // Entry 96
    0x1.b83638ffb21561a23ec9b8a7b0ba7b52p15,
    0x1.aaaaaep-1, -0x1.e0p5
  },
  { // Entry 97
    0x1.c198860000001c1cee146e451365eae1p-10,
    0x1.ad1d1cp-14, 0x1.62e42ep-1
  },
  { // Entry 98
    0x1.cd0c6eefc33dfc2ef3d2beb81ad568cap30,
    0x1.b13b1cp-1, -0x1.ffff1ep6
  },
  { // Entry 99
    0x1.6228e4fef882769ba040164fc4bca0cbp-81,
    0x1.b7ffd8p-1, 0x1.705394p8
  },
  { // Entry 100
    0x1.ae9d756c84b4063f238dd151bec30e0bp-99,
    0x1.be0d7cp-1, 0x1.ede448p8
  },
  { // Entry 101
    0x1.913f68f101ebe490d29d873d1e0fd828p-85,
    0x1.be0f70p-1, 0x1.a8147ap8
  },
  { // Entry 102
    0x1.b1e7215c128082aab49edab1641919a8p-99,
    0x1.be0f70p-1, 0x1.ede5d8p8
  },
  { // Entry 103
    0x1.dc574183f03d7a333c18fc6916daa859p-96,
    0x1.bff0d0p-1, 0x1.ed2fb4p8
  },
  { // Entry 104
    0x1.da6fcd00020da659e4a50ba993a71d92p6,
    0x1.bffffep1, 0x1.e7f782p1
  },
  { // Entry 105
    HUGE_VALF,
    0x1.c25c26p-44, -0x1.40p3
  },
  { // Entry 106
    0x1.951dfaf0d0341097e50f8d51fb5b0b2ap-72,
    0x1.c4ec74p-1, 0x1.935234p8
  },
  { // Entry 107
    0x1.fb1c6cfffef199884e78c26fef057fc3p4,
    0x1.c76380p0, 0x1.80087cp2
  },
  { // Entry 108
    0x1.d82d7dffffefc26e1f09e6d73a276d81p-10,
    0x1.cc8d06p-14, 0x1.62e42ep-1
  },
  { // Entry 109
    0x1.997e0eef7bb3d4eda40dc43c72a53167p-70,
    0x1.d1cdccp-1, 0x1.fc2640p8
  },
  { // Entry 110
    0x1.fe82dd381a8d3056a4c554f1e1764f9dp-1,
    0x1.d40a66p-3, 0x1.02964cp-9
  },
  { // Entry 111
    0x1.f3ab1937169c9ab7aac67b94894ede10p-1,
    0x1.d55552p-2, 0x1.000038p-5
  },
  { // Entry 112
    0x1.fe805f37b89bc0c8b0163db7c11f48f2p-1,
    0x1.da12f0p-1, 0x1.38p-5
  },
  { // Entry 113
    0x1.c27937000f6c15a86f8eb042a0895566p-73,
    0x1.df0a82p-1, 0x1.77fbc0p9
  },
  { // Entry 114
    0x1.c64b0d000027dbefa1e3233ef53619b7p105,
    0x1.dffffep52, 0x1.00087cp1
  },
  { // Entry 115
    0x1.0847b080e10a3f33ba599a218b630ffbp0,
    0x1.e06b8cp-1, -0x1.p-1
  },
  { // Entry 116
    0x1.eff5716fa057c0db02972e5b51a95899p-1,
    0x1.e06b8cp-1, 0x1.p-1
  },
  { // Entry 117
    0x1.083f7f587cdb6cc005ee70abb128067cp0,
    0x1.e08956p-1, -0x1.p-1
  },
  { // Entry 118
    0x1.f004d186653df746f46ac1da51e68817p-1,
    0x1.e08956p-1, 0x1.p-1
  },
  { // Entry 119
    0x1.2e78986ce71690689a17b09e2fd01256p0,
    0x1.e0ee8ap-1, -0x1.5515p1
  },
  { // Entry 120
    0x1.34e306ebdacb3fb249efb92c5df50a30p0,
    0x1.e0ee8ap-1, -0x1.80p1
  },
  { // Entry 121
    0x1.1a22a2b284843cef729aa7923200616bp0,
    0x1.e0ee8ap-1, -0x1.8d89d8p0
  },
  { // Entry 122
    0x1.1d8befed03f9ed8d628e72acfd5846f2p0,
    0x1.e0ee8ap-1, -0x1.beb050p0
  },
  { // Entry 123
    0x1.e9e518fb1617eceb976b420930a3ce51p-1,
    0x1.e0ee8ap-1, 0x1.68f880p-1
  },
  { // Entry 124
    0x1.a0bdbae7d1b95d1adb05939aefcd35f2p-93,
    0x1.e13d0ep-1, 0x1.0220p10
  },
  { // Entry 125
    0x1.8789269c3d7361f6464f369baecd358fp-90,
    0x1.e1f07ep-1, 0x1.p10
  },
  { // Entry 126
    0x1.f67dcb0d034ec28a4309c4415565f9ecp1,
    0x1.e4000ep5, 0x1.55555ap-2
  },
  { // Entry 127
    0x1.e65785986fb7af1219234980dca4ef34p-93,
    0x1.e6f314p-1, 0x1.3e0f80p10
  },
  { // Entry 128
    0x1.e9a57691f06acbd3893901e376830537p-125,
    0x1.e97470p-1, 0x1.dd67c0p10
  },
  { // Entry 129
    0x1.866ec900017d689ca5deb18c4769effbp-2,
    0x1.f040c8p-1, 0x1.eddbacp4
  },
  { // Entry 130
    0x1.f839d5101bcf305e04c187afb53a6c53p-1,
    0x1.f091e2p-1, 0x1.p-1
  },
  { // Entry 131
    0x1.a374c2b00b62172cd4678df5e503b6f1p-13,
    0x1.f60c04p-14, 0x1.e2e42ep-1
  },
  { // Entry 132
    0x1.f8479b115561f17028b236fb8f2c173fp-1,
    0x1.ff174ap-1, 0x1.119996p3
  },
  { // Entry 133
    0x1.f947cf0debb3f5149df66e08396f65c5p3,
    0x1.ff1ffep3, 0x1.fddffep-1
  },
  { // Entry 134
    0x1.f3ae6b36c3163cd2d42f1eddf4e95886p-1,
    0x1.ff7ffep0, -0x1.203c88p-5
  },
  { // Entry 135
    0x1.f842b5127e562bf4cc2fb2aa30312393p-1,
    0x1.ffbffep-10, 0x1.3ffffep-9
  },
  { // Entry 136
    0x1.fc042cfcabd3d00c3fd7e9d168a20182p-1,
    0x1.ffc0p-1, 0x1.ffc7fep3
  },
  { // Entry 137
    0x1.f836cd12927fe3ea2eb9810462c208dcp-1,
    0x1.fff77ep100, -0x1.cb0968p-13
  },
  { // Entry 138
    0x1.fff3fd0c0608c60d8c3c9f07648607d5p14,
    0x1.fff7fep9, 0x1.80p0
  },
  { // Entry 139
    0x1.f811d3140d17296dc633cd00bfd96387p-1,
    0x1.fff8p-1, 0x1.ff80p7
  },
  { // Entry 140
    0x1.fe9d9738d0ca9f11f97a71b1a366145ap-1,
    0x1.fffefep1, -0x1.fffff8p-10
  },
  { // Entry 141
    0x1.f202b300003069a7a886e44fbf6073c2p72,
    0x1.ffff3ep127, 0x1.23d714p-1
  },
  { // Entry 142
    0x1.b834a192875d72ac81b7915cf8979690p-96,
    0x1.ffffbep-1, 0x1.p25
  },
  { // Entry 143
    0x1.ffffed000029bffddef5495e5603ce3bp-1,
    0x1.ffffe2p-1, 0x1.44443ep-1
  },
  { // Entry 144
    0x1.d6ab5d0e7ae03433ad824616d0db8b03p15,
    0x1.ffffeep-40, -0x1.a0ea0cp-2
  },
  { // Entry 145
    0x1.000002fffffffffff27fffe4ffffebc0p0,
    0x1.fffffap-1, -0x1.fffffap-1
  },
  { // Entry 146
    0x1.00000000000400000400000d55556d55p0,
    0x1.fffffcp-1, -0x1.p-23
  },
  { // Entry 147
    0x1.00000000000380000380000acaaabdeap0,
    0x1.fffffcp-1, -0x1.c0p-24
  },
  { // Entry 148
    0x1.ee8fc930954d29b3e28c5c1eafb9f7fdp4,
    0x1.fffffcp-67, -0x1.33334ep-4
  },
  { // Entry 149
    0x1.6a0a0cfff3ffa00e753af84c0100fbf0p13,
    0x1.fffffcp-106, -0x1.075078p-3
  },
  { // Entry 150
    0x1.9aaabcfff2ae3e7c84e87085640355e4p48,
    0x1.fffffcp-120, -0x1.a2e8bep-2
  },
  { // Entry 151
    0x1.f5777afffe2b46a4da98759043de4862p-52,
    0x1.fffffcp80, -0x1.428f58p-1
  },
  { // Entry 152
    0x1.73d3321e7f247def1ed4c816c824c77dp-67,
    0x1.fffffcp119, -0x1.1b91b4p-1
  },
  { // Entry 153
    0x1.000001000001000001000001000001p10,
    0x1.fffffep-11, -0x1.p0
  },
  { // Entry 154
    0x1.fffffeffffffbfffffdfffffebfffff1p0,
    0x1.fffffep1, 0x1.p-1
  },
  { // Entry 155
    0x1.cb5a0d0002f5169a13de39863bb5f91dp-2,
    0x1.421efap-2, 0x1.62e42ep-1
  },
  { // Entry 156
    0x1.cb720dcef90691503cbd1e949db761d9p-1,
    0x1.p-5, 0x1.p-5
  },
  { // Entry 157
    0x1.p-5,
    0x1.p-5, 0x1.p0
  },
  { // Entry 158
    0x1.p0,
    0x1.p0, 0x1.p-5
  },
  { // Entry 159
    0x1.p0,
    0x1.p0, 0x1.p0
  },
  { // Entry 160
    0x1.p-40,
    0x1.p-5, 0x1.p3
  },
  { // Entry 161
    0.0f,
    0x1.p-5, 0x1.p5
  },
  { // Entry 162
    0x1.p0,
    0x1.p0, 0x1.p3
  },
  { // Entry 163
    0x1.p0,
    0x1.p0, 0x1.p5
  },
  { // Entry 164
    0.0f,
    0x1.p-5, 0x1.p10
  },
  { // Entry 165
    0.0f,
    0x1.p-5, 0x1.p12
  },
  { // Entry 166
    0x1.p0,
    0x1.p0, 0x1.p10
  },
  { // Entry 167
    0x1.p0,
    0x1.p0, 0x1.p12
  },
  { // Entry 168
    0x1.11301d0125b50a4ebbf1aed9318ceac5p0,
    0x1.p3, 0x1.p-5
  },
  { // Entry 169
    0x1.p3,
    0x1.p3, 0x1.p0
  },
  { // Entry 170
    0x1.1d4873168b9aa7805b8028990f07a98bp0,
    0x1.p5, 0x1.p-5
  },
  { // Entry 171
    0x1.p5,
    0x1.p5, 0x1.p0
  },
  { // Entry 172
    0x1.p24,
    0x1.p3, 0x1.p3
  },
  { // Entry 173
    0x1.p96,
    0x1.p3, 0x1.p5
  },
  { // Entry 174
    0x1.p40,
    0x1.p5, 0x1.p3
  },
  { // Entry 175
    HUGE_VALF,
    0x1.p5, 0x1.p5
  },
  { // Entry 176
    HUGE_VALF,
    0x1.p3, 0x1.p10
  },
  { // Entry 177
    HUGE_VALF,
    0x1.p3, 0x1.p12
  },
  { // Entry 178
    HUGE_VALF,
    0x1.p5, 0x1.p10
  },
  { // Entry 179
    HUGE_VALF,
    0x1.p5, 0x1.p12
  },
  { // Entry 180
    0x1.3dea64c12342235b41223e13d773fba2p0,
    0x1.p10, 0x1.p-5
  },
  { // Entry 181
    0x1.p10,
    0x1.p10, 0x1.p0
  },
  { // Entry 182
    0x1.4bfdad5362a271d4397afec42e20e036p0,
    0x1.p12, 0x1.p-5
  },
  { // Entry 183
    0x1.p12,
    0x1.p12, 0x1.p0
  },
  { // Entry 184
    0x1.p80,
    0x1.p10, 0x1.p3
  },
  { // Entry 185
    HUGE_VALF,
    0x1.p10, 0x1.p5
  },
  { // Entry 186
    0x1.p96,
    0x1.p12, 0x1.p3
  },
  { // Entry 187
    HUGE_VALF,
    0x1.p12, 0x1.p5
  },
  { // Entry 188
    0x1.00000126055cfd443c5376930d169f32p2,
    0x1.6a09e6p-1, -0x1.p2
  },
  { // Entry 189
    0x1.fffffdb3f548a8d827b65c88p-3,
    0x1.6a09e6p-1, 0x1.p2
  },
  { // Entry 190
    0x1.00000126055cfd443c5376930d169f32p-2,
    0x1.6a09e6p0, -0x1.p2
  },
  { // Entry 191
    0x1.fffffdb3f548a8d827b65c88p1,
    0x1.6a09e6p0, 0x1.p2
  },
  { // Entry 192
    0x1.00000126055cfd443c5376930d169f32p2,
    0x1.6a09e6p-1, -0x1.p2
  },
  { // Entry 193
    0x1.fffffdb3f548a8d827b65c88p-3,
    0x1.6a09e6p-1, 0x1.p2
  },
  { // Entry 194
    0x1.00000126055cfd443c5376930d169f32p-2,
    0x1.6a09e6p0, -0x1.p2
  },
  { // Entry 195
    0x1.fffffdb3f548a8d827b65c88p1,
    0x1.6a09e6p0, 0x1.p2
  },
  { // Entry 196
    0x1.00162f3916670d119697154ae3512c2dp0,
    0x1.6a09e6p-1, -0x1.p-10
  },
  { // Entry 197
    0x1.ffd3a565caf8d230dae1250693a55f23p-1,
    0x1.6a09e6p-1, 0x1.p-10
  },
  { // Entry 198
    0x1.ffd3a5661473cb269f894b40d6cf9bacp-1,
    0x1.6a09e6p0, -0x1.p-10
  },
  { // Entry 199
    0x1.00162f38f1a33230bc340bd3752fc094p0,
    0x1.6a09e6p0, 0x1.p-10
  },
  { // Entry 200
    0x1.948b0fcd6e9e06522c3f35ba781948b0p1,
    0x1.80p-1, -0x1.p2
  },
  { // Entry 201
    0x1.44p-2,
    0x1.80p-1, 0x1.p2
  },
  { // Entry 202
    0x1.948b0fcd6e9e06522c3f35ba781948b0p-3,
    0x1.80p0, -0x1.p2
  },
  { // Entry 203
    0x1.44p2,
    0x1.80p0, 0x1.p2
  },
  { // Entry 204
    0x1.279a74590331c4d218f81e4afb257d06p0,
    0x1.80p-1, -0x1.p-1
  },
  { // Entry 205
    0x1.bb67ae8584caa73b25742d7078b83b89p-1,
    0x1.80p-1, 0x1.p-1
  },
  { // Entry 206
    0x1.a20bd700c2c3dfc042cc1aed7871db45p-1,
    0x1.80p0, -0x1.p-1
  },
  { // Entry 207
    0x1.3988e1409212e7d0321914321a556473p0,
    0x1.80p0, 0x1.p-1
  },
  { // Entry 208
    0x1.00126a0b93db294cabe33da735437f51p0,
    0x1.80p-1, -0x1.p-10
  },
  { // Entry 209
    0x1.ffdb2e8ed2a1fe71bd59fdd610313046p-1,
    0x1.80p-1, 0x1.p-10
  },
  { // Entry 210
    0x1.ffcc1c5973b2129a5b1424e0c88786b8p-1,
    0x1.80p0, -0x1.p-10
  },
  { // Entry 211
    0x1.0019f474aa190038c6af775d92f1d725p0,
    0x1.80p0, 0x1.p-10
  },
  { // Entry 212
    0x1.p0,
    0x1.p0, -0x1.p2
  },
  { // Entry 213
    0x1.p0,
    0x1.p0, 0x1.p2
  },
  { // Entry 214
    0x1.p-4,
    0x1.p1, -0x1.p2
  },
  { // Entry 215
    0x1.p4,
    0x1.p1, 0x1.p2
  },
  { // Entry 216
    0x1.p0,
    0x1.p0, -0x1.p-1
  },
  { // Entry 217
    0x1.p0,
    0x1.p0, 0x1.p-1
  },
  { // Entry 218
    0x1.6a09e667f3bcc908b2fb1366ea957d3ep-1,
    0x1.p1, -0x1.p-1
  },
  { // Entry 219
    0x1.6a09e667f3bcc908b2fb1366ea957d3ep0,
    0x1.p1, 0x1.p-1
  },
  { // Entry 220
    0x1.p0,
    0x1.p0, -0x1.p-10
  },
  { // Entry 221
    0x1.p0,
    0x1.p0, 0x1.p-10
  },
  { // Entry 222
    0x1.ffa74ea381efc217a773f15c025f7c0dp-1,
    0x1.p1, -0x1.p-10
  },
  { // Entry 223
    0x1.002c605e2e8cec506d21bfc89a23a010p0,
    0x1.p1, 0x1.p-10
  },
  { // Entry 224
    0x1.p40,
    0x1.p-10, -0x1.p2
  },
  { // Entry 225
    0x1.p-40,
    0x1.p-10, 0x1.p2
  },
  { // Entry 226
    0x1.fe013f6045e40a7c41499223b4a38ce8p-1,
    0x1.0040p0, -0x1.p2
  },
  { // Entry 227
    0x1.0100601001p0,
    0x1.0040p0, 0x1.p2
  },
  { // Entry 228
    0x1.p5,
    0x1.p-10, -0x1.p-1
  },
  { // Entry 229
    0x1.p-5,
    0x1.p-10, 0x1.p-1
  },
  { // Entry 230
    0x1.ffc00bfd808be0873653647448220fdfp-1,
    0x1.0040p0, -0x1.p-1
  },
  { // Entry 231
    0x1.001ffe003ff601bfac107ca6b29a0c31p0,
    0x1.0040p0, 0x1.p-1
  },
  { // Entry 232
    0x1.01bd1e77170b415e7626621eb5aaff61p0,
    0x1.p-10, -0x1.p-10
  },
  { // Entry 233
    0x1.fc8bc4866e8ad2b963e1828b0761cbc6p-1,
    0x1.p-10, 0x1.p-10
  },
  { // Entry 234
    0x1.ffffe0040055355844443df8680a8e05p-1,
    0x1.0040p0, -0x1.p-10
  },
  { // Entry 235
    0x1.00000ffe00d5256285340e4f3ad36287p0,
    0x1.0040p0, 0x1.p-10
  },
  { // Entry 236
    0x1.000001000001000001000001000001p-128,
    0x1.fffffep127, -0x1.p0
  },
  { // Entry 237
    0x1.fffffep127,
    0x1.fffffep127, 0x1.p0
  },
  { // Entry 238
    HUGE_VALF,
    0x1.p-149, -0x1.e66666p-1
  },
  { // Entry 239
    0x1.5db4ecab3e1cb942fc90a003e77da282p-142,
    0x1.p-149, 0x1.e66666p-1
  },
  { // Entry 240
    0.0f,
    0x1.fffffep-7, 0x1.fffffep5
  },
  { // Entry 241
    0.0f,
    0x1.fffffep-7, 0x1.p6
  },
  { // Entry 242
    0.0f,
    0x1.fffffep-7, 0x1.000002p6
  },
  { // Entry 243
    0.0f,
    0x1.p-6, 0x1.fffffep5
  },
  { // Entry 244
    0.0f,
    0x1.p-6, 0x1.p6
  },
  { // Entry 245
    0.0f,
    0x1.p-6, 0x1.000002p6
  },
  { // Entry 246
    0.0f,
    0x1.000002p-6, 0x1.fffffep5
  },
  { // Entry 247
    0.0f,
    0x1.000002p-6, 0x1.p6
  },
  { // Entry 248
    0.0f,
    0x1.000002p-6, 0x1.000002p6
  },
  { // Entry 249
    0.0f,
    0x1.fffffep-6, 0x1.fffffep4
  },
  { // Entry 250
    0.0f,
    0x1.fffffep-6, 0x1.p5
  },
  { // Entry 251
    0.0f,
    0x1.fffffep-6, 0x1.000002p5
  },
  { // Entry 252
    0.0f,
    0x1.p-5, 0x1.fffffep4
  },
  { // Entry 253
    0.0f,
    0x1.p-5, 0x1.p5
  },
  { // Entry 254
    0.0f,
    0x1.p-5, 0x1.000002p5
  },
  { // Entry 255
    0.0f,
    0x1.000002p-5, 0x1.fffffep4
  },
  { // Entry 256
    0.0f,
    0x1.000002p-5, 0x1.p5
  },
  { // Entry 257
    0.0f,
    0x1.000002p-5, 0x1.000002p5
  },
  { // Entry 258
    0x1.00001c5c879823e3af39baa221df84b0p-64,
    0x1.fffffep-5, 0x1.fffffep3
  },
  { // Entry 259
    0x1.ffffe00000effffba0000e37ffdde0p-65,
    0x1.fffffep-5, 0x1.p4
  },
  { // Entry 260
    0x1.ffff2e8e128f07f8aa95fb8b35d72ea4p-65,
    0x1.fffffep-5, 0x1.000002p4
  },
  { // Entry 261
    0x1.00002c5c89d5ec6ca4d7c8acc017b7c9p-64,
    0x1.p-4, 0x1.fffffep3
  },
  { // Entry 262
    0x1.p-64,
    0x1.p-4, 0x1.p4
  },
  { // Entry 263
    0x1.ffff4e8e06c7e8a2a84daed8ec56d6c3p-65,
    0x1.p-4, 0x1.000002p4
  },
  { // Entry 264
    0x1.00004c5c91217e02a4592ba7ad5df32ep-64,
    0x1.000002p-4, 0x1.fffffep3
  },
  { // Entry 265
    0x1.0000200001e00011800071c002220007p-64,
    0x1.000002p-4, 0x1.p4
  },
  { // Entry 266
    0x1.ffff8e8df4d9a8351320c05d3d814f9fp-65,
    0x1.000002p-4, 0x1.000002p4
  },
  { // Entry 267
    0x1.000008a2b26884f1068b81889467d67fp-24,
    0x1.fffffep-4, 0x1.fffffep2
  },
  { // Entry 268
    0x1.fffff0000037ffff9000008bffff90p-25,
    0x1.fffffep-4, 0x1.p3
  },
  { // Entry 269
    0x1.ffffad753d825dfcdd65e4ea54ccceb5p-25,
    0x1.fffffep-4, 0x1.000002p3
  },
  { // Entry 270
    0x1.000010a2b2c99a85707e8f13dc648710p-24,
    0x1.p-3, 0x1.fffffep2
  },
  { // Entry 271
    0x1.p-24,
    0x1.p-3, 0x1.p3
  },
  { // Entry 272
    0x1.ffffbd753b5607da2c260064823b30a7p-25,
    0x1.p-3, 0x1.000002p3
  },
  { // Entry 273
    0x1.000020a2b433c5b91729fe0493321d3fp-24,
    0x1.000002p-3, 0x1.fffffep2
  },
  { // Entry 274
    0x1.0000100000700001c00004600007p-24,
    0x1.000002p-3, 0x1.p3
  },
  { // Entry 275
    0x1.ffffdd75384d5b715e9437699534883bp-25,
    0x1.000002p-3, 0x1.000002p3
  },
  { // Entry 276
    0x1.0000018b90c2f02a80f3bb82aa12e95dp-8,
    0x1.fffffep-3, 0x1.fffffep1
  },
  { // Entry 277
    0x1.fffff800000bfffff8000002p-9,
    0x1.fffffep-3, 0x1.p2
  },
  { // Entry 278
    0x1.ffffe1d1bdd0bdc6b46ea64a42b1bad2p-9,
    0x1.fffffep-3, 0x1.000002p2
  },
  { // Entry 279
    0x1.0000058b90cf1e6d97f9ca14dbcc1628p-8,
    0x1.p-2, 0x1.fffffep1
  },
  { // Entry 280
    0x1.p-8,
    0x1.p-2, 0x1.p2
  },
  { // Entry 281
    0x1.ffffe9d1bd7c04bc4825147a8c0e63e3p-9,
    0x1.p-2, 0x1.000002p2
  },
  { // Entry 282
    0x1.00000d8b910b7af451a642e6d0b66b06p-8,
    0x1.000002p-2, 0x1.fffffep1
  },
  { // Entry 283
    0x1.000008000018000020000010p-8,
    0x1.000002p-2, 0x1.p2
  },
  { // Entry 284
    0x1.fffff9d1bd1a92a5d11088ed17417f41p-9,
    0x1.000002p-2, 0x1.000002p2
  },
  { // Entry 285
    0x1.fffffec5c8623fb25d7d06ac61a3063fp-3,
    0x1.fffffep-2, 0x1.fffffep0
  },
  { // Entry 286
    0x1.fffffc000002p-3,
    0x1.fffffep-2, 0x1.p1
  },
  { // Entry 287
    0x1.fffff6746f4d088289b880fe02adbfdep-3,
    0x1.fffffep-2, 0x1.000002p1
  },
  { // Entry 288
    0x1.00000162e430e5a18f6119e3c02282a5p-2,
    0x1.p-1, 0x1.fffffep0
  },
  { // Entry 289
    0x1.p-2,
    0x1.p-1, 0x1.p1
  },
  { // Entry 290
    0x1.fffffa746f47f160fcf890e3b801aeddp-3,
    0x1.p-1, 0x1.000002p1
  },
  { // Entry 291
    0x1.00000562e436713246f7a0134c8287eap-2,
    0x1.000002p-1, 0x1.fffffep0
  },
  { // Entry 292
    0x1.000004000004p-2,
    0x1.000002p-1, 0x1.p1
  },
  { // Entry 293
    0x1.0000013a37a4e18f0519a603954a5b0bp-2,
    0x1.000002p-1, 0x1.000002p1
  },
  { // Entry 294
    0x1.fffffe000001ffffff000000aaaaaa80p-1,
    0x1.fffffep-1, 0x1.fffffep-1
  },
  { // Entry 295
    0x1.fffffep-1,
    0x1.fffffep-1, 0x1.p0
  },
  { // Entry 296
    0x1.fffffdfffffc000002000004aaaaaaffp-1,
    0x1.fffffep-1, 0x1.000002p0
  },
  { // Entry 297
    0x1.p0,
    0x1.p0, 0x1.fffffep-1
  },
  { // Entry 298
    0x1.p0,
    0x1.p0, 0x1.p0
  },
  { // Entry 299
    0x1.p0,
    0x1.p0, 0x1.000002p0
  },
  { // Entry 300
    0x1.000001fffffdfffffe000003555553ffp0,
    0x1.000002p0, 0x1.fffffep-1
  },
  { // Entry 301
    0x1.000002p0,
    0x1.000002p0, 0x1.p0
  },
  { // Entry 302
    0x1.000002000004000004000005555558p0,
    0x1.000002p0, 0x1.000002p0
  },
  { // Entry 303
    0x1.6a09e53575b123625cc1968a665581a4p0,
    0x1.fffffep0, 0x1.fffffep-2
  },
  { // Entry 304
    0x1.6a09e5b2eec967cd97b2eff75f471493p0,
    0x1.fffffep0, 0x1.p-1
  },
  { // Entry 305
    0x1.6a09e6ade0fa7319052c4948dea48a76p0,
    0x1.fffffep0, 0x1.000002p-1
  },
  { // Entry 306
    0x1.6a09e5ea7aa390dbf868b7278b744829p0,
    0x1.p1, 0x1.fffffep-2
  },
  { // Entry 307
    0x1.6a09e667f3bcc908b2fb1366ea957d3ep0,
    0x1.p1, 0x1.p-1
  },
  { // Entry 308
    0x1.6a09e762e5efbbd7217018250a3ab194p0,
    0x1.p1, 0x1.000002p-1
  },
  { // Entry 309
    0x1.6a09e75484875c47c3cee01d9f348bd8p0,
    0x1.000002p1, 0x1.fffffep-2
  },
  { // Entry 310
    0x1.6a09e7d1fda27bf77d45272dd2d83a4bp0,
    0x1.000002p1, 0x1.p-1
  },
  { // Entry 311
    0x1.6a09e8ccefd93dcbecf54d233ea8265bp0,
    0x1.000002p1, 0x1.000002p-1
  },
  { // Entry 312
    0x1.6a09e58ff82a4ecedb73f766d3d0758dp0,
    0x1.fffffep1, 0x1.fffffep-3
  },
  { // Entry 313
    0x1.6a09e60d71430d1ad61b45d5d1abdf15p0,
    0x1.fffffep1, 0x1.p-2
  },
  { // Entry 314
    0x1.6a09e70863750c27c3dd5c0ecdce5271p0,
    0x1.fffffep1, 0x1.000002p-2
  },
  { // Entry 315
    0x1.6a09e5ea7aa390dbf868b7278b744829p0,
    0x1.p2, 0x1.fffffep-3
  },
  { // Entry 316
    0x1.6a09e667f3bcc908b2fb1366ea957d3ep0,
    0x1.p2, 0x1.p-2
  },
  { // Entry 317
    0x1.6a09e762e5efbbd7217018250a3ab194p0,
    0x1.p2, 0x1.000002p-2
  },
  { // Entry 318
    0x1.6a09e69f7f954950a1fce0a1b2c362d0p0,
    0x1.000002p2, 0x1.fffffep-3
  },
  { // Entry 319
    0x1.6a09e71cf8af753edb9700ad906c9cd9p0,
    0x1.000002p2, 0x1.p-2
  },
  { // Entry 320
    0x1.6a09e817eae44f9049d532cda2a90cb6p0,
    0x1.000002p2, 0x1.000002p-2
  },
  { // Entry 321
    0x1.4bfdacd3978adf9f3b64fe01f40593aep0,
    0x1.fffffep2, 0x1.fffffep-4
  },
  { // Entry 322
    0x1.4bfdad29e2ecb54005a6dbec67c5e413p0,
    0x1.fffffep2, 0x1.p-3
  },
  { // Entry 323
    0x1.4bfdadd679b0a3cc40ecb60afdc4a552p0,
    0x1.fffffep2, 0x1.000002p-3
  },
  { // Entry 324
    0x1.4bfdacfd174067ea4d43f8b09f974d86p0,
    0x1.p3, 0x1.fffffep-4
  },
  { // Entry 325
    0x1.4bfdad5362a271d4397afec42e20e036p0,
    0x1.p3, 0x1.p-3
  },
  { // Entry 326
    0x1.4bfdadfff966c8f2b8f44b137fbfaa96p0,
    0x1.p3, 0x1.000002p-3
  },
  { // Entry 327
    0x1.4bfdad5016ab0b9134e0574abca78b7ap0,
    0x1.000002p3, 0x1.fffffep-4
  },
  { // Entry 328
    0x1.4bfdada6620d7e0d6487fd9be64887a3p0,
    0x1.000002p3, 0x1.p-3
  },
  { // Entry 329
    0x1.4bfdae52f8d2a6506b74ce232fdcd291p0,
    0x1.000002p3, 0x1.000002p-3
  },
  { // Entry 330
    0x1.306fe05b533131c27612cfff7a0ffdb0p0,
    0x1.fffffep3, 0x1.fffffep-5
  },
  { // Entry 331
    0x1.306fe09014733fc18f2a8e5bc8a30cdcp0,
    0x1.fffffep3, 0x1.p-4
  },
  { // Entry 332
    0x1.306fe0f996f7772c9a94c16083446262p0,
    0x1.fffffep3, 0x1.000002p-4
  },
  { // Entry 333
    0x1.306fe06e5a2f2e8c620f7e55cc803dbap0,
    0x1.p4, 0x1.fffffep-5
  },
  { // Entry 334
    0x1.306fe0a31b7152de8d5a46305c85edecp0,
    0x1.p4, 0x1.p-4
  },
  { // Entry 335
    0x1.306fe10c9df5b6efbd400b7806005fa9p0,
    0x1.p4, 0x1.000002p-4
  },
  { // Entry 336
    0x1.306fe094682af29c8fe9f735fb1c4081p0,
    0x1.000002p4, 0x1.fffffep-5
  },
  { // Entry 337
    0x1.306fe0c9296d4394df5f99b9bd1a47d2p0,
    0x1.000002p4, 0x1.p-4
  },
  { // Entry 338
    0x1.306fe132abf200f257c612e07f149aa3p0,
    0x1.000002p4, 0x1.000002p-4
  },
  { // Entry 339
    0x1.1d4872eebb9da03bbac5af79b0cf9409p0,
    0x1.fffffep4, 0x1.fffffep-6
  },
  { // Entry 340
    0x1.1d48730da1570a7a85ea1fc1fcf88fddp0,
    0x1.fffffep4, 0x1.p-5
  },
  { // Entry 341
    0x1.1d48734b6cc9e902148fafcefa9eaa06p0,
    0x1.fffffep4, 0x1.000002p-5
  },
  { // Entry 342
    0x1.1d4872f7a5e133601ef3b495f3f89a12p0,
    0x1.p5, 0x1.fffffep-6
  },
  { // Entry 343
    0x1.1d4873168b9aa7805b8028990f07a98bp0,
    0x1.p5, 0x1.p-5
  },
  { // Entry 344
    0x1.1d487354570d99caccfbdb7e35ff0df1p0,
    0x1.p5, 0x1.000002p-5
  },
  { // Entry 345
    0x1.1d4873097a683fc01308d4a71615b820p0,
    0x1.000002p5, 0x1.fffffep-6
  },
  { // Entry 346
    0x1.1d4873286021c7a332496ee4ad91ade9p0,
    0x1.000002p5, 0x1.p-5
  },
  { // Entry 347
    0x1.1d4873662b94e1736939a503d83c5e42p0,
    0x1.000002p5, 0x1.000002p-5
  },
  { // Entry 348
    0x1.11301ceb20541ff3f655e3bd12271b3ep0,
    0x1.fffffep5, 0x1.fffffep-7
  },
  { // Entry 349
    0x1.11301cfce0f494304e630799fc8b181fp0,
    0x1.fffffep5, 0x1.p-6
  },
  { // Entry 350
    0x1.11301d206235801ef5580894354f900cp0,
    0x1.fffffep5, 0x1.000002p-6
  },
  { // Entry 351
    0x1.11301cef65149186a0ecb60713565b45p0,
    0x1.p6, 0x1.fffffep-7
  },
  { // Entry 352
    0x1.11301d0125b50a4ebbf1aed9318ceac5p0,
    0x1.p6, 0x1.p-6
  },
  { // Entry 353
    0x1.11301d24a6f5ff54e8d811a4b978b54fp0,
    0x1.p6, 0x1.000002p-6
  },
  { // Entry 354
    0x1.11301cf7ee956810edd94d1c7697f34bp0,
    0x1.000002p6, 0x1.fffffep-7
  },
  { // Entry 355
    0x1.11301d09af35e9f08ec0b6564cfd4d3ap0,
    0x1.000002p6, 0x1.p-6
  },
  { // Entry 356
    0x1.11301d2d3076f125c76f69bf107f4052p0,
    0x1.000002p6, 0x1.000002p-6
  },
  { // Entry 357
    0x1.fffc9d1eaff1e2bc708fbb9fc141d186p127,
    0x1.fffffcp0, 0x1.fffffcp6
  },
  { // Entry 358
    0x1.fffd4e8fb83933cbf5f827e2581f20dcp127,
    0x1.fffffcp0, 0x1.fffffep6
  },
  { // Entry 359
    0x1.fffe0000fdffaca81458f80ec301a2c8p127,
    0x1.fffffcp0, 0x1.p7
  },
  { // Entry 360
    0x1.ffff62e4420a6b06d702f4e2aaffa4e5p127,
    0x1.fffffcp0, 0x1.000002p7
  },
  { // Entry 361
    HUGE_VALF,
    0x1.fffffcp0, 0x1.000004p7
  },
  { // Entry 362
    0x1.fffd9d1d3e00d99bdfe3619f05f2ecc1p127,
    0x1.fffffep0, 0x1.fffffcp6
  },
  { // Entry 363
    0x1.fffe4e8ea000c3f99d84d886c03811fap127,
    0x1.fffffep0, 0x1.fffffep6
  },
  { // Entry 364
    0x1.ffff00003f7ff59501458fa07615868bp127,
    0x1.fffffep0, 0x1.p7
  },
  { // Entry 365
    HUGE_VALF,
    0x1.fffffep0, 0x1.000002p7
  },
  { // Entry 366
    HUGE_VALF,
    0x1.fffffep0, 0x1.000004p7
  },
  { // Entry 367
    0x1.fffe9d1c4b0f37f413d44c66c0481834p127,
    0x1.p1, 0x1.fffffcp6
  },
  { // Entry 368
    0x1.ffff4e8e06c7e8a2a84daed8ec56d6c3p127,
    0x1.p1, 0x1.fffffep6
  },
  { // Entry 369
    HUGE_VALF,
    0x1.p1, 0x1.p7
  },
  { // Entry 370
    HUGE_VALF,
    0x1.p1, 0x1.000002p7
  },
  { // Entry 371
    HUGE_VALF,
    0x1.p1, 0x1.000004p7
  },
  { // Entry 372
    HUGE_VALF,
    0x1.000002p1, 0x1.fffffcp6
  },
  { // Entry 373
    HUGE_VALF,
    0x1.000002p1, 0x1.fffffep6
  },
  { // Entry 374
    HUGE_VALF,
    0x1.000002p1, 0x1.p7
  },
  { // Entry 375
    HUGE_VALF,
    0x1.000002p1, 0x1.000002p7
  },
  { // Entry 376
    HUGE_VALF,
    0x1.000002p1, 0x1.000004p7
  },
  { // Entry 377
    HUGE_VALF,
    0x1.000004p1, 0x1.fffffcp6
  },
  { // Entry 378
    HUGE_VALF,
    0x1.000004p1, 0x1.fffffep6
  },
  { // Entry 379
    HUGE_VALF,
    0x1.000004p1, 0x1.p7
  },
  { // Entry 380
    HUGE_VALF,
    0x1.000004p1, 0x1.000002p7
  },
  { // Entry 381
    HUGE_VALF,
    0x1.000004p1, 0x1.000004p7
  },
  { // Entry 382
    0.0f,
    0x1.fffffcp0, -0x1.2c0004p7
  },
  { // Entry 383
    0.0f,
    0x1.fffffcp0, -0x1.2c0002p7
  },
  { // Entry 384
    0.0f,
    0x1.fffffcp0, -0x1.2cp7
  },
  { // Entry 385
    0.0f,
    0x1.fffffcp0, -0x1.2bfffep7
  },
  { // Entry 386
    0.0f,
    0x1.fffffcp0, -0x1.2bfffcp7
  },
  { // Entry 387
    0.0f,
    0x1.fffffep0, -0x1.2c0004p7
  },
  { // Entry 388
    0.0f,
    0x1.fffffep0, -0x1.2c0002p7
  },
  { // Entry 389
    0.0f,
    0x1.fffffep0, -0x1.2cp7
  },
  { // Entry 390
    0.0f,
    0x1.fffffep0, -0x1.2bfffep7
  },
  { // Entry 391
    0.0f,
    0x1.fffffep0, -0x1.2bfffcp7
  },
  { // Entry 392
    0.0f,
    0x1.p1, -0x1.2c0004p7
  },
  { // Entry 393
    0.0f,
    0x1.p1, -0x1.2c0002p7
  },
  { // Entry 394
    0.0f,
    0x1.p1, -0x1.2cp7
  },
  { // Entry 395
    0.0f,
    0x1.p1, -0x1.2bfffep7
  },
  { // Entry 396
    0.0f,
    0x1.p1, -0x1.2bfffcp7
  },
  { // Entry 397
    0.0f,
    0x1.000002p1, -0x1.2c0004p7
  },
  { // Entry 398
    0.0f,
    0x1.000002p1, -0x1.2c0002p7
  },
  { // Entry 399
    0.0f,
    0x1.000002p1, -0x1.2cp7
  },
  { // Entry 400
    0.0f,
    0x1.000002p1, -0x1.2bfffep7
  },
  { // Entry 401
    0.0f,
    0x1.000002p1, -0x1.2bfffcp7
  },
  { // Entry 402
    0.0f,
    0x1.000004p1, -0x1.2c0004p7
  },
  { // Entry 403
    0.0f,
    0x1.000004p1, -0x1.2c0002p7
  },
  { // Entry 404
    0.0f,
    0x1.000004p1, -0x1.2cp7
  },
  { // Entry 405
    0.0f,
    0x1.000004p1, -0x1.2bfffep7
  },
  { // Entry 406
    0.0f,
    0x1.000004p1, -0x1.2bfffcp7
  },
  { // Entry 407
    0.0f,
    0x1.db6db2p-2, 0x1.c30c2cp8
  },
  { // Entry 408
    0.0f,
    0x1.db6db2p-2, 0x1.c30c2ep8
  },
  { // Entry 409
    0.0f,
    0x1.db6db2p-2, 0x1.c30c30p8
  },
  { // Entry 410
    0.0f,
    0x1.db6db2p-2, 0x1.c30c32p8
  },
  { // Entry 411
    0.0f,
    0x1.db6db2p-2, 0x1.c30c34p8
  },
  { // Entry 412
    0.0f,
    0x1.db6db4p-2, 0x1.c30c2cp8
  },
  { // Entry 413
    0.0f,
    0x1.db6db4p-2, 0x1.c30c2ep8
  },
  { // Entry 414
    0.0f,
    0x1.db6db4p-2, 0x1.c30c30p8
  },
  { // Entry 415
    0.0f,
    0x1.db6db4p-2, 0x1.c30c32p8
  },
  { // Entry 416
    0.0f,
    0x1.db6db4p-2, 0x1.c30c34p8
  },
  { // Entry 417
    0.0f,
    0x1.db6db6p-2, 0x1.c30c2cp8
  },
  { // Entry 418
    0.0f,
    0x1.db6db6p-2, 0x1.c30c2ep8
  },
  { // Entry 419
    0.0f,
    0x1.db6db6p-2, 0x1.c30c30p8
  },
  { // Entry 420
    0.0f,
    0x1.db6db6p-2, 0x1.c30c32p8
  },
  { // Entry 421
    0.0f,
    0x1.db6db6p-2, 0x1.c30c34p8
  },
  { // Entry 422
    0.0f,
    0x1.db6db8p-2, 0x1.c30c2cp8
  },
  { // Entry 423
    0.0f,
    0x1.db6db8p-2, 0x1.c30c2ep8
  },
  { // Entry 424
    0.0f,
    0x1.db6db8p-2, 0x1.c30c30p8
  },
  { // Entry 425
    0.0f,
    0x1.db6db8p-2, 0x1.c30c32p8
  },
  { // Entry 426
    0.0f,
    0x1.db6db8p-2, 0x1.c30c34p8
  },
  { // Entry 427
    0.0f,
    0x1.db6dbap-2, 0x1.c30c2cp8
  },
  { // Entry 428
    0.0f,
    0x1.db6dbap-2, 0x1.c30c2ep8
  },
  { // Entry 429
    0.0f,
    0x1.db6dbap-2, 0x1.c30c30p8
  },
  { // Entry 430
    0.0f,
    0x1.db6dbap-2, 0x1.c30c32p8
  },
  { // Entry 431
    0.0f,
    0x1.db6dbap-2, 0x1.c30c34p8
  },
  { // Entry 432
    -0x1.fffffc000007fffff000001fffffc0p-1,
    -0x1.000002p0, -0x1.p0
  },
  { // Entry 433
    -0x1.p0,
    -0x1.p0, -0x1.p0
  },
  { // Entry 434
    -0x1.000001000001000001000001000001p0,
    -0x1.fffffep-1, -0x1.p0
  },
  { // Entry 435
    HUGE_VALF,
    0x1.p1, 0x1.p10
  },
  { // Entry 436
    HUGE_VALF,
    0x1.p2, 0x1.p9
  },
  { // Entry 437
    0.0f,
    0x1.fffffep-2, 0x1.fffffep9
  },
  { // Entry 438
    0.0f,
    0x1.fffffep-2, 0x1.p10
  },
  { // Entry 439
    0.0f,
    0x1.fffffep-2, 0x1.000002p10
  },
  { // Entry 440
    0.0f,
    0x1.p-1, 0x1.fffffep9
  },
  { // Entry 441
    0.0f,
    0x1.p-1, 0x1.p10
  },
  { // Entry 442
    0.0f,
    0x1.p-1, 0x1.000002p10
  },
  { // Entry 443
    0.0f,
    0x1.000002p-1, 0x1.fffffep9
  },
  { // Entry 444
    0.0f,
    0x1.000002p-1, 0x1.p10
  },
  { // Entry 445
    0.0f,
    0x1.000002p-1, 0x1.000002p10
  },
  { // Entry 446
    0x1.00020467109547572fa8f3f653eda548p-149,
    0x1.p-149, 0x1.fffff6p-1
  },
  { // Entry 447
    0x1.00019d1eed21f448f2c6217eab3d9c55p-149,
    0x1.p-149, 0x1.fffff8p-1
  },
  { // Entry 448
    0x1.000135d6f3596e086d463376a9dbd1e2p-149,
    0x1.p-149, 0x1.fffffap-1
  },
  { // Entry 449
    0x1.0000ce8f233ba3c64adc5667a7b0b245p-149,
    0x1.p-149, 0x1.fffffcp-1
  },
  { // Entry 450
    0x1.000067477cc884b33e03d0bb77571150p-149,
    0x1.p-149, 0x1.fffffep-1
  },
  { // Entry 451
    0x1.p-149,
    0x1.p-149, 0x1.p0
  },
  { // Entry 452
    0.0f,
    0x1.p-149, 0x1.000002p0
  },
  { // Entry 453
    0.0f,
    0x1.p-149, 0x1.000004p0
  },
  { // Entry 454
    0.0f,
    0x1.p-149, 0x1.000006p0
  },
  { // Entry 455
    0.0f,
    0x1.p-149, 0x1.000008p0
  },
  { // Entry 456
    0.0f,
    0x1.p-149, 0x1.00000ap0
  },
  { // Entry 457
    0x1.000200efcf25bab1c7cd22827341ab63p-148,
    0x1.p-148, 0x1.fffff6p-1
  },
  { // Entry 458
    0x1.00019a59204c82fe060cf6d320f15433p-148,
    0x1.p-148, 0x1.fffff8p-1
  },
  { // Entry 459
    0x1.000133c29a8f64f204da13b72ebc56edp-148,
    0x1.p-148, 0x1.fffffap-1
  },
  { // Entry 460
    0x1.0000cd2c3dee501480729506593fd68bp-148,
    0x1.p-148, 0x1.fffffcp-1
  },
  { // Entry 461
    0x1.000066960a6933ec3bae8cab9ccfd543p-148,
    0x1.p-148, 0x1.fffffep-1
  },
  { // Entry 462
    0x1.p-148,
    0x1.p-148, 0x1.p0
  },
  { // Entry 463
    0x1.fffe65a8cd021dedd55a40c272dc8acap-149,
    0x1.p-148, 0x1.000002p0
  },
  { // Entry 464
    0x1.fffccb52e2e1f2602021820ab47036fep-149,
    0x1.p-148, 0x1.000004p0
  },
  { // Entry 465
    0x1.fffb30fe419e75c552c074b75e9e132dp-149,
    0x1.p-148, 0x1.000006p0
  },
  { // Entry 466
    0x1.fff996aae936a08cb2de3b831326836cp-149,
    0x1.p-148, 0x1.000008p0
  },
  { // Entry 467
    0x1.fff7fc58d9a96b26595dc1b91aab1065p-149,
    0x1.p-148, 0x1.00000ap0
  },
  { // Entry 468
    0x1.8002fe5d326e1910dcf5adadc4fb80bap-148,
    0x1.80p-148, 0x1.fffff6p-1
  },
  { // Entry 469
    0x1.80026516e130410cbc34d6be1f314af3p-148,
    0x1.80p-148, 0x1.fffff8p-1
  },
  { // Entry 470
    0x1.8001cbd0cd20048dc0041aae6853f414p-148,
    0x1.80p-148, 0x1.fffffap-1
  },
  { // Entry 471
    0x1.8001328af63d4b28b93bac168d323776p-148,
    0x1.80p-148, 0x1.fffffcp-1
  },
  { // Entry 472
    0x1.800099455c87fc728272d7993c3c0ed2p-148,
    0x1.80p-148, 0x1.fffffep-1
  },
  { // Entry 473
    0x1.80p-148,
    0x1.80p-148, 0x1.p0
  },
  { // Entry 474
    0x1.7ffecd75fe779c39da312a0ae6575aaep-148,
    0x1.80p-148, 0x1.000002p0
  },
  { // Entry 475
    0x1.7ffd9aecf1a35c7e2d6f67b9177b8bc8p-148,
    0x1.80p-148, 0x1.000004p0
  },
  { // Entry 476
    0x1.7ffc6864d9827d757b4b6001d0c80a9bp-148,
    0x1.80p-148, 0x1.000006p0
  },
  { // Entry 477
    0x1.7ffb35ddb6143bc8e145a6d616a1b551p-148,
    0x1.80p-148, 0x1.000008p0
  },
  { // Entry 478
    0x1.7ffa03578757d42218ce40a578c74476p-148,
    0x1.80p-148, 0x1.00000ap0
  },
  { // Entry 479
    0x1.000000a0cf65eb1817a7095d9a0443a7p0,
    0x1.p-29, -0x1.p-29
  },
  { // Entry 480
    0x1.ffffff5f309a60aad5c2309f81f90defp-1,
    0x1.p-29, 0x1.p-30
  },
  { // Entry 481
    0x1.fffffd9e07cf07767a55afbe9acae93ep-1,
    0x1.p55, -0x1.p-29
  },
  { // Entry 482
    0x1.000000987e0cc66344d89b494e1f43b3p0,
    0x1.p55, 0x1.p-30
  },
  { // Entry 483
    0x1.fffffd669427cf159515873887c17cf2p-1,
    0x1.p60, -0x1.p-29
  },
  { // Entry 484
    0x1.000000a65af6ae61be88ea2558790cd7p0,
    0x1.p60, 0x1.p-30
  },
  { // Entry 485
    0x1.ffc003ffb55aaa4cd34f3431ea5b77f1p-1,
    0x1.fffffep-1, 0x1.p13
  },
  { // Entry 486
    0x1.fe00ffa9c0fb3bf28c8a9b2b3d2d7daap-1,
    0x1.fffffep-1, 0x1.p16
  },
  { // Entry 487
    0x1.p0,
    0x1.p0, 0x1.p13
  },
  { // Entry 488
    0x1.p0,
    0x1.p0, 0x1.p16
  },
  { // Entry 489
    0x1.004008006aa554332b8fed09d8ed29f3p0,
    0x1.000002p0, 0x1.p13
  },
  { // Entry 490
    0x1.02020153fc405b123b33a73cb93a3648p0,
    0x1.000002p0, 0x1.p16
  },
  { // Entry 491
    0x1.2c15603269407006b8f35e8e4f1497bap-6,
    -0x1.000002p0, -0x1.p25
  },
  { // Entry 492
    0x1.c846887ee379c5af637c7349afc9f699p-47,
    -0x1.000002p0, -0x1.p28
  },
  { // Entry 493
    0x1.p0,
    -0x1.p0, -0x1.p25
  },
  { // Entry 494
    0x1.p0,
    -0x1.p0, -0x1.p28
  },
  { // Entry 495
    0x1.d8e64d66342891c86fb3c87d1ed6d5c5p2,
    -0x1.fffffep-1, -0x1.p25
  },
  { // Entry 496
    0x1.0f2ec583f611e4b8fc1cc7b50efbb738p23,
    -0x1.fffffep-1, -0x1.p28
  },
  { // Entry 497
    0x1.d8e64d66342891c86fb3c87d1ed6d5c5p2,
    0x1.fffffep-1, -0x1.p25
  },
  { // Entry 498
    0x1.0f2ec583f611e4b8fc1cc7b50efbb738p23,
    0x1.fffffep-1, -0x1.p28
  },
  { // Entry 499
    0x1.p0,
    0x1.p0, -0x1.p25
  },
  { // Entry 500
    0x1.p0,
    0x1.p0, -0x1.p28
  },
  { // Entry 501
    0x1.2c15603269407006b8f35e8e4f1497bap-6,
    0x1.000002p0, -0x1.p25
  },
  { // Entry 502
    0x1.c846887ee379c5af637c7349afc9f699p-47,
    0x1.000002p0, -0x1.p28
  },
  { // Entry 503
    HUGE_VALF,
    -0x1.p15, 0x1.p63
  },
  { // Entry 504
    HUGE_VALF,
    0.0f, -0x1.80p1
  },
  { // Entry 505
    -HUGE_VALF,
    -0.0f, -0x1.80p1
  },
  { // Entry 506
    HUGE_VALF,
    0.0f, -0x1.p0
  },
  { // Entry 507
    -HUGE_VALF,
    -0.0f, -0x1.p0
  },
  { // Entry 508
    HUGE_VALF,
    0.0f, -0x1.fffffep127
  },
  { // Entry 509
    HUGE_VALF,
    0.0f, -0x1.80p2
  },
  { // Entry 510
    HUGE_VALF,
    0.0f, -0x1.p1
  },
  { // Entry 511
    HUGE_VALF,
    0.0f, -0x1.000002p0
  },
  { // Entry 512
    HUGE_VALF,
    0.0f, -0x1.fffffep-1
  },
  { // Entry 513
    HUGE_VALF,
    0.0f, -0x1.p-126
  },
  { // Entry 514
    HUGE_VALF,
    0.0f, -0x1.p-149
  },
  { // Entry 515
    HUGE_VALF,
    -0.0f, -0x1.fffffep127
  },
  { // Entry 516
    HUGE_VALF,
    -0.0f, -0x1.80p2
  },
  { // Entry 517
    HUGE_VALF,
    -0.0f, -0x1.p1
  },
  { // Entry 518
    HUGE_VALF,
    -0.0f, -0x1.000002p0
  },
  { // Entry 519
    HUGE_VALF,
    -0.0f, -0x1.fffffep-1
  },
  { // Entry 520
    HUGE_VALF,
    -0.0f, -0x1.p-126
  },
  { // Entry 521
    HUGE_VALF,
    -0.0f, -0x1.p-149
  },
  { // Entry 522
    HUGE_VALF,
    0.0f, -HUGE_VALF
  },
  { // Entry 523
    HUGE_VALF,
    -0.0f, -HUGE_VALF
  },
  { // Entry 524
    0.0,
    0.0f, 0x1.80p1
  },
  { // Entry 525
    -0.0,
    -0.0f, 0x1.80p1
  },
  { // Entry 526
    0.0,
    0.0f, 0x1.p0
  },
  { // Entry 527
    -0.0,
    -0.0f, 0x1.p0
  },
  { // Entry 528
    0.0,
    0.0f, HUGE_VALF
  },
  { // Entry 529
    0.0,
    0.0f, 0x1.fffffep127
  },
  { // Entry 530
    0.0,
    0.0f, 0x1.80p2
  },
  { // Entry 531
    0.0,
    0.0f, 0x1.p1
  },
  { // Entry 532
    0.0,
    0.0f, 0x1.000002p0
  },
  { // Entry 533
    0.0,
    0.0f, 0x1.fffffep-1
  },
  { // Entry 534
    0.0,
    0.0f, 0x1.p-126
  },
  { // Entry 535
    0.0,
    0.0f, 0x1.p-149
  },
  { // Entry 536
    0.0,
    -0.0f, HUGE_VALF
  },
  { // Entry 537
    0.0,
    -0.0f, 0x1.fffffep127
  },
  { // Entry 538
    0.0,
    -0.0f, 0x1.80p2
  },
  { // Entry 539
    0.0,
    -0.0f, 0x1.p1
  },
  { // Entry 540
    0.0,
    -0.0f, 0x1.000002p0
  },
  { // Entry 541
    0.0,
    -0.0f, 0x1.fffffep-1
  },
  { // Entry 542
    0.0,
    -0.0f, 0x1.p-126
  },
  { // Entry 543
    0.0,
    -0.0f, 0x1.p-149
  },
  { // Entry 544
    0x1.p0,
    -0x1.p0, HUGE_VALF
  },
  { // Entry 545
    0x1.p0,
    -0x1.p0, -HUGE_VALF
  },
  { // Entry 546
    0x1.p0,
    0x1.p0, HUGE_VALF
  },
  { // Entry 547
    0x1.p0,
    0x1.p0, -HUGE_VALF
  },
  { // Entry 548
    0x1.p0,
    0x1.p0, 0x1.fffffep127
  },
  { // Entry 549
    0x1.p0,
    0x1.p0, -0x1.fffffep127
  },
  { // Entry 550
    0x1.p0,
    -0x1.p0, 0x1.fffffep127
  },
  { // Entry 551
    0x1.p0,
    -0x1.p0, -0x1.fffffep127
  },
  { // Entry 552
    0x1.p0,
    0x1.p0, 0x1.p-1
  },
  { // Entry 553
    0x1.p0,
    0x1.p0, -0x1.p-1
  },
  { // Entry 554
    0x1.p0,
    0x1.p0, 0x1.p-126
  },
  { // Entry 555
    0x1.p0,
    0x1.p0, -0x1.p-126
  },
  { // Entry 556
    0x1.p0,
    0x1.p0, 0x1.fffffcp-127
  },
  { // Entry 557
    0x1.p0,
    0x1.p0, -0x1.fffffcp-127
  },
  { // Entry 558
    0x1.p0,
    0x1.p0, 0x1.p-149
  },
  { // Entry 559
    0x1.p0,
    0x1.p0, -0x1.p-149
  },
  { // Entry 560
    0x1.p0,
    0x1.p0, 0.0f
  },
  { // Entry 561
    0x1.p0,
    0x1.p0, -0.0f
  },
  { // Entry 562
    0x1.p0,
    HUGE_VALF, 0.0f
  },
  { // Entry 563
    0x1.p0,
    HUGE_VALF, -0.0f
  },
  { // Entry 564
    0x1.p0,
    0x1.fffffep127, 0.0f
  },
  { // Entry 565
    0x1.p0,
    0x1.fffffep127, -0.0f
  },
  { // Entry 566
    0x1.p0,
    0x1.p-126, 0.0f
  },
  { // Entry 567
    0x1.p0,
    0x1.p-126, -0.0f
  },
  { // Entry 568
    0x1.p0,
    0x1.p-149, 0.0f
  },
  { // Entry 569
    0x1.p0,
    0x1.p-149, -0.0f
  },
  { // Entry 570
    0x1.p0,
    0.0f, 0.0f
  },
  { // Entry 571
    0x1.p0,
    0.0f, -0.0f
  },
  { // Entry 572
    0x1.p0,
    -0.0f, 0.0f
  },
  { // Entry 573
    0x1.p0,
    -0.0f, -0.0f
  },
  { // Entry 574
    0x1.p0,
    -0x1.p-149, 0.0f
  },
  { // Entry 575
    0x1.p0,
    -0x1.p-149, -0.0f
  },
  { // Entry 576
    0x1.p0,
    -0x1.p-126, 0.0f
  },
  { // Entry 577
    0x1.p0,
    -0x1.p-126, -0.0f
  },
  { // Entry 578
    0x1.p0,
    -0x1.fffffep127, 0.0f
  },
  { // Entry 579
    0x1.p0,
    -0x1.fffffep127, -0.0f
  },
  { // Entry 580
    0x1.p0,
    -HUGE_VALF, 0.0f
  },
  { // Entry 581
    0x1.p0,
    -HUGE_VALF, -0.0f
  },
  { // Entry 582
    HUGE_VALF,
    0x1.p-126, -HUGE_VALF
  },
  { // Entry 583
    HUGE_VALF,
    0x1.p-149, -HUGE_VALF
  },
  { // Entry 584
    HUGE_VALF,
    -0x1.p-149, -HUGE_VALF
  },
  { // Entry 585
    HUGE_VALF,
    -0x1.p-126, -HUGE_VALF
  },
  { // Entry 586
    0.0,
    HUGE_VALF, -HUGE_VALF
  },
  { // Entry 587
    0.0,
    0x1.fffffep127, -HUGE_VALF
  },
  { // Entry 588
    0.0,
    0x1.80p0, -HUGE_VALF
  },
  { // Entry 589
    0.0,
    -0x1.80p0, -HUGE_VALF
  },
  { // Entry 590
    0.0,
    -0x1.fffffep127, -HUGE_VALF
  },
  { // Entry 591
    0.0,
    -HUGE_VALF, -HUGE_VALF
  },
  { // Entry 592
    0.0,
    0x1.p-126, HUGE_VALF
  },
  { // Entry 593
    0.0,
    0x1.p-149, HUGE_VALF
  },
  { // Entry 594
    0.0,
    0.0f, HUGE_VALF
  },
  { // Entry 595
    0.0,
    -0.0f, HUGE_VALF
  },
  { // Entry 596
    0.0,
    -0x1.p-149, HUGE_VALF
  },
  { // Entry 597
    0.0,
    -0x1.p-126, HUGE_VALF
  },
  { // Entry 598
    HUGE_VALF,
    HUGE_VALF, HUGE_VALF
  },
  { // Entry 599
    HUGE_VALF,
    0x1.fffffep127, HUGE_VALF
  },
  { // Entry 600
    HUGE_VALF,
    0x1.80p0, HUGE_VALF
  },
  { // Entry 601
    HUGE_VALF,
    -0x1.80p0, HUGE_VALF
  },
  { // Entry 602
    HUGE_VALF,
    -0x1.fffffep127, HUGE_VALF
  },
  { // Entry 603
    HUGE_VALF,
    -HUGE_VALF, HUGE_VALF
  },
  { // Entry 604
    -0.0,
    -HUGE_VALF, -0x1.80p1
  },
  { // Entry 605
    -0.0,
    -HUGE_VALF, -0x1.p0
  },
  { // Entry 606
    0.0,
    -HUGE_VALF, -HUGE_VALF
  },
  { // Entry 607
    0.0,
    -HUGE_VALF, -0x1.921fb6p1
  },
  { // Entry 608
    0.0,
    -HUGE_VALF, -0x1.921fb6p0
  },
  { // Entry 609
    0.0,
    -HUGE_VALF, -0x1.fffffep127
  },
  { // Entry 610
    0.0,
    -HUGE_VALF, -0x1.80p2
  },
  { // Entry 611
    0.0,
    -HUGE_VALF, -0x1.p1
  },
  { // Entry 612
    0.0,
    -HUGE_VALF, -0x1.p-126
  },
  { // Entry 613
    0.0,
    -HUGE_VALF, -0x1.p-149
  },
  { // Entry 614
    -HUGE_VALF,
    -HUGE_VALF, 0x1.80p1
  },
  { // Entry 615
    -HUGE_VALF,
    -HUGE_VALF, 0x1.40p2
  },
  { // Entry 616
    HUGE_VALF,
    -HUGE_VALF, HUGE_VALF
  },
  { // Entry 617
    HUGE_VALF,
    -HUGE_VALF, 0x1.921fb6p1
  },
  { // Entry 618
    HUGE_VALF,
    -HUGE_VALF, 0x1.921fb6p0
  },
  { // Entry 619
    HUGE_VALF,
    -HUGE_VALF, 0x1.fffffep127
  },
  { // Entry 620
    HUGE_VALF,
    -HUGE_VALF, 0x1.80p2
  },
  { // Entry 621
    HUGE_VALF,
    -HUGE_VALF, 0x1.p1
  },
  { // Entry 622
    HUGE_VALF,
    -HUGE_VALF, 0x1.p-126
  },
  { // Entry 623
    HUGE_VALF,
    -HUGE_VALF, 0x1.p-149
  },
  { // Entry 624
    0.0,
    HUGE_VALF, -0x1.p-149
  },
  { // Entry 625
    0.0,
    HUGE_VALF, -0x1.p-126
  },
  { // Entry 626
    0.0,
    HUGE_VALF, -0x1.fffffep127
  },
  { // Entry 627
    0.0,
    HUGE_VALF, -HUGE_VALF
  },
  { // Entry 628
    HUGE_VALF,
    HUGE_VALF, HUGE_VALF
  },
  { // Entry 629
    HUGE_VALF,
    HUGE_VALF, 0x1.fffffep127
  },
  { // Entry 630
    HUGE_VALF,
    HUGE_VALF, 0x1.p-126
  },
  { // Entry 631
    HUGE_VALF,
    HUGE_VALF, 0x1.p-149
  },
  { // Entry 632
    HUGE_VALF,
    0x1.fffffep127, 0x1.fffffep127
  },
  { // Entry 633
    0.0f,
    0x1.p-126, 0x1.p1
  },
  { // Entry 634
    0.0f,
    -0x1.p-126, 0x1.p1
  },
  { // Entry 635
    0.0f,
    0x1.p-149, 0x1.p1
  },
  { // Entry 636
    0.0f,
    -0x1.p-149, 0x1.p1
  },
  { // Entry 637
    HUGE_VALF,
    HUGE_VALF, 0x1.p-1
  },
  { // Entry 638
    0x1.fffffeffffffbfffffdfffffebfffff1p63,
    0x1.fffffep127, 0x1.p-1
  },
  { // Entry 639
    0x1.6a09e667f3bcc908b2fb1366ea957d3ep-1,
    0x1.p-1, 0x1.p-1
  },
  { // Entry 640
    0x1.p-63,
    0x1.p-126, 0x1.p-1
  },
  { // Entry 641
    0x1.6a09e667f3bcc908b2fb1366ea957d3ep-75,
    0x1.p-149, 0x1.p-1
  },
  { // Entry 642
    0.0,
    0.0f, 0x1.p-1
  },
  { // Entry 643
    0.0,
    -0.0f, 0x1.p-1
  },
  { // Entry 644
    HUGE_VALF,
    -HUGE_VALF, 0x1.p-1
  },
  { // Entry 645
    0.0,
    HUGE_VALF, -0x1.p-1
  },
  { // Entry 646
    0x1.0000008000006000005000004600003fp-64,
    0x1.fffffep127, -0x1.p-1
  },
  { // Entry 647
    0x1.6a09e667f3bcc908b2fb1366ea957d3ep0,
    0x1.p-1, -0x1.p-1
  },
  { // Entry 648
    0x1.p63,
    0x1.p-126, -0x1.p-1
  },
  { // Entry 649
    0x1.6a09e667f3bcc908b2fb1366ea957d3ep74,
    0x1.p-149, -0x1.p-1
  },
  { // Entry 650
    HUGE_VALF,
    0.0f, -0x1.p-1
  },
  { // Entry 651
    HUGE_VALF,
    -0.0f, -0x1.p-1
  },
  { // Entry 652
    0.0,
    -HUGE_VALF, -0x1.p-1
  },
  { // Entry 653
    0.0,
    0x1.p-1, HUGE_VALF
  },
  { // Entry 654
    0.0f,
    0x1.p-1, 0x1.fffffep127
  },
  { // Entry 655
    0x1.6a09e667f3bcc908b2fb1366ea957d3ep-1,
    0x1.p-1, 0x1.p-1
  },
  { // Entry 656
    0x1.fffffffffffffffffffffffffffffffap-1,
    0x1.p-1, 0x1.p-126
  },
  { // Entry 657
    0x1.ffffffffffffffffffffffffffffffffp-1,
    0x1.p-1, 0x1.p-149
  },
  { // Entry 658
    0x1.p0,
    0x1.p-1, 0.0f
  },
  { // Entry 659
    0x1.p0,
    0x1.p-1, -0.0f
  },
  { // Entry 660
    0x1.p0,
    0x1.p-1, -0x1.p-149
  },
  { // Entry 661
    0x1.00000000000000000000000000000002p0,
    0x1.p-1, -0x1.p-126
  },
  { // Entry 662
    0x1.6a09e667f3bcc908b2fb1366ea957d3ep0,
    0x1.p-1, -0x1.p-1
  },
  { // Entry 663
    HUGE_VALF,
    0x1.p-1, -0x1.fffffep127
  },
  { // Entry 664
    HUGE_VALF,
    0x1.p-1, -HUGE_VALF
  },
  { // Entry 665
    0.0,
    -0x1.p-1, HUGE_VALF
  },
  { // Entry 666
    0.0f,
    -0x1.p-1, 0x1.fffffep127
  },
  { // Entry 667
    0x1.p0,
    -0x1.p-1, 0.0f
  },
  { // Entry 668
    0x1.p0,
    -0x1.p-1, -0.0f
  },
  { // Entry 669
    HUGE_VALF,
    -0x1.p-1, -0x1.fffffep127
  },
  { // Entry 670
    HUGE_VALF,
    -0x1.p-1, -HUGE_VALF
  },
  { // Entry 671
    0x1.p1,
    0x1.p2, 0x1.p-1
  },
  { // Entry 672
    0x1.80p1,
    0x1.20p3, 0x1.p-1
  },
  { // Entry 673
    0x1.p2,
    0x1.p4, 0x1.p-1
  },
  { // Entry 674
    0x1.p-1,
    0x1.p2, -0x1.p-1
  },
  { // Entry 675
    0x1.p-2,
    0x1.p4, -0x1.p-1
  },
  { // Entry 676
    0x1.p-3,
    0x1.p6, -0x1.p-1
  },
  { // Entry 677
    HUGE_VALF,
    0x1.fffffep-1, -0x1.74910cp62
  },
  { // Entry 678
    0.0f,
    0x1.fffffep-1, 0x1.74910cp62
  },
  { // Entry 679
    0x1.p2,
    0x1.p1, 0x1.p1
  },
  { // Entry 680
    0x1.p-2,
    0x1.p1, -0x1.p1
  },
  { // Entry 681
    0x1.p2,
    -0x1.p1, 0x1.p1
  },
  { // Entry 682
    0x1.p-2,
    -0x1.p1, -0x1.p1
  },
  { // Entry 683
    0x1.b0p4,
    0x1.80p1, 0x1.80p1
  },
  { // Entry 684
    0x1.86a0p11,
    0x1.40p2, 0x1.40p2
  },
  { // Entry 685
    0x1.921ee0p19,
    0x1.c0p2, 0x1.c0p2
  },
  { // Entry 686
    0x1.2a05f2p33,
    0x1.40p3, 0x1.40p3
  },
};
