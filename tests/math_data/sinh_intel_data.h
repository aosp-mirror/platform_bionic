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

static data_1_1_t<double, double> g_sinh_intel_data[] = {
  { // Entry 0
    -0x1.20000000000f30000000003d82666666p-21,
    -0x1.2p-21
  },
  { // Entry 1
    0x1.20000000000f30000000003d82666666p-21,
    0x1.2p-21
  },
  { // Entry 2
    -0x1.f9225f7e3193c80156e29378c34b23d3p831,
    -0x1.20b0659d8a7e1p9
  },
  { // Entry 3
    0x1.f9225f7e3193c80156e29378c34b23d3p831,
    0x1.20b0659d8a7e1p9
  },
  { // Entry 4
    -0x1.653a9c0c08b00f3a2fd5ec9fb1093ff9p52,
    -0x1.288f5c28f5c28p5
  },
  { // Entry 5
    0x1.653a9c0c08b00f3a2fd5ec9fb1093ff9p52,
    0x1.288f5c28f5c28p5
  },
  { // Entry 6
    -0x1.45f775546a77d77fe6234ab5d4ab1036p-1,
    -0x1.3333333333333p-1
  },
  { // Entry 7
    0x1.45f775546a77d77fe6234ab5d4ab1036p-1,
    0x1.3333333333333p-1
  },
  { // Entry 8
    -0x1.e128d3a99c4b28216511a7ad98d106f0p911,
    -0x1.3c640p9
  },
  { // Entry 9
    0x1.e128d3a99c4b28216511a7ad98d106f0p911,
    0x1.3c640p9
  },
  { // Entry 10
    -0x1.41ab9d8bc0cbf81eac0e2bbbbbeab96cp-5,
    -0x1.41967803312afp-5
  },
  { // Entry 11
    0x1.41ab9d8bc0cbf81eac0e2bbbbbeab96cp-5,
    0x1.41967803312afp-5
  },
  { // Entry 12
    -0x1.f5f62d444f24c3bee8aeb75a413b8101p941,
    -0x1.46cf1a4e8eff8p9
  },
  { // Entry 13
    0x1.f5f62d444f24c3bee8aeb75a413b8101p941,
    0x1.46cf1a4e8eff8p9
  },
  { // Entry 14
    -0x1.fd0d27f4cfe98801d15fa2d48e5a54c0p952,
    -0x1.4aa0d96719fc6p9
  },
  { // Entry 15
    0x1.fd0d27f4cfe98801d15fa2d48e5a54c0p952,
    0x1.4aa0d96719fc6p9
  },
  { // Entry 16
    -0x1.f7667599728a17ff07667fe41510ac6cp13,
    -0x1.4c2b291cfadd2p3
  },
  { // Entry 17
    0x1.f7667599728a17ff07667fe41510ac6cp13,
    0x1.4c2b291cfadd2p3
  },
  { // Entry 18
    -0x1.eb34f0a92ee7280128c114f07d355776p958,
    -0x1.4cb09e65eb930p9
  },
  { // Entry 19
    0x1.eb34f0a92ee7280128c114f07d355776p958,
    0x1.4cb09e65eb930p9
  },
  { // Entry 20
    -0x1.d2108e9aa1b124168960a67f1cb28b07p960,
    -0x1.4d5b56d5b55acp9
  },
  { // Entry 21
    0x1.d2108e9aa1b124168960a67f1cb28b07p960,
    0x1.4d5b56d5b55acp9
  },
  { // Entry 22
    -0x1.f68d7cf30e0123c2164c1368d213f9b5p970,
    -0x1.50dc3739dde8ep9
  },
  { // Entry 23
    0x1.f68d7cf30e0123c2164c1368d213f9b5p970,
    0x1.50dc3739dde8ep9
  },
  { // Entry 24
    -0x1.fd799430443f0800ef2fcf2cd9da9697p975,
    -0x1.529994bb15795p9
  },
  { // Entry 25
    0x1.fd799430443f0800ef2fcf2cd9da9697p975,
    0x1.529994bb15795p9
  },
  { // Entry 26
    -0x1.e7b36eb1f1e698017d905c91e25df616p1005,
    -0x1.5cf9ace27d120p9
  },
  { // Entry 27
    0x1.e7b36eb1f1e698017d905c91e25df616p1005,
    0x1.5cf9ace27d120p9
  },
  { // Entry 28
    -0x1.99d18edc5720a8018d0c8bfbf7d98f9ap1011,
    -0x1.5ef7bdef7c2eep9
  },
  { // Entry 29
    0x1.99d18edc5720a8018d0c8bfbf7d98f9ap1011,
    0x1.5ef7bdef7c2eep9
  },
  { // Entry 30
    -0x1.f3ae6e6ad6cf33f952b8d7d26abcd946p1013,
    -0x1.5fc2907bbfb53p9
  },
  { // Entry 31
    0x1.f3ae6e6ad6cf33f952b8d7d26abcd946p1013,
    0x1.5fc2907bbfb53p9
  },
  { // Entry 32
    -0x1.fffffffffeb2a1b0e263ac68076ed4e0p1022,
    -0x1.62e42fefa39eap9
  },
  { // Entry 33
    0x1.fffffffffeb2a1b0e263ac68076ed4e0p1022,
    0x1.62e42fefa39eap9
  },
  { // Entry 34
    -0x1.94f8e42c219097ffbf5323a28f7d715cp-2,
    -0x1.8b1896d9e5343p-2
  },
  { // Entry 35
    0x1.94f8e42c219097ffbf5323a28f7d715cp-2,
    0x1.8b1896d9e5343p-2
  },
  { // Entry 36
    -0x1.90p-1069,
    -0x1.9p-1069
  },
  { // Entry 37
    0x1.90p-1069,
    0x1.9p-1069
  },
  { // Entry 38
    -0x1.a45c4bfc4132c8e8dd4e44e918fef502p-2,
    -0x1.995e6ee493fe5p-2
  },
  { // Entry 39
    0x1.a45c4bfc4132c8e8dd4e44e918fef502p-2,
    0x1.995e6ee493fe5p-2
  },
  { // Entry 40
    -0x1.c7a103379809b38132350ca766cba44bp-1,
    -0x1.9a495ea447852p-1
  },
  { // Entry 41
    0x1.c7a103379809b38132350ca766cba44bp-1,
    0x1.9a495ea447852p-1
  },
  { // Entry 42
    -0x1.9bfa3403f8d457fffa246d880ef814f1p-5,
    -0x1.9bcdcc408ced3p-5
  },
  { // Entry 43
    0x1.9bfa3403f8d457fffa246d880ef814f1p-5,
    0x1.9bcdcc408ced3p-5
  },
  { // Entry 44
    -0x1.aa8c3d4cb5bd9ffffe5e559c563d669cp-5,
    -0x1.aa5af545ae880p-5
  },
  { // Entry 45
    0x1.aa8c3d4cb5bd9ffffe5e559c563d669cp-5,
    0x1.aa5af545ae880p-5
  },
  { // Entry 46
    -0x1.e17e32eb8e6ce8f6dad6a738fa5f0b8ap-2,
    -0x1.d15037f2ebe3cp-2
  },
  { // Entry 47
    0x1.e17e32eb8e6ce8f6dad6a738fa5f0b8ap-2,
    0x1.d15037f2ebe3cp-2
  },
  { // Entry 48
    -0x1.ffffffffffffc0p-1023,
    -0x1.ffffffffffffcp-1023
  },
  { // Entry 49
    0x1.ffffffffffffc0p-1023,
    0x1.ffffffffffffcp-1023
  },
  { // Entry 50
    0x1.p-1074,
    0x1.0p-1074
  },
  { // Entry 51
    -0x1.p-1074,
    -0x1.0p-1074
  },
  { // Entry 52
    0x1.000000000000100000000aaaaaaaaaaap-41,
    0x1.0000000000001p-41
  },
  { // Entry 53
    -0x1.000000000000100000000aaaaaaaaaaap-41,
    -0x1.0000000000001p-41
  },
  { // Entry 54
    0x1.41c7a8814c78683cdbc165597ca61d23p368,
    0x1.0000000000007p8
  },
  { // Entry 55
    -0x1.41c7a8814c78683cdbc165597ca61d23p368,
    -0x1.0000000000007p8
  },
  { // Entry 56
    0x1.1f43febb6d5ed7f79073c9dea85050fep45,
    0x1.0000000e0p5
  },
  { // Entry 57
    -0x1.1f43febb6d5ed7f79073c9dea85050fep45,
    -0x1.0000000e0p5
  },
  { // Entry 58
    0x1.749ea5cf24c2a801014b24d9686ff306p10,
    0x1.0000000ffe017p3
  },
  { // Entry 59
    -0x1.749ea5cf24c2a801014b24d9686ff306p10,
    -0x1.0000000ffe017p3
  },
  { // Entry 60
    0x1.0f2ebe19aebbcffe11114fde20a8b25dp22,
    0x1.0000001p4
  },
  { // Entry 61
    -0x1.0f2ebe19aebbcffe11114fde20a8b25dp22,
    -0x1.0000001p4
  },
  { // Entry 62
    0x1.d03d0547eb5af7ff9287bfe9ef08d02cp1,
    0x1.0000040p1
  },
  { // Entry 63
    -0x1.d03d0547eb5af7ff9287bfe9ef08d02cp1,
    -0x1.0000040p1
  },
  { // Entry 64
    0x1.94dbdfbb66bc651ee76572119d9c1c4cp737,
    0x1.0000202p9
  },
  { // Entry 65
    -0x1.94dbdfbb66bc651ee76572119d9c1c4cp737,
    -0x1.0000202p9
  },
  { // Entry 66
    0x1.203fc65a034d07ffda891ef079ee632dp45,
    0x1.00070p5
  },
  { // Entry 67
    -0x1.203fc65a034d07ffda891ef079ee632dp45,
    -0x1.00070p5
  },
  { // Entry 68
    0x1.043203c63ca348000c2d50e004eab9dfp-3,
    0x1.03801bba53d5fp-3
  },
  { // Entry 69
    -0x1.043203c63ca348000c2d50e004eab9dfp-3,
    -0x1.03801bba53d5fp-3
  },
  { // Entry 70
    0x1.d9ceb6fb3af22800eba0709ab550eb83p4,
    0x1.0539a9db00fb0p2
  },
  { // Entry 71
    -0x1.d9ceb6fb3af22800eba0709ab550eb83p4,
    -0x1.0539a9db00fb0p2
  },
  { // Entry 72
    0x1.19d9db769b3073db5694464547980ab3p-1,
    0x1.0d44121952313p-1
  },
  { // Entry 73
    -0x1.19d9db769b3073db5694464547980ab3p-1,
    -0x1.0d44121952313p-1
  },
  { // Entry 74
    0x1.11aa874937f657fefeb22f10657675b2p-3,
    0x1.10dbb163423cfp-3
  },
  { // Entry 75
    -0x1.11aa874937f657fefeb22f10657675b2p-3,
    -0x1.10dbb163423cfp-3
  },
  { // Entry 76
    0x1.a4e4693413b9970755c15633af25f96bp399,
    0x1.15c18de877563p8
  },
  { // Entry 77
    -0x1.a4e4693413b9970755c15633af25f96bp399,
    -0x1.15c18de877563p8
  },
  { // Entry 78
    0x1.1fae3d9220dd280102c022b12bc3f6fep-2,
    0x1.1c065802b26eap-2
  },
  { // Entry 79
    -0x1.1fae3d9220dd280102c022b12bc3f6fep-2,
    -0x1.1c065802b26eap-2
  },
  { // Entry 80
    0x1.20000000000f30000000003d82666666p-21,
    0x1.2p-21
  },
  { // Entry 81
    -0x1.20000000000f30000000003d82666666p-21,
    -0x1.2p-21
  },
  { // Entry 82
    0x1.35a9257a5a1cb7fecfa291714fac9f89p-1,
    0x1.25589e6453a79p-1
  },
  { // Entry 83
    -0x1.35a9257a5a1cb7fecfa291714fac9f89p-1,
    -0x1.25589e6453a79p-1
  },
  { // Entry 84
    0x1.eaa521edf1bc28014602191ce618c05fp845,
    0x1.2586ca9cf411bp9
  },
  { // Entry 85
    -0x1.eaa521edf1bc28014602191ce618c05fp845,
    -0x1.2586ca9cf411bp9
  },
  { // Entry 86
    0x1.6a09e667f3bc9b61130bc056d478d834p25,
    0x1.25e4f7b2737fap4
  },
  { // Entry 87
    -0x1.6a09e667f3bc9b61130bc056d478d834p25,
    -0x1.25e4f7b2737fap4
  },
  { // Entry 88
    0x1.2a9bbf545f09f7fffc2fd473fc2f8a9ep-2,
    0x1.2687ce7bd7353p-2
  },
  { // Entry 89
    -0x1.2a9bbf545f09f7fffc2fd473fc2f8a9ep-2,
    -0x1.2687ce7bd7353p-2
  },
  { // Entry 90
    0x1.2e6ae008ae071731ab9829f451a64edcp-2,
    0x1.2a2f795d6b514p-2
  },
  { // Entry 91
    -0x1.2e6ae008ae071731ab9829f451a64edcp-2,
    -0x1.2a2f795d6b514p-2
  },
  { // Entry 92
    0x1.fffffffffff84dc369f81dfdbc383332p25,
    0x1.2b708872320dep4
  },
  { // Entry 93
    -0x1.fffffffffff84dc369f81dfdbc383332p25,
    -0x1.2b708872320dep4
  },
  { // Entry 94
    0x1.7c12b8d57fdcf7fffa9ce5e99a4163dep0,
    0x1.2fac64a42ceefp0
  },
  { // Entry 95
    -0x1.7c12b8d57fdcf7fffa9ce5e99a4163dep0,
    -0x1.2fac64a42ceefp0
  },
  { // Entry 96
    0x1.dc851a55686d48012add3c02a54cc4d9p875,
    0x1.2fe8bcd183299p9
  },
  { // Entry 97
    -0x1.dc851a55686d48012add3c02a54cc4d9p875,
    -0x1.2fe8bcd183299p9
  },
  { // Entry 98
    0x1.42546737e09d68014ec9badd9df8fee3p-1,
    0x1.30208b74d1760p-1
  },
  { // Entry 99
    -0x1.42546737e09d68014ec9badd9df8fee3p-1,
    -0x1.30208b74d1760p-1
  },
  { // Entry 100
    0x1.ff12f7296b0408017eaaf48fbf280399p877,
    0x1.30a324d6033b5p9
  },
  { // Entry 101
    -0x1.ff12f7296b0408017eaaf48fbf280399p877,
    -0x1.30a324d6033b5p9
  },
  { // Entry 102
    0x1.46c46118f79ad3e3a58cffe90ae6c228p-1,
    0x1.33ep-1
  },
  { // Entry 103
    -0x1.46c46118f79ad3e3a58cffe90ae6c228p-1,
    -0x1.33ep-1
  },
  { // Entry 104
    0x1.4715736c9497d3c2ba6fecc679c26499p-1,
    0x1.342454787e0eap-1
  },
  { // Entry 105
    -0x1.4715736c9497d3c2ba6fecc679c26499p-1,
    -0x1.342454787e0eap-1
  },
  { // Entry 106
    0x1.8b2e83486133082860f65d0b54a3ff4cp0,
    0x1.380p0
  },
  { // Entry 107
    -0x1.8b2e83486133082860f65d0b54a3ff4cp0,
    -0x1.380p0
  },
  { // Entry 108
    0x1.94743c1df51b480057352f3dbc3a206dp0,
    0x1.3d0p0
  },
  { // Entry 109
    -0x1.94743c1df51b480057352f3dbc3a206dp0,
    -0x1.3d0p0
  },
  { // Entry 110
    0x1.43f3767f52a3a8603144ec06b8fc89a2p-2,
    0x1.3ec6f4738ef75p-2
  },
  { // Entry 111
    -0x1.43f3767f52a3a8603144ec06b8fc89a2p-2,
    -0x1.3ec6f4738ef75p-2
  },
  { // Entry 112
    0x1.411bd68cf1f6682ad666958c437bc7d6p-5,
    0x1.4106cd4f76086p-5
  },
  { // Entry 113
    -0x1.411bd68cf1f6682ad666958c437bc7d6p-5,
    -0x1.4106cd4f76086p-5
  },
  { // Entry 114
    0x1.f1ef9c8a4fdae801b19f8e42eff2c4a8p929,
    0x1.42a565e456e04p9
  },
  { // Entry 115
    -0x1.f1ef9c8a4fdae801b19f8e42eff2c4a8p929,
    -0x1.42a565e456e04p9
  },
  { // Entry 116
    0x1.59ae6cfb065053d12a7ee2829e50cbc0p-1,
    0x1.43af5aa457830p-1
  },
  { // Entry 117
    -0x1.59ae6cfb065053d12a7ee2829e50cbc0p-1,
    -0x1.43af5aa457830p-1
  },
  { // Entry 118
    0x1.5a16aa3594c9d3c64013364ec2c00447p-1,
    0x1.4405baf340402p-1
  },
  { // Entry 119
    -0x1.5a16aa3594c9d3c64013364ec2c00447p-1,
    -0x1.4405baf340402p-1
  },
  { // Entry 120
    0x1.f4ec44194b642801a4afd4c50633e8aap937,
    0x1.456bf23e02428p9
  },
  { // Entry 121
    -0x1.f4ec44194b642801a4afd4c50633e8aap937,
    -0x1.456bf23e02428p9
  },
  { // Entry 122
    0x1.ea91d9533b394801bf3d3ec8f88de568p938,
    0x1.45c1feef8086cp9
  },
  { // Entry 123
    -0x1.ea91d9533b394801bf3d3ec8f88de568p938,
    -0x1.45c1feef8086cp9
  },
  { // Entry 124
    0x1.5c98a49e6503ac198ec2bf98d4df0ce2p-1,
    0x1.4618fe408b57ep-1
  },
  { // Entry 125
    -0x1.5c98a49e6503ac198ec2bf98d4df0ce2p-1,
    -0x1.4618fe408b57ep-1
  },
  { // Entry 126
    0x1.4f14e08c88501827cbe80379c7e7c8c3p-2,
    0x1.495f61b2c806cp-2
  },
  { // Entry 127
    -0x1.4f14e08c88501827cbe80379c7e7c8c3p-2,
    -0x1.495f61b2c806cp-2
  },
  { // Entry 128
    0x1.50000000006078000000084f22cccccdp-20,
    0x1.5p-20
  },
  { // Entry 129
    -0x1.50000000006078000000084f22cccccdp-20,
    -0x1.5p-20
  },
  { // Entry 130
    0x1.6e81c7d17d1df3beb45414ca53bc3f28p-1,
    0x1.54c834c39835cp-1
  },
  { // Entry 131
    -0x1.6e81c7d17d1df3beb45414ca53bc3f28p-1,
    -0x1.54c834c39835cp-1
  },
  { // Entry 132
    0x1.9548e9688fb2e800c466e7d893328f68p991,
    0x1.5807dc787a5d5p9
  },
  { // Entry 133
    -0x1.9548e9688fb2e800c466e7d893328f68p991,
    -0x1.5807dc787a5d5p9
  },
  { // Entry 134
    0x1.85442a2969ace4a71757c1cdd6ab1b82p1007,
    0x1.5d8e43cd6785dp9
  },
  { // Entry 135
    -0x1.85442a2969ace4a71757c1cdd6ab1b82p1007,
    -0x1.5d8e43cd6785dp9
  },
  { // Entry 136
    0x1.f2a3c29fc6d723a63e349bbcd7894a8ap1007,
    0x1.5dadf5d1e452cp9
  },
  { // Entry 137
    -0x1.f2a3c29fc6d723a63e349bbcd7894a8ap1007,
    -0x1.5dadf5d1e452cp9
  },
  { // Entry 138
    0x1.6487aa6e34cb57fefbc4f4ef7b7281efp-2,
    0x1.5db17c4a60fe4p-2
  },
  { // Entry 139
    -0x1.6487aa6e34cb57fefbc4f4ef7b7281efp-2,
    -0x1.5db17c4a60fe4p-2
  },
  { // Entry 140
    0x1.edcb14879613e80176087c1a76dec97cp1008,
    0x1.5e056ed40e56ep9
  },
  { // Entry 141
    -0x1.edcb14879613e80176087c1a76dec97cp1008,
    -0x1.5e056ed40e56ep9
  },
  { // Entry 142
    0x1.6093d5bf2580f00bcdc6c88ac35d2640p-6,
    0x1.608cdeb3ec111p-6
  },
  { // Entry 143
    -0x1.6093d5bf2580f00bcdc6c88ac35d2640p-6,
    -0x1.608cdeb3ec111p-6
  },
  { // Entry 144
    0x1.614e81af3113300dfbdf1ed60454760dp-6,
    0x1.61477f8e69928p-6
  },
  { // Entry 145
    -0x1.614e81af3113300dfbdf1ed60454760dp-6,
    -0x1.61477f8e69928p-6
  },
  { // Entry 146
    0x1.96faa872a06aa3003f5158de9c570302p1023,
    0x1.631f86ac0611bp9
  },
  { // Entry 147
    -0x1.96faa872a06aa3003f5158de9c570302p1023,
    -0x1.631f86ac0611bp9
  },
  { // Entry 148
    0x1.bf668fc6f4f9d31f753c408261bb02b4p1023,
    0x1.632ba58eae071p9
  },
  { // Entry 149
    -0x1.bf668fc6f4f9d31f753c408261bb02b4p1023,
    -0x1.632ba58eae071p9
  },
  { // Entry 150
    0x1.ffffffffbcd3ae595303e74abb3fd75dp1023,
    0x1.633ce8fb9f771p9
  },
  { // Entry 151
    -0x1.ffffffffbcd3ae595303e74abb3fd75dp1023,
    -0x1.633ce8fb9f771p9
  },
  { // Entry 152
    0x1.fffffffff093ae594ed7508a02429436p1023,
    0x1.633ce8fb9f840p9
  },
  { // Entry 153
    -0x1.fffffffff093ae594ed7508a02429436p1023,
    -0x1.633ce8fb9f840p9
  },
  { // Entry 154
    0x1.fffffffff713ae594eafc080a48289f9p1023,
    0x1.633ce8fb9f85ap9
  },
  { // Entry 155
    -0x1.fffffffff713ae594eafc080a48289f9p1023,
    -0x1.633ce8fb9f85ap9
  },
  { // Entry 156
    0x1.63ceae09cb498fefee897d9d09f0f101p-6,
    0x1.63c7858c9e520p-6
  },
  { // Entry 157
    -0x1.63ceae09cb498fefee897d9d09f0f101p-6,
    -0x1.63c7858c9e520p-6
  },
  { // Entry 158
    0x1.650506712af37ff54f81db8832a797e9p-6,
    0x1.64fdcb29465a9p-6
  },
  { // Entry 159
    -0x1.650506712af37ff54f81db8832a797e9p-6,
    -0x1.64fdcb29465a9p-6
  },
  { // Entry 160
    0x1.8465153de7c007fffdec1cebc40841f7p-1,
    0x1.66666666d5da3p-1
  },
  { // Entry 161
    -0x1.8465153de7c007fffdec1cebc40841f7p-1,
    -0x1.66666666d5da3p-1
  },
  { // Entry 162
    0x1.f2056a8752c0a54d0803a9f0c00baf5ep0,
    0x1.6b4p0
  },
  { // Entry 163
    -0x1.f2056a8752c0a54d0803a9f0c00baf5ep0,
    -0x1.6b4p0
  },
  { // Entry 164
    0x1.f54f8eaffc4348da8305aef164c3ba94p0,
    0x1.6ccp0
  },
  { // Entry 165
    -0x1.f54f8eaffc4348da8305aef164c3ba94p0,
    -0x1.6ccp0
  },
  { // Entry 166
    0x1.71e202c00c2319229a5b86f0f26cabb1p-7,
    0x1.71ep-7
  },
  { // Entry 167
    -0x1.71e202c00c2319229a5b86f0f26cabb1p-7,
    -0x1.71ep-7
  },
  { // Entry 168
    0x1.773253dd3f311874a2f0c80764abf42dp-8,
    0x1.7731cd8b74641p-8
  },
  { // Entry 169
    -0x1.773253dd3f311874a2f0c80764abf42dp-8,
    -0x1.7731cd8b74641p-8
  },
  { // Entry 170
    0x1.a08a89e90462985ff5cdbef5be2c143ep-1,
    0x1.7c874423b4655p-1
  },
  { // Entry 171
    -0x1.a08a89e90462985ff5cdbef5be2c143ep-1,
    -0x1.7c874423b4655p-1
  },
  { // Entry 172
    0x1.808fd7f5059e47fed3be0eaf1b9815bdp-4,
    0x1.7fffc7fffffffp-4
  },
  { // Entry 173
    -0x1.808fd7f5059e47fed3be0eaf1b9815bdp-4,
    -0x1.7fffc7fffffffp-4
  },
  { // Entry 174
    0x1.804bf5b40d34e7d143ae427670a1c115p-5,
    0x1.8027e9757bf31p-5
  },
  { // Entry 175
    -0x1.804bf5b40d34e7d143ae427670a1c115p-5,
    -0x1.8027e9757bf31p-5
  },
  { // Entry 176
    0x1.f0e3e5b6c17c02a799c306dbc1b1a8ecp555,
    0x1.820d92fc4b42ap8
  },
  { // Entry 177
    -0x1.f0e3e5b6c17c02a799c306dbc1b1a8ecp555,
    -0x1.820d92fc4b42ap8
  },
  { // Entry 178
    0x1.88665ffaada937dd93491498f2ee9defp-5,
    0x1.884p-5
  },
  { // Entry 179
    -0x1.88665ffaada937dd93491498f2ee9defp-5,
    -0x1.884p-5
  },
  { // Entry 180
    0x1.b4698d92404057ff9724122622082863p-1,
    0x1.8bcbf70469a4cp-1
  },
  { // Entry 181
    -0x1.b4698d92404057ff9724122622082863p-1,
    -0x1.8bcbf70469a4cp-1
  },
  { // Entry 182
    0x1.f612c0c32a0c700d32addc4c6c2b76c3p7,
    0x1.8e0p2
  },
  { // Entry 183
    -0x1.f612c0c32a0c700d32addc4c6c2b76c3p7,
    -0x1.8e0p2
  },
  { // Entry 184
    0x1.bc98715aec56c731186b8ab8deb14510p-1,
    0x1.920p-1
  },
  { // Entry 185
    -0x1.bc98715aec56c731186b8ab8deb14510p-1,
    -0x1.920p-1
  },
  { // Entry 186
    0x1.94618fa9fe2ddce9a5820eacd73ea447p-8,
    0x1.9460e77feba40p-8
  },
  { // Entry 187
    -0x1.94618fa9fe2ddce9a5820eacd73ea447p-8,
    -0x1.9460e77feba40p-8
  },
  { // Entry 188
    0x1.c6b5d3ca60e64ffffe54e7a9d176a802p-1,
    0x1.9999999a10d54p-1
  },
  { // Entry 189
    -0x1.c6b5d3ca60e64ffffe54e7a9d176a802p-1,
    -0x1.9999999a10d54p-1
  },
  { // Entry 190
    0x1.9c560cd3fc44bffffe366df58ed96becp-3,
    0x1.9999999a33ce2p-3
  },
  { // Entry 191
    -0x1.9c560cd3fc44bffffe366df58ed96becp-3,
    -0x1.9999999a33ce2p-3
  },
  { // Entry 192
    0x1.9a48733871434800020cf29985e49f30p-4,
    0x1.9999999a54528p-4
  },
  { // Entry 193
    -0x1.9a48733871434800020cf29985e49f30p-4,
    -0x1.9999999a54528p-4
  },
  { // Entry 194
    0x1.c6b5d3cb11239800020efc8821633bdep-1,
    0x1.9999999a949b7p-1
  },
  { // Entry 195
    -0x1.c6b5d3cb11239800020efc8821633bdep-1,
    -0x1.9999999a949b7p-1
  },
  { // Entry 196
    0x1.9c560cd481e637fffe01fbfdad836d2fp-3,
    0x1.9999999ab6ceap-3
  },
  { // Entry 197
    -0x1.9c560cd481e637fffe01fbfdad836d2fp-3,
    -0x1.9999999ab6ceap-3
  },
  { // Entry 198
    0x1.9c560cd64ac02ffffe536624b3aa885fp-3,
    0x1.9999999c76abep-3
  },
  { // Entry 199
    -0x1.9c560cd64ac02ffffe536624b3aa885fp-3,
    -0x1.9999999c76abep-3
  },
  { // Entry 200
    0x1.9c560cd6ec5c70000171ee4afa616629p-3,
    0x1.9999999d151a4p-3
  },
  { // Entry 201
    -0x1.9c560cd6ec5c70000171ee4afa616629p-3,
    -0x1.9999999d151a4p-3
  },
  { // Entry 202
    0x1.a5573f0e20f1a905add879f99041019fp-2,
    0x1.9a468b4ef44ffp-2
  },
  { // Entry 203
    -0x1.a5573f0e20f1a905add879f99041019fp-2,
    -0x1.9a468b4ef44ffp-2
  },
  { // Entry 204
    0x1.ffffffffffed457a42e161456cf862b2p590,
    0x1.9a57d76d152fcp8
  },
  { // Entry 205
    -0x1.ffffffffffed457a42e161456cf862b2p590,
    -0x1.9a57d76d152fcp8
  },
  { // Entry 206
    0x1.aebdc6208c1248b2ffbba5f783b92a1bp-2,
    0x1.a2f46ea5f9049p-2
  },
  { // Entry 207
    -0x1.aebdc6208c1248b2ffbba5f783b92a1bp-2,
    -0x1.a2f46ea5f9049p-2
  },
  { // Entry 208
    0x1.d64a3c2bfdf088565dad6aa8d6e58f04p-1,
    0x1.a5294a5294a50p-1
  },
  { // Entry 209
    -0x1.d64a3c2bfdf088565dad6aa8d6e58f04p-1,
    -0x1.a5294a5294a50p-1
  },
  { // Entry 210
    0x1.b000000000cd080000001d316399999bp-20,
    0x1.bp-20
  },
  { // Entry 211
    -0x1.b000000000cd080000001d316399999bp-20,
    -0x1.bp-20
  },
  { // Entry 212
    0x1.b6206a36aff4e7f3be1967dee6c19c2dp-3,
    0x1.b2da0d9913589p-3
  },
  { // Entry 213
    -0x1.b6206a36aff4e7f3be1967dee6c19c2dp-3,
    -0x1.b2da0d9913589p-3
  },
  { // Entry 214
    0x1.bd28b272834a2ff3d79fb12e37a18714p-6,
    0x1.bd1aae2323510p-6
  },
  { // Entry 215
    -0x1.bd28b272834a2ff3d79fb12e37a18714p-6,
    -0x1.bd1aae2323510p-6
  },
  { // Entry 216
    0x1.f8c25081d25d19d0d73bf466555bef04p-1,
    0x1.be2p-1
  },
  { // Entry 217
    -0x1.f8c25081d25d19d0d73bf466555bef04p-1,
    -0x1.be2p-1
  },
  { // Entry 218
    0x1.cc8f25f94323284f6d6454d3f3699088p-2,
    0x1.be4b949799901p-2
  },
  { // Entry 219
    -0x1.cc8f25f94323284f6d6454d3f3699088p-2,
    -0x1.be4b949799901p-2
  },
  { // Entry 220
    0x1.068f1b6fd97a97ffc554f2db188f9117p4,
    0x1.bef89775b5e88p1
  },
  { // Entry 221
    -0x1.068f1b6fd97a97ffc554f2db188f9117p4,
    -0x1.bef89775b5e88p1
  },
  { // Entry 222
    0x1.ca73ad17d1f0afff8b6da0d520741b26p-4,
    0x1.c98p-4
  },
  { // Entry 223
    -0x1.ca73ad17d1f0afff8b6da0d520741b26p-4,
    -0x1.c98p-4
  },
  { // Entry 224
    0x1.742768cc82d3b80460f95c20f53171c3p1,
    0x1.c9eca0f325b42p0
  },
  { // Entry 225
    -0x1.742768cc82d3b80460f95c20f53171c3p1,
    -0x1.c9eca0f325b42p0
  },
  { // Entry 226
    0x1.dc0aa0025b62b7fe5b0c13b7cc65bbc6p-2,
    0x1.cc6p-2
  },
  { // Entry 227
    -0x1.dc0aa0025b62b7fe5b0c13b7cc65bbc6p-2,
    -0x1.cc6p-2
  },
  { // Entry 228
    0x1.06c9ccd626cd8800020eb9e544490f6ap0,
    0x1.cccccccd69451p-1
  },
  { // Entry 229
    -0x1.06c9ccd626cd8800020eb9e544490f6ap0,
    -0x1.cccccccd69451p-1
  },
  { // Entry 230
    0x1.d862eb5cde4b0800f5b46008a4384f2ep-5,
    0x1.d82p-5
  },
  { // Entry 231
    -0x1.d862eb5cde4b0800f5b46008a4384f2ep-5,
    -0x1.d82p-5
  },
  { // Entry 232
    0x1.7b150909141397ff00cd2cb5dcdfc798p344,
    0x1.df0e8443492b4p7
  },
  { // Entry 233
    -0x1.7b150909141397ff00cd2cb5dcdfc798p344,
    -0x1.df0e8443492b4p7
  },
  { // Entry 234
    0x1.f370cce952a62835ca29e12b2844ca82p-2,
    0x1.e18p-2
  },
  { // Entry 235
    -0x1.f370cce952a62835ca29e12b2844ca82p-2,
    -0x1.e18p-2
  },
  { // Entry 236
    0x1.e6123f12db92680282920746b67b9489p-6,
    0x1.e60p-6
  },
  { // Entry 237
    -0x1.e6123f12db92680282920746b67b9489p-6,
    -0x1.e60p-6
  },
  { // Entry 238
    0x1.fc59700f23fc880595b286d94cb1161dp-2,
    0x1.e97e4ca09cde7p-2
  },
  { // Entry 239
    -0x1.fc59700f23fc880595b286d94cb1161dp-2,
    -0x1.e97e4ca09cde7p-2
  },
  { // Entry 240
    0x1.a8a3582361d82800005ec0718a5e8e7cp1,
    0x1.ea11b1afdc907p0
  },
  { // Entry 241
    -0x1.a8a3582361d82800005ec0718a5e8e7cp1,
    -0x1.ea11b1afdc907p0
  },
  { // Entry 242
    0x1.f4592b2dde8cf3813b817358e271e980p-3,
    0x1.ef8p-3
  },
  { // Entry 243
    -0x1.f4592b2dde8cf3813b817358e271e980p-3,
    -0x1.ef8p-3
  },
  { // Entry 244
    0x1.48f609e7b6aecf059a4b8c484c3bc435p21,
    0x1.effffffffffffp3
  },
  { // Entry 245
    -0x1.48f609e7b6aecf059a4b8c484c3bc435p21,
    -0x1.effffffffffffp3
  },
  { // Entry 246
    0x1.dbca9263f840fca48450e408fa36b56bp177,
    0x1.effffffffffffp6
  },
  { // Entry 247
    -0x1.dbca9263f840fca48450e408fa36b56bp177,
    -0x1.effffffffffffp6
  },
  { // Entry 248
    0x1.d3764167d84c26146f109714eda9a19ep715,
    0x1.f0e540ea02272p8
  },
  { // Entry 249
    -0x1.d3764167d84c26146f109714eda9a19ep715,
    -0x1.f0e540ea02272p8
  },
  { // Entry 250
    0x1.f44f7cd78c0c8d0cfdd38268b4c1b21ap-5,
    0x1.f3fffffffffffp-5
  },
  { // Entry 251
    -0x1.f44f7cd78c0c8d0cfdd38268b4c1b21ap-5,
    -0x1.f3fffffffffffp-5
  },
  { // Entry 252
    0x1.f4e4fe44a20e73bbcddcba0e9c46e803p-7,
    0x1.f4ep-7
  },
  { // Entry 253
    -0x1.f4e4fe44a20e73bbcddcba0e9c46e803p-7,
    -0x1.f4ep-7
  },
  { // Entry 254
    0x1.f4fd444ef4c02273e0c0b56c3b6b18f4p-11,
    0x1.f4fd3f4fd3f41p-11
  },
  { // Entry 255
    -0x1.f4fd444ef4c02273e0c0b56c3b6b18f4p-11,
    -0x1.f4fd3f4fd3f41p-11
  },
  { // Entry 256
    0x1.f776c257a56b780187bf905e80418279p-9,
    0x1.f7767134f4c72p-9
  },
  { // Entry 257
    -0x1.f776c257a56b780187bf905e80418279p-9,
    -0x1.f7767134f4c72p-9
  },
  { // Entry 258
    0x1.f78e0512e112b1c915f33b6d0094cbdfp-11,
    0x1.f78dfffffffffp-11
  },
  { // Entry 259
    -0x1.f78e0512e112b1c915f33b6d0094cbdfp-11,
    -0x1.f78dfffffffffp-11
  },
  { // Entry 260
    0x1.fd1a4703ffc8e2c09ee2d3142d879d53p-3,
    0x1.f7fffffffffffp-3
  },
  { // Entry 261
    -0x1.fd1a4703ffc8e2c09ee2d3142d879d53p-3,
    -0x1.f7fffffffffffp-3
  },
  { // Entry 262
    0x1.fd2b1321689993d9e7a4f5cc42e417e0p-3,
    0x1.f8104d180ef13p-3
  },
  { // Entry 263
    -0x1.fd2b1321689993d9e7a4f5cc42e417e0p-3,
    -0x1.f8104d180ef13p-3
  },
  { // Entry 264
    0x1.fa8247c8342301fdbf919432600eef6bp-11,
    0x1.fa82429e54867p-11
  },
  { // Entry 265
    -0x1.fa8247c8342301fdbf919432600eef6bp-11,
    -0x1.fa82429e54867p-11
  },
  { // Entry 266
    0x1.f617a27e250ce1b06488e5167c0849a8p90,
    0x1.fdfffffffffffp5
  },
  { // Entry 267
    -0x1.f617a27e250ce1b06488e5167c0849a8p90,
    -0x1.fdfffffffffffp5
  },
  { // Entry 268
    0x1.fef54cde11851d4db7938ee57e080486p-7,
    0x1.feeffffffffffp-7
  },
  { // Entry 269
    -0x1.fef54cde11851d4db7938ee57e080486p-7,
    -0x1.feeffffffffffp-7
  },
  { // Entry 270
    0x1.0a8ddcf177d1eb7da9052d4b680c93e4p-1,
    0x1.ff8ffffffffffp-2
  },
  { // Entry 271
    -0x1.0a8ddcf177d1eb7da9052d4b680c93e4p-1,
    -0x1.ff8ffffffffffp-2
  },
  { // Entry 272
    0x1.ffb5499b99f503b876c9233146b88610p-6,
    0x1.ff9ffffffffffp-6
  },
  { // Entry 273
    -0x1.ffb5499b99f503b876c9233146b88610p-6,
    -0x1.ff9ffffffffffp-6
  },
  { // Entry 274
    0x1.ffd00553d57d8679faafbb417929037bp-11,
    0x1.ffcffffffffffp-11
  },
  { // Entry 275
    -0x1.ffd00553d57d8679faafbb417929037bp-11,
    -0x1.ffcffffffffffp-11
  },
  { // Entry 276
    0x1.738796c76ddd11a4fad67bda6a85c0a3p10,
    0x1.ffcffffffffffp2
  },
  { // Entry 277
    -0x1.738796c76ddd11a4fad67bda6a85c0a3p10,
    -0x1.ffcffffffffffp2
  },
  { // Entry 278
    0x1.3fd752cfca481ff79679f71c8aad6234p91,
    0x1.ffeffffffffffp5
  },
  { // Entry 279
    -0x1.3fd752cfca481ff79679f71c8aad6234p91,
    -0x1.ffeffffffffffp5
  },
  { // Entry 280
    0x1.fffc1154d51a88ed085860dce45cd0f6p-10,
    0x1.fffbfbfffffffp-10
  },
  { // Entry 281
    -0x1.fffc1154d51a88ed085860dce45cd0f6p-10,
    -0x1.fffbfbfffffffp-10
  },
  { // Entry 282
    0x1.0acb07d19f2eec122a06e05f2c4a0de4p-1,
    0x1.fffc7ffffffffp-2
  },
  { // Entry 283
    -0x1.0acb07d19f2eec122a06e05f2c4a0de4p-1,
    -0x1.fffc7ffffffffp-2
  },
  { // Entry 284
    0x1.00a9094ee88097ff3d5673552bc2eab6p-3,
    0x1.fffc7ffffffffp-4
  },
  { // Entry 285
    -0x1.00a9094ee88097ff3d5673552bc2eab6p-3,
    -0x1.fffc7ffffffffp-4
  },
  { // Entry 286
    0x1.7495e977202177ec68508f6f5a5b9165p10,
    0x1.fffe7ffffffffp2
  },
  { // Entry 287
    -0x1.7495e977202177ec68508f6f5a5b9165p10,
    -0x1.fffe7ffffffffp2
  },
  { // Entry 288
    0x1.b4a0165d618bc7fe4fd5aa6fcac302c1p4,
    0x1.fffefffffffffp1
  },
  { // Entry 289
    -0x1.b4a0165d618bc7fe4fd5aa6fcac302c1p4,
    -0x1.fffefffffffffp1
  },
  { // Entry 290
    0x1.95e4816b60a8d769724b586e4deb3b1bp183,
    0x1.fffffdfffffffp6
  },
  { // Entry 291
    -0x1.95e4816b60a8d769724b586e4deb3b1bp183,
    -0x1.fffffdfffffffp6
  },
  { // Entry 292
    0x1.1f43fcc441800800fcf1d836d2c62f72p45,
    0x1.fffffffff97d6p4
  },
  { // Entry 293
    -0x1.1f43fcc441800800fcf1d836d2c62f72p45,
    -0x1.fffffffff97d6p4
  },
  { // Entry 294
    0x1.ffffffffffffc0p-1023,
    0x1.ffffffffffffcp-1023
  },
  { // Entry 295
    -0x1.ffffffffffffc0p-1023,
    -0x1.ffffffffffffcp-1023
  },
  { // Entry 296
    0x1.9476504ba82057f69310608c30e76cebp737,
    0x1.ffffffffffffep8
  },
  { // Entry 297
    -0x1.9476504ba82057f69310608c30e76cebp737,
    -0x1.ffffffffffffep8
  },
  { // Entry 298
    0.0,
    0.0
  },
  { // Entry 299
    0x1.24d1fdb0fdc49fa1ed2d33be6d840c75p-4,
    0x1.2492492492492p-4
  },
  { // Entry 300
    -0x1.24d1fdb0fdc49fa1ed2d33be6d840c75p-4,
    -0x1.2492492492492p-4
  },
  { // Entry 301
    0x1.25914d4754aeca5885ba953dbac88d8fp-3,
    0x1.2492492492492p-3
  },
  { // Entry 302
    -0x1.25914d4754aeca5885ba953dbac88d8fp-3,
    -0x1.2492492492492p-3
  },
  { // Entry 303
    0x1.ba3934de293068e82220c3704c5750b7p-3,
    0x1.b6db6db6db6dbp-3
  },
  { // Entry 304
    -0x1.ba3934de293068e82220c3704c5750b7p-3,
    -0x1.b6db6db6db6dbp-3
  },
  { // Entry 305
    0x1.28917a35f59b8990911b05edbc6f6ba1p-2,
    0x1.2492492492492p-2
  },
  { // Entry 306
    -0x1.28917a35f59b8990911b05edbc6f6ba1p-2,
    -0x1.2492492492492p-2
  },
  { // Entry 307
    0x1.7589dee6de0a2ec648852490e9ae54e2p-2,
    0x1.6db6db6db6db6p-2
  },
  { // Entry 308
    -0x1.7589dee6de0a2ec648852490e9ae54e2p-2,
    -0x1.6db6db6db6db6p-2
  },
  { // Entry 309
    0x1.c46a5bcd3c2c368c2591b8bba7e24256p-2,
    0x1.b6db6db6db6dap-2
  },
  { // Entry 310
    -0x1.c46a5bcd3c2c368c2591b8bba7e24256p-2,
    -0x1.b6db6db6db6dap-2
  },
  { // Entry 311
    0x1.0acd00fe63b95a9896032c78de2323c6p-1,
    0x1.ffffffffffffep-2
  },
  { // Entry 312
    -0x1.0acd00fe63b95a9896032c78de2323c6p-1,
    -0x1.ffffffffffffep-2
  },
  { // Entry 313
    0x1.0acd00fe63b96ca357895761ae66224ap-1,
    0x1.0p-1
  },
  { // Entry 314
    -0x1.0acd00fe63b96ca357895761ae66224ap-1,
    -0x1.0p-1
  },
  { // Entry 315
    0x1.34c1737f26250058df17aa0da89aa5fcp-1,
    0x1.2492492492492p-1
  },
  { // Entry 316
    -0x1.34c1737f26250058df17aa0da89aa5fcp-1,
    -0x1.2492492492492p-1
  },
  { // Entry 317
    0x1.604957b6e9223ab9f0acd5cd087d9d4bp-1,
    0x1.4924924924924p-1
  },
  { // Entry 318
    -0x1.604957b6e9223ab9f0acd5cd087d9d4bp-1,
    -0x1.4924924924924p-1
  },
  { // Entry 319
    0x1.8d9d8f1f9ecb929e09103f190149433dp-1,
    0x1.6db6db6db6db6p-1
  },
  { // Entry 320
    -0x1.8d9d8f1f9ecb929e09103f190149433dp-1,
    -0x1.6db6db6db6db6p-1
  },
  { // Entry 321
    0x1.bcf954b2503820857f4dbefa036f8e5ep-1,
    0x1.9249249249248p-1
  },
  { // Entry 322
    -0x1.bcf954b2503820857f4dbefa036f8e5ep-1,
    -0x1.9249249249248p-1
  },
  { // Entry 323
    0x1.ee9a8a4c3c72bcabcb7b2924d314efa2p-1,
    0x1.b6db6db6db6dap-1
  },
  { // Entry 324
    -0x1.ee9a8a4c3c72bcabcb7b2924d314efa2p-1,
    -0x1.b6db6db6db6dap-1
  },
  { // Entry 325
    0x1.116104c5878d4cf53fe2c1f3896ec672p0,
    0x1.db6db6db6db6cp-1
  },
  { // Entry 326
    -0x1.116104c5878d4cf53fe2c1f3896ec672p0,
    -0x1.db6db6db6db6cp-1
  },
  { // Entry 327
    0x1.2cd9fc44eb980cf78cf76e89b69a3e88p0,
    0x1.ffffffffffffep-1
  },
  { // Entry 328
    -0x1.2cd9fc44eb980cf78cf76e89b69a3e88p0,
    -0x1.ffffffffffffep-1
  },
  { // Entry 329
    0.0,
    0.0
  },
  { // Entry 330
    0x1.18e1e0472274e67654d0f855c36e861dp-6,
    0x1.18de5ab277f45p-6
  },
  { // Entry 331
    -0x1.18e1e0472274e67654d0f855c36e861dp-6,
    -0x1.18de5ab277f45p-6
  },
  { // Entry 332
    0x1.18ec712dd49a7583cfe81c5dccfd99b4p-5,
    0x1.18de5ab277f45p-5
  },
  { // Entry 333
    -0x1.18ec712dd49a7583cfe81c5dccfd99b4p-5,
    -0x1.18de5ab277f45p-5
  },
  { // Entry 334
    0x1.a57d14d0fcc06c297b511eb105c39b51p-5,
    0x1.a54d880bb3ee8p-5
  },
  { // Entry 335
    -0x1.a57d14d0fcc06c297b511eb105c39b51p-5,
    -0x1.a54d880bb3ee8p-5
  },
  { // Entry 336
    0x1.1916b72b2648d65f042e701b14a371ddp-4,
    0x1.18de5ab277f45p-4
  },
  { // Entry 337
    -0x1.1916b72b2648d65f042e701b14a371ddp-4,
    -0x1.18de5ab277f45p-4
  },
  { // Entry 338
    0x1.5f8409b4e2d18ada55848296f71484b2p-4,
    0x1.5f15f15f15f16p-4
  },
  { // Entry 339
    -0x1.5f8409b4e2d18ada55848296f71484b2p-4,
    -0x1.5f15f15f15f16p-4
  },
  { // Entry 340
    0x1.a60bce73024b0ef37175d31aa5a941e6p-4,
    0x1.a54d880bb3ee7p-4
  },
  { // Entry 341
    -0x1.a60bce73024b0ef37175d31aa5a941e6p-4,
    -0x1.a54d880bb3ee7p-4
  },
  { // Entry 342
    0x1.ecb353d02d5fc056ceb2ce9e08b8f8d5p-4,
    0x1.eb851eb851eb8p-4
  },
  { // Entry 343
    -0x1.ecb353d02d5fc056ceb2ce9e08b8f8d5p-4,
    -0x1.eb851eb851eb8p-4
  },
  { // Entry 344
    0x1.ecb353d02d5fc056ceb2ce9e08b8f8d5p-4,
    0x1.eb851eb851eb8p-4
  },
  { // Entry 345
    -0x1.ecb353d02d5fc056ceb2ce9e08b8f8d5p-4,
    -0x1.eb851eb851eb8p-4
  },
  { // Entry 346
    0x1.02243ce0549f980dc7c7d32c56687b61p-3,
    0x1.01767dce434aap-3
  },
  { // Entry 347
    -0x1.02243ce0549f980dc7c7d32c56687b61p-3,
    -0x1.01767dce434aap-3
  },
  { // Entry 348
    0x1.0df0f841fd4a69998703d2ffbf0e1544p-3,
    0x1.0d2a6c405d9f8p-3
  },
  { // Entry 349
    -0x1.0df0f841fd4a69998703d2ffbf0e1544p-3,
    -0x1.0d2a6c405d9f8p-3
  },
  { // Entry 350
    0x1.19bff54d4d1ca252438bfcad4485bbeep-3,
    0x1.18de5ab277f46p-3
  },
  { // Entry 351
    -0x1.19bff54d4d1ca252438bfcad4485bbeep-3,
    -0x1.18de5ab277f46p-3
  },
  { // Entry 352
    0x1.25914d4754aeeaac41a8543c0e5f84bfp-3,
    0x1.2492492492494p-3
  },
  { // Entry 353
    -0x1.25914d4754aeeaac41a8543c0e5f84bfp-3,
    -0x1.2492492492494p-3
  },
  { // Entry 354
    0x1.3165197a2ed9cb0dc90f9bb136a99057p-3,
    0x1.30463796ac9e2p-3
  },
  { // Entry 355
    -0x1.3165197a2ed9cb0dc90f9bb136a99057p-3,
    -0x1.30463796ac9e2p-3
  },
  { // Entry 356
    0x1.3d3b733536d3f9e48c1626776c18450fp-3,
    0x1.3bfa2608c6f30p-3
  },
  { // Entry 357
    -0x1.3d3b733536d3f9e48c1626776c18450fp-3,
    -0x1.3bfa2608c6f30p-3
  },
  { // Entry 358
    0x1.491473cd3e5bb6011680e1c412b75226p-3,
    0x1.47ae147ae147bp-3
  },
  { // Entry 359
    -0x1.491473cd3e5bb6011680e1c412b75226p-3,
    -0x1.47ae147ae147bp-3
  },
  { // Entry 360
    0x1.491473cd3e5bb6011680e1c412b75226p-3,
    0x1.47ae147ae147bp-3
  },
  { // Entry 361
    -0x1.491473cd3e5bb6011680e1c412b75226p-3,
    -0x1.47ae147ae147bp-3
  },
  { // Entry 362
    0x1.227b2f27e6efe03d72b2c311d29c7884p-2,
    0x1.1eb851eb851ecp-2
  },
  { // Entry 363
    -0x1.227b2f27e6efe03d72b2c311d29c7884p-2,
    -0x1.1eb851eb851ecp-2
  },
  { // Entry 364
    0x1.a49c41f850ed2680ee94da3183c89a52p-2,
    0x1.999999999999ap-2
  },
  { // Entry 365
    -0x1.a49c41f850ed2680ee94da3183c89a52p-2,
    -0x1.999999999999ap-2
  },
  { // Entry 366
    0x1.1666dd8c17ac9986d3cd4018364fc2ecp-1,
    0x1.0a3d70a3d70a4p-1
  },
  { // Entry 367
    -0x1.1666dd8c17ac9986d3cd4018364fc2ecp-1,
    -0x1.0a3d70a3d70a4p-1
  },
  { // Entry 368
    0x1.5e832275691f29c754a69f08a0bda060p-1,
    0x1.47ae147ae147bp-1
  },
  { // Entry 369
    -0x1.5e832275691f29c754a69f08a0bda060p-1,
    -0x1.47ae147ae147bp-1
  },
  { // Entry 370
    0x1.abad155b6751a697130d8faafe9d512cp-1,
    0x1.851eb851eb852p-1
  },
  { // Entry 371
    -0x1.abad155b6751a697130d8faafe9d512cp-1,
    -0x1.851eb851eb852p-1
  },
  { // Entry 372
    0x1.ff0182a062c855926fe6373f9b3afd58p-1,
    0x1.c28f5c28f5c29p-1
  },
  { // Entry 373
    -0x1.ff0182a062c855926fe6373f9b3afd58p-1,
    -0x1.c28f5c28f5c29p-1
  },
  { // Entry 374
    0x1.2cd9fc44eb9825a80249487f064ffd5cp0,
    0x1.0p0
  },
  { // Entry 375
    -0x1.2cd9fc44eb9825a80249487f064ffd5cp0,
    -0x1.0p0
  },
  { // Entry 376
    0x1.2cd9fc44eb9825a80249487f064ffd5cp0,
    0x1.0p0
  },
  { // Entry 377
    -0x1.2cd9fc44eb9825a80249487f064ffd5cp0,
    -0x1.0p0
  },
  { // Entry 378
    0x1.c034a7cd6ce9ade4f611fdd05d109cf6p7,
    0x1.86bc88cbf1b67p2
  },
  { // Entry 379
    -0x1.c034a7cd6ce9ade4f611fdd05d109cf6p7,
    -0x1.86bc88cbf1b67p2
  },
  { // Entry 380
    0x1.20af6cb9859eb23e91e63c28d18b0d0ap15,
    0x1.66bc88cbf1b67p3
  },
  { // Entry 381
    -0x1.20af6cb9859eb23e91e63c28d18b0d0ap15,
    -0x1.66bc88cbf1b67p3
  },
  { // Entry 382
    0x1.73e096cf57afce7adf9f1b2a216a5db5p22,
    0x1.050d6698f548dp4
  },
  { // Entry 383
    -0x1.73e096cf57afce7adf9f1b2a216a5db5p22,
    -0x1.050d6698f548dp4
  },
  { // Entry 384
    0x1.df0b13a84513e2ceb180507c54c5b904p29,
    0x1.56bc88cbf1b67p4
  },
  { // Entry 385
    -0x1.df0b13a84513e2ceb180507c54c5b904p29,
    -0x1.56bc88cbf1b67p4
  },
  { // Entry 386
    0x1.348bc1e018bc593ce2f9d1bc0f37e14fp37,
    0x1.a86baafeee241p4
  },
  { // Entry 387
    -0x1.348bc1e018bc593ce2f9d1bc0f37e14fp37,
    -0x1.a86baafeee241p4
  },
  { // Entry 388
    0x1.8d761a3398942448ea796c65748a869ep44,
    0x1.fa1acd31ea91bp4
  },
  { // Entry 389
    -0x1.8d761a3398942448ea796c65748a869ep44,
    -0x1.fa1acd31ea91bp4
  },
  { // Entry 390
    0x1.ffffffffffff9ede67b7a30e661c79e2p51,
    0x1.25e4f7b2737fap5
  },
  { // Entry 391
    -0x1.ffffffffffff9ede67b7a30e661c79e2p51,
    -0x1.25e4f7b2737fap5
  },
  { // Entry 392
    0x1.fffffffdfffeca86c3885786a2b2306fp14,
    0x1.62e42fefa39eep3
  },
  { // Entry 393
    -0x1.fffffffdfffeca86c3885786a2b2306fp14,
    -0x1.62e42fefa39eep3
  },
  { // Entry 394
    0x1.fffffffdffffca86c389578647f59234p14,
    0x1.62e42fefa39efp3
  },
  { // Entry 395
    -0x1.fffffffdffffca86c389578647f59234p14,
    -0x1.62e42fefa39efp3
  },
  { // Entry 396
    0x1.fffffffe0000ca86c38a57866d38f3f8p14,
    0x1.62e42fefa39f0p3
  },
  { // Entry 397
    -0x1.fffffffe0000ca86c38a57866d38f3f8p14,
    -0x1.62e42fefa39f0p3
  },
  { // Entry 398
    0x1.fffdffffffff6542c70828449eb21cd0p6,
    0x1.62e42fefa39eep2
  },
  { // Entry 399
    -0x1.fffdffffffff6542c70828449eb21cd0p6,
    -0x1.62e42fefa39eep2
  },
  { // Entry 400
    0x1.fffdffffffffe5434708284488030bf1p6,
    0x1.62e42fefa39efp2
  },
  { // Entry 401
    -0x1.fffdffffffffe5434708284488030bf1p6,
    -0x1.62e42fefa39efp2
  },
  { // Entry 402
    0x1.fffe000000006543c70828449153db11p6,
    0x1.62e42fefa39f0p2
  },
  { // Entry 403
    -0x1.fffe000000006543c70828449153db11p6,
    -0x1.62e42fefa39f0p2
  },
  { // Entry 404
    0x1.fdffffffffffb254529345a3261b51eep2,
    0x1.62e42fefa39eep1
  },
  { // Entry 405
    -0x1.fdffffffffffb254529345a3261b51eep2,
    -0x1.62e42fefa39eep1
  },
  { // Entry 406
    0x1.fdfffffffffff294529345a3207533d4p2,
    0x1.62e42fefa39efp1
  },
  { // Entry 407
    -0x1.fdfffffffffff294529345a3207533d4p2,
    -0x1.62e42fefa39efp1
  },
  { // Entry 408
    0x1.fe000000000032d4529345a322c715bap2,
    0x1.62e42fefa39f0p1
  },
  { // Entry 409
    -0x1.fe000000000032d4529345a322c715bap2,
    -0x1.62e42fefa39f0p1
  },
  { // Entry 410
    0x1.dfffffffffffd6e5e5f844b9f096239ep0,
    0x1.62e42fefa39eep0
  },
  { // Entry 411
    -0x1.dfffffffffffd6e5e5f844b9f096239ep0,
    -0x1.62e42fefa39eep0
  },
  { // Entry 412
    0x1.dffffffffffff8e5e5f844b9ef41e04cp0,
    0x1.62e42fefa39efp0
  },
  { // Entry 413
    -0x1.dffffffffffff8e5e5f844b9ef41e04cp0,
    -0x1.62e42fefa39efp0
  },
  { // Entry 414
    0x1.e000000000001ae5e5f844b9efcd9cfbp0,
    0x1.62e42fefa39f0p0
  },
  { // Entry 415
    -0x1.e000000000001ae5e5f844b9efcd9cfbp0,
    -0x1.62e42fefa39f0p0
  },
  { // Entry 416
    0x1.7fffffffffffe7d28746bf03f666bce4p-1,
    0x1.62e42fefa39eep-1
  },
  { // Entry 417
    -0x1.7fffffffffffe7d28746bf03f666bce4p-1,
    -0x1.62e42fefa39eep-1
  },
  { // Entry 418
    0x1.7ffffffffffffbd28746bf03f622af6ep-1,
    0x1.62e42fefa39efp-1
  },
  { // Entry 419
    -0x1.7ffffffffffffbd28746bf03f622af6ep-1,
    -0x1.62e42fefa39efp-1
  },
  { // Entry 420
    0x1.8000000000000fd28746bf03f63ea1f7p-1,
    0x1.62e42fefa39f0p-1
  },
  { // Entry 421
    -0x1.8000000000000fd28746bf03f63ea1f7p-1,
    -0x1.62e42fefa39f0p-1
  },
  { // Entry 422
    0x1.6a09e667f3bcb484c2238ce481384c44p-2,
    0x1.62e42fefa39eep-2
  },
  { // Entry 423
    -0x1.6a09e667f3bcb484c2238ce481384c44p-2,
    -0x1.62e42fefa39eep-2
  },
  { // Entry 424
    0x1.6a09e667f3bcc57d38f06c515a94aa60p-2,
    0x1.62e42fefa39efp-2
  },
  { // Entry 425
    -0x1.6a09e667f3bcc57d38f06c515a94aa60p-2,
    -0x1.62e42fefa39efp-2
  },
  { // Entry 426
    0x1.6a09e667f3bcd675afbd4bbe3407a91bp-2,
    0x1.62e42fefa39f0p-2
  },
  { // Entry 427
    -0x1.6a09e667f3bcd675afbd4bbe3407a91bp-2,
    -0x1.62e42fefa39f0p-2
  },
  { // Entry 428
    0x1.64ab8f61134f8c07335ae0143f1aa9e7p-3,
    0x1.62e42fefa39eep-3
  },
  { // Entry 429
    -0x1.64ab8f61134f8c07335ae0143f1aa9e7p-3,
    -0x1.62e42fefa39eep-3
  },
  { // Entry 430
    0x1.64ab8f61134f9c44da464fa4b5054cc6p-3,
    0x1.62e42fefa39efp-3
  },
  { // Entry 431
    -0x1.64ab8f61134f9c44da464fa4b5054cc6p-3,
    -0x1.62e42fefa39efp-3
  },
  { // Entry 432
    0x1.64ab8f61134fac828131bf352af58253p-3,
    0x1.62e42fefa39f0p-3
  },
  { // Entry 433
    -0x1.64ab8f61134fac828131bf352af58253p-3,
    -0x1.62e42fefa39f0p-3
  },
  { // Entry 434
    0x1.6355e6ffbf9b918f1018c6864c912c0bp-4,
    0x1.62e42fefa39eep-4
  },
  { // Entry 435
    -0x1.6355e6ffbf9b918f1018c6864c912c0bp-4,
    -0x1.62e42fefa39eep-4
  },
  { // Entry 436
    0x1.6355e6ffbf9ba19e726e44182bc81666p-4,
    0x1.62e42fefa39efp-4
  },
  { // Entry 437
    -0x1.6355e6ffbf9ba19e726e44182bc81666p-4,
    -0x1.62e42fefa39efp-4
  },
  { // Entry 438
    0x1.6355e6ffbf9bb1add4c3c1aa0b006418p-4,
    0x1.62e42fefa39f0p-4
  },
  { // Entry 439
    -0x1.6355e6ffbf9bb1add4c3c1aa0b006418p-4,
    -0x1.62e42fefa39f0p-4
  },
  { // Entry 440
    0x1.63009ba740a28c74c5e39abbcb6d3e54p-5,
    0x1.62e42fefa39eep-5
  },
  { // Entry 441
    -0x1.63009ba740a28c74c5e39abbcb6d3e54p-5,
    -0x1.62e42fefa39eep-5
  },
  { // Entry 442
    0x1.63009ba740a29c789e02c0a4897fafa4p-5,
    0x1.62e42fefa39efp-5
  },
  { // Entry 443
    -0x1.63009ba740a29c789e02c0a4897fafa4p-5,
    -0x1.62e42fefa39efp-5
  },
  { // Entry 444
    0x1.63009ba740a2ac7c7621e68d479279b3p-5,
    0x1.62e42fefa39f0p-5
  },
  { // Entry 445
    -0x1.63009ba740a2ac7c7621e68d479279b3p-5,
    -0x1.62e42fefa39f0p-5
  },
  { // Entry 446
    0x1.62eb4abcc5a7fb2748b0114da83216c8p-6,
    0x1.62e42fefa39eep-6
  },
  { // Entry 447
    -0x1.62eb4abcc5a7fb2748b0114da83216c8p-6,
    -0x1.62e42fefa39eep-6
  },
  { // Entry 448
    0x1.62eb4abcc5a80b283eb077a1b2694ed2p-6,
    0x1.62e42fefa39efp-6
  },
  { // Entry 449
    -0x1.62eb4abcc5a80b283eb077a1b2694ed2p-6,
    -0x1.62e42fefa39efp-6
  },
  { // Entry 450
    0x1.62eb4abcc5a81b2934b0ddf5bca09d0bp-6,
    0x1.62e42fefa39f0p-6
  },
  { // Entry 451
    -0x1.62eb4abcc5a81b2934b0ddf5bca09d0bp-6,
    -0x1.62e42fefa39f0p-6
  },
  { // Entry 452
    -0x1.000000000000ca85c3898cffd1be6e61p31,
    -0x1.62e42fefa39f0p4
  },
  { // Entry 453
    0x1.000000000000ca85c3898cffd1be6e61p31,
    0x1.62e42fefa39f0p4
  },
  { // Entry 454
    -0x1.ffffffffffff950b871319ff0e6d55b0p30,
    -0x1.62e42fefa39efp4
  },
  { // Entry 455
    0x1.ffffffffffff950b871319ff0e6d55b0p30,
    0x1.62e42fefa39efp4
  },
  { // Entry 456
    -0x1.fffffffffffd950b87131a00795dce9dp30,
    -0x1.62e42fefa39eep4
  },
  { // Entry 457
    0x1.fffffffffffd950b87131a00795dce9dp30,
    0x1.62e42fefa39eep4
  },
  { // Entry 458
    -0x1.fffffffe0000ca86c38a57866d38f3f8p14,
    -0x1.62e42fefa39f0p3
  },
  { // Entry 459
    0x1.fffffffe0000ca86c38a57866d38f3f8p14,
    0x1.62e42fefa39f0p3
  },
  { // Entry 460
    -0x1.fffffffdffffca86c389578647f59234p14,
    -0x1.62e42fefa39efp3
  },
  { // Entry 461
    0x1.fffffffdffffca86c389578647f59234p14,
    0x1.62e42fefa39efp3
  },
  { // Entry 462
    -0x1.fffffffdfffeca86c3885786a2b2306fp14,
    -0x1.62e42fefa39eep3
  },
  { // Entry 463
    0x1.fffffffdfffeca86c3885786a2b2306fp14,
    0x1.62e42fefa39eep3
  },
  { // Entry 464
    -0x1.fffe000000006543c70828449153db11p6,
    -0x1.62e42fefa39f0p2
  },
  { // Entry 465
    0x1.fffe000000006543c70828449153db11p6,
    0x1.62e42fefa39f0p2
  },
  { // Entry 466
    -0x1.fffdffffffffe5434708284488030bf1p6,
    -0x1.62e42fefa39efp2
  },
  { // Entry 467
    0x1.fffdffffffffe5434708284488030bf1p6,
    0x1.62e42fefa39efp2
  },
  { // Entry 468
    -0x1.fffdffffffff6542c70828449eb21cd0p6,
    -0x1.62e42fefa39eep2
  },
  { // Entry 469
    0x1.fffdffffffff6542c70828449eb21cd0p6,
    0x1.62e42fefa39eep2
  },
  { // Entry 470
    -0x1.fe000000000032d4529345a322c715bap2,
    -0x1.62e42fefa39f0p1
  },
  { // Entry 471
    0x1.fe000000000032d4529345a322c715bap2,
    0x1.62e42fefa39f0p1
  },
  { // Entry 472
    -0x1.fdfffffffffff294529345a3207533d4p2,
    -0x1.62e42fefa39efp1
  },
  { // Entry 473
    0x1.fdfffffffffff294529345a3207533d4p2,
    0x1.62e42fefa39efp1
  },
  { // Entry 474
    -0x1.fdffffffffffb254529345a3261b51eep2,
    -0x1.62e42fefa39eep1
  },
  { // Entry 475
    0x1.fdffffffffffb254529345a3261b51eep2,
    0x1.62e42fefa39eep1
  },
  { // Entry 476
    -0x1.e000000000001ae5e5f844b9efcd9cfbp0,
    -0x1.62e42fefa39f0p0
  },
  { // Entry 477
    0x1.e000000000001ae5e5f844b9efcd9cfbp0,
    0x1.62e42fefa39f0p0
  },
  { // Entry 478
    -0x1.dffffffffffff8e5e5f844b9ef41e04cp0,
    -0x1.62e42fefa39efp0
  },
  { // Entry 479
    0x1.dffffffffffff8e5e5f844b9ef41e04cp0,
    0x1.62e42fefa39efp0
  },
  { // Entry 480
    -0x1.dfffffffffffd6e5e5f844b9f096239ep0,
    -0x1.62e42fefa39eep0
  },
  { // Entry 481
    0x1.dfffffffffffd6e5e5f844b9f096239ep0,
    0x1.62e42fefa39eep0
  },
  { // Entry 482
    -0x1.8000000000000fd28746bf03f63ea1f7p-1,
    -0x1.62e42fefa39f0p-1
  },
  { // Entry 483
    0x1.8000000000000fd28746bf03f63ea1f7p-1,
    0x1.62e42fefa39f0p-1
  },
  { // Entry 484
    -0x1.7ffffffffffffbd28746bf03f622af6ep-1,
    -0x1.62e42fefa39efp-1
  },
  { // Entry 485
    0x1.7ffffffffffffbd28746bf03f622af6ep-1,
    0x1.62e42fefa39efp-1
  },
  { // Entry 486
    -0x1.7fffffffffffe7d28746bf03f666bce4p-1,
    -0x1.62e42fefa39eep-1
  },
  { // Entry 487
    0x1.7fffffffffffe7d28746bf03f666bce4p-1,
    0x1.62e42fefa39eep-1
  },
  { // Entry 488
    -0x1.6a09e667f3bcd675afbd4bbe3407a91bp-2,
    -0x1.62e42fefa39f0p-2
  },
  { // Entry 489
    0x1.6a09e667f3bcd675afbd4bbe3407a91bp-2,
    0x1.62e42fefa39f0p-2
  },
  { // Entry 490
    -0x1.6a09e667f3bcc57d38f06c515a94aa60p-2,
    -0x1.62e42fefa39efp-2
  },
  { // Entry 491
    0x1.6a09e667f3bcc57d38f06c515a94aa60p-2,
    0x1.62e42fefa39efp-2
  },
  { // Entry 492
    -0x1.6a09e667f3bcb484c2238ce481384c44p-2,
    -0x1.62e42fefa39eep-2
  },
  { // Entry 493
    0x1.6a09e667f3bcb484c2238ce481384c44p-2,
    0x1.62e42fefa39eep-2
  },
  { // Entry 494
    -0x1.64ab8f61134fac828131bf352af58253p-3,
    -0x1.62e42fefa39f0p-3
  },
  { // Entry 495
    0x1.64ab8f61134fac828131bf352af58253p-3,
    0x1.62e42fefa39f0p-3
  },
  { // Entry 496
    -0x1.64ab8f61134f9c44da464fa4b5054cc6p-3,
    -0x1.62e42fefa39efp-3
  },
  { // Entry 497
    0x1.64ab8f61134f9c44da464fa4b5054cc6p-3,
    0x1.62e42fefa39efp-3
  },
  { // Entry 498
    -0x1.64ab8f61134f8c07335ae0143f1aa9e7p-3,
    -0x1.62e42fefa39eep-3
  },
  { // Entry 499
    0x1.64ab8f61134f8c07335ae0143f1aa9e7p-3,
    0x1.62e42fefa39eep-3
  },
  { // Entry 500
    -0x1.6355e6ffbf9bb1add4c3c1aa0b006418p-4,
    -0x1.62e42fefa39f0p-4
  },
  { // Entry 501
    0x1.6355e6ffbf9bb1add4c3c1aa0b006418p-4,
    0x1.62e42fefa39f0p-4
  },
  { // Entry 502
    -0x1.6355e6ffbf9ba19e726e44182bc81666p-4,
    -0x1.62e42fefa39efp-4
  },
  { // Entry 503
    0x1.6355e6ffbf9ba19e726e44182bc81666p-4,
    0x1.62e42fefa39efp-4
  },
  { // Entry 504
    -0x1.6355e6ffbf9b918f1018c6864c912c0bp-4,
    -0x1.62e42fefa39eep-4
  },
  { // Entry 505
    0x1.6355e6ffbf9b918f1018c6864c912c0bp-4,
    0x1.62e42fefa39eep-4
  },
  { // Entry 506
    -0x1.63009ba740a2ac7c7621e68d479279b3p-5,
    -0x1.62e42fefa39f0p-5
  },
  { // Entry 507
    0x1.63009ba740a2ac7c7621e68d479279b3p-5,
    0x1.62e42fefa39f0p-5
  },
  { // Entry 508
    -0x1.63009ba740a29c789e02c0a4897fafa4p-5,
    -0x1.62e42fefa39efp-5
  },
  { // Entry 509
    0x1.63009ba740a29c789e02c0a4897fafa4p-5,
    0x1.62e42fefa39efp-5
  },
  { // Entry 510
    -0x1.63009ba740a28c74c5e39abbcb6d3e54p-5,
    -0x1.62e42fefa39eep-5
  },
  { // Entry 511
    0x1.63009ba740a28c74c5e39abbcb6d3e54p-5,
    0x1.62e42fefa39eep-5
  },
  { // Entry 512
    0x1.bfeb3206958461e0cd949b740397374bp262,
    0x1.6db6db6db6db7p7
  },
  { // Entry 513
    -0x1.bfeb3206958461e0cd949b740397374bp262,
    -0x1.6db6db6db6db7p7
  },
  { // Entry 514
    0x1.ee4adffc4816c196cc85c579b49b713cp341,
    0x1.db6db6db6db6ep7
  },
  { // Entry 515
    -0x1.ee4adffc4816c196cc85c579b49b713cp341,
    -0x1.db6db6db6db6ep7
  },
  { // Entry 516
    0x1.10bbd304e4d53317191db80168f41e88p421,
    0x1.2492492492492p8
  },
  { // Entry 517
    -0x1.10bbd304e4d53317191db80168f41e88p421,
    -0x1.2492492492492p8
  },
  { // Entry 518
    0x1.2cf8621aa3eacbce3c5c6aac7a52e872p500,
    0x1.5b6db6db6db6dp8
  },
  { // Entry 519
    -0x1.2cf8621aa3eacbce3c5c6aac7a52e872p500,
    -0x1.5b6db6db6db6dp8
  },
  { // Entry 520
    0x1.4c21539572c19b59fc629129d307d9b1p579,
    0x1.9249249249248p8
  },
  { // Entry 521
    -0x1.4c21539572c19b59fc629129d307d9b1p579,
    -0x1.9249249249248p8
  },
  { // Entry 522
    0x1.6e8422b4db33b6293cd44cd1c65585b2p658,
    0x1.c924924924923p8
  },
  { // Entry 523
    -0x1.6e8422b4db33b6293cd44cd1c65585b2p658,
    -0x1.c924924924923p8
  },
  { // Entry 524
    0x1.6dde4c855f3397cd05f383e2ad5ef219p935,
    0x1.4492492492492p9
  },
  { // Entry 525
    -0x1.6dde4c855f3397cd05f383e2ad5ef219p935,
    -0x1.4492492492492p9
  },
  { // Entry 526
    0x1.a178d253fc35a0b9802d9cd5f67cb7efp948,
    0x1.4924924924924p9
  },
  { // Entry 527
    -0x1.a178d253fc35a0b9802d9cd5f67cb7efp948,
    -0x1.4924924924924p9
  },
  { // Entry 528
    0x1.dc5a9c97ea13a0062828386dc2460ac2p961,
    0x1.4db6db6db6db6p9
  },
  { // Entry 529
    -0x1.dc5a9c97ea13a0062828386dc2460ac2p961,
    -0x1.4db6db6db6db6p9
  },
  { // Entry 530
    0x1.0fc53c727155d9dd001733d4258e3203p975,
    0x1.5249249249248p9
  },
  { // Entry 531
    -0x1.0fc53c727155d9dd001733d4258e3203p975,
    -0x1.5249249249248p9
  },
  { // Entry 532
    0x1.361a22f5879a158106bee1e89ea2a4d7p988,
    0x1.56db6db6db6dap9
  },
  { // Entry 533
    -0x1.361a22f5879a158106bee1e89ea2a4d7p988,
    -0x1.56db6db6db6dap9
  },
  { // Entry 534
    0x1.61d716eca93811f8d8288649dc2cee65p1001,
    0x1.5b6db6db6db6cp9
  },
  { // Entry 535
    -0x1.61d716eca93811f8d8288649dc2cee65p1001,
    -0x1.5b6db6db6db6cp9
  },
  { // Entry 536
    HUGE_VAL,
    0x1.76db6db6db6dbp9
  },
  { // Entry 537
    -HUGE_VAL,
    -0x1.76db6db6db6dbp9
  },
  { // Entry 538
    HUGE_VAL,
    0x1.8db6db6db6db6p9
  },
  { // Entry 539
    -HUGE_VAL,
    -0x1.8db6db6db6db6p9
  },
  { // Entry 540
    HUGE_VAL,
    0x1.a492492492491p9
  },
  { // Entry 541
    -HUGE_VAL,
    -0x1.a492492492491p9
  },
  { // Entry 542
    HUGE_VAL,
    0x1.bb6db6db6db6cp9
  },
  { // Entry 543
    -HUGE_VAL,
    -0x1.bb6db6db6db6cp9
  },
  { // Entry 544
    HUGE_VAL,
    0x1.d249249249247p9
  },
  { // Entry 545
    -HUGE_VAL,
    -0x1.d249249249247p9
  },
  { // Entry 546
    HUGE_VAL,
    0x1.e924924924922p9
  },
  { // Entry 547
    -HUGE_VAL,
    -0x1.e924924924922p9
  },
  { // Entry 548
    -0x1.93bf4ec282efb320a57f9ae02e01ae51p1014,
    -0x1.6p9
  },
  { // Entry 549
    0x1.93bf4ec282efb320a57f9ae02e01ae51p1014,
    0x1.6p9
  },
  { // Entry 550
    -0x1.61d716eca99087be9352df5d131a5dd2p1001,
    -0x1.5b6db6db6db6ep9
  },
  { // Entry 551
    0x1.61d716eca99087be9352df5d131a5dd2p1001,
    0x1.5b6db6db6db6ep9
  },
  { // Entry 552
    -0x1.361a22f587e79c09c420d21ecffc00cdp988,
    -0x1.56db6db6db6dcp9
  },
  { // Entry 553
    0x1.361a22f587e79c09c420d21ecffc00cdp988,
    0x1.56db6db6db6dcp9
  },
  { // Entry 554
    -0x1.0fc53c727199cb2c1cb391c8c6b1cb5bp975,
    -0x1.524924924924ap9
  },
  { // Entry 555
    0x1.0fc53c727199cb2c1cb391c8c6b1cb5bp975,
    0x1.524924924924ap9
  },
  { // Entry 556
    -0x1.dc5a9c97ea8ab6ad4e22cc3898b4d422p961,
    -0x1.4db6db6db6db8p9
  },
  { // Entry 557
    0x1.dc5a9c97ea8ab6ad4e22cc3898b4d422p961,
    0x1.4db6db6db6db8p9
  },
  { // Entry 558
    -0x1.a178d253fc9dfeee152cb749eb6f6339p948,
    -0x1.4924924924926p9
  },
  { // Entry 559
    0x1.a178d253fc9dfeee152cb749eb6f6339p948,
    0x1.4924924924926p9
  },
  { // Entry 560
    -0x1.6dde4c855f8f0f60274b5c37930499f5p935,
    -0x1.4492492492494p9
  },
  { // Entry 561
    0x1.6dde4c855f8f0f60274b5c37930499f5p935,
    0x1.4492492492494p9
  },
  { // Entry 562
    -0x1.40a4b9c271c8c4271fbcc3df336e0edbp922,
    -0x1.4000000000002p9
  },
  { // Entry 563
    0x1.40a4b9c271c8c4271fbcc3df336e0edbp922,
    0x1.4000000000002p9
  },
  { // Entry 564
    -0x1.p-1074,
    -0x1.0p-1074
  },
  { // Entry 565
    0x1.p-1074,
    0x1.0p-1074
  },
  { // Entry 566
    -0.0,
    -0.0
  },
  { // Entry 567
    0x1.p-1074,
    0x1.0p-1074
  },
  { // Entry 568
    -0x1.p-1074,
    -0x1.0p-1074
  },
  { // Entry 569
    0x1.ecb353d02d5fb03947e320c5bccaac3fp-4,
    0x1.eb851eb851eb7p-4
  },
  { // Entry 570
    -0x1.ecb353d02d5fb03947e320c5bccaac3fp-4,
    -0x1.eb851eb851eb7p-4
  },
  { // Entry 571
    0x1.ecb353d02d5fc056ceb2ce9e08b8f8d5p-4,
    0x1.eb851eb851eb8p-4
  },
  { // Entry 572
    -0x1.ecb353d02d5fc056ceb2ce9e08b8f8d5p-4,
    -0x1.eb851eb851eb8p-4
  },
  { // Entry 573
    0x1.ecb353d02d5fd07455827c7654a9321fp-4,
    0x1.eb851eb851eb9p-4
  },
  { // Entry 574
    -0x1.ecb353d02d5fd07455827c7654a9321fp-4,
    -0x1.eb851eb851eb9p-4
  },
  { // Entry 575
    0x1.0acd00fe63b9639df6c641ed463c4ca0p-1,
    0x1.fffffffffffffp-2
  },
  { // Entry 576
    -0x1.0acd00fe63b9639df6c641ed463c4ca0p-1,
    -0x1.fffffffffffffp-2
  },
  { // Entry 577
    0x1.0acd00fe63b96ca357895761ae66224ap-1,
    0x1.0p-1
  },
  { // Entry 578
    -0x1.0acd00fe63b96ca357895761ae66224ap-1,
    -0x1.0p-1
  },
  { // Entry 579
    0x1.0acd00fe63b97eae190f824a7eebd40dp-1,
    0x1.0000000000001p-1
  },
  { // Entry 580
    -0x1.0acd00fe63b97eae190f824a7eebd40dp-1,
    -0x1.0000000000001p-1
  },
  { // Entry 581
    0x1.2cd9fc44eb98194fc7a05b845e4f82b3p0,
    0x1.fffffffffffffp-1
  },
  { // Entry 582
    -0x1.2cd9fc44eb98194fc7a05b845e4f82b3p0,
    -0x1.fffffffffffffp-1
  },
  { // Entry 583
    0x1.2cd9fc44eb9825a80249487f064ffd5cp0,
    0x1.0p0
  },
  { // Entry 584
    -0x1.2cd9fc44eb9825a80249487f064ffd5cp0,
    -0x1.0p0
  },
  { // Entry 585
    0x1.2cd9fc44eb983e58779b22745732962dp0,
    0x1.0000000000001p0
  },
  { // Entry 586
    -0x1.2cd9fc44eb983e58779b22745732962dp0,
    -0x1.0000000000001p0
  },
  { // Entry 587
    0x1.ab5adb9c435e4cbe72415713a64f66a1p30,
    0x1.5ffffffffffffp4
  },
  { // Entry 588
    -0x1.ab5adb9c435e4cbe72415713a64f66a1p30,
    -0x1.5ffffffffffffp4
  },
  { // Entry 589
    0x1.ab5adb9c435ff8194ddd9a72c8c01183p30,
    0x1.6p4
  },
  { // Entry 590
    -0x1.ab5adb9c435ff8194ddd9a72c8c01183p30,
    -0x1.6p4
  },
  { // Entry 591
    0x1.ab5adb9c4361a3742979ddd3968b9801p30,
    0x1.6000000000001p4
  },
  { // Entry 592
    -0x1.ab5adb9c4361a3742979ddd3968b9801p30,
    -0x1.6000000000001p4
  },
  { // Entry 593
    0x1.226af33b1fdae7ec593b8b45e80e54d2p32,
    0x1.6ffffffffffffp4
  },
  { // Entry 594
    -0x1.226af33b1fdae7ec593b8b45e80e54d2p32,
    -0x1.6ffffffffffffp4
  },
  { // Entry 595
    0x1.226af33b1fdc0a574c76ab2161309880p32,
    0x1.7p4
  },
  { // Entry 596
    -0x1.226af33b1fdc0a574c76ab2161309880p32,
    -0x1.7p4
  },
  { // Entry 597
    0x1.226af33b1fdd2cc23fb1cafdfcbdcf69p32,
    0x1.7000000000001p4
  },
  { // Entry 598
    -0x1.226af33b1fdd2cc23fb1cafdfcbdcf69p32,
    -0x1.7000000000001p4
  },
  { // Entry 599
    0x1.fffffffffffb9ede67b7a313285faa73p51,
    0x1.25e4f7b2737f9p5
  },
  { // Entry 600
    -0x1.fffffffffffb9ede67b7a313285faa73p51,
    -0x1.25e4f7b2737f9p5
  },
  { // Entry 601
    0x1.ffffffffffff9ede67b7a30e661c79e2p51,
    0x1.25e4f7b2737fap5
  },
  { // Entry 602
    -0x1.ffffffffffff9ede67b7a30e661c79e2p51,
    -0x1.25e4f7b2737fap5
  },
  { // Entry 603
    0x1.000000000001cf6f33dbd188d1eca4a9p52,
    0x1.25e4f7b2737fbp5
  },
  { // Entry 604
    -0x1.000000000001cf6f33dbd188d1eca4a9p52,
    -0x1.25e4f7b2737fbp5
  },
  { // Entry 605
    0x1.6a09e667f3b73b2e9b132d5142f3e5b3p52,
    0x1.28aac01252c6cp5
  },
  { // Entry 606
    -0x1.6a09e667f3b73b2e9b132d5142f3e5b3p52,
    -0x1.28aac01252c6cp5
  },
  { // Entry 607
    0x1.6a09e667f3ba0f4267e314c28d64e8a9p52,
    0x1.28aac01252c6dp5
  },
  { // Entry 608
    -0x1.6a09e667f3ba0f4267e314c28d64e8a9p52,
    -0x1.28aac01252c6dp5
  },
  { // Entry 609
    0x1.6a09e667f3bce35634b2fc397ffd853fp52,
    0x1.28aac01252c6ep5
  },
  { // Entry 610
    -0x1.6a09e667f3bce35634b2fc397ffd853fp52,
    -0x1.28aac01252c6ep5
  },
  { // Entry 611
    0x1.ffffffffff93ae594e9be425a010bdecp1023,
    0x1.633ce8fb9f87cp9
  },
  { // Entry 612
    -0x1.ffffffffff93ae594e9be425a010bdecp1023,
    -0x1.633ce8fb9f87cp9
  },
  { // Entry 613
    0x1.ffffffffffd3ae594e9bda9b6b3a9168p1023,
    0x1.633ce8fb9f87dp9
  },
  { // Entry 614
    -0x1.ffffffffffd3ae594e9bda9b6b3a9168p1023,
    -0x1.633ce8fb9f87dp9
  },
  { // Entry 615
    HUGE_VAL,
    0x1.633ce8fb9f87ep9
  },
  { // Entry 616
    -HUGE_VAL,
    -0x1.633ce8fb9f87ep9
  },
  { // Entry 617
    -HUGE_VAL,
    -0x1.633ce8fb9f87ep9
  },
  { // Entry 618
    HUGE_VAL,
    0x1.633ce8fb9f87ep9
  },
  { // Entry 619
    -0x1.ffffffffffd3ae594e9bda9b6b3a9168p1023,
    -0x1.633ce8fb9f87dp9
  },
  { // Entry 620
    0x1.ffffffffffd3ae594e9bda9b6b3a9168p1023,
    0x1.633ce8fb9f87dp9
  },
  { // Entry 621
    -0x1.ffffffffff93ae594e9be425a010bdecp1023,
    -0x1.633ce8fb9f87cp9
  },
  { // Entry 622
    0x1.ffffffffff93ae594e9be425a010bdecp1023,
    0x1.633ce8fb9f87cp9
  },
  { // Entry 623
    0x1.fffffffffffff005555555555554d559p-31,
    0x1.fffffffffffffp-31
  },
  { // Entry 624
    -0x1.fffffffffffff005555555555554d559p-31,
    -0x1.fffffffffffffp-31
  },
  { // Entry 625
    0x1.0000000000000002aaaaaaaaaaaaaaacp-30,
    0x1.0p-30
  },
  { // Entry 626
    -0x1.0000000000000002aaaaaaaaaaaaaaacp-30,
    -0x1.0p-30
  },
  { // Entry 627
    0x1.0000000000001002aaaaaaaaaaab2aacp-30,
    0x1.0000000000001p-30
  },
  { // Entry 628
    -0x1.0000000000001002aaaaaaaaaaab2aacp-30,
    -0x1.0000000000001p-30
  },
  { // Entry 629
    0x1.00000000aaaaa2aaccccbcccd00cfb7ap-15,
    0x1.fffffffffffffp-16
  },
  { // Entry 630
    -0x1.00000000aaaaa2aaccccbcccd00cfb7ap-15,
    -0x1.fffffffffffffp-16
  },
  { // Entry 631
    0x1.00000000aaaaaaaaccccccccd00d00d0p-15,
    0x1.0p-15
  },
  { // Entry 632
    -0x1.00000000aaaaaaaaccccccccd00d00d0p-15,
    -0x1.0p-15
  },
  { // Entry 633
    0x1.00000000aaaabaaaccccecccd00d0b7ap-15,
    0x1.0000000000001p-15
  },
  { // Entry 634
    -0x1.00000000aaaabaaaccccecccd00d0b7ap-15,
    -0x1.0000000000001p-15
  },
  { // Entry 635
    0x1.0002aaaccccd94d9bbd8527c599a8bc7p-6,
    0x1.fffffffffffffp-7
  },
  { // Entry 636
    -0x1.0002aaaccccd94d9bbd8527c599a8bc7p-6,
    -0x1.fffffffffffffp-7
  },
  { // Entry 637
    0x1.0002aaaccccd9cd9fbd8a7d1dc72c44bp-6,
    0x1.0p-6
  },
  { // Entry 638
    -0x1.0002aaaccccd9cd9fbd8a7d1dc72c44bp-6,
    -0x1.0p-6
  },
  { // Entry 639
    0x1.0002aaaccccdacda7bd9527ce2234152p-6,
    0x1.0000000000001p-6
  },
  { // Entry 640
    -0x1.0002aaaccccdacda7bd9527ce2234152p-6,
    -0x1.0000000000001p-6
  },
  { // Entry 641
    0x1.000aaacccd00c83a3cace89e1977724dp-5,
    0x1.fffffffffffffp-6
  },
  { // Entry 642
    -0x1.000aaacccd00c83a3cace89e1977724dp-5,
    -0x1.fffffffffffffp-6
  },
  { // Entry 643
    0x1.000aaacccd00d03b3cb23dfecf8fcbdcp-5,
    0x1.0p-5
  },
  { // Entry 644
    -0x1.000aaacccd00d03b3cb23dfecf8fcbdcp-5,
    -0x1.0p-5
  },
  { // Entry 645
    0x1.000aaacccd00e03d3cbce8c03bc0aefcp-5,
    0x1.0000000000001p-5
  },
  { // Entry 646
    -0x1.000aaacccd00e03d3cbce8c03bc0aefcp-5,
    -0x1.0000000000001p-5
  },
  { // Entry 647
    0x1.002aacccd9cdc312002bf56151115c11p-4,
    0x1.fffffffffffffp-5
  },
  { // Entry 648
    -0x1.002aacccd9cdc312002bf56151115c11p-4,
    -0x1.fffffffffffffp-5
  },
  { // Entry 649
    0x1.002aacccd9cdcb1600814d8ee0ea5e98p-4,
    0x1.0p-4
  },
  { // Entry 650
    -0x1.002aacccd9cdcb1600814d8ee0ea5e98p-4,
    -0x1.0p-4
  },
  { // Entry 651
    0x1.002aacccd9cddb1e012bfdea009d23c7p-4,
    0x1.0000000000001p-4
  },
  { // Entry 652
    -0x1.002aacccd9cddb1e012bfdea009d23c7p-4,
    -0x1.0000000000001p-4
  },
  { // Entry 653
    0x1.00aaccd00d2f0572b58768290cca24c3p-3,
    0x1.fffffffffffffp-4
  },
  { // Entry 654
    -0x1.00aaccd00d2f0572b58768290cca24c3p-3,
    -0x1.fffffffffffffp-4
  },
  { // Entry 655
    0x1.00aaccd00d2f0d82badd7396c439091ep-3,
    0x1.0p-3
  },
  { // Entry 656
    -0x1.00aaccd00d2f0d82badd7396c439091ep-3,
    -0x1.0p-3
  },
  { // Entry 657
    0x1.00aaccd00d2f1da2c5898a723319d3d5p-3,
    0x1.0000000000001p-3
  },
  { // Entry 658
    -0x1.00aaccd00d2f1da2c5898a723319d3d5p-3,
    -0x1.0000000000001p-3
  },
  { // Entry 659
    0x1.02accd9d081016261f4b0ecbebb5dd8fp-2,
    0x1.fffffffffffffp-3
  },
  { // Entry 660
    -0x1.02accd9d081016261f4b0ecbebb5dd8fp-2,
    -0x1.fffffffffffffp-3
  },
  { // Entry 661
    0x1.02accd9d08101e6674cdf3fc8eaabf2ap-2,
    0x1.0p-2
  },
  { // Entry 662
    -0x1.02accd9d08101e6674cdf3fc8eaabf2ap-2,
    -0x1.0p-2
  },
  { // Entry 663
    0x1.02accd9d08102ee71fd3be5dd4a0a27ap-2,
    0x1.0000000000001p-2
  },
  { // Entry 664
    -0x1.02accd9d08102ee71fd3be5dd4a0a27ap-2,
    -0x1.0000000000001p-2
  },
  { // Entry 665
    0x1.d03cf63b6e19d8da527239bc64c85ceap1,
    0x1.fffffffffffffp0
  },
  { // Entry 666
    -0x1.d03cf63b6e19d8da527239bc64c85ceap1,
    -0x1.fffffffffffffp0
  },
  { // Entry 667
    0x1.d03cf63b6e19f6f34c802c96200970efp1,
    0x1.0p1
  },
  { // Entry 668
    -0x1.d03cf63b6e19f6f34c802c96200970efp1,
    -0x1.0p1
  },
  { // Entry 669
    0x1.d03cf63b6e1a3325409c12499bfc4fdap1,
    0x1.0000000000001p1
  },
  { // Entry 670
    -0x1.d03cf63b6e1a3325409c12499bfc4fdap1,
    -0x1.0000000000001p1
  },
  { // Entry 671
    0x1.b4a380370362d5f21650a55c31b348b4p4,
    0x1.fffffffffffffp1
  },
  { // Entry 672
    -0x1.b4a380370362d5f21650a55c31b348b4p4,
    -0x1.fffffffffffffp1
  },
  { // Entry 673
    0x1.b4a3803703630c8fe70261d92e563a88p4,
    0x1.0p2
  },
  { // Entry 674
    -0x1.b4a3803703630c8fe70261d92e563a88p4,
    -0x1.0p2
  },
  { // Entry 675
    0x1.b4a38037036379cb8865dad33c13c833p4,
    0x1.0000000000001p2
  },
  { // Entry 676
    -0x1.b4a38037036379cb8865dad33c13c833p4,
    -0x1.0000000000001p2
  },
  { // Entry 677
    0x1.749ea514eca5ffdf3fd18b7627cdbdc8p10,
    0x1.fffffffffffffp2
  },
  { // Entry 678
    -0x1.749ea514eca5ffdf3fd18b7627cdbdc8p10,
    -0x1.fffffffffffffp2
  },
  { // Entry 679
    0x1.749ea514eca65d06ea7688aff46cfe09p10,
    0x1.0p3
  },
  { // Entry 680
    -0x1.749ea514eca65d06ea7688aff46cfe09p10,
    -0x1.0p3
  },
  { // Entry 681
    0x1.749ea514eca717563fc08323d3893d7ep10,
    0x1.0000000000001p3
  },
  { // Entry 682
    -0x1.749ea514eca717563fc08323d3893d7ep10,
    -0x1.0000000000001p3
  },
  { // Entry 683
    0x1.0f2ebd0a7ffdb64f4e0e5fcc0c593101p22,
    0x1.fffffffffffffp3
  },
  { // Entry 684
    -0x1.0f2ebd0a7ffdb64f4e0e5fcc0c593101p22,
    -0x1.fffffffffffffp3
  },
  { // Entry 685
    0x1.0f2ebd0a7ffe3de6ac939fced0122707p22,
    0x1.0p4
  },
  { // Entry 686
    -0x1.0f2ebd0a7ffe3de6ac939fced0122707p22,
    -0x1.0p4
  },
  { // Entry 687
    0x1.0f2ebd0a7fff4d15699e1fd522e720dcp22,
    0x1.0000000000001p4
  },
  { // Entry 688
    -0x1.0f2ebd0a7fff4d15699e1fd522e720dcp22,
    -0x1.0000000000001p4
  },
  { // Entry 689
    0x1.1f43fcc4b661a8944ac389a7c7ae7b9dp45,
    0x1.fffffffffffffp4
  },
  { // Entry 690
    -0x1.1f43fcc4b661a8944ac389a7c7ae7b9dp45,
    -0x1.fffffffffffffp4
  },
  { // Entry 691
    0x1.1f43fcc4b662c7d847884009ffe4c4c3p45,
    0x1.0p5
  },
  { // Entry 692
    -0x1.1f43fcc4b662c7d847884009ffe4c4c3p45,
    -0x1.0p5
  },
  { // Entry 693
    0x1.1f43fcc4b66506604111acd1ce1d4d5dp45,
    0x1.0000000000001p5
  },
  { // Entry 694
    -0x1.1f43fcc4b66506604111acd1ce1d4d5dp45,
    -0x1.0000000000001p5
  },
  { // Entry 695
    0x1.425982cf597a4d52c89ea857bbaa807ap91,
    0x1.fffffffffffffp5
  },
  { // Entry 696
    -0x1.425982cf597a4d52c89ea857bbaa807ap91,
    -0x1.fffffffffffffp5
  },
  { // Entry 697
    0x1.425982cf597cd205ce3d5b4edb031756p91,
    0x1.0p6
  },
  { // Entry 698
    -0x1.425982cf597cd205ce3d5b4edb031756p91,
    -0x1.0p6
  },
  { // Entry 699
    0x1.425982cf5981db6bd97ac14c35e666c6p91,
    0x1.0000000000001p6
  },
  { // Entry 700
    -0x1.425982cf5981db6bd97ac14c35e666c6p91,
    -0x1.0000000000001p6
  },
  { // Entry 701
    0x1.95e54c5dd41b20600dd601a0ae672ff4p183,
    0x1.fffffffffffffp6
  },
  { // Entry 702
    -0x1.95e54c5dd41b20600dd601a0ae672ff4p183,
    -0x1.fffffffffffffp6
  },
  { // Entry 703
    0x1.95e54c5dd42177f53f4d5219df11ca3bp183,
    0x1.0p7
  },
  { // Entry 704
    -0x1.95e54c5dd42177f53f4d5219df11ca3bp183,
    -0x1.0p7
  },
  { // Entry 705
    0x1.95e54c5dd42e271fa23bf3585b655060p183,
    0x1.0000000000001p7
  },
  { // Entry 706
    -0x1.95e54c5dd42e271fa23bf3585b655060p183,
    -0x1.0000000000001p7
  },
  { // Entry 707
    0x1.41c7a8814be192a5df25b042af824efdp368,
    0x1.fffffffffffffp7
  },
  { // Entry 708
    -0x1.41c7a8814be192a5df25b042af824efdp368,
    -0x1.fffffffffffffp7
  },
  { // Entry 709
    0x1.41c7a8814beba0e323300f777da65854p368,
    0x1.0p8
  },
  { // Entry 710
    -0x1.41c7a8814beba0e323300f777da65854p368,
    -0x1.0p8
  },
  { // Entry 711
    0x1.41c7a8814bffbd5dab44ced26faccbfbp368,
    0x1.0000000000001p8
  },
  { // Entry 712
    -0x1.41c7a8814bffbd5dab44ced26faccbfbp368,
    -0x1.0000000000001p8
  },
  { // Entry 713
    0x1.9476504ba8399f5b97cae35beb78c3c5p737,
    0x1.fffffffffffffp8
  },
  { // Entry 714
    -0x1.9476504ba8399f5b97cae35beb78c3c5p737,
    -0x1.fffffffffffffp8
  },
  { // Entry 715
    0x1.9476504ba852e6c09c8567c01c5a6648p737,
    0x1.0p9
  },
  { // Entry 716
    -0x1.9476504ba852e6c09c8567c01c5a6648p737,
    -0x1.0p9
  },
  { // Entry 717
    0x1.9476504ba885758aa5fa7545e10e8e46p737,
    0x1.0000000000001p9
  },
  { // Entry 718
    -0x1.9476504ba885758aa5fa7545e10e8e46p737,
    -0x1.0000000000001p9
  },
  { // Entry 719
    -HUGE_VAL,
    -0x1.fffffffffffffp1023
  },
  { // Entry 720
    HUGE_VAL,
    0x1.fffffffffffffp1023
  },
  { // Entry 721
    HUGE_VAL,
    0x1.fffffffffffffp1023
  },
  { // Entry 722
    -HUGE_VAL,
    -0x1.fffffffffffffp1023
  },
  { // Entry 723
    HUGE_VAL,
    HUGE_VAL
  },
  { // Entry 724
    HUGE_VAL,
    0x1.fffffffffffffp1023
  },
  { // Entry 725
    HUGE_VAL,
    0x1.ffffffffffffep1023
  },
  { // Entry 726
    0x1.718f45d72e67155cecb0017179d127e6p3,
    0x1.921fb54442d18p1
  },
  { // Entry 727
    0x1.2690f661dd81ffd244e02b94a5c51d39p1,
    0x1.921fb54442d18p0
  },
  { // Entry 728
    0x1.2cd9fc44eb983e58779b22745732962dp0,
    0x1.0000000000001p0
  },
  { // Entry 729
    0x1.2cd9fc44eb9825a80249487f064ffd5cp0,
    0x1.0p0
  },
  { // Entry 730
    0x1.2cd9fc44eb98194fc7a05b845e4f82b3p0,
    0x1.fffffffffffffp-1
  },
  { // Entry 731
    0x1.bcc270b5227365b85e43b36397224d94p-1,
    0x1.921fb54442d18p-1
  },
  { // Entry 732
    0x1.00000000000010p-1022,
    0x1.0000000000001p-1022
  },
  { // Entry 733
    0x1.p-1022,
    0x1.0p-1022
  },
  { // Entry 734
    0x1.ffffffffffffe0p-1023,
    0x1.ffffffffffffep-1023
  },
  { // Entry 735
    0x1.ffffffffffffc0p-1023,
    0x1.ffffffffffffcp-1023
  },
  { // Entry 736
    0x1.p-1073,
    0x1.0p-1073
  },
  { // Entry 737
    0x1.p-1074,
    0x1.0p-1074
  },
  { // Entry 738
    0.0,
    0.0
  },
  { // Entry 739
    -0.0,
    -0.0
  },
  { // Entry 740
    -0x1.p-1074,
    -0x1.0p-1074
  },
  { // Entry 741
    -0x1.p-1073,
    -0x1.0p-1073
  },
  { // Entry 742
    -0x1.ffffffffffffc0p-1023,
    -0x1.ffffffffffffcp-1023
  },
  { // Entry 743
    -0x1.ffffffffffffe0p-1023,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 744
    -0x1.p-1022,
    -0x1.0p-1022
  },
  { // Entry 745
    -0x1.00000000000010p-1022,
    -0x1.0000000000001p-1022
  },
  { // Entry 746
    -0x1.bcc270b5227365b85e43b36397224d94p-1,
    -0x1.921fb54442d18p-1
  },
  { // Entry 747
    -0x1.2cd9fc44eb98194fc7a05b845e4f82b3p0,
    -0x1.fffffffffffffp-1
  },
  { // Entry 748
    -0x1.2cd9fc44eb9825a80249487f064ffd5cp0,
    -0x1.0p0
  },
  { // Entry 749
    -0x1.2cd9fc44eb983e58779b22745732962dp0,
    -0x1.0000000000001p0
  },
  { // Entry 750
    -0x1.2690f661dd81ffd244e02b94a5c51d39p1,
    -0x1.921fb54442d18p0
  },
  { // Entry 751
    -0x1.718f45d72e67155cecb0017179d127e6p3,
    -0x1.921fb54442d18p1
  },
  { // Entry 752
    -HUGE_VAL,
    -0x1.ffffffffffffep1023
  },
  { // Entry 753
    -HUGE_VAL,
    -0x1.fffffffffffffp1023
  },
  { // Entry 754
    -HUGE_VAL,
    -HUGE_VAL
  },
  { // Entry 755
    0x1.ffffffffffd3ae594e9bda9b6b3a9168p1023,
    0x1.633ce8fb9f87dp9
  },
  { // Entry 756
    -0x1.ffffffffffd3ae594e9bda9b6b3a9168p1023,
    -0x1.633ce8fb9f87dp9
  },
  { // Entry 757
    HUGE_VAL,
    0x1.633ce8fb9f87ep9
  },
  { // Entry 758
    -HUGE_VAL,
    -0x1.633ce8fb9f87ep9
  }
};
