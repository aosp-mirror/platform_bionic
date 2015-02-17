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

static data_1_2_t<float, float, float> g_atan2f_intel_data[] = {
  { // Entry 0
    -0x1.ffffe4000187ffea90012c1fef963e3bp-60,
    -0x1.p-100,
    0x1.00000ep-41
  },
  { // Entry 1
    0x1.ffffe4000187ffea90012c1fef963e3bp-60,
    0x1.p-100,
    0x1.00000ep-41
  },
  { // Entry 2
    -0.0f,
    -0x1.p-100,
    0x1.00000ep50
  },
  { // Entry 3
    0.0f,
    0x1.p-100,
    0x1.00000ep50
  },
  { // Entry 4
    -0x1.7ffffffffff44cccff95f13b15ee40f3p-11,
    -0x1.000002p-10,
    0x1.555554p0
  },
  { // Entry 5
    0x1.7ffffffffff44cccff95f13b15ee40f3p-11,
    0x1.000002p-10,
    0x1.555554p0
  },
  { // Entry 6
    -0x1.fffffc00000d55550555571bbbb2d111p-23,
    -0x1.000004p0,
    0x1.000006p22
  },
  { // Entry 7
    0x1.fffffc00000d55550555571bbbb2d111p-23,
    0x1.000004p0,
    0x1.000006p22
  },
  { // Entry 8
    -0x1.dad20effbd30f4310a58502b0ff3965dp-2,
    -0x1.000006p3,
    0x1.fff186p3
  },
  { // Entry 9
    0x1.dad20effbd30f4310a58502b0ff3965dp-2,
    0x1.000006p3,
    0x1.fff186p3
  },
  { // Entry 10
    -0x1.ff654bdefc197c75159e23b86a1127c1p-5,
    -0x1.0008p16,
    0x1.000030p20
  },
  { // Entry 11
    0x1.ff654bdefc197c75159e23b86a1127c1p-5,
    0x1.0008p16,
    0x1.000030p20
  },
  { // Entry 12
    -0x1.ff9b39e1a4728254bcb91f895e52abc2p-5,
    -0x1.002304p3,
    0x1.000022p7
  },
  { // Entry 13
    0x1.ff9b39e1a4728254bcb91f895e52abc2p-5,
    0x1.002304p3,
    0x1.000022p7
  },
  { // Entry 14
    -0x1.43e6bb010a022abaa97bc92c2bf92b2dp-2,
    -0x1.04fd14p-4,
    0x1.8eb358p-3
  },
  { // Entry 15
    0x1.43e6bb010a022abaa97bc92c2bf92b2dp-2,
    0x1.04fd14p-4,
    0x1.8eb358p-3
  },
  { // Entry 16
    -0x1.905827610aa194066b73a36bcafa2041p-1,
    -0x1.0596bcp-3,
    0x1.0769dcp-3
  },
  { // Entry 17
    0x1.905827610aa194066b73a36bcafa2041p-1,
    0x1.0596bcp-3,
    0x1.0769dcp-3
  },
  { // Entry 18
    -0x1.f5b7710347b9a8b79afdefc31a2185a0p-2,
    -0x1.111118p-2,
    0x1.fffff8p-2
  },
  { // Entry 19
    0x1.f5b7710347b9a8b79afdefc31a2185a0p-2,
    0x1.111118p-2,
    0x1.fffff8p-2
  },
  { // Entry 20
    -0x1.151c477cb91ad4bb4a65e8d3fd3321f4p0,
    -0x1.111118p-14,
    0x1.222218p-15
  },
  { // Entry 21
    0x1.151c477cb91ad4bb4a65e8d3fd3321f4p0,
    0x1.111118p-14,
    0x1.222218p-15
  },
  { // Entry 22
    -0x1.520acb002e18e97cf7bea2ae9290357bp0,
    -0x1.199994p-1,
    0x1.20p-3
  },
  { // Entry 23
    0x1.520acb002e18e97cf7bea2ae9290357bp0,
    0x1.199994p-1,
    0x1.20p-3
  },
  { // Entry 24
    -0x1.d1a1ebad28ca743ee543132b45980d5cp-2,
    -0x1.199998p-1,
    0x1.20p0
  },
  { // Entry 25
    0x1.d1a1ebad28ca743ee543132b45980d5cp-2,
    0x1.199998p-1,
    0x1.20p0
  },
  { // Entry 26
    -0x1.921fb54442d18469898cc51701b839a2p0,
    -0x1.19999ap0,
    0x1.p-149
  },
  { // Entry 27
    0x1.921fb54442d18469898cc51701b839a2p0,
    0x1.19999ap0,
    0x1.p-149
  },
  { // Entry 28
    -0x1.922170fe86dc56969c78b959508174d3p-1,
    -0x1.2c0202p9,
    0x1.2bfffap9
  },
  { // Entry 29
    0x1.922170fe86dc56969c78b959508174d3p-1,
    0x1.2c0202p9,
    0x1.2bfffap9
  },
  { // Entry 30
    -0x1.8ec170fc51bb0a23bd010cc82696f548p0,
    -0x1.2ffff0p6,
    0x1.p0
  },
  { // Entry 31
    0x1.8ec170fc51bb0a23bd010cc82696f548p0,
    0x1.2ffff0p6,
    0x1.p0
  },
  { // Entry 32
    -0x1.2fffffffffffffffffffffffffffffffp-146,
    -0x1.30p-145,
    0x1.p1
  },
  { // Entry 33
    0x1.2fffffffffffffffffffffffffffffffp-146,
    0x1.30p-145,
    0x1.p1
  },
  { // Entry 34
    -0x1.2a73acfced538de0e37fe6b9b0a41ebap-2,
    -0x1.333338p-2,
    0x1.fffffcp-1
  },
  { // Entry 35
    0x1.2a73acfced538de0e37fe6b9b0a41ebap-2,
    0x1.333338p-2,
    0x1.fffffcp-1
  },
  { // Entry 36
    -0x1.921fb54442d18469898cc51701b839a2p1,
    -0x1.40p-147,
    -0x1.fffffep127
  },
  { // Entry 37
    0x1.921fb54442d18469898cc51701b839a2p1,
    0x1.40p-147,
    -0x1.fffffep127
  },
  { // Entry 38
    -0x1.3fffffffffffffffffffffffffffffffp-148,
    -0x1.40p-147,
    0x1.p1
  },
  { // Entry 39
    0x1.3fffffffffffffffffffffffffffffffp-148,
    0x1.40p-147,
    0x1.p1
  },
  { // Entry 40
    -0x1.ffa33dcc72ce5a24fbffc472e6d8aa54p-5,
    -0x1.77fffep-15,
    0x1.77c6e2p-11
  },
  { // Entry 41
    0x1.ffa33dcc72ce5a24fbffc472e6d8aa54p-5,
    0x1.77fffep-15,
    0x1.77c6e2p-11
  },
  { // Entry 42
    -0x1.72eab640dab3ff16e57acdbe73e804d6p-2,
    -0x1.7b4a16p-4,
    0x1.f474d8p-3
  },
  { // Entry 43
    0x1.72eab640dab3ff16e57acdbe73e804d6p-2,
    0x1.7b4a16p-4,
    0x1.f474d8p-3
  },
  { // Entry 44
    -0x1.7fffad001ebebf3a599c03854b51e597p-9,
    -0x1.7ffffep0,
    0x1.000006p9
  },
  { // Entry 45
    0x1.7fffad001ebebf3a599c03854b51e597p-9,
    0x1.7ffffep0,
    0x1.000006p9
  },
  { // Entry 46
    -0x1.e3539c0f15f7f48eab208803a29a2c7dp0,
    -0x1.85e85ep-1,
    -0x1.fffffcp-3
  },
  { // Entry 47
    0x1.e3539c0f15f7f48eab208803a29a2c7dp0,
    0x1.85e85ep-1,
    -0x1.fffffcp-3
  },
  { // Entry 48
    -0x1.fff7a95adac43e9c9763981911f0af91p-6,
    -0x1.881a4ap5,
    0x1.88p10
  },
  { // Entry 49
    0x1.fff7a95adac43e9c9763981911f0af91p-6,
    0x1.881a4ap5,
    0x1.88p10
  },
  { // Entry 50
    -0x1.afffffffffffffffffffffffffffffffp-146,
    -0x1.b0p-145,
    0x1.p1
  },
  { // Entry 51
    0x1.afffffffffffffffffffffffffffffffp-146,
    0x1.b0p-145,
    0x1.p1
  },
  { // Entry 52
    -0x1.a5ce8d1a28d5bcb270bc016790eb423ap0,
    -0x1.bbbbbcp-1,
    -0x1.1179f8p-4
  },
  { // Entry 53
    0x1.a5ce8d1a28d5bcb270bc016790eb423ap0,
    0x1.bbbbbcp-1,
    -0x1.1179f8p-4
  },
  { // Entry 54
    -0x1.eafe7000a5dc264f70fe1dd7f684b160p-3,
    -0x1.d55554p-1,
    0x1.dffffep1
  },
  { // Entry 55
    0x1.eafe7000a5dc264f70fe1dd7f684b160p-3,
    0x1.d55554p-1,
    0x1.dffffep1
  },
  { // Entry 56
    -0x1.eb4a75001deee59a8f1d03f2e725b3aep-2,
    -0x1.d5e926p-3,
    0x1.c38dc4p-2
  },
  { // Entry 57
    0x1.eb4a75001deee59a8f1d03f2e725b3aep-2,
    0x1.d5e926p-3,
    0x1.c38dc4p-2
  },
  { // Entry 58
    -0x1.dfffffffffffffffffffffffffffffffp-147,
    -0x1.e0p-146,
    0x1.p1
  },
  { // Entry 59
    0x1.dfffffffffffffffffffffffffffffffp-147,
    0x1.e0p-146,
    0x1.p1
  },
  { // Entry 60
    -0x1.f12ab8f4f73d14abefa8e36cac1681p-19,
    -0x1.f12a96p2,
    0x1.ffffdcp20
  },
  { // Entry 61
    0x1.f12ab8f4f73d14abefa8e36cac1681p-19,
    0x1.f12a96p2,
    0x1.ffffdcp20
  },
  { // Entry 62
    -0x1.e42b250039e7dca1fe04ee304684c0edp-2,
    -0x1.f8732ap-2,
    0x1.ed16e2p-1
  },
  { // Entry 63
    0x1.e42b250039e7dca1fe04ee304684c0edp-2,
    0x1.f8732ap-2,
    0x1.ed16e2p-1
  },
  { // Entry 64
    -0x1.ff571a1535e84a3ed0617f7c8bd43c9dp-5,
    -0x1.ffffdep-3,
    0x1.fffe7ep1
  },
  { // Entry 65
    0x1.ff571a1535e84a3ed0617f7c8bd43c9dp-5,
    0x1.ffffdep-3,
    0x1.fffe7ep1
  },
  { // Entry 66
    0.0,
    0.0,
    0.0
  },
  { // Entry 67
    0.0f,
    0x1.p-149,
    0x1.p1
  },
  { // Entry 68
    -0.0f,
    -0x1.p-149,
    0x1.p1
  },
  { // Entry 69
    0x1.5ffff7c000317ffed70006f5ffd63cp-147,
    0x1.p-149,
    0x1.745d20p-3
  },
  { // Entry 70
    -0x1.5ffff7c000317ffed70006f5ffd63cp-147,
    -0x1.p-149,
    0x1.745d20p-3
  },
  { // Entry 71
    0x1.40000dc0009740067fc0477d431261e1p-146,
    0x1.p-149,
    0x1.999988p-4
  },
  { // Entry 72
    -0x1.40000dc0009740067fc0477d431261e1p-146,
    -0x1.p-149,
    0x1.999988p-4
  },
  { // Entry 73
    0.0f,
    0x1.p-149,
    0x1.fffffep0
  },
  { // Entry 74
    -0.0f,
    -0x1.p-149,
    0x1.fffffep0
  },
  { // Entry 75
    0x1.000001000000fffaabaa9aabaa8aabddp-30,
    0x1.p2,
    0x1.fffffep31
  },
  { // Entry 76
    -0x1.000001000000fffaabaa9aabaa8aabddp-30,
    -0x1.p2,
    0x1.fffffep31
  },
  { // Entry 77
    0x1.03a264fffa8f8262b1fabf7149142cb1p-1,
    0x1.p9,
    0x1.ccccd8p9
  },
  { // Entry 78
    -0x1.03a264fffa8f8262b1fabf7149142cb1p-1,
    -0x1.p9,
    0x1.ccccd8p9
  },
  { // Entry 79
    0x1.096d02910676c2be11dcfe9fe3175278p1,
    0x1.000002p-1,
    -0x1.19998ep-2
  },
  { // Entry 80
    -0x1.096d02910676c2be11dcfe9fe3175278p1,
    -0x1.000002p-1,
    -0x1.19998ep-2
  },
  { // Entry 81
    0x1.ff54b8d04e797f1463152a327d0b86c4p-2,
    0x1.000002p-1,
    0x1.d55560p-1
  },
  { // Entry 82
    -0x1.ff54b8d04e797f1463152a327d0b86c4p-2,
    -0x1.000002p-1,
    0x1.d55560p-1
  },
  { // Entry 83
    0x1.dac67522e883aedcc9c473438e936964p-2,
    0x1.000002p-1,
    0x1.fffffep-1
  },
  { // Entry 84
    -0x1.dac67522e883aedcc9c473438e936964p-2,
    -0x1.000002p-1,
    0x1.fffffep-1
  },
  { // Entry 85
    0x1.f430999672c04e0df46fd1307191a380p-4,
    0x1.000002p-3,
    0x1.04bd9cp0
  },
  { // Entry 86
    -0x1.f430999672c04e0df46fd1307191a380p-4,
    -0x1.000002p-3,
    0x1.04bd9cp0
  },
  { // Entry 87
    0x1.7fb81eff43d4f24387e27e042d6562dbp-5,
    0x1.000002p-5,
    0x1.555552p-1
  },
  { // Entry 88
    -0x1.7fb81eff43d4f24387e27e042d6562dbp-5,
    -0x1.000002p-5,
    0x1.555552p-1
  },
  { // Entry 89
    0x1.000003000001aaaaa1aaaa80dddd98ddp-23,
    0x1.000002p-23,
    0x1.fffffep-1
  },
  { // Entry 90
    -0x1.000003000001aaaaa1aaaa80dddd98ddp-23,
    -0x1.000002p-23,
    0x1.fffffep-1
  },
  { // Entry 91
    0x1.921fb4fddc6a66f8e54f012a148cac4ep1,
    0x1.000002p-25,
    -0x1.d1745cp-1
  },
  { // Entry 92
    -0x1.921fb4fddc6a66f8e54f012a148cac4ep1,
    -0x1.000002p-25,
    -0x1.d1745cp-1
  },
  { // Entry 93
    0x1.0468a979b1a9f0624f4c1516d96c6422p1,
    0x1.000002p0,
    -0x1.000006p-1
  },
  { // Entry 94
    -0x1.0468a979b1a9f0624f4c1516d96c6422p1,
    -0x1.000002p0,
    -0x1.000006p-1
  },
  { // Entry 95
    0x1.b96e57abf90140f894091838c2b8a690p-1,
    0x1.000002p0,
    0x1.b6db76p-1
  },
  { // Entry 96
    -0x1.b96e57abf90140f894091838c2b8a690p-1,
    -0x1.000002p0,
    0x1.b6db76p-1
  },
  { // Entry 97
    0x1.f01ecfda25de70c3e0bfdea229510fd3p0,
    0x1.000002p1,
    -0x1.89d8a0p-1
  },
  { // Entry 98
    -0x1.f01ecfda25de70c3e0bfdea229510fd3p0,
    -0x1.000002p1,
    -0x1.89d8a0p-1
  },
  { // Entry 99
    0x1.ff5625094d950db0c74144886d91c14cp-5,
    0x1.000004p-3,
    0x1.ffff9ep0
  },
  { // Entry 100
    -0x1.ff5625094d950db0c74144886d91c14cp-5,
    -0x1.000004p-3,
    0x1.ffff9ep0
  },
  { // Entry 101
    0x1.fd5bd4fd7ac8b0cf6006c4414f743ea0p-4,
    0x1.000006p3,
    0x1.ffffe0p5
  },
  { // Entry 102
    -0x1.fd5bd4fd7ac8b0cf6006c4414f743ea0p-4,
    -0x1.000006p3,
    0x1.ffffe0p5
  },
  { // Entry 103
    0x1.8c4f470003e118b76491b0c859d6c053p1,
    0x1.000008p-2,
    -0x1.60p2
  },
  { // Entry 104
    -0x1.8c4f470003e118b76491b0c859d6c053p1,
    -0x1.000008p-2,
    -0x1.60p2
  },
  { // Entry 105
    0x1.2834603b51b0b1b7ada51badb8c5e787p-1,
    0x1.00000ep-20,
    0x1.88p-20
  },
  { // Entry 106
    -0x1.2834603b51b0b1b7ada51badb8c5e787p-1,
    -0x1.00000ep-20,
    0x1.88p-20
  },
  { // Entry 107
    0x1.f77e7bb64eb5f42395a6d8adcffa6337p-2,
    0x1.00000ep-20,
    0x1.ddfffep-20
  },
  { // Entry 108
    -0x1.f77e7bb64eb5f42395a6d8adcffa6337p-2,
    -0x1.00000ep-20,
    0x1.ddfffep-20
  },
  { // Entry 109
    0x1.ffd87cf6fd38249fc231c5402edbc122p-6,
    0x1.000010p-3,
    0x1.fffcfep1
  },
  { // Entry 110
    -0x1.ffd87cf6fd38249fc231c5402edbc122p-6,
    -0x1.000010p-3,
    0x1.fffcfep1
  },
  { // Entry 111
    0x1.fd5bd4fd76b8efb59210712d88b6e912p-4,
    0x1.00001ep3,
    0x1.000008p6
  },
  { // Entry 112
    -0x1.fd5bd4fd76b8efb59210712d88b6e912p-4,
    -0x1.00001ep3,
    0x1.000008p6
  },
  { // Entry 113
    0x1.fd5c357b879b2fe30dedcd3135cb691bp-4,
    0x1.000038p3,
    0x1.ffffe2p5
  },
  { // Entry 114
    -0x1.fd5c357b879b2fe30dedcd3135cb691bp-4,
    -0x1.000038p3,
    0x1.ffffe2p5
  },
  { // Entry 115
    0x1.99392cffffb1e34431dc0b78592ad27cp0,
    0x1.000262p0,
    -0x1.c67ffep-6
  },
  { // Entry 116
    -0x1.99392cffffb1e34431dc0b78592ad27cp0,
    -0x1.000262p0,
    -0x1.c67ffep-6
  },
  { // Entry 117
    0x1.ff654bdefc197c75159e23b86a1127c1p-5,
    0x1.0008p16,
    0x1.000030p20
  },
  { // Entry 118
    -0x1.ff654bdefc197c75159e23b86a1127c1p-5,
    -0x1.0008p16,
    0x1.000030p20
  },
  { // Entry 119
    0x1.321a6aeab209211260a57ffa3329874ep-1,
    0x1.000ep-20,
    0x1.77fffep-20
  },
  { // Entry 120
    -0x1.321a6aeab209211260a57ffa3329874ep-1,
    -0x1.000ep-20,
    0x1.77fffep-20
  },
  { // Entry 121
    0x1.ff753bea780e4b6715b12898d26fada0p-5,
    0x1.0010p-3,
    0x1.000030p1
  },
  { // Entry 122
    -0x1.ff753bea780e4b6715b12898d26fada0p-5,
    -0x1.0010p-3,
    0x1.000030p1
  },
  { // Entry 123
    0x1.400ea9fffd0dcf2989a4e76f8aa5db51p-1,
    0x1.001be4p-1,
    0x1.62e42ep-1
  },
  { // Entry 124
    -0x1.400ea9fffd0dcf2989a4e76f8aa5db51p-1,
    -0x1.001be4p-1,
    0x1.62e42ep-1
  },
  { // Entry 125
    0x1.ff9b39e1a4728254bcb91f895e52abc2p-5,
    0x1.002304p3,
    0x1.000022p7
  },
  { // Entry 126
    -0x1.ff9b39e1a4728254bcb91f895e52abc2p-5,
    -0x1.002304p3,
    0x1.000022p7
  },
  { // Entry 127
    0x1.943f9a4b36eb2b8033de5110689ec228p-1,
    0x1.022228p0,
    0x1.fffffcp-1
  },
  { // Entry 128
    -0x1.943f9a4b36eb2b8033de5110689ec228p-1,
    -0x1.022228p0,
    0x1.fffffcp-1
  },
  { // Entry 129
    0x1.c66450ffe905abdcfe0531d5f14c2238p0,
    0x1.0b7778p-21,
    -0x1.bb0cp-24
  },
  { // Entry 130
    -0x1.c66450ffe905abdcfe0531d5f14c2238p0,
    -0x1.0b7778p-21,
    -0x1.bb0cp-24
  },
  { // Entry 131
    0x1.f759ec36e59bd61b017b6ebaaf148489p-2,
    0x1.0df6b0p9,
    0x1.f83dc0p9
  },
  { // Entry 132
    -0x1.f759ec36e59bd61b017b6ebaaf148489p-2,
    -0x1.0df6b0p9,
    0x1.f83dc0p9
  },
  { // Entry 133
    0x1.0039e2465cf8081fc9c3f6acc6017e31p-1,
    0x1.0f83dap9,
    0x1.f07bd4p9
  },
  { // Entry 134
    -0x1.0039e2465cf8081fc9c3f6acc6017e31p-1,
    -0x1.0f83dap9,
    0x1.f07bd4p9
  },
  { // Entry 135
    0x1.32c00cffff80612ac29d96e5387e4acdp-1,
    0x1.10cee0p1,
    0x1.8f83e4p1
  },
  { // Entry 136
    -0x1.32c00cffff80612ac29d96e5387e4acdp-1,
    -0x1.10cee0p1,
    0x1.8f83e4p1
  },
  { // Entry 137
    0x1.fc9d3effcf63ce3c73d32f688b7e0d3ep-2,
    0x1.133332p9,
    0x1.fbbbacp9
  },
  { // Entry 138
    -0x1.fc9d3effcf63ce3c73d32f688b7e0d3ep-2,
    -0x1.133332p9,
    0x1.fbbbacp9
  },
  { // Entry 139
    0x1.13b7ba9cbd2bde0ae99dd5b90b6a6caep-4,
    0x1.142288p-5,
    0x1.p-1
  },
  { // Entry 140
    -0x1.13b7ba9cbd2bde0ae99dd5b90b6a6caep-4,
    -0x1.142288p-5,
    0x1.p-1
  },
  { // Entry 141
    0x1.9baeb903173549a4605c13cb0ec5c997p-1,
    0x1.1a8a08p-1,
    0x1.102e88p-1
  },
  { // Entry 142
    -0x1.9baeb903173549a4605c13cb0ec5c997p-1,
    -0x1.1a8a08p-1,
    0x1.102e88p-1
  },
  { // Entry 143
    0x1.585ed10003e25039288d2a597baabb4ep-1,
    0x1.1aab0ep-1,
    0x1.62e42ep-1
  },
  { // Entry 144
    -0x1.585ed10003e25039288d2a597baabb4ep-1,
    -0x1.1aab0ep-1,
    0x1.62e42ep-1
  },
  { // Entry 145
    0x1.fd7b30fe75452129dd4d92575b1b6643p-3,
    0x1.20p0,
    0x1.1b6db6p2
  },
  { // Entry 146
    -0x1.fd7b30fe75452129dd4d92575b1b6643p-3,
    -0x1.20p0,
    0x1.1b6db6p2
  },
  { // Entry 147
    0x1.5ee2abfffc833087a8462d843d375f40p-1,
    0x1.221ffcp-1,
    0x1.62e42ep-1
  },
  { // Entry 148
    -0x1.5ee2abfffc833087a8462d843d375f40p-1,
    -0x1.221ffcp-1,
    0x1.62e42ep-1
  },
  { // Entry 149
    0x1.922dd2fea41a07a00852062680449192p-1,
    0x1.223224p9,
    0x1.222224p9
  },
  { // Entry 150
    -0x1.922dd2fea41a07a00852062680449192p-1,
    -0x1.223224p9,
    0x1.222224p9
  },
  { // Entry 151
    0x1.fd98765b7a311ad974b5861737a89126p-4,
    0x1.3024a6p-1,
    0x1.2ffffcp2
  },
  { // Entry 152
    -0x1.fd98765b7a311ad974b5861737a89126p-4,
    -0x1.3024a6p-1,
    0x1.2ffffcp2
  },
  { // Entry 153
    0x1.ff173f59cb25f4362c94ce6ab39ece70p-4,
    0x1.310b7ep-1,
    0x1.2ffffcp2
  },
  { // Entry 154
    -0x1.ff173f59cb25f4362c94ce6ab39ece70p-4,
    -0x1.310b7ep-1,
    0x1.2ffffcp2
  },
  { // Entry 155
    0x1.893661d985cfb6e78d6ed0749b2fd803p-1,
    0x1.31f564p-4,
    0x1.3ccc80p-4
  },
  { // Entry 156
    -0x1.893661d985cfb6e78d6ed0749b2fd803p-1,
    -0x1.31f564p-4,
    0x1.3ccc80p-4
  },
  { // Entry 157
    0x1.3800a6f8595ae7372b172ef6aec40af3p-28,
    0x1.38p-20,
    0x1.fffeeep7
  },
  { // Entry 158
    -0x1.3800a6f8595ae7372b172ef6aec40af3p-28,
    -0x1.38p-20,
    0x1.fffeeep7
  },
  { // Entry 159
    0x1.f51dec230b3dcdee4d4b104276bd091bp0,
    0x1.3a58f8p0,
    -0x1.p-1
  },
  { // Entry 160
    -0x1.f51dec230b3dcdee4d4b104276bd091bp0,
    -0x1.3a58f8p0,
    -0x1.p-1
  },
  { // Entry 161
    0x1.add4fcfffc818f75eda49eae0d8f98e2p-2,
    0x1.3cc366p-2,
    0x1.62e42ep-1
  },
  { // Entry 162
    -0x1.add4fcfffc818f75eda49eae0d8f98e2p-2,
    -0x1.3cc366p-2,
    0x1.62e42ep-1
  },
  { // Entry 163
    0x1.9d6394fffffe8990edfcf5c33f9e7bc1p0,
    0x1.3fc2e4p3,
    -0x1.c28f5ep-2
  },
  { // Entry 164
    -0x1.9d6394fffffe8990edfcf5c33f9e7bc1p0,
    -0x1.3fc2e4p3,
    -0x1.c28f5ep-2
  },
  { // Entry 165
    0x1.16d00513a5c2b116688fed7c9e6d7bf9p-3,
    0x1.40a050p-6,
    0x1.24924ap-3
  },
  { // Entry 166
    -0x1.16d00513a5c2b116688fed7c9e6d7bf9p-3,
    -0x1.40a050p-6,
    0x1.24924ap-3
  },
  { // Entry 167
    0x1.6d71ea27ddde729204699db97fdd037ep-1,
    0x1.41f070p2,
    0x1.73b782p2
  },
  { // Entry 168
    -0x1.6d71ea27ddde729204699db97fdd037ep-1,
    -0x1.41f070p2,
    0x1.73b782p2
  },
  { // Entry 169
    0x1.6e2ce2182a658d8450720e677f21ce61p-1,
    0x1.429ap9,
    0x1.7368e2p9
  },
  { // Entry 170
    -0x1.6e2ce2182a658d8450720e677f21ce61p-1,
    -0x1.429ap9,
    0x1.7368e2p9
  },
  { // Entry 171
    0x1.740a75f5e00f3c2a265818a8e05ccc99p-1,
    0x1.435e54p0,
    0x1.6bca20p0
  },
  { // Entry 172
    -0x1.740a75f5e00f3c2a265818a8e05ccc99p-1,
    -0x1.435e54p0,
    0x1.6bca20p0
  },
  { // Entry 173
    0x1.4eb92766fa1641bdcd6b72f3bd619251p-1,
    0x1.5baa3ap-2,
    0x1.c5c85cp-2
  },
  { // Entry 174
    -0x1.4eb92766fa1641bdcd6b72f3bd619251p-1,
    -0x1.5baa3ap-2,
    0x1.c5c85cp-2
  },
  { // Entry 175
    0x1.feb17ca8152a6f1c96ebab23e1ca4438p-4,
    0x1.5d6c50p-8,
    0x1.5c80p-5
  },
  { // Entry 176
    -0x1.feb17ca8152a6f1c96ebab23e1ca4438p-4,
    -0x1.5d6c50p-8,
    0x1.5c80p-5
  },
  { // Entry 177
    0x1.61e054ffff517564fbb75fa927e9317dp1,
    0x1.62b140p1,
    -0x1.c0p2
  },
  { // Entry 178
    -0x1.61e054ffff517564fbb75fa927e9317dp1,
    -0x1.62b140p1,
    -0x1.c0p2
  },
  { // Entry 179
    0x1.926064fffd342f8f129a70df92a458b3p-1,
    0x1.633de6p-1,
    0x1.62e42ep-1
  },
  { // Entry 180
    -0x1.926064fffd342f8f129a70df92a458b3p-1,
    -0x1.633de6p-1,
    0x1.62e42ep-1
  },
  { // Entry 181
    0x1.ddf15cfffeff907133df83405cf1c383p-2,
    0x1.65a3e2p-2,
    0x1.62e42ep-1
  },
  { // Entry 182
    -0x1.ddf15cfffeff907133df83405cf1c383p-2,
    -0x1.65a3e2p-2,
    0x1.62e42ep-1
  },
  { // Entry 183
    0x1.a8c692fc3efe50c92076f2cdd3f6bd92p0,
    0x1.68b44ep0,
    -0x1.p-3
  },
  { // Entry 184
    -0x1.a8c692fc3efe50c92076f2cdd3f6bd92p0,
    -0x1.68b44ep0,
    -0x1.p-3
  },
  { // Entry 185
    0x1.1d730dfffc0d10826bfff4268c4db210p0,
    0x1.6a0092p0,
    0x1.62e42ep-1
  },
  { // Entry 186
    -0x1.1d730dfffc0d10826bfff4268c4db210p0,
    -0x1.6a0092p0,
    0x1.62e42ep-1
  },
  { // Entry 187
    0x1.9a06c6fffcb000f0eb371998c338bdaep-1,
    0x1.6e04f2p-1,
    0x1.62e42ep-1
  },
  { // Entry 188
    -0x1.9a06c6fffcb000f0eb371998c338bdaep-1,
    -0x1.6e04f2p-1,
    0x1.62e42ep-1
  },
  { // Entry 189
    0x1.921f9f0000092b6cc81e3cd97531299cp0,
    0x1.70p-1,
    0x1.0011p-20
  },
  { // Entry 190
    -0x1.921f9f0000092b6cc81e3cd97531299cp0,
    -0x1.70p-1,
    0x1.0011p-20
  },
  { // Entry 191
    0x1.55a1f300040b007b9fcf88e0bbaa4bf9p0,
    0x1.707652p1,
    0x1.62e42ep-1
  },
  { // Entry 192
    -0x1.55a1f300040b007b9fcf88e0bbaa4bf9p0,
    -0x1.707652p1,
    0x1.62e42ep-1
  },
  { // Entry 193
    0x1.ffa33dcc72ce5a24fbffc472e6d8aa54p-5,
    0x1.77fffep-15,
    0x1.77c6e2p-11
  },
  { // Entry 194
    -0x1.ffa33dcc72ce5a24fbffc472e6d8aa54p-5,
    -0x1.77fffep-15,
    0x1.77c6e2p-11
  },
  { // Entry 195
    0x1.7702d9c0f7f4e1f5f65e806e4e9e2eb4p-3,
    0x1.7b4274p-2,
    0x1.000006p1
  },
  { // Entry 196
    -0x1.7702d9c0f7f4e1f5f65e806e4e9e2eb4p-3,
    -0x1.7b4274p-2,
    0x1.000006p1
  },
  { // Entry 197
    0x1.fac9255e2e84501d7f69135fa78a7842p-2,
    0x1.7c1570p-2,
    0x1.601e80p-1
  },
  { // Entry 198
    -0x1.fac9255e2e84501d7f69135fa78a7842p-2,
    -0x1.7c1570p-2,
    0x1.601e80p-1
  },
  { // Entry 199
    0x1.a4c2220003e9ff7184d11c11dbed790ap-1,
    0x1.7db652p-1,
    0x1.62e42ep-1
  },
  { // Entry 200
    -0x1.a4c2220003e9ff7184d11c11dbed790ap-1,
    -0x1.7db652p-1,
    0x1.62e42ep-1
  },
  { // Entry 201
    0x1.487f682022d3a5562109a0306dcb05a2p-1,
    0x1.7e7a9ap-2,
    0x1.p-1
  },
  { // Entry 202
    -0x1.487f682022d3a5562109a0306dcb05a2p-1,
    -0x1.7e7a9ap-2,
    0x1.p-1
  },
  { // Entry 203
    0x1.e48b2fddff19e1b2ad305bf85f553acfp0,
    0x1.7fbddep0,
    -0x1.p-1
  },
  { // Entry 204
    -0x1.e48b2fddff19e1b2ad305bf85f553acfp0,
    -0x1.7fbddep0,
    -0x1.p-1
  },
  { // Entry 205
    0x1.8000030000017ffff0ffffc44ccc87ccp-23,
    0x1.80p-23,
    0x1.fffffcp-1
  },
  { // Entry 206
    -0x1.8000030000017ffff0ffffc44ccc87ccp-23,
    -0x1.80p-23,
    0x1.fffffcp-1
  },
  { // Entry 207
    0x1.236ede000419f0232206a19dc1c9ba72p0,
    0x1.807cdcp0,
    0x1.62e42ep-1
  },
  { // Entry 208
    -0x1.236ede000419f0232206a19dc1c9ba72p0,
    -0x1.807cdcp0,
    0x1.62e42ep-1
  },
  { // Entry 209
    0x1.23af91000432ff7ca91b5869446d2677p0,
    0x1.817ccep0,
    0x1.62e42ep-1
  },
  { // Entry 210
    -0x1.23af91000432ff7ca91b5869446d2677p0,
    -0x1.817ccep0,
    0x1.62e42ep-1
  },
  { // Entry 211
    0x1.1d0d78ffde75e005ce13a48bb96c20d2p1,
    0x1.86bcf6p-9,
    -0x1.2cde14p-9
  },
  { // Entry 212
    -0x1.1d0d78ffde75e005ce13a48bb96c20d2p1,
    -0x1.86bcf6p-9,
    -0x1.2cde14p-9
  },
  { // Entry 213
    0x1.c40b44f7d49ec3bebbe6c143bb874988p-17,
    0x1.88p-7,
    0x1.bbfdfep9
  },
  { // Entry 214
    -0x1.c40b44f7d49ec3bebbe6c143bb874988p-17,
    -0x1.88p-7,
    0x1.bbfdfep9
  },
  { // Entry 215
    0x1.ac7dfffd2b94ebd2b4155d81fcb743c8p-1,
    0x1.8ba2bcp9,
    0x1.64d916p9
  },
  { // Entry 216
    -0x1.ac7dfffd2b94ebd2b4155d81fcb743c8p-1,
    -0x1.8ba2bcp9,
    0x1.64d916p9
  },
  { // Entry 217
    0x1.f4436c2918d5691620bddea5f0bdb99fp0,
    0x1.904a6ap9,
    -0x1.42e220p8
  },
  { // Entry 218
    -0x1.f4436c2918d5691620bddea5f0bdb99fp0,
    -0x1.904a6ap9,
    -0x1.42e220p8
  },
  { // Entry 219
    0x1.f280a4a85a9834808487443a22c27f9cp-4,
    0x1.90c864p-5,
    0x1.99999ap-2
  },
  { // Entry 220
    -0x1.f280a4a85a9834808487443a22c27f9cp-4,
    -0x1.90c864p-5,
    0x1.99999ap-2
  },
  { // Entry 221
    0x1.0a58d9000005f0ba9a470ce5241f1b9cp-1,
    0x1.969770p-2,
    0x1.62e42ep-1
  },
  { // Entry 222
    -0x1.0a58d9000005f0ba9a470ce5241f1b9cp-1,
    -0x1.969770p-2,
    0x1.62e42ep-1
  },
  { // Entry 223
    0x1.f730a597948e5c35433d522c24bdefa5p-1,
    0x1.9b2698p9,
    0x1.1219d6p9
  },
  { // Entry 224
    -0x1.f730a597948e5c35433d522c24bdefa5p-1,
    -0x1.9b2698p9,
    0x1.1219d6p9
  },
  { // Entry 225
    0x1.25c78f0002b5030803b34e0d3d565ec5p1,
    0x1.a99552p-9,
    -0x1.788ee0p-9
  },
  { // Entry 226
    -0x1.25c78f0002b5030803b34e0d3d565ec5p1,
    -0x1.a99552p-9,
    -0x1.788ee0p-9
  },
  { // Entry 227
    0x1.fec12756125a1c17f496ca7eff6b5d07p-4,
    0x1.aac766p-1,
    0x1.a99994p2
  },
  { // Entry 228
    -0x1.fec12756125a1c17f496ca7eff6b5d07p-4,
    -0x1.aac766p-1,
    0x1.a99994p2
  },
  { // Entry 229
    0x1.ff2726fffadc57a59c068daf94011a06p-2,
    0x1.acd9c8p-2,
    0x1.89469ep-1
  },
  { // Entry 230
    -0x1.ff2726fffadc57a59c068daf94011a06p-2,
    -0x1.acd9c8p-2,
    0x1.89469ep-1
  },
  { // Entry 231
    0x1.6cefa52cd49df53a19a9664ef79b5d21p-1,
    0x1.ba8cp-2,
    0x1.p-1
  },
  { // Entry 232
    -0x1.6cefa52cd49df53a19a9664ef79b5d21p-1,
    -0x1.ba8cp-2,
    0x1.p-1
  },
  { // Entry 233
    0x1.ffecd1bdfc10703be4cadb1ac64a6eacp-6,
    0x1.bf31e2p-5,
    0x1.bf1d60p0
  },
  { // Entry 234
    -0x1.ffecd1bdfc10703be4cadb1ac64a6eacp-6,
    -0x1.bf31e2p-5,
    0x1.bf1d60p0
  },
  { // Entry 235
    0x1.d93732f77c9157c16887ce5aa762f389p-6,
    0x1.c1aep-5,
    0x1.e66658p0
  },
  { // Entry 236
    -0x1.d93732f77c9157c16887ce5aa762f389p-6,
    -0x1.c1aep-5,
    0x1.e66658p0
  },
  { // Entry 237
    0x1.cea8bcf57199048990f21a209a2d2d3ep-45,
    0x1.c25c26p-44,
    0x1.f263a0p0
  },
  { // Entry 238
    -0x1.cea8bcf57199048990f21a209a2d2d3ep-45,
    -0x1.c25c26p-44,
    0x1.f263a0p0
  },
  { // Entry 239
    0x1.90004702e62bf58fd25e1cb1c208fb8bp-1,
    0x1.c8dcb8p2,
    0x1.ccaa94p2
  },
  { // Entry 240
    -0x1.90004702e62bf58fd25e1cb1c208fb8bp-1,
    -0x1.c8dcb8p2,
    0x1.ccaa94p2
  },
  { // Entry 241
    0x1.fd7c865a3e71ad0d8a724c912f6fb8b9p-4,
    0x1.d64866p-3,
    0x1.d629c0p0
  },
  { // Entry 242
    -0x1.fd7c865a3e71ad0d8a724c912f6fb8b9p-4,
    -0x1.d64866p-3,
    0x1.d629c0p0
  },
  { // Entry 243
    0x1.4aa669000170483715efe0528369e73ep-1,
    0x1.d7011cp0,
    0x1.3880c8p1
  },
  { // Entry 244
    -0x1.4aa669000170483715efe0528369e73ep-1,
    -0x1.d7011cp0,
    0x1.3880c8p1
  },
  { // Entry 245
    0x1.f420e6032da03c581c213d0cc2eacf5bp-2,
    0x1.db6e30p-2,
    0x1.bf62a4p-1
  },
  { // Entry 246
    -0x1.f420e6032da03c581c213d0cc2eacf5bp-2,
    -0x1.db6e30p-2,
    0x1.bf62a4p-1
  },
  { // Entry 247
    0x1.922dc15dd25e294f02361f0292bc0df8p-1,
    0x1.dddddcp-2,
    0x1.ddc3a4p-2
  },
  { // Entry 248
    -0x1.922dc15dd25e294f02361f0292bc0df8p-1,
    -0x1.dddddcp-2,
    0x1.ddc3a4p-2
  },
  { // Entry 249
    0x1.9d6fd902defaede7830883b7a2788da8p-1,
    0x1.de61fcp9,
    0x1.c9b22cp9
  },
  { // Entry 250
    -0x1.9d6fd902defaede7830883b7a2788da8p-1,
    -0x1.de61fcp9,
    0x1.c9b22cp9
  },
  { // Entry 251
    0x1.7ee180ca27095c5506b0fa68e94004d0p-4,
    0x1.dffffep-2,
    0x1.40p2
  },
  { // Entry 252
    -0x1.7ee180ca27095c5506b0fa68e94004d0p-4,
    -0x1.dffffep-2,
    0x1.40p2
  },
  { // Entry 253
    0x1.7d848f000bfaf243f75b3a1218dad94ep0,
    0x1.dffffep2,
    0x1.35c292p-1
  },
  { // Entry 254
    -0x1.7d848f000bfaf243f75b3a1218dad94ep0,
    -0x1.dffffep2,
    0x1.35c292p-1
  },
  { // Entry 255
    0x1.dfffe63601c1383bc54eea3773a4624fp-11,
    0x1.dffffep10,
    0x1.000008p21
  },
  { // Entry 256
    -0x1.dfffe63601c1383bc54eea3773a4624fp-11,
    -0x1.dffffep10,
    0x1.000008p21
  },
  { // Entry 257
    0x1.8f82d1b1443d17b008f18f7822175902p-4,
    0x1.e0f078p-6,
    0x1.333334p-2
  },
  { // Entry 258
    -0x1.8f82d1b1443d17b008f18f7822175902p-4,
    -0x1.e0f078p-6,
    0x1.333334p-2
  },
  { // Entry 259
    0x1.07795d7bc568d7597605f1e44388198ep1,
    0x1.e2be36p-2,
    -0x1.fffffcp-3
  },
  { // Entry 260
    -0x1.07795d7bc568d7597605f1e44388198ep1,
    -0x1.e2be36p-2,
    -0x1.fffffcp-3
  },
  { // Entry 261
    0x1.fff95e57a0b39bb8afc31a89674dc197p-92,
    0x1.e62448p8,
    0x1.e62a94p99
  },
  { // Entry 262
    -0x1.fff95e57a0b39bb8afc31a89674dc197p-92,
    -0x1.e62448p8,
    0x1.e62a94p99
  },
  { // Entry 263
    0x1.0e06f7000a4e54e7181ed79d635dead3p0,
    0x1.e783d4p-1,
    0x1.148cf8p-1
  },
  { // Entry 264
    -0x1.0e06f7000a4e54e7181ed79d635dead3p0,
    -0x1.e783d4p-1,
    0x1.148cf8p-1
  },
  { // Entry 265
    0x1.fea63fbd167cf3f4fa0987d1e28cd75fp-4,
    0x1.e7a55ap-1,
    0x1.e66660p2
  },
  { // Entry 266
    -0x1.fea63fbd167cf3f4fa0987d1e28cd75fp-4,
    -0x1.e7a55ap-1,
    0x1.e66660p2
  },
  { // Entry 267
    0x1.d32abcfffffee42f5ad6cc888072e445p0,
    0x1.ece6d4p0,
    -0x1.p-1
  },
  { // Entry 268
    -0x1.d32abcfffffee42f5ad6cc888072e445p0,
    -0x1.ece6d4p0,
    -0x1.p-1
  },
  { // Entry 269
    0x1.ecac96fad1d02ec25eecde4b7f0b97eap-4,
    0x1.ef1060p-5,
    0x1.p-1
  },
  { // Entry 270
    -0x1.ecac96fad1d02ec25eecde4b7f0b97eap-4,
    -0x1.ef1060p-5,
    0x1.p-1
  },
  { // Entry 271
    0x1.1202c2e6b84549d2bdd30f506adfa9d4p0,
    0x1.f07bd2p9,
    0x1.0f83dcp9
  },
  { // Entry 272
    -0x1.1202c2e6b84549d2bdd30f506adfa9d4p0,
    -0x1.f07bd2p9,
    0x1.0f83dcp9
  },
  { // Entry 273
    0x1.ffe7d9701b27043f401f2771fcff61aap-6,
    0x1.fddffep15,
    0x1.fdcd90p20
  },
  { // Entry 274
    -0x1.ffe7d9701b27043f401f2771fcff61aap-6,
    -0x1.fddffep15,
    0x1.fdcd90p20
  },
  { // Entry 275
    0x1.919c94434fc91fb77041e0d4eaadf614p-1,
    0x1.fefffep-10,
    0x1.0003p-9
  },
  { // Entry 276
    -0x1.919c94434fc91fb77041e0d4eaadf614p-1,
    -0x1.fefffep-10,
    0x1.0003p-9
  },
  { // Entry 277
    0x1.1b57780820085bc0391cbd61b2fd2335p0,
    0x1.ff8ffep-20,
    0x1.000088p-20
  },
  { // Entry 278
    -0x1.1b57780820085bc0391cbd61b2fd2335p0,
    -0x1.ff8ffep-20,
    0x1.000088p-20
  },
  { // Entry 279
    0x1.ff976af6e817ac0b343cc79da075b8a8p-6,
    0x1.ffc0p-139,
    0x1.fffep-134
  },
  { // Entry 280
    -0x1.ff976af6e817ac0b343cc79da075b8a8p-6,
    -0x1.ffc0p-139,
    0x1.fffep-134
  },
  { // Entry 281
    0x1.ff7fc3e4933e894c72260e0533856313p-4,
    0x1.fff77ep-5,
    0x1.fdcd2ep-2
  },
  { // Entry 282
    -0x1.ff7fc3e4933e894c72260e0533856313p-4,
    -0x1.fff77ep-5,
    0x1.fdcd2ep-2
  },
  { // Entry 283
    0x1.ff571a1535e84a3ed0617f7c8bd43c9dp-5,
    0x1.ffffdep-3,
    0x1.fffe7ep1
  },
  { // Entry 284
    -0x1.ff571a1535e84a3ed0617f7c8bd43c9dp-5,
    -0x1.ffffdep-3,
    0x1.fffe7ep1
  },
  { // Entry 285
    0x1.fffffdfffff7ffffdfffff7ffffdffffp-127,
    0x1.fffff6p0,
    0x1.fffff8p126
  },
  { // Entry 286
    -0x1.fffffdfffff7ffffdfffff7ffffdffffp-127,
    -0x1.fffff6p0,
    0x1.fffff8p126
  },
  { // Entry 287
    0x1.ffffec000077fffd05556b3554a0155bp-34,
    0x1.fffff8p-2,
    0x1.000006p32
  },
  { // Entry 288
    -0x1.ffffec000077fffd05556b3554a0155bp-34,
    -0x1.fffff8p-2,
    0x1.000006p32
  },
  { // Entry 289
    0x1.55554fffffffffffffffffffffffffffp-104,
    0x1.fffff8p-127,
    0x1.80p-23
  },
  { // Entry 290
    -0x1.55554fffffffffffffffffffffffffffp-104,
    -0x1.fffff8p-127,
    0x1.80p-23
  },
  { // Entry 291
    0x1.ff54beeda807aa4ec5698ce8cc7dcba8p-2,
    0x1.fffffcp-1,
    0x1.d55552p0
  },
  { // Entry 292
    -0x1.ff54beeda807aa4ec5698ce8cc7dcba8p-2,
    -0x1.fffffcp-1,
    0x1.d55552p0
  },
  { // Entry 293
    0x1.fffff800000fffffe000003fffff80p-129,
    0x1.fffffcp-2,
    0x1.000002p127
  },
  { // Entry 294
    -0x1.fffff800000fffffe000003fffff80p-129,
    -0x1.fffffcp-2,
    0x1.000002p127
  },
  { // Entry 295
    0x1.d7625deb9d3d113e0be1ba5dac42e6c0p-2,
    0x1.fffffcp-2,
    0x1.022228p0
  },
  { // Entry 296
    -0x1.d7625deb9d3d113e0be1ba5dac42e6c0p-2,
    -0x1.fffffcp-2,
    0x1.022228p0
  },
  { // Entry 297
    0x1.0c30b75fc8b0637fcbaf3ed21f47bbd5p-118,
    0x1.fffffcp-122,
    0x1.e8ba40p-4
  },
  { // Entry 298
    -0x1.0c30b75fc8b0637fcbaf3ed21f47bbd5p-118,
    -0x1.fffffcp-122,
    0x1.e8ba40p-4
  },
  { // Entry 299
    0x1.fffffbffffffffffffffffffffffffffp-127,
    0x1.fffffcp-126,
    0x1.p1
  },
  { // Entry 300
    -0x1.fffffbffffffffffffffffffffffffffp-127,
    -0x1.fffffcp-126,
    0x1.p1
  },
  { // Entry 301
    0x1.f5b748fc32492f9b0e1a9e29c7b40a45p-3,
    0x1.fffffcp20,
    0x1.00000ap23
  },
  { // Entry 302
    -0x1.f5b748fc32492f9b0e1a9e29c7b40a45p-3,
    -0x1.fffffcp20,
    0x1.00000ap23
  },
  { // Entry 303
    0x1.e8009efffc72402f56046bbb3775db7ep-2,
    0x1.6e6d52p-2,
    0x1.62e42ep-1
  },
  { // Entry 304
    -0x1.e8009efffc72402f56046bbb3775db7ep-2,
    -0x1.6e6d52p-2,
    0x1.62e42ep-1
  },
  { // Entry 305
    -0x1.2d97c7f3321d234f272993d1414a2b39p1,
    -0x1.p0,
    -0x1.p0
  },
  { // Entry 306
    0x1.2d97c7f3321d234f272993d1414a2b39p1,
    0x1.p0,
    -0x1.p0
  },
  { // Entry 307
    -0x1.921fb54442d18469898cc51701b839a2p-1,
    -0x1.p0,
    0x1.p0
  },
  { // Entry 308
    0x1.921fb54442d18469898cc51701b839a2p-1,
    0x1.p0,
    0x1.p0
  },
  { // Entry 309
    0x1.2d97c7f3321d234f272993d1414a2b39p1,
    0x1.p0,
    -0x1.p0
  },
  { // Entry 310
    -0x1.2d97c7f3321d234f272993d1414a2b39p1,
    -0x1.p0,
    -0x1.p0
  },
  { // Entry 311
    0x1.921fb54442d18469898cc51701b839a2p-1,
    0x1.p0,
    0x1.p0
  },
  { // Entry 312
    -0x1.921fb54442d18469898cc51701b839a2p-1,
    -0x1.p0,
    0x1.p0
  },
  { // Entry 313
    0x1.921fb54442d18469898cc51701b839a2p-1,
    0x1.p0,
    0x1.p0
  },
  { // Entry 314
    -0x1.921fb54442d18469898cc51701b839a2p-1,
    -0x1.p0,
    0x1.p0
  },
  { // Entry 315
    0x1.fd5ba9aac2f6dc65912f313e7d111defp-4,
    0x1.p0,
    0x1.p3
  },
  { // Entry 316
    -0x1.fd5ba9aac2f6dc65912f313e7d111defp-4,
    -0x1.p0,
    0x1.p3
  },
  { // Entry 317
    0x1.7249faa996a216a33079d20319e727c3p0,
    0x1.p3,
    0x1.p0
  },
  { // Entry 318
    -0x1.7249faa996a216a33079d20319e727c3p0,
    -0x1.p3,
    0x1.p0
  },
  { // Entry 319
    0x1.921fb54442d18469898cc51701b839a2p-1,
    0x1.p3,
    0x1.p3
  },
  { // Entry 320
    -0x1.921fb54442d18469898cc51701b839a2p-1,
    -0x1.p3,
    0x1.p3
  },
  { // Entry 321
    0x1.ffffd5555bbbba9729ab7aac089473a3p-10,
    0x1.p0,
    0x1.p9
  },
  { // Entry 322
    -0x1.ffffd5555bbbba9729ab7aac089473a3p-10,
    -0x1.p0,
    0x1.p9
  },
  { // Entry 323
    0x1.fffff55555bbbbb72972d00cfde752f9p-11,
    0x1.p0,
    0x1.p10
  },
  { // Entry 324
    -0x1.fffff55555bbbbb72972d00cfde752f9p-11,
    -0x1.p0,
    0x1.p10
  },
  { // Entry 325
    0x1.fff555bbb729ab77cf18ac802beec090p-7,
    0x1.p3,
    0x1.p9
  },
  { // Entry 326
    -0x1.fff555bbb729ab77cf18ac802beec090p-7,
    -0x1.p3,
    0x1.p9
  },
  { // Entry 327
    0x1.fffd555bbba972d00c46a3f77cc15e8ep-8,
    0x1.p3,
    0x1.p10
  },
  { // Entry 328
    -0x1.fffd555bbba972d00c46a3f77cc15e8ep-8,
    -0x1.p3,
    0x1.p10
  },
  { // Entry 329
    0x1.ffffffffffffffffffffffffffffffffp-101,
    0x1.p0,
    0x1.p100
  },
  { // Entry 330
    -0x1.ffffffffffffffffffffffffffffffffp-101,
    -0x1.p0,
    0x1.p100
  },
  { // Entry 331
    0x1.ffffffffffffffffffffffffffffffffp-102,
    0x1.p0,
    0x1.p101
  },
  { // Entry 332
    -0x1.ffffffffffffffffffffffffffffffffp-102,
    -0x1.p0,
    0x1.p101
  },
  { // Entry 333
    0x1.ffffffffffffffffffffffffffffffffp-98,
    0x1.p3,
    0x1.p100
  },
  { // Entry 334
    -0x1.ffffffffffffffffffffffffffffffffp-98,
    -0x1.p3,
    0x1.p100
  },
  { // Entry 335
    0x1.ffffffffffffffffffffffffffffffffp-99,
    0x1.p3,
    0x1.p101
  },
  { // Entry 336
    -0x1.ffffffffffffffffffffffffffffffffp-99,
    -0x1.p3,
    0x1.p101
  },
  { // Entry 337
    0x1.919fb54eed7a957ae3c25a3856b61485p0,
    0x1.p9,
    0x1.p0
  },
  { // Entry 338
    -0x1.919fb54eed7a957ae3c25a3856b61485p0,
    -0x1.p9,
    0x1.p0
  },
  { // Entry 339
    0x1.8e1fca98cb63311299ee93be01605c21p0,
    0x1.p9,
    0x1.p3
  },
  { // Entry 340
    -0x1.8e1fca98cb63311299ee93be01605c21p0,
    -0x1.p9,
    0x1.p3
  },
  { // Entry 341
    0x1.91dfb5459826ccf212a796bd00187cb7p0,
    0x1.p10,
    0x1.p0
  },
  { // Entry 342
    -0x1.91dfb5459826ccf212a796bd00187cb7p0,
    -0x1.p10,
    0x1.p0
  },
  { // Entry 343
    0x1.901fb7eee715daf6b9807e730a3b7843p0,
    0x1.p10,
    0x1.p3
  },
  { // Entry 344
    -0x1.901fb7eee715daf6b9807e730a3b7843p0,
    -0x1.p10,
    0x1.p3
  },
  { // Entry 345
    0x1.921fb54442d18469898cc51701b839a2p-1,
    0x1.p9,
    0x1.p9
  },
  { // Entry 346
    -0x1.921fb54442d18469898cc51701b839a2p-1,
    -0x1.p9,
    0x1.p9
  },
  { // Entry 347
    0x1.dac670561bb4f68adfc88bd978751a06p-2,
    0x1.p9,
    0x1.p10
  },
  { // Entry 348
    -0x1.dac670561bb4f68adfc88bd978751a06p-2,
    -0x1.p9,
    0x1.p10
  },
  { // Entry 349
    0x1.1b6e192ebbe446c6d19aa220a39af320p0,
    0x1.p10,
    0x1.p9
  },
  { // Entry 350
    -0x1.1b6e192ebbe446c6d19aa220a39af320p0,
    -0x1.p10,
    0x1.p9
  },
  { // Entry 351
    0x1.921fb54442d18469898cc51701b839a2p-1,
    0x1.p10,
    0x1.p10
  },
  { // Entry 352
    -0x1.921fb54442d18469898cc51701b839a2p-1,
    -0x1.p10,
    0x1.p10
  },
  { // Entry 353
    0x1.ffffffffffffffffffffffffffffffffp-92,
    0x1.p9,
    0x1.p100
  },
  { // Entry 354
    -0x1.ffffffffffffffffffffffffffffffffp-92,
    -0x1.p9,
    0x1.p100
  },
  { // Entry 355
    0x1.ffffffffffffffffffffffffffffffffp-93,
    0x1.p9,
    0x1.p101
  },
  { // Entry 356
    -0x1.ffffffffffffffffffffffffffffffffp-93,
    -0x1.p9,
    0x1.p101
  },
  { // Entry 357
    0x1.ffffffffffffffffffffffffffffffffp-91,
    0x1.p10,
    0x1.p100
  },
  { // Entry 358
    -0x1.ffffffffffffffffffffffffffffffffp-91,
    -0x1.p10,
    0x1.p100
  },
  { // Entry 359
    0x1.ffffffffffffffffffffffffffffffffp-92,
    0x1.p10,
    0x1.p101
  },
  { // Entry 360
    -0x1.ffffffffffffffffffffffffffffffffp-92,
    -0x1.p10,
    0x1.p101
  },
  { // Entry 361
    0x1.921fb54442d18469898cc516f1b839a2p0,
    0x1.p100,
    0x1.p0
  },
  { // Entry 362
    -0x1.921fb54442d18469898cc516f1b839a2p0,
    -0x1.p100,
    0x1.p0
  },
  { // Entry 363
    0x1.921fb54442d18469898cc51681b839a2p0,
    0x1.p100,
    0x1.p3
  },
  { // Entry 364
    -0x1.921fb54442d18469898cc51681b839a2p0,
    -0x1.p100,
    0x1.p3
  },
  { // Entry 365
    0x1.921fb54442d18469898cc516f9b839a2p0,
    0x1.p101,
    0x1.p0
  },
  { // Entry 366
    -0x1.921fb54442d18469898cc516f9b839a2p0,
    -0x1.p101,
    0x1.p0
  },
  { // Entry 367
    0x1.921fb54442d18469898cc516c1b839a2p0,
    0x1.p101,
    0x1.p3
  },
  { // Entry 368
    -0x1.921fb54442d18469898cc516c1b839a2p0,
    -0x1.p101,
    0x1.p3
  },
  { // Entry 369
    0x1.921fb54442d18469898cc4f701b839a2p0,
    0x1.p100,
    0x1.p9
  },
  { // Entry 370
    -0x1.921fb54442d18469898cc4f701b839a2p0,
    -0x1.p100,
    0x1.p9
  },
  { // Entry 371
    0x1.921fb54442d18469898cc4d701b839a2p0,
    0x1.p100,
    0x1.p10
  },
  { // Entry 372
    -0x1.921fb54442d18469898cc4d701b839a2p0,
    -0x1.p100,
    0x1.p10
  },
  { // Entry 373
    0x1.921fb54442d18469898cc50701b839a2p0,
    0x1.p101,
    0x1.p9
  },
  { // Entry 374
    -0x1.921fb54442d18469898cc50701b839a2p0,
    -0x1.p101,
    0x1.p9
  },
  { // Entry 375
    0x1.921fb54442d18469898cc4f701b839a2p0,
    0x1.p101,
    0x1.p10
  },
  { // Entry 376
    -0x1.921fb54442d18469898cc4f701b839a2p0,
    -0x1.p101,
    0x1.p10
  },
  { // Entry 377
    0x1.921fb54442d18469898cc51701b839a2p-1,
    0x1.p100,
    0x1.p100
  },
  { // Entry 378
    -0x1.921fb54442d18469898cc51701b839a2p-1,
    -0x1.p100,
    0x1.p100
  },
  { // Entry 379
    0x1.dac670561bb4f68adfc88bd978751a06p-2,
    0x1.p100,
    0x1.p101
  },
  { // Entry 380
    -0x1.dac670561bb4f68adfc88bd978751a06p-2,
    -0x1.p100,
    0x1.p101
  },
  { // Entry 381
    0x1.1b6e192ebbe446c6d19aa220a39af320p0,
    0x1.p101,
    0x1.p100
  },
  { // Entry 382
    -0x1.1b6e192ebbe446c6d19aa220a39af320p0,
    -0x1.p101,
    0x1.p100
  },
  { // Entry 383
    0x1.921fb54442d18469898cc51701b839a2p-1,
    0x1.p101,
    0x1.p101
  },
  { // Entry 384
    -0x1.921fb54442d18469898cc51701b839a2p-1,
    -0x1.p101,
    0x1.p101
  },
  { // Entry 385
    -0.0f,
    -0x1.p-149,
    0x1.fffffep0
  },
  { // Entry 386
    0.0f,
    0x1.p-149,
    0x1.fffffep0
  },
  { // Entry 387
    -0.0f,
    -0x1.p-149,
    0x1.p1
  },
  { // Entry 388
    0.0f,
    0x1.p-149,
    0x1.p1
  },
  { // Entry 389
    -0.0f,
    -0x1.p-149,
    0x1.000002p1
  },
  { // Entry 390
    0.0f,
    0x1.p-149,
    0x1.000002p1
  },
  { // Entry 391
    0.0,
    0.0,
    0x1.fffffep0
  },
  { // Entry 392
    0.0,
    0.0,
    0x1.p1
  },
  { // Entry 393
    0.0,
    0.0,
    0x1.000002p1
  },
  { // Entry 394
    0.0f,
    0x1.p-149,
    0x1.fffffep0
  },
  { // Entry 395
    -0.0f,
    -0x1.p-149,
    0x1.fffffep0
  },
  { // Entry 396
    0.0f,
    0x1.p-149,
    0x1.p1
  },
  { // Entry 397
    -0.0f,
    -0x1.p-149,
    0x1.p1
  },
  { // Entry 398
    0.0f,
    0x1.p-149,
    0x1.000002p1
  },
  { // Entry 399
    -0.0f,
    -0x1.p-149,
    0x1.000002p1
  },
  { // Entry 400
    0x1.ff55bb72cfde9c6d964f25b81c5c1aa2p-5,
    0x1.fffffep-4,
    0x1.fffffep0
  },
  { // Entry 401
    -0x1.ff55bb72cfde9c6d964f25b81c5c1aa2p-5,
    -0x1.fffffep-4,
    0x1.fffffep0
  },
  { // Entry 402
    0x1.ff55b974cde098738e59c07aa48dd110p-5,
    0x1.fffffep-4,
    0x1.p1
  },
  { // Entry 403
    -0x1.ff55b974cde098738e59c07aa48dd110p-5,
    -0x1.fffffep-4,
    0x1.p1
  },
  { // Entry 404
    0x1.ff55b578c9f078a33719a85ef29e8a20p-5,
    0x1.fffffep-4,
    0x1.000002p1
  },
  { // Entry 405
    -0x1.ff55b578c9f078a33719a85ef29e8a20p-5,
    -0x1.fffffep-4,
    0x1.000002p1
  },
  { // Entry 406
    0x1.ff55bd70d1de9a7192507f037e5414bbp-5,
    0x1.p-3,
    0x1.fffffep0
  },
  { // Entry 407
    -0x1.ff55bd70d1de9a7192507f037e5414bbp-5,
    -0x1.p-3,
    0x1.fffffep0
  },
  { // Entry 408
    0x1.ff55bb72cfde9c6d964f25b81c5c1aa2p-5,
    0x1.p-3,
    0x1.p1
  },
  { // Entry 409
    -0x1.ff55bb72cfde9c6d964f25b81c5c1aa2p-5,
    -0x1.p-3,
    0x1.p1
  },
  { // Entry 410
    0x1.ff55b776cbea888962afd8276c01a25ep-5,
    0x1.p-3,
    0x1.000002p1
  },
  { // Entry 411
    -0x1.ff55b776cbea888962afd8276c01a25ep-5,
    -0x1.p-3,
    0x1.000002p1
  },
  { // Entry 412
    0x1.ff55c16cd5de8a91665b84abb95e0cc7p-5,
    0x1.000002p-3,
    0x1.fffffep0
  },
  { // Entry 413
    -0x1.ff55c16cd5de8a91665b84abb95e0cc7p-5,
    -0x1.000002p-3,
    0x1.fffffep0
  },
  { // Entry 414
    0x1.ff55bf6ed3da98798265cc3f27c896c7p-5,
    0x1.000002p-3,
    0x1.p1
  },
  { // Entry 415
    -0x1.ff55bf6ed3da98798265cc3f27c896c7p-5,
    -0x1.000002p-3,
    0x1.p1
  },
  { // Entry 416
    0x1.ff55bb72cfde9c6d964f25b81c5c1aa2p-5,
    0x1.000002p-3,
    0x1.000002p1
  },
  { // Entry 417
    -0x1.ff55bb72cfde9c6d964f25b81c5c1aa2p-5,
    -0x1.000002p-3,
    0x1.000002p1
  },
  { // Entry 418
    -0x1.ff55c16cd5de8a91665b84abb95e0cc7p-5,
    -0x1.000002p-3,
    0x1.fffffep0
  },
  { // Entry 419
    0x1.ff55c16cd5de8a91665b84abb95e0cc7p-5,
    0x1.000002p-3,
    0x1.fffffep0
  },
  { // Entry 420
    -0x1.ff55bf6ed3da98798265cc3f27c896c7p-5,
    -0x1.000002p-3,
    0x1.p1
  },
  { // Entry 421
    0x1.ff55bf6ed3da98798265cc3f27c896c7p-5,
    0x1.000002p-3,
    0x1.p1
  },
  { // Entry 422
    -0x1.ff55bb72cfde9c6d964f25b81c5c1aa2p-5,
    -0x1.000002p-3,
    0x1.000002p1
  },
  { // Entry 423
    0x1.ff55bb72cfde9c6d964f25b81c5c1aa2p-5,
    0x1.000002p-3,
    0x1.000002p1
  },
  { // Entry 424
    -0x1.ff55bd70d1de9a7192507f037e5414bbp-5,
    -0x1.p-3,
    0x1.fffffep0
  },
  { // Entry 425
    0x1.ff55bd70d1de9a7192507f037e5414bbp-5,
    0x1.p-3,
    0x1.fffffep0
  },
  { // Entry 426
    -0x1.ff55bb72cfde9c6d964f25b81c5c1aa2p-5,
    -0x1.p-3,
    0x1.p1
  },
  { // Entry 427
    0x1.ff55bb72cfde9c6d964f25b81c5c1aa2p-5,
    0x1.p-3,
    0x1.p1
  },
  { // Entry 428
    -0x1.ff55b776cbea888962afd8276c01a25ep-5,
    -0x1.p-3,
    0x1.000002p1
  },
  { // Entry 429
    0x1.ff55b776cbea888962afd8276c01a25ep-5,
    0x1.p-3,
    0x1.000002p1
  },
  { // Entry 430
    -0x1.ff55bb72cfde9c6d964f25b81c5c1aa2p-5,
    -0x1.fffffep-4,
    0x1.fffffep0
  },
  { // Entry 431
    0x1.ff55bb72cfde9c6d964f25b81c5c1aa2p-5,
    0x1.fffffep-4,
    0x1.fffffep0
  },
  { // Entry 432
    -0x1.ff55b974cde098738e59c07aa48dd110p-5,
    -0x1.fffffep-4,
    0x1.p1
  },
  { // Entry 433
    0x1.ff55b974cde098738e59c07aa48dd110p-5,
    0x1.fffffep-4,
    0x1.p1
  },
  { // Entry 434
    -0x1.ff55b578c9f078a33719a85ef29e8a20p-5,
    -0x1.fffffep-4,
    0x1.000002p1
  },
  { // Entry 435
    0x1.ff55b578c9f078a33719a85ef29e8a20p-5,
    0x1.fffffep-4,
    0x1.000002p1
  },
  { // Entry 436
    0x1.a271f5940186465d406645186f3ff94ap-2,
    0x1.bb67acp-1,
    0x1.fffffep0
  },
  { // Entry 437
    -0x1.a271f5940186465d406645186f3ff94ap-2,
    -0x1.bb67acp-1,
    0x1.fffffep0
  },
  { // Entry 438
    0x1.a271f41e9cbda3a2694ca4df76dc4a86p-2,
    0x1.bb67acp-1,
    0x1.p1
  },
  { // Entry 439
    -0x1.a271f41e9cbda3a2694ca4df76dc4a86p-2,
    -0x1.bb67acp-1,
    0x1.p1
  },
  { // Entry 440
    0x1.a271f133d333bccb9aba4067b1d551a2p-2,
    0x1.bb67acp-1,
    0x1.000002p1
  },
  { // Entry 441
    -0x1.a271f133d333bccb9aba4067b1d551a2p-2,
    -0x1.bb67acp-1,
    0x1.000002p1
  },
  { // Entry 442
    0x1.a271f74329f3af14ab02f72e14627e3ep-2,
    0x1.bb67aep-1,
    0x1.fffffep0
  },
  { // Entry 443
    -0x1.a271f74329f3af14ab02f72e14627e3ep-2,
    -0x1.bb67aep-1,
    0x1.fffffep0
  },
  { // Entry 444
    0x1.a271f5cdc529e5591cbe01f0dff5d436p-2,
    0x1.bb67aep-1,
    0x1.p1
  },
  { // Entry 445
    -0x1.a271f5cdc529e5591cbe01f0dff5d436p-2,
    -0x1.bb67aep-1,
    0x1.p1
  },
  { // Entry 446
    0x1.a271f2e2fb9db080e2f78cbaa7ae76dap-2,
    0x1.bb67aep-1,
    0x1.000002p1
  },
  { // Entry 447
    -0x1.a271f2e2fb9db080e2f78cbaa7ae76dap-2,
    -0x1.bb67aep-1,
    0x1.000002p1
  },
  { // Entry 448
    0x1.a271f8f252607a942b743a29251f41b3p-2,
    0x1.bb67b0p-1,
    0x1.fffffep0
  },
  { // Entry 449
    -0x1.a271f8f252607a942b743a29251f41b3p-2,
    -0x1.bb67b0p-1,
    0x1.fffffep0
  },
  { // Entry 450
    0x1.a271f77ced9589d7e7784be8c59b289ep-2,
    0x1.bb67b0p-1,
    0x1.p1
  },
  { // Entry 451
    -0x1.a271f77ced9589d7e7784be8c59b289ep-2,
    -0x1.bb67b0p-1,
    0x1.p1
  },
  { // Entry 452
    0x1.a271f492240706fe45667de97ee1051bp-2,
    0x1.bb67b0p-1,
    0x1.000002p1
  },
  { // Entry 453
    -0x1.a271f492240706fe45667de97ee1051bp-2,
    -0x1.bb67b0p-1,
    0x1.000002p1
  },
  { // Entry 454
    -0x1.a271f8f252607a942b743a29251f41b3p-2,
    -0x1.bb67b0p-1,
    0x1.fffffep0
  },
  { // Entry 455
    0x1.a271f8f252607a942b743a29251f41b3p-2,
    0x1.bb67b0p-1,
    0x1.fffffep0
  },
  { // Entry 456
    -0x1.a271f77ced9589d7e7784be8c59b289ep-2,
    -0x1.bb67b0p-1,
    0x1.p1
  },
  { // Entry 457
    0x1.a271f77ced9589d7e7784be8c59b289ep-2,
    0x1.bb67b0p-1,
    0x1.p1
  },
  { // Entry 458
    -0x1.a271f492240706fe45667de97ee1051bp-2,
    -0x1.bb67b0p-1,
    0x1.000002p1
  },
  { // Entry 459
    0x1.a271f492240706fe45667de97ee1051bp-2,
    0x1.bb67b0p-1,
    0x1.000002p1
  },
  { // Entry 460
    -0x1.a271f74329f3af14ab02f72e14627e3ep-2,
    -0x1.bb67aep-1,
    0x1.fffffep0
  },
  { // Entry 461
    0x1.a271f74329f3af14ab02f72e14627e3ep-2,
    0x1.bb67aep-1,
    0x1.fffffep0
  },
  { // Entry 462
    -0x1.a271f5cdc529e5591cbe01f0dff5d436p-2,
    -0x1.bb67aep-1,
    0x1.p1
  },
  { // Entry 463
    0x1.a271f5cdc529e5591cbe01f0dff5d436p-2,
    0x1.bb67aep-1,
    0x1.p1
  },
  { // Entry 464
    -0x1.a271f2e2fb9db080e2f78cbaa7ae76dap-2,
    -0x1.bb67aep-1,
    0x1.000002p1
  },
  { // Entry 465
    0x1.a271f2e2fb9db080e2f78cbaa7ae76dap-2,
    0x1.bb67aep-1,
    0x1.000002p1
  },
  { // Entry 466
    -0x1.a271f5940186465d406645186f3ff94ap-2,
    -0x1.bb67acp-1,
    0x1.fffffep0
  },
  { // Entry 467
    0x1.a271f5940186465d406645186f3ff94ap-2,
    0x1.bb67acp-1,
    0x1.fffffep0
  },
  { // Entry 468
    -0x1.a271f41e9cbda3a2694ca4df76dc4a86p-2,
    -0x1.bb67acp-1,
    0x1.p1
  },
  { // Entry 469
    0x1.a271f41e9cbda3a2694ca4df76dc4a86p-2,
    0x1.bb67acp-1,
    0x1.p1
  },
  { // Entry 470
    -0x1.a271f133d333bccb9aba4067b1d551a2p-2,
    -0x1.bb67acp-1,
    0x1.000002p1
  },
  { // Entry 471
    0x1.a271f133d333bccb9aba4067b1d551a2p-2,
    0x1.bb67acp-1,
    0x1.000002p1
  },
  { // Entry 472
    0x1.e1fc087d068cbfd25c64d0da9961de0dp-1,
    0x1.5f89e0p1,
    0x1.fffffep0
  },
  { // Entry 473
    -0x1.e1fc087d068cbfd25c64d0da9961de0dp-1,
    -0x1.5f89e0p1,
    0x1.fffffep0
  },
  { // Entry 474
    0x1.e1fc078961a1b8d58eff0ef3573e5ebbp-1,
    0x1.5f89e0p1,
    0x1.p1
  },
  { // Entry 475
    -0x1.e1fc078961a1b8d58eff0ef3573e5ebbp-1,
    -0x1.5f89e0p1,
    0x1.p1
  },
  { // Entry 476
    0x1.e1fc05a217cda574231fab7ef56a802ep-1,
    0x1.5f89e0p1,
    0x1.000002p1
  },
  { // Entry 477
    -0x1.e1fc05a217cda574231fab7ef56a802ep-1,
    -0x1.5f89e0p1,
    0x1.000002p1
  },
  { // Entry 478
    0x1.e1fc09dfe1de87be63d2efa76aedeef1p-1,
    0x1.5f89e2p1,
    0x1.fffffep0
  },
  { // Entry 479
    -0x1.e1fc09dfe1de87be63d2efa76aedeef1p-1,
    -0x1.5f89e2p1,
    0x1.fffffep0
  },
  { // Entry 480
    0x1.e1fc08ec3cf3edab43c88895dc8bd149p-1,
    0x1.5f89e2p1,
    0x1.p1
  },
  { // Entry 481
    -0x1.e1fc08ec3cf3edab43c88895dc8bd149p-1,
    -0x1.5f89e2p1,
    0x1.p1
  },
  { // Entry 482
    0x1.e1fc0704f320b41d2df91c8fc7a785a6p-1,
    0x1.5f89e2p1,
    0x1.000002p1
  },
  { // Entry 483
    -0x1.e1fc0704f320b41d2df91c8fc7a785a6p-1,
    -0x1.5f89e2p1,
    0x1.000002p1
  },
  { // Entry 484
    0x1.e1fc0b42bd2dac34dc336ebe1a5dd639p-1,
    0x1.5f89e4p1,
    0x1.fffffep0
  },
  { // Entry 485
    -0x1.e1fc0b42bd2dac34dc336ebe1a5dd639p-1,
    -0x1.5f89e4p1,
    0x1.fffffep0
  },
  { // Entry 486
    0x1.e1fc0a4f18437f0b6a89382bec836a03p-1,
    0x1.5f89e4p1,
    0x1.p1
  },
  { // Entry 487
    -0x1.e1fc0a4f18437f0b6a89382bec836a03p-1,
    -0x1.5f89e4p1,
    0x1.p1
  },
  { // Entry 488
    0x1.e1fc0867ce711f50acd36ef1986a9ed3p-1,
    0x1.5f89e4p1,
    0x1.000002p1
  },
  { // Entry 489
    -0x1.e1fc0867ce711f50acd36ef1986a9ed3p-1,
    -0x1.5f89e4p1,
    0x1.000002p1
  },
  { // Entry 490
    -0x1.e1fc0b42bd2dac34dc336ebe1a5dd639p-1,
    -0x1.5f89e4p1,
    0x1.fffffep0
  },
  { // Entry 491
    0x1.e1fc0b42bd2dac34dc336ebe1a5dd639p-1,
    0x1.5f89e4p1,
    0x1.fffffep0
  },
  { // Entry 492
    -0x1.e1fc0a4f18437f0b6a89382bec836a03p-1,
    -0x1.5f89e4p1,
    0x1.p1
  },
  { // Entry 493
    0x1.e1fc0a4f18437f0b6a89382bec836a03p-1,
    0x1.5f89e4p1,
    0x1.p1
  },
  { // Entry 494
    -0x1.e1fc0867ce711f50acd36ef1986a9ed3p-1,
    -0x1.5f89e4p1,
    0x1.000002p1
  },
  { // Entry 495
    0x1.e1fc0867ce711f50acd36ef1986a9ed3p-1,
    0x1.5f89e4p1,
    0x1.000002p1
  },
  { // Entry 496
    -0x1.e1fc09dfe1de87be63d2efa76aedeef1p-1,
    -0x1.5f89e2p1,
    0x1.fffffep0
  },
  { // Entry 497
    0x1.e1fc09dfe1de87be63d2efa76aedeef1p-1,
    0x1.5f89e2p1,
    0x1.fffffep0
  },
  { // Entry 498
    -0x1.e1fc08ec3cf3edab43c88895dc8bd149p-1,
    -0x1.5f89e2p1,
    0x1.p1
  },
  { // Entry 499
    0x1.e1fc08ec3cf3edab43c88895dc8bd149p-1,
    0x1.5f89e2p1,
    0x1.p1
  },
  { // Entry 500
    -0x1.e1fc0704f320b41d2df91c8fc7a785a6p-1,
    -0x1.5f89e2p1,
    0x1.000002p1
  },
  { // Entry 501
    0x1.e1fc0704f320b41d2df91c8fc7a785a6p-1,
    0x1.5f89e2p1,
    0x1.000002p1
  },
  { // Entry 502
    -0x1.e1fc087d068cbfd25c64d0da9961de0dp-1,
    -0x1.5f89e0p1,
    0x1.fffffep0
  },
  { // Entry 503
    0x1.e1fc087d068cbfd25c64d0da9961de0dp-1,
    0x1.5f89e0p1,
    0x1.fffffep0
  },
  { // Entry 504
    -0x1.e1fc078961a1b8d58eff0ef3573e5ebbp-1,
    -0x1.5f89e0p1,
    0x1.p1
  },
  { // Entry 505
    0x1.e1fc078961a1b8d58eff0ef3573e5ebbp-1,
    0x1.5f89e0p1,
    0x1.p1
  },
  { // Entry 506
    -0x1.e1fc05a217cda574231fab7ef56a802ep-1,
    -0x1.5f89e0p1,
    0x1.000002p1
  },
  { // Entry 507
    0x1.e1fc05a217cda574231fab7ef56a802ep-1,
    0x1.5f89e0p1,
    0x1.000002p1
  },
  { // Entry 508
    0x1.921fb54442d18469898cc51701b839a2p-1,
    0x1.fffffep0,
    0x1.fffffep0
  },
  { // Entry 509
    -0x1.921fb54442d18469898cc51701b839a2p-1,
    -0x1.fffffep0,
    0x1.fffffep0
  },
  { // Entry 510
    0x1.921fb44442d1046989621a6c570d8f04p-1,
    0x1.fffffep0,
    0x1.p1
  },
  { // Entry 511
    -0x1.921fb44442d1046989621a6c570d8f04p-1,
    -0x1.fffffep0,
    0x1.p1
  },
  { // Entry 512
    0x1.921fb24442d304698b0cc51401b839c8p-1,
    0x1.fffffep0,
    0x1.000002p1
  },
  { // Entry 513
    -0x1.921fb24442d304698b0cc51401b839c8p-1,
    -0x1.fffffep0,
    0x1.000002p1
  },
  { // Entry 514
    0x1.921fb64442d2046989b76fc1ac62e440p-1,
    0x1.p1,
    0x1.fffffep0
  },
  { // Entry 515
    -0x1.921fb64442d2046989b76fc1ac62e440p-1,
    -0x1.p1,
    0x1.fffffep0
  },
  { // Entry 516
    0x1.921fb54442d18469898cc51701b839a2p-1,
    0x1.p1,
    0x1.p1
  },
  { // Entry 517
    -0x1.921fb54442d18469898cc51701b839a2p-1,
    -0x1.p1,
    0x1.p1
  },
  { // Entry 518
    0x1.921fb34442d3846988376fc1ac62e5e6p-1,
    0x1.p1,
    0x1.000002p1
  },
  { // Entry 519
    -0x1.921fb34442d3846988376fc1ac62e5e6p-1,
    -0x1.p1,
    0x1.000002p1
  },
  { // Entry 520
    0x1.921fb84442d00469880cc51a01b8397bp-1,
    0x1.000002p1,
    0x1.fffffep0
  },
  { // Entry 521
    -0x1.921fb84442d00469880cc51a01b8397bp-1,
    -0x1.000002p1,
    0x1.fffffep0
  },
  { // Entry 522
    0x1.921fb74442cf84698ae21a6c570d8d5ep-1,
    0x1.000002p1,
    0x1.p1
  },
  { // Entry 523
    -0x1.921fb74442cf84698ae21a6c570d8d5ep-1,
    -0x1.000002p1,
    0x1.p1
  },
  { // Entry 524
    0x1.921fb54442d18469898cc51701b839a2p-1,
    0x1.000002p1,
    0x1.000002p1
  },
  { // Entry 525
    -0x1.921fb54442d18469898cc51701b839a2p-1,
    -0x1.000002p1,
    0x1.000002p1
  },
  { // Entry 526
    0x1.ffffffd555555bbbbbba972972d00dp-14,
    0x1.fffffep-13,
    0x1.fffffep0
  },
  { // Entry 527
    -0x1.ffffffd555555bbbbbba972972d00dp-14,
    -0x1.fffffep-13,
    0x1.fffffep0
  },
  { // Entry 528
    0x1.fffffdd55555dbbbbb1a9729e57ab751p-14,
    0x1.fffffep-13,
    0x1.p1
  },
  { // Entry 529
    -0x1.fffffdd55555dbbbbb1a9729e57ab751p-14,
    -0x1.fffffep-13,
    0x1.p1
  },
  { // Entry 530
    0x1.fffff9d55562dbbb9bda97790acf3db2p-14,
    0x1.fffffep-13,
    0x1.000002p1
  },
  { // Entry 531
    -0x1.fffff9d55562dbbb9bda97790acf3db2p-14,
    -0x1.fffffep-13,
    0x1.000002p1
  },
  { // Entry 532
    0x1.000000eaaaab6dddde6d4b951012b14cp-13,
    0x1.p-12,
    0x1.fffffep0
  },
  { // Entry 533
    -0x1.000000eaaaab6dddde6d4b951012b14cp-13,
    -0x1.p-12,
    0x1.fffffep0
  },
  { // Entry 534
    0x1.ffffffd555555bbbbbba972972d00dp-14,
    0x1.p-12,
    0x1.p1
  },
  { // Entry 535
    -0x1.ffffffd555555bbbbbba972972d00dp-14,
    -0x1.p-12,
    0x1.p1
  },
  { // Entry 536
    0x1.fffffbd5555e5bbba77a97585824f2d2p-14,
    0x1.p-12,
    0x1.000002p1
  },
  { // Entry 537
    -0x1.fffffbd5555e5bbba77a97585824f2d2p-14,
    -0x1.p-12,
    0x1.000002p1
  },
  { // Entry 538
    0x1.000002eaaaacedddde0d4b917d68009bp-13,
    0x1.000002p-12,
    0x1.fffffep0
  },
  { // Entry 539
    -0x1.000002eaaaacedddde0d4b917d68009bp-13,
    -0x1.000002p-12,
    0x1.fffffep0
  },
  { // Entry 540
    0x1.000001eaaaaa2ddddcfd4b9486bd5ca7p-13,
    0x1.000002p-12,
    0x1.p1
  },
  { // Entry 541
    -0x1.000001eaaaaa2ddddcfd4b9486bd5ca7p-13,
    -0x1.000002p-12,
    0x1.p1
  },
  { // Entry 542
    0x1.ffffffd555555bbbbbba972972d00dp-14,
    0x1.000002p-12,
    0x1.000002p1
  },
  { // Entry 543
    -0x1.ffffffd555555bbbbbba972972d00dp-14,
    -0x1.000002p-12,
    0x1.000002p1
  },
  { // Entry 544
    -0x1.2d97c7f3321d234f272993d1414a2b39p1,
    -0x1.p-149,
    -0x1.p-149
  },
  { // Entry 545
    0x1.2d97c7f3321d234f272993d1414a2b39p1,
    0x1.p-149,
    -0x1.p-149
  },
  { // Entry 546
    -0x1.921fb54442d18469898cc51701b839a2p0,
    -0x1.p-149,
    0.0
  },
  { // Entry 547
    0x1.921fb54442d18469898cc51701b839a2p0,
    0x1.p-149,
    0.0
  },
  { // Entry 548
    -0x1.921fb54442d18469898cc51701b839a2p-1,
    -0x1.p-149,
    0x1.p-149
  },
  { // Entry 549
    0x1.921fb54442d18469898cc51701b839a2p-1,
    0x1.p-149,
    0x1.p-149
  },
  { // Entry 550
    0x1.921fb54442d18469898cc51701b839a2p1,
    0.0,
    -0x1.p-149
  },
  { // Entry 551
    0.0,
    0.0,
    0.0
  },
  { // Entry 552
    0.0,
    0.0,
    0x1.p-149
  },
  { // Entry 553
    0x1.2d97c7f3321d234f272993d1414a2b39p1,
    0x1.p-149,
    -0x1.p-149
  },
  { // Entry 554
    -0x1.2d97c7f3321d234f272993d1414a2b39p1,
    -0x1.p-149,
    -0x1.p-149
  },
  { // Entry 555
    0x1.921fb54442d18469898cc51701b839a2p0,
    0x1.p-149,
    0.0
  },
  { // Entry 556
    -0x1.921fb54442d18469898cc51701b839a2p0,
    -0x1.p-149,
    0.0
  },
  { // Entry 557
    0x1.921fb54442d18469898cc51701b839a2p-1,
    0x1.p-149,
    0x1.p-149
  },
  { // Entry 558
    -0x1.921fb54442d18469898cc51701b839a2p-1,
    -0x1.p-149,
    0x1.p-149
  },
  { // Entry 559
    -0.0f,
    -0x1.p-149,
    0x1.fffffep127
  },
  { // Entry 560
    0.0f,
    0x1.p-149,
    0x1.fffffep127
  },
  { // Entry 561
    0.0,
    0.0,
    0x1.fffffep127
  },
  { // Entry 562
    0.0f,
    0x1.p-149,
    0x1.fffffep127
  },
  { // Entry 563
    -0.0f,
    -0x1.p-149,
    0x1.fffffep127
  },
  { // Entry 564
    -0x1.921fb54442d18469898cc51701b839a2p1,
    -0x1.p-149,
    -0x1.fffffep127
  },
  { // Entry 565
    0x1.921fb54442d18469898cc51701b839a2p1,
    0x1.p-149,
    -0x1.fffffep127
  },
  { // Entry 566
    0x1.921fb54442d18469898cc51701b839a2p1,
    0.0,
    -0x1.fffffep127
  },
  { // Entry 567
    0x1.921fb54442d18469898cc51701b839a2p1,
    0x1.p-149,
    -0x1.fffffep127
  },
  { // Entry 568
    -0x1.921fb54442d18469898cc51701b839a2p1,
    -0x1.p-149,
    -0x1.fffffep127
  },
  { // Entry 569
    0x1.921fb54442d18469898cc51701b839a2p0,
    0x1.fffffep127,
    -0x1.p-149
  },
  { // Entry 570
    -0x1.921fb54442d18469898cc51701b839a2p0,
    -0x1.fffffep127,
    -0x1.p-149
  },
  { // Entry 571
    0x1.921fb54442d18469898cc51701b839a2p0,
    0x1.fffffep127,
    0.0
  },
  { // Entry 572
    -0x1.921fb54442d18469898cc51701b839a2p0,
    -0x1.fffffep127,
    0.0
  },
  { // Entry 573
    0x1.921fb54442d18469898cc51701b839a2p0,
    0x1.fffffep127,
    0x1.p-149
  },
  { // Entry 574
    -0x1.921fb54442d18469898cc51701b839a2p0,
    -0x1.fffffep127,
    0x1.p-149
  },
  { // Entry 575
    -0x1.921fb54442d18469898cc51701b839a2p0,
    -0x1.fffffep127,
    -0x1.p-149
  },
  { // Entry 576
    0x1.921fb54442d18469898cc51701b839a2p0,
    0x1.fffffep127,
    -0x1.p-149
  },
  { // Entry 577
    -0x1.921fb54442d18469898cc51701b839a2p0,
    -0x1.fffffep127,
    0.0
  },
  { // Entry 578
    0x1.921fb54442d18469898cc51701b839a2p0,
    0x1.fffffep127,
    0.0
  },
  { // Entry 579
    -0x1.921fb54442d18469898cc51701b839a2p0,
    -0x1.fffffep127,
    0x1.p-149
  },
  { // Entry 580
    0x1.921fb54442d18469898cc51701b839a2p0,
    0x1.fffffep127,
    0x1.p-149
  },
  { // Entry 581
    0x1.921fb54442d18469898cc51701b839a2p-1,
    0x1.fffffep127,
    0x1.fffffep127
  },
  { // Entry 582
    -0x1.921fb54442d18469898cc51701b839a2p-1,
    -0x1.fffffep127,
    0x1.fffffep127
  },
  { // Entry 583
    -0x1.2d97c7f3321d234f272993d1414a2b39p1,
    -0x1.fffffep127,
    -0x1.fffffep127
  },
  { // Entry 584
    0x1.2d97c7f3321d234f272993d1414a2b39p1,
    0x1.fffffep127,
    -0x1.fffffep127
  },
  { // Entry 585
    0x1.ffffffffffffffffffffffffffff5555p-57,
    0x1.fffffep0,
    0x1.fffffep56
  },
  { // Entry 586
    -0x1.ffffffffffffffffffffffffffff5555p-57,
    -0x1.fffffep0,
    0x1.fffffep56
  },
  { // Entry 587
    0x1.fffffdffffffffffffffffffffff5555p-57,
    0x1.fffffep0,
    0x1.p57
  },
  { // Entry 588
    -0x1.fffffdffffffffffffffffffffff5555p-57,
    -0x1.fffffep0,
    0x1.p57
  },
  { // Entry 589
    0x1.fffffa00000bffffe800002ffffef555p-57,
    0x1.fffffep0,
    0x1.000002p57
  },
  { // Entry 590
    -0x1.fffffa00000bffffe800002ffffef555p-57,
    -0x1.fffffep0,
    0x1.000002p57
  },
  { // Entry 591
    0x1.000001000001000001000000ffffabaap-56,
    0x1.p1,
    0x1.fffffep56
  },
  { // Entry 592
    -0x1.000001000001000001000000ffffabaap-56,
    -0x1.p1,
    0x1.fffffep56
  },
  { // Entry 593
    0x1.ffffffffffffffffffffffffffff5555p-57,
    0x1.p1,
    0x1.p57
  },
  { // Entry 594
    -0x1.ffffffffffffffffffffffffffff5555p-57,
    -0x1.p1,
    0x1.p57
  },
  { // Entry 595
    0x1.fffffc000007fffff000001fffff1555p-57,
    0x1.p1,
    0x1.000002p57
  },
  { // Entry 596
    -0x1.fffffc000007fffff000001fffff1555p-57,
    -0x1.p1,
    0x1.000002p57
  },
  { // Entry 597
    0x1.000003000003000003000002ffffadaap-56,
    0x1.000002p1,
    0x1.fffffep56
  },
  { // Entry 598
    -0x1.000003000003000003000002ffffadaap-56,
    -0x1.000002p1,
    0x1.fffffep56
  },
  { // Entry 599
    0x1.000001ffffffffffffffffffffffaaaap-56,
    0x1.000002p1,
    0x1.p57
  },
  { // Entry 600
    -0x1.000001ffffffffffffffffffffffaaaap-56,
    -0x1.000002p1,
    0x1.p57
  },
  { // Entry 601
    0x1.ffffffffffffffffffffffffffff5555p-57,
    0x1.000002p1,
    0x1.000002p57
  },
  { // Entry 602
    -0x1.ffffffffffffffffffffffffffff5555p-57,
    -0x1.000002p1,
    0x1.000002p57
  },
  { // Entry 603
    -0x1.000003000003000003000002ffffadaap-56,
    -0x1.000002p1,
    0x1.fffffep56
  },
  { // Entry 604
    0x1.000003000003000003000002ffffadaap-56,
    0x1.000002p1,
    0x1.fffffep56
  },
  { // Entry 605
    -0x1.000001ffffffffffffffffffffffaaaap-56,
    -0x1.000002p1,
    0x1.p57
  },
  { // Entry 606
    0x1.000001ffffffffffffffffffffffaaaap-56,
    0x1.000002p1,
    0x1.p57
  },
  { // Entry 607
    -0x1.ffffffffffffffffffffffffffff5555p-57,
    -0x1.000002p1,
    0x1.000002p57
  },
  { // Entry 608
    0x1.ffffffffffffffffffffffffffff5555p-57,
    0x1.000002p1,
    0x1.000002p57
  },
  { // Entry 609
    -0x1.000001000001000001000000ffffabaap-56,
    -0x1.p1,
    0x1.fffffep56
  },
  { // Entry 610
    0x1.000001000001000001000000ffffabaap-56,
    0x1.p1,
    0x1.fffffep56
  },
  { // Entry 611
    -0x1.ffffffffffffffffffffffffffff5555p-57,
    -0x1.p1,
    0x1.p57
  },
  { // Entry 612
    0x1.ffffffffffffffffffffffffffff5555p-57,
    0x1.p1,
    0x1.p57
  },
  { // Entry 613
    -0x1.fffffc000007fffff000001fffff1555p-57,
    -0x1.p1,
    0x1.000002p57
  },
  { // Entry 614
    0x1.fffffc000007fffff000001fffff1555p-57,
    0x1.p1,
    0x1.000002p57
  },
  { // Entry 615
    -0x1.ffffffffffffffffffffffffffff5555p-57,
    -0x1.fffffep0,
    0x1.fffffep56
  },
  { // Entry 616
    0x1.ffffffffffffffffffffffffffff5555p-57,
    0x1.fffffep0,
    0x1.fffffep56
  },
  { // Entry 617
    -0x1.fffffdffffffffffffffffffffff5555p-57,
    -0x1.fffffep0,
    0x1.p57
  },
  { // Entry 618
    0x1.fffffdffffffffffffffffffffff5555p-57,
    0x1.fffffep0,
    0x1.p57
  },
  { // Entry 619
    -0x1.fffffa00000bffffe800002ffffef555p-57,
    -0x1.fffffep0,
    0x1.000002p57
  },
  { // Entry 620
    0x1.fffffa00000bffffe800002ffffef555p-57,
    0x1.fffffep0,
    0x1.000002p57
  },
  { // Entry 621
    0x1.fffffffffffffff555555555555555bbp-31,
    0x1.fffffep1,
    0x1.fffffep31
  },
  { // Entry 622
    -0x1.fffffffffffffff555555555555555bbp-31,
    -0x1.fffffep1,
    0x1.fffffep31
  },
  { // Entry 623
    0x1.fffffdfffffffff555557555553555bbp-31,
    0x1.fffffep1,
    0x1.p32
  },
  { // Entry 624
    -0x1.fffffdfffffffff555557555553555bbp-31,
    -0x1.fffffep1,
    0x1.p32
  },
  { // Entry 625
    0x1.fffffa00000bfff53d55b5855374f5c2p-31,
    0x1.fffffep1,
    0x1.000002p32
  },
  { // Entry 626
    -0x1.fffffa00000bfff53d55b5855374f5c2p-31,
    -0x1.fffffep1,
    0x1.000002p32
  },
  { // Entry 627
    0x1.000001000000fffaabaa9aabaa8aabddp-30,
    0x1.p2,
    0x1.fffffep31
  },
  { // Entry 628
    -0x1.000001000000fffaabaa9aabaa8aabddp-30,
    -0x1.p2,
    0x1.fffffep31
  },
  { // Entry 629
    0x1.fffffffffffffff555555555555555bbp-31,
    0x1.p2,
    0x1.p32
  },
  { // Entry 630
    -0x1.fffffffffffffff555555555555555bbp-31,
    -0x1.p2,
    0x1.p32
  },
  { // Entry 631
    0x1.fffffc000007fff545559575545515bfp-31,
    0x1.p2,
    0x1.000002p32
  },
  { // Entry 632
    -0x1.fffffc000007fff545559575545515bfp-31,
    -0x1.p2,
    0x1.000002p32
  },
  { // Entry 633
    0x1.000003000002fffaadaa7aada9eaaddbp-30,
    0x1.000002p2,
    0x1.fffffep31
  },
  { // Entry 634
    -0x1.000003000002fffaadaa7aada9eaaddbp-30,
    -0x1.000002p2,
    0x1.fffffep31
  },
  { // Entry 635
    0x1.000001fffffffffaaaaa8aaaaa6aaaddp-30,
    0x1.000002p2,
    0x1.p32
  },
  { // Entry 636
    -0x1.000001fffffffffaaaaa8aaaaa6aaaddp-30,
    -0x1.000002p2,
    0x1.p32
  },
  { // Entry 637
    0x1.fffffffffffffff555555555555555bbp-31,
    0x1.000002p2,
    0x1.000002p32
  },
  { // Entry 638
    -0x1.fffffffffffffff555555555555555bbp-31,
    -0x1.000002p2,
    0x1.000002p32
  },
  { // Entry 639
    -0x1.000003000002fffaadaa7aada9eaaddbp-30,
    -0x1.000002p-2,
    0x1.fffffep27
  },
  { // Entry 640
    0x1.000003000002fffaadaa7aada9eaaddbp-30,
    0x1.000002p-2,
    0x1.fffffep27
  },
  { // Entry 641
    -0x1.000001fffffffffaaaaa8aaaaa6aaaddp-30,
    -0x1.000002p-2,
    0x1.p28
  },
  { // Entry 642
    0x1.000001fffffffffaaaaa8aaaaa6aaaddp-30,
    0x1.000002p-2,
    0x1.p28
  },
  { // Entry 643
    -0x1.fffffffffffffff555555555555555bbp-31,
    -0x1.000002p-2,
    0x1.000002p28
  },
  { // Entry 644
    0x1.fffffffffffffff555555555555555bbp-31,
    0x1.000002p-2,
    0x1.000002p28
  },
  { // Entry 645
    -0x1.000001000000fffaabaa9aabaa8aabddp-30,
    -0x1.p-2,
    0x1.fffffep27
  },
  { // Entry 646
    0x1.000001000000fffaabaa9aabaa8aabddp-30,
    0x1.p-2,
    0x1.fffffep27
  },
  { // Entry 647
    -0x1.fffffffffffffff555555555555555bbp-31,
    -0x1.p-2,
    0x1.p28
  },
  { // Entry 648
    0x1.fffffffffffffff555555555555555bbp-31,
    0x1.p-2,
    0x1.p28
  },
  { // Entry 649
    -0x1.fffffc000007fff545559575545515bfp-31,
    -0x1.p-2,
    0x1.000002p28
  },
  { // Entry 650
    0x1.fffffc000007fff545559575545515bfp-31,
    0x1.p-2,
    0x1.000002p28
  },
  { // Entry 651
    -0x1.fffffffffffffff555555555555555bbp-31,
    -0x1.fffffep-3,
    0x1.fffffep27
  },
  { // Entry 652
    0x1.fffffffffffffff555555555555555bbp-31,
    0x1.fffffep-3,
    0x1.fffffep27
  },
  { // Entry 653
    -0x1.fffffdfffffffff555557555553555bbp-31,
    -0x1.fffffep-3,
    0x1.p28
  },
  { // Entry 654
    0x1.fffffdfffffffff555557555553555bbp-31,
    0x1.fffffep-3,
    0x1.p28
  },
  { // Entry 655
    -0x1.fffffa00000bfff53d55b5855374f5c2p-31,
    -0x1.fffffep-3,
    0x1.000002p28
  },
  { // Entry 656
    0x1.fffffa00000bfff53d55b5855374f5c2p-31,
    0x1.fffffep-3,
    0x1.000002p28
  },
  { // Entry 657
    0x1.921fb54442d18469898cc51701b839a2p-1,
    0x1.fffffep2,
    0x1.fffffep2
  },
  { // Entry 658
    -0x1.921fb54442d18469898cc51701b839a2p-1,
    -0x1.fffffep2,
    0x1.fffffep2
  },
  { // Entry 659
    0x1.921fb44442d1046989621a6c570d8f04p-1,
    0x1.fffffep2,
    0x1.p3
  },
  { // Entry 660
    -0x1.921fb44442d1046989621a6c570d8f04p-1,
    -0x1.fffffep2,
    0x1.p3
  },
  { // Entry 661
    0x1.921fb24442d304698b0cc51401b839c8p-1,
    0x1.fffffep2,
    0x1.000002p3
  },
  { // Entry 662
    -0x1.921fb24442d304698b0cc51401b839c8p-1,
    -0x1.fffffep2,
    0x1.000002p3
  },
  { // Entry 663
    0x1.921fb64442d2046989b76fc1ac62e440p-1,
    0x1.p3,
    0x1.fffffep2
  },
  { // Entry 664
    -0x1.921fb64442d2046989b76fc1ac62e440p-1,
    -0x1.p3,
    0x1.fffffep2
  },
  { // Entry 665
    0x1.921fb54442d18469898cc51701b839a2p-1,
    0x1.p3,
    0x1.p3
  },
  { // Entry 666
    -0x1.921fb54442d18469898cc51701b839a2p-1,
    -0x1.p3,
    0x1.p3
  },
  { // Entry 667
    0x1.921fb34442d3846988376fc1ac62e5e6p-1,
    0x1.p3,
    0x1.000002p3
  },
  { // Entry 668
    -0x1.921fb34442d3846988376fc1ac62e5e6p-1,
    -0x1.p3,
    0x1.000002p3
  },
  { // Entry 669
    0x1.921fb84442d00469880cc51a01b8397bp-1,
    0x1.000002p3,
    0x1.fffffep2
  },
  { // Entry 670
    -0x1.921fb84442d00469880cc51a01b8397bp-1,
    -0x1.000002p3,
    0x1.fffffep2
  },
  { // Entry 671
    0x1.921fb74442cf84698ae21a6c570d8d5ep-1,
    0x1.000002p3,
    0x1.p3
  },
  { // Entry 672
    -0x1.921fb74442cf84698ae21a6c570d8d5ep-1,
    -0x1.000002p3,
    0x1.p3
  },
  { // Entry 673
    0x1.921fb54442d18469898cc51701b839a2p-1,
    0x1.000002p3,
    0x1.000002p3
  },
  { // Entry 674
    -0x1.921fb54442d18469898cc51701b839a2p-1,
    -0x1.000002p3,
    0x1.000002p3
  },
  { // Entry 675
    -0x1.dac67522e883aedcc9c473438e936964p-2,
    -0x1.000002p3,
    0x1.fffffep3
  },
  { // Entry 676
    0x1.dac67522e883aedcc9c473438e936964p-2,
    0x1.000002p3,
    0x1.fffffep3
  },
  { // Entry 677
    -0x1.dac673894ee6e20ffe552cf613035e41p-2,
    -0x1.000002p3,
    0x1.p4
  },
  { // Entry 678
    0x1.dac673894ee6e20ffe552cf613035e41p-2,
    0x1.000002p3,
    0x1.p4
  },
  { // Entry 679
    -0x1.dac670561bb4f68adfc88bd978751a06p-2,
    -0x1.000002p3,
    0x1.000002p4
  },
  { // Entry 680
    0x1.dac670561bb4f68adfc88bd978751a06p-2,
    0x1.000002p3,
    0x1.000002p4
  },
  { // Entry 681
    -0x1.dac671efb54fd7d28ecd5330c89a3d73p-2,
    -0x1.p3,
    0x1.fffffep3
  },
  { // Entry 682
    0x1.dac671efb54fd7d28ecd5330c89a3d73p-2,
    0x1.p3,
    0x1.fffffep3
  },
  { // Entry 683
    -0x1.dac670561bb4f68adfc88bd978751a06p-2,
    -0x1.p3,
    0x1.p4
  },
  { // Entry 684
    0x1.dac670561bb4f68adfc88bd978751a06p-2,
    0x1.p3,
    0x1.p4
  },
  { // Entry 685
    -0x1.dac66d22e886e20ff6fe7a2378baf6f9p-2,
    -0x1.p3,
    0x1.000002p4
  },
  { // Entry 686
    0x1.dac66d22e886e20ff6fe7a2378baf6f9p-2,
    0x1.p3,
    0x1.000002p4
  },
  { // Entry 687
    -0x1.dac670561bb4f68adfc88bd978751a06p-2,
    -0x1.fffffep2,
    0x1.fffffep3
  },
  { // Entry 688
    0x1.dac670561bb4f68adfc88bd978751a06p-2,
    0x1.fffffep2,
    0x1.fffffep3
  },
  { // Entry 689
    -0x1.dac66ebc821b0b05c115b007ee262f78p-2,
    -0x1.fffffep2,
    0x1.p4
  },
  { // Entry 690
    0x1.dac66ebc821b0b05c115b007ee262f78p-2,
    0x1.fffffep2,
    0x1.p4
  },
  { // Entry 691
    -0x1.dac66b894eeee20ff7663e0a055c2460p-2,
    -0x1.fffffep2,
    0x1.000002p4
  },
  { // Entry 692
    0x1.dac66b894eeee20ff7663e0a055c2460p-2,
    0x1.fffffep2,
    0x1.000002p4
  },
  { // Entry 693
    0x1.72c43fa570aa5c9e564c7f0a5befa484p1,
    0x1.fffffep2,
    -0x1.000002p5
  },
  { // Entry 694
    -0x1.72c43fa570aa5c9e564c7f0a5befa484p1,
    -0x1.fffffep2,
    -0x1.000002p5
  },
  { // Entry 695
    0x1.72c43f69346ec6ea833e8c8f811d5b23p1,
    0x1.fffffep2,
    -0x1.p5
  },
  { // Entry 696
    -0x1.72c43f69346ec6ea833e8c8f811d5b23p1,
    -0x1.fffffep2,
    -0x1.p5
  },
  { // Entry 697
    0x1.72c43f4b1650a706dbd3d13842c3f6a5p1,
    0x1.fffffep2,
    -0x1.fffffep4
  },
  { // Entry 698
    -0x1.72c43f4b1650a706dbd3d13842c3f6a5p1,
    -0x1.fffffep2,
    -0x1.fffffep4
  },
  { // Entry 699
    0x1.72c43f87528c71e0c59cd3cd4eedc91cp1,
    0x1.p3,
    -0x1.000002p5
  },
  { // Entry 700
    -0x1.72c43f87528c71e0c59cd3cd4eedc91cp1,
    -0x1.p3,
    -0x1.000002p5
  },
  { // Entry 701
    0x1.72c43f4b1650a706dbd3d13842c3f6a5p1,
    0x1.p3,
    -0x1.p5
  },
  { // Entry 702
    -0x1.72c43f4b1650a706dbd3d13842c3f6a5p1,
    -0x1.p3,
    -0x1.p5
  },
  { // Entry 703
    0x1.72c43f2cf8326c9028ca86607b667a8cp1,
    0x1.p3,
    -0x1.fffffep4
  },
  { // Entry 704
    -0x1.72c43f2cf8326c9028ca86607b667a8cp1,
    -0x1.p3,
    -0x1.fffffep4
  },
  { // Entry 705
    0x1.72c43f4b1650a706dbd3d13842c3f6a5p1,
    0x1.000002p3,
    -0x1.000002p5
  },
  { // Entry 706
    -0x1.72c43f4b1650a706dbd3d13842c3f6a5p1,
    -0x1.000002p3,
    -0x1.000002p5
  },
  { // Entry 707
    0x1.72c43f0eda1471e0c4cf752a26ca10a6p1,
    0x1.000002p3,
    -0x1.p5
  },
  { // Entry 708
    -0x1.72c43f0eda1471e0c4cf752a26ca10a6p1,
    -0x1.000002p3,
    -0x1.p5
  },
  { // Entry 709
    0x1.72c43ef0bbf60243faa66eaf95b8eb8ep1,
    0x1.000002p3,
    -0x1.fffffep4
  },
  { // Entry 710
    -0x1.72c43ef0bbf60243faa66eaf95b8eb8ep1,
    -0x1.000002p3,
    -0x1.fffffep4
  },
  { // Entry 711
    0x1.fd5ba9aac2f6dc65912f313e7d111defp-4,
    0x1.fffffep2,
    0x1.fffffep5
  },
  { // Entry 712
    -0x1.fd5ba9aac2f6dc65912f313e7d111defp-4,
    -0x1.fffffep2,
    0x1.fffffep5
  },
  { // Entry 713
    0x1.fd5ba7b2a374dc8497123b64b398aae2p-4,
    0x1.fffffep2,
    0x1.p6
  },
  { // Entry 714
    -0x1.fd5ba7b2a374dc8497123b64b398aae2p-4,
    -0x1.fffffep2,
    0x1.p6
  },
  { // Entry 715
    0x1.fd5ba3c2647c7ef6c76d6d5ea97bab75p-4,
    0x1.fffffep2,
    0x1.000002p6
  },
  { // Entry 716
    -0x1.fd5ba3c2647c7ef6c76d6d5ea97bab75p-4,
    -0x1.fffffep2,
    0x1.000002p6
  },
  { // Entry 717
    0x1.fd5baba2e27ac4e31ede5c4d3485ebacp-4,
    0x1.p3,
    0x1.fffffep5
  },
  { // Entry 718
    -0x1.fd5baba2e27ac4e31ede5c4d3485ebacp-4,
    -0x1.p3,
    0x1.fffffep5
  },
  { // Entry 719
    0x1.fd5ba9aac2f6dc65912f313e7d111defp-4,
    0x1.p3,
    0x1.p6
  },
  { // Entry 720
    -0x1.fd5ba9aac2f6dc65912f313e7d111defp-4,
    -0x1.p3,
    0x1.p6
  },
  { // Entry 721
    0x1.fd5ba5ba83faad9ea550e6d54b02d0f9p-4,
    0x1.p3,
    0x1.000002p6
  },
  { // Entry 722
    -0x1.fd5ba5ba83faad9ea550e6d54b02d0f9p-4,
    -0x1.p3,
    0x1.000002p6
  },
  { // Entry 723
    0x1.fd5baf932182675568b9d1daf2fd1727p-4,
    0x1.000002p3,
    0x1.fffffep5
  },
  { // Entry 724
    -0x1.fd5baf932182675568b9d1daf2fd1727p-4,
    -0x1.000002p3,
    0x1.fffffep5
  },
  { // Entry 725
    0x1.fd5bad9b01faad9eb46ef9bda99d4fdap-4,
    0x1.000002p3,
    0x1.p6
  },
  { // Entry 726
    -0x1.fd5bad9b01faad9eb46ef9bda99d4fdap-4,
    -0x1.000002p3,
    0x1.p6
  },
  { // Entry 727
    0x1.fd5ba9aac2f6dc65912f313e7d111defp-4,
    0x1.000002p3,
    0x1.000002p6
  },
  { // Entry 728
    -0x1.fd5ba9aac2f6dc65912f313e7d111defp-4,
    -0x1.000002p3,
    0x1.000002p6
  },
  { // Entry 729
    -0x1.ff55c16cd5de8a91665b84abb95e0cc7p-5,
    -0x1.000002p3,
    0x1.fffffep6
  },
  { // Entry 730
    0x1.ff55c16cd5de8a91665b84abb95e0cc7p-5,
    0x1.000002p3,
    0x1.fffffep6
  },
  { // Entry 731
    -0x1.ff55bf6ed3da98798265cc3f27c896c7p-5,
    -0x1.000002p3,
    0x1.p7
  },
  { // Entry 732
    0x1.ff55bf6ed3da98798265cc3f27c896c7p-5,
    0x1.000002p3,
    0x1.p7
  },
  { // Entry 733
    -0x1.ff55bb72cfde9c6d964f25b81c5c1aa2p-5,
    -0x1.000002p3,
    0x1.000002p7
  },
  { // Entry 734
    0x1.ff55bb72cfde9c6d964f25b81c5c1aa2p-5,
    0x1.000002p3,
    0x1.000002p7
  },
  { // Entry 735
    -0x1.ff55bd70d1de9a7192507f037e5414bbp-5,
    -0x1.p3,
    0x1.fffffep6
  },
  { // Entry 736
    0x1.ff55bd70d1de9a7192507f037e5414bbp-5,
    0x1.p3,
    0x1.fffffep6
  },
  { // Entry 737
    -0x1.ff55bb72cfde9c6d964f25b81c5c1aa2p-5,
    -0x1.p3,
    0x1.p7
  },
  { // Entry 738
    0x1.ff55bb72cfde9c6d964f25b81c5c1aa2p-5,
    0x1.p3,
    0x1.p7
  },
  { // Entry 739
    -0x1.ff55b776cbea888962afd8276c01a25ep-5,
    -0x1.p3,
    0x1.000002p7
  },
  { // Entry 740
    0x1.ff55b776cbea888962afd8276c01a25ep-5,
    0x1.p3,
    0x1.000002p7
  },
  { // Entry 741
    -0x1.ff55bb72cfde9c6d964f25b81c5c1aa2p-5,
    -0x1.fffffep2,
    0x1.fffffep6
  },
  { // Entry 742
    0x1.ff55bb72cfde9c6d964f25b81c5c1aa2p-5,
    0x1.fffffep2,
    0x1.fffffep6
  },
  { // Entry 743
    -0x1.ff55b974cde098738e59c07aa48dd110p-5,
    -0x1.fffffep2,
    0x1.p7
  },
  { // Entry 744
    0x1.ff55b974cde098738e59c07aa48dd110p-5,
    0x1.fffffep2,
    0x1.p7
  },
  { // Entry 745
    -0x1.ff55b578c9f078a33719a85ef29e8a20p-5,
    -0x1.fffffep2,
    0x1.000002p7
  },
  { // Entry 746
    0x1.ff55b578c9f078a33719a85ef29e8a20p-5,
    0x1.fffffep2,
    0x1.000002p7
  },
  { // Entry 747
    0x1.0468a9467e7938105dfef0f3f2ae89fdp1,
    0x1.fffffep2,
    -0x1.000002p2
  },
  { // Entry 748
    -0x1.0468a9467e7938105dfef0f3f2ae89fdp1,
    -0x1.fffffep2,
    -0x1.000002p2
  },
  { // Entry 749
    0x1.0468a8e01812bd2f16a00cf199ef647fp1,
    0x1.fffffep2,
    -0x1.p2
  },
  { // Entry 750
    -0x1.0468a8e01812bd2f16a00cf199ef647fp1,
    -0x1.fffffep2,
    -0x1.p2
  },
  { // Entry 751
    0x1.0468a8ace4df610620bf7406afeac012p1,
    0x1.fffffep2,
    -0x1.fffffep1
  },
  { // Entry 752
    -0x1.0468a8ace4df610620bf7406afeac012p1,
    -0x1.fffffep2,
    -0x1.fffffep1
  },
  { // Entry 753
    0x1.0468a9134b459e76c491082a433c8899p1,
    0x1.p3,
    -0x1.000002p2
  },
  { // Entry 754
    -0x1.0468a9134b459e76c491082a433c8899p1,
    -0x1.p3,
    -0x1.000002p2
  },
  { // Entry 755
    0x1.0468a8ace4df610620bf7406afeac012p1,
    0x1.p3,
    -0x1.p2
  },
  { // Entry 756
    -0x1.0468a8ace4df610620bf7406afeac012p1,
    -0x1.p3,
    -0x1.p2
  },
  { // Entry 757
    0x1.0468a879b1ac23957ce9188c7ea0e2c0p1,
    0x1.p3,
    -0x1.fffffep1
  },
  { // Entry 758
    -0x1.0468a879b1ac23957ce9188c7ea0e2c0p1,
    -0x1.p3,
    -0x1.fffffep1
  },
  { // Entry 759
    0x1.0468a8ace4df610620bf7406afeac012p1,
    0x1.000002p3,
    -0x1.000002p2
  },
  { // Entry 760
    -0x1.0468a8ace4df610620bf7406afeac012p1,
    -0x1.000002p3,
    -0x1.000002p2
  },
  { // Entry 761
    0x1.0468a8467e799e76c3a631cfeff37bb0p1,
    0x1.000002p3,
    -0x1.p2
  },
  { // Entry 762
    -0x1.0468a8467e799e76c3a631cfeff37bb0p1,
    -0x1.000002p3,
    -0x1.p2
  },
  { // Entry 763
    0x1.0468a8134b469e76c3b32a4cc187a15dp1,
    0x1.000002p3,
    -0x1.fffffep1
  },
  { // Entry 764
    -0x1.0468a8134b469e76c3b32a4cc187a15dp1,
    -0x1.000002p3,
    -0x1.fffffep1
  },
  { // Entry 765
    0x1.ffffffffffffffffffffffffffffffffp-101,
    0x1.fffffep-103,
    0x1.fffffep-3
  },
  { // Entry 766
    -0x1.ffffffffffffffffffffffffffffffffp-101,
    -0x1.fffffep-103,
    0x1.fffffep-3
  },
  { // Entry 767
    0x1.fffffdffffffffffffffffffffffffffp-101,
    0x1.fffffep-103,
    0x1.p-2
  },
  { // Entry 768
    -0x1.fffffdffffffffffffffffffffffffffp-101,
    -0x1.fffffep-103,
    0x1.p-2
  },
  { // Entry 769
    0x1.fffffa00000bffffe800002fffffa0p-101,
    0x1.fffffep-103,
    0x1.000002p-2
  },
  { // Entry 770
    -0x1.fffffa00000bffffe800002fffffa0p-101,
    -0x1.fffffep-103,
    0x1.000002p-2
  },
  { // Entry 771
    0x1.000001000001000001000001000001p-100,
    0x1.p-102,
    0x1.fffffep-3
  },
  { // Entry 772
    -0x1.000001000001000001000001000001p-100,
    -0x1.p-102,
    0x1.fffffep-3
  },
  { // Entry 773
    0x1.ffffffffffffffffffffffffffffffffp-101,
    0x1.p-102,
    0x1.p-2
  },
  { // Entry 774
    -0x1.ffffffffffffffffffffffffffffffffp-101,
    -0x1.p-102,
    0x1.p-2
  },
  { // Entry 775
    0x1.fffffc000007fffff000001fffffc0p-101,
    0x1.p-102,
    0x1.000002p-2
  },
  { // Entry 776
    -0x1.fffffc000007fffff000001fffffc0p-101,
    -0x1.p-102,
    0x1.000002p-2
  },
  { // Entry 777
    0x1.000003000003000003000003000003p-100,
    0x1.000002p-102,
    0x1.fffffep-3
  },
  { // Entry 778
    -0x1.000003000003000003000003000003p-100,
    -0x1.000002p-102,
    0x1.fffffep-3
  },
  { // Entry 779
    0x1.000001ffffffffffffffffffffffffffp-100,
    0x1.000002p-102,
    0x1.p-2
  },
  { // Entry 780
    -0x1.000001ffffffffffffffffffffffffffp-100,
    -0x1.000002p-102,
    0x1.p-2
  },
  { // Entry 781
    0x1.ffffffffffffffffffffffffffffffffp-101,
    0x1.000002p-102,
    0x1.000002p-2
  },
  { // Entry 782
    -0x1.ffffffffffffffffffffffffffffffffp-101,
    -0x1.000002p-102,
    0x1.000002p-2
  },
  { // Entry 783
    0x1.ffffffffffffffffffffffffffffffffp-101,
    0x1.fffffcp2,
    0x1.fffffcp102
  },
  { // Entry 784
    -0x1.ffffffffffffffffffffffffffffffffp-101,
    -0x1.fffffcp2,
    0x1.fffffcp102
  },
  { // Entry 785
    0x1.fffffdfffffdfffffdfffffdfffffdffp-101,
    0x1.fffffcp2,
    0x1.fffffep102
  },
  { // Entry 786
    -0x1.fffffdfffffdfffffdfffffdfffffdffp-101,
    -0x1.fffffcp2,
    0x1.fffffep102
  },
  { // Entry 787
    0x1.fffffbffffffffffffffffffffffffffp-101,
    0x1.fffffcp2,
    0x1.p103
  },
  { // Entry 788
    -0x1.fffffbffffffffffffffffffffffffffp-101,
    -0x1.fffffcp2,
    0x1.p103
  },
  { // Entry 789
    0x1.fffff800000fffffe000003fffff80p-101,
    0x1.fffffcp2,
    0x1.000002p103
  },
  { // Entry 790
    -0x1.fffff800000fffffe000003fffff80p-101,
    -0x1.fffffcp2,
    0x1.000002p103
  },
  { // Entry 791
    0x1.fffff400002fffff400002fffff4p-101,
    0x1.fffffcp2,
    0x1.000004p103
  },
  { // Entry 792
    -0x1.fffff400002fffff400002fffff4p-101,
    -0x1.fffffcp2,
    0x1.000004p103
  },
  { // Entry 793
    0x1.000001000002000004000008000010p-100,
    0x1.fffffep2,
    0x1.fffffcp102
  },
  { // Entry 794
    -0x1.000001000002000004000008000010p-100,
    -0x1.fffffep2,
    0x1.fffffcp102
  },
  { // Entry 795
    0x1.ffffffffffffffffffffffffffffffffp-101,
    0x1.fffffep2,
    0x1.fffffep102
  },
  { // Entry 796
    -0x1.ffffffffffffffffffffffffffffffffp-101,
    -0x1.fffffep2,
    0x1.fffffep102
  },
  { // Entry 797
    0x1.fffffdffffffffffffffffffffffffffp-101,
    0x1.fffffep2,
    0x1.p103
  },
  { // Entry 798
    -0x1.fffffdffffffffffffffffffffffffffp-101,
    -0x1.fffffep2,
    0x1.p103
  },
  { // Entry 799
    0x1.fffffa00000bffffe800002fffffa0p-101,
    0x1.fffffep2,
    0x1.000002p103
  },
  { // Entry 800
    -0x1.fffffa00000bffffe800002fffffa0p-101,
    -0x1.fffffep2,
    0x1.000002p103
  },
  { // Entry 801
    0x1.fffff6000027ffff6000027ffff6p-101,
    0x1.fffffep2,
    0x1.000004p103
  },
  { // Entry 802
    -0x1.fffff6000027ffff6000027ffff6p-101,
    -0x1.fffffep2,
    0x1.000004p103
  },
  { // Entry 803
    0x1.000002000004000008000010000020p-100,
    0x1.p3,
    0x1.fffffcp102
  },
  { // Entry 804
    -0x1.000002000004000008000010000020p-100,
    -0x1.p3,
    0x1.fffffcp102
  },
  { // Entry 805
    0x1.000001000001000001000001000001p-100,
    0x1.p3,
    0x1.fffffep102
  },
  { // Entry 806
    -0x1.000001000001000001000001000001p-100,
    -0x1.p3,
    0x1.fffffep102
  },
  { // Entry 807
    0x1.ffffffffffffffffffffffffffffffffp-101,
    0x1.p3,
    0x1.p103
  },
  { // Entry 808
    -0x1.ffffffffffffffffffffffffffffffffp-101,
    -0x1.p3,
    0x1.p103
  },
  { // Entry 809
    0x1.fffffc000007fffff000001fffffc0p-101,
    0x1.p3,
    0x1.000002p103
  },
  { // Entry 810
    -0x1.fffffc000007fffff000001fffffc0p-101,
    -0x1.p3,
    0x1.000002p103
  },
  { // Entry 811
    0x1.fffff800001fffff800001fffff8p-101,
    0x1.p3,
    0x1.000004p103
  },
  { // Entry 812
    -0x1.fffff800001fffff800001fffff8p-101,
    -0x1.p3,
    0x1.000004p103
  },
  { // Entry 813
    0x1.000004000008000010000020000040p-100,
    0x1.000002p3,
    0x1.fffffcp102
  },
  { // Entry 814
    -0x1.000004000008000010000020000040p-100,
    -0x1.000002p3,
    0x1.fffffcp102
  },
  { // Entry 815
    0x1.000003000003000003000003000003p-100,
    0x1.000002p3,
    0x1.fffffep102
  },
  { // Entry 816
    -0x1.000003000003000003000003000003p-100,
    -0x1.000002p3,
    0x1.fffffep102
  },
  { // Entry 817
    0x1.000001ffffffffffffffffffffffffffp-100,
    0x1.000002p3,
    0x1.p103
  },
  { // Entry 818
    -0x1.000001ffffffffffffffffffffffffffp-100,
    -0x1.000002p3,
    0x1.p103
  },
  { // Entry 819
    0x1.ffffffffffffffffffffffffffffffffp-101,
    0x1.000002p3,
    0x1.000002p103
  },
  { // Entry 820
    -0x1.ffffffffffffffffffffffffffffffffp-101,
    -0x1.000002p3,
    0x1.000002p103
  },
  { // Entry 821
    0x1.fffffc00000fffffc00000fffffcp-101,
    0x1.000002p3,
    0x1.000004p103
  },
  { // Entry 822
    -0x1.fffffc00000fffffc00000fffffcp-101,
    -0x1.000002p3,
    0x1.000004p103
  },
  { // Entry 823
    0x1.00000600000c000018000030000060p-100,
    0x1.000004p3,
    0x1.fffffcp102
  },
  { // Entry 824
    -0x1.00000600000c000018000030000060p-100,
    -0x1.000004p3,
    0x1.fffffcp102
  },
  { // Entry 825
    0x1.000005000005000005000005000005p-100,
    0x1.000004p3,
    0x1.fffffep102
  },
  { // Entry 826
    -0x1.000005000005000005000005000005p-100,
    -0x1.000004p3,
    0x1.fffffep102
  },
  { // Entry 827
    0x1.000003ffffffffffffffffffffffffffp-100,
    0x1.000004p3,
    0x1.p103
  },
  { // Entry 828
    -0x1.000003ffffffffffffffffffffffffffp-100,
    -0x1.000004p3,
    0x1.p103
  },
  { // Entry 829
    0x1.000001fffffc000007fffff000001fffp-100,
    0x1.000004p3,
    0x1.000002p103
  },
  { // Entry 830
    -0x1.000001fffffc000007fffff000001fffp-100,
    -0x1.000004p3,
    0x1.000002p103
  },
  { // Entry 831
    0x1.ffffffffffffffffffffffffffffffffp-101,
    0x1.000004p3,
    0x1.000004p103
  },
  { // Entry 832
    -0x1.ffffffffffffffffffffffffffffffffp-101,
    -0x1.000004p3,
    0x1.000004p103
  },
  { // Entry 833
    0x1.921fb54442d18469898cc51701b839a2p1,
    0.0f,
    -0.0f
  },
  { // Entry 834
    -0x1.921fb54442d18469898cc51701b839a2p1,
    -0.0f,
    -0.0f
  },
  { // Entry 835
    0.0,
    0.0f,
    0.0f
  },
  { // Entry 836
    -0.0,
    -0.0f,
    0.0f
  },
  { // Entry 837
    0x1.921fb54442d18469898cc51701b839a2p1,
    0.0f,
    -0x1.p-149
  },
  { // Entry 838
    0x1.921fb54442d18469898cc51701b839a2p1,
    0.0f,
    -0x1.fffffcp-127
  },
  { // Entry 839
    0x1.921fb54442d18469898cc51701b839a2p1,
    0.0f,
    -0x1.p-126
  },
  { // Entry 840
    0x1.921fb54442d18469898cc51701b839a2p1,
    0.0f,
    -0x1.fffffep127
  },
  { // Entry 841
    0x1.921fb54442d18469898cc51701b839a2p1,
    0.0f,
    -HUGE_VALF
  },
  { // Entry 842
    -0x1.921fb54442d18469898cc51701b839a2p1,
    -0.0f,
    -0x1.p-149
  },
  { // Entry 843
    -0x1.921fb54442d18469898cc51701b839a2p1,
    -0.0f,
    -0x1.fffffcp-127
  },
  { // Entry 844
    -0x1.921fb54442d18469898cc51701b839a2p1,
    -0.0f,
    -0x1.p-126
  },
  { // Entry 845
    -0x1.921fb54442d18469898cc51701b839a2p1,
    -0.0f,
    -0x1.fffffep127
  },
  { // Entry 846
    -0x1.921fb54442d18469898cc51701b839a2p1,
    -0.0f,
    -HUGE_VALF
  },
  { // Entry 847
    0.0,
    0.0f,
    HUGE_VALF
  },
  { // Entry 848
    0.0,
    0.0f,
    0x1.fffffep127
  },
  { // Entry 849
    0.0,
    0.0f,
    0x1.p-126
  },
  { // Entry 850
    0.0,
    0.0f,
    0x1.fffffcp-127
  },
  { // Entry 851
    0.0,
    0.0f,
    0x1.p-149
  },
  { // Entry 852
    -0.0,
    -0.0f,
    HUGE_VALF
  },
  { // Entry 853
    -0.0,
    -0.0f,
    0x1.fffffep127
  },
  { // Entry 854
    -0.0,
    -0.0f,
    0x1.p-126
  },
  { // Entry 855
    -0.0,
    -0.0f,
    0x1.fffffcp-127
  },
  { // Entry 856
    -0.0,
    -0.0f,
    0x1.p-149
  },
  { // Entry 857
    -0x1.921fb54442d18469898cc51701b839a2p0,
    -HUGE_VALF,
    0.0f
  },
  { // Entry 858
    -0x1.921fb54442d18469898cc51701b839a2p0,
    -0x1.fffffep127,
    0.0f
  },
  { // Entry 859
    -0x1.921fb54442d18469898cc51701b839a2p0,
    -0x1.p-126,
    0.0f
  },
  { // Entry 860
    -0x1.921fb54442d18469898cc51701b839a2p0,
    -0x1.p-149,
    0.0f
  },
  { // Entry 861
    -0x1.921fb54442d18469898cc51701b839a2p0,
    -HUGE_VALF,
    -0.0f
  },
  { // Entry 862
    -0x1.921fb54442d18469898cc51701b839a2p0,
    -0x1.fffffep127,
    -0.0f
  },
  { // Entry 863
    -0x1.921fb54442d18469898cc51701b839a2p0,
    -0x1.p-126,
    -0.0f
  },
  { // Entry 864
    -0x1.921fb54442d18469898cc51701b839a2p0,
    -0x1.p-149,
    -0.0f
  },
  { // Entry 865
    0x1.921fb54442d18469898cc51701b839a2p0,
    HUGE_VALF,
    0.0f
  },
  { // Entry 866
    0x1.921fb54442d18469898cc51701b839a2p0,
    0x1.fffffep127,
    0.0f
  },
  { // Entry 867
    0x1.921fb54442d18469898cc51701b839a2p0,
    0x1.p-126,
    0.0f
  },
  { // Entry 868
    0x1.921fb54442d18469898cc51701b839a2p0,
    0x1.p-149,
    0.0f
  },
  { // Entry 869
    0x1.921fb54442d18469898cc51701b839a2p0,
    HUGE_VALF,
    -0.0f
  },
  { // Entry 870
    0x1.921fb54442d18469898cc51701b839a2p0,
    0x1.fffffep127,
    -0.0f
  },
  { // Entry 871
    0x1.921fb54442d18469898cc51701b839a2p0,
    0x1.p-126,
    -0.0f
  },
  { // Entry 872
    0x1.921fb54442d18469898cc51701b839a2p0,
    0x1.p-149,
    -0.0f
  },
  { // Entry 873
    0x1.921fb54442d18469898cc51701b839a2p1,
    0x1.fffffep127,
    -HUGE_VALF
  },
  { // Entry 874
    0x1.921fb54442d18469898cc51701b839a2p1,
    0x1.p-126,
    -HUGE_VALF
  },
  { // Entry 875
    0x1.921fb54442d18469898cc51701b839a2p1,
    0x1.p-149,
    -HUGE_VALF
  },
  { // Entry 876
    -0x1.921fb54442d18469898cc51701b839a2p1,
    -0x1.fffffep127,
    -HUGE_VALF
  },
  { // Entry 877
    -0x1.921fb54442d18469898cc51701b839a2p1,
    -0x1.p-126,
    -HUGE_VALF
  },
  { // Entry 878
    -0x1.921fb54442d18469898cc51701b839a2p1,
    -0x1.p-149,
    -HUGE_VALF
  },
  { // Entry 879
    0.0,
    0x1.fffffep127,
    HUGE_VALF
  },
  { // Entry 880
    0.0,
    0x1.p-126,
    HUGE_VALF
  },
  { // Entry 881
    0.0,
    0x1.p-149,
    HUGE_VALF
  },
  { // Entry 882
    -0.0,
    -0x1.fffffep127,
    HUGE_VALF
  },
  { // Entry 883
    -0.0,
    -0x1.p-126,
    HUGE_VALF
  },
  { // Entry 884
    -0.0,
    -0x1.p-149,
    HUGE_VALF
  },
  { // Entry 885
    0x1.921fb54442d18469898cc51701b839a2p0,
    HUGE_VALF,
    0x1.fffffep127
  },
  { // Entry 886
    0x1.921fb54442d18469898cc51701b839a2p0,
    HUGE_VALF,
    0x1.p-126
  },
  { // Entry 887
    0x1.921fb54442d18469898cc51701b839a2p0,
    HUGE_VALF,
    0x1.p-149
  },
  { // Entry 888
    0x1.921fb54442d18469898cc51701b839a2p0,
    HUGE_VALF,
    -0x1.fffffep127
  },
  { // Entry 889
    0x1.921fb54442d18469898cc51701b839a2p0,
    HUGE_VALF,
    -0x1.p-126
  },
  { // Entry 890
    0x1.921fb54442d18469898cc51701b839a2p0,
    HUGE_VALF,
    -0x1.p-149
  },
  { // Entry 891
    -0x1.921fb54442d18469898cc51701b839a2p0,
    -HUGE_VALF,
    0x1.fffffep127
  },
  { // Entry 892
    -0x1.921fb54442d18469898cc51701b839a2p0,
    -HUGE_VALF,
    0x1.p-126
  },
  { // Entry 893
    -0x1.921fb54442d18469898cc51701b839a2p0,
    -HUGE_VALF,
    0x1.p-149
  },
  { // Entry 894
    -0x1.921fb54442d18469898cc51701b839a2p0,
    -HUGE_VALF,
    -0x1.fffffep127
  },
  { // Entry 895
    -0x1.921fb54442d18469898cc51701b839a2p0,
    -HUGE_VALF,
    -0x1.p-126
  },
  { // Entry 896
    -0x1.921fb54442d18469898cc51701b839a2p0,
    -HUGE_VALF,
    -0x1.p-149
  },
  { // Entry 897
    0x1.2d97c7f3321d234f272993d1414a2b39p1,
    HUGE_VALF,
    -HUGE_VALF
  },
  { // Entry 898
    -0x1.2d97c7f3321d234f272993d1414a2b39p1,
    -HUGE_VALF,
    -HUGE_VALF
  },
  { // Entry 899
    0x1.921fb54442d18469898cc51701b839a2p-1,
    HUGE_VALF,
    HUGE_VALF
  },
  { // Entry 900
    -0x1.921fb54442d18469898cc51701b839a2p-1,
    -HUGE_VALF,
    HUGE_VALF
  },
  { // Entry 901
    0x1.921fb54442d18469898cc51701b839a2p-1,
    0x1.fffffep127,
    0x1.fffffep127
  },
  { // Entry 902
    0x1.921fb54442d18469898cc51701b839a2p0,
    0x1.fffffep127,
    0x1.p-126
  },
  { // Entry 903
    0x1.921fb54442d18469898cc51701b839a2p0,
    0x1.fffffep127,
    0x1.p-149
  },
  { // Entry 904
    0x1.921fb54442d18469898cc51701b839a2p0,
    0x1.fffffep127,
    -0x1.p-149
  },
  { // Entry 905
    0x1.921fb54442d18469898cc51701b839a2p0,
    0x1.fffffep127,
    -0x1.p-126
  },
  { // Entry 906
    0x1.2d97c7f3321d234f272993d1414a2b39p1,
    0x1.fffffep127,
    -0x1.fffffep127
  },
  { // Entry 907
    0.0f,
    0x1.p-126,
    0x1.fffffep127
  },
  { // Entry 908
    0.0f,
    0x1.p-149,
    0x1.fffffep127
  },
  { // Entry 909
    -0.0f,
    -0x1.p-149,
    0x1.fffffep127
  },
  { // Entry 910
    -0.0f,
    -0x1.p-126,
    0x1.fffffep127
  },
  { // Entry 911
    -0x1.921fb54442d18469898cc51701b839a2p-1,
    -0x1.fffffep127,
    0x1.fffffep127
  },
  { // Entry 912
    0x1.921fb54442d18469898cc51701b839a2p-1,
    0x1.p-126,
    0x1.p-126
  },
  { // Entry 913
    0x1.921fb34442d184698c376fc1ac62dde6p0,
    0x1.p-126,
    0x1.p-149
  },
  { // Entry 914
    0x1.921fb74442d1846986e21a6c570d955ep0,
    0x1.p-126,
    -0x1.p-149
  },
  { // Entry 915
    0x1.2d97c7f3321d234f272993d1414a2b39p1,
    0x1.p-126,
    -0x1.p-126
  },
  { // Entry 916
    0x1.921fb54442d18469898cc51701b839a2p1,
    0x1.p-126,
    -0x1.fffffep127
  },
  { // Entry 917
    0x1.fffffffffffd55555555555bbbbbbbbbp-24,
    0x1.p-149,
    0x1.p-126
  },
  { // Entry 918
    -0x1.fffffffffffd55555555555bbbbbbbbbp-24,
    -0x1.p-149,
    0x1.p-126
  },
  { // Entry 919
    -0x1.921fb54442d18469898cc51701b839a2p-1,
    -0x1.p-126,
    0x1.p-126
  },
  { // Entry 920
    -0x1.921fb54442d18469898cc51701b839a2p0,
    -0x1.fffffep127,
    0x1.p-126
  },
  { // Entry 921
    0x1.921fb54442d18469898cc51701b839a2p-1,
    0x1.p-149,
    0x1.p-149
  },
  { // Entry 922
    0x1.2d97c7f3321d234f272993d1414a2b39p1,
    0x1.p-149,
    -0x1.p-149
  },
  { // Entry 923
    0x1.921fb44442d184698ae21a6c570d8bc4p1,
    0x1.p-149,
    -0x1.p-126
  },
  { // Entry 924
    0x1.921fb54442d18469898cc51701b839a2p1,
    0x1.p-149,
    -0x1.fffffep127
  },
  { // Entry 925
    -0x1.921fb54442d18469898cc51701b839a2p-1,
    -0x1.p-149,
    0x1.p-149
  },
  { // Entry 926
    -0x1.921fb34442d184698c376fc1ac62dde6p0,
    -0x1.p-126,
    0x1.p-149
  },
  { // Entry 927
    -0x1.921fb54442d18469898cc51701b839a2p0,
    -0x1.fffffep127,
    0x1.p-149
  },
  { // Entry 928
    -0x1.2d97c7f3321d234f272993d1414a2b39p1,
    -0x1.p-149,
    -0x1.p-149
  },
  { // Entry 929
    -0x1.921fb44442d184698ae21a6c570d8bc4p1,
    -0x1.p-149,
    -0x1.p-126
  },
  { // Entry 930
    -0x1.921fb54442d18469898cc51701b839a2p1,
    -0x1.p-149,
    -0x1.fffffep127
  },
  { // Entry 931
    -0x1.921fb74442d1846986e21a6c570d955ep0,
    -0x1.p-126,
    -0x1.p-149
  },
  { // Entry 932
    -0x1.921fb54442d18469898cc51701b839a2p0,
    -0x1.fffffep127,
    -0x1.p-149
  },
  { // Entry 933
    -0x1.2d97c7f3321d234f272993d1414a2b39p1,
    -0x1.p-126,
    -0x1.p-126
  },
  { // Entry 934
    -0x1.921fb54442d18469898cc51701b839a2p1,
    -0x1.p-126,
    -0x1.fffffep127
  },
  { // Entry 935
    -0x1.921fb54442d18469898cc51701b839a2p0,
    -0x1.fffffep127,
    -0x1.p-126
  },
  { // Entry 936
    -0x1.2d97c7f3321d234f272993d1414a2b39p1,
    -0x1.fffffep127,
    -0x1.fffffep127
  }
};
