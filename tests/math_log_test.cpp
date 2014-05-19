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
} log_intel_data_t;

static log_intel_data_t g_log_intel_data[] = {
  { // Entry 0
    0x1.d77fd13d27ffefffffffffffb5ed9843p-11,
    0x1.003af6c37c1d3p0
  },
  { // Entry 1
    0x1.411efd297c7808000000000016d615b2p-6,
    0x1.05112792934b3p0
  },
  { // Entry 2
    0x1.d4840664446fb800000000001f574ffcp-6,
    0x1.076d1d8c75ea6p0
  },
  { // Entry 3
    0x1.f6e4c3ced7c71ffffffffffffee947dap-3,
    0x1.47408cb9583cep0
  },
  { // Entry 4
    0x1.1a0408712e009ffffffffffffee46012p-2,
    0x1.512b3126454f3p0
  },
  { // Entry 5
    0x1.8b52c15ea9c237fffffffffffe61395cp-2,
    0x1.789e95b11578cp0
  },
  { // Entry 6
    0x1.d707029bb59d8ffffffffffffecf6ef6p-2,
    0x1.958497f7b353fp0
  },
  { // Entry 7
    0x1.0727af5fee8f5ffffffffffffec7021fp-1,
    0x1.ac032a8d2ec23p0
  },
  { // Entry 8
    0x1.12fcce02efb320000000000001435b90p-1,
    0x1.b604e1942098dp0
  },
  { // Entry 9
    0x1.178e6d3ecace98000000000001300accp-1,
    0x1.b9f1fa4587967p0
  },
  { // Entry 10
    0x1.62f71c4656b60fffffffffffff7f168ep-1,
    0x1.000976581ce4ep1
  },
  { // Entry 11
    0x1.d6336a88077a9fffffffffffffe1f817p0,
    0x1.91a8dff540ff7p2
  },
  { // Entry 12
    0x1.016e82ceda358fffffffffffffacbf62p1,
    0x1.de37fb31fd5fcp2
  },
  { // Entry 13
    0x1.1e126f5d95f3880000000000005d3761p1,
    0x1.2b1199e497739p3
  },
  { // Entry 14
    0x1.2f3832cad3d5f000000000000078801cp1,
    0x1.55f0eaa1b2fc8p3
  },
  { // Entry 15
    0x1.42ee3c7dc494600000000000004ea57fp1,
    0x1.8ede492d96072p3
  },
  { // Entry 16
    0x1.6b5df7e3cd421800000000000044613bp1,
    0x1.11867637cbd03p4
  },
  { // Entry 17
    0x1.7e7f095703eee7ffffffffffffacdc06p1,
    0x1.3d9d7d597a9ddp4
  },
  { // Entry 18
    0x1.9687c83faf0067ffffffffffffb9cf3fp1,
    0x1.7f3825778aaafp4
  },
  { // Entry 19
    0x1.83d4bcdebb3f3fffffffffffffffacc9p2,
    0x1.ac50b409c8aeep8
  },
  { // Entry 20
    0x1.fffffffffffff0000000000000aaaaaap-53,
    0x1.0000000000001p0
  },
  { // Entry 21
    0x1.76e7e5d7b6eabffffffffffffff06f80p3,
    0x1.de7cd6751029ap16
  },
  { // Entry 22
    0x1.fffffffffffff0000000000000aaaaaap-53,
    0x1.0000000000001p0
  },
  { // Entry 23
    -0x1.91550c357f883fff899dff30d0f303eep8,
    0x1.0000000000003p-579
  },
  { // Entry 24
    0x1.7fffffffffffb8000000000011ffffffp-50,
    0x1.0000000000006p0
  },
  { // Entry 25
    0x1.bffffffffffe780000000001c9555555p-48,
    0x1.000000000001cp0
  },
  { // Entry 26
    0x1.7fdfffffff7017ff000047ee017ff52cp-41,
    0x1.0000000000bffp0
  },
  { // Entry 27
    0x1.0fffffffedf0000001996aaaaa81e28ap-35,
    0x1.00000000220p0
  },
  { // Entry 28
    0x1.ffffffff00000000aaaaaaaa2aaaaaabp-33,
    0x1.000000010p0
  },
  { // Entry 29
    0x1.7fffff70000047ffffd78000184cccbdp-25,
    0x1.000000cp0
  },
  { // Entry 30
    0x1.0a2ea3e77af060004010c4f6b437fb45p1,
    0x1.00070p3
  },
  { // Entry 31
    0x1.90412094d367f99f3c615231ad98bf3dp-13,
    0x1.000c82573f5f9p0
  },
  { // Entry 32
    0x1.ffe002aa6ab1106678ad8b318cb38545p-12,
    0x1.002p0
  },
  { // Entry 33
    0x1.bfcf07242969d15db4d2b3efe1c037e0p-11,
    0x1.00380p0
  },
  { // Entry 34
    0x1.d77fd13d27ffefffffffffffb5ed9843p-11,
    0x1.003af6c37c1d3p0
  },
  { // Entry 35
    0x1.ea054ce8508e4874442e95e41718b0dep-10,
    0x1.007a9ea7a9e80p0
  },
  { // Entry 36
    0x1.ff004a7ab9083f9e1717d3a4aaec8657p-10,
    0x1.007fdff7fdfe0p0
  },
  { // Entry 37
    0x1.ffda65d44dccc889115a4a4ff75c5a77p-10,
    0x1.0080169a16cd5p0
  },
  { // Entry 38
    0x1.b956989d2589d7fd6838e57a702f38acp-9,
    0x1.00dd0a8317176p0
  },
  { // Entry 39
    0x1.fd0816d97152c7fead7b849f7614fb48p-9,
    0x1.00ff02ba8a543p0
  },
  { // Entry 40
    -0x1.37915555785b07fc19dd2632088a1e7fp9,
    0x1.00fffffffffffp-899
  },
  { // Entry 41
    0x1.64a2a9b6a40588010102807138dff50cp0,
    0x1.01c00000000dcp2
  },
  { // Entry 42
    0x1.d465957106bd286bae67c08297163ea3p-8,
    0x1.01d6131d09dc5p0
  },
  { // Entry 43
    0x1.fdf639bea5ad3762df24a208e2a1cbe8p-8,
    0x1.01fff37a34084p0
  },
  { // Entry 44
    0x1.fdf6e4fcf456977205228530acd57155p-8,
    0x1.01fff426c8cb6p0
  },
  { // Entry 45
    0x1.fdf6e4fd0426f76d6d7b839b5f6c8bafp-8,
    0x1.01fff426c8db5p0
  },
  { // Entry 46
    0x1.fdfe9c57debe1760fe4ff0899bf9e6dcp-8,
    0x1.01fffbed922e3p0
  },
  { // Entry 47
    -0x1.d17be2451229ffff11e851804e97cdd7p3,
    0x1.028p-21
  },
  { // Entry 48
    0x1.6dcb2fed7f25d3f286707230be190760p-7,
    0x1.02dfafa07df6dp0
  },
  { // Entry 49
    0x1.ff092a85ee02f7fe7fc5dc1d417184c0p-7,
    0x1.040615461f6cap0
  },
  { // Entry 50
    0x1.0aa53cea3f1abfff1ddfb5af4ac49aaep-6,
    0x1.04334eec5a65dp0
  },
  { // Entry 51
    0x1.6ea07021c133480174835d142a4b831ep-6,
    0x1.05cb09bb9fed7p0
  },
  { // Entry 52
    0x1.7199f984f560800135ff3aec3a0796a8p-6,
    0x1.05d735184261cp0
  },
  { // Entry 53
    0x1.7788bc9c08318801546c035cbd9a269ap-6,
    0x1.05ef7bdee7be0p0
  },
  { // Entry 54
    0x1.c2688e952a7237fdd8563297de625acfp-6,
    0x1.0722a05711778p0
  },
  { // Entry 55
    0x1.e02151352512f7c230cc4b895dc68dd1p-6,
    0x1.079cf00fe24f9p0
  },
  { // Entry 56
    -0x1.3e1fee699c6bc7fc001a359e0efe2db7p8,
    0x1.07cp-459
  },
  { // Entry 57
    0x1.f31b56b8b6f1a2862582f528b33c4de2p-6,
    0x1.07eb259ee01b5p0
  },
  { // Entry 58
    -0x1.d0819095fcd6c800924ecd0308ebe218p3,
    0x1.0a8551f8fc7f8p-21
  },
  { // Entry 59
    -0x1.0268d40000c718005da0249ebb0a3024p1,
    0x1.0fff8p-3
  },
  { // Entry 60
    0x1.6b5df7e3cd421800000000000044613bp1,
    0x1.11867637cbd03p4
  },
  { // Entry 61
    0x1.6b96cb66f55c2800172d81c1cef357b1p1,
    0x1.11ffffffffff8p4
  },
  { // Entry 62
    -0x1.f7a213a7cd380fffde498c453f12553dp0,
    0x1.1e6p-3
  },
  { // Entry 63
    -0x1.a0765853dec06ffedf49002b5d3008bep8,
    0x1.206658d9b0f05p-601
  },
  { // Entry 64
    0x1.0d7b6ff6e3a247fffffefb9449faf8e6p6,
    0x1.2514738e6dcadp97
  },
  { // Entry 65
    0x1.1ee3899f6818e7ffffd09e6eb4879725p1,
    0x1.2cfbb9e4a1599p3
  },
  { // Entry 66
    -0x1.33153336792897fd38f4af7ab5c00769p6,
    0x1.2f0617037e590p-111
  },
  { // Entry 67
    -0x1.04c1e6cec96377febe1183e3bd373c3ep3,
    0x1.2f29fd4b42515p-12
  },
  { // Entry 68
    -0x1.0578bc4a143d699806f945d1356d06a2p-1,
    0x1.333e1f03af55ep-1
  },
  { // Entry 69
    -0x1.fd2a1d4dcf23d7fe75f7eef9498fdaa2p-2,
    0x1.3767a0aaf1452p-1
  },
  { // Entry 70
    -0x1.fb0ce0ec79bc97fe95141ce227ed47d2p-2,
    0x1.380c640e6246cp-1
  },
  { // Entry 71
    -0x1.ee693caa501a93f30062e2a3c445ac9cp-2,
    0x1.3bec837d601c1p-1
  },
  { // Entry 72
    -0x1.ed67b9b0f2f8d420adcdf51f432817fap-2,
    0x1.3c3c0p-1
  },
  { // Entry 73
    0x1.25397dc9f85bd800000a3d55bd6d8e50p1,
    0x1.3c419b9db662ap3
  },
  { // Entry 74
    0x1.9c041f7ed8d329e312aad8493a3a79a2p0,
    0x1.3ffffffffffffp2
  },
  { // Entry 75
    0x1.c8ff7c79a9a21ac25d81ef2ffb9a24aep-3,
    0x1.4p0
  },
  { // Entry 76
    0x1.d42adfec35d147fe04ff7b1b027c5fdap-3,
    0x1.41cp0
  },
  { // Entry 77
    0x1.d7577477b9ead7fe9b2bc6960a766303p-3,
    0x1.423fc24d04fc6p0
  },
  { // Entry 78
    -0x1.d360e90c3850b36d61103f575b991880p-2,
    0x1.446p-1
  },
  { // Entry 79
    -0x1.cdeabe01a86b2d50c8ea3bb030c66bbcp-2,
    0x1.461c159a76d4bp-1
  },
  { // Entry 80
    0x1.fb9186d5e3e244ee4e28531563b69a2cp-3,
    0x1.47fffffffffffp0
  },
  { // Entry 81
    0x1.fb9186d5e3e2a8d55466c3794d2ca092p-3,
    0x1.480p0
  },
  { // Entry 82
    0x1.fb920464f5fc777f3cb0d8448c30e701p-3,
    0x1.4800141bea83fp0
  },
  { // Entry 83
    0x1.fb9c2792045e9e82eff43ef1ce0bee02p-3,
    0x1.4801b3bf42a4ap0
  },
  { // Entry 84
    0x1.fc46718a2dc22ddbfabb2598a137baa7p-3,
    0x1.481cfade8a64dp0
  },
  { // Entry 85
    0x1.ff9bd7855aaeab815fd85d2cb0694231p-3,
    0x1.48a5d0c7ac5b5p0
  },
  { // Entry 86
    0x1.ffd65f29bfd4edf98ec849c90032f302p-3,
    0x1.48af356081ca1p0
  },
  { // Entry 87
    0x1.e737cb23865c6b921552ad81d572b729p-1,
    0x1.4b8p1
  },
  { // Entry 88
    0x1.90ed52d1a23c4ffed8ee7725c53035d6p8,
    0x1.556ee611fa309p578
  },
  { // Entry 89
    -0x1.b25b8d863b3b4800fcec4eb5c14a7c2ep3,
    0x1.55ep-20
  },
  { // Entry 90
    0x1.16758a93d29af7ff100a0c107fb0c9e7p6,
    0x1.5996659966598p100
  },
  { // Entry 91
    -0x1.7e3a93bf3540d81e3dbbb57585ec1674p-2,
    0x1.608054de74582p-1
  },
  { // Entry 92
    0x1.583a417be2216cb936610ca9843511dbp-2,
    0x1.6649bb515354fp0
  },
  { // Entry 93
    -0x1.4f7dac3f1d7f3fffffdf521ccfc9dbcdp5,
    0x1.69b109377f7p-61
  },
  { // Entry 94
    0x1.62e42fefa383734a769bb41258319c1ap-2,
    0x1.6a09e667f3b31p0
  },
  { // Entry 95
    -0x1.e7a2dd016daa87ffa79528e89b610c0ap1,
    0x1.6b0p-6
  },
  { // Entry 96
    0x1.739d7f6bbd0069ce24c53fad3ef796abp-2,
    0x1.7p0
  },
  { // Entry 97
    -0x1.4c3af4797206280013c4d45af696d7cbp-2,
    0x1.7223cef78e25dp-1
  },
  { // Entry 98
    -0x1.48e65fe2c23057fdeb25c7f7c4859e9cp-2,
    0x1.73587ce753680p-1
  },
  { // Entry 99
    -0x1.4057b02bfdd097fe57379888740d8103p-2,
    0x1.76764087395fcp-1
  },
  { // Entry 100
    0x1.4a5f12ed407a400083645efcc658eda6p8,
    0x1.8ad934b838bdap476
  },
  { // Entry 101
    -0x1.83d0f90690d0f7ff207b1f007f7a3917p1,
    0x1.8bep-5
  },
  { // Entry 102
    -0x1.e387545735931400edb47ee4265d728ep-1,
    0x1.8e4p-2
  },
  { // Entry 103
    -0x1.fcc59e01369768243eca599e59e6a909p-3,
    0x1.8f6p-1
  },
  { // Entry 104
    -0x1.edd6149b33156828750bb3a608c4ddc1p-3,
    0x1.924c57d629082p-1
  },
  { // Entry 105
    0x1.cfbe6164a73f1361604926a0a90f519fp-2,
    0x1.92a4cb798664bp0
  },
  { // Entry 106
    -0x1.b428216a5c1587fdbf2f97d4c27148ccp4,
    0x1.980p-40
  },
  { // Entry 107
    -0x1.d081f0f25d66380027d59b6ebdb9bac9p-3,
    0x1.9819d0255be60p-1
  },
  { // Entry 108
    -0x1.cc0d6a92b79157fe82bfad0addd085a5p-1,
    0x1.a0ef883be2177p-2
  },
  { // Entry 109
    -0x1.8f62e2cb658638000030eb1a289f78e3p-3,
    0x1.a549336310e6fp-1
  },
  { // Entry 110
    -0x1.3651d0de2368d7fee3d1d6ee2b227744p-3,
    0x1.b80300c030118p-1
  },
  { // Entry 111
    0x1.20f25e686e7f97feceff9a7299fcd1edp-1,
    0x1.c22024dc8289cp0
  },
  { // Entry 112
    -0x1.9e327eb6ac2c37f7a39af2c988abe2b6p-1,
    0x1.c80p-2
  },
  { // Entry 113
    -0x1.b76f3d0aaf0c97fadefb4f5332d365a9p-4,
    0x1.cbea22c8a9a16p-1
  },
  { // Entry 114
    -0x1.9335e5d59499181fa0f312a9854d3042p-4,
    0x1.cffffffffffffp-1
  },
  { // Entry 115
    0x1.b6cc2b05c90a77fe9a0bb3019d8a9aedp2,
    0x1.dae1894a2dbedp9
  },
  { // Entry 116
    -0x1.fc702b8d96d0d7d4794fd56c9bec2ce2p-5,
    0x1.e1304c1304c13p-1
  },
  { // Entry 117
    -0x1.f6c7f7d0b03557d1c2690fc214b3ef43p-5,
    0x1.e185646bc7856p-1
  },
  { // Entry 118
    -0x1.eb8dcb764e07e7d4f0ef36c6ee88eacap-5,
    0x1.e22e7349f6e97p-1
  },
  { // Entry 119
    -0x1.d94a9c323e77c7fd43e2e13f541141cbp-5,
    0x1.e341f0592c2abp-1
  },
  { // Entry 120
    -0x1.ccb106085394f7a05e03df8ac23eae5dp-5,
    0x1.e4005dfd66c32p-1
  },
  { // Entry 121
    -0x1.a9bd8afc6d84b7ec7bbc80cc6f2983f8p-5,
    0x1.e61222241ca7fp-1
  },
  { // Entry 122
    -0x1.787392a45888d8006bb538c724c45f7ep-5,
    0x1.e901133e161e4p-1
  },
  { // Entry 123
    -0x1.79a634cdfb255800008607b0c2c01570p4,
    0x1.eddc0821c76c1p-35
  },
  { // Entry 124
    0x1.5172153d5fe9b7ff0f26a87c085b87d7p-1,
    0x1.eed9208a6a349p0
  },
  { // Entry 125
    -0x1.12b227244ff977d878e1166c29f76aa5p-5,
    0x1.ef1dbcea195cdp-1
  },
  { // Entry 126
    -0x1.f8a34d2505f817d848f4e8959713fb76p-6,
    0x1.f0786fa48bd9cp-1
  },
  { // Entry 127
    0x1.06885d03c19c38000002f99205a60b54p1,
    0x1.f1a945a457d96p2
  },
  { // Entry 128
    0x1.a28cccb9c6f0d7fedd89e9c4d4b9f68bp6,
    0x1.f1fffffffffffp150
  },
  { // Entry 129
    -0x1.b032da095671e7c8d427eb42292764d2p-6,
    0x1.f2ab9dee841b5p-1
  },
  { // Entry 130
    -0x1.9d66ada69056485326194e3f1c7b170cp-6,
    0x1.f33e2a944ac96p-1
  },
  { // Entry 131
    -0x1.88e7a6bf1b5b97fdbf4dd5e05467c25dp-6,
    0x1.f3de268394e8dp-1
  },
  { // Entry 132
    -0x1.84e362ad217017fe7ee6c5e9580aa1ddp-6,
    0x1.f3fd86b392884p-1
  },
  { // Entry 133
    -0x1.744eff6d77dde7d2a179669e225699e3p-6,
    0x1.f47f1e1f53f24p-1
  },
  { // Entry 134
    -0x1.68b4f86497a0a7fe24c5fba78bf17a88p0,
    0x1.f48p-3
  },
  { // Entry 135
    -0x1.6f51e68fcd7cd7fa1e6acd9f4b979341p-6,
    0x1.f4a622df760c9p-1
  },
  { // Entry 136
    -0x1.6bab4f93a37207d2376593e12e890d80p-6,
    0x1.f4c2b2a5a5c1dp-1
  },
  { // Entry 137
    -0x1.65ae4deeda7b77fe9c5dac2c0c0dedf4p-6,
    0x1.f4f18fab5c3b8p-1
  },
  { // Entry 138
    -0x1.4c2a64cb22441fe74e2f868934e9e491p-6,
    0x1.f5b96e5b96eb7p-1
  },
  { // Entry 139
    -0x1.43ae6c44b9f767f34ebf8a2cc3744c94p-6,
    0x1.f5fbf5cedc480p-1
  },
  { // Entry 140
    -0x1.41f7812310c9b0015ca3978caea2af49p-6,
    0x1.f60968a5952d9p-1
  },
  { // Entry 141
    -0x1.3b0cb0b1469db800b20aa116988cb6f2p-6,
    0x1.f63fae6bae321p-1
  },
  { // Entry 142
    -0x1.311e67575b4f5006994df3158c7316ccp-6,
    0x1.f68da368da368p-1
  },
  { // Entry 143
    -0x1.020da703f2f9d7d88c2361f9312c8382p-6,
    0x1.f7ffbefc5d9d8p-1
  },
  { // Entry 144
    -0x1.0205658935b947ec5874530468145ae7p-6,
    0x1.f7fffffffffe6p-1
  },
  { // Entry 145
    -0x1.ae825fe2eb6707f96a6e4c1dd535f9ccp-7,
    0x1.f95139c561139p-1
  },
  { // Entry 146
    -0x1.23851c80dddf27fb847cdd237db32e86p-7,
    0x1.fb77177fb5d35p-1
  },
  { // Entry 147
    -0x1.7c498eb87ed687f5ab6d00e39946d3bfp-8,
    0x1.fd09a0b5b17f0p-1
  },
  { // Entry 148
    -0x1.50dd41aeb4865700c68f5df4a8438e0dp-8,
    0x1.fd6p-1
  },
  { // Entry 149
    -0x1.08b8bba5db3a85b4e252ed1003e3481fp-8,
    0x1.fdef9fe7f9fe3p-1
  },
  { // Entry 150
    -0x1.07c989add9f585e65c78fe912c1a8cp-8,
    0x1.fdf17c5f17cp-1
  },
  { // Entry 151
    -0x1.008055958e10b542f640a94ed45f9bc0p-8,
    0x1.fdfffffffff55p-1
  },
  { // Entry 152
    -0x1.1f3e47178f97c75334a62a2f7aa66bc5p-9,
    0x1.fee1123d05c1bp-1
  },
  { // Entry 153
    -0x1.ff7faa9ab136778a743baa4f52803c4ep-10,
    0x1.ff007ffffffffp-1
  },
  { // Entry 154
    -0x1.ff39ecbe26759778f8bbf67f1e4a7e0ap-10,
    0x1.ff00a2cd88b55p-1
  },
  { // Entry 155
    -0x1.6363477698d257fc17522b50ff55553cp0,
    0x1.ff020ffffffffp-3
  },
  { // Entry 156
    -0x1.f7a248bf22fa97facadb946630e3967cp-10,
    0x1.ff046cbe6cbddp-1
  },
  { // Entry 157
    -0x1.dae60fee9fa9f7e341fe9059ccf60974p-10,
    0x1.ff12c3ff12c30p-1
  },
  { // Entry 158
    -0x1.d208bc8587776791c38a76a675c1ea45p-10,
    0x1.ff17309fefcd2p-1
  },
  { // Entry 159
    -0x1.babff263eabf87ac216ead65574acf01p-12,
    0x1.ffc8aaff534d4p-1
  },
  { // Entry 160
    -0x1.c00c40725b0617de17d7b74e49091d74p-13,
    0x1.ffe3fffffffffp-1
  },
  { // Entry 161
    -0x1.78bfa5004153173f2f5226cd23948518p-13,
    0x1.ffe874904abd0p-1
  },
  { // Entry 162
    -0x1.1148dff957e7c74564229e9cb9c4b83cp-13,
    0x1.ffeeebbaeebb8p-1
  },
  { // Entry 163
    -0x1.4aa196aae1ef97ff1730897799ee4f02p8,
    0x1.fffffbbffffffp-478
  },
  { // Entry 164
    0x1.8e8f43d38040fffeda732c8d164c1eb5p8,
    0x1.fffffbbffffffp574
  },
  { // Entry 165
    -0x1.00000401000008015565655755957558p-31,
    0x1.fffffffbfffffp-1
  },
  { // Entry 166
    -0x1.ff8040007fc027f82c8ac2a212ef8e0ep-34,
    0x1.ffffffff003fep-1
  },
  { // Entry 167
    0x1.86ef5ccdfa1b17fe78c886a9d8b2faaep7,
    0x1.ffffffffddfffp281
  },
  { // Entry 168
    -0x1.00080000008008002000555d55955640p-40,
    0x1.fffffffffdfffp-1
  },
  { // Entry 169
    -0x1.00200000002008008000055755955801p-42,
    0x1.ffffffffff7ffp-1
  },
  { // Entry 170
    -0x1.00400000001008010000015655955aaap-43,
    0x1.ffffffffffbffp-1
  },
  { // Entry 171
    0x1.30fc1931f09c97ff42ff5cad467897fdp7,
    0x1.fffffffffffeep219
  },
  { // Entry 172
    -0x1.00000000000010000000000001555555p-51,
    0x1.ffffffffffffcp-1
  },
  { // Entry 173
    -0x1.00000000000008000000000000555555p-52,
    0x1.ffffffffffffep-1
  },
  { // Entry 174
    -0x1.03fe55a061c757fed6a1da317da50b1ap-1,
    0x1.342185798f6d6p-1
  },
  { // Entry 175
    -0x1.62e42fefa39f0ce4e0d5078578892721p-2,
    0x1.6a09e667f3bccp-1
  },
  { // Entry 176
    -0x1.da391c9043a0f16abe056e384bf07f29p-3,
    0x1.962b5f9438d25p-1
  },
  { // Entry 177
    -0x1.06fbec52082bd253e20210b58868e521p-3,
    0x1.c24cd8c07de7ep-1
  },
  { // Entry 178
    -0x1.1e0a8f670c50ad172b2608e4a78b5e60p-5,
    0x1.ee6e51ecc2fd7p-1
  },
  { // Entry 179
    0x1.9e548e7e3dde7b0f34385f075e850106p-5,
    0x1.0d47e58c84098p0
  },
  { // Entry 180
    0x1.08e0bae73ac238d5d0d1fd9a04579630p-3,
    0x1.2358a222a6944p0
  },
  { // Entry 181
    0x1.9e6462187c36a8ddeed899c4e3596896p-3,
    0x1.39695eb8c91f0p0
  },
  { // Entry 182
    0x1.14dd6b0af939ac9fd1cf8d8a3a630a07p-2,
    0x1.4f7a1b4eeba9cp0
  },
  { // Entry 183
    0x1.56181f19d7198035cf7c5df02d166026p-2,
    0x1.658ad7e50e348p0
  },
  { // Entry 184
    0x1.936a82fadcd85a768a1655a807a5c5fdp-2,
    0x1.7b9b947b30bf4p0
  },
  { // Entry 185
    0x1.cd45b0a9f25021e4c6ab876e541defc9p-2,
    0x1.91ac5111534a0p0
  },
  { // Entry 186
    0x1.02044c9b70627350f5c613ba3eff7f14p-1,
    0x1.a7bd0da775d4cp0
  },
  { // Entry 187
    0x1.1c01dad59e72fa047b61ac26c51f4c17p-1,
    0x1.bdcdca3d985f8p0
  },
  { // Entry 188
    0x1.34bde9b821fe31136e67453dfecf9e34p-1,
    0x1.d3de86d3baea4p0
  },
  { // Entry 189
    0x1.4c561dbbb21d9a8faab44f6777f929f7p-1,
    0x1.e9ef4369dd750p0
  },
  { // Entry 190
    0x1.62e42fefa39eb35793c7673003e5ed5ep-1,
    0x1.ffffffffffffcp0
  },
  { // Entry 191
    -0x1.62e42fefa39f0ce4e0d5078578892721p-2,
    0x1.6a09e667f3bccp-1
  },
  { // Entry 192
    -0x1.24cfce6f80d9b57bf711e8c7ba21def9p-2,
    0x1.80aa84ce72f89p-1
  },
  { // Entry 193
    -0x1.d490246defa6c24070421157c8bdbac4p-3,
    0x1.974b2334f2346p-1
  },
  { // Entry 194
    -0x1.65d558d4ce00a4188793f7452a41c9d3p-3,
    0x1.adebc19b71703p-1
  },
  { // Entry 195
    -0x1.f991c6cb3b376d3e05c0449d9978b9bbp-4,
    0x1.c48c6001f0ac0p-1
  },
  { // Entry 196
    -0x1.31b994d3a4f80646146a3561e1668213p-4,
    0x1.db2cfe686fe7dp-1
  },
  { // Entry 197
    -0x1.ccb73cdddb2b3b5b841eb150a3d5e34dp-6,
    0x1.f1cd9cceef23ap-1
  },
  { // Entry 198
    0x1.0b94f7c19617485bcb4375b0b82ed7aap-6,
    0x1.04371d9ab72fbp0
  },
  { // Entry 199
    0x1.e27076e2af2d92344012fb7464c787c6p-5,
    0x1.0f876ccdf6cd9p0
  },
  { // Entry 200
    0x1.986d3228180be0924ac3a19b72870e79p-4,
    0x1.1ad7bc01366b7p0
  },
  { // Entry 201
    0x1.1c898c16999f2915ad51a005b68ddcc4p-3,
    0x1.26280b3476095p0
  },
  { // Entry 202
    0x1.69d4592a03621b2e6cccc2036f3931ffp-3,
    0x1.31785a67b5a73p0
  },
  { // Entry 203
    0x1.b44f77bcc8f55a888449bc53e9d9bf3ep-3,
    0x1.3cc8a99af5451p0
  },
  { // Entry 204
    0x1.fc2d6cf47cf0b2e0b88f48832402408cp-3,
    0x1.4818f8ce34e2fp0
  },
  { // Entry 205
    0x1.20cdcd192ab6530b24d75ab4459e3bc3p-2,
    0x1.536948017480dp0
  },
  { // Entry 206
    0x1.426174dbd515d649d69984816e68940dp-2,
    0x1.5eb99734b41ebp0
  },
  { // Entry 207
    0x1.62e42fefa39e52069052cb73bf7c2d5fp-2,
    0x1.6a09e667f3bc9p0
  },
  { // Entry 208
    -0x1.269621134db92783beb7676c0aa9c2a3p-2,
    0x1.8p-1
  },
  { // Entry 209
    -0x1.89fa465cb93c39302d7a43bf8a8345c8p-3,
    0x1.a666666666666p-1
  },
  { // Entry 210
    -0x1.af8e8210a41648ad2ef5a75c80ae9b02p-4,
    0x1.cccccccccccccp-1
  },
  { // Entry 211
    -0x1.9ece955321b91119b64b048613dbc07dp-6,
    0x1.f333333333332p-1
  },
  { // Entry 212
    0x1.8fb063ef2c7d10ac8c5ce361f3a59a73p-5,
    0x1.0ccccccccccccp0
  },
  { // Entry 213
    0x1.e27076e2af2d7b106f9c6fe70000f0b8p-4,
    0x1.1ffffffffffffp0
  },
  { // Entry 214
    0x1.7565011e4966e394742cdeb80aaa0b85p-3,
    0x1.3333333333332p0
  },
  { // Entry 215
    0x1.f18dc41e8ef1708ddac617e72a16a351p-3,
    0x1.4666666666665p0
  },
  { // Entry 216
    0x1.334e9e47d07efd97d13fea265dcec2d1p-2,
    0x1.5999999999998p0
  },
  { // Entry 217
    0x1.6aac04146151ff3644f0da8793fe911fp-2,
    0x1.6cccccccccccbp0
  },
  { // Entry 218
    0x1.9f323ecbf98469d61382119eac3e89e1p-2,
    0x1.7fffffffffffep0
  },
  { // Entry 219
    0.0,
    0x1.0p0
  },
  { // Entry 220
    0x1.8663f793c46cc6d5699b82cc3bd3be38p-4,
    0x1.199999999999ap0
  },
  { // Entry 221
    0x1.7565011e4967b8e9c982340d62384469p-3,
    0x1.3333333333334p0
  },
  { // Entry 222
    0x1.0ca937be1b9df8e84ab52d4b0aefd012p-2,
    0x1.4cccccccccccep0
  },
  { // Entry 223
    0x1.588c2d9133494380eebceb76c1928f7cp-2,
    0x1.6666666666668p0
  },
  { // Entry 224
    0x1.9f323ecbf9851480be2cbc4956e9348bp-2,
    0x1.8000000000002p0
  },
  { // Entry 225
    0x1.e148a1a2726d394df8cdd6c80d7ec865p-2,
    0x1.999999999999cp0
  },
  { // Entry 226
    0x1.0fae81914a9947bd2de51187a6308fdbp-1,
    0x1.b333333333336p0
  },
  { // Entry 227
    0x1.2cf25fad8f1c715e5fafceb63bf60707p-1,
    0x1.cccccccccccd0p0
  },
  { // Entry 228
    0x1.48a11293d785f86a7b177f4aa4e9cadbp-1,
    0x1.e66666666666ap0
  },
  { // Entry 229
    0x1.62e42fefa39ef35793c7673007e5ed5ep-1,
    0x1.0p1
  },
  { // Entry 230
    0x1.1542457337d42e1c6b73c89d862ba171p6,
    0x1.0p100
  },
  { // Entry 231
    0x1.15a3de711cc5494e20ce2f7e393a9661p6,
    0x1.199999999999ap100
  },
  { // Entry 232
    0x1.15fcf7f3c6f8e1f8e05889b78cdcbd94p6,
    0x1.3333333333334p100
  },
  { // Entry 233
    0x1.164eeeaaf5efcc1553be7dcad1369141p6,
    0x1.4cccccccccccep100
  },
  { // Entry 234
    0x1.169ad1a0c907775fec628588fced3401p6,
    0x1.6666666666668p100
  },
  { // Entry 235
    0x1.16e177b203cdb330ec31f559cf828aa6p6,
    0x1.8000000000002p100
  },
  { // Entry 236
    0x1.17238e14da469b55b96c96744e39203ap6,
    0x1.999999999999cp100
  },
  { // Entry 237
    0x1.1761a2765a6960abe5cf92c095780291p6,
    0x1.b333333333336p100
  },
  { // Entry 238
    0x1.179c2a3292f266ff2833283af2a38d7fp6,
    0x1.cccccccccccd0p100
  },
  { // Entry 239
    0x1.17d387985f833a0d4069f79c1b757507p6,
    0x1.e66666666666ap100
  },
  { // Entry 240
    0x1.18080dd3171b6c031a9b576be63b6d4cp6,
    0x1.0p101
  },
  { // Entry 241
    0x1.1542457337d42e1c6b73c89d862ba171p7,
    0x1.0p200
  },
  { // Entry 242
    0x1.157311f22a4cbbb54620fc0ddfb31be9p7,
    0x1.199999999999ap200
  },
  { // Entry 243
    0x1.159f9eb37f66880aa5e6292a89842f82p7,
    0x1.3333333333334p200
  },
  { // Entry 244
    0x1.15c89a0f16e1fd18df9923342bb11959p7,
    0x1.4cccccccccccep200
  },
  { // Entry 245
    0x1.15ee8b8a006dd2be2beb2713418c6ab9p7,
    0x1.6666666666668p200
  },
  { // Entry 246
    0x1.1611de929dd0f0a6abd2defbaad7160cp7,
    0x1.8000000000002p200
  },
  { // Entry 247
    0x1.1632e9c4090d64b912702f88ea3260d6p7,
    0x1.999999999999cp200
  },
  { // Entry 248
    0x1.1651f3f4c91ec76428a1adaf0dd1d201p7,
    0x1.b333333333336p200
  },
  { // Entry 249
    0x1.166f37d2e5634a8dc9d3786c3c679778p7,
    0x1.cccccccccccd0p200
  },
  { // Entry 250
    0x1.168ae685cbabb414d5eee01cd0d08b3cp7,
    0x1.e66666666666ap200
  },
  { // Entry 251
    0x1.16a529a32777cd0fc3079004b633875fp7,
    0x1.0p201
  },
  { // Entry 252
    0x1.5a92d6d005c939a38650bac4e7b689cep9,
    0x1.0p1000
  },
  { // Entry 253
    -0x1.0000080000555559555588888b333357p-20,
    0x1.ffffep-1
  },
  { // Entry 254
    -0x1.0000040000155555d5555888889ddddep-21,
    0x1.fffffp-1
  },
  { // Entry 255
    0.0,
    0x1.0p0
  },
  { // Entry 256
    0x1.fffff800002aaaa9aaaab11110e66667p-22,
    0x1.0000080p0
  },
  { // Entry 257
    0x1.fffff00000aaaaa2aaab11110bbbbc04p-21,
    0x1.00001p0
  },
  { // Entry 258
    -0x1.00000002000000055555556555555588p-30,
    0x1.fffffff80p-1
  },
  { // Entry 259
    -0x1.00000001000000015555555755555558p-31,
    0x1.fffffffc0p-1
  },
  { // Entry 260
    0.0,
    0x1.0p0
  },
  { // Entry 261
    0x1.fffffffe00000002aaaaaaa6aaaaaab1p-32,
    0x1.000000020p0
  },
  { // Entry 262
    0x1.fffffffc0000000aaaaaaa8aaaaaab11p-31,
    0x1.000000040p0
  },
  { // Entry 263
    -0x1.00000000008000000000555555555595p-40,
    0x1.fffffffffe0p-1
  },
  { // Entry 264
    -0x1.0000000000400000000015555555555dp-41,
    0x1.ffffffffff0p-1
  },
  { // Entry 265
    0.0,
    0x1.0p0
  },
  { // Entry 266
    0x1.ffffffffff80000000002aaaaaaaaa9ap-42,
    0x1.00000000008p0
  },
  { // Entry 267
    0x1.ffffffffff0000000000aaaaaaaaaa2ap-41,
    0x1.00000000010p0
  },
  { // Entry 268
    -0x1.00000000000020000000000005555555p-50,
    0x1.ffffffffffff8p-1
  },
  { // Entry 269
    -0x1.00000000000010000000000001555555p-51,
    0x1.ffffffffffffcp-1
  },
  { // Entry 270
    0.0,
    0x1.0p0
  },
  { // Entry 271
    0x1.ffffffffffffe0000000000002aaaaaap-52,
    0x1.0000000000002p0
  },
  { // Entry 272
    0x1.ffffffffffffc000000000000aaaaaaap-51,
    0x1.0000000000004p0
  },
  { // Entry 273
    0x1.62e42fefa39ef35393c7673007e5dd5ep9,
    0x1.fffffffffffffp1023
  },
  { // Entry 274
    -0x1.74385446d71c36395a7ea3b9e048a775p9,
    0x1.0p-1074
  },
  { // Entry 275
    -0x1.62e42fefa39f3a261da205fd13cb53dfp-2,
    0x1.6a09e667f3bcbp-1
  },
  { // Entry 276
    -0x1.62e42fefa39f0ce4e0d5078578892721p-2,
    0x1.6a09e667f3bccp-1
  },
  { // Entry 277
    -0x1.62e42fefa39edfa3a408090ddf46fa62p-2,
    0x1.6a09e667f3bcdp-1
  },
  { // Entry 278
    0x1.62e42fefa39eac8909ecc862fc0086ddp-2,
    0x1.6a09e667f3bcbp0
  },
  { // Entry 279
    0x1.62e42fefa39ed9ca46b9c6da9742b39bp-2,
    0x1.6a09e667f3bccp0
  },
  { // Entry 280
    0x1.62e42fefa39f070b8386c5523084e05ap-2,
    0x1.6a09e667f3bcdp0
  },
  { // Entry 281
    -0x1.62e42fefa39f035793c767300825ed5ep-1,
    0x1.fffffffffffffp-2
  },
  { // Entry 282
    -0x1.62e42fefa39ef35793c7673007e5ed5ep-1,
    0x1.0p-1
  },
  { // Entry 283
    -0x1.62e42fefa39ed35793c7673008e5ed5ep-1,
    0x1.0000000000001p-1
  },
  { // Entry 284
    -0x1.269621134db9522e69621216b637fb86p-2,
    0x1.7ffffffffffffp-1
  },
  { // Entry 285
    -0x1.269621134db92783beb7676c0aa9c2a3p-2,
    0x1.8p-1
  },
  { // Entry 286
    -0x1.269621134db8fcd9140cbcc160e2a631p-2,
    0x1.8000000000001p-1
  },
  { // Entry 287
    0x1.9f323ecbf9849480be2cbc495993df36p-2,
    0x1.7ffffffffffffp0
  },
  { // Entry 288
    0x1.9f323ecbf984bf2b68d766f405221819p-2,
    0x1.8p0
  },
  { // Entry 289
    0x1.9f323ecbf984e9d61382119eaee9348bp-2,
    0x1.8000000000001p0
  },
  { // Entry 290
    0x1.54e3c0b10a36434abac277160311ccb4p-9,
    0x1.00aaaaaaaaaaap0
  },
  { // Entry 291
    0x1.54e3c0b10a562e0394dbbb9359f477ddp-9,
    0x1.00aaaaaaaaaabp0
  },
  { // Entry 292
    0x1.54e3c0b10a7618bc6ef5000eb37f2563p-9,
    0x1.00aaaaaaaaaacp0
  },
  { // Entry 293
    0x1.62e42fefa39eeb5793c7673007c5ed5ep0,
    0x1.fffffffffffffp1
  },
  { // Entry 294
    0x1.62e42fefa39ef35793c7673007e5ed5ep0,
    0x1.0p2
  },
  { // Entry 295
    0x1.62e42fefa39f035793c767300765ed5ep0,
    0x1.0000000000001p2
  },
  { // Entry 296
    0x1.62e42fefa39ee35793c7673007a5ed5ep-1,
    0x1.fffffffffffffp0
  },
  { // Entry 297
    0x1.62e42fefa39ef35793c7673007e5ed5ep-1,
    0x1.0p1
  },
  { // Entry 298
    0x1.62e42fefa39f135793c7673006e5ed5ep-1,
    0x1.0000000000001p1
  },
  { // Entry 299
    -0x1.00000000000004000000000000155555p-53,
    0x1.fffffffffffffp-1
  },
  { // Entry 300
    0.0,
    0x1.0p0
  },
  { // Entry 301
    0x1.fffffffffffff0000000000000aaaaaap-53,
    0x1.0000000000001p0
  },
  { // Entry 302
    -0x1.62e42fefa39f035793c767300825ed5ep-1,
    0x1.fffffffffffffp-2
  },
  { // Entry 303
    -0x1.62e42fefa39ef35793c7673007e5ed5ep-1,
    0x1.0p-1
  },
  { // Entry 304
    -0x1.62e42fefa39ed35793c7673008e5ed5ep-1,
    0x1.0000000000001p-1
  },
  { // Entry 305
    -0x1.62e42fefa39efb5793c767300805ed5ep0,
    0x1.fffffffffffffp-3
  },
  { // Entry 306
    -0x1.62e42fefa39ef35793c7673007e5ed5ep0,
    0x1.0p-2
  },
  { // Entry 307
    -0x1.62e42fefa39ee35793c767300865ed5ep0,
    0x1.0000000000001p-2
  },
  { // Entry 308
    -0x1.0a2b23f3bab73a81aed58d6405fc7206p1,
    0x1.fffffffffffffp-4
  },
  { // Entry 309
    -0x1.0a2b23f3bab73681aed58d6405ec7206p1,
    0x1.0p-3
  },
  { // Entry 310
    -0x1.0a2b23f3bab72e81aed58d64062c7206p1,
    0x1.0000000000001p-3
  },
  { // Entry 311
    -0x1.62e42fefa39ef75793c7673007f5ed5ep1,
    0x1.fffffffffffffp-5
  },
  { // Entry 312
    -0x1.62e42fefa39ef35793c7673007e5ed5ep1,
    0x1.0p-4
  },
  { // Entry 313
    -0x1.62e42fefa39eeb5793c767300825ed5ep1,
    0x1.0000000000001p-4
  },
  { // Entry 314
    -0x1.bb9d3beb8c86b42d78b940fc09ef68b6p1,
    0x1.fffffffffffffp-6
  },
  { // Entry 315
    -0x1.bb9d3beb8c86b02d78b940fc09df68b6p1,
    0x1.0p-5
  },
  { // Entry 316
    -0x1.bb9d3beb8c86a82d78b940fc0a1f68b6p1,
    0x1.0000000000001p-5
  },
  { // Entry 317
    -0x1.0a2b23f3bab73881aed58d6405f47206p2,
    0x1.fffffffffffffp-7
  },
  { // Entry 318
    -0x1.0a2b23f3bab73681aed58d6405ec7206p2,
    0x1.0p-6
  },
  { // Entry 319
    -0x1.0a2b23f3bab73281aed58d64060c7206p2,
    0x1.0000000000001p-6
  },
  { // Entry 320
    -0x1.3687a9f1af2b16eca14e7a4a06f12fb2p2,
    0x1.fffffffffffffp-8
  },
  { // Entry 321
    -0x1.3687a9f1af2b14eca14e7a4a06e92fb2p2,
    0x1.0p-7
  },
  { // Entry 322
    -0x1.3687a9f1af2b10eca14e7a4a07092fb2p2,
    0x1.0000000000001p-7
  },
  { // Entry 323
    -0x1.62e42fefa39ef55793c7673007eded5ep2,
    0x1.fffffffffffffp-9
  },
  { // Entry 324
    -0x1.62e42fefa39ef35793c7673007e5ed5ep2,
    0x1.0p-8
  },
  { // Entry 325
    -0x1.62e42fefa39eef5793c767300805ed5ep2,
    0x1.0000000000001p-8
  },
  { // Entry 326
    -0x1.8f40b5ed9812d3c28640541608eaab0ap2,
    0x1.fffffffffffffp-10
  },
  { // Entry 327
    -0x1.8f40b5ed9812d1c28640541608e2ab0ap2,
    0x1.0p-9
  },
  { // Entry 328
    -0x1.8f40b5ed9812cdc2864054160902ab0ap2,
    0x1.0000000000001p-9
  },
  { // Entry 329
    -0x1.bb9d3beb8c86b22d78b940fc09e768b6p2,
    0x1.fffffffffffffp-11
  },
  { // Entry 330
    -0x1.bb9d3beb8c86b02d78b940fc09df68b6p2,
    0x1.0p-10
  },
  { // Entry 331
    -0x1.bb9d3beb8c86ac2d78b940fc09ff68b6p2,
    0x1.0000000000001p-10
  },
  { // Entry 332
    -0x1.205966f2b4f126b7281203d7066ed0dcp3,
    0x1.fffffffffffffp-14
  },
  { // Entry 333
    -0x1.205966f2b4f125b7281203d7066ad0dcp3,
    0x1.0p-13
  },
  { // Entry 334
    -0x1.205966f2b4f123b7281203d7067ad0dcp3,
    0x1.0000000000001p-13
  },
  { // Entry 335
    -0x1.205966f2b4f126b7281203d7066ed0dcp3,
    0x1.fffffffffffffp-14
  },
  { // Entry 336
    -0x1.205966f2b4f125b7281203d7066ad0dcp3,
    0x1.0p-13
  },
  { // Entry 337
    -0x1.205966f2b4f123b7281203d7067ad0dcp3,
    0x1.0000000000001p-13
  },
  { // Entry 338
    -0x1.0a2b23f3bab73a81aed58d6405fc7206p1,
    0x1.fffffffffffffp-4
  },
  { // Entry 339
    -0x1.0a2b23f3bab73681aed58d6405ec7206p1,
    0x1.0p-3
  },
  { // Entry 340
    -0x1.0a2b23f3bab72e81aed58d64062c7206p1,
    0x1.0000000000001p-3
  },
  { // Entry 341
    -0x1.1178e8227e480707cafd4459785b0e8fp-3,
    0x1.bffffffffffffp-1
  },
  { // Entry 342
    -0x1.1178e8227e47bde338b41fc72de81e3bp-3,
    0x1.cp-1
  },
  { // Entry 343
    -0x1.1178e8227e4774bea66afb34e611e9fdp-3,
    0x1.c000000000001p-1
  },
  { // Entry 344
    -0x1.62e42fefa39ef75793c7673007f5ed5ep1,
    0x1.fffffffffffffp-5
  },
  { // Entry 345
    -0x1.62e42fefa39ef35793c7673007e5ed5ep1,
    0x1.0p-4
  },
  { // Entry 346
    -0x1.62e42fefa39eeb5793c767300825ed5ep1,
    0x1.0000000000001p-4
  },
  { // Entry 347
    -0x1.08598b59e3a0f112c86247d8be41d487p-4,
    0x1.dffffffffffffp-1
  },
  { // Entry 348
    -0x1.08598b59e3a0688a3fd9bf503372c12fp-4,
    0x1.ep-1
  },
  { // Entry 349
    -0x1.08598b59e39fe001b75136c7ad30c376p-4,
    0x1.e000000000001p-1
  },
  { // Entry 350
    -0x1.bb9d3beb8c86b42d78b940fc09ef68b6p1,
    0x1.fffffffffffffp-6
  },
  { // Entry 351
    -0x1.bb9d3beb8c86b02d78b940fc09df68b6p1,
    0x1.0p-5
  },
  { // Entry 352
    -0x1.bb9d3beb8c86a82d78b940fc0a1f68b6p1,
    0x1.0000000000001p-5
  },
  { // Entry 353
    -0x1.0415d89e74454f43844b7e55cedd837bp-5,
    0x1.effffffffffffp-1
  },
  { // Entry 354
    -0x1.0415d89e7444470173c75d4d8889de0ep-5,
    0x1.fp-1
  },
  { // Entry 355
    -0x1.0415d89e74433ebf63433c454abc7b39p-5,
    0x1.f000000000001p-1
  },
  { // Entry 356
    -0x1.0a2b23f3bab73881aed58d6405f47206p2,
    0x1.fffffffffffffp-7
  },
  { // Entry 357
    -0x1.0a2b23f3bab73681aed58d6405ec7206p2,
    0x1.0p-6
  },
  { // Entry 358
    -0x1.0a2b23f3bab73281aed58d64060c7206p2,
    0x1.0000000000001p-6
  },
  { // Entry 359
    -0x1.0205658935867cbfa5a9263bd81da4a0p-6,
    0x1.f7fffffffffffp-1
  },
  { // Entry 360
    -0x1.020565893584749f23a105b9c7bb9a6fp-6,
    0x1.f80p-1
  },
  { // Entry 361
    -0x1.0205658935826c7ea198e537c7dca08fp-6,
    0x1.f800000000001p-1
  },
  { // Entry 362
    -0x1.3687a9f1af2b16eca14e7a4a06f12fb2p2,
    0x1.fffffffffffffp-8
  },
  { // Entry 363
    -0x1.3687a9f1af2b14eca14e7a4a06e92fb2p2,
    0x1.0p-7
  },
  { // Entry 364
    -0x1.3687a9f1af2b10eca14e7a4a07092fb2p2,
    0x1.0000000000001p-7
  },
  { // Entry 365
    -0x1.010157588deb1a9cec9b03fbee49c6b0p-7,
    0x1.fbfffffffffffp-1
  },
  { // Entry 366
    -0x1.010157588de7128ccc5a82f9da00f48bp-7,
    0x1.fc0p-1
  },
  { // Entry 367
    -0x1.010157588de30a7cac1a01f7e639a670p-7,
    0x1.fc00000000001p-1
  },
  { // Entry 368
    -0x1.62e42fefa39ef55793c7673007eded5ep2,
    0x1.fffffffffffffp-9
  },
  { // Entry 369
    -0x1.62e42fefa39ef35793c7673007e5ed5ep2,
    0x1.0p-8
  },
  { // Entry 370
    -0x1.62e42fefa39eef5793c767300805ed5ep2,
    0x1.0000000000001p-8
  },
  { // Entry 371
    -0x1.0080559588bb5feda0eb4595c5fbe2b1p-8,
    0x1.fdfffffffffffp-1
  },
  { // Entry 372
    -0x1.0080559588b357e598e33d8d9db37a29p-8,
    0x1.fe0p-1
  },
  { // Entry 373
    -0x1.0080559588ab4fdd90db3585b5ebd2a1p-8,
    0x1.fe00000000001p-1
  },
  { // Entry 374
    -0x1.8f40b5ed9812d3c28640541608eaab0ap2,
    0x1.fffffffffffffp-10
  },
  { // Entry 375
    -0x1.8f40b5ed9812d1c28640541608e2ab0ap2,
    0x1.0p-9
  },
  { // Entry 376
    -0x1.8f40b5ed9812cdc2864054160902ab0ap2,
    0x1.0000000000001p-9
  },
  { // Entry 377
    -0x1.0040155d5899e674691fef41402f3e7bp-9,
    0x1.fefffffffffffp-1
  },
  { // Entry 378
    -0x1.0040155d5889de70671eeec0bfcefe53p-9,
    0x1.ff0p-1
  },
  { // Entry 379
    -0x1.0040155d5879d66c651dee40bfef1e6bp-9,
    0x1.ff00000000001p-1
  },
  { // Entry 380
    -0x1.bb9d3beb8c86b22d78b940fc09e768b6p2,
    0x1.fffffffffffffp-11
  },
  { // Entry 381
    -0x1.bb9d3beb8c86b02d78b940fc09df68b6p2,
    0x1.0p-10
  },
  { // Entry 382
    -0x1.bb9d3beb8c86ac2d78b940fc09ff68b6p2,
    0x1.0000000000001p-10
  },
  { // Entry 383
    -0x1.0020055655a89b377d5801fd08fd9f2dp-10,
    0x1.ff7ffffffffffp-1
  },
  { // Entry 384
    -0x1.00200556558893357cd7e1f486bd0705p-10,
    0x1.ff8p-1
  },
  { // Entry 385
    -0x1.0020055655688b337c57c1ed04fc9eedp-10,
    0x1.ff80000000001p-1
  },
  { // Entry 386
    -0x1.205966f2b4f126b7281203d7066ed0dcp3,
    0x1.fffffffffffffp-14
  },
  { // Entry 387
    -0x1.205966f2b4f125b7281203d7066ad0dcp3,
    0x1.0p-13
  },
  { // Entry 388
    -0x1.205966f2b4f123b7281203d7067ad0dcp3,
    0x1.0000000000001p-13
  },
  { // Entry 389
    -0x1.0004001556d56088dde0703f034ca096p-13,
    0x1.ffeffffffffffp-1
  },
  { // Entry 390
    -0x1.0004001555d558889dde702b028c9996p-13,
    0x1.fffp-1
  },
  { // Entry 391
    -0x1.0004001554d550885ddc701f024c9896p-13,
    0x1.fff0000000001p-1
  },
  { // Entry 392
    HUGE_VAL,
    HUGE_VAL
  },
  { // Entry 393
    0x1.62e42fefa39ef35393c7673007e5dd5ep9,
    0x1.fffffffffffffp1023
  },
  { // Entry 394
    0x1.62e42fefa39ef34f93c7673007e5ad5ep9,
    0x1.ffffffffffffep1023
  },
  { // Entry 395
    0x1.250d048e7a1bcdee499fa5b0ca1dd243p0,
    0x1.921fb54442d18p1
  },
  { // Entry 396
    0x1.ce6bb25aa1315109feefc86318ab6e52p-2,
    0x1.921fb54442d18p0
  },
  { // Entry 397
    0x1.fffffffffffff0000000000000aaaaaap-53,
    0x1.0000000000001p0
  },
  { // Entry 398
    0.0,
    0x1.0p0
  },
  { // Entry 399
    -0x1.00000000000004000000000000155555p-53,
    0x1.fffffffffffffp-1
  },
  { // Entry 400
    -0x1.eeb95b094c192b4a513e0bf9ee40d8d4p-3,
    0x1.921fb54442d18p-1
  },
  { // Entry 401
    -0x1.6232bdd7abcd23d5e7fd837c6fe23a67p9,
    0x1.0000000000001p-1022
  },
  { // Entry 402
    -0x1.6232bdd7abcd23dde7fd837c6fe1fa67p9,
    0x1.0p-1022
  },
  { // Entry 403
    -0x1.6232bdd7abcd23e5e7fd837c6fe23a67p9,
    0x1.ffffffffffffep-1023
  },
  { // Entry 404
    -0x1.6232bdd7abcd23ede7fd837c6fe2fa67p9,
    0x1.ffffffffffffcp-1023
  },
  { // Entry 405
    -0x1.73df9b3adb334e7c8499b1e01446adfap9,
    0x1.0p-1073
  },
  { // Entry 406
    -0x1.74385446d71c36395a7ea3b9e048a775p9,
    0x1.0p-1074
  },
  { // Entry 407
    -HUGE_VAL,
    0.0
  },
  { // Entry 408
    -HUGE_VAL,
    -0.0
  },
};
#endif // __BIONIC__

TEST(math_log, log_intel) {
#if defined(__BIONIC__)
  for (size_t i = 0; i < sizeof(g_log_intel_data)/sizeof(log_intel_data_t); i++) {
    EXPECT_DOUBLE_EQ(g_log_intel_data[i].expected, log(g_log_intel_data[i].call_data)) << "Failed on element " << i;
  }
#else // __BIONIC__
  GTEST_LOG_(INFO) << "This test does nothing.";
#endif // __BIONIC__
}
