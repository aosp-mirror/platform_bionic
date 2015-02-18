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

static data_1_1_t<double, double> g_expm1_intel_data[] = {
  { // Entry 0
    -0x1.ffffffffff0000000000555555555540p-41,
    -0x1.0p-40
  },
  { // Entry 1
    -0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.0p30
  },
  { // Entry 2
    -0x1.fe0154aaeed8738220213bf805c9a017p-8,
    -0x1.0000000000002p-7
  },
  { // Entry 3
    -0x1.ffffffffffa000000000055555555556p-42,
    -0x1.00000000001p-41
  },
  { // Entry 4
    -0x1.fe0154aaef571540b64b8485cc828f04p-8,
    -0x1.00000000003ffp-7
  },
  { // Entry 5
    -0x1.43a54e4eb0119800b5b8f35e2b4e7e81p-1,
    -0x1.000000002p0
  },
  { // Entry 6
    -0x1.bacf4c925373696fd21f24ae89354a32p-1,
    -0x1.003p1
  },
  { // Entry 7
    -0x1.fcc2556e8534300e63c12f8a5a1106b6p-7,
    -0x1.006p-6
  },
  { // Entry 8
    -0x1.bdaeea20744956636e2e888fc1809651p-1,
    -0x1.059def2b2f2c4p1
  },
  { // Entry 9
    -0x1.07ffffffff77e00000002ecafffffff3p-40,
    -0x1.080p-40
  },
  { // Entry 10
    -0x1.1b19e5e90e6538002ec4e9f63c1927a9p-3,
    -0x1.30ae80687cd57p-3
  },
  { // Entry 11
    -0x1.d23c83e5c923a8d750b23742ad5d2d3dp-1,
    -0x1.3519530a863ffp1
  },
  { // Entry 12
    -0x1.34533cf44744c7f7dfe24cc81ce93a5ep-4,
    -0x1.408c0a43cd97cp-4
  },
  { // Entry 13
    -0x1.79c6caa9e49af4463cee3d526a8e1762p-1,
    -0x1.56bccf9c08f94p0
  },
  { // Entry 14
    -0x1.7de898bb4a1814449fc9bf5a787f2ce3p-1,
    -0x1.5ebe08ce59440p0
  },
  { // Entry 15
    -0x1.fde856b4a6b0d7fc360a96572944a5edp-1,
    -0x1.5ffffffffff80p2
  },
  { // Entry 16
    -0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.6p9
  },
  { // Entry 17
    -0x1.7f637276db37ab8b4f8af38ad62afdb3p-1,
    -0x1.61abd3bb638ffp0
  },
  { // Entry 18
    -0x1.7fd3858818630ad9afd1179848ee2effp-1,
    -0x1.628b4a70e8586p0
  },
  { // Entry 19
    -0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.62ep9
  },
  { // Entry 20
    -0x1.785b18f5275d64a3f5fda9ef739d80b5p-8,
    -0x1.7970cf2265b9ap-8
  },
  { // Entry 21
    -0x1.8b92870fa2b597fe9b02c25e9ad8a3cep-4,
    -0x1.ap-4
  },
  { // Entry 22
    -0x1.b57abe9ba86d56e26962c5525f1347a7p-8,
    -0x1.b6f238c2a040ap-8
  },
  { // Entry 23
    -0x1.b57abe9ba88897ff1194f673b12e2258p-8,
    -0x1.b6f238c2a05c1p-8
  },
  { // Entry 24
    -0x1.b57abe9ba88917245435a89f6cc2597ep-8,
    -0x1.b6f238c2a05c9p-8
  },
  { // Entry 25
    -0x1.b76f5651d19c26f8956b99cf61f3f2efp-6,
    -0x1.bd6f7bfa7895ep-6
  },
  { // Entry 26
    -0x1.b76f5651d3bc6710b0229be3f9607089p-6,
    -0x1.bd6f7bfa7ac52p-6
  },
  { // Entry 27
    -0x1.a78c2b7ae21669f0f3cd37c10528267ep-1,
    -0x1.c18p0
  },
  { // Entry 28
    -0x1.2b8ded3132d61ffff49548b6ec6ec8c6p-1,
    -0x1.c26p-1
  },
  { // Entry 29
    -0x1.ac6b158d953de99425206483bdcd2b28p-1,
    -0x1.dp0
  },
  { // Entry 30
    -0x1.ffefffffffffe8007ffe000000954f55p-54,
    -0x1.ffeffffffffffp-54
  },
  { // Entry 31
    -0x1.bab52178ee9089cf090261ec85161b44p-1,
    -0x1.ffff3ffffffffp0
  },
  { // Entry 32
    -0x1.f69f5523ef47a800c36704994de6bf7cp-1,
    -0x1.fffffffffe9efp1
  },
  { // Entry 33
    -0x1.fe0154aaee98b381f5a12261d8cf0352p-8,
    -0x1.ffffffffffcp-8
  },
  { // Entry 34
    -0x1.92e9a0720d3027f60f578a2cd30d104ep-2,
    -0x1.ffffffffffe7fp-2
  },
  { // Entry 35
    -0x1.bab5557101f8c8d73cd274ba659b2727p-1,
    -0x1.ffffffffffffep0
  },
  { // Entry 36
    -0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.ffffffffffffep7
  },
  { // Entry 37
    0x1.000000000080000000002aaaaaaaaab5p-40,
    0x1.0p-40
  },
  { // Entry 38
    0x1.000000000000080000000000002aaaaap-52,
    0x1.0p-52
  },
  { // Entry 39
    0x1.9476504ba885758aa5fa7545e10e8e46p738,
    0x1.0000000000001p9
  },
  { // Entry 40
    0x1.0f2ebb2c65d9a80081ac2e65e8025ab1p23,
    0x1.000000020p4
  },
  { // Entry 41
    0x1.e5208c8ebb607ad767c1adb2ae9616f4p739,
    0x1.007p9
  },
  { // Entry 42
    0x1.040000000084080000002cb2b5555560p-40,
    0x1.040p-40
  },
  { // Entry 43
    0x1.c61e8108cb3b100f4a9641fe4b59d5a2p0,
    0x1.052f742bb53d6p0
  },
  { // Entry 44
    0x1.06466f97b426d000000a905602d7fb69p-10,
    0x1.0624decad85d9p-10
  },
  { // Entry 45
    0x1.65591a3a7b9fabe891c2ea5f47a6bb96p-1,
    0x1.0f0ffffffffffp-1
  },
  { // Entry 46
    0x1.66f0fb901f2bd45d99c3ae0c5506ad7fp-1,
    0x1.1p-1
  },
  { // Entry 47
    0x1.26beacef84dda800ee87d91d88c199a7p-6,
    0x1.242p-6
  },
  { // Entry 48
    0x1.11f6270d25be700ef7c34d02a29974f1p53,
    0x1.266fd7cddff42p5
  },
  { // Entry 49
    0x1.2d26216139d81006bcd5c876ca600a38p53,
    0x1.27319e818c230p5
  },
  { // Entry 50
    0x1.4d13fbb1a00192785df27257f060e683p53,
    0x1.280p5
  },
  { // Entry 51
    0x1.60c9b536e33bafefc62bca96f884a22ep53,
    0x1.2875bd6dab630p5
  },
  { // Entry 52
    0x1.8244f738ab986fcb022374240d8605cap53,
    0x1.292f6d8d306c3p5
  },
  { // Entry 53
    0x1.f4b1ecd508504fdbe96629980d48a020p53,
    0x1.2b42ce6e584ebp5
  },
  { // Entry 54
    0x1.88a122d234b394b88696ada7f7c11a0ap865,
    0x1.2c0p9
  },
  { // Entry 55
    0x1.6641633703ea28000010995bb7c4b21dp-2,
    0x1.33333342022a7p-2
  },
  { // Entry 56
    0x1.04dadee28c11c800fb9094435c1de727p7,
    0x1.38389c48b0fcep2
  },
  { // Entry 57
    0x1.007848baed8b37c3e6cc3bfb1a101644p58,
    0x1.41a28cd5395c0p5
  },
  { // Entry 58
    0x1.0c719229fb04b7c21ec3e0d7f2aed918p58,
    0x1.42000000040p5
  },
  { // Entry 59
    0x1.10924600307447c139d9bb2f82ff9a27p58,
    0x1.421f4066cf2fcp5
  },
  { // Entry 60
    0x1.31d215d36b1cc7c023dacc0edfa71bebp58,
    0x1.430af90c17e36p5
  },
  { // Entry 61
    0x1.379553b19df207c01565e5f16d485d4dp58,
    0x1.4331346ca6ce7p5
  },
  { // Entry 62
    0x1.379553b498da57c00fec0a571dd48a6fp58,
    0x1.4331346cba64fp5
  },
  { // Entry 63
    0x1.4bc2fdce156117bf5a49805dd419c072p58,
    0x1.43b1b79351f4ep5
  },
  { // Entry 64
    0x1.cf392076a1bdd7bf0d53e64efec10053p58,
    0x1.465d52b8b0596p5
  },
  { // Entry 65
    0x1.df8028d08d7bf7c01a066c0bca539e5ap58,
    0x1.46a40dae90670p5
  },
  { // Entry 66
    0x1.ea57988e94c817bf8f52f84ed3df88b0p58,
    0x1.46d1d7d9e8a98p5
  },
  { // Entry 67
    0x1.cb419b9279b35763d113e6c5db79dc54p943,
    0x1.471c71c71c71cp9
  },
  { // Entry 68
    0x1.f1345355d78ba4bf7b3fd1e3ecbf7dfdp948,
    0x1.48e2388e2391cp9
  },
  { // Entry 69
    0x1.55ab836495abe800005ca6d200952433p1,
    0x1.4cccccce8ce97p0
  },
  { // Entry 70
    0x1.6a77c2478bdb150bbc8ea756bbb8570bp970,
    0x1.5059aabfe5765p9
  },
  { // Entry 71
    0x1.e1afc1f1512b7428d3d28c586dcd7da0p974,
    0x1.51e0f4c70ecdap9
  },
  { // Entry 72
    0x1.5b1bac52655bf015d0c0897352cff074p-8,
    0x1.5a312e2d5469ep-8
  },
  { // Entry 73
    0x1.5d98a8b1c5b8a043f872dce4155ba02dp-8,
    0x1.5caacc5a85cadp-8
  },
  { // Entry 74
    0x1.f2a3c29fc6d723a63e349bbcd7894a8ap1008,
    0x1.5dadf5d1e452cp9
  },
  { // Entry 75
    0x1.624ca1ace3f15973a463b539c79a29fdp-8,
    0x1.615856f590456p-8
  },
  { // Entry 76
    0x1.624ca1ace613a9790c0e0fcff924bb7fp-8,
    0x1.615856f59264cp-8
  },
  { // Entry 77
    0x1.fffffffffc72a1b0e266677220702371p1023,
    0x1.62e42fefa39e1p9
  },
  { // Entry 78
    0x1.66bb6f898c6b5fb5d846de17be366ad0p-8,
    0x1.65c100ffac3fdp-8
  },
  { // Entry 79
    0x1.03854c2737b8d7ffffa5944ecc584479p0,
    0x1.66666668c8bc0p-1
  },
  { // Entry 80
    0x1.ad445f949fa7d34496e83174e2786b07p-2,
    0x1.668p-2
  },
  { // Entry 81
    0x1.67ad945f2f1d9fe2b66dc062db7996efp-8,
    0x1.66b1d3ec2054fp-8
  },
  { // Entry 82
    0x1.ae35f07f55b872de4707744c0a26ae90p-2,
    0x1.672a28295e9c9p-2
  },
  { // Entry 83
    0x1.78a4af6b33748fe2cb05dd6a4bfa7056p-8,
    0x1.7790abed48f5ap-8
  },
  { // Entry 84
    0x1.7bcae2fa3a8cc8cce95336f706279e5ap-8,
    0x1.7ab23f3a26807p-8
  },
  { // Entry 85
    0x1.7ce527adde0b88d06ecba195c7b0aa4cp-8,
    0x1.7bcae2fa3adbep-8
  },
  { // Entry 86
    0x1.7ce527ade0f5d8d09fcd9764c82d1274p-8,
    0x1.7bcae2fa3dc1ep-8
  },
  { // Entry 87
    0x1.7e0110f8b0e678d4a8586472ae7bc1d2p-8,
    0x1.7ce527ade25b4p-8
  },
  { // Entry 88
    0x1.7f1ea28925a638dc0db4fbf091b90b09p-8,
    0x1.7e0110f8b0c8cp-8
  },
  { // Entry 89
    0x1.803de018c41128de07c3aaf8cd79d45cp-8,
    0x1.7f1ea28926651p-8
  },
  { // Entry 90
    0x1.fe31152b7ef6b1e0a8b9fec7ecdd85a4p553,
    0x1.8p8
  },
  { // Entry 91
    0x1.d38c898541cf95544db45ffc7e46fd16p-2,
    0x1.812p-2
  },
  { // Entry 92
    0x1.8527a1ecdbec28000500f2ef81065e65p-7,
    0x1.82dcb4e52cab1p-7
  },
  { // Entry 93
    0x1.8fe5e61a83cad7fcee78aa274e8dc654p-9,
    0x1.8f4a0b9ff7ed0p-9
  },
  { // Entry 94
    0x1.e9306d671550b7fce52fe384e236ed51p-2,
    0x1.8fep-2
  },
  { // Entry 95
    0x1.aec7b35c8c209fffffe76d4ac148ca7ap-4,
    0x1.9999999be6ebep-4
  },
  { // Entry 96
    0x1.c56ecf3ddea747ffffa1cfcd9266f384p-3,
    0x1.999999a7f45f9p-3
  },
  { // Entry 97
    0x1.f7a0e4d5067effffffc2bf0d03877722p-2,
    0x1.999999a867f17p-2
  },
  { // Entry 98
    0x1.f7a0e4d8af7c480000325c9866d3f666p-2,
    0x1.999999aadc06ap-2
  },
  { // Entry 99
    0x1.fc8ecabe156a92f92ccd95f742d5f70fp-2,
    0x1.9ce61d3061544p-2
  },
  { // Entry 100
    0x1.c8082a8e3022880002e39fa93786b0cep-4,
    0x1.b062a2df1de98p-4
  },
  { // Entry 101
    0x1.b5e1f0f0c1d798a5b169cb191cd0c621p-8,
    0x1.b46d1b46e5ccep-8
  },
  { // Entry 102
    0x1.b5e9fa9919edd8bafdec03458df49c11p-8,
    0x1.b4751746e5ccdp-8
  },
  { // Entry 103
    0x1.b5e9fa9919ede8d65c8bacd72cd2a89cp-8,
    0x1.b4751746e5ccep-8
  },
  { // Entry 104
    0x1.b7594565a4b428b4734e2c9b43bec612p-8,
    0x1.b5e1f0f0af677p-8
  },
  { // Entry 105
    0x1.cf7fce3931c5e7ffe406b08dc477f91bp-8,
    0x1.cdde2e3d70c6dp-8
  },
  { // Entry 106
    0x1.cf44b5362775480990d2193fdac2a6c2p-9,
    0x1.ce739ce735ce2p-9
  },
  { // Entry 107
    0x1.8260dae0f18853ff9edaf44b27ba17b2p0,
    0x1.d70a3d70a3d71p-1
  },
  { // Entry 108
    0x1.dec5c594a41bb72f3ab6a63927f8e80dp-8,
    0x1.dd083d2908a81p-8
  },
  { // Entry 109
    0x1.e08690c3fb77571e49537d4d5f69aaf2p-8,
    0x1.dec5c594ecfbcp-8
  },
  { // Entry 110
    0x1.e6f8da92954ce80b660157d3ae9f4e70p-9,
    0x1.e611d78dcf946p-9
  },
  { // Entry 111
    0x1.95e784ba628073ff77d2f7ed4d6201bcp0,
    0x1.e65f036272239p-1
  },
  { // Entry 112
    0x1.e7d8753271e7a80359bd34ac05d687f5p-9,
    0x1.e6f09e1a48351p-9
  },
  { // Entry 113
    0x1.98df5c213427f4084f77d615f3544c56p0,
    0x1.e8a974c5d39f7p-1
  },
  { // Entry 114
    0x1.9f5b8bec582e4b59bb698dbd2576ed44p0,
    0x1.eda1b9b5dff58p-1
  },
  { // Entry 115
    0x1.f1be12f8f20cf876bccb9e47e3eb30b8p-7,
    0x1.ee0p-7
  },
  { // Entry 116
    0x1.f15c950aadd3178e4e8d488691307274p-8,
    0x1.ef7bdef7bdef2p-8
  },
  { // Entry 117
    0x1.f4647ce7fdefc80415bb8c747bed498ep-9,
    0x1.f3709599bd0a0p-9
  },
  { // Entry 118
    0x1.fbfc81c0062a280bc3db8a3918010b6ap-9,
    0x1.fb01276ad538bp-9
  },
  { // Entry 119
    0x1.4231178c2348f5f77eedb27dc404f616p734,
    0x1.fcfffffffffffp8
  },
  { // Entry 120
    0x1.74218bce788eb46746b38b578759ecc0p11,
    0x1.ffeffffffffffp2
  },
  { // Entry 121
    0x1.00000000009ff7ffffffea9aaaaaaa63p-39,
    0x1.ffffffffff3ffp-40
  },
  { // Entry 122
    0x1.0000000000000ffffffffffffeaaaaaap-50,
    0x1.ffffffffffffep-51
  },
  { // Entry 123
    0x1.ffffffffffffe7ffffffffffff155555p-54,
    0x1.ffffffffffffep-54
  },
  { // Entry 124
    0x1.00000000007ff80000002aa2aaaaaab5p-40,
    0x1.fffffffffffffp-41
  },
  { // Entry 125
    0x1.304d6aeca25253146dec9182369ba415p69,
    0x1.7ffffffffffffp5
  },
  { // Entry 126
    0x1.304d6aeca254b3af43c5d6293d5f65c7p69,
    0x1.8p5
  },
  { // Entry 127
    0x1.304d6aeca257144a199f1ad50558d32cp69,
    0x1.8000000000001p5
  },
  { // Entry 128
    -0x1.ffffffffffffffffffc0e327b6954e21p-1,
    -0x1.a000000000001p5
  },
  { // Entry 129
    -0x1.ffffffffffffffffffc0e327b6954da3p-1,
    -0x1.ap5
  },
  { // Entry 130
    -0x1.ffffffffffffffffffc0e327b6954d25p-1,
    -0x1.9ffffffffffffp5
  },
  { // Entry 131
    0x1.55779b984f395dea36a277b8bee2c64cp115,
    0x1.3ffffffffffffp6
  },
  { // Entry 132
    0x1.55779b984f3eb3c8a503b4a8e2487d98p115,
    0x1.4p6
  },
  { // Entry 133
    0x1.55779b984f4409a71364f1ae5d27ee69p115,
    0x1.4000000000001p6
  },
  { // Entry 134
    -0x1.fffffffffffffffffffffffffffffffbp-1,
    -0x1.6000000000001p6
  },
  { // Entry 135
    -0x1.fffffffffffffffffffffffffffffffbp-1,
    -0x1.6p6
  },
  { // Entry 136
    -0x1.fffffffffffffffffffffffffffffffbp-1,
    -0x1.5ffffffffffffp6
  },
  { // Entry 137
    0x1.40a4b9c27150866176d22f2139d1d40fp923,
    0x1.3ffffffffffffp9
  },
  { // Entry 138
    0x1.40a4b9c271789af8af205bb34f743337p923,
    0x1.4p9
  },
  { // Entry 139
    0x1.40a4b9c271a0af8fe76e8d47f7fd9c26p923,
    0x1.4000000000001p9
  },
  { // Entry 140
    -0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.6000000000001p9
  },
  { // Entry 141
    -0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.6p9
  },
  { // Entry 142
    -0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.5ffffffffffffp9
  },
  { // Entry 143
    0x1.03996528e072b78a330480884c79baf7p75,
    0x1.9ffffffffffffp5
  },
  { // Entry 144
    0x1.03996528e074bebcfd56416fc2c0eb92p75,
    0x1.ap5
  },
  { // Entry 145
    0x1.03996528e076c5efc7a8025b476db0d0p75,
    0x1.a000000000001p5
  },
  { // Entry 146
    -0x1.fffffffffffffffff28a2a28e2df408cp-1,
    -0x1.8000000000001p5
  },
  { // Entry 147
    -0x1.fffffffffffffffff28a2a28e2df25a0p-1,
    -0x1.8p5
  },
  { // Entry 148
    -0x1.fffffffffffffffff28a2a28e2df0ab5p-1,
    -0x1.7ffffffffffffp5
  },
  { // Entry 149
    0x1.f1056dc7bf1b0fc857b67999f503526fp126,
    0x1.5ffffffffffffp6
  },
  { // Entry 150
    0x1.f1056dc7bf22d3de0ed57615bc501f87p126,
    0x1.6p6
  },
  { // Entry 151
    0x1.f1056dc7bf2a97f3c5f472b093f3c91bp126,
    0x1.6000000000001p6
  },
  { // Entry 152
    -0x1.ffffffffffffffffffffffffffffd004p-1,
    -0x1.4000000000001p6
  },
  { // Entry 153
    -0x1.ffffffffffffffffffffffffffffd004p-1,
    -0x1.4p6
  },
  { // Entry 154
    -0x1.ffffffffffffffffffffffffffffd004p-1,
    -0x1.3ffffffffffffp6
  },
  { // Entry 155
    0x1.93bf4ec282bd3b36cd2f4011488a8364p1015,
    0x1.5ffffffffffffp9
  },
  { // Entry 156
    0x1.93bf4ec282efb320a57f9ae02e01ae51p1015,
    0x1.6p9
  },
  { // Entry 157
    0x1.93bf4ec283222b0a7dcffbfe10b3e34ap1015,
    0x1.6000000000001p9
  },
  { // Entry 158
    -0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.4000000000001p9
  },
  { // Entry 159
    -0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.4p9
  },
  { // Entry 160
    -0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.3ffffffffffffp9
  },
  { // Entry 161
    -0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.1cb90bfbe8e7cp9
  },
  { // Entry 162
    -0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.397217f7d1cf8p9
  },
  { // Entry 163
    -0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.562b23f3bab73p9
  },
  { // Entry 164
    -0x1.0000000000000654361c4c67fbf90232p-1,
    -0x1.62e42fefa39f0p-1
  },
  { // Entry 165
    -0x1.fffffffffffffca86c3898cff81747c6p-2,
    -0x1.62e42fefa39efp-1
  },
  { // Entry 166
    -0x1.ffffffffffffeca86c3898cff7bc8b28p-2,
    -0x1.62e42fefa39eep-1
  },
  { // Entry 167
    -0x1.2bec3330188676e1ed35fec1b10e40dcp-2,
    -0x1.62e42fefa39f0p-2
  },
  { // Entry 168
    -0x1.2bec333018866b919e02bf23cad327f3p-2,
    -0x1.62e42fefa39efp-2
  },
  { // Entry 169
    -0x1.2bec3330188660414ecf7f85e46acdcdp-2,
    -0x1.62e42fefa39eep-2
  },
  { // Entry 170
    -0x1.45d819a94b14b3030eebbb9c6d4a2ce8p-3,
    -0x1.62e42fefa39f0p-3
  },
  { // Entry 171
    -0x1.45d819a94b14a58ebf1f0e3296a2beb6p-3,
    -0x1.62e42fefa39efp-3
  },
  { // Entry 172
    -0x1.45d819a94b14981a6f5260c8bfe067e5p-3,
    -0x1.62e42fefa39eep-3
  },
  { // Entry 173
    -0x1.53f391822dbc78ae783b45864b0aa398p-4,
    -0x1.62e42fefa39f0p-4
  },
  { // Entry 174
    -0x1.53f391822dbc6a026bccc7b4077bfc0ap-4,
    -0x1.62e42fefa39efp-4
  },
  { // Entry 175
    -0x1.53f391822dbc5b565f5e49e1c3dea870p-4,
    -0x1.62e42fefa39eep-4
  },
  { // Entry 176
    -0x1.5b505d5b6f26868f9677878648368b11p-5,
    -0x1.62e42fefa39f0p-5
  },
  { // Entry 177
    -0x1.5b505d5b6f26773d3ea6353ddb75fe46p-5,
    -0x1.62e42fefa39efp-5
  },
  { // Entry 178
    -0x1.5b505d5b6f2667eae6d4e2f56eadc84fp-5,
    -0x1.62e42fefa39eep-5
  },
  { // Entry 179
    -0x1.5f134923757f3dc3f347d61df5fb6626p-6,
    -0x1.62e42fefa39f0p-6
  },
  { // Entry 180
    -0x1.5f134923757f2e1bb81a1efb55c8e21cp-6,
    -0x1.62e42fefa39efp-6
  },
  { // Entry 181
    -0x1.5f134923757f1e737cec67d8b5927402p-6,
    -0x1.62e42fefa39eep-6
  },
  { // Entry 182
    0x1.66c34c5615d0d7db1473bac29ad1b98dp-6,
    0x1.62e42fefa39eep-6
  },
  { // Entry 183
    0x1.66c34c5615d0e834c546d0480f09bb89p-6,
    0x1.62e42fefa39efp-6
  },
  { // Entry 184
    0x1.66c34c5615d0f88e7619e5cd8345d3f0p-6,
    0x1.62e42fefa39f0p-6
  },
  { // Entry 185
    0x1.6ab0d9f3121eb0fea4f25282282cb459p-5,
    0x1.62e42fefa39eep-5
  },
  { // Entry 186
    0x1.6ab0d9f3121ec1b3fd5f4c0b37896101p-5,
    0x1.62e42fefa39efp-5
  },
  { // Entry 187
    0x1.6ab0d9f3121ed26955cc459446ee6856p-5,
    0x1.62e42fefa39f0p-5
  },
  { // Entry 188
    0x1.72b83c7d517ac7c7c0d3432ad543afa6p-4,
    0x1.62e42fefa39eep-4
  },
  { // Entry 189
    0x1.72b83c7d517ad93a790fc07c501430c3p-4,
    0x1.62e42fefa39efp-4
  },
  { // Entry 190
    0x1.72b83c7d517aeaad314c3dcdcaf62498p-4,
    0x1.62e42fefa39f0p-4
  },
  { // Entry 191
    0x1.837f0518db8a7ff3f7635f5fbe54ebe8p-3,
    0x1.62e42fefa39eep-3
  },
  { // Entry 192
    0x1.837f0518db8a92faf56d9116d367dad5p-3,
    0x1.62e42fefa39efp-3
  },
  { // Entry 193
    0x1.837f0518db8aa601f377c2cde8a0d7bep-3,
    0x1.62e42fefa39f0p-3
  },
  { // Entry 194
    0x1.a827999fcef308c835779a431e05cabbp-2,
    0x1.62e42fefa39eep-2
  },
  { // Entry 195
    0x1.a827999fcef31f68d3de197eea562ccep-2,
    0x1.62e42fefa39efp-2
  },
  { // Entry 196
    0x1.a827999fcef33609724498bab701115ap-2,
    0x1.62e42fefa39f0p-2
  },
  { // Entry 197
    0x1.ffffffffffffd950d871319ff0ef3435p-1,
    0x1.62e42fefa39eep-1
  },
  { // Entry 198
    0x1.fffffffffffff950d871319ff039baf9p-1,
    0x1.62e42fefa39efp-1
  },
  { // Entry 199
    0x1.0000000000000ca86c3898cff84220dep0,
    0x1.62e42fefa39f0p-1
  },
  { // Entry 200
    0x1.7fffffffffffd950d871319ff1aa4328p1,
    0x1.62e42fefa39eep0
  },
  { // Entry 201
    0x1.7ffffffffffff950d871319ff03f50afp1,
    0x1.62e42fefa39efp0
  },
  { // Entry 202
    0x1.8000000000001950d871319ff0d45e36p1,
    0x1.62e42fefa39f0p0
  },
  { // Entry 203
    0x1.dfffffffffffb2a1b0e2633fe640c21bp3,
    0x1.62e42fefa39eep1
  },
  { // Entry 204
    0x1.dffffffffffff2a1b0e2633fe094f837p3,
    0x1.62e42fefa39efp1
  },
  { // Entry 205
    0x1.e0000000000032a1b0e2633fe2e92e54p3,
    0x1.62e42fefa39f0p1
  },
  { // Entry 206
    0x1.fdffffffffff654361c4c67fd8327361p7,
    0x1.62e42fefa39eep2
  },
  { // Entry 207
    0x1.fdffffffffffe54361c4c67fc1834bd3p7,
    0x1.62e42fefa39efp2
  },
  { // Entry 208
    0x1.fe0000000000654361c4c67fcad42444p7,
    0x1.62e42fefa39f0p2
  },
  { // Entry 209
    0x1.fffdfffffffeca86c3898cffdf28a36fp15,
    0x1.62e42fefa39eep3
  },
  { // Entry 210
    0x1.fffdffffffffca86c3898cff846c0534p15,
    0x1.62e42fefa39efp3
  },
  { // Entry 211
    0x1.fffe00000000ca86c3898cffa9af66f9p15,
    0x1.62e42fefa39f0p3
  },
  { // Entry 212
    0x1.fffffffdfffd950d87131a007960398fp31,
    0x1.62e42fefa39eep4
  },
  { // Entry 213
    0x1.fffffffdffff950d871319ff0e6dc0a3p31,
    0x1.62e42fefa39efp4
  },
  { // Entry 214
    0x1.fffffffe0001950d871319ffa37b47b6p31,
    0x1.62e42fefa39f0p4
  },
  { // Entry 215
    0x1.ffffffffffb2a1b0e26345b8dfe00697p1023,
    0x1.62e42fefa39eep9
  },
  { // Entry 216
    0x1.fffffffffff2a1b0e263400d15fc52ffp1023,
    0x1.62e42fefa39efp9
  },
  { // Entry 217
    HUGE_VAL,
    0x1.62e42fefa39f0p9
  },
  { // Entry 218
    -0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.72e42fefa39f0p9
  },
  { // Entry 219
    -0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.72e42fefa39efp9
  },
  { // Entry 220
    -0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.72e42fefa39eep9
  },
  { // Entry 221
    -0x1.c5041854df7d5ed1e4b8c796ef6ef281p-3,
    -0x1.0000000000001p-2
  },
  { // Entry 222
    -0x1.c5041854df7d45e5f51a1b14e4b86234p-3,
    -0x1.0p-2
  },
  { // Entry 223
    -0x1.c5041854df7d396ffd4ac4d3df37b827p-3,
    -0x1.fffffffffffffp-3
  },
  { // Entry 224
    -0x1.e14aed893eef58797f12838f2b969ac0p-4,
    -0x1.0000000000001p-3
  },
  { // Entry 225
    -0x1.e14aed893eef3c3c14ed960d0a2b5054p-4,
    -0x1.0p-3
  },
  { // Entry 226
    -0x1.e14aed893eef2e1d5fdb1f4bf9607d0ep-4,
    -0x1.fffffffffffffp-4
  },
  { // Entry 227
    -0x1.f0540438fd5c4fb179fdc0f96e33a687p-5,
    -0x1.0000000000001p-4
  },
  { // Entry 228
    -0x1.f0540438fd5c31a1ce01f9f6ca74502bp-5,
    -0x1.0p-4
  },
  { // Entry 229
    -0x1.f0540438fd5c2299f804167578895f1dp-5,
    -0x1.fffffffffffffp-5
  },
  { // Entry 230
    -0x1.f8152aee9450fc6df41295c712a2cfcbp-6,
    -0x1.0000000000001p-5
  },
  { // Entry 231
    -0x1.f8152aee9450dd69fea80d113b1945c7p-6,
    -0x1.0p-5
  },
  { // Entry 232
    -0x1.f8152aee9450cde803f2c8b64f4eb008p-6,
    -0x1.fffffffffffffp-6
  },
  { // Entry 233
    -0x1.fc055004416dd58cbbb4a9b4ef23fb67p-7,
    -0x1.0000000000001p-6
  },
  { // Entry 234
    -0x1.fc055004416db60bbd08aac54a956e76p-7,
    -0x1.0p-6
  },
  { // Entry 235
    -0x1.fc055004416da64b3db2ab4d784b33e6p-7,
    -0x1.fffffffffffffp-7
  },
  { // Entry 236
    -0x1.fe0154aaeed853c1e04bd155e0d61457p-8,
    -0x1.0000000000001p-7
  },
  { // Entry 237
    -0x1.fe0154aaeed83401a07666b3bbde908fp-8,
    -0x1.0p-7
  },
  { // Entry 238
    -0x1.fe0154aaeed82421808bb162a96151a8p-8,
    -0x1.fffffffffffffp-8
  },
  { // Entry 239
    -0x1.ff0055400443ae32f1e9274ffa299d5dp-9,
    -0x1.0000000000001p-8
  },
  { // Entry 240
    -0x1.ff00554004438e52e1ee7b503e63818cp-9,
    -0x1.0p-8
  },
  { // Entry 241
    -0x1.ff00554004437e62d9f12550607fb463p-9,
    -0x1.fffffffffffffp-9
  },
  { // Entry 242
    -0x1.ff801552aaef092effe8945b04b60168p-10,
    -0x1.0000000000001p-9
  },
  { // Entry 243
    -0x1.ff801552aaeee93efbe93ef05c2dcb20p-10,
    -0x1.0p-9
  },
  { // Entry 244
    -0x1.ff801552aaeed946f9e9943b07e9502cp-10,
    -0x1.fffffffffffffp-10
  },
  { // Entry 245
    -0x1.ffc005550004640ec40c0e6e9887b0c9p-11,
    -0x1.0000000000001p-10
  },
  { // Entry 246
    -0x1.ffc0055500044416c30c23c298990114p-11,
    -0x1.0p-10
  },
  { // Entry 247
    -0x1.ffc005550004341ac28c2e6c98a17946p-11,
    -0x1.fffffffffffffp-11
  },
  { // Entry 248
    -0x1.fff80015552acaedee97e99bef6c42ffp-14,
    -0x1.0000000000001p-13
  },
  { // Entry 249
    -0x1.fff80015552aaaeeee93e9a69a01a076p-14,
    -0x1.0p-13
  },
  { // Entry 250
    -0x1.fff80015552a9aef6e91e9abef4c4932p-14,
    -0x1.fffffffffffffp-14
  },
  { // Entry 251
    0x1.22d78f0fa0618f943ff0bb2de7cfdf0cp-2,
    0x1.fffffffffffffp-3
  },
  { // Entry 252
    0x1.22d78f0fa06199d9ef0eda6eaaf94d3bp-2,
    0x1.0p-2
  },
  { // Entry 253
    0x1.22d78f0fa061ae654d4b18f03189cbb3p-2,
    0x1.0000000000001p-2
  },
  { // Entry 254
    0x1.10b022db7ae673d6bb2140ac1ce40bp-3,
    0x1.fffffffffffffp-4
  },
  { // Entry 255
    0x1.10b022db7ae67ce76b441c27035c6a13p-3,
    0x1.0p-3
  },
  { // Entry 256
    0x1.10b022db7ae68f08cb89d31cd0685a4ap-3,
    0x1.0000000000001p-3
  },
  { // Entry 257
    0x1.082b577d34ed74d70455df87e5de0894p-4,
    0x1.fffffffffffffp-5
  },
  { // Entry 258
    0x1.082b577d34ed7d5b1a019e225c9a951bp-4,
    0x1.0p-4
  },
  { // Entry 259
    0x1.082b577d34ed8e6345591b574a20744bp-4,
    0x1.0000000000001p-4
  },
  { // Entry 260
    0x1.040ac0224fd9298077606ce10b478c97p-5,
    0x1.fffffffffffffp-6
  },
  { // Entry 261
    0x1.040ac0224fd931c17a1075750192f4d5p-5,
    0x1.0p-5
  },
  { // Entry 262
    0x1.040ac0224fd942437f70869cee2ff613p-5,
    0x1.0000000000001p-5
  },
  { // Entry 263
    0x1.0202ad5778e4568dd8d74f51f70f7d9cp-6,
    0x1.fffffffffffffp-7
  },
  { // Entry 264
    0x1.0202ad5778e45eae192cfa41139ad15bp-6,
    0x1.0p-6
  },
  { // Entry 265
    0x1.0202ad5778e46eee99d8501f4cb484f2p-6,
    0x1.0000000000001p-6
  },
  { // Entry 266
    0x1.0100ab00222d7e0921b6ae3791767825p-7,
    0x1.fffffffffffffp-8
  },
  { // Entry 267
    0x1.0100ab00222d861931c15e39b44e9937p-7,
    0x1.0p-7
  },
  { // Entry 268
    0x1.0100ab00222d963951d6be3dfa005e5fp-7,
    0x1.0000000000001p-7
  },
  { // Entry 269
    0x1.00802ab55777ca8226417cbfee2ff38bp-8,
    0x1.fffffffffffffp-9
  },
  { // Entry 270
    0x1.00802ab55777d28a2a42d26aa9ee67bcp-8,
    0x1.0p-8
  },
  { // Entry 271
    0x1.00802ab55777e29a32457dc0216c10dfp-8,
    0x1.0000000000001p-8
  },
  { // Entry 272
    0x1.00400aac00221cf682ab5035e9096355p-9,
    0x1.fffffffffffffp-10
  },
  { // Entry 273
    0x1.00400aac002224fa83ab7ae5e991e737p-9,
    0x1.0p-9
  },
  { // Entry 274
    0x1.00400aac0022350285abd045eaa34f2bp-9,
    0x1.0000000000001p-9
  },
  { // Entry 275
    0x1.002002aad5576f8c39739c649f2fc237p-10,
    0x1.fffffffffffffp-11
  },
  { // Entry 276
    0x1.002002aad557778e39b3a1ba49dea952p-10,
    0x1.0p-10
  },
  { // Entry 277
    0x1.002002aad55787923a33ac659f3ca792p-10,
    0x1.0000000000001p-10
  },
  { // Entry 278
    0x1.0004000aaabff821e24ea52ba86932b8p-13,
    0x1.fffffffffffffp-14
  },
  { // Entry 279
    0x1.0004000aaac00022224fa52e531931c1p-13,
    0x1.0p-13
  },
  { // Entry 280
    0x1.0004000aaac01022a251a533a87935d2p-13,
    0x1.0000000000001p-13
  },
  { // Entry 281
    -0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.0000000000001p9
  },
  { // Entry 282
    -0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.0p9
  },
  { // Entry 283
    -0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.fffffffffffffp8
  },
  { // Entry 284
    -0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.0000000000001p8
  },
  { // Entry 285
    -0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.0p8
  },
  { // Entry 286
    -0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.fffffffffffffp7
  },
  { // Entry 287
    -0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.0000000000001p7
  },
  { // Entry 288
    -0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.0p7
  },
  { // Entry 289
    -0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.fffffffffffffp6
  },
  { // Entry 290
    -0x1.ffffffffffffffffffffffe6962b8cdep-1,
    -0x1.0000000000001p6
  },
  { // Entry 291
    -0x1.ffffffffffffffffffffffe6962b8cdep-1,
    -0x1.0p6
  },
  { // Entry 292
    -0x1.ffffffffffffffffffffffe6962b8cdep-1,
    -0x1.fffffffffffffp5
  },
  { // Entry 293
    -0x1.fffffffffff8dee6c227a6f43aa81530p-1,
    -0x1.0000000000001p5
  },
  { // Entry 294
    -0x1.fffffffffff8dee6c227a6e5f875997fp-1,
    -0x1.0p5
  },
  { // Entry 295
    -0x1.fffffffffff8dee6c227a6ded75c5ba7p-1,
    -0x1.fffffffffffffp4
  },
  { // Entry 296
    -0x1.fffffc395488a22f4a6b5eb875ea5a66p-1,
    -0x1.0000000000001p4
  },
  { // Entry 297
    -0x1.fffffc395488a22f46a4b3411819a2eep-1,
    -0x1.0p4
  },
  { // Entry 298
    -0x1.fffffc395488a22f44c15d85693145c7p-1,
    -0x1.fffffffffffffp3
  },
  { // Entry 299
    -0x1.ffd407bdf7dfb0bc84275b4125a96eb7p-1,
    -0x1.0000000000001p3
  },
  { // Entry 300
    -0x1.ffd407bdf7dfb0a688065730fe0231c2p-1,
    -0x1.0p3
  },
  { // Entry 301
    -0x1.ffd407bdf7dfb09b89f5d528ea2a7402p-1,
    -0x1.fffffffffffffp2
  },
  { // Entry 302
    -0x1.f69f5523ef61881c365f838e3cece5d4p-1,
    -0x1.0000000000001p2
  },
  { // Entry 303
    -0x1.f69f5523ef6185c40ba87f669ea8ee15p-1,
    -0x1.0p2
  },
  { // Entry 304
    -0x1.f69f5523ef618497f64cfd52cf4eae35p-1,
    -0x1.fffffffffffffp1
  },
  { // Entry 305
    -0x1.bab5557101f8da29e776343c313b029ep-1,
    -0x1.0000000000001p1
  },
  { // Entry 306
    -0x1.bab5557101f8d1809224547b4bf5aa38p-1,
    -0x1.0p1
  },
  { // Entry 307
    -0x1.bab5557101f8cd2be77b649ad8eb0e05p-1,
    -0x1.fffffffffffffp0
  },
  { // Entry 308
    -0x1.43a54e4e98864d90355d87727adb37e7p-1,
    -0x1.0000000000001p0
  },
  { // Entry 309
    -0x1.43a54e4e988641ca8a4270fadf560de4p-1,
    -0x1.0p0
  },
  { // Entry 310
    -0x1.43a54e4e98863be7b4b4e5bf114cd6e0p-1,
    -0x1.fffffffffffffp-1
  },
  { // Entry 311
    0x1.9476504ba8399f5b97cae35beb78c3c5p738,
    0x1.fffffffffffffp8
  },
  { // Entry 312
    0x1.9476504ba852e6c09c8567c01c5a6648p738,
    0x1.0p9
  },
  { // Entry 313
    0x1.9476504ba885758aa5fa7545e10e8e46p738,
    0x1.0000000000001p9
  },
  { // Entry 314
    0x1.41c7a8814be192a5df25b042af824efdp369,
    0x1.fffffffffffffp7
  },
  { // Entry 315
    0x1.41c7a8814beba0e323300f777da65854p369,
    0x1.0p8
  },
  { // Entry 316
    0x1.41c7a8814bffbd5dab44ced26faccbfbp369,
    0x1.0000000000001p8
  },
  { // Entry 317
    0x1.95e54c5dd41b20600dd601a0ae672ff4p184,
    0x1.fffffffffffffp6
  },
  { // Entry 318
    0x1.95e54c5dd42177f53f4d5219df11ca3bp184,
    0x1.0p7
  },
  { // Entry 319
    0x1.95e54c5dd42e271fa23bf3585b655060p184,
    0x1.0000000000001p7
  },
  { // Entry 320
    0x1.425982cf597a4d52c89ea847bbaa807ap92,
    0x1.fffffffffffffp5
  },
  { // Entry 321
    0x1.425982cf597cd205ce3d5b3edb031756p92,
    0x1.0p6
  },
  { // Entry 322
    0x1.425982cf5981db6bd97ac13c35e666c6p92,
    0x1.0000000000001p6
  },
  { // Entry 323
    0x1.1f43fcc4b65da8944ac389b609e0f74ep46,
    0x1.fffffffffffffp4
  },
  { // Entry 324
    0x1.1f43fcc4b65ec7d84788401842174074p46,
    0x1.0p5
  },
  { // Entry 325
    0x1.1f43fcc4b66106604111ace0104fc90ep46,
    0x1.0000000000001p5
  },
  { // Entry 326
    0x1.0f2ebb0a80017cfac56c30874afbab98p23,
    0x1.fffffffffffffp3
  },
  { // Entry 327
    0x1.0f2ebb0a8002049223f170882b5ee5efp23,
    0x1.0p4
  },
  { // Entry 328
    0x1.0f2ebb0a800313c0e0fbf08ab7886866p23,
    0x1.0000000000001p4
  },
  { // Entry 329
    0x1.747ea7d470c681e43618ec18d53f1b21p11,
    0x1.fffffffffffffp2
  },
  { // Entry 330
    0x1.747ea7d470c6df0be00e084a815d1de6p11,
    0x1.0p3
  },
  { // Entry 331
    0x1.747ea7d470c7995b33f840ae1f76e2e7p11,
    0x1.0000000000001p3
  },
  { // Entry 332
    0x1.acc902e273a54fdfb6777166e6760dfbp5,
    0x1.fffffffffffffp1
  },
  { // Entry 333
    0x1.acc902e273a58678d6d3bfdb93db96d0p5,
    0x1.0p2
  },
  { // Entry 334
    0x1.acc902e273a5f3ab178c5cc50320149cp5,
    0x1.0000000000001p2
  },
  { // Entry 335
    0x1.98e64b8d4ddabf34d582cd2909aafb2ap2,
    0x1.fffffffffffffp0
  },
  { // Entry 336
    0x1.98e64b8d4ddadcc33a3ba206b68abba8p2,
    0x1.0p1
  },
  { // Entry 337
    0x1.98e64b8d4ddb17e003ad4bc215d4ef86p2,
    0x1.0000000000001p1
  },
  { // Entry 338
    0x1.b7e151628aed14abb4e6442933f899f6p0,
    0x1.fffffffffffffp-1
  },
  { // Entry 339
    0x1.b7e151628aed2a6abf7158809cf4f3c7p0,
    0x1.0p0
  },
  { // Entry 340
    0x1.b7e151628aed55e8d487812f70f79067p0,
    0x1.0000000000001p0
  },
  { // Entry 341
    0x1.ffc045692fc9dbc7b7e032576e5e26f8p1023,
    0x1.62e41ffffffffp9
  },
  { // Entry 342
    0x1.ffc045693009d3d065062f9267dff55ep1023,
    0x1.62e42p9
  },
  { // Entry 343
    0x1.ffc045693049cbd9122c34cc62776884p1023,
    0x1.62e4200000001p9
  },
  { // Entry 344
    -0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.72e4200000001p9
  },
  { // Entry 345
    -0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.72e42p9
  },
  { // Entry 346
    -0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.72e41ffffffffp9
  },
  { // Entry 347
    HUGE_VAL,
    0x1.0p1020
  },
  { // Entry 348
    HUGE_VAL,
    0x1.999999999999ap1020
  },
  { // Entry 349
    HUGE_VAL,
    0x1.199999999999ap1021
  },
  { // Entry 350
    HUGE_VAL,
    0x1.6666666666667p1021
  },
  { // Entry 351
    HUGE_VAL,
    0x1.b333333333334p1021
  },
  { // Entry 352
    HUGE_VAL,
    0x1.0p1022
  },
  { // Entry 353
    HUGE_VAL,
    0x1.fffffffffffffp1023
  },
  { // Entry 354
    HUGE_VAL,
    HUGE_VAL
  },
  { // Entry 355
    -0x1.p0,
    -HUGE_VAL
  },
  { // Entry 356
    HUGE_VAL,
    0x1.fffffffffffffp1023
  },
  { // Entry 357
    -0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.fffffffffffffp1023
  },
  { // Entry 358
    HUGE_VAL,
    0x1.ffffffffffffep1023
  },
  { // Entry 359
    -0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.ffffffffffffep1023
  },
  { // Entry 360
    0x1.624046eb09338d2991a30893e7f4108dp4,
    0x1.921fb54442d18p1
  },
  { // Entry 361
    -0x1.e9dfdd84a671066b619f1bb23ba2eb2fp-1,
    -0x1.921fb54442d18p1
  },
  { // Entry 362
    0x1.e7bdb90ab26bdf555eaf19da7f043f2cp1,
    0x1.921fb54442d18p0
  },
  { // Entry 363
    -0x1.9590cee42260813cac44f53b3217ed19p-1,
    -0x1.921fb54442d18p0
  },
  { // Entry 364
    0x1.b7e151628aed55e8d487812f70f79067p0,
    0x1.0000000000001p0
  },
  { // Entry 365
    -0x1.43a54e4e98864d90355d87727adb37e7p-1,
    -0x1.0000000000001p0
  },
  { // Entry 366
    0x1.b7e151628aed2a6abf7158809cf4f3c7p0,
    0x1.0p0
  },
  { // Entry 367
    -0x1.43a54e4e988641ca8a4270fadf560de4p-1,
    -0x1.0p0
  },
  { // Entry 368
    0x1.b7e151628aed14abb4e6442933f899f6p0,
    0x1.fffffffffffffp-1
  },
  { // Entry 369
    -0x1.43a54e4e98863be7b4b4e5bf114cd6e0p-1,
    -0x1.fffffffffffffp-1
  },
  { // Entry 370
    0x1.317acd28e3954ab0b8e398654f25590ap0,
    0x1.921fb54442d18p-1
  },
  { // Entry 371
    -0x1.168f47187dbc360f4ac035fc8ff9e913p-1,
    -0x1.921fb54442d18p-1
  },
  { // Entry 372
    0x1.00000000000010p-1022,
    0x1.0000000000001p-1022
  },
  { // Entry 373
    -0x1.0000000000000fffffffffffffffffffp-1022,
    -0x1.0000000000001p-1022
  },
  { // Entry 374
    0x1.p-1022,
    0x1.0p-1022
  },
  { // Entry 375
    -0x1.ffffffffffffffffffffffffffffffffp-1023,
    -0x1.0p-1022
  },
  { // Entry 376
    0x1.ffffffffffffe0p-1023,
    0x1.ffffffffffffep-1023
  },
  { // Entry 377
    -0x1.ffffffffffffdfffffffffffffffffffp-1023,
    -0x1.ffffffffffffep-1023
  },
  { // Entry 378
    0x1.ffffffffffffc0p-1023,
    0x1.ffffffffffffcp-1023
  },
  { // Entry 379
    -0x1.ffffffffffffbfffffffffffffffffffp-1023,
    -0x1.ffffffffffffcp-1023
  },
  { // Entry 380
    0x1.p-1073,
    0x1.0p-1073
  },
  { // Entry 381
    -0x1.ffffffffffffffffffffffffffffffffp-1074,
    -0x1.0p-1073
  },
  { // Entry 382
    0x1.p-1074,
    0x1.0p-1074
  },
  { // Entry 383
    -0.0,
    -0x1.0p-1074
  },
  { // Entry 384
    0.0,
    0.0
  },
  { // Entry 385
    -0.0,
    -0.0
  },
  { // Entry 386
    0x1.fffffffffff2a1b0e263400d15fc52ffp1023,
    0x1.62e42fefa39efp9
  },
  { // Entry 387
    HUGE_VAL,
    0x1.62e42fefa39f0p9
  }
};
