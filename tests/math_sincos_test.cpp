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
  double sin_expected;
  double cos_expected;
  double call_data;
} sincos_intel_data_t;

static sincos_intel_data_t g_sincos_intel_data[] = {
  { // Entry 0
    -0x1.ce9a94ea9c2ad95597b1193b2300d19ap-1,
    -0x1.b6d3057776dc38335b16745f2d756ab6p-2,
    -0x1.01c000003p1,
  },
  { // Entry 1
    0x1.ce9a94ea9c2ad95597b1193b2300d19ap-1,
    -0x1.b6d3057776dc38335b16745f2d756ab6p-2,
    0x1.01c000003p1,
  },
  { // Entry 2
    -0x1.ce2cad2f92157153b4a9e012e3461d0ap-1,
    -0x1.b8a14c745bd4c832bae9785655c91b60p-2,
    -0x1.01fffc080p1,
  },
  { // Entry 3
    0x1.ce2cad2f92157153b4a9e012e3461d0ap-1,
    -0x1.b8a14c745bd4c832bae9785655c91b60p-2,
    0x1.01fffc080p1,
  },
  { // Entry 4
    -0x1.1a7444726f5e9dc2ee069dc3e500ab4fp-2,
    0x1.ec231802917bdffa627ab6a59abe3f7dp-1,
    -0x1.1e2a1563e068ep-2,
  },
  { // Entry 5
    0x1.1a7444726f5e9dc2ee069dc3e500ab4fp-2,
    0x1.ec231802917bdffa627ab6a59abe3f7dp-1,
    0x1.1e2a1563e068ep-2,
  },
  { // Entry 6
    -0x1.efdab5e65c8fd550d4688b62791fe694p-1,
    0x1.fe4123d266ca37faeee822138eb331d3p-3,
    -0x1.83ef2196f92f0p87,
  },
  { // Entry 7
    0x1.efdab5e65c8fd550d4688b62791fe694p-1,
    0x1.fe4123d266ca37faeee822138eb331d3p-3,
    0x1.83ef2196f92f0p87,
  },
  { // Entry 8
    -0x1.eff5edb1ad416cb6ca3109f1c0dfc34ap-1,
    0x1.fc9935a7481717fa8aeca7a9c5833084p-3,
    -0x1.a486d79764fb8p86,
  },
  { // Entry 9
    0x1.eff5edb1ad416cb6ca3109f1c0dfc34ap-1,
    0x1.fc9935a7481717fa8aeca7a9c5833084p-3,
    0x1.a486d79764fb8p86,
  },
  { // Entry 10
    -0x1.b78f2c97c88028396ec94ba9ea58dd68p-2,
    0x1.ce6dea6788fde425f68fe33b0ffcc244p-1,
    -0x1.c65173556ccfbp-2,
  },
  { // Entry 11
    0x1.b78f2c97c88028396ec94ba9ea58dd68p-2,
    0x1.ce6dea6788fde425f68fe33b0ffcc244p-1,
    0x1.c65173556ccfbp-2,
  },
  { // Entry 12
    -0x1.c7885aef33a94ffc5ae06be9444efad5p-3,
    0x1.f32c8792006349b33b09fe57f80d9ed1p-1,
    -0x1.cb6p-3,
  },
  { // Entry 13
    0x1.c7885aef33a94ffc5ae06be9444efad5p-3,
    0x1.f32c8792006349b33b09fe57f80d9ed1p-1,
    0x1.cb6p-3,
  },
  { // Entry 14
    -0x1.f74a97abb47bc823e92eb9d66f1d8b54p-3,
    0x1.f04c859e062b9202aa2b9bf0486b5afdp-1,
    -0x1.fc7fffffffffep-3,
  },
  { // Entry 15
    0x1.f74a97abb47bc823e92eb9d66f1d8b54p-3,
    0x1.f04c859e062b9202aa2b9bf0486b5afdp-1,
    0x1.fc7fffffffffep-3,
  },
  { // Entry 16
    0x1.0000000000002fffd555555555553d55p-32,
    0x1.fffffffffffffffeffffffffffffa0p-1,
    0x1.0000000000003p-32,
  },
  { // Entry 17
    -0x1.0000000000002fffd555555555553d55p-32,
    0x1.fffffffffffffffeffffffffffffa0p-1,
    -0x1.0000000000003p-32,
  },
  { // Entry 18
    0x1.d18f6ead199a3b95430d5516e93c8d7bp-1,
    -0x1.aa2265753e6687fde76269ee92a784b0p-2,
    0x1.00000000010p1,
  },
  { // Entry 19
    -0x1.d18f6ead199a3b95430d5516e93c8d7bp-1,
    -0x1.aa2265753e6687fde76269ee92a784b0p-2,
    -0x1.00000000010p1,
  },
  { // Entry 20
    0x1.b64d59dd8a5249e01113f4cb37d13c40p-1,
    -0x1.08a445ad4737e80060cf3a5ff94d3067p-1,
    0x1.001p557,
  },
  { // Entry 21
    -0x1.b64d59dd8a5249e01113f4cb37d13c40p-1,
    -0x1.08a445ad4737e80060cf3a5ff94d3067p-1,
    -0x1.001p557,
  },
  { // Entry 22
    0x1.ce77f24fd4d9a790125ff5290a62b7f1p-1,
    -0x1.b764f40c9716b834bb72589348cfa4b8p-2,
    0x1.01d4313757482p1,
  },
  { // Entry 23
    -0x1.ce77f24fd4d9a790125ff5290a62b7f1p-1,
    -0x1.b764f40c9716b834bb72589348cfa4b8p-2,
    -0x1.01d4313757482p1,
  },
  { // Entry 24
    0x1.ce39b7df7f4acb81e37c532638f9bf49p-1,
    -0x1.b86a908f05c0a839e19d4aa63156d32bp-2,
    0x1.01f867d44bc82p1,
  },
  { // Entry 25
    -0x1.ce39b7df7f4acb81e37c532638f9bf49p-1,
    -0x1.b86a908f05c0a839e19d4aa63156d32bp-2,
    -0x1.01f867d44bc82p1,
  },
  { // Entry 26
    0x1.ce70046acb80de75e903468cea8ab427p-1,
    -0x1.b78654766c76483ce90a0bc2ac957b59p-2,
    0x1.09860e8ed1e90p3,
  },
  { // Entry 27
    -0x1.ce70046acb80de75e903468cea8ab427p-1,
    -0x1.b78654766c76483ce90a0bc2ac957b59p-2,
    -0x1.09860e8ed1e90p3,
  },
  { // Entry 28
    0x1.ce68564e251f16285d604e04657f30e6p-1,
    -0x1.b7a6a56f5796683c58e01b5b7061c16cp-2,
    0x1.09872cce51fbdp3,
  },
  { // Entry 29
    -0x1.ce68564e251f16285d604e04657f30e6p-1,
    -0x1.b7a6a56f5796683c58e01b5b7061c16cp-2,
    -0x1.09872cce51fbdp3,
  },
  { // Entry 30
    -0x1.ce8ab32f3b002065f2d99e57a1072705p-1,
    -0x1.b715f769cf1e582e23a15e8b80a70486p-2,
    0x1.113b13b13b224p2,
  },
  { // Entry 31
    0x1.ce8ab32f3b002065f2d99e57a1072705p-1,
    -0x1.b715f769cf1e582e23a15e8b80a70486p-2,
    -0x1.113b13b13b224p2,
  },
  { // Entry 32
    0x1.c2df57188d3099e1baf7f721d7318bd9p-1,
    0x1.e536ae395dfce001457970c8aaac3b1fp-2,
    0x1.13cp0,
  },
  { // Entry 33
    -0x1.c2df57188d3099e1baf7f721d7318bd9p-1,
    0x1.e536ae395dfce001457970c8aaac3b1fp-2,
    -0x1.13cp0,
  },
  { // Entry 34
    0x1.b7dcd7c85c820838eecfa53e52078b2bp-2,
    -0x1.ce5b7372046eaa467f49e1debe3662dfp-1,
    0x1.1f628c5610717p3,
  },
  { // Entry 35
    -0x1.b7dcd7c85c820838eecfa53e52078b2bp-2,
    -0x1.ce5b7372046eaa467f49e1debe3662dfp-1,
    -0x1.1f628c5610717p3,
  },
  { // Entry 36
    0x1.ff3466b1ec8bed978f7c27018bc09678p-1,
    0x1.c86caa04929857c2816d30693fceab36p-5,
    0x1.1f699d708d497p16,
  },
  { // Entry 37
    -0x1.ff3466b1ec8bed978f7c27018bc09678p-1,
    0x1.c86caa04929857c2816d30693fceab36p-5,
    -0x1.1f699d708d497p16,
  },
  { // Entry 38
    0x1.b61d6aff754c2835697b5aa5339b635cp-2,
    -0x1.cec59b2d230a83a86804730afdf1becfp-1,
    0x1.1f72064620ef4p3,
  },
  { // Entry 39
    -0x1.b61d6aff754c2835697b5aa5339b635cp-2,
    -0x1.cec59b2d230a83a86804730afdf1becfp-1,
    -0x1.1f72064620ef4p3,
  },
  { // Entry 40
    0x1.cdf604838e499bd5d9d2712397b23c07p-1,
    0x1.b98656b85bc2683216a947335ea689fbp-2,
    0x1.1fffffdcefe40p0,
  },
  { // Entry 41
    -0x1.cdf604838e499bd5d9d2712397b23c07p-1,
    0x1.b98656b85bc2683216a947335ea689fbp-2,
    -0x1.1fffffdcefe40p0,
  },
  { // Entry 42
    0x1.ce913329696cd3fa471c7b00891075d4p-1,
    0x1.b6fa92e5f576c834fc15d8998fccc728p-2,
    0x1.20b478c4aa9edp0,
  },
  { // Entry 43
    -0x1.ce913329696cd3fa471c7b00891075d4p-1,
    0x1.b6fa92e5f576c834fc15d8998fccc728p-2,
    -0x1.20b478c4aa9edp0,
  },
  { // Entry 44
    0x1.ceabc025ed3d57323fca51626d2bf28cp-1,
    0x1.b68a988604a7e83cd382fdf329d0d76ep-2,
    0x1.20d37456e7453p0,
  },
  { // Entry 45
    -0x1.ceabc025ed3d57323fca51626d2bf28cp-1,
    0x1.b68a988604a7e83cd382fdf329d0d76ep-2,
    -0x1.20d37456e7453p0,
  },
  { // Entry 46
    0x1.ceb022b6b5ae07a267f0e7dc6a14a214p-1,
    0x1.b67816b80ed0a82d11aee7aaa8008fedp-2,
    0x1.20d8930cdf602p0,
  },
  { // Entry 47
    -0x1.ceb022b6b5ae07a267f0e7dc6a14a214p-1,
    0x1.b67816b80ed0a82d11aee7aaa8008fedp-2,
    -0x1.20d8930cdf602p0,
  },
  { // Entry 48
    -0x1.9e62aca53c660801b62604018a9d19ddp-4,
    -0x1.fd5f830f860f333de490a42c2f045012p-1,
    0x1.30d5f8e54b6d8p3,
  },
  { // Entry 49
    0x1.9e62aca53c660801b62604018a9d19ddp-4,
    -0x1.fd5f830f860f333de490a42c2f045012p-1,
    -0x1.30d5f8e54b6d8p3,
  },
  { // Entry 50
    0x1.3ed2aeefeafc97f0ee0fb3fa4fb46052p-3,
    0x1.f9c201e4eb65fd5e5dbd97662505ff6fp-1,
    0x1.402p-3,
  },
  { // Entry 51
    -0x1.3ed2aeefeafc97f0ee0fb3fa4fb46052p-3,
    0x1.f9c201e4eb65fd5e5dbd97662505ff6fp-1,
    -0x1.402p-3,
  },
  { // Entry 52
    -0x1.ff65d2ff4a8cc41cb8bb6df306e07be7p-1,
    -0x1.8d3822ef260a57b385611f08577b75d9p-5,
    0x1.4a40ec149a66fp16,
  },
  { // Entry 53
    0x1.ff65d2ff4a8cc41cb8bb6df306e07be7p-1,
    -0x1.8d3822ef260a57b385611f08577b75d9p-5,
    -0x1.4a40ec149a66fp16,
  },
  { // Entry 54
    0x1.4fffffffff9f88000000084f22ccccccp-20,
    0x1.fffffffffe47000000003f4ebffffffcp-1,
    0x1.5p-20,
  },
  { // Entry 55
    -0x1.4fffffffff9f88000000084f22ccccccp-20,
    0x1.fffffffffe47000000003f4ebffffffcp-1,
    -0x1.5p-20,
  },
  { // Entry 56
    0x1.79c599e1e91af809f9e69771796cd507p-1,
    -0x1.5997065cb9653702d4c9d9b6bc58f768p-1,
    0x1.5294a5294a528p4,
  },
  { // Entry 57
    -0x1.79c599e1e91af809f9e69771796cd507p-1,
    -0x1.5997065cb9653702d4c9d9b6bc58f768p-1,
    -0x1.5294a5294a528p4,
  },
  { // Entry 58
    -0x1.ff7996073bba6c6ede46f52d445623c9p-1,
    0x1.72e7437910cc083fac4f6f62a2eb38afp-5,
    0x1.57431aacf5c58p16,
  },
  { // Entry 59
    0x1.ff7996073bba6c6ede46f52d445623c9p-1,
    0x1.72e7437910cc083fac4f6f62a2eb38afp-5,
    -0x1.57431aacf5c58p16,
  },
  { // Entry 60
    0x1.f81c4f9a5181462ae735e21222d498c4p-1,
    0x1.6623d2eb6add1ffc398a3c20447f9d06p-3,
    0x1.652p0,
  },
  { // Entry 61
    -0x1.f81c4f9a5181462ae735e21222d498c4p-1,
    0x1.6623d2eb6add1ffc398a3c20447f9d06p-3,
    -0x1.652p0,
  },
  { // Entry 62
    -0x1.c42a091026f45286d061085c5c9fddb7p-1,
    0x1.e0619960a11c6801e80ab0c9e25f89d0p-2,
    0x1.6f7bdef7bdef4p3,
  },
  { // Entry 63
    0x1.c42a091026f45286d061085c5c9fddb7p-1,
    0x1.e0619960a11c6801e80ab0c9e25f89d0p-2,
    -0x1.6f7bdef7bdef4p3,
  },
  { // Entry 64
    -0x1.f9c4364ba198f7e32b672366c34b8b7dp-2,
    0x1.bd309f3dfcd489128e5ecbc31680c4a5p-1,
    0x1.711p2,
  },
  { // Entry 65
    0x1.f9c4364ba198f7e32b672366c34b8b7dp-2,
    0x1.bd309f3dfcd489128e5ecbc31680c4a5p-1,
    -0x1.711p2,
  },
  { // Entry 66
    -0x1.be6e5bea1a4d88331fd8e460cd677245p-2,
    0x1.ccc7d99b57ab54f04ed918ec14a2507dp-1,
    0x1.7540aa5882dc2p2,
  },
  { // Entry 67
    0x1.be6e5bea1a4d88331fd8e460cd677245p-2,
    0x1.ccc7d99b57ab54f04ed918ec14a2507dp-1,
    -0x1.7540aa5882dc2p2,
  },
  { // Entry 68
    0x1.c90c841d1494c0757e8ebb16725d8718p-3,
    -0x1.f3165a0b306b1ffcf8d11909fffba167p-1,
    0x1.7550d28ffccc4p1,
  },
  { // Entry 69
    -0x1.c90c841d1494c0757e8ebb16725d8718p-3,
    -0x1.f3165a0b306b1ffcf8d11909fffba167p-1,
    -0x1.7550d28ffccc4p1,
  },
  { // Entry 70
    -0x1.b649d577e1b2a839d25d19807eb2c564p-2,
    0x1.cebb175d36b934bc0995a0be35cde1eep-1,
    0x1.75d11fa0d6242p2,
  },
  { // Entry 71
    0x1.b649d577e1b2a839d25d19807eb2c564p-2,
    0x1.cebb175d36b934bc0995a0be35cde1eep-1,
    -0x1.75d11fa0d6242p2,
  },
  { // Entry 72
    0x1.b78730d11d8408320d21ca6ad2be3368p-2,
    -0x1.ce6fd00ed16501cb13b908477e102811p-1,
    0x1.bc50444ee6286p9,
  },
  { // Entry 73
    -0x1.b78730d11d8408320d21ca6ad2be3368p-2,
    -0x1.ce6fd00ed16501cb13b908477e102811p-1,
    -0x1.bc50444ee6286p9,
  },
  { // Entry 74
    0x1.b6b0b0996e7e6835acdb36e55a08bf15p-2,
    0x1.cea2b8cc552181d0b0aead27e94a9168p-1,
    0x1.c55b2bf19ce54p-2,
  },
  { // Entry 75
    -0x1.b6b0b0996e7e6835acdb36e55a08bf15p-2,
    0x1.cea2b8cc552181d0b0aead27e94a9168p-1,
    -0x1.c55b2bf19ce54p-2,
  },
  { // Entry 76
    0x1.b6facf665891482ea8c61f5ca32f280dp-2,
    0x1.ce9124cec4150559d947a526ad98f2f4p-1,
    0x1.c5ad34f5f472ap-2,
  },
  { // Entry 77
    -0x1.b6facf665891482ea8c61f5ca32f280dp-2,
    0x1.ce9124cec4150559d947a526ad98f2f4p-1,
    -0x1.c5ad34f5f472ap-2,
  },
  { // Entry 78
    -0x1.f83a0983dd15d00301e2df21e3bee635p-2,
    -0x1.bda0596df060004d579563ad8c67d151p-1,
    0x1.d4067c60f471ep1,
  },
  { // Entry 79
    0x1.f83a0983dd15d00301e2df21e3bee635p-2,
    -0x1.bda0596df060004d579563ad8c67d151p-1,
    -0x1.d4067c60f471ep1,
  },
  { // Entry 80
    0x1.9cb6a9bbce64a3e97a7267fdec25c83bp-1,
    0x1.2f011326420e5002172db245fd9063e2p-1,
    0x1.dffffffffffffp-1,
  },
  { // Entry 81
    -0x1.9cb6a9bbce64a3e97a7267fdec25c83bp-1,
    0x1.2f011326420e5002172db245fd9063e2p-1,
    -0x1.dffffffffffffp-1,
  },
  { // Entry 82
    0x1.f5f0be28565c5ad763c103d981fc5c4ep-5,
    0x1.ff09babb076e4803e57e68204570fd5bp-1,
    0x1.f64147d8add84p-5,
  },
  { // Entry 83
    -0x1.f5f0be28565c5ad763c103d981fc5c4ep-5,
    0x1.ff09babb076e4803e57e68204570fd5bp-1,
    -0x1.f64147d8add84p-5,
  },
  { // Entry 84
    -0x1.d4da5f56888e200fda4ebac7db1cdbefp-1,
    0x1.9b70cd3284e157fb84491d581cb86bd3p-2,
    0x1.fe6183efa397cp83,
  },
  { // Entry 85
    0x1.d4da5f56888e200fda4ebac7db1cdbefp-1,
    0x1.9b70cd3284e157fb84491d581cb86bd3p-2,
    -0x1.fe6183efa397cp83,
  },
  { // Entry 86
    0x1.fa9f6ca0ec44e0010026f385c0ab8690p-3,
    0x1.f016474b75667424a050d79014fd2385p-1,
    0x1.ffeffffffffffp-3,
  },
  { // Entry 87
    -0x1.fa9f6ca0ec44e0010026f385c0ab8690p-3,
    0x1.f016474b75667424a050d79014fd2385p-1,
    -0x1.ffeffffffffffp-3,
  },
  { // Entry 88
    -0x1.ff4868ddaba6ba32c6b714aef99ff2f7p-1,
    -0x1.b16f0eb25ae467c2a185e516f1188b20p-5,
    0x1.fff7ff800001fp15,
  },
  { // Entry 89
    0x1.ff4868ddaba6ba32c6b714aef99ff2f7p-1,
    -0x1.b16f0eb25ae467c2a185e516f1188b20p-5,
    -0x1.fff7ff800001fp15,
  },
  { // Entry 90
    -0x1.ff4f1e9c248912648701818d075b3953p-1,
    -0x1.a971e3b64d08d7c3f37d299b43616eb4p-5,
    0x1.fff7ffffffcp15,
  },
  { // Entry 91
    0x1.ff4f1e9c248912648701818d075b3953p-1,
    -0x1.a971e3b64d08d7c3f37d299b43616eb4p-5,
    -0x1.fff7ffffffcp15,
  },
  { // Entry 92
    0x1.d19616fc7ee4605345c25606cfc93235p-1,
    -0x1.aa054c4909384811a063273112604c31p-2,
    0x1.fff80p0,
  },
  { // Entry 93
    -0x1.d19616fc7ee4605345c25606cfc93235p-1,
    -0x1.aa054c4909384811a063273112604c31p-2,
    -0x1.fff80p0,
  },
  { // Entry 94
    0x1.ce3509751c4614837fa4b34963c6f5d8p-1,
    0x1.b87e37101654482144d71d04972267d8p-2,
    0x1.ffffbffe3ffffp14,
  },
  { // Entry 95
    -0x1.ce3509751c4614837fa4b34963c6f5d8p-1,
    0x1.b87e37101654482144d71d04972267d8p-2,
    -0x1.ffffbffe3ffffp14,
  },
  { // Entry 96
    0x1.d18f76ffc6e4ba0a3134e5be21b5bc8fp-1,
    -0x1.aa2241160227896c68ef17839f17dce5p-2,
    0x1.fffff60p0,
  },
  { // Entry 97
    -0x1.d18f76ffc6e4ba0a3134e5be21b5bc8fp-1,
    -0x1.aa2241160227896c68ef17839f17dce5p-2,
    -0x1.fffff60p0,
  },
  { // Entry 98
    -0x1.837b994a6d8ff7f2750755df5843e84dp-1,
    -0x1.4eaa65b9e2ecc308fd82f65e09d06be4p-1,
    0x1.fffffe3ffffffp1,
  },
  { // Entry 99
    0x1.837b994a6d8ff7f2750755df5843e84dp-1,
    -0x1.4eaa65b9e2ecc308fd82f65e09d06be4p-1,
    -0x1.fffffe3ffffffp1,
  },
  { // Entry 100
    0x1.aed548f090c1dffe6e04322dc8e8cbfap-1,
    0x1.14a280fb507cf8999a1b291995646152p-1,
    0x1.ffffffffffe7fp-1,
  },
  { // Entry 101
    -0x1.aed548f090c1dffe6e04322dc8e8cbfap-1,
    0x1.14a280fb507cf8999a1b291995646152p-1,
    -0x1.ffffffffffe7fp-1,
  },
  { // Entry 102
    -0.0,
    0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.0p-1074,
  },
  { // Entry 103
    0.0,
    0x1.ffffffffffffffffffffffffffffffffp-1,
    0x1.0p-1074,
  },
  { // Entry 104
    -0.0,
    0x1.p0,
    -0.0,
  },
  { // Entry 105
    0.0,
    0x1.ffffffffffffffffffffffffffffffffp-1,
    0x1.0p-1074,
  },
  { // Entry 106
    -0.0,
    0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.0p-1074,
  },
  { // Entry 107
    -0x1.0000000000000fffffffffffffffffffp-1022,
    0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.0000000000001p-1022,
  },
  { // Entry 108
    0x1.0000000000000fffffffffffffffffffp-1022,
    0x1.ffffffffffffffffffffffffffffffffp-1,
    0x1.0000000000001p-1022,
  },
  { // Entry 109
    -0x1.ffffffffffffffffffffffffffffffffp-1023,
    0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.0p-1022,
  },
  { // Entry 110
    0x1.ffffffffffffffffffffffffffffffffp-1023,
    0x1.ffffffffffffffffffffffffffffffffp-1,
    0x1.0p-1022,
  },
  { // Entry 111
    -0x1.ffffffffffffdfffffffffffffffffffp-1023,
    0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.ffffffffffffep-1023,
  },
  { // Entry 112
    0x1.ffffffffffffdfffffffffffffffffffp-1023,
    0x1.ffffffffffffffffffffffffffffffffp-1,
    0x1.ffffffffffffep-1023,
  },
  { // Entry 113
    0x1.ffffffffffffdfffffffffffffffffffp-1023,
    0x1.ffffffffffffffffffffffffffffffffp-1,
    0x1.ffffffffffffep-1023,
  },
  { // Entry 114
    -0x1.ffffffffffffdfffffffffffffffffffp-1023,
    0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.ffffffffffffep-1023,
  },
  { // Entry 115
    0x1.ffffffffffffffffffffffffffffffffp-1023,
    0x1.ffffffffffffffffffffffffffffffffp-1,
    0x1.0p-1022,
  },
  { // Entry 116
    -0x1.ffffffffffffffffffffffffffffffffp-1023,
    0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.0p-1022,
  },
  { // Entry 117
    0x1.0000000000000fffffffffffffffffffp-1022,
    0x1.ffffffffffffffffffffffffffffffffp-1,
    0x1.0000000000001p-1022,
  },
  { // Entry 118
    -0x1.0000000000000fffffffffffffffffffp-1022,
    0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.0000000000001p-1022,
  },
  { // Entry 119
    0x1.9999996de8ca198c24ab9449beee16d5p-13,
    0x1.ffffff5c28f5cb4c5272061281211120p-1,
    0x1.999999999999ap-13,
  },
  { // Entry 120
    -0x1.9999996de8ca198c24ab9449beee16d5p-13,
    0x1.ffffff5c28f5cb4c5272061281211120p-1,
    -0x1.999999999999ap-13,
  },
  { // Entry 121
    0x1.999998ead65b96f78a4dbfd839c7ef2ep-12,
    0x1.fffffd70a3d7960cd5695a06fdb80e74p-1,
    0x1.999999999999ap-12,
  },
  { // Entry 122
    -0x1.999998ead65b96f78a4dbfd839c7ef2ep-12,
    0x1.fffffd70a3d7960cd5695a06fdb80e74p-1,
    -0x1.999999999999ap-12,
  },
  { // Entry 123
    0x1.3333320c49babff151b6d04290e2c3a2p-11,
    0x1.fffffa3d70a69ad42b39d8696632f856p-1,
    0x1.3333333333334p-11,
  },
  { // Entry 124
    -0x1.3333320c49babff151b6d04290e2c3a2p-11,
    0x1.fffffa3d70a69ad42b39d8696632f856p-1,
    -0x1.3333333333334p-11,
  },
  { // Entry 125
    0x1.999996de8ca2884da2f08f25bb024d08p-11,
    0x1.fffff5c28f64e5ec0da0a4f7f4388052p-1,
    0x1.999999999999ap-11,
  },
  { // Entry 126
    -0x1.999996de8ca2884da2f08f25bb024d08p-11,
    0x1.fffff5c28f64e5ec0da0a4f7f4388052p-1,
    -0x1.999999999999ap-11,
  },
  { // Entry 127
    0x1.fffffaaaaaaeeeeeed4ed4edab4c7bd6p-11,
    0x1.fffff0000015555549f49f4d34d34ca0p-1,
    0x1.0p-10,
  },
  { // Entry 128
    -0x1.fffffaaaaaaeeeeeed4ed4edab4c7bd6p-11,
    0x1.fffff0000015555549f49f4d34d34ca0p-1,
    -0x1.0p-10,
  },
  { // Entry 129
    0x1.33332e978d552afc883bdb04751e3835p-10,
    0x1.ffffe8f5c2bb98c7c103d2ff79f15d6ap-1,
    0x1.3333333333333p-10,
  },
  { // Entry 130
    -0x1.33332e978d552afc883bdb04751e3835p-10,
    0x1.ffffe8f5c2bb98c7c103d2ff79f15d6ap-1,
    -0x1.3333333333333p-10,
  },
  { // Entry 131
    0x1.66665f1529aff8a3809246670a436c3cp-10,
    0x1.ffffe0a3d75c31b26451166d6f398abdp-1,
    0x1.6666666666666p-10,
  },
  { // Entry 132
    -0x1.66665f1529aff8a3809246670a436c3cp-10,
    0x1.ffffe0a3d75c31b26451166d6f398abdp-1,
    -0x1.6666666666666p-10,
  },
  { // Entry 133
    0x1.99998ead65cdf82e194c133997f2fb68p-10,
    0x1.ffffd70a3dfc733b3331d8382b1e9df5p-1,
    0x1.9999999999999p-10,
  },
  { // Entry 134
    -0x1.99998ead65cdf82e194c133997f2fb68p-10,
    0x1.ffffd70a3dfc733b3331d8382b1e9df5p-1,
    -0x1.9999999999999p-10,
  },
  { // Entry 135
    0x1.ccccbd3f7d15d42affb9f02bf1dc257bp-10,
    0x1.ffffcc28f6a2823f3765b50659ecb0e2p-1,
    0x1.cccccccccccccp-10,
  },
  { // Entry 136
    -0x1.ccccbd3f7d15d42affb9f02bf1dc257bp-10,
    0x1.ffffcc28f6a2823f3765b50659ecb0e2p-1,
    -0x1.cccccccccccccp-10,
  },
  { // Entry 137
    0x1.0665ae9c7b44ed280216be2104f28f02p-7,
    0x1.fffbcc2a6e86fef7d2af1580bd8e6699p-1,
    0x1.0666666666666p-7,
  },
  { // Entry 138
    -0x1.0665ae9c7b44ed280216be2104f28f02p-7,
    0x1.fffbcc2a6e86fef7d2af1580bd8e6699p-1,
    -0x1.0666666666666p-7,
  },
  { // Entry 139
    0x1.ccc8e97b59f618898c4ac3a0aeddf709p-7,
    0x1.fff30a4b6fcc1405e18fbf7335d2f789p-1,
    0x1.cccccccccccccp-7,
  },
  { // Entry 140
    -0x1.ccc8e97b59f618898c4ac3a0aeddf709p-7,
    0x1.fff30a4b6fcc1405e18fbf7335d2f789p-1,
    -0x1.cccccccccccccp-7,
  },
  { // Entry 141
    0x1.4993e8a8ff79b132046efa7856a97538p-6,
    0x1.ffe57a780f38c0db37051fa8c8d60fbcp-1,
    0x1.4999999999999p-6,
  },
  { // Entry 142
    -0x1.4993e8a8ff79b132046efa7856a97538p-6,
    0x1.ffe57a780f38c0db37051fa8c8d60fbcp-1,
    -0x1.4999999999999p-6,
  },
  { // Entry 143
    0x1.acc044c56db0e19f82c9c3cff246e201p-6,
    0x1.ffd31cd0e1d62c05d2cded21add8bd33p-1,
    0x1.accccccccccccp-6,
  },
  { // Entry 144
    -0x1.acc044c56db0e19f82c9c3cff246e201p-6,
    0x1.ffd31cd0e1d62c05d2cded21add8bd33p-1,
    -0x1.accccccccccccp-6,
  },
  { // Entry 145
    0x1.07f44d67cf41afbc0c95108b99f91b01p-5,
    0x1.ffbbf18207542ef81390d73c3ba89c1ap-1,
    0x1.080p-5,
  },
  { // Entry 146
    -0x1.07f44d67cf41afbc0c95108b99f91b01p-5,
    0x1.ffbbf18207542ef81390d73c3ba89c1ap-1,
    -0x1.080p-5,
  },
  { // Entry 147
    0x1.3985fe46f1c8714eaa1418561963e89bp-5,
    0x1.ff9ff8c3299f54457bbaf8c12173b46bp-1,
    0x1.399999999999ap-5,
  },
  { // Entry 148
    -0x1.3985fe46f1c8714eaa1418561963e89bp-5,
    0x1.ff9ff8c3299f54457bbaf8c12173b46bp-1,
    -0x1.399999999999ap-5,
  },
  { // Entry 149
    0x1.6b14bde93ac5f7d24544d0ecf8be7aeep-5,
    0x1.ff7f32d77c5b1c42f1660c9b6f2ef64fp-1,
    0x1.6b33333333334p-5,
  },
  { // Entry 150
    -0x1.6b14bde93ac5f7d24544d0ecf8be7aeep-5,
    0x1.ff7f32d77c5b1c42f1660c9b6f2ef64fp-1,
    -0x1.6b33333333334p-5,
  },
  { // Entry 151
    0x1.9ca0153ed8396b02f8605219a5fe5917p-5,
    0x1.ff59a00dbc40896bb5e4ac8ad293afb4p-1,
    0x1.9cccccccccccep-5,
  },
  { // Entry 152
    -0x1.9ca0153ed8396b02f8605219a5fe5917p-5,
    0x1.ff59a00dbc40896bb5e4ac8ad293afb4p-1,
    -0x1.9cccccccccccep-5,
  },
  { // Entry 153
    0x1.ce278d4027d34387f184d4ab2aaf545fp-5,
    0x1.ff2f40c02e60f61d6dcfc39b6c2be087p-1,
    0x1.ce66666666666p-5,
  },
  { // Entry 154
    -0x1.ce278d4027d34387f184d4ab2aaf545fp-5,
    0x1.ff2f40c02e60f61d6dcfc39b6c2be087p-1,
    -0x1.ce66666666666p-5,
  },
  { // Entry 155
    0x1.43c1e9c171a667a0b92519a04fa5a91cp-1,
    0x1.8ca46c7d8975e57a1484f05c3738d83bp-1,
    0x1.5e7fc4369bdadp-1,
  },
  { // Entry 156
    -0x1.43c1e9c171a667a0b92519a04fa5a91cp-1,
    0x1.8ca46c7d8975e57a1484f05c3738d83bp-1,
    -0x1.5e7fc4369bdadp-1,
  },
  { // Entry 157
    0x1.ee3d6bcea09ca18b1d1ce7ee04fd886fp-1,
    0x1.0b5d3802fc7991140168f294eedd7904p-2,
    0x1.4e7fc4369bdadp0,
  },
  { // Entry 158
    -0x1.ee3d6bcea09ca18b1d1ce7ee04fd886fp-1,
    0x1.0b5d3802fc7991140168f294eedd7904p-2,
    -0x1.4e7fc4369bdadp0,
  },
  { // Entry 159
    0x1.df8e22ea809d65c6a69b96aca60be432p-1,
    -0x1.66b96f53323af1d7e31a7162ab18a75bp-2,
    0x1.edbfa651e9c84p0,
  },
  { // Entry 160
    -0x1.df8e22ea809d65c6a69b96aca60be432p-1,
    -0x1.66b96f53323af1d7e31a7162ab18a75bp-2,
    -0x1.edbfa651e9c84p0,
  },
  { // Entry 161
    0x1.1d3479eac7ae35e2fbea0ae696434692p-1,
    -0x1.a93554888c32fa57f22a9529a320c1cbp-1,
    0x1.467fc4369bdadp1,
  },
  { // Entry 162
    -0x1.1d3479eac7ae35e2fbea0ae696434692p-1,
    -0x1.a93554888c32fa57f22a9529a320c1cbp-1,
    -0x1.467fc4369bdadp1,
  },
  { // Entry 163
    -0x1.ffeaaaeeee84b44ccefef832254d28c0p-6,
    -0x1.ffc00155527d2b9fda2ae89396e09727p-1,
    0x1.961fb54442d18p1,
  },
  { // Entry 164
    0x1.ffeaaaeeee84b44ccefef832254d28c0p-6,
    -0x1.ffc00155527d2b9fda2ae89396e09727p-1,
    -0x1.961fb54442d18p1,
  },
  { // Entry 165
    -0x1.3734d32d49bd0b942772a7567d514140p-1,
    -0x1.96907c5c7c25b88e34addff1fbef66e4p-1,
    0x1.e5bfa651e9c83p1,
  },
  { // Entry 166
    0x1.3734d32d49bd0b942772a7567d514140p-1,
    -0x1.96907c5c7c25b88e34addff1fbef66e4p-1,
    -0x1.e5bfa651e9c83p1,
  },
  { // Entry 167
    -0x1.e9d25d19911e205b653521f42b9b864fp-1,
    -0x1.2a1e5a50f948cd487c5309682b110a53p-2,
    0x1.1aafcbafc85f7p2,
  },
  { // Entry 168
    0x1.e9d25d19911e205b653521f42b9b864fp-1,
    -0x1.2a1e5a50f948cd487c5309682b110a53p-2,
    -0x1.1aafcbafc85f7p2,
  },
  { // Entry 169
    -0x1.e4ecdc5a4e465899928eb9fc95829d48p-1,
    0x1.4894f695dc56bce8b273e5524f181264p-2,
    0x1.427fc4369bdadp2,
  },
  { // Entry 170
    0x1.e4ecdc5a4e465899928eb9fc95829d48p-1,
    0x1.4894f695dc56bce8b273e5524f181264p-2,
    -0x1.427fc4369bdadp2,
  },
  { // Entry 171
    -0x1.2a59f1034426197fa6eee22762967f25p-1,
    0x1.a016ea3a692ce0c321b77f168de39122p-1,
    0x1.6a4fbcbd6f562p2,
  },
  { // Entry 172
    0x1.2a59f1034426197fa6eee22762967f25p-1,
    0x1.a016ea3a692ce0c321b77f168de39122p-1,
    -0x1.6a4fbcbd6f562p2,
  },
  { // Entry 173
    -0x1.26312443bd35f19312eac0a1a6b5659ep-1,
    0x1.a30a69f5537ebc22f0870c2bd26ef284p-1,
    0x1.6af2eff0a2896p2,
  },
  { // Entry 174
    0x1.26312443bd35f19312eac0a1a6b5659ep-1,
    0x1.a30a69f5537ebc22f0870c2bd26ef284p-1,
    -0x1.6af2eff0a2896p2,
  },
  { // Entry 175
    -0x1.e18e660a5e2fb316ecbb9ed70122eff5p-1,
    0x1.5bd62e8b04ad5915e66242349b756e11p-2,
    0x1.43c62a9d02414p2,
  },
  { // Entry 176
    0x1.e18e660a5e2fb316ecbb9ed70122eff5p-1,
    0x1.5bd62e8b04ad5915e66242349b756e11p-2,
    -0x1.43c62a9d02414p2,
  },
  { // Entry 177
    -0x1.ee0e83a0198b6e2ef7c48e6625291a0ap-1,
    -0x1.0cb71f671e63410966e78d2009c0616fp-2,
    0x1.1c99654961f92p2,
  },
  { // Entry 178
    0x1.ee0e83a0198b6e2ef7c48e6625291a0ap-1,
    -0x1.0cb71f671e63410966e78d2009c0616fp-2,
    -0x1.1c99654961f92p2,
  },
  { // Entry 179
    -0x1.4727747338e4653616eadbd7ec3d02d3p-1,
    -0x1.89d86aa8521c11b74f8b1954c08f9b36p-1,
    0x1.ead93feb8361fp1,
  },
  { // Entry 180
    0x1.4727747338e4653616eadbd7ec3d02d3p-1,
    -0x1.89d86aa8521c11b74f8b1954c08f9b36p-1,
    -0x1.ead93feb8361fp1,
  },
  { // Entry 181
    -0x1.4ba2f75dda5fe434320905a7184ff1afp-4,
    -0x1.fe51ac554a16ad8194f181085f8a17f2p-1,
    0x1.9c7fb54442d1ap1,
  },
  { // Entry 182
    0x1.4ba2f75dda5fe434320905a7184ff1afp-4,
    -0x1.fe51ac554a16ad8194f181085f8a17f2p-1,
    -0x1.9c7fb54442d1ap1,
  },
  { // Entry 183
    0x1.034c4d633b4ef0a9089b43892a462a26p-1,
    -0x1.b97c04d08bc5d765b341a22b2c720b6fp-1,
    0x1.4e262a9d02415p1,
  },
  { // Entry 184
    -0x1.034c4d633b4ef0a9089b43892a462a26p-1,
    -0x1.b97c04d08bc5d765b341a22b2c720b6fp-1,
    -0x1.4e262a9d02415p1,
  },
  { // Entry 185
    0x1.d1e4cde2f3944f4c134c05cc4e5339a3p-1,
    -0x1.a8ac8a3e58f6ca952390299d2e8b187fp-2,
    0x1.ff993feb83620p0,
  },
  { // Entry 186
    -0x1.d1e4cde2f3944f4c134c05cc4e5339a3p-1,
    -0x1.a8ac8a3e58f6ca952390299d2e8b187fp-2,
    -0x1.ff993feb83620p0,
  },
  { // Entry 187
    0x1.f750235c949926c48c90e41a91474c06p-1,
    0x1.77a8b9b3d254a9e39d02b3eb3e2390e7p-3,
    0x1.62e62a9d02416p0,
  },
  { // Entry 188
    -0x1.f750235c949926c48c90e41a91474c06p-1,
    0x1.77a8b9b3d254a9e39d02b3eb3e2390e7p-3,
    -0x1.62e62a9d02416p0,
  },
  { // Entry 189
    0x1.65f7d571279b0b8005552fd47a2e77aep-1,
    0x1.6e1061205dd79051c112d30a05097c61p-1,
    0x1.8c662a9d02419p-1,
  },
  { // Entry 190
    -0x1.65f7d571279b0b8005552fd47a2e77aep-1,
    0x1.6e1061205dd79051c112d30a05097c61p-1,
    -0x1.8c662a9d02419p-1,
  },
  { // Entry 191
    -0x1.fe043f57369d6a52fa33f0119ec4da19p-1,
    -0x1.682f3cc3c7a08da2ce02a41cdc7bed86p-4,
    -0x1.a8aa1d11c44ffp0,
  },
  { // Entry 192
    0x1.fe043f57369d6a52fa33f0119ec4da19p-1,
    -0x1.682f3cc3c7a08da2ce02a41cdc7bed86p-4,
    0x1.a8aa1d11c44ffp0,
  },
  { // Entry 193
    -0x1.fff18f24f3e4b87bf8c3762cb44f46d6p-1,
    -0x1.e6669a270c36d4879b428ddba96cd87bp-7,
    -0x1.95ec8b9e03d54p0,
  },
  { // Entry 194
    0x1.fff18f24f3e4b87bf8c3762cb44f46d6p-1,
    -0x1.e6669a270c36d4879b428ddba96cd87bp-7,
    0x1.95ec8b9e03d54p0,
  },
  { // Entry 195
    -0x1.ff20d961624e7063a78203b811f579cap-1,
    0x1.ddd1ec25e209f1bbf7e17ef6c8450cd7p-5,
    -0x1.832efa2a435a9p0,
  },
  { // Entry 196
    0x1.ff20d961624e7063a78203b811f579cap-1,
    0x1.ddd1ec25e209f1bbf7e17ef6c8450cd7p-5,
    0x1.832efa2a435a9p0,
  },
  { // Entry 197
    -0x1.fb933c40107fd775185ac14918c8fbafp-1,
    0x1.0cab9115640d993082a7343bb5affea2p-3,
    -0x1.707168b682dfep0,
  },
  { // Entry 198
    0x1.fb933c40107fd775185ac14918c8fbafp-1,
    0x1.0cab9115640d993082a7343bb5affea2p-3,
    0x1.707168b682dfep0,
  },
  { // Entry 199
    -0x1.f54d971881ad685b782ef88e6350f7cdp-1,
    0x1.a0723a95492edee5dc98394e45f96d88p-3,
    -0x1.5db3d742c2653p0,
  },
  { // Entry 200
    0x1.f54d971881ad685b782ef88e6350f7cdp-1,
    0x1.a0723a95492edee5dc98394e45f96d88p-3,
    0x1.5db3d742c2653p0,
  },
  { // Entry 201
    -0x1.ec5883b7b6cf4d859ab04e15d53698c9p-1,
    0x1.18fee96a1a585928a94cda7e3d916fe1p-2,
    -0x1.4af645cf01ea8p0,
  },
  { // Entry 202
    0x1.ec5883b7b6cf4d859ab04e15d53698c9p-1,
    0x1.18fee96a1a585928a94cda7e3d916fe1p-2,
    0x1.4af645cf01ea8p0,
  },
  { // Entry 203
    -0x1.e0c04a94e17309c806c1c78bddc1d607p-1,
    0x1.6043621b13be2ff07085f8278598e566p-2,
    -0x1.3838b45b416fdp0,
  },
  { // Entry 204
    0x1.e0c04a94e17309c806c1c78bddc1d607p-1,
    0x1.6043621b13be2ff07085f8278598e566p-2,
    0x1.3838b45b416fdp0,
  },
  { // Entry 205
    -0x1.d294d1f96c7ebdb9869dd97cf574ddb9p-1,
    0x1.a5a4ccf40d9d9ba97faa4e23ecce9e3ap-2,
    -0x1.257b22e780f52p0,
  },
  { // Entry 206
    0x1.d294d1f96c7ebdb9869dd97cf574ddb9p-1,
    0x1.a5a4ccf40d9d9ba97faa4e23ecce9e3ap-2,
    0x1.257b22e780f52p0,
  },
  { // Entry 207
    -0x1.c1e9883373d7ecc48c92dc8875505f7ep-1,
    0x1.e8c405f36f85b7f5d6a38dfd4a692341p-2,
    -0x1.12bd9173c07abp0,
  },
  { // Entry 208
    0x1.c1e9883373d7ecc48c92dc8875505f7ep-1,
    0x1.e8c405f36f85b7f5d6a38dfd4a692341p-2,
    0x1.12bd9173c07abp0,
  },
  { // Entry 209
    -0x1.a2c289d9d055ac377f67d7a54a0b3005p-1,
    0x1.26976a6c4e0f86633327f1ceecb508aep-1,
    -0x1.ea5c3ed5b3850p-1,
  },
  { // Entry 210
    0x1.a2c289d9d055ac377f67d7a54a0b3005p-1,
    0x1.26976a6c4e0f86633327f1ceecb508aep-1,
    0x1.ea5c3ed5b3850p-1,
  },
  { // Entry 211
    -0x1.95f05257dbcb5f4b12636c5878ea405ap-1,
    0x1.3805a1882009f2843da808e959f17861p-1,
    -0x1.d4b87dab670a0p-1,
  },
  { // Entry 212
    0x1.95f05257dbcb5f4b12636c5878ea405ap-1,
    0x1.3805a1882009f2843da808e959f17861p-1,
    0x1.d4b87dab670a0p-1,
  },
  { // Entry 213
    -0x1.88647f26a6e0f6b2715a6c3797ec11f5p-1,
    0x1.48e52e0a65bcb3cd46455c4d2338bdf2p-1,
    -0x1.bf14bc811a8f0p-1,
  },
  { // Entry 214
    0x1.88647f26a6e0f6b2715a6c3797ec11f5p-1,
    0x1.48e52e0a65bcb3cd46455c4d2338bdf2p-1,
    0x1.bf14bc811a8f0p-1,
  },
  { // Entry 215
    -0x1.7a2541dfd4e752de38f04aba21fc9d9fp-1,
    0x1.592e58ea0a9eec0b357eb4e9a83b0ea5p-1,
    -0x1.a970fb56ce140p-1,
  },
  { // Entry 216
    0x1.7a2541dfd4e752de38f04aba21fc9d9fp-1,
    0x1.592e58ea0a9eec0b357eb4e9a83b0ea5p-1,
    0x1.a970fb56ce140p-1,
  },
  { // Entry 217
    -0x1.6b391e25bc26cbbcf7a0184070af9c39p-1,
    0x1.68d9afe052d1f0e9324ae876961bcdb1p-1,
    -0x1.93cd3a2c81990p-1,
  },
  { // Entry 218
    0x1.6b391e25bc26cbbcf7a0184070af9c39p-1,
    0x1.68d9afe052d1f0e9324ae876961bcdb1p-1,
    0x1.93cd3a2c81990p-1,
  },
  { // Entry 219
    -0x1.5ba6e6a8e706535b98fc99dfaef824f1p-1,
    0x1.77e008d0775e744eb16a2c4ec7184c43p-1,
    -0x1.7e297902351e0p-1,
  },
  { // Entry 220
    0x1.5ba6e6a8e706535b98fc99dfaef824f1p-1,
    0x1.77e008d0775e744eb16a2c4ec7184c43p-1,
    0x1.7e297902351e0p-1,
  },
  { // Entry 221
    -0x1.4b75ba096fa549eb93595d8194ab917fp-1,
    0x1.863a850e438fe029302aba0f5f127616p-1,
    -0x1.6885b7d7e8a30p-1,
  },
  { // Entry 222
    0x1.4b75ba096fa549eb93595d8194ab917fp-1,
    0x1.863a850e438fe029302aba0f5f127616p-1,
    0x1.6885b7d7e8a30p-1,
  },
  { // Entry 223
    -0x1.3aacff95a3122b15f372bfd2fdf9a75fp-1,
    0x1.93e2948233fce814439ed51fd2548920p-1,
    -0x1.52e1f6ad9c280p-1,
  },
  { // Entry 224
    0x1.3aacff95a3122b15f372bfd2fdf9a75fp-1,
    0x1.93e2948233fce814439ed51fd2548920p-1,
    0x1.52e1f6ad9c280p-1,
  },
  { // Entry 225
    -0x1.295463e769284a5aed17a443392f38f3p-1,
    0x1.a0d1f8a9a791d4b5694ca68a42fe6c9bp-1,
    -0x1.3d3e35834fad0p-1,
  },
  { // Entry 226
    0x1.295463e769284a5aed17a443392f38f3p-1,
    0x1.a0d1f8a9a791d4b5694ca68a42fe6c9bp-1,
    0x1.3d3e35834fad0p-1,
  },
  { // Entry 227
    -0x1.fc769b77e588495a6f642ca24e4ed3fcp-2,
    0x1.bc6bd861e13de309428e00f7bef6c3ecp-1,
    -0x1.0a0b02501c799p-1,
  },
  { // Entry 228
    0x1.fc769b77e588495a6f642ca24e4ed3fcp-2,
    0x1.bc6bd861e13de309428e00f7bef6c3ecp-1,
    0x1.0a0b02501c799p-1,
  },
  { // Entry 229
    -0x1.c853c78462de46b5743315612f8b5a7cp-2,
    0x1.ca59c6fa3d9ce238a227393b6b075bc5p-1,
    -0x1.d8f7208e6b82cp-2,
  },
  { // Entry 230
    0x1.c853c78462de46b5743315612f8b5a7cp-2,
    0x1.ca59c6fa3d9ce238a227393b6b075bc5p-1,
    0x1.d8f7208e6b82cp-2,
  },
  { // Entry 231
    -0x1.92aba90aaf27249de49c78fc643c8b72p-2,
    0x1.d6c0b125791cffce83e32564712b78c6p-1,
    -0x1.9dd83c7c9e126p-2,
  },
  { // Entry 232
    0x1.92aba90aaf27249de49c78fc643c8b72p-2,
    0x1.d6c0b125791cffce83e32564712b78c6p-1,
    0x1.9dd83c7c9e126p-2,
  },
  { // Entry 233
    -0x1.5bac064658f39460c83113c0a0097a0cp-2,
    0x1.e1960261829858391645bbe12019e58ap-1,
    -0x1.62b9586ad0a20p-2,
  },
  { // Entry 234
    0x1.5bac064658f39460c83113c0a0097a0cp-2,
    0x1.e1960261829858391645bbe12019e58ap-1,
    0x1.62b9586ad0a20p-2,
  },
  { // Entry 235
    -0x1.2383ca8078e58477cd5fb1d9de031dcep-2,
    0x1.ead07cc6356964e27a1036d2f8b158f7p-1,
    -0x1.279a74590331ap-2,
  },
  { // Entry 236
    0x1.2383ca8078e58477cd5fb1d9de031dcep-2,
    0x1.ead07cc6356964e27a1036d2f8b158f7p-1,
    0x1.279a74590331ap-2,
  },
  { // Entry 237
    -0x1.d4c5bc11d2371af2fe25ef5ede2766a3p-3,
    0x1.f26840e7b2188f7a0cc661a0ede3728bp-1,
    -0x1.d8f7208e6b829p-3,
  },
  { // Entry 238
    0x1.d4c5bc11d2371af2fe25ef5ede2766a3p-3,
    0x1.f26840e7b2188f7a0cc661a0ede3728bp-1,
    0x1.d8f7208e6b829p-3,
  },
  { // Entry 239
    -0x1.60f3faaf43023d3c7863ae06d4d59774p-3,
    0x1.f856d48db797dec0b79e1353409dc3f2p-1,
    -0x1.62b9586ad0a1ep-3,
  },
  { // Entry 240
    0x1.60f3faaf43023d3c7863ae06d4d59774p-3,
    0x1.f856d48db797dec0b79e1353409dc3f2p-1,
    0x1.62b9586ad0a1ep-3,
  },
  { // Entry 241
    -0x1.d7ea3de45a9d6563ac005c0c5bad8c50p-4,
    0x1.fc97283a424797215f8a8d1967736c9bp-1,
    -0x1.d8f7208e6b826p-4,
  },
  { // Entry 242
    0x1.d7ea3de45a9d6563ac005c0c5bad8c50p-4,
    0x1.fc97283a424797215f8a8d1967736c9bp-1,
    0x1.d8f7208e6b826p-4,
  },
  { // Entry 243
    -0x1.d8b3df489987a6fe0eead008e720aa22p-5,
    0x1.ff259b7ab9f4f9a8cb9f1c333272e409p-1,
    -0x1.d8f7208e6b82dp-5,
  },
  { // Entry 244
    0x1.d8b3df489987a6fe0eead008e720aa22p-5,
    0x1.ff259b7ab9f4f9a8cb9f1c333272e409p-1,
    0x1.d8f7208e6b82dp-5,
  },
  { // Entry 245
    0x1.d8b3df489987a6fe0eead008e720aa22p-5,
    0x1.ff259b7ab9f4f9a8cb9f1c333272e409p-1,
    0x1.d8f7208e6b82dp-5,
  },
  { // Entry 246
    -0x1.d8b3df489987a6fe0eead008e720aa22p-5,
    0x1.ff259b7ab9f4f9a8cb9f1c333272e409p-1,
    -0x1.d8f7208e6b82dp-5,
  },
  { // Entry 247
    0x1.d7ea3de45a9dd4a4bccd1a8c048faf4cp-4,
    0x1.fc97283a424795847294654a1d8a08edp-1,
    0x1.d8f7208e6b82dp-4,
  },
  { // Entry 248
    -0x1.d7ea3de45a9dd4a4bccd1a8c048faf4cp-4,
    0x1.fc97283a424795847294654a1d8a08edp-1,
    -0x1.d8f7208e6b82dp-4,
  },
  { // Entry 249
    0x1.60f3faaf43027c4752f564f9d0818fe8p-3,
    0x1.f856d48db797dbfecfa8b4cd3be44027p-1,
    0x1.62b9586ad0a22p-3,
  },
  { // Entry 250
    -0x1.60f3faaf43027c4752f564f9d0818fe8p-3,
    0x1.f856d48db797dbfecfa8b4cd3be44027p-1,
    -0x1.62b9586ad0a22p-3,
  },
  { // Entry 251
    0x1.d4c5bc11d23759400642e5a1efdc0f85p-3,
    0x1.f26840e7b2188bd0814e3dfc7f6f3f87p-1,
    0x1.d8f7208e6b82dp-3,
  },
  { // Entry 252
    -0x1.d4c5bc11d23759400642e5a1efdc0f85p-3,
    0x1.f26840e7b2188bd0814e3dfc7f6f3f87p-1,
    -0x1.d8f7208e6b82dp-3,
  },
  { // Entry 253
    0x1.2383ca8078e5a324d52c1530742cd4f5p-2,
    0x1.ead07cc6356960546ae634ef62621fb2p-1,
    0x1.279a74590331cp-2,
  },
  { // Entry 254
    -0x1.2383ca8078e5a324d52c1530742cd4f5p-2,
    0x1.ead07cc6356960546ae634ef62621fb2p-1,
    -0x1.279a74590331cp-2,
  },
  { // Entry 255
    0x1.5bac064658f3b27a28572bea256195efp-2,
    0x1.e1960261829852ca662ca27d518c2fa9p-1,
    0x1.62b9586ad0a22p-2,
  },
  { // Entry 256
    -0x1.5bac064658f3b27a28572bea256195efp-2,
    0x1.e1960261829852ca662ca27d518c2fa9p-1,
    -0x1.62b9586ad0a22p-2,
  },
  { // Entry 257
    0x1.92aba90aaf274209efaed08e34071e3bp-2,
    0x1.d6c0b125791cf983d53efaa7d45e291ep-1,
    0x1.9dd83c7c9e128p-2,
  },
  { // Entry 258
    -0x1.92aba90aaf274209efaed08e34071e3bp-2,
    0x1.d6c0b125791cf983d53efaa7d45e291ep-1,
    -0x1.9dd83c7c9e128p-2,
  },
  { // Entry 259
    0x1.c853c78462de635b10a2b93afd75da26p-2,
    0x1.ca59c6fa3d9cdb17530927aff1b33abbp-1,
    0x1.d8f7208e6b82ep-2,
  },
  { // Entry 260
    -0x1.c853c78462de635b10a2b93afd75da26p-2,
    0x1.ca59c6fa3d9cdb17530927aff1b33abbp-1,
    -0x1.d8f7208e6b82ep-2,
  },
  { // Entry 261
    0x1.fc769b77e588495a6f642ca24e4ed3fcp-2,
    0x1.bc6bd861e13de309428e00f7bef6c3ecp-1,
    0x1.0a0b02501c799p-1,
  },
  { // Entry 262
    -0x1.fc769b77e588495a6f642ca24e4ed3fcp-2,
    0x1.bc6bd861e13de309428e00f7bef6c3ecp-1,
    -0x1.0a0b02501c799p-1,
  },
  { // Entry 263
    0x1.295463e769281640ae026f50fc45e301p-1,
    0x1.a0d1f8a9a791f9dff5c993af4908264dp-1,
    0x1.3d3e35834faccp-1,
  },
  { // Entry 264
    -0x1.295463e769281640ae026f50fc45e301p-1,
    0x1.a0d1f8a9a791f9dff5c993af4908264dp-1,
    -0x1.3d3e35834faccp-1,
  },
  { // Entry 265
    0x1.3aacff95a311f899a0e279535e81c4ecp-1,
    0x1.93e2948233fd0f69e3918982148f8265p-1,
    0x1.52e1f6ad9c27cp-1,
  },
  { // Entry 266
    -0x1.3aacff95a311f899a0e279535e81c4ecp-1,
    0x1.93e2948233fd0f69e3918982148f8265p-1,
    -0x1.52e1f6ad9c27cp-1,
  },
  { // Entry 267
    0x1.4b75ba096fa5192442b7950f960f8006p-1,
    0x1.863a850e43900997e76be80405437377p-1,
    0x1.6885b7d7e8a2cp-1,
  },
  { // Entry 268
    -0x1.4b75ba096fa5192442b7950f960f8006p-1,
    0x1.863a850e43900997e76be80405437377p-1,
    -0x1.6885b7d7e8a2cp-1,
  },
  { // Entry 269
    0x1.5ba6e6a8e706245f97e28af3ddb700f6p-1,
    0x1.77e008d0775e9fc38e3f492f8e93ff51p-1,
    0x1.7e297902351dcp-1,
  },
  { // Entry 270
    -0x1.5ba6e6a8e706245f97e28af3ddb700f6p-1,
    0x1.77e008d0775e9fc38e3f492f8e93ff51p-1,
    -0x1.7e297902351dcp-1,
  },
  { // Entry 271
    0x1.6b391e25bc269ea1c1a40de62fbc03b4p-1,
    0x1.68d9afe052d21e50560f9ffb6cc1b945p-1,
    0x1.93cd3a2c8198cp-1,
  },
  { // Entry 272
    -0x1.6b391e25bc269ea1c1a40de62fbc03b4p-1,
    0x1.68d9afe052d21e50560f9ffb6cc1b945p-1,
    -0x1.93cd3a2c8198cp-1,
  },
  { // Entry 273
    0x1.7a2541dfd4e727b86dd309664186ec6bp-1,
    0x1.592e58ea0a9f1b4fddbaaf868fe47911p-1,
    0x1.a970fb56ce13cp-1,
  },
  { // Entry 274
    -0x1.7a2541dfd4e727b86dd309664186ec6bp-1,
    0x1.592e58ea0a9f1b4fddbaaf868fe47911p-1,
    -0x1.a970fb56ce13cp-1,
  },
  { // Entry 275
    0x1.88647f26a6e0cd95cb991f7ffe61a02ep-1,
    0x1.48e52e0a65bce4d9d62a31293f7d41c1p-1,
    0x1.bf14bc811a8ecp-1,
  },
  { // Entry 276
    -0x1.88647f26a6e0cd95cb991f7ffe61a02ep-1,
    0x1.48e52e0a65bce4d9d62a31293f7d41c1p-1,
    -0x1.bf14bc811a8ecp-1,
  },
  { // Entry 277
    0x1.95f05257dbcb384a5e326857376dd801p-1,
    0x1.3805a188200a254247f30462c36acf6ap-1,
    0x1.d4b87dab6709cp-1,
  },
  { // Entry 278
    -0x1.95f05257dbcb384a5e326857376dd801p-1,
    0x1.3805a188200a254247f30462c36acf6ap-1,
    -0x1.d4b87dab6709cp-1,
  },
  { // Entry 279
    0x1.a2c289d9d0558764921a4de355f9448cp-1,
    0x1.26976a6c4e0fbabb84632bd99feec9c6p-1,
    0x1.ea5c3ed5b384cp-1,
  },
  { // Entry 280
    -0x1.a2c289d9d0558764921a4de355f9448cp-1,
    0x1.26976a6c4e0fbabb84632bd99feec9c6p-1,
    -0x1.ea5c3ed5b384cp-1,
  },
  { // Entry 281
    0x1.c1e9883373d7ecc48c92dc8875505f7ep-1,
    0x1.e8c405f36f85b7f5d6a38dfd4a692341p-2,
    0x1.12bd9173c07abp0,
  },
  { // Entry 282
    -0x1.c1e9883373d7ecc48c92dc8875505f7ep-1,
    0x1.e8c405f36f85b7f5d6a38dfd4a692341p-2,
    -0x1.12bd9173c07abp0,
  },
  { // Entry 283
    0x1.d294d1f96c7ef26e203c5b309a55671fp-1,
    0x1.a5a4ccf40d9cb25f16ad97e480c4b483p-2,
    0x1.257b22e780f56p0,
  },
  { // Entry 284
    -0x1.d294d1f96c7ef26e203c5b309a55671fp-1,
    0x1.a5a4ccf40d9cb25f16ad97e480c4b483p-2,
    -0x1.257b22e780f56p0,
  },
  { // Entry 285
    0x1.e0c04a94e17335d073052a0394b9e1c3p-1,
    0x1.6043621b13bd3f904b3b876df5b2c6f4p-2,
    0x1.3838b45b41701p0,
  },
  { // Entry 286
    -0x1.e0c04a94e17335d073052a0394b9e1c3p-1,
    0x1.6043621b13bd3f904b3b876df5b2c6f4p-2,
    -0x1.3838b45b41701p0,
  },
  { // Entry 287
    0x1.ec5883b7b6cf70a577dd9160d0f8e9d5p-1,
    0x1.18fee96a1a5762fc6770ff168e06ab3ep-2,
    0x1.4af645cf01eacp0,
  },
  { // Entry 288
    -0x1.ec5883b7b6cf70a577dd9160d0f8e9d5p-1,
    0x1.18fee96a1a5762fc6770ff168e06ab3ep-2,
    -0x1.4af645cf01eacp0,
  },
  { // Entry 289
    0x1.f54d971881ad82629bd84d214194e8ddp-1,
    0x1.a0723a95492ce998457fb7a0d09a6385p-3,
    0x1.5db3d742c2657p0,
  },
  { // Entry 290
    -0x1.f54d971881ad82629bd84d214194e8ddp-1,
    0x1.a0723a95492ce998457fb7a0d09a6385p-3,
    -0x1.5db3d742c2657p0,
  },
  { // Entry 291
    0x1.fb933c40107fe83fd16c1789e27f69f7p-1,
    0x1.0cab9115640b9d9d466723bbd5d589bep-3,
    0x1.707168b682e02p0,
  },
  { // Entry 292
    -0x1.fb933c40107fe83fd16c1789e27f69f7p-1,
    0x1.0cab9115640b9d9d466723bbd5d589bep-3,
    -0x1.707168b682e02p0,
  },
  { // Entry 293
    0x1.ff20d961624e77daef329b4029c362dep-1,
    0x1.ddd1ec25e201f538925bf5bcf7c7df6ep-5,
    0x1.832efa2a435adp0,
  },
  { // Entry 294
    -0x1.ff20d961624e77daef329b4029c362dep-1,
    0x1.ddd1ec25e201f538925bf5bcf7c7df6ep-5,
    -0x1.832efa2a435adp0,
  },
  { // Entry 295
    0x1.fff18f24f3e4b69592294f206d7b32c2p-1,
    -0x1.e6669a270c56d3a08d91cc2721f92fe1p-7,
    0x1.95ec8b9e03d58p0,
  },
  { // Entry 296
    -0x1.fff18f24f3e4b69592294f206d7b32c2p-1,
    -0x1.e6669a270c56d3a08d91cc2721f92fe1p-7,
    -0x1.95ec8b9e03d58p0,
  },
  { // Entry 297
    0x1.fe043f57369d6a52fa33f0119ec4da19p-1,
    -0x1.682f3cc3c7a08da2ce02a41cdc7bed86p-4,
    0x1.a8aa1d11c44ffp0,
  },
  { // Entry 298
    -0x1.fe043f57369d6a52fa33f0119ec4da19p-1,
    -0x1.682f3cc3c7a08da2ce02a41cdc7bed86p-4,
    -0x1.a8aa1d11c44ffp0,
  },
  { // Entry 299
    0x1.b3d3695acc4136b2d44714f9b38419b4p-1,
    0x1.0cb3469a29ea66d4031be769702aad5cp-1,
    0x1.04aff6d330942p0,
  },
  { // Entry 300
    -0x1.b3d3695acc4136b2d44714f9b38419b4p-1,
    0x1.0cb3469a29ea66d4031be769702aad5cp-1,
    -0x1.04aff6d330942p0,
  },
  { // Entry 301
    0x1.b3d41972dc8063994f63413d5e4d8e4bp-1,
    0x1.0cb228fa7f8117c82e61cf5393341c64p-1,
    0x1.04b09e98dcdb4p0,
  },
  { // Entry 302
    -0x1.b3d41972dc8063994f63413d5e4d8e4bp-1,
    0x1.0cb228fa7f8117c82e61cf5393341c64p-1,
    -0x1.04b09e98dcdb4p0,
  },
  { // Entry 303
    0x1.b3d4c98a318fb66f821d7286ae7dce7bp-1,
    0x1.0cb10b5a61b05a73e78a3e4447baf514p-1,
    0x1.04b1465e89226p0,
  },
  { // Entry 304
    -0x1.b3d4c98a318fb66f821d7286ae7dce7bp-1,
    0x1.0cb10b5a61b05a73e78a3e4447baf514p-1,
    -0x1.04b1465e89226p0,
  },
  { // Entry 305
    0x1.b3d579a0cb6ee393ff75b58ffe16d13fp-1,
    0x1.0cafedb9d078a984086928aa40d2e4a5p-1,
    0x1.04b1ee2435698p0,
  },
  { // Entry 306
    -0x1.b3d579a0cb6ee393ff75b58ffe16d13fp-1,
    0x1.0cafedb9d078a984086928aa40d2e4a5p-1,
    -0x1.04b1ee2435698p0,
  },
  { // Entry 307
    0x1.b3d629b6aa1d9f65aad1a2fc932c8bcbp-1,
    0x1.0caed018cbda7fa59c631cd55b31aa8dp-1,
    0x1.04b295e9e1b0ap0,
  },
  { // Entry 308
    -0x1.b3d629b6aa1d9f65aad1a2fc932c8bcbp-1,
    0x1.0caed018cbda7fa59c631cd55b31aa8dp-1,
    -0x1.04b295e9e1b0ap0,
  },
  { // Entry 309
    0x1.b3d6d9cbcd9b9e43b7fc7fd428a44dd8p-1,
    0x1.0cadb27753d65785e06d0e464006149ep-1,
    0x1.04b33daf8df7cp0,
  },
  { // Entry 310
    -0x1.b3d6d9cbcd9b9e43b7fc7fd428a44dd8p-1,
    0x1.0cadb27753d65785e06d0e464006149ep-1,
    -0x1.04b33daf8df7cp0,
  },
  { // Entry 311
    0x1.b3d789e035e8948dab275dfe546c5b08p-1,
    0x1.0cac94d5686cabd2430c20fdf2855b47p-1,
    0x1.04b3e5753a3eep0,
  },
  { // Entry 312
    -0x1.b3d789e035e8948dab275dfe546c5b08p-1,
    0x1.0cac94d5686cabd2430c20fdf2855b47p-1,
    -0x1.04b3e5753a3eep0,
  },
  { // Entry 313
    0x1.b3d839f3e30436a358e93cbdcb2bb367p-1,
    0x1.0cab7733099df738645574cd482ef4b2p-1,
    0x1.04b48d3ae6860p0,
  },
  { // Entry 314
    -0x1.b3d839f3e30436a358e93cbdcb2bb367p-1,
    0x1.0cab7733099df738645574cd482ef4b2p-1,
    -0x1.04b48d3ae6860p0,
  },
  { // Entry 315
    0x1.b3d8ea06d4ee0684f5741ec777ed88e0p-1,
    0x1.0caa5990376b061ec1cf3890f1b8e1e3p-1,
    0x1.04b5350092ccfp0,
  },
  { // Entry 316
    -0x1.b3d8ea06d4ee0684f5741ec777ed88e0p-1,
    0x1.0caa5990376b061ec1cf3890f1b8e1e3p-1,
    -0x1.04b5350092ccfp0,
  },
  { // Entry 317
    -0.0,
    0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.0p-1074,
  },
  { // Entry 318
    0.0,
    0x1.ffffffffffffffffffffffffffffffffp-1,
    0x1.0p-1074,
  },
  { // Entry 319
    -0.0,
    0x1.p0,
    -0.0,
  },
  { // Entry 320
    0.0,
    0x1.ffffffffffffffffffffffffffffffffp-1,
    0x1.0p-1074,
  },
  { // Entry 321
    -0.0,
    0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.0p-1074,
  },
  { // Entry 322
    0x1.1773d561fd5065d0e9607a728a39eed2p-1,
    0x1.ad02c771c35ed5f01089a00c6a27e0cfp-1,
    0x1.279a74590331bp-1,
  },
  { // Entry 323
    -0x1.1773d561fd5065d0e9607a728a39eed2p-1,
    0x1.ad02c771c35ed5f01089a00c6a27e0cfp-1,
    -0x1.279a74590331bp-1,
  },
  { // Entry 324
    0x1.1773d561fd507338ff9c088d80c680dbp-1,
    0x1.ad02c771c35ecd3471de9021e6c3b92bp-1,
    0x1.279a74590331cp-1,
  },
  { // Entry 325
    -0x1.1773d561fd507338ff9c088d80c680dbp-1,
    0x1.ad02c771c35ecd3471de9021e6c3b92bp-1,
    -0x1.279a74590331cp-1,
  },
  { // Entry 326
    0x1.1773d561fd5080a115d796a8770d35efp-1,
    0x1.ad02c771c35ec478d333803762f450d5p-1,
    0x1.279a74590331dp-1,
  },
  { // Entry 327
    -0x1.1773d561fd5080a115d796a8770d35efp-1,
    0x1.ad02c771c35ec478d333803762f450d5p-1,
    -0x1.279a74590331dp-1,
  },
  { // Entry 328
    0x1.f95b8e7107418c11c94d4a54a9da9b7ap-1,
    -0x1.48d1ddd2b2b3f8c21b9421e65b380735p-3,
    0x1.bb67ae8584ca9p0,
  },
  { // Entry 329
    -0x1.f95b8e7107418c11c94d4a54a9da9b7ap-1,
    -0x1.48d1ddd2b2b3f8c21b9421e65b380735p-3,
    -0x1.bb67ae8584ca9p0,
  },
  { // Entry 330
    0x1.f95b8e71074186ee81d5ff89d8fae545p-1,
    -0x1.48d1ddd2b2b47718ff3063b6bd981099p-3,
    0x1.bb67ae8584caap0,
  },
  { // Entry 331
    -0x1.f95b8e71074186ee81d5ff89d8fae545p-1,
    -0x1.48d1ddd2b2b47718ff3063b6bd981099p-3,
    -0x1.bb67ae8584caap0,
  },
  { // Entry 332
    0x1.f95b8e71074181cb3a5eb4bf0621d381p-1,
    -0x1.48d1ddd2b2b4f56fe2cca5871eaf4820p-3,
    0x1.bb67ae8584cabp0,
  },
  { // Entry 333
    -0x1.f95b8e71074181cb3a5eb4bf0621d381p-1,
    -0x1.48d1ddd2b2b4f56fe2cca5871eaf4820p-3,
    -0x1.bb67ae8584cabp0,
  },
  { // Entry 334
    0x1.b1d8305321615ac938cff02be9f25085p-2,
    0x1.cfc6cfa52ad9f9911db4ca0d45fdb0b3p-1,
    0x1.bffffffffffffp-2,
  },
  { // Entry 335
    -0x1.b1d8305321615ac938cff02be9f25085p-2,
    0x1.cfc6cfa52ad9f9911db4ca0d45fdb0b3p-1,
    -0x1.bffffffffffffp-2,
  },
  { // Entry 336
    0x1.b1d83053216169476f4d1982b9b14ab1p-2,
    0x1.cfc6cfa52ad9f62d6d5423ca8339a00ap-1,
    0x1.cp-2,
  },
  { // Entry 337
    -0x1.b1d83053216169476f4d1982b9b14ab1p-2,
    0x1.cfc6cfa52ad9f62d6d5423ca8339a00ap-1,
    -0x1.cp-2,
  },
  { // Entry 338
    0x1.b1d83053216177c5a5ca42d98955275ap-2,
    0x1.cfc6cfa52ad9f2c9bcf37d87c05892f5p-1,
    0x1.c000000000001p-2,
  },
  { // Entry 339
    -0x1.b1d83053216177c5a5ca42d98955275ap-2,
    0x1.cfc6cfa52ad9f2c9bcf37d87c05892f5p-1,
    -0x1.c000000000001p-2,
  },
  { // Entry 340
    0x1.44eb381cf3869ea71ccb36863e4ea65bp-1,
    0x1.8bb105a5dc90104051d08cb965631807p-1,
    0x1.5ffffffffffffp-1,
  },
  { // Entry 341
    -0x1.44eb381cf3869ea71ccb36863e4ea65bp-1,
    0x1.8bb105a5dc90104051d08cb965631807p-1,
    -0x1.5ffffffffffffp-1,
  },
  { // Entry 342
    0x1.44eb381cf386ab04a4f8656abea80b83p-1,
    0x1.8bb105a5dc900618f80fa51d303c69p-1,
    0x1.6p-1,
  },
  { // Entry 343
    -0x1.44eb381cf386ab04a4f8656abea80b83p-1,
    0x1.8bb105a5dc900618f80fa51d303c69p-1,
    -0x1.6p-1,
  },
  { // Entry 344
    0x1.44eb381cf386b7622d25944f3eb035dcp-1,
    0x1.8bb105a5dc8ffbf19e4ebd80fab2cdb8p-1,
    0x1.6000000000001p-1,
  },
  { // Entry 345
    -0x1.44eb381cf386b7622d25944f3eb035dcp-1,
    0x1.8bb105a5dc8ffbf19e4ebd80fab2cdb8p-1,
    -0x1.6000000000001p-1,
  },
  { // Entry 346
    0x1.dad902fa8ac864fd8afa0bdc609ded19p-1,
    0x1.7ef4842f0bcd11686aaf6f21c9aa8354p-2,
    0x1.2ffffffffffffp0,
  },
  { // Entry 347
    -0x1.dad902fa8ac864fd8afa0bdc609ded19p-1,
    0x1.7ef4842f0bcd11686aaf6f21c9aa8354p-2,
    -0x1.2ffffffffffffp0,
  },
  { // Entry 348
    0x1.dad902fa8ac870f52f1b843ac83bc3edp-1,
    0x1.7ef4842f0bccd60d4a501dc8bc4b57b3p-2,
    0x1.3p0,
  },
  { // Entry 349
    -0x1.dad902fa8ac870f52f1b843ac83bc3edp-1,
    0x1.7ef4842f0bccd60d4a501dc8bc4b57b3p-2,
    -0x1.3p0,
  },
  { // Entry 350
    0x1.dad902fa8ac87cecd33cfc992dfec1bep-1,
    0x1.7ef4842f0bcc9ab229f0cc6fad6d378dp-2,
    0x1.3000000000001p0,
  },
  { // Entry 351
    -0x1.dad902fa8ac87cecd33cfc992dfec1bep-1,
    0x1.7ef4842f0bcc9ab229f0cc6fad6d378dp-2,
    -0x1.3000000000001p0,
  },
  { // Entry 352
    0x1.4b707a7acdecf90a188d0230fad3ad58p-1,
    -0x1.863efa361dc2294e929b9515fb34f9bap-1,
    0x1.37fffffffffffp1,
  },
  { // Entry 353
    -0x1.4b707a7acdecf90a188d0230fad3ad58p-1,
    -0x1.863efa361dc2294e929b9515fb34f9bap-1,
    -0x1.37fffffffffffp1,
  },
  { // Entry 354
    0x1.4b707a7acdecc84239463e78b312fa10p-1,
    -0x1.863efa361dc252bca1eaeed39749bed7p-1,
    0x1.380p1,
  },
  { // Entry 355
    -0x1.4b707a7acdecc84239463e78b312fa10p-1,
    -0x1.863efa361dc252bca1eaeed39749bed7p-1,
    -0x1.380p1,
  },
  { // Entry 356
    0x1.4b707a7acdec977a59ff7ac0662484ddp-1,
    -0x1.863efa361dc27c2ab13a48912d45880bp-1,
    0x1.3800000000001p1,
  },
  { // Entry 357
    -0x1.4b707a7acdec977a59ff7ac0662484ddp-1,
    -0x1.863efa361dc27c2ab13a48912d45880bp-1,
    -0x1.3800000000001p1,
  },
  { // Entry 358
    0x1.066e7eb76f5c6678fd8325a81f1925c6p-4,
    0x1.fef2b2d21cf6c106e86ff9395f8204a0p-1,
    0x1.069c8b46b3792p-4,
  },
  { // Entry 359
    -0x1.066e7eb76f5c6678fd8325a81f1925c6p-4,
    0x1.fef2b2d21cf6c106e86ff9395f8204a0p-1,
    -0x1.069c8b46b3792p-4,
  },
  { // Entry 360
    0x1.05e4761ab8d8f0a7dba834000f236650p-3,
    0x1.fbcbe693bd8ec85723b6cb55e4f5e78fp-1,
    0x1.069c8b46b3792p-3,
  },
  { // Entry 361
    -0x1.05e4761ab8d8f0a7dba834000f236650p-3,
    0x1.fbcbe693bd8ec85723b6cb55e4f5e78fp-1,
    -0x1.069c8b46b3792p-3,
  },
  { // Entry 362
    0x1.877e2cd4f6fd9ba498e327053032734fp-3,
    0x1.f68eebfcbb5e841900e2542f7c24bab0p-1,
    0x1.89ead0ea0d35bp-3,
  },
  { // Entry 363
    -0x1.877e2cd4f6fd9ba498e327053032734fp-3,
    0x1.f68eebfcbb5e841900e2542f7c24bab0p-1,
    -0x1.89ead0ea0d35bp-3,
  },
  { // Entry 364
    0x1.03be06f97cbee47698539f977cadbe7ep-2,
    0x1.ef4145b4aecffbdaaffb78ffb49ac9bdp-1,
    0x1.069c8b46b3792p-2,
  },
  { // Entry 365
    -0x1.03be06f97cbee47698539f977cadbe7ep-2,
    0x1.ef4145b4aecffbdaaffb78ffb49ac9bdp-1,
    -0x1.069c8b46b3792p-2,
  },
  { // Entry 366
    0x1.42abba8c72fbb8ca96f79aa4bb03584ep-2,
    0x1.e5eaa286fbbc670dbf6392d7c98ab0a0p-1,
    0x1.4843ae1860576p-2,
  },
  { // Entry 367
    -0x1.42abba8c72fbb8ca96f79aa4bb03584ep-2,
    0x1.e5eaa286fbbc670dbf6392d7c98ab0a0p-1,
    -0x1.4843ae1860576p-2,
  },
  { // Entry 368
    0x1.8045fe64e62dc3d686d976d7d5a7c689p-2,
    0x1.da94d54dd4c0876e18ee97c70d1a4a94p-1,
    0x1.89ead0ea0d35ap-2,
  },
  { // Entry 369
    -0x1.8045fe64e62dc3d686d976d7d5a7c689p-2,
    0x1.da94d54dd4c0876e18ee97c70d1a4a94p-1,
    -0x1.89ead0ea0d35ap-2,
  },
  { // Entry 370
    0x1.bc4c04d71abbeea5ab064ecfbf54c613p-2,
    0x1.cd4bca9cb5c715302001e446cc93a7bcp-1,
    0x1.cb91f3bbba13ep-2,
  },
  { // Entry 371
    -0x1.bc4c04d71abbeea5ab064ecfbf54c613p-2,
    0x1.cd4bca9cb5c715302001e446cc93a7bcp-1,
    -0x1.cb91f3bbba13ep-2,
  },
  { // Entry 372
    0x1.f67ea975b86a01510e6bde3778138934p-2,
    0x1.be1d7c3534c40331fddf243d8a0a56b0p-1,
    0x1.069c8b46b3791p-1,
  },
  { // Entry 373
    -0x1.f67ea975b86a01510e6bde3778138934p-2,
    0x1.be1d7c3534c40331fddf243d8a0a56b0p-1,
    -0x1.069c8b46b3791p-1,
  },
  { // Entry 374
    0x1.175059bf0d42524ecb0bf4243b55973dp-1,
    0x1.ad19e2535aa9678bd5fdafc68817700bp-1,
    0x1.27701caf89e83p-1,
  },
  { // Entry 375
    -0x1.175059bf0d42524ecb0bf4243b55973dp-1,
    0x1.ad19e2535aa9678bd5fdafc68817700bp-1,
    -0x1.27701caf89e83p-1,
  },
  { // Entry 376
    0x1.323b8b1fb4ba21dd12cce820e156a4fcp-1,
    0x1.9a52e2e0fbcb3a3f4bde6f6ec27767a9p-1,
    0x1.4843ae1860575p-1,
  },
  { // Entry 377
    -0x1.323b8b1fb4ba21dd12cce820e156a4fcp-1,
    0x1.9a52e2e0fbcb3a3f4bde6f6ec27767a9p-1,
    -0x1.4843ae1860575p-1,
  },
  { // Entry 378
    0x1.4be4979c5efb306c1a77024032849b52p-1,
    0x1.85dc3ea1bbce9a8085f66593a87b7e2ep-1,
    0x1.69173f8136c67p-1,
  },
  { // Entry 379
    -0x1.4be4979c5efb306c1a77024032849b52p-1,
    0x1.85dc3ea1bbce9a8085f66593a87b7e2ep-1,
    -0x1.69173f8136c67p-1,
  },
  { // Entry 380
    0x1.643080d67acc1332c64a85612cacafb9p-1,
    0x1.6fcb7c6b8b919af3dda53094c9a27aabp-1,
    0x1.89ead0ea0d359p-1,
  },
  { // Entry 381
    -0x1.643080d67acc1332c64a85612cacafb9p-1,
    0x1.6fcb7c6b8b919af3dda53094c9a27aabp-1,
    -0x1.89ead0ea0d359p-1,
  },
  { // Entry 382
    0x1.7b05b7b6c612e5b08d5efe49a46e21a1p-1,
    0x1.5837d2817cf303ef6dae69faeb0f015ep-1,
    0x1.aabe6252e3a4bp-1,
  },
  { // Entry 383
    -0x1.7b05b7b6c612e5b08d5efe49a46e21a1p-1,
    0x1.5837d2817cf303ef6dae69faeb0f015ep-1,
    -0x1.aabe6252e3a4bp-1,
  },
  { // Entry 384
    0x1.904c37505de48fa8e76287960fd44594p-1,
    0x1.3f3a0e28bedd40445858f823e150264bp-1,
    0x1.cb91f3bbba13dp-1,
  },
  { // Entry 385
    -0x1.904c37505de48fa8e76287960fd44594p-1,
    0x1.3f3a0e28bedd40445858f823e150264bp-1,
    -0x1.cb91f3bbba13dp-1,
  },
  { // Entry 386
    0x1.a3ed9e252938a14c79c575639c15a91dp-1,
    0x1.24ec799171642dbd24d259005822bd25p-1,
    0x1.ec6585249082fp-1,
  },
  { // Entry 387
    -0x1.a3ed9e252938a14c79c575639c15a91dp-1,
    0x1.24ec799171642dbd24d259005822bd25p-1,
    -0x1.ec6585249082fp-1,
  },
  { // Entry 388
    0x1.b5d545b109bf935594036798cf40c9b0p-1,
    0x1.096ac02ec42c85b7b10afed9202785b9p-1,
    0x1.069c8b46b3791p0,
  },
  { // Entry 389
    -0x1.b5d545b109bf935594036798cf40c9b0p-1,
    0x1.096ac02ec42c85b7b10afed9202785b9p-1,
    -0x1.069c8b46b3791p0,
  },
  { // Entry 390
    0x1.c5f058230e7fd14d3e5e315349f699efp-1,
    0x1.d9a3a336edb7613df062e86a32d09fe1p-2,
    0x1.170653fb1eb0ap0,
  },
  { // Entry 391
    -0x1.c5f058230e7fd14d3e5e315349f699efp-1,
    0x1.d9a3a336edb7613df062e86a32d09fe1p-2,
    -0x1.170653fb1eb0ap0,
  },
  { // Entry 392
    0x1.d42de42dce1346a03d1f6abf0eba9022p-1,
    0x1.9e7f8652b47582afd29744293170c07cp-2,
    0x1.27701caf89e83p0,
  },
  { // Entry 393
    -0x1.d42de42dce1346a03d1f6abf0eba9022p-1,
    0x1.9e7f8652b47582afd29744293170c07cp-2,
    -0x1.27701caf89e83p0,
  },
  { // Entry 394
    0x1.e07eeeda109cb504afcca860d4b5dd32p-1,
    0x1.61a76077aee07bb349ca76cf700913d1p-2,
    0x1.37d9e563f51fcp0,
  },
  { // Entry 395
    -0x1.e07eeeda109cb504afcca860d4b5dd32p-1,
    0x1.61a76077aee07bb349ca76cf700913d1p-2,
    -0x1.37d9e563f51fcp0,
  },
  { // Entry 396
    0x1.ead6834909b93371faf3beaddbd60eddp-1,
    0x1.235b331d8f748e20fb6ddb6a708dba10p-2,
    0x1.4843ae1860575p0,
  },
  { // Entry 397
    -0x1.ead6834909b93371faf3beaddbd60eddp-1,
    0x1.235b331d8f748e20fb6ddb6a708dba10p-2,
    -0x1.4843ae1860575p0,
  },
  { // Entry 398
    0x1.f329c0558e967e4cab58d0fa572d62d2p-1,
    0x1.c7b90e3024593da8449963cfe08dde85p-3,
    0x1.58ad76cccb8eep0,
  },
  { // Entry 399
    -0x1.f329c0558e967e4cab58d0fa572d62d2p-1,
    0x1.c7b90e3024593da8449963cfe08dde85p-3,
    -0x1.58ad76cccb8eep0,
  },
  { // Entry 400
    0x1.f96fe405f1ac5dc9cf343508067bfcaep-1,
    0x1.46dc4f4ce83da727ea048cc7d2f276d1p-3,
    0x1.69173f8136c67p0,
  },
  { // Entry 401
    -0x1.f96fe405f1ac5dc9cf343508067bfcaep-1,
    0x1.46dc4f4ce83da727ea048cc7d2f276d1p-3,
    -0x1.69173f8136c67p0,
  },
  { // Entry 402
    0x1.fda254c27a01f4786c149d6a7779cc3ap-1,
    0x1.894f70befbb99ab7df9d1790a28f48adp-4,
    0x1.79810835a1fe0p0,
  },
  { // Entry 403
    -0x1.fda254c27a01f4786c149d6a7779cc3ap-1,
    0x1.894f70befbb99ab7df9d1790a28f48adp-4,
    -0x1.79810835a1fe0p0,
  },
  { // Entry 404
    0x1.ffbca846c4fc997f1a381420208884e0p-1,
    0x1.069107ae9332f95fa2c5ceeadfb29f77p-5,
    0x1.89ead0ea0d359p0,
  },
  { // Entry 405
    -0x1.ffbca846c4fc997f1a381420208884e0p-1,
    0x1.069107ae9332f95fa2c5ceeadfb29f77p-5,
    -0x1.89ead0ea0d359p0,
  },
  { // Entry 406
    0x1.ffbca846c4fc9f30bfb458ef2091c8eep-1,
    -0x1.069107ae9327e0731a748c21f03b5efcp-5,
    0x1.9a54999e786d2p0,
  },
  { // Entry 407
    -0x1.ffbca846c4fc9f30bfb458ef2091c8eep-1,
    -0x1.069107ae9327e0731a748c21f03b5efcp-5,
    -0x1.9a54999e786d2p0,
  },
  { // Entry 408
    0x1.fda254c27a0205875f271435f827160cp-1,
    -0x1.894f70befbb41417dff843e81fac388bp-4,
    0x1.aabe6252e3a4bp0,
  },
  { // Entry 409
    -0x1.fda254c27a0205875f271435f827160cp-1,
    -0x1.894f70befbb41417dff843e81fac388bp-4,
    -0x1.aabe6252e3a4bp0,
  },
  { // Entry 410
    0x1.f96fe405f1ac7a241e02e58b0cbf3ae7p-1,
    -0x1.46dc4f4ce83ae9ab1cc1b2367cb753ebp-3,
    0x1.bb282b074edc4p0,
  },
  { // Entry 411
    -0x1.f96fe405f1ac7a241e02e58b0cbf3ae7p-1,
    -0x1.46dc4f4ce83ae9ab1cc1b2367cb753ebp-3,
    -0x1.bb282b074edc4p0,
  },
  { // Entry 412
    0x1.f329c0558e96a5d48272ad4c49ec53b8p-1,
    -0x1.c7b90e30245688e099860e8d4fff601cp-3,
    0x1.cb91f3bbba13dp0,
  },
  { // Entry 413
    -0x1.f329c0558e96a5d48272ad4c49ec53b8p-1,
    -0x1.c7b90e30245688e099860e8d4fff601cp-3,
    -0x1.cb91f3bbba13dp0,
  },
  { // Entry 414
    0x1.ead6834909b965fdc4b0ceffc0f285c6p-1,
    -0x1.235b331d8f7339841a517312d0d347fbp-2,
    0x1.dbfbbc70254b6p0,
  },
  { // Entry 415
    -0x1.ead6834909b965fdc4b0ceffc0f285c6p-1,
    -0x1.235b331d8f7339841a517312d0d347fbp-2,
    -0x1.dbfbbc70254b6p0,
  },
  { // Entry 416
    0x1.e07eeeda109cf25f400cd5f46acec887p-1,
    -0x1.61a76077aedf2e43aca418f7a2e1324dp-2,
    0x1.ec6585249082fp0,
  },
  { // Entry 417
    -0x1.e07eeeda109cf25f400cd5f46acec887p-1,
    -0x1.61a76077aedf2e43aca418f7a2e1324dp-2,
    -0x1.ec6585249082fp0,
  },
  { // Entry 418
    0x1.d42de42dce138e890939e56c439ded90p-1,
    -0x1.9e7f8652b4743dcc3c3568baff8bf9ebp-2,
    0x1.fccf4dd8fbba8p0,
  },
  { // Entry 419
    -0x1.d42de42dce138e890939e56c439ded90p-1,
    -0x1.9e7f8652b4743dcc3c3568baff8bf9ebp-2,
    -0x1.fccf4dd8fbba8p0,
  },
  { // Entry 420
    0x1.c5f058230e8014ab83ece0c3a638c079p-1,
    -0x1.d9a3a336edb65efa30e1a6679aa064c2p-2,
    0x1.069c8b46b3791p1,
  },
  { // Entry 421
    -0x1.c5f058230e8014ab83ece0c3a638c079p-1,
    -0x1.d9a3a336edb65efa30e1a6679aa064c2p-2,
    -0x1.069c8b46b3791p1,
  },
  { // Entry 422
    0x1.b5d545b109bfce3fc4d77001afe2f2b6p-1,
    -0x1.096ac02ec42c24880a5951788cb383c8p-1,
    0x1.0ed16fa0e914ep1,
  },
  { // Entry 423
    -0x1.b5d545b109bfce3fc4d77001afe2f2b6p-1,
    -0x1.096ac02ec42c24880a5951788cb383c8p-1,
    -0x1.0ed16fa0e914ep1,
  },
  { // Entry 424
    0x1.a3ed9e252938d92a5553b3c09d2bddd3p-1,
    -0x1.24ec79917163dda65afd8109f59cb465p-1,
    0x1.170653fb1eb0bp1,
  },
  { // Entry 425
    -0x1.a3ed9e252938d92a5553b3c09d2bddd3p-1,
    -0x1.24ec79917163dda65afd8109f59cb465p-1,
    -0x1.170653fb1eb0bp1,
  },
  { // Entry 426
    0x1.904c37505de4b8975dd2730e196ddfc3p-1,
    -0x1.3f3a0e28bedd0cf0c4bfbd8c82a3baafp-1,
    0x1.1f3b3855544c8p1,
  },
  { // Entry 427
    -0x1.904c37505de4b8975dd2730e196ddfc3p-1,
    -0x1.3f3a0e28bedd0cf0c4bfbd8c82a3baafp-1,
    -0x1.1f3b3855544c8p1,
  },
  { // Entry 428
    0x1.7b05b7b6c612fc4fda3812b1f1348389p-1,
    -0x1.5837d2817cf2eb069035552dc3ae834cp-1,
    0x1.27701caf89e85p1,
  },
  { // Entry 429
    -0x1.7b05b7b6c612fc4fda3812b1f1348389p-1,
    -0x1.5837d2817cf2eb069035552dc3ae834cp-1,
    -0x1.27701caf89e85p1,
  },
  { // Entry 430
    0x1.643080d67acc14620672dda6241ea305p-1,
    -0x1.6fcb7c6b8b9199ce2f17dd3ee86b3b9ap-1,
    0x1.2fa50109bf842p1,
  },
  { // Entry 431
    -0x1.643080d67acc14620672dda6241ea305p-1,
    -0x1.6fcb7c6b8b9199ce2f17dd3ee86b3b9ap-1,
    -0x1.2fa50109bf842p1,
  },
  { // Entry 432
    0x1.4be4979c5efb194fc82ac367fedf93bcp-1,
    -0x1.85dc3ea1bbceae2d294421e8c7350f8cp-1,
    0x1.37d9e563f51ffp1,
  },
  { // Entry 433
    -0x1.4be4979c5efb194fc82ac367fedf93bcp-1,
    -0x1.85dc3ea1bbceae2d294421e8c7350f8cp-1,
    -0x1.37d9e563f51ffp1,
  },
  { // Entry 434
    0x1.323b8b1fb4b9efe5075ede8049a85c3dp-1,
    -0x1.9a52e2e0fbcb5f8a3f55c274f9ec754bp-1,
    0x1.400ec9be2abbcp1,
  },
  { // Entry 435
    -0x1.323b8b1fb4b9efe5075ede8049a85c3dp-1,
    -0x1.9a52e2e0fbcb5f8a3f55c274f9ec754bp-1,
    -0x1.400ec9be2abbcp1,
  },
  { // Entry 436
    0x1.175059bf0d42033bbcf598c88b176e61p-1,
    -0x1.ad19e2535aa99b049ac0b5858c5d381fp-1,
    0x1.4843ae1860579p1,
  },
  { // Entry 437
    -0x1.175059bf0d42033bbcf598c88b176e61p-1,
    -0x1.ad19e2535aa99b049ac0b5858c5d381fp-1,
    -0x1.4843ae1860579p1,
  },
  { // Entry 438
    0x1.f67ea975b8692521f77d6754b302c5c4p-2,
    -0x1.be1d7c3534c44132ab1c4130cbe9dfa0p-1,
    0x1.5078927295f36p1,
  },
  { // Entry 439
    -0x1.f67ea975b8692521f77d6754b302c5c4p-2,
    -0x1.be1d7c3534c44132ab1c4130cbe9dfa0p-1,
    -0x1.5078927295f36p1,
  },
  { // Entry 440
    0x1.bc4c04d71abad14efc29a66342ada723p-2,
    -0x1.cd4bca9cb5c759e4d6dc8601ec3d84b6p-1,
    0x1.58ad76cccb8f3p1,
  },
  { // Entry 441
    -0x1.bc4c04d71abad14efc29a66342ada723p-2,
    -0x1.cd4bca9cb5c759e4d6dc8601ec3d84b6p-1,
    -0x1.58ad76cccb8f3p1,
  },
  { // Entry 442
    0x1.8045fe64e62c62f57f077ea251e2f2dcp-2,
    -0x1.da94d54dd4c0cedccd73684994422740p-1,
    0x1.60e25b27012b0p1,
  },
  { // Entry 443
    -0x1.8045fe64e62c62f57f077ea251e2f2dcp-2,
    -0x1.da94d54dd4c0cedccd73684994422740p-1,
    -0x1.60e25b27012b0p1,
  },
  { // Entry 444
    0x1.42abba8c72fa12be920b316627512e41p-2,
    -0x1.e5eaa286fbbcad1e4a6373392e679669p-1,
    0x1.69173f8136c6dp1,
  },
  { // Entry 445
    -0x1.42abba8c72fa12be920b316627512e41p-2,
    -0x1.e5eaa286fbbcad1e4a6373392e679669p-1,
    -0x1.69173f8136c6dp1,
  },
  { // Entry 446
    0x1.03be06f97cbcf866021e5a5c62c6b07ep-2,
    -0x1.ef4145b4aed03c5f1d39763b1eee6ed8p-1,
    0x1.714c23db6c62ap1,
  },
  { // Entry 447
    -0x1.03be06f97cbcf866021e5a5c62c6b07ep-2,
    -0x1.ef4145b4aed03c5f1d39763b1eee6ed8p-1,
    -0x1.714c23db6c62ap1,
  },
  { // Entry 448
    0x1.877e2cd4f6f94710f2776775b01c73dbp-3,
    -0x1.f68eebfcbb5eba124d8cc48fd1beb04dp-1,
    0x1.79810835a1fe7p1,
  },
  { // Entry 449
    -0x1.877e2cd4f6f94710f2776775b01c73dbp-3,
    -0x1.f68eebfcbb5eba124d8cc48fd1beb04dp-1,
    -0x1.79810835a1fe7p1,
  },
  { // Entry 450
    0x1.05e4761ab8d421719567717f76712867p-3,
    -0x1.fbcbe693bd8ef006f5ff02210dfe0619p-1,
    0x1.81b5ec8fd79a4p1,
  },
  { // Entry 451
    -0x1.05e4761ab8d421719567717f76712867p-3,
    -0x1.fbcbe693bd8ef006f5ff02210dfe0619p-1,
    -0x1.81b5ec8fd79a4p1,
  },
  { // Entry 452
    0x1.066e7eb76f5dd2ea19b6991e8a1a3634p-4,
    -0x1.fef2b2d21cf6be1a2c7ea665ef1f874ep-1,
    0x1.89ead0ea0d35bp1,
  },
  { // Entry 453
    -0x1.066e7eb76f5dd2ea19b6991e8a1a3634p-4,
    -0x1.fef2b2d21cf6be1a2c7ea665ef1f874ep-1,
    -0x1.89ead0ea0d35bp1,
  },
  { // Entry 454
    0x1.03be06f97cbf09cc0badbdae803d7b4ep-2,
    0x1.ef4145b4aecff6f58edecf24955428c1p-1,
    -0x1.81b5ec8fd799fp2,
  },
  { // Entry 455
    -0x1.03be06f97cbf09cc0badbdae803d7b4ep-2,
    0x1.ef4145b4aecff6f58edecf24955428c1p-1,
    0x1.81b5ec8fd799fp2,
  },
  { // Entry 456
    0x1.f67ea975b86a22f2348778824f95d84ap-2,
    0x1.be1d7c3534c3f9b9b35619280049de85p-1,
    -0x1.714c23db6c626p2,
  },
  { // Entry 457
    -0x1.f67ea975b86a22f2348778824f95d84ap-2,
    0x1.be1d7c3534c3f9b9b35619280049de85p-1,
    0x1.714c23db6c626p2,
  },
  { // Entry 458
    0x1.643080d67acc210fa27e9247a8286220p-1,
    0x1.6fcb7c6b8b918d86fc83d612a6587eddp-1,
    -0x1.60e25b27012adp2,
  },
  { // Entry 459
    -0x1.643080d67acc210fa27e9247a8286220p-1,
    0x1.6fcb7c6b8b918d86fc83d612a6587eddp-1,
    0x1.60e25b27012adp2,
  },
  { // Entry 460
    0x1.b5d545b109bf950b419702972b94f8fap-1,
    0x1.096ac02ec42c82e5b225185bd6c757d5p-1,
    -0x1.5078927295f34p2,
  },
  { // Entry 461
    -0x1.b5d545b109bf950b419702972b94f8fap-1,
    0x1.096ac02ec42c82e5b225185bd6c757d5p-1,
    0x1.5078927295f34p2,
  },
  { // Entry 462
    0x1.ead6834909b9346234dbb601d0486cf2p-1,
    0x1.235b331d8f7487ce2db97819fae7777cp-2,
    -0x1.400ec9be2abbbp2,
  },
  { // Entry 463
    -0x1.ead6834909b9346234dbb601d0486cf2p-1,
    0x1.235b331d8f7487ce2db97819fae7777cp-2,
    0x1.400ec9be2abbbp2,
  },
  { // Entry 464
    0x1.ffbca846c4fc999a29dc1d6b2d7cb413p-1,
    0x1.069107ae9332c4a1cd2dc033b8d50598p-5,
    -0x1.2fa50109bf842p2,
  },
  { // Entry 465
    -0x1.ffbca846c4fc999a29dc1d6b2d7cb413p-1,
    0x1.069107ae9332c4a1cd2dc033b8d50598p-5,
    0x1.2fa50109bf842p2,
  },
  { // Entry 466
    0x1.f329c0558e96a518a2af3ae7800a5b65p-1,
    -0x1.c7b90e30245695bd1ec170f45feeb1ffp-3,
    -0x1.1f3b3855544c9p2,
  },
  { // Entry 467
    -0x1.f329c0558e96a518a2af3ae7800a5b65p-1,
    -0x1.c7b90e30245695bd1ec170f45feeb1ffp-3,
    0x1.1f3b3855544c9p2,
  },
  { // Entry 468
    0x1.c5f058230e8021f21bd0ac2c0f6809a9p-1,
    -0x1.d9a3a336edb62c1541b8584cd6c00f87p-2,
    -0x1.0ed16fa0e9150p2,
  },
  { // Entry 469
    -0x1.c5f058230e8021f21bd0ac2c0f6809a9p-1,
    -0x1.d9a3a336edb62c1541b8584cd6c00f87p-2,
    0x1.0ed16fa0e9150p2,
  },
  { // Entry 470
    0x1.7b05b7b6c61365a9ac9e908b8e5d3ce4p-1,
    -0x1.5837d2817cf27705cac7881fb569ffc7p-1,
    -0x1.fccf4dd8fbbaep1,
  },
  { // Entry 471
    -0x1.7b05b7b6c61365a9ac9e908b8e5d3ce4p-1,
    -0x1.5837d2817cf27705cac7881fb569ffc7p-1,
    0x1.fccf4dd8fbbaep1,
  },
  { // Entry 472
    0x1.175059bf0d42f1d6b391f07f96f2353dp-1,
    -0x1.ad19e2535aa8ffb40066d78aef71fabdp-1,
    -0x1.dbfbbc70254bcp1,
  },
  { // Entry 473
    -0x1.175059bf0d42f1d6b391f07f96f2353dp-1,
    -0x1.ad19e2535aa8ffb40066d78aef71fabdp-1,
    0x1.dbfbbc70254bcp1,
  },
  { // Entry 474
    0x1.42abba8c72fd22194793246b8d19960ap-2,
    -0x1.e5eaa286fbbc2b129238160df30ce704p-1,
    -0x1.bb282b074edcap1,
  },
  { // Entry 475
    -0x1.42abba8c72fd22194793246b8d19960ap-2,
    -0x1.e5eaa286fbbc2b129238160df30ce704p-1,
    0x1.bb282b074edcap1,
  },
  { // Entry 476
    0x1.066e7eb76f62b5f4563de26dca890017p-4,
    -0x1.fef2b2d21cf6b40ff3b530ce8dc0d8a7p-1,
    -0x1.9a54999e786d8p1,
  },
  { // Entry 477
    -0x1.066e7eb76f62b5f4563de26dca890017p-4,
    -0x1.fef2b2d21cf6b40ff3b530ce8dc0d8a7p-1,
    0x1.9a54999e786d8p1,
  },
  { // Entry 478
    -0x1.877e2cd4f6fa42586875c5250a169e48p-3,
    -0x1.f68eebfcbb5eadd65c261cd803990ae1p-1,
    -0x1.79810835a1fe6p1,
  },
  { // Entry 479
    0x1.877e2cd4f6fa42586875c5250a169e48p-3,
    -0x1.f68eebfcbb5eadd65c261cd803990ae1p-1,
    0x1.79810835a1fe6p1,
  },
  { // Entry 480
    -0x1.bc4c04d71aba5dfc098278f168bbd962p-2,
    -0x1.cd4bca9cb5c775a99729f7ad95b7dce3p-1,
    -0x1.58ad76cccb8f4p1,
  },
  { // Entry 481
    0x1.bc4c04d71aba5dfc098278f168bbd962p-2,
    -0x1.cd4bca9cb5c775a99729f7ad95b7dce3p-1,
    0x1.58ad76cccb8f4p1,
  },
  { // Entry 482
    -0x1.4be4979c5efa871d30ae1cfa66389199p-1,
    -0x1.85dc3ea1bbcf2aa2e21ec586d5497e35p-1,
    -0x1.37d9e563f5202p1,
  },
  { // Entry 483
    0x1.4be4979c5efa871d30ae1cfa66389199p-1,
    -0x1.85dc3ea1bbcf2aa2e21ec586d5497e35p-1,
    0x1.37d9e563f5202p1,
  },
  { // Entry 484
    -0x1.a3ed9e25293822168958cce1e09f7c11p-1,
    -0x1.24ec79917164e41addd4bacd4420f9fbp-1,
    -0x1.170653fb1eb10p1,
  },
  { // Entry 485
    0x1.a3ed9e25293822168958cce1e09f7c11p-1,
    -0x1.24ec79917164e41addd4bacd4420f9fbp-1,
    0x1.170653fb1eb10p1,
  },
  { // Entry 486
    -0x1.e07eeeda109c62b340dc36e92169648dp-1,
    -0x1.61a76077aee23b11f0c673f638003b0ap-2,
    -0x1.ec6585249083cp0,
  },
  { // Entry 487
    0x1.e07eeeda109c62b340dc36e92169648dp-1,
    -0x1.61a76077aee23b11f0c673f638003b0ap-2,
    0x1.ec6585249083cp0,
  },
  { // Entry 488
    -0x1.fda254c27a01dd954db3aea505e49453p-1,
    -0x1.894f70befbc104b706e85cf4c1c96a52p-4,
    -0x1.aabe6252e3a58p0,
  },
  { // Entry 489
    0x1.fda254c27a01dd954db3aea505e49453p-1,
    -0x1.894f70befbc104b706e85cf4c1c96a52p-4,
    0x1.aabe6252e3a58p0,
  },
  { // Entry 490
    -0x1.f96fe405f1aca02e8f4fd433e59aa973p-1,
    0x1.46dc4f4ce8373c7c44f13b57363edd3bp-3,
    -0x1.69173f8136c74p0,
  },
  { // Entry 491
    0x1.f96fe405f1aca02e8f4fd433e59aa973p-1,
    0x1.46dc4f4ce8373c7c44f13b57363edd3bp-3,
    0x1.69173f8136c74p0,
  },
  { // Entry 492
    -0x1.d42de42dce13ef040bb1040e3148d7dep-1,
    0x1.9e7f8652b47289e53fccd54955db4552p-2,
    -0x1.27701caf89e90p0,
  },
  { // Entry 493
    0x1.d42de42dce13ef040bb1040e3148d7dep-1,
    0x1.9e7f8652b47289e53fccd54955db4552p-2,
    0x1.27701caf89e90p0,
  },
  { // Entry 494
    -0x1.904c37505de5930812e3a2a94feaa51bp-1,
    0x1.3f3a0e28bedbfb066b67abd9c338409ep-1,
    -0x1.cb91f3bbba157p-1,
  },
  { // Entry 495
    0x1.904c37505de5930812e3a2a94feaa51bp-1,
    0x1.3f3a0e28bedbfb066b67abd9c338409ep-1,
    0x1.cb91f3bbba157p-1,
  },
  { // Entry 496
    -0x1.323b8b1fb4bb626dd40cacd74963ac6cp-1,
    0x1.9a52e2e0fbca4b00c72daa3cdaca257cp-1,
    -0x1.4843ae186058ep-1,
  },
  { // Entry 497
    0x1.323b8b1fb4bb626dd40cacd74963ac6cp-1,
    0x1.9a52e2e0fbca4b00c72daa3cdaca257cp-1,
    0x1.4843ae186058ep-1,
  },
  { // Entry 498
    -0x1.8045fe64e6308bb5c6ce35f834b93c63p-2,
    0x1.da94d54dd4bff753d988c1755e2ffc04p-1,
    -0x1.89ead0ea0d38ap-2,
  },
  { // Entry 499
    0x1.8045fe64e6308bb5c6ce35f834b93c63p-2,
    0x1.da94d54dd4bff753d988c1755e2ffc04p-1,
    0x1.89ead0ea0d38ap-2,
  },
  { // Entry 500
    -0x1.05e4761ab8dec44ed0fa30d335049c40p-3,
    0x1.fbcbe693bd8e98423207e36587d942b7p-1,
    -0x1.069c8b46b37f0p-3,
  },
  { // Entry 501
    0x1.05e4761ab8dec44ed0fa30d335049c40p-3,
    0x1.fbcbe693bd8e98423207e36587d942b7p-1,
    0x1.069c8b46b37f0p-3,
  },
  { // Entry 502
    0x1.05e4761ab8d31d00e656372c5c04aa6ep-3,
    0x1.fbcbe693bd8ef86c1565b3453036e55ep-1,
    0x1.069c8b46b3734p-3,
  },
  { // Entry 503
    -0x1.05e4761ab8d31d00e656372c5c04aa6ep-3,
    0x1.fbcbe693bd8ef86c1565b3453036e55ep-1,
    -0x1.069c8b46b3734p-3,
  },
  { // Entry 504
    0x1.8045fe64e62b19a094399502afb76e5cp-2,
    0x1.da94d54dd4c11187405ada7f04e5b171p-1,
    0x1.89ead0ea0d32cp-2,
  },
  { // Entry 505
    -0x1.8045fe64e62b19a094399502afb76e5cp-2,
    0x1.da94d54dd4c11187405ada7f04e5b171p-1,
    -0x1.89ead0ea0d32cp-2,
  },
  { // Entry 506
    0x1.323b8b1fb4b907c416d23b04e0ec0e72p-1,
    0x1.9a52e2e0fbcc0cc83b843bae58c6cdf8p-1,
    0x1.4843ae186055fp-1,
  },
  { // Entry 507
    -0x1.323b8b1fb4b907c416d23b04e0ec0e72p-1,
    0x1.9a52e2e0fbcc0cc83b843bae58c6cdf8p-1,
    -0x1.4843ae186055fp-1,
  },
  { // Entry 508
    0x1.904c37505de3be2ace17ca5487750231p-1,
    0x1.3f3a0e28bede46f65ca5b5c19ad99dd7p-1,
    0x1.cb91f3bbba128p-1,
  },
  { // Entry 509
    -0x1.904c37505de3be2ace17ca5487750231p-1,
    0x1.3f3a0e28bede46f65ca5b5c19ad99dd7p-1,
    -0x1.cb91f3bbba128p-1,
  },
  { // Entry 510
    0x1.d42de42dce12b82466f2fcb63b294751p-1,
    0x1.9e7f8652b478066eec563f835097f148p-2,
    0x1.27701caf89e78p0,
  },
  { // Entry 511
    -0x1.d42de42dce12b82466f2fcb63b294751p-1,
    0x1.9e7f8652b478066eec563f835097f148p-2,
    -0x1.27701caf89e78p0,
  },
  { // Entry 512
    0x1.f96fe405f1ac259bf192fd1cf64e2f12p-1,
    0x1.46dc4f4ce843151b9d14e561879e5fe3p-3,
    0x1.69173f8136c5cp0,
  },
  { // Entry 513
    -0x1.f96fe405f1ac259bf192fd1cf64e2f12p-1,
    0x1.46dc4f4ce843151b9d14e561879e5fe3p-3,
    -0x1.69173f8136c5cp0,
  },
  { // Entry 514
    0x1.fda254c27a02275432d77dd6f9704644p-1,
    -0x1.894f70befba9211b0dcaa4dca450670fp-4,
    0x1.aabe6252e3a40p0,
  },
  { // Entry 515
    -0x1.fda254c27a02275432d77dd6f9704644p-1,
    -0x1.894f70befba9211b0dcaa4dca450670fp-4,
    -0x1.aabe6252e3a40p0,
  },
  { // Entry 516
    0x1.e07eeeda109d6bf0c935fa10b1280c6dp-1,
    -0x1.61a76077aedc99952438421f820a2befp-2,
    0x1.ec65852490824p0,
  },
  { // Entry 517
    -0x1.e07eeeda109d6bf0c935fa10b1280c6dp-1,
    -0x1.61a76077aedc99952438421f820a2befp-2,
    -0x1.ec65852490824p0,
  },
  { // Entry 518
    0x1.a3ed9e252939d9793fb2f6f75e5c76e7p-1,
    -0x1.24ec799171626e36709cfcf7c7752332p-1,
    0x1.170653fb1eb04p1,
  },
  { // Entry 519
    -0x1.a3ed9e252939d9793fb2f6f75e5c76e7p-1,
    -0x1.24ec799171626e36709cfcf7c7752332p-1,
    -0x1.170653fb1eb04p1,
  },
  { // Entry 520
    0x1.4be4979c5efccfe78ea0b6afb0cbba37p-1,
    -0x1.85dc3ea1bbcd38cbfeb4370d5405eebap-1,
    0x1.37d9e563f51f6p1,
  },
  { // Entry 521
    -0x1.4be4979c5efccfe78ea0b6afb0cbba37p-1,
    -0x1.85dc3ea1bbcd38cbfeb4370d5405eebap-1,
    -0x1.37d9e563f51f6p1,
  },
  { // Entry 522
    0x1.bc4c04d71abfc5df69589a45d5e3196ep-2,
    -0x1.cd4bca9cb5c628709388a39fc84591d1p-1,
    0x1.58ad76cccb8e8p1,
  },
  { // Entry 523
    -0x1.bc4c04d71abfc5df69589a45d5e3196ep-2,
    -0x1.cd4bca9cb5c628709388a39fc84591d1p-1,
    -0x1.58ad76cccb8e8p1,
  },
  { // Entry 524
    0x1.877e2cd4f70609b1f062295b64aed4bdp-3,
    -0x1.f68eebfcbb5e1b070b564037f5571a39p-1,
    0x1.79810835a1fdap1,
  },
  { // Entry 525
    -0x1.877e2cd4f70609b1f062295b64aed4bdp-3,
    -0x1.f68eebfcbb5e1b070b564037f5571a39p-1,
    -0x1.79810835a1fdap1,
  },
  { // Entry 526
    -0x1.066e7eb76f4ac293f46486dc328d450bp-4,
    -0x1.fef2b2d21cf6e544ab7795aed10d9fa7p-1,
    0x1.9a54999e786ccp1,
  },
  { // Entry 527
    0x1.066e7eb76f4ac293f46486dc328d450bp-4,
    -0x1.fef2b2d21cf6e544ab7795aed10d9fa7p-1,
    -0x1.9a54999e786ccp1,
  },
  { // Entry 528
    -0x1.42abba8c72f770595ffe3135a0e0ad83p-2,
    -0x1.e5eaa286fbbd1d135e216c49a9f7e5dap-1,
    0x1.bb282b074edbep1,
  },
  { // Entry 529
    0x1.42abba8c72f770595ffe3135a0e0ad83p-2,
    -0x1.e5eaa286fbbd1d135e216c49a9f7e5dap-1,
    -0x1.bb282b074edbep1,
  },
  { // Entry 530
    -0x1.175059bf0d406e2fe014e880dd29cfacp-1,
    -0x1.ad19e2535aaaa2ac87056b6d7776e97ap-1,
    0x1.dbfbbc70254b0p1,
  },
  { // Entry 531
    0x1.175059bf0d406e2fe014e880dd29cfacp-1,
    -0x1.ad19e2535aaaa2ac87056b6d7776e97ap-1,
    -0x1.dbfbbc70254b0p1,
  },
  { // Entry 532
    -0x1.7b05b7b6c6116155f0dc551e316e1e0bp-1,
    -0x1.5837d2817cf4af8e5e59b13b4aa9b5e3p-1,
    0x1.fccf4dd8fbba2p1,
  },
  { // Entry 533
    0x1.7b05b7b6c6116155f0dc551e316e1e0bp-1,
    -0x1.5837d2817cf4af8e5e59b13b4aa9b5e3p-1,
    -0x1.fccf4dd8fbba2p1,
  },
  { // Entry 534
    -0x1.c5f058230e7ebeb7616779e16fa9b537p-1,
    -0x1.d9a3a336edbb7de64a2183cb27be4b5bp-2,
    0x1.0ed16fa0e914ap2,
  },
  { // Entry 535
    0x1.c5f058230e7ebeb7616779e16fa9b537p-1,
    -0x1.d9a3a336edbb7de64a2183cb27be4b5bp-2,
    -0x1.0ed16fa0e914ap2,
  },
  { // Entry 536
    -0x1.f329c0558e95fa333d5d2d44d654777cp-1,
    -0x1.c7b90e30246248b7a0c2c87a3dd25224p-3,
    0x1.1f3b3855544c3p2,
  },
  { // Entry 537
    0x1.f329c0558e95fa333d5d2d44d654777cp-1,
    -0x1.c7b90e30246248b7a0c2c87a3dd25224p-3,
    -0x1.1f3b3855544c3p2,
  },
  { // Entry 538
    -0x1.ffbca846c4fcb237c2947b35b037a2p-1,
    0x1.069107ae9302caf2068b48842afdf051p-5,
    0x1.2fa50109bf83cp2,
  },
  { // Entry 539
    0x1.ffbca846c4fcb237c2947b35b037a2p-1,
    0x1.069107ae9302caf2068b48842afdf051p-5,
    -0x1.2fa50109bf83cp2,
  },
  { // Entry 540
    -0x1.ead6834909ba0ee69b31e1970df1bb8bp-1,
    0x1.235b331d8f6ec74aa3de5aed15fa3f68p-2,
    0x1.400ec9be2abb5p2,
  },
  { // Entry 541
    0x1.ead6834909ba0ee69b31e1970df1bb8bp-1,
    0x1.235b331d8f6ec74aa3de5aed15fa3f68p-2,
    -0x1.400ec9be2abb5p2,
  },
  { // Entry 542
    -0x1.b5d545b109c1232b61dd28d8035d95cbp-1,
    0x1.096ac02ec429f225c99b89bb4c9e5d3ep-1,
    0x1.5078927295f2ep2,
  },
  { // Entry 543
    0x1.b5d545b109c1232b61dd28d8035d95cbp-1,
    0x1.096ac02ec429f225c99b89bb4c9e5d3ep-1,
    -0x1.5078927295f2ep2,
  },
  { // Entry 544
    -0x1.643080d67ace48c0dd1fe3a06bbc4bf5p-1,
    0x1.6fcb7c6b8b8f773e3b421dded6fc1f26p-1,
    0x1.60e25b27012a7p2,
  },
  { // Entry 545
    0x1.643080d67ace48c0dd1fe3a06bbc4bf5p-1,
    0x1.6fcb7c6b8b8f773e3b421dded6fc1f26p-1,
    -0x1.60e25b27012a7p2,
  },
  { // Entry 546
    -0x1.f67ea975b86f5d4aa92716cc077473a7p-2,
    0x1.be1d7c3534c280dab43dced670330b63p-1,
    0x1.714c23db6c620p2,
  },
  { // Entry 547
    0x1.f67ea975b86f5d4aa92716cc077473a7p-2,
    0x1.be1d7c3534c280dab43dced670330b63p-1,
    -0x1.714c23db6c620p2,
  },
  { // Entry 548
    -0x1.03be06f97cc4d78fdccbca1d40e86011p-2,
    0x1.ef4145b4aecf342709a3b19320d1b194p-1,
    0x1.81b5ec8fd7999p2,
  },
  { // Entry 549
    0x1.03be06f97cc4d78fdccbca1d40e86011p-2,
    0x1.ef4145b4aecf342709a3b19320d1b194p-1,
    -0x1.81b5ec8fd7999p2,
  },
  { // Entry 550
    0x1.efb26ef930c4c3fa3245963c1dcec0a6p-5,
    0x1.ff0fd2c96adfbae576981ee4b34769dep-1,
    0x1.effffffffffffp-5,
  },
  { // Entry 551
    -0x1.efb26ef930c4c3fa3245963c1dcec0a6p-5,
    0x1.ff0fd2c96adfbae576981ee4b34769dep-1,
    -0x1.effffffffffffp-5,
  },
  { // Entry 552
    0x1.efb26ef930c4d3f2b0dbe1931ba5ae64p-5,
    0x1.ff0fd2c96adfbad5f904a71b2d210a2ap-1,
    0x1.fp-5,
  },
  { // Entry 553
    -0x1.efb26ef930c4d3f2b0dbe1931ba5ae64p-5,
    0x1.ff0fd2c96adfbad5f904a71b2d210a2ap-1,
    -0x1.fp-5,
  },
  { // Entry 554
    0x1.efb26ef930c4e3eb2f722cea197c2036p-5,
    0x1.ff0fd2c96adfbac67b712f51a6fa2ab3p-1,
    0x1.f000000000001p-5,
  },
  { // Entry 555
    -0x1.efb26ef930c4e3eb2f722cea197c2036p-5,
    0x1.ff0fd2c96adfbac67b712f51a6fa2ab3p-1,
    -0x1.f000000000001p-5,
  },
  { // Entry 556
    0x1.f6baaa131de633ad4e0e7d6465d12a05p-4,
    0x1.fc210055467fe5c8f76e75fd7083818cp-1,
    0x1.f7fffffffffffp-4,
  },
  { // Entry 557
    -0x1.f6baaa131de633ad4e0e7d6465d12a05p-4,
    0x1.fc210055467fe5c8f76e75fd7083818cp-1,
    -0x1.f7fffffffffffp-4,
  },
  { // Entry 558
    0x1.f6baaa131de6438e5611279864fe7663p-4,
    0x1.fc210055467fe58a20193399b3bc0dd2p-1,
    0x1.f80p-4,
  },
  { // Entry 559
    -0x1.f6baaa131de6438e5611279864fe7663p-4,
    0x1.fc210055467fe58a20193399b3bc0dd2p-1,
    -0x1.f80p-4,
  },
  { // Entry 560
    0x1.f6baaa131de6536f5e13d1cc6429cc07p-4,
    0x1.fc210055467fe54b48c3f135f6f29df7p-1,
    0x1.f800000000001p-4,
  },
  { // Entry 561
    -0x1.f6baaa131de6536f5e13d1cc6429cc07p-4,
    0x1.fc210055467fe54b48c3f135f6f29df7p-1,
    -0x1.f800000000001p-4,
  },
  { // Entry 562
    0x1.4a8c3b4e9c7ff00a36e061a0d2295093p-3,
    0x1.f94984b2552e19e7329413b8c2e8dc51p-1,
    0x1.4bfffffffffffp-3,
  },
  { // Entry 563
    -0x1.4a8c3b4e9c7ff00a36e061a0d2295093p-3,
    0x1.f94984b2552e19e7329413b8c2e8dc51p-1,
    -0x1.4bfffffffffffp-3,
  },
  { // Entry 564
    0x1.4a8c3b4e9c7fffd48305f44a42f5f50fp-3,
    0x1.f94984b2552e1941ec766c6a82ece4a3p-1,
    0x1.4c0p-3,
  },
  { // Entry 565
    -0x1.4a8c3b4e9c7fffd48305f44a42f5f50fp-3,
    0x1.f94984b2552e1941ec766c6a82ece4a3p-1,
    -0x1.4c0p-3,
  },
  { // Entry 566
    0x1.4a8c3b4e9c800f9ecf2b86f3b3bd6f5ap-3,
    0x1.f94984b2552e189ca658c51c42e907cep-1,
    0x1.4c00000000001p-3,
  },
  { // Entry 567
    -0x1.4a8c3b4e9c800f9ecf2b86f3b3bd6f5ap-3,
    0x1.f94984b2552e189ca658c51c42e907cep-1,
    -0x1.4c00000000001p-3,
  },
  { // Entry 568
    0x1.2e9cd95baba325fe6067233d4496aaacp-2,
    0x1.e921dd42f09ba868603ea376f6e2d012p-1,
    0x1.3333333333332p-2,
  },
  { // Entry 569
    -0x1.2e9cd95baba325fe6067233d4496aaacp-2,
    0x1.e921dd42f09ba868603ea376f6e2d012p-1,
    -0x1.3333333333332p-2,
  },
  { // Entry 570
    0x1.2e9cd95baba335476f513ac221d078c7p-2,
    0x1.e921dd42f09ba60b268bec1fb0878a42p-1,
    0x1.3333333333333p-2,
  },
  { // Entry 571
    -0x1.2e9cd95baba335476f513ac221d078c7p-2,
    0x1.e921dd42f09ba60b268bec1fb0878a42p-1,
    -0x1.3333333333333p-2,
  },
  { // Entry 572
    0x1.2e9cd95baba344907e3b5246fef75d15p-2,
    0x1.e921dd42f09ba3adecd934c86a0db254p-1,
    0x1.3333333333334p-2,
  },
  { // Entry 573
    -0x1.2e9cd95baba344907e3b5246fef75d15p-2,
    0x1.e921dd42f09ba3adecd934c86a0db254p-1,
    -0x1.3333333333334p-2,
  },
  { // Entry 574
    0x1.3faefc7a5466ef3045c3f1be716ad568p-1,
    0x1.8feedb86bf0ef3158f8a1dcbef49d123p-1,
    0x1.594317acc4ef8p-1,
  },
  { // Entry 575
    -0x1.3faefc7a5466ef3045c3f1be716ad568p-1,
    0x1.8feedb86bf0ef3158f8a1dcbef49d123p-1,
    -0x1.594317acc4ef8p-1,
  },
  { // Entry 576
    0x1.3faefc7a5466fbafbca027b6e8db8c04p-1,
    0x1.8feedb86bf0ee91817a64b28b79e5119p-1,
    0x1.594317acc4ef9p-1,
  },
  { // Entry 577
    -0x1.3faefc7a5466fbafbca027b6e8db8c04p-1,
    0x1.8feedb86bf0ee91817a64b28b79e5119p-1,
    -0x1.594317acc4ef9p-1,
  },
  { // Entry 578
    0x1.3faefc7a5467082f337c5daf5ffc56e2p-1,
    0x1.8feedb86bf0edf1a9fc278857f8ed559p-1,
    0x1.594317acc4efap-1,
  },
  { // Entry 579
    -0x1.3faefc7a5467082f337c5daf5ffc56e2p-1,
    0x1.8feedb86bf0edf1a9fc278857f8ed559p-1,
    -0x1.594317acc4efap-1,
  },
  { // Entry 580
    0x1.6888a4e134b2def5bea51f4cd7d647c6p-1,
    0x1.6b898fa9efb5dd6f9e17e3442d59b241p-1,
    0x1.8ffffffffffffp-1,
  },
  { // Entry 581
    -0x1.6888a4e134b2def5bea51f4cd7d647c6p-1,
    0x1.6b898fa9efb5dd6f9e17e3442d59b241p-1,
    -0x1.8ffffffffffffp-1,
  },
  { // Entry 582
    0x1.6888a4e134b2ea520b226eca8694b3a2p-1,
    0x1.6b898fa9efb5d22b58f0d99e9634931ap-1,
    0x1.9p-1,
  },
  { // Entry 583
    -0x1.6888a4e134b2ea520b226eca8694b3a2p-1,
    0x1.6b898fa9efb5d22b58f0d99e9634931ap-1,
    -0x1.9p-1,
  },
  { // Entry 584
    0x1.6888a4e134b2f5ae579fbe4834f8fd55p-1,
    0x1.6b898fa9efb5c6e713c9cff8feb4918fp-1,
    0x1.9000000000001p-1,
  },
  { // Entry 585
    -0x1.6888a4e134b2f5ae579fbe4834f8fd55p-1,
    0x1.6b898fa9efb5c6e713c9cff8feb4918fp-1,
    -0x1.9000000000001p-1,
  },
  { // Entry 586
    -0.0,
    0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.0p-1074,
  },
  { // Entry 587
    0.0,
    0x1.ffffffffffffffffffffffffffffffffp-1,
    0x1.0p-1074,
  },
  { // Entry 588
    -0.0,
    0x1.p0,
    -0.0,
  },
  { // Entry 589
    0.0,
    0x1.ffffffffffffffffffffffffffffffffp-1,
    0x1.0p-1074,
  },
  { // Entry 590
    -0.0,
    0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.0p-1074,
  },
  { // Entry 591
    0x1.91f65f10dd812a0b95ff71871ab48207p-5,
    0x1.ff621e3796d7de4a8ab7d7cd8488f499p-1,
    0x1.921fb54442d17p-5,
  },
  { // Entry 592
    -0x1.91f65f10dd812a0b95ff71871ab48207p-5,
    0x1.ff621e3796d7de4a8ab7d7cd8488f499p-1,
    -0x1.921fb54442d17p-5,
  },
  { // Entry 593
    0x1.91f65f10dd813a06a6f12e3dd9a6a41ep-5,
    0x1.ff621e3796d7de3dfb04df46987f6450p-1,
    0x1.921fb54442d18p-5,
  },
  { // Entry 594
    -0x1.91f65f10dd813a06a6f12e3dd9a6a41ep-5,
    0x1.ff621e3796d7de3dfb04df46987f6450p-1,
    -0x1.921fb54442d18p-5,
  },
  { // Entry 595
    0x1.91f65f10dd814a01b7e2eaf4989861b7p-5,
    0x1.ff621e3796d7de316b51e6bfac75542fp-1,
    0x1.921fb54442d19p-5,
  },
  { // Entry 596
    -0x1.91f65f10dd814a01b7e2eaf4989861b7p-5,
    0x1.ff621e3796d7de316b51e6bfac75542fp-1,
    -0x1.921fb54442d19p-5,
  },
  { // Entry 597
    0x1.917a6bc29b42a9cd2e787ee00ca8f8e3p-4,
    0x1.fd88da3d12525a208898cf58ded0eeb3p-1,
    0x1.921fb54442d17p-4,
  },
  { // Entry 598
    -0x1.917a6bc29b42a9cd2e787ee00ca8f8e3p-4,
    0x1.fd88da3d12525a208898cf58ded0eeb3p-1,
    -0x1.921fb54442d17p-4,
  },
  { // Entry 599
    0x1.917a6bc29b42b9b9754a67729f79346bp-4,
    0x1.fd88da3d125259ee594b5705767ab649p-1,
    0x1.921fb54442d18p-4,
  },
  { // Entry 600
    -0x1.917a6bc29b42b9b9754a67729f79346bp-4,
    0x1.fd88da3d125259ee594b5705767ab649p-1,
    -0x1.921fb54442d18p-4,
  },
  { // Entry 601
    0x1.917a6bc29b42c9a5bc1c50053247de78p-4,
    0x1.fd88da3d125259bc29fddeb20e228056p-1,
    0x1.921fb54442d19p-4,
  },
  { // Entry 602
    -0x1.917a6bc29b42c9a5bc1c50053247de78p-4,
    0x1.fd88da3d125259bc29fddeb20e228056p-1,
    -0x1.921fb54442d19p-4,
  },
  { // Entry 603
    0x1.8f8b83c69a60975f98453886f953a0e5p-3,
    0x1.f6297cff75cb03ab1fced6337e35a245p-1,
    0x1.921fb54442d17p-3,
  },
  { // Entry 604
    -0x1.8f8b83c69a60975f98453886f953a0e5p-3,
    0x1.f6297cff75cb03ab1fced6337e35a245p-1,
    -0x1.921fb54442d17p-3,
  },
  { // Entry 605
    0x1.8f8b83c69a60a710e42d3435516ddaccp-3,
    0x1.f6297cff75cb02e35a0cf2e64de60626p-1,
    0x1.921fb54442d18p-3,
  },
  { // Entry 606
    -0x1.8f8b83c69a60a710e42d3435516ddaccp-3,
    0x1.f6297cff75cb02e35a0cf2e64de60626p-1,
    -0x1.921fb54442d18p-3,
  },
  { // Entry 607
    0x1.8f8b83c69a60b6c230152fe3a981d686p-3,
    0x1.f6297cff75cb021b944b0f991d8e9161p-1,
    0x1.921fb54442d19p-3,
  },
  { // Entry 608
    -0x1.8f8b83c69a60b6c230152fe3a981d686p-3,
    0x1.f6297cff75cb021b944b0f991d8e9161p-1,
    -0x1.921fb54442d19p-3,
  },
  { // Entry 609
    0x1.87de2a6aea961a3e9dedea57fe7e6514p-2,
    0x1.d906bcf328d46672d9c1a865898e5966p-1,
    0x1.921fb54442d17p-2,
  },
  { // Entry 610
    -0x1.87de2a6aea961a3e9dedea57fe7e6514p-2,
    0x1.d906bcf328d46672d9c1a865898e5966p-1,
    -0x1.921fb54442d17p-2,
  },
  { // Entry 611
    0x1.87de2a6aea962906d3d5839ea1a5bcf0p-2,
    0x1.d906bcf328d463631d6cd2905d4b13f5p-1,
    0x1.921fb54442d18p-2,
  },
  { // Entry 612
    -0x1.87de2a6aea962906d3d5839ea1a5bcf0p-2,
    0x1.d906bcf328d463631d6cd2905d4b13f5p-1,
    -0x1.921fb54442d18p-2,
  },
  { // Entry 613
    0x1.87de2a6aea9637cf09bd1ce544b496eap-2,
    0x1.d906bcf328d460536117fcbb30ea3e17p-1,
    0x1.921fb54442d19p-2,
  },
  { // Entry 614
    -0x1.87de2a6aea9637cf09bd1ce544b496eap-2,
    0x1.d906bcf328d460536117fcbb30ea3e17p-1,
    -0x1.921fb54442d19p-2,
  },
  { // Entry 615
    0x1.6a09e667f3bcba99afb0135729457874p-1,
    0x1.6a09e667f3bcd777b6461376ab523242p-1,
    0x1.921fb54442d17p-1,
  },
  { // Entry 616
    -0x1.6a09e667f3bcba99afb0135729457874p-1,
    0x1.6a09e667f3bcd777b6461376ab523242p-1,
    -0x1.921fb54442d17p-1,
  },
  { // Entry 617
    0x1.6a09e667f3bcc5e9fee352f50fd3f4e9p-1,
    0x1.6a09e667f3bccc276712d3d8c5502387p-1,
    0x1.921fb54442d18p-1,
  },
  { // Entry 618
    -0x1.6a09e667f3bcc5e9fee352f50fd3f4e9p-1,
    0x1.6a09e667f3bccc276712d3d8c5502387p-1,
    -0x1.921fb54442d18p-1,
  },
  { // Entry 619
    0x1.6a09e667f3bcd13a4e169292f607eee5p-1,
    0x1.6a09e667f3bcc0d717df943adef39253p-1,
    0x1.921fb54442d19p-1,
  },
  { // Entry 620
    -0x1.6a09e667f3bcd13a4e169292f607eee5p-1,
    0x1.6a09e667f3bcc0d717df943adef39253p-1,
    -0x1.921fb54442d19p-1,
  },
  { // Entry 621
    0x1.fffffffffffffffffffffffffe5f56ffp-1,
    0x1.469898cc51701b839a252049c0b8b50bp-52,
    0x1.921fb54442d17p0,
  },
  { // Entry 622
    -0x1.fffffffffffffffffffffffffe5f56ffp-1,
    0x1.469898cc51701b839a252049c0b8b50bp-52,
    -0x1.921fb54442d17p0,
  },
  { // Entry 623
    0x1.ffffffffffffffffffffffffffec8831p-1,
    0x1.1a62633145c06e0e6894812704419fa8p-54,
    0x1.921fb54442d18p0,
  },
  { // Entry 624
    -0x1.ffffffffffffffffffffffffffec8831p-1,
    0x1.1a62633145c06e0e6894812704419fa8p-54,
    -0x1.921fb54442d18p0,
  },
  { // Entry 625
    0x1.ffffffffffffffffffffffffff79b963p-1,
    -0x1.72cece675d1fc8f8cbb5bf6c7dbcfba0p-53,
    0x1.921fb54442d19p0,
  },
  { // Entry 626
    -0x1.ffffffffffffffffffffffffff79b963p-1,
    -0x1.72cece675d1fc8f8cbb5bf6c7dbcfba0p-53,
    -0x1.921fb54442d19p0,
  },
  { // Entry 627
    0x1.469898cc51701b839a252049bfaeed42p-51,
    -0x1.fffffffffffffffffffffffff97d5bffp-1,
    0x1.921fb54442d17p1,
  },
  { // Entry 628
    -0x1.469898cc51701b839a252049bfaeed42p-51,
    -0x1.fffffffffffffffffffffffff97d5bffp-1,
    -0x1.921fb54442d17p1,
  },
  { // Entry 629
    0x1.1a62633145c06e0e689481270436e2edp-53,
    -0x1.ffffffffffffffffffffffffffb220c5p-1,
    0x1.921fb54442d18p1,
  },
  { // Entry 630
    -0x1.1a62633145c06e0e689481270436e2edp-53,
    -0x1.ffffffffffffffffffffffffffb220c5p-1,
    -0x1.921fb54442d18p1,
  },
  { // Entry 631
    -0x1.72cece675d1fc8f8cbb5bf6c7d5bbc5cp-52,
    -0x1.fffffffffffffffffffffffffde6e58cp-1,
    0x1.921fb54442d19p1,
  },
  { // Entry 632
    0x1.72cece675d1fc8f8cbb5bf6c7d5bbc5cp-52,
    -0x1.fffffffffffffffffffffffffde6e58cp-1,
    -0x1.921fb54442d19p1,
  },
  { // Entry 633
    -0x1.469898cc51701b839a252049bb87ce1bp-50,
    0x1.ffffffffffffffffffffffffe5f56ffep-1,
    0x1.921fb54442d17p2,
  },
  { // Entry 634
    0x1.469898cc51701b839a252049bb87ce1bp-50,
    0x1.ffffffffffffffffffffffffe5f56ffep-1,
    -0x1.921fb54442d17p2,
  },
  { // Entry 635
    -0x1.1a62633145c06e0e68948127040bf003p-52,
    0x1.fffffffffffffffffffffffffec88317p-1,
    0x1.921fb54442d18p2,
  },
  { // Entry 636
    0x1.1a62633145c06e0e68948127040bf003p-52,
    0x1.fffffffffffffffffffffffffec88317p-1,
    -0x1.921fb54442d18p2,
  },
  { // Entry 637
    0x1.72cece675d1fc8f8cbb5bf6c7bd6bf4dp-51,
    0x1.fffffffffffffffffffffffff79b9631p-1,
    0x1.921fb54442d19p2,
  },
  { // Entry 638
    -0x1.72cece675d1fc8f8cbb5bf6c7bd6bf4dp-51,
    0x1.fffffffffffffffffffffffff79b9631p-1,
    -0x1.921fb54442d19p2,
  },
  { // Entry 639
    -0x1.469898cc51701b839a252049aaeb5182p-49,
    0x1.ffffffffffffffffffffffff97d5bff8p-1,
    0x1.921fb54442d17p3,
  },
  { // Entry 640
    0x1.469898cc51701b839a252049aaeb5182p-49,
    0x1.ffffffffffffffffffffffff97d5bff8p-1,
    -0x1.921fb54442d17p3,
  },
  { // Entry 641
    -0x1.1a62633145c06e0e689481270360245cp-51,
    0x1.fffffffffffffffffffffffffb220c5ep-1,
    0x1.921fb54442d18p3,
  },
  { // Entry 642
    0x1.1a62633145c06e0e689481270360245cp-51,
    0x1.fffffffffffffffffffffffffb220c5ep-1,
    -0x1.921fb54442d18p3,
  },
  { // Entry 643
    0x1.72cece675d1fc8f8cbb5bf6c75c2cb0ep-50,
    0x1.ffffffffffffffffffffffffde6e58c4p-1,
    0x1.921fb54442d19p3,
  },
  { // Entry 644
    -0x1.72cece675d1fc8f8cbb5bf6c75c2cb0ep-50,
    0x1.ffffffffffffffffffffffffde6e58c4p-1,
    -0x1.921fb54442d19p3,
  },
  { // Entry 645
    -0x1.469898cc51701b839a25204968795f1cp-48,
    0x1.fffffffffffffffffffffffe5f56ffe1p-1,
    0x1.921fb54442d17p4,
  },
  { // Entry 646
    0x1.469898cc51701b839a25204968795f1cp-48,
    0x1.fffffffffffffffffffffffe5f56ffe1p-1,
    -0x1.921fb54442d17p4,
  },
  { // Entry 647
    -0x1.1a62633145c06e0e6894812700b0f5bfp-50,
    0x1.ffffffffffffffffffffffffec88317ap-1,
    0x1.921fb54442d18p4,
  },
  { // Entry 648
    0x1.1a62633145c06e0e6894812700b0f5bfp-50,
    0x1.ffffffffffffffffffffffffec88317ap-1,
    -0x1.921fb54442d18p4,
  },
  { // Entry 649
    0x1.72cece675d1fc8f8cbb5bf6c5d72fa11p-49,
    0x1.ffffffffffffffffffffffff79b96313p-1,
    0x1.921fb54442d19p4,
  },
  { // Entry 650
    -0x1.72cece675d1fc8f8cbb5bf6c5d72fa11p-49,
    0x1.ffffffffffffffffffffffff79b96313p-1,
    -0x1.921fb54442d19p4,
  },
  { // Entry 651
    -0x1.469898cc51701b839a2520485eb19584p-47,
    0x1.fffffffffffffffffffffff97d5bff87p-1,
    0x1.921fb54442d17p5,
  },
  { // Entry 652
    0x1.469898cc51701b839a2520485eb19584p-47,
    0x1.fffffffffffffffffffffff97d5bff87p-1,
    -0x1.921fb54442d17p5,
  },
  { // Entry 653
    -0x1.1a62633145c06e0e68948126f5f43b4cp-49,
    0x1.ffffffffffffffffffffffffb220c5e9p-1,
    0x1.921fb54442d18p5,
  },
  { // Entry 654
    0x1.1a62633145c06e0e68948126f5f43b4cp-49,
    0x1.ffffffffffffffffffffffffb220c5e9p-1,
    -0x1.921fb54442d18p5,
  },
  { // Entry 655
    0x1.72cece675d1fc8f8cbb5bf6bfc33b61fp-48,
    0x1.fffffffffffffffffffffffde6e58c4cp-1,
    0x1.921fb54442d19p5,
  },
  { // Entry 656
    -0x1.72cece675d1fc8f8cbb5bf6bfc33b61fp-48,
    0x1.fffffffffffffffffffffffde6e58c4cp-1,
    -0x1.921fb54442d19p5,
  },
  { // Entry 657
    -0x1.469898cc51701b839a25204437926f25p-46,
    0x1.ffffffffffffffffffffffe5f56ffe1dp-1,
    0x1.921fb54442d17p6,
  },
  { // Entry 658
    0x1.469898cc51701b839a25204437926f25p-46,
    0x1.ffffffffffffffffffffffe5f56ffe1dp-1,
    -0x1.921fb54442d17p6,
  },
  { // Entry 659
    -0x1.1a62633145c06e0e68948126cb01517dp-48,
    0x1.fffffffffffffffffffffffec88317a7p-1,
    0x1.921fb54442d18p6,
  },
  { // Entry 660
    0x1.1a62633145c06e0e68948126cb01517dp-48,
    0x1.fffffffffffffffffffffffec88317a7p-1,
    -0x1.921fb54442d18p6,
  },
  { // Entry 661
    0x1.72cece675d1fc8f8cbb5bf6a7736a658p-47,
    0x1.fffffffffffffffffffffff79b963131p-1,
    0x1.921fb54442d19p6,
  },
  { // Entry 662
    -0x1.72cece675d1fc8f8cbb5bf6a7736a658p-47,
    0x1.fffffffffffffffffffffff79b963131p-1,
    -0x1.921fb54442d19p6,
  },
  { // Entry 663
    -0x1.469898cc51701b839a2520339b15d5aap-45,
    0x1.ffffffffffffffffffffff97d5bff874p-1,
    0x1.921fb54442d17p7,
  },
  { // Entry 664
    0x1.469898cc51701b839a2520339b15d5aap-45,
    0x1.ffffffffffffffffffffff97d5bff874p-1,
    -0x1.921fb54442d17p7,
  },
  { // Entry 665
    -0x1.1a62633145c06e0e689481261f35aa43p-47,
    0x1.fffffffffffffffffffffffb220c5e9dp-1,
    0x1.921fb54442d18p7,
  },
  { // Entry 666
    0x1.1a62633145c06e0e689481261f35aa43p-47,
    0x1.fffffffffffffffffffffffb220c5e9dp-1,
    -0x1.921fb54442d18p7,
  },
  { // Entry 667
    0x1.72cece675d1fc8f8cbb5bf646342673ap-46,
    0x1.ffffffffffffffffffffffde6e58c4c6p-1,
    0x1.921fb54442d19p7,
  },
  { // Entry 668
    -0x1.72cece675d1fc8f8cbb5bf646342673ap-46,
    0x1.ffffffffffffffffffffffde6e58c4c6p-1,
    -0x1.921fb54442d19p7,
  },
  { // Entry 669
    0x1.6a09e667f3bcffa60c0f53340fd24904p-1,
    -0x1.6a09e667f3bc926b59e6d399bd1b906cp-1,
    0x1.2d97c7f3321d1p1,
  },
  { // Entry 670
    -0x1.6a09e667f3bcffa60c0f53340fd24904p-1,
    -0x1.6a09e667f3bc926b59e6d399bd1b906cp-1,
    -0x1.2d97c7f3321d1p1,
  },
  { // Entry 671
    0x1.6a09e667f3bcd264cf4254bc7ab0c9fap-1,
    -0x1.6a09e667f3bcbfac96b3d2115a3c3e21p-1,
    0x1.2d97c7f3321d2p1,
  },
  { // Entry 672
    -0x1.6a09e667f3bcd264cf4254bc7ab0c9fap-1,
    -0x1.6a09e667f3bcbfac96b3d2115a3c3e21p-1,
    -0x1.2d97c7f3321d2p1,
  },
  { // Entry 673
    0x1.6a09e667f3bca52392755644dfe72357p-1,
    -0x1.6a09e667f3bcecedd380d088f1b4c43cp-1,
    0x1.2d97c7f3321d3p1,
  },
  { // Entry 674
    -0x1.6a09e667f3bca52392755644dfe72357p-1,
    -0x1.6a09e667f3bcecedd380d088f1b4c43cp-1,
    -0x1.2d97c7f3321d3p1,
  },
  { // Entry 675
    -0x1.6a09e667f3bc8c2df1b752b606a0a472p-1,
    -0x1.6a09e667f3bd05e3743ed417c44fba46p-1,
    0x1.f6a7a2955385dp1,
  },
  { // Entry 676
    0x1.6a09e667f3bc8c2df1b752b606a0a472p-1,
    -0x1.6a09e667f3bd05e3743ed417c44fba46p-1,
    -0x1.f6a7a2955385dp1,
  },
  { // Entry 677
    -0x1.6a09e667f3bcb96f2e84512da488ff2dp-1,
    -0x1.6a09e667f3bcd8a23771d5a02ff5e843p-1,
    0x1.f6a7a2955385ep1,
  },
  { // Entry 678
    0x1.6a09e667f3bcb96f2e84512da488ff2dp-1,
    -0x1.6a09e667f3bcd8a23771d5a02ff5e843p-1,
    -0x1.f6a7a2955385ep1,
  },
  { // Entry 679
    -0x1.6a09e667f3bce6b06b514fa53cc9324fp-1,
    -0x1.6a09e667f3bcab60faa4d72895f3eea5p-1,
    0x1.f6a7a2955385fp1,
  },
  { // Entry 680
    0x1.6a09e667f3bce6b06b514fa53cc9324fp-1,
    -0x1.6a09e667f3bcab60faa4d72895f3eea5p-1,
    -0x1.f6a7a2955385fp1,
  },
  { // Entry 681
    -0x1.ffffffffffffffffffffffffe8b27b6ap-1,
    -0x1.34f272993d1414a2b39bd8374c1d1631p-50,
    0x1.2d97c7f3321d1p2,
  },
  { // Entry 682
    0x1.ffffffffffffffffffffffffe8b27b6ap-1,
    -0x1.34f272993d1414a2b39bd8374c1d1631p-50,
    -0x1.2d97c7f3321d1p2,
  },
  { // Entry 683
    -0x1.ffffffffffffffffffffffffff50c9bdp-1,
    -0x1.a79394c9e8a0a5159cdec1ba86377c92p-53,
    0x1.2d97c7f3321d2p2,
  },
  { // Entry 684
    0x1.ffffffffffffffffffffffffff50c9bdp-1,
    -0x1.a79394c9e8a0a5159cdec1ba86377c92p-53,
    -0x1.2d97c7f3321d2p2,
  },
  { // Entry 685
    -0x1.fffffffffffffffffffffffff5ef1810p-1,
    0x1.961b1acd85d7d6ba98c84f915bbcbc6cp-51,
    0x1.2d97c7f3321d3p2,
  },
  { // Entry 686
    0x1.fffffffffffffffffffffffff5ef1810p-1,
    0x1.961b1acd85d7d6ba98c84f915bbcbc6cp-51,
    -0x1.2d97c7f3321d3p2,
  },
  { // Entry 687
    -0x1.6a09e667f3bd3962193b5373069ba0c2p-1,
    0x1.6a09e667f3bc58af4cbad35aabb200f4p-1,
    0x1.5fdbbe9bba774p2,
  },
  { // Entry 688
    0x1.6a09e667f3bd3962193b5373069ba0c2p-1,
    0x1.6a09e667f3bc58af4cbad35aabb200f4p-1,
    -0x1.5fdbbe9bba774p2,
  },
  { // Entry 689
    -0x1.6a09e667f3bcdedf9fa15683e51f7e60p-1,
    0x1.6a09e667f3bcb331c654d049eeba380fp-1,
    0x1.5fdbbe9bba775p2,
  },
  { // Entry 690
    0x1.6a09e667f3bcdedf9fa15683e51f7e60p-1,
    0x1.6a09e667f3bcb331c654d049eeba380fp-1,
    -0x1.5fdbbe9bba775p2,
  },
  { // Entry 691
    -0x1.6a09e667f3bc845d26075994ad02bd98p-1,
    0x1.6a09e667f3bd0db43feecd391b21d0c4p-1,
    0x1.5fdbbe9bba776p2,
  },
  { // Entry 692
    0x1.6a09e667f3bc845d26075994ad02bd98p-1,
    0x1.6a09e667f3bd0db43feecd391b21d0c4p-1,
    -0x1.5fdbbe9bba776p2,
  },
  { // Entry 693
    0x1.6a09e667f3bc5271e48b5276f438579fp-1,
    0x1.6a09e667f3bd3f9f816ad456ba1a54a9p-1,
    0x1.c463abeccb2bap2,
  },
  { // Entry 694
    -0x1.6a09e667f3bc5271e48b5276f438579fp-1,
    0x1.6a09e667f3bd3f9f816ad456ba1a54a9p-1,
    -0x1.c463abeccb2bap2,
  },
  { // Entry 695
    0x1.6a09e667f3bcacf45e254f6638cfe8c6p-1,
    0x1.6a09e667f3bce51d07d0d7679a2d8c53p-1,
    0x1.c463abeccb2bbp2,
  },
  { // Entry 696
    -0x1.6a09e667f3bcacf45e254f6638cfe8c6p-1,
    0x1.6a09e667f3bce51d07d0d7679a2d8c53p-1,
    -0x1.c463abeccb2bbp2,
  },
  { // Entry 697
    0x1.6a09e667f3bd0776d7bf4c5566c6db87p-1,
    0x1.6a09e667f3bc8a9a8e36da7863a02597p-1,
    0x1.c463abeccb2bcp2,
  },
  { // Entry 698
    -0x1.6a09e667f3bd0776d7bf4c5566c6db87p-1,
    0x1.6a09e667f3bc8a9a8e36da7863a02597p-1,
    -0x1.c463abeccb2bcp2,
  },
  { // Entry 699
    0x1.ffffffffffffffffffffffffe31174f5p-1,
    0x1.583ebeff65cc226480ae685c2ad9afdap-50,
    0x1.f6a7a2955385dp2,
  },
  { // Entry 700
    -0x1.ffffffffffffffffffffffffe31174f5p-1,
    0x1.583ebeff65cc226480ae685c2ad9afdap-50,
    -0x1.f6a7a2955385dp2,
  },
  { // Entry 701
    0x1.fffffffffffffffffffffffffe194cd4p-1,
    0x1.60fafbfd9730899202b9a170c4e6a849p-52,
    0x1.f6a7a2955385ep2,
  },
  { // Entry 702
    -0x1.fffffffffffffffffffffffffe194cd4p-1,
    0x1.60fafbfd9730899202b9a170c4e6a849p-52,
    -0x1.f6a7a2955385ep2,
  },
  { // Entry 703
    0x1.fffffffffffffffffffffffff92124b4p-1,
    -0x1.4f8282013467bb36fea32f479bd48f4ap-51,
    0x1.f6a7a2955385fp2,
  },
  { // Entry 704
    -0x1.fffffffffffffffffffffffff92124b4p-1,
    -0x1.4f8282013467bb36fea32f479bd48f4ap-51,
    -0x1.f6a7a2955385fp2,
  },
  { // Entry 705
    0x1.6a09e667f3bdfae1dcce4f18665681c6p-1,
    -0x1.6a09e667f3bb972f8927d7b46c737485p-1,
    0x1.1475cc9eedeffp3,
  },
  { // Entry 706
    -0x1.6a09e667f3bdfae1dcce4f18665681c6p-1,
    -0x1.6a09e667f3bb972f8927d7b46c737485p-1,
    -0x1.1475cc9eedeffp3,
  },
  { // Entry 707
    0x1.6a09e667f3bd45dce99a553a6d7d8065p-1,
    -0x1.6a09e667f3bc4c347c5bd1933ca3261fp-1,
    0x1.1475cc9eedfp3,
  },
  { // Entry 708
    -0x1.6a09e667f3bd45dce99a553a6d7d8065p-1,
    -0x1.6a09e667f3bc4c347c5bd1933ca3261fp-1,
    -0x1.1475cc9eedfp3,
  },
  { // Entry 709
    0x1.6a09e667f3bc90d7f6665b5c1a22056ap-1,
    -0x1.6a09e667f3bd01396f8fcb71b2505e1fp-1,
    0x1.1475cc9eedf01p3,
  },
  { // Entry 710
    -0x1.6a09e667f3bc90d7f6665b5c1a22056ap-1,
    -0x1.6a09e667f3bd01396f8fcb71b2505e1fp-1,
    -0x1.1475cc9eedf01p3,
  },
  { // Entry 711
    0x1.34f272993d1414a2b39bd8373e0d6b94p-49,
    -0x1.ffffffffffffffffffffffffa2c9eda8p-1,
    0x1.2d97c7f3321d1p3,
  },
  { // Entry 712
    -0x1.34f272993d1414a2b39bd8373e0d6b94p-49,
    -0x1.ffffffffffffffffffffffffa2c9eda8p-1,
    -0x1.2d97c7f3321d1p3,
  },
  { // Entry 713
    0x1.a79394c9e8a0a5159cdec1ba85a688bdp-52,
    -0x1.fffffffffffffffffffffffffd4326f5p-1,
    0x1.2d97c7f3321d2p3,
  },
  { // Entry 714
    -0x1.a79394c9e8a0a5159cdec1ba85a688bdp-52,
    -0x1.fffffffffffffffffffffffffd4326f5p-1,
    -0x1.2d97c7f3321d2p3,
  },
  { // Entry 715
    -0x1.961b1acd85d7d6ba98c84f9153c0cc16p-50,
    -0x1.ffffffffffffffffffffffffd7bc6041p-1,
    0x1.2d97c7f3321d3p3,
  },
  { // Entry 716
    0x1.961b1acd85d7d6ba98c84f9153c0cc16p-50,
    -0x1.ffffffffffffffffffffffffd7bc6041p-1,
    -0x1.2d97c7f3321d3p3,
  },
  { // Entry 717
    -0x1.6a09e667f3bb90f220f856d0b1a406c3p-1,
    -0x1.6a09e667f3be011f44fdcffc167f7140p-1,
    0x1.46b9c347764a2p3,
  },
  { // Entry 718
    0x1.6a09e667f3bb90f220f856d0b1a406c3p-1,
    -0x1.6a09e667f3be011f44fdcffc167f7140p-1,
    -0x1.46b9c347764a2p3,
  },
  { // Entry 719
    -0x1.6a09e667f3bc45f7142c50af84f26c75p-1,
    -0x1.6a09e667f3bd4c1a51c9d61e20c523f7p-1,
    0x1.46b9c347764a3p3,
  },
  { // Entry 720
    0x1.6a09e667f3bc45f7142c50af84f26c75p-1,
    -0x1.6a09e667f3bd4c1a51c9d61e20c523f7p-1,
    -0x1.46b9c347764a3p3,
  },
  { // Entry 721
    -0x1.6a09e667f3bcfafc07604a8dfdbe588dp-1,
    -0x1.6a09e667f3bc97155e95dc3fd0885d14p-1,
    0x1.46b9c347764a4p3,
  },
  { // Entry 722
    0x1.6a09e667f3bcfafc07604a8dfdbe588dp-1,
    -0x1.6a09e667f3bc97155e95dc3fd0885d14p-1,
    -0x1.46b9c347764a4p3,
  },
  { // Entry 723
    -0x1.ffffffffffffffffffffffff9d634e9fp-1,
    -0x1.3dc585b2c742181326e07c40748873bbp-49,
    0x1.5fdbbe9bba774p3,
  },
  { // Entry 724
    0x1.ffffffffffffffffffffffff9d634e9fp-1,
    -0x1.3dc585b2c742181326e07c40748873bbp-49,
    -0x1.5fdbbe9bba774p3,
  },
  { // Entry 725
    -0x1.fffffffffffffffffffffffffc461178p-1,
    -0x1.ee2c2d963a10c0993703e20446463301p-52,
    0x1.5fdbbe9bba775p3,
  },
  { // Entry 726
    0x1.fffffffffffffffffffffffffc461178p-1,
    -0x1.ee2c2d963a10c0993703e20446463301p-52,
    -0x1.5fdbbe9bba775p3,
  },
  { // Entry 727
    -0x1.ffffffffffffffffffffffffdb28d451p-1,
    0x1.8474f49a717bcfd9b23f077ee4d090cfp-50,
    0x1.5fdbbe9bba776p3,
  },
  { // Entry 728
    0x1.ffffffffffffffffffffffffdb28d451p-1,
    0x1.8474f49a717bcfd9b23f077ee4d090cfp-50,
    -0x1.5fdbbe9bba776p3,
  },
  { // Entry 729
    -0x1.6a09e667f3be075cad2d50dfc68cd88fp-1,
    0x1.6a09e667f3bb8ab4b8c8d5ecf6b910d6p-1,
    0x1.78fdb9effea45p3,
  },
  { // Entry 730
    0x1.6a09e667f3be075cad2d50dfc68cd88fp-1,
    0x1.6a09e667f3bb8ab4b8c8d5ecf6b910d6p-1,
    -0x1.78fdb9effea45p3,
  },
  { // Entry 731
    -0x1.6a09e667f3bd5257b9f95701d3f13f5dp-1,
    0x1.6a09e667f3bc3fb9abfccfcbcd262aa0p-1,
    0x1.78fdb9effea46p3,
  },
  { // Entry 732
    0x1.6a09e667f3bd5257b9f95701d3f13f5dp-1,
    0x1.6a09e667f3bc3fb9abfccfcbcd262aa0p-1,
    -0x1.78fdb9effea46p3,
  },
  { // Entry 733
    -0x1.6a09e667f3bc9d52c6c55d2386d32c92p-1,
    0x1.6a09e667f3bcf4be9f30c9aa4910cacfp-1,
    0x1.78fdb9effea47p3,
  },
  { // Entry 734
    0x1.6a09e667f3bc9d52c6c55d2386d32c92p-1,
    0x1.6a09e667f3bcf4be9f30c9aa4910cacfp-1,
    -0x1.78fdb9effea47p3,
  },
  { // Entry 735
    0x1.6a09e667f3bb8477509955093bb292bep-1,
    0x1.6a09e667f3be0d9a155cd1c3767eb7b3p-1,
    0x1.ab41b09886fe8p3,
  },
  { // Entry 736
    -0x1.6a09e667f3bb8477509955093bb292bep-1,
    0x1.6a09e667f3be0d9a155cd1c3767eb7b3p-1,
    -0x1.ab41b09886fe8p3,
  },
  { // Entry 737
    0x1.6a09e667f3bc397c43cd4ee8153e60a0p-1,
    0x1.6a09e667f3bd58952228d7e58701d299p-1,
    0x1.ab41b09886fe9p3,
  },
  { // Entry 738
    -0x1.6a09e667f3bc397c43cd4ee8153e60a0p-1,
    0x1.6a09e667f3bd58952228d7e58701d299p-1,
    -0x1.ab41b09886fe9p3,
  },
  { // Entry 739
    0x1.6a09e667f3bcee81370148c69447b4e7p-1,
    0x1.6a09e667f3bca3902ef4de073d0273e6p-1,
    0x1.ab41b09886feap3,
  },
  { // Entry 740
    -0x1.6a09e667f3bcee81370148c69447b4e7p-1,
    0x1.6a09e667f3bca3902ef4de073d0273e6p-1,
    -0x1.ab41b09886feap3,
  },
  { // Entry 741
    0x1.ffffffffffffffffffffffff922141b4p-1,
    0x1.4f6babe5db9e1ef40d69c452e135591dp-49,
    0x1.c463abeccb2bap3,
  },
  { // Entry 742
    -0x1.ffffffffffffffffffffffff922141b4p-1,
    0x1.4f6babe5db9e1ef40d69c452e135591dp-49,
    -0x1.c463abeccb2bap3,
  },
  { // Entry 743
    0x1.fffffffffffffffffffffffff9d717a7p-1,
    0x1.3daeaf976e787bd035a7114be387b5c3p-51,
    0x1.c463abeccb2bbp3,
  },
  { // Entry 744
    -0x1.fffffffffffffffffffffffff9d717a7p-1,
    0x1.3daeaf976e787bd035a7114be387b5c3p-51,
    -0x1.c463abeccb2bbp3,
  },
  { // Entry 745
    0x1.ffffffffffffffffffffffffe18ced9ap-1,
    -0x1.6128a83448c3c217e52c775a0698d26ap-50,
    0x1.c463abeccb2bcp3,
  },
  { // Entry 746
    -0x1.ffffffffffffffffffffffffe18ced9ap-1,
    -0x1.6128a83448c3c217e52c775a0698d26ap-50,
    -0x1.c463abeccb2bcp3,
  },
  { // Entry 747
    0x1.6a09e667f3be13d77d8c52a726550eacp-1,
    -0x1.6a09e667f3bb7e39e869d42580908c7cp-1,
    0x1.dd85a7410f58bp3,
  },
  { // Entry 748
    -0x1.6a09e667f3be13d77d8c52a726550eacp-1,
    -0x1.6a09e667f3bb7e39e869d42580908c7cp-1,
    -0x1.dd85a7410f58bp3,
  },
  { // Entry 749
    0x1.6a09e667f3bd5ed28a5858c939f6ddaap-1,
    -0x1.6a09e667f3bc333edb9dce045d3b0e75p-1,
    0x1.dd85a7410f58cp3,
  },
  { // Entry 750
    -0x1.6a09e667f3bd5ed28a5858c939f6ddaap-1,
    -0x1.6a09e667f3bc333edb9dce045d3b0e75p-1,
    -0x1.dd85a7410f58cp3,
  },
  { // Entry 751
    0x1.6a09e667f3bca9cd97245eeaf316330ep-1,
    -0x1.6a09e667f3bce843ced1c7e2df6316d4p-1,
    0x1.dd85a7410f58dp3,
  },
  { // Entry 752
    -0x1.6a09e667f3bca9cd97245eeaf316330ep-1,
    -0x1.6a09e667f3bce843ced1c7e2df6316d4p-1,
    -0x1.dd85a7410f58dp3,
  },
  { // Entry 753
    0x1.583ebeff65cc226480ae685c1765dec1p-49,
    -0x1.ffffffffffffffffffffffff8c45d3d4p-1,
    0x1.f6a7a2955385dp3,
  },
  { // Entry 754
    -0x1.583ebeff65cc226480ae685c1765dec1p-49,
    -0x1.ffffffffffffffffffffffff8c45d3d4p-1,
    -0x1.f6a7a2955385dp3,
  },
  { // Entry 755
    0x1.60fafbfd9730899202b9a170c3971e86p-51,
    -0x1.fffffffffffffffffffffffff8653353p-1,
    0x1.f6a7a2955385ep3,
  },
  { // Entry 756
    -0x1.60fafbfd9730899202b9a170c3971e86p-51,
    -0x1.fffffffffffffffffffffffff8653353p-1,
    -0x1.f6a7a2955385ep3,
  },
  { // Entry 757
    -0x1.4f8282013467bb36fea32f479753fe7ap-50,
    -0x1.ffffffffffffffffffffffffe48492d3p-1,
    0x1.f6a7a2955385fp3,
  },
  { // Entry 758
    0x1.4f8282013467bb36fea32f479753fe7ap-50,
    -0x1.ffffffffffffffffffffffffe48492d3p-1,
    -0x1.f6a7a2955385fp3,
  },
  { // Entry 759
    -0x1.6a09e667f3ba0df299d25f82f639251fp-1,
    -0x1.6a09e667f3bf841ecc23c74599076a81p-1,
    0x1.07e4cef4cbd96p4,
  },
  { // Entry 760
    0x1.6a09e667f3ba0df299d25f82f639251fp-1,
    -0x1.6a09e667f3bf841ecc23c74599076a81p-1,
    -0x1.07e4cef4cbd96p4,
  },
  { // Entry 761
    -0x1.6a09e667f3bb77fc803a5341c552fe0fp-1,
    -0x1.6a09e667f3be1a14e5bbd38ad60fdd7bp-1,
    0x1.07e4cef4cbd97p4,
  },
  { // Entry 762
    0x1.6a09e667f3bb77fc803a5341c552fe0fp-1,
    -0x1.6a09e667f3be1a14e5bbd38ad60fdd7bp-1,
    -0x1.07e4cef4cbd97p4,
  },
  { // Entry 763
    -0x1.6a09e667f3bce20666a246ff2a62f097p-1,
    -0x1.6a09e667f3bcb00aff53dfcea90e6a0cp-1,
    0x1.07e4cef4cbd98p4,
  },
  { // Entry 764
    0x1.6a09e667f3bce20666a246ff2a62f097p-1,
    -0x1.6a09e667f3bcb00aff53dfcea90e6a0cp-1,
    -0x1.07e4cef4cbd98p4,
  },
  { // Entry 765
    -0x1.fffffffffffffffffffffffd2531a43dp-1,
    -0x1.b088e90c77fd12ea79f98631e6f0b74bp-48,
    0x1.1475cc9eedeffp4,
  },
  { // Entry 766
    0x1.fffffffffffffffffffffffd2531a43dp-1,
    -0x1.b088e90c77fd12ea79f98631e6f0b74bp-48,
    -0x1.1475cc9eedeffp4,
  },
  { // Entry 767
    -0x1.ffffffffffffffffffffffff86437656p-1,
    -0x1.6111d218effa25d4f3f30c654d7c36a1p-49,
    0x1.1475cc9eedfp4,
  },
  { // Entry 768
    0x1.ffffffffffffffffffffffff86437656p-1,
    -0x1.6111d218effa25d4f3f30c654d7c36a1p-49,
    -0x1.1475cc9eedfp4,
  },
  { // Entry 769
    -0x1.ffffffffffffffffffffffffe755486fp-1,
    0x1.3ddc5bce200bb4561819e73527f5a6d7p-50,
    0x1.1475cc9eedf01p4,
  },
  { // Entry 770
    0x1.ffffffffffffffffffffffffe755486fp-1,
    0x1.3ddc5bce200bb4561819e73527f5a6d7p-50,
    -0x1.1475cc9eedf01p4,
  },
  { // Entry 771
    -0x1.6a09e667f3bed557411f4e4c114d7357p-1,
    0x1.6a09e667f3babcba24d6d87ecc8f83b4p-1,
    0x1.2106ca4910068p4,
  },
  { // Entry 772
    0x1.6a09e667f3bed557411f4e4c114d7357p-1,
    0x1.6a09e667f3babcba24d6d87ecc8f83b4p-1,
    -0x1.2106ca4910068p4,
  },
  { // Entry 773
    -0x1.6a09e667f3bd6b4d5ab75a909f8e5b4cp-1,
    0x1.6a09e667f3bc26c40b3ecc3cece1d1a0p-1,
    0x1.2106ca4910069p4,
  },
  { // Entry 774
    0x1.6a09e667f3bd6b4d5ab75a909f8e5b4cp-1,
    0x1.6a09e667f3bc26c40b3ecc3cece1d1a0p-1,
    -0x1.2106ca4910069p4,
  },
  { // Entry 775
    -0x1.6a09e667f3bc0143744f66d3c3c55cd9p-1,
    0x1.6a09e667f3bd90cdf1a6bff9a32a3923p-1,
    0x1.2106ca491006ap4,
  },
  { // Entry 776
    0x1.6a09e667f3bc0143744f66d3c3c55cd9p-1,
    0x1.6a09e667f3bd90cdf1a6bff9a32a3923p-1,
    -0x1.2106ca491006ap4,
  },
  { // Entry 777
    -0x1.34f272993d1414a2b39bd83705cec120p-48,
    0x1.fffffffffffffffffffffffe8b27b6a2p-1,
    0x1.2d97c7f3321d1p4,
  },
  { // Entry 778
    0x1.34f272993d1414a2b39bd83705cec120p-48,
    0x1.fffffffffffffffffffffffe8b27b6a2p-1,
    -0x1.2d97c7f3321d1p4,
  },
  { // Entry 779
    -0x1.a79394c9e8a0a5159cdec1ba8362b968p-51,
    0x1.fffffffffffffffffffffffff50c9bd4p-1,
    0x1.2d97c7f3321d2p4,
  },
  { // Entry 780
    0x1.a79394c9e8a0a5159cdec1ba8362b968p-51,
    0x1.fffffffffffffffffffffffff50c9bd4p-1,
    -0x1.2d97c7f3321d2p4,
  },
  { // Entry 781
    0x1.961b1acd85d7d6ba98c84f9133d10abep-49,
    0x1.ffffffffffffffffffffffff5ef18107p-1,
    0x1.2d97c7f3321d3p4,
  },
  { // Entry 782
    -0x1.961b1acd85d7d6ba98c84f9133d10abep-49,
    0x1.ffffffffffffffffffffffff5ef18107p-1,
    -0x1.2d97c7f3321d3p4,
  },
  { // Entry 783
    0x1.6a09e667f3ba0177c9735dbb72f09f65p-1,
    0x1.6a09e667f3bf90999c82c90cebaf9f3fp-1,
    0x1.3a28c59d54339p4,
  },
  { // Entry 784
    -0x1.6a09e667f3ba0177c9735dbb72f09f65p-1,
    0x1.6a09e667f3bf90999c82c90cebaf9f3fp-1,
    -0x1.3a28c59d54339p4,
  },
  { // Entry 785
    0x1.6a09e667f3bb6b81afdb517a4e8548b4p-1,
    0x1.6a09e667f3be268fb61ad5523532e298p-1,
    0x1.3a28c59d5433ap4,
  },
  { // Entry 786
    -0x1.6a09e667f3bb6b81afdb517a4e8548b4p-1,
    0x1.6a09e667f3be268fb61ad5523532e298p-1,
    -0x1.3a28c59d5433ap4,
  },
  { // Entry 787
    0x1.6a09e667f3bcd58b96434537c0100b9bp-1,
    0x1.6a09e667f3bcbc85cfb2e19614ac3f88p-1,
    0x1.3a28c59d5433bp4,
  },
  { // Entry 788
    -0x1.6a09e667f3bcd58b96434537c0100b9bp-1,
    0x1.6a09e667f3bcbc85cfb2e19614ac3f88p-1,
    -0x1.3a28c59d5433bp4,
  },
  { // Entry 789
    0x1.fffffffffffffffffffffffd0711f437p-1,
    0x1.b95bfc26022b165aed3e2a3b12382479p-48,
    0x1.46b9c347764a2p4,
  },
  { // Entry 790
    -0x1.fffffffffffffffffffffffd0711f437p-1,
    0x1.b95bfc26022b165aed3e2a3b12382479p-48,
    -0x1.46b9c347764a2p4,
  },
  { // Entry 791
    0x1.ffffffffffffffffffffffff79c9ec83p-1,
    0x1.72b7f84c04562cb5da7c5477b957adebp-49,
    0x1.46b9c347764a3p4,
  },
  { // Entry 792
    -0x1.ffffffffffffffffffffffff79c9ec83p-1,
    0x1.72b7f84c04562cb5da7c5477b957adebp-49,
    -0x1.46b9c347764a3p4,
  },
  { // Entry 793
    0x1.ffffffffffffffffffffffffec81e4cfp-1,
    -0x1.1a900f67f753a6944b07571048f1cad2p-50,
    0x1.46b9c347764a4p4,
  },
  { // Entry 794
    -0x1.ffffffffffffffffffffffffec81e4cfp-1,
    -0x1.1a900f67f753a6944b07571048f1cad2p-50,
    -0x1.46b9c347764a4p4,
  },
  { // Entry 795
    0x1.6a09e667f3bee1d2117e501369fbffefp-1,
    -0x1.6a09e667f3bab03f5477d6b74f4d55d5p-1,
    0x1.534ac0f19860bp4,
  },
  { // Entry 796
    -0x1.6a09e667f3bee1d2117e501369fbffefp-1,
    -0x1.6a09e667f3bab03f5477d6b74f4d55d5p-1,
    -0x1.534ac0f19860bp4,
  },
  { // Entry 797
    0x1.6a09e667f3bd77c82b165c5804b7b843p-1,
    -0x1.6a09e667f3bc1a493adfca757c1a741fp-1,
    0x1.534ac0f19860cp4,
  },
  { // Entry 798
    -0x1.6a09e667f3bd77c82b165c5804b7b843p-1,
    -0x1.6a09e667f3bc1a493adfca757c1a741fp-1,
    -0x1.534ac0f19860cp4,
  },
  { // Entry 799
    0x1.6a09e667f3bc0dbe44ae689b35698a2fp-1,
    -0x1.6a09e667f3bd84532147be323eddac01p-1,
    0x1.534ac0f19860dp4,
  },
  { // Entry 800
    -0x1.6a09e667f3bc0dbe44ae689b35698a2fp-1,
    -0x1.6a09e667f3bd84532147be323eddac01p-1,
    -0x1.534ac0f19860dp4,
  },
  { // Entry 801
    0x1.3dc585b2c742181326e07c40375464dep-48,
    -0x1.fffffffffffffffffffffffe758d3a7cp-1,
    0x1.5fdbbe9bba774p4,
  },
  { // Entry 802
    -0x1.3dc585b2c742181326e07c40375464dep-48,
    -0x1.fffffffffffffffffffffffe758d3a7cp-1,
    -0x1.5fdbbe9bba774p4,
  },
  { // Entry 803
    0x1.ee2c2d963a10c0993703e20442ad7b8dp-51,
    -0x1.fffffffffffffffffffffffff11845e1p-1,
    0x1.5fdbbe9bba775p4,
  },
  { // Entry 804
    -0x1.ee2c2d963a10c0993703e20442ad7b8dp-51,
    -0x1.fffffffffffffffffffffffff11845e1p-1,
    -0x1.5fdbbe9bba775p4,
  },
  { // Entry 805
    -0x1.8474f49a717bcfd9b23f077ec8dd175bp-49,
    -0x1.ffffffffffffffffffffffff6ca35147p-1,
    0x1.5fdbbe9bba776p4,
  },
  { // Entry 806
    0x1.8474f49a717bcfd9b23f077ec8dd175bp-49,
    -0x1.ffffffffffffffffffffffff6ca35147p-1,
    -0x1.5fdbbe9bba776p4,
  },
  { // Entry 807
    -0x1.6a09e667f3b9f4fcf9145bf3ef39f901p-1,
    -0x1.6a09e667f3bf9d146ce1cad43de9b352p-1,
    0x1.6c6cbc45dc8dcp4,
  },
  { // Entry 808
    0x1.6a09e667f3b9f4fcf9145bf3ef39f901p-1,
    -0x1.6a09e667f3bf9d146ce1cad43de9b352p-1,
    -0x1.6c6cbc45dc8dcp4,
  },
  { // Entry 809
    -0x1.6a09e667f3bb5f06df7c4fb2d74972aep-1,
    -0x1.6a09e667f3be330a8679d71993e7c709p-1,
    0x1.6c6cbc45dc8ddp4,
  },
  { // Entry 810
    0x1.6a09e667f3bb5f06df7c4fb2d74972aep-1,
    -0x1.6a09e667f3be330a8679d71993e7c709p-1,
    -0x1.6c6cbc45dc8ddp4,
  },
  { // Entry 811
    -0x1.6a09e667f3bcc910c5e44370554f05f4p-1,
    -0x1.6a09e667f3bcc900a011e35d7fdbf459p-1,
    0x1.6c6cbc45dc8dep4,
  },
  { // Entry 812
    0x1.6a09e667f3bcc910c5e44370554f05f4p-1,
    -0x1.6a09e667f3bcc900a011e35d7fdbf459p-1,
    -0x1.6c6cbc45dc8dep4,
  },
  { // Entry 813
    -0x1.fffffffffffffffffffffffce85685bdp-1,
    -0x1.c22f0f3f8c5919cb6082ce443cf95022p-48,
    0x1.78fdb9effea45p4,
  },
  { // Entry 814
    0x1.fffffffffffffffffffffffce85685bdp-1,
    -0x1.c22f0f3f8c5919cb6082ce443cf95022p-48,
    -0x1.78fdb9effea45p4,
  },
  { // Entry 815
    -0x1.ffffffffffffffffffffffff6cb4a43dp-1,
    -0x1.845e1e7f18b23396c1059c8a24c2609cp-49,
    0x1.78fdb9effea46p4,
  },
  { // Entry 816
    0x1.ffffffffffffffffffffffff6cb4a43dp-1,
    -0x1.845e1e7f18b23396c1059c8a24c2609cp-49,
    -0x1.78fdb9effea46p4,
  },
  { // Entry 817
    -0x1.fffffffffffffffffffffffff112c2bcp-1,
    0x1.ee8786039d3731a4fbe98dd6d32ff62ap-51,
    0x1.78fdb9effea47p4,
  },
  { // Entry 818
    0x1.fffffffffffffffffffffffff112c2bcp-1,
    0x1.ee8786039d3731a4fbe98dd6d32ff62ap-51,
    -0x1.78fdb9effea47p4,
  },
  { // Entry 819
    -0x1.6a09e667f3beee4ce1dd51dac23c6bdcp-1,
    0x1.6a09e667f3baa3c48418d4efd19d074ap-1,
    0x1.858eb79a20baep4,
  },
  { // Entry 820
    0x1.6a09e667f3beee4ce1dd51dac23c6bdcp-1,
    0x1.6a09e667f3baa3c48418d4efd19d074ap-1,
    -0x1.858eb79a20baep4,
  },
  { // Entry 821
    -0x1.6a09e667f3bd8442fb755e1f6972f48fp-1,
    0x1.6a09e667f3bc0dce6a80c8ae0ae4f5f3p-1,
    0x1.858eb79a20bafp4,
  },
  { // Entry 822
    0x1.6a09e667f3bd8442fb755e1f6972f48fp-1,
    0x1.6a09e667f3bc0dce6a80c8ae0ae4f5f3p-1,
    -0x1.858eb79a20bafp4,
  },
  { // Entry 823
    -0x1.6a09e667f3bc1a39150d6a62a69f96dap-1,
    0x1.6a09e667f3bd77d850e8bc6ada22fe34p-1,
    0x1.858eb79a20bb0p4,
  },
  { // Entry 824
    0x1.6a09e667f3bc1a39150d6a62a69f96dap-1,
    0x1.6a09e667f3bd77d850e8bc6ada22fe34p-1,
    -0x1.858eb79a20bb0p4,
  },
  { // Entry 825
    0x1.fa7299b17573d373c615096572a0de04p-1,
    -0x1.2ccaf641d4261ea3ccd8d2a102dc3066p-3,
    0x1.fffffffffffffp62,
  },
  { // Entry 826
    -0x1.fa7299b17573d373c615096572a0de04p-1,
    -0x1.2ccaf641d4261ea3ccd8d2a102dc3066p-3,
    -0x1.fffffffffffffp62,
  },
  { // Entry 827
    0x1.fff6dfd42dc54430bc0576b00a88bd94p-1,
    0x1.82aa375b3c33e70663731bab4beb6ed3p-7,
    0x1.0p63,
  },
  { // Entry 828
    -0x1.fff6dfd42dc54430bc0576b00a88bd94p-1,
    0x1.82aa375b3c33e70663731bab4beb6ed3p-7,
    -0x1.0p63,
  },
  { // Entry 829
    0x1.e456b818e7396b45989978d4b7ebf7b3p-1,
    0x1.4c0622a6e35ddefd5f2b7700716a581fp-2,
    0x1.0000000000001p63,
  },
  { // Entry 830
    -0x1.e456b818e7396b45989978d4b7ebf7b3p-1,
    0x1.4c0622a6e35ddefd5f2b7700716a581fp-2,
    -0x1.0000000000001p63,
  },
  { // Entry 831
    -0x1.86dcca0d689e7b56ef79481be4a645c6p-1,
    0x1.4ab650b8c60734385375e22603fcdb10p-1,
    0x1.fffffffffffffp26,
  },
  { // Entry 832
    0x1.86dcca0d689e7b56ef79481be4a645c6p-1,
    0x1.4ab650b8c60734385375e22603fcdb10p-1,
    -0x1.fffffffffffffp26,
  },
  { // Entry 833
    -0x1.86dcc9babb0a40ee875cab3b9e892757p-1,
    0x1.4ab6511a7d39ad3cc88ded1e775ca147p-1,
    0x1.0p27,
  },
  { // Entry 834
    0x1.86dcc9babb0a40ee875cab3b9e892757p-1,
    0x1.4ab6511a7d39ad3cc88ded1e775ca147p-1,
    -0x1.0p27,
  },
  { // Entry 835
    -0x1.86dcc9155fe182d4515599426bb14b94p-1,
    0x1.4ab651ddeb9e61438382f02167ade4afp-1,
    0x1.0000000000001p27,
  },
  { // Entry 836
    0x1.86dcc9155fe182d4515599426bb14b94p-1,
    0x1.4ab651ddeb9e61438382f02167ade4afp-1,
    -0x1.0000000000001p27,
  },
  { // Entry 837
    -0x1.8f22f84d42da1f57469dfaac44a8b73ap-1,
    0x1.40ad67e777b1df0195189f50a8c26345p-1,
    0x1.fffffffffffffp23,
  },
  { // Entry 838
    0x1.8f22f84d42da1f57469dfaac44a8b73ap-1,
    0x1.40ad67e777b1df0195189f50a8c26345p-1,
    -0x1.fffffffffffffp23,
  },
  { // Entry 839
    -0x1.8f22f8433d6edfe9a4aff9622517caa9p-1,
    0x1.40ad67f3f0c9a143963c9c96dbce3f8ap-1,
    0x1.0p24,
  },
  { // Entry 840
    0x1.8f22f8433d6edfe9a4aff9622517caa9p-1,
    0x1.40ad67f3f0c9a143963c9c96dbce3f8ap-1,
    -0x1.0p24,
  },
  { // Entry 841
    -0x1.8f22f82f32985fe30699c6e12d9dfce2p-1,
    0x1.40ad680ce2f924d716769d1064bc8defp-1,
    0x1.0000000000001p24,
  },
  { // Entry 842
    0x1.8f22f82f32985fe30699c6e12d9dfce2p-1,
    0x1.40ad680ce2f924d716769d1064bc8defp-1,
    -0x1.0000000000001p24,
  },
  { // Entry 843
    -0x1.837b9dddc1eabd379d729f575fc1144cp-1,
    -0x1.4eaa606db24c3cb5e15d7b19d5ee73a1p-1,
    0x1.fffffffffffffp1,
  },
  { // Entry 844
    0x1.837b9dddc1eabd379d729f575fc1144cp-1,
    -0x1.4eaa606db24c3cb5e15d7b19d5ee73a1p-1,
    -0x1.fffffffffffffp1,
  },
  { // Entry 845
    -0x1.837b9dddc1eae70ce98055a0e450d93cp-1,
    -0x1.4eaa606db24c0c466da1c2dc7baa2b32p-1,
    0x1.0p2,
  },
  { // Entry 846
    0x1.837b9dddc1eae70ce98055a0e450d93cp-1,
    -0x1.4eaa606db24c0c466da1c2dc7baa2b32p-1,
    -0x1.0p2,
  },
  { // Entry 847
    -0x1.837b9dddc1eb3ab7819bc233db4697b5p-1,
    -0x1.4eaa606db24bab67862a5261b7719dcfp-1,
    0x1.0000000000001p2,
  },
  { // Entry 848
    0x1.837b9dddc1eb3ab7819bc233db4697b5p-1,
    -0x1.4eaa606db24bab67862a5261b7719dcfp-1,
    -0x1.0000000000001p2,
  },
  { // Entry 849
    0x1.d18f6ead1b446b4bcb73c2390b330d75p-1,
    -0x1.aa2265753720101145230952e61c6f2cp-2,
    0x1.fffffffffffffp0,
  },
  { // Entry 850
    -0x1.d18f6ead1b446b4bcb73c2390b330d75p-1,
    -0x1.aa2265753720101145230952e61c6f2cp-2,
    -0x1.fffffffffffffp0,
  },
  { // Entry 851
    0x1.d18f6ead1b445dfab848188009c9bb95p-1,
    -0x1.aa22657537204a4332f8acbb72b0d768p-2,
    0x1.0p1,
  },
  { // Entry 852
    -0x1.d18f6ead1b445dfab848188009c9bb95p-1,
    -0x1.aa22657537204a4332f8acbb72b0d768p-2,
    -0x1.0p1,
  },
  { // Entry 853
    0x1.d18f6ead1b44435891f0c50e01826988p-1,
    -0x1.aa2265753720bea70ea3f38c86db40afp-2,
    0x1.0000000000001p1,
  },
  { // Entry 854
    -0x1.d18f6ead1b44435891f0c50e01826988p-1,
    -0x1.aa2265753720bea70ea3f38c86db40afp-2,
    -0x1.0000000000001p1,
  },
  { // Entry 855
    0x1.aed548f090ced79c79cbf790441f8197p-1,
    0x1.14a280fb5068c69a2ed45fb547c2f271p-1,
    0x1.fffffffffffffp-1,
  },
  { // Entry 856
    -0x1.aed548f090ced79c79cbf790441f8197p-1,
    0x1.14a280fb5068c69a2ed45fb547c2f271p-1,
    -0x1.fffffffffffffp-1,
  },
  { // Entry 857
    0x1.aed548f090cee0418dd3d2138a1e7865p-1,
    0x1.14a280fb5068b923848cdb2ed0e37a53p-1,
    0x1.0p0,
  },
  { // Entry 858
    -0x1.aed548f090cee0418dd3d2138a1e7865p-1,
    0x1.14a280fb5068b923848cdb2ed0e37a53p-1,
    -0x1.0p0,
  },
  { // Entry 859
    0x1.aed548f090cef18bb5e3871a14d94609p-1,
    0x1.14a280fb50689e362ffdd221e2551035p-1,
    0x1.0000000000001p0,
  },
  { // Entry 860
    -0x1.aed548f090cef18bb5e3871a14d94609p-1,
    0x1.14a280fb50689e362ffdd221e2551035p-1,
    -0x1.0000000000001p0,
  },
  { // Entry 861
    0x1.eaee8744b05ef07cd0b9089130598314p-2,
    0x1.c1528065b7d4fdb158c9c4a6b3e4b740p-1,
    0x1.fffffffffffffp-2,
  },
  { // Entry 862
    -0x1.eaee8744b05ef07cd0b9089130598314p-2,
    0x1.c1528065b7d4fdb158c9c4a6b3e4b740p-1,
    -0x1.fffffffffffffp-2,
  },
  { // Entry 863
    0x1.eaee8744b05efe8764bc364fd837b666p-2,
    0x1.c1528065b7d4f9db7bbb3b45f5f5b30ap-1,
    0x1.0p-1,
  },
  { // Entry 864
    -0x1.eaee8744b05efe8764bc364fd837b666p-2,
    0x1.c1528065b7d4f9db7bbb3b45f5f5b30ap-1,
    -0x1.0p-1,
  },
  { // Entry 865
    0x1.eaee8744b05f1a9c8cc291cd27981051p-2,
    0x1.c1528065b7d4f22fc19e288479c36b27p-1,
    0x1.0000000000001p-1,
  },
  { // Entry 866
    -0x1.eaee8744b05f1a9c8cc291cd27981051p-2,
    0x1.c1528065b7d4f22fc19e288479c36b27p-1,
    -0x1.0000000000001p-1,
  },
  { // Entry 867
    0x1.faaeed4f31575c27f39c1d7c012a4413p-3,
    0x1.f01549f7deea184c5f1d210b6adbe56cp-1,
    0x1.fffffffffffffp-3,
  },
  { // Entry 868
    -0x1.faaeed4f31575c27f39c1d7c012a4413p-3,
    0x1.f01549f7deea184c5f1d210b6adbe56cp-1,
    -0x1.fffffffffffffp-3,
  },
  { // Entry 869
    0x1.faaeed4f31576ba89debdc7351e8b1aep-3,
    0x1.f01549f7deea174f07a67972bf29f148p-1,
    0x1.0p-2,
  },
  { // Entry 870
    -0x1.faaeed4f31576ba89debdc7351e8b1aep-3,
    0x1.f01549f7deea174f07a67972bf29f148p-1,
    -0x1.0p-2,
  },
  { // Entry 871
    0x1.faaeed4f31578aa9f28b5a61f34dccb1p-3,
    0x1.f01549f7deea155458b92a4167aec7ffp-1,
    0x1.0000000000001p-2,
  },
  { // Entry 872
    -0x1.faaeed4f31578aa9f28b5a61f34dccb1p-3,
    0x1.f01549f7deea155458b92a4167aec7ffp-1,
    -0x1.0000000000001p-2,
  },
  { // Entry 873
    0x1.feaaeee86ee34cc05eff28740ee7b469p-4,
    0x1.fc015527d5bd371a12320249ca4dafd7p-1,
    0x1.fffffffffffffp-4,
  },
  { // Entry 874
    -0x1.feaaeee86ee34cc05eff28740ee7b469p-4,
    0x1.fc015527d5bd371a12320249ca4dafd7p-1,
    -0x1.fffffffffffffp-4,
  },
  { // Entry 875
    0x1.feaaeee86ee35ca069a86721f89f85a5p-4,
    0x1.fc015527d5bd36da3cd4253bede319cap-1,
    0x1.0p-3,
  },
  { // Entry 876
    -0x1.feaaeee86ee35ca069a86721f89f85a5p-4,
    0x1.fc015527d5bd36da3cd4253bede319cap-1,
    -0x1.0p-3,
  },
  { // Entry 877
    0x1.feaaeee86ee37c607efae47dcc092c1cp-4,
    0x1.fc015527d5bd365a92186b203507f9adp-1,
    0x1.0000000000001p-3,
  },
  { // Entry 878
    -0x1.feaaeee86ee37c607efae47dcc092c1cp-4,
    0x1.fc015527d5bd365a92186b203507f9adp-1,
    -0x1.0000000000001p-3,
  },
  { // Entry 879
    0x1.ffaaaeeed4ed9b53a408c0f2bc02c8f9p-5,
    0x1.ff0015549f4d34da0b745dc7433145efp-1,
    0x1.fffffffffffffp-5,
  },
  { // Entry 880
    -0x1.ffaaaeeed4ed9b53a408c0f2bc02c8f9p-5,
    0x1.ff0015549f4d34da0b745dc7433145efp-1,
    -0x1.fffffffffffffp-5,
  },
  { // Entry 881
    0x1.ffaaaeeed4edab4ba4b365ed25a9595fp-5,
    0x1.ff0015549f4d34ca0e1ee6509bc42b71p-1,
    0x1.0p-4,
  },
  { // Entry 882
    -0x1.ffaaaeeed4edab4ba4b365ed25a9595fp-5,
    0x1.ff0015549f4d34ca0e1ee6509bc42b71p-1,
    -0x1.0p-4,
  },
  { // Entry 883
    0x1.ffaaaeeed4edcb3ba608afe1f8f4fa6bp-5,
    0x1.ff0015549f4d34aa1373f7634ce87737p-1,
    0x1.0000000000001p-4,
  },
  { // Entry 884
    -0x1.ffaaaeeed4edcb3ba608afe1f8f4fa6bp-5,
    0x1.ff0015549f4d34aa1373f7634ce87737p-1,
    -0x1.0000000000001p-4,
  },
  { // Entry 885
    0x1.ffeaaaeeee86d8ccfe368cd95e38f003p-6,
    0x1.ffc00155527d2b16aeb09fb70636a10ap-1,
    0x1.fffffffffffffp-6,
  },
  { // Entry 886
    -0x1.ffeaaaeeee86d8ccfe368cd95e38f003p-6,
    0x1.ffc00155527d2b16aeb09fb70636a10ap-1,
    -0x1.fffffffffffffp-6,
  },
  { // Entry 887
    0x1.ffeaaaeeee86e8cafe41376d47919579p-6,
    0x1.ffc00155527d2b12aedb49d92928df72p-1,
    0x1.0p-5,
  },
  { // Entry 888
    -0x1.ffeaaaeeee86e8cafe41376d47919579p-6,
    0x1.ffc00155527d2b12aedb49d92928df72p-1,
    -0x1.0p-5,
  },
  { // Entry 889
    0x1.ffeaaaeeee8708c6fe568c951a428069p-6,
    0x1.ffc00155527d2b0aaf309e1d6f0cfc4ep-1,
    0x1.0000000000001p-5,
  },
  { // Entry 890
    -0x1.ffeaaaeeee8708c6fe568c951a428069p-6,
    0x1.ffc00155527d2b0aaf309e1d6f0cfc4ep-1,
    -0x1.0000000000001p-5,
  },
  { // Entry 891
    0x1.fffaaaaeeeed3ed5c9c5ab6538f9cce0p-7,
    0x1.fff000155549f4a38a2563ef344c3ff4p-1,
    0x1.fffffffffffffp-7,
  },
  { // Entry 892
    -0x1.fffaaaaeeeed3ed5c9c5ab6538f9cce0p-7,
    0x1.fff000155549f4a38a2563ef344c3ff4p-1,
    -0x1.fffffffffffffp-7,
  },
  { // Entry 893
    0x1.fffaaaaeeeed4ed549c6560f889ee531p-7,
    0x1.fff000155549f4a28a280e97bcd59c8ap-1,
    0x1.0p-6,
  },
  { // Entry 894
    -0x1.fffaaaaeeeed4ed549c6560f889ee531p-7,
    0x1.fff000155549f4a28a280e97bcd59c8ap-1,
    -0x1.0p-6,
  },
  { // Entry 895
    0x1.fffaaaaeeeed6ed449c7ab6427e8fdd4p-7,
    0x1.fff000155549f4a08a2d63e8cde83db5p-1,
    0x1.0000000000001p-6,
  },
  { // Entry 896
    -0x1.fffaaaaeeeed6ed449c7ab6427e8fdd4p-7,
    0x1.fff000155549f4a08a2d63e8cde83db5p-1,
    -0x1.0000000000001p-6,
  },
  { // Entry 897
    0x1.fffffffaaaaa9aaeeeef6eed4ed442a4p-15,
    0x1.fffffff00000001555565549f49c9f4dp-1,
    0x1.fffffffffffffp-15,
  },
  { // Entry 898
    -0x1.fffffffaaaaa9aaeeeef6eed4ed442a4p-15,
    0x1.fffffff00000001555565549f49c9f4dp-1,
    -0x1.fffffffffffffp-15,
  },
  { // Entry 899
    0x1.fffffffaaaaaaaaeeeeeeeed4ed4ed4fp-15,
    0x1.fffffff00000001555555549f49f49f7p-1,
    0x1.0p-14,
  },
  { // Entry 900
    -0x1.fffffffaaaaaaaaeeeeeeeed4ed4ed4fp-15,
    0x1.fffffff00000001555555549f49f49f7p-1,
    -0x1.0p-14,
  },
  { // Entry 901
    0x1.fffffffaaaaacaaeeeedeeed4ed642a4p-15,
    0x1.fffffff00000001555535549f4a49f4dp-1,
    0x1.0000000000001p-14,
  },
  { // Entry 902
    -0x1.fffffffaaaaacaaeeeedeeed4ed642a4p-15,
    0x1.fffffff00000001555535549f4a49f4dp-1,
    -0x1.0000000000001p-14,
  },
  { // Entry 903
    0x1.ffffffffffffeeaaaaaaaaaaaacaeeeep-28,
    0x1.fffffffffffffc000000000000415555p-1,
    0x1.fffffffffffffp-28,
  },
  { // Entry 904
    -0x1.ffffffffffffeeaaaaaaaaaaaacaeeeep-28,
    0x1.fffffffffffffc000000000000415555p-1,
    -0x1.fffffffffffffp-28,
  },
  { // Entry 905
    0x1.fffffffffffffeaaaaaaaaaaaaaaeeeep-28,
    0x1.fffffffffffffc000000000000015555p-1,
    0x1.0p-27,
  },
  { // Entry 906
    -0x1.fffffffffffffeaaaaaaaaaaaaaaeeeep-28,
    0x1.fffffffffffffc000000000000015555p-1,
    -0x1.0p-27,
  },
  { // Entry 907
    0x1.0000000000000f555555555555357777p-27,
    0x1.fffffffffffffbffffffffffff815555p-1,
    0x1.0000000000001p-27,
  },
  { // Entry 908
    -0x1.0000000000000f555555555555357777p-27,
    0x1.fffffffffffffbffffffffffff815555p-1,
    -0x1.0000000000001p-27,
  },
  { // Entry 909
    0x1.ffffffffffffeffaaaaaaaaaaaab2aaep-31,
    0x1.fffffffffffffff00000000000010015p-1,
    0x1.fffffffffffffp-31,
  },
  { // Entry 910
    -0x1.ffffffffffffeffaaaaaaaaaaaab2aaep-31,
    0x1.fffffffffffffff00000000000010015p-1,
    -0x1.fffffffffffffp-31,
  },
  { // Entry 911
    0x1.fffffffffffffffaaaaaaaaaaaaaaaaep-31,
    0x1.fffffffffffffff00000000000000015p-1,
    0x1.0p-30,
  },
  { // Entry 912
    -0x1.fffffffffffffffaaaaaaaaaaaaaaaaep-31,
    0x1.fffffffffffffff00000000000000015p-1,
    -0x1.0p-30,
  },
  { // Entry 913
    0x1.0000000000000ffd555555555554d557p-30,
    0x1.ffffffffffffffeffffffffffffe0015p-1,
    0x1.0000000000001p-30,
  },
  { // Entry 914
    -0x1.0000000000000ffd555555555554d557p-30,
    0x1.ffffffffffffffeffffffffffffe0015p-1,
    -0x1.0000000000001p-30,
  },
  { // Entry 915
    -0x1.452fc98b34e96b61139b09a7c84a44bdp-8,
    -0x1.fffe62ecfab753c071b2680e1e26bbcep-1,
    -0x1.fffffffffffffp1023,
  },
  { // Entry 916
    0x1.452fc98b34e96b61139b09a7c84a44bdp-8,
    -0x1.fffe62ecfab753c071b2680e1e26bbcep-1,
    0x1.fffffffffffffp1023,
  },
  { // Entry 917
    0x1.452fc98b34e96b61139b09a7c84a44bdp-8,
    -0x1.fffe62ecfab753c071b2680e1e26bbcep-1,
    0x1.fffffffffffffp1023,
  },
  { // Entry 918
    -0x1.452fc98b34e96b61139b09a7c84a44bdp-8,
    -0x1.fffe62ecfab753c071b2680e1e26bbcep-1,
    -0x1.fffffffffffffp1023,
  },
  { // Entry 919
    0x1.452fc98b34e96b61139b09a7c84a44bdp-8,
    -0x1.fffe62ecfab753c071b2680e1e26bbcep-1,
    0x1.fffffffffffffp1023,
  },
  { // Entry 920
    -0x1.452fc98b34e96b61139b09a7c84a44bdp-8,
    -0x1.fffe62ecfab753c071b2680e1e26bbcep-1,
    -0x1.fffffffffffffp1023,
  },
  { // Entry 921
    0x1.daa3677c6ee8a22eb6c4b12ca10ce021p-1,
    0x1.7ffdfb4c5308f777cf774c733b256695p-2,
    0x1.ffffffffffffep1023,
  },
  { // Entry 922
    -0x1.daa3677c6ee8a22eb6c4b12ca10ce021p-1,
    0x1.7ffdfb4c5308f777cf774c733b256695p-2,
    -0x1.ffffffffffffep1023,
  },
  { // Entry 923
    0x1.1a62633145c06e0e689481270436e2edp-53,
    -0x1.ffffffffffffffffffffffffffb220c5p-1,
    0x1.921fb54442d18p1,
  },
  { // Entry 924
    -0x1.1a62633145c06e0e689481270436e2edp-53,
    -0x1.ffffffffffffffffffffffffffb220c5p-1,
    -0x1.921fb54442d18p1,
  },
  { // Entry 925
    0x1.ffffffffffffffffffffffffffec8831p-1,
    0x1.1a62633145c06e0e6894812704419fa8p-54,
    0x1.921fb54442d18p0,
  },
  { // Entry 926
    -0x1.ffffffffffffffffffffffffffec8831p-1,
    0x1.1a62633145c06e0e6894812704419fa8p-54,
    -0x1.921fb54442d18p0,
  },
  { // Entry 927
    0x1.aed548f090cef18bb5e3871a14d94609p-1,
    0x1.14a280fb50689e362ffdd221e2551035p-1,
    0x1.0000000000001p0,
  },
  { // Entry 928
    -0x1.aed548f090cef18bb5e3871a14d94609p-1,
    0x1.14a280fb50689e362ffdd221e2551035p-1,
    -0x1.0000000000001p0,
  },
  { // Entry 929
    0x1.aed548f090cee0418dd3d2138a1e7865p-1,
    0x1.14a280fb5068b923848cdb2ed0e37a53p-1,
    0x1.0p0,
  },
  { // Entry 930
    -0x1.aed548f090cee0418dd3d2138a1e7865p-1,
    0x1.14a280fb5068b923848cdb2ed0e37a53p-1,
    -0x1.0p0,
  },
  { // Entry 931
    0x1.aed548f090ced79c79cbf790441f8197p-1,
    0x1.14a280fb5068c69a2ed45fb547c2f271p-1,
    0x1.fffffffffffffp-1,
  },
  { // Entry 932
    -0x1.aed548f090ced79c79cbf790441f8197p-1,
    0x1.14a280fb5068c69a2ed45fb547c2f271p-1,
    -0x1.fffffffffffffp-1,
  },
  { // Entry 933
    0x1.6a09e667f3bcc5e9fee352f50fd3f4e9p-1,
    0x1.6a09e667f3bccc276712d3d8c5502387p-1,
    0x1.921fb54442d18p-1,
  },
  { // Entry 934
    -0x1.6a09e667f3bcc5e9fee352f50fd3f4e9p-1,
    0x1.6a09e667f3bccc276712d3d8c5502387p-1,
    -0x1.921fb54442d18p-1,
  },
  { // Entry 935
    0x1.0000000000000fffffffffffffffffffp-1022,
    0x1.ffffffffffffffffffffffffffffffffp-1,
    0x1.0000000000001p-1022,
  },
  { // Entry 936
    -0x1.0000000000000fffffffffffffffffffp-1022,
    0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.0000000000001p-1022,
  },
  { // Entry 937
    0x1.ffffffffffffffffffffffffffffffffp-1023,
    0x1.ffffffffffffffffffffffffffffffffp-1,
    0x1.0p-1022,
  },
  { // Entry 938
    -0x1.ffffffffffffffffffffffffffffffffp-1023,
    0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.0p-1022,
  },
  { // Entry 939
    0x1.ffffffffffffdfffffffffffffffffffp-1023,
    0x1.ffffffffffffffffffffffffffffffffp-1,
    0x1.ffffffffffffep-1023,
  },
  { // Entry 940
    -0x1.ffffffffffffdfffffffffffffffffffp-1023,
    0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.ffffffffffffep-1023,
  },
  { // Entry 941
    0x1.ffffffffffffbfffffffffffffffffffp-1023,
    0x1.ffffffffffffffffffffffffffffffffp-1,
    0x1.ffffffffffffcp-1023,
  },
  { // Entry 942
    -0x1.ffffffffffffbfffffffffffffffffffp-1023,
    0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.ffffffffffffcp-1023,
  },
  { // Entry 943
    0x1.ffffffffffffffffffffffffffffffffp-1074,
    0x1.ffffffffffffffffffffffffffffffffp-1,
    0x1.0p-1073,
  },
  { // Entry 944
    -0x1.ffffffffffffffffffffffffffffffffp-1074,
    0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.0p-1073,
  },
  { // Entry 945
    0.0,
    0x1.ffffffffffffffffffffffffffffffffp-1,
    0x1.0p-1074,
  },
  { // Entry 946
    -0.0,
    0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.0p-1074,
  },
  { // Entry 947
    0.0,
    0x1.p0,
    0.0,
  },
  { // Entry 948
    -0.0,
    0x1.p0,
    -0.0,
  },
};
#endif // __BIONIC__

TEST(math_sincos, sincos_intel) {
#if defined(__BIONIC__)
  for (size_t i = 0; i < sizeof(g_sincos_intel_data)/sizeof(sincos_intel_data_t); i++) {
   double dsin, dcos;
   sincos(g_sincos_intel_data[i].call_data, &dsin, &dcos);
   EXPECT_DOUBLE_EQ(g_sincos_intel_data[i].sin_expected, dsin) << "Failed on element " << i;
   EXPECT_DOUBLE_EQ(g_sincos_intel_data[i].cos_expected, dcos) << "Failed on element " << i;
  }
#else // __BIONIC__
  GTEST_LOG_(INFO) << "This test does nothing.";
#endif // __BIONIC__
}
