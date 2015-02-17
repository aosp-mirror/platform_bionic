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

static data_1_2_t<double, double, double> g_hypot_intel_data[] = {
  { // Entry 0
    0x1.74334f2872bf324a8b6c0ffaf2f4ee3dp0,
    -0x1.0b2502b3f7656p0,
    0x1.032a74c8e2bbdp0
  },
  { // Entry 1
    0x1.21c9123e6cbbf812953910371e275dc7p3,
    -0x1.21c9107b0e488p3,
    0x1.ff77fffffffffp-9
  },
  { // Entry 2
    0x1.ad09d0c85a9b738fbeb590492c45108fp-21,
    -0x1.3db5af72d9074p-21,
    0x1.2054976e47184p-21
  },
  { // Entry 3
    0x1.6a1f7b584e052800e5d5eb2c842defa6p-1,
    -0x1.47200db32f88cp-1,
    -0x1.36a049ab1eee0p-2
  },
  { // Entry 4
    0x1.893eff10a04aed61358d3d5b6481eebcp-425,
    -0x1.5a9c9453941a0p-426,
    -0x1.60ff7b7326510p-425
  },
  { // Entry 5
    0x1.94c583ada5b5218962e6ed1568fead12p0,
    -0x1.7ffffffffffffp0,
    0x1.0000000000003p-1
  },
  { // Entry 6
    0x1.97cfe6e25cd448cf5dbcb52213679796p-11,
    -0x1.8e38e38e38e37p-11,
    -0x1.5fad40a57eb38p-13
  },
  { // Entry 7
    0x1.9e661829e5ee17ffba1d22ecf0580873p421,
    -0x1.9897fbb0fa747p418,
    0x1.9b3d45740c34cp421
  },
  { // Entry 8
    0x1.c7653d4e9e6c77fe2eb3fc6720505db6p-11,
    -0x1.bbbbbbbbbbbbcp-11,
    -0x1.9999999999c33p-13
  },
  { // Entry 9
    0x1.ddffe6e5a3a8384016ed35f115bc095ep-11,
    -0x1.e9131abf0b717p-14,
    0x1.da12f684bda24p-11
  },
  { // Entry 10
    0x1.7158b50ca33488012d796eb6f1a7589bp0,
    -0x1.f5723be0cafb4p-1,
    0x1.0f35b6d1e4e0fp0
  },
  { // Entry 11
    0x1.00007fffdffff7ffe2000dfff64007afp0,
    -0x1.ffffffffffffdp-1,
    0x1.ffffffffffffcp-9
  },
  { // Entry 12
    0x1.fffffffep-1043,
    0.0,
    0x1.fffffffe0p-1043
  },
  { // Entry 13
    0x1.199999999999a0p0,
    0x1.0p-1074,
    -0x1.199999999999ap0
  },
  { // Entry 14
    0x1.aaaaaaaaaaaaa0p0,
    0x1.0p-1074,
    -0x1.aaaaaaaaaaaaap0
  },
  { // Entry 15
    0x1.b87065d24cee52b080d32543ca9cfc19p-1,
    0x1.0000000000001p-1,
    -0x1.6666666666668p-1
  },
  { // Entry 16
    0x1.43596ffaa74788558d1fbef5bc6654e5p0,
    0x1.0000000000001p-2,
    -0x1.3cf3cf3cf3cf4p0
  },
  { // Entry 17
    0x1.4ccccccccccd08000000000000627627p-2,
    0x1.0000000000001p-3,
    -0x1.3333333333337p-2
  },
  { // Entry 18
    0x1.801554bda99c72d8de8e8d0810523d56p0,
    0x1.0000000000001p-5,
    0x1.8000000000001p0
  },
  { // Entry 19
    0x1.74b50ce2454308015045eece9494acfbp-3,
    0x1.0000000000001p-7,
    -0x1.745d1745d0e18p-3
  },
  { // Entry 20
    0x1.28ff91ab72d727facf9be8fbd129e05ep-2,
    0x1.0000000000080p-3,
    0x1.0c0p-2
  },
  { // Entry 21
    0x1.000033d5ab09e8017b9fe870280d1247p9,
    0x1.0000000000aeep9,
    0x1.45d1745d1745ep0
  },
  { // Entry 22
    0x1.07e0f670c16e48e1e7c24e5939e31f55p-3,
    0x1.00000009880p-3,
    0x1.ffffff8cfffffp-6
  },
  { // Entry 23
    0x1.b596b5878e25800001094dfd216cf693p-1,
    0x1.00000040ed435p-1,
    0x1.62e42fefa39efp-1
  },
  { // Entry 24
    0x1.0008ffd80967981d0efdf34de42be658p-4,
    0x1.0000007ffffdep-10,
    0x1.0000fffffffffp-4
  },
  { // Entry 25
    0x1.6a09e8e19116080994af0efbde159838p-20,
    0x1.0000037ffffdfp-20,
    0x1.00000000110p-20
  },
  { // Entry 26
    0x1.00009ec452a8c81c490f9ba38768ce7cp-3,
    0x1.0000043ffffdfp-3,
    0x1.19453808ca296p-11
  },
  { // Entry 27
    0x1.b879b2c3faae37fe5d8254c1a9443fd6p-1,
    0x1.000fffffff6c8p-1,
    -0x1.6666666666668p-1
  },
  { // Entry 28
    0x1.6b9824a5d9fefc6fac3c06f2beba6d16p0,
    0x1.001p0,
    0x1.0222222222223p0
  },
  { // Entry 29
    0x1.6b1dc233c08aacf04d42b3d293e12a49p0,
    0x1.0016f5e74bfddp0,
    -0x1.016eb68415ab1p0
  },
  { // Entry 30
    0x1.778d27690518dd41bd73ad488f2a2174p-27,
    0x1.00a436e9442ddp-27,
    0x1.122dc42e12491p-27
  },
  { // Entry 31
    0x1.6c9ed56d3e093800300f2c229b359a3dp0,
    0x1.01b59372d3dp0,
    -0x1.01f11caa0d8fap0
  },
  { // Entry 32
    0x1.62e44823f6c828019d99f2ea6e42b44dp-1,
    0x1.0624dd41fac87p-10,
    0x1.62e42fefa39efp-1
  },
  { // Entry 33
    0x1.62e44823f6c9980000fc0f85b3c55a79p-1,
    0x1.0624dd49c38c9p-10,
    0x1.62e42fefa39efp-1
  },
  { // Entry 34
    0x1.086b948a12d8c800cf1808a10a5174d9p3,
    0x1.086ac9804c16fp3,
    0x1.47ae147ae1488p-5
  },
  { // Entry 35
    0x1.74334f2872bf324a8b6c0ffaf2f4ee3dp0,
    0x1.0b2502b3f7656p0,
    -0x1.032a74c8e2bbdp0
  },
  { // Entry 36
    0x1.b174e26559df6801e67982110c79e921p0,
    0x1.0dadec75407d1p0,
    0x1.53594d6535950p0
  },
  { // Entry 37
    0x1.0fa6ab587be3f81316d103dd56845189p2,
    0x1.0dc27b7edad61p2,
    -0x1.fffffffffffdfp-2
  },
  { // Entry 38
    0x1.0e00000001e77800795b3317cdb8cf48p-1,
    0x1.0e0p-1,
    0x1.00880p-20
  },
  { // Entry 39
    0x1.1e643a24dde918108702a958a34659bdp1,
    0x1.17261d1fbe70fp1,
    -0x1.0p-1
  },
  { // Entry 40
    0x1.00009ec452a8c81c490f9ba38768ce7cp-3,
    0x1.19453808ca296p-11,
    0x1.0000043ffffdfp-3
  },
  { // Entry 41
    0x1.1f7648cb9c2928102f301b4e2a6da7f8p3,
    0x1.1f6fb7dbedf31p3,
    -0x1.eb851eb851eb2p-4
  },
  { // Entry 42
    0x1.3fc168b1ba65f7fefcba8c51c9dceebep1,
    0x1.333333334955dp1,
    0x1.62e42fefa39efp-1
  },
  { // Entry 43
    0x1.d561dc6bbc69b7fffefd4eef36bb45cep-1,
    0x1.33333336ffb33p-1,
    0x1.62e42fefa39efp-1
  },
  { // Entry 44
    0x1.b6d63492d208b7fe66769600852b12d8p7,
    0x1.3845636425767p7,
    0x1.34534564561d4p7
  },
  { // Entry 45
    0x1.b6d63492cf6ddfff7a4bf179a9f2d6cap7,
    0x1.3845636425776p7,
    0x1.3453456452673p7
  },
  { // Entry 46
    0x1.853a0d5122cef456b05a1510fbead643p6,
    0x1.3bbd9e1fa27b4p6,
    0x1.c7372b6a514bcp5
  },
  { // Entry 47
    0x1.3fba0ae4ce08b810e8f56ddaf12a7f4fp3,
    0x1.3e1f0f87c3dd1p3,
    -0x1.fffffffffffdfp-1
  },
  { // Entry 48
    0x1.b71be4215a53283d71f5b110a870e894p-11,
    0x1.484e2afe0bbc6p-13,
    -0x1.af5ebd7af5ec0p-11
  },
  { // Entry 49
    0x1.56d07f9feb80d804781ae4305058b676p2,
    0x1.550fe1779c5p2,
    -0x1.14f2805f85d24p-1
  },
  { // Entry 50
    0x1.a52df5c24c89489d50528533a7f35763p2,
    0x1.5555555555556p0,
    0x1.9c71c71c71c69p2
  },
  { // Entry 51
    0x1.b993cc4482b447ff4f74030e8ba14870p-1,
    0x1.57354071c6426p-3,
    -0x1.b1293f6f53880p-1
  },
  { // Entry 52
    0x1.a7e2abc57f0e380a70c24d675241f120p0,
    0x1.5b2d96cb65bp0,
    -0x1.e666666666664p-1
  },
  { // Entry 53
    0x1.e44d26303c8e703260adac35beb0201ap421,
    0x1.600ec23b7b61ep421,
    -0x1.4c92148cef14ap421
  },
  { // Entry 54
    0x1.f8611701969ccfffff045c3f99fe48f7p-1,
    0x1.6666666dac2fap-1,
    0x1.62e42fefa39efp-1
  },
  { // Entry 55
    0x1.6cc93c4d65368802345842af2282a5eap1,
    0x1.6cc93a754133ep1,
    0x1.257430139p-10
  },
  { // Entry 56
    0x1.6cc93c4d653688025c9147b5b60441e9p1,
    0x1.6cc93a754133ep1,
    0x1.25743013900c8p-10
  },
  { // Entry 57
    0x1.6cc93c4d653688025e2d2930daa313d5p1,
    0x1.6cc93a754133ep1,
    0x1.25743013900d0p-10
  },
  { // Entry 58
    0x1.d488ac97053f37fbba277d07ac43cad5p-20,
    0x1.7p-20,
    0x1.220p-20
  },
  { // Entry 59
    0x1.400000004cccc800052f1bc6a6c17e88p-1,
    0x1.7ffffffffffffp-2,
    0x1.000000006p-1
  },
  { // Entry 60
    0x1.ffee8df9517ff7fe75600bb975e5ce61p0,
    0x1.81792910a5db1p-1,
    -0x1.da43b5dce0b18p0
  },
  { // Entry 61
    0x1.9b0a5736513fc7ffab037ae75d04e99ap2,
    0x1.88a4522914881p2,
    0x1.e666666666667p0
  },
  { // Entry 62
    0x1.a5fa08a755b5c900f2d5cc6751e1ecf9p2,
    0x1.88cb3c9484e2ap0,
    0x1.9a6449e59bb5dp2
  },
  { // Entry 63
    0x1.8befefed027e87ff6c70308e205c2a19p6,
    0x1.8beea4e1a0873p6,
    -0x1.0p-1
  },
  { // Entry 64
    0x1.96991a72bfd0100000868ffe3e831279p1,
    0x1.8cccccce3bcbdp1,
    0x1.62e42fefa39efp-1
  },
  { // Entry 65
    0x1.bf7cd9d02c7e220c699cc834fdd4fb41p-4,
    0x1.8ddf4152067fcp-4,
    -0x1.999999999999ap-5
  },
  { // Entry 66
    0x1.76e7ba8bc745280094a71daf10d4a68ep25,
    0x1.97fe3896c80f0p2,
    0x1.76e7ba8bc741bp25
  },
  { // Entry 67
    0x1.0efacef2e4e81ffffefe587f1ae783b6p0,
    0x1.999999a0c0a0bp-1,
    0x1.62e42fefa39efp-1
  },
  { // Entry 68
    0x1.715e867859a8580001048a1e9e9dff7cp-1,
    0x1.999999b9ce793p-3,
    0x1.62e42fefa39efp-1
  },
  { // Entry 69
    0x1.6690cd7c39fa4800010745dc1f901919p-1,
    0x1.999999da8ed7ap-4,
    0x1.62e42fefa39efp-1
  },
  { // Entry 70
    0x1.f65294baeb7788330decefa598e273d5p-11,
    0x1.9dbcc48676f94p-15,
    0x1.f5a814afd6a11p-11
  },
  { // Entry 71
    0x1.c26cb730864d698c82db5769586bd519p0,
    0x1.a2882f7660c18p-2,
    0x1.b61a64501888ap0
  },
  { // Entry 72
    0x1.209f4f2c5979e816bf99efe18e6f1cdap1,
    0x1.b8f22f033c872p-3,
    0x1.1f4db533bcddcp1
  },
  { // Entry 73
    0x1.50225479d4b157fe785588557cc66cdep-10,
    0x1.bd8caaaf99090p-11,
    -0x1.f76b23986ff44p-11
  },
  { // Entry 74
    0x1.060db00245bf781048c529e4efff0afbp25,
    0x1.bffffffffffffp22,
    0x1.0000027ffffdfp25
  },
  { // Entry 75
    0x1.c8c25b45aba168f0187bb5c3abbc3d16p-11,
    0x1.c06b09e919d94p-11,
    -0x1.5b911048a3310p-13
  },
  { // Entry 76
    0x1.f53b21b5c40249b92a9c223bae43323bp0,
    0x1.c81e6f7fe3993p-2,
    -0x1.e8167b6df2ee0p0
  },
  { // Entry 77
    0x1.f5950f056e39e90cbaac1f89ab36b40ap2,
    0x1.cba2e8ba2e8b7p0,
    0x1.e83e0f83e0f76p2
  },
  { // Entry 78
    0x1.ddffe6e5a3a8384016ed35f115bc095ep-11,
    0x1.da12f684bda24p-11,
    -0x1.e9131abf0b717p-14
  },
  { // Entry 79
    0x1.7941bb05a39ca7ff5e4553b1fc4d7db9p-423,
    0x1.f8d7bbd7ce920p-426,
    -0x1.73f0fd4fd9fd0p-423
  },
  { // Entry 80
    0x1.b13fad7cb7c50801dede1905f3f366a1p9,
    0x1.f91b91b91b905p2,
    0x1.b13b13b13b130p9
  },
  { // Entry 81
    0x1.69fd85887947900071fbc08183b8ab23p0,
    0x1.fcf76c540d958p-1,
    -0x1.017098d82f95ep0
  },
  { // Entry 82
    0x1.21c9123e6cbbf812953910371e275dc7p3,
    0x1.ff77fffffffffp-9,
    -0x1.21c9107b0e488p3
  },
  { // Entry 83
    0x1.c66addfec91c411f38e2aacb6ea06a91p-3,
    0x1.ffeffffffffffp-4,
    -0x1.7777777777774p-3
  },
  { // Entry 84
    0x1.4eb522b24186e8254574c77b5f914855p-1,
    0x1.ffeffffffffffp-4,
    0x1.488888888888ap-1
  },
  { // Entry 85
    0x1.002caffe59b0a7feeda747a94b176ccap4,
    0x1.ffeffffffffffp3,
    -0x1.4888888888888p-1
  },
  { // Entry 86
    0x1.fff28f6f00e797fec43eb25e08b861abp3,
    0x1.ffeffffffffffp3,
    -0x1.99999999a7508p-4
  },
  { // Entry 87
    0x1.00000001fffff7fe0007f00400100ff6p20,
    0x1.fffffbfffffffp4,
    0x1.0p20
  },
  { // Entry 88
    0x1.0082de91198ee8170bcff2900895b92ap2,
    0x1.ffffffffffdffp-3,
    0x1.0002fffffffdfp2
  },
  { // Entry 89
    0x1.6a09e667f3c5125ab5042ba7be436cbbp-2,
    0x1.ffffffffffff7p-3,
    0x1.00000000000c0p-2
  },
  { // Entry 90
    0x1.ffffffffffffb0p500,
    0x1.ffffffffffffbp500,
    0x1.ffffffffffffbp-1
  },
  { // Entry 91
    0x1.333574eb66a002798d20bb2ca70862e4p-1,
    0x1.ffffffffffffep-3,
    0x1.1745d1745d177p-1
  },
  { // Entry 92
    0x1.745d1745d17557ffffffffffc41ap-3,
    0x1.ffffffffffffep-28,
    0x1.745d1745d1750p-3
  },
  { // Entry 93
    0x1.00000000000000007fffffffffffefffp1,
    0x1.ffffffffffffep-32,
    -0x1.0p1
  },
  { // Entry 94
    0x1.7777777777780000015d1745d1745c6cp-4,
    0x1.ffffffffffffep-40,
    0x1.7777777777780p-4
  },
  { // Entry 95
    0x1.01c5967e49cb581b1ce389659d8f68ecp2,
    0x1.ffffffffffffep1,
    -0x1.e2be2be2be2c3p-2
  },
  { // Entry 96
    0x1.0058d424f448e820225d2e7a25abc0ebp4,
    0x1.ffffffffffffep3,
    -0x1.aaaaaaaaaaaa8p-1
  },
  { // Entry 97
    0x1.6a09e667f3bcdfa9516192a2b726086dp0,
    -0x1.0000000000001p0,
    -0x1.0000000000001p0
  },
  { // Entry 98
    0x1.6a09e667f3bcd459022e5304d10b0412p0,
    -0x1.0000000000001p0,
    -0x1.0p0
  },
  { // Entry 99
    0x1.6a09e667f3bcceb0da94b335de1f72d2p0,
    -0x1.0000000000001p0,
    -0x1.fffffffffffffp-1
  },
  { // Entry 100
    0x1.6a09e667f3bcd459022e5304d10b0412p0,
    -0x1.0p0,
    -0x1.0000000000001p0
  },
  { // Entry 101
    0x1.6a09e667f3bcc908b2fb1366ea957d3ep0,
    -0x1.0p0,
    -0x1.0p0
  },
  { // Entry 102
    0x1.6a09e667f3bcc3608b617397f77caac1p0,
    -0x1.0p0,
    -0x1.fffffffffffffp-1
  },
  { // Entry 103
    0x1.6a09e667f3bcceb0da94b335de1f72d2p0,
    -0x1.fffffffffffffp-1,
    -0x1.0000000000001p0
  },
  { // Entry 104
    0x1.6a09e667f3bcc3608b617397f77caac1p0,
    -0x1.fffffffffffffp-1,
    -0x1.0p0
  },
  { // Entry 105
    0x1.6a09e667f3bcbdb863c7d3c9044d37a6p0,
    -0x1.fffffffffffffp-1,
    -0x1.fffffffffffffp-1
  },
  { // Entry 106
    0x1.6a09e667f3bcceb0da94b335de1f72d2p0,
    -0x1.0000000000001p0,
    0x1.fffffffffffffp-1
  },
  { // Entry 107
    0x1.6a09e667f3bcd459022e5304d10b0412p0,
    -0x1.0000000000001p0,
    0x1.0p0
  },
  { // Entry 108
    0x1.6a09e667f3bcdfa9516192a2b726086dp0,
    -0x1.0000000000001p0,
    0x1.0000000000001p0
  },
  { // Entry 109
    0x1.6a09e667f3bcc3608b617397f77caac1p0,
    -0x1.0p0,
    0x1.fffffffffffffp-1
  },
  { // Entry 110
    0x1.6a09e667f3bcc908b2fb1366ea957d3ep0,
    -0x1.0p0,
    0x1.0p0
  },
  { // Entry 111
    0x1.6a09e667f3bcd459022e5304d10b0412p0,
    -0x1.0p0,
    0x1.0000000000001p0
  },
  { // Entry 112
    0x1.6a09e667f3bcbdb863c7d3c9044d37a6p0,
    -0x1.fffffffffffffp-1,
    0x1.fffffffffffffp-1
  },
  { // Entry 113
    0x1.6a09e667f3bcc3608b617397f77caac1p0,
    -0x1.fffffffffffffp-1,
    0x1.0p0
  },
  { // Entry 114
    0x1.6a09e667f3bcceb0da94b335de1f72d2p0,
    -0x1.fffffffffffffp-1,
    0x1.0000000000001p0
  },
  { // Entry 115
    0x1.6a09e667f3bcceb0da94b335de1f72d2p0,
    0x1.fffffffffffffp-1,
    -0x1.0000000000001p0
  },
  { // Entry 116
    0x1.6a09e667f3bcc3608b617397f77caac1p0,
    0x1.fffffffffffffp-1,
    -0x1.0p0
  },
  { // Entry 117
    0x1.6a09e667f3bcbdb863c7d3c9044d37a6p0,
    0x1.fffffffffffffp-1,
    -0x1.fffffffffffffp-1
  },
  { // Entry 118
    0x1.6a09e667f3bcd459022e5304d10b0412p0,
    0x1.0p0,
    -0x1.0000000000001p0
  },
  { // Entry 119
    0x1.6a09e667f3bcc908b2fb1366ea957d3ep0,
    0x1.0p0,
    -0x1.0p0
  },
  { // Entry 120
    0x1.6a09e667f3bcc3608b617397f77caac1p0,
    0x1.0p0,
    -0x1.fffffffffffffp-1
  },
  { // Entry 121
    0x1.6a09e667f3bcdfa9516192a2b726086dp0,
    0x1.0000000000001p0,
    -0x1.0000000000001p0
  },
  { // Entry 122
    0x1.6a09e667f3bcd459022e5304d10b0412p0,
    0x1.0000000000001p0,
    -0x1.0p0
  },
  { // Entry 123
    0x1.6a09e667f3bcceb0da94b335de1f72d2p0,
    0x1.0000000000001p0,
    -0x1.fffffffffffffp-1
  },
  { // Entry 124
    0x1.6a09e667f3bcbdb863c7d3c9044d37a6p0,
    0x1.fffffffffffffp-1,
    0x1.fffffffffffffp-1
  },
  { // Entry 125
    0x1.6a09e667f3bcc3608b617397f77caac1p0,
    0x1.fffffffffffffp-1,
    0x1.0p0
  },
  { // Entry 126
    0x1.6a09e667f3bcceb0da94b335de1f72d2p0,
    0x1.fffffffffffffp-1,
    0x1.0000000000001p0
  },
  { // Entry 127
    0x1.6a09e667f3bcc3608b617397f77caac1p0,
    0x1.0p0,
    0x1.fffffffffffffp-1
  },
  { // Entry 128
    0x1.6a09e667f3bcc908b2fb1366ea957d3ep0,
    0x1.0p0,
    0x1.0p0
  },
  { // Entry 129
    0x1.6a09e667f3bcd459022e5304d10b0412p0,
    0x1.0p0,
    0x1.0000000000001p0
  },
  { // Entry 130
    0x1.6a09e667f3bcceb0da94b335de1f72d2p0,
    0x1.0000000000001p0,
    0x1.fffffffffffffp-1
  },
  { // Entry 131
    0x1.6a09e667f3bcd459022e5304d10b0412p0,
    0x1.0000000000001p0,
    0x1.0p0
  },
  { // Entry 132
    0x1.6a09e667f3bcdfa9516192a2b726086dp0,
    0x1.0000000000001p0,
    0x1.0000000000001p0
  },
  { // Entry 133
    0x1.6a09e667f3bcc908b2fb1366ea957d3ep0,
    -0x1.0p0,
    -0x1.0p0
  },
  { // Entry 134
    0x1.6a09e667f3bcc908b2fb1366ea957d3ep0,
    -0x1.0p0,
    0x1.0p0
  },
  { // Entry 135
    0x1.6a09e667f3bcc908b2fb1366ea957d3ep0,
    0x1.0p0,
    -0x1.0p0
  },
  { // Entry 136
    0x1.6a09e667f3bcc908b2fb1366ea957d3ep0,
    0x1.0p0,
    0x1.0p0
  },
  { // Entry 137
    0x1.6a09e667f3bcc908b2fb1366ea957d3ep0,
    0x1.0p0,
    0x1.0p0
  },
  { // Entry 138
    0x1.01fe03f61bad04b1068572febc925ad1p3,
    0x1.0p0,
    0x1.0p3
  },
  { // Entry 139
    0x1.01fe03f61bad04b1068572febc925ad1p3,
    0x1.0p3,
    0x1.0p0
  },
  { // Entry 140
    0x1.6a09e667f3bcc908b2fb1366ea957d3ep3,
    0x1.0p3,
    0x1.0p3
  },
  { // Entry 141
    0x1.00001ffffe00003ffff60001bfffacp9,
    0x1.0p0,
    0x1.0p9
  },
  { // Entry 142
    0x1.000007ffffe00000fffff600006ffffap10,
    0x1.0p0,
    0x1.0p10
  },
  { // Entry 143
    0x1.0007ffe000fff6006ffac041fca62cadp9,
    0x1.0p3,
    0x1.0p9
  },
  { // Entry 144
    0x1.0001fffe0003fff6001bffac0107fca6p10,
    0x1.0p3,
    0x1.0p10
  },
  { // Entry 145
    0x1.p100,
    0x1.0p0,
    0x1.0p100
  },
  { // Entry 146
    0x1.p101,
    0x1.0p0,
    0x1.0p101
  },
  { // Entry 147
    0x1.p100,
    0x1.0p3,
    0x1.0p100
  },
  { // Entry 148
    0x1.p101,
    0x1.0p3,
    0x1.0p101
  },
  { // Entry 149
    0x1.00001ffffe00003ffff60001bfffacp9,
    0x1.0p9,
    0x1.0p0
  },
  { // Entry 150
    0x1.0007ffe000fff6006ffac041fca62cadp9,
    0x1.0p9,
    0x1.0p3
  },
  { // Entry 151
    0x1.000007ffffe00000fffff600006ffffap10,
    0x1.0p10,
    0x1.0p0
  },
  { // Entry 152
    0x1.0001fffe0003fff6001bffac0107fca6p10,
    0x1.0p10,
    0x1.0p3
  },
  { // Entry 153
    0x1.6a09e667f3bcc908b2fb1366ea957d3ep9,
    0x1.0p9,
    0x1.0p9
  },
  { // Entry 154
    0x1.1e3779b97f4a7c15f39cc0605cedc834p10,
    0x1.0p9,
    0x1.0p10
  },
  { // Entry 155
    0x1.1e3779b97f4a7c15f39cc0605cedc834p10,
    0x1.0p10,
    0x1.0p9
  },
  { // Entry 156
    0x1.6a09e667f3bcc908b2fb1366ea957d3ep10,
    0x1.0p10,
    0x1.0p10
  },
  { // Entry 157
    0x1.p100,
    0x1.0p9,
    0x1.0p100
  },
  { // Entry 158
    0x1.p101,
    0x1.0p9,
    0x1.0p101
  },
  { // Entry 159
    0x1.p100,
    0x1.0p10,
    0x1.0p100
  },
  { // Entry 160
    0x1.p101,
    0x1.0p10,
    0x1.0p101
  },
  { // Entry 161
    0x1.p100,
    0x1.0p100,
    0x1.0p0
  },
  { // Entry 162
    0x1.p100,
    0x1.0p100,
    0x1.0p3
  },
  { // Entry 163
    0x1.p101,
    0x1.0p101,
    0x1.0p0
  },
  { // Entry 164
    0x1.p101,
    0x1.0p101,
    0x1.0p3
  },
  { // Entry 165
    0x1.p100,
    0x1.0p100,
    0x1.0p9
  },
  { // Entry 166
    0x1.p100,
    0x1.0p100,
    0x1.0p10
  },
  { // Entry 167
    0x1.p101,
    0x1.0p101,
    0x1.0p9
  },
  { // Entry 168
    0x1.p101,
    0x1.0p101,
    0x1.0p10
  },
  { // Entry 169
    0x1.6a09e667f3bcc908b2fb1366ea957d3ep100,
    0x1.0p100,
    0x1.0p100
  },
  { // Entry 170
    0x1.1e3779b97f4a7c15f39cc0605cedc834p101,
    0x1.0p100,
    0x1.0p101
  },
  { // Entry 171
    0x1.1e3779b97f4a7c15f39cc0605cedc834p101,
    0x1.0p101,
    0x1.0p100
  },
  { // Entry 172
    0x1.6a09e667f3bcc908b2fb1366ea957d3ep101,
    0x1.0p101,
    0x1.0p101
  },
  { // Entry 173
    0x1.ad5336963eefa83d75cf889be3a34d14p2,
    0x1.7ffffffffffffp2,
    0x1.7ffffffffffffp1
  },
  { // Entry 174
    0x1.ad5336963eefabd15a8840999ed93d89p2,
    0x1.7ffffffffffffp2,
    0x1.8p1
  },
  { // Entry 175
    0x1.ad5336963eefaf653f40f8975a2db59ep2,
    0x1.7ffffffffffffp2,
    0x1.8000000000001p1
  },
  { // Entry 176
    0x1.ad5336963eefb68d08b26892d04d4378p2,
    0x1.8p2,
    0x1.7ffffffffffffp1
  },
  { // Entry 177
    0x1.ad5336963eefba20ed6b20908b64ac4ep2,
    0x1.8p2,
    0x1.8p1
  },
  { // Entry 178
    0x1.ad5336963eefbdb4d223d88e469a9cc3p2,
    0x1.8p2,
    0x1.8000000000001p1
  },
  { // Entry 179
    0x1.ad5336963eefc4dc9b954889bd15c17dp2,
    0x1.8000000000001p2,
    0x1.7ffffffffffffp1
  },
  { // Entry 180
    0x1.ad5336963eefc870804e0087780ea2b2p2,
    0x1.8000000000001p2,
    0x1.8p1
  },
  { // Entry 181
    0x1.ad5336963eefcc046506b88533260b87p2,
    0x1.8000000000001p2,
    0x1.8000000000001p1
  },
  { // Entry 182
    0x1.fffffffffffff0p1023,
    0x1.fffffffffffffp1023,
    0.0
  },
  { // Entry 183
    0x1.6a09e667f3bc9bc7762e14ef517466dep-1022,
    0x1.ffffffffffffcp-1023,
    0x1.ffffffffffffcp-1023
  },
  { // Entry 184
    0x1.6a09e667f3bca717c561548d37e9edb3p-1022,
    0x1.ffffffffffffcp-1023,
    0x1.ffffffffffffep-1023
  },
  { // Entry 185
    0x1.6a09e667f3bcb2681494942b1eb9f701p-1022,
    0x1.ffffffffffffcp-1023,
    0x1.0p-1022
  },
  { // Entry 186
    0x1.6a09e667f3bca717c561548d37e9edb3p-1022,
    0x1.ffffffffffffep-1023,
    0x1.ffffffffffffcp-1023
  },
  { // Entry 187
    0x1.6a09e667f3bcb2681494942b1e04f20ep-1022,
    0x1.ffffffffffffep-1023,
    0x1.ffffffffffffep-1023
  },
  { // Entry 188
    0x1.6a09e667f3bcbdb863c7d3c9047a78e3p-1022,
    0x1.ffffffffffffep-1023,
    0x1.0p-1022
  },
  { // Entry 189
    0x1.6a09e667f3bcb2681494942b1eb9f701p-1022,
    0x1.0p-1022,
    0x1.ffffffffffffcp-1023
  },
  { // Entry 190
    0x1.6a09e667f3bcbdb863c7d3c9047a78e3p-1022,
    0x1.0p-1022,
    0x1.ffffffffffffep-1023
  },
  { // Entry 191
    0x1.6a09e667f3bcc908b2fb1366ea957d3ep-1022,
    0x1.0p-1022,
    0x1.0p-1022
  },
  { // Entry 192
    0x1.6a09e667f3bcc908b2fb1366ea957d3ep-1074,
    0x1.0p-1074,
    0x1.0p-1074
  },
  { // Entry 193
    0x1.6a09e667f3bcbdb863c7d3c9044d37a6p512,
    0x1.fffffffffffffp511,
    0x1.fffffffffffffp511
  },
  { // Entry 194
    0x1.6a09e667f3bcbdb863c7d3c9044d37a6p512,
    0x1.fffffffffffffp511,
    0x1.fffffffffffffp511
  },
  { // Entry 195
    0x1.6a09e667f3bcbdb863c7d3c9044d37a6p501,
    0x1.fffffffffffffp500,
    0x1.fffffffffffffp500
  },
  { // Entry 196
    0x1.6a09e667f3bcc3608b617397f77caac1p501,
    0x1.fffffffffffffp500,
    0x1.0p501
  },
  { // Entry 197
    0x1.6a09e667f3bcceb0da94b335de1f72d2p501,
    0x1.fffffffffffffp500,
    0x1.0000000000001p501
  },
  { // Entry 198
    0x1.6a09e667f3bcc3608b617397f77caac1p501,
    0x1.0p501,
    0x1.fffffffffffffp500
  },
  { // Entry 199
    0x1.6a09e667f3bcc908b2fb1366ea957d3ep501,
    0x1.0p501,
    0x1.0p501
  },
  { // Entry 200
    0x1.6a09e667f3bcd459022e5304d10b0412p501,
    0x1.0p501,
    0x1.0000000000001p501
  },
  { // Entry 201
    0x1.6a09e667f3bcceb0da94b335de1f72d2p501,
    0x1.0000000000001p501,
    0x1.fffffffffffffp500
  },
  { // Entry 202
    0x1.6a09e667f3bcd459022e5304d10b0412p501,
    0x1.0000000000001p501,
    0x1.0p501
  },
  { // Entry 203
    0x1.6a09e667f3bcdfa9516192a2b726086dp501,
    0x1.0000000000001p501,
    0x1.0000000000001p501
  },
  { // Entry 204
    0x1.6a09e667f3bcbdb863c7d3c9044d37a6p-501,
    0x1.fffffffffffffp-502,
    0x1.fffffffffffffp-502
  },
  { // Entry 205
    0x1.6a09e667f3bcc3608b617397f77caac1p-501,
    0x1.fffffffffffffp-502,
    0x1.0p-501
  },
  { // Entry 206
    0x1.6a09e667f3bcceb0da94b335de1f72d2p-501,
    0x1.fffffffffffffp-502,
    0x1.0000000000001p-501
  },
  { // Entry 207
    0x1.6a09e667f3bcc3608b617397f77caac1p-501,
    0x1.0p-501,
    0x1.fffffffffffffp-502
  },
  { // Entry 208
    0x1.6a09e667f3bcc908b2fb1366ea957d3ep-501,
    0x1.0p-501,
    0x1.0p-501
  },
  { // Entry 209
    0x1.6a09e667f3bcd459022e5304d10b0412p-501,
    0x1.0p-501,
    0x1.0000000000001p-501
  },
  { // Entry 210
    0x1.6a09e667f3bcceb0da94b335de1f72d2p-501,
    0x1.0000000000001p-501,
    0x1.fffffffffffffp-502
  },
  { // Entry 211
    0x1.6a09e667f3bcd459022e5304d10b0412p-501,
    0x1.0000000000001p-501,
    0x1.0p-501
  },
  { // Entry 212
    0x1.6a09e667f3bcdfa9516192a2b726086dp-501,
    0x1.0000000000001p-501,
    0x1.0000000000001p-501
  },
  { // Entry 213
    0x1.fffffffffffff0p500,
    0x1.fffffffffffffp500,
    0x1.fffffffffffffp-502
  },
  { // Entry 214
    0x1.fffffffffffff0p500,
    0x1.fffffffffffffp500,
    0x1.0p-501
  },
  { // Entry 215
    0x1.fffffffffffff0p500,
    0x1.fffffffffffffp500,
    0x1.0000000000001p-501
  },
  { // Entry 216
    0x1.p501,
    0x1.0p501,
    0x1.fffffffffffffp-502
  },
  { // Entry 217
    0x1.p501,
    0x1.0p501,
    0x1.0p-501
  },
  { // Entry 218
    0x1.p501,
    0x1.0p501,
    0x1.0000000000001p-501
  },
  { // Entry 219
    0x1.00000000000010p501,
    0x1.0000000000001p501,
    0x1.fffffffffffffp-502
  },
  { // Entry 220
    0x1.00000000000010p501,
    0x1.0000000000001p501,
    0x1.0p-501
  },
  { // Entry 221
    0x1.00000000000010p501,
    0x1.0000000000001p501,
    0x1.0000000000001p-501
  },
  { // Entry 222
    0x1.fffffffffffff0p500,
    0x1.fffffffffffffp500,
    -0x1.0p-1074
  },
  { // Entry 223
    0x1.fffffffffffff0p500,
    0x1.fffffffffffffp500,
    -0.0
  },
  { // Entry 224
    0x1.fffffffffffff0p500,
    0x1.fffffffffffffp500,
    0x1.0p-1074
  },
  { // Entry 225
    0x1.p501,
    0x1.0p501,
    -0x1.0p-1074
  },
  { // Entry 226
    0x1.p501,
    0x1.0p501,
    -0.0
  },
  { // Entry 227
    0x1.p501,
    0x1.0p501,
    0x1.0p-1074
  },
  { // Entry 228
    0x1.00000000000010p501,
    0x1.0000000000001p501,
    -0x1.0p-1074
  },
  { // Entry 229
    0x1.00000000000010p501,
    0x1.0000000000001p501,
    -0.0
  },
  { // Entry 230
    0x1.00000000000010p501,
    0x1.0000000000001p501,
    0x1.0p-1074
  },
  { // Entry 231
    0x1.fffffffffffff0p-502,
    0x1.fffffffffffffp-502,
    -0x1.0p-1074
  },
  { // Entry 232
    0x1.fffffffffffff0p-502,
    0x1.fffffffffffffp-502,
    -0.0
  },
  { // Entry 233
    0x1.fffffffffffff0p-502,
    0x1.fffffffffffffp-502,
    0x1.0p-1074
  },
  { // Entry 234
    0x1.p-501,
    0x1.0p-501,
    -0x1.0p-1074
  },
  { // Entry 235
    0x1.p-501,
    0x1.0p-501,
    -0.0
  },
  { // Entry 236
    0x1.p-501,
    0x1.0p-501,
    0x1.0p-1074
  },
  { // Entry 237
    0x1.00000000000010p-501,
    0x1.0000000000001p-501,
    -0x1.0p-1074
  },
  { // Entry 238
    0x1.00000000000010p-501,
    0x1.0000000000001p-501,
    -0.0
  },
  { // Entry 239
    0x1.00000000000010p-501,
    0x1.0000000000001p-501,
    0x1.0p-1074
  },
  { // Entry 240
    0x1.fffffffffffff0p500,
    0x1.fffffffffffffp500,
    0x1.fffffffffffffp-1
  },
  { // Entry 241
    0x1.fffffffffffff0p500,
    0x1.fffffffffffffp500,
    0x1.0p0
  },
  { // Entry 242
    0x1.fffffffffffff0p500,
    0x1.fffffffffffffp500,
    0x1.0000000000001p0
  },
  { // Entry 243
    0x1.p501,
    0x1.0p501,
    0x1.fffffffffffffp-1
  },
  { // Entry 244
    0x1.p501,
    0x1.0p501,
    0x1.0p0
  },
  { // Entry 245
    0x1.p501,
    0x1.0p501,
    0x1.0000000000001p0
  },
  { // Entry 246
    0x1.00000000000010p501,
    0x1.0000000000001p501,
    0x1.fffffffffffffp-1
  },
  { // Entry 247
    0x1.00000000000010p501,
    0x1.0000000000001p501,
    0x1.0p0
  },
  { // Entry 248
    0x1.00000000000010p501,
    0x1.0000000000001p501,
    0x1.0000000000001p0
  },
  { // Entry 249
    0x1.fffffffffffff0p-1,
    0x1.fffffffffffffp-502,
    0x1.fffffffffffffp-1
  },
  { // Entry 250
    0x1.p0,
    0x1.fffffffffffffp-502,
    0x1.0p0
  },
  { // Entry 251
    0x1.00000000000010p0,
    0x1.fffffffffffffp-502,
    0x1.0000000000001p0
  },
  { // Entry 252
    0x1.fffffffffffff0p-1,
    0x1.0p-501,
    0x1.fffffffffffffp-1
  },
  { // Entry 253
    0x1.p0,
    0x1.0p-501,
    0x1.0p0
  },
  { // Entry 254
    0x1.00000000000010p0,
    0x1.0p-501,
    0x1.0000000000001p0
  },
  { // Entry 255
    0x1.fffffffffffff0p-1,
    0x1.0000000000001p-501,
    0x1.fffffffffffffp-1
  },
  { // Entry 256
    0x1.p0,
    0x1.0000000000001p-501,
    0x1.0p0
  },
  { // Entry 257
    0x1.00000000000010p0,
    0x1.0000000000001p-501,
    0x1.0000000000001p0
  },
  { // Entry 258
    0x1.fffffffffffff000000000000fffffffp49,
    0x1.fffffffffffffp49,
    0x1.fffffffffffffp-1
  },
  { // Entry 259
    0x1.fffffffffffff0000000000010p49,
    0x1.fffffffffffffp49,
    0x1.0p0
  },
  { // Entry 260
    0x1.fffffffffffff0000000000010p49,
    0x1.fffffffffffffp49,
    0x1.0000000000001p0
  },
  { // Entry 261
    0x1.00000000000000000000000007ffffffp50,
    0x1.0p50,
    0x1.fffffffffffffp-1
  },
  { // Entry 262
    0x1.00000000000000000000000007ffffffp50,
    0x1.0p50,
    0x1.0p0
  },
  { // Entry 263
    0x1.00000000000000000000000008p50,
    0x1.0p50,
    0x1.0000000000001p0
  },
  { // Entry 264
    0x1.00000000000010000000000007ffffffp50,
    0x1.0000000000001p50,
    0x1.fffffffffffffp-1
  },
  { // Entry 265
    0x1.00000000000010000000000007ffffffp50,
    0x1.0000000000001p50,
    0x1.0p0
  },
  { // Entry 266
    0x1.00000000000010000000000008p50,
    0x1.0000000000001p50,
    0x1.0000000000001p0
  },
  { // Entry 267
    0x1.fffffffffffff0000000000003ffffffp50,
    0x1.fffffffffffffp50,
    0x1.fffffffffffffp-1
  },
  { // Entry 268
    0x1.fffffffffffff0000000000004p50,
    0x1.fffffffffffffp50,
    0x1.0p0
  },
  { // Entry 269
    0x1.fffffffffffff0000000000004p50,
    0x1.fffffffffffffp50,
    0x1.0000000000001p0
  },
  { // Entry 270
    0x1.00000000000000000000000001ffffffp51,
    0x1.0p51,
    0x1.fffffffffffffp-1
  },
  { // Entry 271
    0x1.00000000000000000000000001ffffffp51,
    0x1.0p51,
    0x1.0p0
  },
  { // Entry 272
    0x1.00000000000000000000000002p51,
    0x1.0p51,
    0x1.0000000000001p0
  },
  { // Entry 273
    0x1.00000000000010000000000001ffffffp51,
    0x1.0000000000001p51,
    0x1.fffffffffffffp-1
  },
  { // Entry 274
    0x1.00000000000010000000000001ffffffp51,
    0x1.0000000000001p51,
    0x1.0p0
  },
  { // Entry 275
    0x1.00000000000010000000000002p51,
    0x1.0000000000001p51,
    0x1.0000000000001p0
  },
  { // Entry 276
    0x1.fffffffffffff0000000000000ffffffp51,
    0x1.fffffffffffffp51,
    0x1.fffffffffffffp-1
  },
  { // Entry 277
    0x1.fffffffffffff0000000000001p51,
    0x1.fffffffffffffp51,
    0x1.0p0
  },
  { // Entry 278
    0x1.fffffffffffff0000000000001p51,
    0x1.fffffffffffffp51,
    0x1.0000000000001p0
  },
  { // Entry 279
    0x1.000000000000000000000000007fffffp52,
    0x1.0p52,
    0x1.fffffffffffffp-1
  },
  { // Entry 280
    0x1.000000000000000000000000007fffffp52,
    0x1.0p52,
    0x1.0p0
  },
  { // Entry 281
    0x1.0000000000000000000000000080p52,
    0x1.0p52,
    0x1.0000000000001p0
  },
  { // Entry 282
    0x1.000000000000100000000000007fffffp52,
    0x1.0000000000001p52,
    0x1.fffffffffffffp-1
  },
  { // Entry 283
    0x1.000000000000100000000000007fffffp52,
    0x1.0000000000001p52,
    0x1.0p0
  },
  { // Entry 284
    0x1.0000000000001000000000000080p52,
    0x1.0000000000001p52,
    0x1.0000000000001p0
  },
  { // Entry 285
    0x1.fffffffffffff00000000000003fffffp52,
    0x1.fffffffffffffp52,
    0x1.fffffffffffffp-1
  },
  { // Entry 286
    0x1.fffffffffffff000000000000040p52,
    0x1.fffffffffffffp52,
    0x1.0p0
  },
  { // Entry 287
    0x1.fffffffffffff000000000000040p52,
    0x1.fffffffffffffp52,
    0x1.0000000000001p0
  },
  { // Entry 288
    0x1.000000000000000000000000001fffffp53,
    0x1.0p53,
    0x1.fffffffffffffp-1
  },
  { // Entry 289
    0x1.000000000000000000000000001fffffp53,
    0x1.0p53,
    0x1.0p0
  },
  { // Entry 290
    0x1.0000000000000000000000000020p53,
    0x1.0p53,
    0x1.0000000000001p0
  },
  { // Entry 291
    0x1.000000000000100000000000001fffffp53,
    0x1.0000000000001p53,
    0x1.fffffffffffffp-1
  },
  { // Entry 292
    0x1.000000000000100000000000001fffffp53,
    0x1.0000000000001p53,
    0x1.0p0
  },
  { // Entry 293
    0x1.0000000000001000000000000020p53,
    0x1.0000000000001p53,
    0x1.0000000000001p0
  },
  { // Entry 294
    0x1.fffffffffffff00000000000000fffffp53,
    0x1.fffffffffffffp53,
    0x1.fffffffffffffp-1
  },
  { // Entry 295
    0x1.fffffffffffff000000000000010p53,
    0x1.fffffffffffffp53,
    0x1.0p0
  },
  { // Entry 296
    0x1.fffffffffffff000000000000010p53,
    0x1.fffffffffffffp53,
    0x1.0000000000001p0
  },
  { // Entry 297
    0x1.0000000000000000000000000007ffffp54,
    0x1.0p54,
    0x1.fffffffffffffp-1
  },
  { // Entry 298
    0x1.0000000000000000000000000007ffffp54,
    0x1.0p54,
    0x1.0p0
  },
  { // Entry 299
    0x1.0000000000000000000000000008p54,
    0x1.0p54,
    0x1.0000000000001p0
  },
  { // Entry 300
    0x1.0000000000001000000000000007ffffp54,
    0x1.0000000000001p54,
    0x1.fffffffffffffp-1
  },
  { // Entry 301
    0x1.0000000000001000000000000007ffffp54,
    0x1.0000000000001p54,
    0x1.0p0
  },
  { // Entry 302
    0x1.0000000000001000000000000008p54,
    0x1.0000000000001p54,
    0x1.0000000000001p0
  },
  { // Entry 303
    0x1.6a09e667f3bcbdb863c7d3c9044d37a6p-500,
    0x1.fffffffffffffp-501,
    0x1.fffffffffffffp-501
  },
  { // Entry 304
    0x1.6a09e667f3bcc3608b617397f77caac1p-500,
    0x1.fffffffffffffp-501,
    0x1.0p-500
  },
  { // Entry 305
    0x1.6a09e667f3bcceb0da94b335de1f72d2p-500,
    0x1.fffffffffffffp-501,
    0x1.0000000000001p-500
  },
  { // Entry 306
    0x1.6a09e667f3bcc3608b617397f77caac1p-500,
    0x1.0p-500,
    0x1.fffffffffffffp-501
  },
  { // Entry 307
    0x1.6a09e667f3bcc908b2fb1366ea957d3ep-500,
    0x1.0p-500,
    0x1.0p-500
  },
  { // Entry 308
    0x1.6a09e667f3bcd459022e5304d10b0412p-500,
    0x1.0p-500,
    0x1.0000000000001p-500
  },
  { // Entry 309
    0x1.6a09e667f3bcceb0da94b335de1f72d2p-500,
    0x1.0000000000001p-500,
    0x1.fffffffffffffp-501
  },
  { // Entry 310
    0x1.6a09e667f3bcd459022e5304d10b0412p-500,
    0x1.0000000000001p-500,
    0x1.0p-500
  },
  { // Entry 311
    0x1.6a09e667f3bcdfa9516192a2b726086dp-500,
    0x1.0000000000001p-500,
    0x1.0000000000001p-500
  },
  { // Entry 312
    0x1.6a09e667f3bcbdb863c7d3c9044d37a6p-60,
    0x1.fffffffffffffp-61,
    0x1.fffffffffffffp-61
  },
  { // Entry 313
    0x1.6a09e667f3bcc3608b617397f77caac1p-60,
    0x1.fffffffffffffp-61,
    0x1.0p-60
  },
  { // Entry 314
    0x1.6a09e667f3bcceb0da94b335de1f72d2p-60,
    0x1.fffffffffffffp-61,
    0x1.0000000000001p-60
  },
  { // Entry 315
    0x1.6a09e667f3bcc3608b617397f77caac1p-60,
    0x1.0p-60,
    0x1.fffffffffffffp-61
  },
  { // Entry 316
    0x1.6a09e667f3bcc908b2fb1366ea957d3ep-60,
    0x1.0p-60,
    0x1.0p-60
  },
  { // Entry 317
    0x1.6a09e667f3bcd459022e5304d10b0412p-60,
    0x1.0p-60,
    0x1.0000000000001p-60
  },
  { // Entry 318
    0x1.6a09e667f3bcceb0da94b335de1f72d2p-60,
    0x1.0000000000001p-60,
    0x1.fffffffffffffp-61
  },
  { // Entry 319
    0x1.6a09e667f3bcd459022e5304d10b0412p-60,
    0x1.0000000000001p-60,
    0x1.0p-60
  },
  { // Entry 320
    0x1.6a09e667f3bcdfa9516192a2b726086dp-60,
    0x1.0000000000001p-60,
    0x1.0000000000001p-60
  },
  { // Entry 321
    0x1.6a09e667f3bcbdb863c7d3c9044d37a6p-10,
    0x1.fffffffffffffp-11,
    0x1.fffffffffffffp-11
  },
  { // Entry 322
    0x1.6a09e667f3bcc3608b617397f77caac1p-10,
    0x1.fffffffffffffp-11,
    0x1.0p-10
  },
  { // Entry 323
    0x1.6a09e667f3bcceb0da94b335de1f72d2p-10,
    0x1.fffffffffffffp-11,
    0x1.0000000000001p-10
  },
  { // Entry 324
    0x1.6a09e667f3bcc3608b617397f77caac1p-10,
    0x1.0p-10,
    0x1.fffffffffffffp-11
  },
  { // Entry 325
    0x1.6a09e667f3bcc908b2fb1366ea957d3ep-10,
    0x1.0p-10,
    0x1.0p-10
  },
  { // Entry 326
    0x1.6a09e667f3bcd459022e5304d10b0412p-10,
    0x1.0p-10,
    0x1.0000000000001p-10
  },
  { // Entry 327
    0x1.6a09e667f3bcceb0da94b335de1f72d2p-10,
    0x1.0000000000001p-10,
    0x1.fffffffffffffp-11
  },
  { // Entry 328
    0x1.6a09e667f3bcd459022e5304d10b0412p-10,
    0x1.0000000000001p-10,
    0x1.0p-10
  },
  { // Entry 329
    0x1.6a09e667f3bcdfa9516192a2b726086dp-10,
    0x1.0000000000001p-10,
    0x1.0000000000001p-10
  },
  { // Entry 330
    0x1.6a09e667f3bcbdb863c7d3c9044d37a6p-1,
    0x1.fffffffffffffp-2,
    0x1.fffffffffffffp-2
  },
  { // Entry 331
    0x1.6a09e667f3bcc3608b617397f77caac1p-1,
    0x1.fffffffffffffp-2,
    0x1.0p-1
  },
  { // Entry 332
    0x1.6a09e667f3bcceb0da94b335de1f72d2p-1,
    0x1.fffffffffffffp-2,
    0x1.0000000000001p-1
  },
  { // Entry 333
    0x1.6a09e667f3bcc3608b617397f77caac1p-1,
    0x1.0p-1,
    0x1.fffffffffffffp-2
  },
  { // Entry 334
    0x1.6a09e667f3bcc908b2fb1366ea957d3ep-1,
    0x1.0p-1,
    0x1.0p-1
  },
  { // Entry 335
    0x1.6a09e667f3bcd459022e5304d10b0412p-1,
    0x1.0p-1,
    0x1.0000000000001p-1
  },
  { // Entry 336
    0x1.6a09e667f3bcceb0da94b335de1f72d2p-1,
    0x1.0000000000001p-1,
    0x1.fffffffffffffp-2
  },
  { // Entry 337
    0x1.6a09e667f3bcd459022e5304d10b0412p-1,
    0x1.0000000000001p-1,
    0x1.0p-1
  },
  { // Entry 338
    0x1.6a09e667f3bcdfa9516192a2b726086dp-1,
    0x1.0000000000001p-1,
    0x1.0000000000001p-1
  },
  { // Entry 339
    0x1.6a09e667f3bcbdb863c7d3c9044d37a6p1,
    0x1.fffffffffffffp0,
    0x1.fffffffffffffp0
  },
  { // Entry 340
    0x1.6a09e667f3bcc3608b617397f77caac1p1,
    0x1.fffffffffffffp0,
    0x1.0p1
  },
  { // Entry 341
    0x1.6a09e667f3bcceb0da94b335de1f72d2p1,
    0x1.fffffffffffffp0,
    0x1.0000000000001p1
  },
  { // Entry 342
    0x1.6a09e667f3bcc3608b617397f77caac1p1,
    0x1.0p1,
    0x1.fffffffffffffp0
  },
  { // Entry 343
    0x1.6a09e667f3bcc908b2fb1366ea957d3ep1,
    0x1.0p1,
    0x1.0p1
  },
  { // Entry 344
    0x1.6a09e667f3bcd459022e5304d10b0412p1,
    0x1.0p1,
    0x1.0000000000001p1
  },
  { // Entry 345
    0x1.6a09e667f3bcceb0da94b335de1f72d2p1,
    0x1.0000000000001p1,
    0x1.fffffffffffffp0
  },
  { // Entry 346
    0x1.6a09e667f3bcd459022e5304d10b0412p1,
    0x1.0000000000001p1,
    0x1.0p1
  },
  { // Entry 347
    0x1.6a09e667f3bcdfa9516192a2b726086dp1,
    0x1.0000000000001p1,
    0x1.0000000000001p1
  },
  { // Entry 348
    0x1.6a09e667f3bcbdb863c7d3c9044d37a6p10,
    0x1.fffffffffffffp9,
    0x1.fffffffffffffp9
  },
  { // Entry 349
    0x1.6a09e667f3bcc3608b617397f77caac1p10,
    0x1.fffffffffffffp9,
    0x1.0p10
  },
  { // Entry 350
    0x1.6a09e667f3bcceb0da94b335de1f72d2p10,
    0x1.fffffffffffffp9,
    0x1.0000000000001p10
  },
  { // Entry 351
    0x1.6a09e667f3bcc3608b617397f77caac1p10,
    0x1.0p10,
    0x1.fffffffffffffp9
  },
  { // Entry 352
    0x1.6a09e667f3bcc908b2fb1366ea957d3ep10,
    0x1.0p10,
    0x1.0p10
  },
  { // Entry 353
    0x1.6a09e667f3bcd459022e5304d10b0412p10,
    0x1.0p10,
    0x1.0000000000001p10
  },
  { // Entry 354
    0x1.6a09e667f3bcceb0da94b335de1f72d2p10,
    0x1.0000000000001p10,
    0x1.fffffffffffffp9
  },
  { // Entry 355
    0x1.6a09e667f3bcd459022e5304d10b0412p10,
    0x1.0000000000001p10,
    0x1.0p10
  },
  { // Entry 356
    0x1.6a09e667f3bcdfa9516192a2b726086dp10,
    0x1.0000000000001p10,
    0x1.0000000000001p10
  },
  { // Entry 357
    0x1.6a09e667f3bcbdb863c7d3c9044d37a6p60,
    0x1.fffffffffffffp59,
    0x1.fffffffffffffp59
  },
  { // Entry 358
    0x1.6a09e667f3bcc3608b617397f77caac1p60,
    0x1.fffffffffffffp59,
    0x1.0p60
  },
  { // Entry 359
    0x1.6a09e667f3bcceb0da94b335de1f72d2p60,
    0x1.fffffffffffffp59,
    0x1.0000000000001p60
  },
  { // Entry 360
    0x1.6a09e667f3bcc3608b617397f77caac1p60,
    0x1.0p60,
    0x1.fffffffffffffp59
  },
  { // Entry 361
    0x1.6a09e667f3bcc908b2fb1366ea957d3ep60,
    0x1.0p60,
    0x1.0p60
  },
  { // Entry 362
    0x1.6a09e667f3bcd459022e5304d10b0412p60,
    0x1.0p60,
    0x1.0000000000001p60
  },
  { // Entry 363
    0x1.6a09e667f3bcceb0da94b335de1f72d2p60,
    0x1.0000000000001p60,
    0x1.fffffffffffffp59
  },
  { // Entry 364
    0x1.6a09e667f3bcd459022e5304d10b0412p60,
    0x1.0000000000001p60,
    0x1.0p60
  },
  { // Entry 365
    0x1.6a09e667f3bcdfa9516192a2b726086dp60,
    0x1.0000000000001p60,
    0x1.0000000000001p60
  },
  { // Entry 366
    0x1.6a09e667f3bcbdb863c7d3c9044d37a6p500,
    0x1.fffffffffffffp499,
    0x1.fffffffffffffp499
  },
  { // Entry 367
    0x1.6a09e667f3bcc3608b617397f77caac1p500,
    0x1.fffffffffffffp499,
    0x1.0p500
  },
  { // Entry 368
    0x1.6a09e667f3bcceb0da94b335de1f72d2p500,
    0x1.fffffffffffffp499,
    0x1.0000000000001p500
  },
  { // Entry 369
    0x1.6a09e667f3bcc3608b617397f77caac1p500,
    0x1.0p500,
    0x1.fffffffffffffp499
  },
  { // Entry 370
    0x1.6a09e667f3bcc908b2fb1366ea957d3ep500,
    0x1.0p500,
    0x1.0p500
  },
  { // Entry 371
    0x1.6a09e667f3bcd459022e5304d10b0412p500,
    0x1.0p500,
    0x1.0000000000001p500
  },
  { // Entry 372
    0x1.6a09e667f3bcceb0da94b335de1f72d2p500,
    0x1.0000000000001p500,
    0x1.fffffffffffffp499
  },
  { // Entry 373
    0x1.6a09e667f3bcd459022e5304d10b0412p500,
    0x1.0000000000001p500,
    0x1.0p500
  },
  { // Entry 374
    0x1.6a09e667f3bcdfa9516192a2b726086dp500,
    0x1.0000000000001p500,
    0x1.0000000000001p500
  },
  { // Entry 375
    0x1.fffffffffffff0p1022,
    0x1.fffffffffffffp1022,
    0x1.fffffffffffffp-1
  },
  { // Entry 376
    0x1.fffffffffffff0p1022,
    0x1.fffffffffffffp1022,
    0x1.0p0
  },
  { // Entry 377
    0x1.fffffffffffff0p1022,
    0x1.fffffffffffffp1022,
    0x1.0000000000001p0
  },
  { // Entry 378
    0x1.p1023,
    0x1.0p1023,
    0x1.fffffffffffffp-1
  },
  { // Entry 379
    0x1.p1023,
    0x1.0p1023,
    0x1.0p0
  },
  { // Entry 380
    0x1.p1023,
    0x1.0p1023,
    0x1.0000000000001p0
  },
  { // Entry 381
    0x1.00000000000010p1023,
    0x1.0000000000001p1023,
    0x1.fffffffffffffp-1
  },
  { // Entry 382
    0x1.00000000000010p1023,
    0x1.0000000000001p1023,
    0x1.0p0
  },
  { // Entry 383
    0x1.00000000000010p1023,
    0x1.0000000000001p1023,
    0x1.0000000000001p0
  },
  { // Entry 384
    0x1.778d27690518c71d8d4d782889fc1c38p-27,
    0x1.00a436e9442ebp-27,
    0x1.122dc42e12482p-27
  },
  { // Entry 385
    0x1.778d27690518d2cbeb1e43a94a18dcbbp-27,
    0x1.00a436e9442ebp-27,
    0x1.122dc42e12483p-27
  },
  { // Entry 386
    0x1.778d27690518de7a48ef0f2a0a871bb3p-27,
    0x1.00a436e9442ebp-27,
    0x1.122dc42e12484p-27
  },
  { // Entry 387
    0x1.778d27690518d20ca53cbc1df3f2eff8p-27,
    0x1.00a436e9442ecp-27,
    0x1.122dc42e12482p-27
  },
  { // Entry 388
    0x1.778d27690518ddbb030d879eb3b8a069p-27,
    0x1.00a436e9442ecp-27,
    0x1.122dc42e12483p-27
  },
  { // Entry 389
    0x1.778d27690518e96960de531f73cfcf4fp-27,
    0x1.00a436e9442ecp-27,
    0x1.122dc42e12484p-27
  },
  { // Entry 390
    0x1.778d27690518dcfbbd2c00135e46c6d1p-27,
    0x1.00a436e9442edp-27,
    0x1.122dc42e12482p-27
  },
  { // Entry 391
    0x1.778d27690518e8aa1afccb941db56731p-27,
    0x1.00a436e9442edp-27,
    0x1.122dc42e12483p-27
  },
  { // Entry 392
    0x1.778d27690518f45878cd9714dd758605p-27,
    0x1.00a436e9442edp-27,
    0x1.122dc42e12484p-27
  },
  { // Entry 393
    0x1.74334f2872bf31f2bd78c8d32ad384a6p0,
    0x1.0b2502b3f7655p0,
    -0x1.032a74c8e2bbep0
  },
  { // Entry 394
    0x1.74334f2872bf26ceaa6e8d36067093ffp0,
    0x1.0b2502b3f7655p0,
    -0x1.032a74c8e2bbdp0
  },
  { // Entry 395
    0x1.74334f2872bf1baa97645198e2685868p0,
    0x1.0b2502b3f7655p0,
    -0x1.032a74c8e2bbcp0
  },
  { // Entry 396
    0x1.74334f2872bf3d6e9e764b9816ffdf5ep0,
    0x1.0b2502b3f7656p0,
    -0x1.032a74c8e2bbep0
  },
  { // Entry 397
    0x1.74334f2872bf324a8b6c0ffaf2f4ee3dp0,
    0x1.0b2502b3f7656p0,
    -0x1.032a74c8e2bbdp0
  },
  { // Entry 398
    0x1.74334f2872bf27267861d45dcf44b22bp0,
    0x1.0b2502b3f7656p0,
    -0x1.032a74c8e2bbcp0
  },
  { // Entry 399
    0x1.74334f2872bf48ea7f73ce5d038198c9p0,
    0x1.0b2502b3f7657p0,
    -0x1.032a74c8e2bbep0
  },
  { // Entry 400
    0x1.74334f2872bf3dc66c6992bfdfcea72ep0,
    0x1.0b2502b3f7657p0,
    -0x1.032a74c8e2bbdp0
  },
  { // Entry 401
    0x1.74334f2872bf32a2595f5722bc766aa2p0,
    0x1.0b2502b3f7657p0,
    -0x1.032a74c8e2bbcp0
  },
  { // Entry 402
    0x1.b6d63492cf6c5f0d4a9e41a4ed8f8b94p7,
    0x1.3845636425763p7,
    0x1.3453456452664p7
  },
  { // Entry 403
    0x1.b6d63492cf6c6a4b20bba441a2ca5ba3p7,
    0x1.3845636425763p7,
    0x1.3453456452665p7
  },
  { // Entry 404
    0x1.b6d63492cf6c7588f6d906de5850ca51p7,
    0x1.3845636425763p7,
    0x1.3453456452666p7
  },
  { // Entry 405
    0x1.b6d63492cf6c6a6ff4ee83c89e71f86dp7,
    0x1.3845636425764p7,
    0x1.3453456452664p7
  },
  { // Entry 406
    0x1.b6d63492cf6c75adcb0be66553621e7ap7,
    0x1.3845636425764p7,
    0x1.3453456452665p7
  },
  { // Entry 407
    0x1.b6d63492cf6c80eba1294902089de325p7,
    0x1.3845636425764p7,
    0x1.3453456452666p7
  },
  { // Entry 408
    0x1.b6d63492cf6c75d29f3ec5ec4f9e1dc5p7,
    0x1.3845636425765p7,
    0x1.3453456452664p7
  },
  { // Entry 409
    0x1.b6d63492cf6c8110755c2889044399cfp7,
    0x1.3845636425765p7,
    0x1.3453456452665p7
  },
  { // Entry 410
    0x1.b6d63492cf6c8c4e4b798b25b934b477p7,
    0x1.3845636425765p7,
    0x1.3453456452666p7
  },
  { // Entry 411
    0x1.b6d63492cf6c8c4e4b798b25b934b477p-6,
    -0x1.3845636425765p-6,
    -0x1.3453456452666p-6
  },
  { // Entry 412
    0x1.b6d63492cf6c8110755c2889044399cfp-6,
    -0x1.3845636425765p-6,
    -0x1.3453456452665p-6
  },
  { // Entry 413
    0x1.b6d63492cf6c75d29f3ec5ec4f9e1dc5p-6,
    -0x1.3845636425765p-6,
    -0x1.3453456452664p-6
  },
  { // Entry 414
    0x1.b6d63492cf6c80eba1294902089de325p-6,
    -0x1.3845636425764p-6,
    -0x1.3453456452666p-6
  },
  { // Entry 415
    0x1.b6d63492cf6c75adcb0be66553621e7ap-6,
    -0x1.3845636425764p-6,
    -0x1.3453456452665p-6
  },
  { // Entry 416
    0x1.b6d63492cf6c6a6ff4ee83c89e71f86dp-6,
    -0x1.3845636425764p-6,
    -0x1.3453456452664p-6
  },
  { // Entry 417
    0x1.b6d63492cf6c7588f6d906de5850ca51p-6,
    -0x1.3845636425763p-6,
    -0x1.3453456452666p-6
  },
  { // Entry 418
    0x1.b6d63492cf6c6a4b20bba441a2ca5ba3p-6,
    -0x1.3845636425763p-6,
    -0x1.3453456452665p-6
  },
  { // Entry 419
    0x1.b6d63492cf6c5f0d4a9e41a4ed8f8b94p-6,
    -0x1.3845636425763p-6,
    -0x1.3453456452664p-6
  },
  { // Entry 420
    0x1.9a134186a4136915d6a2f7171812deefp-16,
    -0x1.3845636425765p-16,
    -0x1.09cc3d7f1c881p-16
  },
  { // Entry 421
    0x1.9a134186a4135eb6f0c519097d75243dp-16,
    -0x1.3845636425765p-16,
    -0x1.09cc3d7f1c880p-16
  },
  { // Entry 422
    0x1.9a134186a41354580ae73afbe33415cfp-16,
    -0x1.3845636425765p-16,
    -0x1.09cc3d7f1c87fp-16
  },
  { // Entry 423
    0x1.9a134186a4135ce6bf291d40fc6e4392p-16,
    -0x1.3845636425764p-16,
    -0x1.09cc3d7f1c881p-16
  },
  { // Entry 424
    0x1.9a134186a4135287d94b3f336181a757p-16,
    -0x1.3845636425764p-16,
    -0x1.09cc3d7f1c880p-16
  },
  { // Entry 425
    0x1.9a134186a4134828f36d6125c6f1b75fp-16,
    -0x1.3845636425764p-16,
    -0x1.09cc3d7f1c87fp-16
  },
  { // Entry 426
    0x1.9a134186a41350b7a7af436ae10ccc7ap-16,
    -0x1.3845636425763p-16,
    -0x1.09cc3d7f1c881p-16
  },
  { // Entry 427
    0x1.9a134186a4134658c1d1655d45d14eb5p-16,
    -0x1.3845636425763p-16,
    -0x1.09cc3d7f1c880p-16
  },
  { // Entry 428
    0x1.9a134186a4133bf9dbf3874faaf27d32p-16,
    -0x1.3845636425763p-16,
    -0x1.09cc3d7f1c87fp-16
  },
  { // Entry 429
    0x1.6a09e667f3bcdfa9516192a2b726086dp-6,
    -0x1.0000000000001p-6,
    -0x1.0000000000001p-6
  },
  { // Entry 430
    0x1.6a09e667f3bcd459022e5304d10b0412p-6,
    -0x1.0000000000001p-6,
    -0x1.0p-6
  },
  { // Entry 431
    0x1.6a09e667f3bcceb0da94b335de1f72d2p-6,
    -0x1.0000000000001p-6,
    -0x1.fffffffffffffp-7
  },
  { // Entry 432
    0x1.6a09e667f3bcd459022e5304d10b0412p-6,
    -0x1.0p-6,
    -0x1.0000000000001p-6
  },
  { // Entry 433
    0x1.6a09e667f3bcc908b2fb1366ea957d3ep-6,
    -0x1.0p-6,
    -0x1.0p-6
  },
  { // Entry 434
    0x1.6a09e667f3bcc3608b617397f77caac1p-6,
    -0x1.0p-6,
    -0x1.fffffffffffffp-7
  },
  { // Entry 435
    0x1.6a09e667f3bcceb0da94b335de1f72d2p-6,
    -0x1.fffffffffffffp-7,
    -0x1.0000000000001p-6
  },
  { // Entry 436
    0x1.6a09e667f3bcc3608b617397f77caac1p-6,
    -0x1.fffffffffffffp-7,
    -0x1.0p-6
  },
  { // Entry 437
    0x1.6a09e667f3bcbdb863c7d3c9044d37a6p-6,
    -0x1.fffffffffffffp-7,
    -0x1.fffffffffffffp-7
  },
  { // Entry 438
    0x1.6a09e667f3bcc908b2fb1366ea957d3ep-1074,
    -0x1.0p-1074,
    -0x1.0p-1074
  },
  { // Entry 439
    0x1.p-1074,
    -0x1.0p-1074,
    -0.0
  },
  { // Entry 440
    0x1.6a09e667f3bcc908b2fb1366ea957d3ep-1074,
    -0x1.0p-1074,
    0x1.0p-1074
  },
  { // Entry 441
    0x1.p-1074,
    -0.0,
    -0x1.0p-1074
  },
  { // Entry 442
    0.0,
    -0.0,
    -0.0
  },
  { // Entry 443
    0x1.p-1074,
    -0.0,
    0x1.0p-1074
  },
  { // Entry 444
    0x1.6a09e667f3bcc908b2fb1366ea957d3ep-1074,
    0x1.0p-1074,
    -0x1.0p-1074
  },
  { // Entry 445
    0x1.p-1074,
    0x1.0p-1074,
    -0.0
  },
  { // Entry 446
    0x1.6a09e667f3bcc908b2fb1366ea957d3ep-1074,
    0x1.0p-1074,
    0x1.0p-1074
  },
  { // Entry 447
    0x1.1e3779b97f4a732437cef466090d1897p-400,
    0x1.fffffffffffffp-401,
    0x1.fffffffffffffp-402
  },
  { // Entry 448
    0x1.1e3779b97f4a74ee2a2b5064e6a62857p-400,
    0x1.fffffffffffffp-401,
    0x1.0p-401
  },
  { // Entry 449
    0x1.1e3779b97f4a78820ee40862a1faa06cp-400,
    0x1.fffffffffffffp-401,
    0x1.0000000000001p-401
  },
  { // Entry 450
    0x1.1e3779b97f4a7a4c014064617f602b4fp-400,
    0x1.0p-400,
    0x1.fffffffffffffp-402
  },
  { // Entry 451
    0x1.1e3779b97f4a7c15f39cc0605cedc834p-400,
    0x1.0p-400,
    0x1.0p-401
  },
  { // Entry 452
    0x1.1e3779b97f4a7fa9d855785e182b5a91p-400,
    0x1.0p-400,
    0x1.0000000000001p-401
  },
  { // Entry 453
    0x1.1e3779b97f4a889b942344586c28a953p-400,
    0x1.0000000000001p-400,
    0x1.fffffffffffffp-402
  },
  { // Entry 454
    0x1.1e3779b97f4a8a65867fa057499f6080p-400,
    0x1.0000000000001p-400,
    0x1.0p-401
  },
  { // Entry 455
    0x1.1e3779b97f4a8df96b38585504af276dp-400,
    0x1.0000000000001p-400,
    0x1.0000000000001p-401
  },
  { // Entry 456
    0x1.1e3779b97f4a732437cef466090d1897p-511,
    0x1.fffffffffffffp-513,
    0x1.fffffffffffffp-512
  },
  { // Entry 457
    0x1.1e3779b97f4a7a4c014064617f602b4fp-511,
    0x1.fffffffffffffp-513,
    0x1.0p-511
  },
  { // Entry 458
    0x1.1e3779b97f4a889b942344586c28a953p-511,
    0x1.fffffffffffffp-513,
    0x1.0000000000001p-511
  },
  { // Entry 459
    0x1.1e3779b97f4a74ee2a2b5064e6a62857p-511,
    0x1.0p-512,
    0x1.fffffffffffffp-512
  },
  { // Entry 460
    0x1.1e3779b97f4a7c15f39cc0605cedc834p-511,
    0x1.0p-512,
    0x1.0p-511
  },
  { // Entry 461
    0x1.1e3779b97f4a8a65867fa057499f6080p-511,
    0x1.0p-512,
    0x1.0000000000001p-511
  },
  { // Entry 462
    0x1.1e3779b97f4a78820ee40862a1faa06cp-511,
    0x1.0000000000001p-512,
    0x1.fffffffffffffp-512
  },
  { // Entry 463
    0x1.1e3779b97f4a7fa9d855785e182b5a91p-511,
    0x1.0000000000001p-512,
    0x1.0p-511
  },
  { // Entry 464
    0x1.1e3779b97f4a8df96b38585504af276dp-511,
    0x1.0000000000001p-512,
    0x1.0000000000001p-511
  },
  { // Entry 465
    0x1.1e3779b97f4a732437cef466090d1897p1022,
    0x1.fffffffffffffp1021,
    0x1.fffffffffffffp1020
  },
  { // Entry 466
    0x1.1e3779b97f4a74ee2a2b5064e6a62857p1022,
    0x1.fffffffffffffp1021,
    0x1.0p1021
  },
  { // Entry 467
    0x1.1e3779b97f4a78820ee40862a1faa06cp1022,
    0x1.fffffffffffffp1021,
    0x1.0000000000001p1021
  },
  { // Entry 468
    0x1.1e3779b97f4a7a4c014064617f602b4fp1022,
    0x1.0p1022,
    0x1.fffffffffffffp1020
  },
  { // Entry 469
    0x1.1e3779b97f4a7c15f39cc0605cedc834p1022,
    0x1.0p1022,
    0x1.0p1021
  },
  { // Entry 470
    0x1.1e3779b97f4a7fa9d855785e182b5a91p1022,
    0x1.0p1022,
    0x1.0000000000001p1021
  },
  { // Entry 471
    0x1.1e3779b97f4a889b942344586c28a953p1022,
    0x1.0000000000001p1022,
    0x1.fffffffffffffp1020
  },
  { // Entry 472
    0x1.1e3779b97f4a8a65867fa057499f6080p1022,
    0x1.0000000000001p1022,
    0x1.0p1021
  },
  { // Entry 473
    0x1.1e3779b97f4a8df96b38585504af276dp1022,
    0x1.0000000000001p1022,
    0x1.0000000000001p1021
  },
  { // Entry 474
    HUGE_VAL,
    HUGE_VAL,
    HUGE_VAL
  },
  { // Entry 475
    HUGE_VAL,
    HUGE_VAL,
    0x1.fffffffffffffp1023
  },
  { // Entry 476
    HUGE_VAL,
    HUGE_VAL,
    0x1.0p-1022
  },
  { // Entry 477
    HUGE_VAL,
    HUGE_VAL,
    0x1.0p-1074
  },
  { // Entry 478
    HUGE_VAL,
    HUGE_VAL,
    0.0
  },
  { // Entry 479
    HUGE_VAL,
    HUGE_VAL,
    -0.0
  },
  { // Entry 480
    HUGE_VAL,
    HUGE_VAL,
    -0x1.0p-1074
  },
  { // Entry 481
    HUGE_VAL,
    HUGE_VAL,
    -0x1.0p-1022
  },
  { // Entry 482
    HUGE_VAL,
    HUGE_VAL,
    -0x1.fffffffffffffp1023
  },
  { // Entry 483
    HUGE_VAL,
    HUGE_VAL,
    -HUGE_VAL
  },
  { // Entry 484
    HUGE_VAL,
    -HUGE_VAL,
    HUGE_VAL
  },
  { // Entry 485
    HUGE_VAL,
    -HUGE_VAL,
    0x1.fffffffffffffp1023
  },
  { // Entry 486
    HUGE_VAL,
    -HUGE_VAL,
    0x1.0p-1022
  },
  { // Entry 487
    HUGE_VAL,
    -HUGE_VAL,
    0x1.0p-1074
  },
  { // Entry 488
    HUGE_VAL,
    -HUGE_VAL,
    0.0
  },
  { // Entry 489
    HUGE_VAL,
    -HUGE_VAL,
    -0.0
  },
  { // Entry 490
    HUGE_VAL,
    -HUGE_VAL,
    -0x1.0p-1074
  },
  { // Entry 491
    HUGE_VAL,
    -HUGE_VAL,
    -0x1.0p-1022
  },
  { // Entry 492
    HUGE_VAL,
    -HUGE_VAL,
    -0x1.fffffffffffffp1023
  },
  { // Entry 493
    HUGE_VAL,
    -HUGE_VAL,
    -HUGE_VAL
  },
  { // Entry 494
    HUGE_VAL,
    0x1.fffffffffffffp1023,
    HUGE_VAL
  },
  { // Entry 495
    HUGE_VAL,
    0x1.0p-1022,
    HUGE_VAL
  },
  { // Entry 496
    HUGE_VAL,
    0x1.0p-1074,
    HUGE_VAL
  },
  { // Entry 497
    HUGE_VAL,
    0.0,
    HUGE_VAL
  },
  { // Entry 498
    HUGE_VAL,
    -0.0,
    HUGE_VAL
  },
  { // Entry 499
    HUGE_VAL,
    -0x1.0p-1074,
    HUGE_VAL
  },
  { // Entry 500
    HUGE_VAL,
    -0x1.0p-1022,
    HUGE_VAL
  },
  { // Entry 501
    HUGE_VAL,
    -0x1.fffffffffffffp1023,
    HUGE_VAL
  },
  { // Entry 502
    HUGE_VAL,
    -HUGE_VAL,
    HUGE_VAL
  },
  { // Entry 503
    HUGE_VAL,
    HUGE_VAL,
    -HUGE_VAL
  },
  { // Entry 504
    HUGE_VAL,
    0x1.fffffffffffffp1023,
    -HUGE_VAL
  },
  { // Entry 505
    HUGE_VAL,
    0x1.0p-1022,
    -HUGE_VAL
  },
  { // Entry 506
    HUGE_VAL,
    0x1.0p-1074,
    -HUGE_VAL
  },
  { // Entry 507
    HUGE_VAL,
    0.0,
    -HUGE_VAL
  },
  { // Entry 508
    HUGE_VAL,
    -0.0,
    -HUGE_VAL
  },
  { // Entry 509
    HUGE_VAL,
    -0x1.0p-1074,
    -HUGE_VAL
  },
  { // Entry 510
    HUGE_VAL,
    -0x1.0p-1022,
    -HUGE_VAL
  },
  { // Entry 511
    HUGE_VAL,
    -0x1.fffffffffffffp1023,
    -HUGE_VAL
  },
  { // Entry 512
    0x1.fffffffffffff0p1023,
    0.0,
    0x1.fffffffffffffp1023
  },
  { // Entry 513
    0x1.p-1022,
    0.0,
    0x1.0p-1022
  },
  { // Entry 514
    0x1.p-1074,
    0.0,
    0x1.0p-1074
  },
  { // Entry 515
    0.0,
    0.0,
    0.0
  },
  { // Entry 516
    0.0,
    0.0,
    -0.0
  },
  { // Entry 517
    0x1.p-1074,
    0.0,
    -0x1.0p-1074
  },
  { // Entry 518
    0x1.p-1022,
    0.0,
    -0x1.0p-1022
  },
  { // Entry 519
    0x1.fffffffffffff0p1023,
    0.0,
    -0x1.fffffffffffffp1023
  },
  { // Entry 520
    0x1.fffffffffffff0p1023,
    -0.0,
    0x1.fffffffffffffp1023
  },
  { // Entry 521
    0x1.p-1022,
    -0.0,
    0x1.0p-1022
  },
  { // Entry 522
    0x1.p-1074,
    -0.0,
    0x1.0p-1074
  },
  { // Entry 523
    0.0,
    -0.0,
    0.0
  },
  { // Entry 524
    0.0,
    -0.0,
    -0.0
  },
  { // Entry 525
    0x1.p-1074,
    -0.0,
    -0x1.0p-1074
  },
  { // Entry 526
    0x1.p-1022,
    -0.0,
    -0x1.0p-1022
  },
  { // Entry 527
    0x1.fffffffffffff0p1023,
    -0.0,
    -0x1.fffffffffffffp1023
  },
  { // Entry 528
    0x1.fffffffffffff0p1023,
    0x1.fffffffffffffp1023,
    0.0
  },
  { // Entry 529
    0x1.p-1022,
    0x1.0p-1022,
    0.0
  },
  { // Entry 530
    0x1.p-1074,
    0x1.0p-1074,
    0.0
  },
  { // Entry 531
    0x1.p-1074,
    -0x1.0p-1074,
    0.0
  },
  { // Entry 532
    0x1.p-1022,
    -0x1.0p-1022,
    0.0
  },
  { // Entry 533
    0x1.fffffffffffff0p1023,
    -0x1.fffffffffffffp1023,
    0.0
  },
  { // Entry 534
    0x1.fffffffffffff0p1023,
    0x1.fffffffffffffp1023,
    -0.0
  },
  { // Entry 535
    0x1.p-1022,
    0x1.0p-1022,
    -0.0
  },
  { // Entry 536
    0x1.p-1074,
    0x1.0p-1074,
    -0.0
  },
  { // Entry 537
    0x1.p-1074,
    -0x1.0p-1074,
    -0.0
  },
  { // Entry 538
    0x1.p-1022,
    -0x1.0p-1022,
    -0.0
  },
  { // Entry 539
    0x1.fffffffffffff0p1023,
    -0x1.fffffffffffffp1023,
    -0.0
  },
  { // Entry 540
    HUGE_VAL,
    0x1.fffffffffffffp1023,
    0x1.fffffffffffffp1023
  },
  { // Entry 541
    0x1.fffffffffffff0p1023,
    0x1.fffffffffffffp1023,
    0x1.0p-1022
  },
  { // Entry 542
    0x1.fffffffffffff0p1023,
    0x1.fffffffffffffp1023,
    0x1.0p-1074
  },
  { // Entry 543
    0x1.fffffffffffff0p1023,
    0x1.fffffffffffffp1023,
    -0x1.0p-1074
  },
  { // Entry 544
    0x1.fffffffffffff0p1023,
    0x1.fffffffffffffp1023,
    -0x1.0p-1022
  },
  { // Entry 545
    HUGE_VAL,
    0x1.fffffffffffffp1023,
    -0x1.fffffffffffffp1023
  },
  { // Entry 546
    0x1.6a09e667f3bcc908b2fb1366ea957d3ep-1022,
    0x1.0p-1022,
    0x1.0p-1022
  },
  { // Entry 547
    0x1.000000000000000000000000007fffffp-1022,
    0x1.0p-1022,
    0x1.0p-1074
  },
  { // Entry 548
    0x1.6a09e667f3bcc908b2fb1366ea957d3ep-1074,
    0x1.0p-1074,
    0x1.0p-1074
  },
  { // Entry 549
    0x1.6a09e667f3bcc908b2fb1366ea957d3ep-1074,
    -0x1.0p-1074,
    0x1.0p-1074
  },
  { // Entry 550
    0x1.40p2,
    0x1.8p1,
    0x1.0p2
  },
  { // Entry 551
    0x1.40p2,
    0x1.8p1,
    -0x1.0p2
  },
  { // Entry 552
    0x1.a0p3,
    0x1.4p2,
    0x1.8p3
  },
  { // Entry 553
    0x1.a0p3,
    0x1.4p2,
    -0x1.8p3
  }
};
