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

#include <math.h>

#include <gtest/gtest.h>

#if defined(__BIONIC__)
typedef struct {
  double expected;
  double call_data;
} exp_intel_data_t;

static exp_intel_data_t g_exp_intel_data[] = {
  { // Entry 0
    0x1.0000000000001fffffffffffffffffffp0,
    0x1.ffffffffffffep-52
  },
  { // Entry 1
    0x1.000000000011ffffffffffffffffffffp0,
    0x1.1ffffffffff5ep-44
  },
  { // Entry 2
    0x1.000000000207fffffffffffffffffd34p0,
    0x1.03fffffffef7fp-39
  },
  { // Entry 3
    0x1.00000000c1b59800000000000027a7d5p0,
    0x1.836b2fff6d6cbp-33
  },
  { // Entry 4
    0x1.000000033d397800000000000002a51dp0,
    0x1.9e9cbbfd6080bp-31
  },
  { // Entry 5
    0x1.003af6c37c1d3000000000000009446ep0,
    0x1.d77fd13d27fffp-11
  },
  { // Entry 6
    0x1.016b4df3299d77ffffffffffffe83e10p0,
    0x1.6a4d1af9cc989p-8
  },
  { // Entry 7
    0x1.0ca4a41663fe07ffffffffffffdab235p0,
    0x1.8ae823850230bp-5
  },
  { // Entry 8
    0x1.1538ea18a4585000000000000039e054p0,
    0x1.46370d915991bp-4
  },
  { // Entry 9
    0x1.47408cb9583ce00000000000002c896fp0,
    0x1.f6e4c3ced7c72p-3
  },
  { // Entry 10
    0x1.27c2e4bc1ee707ffffffffffffeb0c2dp1,
    0x1.accfbe46b4ef0p-1
  },
  { // Entry 11
    0x1.557d4acd7e5568000000000000202c86p2,
    0x1.aca7ae8da5a7bp0
  },
  { // Entry 12
    0x1.91a8dff540ff700000000000002f1e33p2,
    0x1.d6336a88077aap0
  },
  { // Entry 13
    0x1.8ede492d96071fffffffffffff0aecd2p3,
    0x1.42ee3c7dc4946p1
  },
  { // Entry 14
    0x1.ac50b409c8aee0000000000000022ce3p8,
    0x1.83d4bcdebb3f4p2
  },
  { // Entry 15
    0x1.f3e558cf4de5400000000000001f5808p-23,
    -0x1.e8bdbfcd9144ep3
  },
  { // Entry 16
    0x1.951c6dc5d24e27ffffffffffffb6e0c3p-9,
    -0x1.71e0b869b5e79p2
  },
  { // Entry 17
    0x1.1064b2c103dda80000000000003eb5d7p-3,
    -0x1.02393d5976769p1
  },
  { // Entry 18
    0x1.3ef1e9b3a81c7fffffffffffffc315p-2,
    -0x1.2a9cad9998262p0
  },
  { // Entry 19
    0x1.534d4de87071300000000000003c6fb7p-3,
    -0x1.cc37ef7de7501p0
  },
  { // Entry 20
    0x1.2217147b85ea980000000000007745a3p-1,
    -0x1.22e24fa3d5cf9p-1
  },
  { // Entry 21
    0x1.9403fd0ee51c800000000000007720f1p-2,
    -0x1.dc2b5df1f7d3dp-1
  },
  { // Entry 22
    0x1.baded30cbf1c3ffffffffffffffbd904p-1,
    -0x1.290ea09e36479p-3
  },
  { // Entry 23
    0x1.ffe5d0bb7eabf0000000000000030320p-1,
    -0x1.a2fefefd580dfp-13
  },
  { // Entry 24
    0x1.ffffff84b39c4fffffffffffffff17f4p-1,
    -0x1.ed318efb627eap-27
  },
  { // Entry 25
    0x1.fffffffad0ae6800000000000009f1a8p-1,
    -0x1.4bd46601ae1efp-31
  },
  { // Entry 26
    0x1.ffffffffff7000000000000000000003p-1,
    -0x1.2000000000288p-42
  },
  { // Entry 27
    0x1.fffffffffffdp-1,
    -0x1.8000000000012p-48
  },
  { // Entry 28
    0x1.ffffffffffffc0p-1,
    -0x1.0000000000001p-51
  },
  { // Entry 29
    0x1.0000000000000fffffffffffffffffffp0,
    0x1.fffffffffffffp-53
  },
  { // Entry 30
    0x1.000000000001ffffffffffffffffffffp0,
    0x1.fffffffffffe0p-48
  },
  { // Entry 31
    0x1.000000017ffe80000000000000035ffdp0,
    0x1.7ffe7ffee0024p-32
  },
  { // Entry 32
    0x1.0000000180017ffffffffffffffc9ffdp0,
    0x1.80017ffedffdcp-32
  },
  { // Entry 33
    0x1.00000075e9f6400000000000000b1e80p0,
    0x1.d7a7d893609e5p-26
  },
  { // Entry 34
    0x1.0006e83736f8c80000000000000aa0afp0,
    0x1.ba07d73250de7p-14
  },
  { // Entry 35
    0x1.de7cd6751029a0000000000000e8bb2fp16,
    0x1.76e7e5d7b6eacp3
  },
  { // Entry 36
    0x1.1d71965f516ad80000000000009f7e97p19,
    0x1.a8ead058bc6b8p3
  },
  { // Entry 37
    0x1.a8c02e974c314fffffffffffffe21f03p25,
    0x1.1d5c2daebe367p4
  },
  { // Entry 38
    0x1.b890ca8637ae1fffffffffffffe42161p40,
    0x1.c44ce0d716a1ap4
  },
  { // Entry 39
    0x1.f03f56a88b5d781c905f6b51c76ae981p-1,
    -0x1.0000000000001p-5
  },
  { // Entry 40
    0x1.ff003ff556aa87ebb06e0eb57287567fp-1,
    -0x1.000000000000ap-9
  },
  { // Entry 41
    0x1.8ebef9eac81fc8001a3dbd88248efe13p-1,
    -0x1.0000000000025p-2
  },
  { // Entry 42
    0x1.fc03fd56a469b800008bcdfe018dbc74p-1,
    -0x1.0000000171051p-7
  },
  { // Entry 43
    0x1.78b5612dbee010008d3bc0735c80f7a8p-2,
    -0x1.0000018p0
  },
  { // Entry 44
    0x1.969c870ea9f5a801ddc1ff08a008db09p-93,
    -0x1.000001e3ep6
  },
  { // Entry 45
    0x1.78af867bc511d321cd454dbddaf806bcp-2,
    -0x1.0003fc0p0
  },
  { // Entry 46
    0x1.fffffffffefff8000000000400100015p-1,
    -0x1.00080000004p-41
  },
  { // Entry 47
    0x1.789214093c81d3b65fac659fb3fc8249p-2,
    -0x1.00180p0
  },
  { // Entry 48
    0x1.8e298e52fcce88014ad95c4f3bd88765p-1,
    -0x1.018p-2
  },
  { // Entry 49
    0x1.aa6ffb0ba519f801f9c3e146a15dc02cp-24,
    -0x1.02010p4
  },
  { // Entry 50
    0x1.dd7a46b8d85d67d67359337109c25168p-376,
    -0x1.040p8
  },
  { // Entry 51
    0x1.33e96ca3bcf3e801f9b3f6126fc60699p-1,
    -0x1.045b948724130p-1
  },
  { // Entry 52
    0x1.ef2f652dc26057fffc98aa54d4c33c16p-1,
    -0x1.118e05cfa3e80p-5
  },
  { // Entry 53
    0x1.a5d2ce977bbe28048b316927f970c201p-7,
    -0x1.169156e17b9b9p2
  },
  { // Entry 54
    0x1.a0207cf4190f700452282b04fa390407p-7,
    -0x1.17701b3bf0502p2
  },
  { // Entry 55
    0x1.fb97e1ff8cbba7adf16f3fbd6878f560p-1,
    -0x1.1b4p-7
  },
  { // Entry 56
    0x1.bcff09ae30c46804a6fafd550766eed2p-1,
    -0x1.1f4p-3
  },
  { // Entry 57
    0x1.fee70cf5e86ef4e54d0b1fa2701a346fp-833,
    -0x1.2059ad42c3cf7p9
  },
  { // Entry 58
    0x1.7ccd3f33407458011470b3f67197a9c8p-1,
    -0x1.2f269d2ca38d2p-2
  },
  { // Entry 59
    0x1.09cdb36e977bc7ffffe6d4ddbb18d553p-14,
    -0x1.3553cf1828b0bp3
  },
  { // Entry 60
    0x1.f9dc5cd546d9b800dc3d95519dc67ee8p-449,
    -0x1.368ac083128fcp8
  },
  { // Entry 61
    0x1.f1c53c907f6f576e62e401315bafc69ep-15,
    -0x1.376e8f679ae16p3
  },
  { // Entry 62
    0x1.f24595954e6618028965a9d67edeaf7bp-8,
    -0x1.3844f67495f7dp2
  },
  { // Entry 63
    0x1.fb56521290a222527d1d3c3638265f46p-905,
    -0x1.394e9e1b089d7p9
  },
  { // Entry 64
    0x1.dc1658ff6e0707fafd2fd8306c4b1897p-114,
    -0x1.3997ea51e5271p6
  },
  { // Entry 65
    0x1.ec8b3c090f8b97716a3d38de46d68a1ep-1,
    -0x1.3d5de560e1906p-5
  },
  { // Entry 66
    0x1.af411bf985220b562909c652c087fef4p-951,
    -0x1.4954aa552a960p9
  },
  { // Entry 67
    0x1.e1b71d238d5077fe3bea222fcba15608p-957,
    -0x1.4b5ad6a9ad6adp9
  },
  { // Entry 68
    0x1.cc666a3519ca080f1daef57f92b41d29p-973,
    -0x1.50ec32686a834p9
  },
  { // Entry 69
    0x1.22462bd5f72b0fffa6085b04427fdf5dp-976,
    -0x1.52316872b0222p9
  },
  { // Entry 70
    0x1.c1ebc18610301fff2424bc3936260dd2p-981,
    -0x1.53b4ed3b4ec77p9
  },
  { // Entry 71
    0x1.8d015bcaf9e18b57fdf057239d94f586p-981,
    -0x1.53c4f13c4f079p9
  },
  { // Entry 72
    0x1.fd5835460ed48c4e14207309a1237f90p-989,
    -0x1.566ad4a41bec0p9
  },
  { // Entry 73
    0x1.58733fbc088ef7ff38e15f94166c022cp-994,
    -0x1.585883e7b3b38p9
  },
  { // Entry 74
    0x1.b052090690481802acf0169f62bf32c9p-32,
    -0x1.5a81ecf68beccp4
  },
  { // Entry 75
    0x1.df6154bc644dac0f9e3a56646948b7c5p-1015,
    -0x1.5f75629af4eb7p9
  },
  { // Entry 76
    0x1.ff4b0da37083502c114a7ac56ac0498fp-1023,
    -0x1.6232eb1c432f2p9
  },
  { // Entry 77
    0x1.625edd4c7513e842f27a931c2bbe007cp-1023,
    -0x1.6261d7dbf48ccp9
  },
  { // Entry 78
    0x1.d572d24973be20006bad92e29bcba984p-1,
    -0x1.6364b8f747e32p-4
  },
  { // Entry 79
    0x1.ee4dd792245778020250f8981d1a0ad1p-9,
    -0x1.6524796b40895p2
  },
  { // Entry 80
    0x1.f660c59b294505d8989f85f8ed2a1878p-5,
    -0x1.6551daae369cep1
  },
  { // Entry 81
    0x1.aa070449efffe9b910657898cdc0ccddp-1037,
    -0x1.672463f141607p9
  },
  { // Entry 82
    0x1.d4e7fc331b7327ffccf4ee0c59aa7b22p-1,
    -0x1.6820c99c1dc16p-4
  },
  { // Entry 83
    0x1.fcc448acf476575c66ee396912dbaacdp-528,
    -0x1.6d4b7e82c3f02p8
  },
  { // Entry 84
    0x1.7fff5a1bffcb2062c9aaa5b07d097255p-529,
    -0x1.6e44f765fda76p8
  },
  { // Entry 85
    0x1.fa4ecd8ae57187fffffff9910c476c64p-1,
    -0x1.6e56e725b8304p-7
  },
  { // Entry 86
    0x1.e8d2dafd017ce8032bcb89459464fe7dp-3,
    -0x1.6ecp0
  },
  { // Entry 87
    0x1.214e34caac9e67139688ad1632dd2055p-1062,
    -0x1.7p9
  },
  { // Entry 88
    0x1.c87f21775a482a09a44af59c3c3ae44bp-1071,
    -0x1.72e42p9
  },
  { // Entry 89
    0x1.a349b2329c6777ffce65934b3b6203a7p-273,
    -0x1.7978bac71121cp7
  },
  { // Entry 90
    0x1.d2a6d5ea995c17ab448132e1e0453805p-1,
    -0x1.7bde79e0f970cp-4
  },
  { // Entry 91
    0x1.b776dc64c76d197e8c644dc8cb509c4ap-3,
    -0x1.8a0p0
  },
  { // Entry 92
    0x1.4446fcf7da689802d8c725db8c1145afp-72,
    -0x1.8d5c84f0bac8cp5
  },
  { // Entry 93
    0x1.e72a0b68bb82b78ce0524d1abb6f7abep-73,
    -0x1.8fa68b4447230p5
  },
  { // Entry 94
    0x1.fc33d5e1ca6df7d5069b03dc5a683e4fp-75,
    -0x1.9a671693b946bp5
  },
  { // Entry 95
    0x1.e6981fd6ef79c7fffffcc7201f82fd49p-1,
    -0x1.a0ec89f897a75p-5
  },
  { // Entry 96
    0x1.f2b9d3b6cf2277a1283f8e00743a0669p-1,
    -0x1.ae6p-6
  },
  { // Entry 97
    0x1.e5034c8d191bbfff719f2098e33c0577p-1,
    -0x1.bb9706e2f1bb4p-5
  },
  { // Entry 98
    0x1.f24dc90f9fbeb8028fbcaeebabc7e9fcp-1,
    -0x1.bc3edad2e1efbp-6
  },
  { // Entry 99
    0x1.71f237d64a6eb46aa154476fd0fc3886p-11,
    -0x1.d0697edbe0052p2
  },
  { // Entry 100
    0x1.f12c0d3addaa48004b23516d2a113804p-1,
    -0x1.e18p-6
  },
  { // Entry 101
    0x1.f110dd9073d71800bb4056830b517b08p-1,
    -0x1.e50p-6
  },
  { // Entry 102
    0x1.f3aa7a860574c2b0783061fe1fd1df2ep-705,
    -0x1.e7fffffffffffp8
  },
  { // Entry 103
    0x1.f0f898d55117081fc3b8792205e4e130p-1,
    -0x1.e82p-6
  },
  { // Entry 104
    0x1.ff814fff18dac854185d019b0f8ead36p-1,
    -0x1.fafebfafebfb0p-11
  },
  { // Entry 105
    0x1.ffffffff01c0701c46208c5416d943adp-1,
    -0x1.fc7f1fc7f20p-34
  },
  { // Entry 106
    0x1.7907d4148bd91687a1b50f26e8830775p-2,
    -0x1.ff8ffffffffffp-1
  },
  { // Entry 107
    0x1.ffffffffffff801c0000000013f900c3p-1,
    -0x1.ff8ffffffffffp-51
  },
  { // Entry 108
    0x1.e0fbb03a0c27d7e84804cadec377bdf9p-1,
    -0x1.ffeffffffffffp-5
  },
  { // Entry 109
    0x1.ffffffffff00080000004004000ffff5p-1,
    -0x1.ffeffffffffffp-42
  },
  { // Entry 110
    0x1.ffffffffff0007fffffffffc00100015p-1,
    -0x1.fff00000008p-42
  },
  { // Entry 111
    0x1.ffffffffffffffffffffffffe000719ep-1,
    -0x1.fff8e61eadd48p-101
  },
  { // Entry 112
    0x1.fffffffffc00080000040010000ffd55p-1,
    -0x1.fffbfffffffffp-40
  },
  { // Entry 113
    0x1.fffffffffc00001c0004001fc7fffe19p-1,
    -0x1.fffff1fffffffp-40
  },
  { // Entry 114
    0x1.ff800ffeaac008589f9a96af73eaff67p-1,
    -0x1.ffffffffffda5p-11
  },
  { // Entry 115
    0x1.e355bbaee8d85815e4b476b704d0f4f1p-24,
    -0x1.fffffffffff7dp3
  },
  { // Entry 116
    0x1.78b56362cef3da984453d1c72c344a31p-2,
    -0x1.ffffffffffff8p-1
  },
  { // Entry 117
    0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.ffffffffffffcp-1023
  },
  { // Entry 118
    0x1.42eb9f39afbac7e5e73c32a21e853f26p-185,
    -0x1.ffffffffffffep6
  },
  { // Entry 119
    0x1.0000000000000800000000000020p0,
    0x1.0p-53
  },
  { // Entry 120
    0x1.95e54c5dd42177f53f4d5219df11ca3bp184,
    0x1.0p7
  },
  { // Entry 121
    0x1.9476504ba885758aa5fa7545e10e8e46p738,
    0x1.0000000000001p9
  },
  { // Entry 122
    0x1.0100802ab5577802ba424ad46b106a66p0,
    0x1.0000000000003p-8
  },
  { // Entry 123
    0x1.0000004000000800001caaaab1b55556p0,
    0x1.0000000000007p-26
  },
  { // Entry 124
    0x1.d8e64b8f26c128517489a490e684d8a8p2,
    0x1.000000008p1
  },
  { // Entry 125
    0x1.749ea7e015bc1dddfa19fbb3cf8bded4p11,
    0x1.000000010p3
  },
  { // Entry 126
    0x1.0f2ebd2c65d9a80081ac2e65e8025ab1p23,
    0x1.000000020p4
  },
  { // Entry 127
    0x1.0f2ec1473afea8081fa4566927a4cfd3p23,
    0x1.0000004p4
  },
  { // Entry 128
    0x1.0000000001000800000080080020002ap0,
    0x1.00080p-40
  },
  { // Entry 129
    0x1.00000000008008000000200400200005p0,
    0x1.001p-41
  },
  { // Entry 130
    0x1.000000000000080080000000002004p0,
    0x1.001p-53
  },
  { // Entry 131
    0x1.a8bf81a597edd223f2db6ebfe2eb1cc1p184,
    0x1.00173eab3623ap7
  },
  { // Entry 132
    0x1.75e54175aabb37fff5e5b44ba0f101b7p11,
    0x1.001c0p3
  },
  { // Entry 133
    0x1.60008fbed6c5280206e79909321d09c8p1,
    0x1.02f8af8af8affp0
  },
  { // Entry 134
    0x1.04184bb80ff43fffc89c53f1ee04ce04p0,
    0x1.040p-6
  },
  { // Entry 135
    0x1.00000840002208005d9600c105613e7cp0,
    0x1.080p-21
  },
  { // Entry 136
    0x1.2338ab9b7432080000000973938bacb0p0,
    0x1.08000001d0fc8p-3
  },
  { // Entry 137
    0x1.acc91bfa7c54c8036548c968b36e5750p0,
    0x1.0814419d6a65ap-1
  },
  { // Entry 138
    0x1.a4e4693413b9970755c15633af25f96bp400,
    0x1.15c18de877563p8
  },
  { // Entry 139
    0x1.000000000000480000000000061fffffp0,
    0x1.1ffffffffffffp-50
  },
  { // Entry 140
    0x1.eaa521edf1bc28014602191ce618c05fp846,
    0x1.2586ca9cf411bp9
  },
  { // Entry 141
    0x1.fca9c47016cb17617b275b298cf9f894p26,
    0x1.2b55c037ebb9dp4
  },
  { // Entry 142
    0x1.fcbb1e5c8d53575abc78726f5cea6f51p26,
    0x1.2b564bee0a6cap4
  },
  { // Entry 143
    0x1.cc0f9d911f1d1002cf0af382fdf62f02p0,
    0x1.2c2p-1
  },
  { // Entry 144
    0x1.b68a28b09fe947fdf00104f6d8c24801p6,
    0x1.2c9e33f794769p2
  },
  { // Entry 145
    0x1.d2e035b1892d6802a9c9e288d8e97c98p0,
    0x1.33a74ff06fb66p-1
  },
  { // Entry 146
    0x1.6acde844356c921fa99313aa18670593p3,
    0x1.36cce3c3925p1
  },
  { // Entry 147
    0x1.06dadee28c11c800fb9094435c1de727p7,
    0x1.38389c48b0fcep2
  },
  { // Entry 148
    0x1.c78f37f07cea6426ed5548b4a31d4ac2p904,
    0x1.3997381d7d94fp9
  },
  { // Entry 149
    0x1.bfe0c519166c47fe3a4b34e6c34b5ae4p1,
    0x1.40a339c81cecbp0
  },
  { // Entry 150
    0x1.fbc9c30603087488a3d92265b53c2a65p927,
    0x1.419dbcc486770p9
  },
  { // Entry 151
    0x1.f1ef9c8a4fdae801b19f8e42eff2c4a8p930,
    0x1.42a565e456e04p9
  },
  { // Entry 152
    0x1.da134d5a4d1e1800c436544994058ce6p934,
    0x1.44020100804p9
  },
  { // Entry 153
    0x1.f4ec44194b642801a4afd4c50633e8aap938,
    0x1.456bf23e02428p9
  },
  { // Entry 154
    0x1.ea91d9533b394801bf3d3ec8f88de568p939,
    0x1.45c1feef8086cp9
  },
  { // Entry 155
    0x1.cb419b9279b35763d113e6c5db79dc54p943,
    0x1.471c71c71c71cp9
  },
  { // Entry 156
    0x1.6d14308828321de6fe67a4136f17d1f8p29,
    0x1.474c9cc44cc25p4
  },
  { // Entry 157
    0x1.c0194b4186e2780120b873d72456ecddp956,
    0x1.4b9ab17812030p9
  },
  { // Entry 158
    0x1.15c605853476b80008cabc4b207ffd1fp0,
    0x1.4e59dc7e2b053p-4
  },
  { // Entry 159
    0x1.6dee38735a6d3800d8ec117683275f3ap972,
    0x1.510c5465d984bp9
  },
  { // Entry 160
    0x1.f1e96be2a52cd777e8cf54184ed7ae48p0,
    0x1.549b0f5742382p-1
  },
  { // Entry 161
    0x1.f253c5990aad2800004db615b1148476p0,
    0x1.55085fb86501ap-1
  },
  { // Entry 162
    0x1.279b1c43a26617ff4c7508c35e562b22p993,
    0x1.58383126e94bfp9
  },
  { // Entry 163
    0x1.6fd5ab59153f32a4a383710a3419f57dp996,
    0x1.595e5795e592bp9
  },
  { // Entry 164
    0x1.5ecf1876539547ffd612e5c4b715c8adp1004,
    0x1.5c1e0f0783c10p9
  },
  { // Entry 165
    0x1.f2a3c29fc6d723a63e349bbcd7894a8ap1008,
    0x1.5dadf5d1e452cp9
  },
  { // Entry 166
    0x1.edcb14879613e80176087c1a76dec97cp1009,
    0x1.5e056ed40e56ep9
  },
  { // Entry 167
    0x1.fcc7e5ca80b1f75c14d762f846bc1669p0,
    0x1.5fa97bb4e9060p-1
  },
  { // Entry 168
    0x1.f8b5c987a3d877e18bfcada50f1c54b0p3,
    0x1.610e47e1325dcp1
  },
  { // Entry 169
    0x1.f1e9d8cf24fcd8025127877a738eb2d1p7,
    0x1.611b19dcc53e4p2
  },
  { // Entry 170
    0x1.ffc045692e8a039c5622b8219f19c619p1023,
    0x1.62e41fffffffap9
  },
  { // Entry 171
    0x1.ffc045693009d3d065062f9267dff55ep1023,
    0x1.62e42p9
  },
  { // Entry 172
    0x1.ffc04569a2fb83679239091df068997ep1023,
    0x1.62e42000001ccp9
  },
  { // Entry 173
    0x1.fffffffffeb2a1b0e263ac68076ed4e0p1023,
    0x1.62e42fefa39eap9
  },
  { // Entry 174
    0x1.00000b40003f4800ed4e029b6b65ddb1p0,
    0x1.680p-21
  },
  { // Entry 175
    0x1.17e62f70bec99801a2936e89b54c5256p8,
    0x1.689a7dc5921b8p2
  },
  { // Entry 176
    0x1.6daaa1336a31500174ca0be569806fdep264,
    0x1.6eb1de69ace32p7
  },
  { // Entry 177
    0x1.72a9dadea7498c95a514c01d5403e5d0p0,
    0x1.7b0p-2
  },
  { // Entry 178
    0x1.1ed3fe64fc539800f9cd34ec4c83d9d1p2,
    0x1.7fffffffffff9p0
  },
  { // Entry 179
    0x1.000000000000180000000000001fffffp0,
    0x1.7ffffffffffffp-52
  },
  { // Entry 180
    0x1.fe31152b7ef6b1e0a8b9fec7ecdd85a4p553,
    0x1.8p8
  },
  { // Entry 181
    0x1.74d77b76d95163226caf84760a346abbp0,
    0x1.810p-2
  },
  { // Entry 182
    0x1.f0e3e5b6c17c02a799c306dbc1b1a8ecp556,
    0x1.820d92fc4b42ap8
  },
  { // Entry 183
    0x1.00000000c1b59800000000000027a7d5p0,
    0x1.836b2fff6d6cbp-33
  },
  { // Entry 184
    0x1.78b69d690db792b574f904d2bb1e9ebep570,
    0x1.8b7aee631f489p8
  },
  { // Entry 185
    0x1.4dad95877ae078000012827fac891535p144,
    0x1.905011e0df629p6
  },
  { // Entry 186
    0x1.fcbb8778f61217665a6e6f2119880b29p144,
    0x1.91fffp6
  },
  { // Entry 187
    0x1.7a5b2771e0a16ce6ecd6de8573c11192p4,
    0x1.94e54738fb4b7p1
  },
  { // Entry 188
    0x1.7d676a26fe5caccea59b023540bec80fp0,
    0x1.984p-2
  },
  { // Entry 189
    0x1.698764128a4448010e251b55f37a7b6ep147,
    0x1.98f381d7db44fp6
  },
  { // Entry 190
    0x1.44e5a752f9e1efffb31481484aa7692ap9,
    0x1.9e8186b267a28p2
  },
  { // Entry 191
    0x1.8023d298e372f8020ec8cf4a70affa87p0,
    0x1.9f91c1449c421p-2
  },
  { // Entry 192
    0x1.df9bd06daf8ce7b123df43bb50109938p299,
    0x1.9fc1f212d70f6p7
  },
  { // Entry 193
    0x1.81a4fbca9dcdb800000ae068bc21c4f5p0,
    0x1.a392789eafce9p-2
  },
  { // Entry 194
    0x1.825f354ccf6f2fffab7e9eb828fbc67dp0,
    0x1.a5807ca1392b2p-2
  },
  { // Entry 195
    0x1.3c1e2876834aa7fa46011e98abeadf76p0,
    0x1.bp-3
  },
  { // Entry 196
    0x1.1c7e1aca53795fff54a5fdf5529261cap0,
    0x1.b04p-4
  },
  { // Entry 197
    0x1.a2d50b8b2880e801a27a34d1954dab50p331,
    0x1.cbd917ccad096p7
  },
  { // Entry 198
    0x1.419c8a8da1b79802e20296e5f9279626p1,
    0x1.d7b677e333d38p-1
  },
  { // Entry 199
    0x1.41d1b8b4649eb8000023f9776da9d645p1,
    0x1.d80b1a6330bebp-1
  },
  { // Entry 200
    0x1.bf3447921fd397fd6db462cd2fbb39fep10,
    0x1.df50d9af9e410p2
  },
  { // Entry 201
    0x1.9919794bc86298020a19588552ceae5cp0,
    0x1.e0081ebc3264ep-2
  },
  { // Entry 202
    0x1.76a62bad715b4800daadb6fc2b6e6166p5,
    0x1.ec5bb209e5707p1
  },
  { // Entry 203
    0x1.c701554c761aa80eedf2a49ef60e562cp2,
    0x1.f62p0
  },
  { // Entry 204
    0x1.c3dda3c952b918012fd79fbb8ea38f69p727,
    0x1.f87c7820d8446p8
  },
  { // Entry 205
    0x1.d28c6097b9d5578835e6cff1b19464cdp2,
    0x1.fc89dc1aa909cp0
  },
  { // Entry 206
    0x1.4231178c2348f5f77eedb27dc404f616p734,
    0x1.fcfffffffffffp8
  },
  { // Entry 207
    0x1.04034591911e77ff1834fd70d0bbb4b0p0,
    0x1.fda6dfe27ffffp-7
  },
  { // Entry 208
    0x1.a5b5691c4b0ef803cab2bad4a2013f14p0,
    0x1.ff1dd1fffffffp-2
  },
  { // Entry 209
    0x1.0000000000003ff20000000005fc8062p0,
    0x1.ff8ffffffffffp-51
  },
  { // Entry 210
    0x1.6d4fd9ab47c9200073aa8127a2419ac8p738,
    0x1.ffe5effffffffp8
  },
  { // Entry 211
    0x1.0000000000fff80000000000001fffaap0,
    0x1.ffefffffff001p-41
  },
  { // Entry 212
    0x1.74418bce788eb46746b38b578759ecc0p11,
    0x1.ffeffffffffffp2
  },
  { // Entry 213
    0x1.0000000000fff80000007ff80020002ap0,
    0x1.fffp-41
  },
  { // Entry 214
    0x1.0000000000fffff200007ff7f200008cp0,
    0x1.ffffe3fffffffp-41
  },
  { // Entry 215
    0x1.d8e64b8d2185281f984a4e51148dadecp2,
    0x1.ffffffffe7fffp0
  },
  { // Entry 216
    0x1.p0,
    0x1.ffffffffffffcp-1023
  },
  { // Entry 217
    0x1.00000000000007ffffffffffff9fffffp0,
    0x1.ffffffffffffep-54
  },
  { // Entry 218
    0x1.00000000000007ffffffffffffdfffffp0,
    0x1.fffffffffffffp-54
  },
  { // Entry 219
    0x1.304d6aeca252531475ec9182369ba415p69,
    0x1.7ffffffffffffp5
  },
  { // Entry 220
    0x1.304d6aeca254b3af4bc5d6293d5f65c7p69,
    0x1.8p5
  },
  { // Entry 221
    0x1.304d6aeca257144a219f1ad50558d32cp69,
    0x1.8000000000001p5
  },
  { // Entry 222
    0x1.f8e6c24b558ef174995bcfad495149f5p-76,
    -0x1.a000000000001p5
  },
  { // Entry 223
    0x1.f8e6c24b5592e3421df27acf1e080144p-76,
    -0x1.ap5
  },
  { // Entry 224
    0x1.f8e6c24b5596d50fa28925f8d659c1bfp-76,
    -0x1.9ffffffffffffp5
  },
  { // Entry 225
    0x1.55779b984f395dea36a277b8bee2e64cp115,
    0x1.3ffffffffffffp6
  },
  { // Entry 226
    0x1.55779b984f3eb3c8a503b4a8e2489d98p115,
    0x1.4p6
  },
  { // Entry 227
    0x1.55779b984f4409a71364f1ae5d280e69p115,
    0x1.4000000000001p6
  },
  { // Entry 228
    0x1.07b7112bc1fbc2c0f9c1365330930310p-127,
    -0x1.6000000000001p6
  },
  { // Entry 229
    0x1.07b7112bc1ffe19d3e703e4a794f7372p-127,
    -0x1.6p6
  },
  { // Entry 230
    0x1.07b7112bc2040079831f46523d7cf692p-127,
    -0x1.5ffffffffffffp6
  },
  { // Entry 231
    0x1.40a4b9c27150866176d22f2139d1d40fp923,
    0x1.3ffffffffffffp9
  },
  { // Entry 232
    0x1.40a4b9c271789af8af205bb34f743337p923,
    0x1.4p9
  },
  { // Entry 233
    0x1.40a4b9c271a0af8fe76e8d47f7fd9c26p923,
    0x1.4000000000001p9
  },
  { // Entry 234
    0x1.44a3824e525d56fc3f02f886375ec876p-1016,
    -0x1.6000000000001p9
  },
  { // Entry 235
    0x1.44a3824e5285eb6c88cd46ba5deb457ap-1016,
    -0x1.6p9
  },
  { // Entry 236
    0x1.44a3824e52ae7fdcd2979a011280fbc8p-1016,
    -0x1.5ffffffffffffp9
  },
  { // Entry 237
    0x1.03996528e072b78a332480884c79baf7p75,
    0x1.9ffffffffffffp5
  },
  { // Entry 238
    0x1.03996528e074bebcfd76416fc2c0eb92p75,
    0x1.ap5
  },
  { // Entry 239
    0x1.03996528e076c5efc7c8025b476db0d0p75,
    0x1.a000000000001p5
  },
  { // Entry 240
    0x1.aebabae3a417ee6e7faf9ecfc951040cp-70,
    -0x1.8000000000001p5
  },
  { // Entry 241
    0x1.aebabae3a41b4be3f576e70303a37932p-70,
    -0x1.8p5
  },
  { // Entry 242
    0x1.aebabae3a41ea9596b3e2f3cf8e0d9e7p-70,
    -0x1.7ffffffffffffp5
  },
  { // Entry 243
    0x1.f1056dc7bf1b0fc857b67999f5035273p126,
    0x1.5ffffffffffffp6
  },
  { // Entry 244
    0x1.f1056dc7bf22d3de0ed57615bc501f8bp126,
    0x1.6p6
  },
  { // Entry 245
    0x1.f1056dc7bf2a97f3c5f472b093f3c91fp126,
    0x1.6000000000001p6
  },
  { // Entry 246
    0x1.7fd974d372de49099ee7bf48ae346eaap-116,
    -0x1.4000000000001p6
  },
  { // Entry 247
    0x1.7fd974d372e4486f72358acdd12690e5p-116,
    -0x1.4p6
  },
  { // Entry 248
    0x1.7fd974d372ea47d54583566af1b00056p-116,
    -0x1.3ffffffffffffp6
  },
  { // Entry 249
    0x1.93bf4ec282bd3b36cd2f4011488a8364p1015,
    0x1.5ffffffffffffp9
  },
  { // Entry 250
    0x1.93bf4ec282efb320a57f9ae02e01ae51p1015,
    0x1.6p9
  },
  { // Entry 251
    0x1.93bf4ec283222b0a7dcffbfe10b3e34ap1015,
    0x1.6000000000001p9
  },
  { // Entry 252
    0x1.98c72ca0cab14eda5aca97bee0fdd48fp-924,
    -0x1.4000000000001p9
  },
  { // Entry 253
    0x1.98c72ca0cae467bfeee3f11a4aa26f77p-924,
    -0x1.4p9
  },
  { // Entry 254
    0x1.98c72ca0cb1780a582fd50d8d0f98d8bp-924,
    -0x1.3ffffffffffffp9
  },
  { // Entry 255
    0x1.61013a44a981c910f20f80756007120ap-822,
    -0x1.1cb90bfbe8e7cp9
  },
  { // Entry 256
    0x1.8087717a7f08a7251210e71d47acc5e3p-905,
    -0x1.397217f7d1cf8p9
  },
  { // Entry 257
    0x1.a2de59d85452109f34165fa55f9e4e7bp-988,
    -0x1.562b23f3bab73p9
  },
  { // Entry 258
    0x1.fffffffffffff35793c76730080dfb9bp-2,
    -0x1.62e42fefa39f0p-1
  },
  { // Entry 259
    0x1.00000000000001abc9e3b39803f45c1cp-1,
    -0x1.62e42fefa39efp-1
  },
  { // Entry 260
    0x1.00000000000009abc9e3b3980421ba6bp-1,
    -0x1.62e42fefa39eep-1
  },
  { // Entry 261
    0x1.6a09e667f3bcc48f0965009f2778df91p-1,
    -0x1.62e42fefa39f0p-2
  },
  { // Entry 262
    0x1.6a09e667f3bcca3730fea06e1a966c06p-1,
    -0x1.62e42fefa39efp-2
  },
  { // Entry 263
    0x1.6a09e667f3bccfdf5898403d0dca9919p-1,
    -0x1.62e42fefa39eep-2
  },
  { // Entry 264
    0x1.ae89f995ad3ad33f3c451118e4ad74c5p-1,
    -0x1.62e42fefa39f0p-3
  },
  { // Entry 265
    0x1.ae89f995ad3ad69c50383c735a575052p-1,
    -0x1.62e42fefa39efp-3
  },
  { // Entry 266
    0x1.ae89f995ad3ad9f9642b67cdd007e606p-1,
    -0x1.62e42fefa39eep-3
  },
  { // Entry 267
    0x1.d5818dcfba4870ea30f8974f369eab8cp-1,
    -0x1.62e42fefa39f0p-4
  },
  { // Entry 268
    0x1.d5818dcfba4872bfb28667097f10807ep-1,
    -0x1.62e42fefa39efp-4
  },
  { // Entry 269
    0x1.d5818dcfba487495341436c3c7842af1p-1,
    -0x1.62e42fefa39eep-4
  },
  { // Entry 270
    0x1.ea4afa2a490d9797069887879b7c974ep-1,
    -0x1.62e42fefa39f0p-5
  },
  { // Entry 271
    0x1.ea4afa2a490d988c2c159cac2248a01bp-1,
    -0x1.62e42fefa39efp-5
  },
  { // Entry 272
    0x1.ea4afa2a490d99815192b1d0a915237bp-1,
    -0x1.62e42fefa39eep-5
  },
  { // Entry 273
    0x1.f50765b6e4540611e065c14f105024cep-1,
    -0x1.62e42fefa39f0p-6
  },
  { // Entry 274
    0x1.f50765b6e454068f223f2f082551b8efp-1,
    -0x1.62e42fefa39efp-6
  },
  { // Entry 275
    0x1.f50765b6e454070c64189cc13a536c5fp-1,
    -0x1.62e42fefa39eep-6
  },
  { // Entry 276
    0x1.059b0d315857435f6c51ceeb0a6b46e6p0,
    0x1.62e42fefa39eep-6
  },
  { // Entry 277
    0x1.059b0d31585743a0d3151b41203c26eep0,
    0x1.62e42fefa39efp-6
  },
  { // Entry 278
    0x1.059b0d31585743e239d86797360d174fp0,
    0x1.62e42fefa39f0p-6
  },
  { // Entry 279
    0x1.0b5586cf9890f587f5279294114165a2p0,
    0x1.62e42fefa39eep-5
  },
  { // Entry 280
    0x1.0b5586cf9890f60d9feafa6059bc4b08p0,
    0x1.62e42fefa39efp-5
  },
  { // Entry 281
    0x1.0b5586cf9890f6934aae622ca2377342p0,
    0x1.62e42fefa39f0p-5
  },
  { // Entry 282
    0x1.172b83c7d517ac7c7c0d3432ad543afap0,
    0x1.62e42fefa39eep-4
  },
  { // Entry 283
    0x1.172b83c7d517ad93a790fc07c501430cp0,
    0x1.62e42fefa39efp-4
  },
  { // Entry 284
    0x1.172b83c7d517aeaad314c3dcdcaf6249p0,
    0x1.62e42fefa39f0p-4
  },
  { // Entry 285
    0x1.306fe0a31b714ffe7eec6bebf7ca9d7dp0,
    0x1.62e42fefa39eep-3
  },
  { // Entry 286
    0x1.306fe0a31b71525f5eadb222da6cfb5ap0,
    0x1.62e42fefa39efp-3
  },
  { // Entry 287
    0x1.306fe0a31b7154c03e6ef859bd141af7p0,
    0x1.62e42fefa39f0p-3
  },
  { // Entry 288
    0x1.6a09e667f3bcc2320d5de690c78172aep0,
    0x1.62e42fefa39eep-2
  },
  { // Entry 289
    0x1.6a09e667f3bcc7da34f7865fba958b33p0,
    0x1.62e42fefa39efp-2
  },
  { // Entry 290
    0x1.6a09e667f3bccd825c91262eadc04456p0,
    0x1.62e42fefa39f0p-2
  },
  { // Entry 291
    0x1.ffffffffffffeca86c3898cff8779a1ap0,
    0x1.62e42fefa39eep-1
  },
  { // Entry 292
    0x1.fffffffffffffca86c3898cff81cdd7cp0,
    0x1.62e42fefa39efp-1
  },
  { // Entry 293
    0x1.0000000000000654361c4c67fc21106fp1,
    0x1.62e42fefa39f0p-1
  },
  { // Entry 294
    0x1.ffffffffffffd950d871319ff1aa4328p1,
    0x1.62e42fefa39eep0
  },
  { // Entry 295
    0x1.fffffffffffff950d871319ff03f50afp1,
    0x1.62e42fefa39efp0
  },
  { // Entry 296
    0x1.0000000000000ca86c3898cff86a2f1bp2,
    0x1.62e42fefa39f0p0
  },
  { // Entry 297
    0x1.ffffffffffffb2a1b0e2633fe640c21bp3,
    0x1.62e42fefa39eep1
  },
  { // Entry 298
    0x1.fffffffffffff2a1b0e2633fe094f837p3,
    0x1.62e42fefa39efp1
  },
  { // Entry 299
    0x1.0000000000001950d871319ff174972ap4,
    0x1.62e42fefa39f0p1
  },
  { // Entry 300
    0x1.ffffffffffff654361c4c67fd8327361p7,
    0x1.62e42fefa39eep2
  },
  { // Entry 301
    0x1.ffffffffffffe54361c4c67fc1834bd3p7,
    0x1.62e42fefa39efp2
  },
  { // Entry 302
    0x1.00000000000032a1b0e2633fe56a1222p8,
    0x1.62e42fefa39f0p2
  },
  { // Entry 303
    0x1.fffffffffffeca86c3898cffdf28a36fp15,
    0x1.62e42fefa39eep3
  },
  { // Entry 304
    0x1.ffffffffffffca86c3898cff846c0534p15,
    0x1.62e42fefa39efp3
  },
  { // Entry 305
    0x1.000000000000654361c4c67fd4d7b37cp16,
    0x1.62e42fefa39f0p3
  },
  { // Entry 306
    0x1.fffffffffffd950d87131a007960398fp31,
    0x1.62e42fefa39eep4
  },
  { // Entry 307
    0x1.ffffffffffff950d871319ff0e6dc0a3p31,
    0x1.62e42fefa39efp4
  },
  { // Entry 308
    0x1.000000000000ca86c3898cffd1bda3dbp32,
    0x1.62e42fefa39f0p4
  },
  { // Entry 309
    0x1.ffffffffffb2a1b0e26345b8dfe00697p1023,
    0x1.62e42fefa39eep9
  },
  { // Entry 310
    0x1.fffffffffff2a1b0e263400d15fc52ffp1023,
    0x1.62e42fefa39efp9
  },
  { // Entry 311
    HUGE_VAL,
    0x1.62e42fefa39f0p9
  },
  { // Entry 312
    0x1.c8464f76161962ed1930796c0794254ap-1071,
    -0x1.72e42fefa39f0p9
  },
  { // Entry 313
    0x1.c8464f7616526bb707f34028f1d63786p-1071,
    -0x1.72e42fefa39efp9
  },
  { // Entry 314
    0x1.c8464f76168b7480f6b60e06f556221bp-1071,
    -0x1.72e42fefa39eep9
  },
  { // Entry 315
    0x1.8ebef9eac820a84b86d1ce1a4424435fp-1,
    -0x1.0000000000001p-2
  },
  { // Entry 316
    0x1.8ebef9eac820ae8682b9793ac6d1e772p-1,
    -0x1.0p-2
  },
  { // Entry 317
    0x1.8ebef9eac820b1a400ad4ecb083211f6p-1,
    -0x1.fffffffffffffp-3
  },
  { // Entry 318
    0x1.c3d6a24ed82214f0d01daf8e1a8d2ca7p-1,
    -0x1.0000000000001p-3
  },
  { // Entry 319
    0x1.c3d6a24ed82218787d624d3e5eba95f5p-1,
    -0x1.0p-3
  },
  { // Entry 320
    0x1.c3d6a24ed8221a3c54049c1680d3f05ep-1,
    -0x1.fffffffffffffp-4
  },
  { // Entry 321
    0x1.e0fabfbc702a3b04e86023f0691cc597p-1,
    -0x1.0000000000001p-4
  },
  { // Entry 322
    0x1.e0fabfbc702a3ce5e31fe0609358bafdp-1,
    -0x1.0p-4
  },
  { // Entry 323
    0x1.e0fabfbc702a3dd6607fbe98a8776a0ep-1,
    -0x1.fffffffffffffp-5
  },
  { // Entry 324
    0x1.f03f56a88b5d781c905f6b51c76ae981p-1,
    -0x1.0000000000001p-5
  },
  { // Entry 325
    0x1.f03f56a88b5d7914b00abf97762735d1p-1,
    -0x1.0p-5
  },
  { // Entry 326
    0x1.f03f56a88b5d7990bfe069ba4d858a7fp-1,
    -0x1.fffffffffffffp-6
  },
  { // Entry 327
    0x1.f80feabfeefa48a9cd112d592c437012p-1,
    -0x1.0000000000001p-6
  },
  { // Entry 328
    0x1.f80feabfeefa4927d10bdd54ead5aa46p-1,
    -0x1.0p-6
  },
  { // Entry 329
    0x1.f80feabfeefa4966d3093552ca1ed330p-1,
    -0x1.fffffffffffffp-7
  },
  { // Entry 330
    0x1.fc03fd56aa224f587c3f685d543e53d7p-1,
    -0x1.0000000000001p-7
  },
  { // Entry 331
    0x1.fc03fd56aa224f97fcbf1332988842dep-1,
    -0x1.0p-7
  },
  { // Entry 332
    0x1.fc03fd56aa224fb7bcfee89d3aad3d5cp-1,
    -0x1.fffffffffffffp-8
  },
  { // Entry 333
    0x1.fe00ffaabffbbc51cd0e16d8b005d662p-1,
    -0x1.0000000000001p-8
  },
  { // Entry 334
    0x1.fe00ffaabffbbc71ad1e1184afc19c7ep-1,
    -0x1.0p-8
  },
  { // Entry 335
    0x1.fe00ffaabffbbc819d260edaaf9f804bp-1,
    -0x1.fffffffffffffp-9
  },
  { // Entry 336
    0x1.ff003ff556aa887b68800bb5d27da4ffp-1,
    -0x1.0000000000001p-9
  },
  { // Entry 337
    0x1.ff003ff556aa888b60820b6087d1e91ap-1,
    -0x1.0p-9
  },
  { // Entry 338
    0x1.ff003ff556aa88935c830b35e27c0b57p-1,
    -0x1.fffffffffffffp-10
  },
  { // Entry 339
    0x1.ff800ffeaabffee6fc4efcfc6459de13p-1,
    -0x1.0000000000001p-10
  },
  { // Entry 340
    0x1.ff800ffeaabffeeefa4f3cf70f59d9bfp-1,
    -0x1.0p-10
  },
  { // Entry 341
    0x1.ff800ffeaabffef2f94f5cf464d9d7a1p-1,
    -0x1.fffffffffffffp-11
  },
  { // Entry 342
    0x1.fff0003fff5556a9a8908b40b320849dp-1,
    -0x1.0000000000001p-13
  },
  { // Entry 343
    0x1.fff0003fff5556aaa8888b60b2cb2ff2p-1,
    -0x1.0p-13
  },
  { // Entry 344
    0x1.fff0003fff5556ab28848b70b2a0859dp-1,
    -0x1.fffffffffffffp-14
  },
  { // Entry 345
    0x1.48b5e3c3e81863e50ffc2ecb79f3f7c3p0,
    0x1.fffffffffffffp-3
  },
  { // Entry 346
    0x1.48b5e3c3e81866767bc3b69baabe534ep0,
    0x1.0p-2
  },
  { // Entry 347
    0x1.48b5e3c3e8186b995352c63c0c6272ecp0,
    0x1.0000000000001p-2
  },
  { // Entry 348
    0x1.2216045b6f5cce7ad7642815839c8160p0,
    0x1.fffffffffffffp-4
  },
  { // Entry 349
    0x1.2216045b6f5ccf9ced688384e06b8d42p0,
    0x1.0p-3
  },
  { // Entry 350
    0x1.2216045b6f5cd1e119713a639a0d0b49p0,
    0x1.0000000000001p-3
  },
  { // Entry 351
    0x1.1082b577d34ed74d70455df87e5de089p0,
    0x1.fffffffffffffp-5
  },
  { // Entry 352
    0x1.1082b577d34ed7d5b1a019e225c9a951p0,
    0x1.0p-4
  },
  { // Entry 353
    0x1.1082b577d34ed8e6345591b574a20744p0,
    0x1.0000000000001p-4
  },
  { // Entry 354
    0x1.08205601127ec94c03bb0367085a3c64p0,
    0x1.fffffffffffffp-6
  },
  { // Entry 355
    0x1.08205601127ec98e0bd083aba80c97a6p0,
    0x1.0p-5
  },
  { // Entry 356
    0x1.08205601127eca121bfb8434e7717fb0p0,
    0x1.0000000000001p-5
  },
  { // Entry 357
    0x1.04080ab55de3915a37635d3d47dc3df6p0,
    0x1.fffffffffffffp-7
  },
  { // Entry 358
    0x1.04080ab55de3917ab864b3e9044e6b45p0,
    0x1.0p-6
  },
  { // Entry 359
    0x1.04080ab55de391bbba6761407d32d213p0,
    0x1.0000000000001p-6
  },
  { // Entry 360
    0x1.0202015600445afc12436d5c6f22ecf0p0,
    0x1.fffffffffffffp-8
  },
  { // Entry 361
    0x1.0202015600445b0c326382bc73689d32p0,
    0x1.0p-7
  },
  { // Entry 362
    0x1.0202015600445b2c72a3ad7c7bf400bcp0,
    0x1.0000000000001p-7
  },
  { // Entry 363
    0x1.0100802ab55777ca8226417cbfee2ff3p0,
    0x1.fffffffffffffp-9
  },
  { // Entry 364
    0x1.0100802ab55777d28a2a42d26aa9ee67p0,
    0x1.0p-8
  },
  { // Entry 365
    0x1.0100802ab55777e29a32457dc0216c10p0,
    0x1.0000000000001p-8
  },
  { // Entry 366
    0x1.008020055600110e7b4155a81af484b1p0,
    0x1.fffffffffffffp-10
  },
  { // Entry 367
    0x1.00802005560011127d41d5bd72f4c8f3p0,
    0x1.0p-9
  },
  { // Entry 368
    0x1.008020055600111a8142d5e822f551a7p0,
    0x1.0000000000001p-9
  },
  { // Entry 369
    0x1.00400800aab555dbe30e5ce71927cbf0p0,
    0x1.fffffffffffffp-11
  },
  { // Entry 370
    0x1.00400800aab555dde38e6ce86e9277aap0,
    0x1.0p-10
  },
  { // Entry 371
    0x1.00400800aab555e1e48e8ceb1967cf29p0,
    0x1.0000000000001p-10
  },
  { // Entry 372
    0x1.00080020005555ffc10f1275295d4349p0,
    0x1.fffffffffffffp-14
  },
  { // Entry 373
    0x1.00080020005556000111127d297298c9p0,
    0x1.0p-13
  },
  { // Entry 374
    0x1.00080020005556008115128d299d43c9p0,
    0x1.0000000000001p-13
  },
  { // Entry 375
    0x1.44109edb206a938fad670b68ab99b768p-739,
    -0x1.0000000000001p9
  },
  { // Entry 376
    0x1.44109edb209315a388cb1b433ecd1a8ap-739,
    -0x1.0p9
  },
  { // Entry 377
    0x1.44109edb20a756ad767d2516a15514ccp-739,
    -0x1.fffffffffffffp8
  },
  { // Entry 378
    0x1.9755956ad4d04a606a8cd9ddfb368effp-370,
    -0x1.0000000000001p8
  },
  { // Entry 379
    0x1.9755956ad4e9bfb9c13a27ae4c07ed37p-370,
    -0x1.0p8
  },
  { // Entry 380
    0x1.9755956ad4f67a666c90cf2f3488a463p-370,
    -0x1.fffffffffffffp7
  },
  { // Entry 381
    0x1.42eb9f39afa6992bf3a1379715f6683bp-185,
    -0x1.0000000000001p7
  },
  { // Entry 382
    0x1.42eb9f39afb0b088ed6eb4f43cc9ec7bp-185,
    -0x1.0p7
  },
  { // Entry 383
    0x1.42eb9f39afb5bc376a5573c1164a9c03p-185,
    -0x1.fffffffffffffp6
  },
  { // Entry 384
    0x1.969d47321e46604a105f8de32d9dc685p-93,
    -0x1.0000000000001p6
  },
  { // Entry 385
    0x1.969d47321e4cbabf2d28070963b04194p-93,
    -0x1.0p6
  },
  { // Entry 386
    0x1.969d47321e4fe7f9bb8c43a606692a48p-93,
    -0x1.fffffffffffffp5
  },
  { // Entry 387
    0x1.c8464f761642f155fab3e5bb144fb910p-47,
    -0x1.0000000000001p5
  },
  { // Entry 388
    0x1.c8464f76164681e299a0124487884d64p-47,
    -0x1.0p5
  },
  { // Entry 389
    0x1.c8464f7616484a28e916288bed8e0ebfp-47,
    -0x1.fffffffffffffp4
  },
  { // Entry 390
    0x1.e355bbaee85aca50a3c50ad2ccffbb29p-24,
    -0x1.0000000000001p4
  },
  { // Entry 391
    0x1.e355bbaee85cada65f73f32e88fb3cc6p-24,
    -0x1.0p4
  },
  { // Entry 392
    0x1.e355bbaee85d9f513d4b675d1c3923f6p-24,
    -0x1.fffffffffffffp3
  },
  { // Entry 393
    0x1.5fc21041027a1bdec525f6d2b48a4637p-12,
    -0x1.0000000000001p3
  },
  { // Entry 394
    0x1.5fc21041027acbbfcd46780fee71ead2p-12,
    -0x1.0p3
  },
  { // Entry 395
    0x1.5fc21041027b23b05156b8aeac5feea5p-12,
    -0x1.fffffffffffffp2
  },
  { // Entry 396
    0x1.2c155b8213cefc79340f8e386263456ap-6,
    -0x1.0000000000001p2
  },
  { // Entry 397
    0x1.2c155b8213cf477e8af0132c2ae23d4ap-6,
    -0x1.0p2
  },
  { // Entry 398
    0x1.2c155b8213cf6d01366055a6162a395fp-6,
    -0x1.fffffffffffffp1
  },
  { // Entry 399
    0x1.152aaa3bf81c975862272f0f3b13f586p-3,
    -0x1.0000000000001p1
  },
  { // Entry 400
    0x1.152aaa3bf81cb9fdb76eae12d029571fp-3,
    -0x1.0p1
  },
  { // Entry 401
    0x1.152aaa3bf81ccb5062126d949c53c7ebp-3,
    -0x1.fffffffffffffp0
  },
  { // Entry 402
    0x1.78b56362cef364df9544f11b0a499031p-2,
    -0x1.0000000000001p0
  },
  { // Entry 403
    0x1.78b56362cef37c6aeb7b1e0a4153e437p-2,
    -0x1.0p0
  },
  { // Entry 404
    0x1.78b56362cef3883096963481dd66523fp-2,
    -0x1.fffffffffffffp-1
  },
  { // Entry 405
    0x1.9476504ba8399f5b97cae35beb78c3c5p738,
    0x1.fffffffffffffp8
  },
  { // Entry 406
    0x1.9476504ba852e6c09c8567c01c5a6648p738,
    0x1.0p9
  },
  { // Entry 407
    0x1.9476504ba885758aa5fa7545e10e8e46p738,
    0x1.0000000000001p9
  },
  { // Entry 408
    0x1.41c7a8814be192a5df25b042af824efdp369,
    0x1.fffffffffffffp7
  },
  { // Entry 409
    0x1.41c7a8814beba0e323300f777da65854p369,
    0x1.0p8
  },
  { // Entry 410
    0x1.41c7a8814bffbd5dab44ced26faccbfbp369,
    0x1.0000000000001p8
  },
  { // Entry 411
    0x1.95e54c5dd41b20600dd601a0ae672ff4p184,
    0x1.fffffffffffffp6
  },
  { // Entry 412
    0x1.95e54c5dd42177f53f4d5219df11ca3bp184,
    0x1.0p7
  },
  { // Entry 413
    0x1.95e54c5dd42e271fa23bf3585b655060p184,
    0x1.0000000000001p7
  },
  { // Entry 414
    0x1.425982cf597a4d52c89ea857bbaa807ap92,
    0x1.fffffffffffffp5
  },
  { // Entry 415
    0x1.425982cf597cd205ce3d5b4edb031756p92,
    0x1.0p6
  },
  { // Entry 416
    0x1.425982cf5981db6bd97ac14c35e666c6p92,
    0x1.0000000000001p6
  },
  { // Entry 417
    0x1.1f43fcc4b661a8944ac389b609e0f74ep46,
    0x1.fffffffffffffp4
  },
  { // Entry 418
    0x1.1f43fcc4b662c7d84788401842174074p46,
    0x1.0p5
  },
  { // Entry 419
    0x1.1f43fcc4b66506604111ace0104fc90ep46,
    0x1.0000000000001p5
  },
  { // Entry 420
    0x1.0f2ebd0a80017cfac56c30874afbab98p23,
    0x1.fffffffffffffp3
  },
  { // Entry 421
    0x1.0f2ebd0a8002049223f170882b5ee5efp23,
    0x1.0p4
  },
  { // Entry 422
    0x1.0f2ebd0a800313c0e0fbf08ab7886866p23,
    0x1.0000000000001p4
  },
  { // Entry 423
    0x1.749ea7d470c681e43618ec18d53f1b21p11,
    0x1.fffffffffffffp2
  },
  { // Entry 424
    0x1.749ea7d470c6df0be00e084a815d1de6p11,
    0x1.0p3
  },
  { // Entry 425
    0x1.749ea7d470c7995b33f840ae1f76e2e7p11,
    0x1.0000000000001p3
  },
  { // Entry 426
    0x1.b4c902e273a54fdfb6777166e6760dfbp5,
    0x1.fffffffffffffp1
  },
  { // Entry 427
    0x1.b4c902e273a58678d6d3bfdb93db96d0p5,
    0x1.0p2
  },
  { // Entry 428
    0x1.b4c902e273a5f3ab178c5cc50320149cp5,
    0x1.0000000000001p2
  },
  { // Entry 429
    0x1.d8e64b8d4ddabf34d582cd2909aafb2ap2,
    0x1.fffffffffffffp0
  },
  { // Entry 430
    0x1.d8e64b8d4ddadcc33a3ba206b68abba8p2,
    0x1.0p1
  },
  { // Entry 431
    0x1.d8e64b8d4ddb17e003ad4bc215d4ef86p2,
    0x1.0000000000001p1
  },
  { // Entry 432
    0x1.5bf0a8b145768a55da73221499fc4cfbp1,
    0x1.fffffffffffffp-1
  },
  { // Entry 433
    0x1.5bf0a8b1457695355fb8ac404e7a79e3p1,
    0x1.0p0
  },
  { // Entry 434
    0x1.5bf0a8b14576aaf46a43c097b87bc833p1,
    0x1.0000000000001p0
  },
  { // Entry 435
    0x1.ffc045692fc9dbc7b7e032576e5e26f8p1023,
    0x1.62e41ffffffffp9
  },
  { // Entry 436
    0x1.ffc045693009d3d065062f9267dff55ep1023,
    0x1.62e42p9
  },
  { // Entry 437
    0x1.ffc045693049cbd9122c34cc62776884p1023,
    0x1.62e4200000001p9
  },
  { // Entry 438
    0x1.c87f21775a0f1a25755fb027f94949a1p-1071,
    -0x1.72e4200000001p9
  },
  { // Entry 439
    0x1.c87f21775a482a09a44af59c3c3ae44bp-1071,
    -0x1.72e42p9
  },
  { // Entry 440
    0x1.c87f21775a8139edd33642327bb25c5fp-1071,
    -0x1.72e41ffffffffp9
  },
  { // Entry 441
    HUGE_VAL,
    0x1.0p1020
  },
  { // Entry 442
    HUGE_VAL,
    0x1.999999999999ap1020
  },
  { // Entry 443
    HUGE_VAL,
    0x1.199999999999ap1021
  },
  { // Entry 444
    HUGE_VAL,
    0x1.6666666666667p1021
  },
  { // Entry 445
    HUGE_VAL,
    0x1.b333333333334p1021
  },
  { // Entry 446
    HUGE_VAL,
    0x1.0p1022
  },
  { // Entry 447
    HUGE_VAL,
    0x1.fffffffffffffp1023
  },
  { // Entry 448
    HUGE_VAL,
    HUGE_VAL
  },
  { // Entry 449
    HUGE_VAL,
    0x1.fffffffffffffp1023
  },
  { // Entry 450
    HUGE_VAL,
    0x1.ffffffffffffep1023
  },
  { // Entry 451
    0x1.724046eb09338d2991a30893e7f4108dp4,
    0x1.921fb54442d18p1
  },
  { // Entry 452
    0x1.33dedc855935efaaaf578ced3f821f96p2,
    0x1.921fb54442d18p0
  },
  { // Entry 453
    0x1.5bf0a8b14576aaf46a43c097b87bc833p1,
    0x1.0000000000001p0
  },
  { // Entry 454
    0x1.5bf0a8b1457695355fb8ac404e7a79e3p1,
    0x1.0p0
  },
  { // Entry 455
    0x1.5bf0a8b145768a55da73221499fc4cfbp1,
    0x1.fffffffffffffp-1
  },
  { // Entry 456
    0x1.18bd669471caa5585c71cc32a792ac85p1,
    0x1.921fb54442d18p-1
  },
  { // Entry 457
    0x1.p0,
    0x1.0000000000001p-1022
  },
  { // Entry 458
    0x1.p0,
    0x1.0p-1022
  },
  { // Entry 459
    0x1.p0,
    0x1.ffffffffffffep-1023
  },
  { // Entry 460
    0x1.p0,
    0x1.ffffffffffffcp-1023
  },
  { // Entry 461
    0x1.p0,
    0x1.0p-1073
  },
  { // Entry 462
    0x1.p0,
    0x1.0p-1074
  },
  { // Entry 463
    0x1.p0,
    0.0
  },
  { // Entry 464
    0x1.p0,
    -0.0
  },
  { // Entry 465
    0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.0p-1074
  },
  { // Entry 466
    0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.0p-1073
  },
  { // Entry 467
    0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.ffffffffffffcp-1023
  },
  { // Entry 468
    0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 469
    0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.0p-1022
  },
  { // Entry 470
    0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.0000000000001p-1022
  },
  { // Entry 471
    0x1.d2e171cf048793e16a7f9406e00c2dd9p-2,
    -0x1.921fb54442d18p-1
  },
  { // Entry 472
    0x1.78b56362cef3883096963481dd66523fp-2,
    -0x1.fffffffffffffp-1
  },
  { // Entry 473
    0x1.78b56362cef37c6aeb7b1e0a4153e437p-2,
    -0x1.0p0
  },
  { // Entry 474
    0x1.78b56362cef364df9544f11b0a499031p-2,
    -0x1.0000000000001p0
  },
  { // Entry 475
    0x1.a9bcc46f767dfb0d4eec2b1337a04b9ap-3,
    -0x1.921fb54442d18p0
  },
  { // Entry 476
    0x1.620227b598ef9949e60e44dc45d14d0cp-5,
    -0x1.921fb54442d18p1
  },
  { // Entry 477
    0.0,
    -0x1.ffffffffffffep1023
  },
  { // Entry 478
    0.0,
    -0x1.fffffffffffffp1023
  },
  { // Entry 479
    0.0,
    -HUGE_VAL
  },
  { // Entry 480
    0x1.fffffffffff2a1b0e263400d15fc52ffp1023,
    0x1.62e42fefa39efp9
  },
  { // Entry 481
    HUGE_VAL,
    0x1.62e42fefa39f0p9
  },
  { // Entry 482
    0x1.000000000007bbcffb06f8fdab896db0p-1022,
    -0x1.6232bdd7abcd2p9
  },
  { // Entry 483
    0x1.ffffffffffcf779ff60df40c631419a2p-1023,
    -0x1.6232bdd7abcd3p9
  },
};
#endif // __BIONIC__

TEST(math_exp, exp_intel) {
#if defined(__BIONIC__)
  for (size_t i = 0; i < sizeof(g_exp_intel_data)/sizeof(exp_intel_data_t); i++) {
    EXPECT_DOUBLE_EQ(g_exp_intel_data[i].expected, exp(g_exp_intel_data[i].call_data)) << "Failed on element " << i;
  }
#else // __BIONIC__
  GTEST_LOG_(INFO) << "This test does nothing.";
#endif // __BIONIC__
}
