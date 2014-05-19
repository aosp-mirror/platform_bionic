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
  float expected;
  float call_data;
} logf_intel_data_t;

static logf_intel_data_t g_logf_intel_data[] = {
  { // Entry 0
    -0x1.bb9d3aeb8c87b02d7763eba8b48a102dp1,
    0x1.000002p-5
  },
  { // Entry 1
    0x1.fffffe000002aaaaa6aaaab111110666p-24,
    0x1.000002p0
  },
  { // Entry 2
    -0x1.c6b45ceb09a5a7c82aacd1cadf7253dcp4,
    0x1.000008p-41
  },
  { // Entry 3
    -0x1.d1cb7cea86d09f62474b14c45f4cb680p3,
    0x1.000010p-21
  },
  { // Entry 4
    -0x1.fe2800e87c347d788f394ef2e93db868p5,
    0x1.000080p-92
  },
  { // Entry 5
    0x1.1fffaf001e5ff32f85c436e59fe73b1ep-17,
    0x1.000090p0
  },
  { // Entry 6
    -0x1.0a27a3fffa7e0d031d9a55d157a0e8c8p1,
    0x1.0007p-3
  },
  { // Entry 7
    0x1.54de6ee78989a9acfc875c1d45e16490p-9,
    0x1.00aaa8p0
  },
  { // Entry 8
    0x1.8f4826fff787c58fab520e505952bb55p-9,
    0x1.00c7f2p0
  },
  { // Entry 9
    -0x1.6150cefffc83ba2da43d8d7455ef2fdep-1,
    0x1.00cap-1
  },
  { // Entry 10
    0x1.cf2e3eb0928bdf5cbf064a27c6422c85p-9,
    0x1.00e8p0
  },
  { // Entry 11
    0x1.f0ee8a9a67ab36597ffd3c93304d0cc5p-9,
    0x1.00f8f0p0
  },
  { // Entry 12
    0x1.fbdfc897d239fb49dc1c31afeaea1ea5p-9,
    0x1.00fe6ep0
  },
  { // Entry 13
    0x1.fdc5e48f893e8f48967ee4ff1e895dd5p-9,
    0x1.00ff62p0
  },
  { // Entry 14
    0x1.be79c70058ec8f9a6c04043f52763c30p-8,
    0x1.01c0p0
  },
  { // Entry 15
    0x1.e3afef036c442bea46d105f6864ea6f3p-8,
    0x1.01e57ap0
  },
  { // Entry 16
    0x1.eb92db03d89f25719af908ebb6e7e510p-8,
    0x1.01ed6cp0
  },
  { // Entry 17
    0x1.f873b502f9427cb1a14872267ffae2f8p-8,
    0x1.01fa66p0
  },
  { // Entry 18
    0x1.fc108903fcd4cd445800cb06c164d0b0p-8,
    0x1.01fe0ap0
  },
  { // Entry 19
    0x1.fea168fad6274232ad3998c3e39e43d9p-8,
    0x1.0200a0p0
  },
  { // Entry 20
    0x1.0ce4c9fe3edd7f45dc38d30f76305931p-7,
    0x1.021cp0
  },
  { // Entry 21
    0x1.0ee096e2764f9f1e64840607436cc093p-7,
    0x1.0220p0
  },
  { // Entry 22
    0x1.fc098efffe49fe32c3576def6f303335p-7,
    0x1.03fffep0
  },
  { // Entry 23
    -0x1.58fed400015fff94ac3bb9ebbc81c8aep-1,
    0x1.04ffp-1
  },
  { // Entry 24
    -0x1.58b1f1ffffa56b55b6a722e0ab2c5ae6p-1,
    0x1.052634p-1
  },
  { // Entry 25
    0x1.e720a3003d099731c85ce6d689546a76p-6,
    0x1.07b9c4p0
  },
  { // Entry 26
    0x1.f5a4cb00353937ad8b1e07a6a469189fp-6,
    0x1.07f59cp0
  },
  { // Entry 27
    -0x1.b0080fffd588ec91883715736aae0f50p4,
    0x1.08p-39
  },
  { // Entry 28
    0x1.7f64a700002b54d6fb5d69bc35c5e2cap-1,
    0x1.0ea7b0p1
  },
  { // Entry 29
    0x1.cf2825078d8bd21f5b5543342e66b54dp-5,
    0x1.0ee4p0
  },
  { // Entry 30
    0x1.845657000391f340da745d9e5d283165p-1,
    0x1.1148p1
  },
  { // Entry 31
    -0x1.b959420004b51fb80d8329172d8922f6p3,
    0x1.12c8p-20
  },
  { // Entry 32
    -0x1.3cb226cef9a610cf77dc0067902b4099p-1,
    0x1.13d4p-1
  },
  { // Entry 33
    0x1.5e1a22fccd87d40b6e7ebc0226374d61p-4,
    0x1.16d8p0
  },
  { // Entry 34
    -0x1.35fb76dd8c7a211f33842af42c599114p-1,
    0x1.1778p-1
  },
  { // Entry 35
    -0x1.35028ad9d8c85c1fca93f355d4796bc1p-1,
    0x1.18p-1
  },
  { // Entry 36
    0x1.a3f71cff14b8111aded976c26d18960ep-4,
    0x1.1ba4p0
  },
  { // Entry 37
    -0x1.55a061fff90fa6df755f7aef5ab9f978p1,
    0x1.1bf4p-4
  },
  { // Entry 38
    -0x1.ce5ffde66a9af783b86443209ecec164p3,
    0x1.1ce0p-21
  },
  { // Entry 39
    -0x1.ce3a55ea5d47322bdbbe0fff5479ca1ep3,
    0x1.1e30p-21
  },
  { // Entry 40
    0x1.e23792ef52971fd6c72a99f598a0fbacp-4,
    0x1.1ffcp0
  },
  { // Entry 41
    0x1.8fe0c85314ba5e09e115528c02ef6e31p0,
    0x1.312e78p2
  },
  { // Entry 42
    -0x1.fdedfade465d57336a9b8a3562d3b176p-2,
    0x1.372c16p-1
  },
  { // Entry 43
    -0x1.fd9c98de7d89b9e29546a9b7692cacddp-2,
    0x1.3744d2p-1
  },
  { // Entry 44
    -0x1.fd5656de0c66d47b7b9cf4fa68b356e4p-2,
    0x1.375a2ep-1
  },
  { // Entry 45
    -0x1.fca98cde0ceb18d6d7bab7705d14de17p-2,
    0x1.378ebcp-1
  },
  { // Entry 46
    -0x1.fc0a58de4ea7c9432ac00ff0adb6db7ep-2,
    0x1.37bf30p-1
  },
  { // Entry 47
    -0x1.ebdede51de7d7b359acc23ec312af2d4p-2,
    0x1.3cb56ap-1
  },
  { // Entry 48
    -0x1.dae0dedfdda2569379630abffcaffff2p-2,
    0x1.420208p-1
  },
  { // Entry 49
    -0x1.d930ca53ae8e0695d14e48376b584668p-2,
    0x1.428a04p-1
  },
  { // Entry 50
    0x1.ec3649c72c05a069c2d000090851c639p-3,
    0x1.458cbep0
  },
  { // Entry 51
    0x1.f3e63257ecd12b0d9ccc9ff34c3f3d6ep-3,
    0x1.46c626p0
  },
  { // Entry 52
    -0x1.693cfa30b633b1d03f4ecd88a750ac79p-2,
    0x1.67cd3ap-1
  },
  { // Entry 53
    -0x1.692b5e42108b73d7599518074ce8670ap-2,
    0x1.67d36ap-1
  },
  { // Entry 54
    -0x1.63bbba1771b8208d851ab36cd6d93f38p-2,
    0x1.69bdbap-1
  },
  { // Entry 55
    0x1.62e25eec85781e60cb49b4f21becabdfp-2,
    0x1.6a0942p0
  },
  { // Entry 56
    0x1.62e3ce9ef86f6524351edd87310e8743p-2,
    0x1.6a09c4p0
  },
  { // Entry 57
    -0x1.3c607adedb168d9d89c6b2265b4dd0a4p-2,
    0x1.77ea38p-1
  },
  { // Entry 58
    -0x1.194432fffcb092d891670b7f8f628fc4p6,
    0x1.77fffep-102
  },
  { // Entry 59
    0x1.193ea500258270930f8e7d7af244dcffp0,
    0x1.7ffffcp1
  },
  { // Entry 60
    -0x1.dac08de1d26b0f69e1ed58bd3d9fd82bp3,
    0x1.82fe92p-22
  },
  { // Entry 61
    0x1.c8d70de117cb2fe459ed64cc67e5abc9p3,
    0x1.830608p20
  },
  { // Entry 62
    0x1.f2272ae325a57546f69496cf261be046p1,
    0x1.88p5
  },
  { // Entry 63
    0x1.c9c5ade34763c0b9a180d863bfc7f106p3,
    0x1.8e7686p20
  },
  { // Entry 64
    -0x1.f991d108abe1fc9c91e91ae7f707bd4fp-3,
    0x1.8ffffep-1
  },
  { // Entry 65
    -0x1.827d4d002e5d13215ce5f8de87566933p1,
    0x1.8ffffep-5
  },
  { // Entry 66
    -0x1.f305bd058b3c9e64101476e5de911d83p-3,
    0x1.9147e0p-1
  },
  { // Entry 67
    -0x1.ee7ee50650878a84e97f4cec96736bc5p-3,
    0x1.922b30p-1
  },
  { // Entry 68
    0x1.dc0391005bf0fccb92c262eeb6a300e4p0,
    0x1.9ae278p2
  },
  { // Entry 69
    -0x1.2650b600000be0c5902c7a15495161c5p1,
    0x1.9aeea0p-4
  },
  { // Entry 70
    -0x1.7ba9b7ffff3d189f5a17149697c12ce2p1,
    0x1.a5e970p-5
  },
  { // Entry 71
    -0x1.206170fffc9e8a93c1986e21787bed5ap1,
    0x1.ae6ef0p-4
  },
  { // Entry 72
    -0x1.d73d7ffc1e7abc4225e39878828f8666p3,
    0x1.afe26cp-22
  },
  { // Entry 73
    -0x1.25174700000008ddbe755739d626364fp3,
    0x1.b97c70p-14
  },
  { // Entry 74
    -0x1.1ca5b500035fc07c389a9e5cbfd78edbp1,
    0x1.bb2c2cp-4
  },
  { // Entry 75
    -0x1.24b0ebfffcc32ee1b9b739e2a4c7d059p3,
    0x1.bf0970p-14
  },
  { // Entry 76
    -0x1.669915000230d74fa2da2f40bbdf4469p3,
    0x1.c7fffep-17
  },
  { // Entry 77
    -0x1.954aeedf4d5f5fb7c474a35723b4e707p-1,
    0x1.cffffep-2
  },
  { // Entry 78
    -0x1.92e76afffcd449b3ac5d412b622f78a8p-4,
    0x1.d008e4p-1
  },
  { // Entry 79
    -0x1.ffa24f1c1233a9f8e8ae684edf89313fp-5,
    0x1.e10040p-1
  },
  { // Entry 80
    -0x1.117889fffffbdf8a5cd7c34f7246f3cfp1,
    0x1.e39bb4p-4
  },
  { // Entry 81
    -0x1.894ac2dcdde549d0d0922b5b4f54e028p-5,
    0x1.e7fffep-1
  },
  { // Entry 82
    -0x1.381d36ffe60eb1dfb0686574074223e7p-5,
    0x1.ecdc1cp-1
  },
  { // Entry 83
    0x1.d14973edb3656c771e18eca84bc94c76p4,
    0x1.effffep41
  },
  { // Entry 84
    -0x1.efea18fff1115e81636f53b65665e16ep-6,
    0x1.f0bc20p-1
  },
  { // Entry 85
    -0x1.a5b584ffdad31147d1140b1694740ad7p-6,
    0x1.f2fd60p-1
  },
  { // Entry 86
    -0x1.62b35affdf0a2429284b006b680bd1bep-6,
    0x1.f508e4p-1
  },
  { // Entry 87
    -0x1.0cb140ffff9f8f1e06e7f9af0a531a5bp1,
    0x1.f600dcp-4
  },
  { // Entry 88
    -0x1.0bfe70eb451ad09a7cb75c1eedfa6150p1,
    0x1.f8c010p-4
  },
  { // Entry 89
    -0x1.70c8c50dce246dd6bcb0461e7dc4937ep-7,
    0x1.fa4522p-1
  },
  { // Entry 90
    -0x1.416a71b773a2c3e3980b11efd1a197b5p-8,
    0x1.fd7ebep-1
  },
  { // Entry 91
    -0x1.c0c674621c5b474a430e13570c580388p-9,
    0x1.fe3ffep-1
  },
  { // Entry 92
    -0x1.04863cfffbbf7edb167cb60df3f3d599p-9,
    0x1.fefbbcp-1
  },
  { // Entry 93
    -0x1.0a5ddf00004ebf2a6feee99d5bddde34p1,
    0x1.ff353cp-4
  },
  { // Entry 94
    -0x1.d80366485b747247a45bb9a470179df1p-15,
    0x1.fff8a0p-1
  },
  { // Entry 95
    -0x1.d1cb7fea86c49f63074b0d445fb65014p3,
    0x1.fffff0p-22
  },
  { // Entry 96
    0x1.d1cb7dea86bc9f62dca06199b5053f03p3,
    0x1.fffff0p20
  },
  { // Entry 97
    -0x1.00000200000555556555558888893333p-22,
    0x1.fffff8p-1
  },
  { // Entry 98
    -0x1.00000100000155555755555888888dddp-23,
    0x1.fffffcp-1
  },
  { // Entry 99
    -0x1.000000800000555555955555888888b3p-24,
    0x1.fffffep-1
  },
  { // Entry 100
    -0x1.62e43115a8fb47c3a7c2e76a80cca9a5p-2,
    0x1.6a09e6p-1
  },
  { // Entry 101
    -0x1.da391a70d28a24641626f5e9155324b5p-3,
    0x1.962b60p-1
  },
  { // Entry 102
    -0x1.06fbe6a4e25295f80fb2274afe6d3bacp-3,
    0x1.c24cdap-1
  },
  { // Entry 103
    -0x1.1e0a6d053425d3d6528aa717ecc9a578p-5,
    0x1.ee6e54p-1
  },
  { // Entry 104
    0x1.9e549c3779f093451892a1fe4e67b50ep-5,
    0x1.0d47e6p0
  },
  { // Entry 105
    0x1.08e0b9f3a7944bab3ecb56d6a7ff03b5p-3,
    0x1.2358a2p0
  },
  { // Entry 106
    0x1.9e645d60ff198986479346d164ecad7ep-3,
    0x1.39695ep0
  },
  { // Entry 107
    0x1.14dd670cac0212506bf72fb762d8a7dbp-2,
    0x1.4f7a1ap0
  },
  { // Entry 108
    0x1.561819aca49de6b1d1ccda1095c74502p-2,
    0x1.658ad6p0
  },
  { // Entry 109
    0x1.936a7c496c3e21406af9f4ac9beac6a5p-2,
    0x1.7b9b92p0
  },
  { // Entry 110
    0x1.cd45a8d7e3403f5278d5e3d566349f75p-2,
    0x1.91ac4ep0
  },
  { // Entry 111
    0x1.02044831211ed0068efa4c2a8b9870bfp-1,
    0x1.a7bd0ap0
  },
  { // Entry 112
    0x1.1c01d5f6d3a50d31c45cbfddd0272765p-1,
    0x1.bdcdc6p0
  },
  { // Entry 113
    0x1.34bde46fd84fca43ba3f4799acab10ffp-1,
    0x1.d3de82p0
  },
  { // Entry 114
    0x1.4c5618136a3221d84476b78353fabc80p-1,
    0x1.e9ef3ep0
  },
  { // Entry 115
    0x1.62e429efa395f35781c7670787e58c2bp-1,
    0x1.fffffap0
  },
  { // Entry 116
    -0x1.62e43115a8fb47c3a7c2e76a80cca9a5p-2,
    0x1.6a09e6p-1
  },
  { // Entry 117
    -0x1.24cfd09514bae9e21501d1a28c37d51ep-2,
    0x1.80aa84p-1
  },
  { // Entry 118
    -0x1.d4902a7f6a1d18da241dffb42a0818edp-3,
    0x1.974b22p-1
  },
  { // Entry 119
    -0x1.65d5607cc85baba26e2faf78865dc991p-3,
    0x1.adebc0p-1
  },
  { // Entry 120
    -0x1.f991d8f6e2f011943d5a6a60cd646002p-4,
    0x1.c48c5ep-1
  },
  { // Entry 121
    -0x1.31b9a995502d59241500cd1ce955f5c4p-4,
    0x1.db2cfcp-1
  },
  { // Entry 122
    -0x1.ccb7994bda818c26cc15fca660f364ecp-6,
    0x1.f1cd9ap-1
  },
  { // Entry 123
    0x1.0b9492bd99570e8a2a2f5a7f59644b42p-6,
    0x1.04371cp0
  },
  { // Entry 124
    0x1.e2705e9cc554d8b565433c8fad2db164p-5,
    0x1.0f876cp0
  },
  { // Entry 125
    0x1.986d321688b074a21eb26b41679ee309p-4,
    0x1.1ad7bcp0
  },
  { // Entry 126
    0x1.1c89919fb208ebda275d6fee75577a43p-3,
    0x1.26280cp0
  },
  { // Entry 127
    0x1.69d463db5d0d8cbc5da8bd2c1f5d2953p-3,
    0x1.31785cp0
  },
  { // Entry 128
    0x1.b44f87381611ab2806d6f00d213de2cfp-3,
    0x1.3cc8acp0
  },
  { // Entry 129
    0x1.fc2d80e5311b3f7e05f26b963044ea08p-3,
    0x1.4818fcp0
  },
  { // Entry 130
    0x1.20cdd9262c59a7726689f4fe5ffa58fep-2,
    0x1.53694cp0
  },
  { // Entry 131
    0x1.426182db20e23d0b473ae1c2d1975c3fp-2,
    0x1.5eb99cp0
  },
  { // Entry 132
    0x1.62e42ec99e429eeb7fcbe6f58eff3117p-2,
    0x1.6a09e6p0
  },
  { // Entry 133
    -0x1.269621134db92783beb7676c0aa9c2a3p-2,
    0x1.80p-1
  },
  { // Entry 134
    -0x1.89fa484d355b5e186f5662b3da3e745bp-3,
    0x1.a66666p-1
  },
  { // Entry 135
    -0x1.af8e892d15de87e2e9e59dff5c0633bep-4,
    0x1.ccccccp-1
  },
  { // Entry 136
    -0x1.9ecebcb597ea1bb43d896f584759744ap-6,
    0x1.f33332p-1
  },
  { // Entry 137
    0x1.8fb04b8da65ceb85a2edb7d0ff0ad2d3p-5,
    0x1.0cccccp0
  },
  { // Entry 138
    0x1.e27076e2af2e5e9ea87ffe1fe9e155dbp-4,
    0x1.20p0
  },
  { // Entry 139
    0x1.756506739ebaf1cd58855d231e777a59p-3,
    0x1.333334p0
  },
  { // Entry 140
    0x1.f18dce2898f5ba919d825d9696c6c774p-3,
    0x1.466668p0
  },
  { // Entry 141
    0x1.334ea564424013b7739afbc9478741b5p-2,
    0x1.59999cp0
  },
  { // Entry 142
    0x1.6aac0d0fe3871ab60d70d41e954c2bd3p-2,
    0x1.6cccd0p0
  },
  { // Entry 143
    0x1.9f323ecbf984bf2b68d766f405221819p-2,
    0x1.80p0
  },
  { // Entry 144
    0.0,
    0x1.p0
  },
  { // Entry 145
    0x1.8663fd6538c872349ec1a85e05ec5fb5p-4,
    0x1.19999ap0
  },
  { // Entry 146
    0x1.756506739ebaf1cd58855d231e777a59p-3,
    0x1.333334p0
  },
  { // Entry 147
    0x1.0ca93b6f56afbacd07973b0a3c67fc47p-2,
    0x1.4ccccep0
  },
  { // Entry 148
    0x1.588c32237c6aefe96e1db27c46710d39p-2,
    0x1.666668p0
  },
  { // Entry 149
    0x1.9f3244214ed68647ddc799823e05a9b1p-2,
    0x1.800002p0
  },
  { // Entry 150
    0x1.e148a7a27268594dfd4dd6c301fece79p-2,
    0x1.99999cp0
  },
  { // Entry 151
    0x1.0fae84dc95e1a7d9897472d55a7563e8p-1,
    0x1.b33336p0
  },
  { // Entry 152
    0x1.2cf2633bc7fc9d9d9929319aa918822cp-1,
    0x1.ccccd0p0
  },
  { // Entry 153
    0x1.48a1165df274ab7963f8550dcbfb6aebp-1,
    0x1.e6666ap0
  },
  { // Entry 154
    0x1.62e42fefa39ef35793c7673007e5ed5ep-1,
    0x1.p1
  },
  { // Entry 155
    0x1.1542457337d42e1c6b73c89d862ba171p6,
    0x1.p100
  },
  { // Entry 156
    0x1.15a3de7291226038f89b79079dad1c89p6,
    0x1.19999ap100
  },
  { // Entry 157
    0x1.15fcf7f671a38b9552200b4c17badd2fp6,
    0x1.333334p100
  },
  { // Entry 158
    0x1.164eeeaea72addd7387b5fd89068096ep6,
    0x1.4ccccep100
  },
  { // Entry 159
    0x1.169ad1a55b50990c54e1e6500272127fp6,
    0x1.666668p100
  },
  { // Entry 160
    0x1.16e177b7592304a2b35190370869a71bp6,
    0x1.800002p100
  },
  { // Entry 161
    0x1.17238e1ada469675b9711674492da040p6,
    0x1.99999cp100
  },
  { // Entry 162
    0x1.1761a27cf0fff16c1e86b18330e08c39p6,
    0x1.b33336p100
  },
  { // Entry 163
    0x1.179c2a39af642757a6a61b00bb7dd276p6,
    0x1.ccccd0p100
  },
  { // Entry 164
    0x1.17d3879ff3b917735e3bb947a1c39847p6,
    0x1.e6666ap100
  },
  { // Entry 165
    0x1.18080dd3171b6c031a9b576be63b6d4cp6,
    0x1.p101
  },
  { // Entry 166
    -0x1.0000080000555559555588888b333357p-20,
    0x1.ffffe0p-1
  },
  { // Entry 167
    -0x1.0000040000155555d5555888889ddddep-21,
    0x1.fffff0p-1
  },
  { // Entry 168
    0.0,
    0x1.p0
  },
  { // Entry 169
    0x1.fffff800002aaaa9aaaab11110e66667p-22,
    0x1.000008p0
  },
  { // Entry 170
    0x1.fffff00000aaaaa2aaab11110bbbbc04p-21,
    0x1.000010p0
  },
  { // Entry 171
    -0x1.000000800000555555955555888888b3p-24,
    0x1.fffffep-1
  },
  { // Entry 172
    -0x1.000000800000555555955555888888b3p-24,
    0x1.fffffep-1
  },
  { // Entry 173
    -0x1.000000800000555555955555888888b3p-24,
    0x1.fffffep-1
  },
  { // Entry 174
    -0x1.000000800000555555955555888888b3p-24,
    0x1.fffffep-1
  },
  { // Entry 175
    -0x1.000000800000555555955555888888b3p-24,
    0x1.fffffep-1
  },
  { // Entry 176
    -0x1.000000800000555555955555888888b3p-24,
    0x1.fffffep-1
  },
  { // Entry 177
    -0x1.000000800000555555955555888888b3p-24,
    0x1.fffffep-1
  },
  { // Entry 178
    -0x1.000000800000555555955555888888b3p-24,
    0x1.fffffep-1
  },
  { // Entry 179
    -0x1.000000800000555555955555888888b3p-24,
    0x1.fffffep-1
  },
  { // Entry 180
    -0x1.000000800000555555955555888888b3p-24,
    0x1.fffffep-1
  },
  { // Entry 181
    -0x1.000000800000555555955555888888b3p-24,
    0x1.fffffep-1
  },
  { // Entry 182
    -0x1.000000800000555555955555888888b3p-24,
    0x1.fffffep-1
  },
  { // Entry 183
    -0x1.000000800000555555955555888888b3p-24,
    0x1.fffffep-1
  },
  { // Entry 184
    -0x1.000000800000555555955555888888b3p-24,
    0x1.fffffep-1
  },
  { // Entry 185
    -0x1.000000800000555555955555888888b3p-24,
    0x1.fffffep-1
  },
  { // Entry 186
    0x1.62e42feba39ef15793c611dab1909808p6,
    0x1.fffffep127
  },
  { // Entry 187
    -0x1.9d1d9fccf4770743f2061e1de931a650p6,
    0x1.p-149
  },
  { // Entry 188
    -0x1.62e436bdd09a876194940b96cb28fd7fp-2,
    0x1.6a09e4p-1
  },
  { // Entry 189
    -0x1.62e43115a8fb47c3a7c2e76a80cca9a5p-2,
    0x1.6a09e6p-1
  },
  { // Entry 190
    -0x1.62e42b6d81640825bf89d8b8d9d0ff58p-2,
    0x1.6a09e8p-1
  },
  { // Entry 191
    0x1.62e4292176a35f4d92fac2c944a2dd3dp-2,
    0x1.6a09e4p0
  },
  { // Entry 192
    0x1.62e42ec99e429eeb7fcbe6f58eff3117p-2,
    0x1.6a09e6p0
  },
  { // Entry 193
    0x1.62e43471c5d9de896804f5a735fadb65p-2,
    0x1.6a09e8p0
  },
  { // Entry 194
    -0x1.62e431efa39ff357947211db3290986fp-1,
    0x1.fffffep-2
  },
  { // Entry 195
    -0x1.62e42fefa39ef35793c7673007e5ed5ep-1,
    0x1.p-1
  },
  { // Entry 196
    -0x1.62e42befa3a2f3578e7211e2b2908b3cp-1,
    0x1.000002p-1
  },
  { // Entry 197
    -0x1.26962668a3120b11fac40bc7b22b5a8dp-2,
    0x1.7ffffep-1
  },
  { // Entry 198
    -0x1.269621134db92783beb7676c0aa9c2a3p-2,
    0x1.80p-1
  },
  { // Entry 199
    -0x1.26961bbdf867606749c734ddd1c6310bp-2,
    0x1.800002p-1
  },
  { // Entry 200
    0x1.9f323976a42bdb9d2ccac2985da0802fp-2,
    0x1.7ffffep0
  },
  { // Entry 201
    0x1.9f323ecbf984bf2b68d766f405221819p-2,
    0x1.80p0
  },
  { // Entry 202
    0x1.9f3244214ed68647ddc799823e05a9b1p-2,
    0x1.800002p0
  },
  { // Entry 203
    0x1.54de6ee78989a9acfc875c1d45e16490p-9,
    0x1.00aaa8p0
  },
  { // Entry 204
    0x1.54e26c3eab6ea24115a4ecbc6c21c3dep-9,
    0x1.00aaaap0
  },
  { // Entry 205
    0x1.54e66995c55e3ad40cb2b01d77b8b63fp-9,
    0x1.00aaacp0
  },
  { // Entry 206
    0x1.62e42eefa39e7357937211da729097d5p0,
    0x1.fffffep1
  },
  { // Entry 207
    0x1.62e42fefa39ef35793c7673007e5ed5ep0,
    0x1.p2
  },
  { // Entry 208
    0x1.62e431efa39cf357967211d6b2909e6fp0,
    0x1.000002p2
  },
  { // Entry 209
    0x1.62e42defa39df357931cbc84dd3b424dp-1,
    0x1.fffffep0
  },
  { // Entry 210
    0x1.62e42fefa39ef35793c7673007e5ed5ep-1,
    0x1.p1
  },
  { // Entry 211
    0x1.62e433efa39af357991cbc7d5d3b4f80p-1,
    0x1.000002p1
  },
  { // Entry 212
    -0x1.000000800000555555955555888888b3p-24,
    0x1.fffffep-1
  },
  { // Entry 213
    0.0,
    0x1.p0
  },
  { // Entry 214
    0x1.fffffe000002aaaaa6aaaab111110666p-24,
    0x1.000002p0
  },
  { // Entry 215
    -0x1.62e431efa39ff357947211db3290986fp-1,
    0x1.fffffep-2
  },
  { // Entry 216
    -0x1.62e42fefa39ef35793c7673007e5ed5ep-1,
    0x1.p-1
  },
  { // Entry 217
    -0x1.62e42befa3a2f3578e7211e2b2908b3cp-1,
    0x1.000002p-1
  },
  { // Entry 218
    -0x1.62e430efa39f7357941cbc859d3b42e7p0,
    0x1.fffffep-3
  },
  { // Entry 219
    -0x1.62e42fefa39ef35793c7673007e5ed5ep0,
    0x1.p-2
  },
  { // Entry 220
    -0x1.62e42defa3a0f357911cbc895d3b3c4dp0,
    0x1.000002p-2
  },
  { // Entry 221
    -0x1.0a2b2473bab77681af00380ed0971ccbp1,
    0x1.fffffep-4
  },
  { // Entry 222
    -0x1.0a2b23f3bab73681aed58d6405ec7206p1,
    0x1.p-3
  },
  { // Entry 223
    -0x1.0a2b22f3bab83681ad803810b097197ep1,
    0x1.000002p-3
  },
  { // Entry 224
    -0x1.62e4306fa39f335793f211dad2909822p1,
    0x1.fffffep-5
  },
  { // Entry 225
    -0x1.62e42fefa39ef35793c7673007e5ed5ep1,
    0x1.p-4
  },
  { // Entry 226
    -0x1.62e42eefa39ff357927211dcb29094d5p1,
    0x1.000002p-4
  },
  { // Entry 227
    -0x1.bb9d3c6b8c86f02d78e3eba6d48a137ap1,
    0x1.fffffep-6
  },
  { // Entry 228
    -0x1.bb9d3beb8c86b02d78b940fc09df68b6p1,
    0x1.p-5
  },
  { // Entry 229
    -0x1.bb9d3aeb8c87b02d7763eba8b48a102dp1,
    0x1.000002p-5
  },
  { // Entry 230
    -0x1.0a2b2433bab75681aeeae2b96b41c769p2,
    0x1.fffffep-7
  },
  { // Entry 231
    -0x1.0a2b23f3bab73681aed58d6405ec7206p2,
    0x1.p-6
  },
  { // Entry 232
    -0x1.0a2b2373bab7b681ae2ae2ba5b41c5c2p2,
    0x1.000002p-6
  },
  { // Entry 233
    -0x1.3687aa31af2b34eca163cf9f6c3e8514p2,
    0x1.fffffep-8
  },
  { // Entry 234
    -0x1.3687a9f1af2b14eca14e7a4a06e92fb2p2,
    0x1.p-7
  },
  { // Entry 235
    -0x1.3687a971af2b94eca0a3cfa05c3e836ep2,
    0x1.000002p-7
  },
  { // Entry 236
    -0x1.62e4302fa39f135793dcbc856d3b42c0p2,
    0x1.fffffep-9
  },
  { // Entry 237
    -0x1.62e42fefa39ef35793c7673007e5ed5ep2,
    0x1.p-8
  },
  { // Entry 238
    -0x1.62e42f6fa39f7357931cbc865d3b411ap2,
    0x1.000002p-8
  },
  { // Entry 239
    -0x1.8f40b62d9812f1c28655a96b6e38006cp2,
    0x1.fffffep-10
  },
  { // Entry 240
    -0x1.8f40b5ed9812d1c28640541608e2ab0ap2,
    0x1.p-9
  },
  { // Entry 241
    -0x1.8f40b56d981351c28595a96c5e37fec6p2,
    0x1.000002p-9
  },
  { // Entry 242
    -0x1.bb9d3c2b8c86d02d78ce96516f34be18p2,
    0x1.fffffep-11
  },
  { // Entry 243
    -0x1.bb9d3beb8c86b02d78b940fc09df68b6p2,
    0x1.p-10
  },
  { // Entry 244
    -0x1.bb9d3b6b8c87302d780e96525f34bc71p2,
    0x1.000002p-10
  },
  { // Entry 245
    -0x1.20596712b4f135b7281cae81b9157b8dp3,
    0x1.fffffep-14
  },
  { // Entry 246
    -0x1.205966f2b4f125b7281203d7066ad0dcp3,
    0x1.p-13
  },
  { // Entry 247
    -0x1.205966b2b4f165b727bcae8231157abap3,
    0x1.000002p-13
  },
  { // Entry 248
    -0x1.20596712b4f135b7281cae81b9157b8dp3,
    0x1.fffffep-14
  },
  { // Entry 249
    -0x1.205966f2b4f125b7281203d7066ad0dcp3,
    0x1.p-13
  },
  { // Entry 250
    -0x1.205966b2b4f165b727bcae8231157abap3,
    0x1.000002p-13
  },
  { // Entry 251
    -0x1.0a2b2473bab77681af00380ed0971ccbp1,
    0x1.fffffep-4
  },
  { // Entry 252
    -0x1.0a2b23f3bab73681aed58d6405ec7206p1,
    0x1.p-3
  },
  { // Entry 253
    -0x1.0a2b22f3bab83681ad803810b097197ep1,
    0x1.000002p-3
  },
  { // Entry 254
    -0x1.1178f14710961bedaf9f799a53afc33dp-3,
    0x1.bffffep-1
  },
  { // Entry 255
    -0x1.1178e8227e47bde338b41fc72de81e3bp-3,
    0x1.c0p-1
  },
  { // Entry 256
    -0x1.1178defdec03d2c915604897971b7658p-3,
    0x1.c00002p-1
  },
  { // Entry 257
    -0x1.62e4306fa39f335793f211dad2909822p1,
    0x1.fffffep-5
  },
  { // Entry 258
    -0x1.62e42fefa39ef35793c7673007e5ed5ep1,
    0x1.p-4
  },
  { // Entry 259
    -0x1.62e42eefa39ff357927211dcb29094d5p1,
    0x1.000002p-4
  },
  { // Entry 260
    -0x1.08599c6af4ba93c693b13122f6824a8bp-4,
    0x1.dffffep-1
  },
  { // Entry 261
    -0x1.08598b59e3a0688a3fd9bf503372c12fp-4,
    0x1.e0p-1
  },
  { // Entry 262
    -0x1.08597a48d29871a4649d0a66ccbd5edbp-4,
    0x1.e00002p-1
  },
  { // Entry 263
    -0x1.bb9d3c6b8c86f02d78e3eba6d48a137ap1,
    0x1.fffffep-6
  },
  { // Entry 264
    -0x1.bb9d3beb8c86b02d78b940fc09df68b6p1,
    0x1.p-5
  },
  { // Entry 265
    -0x1.bb9d3aeb8c87b02d7763eba8b48a102dp1,
    0x1.000002p-5
  },
  { // Entry 266
    -0x1.0415f9a6b665d7a7b9958d7d28f62a74p-5,
    0x1.effffep-1
  },
  { // Entry 267
    -0x1.0415d89e7444470173c75d4d8889de0ep-5,
    0x1.f0p-1
  },
  { // Entry 268
    -0x1.0415b7963244cf65919a578b2daa29d5p-5,
    0x1.f00002p-1
  },
  { // Entry 269
    -0x1.0a2b2433bab75681aeeae2b96b41c769p2,
    0x1.fffffep-7
  },
  { // Entry 270
    -0x1.0a2b23f3bab73681aed58d6405ec7206p2,
    0x1.p-6
  },
  { // Entry 271
    -0x1.0a2b2373bab7b681ae2ae2ba5b41c5c2p2,
    0x1.000002p-6
  },
  { // Entry 272
    -0x1.0205a68d45e67ed01e10e322bd43a170p-6,
    0x1.f7fffep-1
  },
  { // Entry 273
    -0x1.020565893584749f23a105b9c7bb9a6fp-6,
    0x1.f8p-1
  },
  { // Entry 274
    -0x1.02052485256476af6f4daabd139b5e57p-6,
    0x1.f80002p-1
  },
  { // Entry 275
    -0x1.3687aa31af2b34eca163cf9f6c3e8514p2,
    0x1.fffffep-8
  },
  { // Entry 276
    -0x1.3687a9f1af2b14eca14e7a4a06e92fb2p2,
    0x1.p-7
  },
  { // Entry 277
    -0x1.3687a971af2b94eca0a3cfa05c3e836ep2,
    0x1.000002p-7
  },
  { // Entry 278
    -0x1.0101d85a923025b54cbaae499d5e40bfp-7,
    0x1.fbfffep-1
  },
  { // Entry 279
    -0x1.010157588de7128ccc5a82f9da00f48bp-7,
    0x1.fcp-1
  },
  { // Entry 280
    -0x1.0100d6568a200574745b39f0aae26685p-7,
    0x1.fc0002p-1
  },
  { // Entry 281
    -0x1.62e4302fa39f135793dcbc856d3b42c0p2,
    0x1.fffffep-9
  },
  { // Entry 282
    -0x1.62e42fefa39ef35793c7673007e5ed5ep2,
    0x1.p-8
  },
  { // Entry 283
    -0x1.62e42f6fa39f7357931cbc865d3b411ap2,
    0x1.000002p-8
  },
  { // Entry 284
    -0x1.008156968a355a68f2be9b035772c9c3p-8,
    0x1.fdfffep-1
  },
  { // Entry 285
    -0x1.0080559588b357e598e33d8d9db37a29p-8,
    0x1.fep-1
  },
  { // Entry 286
    -0x1.007f549488335866440de7a1f2084736p-8,
    0x1.fe0002p-1
  },
  { // Entry 287
    -0x1.8f40b62d9812f1c28655a96b6e38006cp2,
    0x1.fffffep-10
  },
  { // Entry 288
    -0x1.8f40b5ed9812d1c28640541608e2ab0ap2,
    0x1.p-9
  },
  { // Entry 289
    -0x1.8f40b56d981351c28595a96c5e37fec6p2,
    0x1.000002p-9
  },
  { // Entry 290
    -0x1.0042165dd9caff419b1eccdf720a36d2p-9,
    0x1.fefffep-1
  },
  { // Entry 291
    -0x1.0040155d5889de70671eeec0bfcefe53p-9,
    0x1.ffp-1
  },
  { // Entry 292
    -0x1.003e145cd94abf2033bf71dc3028520cp-9,
    0x1.ff0002p-1
  },
  { // Entry 293
    -0x1.bb9d3c2b8c86d02d78ce96516f34be18p2,
    0x1.fffffep-11
  },
  { // Entry 294
    -0x1.bb9d3beb8c86b02d78b940fc09df68b6p2,
    0x1.p-10
  },
  { // Entry 295
    -0x1.bb9d3b6b8c87302d780e96525f34bc71p2,
    0x1.000002p-10
  },
  { // Entry 296
    -0x1.0024065697999797f377cb852750240bp-10,
    0x1.ff7ffep-1
  },
  { // Entry 297
    -0x1.00200556558893357cd7e1f486bd0705p-10,
    0x1.ff80p-1
  },
  { // Entry 298
    -0x1.001c045617798f93464c0067a7eaae4fp-10,
    0x1.ff8002p-1
  },
  { // Entry 299
    -0x1.20596712b4f135b7281cae81b9157b8dp3,
    0x1.fffffep-14
  },
  { // Entry 300
    -0x1.205966f2b4f125b7281203d7066ad0dcp3,
    0x1.p-13
  },
  { // Entry 301
    -0x1.205966b2b4f165b727bcae8231157abap3,
    0x1.000002p-13
  },
  { // Entry 302
    -0x1.002401156dd698a14a193857b6b2e2bdp-13,
    0x1.ffeffep-1
  },
  { // Entry 303
    -0x1.0004001555d558889dde702b028c9996p-13,
    0x1.fff0p-1
  },
  { // Entry 304
    -0x1.ffc7fe2abbac310fe54784015d23a61dp-14,
    0x1.fff002p-1
  },
  { // Entry 305
    HUGE_VALF,
    HUGE_VALF
  },
  { // Entry 306
    0x1.62e42feba39ef15793c611dab1909808p6,
    0x1.fffffep127
  },
  { // Entry 307
    0x1.62e42fe7a39eeb5793bcbc854d3b429ap6,
    0x1.fffffcp127
  },
  { // Entry 308
    0x1.250d0505fece83f5f3328cc322f65153p0,
    0x1.921fb6p1
  },
  { // Entry 309
    0x1.ce6bb438b3fc2928a53b64ac7c0d6a91p-2,
    0x1.921fb6p0
  },
  { // Entry 310
    0x1.fffffe000002aaaaa6aaaab111110666p-24,
    0x1.000002p0
  },
  { // Entry 311
    0.0,
    0x1.p0
  },
  { // Entry 312
    -0x1.000000800000555555955555888888b3p-24,
    0x1.fffffep-1
  },
  { // Entry 313
    -0x1.eeb9574d26837b0d04a6d367277ce056p-3,
    0x1.921fb6p-1
  },
  { // Entry 314
    -0x1.5d589f27e5107f8a356d9ee8ad1baae4p6,
    0x1.000002p-126
  },
  { // Entry 315
    -0x1.5d589f2fe510778a3578499347c655a9p6,
    0x1.p-126
  },
  { // Entry 316
    -0x1.5d589f37e5107f8a3582f43e0271006dp6,
    0x1.fffffcp-127
  },
  { // Entry 317
    -0x1.5d589f3fe510978a35cd9ee99d1bae31p6,
    0x1.fffff8p-127
  },
  { // Entry 318
    -0x1.9a57d76d152fc95d42de8f4f8921da75p6,
    0x1.p-148
  },
  { // Entry 319
    -0x1.9d1d9fccf4770743f2061e1de931a650p6,
    0x1.p-149
  },
  { // Entry 320
    -HUGE_VALF,
    0.0f
  },
  { // Entry 321
    -HUGE_VALF,
    -0.0f
  },
};
#endif // __BIONIC__

TEST(math_logf, logf_intel) {
#if defined(__BIONIC__)
  for (size_t i = 0; i < sizeof(g_logf_intel_data)/sizeof(logf_intel_data_t); i++) {
    EXPECT_FLOAT_EQ(g_logf_intel_data[i].expected, logf(g_logf_intel_data[i].call_data)) << "Failed on element " << i;
  }
#else // __BIONIC__
  GTEST_LOG_(INFO) << "This test does nothing.";
#endif // __BIONIC__
}
