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

static data_1_1_t<double, double> g_acosh_intel_data[] = {
  { // Entry 0
    0x1.52417db067f37fff78da0e59c786a63ep8,
    0x1.0000000000001p487
  },
  { // Entry 1
    0x1.132def2b505ebfb768161d82be1f888dp9,
    0x1.0000000000001p793
  },
  { // Entry 2
    0x1.0979b1dbc2e56800030ba9b06cf83f10p9,
    0x1.000000000001fp765
  },
  { // Entry 3
    0x1.2c2fc595456a2807214d0087f4432d47p-23,
    0x1.000000000002cp0
  },
  { // Entry 4
    0x1.7fffffffffff70000000000091ccccccp-23,
    0x1.0000000000048p0
  },
  { // Entry 5
    0x1.fffffffffffaaaaaaaaaaad111111111p-22,
    0x1.00000000002p0
  },
  { // Entry 6
    0x1.bb67ae854d5db16a878f9eb2adb06a0bp-16,
    0x1.000000018p0
  },
  { // Entry 7
    0x1.69dca2563fe028021e9094ed47ed04ecp-15,
    0x1.00000003ff0p0
  },
  { // Entry 8
    0x1.30fc1934f09c97ff42ffecad467897fdp6,
    0x1.000000cp109
  },
  { // Entry 9
    0x1.6c275e69b28b4441b5463b5476d53758p-10,
    0x1.0000103p0
  },
  { // Entry 10
    0x1.b1e5d906d5ed79cefcae2668c5f67c8ap-10,
    0x1.000016fb5b0c4p0
  },
  { // Entry 11
    0x1.deee9cb901ed887353ce5684cd29c83ep-10,
    0x1.00001c0p0
  },
  { // Entry 12
    0x1.deee5b3e7d4c333cbcba1f16d8473a1ep-8,
    0x1.0001cp0
  },
  { // Entry 13
    0x1.ffffaaaad110fa35b2e863129439b017p-8,
    0x1.00020p0
  },
  { // Entry 14
    0x1.338a7b0a9bbf4515d91fc94b631d949bp-7,
    0x1.0002e2ec3f80cp0
  },
  { // Entry 15
    0x1.398892de8eab46dddf895e6b2df71e14p-7,
    0x1.00030p0
  },
  { // Entry 16
    0x1.bb66d0d2d8d230fe173d0d972c5321a0p-7,
    0x1.00060p0
  },
  { // Entry 17
    0x1.ffdea9ecfe4a23fd37592420dd1e4aecp-7,
    0x1.0007ff0p0
  },
  { // Entry 18
    0x1.6a0803b6df85a5a6a28a7d24344fd7bcp-6,
    0x1.001p0
  },
  { // Entry 19
    0x1.13b744b6fc24081df6488fc0a0521447p-5,
    0x1.00251f4dbf0f3p0
  },
  { // Entry 20
    0x1.5164c776eb38b7a1b4e392209f7cd76cp0,
    0x1.00380p1
  },
  { // Entry 21
    0x1.74927a59064b972c627d0f8dbf3a208bp-5,
    0x1.0043ca3ea0570p0
  },
  { // Entry 22
    0x1.e9b61fa83327114a9499c4386197f7ecp-5,
    0x1.007522166b864p0
  },
  { // Entry 23
    0x1.4a6b504ae30bf818ff58df731784a2e5p-4,
    0x1.00d55a07e7d7dp0
  },
  { // Entry 24
    0x1.6e48df1bd304d83259b7350ef19d654ap-4,
    0x1.010636f08d98cp0
  },
  { // Entry 25
    0x1.86cc84485647b80c608bfc977c465c3ep-4,
    0x1.012a83d511968p0
  },
  { // Entry 26
    0x1.8c96a62f43fda829f2c6aa64fc7c3f52p-4,
    0x1.01336eaa27065p0
  },
  { // Entry 27
    0x1.c96ae158c261681aae2f1ac5b1e7b53dp-4,
    0x1.01991427286a7p0
  },
  { // Entry 28
    0x1.fd303bdcd51d207b38fd033ccca4ebe0p-4,
    0x1.01fb0b7471c13p0
  },
  { // Entry 29
    0x1.01fbf091ad42880b50591ac5a3c25a55p-3,
    0x1.0208a7bec3ef6p0
  },
  { // Entry 30
    0x1.2142780a5b4da80572f1f1e417c281e0p-3,
    0x1.028ec4a860985p0
  },
  { // Entry 31
    0x1.c6f3debc6b9baf8fd4952d3e75007116p4,
    0x1.040p40
  },
  { // Entry 32
    0x1.b776eaca67a8d81470ca11e3c19618f4p-3,
    0x1.05ea9e87359f0p0
  },
  { // Entry 33
    0x1.c738f388674bbffeab4246796640039ap-3,
    0x1.0659a435f099fp0
  },
  { // Entry 34
    0x1.f33d4f7790f6982e3cae58a8f5a4c85cp-3,
    0x1.07a4d97d8d94cp0
  },
  { // Entry 35
    0x1.f6ac7bad8b4ac7489787663c51fd8389p-3,
    0x1.07cp0
  },
  { // Entry 36
    0x1.fc25c7d91809f80c15ad7b8a098904e9p-3,
    0x1.07ebaac665ee8p0
  },
  { // Entry 37
    0x1.14d72e562b86f80b92db76914c1a8483p-2,
    0x1.0969a517e7390p0
  },
  { // Entry 38
    0x1.3724eb536abd17f3549fde7c0a8bcc78p4,
    0x1.0a05028140ap27
  },
  { // Entry 39
    0x1.424e1a83309277fc74e6252f9ccff51ep4,
    0x1.0b31d5526e304p28
  },
  { // Entry 40
    0x1.42dc24aefea4a00000f4c4c42f7676bdp-2,
    0x1.0cd48770c2348p0
  },
  { // Entry 41
    0x1.aa3dbe48def817845faa61fd5cb0449ap-2,
    0x1.168p0
  },
  { // Entry 42
    0x1.6c0ff5895036d14a54136cb97458c3a1p0,
    0x1.18c6318c6318cp1
  },
  { // Entry 43
    0x1.14aeaf2cf882b800017816b0634a51c7p1,
    0x1.1999999a7f91bp2
  },
  { // Entry 44
    0x1.c636c1b2700c78000114e5846e56f02ap-2,
    0x1.1999999abcb84p0
  },
  { // Entry 45
    0x1.c636c1b55e89800000206f2d5b63746ep-2,
    0x1.1999999b12b2fp0
  },
  { // Entry 46
    0x1.c636c1b787628800007e1a95058e28f9p-2,
    0x1.1999999b52092p0
  },
  { // Entry 47
    0x1.c636c1bc867dc0000156a1eae635a35ep-2,
    0x1.1999999be4936p0
  },
  { // Entry 48
    0x1.c636c1c7da2afffffeb98fc860cd7ceep-2,
    0x1.1999999d30c68p0
  },
  { // Entry 49
    0x1.c6d30f1d087751157fa51c32440dd291p-2,
    0x1.19ab84ff770f9p0
  },
  { // Entry 50
    0x1.38138021525b17f5a7d79c6787045fbap4,
    0x1.19f9842cbe9dap27
  },
  { // Entry 51
    0x1.cff8efdd68b8b000088f99302f13fd55p-2,
    0x1.1abb14934c112p0
  },
  { // Entry 52
    0x1.4345ce06726eeffd3deec654e93bb704p4,
    0x1.1bd9ff3818250p28
  },
  { // Entry 53
    0x1.da627b574124041f55d0b8534c07caa2p-2,
    0x1.1bf734206562ep0
  },
  { // Entry 54
    0x1.dcfa110e4d2be4e60f4c2c7b792aa979p-2,
    0x1.1c4711c4711c4p0
  },
  { // Entry 55
    0x1.e4f600bca9b43c7505820f34625aedf8p-2,
    0x1.1d4p0
  },
  { // Entry 56
    0x1.435af0cd8723f7fc0f030744eaf5e4f3p4,
    0x1.1d51ee6904f05p28
  },
  { // Entry 57
    0x1.f66cd8a589f9e801dcbbaba95fa2db1bp-2,
    0x1.1f7p0
  },
  { // Entry 58
    0x1.fb04da24bd3263c3c19595829f887623p-2,
    0x1.2006d9ba6b627p0
  },
  { // Entry 59
    0x1.fb4d685e13d1738553151c2a08436513p-2,
    0x1.201034be9b997p0
  },
  { // Entry 60
    0x1.fd9747d199d9e34b5ee5a758b3a33b2ep-2,
    0x1.205bf510b5de4p0
  },
  { // Entry 61
    0x1.fde64921f2be26d349af15c65d2baec8p-2,
    0x1.206633589fb42p0
  },
  { // Entry 62
    0x1.ff88ab5b57988a62645ec106c4097863p-2,
    0x1.209c8ea824394p0
  },
  { // Entry 63
    0x1.ffaa5d190b3e38a2f5978b0cbdef37c0p-2,
    0x1.20a0f16a1f3a8p0
  },
  { // Entry 64
    0x1.43d0ccb7eaf817fbfc58bb2d606c246ap4,
    0x1.25a62ecd4ac96p28
  },
  { // Entry 65
    0x1.25942d7ea38d3037fdf235c374a0a10ap-1,
    0x1.2b4p0
  },
  { // Entry 66
    0x1.1eb90fcb975c97e99a03cd4e9ecf7efep1,
    0x1.30000000e4cffp2
  },
  { // Entry 67
    0x1.1ed61acd1cef37f72ebe2150d786654ap1,
    0x1.304376382bfc1p2
  },
  { // Entry 68
    0x1.1f962e5c168007edbcf9aaa8334a7be8p1,
    0x1.32032a240af45p2
  },
  { // Entry 69
    0x1.1fda546800eb981039b042c0a6205a51p1,
    0x1.32a2a7cec80a3p2
  },
  { // Entry 70
    0x1.1ff53fa69f9f6813df120c0fc9a7c82fp1,
    0x1.32e1bf98770d2p2
  },
  { // Entry 71
    0x1.85a6fe5151e877fffe89df73281dac1ep0,
    0x1.333333335c4e7p1
  },
  { // Entry 72
    0x1.203dae008f42281336198904d353a9d3p1,
    0x1.338bc6d217390p2
  },
  { // Entry 73
    0x1.204200d0ad3cb80822eaaf1a8fd400eep1,
    0x1.3395f01ec30aep2
  },
  { // Entry 74
    0x1.2180ae42458557f160869fa88bfdd767p1,
    0x1.3686b30ec28f9p2
  },
  { // Entry 75
    0x1.22824d7775d127ed6249aedcd653a683p1,
    0x1.38ecbb448bb60p2
  },
  { // Entry 76
    0x1.24d7aa57e09e200f0fa51b8e122a50d1p1,
    0x1.3e8fa3e8fa3e8p2
  },
  { // Entry 77
    0x1.24ead0998b45e80c15775fe412fa3476p1,
    0x1.3ebe5740abf57p2
  },
  { // Entry 78
    0x1.9119c13a31baffe46835ab2266588de9p0,
    0x1.4p1
  },
  { // Entry 79
    0x1.638eab49216f8ee9217f986540739282p-1,
    0x1.404p0
  },
  { // Entry 80
    0x1.663100c2a4fe2251bc802e040c21517cp-1,
    0x1.413e827d04fa0p0
  },
  { // Entry 81
    0x1.2a8a45eb147ce80084d5dc0629061b72p1,
    0x1.4cc5baf5c8392p2
  },
  { // Entry 82
    0x1.834b2cacec9cf00000bf6612e57cbe8fp-1,
    0x1.4ccccccd6481ap0
  },
  { // Entry 83
    0x1.834b2cb510a9c7fffe91256bde54bbddp-1,
    0x1.4cccccd0c613dp0
  },
  { // Entry 84
    0x1.869f689d41e5ae1cbc4db884da78fec0p-1,
    0x1.4e309016165fcp0
  },
  { // Entry 85
    0x1.dfcd5df1bc2707ffd5ca5383f4cce6e7p1,
    0x1.53d4f53d4f53cp4
  },
  { // Entry 86
    0x1.2e3bb6dd0b0ae0067c5f911faaaa78ddp1,
    0x1.5655956559564p2
  },
  { // Entry 87
    0x1.30af83c42c157ff130f6bbdfb23ca759p1,
    0x1.5cd735cd735ccp2
  },
  { // Entry 88
    0x1.af87977409910c12e8a8802fd87c6abfp-1,
    0x1.6070381c0e040p0
  },
  { // Entry 89
    0x1.3bacc53061f3b7f7d9035c57315345fbp4,
    0x1.6118461184610p27
  },
  { // Entry 90
    0x1.b2066fe0952af7fd5b1a52e397d20b42p-1,
    0x1.619f89771feaap0
  },
  { // Entry 91
    0x1.b243d68391f9d80c17216d59e4919bafp-1,
    0x1.61bccd7f349c4p0
  },
  { // Entry 92
    0x1.bbe95ab6d25078000176eb5757518ce0p-1,
    0x1.6666666a4d8cap0
  },
  { // Entry 93
    0x1.bce47c50e597e80168ea6ea197b7c5fbp-1,
    0x1.66e198e40a07cp0
  },
  { // Entry 94
    0x1.c4b434e7858417fe5522bdc24515e3abp-1,
    0x1.6ac2abcce660fp0
  },
  { // Entry 95
    0x1.b4b0591fab93e80c344916601f3f98fep0,
    0x1.6c0p1
  },
  { // Entry 96
    0x1.c9e777034bed37fc519e004af23c57ecp-1,
    0x1.6d63c0cb542d6p0
  },
  { // Entry 97
    0x1.cda9310b784e5000aeae7baa2dcc4cfcp-1,
    0x1.6f5p0
  },
  { // Entry 98
    0x1.d169426b135d0bbab276664d9f830c71p-1,
    0x1.7140727bb4fa3p0
  },
  { // Entry 99
    0x1.d740fdf53668a1bcea81609db9e0db68p-1,
    0x1.745p0
  },
  { // Entry 100
    0x1.bc01207bd25b6801df8e788fb5f41357p0,
    0x1.75e32cf383997p1
  },
  { // Entry 101
    0x1.ecc2caec5160436e6ef0c4dfd37de905p-1,
    0x1.7fffffffffffdp0
  },
  { // Entry 102
    0x1.ecc2caf0a75cdffffe93419822098956p-1,
    0x1.800000026c803p0
  },
  { // Entry 103
    0x1.ee3b06ecea5ed564406442d07861a73fp-1,
    0x1.80d2ba083b446p0
  },
  { // Entry 104
    0x1.f314c9cb875be7f25915ef6fe8147ea7p-1,
    0x1.839p0
  },
  { // Entry 105
    0x1.f4ba2f1cad8f475dfb4fa048b5cece75p-1,
    0x1.848p0
  },
  { // Entry 106
    0x1.fbd18e6aa534eed05007aee3d66b990ap-1,
    0x1.8895b461da6c6p0
  },
  { // Entry 107
    0x1.9bdb225dace4b0005714c41371dff0c4p1,
    0x1.90240902409p3
  },
  { // Entry 108
    0x1.0c0616dbd301e000016d7f0d89731675p0,
    0x1.9999999ac11f3p0
  },
  { // Entry 109
    0x1.d4d19d0a825927fe1b0973d8b461e8edp0,
    0x1.99cp1
  },
  { // Entry 110
    0x1.4c703d5db8586802badfb82b797d3dc0p1,
    0x1.b0020p2
  },
  { // Entry 111
    0x1.1efb699cdcd33801fb03b9466fdd60fap0,
    0x1.b26c9b26c9b26p0
  },
  { // Entry 112
    0x1.2d72a3ace48437fde986eb51409ae273p0,
    0x1.c6f61e8a542a8p0
  },
  { // Entry 113
    0x1.f1b4656fac2777ff0b0732f4ed9eaaf0p0,
    0x1.c86p1
  },
  { // Entry 114
    0x1.5550540d3de547fce11196feb22aa2e1p1,
    0x1.ceb1dd915e476p2
  },
  { // Entry 115
    0x1.e4db571e008197fe9e09c3aa26aa7fccp3,
    0x1.d0741d0741d04p20
  },
  { // Entry 116
    0x1.07eac9f6dafa57ff028d331cb48f9038p3,
    0x1.dd374dd374dd0p10
  },
  { // Entry 117
    0x1.e784c2b3e554f800004d96919f791652p5,
    0x1.e3920fcba08c5p86
  },
  { // Entry 118
    0x1.e4bcd2d77ead3ffffa7087c93f5678b5p2,
    0x1.e6bd865d59181p9
  },
  { // Entry 119
    0x1.09ba252166ce8800003aa2a95746a4aap3,
    0x1.f8fc7e3f1f880p10
  },
  { // Entry 120
    0x1.4e6b108abebaefffc5c616605660da14p0,
    0x1.fb5p0
  },
  { // Entry 121
    0x1.2a66594f2e5b0fffff7ff379f5e243a7p9,
    0x1.fff003fffffffp859
  },
  { // Entry 122
    0x1.081ca3e524daf5a4d1e9e6092a37c659p1,
    0x1.fff7fffffffffp1
  },
  { // Entry 123
    0x1.081ce5ff7fcfd7ff29362493ef56165fp1,
    0x1.fff8fffffffffp1
  },
  { // Entry 124
    0x1.6262acbb698ca80507700d5ef3d0c5adp1,
    0x1.fffcfffffffffp2
  },
  { // Entry 125
    0x1.8e8f43d38040fffeda732c8d164c1eb5p8,
    0x1.fffffbbffffffp573
  },
  { // Entry 126
    0x1.c55179395a000800ddc334790469d4dep7,
    0x1.fffffe3ffffffp325
  },
  { // Entry 127
    0x1.27a094edef0c27ffb3d9ba9f6d2910a5p9,
    0x1.fffffe3ffffffp851
  },
  { // Entry 128
    0x1.27f94df9eaf50fbc89beac79392b0a20p9,
    0x1.fffffe3ffffffp852
  },
  { // Entry 129
    0x1.bb7d2fe3dbf7f7fee03edebc7a01d599p1,
    0x1.fffffffbfbfffp3
  },
  { // Entry 130
    0x1.62e3efef359dffffb4e2975678a61bf4p2,
    0x1.ffffffff8ffffp6
  },
  { // Entry 131
    0x1.86ef5ccdfa1b17fe78c886a9d8b2faaep7,
    0x1.ffffffffddfffp280
  },
  { // Entry 132
    0x1.62e3efef419e17fffe6390b9f02bcc28p2,
    0x1.ffffffffeffffp6
  },
  { // Entry 133
    0x1.62e3efef439dffffd26b10f8467623p2,
    0x1.ffffffffffff1p6
  },
  { // Entry 134
    0x1.419ecb712c4808035decb58386841d9dp4,
    0x1.ffffffffffff7p27
  },
  { // Entry 135
    0x1.633ce8fb9f87dafc69ac5909d3e5a6d9p9,
    0x1.ffffffffffffap1023
  },
  { // Entry 136
    0x1.62e3efef439e1800026ba0fa2d3cdb98p2,
    0x1.ffffffffffffdp6
  },
  { // Entry 137
    0x1.5ca72d17ed3ea80089ae65dfafc1e2b2p8,
    0x1.ffffffffffffep501
  },
  { // Entry 138
    0.0,
    0x1.0p0
  },
  { // Entry 139
    0x1.9f323ecbf9848bf835a433c0ce9aed17p-2,
    0x1.1555555555555p0
  },
  { // Entry 140
    0x1.23a4fbcdbc0835819feea2ceae6532bdp-1,
    0x1.2aaaaaaaaaaaap0
  },
  { // Entry 141
    0x1.62e42fefa39ec8ace91cbc855a44bdf6p-1,
    0x1.3ffffffffffffp0
  },
  { // Entry 142
    0x1.973a2448a635d2473522e0e7015d28f1p-1,
    0x1.5555555555554p0
  },
  { // Entry 143
    0x1.c484603eb09c0970ffa86254d6babfa5p-1,
    0x1.6aaaaaaaaaaa9p0
  },
  { // Entry 144
    0x1.ecc2caec5160600d94b684cdb2112543p-1,
    0x1.7fffffffffffep0
  },
  { // Entry 145
    0.0,
    0x1.0p0
  },
  { // Entry 146
    0x1.79072028586b73758a4f622cafb07d48p-1,
    0x1.489a5796de0b2p0
  },
  { // Entry 147
    0x1.94d80f30e93e5e29997af8fe4481c88cp-1,
    0x1.54494203c1934p0
  },
  { // Entry 148
    0x1.cddcc71de32ab5ac57c13ba40ec7963bp-1,
    0x1.6f6a8be981db0p0
  },
  { // Entry 149
    0x1.8fcb9d874c026f2c12450971bb1bddfcp-1,
    0x1.521792ea7d26ep0
  },
  { // Entry 150
    0x1.8ca5043b79263a06aa0f70d7d0bda22bp-2,
    0x1.13723f2585da2p0
  },
  { // Entry 151
    0x1.ecc2caec5160994be04204a968c7020dp-1,
    0x1.8p0
  },
  { // Entry 152
    0x1.ecc2caec5160994be04204a968c7020dp-1,
    0x1.8p0
  },
  { // Entry 153
    0x1.0893ff7cee46eb16015477f9b6695819p0,
    0x1.9555555555555p0
  },
  { // Entry 154
    0x1.193ea7aad030a176a4198d5505137cb5p0,
    0x1.aaaaaaaaaaaaap0
  },
  { // Entry 155
    0x1.28a7cbb850061ed8cb452c64c52218c9p0,
    0x1.bffffffffffffp0
  },
  { // Entry 156
    0x1.37030b8cc93542ccc38cca9157b0f26dp0,
    0x1.d555555555554p0
  },
  { // Entry 157
    0x1.44779e1ebd847257f6c077cb3350b457p0,
    0x1.eaaaaaaaaaaa9p0
  },
  { // Entry 158
    0x1.5124271980433744c1063fe570409b9ap0,
    0x1.ffffffffffffep0
  },
  { // Entry 159
    0x1.ecc2caec5160994be04204a968c7020dp-1,
    0x1.8p0
  },
  { // Entry 160
    0x1.0c2423fc001c38dcbc9cd1946000f563p0,
    0x1.99bf25234bccap0
  },
  { // Entry 161
    0x1.197e89ca48809b3746de418fbf0ee383p0,
    0x1.aaffe573bd7bbp0
  },
  { // Entry 162
    0x1.261b72900d136b90cbef8fa9a3bbd85ap0,
    0x1.bc5ccd71976cbp0
  },
  { // Entry 163
    0x1.fbbfb95324eb186f3d677aed30c35884p-1,
    0x1.888b56d86b26ep0
  },
  { // Entry 164
    0x1.4cf1a48b4bdba9043707a45b35f0d529p0,
    0x1.f8cc6db1bbcb4p0
  },
  { // Entry 165
    0x1.51242719804349be684bd0188d52ceccp0,
    0x1.0p1
  },
  { // Entry 166
    0x1.18080dd3171b6c031a9b576be63b6d4cp6,
    0x1.0p100
  },
  { // Entry 167
    0x1.1869a6d0fc0c8734cff5be4c994a623cp6,
    0x1.199999999999ap100
  },
  { // Entry 168
    0x1.18c2c053a6401fdf8f801885ecec896ep6,
    0x1.3333333333334p100
  },
  { // Entry 169
    0x1.1914b70ad53709fc02e60c9931465d1cp6,
    0x1.4cccccccccccep100
  },
  { // Entry 170
    0x1.19609a00a84eb5469b8a14575cfcffdcp6,
    0x1.6666666666668p100
  },
  { // Entry 171
    0x1.19a74011e314f1179b5984282f925681p6,
    0x1.8000000000002p100
  },
  { // Entry 172
    0x1.19e95674b98dd93c68942542ae48ec14p6,
    0x1.999999999999cp100
  },
  { // Entry 173
    0x1.1a276ad639b09e9294f7218ef587ce6cp6,
    0x1.b333333333336p100
  },
  { // Entry 174
    0x1.1a61f2927239a4e5d75ab70952b3595ap6,
    0x1.cccccccccccd0p100
  },
  { // Entry 175
    0x1.1a994ff83eca77f3ef91866a7b8540e2p6,
    0x1.e66666666666ap100
  },
  { // Entry 176
    0x1.1acdd632f662a9e9c9c2e63a464b3927p6,
    0x1.0p101
  },
  { // Entry 177
    0x1.16a529a32777cd0fc3079004b633875fp7,
    0x1.0p200
  },
  { // Entry 178
    0x1.16d5f62219f05aa89db4c3750fbb01d6p7,
    0x1.199999999999ap200
  },
  { // Entry 179
    0x1.170282e36f0a26fdfd79f091b98c1570p7,
    0x1.3333333333334p200
  },
  { // Entry 180
    0x1.172b7e3f06859c0c372cea9b5bb8ff47p7,
    0x1.4cccccccccccep200
  },
  { // Entry 181
    0x1.17516fb9f01171b1837eee7a719450a6p7,
    0x1.6666666666668p200
  },
  { // Entry 182
    0x1.1774c2c28d748f9a0366a662dadefbf9p7,
    0x1.8000000000002p200
  },
  { // Entry 183
    0x1.1795cdf3f8b103ac6a03f6f01a3a46c3p7,
    0x1.999999999999cp200
  },
  { // Entry 184
    0x1.17b4d824b8c26657803575163dd9b7efp7,
    0x1.b333333333336p200
  },
  { // Entry 185
    0x1.17d21c02d506e98121673fd36c6f7d66p7,
    0x1.cccccccccccd0p200
  },
  { // Entry 186
    0x1.17edcab5bb4f53082d82a78400d8712ap7,
    0x1.e66666666666ap200
  },
  { // Entry 187
    0x1.18080dd3171b6c031a9b576be63b6d4cp7,
    0x1.0p201
  },
  { // Entry 188
    0x1.5aeb8fdc01b221605c35ac9eb3b88349p9,
    0x1.0p1000
  },
  { // Entry 189
    0x1.5af7c2fbbe5044c692e0f97aca1a61e7p9,
    0x1.199999999999ap1000
  },
  { // Entry 190
    0x1.5b02e62c1396b7dbead244c1f48ea6cdp9,
    0x1.3333333333334p1000
  },
  { // Entry 191
    0x1.5b0d2502f975951f793f03445d19e143p9,
    0x1.4cccccccccccep1000
  },
  { // Entry 192
    0x1.5b16a161b3d88a88cc53843c2290b59bp9,
    0x1.6666666666668p1000
  },
  { // Entry 193
    0x1.5b1f7623db315202ec4d72363ce36070p9,
    0x1.8000000000002p1000
  },
  { // Entry 194
    0x1.5b27b8f036006f0785f4c6598cba3322p9,
    0x1.999999999999cp1000
  },
  { // Entry 195
    0x1.5b2f7b7c6604c7b24b8125e315a20f6dp9,
    0x1.b333333333336p1000
  },
  { // Entry 196
    0x1.5b36cc73ed15e87cb3cd9892614780cbp9,
    0x1.cccccccccccd0p1000
  },
  { // Entry 197
    0x1.5b3db820a6a802de76d4727e8661bdbcp9,
    0x1.e66666666666ap1000
  },
  { // Entry 198
    0x1.5b4448e7fd9b091d321a9e787fba7cc4p9,
    0x1.0p1001
  },
  { // Entry 199
    0.0,
    0x1.0p0
  },
  { // Entry 200
    0x1.ecc2caec51607cacba7c44bb8e7ed846p-1,
    0x1.7ffffffffffffp0
  },
  { // Entry 201
    0x1.ecc2caec5160994be04204a968c7020dp-1,
    0x1.8p0
  },
  { // Entry 202
    0x1.ecc2caec5160b5eb0607c49740e9a298p-1,
    0x1.8000000000001p0
  },
  { // Entry 203
    0x1.512427198043408194a907fefefaf99cp0,
    0x1.fffffffffffffp0
  },
  { // Entry 204
    0x1.51242719804349be684bd0188d52ceccp0,
    0x1.0p1
  },
  { // Entry 205
    0x1.5124271980435c380f91604ba8dadeb9p0,
    0x1.0000000000001p1
  },
  { // Entry 206
    0x1.081eb4b42159138d780ef9da45476c93p1,
    0x1.fffffffffffffp1
  },
  { // Entry 207
    0x1.081eb4b4215917af0d37af17fbf93f73p1,
    0x1.0p2
  },
  { // Entry 208
    0x1.081eb4b421591ff23789199368f32314p1,
    0x1.0000000000001p2
  },
  { // Entry 209
    0x1.1542457337d4299c6b73c89d8469a171p4,
    0x1.fffffffffffffp23
  },
  { // Entry 210
    0x1.1542457337d42a1c6b73c89d84aba171p4,
    0x1.0p24
  },
  { // Entry 211
    0x1.1542457337d42b1c6b73c89d8523a171p4,
    0x1.0000000000001p24
  },
  { // Entry 212
    0x1.3687a9f1af2b145ca14e7a4a06e617b2p4,
    0x1.fffffffffffffp26
  },
  { // Entry 213
    0x1.3687a9f1af2b14dca14e7a4a06e917b2p4,
    0x1.0p27
  },
  { // Entry 214
    0x1.3687a9f1af2b15dca14e7a4a06e317b2p4,
    0x1.0000000000001p27
  },
  { // Entry 215
    0x1.419ecb712c480c035decb58387261d9dp4,
    0x1.fffffffffffffp27
  },
  { // Entry 216
    0x1.419ecb712c480c835decb58387285d9dp4,
    0x1.0p28
  },
  { // Entry 217
    0x1.419ecb712c480d835decb5838720dd9dp4,
    0x1.0000000000001p28
  },
  { // Entry 218
    0x1.62e42fefa39ef31793c7673007e4ed5ep5,
    0x1.fffffffffffffp62
  },
  { // Entry 219
    0x1.62e42fefa39ef35793c7673007e5ed5ep5,
    0x1.0p63
  },
  { // Entry 220
    0x1.62e42fefa39ef3d793c7673007e1ed5ep5,
    0x1.0000000000001p63
  },
  { // Entry 221
    0x1.601e678fc457b550e49fd861a7d5a183p6,
    0x1.fffffffffffffp125
  },
  { // Entry 222
    0x1.601e678fc457b570e49fd861a7d62183p6,
    0x1.0p126
  },
  { // Entry 223
    0x1.601e678fc457b5b0e49fd861a7d42183p6,
    0x1.0000000000001p126
  },
  { // Entry 224
    0x1.628b76e3a7b60b96bde275563be3e3e3p9,
    0x1.fffffffffffffp1021
  },
  { // Entry 225
    0x1.628b76e3a7b60b9abde275563be3f3e3p9,
    0x1.0p1022
  },
  { // Entry 226
    0x1.628b76e3a7b60ba2bde275563be3b3e3p9,
    0x1.0000000000001p1022
  },
  { // Entry 227
    0x1.633ce8fb9f87db1069ac5909d3e7d6d9p9,
    0x1.fffffffffffffp1023
  },
  { // Entry 228
    HUGE_VAL,
    HUGE_VAL
  },
  { // Entry 229
    0x1.633ce8fb9f87db1069ac5909d3e7d6d9p9,
    0x1.fffffffffffffp1023
  },
  { // Entry 230
    0x1.633ce8fb9f87db0c69ac5909d3e7a6d9p9,
    0x1.ffffffffffffep1023
  },
  { // Entry 231
    0x1.cfc02f90106c17a3fd778845de3494b4p0,
    0x1.921fb54442d18p1
  },
  { // Entry 232
    0x1.05f23c6cbaf30c042e32011989ade594p0,
    0x1.921fb54442d18p0
  },
  { // Entry 233
    0x1.6a09e667f3bcc725fb1d3377443ae618p-26,
    0x1.0000000000001p0
  },
  { // Entry 234
    0.0,
    0x1.0p0
  }
};
