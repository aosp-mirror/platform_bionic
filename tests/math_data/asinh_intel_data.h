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

static data_1_1_t<double, double> g_asinh_intel_data[] = {
  { // Entry 0
    -0x1.0000000000000fffffffffffffffffffp-1022,
    -0x1.0000000000001p-1022
  },
  { // Entry 1
    0x1.0000000000000fffffffffffffffffffp-1022,
    0x1.0000000000001p-1022
  },
  { // Entry 2
    -0x1.f2eba5eb5b53c803301c0f8279efd38cp-2,
    -0x1.0372ae11c5a53p-1
  },
  { // Entry 3
    0x1.f2eba5eb5b53c803301c0f8279efd38cp-2,
    0x1.0372ae11c5a53p-1
  },
  { // Entry 4
    -0x1.f8df023d2021bc3d400b315a5494d006p-2,
    -0x1.06c9b26c9b268p-1
  },
  { // Entry 5
    0x1.f8df023d2021bc3d400b315a5494d006p-2,
    0x1.06c9b26c9b268p-1
  },
  { // Entry 6
    -0x1.fb25d3a760a193706b3228440829ee8fp-2,
    -0x1.08116a27c1078p-1
  },
  { // Entry 7
    0x1.fb25d3a760a193706b3228440829ee8fp-2,
    0x1.08116a27c1078p-1
  },
  { // Entry 8
    -0x1.fec35b3e7215a3724feecdb2518899cep-2,
    -0x1.0a1a86a1a87p-1
  },
  { // Entry 9
    0x1.fec35b3e7215a3724feecdb2518899cep-2,
    0x1.0a1a86a1a87p-1
  },
  { // Entry 10
    -0x1.ff951bb7ee7f7a1d9dcf30fb1f47bad6p-2,
    -0x1.0a90be1c15949p-1
  },
  { // Entry 11
    0x1.ff951bb7ee7f7a1d9dcf30fb1f47bad6p-2,
    0x1.0a90be1c15949p-1
  },
  { // Entry 12
    -0x1.3b60a4460a2d2800883eb5ded3be4df0p2,
    -0x1.1421084210848p6
  },
  { // Entry 13
    0x1.3b60a4460a2d2800883eb5ded3be4df0p2,
    0x1.1421084210848p6
  },
  { // Entry 14
    -0x1.138aeab017488804b338e9ac099ab687p-2,
    -0x1.16e1400db88a8p-2
  },
  { // Entry 15
    0x1.138aeab017488804b338e9ac099ab687p-2,
    0x1.16e1400db88a8p-2
  },
  { // Entry 16
    -0x1.fdbacdc8a66437ff0c93d9767a45cb0cp-1,
    -0x1.2b1a532971568p0
  },
  { // Entry 17
    0x1.fdbacdc8a66437ff0c93d9767a45cb0cp-1,
    0x1.2b1a532971568p0
  },
  { // Entry 18
    -0x1.24501dcbce83f8126c61bacbb8a27cedp1,
    -0x1.36c03904a8e4ep2
  },
  { // Entry 19
    0x1.24501dcbce83f8126c61bacbb8a27cedp1,
    0x1.36c03904a8e4ep2
  },
  { // Entry 20
    -0x1.5d36f22f9d342ff8da6ba2b17b474db8p-3,
    -0x1.5ee8cb3c781c0p-3
  },
  { // Entry 21
    0x1.5d36f22f9d342ff8da6ba2b17b474db8p-3,
    0x1.5ee8cb3c781c0p-3
  },
  { // Entry 22
    -0x1.52b6672b024fe000ae144fc9afaeaf51p-1,
    -0x1.6bf6fdbf6fd24p-1
  },
  { // Entry 23
    0x1.52b6672b024fe000ae144fc9afaeaf51p-1,
    0x1.6bf6fdbf6fd24p-1
  },
  { // Entry 24
    -0x1.3993d63acc4ba8035d2b420b1158f993p1,
    -0x1.70000000000ffp2
  },
  { // Entry 25
    0x1.3993d63acc4ba8035d2b420b1158f993p1,
    0x1.70000000000ffp2
  },
  { // Entry 26
    -0x1.97580351103362867acce504fee4245dp-9,
    -0x1.97582e4a115p-9
  },
  { // Entry 27
    0x1.97580351103362867acce504fee4245dp-9,
    0x1.97582e4a115p-9
  },
  { // Entry 28
    -0x1.97b3da2c985dc7e04ebe6b0d62a5513ap-3,
    -0x1.9a6699a6699b0p-3
  },
  { // Entry 29
    0x1.97b3da2c985dc7e04ebe6b0d62a5513ap-3,
    0x1.9a6699a6699b0p-3
  },
  { // Entry 30
    -0x1.92a338ada07c1b531a9867c9a235d502p-2,
    -0x1.9d17d9fcad768p-2
  },
  { // Entry 31
    0x1.92a338ada07c1b531a9867c9a235d502p-2,
    0x1.9d17d9fcad768p-2
  },
  { // Entry 32
    -0x1.4c76858b980217a240a5c74a2588e630p1,
    -0x1.ab52dd08f34f4p2
  },
  { // Entry 33
    0x1.4c76858b980217a240a5c74a2588e630p1,
    0x1.ab52dd08f34f4p2
  },
  { // Entry 34
    -0x1.c06f36b1bdfbf6638d2d6cdc9bc63083p-8,
    -0x1.c0701c0701ep-8
  },
  { // Entry 35
    0x1.c06f36b1bdfbf6638d2d6cdc9bc63083p-8,
    0x1.c0701c0701ep-8
  },
  { // Entry 36
    0.0,
    0x1.0p-1074
  },
  { // Entry 37
    -0.0,
    -0x1.0p-1074
  },
  { // Entry 38
    0x1.0000000000000fffffffffffffffffffp-1022,
    0x1.0000000000001p-1022
  },
  { // Entry 39
    -0x1.0000000000000fffffffffffffffffffp-1022,
    -0x1.0000000000001p-1022
  },
  { // Entry 40
    0x1.419ecb712c480f8b5decb58386dedd9dp4,
    0x1.0000000000003p28
  },
  { // Entry 41
    -0x1.419ecb712c480f8b5decb58386dedd9dp4,
    -0x1.0000000000003p28
  },
  { // Entry 42
    0x1.ecc2caec516161a5e8aa442a5078d5cfp-2,
    0x1.0000000000007p-1
  },
  { // Entry 43
    -0x1.ecc2caec516161a5e8aa442a5078d5cfp-2,
    -0x1.0000000000007p-1
  },
  { // Entry 44
    0x1.facfb2399e674ffa702bb3c88b513063p-3,
    0x1.0000000000020p-2
  },
  { // Entry 45
    -0x1.facfb2399e674ffa702bb3c88b513063p-3,
    -0x1.0000000000020p-2
  },
  { // Entry 46
    0x1.fffaaad10fbf68e32f80487342e56091p-7,
    0x1.00000000000e0p-6
  },
  { // Entry 47
    -0x1.fffaaad10fbf68e32f80487342e56091p-7,
    -0x1.00000000000e0p-6
  },
  { // Entry 48
    0x1.ffeaad10b5eade934d677b23c2b7f1f1p-6,
    0x1.00000000001c0p-5
  },
  { // Entry 49
    -0x1.ffeaad10b5eade934d677b23c2b7f1f1p-6,
    -0x1.00000000001c0p-5
  },
  { // Entry 50
    0x1.c3436617a1808a74e9cb44e078522310p-1,
    0x1.00000000030p0
  },
  { // Entry 51
    -0x1.c3436617a1808a74e9cb44e078522310p-1,
    -0x1.00000000030p0
  },
  { // Entry 52
    0x1.ffffeaaacb7b0ce864203f798c0ce39dp-10,
    0x1.000000000f350p-9
  },
  { // Entry 53
    -0x1.ffffeaaacb7b0ce864203f798c0ce39dp-10,
    -0x1.000000000f350p-9
  },
  { // Entry 54
    0x1.ffffeaaacd310cb1a42a8370b88e1d60p-10,
    0x1.00000000101p-9
  },
  { // Entry 55
    -0x1.ffffeaaacd310cb1a42a8370b88e1d60p-10,
    -0x1.00000000101p-9
  },
  { // Entry 56
    0x1.fead0b69d618a81747624f3c7ca38382p-4,
    0x1.000000002p-3
  },
  { // Entry 57
    -0x1.fead0b69d618a81747624f3c7ca38382p-4,
    -0x1.000000002p-3
  },
  { // Entry 58
    0x1.ffeaad1131a3687962cb8ca0a64f4c60p-6,
    0x1.000000003e0p-5
  },
  { // Entry 59
    -0x1.ffeaad1131a3687962cb8ca0a64f4c60p-6,
    -0x1.000000003e0p-5
  },
  { // Entry 60
    0x1.ffeaad1135a2e8915dcc65794d5c1216p-6,
    0x1.000000004p-5
  },
  { // Entry 61
    -0x1.ffeaad1135a2e8915dcc65794d5c1216p-6,
    -0x1.000000004p-5
  },
  { // Entry 62
    0x1.fead0b6d0fabe7f7940a172c81b216a2p-4,
    0x1.00000001cp-3
  },
  { // Entry 63
    -0x1.fead0b6d0fabe7f7940a172c81b216a2p-4,
    -0x1.00000001cp-3
  },
  { // Entry 64
    0x1.719218369adf88000392d7dc8518a18fp0,
    0x1.000000060p1
  },
  { // Entry 65
    -0x1.719218369adf88000392d7dc8518a18fp0,
    -0x1.000000060p1
  },
  { // Entry 66
    0x1.ecc2cafab28f10000058d757d62be0f2p-2,
    0x1.0000000809d7dp-1
  },
  { // Entry 67
    -0x1.ecc2cafab28f10000058d757d62be0f2p-2,
    -0x1.0000000809d7dp-1
  },
  { // Entry 68
    0x1.ffeabb0ef606c7da67459428149adbadp-6,
    0x1.0000070p-5
  },
  { // Entry 69
    -0x1.ffeabb0ef606c7da67459428149adbadp-6,
    -0x1.0000070p-5
  },
  { // Entry 70
    0x1.0c1f98037ddb97ff3c5068b316a00165p1,
    0x1.00001c0p2
  },
  { // Entry 71
    -0x1.0c1f98037ddb97ff3c5068b316a00165p1,
    -0x1.00001c0p2
  },
  { // Entry 72
    0x1.fad06c7e0e0a7003b1140bc95ab405ffp-3,
    0x1.00006p-2
  },
  { // Entry 73
    -0x1.fad06c7e0e0a7003b1140bc95ab405ffp-3,
    -0x1.00006p-2
  },
  { // Entry 74
    0x1.c344d0212927f5fa42a3dd90c845aafep-1,
    0x1.00010p0
  },
  { // Entry 75
    -0x1.c344d0212927f5fa42a3dd90c845aafep-1,
    -0x1.00010p0
  },
  { // Entry 76
    0x1.5507660f47456800d91258e8755bd5f4p7,
    0x1.001p245
  },
  { // Entry 77
    -0x1.5507660f47456800d91258e8755bd5f4p7,
    -0x1.001p245
  },
  { // Entry 78
    0x1.210ae10a6cc59fbb8020167ab1dffccap9,
    0x1.001p833
  },
  { // Entry 79
    -0x1.210ae10a6cc59fbb8020167ab1dffccap9,
    -0x1.001p833
  },
  { // Entry 80
    0x1.d5f45b75d63b680001d13ac1dbe562bcp8,
    0x1.003p677
  },
  { // Entry 81
    -0x1.d5f45b75d63b680001d13ac1dbe562bcp8,
    -0x1.003p677
  },
  { // Entry 82
    0x1.ffb3c044157318449812a557bcf1706ap-4,
    0x1.008460a44ffc1p-3
  },
  { // Entry 83
    -0x1.ffb3c044157318449812a557bcf1706ap-4,
    -0x1.008460a44ffc1p-3
  },
  { // Entry 84
    0x1.edb50436723d4759390d6456c2e8adf5p-2,
    0x1.00876f69ec52fp-1
  },
  { // Entry 85
    -0x1.edb50436723d4759390d6456c2e8adf5p-2,
    -0x1.00876f69ec52fp-1
  },
  { // Entry 86
    0x1.41bb41c3f17da804e9ad14f4bd75bacep4,
    0x1.01c8fb2951ca4p28
  },
  { // Entry 87
    -0x1.41bb41c3f17da804e9ad14f4bd75bacep4,
    -0x1.01c8fb2951ca4p28
  },
  { // Entry 88
    0x1.c6dfe797565f57ffff7a8713d032c096p-1,
    0x1.028f5c2a30342p0
  },
  { // Entry 89
    -0x1.c6dfe797565f57ffff7a8713d032c096p-1,
    -0x1.028f5c2a30342p0
  },
  { // Entry 90
    0x1.c6dfe79afae7afffffa3117d3818eeecp-1,
    0x1.028f5c2cc6e8bp0
  },
  { // Entry 91
    -0x1.c6dfe79afae7afffffa3117d3818eeecp-1,
    -0x1.028f5c2cc6e8bp0
  },
  { // Entry 92
    0x1.f15897705c5497fd072c467ee385c7a0p-2,
    0x1.0290d52c6e91cp-1
  },
  { // Entry 93
    -0x1.f15897705c5497fd072c467ee385c7a0p-2,
    -0x1.0290d52c6e91cp-1
  },
  { // Entry 94
    0x1.f1760421743425feec4439805ecd1fc5p-2,
    0x1.02a150a8542a0p-1
  },
  { // Entry 95
    -0x1.f1760421743425feec4439805ecd1fc5p-2,
    -0x1.02a150a8542a0p-1
  },
  { // Entry 96
    0x1.f6bc79c87c02ec261ae70361810513bfp-2,
    0x1.0596af363ee5ap-1
  },
  { // Entry 97
    -0x1.f6bc79c87c02ec261ae70361810513bfp-2,
    -0x1.0596af363ee5ap-1
  },
  { // Entry 98
    0x1.78a6cf7b3035bce4e063abd1ab00c432p0,
    0x1.08040201008p1
  },
  { // Entry 99
    -0x1.78a6cf7b3035bce4e063abd1ab00c432p0,
    -0x1.08040201008p1
  },
  { // Entry 100
    0x1.fb18e232de8cd7fb0a7c7d40c2100415p-2,
    0x1.080a2213c5d7fp-1
  },
  { // Entry 101
    -0x1.fb18e232de8cd7fb0a7c7d40c2100415p-2,
    -0x1.080a2213c5d7fp-1
  },
  { // Entry 102
    0x1.fc36c248c852a7ffddc9062d9e764f4bp-2,
    0x1.08aaffb099863p-1
  },
  { // Entry 103
    -0x1.fc36c248c852a7ffddc9062d9e764f4bp-2,
    -0x1.08aaffb099863p-1
  },
  { // Entry 104
    0x1.0f28edd4476116d66ca78dc309cf80f9p-9,
    0x1.0f28fa815b6c8p-9
  },
  { // Entry 105
    -0x1.0f28edd4476116d66ca78dc309cf80f9p-9,
    -0x1.0f28fa815b6c8p-9
  },
  { // Entry 106
    0x1.151f44f2b4f427ffff7a41c02c5cf355p1,
    0x1.133333335479ap2
  },
  { // Entry 107
    -0x1.151f44f2b4f427ffff7a41c02c5cf355p1,
    -0x1.133333335479ap2
  },
  { // Entry 108
    0x1.e46bf608630f17f3183a7d1db5529144p-1,
    0x1.180p0
  },
  { // Entry 109
    -0x1.e46bf608630f17f3183a7d1db5529144p-1,
    -0x1.180p0
  },
  { // Entry 110
    0x1.434264c6fc6708034a24c8e524f5687dp4,
    0x1.1b9d819ebf8cep28
  },
  { // Entry 111
    -0x1.434264c6fc6708034a24c8e524f5687dp4,
    -0x1.1b9d819ebf8cep28
  },
  { // Entry 112
    0x1.101ff8a713880a351f47c974f12ce55ep-1,
    0x1.1d1e74e330911p-1
  },
  { // Entry 113
    -0x1.101ff8a713880a351f47c974f12ce55ep-1,
    -0x1.1d1e74e330911p-1
  },
  { // Entry 114
    0x1.1a493ab88461d7fd3beeba172ff9a7f0p-2,
    0x1.1dep-2
  },
  { // Entry 115
    -0x1.1a493ab88461d7fd3beeba172ff9a7f0p-2,
    -0x1.1dep-2
  },
  { // Entry 116
    0x1.12e044c3ab17180beb67fd37dde3818ep-1,
    0x1.2045a703c2358p-1
  },
  { // Entry 117
    -0x1.12e044c3ab17180beb67fd37dde3818ep-1,
    -0x1.2045a703c2358p-1
  },
  { // Entry 118
    0x1.134f8303f8e7d7792a8cfcae2ab188a3p-1,
    0x1.20c557b1da1d0p-1
  },
  { // Entry 119
    -0x1.134f8303f8e7d7792a8cfcae2ab188a3p-1,
    -0x1.20c557b1da1d0p-1
  },
  { // Entry 120
    0x1.13a88f92f15df43236c0af1f8703f1e3p-1,
    0x1.212b987b85034p-1
  },
  { // Entry 121
    -0x1.13a88f92f15df43236c0af1f8703f1e3p-1,
    -0x1.212b987b85034p-1
  },
  { // Entry 122
    0x1.16805e66deb3cf0b1c0ec9b60fdb7894p-1,
    0x1.24709b4a7de54p-1
  },
  { // Entry 123
    -0x1.16805e66deb3cf0b1c0ec9b60fdb7894p-1,
    -0x1.24709b4a7de54p-1
  },
  { // Entry 124
    0x1.265195db6355dfff0127bbf1e5e11703p-6,
    0x1.2655a343af923p-6
  },
  { // Entry 125
    -0x1.265195db6355dfff0127bbf1e5e11703p-6,
    -0x1.2655a343af923p-6
  },
  { // Entry 126
    0x1.43fe1550dc5730023455469bb4ddb63fp4,
    0x1.28e9e3033b2d0p28
  },
  { // Entry 127
    -0x1.43fe1550dc5730023455469bb4ddb63fp4,
    -0x1.28e9e3033b2d0p28
  },
  { // Entry 128
    0x1.218f8513eb4d27ec00b1291cfdc18d67p1,
    0x1.300000036bf99p2
  },
  { // Entry 129
    -0x1.218f8513eb4d27ec00b1291cfdc18d67p1,
    -0x1.300000036bf99p2
  },
  { // Entry 130
    0x1.22a54301136de7ee5ce85eb01455a4eep1,
    0x1.32a4e674697e7p2
  },
  { // Entry 131
    -0x1.22a54301136de7ee5ce85eb01455a4eep1,
    -0x1.32a4e674697e7p2
  },
  { // Entry 132
    0x1.24292ef03128080ed3d9b08df44f6fb7p1,
    0x1.365fc696fa5b0p2
  },
  { // Entry 133
    -0x1.24292ef03128080ed3d9b08df44f6fb7p1,
    -0x1.365fc696fa5b0p2
  },
  { // Entry 134
    0x1.2575169c887057ffff7a5b19ee86a3d5p1,
    0x1.3999999a1ab8cp2
  },
  { // Entry 135
    -0x1.2575169c887057ffff7a5b19ee86a3d5p1,
    -0x1.3999999a1ab8cp2
  },
  { // Entry 136
    0x1.25dc8638bdb2f80ff1668031438c9765p1,
    0x1.3a9ca45e66e91p2
  },
  { // Entry 137
    -0x1.25dc8638bdb2f80ff1668031438c9765p1,
    -0x1.3a9ca45e66e91p2
  },
  { // Entry 138
    0x1.26409faea12417fd5ca4674254142d91p1,
    0x1.3b9817a24777ep2
  },
  { // Entry 139
    -0x1.26409faea12417fd5ca4674254142d91p1,
    -0x1.3b9817a24777ep2
  },
  { // Entry 140
    0x1.4fffffffff9f880000004ac8393332e6p-20,
    0x1.5p-20
  },
  { // Entry 141
    -0x1.4fffffffff9f880000004ac8393332e6p-20,
    -0x1.5p-20
  },
  { // Entry 142
    0x1.2ee78375a21e580b5728c038fb225c9fp1,
    0x1.52191d255a790p2
  },
  { // Entry 143
    -0x1.2ee78375a21e580b5728c038fb225c9fp1,
    -0x1.52191d255a790p2
  },
  { // Entry 144
    0x1.3185d0f16e6b6807a095bb392a3382b4p1,
    0x1.593552a340f40p2
  },
  { // Entry 145
    -0x1.3185d0f16e6b6807a095bb392a3382b4p1,
    -0x1.593552a340f40p2
  },
  { // Entry 146
    0x1.5aa83c174d0747d7431c3ae246f0a0c0p-5,
    0x1.5ac2b9013fba0p-5
  },
  { // Entry 147
    -0x1.5aa83c174d0747d7431c3ae246f0a0c0p-5,
    -0x1.5ac2b9013fba0p-5
  },
  { // Entry 148
    0x1.339f4c2a909867fb9c10e43011125aa5p1,
    0x1.5f03888dbf20fp2
  },
  { // Entry 149
    -0x1.339f4c2a909867fb9c10e43011125aa5p1,
    -0x1.5f03888dbf20fp2
  },
  { // Entry 150
    0x1.bd38dfe35d1acaae5c9501194270a136p0,
    0x1.6113b497290a0p1
  },
  { // Entry 151
    -0x1.bd38dfe35d1acaae5c9501194270a136p0,
    -0x1.6113b497290a0p1
  },
  { // Entry 152
    0x1.c1075a363e410a8f98845c743ba3787ap0,
    0x1.66b359acd6680p1
  },
  { // Entry 153
    -0x1.c1075a363e410a8f98845c743ba3787ap0,
    -0x1.66b359acd6680p1
  },
  { // Entry 154
    0x1.c262c12766e32579979b9555c11e1726p0,
    0x1.68b97a389b46cp1
  },
  { // Entry 155
    -0x1.c262c12766e32579979b9555c11e1726p0,
    -0x1.68b97a389b46cp1
  },
  { // Entry 156
    0x1.747df23f098e1ea8bfc81fa5be8e0998p-7,
    0x1.748p-7
  },
  { // Entry 157
    -0x1.747df23f098e1ea8bfc81fa5be8e0998p-7,
    -0x1.748p-7
  },
  { // Entry 158
    0x1.734f39e590ff4c558714ce06a3e0ce12p-2,
    0x1.7b8p-2
  },
  { // Entry 159
    -0x1.734f39e590ff4c558714ce06a3e0ce12p-2,
    -0x1.7b8p-2
  },
  { // Entry 160
    0x1.84ea54e95b79d80052d4496a3b257037p-4,
    0x1.858p-4
  },
  { // Entry 161
    -0x1.84ea54e95b79d80052d4496a3b257037p-4,
    -0x1.858p-4
  },
  { // Entry 162
    0x1.8c3d8723afa7d7fe5aac8c67302c2016p-7,
    0x1.8c4p-7
  },
  { // Entry 163
    -0x1.8c3d8723afa7d7fe5aac8c67302c2016p-7,
    -0x1.8c4p-7
  },
  { // Entry 164
    0x1.6fdab10671bdb04ee0b9e2255ea89ee4p-1,
    0x1.905415054158cp-1
  },
  { // Entry 165
    -0x1.6fdab10671bdb04ee0b9e2255ea89ee4p-1,
    -0x1.905415054158cp-1
  },
  { // Entry 166
    0x1.77204b30761997ffff9f14bcadd5050ep-1,
    0x1.9999999df31f2p-1
  },
  { // Entry 167
    -0x1.77204b30761997ffff9f14bcadd5050ep-1,
    -0x1.9999999df31f2p-1
  },
  { // Entry 168
    0x1.8f656b48fdbb8800007fb53bc7f22857p-2,
    0x1.999999a8e2404p-2
  },
  { // Entry 169
    -0x1.8f656b48fdbb8800007fb53bc7f22857p-2,
    -0x1.999999a8e2404p-2
  },
  { // Entry 170
    0x1.98eb9ea8504947ffff97daef209016f0p-4,
    0x1.999999c3bfab9p-4
  },
  { // Entry 171
    -0x1.98eb9ea8504947ffff97daef209016f0p-4,
    -0x1.999999c3bfab9p-4
  },
  { // Entry 172
    0x1.98eb9eaddfeba000008e0f13b5b376e8p-4,
    0x1.999999c95667ap-4
  },
  { // Entry 173
    -0x1.98eb9eaddfeba000008e0f13b5b376e8p-4,
    -0x1.999999c95667ap-4
  },
  { // Entry 174
    0x1.796335ca772c2274edcfef9c0d7aea2ap-1,
    0x1.9c8p-1
  },
  { // Entry 175
    -0x1.796335ca772c2274edcfef9c0d7aea2ap-1,
    -0x1.9c8p-1
  },
  { // Entry 176
    0x1.a3fc70e78b72affea798fb1ed9e01b11p-7,
    0x1.a3ff627f789p-7
  },
  { // Entry 177
    -0x1.a3fc70e78b72affea798fb1ed9e01b11p-7,
    -0x1.a3ff627f789p-7
  },
  { // Entry 178
    0x1.4f0a85b0ad4857ff001b414064939016p9,
    0x1.a6edff7a583f8p965
  },
  { // Entry 179
    -0x1.4f0a85b0ad4857ff001b414064939016p9,
    -0x1.a6edff7a583f8p965
  },
  { // Entry 180
    0x1.4c2b484bc41907fe51fbf0db7eb6c058p1,
    0x1.aa552a954aap2
  },
  { // Entry 181
    -0x1.4c2b484bc41907fe51fbf0db7eb6c058p1,
    -0x1.aa552a954aap2
  },
  { // Entry 182
    0x1.8964d070b42ff2d026071f870e311b26p-1,
    0x1.b1427cd988b8cp-1
  },
  { // Entry 183
    -0x1.8964d070b42ff2d026071f870e311b26p-1,
    -0x1.b1427cd988b8cp-1
  },
  { // Entry 184
    0x1.4d02914955d62fffff71df03043350f4p0,
    0x1.b3333337506cap0
  },
  { // Entry 185
    -0x1.4d02914955d62fffff71df03043350f4p0,
    -0x1.b3333337506cap0
  },
  { // Entry 186
    0x1.b010ad38a80b2ae0fc455f9f136e746dp-2,
    0x1.bd0p-2
  },
  { // Entry 187
    -0x1.b010ad38a80b2ae0fc455f9f136e746dp-2,
    -0x1.bd0p-2
  },
  { // Entry 188
    0x1.c8543a9a9c24311233a315255f5c4651p-9,
    0x1.c8547704cc94ap-9
  },
  { // Entry 189
    -0x1.c8543a9a9c24311233a315255f5c4651p-9,
    -0x1.c8547704cc94ap-9
  },
  { // Entry 190
    0x1.c9ac0c777fff8ff9d8ee7153627176abp-4,
    0x1.caap-4
  },
  { // Entry 191
    -0x1.c9ac0c777fff8ff9d8ee7153627176abp-4,
    -0x1.caap-4
  },
  { // Entry 192
    0x1.ca6adaba65efe7f20f871de10947cd5cp-4,
    0x1.cb5fffffe7a06p-4
  },
  { // Entry 193
    -0x1.ca6adaba65efe7f20f871de10947cd5cp-4,
    -0x1.cb5fffffe7a06p-4
  },
  { // Entry 194
    0x1.ca6adaba7e28a7f59fa07b692a9fe0f9p-4,
    0x1.cb6p-4
  },
  { // Entry 195
    -0x1.ca6adaba7e28a7f59fa07b692a9fe0f9p-4,
    -0x1.cb6p-4
  },
  { // Entry 196
    0x1.e1c5c0ca279f77ff07f31b933b58d12ap2,
    0x1.d0b42d0b42d08p9
  },
  { // Entry 197
    -0x1.e1c5c0ca279f77ff07f31b933b58d12ap2,
    -0x1.d0b42d0b42d08p9
  },
  { // Entry 198
    0x1.a3e8b71cba28d7fec3d1c6454bbba99dp-1,
    0x1.d4974eca333c3p-1
  },
  { // Entry 199
    -0x1.a3e8b71cba28d7fec3d1c6454bbba99dp-1,
    -0x1.d4974eca333c3p-1
  },
  { // Entry 200
    0x1.d4ede01f10ab67b211c278274a3c64eap-5,
    0x1.d52f71e93cb21p-5
  },
  { // Entry 201
    -0x1.d4ede01f10ab67b211c278274a3c64eap-5,
    -0x1.d52f71e93cb21p-5
  },
  { // Entry 202
    0x1.a66b6c181a57eec89a856dab88260c85p-1,
    0x1.d80p-1
  },
  { // Entry 203
    -0x1.a66b6c181a57eec89a856dab88260c85p-1,
    -0x1.d80p-1
  },
  { // Entry 204
    0x1.da493afa5f7b1834c0ac9129ea3f8481p-5,
    0x1.da8d12b111853p-5
  },
  { // Entry 205
    -0x1.da493afa5f7b1834c0ac9129ea3f8481p-5,
    -0x1.da8d12b111853p-5
  },
  { // Entry 206
    0x1.d86c93088bcd89c3a7ea69e10bebc834p-3,
    0x1.dcap-3
  },
  { // Entry 207
    -0x1.d86c93088bcd89c3a7ea69e10bebc834p-3,
    -0x1.dcap-3
  },
  { // Entry 208
    0x1.5b210231129f7801b2df8c036bedd614p1,
    0x1.dfbf7efdfbf78p2
  },
  { // Entry 209
    -0x1.5b210231129f7801b2df8c036bedd614p1,
    -0x1.dfbf7efdfbf78p2
  },
  { // Entry 210
    0x1.e743ce73d923d00856cbaafe09163cdfp-3,
    0x1.ebep-3
  },
  { // Entry 211
    -0x1.e743ce73d923d00856cbaafe09163cdfp-3,
    -0x1.ebep-3
  },
  { // Entry 212
    0x1.de596b4f4d5018eb85e6571011c15683p-2,
    0x1.efeffffffffffp-2
  },
  { // Entry 213
    -0x1.de596b4f4d5018eb85e6571011c15683p-2,
    -0x1.efeffffffffffp-2
  },
  { // Entry 214
    0x1.6a6eb2ffee2edafd7fed403ad2c6d203p0,
    0x1.f03c3f9d576bcp0
  },
  { // Entry 215
    -0x1.6a6eb2ffee2edafd7fed403ad2c6d203p0,
    -0x1.f03c3f9d576bcp0
  },
  { // Entry 216
    0x1.f3df6e88a792e801ff70692e5a789f21p-5,
    0x1.f42edbd85d9f8p-5
  },
  { // Entry 217
    -0x1.f3df6e88a792e801ff70692e5a789f21p-5,
    -0x1.f42edbd85d9f8p-5
  },
  { // Entry 218
    0x1.6c59a446d1b78b1c9b74684b78b10b0fp0,
    0x1.f46ea5f8f54e8p0
  },
  { // Entry 219
    -0x1.6c59a446d1b78b1c9b74684b78b10b0fp0,
    -0x1.f46ea5f8f54e8p0
  },
  { // Entry 220
    0x1.f55afe0cccf0971d810065fd8ca51746p-7,
    0x1.f56p-7
  },
  { // Entry 221
    -0x1.f55afe0cccf0971d810065fd8ca51746p-7,
    -0x1.f56p-7
  },
  { // Entry 222
    0x1.f72c28bf439cf823462f9436177c0f68p-3,
    0x1.fc4p-3
  },
  { // Entry 223
    -0x1.f72c28bf439cf823462f9436177c0f68p-3,
    -0x1.fc4p-3
  },
  { // Entry 224
    0x1.fe3de8a25c148ff37ff3fff4ea96b0c5p-4,
    0x1.ff8ffffffffffp-4
  },
  { // Entry 225
    -0x1.fe3de8a25c148ff37ff3fff4ea96b0c5p-4,
    -0x1.ff8ffffffffffp-4
  },
  { // Entry 226
    0x1.ff7abb0b079d45b8c077c138812ef785p-6,
    0x1.ff8ffffffffffp-6
  },
  { // Entry 227
    -0x1.ff7abb0b079d45b8c077c138812ef785p-6,
    -0x1.ff8ffffffffffp-6
  },
  { // Entry 228
    0x1.ff8faae2c4a7e15f49f1e7fd10226df8p-9,
    0x1.ff8ffffffffffp-9
  },
  { // Entry 229
    -0x1.ff8faae2c4a7e15f49f1e7fd10226df8p-9,
    -0x1.ff8ffffffffffp-9
  },
  { // Entry 230
    0x1.6363716659a0d6ec9a4d1673a0c1caf9p1,
    0x1.ffffff3ffffffp2
  },
  { // Entry 231
    -0x1.6363716659a0d6ec9a4d1673a0c1caf9p1,
    -0x1.ffffff3ffffffp2
  },
  { // Entry 232
    0x1.fffaaab70073cffe1ca7af59dea383bdp-7,
    0x1.ffffffe5effffp-7
  },
  { // Entry 233
    -0x1.fffaaab70073cffe1ca7af59dea383bdp-7,
    -0x1.ffffffe5effffp-7
  },
  { // Entry 234
    0x1.ffeaad091c25f7fe78be0efb91393b35p-6,
    0x1.fffffff8657ffp-6
  },
  { // Entry 235
    -0x1.ffeaad091c25f7fe78be0efb91393b35p-6,
    -0x1.fffffff8657ffp-6
  },
  { // Entry 236
    0x1.fffaaacd0fc35843b8b9c2c8eaaf170dp-7,
    0x1.fffffffbfffffp-7
  },
  { // Entry 237
    -0x1.fffaaacd0fc35843b8b9c2c8eaaf170dp-7,
    -0x1.fffffffbfffffp-7
  },
  { // Entry 238
    0x1.c34366179ac8d01e2cd45de4745e134ap-1,
    0x1.fffffffffc7ffp-1
  },
  { // Entry 239
    -0x1.c34366179ac8d01e2cd45de4745e134ap-1,
    -0x1.fffffffffc7ffp-1
  },
  { // Entry 240
    0x1.fead0b6996797834b43bac6b0f3fe20fp-4,
    0x1.ffffffffffe21p-4
  },
  { // Entry 241
    -0x1.fead0b6996797834b43bac6b0f3fe20fp-4,
    -0x1.ffffffffffe21p-4
  },
  { // Entry 242
    0x1.c34366179d41a11c2058c40156eae780p-1,
    0x1.fffffffffffeep-1
  },
  { // Entry 243
    -0x1.c34366179d41a11c2058c40156eae780p-1,
    -0x1.fffffffffffeep-1
  },
  { // Entry 244
    0x1.ffffaaaad10fda3642e7f712ee391058p-9,
    0x1.fffffffffffeep-9
  },
  { // Entry 245
    -0x1.ffffaaaad10fda3642e7f712ee391058p-9,
    -0x1.fffffffffffeep-9
  },
  { // Entry 246
    0x1.30fc1931f09c97ff42ff5cad467897fdp7,
    0x1.fffffffffffeep218
  },
  { // Entry 247
    -0x1.30fc1931f09c97ff42ff5cad467897fdp7,
    -0x1.fffffffffffeep218
  },
  { // Entry 248
    0x1.ffaad0fa452557ff22342e0cd4997830p-5,
    0x1.ffffffffffff3p-5
  },
  { // Entry 249
    -0x1.ffaad0fa452557ff22342e0cd4997830p-5,
    -0x1.ffffffffffff3p-5
  },
  { // Entry 250
    0x1.5ca72d17ed3ea80089ae65dfafc1e2b2p8,
    0x1.ffffffffffffep501
  },
  { // Entry 251
    -0x1.5ca72d17ed3ea80089ae65dfafc1e2b2p8,
    -0x1.ffffffffffffep501
  },
  { // Entry 252
    -0x1.c34366179d426cc1b1f33d1ba4a314a7p-1,
    -0x1.0p0
  },
  { // Entry 253
    0x1.c34366179d426cc1b1f33d1ba4a314a7p-1,
    0x1.0p0
  },
  { // Entry 254
    -0x1.9490fec3efbe5988497c1478565aa446p-1,
    -0x1.cp-1
  },
  { // Entry 255
    0x1.9490fec3efbe5988497c1478565aa446p-1,
    0x1.cp-1
  },
  { // Entry 256
    -0x1.62e42fefa39ef35793c7673007e5ed5ep-1,
    -0x1.8p-1
  },
  { // Entry 257
    0x1.62e42fefa39ef35793c7673007e5ed5ep-1,
    0x1.8p-1
  },
  { // Entry 258
    -0x1.2e27502cbfb3347f6bcbf84eb95ca4a3p-1,
    -0x1.4p-1
  },
  { // Entry 259
    0x1.2e27502cbfb3347f6bcbf84eb95ca4a3p-1,
    0x1.4p-1
  },
  { // Entry 260
    -0x1.ecc2caec5160994be04204a968c7020dp-2,
    -0x1.0p-1
  },
  { // Entry 261
    0x1.ecc2caec5160994be04204a968c7020dp-2,
    0x1.0p-1
  },
  { // Entry 262
    -0x1.7786a7973a5923b96eef2610c25e841ap-2,
    -0x1.8p-2
  },
  { // Entry 263
    0x1.7786a7973a5923b96eef2610c25e841ap-2,
    0x1.8p-2
  },
  { // Entry 264
    -0x1.facfb2399e636e8d67115de540a778acp-3,
    -0x1.0p-2
  },
  { // Entry 265
    0x1.facfb2399e636e8d67115de540a778acp-3,
    0x1.0p-2
  },
  { // Entry 266
    -0x1.fead0b6996972d06058e3eb12c938140p-4,
    -0x1.0p-3
  },
  { // Entry 267
    0x1.fead0b6996972d06058e3eb12c938140p-4,
    0x1.0p-3
  },
  { // Entry 268
    0.0,
    0.0
  },
  { // Entry 269
    0x1.fead0b6996972d06058e3eb12c938140p-4,
    0x1.0p-3
  },
  { // Entry 270
    -0x1.fead0b6996972d06058e3eb12c938140p-4,
    -0x1.0p-3
  },
  { // Entry 271
    0x1.facfb2399e636e8d67115de540a778acp-3,
    0x1.0p-2
  },
  { // Entry 272
    -0x1.facfb2399e636e8d67115de540a778acp-3,
    -0x1.0p-2
  },
  { // Entry 273
    0x1.7786a7973a5923b96eef2610c25e841ap-2,
    0x1.8p-2
  },
  { // Entry 274
    -0x1.7786a7973a5923b96eef2610c25e841ap-2,
    -0x1.8p-2
  },
  { // Entry 275
    0x1.ecc2caec5160994be04204a968c7020dp-2,
    0x1.0p-1
  },
  { // Entry 276
    -0x1.ecc2caec5160994be04204a968c7020dp-2,
    -0x1.0p-1
  },
  { // Entry 277
    0x1.2e27502cbfb3347f6bcbf84eb95ca4a3p-1,
    0x1.4p-1
  },
  { // Entry 278
    -0x1.2e27502cbfb3347f6bcbf84eb95ca4a3p-1,
    -0x1.4p-1
  },
  { // Entry 279
    0x1.62e42fefa39ef35793c7673007e5ed5ep-1,
    0x1.8p-1
  },
  { // Entry 280
    -0x1.62e42fefa39ef35793c7673007e5ed5ep-1,
    -0x1.8p-1
  },
  { // Entry 281
    0x1.9490fec3efbe5988497c1478565aa446p-1,
    0x1.cp-1
  },
  { // Entry 282
    -0x1.9490fec3efbe5988497c1478565aa446p-1,
    -0x1.cp-1
  },
  { // Entry 283
    0x1.c34366179d426cc1b1f33d1ba4a314a7p-1,
    0x1.0p0
  },
  { // Entry 284
    -0x1.c34366179d426cc1b1f33d1ba4a314a7p-1,
    -0x1.0p0
  },
  { // Entry 285
    0x1.18080dd3171b6c031a9b576be63b6d4cp6,
    0x1.0p100
  },
  { // Entry 286
    -0x1.18080dd3171b6c031a9b576be63b6d4cp6,
    -0x1.0p100
  },
  { // Entry 287
    0x1.1869a6d0fc0c8734cff5be4c994a623cp6,
    0x1.199999999999ap100
  },
  { // Entry 288
    -0x1.1869a6d0fc0c8734cff5be4c994a623cp6,
    -0x1.199999999999ap100
  },
  { // Entry 289
    0x1.18c2c053a6401fdf8f801885ecec896ep6,
    0x1.3333333333334p100
  },
  { // Entry 290
    -0x1.18c2c053a6401fdf8f801885ecec896ep6,
    -0x1.3333333333334p100
  },
  { // Entry 291
    0x1.1914b70ad53709fc02e60c9931465d1cp6,
    0x1.4cccccccccccep100
  },
  { // Entry 292
    -0x1.1914b70ad53709fc02e60c9931465d1cp6,
    -0x1.4cccccccccccep100
  },
  { // Entry 293
    0x1.19609a00a84eb5469b8a14575cfcffdcp6,
    0x1.6666666666668p100
  },
  { // Entry 294
    -0x1.19609a00a84eb5469b8a14575cfcffdcp6,
    -0x1.6666666666668p100
  },
  { // Entry 295
    0x1.19a74011e314f1179b5984282f925681p6,
    0x1.8000000000002p100
  },
  { // Entry 296
    -0x1.19a74011e314f1179b5984282f925681p6,
    -0x1.8000000000002p100
  },
  { // Entry 297
    0x1.19e95674b98dd93c68942542ae48ec14p6,
    0x1.999999999999cp100
  },
  { // Entry 298
    -0x1.19e95674b98dd93c68942542ae48ec14p6,
    -0x1.999999999999cp100
  },
  { // Entry 299
    0x1.1a276ad639b09e9294f7218ef587ce6cp6,
    0x1.b333333333336p100
  },
  { // Entry 300
    -0x1.1a276ad639b09e9294f7218ef587ce6cp6,
    -0x1.b333333333336p100
  },
  { // Entry 301
    0x1.1a61f2927239a4e5d75ab70952b3595ap6,
    0x1.cccccccccccd0p100
  },
  { // Entry 302
    -0x1.1a61f2927239a4e5d75ab70952b3595ap6,
    -0x1.cccccccccccd0p100
  },
  { // Entry 303
    0x1.1a994ff83eca77f3ef91866a7b8540e2p6,
    0x1.e66666666666ap100
  },
  { // Entry 304
    -0x1.1a994ff83eca77f3ef91866a7b8540e2p6,
    -0x1.e66666666666ap100
  },
  { // Entry 305
    0x1.1acdd632f662a9e9c9c2e63a464b3927p6,
    0x1.0p101
  },
  { // Entry 306
    -0x1.1acdd632f662a9e9c9c2e63a464b3927p6,
    -0x1.0p101
  },
  { // Entry 307
    0x1.16a529a32777cd0fc3079004b633875fp7,
    0x1.0p200
  },
  { // Entry 308
    -0x1.16a529a32777cd0fc3079004b633875fp7,
    -0x1.0p200
  },
  { // Entry 309
    0x1.16d5f62219f05aa89db4c3750fbb01d6p7,
    0x1.199999999999ap200
  },
  { // Entry 310
    -0x1.16d5f62219f05aa89db4c3750fbb01d6p7,
    -0x1.199999999999ap200
  },
  { // Entry 311
    0x1.170282e36f0a26fdfd79f091b98c1570p7,
    0x1.3333333333334p200
  },
  { // Entry 312
    -0x1.170282e36f0a26fdfd79f091b98c1570p7,
    -0x1.3333333333334p200
  },
  { // Entry 313
    0x1.172b7e3f06859c0c372cea9b5bb8ff47p7,
    0x1.4cccccccccccep200
  },
  { // Entry 314
    -0x1.172b7e3f06859c0c372cea9b5bb8ff47p7,
    -0x1.4cccccccccccep200
  },
  { // Entry 315
    0x1.17516fb9f01171b1837eee7a719450a6p7,
    0x1.6666666666668p200
  },
  { // Entry 316
    -0x1.17516fb9f01171b1837eee7a719450a6p7,
    -0x1.6666666666668p200
  },
  { // Entry 317
    0x1.1774c2c28d748f9a0366a662dadefbf9p7,
    0x1.8000000000002p200
  },
  { // Entry 318
    -0x1.1774c2c28d748f9a0366a662dadefbf9p7,
    -0x1.8000000000002p200
  },
  { // Entry 319
    0x1.1795cdf3f8b103ac6a03f6f01a3a46c3p7,
    0x1.999999999999cp200
  },
  { // Entry 320
    -0x1.1795cdf3f8b103ac6a03f6f01a3a46c3p7,
    -0x1.999999999999cp200
  },
  { // Entry 321
    0x1.17b4d824b8c26657803575163dd9b7efp7,
    0x1.b333333333336p200
  },
  { // Entry 322
    -0x1.17b4d824b8c26657803575163dd9b7efp7,
    -0x1.b333333333336p200
  },
  { // Entry 323
    0x1.17d21c02d506e98121673fd36c6f7d66p7,
    0x1.cccccccccccd0p200
  },
  { // Entry 324
    -0x1.17d21c02d506e98121673fd36c6f7d66p7,
    -0x1.cccccccccccd0p200
  },
  { // Entry 325
    0x1.17edcab5bb4f53082d82a78400d8712ap7,
    0x1.e66666666666ap200
  },
  { // Entry 326
    -0x1.17edcab5bb4f53082d82a78400d8712ap7,
    -0x1.e66666666666ap200
  },
  { // Entry 327
    0x1.18080dd3171b6c031a9b576be63b6d4cp7,
    0x1.0p201
  },
  { // Entry 328
    -0x1.18080dd3171b6c031a9b576be63b6d4cp7,
    -0x1.0p201
  },
  { // Entry 329
    0x1.5aeb8fdc01b221605c35ac9eb3b88349p9,
    0x1.0p1000
  },
  { // Entry 330
    -0x1.5aeb8fdc01b221605c35ac9eb3b88349p9,
    -0x1.0p1000
  },
  { // Entry 331
    0x1.5af7c2fbbe5044c692e0f97aca1a61e7p9,
    0x1.199999999999ap1000
  },
  { // Entry 332
    -0x1.5af7c2fbbe5044c692e0f97aca1a61e7p9,
    -0x1.199999999999ap1000
  },
  { // Entry 333
    0x1.5b02e62c1396b7dbead244c1f48ea6cdp9,
    0x1.3333333333334p1000
  },
  { // Entry 334
    -0x1.5b02e62c1396b7dbead244c1f48ea6cdp9,
    -0x1.3333333333334p1000
  },
  { // Entry 335
    0x1.5b0d2502f975951f793f03445d19e143p9,
    0x1.4cccccccccccep1000
  },
  { // Entry 336
    -0x1.5b0d2502f975951f793f03445d19e143p9,
    -0x1.4cccccccccccep1000
  },
  { // Entry 337
    0x1.5b16a161b3d88a88cc53843c2290b59bp9,
    0x1.6666666666668p1000
  },
  { // Entry 338
    -0x1.5b16a161b3d88a88cc53843c2290b59bp9,
    -0x1.6666666666668p1000
  },
  { // Entry 339
    0x1.5b1f7623db315202ec4d72363ce36070p9,
    0x1.8000000000002p1000
  },
  { // Entry 340
    -0x1.5b1f7623db315202ec4d72363ce36070p9,
    -0x1.8000000000002p1000
  },
  { // Entry 341
    0x1.5b27b8f036006f0785f4c6598cba3322p9,
    0x1.999999999999cp1000
  },
  { // Entry 342
    -0x1.5b27b8f036006f0785f4c6598cba3322p9,
    -0x1.999999999999cp1000
  },
  { // Entry 343
    0x1.5b2f7b7c6604c7b24b8125e315a20f6dp9,
    0x1.b333333333336p1000
  },
  { // Entry 344
    -0x1.5b2f7b7c6604c7b24b8125e315a20f6dp9,
    -0x1.b333333333336p1000
  },
  { // Entry 345
    0x1.5b36cc73ed15e87cb3cd9892614780cbp9,
    0x1.cccccccccccd0p1000
  },
  { // Entry 346
    -0x1.5b36cc73ed15e87cb3cd9892614780cbp9,
    -0x1.cccccccccccd0p1000
  },
  { // Entry 347
    0x1.5b3db820a6a802de76d4727e8661bdbcp9,
    0x1.e66666666666ap1000
  },
  { // Entry 348
    -0x1.5b3db820a6a802de76d4727e8661bdbcp9,
    -0x1.e66666666666ap1000
  },
  { // Entry 349
    0x1.5b4448e7fd9b091d321a9e787fba7cc4p9,
    0x1.0p1001
  },
  { // Entry 350
    -0x1.5b4448e7fd9b091d321a9e787fba7cc4p9,
    -0x1.0p1001
  },
  { // Entry 351
    -0x1.633ce8fb9f87db1069ac5909d3e7d6d9p9,
    -0x1.fffffffffffffp1023
  },
  { // Entry 352
    0x1.633ce8fb9f87db1069ac5909d3e7d6d9p9,
    0x1.fffffffffffffp1023
  },
  { // Entry 353
    0x1.633ce8fb9f87db1069ac5909d3e7d6d9p9,
    0x1.fffffffffffffp1023
  },
  { // Entry 354
    -0x1.633ce8fb9f87db1069ac5909d3e7d6d9p9,
    -0x1.fffffffffffffp1023
  },
  { // Entry 355
    -0.0,
    -0x1.0p-1074
  },
  { // Entry 356
    0.0,
    0x1.0p-1074
  },
  { // Entry 357
    -0.0,
    -0.0
  },
  { // Entry 358
    0.0,
    0x1.0p-1074
  },
  { // Entry 359
    -0.0,
    -0x1.0p-1074
  },
  { // Entry 360
    -0x1.0499e40c65ff571c4214191e409f886cp-1,
    -0x1.1000000000001p-1
  },
  { // Entry 361
    0x1.0499e40c65ff571c4214191e409f886cp-1,
    0x1.1000000000001p-1
  },
  { // Entry 362
    -0x1.0499e40c65ff48fb04213eab32faa345p-1,
    -0x1.1p-1
  },
  { // Entry 363
    0x1.0499e40c65ff48fb04213eab32faa345p-1,
    0x1.1p-1
  },
  { // Entry 364
    -0x1.0499e40c65ff3ad9c62e64382526e89ap-1,
    -0x1.0ffffffffffffp-1
  },
  { // Entry 365
    0x1.0499e40c65ff3ad9c62e64382526e89ap-1,
    0x1.0ffffffffffffp-1
  },
  { // Entry 366
    0x1.0499e40c65ff3ad9c62e64382526e89ap-1,
    0x1.0ffffffffffffp-1
  },
  { // Entry 367
    -0x1.0499e40c65ff3ad9c62e64382526e89ap-1,
    -0x1.0ffffffffffffp-1
  },
  { // Entry 368
    0x1.0499e40c65ff48fb04213eab32faa345p-1,
    0x1.1p-1
  },
  { // Entry 369
    -0x1.0499e40c65ff48fb04213eab32faa345p-1,
    -0x1.1p-1
  },
  { // Entry 370
    0x1.0499e40c65ff571c4214191e409f886cp-1,
    0x1.1000000000001p-1
  },
  { // Entry 371
    -0x1.0499e40c65ff571c4214191e409f886cp-1,
    -0x1.1000000000001p-1
  },
  { // Entry 372
    0x1.62e42fefa39ef31793c7673007e4ed5ep5,
    0x1.fffffffffffffp62
  },
  { // Entry 373
    -0x1.62e42fefa39ef31793c7673007e4ed5ep5,
    -0x1.fffffffffffffp62
  },
  { // Entry 374
    0x1.62e42fefa39ef35793c7673007e5ed5ep5,
    0x1.0p63
  },
  { // Entry 375
    -0x1.62e42fefa39ef35793c7673007e5ed5ep5,
    -0x1.0p63
  },
  { // Entry 376
    0x1.62e42fefa39ef3d793c7673007e1ed5ep5,
    0x1.0000000000001p63
  },
  { // Entry 377
    -0x1.62e42fefa39ef3d793c7673007e1ed5ep5,
    -0x1.0000000000001p63
  },
  { // Entry 378
    0x1.419ecb712c480c0b5decb58387269d9dp4,
    0x1.fffffffffffffp27
  },
  { // Entry 379
    -0x1.419ecb712c480c0b5decb58387269d9dp4,
    -0x1.fffffffffffffp27
  },
  { // Entry 380
    0x1.419ecb712c480c8b5decb58387285d9dp4,
    0x1.0p28
  },
  { // Entry 381
    -0x1.419ecb712c480c8b5decb58387285d9dp4,
    -0x1.0p28
  },
  { // Entry 382
    0x1.419ecb712c480d8b5decb583871fdd9dp4,
    0x1.0000000000001p28
  },
  { // Entry 383
    -0x1.419ecb712c480d8b5decb583871fdd9dp4,
    -0x1.0000000000001p28
  },
  { // Entry 384
    0x1.3687a9f1af2b147ca14e7a4a06e817b2p4,
    0x1.fffffffffffffp26
  },
  { // Entry 385
    -0x1.3687a9f1af2b147ca14e7a4a06e817b2p4,
    -0x1.fffffffffffffp26
  },
  { // Entry 386
    0x1.3687a9f1af2b14fca14e7a4a06e917b2p4,
    0x1.0p27
  },
  { // Entry 387
    -0x1.3687a9f1af2b14fca14e7a4a06e917b2p4,
    -0x1.0p27
  },
  { // Entry 388
    0x1.3687a9f1af2b15fca14e7a4a06df17b2p4,
    0x1.0000000000001p27
  },
  { // Entry 389
    -0x1.3687a9f1af2b15fca14e7a4a06df17b2p4,
    -0x1.0000000000001p27
  },
  { // Entry 390
    0x1.1542457337d4319c6b73c89d84e9a171p4,
    0x1.fffffffffffffp23
  },
  { // Entry 391
    -0x1.1542457337d4319c6b73c89d84e9a171p4,
    -0x1.fffffffffffffp23
  },
  { // Entry 392
    0x1.1542457337d4321c6b73c89d84aba171p4,
    0x1.0p24
  },
  { // Entry 393
    -0x1.1542457337d4321c6b73c89d84aba171p4,
    -0x1.0p24
  },
  { // Entry 394
    0x1.1542457337d4331c6b73c89d8423a171p4,
    0x1.0000000000001p24
  },
  { // Entry 395
    -0x1.1542457337d4331c6b73c89d8423a171p4,
    -0x1.0000000000001p24
  },
  { // Entry 396
    0x1.0c1f8a6e80eeae5c96894f2bffb535afp1,
    0x1.fffffffffffffp1
  },
  { // Entry 397
    -0x1.0c1f8a6e80eeae5c96894f2bffb535afp1,
    -0x1.fffffffffffffp1
  },
  { // Entry 398
    0x1.0c1f8a6e80eeb23e03926981e348eb2ep1,
    0x1.0p2
  },
  { // Entry 399
    -0x1.0c1f8a6e80eeb23e03926981e348eb2ep1,
    -0x1.0p2
  },
  { // Entry 400
    0x1.0c1f8a6e80eeba00dda49e2daa18ae71p1,
    0x1.0000000000001p2
  },
  { // Entry 401
    -0x1.0c1f8a6e80eeba00dda49e2daa18ae71p1,
    -0x1.0000000000001p2
  },
  { // Entry 402
    0x1.719218313d086bd11ec0138398310287p0,
    0x1.fffffffffffffp0
  },
  { // Entry 403
    -0x1.719218313d086bd11ec0138398310287p0,
    -0x1.fffffffffffffp0
  },
  { // Entry 404
    0x1.719218313d0872f8e831837f0e954189p0,
    0x1.0p1
  },
  { // Entry 405
    -0x1.719218313d0872f8e831837f0e954189p0,
    -0x1.0p1
  },
  { // Entry 406
    0x1.719218313d0881487b146375fad45d3fp0,
    0x1.0000000000001p1
  },
  { // Entry 407
    -0x1.719218313d0881487b146375fad45d3fp0,
    -0x1.0000000000001p1
  },
  { // Entry 408
    0x1.c34366179d42617162bffd7dbe442e71p-1,
    0x1.fffffffffffffp-1
  },
  { // Entry 409
    -0x1.c34366179d42617162bffd7dbe442e71p-1,
    -0x1.fffffffffffffp-1
  },
  { // Entry 410
    0x1.c34366179d426cc1b1f33d1ba4a314a7p-1,
    0x1.0p0
  },
  { // Entry 411
    -0x1.c34366179d426cc1b1f33d1ba4a314a7p-1,
    -0x1.0p0
  },
  { // Entry 412
    0x1.c34366179d4283625059bc5770d91d5dp-1,
    0x1.0000000000001p0
  },
  { // Entry 413
    -0x1.c34366179d4283625059bc5770d91d5dp-1,
    -0x1.0000000000001p0
  },
  { // Entry 414
    0x1.ecc2caec51608afc4d5f24b27c20dc9cp-2,
    0x1.fffffffffffffp-2
  },
  { // Entry 415
    -0x1.ecc2caec51608afc4d5f24b27c20dc9cp-2,
    -0x1.fffffffffffffp-2
  },
  { // Entry 416
    0x1.ecc2caec5160994be04204a968c7020dp-2,
    0x1.0p-1
  },
  { // Entry 417
    -0x1.ecc2caec5160994be04204a968c7020dp-2,
    -0x1.0p-1
  },
  { // Entry 418
    0x1.ecc2caec5160b5eb0607c49741ce9bc6p-2,
    0x1.0000000000001p-1
  },
  { // Entry 419
    -0x1.ecc2caec5160b5eb0607c49741ce9bc6p-2,
    -0x1.0000000000001p-1
  },
  { // Entry 420
    0x1.facfb2399e635f07b2ecf48db28f6b82p-3,
    0x1.fffffffffffffp-3
  },
  { // Entry 421
    -0x1.facfb2399e635f07b2ecf48db28f6b82p-3,
    -0x1.fffffffffffffp-3
  },
  { // Entry 422
    0x1.facfb2399e636e8d67115de540a778acp-3,
    0x1.0p-2
  },
  { // Entry 423
    -0x1.facfb2399e636e8d67115de540a778acp-3,
    -0x1.0p-2
  },
  { // Entry 424
    0x1.facfb2399e638d98cf5a30945cc1a910p-3,
    0x1.0000000000001p-2
  },
  { // Entry 425
    -0x1.facfb2399e638d98cf5a30945cc1a910p-3,
    -0x1.0000000000001p-2
  },
  { // Entry 426
    0x1.fead0b6996971d25a6c9ee383ff9d971p-4,
    0x1.fffffffffffffp-4
  },
  { // Entry 427
    -0x1.fead0b6996971d25a6c9ee383ff9d971p-4,
    -0x1.fffffffffffffp-4
  },
  { // Entry 428
    0x1.fead0b6996972d06058e3eb12c938140p-4,
    0x1.0p-3
  },
  { // Entry 429
    -0x1.fead0b6996972d06058e3eb12c938140p-4,
    -0x1.0p-3
  },
  { // Entry 430
    0x1.fead0b6996974cc6c316dfa305c0f42dp-4,
    0x1.0000000000001p-3
  },
  { // Entry 431
    -0x1.fead0b6996974cc6c316dfa305c0f42dp-4,
    -0x1.0000000000001p-3
  },
  { // Entry 432
    0x1.ffaad0fa4526179f69f8625dbfeb270fp-5,
    0x1.fffffffffffffp-5
  },
  { // Entry 433
    -0x1.ffaad0fa4526179f69f8625dbfeb270fp-5,
    -0x1.fffffffffffffp-5
  },
  { // Entry 434
    0x1.ffaad0fa452627976ff366b9d3840fd1p-5,
    0x1.0p-4
  },
  { // Entry 435
    -0x1.ffaad0fa452627976ff366b9d3840fd1p-5,
    -0x1.0p-4
  },
  { // Entry 436
    0x1.ffaad0fa452647877be96f71fab46392p-5,
    0x1.0000000000001p-4
  },
  { // Entry 437
    -0x1.ffaad0fa452647877be96f71fab46392p-5,
    -0x1.0000000000001p-4
  },
  { // Entry 438
    0x1.ffeaad10b5b2d593fd4d7fd398a04e17p-6,
    0x1.fffffffffffffp-6
  },
  { // Entry 439
    -0x1.ffeaad10b5b2d593fd4d7fd398a04e17p-6,
    -0x1.fffffffffffffp-6
  },
  { // Entry 440
    0x1.ffeaad10b5b2e591fdad6bd7f7a497c3p-6,
    0x1.0p-5
  },
  { // Entry 441
    -0x1.ffeaad10b5b2e591fdad6bd7f7a497c3p-6,
    -0x1.0p-5
  },
  { // Entry 442
    0x1.ffeaad10b5b3058dfe6d43e0b5accb41p-6,
    0x1.0000000000001p-5
  },
  { // Entry 443
    -0x1.ffeaad10b5b3058dfe6d43e0b5accb41p-6,
    -0x1.0000000000001p-5
  },
  { // Entry 444
    0x1.fffaaad10fa359c3a4fad4bba332af54p-7,
    0x1.fffffffffffffp-7
  },
  { // Entry 445
    -0x1.fffaaad10fa359c3a4fad4bba332af54p-7,
    -0x1.fffffffffffffp-7
  },
  { // Entry 446
    0x1.fffaaad10fa369c32500d46ba7927458p-7,
    0x1.0p-6
  },
  { // Entry 447
    -0x1.fffaaad10fa369c32500d46ba7927458p-7,
    -0x1.0p-6
  },
  { // Entry 448
    0x1.fffaaad10fa389c2250cd3cbb051e660p-7,
    0x1.0000000000001p-6
  },
  { // Entry 449
    -0x1.fffaaad10fa389c2250cd3cbb051e660p-7,
    -0x1.0000000000001p-6
  },
  { // Entry 450
    0x1.fffffffaaaaa9ad111118fa35a2fb2e8p-15,
    0x1.fffffffffffffp-15
  },
  { // Entry 451
    -0x1.fffffffaaaaa9ad111118fa35a2fb2e8p-15,
    -0x1.fffffffffffffp-15
  },
  { // Entry 452
    0x1.fffffffaaaaaaad111110fa35a35b2e8p-15,
    0x1.0p-14
  },
  { // Entry 453
    -0x1.fffffffaaaaaaad111110fa35a35b2e8p-15,
    -0x1.0p-14
  },
  { // Entry 454
    0x1.fffffffaaaaacad111100fa35a41b2e8p-15,
    0x1.0000000000001p-14
  },
  { // Entry 455
    -0x1.fffffffaaaaacad111100fa35a41b2e8p-15,
    -0x1.0000000000001p-14
  },
  { // Entry 456
    0x1.ffffffffffffeeaaaaaaaaaaaacd1111p-28,
    0x1.fffffffffffffp-28
  },
  { // Entry 457
    -0x1.ffffffffffffeeaaaaaaaaaaaacd1111p-28,
    -0x1.fffffffffffffp-28
  },
  { // Entry 458
    0x1.fffffffffffffeaaaaaaaaaaaaad1111p-28,
    0x1.0p-27
  },
  { // Entry 459
    -0x1.fffffffffffffeaaaaaaaaaaaaad1111p-28,
    -0x1.0p-27
  },
  { // Entry 460
    0x1.0000000000000f555555555555368888p-27,
    0x1.0000000000001p-27
  },
  { // Entry 461
    -0x1.0000000000000f555555555555368888p-27,
    -0x1.0000000000001p-27
  },
  { // Entry 462
    0x1.ffffffffffffefaaaaaaaaaaaab2d111p-29,
    0x1.fffffffffffffp-29
  },
  { // Entry 463
    -0x1.ffffffffffffefaaaaaaaaaaaab2d111p-29,
    -0x1.fffffffffffffp-29
  },
  { // Entry 464
    0x1.ffffffffffffffaaaaaaaaaaaaaad111p-29,
    0x1.0p-28
  },
  { // Entry 465
    -0x1.ffffffffffffffaaaaaaaaaaaaaad111p-29,
    -0x1.0p-28
  },
  { // Entry 466
    0x1.0000000000000fd555555555554d6888p-28,
    0x1.0000000000001p-28
  },
  { // Entry 467
    -0x1.0000000000000fd555555555554d6888p-28,
    -0x1.0000000000001p-28
  },
  { // Entry 468
    0x1.ffffffffffffeffaaaaaaaaaaaab2ad1p-31,
    0x1.fffffffffffffp-31
  },
  { // Entry 469
    -0x1.ffffffffffffeffaaaaaaaaaaaab2ad1p-31,
    -0x1.fffffffffffffp-31
  },
  { // Entry 470
    0x1.fffffffffffffffaaaaaaaaaaaaaaad1p-31,
    0x1.0p-30
  },
  { // Entry 471
    -0x1.fffffffffffffffaaaaaaaaaaaaaaad1p-31,
    -0x1.0p-30
  },
  { // Entry 472
    0x1.0000000000000ffd555555555554d568p-30,
    0x1.0000000000001p-30
  },
  { // Entry 473
    -0x1.0000000000000ffd555555555554d568p-30,
    -0x1.0000000000001p-30
  },
  { // Entry 474
    HUGE_VAL,
    HUGE_VAL
  },
  { // Entry 475
    -HUGE_VAL,
    -HUGE_VAL
  },
  { // Entry 476
    0x1.633ce8fb9f87db1069ac5909d3e7d6d9p9,
    0x1.fffffffffffffp1023
  },
  { // Entry 477
    -0x1.633ce8fb9f87db1069ac5909d3e7d6d9p9,
    -0x1.fffffffffffffp1023
  },
  { // Entry 478
    0x1.633ce8fb9f87db0c69ac5909d3e7a6d9p9,
    0x1.ffffffffffffep1023
  },
  { // Entry 479
    -0x1.633ce8fb9f87db0c69ac5909d3e7a6d9p9,
    -0x1.ffffffffffffep1023
  },
  { // Entry 480
    0x1.dcbf69f10006cbe9c11ca9a5d76ab0c1p0,
    0x1.921fb54442d18p1
  },
  { // Entry 481
    -0x1.dcbf69f10006cbe9c11ca9a5d76ab0c1p0,
    -0x1.921fb54442d18p1
  },
  { // Entry 482
    0x1.3bc04e847ec0514731ddcb476d407d39p0,
    0x1.921fb54442d18p0
  },
  { // Entry 483
    -0x1.3bc04e847ec0514731ddcb476d407d39p0,
    -0x1.921fb54442d18p0
  },
  { // Entry 484
    0x1.c34366179d4283625059bc5770d91d5dp-1,
    0x1.0000000000001p0
  },
  { // Entry 485
    -0x1.c34366179d4283625059bc5770d91d5dp-1,
    -0x1.0000000000001p0
  },
  { // Entry 486
    0x1.c34366179d426cc1b1f33d1ba4a314a7p-1,
    0x1.0p0
  },
  { // Entry 487
    -0x1.c34366179d426cc1b1f33d1ba4a314a7p-1,
    -0x1.0p0
  },
  { // Entry 488
    0x1.c34366179d42617162bffd7dbe442e71p-1,
    0x1.fffffffffffffp-1
  },
  { // Entry 489
    -0x1.c34366179d42617162bffd7dbe442e71p-1,
    -0x1.fffffffffffffp-1
  },
  { // Entry 490
    0x1.7144779e3f0ba7a6bf77ae922933a297p-1,
    0x1.921fb54442d18p-1
  },
  { // Entry 491
    -0x1.7144779e3f0ba7a6bf77ae922933a297p-1,
    -0x1.921fb54442d18p-1
  },
  { // Entry 492
    0x1.0000000000000fffffffffffffffffffp-1022,
    0x1.0000000000001p-1022
  },
  { // Entry 493
    -0x1.0000000000000fffffffffffffffffffp-1022,
    -0x1.0000000000001p-1022
  },
  { // Entry 494
    0x1.ffffffffffffffffffffffffffffffffp-1023,
    0x1.0p-1022
  },
  { // Entry 495
    -0x1.ffffffffffffffffffffffffffffffffp-1023,
    -0x1.0p-1022
  },
  { // Entry 496
    0x1.ffffffffffffdfffffffffffffffffffp-1023,
    0x1.ffffffffffffep-1023
  },
  { // Entry 497
    -0x1.ffffffffffffdfffffffffffffffffffp-1023,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 498
    0x1.ffffffffffffbfffffffffffffffffffp-1023,
    0x1.ffffffffffffcp-1023
  },
  { // Entry 499
    -0x1.ffffffffffffbfffffffffffffffffffp-1023,
    -0x1.ffffffffffffcp-1023
  },
  { // Entry 500
    0x1.ffffffffffffffffffffffffffffffffp-1074,
    0x1.0p-1073
  },
  { // Entry 501
    -0x1.ffffffffffffffffffffffffffffffffp-1074,
    -0x1.0p-1073
  },
  { // Entry 502
    0.0,
    0x1.0p-1074
  },
  { // Entry 503
    -0.0,
    -0x1.0p-1074
  },
  { // Entry 504
    0.0,
    0.0
  },
  { // Entry 505
    -0.0,
    -0.0
  }
};
