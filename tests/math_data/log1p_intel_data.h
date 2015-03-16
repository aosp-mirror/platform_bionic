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

static data_1_1_t<double, double> g_log1p_intel_data[] = {
  { // Entry 0
    -0x1.00000000000010p-1022,
    -0x1.0000000000001p-1022
  },
  { // Entry 1
    -0x1.0415d89e74446809b5d7e16e90dcfb17p-5,
    -0x1.0000000000002p-5
  },
  { // Entry 2
    -0x1.269621334db92803beb76a16b5547d4dp-2,
    -0x1.000000180p-2
  },
  { // Entry 3
    -0x1.00080000000008008002000000555d55p-52,
    -0x1.00080p-52
  },
  { // Entry 4
    -0x1.001000000040080040001559559556b2p-41,
    -0x1.001p-41
  },
  { // Entry 5
    -0x1.65724c2110f35416c9322de1fbce6ea0p-2,
    -0x1.2dba262a7c8b0p-2
  },
  { // Entry 6
    -0x1.5af4179028eb7638f1145bd433d6c831p-11,
    -0x1.5ad6b5ad6b5b0p-11
  },
  { // Entry 7
    -0x1.74fc36f06cd5b7ffd79b0ff90a64bb6ep-6,
    -0x1.70c58e67b7aeap-6
  },
  { // Entry 8
    -0x1.d62e0cd7372ac11cfb2f285d279dc3d4p-2,
    -0x1.7905b82a1839dp-2
  },
  { // Entry 9
    -0x1.7d9c1debf082f7fe3df487d0e4823676p-6,
    -0x1.793331eece596p-6
  },
  { // Entry 10
    -0x1.e148a1a2726cbfb45f343d2e78b71a51p-2,
    -0x1.7ffffffffffffp-2
  },
  { // Entry 11
    -0x1.9baf61134c048801e3731883b65290b3p-5,
    -0x1.9182fde7e3318p-5
  },
  { // Entry 12
    -0x1.ff93ccbd3124237a5b3cf16c47c915c9p-2,
    -0x1.92a7fc86dcbd9p-2
  },
  { // Entry 13
    -0x1.8feb5ba3c5b0d7fddffe1b9133bd3827p0,
    -0x1.94a5294a5294cp-1
  },
  { // Entry 14
    -0x1.e265a1d9483178002922dd984d33f198p-4,
    -0x1.c712d0d7f0490p-4
  },
  { // Entry 15
    -0x1.cd4e4c03a55707e868994265170eefb8p-10,
    -0x1.cce673399cce8p-10
  },
  { // Entry 16
    -0x1.e5df7f9b307ac000115f8473c90fb515p-9,
    -0x1.e4f93e4f93e50p-9
  },
  { // Entry 17
    -0x1.f96ef48ecd4037fe220ffae33fef5d04p-5,
    -0x1.ea28302403580p-5
  },
  { // Entry 18
    -0x1.fdd09f73d7f688dd5508c770fe7b7a9fp-5,
    -0x1.ee4675d0ac9aap-5
  },
  { // Entry 19
    -0x1.ffffffffffe4b72dd5ac98791a728e1fp-5,
    -0x1.f0540438fd429p-5
  },
  { // Entry 20
    -0x1.ff7fac9bb11607daf86980492f147eedp-10,
    -0x1.ff0001ffffffep-10
  },
  { // Entry 21
    -0x1.ffe7fffffffff7ff40047fffffaaaaaap-54,
    -0x1.ffe7fffffffffp-54
  },
  { // Entry 22
    -0x1.4cb9ed50b6bc79d44d301801ce0ff6f3p4,
    -0x1.fffffff801fffp-1
  },
  { // Entry 23
    -0x1.57cd0e3026827bbcd5d3d6a532515bd1p4,
    -0x1.fffffffbfffffp-1
  },
  { // Entry 24
    -0x1.002005545508732d7b57a1ec86bd5c7ap-10,
    -0x1.fffffffc0p-11
  },
  { // Entry 25
    -0x1.08598b57c1806001dbb99c0aebf44bdep-4,
    -0x1.fffffffc0003fp-5
  },
  { // Entry 26
    -0x1.08598b58e3a06001bf513750331fb25cp-4,
    -0x1.fffffffe1ffffp-5
  },
  { // Entry 27
    -0x1.ffffffff000007fffffff800002aacaap-54,
    -0x1.ffffffff0p-54
  },
  { // Entry 28
    -0x1.00000000001ff7fffffff54d55555547p-40,
    -0x1.ffffffffff3ffp-41
  },
  { // Entry 29
    -0x1.ffffffffffffeffffffffffffeaaaaaap-53,
    -0x1.ffffffffffffep-53
  },
  { // Entry 30
    0x1.ffffffffff0000000000aaaaaaaaaa2ap-41,
    0x1.0p-40
  },
  { // Entry 31
    0x1.ffffffffffffffffffffffffffffffp-121,
    0x1.0p-120
  },
  { // Entry 32
    0.0,
    0x1.0p-1074
  },
  { // Entry 33
    0x1.fffffffffffffffffffffffffeaaaaaap-52,
    0x1.0000000000001p-51
  },
  { // Entry 34
    0x1.00000000000007ffffffffffff555555p-52,
    0x1.0000000000001p-52
  },
  { // Entry 35
    0x1.9f323ecbf9855480be2cbc494f93df36p-2,
    0x1.0000000000007p-1
  },
  { // Entry 36
    0x1.ffffffffffc0000000000aaaaaaaaaa8p-42,
    0x1.00000000002p-41
  },
  { // Entry 37
    0x1.ffffffffffdffffffffffaaaaaaaaaaep-42,
    0x1.00000000003p-41
  },
  { // Entry 38
    0x1.ffe002ae6a31006877edb3328bd3ae91p-12,
    0x1.00000001fffffp-11
  },
  { // Entry 39
    0x1.9f323f094c68a8000013901093412da6p-2,
    0x1.0000002dfe2afp-1
  },
  { // Entry 40
    0x1.9f323f094c692800000be5b40e615d2dp-2,
    0x1.0000002dfe2b5p-1
  },
  { // Entry 41
    0x1.193ea82ad0308976a42437ffabe62762p0,
    0x1.000000cp1
  },
  { // Entry 42
    0x1.f0b21b0c9a27f7973092bef2b8a18d80p-5,
    0x1.00080p-4
  },
  { // Entry 43
    0x1.1ace1631f668001f17e5430537a94f9fp5,
    0x1.00080p51
  },
  { // Entry 44
    0x1.000fffffffbff7ffc0001559559556a2p-41,
    0x1.001p-41
  },
  { // Entry 45
    0x1.f31cdeeb3cd4c7c0a3e945ad856befcbp4,
    0x1.00cp45
  },
  { // Entry 46
    0x1.206360b7e569587b36009d7c942d4f3cp5,
    0x1.014p52
  },
  { // Entry 47
    0x1.f333a5f5edb1b76e16684e60b7181719p-5,
    0x1.015cdfc51f91cp-4
  },
  { // Entry 48
    0x1.64892563f80250000b60adaac677e2eap-1,
    0x1.01a5a2b15fc5cp0
  },
  { // Entry 49
    0x1.0482afcf527d98002bc41c40cd3b44c5p-23,
    0x1.0482b0d86c362p-23
  },
  { // Entry 50
    0x1.045dcf2cb15f57fe3f2ed152226368c8p-5,
    0x1.088c59ac8c7d1p-5
  },
  { // Entry 51
    0x1.015e05876e3e67fff047c696eba44ba2p-4,
    0x1.09ap-4
  },
  { // Entry 52
    0x1.0b6515d81d9732694cd7ec512fc6f1b4p-11,
    0x1.0b768b5ad8019p-11
  },
  { // Entry 53
    0x1.b346a1d28f44d7fdcee7a0bd07405845p-2,
    0x1.0f35566ed3cc2p-1
  },
  { // Entry 54
    0x1.b3cce9b7221757feb43dcf531070c894p0,
    0x1.1f27c14e425b9p2
  },
  { // Entry 55
    0x1.fbc379bd13a6b00091e8da2307a3712fp-3,
    0x1.202p-2
  },
  { // Entry 56
    0x1.2140a33ee4f537fe4de38bae4056e098p-5,
    0x1.266b753946441p-5
  },
  { // Entry 57
    0x1.d6bfbea5ab7fd4c43b30348da32e2a7dp-2,
    0x1.2ad0c02f60402p-1
  },
  { // Entry 58
    0x1.c09da5a8b37876f669efaffd93412f9ap0,
    0x1.312e7b7be62a5p2
  },
  { // Entry 59
    0x1.e3a91d4d7516cb9db08fd3c3cf7d40cap-2,
    0x1.351a8d46a35p-1
  },
  { // Entry 60
    0x1.f128f5faf06ecb35c83b1131cf5d73d5p-2,
    0x1.4p-1
  },
  { // Entry 61
    0x1.f1ee31f14d4f17ffde2f2fe766dfc318p-2,
    0x1.40a0502814080p-1
  },
  { // Entry 62
    0x1.41e3e450b6073001c502b22fec3ab4d7p-5,
    0x1.484c43acc194cp-5
  },
  { // Entry 63
    0x1.4d9ff934d99f37ff40fd39eb618dcd3ap-21,
    0x1.4dap-21
  },
  { // Entry 64
    0x1.a0711f9b475687ffffd2981b5b49910ep2,
    0x1.4e5fffff0p9
  },
  { // Entry 65
    0x1.e1905175711a17c09fd40254fad72ae8p4,
    0x1.56f3052920ef0p43
  },
  { // Entry 66
    0x1.4f7ef3b13e1fa800361c4277dfa1092ap-4,
    0x1.5d9e6884d6ac2p-4
  },
  { // Entry 67
    0x1.e45c01e8c233cffe5ac108bc6c123bfap0,
    0x1.688p2
  },
  { // Entry 68
    0x1.c34366179d4258048e0ec51c6fefd58cp-1,
    0x1.6a09e667f3bcbp0
  },
  { // Entry 69
    0x1.e2af1644433ac7c03096da53cf28c18ap4,
    0x1.6fd1ffb140878p43
  },
  { // Entry 70
    0x1.6392a510033287ffc3d40d8ce33d1627p-4,
    0x1.73767fd8537b3p-4
  },
  { // Entry 71
    0x1.d36a35aaae768800f77be0b2a29e40b7p-1,
    0x1.7dd89e50e078ep0
  },
  { // Entry 72
    0x1.83ffed9f8129180039f0eacf23501c34p-20,
    0x1.840p-20
  },
  { // Entry 73
    0x1.8996381ef2cb67ff2c1a031d8e88fa81p-8,
    0x1.8ac562b158ac4p-8
  },
  { // Entry 74
    0x1.926499264fd877fe77bab85881dbab74p-43,
    0x1.926499265p-43
  },
  { // Entry 75
    0x1.e737cb23865c6b921552ad81d572b729p-1,
    0x1.970p0
  },
  { // Entry 76
    0x1.588c2de5e88db000000ea4e59847d15cp-2,
    0x1.99999a1030f9dp-2
  },
  { // Entry 77
    0x1.588c2df2c02057ffffefc30ff25d79ddp-2,
    0x1.99999a222b93fp-2
  },
  { // Entry 78
    0x1.756501be3e242800001019cd7cd7ce3fp-3,
    0x1.99999a598c15cp-3
  },
  { // Entry 79
    0x1.756502257dbf5000000bc0ddc72156fap-3,
    0x1.99999ad572033p-3
  },
  { // Entry 80
    0x1.8663f9903e12effffff039fafc6b5f67p-4,
    0x1.99999bc8ec375p-4
  },
  { // Entry 81
    0x1.9bd8abb150fbd005aa9e2ed5a074a08ep-21,
    0x1.9bd8b60b96e2fp-21
  },
  { // Entry 82
    0x1.9955bad1e36537ffd7fd8448d392de25p-7,
    0x1.9be6f9be6f9b1p-7
  },
  { // Entry 83
    0x1.5ba06e3fb01a2d107ec5201223f00bbbp-2,
    0x1.9dead086a58cdp-2
  },
  { // Entry 84
    0x1.5f1a557f41f26cc673db4f91686a3758p-2,
    0x1.a2ce8df554b2cp-2
  },
  { // Entry 85
    0x1.62405ebd6ab333837c8a77026ab4aae8p-2,
    0x1.a74p-2
  },
  { // Entry 86
    0x1.f5f73d69114c2b85b3b151d45a33d0e5p-1,
    0x1.aa6p0
  },
  { // Entry 87
    0x1.b229fbeca7781fffe6f5fdb97b7242c6p-5,
    0x1.bdep-5
  },
  { // Entry 88
    0x1.bc21a8cfe0c4178b34990a731d3fbd15p-5,
    0x1.c86432190c8p-5
  },
  { // Entry 89
    0x1.07952367af5c880000105e2b54a5a062p0,
    0x1.cccccced2ed7ep0
  },
  { // Entry 90
    0x1.c1de8bc3181ba001c1b60c40eff90650p-5,
    0x1.ce7375b5023c4p-5
  },
  { // Entry 91
    0x1.d59efda67795a800fddf8c5bba4a60b3p-43,
    0x1.d59efda677cb8p-43
  },
  { // Entry 92
    0x1.d80158c4069057ff768740aa80c0bd66p-7,
    0x1.db6bcf502f3e0p-7
  },
  { // Entry 93
    0x1.dfeabe29b510312e8367f414b0511949p-11,
    0x1.e022fd930f86ap-11
  },
  { // Entry 94
    0x1.8a9a59caf11980a5915d2b6b7cf2553dp-2,
    0x1.e16b24d38d1b2p-2
  },
  { // Entry 95
    0x1.8f11e873662c77e1769d569868a65e72p-2,
    0x1.e80p-2
  },
  { // Entry 96
    0x1.dd166106e87f37622aac2c908d6aaf91p-5,
    0x1.eb40e151fad81p-5
  },
  { // Entry 97
    0x1.ec80ffffffc4b7fe6ff009824ddc235ap-43,
    0x1.ec80fffffffffp-43
  },
  { // Entry 98
    0x1.edf52c2e34740b24b736dca45fb4ae9ep-13,
    0x1.ee0410e3b1d24p-13
  },
  { // Entry 99
    0x1.f02717d855569ffe85bb9f224358afeap-6,
    0x1.f7bdd6789c670p-6
  },
  { // Entry 100
    0x1.fbfffffffe07e80000029aea55555174p-40,
    0x1.fbfffffffffffp-40
  },
  { // Entry 101
    0x1.fbfffffffe07f80000029aca95555174p-40,
    0x1.fc0p-40
  },
  { // Entry 102
    0x1.367799dc39a238068eae0d5339eafee2p5,
    0x1.fc00000000006p55
  },
  { // Entry 103
    0x1.ffc7ffffff0027fcf000aa82af0a71p-41,
    0x1.ffc7fffffffffp-41
  },
  { // Entry 104
    0x1.ffdfffffffffe800fff8000000aa9aabp-54,
    0x1.ffdffffffffffp-54
  },
  { // Entry 105
    0x1.fff7fffffefff7fff000aab2aa8aa9ffp-41,
    0x1.fff7fffffffffp-41
  },
  { // Entry 106
    0x1.fff7ffffffffe8003fff800000aaa6aap-54,
    0x1.fff7fffffffffp-54
  },
  { // Entry 107
    0x1.c55179395a000800ddc334790469d4dep7,
    0x1.fffffe3ffffffp326
  },
  { // Entry 108
    0x1.ffc00aa4ac10abd44706d89cf12892a3p-11,
    0x1.fffffffbfffffp-11
  },
  { // Entry 109
    0x1.ffc00aa7ab50ebc44bf56111ce332375p-11,
    0x1.ffffffff0p-11
  },
  { // Entry 110
    0x1.25e4f7b2737f9fc486612173c6596892p5,
    0x1.ffffffffffff8p52
  },
  { // Entry 111
    0x1.62e42fefa39ef33793c7673007e1ed5ep9,
    0x1.ffffffffffff8p1023
  },
  { // Entry 112
    0x1.9f323ecbf98489d61382119eae69348bp-2,
    0x1.ffffffffffffbp-2
  },
  { // Entry 113
    0x1.5ca72d17ed3ea80089ae65dfafc1e2b2p8,
    0x1.ffffffffffffep502
  },
  { // Entry 114
    0x1.11d14e1fcb72edcb28a032e083a6f199p-1,
    0x1.6a09e667f3bccp-1
  },
  { // Entry 115
    0x1.2ccac6c8f41b74d6b733c9141c0bece5p-1,
    0x1.995255f2d00abp-1
  },
  { // Entry 116
    0x1.466a9269707376e50187259ee2b04818p-1,
    0x1.c89ac57dac58ap-1
  },
  { // Entry 117
    0x1.5ed1a7dce11ace55a3cdbe341ee88222p-1,
    0x1.f7e3350888a69p-1
  },
  { // Entry 118
    0x1.761c7d9dddc01d509dcb9b4ebceca84ep-1,
    0x1.1395d249b27a4p0
  },
  { // Entry 119
    0x1.8c63d27d4ca03daba8c98a232b2380f0p-1,
    0x1.2b3a0a0f20a14p0
  },
  { // Entry 120
    0x1.a1bd4c77d55363ab3b61dc89f7812c71p-1,
    0x1.42de41d48ec84p0
  },
  { // Entry 121
    0x1.b63bf7baf5eaa6eadec65ed0408ff964p-1,
    0x1.5a827999fcef4p0
  },
  { // Entry 122
    0x1.c9f0ad341cbebd1d84ae0c2674a34983p-1,
    0x1.7226b15f6b164p0
  },
  { // Entry 123
    0x1.dcea661b59e7f2a61f64bc6d943ab5aep-1,
    0x1.89cae924d93d4p0
  },
  { // Entry 124
    0x1.ef36808e501ff5bc97de3be617ad08b5p-1,
    0x1.a16f20ea47644p0
  },
  { // Entry 125
    0x1.00707c29c4643ea6f53f2c0edcf3f90ep0,
    0x1.b91358afb58b4p0
  },
  { // Entry 126
    0x1.08fa4b129d365103d8615b0fee830753p0,
    0x1.d0b7907523b24p0
  },
  { // Entry 127
    0x1.113d8baca8608c19974ff89c21cc8d16p0,
    0x1.e85bc83a91d94p0
  },
  { // Entry 128
    0x1.193ea7aad030a976a4198d55053b7cb5p0,
    0x1.0p1
  },
  { // Entry 129
    0x1.11d14e1fcb72edcb28a032e083a6f199p-1,
    0x1.6a09e667f3bccp-1
  },
  { // Entry 130
    0x1.26990e07e25825de21cb52e655390d9ap-1,
    0x1.8e3e170bf282dp-1
  },
  { // Entry 131
    0x1.3a914a1db8cc3855d200ca3202e23d04p-1,
    0x1.b27247aff148ep-1
  },
  { // Entry 132
    0x1.4dc997cbf2ed6806315c6962614b41f2p-1,
    0x1.d6a67853f00efp-1
  },
  { // Entry 133
    0x1.604fdb515451526fcf632e2255d97ef2p-1,
    0x1.fadaa8f7eed50p-1
  },
  { // Entry 134
    0x1.72308447c51665ec8f42f6c1c2f51294p-1,
    0x1.0f876ccdf6cd9p0
  },
  { // Entry 135
    0x1.8376bff406f913a3579a23f2e932df57p-1,
    0x1.21a1851ff630ap0
  },
  { // Entry 136
    0x1.942ca35e8f18f3591aded43add2260dbp-1,
    0x1.33bb9d71f593bp0
  },
  { // Entry 137
    0x1.a45b4ec4852597b4ab8fdd6275a5c1f7p-1,
    0x1.45d5b5c3f4f6cp0
  },
  { // Entry 138
    0x1.b40b0b9a489f168f5ffc2c60ac5bd06ap-1,
    0x1.57efce15f459dp0
  },
  { // Entry 139
    0x1.c34366179d426545cadbc394096e719bp-1,
    0x1.6a09e667f3bccp0
  },
  { // Entry 140
    0x1.1e85f5e7040d03dec59a5f3e3c6be5cfp-1,
    0x1.8p-1
  },
  { // Entry 141
    0x1.34024ac47b6fcebf994c642ef7a882adp-1,
    0x1.a666666666666p-1
  },
  { // Entry 142
    0x1.48a11293d785b50c2a3feb14c3680501p-1,
    0x1.cccccccccccccp-1
  },
  { // Entry 143
    0x1.5c73760b3c362e51806f6a2fcb5402b3p-1,
    0x1.f333333333332p-1
  },
  { // Entry 144
    0x1.6f88b286b22f0a5f70b8ce35df42c80ap-1,
    0x1.0ccccccccccccp0
  },
  { // Entry 145
    0x1.81ee60afb5018aaa0181c98fe3d11e57p-1,
    0x1.1ffffffffffffp0
  },
  { // Entry 146
    0x1.93b0aee21c2c6f1afb29632c77f0434bp-1,
    0x1.3333333333332p0
  },
  { // Entry 147
    0x1.a4da91c611dbcf17d743bad01121e91dp-1,
    0x1.4666666666665p0
  },
  { // Entry 148
    0x1.b575ed0743492c8aacff60d5920dffc4p-1,
    0x1.5999999999998p0
  },
  { // Entry 149
    0x1.c58bb5a60978a15095fe55861acca737p-1,
    0x1.6cccccccccccbp0
  },
  { // Entry 150
    0x1.d5240f0e0e07606e918e49626c8f05e6p-1,
    0x1.7fffffffffffep0
  },
  { // Entry 151
    0x1.62e42fefa39ef35793c7673007e5ed5ep-1,
    0x1.0p0
  },
  { // Entry 152
    0x1.7bdf362e9666e2dc442baf4dc625807dp-1,
    0x1.199999999999ap0
  },
  { // Entry 153
    0x1.93b0aee21c2c8c3240fad7898f606525p-1,
    0x1.3333333333334p0
  },
  { // Entry 154
    0x1.aa73108717b6b240250c5393b356e40ap-1,
    0x1.4cccccccccccep0
  },
  { // Entry 155
    0x1.c03d703735f8e1920627f4336073fe78p-1,
    0x1.6666666666668p0
  },
  { // Entry 156
    0x1.d5240f0e0e0793a1c4c17c959fc23919p-1,
    0x1.8000000000002p0
  },
  { // Entry 157
    0x1.e938cbceb16defcbb921fdd58d5dd567p-1,
    0x1.999999999999cp0
  },
  { // Entry 158
    0x1.fc8b7f138bdeb93fee2e78b4fe3e0831p-1,
    0x1.b333333333336p0
  },
  { // Entry 159
    0x1.0795235c1ea1ca8c0592ee75b4579a8ep0,
    0x1.cccccccccccd0p0
  },
  { // Entry 160
    0x1.1090e20315213b2ddb45c328435c3ca7p0,
    0x1.e66666666666ap0
  },
  { // Entry 161
    0x1.193ea7aad030a976a4198d55053b7cb5p0,
    0x1.0p1
  },
  { // Entry 162
    -0x1.62e42fefa39ef35793c7673007e5ed5ep-1,
    -0x1.0p-1
  },
  { // Entry 163
    -0x1.3217b0fd2b116908897cb1050beea205p-1,
    -0x1.ccccccccccccdp-2
  },
  { // Entry 164
    -0x1.058aefa811451fc7cc1184d75997dc44p-1,
    -0x1.999999999999ap-2
  },
  { // Entry 165
    -0x1.b91f28212ba0379f7a6379b28d1ba9b7p-2,
    -0x1.6666666666667p-2
  },
  { // Entry 166
    -0x1.6d3c324e13f4fe9befad50a0273411c8p-2,
    -0x1.3333333333334p-2
  },
  { // Entry 167
    -0x1.269621134db93cd9140cbcc16037fb86p-2,
    -0x1.0000000000001p-2
  },
  { // Entry 168
    -0x1.c8ff7c79a9a24ac25d81ef2ffc2a24aep-3,
    -0x1.999999999999cp-3
  },
  { // Entry 169
    -0x1.4cd6b9796417b5f11f10de290430b32bp-3,
    -0x1.3333333333336p-3
  },
  { // Entry 170
    -0x1.af8e8210a41636e61283e0400e72f380p-4,
    -0x1.999999999999fp-4
  },
  { // Entry 171
    -0x1.a431d5bcc1942814e94f55ea2e15d5f4p-5,
    -0x1.99999999999a4p-5
  },
  { // Entry 172
    -0x1.400000000000032000000000000a6aaap-54,
    -0x1.4p-54
  },
  { // Entry 173
    0.0,
    0.0
  },
  { // Entry 174
    0x1.8fb063ef2c7e9cdd4f691425091f8212p-5,
    0x1.999999999999ap-5
  },
  { // Entry 175
    0x1.8663f793c46c6f8f982725b4f7100a62p-4,
    0x1.999999999999ap-4
  },
  { // Entry 176
    0x1.1e3b825dd05ec3fb503515bb34638d41p-3,
    0x1.3333333333334p-3
  },
  { // Entry 177
    0x1.7565011e496768e9c982340d63fd99bep-3,
    0x1.999999999999ap-3
  },
  { // Entry 178
    0x1.c8ff7c79a9a21ac25d81ef2ffb9a24aep-3,
    0x1.0p-2
  },
  { // Entry 179
    0x1.0ca937be1b9dbb5e7217a3726f197837p-2,
    0x1.3333333333333p-2
  },
  { // Entry 180
    0x1.334e9e47d07f44b44307069827b79584p-2,
    0x1.6666666666666p-2
  },
  { // Entry 181
    0x1.588c2d913348f380eebceb76c4296aeap-2,
    0x1.9999999999999p-2
  },
  { // Entry 182
    0x1.7c7b282d0d46adc1a6a2b9d712581488p-2,
    0x1.cccccccccccccp-2
  },
  { // Entry 183
    0x1.9f323ecbf984b480be2cbc495a69348bp-2,
    0x1.fffffffffffffp-2
  },
  { // Entry 184
    -0x1.7f7427b73e38f503b99f86230b9f8fa9p1,
    -0x1.e666666666666p-1
  },
  { // Entry 185
    -0x1.26bb1bbb5551382dd4adac5709a61451p1,
    -0x1.cccccccccccccp-1
  },
  { // Entry 186
    -0x1.e5a9a7c3ac414090cf257ef11203a29dp0,
    -0x1.b333333333332p-1
  },
  { // Entry 187
    -0x1.9c041f7ed8d2f6afdf77a5160f5931f4p0,
    -0x1.9999999999998p-1
  },
  { // Entry 188
    -0x1.62e42fefa39eb35793c767300fe5ed5ep0,
    -0x1.7fffffffffffep-1
  },
  { // Entry 189
    -0x1.34378fcbda71c6e50541cb590e10abedp0,
    -0x1.6666666666664p-1
  },
  { // Entry 190
    -0x1.0cc1248b56cc74c07caa7576f1233f0cp0,
    -0x1.4cccccccccccap-1
  },
  { // Entry 191
    -0x1.d5240f0e0e06fa082b27e2fc16cc768ap-1,
    -0x1.3333333333330p-1
  },
  { // Entry 192
    -0x1.98d60031b8212e345617e33819904bcep-1,
    -0x1.1999999999996p-1
  },
  { // Entry 193
    -0x1.62e42fefa39ef35793c7673007e5ed5ep-1,
    -0x1.0p-1
  },
  { // Entry 194
    0x1.1542457337d42e1c6b73c89d866ba171p6,
    0x1.0p100
  },
  { // Entry 195
    0x1.15a3de711cc5494e20ce2f7e3974c4edp6,
    0x1.199999999999ap100
  },
  { // Entry 196
    0x1.15fcf7f3c6f8e1f8e05889b78d1212e9p6,
    0x1.3333333333334p100
  },
  { // Entry 197
    0x1.164eeeaaf5efcc1553be7dcad167cc55p6,
    0x1.4cccccccccccep100
  },
  { // Entry 198
    0x1.169ad1a0c907775fec628588fd1aeadcp6,
    0x1.6666666666668p100
  },
  { // Entry 199
    0x1.16e177b203cdb330ec31f559cfad3551p6,
    0x1.8000000000002p100
  },
  { // Entry 200
    0x1.17238e14da469b55b96c96744e61203ap6,
    0x1.999999999999cp100
  },
  { // Entry 201
    0x1.1761a2765a6960abe5cf92c0959da837p6,
    0x1.b333333333336p100
  },
  { // Entry 202
    0x1.179c2a3292f266ff2833283af2c71bb8p6,
    0x1.cccccccccccd0p100
  },
  { // Entry 203
    0x1.17d387985f833a0d4069f79c1b97242fp6,
    0x1.e66666666666ap100
  },
  { // Entry 204
    0x1.18080dd3171b6c031a9b576be65b6d4cp6,
    0x1.0p101
  },
  { // Entry 205
    0x1.1542457337d42e1c6b73c89d862ba171p7,
    0x1.0p200
  },
  { // Entry 206
    0x1.157311f22a4cbbb54620fc0ddfb31be9p7,
    0x1.199999999999ap200
  },
  { // Entry 207
    0x1.159f9eb37f66880aa5e6292a89842f82p7,
    0x1.3333333333334p200
  },
  { // Entry 208
    0x1.15c89a0f16e1fd18df9923342bb11959p7,
    0x1.4cccccccccccep200
  },
  { // Entry 209
    0x1.15ee8b8a006dd2be2beb2713418c6ab9p7,
    0x1.6666666666668p200
  },
  { // Entry 210
    0x1.1611de929dd0f0a6abd2defbaad7160cp7,
    0x1.8000000000002p200
  },
  { // Entry 211
    0x1.1632e9c4090d64b912702f88ea3260d6p7,
    0x1.999999999999cp200
  },
  { // Entry 212
    0x1.1651f3f4c91ec76428a1adaf0dd1d201p7,
    0x1.b333333333336p200
  },
  { // Entry 213
    0x1.166f37d2e5634a8dc9d3786c3c679778p7,
    0x1.cccccccccccd0p200
  },
  { // Entry 214
    0x1.168ae685cbabb414d5eee01cd0d08b3cp7,
    0x1.e66666666666ap200
  },
  { // Entry 215
    0x1.16a529a32777cd0fc3079004b633875fp7,
    0x1.0p201
  },
  { // Entry 216
    0x1.5a92d6d005c939a38650bac4e7b689cep9,
    0x1.0p1000
  },
  { // Entry 217
    0x1.5a9f09efc2675d09bcfc07a0fe18686cp9,
    0x1.199999999999ap1000
  },
  { // Entry 218
    0x1.5aaa2d2017add01f14ed52e8288cad52p9,
    0x1.3333333333334p1000
  },
  { // Entry 219
    0x1.5ab46bf6fd8cad62a35a116a9117e7c8p9,
    0x1.4cccccccccccep1000
  },
  { // Entry 220
    0x1.5abde855b7efa2cbf66e9262568ebc20p9,
    0x1.6666666666668p1000
  },
  { // Entry 221
    0x1.5ac6bd17df486a461668805c70e166f4p9,
    0x1.8000000000002p1000
  },
  { // Entry 222
    0x1.5aceffe43a17874ab00fd47fc0b839a7p9,
    0x1.999999999999cp1000
  },
  { // Entry 223
    0x1.5ad6c2706a1bdff5759c340949a015f2p9,
    0x1.b333333333336p1000
  },
  { // Entry 224
    0x1.5ade1367f12d00bfdde8a6b895458750p9,
    0x1.cccccccccccd0p1000
  },
  { // Entry 225
    0x1.5ae4ff14aabf1b21a0ef80a4ba5fc441p9,
    0x1.e66666666666ap1000
  },
  { // Entry 226
    0x1.5aeb8fdc01b221605c35ac9eb3b88349p9,
    0x1.0p1001
  },
  { // Entry 227
    0x1.62e42fefa39ef35393c7673007e5dd5ep9,
    0x1.fffffffffffffp1023
  },
  { // Entry 228
    0.0,
    0x1.0p-1074
  },
  { // Entry 229
    -0x1.p-1074,
    -0x1.0p-1074
  },
  { // Entry 230
    0x1.11d14e1fcb72e46bc706b21c5008b9f1p-1,
    0x1.6a09e667f3bcbp-1
  },
  { // Entry 231
    0x1.11d14e1fcb72edcb28a032e083a6f199p-1,
    0x1.6a09e667f3bccp-1
  },
  { // Entry 232
    0x1.11d14e1fcb72f72a8a39b3a4b7193d0ep-1,
    0x1.6a09e667f3bcdp-1
  },
  { // Entry 233
    -0x1.3a5abf07b788ff1b06e03c7b74301bb8p0,
    -0x1.6a09e667f3bcdp-1
  },
  { // Entry 234
    -0x1.3a5abf07b788e3cab7acfcdd8e180c5dp0,
    -0x1.6a09e667f3bccp-1
  },
  { // Entry 235
    -0x1.3a5abf07b788c87a6879bd3faaea06e8p0,
    -0x1.6a09e667f3bcbp-1
  },
  { // Entry 236
    0x1.c34366179d4258048e0ec51c6fefd58cp-1,
    0x1.6a09e667f3bcbp0
  },
  { // Entry 237
    0x1.c34366179d426545cadbc394096e719bp-1,
    0x1.6a09e667f3bccp0
  },
  { // Entry 238
    0x1.c34366179d42728707a8c20ba2953544p-1,
    0x1.6a09e667f3bcdp0
  },
  { // Entry 239
    0x1.9f323ecbf984b480be2cbc495a69348bp-2,
    0x1.fffffffffffffp-2
  },
  { // Entry 240
    0x1.9f323ecbf984bf2b68d766f405221819p-2,
    0x1.0p-1
  },
  { // Entry 241
    0x1.9f323ecbf984d480be2cbc495a3e89e1p-2,
    0x1.0000000000001p-1
  },
  { // Entry 242
    0x1.1e85f5e7040cfaba33513aabf3326da5p-1,
    0x1.7ffffffffffffp-1
  },
  { // Entry 243
    0x1.1e85f5e7040d03dec59a5f3e3c6be5cfp-1,
    0x1.8p-1
  },
  { // Entry 244
    0x1.1e85f5e7040d0d0357e383d0857b9238p-1,
    0x1.8000000000001p-1
  },
  { // Entry 245
    -0x1.62e42fefa39f135793c7673009e5ed5ep0,
    -0x1.8000000000001p-1
  },
  { // Entry 246
    -0x1.62e42fefa39ef35793c7673007e5ed5ep0,
    -0x1.8p-1
  },
  { // Entry 247
    -0x1.62e42fefa39ed35793c7673009e5ed5ep0,
    -0x1.7ffffffffffffp-1
  },
  { // Entry 248
    0x1.d5240f0e0e076d3b5e5b162f39d6b3fap-1,
    0x1.7ffffffffffffp0
  },
  { // Entry 249
    0x1.d5240f0e0e077a082b27e2fc06cc768ap-1,
    0x1.8p0
  },
  { // Entry 250
    0x1.d5240f0e0e0786d4f7f4afc8d3704d94p-1,
    0x1.8000000000001p0
  },
  { // Entry 251
    -0x1.25e4f7b2737fa18486612173c68a6892p5,
    -0x1.fffffffffffffp-1
  },
  { // Entry 252
    0x1.9c041f7ed8d3304979113eafa0de50acp0,
    0x1.fffffffffffffp1
  },
  { // Entry 253
    0x1.9c041f7ed8d336afdf77a516075931f4p0,
    0x1.0p2
  },
  { // Entry 254
    0x1.9c041f7ed8d3437cac4471e2d3d4133bp0,
    0x1.0000000000001p2
  },
  { // Entry 255
    0x1.193ea7aad030a4214ec437ffafd7ee7cp0,
    0x1.fffffffffffffp0
  },
  { // Entry 256
    0x1.193ea7aad030a976a4198d55053b7cb5p0,
    0x1.0p1
  },
  { // Entry 257
    0x1.193ea7aad030b4214ec437ffafad43d2p0,
    0x1.0000000000001p1
  },
  { // Entry 258
    0x1.62e42fefa39eeb5793c7673007d5ed5ep-1,
    0x1.fffffffffffffp-1
  },
  { // Entry 259
    0x1.62e42fefa39ef35793c7673007e5ed5ep-1,
    0x1.0p0
  },
  { // Entry 260
    0x1.62e42fefa39f035793c7673007a5ed5ep-1,
    0x1.0000000000001p0
  },
  { // Entry 261
    0x1.9f323ecbf984b480be2cbc495a69348bp-2,
    0x1.fffffffffffffp-2
  },
  { // Entry 262
    0x1.9f323ecbf984bf2b68d766f405221819p-2,
    0x1.0p-1
  },
  { // Entry 263
    0x1.9f323ecbf984d480be2cbc495a3e89e1p-2,
    0x1.0000000000001p-1
  },
  { // Entry 264
    0x1.c8ff7c79a9a20df590b522632ec31a70p-3,
    0x1.fffffffffffffp-3
  },
  { // Entry 265
    0x1.c8ff7c79a9a21ac25d81ef2ffb9a24aep-3,
    0x1.0p-2
  },
  { // Entry 266
    0x1.c8ff7c79a9a2345bf71b88c9950ac885p-3,
    0x1.0000000000001p-2
  },
  { // Entry 267
    0x1.e27076e2af2e5065c4f1c53c5ba22021p-4,
    0x1.fffffffffffffp-4
  },
  { // Entry 268
    0x1.e27076e2af2e5e9ea87ffe1fe9e155dbp-4,
    0x1.0p-3
  },
  { // Entry 269
    0x1.e27076e2af2e7b106f9c6fe70639d447p-4,
    0x1.0000000000001p-3
  },
  { // Entry 270
    0x1.f0a30c01162a5708bd8807dfa41c78f8p-5,
    0x1.fffffffffffffp-5
  },
  { // Entry 271
    0x1.f0a30c01162a6617cc9716eeb32f131ap-5,
    0x1.0p-4
  },
  { // Entry 272
    0x1.f0a30c01162a8435eab5350cd13f04eep-5,
    0x1.0000000000001p-4
  },
  { // Entry 273
    0x1.f829b0e7832ff54baec8fe6c44c511fdp-6,
    0x1.fffffffffffffp-6
  },
  { // Entry 274
    0x1.f829b0e7833004cf8fc13c7bc8a7ebabp-6,
    0x1.0p-5
  },
  { // Entry 275
    0x1.f829b0e7833023d751b1b89ad0625665p-6,
    0x1.0000000000001p-5
  },
  { // Entry 276
    0x1.fc0a8b0fc03e2d38f1978c3b9c1379b6p-7,
    0x1.fffffffffffffp-7
  },
  { // Entry 277
    0x1.fc0a8b0fc03e3cf9eda74d37abd56df5p-7,
    0x1.0p-6
  },
  { // Entry 278
    0x1.fc0a8b0fc03e5c7be5c6cf2fcb538558p-7,
    0x1.0000000000001p-6
  },
  { // Entry 279
    0x1.fe02a6b106787fe3370f3b19ca72746ep-8,
    0x1.fffffffffffffp-8
  },
  { // Entry 280
    0x1.fe02a6b106788fc37690391dc282d2b3p-8,
    0x1.0p-7
  },
  { // Entry 281
    0x1.fe02a6b10678af83f5923525b2a09b1bp-8,
    0x1.0000000000001p-7
  },
  { // Entry 282
    0x1.ff00aa2b10bbf4b076c559c4c4c719a8p-9,
    0x1.fffffffffffffp-9
  },
  { // Entry 283
    0x1.ff00aa2b10bc04a086b569b4d4b76919p-9,
    0x1.0p-8
  },
  { // Entry 284
    0x1.ff00aa2b10bc2480a6958994f4968af6p-9,
    0x1.0000000000001p-8
  },
  { // Entry 285
    0x1.ff802a9ab10e579274ea53f96c2ac73bp-10,
    0x1.fffffffffffffp-10
  },
  { // Entry 286
    0x1.ff802a9ab10e678a78e854f8ec6ac72bp-10,
    0x1.0p-9
  },
  { // Entry 287
    0x1.ff802a9ab10e877a80e456f7ecea07cap-10,
    0x1.0000000000001p-9
  },
  { // Entry 288
    0x1.ffc00aa8ab10ebc44c055914cd3364b9p-11,
    0x1.fffffffffffffp-11
  },
  { // Entry 289
    0x1.ffc00aa8ab10fbc04d051924c9347471p-11,
    0x1.0p-10
  },
  { // Entry 290
    0x1.ffc00aa8ab111bb84f049944c1363411p-11,
    0x1.0000000000001p-10
  },
  { // Entry 291
    0x1.fff8002aa9aaa11166638b10aec3e0a4p-14,
    0x1.fffffffffffffp-14
  },
  { // Entry 292
    0x1.fff8002aa9aab110e6678af0afc3daa4p-14,
    0x1.0p-13
  },
  { // Entry 293
    0x1.fff8002aa9aad10fe66f8ab0b1c3c2a5p-14,
    0x1.0000000000001p-13
  },
  { // Entry 294
    -0x1.62e42fefa39f135793c7673008e5ed5ep-1,
    -0x1.0000000000001p-1
  },
  { // Entry 295
    -0x1.62e42fefa39ef35793c7673007e5ed5ep-1,
    -0x1.0p-1
  },
  { // Entry 296
    -0x1.62e42fefa39ee35793c767300825ed5ep-1,
    -0x1.fffffffffffffp-2
  },
  { // Entry 297
    -0x1.269621134db93cd9140cbcc16037fb86p-2,
    -0x1.0000000000001p-2
  },
  { // Entry 298
    -0x1.269621134db92783beb7676c0aa9c2a3p-2,
    -0x1.0p-2
  },
  { // Entry 299
    -0x1.269621134db91cd9140cbcc1600d50dbp-2,
    -0x1.fffffffffffffp-3
  },
  { // Entry 300
    -0x1.1178e8227e47d02c5d4668ebc04628aep-3,
    -0x1.0000000000001p-3
  },
  { // Entry 301
    -0x1.1178e8227e47bde338b41fc72de81e3bp-3,
    -0x1.0p-3
  },
  { // Entry 302
    -0x1.1178e8227e47b4bea66afb34e4c8c56ap-3,
    -0x1.fffffffffffffp-4
  },
  { // Entry 303
    -0x1.08598b59e3a0799b50ead061448cec6cp-4,
    -0x1.0000000000001p-4
  },
  { // Entry 304
    -0x1.08598b59e3a0688a3fd9bf503372c12fp-4,
    -0x1.0p-4
  },
  { // Entry 305
    -0x1.08598b59e3a06001b75136c7aaec7f32p-4,
    -0x1.fffffffffffffp-5
  },
  { // Entry 306
    -0x1.0415d89e7444578594cf9f5e0caf2971p-5,
    -0x1.0000000000001p-5
  },
  { // Entry 307
    -0x1.0415d89e7444470173c75d4d8889de0ep-5,
    -0x1.0p-5
  },
  { // Entry 308
    -0x1.0415d89e74443ebf63433c45467a6ab5p-5,
    -0x1.fffffffffffffp-6
  },
  { // Entry 309
    -0x1.02056589358484e027b146bdd7feaee1p-6,
    -0x1.0000000000001p-6
  },
  { // Entry 310
    -0x1.020565893584749f23a105b9c7bb9a6fp-6,
    -0x1.0p-6
  },
  { // Entry 311
    -0x1.0205658935846c7ea198e537bf9b9c7fp-6,
    -0x1.fffffffffffffp-7
  },
  { // Entry 312
    -0x1.010157588de722ad0cdb84fde21218d8p-7,
    -0x1.0000000000001p-7
  },
  { // Entry 313
    -0x1.010157588de7128ccc5a82f9da00f48bp-7,
    -0x1.0p-7
  },
  { // Entry 314
    -0x1.010157588de70a7cac1a01f7d5f9256ep-7,
    -0x1.fffffffffffffp-8
  },
  { // Entry 315
    -0x1.0080559588b367f5a8f34d9dadc40b3ap-8,
    -0x1.0000000000001p-8
  },
  { // Entry 316
    -0x1.0080559588b357e598e33d8d9db37a29p-8,
    -0x1.0p-8
  },
  { // Entry 317
    -0x1.0080559588b34fdd90db358595ab9261p-8,
    -0x1.fffffffffffffp-9
  },
  { // Entry 318
    -0x1.0040155d5889ee786b20efc1400f5ea4p-9,
    -0x1.0000000000001p-9
  },
  { // Entry 319
    -0x1.0040155d5889de70671eeec0bfcefe53p-9,
    -0x1.0p-9
  },
  { // Entry 320
    -0x1.0040155d5889d66c651dee407faefe5bp-9,
    -0x1.fffffffffffffp-10
  },
  { // Entry 321
    -0x1.002005565588a3397dd822048abe2755p-10,
    -0x1.0000000000001p-10
  },
  { // Entry 322
    -0x1.00200556558893357cd7e1f486bd0705p-10,
    -0x1.0p-10
  },
  { // Entry 323
    -0x1.0020055655888b337c57c1ec84bc8ee9p-10,
    -0x1.fffffffffffffp-11
  },
  { // Entry 324
    -0x1.0004001555d568891de2704b038ca596p-13,
    -0x1.0000000000001p-13
  },
  { // Entry 325
    -0x1.0004001555d558889dde702b028c9996p-13,
    -0x1.0p-13
  },
  { // Entry 326
    -0x1.0004001555d550885ddc701b020c9696p-13,
    -0x1.fffffffffffffp-14
  },
  { // Entry 327
    0x1.73242d45267376d3a2a0a820c4902335p-1,
    0x1.1082b577d34eap0
  },
  { // Entry 328
    0x1.73242d4526738653ad4a41d5fe7ea6c1p-1,
    0x1.1082b577d34ebp0
  },
  { // Entry 329
    0x1.73242d45267395d3b7f3db8b37f509a8p-1,
    0x1.1082b577d34ecp0
  },
  { // Entry 330
    0x1.73242d452673a553c29d754070f34beap-1,
    0x1.1082b577d34edp0
  },
  { // Entry 331
    0x1.73242d452673b4d3cd470ef5a9796d86p-1,
    0x1.1082b577d34eep0
  },
  { // Entry 332
    0x1.73242d452673c453d7f0a8aae1876e7ep-1,
    0x1.1082b577d34efp0
  },
  { // Entry 333
    0x1.73242d452673d3d3e29a4260191d4ecfp-1,
    0x1.1082b577d34f0p0
  },
  { // Entry 334
    -0x1.00000000000018ade0e8cb684e083468p-4,
    -0x1.f0540438fd5c6p-5
  },
  { // Entry 335
    -0x1.0000000000001029cb3d0ccdd73ea85ap-4,
    -0x1.f0540438fd5c5p-5
  },
  { // Entry 336
    -0x1.00000000000007a5b5914e336079a4a5p-4,
    -0x1.f0540438fd5c4p-5
  },
  { // Entry 337
    -0x1.fffffffffffffe433fcb1f31d3725290p-5,
    -0x1.f0540438fd5c3p-5
  },
  { // Entry 338
    -0x1.ffffffffffffed3b1473a1fce5fa6c86p-5,
    -0x1.f0540438fd5c2p-5
  },
  { // Entry 339
    -0x1.ffffffffffffdc32e91c24c7f88b972cp-5,
    -0x1.f0540438fd5c1p-5
  },
  { // Entry 340
    -0x1.ffffffffffffcb2abdc4a7930b25d282p-5,
    -0x1.f0540438fd5c0p-5
  },
  { // Entry 341
    HUGE_VAL,
    HUGE_VAL
  },
  { // Entry 342
    0x1.62e42fefa39ef35393c7673007e5dd5ep9,
    0x1.fffffffffffffp1023
  },
  { // Entry 343
    0x1.62e42fefa39ef34f93c7673007e5ad5ep9,
    0x1.ffffffffffffep1023
  },
  { // Entry 344
    0x1.6bcbed09f00aece2ea800b6af0f24a0bp0,
    0x1.921fb54442d18p1
  },
  { // Entry 345
    0x1.e3703db0ab119bed3e763f434dd7c4fbp-1,
    0x1.921fb54442d18p0
  },
  { // Entry 346
    0x1.62e42fefa39f035793c7673007a5ed5ep-1,
    0x1.0000000000001p0
  },
  { // Entry 347
    0x1.62e42fefa39ef35793c7673007e5ed5ep-1,
    0x1.0p0
  },
  { // Entry 348
    -HUGE_VAL,
    -0x1.0p0
  },
  { // Entry 349
    0x1.62e42fefa39eeb5793c7673007d5ed5ep-1,
    0x1.fffffffffffffp-1
  },
  { // Entry 350
    -0x1.25e4f7b2737fa18486612173c68a6892p5,
    -0x1.fffffffffffffp-1
  },
  { // Entry 351
    0x1.28c6c3a79fe295c7ca64ed982642adcfp-1,
    0x1.921fb54442d18p-1
  },
  { // Entry 352
    -0x1.89f9ff0c761bc5454f17ec539e887d37p0,
    -0x1.921fb54442d18p-1
  },
  { // Entry 353
    0x1.0000000000000fffffffffffffffffffp-1022,
    0x1.0000000000001p-1022
  },
  { // Entry 354
    -0x1.00000000000010p-1022,
    -0x1.0000000000001p-1022
  },
  { // Entry 355
    0x1.ffffffffffffffffffffffffffffffffp-1023,
    0x1.0p-1022
  },
  { // Entry 356
    -0x1.p-1022,
    -0x1.0p-1022
  },
  { // Entry 357
    0x1.ffffffffffffdfffffffffffffffffffp-1023,
    0x1.ffffffffffffep-1023
  },
  { // Entry 358
    -0x1.ffffffffffffe0p-1023,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 359
    0x1.ffffffffffffbfffffffffffffffffffp-1023,
    0x1.ffffffffffffcp-1023
  },
  { // Entry 360
    -0x1.ffffffffffffc0p-1023,
    -0x1.ffffffffffffcp-1023
  },
  { // Entry 361
    0x1.ffffffffffffffffffffffffffffffffp-1074,
    0x1.0p-1073
  },
  { // Entry 362
    -0x1.p-1073,
    -0x1.0p-1073
  },
  { // Entry 363
    0.0,
    0x1.0p-1074
  },
  { // Entry 364
    -0x1.p-1074,
    -0x1.0p-1074
  },
  { // Entry 365
    0.0,
    0.0
  },
  { // Entry 366
    -0.0,
    -0.0
  }
};
