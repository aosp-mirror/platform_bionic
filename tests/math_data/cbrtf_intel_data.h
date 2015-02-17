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

static data_1_1_t<float, float> g_cbrtf_intel_data[] = {
  { // Entry 0
    -0x1.00007fffc000355520003aaa663936aap-42,
    -0x1.000180p-126
  },
  { // Entry 1
    0x1.00007fffc000355520003aaa663936aap-42,
    0x1.000180p-126
  },
  { // Entry 2
    -0x1.0000ffff0001aaa7555caa998e62715fp-2,
    -0x1.0003p-6
  },
  { // Entry 3
    0x1.0000ffff0001aaa7555caa998e62715fp-2,
    0x1.0003p-6
  },
  { // Entry 4
    -0x1.007c2715b6911795158a4d4cc45b8d59p2,
    -0x1.01752ap6
  },
  { // Entry 5
    0x1.007c2715b6911795158a4d4cc45b8d59p2,
    0x1.01752ap6
  },
  { // Entry 6
    -0x1.0b9ccd06011fce363b7a50a0a3b8e26cp1,
    -0x1.247112p3
  },
  { // Entry 7
    0x1.0b9ccd06011fce363b7a50a0a3b8e26cp1,
    0x1.247112p3
  },
  { // Entry 8
    -0x1.aa863b0a38d00e125cbd173f60ddbb68p1,
    -0x1.27fff8p5
  },
  { // Entry 9
    0x1.aa863b0a38d00e125cbd173f60ddbb68p1,
    0x1.27fff8p5
  },
  { // Entry 10
    -0x1.b0ff4b0cf530be2de450549f985f42afp-3,
    -0x1.35ae80p-7
  },
  { // Entry 11
    0x1.b0ff4b0cf530be2de450549f985f42afp-3,
    0x1.35ae80p-7
  },
  { // Entry 12
    -0x1.d30d4d0027a339e3355b6acc1d16b858p-11,
    -0x1.84a5b6p-31
  },
  { // Entry 13
    0x1.d30d4d0027a339e3355b6acc1d16b858p-11,
    0x1.84a5b6p-31
  },
  { // Entry 14
    -0x1.7bc7460098a24fb469f60dedd0c113cep-4,
    -0x1.a1e880p-11
  },
  { // Entry 15
    0x1.7bc7460098a24fb469f60dedd0c113cep-4,
    0x1.a1e880p-11
  },
  { // Entry 16
    -0x1.e18718fd2b5e5307048d4417449467b6p-11,
    -0x1.a9ea80p-31
  },
  { // Entry 17
    0x1.e18718fd2b5e5307048d4417449467b6p-11,
    0x1.a9ea80p-31
  },
  { // Entry 18
    -0x1.e3082be3326da6c23ae338a3f121dd1fp-11,
    -0x1.adeb80p-31
  },
  { // Entry 19
    0x1.e3082be3326da6c23ae338a3f121dd1fp-11,
    0x1.adeb80p-31
  },
  { // Entry 20
    0x1.965ff706d5d0ceecd979f25d2bd0f2dcp-14,
    0x1.000018p-40
  },
  { // Entry 21
    -0x1.965ff706d5d0ceecd979f25d2bd0f2dcp-14,
    -0x1.000018p-40
  },
  { // Entry 22
    0x1.96602e0e72a7fdf15e33145af6f263d8p0,
    0x1.000080p2
  },
  { // Entry 23
    -0x1.96602e0e72a7fdf15e33145af6f263d8p0,
    -0x1.000080p2
  },
  { // Entry 24
    0x1.96603efd1611ff8ea190cefd028f4506p-14,
    0x1.0000a0p-40
  },
  { // Entry 25
    -0x1.96603efd1611ff8ea190cefd028f4506p-14,
    -0x1.0000a0p-40
  },
  { // Entry 26
    0x1.00003ffff00006aaa755572aa998e434p-2,
    0x1.0000c0p-6
  },
  { // Entry 27
    -0x1.00003ffff00006aaa755572aa998e434p-2,
    -0x1.0000c0p-6
  },
  { // Entry 28
    0x1.966060da58aa5d0c57334a9a680c3e14p-14,
    0x1.0000e0p-40
  },
  { // Entry 29
    -0x1.966060da58aa5d0c57334a9a680c3e14p-14,
    -0x1.0000e0p-40
  },
  { // Entry 30
    0x1.428ad0dd9e52d1023b2faca281049d23p-42,
    0x1.000180p-125
  },
  { // Entry 31
    -0x1.428ad0dd9e52d1023b2faca281049d23p-42,
    -0x1.000180p-125
  },
  { // Entry 32
    0x1.00007fffc000355520003aaa663936aap-42,
    0x1.000180p-126
  },
  { // Entry 33
    -0x1.00007fffc000355520003aaa663936aap-42,
    -0x1.000180p-126
  },
  { // Entry 34
    0x1.966118fd0fff4af1016786c52473d739p0,
    0x1.00023cp2
  },
  { // Entry 35
    -0x1.966118fd0fff4af1016786c52473d739p0,
    -0x1.00023cp2
  },
  { // Entry 36
    0x1.0000ffff0001aaa7555caa998e62715fp-2,
    0x1.0003p-6
  },
  { // Entry 37
    -0x1.0000ffff0001aaa7555caa998e62715fp-2,
    -0x1.0003p-6
  },
  { // Entry 38
    0x1.9661c46c3f2accbc3b2879ecc64c5563p-42,
    0x1.000380p-124
  },
  { // Entry 39
    -0x1.9661c46c3f2accbc3b2879ecc64c5563p-42,
    -0x1.000380p-124
  },
  { // Entry 40
    0x1.000954fe3e2e3b3fc203c0f1122ef525p-2,
    0x1.001cp-6
  },
  { // Entry 41
    -0x1.000954fe3e2e3b3fc203c0f1122ef525p-2,
    -0x1.001cp-6
  },
  { // Entry 42
    0x1.000fff001aa755ca9990d15f4978c319p-1,
    0x1.0030p-3
  },
  { // Entry 43
    -0x1.000fff001aa755ca9990d15f4978c319p-1,
    -0x1.0030p-3
  },
  { // Entry 44
    0x1.000fff001aa755ca9990d15f4978c319p-2,
    0x1.0030p-6
  },
  { // Entry 45
    -0x1.000fff001aa755ca9990d15f4978c319p-2,
    -0x1.0030p-6
  },
  { // Entry 46
    0x1.96e94efe3bb4f031f6fd29764c187ba9p-14,
    0x1.0104p-40
  },
  { // Entry 47
    -0x1.96e94efe3bb4f031f6fd29764c187ba9p-14,
    -0x1.0104p-40
  },
  { // Entry 48
    0x1.4354f47046b122c8269cefa33f6945e0p-12,
    0x1.01e4p-35
  },
  { // Entry 49
    -0x1.4354f47046b122c8269cefa33f6945e0p-12,
    -0x1.01e4p-35
  },
  { // Entry 50
    0x1.435ce4ffe5df0ed6186f37c8cd55dfcdp-14,
    0x1.01f7p-41
  },
  { // Entry 51
    -0x1.435ce4ffe5df0ed6186f37c8cd55dfcdp-14,
    -0x1.01f7p-41
  },
  { // Entry 52
    0x1.43da6b00005c39b0e6c7c4cec8063f45p0,
    0x1.0323dep1
  },
  { // Entry 53
    -0x1.43da6b00005c39b0e6c7c4cec8063f45p0,
    -0x1.0323dep1
  },
  { // Entry 54
    0x1.998ca5fd9079c0acf8d0c470551815c4p-14,
    0x1.060cp-40
  },
  { // Entry 55
    -0x1.998ca5fd9079c0acf8d0c470551815c4p-14,
    -0x1.060cp-40
  },
  { // Entry 56
    0x1.998ebb71eaf54f2b63663678acd9c1b0p-14,
    0x1.0610p-40
  },
  { // Entry 57
    -0x1.998ebb71eaf54f2b63663678acd9c1b0p-14,
    -0x1.0610p-40
  },
  { // Entry 58
    0x1.454f770000009a77b66758620294365fp-1,
    0x1.06a76ap-2
  },
  { // Entry 59
    -0x1.454f770000009a77b66758620294365fp-1,
    -0x1.06a76ap-2
  },
  { // Entry 60
    0x1.9ap-4,
    0x1.06e9aap-10
  },
  { // Entry 61
    -0x1.9ap-4,
    -0x1.06e9aap-10
  },
  { // Entry 62
    0x1.9a2dc6da8c05f5e76488d08a9d68aab3p0,
    0x1.0741c4p2
  },
  { // Entry 63
    -0x1.9a2dc6da8c05f5e76488d08a9d68aab3p0,
    -0x1.0741c4p2
  },
  { // Entry 64
    0x1.9b67b1a6ed6fd1c14e0973d1bbdffc44p-14,
    0x1.09a0p-40
  },
  { // Entry 65
    -0x1.9b67b1a6ed6fd1c14e0973d1bbdffc44p-14,
    -0x1.09a0p-40
  },
  { // Entry 66
    0x1.46ffc700366b1255b7648b4899b98d5ap-14,
    0x1.0ac4p-41
  },
  { // Entry 67
    -0x1.46ffc700366b1255b7648b4899b98d5ap-14,
    -0x1.0ac4p-41
  },
  { // Entry 68
    0x1.9c1f2ad69d37542ebe215ede37325f7dp-14,
    0x1.0b04p-40
  },
  { // Entry 69
    -0x1.9c1f2ad69d37542ebe215ede37325f7dp-14,
    -0x1.0b04p-40
  },
  { // Entry 70
    0x1.9c8148004106b29bacdfd646ccb890bbp-2,
    0x1.0bc2e2p-4
  },
  { // Entry 71
    -0x1.9c8148004106b29bacdfd646ccb890bbp-2,
    -0x1.0bc2e2p-4
  },
  { // Entry 72
    0x1.478a7bb572cc90cef3596fd852b532cdp0,
    0x1.0c1808p1
  },
  { // Entry 73
    -0x1.478a7bb572cc90cef3596fd852b532cdp0,
    -0x1.0c1808p1
  },
  { // Entry 74
    0x1.9d2397032cef4ebe8a2c5c889a8876a1p-1,
    0x1.0cff70p-1
  },
  { // Entry 75
    -0x1.9d2397032cef4ebe8a2c5c889a8876a1p-1,
    -0x1.0cff70p-1
  },
  { // Entry 76
    0x1.9e20b8d6240f02f0eaf930fdf441789ep0,
    0x1.0eef12p2
  },
  { // Entry 77
    -0x1.9e20b8d6240f02f0eaf930fdf441789ep0,
    -0x1.0eef12p2
  },
  { // Entry 78
    0x1.9ead3105d958ea7ec76503944303ef7dp2,
    0x1.100322p8
  },
  { // Entry 79
    -0x1.9ead3105d958ea7ec76503944303ef7dp2,
    -0x1.100322p8
  },
  { // Entry 80
    0x1.9f2faffdef6461c09455448fa928b9c3p-1,
    0x1.110440p-1
  },
  { // Entry 81
    -0x1.9f2faffdef6461c09455448fa928b9c3p-1,
    -0x1.110440p-1
  },
  { // Entry 82
    0x1.a0dfb8137c3e91c3f3e838645b454f05p-14,
    0x1.145cp-40
  },
  { // Entry 83
    -0x1.a0dfb8137c3e91c3f3e838645b454f05p-14,
    -0x1.145cp-40
  },
  { // Entry 84
    0x1.a2311b035270313c9ca7f92e42644378p-1,
    0x1.16fd1ep-1
  },
  { // Entry 85
    -0x1.a2311b035270313c9ca7f92e42644378p-1,
    -0x1.16fd1ep-1
  },
  { // Entry 86
    0x1.a25e7efd01b4c7f69f01fe96f643494fp-14,
    0x1.1758p-40
  },
  { // Entry 87
    -0x1.a25e7efd01b4c7f69f01fe96f643494fp-14,
    -0x1.1758p-40
  },
  { // Entry 88
    0x1.a28a68ffbdfb7eb6667d2cdd341cd234p-14,
    0x1.17b0p-40
  },
  { // Entry 89
    -0x1.a28a68ffbdfb7eb6667d2cdd341cd234p-14,
    -0x1.17b0p-40
  },
  { // Entry 90
    0x1.a2b24d04a7585fdde607a7f42d370876p-42,
    0x1.18p-124
  },
  { // Entry 91
    -0x1.a2b24d04a7585fdde607a7f42d370876p-42,
    -0x1.18p-124
  },
  { // Entry 92
    0x1.a480db076345638b28d0b5d97e216402p-14,
    0x1.1ba4p-40
  },
  { // Entry 93
    -0x1.a480db076345638b28d0b5d97e216402p-14,
    -0x1.1ba4p-40
  },
  { // Entry 94
    0x1.4e11970614cfe9395e7c524b2922ed62p-14,
    0x1.1c71c8p-41
  },
  { // Entry 95
    -0x1.4e11970614cfe9395e7c524b2922ed62p-14,
    -0x1.1c71c8p-41
  },
  { // Entry 96
    0x1.a524ac009e9723d4018859285260a132p-14,
    0x1.1cf0p-40
  },
  { // Entry 97
    -0x1.a524ac009e9723d4018859285260a132p-14,
    -0x1.1cf0p-40
  },
  { // Entry 98
    0x1.a5b64e6d1ad233a593368ac2163d6b7dp-14,
    0x1.1e18p-40
  },
  { // Entry 99
    -0x1.a5b64e6d1ad233a593368ac2163d6b7dp-14,
    -0x1.1e18p-40
  },
  { // Entry 100
    0x1.a6a58c5b6df5c725e3ec4ab187301586p-14,
    0x1.1ffffep-40
  },
  { // Entry 101
    -0x1.a6a58c5b6df5c725e3ec4ab187301586p-14,
    -0x1.1ffffep-40
  },
  { // Entry 102
    0x1.acp-4,
    0x1.2b1530p-10
  },
  { // Entry 103
    -0x1.acp-4,
    -0x1.2b1530p-10
  },
  { // Entry 104
    0x1.5500563b025a26e6cde7846c60dd1a63p0,
    0x1.2e85dcp1
  },
  { // Entry 105
    -0x1.5500563b025a26e6cde7846c60dd1a63p0,
    -0x1.2e85dcp1
  },
  { // Entry 106
    0x1.57bfb300c89d535ddc61f9b3d999dc1ep0,
    0x1.35e4f8p1
  },
  { // Entry 107
    -0x1.57bfb300c89d535ddc61f9b3d999dc1ep0,
    -0x1.35e4f8p1
  },
  { // Entry 108
    0x1.b2e9430bd21aae846599fdf68e51e1e6p-1,
    0x1.39ce70p-1
  },
  { // Entry 109
    -0x1.b2e9430bd21aae846599fdf68e51e1e6p-1,
    -0x1.39ce70p-1
  },
  { // Entry 110
    0x1.b40a010c84eac96d410f8ba21fda3f22p0,
    0x1.3c4114p2
  },
  { // Entry 111
    -0x1.b40a010c84eac96d410f8ba21fda3f22p0,
    -0x1.3c4114p2
  },
  { // Entry 112
    0x1.c5d8590ca543fcb1c0bfb46e90e419bep0,
    0x1.6499f6p2
  },
  { // Entry 113
    -0x1.c5d8590ca543fcb1c0bfb46e90e419bep0,
    -0x1.6499f6p2
  },
  { // Entry 114
    0x1.c6791f0cb47f19f16d083c211d10420bp0,
    0x1.661576p2
  },
  { // Entry 115
    -0x1.c6791f0cb47f19f16d083c211d10420bp0,
    -0x1.661576p2
  },
  { // Entry 116
    0x1.c71f7f0cd5c51d6bb49a93023a9a652dp-1,
    0x1.679f4ap-1
  },
  { // Entry 117
    -0x1.c71f7f0cd5c51d6bb49a93023a9a652dp-1,
    -0x1.679f4ap-1
  },
  { // Entry 118
    0x1.c728c50c9eca36360c22256e33e42dc2p0,
    0x1.67b546p2
  },
  { // Entry 119
    -0x1.c728c50c9eca36360c22256e33e42dc2p0,
    -0x1.67b546p2
  },
  { // Entry 120
    0x1.1fb2b50224020b9a31b5e09844e1348dp-1,
    0x1.6b5ad6p-3
  },
  { // Entry 121
    -0x1.1fb2b50224020b9a31b5e09844e1348dp-1,
    -0x1.6b5ad6p-3
  },
  { // Entry 122
    0x1.6ab560ffff8428b4d453bae089599bbap-1,
    0x1.6c0d4ap-2
  },
  { // Entry 123
    -0x1.6ab560ffff8428b4d453bae089599bbap-1,
    -0x1.6c0d4ap-2
  },
  { // Entry 124
    0x1.cdee0281e74e9710c68918089b9a62f2p-42,
    0x1.77fffep-124
  },
  { // Entry 125
    -0x1.cdee0281e74e9710c68918089b9a62f2p-42,
    -0x1.77fffep-124
  },
  { // Entry 126
    0x1.d11fb6f9f76e1fd19de47cbe7090dffbp-43,
    0x1.7fda9cp-127
  },
  { // Entry 127
    -0x1.d11fb6f9f76e1fd19de47cbe7090dffbp-43,
    -0x1.7fda9cp-127
  },
  { // Entry 128
    0x1.7ad659001595cf1b272d5984d8001451p-2,
    0x1.9ecf20p-5
  },
  { // Entry 129
    -0x1.7ad659001595cf1b272d5984d8001451p-2,
    -0x1.9ecf20p-5
  },
  { // Entry 130
    0x1.7ad659001595cf1b272d5984d8001451p-41,
    0x1.9ecf20p-122
  },
  { // Entry 131
    -0x1.7ad659001595cf1b272d5984d8001451p-41,
    -0x1.9ecf20p-122
  },
  { // Entry 132
    0x1.df1025ff8b1feb85afffe8b0cce5c6c1p-1,
    0x1.a368d0p-1
  },
  { // Entry 133
    -0x1.df1025ff8b1feb85afffe8b0cce5c6c1p-1,
    -0x1.a368d0p-1
  },
  { // Entry 134
    0x1.dfa5d2ff7b31fa9b073558a718dd7beap-3,
    0x1.a4f268p-7
  },
  { // Entry 135
    -0x1.dfa5d2ff7b31fa9b073558a718dd7beap-3,
    -0x1.a4f268p-7
  },
  { // Entry 136
    0x1.e14d56805a724bfafe4ad3bdbf27694fp0,
    0x1.a9514ep2
  },
  { // Entry 137
    -0x1.e14d56805a724bfafe4ad3bdbf27694fp0,
    -0x1.a9514ep2
  },
  { // Entry 138
    0x1.e2df53063263dd47f4e3b4be5d8acd20p-2,
    0x1.ad7e78p-4
  },
  { // Entry 139
    -0x1.e2df53063263dd47f4e3b4be5d8acd20p-2,
    -0x1.ad7e78p-4
  },
  { // Entry 140
    0x1.eb5752ff94df0f59dec8df36f4cc1b31p-41,
    0x1.c47d8cp-121
  },
  { // Entry 141
    -0x1.eb5752ff94df0f59dec8df36f4cc1b31p-41,
    -0x1.c47d8cp-121
  },
  { // Entry 142
    0x1.ebe5df03d9d653e93e8fd07e1190dca7p-2,
    0x1.c607d2p-4
  },
  { // Entry 143
    -0x1.ebe5df03d9d653e93e8fd07e1190dca7p-2,
    -0x1.c607d2p-4
  },
  { // Entry 144
    0x1.ecp-3,
    0x1.c65030p-7
  },
  { // Entry 145
    -0x1.ecp-3,
    -0x1.c65030p-7
  },
  { // Entry 146
    0x1.ed53c2fed7938cfe7d57506934d72ep-43,
    0x1.c9fff0p-127
  },
  { // Entry 147
    -0x1.ed53c2fed7938cfe7d57506934d72ep-43,
    -0x1.c9fff0p-127
  },
  { // Entry 148
    0x1.87d30f000003670b89545d5765e0e462p-2,
    0x1.caf302p-5
  },
  { // Entry 149
    -0x1.87d30f000003670b89545d5765e0e462p-2,
    -0x1.caf302p-5
  },
  { // Entry 150
    0x1.37964effff778957b16b4208af77a9b9p-1,
    0x1.cd97a8p-3
  },
  { // Entry 151
    -0x1.37964effff778957b16b4208af77a9b9p-1,
    -0x1.cd97a8p-3
  },
  { // Entry 152
    0x1.ef78e20aee43030728af06495ee66d87p-14,
    0x1.cffffep-40
  },
  { // Entry 153
    -0x1.ef78e20aee43030728af06495ee66d87p-14,
    -0x1.cffffep-40
  },
  { // Entry 154
    0x1.f060ceff8db83f2f87077b938a0de67fp-1,
    0x1.d28cc4p-1
  },
  { // Entry 155
    -0x1.f060ceff8db83f2f87077b938a0de67fp-1,
    -0x1.d28cc4p-1
  },
  { // Entry 156
    0x1.f0d282ff86ba2828aff1098a7563fc5bp-1,
    0x1.d3cdaap-1
  },
  { // Entry 157
    -0x1.f0d282ff86ba2828aff1098a7563fc5bp-1,
    -0x1.d3cdaap-1
  },
  { // Entry 158
    0x1.f1a89236719cd91fce63ecac19a53d56p0,
    0x1.d62b5ap2
  },
  { // Entry 159
    -0x1.f1a89236719cd91fce63ecac19a53d56p0,
    -0x1.d62b5ap2
  },
  { // Entry 160
    0x1.f1c2bfc0386315c501e39e3ac19c4279p-1,
    0x1.d67590p-1
  },
  { // Entry 161
    -0x1.f1c2bfc0386315c501e39e3ac19c4279p-1,
    -0x1.d67590p-1
  },
  { // Entry 162
    0x1.f2p-4,
    0x1.d72352p-10
  },
  { // Entry 163
    -0x1.f2p-4,
    -0x1.d72352p-10
  },
  { // Entry 164
    0x1.f21c203557858a7f1c78b0cb14718fa0p0,
    0x1.d7732ap2
  },
  { // Entry 165
    -0x1.f21c203557858a7f1c78b0cb14718fa0p0,
    -0x1.d7732ap2
  },
  { // Entry 166
    0x1.f21cf4389d599ec5022b876e9dab8142p-8,
    0x1.d77584p-22
  },
  { // Entry 167
    -0x1.f21cf4389d599ec5022b876e9dab8142p-8,
    -0x1.d77584p-22
  },
  { // Entry 168
    0x1.f250a10528fd3cf7552e65df7a4956f7p2,
    0x1.d8084ep8
  },
  { // Entry 169
    -0x1.f250a10528fd3cf7552e65df7a4956f7p2,
    -0x1.d8084ep8
  },
  { // Entry 170
    0x1.8be60f000087a7c285d12416d0b0cb96p-5,
    0x1.d96a4ap-14
  },
  { // Entry 171
    -0x1.8be60f000087a7c285d12416d0b0cb96p-5,
    -0x1.d96a4ap-14
  },
  { // Entry 172
    0x1.3a7bb8ffff77ce75cfe33eca149a6a50p-1,
    0x1.da956cp-3
  },
  { // Entry 173
    -0x1.3a7bb8ffff77ce75cfe33eca149a6a50p-1,
    -0x1.da956cp-3
  },
  { // Entry 174
    0x1.f5477afcf320bd46a28a22db0896968ep0,
    0x1.e081a4p2
  },
  { // Entry 175
    -0x1.f5477afcf320bd46a28a22db0896968ep0,
    -0x1.e081a4p2
  },
  { // Entry 176
    0x1.f63e0aff87cf1648687fec920f40ac1ap-3,
    0x1.e3480ap-7
  },
  { // Entry 177
    -0x1.f63e0aff87cf1648687fec920f40ac1ap-3,
    -0x1.e3480ap-7
  },
  { // Entry 178
    0x1.f64d78ff83bdecdd6abbbb5c278476c2p2,
    0x1.e37496p8
  },
  { // Entry 179
    -0x1.f64d78ff83bdecdd6abbbb5c278476c2p2,
    -0x1.e37496p8
  },
  { // Entry 180
    0x1.f67dbc1e6250897b0e90aa1d01d222c7p-14,
    0x1.e3fffep-40
  },
  { // Entry 181
    -0x1.f67dbc1e6250897b0e90aa1d01d222c7p-14,
    -0x1.e3fffep-40
  },
  { // Entry 182
    0x1.f696a6ff9280045087896842b6ed5614p0,
    0x1.e44802p2
  },
  { // Entry 183
    -0x1.f696a6ff9280045087896842b6ed5614p0,
    -0x1.e44802p2
  },
  { // Entry 184
    0x1.f6b6e4ff4a938651705569091603de15p0,
    0x1.e4a53cp2
  },
  { // Entry 185
    -0x1.f6b6e4ff4a938651705569091603de15p0,
    -0x1.e4a53cp2
  },
  { // Entry 186
    0x1.3de6d50029c5bbf888e67bae14879833p-1,
    0x1.ea3a80p-3
  },
  { // Entry 187
    -0x1.3de6d50029c5bbf888e67bae14879833p-1,
    -0x1.ea3a80p-3
  },
  { // Entry 188
    0x1.fc5fc5d0ad07f38ec9b17b1f7d4e1717p0,
    0x1.f532fep2
  },
  { // Entry 189
    -0x1.fc5fc5d0ad07f38ec9b17b1f7d4e1717p0,
    -0x1.f532fep2
  },
  { // Entry 190
    0x1.fc7d65d0f6b174adf860b9de60ce7b24p-1,
    0x1.f58aa2p-1
  },
  { // Entry 191
    -0x1.fc7d65d0f6b174adf860b9de60ce7b24p-1,
    -0x1.f58aa2p-1
  },
  { // Entry 192
    0x1.40d9df284bdbddbc87772703bfc95645p-42,
    0x1.f7fffep-126
  },
  { // Entry 193
    -0x1.40d9df284bdbddbc87772703bfc95645p-42,
    -0x1.f7fffep-126
  },
  { // Entry 194
    0x1.945120ffff77f4d7e68f17a43da19205p-1,
    0x1.f842f0p-2
  },
  { // Entry 195
    -0x1.945120ffff77f4d7e68f17a43da19205p-1,
    -0x1.f842f0p-2
  },
  { // Entry 196
    0x1.fd92627ea97689afac4039869f00e4aep-43,
    0x1.f8bffcp-127
  },
  { // Entry 197
    -0x1.fd92627ea97689afac4039869f00e4aep-43,
    -0x1.f8bffcp-127
  },
  { // Entry 198
    0x1.feff7ee99408c9b222d85d44d72d687fp-14,
    0x1.fcfffep-40
  },
  { // Entry 199
    -0x1.feff7ee99408c9b222d85d44d72d687fp-14,
    -0x1.fcfffep-40
  },
  { // Entry 200
    0x1.4217cb047241feeb78c9a78591f7bc72p-42,
    0x1.fddffep-126
  },
  { // Entry 201
    -0x1.4217cb047241feeb78c9a78591f7bc72p-42,
    -0x1.fddffep-126
  },
  { // Entry 202
    0x1.fffa53efe9ebf75d6e114db81461dc7ep-43,
    0x1.ffeefcp-127
  },
  { // Entry 203
    -0x1.fffa53efe9ebf75d6e114db81461dc7ep-43,
    -0x1.ffeefcp-127
  },
  { // Entry 204
    0x1.965cbd06313b1d823e96d5c4737221fbp13,
    0x1.fff3fep40
  },
  { // Entry 205
    -0x1.965cbd06313b1d823e96d5c4737221fbp13,
    -0x1.fff3fep40
  },
  { // Entry 206
    0x1.42897306f46b887b6eb89d9b502e1782p-2,
    0x1.fffc7ep-6
  },
  { // Entry 207
    -0x1.42897306f46b887b6eb89d9b502e1782p-2,
    -0x1.fffc7ep-6
  },
  { // Entry 208
    0x1.42897306f46b887b6eb89d9b502e1782p-42,
    0x1.fffc7ep-126
  },
  { // Entry 209
    -0x1.42897306f46b887b6eb89d9b502e1782p-42,
    -0x1.fffc7ep-126
  },
  { // Entry 210
    0x1.965fb7005dcfa5daf92e3ebc255183c2p13,
    0x1.ffff3ep40
  },
  { // Entry 211
    -0x1.965fb7005dcfa5daf92e3ebc255183c2p13,
    -0x1.ffff3ep40
  },
  { // Entry 212
    0x1.fffff3ffffb7fffd2fffde3ffe427fe7p-42,
    0x1.ffffdcp-124
  },
  { // Entry 213
    -0x1.fffff3ffffb7fffd2fffde3ffe427fe7p-42,
    -0x1.ffffdcp-124
  },
  { // Entry 214
    0x1.fffffdfffffdfffffcaaaaa3fffff155p-14,
    0x1.fffffap-40
  },
  { // Entry 215
    -0x1.fffffdfffffdfffffcaaaaa3fffff155p-14,
    -0x1.fffffap-40
  },
  { // Entry 216
    0x1.965fe944ec46dbaa04d0e2812cfbefb0p-42,
    0x1.fffffcp-125
  },
  { // Entry 217
    -0x1.965fe944ec46dbaa04d0e2812cfbefb0p-42,
    -0x1.fffffcp-125
  },
  { // Entry 218
    0x1.55984b0000856675bab5f168f2e53b1ap-1,
    0x1.301afep-2
  },
  { // Entry 219
    -0x1.55984b0000856675bab5f168f2e53b1ap-1,
    -0x1.301afep-2
  },
  { // Entry 220
    0x1.p0,
    0x1.p0
  },
  { // Entry 221
    -0x1.p0,
    -0x1.p0
  },
  { // Entry 222
    0x1.0ce9d549c583299981fd71ff4fb99542p0,
    0x1.28ba2ep0
  },
  { // Entry 223
    -0x1.0ce9d549c583299981fd71ff4fb99542p0,
    -0x1.28ba2ep0
  },
  { // Entry 224
    0x1.18b16ebaf3cb379fae9ca7124ed79cffp0,
    0x1.51745cp0
  },
  { // Entry 225
    -0x1.18b16ebaf3cb379fae9ca7124ed79cffp0,
    -0x1.51745cp0
  },
  { // Entry 226
    0x1.238f2c18d09933e19014663c566270fcp0,
    0x1.7a2e8ap0
  },
  { // Entry 227
    -0x1.238f2c18d09933e19014663c566270fcp0,
    -0x1.7a2e8ap0
  },
  { // Entry 228
    0x1.2dabb75e8acb9144f6b52cda420fefaap0,
    0x1.a2e8b8p0
  },
  { // Entry 229
    -0x1.2dabb75e8acb9144f6b52cda420fefaap0,
    -0x1.a2e8b8p0
  },
  { // Entry 230
    0x1.3725795f7ddf1e78729e44a17e53d61ep0,
    0x1.cba2e6p0
  },
  { // Entry 231
    -0x1.3725795f7ddf1e78729e44a17e53d61ep0,
    -0x1.cba2e6p0
  },
  { // Entry 232
    0x1.4013da13344ab46d4137da308b33cf09p0,
    0x1.f45d14p0
  },
  { // Entry 233
    -0x1.4013da13344ab46d4137da308b33cf09p0,
    -0x1.f45d14p0
  },
  { // Entry 234
    0x1.48894bf47f0d516d9880c85daea2ba0ep0,
    0x1.0e8ba2p1
  },
  { // Entry 235
    -0x1.48894bf47f0d516d9880c85daea2ba0ep0,
    -0x1.0e8ba2p1
  },
  { // Entry 236
    0x1.5094a1c4f343bd5747ddd428a2e21726p0,
    0x1.22e8bap1
  },
  { // Entry 237
    -0x1.5094a1c4f343bd5747ddd428a2e21726p0,
    -0x1.22e8bap1
  },
  { // Entry 238
    0x1.5841f8f99a25bf1c33fed0059f449f12p0,
    0x1.3745d2p1
  },
  { // Entry 239
    -0x1.5841f8f99a25bf1c33fed0059f449f12p0,
    -0x1.3745d2p1
  },
  { // Entry 240
    0x1.5f9b5c89b6723a4740b02d00e63ed8c5p0,
    0x1.4ba2eap1
  },
  { // Entry 241
    -0x1.5f9b5c89b6723a4740b02d00e63ed8c5p0,
    -0x1.4ba2eap1
  },
  { // Entry 242
    0x1.66a93a398814835bf64bd954530c2e24p0,
    0x1.600002p1
  },
  { // Entry 243
    -0x1.66a93a398814835bf64bd954530c2e24p0,
    -0x1.600002p1
  },
  { // Entry 244
    0x1.6d72b8c12f197ccd2a891dce2b3420ccp0,
    0x1.745d1ap1
  },
  { // Entry 245
    -0x1.6d72b8c12f197ccd2a891dce2b3420ccp0,
    -0x1.745d1ap1
  },
  { // Entry 246
    0x1.73fdf8501e9f51315cd9208aeb0df5fdp0,
    0x1.88ba32p1
  },
  { // Entry 247
    -0x1.73fdf8501e9f51315cd9208aeb0df5fdp0,
    -0x1.88ba32p1
  },
  { // Entry 248
    0x1.7a5043b0c25062c3a77f012041ed0dd3p0,
    0x1.9d174ap1
  },
  { // Entry 249
    -0x1.7a5043b0c25062c3a77f012041ed0dd3p0,
    -0x1.9d174ap1
  },
  { // Entry 250
    0x1.806e3648370d0107430e69be51ba6181p0,
    0x1.b17462p1
  },
  { // Entry 251
    -0x1.806e3648370d0107430e69be51ba6181p0,
    -0x1.b17462p1
  },
  { // Entry 252
    0x1.865bd9deffdb100981101a7bb371c6b8p0,
    0x1.c5d17ap1
  },
  { // Entry 253
    -0x1.865bd9deffdb100981101a7bb371c6b8p0,
    -0x1.c5d17ap1
  },
  { // Entry 254
    0x1.8c1cbe427502dcf0e5ddaa6cde822e1fp0,
    0x1.da2e92p1
  },
  { // Entry 255
    -0x1.8c1cbe427502dcf0e5ddaa6cde822e1fp0,
    -0x1.da2e92p1
  },
  { // Entry 256
    0x1.91b40c3970fa45b45e1ec4f180366a63p0,
    0x1.ee8baap1
  },
  { // Entry 257
    -0x1.91b40c3970fa45b45e1ec4f180366a63p0,
    -0x1.ee8baap1
  },
  { // Entry 258
    0x1.9724945921484b9f5eb5ded43b84d0a6p0,
    0x1.017460p2
  },
  { // Entry 259
    -0x1.9724945921484b9f5eb5ded43b84d0a6p0,
    -0x1.017460p2
  },
  { // Entry 260
    0x1.9c70ddb3118685770f9b9f2ed474a9d0p0,
    0x1.0ba2ecp2
  },
  { // Entry 261
    -0x1.9c70ddb3118685770f9b9f2ed474a9d0p0,
    -0x1.0ba2ecp2
  },
  { // Entry 262
    0x1.a19b2cfb98d6b98fca487b40078f60f8p0,
    0x1.15d178p2
  },
  { // Entry 263
    -0x1.a19b2cfb98d6b98fca487b40078f60f8p0,
    -0x1.15d178p2
  },
  { // Entry 264
    0x1.a6a58f4acd2830ccf461068bd78b43dfp0,
    0x1.200004p2
  },
  { // Entry 265
    -0x1.a6a58f4acd2830ccf461068bd78b43dfp0,
    -0x1.200004p2
  },
  { // Entry 266
    0x1.ab91e0c4d7beb2de033eb06c97ff2623p0,
    0x1.2a2e90p2
  },
  { // Entry 267
    -0x1.ab91e0c4d7beb2de033eb06c97ff2623p0,
    -0x1.2a2e90p2
  },
  { // Entry 268
    0x1.b061d2aa517a9271e11f29270be275c2p0,
    0x1.345d1cp2
  },
  { // Entry 269
    -0x1.b061d2aa517a9271e11f29270be275c2p0,
    -0x1.345d1cp2
  },
  { // Entry 270
    0x1.b516f07bda4aaf3cbb849ded81335246p0,
    0x1.3e8ba8p2
  },
  { // Entry 271
    -0x1.b516f07bda4aaf3cbb849ded81335246p0,
    -0x1.3e8ba8p2
  },
  { // Entry 272
    0x1.b9b2a45b63834be7e92068f4eff7d21fp0,
    0x1.48ba34p2
  },
  { // Entry 273
    -0x1.b9b2a45b63834be7e92068f4eff7d21fp0,
    -0x1.48ba34p2
  },
  { // Entry 274
    0x1.be363accf4e07fa728c35cb0445f056bp0,
    0x1.52e8c0p2
  },
  { // Entry 275
    -0x1.be363accf4e07fa728c35cb0445f056bp0,
    -0x1.52e8c0p2
  },
  { // Entry 276
    0x1.c2a2e5f1ffae47369528dd6baf39165dp0,
    0x1.5d174cp2
  },
  { // Entry 277
    -0x1.c2a2e5f1ffae47369528dd6baf39165dp0,
    -0x1.5d174cp2
  },
  { // Entry 278
    0x1.c6f9c0551bb2a258d6a396ca52615749p0,
    0x1.6745d8p2
  },
  { // Entry 279
    -0x1.c6f9c0551bb2a258d6a396ca52615749p0,
    -0x1.6745d8p2
  },
  { // Entry 280
    0x1.cb3bcf57f15ff14a02f69b42213ee5c6p0,
    0x1.717464p2
  },
  { // Entry 281
    -0x1.cb3bcf57f15ff14a02f69b42213ee5c6p0,
    -0x1.717464p2
  },
  { // Entry 282
    0x1.cf6a0551d85b12144ae1377a6a9b72b2p0,
    0x1.7ba2f0p2
  },
  { // Entry 283
    -0x1.cf6a0551d85b12144ae1377a6a9b72b2p0,
    -0x1.7ba2f0p2
  },
  { // Entry 284
    0x1.d385436b1dde883e39c480d710236941p0,
    0x1.85d17cp2
  },
  { // Entry 285
    -0x1.d385436b1dde883e39c480d710236941p0,
    -0x1.85d17cp2
  },
  { // Entry 286
    0x1.d78e5b3ed606f3cb13a91f1357f91657p0,
    0x1.900008p2
  },
  { // Entry 287
    -0x1.d78e5b3ed606f3cb13a91f1357f91657p0,
    -0x1.900008p2
  },
  { // Entry 288
    0x1.db86104b70ee77d6387ad03f5a44c6b3p0,
    0x1.9a2e94p2
  },
  { // Entry 289
    -0x1.db86104b70ee77d6387ad03f5a44c6b3p0,
    -0x1.9a2e94p2
  },
  { // Entry 290
    0x1.df6d1938f2d285672ba023837c7650a3p0,
    0x1.a45d20p2
  },
  { // Entry 291
    -0x1.df6d1938f2d285672ba023837c7650a3p0,
    -0x1.a45d20p2
  },
  { // Entry 292
    0x1.e34420fa948c2d5fc8ce30e75343315bp0,
    0x1.ae8bacp2
  },
  { // Entry 293
    -0x1.e34420fa948c2d5fc8ce30e75343315bp0,
    -0x1.ae8bacp2
  },
  { // Entry 294
    0x1.e70bc7d0aa6f9a8c1a90bdfd23944160p0,
    0x1.b8ba38p2
  },
  { // Entry 295
    -0x1.e70bc7d0aa6f9a8c1a90bdfd23944160p0,
    -0x1.b8ba38p2
  },
  { // Entry 296
    0x1.eac4a42ef11caabdbf485346bb85dca9p0,
    0x1.c2e8c4p2
  },
  { // Entry 297
    -0x1.eac4a42ef11caabdbf485346bb85dca9p0,
    -0x1.c2e8c4p2
  },
  { // Entry 298
    0x1.ee6f438ac201cc83c3551ec3ec88f765p0,
    0x1.cd1750p2
  },
  { // Entry 299
    -0x1.ee6f438ac201cc83c3551ec3ec88f765p0,
    -0x1.cd1750p2
  },
  { // Entry 300
    0x1.f20c2b142e1141f944a5bd436f899fd8p0,
    0x1.d745dcp2
  },
  { // Entry 301
    -0x1.f20c2b142e1141f944a5bd436f899fd8p0,
    -0x1.d745dcp2
  },
  { // Entry 302
    0x1.f59bd85c8f80600f9a9091ac4ae69b2bp0,
    0x1.e17468p2
  },
  { // Entry 303
    -0x1.f59bd85c8f80600f9a9091ac4ae69b2bp0,
    -0x1.e17468p2
  },
  { // Entry 304
    0x1.f91ec1ecc7699ea908b3dc91bb2117d7p0,
    0x1.eba2f4p2
  },
  { // Entry 305
    -0x1.f91ec1ecc7699ea908b3dc91bb2117d7p0,
    -0x1.eba2f4p2
  },
  { // Entry 306
    0x1.fc9557cd0eaee03376a0df6d1ba5d501p0,
    0x1.f5d180p2
  },
  { // Entry 307
    -0x1.fc9557cd0eaee03376a0df6d1ba5d501p0,
    -0x1.f5d180p2
  },
  { // Entry 308
    0x1.p1,
    0x1.p3
  },
  { // Entry 309
    -0x1.p1,
    -0x1.p3
  },
  { // Entry 310
    0x1.428a2f98d728ae223ddab715be250d0cp33,
    0x1.p100
  },
  { // Entry 311
    -0x1.428a2f98d728ae223ddab715be250d0cp33,
    -0x1.p100
  },
  { // Entry 312
    0x1.4cf38fca0ab1d0dd5e3f8b10df7de89dp33,
    0x1.19999ap100
  },
  { // Entry 313
    -0x1.4cf38fca0ab1d0dd5e3f8b10df7de89dp33,
    -0x1.19999ap100
  },
  { // Entry 314
    0x1.56bfeab31a1e9d53e75261e0f714927bp33,
    0x1.333334p100
  },
  { // Entry 315
    -0x1.56bfeab31a1e9d53e75261e0f714927bp33,
    -0x1.333334p100
  },
  { // Entry 316
    0x1.600483d224f2459cbe9a69151151c908p33,
    0x1.4ccccep100
  },
  { // Entry 317
    -0x1.600483d224f2459cbe9a69151151c908p33,
    -0x1.4ccccep100
  },
  { // Entry 318
    0x1.68d25b2553fccd180af10eec660ead4dp33,
    0x1.666668p100
  },
  { // Entry 319
    -0x1.68d25b2553fccd180af10eec660ead4dp33,
    -0x1.666668p100
  },
  { // Entry 320
    0x1.713745353c7f5d6125705b069ec452b8p33,
    0x1.800002p100
  },
  { // Entry 321
    -0x1.713745353c7f5d6125705b069ec452b8p33,
    -0x1.800002p100
  },
  { // Entry 322
    0x1.793ead9e429872e2b3fe619f7f25153cp33,
    0x1.99999cp100
  },
  { // Entry 323
    -0x1.793ead9e429872e2b3fe619f7f25153cp33,
    -0x1.99999cp100
  },
  { // Entry 324
    0x1.80f221dd370feca7c7efbe496a84ee22p33,
    0x1.b33336p100
  },
  { // Entry 325
    -0x1.80f221dd370feca7c7efbe496a84ee22p33,
    -0x1.b33336p100
  },
  { // Entry 326
    0x1.8859b6a5ff499f31338b11076fb457bap33,
    0x1.ccccd0p100
  },
  { // Entry 327
    -0x1.8859b6a5ff499f31338b11076fb457bap33,
    -0x1.ccccd0p100
  },
  { // Entry 328
    0x1.8f7c53604ebc3a1267eb6083464ac3a6p33,
    0x1.e6666ap100
  },
  { // Entry 329
    -0x1.8f7c53604ebc3a1267eb6083464ac3a6p33,
    -0x1.e6666ap100
  },
  { // Entry 330
    0x1.965fea53d6e3c82b05999ab43dc4def1p33,
    0x1.p101
  },
  { // Entry 331
    -0x1.965fea53d6e3c82b05999ab43dc4def1p33,
    -0x1.p101
  },
  { // Entry 332
    0x1.965fe9cc61957f119f89d08a8918481ep0,
    0x1.fffffep1
  },
  { // Entry 333
    -0x1.965fe9cc61957f119f89d08a8918481ep0,
    -0x1.fffffep1
  },
  { // Entry 334
    0x1.965fea53d6e3c82b05999ab43dc4def1p0,
    0x1.p2
  },
  { // Entry 335
    -0x1.965fea53d6e3c82b05999ab43dc4def1p0,
    -0x1.p2
  },
  { // Entry 336
    0x1.965feb62c17f4b733617ccc56909e9f9p0,
    0x1.000002p2
  },
  { // Entry 337
    -0x1.965feb62c17f4b733617ccc56909e9f9p0,
    -0x1.000002p2
  },
  { // Entry 338
    0x1.428a2f2d53c3575963d11c9df5d9ad83p0,
    0x1.fffffep0
  },
  { // Entry 339
    -0x1.428a2f2d53c3575963d11c9df5d9ad83p0,
    -0x1.fffffep0
  },
  { // Entry 340
    0x1.428a2f98d728ae223ddab715be250d0cp0,
    0x1.p1
  },
  { // Entry 341
    -0x1.428a2f98d728ae223ddab715be250d0cp0,
    -0x1.p1
  },
  { // Entry 342
    0x1.428a306fddf284ad27ff7ccb79aabf3cp0,
    0x1.000002p1
  },
  { // Entry 343
    -0x1.428a306fddf284ad27ff7ccb79aabf3cp0,
    -0x1.000002p1
  },
  { // Entry 344
    0x1.ffffff5555551c71c6fcd6e9cb5339e1p-1,
    0x1.fffffep-1
  },
  { // Entry 345
    -0x1.ffffff5555551c71c6fcd6e9cb5339e1p-1,
    -0x1.fffffep-1
  },
  { // Entry 346
    0x1.p0,
    0x1.p0
  },
  { // Entry 347
    -0x1.p0,
    -0x1.p0
  },
  { // Entry 348
    0x1.000000aaaaaa38e38eb74f028086d9fcp0,
    0x1.000002p0
  },
  { // Entry 349
    -0x1.000000aaaaaa38e38eb74f028086d9fcp0,
    -0x1.000002p0
  },
  { // Entry 350
    0x1.965fe9cc61957f119f89d08a8918481ep-1,
    0x1.fffffep-2
  },
  { // Entry 351
    -0x1.965fe9cc61957f119f89d08a8918481ep-1,
    -0x1.fffffep-2
  },
  { // Entry 352
    0x1.965fea53d6e3c82b05999ab43dc4def1p-1,
    0x1.p-1
  },
  { // Entry 353
    -0x1.965fea53d6e3c82b05999ab43dc4def1p-1,
    -0x1.p-1
  },
  { // Entry 354
    0x1.965feb62c17f4b733617ccc56909e9f9p-1,
    0x1.000002p-1
  },
  { // Entry 355
    -0x1.965feb62c17f4b733617ccc56909e9f9p-1,
    -0x1.000002p-1
  },
  { // Entry 356
    0x1.428a2f2d53c3575963d11c9df5d9ad83p-1,
    0x1.fffffep-3
  },
  { // Entry 357
    -0x1.428a2f2d53c3575963d11c9df5d9ad83p-1,
    -0x1.fffffep-3
  },
  { // Entry 358
    0x1.428a2f98d728ae223ddab715be250d0cp-1,
    0x1.p-2
  },
  { // Entry 359
    -0x1.428a2f98d728ae223ddab715be250d0cp-1,
    -0x1.p-2
  },
  { // Entry 360
    0x1.428a306fddf284ad27ff7ccb79aabf3cp-1,
    0x1.000002p-2
  },
  { // Entry 361
    -0x1.428a306fddf284ad27ff7ccb79aabf3cp-1,
    -0x1.000002p-2
  },
  { // Entry 362
    0x1.ffffff5555551c71c6fcd6e9cb5339e1p-2,
    0x1.fffffep-4
  },
  { // Entry 363
    -0x1.ffffff5555551c71c6fcd6e9cb5339e1p-2,
    -0x1.fffffep-4
  },
  { // Entry 364
    0x1.p-1,
    0x1.p-3
  },
  { // Entry 365
    -0x1.p-1,
    -0x1.p-3
  },
  { // Entry 366
    0x1.000000aaaaaa38e38eb74f028086d9fcp-1,
    0x1.000002p-3
  },
  { // Entry 367
    -0x1.000000aaaaaa38e38eb74f028086d9fcp-1,
    -0x1.000002p-3
  },
  { // Entry 368
    0x1.965fe9cc61957f119f89d08a8918481ep-2,
    0x1.fffffep-5
  },
  { // Entry 369
    -0x1.965fe9cc61957f119f89d08a8918481ep-2,
    -0x1.fffffep-5
  },
  { // Entry 370
    0x1.965fea53d6e3c82b05999ab43dc4def1p-2,
    0x1.p-4
  },
  { // Entry 371
    -0x1.965fea53d6e3c82b05999ab43dc4def1p-2,
    -0x1.p-4
  },
  { // Entry 372
    0x1.965feb62c17f4b733617ccc56909e9f9p-2,
    0x1.000002p-4
  },
  { // Entry 373
    -0x1.965feb62c17f4b733617ccc56909e9f9p-2,
    -0x1.000002p-4
  },
  { // Entry 374
    0x1.428a2f2d53c3575963d11c9df5d9ad83p-2,
    0x1.fffffep-6
  },
  { // Entry 375
    -0x1.428a2f2d53c3575963d11c9df5d9ad83p-2,
    -0x1.fffffep-6
  },
  { // Entry 376
    0x1.428a2f98d728ae223ddab715be250d0cp-2,
    0x1.p-5
  },
  { // Entry 377
    -0x1.428a2f98d728ae223ddab715be250d0cp-2,
    -0x1.p-5
  },
  { // Entry 378
    0x1.428a306fddf284ad27ff7ccb79aabf3cp-2,
    0x1.000002p-5
  },
  { // Entry 379
    -0x1.428a306fddf284ad27ff7ccb79aabf3cp-2,
    -0x1.000002p-5
  },
  { // Entry 380
    0x1.ffffff5555551c71c6fcd6e9cb5339e1p-3,
    0x1.fffffep-7
  },
  { // Entry 381
    -0x1.ffffff5555551c71c6fcd6e9cb5339e1p-3,
    -0x1.fffffep-7
  },
  { // Entry 382
    0x1.p-2,
    0x1.p-6
  },
  { // Entry 383
    -0x1.p-2,
    -0x1.p-6
  },
  { // Entry 384
    0x1.000000aaaaaa38e38eb74f028086d9fcp-2,
    0x1.000002p-6
  },
  { // Entry 385
    -0x1.000000aaaaaa38e38eb74f028086d9fcp-2,
    -0x1.000002p-6
  },
  { // Entry 386
    0x1.428a2f98d728ae223ddab715be250d0cp-50,
    0x1.p-149
  },
  { // Entry 387
    -0x1.428a2f98d728ae223ddab715be250d0cp-50,
    -0x1.p-149
  },
  { // Entry 388
    -0x1.428a2f98d728ae223ddab715be250d0cp-50,
    -0x1.p-149
  },
  { // Entry 389
    0x1.428a2f98d728ae223ddab715be250d0cp-50,
    0x1.p-149
  },
  { // Entry 390
    0x1.965fe9cc61957f119f89d08a8918481ep42,
    0x1.fffffep127
  },
  { // Entry 391
    -0x1.965fe9cc61957f119f89d08a8918481ep42,
    -0x1.fffffep127
  },
  { // Entry 392
    -0x1.965fe9cc61957f119f89d08a8918481ep42,
    -0x1.fffffep127
  },
  { // Entry 393
    0x1.965fe9cc61957f119f89d08a8918481ep42,
    0x1.fffffep127
  },
  { // Entry 394
    HUGE_VALF,
    HUGE_VALF
  },
  { // Entry 395
    -HUGE_VALF,
    -HUGE_VALF
  },
  { // Entry 396
    0x1.965fe9cc61957f119f89d08a8918481ep42,
    0x1.fffffep127
  },
  { // Entry 397
    -0x1.965fe9cc61957f119f89d08a8918481ep42,
    -0x1.fffffep127
  },
  { // Entry 398
    0x1.965fe944ec46dbaa04d0e2812cfbefb0p42,
    0x1.fffffcp127
  },
  { // Entry 399
    -0x1.965fe944ec46dbaa04d0e2812cfbefb0p42,
    -0x1.fffffcp127
  },
  { // Entry 400
    0x1.76ef7ead6985271fe7617b1da5065543p0,
    0x1.921fb6p1
  },
  { // Entry 401
    -0x1.76ef7ead6985271fe7617b1da5065543p0,
    -0x1.921fb6p1
  },
  { // Entry 402
    0x1.2996267c5deedc47b88ccae60aa2742ap0,
    0x1.921fb6p0
  },
  { // Entry 403
    -0x1.2996267c5deedc47b88ccae60aa2742ap0,
    -0x1.921fb6p0
  },
  { // Entry 404
    0x1.d863a02639c8222baeb0d484991e52cdp-1,
    0x1.921fb6p-1
  },
  { // Entry 405
    -0x1.d863a02639c8222baeb0d484991e52cdp-1,
    -0x1.921fb6p-1
  },
  { // Entry 406
    0x1.p1,
    0x1.p3
  },
  { // Entry 407
    -0x1.p1,
    -0x1.p3
  },
  { // Entry 408
    0x1.428a2f98d728ae223ddab715be250d0cp0,
    0x1.p1
  },
  { // Entry 409
    -0x1.428a2f98d728ae223ddab715be250d0cp0,
    -0x1.p1
  },
  { // Entry 410
    0x1.000000aaaaaa38e38eb74f028086d9fcp0,
    0x1.000002p0
  },
  { // Entry 411
    -0x1.000000aaaaaa38e38eb74f028086d9fcp0,
    -0x1.000002p0
  },
  { // Entry 412
    0x1.p0,
    0x1.p0
  },
  { // Entry 413
    -0x1.p0,
    -0x1.p0
  },
  { // Entry 414
    0x1.ffffff5555551c71c6fcd6e9cb5339e1p-1,
    0x1.fffffep-1
  },
  { // Entry 415
    -0x1.ffffff5555551c71c6fcd6e9cb5339e1p-1,
    -0x1.fffffep-1
  },
  { // Entry 416
    0x1.000000aaaaaa38e38eb74f028086d9fcp-42,
    0x1.000002p-126
  },
  { // Entry 417
    -0x1.000000aaaaaa38e38eb74f028086d9fcp-42,
    -0x1.000002p-126
  },
  { // Entry 418
    0x1.p-42,
    0x1.p-126
  },
  { // Entry 419
    -0x1.p-42,
    -0x1.p-126
  },
  { // Entry 420
    0x1.fffffeaaaaa9c71c70ca45869598bfe9p-43,
    0x1.fffffcp-127
  },
  { // Entry 421
    -0x1.fffffeaaaaa9c71c70ca45869598bfe9p-43,
    -0x1.fffffcp-127
  },
  { // Entry 422
    0x1.fffffd555551c71c69e0650db20a4b26p-43,
    0x1.fffff8p-127
  },
  { // Entry 423
    -0x1.fffffd555551c71c69e0650db20a4b26p-43,
    -0x1.fffff8p-127
  },
  { // Entry 424
    0x1.965fea53d6e3c82b05999ab43dc4def1p-50,
    0x1.p-148
  },
  { // Entry 425
    -0x1.965fea53d6e3c82b05999ab43dc4def1p-50,
    -0x1.p-148
  },
  { // Entry 426
    0x1.428a2f98d728ae223ddab715be250d0cp-50,
    0x1.p-149
  },
  { // Entry 427
    -0x1.428a2f98d728ae223ddab715be250d0cp-50,
    -0x1.p-149
  },
  { // Entry 428
    0.0,
    0.0f
  },
  { // Entry 429
    -0.0,
    -0.0f
  },
  { // Entry 430
    0x1.80p1,
    0x1.b0p4
  },
  { // Entry 431
    -0x1.80p1,
    -0x1.b0p4
  },
  { // Entry 432
    0x1.40p2,
    0x1.f4p6
  },
  { // Entry 433
    -0x1.40p2,
    -0x1.f4p6
  }
};
