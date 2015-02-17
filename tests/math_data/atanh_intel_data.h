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

static data_1_1_t<double, double> g_atanh_intel_data[] = {
  { // Entry 0
    -0x1.p-1074,
    -0x1.0p-1074
  },
  { // Entry 1
    0x1.p-1074,
    0x1.0p-1074
  },
  { // Entry 2
    -0x1.33c537256ac26ea1b8865a026e72c752p-1,
    -0x1.136p-1
  },
  { // Entry 3
    0x1.33c537256ac26ea1b8865a026e72c752p-1,
    0x1.136p-1
  },
  { // Entry 4
    -0x1.44767fdc853773ecd65b43b0efd1f8d6p-1,
    -0x1.1f07c1f07c1f4p-1
  },
  { // Entry 5
    0x1.44767fdc853773ecd65b43b0efd1f8d6p-1,
    0x1.1f07c1f07c1f4p-1
  },
  { // Entry 6
    -0x1.7761ddbd0573b7ff1d51e300bdb439bep-1,
    -0x1.400000010p-1
  },
  { // Entry 7
    0x1.7761ddbd0573b7ff1d51e300bdb439bep-1,
    0x1.400000010p-1
  },
  { // Entry 8
    -0x1.6259494a4bb397ff89dd84b74e230f31p-2,
    -0x1.54d9d811468c2p-2
  },
  { // Entry 9
    0x1.6259494a4bb397ff89dd84b74e230f31p-2,
    0x1.54d9d811468c2p-2
  },
  { // Entry 10
    -0x1.6719a6fbfef1d7fc326d067903183cddp-2,
    -0x1.5911c3a70cebdp-2
  },
  { // Entry 11
    0x1.6719a6fbfef1d7fc326d067903183cddp-2,
    0x1.5911c3a70cebdp-2
  },
  { // Entry 12
    -0x1.ad75b9841b24e264875483def1610c01p-1,
    -0x1.5ece354ff80a2p-1
  },
  { // Entry 13
    0x1.ad75b9841b24e264875483def1610c01p-1,
    0x1.5ece354ff80a2p-1
  },
  { // Entry 14
    -0x1.c08e6aa55e4172db4c413132b24283edp-1,
    -0x1.68ae1ca8f6ad6p-1
  },
  { // Entry 15
    0x1.c08e6aa55e4172db4c413132b24283edp-1,
    0x1.68ae1ca8f6ad6p-1
  },
  { // Entry 16
    -0x1.d6f10a7081e7ecc4a2d3d9e2371d1cb0p-1,
    -0x1.739ce739ce73cp-1
  },
  { // Entry 17
    0x1.d6f10a7081e7ecc4a2d3d9e2371d1cb0p-1,
    0x1.739ce739ce73cp-1
  },
  { // Entry 18
    -0x1.862796317ed3d7fcb8bec742b2ea5c0bp-2,
    -0x1.744f8613c514bp-2
  },
  { // Entry 19
    0x1.862796317ed3d7fcb8bec742b2ea5c0bp-2,
    0x1.744f8613c514bp-2
  },
  { // Entry 20
    -0x1.8027fe496eaad0006a3df4a7cfc399bap-5,
    -0x1.7fdff7fffffffp-5
  },
  { // Entry 21
    0x1.8027fe496eaad0006a3df4a7cfc399bap-5,
    0x1.7fdff7fffffffp-5
  },
  { // Entry 22
    -0x1.80602a138a48e581b7bf0a1d1f84769fp-10,
    -0x1.8060180601ff6p-10
  },
  { // Entry 23
    0x1.80602a138a48e581b7bf0a1d1f84769fp-10,
    0x1.8060180601ff6p-10
  },
  { // Entry 24
    -0x1.9f323ecbf984c5d61382119eafcddf36p-3,
    -0x1.999999999999ap-3
  },
  { // Entry 25
    0x1.9f323ecbf984c5d61382119eafcddf36p-3,
    0x1.999999999999ap-3
  },
  { // Entry 26
    -0x1.b7c54f4582a8f52cb0434b624cb3140bp-2,
    -0x1.9e9703735f652p-2
  },
  { // Entry 27
    0x1.b7c54f4582a8f52cb0434b624cb3140bp-2,
    0x1.9e9703735f652p-2
  },
  { // Entry 28
    -0x1.ac44a1f923250f86e06d88e6919a1a4fp-24,
    -0x1.ac44a1f923238p-24
  },
  { // Entry 29
    0x1.ac44a1f923250f86e06d88e6919a1a4fp-24,
    0x1.ac44a1f923238p-24
  },
  { // Entry 30
    -0x1.cee62c51688218abca36efcf5f6add63p-2,
    -0x1.b1bfa1c2ff5c8p-2
  },
  { // Entry 31
    0x1.cee62c51688218abca36efcf5f6add63p-2,
    0x1.b1bfa1c2ff5c8p-2
  },
  { // Entry 32
    -0x1.b4c1183827d4a805d64de6f870cd6888p-5,
    -0x1.b45746fb45980p-5
  },
  { // Entry 33
    0x1.b4c1183827d4a805d64de6f870cd6888p-5,
    0x1.b45746fb45980p-5
  },
  { // Entry 34
    -0x1.cd1ce8658f1e27f929bb26f71cf39ep-3,
    -0x1.c579d4043e054p-3
  },
  { // Entry 35
    0x1.cd1ce8658f1e27f929bb26f71cf39ep-3,
    0x1.c579d4043e054p-3
  },
  { // Entry 36
    -0x1.f4b9755f2c26e7fc906b87927f3076ecp-2,
    -0x1.d04b9bb0bda28p-2
  },
  { // Entry 37
    0x1.f4b9755f2c26e7fc906b87927f3076ecp-2,
    0x1.d04b9bb0bda28p-2
  },
  { // Entry 38
    -0x1.d49dd5cd8086d7fe196df1da63aadaf9p-4,
    -0x1.d29523bb69328p-4
  },
  { // Entry 39
    0x1.d49dd5cd8086d7fe196df1da63aadaf9p-4,
    0x1.d29523bb69328p-4
  },
  { // Entry 40
    -0x1.f7f60ac95611e75a2a085f35a7c508dcp-2,
    -0x1.d2dce780a7304p-2
  },
  { // Entry 41
    0x1.f7f60ac95611e75a2a085f35a7c508dcp-2,
    0x1.d2dce780a7304p-2
  },
  { // Entry 42
    -0x1.df875eb326b209b9c9a00f82e3dbc3bap-3,
    -0x1.d6f41e3ea643ap-3
  },
  { // Entry 43
    0x1.df875eb326b209b9c9a00f82e3dbc3bap-3,
    0x1.d6f41e3ea643ap-3
  },
  { // Entry 44
    -0x1.fe0dc4fabe81f72d042d459cdb17f7c5p-2,
    -0x1.d7ad1055ed587p-2
  },
  { // Entry 45
    0x1.fe0dc4fabe81f72d042d459cdb17f7c5p-2,
    0x1.d7ad1055ed587p-2
  },
  { // Entry 46
    -0x1.ede7fef85615d5762723a4bc9071bcfcp-4,
    -0x1.eb86b85bf65d8p-4
  },
  { // Entry 47
    0x1.ede7fef85615d5762723a4bc9071bcfcp-4,
    0x1.eb86b85bf65d8p-4
  },
  { // Entry 48
    -0x1.ffff0f05db419e0562a8a13e0c88ec0cp-3,
    -0x1.f59707e3f49d0p-3
  },
  { // Entry 49
    0x1.ffff0f05db419e0562a8a13e0c88ec0cp-3,
    0x1.f59707e3f49d0p-3
  },
  { // Entry 50
    -0x1.340af764783edfffac199b0ebf01c362p1,
    -0x1.f7cp-1
  },
  { // Entry 51
    0x1.340af764783edfffac199b0ebf01c362p1,
    0x1.f7cp-1
  },
  { // Entry 52
    -0x1.fc0000000000000000000000029aca95p-52,
    -0x1.fc0p-52
  },
  { // Entry 53
    0x1.fc0000000000000000000000029aca95p-52,
    0x1.fc0p-52
  },
  { // Entry 54
    -0x1.fdc93ea04e6030021bf3b7f1b7274addp-5,
    -0x1.fd210af77856cp-5
  },
  { // Entry 55
    0x1.fdc93ea04e6030021bf3b7f1b7274addp-5,
    0x1.fd210af77856cp-5
  },
  { // Entry 56
    -0x1.ffeaa91115a4e8716dc9f09be20a9364p-7,
    -0x1.ffep-7
  },
  { // Entry 57
    0x1.ffeaa91115a4e8716dc9f09be20a9364p-7,
    0x1.ffep-7
  },
  { // Entry 58
    -0x1.9a775e687850d877587114f931f61369p3,
    -0x1.ffffffffe03edp-1
  },
  { // Entry 59
    0x1.9a775e687850d877587114f931f61369p3,
    0x1.ffffffffe03edp-1
  },
  { // Entry 60
    -0x1.9ba863fb6bf8e791c8099e55cff570c3p3,
    -0x1.ffffffffe2863p-1
  },
  { // Entry 61
    0x1.9ba863fb6bf8e791c8099e55cff570c3p3,
    0x1.ffffffffe2863p-1
  },
  { // Entry 62
    -0x1.f369d8eedfbb384b0ee31be424423ec2p3,
    -0x1.ffffffffffe0bp-1
  },
  { // Entry 63
    0x1.f369d8eedfbb384b0ee31be424423ec2p3,
    0x1.ffffffffffe0bp-1
  },
  { // Entry 64
    -0x1.02bd22bd19797815b1ddefc90c41f8fbp4,
    -0x1.fffffffffff5ep-1
  },
  { // Entry 65
    0x1.02bd22bd19797815b1ddefc90c41f8fbp4,
    0x1.fffffffffff5ep-1
  },
  { // Entry 66
    -0x1.1841a4bab2d6d03a28537f43de9e90a4p4,
    -0x1.ffffffffffff5p-1
  },
  { // Entry 67
    0x1.1841a4bab2d6d03a28537f43de9e90a4p4,
    0x1.ffffffffffff5p-1
  },
  { // Entry 68
    0x1.p-99,
    0x1.0p-99
  },
  { // Entry 69
    -0x1.p-99,
    -0x1.0p-99
  },
  { // Entry 70
    0x1.p-1074,
    0x1.0p-1074
  },
  { // Entry 71
    -0x1.p-1074,
    -0x1.0p-1074
  },
  { // Entry 72
    0x1.00000000000010000000155555555555p-41,
    0x1.0000000000001p-41
  },
  { // Entry 73
    -0x1.00000000000010000000155555555555p-41,
    -0x1.0000000000001p-41
  },
  { // Entry 74
    0x1.015891c9eaefd81f5edb9583f3871889p-3,
    0x1.0000000000006p-3
  },
  { // Entry 75
    -0x1.015891c9eaefd81f5edb9583f3871889p-3,
    -0x1.0000000000006p-3
  },
  { // Entry 76
    0x1.193ea7aad0313ecbf96ee2aa7057ee7cp-1,
    0x1.0000000000007p-1
  },
  { // Entry 77
    -0x1.193ea7aad0313ecbf96ee2aa7057ee7cp-1,
    -0x1.0000000000007p-1
  },
  { // Entry 78
    0x1.000555888ad4c9be103a862dcf933db6p-6,
    0x1.0000000000030p-6
  },
  { // Entry 79
    -0x1.000555888ad4c9be103a862dcf933db6p-6,
    -0x1.0000000000030p-6
  },
  { // Entry 80
    0x1.0000055555a8988cd2ad575377ece0d7p-10,
    0x1.0000000000201p-10
  },
  { // Entry 81
    -0x1.0000055555a8988cd2ad575377ece0d7p-10,
    -0x1.0000000000201p-10
  },
  { // Entry 82
    0x1.015891c9f107d81f5edc2a1f85d03e57p-3,
    0x1.00000000060p-3
  },
  { // Entry 83
    -0x1.015891c9f107d81f5edc2a1f85d03e57p-3,
    -0x1.00000000060p-3
  },
  { // Entry 84
    0x1.005588ad597cefed3539549b86ce2b1bp-4,
    0x1.00000000220p-4
  },
  { // Entry 85
    -0x1.005588ad597cefed3539549b86ce2b1bp-4,
    -0x1.00000000220p-4
  },
  { // Entry 86
    0x1.0000055577888aaad1cf378dd5b4caa3p-10,
    0x1.00000000220p-10
  },
  { // Entry 87
    -0x1.0000055577888aaad1cf378dd5b4caa3p-10,
    -0x1.00000000220p-10
  },
  { // Entry 88
    0x1.193ea7fca06d7000000fd6e3e45036c3p-1,
    0x1.0000003d5c2d9p-1
  },
  { // Entry 89
    -0x1.193ea7fca06d7000000fd6e3e45036c3p-1,
    -0x1.0000003d5c2d9p-1
  },
  { // Entry 90
    0x1.0158920aeeffb7df62fc5f72d05afc8ap-3,
    0x1.0000004p-3
  },
  { // Entry 91
    -0x1.0158920aeeffb7df62fc5f72d05afc8ap-3,
    -0x1.0000004p-3
  },
  { // Entry 92
    0x1.015894d61bb2a7f0ee6532fa66c13887p-3,
    0x1.0000030p-3
  },
  { // Entry 93
    -0x1.015894d61bb2a7f0ee6532fa66c13887p-3,
    -0x1.0000030p-3
  },
  { // Entry 94
    0x1.00255c8a5b4f98134613c6ae100b17d2p-5,
    0x1.001p-5
  },
  { // Entry 95
    -0x1.00255c8a5b4f98134613c6ae100b17d2p-5,
    -0x1.001p-5
  },
  { // Entry 96
    0x1.199403c895f3b2fbd6e04ef63e5e0b99p-1,
    0x1.003fffcp-1
  },
  { // Entry 97
    -0x1.199403c895f3b2fbd6e04ef63e5e0b99p-1,
    -0x1.003fffcp-1
  },
  { // Entry 98
    0x1.00401004000480000000000005595756p-50,
    0x1.0040100400048p-50
  },
  { // Entry 99
    -0x1.00401004000480000000000005595756p-50,
    -0x1.0040100400048p-50
  },
  { // Entry 100
    0x1.00b5e931e4c3080916948fa283902fa8p-4,
    0x1.006p-4
  },
  { // Entry 101
    -0x1.00b5e931e4c3080916948fa283902fa8p-4,
    -0x1.006p-4
  },
  { // Entry 102
    0x1.021c8577650fa41d24281561edcb1273p-10,
    0x1.021c8p-10
  },
  { // Entry 103
    -0x1.021c8577650fa41d24281561edcb1273p-10,
    -0x1.021c8p-10
  },
  { // Entry 104
    0x1.03858e51088d27f9df83774139563f2cp-6,
    0x1.038p-6
  },
  { // Entry 105
    -0x1.03858e51088d27f9df83774139563f2cp-6,
    -0x1.038p-6
  },
  { // Entry 106
    0x1.1e9b2fd18d91b42e390d13e9beb6978cp-1,
    0x1.040p-1
  },
  { // Entry 107
    -0x1.1e9b2fd18d91b42e390d13e9beb6978cp-1,
    -0x1.040p-1
  },
  { // Entry 108
    0x1.0841776c420d4707689f45329a9bf2cep-7,
    0x1.084p-7
  },
  { // Entry 109
    -0x1.0841776c420d4707689f45329a9bf2cep-7,
    -0x1.084p-7
  },
  { // Entry 110
    0x1.37ed416dfaf6747b307bee589157fe93p-1,
    0x1.1650efedb9eb2p-1
  },
  { // Entry 111
    -0x1.37ed416dfaf6747b307bee589157fe93p-1,
    -0x1.1650efedb9eb2p-1
  },
  { // Entry 112
    0x1.45e1141a8c00e0b0eb767eb3382f20b8p-1,
    0x1.1ffffffffffffp-1
  },
  { // Entry 113
    -0x1.45e1141a8c00e0b0eb767eb3382f20b8p-1,
    -0x1.1ffffffffffffp-1
  },
  { // Entry 114
    0x1.45e1141a8c00f818c85ab35ce89683a2p-1,
    0x1.2p-1
  },
  { // Entry 115
    -0x1.45e1141a8c00f818c85ab35ce89683a2p-1,
    -0x1.2p-1
  },
  { // Entry 116
    0x1.2e223119d32f870a129b78a196ee4c8dp-7,
    0x1.2e2p-7
  },
  { // Entry 117
    -0x1.2e223119d32f870a129b78a196ee4c8dp-7,
    -0x1.2e2p-7
  },
  { // Entry 118
    0x1.600c9c6f70efcd85cd16189ee688ead0p-1,
    0x1.316p-1
  },
  { // Entry 119
    -0x1.600c9c6f70efcd85cd16189ee688ead0p-1,
    -0x1.316p-1
  },
  { // Entry 120
    0x1.3b5afc2b8cfd87f655c91414c5969d60p-2,
    0x1.31cp-2
  },
  { // Entry 121
    -0x1.3b5afc2b8cfd87f655c91414c5969d60p-2,
    -0x1.31cp-2
  },
  { // Entry 122
    0x1.62e4307128100800001f7881babc44f3p-1,
    0x1.3333338617529p-1
  },
  { // Entry 123
    -0x1.62e4307128100800001f7881babc44f3p-1,
    -0x1.3333338617529p-1
  },
  { // Entry 124
    0x1.33aef545bb20968537b09375e6d5c60ap-7,
    0x1.33aca4ae2b081p-7
  },
  { // Entry 125
    -0x1.33aef545bb20968537b09375e6d5c60ap-7,
    -0x1.33aca4ae2b081p-7
  },
  { // Entry 126
    0x1.9c5cfbb889a7419fe7705e893b99fbb5p-1,
    0x1.5586ad8669418p-1
  },
  { // Entry 127
    -0x1.9c5cfbb889a7419fe7705e893b99fbb5p-1,
    -0x1.5586ad8669418p-1
  },
  { // Entry 128
    0x1.9d5e0765d3182e417e4d91808f30b95fp-1,
    0x1.56152a51dda72p-1
  },
  { // Entry 129
    -0x1.9d5e0765d3182e417e4d91808f30b95fp-1,
    -0x1.56152a51dda72p-1
  },
  { // Entry 130
    0x1.9d783af9f97bce33bd221a9954befb0cp-1,
    0x1.5623ab271fa52p-1
  },
  { // Entry 131
    -0x1.9d783af9f97bce33bd221a9954befb0cp-1,
    -0x1.5623ab271fa52p-1
  },
  { // Entry 132
    0x1.56a0f0b4476de80270a6332ff4450533p-5,
    0x1.566dd4892fab9p-5
  },
  { // Entry 133
    -0x1.56a0f0b4476de80270a6332ff4450533p-5,
    -0x1.566dd4892fab9p-5
  },
  { // Entry 134
    0x1.5db43aa0e3e55fffa5ad9886e8f22cb8p-3,
    0x1.5a582cdc4e9d4p-3
  },
  { // Entry 135
    -0x1.5db43aa0e3e55fffa5ad9886e8f22cb8p-3,
    -0x1.5a582cdc4e9d4p-3
  },
  { // Entry 136
    0x1.ab9dfa0ec89b8247c03f70d6fccdfd66p-1,
    0x1.5dd34e7af8d61p-1
  },
  { // Entry 137
    -0x1.ab9dfa0ec89b8247c03f70d6fccdfd66p-1,
    -0x1.5dd34e7af8d61p-1
  },
  { // Entry 138
    0x1.67a648e5b16c6a6999d9665a8c288d27p-8,
    0x1.67a55c49aa5d6p-8
  },
  { // Entry 139
    -0x1.67a648e5b16c6a6999d9665a8c288d27p-8,
    -0x1.67a55c49aa5d6p-8
  },
  { // Entry 140
    0x1.7b57ee7bea57a7fcebaaea6f557706c2p-2,
    0x1.6ae491f70c7cbp-2
  },
  { // Entry 141
    -0x1.7b57ee7bea57a7fcebaaea6f557706c2p-2,
    -0x1.6ae491f70c7cbp-2
  },
  { // Entry 142
    0x1.7222b50fd4f8ce0954e89313933bded3p-3,
    0x1.6e2856e2856f5p-3
  },
  { // Entry 143
    -0x1.7222b50fd4f8ce0954e89313933bded3p-3,
    -0x1.6e2856e2856f5p-3
  },
  { // Entry 144
    0x1.cf6347191f5b5aba22dc8400fa882ceep-1,
    0x1.7p-1
  },
  { // Entry 145
    -0x1.cf6347191f5b5aba22dc8400fa882ceep-1,
    -0x1.7p-1
  },
  { // Entry 146
    0x1.83916f868284f882ad9463d174a59d97p-2,
    0x1.721060c1a73cep-2
  },
  { // Entry 147
    -0x1.83916f868284f882ad9463d174a59d97p-2,
    -0x1.721060c1a73cep-2
  },
  { // Entry 148
    0x1.85e0806e8e13b7fcc08479529c8104e6p-2,
    0x1.7411d463bfe90p-2
  },
  { // Entry 149
    -0x1.85e0806e8e13b7fcc08479529c8104e6p-2,
    -0x1.7411d463bfe90p-2
  },
  { // Entry 150
    0x1.f2272af46bbe08000012b87d08e7932fp-1,
    0x1.800000078eaacp-1
  },
  { // Entry 151
    -0x1.f2272af46bbe08000012b87d08e7932fp-1,
    -0x1.800000078eaacp-1
  },
  { // Entry 152
    0x1.f2272af46bbf0800001fad0fd766e8cfp-1,
    0x1.800000078eab3p-1
  },
  { // Entry 153
    -0x1.f2272af46bbf0800001fad0fd766e8cfp-1,
    -0x1.800000078eab3p-1
  },
  { // Entry 154
    0x1.83e4a353f34f3562d9d23f45dc8b2e29p-7,
    0x1.83ep-7
  },
  { // Entry 155
    -0x1.83e4a353f34f3562d9d23f45dc8b2e29p-7,
    -0x1.83ep-7
  },
  { // Entry 156
    0x1.89b541d1b39fa30a054d69c38ffbdb5ep-4,
    0x1.888p-4
  },
  { // Entry 157
    -0x1.89b541d1b39fa30a054d69c38ffbdb5ep-4,
    -0x1.888p-4
  },
  { // Entry 158
    0x1.8a08c32ee13cd9422b9ad12f398f50bbp-8,
    0x1.8a078c03f8dcep-8
  },
  { // Entry 159
    -0x1.8a08c32ee13cd9422b9ad12f398f50bbp-8,
    -0x1.8a078c03f8dcep-8
  },
  { // Entry 160
    0x1.946669a6bba909c4bc5da852e75a3d66p-8,
    0x1.9465194651941p-8
  },
  { // Entry 161
    -0x1.946669a6bba909c4bc5da852e75a3d66p-8,
    -0x1.9465194651941p-8
  },
  { // Entry 162
    0x1.9c7d184ac6505eee21ace6732a52730cp-3,
    0x1.970p-3
  },
  { // Entry 163
    -0x1.9c7d184ac6505eee21ace6732a52730cp-3,
    -0x1.970p-3
  },
  { // Entry 164
    0x1.98da3c40000e9801ec829a13899425ecp-4,
    0x1.978p-4
  },
  { // Entry 165
    -0x1.98da3c40000e9801ec829a13899425ecp-4,
    -0x1.978p-4
  },
  { // Entry 166
    0x1.9af93cdc56240000000fff41a04220ffp-4,
    0x1.999999a3a18c2p-4
  },
  { // Entry 167
    -0x1.9af93cdc56240000000fff41a04220ffp-4,
    -0x1.999999a3a18c2p-4
  },
  { // Entry 168
    0x1.9af93cdc566f000000197f297a13895cp-4,
    0x1.999999a3a1d66p-4
  },
  { // Entry 169
    -0x1.9af93cdc566f000000197f297a13895cp-4,
    -0x1.999999a3a1d66p-4
  },
  { // Entry 170
    0x1.193ea7fa8d771fffffe6de660aab4045p0,
    0x1.999999d303287p-1
  },
  { // Entry 171
    -0x1.193ea7fa8d771fffffe6de660aab4045p0,
    -0x1.999999d303287p-1
  },
  { // Entry 172
    0x1.9af93d0c9ef7d80000017b97c0c0930cp-4,
    0x1.999999d36ec44p-4
  },
  { // Entry 173
    -0x1.9af93d0c9ef7d80000017b97c0c0930cp-4,
    -0x1.999999d36ec44p-4
  },
  { // Entry 174
    0x1.9f323f7638726800001eda3701a5c338p-3,
    0x1.99999a3d09361p-3
  },
  { // Entry 175
    -0x1.9f323f7638726800001eda3701a5c338p-3,
    -0x1.99999a3d09361p-3
  },
  { // Entry 176
    0x1.9f323fd47175b7ffffe05fbf960efc6ap-3,
    0x1.99999a977d623p-3
  },
  { // Entry 177
    -0x1.9f323fd47175b7ffffe05fbf960efc6ap-3,
    -0x1.99999a977d623p-3
  },
  { // Entry 178
    0x1.9f323fe10c9a1800001efbbe0bb48ebbp-3,
    0x1.99999aa39770ap-3
  },
  { // Entry 179
    -0x1.9f323fe10c9a1800001efbbe0bb48ebbp-3,
    -0x1.99999aa39770ap-3
  },
  { // Entry 180
    0x1.9a179be1e7a6e801a0cbc1770ccc0691p-5,
    0x1.99cp-5
  },
  { // Entry 181
    -0x1.9a179be1e7a6e801a0cbc1770ccc0691p-5,
    -0x1.99cp-5
  },
  { // Entry 182
    0x1.a5256971dc6e440698f25410f9a508ffp-10,
    0x1.a52551b31353cp-10
  },
  { // Entry 183
    -0x1.a5256971dc6e440698f25410f9a508ffp-10,
    -0x1.a52551b31353cp-10
  },
  { // Entry 184
    0x1.ad6df00c82cd92c93177514dd245567bp-24,
    0x1.ad6df00c82cc0p-24
  },
  { // Entry 185
    -0x1.ad6df00c82cd92c93177514dd245567bp-24,
    -0x1.ad6df00c82cc0p-24
  },
  { // Entry 186
    0x1.aec648950aa9b6160bf45bf45b2ce0bep-8,
    0x1.aec4b201aa53bp-8
  },
  { // Entry 187
    -0x1.aec648950aa9b6160bf45bf45b2ce0bep-8,
    -0x1.aec4b201aa53bp-8
  },
  { // Entry 188
    0x1.b6cabb35f338f7fb83223470c9fbfb09p-3,
    0x1.b032e138a539dp-3
  },
  { // Entry 189
    -0x1.b6cabb35f338f7fb83223470c9fbfb09p-3,
    -0x1.b032e138a539dp-3
  },
  { // Entry 190
    0x1.d6f4c64bee95d884b07a53fe12d571f4p-2,
    0x1.b85680001c332p-2
  },
  { // Entry 191
    -0x1.d6f4c64bee95d884b07a53fe12d571f4p-2,
    -0x1.b85680001c332p-2
  },
  { // Entry 192
    0x1.c184b5fbed8192fa453d4d9c1577a9e3p-10,
    0x1.c184991bf2fp-10
  },
  { // Entry 193
    -0x1.c184b5fbed8192fa453d4d9c1577a9e3p-10,
    -0x1.c184991bf2fp-10
  },
  { // Entry 194
    0x1.e71d3517d3e01b42d5dae4c3aaf70503p-2,
    0x1.c56b0b96cdf91p-2
  },
  { // Entry 195
    -0x1.e71d3517d3e01b42d5dae4c3aaf70503p-2,
    -0x1.c56b0b96cdf91p-2
  },
  { // Entry 196
    0x1.c5e0000001db8fffed2e2b94fd54870dp-20,
    0x1.c5ep-20
  },
  { // Entry 197
    -0x1.c5e0000001db8fffed2e2b94fd54870dp-20,
    -0x1.c5ep-20
  },
  { // Entry 198
    0x1.f055451fb359e7fffffbe5195d4377e8p-2,
    0x1.ccccccd660083p-2
  },
  { // Entry 199
    -0x1.f055451fb359e7fffffbe5195d4377e8p-2,
    -0x1.ccccccd660083p-2
  },
  { // Entry 200
    0x1.f1c704e1f3c8a800b71131c90e193596p-2,
    0x1.cdf37cdf37cd9p-2
  },
  { // Entry 201
    -0x1.f1c704e1f3c8a800b71131c90e193596p-2,
    -0x1.cdf37cdf37cd9p-2
  },
  { // Entry 202
    0x1.d00a0587151948029cb1fb36b2a24903p-5,
    0x1.cf8b2052bbb11p-5
  },
  { // Entry 203
    -0x1.d00a0587151948029cb1fb36b2a24903p-5,
    -0x1.cf8b2052bbb11p-5
  },
  { // Entry 204
    0x1.f4656a69bea6d733e8f3dfaec12111c3p-2,
    0x1.d008d55f75360p-2
  },
  { // Entry 205
    -0x1.f4656a69bea6d733e8f3dfaec12111c3p-2,
    -0x1.d008d55f75360p-2
  },
  { // Entry 206
    0x1.d0cad6adc9a0c837bbecea984e9019d7p-5,
    0x1.d04b532bd5b41p-5
  },
  { // Entry 207
    -0x1.d0cad6adc9a0c837bbecea984e9019d7p-5,
    -0x1.d04b532bd5b41p-5
  },
  { // Entry 208
    0x1.f62f40794a7b089973231ae614553eb0p-2,
    0x1.d1745d1745d11p-2
  },
  { // Entry 209
    -0x1.f62f40794a7b089973231ae614553eb0p-2,
    -0x1.d1745d1745d11p-2
  },
  { // Entry 210
    0x1.d1c00000008077fe5d003fc8ce63e4a4p-21,
    0x1.d1cp-21
  },
  { // Entry 211
    -0x1.d1c00000008077fe5d003fc8ce63e4a4p-21,
    -0x1.d1cp-21
  },
  { // Entry 212
    0x1.f6beddb6ec29b749a9e4a3f67a36b414p-2,
    0x1.d1e646f156570p-2
  },
  { // Entry 213
    -0x1.f6beddb6ec29b749a9e4a3f67a36b414p-2,
    -0x1.d1e646f156570p-2
  },
  { // Entry 214
    0x1.fabc7c84166033eb57a453fd83585dc8p-2,
    0x1.d50efa205a174p-2
  },
  { // Entry 215
    -0x1.fabc7c84166033eb57a453fd83585dc8p-2,
    -0x1.d50efa205a174p-2
  },
  { // Entry 216
    0x1.d62f43b4c2c737fdd232cf2e299076f7p-11,
    0x1.d62f3b71fca8cp-11
  },
  { // Entry 217
    -0x1.d62f43b4c2c737fdd232cf2e299076f7p-11,
    -0x1.d62f3b71fca8cp-11
  },
  { // Entry 218
    0x1.e3a4b468f251480a6049e3fe17b89646p-5,
    0x1.e3150daedb476p-5
  },
  { // Entry 219
    -0x1.e3a4b468f251480a6049e3fe17b89646p-5,
    -0x1.e3150daedb476p-5
  },
  { // Entry 220
    0x1.e68e0c2de6d2280c8a117c4d61d8f42fp-5,
    0x1.e5fbc9eecbdaep-5
  },
  { // Entry 221
    -0x1.e68e0c2de6d2280c8a117c4d61d8f42fp-5,
    -0x1.e5fbc9eecbdaep-5
  },
  { // Entry 222
    0x1.e9de86e8fd3be801a9f830844ba5e501p-5,
    0x1.e9494303cd80fp-5
  },
  { // Entry 223
    -0x1.e9de86e8fd3be801a9f830844ba5e501p-5,
    -0x1.e9494303cd80fp-5
  },
  { // Entry 224
    0x1.edbcc82a00a4c001e7ac01891849800ep-5,
    0x1.ed23f4c89da70p-5
  },
  { // Entry 225
    -0x1.edbcc82a00a4c001e7ac01891849800ep-5,
    -0x1.ed23f4c89da70p-5
  },
  { // Entry 226
    0x1.fa0dc9d7131fee2b38ba993a65f82a06p-3,
    0x1.effffffffffffp-3
  },
  { // Entry 227
    -0x1.fa0dc9d7131fee2b38ba993a65f82a06p-3,
    -0x1.effffffffffffp-3
  },
  { // Entry 228
    0x1.f37429af961a9824754b77a1b593d39ap-4,
    0x1.f0fe3530f7239p-4
  },
  { // Entry 229
    -0x1.f37429af961a9824754b77a1b593d39ap-4,
    -0x1.f0fe3530f7239p-4
  },
  { // Entry 230
    0x1.f37429af961ab89edde6f4ae74375a06p-4,
    0x1.f0fe3530f723bp-4
  },
  { // Entry 231
    -0x1.f37429af961ab89edde6f4ae74375a06p-4,
    -0x1.f0fe3530f723bp-4
  },
  { // Entry 232
    0x1.f37429af961ac8dc1234b334d38f1d1bp-4,
    0x1.f0fe3530f723cp-4
  },
  { // Entry 233
    -0x1.f37429af961ac8dc1234b334d38f1d1bp-4,
    -0x1.f0fe3530f723cp-4
  },
  { // Entry 234
    0x1.f37429af962b268ac88eb6a2f4026151p-4,
    0x1.f0fe3530f733ep-4
  },
  { // Entry 235
    -0x1.f37429af962b268ac88eb6a2f4026151p-4,
    -0x1.f0fe3530f733ep-4
  },
  { // Entry 236
    0x1.f1e9c43b21348857c7e465e46799dce1p-5,
    0x1.f14d08c7109aap-5
  },
  { // Entry 237
    -0x1.f1e9c43b21348857c7e465e46799dce1p-5,
    -0x1.f14d08c7109aap-5
  },
  { // Entry 238
    0x1.f90b42375a486a39cdf9b2ccf2824fecp-4,
    0x1.f68p-4
  },
  { // Entry 239
    -0x1.f90b42375a486a39cdf9b2ccf2824fecp-4,
    -0x1.f68p-4
  },
  { // Entry 240
    0x1.f72a153ff7688808c896dd6ffe6516d4p-5,
    0x1.f688582bdf450p-5
  },
  { // Entry 241
    -0x1.f72a153ff7688808c896dd6ffe6516d4p-5,
    -0x1.f688582bdf450p-5
  },
  { // Entry 242
    0x1.f7e703f1db06e802f9321fd5e2394e07p-5,
    0x1.f744909706414p-5
  },
  { // Entry 243
    -0x1.f7e703f1db06e802f9321fd5e2394e07p-5,
    -0x1.f744909706414p-5
  },
  { // Entry 244
    0x1.340af764783edfffac199b0ebf01c362p1,
    0x1.f7cp-1
  },
  { // Entry 245
    -0x1.340af764783edfffac199b0ebf01c362p1,
    -0x1.f7cp-1
  },
  { // Entry 246
    0x1.fa24a006fb7277fe99107e535f1488d7p-5,
    0x1.f98p-5
  },
  { // Entry 247
    -0x1.fa24a006fb7277fe99107e535f1488d7p-5,
    -0x1.f98p-5
  },
  { // Entry 248
    0x1.fabe9384d8eb28030d5306c1d38ffe3cp-5,
    0x1.fa195d3f2824ap-5
  },
  { // Entry 249
    -0x1.fabe9384d8eb28030d5306c1d38ffe3cp-5,
    -0x1.fa195d3f2824ap-5
  },
  { // Entry 250
    0x1.fac53cc7f51a2825f03615ff2011a3f4p-5,
    0x1.fa2p-5
  },
  { // Entry 251
    -0x1.fac53cc7f51a2825f03615ff2011a3f4p-5,
    -0x1.fa2p-5
  },
  { // Entry 252
    0x1.fca715610d4c584a721b2c19e6223c63p-5,
    0x1.fbfffffffffffp-5
  },
  { // Entry 253
    -0x1.fca715610d4c584a721b2c19e6223c63p-5,
    -0x1.fbfffffffffffp-5
  },
  { // Entry 254
    0x1.ff2303e94a6fa776b99fdcdac342443dp-4,
    0x1.fc7ffffffffffp-4
  },
  { // Entry 255
    -0x1.ff2303e94a6fa776b99fdcdac342443dp-4,
    -0x1.fc7ffffffffffp-4
  },
  { // Entry 256
    0x1.fcca7762322a195ec28591033e93e55bp-7,
    0x1.fccp-7
  },
  { // Entry 257
    -0x1.fcca7762322a195ec28591033e93e55bp-7,
    -0x1.fccp-7
  },
  { // Entry 258
    0x1.fde34e5e71112802cd5dc4e1fb2d0640p-5,
    0x1.fd3b00ef28dc9p-5
  },
  { // Entry 259
    -0x1.fde34e5e71112802cd5dc4e1fb2d0640p-5,
    -0x1.fd3b00ef28dc9p-5
  },
  { // Entry 260
    0x1.fd5da51e6bee9b019e62cd796699df5cp-8,
    0x1.fd5b04f37a8adp-8
  },
  { // Entry 261
    -0x1.fd5da51e6bee9b019e62cd796699df5cp-8,
    -0x1.fd5b04f37a8adp-8
  },
  { // Entry 262
    0x1.ff08b9b3981768022e66dd42cd419f4cp-9,
    0x1.ff080ffffffffp-9
  },
  { // Entry 263
    -0x1.ff08b9b3981768022e66dd42cd419f4cp-9,
    -0x1.ff080ffffffffp-9
  },
  { // Entry 264
    0x1.ff782a88ba0fba00f908e16f051a1810p-10,
    0x1.ff77fffffffffp-10
  },
  { // Entry 265
    -0x1.ff782a88ba0fba00f908e16f051a1810p-10,
    -0x1.ff77fffffffffp-10
  },
  { // Entry 266
    0x1.0154818928eb11132905352501826af5p-3,
    0x1.fff7fffffffffp-4
  },
  { // Entry 267
    -0x1.0154818928eb11132905352501826af5p-3,
    -0x1.fff7fffffffffp-4
  },
  { // Entry 268
    0x1.fffca6a70d15564482dc93a139764e2dp-9,
    0x1.fffbfbfffffffp-9
  },
  { // Entry 269
    -0x1.fffca6a70d15564482dc93a139764e2dp-9,
    -0x1.fffbfbfffffffp-9
  },
  { // Entry 270
    0x1.0157cebdbc7ecff56d936def2dc90848p-3,
    0x1.fffe7ffffffffp-4
  },
  { // Entry 271
    -0x1.0157cebdbc7ecff56d936def2dc90848p-3,
    -0x1.fffe7ffffffffp-4
  },
  { // Entry 272
    0x1.ffff2aaa70e11229a646c3ea214d5c6bp-10,
    0x1.fffeffffffcffp-10
  },
  { // Entry 273
    -0x1.ffff2aaa70e11229a646c3ea214d5c6bp-10,
    -0x1.fffeffffffcffp-10
  },
  { // Entry 274
    0x1.fe849ae4ae0948fc35cd560fe0f7a64fp2,
    0x1.fffff8170432cp-1
  },
  { // Entry 275
    -0x1.fe849ae4ae0948fc35cd560fe0f7a64fp2,
    -0x1.fffff8170432cp-1
  },
  { // Entry 276
    0x1.fe8636119def0727f0b21ad8da17b705p2,
    0x1.fffff81769d3bp-1
  },
  { // Entry 277
    -0x1.fe8636119def0727f0b21ad8da17b705p2,
    -0x1.fffff81769d3bp-1
  },
  { // Entry 278
    0x1.feab0f8d089237326f5246ce7822ddcap2,
    0x1.fffff8207ffffp-1
  },
  { // Entry 279
    -0x1.feab0f8d089237326f5246ce7822ddcap2,
    -0x1.fffff8207ffffp-1
  },
  { // Entry 280
    0x1.fffffbc002aa9a99aab134f0ccf89dcfp-20,
    0x1.fffffbbffffffp-20
  },
  { // Entry 281
    -0x1.fffffbc002aa9a99aab134f0ccf89dcfp-20,
    -0x1.fffffbbffffffp-20
  },
  { // Entry 282
    0x1.38aa9bbc81de80372da066273e181f3dp3,
    0x1.ffffffc7fffffp-1
  },
  { // Entry 283
    -0x1.38aa9bbc81de80372da066273e181f3dp3,
    -0x1.ffffffc7fffffp-1
  },
  { // Entry 284
    0x1.a791d873bcf1ef6cc589b55be94c11ccp3,
    0x1.fffffffff1fffp-1
  },
  { // Entry 285
    -0x1.a791d873bcf1ef6cc589b55be94c11ccp3,
    -0x1.fffffffff1fffp-1
  },
  { // Entry 286
    0x1.048fa31ec6a076cfd26abb456c9e863ap4,
    0x1.fffffffffff7fp-1
  },
  { // Entry 287
    -0x1.048fa31ec6a076cfd26abb456c9e863ap4,
    -0x1.fffffffffff7fp-1
  },
  { // Entry 288
    0x1.ffffffffffff8aaaaaaaaaa9f1111111p-25,
    0x1.fffffffffffeep-25
  },
  { // Entry 289
    -0x1.ffffffffffff8aaaaaaaaaa9f1111111p-25,
    -0x1.fffffffffffeep-25
  },
  { // Entry 290
    -0x1.31dd28c89d64f3513ea98f014ae7630cp1,
    -0x1.f777777777777p-1
  },
  { // Entry 291
    0x1.31dd28c89d64f3513ea98f014ae7630cp1,
    0x1.f777777777777p-1
  },
  { // Entry 292
    -0x1.04f65f9c7297527749382883b8e88e33p1,
    -0x1.eeeeeeeeeeeeep-1
  },
  { // Entry 293
    0x1.04f65f9c7297527749382883b8e88e33p1,
    0x1.eeeeeeeeeeeeep-1
  },
  { // Entry 294
    -0x1.d4ef968880dcf1c48bf6d707008e71a0p0,
    -0x1.e666666666665p-1
  },
  { // Entry 295
    0x1.d4ef968880dcf1c48bf6d707008e71a0p0,
    0x1.e666666666665p-1
  },
  { // Entry 296
    -0x1.af038cbcdfe177f9b97cb13acb6a1d56p0,
    -0x1.ddddddddddddcp-1
  },
  { // Entry 297
    0x1.af038cbcdfe177f9b97cb13acb6a1d56p0,
    0x1.ddddddddddddcp-1
  },
  { // Entry 298
    -0x1.9157dfdd1b3e8bb2bc8c94b692c36c8bp0,
    -0x1.d555555555553p-1
  },
  { // Entry 299
    0x1.9157dfdd1b3e8bb2bc8c94b692c36c8bp0,
    0x1.d555555555553p-1
  },
  { // Entry 300
    -0x1.78e360604b32513afa302dd9090f54afp0,
    -0x1.ccccccccccccap-1
  },
  { // Entry 301
    0x1.78e360604b32513afa302dd9090f54afp0,
    0x1.ccccccccccccap-1
  },
  { // Entry 302
    -0x1.640775d4dd98457b36fb7ce98ec43308p0,
    -0x1.c444444444441p-1
  },
  { // Entry 303
    0x1.640775d4dd98457b36fb7ce98ec43308p0,
    0x1.c444444444441p-1
  },
  { // Entry 304
    -0x1.51cca16d7bb9ff79603c2533c5c76b7ap0,
    -0x1.bbbbbbbbbbbb8p-1
  },
  { // Entry 305
    0x1.51cca16d7bb9ff79603c2533c5c76b7ap0,
    0x1.bbbbbbbbbbbb8p-1
  },
  { // Entry 306
    -0x1.41933b0e446305a96ace1bc262cdee99p0,
    -0x1.b33333333332fp-1
  },
  { // Entry 307
    0x1.41933b0e446305a96ace1bc262cdee99p0,
    0x1.b33333333332fp-1
  },
  { // Entry 308
    -0x1.32ee3b77f374414d3a29141080dfabeap0,
    -0x1.aaaaaaaaaaaa6p-1
  },
  { // Entry 309
    0x1.32ee3b77f374414d3a29141080dfabeap0,
    0x1.aaaaaaaaaaaa6p-1
  },
  { // Entry 310
    -0x1.258fdae8372b9231a664ea76c9d6586fp0,
    -0x1.a22222222221dp-1
  },
  { // Entry 311
    0x1.258fdae8372b9231a664ea76c9d6586fp0,
    0x1.a22222222221dp-1
  },
  { // Entry 312
    -0x1.193ea7aad0302d04dcfd1b8e192ed85dp0,
    -0x1.9999999999994p-1
  },
  { // Entry 313
    0x1.193ea7aad0302d04dcfd1b8e192ed85dp0,
    0x1.9999999999994p-1
  },
  { // Entry 314
    -0x1.0dcefea4d025e0b8d09052e46fdf4f2ep0,
    -0x1.911111111110bp-1
  },
  { // Entry 315
    0x1.0dcefea4d025e0b8d09052e46fdf4f2ep0,
    0x1.911111111110bp-1
  },
  { // Entry 316
    -0x1.031ef11090f771d990e41e47d30913d6p0,
    -0x1.8888888888882p-1
  },
  { // Entry 317
    0x1.031ef11090f771d990e41e47d30913d6p0,
    0x1.8888888888882p-1
  },
  { // Entry 318
    -0x1.f2272ae325a47546f69496cf861be046p-1,
    -0x1.7fffffffffff9p-1
  },
  { // Entry 319
    0x1.f2272ae325a47546f69496cf861be046p-1,
    0x1.7fffffffffff9p-1
  },
  { // Entry 320
    -0x1.df2e6d6e5fb9a3aede73b55578f55672p-1,
    -0x1.7777777777770p-1
  },
  { // Entry 321
    0x1.df2e6d6e5fb9a3aede73b55578f55672p-1,
    0x1.7777777777770p-1
  },
  { // Entry 322
    -0x1.cd302116f50ababc40c132419e1dab2ap-1,
    -0x1.6eeeeeeeeeee7p-1
  },
  { // Entry 323
    0x1.cd302116f50ababc40c132419e1dab2ap-1,
    0x1.6eeeeeeeeeee7p-1
  },
  { // Entry 324
    -0x1.bc0ed0947fbd88e1ba52723b57950592p-1,
    -0x1.666666666665ep-1
  },
  { // Entry 325
    0x1.bc0ed0947fbd88e1ba52723b57950592p-1,
    0x1.666666666665ep-1
  },
  { // Entry 326
    -0x1.abb1c9065825972aaaf3d164ca1f5323p-1,
    -0x1.5ddddddddddd5p-1
  },
  { // Entry 327
    0x1.abb1c9065825972aaaf3d164ca1f5323p-1,
    0x1.5ddddddddddd5p-1
  },
  { // Entry 328
    -0x1.9c041f7ed8d229e312aad84998a0e008p-1,
    -0x1.555555555554cp-1
  },
  { // Entry 329
    0x1.9c041f7ed8d229e312aad84998a0e008p-1,
    0x1.555555555554cp-1
  },
  { // Entry 330
    -0x1.8cf3f3b791739ba35824b20bb67bc051p-1,
    -0x1.4ccccccccccc3p-1
  },
  { // Entry 331
    0x1.8cf3f3b791739ba35824b20bb67bc051p-1,
    0x1.4ccccccccccc3p-1
  },
  { // Entry 332
    -0x1.7e71ded66460614d3cfeeae5195853fdp-1,
    -0x1.444444444443ap-1
  },
  { // Entry 333
    0x1.7e71ded66460614d3cfeeae5195853fdp-1,
    0x1.444444444443ap-1
  },
  { // Entry 334
    -0x1.7070827f1c7ee9fc23eb09099495f760p-1,
    -0x1.3bbbbbbbbbbb1p-1
  },
  { // Entry 335
    0x1.7070827f1c7ee9fc23eb09099495f760p-1,
    0x1.3bbbbbbbbbbb1p-1
  },
  { // Entry 336
    -0x1.62e42fefa39ddb5793c7673063c5ed5ep-1,
    -0x1.3333333333328p-1
  },
  { // Entry 337
    0x1.62e42fefa39ddb5793c7673063c5ed5ep-1,
    0x1.3333333333328p-1
  },
  { // Entry 338
    -0x1.55c2a141bd913c9da91e9ba97d84ef42p-1,
    -0x1.2aaaaaaaaaa9fp-1
  },
  { // Entry 339
    0x1.55c2a141bd913c9da91e9ba97d84ef42p-1,
    0x1.2aaaaaaaaaa9fp-1
  },
  { // Entry 340
    -0x1.4902c08bec8b8d6ba2debcee67107aa5p-1,
    -0x1.2222222222216p-1
  },
  { // Entry 341
    0x1.4902c08bec8b8d6ba2debcee67107aa5p-1,
    0x1.2222222222216p-1
  },
  { // Entry 342
    -0x1.3c9c79bc8508ca2d5b90a842ee7edfa3p-1,
    -0x1.199999999998dp-1
  },
  { // Entry 343
    0x1.3c9c79bc8508ca2d5b90a842ee7edfa3p-1,
    0x1.199999999998dp-1
  },
  { // Entry 344
    -0x1.308894d710d767af349ad5664f91afbcp-1,
    -0x1.1111111111104p-1
  },
  { // Entry 345
    0x1.308894d710d767af349ad5664f91afbcp-1,
    0x1.1111111111104p-1
  },
  { // Entry 346
    -0x1.24c096cf976a96087c1335628fdc0098p-1,
    -0x1.088888888887bp-1
  },
  { // Entry 347
    0x1.24c096cf976a96087c1335628fdc0098p-1,
    0x1.088888888887bp-1
  },
  { // Entry 348
    -0x1.193ea7aad030a976a4198d55053b7cb5p-1,
    -0x1.0p-1
  },
  { // Entry 349
    0x1.193ea7aad030a976a4198d55053b7cb5p-1,
    0x1.0p-1
  },
  { // Entry 350
    0x1.193ea7aad030a976a4198d55053b7cb5p-1,
    0x1.0p-1
  },
  { // Entry 351
    -0x1.193ea7aad030a976a4198d55053b7cb5p-1,
    -0x1.0p-1
  },
  { // Entry 352
    0x1.24c096cf976bc79a7ab78995d36b066bp-1,
    0x1.0888888888889p-1
  },
  { // Entry 353
    -0x1.24c096cf976bc79a7ab78995d36b066bp-1,
    -0x1.0888888888889p-1
  },
  { // Entry 354
    0x1.308894d710d8a0ba55ff01eb87cbc220p-1,
    0x1.1111111111112p-1
  },
  { // Entry 355
    -0x1.308894d710d8a0ba55ff01eb87cbc220p-1,
    -0x1.1111111111112p-1
  },
  { // Entry 356
    0x1.3c9c79bc850a0b52fa4dacd910d12a32p-1,
    0x1.199999999999bp-1
  },
  { // Entry 357
    -0x1.3c9c79bc850a0b52fa4dacd910d12a32p-1,
    -0x1.199999999999bp-1
  },
  { // Entry 358
    0x1.4902c08bec8cd75f11102da30f1f78d7p-1,
    0x1.2222222222224p-1
  },
  { // Entry 359
    -0x1.4902c08bec8cd75f11102da30f1f78d7p-1,
    -0x1.2222222222224p-1
  },
  { // Entry 360
    0x1.55c2a141bd929027179a90e1bcdc1a2dp-1,
    0x1.2aaaaaaaaaaadp-1
  },
  { // Entry 361
    -0x1.55c2a141bd929027179a90e1bcdc1a2dp-1,
    -0x1.2aaaaaaaaaaadp-1
  },
  { // Entry 362
    0x1.62e42fefa39f395793c767300da3ed5ep-1,
    0x1.3333333333336p-1
  },
  { // Entry 363
    -0x1.62e42fefa39f395793c767300da3ed5ep-1,
    -0x1.3333333333336p-1
  },
  { // Entry 364
    0x1.7070827f1c80536feb7673dd88b946ecp-1,
    0x1.3bbbbbbbbbbbfp-1
  },
  { // Entry 365
    -0x1.7070827f1c80536feb7673dd88b946ecp-1,
    -0x1.3bbbbbbbbbbbfp-1
  },
  { // Entry 366
    0x1.7e71ded66461d753e33ac2ff618644e0p-1,
    0x1.4444444444448p-1
  },
  { // Entry 367
    -0x1.7e71ded66461d753e33ac2ff618644e0p-1,
    -0x1.4444444444448p-1
  },
  { // Entry 368
    0x1.8cf3f3b791751f845062c18f4b0d7fe7p-1,
    0x1.4ccccccccccd1p-1
  },
  { // Entry 369
    -0x1.8cf3f3b791751f845062c18f4b0d7fe7p-1,
    -0x1.4ccccccccccd1p-1
  },
  { // Entry 370
    0x1.9c041f7ed8d3bd1645de0b7c8544b713p-1,
    0x1.555555555555ap-1
  },
  { // Entry 371
    -0x1.9c041f7ed8d3bd1645de0b7c8544b713p-1,
    -0x1.555555555555ap-1
  },
  { // Entry 372
    0x1.abb1c90658273b62b26c47dabd2b16cap-1,
    0x1.5dddddddddde3p-1
  },
  { // Entry 373
    -0x1.abb1c90658273b62b26c47dabd2b16cap-1,
    -0x1.5dddddddddde3p-1
  },
  { // Entry 374
    0x1.bc0ed0947fbf4018f189a9725a0c8214p-1,
    0x1.666666666666cp-1
  },
  { // Entry 375
    -0x1.bc0ed0947fbf4018f189a9725a0c8214p-1,
    -0x1.666666666666cp-1
  },
  { // Entry 376
    0x1.cd302116f50c8745aed84bd751fb575cp-1,
    0x1.6eeeeeeeeeef5p-1
  },
  { // Entry 377
    -0x1.cd302116f50c8745aed84bd751fb575cp-1,
    -0x1.6eeeeeeeeeef5p-1
  },
  { // Entry 378
    0x1.df2e6d6e5fbb884c684c52df3b260c38p-1,
    0x1.777777777777ep-1
  },
  { // Entry 379
    -0x1.df2e6d6e5fbb884c684c52df3b260c38p-1,
    -0x1.777777777777ep-1
  },
  { // Entry 380
    0x1.f2272ae325a67546f69496cf861be046p-1,
    0x1.8000000000007p-1
  },
  { // Entry 381
    -0x1.f2272ae325a67546f69496cf861be046p-1,
    -0x1.8000000000007p-1
  },
  { // Entry 382
    0x1.031ef11090f8818c48703199fec1433ap0,
    0x1.8888888888890p-1
  },
  { // Entry 383
    -0x1.031ef11090f8818c48703199fec1433ap0,
    -0x1.8888888888890p-1
  },
  { // Entry 384
    0x1.0dcefea4d0270295d8d877b36ea1c0e3p0,
    0x1.9111111111119p-1
  },
  { // Entry 385
    -0x1.0dcefea4d0270295d8d877b36ea1c0e3p0,
    -0x1.9111111111119p-1
  },
  { // Entry 386
    0x1.193ea7aad03164214ec438001cc9b599p0,
    0x1.99999999999a2p-1
  },
  { // Entry 387
    -0x1.193ea7aad03164214ec438001cc9b599p0,
    -0x1.99999999999a2p-1
  },
  { // Entry 388
    0x1.258fdae8372ce27963c75835d46b66e6p0,
    0x1.a22222222222bp-1
  },
  { // Entry 389
    -0x1.258fdae8372ce27963c75835d46b66e6p0,
    -0x1.a22222222222bp-1
  },
  { // Entry 390
    0x1.32ee3b77f375afd8dd11ce3f9e4b9287p0,
    0x1.aaaaaaaaaaab4p-1
  },
  { // Entry 391
    -0x1.32ee3b77f375afd8dd11ce3f9e4b9287p0,
    -0x1.aaaaaaaaaaab4p-1
  },
  { // Entry 392
    0x1.41933b0e44649943f09224fce382c799p0,
    0x1.b33333333333dp-1
  },
  { // Entry 393
    -0x1.41933b0e44649943f09224fce382c799p0,
    -0x1.b33333333333dp-1
  },
  { // Entry 394
    0x1.51cca16d7bbbc179603c253505b36b7ap0,
    0x1.bbbbbbbbbbbc6p-1
  },
  { // Entry 395
    -0x1.51cca16d7bbbc179603c253505b36b7ap0,
    -0x1.bbbbbbbbbbbc6p-1
  },
  { // Entry 396
    0x1.640775d4dd9a4337400b58abfdea644fp0,
    0x1.c44444444444fp-1
  },
  { // Entry 397
    -0x1.640775d4dd9a4337400b58abfdea644fp0,
    -0x1.c44444444444fp-1
  },
  { // Entry 398
    0x1.78e360604b349eb43d8e7eb37a3c01b6p0,
    0x1.cccccccccccd8p-1
  },
  { // Entry 399
    -0x1.78e360604b349eb43d8e7eb37a3c01b6p0,
    -0x1.cccccccccccd8p-1
  },
  { // Entry 400
    0x1.9157dfdd1b4148ea63817356fc04c13bp0,
    0x1.d555555555561p-1
  },
  { // Entry 401
    -0x1.9157dfdd1b4148ea63817356fc04c13bp0,
    -0x1.d555555555561p-1
  },
  { // Entry 402
    0x1.af038cbcdfe4dcf0e5a000b57077d005p0,
    0x1.dddddddddddeap-1
  },
  { // Entry 403
    -0x1.af038cbcdfe4dcf0e5a000b57077d005p0,
    -0x1.dddddddddddeap-1
  },
  { // Entry 404
    0x1.d4ef968880e16e7c57738ee1cab27657p0,
    0x1.e666666666673p-1
  },
  { // Entry 405
    -0x1.d4ef968880e16e7c57738ee1cab27657p0,
    -0x1.e666666666673p-1
  },
  { // Entry 406
    0x1.04f65f9c729aa8b4082276b069b6c479p1,
    0x1.eeeeeeeeeeefcp-1
  },
  { // Entry 407
    -0x1.04f65f9c729aa8b4082276b069b6c479p1,
    -0x1.eeeeeeeeeeefcp-1
  },
  { // Entry 408
    0x1.31dd28c89d64f3513ea98f014ae7630cp1,
    0x1.f777777777777p-1
  },
  { // Entry 409
    -0x1.31dd28c89d64f3513ea98f014ae7630cp1,
    -0x1.f777777777777p-1
  },
  { // Entry 410
    -0x1.2b708872320e1d31e4b03f1086a9c047p4,
    -0x1.fffffffffffffp-1
  },
  { // Entry 411
    0x1.2b708872320e1d31e4b03f1086a9c047p4,
    0x1.fffffffffffffp-1
  },
  { // Entry 412
    -0x1.25e4f7b2737fa14486612173c6896892p4,
    -0x1.ffffffffffffep-1
  },
  { // Entry 413
    0x1.25e4f7b2737fa14486612173c6896892p4,
    0x1.ffffffffffffep-1
  },
  { // Entry 414
    -0x1.22a69334db8c97a62f8f72a5de7de462p4,
    -0x1.ffffffffffffdp-1
  },
  { // Entry 415
    0x1.22a69334db8c97a62f8f72a5de7de462p4,
    0x1.ffffffffffffdp-1
  },
  { // Entry 416
    0x1.2b708872320e1d31e4b03f1086a9c047p4,
    0x1.fffffffffffffp-1
  },
  { // Entry 417
    -0x1.2b708872320e1d31e4b03f1086a9c047p4,
    -0x1.fffffffffffffp-1
  },
  { // Entry 418
    0x1.25e4f7b2737fa14486612173c6896892p4,
    0x1.ffffffffffffep-1
  },
  { // Entry 419
    -0x1.25e4f7b2737fa14486612173c6896892p4,
    -0x1.ffffffffffffep-1
  },
  { // Entry 420
    0x1.22a69334db8c97a62f8f72a5de7de462p4,
    0x1.ffffffffffffdp-1
  },
  { // Entry 421
    -0x1.22a69334db8c97a62f8f72a5de7de462p4,
    -0x1.ffffffffffffdp-1
  },
  { // Entry 422
    0x1.4a851baf27b6d549b7c524fbd91644b2p-3,
    0x1.47ae147ae147ap-3
  },
  { // Entry 423
    -0x1.4a851baf27b6d549b7c524fbd91644b2p-3,
    -0x1.47ae147ae147ap-3
  },
  { // Entry 424
    0x1.4a851baf27b6e5b55490996c8137296ap-3,
    0x1.47ae147ae147bp-3
  },
  { // Entry 425
    -0x1.4a851baf27b6e5b55490996c8137296ap-3,
    -0x1.47ae147ae147bp-3
  },
  { // Entry 426
    0x1.4a851baf27b6f620f15c0ddd2962d721p-3,
    0x1.47ae147ae147cp-3
  },
  { // Entry 427
    -0x1.4a851baf27b6f620f15c0ddd2962d721p-3,
    -0x1.47ae147ae147cp-3
  },
  { // Entry 428
    -0x1.4a851baf27b6f620f15c0ddd2962d721p-3,
    -0x1.47ae147ae147cp-3
  },
  { // Entry 429
    0x1.4a851baf27b6f620f15c0ddd2962d721p-3,
    0x1.47ae147ae147cp-3
  },
  { // Entry 430
    -0x1.4a851baf27b6e5b55490996c8137296ap-3,
    -0x1.47ae147ae147bp-3
  },
  { // Entry 431
    0x1.4a851baf27b6e5b55490996c8137296ap-3,
    0x1.47ae147ae147bp-3
  },
  { // Entry 432
    -0x1.4a851baf27b6d549b7c524fbd91644b2p-3,
    -0x1.47ae147ae147ap-3
  },
  { // Entry 433
    0x1.4a851baf27b6d549b7c524fbd91644b2p-3,
    0x1.47ae147ae147ap-3
  },
  { // Entry 434
    0x1.193ea7aad0309ecbf96ee2aa5aad43d2p-1,
    0x1.fffffffffffffp-2
  },
  { // Entry 435
    -0x1.193ea7aad0309ecbf96ee2aa5aad43d2p-1,
    -0x1.fffffffffffffp-2
  },
  { // Entry 436
    0x1.193ea7aad030a976a4198d55053b7cb5p-1,
    0x1.0p-1
  },
  { // Entry 437
    -0x1.193ea7aad030a976a4198d55053b7cb5p-1,
    -0x1.0p-1
  },
  { // Entry 438
    0x1.193ea7aad030becbf96ee2aa5b029927p-1,
    0x1.0000000000001p-1
  },
  { // Entry 439
    -0x1.193ea7aad030becbf96ee2aa5b029927p-1,
    -0x1.0000000000001p-1
  },
  { // Entry 440
    0x1.058aefa8114511e9ee33a6f97bb76f0ap-2,
    0x1.fffffffffffffp-3
  },
  { // Entry 441
    -0x1.058aefa8114511e9ee33a6f97bb76f0ap-2,
    -0x1.fffffffffffffp-3
  },
  { // Entry 442
    0x1.058aefa811451a7276bc2f82043b6a7dp-2,
    0x1.0p-2
  },
  { // Entry 443
    -0x1.058aefa811451a7276bc2f82043b6a7dp-2,
    -0x1.0p-2
  },
  { // Entry 444
    0x1.058aefa811452b8387cd4093155eafe4p-2,
    0x1.0000000000001p-2
  },
  { // Entry 445
    -0x1.058aefa811452b8387cd4093155eafe4p-2,
    -0x1.0000000000001p-2
  },
  { // Entry 446
    0x1.015891c9eaef6e78c471eee9894ceabdp-3,
    0x1.fffffffffffffp-4
  },
  { // Entry 447
    -0x1.015891c9eaef6e78c471eee9894ceabdp-3,
    -0x1.fffffffffffffp-4
  },
  { // Entry 448
    0x1.015891c9eaef7699467a0f6b916c6494p-3,
    0x1.0p-3
  },
  { // Entry 449
    -0x1.015891c9eaef7699467a0f6b916c6494p-3,
    -0x1.0p-3
  },
  { // Entry 450
    0x1.015891c9eaef86da4a8a506fa1b18969p-3,
    0x1.0000000000001p-3
  },
  { // Entry 451
    -0x1.015891c9eaef86da4a8a506fa1b18969p-3,
    -0x1.0000000000001p-3
  },
  { // Entry 452
    0x1.005588ad375ac5c30b0a9d5bbe7d5dd7p-4,
    0x1.fffffffffffffp-5
  },
  { // Entry 453
    -0x1.005588ad375ac5c30b0a9d5bbe7d5dd7p-4,
    -0x1.fffffffffffffp-5
  },
  { // Entry 454
    0x1.005588ad375acdcb1312a563c685255ep-4,
    0x1.0p-4
  },
  { // Entry 455
    -0x1.005588ad375acdcb1312a563c685255ep-4,
    -0x1.0p-4
  },
  { // Entry 456
    0x1.005588ad375adddb2322b573d6963771p-4,
    0x1.0000000000001p-4
  },
  { // Entry 457
    -0x1.005588ad375adddb2322b573d6963771p-4,
    -0x1.0000000000001p-4
  },
  { // Entry 458
    0x1.001558891aee1cb29d53ddbdb46e79d9p-5,
    0x1.fffffffffffffp-6
  },
  { // Entry 459
    -0x1.001558891aee1cb29d53ddbdb46e79d9p-5,
    -0x1.fffffffffffffp-6
  },
  { // Entry 460
    0x1.001558891aee24b49dd3fdc5b66ee9f1p-5,
    0x1.0p-5
  },
  { // Entry 461
    -0x1.001558891aee24b49dd3fdc5b66ee9f1p-5,
    -0x1.0p-5
  },
  { // Entry 462
    0x1.001558891aee34b89ed43dd5ba702a52p-5,
    0x1.0000000000001p-5
  },
  { // Entry 463
    -0x1.001558891aee34b89ed43dd5ba702a52p-5,
    -0x1.0000000000001p-5
  },
  { // Entry 464
    0x1.000555888ad1c18d8d3255aac6d2acadp-6,
    0x1.fffffffffffffp-7
  },
  { // Entry 465
    -0x1.000555888ad1c18d8d3255aac6d2acadp-6,
    -0x1.fffffffffffffp-7
  },
  { // Entry 466
    0x1.000555888ad1c98e0d3a562aced328b5p-6,
    0x1.0p-6
  },
  { // Entry 467
    -0x1.000555888ad1c98e0d3a562aced328b5p-6,
    -0x1.0p-6
  },
  { // Entry 468
    0x1.000555888ad1d98f0d4a572aded438c7p-6,
    0x1.0000000000001p-6
  },
  { // Entry 469
    -0x1.000555888ad1d98f0d4a572aded438c7p-6,
    -0x1.0000000000001p-6
  },
  { // Entry 470
    0x1.000155588891a53723d0cfc25d992fd2p-7,
    0x1.fffffffffffffp-8
  },
  { // Entry 471
    -0x1.000155588891a53723d0cfc25d992fd2p-7,
    -0x1.fffffffffffffp-8
  },
  { // Entry 472
    0x1.000155588891ad3743d14fc45da12ef2p-7,
    0x1.0p-7
  },
  { // Entry 473
    -0x1.000155588891ad3743d14fc45da12ef2p-7,
    -0x1.0p-7
  },
  { // Entry 474
    0x1.000155588891bd3783d24fc85db13332p-7,
    0x1.0000000000001p-7
  },
  { // Entry 475
    -0x1.000155588891bd3783d24fc85db13332p-7,
    -0x1.0000000000001p-7
  },
  { // Entry 476
    0x1.000055558888a51ae61ef133fc078f9ap-8,
    0x1.fffffffffffffp-9
  },
  { // Entry 477
    -0x1.000055558888a51ae61ef133fc078f9ap-8,
    -0x1.fffffffffffffp-9
  },
  { // Entry 478
    0x1.000055558888ad1aee1ef9340407975ap-8,
    0x1.0p-8
  },
  { // Entry 479
    -0x1.000055558888ad1aee1ef9340407975ap-8,
    -0x1.0p-8
  },
  { // Entry 480
    0x1.000055558888bd1afe1f09341407a85bp-8,
    0x1.0000000000001p-8
  },
  { // Entry 481
    -0x1.000055558888bd1afe1f09341407a85bp-8,
    -0x1.0000000000001p-8
  },
  { // Entry 482
    0x1.000015555888811acfc98c1e9ae230fcp-9,
    0x1.fffffffffffffp-10
  },
  { // Entry 483
    -0x1.000015555888811acfc98c1e9ae230fcp-9,
    -0x1.fffffffffffffp-10
  },
  { // Entry 484
    0x1.000015555888891ad1c98c9e9b0230f4p-9,
    0x1.0p-9
  },
  { // Entry 485
    -0x1.000015555888891ad1c98c9e9b0230f4p-9,
    -0x1.0p-9
  },
  { // Entry 486
    0x1.000015555888991ad5c98d9e9b423144p-9,
    0x1.0000000000001p-9
  },
  { // Entry 487
    -0x1.000015555888991ad5c98d9e9b423144p-9,
    -0x1.0000000000001p-9
  },
  { // Entry 488
    0x1.000005555588808ad12d373b75ab20a3p-10,
    0x1.fffffffffffffp-11
  },
  { // Entry 489
    -0x1.000005555588808ad12d373b75ab20a3p-10,
    -0x1.fffffffffffffp-11
  },
  { // Entry 490
    0x1.000005555588888ad1ad374375aba09fp-10,
    0x1.0p-10
  },
  { // Entry 491
    -0x1.000005555588888ad1ad374375aba09fp-10,
    -0x1.0p-10
  },
  { // Entry 492
    0x1.000005555588988ad2ad375375aca0afp-10,
    0x1.0000000000001p-10
  },
  { // Entry 493
    -0x1.000005555588988ad2ad375375aca0afp-10,
    -0x1.0000000000001p-10
  },
  { // Entry 494
    0x1.0000000555554d8888880ad1ad12ee1ep-14,
    0x1.fffffffffffffp-15
  },
  { // Entry 495
    -0x1.0000000555554d8888880ad1ad12ee1ep-14,
    -0x1.fffffffffffffp-15
  },
  { // Entry 496
    0x1.000000055555558888888ad1ad1aee1ep-14,
    0x1.0p-14
  },
  { // Entry 497
    -0x1.000000055555558888888ad1ad1aee1ep-14,
    -0x1.0p-14
  },
  { // Entry 498
    0x1.000000055555658888898ad1ad2aee1ep-14,
    0x1.0000000000001p-14
  },
  { // Entry 499
    -0x1.000000055555658888898ad1ad2aee1ep-14,
    -0x1.0000000000001p-14
  },
  { // Entry 500
    0x1.fffffffffffff0aaaaaaaaaaaa9b1111p-29,
    0x1.fffffffffffffp-29
  },
  { // Entry 501
    -0x1.fffffffffffff0aaaaaaaaaaaa9b1111p-29,
    -0x1.fffffffffffffp-29
  },
  { // Entry 502
    0x1.00000000000000555555555555558888p-28,
    0x1.0p-28
  },
  { // Entry 503
    -0x1.00000000000000555555555555558888p-28,
    -0x1.0p-28
  },
  { // Entry 504
    0x1.00000000000010555555555555658888p-28,
    0x1.0000000000001p-28
  },
  { // Entry 505
    -0x1.00000000000010555555555555658888p-28,
    -0x1.0000000000001p-28
  },
  { // Entry 506
    0x1.fffffffffffff00aaaaaaaaaaaa9ab11p-31,
    0x1.fffffffffffffp-31
  },
  { // Entry 507
    -0x1.fffffffffffff00aaaaaaaaaaaa9ab11p-31,
    -0x1.fffffffffffffp-31
  },
  { // Entry 508
    0x1.00000000000000055555555555555588p-30,
    0x1.0p-30
  },
  { // Entry 509
    -0x1.00000000000000055555555555555588p-30,
    -0x1.0p-30
  },
  { // Entry 510
    0x1.00000000000010055555555555565588p-30,
    0x1.0000000000001p-30
  },
  { // Entry 511
    -0x1.00000000000010055555555555565588p-30,
    -0x1.0000000000001p-30
  },
  { // Entry 512
    -0x1.193ea7aad030becbf96ee2aa5b029927p-1,
    -0x1.0000000000001p-1
  },
  { // Entry 513
    0x1.193ea7aad030becbf96ee2aa5b029927p-1,
    0x1.0000000000001p-1
  },
  { // Entry 514
    -0x1.193ea7aad030a976a4198d55053b7cb5p-1,
    -0x1.0p-1
  },
  { // Entry 515
    0x1.193ea7aad030a976a4198d55053b7cb5p-1,
    0x1.0p-1
  },
  { // Entry 516
    -0x1.193ea7aad0309ecbf96ee2aa5aad43d2p-1,
    -0x1.fffffffffffffp-2
  },
  { // Entry 517
    0x1.193ea7aad0309ecbf96ee2aa5aad43d2p-1,
    0x1.fffffffffffffp-2
  },
  { // Entry 518
    -0x1.058aefa811452b8387cd4093155eafe4p-2,
    -0x1.0000000000001p-2
  },
  { // Entry 519
    0x1.058aefa811452b8387cd4093155eafe4p-2,
    0x1.0000000000001p-2
  },
  { // Entry 520
    -0x1.058aefa811451a7276bc2f82043b6a7dp-2,
    -0x1.0p-2
  },
  { // Entry 521
    0x1.058aefa811451a7276bc2f82043b6a7dp-2,
    0x1.0p-2
  },
  { // Entry 522
    -0x1.058aefa8114511e9ee33a6f97bb76f0ap-2,
    -0x1.fffffffffffffp-3
  },
  { // Entry 523
    0x1.058aefa8114511e9ee33a6f97bb76f0ap-2,
    0x1.fffffffffffffp-3
  },
  { // Entry 524
    -0x1.015891c9eaef86da4a8a506fa1b18969p-3,
    -0x1.0000000000001p-3
  },
  { // Entry 525
    0x1.015891c9eaef86da4a8a506fa1b18969p-3,
    0x1.0000000000001p-3
  },
  { // Entry 526
    -0x1.015891c9eaef7699467a0f6b916c6494p-3,
    -0x1.0p-3
  },
  { // Entry 527
    0x1.015891c9eaef7699467a0f6b916c6494p-3,
    0x1.0p-3
  },
  { // Entry 528
    -0x1.015891c9eaef6e78c471eee9894ceabdp-3,
    -0x1.fffffffffffffp-4
  },
  { // Entry 529
    0x1.015891c9eaef6e78c471eee9894ceabdp-3,
    0x1.fffffffffffffp-4
  },
  { // Entry 530
    -0x1.005588ad375adddb2322b573d6963771p-4,
    -0x1.0000000000001p-4
  },
  { // Entry 531
    0x1.005588ad375adddb2322b573d6963771p-4,
    0x1.0000000000001p-4
  },
  { // Entry 532
    -0x1.005588ad375acdcb1312a563c685255ep-4,
    -0x1.0p-4
  },
  { // Entry 533
    0x1.005588ad375acdcb1312a563c685255ep-4,
    0x1.0p-4
  },
  { // Entry 534
    -0x1.005588ad375ac5c30b0a9d5bbe7d5dd7p-4,
    -0x1.fffffffffffffp-5
  },
  { // Entry 535
    0x1.005588ad375ac5c30b0a9d5bbe7d5dd7p-4,
    0x1.fffffffffffffp-5
  },
  { // Entry 536
    -0x1.001558891aee34b89ed43dd5ba702a52p-5,
    -0x1.0000000000001p-5
  },
  { // Entry 537
    0x1.001558891aee34b89ed43dd5ba702a52p-5,
    0x1.0000000000001p-5
  },
  { // Entry 538
    -0x1.001558891aee24b49dd3fdc5b66ee9f1p-5,
    -0x1.0p-5
  },
  { // Entry 539
    0x1.001558891aee24b49dd3fdc5b66ee9f1p-5,
    0x1.0p-5
  },
  { // Entry 540
    -0x1.001558891aee1cb29d53ddbdb46e79d9p-5,
    -0x1.fffffffffffffp-6
  },
  { // Entry 541
    0x1.001558891aee1cb29d53ddbdb46e79d9p-5,
    0x1.fffffffffffffp-6
  },
  { // Entry 542
    -0x1.000555888ad1d98f0d4a572aded438c7p-6,
    -0x1.0000000000001p-6
  },
  { // Entry 543
    0x1.000555888ad1d98f0d4a572aded438c7p-6,
    0x1.0000000000001p-6
  },
  { // Entry 544
    -0x1.000555888ad1c98e0d3a562aced328b5p-6,
    -0x1.0p-6
  },
  { // Entry 545
    0x1.000555888ad1c98e0d3a562aced328b5p-6,
    0x1.0p-6
  },
  { // Entry 546
    -0x1.000555888ad1c18d8d3255aac6d2acadp-6,
    -0x1.fffffffffffffp-7
  },
  { // Entry 547
    0x1.000555888ad1c18d8d3255aac6d2acadp-6,
    0x1.fffffffffffffp-7
  },
  { // Entry 548
    -0x1.000155588891bd3783d24fc85db13332p-7,
    -0x1.0000000000001p-7
  },
  { // Entry 549
    0x1.000155588891bd3783d24fc85db13332p-7,
    0x1.0000000000001p-7
  },
  { // Entry 550
    -0x1.000155588891ad3743d14fc45da12ef2p-7,
    -0x1.0p-7
  },
  { // Entry 551
    0x1.000155588891ad3743d14fc45da12ef2p-7,
    0x1.0p-7
  },
  { // Entry 552
    -0x1.000155588891a53723d0cfc25d992fd2p-7,
    -0x1.fffffffffffffp-8
  },
  { // Entry 553
    0x1.000155588891a53723d0cfc25d992fd2p-7,
    0x1.fffffffffffffp-8
  },
  { // Entry 554
    -0x1.000055558888bd1afe1f09341407a85bp-8,
    -0x1.0000000000001p-8
  },
  { // Entry 555
    0x1.000055558888bd1afe1f09341407a85bp-8,
    0x1.0000000000001p-8
  },
  { // Entry 556
    -0x1.000055558888ad1aee1ef9340407975ap-8,
    -0x1.0p-8
  },
  { // Entry 557
    0x1.000055558888ad1aee1ef9340407975ap-8,
    0x1.0p-8
  },
  { // Entry 558
    -0x1.000055558888a51ae61ef133fc078f9ap-8,
    -0x1.fffffffffffffp-9
  },
  { // Entry 559
    0x1.000055558888a51ae61ef133fc078f9ap-8,
    0x1.fffffffffffffp-9
  },
  { // Entry 560
    -0x1.000015555888991ad5c98d9e9b423144p-9,
    -0x1.0000000000001p-9
  },
  { // Entry 561
    0x1.000015555888991ad5c98d9e9b423144p-9,
    0x1.0000000000001p-9
  },
  { // Entry 562
    -0x1.000015555888891ad1c98c9e9b0230f4p-9,
    -0x1.0p-9
  },
  { // Entry 563
    0x1.000015555888891ad1c98c9e9b0230f4p-9,
    0x1.0p-9
  },
  { // Entry 564
    -0x1.000015555888811acfc98c1e9ae230fcp-9,
    -0x1.fffffffffffffp-10
  },
  { // Entry 565
    0x1.000015555888811acfc98c1e9ae230fcp-9,
    0x1.fffffffffffffp-10
  },
  { // Entry 566
    -0x1.000005555588988ad2ad375375aca0afp-10,
    -0x1.0000000000001p-10
  },
  { // Entry 567
    0x1.000005555588988ad2ad375375aca0afp-10,
    0x1.0000000000001p-10
  },
  { // Entry 568
    -0x1.000005555588888ad1ad374375aba09fp-10,
    -0x1.0p-10
  },
  { // Entry 569
    0x1.000005555588888ad1ad374375aba09fp-10,
    0x1.0p-10
  },
  { // Entry 570
    -0x1.000005555588808ad12d373b75ab20a3p-10,
    -0x1.fffffffffffffp-11
  },
  { // Entry 571
    0x1.000005555588808ad12d373b75ab20a3p-10,
    0x1.fffffffffffffp-11
  },
  { // Entry 572
    -0x1.000000055555658888898ad1ad2aee1ep-14,
    -0x1.0000000000001p-14
  },
  { // Entry 573
    0x1.000000055555658888898ad1ad2aee1ep-14,
    0x1.0000000000001p-14
  },
  { // Entry 574
    -0x1.000000055555558888888ad1ad1aee1ep-14,
    -0x1.0p-14
  },
  { // Entry 575
    0x1.000000055555558888888ad1ad1aee1ep-14,
    0x1.0p-14
  },
  { // Entry 576
    -0x1.0000000555554d8888880ad1ad12ee1ep-14,
    -0x1.fffffffffffffp-15
  },
  { // Entry 577
    0x1.0000000555554d8888880ad1ad12ee1ep-14,
    0x1.fffffffffffffp-15
  },
  { // Entry 578
    -0x1.00000000000010555555555555658888p-28,
    -0x1.0000000000001p-28
  },
  { // Entry 579
    0x1.00000000000010555555555555658888p-28,
    0x1.0000000000001p-28
  },
  { // Entry 580
    -0x1.00000000000000555555555555558888p-28,
    -0x1.0p-28
  },
  { // Entry 581
    0x1.00000000000000555555555555558888p-28,
    0x1.0p-28
  },
  { // Entry 582
    -0x1.fffffffffffff0aaaaaaaaaaaa9b1111p-29,
    -0x1.fffffffffffffp-29
  },
  { // Entry 583
    0x1.fffffffffffff0aaaaaaaaaaaa9b1111p-29,
    0x1.fffffffffffffp-29
  },
  { // Entry 584
    -0x1.00000000000010055555555555565588p-30,
    -0x1.0000000000001p-30
  },
  { // Entry 585
    0x1.00000000000010055555555555565588p-30,
    0x1.0000000000001p-30
  },
  { // Entry 586
    -0x1.00000000000000055555555555555588p-30,
    -0x1.0p-30
  },
  { // Entry 587
    0x1.00000000000000055555555555555588p-30,
    0x1.0p-30
  },
  { // Entry 588
    -0x1.fffffffffffff00aaaaaaaaaaaa9ab11p-31,
    -0x1.fffffffffffffp-31
  },
  { // Entry 589
    0x1.fffffffffffff00aaaaaaaaaaaa9ab11p-31,
    0x1.fffffffffffffp-31
  },
  { // Entry 590
    HUGE_VAL,
    0x1.0p0
  },
  { // Entry 591
    -HUGE_VAL,
    -0x1.0p0
  },
  { // Entry 592
    0x1.2b708872320e1d31e4b03f1086a9c047p4,
    0x1.fffffffffffffp-1
  },
  { // Entry 593
    -0x1.2b708872320e1d31e4b03f1086a9c047p4,
    -0x1.fffffffffffffp-1
  },
  { // Entry 594
    0x1.0f2eb070230688149a25318fd8d4ea0fp0,
    0x1.921fb54442d18p-1
  },
  { // Entry 595
    -0x1.0f2eb070230688149a25318fd8d4ea0fp0,
    -0x1.921fb54442d18p-1
  },
  { // Entry 596
    0x1.00000000000010p-1022,
    0x1.0000000000001p-1022
  },
  { // Entry 597
    -0x1.00000000000010p-1022,
    -0x1.0000000000001p-1022
  },
  { // Entry 598
    0x1.p-1022,
    0x1.0p-1022
  },
  { // Entry 599
    -0x1.p-1022,
    -0x1.0p-1022
  },
  { // Entry 600
    0x1.ffffffffffffe0p-1023,
    0x1.ffffffffffffep-1023
  },
  { // Entry 601
    -0x1.ffffffffffffe0p-1023,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 602
    0x1.ffffffffffffc0p-1023,
    0x1.ffffffffffffcp-1023
  },
  { // Entry 603
    -0x1.ffffffffffffc0p-1023,
    -0x1.ffffffffffffcp-1023
  },
  { // Entry 604
    0x1.p-1073,
    0x1.0p-1073
  },
  { // Entry 605
    -0x1.p-1073,
    -0x1.0p-1073
  },
  { // Entry 606
    0x1.p-1074,
    0x1.0p-1074
  },
  { // Entry 607
    -0x1.p-1074,
    -0x1.0p-1074
  },
  { // Entry 608
    0.0,
    0.0
  },
  { // Entry 609
    -0.0,
    -0.0
  }
};
