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

static data_1_1_t<float, float> g_acoshf_intel_data[] = {
  { // Entry 0
    0x1.51242719804349be684bd0188d52ceccp0,
    0x1.p1
  },
  { // Entry 1
    0x1.7912730e9dd8c28d0c2e8851730eeb45p4,
    0x1.000002p33
  },
  { // Entry 2
    0x1.62e42fffa39ee35793dcbc853d3b42e7p5,
    0x1.000002p63
  },
  { // Entry 3
    0x1.6a09dedd14b1e5d3f0a7b66fb7978e52p-9,
    0x1.000040p0
  },
  { // Entry 4
    0x1.5124710011087370bef8ff29334f0588p0,
    0x1.000040p1
  },
  { // Entry 5
    0x1.7ffff7000091ccc09884d33b64b1eb87p-9,
    0x1.000048p0
  },
  { // Entry 6
    0x1.686fc30f61d32f36cebd3556647e6d85p5,
    0x1.00004cp64
  },
  { // Entry 7
    0x1.5125e27f7363b91a4d3149cf50666ecap0,
    0x1.000180p1
  },
  { // Entry 8
    0x1.e330350c572f333162767c36dce61564p-8,
    0x1.0001c8p0
  },
  { // Entry 9
    0x1.52a797d729941823c44aae94a78e8d74p-7,
    0x1.000380p0
  },
  { // Entry 10
    0x1.94c4db06c1e84a221d39f0a3cee05599p-7,
    0x1.0005p0
  },
  { // Entry 11
    0x1.deed89b7b3535ce83319a83454260bf8p-7,
    0x1.0007p0
  },
  { // Entry 12
    0x1.52a1ce85b747431168d159e69c1ef56ep-5,
    0x1.0038p0
  },
  { // Entry 13
    0x1.67d67454b91b1d46567f99ba2e2e100cp-5,
    0x1.003f3cp0
  },
  { // Entry 14
    0x1.deff5d6d7e77e9ef89d533cd1b4674c0p-5,
    0x1.007010p0
  },
  { // Entry 15
    0x1.03ecf505a34cdb22e926c22dafdcba93p-4,
    0x1.0084p0
  },
  { // Entry 16
    0x1.522637e146375db3d5e54da506a6da8ap0,
    0x1.00e0p1
  },
  { // Entry 17
    0x1.74d0fb045fad2bb6a0e3f2f93c3dbcc4p-4,
    0x1.010fa8p0
  },
  { // Entry 18
    0x1.90b591058df058eb707359449093e7d5p-4,
    0x1.0139dcp0
  },
  { // Entry 19
    0x1.bb67a8fd17fb152d1c73ebdb092cac1dp-4,
    0x1.018060p0
  },
  { // Entry 20
    0x1.e71f530f94e947158a386b336cdec658p-4,
    0x1.01d0p0
  },
  { // Entry 21
    0x1.ffaad0fa452627976ff366b9d3840fd1p-4,
    0x1.02p0
  },
  { // Entry 22
    0x1.5530ccfff7ae8f7c70f1590984ee044fp0,
    0x1.038ap1
  },
  { // Entry 23
    0x1.5e4fd4ffff5dbe26d4ed5650c003b86ap0,
    0x1.0bc0p1
  },
  { // Entry 24
    0x1.5fab1f780d388e9cc57b36be3c3141c7p0,
    0x1.0dp1
  },
  { // Entry 25
    0x1.763bdf002ea17936e0bfcfe7b6511bcbp-2,
    0x1.114986p0
  },
  { // Entry 26
    0x1.a00911010f93abee028e302008964513p-2,
    0x1.156bbcp0
  },
  { // Entry 27
    0x1.94e9050d7f9b05eaab2ab578f9f7c8a9p2,
    0x1.17a93cp8
  },
  { // Entry 28
    0x1.b6c931c025238ebcf98ef12eb28d8307p5,
    0x1.18p78
  },
  { // Entry 29
    0x1.bb6f05ffddc8a6d7ec01df7072e6e0f0p-2,
    0x1.18616cp0
  },
  { // Entry 30
    0x1.6d74ee000195eb1aa7d81dd17a217ffap0,
    0x1.1a23bap1
  },
  { // Entry 31
    0x1.ca976f7083fa74fb28b04fb16943e348p1,
    0x1.20p4
  },
  { // Entry 32
    0x1.efbe20ff9b93b8c1be0904c4167348d7p2,
    0x1.210840p10
  },
  { // Entry 33
    0x1.76b1c30001e25f3c8bf59f51e1345b89p0,
    0x1.2365e8p1
  },
  { // Entry 34
    0x1.14d7f7fffe2fabae91a11982e4e616c8p-1,
    0x1.2658p0
  },
  { // Entry 35
    0x1.2693990483fd8eeb51271e2e585b684dp-1,
    0x1.2b8d74p0
  },
  { // Entry 36
    0x1.5c4e960001d47445bae41369dbff3bebp-1,
    0x1.3d8ea8p0
  },
  { // Entry 37
    0x1.6aae7300008fa4d9f021ed601c65f965p-1,
    0x1.42f55cp0
  },
  { // Entry 38
    0x1.9e86a6000ecf0210e4a6a5b7423d0413p0,
    0x1.4fd3f0p1
  },
  { // Entry 39
    0x1.8e05b6fd5d1b8aec832f758abac8fe89p-1,
    0x1.515450p0
  },
  { // Entry 40
    0x1.df328b0ba47a77279fd4ced3f49c93eap1,
    0x1.523b56p4
  },
  { // Entry 41
    0x1.9eb7a2fc5b6aa4ff59b8601984b72a68p-1,
    0x1.58ac40p0
  },
  { // Entry 42
    0x1.abc47a73960e8473135511220cc16ca9p0,
    0x1.6058p1
  },
  { // Entry 43
    0x1.83ceeb0e93a6e047b70a3145b22d0855p3,
    0x1.660dd6p16
  },
  { // Entry 44
    0x1.e7306f0ae25f79290292e6e2e6fa8ca0p1,
    0x1.67ffc0p4
  },
  { // Entry 45
    0x1.c3bf8400023ca827c6741d7e90c625f4p-1,
    0x1.6a48p0
  },
  { // Entry 46
    0x1.9036310001a25b1ccef0f5035d136dc3p1,
    0x1.6d7680p3
  },
  { // Entry 47
    0x1.cb7077ffffb491dd760b7538a02c6e3ep-1,
    0x1.6e2c4cp0
  },
  { // Entry 48
    0x1.d466eb047d3274c3f8e4ad57ff764ea1p-1,
    0x1.72d0p0
  },
  { // Entry 49
    0x1.d53c6fc6f92e0ba23b31c22d8cc254cfp-1,
    0x1.7340p0
  },
  { // Entry 50
    0x1.ec49d25fbb6766d39e90829e6e2e250cp1,
    0x1.769da0p4
  },
  { // Entry 51
    0x1.dc679d017683946d78e2a9cc803cf6c7p-1,
    0x1.770d10p0
  },
  { // Entry 52
    0x1.e8c0b0fffe1ddf6adf3d4c2f7dd95d58p-1,
    0x1.7dc566p0
  },
  { // Entry 53
    0x1.e9609b000000a0eda71092f93ae128abp-1,
    0x1.7e1deep0
  },
  { // Entry 54
    0x1.ecc2c030a30fcdab9ac241b66cd30c25p-1,
    0x1.7ffffap0
  },
  { // Entry 55
    0x1.ecc35a07f3682dbaa360587c559ccbd3p-1,
    0x1.800050p0
  },
  { // Entry 56
    0x1.ecc6dc03c34154354f855c6bd517af5dp-1,
    0x1.800246p0
  },
  { // Entry 57
    0x1.f0192f00019712eb97524c0bc702be17p-1,
    0x1.81dfb6p0
  },
  { // Entry 58
    0x1.f284540001b93c8ebe3f4affe21905a6p-1,
    0x1.833df6p0
  },
  { // Entry 59
    0x1.f4d44c1caf6cd216b634d3097e9011f1p-1,
    0x1.848ee8p0
  },
  { // Entry 60
    0x1.f4ff87d0159c59ba0482602abe442ae8p-1,
    0x1.84a798p0
  },
  { // Entry 61
    0x1.fbd18dc250d3324af75f978654b26cdfp-1,
    0x1.8895b4p0
  },
  { // Entry 62
    0x1.fc5d43a0453c54315cc3647a30e4ed2bp-1,
    0x1.88e6fap0
  },
  { // Entry 63
    0x1.feb4430000ee8977e14ac962c3ef7706p-1,
    0x1.8a44bap0
  },
  { // Entry 64
    0x1.ce51f9f47895ee807158da16a38ca157p0,
    0x1.8ffffep1
  },
  { // Entry 65
    0x1.6c02870f43f412f2facda9c71af64d9ap5,
    0x1.9026f4p64
  },
  { // Entry 66
    0x1.47533d0000264c4cbb7c2fab58133240p1,
    0x1.9f47e2p2
  },
  { // Entry 67
    0x1.1a30b200001c3de79bc0f29982af5fc1p0,
    0x1.abee22p0
  },
  { // Entry 68
    0x1.3f6350ffda1d235a4490f7aa2ce26ae7p4,
    0x1.bd531cp27
  },
  { // Entry 69
    0x1.50eb6d04542893111cfd374dfd3d214fp1,
    0x1.bf3baap2
  },
  { // Entry 70
    0x1.2dfa93ff2c6700d1d90825d37183dcd9p2,
    0x1.bffffep5
  },
  { // Entry 71
    0x1.ecf4c21af95787266aac99616d63af21p0,
    0x1.c053d4p1
  },
  { // Entry 72
    0x1.ee596e252c01641fd16160b80bc6afe6p0,
    0x1.c2ac2ap1
  },
  { // Entry 73
    0x1.52826efff379e591193fb977ff4e6bb1p1,
    0x1.c4c3fcp2
  },
  { // Entry 74
    0x1.cb605d0b0f66c2ac5857cda13901790bp5,
    0x1.cb0d08p81
  },
  { // Entry 75
    0x1.f38fc1e25f10f5fb2271b50edba446b8p0,
    0x1.cb9080p1
  },
  { // Entry 76
    0x1.3940a3ffff65e12ff76d6976a25254bfp0,
    0x1.d8cb54p0
  },
  { // Entry 77
    0x1.40889effd28e277ad840d7466abad6ecp4,
    0x1.de61fcp27
  },
  { // Entry 78
    0x1.09aa20ff6df329fc6965c5157042b44ap3,
    0x1.f7fffep10
  },
  { // Entry 79
    0x1.dca21f00608c1d5dfa8c6e2db5abd9c0p4,
    0x1.f7fffep41
  },
  { // Entry 80
    0x1.62636e000aae80a748dcd7555caf8e89p2,
    0x1.fbfffep6
  },
  { // Entry 81
    0x1.50a2ac95684b68fdc508df40cc73323dp0,
    0x1.ff1ffep0
  },
  { // Entry 82
    0x1.50b9c8d9ac3d9fed6029492e2946e89cp0,
    0x1.ff47f0p0
  },
  { // Entry 83
    0x1.b6102affc7f74638c6d979799db2bfaap5,
    0x1.ff9ffep77
  },
  { // Entry 84
    0x1.50f6250001e11ede297c4b3f4b76e264p0,
    0x1.ffb058p0
  },
  { // Entry 85
    0x1.510a08ffff3a5b971fb41b757c6603ecp0,
    0x1.ffd2c6p0
  },
  { // Entry 86
    0x1.419ecb012c46848356c72808ab86361cp4,
    0x1.fffff2p27
  },
  { // Entry 87
    0x1.55074600473a9dd627ac47d1d2419990p6,
    0x1.fffff8p121
  },
  { // Entry 88
    0x1.640e90fffe1db3e4bbbe3d2c1b08c229p0,
    0x1.111874p1
  },
  { // Entry 89
    0.0,
    0x1.p0
  },
  { // Entry 90
    0x1.9f3245325fddd5b2c87f249c5271c1cdp-2,
    0x1.155556p0
  },
  { // Entry 91
    0x1.23a5003dc2a6d928dd921e808a9011e8p-1,
    0x1.2aaaacp0
  },
  { // Entry 92
    0x1.62e43544f8e86e9a20f297ce4a2bc5d8p-1,
    0x1.400002p0
  },
  { // Entry 93
    0x1.973a2a54caa1da0a04be159db5cae8abp-1,
    0x1.555558p0
  },
  { // Entry 94
    0x1.c48466e37608eec558429434454efbc0p-1,
    0x1.6aaaaep0
  },
  { // Entry 95
    0x1.ecc2caec5160994be04204a968c7020dp-1,
    0x1.80p0
  },
  { // Entry 96
    0.0,
    0x1.p0
  },
  { // Entry 97
    0x1.7907212d9f29112f246e3e48d17cb877p-1,
    0x1.489a58p0
  },
  { // Entry 98
    0x1.94d80f28552a7960dbd361ef8d997239p-1,
    0x1.544942p0
  },
  { // Entry 99
    0x1.cddcc749958a508d272c8af1d7f4ee9fp-1,
    0x1.6f6a8cp0
  },
  { // Entry 100
    0x1.8fcba00aaf47e796d01724c28df0a8c3p-1,
    0x1.521794p0
  },
  { // Entry 101
    0x1.8ca50cd428a176f539205f3add783b57p-2,
    0x1.137240p0
  },
  { // Entry 102
    0x1.ecc2caec5160994be04204a968c7020dp-1,
    0x1.80p0
  },
  { // Entry 103
    0x1.ecc2caec5160994be04204a968c7020dp-1,
    0x1.80p0
  },
  { // Entry 104
    0x1.08940007f543cfa0adae2e6229dce7e2p0,
    0x1.955556p0
  },
  { // Entry 105
    0x1.193ea8aad0300976a4b6e2a99a10d315p0,
    0x1.aaaaacp0
  },
  { // Entry 106
    0x1.28a7cd1cd2d875d89ba32eb5d574ffa4p0,
    0x1.c00002p0
  },
  { // Entry 107
    0x1.37030d490f3cb36dda8e8436280f6666p0,
    0x1.d55558p0
  },
  { // Entry 108
    0x1.4477a0289e7622001965214199d0661bp0,
    0x1.eaaaaep0
  },
  { // Entry 109
    0x1.51242719804349be684bd0188d52ceccp0,
    0x1.p1
  },
  { // Entry 110
    0x1.ecc2caec5160994be04204a968c7020dp-1,
    0x1.80p0
  },
  { // Entry 111
    0x1.0c242312e9f147c72de6f878eed5f263p0,
    0x1.99bf24p0
  },
  { // Entry 112
    0x1.197e88b3d1486826e7557849fa8702f9p0,
    0x1.aaffe4p0
  },
  { // Entry 113
    0x1.261b718b8dc24a39a77a013459187eabp0,
    0x1.bc5cccp0
  },
  { // Entry 114
    0x1.fbbfbb4fb3c51a1a693b8538d12b2528p-1,
    0x1.888b58p0
  },
  { // Entry 115
    0x1.4cf1a4b95964bc7af475a1628b613d0bp0,
    0x1.f8cc6ep0
  },
  { // Entry 116
    0x1.51242719804349be684bd0188d52ceccp0,
    0x1.p1
  },
  { // Entry 117
    0x1.18080dd3171b6c031a9b576be63b6d4cp6,
    0x1.p100
  },
  { // Entry 118
    0x1.1869a6d270699e1fa7c307d5fdbce864p6,
    0x1.19999ap100
  },
  { // Entry 119
    0x1.18c2c05650eac97c01479a1a77caa909p6,
    0x1.333334p100
  },
  { // Entry 120
    0x1.1914b70e86721bbde7a2eea6f077d548p6,
    0x1.4ccccep100
  },
  { // Entry 121
    0x1.19609a053a97d6f30409751e6281de59p6,
    0x1.666668p100
  },
  { // Entry 122
    0x1.19a74017386a428962791f05687972f6p6,
    0x1.800002p100
  },
  { // Entry 123
    0x1.19e9567ab98dd45c6898a542a93d6c1bp6,
    0x1.99999cp100
  },
  { // Entry 124
    0x1.1a276adcd0472f52cdae405190f05814p6,
    0x1.b33336p100
  },
  { // Entry 125
    0x1.1a61f2998eab653e55cda9cf1b8d9e50p6,
    0x1.ccccd0p100
  },
  { // Entry 126
    0x1.1a994fffd300555a0d63481601d36422p6,
    0x1.e6666ap100
  },
  { // Entry 127
    0x1.1acdd632f662a9e9c9c2e63a464b3927p6,
    0x1.p101
  },
  { // Entry 128
    0.0,
    0x1.p0
  },
  { // Entry 129
    0x1.ecc2c7586ca3963ba572db868c3947eep-1,
    0x1.7ffffep0
  },
  { // Entry 130
    0x1.ecc2caec5160994be04204a968c7020dp-1,
    0x1.80p0
  },
  { // Entry 131
    0x1.ecc2ce80361506372c8accaeb16b83abp-1,
    0x1.800002p0
  },
  { // Entry 132
    0x1.512425f1e5ce2ba992dbea3a907450b6p0,
    0x1.fffffep0
  },
  { // Entry 133
    0x1.51242719804349be684bd0188d52ceccp0,
    0x1.p1
  },
  { // Entry 134
    0x1.51242968b528e77e4665f8cde850553dp0,
    0x1.000002p1
  },
  { // Entry 135
    0x1.081eb42feeb3ba85ed12ce4bc0fcf1eep1,
    0x1.fffffep1
  },
  { // Entry 136
    0x1.081eb4b4215917af0d37af17fbf93f73p1,
    0x1.p2
  },
  { // Entry 137
    0x1.081eb5bc86a22af8d808c499360fc118p1,
    0x1.000002p2
  },
  { // Entry 138
    0x1.1542456337d4221c6b6673481f564c03p4,
    0x1.fffffep23
  },
  { // Entry 139
    0x1.1542457337d42a1c6b73c89d84aba171p4,
    0x1.p24
  },
  { // Entry 140
    0x1.1542459337d40a1c6bae7347bf564d0ep4,
    0x1.000002p24
  },
  { // Entry 141
    0x1.3687a9e1af2b0cdca14904f4ad63c259p4,
    0x1.fffffep26
  },
  { // Entry 142
    0x1.3687a9f1af2b14dca14e7a4a06e917b2p4,
    0x1.p27
  },
  { // Entry 143
    0x1.3687aa11af2af4dca17964f470d3c2c5p4,
    0x1.000002p27
  },
  { // Entry 144
    0x1.419ecb612c4804835de7582e2dc70845p4,
    0x1.fffffep27
  },
  { // Entry 145
    0x1.419ecb712c480c835decb58387285d9dp4,
    0x1.p28
  },
  { // Entry 146
    0x1.419ecb912c47ec835e17702df1a308afp4,
    0x1.000002p28
  },
  { // Entry 147
    0x1.62e42fe7a39eef5793c4bc855b3b42b2p5,
    0x1.fffffep62
  },
  { // Entry 148
    0x1.62e42fefa39ef35793c7673007e5ed5ep5,
    0x1.p63
  },
  { // Entry 149
    0x1.62e42fffa39ee35793dcbc853d3b42e7p5,
    0x1.000002p63
  },
  { // Entry 150
    0x1.601e678bc457b370e49e830c5180cc2dp6,
    0x1.fffffep125
  },
  { // Entry 151
    0x1.601e678fc457b570e49fd861a7d62183p6,
    0x1.p126
  },
  { // Entry 152
    0x1.601e6797c457ad70e4aa830c4280cc48p6,
    0x1.000002p126
  },
  { // Entry 153
    0x1.65a9f84b82e62f3e42eda0a911a063e3p6,
    0x1.fffffep127
  },
  { // Entry 154
    HUGE_VALF,
    HUGE_VALF
  },
  { // Entry 155
    0x1.65a9f84b82e62f3e42eda0a911a063e3p6,
    0x1.fffffep127
  },
  { // Entry 156
    0x1.65a9f84782e6293e42e44b53ad4b0e74p6,
    0x1.fffffcp127
  },
  { // Entry 157
    0x1.cfc0300e23df54cd908a25ac434e488cp0,
    0x1.921fb6p1
  },
  { // Entry 158
    0x1.05f23d07b63b0afafa9ad8203dad69f2p0,
    0x1.921fb6p0
  },
  { // Entry 159
    0x1.ffffffaaaaaad11110fa35a369c3dc32p-12,
    0x1.000002p0
  },
  { // Entry 160
    0.0,
    0x1.p0
  }
};
