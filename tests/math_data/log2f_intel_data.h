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

static data_1_1_t<float, float> g_log2f_intel_data[] = {
  { // Entry 0
    0x1.715419fdb96231bd1fa15f37bfa42910p-17,
    0x1.000080p0
  },
  { // Entry 1
    0x1.14fe88ff5753fa56dc27bca056285672p-15,
    0x1.000180p0
  },
  { // Entry 2
    0x1.715305002e4ae466ed064a01ae55204ep-15,
    0x1.0002p0
  },
  { // Entry 3
    0x1.4327b20433837a3e2c610a392bbd295ap-14,
    0x1.000380p0
  },
  { // Entry 4
    0x1.64531effed17eb9b0a93b3ccaa24f82bp-13,
    0x1.0007b8p0
  },
  { // Entry 5
    -0x1.ffe7a4ffff5f125cad7f0468b55a873cp2,
    0x1.004390p-8
  },
  { // Entry 6
    -0x1.4fdd63002e000ea74a80c91b771feb2ep4,
    0x1.0181p-21
  },
  { // Entry 7
    0x1.61382d01166a5f656628676dd57287c3p-7,
    0x1.01eb80p0
  },
  { // Entry 8
    0x1.67ea1b041aeef5d06d27db173c4a8ec3p-7,
    0x1.01f4dap0
  },
  { // Entry 9
    0x1.6f089703fa8b836209e806374014eeeap-7,
    0x1.01feccp0
  },
  { // Entry 10
    0x1.d8c5b9000072814ba11dc07e3e55730cp-6,
    0x1.052cp0
  },
  { // Entry 11
    0x1.65ad73003698ae3b6629d42d6d969d65p-5,
    0x1.07ddb4p0
  },
  { // Entry 12
    0x1.671b720031bff18c21d3c6680b607a97p-5,
    0x1.07e5e0p0
  },
  { // Entry 13
    -0x1.bd01820013d2bdf2e708a03b63cad894p2,
    0x1.0870p-7
  },
  { // Entry 14
    0x1.bc8a3f002d49ff2fff61bbc88ca84366p-5,
    0x1.09d0p0
  },
  { // Entry 15
    0x1.d15cfd000ba18b834668273db43a54f4p-5,
    0x1.0a48p0
  },
  { // Entry 16
    0x1.254503fffff2c7469f0c551c58628c75p-4,
    0x1.0d0686p0
  },
  { // Entry 17
    0x1.8972445dbe2011fbaf76774ecaa1886bp-4,
    0x1.11a0p0
  },
  { // Entry 18
    -0x1.4e0dddfffd1fadff896c773a983cb069p4,
    0x1.1684p-21
  },
  { // Entry 19
    -0x1.6d9f4300000188d5e28bf9ecd7eb48a3p1,
    0x1.1ac9bcp-3
  },
  { // Entry 20
    0x1.3e8666fed9e0919054d1723c7bfa2a2ep-3,
    0x1.1d24p0
  },
  { // Entry 21
    -0x1.a9967dfffdea2f22e87a3d2cfb4653e0p-1,
    0x1.1fc530p-1
  },
  { // Entry 22
    -0x1.ac7b430000002b16a835260d35f73f3bp0,
    0x1.40f572p-2
  },
  { // Entry 23
    -0x1.50fd36ffff7b1fe211b2f1e467a808c6p1,
    0x1.4a37aap-3
  },
  { // Entry 24
    -0x1.68e3e700011eed1c8bdf78a5004ec845p4,
    0x1.5c5780p-23
  },
  { // Entry 25
    -0x1.14c560fe68af880e0a0f337d55565281p-1,
    0x1.60p-1
  },
  { // Entry 26
    0x1.eee0f9e9bd541c2161fbaa601ad44f9dp-2,
    0x1.65ddfap0
  },
  { // Entry 27
    0x1.ffff6b715e229192074dd520e800523dp-2,
    0x1.6a09c2p0
  },
  { // Entry 28
    0x1.ffffb4e49986d923a4e540f03b1da0dap-2,
    0x1.6a09d4p0
  },
  { // Entry 29
    -0x1.000008fd564a8532198fd9c602596351p-1,
    0x1.6a09e2p-1
  },
  { // Entry 30
    -0x1.5e7df5fe538ab34efb515ac93b443d55p2,
    0x1.70p-6
  },
  { // Entry 31
    0x1.3719d8ffda8ee27c20ca5d36ce40a19bp3,
    0x1.a63c60p9
  },
  { // Entry 32
    -0x1.95152a001cb0000297a418bc58741cd8p6,
    0x1.a86a40p-102
  },
  { // Entry 33
    -0x1.a86d52000000dde9cc8582ca6f4adf02p3,
    0x1.aa932cp-14
  },
  { // Entry 34
    -0x1.a65bf4fffdc2eb6e2bba1e155731485cp3,
    0x1.be1dacp-14
  },
  { // Entry 35
    0x1.19e96affffe46969e091319af39bf01dp3,
    0x1.c0be08p8
  },
  { // Entry 36
    -0x1.6b2194fffbcc473e6b408598b11c0b76p-3,
    0x1.c4c990p-1
  },
  { // Entry 37
    -0x1.b0747afff09129e7c27981832371a2b7p-4,
    0x1.dbde6cp-1
  },
  { // Entry 38
    -0x1.a31d90fffdc06fa674e6c77e0d27325ep3,
    0x1.de9690p-14
  },
  { // Entry 39
    -0x1.7d61ebfff707baf0a4babc7a455fdedap-4,
    0x1.dfffdep-1
  },
  { // Entry 40
    -0x1.8be3350000129024f1746df09b18e1c0p1,
    0x1.e01448p-4
  },
  { // Entry 41
    -0x1.2dab68ffff653aa0f522110f165fc6cdp-4,
    0x1.e684aep-1
  },
  { // Entry 42
    -0x1.1ddc06ffe493eb9e838bc57f1692f071p-5,
    0x1.f3c3c0p-1
  },
  { // Entry 43
    -0x1.fe5c28b51763fd9e597b74b77eaef363p-6,
    0x1.f5104cp-1
  },
  { // Entry 44
    -0x1.fb46c8ffb297bc8b36ec72bf8f845dafp-6,
    0x1.f52108p-1
  },
  { // Entry 45
    -0x1.83a4382bc0ca76ba53a02f5c39dd1105p1,
    0x1.f600a8p-4
  },
  { // Entry 46
    -0x1.c5685effd6dc60c465f6d4566400ea1bp-6,
    0x1.f645bcp-1
  },
  { // Entry 47
    -0x1.c0daa2ffe3fde42c220d9cdd6dba636bp-6,
    0x1.f65e82p-1
  },
  { // Entry 48
    -0x1.a0ab3effb22b2baa230d01d368f486cfp-6,
    0x1.f70dbep-1
  },
  { // Entry 49
    -0x1.9993e6b5eebc60c6416982df9bf027cdp-6,
    0x1.f73462p-1
  },
  { // Entry 50
    -0x1.90db40b3d98bebe6e068b72646536233p-6,
    0x1.f763ecp-1
  },
  { // Entry 51
    -0x1.805ce6d1eec4e554c05d1277b7a61f1ep-6,
    0x1.f7bde0p-1
  },
  { // Entry 52
    -0x1.71c5270003e7400a82571e14d211b1abp-9,
    0x1.fefff2p-1
  },
  { // Entry 53
    -0x1.4fd0950000f9ea163e3b6b379b806a27p-9,
    0x1.ff1770p-1
  },
  { // Entry 54
    -0x1.802e2bfffffc33f278fa52402320f10ap1,
    0x1.ff800cp-4
  },
  { // Entry 55
    -0x1.718867c39aac5ee37685394fe9bfd749p-13,
    0x1.ffeffep-1
  },
  { // Entry 56
    -0x1.72c684e5cfc146d2275210812c0c7f68p-16,
    0x1.fffdfep-1
  },
  { // Entry 57
    -0x1.71552efd6e75c155bd4e4ed94f59a26ap-16,
    0x1.fffep-1
  },
  { // Entry 58
    -0x1.48ef5e00535d9165bbb9b9a8c056f797p-18,
    0x1.ffff8ep-1
  },
  { // Entry 59
    -0x1.71547935612438aa6af6b5495892e719p-22,
    0x1.fffff8p-1
  },
  { // Entry 60
    -0x1.715477c40ca820a04d97be4efccd95a9p-23,
    0x1.fffffcp-1
  },
  { // Entry 61
    -0x1.7154770b626b85efbccdf68d2e9789f9p-24,
    0x1.fffffep-1
  },
  { // Entry 62
    -0x1.000000d4175ddebaa6cc9d6112365229p-1,
    0x1.6a09e6p-1
  },
  { // Entry 63
    -0x1.561462846d15350ee6248ecd4c1b002bp-2,
    0x1.962b60p-1
  },
  { // Entry 64
    -0x1.7b67dba86e896c0a53d2c939ef0d29bfp-3,
    0x1.c24cdap-1
  },
  { // Entry 65
    -0x1.9cab66c58143f2a800e68dbb02f54331p-5,
    0x1.ee6e54p-1
  },
  { // Entry 66
    0x1.2ae054380d16d8f59673a7e3928246cdp-4,
    0x1.0d47e6p0
  },
  { // Entry 67
    0x1.7e23503264c4ad371b8f163c4f9aef26p-3,
    0x1.2358a2p0
  },
  { // Entry 68
    0x1.2aebb187ce6ee362d3103a2200b286e2p-2,
    0x1.39695ep0
  },
  { // Entry 69
    0x1.8f6e7a2697b530fad3d12a9d6a8f1d3ap-2,
    0x1.4f7a1ap0
  },
  { // Entry 70
    0x1.ed899b07eb9cb8af2c927ecf12ec7cbep-2,
    0x1.658ad6p0
  },
  { // Entry 71
    0x1.2300cb4606615b744653e167d86f2813p-1,
    0x1.7b9b92p0
  },
  { // Entry 72
    0x1.4cbccc36deb50dd3e00c1a4eca18d57ep-1,
    0x1.91ac4ep0
  },
  { // Entry 73
    0x1.743d4cb7d92562e1484f862b688a46fbp-1,
    0x1.a7bd0ap0
  },
  { // Entry 74
    0x1.99bc5947999b190ccc67cef041311282p-1,
    0x1.bdcdc6p0
  },
  { // Entry 75
    0x1.bd6b934775bef13a6359e331efe67a11p-1,
    0x1.d3de82p0
  },
  { // Entry 76
    0x1.df75be8ef439ce361ebbb13fff3a7b53p-1,
    0x1.e9ef3ep0
  },
  { // Entry 77
    0x1.fffff758052d13b69dd6c8d6740a3357p-1,
    0x1.fffffap0
  },
  { // Entry 78
    -0x1.000000d4175ddebaa6cc9d6112365229p-1,
    0x1.6a09e6p-1
  },
  { // Entry 79
    -0x1.73324a9b9d2895d79d8ddf07a502bep-2,
    0x1.8e3e16p-1
  },
  { // Entry 80
    -0x1.e54e432361189c3353cc06c2dd27d736p-3,
    0x1.b27246p-1
  },
  { // Entry 81
    -0x1.f19de909d7e63fd264a2c312d0c1789dp-4,
    0x1.d6a676p-1
  },
  { // Entry 82
    -0x1.dd89b738d21d0221c908b8a0c32f1212p-7,
    0x1.fadaa6p-1
  },
  { // Entry 83
    0x1.5c01921d594ace74709e134409d4ed24p-4,
    0x1.0f876cp0
  },
  { // Entry 84
    0x1.6cb0eb0cc03326cfabdb68ecee7aba40p-3,
    0x1.21a184p0
  },
  { // Entry 85
    0x1.0fe8503e467106a65788ddb3a15f5375p-2,
    0x1.33bb9cp0
  },
  { // Entry 86
    0x1.64594511e637e6f93b5cb2912b99abedp-2,
    0x1.45d5b4p0
  },
  { // Entry 87
    0x1.b4392815bf92113e75eb0dd2b26fd740p-2,
    0x1.57efccp0
  },
  { // Entry 88
    0x1.fffff62e925d61652c914504dc3ee2cep-2,
    0x1.6a09e4p0
  },
  { // Entry 89
    -0x1.a8ff971810a5e1817fd3b7d7e5d148bbp-2,
    0x1.80p-1
  },
  { // Entry 90
    -0x1.1c31df37d71943eb77829a1feb37b99ap-2,
    0x1.a66666p-1
  },
  { // Entry 91
    -0x1.374d6afb125968a0c493df15bf69e438p-3,
    0x1.ccccccp-1
  },
  { // Entry 92
    -0x1.2b386cc87f9a4eee785d4a5dbce887b7p-5,
    0x1.f33332p-1
  },
  { // Entry 93
    0x1.20507dbe3011bddb9a9b123c4341bc4cp-4,
    0x1.0cccccp0
  },
  { // Entry 94
    0x1.5c01a39fbd6879fa00b120a068badd12p-3,
    0x1.20p0
  },
  { // Entry 95
    0x1.0d58e803fedbad8f59d5947b2a21a425p-2,
    0x1.333334p0
  },
  { // Entry 96
    0x1.66e8c754261d0ebda20c00ad74e85091p-2,
    0x1.466668p0
  },
  { // Entry 97
    0x1.bb59c03d54eb4b10a384046c38ed39fdp-2,
    0x1.59999cp0
  },
  { // Entry 98
    0x1.059cd374571eb31852640bb553677c83p-1,
    0x1.6cccd0p0
  },
  { // Entry 99
    0x1.2b803473f7ad0f3f401624140d175ba2p-1,
    0x1.80p0
  },
  { // Entry 100
    0.0,
    0x1.p0
  },
  { // Entry 101
    0x1.199b76bf23e221a6231fc33bca41b607p-3,
    0x1.19999ap0
  },
  { // Entry 102
    0x1.0d58e803fedbad8f59d5947b2a21a425p-2,
    0x1.333334p0
  },
  { // Entry 103
    0x1.8398925fcd61fbf1aa81ef798b08bb2cp-2,
    0x1.4ccccep0
  },
  { // Entry 104
    0x1.f113c1858de496d814a68e82919d673bp-2,
    0x1.666668p0
  },
  { // Entry 105
    0x1.2b80384cd8e605e99a5cd99f34293888p-1,
    0x1.800002p0
  },
  { // Entry 106
    0x1.5b2c41f5948361383f498e179a6dd347p-1,
    0x1.99999cp0
  },
  { // Entry 107
    0x1.87f43057c707ec89ca00835fb95f66d7p-1,
    0x1.b33336p0
  },
  { // Entry 108
    0x1.b22cabaab2c8964911abde220f5f415cp-1,
    0x1.ccccd0p0
  },
  { // Entry 109
    0x1.da1c9dfd31a7706146ef266c16ed655dp-1,
    0x1.e6666ap0
  },
  { // Entry 110
    0x1.p0,
    0x1.p1
  },
  { // Entry 111
    0x1.90p6,
    0x1.p100
  },
  { // Entry 112
    0x1.908ccdbb5f91f110d3118fe19de520dbp6,
    0x1.19999ap100
  },
  { // Entry 113
    0x1.910d58e803fedbad8f59d5947b2a21a4p6,
    0x1.333334p100
  },
  { // Entry 114
    0x1.918398925fcd61fbf1aa81ef798b08bbp6,
    0x1.4ccccep100
  },
  { // Entry 115
    0x1.91f113c1858de496d814a68e82919d67p6,
    0x1.666668p100
  },
  { // Entry 116
    0x1.9257007099b1cc0bd334b9b33e685271p6,
    0x1.800002p100
  },
  { // Entry 117
    0x1.92b65883eb2906c2707e931c2f34dba6p6,
    0x1.99999cp100
  },
  { // Entry 118
    0x1.930fe860af8e0fd913940106bf72becdp6,
    0x1.b33336p100
  },
  { // Entry 119
    0x1.936459575565912c922357bc441ebe82p6,
    0x1.ccccd0p100
  },
  { // Entry 120
    0x1.93b4393bfa634ee0c28dde4cd82ddacap6,
    0x1.e6666ap100
  },
  { // Entry 121
    0x1.94p6,
    0x1.p101
  },
  { // Entry 122
    -0x1.715481dd5c5d93663255eca7ba82aeb6p-20,
    0x1.ffffe0p-1
  },
  { // Entry 123
    -0x1.71547c180a27f362d17a1f59be1bb55dp-21,
    0x1.fffff0p-1
  },
  { // Entry 124
    0.0,
    0x1.p0
  },
  { // Entry 125
    0x1.7154708d66755d9fe119ed1e85c13f40p-21,
    0x1.000008p0
  },
  { // Entry 126
    0x1.71546ac814f867d7a99ac240f177d35fp-20,
    0x1.000010p0
  },
  { // Entry 127
    -0x1.7154770b626b85efbccdf68d2e9789f9p-24,
    0x1.fffffep-1
  },
  { // Entry 128
    -0x1.7154770b626b85efbccdf68d2e9789f9p-24,
    0x1.fffffep-1
  },
  { // Entry 129
    -0x1.7154770b626b85efbccdf68d2e9789f9p-24,
    0x1.fffffep-1
  },
  { // Entry 130
    -0x1.7154770b626b85efbccdf68d2e9789f9p-24,
    0x1.fffffep-1
  },
  { // Entry 131
    -0x1.7154770b626b85efbccdf68d2e9789f9p-24,
    0x1.fffffep-1
  },
  { // Entry 132
    -0x1.7154770b626b85efbccdf68d2e9789f9p-24,
    0x1.fffffep-1
  },
  { // Entry 133
    -0x1.7154770b626b85efbccdf68d2e9789f9p-24,
    0x1.fffffep-1
  },
  { // Entry 134
    -0x1.7154770b626b85efbccdf68d2e9789f9p-24,
    0x1.fffffep-1
  },
  { // Entry 135
    -0x1.7154770b626b85efbccdf68d2e9789f9p-24,
    0x1.fffffep-1
  },
  { // Entry 136
    -0x1.7154770b626b85efbccdf68d2e9789f9p-24,
    0x1.fffffep-1
  },
  { // Entry 137
    -0x1.7154770b626b85efbccdf68d2e9789f9p-24,
    0x1.fffffep-1
  },
  { // Entry 138
    -0x1.7154770b626b85efbccdf68d2e9789f9p-24,
    0x1.fffffep-1
  },
  { // Entry 139
    -0x1.7154770b626b85efbccdf68d2e9789f9p-24,
    0x1.fffffep-1
  },
  { // Entry 140
    -0x1.7154770b626b85efbccdf68d2e9789f9p-24,
    0x1.fffffep-1
  },
  { // Entry 141
    -0x1.7154770b626b85efbccdf68d2e9789f9p-24,
    0x1.fffffep-1
  },
  { // Entry 142
    0x1.fffffffa3aae23d27651e8410cc825cbp6,
    0x1.fffffep127
  },
  { // Entry 143
    -0x1.2ap7,
    0x1.p-149
  },
  { // Entry 144
    -0x1.000004e8b6d14f4d69b75d7d91e08e98p-1,
    0x1.6a09e4p-1
  },
  { // Entry 145
    -0x1.000000d4175ddebaa6cc9d6112365229p-1,
    0x1.6a09e6p-1
  },
  { // Entry 146
    -0x1.fffff97eefe066f380fa3704987b9811p-2,
    0x1.6a09e8p-1
  },
  { // Entry 147
    0x1.fffff62e925d61652c914504dc3ee2cep-2,
    0x1.6a09e4p0
  },
  { // Entry 148
    0x1.fffffe57d144428ab266c53ddb935bacp-2,
    0x1.6a09e6p0
  },
  { // Entry 149
    0x1.00000340880fcc863f82e47db3c233f7p-1,
    0x1.6a09e8p0
  },
  { // Entry 150
    -0x1.0000017154770b626b85efbccdf68d2ep0,
    0x1.fffffep-2
  },
  { // Entry 151
    -0x1.p0,
    0x1.p-1
  },
  { // Entry 152
    -0x1.fffffa3aae2c7a711213405fc20a11b8p-1,
    0x1.000002p-1
  },
  { // Entry 153
    -0x1.a8ff9ec9d322112ed31f244bceb1ec85p-2,
    0x1.7ffffep-1
  },
  { // Entry 154
    -0x1.a8ff971810a5e1817fd3b7d7e5d148bbp-2,
    0x1.80p-1
  },
  { // Entry 155
    -0x1.a8ff8f664e33f42ccb464cc197ad8eefp-2,
    0x1.800002p-1
  },
  { // Entry 156
    0x1.2b80309b166ef76896706dda18a709bdp-1,
    0x1.7ffffep0
  },
  { // Entry 157
    0x1.2b803473f7ad0f3f401624140d175ba2p-1,
    0x1.80p0
  },
  { // Entry 158
    0x1.2b80384cd8e605e99a5cd99f34293888p-1,
    0x1.800002p0
  },
  { // Entry 159
    0x1.ebc51464ccd66f10e7d234a2a0ce225fp-9,
    0x1.00aaa8p0
  },
  { // Entry 160
    0x1.ebcad5e05d58c6ddfd6c09c193fb3e3ep-9,
    0x1.00aaaap0
  },
  { // Entry 161
    0x1.ebd0975be25fcf1843facabaa7aa7b51p-9,
    0x1.00aaacp0
  },
  { // Entry 162
    0x1.fffffe8eab88f49d947a1043320972d1p0,
    0x1.fffffep1
  },
  { // Entry 163
    0x1.p1,
    0x1.p2
  },
  { // Entry 164
    0x1.000001715474e163bb7b2fe80f7d7b91p1,
    0x1.000002p2
  },
  { // Entry 165
    0x1.fffffd1d5711e93b28f420866412e5a2p-1,
    0x1.fffffep0
  },
  { // Entry 166
    0x1.p0,
    0x1.p1
  },
  { // Entry 167
    0x1.000002e2a8e9c2c776f65fd01efaf723p0,
    0x1.000002p1
  },
  { // Entry 168
    -0x1.7154770b626b85efbccdf68d2e9789f9p-24,
    0x1.fffffep-1
  },
  { // Entry 169
    0.0,
    0x1.p0
  },
  { // Entry 170
    0x1.715474e163bb7b2fe80f7d7b91f1851cp-23,
    0x1.000002p0
  },
  { // Entry 171
    -0x1.0000017154770b626b85efbccdf68d2ep0,
    0x1.fffffep-2
  },
  { // Entry 172
    -0x1.p0,
    0x1.p-1
  },
  { // Entry 173
    -0x1.fffffa3aae2c7a711213405fc20a11b8p-1,
    0x1.000002p-1
  },
  { // Entry 174
    -0x1.000000b8aa3b85b135c2f7de66fb4697p1,
    0x1.fffffep-3
  },
  { // Entry 175
    -0x1.p1,
    0x1.p-2
  },
  { // Entry 176
    -0x1.fffffd1d57163d388909a02fe10508dcp0,
    0x1.000002p-2
  },
  { // Entry 177
    -0x1.800000b8aa3b85b135c2f7de66fb4697p1,
    0x1.fffffep-4
  },
  { // Entry 178
    -0x1.80p1,
    0x1.p-3
  },
  { // Entry 179
    -0x1.7ffffe8eab8b1e9c4484d017f082846ep1,
    0x1.000002p-3
  },
  { // Entry 180
    -0x1.0000005c551dc2d89ae17bef337da34bp2,
    0x1.fffffep-5
  },
  { // Entry 181
    -0x1.p2,
    0x1.p-4
  },
  { // Entry 182
    -0x1.fffffe8eab8b1e9c4484d017f082846ep1,
    0x1.000002p-4
  },
  { // Entry 183
    -0x1.4000005c551dc2d89ae17bef337da34bp2,
    0x1.fffffep-6
  },
  { // Entry 184
    -0x1.40p2,
    0x1.p-5
  },
  { // Entry 185
    -0x1.3fffff4755c58f4e2242680bf8414237p2,
    0x1.000002p-5
  },
  { // Entry 186
    -0x1.8000005c551dc2d89ae17bef337da34bp2,
    0x1.fffffep-7
  },
  { // Entry 187
    -0x1.80p2,
    0x1.p-6
  },
  { // Entry 188
    -0x1.7fffff4755c58f4e2242680bf8414237p2,
    0x1.000002p-6
  },
  { // Entry 189
    -0x1.c000005c551dc2d89ae17bef337da34bp2,
    0x1.fffffep-8
  },
  { // Entry 190
    -0x1.c0p2,
    0x1.p-7
  },
  { // Entry 191
    -0x1.bfffff4755c58f4e2242680bf8414237p2,
    0x1.000002p-7
  },
  { // Entry 192
    -0x1.0000002e2a8ee16c4d70bdf799bed1a5p3,
    0x1.fffffep-9
  },
  { // Entry 193
    -0x1.p3,
    0x1.p-8
  },
  { // Entry 194
    -0x1.ffffff4755c58f4e2242680bf8414237p2,
    0x1.000002p-8
  },
  { // Entry 195
    -0x1.2000002e2a8ee16c4d70bdf799bed1a5p3,
    0x1.fffffep-10
  },
  { // Entry 196
    -0x1.20p3,
    0x1.p-9
  },
  { // Entry 197
    -0x1.1fffffa3aae2c7a711213405fc20a11bp3,
    0x1.000002p-9
  },
  { // Entry 198
    -0x1.4000002e2a8ee16c4d70bdf799bed1a5p3,
    0x1.fffffep-11
  },
  { // Entry 199
    -0x1.40p3,
    0x1.p-10
  },
  { // Entry 200
    -0x1.3fffffa3aae2c7a711213405fc20a11bp3,
    0x1.000002p-10
  },
  { // Entry 201
    -0x1.a000002e2a8ee16c4d70bdf799bed1a5p3,
    0x1.fffffep-14
  },
  { // Entry 202
    -0x1.a0p3,
    0x1.p-13
  },
  { // Entry 203
    -0x1.9fffffa3aae2c7a711213405fc20a11bp3,
    0x1.000002p-13
  },
  { // Entry 204
    -0x1.a000002e2a8ee16c4d70bdf799bed1a5p3,
    0x1.fffffep-14
  },
  { // Entry 205
    -0x1.a0p3,
    0x1.p-13
  },
  { // Entry 206
    -0x1.9fffffa3aae2c7a711213405fc20a11bp3,
    0x1.000002p-13
  },
  { // Entry 207
    -0x1.800000b8aa3b85b135c2f7de66fb4697p1,
    0x1.fffffep-4
  },
  { // Entry 208
    -0x1.80p1,
    0x1.p-3
  },
  { // Entry 209
    -0x1.7ffffe8eab8b1e9c4484d017f082846ep1,
    0x1.000002p-3
  },
  { // Entry 210
    -0x1.8a898ddcb6efed6595efafc5e077a1cbp-3,
    0x1.bffffep-1
  },
  { // Entry 211
    -0x1.8a8980abfbd32666a9b7e2df60d2bdc6p-3,
    0x1.c0p-1
  },
  { // Entry 212
    -0x1.8a89737b40c57286b134031126c9c7edp-3,
    0x1.c00002p-1
  },
  { // Entry 213
    -0x1.0000005c551dc2d89ae17bef337da34bp2,
    0x1.fffffep-5
  },
  { // Entry 214
    -0x1.p2,
    0x1.p-4
  },
  { // Entry 215
    -0x1.fffffe8eab8b1e9c4484d017f082846ep1,
    0x1.000002p-4
  },
  { // Entry 216
    -0x1.7d60620c36d87cfcd8babf751edc0c8bp-4,
    0x1.dffffep-1
  },
  { // Entry 217
    -0x1.7d60496cfbb4c673b4511f8c2b4e4fb7p-4,
    0x1.e0p-1
  },
  { // Entry 218
    -0x1.7d6030cdc0ab535cca1fd50552237b1ep-4,
    0x1.e00002p-1
  },
  { // Entry 219
    -0x1.4000005c551dc2d89ae17bef337da34bp2,
    0x1.fffffep-6
  },
  { // Entry 220
    -0x1.40p2,
    0x1.p-5
  },
  { // Entry 221
    -0x1.3fffff4755c58f4e2242680bf8414237p2,
    0x1.000002p-5
  },
  { // Entry 222
    -0x1.77397c4562d9e54641f615a6ca2b27bap-5,
    0x1.effffep-1
  },
  { // Entry 223
    -0x1.77394c9d958d55de5c380fe0871d757fp-5,
    0x1.f0p-1
  },
  { // Entry 224
    -0x1.77391cf5c871f7ce6a0d60c3fcc8c0a3p-5,
    0x1.f00002p-1
  },
  { // Entry 225
    -0x1.8000005c551dc2d89ae17bef337da34bp2,
    0x1.fffffep-7
  },
  { // Entry 226
    -0x1.80p2,
    0x1.p-6
  },
  { // Entry 227
    -0x1.7fffff4755c58f4e2242680bf8414237p2,
    0x1.000002p-6
  },
  { // Entry 228
    -0x1.743f462e4254f5e2be25b8506028d08ap-6,
    0x1.f7fffep-1
  },
  { // Entry 229
    -0x1.743ee861f3556365483611f7c0bf059fp-6,
    0x1.f8p-1
  },
  { // Entry 230
    -0x1.743e8a95a4b51a5c74be0d5ae65aab1bp-6,
    0x1.f80002p-1
  },
  { // Entry 231
    -0x1.c000005c551dc2d89ae17bef337da34bp2,
    0x1.fffffep-8
  },
  { // Entry 232
    -0x1.c0p2,
    0x1.p-7
  },
  { // Entry 233
    -0x1.bfffff4755c58f4e2242680bf8414237p2,
    0x1.000002p-7
  },
  { // Entry 234
    -0x1.72c8743f6fa9cfbe1e287ad19aca6d67p-7,
    0x1.fbfffep-1
  },
  { // Entry 235
    -0x1.72c7ba20f73275b5d184a2c615b70ad4p-7,
    0x1.fcp-1
  },
  { // Entry 236
    -0x1.72c700027f76b150e530a12360d1566ap-7,
    0x1.fc0002p-1
  },
  { // Entry 237
    -0x1.0000002e2a8ee16c4d70bdf799bed1a5p3,
    0x1.fffffep-9
  },
  { // Entry 238
    -0x1.p3,
    0x1.p-8
  },
  { // Entry 239
    -0x1.ffffff4755c58f4e2242680bf8414237p2,
    0x1.000002p-8
  },
  { // Entry 240
    -0x1.720f0ecde68050a44c9a2eb30002eb02p-8,
    0x1.fdfffep-1
  },
  { // Entry 241
    -0x1.720d9c06a835ea6ef18f977e5d8a37abp-8,
    0x1.fep-1
  },
  { // Entry 242
    -0x1.720c293f6b5fbfb29fd6cb29447e6957p-8,
    0x1.fe0002p-1
  },
  { // Entry 243
    -0x1.2000002e2a8ee16c4d70bdf799bed1a5p3,
    0x1.fffffep-10
  },
  { // Entry 244
    -0x1.20p3,
    0x1.p-9
  },
  { // Entry 245
    -0x1.1fffffa3aae2c7a711213405fc20a11bp3,
    0x1.000002p-9
  },
  { // Entry 246
    -0x1.71b3ce5de192eae3e822586249ef1031p-9,
    0x1.fefffep-1
  },
  { // Entry 247
    -0x1.71b0ea42e5fda261dbbd1a498f533398p-9,
    0x1.ffp-1
  },
  { // Entry 248
    -0x1.71ae0627ed4de7a0d25affc95a315118p-9,
    0x1.ff0002p-1
  },
  { // Entry 249
    -0x1.4000002e2a8ee16c4d70bdf799bed1a5p3,
    0x1.fffffep-11
  },
  { // Entry 250
    -0x1.40p3,
    0x1.p-10
  },
  { // Entry 251
    -0x1.3fffffa3aae2c7a711213405fc20a11bp3,
    0x1.000002p-10
  },
  { // Entry 252
    -0x1.71886f5843ade047bd2d072e96484a61p-10,
    0x1.ff7ffep-1
  },
  { // Entry 253
    -0x1.7182a894b69c595f7920cea1619c6e57p-10,
    0x1.ff80p-1
  },
  { // Entry 254
    -0x1.717ce1d12f53080ec86587c1ed76029bp-10,
    0x1.ff8002p-1
  },
  { // Entry 255
    -0x1.a000002e2a8ee16c4d70bdf799bed1a5p3,
    0x1.fffffep-14
  },
  { // Entry 256
    -0x1.a0p3,
    0x1.p-13
  },
  { // Entry 257
    -0x1.9fffffa3aae2c7a711213405fc20a11bp3,
    0x1.000002p-13
  },
  { // Entry 258
    -0x1.718867c39aac5ee37685394fe9bfd749p-13,
    0x1.ffeffep-1
  },
  { // Entry 259
    -0x1.715a3bc3593d4d4a2a239745f6427420p-13,
    0x1.fff0p-1
  },
  { // Entry 260
    -0x1.712c0fc345fbad46c2c9f3884df7233ep-13,
    0x1.fff002p-1
  },
  { // Entry 261
    HUGE_VALF,
    HUGE_VALF
  },
  { // Entry 262
    0x1.fffffffa3aae23d27651e8410cc825cbp6,
    0x1.fffffep127
  },
  { // Entry 263
    0x1.fffffff4755c41df9abefafd93420d88p6,
    0x1.fffffcp127
  },
  { // Entry 264
    0x1.a6c873f5fb93d2711418c769ccf4818ap0,
    0x1.921fb6p1
  },
  { // Entry 265
    0x1.4d90e7ebf727a4e228318ed399e90315p-1,
    0x1.921fb6p0
  },
  { // Entry 266
    0x1.715474e163bb7b2fe80f7d7b91f1851cp-23,
    0x1.000002p0
  },
  { // Entry 267
    0.0,
    0x1.p0
  },
  { // Entry 268
    -0x1.7154770b626b85efbccdf68d2e9789f9p-24,
    0x1.fffffep-1
  },
  { // Entry 269
    -0x1.64de302811b0b63baf9ce258cc2df9d5p-2,
    0x1.921fb6p-1
  },
  { // Entry 270
    -0x1.f7fffff4755c58f4e2242680bf841423p6,
    0x1.000002p-126
  },
  { // Entry 271
    -0x1.f8p6,
    0x1.p-126
  },
  { // Entry 272
    -0x1.f800000b8aa3be20654105026cbdf277p6,
    0x1.fffffcp-127
  },
  { // Entry 273
    -0x1.f80000171547935612438aa6af6b5495p6,
    0x1.fffff8p-127
  },
  { // Entry 274
    -0x1.28p7,
    0x1.p-148
  },
  { // Entry 275
    -0x1.2ap7,
    0x1.p-149
  },
  { // Entry 276
    -HUGE_VALF,
    0.0f
  },
  { // Entry 277
    -HUGE_VALF,
    -0.0f
  },
  { // Entry 278
    0x1.f4p6,
    0x1.p125
  },
  { // Entry 279
    -0x1.fcp6,
    0x1.p-127
  },
  { // Entry 280
    0x1.p0,
    0x1.p1
  },
  { // Entry 281
    0x1.p1,
    0x1.p2
  },
  { // Entry 282
    -0x1.p0,
    0x1.p-1
  }
};
