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

static data_1_1_t<float, float> g_asinhf_intel_data[] = {
  { // Entry 0
    -0x1.0f1feafffe3d1154765de9942446abdfp-2,
    -0x1.124db8p-2
  },
  { // Entry 1
    0x1.0f1feafffe3d1154765de9942446abdfp-2,
    0x1.124db8p-2
  },
  { // Entry 2
    -0x1.250abb00034e7ff129bd35187e5a90a8p-11,
    -0x1.250abcp-11
  },
  { // Entry 3
    0x1.250abb00034e7ff129bd35187e5a90a8p-11,
    0x1.250abcp-11
  },
  { // Entry 4
    -0x1.544e70ffffe14ccbadab97b65580f0f7p-5,
    -0x1.546780p-5
  },
  { // Entry 5
    0x1.544e70ffffe14ccbadab97b65580f0f7p-5,
    0x1.546780p-5
  },
  { // Entry 6
    -0x1.e4713d0abf552bdbc167e2422c33c9cbp1,
    -0x1.60p4
  },
  { // Entry 7
    0x1.e4713d0abf552bdbc167e2422c33c9cbp1,
    0x1.60p4
  },
  { // Entry 8
    -0x1.5fe9a00168f4b6cc1a76f27bb6acd88bp-4,
    -0x1.605880p-4
  },
  { // Entry 9
    0x1.5fe9a00168f4b6cc1a76f27bb6acd88bp-4,
    0x1.605880p-4
  },
  { // Entry 10
    -0x1.dade546facd5dc06721c3a76f2fa6a4cp0,
    -0x1.8f096ep1
  },
  { // Entry 11
    0x1.dade546facd5dc06721c3a76f2fa6a4cp0,
    0x1.8f096ep1
  },
  { // Entry 12
    -0x1.d4982b0c930dad1b48293ef31d693cecp-6,
    -0x1.d4a886p-6
  },
  { // Entry 13
    0x1.d4982b0c930dad1b48293ef31d693cecp-6,
    0x1.d4a886p-6
  },
  { // Entry 14
    -0x1.d9ecbcfff8be129cc3cf7bf216da5f46p-4,
    -0x1.dafba0p-4
  },
  { // Entry 15
    0x1.d9ecbcfff8be129cc3cf7bf216da5f46p-4,
    0x1.dafba0p-4
  },
  { // Entry 16
    -0x1.f95ae3069ee6ea46106060615b04d1b9p-3,
    -0x1.fe7fc0p-3
  },
  { // Entry 17
    0x1.f95ae3069ee6ea46106060615b04d1b9p-3,
    0x1.fe7fc0p-3
  },
  { // Entry 18
    0x1.ffeaad10b5b2e591fdad6bd7f7a497c3p-6,
    0x1.p-5
  },
  { // Entry 19
    -0x1.ffeaad10b5b2e591fdad6bd7f7a497c3p-6,
    -0x1.p-5
  },
  { // Entry 20
    0x1.ffffffffffffffffffffffffffffffffp-144,
    0x1.p-143
  },
  { // Entry 21
    -0x1.ffffffffffffffffffffffffffffffffp-144,
    -0x1.p-143
  },
  { // Entry 22
    0x1.ffeab11035cadf93754fd9171a996c98p-6,
    0x1.000002p-5
  },
  { // Entry 23
    -0x1.ffeab11035cadf93754fd9171a996c98p-6,
    -0x1.000002p-5
  },
  { // Entry 24
    0x1.000001fffffffffffffff55555155554p-41,
    0x1.000002p-41
  },
  { // Entry 25
    -0x1.000001fffffffffffffff55555155554p-41,
    -0x1.000002p-41
  },
  { // Entry 26
    0x1.000001ffffffffffffffffffffffffffp-126,
    0x1.000002p-126
  },
  { // Entry 27
    -0x1.000001ffffffffffffffffffffffffffp-126,
    -0x1.000002p-126
  },
  { // Entry 28
    0x1.7912730e9dd8c28d0e2e8849730f0345p4,
    0x1.000002p33
  },
  { // Entry 29
    -0x1.7912730e9dd8c28d0e2e8849730f0345p4,
    -0x1.000002p33
  },
  { // Entry 30
    0x1.fead3b0ab2db53675e78fa2d41047d4ap-4,
    0x1.000018p-3
  },
  { // Entry 31
    -0x1.fead3b0ab2db53675e78fa2d41047d4ap-4,
    -0x1.000018p-3
  },
  { // Entry 32
    0x1.fead42fae23a4ec3ec74e75d7e6a8e01p-4,
    0x1.00001cp-3
  },
  { // Entry 33
    -0x1.fead42fae23a4ec3ec74e75d7e6a8e01p-4,
    -0x1.00001cp-3
  },
  { // Entry 34
    0x1.686fc30f61d32f36cebd3556647e6d85p5,
    0x1.00004cp64
  },
  { // Entry 35
    -0x1.686fc30f61d32f36cebd3556647e6d85p5,
    -0x1.00004cp64
  },
  { // Entry 36
    0x1.0c1fb8ff9524366b4770e679f3d2be09p1,
    0x1.000060p2
  },
  { // Entry 37
    -0x1.0c1fb8ff9524366b4770e679f3d2be09p1,
    -0x1.000060p2
  },
  { // Entry 38
    0x1.ecc43f011e3008670443b4fad065f492p-2,
    0x1.0000d0p-1
  },
  { // Entry 39
    -0x1.ecc43f011e3008670443b4fad065f492p-2,
    -0x1.0000d0p-1
  },
  { // Entry 40
    0x1.feafbe09a9ba162c9d72e1d6bf7564a3p-4,
    0x1.00015cp-3
  },
  { // Entry 41
    -0x1.feafbe09a9ba162c9d72e1d6bf7564a3p-4,
    -0x1.00015cp-3
  },
  { // Entry 42
    0x1.c3458525ab38fbabe76fc767cf7a5730p-1,
    0x1.000180p0
  },
  { // Entry 43
    -0x1.c3458525ab38fbabe76fc767cf7a5730p-1,
    -0x1.000180p0
  },
  { // Entry 44
    0x1.fad2e50655314fc4b2c8f27b0fc674ddp-3,
    0x1.0001a6p-2
  },
  { // Entry 45
    -0x1.fad2e50655314fc4b2c8f27b0fc674ddp-3,
    -0x1.0001a6p-2
  },
  { // Entry 46
    0x1.fff8d74b7e204f81827216900ed8543fp-6,
    0x1.000716p-5
  },
  { // Entry 47
    -0x1.fff8d74b7e204f81827216900ed8543fp-6,
    -0x1.000716p-5
  },
  { // Entry 48
    0x1.ffbac9000089648597139384da627d36p-5,
    0x1.0008p-4
  },
  { // Entry 49
    -0x1.ffbac9000089648597139384da627d36p-5,
    -0x1.0008p-4
  },
  { // Entry 50
    0x1.fffb13040741e3467cc63b91d9b9d8c7p-6,
    0x1.000834p-5
  },
  { // Entry 51
    -0x1.fffb13040741e3467cc63b91d9b9d8c7p-6,
    -0x1.000834p-5
  },
  { // Entry 52
    0x1.fec8d40c701c746cf9bdcc066cc0c5bbp-4,
    0x1.000ep-3
  },
  { // Entry 53
    -0x1.fec8d40c701c746cf9bdcc066cc0c5bbp-4,
    -0x1.000ep-3
  },
  { // Entry 54
    0x1.0c3ab19e45eb29d7ac3cb690b07f2c76p1,
    0x1.0038p2
  },
  { // Entry 55
    -0x1.0c3ab19e45eb29d7ac3cb690b07f2c76p1,
    -0x1.0038p2
  },
  { // Entry 56
    0x1.c3b5a37f910bfbf8ba8ffc861755f387p-1,
    0x1.0050cep0
  },
  { // Entry 57
    -0x1.c3b5a37f910bfbf8ba8ffc861755f387p-1,
    -0x1.0050cep0
  },
  { // Entry 58
    0x1.ff9b301b22a673c34515e6fb85810f60p-4,
    0x1.0078p-3
  },
  { // Entry 59
    -0x1.ff9b301b22a673c34515e6fb85810f60p-4,
    -0x1.0078p-3
  },
  { // Entry 60
    0x1.c4bc4cfeab01d553217d775dafbe1e54p-1,
    0x1.010ac8p0
  },
  { // Entry 61
    -0x1.c4bc4cfeab01d553217d775dafbe1e54p-1,
    -0x1.010ac8p0
  },
  { // Entry 62
    0x1.fe5b8ef85ffc7ee05a4f3f96b6e60554p-3,
    0x1.01d4p-2
  },
  { // Entry 63
    -0x1.fe5b8ef85ffc7ee05a4f3f96b6e60554p-3,
    -0x1.01d4p-2
  },
  { // Entry 64
    0x1.795233437bda10a17c9819cb13d288e3p3,
    0x1.02p16
  },
  { // Entry 65
    -0x1.795233437bda10a17c9819cb13d288e3p3,
    -0x1.02p16
  },
  { // Entry 66
    0x1.7952350002c01bfe0af7d7457dbf20dap3,
    0x1.02000ep16
  },
  { // Entry 67
    -0x1.7952350002c01bfe0af7d7457dbf20dap3,
    -0x1.02000ep16
  },
  { // Entry 68
    0x1.f1abad0010bc92fb3c926eb5ded61431p-2,
    0x1.02bf60p-1
  },
  { // Entry 69
    -0x1.f1abad0010bc92fb3c926eb5ded61431p-2,
    -0x1.02bf60p-1
  },
  { // Entry 70
    0x1.ca35e4554c95b4f73f96234be8db5a0cp-1,
    0x1.04efa8p0
  },
  { // Entry 71
    -0x1.ca35e4554c95b4f73f96234be8db5a0cp-1,
    -0x1.04efa8p0
  },
  { // Entry 72
    0x1.f62556ffff16c63d4ba67855548e7a1ap-2,
    0x1.0541d6p-1
  },
  { // Entry 73
    -0x1.f62556ffff16c63d4ba67855548e7a1ap-2,
    -0x1.0541d6p-1
  },
  { // Entry 74
    0x1.cc0aec88d32a2ccff738ff8f5e116de3p-1,
    0x1.063ef4p0
  },
  { // Entry 75
    -0x1.cc0aec88d32a2ccff738ff8f5e116de3p-1,
    -0x1.063ef4p0
  },
  { // Entry 76
    0x1.06ac7a01a2b93dd343117d62da00efe9p-5,
    0x1.06b8p-5
  },
  { // Entry 77
    -0x1.06ac7a01a2b93dd343117d62da00efe9p-5,
    -0x1.06b8p-5
  },
  { // Entry 78
    0x1.78a50f013838dd4ba303180ecb0af32fp0,
    0x1.0802p1
  },
  { // Entry 79
    -0x1.78a50f013838dd4ba303180ecb0af32fp0,
    -0x1.0802p1
  },
  { // Entry 80
    0x1.104e01af3396534a594bd72c365a3eedp1,
    0x1.08c230p2
  },
  { // Entry 81
    -0x1.104e01af3396534a594bd72c365a3eedp1,
    -0x1.08c230p2
  },
  { // Entry 82
    0x1.d0517f0001a7d9a1e6f44c6bab1469eep-1,
    0x1.0950c8p0
  },
  { // Entry 83
    -0x1.d0517f0001a7d9a1e6f44c6bab1469eep-1,
    -0x1.0950c8p0
  },
  { // Entry 84
    0x1.fe0a030fb46d45fb1c02b3272bcd1914p-2,
    0x1.09b21ap-1
  },
  { // Entry 85
    -0x1.fe0a030fb46d45fb1c02b3272bcd1914p-2,
    -0x1.09b21ap-1
  },
  { // Entry 86
    0x1.fea6bc4743366aeba868336ffac3acbdp-2,
    0x1.0a0a66p-1
  },
  { // Entry 87
    -0x1.fea6bc4743366aeba868336ffac3acbdp-2,
    -0x1.0a0a66p-1
  },
  { // Entry 88
    0x1.d822f300019612b0ea114300b20b6d9dp-1,
    0x1.0ef9fap0
  },
  { // Entry 89
    -0x1.d822f300019612b0ea114300b20b6d9dp-1,
    -0x1.0ef9fap0
  },
  { // Entry 90
    0x1.d98a7896e162415f9165e41af5e889cbp-1,
    0x1.10p0
  },
  { // Entry 91
    -0x1.d98a7896e162415f9165e41af5e889cbp-1,
    -0x1.10p0
  },
  { // Entry 92
    0x1.1034b2000a6d7c1400fd184bdac732ffp-4,
    0x1.1068p-4
  },
  { // Entry 93
    -0x1.1034b2000a6d7c1400fd184bdac732ffp-4,
    -0x1.1068p-4
  },
  { // Entry 94
    0x1.de5c70fffea7cdc7698c00ca7b57f914p-1,
    0x1.138754p0
  },
  { // Entry 95
    -0x1.de5c70fffea7cdc7698c00ca7b57f914p-1,
    -0x1.138754p0
  },
  { // Entry 96
    0x1.16a7b0fce815d17a58d940605c85f9cfp-3,
    0x1.1784p-3
  },
  { // Entry 97
    -0x1.16a7b0fce815d17a58d940605c85f9cfp-3,
    -0x1.1784p-3
  },
  { // Entry 98
    0x1.e46bf608630f17f3183a7d1db5529144p-1,
    0x1.18p0
  },
  { // Entry 99
    -0x1.e46bf608630f17f3183a7d1db5529144p-1,
    -0x1.18p0
  },
  { // Entry 100
    0x1.b6c931c025238ebcf98ef12eb28d8307p5,
    0x1.18p78
  },
  { // Entry 101
    -0x1.b6c931c025238ebcf98ef12eb28d8307p5,
    -0x1.18p78
  },
  { // Entry 102
    0x1.7bf48d0006896bad6b7e5e69afbdc70bp3,
    0x1.1823p16
  },
  { // Entry 103
    -0x1.7bf48d0006896bad6b7e5e69afbdc70bp3,
    -0x1.1823p16
  },
  { // Entry 104
    0x1.e681682e3230779582a57284cdf10552p-1,
    0x1.198be0p0
  },
  { // Entry 105
    -0x1.e681682e3230779582a57284cdf10552p-1,
    -0x1.198be0p0
  },
  { // Entry 106
    0x1.e7ff47ef1be499dbdb2d4c2dab7144d2p-1,
    0x1.1aa8p0
  },
  { // Entry 107
    -0x1.e7ff47ef1be499dbdb2d4c2dab7144d2p-1,
    -0x1.1aa8p0
  },
  { // Entry 108
    0x1.19ffa8fffcaa5a70836fe5869d6f3e64p-3,
    0x1.1ae4p-3
  },
  { // Entry 109
    -0x1.19ffa8fffcaa5a70836fe5869d6f3e64p-3,
    -0x1.1ae4p-3
  },
  { // Entry 110
    0x1.e8d45f38a22bc64723c44174227b6055p-1,
    0x1.1b46d0p0
  },
  { // Entry 111
    -0x1.e8d45f38a22bc64723c44174227b6055p-1,
    -0x1.1b46d0p0
  },
  { // Entry 112
    0x1.e8db53fe01cb2f1941b1e02656828b71p-1,
    0x1.1b4cp0
  },
  { // Entry 113
    -0x1.e8db53fe01cb2f1941b1e02656828b71p-1,
    -0x1.1b4cp0
  },
  { // Entry 114
    0x1.e93d1fffffc916cac5df6685ccfb9a95p-1,
    0x1.1b94f4p0
  },
  { // Entry 115
    -0x1.e93d1fffffc916cac5df6685ccfb9a95p-1,
    -0x1.1b94f4p0
  },
  { // Entry 116
    0x1.e9cc87321d1ed3ec8130e9128585214cp-1,
    0x1.1cp0
  },
  { // Entry 117
    -0x1.e9cc87321d1ed3ec8130e9128585214cp-1,
    -0x1.1cp0
  },
  { // Entry 118
    0x1.18760b00045eb313cf2650593887e2bep6,
    0x1.1d0740p100
  },
  { // Entry 119
    -0x1.18760b00045eb313cf2650593887e2bep6,
    -0x1.1d0740p100
  },
  { // Entry 120
    0x1.ed98b1f64a808793ae088a13d43da9c2p-1,
    0x1.1ed8p0
  },
  { // Entry 121
    -0x1.ed98b1f64a808793ae088a13d43da9c2p-1,
    -0x1.1ed8p0
  },
  { // Entry 122
    0x1.24f53378690fc7ab8aafbeb62b1c3badp-5,
    0x1.250530p-5
  },
  { // Entry 123
    -0x1.24f53378690fc7ab8aafbeb62b1c3badp-5,
    -0x1.250530p-5
  },
  { // Entry 124
    0x1.4a5b157658bb51fe2cd170c897ff9227p-5,
    0x1.4a7202p-5
  },
  { // Entry 125
    -0x1.4a5b157658bb51fe2cd170c897ff9227p-5,
    -0x1.4a7202p-5
  },
  { // Entry 126
    0x1.b736e800018136bc46678d296cd029bdp0,
    0x1.585c20p1
  },
  { // Entry 127
    -0x1.b736e800018136bc46678d296cd029bdp0,
    -0x1.585c20p1
  },
  { // Entry 128
    0x1.220b35fffea45ca3a7091d0d1dd0328dp0,
    0x1.6433f4p0
  },
  { // Entry 129
    -0x1.220b35fffea45ca3a7091d0d1dd0328dp0,
    -0x1.6433f4p0
  },
  { // Entry 130
    0x1.6b88e10b317c9c2d54079667c212fd16p-6,
    0x1.6b9084p-6
  },
  { // Entry 131
    -0x1.6b88e10b317c9c2d54079667c212fd16p-6,
    -0x1.6b9084p-6
  },
  { // Entry 132
    0x1.c570240000129c3304aa0b9915097b93p0,
    0x1.6d505ep1
  },
  { // Entry 133
    -0x1.c570240000129c3304aa0b9915097b93p0,
    -0x1.6d505ep1
  },
  { // Entry 134
    0x1.71f3de02331239b851f896c8c251b36fp-4,
    0x1.7274b0p-4
  },
  { // Entry 135
    -0x1.71f3de02331239b851f896c8c251b36fp-4,
    -0x1.7274b0p-4
  },
  { // Entry 136
    0x1.7fffff000001ccccc883a8462e52c2d1p-10,
    0x1.800008p-10
  },
  { // Entry 137
    -0x1.7fffff000001ccccc883a8462e52c2d1p-10,
    -0x1.800008p-10
  },
  { // Entry 138
    0x1.d2c365a2367e0a0f342a944b8fe912adp0,
    0x1.81f778p1
  },
  { // Entry 139
    -0x1.d2c365a2367e0a0f342a944b8fe912adp0,
    -0x1.81f778p1
  },
  { // Entry 140
    0x1.7042f9000343b85d0080af58e6ce2a5bp3,
    0x1.84c61ep15
  },
  { // Entry 141
    -0x1.7042f9000343b85d0080af58e6ce2a5bp3,
    -0x1.84c61ep15
  },
  { // Entry 142
    0x1.ca328b0b0732378d71b2ed8f94926b5cp5,
    0x1.8c25e4p81
  },
  { // Entry 143
    -0x1.ca328b0b0732378d71b2ed8f94926b5cp5,
    -0x1.8c25e4p81
  },
  { // Entry 144
    0x1.6c02870f43f412f2facda9c71af64d9ap5,
    0x1.9026f4p64
  },
  { // Entry 145
    -0x1.6c02870f43f412f2facda9c71af64d9ap5,
    -0x1.9026f4p64
  },
  { // Entry 146
    0x1.dc29a21d978c49d5ef40e243da93e547p0,
    0x1.912912p1
  },
  { // Entry 147
    -0x1.dc29a21d978c49d5ef40e243da93e547p0,
    -0x1.912912p1
  },
  { // Entry 148
    0x1.e87da9ce17176d9508dd14c470c83b98p0,
    0x1.a5e970p1
  },
  { // Entry 149
    -0x1.e87da9ce17176d9508dd14c470c83b98p0,
    -0x1.a5e970p1
  },
  { // Entry 150
    0x1.52728c00e52ef07d6e0a17848d326ee2p1,
    0x1.bffffep2
  },
  { // Entry 151
    -0x1.52728c00e52ef07d6e0a17848d326ee2p1,
    -0x1.bffffep2
  },
  { // Entry 152
    0x1.5512b2ffffffdc9663a38b72b6dcdf6fp0,
    0x1.c3523ep0
  },
  { // Entry 153
    -0x1.5512b2ffffffdc9663a38b72b6dcdf6fp0,
    -0x1.c3523ep0
  },
  { // Entry 154
    0x1.c52b7d0f27e70062b6eeb63fbfbccb96p-6,
    0x1.c53a48p-6
  },
  { // Entry 155
    -0x1.c52b7d0f27e70062b6eeb63fbfbccb96p-6,
    -0x1.c53a48p-6
  },
  { // Entry 156
    0x1.a89b32fff40d30fcb4988c656e03af82p-1,
    0x1.dafa74p-1
  },
  { // Entry 157
    -0x1.a89b32fff40d30fcb4988c656e03af82p-1,
    -0x1.dafa74p-1
  },
  { // Entry 158
    0x1.6274b2fffe689d7abb667b3f8ab66e94p0,
    0x1.df1344p0
  },
  { // Entry 159
    -0x1.6274b2fffe689d7abb667b3f8ab66e94p0,
    -0x1.df1344p0
  },
  { // Entry 160
    0x1.e9d89afff66699a3a0a0107eec7292b8p-4,
    0x1.eb03bcp-4
  },
  { // Entry 161
    -0x1.e9d89afff66699a3a0a0107eec7292b8p-4,
    -0x1.eb03bcp-4
  },
  { // Entry 162
    0x1.baba624cf203c38bfc585ab19463e81bp-1,
    0x1.f3fffep-1
  },
  { // Entry 163
    -0x1.baba624cf203c38bfc585ab19463e81bp-1,
    -0x1.f3fffep-1
  },
  { // Entry 164
    0x1.bcc66ead9bdc7bae10b705739ce6273ap-1,
    0x1.f6dd80p-1
  },
  { // Entry 165
    -0x1.bcc66ead9bdc7bae10b705739ce6273ap-1,
    -0x1.f6dd80p-1
  },
  { // Entry 166
    0x1.dca21f00608c1d5dfa8c6eb1ce2f725cp4,
    0x1.f7fffep41
  },
  { // Entry 167
    -0x1.dca21f00608c1d5dfa8c6eb1ce2f725cp4,
    -0x1.f7fffep41
  },
  { // Entry 168
    0x1.0a2f1d0000074b9fe1702b2e3f079a3bp1,
    0x1.f81024p1
  },
  { // Entry 169
    -0x1.0a2f1d0000074b9fe1702b2e3f079a3bp1,
    -0x1.f81024p1
  },
  { // Entry 170
    0x1.f8b8c2d940b76e1cca1ebf54f4195d20p-4,
    0x1.f9fffep-4
  },
  { // Entry 171
    -0x1.f8b8c2d940b76e1cca1ebf54f4195d20p-4,
    -0x1.f9fffep-4
  },
  { // Entry 172
    0x1.fa6d06fff6691e956f3207b0a5ca4eeap-4,
    0x1.fbb796p-4
  },
  { // Entry 173
    -0x1.fa6d06fff6691e956f3207b0a5ca4eeap-4,
    -0x1.fbb796p-4
  },
  { // Entry 174
    0x1.fd26a51202d84047f50c18a4c4235d86p-6,
    0x1.fd3ba0p-6
  },
  { // Entry 175
    -0x1.fd26a51202d84047f50c18a4c4235d86p-6,
    -0x1.fd3ba0p-6
  },
  { // Entry 176
    0x1.eaf8a9005792f097e006d070ec6e7538p-2,
    0x1.fdfffep-2
  },
  { // Entry 177
    -0x1.eaf8a9005792f097e006d070ec6e7538p-2,
    -0x1.fdfffep-2
  },
  { // Entry 178
    0x1.ff322f1260d5d5168ca6d76975246012p-7,
    0x1.ff377ep-7
  },
  { // Entry 179
    -0x1.ff322f1260d5d5168ca6d76975246012p-7,
    -0x1.ff377ep-7
  },
  { // Entry 180
    0x1.b6102affc7f74638c6d979799db2bfaap5,
    0x1.ff9ffep77
  },
  { // Entry 181
    -0x1.b6102affc7f74638c6d979799db2bfaap5,
    -0x1.ff9ffep77
  },
  { // Entry 182
    0x1.ffb29911437bb58cb151e304b1fad9a8p-7,
    0x1.ffb7ecp-7
  },
  { // Entry 183
    -0x1.ffb29911437bb58cb151e304b1fad9a8p-7,
    -0x1.ffb7ecp-7
  },
  { // Entry 184
    0x1.fe8d48ac193eaa17e2bb8b9342791f2dp-4,
    0x1.ffdffep-4
  },
  { // Entry 185
    -0x1.fe8d48ac193eaa17e2bb8b9342791f2dp-4,
    -0x1.ffdffep-4
  },
  { // Entry 186
    0x1.ffdfa8bad104e0551f02b0530cfad4f8p-9,
    0x1.ffdffep-9
  },
  { // Entry 187
    -0x1.ffdfa8bad104e0551f02b0530cfad4f8p-9,
    -0x1.ffdffep-9
  },
  { // Entry 188
    0x1.fea49a3aebbc35e17238eea29547caf8p-4,
    0x1.fff77ep-4
  },
  { // Entry 189
    -0x1.fea49a3aebbc35e17238eea29547caf8p-4,
    -0x1.fff77ep-4
  },
  { // Entry 190
    0x1.facf9900599e9e6aa0b023811476fee8p-3,
    0x1.ffffe6p-3
  },
  { // Entry 191
    -0x1.facf9900599e9e6aa0b023811476fee8p-3,
    -0x1.ffffe6p-3
  },
  { // Entry 192
    0x1.ffaac50040a9c0881e560e4889792087p-5,
    0x1.fffff4p-5
  },
  { // Entry 193
    -0x1.ffaac50040a9c0881e560e4889792087p-5,
    -0x1.fffff4p-5
  },
  { // Entry 194
    0x1.ffaac8fe42289581485ee110507837f6p-5,
    0x1.fffff8p-5
  },
  { // Entry 195
    -0x1.ffaac8fe42289581485ee110507837f6p-5,
    -0x1.fffff8p-5
  },
  { // Entry 196
    0x1.c9d926ffffec577f549c952aff8ea67ep-1,
    0x1.04ad76p0
  },
  { // Entry 197
    -0x1.c9d926ffffec577f549c952aff8ea67ep-1,
    -0x1.04ad76p0
  },
  { // Entry 198
    -0x1.c34366179d426cc1b1f33d1ba4a314a7p-1,
    -0x1.p0
  },
  { // Entry 199
    0x1.c34366179d426cc1b1f33d1ba4a314a7p-1,
    0x1.p0
  },
  { // Entry 200
    -0x1.9490fec3efbe5988497c1478565aa446p-1,
    -0x1.c0p-1
  },
  { // Entry 201
    0x1.9490fec3efbe5988497c1478565aa446p-1,
    0x1.c0p-1
  },
  { // Entry 202
    -0x1.62e42fefa39ef35793c7673007e5ed5ep-1,
    -0x1.80p-1
  },
  { // Entry 203
    0x1.62e42fefa39ef35793c7673007e5ed5ep-1,
    0x1.80p-1
  },
  { // Entry 204
    -0x1.2e27502cbfb3347f6bcbf84eb95ca4a3p-1,
    -0x1.40p-1
  },
  { // Entry 205
    0x1.2e27502cbfb3347f6bcbf84eb95ca4a3p-1,
    0x1.40p-1
  },
  { // Entry 206
    -0x1.ecc2caec5160994be04204a968c7020dp-2,
    -0x1.p-1
  },
  { // Entry 207
    0x1.ecc2caec5160994be04204a968c7020dp-2,
    0x1.p-1
  },
  { // Entry 208
    -0x1.7786a7973a5923b96eef2610c25e841ap-2,
    -0x1.80p-2
  },
  { // Entry 209
    0x1.7786a7973a5923b96eef2610c25e841ap-2,
    0x1.80p-2
  },
  { // Entry 210
    -0x1.facfb2399e636e8d67115de540a778acp-3,
    -0x1.p-2
  },
  { // Entry 211
    0x1.facfb2399e636e8d67115de540a778acp-3,
    0x1.p-2
  },
  { // Entry 212
    -0x1.fead0b6996972d06058e3eb12c938140p-4,
    -0x1.p-3
  },
  { // Entry 213
    0x1.fead0b6996972d06058e3eb12c938140p-4,
    0x1.p-3
  },
  { // Entry 214
    0.0,
    0.0
  },
  { // Entry 215
    0x1.fead0b6996972d06058e3eb12c938140p-4,
    0x1.p-3
  },
  { // Entry 216
    -0x1.fead0b6996972d06058e3eb12c938140p-4,
    -0x1.p-3
  },
  { // Entry 217
    0x1.facfb2399e636e8d67115de540a778acp-3,
    0x1.p-2
  },
  { // Entry 218
    -0x1.facfb2399e636e8d67115de540a778acp-3,
    -0x1.p-2
  },
  { // Entry 219
    0x1.7786a7973a5923b96eef2610c25e841ap-2,
    0x1.80p-2
  },
  { // Entry 220
    -0x1.7786a7973a5923b96eef2610c25e841ap-2,
    -0x1.80p-2
  },
  { // Entry 221
    0x1.ecc2caec5160994be04204a968c7020dp-2,
    0x1.p-1
  },
  { // Entry 222
    -0x1.ecc2caec5160994be04204a968c7020dp-2,
    -0x1.p-1
  },
  { // Entry 223
    0x1.2e27502cbfb3347f6bcbf84eb95ca4a3p-1,
    0x1.40p-1
  },
  { // Entry 224
    -0x1.2e27502cbfb3347f6bcbf84eb95ca4a3p-1,
    -0x1.40p-1
  },
  { // Entry 225
    0x1.62e42fefa39ef35793c7673007e5ed5ep-1,
    0x1.80p-1
  },
  { // Entry 226
    -0x1.62e42fefa39ef35793c7673007e5ed5ep-1,
    -0x1.80p-1
  },
  { // Entry 227
    0x1.9490fec3efbe5988497c1478565aa446p-1,
    0x1.c0p-1
  },
  { // Entry 228
    -0x1.9490fec3efbe5988497c1478565aa446p-1,
    -0x1.c0p-1
  },
  { // Entry 229
    0x1.c34366179d426cc1b1f33d1ba4a314a7p-1,
    0x1.p0
  },
  { // Entry 230
    -0x1.c34366179d426cc1b1f33d1ba4a314a7p-1,
    -0x1.p0
  },
  { // Entry 231
    0x1.18080dd3171b6c031a9b576be63b6d4cp6,
    0x1.p100
  },
  { // Entry 232
    -0x1.18080dd3171b6c031a9b576be63b6d4cp6,
    -0x1.p100
  },
  { // Entry 233
    0x1.1869a6d270699e1fa7c307d5fdbce864p6,
    0x1.19999ap100
  },
  { // Entry 234
    -0x1.1869a6d270699e1fa7c307d5fdbce864p6,
    -0x1.19999ap100
  },
  { // Entry 235
    0x1.18c2c05650eac97c01479a1a77caa909p6,
    0x1.333334p100
  },
  { // Entry 236
    -0x1.18c2c05650eac97c01479a1a77caa909p6,
    -0x1.333334p100
  },
  { // Entry 237
    0x1.1914b70e86721bbde7a2eea6f077d548p6,
    0x1.4ccccep100
  },
  { // Entry 238
    -0x1.1914b70e86721bbde7a2eea6f077d548p6,
    -0x1.4ccccep100
  },
  { // Entry 239
    0x1.19609a053a97d6f30409751e6281de59p6,
    0x1.666668p100
  },
  { // Entry 240
    -0x1.19609a053a97d6f30409751e6281de59p6,
    -0x1.666668p100
  },
  { // Entry 241
    0x1.19a74017386a428962791f05687972f6p6,
    0x1.800002p100
  },
  { // Entry 242
    -0x1.19a74017386a428962791f05687972f6p6,
    -0x1.800002p100
  },
  { // Entry 243
    0x1.19e9567ab98dd45c6898a542a93d6c1bp6,
    0x1.99999cp100
  },
  { // Entry 244
    -0x1.19e9567ab98dd45c6898a542a93d6c1bp6,
    -0x1.99999cp100
  },
  { // Entry 245
    0x1.1a276adcd0472f52cdae405190f05814p6,
    0x1.b33336p100
  },
  { // Entry 246
    -0x1.1a276adcd0472f52cdae405190f05814p6,
    -0x1.b33336p100
  },
  { // Entry 247
    0x1.1a61f2998eab653e55cda9cf1b8d9e50p6,
    0x1.ccccd0p100
  },
  { // Entry 248
    -0x1.1a61f2998eab653e55cda9cf1b8d9e50p6,
    -0x1.ccccd0p100
  },
  { // Entry 249
    0x1.1a994fffd300555a0d63481601d36422p6,
    0x1.e6666ap100
  },
  { // Entry 250
    -0x1.1a994fffd300555a0d63481601d36422p6,
    -0x1.e6666ap100
  },
  { // Entry 251
    0x1.1acdd632f662a9e9c9c2e63a464b3927p6,
    0x1.p101
  },
  { // Entry 252
    -0x1.1acdd632f662a9e9c9c2e63a464b3927p6,
    -0x1.p101
  },
  { // Entry 253
    -0x1.65a9f84b82e62f3e42eda0a911a063e3p6,
    -0x1.fffffep127
  },
  { // Entry 254
    0x1.65a9f84b82e62f3e42eda0a911a063e3p6,
    0x1.fffffep127
  },
  { // Entry 255
    0x1.65a9f84b82e62f3e42eda0a911a063e3p6,
    0x1.fffffep127
  },
  { // Entry 256
    -0x1.65a9f84b82e62f3e42eda0a911a063e3p6,
    -0x1.fffffep127
  },
  { // Entry 257
    -0.0f,
    -0x1.p-149
  },
  { // Entry 258
    0.0f,
    0x1.p-149
  },
  { // Entry 259
    0.0,
    0.0
  },
  { // Entry 260
    0.0f,
    0x1.p-149
  },
  { // Entry 261
    -0.0f,
    -0x1.p-149
  },
  { // Entry 262
    -0x1.0499e5d08dbd469e5c8e399d3fdaf2bap-1,
    -0x1.100002p-1
  },
  { // Entry 263
    0x1.0499e5d08dbd469e5c8e399d3fdaf2bap-1,
    0x1.100002p-1
  },
  { // Entry 264
    -0x1.0499e40c65ff48fb04213eab32faa345p-1,
    -0x1.10p-1
  },
  { // Entry 265
    0x1.0499e40c65ff48fb04213eab32faa345p-1,
    0x1.10p-1
  },
  { // Entry 266
    -0x1.0499e2483e40900199471311080aa3efp-1,
    -0x1.0ffffep-1
  },
  { // Entry 267
    0x1.0499e2483e40900199471311080aa3efp-1,
    0x1.0ffffep-1
  },
  { // Entry 268
    0x1.0499e2483e40900199471311080aa3efp-1,
    0x1.0ffffep-1
  },
  { // Entry 269
    -0x1.0499e2483e40900199471311080aa3efp-1,
    -0x1.0ffffep-1
  },
  { // Entry 270
    0x1.0499e40c65ff48fb04213eab32faa345p-1,
    0x1.10p-1
  },
  { // Entry 271
    -0x1.0499e40c65ff48fb04213eab32faa345p-1,
    -0x1.10p-1
  },
  { // Entry 272
    0x1.0499e5d08dbd469e5c8e399d3fdaf2bap-1,
    0x1.100002p-1
  },
  { // Entry 273
    -0x1.0499e5d08dbd469e5c8e399d3fdaf2bap-1,
    -0x1.100002p-1
  },
  { // Entry 274
    0x1.62e42fe7a39eef5793c4bc855b3b42b2p5,
    0x1.fffffep62
  },
  { // Entry 275
    -0x1.62e42fe7a39eef5793c4bc855b3b42b2p5,
    -0x1.fffffep62
  },
  { // Entry 276
    0x1.62e42fefa39ef35793c7673007e5ed5ep5,
    0x1.p63
  },
  { // Entry 277
    -0x1.62e42fefa39ef35793c7673007e5ed5ep5,
    -0x1.p63
  },
  { // Entry 278
    0x1.62e42fffa39ee35793dcbc853d3b42e7p5,
    0x1.000002p63
  },
  { // Entry 279
    -0x1.62e42fffa39ee35793dcbc853d3b42e7p5,
    -0x1.000002p63
  },
  { // Entry 280
    0x1.419ecb612c48048b5de7682e2ddf0845p4,
    0x1.fffffep27
  },
  { // Entry 281
    -0x1.419ecb612c48048b5de7682e2ddf0845p4,
    -0x1.fffffep27
  },
  { // Entry 282
    0x1.419ecb712c480c8b5decb58387285d9dp4,
    0x1.p28
  },
  { // Entry 283
    -0x1.419ecb712c480c8b5decb58387285d9dp4,
    -0x1.p28
  },
  { // Entry 284
    0x1.419ecb912c47ec8b5e17502df20308aep4,
    0x1.000002p28
  },
  { // Entry 285
    -0x1.419ecb912c47ec8b5e17502df20308aep4,
    -0x1.000002p28
  },
  { // Entry 286
    0x1.3687a9e1af2b0cfca14944f4adc3c25ap4,
    0x1.fffffep26
  },
  { // Entry 287
    -0x1.3687a9e1af2b0cfca14944f4adc3c25ap4,
    -0x1.fffffep26
  },
  { // Entry 288
    0x1.3687a9f1af2b14fca14e7a4a06e917b2p4,
    0x1.p27
  },
  { // Entry 289
    -0x1.3687a9f1af2b14fca14e7a4a06e917b2p4,
    -0x1.p27
  },
  { // Entry 290
    0x1.3687aa11af2af4fca178e4f47253c2c1p4,
    0x1.000002p27
  },
  { // Entry 291
    -0x1.3687aa11af2af4fca178e4f47253c2c1p4,
    -0x1.000002p27
  },
  { // Entry 292
    0x1.1542456337d42a1c6b76734837564c23p4,
    0x1.fffffep23
  },
  { // Entry 293
    -0x1.1542456337d42a1c6b76734837564c23p4,
    -0x1.fffffep23
  },
  { // Entry 294
    0x1.1542457337d4321c6b73c89d84aba171p4,
    0x1.p24
  },
  { // Entry 295
    -0x1.1542457337d4321c6b73c89d84aba171p4,
    -0x1.p24
  },
  { // Entry 296
    0x1.1542459337d4121c6b8e73481f564c0ep4,
    0x1.000002p24
  },
  { // Entry 297
    -0x1.1542459337d4121c6b8e73481f564c0ep4,
    -0x1.000002p24
  },
  { // Entry 298
    0x1.0c1f89f2534d548373a25a26f285c8e4p1,
    0x1.fffffep1
  },
  { // Entry 299
    -0x1.0c1f89f2534d548373a25a26f285c8e4p1,
    -0x1.fffffep1
  },
  { // Entry 300
    0x1.0c1f8a6e80eeb23e03926981e348eb2ep1,
    0x1.p2
  },
  { // Entry 301
    -0x1.0c1f8a6e80eeb23e03926981e348eb2ep1,
    -0x1.p2
  },
  { // Entry 302
    0x1.0c1f8b66dc300f1430203df7b7466063p1,
    0x1.000002p2
  },
  { // Entry 303
    -0x1.0c1f8b66dc300f1430203df7b7466063p1,
    -0x1.000002p2
  },
  { // Entry 304
    0x1.7192174c43d9e96299f78116852f0226p0,
    0x1.fffffep0
  },
  { // Entry 305
    -0x1.7192174c43d9e96299f78116852f0226p0,
    -0x1.fffffep0
  },
  { // Entry 306
    0x1.719218313d0872f8e831837f0e954189p0,
    0x1.p1
  },
  { // Entry 307
    -0x1.719218313d0872f8e831837f0e954189p0,
    -0x1.p1
  },
  { // Entry 308
    0x1.719219fb2f63609c4a04634fb68878aep0,
    0x1.000002p1
  },
  { // Entry 309
    -0x1.719219fb2f63609c4a04634fb68878aep0,
    -0x1.000002p1
  },
  { // Entry 310
    0x1.c34364ad935baa4b7b8121ba8df62b6cp-1,
    0x1.fffffep-1
  },
  { // Entry 311
    -0x1.c34364ad935baa4b7b8121ba8df62b6cp-1,
    -0x1.fffffep-1
  },
  { // Entry 312
    0x1.c34366179d426cc1b1f33d1ba4a314a7p-1,
    0x1.p0
  },
  { // Entry 313
    -0x1.c34366179d426cc1b1f33d1ba4a314a7p-1,
    -0x1.p0
  },
  { // Entry 314
    0x1.c34368ebb10dd29f459608bca43f91c8p-1,
    0x1.000002p0
  },
  { // Entry 315
    -0x1.c34368ebb10dd29f459608bca43f91c8p-1,
    -0x1.000002p0
  },
  { // Entry 316
    0x1.ecc2c9225f040f819311fcc70a981347p-2,
    0x1.fffffep-2
  },
  { // Entry 317
    -0x1.ecc2c9225f040f819311fcc70a981347p-2,
    -0x1.fffffep-2
  },
  { // Entry 318
    0x1.ecc2caec5160994be04204a968c7020dp-2,
    0x1.p-1
  },
  { // Entry 319
    -0x1.ecc2caec5160994be04204a968c7020dp-2,
    -0x1.p-1
  },
  { // Entry 320
    0x1.ecc2ce8036189a1bdcaca590368850b2p-2,
    0x1.000002p-1
  },
  { // Entry 321
    -0x1.ecc2ce8036189a1bdcaca590368850b2p-2,
    -0x1.000002p-1
  },
  { // Entry 322
    0x1.facfb048e7ded2c6807bda0b8101be5ep-3,
    0x1.fffffep-3
  },
  { // Entry 323
    -0x1.facfb048e7ded2c6807bda0b8101be5ep-3,
    -0x1.fffffep-3
  },
  { // Entry 324
    0x1.facfb2399e636e8d67115de540a778acp-3,
    0x1.p-2
  },
  { // Entry 325
    -0x1.facfb2399e636e8d67115de540a778acp-3,
    -0x1.p-2
  },
  { // Entry 326
    0x1.facfb61b0b6c4e73771a7b444600d862p-3,
    0x1.000002p-2
  },
  { // Entry 327
    -0x1.facfb61b0b6c4e73771a7b444600d862p-3,
    -0x1.000002p-2
  },
  { // Entry 328
    0x1.fead096d8abe9f0e7222b9aab95512f1p-4,
    0x1.fffffep-4
  },
  { // Entry 329
    -0x1.fead096d8abe9f0e7222b9aab95512f1p-4,
    -0x1.fffffep-4
  },
  { // Entry 330
    0x1.fead0b6996972d06058e3eb12c938140p-4,
    0x1.p-3
  },
  { // Entry 331
    -0x1.fead0b6996972d06058e3eb12c938140p-4,
    -0x1.p-3
  },
  { // Entry 332
    0x1.fead0f61ae4831826943c3b871be46a4p-4,
    0x1.000002p-3
  },
  { // Entry 333
    -0x1.fead0f61ae4831826943c3b871be46a4p-4,
    -0x1.000002p-3
  },
  { // Entry 334
    0x1.ffaacefb4466c60d6ba2d2fac8774fdap-5,
    0x1.fffffep-5
  },
  { // Entry 335
    -0x1.ffaacefb4466c60d6ba2d2fac8774fdap-5,
    -0x1.fffffep-5
  },
  { // Entry 336
    0x1.ffaad0fa452627976ff366b9d3840fd1p-5,
    0x1.p-4
  },
  { // Entry 337
    -0x1.ffaad0fa452627976ff366b9d3840fd1p-5,
    -0x1.p-4
  },
  { // Entry 338
    0x1.ffaad4f846a4e4b46d5fa87174990d96p-5,
    0x1.000002p-4
  },
  { // Entry 339
    -0x1.ffaad4f846a4e4b46d5fa87174990d96p-5,
    -0x1.000002p-4
  },
  { // Entry 340
    0x1.ffeaab10f5a6e7d189c57c0e68bbb03ap-6,
    0x1.fffffep-6
  },
  { // Entry 341
    -0x1.ffeaab10f5a6e7d189c57c0e68bbb03ap-6,
    -0x1.fffffep-6
  },
  { // Entry 342
    0x1.ffeaad10b5b2e591fdad6bd7f7a497c3p-6,
    0x1.p-5
  },
  { // Entry 343
    -0x1.ffeaad10b5b2e591fdad6bd7f7a497c3p-6,
    -0x1.p-5
  },
  { // Entry 344
    0x1.ffeab11035cadf93754fd9171a996c98p-6,
    0x1.000002p-5
  },
  { // Entry 345
    -0x1.ffeab11035cadf93754fd9171a996c98p-6,
    -0x1.000002p-5
  },
  { // Entry 346
    0x1.fffaa8d11fa2a9bd25f4c3a139791ba3p-7,
    0x1.fffffep-7
  },
  { // Entry 347
    -0x1.fffaa8d11fa2a9bd25f4c3a139791ba3p-7,
    -0x1.fffffep-7
  },
  { // Entry 348
    0x1.fffaaad10fa369c32500d46ba7927458p-7,
    0x1.p-6
  },
  { // Entry 349
    -0x1.fffaaad10fa369c32500d46ba7927458p-7,
    -0x1.p-6
  },
  { // Entry 350
    0x1.fffaaed0efa4e96f2c182216a1ad2218p-7,
    0x1.000002p-6
  },
  { // Entry 351
    -0x1.fffaaed0efa4e96f2c182216a1ad2218p-7,
    -0x1.000002p-6
  },
  { // Entry 352
    0x1.fffffdfaaaaabad111004fa36115083cp-15,
    0x1.fffffep-15
  },
  { // Entry 353
    -0x1.fffffdfaaaaabad111004fa36115083cp-15,
    -0x1.fffffep-15
  },
  { // Entry 354
    0x1.fffffffaaaaaaad111110fa35a35b2e8p-15,
    0x1.p-14
  },
  { // Entry 355
    -0x1.fffffffaaaaaaad111110fa35a35b2e8p-15,
    -0x1.p-14
  },
  { // Entry 356
    0x1.000001fd55554568886947d19abb8424p-14,
    0x1.000002p-14
  },
  { // Entry 357
    -0x1.000001fd55554568886947d19abb8424p-14,
    -0x1.000002p-14
  },
  { // Entry 358
    0x1.fffffdfffffffeaaaaaeaaaaa6ad1112p-28,
    0x1.fffffep-28
  },
  { // Entry 359
    -0x1.fffffdfffffffeaaaaaeaaaaa6ad1112p-28,
    -0x1.fffffep-28
  },
  { // Entry 360
    0x1.fffffffffffffeaaaaaaaaaaaaad1111p-28,
    0x1.p-27
  },
  { // Entry 361
    -0x1.fffffffffffffeaaaaaaaaaaaaad1111p-28,
    -0x1.p-27
  },
  { // Entry 362
    0x1.000001ffffffff55555155554d568883p-27,
    0x1.000002p-27
  },
  { // Entry 363
    -0x1.000001ffffffff55555155554d568883p-27,
    -0x1.000002p-27
  },
  { // Entry 364
    0x1.fffffdffffffffaaaaabaaaaa9aad111p-29,
    0x1.fffffep-29
  },
  { // Entry 365
    -0x1.fffffdffffffffaaaaabaaaaa9aad111p-29,
    -0x1.fffffep-29
  },
  { // Entry 366
    0x1.ffffffffffffffaaaaaaaaaaaaaad111p-29,
    0x1.p-28
  },
  { // Entry 367
    -0x1.ffffffffffffffaaaaaaaaaaaaaad111p-29,
    -0x1.p-28
  },
  { // Entry 368
    0x1.000001ffffffffd55554555553556887p-28,
    0x1.000002p-28
  },
  { // Entry 369
    -0x1.000001ffffffffd55554555553556887p-28,
    -0x1.000002p-28
  },
  { // Entry 370
    0x1.fffffdfffffffffaaaaabaaaaa9aaad1p-31,
    0x1.fffffep-31
  },
  { // Entry 371
    -0x1.fffffdfffffffffaaaaabaaaaa9aaad1p-31,
    -0x1.fffffep-31
  },
  { // Entry 372
    0x1.fffffffffffffffaaaaaaaaaaaaaaad1p-31,
    0x1.p-30
  },
  { // Entry 373
    -0x1.fffffffffffffffaaaaaaaaaaaaaaad1p-31,
    -0x1.p-30
  },
  { // Entry 374
    0x1.000001fffffffffd5555455555355568p-30,
    0x1.000002p-30
  },
  { // Entry 375
    -0x1.000001fffffffffd5555455555355568p-30,
    -0x1.000002p-30
  },
  { // Entry 376
    HUGE_VALF,
    HUGE_VALF
  },
  { // Entry 377
    -HUGE_VALF,
    -HUGE_VALF
  },
  { // Entry 378
    0x1.65a9f84b82e62f3e42eda0a911a063e3p6,
    0x1.fffffep127
  },
  { // Entry 379
    -0x1.65a9f84b82e62f3e42eda0a911a063e3p6,
    -0x1.fffffep127
  },
  { // Entry 380
    0x1.65a9f84782e6293e42e44b53ad4b0e74p6,
    0x1.fffffcp127
  },
  { // Entry 381
    -0x1.65a9f84782e6293e42e44b53ad4b0e74p6,
    -0x1.fffffcp127
  },
  { // Entry 382
    0x1.dcbf6a62e35477f9ae79be71ed97620bp0,
    0x1.921fb6p1
  },
  { // Entry 383
    -0x1.dcbf6a62e35477f9ae79be71ed97620bp0,
    -0x1.921fb6p1
  },
  { // Entry 384
    0x1.3bc04ee951032b4f7509b3b2e0f0715ap0,
    0x1.921fb6p0
  },
  { // Entry 385
    -0x1.3bc04ee951032b4f7509b3b2e0f0715ap0,
    -0x1.921fb6p0
  },
  { // Entry 386
    0x1.c34368ebb10dd29f459608bca43f91c8p-1,
    0x1.000002p0
  },
  { // Entry 387
    -0x1.c34368ebb10dd29f459608bca43f91c8p-1,
    -0x1.000002p0
  },
  { // Entry 388
    0x1.c34366179d426cc1b1f33d1ba4a314a7p-1,
    0x1.p0
  },
  { // Entry 389
    -0x1.c34366179d426cc1b1f33d1ba4a314a7p-1,
    -0x1.p0
  },
  { // Entry 390
    0x1.c34364ad935baa4b7b8121ba8df62b6cp-1,
    0x1.fffffep-1
  },
  { // Entry 391
    -0x1.c34364ad935baa4b7b8121ba8df62b6cp-1,
    -0x1.fffffep-1
  },
  { // Entry 392
    0x1.71447831e43cde2ed30650428c5a8410p-1,
    0x1.921fb6p-1
  },
  { // Entry 393
    -0x1.71447831e43cde2ed30650428c5a8410p-1,
    -0x1.921fb6p-1
  },
  { // Entry 394
    0x1.000001ffffffffffffffffffffffffffp-126,
    0x1.000002p-126
  },
  { // Entry 395
    -0x1.000001ffffffffffffffffffffffffffp-126,
    -0x1.000002p-126
  },
  { // Entry 396
    0x1.ffffffffffffffffffffffffffffffffp-127,
    0x1.p-126
  },
  { // Entry 397
    -0x1.ffffffffffffffffffffffffffffffffp-127,
    -0x1.p-126
  },
  { // Entry 398
    0x1.fffffbffffffffffffffffffffffffffp-127,
    0x1.fffffcp-127
  },
  { // Entry 399
    -0x1.fffffbffffffffffffffffffffffffffp-127,
    -0x1.fffffcp-127
  },
  { // Entry 400
    0x1.fffff7ffffffffffffffffffffffffffp-127,
    0x1.fffff8p-127
  },
  { // Entry 401
    -0x1.fffff7ffffffffffffffffffffffffffp-127,
    -0x1.fffff8p-127
  },
  { // Entry 402
    0x1.ffffffffffffffffffffffffffffffffp-149,
    0x1.p-148
  },
  { // Entry 403
    -0x1.ffffffffffffffffffffffffffffffffp-149,
    -0x1.p-148
  },
  { // Entry 404
    0.0f,
    0x1.p-149
  },
  { // Entry 405
    -0.0f,
    -0x1.p-149
  },
  { // Entry 406
    0.0,
    0.0f
  },
  { // Entry 407
    -0.0,
    -0.0f
  }
};
