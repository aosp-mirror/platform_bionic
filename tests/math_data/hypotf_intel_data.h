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

static data_1_2_t<float, float, float> g_hypotf_intel_data[] = {
  { // Entry 0
    0x1.804fa3000c7ebdf47434f397ec84c7a0p3,
    -0x1.3372aep3,
    0x1.cd2cfap2
  },
  { // Entry 1
    0x1.75d046fff6326da34cade3039156d2ecp3,
    -0x1.3372b0p3,
    -0x1.a94388p2
  },
  { // Entry 2
    0x1.9a134b0022862e28be8459544c0a0179p-16,
    -0x1.38456cp-16,
    -0x1.09cc42p-16
  },
  { // Entry 3
    0x1.55555e0000000000000000125fff8890p-101,
    -0x1.55555ep-101,
    0x1.c0p-147
  },
  { // Entry 4
    0x1.f6c65800001e1be1724156e80ea39f54p-41,
    -0x1.5cp-62,
    -0x1.f6c658p-41
  },
  { // Entry 5
    0x1.75298b006e4054fd81616e2f820cb18ap4,
    -0x1.6b91b0p4,
    -0x1.5046aep2
  },
  { // Entry 6
    0x1.7ffffc0055555a38da2bfed0dc82cd7ap0,
    -0x1.7ffffcp0,
    0x1.000006p-16
  },
  { // Entry 7
    0x1.f13b56fc46bed2325ccd2c7cd5e69632p21,
    -0x1.f12bp21,
    0x1.fddffep15
  },
  { // Entry 8
    0x1.555570p-2,
    0x1.p-149,
    0x1.555570p-2
  },
  { // Entry 9
    0x1.55553ap100,
    0x1.p0,
    0x1.55553ap100
  },
  { // Entry 10
    0x1.6a09eaa611765e10631270d5de08ee6ep100,
    0x1.000006p100,
    0x1.p100
  },
  { // Entry 11
    0x1.6a099efd14740b74de47d3287c3ef106p-19,
    0x1.000024p-19,
    0x1.fffeeep-20
  },
  { // Entry 12
    0x1.6a15e5005f98a0819477ee86c40c70a6p0,
    0x1.0142b2p0,
    -0x1.fd9982p-1
  },
  { // Entry 13
    0x1.7c02c30005404c762e561df458868db8p6,
    0x1.077f4cp6,
    0x1.11d1ecp6
  },
  { // Entry 14
    0x1.eb772efc48db2565eb91a892b9109e1ap2,
    0x1.198cc0p0,
    -0x1.e66626p2
  },
  { // Entry 15
    0x1.fdecbd00002c3d202ee051ddfe076387p9,
    0x1.1bd856p1,
    0x1.fdec6ep9
  },
  { // Entry 16
    0x1.800000001affffffff0d0000001115ffp-23,
    0x1.20p-40,
    0x1.80p-23
  },
  { // Entry 17
    0x1.2004d7000c9e59b6c319a40f068137f1p6,
    0x1.20p6,
    0x1.a66670p-1
  },
  { // Entry 18
    0x1.38031300000fffd7a4800dc6d8c127c4p1,
    0x1.3160acp1,
    -0x1.p-1
  },
  { // Entry 19
    0x1.814cee00298d8fda8ba443fa2826cc78p9,
    0x1.41f070p2,
    0x1.814ad4p9
  },
  { // Entry 20
    0x1.ba86e8ff3abb57bc63ed628576cbf719p100,
    0x1.60p100,
    0x1.0c30c4p100
  },
  { // Entry 21
    0x1.276275p-10,
    0x1.627626p-11,
    0x1.d89d88p-11
  },
  { // Entry 22
    0x1.30e418fc6e8f1e1342c13fc83cd67621p3,
    0x1.74acc6p-1,
    -0x1.30p3
  },
  { // Entry 23
    0x1.fe01f0fc1044c738957a70f05fc15f48p3,
    0x1.758fc2p-2,
    0x1.fddfbcp3
  },
  { // Entry 24
    0x1.000009p0,
    0x1.80p-10,
    0x1.ffffeep-1
  },
  { // Entry 25
    0x1.4000030000066666570a3d851eb86f69p3,
    0x1.80000ap2,
    0x1.p3
  },
  { // Entry 26
    0x1.8bb3cb0000954ddd1ba4ba3d1be7bcb3p0,
    0x1.88603ep0,
    -0x1.99999ap-3
  },
  { // Entry 27
    0x1.068c070000048279abdd7e2cb99727aep1,
    0x1.8bab4ap-1,
    -0x1.e66674p0
  },
  { // Entry 28
    0x1.8f827b00115def47c454b3599143d2eap10,
    0x1.8f827ap10,
    0x1.c454b0p-2
  },
  { // Entry 29
    0x1.a6645cfc2ab00e5f4a76893e77c93e80p0,
    0x1.9287e2p0,
    -0x1.p-1
  },
  { // Entry 30
    0x1.1bcf0effffc79fbfe4638ed7c7f20ad3p1,
    0x1.938d28p-1,
    0x1.094590p1
  },
  { // Entry 31
    0x1.10d8ce00004de00b92c66641b1a95fc7p0,
    0x1.9e874ap-1,
    0x1.62e42ep-1
  },
  { // Entry 32
    0x1.fc516efc69a62ca47aa82a4c310ba6c3p-2,
    0x1.af1948p-6,
    -0x1.fb9a80p-2
  },
  { // Entry 33
    0x1.e9e6a4fc2ddde87638b4684564e14ce6p-3,
    0x1.b81272p-5,
    -0x1.dd633ep-3
  },
  { // Entry 34
    0x1.7396f60007c18075d8efd4c838b91544p-1,
    0x1.b89068p-3,
    0x1.62e42ep-1
  },
  { // Entry 35
    0x1.c2c3acfcf40d75a5ea60e9c114443950p1,
    0x1.c1a088p1,
    -0x1.p-2
  },
  { // Entry 36
    0x1.a43e960006dba02d6beb1d2c1eeaeceap-1,
    0x1.c2250cp-2,
    0x1.62e42ep-1
  },
  { // Entry 37
    0x1.1efb1b00082040053780041dc6b1f307p0,
    0x1.c317a0p-1,
    0x1.62e42ep-1
  },
  { // Entry 38
    0x1.58e36d0000017c0ab31fb608eac90731p3,
    0x1.ce38bap2,
    0x1.ffffccp2
  },
  { // Entry 39
    0x1.fcef76fc92680470b1a17bd9b4b35c4ap-1,
    0x1.d1e90ap-1,
    -0x1.999958p-2
  },
  { // Entry 40
    0x1.ed06b0fc3e0586fd61429f9f743c6d47p0,
    0x1.dc1edcp0,
    -0x1.p-1
  },
  { // Entry 41
    0x1.af9c310000012fae75588f7c15db9873p-1,
    0x1.eb4822p-2,
    0x1.62e42ep-1
  },
  { // Entry 42
    0x1.fffc00fc05b6516a140feda87b19a0a1p2,
    0x1.fbffbep-10,
    0x1.fffcp2
  },
  { // Entry 43
    0x1.ff7cc2fc27e17feee79397d1019b42f2p1,
    0x1.fbfffep-3,
    0x1.fe8040p1
  },
  { // Entry 44
    0x1.fe01f0fc1044c738957a70f05fc15f48p3,
    0x1.fddfbcp3,
    0x1.758fc2p-2
  },
  { // Entry 45
    0x1.f13b56fc46bed2325ccd2c7cd5e69632p21,
    0x1.fddffep15,
    -0x1.f12bp21
  },
  { // Entry 46
    0x1.c48a1cfcd1996ebda81b01af08af23dfp0,
    0x1.ff7ffep-3,
    0x1.c0p0
  },
  { // Entry 47
    0x1.ff87fb000001c07920c4dcf4f126de76p7,
    0x1.ff8ffep1,
    0x1.ff77fep7
  },
  { // Entry 48
    0x1.ffffffff8007fffff001ffeffc00bff3p-127,
    0x1.ffe0p-138,
    0x1.fffffcp-127
  },
  { // Entry 49
    0x1.01fbfb00000001fc0fda76cb886d48a6p2,
    0x1.fffbfep1,
    0x1.fff9fep-2
  },
  { // Entry 50
    0x1.1e376efdd5824c75e9a0b3b7ea4e60bcp21,
    0x1.ffffc0p20,
    0x1.000050p20
  },
  { // Entry 51
    0x1.1e376efdd469e54558718854476503e6p-19,
    0x1.ffffc6p-20,
    0x1.000044p-20
  },
  { // Entry 52
    0x1.fffff0ffffffc0000e200069ec031c2ep6,
    0x1.fffff8p-6,
    0x1.fffff0p6
  },
  { // Entry 53
    0x1.6a09e4fde9d71619ea1b09860fc404c9p0,
    0x1.fffffcp-1,
    0x1.p0
  },
  { // Entry 54
    0x1.fffff8ffffffc0000320000aec00269ep-2,
    0x1.fffffcp-14,
    0x1.fffff8p-2
  },
  { // Entry 55
    0x1.800000000000001555550000005554bdp-24,
    0x1.fffffcp-54,
    -0x1.80p-24
  },
  { // Entry 56
    0x1.fffffcfffffdc000009fffffac000035p10,
    0x1.fffffcp10,
    0x1.fffffcp-2
  },
  { // Entry 57
    0x1.fffffd000001c00002a000032c000275p12,
    0x1.fffffcp12,
    -0x1.p1
  },
  { // Entry 58
    0x1.fffffcffffffc00000a00000ec000176p12,
    0x1.fffffcp12,
    0x1.fffffep0
  },
  { // Entry 59
    0x1.6a09e93c078998f02c8d24cce0bc4b13p0,
    -0x1.000002p0,
    -0x1.000002p0
  },
  { // Entry 60
    0x1.6a09e7d1fda3e601624311059df7157bp0,
    -0x1.000002p0,
    -0x1.p0
  },
  { // Entry 61
    0x1.6a09e71cf8b1944db3c8e462b0886601p0,
    -0x1.000002p0,
    -0x1.fffffep-1
  },
  { // Entry 62
    0x1.6a09e7d1fda3e601624311059df7157bp0,
    -0x1.p0,
    -0x1.000002p0
  },
  { // Entry 63
    0x1.6a09e667f3bcc908b2fb1366ea957d3ep0,
    -0x1.p0,
    -0x1.p0
  },
  { // Entry 64
    0x1.6a09e5b2eec9c250117a2e237528575cp0,
    -0x1.p0,
    -0x1.fffffep-1
  },
  { // Entry 65
    0x1.6a09e71cf8b1944db3c8e462b0886601p0,
    -0x1.fffffep-1,
    -0x1.000002p0
  },
  { // Entry 66
    0x1.6a09e5b2eec9c250117a2e237528575cp0,
    -0x1.fffffep-1,
    -0x1.p0
  },
  { // Entry 67
    0x1.6a09e4fde9d66114f6320ab3ef821653p0,
    -0x1.fffffep-1,
    -0x1.fffffep-1
  },
  { // Entry 68
    0x1.6a09e71cf8b1944db3c8e462b0886601p0,
    -0x1.000002p0,
    0x1.fffffep-1
  },
  { // Entry 69
    0x1.6a09e7d1fda3e601624311059df7157bp0,
    -0x1.000002p0,
    0x1.p0
  },
  { // Entry 70
    0x1.6a09e93c078998f02c8d24cce0bc4b13p0,
    -0x1.000002p0,
    0x1.000002p0
  },
  { // Entry 71
    0x1.6a09e5b2eec9c250117a2e237528575cp0,
    -0x1.p0,
    0x1.fffffep-1
  },
  { // Entry 72
    0x1.6a09e667f3bcc908b2fb1366ea957d3ep0,
    -0x1.p0,
    0x1.p0
  },
  { // Entry 73
    0x1.6a09e7d1fda3e601624311059df7157bp0,
    -0x1.p0,
    0x1.000002p0
  },
  { // Entry 74
    0x1.6a09e4fde9d66114f6320ab3ef821653p0,
    -0x1.fffffep-1,
    0x1.fffffep-1
  },
  { // Entry 75
    0x1.6a09e5b2eec9c250117a2e237528575cp0,
    -0x1.fffffep-1,
    0x1.p0
  },
  { // Entry 76
    0x1.6a09e71cf8b1944db3c8e462b0886601p0,
    -0x1.fffffep-1,
    0x1.000002p0
  },
  { // Entry 77
    0x1.6a09e71cf8b1944db3c8e462b0886601p0,
    0x1.fffffep-1,
    -0x1.000002p0
  },
  { // Entry 78
    0x1.6a09e5b2eec9c250117a2e237528575cp0,
    0x1.fffffep-1,
    -0x1.p0
  },
  { // Entry 79
    0x1.6a09e4fde9d66114f6320ab3ef821653p0,
    0x1.fffffep-1,
    -0x1.fffffep-1
  },
  { // Entry 80
    0x1.6a09e7d1fda3e601624311059df7157bp0,
    0x1.p0,
    -0x1.000002p0
  },
  { // Entry 81
    0x1.6a09e667f3bcc908b2fb1366ea957d3ep0,
    0x1.p0,
    -0x1.p0
  },
  { // Entry 82
    0x1.6a09e5b2eec9c250117a2e237528575cp0,
    0x1.p0,
    -0x1.fffffep-1
  },
  { // Entry 83
    0x1.6a09e93c078998f02c8d24cce0bc4b13p0,
    0x1.000002p0,
    -0x1.000002p0
  },
  { // Entry 84
    0x1.6a09e7d1fda3e601624311059df7157bp0,
    0x1.000002p0,
    -0x1.p0
  },
  { // Entry 85
    0x1.6a09e71cf8b1944db3c8e462b0886601p0,
    0x1.000002p0,
    -0x1.fffffep-1
  },
  { // Entry 86
    0x1.6a09e4fde9d66114f6320ab3ef821653p0,
    0x1.fffffep-1,
    0x1.fffffep-1
  },
  { // Entry 87
    0x1.6a09e5b2eec9c250117a2e237528575cp0,
    0x1.fffffep-1,
    0x1.p0
  },
  { // Entry 88
    0x1.6a09e71cf8b1944db3c8e462b0886601p0,
    0x1.fffffep-1,
    0x1.000002p0
  },
  { // Entry 89
    0x1.6a09e5b2eec9c250117a2e237528575cp0,
    0x1.p0,
    0x1.fffffep-1
  },
  { // Entry 90
    0x1.6a09e667f3bcc908b2fb1366ea957d3ep0,
    0x1.p0,
    0x1.p0
  },
  { // Entry 91
    0x1.6a09e7d1fda3e601624311059df7157bp0,
    0x1.p0,
    0x1.000002p0
  },
  { // Entry 92
    0x1.6a09e71cf8b1944db3c8e462b0886601p0,
    0x1.000002p0,
    0x1.fffffep-1
  },
  { // Entry 93
    0x1.6a09e7d1fda3e601624311059df7157bp0,
    0x1.000002p0,
    0x1.p0
  },
  { // Entry 94
    0x1.6a09e93c078998f02c8d24cce0bc4b13p0,
    0x1.000002p0,
    0x1.000002p0
  },
  { // Entry 95
    0x1.6a09e667f3bcc908b2fb1366ea957d3ep0,
    -0x1.p0,
    -0x1.p0
  },
  { // Entry 96
    0x1.6a09e667f3bcc908b2fb1366ea957d3ep0,
    -0x1.p0,
    0x1.p0
  },
  { // Entry 97
    0x1.6a09e667f3bcc908b2fb1366ea957d3ep0,
    0x1.p0,
    -0x1.p0
  },
  { // Entry 98
    0x1.6a09e667f3bcc908b2fb1366ea957d3ep0,
    0x1.p0,
    0x1.p0
  },
  { // Entry 99
    0x1.6a09e667f3bcc908b2fb1366ea957d3ep0,
    0x1.p0,
    0x1.p0
  },
  { // Entry 100
    0x1.01fe03f61bad04b1068572febc925ad1p3,
    0x1.p0,
    0x1.p3
  },
  { // Entry 101
    0x1.01fe03f61bad04b1068572febc925ad1p3,
    0x1.p3,
    0x1.p0
  },
  { // Entry 102
    0x1.6a09e667f3bcc908b2fb1366ea957d3ep3,
    0x1.p3,
    0x1.p3
  },
  { // Entry 103
    0x1.00001ffffe00003ffff60001bfffacp9,
    0x1.p0,
    0x1.p9
  },
  { // Entry 104
    0x1.000007ffffe00000fffff600006ffffap10,
    0x1.p0,
    0x1.p10
  },
  { // Entry 105
    0x1.0007ffe000fff6006ffac041fca62cadp9,
    0x1.p3,
    0x1.p9
  },
  { // Entry 106
    0x1.0001fffe0003fff6001bffac0107fca6p10,
    0x1.p3,
    0x1.p10
  },
  { // Entry 107
    0x1.p100,
    0x1.p0,
    0x1.p100
  },
  { // Entry 108
    0x1.p101,
    0x1.p0,
    0x1.p101
  },
  { // Entry 109
    0x1.p100,
    0x1.p3,
    0x1.p100
  },
  { // Entry 110
    0x1.p101,
    0x1.p3,
    0x1.p101
  },
  { // Entry 111
    0x1.00001ffffe00003ffff60001bfffacp9,
    0x1.p9,
    0x1.p0
  },
  { // Entry 112
    0x1.0007ffe000fff6006ffac041fca62cadp9,
    0x1.p9,
    0x1.p3
  },
  { // Entry 113
    0x1.000007ffffe00000fffff600006ffffap10,
    0x1.p10,
    0x1.p0
  },
  { // Entry 114
    0x1.0001fffe0003fff6001bffac0107fca6p10,
    0x1.p10,
    0x1.p3
  },
  { // Entry 115
    0x1.6a09e667f3bcc908b2fb1366ea957d3ep9,
    0x1.p9,
    0x1.p9
  },
  { // Entry 116
    0x1.1e3779b97f4a7c15f39cc0605cedc834p10,
    0x1.p9,
    0x1.p10
  },
  { // Entry 117
    0x1.1e3779b97f4a7c15f39cc0605cedc834p10,
    0x1.p10,
    0x1.p9
  },
  { // Entry 118
    0x1.6a09e667f3bcc908b2fb1366ea957d3ep10,
    0x1.p10,
    0x1.p10
  },
  { // Entry 119
    0x1.p100,
    0x1.p9,
    0x1.p100
  },
  { // Entry 120
    0x1.p101,
    0x1.p9,
    0x1.p101
  },
  { // Entry 121
    0x1.p100,
    0x1.p10,
    0x1.p100
  },
  { // Entry 122
    0x1.p101,
    0x1.p10,
    0x1.p101
  },
  { // Entry 123
    0x1.p100,
    0x1.p100,
    0x1.p0
  },
  { // Entry 124
    0x1.p100,
    0x1.p100,
    0x1.p3
  },
  { // Entry 125
    0x1.p101,
    0x1.p101,
    0x1.p0
  },
  { // Entry 126
    0x1.p101,
    0x1.p101,
    0x1.p3
  },
  { // Entry 127
    0x1.p100,
    0x1.p100,
    0x1.p9
  },
  { // Entry 128
    0x1.p100,
    0x1.p100,
    0x1.p10
  },
  { // Entry 129
    0x1.p101,
    0x1.p101,
    0x1.p9
  },
  { // Entry 130
    0x1.p101,
    0x1.p101,
    0x1.p10
  },
  { // Entry 131
    0x1.6a09e667f3bcc908b2fb1366ea957d3ep100,
    0x1.p100,
    0x1.p100
  },
  { // Entry 132
    0x1.1e3779b97f4a7c15f39cc0605cedc834p101,
    0x1.p100,
    0x1.p101
  },
  { // Entry 133
    0x1.1e3779b97f4a7c15f39cc0605cedc834p101,
    0x1.p101,
    0x1.p100
  },
  { // Entry 134
    0x1.6a09e667f3bcc908b2fb1366ea957d3ep101,
    0x1.p101,
    0x1.p101
  },
  { // Entry 135
    0x1.ad533459cffc47225872f4a951e3eb94p2,
    0x1.7ffffep2,
    0x1.7ffffep1
  },
  { // Entry 136
    0x1.ad5334cc4c939b314f9ca7280b7b2232p2,
    0x1.7ffffep2,
    0x1.80p1
  },
  { // Entry 137
    0x1.ad53353ec92b695ec60ef2256f96f6bcp2,
    0x1.7ffffep2,
    0x1.800002p1
  },
  { // Entry 138
    0x1.ad533623c258e030759a4ee61d10fcc3p2,
    0x1.80p2,
    0x1.7ffffep1
  },
  { // Entry 139
    0x1.ad5336963eefba20ed6b20908b64ac4ep2,
    0x1.80p2,
    0x1.80p1
  },
  { // Entry 140
    0x1.ad533708bb870e2fe4436964fee071dep2,
    0x1.80p2,
    0x1.800002p1
  },
  { // Entry 141
    0x1.ad5337edb4b5f35d10e52ba46980f177p2,
    0x1.800002p2,
    0x1.7ffffep1
  },
  { // Entry 142
    0x1.ad533860314c532f0a41112392b8f610p2,
    0x1.800002p2,
    0x1.80p1
  },
  { // Entry 143
    0x1.ad5338d2ade32d1f82634c77c4e56d07p2,
    0x1.800002p2,
    0x1.800002p1
  },
  { // Entry 144
    0x1.fffffep127,
    0x1.fffffep127,
    0.0
  },
  { // Entry 145
    0x1.6a09e0bfcc232939bfd6f09afe47e193p-126,
    0x1.fffff8p-127,
    0x1.fffff8p-127
  },
  { // Entry 146
    0x1.6a09e229d60a463271f3020c29b89eb0p-126,
    0x1.fffff8p-127,
    0x1.fffffcp-127
  },
  { // Entry 147
    0x1.6a09e393dff2cd350be1111ca28a749bp-126,
    0x1.fffff8p-127,
    0x1.p-126
  },
  { // Entry 148
    0x1.6a09e229d60a463271f3020c29b89eb0p-126,
    0x1.fffffcp-127,
    0x1.fffff8p-127
  },
  { // Entry 149
    0x1.6a09e393dfeff92139690200f46eaf69p-126,
    0x1.fffffcp-127,
    0x1.fffffcp-127
  },
  { // Entry 150
    0x1.6a09e4fde9d71619ea1b09860fc404c9p-126,
    0x1.fffffcp-127,
    0x1.p-126
  },
  { // Entry 151
    0x1.6a09e393dff2cd350be1111ca28a749bp-126,
    0x1.p-126,
    0x1.fffff8p-127
  },
  { // Entry 152
    0x1.6a09e4fde9d71619ea1b09860fc404c9p-126,
    0x1.p-126,
    0x1.fffffcp-127
  },
  { // Entry 153
    0x1.6a09e667f3bcc908b2fb1366ea957d3ep-126,
    0x1.p-126,
    0x1.p-126
  },
  { // Entry 154
    0x1.6a09e667f3bcc908b2fb1366ea957d3ep-149,
    0x1.p-149,
    0x1.p-149
  },
  { // Entry 155
    0x1.6a09e4fde9d66114f6320ab3ef821653p64,
    0x1.fffffep63,
    0x1.fffffep63
  },
  { // Entry 156
    0x1.6a09e4fde9d66114f6320ab3ef821653p64,
    0x1.fffffep63,
    0x1.fffffep63
  },
  { // Entry 157
    0x1.6a09e4fde9d66114f6320ab3ef821653p53,
    0x1.fffffep52,
    0x1.fffffep52
  },
  { // Entry 158
    0x1.6a09e5b2eec9c250117a2e237528575cp53,
    0x1.fffffep52,
    0x1.p53
  },
  { // Entry 159
    0x1.6a09e71cf8b1944db3c8e462b0886601p53,
    0x1.fffffep52,
    0x1.000002p53
  },
  { // Entry 160
    0x1.6a09e5b2eec9c250117a2e237528575cp53,
    0x1.p53,
    0x1.fffffep52
  },
  { // Entry 161
    0x1.6a09e667f3bcc908b2fb1366ea957d3ep53,
    0x1.p53,
    0x1.p53
  },
  { // Entry 162
    0x1.6a09e7d1fda3e601624311059df7157bp53,
    0x1.p53,
    0x1.000002p53
  },
  { // Entry 163
    0x1.6a09e71cf8b1944db3c8e462b0886601p53,
    0x1.000002p53,
    0x1.fffffep52
  },
  { // Entry 164
    0x1.6a09e7d1fda3e601624311059df7157bp53,
    0x1.000002p53,
    0x1.p53
  },
  { // Entry 165
    0x1.6a09e93c078998f02c8d24cce0bc4b13p53,
    0x1.000002p53,
    0x1.000002p53
  },
  { // Entry 166
    0x1.6a09e4fde9d66114f6320ab3ef821653p-53,
    0x1.fffffep-54,
    0x1.fffffep-54
  },
  { // Entry 167
    0x1.6a09e5b2eec9c250117a2e237528575cp-53,
    0x1.fffffep-54,
    0x1.p-53
  },
  { // Entry 168
    0x1.6a09e71cf8b1944db3c8e462b0886601p-53,
    0x1.fffffep-54,
    0x1.000002p-53
  },
  { // Entry 169
    0x1.6a09e5b2eec9c250117a2e237528575cp-53,
    0x1.p-53,
    0x1.fffffep-54
  },
  { // Entry 170
    0x1.6a09e667f3bcc908b2fb1366ea957d3ep-53,
    0x1.p-53,
    0x1.p-53
  },
  { // Entry 171
    0x1.6a09e7d1fda3e601624311059df7157bp-53,
    0x1.p-53,
    0x1.000002p-53
  },
  { // Entry 172
    0x1.6a09e71cf8b1944db3c8e462b0886601p-53,
    0x1.000002p-53,
    0x1.fffffep-54
  },
  { // Entry 173
    0x1.6a09e7d1fda3e601624311059df7157bp-53,
    0x1.000002p-53,
    0x1.p-53
  },
  { // Entry 174
    0x1.6a09e93c078998f02c8d24cce0bc4b13p-53,
    0x1.000002p-53,
    0x1.000002p-53
  },
  { // Entry 175
    0x1.fffffep52,
    0x1.fffffep52,
    0x1.fffffep-54
  },
  { // Entry 176
    0x1.fffffep52,
    0x1.fffffep52,
    0x1.p-53
  },
  { // Entry 177
    0x1.fffffep52,
    0x1.fffffep52,
    0x1.000002p-53
  },
  { // Entry 178
    0x1.p53,
    0x1.p53,
    0x1.fffffep-54
  },
  { // Entry 179
    0x1.p53,
    0x1.p53,
    0x1.p-53
  },
  { // Entry 180
    0x1.p53,
    0x1.p53,
    0x1.000002p-53
  },
  { // Entry 181
    0x1.000002p53,
    0x1.000002p53,
    0x1.fffffep-54
  },
  { // Entry 182
    0x1.000002p53,
    0x1.000002p53,
    0x1.p-53
  },
  { // Entry 183
    0x1.000002p53,
    0x1.000002p53,
    0x1.000002p-53
  },
  { // Entry 184
    0x1.fffffep52,
    0x1.fffffep52,
    -0x1.p-149
  },
  { // Entry 185
    0x1.fffffep52,
    0x1.fffffep52,
    0.0
  },
  { // Entry 186
    0x1.fffffep52,
    0x1.fffffep52,
    0x1.p-149
  },
  { // Entry 187
    0x1.p53,
    0x1.p53,
    -0x1.p-149
  },
  { // Entry 188
    0x1.p53,
    0x1.p53,
    0.0
  },
  { // Entry 189
    0x1.p53,
    0x1.p53,
    0x1.p-149
  },
  { // Entry 190
    0x1.000002p53,
    0x1.000002p53,
    -0x1.p-149
  },
  { // Entry 191
    0x1.000002p53,
    0x1.000002p53,
    0.0
  },
  { // Entry 192
    0x1.000002p53,
    0x1.000002p53,
    0x1.p-149
  },
  { // Entry 193
    0x1.fffffep-54,
    0x1.fffffep-54,
    -0x1.p-149
  },
  { // Entry 194
    0x1.fffffep-54,
    0x1.fffffep-54,
    0.0
  },
  { // Entry 195
    0x1.fffffep-54,
    0x1.fffffep-54,
    0x1.p-149
  },
  { // Entry 196
    0x1.p-53,
    0x1.p-53,
    -0x1.p-149
  },
  { // Entry 197
    0x1.p-53,
    0x1.p-53,
    0.0
  },
  { // Entry 198
    0x1.p-53,
    0x1.p-53,
    0x1.p-149
  },
  { // Entry 199
    0x1.000002p-53,
    0x1.000002p-53,
    -0x1.p-149
  },
  { // Entry 200
    0x1.000002p-53,
    0x1.000002p-53,
    0.0
  },
  { // Entry 201
    0x1.000002p-53,
    0x1.000002p-53,
    0x1.p-149
  },
  { // Entry 202
    0x1.fffffe000000000000000000003fffffp52,
    0x1.fffffep52,
    0x1.fffffep-1
  },
  { // Entry 203
    0x1.fffffe0000000000000000000040p52,
    0x1.fffffep52,
    0x1.p0
  },
  { // Entry 204
    0x1.fffffe00000000000000000000400001p52,
    0x1.fffffep52,
    0x1.000002p0
  },
  { // Entry 205
    0x1.000000000000000000000000001fffffp53,
    0x1.p53,
    0x1.fffffep-1
  },
  { // Entry 206
    0x1.000000000000000000000000001fffffp53,
    0x1.p53,
    0x1.p0
  },
  { // Entry 207
    0x1.0000000000000000000000000020p53,
    0x1.p53,
    0x1.000002p0
  },
  { // Entry 208
    0x1.000002000000000000000000001fffffp53,
    0x1.000002p53,
    0x1.fffffep-1
  },
  { // Entry 209
    0x1.000002000000000000000000001fffffp53,
    0x1.000002p53,
    0x1.p0
  },
  { // Entry 210
    0x1.0000020000000000000000000020p53,
    0x1.000002p53,
    0x1.000002p0
  },
  { // Entry 211
    0x1.fffffe000000000000000000003fffffp-1,
    0x1.fffffep-54,
    0x1.fffffep-1
  },
  { // Entry 212
    0x1.000000000000000000000000001fffffp0,
    0x1.fffffep-54,
    0x1.p0
  },
  { // Entry 213
    0x1.000002000000000000000000001fffffp0,
    0x1.fffffep-54,
    0x1.000002p0
  },
  { // Entry 214
    0x1.fffffe0000000000000000000040p-1,
    0x1.p-53,
    0x1.fffffep-1
  },
  { // Entry 215
    0x1.000000000000000000000000001fffffp0,
    0x1.p-53,
    0x1.p0
  },
  { // Entry 216
    0x1.000002000000000000000000001fffffp0,
    0x1.p-53,
    0x1.000002p0
  },
  { // Entry 217
    0x1.fffffe00000000000000000000400001p-1,
    0x1.000002p-53,
    0x1.fffffep-1
  },
  { // Entry 218
    0x1.0000000000000000000000000020p0,
    0x1.000002p-53,
    0x1.p0
  },
  { // Entry 219
    0x1.0000020000000000000000000020p0,
    0x1.000002p-53,
    0x1.000002p0
  },
  { // Entry 220
    0x1.fffffe00003fffffbffffc000004p20,
    0x1.fffffep20,
    0x1.fffffep-1
  },
  { // Entry 221
    0x1.fffffe00004000003ffffc3ffff440p20,
    0x1.fffffep20,
    0x1.p0
  },
  { // Entry 222
    0x1.fffffe00004000013ffffe3fffd63fffp20,
    0x1.fffffep20,
    0x1.000002p0
  },
  { // Entry 223
    0x1.00000000001fffffbffffe200008p21,
    0x1.p21,
    0x1.fffffep-1
  },
  { // Entry 224
    0x1.00000000001ffffffffffep21,
    0x1.p21,
    0x1.p0
  },
  { // Entry 225
    0x1.00000000002000007ffffe7ffff0p21,
    0x1.p21,
    0x1.000002p0
  },
  { // Entry 226
    0x1.00000200001fffff7fffff200011bfffp21,
    0x1.000002p21,
    0x1.fffffep-1
  },
  { // Entry 227
    0x1.00000200001fffffbffffe80000bp21,
    0x1.000002p21,
    0x1.p0
  },
  { // Entry 228
    0x1.00000200002000003ffffdfffffcp21,
    0x1.000002p21,
    0x1.000002p0
  },
  { // Entry 229
    0x1.fffffe00000fffffefffffc0000040p21,
    0x1.fffffep21,
    0x1.fffffep-1
  },
  { // Entry 230
    0x1.fffffe00001000000fffffcfffff50p21,
    0x1.fffffep21,
    0x1.p0
  },
  { // Entry 231
    0x1.fffffe00001000005000004ffffdcfffp21,
    0x1.fffffep21,
    0x1.000002p0
  },
  { // Entry 232
    0x1.000000000007ffffefffffe8000080p22,
    0x1.p22,
    0x1.fffffep-1
  },
  { // Entry 233
    0x1.000000000007ffffffffffe0p22,
    0x1.p22,
    0x1.p0
  },
  { // Entry 234
    0x1.00000000000800001ffffffffffeffffp22,
    0x1.p22,
    0x1.000002p0
  },
  { // Entry 235
    0x1.000002000007ffffe00000280000afffp22,
    0x1.000002p22,
    0x1.fffffep-1
  },
  { // Entry 236
    0x1.000002000007fffff000000000007fffp22,
    0x1.000002p22,
    0x1.p0
  },
  { // Entry 237
    0x1.00000200000800000fffffdfffffc0p22,
    0x1.000002p22,
    0x1.000002p0
  },
  { // Entry 238
    0x1.fffffe000003fffffbfffffc000004p22,
    0x1.fffffep22,
    0x1.fffffep-1
  },
  { // Entry 239
    0x1.fffffe000004000003fffffffffff7ffp22,
    0x1.fffffep22,
    0x1.p0
  },
  { // Entry 240
    0x1.fffffe00000400001400001ffffff7ffp22,
    0x1.fffffep22,
    0x1.000002p0
  },
  { // Entry 241
    0x1.000000000001fffffc000000000007ffp23,
    0x1.p23,
    0x1.fffffep-1
  },
  { // Entry 242
    0x1.000000000001fffffffffffep23,
    0x1.p23,
    0x1.p0
  },
  { // Entry 243
    0x1.000000000002000008000005ffffefffp23,
    0x1.p23,
    0x1.000002p0
  },
  { // Entry 244
    0x1.000002000001fffff800000fffffefffp23,
    0x1.000002p23,
    0x1.fffffep-1
  },
  { // Entry 245
    0x1.000002000001fffffc000005fffffbffp23,
    0x1.000002p23,
    0x1.p0
  },
  { // Entry 246
    0x1.000002000002000003fffffdfffffcp23,
    0x1.000002p23,
    0x1.000002p0
  },
  { // Entry 247
    0x1.fffffe000000fffffeffffffc0000040p23,
    0x1.fffffep23,
    0x1.fffffep-1
  },
  { // Entry 248
    0x1.fffffe000001000001000000c000003fp23,
    0x1.fffffep23,
    0x1.p0
  },
  { // Entry 249
    0x1.fffffe000001000005000008c000063fp23,
    0x1.fffffep23,
    0x1.000002p0
  },
  { // Entry 250
    0x1.0000000000007fffff0000006000007fp24,
    0x1.p24,
    0x1.fffffep-1
  },
  { // Entry 251
    0x1.0000000000007fffffffffffe0p24,
    0x1.p24,
    0x1.p0
  },
  { // Entry 252
    0x1.000000000000800002000001dffffeffp24,
    0x1.p24,
    0x1.000002p0
  },
  { // Entry 253
    0x1.0000020000007ffffe0000045ffff840p24,
    0x1.000002p24,
    0x1.fffffep-1
  },
  { // Entry 254
    0x1.0000020000007fffff000001dffffcc0p24,
    0x1.000002p24,
    0x1.p0
  },
  { // Entry 255
    0x1.000002000000800000ffffffdfffffc0p24,
    0x1.000002p24,
    0x1.000002p0
  },
  { // Entry 256
    0x1.fffffe0000003fffffbffffffc000004p24,
    0x1.fffffep24,
    0x1.fffffep-1
  },
  { // Entry 257
    0x1.fffffe0000004000004000003c000034p24,
    0x1.fffffep24,
    0x1.p0
  },
  { // Entry 258
    0x1.fffffe0000004000014000023c000214p24,
    0x1.fffffep24,
    0x1.000002p0
  },
  { // Entry 259
    0x1.0000000000001fffffc000001e000007p25,
    0x1.p25,
    0x1.fffffep-1
  },
  { // Entry 260
    0x1.0000000000001ffffffffffffep25,
    0x1.p25,
    0x1.p0
  },
  { // Entry 261
    0x1.0000000000002000008000007dffffefp25,
    0x1.p25,
    0x1.000002p0
  },
  { // Entry 262
    0x1.0000020000001fffff8000011dfffdd4p25,
    0x1.000002p25,
    0x1.fffffep-1
  },
  { // Entry 263
    0x1.0000020000001fffffc000007dffff0cp25,
    0x1.000002p25,
    0x1.p0
  },
  { // Entry 264
    0x1.0000020000002000003ffffffdfffffcp25,
    0x1.000002p25,
    0x1.000002p0
  },
  { // Entry 265
    0x1.6a09e4fde9d66114f6320ab3ef821653p-60,
    0x1.fffffep-61,
    0x1.fffffep-61
  },
  { // Entry 266
    0x1.6a09e5b2eec9c250117a2e237528575cp-60,
    0x1.fffffep-61,
    0x1.p-60
  },
  { // Entry 267
    0x1.6a09e71cf8b1944db3c8e462b0886601p-60,
    0x1.fffffep-61,
    0x1.000002p-60
  },
  { // Entry 268
    0x1.6a09e5b2eec9c250117a2e237528575cp-60,
    0x1.p-60,
    0x1.fffffep-61
  },
  { // Entry 269
    0x1.6a09e667f3bcc908b2fb1366ea957d3ep-60,
    0x1.p-60,
    0x1.p-60
  },
  { // Entry 270
    0x1.6a09e7d1fda3e601624311059df7157bp-60,
    0x1.p-60,
    0x1.000002p-60
  },
  { // Entry 271
    0x1.6a09e71cf8b1944db3c8e462b0886601p-60,
    0x1.000002p-60,
    0x1.fffffep-61
  },
  { // Entry 272
    0x1.6a09e7d1fda3e601624311059df7157bp-60,
    0x1.000002p-60,
    0x1.p-60
  },
  { // Entry 273
    0x1.6a09e93c078998f02c8d24cce0bc4b13p-60,
    0x1.000002p-60,
    0x1.000002p-60
  },
  { // Entry 274
    0x1.6a09e4fde9d66114f6320ab3ef821653p-10,
    0x1.fffffep-11,
    0x1.fffffep-11
  },
  { // Entry 275
    0x1.6a09e5b2eec9c250117a2e237528575cp-10,
    0x1.fffffep-11,
    0x1.p-10
  },
  { // Entry 276
    0x1.6a09e71cf8b1944db3c8e462b0886601p-10,
    0x1.fffffep-11,
    0x1.000002p-10
  },
  { // Entry 277
    0x1.6a09e5b2eec9c250117a2e237528575cp-10,
    0x1.p-10,
    0x1.fffffep-11
  },
  { // Entry 278
    0x1.6a09e667f3bcc908b2fb1366ea957d3ep-10,
    0x1.p-10,
    0x1.p-10
  },
  { // Entry 279
    0x1.6a09e7d1fda3e601624311059df7157bp-10,
    0x1.p-10,
    0x1.000002p-10
  },
  { // Entry 280
    0x1.6a09e71cf8b1944db3c8e462b0886601p-10,
    0x1.000002p-10,
    0x1.fffffep-11
  },
  { // Entry 281
    0x1.6a09e7d1fda3e601624311059df7157bp-10,
    0x1.000002p-10,
    0x1.p-10
  },
  { // Entry 282
    0x1.6a09e93c078998f02c8d24cce0bc4b13p-10,
    0x1.000002p-10,
    0x1.000002p-10
  },
  { // Entry 283
    0x1.6a09e4fde9d66114f6320ab3ef821653p-1,
    0x1.fffffep-2,
    0x1.fffffep-2
  },
  { // Entry 284
    0x1.6a09e5b2eec9c250117a2e237528575cp-1,
    0x1.fffffep-2,
    0x1.p-1
  },
  { // Entry 285
    0x1.6a09e71cf8b1944db3c8e462b0886601p-1,
    0x1.fffffep-2,
    0x1.000002p-1
  },
  { // Entry 286
    0x1.6a09e5b2eec9c250117a2e237528575cp-1,
    0x1.p-1,
    0x1.fffffep-2
  },
  { // Entry 287
    0x1.6a09e667f3bcc908b2fb1366ea957d3ep-1,
    0x1.p-1,
    0x1.p-1
  },
  { // Entry 288
    0x1.6a09e7d1fda3e601624311059df7157bp-1,
    0x1.p-1,
    0x1.000002p-1
  },
  { // Entry 289
    0x1.6a09e71cf8b1944db3c8e462b0886601p-1,
    0x1.000002p-1,
    0x1.fffffep-2
  },
  { // Entry 290
    0x1.6a09e7d1fda3e601624311059df7157bp-1,
    0x1.000002p-1,
    0x1.p-1
  },
  { // Entry 291
    0x1.6a09e93c078998f02c8d24cce0bc4b13p-1,
    0x1.000002p-1,
    0x1.000002p-1
  },
  { // Entry 292
    0x1.6a09e4fde9d66114f6320ab3ef821653p1,
    0x1.fffffep0,
    0x1.fffffep0
  },
  { // Entry 293
    0x1.6a09e5b2eec9c250117a2e237528575cp1,
    0x1.fffffep0,
    0x1.p1
  },
  { // Entry 294
    0x1.6a09e71cf8b1944db3c8e462b0886601p1,
    0x1.fffffep0,
    0x1.000002p1
  },
  { // Entry 295
    0x1.6a09e5b2eec9c250117a2e237528575cp1,
    0x1.p1,
    0x1.fffffep0
  },
  { // Entry 296
    0x1.6a09e667f3bcc908b2fb1366ea957d3ep1,
    0x1.p1,
    0x1.p1
  },
  { // Entry 297
    0x1.6a09e7d1fda3e601624311059df7157bp1,
    0x1.p1,
    0x1.000002p1
  },
  { // Entry 298
    0x1.6a09e71cf8b1944db3c8e462b0886601p1,
    0x1.000002p1,
    0x1.fffffep0
  },
  { // Entry 299
    0x1.6a09e7d1fda3e601624311059df7157bp1,
    0x1.000002p1,
    0x1.p1
  },
  { // Entry 300
    0x1.6a09e93c078998f02c8d24cce0bc4b13p1,
    0x1.000002p1,
    0x1.000002p1
  },
  { // Entry 301
    0x1.6a09e4fde9d66114f6320ab3ef821653p10,
    0x1.fffffep9,
    0x1.fffffep9
  },
  { // Entry 302
    0x1.6a09e5b2eec9c250117a2e237528575cp10,
    0x1.fffffep9,
    0x1.p10
  },
  { // Entry 303
    0x1.6a09e71cf8b1944db3c8e462b0886601p10,
    0x1.fffffep9,
    0x1.000002p10
  },
  { // Entry 304
    0x1.6a09e5b2eec9c250117a2e237528575cp10,
    0x1.p10,
    0x1.fffffep9
  },
  { // Entry 305
    0x1.6a09e667f3bcc908b2fb1366ea957d3ep10,
    0x1.p10,
    0x1.p10
  },
  { // Entry 306
    0x1.6a09e7d1fda3e601624311059df7157bp10,
    0x1.p10,
    0x1.000002p10
  },
  { // Entry 307
    0x1.6a09e71cf8b1944db3c8e462b0886601p10,
    0x1.000002p10,
    0x1.fffffep9
  },
  { // Entry 308
    0x1.6a09e7d1fda3e601624311059df7157bp10,
    0x1.000002p10,
    0x1.p10
  },
  { // Entry 309
    0x1.6a09e93c078998f02c8d24cce0bc4b13p10,
    0x1.000002p10,
    0x1.000002p10
  },
  { // Entry 310
    0x1.6a09e4fde9d66114f6320ab3ef821653p60,
    0x1.fffffep59,
    0x1.fffffep59
  },
  { // Entry 311
    0x1.6a09e5b2eec9c250117a2e237528575cp60,
    0x1.fffffep59,
    0x1.p60
  },
  { // Entry 312
    0x1.6a09e71cf8b1944db3c8e462b0886601p60,
    0x1.fffffep59,
    0x1.000002p60
  },
  { // Entry 313
    0x1.6a09e5b2eec9c250117a2e237528575cp60,
    0x1.p60,
    0x1.fffffep59
  },
  { // Entry 314
    0x1.6a09e667f3bcc908b2fb1366ea957d3ep60,
    0x1.p60,
    0x1.p60
  },
  { // Entry 315
    0x1.6a09e7d1fda3e601624311059df7157bp60,
    0x1.p60,
    0x1.000002p60
  },
  { // Entry 316
    0x1.6a09e71cf8b1944db3c8e462b0886601p60,
    0x1.000002p60,
    0x1.fffffep59
  },
  { // Entry 317
    0x1.6a09e7d1fda3e601624311059df7157bp60,
    0x1.000002p60,
    0x1.p60
  },
  { // Entry 318
    0x1.6a09e93c078998f02c8d24cce0bc4b13p60,
    0x1.000002p60,
    0x1.000002p60
  },
  { // Entry 319
    0x1.fffffep126,
    0x1.fffffep126,
    0x1.fffffep-1
  },
  { // Entry 320
    0x1.fffffep126,
    0x1.fffffep126,
    0x1.p0
  },
  { // Entry 321
    0x1.fffffep126,
    0x1.fffffep126,
    0x1.000002p0
  },
  { // Entry 322
    0x1.p127,
    0x1.p127,
    0x1.fffffep-1
  },
  { // Entry 323
    0x1.p127,
    0x1.p127,
    0x1.p0
  },
  { // Entry 324
    0x1.p127,
    0x1.p127,
    0x1.000002p0
  },
  { // Entry 325
    0x1.000002p127,
    0x1.000002p127,
    0x1.fffffep-1
  },
  { // Entry 326
    0x1.000002p127,
    0x1.000002p127,
    0x1.p0
  },
  { // Entry 327
    0x1.000002p127,
    0x1.000002p127,
    0x1.000002p0
  },
  { // Entry 328
    0x1.778d23d44b55c4e7b10db1b757f6ee55p-27,
    0x1.00a434p-27,
    0x1.122dc2p-27
  },
  { // Entry 329
    0x1.778d254a171118efb3206477d0250ecdp-27,
    0x1.00a434p-27,
    0x1.122dc4p-27
  },
  { // Entry 330
    0x1.778d26bfe2cdb2f186ce7d3b1303e445p-27,
    0x1.00a434p-27,
    0x1.122dc6p-27
  },
  { // Entry 331
    0x1.778d25322e53c575779bf8453dfa3d1cp-27,
    0x1.00a436p-27,
    0x1.122dc2p-27
  },
  { // Entry 332
    0x1.778d26a7fa0dbd3d308d85a2de5329b5p-27,
    0x1.00a436p-27,
    0x1.122dc4p-27
  },
  { // Entry 333
    0x1.778d281dc5c8fafebca0051d801423f2p-27,
    0x1.00a436p-27,
    0x1.122dc6p-27
  },
  { // Entry 334
    0x1.778d269011533a0fa573ec7af086bc32p-27,
    0x1.00a438p-27,
    0x1.122dc2p-27
  },
  { // Entry 335
    0x1.778d2805dd0bd597165aecc2378bac1cp-27,
    0x1.00a438p-27,
    0x1.122dc4p-27
  },
  { // Entry 336
    0x1.778d297ba8c5b7185be86b37ed71b7e7p-27,
    0x1.00a438p-27,
    0x1.122dc6p-27
  },
  { // Entry 337
    0x1.74334e106cbed9ae84bc8c76dd5c5713p0,
    0x1.0b25p0,
    -0x1.032a76p0
  },
  { // Entry 338
    0x1.74334cabea5b8f74d5d14c94e0c759e1p0,
    0x1.0b25p0,
    -0x1.032a74p0
  },
  { // Entry 339
    0x1.74334b4767f9b00f6266f798ce03fe56p0,
    0x1.0b25p0,
    -0x1.032a72p0
  },
  { // Entry 340
    0x1.74334f7fe8dc915f56954cb730ec4eacp0,
    0x1.0b2502p0,
    -0x1.032a76p0
  },
  { // Entry 341
    0x1.74334e1b667aa723bef8bd9997e318dbp0,
    0x1.0b2502p0,
    -0x1.032a74p0
  },
  { // Entry 342
    0x1.74334cb6e41a27bc64192c7623c9095ep0,
    0x1.0b2502p0,
    -0x1.032a72p0
  },
  { // Entry 343
    0x1.743350ef64fb9e8af653c2c33082504fp0,
    0x1.0b2504p0,
    -0x1.032a76p0
  },
  { // Entry 344
    0x1.74334f8ae29b144d7495e8f3d356be7fp0,
    0x1.0b2504p0,
    -0x1.032a74p0
  },
  { // Entry 345
    0x1.74334e26603bf4e430d12029be7e9e50p0,
    0x1.0b2504p0,
    -0x1.032a72p0
  },
  { // Entry 346
    0x1.b6d62fc6f7a81ec948a1141efc6052a7p7,
    0x1.384560p7,
    0x1.345342p7
  },
  { // Entry 347
    0x1.b6d6312eb26c5bc486b92f8cf55694f0p7,
    0x1.384560p7,
    0x1.345344p7
  },
  { // Entry 348
    0x1.b6d632966d31c73a4041badc1888237dp7,
    0x1.384560p7,
    0x1.345346p7
  },
  { // Entry 349
    0x1.b6d631334cf2c0ff05d0de26ff401478p7,
    0x1.384562p7,
    0x1.345342p7
  },
  { // Entry 350
    0x1.b6d6329b07b5d35237676d0ef4b9f426p7,
    0x1.384562p7,
    0x1.345344p7
  },
  { // Entry 351
    0x1.b6d63402c27a141fe55cef1e3465ad6dp7,
    0x1.384562p7,
    0x1.345346p7
  },
  { // Entry 352
    0x1.b6d6329fa23e8a16b9bb93298235048cp7,
    0x1.384564p7,
    0x1.345342p7
  },
  { // Entry 353
    0x1.b6d634075d0071c1dfcec3418989e05ep7,
    0x1.384564p7,
    0x1.345344p7
  },
  { // Entry 354
    0x1.b6d6356f17c387e7832f69c16c81e0ccp7,
    0x1.384564p7,
    0x1.345346p7
  },
  { // Entry 355
    0x1.b6d6356f17c387e7832f69c16c81e0ccp-6,
    -0x1.384564p-6,
    -0x1.345346p-6
  },
  { // Entry 356
    0x1.b6d634075d0071c1dfcec3418989e05ep-6,
    -0x1.384564p-6,
    -0x1.345344p-6
  },
  { // Entry 357
    0x1.b6d6329fa23e8a16b9bb93298235048cp-6,
    -0x1.384564p-6,
    -0x1.345342p-6
  },
  { // Entry 358
    0x1.b6d63402c27a141fe55cef1e3465ad6dp-6,
    -0x1.384562p-6,
    -0x1.345346p-6
  },
  { // Entry 359
    0x1.b6d6329b07b5d35237676d0ef4b9f426p-6,
    -0x1.384562p-6,
    -0x1.345344p-6
  },
  { // Entry 360
    0x1.b6d631334cf2c0ff05d0de26ff401478p-6,
    -0x1.384562p-6,
    -0x1.345342p-6
  },
  { // Entry 361
    0x1.b6d632966d31c73a4041badc1888237dp-6,
    -0x1.384560p-6,
    -0x1.345346p-6
  },
  { // Entry 362
    0x1.b6d6312eb26c5bc486b92f8cf55694f0p-6,
    -0x1.384560p-6,
    -0x1.345344p-6
  },
  { // Entry 363
    0x1.b6d62fc6f7a81ec948a1141efc6052a7p-6,
    -0x1.384560p-6,
    -0x1.345342p-6
  },
  { // Entry 364
    0x1.9a134250dd50582b3680d82375c95486p-16,
    -0x1.384564p-16,
    -0x1.09cc3ep-16
  },
  { // Entry 365
    0x1.9a1341050095587ce0ff2f690abe3130p-16,
    -0x1.384564p-16,
    -0x1.09cc3cp-16
  },
  { // Entry 366
    0x1.9a133fb923dbcb7f9d68db36361f7a4cp-16,
    -0x1.384564p-16,
    -0x1.09cc3ap-16
  },
  { // Entry 367
    0x1.9a1340cafa61a0e627789b5e20463d1fp-16,
    -0x1.384562p-16,
    -0x1.09cc3ep-16
  },
  { // Entry 368
    0x1.9a133f7f1da565b1a938b5bc2a05ad2bp-16,
    -0x1.384562p-16,
    -0x1.09cc3cp-16
  },
  { // Entry 369
    0x1.9a133e3340ea9d2e3c45e546f2b30188p-16,
    -0x1.384562p-16,
    -0x1.09cc3ap-16
  },
  { // Entry 370
    0x1.9a133f451773f6322dcdf8373e99ef77p-16,
    -0x1.384560p-16,
    -0x1.09cc3ep-16
  },
  { // Entry 371
    0x1.9a133df93ab67f77855132975a6d3aa2p-16,
    -0x1.384560p-16,
    -0x1.09cc3cp-16
  },
  { // Entry 372
    0x1.9a133cad5dfa7b6ded8342c2ed23507ep-16,
    -0x1.384560p-16,
    -0x1.09cc3ap-16
  },
  { // Entry 373
    0x1.6a09e93c078998f02c8d24cce0bc4b13p-6,
    -0x1.000002p-6,
    -0x1.000002p-6
  },
  { // Entry 374
    0x1.6a09e7d1fda3e601624311059df7157bp-6,
    -0x1.000002p-6,
    -0x1.p-6
  },
  { // Entry 375
    0x1.6a09e71cf8b1944db3c8e462b0886601p-6,
    -0x1.000002p-6,
    -0x1.fffffep-7
  },
  { // Entry 376
    0x1.6a09e7d1fda3e601624311059df7157bp-6,
    -0x1.p-6,
    -0x1.000002p-6
  },
  { // Entry 377
    0x1.6a09e667f3bcc908b2fb1366ea957d3ep-6,
    -0x1.p-6,
    -0x1.p-6
  },
  { // Entry 378
    0x1.6a09e5b2eec9c250117a2e237528575cp-6,
    -0x1.p-6,
    -0x1.fffffep-7
  },
  { // Entry 379
    0x1.6a09e71cf8b1944db3c8e462b0886601p-6,
    -0x1.fffffep-7,
    -0x1.000002p-6
  },
  { // Entry 380
    0x1.6a09e5b2eec9c250117a2e237528575cp-6,
    -0x1.fffffep-7,
    -0x1.p-6
  },
  { // Entry 381
    0x1.6a09e4fde9d66114f6320ab3ef821653p-6,
    -0x1.fffffep-7,
    -0x1.fffffep-7
  },
  { // Entry 382
    0x1.6a09e667f3bcc908b2fb1366ea957d3ep-149,
    -0x1.p-149,
    -0x1.p-149
  },
  { // Entry 383
    0x1.p-149,
    -0x1.p-149,
    0.0
  },
  { // Entry 384
    0x1.6a09e667f3bcc908b2fb1366ea957d3ep-149,
    -0x1.p-149,
    0x1.p-149
  },
  { // Entry 385
    0x1.p-149,
    0.0,
    -0x1.p-149
  },
  { // Entry 386
    0.0,
    0.0,
    0.0
  },
  { // Entry 387
    0x1.p-149,
    0.0,
    0x1.p-149
  },
  { // Entry 388
    0x1.6a09e667f3bcc908b2fb1366ea957d3ep-149,
    0x1.p-149,
    -0x1.p-149
  },
  { // Entry 389
    0x1.p-149,
    0x1.p-149,
    0.0
  },
  { // Entry 390
    0x1.6a09e667f3bcc908b2fb1366ea957d3ep-149,
    0x1.p-149,
    0x1.p-149
  },
  { // Entry 391
    HUGE_VALF,
    HUGE_VALF,
    HUGE_VALF
  },
  { // Entry 392
    HUGE_VALF,
    HUGE_VALF,
    0x1.fffffep127
  },
  { // Entry 393
    HUGE_VALF,
    HUGE_VALF,
    0x1.p-126
  },
  { // Entry 394
    HUGE_VALF,
    HUGE_VALF,
    0x1.p-149
  },
  { // Entry 395
    HUGE_VALF,
    HUGE_VALF,
    0.0f
  },
  { // Entry 396
    HUGE_VALF,
    HUGE_VALF,
    -0.0f
  },
  { // Entry 397
    HUGE_VALF,
    HUGE_VALF,
    -0x1.p-149
  },
  { // Entry 398
    HUGE_VALF,
    HUGE_VALF,
    -0x1.p-126
  },
  { // Entry 399
    HUGE_VALF,
    HUGE_VALF,
    -0x1.fffffep127
  },
  { // Entry 400
    HUGE_VALF,
    HUGE_VALF,
    -HUGE_VALF
  },
  { // Entry 401
    HUGE_VALF,
    -HUGE_VALF,
    HUGE_VALF
  },
  { // Entry 402
    HUGE_VALF,
    -HUGE_VALF,
    0x1.fffffep127
  },
  { // Entry 403
    HUGE_VALF,
    -HUGE_VALF,
    0x1.p-126
  },
  { // Entry 404
    HUGE_VALF,
    -HUGE_VALF,
    0x1.p-149
  },
  { // Entry 405
    HUGE_VALF,
    -HUGE_VALF,
    0.0f
  },
  { // Entry 406
    HUGE_VALF,
    -HUGE_VALF,
    -0.0f
  },
  { // Entry 407
    HUGE_VALF,
    -HUGE_VALF,
    -0x1.p-149
  },
  { // Entry 408
    HUGE_VALF,
    -HUGE_VALF,
    -0x1.p-126
  },
  { // Entry 409
    HUGE_VALF,
    -HUGE_VALF,
    -0x1.fffffep127
  },
  { // Entry 410
    HUGE_VALF,
    -HUGE_VALF,
    -HUGE_VALF
  },
  { // Entry 411
    HUGE_VALF,
    0x1.fffffep127,
    HUGE_VALF
  },
  { // Entry 412
    HUGE_VALF,
    0x1.p-126,
    HUGE_VALF
  },
  { // Entry 413
    HUGE_VALF,
    0x1.p-149,
    HUGE_VALF
  },
  { // Entry 414
    HUGE_VALF,
    0.0f,
    HUGE_VALF
  },
  { // Entry 415
    HUGE_VALF,
    -0.0f,
    HUGE_VALF
  },
  { // Entry 416
    HUGE_VALF,
    -0x1.p-149,
    HUGE_VALF
  },
  { // Entry 417
    HUGE_VALF,
    -0x1.p-126,
    HUGE_VALF
  },
  { // Entry 418
    HUGE_VALF,
    -0x1.fffffep127,
    HUGE_VALF
  },
  { // Entry 419
    HUGE_VALF,
    -HUGE_VALF,
    HUGE_VALF
  },
  { // Entry 420
    HUGE_VALF,
    HUGE_VALF,
    -HUGE_VALF
  },
  { // Entry 421
    HUGE_VALF,
    0x1.fffffep127,
    -HUGE_VALF
  },
  { // Entry 422
    HUGE_VALF,
    0x1.p-126,
    -HUGE_VALF
  },
  { // Entry 423
    HUGE_VALF,
    0x1.p-149,
    -HUGE_VALF
  },
  { // Entry 424
    HUGE_VALF,
    0.0f,
    -HUGE_VALF
  },
  { // Entry 425
    HUGE_VALF,
    -0.0f,
    -HUGE_VALF
  },
  { // Entry 426
    HUGE_VALF,
    -0x1.p-149,
    -HUGE_VALF
  },
  { // Entry 427
    HUGE_VALF,
    -0x1.p-126,
    -HUGE_VALF
  },
  { // Entry 428
    HUGE_VALF,
    -0x1.fffffep127,
    -HUGE_VALF
  },
  { // Entry 429
    0x1.fffffep127,
    0.0f,
    0x1.fffffep127
  },
  { // Entry 430
    0x1.p-126,
    0.0f,
    0x1.p-126
  },
  { // Entry 431
    0x1.p-149,
    0.0f,
    0x1.p-149
  },
  { // Entry 432
    0.0,
    0.0f,
    0.0f
  },
  { // Entry 433
    0.0,
    0.0f,
    -0.0f
  },
  { // Entry 434
    0x1.p-149,
    0.0f,
    -0x1.p-149
  },
  { // Entry 435
    0x1.p-126,
    0.0f,
    -0x1.p-126
  },
  { // Entry 436
    0x1.fffffep127,
    0.0f,
    -0x1.fffffep127
  },
  { // Entry 437
    0x1.fffffep127,
    -0.0f,
    0x1.fffffep127
  },
  { // Entry 438
    0x1.p-126,
    -0.0f,
    0x1.p-126
  },
  { // Entry 439
    0x1.p-149,
    -0.0f,
    0x1.p-149
  },
  { // Entry 440
    0.0,
    -0.0f,
    0.0f
  },
  { // Entry 441
    0.0,
    -0.0f,
    -0.0f
  },
  { // Entry 442
    0x1.p-149,
    -0.0f,
    -0x1.p-149
  },
  { // Entry 443
    0x1.p-126,
    -0.0f,
    -0x1.p-126
  },
  { // Entry 444
    0x1.fffffep127,
    -0.0f,
    -0x1.fffffep127
  },
  { // Entry 445
    0x1.fffffep127,
    0x1.fffffep127,
    0.0f
  },
  { // Entry 446
    0x1.p-126,
    0x1.p-126,
    0.0f
  },
  { // Entry 447
    0x1.p-149,
    0x1.p-149,
    0.0f
  },
  { // Entry 448
    0x1.p-149,
    -0x1.p-149,
    0.0f
  },
  { // Entry 449
    0x1.p-126,
    -0x1.p-126,
    0.0f
  },
  { // Entry 450
    0x1.fffffep127,
    -0x1.fffffep127,
    0.0f
  },
  { // Entry 451
    0x1.fffffep127,
    0x1.fffffep127,
    -0.0f
  },
  { // Entry 452
    0x1.p-126,
    0x1.p-126,
    -0.0f
  },
  { // Entry 453
    0x1.p-149,
    0x1.p-149,
    -0.0f
  },
  { // Entry 454
    0x1.p-149,
    -0x1.p-149,
    -0.0f
  },
  { // Entry 455
    0x1.p-126,
    -0x1.p-126,
    -0.0f
  },
  { // Entry 456
    0x1.fffffep127,
    -0x1.fffffep127,
    -0.0f
  },
  { // Entry 457
    HUGE_VALF,
    0x1.fffffep127,
    0x1.fffffep127
  },
  { // Entry 458
    0x1.fffffep127,
    0x1.fffffep127,
    0x1.p-126
  },
  { // Entry 459
    0x1.fffffep127,
    0x1.fffffep127,
    0x1.p-149
  },
  { // Entry 460
    0x1.fffffep127,
    0x1.fffffep127,
    -0x1.p-149
  },
  { // Entry 461
    0x1.fffffep127,
    0x1.fffffep127,
    -0x1.p-126
  },
  { // Entry 462
    HUGE_VALF,
    0x1.fffffep127,
    -0x1.fffffep127
  },
  { // Entry 463
    0x1.6a09e667f3bcc908b2fb1366ea957d3ep-126,
    0x1.p-126,
    0x1.p-126
  },
  { // Entry 464
    0x1.000000000001fffffffffffep-126,
    0x1.p-126,
    0x1.p-149
  },
  { // Entry 465
    0x1.6a09e667f3bcc908b2fb1366ea957d3ep-149,
    0x1.p-149,
    0x1.p-149
  },
  { // Entry 466
    0x1.6a09e667f3bcc908b2fb1366ea957d3ep-149,
    -0x1.p-149,
    0x1.p-149
  },
  { // Entry 467
    0x1.40p2,
    0x1.80p1,
    0x1.p2
  },
  { // Entry 468
    0x1.40p2,
    0x1.80p1,
    -0x1.p2
  },
  { // Entry 469
    0x1.a0p3,
    0x1.40p2,
    0x1.80p3
  },
  { // Entry 470
    0x1.a0p3,
    0x1.40p2,
    -0x1.80p3
  }
};
