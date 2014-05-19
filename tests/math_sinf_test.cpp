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
} sinf_intel_data_t;

static sinf_intel_data_t g_sinf_intel_data[] = {
  { // Entry 0
    -0x1.0003fffffff554d5535552cccf778ccdp-21,
    -0x1.0004p-21
  },
  { // Entry 1
    0x1.0003fffffff554d5535552cccf778ccdp-21,
    0x1.0004p-21
  },
  { // Entry 2
    -0x1.d30ca7008af24ef048a7a05eecde2120p-4,
    -0x1.00cbf0p10
  },
  { // Entry 3
    0x1.d30ca7008af24ef048a7a05eecde2120p-4,
    0x1.00cbf0p10
  },
  { // Entry 4
    0x1.8d665d0000200efc4478008658c617c7p-1,
    -0x1.01ee34p2
  },
  { // Entry 5
    -0x1.8d665d0000200efc4478008658c617c7p-1,
    0x1.01ee34p2
  },
  { // Entry 6
    0x1.ec042ff972e42c23f28f2da4fb75f851p-1,
    -0x1.18f1f4p4
  },
  { // Entry 7
    -0x1.ec042ff972e42c23f28f2da4fb75f851p-1,
    0x1.18f1f4p4
  },
  { // Entry 8
    0x1.f0d384240ad9f2dba47b96b08271b41fp-1,
    -0x1.1df906p2
  },
  { // Entry 9
    -0x1.f0d384240ad9f2dba47b96b08271b41fp-1,
    0x1.1df906p2
  },
  { // Entry 10
    -0x1.4616daf9cd3ce4e92186e7d8ec3d3ef0p-5,
    -0x1.3ap6
  },
  { // Entry 11
    0x1.4616daf9cd3ce4e92186e7d8ec3d3ef0p-5,
    0x1.3ap6
  },
  { // Entry 12
    0x1.df04c5f9de343235f4f8b1797286872bp-1,
    -0x1.44b096p2
  },
  { // Entry 13
    -0x1.df04c5f9de343235f4f8b1797286872bp-1,
    0x1.44b096p2
  },
  { // Entry 14
    -0x1.feffbd7334c7043b90a6bc2d76dda7c8p-1,
    -0x1.821cf6p0
  },
  { // Entry 15
    0x1.feffbd7334c7043b90a6bc2d76dda7c8p-1,
    0x1.821cf6p0
  },
  { // Entry 16
    0x1.ebac56ffe6f27893c69f68d1d7b4a448p-3,
    -0x1.829b86p2
  },
  { // Entry 17
    -0x1.ebac56ffe6f27893c69f68d1d7b4a448p-3,
    0x1.829b86p2
  },
  { // Entry 18
    0x1.b725ccef65b1390912a0f5eaab0e7202p-6,
    -0x1.8580fep7
  },
  { // Entry 19
    -0x1.b725ccef65b1390912a0f5eaab0e7202p-6,
    0x1.8580fep7
  },
  { // Entry 20
    0x1.0b3366508957520d9dc88d7c09337e24p-1,
    -0x1.fffffep127
  },
  { // Entry 21
    -0x1.0b3366508957520d9dc88d7c09337e24p-1,
    0x1.fffffep127
  },
  { // Entry 22
    0.0f,
    0x1.p-149
  },
  { // Entry 23
    -0.0f,
    -0x1.p-149
  },
  { // Entry 24
    -0x1.f83de7c0683d3bd90a769f62a41ac1c3p-1,
    0x1.000002p52
  },
  { // Entry 25
    0x1.f83de7c0683d3bd90a769f62a41ac1c3p-1,
    -0x1.000002p52
  },
  { // Entry 26
    -0x1.7dbd11ffde4c20dbb376019f1c654bb0p-2,
    0x1.000022p62
  },
  { // Entry 27
    0x1.7dbd11ffde4c20dbb376019f1c654bb0p-2,
    -0x1.000022p62
  },
  { // Entry 28
    0x1.eaf0d5008f2db899e2487da9d3b9cde7p-2,
    0x1.000150p-1
  },
  { // Entry 29
    -0x1.eaf0d5008f2db899e2487da9d3b9cde7p-2,
    -0x1.000150p-1
  },
  { // Entry 30
    0x1.ffae8cfefa813da87498df988c3d8f5ep-5,
    0x1.0001f0p-4
  },
  { // Entry 31
    -0x1.ffae8cfefa813da87498df988c3d8f5ep-5,
    -0x1.0001f0p-4
  },
  { // Entry 32
    0x1.89c7d6087a861bb6fe794148c3425348p-1,
    0x1.0003p40
  },
  { // Entry 33
    -0x1.89c7d6087a861bb6fe794148c3425348p-1,
    -0x1.0003p40
  },
  { // Entry 34
    -0x1.59e1f519ef8b722eae12a404cad937ebp-1,
    0x1.000380p126
  },
  { // Entry 35
    0x1.59e1f519ef8b722eae12a404cad937ebp-1,
    -0x1.000380p126
  },
  { // Entry 36
    0x1.0003fffffff554d5535552cccf778ccdp-21,
    0x1.0004p-21
  },
  { // Entry 37
    -0x1.0003fffffff554d5535552cccf778ccdp-21,
    -0x1.0004p-21
  },
  { // Entry 38
    0x1.383065027ec4f20ab1c0027d991b805ap-6,
    0x1.0007c0p9
  },
  { // Entry 39
    -0x1.383065027ec4f20ab1c0027d991b805ap-6,
    -0x1.0007c0p9
  },
  { // Entry 40
    0x1.eb0397002fdfbae226f069d6cd53c40fp-2,
    0x1.000cp-1
  },
  { // Entry 41
    -0x1.eb0397002fdfbae226f069d6cd53c40fp-2,
    -0x1.000cp-1
  },
  { // Entry 42
    0x1.fedc8b0001f7dacee090c5edc3bfa547p-4,
    0x1.0019p-3
  },
  { // Entry 43
    -0x1.fedc8b0001f7dacee090c5edc3bfa547p-4,
    -0x1.0019p-3
  },
  { // Entry 44
    0x1.ab471af26ffea23d7643cf614c860b18p-10,
    0x1.003820p68
  },
  { // Entry 45
    -0x1.ab471af26ffea23d7643cf614c860b18p-10,
    -0x1.003820p68
  },
  { // Entry 46
    0x1.ff29eefdde3041fc5c75a1f822a5421dp-4,
    0x1.0040p-3
  },
  { // Entry 47
    -0x1.ff29eefdde3041fc5c75a1f822a5421dp-4,
    -0x1.0040p-3
  },
  { // Entry 48
    0x1.29070175206b3f198c893e19b4bd6cb8p-13,
    0x1.01f0p103
  },
  { // Entry 49
    -0x1.29070175206b3f198c893e19b4bd6cb8p-13,
    -0x1.01f0p103
  },
  { // Entry 50
    -0x1.ffffffffffffaafbe53de74231954709p-1,
    0x1.04ccbcp18
  },
  { // Entry 51
    0x1.ffffffffffffaafbe53de74231954709p-1,
    -0x1.04ccbcp18
  },
  { // Entry 52
    0x1.f4f72cad5f5fb17ec608ec7ac1cbca8bp-2,
    0x1.05bcp-1
  },
  { // Entry 53
    -0x1.f4f72cad5f5fb17ec608ec7ac1cbca8bp-2,
    -0x1.05bcp-1
  },
  { // Entry 54
    0x1.f896c6689d7e75e351ecc85731254cd3p-1,
    0x1.07c1f8p119
  },
  { // Entry 55
    -0x1.f896c6689d7e75e351ecc85731254cd3p-1,
    -0x1.07c1f8p119
  },
  { // Entry 56
    -0x1.267593f4d59c7de6170090a6a084cdcap-14,
    0x1.080180p23
  },
  { // Entry 57
    0x1.267593f4d59c7de6170090a6a084cdcap-14,
    -0x1.080180p23
  },
  { // Entry 58
    0x1.fc86372a841549410c12dbec6fe80dc5p-2,
    0x1.0a14p-1
  },
  { // Entry 59
    -0x1.fc86372a841549410c12dbec6fe80dc5p-2,
    -0x1.0a14p-1
  },
  { // Entry 60
    0x1.bca28f0004316bae8df6a2352cca1382p-1,
    0x1.0b770ap1
  },
  { // Entry 61
    -0x1.bca28f0004316bae8df6a2352cca1382p-1,
    -0x1.0b770ap1
  },
  { // Entry 62
    -0x1.fade9abd67526d37189df004b51d9d5ap-1,
    0x1.0ddcp96
  },
  { // Entry 63
    0x1.fade9abd67526d37189df004b51d9d5ap-1,
    -0x1.0ddcp96
  },
  { // Entry 64
    0x1.bf683cffffec6e8672a49f08aae49c46p-1,
    0x1.1024p0
  },
  { // Entry 65
    -0x1.bf683cffffec6e8672a49f08aae49c46p-1,
    -0x1.1024p0
  },
  { // Entry 66
    -0x1.f14407f6441e92f480d2e22859d67a39p-1,
    0x1.109cc4p4
  },
  { // Entry 67
    0x1.f14407f6441e92f480d2e22859d67a39p-1,
    -0x1.109cc4p4
  },
  { // Entry 68
    -0x1.ff7e7b9983a7c2ebecfab1a0b0af3c4dp-1,
    0x1.13bfb2p4
  },
  { // Entry 69
    0x1.ff7e7b9983a7c2ebecfab1a0b0af3c4dp-1,
    -0x1.13bfb2p4
  },
  { // Entry 70
    0x1.c4942b1e9ec175aa7d3fd75f2e37e566p-1,
    0x1.1590p0
  },
  { // Entry 71
    -0x1.c4942b1e9ec175aa7d3fd75f2e37e566p-1,
    -0x1.1590p0
  },
  { // Entry 72
    0x1.fff8d36866faaac8c17e1267498bd132p-1,
    0x1.18p85
  },
  { // Entry 73
    -0x1.fff8d36866faaac8c17e1267498bd132p-1,
    -0x1.18p85
  },
  { // Entry 74
    0x1.c78d962546ce51b882c9054eccd829b1p-1,
    0x1.18c8p0
  },
  { // Entry 75
    -0x1.c78d962546ce51b882c9054eccd829b1p-1,
    -0x1.18c8p0
  },
  { // Entry 76
    0x1.c7cb99a111f26527b805734962fb12a0p-1,
    0x1.190cp0
  },
  { // Entry 77
    -0x1.c7cb99a111f26527b805734962fb12a0p-1,
    -0x1.190cp0
  },
  { // Entry 78
    -0x1.70898100055943898aa7053017ecba69p-1,
    0x1.1e8f14p62
  },
  { // Entry 79
    0x1.70898100055943898aa7053017ecba69p-1,
    -0x1.1e8f14p62
  },
  { // Entry 80
    0x1.cdef1e4eb37e9f2249656c27d62b1895p-1,
    0x1.1ff8p0
  },
  { // Entry 81
    -0x1.cdef1e4eb37e9f2249656c27d62b1895p-1,
    -0x1.1ff8p0
  },
  { // Entry 82
    -0x1.5df0dd60f122bf79d1ddcf295c34cb0ep-1,
    0x1.2244a6p59
  },
  { // Entry 83
    0x1.5df0dd60f122bf79d1ddcf295c34cb0ep-1,
    -0x1.2244a6p59
  },
  { // Entry 84
    -0x1.fb8dd74f1b091a7b0f9f9d44db84661cp-3,
    0x1.287ae0p13
  },
  { // Entry 85
    0x1.fb8dd74f1b091a7b0f9f9d44db84661cp-3,
    -0x1.287ae0p13
  },
  { // Entry 86
    -0x1.fffffffffffff5c0d2630ee0a1fb4e7bp-1,
    0x1.2d97c8p2
  },
  { // Entry 87
    0x1.fffffffffffff5c0d2630ee0a1fb4e7bp-1,
    -0x1.2d97c8p2
  },
  { // Entry 88
    -0x1.99bc5b961b1ac296dbe1980fd2c890a0p-26,
    0x1.2d97c8p3
  },
  { // Entry 89
    0x1.99bc5b961b1ac296dbe1980fd2c890a0p-26,
    -0x1.2d97c8p3
  },
  { // Entry 90
    0x1.ffd5158b81944ff6b4833fdf1faa9281p-1,
    0x1.32cdc6p18
  },
  { // Entry 91
    -0x1.ffd5158b81944ff6b4833fdf1faa9281p-1,
    -0x1.32cdc6p18
  },
  { // Entry 92
    0x1.e191e77f58f479b564ec538b671f98afp-14,
    0x1.37efd8p100
  },
  { // Entry 93
    -0x1.e191e77f58f479b564ec538b671f98afp-14,
    -0x1.37efd8p100
  },
  { // Entry 94
    0x1.fffe974dceb10d4877a75bc77870ffebp-1,
    0x1.38p11
  },
  { // Entry 95
    -0x1.fffe974dceb10d4877a75bc77870ffebp-1,
    -0x1.38p11
  },
  { // Entry 96
    -0x1.b63f2c3f537b10eec2fbf340e4898644p-18,
    0x1.3d72b8p15
  },
  { // Entry 97
    0x1.b63f2c3f537b10eec2fbf340e4898644p-18,
    -0x1.3d72b8p15
  },
  { // Entry 98
    -0x1.e0208cff17f9186218945af9256d736cp-1,
    0x1.444b20p7
  },
  { // Entry 99
    0x1.e0208cff17f9186218945af9256d736cp-1,
    -0x1.444b20p7
  },
  { // Entry 100
    0x1.fffff9002cf87dade6f62eb709dc2aa7p-1,
    0x1.46b71ep4
  },
  { // Entry 101
    -0x1.fffff9002cf87dade6f62eb709dc2aa7p-1,
    -0x1.46b71ep4
  },
  { // Entry 102
    0x1.c875581a762211dd5caaf9abde5cf656p-21,
    0x1.4d0242p7
  },
  { // Entry 103
    -0x1.c875581a762211dd5caaf9abde5cf656p-21,
    -0x1.4d0242p7
  },
  { // Entry 104
    -0x1.6a6f79000053a3842566f1e018d6359fp-1,
    0x1.511e8ep28
  },
  { // Entry 105
    0x1.6a6f79000053a3842566f1e018d6359fp-1,
    -0x1.511e8ep28
  },
  { // Entry 106
    0x1.fffe9ffa6b569dd8c720d68e80bd9a63p-1,
    0x1.5354d0p100
  },
  { // Entry 107
    -0x1.fffe9ffa6b569dd8c720d68e80bd9a63p-1,
    -0x1.5354d0p100
  },
  { // Entry 108
    0x1.3b4d58fffff29126460f258ec4423e9ap-1,
    0x1.53ad5cp-1
  },
  { // Entry 109
    -0x1.3b4d58fffff29126460f258ec4423e9ap-1,
    -0x1.53ad5cp-1
  },
  { // Entry 110
    -0x1.fffeb762e93ead5ac70f85064fc79cffp-1,
    0x1.60p3
  },
  { // Entry 111
    0x1.fffeb762e93ead5ac70f85064fc79cffp-1,
    -0x1.60p3
  },
  { // Entry 112
    0x1.1772377ffe5b1f436c0149e328446f74p-20,
    0x1.6c6cbcp6
  },
  { // Entry 113
    -0x1.1772377ffe5b1f436c0149e328446f74p-20,
    -0x1.6c6cbcp6
  },
  { // Entry 114
    0x1.77fffdfffffff78d0577d9552663ea3ep-26,
    0x1.77fffep-26
  },
  { // Entry 115
    -0x1.77fffdfffffff78d0577d9552663ea3ep-26,
    -0x1.77fffep-26
  },
  { // Entry 116
    -0x1.ff04979385946d0568c153bf9e125fccp-1,
    0x1.77fffep4
  },
  { // Entry 117
    0x1.ff04979385946d0568c153bf9e125fccp-1,
    -0x1.77fffep4
  },
  { // Entry 118
    0x1.feb7a96a577f715fa7ad8788294c045bp-1,
    0x1.7ffffep0
  },
  { // Entry 119
    -0x1.feb7a96a577f715fa7ad8788294c045bp-1,
    -0x1.7ffffep0
  },
  { // Entry 120
    0x1.feb7eb54653509617faf3268344437fcp-1,
    0x1.8001d0p0
  },
  { // Entry 121
    -0x1.feb7eb54653509617faf3268344437fcp-1,
    -0x1.8001d0p0
  },
  { // Entry 122
    0x1.ff6535998ea004d48763597b6df9cd65p-1,
    0x1.807ffep13
  },
  { // Entry 123
    -0x1.ff6535998ea004d48763597b6df9cd65p-1,
    -0x1.807ffep13
  },
  { // Entry 124
    0x1.ff76ed9a192e9a2126bcfc0f306240d4p-1,
    0x1.866a42p0
  },
  { // Entry 125
    -0x1.ff76ed9a192e9a2126bcfc0f306240d4p-1,
    -0x1.866a42p0
  },
  { // Entry 126
    0x1.ace844f1eb6c3077277929ff4ade3f41p-5,
    0x1.8864p35
  },
  { // Entry 127
    -0x1.ace844f1eb6c3077277929ff4ade3f41p-5,
    -0x1.8864p35
  },
  { // Entry 128
    0x1.e0d108f196fd3cbcb7c2c76aee738205p-5,
    0x1.8a9b56p1
  },
  { // Entry 129
    -0x1.e0d108f196fd3cbcb7c2c76aee738205p-5,
    -0x1.8a9b56p1
  },
  { // Entry 130
    -0x1.6a09af0006fe0b47c81d8695c017a8dfp-1,
    0x1.8e96eep9
  },
  { // Entry 131
    0x1.6a09af0006fe0b47c81d8695c017a8dfp-1,
    -0x1.8e96eep9
  },
  { // Entry 132
    -0x1.dd59dceecf3fbd015e9e29b37aaf511ap-9,
    0x1.91e40ap2
  },
  { // Entry 133
    0x1.dd59dceecf3fbd015e9e29b37aaf511ap-9,
    -0x1.91e40ap2
  },
  { // Entry 134
    -0x1.fb73f1187e5053bacb752b89c6a3de57p-9,
    0x1.91fffep3
  },
  { // Entry 135
    0x1.fb73f1187e5053bacb752b89c6a3de57p-9,
    -0x1.91fffep3
  },
  { // Entry 136
    -0x1.777a5cf72cec5fd61896cb4f40d1de79p-24,
    0x1.921fb6p1
  },
  { // Entry 137
    0x1.777a5cf72cec5fd61896cb4f40d1de79p-24,
    -0x1.921fb6p1
  },
  { // Entry 138
    -0x1.ffffff79db917265e675574b8d83fa6ep-1,
    0x1.94fd7cp100
  },
  { // Entry 139
    0x1.ffffff79db917265e675574b8d83fa6ep-1,
    -0x1.94fd7cp100
  },
  { // Entry 140
    0x1.ffffffffffd7e83678a38329c1e093b5p-1,
    0x1.986834p6
  },
  { // Entry 141
    -0x1.ffffffffffd7e83678a38329c1e093b5p-1,
    -0x1.986834p6
  },
  { // Entry 142
    0x1.16ae29144d3aeb0ef741c3e2aba34078p-24,
    0x1.9a48dep17
  },
  { // Entry 143
    -0x1.16ae29144d3aeb0ef741c3e2aba34078p-24,
    -0x1.9a48dep17
  },
  { // Entry 144
    0x1.ff7ad398fd589528bb07db1e0bdd40f5p-1,
    0x1.9daa38p0
  },
  { // Entry 145
    -0x1.ff7ad398fd589528bb07db1e0bdd40f5p-1,
    -0x1.9daa38p0
  },
  { // Entry 146
    0x1.9df5f10000002f4b7421dfe9e955b60dp-4,
    0x1.9eab2ep-4
  },
  { // Entry 147
    -0x1.9df5f10000002f4b7421dfe9e955b60dp-4,
    -0x1.9eab2ep-4
  },
  { // Entry 148
    0x1.76dcf7000053c428e30ef99d9cf91accp-1,
    0x1.a499p-1
  },
  { // Entry 149
    -0x1.76dcf7000053c428e30ef99d9cf91accp-1,
    -0x1.a499p-1
  },
  { // Entry 150
    0x1.a9d4510000a2a82c391a9b36186ead65p-5,
    0x1.aa0578p-5
  },
  { // Entry 151
    -0x1.a9d4510000a2a82c391a9b36186ead65p-5,
    -0x1.aa0578p-5
  },
  { // Entry 152
    0x1.6a097dcc8b1356712fc20f82c4f87e4dp-1,
    0x1.b4ae70p6
  },
  { // Entry 153
    -0x1.6a097dcc8b1356712fc20f82c4f87e4dp-1,
    -0x1.b4ae70p6
  },
  { // Entry 154
    0x1.849926ffffeb95860262a99770a8c9c3p-1,
    0x1.b936dap-1
  },
  { // Entry 155
    -0x1.849926ffffeb95860262a99770a8c9c3p-1,
    -0x1.b936dap-1
  },
  { // Entry 156
    -0x1.dac40ef1e040315f07519a99c2034fffp-5,
    0x1.ba2784p82
  },
  { // Entry 157
    0x1.dac40ef1e040315f07519a99c2034fffp-5,
    -0x1.ba2784p82
  },
  { // Entry 158
    0x1.81074b0346f07390395b4ee2b50b3f94p-14,
    0x1.bdb44ap107
  },
  { // Entry 159
    -0x1.81074b0346f07390395b4ee2b50b3f94p-14,
    -0x1.bdb44ap107
  },
  { // Entry 160
    0x1.235debb727b95daaec183bfa4e728be1p-9,
    0x1.bdbefep119
  },
  { // Entry 161
    -0x1.235debb727b95daaec183bfa4e728be1p-9,
    -0x1.bdbefep119
  },
  { // Entry 162
    -0x1.99663da94db5290730b029513370e863p-23,
    0x1.beeeeep80
  },
  { // Entry 163
    0x1.99663da94db5290730b029513370e863p-23,
    -0x1.beeeeep80
  },
  { // Entry 164
    -0x1.62faa0ffdc55a4c54f1229fc0302bd5fp-2,
    0x1.bf6fd0p1
  },
  { // Entry 165
    0x1.62faa0ffdc55a4c54f1229fc0302bd5fp-2,
    -0x1.bf6fd0p1
  },
  { // Entry 166
    -0x1.c6d444de6b90c7b8b43402ef749a4c3dp-5,
    0x1.bffffep45
  },
  { // Entry 167
    0x1.c6d444de6b90c7b8b43402ef749a4c3dp-5,
    -0x1.bffffep45
  },
  { // Entry 168
    0x1.ffffffffffffa3c7657b85e5b44bbd44p-1,
    0x1.c463acp3
  },
  { // Entry 169
    -0x1.ffffffffffffa3c7657b85e5b44bbd44p-1,
    -0x1.c463acp3
  },
  { // Entry 170
    0x1.913648ffff9e23787f5ddb75dc55eec8p-1,
    0x1.cd0a0cp-1
  },
  { // Entry 171
    -0x1.913648ffff9e23787f5ddb75dc55eec8p-1,
    -0x1.cd0a0cp-1
  },
  { // Entry 172
    -0x1.a1d47cfd2825f1c3e2835e943b9669efp-5,
    0x1.d41818p12
  },
  { // Entry 173
    0x1.a1d47cfd2825f1c3e2835e943b9669efp-5,
    -0x1.d41818p12
  },
  { // Entry 174
    0x1.6a22db000036a9592db722e2d9be4869p-1,
    0x1.e62944p8
  },
  { // Entry 175
    -0x1.6a22db000036a9592db722e2d9be4869p-1,
    -0x1.e62944p8
  },
  { // Entry 176
    0x1.d4de8affffffd96f83ba442fdf2f7101p-2,
    0x1.e7061ep-2
  },
  { // Entry 177
    -0x1.d4de8affffffd96f83ba442fdf2f7101p-2,
    -0x1.e7061ep-2
  },
  { // Entry 178
    -0x1.e3ce04f1b94e1d82f0001cb99fed9275p-5,
    0x1.e7fffep95
  },
  { // Entry 179
    0x1.e3ce04f1b94e1d82f0001cb99fed9275p-5,
    -0x1.e7fffep95
  },
  { // Entry 180
    0x1.e97362ffff5d46e1e98c8a8c94478a44p-5,
    0x1.e9be0ep-5
  },
  { // Entry 181
    -0x1.e97362ffff5d46e1e98c8a8c94478a44p-5,
    -0x1.e9be0ep-5
  },
  { // Entry 182
    -0x1.57975cfefd7e705f3eb03a9e8e48a18cp-1,
    0x1.eb1dbep12
  },
  { // Entry 183
    0x1.57975cfefd7e705f3eb03a9e8e48a18cp-1,
    -0x1.eb1dbep12
  },
  { // Entry 184
    -0x1.6a1012e4b69b086c3dc0427aba80bc96p-1,
    0x1.eb1e8ap12
  },
  { // Entry 185
    0x1.6a1012e4b69b086c3dc0427aba80bc96p-1,
    -0x1.eb1e8ap12
  },
  { // Entry 186
    -0x1.9f42e4f467696ad36a70ac8b70e0cd32p-1,
    0x1.effc8ap24
  },
  { // Entry 187
    0x1.9f42e4f467696ad36a70ac8b70e0cd32p-1,
    -0x1.effc8ap24
  },
  { // Entry 188
    0x1.cbadb8ef8f3e358d2b8be493c11b57b1p-6,
    0x1.f5c1c4p3
  },
  { // Entry 189
    -0x1.cbadb8ef8f3e358d2b8be493c11b57b1p-6,
    -0x1.f5c1c4p3
  },
  { // Entry 190
    0x1.5aee2cfff087e59dfcd856cbfe4544dcp-2,
    0x1.f7fffep111
  },
  { // Entry 191
    -0x1.5aee2cfff087e59dfcd856cbfe4544dcp-2,
    -0x1.f7fffep111
  },
  { // Entry 192
    0x1.26db7d00bb06743fee581a888e2378f8p-1,
    0x1.f92518p24
  },
  { // Entry 193
    -0x1.26db7d00bb06743fee581a888e2378f8p-1,
    -0x1.f92518p24
  },
  { // Entry 194
    0x1.fffffffffffffebccf78a432a67858bbp-1,
    0x1.f9cbe2p7
  },
  { // Entry 195
    -0x1.fffffffffffffebccf78a432a67858bbp-1,
    -0x1.f9cbe2p7
  },
  { // Entry 196
    -0x1.1fa3bb9a07e77fd667f493622c9029ffp-27,
    0x1.f9cbe2p8
  },
  { // Entry 197
    0x1.1fa3bb9a07e77fd667f493622c9029ffp-27,
    -0x1.f9cbe2p8
  },
  { // Entry 198
    0x1.9ec29af1e15dd67f17e32047d9ff8db7p-1,
    0x1.fad160p24
  },
  { // Entry 199
    -0x1.9ec29af1e15dd67f17e32047d9ff8db7p-1,
    -0x1.fad160p24
  },
  { // Entry 200
    0x1.fbc2780f4bf8bc24d3fccc17f9e77f32p-1,
    0x1.fee5bap2
  },
  { // Entry 201
    -0x1.fbc2780f4bf8bc24d3fccc17f9e77f32p-1,
    -0x1.fee5bap2
  },
  { // Entry 202
    0x1.a8d7c10005ada8f7d4912bf603a315fep-1,
    0x1.ff0caep10
  },
  { // Entry 203
    -0x1.a8d7c10005ada8f7d4912bf603a315fep-1,
    -0x1.ff0caep10
  },
  { // Entry 204
    -0x1.56b02d00045da146e855d3770c9646fap-1,
    0x1.ff8406p80
  },
  { // Entry 205
    0x1.56b02d00045da146e855d3770c9646fap-1,
    -0x1.ff8406p80
  },
  { // Entry 206
    0x1.c02749ff4c4c8fcbb471610884d8e0bdp-3,
    0x1.ff9ffep40
  },
  { // Entry 207
    -0x1.c02749ff4c4c8fcbb471610884d8e0bdp-3,
    -0x1.ff9ffep40
  },
  { // Entry 208
    -0x1.388e710058342aa5b69afe3e2090a457p-1,
    0x1.ffbd58p24
  },
  { // Entry 209
    0x1.388e710058342aa5b69afe3e2090a457p-1,
    -0x1.ffbd58p24
  },
  { // Entry 210
    0x1.9e805aef3282d4ced40272702f174f53p-6,
    0x1.ffdd7ep72
  },
  { // Entry 211
    -0x1.9e805aef3282d4ced40272702f174f53p-6,
    -0x1.ffdd7ep72
  },
  { // Entry 212
    -0x1.fb3aef016b8ce66370ab61974e8ee0b2p-6,
    0x1.ffdffep91
  },
  { // Entry 213
    0x1.fb3aef016b8ce66370ab61974e8ee0b2p-6,
    -0x1.ffdffep91
  },
  { // Entry 214
    -0x1.e43148da53f422356edd3bdbaef3b2fap-3,
    0x1.fff77ep23
  },
  { // Entry 215
    0x1.e43148da53f422356edd3bdbaef3b2fap-3,
    -0x1.fff77ep23
  },
  { // Entry 216
    0x1.d6e1864764d24b4de18cb4c1903a7957p-1,
    0x1.fffe3ep5
  },
  { // Entry 217
    -0x1.d6e1864764d24b4de18cb4c1903a7957p-1,
    -0x1.fffe3ep5
  },
  { // Entry 218
    -0x1.ffff49c52f585e672d7185448d46e977p-1,
    0x1.fffe3ep41
  },
  { // Entry 219
    0x1.ffff49c52f585e672d7185448d46e977p-1,
    -0x1.fffe3ep41
  },
  { // Entry 220
    0x1.afc6a99cbade83b6be92b2ca45167988p-5,
    0x1.fffe3ep45
  },
  { // Entry 221
    -0x1.afc6a99cbade83b6be92b2ca45167988p-5,
    -0x1.fffe3ep45
  },
  { // Entry 222
    -0x1.53a29e54b4b142830dc796c9ed5fc59bp-9,
    0x1.fffe7ep103
  },
  { // Entry 223
    0x1.53a29e54b4b142830dc796c9ed5fc59bp-9,
    -0x1.fffe7ep103
  },
  { // Entry 224
    -0x1.47e89fd5047c4c94b5c0f61e375f2849p-11,
    0x1.fffe7ep126
  },
  { // Entry 225
    0x1.47e89fd5047c4c94b5c0f61e375f2849p-11,
    -0x1.fffe7ep126
  },
  { // Entry 226
    0x1.a4256c90c37410ce957c0b2501937618p-8,
    0x1.ffff8ep40
  },
  { // Entry 227
    -0x1.a4256c90c37410ce957c0b2501937618p-8,
    -0x1.ffff8ep40
  },
  { // Entry 228
    -0x1.d2bd84f5018f40c94c7f6e4552ef8e91p-3,
    0x1.ffffa4p23
  },
  { // Entry 229
    0x1.d2bd84f5018f40c94c7f6e4552ef8e91p-3,
    -0x1.ffffa4p23
  },
  { // Entry 230
    0x1.ffaa8cffd381ebe7b2d6a3e69bda3887p-5,
    0x1.ffffdep-5
  },
  { // Entry 231
    -0x1.ffaa8cffd381ebe7b2d6a3e69bda3887p-5,
    -0x1.ffffdep-5
  },
  { // Entry 232
    0x1.fa8d3ca23be2a84d37f0c04e97bab5e5p-1,
    0x1.fffff0p2
  },
  { // Entry 233
    -0x1.fa8d3ca23be2a84d37f0c04e97bab5e5p-1,
    -0x1.fffff0p2
  },
  { // Entry 234
    -0x1.ff978ecd320ad2d70c200824e4010843p-1,
    0x1.fffff0p7
  },
  { // Entry 235
    0x1.ff978ecd320ad2d70c200824e4010843p-1,
    -0x1.fffff0p7
  },
  { // Entry 236
    0x1.d18f76ffc6e4ba0a3134e5be21b5bc8fp-1,
    0x1.fffff6p0
  },
  { // Entry 237
    -0x1.d18f76ffc6e4ba0a3134e5be21b5bc8fp-1,
    -0x1.fffff6p0
  },
  { // Entry 238
    0x1.eaee8200b8dba5138f9ada42a22ab319p-2,
    0x1.fffffap-2
  },
  { // Entry 239
    -0x1.eaee8200b8dba5138f9ada42a22ab319p-2,
    -0x1.fffffap-2
  },
  { // Entry 240
    -0x1.0b3366508957520d9dc88d7c09337e24p-1,
    0x1.fffffep127
  },
  { // Entry 241
    0x1.0b3366508957520d9dc88d7c09337e24p-1,
    -0x1.fffffep127
  },
  { // Entry 242
    -0.0f,
    -0x1.p-149
  },
  { // Entry 243
    0.0f,
    0x1.p-149
  },
  { // Entry 244
    0.0,
    0.0
  },
  { // Entry 245
    0.0f,
    0x1.p-149
  },
  { // Entry 246
    -0.0f,
    -0x1.p-149
  },
  { // Entry 247
    -0x1.000001ffffffffffffffffffffffffffp-126,
    -0x1.000002p-126
  },
  { // Entry 248
    0x1.000001ffffffffffffffffffffffffffp-126,
    0x1.000002p-126
  },
  { // Entry 249
    -0x1.ffffffffffffffffffffffffffffffffp-127,
    -0x1.p-126
  },
  { // Entry 250
    0x1.ffffffffffffffffffffffffffffffffp-127,
    0x1.p-126
  },
  { // Entry 251
    -0x1.fffffbffffffffffffffffffffffffffp-127,
    -0x1.fffffcp-127
  },
  { // Entry 252
    0x1.fffffbffffffffffffffffffffffffffp-127,
    0x1.fffffcp-127
  },
  { // Entry 253
    0x1.fffffbffffffffffffffffffffffffffp-127,
    0x1.fffffcp-127
  },
  { // Entry 254
    -0x1.fffffbffffffffffffffffffffffffffp-127,
    -0x1.fffffcp-127
  },
  { // Entry 255
    0x1.ffffffffffffffffffffffffffffffffp-127,
    0x1.p-126
  },
  { // Entry 256
    -0x1.ffffffffffffffffffffffffffffffffp-127,
    -0x1.p-126
  },
  { // Entry 257
    0x1.000001ffffffffffffffffffffffffffp-126,
    0x1.000002p-126
  },
  { // Entry 258
    -0x1.000001ffffffffffffffffffffffffffp-126,
    -0x1.000002p-126
  },
  { // Entry 259
    0x1.999999d44f3058c789014b7d0e22fec3p-13,
    0x1.99999ap-13
  },
  { // Entry 260
    -0x1.999999d44f3058c789014b7d0e22fec3p-13,
    -0x1.99999ap-13
  },
  { // Entry 261
    0x1.999999513cc173e51bb9955b066f1db8p-12,
    0x1.99999ap-12
  },
  { // Entry 262
    -0x1.999999513cc173e51bb9955b066f1db8p-12,
    -0x1.99999ap-12
  },
  { // Entry 263
    0x1.333332d91685321e5fc397d20a5af1e6p-11,
    0x1.333334p-11
  },
  { // Entry 264
    -0x1.333332d91685321e5fc397d20a5af1e6p-11,
    -0x1.333334p-11
  },
  { // Entry 265
    0x1.99999744f306dc03e9ef70897f78299ep-11,
    0x1.99999ap-11
  },
  { // Entry 266
    -0x1.99999744f306dc03e9ef70897f78299ep-11,
    -0x1.99999ap-11
  },
  { // Entry 267
    0x1.fffffaaaaaaeeeeeed4ed4edab4c7bd6p-11,
    0x1.p-10
  },
  { // Entry 268
    -0x1.fffffaaaaaaeeeeeed4ed4edab4c7bd6p-11,
    -0x1.p-10
  },
  { // Entry 269
    0x1.33332f645a18c3b0ccfc0a3cf7b2e91bp-10,
    0x1.333334p-10
  },
  { // Entry 270
    -0x1.33332f645a18c3b0ccfc0a3cf7b2e91bp-10,
    -0x1.333334p-10
  },
  { // Entry 271
    0x1.666660aec330821c7a100cf488c380ebp-10,
    0x1.666668p-10
  },
  { // Entry 272
    -0x1.666660aec330821c7a100cf488c380ebp-10,
    -0x1.666668p-10
  },
  { // Entry 273
    0x1.99999113cc034144fdbdc8a1dc713253p-10,
    0x1.99999cp-10
  },
  { // Entry 274
    -0x1.99999113cc034144fdbdc8a1dc713253p-10,
    -0x1.99999cp-10
  },
  { // Entry 275
    0x1.ccccbc72b05dd0951a9c5e65560c56e3p-10,
    0x1.ccccccp-10
  },
  { // Entry 276
    -0x1.ccccbc72b05dd0951a9c5e65560c56e3p-10,
    -0x1.ccccccp-10
  },
  { // Entry 277
    0x1.0665ae3615b5b7de52798064dfc59b29p-7,
    0x1.066666p-7
  },
  { // Entry 278
    -0x1.0665ae3615b5b7de52798064dfc59b29p-7,
    -0x1.066666p-7
  },
  { // Entry 279
    0x1.ccc8e8ae92586d8d66b6ad0aedcb0d94p-7,
    0x1.ccccccp-7
  },
  { // Entry 280
    -0x1.ccc8e8ae92586d8d66b6ad0aedcb0d94p-7,
    -0x1.ccccccp-7
  },
  { // Entry 281
    0x1.4993e70f7b17c10af9f97fc7b33b822bp-6,
    0x1.499998p-6
  },
  { // Entry 282
    -0x1.4993e70f7b17c10af9f97fc7b33b822bp-6,
    -0x1.499998p-6
  },
  { // Entry 283
    0x1.acc043f8b2d89ad5143c030e9766fc11p-6,
    0x1.acccccp-6
  },
  { // Entry 284
    -0x1.acc043f8b2d89ad5143c030e9766fc11p-6,
    -0x1.acccccp-6
  },
  { // Entry 285
    0x1.07f44d67cf41afbc0c95108b99f91b01p-5,
    0x1.08p-5
  },
  { // Entry 286
    -0x1.07f44d67cf41afbc0c95108b99f91b01p-5,
    -0x1.08p-5
  },
  { // Entry 287
    0x1.3985fead44fa2b851e651acba369d769p-5,
    0x1.39999ap-5
  },
  { // Entry 288
    -0x1.3985fead44fa2b851e651acba369d769p-5,
    -0x1.39999ap-5
  },
  { // Entry 289
    0x1.6b14beb5d40d745096247e59b622828bp-5,
    0x1.6b3334p-5
  },
  { // Entry 290
    -0x1.6b14beb5d40d745096247e59b622828bp-5,
    -0x1.6b3334p-5
  },
  { // Entry 291
    0x1.9ca01671a7995fc97cebd69729cc4309p-5,
    0x1.9ccccep-5
  },
  { // Entry 292
    -0x1.9ca01671a7995fc97cebd69729cc4309p-5,
    -0x1.9ccccep-5
  },
  { // Entry 293
    0x1.ce278cd9eb2cbd0c6a4e1279f690e856p-5,
    0x1.ce6666p-5
  },
  { // Entry 294
    -0x1.ce278cd9eb2cbd0c6a4e1279f690e856p-5,
    -0x1.ce6666p-5
  },
  { // Entry 295
    0x1.43c1e9972391aa8ecd8a9ccba907920ap-1,
    0x1.5e7fc4p-1
  },
  { // Entry 296
    -0x1.43c1e9972391aa8ecd8a9ccba907920ap-1,
    -0x1.5e7fc4p-1
  },
  { // Entry 297
    0x1.ee3d6bb21c64b2382efcff0cdf30ce0bp-1,
    0x1.4e7fc4p0
  },
  { // Entry 298
    -0x1.ee3d6bb21c64b2382efcff0cdf30ce0bp-1,
    -0x1.4e7fc4p0
  },
  { // Entry 299
    0x1.df8e2323e4bf1a538a100ec1bf3494a9p-1,
    0x1.edbfa6p0
  },
  { // Entry 300
    -0x1.df8e2323e4bf1a538a100ec1bf3494a9p-1,
    -0x1.edbfa6p0
  },
  { // Entry 301
    0x1.1d347aa02feb3bb1750d25509435da88p-1,
    0x1.467fc4p1
  },
  { // Entry 302
    -0x1.1d347aa02feb3bb1750d25509435da88p-1,
    -0x1.467fc4p1
  },
  { // Entry 303
    -0x1.ffea08e1c97f4c4de01961cc3e7b04dcp-6,
    0x1.961fb4p1
  },
  { // Entry 304
    0x1.ffea08e1c97f4c4de01961cc3e7b04dcp-6,
    -0x1.961fb4p1
  },
  { // Entry 305
    -0x1.3734cbced9c0f484e5f762e00216e620p-1,
    0x1.e5bfa4p1
  },
  { // Entry 306
    0x1.3734cbced9c0f484e5f762e00216e620p-1,
    -0x1.e5bfa4p1
  },
  { // Entry 307
    -0x1.e9d2592bec10c3acb15c5852239a2aa8p-1,
    0x1.1aafcap2
  },
  { // Entry 308
    0x1.e9d2592bec10c3acb15c5852239a2aa8p-1,
    -0x1.1aafcap2
  },
  { // Entry 309
    -0x1.e4ece208d0c4913ab019cab0ce9c785ep-1,
    0x1.427fc2p2
  },
  { // Entry 310
    0x1.e4ece208d0c4913ab019cab0ce9c785ep-1,
    -0x1.427fc2p2
  },
  { // Entry 311
    -0x1.2a5a02d392b54f641a0d88bd4ac6c2e1p-1,
    0x1.6a4fbap2
  },
  { // Entry 312
    0x1.2a5a02d392b54f641a0d88bd4ac6c2e1p-1,
    -0x1.6a4fbap2
  },
  { // Entry 313
    -0x1.263123df22d13ed329c665c83c0e71e8p-1,
    0x1.6af2f0p2
  },
  { // Entry 314
    0x1.263123df22d13ed329c665c83c0e71e8p-1,
    -0x1.6af2f0p2
  },
  { // Entry 315
    -0x1.e18e67b508ffc9e42f6c9e72f8e545f3p-1,
    0x1.43c62ap2
  },
  { // Entry 316
    0x1.e18e67b508ffc9e42f6c9e72f8e545f3p-1,
    -0x1.43c62ap2
  },
  { // Entry 317
    -0x1.ee0e80ec9d1562c17a6cf608af9b0ed4p-1,
    0x1.1c9964p2
  },
  { // Entry 318
    0x1.ee0e80ec9d1562c17a6cf608af9b0ed4p-1,
    -0x1.1c9964p2
  },
  { // Entry 319
    -0x1.472768637ea8866a652098a43aa688a6p-1,
    0x1.ead93cp1
  },
  { // Entry 320
    0x1.472768637ea8866a652098a43aa688a6p-1,
    -0x1.ead93cp1
  },
  { // Entry 321
    -0x1.4ba24f6325f21420e7c48d4f91e28064p-4,
    0x1.9c7fb0p1
  },
  { // Entry 322
    0x1.4ba24f6325f21420e7c48d4f91e28064p-4,
    -0x1.9c7fb0p1
  },
  { // Entry 323
    0x1.034c643295153aaffe5d9a0f29e92844p-1,
    0x1.4e2624p1
  },
  { // Entry 324
    -0x1.034c643295153aaffe5d9a0f29e92844p-1,
    -0x1.4e2624p1
  },
  { // Entry 325
    0x1.d1e4d96eac917574b948e4ea9e37b36dp-1,
    0x1.ff9932p0
  },
  { // Entry 326
    -0x1.d1e4d96eac917574b948e4ea9e37b36dp-1,
    -0x1.ff9932p0
  },
  { // Entry 327
    0x1.f7501e002bcafb897f931931d3a57afdp-1,
    0x1.62e61cp0
  },
  { // Entry 328
    -0x1.f7501e002bcafb897f931931d3a57afdp-1,
    -0x1.62e61cp0
  },
  { // Entry 329
    0x1.65f7d66ef6591cfc51ab29dc2086d3e7p-1,
    0x1.8c662cp-1
  },
  { // Entry 330
    -0x1.65f7d66ef6591cfc51ab29dc2086d3e7p-1,
    -0x1.8c662cp-1
  },
  { // Entry 331
    -0x1.fe043f875c6ed4a2c1b8d69a09fcf578p-1,
    -0x1.a8aa1cp0
  },
  { // Entry 332
    0x1.fe043f875c6ed4a2c1b8d69a09fcf578p-1,
    0x1.a8aa1cp0
  },
  { // Entry 333
    -0x1.fff18f313e66f1ae25f89a5f7a1f84c4p-1,
    -0x1.95ec8ap0
  },
  { // Entry 334
    0x1.fff18f313e66f1ae25f89a5f7a1f84c4p-1,
    0x1.95ec8ap0
  },
  { // Entry 335
    -0x1.ff20d920b9e9c23154f97e2f342a2884p-1,
    -0x1.832ef8p0
  },
  { // Entry 336
    0x1.ff20d920b9e9c23154f97e2f342a2884p-1,
    0x1.832ef8p0
  },
  { // Entry 337
    -0x1.fb933b89d7db3286eed0cfeabee98875p-1,
    -0x1.707166p0
  },
  { // Entry 338
    0x1.fb933b89d7db3286eed0cfeabee98875p-1,
    0x1.707166p0
  },
  { // Entry 339
    -0x1.f54d95c5058b7dc9972ab6f9928ca043p-1,
    -0x1.5db3d4p0
  },
  { // Entry 340
    0x1.f54d95c5058b7dc9972ab6f9928ca043p-1,
    0x1.5db3d4p0
  },
  { // Entry 341
    -0x1.ec5881a09c46aae020cd9036098a196ep-1,
    -0x1.4af642p0
  },
  { // Entry 342
    0x1.ec5881a09c46aae020cd9036098a196ep-1,
    0x1.4af642p0
  },
  { // Entry 343
    -0x1.e0c04795919d961a5e83e505df31c624p-1,
    -0x1.3838b0p0
  },
  { // Entry 344
    0x1.e0c04795919d961a5e83e505df31c624p-1,
    0x1.3838b0p0
  },
  { // Entry 345
    -0x1.d294cdef7cc161633a02d3e62058be0ep-1,
    -0x1.257b1ep0
  },
  { // Entry 346
    0x1.d294cdef7cc161633a02d3e62058be0ep-1,
    0x1.257b1ep0
  },
  { // Entry 347
    -0x1.c1e988b95614abd65d3d811f5c88039bp-1,
    -0x1.12bd92p0
  },
  { // Entry 348
    0x1.c1e988b95614abd65d3d811f5c88039bp-1,
    0x1.12bd92p0
  },
  { // Entry 349
    -0x1.a2c2895edb0d4ba51cdbd5390cac468fp-1,
    -0x1.ea5c3ep-1
  },
  { // Entry 350
    0x1.a2c2895edb0d4ba51cdbd5390cac468fp-1,
    0x1.ea5c3ep-1
  },
  { // Entry 351
    -0x1.95f05153644d60b94d2f2e700dfd3a37p-1,
    -0x1.d4b87cp-1
  },
  { // Entry 352
    0x1.95f05153644d60b94d2f2e700dfd3a37p-1,
    0x1.d4b87cp-1
  },
  { // Entry 353
    -0x1.88647d8ad2e41fb7c0af0f64614c9993p-1,
    -0x1.bf14bap-1
  },
  { // Entry 354
    0x1.88647d8ad2e41fb7c0af0f64614c9993p-1,
    0x1.bf14bap-1
  },
  { // Entry 355
    -0x1.7a253f9f89a7d3e4f9c54638418e97f6p-1,
    -0x1.a970f8p-1
  },
  { // Entry 356
    0x1.7a253f9f89a7d3e4f9c54638418e97f6p-1,
    0x1.a970f8p-1
  },
  { // Entry 357
    -0x1.6b391b34aab828fbe7cd7dcaf9ef3bd6p-1,
    -0x1.93cd36p-1
  },
  { // Entry 358
    0x1.6b391b34aab828fbe7cd7dcaf9ef3bd6p-1,
    0x1.93cd36p-1
  },
  { // Entry 359
    -0x1.5ba6e2fb980d482cf00ede80f5597fb2p-1,
    -0x1.7e2974p-1
  },
  { // Entry 360
    0x1.5ba6e2fb980d482cf00ede80f5597fb2p-1,
    0x1.7e2974p-1
  },
  { // Entry 361
    -0x1.4b75b5954e718020900719e25a7ea93bp-1,
    -0x1.6885b2p-1
  },
  { // Entry 362
    0x1.4b75b5954e718020900719e25a7ea93bp-1,
    0x1.6885b2p-1
  },
  { // Entry 363
    -0x1.3aacfa510810054c52ae0b67d116eb40p-1,
    -0x1.52e1f0p-1
  },
  { // Entry 364
    0x1.3aacfa510810054c52ae0b67d116eb40p-1,
    0x1.52e1f0p-1
  },
  { // Entry 365
    -0x1.2954644ceb8e3a2479c83ae84af57d3ep-1,
    -0x1.3d3e36p-1
  },
  { // Entry 366
    0x1.2954644ceb8e3a2479c83ae84af57d3ep-1,
    0x1.3d3e36p-1
  },
  { // Entry 367
    -0x1.fc769aecd265cfea08e0ff30c2fbcacdp-2,
    -0x1.0a0b02p-1
  },
  { // Entry 368
    0x1.fc769aecd265cfea08e0ff30c2fbcacdp-2,
    0x1.0a0b02p-1
  },
  { // Entry 369
    -0x1.c853c704e3b94322031d6b47aef853c9p-2,
    -0x1.d8f720p-2
  },
  { // Entry 370
    0x1.c853c704e3b94322031d6b47aef853c9p-2,
    0x1.d8f720p-2
  },
  { // Entry 371
    -0x1.92aba8981b25deda4cc1817251723a1bp-2,
    -0x1.9dd83cp-2
  },
  { // Entry 372
    0x1.92aba8981b25deda4cc1817251723a1bp-2,
    0x1.9dd83cp-2
  },
  { // Entry 373
    -0x1.5bac05e1e0a7c2de280fcb93be67a4dap-2,
    -0x1.62b958p-2
  },
  { // Entry 374
    0x1.5bac05e1e0a7c2de280fcb93be67a4dap-2,
    0x1.62b958p-2
  },
  { // Entry 375
    -0x1.2383ca2b249807d95005d96cfdaecd6cp-2,
    -0x1.279a74p-2
  },
  { // Entry 376
    0x1.2383ca2b249807d95005d96cfdaecd6cp-2,
    0x1.279a74p-2
  },
  { // Entry 377
    -0x1.d4c5bb872ea5375834ca0bca088d1d75p-3,
    -0x1.d8f720p-3
  },
  { // Entry 378
    0x1.d4c5bb872ea5375834ca0bca088d1d75p-3,
    0x1.d8f720p-3
  },
  { // Entry 379
    -0x1.60f3fa460b85811d2ae710cd69ec3690p-3,
    -0x1.62b958p-3
  },
  { // Entry 380
    0x1.60f3fa460b85811d2ae710cd69ec3690p-3,
    0x1.62b958p-3
  },
  { // Entry 381
    -0x1.d7ea3d56e1e6244c8786d74f189d98acp-4,
    -0x1.d8f720p-4
  },
  { // Entry 382
    0x1.d7ea3d56e1e6244c8786d74f189d98acp-4,
    0x1.d8f720p-4
  },
  { // Entry 383
    -0x1.d8b3deba6ac493b04b2103a0dbaef02fp-5,
    -0x1.d8f720p-5
  },
  { // Entry 384
    0x1.d8b3deba6ac493b04b2103a0dbaef02fp-5,
    0x1.d8f720p-5
  },
  { // Entry 385
    0x1.d8b3deba6ac493b04b2103a0dbaef02fp-5,
    0x1.d8f720p-5
  },
  { // Entry 386
    -0x1.d8b3deba6ac493b04b2103a0dbaef02fp-5,
    -0x1.d8f720p-5
  },
  { // Entry 387
    0x1.d7ea3d56e1e6244c8786d74f189d98acp-4,
    0x1.d8f720p-4
  },
  { // Entry 388
    -0x1.d7ea3d56e1e6244c8786d74f189d98acp-4,
    -0x1.d8f720p-4
  },
  { // Entry 389
    0x1.60f3fa460b85811d2ae710cd69ec3690p-3,
    0x1.62b958p-3
  },
  { // Entry 390
    -0x1.60f3fa460b85811d2ae710cd69ec3690p-3,
    -0x1.62b958p-3
  },
  { // Entry 391
    0x1.d4c5bb872ea5375834ca0bca088d1d75p-3,
    0x1.d8f720p-3
  },
  { // Entry 392
    -0x1.d4c5bb872ea5375834ca0bca088d1d75p-3,
    -0x1.d8f720p-3
  },
  { // Entry 393
    0x1.2383ca2b249807d95005d96cfdaecd6cp-2,
    0x1.279a74p-2
  },
  { // Entry 394
    -0x1.2383ca2b249807d95005d96cfdaecd6cp-2,
    -0x1.279a74p-2
  },
  { // Entry 395
    0x1.5bac05e1e0a7c2de280fcb93be67a4dap-2,
    0x1.62b958p-2
  },
  { // Entry 396
    -0x1.5bac05e1e0a7c2de280fcb93be67a4dap-2,
    -0x1.62b958p-2
  },
  { // Entry 397
    0x1.92aba8981b25deda4cc1817251723a1bp-2,
    0x1.9dd83cp-2
  },
  { // Entry 398
    -0x1.92aba8981b25deda4cc1817251723a1bp-2,
    -0x1.9dd83cp-2
  },
  { // Entry 399
    0x1.c853c704e3b94322031d6b47aef853c9p-2,
    0x1.d8f720p-2
  },
  { // Entry 400
    -0x1.c853c704e3b94322031d6b47aef853c9p-2,
    -0x1.d8f720p-2
  },
  { // Entry 401
    0x1.fc769aecd265cfea08e0ff30c2fbcacdp-2,
    0x1.0a0b02p-1
  },
  { // Entry 402
    -0x1.fc769aecd265cfea08e0ff30c2fbcacdp-2,
    -0x1.0a0b02p-1
  },
  { // Entry 403
    0x1.2954644ceb8e3a2479c83ae84af57d3ep-1,
    0x1.3d3e36p-1
  },
  { // Entry 404
    -0x1.2954644ceb8e3a2479c83ae84af57d3ep-1,
    -0x1.3d3e36p-1
  },
  { // Entry 405
    0x1.3aad00a09268a39df1653c70db91f157p-1,
    0x1.52e1f8p-1
  },
  { // Entry 406
    -0x1.3aad00a09268a39df1653c70db91f157p-1,
    -0x1.52e1f8p-1
  },
  { // Entry 407
    0x1.4b75bbae388a7f3466e7f2d6bdcf72bbp-1,
    0x1.6885bap-1
  },
  { // Entry 408
    -0x1.4b75bbae388a7f3466e7f2d6bdcf72bbp-1,
    -0x1.6885bap-1
  },
  { // Entry 409
    0x1.5ba6e8db1833475712b9a42a1ad0d2c2p-1,
    0x1.7e297cp-1
  },
  { // Entry 410
    -0x1.5ba6e8db1833475712b9a42a1ad0d2c2p-1,
    -0x1.7e297cp-1
  },
  { // Entry 411
    0x1.6b3920d8117828928fe10ac70ba69e76p-1,
    0x1.93cd3ep-1
  },
  { // Entry 412
    -0x1.6b3920d8117828928fe10ac70ba69e76p-1,
    -0x1.93cd3ep-1
  },
  { // Entry 413
    0x1.7a25450443098836c5202375db4b8462p-1,
    0x1.a971p-1
  },
  { // Entry 414
    -0x1.7a25450443098836c5202375db4b8462p-1,
    -0x1.a971p-1
  },
  { // Entry 415
    0x1.886482ae6797b38364f5c72ce9a3b76fp-1,
    0x1.bf14c2p-1
  },
  { // Entry 416
    -0x1.886482ae6797b38364f5c72ce9a3b76fp-1,
    -0x1.bf14c2p-1
  },
  { // Entry 417
    0x1.95f056337acc1d2d557525232e915467p-1,
    0x1.d4b884p-1
  },
  { // Entry 418
    -0x1.95f056337acc1d2d557525232e915467p-1,
    -0x1.d4b884p-1
  },
  { // Entry 419
    0x1.a2c2895edb0d4ba51cdbd5390cac468fp-1,
    0x1.ea5c3ep-1
  },
  { // Entry 420
    -0x1.a2c2895edb0d4ba51cdbd5390cac468fp-1,
    -0x1.ea5c3ep-1
  },
  { // Entry 421
    0x1.c1e988b95614abd65d3d811f5c88039bp-1,
    0x1.12bd92p0
  },
  { // Entry 422
    -0x1.c1e988b95614abd65d3d811f5c88039bp-1,
    -0x1.12bd92p0
  },
  { // Entry 423
    0x1.d294d2e06b3d10a4de263172d50f4497p-1,
    0x1.257b24p0
  },
  { // Entry 424
    -0x1.d294d2e06b3d10a4de263172d50f4497p-1,
    -0x1.257b24p0
  },
  { // Entry 425
    0x1.e0c04bb65bd33012be72a340df2c044bp-1,
    0x1.3838b6p0
  },
  { // Entry 426
    -0x1.e0c04bb65bd33012be72a340df2c044bp-1,
    -0x1.3838b6p0
  },
  { // Entry 427
    0x1.ec5884eb990c3deaaeebd3f0f84d6962p-1,
    0x1.4af648p0
  },
  { // Entry 428
    -0x1.ec5884eb990c3deaaeebd3f0f84d6962p-1,
    -0x1.4af648p0
  },
  { // Entry 429
    0x1.f54d9835b0e66e17612160272521f3b0p-1,
    0x1.5db3dap0
  },
  { // Entry 430
    -0x1.f54d9835b0e66e17612160272521f3b0p-1,
    -0x1.5db3dap0
  },
  { // Entry 431
    0x1.fb933d1cd931685e902e403a1baaecfdp-1,
    0x1.70716cp0
  },
  { // Entry 432
    -0x1.fb933d1cd931685e902e403a1baaecfdp-1,
    -0x1.70716cp0
  },
  { // Entry 433
    0x1.ff20d9d3e8984fec33982e42f5884f2cp-1,
    0x1.832efep0
  },
  { // Entry 434
    -0x1.ff20d9d3e8984fec33982e42f5884f2cp-1,
    -0x1.832efep0
  },
  { // Entry 435
    0x1.fff18f03a4b7e6aacf51f83931e85042p-1,
    0x1.95ec90p0
  },
  { // Entry 436
    -0x1.fff18f03a4b7e6aacf51f83931e85042p-1,
    -0x1.95ec90p0
  },
  { // Entry 437
    0x1.fe043f875c6ed4a2c1b8d69a09fcf578p-1,
    0x1.a8aa1cp0
  },
  { // Entry 438
    -0x1.fe043f875c6ed4a2c1b8d69a09fcf578p-1,
    -0x1.a8aa1cp0
  },
  { // Entry 439
    0x1.b3d36a96880cf69d9884a49f5381e917p-1,
    0x1.04aff8p0
  },
  { // Entry 440
    -0x1.b3d36a96880cf69d9884a49f5381e917p-1,
    -0x1.04aff8p0
  },
  { // Entry 441
    0x1.b3d41aebcf391c30c3d2f1ee7b79710cp-1,
    0x1.04b0a0p0
  },
  { // Entry 442
    -0x1.b3d41aebcf391c30c3d2f1ee7b79710cp-1,
    -0x1.04b0a0p0
  },
  { // Entry 443
    0x1.b3d4cb405ab3292be7df5b1b98032fbep-1,
    0x1.04b148p0
  },
  { // Entry 444
    -0x1.b3d4cb405ab3292be7df5b1b98032fbep-1,
    -0x1.04b148p0
  },
  { // Entry 445
    0x1.b3d57b942a7ad19e9b9892c9319e1be6p-1,
    0x1.04b1f0p0
  },
  { // Entry 446
    -0x1.b3d57b942a7ad19e9b9892c9319e1be6p-1,
    -0x1.04b1f0p0
  },
  { // Entry 447
    0x1.b3d62be73e8fc998c6c2df6590425613p-1,
    0x1.04b298p0
  },
  { // Entry 448
    -0x1.b3d62be73e8fc998c6c2df6590425613p-1,
    -0x1.04b298p0
  },
  { // Entry 449
    0x1.b3d6dc3996f1c52aa1f83bdee1d0e023p-1,
    0x1.04b340p0
  },
  { // Entry 450
    -0x1.b3d6dc3996f1c52aa1f83bdee1d0e023p-1,
    -0x1.04b340p0
  },
  { // Entry 451
    0x1.b3d78c8b33a07864b6a878573db34bcap-1,
    0x1.04b3e8p0
  },
  { // Entry 452
    -0x1.b3d78c8b33a07864b6a878573db34bcap-1,
    -0x1.04b3e8p0
  },
  { // Entry 453
    0x1.b3d83cdc149b9757df195ad885ab5201p-1,
    0x1.04b490p0
  },
  { // Entry 454
    -0x1.b3d83cdc149b9757df195ad885ab5201p-1,
    -0x1.04b490p0
  },
  { // Entry 455
    0x1.b3d8e8f9908360b38cd13fcbf6224d93p-1,
    0x1.04b534p0
  },
  { // Entry 456
    -0x1.b3d8e8f9908360b38cd13fcbf6224d93p-1,
    -0x1.04b534p0
  },
  { // Entry 457
    -0.0f,
    -0x1.p-149
  },
  { // Entry 458
    0.0f,
    0x1.p-149
  },
  { // Entry 459
    0.0,
    0.0
  },
  { // Entry 460
    0.0f,
    0x1.p-149
  },
  { // Entry 461
    -0.0f,
    -0x1.p-149
  },
  { // Entry 462
    0x1.1773d36a64df61d6715e60af063559f4p-1,
    0x1.279a72p-1
  },
  { // Entry 463
    -0x1.1773d36a64df61d6715e60af063559f4p-1,
    -0x1.279a72p-1
  },
  { // Entry 464
    0x1.1773d51767a78fe91b55f6b7e5fd44c2p-1,
    0x1.279a74p-1
  },
  { // Entry 465
    -0x1.1773d51767a78fe91b55f6b7e5fd44c2p-1,
    -0x1.279a74p-1
  },
  { // Entry 466
    0x1.1773d6c46a6ea687f03625194d25bb52p-1,
    0x1.279a76p-1
  },
  { // Entry 467
    -0x1.1773d6c46a6ea687f03625194d25bb52p-1,
    -0x1.279a76p-1
  },
  { // Entry 468
    0x1.f95b8f40501057ac49acef13993b0c55p-1,
    0x1.bb67acp0
  },
  { // Entry 469
    -0x1.f95b8f40501057ac49acef13993b0c55p-1,
    -0x1.bb67acp0
  },
  { // Entry 470
    0x1.f95b8e9be727702f7595ae1000a14a1ap-1,
    0x1.bb67aep0
  },
  { // Entry 471
    -0x1.f95b8e9be727702f7595ae1000a14a1ap-1,
    -0x1.bb67aep0
  },
  { // Entry 472
    0x1.f95b8df77e36a344670ed07149191a58p-1,
    0x1.bb67b0p0
  },
  { // Entry 473
    -0x1.f95b8df77e36a344670ed07149191a58p-1,
    -0x1.bb67b0p0
  },
  { // Entry 474
    0x1.b1d82e835a918de18f5fdadc8b1240cfp-2,
    0x1.bffffep-2
  },
  { // Entry 475
    -0x1.b1d82e835a918de18f5fdadc8b1240cfp-2,
    -0x1.bffffep-2
  },
  { // Entry 476
    0x1.b1d83053216169476f4d1982b9b14ab1p-2,
    0x1.c0p-2
  },
  { // Entry 477
    -0x1.b1d83053216169476f4d1982b9b14ab1p-2,
    -0x1.c0p-2
  },
  { // Entry 478
    0x1.b1d83222e830d83743258fd09040ee56p-2,
    0x1.c00002p-2
  },
  { // Entry 479
    -0x1.b1d83222e830d83743258fd09040ee56p-2,
    -0x1.c00002p-2
  },
  { // Entry 480
    0x1.44eb3691428062b27925c585ad59d62ap-1,
    0x1.5ffffep-1
  },
  { // Entry 481
    -0x1.44eb3691428062b27925c585ad59d62ap-1,
    -0x1.5ffffep-1
  },
  { // Entry 482
    0x1.44eb381cf386ab04a4f8656abea80b83p-1,
    0x1.60p-1
  },
  { // Entry 483
    -0x1.44eb381cf386ab04a4f8656abea80b83p-1,
    -0x1.60p-1
  },
  { // Entry 484
    0x1.44eb39a8a48bae6b98ae11c9400535e5p-1,
    0x1.600002p-1
  },
  { // Entry 485
    -0x1.44eb39a8a48bae6b98ae11c9400535e5p-1,
    -0x1.600002p-1
  },
  { // Entry 486
    0x1.dad9017b96408c375d4faf0e4776d1fcp-1,
    0x1.2ffffep0
  },
  { // Entry 487
    -0x1.dad9017b96408c375d4faf0e4776d1fcp-1,
    -0x1.2ffffep0
  },
  { // Entry 488
    0x1.dad902fa8ac870f52f1b843ac83bc3edp-1,
    0x1.30p0
  },
  { // Entry 489
    -0x1.dad902fa8ac870f52f1b843ac83bc3edp-1,
    -0x1.30p0
  },
  { // Entry 490
    0x1.dad904797f48ea4ef4fd2e47fe4d52bdp-1,
    0x1.300002p0
  },
  { // Entry 491
    -0x1.dad904797f48ea4ef4fd2e47fe4d52bdp-1,
    -0x1.300002p0
  },
  { // Entry 492
    0x1.4b708093c9cb45355e7821e5aad98ce8p-1,
    0x1.37fffep1
  },
  { // Entry 493
    -0x1.4b708093c9cb45355e7821e5aad98ce8p-1,
    -0x1.37fffep1
  },
  { // Entry 494
    0x1.4b707a7acdecc84239463e78b312fa10p-1,
    0x1.38p1
  },
  { // Entry 495
    -0x1.4b707a7acdecc84239463e78b312fa10p-1,
    -0x1.38p1
  },
  { // Entry 496
    0x1.4b707461d1f994476c677c5ad5ddb264p-1,
    0x1.380002p1
  },
  { // Entry 497
    -0x1.4b707461d1f994476c677c5ad5ddb264p-1,
    -0x1.380002p1
  },
  { // Entry 498
    0x1.066e7f705a6ca2b9e107f7dc9f3b26e6p-4,
    0x1.069c8cp-4
  },
  { // Entry 499
    -0x1.066e7f705a6ca2b9e107f7dc9f3b26e6p-4,
    -0x1.069c8cp-4
  },
  { // Entry 500
    0x1.05e476d27febc8b7e9690009b367c327p-3,
    0x1.069c8cp-3
  },
  { // Entry 501
    -0x1.05e476d27febc8b7e9690009b367c327p-3,
    -0x1.069c8cp-3
  },
  { // Entry 502
    0x1.877e2de5c9a066b8db595adc149af0c0p-3,
    0x1.89ead2p-3
  },
  { // Entry 503
    -0x1.877e2de5c9a066b8db595adc149af0c0p-3,
    -0x1.89ead2p-3
  },
  { // Entry 504
    0x1.03be07acb9dab719b4343a33b9fa6afep-2,
    0x1.069c8cp-2
  },
  { // Entry 505
    -0x1.03be07acb9dab719b4343a33b9fa6afep-2,
    -0x1.069c8cp-2
  },
  { // Entry 506
    0x1.42abbc5b3b2f91e8ece46e5effd28369p-2,
    0x1.4843b0p-2
  },
  { // Entry 507
    -0x1.42abbc5b3b2f91e8ece46e5effd28369p-2,
    -0x1.4843b0p-2
  },
  { // Entry 508
    0x1.804601411d93f4750919670061de07d9p-2,
    0x1.89ead4p-2
  },
  { // Entry 509
    -0x1.804601411d93f4750919670061de07d9p-2,
    -0x1.89ead4p-2
  },
  { // Entry 510
    0x1.bc4c08af356088b1694995bfaf8a297bp-2,
    0x1.cb91f8p-2
  },
  { // Entry 511
    -0x1.bc4c08af356088b1694995bfaf8a297bp-2,
    -0x1.cb91f8p-2
  },
  { // Entry 512
    0x1.f67eae34dc0b42b465fd2a3fb07564a4p-2,
    0x1.069c8ep-1
  },
  { // Entry 513
    -0x1.f67eae34dc0b42b465fd2a3fb07564a4p-2,
    -0x1.069c8ep-1
  },
  { // Entry 514
    0x1.17505c86231898fd86b18d2282d93eedp-1,
    0x1.277020p-1
  },
  { // Entry 515
    -0x1.17505c86231898fd86b18d2282d93eedp-1,
    -0x1.277020p-1
  },
  { // Entry 516
    0x1.323b8e40d16575e50dc7b6e567bb5084p-1,
    0x1.4843b2p-1
  },
  { // Entry 517
    -0x1.323b8e40d16575e50dc7b6e567bb5084p-1,
    -0x1.4843b2p-1
  },
  { // Entry 518
    0x1.4be49b08a1e1629cbdaa507e18255cd8p-1,
    0x1.691744p-1
  },
  { // Entry 519
    -0x1.4be49b08a1e1629cbdaa507e18255cd8p-1,
    -0x1.691744p-1
  },
  { // Entry 520
    0x1.6430847dbbbfd46cbebbc6d5f51c7c49p-1,
    0x1.89ead6p-1
  },
  { // Entry 521
    -0x1.6430847dbbbfd46cbebbc6d5f51c7c49p-1,
    -0x1.89ead6p-1
  },
  { // Entry 522
    0x1.7b05bb87b38844e56003c41ef804b273p-1,
    0x1.aabe68p-1
  },
  { // Entry 523
    -0x1.7b05bb87b38844e56003c41ef804b273p-1,
    -0x1.aabe68p-1
  },
  { // Entry 524
    0x1.904c3b389d55d3deddb39d05eb366571p-1,
    0x1.cb91fap-1
  },
  { // Entry 525
    -0x1.904c3b389d55d3deddb39d05eb366571p-1,
    -0x1.cb91fap-1
  },
  { // Entry 526
    0x1.a3eda211798a82697d62431f9ae46cc4p-1,
    0x1.ec658cp-1
  },
  { // Entry 527
    -0x1.a3eda211798a82697d62431f9ae46cc4p-1,
    -0x1.ec658cp-1
  },
  { // Entry 528
    0x1.b5d54883fcb6123bc28aac91f085e4eep-1,
    0x1.069c8ep0
  },
  { // Entry 529
    -0x1.b5d54883fcb6123bc28aac91f085e4eep-1,
    -0x1.069c8ep0
  },
  { // Entry 530
    0x1.c5f05a0135d4882c768cdf18e2e1112cp-1,
    0x1.170656p0
  },
  { // Entry 531
    -0x1.c5f05a0135d4882c768cdf18e2e1112cp-1,
    -0x1.170656p0
  },
  { // Entry 532
    0x1.d42de53e315c839ce188e201205e99dep-1,
    0x1.27701ep0
  },
  { // Entry 533
    -0x1.d42de53e315c839ce188e201205e99dep-1,
    -0x1.27701ep0
  },
  { // Entry 534
    0x1.e07eef45d91eea8a6cc7369aa0e55388p-1,
    0x1.37d9e6p0
  },
  { // Entry 535
    -0x1.e07eef45d91eea8a6cc7369aa0e55388p-1,
    -0x1.37d9e6p0
  },
  { // Entry 536
    0x1.ead6833b2aa002baa1c2b19a38dc9b79p-1,
    0x1.4843aep0
  },
  { // Entry 537
    -0x1.ead6833b2aa002baa1c2b19a38dc9b79p-1,
    -0x1.4843aep0
  },
  { // Entry 538
    0x1.f329bffa6a208591eecb6905d7594e3bp-1,
    0x1.58ad76p0
  },
  { // Entry 539
    -0x1.f329bffa6a208591eecb6905d7594e3bp-1,
    -0x1.58ad76p0
  },
  { // Entry 540
    0x1.f96fe38afbd95b5fcd08608110e9381fp-1,
    0x1.69173ep0
  },
  { // Entry 541
    -0x1.f96fe38afbd95b5fcd08608110e9381fp-1,
    -0x1.69173ep0
  },
  { // Entry 542
    0x1.fda25455d9567772f20f25d15efc6775p-1,
    0x1.798106p0
  },
  { // Entry 543
    -0x1.fda25455d9567772f20f25d15efc6775p-1,
    -0x1.798106p0
  },
  { // Entry 544
    0x1.ffbca816f1f1516ec5d757b0db54ae34p-1,
    0x1.89eacep0
  },
  { // Entry 545
    -0x1.ffbca816f1f1516ec5d757b0db54ae34p-1,
    -0x1.89eacep0
  },
  { // Entry 546
    0x1.ffbca88228b163189ab8d637db99bd2dp-1,
    0x1.9a5496p0
  },
  { // Entry 547
    -0x1.ffbca88228b163189ab8d637db99bd2dp-1,
    -0x1.9a5496p0
  },
  { // Entry 548
    0x1.fda255970ccddb9d127ecf63403c2bf7p-1,
    0x1.aabe5ep0
  },
  { // Entry 549
    -0x1.fda255970ccddb9d127ecf63403c2bf7p-1,
    -0x1.aabe5ep0
  },
  { // Entry 550
    0x1.f96fe5a0da244489fb2f4b97b3e48757p-1,
    0x1.bb2826p0
  },
  { // Entry 551
    -0x1.f96fe5a0da244489fb2f4b97b3e48757p-1,
    -0x1.bb2826p0
  },
  { // Entry 552
    0x1.f329c2e2c1a39bad8ecdcb87961ba44ap-1,
    0x1.cb91eep0
  },
  { // Entry 553
    -0x1.f329c2e2c1a39bad8ecdcb87961ba44ap-1,
    -0x1.cb91eep0
  },
  { // Entry 554
    0x1.ead686f2ec572c83ed34a01f764d193ep-1,
    0x1.dbfbb6p0
  },
  { // Entry 555
    -0x1.ead686f2ec572c83ed34a01f764d193ep-1,
    -0x1.dbfbb6p0
  },
  { // Entry 556
    0x1.e07ef3c91bd500a0de230ad573163163p-1,
    0x1.ec657ep0
  },
  { // Entry 557
    -0x1.e07ef3c91bd500a0de230ad573163163p-1,
    -0x1.ec657ep0
  },
  { // Entry 558
    0x1.d42dea8835c88adb9cde17347f934e25p-1,
    0x1.fccf46p0
  },
  { // Entry 559
    -0x1.d42dea8835c88adb9cde17347f934e25p-1,
    -0x1.fccf46p0
  },
  { // Entry 560
    0x1.c5f05e32c80fb0fe603033ec028a4c32p-1,
    0x1.069c88p1
  },
  { // Entry 561
    -0x1.c5f05e32c80fb0fe603033ec028a4c32p-1,
    -0x1.069c88p1
  },
  { // Entry 562
    0x1.b5d54d3732d3b2e79d4907e115401ddap-1,
    0x1.0ed16cp1
  },
  { // Entry 563
    -0x1.b5d54d3732d3b2e79d4907e115401ddap-1,
    -0x1.0ed16cp1
  },
  { // Entry 564
    0x1.a3eda74161d06b83ec2c8dc396d813b9p-1,
    0x1.170650p1
  },
  { // Entry 565
    -0x1.a3eda74161d06b83ec2c8dc396d813b9p-1,
    -0x1.170650p1
  },
  { // Entry 566
    0x1.904c421efce58f4e8170d36dcda8e02cp-1,
    0x1.1f3b34p1
  },
  { // Entry 567
    -0x1.904c421efce58f4e8170d36dcda8e02cp-1,
    -0x1.1f3b34p1
  },
  { // Entry 568
    0x1.7b05c45093944d6afb0c90d2f9cb217fp-1,
    0x1.277018p1
  },
  { // Entry 569
    -0x1.7b05c45093944d6afb0c90d2f9cb217fp-1,
    -0x1.277018p1
  },
  { // Entry 570
    0x1.64308f506ffdaf1326d10b3380278e98p-1,
    0x1.2fa4fcp1
  },
  { // Entry 571
    -0x1.64308f506ffdaf1326d10b3380278e98p-1,
    -0x1.2fa4fcp1
  },
  { // Entry 572
    0x1.4be4a8076c135a48f3f1a1aaa362475fp-1,
    0x1.37d9e0p1
  },
  { // Entry 573
    -0x1.4be4a8076c135a48f3f1a1aaa362475fp-1,
    -0x1.37d9e0p1
  },
  { // Entry 574
    0x1.323b9d888d4da77a610893735eeed1cbp-1,
    0x1.400ec4p1
  },
  { // Entry 575
    -0x1.323b9d888d4da77a610893735eeed1cbp-1,
    -0x1.400ec4p1
  },
  { // Entry 576
    0x1.17506e2dfb603d34b9af39b12c1db735p-1,
    0x1.4843a8p1
  },
  { // Entry 577
    -0x1.17506e2dfb603d34b9af39b12c1db735p-1,
    -0x1.4843a8p1
  },
  { // Entry 578
    0x1.f67ed667352d4827450013f15e321bfbp-2,
    0x1.50788cp1
  },
  { // Entry 579
    -0x1.f67ed667352d4827450013f15e321bfbp-2,
    -0x1.50788cp1
  },
  { // Entry 580
    0x1.bc4c35da51e34b776e5e04da58f23441p-2,
    0x1.58ad70p1
  },
  { // Entry 581
    -0x1.bc4c35da51e34b776e5e04da58f23441p-2,
    -0x1.58ad70p1
  },
  { // Entry 582
    0x1.8046336e68427cf756056d3f4edbb662p-2,
    0x1.60e254p1
  },
  { // Entry 583
    -0x1.8046336e68427cf756056d3f4edbb662p-2,
    -0x1.60e254p1
  },
  { // Entry 584
    0x1.42abf3872905e632f204c41b24af90b6p-2,
    0x1.691738p1
  },
  { // Entry 585
    -0x1.42abf3872905e632f204c41b24af90b6p-2,
    -0x1.691738p1
  },
  { // Entry 586
    0x1.03be43c699f3536990dcf5a6665ac239p-2,
    0x1.714c1cp1
  },
  { // Entry 587
    -0x1.03be43c699f3536990dcf5a6665ac239p-2,
    -0x1.714c1cp1
  },
  { // Entry 588
    0x1.877eadc2fdfc2f0db1e8b78cd3fbfbd2p-3,
    0x1.7981p1
  },
  { // Entry 589
    -0x1.877eadc2fdfc2f0db1e8b78cd3fbfbd2p-3,
    -0x1.7981p1
  },
  { // Entry 590
    0x1.05e4fdf846632a8208d90de72d3a6da8p-3,
    0x1.81b5e4p1
  },
  { // Entry 591
    -0x1.05e4fdf846632a8208d90de72d3a6da8p-3,
    -0x1.81b5e4p1
  },
  { // Entry 592
    0x1.066f9b630b72dff16450e89afdf7e048p-4,
    0x1.89eac8p1
  },
  { // Entry 593
    -0x1.066f9b630b72dff16450e89afdf7e048p-4,
    -0x1.89eac8p1
  },
  { // Entry 594
    0x1.03bdf0b79ccf739529d54d422861046cp-2,
    -0x1.81b5eep2
  },
  { // Entry 595
    -0x1.03bdf0b79ccf739529d54d422861046cp-2,
    0x1.81b5eep2
  },
  { // Entry 596
    0x1.f67e8b95f5460ea369a803837b721abdp-2,
    -0x1.714c26p2
  },
  { // Entry 597
    -0x1.f67e8b95f5460ea369a803837b721abdp-2,
    0x1.714c26p2
  },
  { // Entry 598
    0x1.643070791751dc0636d1854d2bdbc5d4p-1,
    -0x1.60e25ep2
  },
  { // Entry 599
    -0x1.643070791751dc0636d1854d2bdbc5d4p-1,
    0x1.60e25ep2
  },
  { // Entry 600
    0x1.b5d536f59113a43af30e8c9db8a951a5p-1,
    -0x1.507896p2
  },
  { // Entry 601
    -0x1.b5d536f59113a43af30e8c9db8a951a5p-1,
    0x1.507896p2
  },
  { // Entry 602
    0x1.ead679985549140318349f512dca7a6bp-1,
    -0x1.400ecep2
  },
  { // Entry 603
    -0x1.ead679985549140318349f512dca7a6bp-1,
    0x1.400ecep2
  },
  { // Entry 604
    0x1.ffbca7010e0b0452f56075cfd5982880p-1,
    -0x1.2fa506p2
  },
  { // Entry 605
    -0x1.ffbca7010e0b0452f56075cfd5982880p-1,
    0x1.2fa506p2
  },
  { // Entry 606
    0x1.f329ca6bfc7425d89c2b4b9ad73ab108p-1,
    -0x1.1f3b3ep2
  },
  { // Entry 607
    -0x1.f329ca6bfc7425d89c2b4b9ad73ab108p-1,
    0x1.1f3b3ep2
  },
  { // Entry 608
    0x1.c5f06fb69427ac0f2d69428d82b5e669p-1,
    -0x1.0ed176p2
  },
  { // Entry 609
    -0x1.c5f06fb69427ac0f2d69428d82b5e669p-1,
    0x1.0ed176p2
  },
  { // Entry 610
    0x1.7b05d864ec9802adbc4b5577c233836ap-1,
    -0x1.fccf5ap1
  },
  { // Entry 611
    -0x1.7b05d864ec9802adbc4b5577c233836ap-1,
    0x1.fccf5ap1
  },
  { // Entry 612
    0x1.1750808185a998bbcecc3a6ac0cb2907p-1,
    -0x1.dbfbc8p1
  },
  { // Entry 613
    -0x1.1750808185a998bbcecc3a6ac0cb2907p-1,
    0x1.dbfbc8p1
  },
  { // Entry 614
    0x1.42ac0dd9495211816bf04ca53bce4beap-2,
    -0x1.bb2836p1
  },
  { // Entry 615
    -0x1.42ac0dd9495211816bf04ca53bce4beap-2,
    0x1.bb2836p1
  },
  { // Entry 616
    0x1.066fca39a70b52d06f2cd7eab69c31f2p-4,
    -0x1.9a54a4p1
  },
  { // Entry 617
    -0x1.066fca39a70b52d06f2cd7eab69c31f2p-4,
    0x1.9a54a4p1
  },
  { // Entry 618
    -0x1.877d931298e6fbc654f065536cff2b54p-3,
    -0x1.798112p1
  },
  { // Entry 619
    0x1.877d931298e6fbc654f065536cff2b54p-3,
    0x1.798112p1
  },
  { // Entry 620
    -0x1.bc4bc2875eb6d38eda3b49cb2320b561p-2,
    -0x1.58ad80p1
  },
  { // Entry 621
    0x1.bc4bc2875eb6d38eda3b49cb2320b561p-2,
    0x1.58ad80p1
  },
  { // Entry 622
    -0x1.4be47d6354c4ced53780b1b519acdec2p-1,
    -0x1.37d9eep1
  },
  { // Entry 623
    0x1.4be47d6354c4ced53780b1b519acdec2p-1,
    0x1.37d9eep1
  },
  { // Entry 624
    -0x1.a3ed8bcb35cbcf8c6089f82a91c31d5bp-1,
    -0x1.17065cp1
  },
  { // Entry 625
    0x1.a3ed8bcb35cbcf8c6089f82a91c31d5bp-1,
    0x1.17065cp1
  },
  { // Entry 626
    -0x1.e07ee496ea109654c42e171fdc4537c4p-1,
    -0x1.ec6594p0
  },
  { // Entry 627
    0x1.e07ee496ea109654c42e171fdc4537c4p-1,
    0x1.ec6594p0
  },
  { // Entry 628
    -0x1.fda2522219689d0e8069d90f5c969b92p-1,
    -0x1.aabe70p0
  },
  { // Entry 629
    0x1.fda2522219689d0e8069d90f5c969b92p-1,
    0x1.aabe70p0
  },
  { // Entry 630
    -0x1.f96fe802fe570372d0fcb6e934b43061p-1,
    -0x1.69174cp0
  },
  { // Entry 631
    0x1.f96fe802fe570372d0fcb6e934b43061p-1,
    0x1.69174cp0
  },
  { // Entry 632
    -0x1.d42ded56ae88a6e1cf270af27e6f1804p-1,
    -0x1.277028p0
  },
  { // Entry 633
    0x1.d42ded56ae88a6e1cf270af27e6f1804p-1,
    0x1.277028p0
  },
  { // Entry 634
    -0x1.904c45326d6dde224381d1d590ada41cp-1,
    -0x1.cb920ap-1
  },
  { // Entry 635
    0x1.904c45326d6dde224381d1d590ada41cp-1,
    0x1.cb920ap-1
  },
  { // Entry 636
    -0x1.323b9cadbb19e75a44483fb64ad8ddf6p-1,
    -0x1.4843c4p-1
  },
  { // Entry 637
    0x1.323b9cadbb19e75a44483fb64ad8ddf6p-1,
    0x1.4843c4p-1
  },
  { // Entry 638
    -0x1.80462654bde766faf47f3140e290996dp-2,
    -0x1.89eafcp-2
  },
  { // Entry 639
    0x1.80462654bde766faf47f3140e290996dp-2,
    0x1.89eafcp-2
  },
  { // Entry 640
    -0x1.05e4ca21f386a82bc2e4efcdebb1962bp-3,
    -0x1.069ce0p-3
  },
  { // Entry 641
    0x1.05e4ca21f386a82bc2e4efcdebb1962bp-3,
    0x1.069ce0p-3
  },
  { // Entry 642
    0x1.05e423830be01f9fe3c57d06867e0056p-3,
    0x1.069c38p-3
  },
  { // Entry 643
    -0x1.05e423830be01f9fe3c57d06867e0056p-3,
    -0x1.069c38p-3
  },
  { // Entry 644
    0x1.8045d87852f1307fea6dc751c4d15992p-2,
    0x1.89eaa8p-2
  },
  { // Entry 645
    -0x1.8045d87852f1307fea6dc751c4d15992p-2,
    -0x1.89eaa8p-2
  },
  { // Entry 646
    0x1.323b7b04ee88cff98b2a1620e1f61a01p-1,
    0x1.48439ap-1
  },
  { // Entry 647
    -0x1.323b7b04ee88cff98b2a1620e1f61a01p-1,
    -0x1.48439ap-1
  },
  { // Entry 648
    0x1.904c2b02aa59528ce044bf2213c96859p-1,
    0x1.cb91e0p-1
  },
  { // Entry 649
    -0x1.904c2b02aa59528ce044bf2213c96859p-1,
    -0x1.cb91e0p-1
  },
  { // Entry 650
    0x1.d42ddd25b3797e6a679f76e05e6c3e08p-1,
    0x1.277014p0
  },
  { // Entry 651
    -0x1.d42ddd25b3797e6a679f76e05e6c3e08p-1,
    -0x1.277014p0
  },
  { // Entry 652
    0x1.f96fe1a0b12d0ad4fa8c82d8af989c5ap-1,
    0x1.691738p0
  },
  { // Entry 653
    -0x1.f96fe1a0b12d0ad4fa8c82d8af989c5ap-1,
    -0x1.691738p0
  },
  { // Entry 654
    0x1.fda255f96094d8fe4e859c4cf0dd68a5p-1,
    0x1.aabe5cp0
  },
  { // Entry 655
    -0x1.fda255f96094d8fe4e859c4cf0dd68a5p-1,
    -0x1.aabe5cp0
  },
  { // Entry 656
    0x1.e07ef267748b982778f8d50d2981bb3ap-1,
    0x1.ec6580p0
  },
  { // Entry 657
    -0x1.e07ef267748b982778f8d50d2981bb3ap-1,
    -0x1.ec6580p0
  },
  { // Entry 658
    0x1.a3eda2adb01143fb21453b20bd1748fep-1,
    0x1.170652p1
  },
  { // Entry 659
    -0x1.a3eda2adb01143fb21453b20bd1748fep-1,
    -0x1.170652p1
  },
  { // Entry 660
    0x1.4be49bd88a64a0bb414ddacac4fa8de9p-1,
    0x1.37d9e4p1
  },
  { // Entry 661
    -0x1.4be49bd88a64a0bb414ddacac4fa8de9p-1,
    -0x1.37d9e4p1
  },
  { // Entry 662
    0x1.bc4c0a9b3782e220ae55786369ccf190p-2,
    0x1.58ad76p1
  },
  { // Entry 663
    -0x1.bc4c0a9b3782e220ae55786369ccf190p-2,
    -0x1.58ad76p1
  },
  { // Entry 664
    0x1.877e301f43cafffe6644a8958f108729p-3,
    0x1.798108p1
  },
  { // Entry 665
    -0x1.877e301f43cafffe6644a8958f108729p-3,
    -0x1.798108p1
  },
  { // Entry 666
    -0x1.066e8ae1f824a69817e6a806e6317e28p-4,
    0x1.9a549ap1
  },
  { // Entry 667
    0x1.066e8ae1f824a69817e6a806e6317e28p-4,
    -0x1.9a549ap1
  },
  { // Entry 668
    -0x1.42abc1eca11a0ad12ca6eeff197318aap-2,
    0x1.bb282cp1
  },
  { // Entry 669
    0x1.42abc1eca11a0ad12ca6eeff197318aap-2,
    -0x1.bb282cp1
  },
  { // Entry 670
    -0x1.17505efb8119773c647468be1dfee45ep-1,
    0x1.dbfbbep1
  },
  { // Entry 671
    0x1.17505efb8119773c647468be1dfee45ep-1,
    -0x1.dbfbbep1
  },
  { // Entry 672
    -0x1.7b05bd8091cd79dff359c8412b0de1a9p-1,
    0x1.fccf50p1
  },
  { // Entry 673
    0x1.7b05bd8091cd79dff359c8412b0de1a9p-1,
    -0x1.fccf50p1
  },
  { // Entry 674
    -0x1.c5f05982eabf022748960961666d540dp-1,
    0x1.0ed170p2
  },
  { // Entry 675
    0x1.c5f05982eabf022748960961666d540dp-1,
    -0x1.0ed170p2
  },
  { // Entry 676
    -0x1.f329bfbda8122f83e3a1ea0242eb76aap-1,
    0x1.1f3b38p2
  },
  { // Entry 677
    0x1.f329bfbda8122f83e3a1ea0242eb76aap-1,
    -0x1.1f3b38p2
  },
  { // Entry 678
    -0x1.ffbca88ae90f0900b6d3ad89eddd2c80p-1,
    0x1.2fa5p2
  },
  { // Entry 679
    0x1.ffbca88ae90f0900b6d3ad89eddd2c80p-1,
    -0x1.2fa5p2
  },
  { // Entry 680
    -0x1.ead687409c95dcaf61af98513517f507p-1,
    0x1.400ec8p2
  },
  { // Entry 681
    0x1.ead687409c95dcaf61af98513517f507p-1,
    -0x1.400ec8p2
  },
  { // Entry 682
    -0x1.b5d54fd79372b90d5d4c7acf7adaed42p-1,
    0x1.507890p2
  },
  { // Entry 683
    0x1.b5d54fd79372b90d5d4c7acf7adaed42p-1,
    -0x1.507890p2
  },
  { // Entry 684
    -0x1.643092f42ae797375531420c005ca2cfp-1,
    0x1.60e258p2
  },
  { // Entry 685
    0x1.643092f42ae797375531420c005ca2cfp-1,
    -0x1.60e258p2
  },
  { // Entry 686
    -0x1.f67edf3b7bee8554d54d84ea83f6cb21p-2,
    0x1.714c20p2
  },
  { // Entry 687
    0x1.f67edf3b7bee8554d54d84ea83f6cb21p-2,
    -0x1.714c20p2
  },
  { // Entry 688
    -0x1.03be4d93d949325340b2f464201545a7p-2,
    0x1.81b5e8p2
  },
  { // Entry 689
    0x1.03be4d93d949325340b2f464201545a7p-2,
    -0x1.81b5e8p2
  },
  { // Entry 690
    0x1.efb26cfa20f2098ff7e9e42f0260eb01p-5,
    0x1.effffep-5
  },
  { // Entry 691
    -0x1.efb26cfa20f2098ff7e9e42f0260eb01p-5,
    -0x1.effffep-5
  },
  { // Entry 692
    0x1.efb26ef930c4d3f2b0dbe1931ba5ae64p-5,
    0x1.f0p-5
  },
  { // Entry 693
    -0x1.efb26ef930c4d3f2b0dbe1931ba5ae64p-5,
    -0x1.f0p-5
  },
  { // Entry 694
    0x1.efb270f840979c65b75ee5c67016a866p-5,
    0x1.f00002p-5
  },
  { // Entry 695
    -0x1.efb270f840979c65b75ee5c67016a866p-5,
    -0x1.f00002p-5
  },
  { // Entry 696
    0x1.f6baa816fce5ea5a60d8c9fd2a289380p-4,
    0x1.f7fffep-4
  },
  { // Entry 697
    -0x1.f6baa816fce5ea5a60d8c9fd2a289380p-4,
    -0x1.f7fffep-4
  },
  { // Entry 698
    0x1.f6baaa131de6438e5611279864fe7663p-4,
    0x1.f8p-4
  },
  { // Entry 699
    -0x1.f6baaa131de6438e5611279864fe7663p-4,
    -0x1.f8p-4
  },
  { // Entry 700
    0x1.f6baac0f3ee694e760a138bc06c8be3dp-4,
    0x1.f80002p-4
  },
  { // Entry 701
    -0x1.f6baac0f3ee694e760a138bc06c8be3dp-4,
    -0x1.f80002p-4
  },
  { // Entry 702
    0x1.4a8c395552fb432af31780e883c98f71p-3,
    0x1.4bfffep-3
  },
  { // Entry 703
    -0x1.4a8c395552fb432af31780e883c98f71p-3,
    -0x1.4bfffep-3
  },
  { // Entry 704
    0x1.4a8c3b4e9c7fffd48305f44a42f5f50fp-3,
    0x1.4cp-3
  },
  { // Entry 705
    -0x1.4a8c3b4e9c7fffd48305f44a42f5f50fp-3,
    -0x1.4cp-3
  },
  { // Entry 706
    0x1.4a8c3d47e604a7d54f3f7de402409e2cp-3,
    0x1.4c0002p-3
  },
  { // Entry 707
    -0x1.4a8c3d47e604a7d54f3f7de402409e2cp-3,
    -0x1.4c0002p-3
  },
  { // Entry 708
    0x1.2e9cd83630eb35c12efcfb8413583998p-2,
    0x1.333332p-2
  },
  { // Entry 709
    -0x1.2e9cd83630eb35c12efcfb8413583998p-2,
    -0x1.333332p-2
  },
  { // Entry 710
    0x1.2e9cda1f52c88042833f236ff0f9d486p-2,
    0x1.333334p-2
  },
  { // Entry 711
    -0x1.2e9cda1f52c88042833f236ff0f9d486p-2,
    -0x1.333334p-2
  },
  { // Entry 712
    0x1.2e9cdc0874a57f1ca0f976a9b01e4a71p-2,
    0x1.333336p-2
  },
  { // Entry 713
    -0x1.2e9cdc0874a57f1ca0f976a9b01e4a71p-2,
    -0x1.333336p-2
  },
  { // Entry 714
    0x1.3faefb2b68e6786eb692bd4e4045213ep-1,
    0x1.594316p-1
  },
  { // Entry 715
    -0x1.3faefb2b68e6786eb692bd4e4045213ep-1,
    -0x1.594316p-1
  },
  { // Entry 716
    0x1.3faefcbb57c26b0d84b63dbfb72b413bp-1,
    0x1.594318p-1
  },
  { // Entry 717
    -0x1.3faefcbb57c26b0d84b63dbfb72b413bp-1,
    -0x1.594318p-1
  },
  { // Entry 718
    0x1.3faefe4b469d1dfd561e666edda7c6e6p-1,
    0x1.59431ap-1
  },
  { // Entry 719
    -0x1.3faefe4b469d1dfd561e666edda7c6e6p-1,
    -0x1.59431ap-1
  },
  { // Entry 720
    0x1.6888a375ab228c1e031c4005769509f9p-1,
    0x1.8ffffep-1
  },
  { // Entry 721
    -0x1.6888a375ab228c1e031c4005769509f9p-1,
    -0x1.8ffffep-1
  },
  { // Entry 722
    0x1.6888a4e134b2ea520b226eca8694b3a2p-1,
    0x1.90p-1
  },
  { // Entry 723
    -0x1.6888a4e134b2ea520b226eca8694b3a2p-1,
    -0x1.90p-1
  },
  { // Entry 724
    0x1.6888a64cbe41dffd6e4768dcca4db53bp-1,
    0x1.900002p-1
  },
  { // Entry 725
    -0x1.6888a64cbe41dffd6e4768dcca4db53bp-1,
    -0x1.900002p-1
  },
  { // Entry 726
    -0.0f,
    -0x1.p-149
  },
  { // Entry 727
    0.0f,
    0x1.p-149
  },
  { // Entry 728
    0.0,
    0.0
  },
  { // Entry 729
    0.0f,
    0x1.p-149
  },
  { // Entry 730
    -0.0f,
    -0x1.p-149
  },
  { // Entry 731
    0x1.91f65dccfead353d8db9c32f12262730p-5,
    0x1.921fb4p-5
  },
  { // Entry 732
    -0x1.91f65dccfead353d8db9c32f12262730p-5,
    -0x1.921fb4p-5
  },
  { // Entry 733
    0x1.91f65fcc60cb6d09fcc5c35dd6a798c8p-5,
    0x1.921fb6p-5
  },
  { // Entry 734
    -0x1.91f65fcc60cb6d09fcc5c35dd6a798c8p-5,
    -0x1.921fb6p-5
  },
  { // Entry 735
    0x1.91f661cbc2e9a3447571f72bcfbc21e2p-5,
    0x1.921fb8p-5
  },
  { // Entry 736
    -0x1.91f661cbc2e9a3447571f72bcfbc21e2p-5,
    -0x1.921fb8p-5
  },
  { // Entry 737
    0x1.917a6a7fe8297bf0a1125fb02b2038aep-4,
    0x1.921fb4p-4
  },
  { // Entry 738
    -0x1.917a6a7fe8297bf0a1125fb02b2038aep-4,
    -0x1.921fb4p-4
  },
  { // Entry 739
    0x1.917a6c7d7103b9d90e09615164449c6bp-4,
    0x1.921fb6p-4
  },
  { // Entry 740
    -0x1.917a6c7d7103b9d90e09615164449c6bp-4,
    -0x1.921fb6p-4
  },
  { // Entry 741
    0x1.917a6e7af9ddf17b914e6d2e8e83b33ep-4,
    0x1.921fb8p-4
  },
  { // Entry 742
    -0x1.917a6e7af9ddf17b914e6d2e8e83b33ep-4,
    -0x1.921fb8p-4
  },
  { // Entry 743
    0x1.8f8b82889296b5cf7904db1e74b3466bp-3,
    0x1.921fb4p-3
  },
  { // Entry 744
    -0x1.8f8b82889296b5cf7904db1e74b3466bp-3,
    -0x1.921fb4p-3
  },
  { // Entry 745
    0x1.8f8b847ebc13b8998ec5b37e7065341ep-3,
    0x1.921fb6p-3
  },
  { // Entry 746
    -0x1.8f8b847ebc13b8998ec5b37e7065341ep-3,
    -0x1.921fb6p-3
  },
  { // Entry 747
    0x1.8f8b8674e590a26aec3ea01d30aed486p-3,
    0x1.921fb8p-3
  },
  { // Entry 748
    -0x1.8f8b8674e590a26aec3ea01d30aed486p-3,
    -0x1.921fb8p-3
  },
  { // Entry 749
    0x1.87de293f569717a42a3bdb01aeae2063p-2,
    0x1.921fb4p-2
  },
  { // Entry 750
    -0x1.87de293f569717a42a3bdb01aeae2063p-2,
    -0x1.921fb4p-2
  },
  { // Entry 751
    0x1.87de2b185d5417dca800b85ca1319043p-2,
    0x1.921fb6p-2
  },
  { // Entry 752
    -0x1.87de2b185d5417dca800b85ca1319043p-2,
    -0x1.921fb6p-2
  },
  { // Entry 753
    0x1.87de2cf16410b61d9aff7e628fc853b2p-2,
    0x1.921fb8p-2
  },
  { // Entry 754
    -0x1.87de2cf16410b61d9aff7e628fc853b2p-2,
    -0x1.921fb8p-2
  },
  { // Entry 755
    0x1.6a09e582aa3945461b5a8a0787d5ab5bp-1,
    0x1.921fb4p-1
  },
  { // Entry 756
    -0x1.6a09e582aa3945461b5a8a0787d5ab5bp-1,
    -0x1.921fb4p-1
  },
  { // Entry 757
    0x1.6a09e6ecb41fdd7e681872c854887019p-1,
    0x1.921fb6p-1
  },
  { // Entry 758
    -0x1.6a09e6ecb41fdd7e681872c854887019p-1,
    -0x1.921fb6p-1
  },
  { // Entry 759
    0x1.6a09e856be050baccde9a76961e84aa7p-1,
    0x1.921fb8p-1
  },
  { // Entry 760
    -0x1.6a09e856be050baccde9a76961e84aa7p-1,
    -0x1.921fb8p-1
  },
  { // Entry 761
    0x1.fffffffffffe6546cc38211c26dabeebp-1,
    0x1.921fb4p0
  },
  { // Entry 762
    -0x1.fffffffffffe6546cc38211c26dabeebp-1,
    -0x1.921fb4p0
  },
  { // Entry 763
    0x1.ffffffffffff76521249c7422930ed82p-1,
    0x1.921fb6p0
  },
  { // Entry 764
    -0x1.ffffffffffff76521249c7422930ed82p-1,
    -0x1.921fb6p0
  },
  { // Entry 765
    0x1.fffffffffff8875d585b6d6cfce97d9cp-1,
    0x1.921fb8p0
  },
  { // Entry 766
    -0x1.fffffffffff8875d585b6d6cfce97d9cp-1,
    -0x1.921fb8p0
  },
  { // Entry 767
    0x1.4442d184698831f15b6315bfa6b5ae75p-23,
    0x1.921fb4p1
  },
  { // Entry 768
    -0x1.4442d184698831f15b6315bfa6b5ae75p-23,
    -0x1.921fb4p1
  },
  { // Entry 769
    -0x1.777a5cf72cec5fd61896cb4f40d1de79p-24,
    0x1.921fb6p1
  },
  { // Entry 770
    0x1.777a5cf72cec5fd61896cb4f40d1de79p-24,
    -0x1.921fb6p1
  },
  { // Entry 771
    -0x1.5dde973dcb346afa46203cddc6f7fe97p-22,
    0x1.921fb8p1
  },
  { // Entry 772
    0x1.5dde973dcb346afa46203cddc6f7fe97p-22,
    -0x1.921fb8p1
  },
  { // Entry 773
    -0x1.4442d1846984217628872e56eb58b4c1p-22,
    0x1.921fb4p2
  },
  { // Entry 774
    0x1.4442d1846984217628872e56eb58b4c1p-22,
    -0x1.921fb4p2
  },
  { // Entry 775
    0x1.777a5cf72ceacbf6ec657e977ef771f1p-23,
    0x1.921fb6p2
  },
  { // Entry 776
    -0x1.777a5cf72ceacbf6ec657e977ef771f1p-23,
    -0x1.921fb6p2
  },
  { // Entry 777
    0x1.5dde973dcb1fff10bb0388479e82f4bbp-21,
    0x1.921fb8p2
  },
  { // Entry 778
    -0x1.5dde973dcb1fff10bb0388479e82f4bbp-21,
    -0x1.921fb8p2
  },
  { // Entry 779
    -0x1.4442d1846973df895d1791023ded513cp-21,
    0x1.921fb4p3
  },
  { // Entry 780
    0x1.4442d1846973df895d1791023ded513cp-21,
    -0x1.921fb4p3
  },
  { // Entry 781
    0x1.777a5cf72ce47c7a3ba04bc2a607a9a7p-22,
    0x1.921fb6p3
  },
  { // Entry 782
    -0x1.777a5cf72ce47c7a3ba04bc2a607a9a7p-22,
    -0x1.921fb6p3
  },
  { // Entry 783
    0x1.5dde973dcace4f6a8e90bd15e00610f8p-20,
    0x1.921fb8p3
  },
  { // Entry 784
    -0x1.5dde973dcace4f6a8e90bd15e00610f8p-20,
    -0x1.921fb8p3
  },
  { // Entry 785
    -0x1.4442d1846932d7d62f59209388c7f7cap-20,
    0x1.921fb4p4
  },
  { // Entry 786
    0x1.4442d1846932d7d62f59209388c7f7cap-20,
    -0x1.921fb4p4
  },
  { // Entry 787
    0x1.777a5cf72ccb3e87788b811229e725bcp-21,
    0x1.921fb6p4
  },
  { // Entry 788
    -0x1.777a5cf72ccb3e87788b811229e725bcp-21,
    -0x1.921fb6p4
  },
  { // Entry 789
    0x1.5dde973dc98790d1dcc602bd1b86bccap-19,
    0x1.921fb8p4
  },
  { // Entry 790
    -0x1.5dde973dc98790d1dcc602bd1b86bccap-19,
    -0x1.921fb8p4
  },
  { // Entry 791
    -0x1.4442d184682eb909785fad18bcb5dbfcp-19,
    0x1.921fb4p5
  },
  { // Entry 792
    0x1.4442d184682eb909785fad18bcb5dbfcp-19,
    -0x1.921fb4p5
  },
  { // Entry 793
    0x1.777a5cf72c6646bc6c38607eb34eea13p-20,
    0x1.921fb6p5
  },
  { // Entry 794
    -0x1.777a5cf72c6646bc6c38607eb34eea13p-20,
    -0x1.921fb6p5
  },
  { // Entry 795
    0x1.5dde973dc46c966f15a2403d60cd14d0p-18,
    0x1.921fb8p5
  },
  { // Entry 796
    -0x1.5dde973dc46c966f15a2403d60cd14d0p-18,
    -0x1.921fb8p5
  },
  { // Entry 797
    -0x1.4442d184641e3dd69c7ec32e14a209a5p-18,
    0x1.921fb4p6
  },
  { // Entry 798
    0x1.4442d184641e3dd69c7ec32e14a209a5p-18,
    -0x1.921fb4p6
  },
  { // Entry 799
    0x1.777a5cf72ad267903aec8118778b3b5ap-19,
    0x1.921fb6p6
  },
  { // Entry 800
    -0x1.777a5cf72ad267903aec8118778b3b5ap-19,
    -0x1.921fb6p6
  },
  { // Entry 801
    0x1.5dde973db000ace3f985a473ea1fc039p-17,
    0x1.921fb8p6
  },
  { // Entry 802
    -0x1.5dde973db000ace3f985a473ea1fc039p-17,
    -0x1.921fb8p6
  },
  { // Entry 803
    -0x1.4442d18453dc510b2d495b8bf79bd1cep-17,
    0x1.921fb4p7
  },
  { // Entry 804
    0x1.4442d18453dc510b2d495b8bf79bd1cep-17,
    -0x1.921fb4p7
  },
  { // Entry 805
    0x1.777a5cf72482eadf75c731f972507718p-18,
    0x1.921fb6p7
  },
  { // Entry 806
    -0x1.777a5cf72482eadf75c731f972507718p-18,
    -0x1.921fb6p7
  },
  { // Entry 807
    0x1.5dde973d5e5106b7903a18a552aefc6ep-16,
    0x1.921fb8p7
  },
  { // Entry 808
    -0x1.5dde973d5e5106b7903a18a552aefc6ep-16,
    -0x1.921fb8p7
  },
  { // Entry 809
    0x1.6a09f1940b80c8e25cfc8c10d42576c6p-1,
    0x1.2d97c4p1
  },
  { // Entry 810
    -0x1.6a09f1940b80c8e25cfc8c10d42576c6p-1,
    -0x1.2d97c4p1
  },
  { // Entry 811
    0x1.6a09ebebe40889245d57c5c9d90a6d82p-1,
    0x1.2d97c6p1
  },
  { // Entry 812
    -0x1.6a09ebebe40889245d57c5c9d90a6d82p-1,
    -0x1.2d97c6p1
  },
  { // Entry 813
    0x1.6a09e643bc79a8c79ef4bf187727e269p-1,
    0x1.2d97c8p1
  },
  { // Entry 814
    -0x1.6a09e643bc79a8c79ef4bf187727e269p-1,
    -0x1.2d97c8p1
  },
  { // Entry 815
    -0x1.6a09df19704cf14108e09000ff6374bfp-1,
    0x1.f6a7a0p1
  },
  { // Entry 816
    0x1.6a09df19704cf14108e09000ff6374bfp-1,
    -0x1.f6a7a0p1
  },
  { // Entry 817
    -0x1.6a09e4c197f87ace1c81b43022be39b1p-1,
    0x1.f6a7a2p1
  },
  { // Entry 818
    0x1.6a09e4c197f87ace1c81b43022be39b1p-1,
    -0x1.f6a7a2p1
  },
  { // Entry 819
    -0x1.6a09ea69bf8d63bce40958f5c4b4f155p-1,
    0x1.f6a7a4p1
  },
  { // Entry 820
    0x1.6a09ea69bf8d63bce40958f5c4b4f155p-1,
    -0x1.f6a7a4p1
  },
  { // Entry 821
    -0x1.ffffffffff065cb240bb8f9519d2c6f1p-1,
    0x1.2d97c4p2
  },
  { // Entry 822
    0x1.ffffffffff065cb240bb8f9519d2c6f1p-1,
    -0x1.2d97c4p2
  },
  { // Entry 823
    -0x1.ffffffffffc32939898f464aafc2e74ap-1,
    0x1.2d97c6p2
  },
  { // Entry 824
    0x1.ffffffffffc32939898f464aafc2e74ap-1,
    -0x1.2d97c6p2
  },
  { // Entry 825
    -0x1.fffffffffffff5c0d2630ee0a1fb4e7bp-1,
    0x1.2d97c8p2
  },
  { // Entry 826
    0x1.fffffffffffff5c0d2630ee0a1fb4e7bp-1,
    -0x1.2d97c8p2
  },
  { // Entry 827
    -0x1.6a09f529316cde5a190d235cc4ccb825p-1,
    0x1.5fdbbcp2
  },
  { // Entry 828
    0x1.6a09f529316cde5a190d235cc4ccb825p-1,
    -0x1.5fdbbcp2
  },
  { // Entry 829
    -0x1.6a09e9d8e2826770567ea818b2e89960p-1,
    0x1.5fdbbep2
  },
  { // Entry 830
    0x1.6a09e9d8e2826770567ea818b2e89960p-1,
    -0x1.5fdbbep2
  },
  { // Entry 831
    -0x1.6a09de88933d6e0c1db78e1d7cd15173p-1,
    0x1.5fdbc0p2
  },
  { // Entry 832
    0x1.6a09de88933d6e0c1db78e1d7cd15173p-1,
    -0x1.5fdbc0p2
  },
  { // Entry 833
    0x1.6a09d033fa715a407a6f03d01b91113fp-1,
    0x1.c463a8p2
  },
  { // Entry 834
    -0x1.6a09d033fa715a407a6f03d01b91113fp-1,
    -0x1.c463a8p2
  },
  { // Entry 835
    0x1.6a09db844a28f8635851fdf8818515efp-1,
    0x1.c463aap2
  },
  { // Entry 836
    -0x1.6a09db844a28f8635851fdf8818515efp-1,
    -0x1.c463aap2
  },
  { // Entry 837
    0x1.6a09e6d49986140f55226fc58672612cp-1,
    0x1.c463acp2
  },
  { // Entry 838
    -0x1.6a09e6d49986140f55226fc58672612cp-1,
    -0x1.c463acp2
  },
  { // Entry 839
    0x1.ffffffffff95397934cac1f28532d3d3p-1,
    0x1.f6a7a0p2
  },
  { // Entry 840
    -0x1.ffffffffff95397934cac1f28532d3d3p-1,
    -0x1.f6a7a0p2
  },
  { // Entry 841
    0x1.fffffffffffa8e5aae2bb93ae590f984p-1,
    0x1.f6a7a2p2
  },
  { // Entry 842
    -0x1.fffffffffffa8e5aae2bb93ae590f984p-1,
    -0x1.f6a7a2p2
  },
  { // Entry 843
    0x1.ffffffffffdfe33c278cb48a59ee3ef2p-1,
    0x1.f6a7a4p2
  },
  { // Entry 844
    -0x1.ffffffffffdfe33c278cb48a59ee3ef2p-1,
    -0x1.f6a7a4p2
  },
  { // Entry 845
    0x1.6a0a040ea5c32ba4afbeb86a614c5d16p-1,
    0x1.1475cap3
  },
  { // Entry 846
    -0x1.6a0a040ea5c32ba4afbeb86a614c5d16p-1,
    -0x1.1475cap3
  },
  { // Entry 847
    0x1.6a09ed6e088212b1e260a5132d6959b7p-1,
    0x1.1475ccp3
  },
  { // Entry 848
    -0x1.6a09ed6e088212b1e260a5132d6959b7p-1,
    -0x1.1475ccp3
  },
  { // Entry 849
    0x1.6a09d6cd69d6efd1a6fa2dd4c617cbbep-1,
    0x1.1475cep3
  },
  { // Entry 850
    -0x1.6a09d6cd69d6efd1a6fa2dd4c617cbbep-1,
    -0x1.1475cep3
  },
  { // Entry 851
    0x1.f9990e91a64ae486757878bdfee0f703p-20,
    0x1.2d97c4p3
  },
  { // Entry 852
    -0x1.f9990e91a64ae486757878bdfee0f703p-20,
    -0x1.2d97c4p3
  },
  { // Entry 853
    0x1.f3321d234ed8128aabb0499a43b4def2p-21,
    0x1.2d97c6p3
  },
  { // Entry 854
    -0x1.f3321d234ed8128aabb0499a43b4def2p-21,
    -0x1.2d97c6p3
  },
  { // Entry 855
    -0x1.99bc5b961b1ac296dbe1980fd2c890a0p-26,
    0x1.2d97c8p3
  },
  { // Entry 856
    0x1.99bc5b961b1ac296dbe1980fd2c890a0p-26,
    -0x1.2d97c8p3
  },
  { // Entry 857
    -0x1.6a09c14e83f8db080d1223f887cc12ecp-1,
    0x1.46b9c0p3
  },
  { // Entry 858
    0x1.6a09c14e83f8db080d1223f887cc12ecp-1,
    -0x1.46b9c0p3
  },
  { // Entry 859
    -0x1.6a09d7ef23fbec1ed812e807beb0492fp-1,
    0x1.46b9c2p3
  },
  { // Entry 860
    0x1.6a09d7ef23fbec1ed812e807beb0492fp-1,
    -0x1.46b9c2p3
  },
  { // Entry 861
    -0x1.6a09ee8fc294f35db3efce565365af89p-1,
    0x1.46b9c4p3
  },
  { // Entry 862
    0x1.6a09ee8fc294f35db3efce565365af89p-1,
    -0x1.46b9c4p3
  },
  { // Entry 863
    -0x1.fffffffffe4c96b397d951cb21861c95p-1,
    0x1.5fdbbcp3
  },
  { // Entry 864
    0x1.fffffffffe4c96b397d951cb21861c95p-1,
    -0x1.5fdbbcp3
  },
  { // Entry 865
    -0x1.ffffffffffe8512aebb56c9e75b41941p-1,
    0x1.5fdbbep3
  },
  { // Entry 866
    0x1.ffffffffffe8512aebb56c9e75b41941p-1,
    -0x1.5fdbbep3
  },
  { // Entry 867
    -0x1.ffffffffff840ba23f91c9cb49a10b27p-1,
    0x1.5fdbc0p3
  },
  { // Entry 868
    0x1.ffffffffff840ba23f91c9cb49a10b27p-1,
    -0x1.5fdbc0p3
  },
  { // Entry 869
    -0x1.6a0a12f4197c90a0ee4a094b6377aa23p-1,
    0x1.78fdb6p3
  },
  { // Entry 870
    0x1.6a0a12f4197c90a0ee4a094b6377aa23p-1,
    -0x1.78fdb6p3
  },
  { // Entry 871
    -0x1.6a09fc537d29cf131d6710991bebabedp-1,
    0x1.78fdb8p3
  },
  { // Entry 872
    0x1.6a09fc537d29cf131d6710991bebabedp-1,
    -0x1.78fdb8p3
  },
  { // Entry 873
    -0x1.6a09e5b2df6d0388f9070c4340f3e669p-1,
    0x1.78fdbap3
  },
  { // Entry 874
    0x1.6a09e5b2df6d0388f9070c4340f3e669p-1,
    -0x1.78fdbap3
  },
  { // Entry 875
    0x1.6a09c909add4dbf32253a39d5c306308p-1,
    0x1.ab41aep3
  },
  { // Entry 876
    -0x1.6a09c909add4dbf32253a39d5c306308p-1,
    -0x1.ab41aep3
  },
  { // Entry 877
    0x1.6a09dfaa4d5c3a7f056f3e61a365b29ep-1,
    0x1.ab41b0p3
  },
  { // Entry 878
    -0x1.6a09dfaa4d5c3a7f056f3e61a365b29ep-1,
    -0x1.ab41b0p3
  },
  { // Entry 879
    0x1.6a09f64aeb798f2b3e3d9b16e8e3c412p-1,
    0x1.ab41b2p3
  },
  { // Entry 880
    -0x1.6a09f64aeb798f2b3e3d9b16e8e3c412p-1,
    -0x1.ab41b2p3
  },
  { // Entry 881
    0x1.fffffffffc260d6ffb8f4cd8ab3fd020p-1,
    0x1.c463a8p3
  },
  { // Entry 882
    -0x1.fffffffffc260d6ffb8f4cd8ab3fd020p-1,
    -0x1.c463a8p3
  },
  { // Entry 883
    0x1.ffffffffff12d89bb084dd762848b3d6p-1,
    0x1.c463aap3
  },
  { // Entry 884
    -0x1.ffffffffff12d89bb084dd762848b3d6p-1,
    -0x1.c463aap3
  },
  { // Entry 885
    0x1.ffffffffffffa3c7657b85e5b44bbd44p-1,
    0x1.c463acp3
  },
  { // Entry 886
    -0x1.ffffffffffffa3c7657b85e5b44bbd44p-1,
    -0x1.c463acp3
  },
  { // Entry 887
    0x1.6a0a0b38f134a3295a0b386e42f1ca7ap-1,
    0x1.dd85a4p3
  },
  { // Entry 888
    -0x1.6a0a0b38f134a3295a0b386e42f1ca7ap-1,
    -0x1.dd85a4p3
  },
  { // Entry 889
    0x1.6a09f49854662eff1b35755a129044a7p-1,
    0x1.dd85a6p3
  },
  { // Entry 890
    -0x1.6a09f49854662eff1b35755a129044a7p-1,
    -0x1.dd85a6p3
  },
  { // Entry 891
    0x1.6a09ddf7b62db0e0440b6a4262203b11p-1,
    0x1.dd85a8p3
  },
  { // Entry 892
    -0x1.6a09ddf7b62db0e0440b6a4262203b11p-1,
    -0x1.dd85a8p3
  },
  { // Entry 893
    0x1.4aa9c2f2c19a062b18a017bcd5424feap-20,
    0x1.f6a7a0p3
  },
  { // Entry 894
    -0x1.4aa9c2f2c19a062b18a017bcd5424feap-20,
    -0x1.f6a7a0p3
  },
  { // Entry 895
    0x1.2aa70bcb07d3a40781510d213652e43ap-22,
    0x1.f6a7a2p3
  },
  { // Entry 896
    -0x1.2aa70bcb07d3a40781510d213652e43ap-22,
    -0x1.f6a7a2p3
  },
  { // Entry 897
    -0x1.6aac7a1a7bf5bbd49572ffb8d7749922p-21,
    0x1.f6a7a4p3
  },
  { // Entry 898
    0x1.6aac7a1a7bf5bbd49572ffb8d7749922p-21,
    -0x1.f6a7a4p3
  },
  { // Entry 899
    -0x1.6a09a383953124096898340f9168b9d5p-1,
    0x1.07e4ccp4
  },
  { // Entry 900
    0x1.6a09a383953124096898340f9168b9d5p-1,
    -0x1.07e4ccp4
  },
  { // Entry 901
    -0x1.6a09d0c4d7869961c47a9b0b968cc910p-1,
    0x1.07e4cep4
  },
  { // Entry 902
    0x1.6a09d0c4d7869961c47a9b0b968cc910p-1,
    -0x1.07e4cep4
  },
  { // Entry 903
    -0x1.6a09fe061433e7770d00ca59d5a56251p-1,
    0x1.07e4d0p4
  },
  { // Entry 904
    0x1.6a09fe061433e7770d00ca59d5a56251p-1,
    -0x1.07e4d0p4
  },
  { // Entry 905
    -0x1.fffffffff9219dae5feda1b539335803p-1,
    0x1.1475cap4
  },
  { // Entry 906
    0x1.fffffffff9219dae5feda1b539335803p-1,
    -0x1.1475cap4
  },
  { // Entry 907
    -0x1.ffffffffff9d556e8c0bf0a80d610808p-1,
    0x1.1475ccp4
  },
  { // Entry 908
    0x1.ffffffffff9d556e8c0bf0a80d610808p-1,
    -0x1.1475ccp4
  },
  { // Entry 909
    -0x1.fffffffffe190d2eb82e74efd2093215p-1,
    0x1.1475cep4
  },
  { // Entry 910
    0x1.fffffffffe190d2eb82e74efd2093215p-1,
    -0x1.1475cep4
  },
  { // Entry 911
    -0x1.6a0a1a1e64a28eee238dc852846aacd5p-1,
    0x1.2106c8p4
  },
  { // Entry 912
    0x1.6a0a1a1e64a28eee238dc852846aacd5p-1,
    -0x1.2106c8p4
  },
  { // Entry 913
    -0x1.6a09ecdd2b784b699034ee8102670e27p-1,
    0x1.2106cap4
  },
  { // Entry 914
    0x1.6a09ecdd2b784b699034ee8102670e27p-1,
    -0x1.2106cap4
  },
  { // Entry 915
    -0x1.6a09bf9beca5e03188301639c09ed574p-1,
    0x1.2106ccp4
  },
  { // Entry 916
    0x1.6a09bf9beca5e03188301639c09ed574p-1,
    -0x1.2106ccp4
  },
  { // Entry 917
    -0x1.f9990e91a270d3bc1c02f4f69f48e675p-19,
    0x1.2d97c4p4
  },
  { // Entry 918
    0x1.f9990e91a270d3bc1c02f4f69f48e675p-19,
    -0x1.2d97c4p4
  },
  { // Entry 919
    -0x1.f3321d234deacd6f3afd75039685012fp-20,
    0x1.2d97c6p4
  },
  { // Entry 920
    0x1.f3321d234deacd6f3afd75039685012fp-20,
    -0x1.2d97c6p4
  },
  { // Entry 921
    0x1.99bc5b961b1aa1c9e8023074f3406fd9p-25,
    0x1.2d97c8p4
  },
  { // Entry 922
    -0x1.99bc5b961b1aa1c9e8023074f3406fd9p-25,
    -0x1.2d97c8p4
  },
  { // Entry 923
    0x1.6a09949e1ce1ec501afcb35d731bf62cp-1,
    0x1.3a28c2p4
  },
  { // Entry 924
    -0x1.6a09949e1ce1ec501afcb35d731bf62cp-1,
    -0x1.3a28c2p4
  },
  { // Entry 925
    0x1.6a09c1df6114100c65d1ff6c55755e72p-1,
    0x1.3a28c4p4
  },
  { // Entry 926
    -0x1.6a09c1df6114100c65d1ff6c55755e72p-1,
    -0x1.3a28c4p4
  },
  { // Entry 927
    0x1.6a09ef209f9e0cc13324ddf2b361553fp-1,
    0x1.3a28c6p4
  },
  { // Entry 928
    -0x1.6a09ef209f9e0cc13324ddf2b361553fp-1,
    -0x1.3a28c6p4
  },
  { // Entry 929
    0x1.fffffffff53f476ec4f59f26c4bcdfa0p-1,
    0x1.46b9c0p4
  },
  { // Entry 930
    -0x1.fffffffff53f476ec4f59f26c4bcdfa0p-1,
    -0x1.46b9c0p4
  },
  { // Entry 931
    0x1.fffffffffe5d2097b34334ad679dd7a4p-1,
    0x1.46b9c2p4
  },
  { // Entry 932
    -0x1.fffffffffe5d2097b34334ad679dd7a4p-1,
    -0x1.46b9c2p4
  },
  { // Entry 933
    0x1.ffffffffff7af9c0a19a005c565c6af7p-1,
    0x1.46b9c4p4
  },
  { // Entry 934
    -0x1.ffffffffff7af9c0a19a005c565c6af7p-1,
    -0x1.46b9c4p4
  },
  { // Entry 935
    0x1.6a0a2903d773925b052fb006ac670c23p-1,
    0x1.534abep4
  },
  { // Entry 936
    -0x1.6a0a2903d773925b052fb006ac670c23p-1,
    -0x1.534abep4
  },
  { // Entry 937
    0x1.6a09fbc2a025fdae918466fa00142143p-1,
    0x1.534ac0p4
  },
  { // Entry 938
    -0x1.6a09fbc2a025fdae918466fa00142143p-1,
    -0x1.534ac0p4
  },
  { // Entry 939
    0x1.6a09ce8163304113135a68ae93d3fa0ep-1,
    0x1.534ac2p4
  },
  { // Entry 940
    -0x1.6a09ce8163304113135a68ae93d3fa0ep-1,
    -0x1.534ac2p4
  },
  { // Entry 941
    0x1.4ddd3ba9ecb19d6bb6ea161120e447b9p-19,
    0x1.5fdbbcp4
  },
  { // Entry 942
    -0x1.4ddd3ba9ecb19d6bb6ea161120e447b9p-19,
    -0x1.5fdbbcp4
  },
  { // Entry 943
    0x1.3774eea7b89d80df7816fe208ec69fc0p-21,
    0x1.5fdbbep4
  },
  { // Entry 944
    -0x1.3774eea7b89d80df7816fe208ec69fc0p-21,
    -0x1.5fdbbep4
  },
  { // Entry 945
    -0x1.644588ac2334a3d5452d9960282cf80dp-20,
    0x1.5fdbc0p4
  },
  { // Entry 946
    0x1.644588ac2334a3d5452d9960282cf80dp-20,
    -0x1.5fdbc0p4
  },
  { // Entry 947
    -0x1.6a09b2f9ea049e855e35ca9ce7e0d89ap-1,
    0x1.6c6cbap4
  },
  { // Entry 948
    0x1.6a09b2f9ea049e855e35ca9ce7e0d89ap-1,
    -0x1.6c6cbap4
  },
  { // Entry 949
    -0x1.6a09e03b2a6b49c6134c67b42baee668p-1,
    0x1.6c6cbcp4
  },
  { // Entry 950
    0x1.6a09e03b2a6b49c6134c67b42baee668p-1,
    -0x1.6c6cbcp4
  },
  { // Entry 951
    -0x1.6a0a0d7c6529cd85dbbb3a5c2cd3fae5p-1,
    0x1.6c6cbep4
  },
  { // Entry 952
    0x1.6a0a0d7c6529cd85dbbb3a5c2cd3fae5p-1,
    -0x1.6c6cbep4
  },
  { // Entry 953
    -0x1.fffffffff07f0ab12aa8f41f29c15392p-1,
    0x1.78fdb6p4
  },
  { // Entry 954
    0x1.fffffffff07f0ab12aa8f41f29c15392p-1,
    -0x1.78fdb6p4
  },
  { // Entry 955
    -0x1.fffffffffc3f0542db21dcbcb847dac3p-1,
    0x1.78fdb8p4
  },
  { // Entry 956
    0x1.fffffffffc3f0542db21dcbcb847dac3p-1,
    -0x1.78fdb8p4
  },
  { // Entry 957
    -0x1.fffffffffffeffd48bac73efe60c7fcfp-1,
    0x1.78fdbap4
  },
  { // Entry 958
    0x1.fffffffffffeffd48bac73efe60c7fcfp-1,
    -0x1.78fdbap4
  },
  { // Entry 959
    -0x1.6a0a37e949a7ad698a32234c73e5afbap-1,
    0x1.858eb4p4
  },
  { // Entry 960
    0x1.6a0a37e949a7ad698a32234c73e5afbap-1,
    -0x1.858eb4p4
  },
  { // Entry 961
    -0x1.6a0a0aa81436c7a8d33a38d704030d14p-1,
    0x1.858eb6p4
  },
  { // Entry 962
    0x1.6a0a0aa81436c7a8d33a38d704030d14p-1,
    -0x1.858eb6p4
  },
  { // Entry 963
    -0x1.6a09dd66d91db9bd7bf355faff08f194p-1,
    0x1.858eb8p4
  },
  { // Entry 964
    0x1.6a09dd66d91db9bd7bf355faff08f194p-1,
    -0x1.858eb8p4
  },
  { // Entry 965
    0x1.c048b38a8bbf59f414fec7079209926ep-3,
    0x1.fffffep62
  },
  { // Entry 966
    -0x1.c048b38a8bbf59f414fec7079209926ep-3,
    -0x1.fffffep62
  },
  { // Entry 967
    0x1.fff6dfd42dc54430bc0576b00a88bd94p-1,
    0x1.p63
  },
  { // Entry 968
    -0x1.fff6dfd42dc54430bc0576b00a88bd94p-1,
    -0x1.p63
  },
  { // Entry 969
    -0x1.d6637d070347ee94e830445e76486727p-1,
    0x1.000002p63
  },
  { // Entry 970
    0x1.d6637d070347ee94e830445e76486727p-1,
    -0x1.000002p63
  },
  { // Entry 971
    -0x1.0e5283661df0ca0f55ab6167e14514a1p-1,
    0x1.fffffep26
  },
  { // Entry 972
    0x1.0e5283661df0ca0f55ab6167e14514a1p-1,
    -0x1.fffffep26
  },
  { // Entry 973
    -0x1.86dcc9babb0a40ee875cab3b9e892757p-1,
    0x1.p27
  },
  { // Entry 974
    0x1.86dcc9babb0a40ee875cab3b9e892757p-1,
    -0x1.p27
  },
  { // Entry 975
    0x1.171999b629fd5b6357c6dff4d7827d95p-1,
    0x1.000002p27
  },
  { // Entry 976
    -0x1.171999b629fd5b6357c6dff4d7827d95p-1,
    -0x1.000002p27
  },
  { // Entry 977
    -0x1.e57ec09221973550d1e5798dcf0cd25dp-1,
    0x1.fffffep23
  },
  { // Entry 978
    0x1.e57ec09221973550d1e5798dcf0cd25dp-1,
    -0x1.fffffep23
  },
  { // Entry 979
    -0x1.8f22f8433d6edfe9a4aff9622517caa9p-1,
    0x1.p24
  },
  { // Entry 980
    0x1.8f22f8433d6edfe9a4aff9622517caa9p-1,
    -0x1.p24
  },
  { // Entry 981
    0x1.c9b0c7265c543f80faf01741c6458560p-1,
    0x1.000002p24
  },
  { // Entry 982
    -0x1.c9b0c7265c543f80faf01741c6458560p-1,
    -0x1.000002p24
  },
  { // Entry 983
    -0x1.837b98a3185d1466d852f0a7dc1d248ep-1,
    0x1.fffffep1
  },
  { // Entry 984
    0x1.837b98a3185d1466d852f0a7dc1d248ep-1,
    -0x1.fffffep1
  },
  { // Entry 985
    -0x1.837b9dddc1eae70ce98055a0e450d93cp-1,
    0x1.p2
  },
  { // Entry 986
    0x1.837b9dddc1eae70ce98055a0e450d93cp-1,
    -0x1.p2
  },
  { // Entry 987
    -0x1.837ba85314bde52b1e9c2c8ed2712c72p-1,
    0x1.000002p2
  },
  { // Entry 988
    0x1.837ba85314bde52b1e9c2c8ed2712c72p-1,
    -0x1.000002p2
  },
  { // Entry 989
    0x1.d18f70573da63012fa1c0e3d2ebbe59cp-1,
    0x1.fffffep0
  },
  { // Entry 990
    -0x1.d18f70573da63012fa1c0e3d2ebbe59cp-1,
    -0x1.fffffep0
  },
  { // Entry 991
    0x1.d18f6ead1b445dfab848188009c9bb95p-1,
    0x1.p1
  },
  { // Entry 992
    -0x1.d18f6ead1b445dfab848188009c9bb95p-1,
    -0x1.p1
  },
  { // Entry 993
    0x1.d18f6b58d66ae7110b2b6f7cffba6ec1p-1,
    0x1.000002p1
  },
  { // Entry 994
    -0x1.d18f6b58d66ae7110b2b6f7cffba6ec1p-1,
    -0x1.000002p1
  },
  { // Entry 995
    0x1.aed547dbee4d0d8680d0813d1e4e21d0p-1,
    0x1.fffffep-1
  },
  { // Entry 996
    -0x1.aed547dbee4d0d8680d0813d1e4e21d0p-1,
    -0x1.fffffep-1
  },
  { // Entry 997
    0x1.aed548f090cee0418dd3d2138a1e7865p-1,
    0x1.p0
  },
  { // Entry 998
    -0x1.aed548f090cee0418dd3d2138a1e7865p-1,
    -0x1.p0
  },
  { // Entry 999
    0x1.aed54b19d5cd7937cbf41ed408ca0a52p-1,
    0x1.000002p0
  },
  { // Entry 1000
    -0x1.aed54b19d5cd7937cbf41ed408ca0a52p-1,
    -0x1.000002p0
  },
  { // Entry 1001
    0x1.eaee85835dde5b71beec7d8d98052112p-2,
    0x1.fffffep-2
  },
  { // Entry 1002
    -0x1.eaee85835dde5b71beec7d8d98052112p-2,
    -0x1.fffffep-2
  },
  { // Entry 1003
    0x1.eaee8744b05efe8764bc364fd837b666p-2,
    0x1.p-1
  },
  { // Entry 1004
    -0x1.eaee8744b05efe8764bc364fd837b666p-2,
    -0x1.p-1
  },
  { // Entry 1005
    0x1.eaee8ac7555ed47fca77ceed174c8ea0p-2,
    0x1.000002p-1
  },
  { // Entry 1006
    -0x1.eaee8ac7555ed47fca77ceed174c8ea0p-2,
    -0x1.000002p-1
  },
  { // Entry 1007
    0x1.faaeeb5f1c0d63f43c6f3ec46011690fp-3,
    0x1.fffffep-3
  },
  { // Entry 1008
    -0x1.faaeeb5f1c0d63f43c6f3ec46011690fp-3,
    -0x1.fffffep-3
  },
  { // Entry 1009
    0x1.faaeed4f31576ba89debdc7351e8b1aep-3,
    0x1.p-2
  },
  { // Entry 1010
    -0x1.faaeed4f31576ba89debdc7351e8b1aep-3,
    -0x1.p-2
  },
  { // Entry 1011
    0x1.faaef12f5beb1c1094473d3c3365b9e1p-3,
    0x1.000002p-2
  },
  { // Entry 1012
    -0x1.faaef12f5beb1c1094473d3c3365b9e1p-3,
    -0x1.000002p-2
  },
  { // Entry 1013
    0x1.feaaecec6d8e30cd56950eb2ebdcebd4p-4,
    0x1.fffffep-4
  },
  { // Entry 1014
    -0x1.feaaecec6d8e30cd56950eb2ebdcebd4p-4,
    -0x1.fffffep-4
  },
  { // Entry 1015
    0x1.feaaeee86ee35ca069a86721f89f85a5p-4,
    0x1.p-3
  },
  { // Entry 1016
    -0x1.feaaeee86ee35ca069a86721f89f85a5p-4,
    -0x1.p-3
  },
  { // Entry 1017
    0x1.feaaf2e0718d9c568c9442c81545cd62p-4,
    0x1.000002p-3
  },
  { // Entry 1018
    -0x1.feaaf2e0718d9c568c9442c81545cd62p-4,
    -0x1.000002p-3
  },
  { // Entry 1019
    0x1.ffaaacefd4d855ac8227799f3e263d7ap-5,
    0x1.fffffep-5
  },
  { // Entry 1020
    -0x1.ffaaacefd4d855ac8227799f3e263d7ap-5,
    -0x1.fffffep-5
  },
  { // Entry 1021
    0x1.ffaaaeeed4edab4ba4b365ed25a9595fp-5,
    0x1.p-4
  },
  { // Entry 1022
    -0x1.ffaaaeeed4edab4ba4b365ed25a9595fp-5,
    -0x1.p-4
  },
  { // Entry 1023
    0x1.ffaab2ecd518508ae9bc730a165a8eadp-5,
    0x1.000002p-4
  },
  { // Entry 1024
    -0x1.ffaab2ecd518508ae9bc730a165a8eadp-5,
    -0x1.000002p-4
  },
  { // Entry 1025
    0x1.ffeaa8ef2e85933883c0dc33462387b5p-6,
    0x1.fffffep-6
  },
  { // Entry 1026
    -0x1.ffeaa8ef2e85933883c0dc33462387b5p-6,
    -0x1.fffffep-6
  },
  { // Entry 1027
    0x1.ffeaaaeeee86e8cafe41376d47919579p-6,
    0x1.p-5
  },
  { // Entry 1028
    -0x1.ffeaaaeeee86e8cafe41376d47919579p-6,
    -0x1.p-5
  },
  { // Entry 1029
    0x1.ffeaaeee6e89927003413abe64e9dc21p-6,
    0x1.000002p-5
  },
  { // Entry 1030
    -0x1.ffeaaeee6e89927003413abe64e9dc21p-6,
    -0x1.000002p-5
  },
  { // Entry 1031
    0x1.fffaa8aefeed396ffffc636313d0ba6dp-7,
    0x1.fffffep-7
  },
  { // Entry 1032
    -0x1.fffaa8aefeed396ffffc636313d0ba6dp-7,
    -0x1.fffffep-7
  },
  { // Entry 1033
    0x1.fffaaaaeeeed4ed549c6560f889ee531p-7,
    0x1.p-6
  },
  { // Entry 1034
    -0x1.fffaaaaeeeed4ed549c6560f889ee531p-7,
    -0x1.p-6
  },
  { // Entry 1035
    0x1.fffaaeaeceed793fde5a1a9ca5bb1ee6p-7,
    0x1.000002p-6
  },
  { // Entry 1036
    -0x1.fffaaeaeceed793fde5a1a9ca5bb1ee6p-7,
    -0x1.000002p-6
  },
  { // Entry 1037
    0x1.fffffdfaaaaabaaeeeded997feffa35ap-15,
    0x1.fffffep-15
  },
  { // Entry 1038
    -0x1.fffffdfaaaaabaaeeeded997feffa35ap-15,
    -0x1.fffffep-15
  },
  { // Entry 1039
    0x1.fffffffaaaaaaaaeeeeeeeed4ed4ed4fp-15,
    0x1.p-14
  },
  { // Entry 1040
    -0x1.fffffffaaaaaaaaeeeeeeeed4ed4ed4fp-15,
    -0x1.p-14
  },
  { // Entry 1041
    0x1.000001fd5555455777578ccbe7bfc09cp-14,
    0x1.000002p-14
  },
  { // Entry 1042
    -0x1.000001fd5555455777578ccbe7bfc09cp-14,
    -0x1.000002p-14
  },
  { // Entry 1043
    0x1.fffffdfffffffeaaaaaeaaaaa6aaeef0p-28,
    0x1.fffffep-28
  },
  { // Entry 1044
    -0x1.fffffdfffffffeaaaaaeaaaaa6aaeef0p-28,
    -0x1.fffffep-28
  },
  { // Entry 1045
    0x1.fffffffffffffeaaaaaaaaaaaaaaeeeep-28,
    0x1.p-27
  },
  { // Entry 1046
    -0x1.fffffffffffffeaaaaaaaaaaaaaaeeeep-28,
    -0x1.p-27
  },
  { // Entry 1047
    0x1.000001ffffffff55555155554d557772p-27,
    0x1.000002p-27
  },
  { // Entry 1048
    -0x1.000001ffffffff55555155554d557772p-27,
    -0x1.000002p-27
  },
  { // Entry 1049
    0x1.fffffdfffffffffaaaaabaaaaa9aaaaep-31,
    0x1.fffffep-31
  },
  { // Entry 1050
    -0x1.fffffdfffffffffaaaaabaaaaa9aaaaep-31,
    -0x1.fffffep-31
  },
  { // Entry 1051
    0x1.fffffffffffffffaaaaaaaaaaaaaaaaep-31,
    0x1.p-30
  },
  { // Entry 1052
    -0x1.fffffffffffffffaaaaaaaaaaaaaaaaep-31,
    -0x1.p-30
  },
  { // Entry 1053
    0x1.000001fffffffffd5555455555355557p-30,
    0x1.000002p-30
  },
  { // Entry 1054
    -0x1.000001fffffffffd5555455555355557p-30,
    -0x1.000002p-30
  },
  { // Entry 1055
    0x1.0b3366508957520d9dc88d7c09337e24p-1,
    -0x1.fffffep127
  },
  { // Entry 1056
    -0x1.0b3366508957520d9dc88d7c09337e24p-1,
    0x1.fffffep127
  },
  { // Entry 1057
    -0x1.0b3366508957520d9dc88d7c09337e24p-1,
    0x1.fffffep127
  },
  { // Entry 1058
    0x1.0b3366508957520d9dc88d7c09337e24p-1,
    -0x1.fffffep127
  },
  { // Entry 1059
    -0x1.0b3366508957520d9dc88d7c09337e24p-1,
    0x1.fffffep127
  },
  { // Entry 1060
    0x1.0b3366508957520d9dc88d7c09337e24p-1,
    -0x1.fffffep127
  },
  { // Entry 1061
    -0x1.48ce575202efd93c62f7b88106ea1d4dp-1,
    0x1.fffffcp127
  },
  { // Entry 1062
    0x1.48ce575202efd93c62f7b88106ea1d4dp-1,
    -0x1.fffffcp127
  },
  { // Entry 1063
    -0x1.777a5cf72cec5fd61896cb4f40d1de79p-24,
    0x1.921fb6p1
  },
  { // Entry 1064
    0x1.777a5cf72cec5fd61896cb4f40d1de79p-24,
    -0x1.921fb6p1
  },
  { // Entry 1065
    0x1.ffffffffffff76521249c7422930ed82p-1,
    0x1.921fb6p0
  },
  { // Entry 1066
    -0x1.ffffffffffff76521249c7422930ed82p-1,
    -0x1.921fb6p0
  },
  { // Entry 1067
    0x1.aed54b19d5cd7937cbf41ed408ca0a52p-1,
    0x1.000002p0
  },
  { // Entry 1068
    -0x1.aed54b19d5cd7937cbf41ed408ca0a52p-1,
    -0x1.000002p0
  },
  { // Entry 1069
    0x1.aed548f090cee0418dd3d2138a1e7865p-1,
    0x1.p0
  },
  { // Entry 1070
    -0x1.aed548f090cee0418dd3d2138a1e7865p-1,
    -0x1.p0
  },
  { // Entry 1071
    0x1.aed547dbee4d0d8680d0813d1e4e21d0p-1,
    0x1.fffffep-1
  },
  { // Entry 1072
    -0x1.aed547dbee4d0d8680d0813d1e4e21d0p-1,
    -0x1.fffffep-1
  },
  { // Entry 1073
    0x1.6a09e6ecb41fdd7e681872c854887019p-1,
    0x1.921fb6p-1
  },
  { // Entry 1074
    -0x1.6a09e6ecb41fdd7e681872c854887019p-1,
    -0x1.921fb6p-1
  },
  { // Entry 1075
    0x1.000001ffffffffffffffffffffffffffp-126,
    0x1.000002p-126
  },
  { // Entry 1076
    -0x1.000001ffffffffffffffffffffffffffp-126,
    -0x1.000002p-126
  },
  { // Entry 1077
    0x1.ffffffffffffffffffffffffffffffffp-127,
    0x1.p-126
  },
  { // Entry 1078
    -0x1.ffffffffffffffffffffffffffffffffp-127,
    -0x1.p-126
  },
  { // Entry 1079
    0x1.fffffbffffffffffffffffffffffffffp-127,
    0x1.fffffcp-127
  },
  { // Entry 1080
    -0x1.fffffbffffffffffffffffffffffffffp-127,
    -0x1.fffffcp-127
  },
  { // Entry 1081
    0x1.fffff7ffffffffffffffffffffffffffp-127,
    0x1.fffff8p-127
  },
  { // Entry 1082
    -0x1.fffff7ffffffffffffffffffffffffffp-127,
    -0x1.fffff8p-127
  },
  { // Entry 1083
    0x1.ffffffffffffffffffffffffffffffffp-149,
    0x1.p-148
  },
  { // Entry 1084
    -0x1.ffffffffffffffffffffffffffffffffp-149,
    -0x1.p-148
  },
  { // Entry 1085
    0.0f,
    0x1.p-149
  },
  { // Entry 1086
    -0.0f,
    -0x1.p-149
  },
  { // Entry 1087
    0.0,
    0.0f
  },
  { // Entry 1088
    -0.0,
    -0.0f
  },
};
#endif // __BIONIC__

TEST(math_sinf, sinf_intel) {
#if defined(__BIONIC__)
  for (size_t i = 0; i < sizeof(g_sinf_intel_data)/sizeof(sinf_intel_data_t); i++) {
    EXPECT_FLOAT_EQ(g_sinf_intel_data[i].expected, sinf(g_sinf_intel_data[i].call_data)) << "Failed on element " << i;
  }
#else // __BIONIC__
  GTEST_LOG_(INFO) << "This test does nothing.";
#endif // __BIONIC__
}
