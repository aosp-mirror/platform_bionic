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

static data_2_1_t<float, float, float> g_sincosf_intel_data[] = {
  { // Entry 0
    -0x1.b6a7abffaf59a5ac181e3e1abf961698p-1,
    0x1.080e74c116863cfab82a0fd59c71b363p-1,
    -0x1.0768p0,
  },
  { // Entry 1
    0x1.b6a7abffaf59a5ac181e3e1abf961698p-1,
    0x1.080e74c116863cfab82a0fd59c71b363p-1,
    0x1.0768p0,
  },
  { // Entry 2
    0x1.762da6648fde00e398462e7163094d52p-2,
    0x1.dc9802ffffffe0cbfc57ef287d3c83afp-1,
    -0x1.8f219cp5,
  },
  { // Entry 3
    -0x1.762da6648fde00e398462e7163094d52p-2,
    0x1.dc9802ffffffe0cbfc57ef287d3c83afp-1,
    0x1.8f219cp5,
  },
  { // Entry 4
    -0x1.493b6a7d3f5b7bf66fa32a01515d6c43p-1,
    -0x1.881c26fd25d0bf6d6ea38708a0976b35p-1,
    -0x1.d3a51ap80,
  },
  { // Entry 5
    0x1.493b6a7d3f5b7bf66fa32a01515d6c43p-1,
    -0x1.881c26fd25d0bf6d6ea38708a0976b35p-1,
    0x1.d3a51ap80,
  },
  { // Entry 6
    -0x1.e0d1dc006e85835b0394abe97de77fbcp-4,
    0x1.fc75ca6313ef766012db3ca99a1a52d1p-1,
    -0x1.e1ee52p-4,
  },
  { // Entry 7
    0x1.e0d1dc006e85835b0394abe97de77fbcp-4,
    0x1.fc75ca6313ef766012db3ca99a1a52d1p-1,
    0x1.e1ee52p-4,
  },
  { // Entry 8
    -0x1.2b6a9ca3df622efe2aa3154a6352bc70p-1,
    -0x1.9f52def55d89e92e7df20ec21161deb8p-1,
    -0x1.f3ffe2p24,
  },
  { // Entry 9
    0x1.2b6a9ca3df622efe2aa3154a6352bc70p-1,
    -0x1.9f52def55d89e92e7df20ec21161deb8p-1,
    0x1.f3ffe2p24,
  },
  { // Entry 10
    0x1.a3422af13b2cfb78d0cc458d9ac6bde9p-1,
    -0x1.25e1aa1d9383c1f27027cf69b69477fdp-1,
    -0x1.ff211ep24,
  },
  { // Entry 11
    -0x1.a3422af13b2cfb78d0cc458d9ac6bde9p-1,
    -0x1.25e1aa1d9383c1f27027cf69b69477fdp-1,
    0x1.ff211ep24,
  },
  { // Entry 12
    0x1.ffffffaaaaaaaeeeeeeed4ed4ed549c6p-13,
    0x1.ffffff000000155555549f49f4a28a28p-1,
    0x1.p-12,
  },
  { // Entry 13
    -0x1.ffffffaaaaaaaeeeeeeed4ed4ed549c6p-13,
    0x1.ffffff000000155555549f49f4a28a28p-1,
    -0x1.p-12,
  },
  { // Entry 14
    0x1.000001d555545777758cbfcafc09ef7ep-12,
    0x1.fffffefffffc155551ff49f696c4aecap-1,
    0x1.000002p-12,
  },
  { // Entry 15
    -0x1.000001d555545777758cbfcafc09ef7ep-12,
    0x1.fffffefffffc155551ff49f696c4aecap-1,
    -0x1.000002p-12,
  },
  { // Entry 16
    0x1.000001ffffffffffd555545555535555p-32,
    0x1.fffffffffffffffefffffbfffffcp-1,
    0x1.000002p-32,
  },
  { // Entry 17
    -0x1.000001ffffffffffd555545555535555p-32,
    0x1.fffffffffffffffefffffbfffffcp-1,
    -0x1.000002p-32,
  },
  { // Entry 18
    0x1.eaf0d5008f2db899e2487da9d3b9cde7p-2,
    0x1.c151df4f12b197a665a2907e4933a1efp-1,
    0x1.000150p-1,
  },
  { // Entry 19
    -0x1.eaf0d5008f2db899e2487da9d3b9cde7p-2,
    0x1.c151df4f12b197a665a2907e4933a1efp-1,
    -0x1.000150p-1,
  },
  { // Entry 20
    0x1.eaf77efff659021bc827bbd5696ad19ep-2,
    0x1.c1500d494ed2cc5002c27cc5ca3d96f8p-1,
    0x1.00051cp-1,
  },
  { // Entry 21
    -0x1.eaf77efff659021bc827bbd5696ad19ep-2,
    0x1.c1500d494ed2cc5002c27cc5ca3d96f8p-1,
    -0x1.00051cp-1,
  },
  { // Entry 22
    -0x1.5aa53c1c1317f30b4c108722e2b64612p-1,
    0x1.78cdb6ffa6bff899e1723ed0139c48cbp-1,
    0x1.000fdep80,
  },
  { // Entry 23
    0x1.5aa53c1c1317f30b4c108722e2b64612p-1,
    0x1.78cdb6ffa6bff899e1723ed0139c48cbp-1,
    -0x1.000fdep80,
  },
  { // Entry 24
    0x1.aef38822e64311445545980fbc5b56fap-1,
    0x1.14735fffd55120a4480b7c91a1d78d6ep-1,
    0x1.001cp0,
  },
  { // Entry 25
    -0x1.aef38822e64311445545980fbc5b56fap-1,
    0x1.14735fffd55120a4480b7c91a1d78d6ep-1,
    -0x1.001cp0,
  },
  { // Entry 26
    -0x1.7e12aee0c862b1709bd50205bba32167p-1,
    -0x1.54d4290010a68826a882e7c87d6694d2p-1,
    0x1.0027b8p40,
  },
  { // Entry 27
    0x1.7e12aee0c862b1709bd50205bba32167p-1,
    -0x1.54d4290010a68826a882e7c87d6694d2p-1,
    -0x1.0027b8p40,
  },
  { // Entry 28
    -0x1.bd6944ef4dd1b43962e2d3480410fd87p-1,
    0x1.f8fc8efff67d834f6878ab4e44fa89b5p-2,
    0x1.004840p68,
  },
  { // Entry 29
    0x1.bd6944ef4dd1b43962e2d3480410fd87p-1,
    0x1.f8fc8efff67d834f6878ab4e44fa89b5p-2,
    -0x1.004840p68,
  },
  { // Entry 30
    0x1.f4e23e00165a4907a656a7cf532bbde4p-2,
    0x1.be91601e47bed6d45e8372687b373c0cp-1,
    0x1.05b0p-1,
  },
  { // Entry 31
    -0x1.f4e23e00165a4907a656a7cf532bbde4p-2,
    0x1.be91601e47bed6d45e8372687b373c0cp-1,
    -0x1.05b0p-1,
  },
  { // Entry 32
    0x1.b6a7abffaf59a5ac181e3e1abf961698p-1,
    0x1.080e74c116863cfab82a0fd59c71b363p-1,
    0x1.0768p0,
  },
  { // Entry 33
    -0x1.b6a7abffaf59a5ac181e3e1abf961698p-1,
    0x1.080e74c116863cfab82a0fd59c71b363p-1,
    -0x1.0768p0,
  },
  { // Entry 34
    0x1.bcb8aafffffd69d2865aaf3f4c5074e8p-1,
    0x1.fb69a6dc513bcba59d5dc68d3d59a05cp-2,
    0x1.0d67a4p0,
  },
  { // Entry 35
    -0x1.bcb8aafffffd69d2865aaf3f4c5074e8p-1,
    0x1.fb69a6dc513bcba59d5dc68d3d59a05cp-2,
    -0x1.0d67a4p0,
  },
  { // Entry 36
    0x1.bf683cffffec6e8672a49f08aae49c46p-1,
    0x1.f1e0f653d695977ccd240ff6b9297393p-2,
    0x1.1024p0,
  },
  { // Entry 37
    -0x1.bf683cffffec6e8672a49f08aae49c46p-1,
    0x1.f1e0f653d695977ccd240ff6b9297393p-2,
    -0x1.1024p0,
  },
  { // Entry 38
    0x1.8b1e8e1c16a8d970a78a2391eea6575cp-1,
    -0x1.459d4500000092b4b8307c652ee639a7p-1,
    0x1.21497ep1,
  },
  { // Entry 39
    -0x1.8b1e8e1c16a8d970a78a2391eea6575cp-1,
    -0x1.459d4500000092b4b8307c652ee639a7p-1,
    -0x1.21497ep1,
  },
  { // Entry 40
    0x1.6991e0ffe7f9c9b3ef7b3017d23639fap-1,
    -0x1.6a81c4133db73e276c89e724c6fe975bp-1,
    0x1.2dc230p1,
  },
  { // Entry 41
    -0x1.6991e0ffe7f9c9b3ef7b3017d23639fap-1,
    -0x1.6a81c4133db73e276c89e724c6fe975bp-1,
    -0x1.2dc230p1,
  },
  { // Entry 42
    0x1.41f6bd0000d13ba2bd7a63bf6d950865p-1,
    -0x1.8e197fe4fac9901dc07a2878baf15c7ap-1,
    0x1.344248p18,
  },
  { // Entry 43
    -0x1.41f6bd0000d13ba2bd7a63bf6d950865p-1,
    -0x1.8e197fe4fac9901dc07a2878baf15c7ap-1,
    -0x1.344248p18,
  },
  { // Entry 44
    0x1.37f8deffffff61ca15c366b3add8f373p-1,
    -0x1.95fa20d230fdf9e8cccc5921e61f03adp-1,
    0x1.3e42p1,
  },
  { // Entry 45
    -0x1.37f8deffffff61ca15c366b3add8f373p-1,
    -0x1.95fa20d230fdf9e8cccc5921e61f03adp-1,
    -0x1.3e42p1,
  },
  { // Entry 46
    0x1.2ed7ce518f4660cce49556856784970cp-1,
    0x1.9cd4f3000002264af2b32ebe5d8243c0p-1,
    0x1.440c3ap-1,
  },
  { // Entry 47
    -0x1.2ed7ce518f4660cce49556856784970cp-1,
    0x1.9cd4f3000002264af2b32ebe5d8243c0p-1,
    -0x1.440c3ap-1,
  },
  { // Entry 48
    0x1.475fc937750bdf2f704177612d31dc1fp-1,
    0x1.89a999000012c047e4b24181d6158640p-1,
    0x1.632f6ap-1,
  },
  { // Entry 49
    -0x1.475fc937750bdf2f704177612d31dc1fp-1,
    0x1.89a999000012c047e4b24181d6158640p-1,
    -0x1.632f6ap-1,
  },
  { // Entry 50
    -0x1.8ace1cfcc0a766b35d0e8f48f3169e8ap-1,
    -0x1.45fec95a40619819e3307aebe0cb5e89p-1,
    0x1.6e8f18p24,
  },
  { // Entry 51
    0x1.8ace1cfcc0a766b35d0e8f48f3169e8ap-1,
    -0x1.45fec95a40619819e3307aebe0cb5e89p-1,
    -0x1.6e8f18p24,
  },
  { // Entry 52
    0x1.7c6b92ff24ff028ea35c459e5ebeb2cdp-1,
    0x1.56ac2ae54cc9a5210ae673ea1ce31d47p-1,
    0x1.795840p23,
  },
  { // Entry 53
    -0x1.7c6b92ff24ff028ea35c459e5ebeb2cdp-1,
    0x1.56ac2ae54cc9a5210ae673ea1ce31d47p-1,
    -0x1.795840p23,
  },
  { // Entry 54
    0x1.697cf4ffea9244e85cea640d13c6fbb5p-1,
    -0x1.6a96a106ace21610ab529c9047c7e6e2p-1,
    0x1.7fe7fep35,
  },
  { // Entry 55
    -0x1.697cf4ffea9244e85cea640d13c6fbb5p-1,
    -0x1.6a96a106ace21610ab529c9047c7e6e2p-1,
    -0x1.7fe7fep35,
  },
  { // Entry 56
    0x1.9c6cf2fffffe8588512ea3c808b0fa9fp-1,
    0x1.2f6560e0f14cd5d68920bec0e0c25354p-1,
    0x1.ce1026p2,
  },
  { // Entry 57
    -0x1.9c6cf2fffffe8588512ea3c808b0fa9fp-1,
    0x1.2f6560e0f14cd5d68920bec0e0c25354p-1,
    -0x1.ce1026p2,
  },
  { // Entry 58
    -0x1.07fac04c6e73f485096bdaa3e65044d9p-1,
    -0x1.b6b387ffceb67a18c15e868550b96462p-1,
    0x1.d775d0p1,
  },
  { // Entry 59
    0x1.07fac04c6e73f485096bdaa3e65044d9p-1,
    -0x1.b6b387ffceb67a18c15e868550b96462p-1,
    -0x1.d775d0p1,
  },
  { // Entry 60
    0x1.9a8c62f4b2a949b06924662d9436bf17p-1,
    0x1.31ee7074e3ebc5a30f0092ec6c5d8b80p-1,
    0x1.dfeaaap108,
  },
  { // Entry 61
    -0x1.9a8c62f4b2a949b06924662d9436bf17p-1,
    0x1.31ee7074e3ebc5a30f0092ec6c5d8b80p-1,
    -0x1.dfeaaap108,
  },
  { // Entry 62
    0x1.d4de8affffffd96f83ba442fdf2f7101p-2,
    0x1.c72cd4e1d6c78547d31707be80b4b0aep-1,
    0x1.e7061ep-2,
  },
  { // Entry 63
    -0x1.d4de8affffffd96f83ba442fdf2f7101p-2,
    0x1.c72cd4e1d6c78547d31707be80b4b0aep-1,
    -0x1.e7061ep-2,
  },
  { // Entry 64
    -0x1.483445d4194813168a1ac63eee1943f9p-1,
    0x1.88f882fcbc81ffdf720a4bb1b2fca71dp-1,
    0x1.e89b2ap80,
  },
  { // Entry 65
    0x1.483445d4194813168a1ac63eee1943f9p-1,
    0x1.88f882fcbc81ffdf720a4bb1b2fca71dp-1,
    -0x1.e89b2ap80,
  },
  { // Entry 66
    0x1.565e410014916fa6550e606ed588c6b8p-1,
    0x1.7cb1b34af85589bdaf7c0793af30b682p-1,
    0x1.fe8270p18,
  },
  { // Entry 67
    -0x1.565e410014916fa6550e606ed588c6b8p-1,
    0x1.7cb1b34af85589bdaf7c0793af30b682p-1,
    -0x1.fe8270p18,
  },
  { // Entry 68
    -0x1.8356fbaea0454bce3983d9a0e0250c55p-1,
    -0x1.4ed4c5ffc8018741870949767ff8dafap-1,
    0x1.fff1fep1,
  },
  { // Entry 69
    0x1.8356fbaea0454bce3983d9a0e0250c55p-1,
    -0x1.4ed4c5ffc8018741870949767ff8dafap-1,
    -0x1.fff1fep1,
  },
  { // Entry 70
    0x1.fa9d6bfb33a77b2e3b3289168ced6dcap-1,
    -0x1.28406a0025545c9a568dacd45518dd65p-3,
    0x1.fff1fep2,
  },
  { // Entry 71
    -0x1.fa9d6bfb33a77b2e3b3289168ced6dcap-1,
    -0x1.28406a0025545c9a568dacd45518dd65p-3,
    -0x1.fff1fep2,
  },
  { // Entry 72
    0x1.d18f76ffc6e4ba0a3134e5be21b5bc8fp-1,
    -0x1.aa2241160227896c68ef17839f17dce5p-2,
    0x1.fffff6p0,
  },
  { // Entry 73
    -0x1.d18f76ffc6e4ba0a3134e5be21b5bc8fp-1,
    -0x1.aa2241160227896c68ef17839f17dce5p-2,
    -0x1.fffff6p0,
  },
  { // Entry 74
    0x1.fffffaaaaab2eeeedc3dc3f1ee7b41fep-12,
    0x1.fffffc00001155553a7d27f4a289f319p-1,
    0x1.fffffcp-12,
  },
  { // Entry 75
    -0x1.fffffaaaaab2eeeedc3dc3f1ee7b41fep-12,
    0x1.fffffc00001155553a7d27f4a289f319p-1,
    -0x1.fffffcp-12,
  },
  { // Entry 76
    -0.0f,
    0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.p-149,
  },
  { // Entry 77
    0.0f,
    0x1.ffffffffffffffffffffffffffffffffp-1,
    0x1.p-149,
  },
  { // Entry 78
    0.0,
    0x1.p0,
    0.0,
  },
  { // Entry 79
    0.0f,
    0x1.ffffffffffffffffffffffffffffffffp-1,
    0x1.p-149,
  },
  { // Entry 80
    -0.0f,
    0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.p-149,
  },
  { // Entry 81
    -0x1.000001ffffffffffffffffffffffffffp-126,
    0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.000002p-126,
  },
  { // Entry 82
    0x1.000001ffffffffffffffffffffffffffp-126,
    0x1.ffffffffffffffffffffffffffffffffp-1,
    0x1.000002p-126,
  },
  { // Entry 83
    -0x1.ffffffffffffffffffffffffffffffffp-127,
    0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.p-126,
  },
  { // Entry 84
    0x1.ffffffffffffffffffffffffffffffffp-127,
    0x1.ffffffffffffffffffffffffffffffffp-1,
    0x1.p-126,
  },
  { // Entry 85
    -0x1.fffffbffffffffffffffffffffffffffp-127,
    0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.fffffcp-127,
  },
  { // Entry 86
    0x1.fffffbffffffffffffffffffffffffffp-127,
    0x1.ffffffffffffffffffffffffffffffffp-1,
    0x1.fffffcp-127,
  },
  { // Entry 87
    0x1.fffffbffffffffffffffffffffffffffp-127,
    0x1.ffffffffffffffffffffffffffffffffp-1,
    0x1.fffffcp-127,
  },
  { // Entry 88
    -0x1.fffffbffffffffffffffffffffffffffp-127,
    0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.fffffcp-127,
  },
  { // Entry 89
    0x1.ffffffffffffffffffffffffffffffffp-127,
    0x1.ffffffffffffffffffffffffffffffffp-1,
    0x1.p-126,
  },
  { // Entry 90
    -0x1.ffffffffffffffffffffffffffffffffp-127,
    0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.p-126,
  },
  { // Entry 91
    0x1.000001ffffffffffffffffffffffffffp-126,
    0x1.ffffffffffffffffffffffffffffffffp-1,
    0x1.000002p-126,
  },
  { // Entry 92
    -0x1.000001ffffffffffffffffffffffffffp-126,
    0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.000002p-126,
  },
  { // Entry 93
    0x1.999999d44f3058c789014b7d0e22fec3p-13,
    0x1.ffffff5c28f57960cd56ebfe9482a61fp-1,
    0x1.99999ap-13,
  },
  { // Entry 94
    -0x1.999999d44f3058c789014b7d0e22fec3p-13,
    0x1.ffffff5c28f57960cd56ebfe9482a61fp-1,
    -0x1.99999ap-13,
  },
  { // Entry 95
    0x1.999999513cc173e51bb9955b066f1db8p-12,
    0x1.fffffd70a3d64e5ec165cd4307ad1326p-1,
    0x1.99999ap-12,
  },
  { // Entry 96
    -0x1.999999513cc173e51bb9955b066f1db8p-12,
    0x1.fffffd70a3d64e5ec165cd4307ad1326p-1,
    -0x1.99999ap-12,
  },
  { // Entry 97
    0x1.333332d91685321e5fc397d20a5af1e6p-11,
    0x1.fffffa3d709eecbfb5a385b0f3e0cb8fp-1,
    0x1.333334p-11,
  },
  { // Entry 98
    -0x1.333332d91685321e5fc397d20a5af1e6p-11,
    0x1.fffffa3d709eecbfb5a385b0f3e0cb8fp-1,
    -0x1.333334p-11,
  },
  { // Entry 99
    0x1.99999744f306dc03e9ef70897f78299ep-11,
    0x1.fffff5c28f5fc733c4202aa0bda92f97p-1,
    0x1.99999ap-11,
  },
  { // Entry 100
    -0x1.99999744f306dc03e9ef70897f78299ep-11,
    0x1.fffff5c28f5fc733c4202aa0bda92f97p-1,
    -0x1.99999ap-11,
  },
  { // Entry 101
    0x1.fffffaaaaaaeeeeeed4ed4edab4c7bd6p-11,
    0x1.fffff0000015555549f49f4d34d34ca0p-1,
    0x1.p-10,
  },
  { // Entry 102
    -0x1.fffffaaaaaaeeeeeed4ed4edab4c7bd6p-11,
    0x1.fffff0000015555549f49f4d34d34ca0p-1,
    -0x1.p-10,
  },
  { // Entry 103
    0x1.33332f645a18c3b0ccfc0a3cf7b2e91bp-10,
    0x1.ffffe8f5c29ce07640bd5f6d8bb1ea6bp-1,
    0x1.333334p-10,
  },
  { // Entry 104
    -0x1.33332f645a18c3b0ccfc0a3cf7b2e91bp-10,
    0x1.ffffe8f5c29ce07640bd5f6d8bb1ea6bp-1,
    -0x1.333334p-10,
  },
  { // Entry 105
    0x1.666660aec330821c7a100cf488c380ebp-10,
    0x1.ffffe0a3d714839f3601147ada73f8d9p-1,
    0x1.666668p-10,
  },
  { // Entry 106
    -0x1.666660aec330821c7a100cf488c380ebp-10,
    0x1.ffffe0a3d714839f3601147ada73f8d9p-1,
    -0x1.666668p-10,
  },
  { // Entry 107
    0x1.99999113cc034144fdbdc8a1dc713253p-10,
    0x1.ffffd70a3d8191f66de5408fb2b995a0p-1,
    0x1.99999cp-10,
  },
  { // Entry 108
    -0x1.99999113cc034144fdbdc8a1dc713253p-10,
    0x1.ffffd70a3d8191f66de5408fb2b995a0p-1,
    -0x1.99999cp-10,
  },
  { // Entry 109
    0x1.ccccbc72b05dd0951a9c5e65560c56e3p-10,
    0x1.ffffcc28f6d096b87d6d19a06e96999bp-1,
    0x1.ccccccp-10,
  },
  { // Entry 110
    -0x1.ccccbc72b05dd0951a9c5e65560c56e3p-10,
    0x1.ffffcc28f6d096b87d6d19a06e96999bp-1,
    -0x1.ccccccp-10,
  },
  { // Entry 111
    0x1.0665ae3615b5b7de52798064dfc59b29p-7,
    0x1.fffbcc2a71ceaabf5582b6da1fc30531p-1,
    0x1.066666p-7,
  },
  { // Entry 112
    -0x1.0665ae3615b5b7de52798064dfc59b29p-7,
    0x1.fffbcc2a71ceaabf5582b6da1fc30531p-1,
    -0x1.066666p-7,
  },
  { // Entry 113
    0x1.ccc8e8ae92586d8d66b6ad0aedcb0d94p-7,
    0x1.fff30a4b7b5119d8e299f717ec0ece16p-1,
    0x1.ccccccp-7,
  },
  { // Entry 114
    -0x1.ccc8e8ae92586d8d66b6ad0aedcb0d94p-7,
    0x1.fff30a4b7b5119d8e299f717ec0ece16p-1,
    -0x1.ccccccp-7,
  },
  { // Entry 115
    0x1.4993e70f7b17c10af9f97fc7b33b822bp-6,
    0x1.ffe57a785123226c1e2efb411353edc9p-1,
    0x1.499998p-6,
  },
  { // Entry 116
    -0x1.4993e70f7b17c10af9f97fc7b33b822bp-6,
    0x1.ffe57a785123226c1e2efb411353edc9p-1,
    -0x1.499998p-6,
  },
  { // Entry 117
    0x1.acc043f8b2d89ad5143c030e9766fc11p-6,
    0x1.ffd31cd10cb632d9733ac5f5f327a5f9p-1,
    0x1.acccccp-6,
  },
  { // Entry 118
    -0x1.acc043f8b2d89ad5143c030e9766fc11p-6,
    0x1.ffd31cd10cb632d9733ac5f5f327a5f9p-1,
    -0x1.acccccp-6,
  },
  { // Entry 119
    0x1.07f44d67cf41afbc0c95108b99f91b01p-5,
    0x1.ffbbf18207542ef81390d73c3ba89c1ap-1,
    0x1.08p-5,
  },
  { // Entry 120
    -0x1.07f44d67cf41afbc0c95108b99f91b01p-5,
    0x1.ffbbf18207542ef81390d73c3ba89c1ap-1,
    -0x1.08p-5,
  },
  { // Entry 121
    0x1.3985fead44fa2b851e651acba369d769p-5,
    0x1.ff9ff8c2eaeaee30fb211765af835171p-1,
    0x1.39999ap-5,
  },
  { // Entry 122
    -0x1.3985fead44fa2b851e651acba369d769p-5,
    0x1.ff9ff8c2eaeaee30fb211765af835171p-1,
    -0x1.39999ap-5,
  },
  { // Entry 123
    0x1.6b14beb5d40d745096247e59b622828bp-5,
    0x1.ff7f32d6eb1f9cf90226ec291c9e0922p-1,
    0x1.6b3334p-5,
  },
  { // Entry 124
    -0x1.6b14beb5d40d745096247e59b622828bp-5,
    0x1.ff7f32d6eb1f9cf90226ec291c9e0922p-1,
    -0x1.6b3334p-5,
  },
  { // Entry 125
    0x1.9ca01671a7995fc97cebd69729cc4309p-5,
    0x1.ff59a00cc4ad492ca2597495412998edp-1,
    0x1.9ccccep-5,
  },
  { // Entry 126
    -0x1.9ca01671a7995fc97cebd69729cc4309p-5,
    0x1.ff59a00cc4ad492ca2597495412998edp-1,
    -0x1.9ccccep-5,
  },
  { // Entry 127
    0x1.ce278cd9eb2cbd0c6a4e1279f690e856p-5,
    0x1.ff2f40c08acf4580a8d13380d9073398p-1,
    0x1.ce6666p-5,
  },
  { // Entry 128
    -0x1.ce278cd9eb2cbd0c6a4e1279f690e856p-5,
    0x1.ff2f40c08acf4580a8d13380d9073398p-1,
    -0x1.ce6666p-5,
  },
  { // Entry 129
    0x1.43c1e9972391aa8ecd8a9ccba907920ap-1,
    0x1.8ca46ca011771bfae0d9edbf1dacb402p-1,
    0x1.5e7fc4p-1,
  },
  { // Entry 130
    -0x1.43c1e9972391aa8ecd8a9ccba907920ap-1,
    0x1.8ca46ca011771bfae0d9edbf1dacb402p-1,
    -0x1.5e7fc4p-1,
  },
  { // Entry 131
    0x1.ee3d6bb21c64b2382efcff0cdf30ce0bp-1,
    0x1.0b5d38d5d82e4a7624dac4e10ce159c2p-2,
    0x1.4e7fc4p0,
  },
  { // Entry 132
    -0x1.ee3d6bb21c64b2382efcff0cdf30ce0bp-1,
    0x1.0b5d38d5d82e4a7624dac4e10ce159c2p-2,
    -0x1.4e7fc4p0,
  },
  { // Entry 133
    0x1.df8e2323e4bf1a538a100ec1bf3494a9p-1,
    -0x1.66b96e204e69cda8e9cf50996432539ep-2,
    0x1.edbfa6p0,
  },
  { // Entry 134
    -0x1.df8e2323e4bf1a538a100ec1bf3494a9p-1,
    -0x1.66b96e204e69cda8e9cf50996432539ep-2,
    -0x1.edbfa6p0,
  },
  { // Entry 135
    0x1.1d347aa02feb3bb1750d25509435da88p-1,
    -0x1.a935540edeca4c220ed91dc5481e4d9bp-1,
    0x1.467fc4p1,
  },
  { // Entry 136
    -0x1.1d347aa02feb3bb1750d25509435da88p-1,
    -0x1.a935540edeca4c220ed91dc5481e4d9bp-1,
    -0x1.467fc4p1,
  },
  { // Entry 137
    -0x1.ffea08e1c97f4c4de01961cc3e7b04dcp-6,
    -0x1.ffc0017dd9209dd8891f17fe4c9eee46p-1,
    0x1.961fb4p1,
  },
  { // Entry 138
    0x1.ffea08e1c97f4c4de01961cc3e7b04dcp-6,
    -0x1.ffc0017dd9209dd8891f17fe4c9eee46p-1,
    -0x1.961fb4p1,
  },
  { // Entry 139
    -0x1.3734cbced9c0f484e5f762e00216e620p-1,
    -0x1.969082007733d787f4c36ba0f9425694p-1,
    0x1.e5bfa4p1,
  },
  { // Entry 140
    0x1.3734cbced9c0f484e5f762e00216e620p-1,
    -0x1.969082007733d787f4c36ba0f9425694p-1,
    -0x1.e5bfa4p1,
  },
  { // Entry 141
    -0x1.e9d2592bec10c3acb15c5852239a2aa8p-1,
    -0x1.2a1e74223d9bddb7db59f781f96b65ecp-2,
    0x1.1aafcap2,
  },
  { // Entry 142
    0x1.e9d2592bec10c3acb15c5852239a2aa8p-1,
    -0x1.2a1e74223d9bddb7db59f781f96b65ecp-2,
    -0x1.1aafcap2,
  },
  { // Entry 143
    -0x1.e4ece208d0c4913ab019cab0ce9c785ep-1,
    0x1.4894d50b84dbc981134a591ac4165d9ep-2,
    0x1.427fc2p2,
  },
  { // Entry 144
    0x1.e4ece208d0c4913ab019cab0ce9c785ep-1,
    0x1.4894d50b84dbc981134a591ac4165d9ep-2,
    -0x1.427fc2p2,
  },
  { // Entry 145
    -0x1.2a5a02d392b54f641a0d88bd4ac6c2e1p-1,
    0x1.a016dd7480a1eea4d49efb585fa49c86p-1,
    0x1.6a4fbap2,
  },
  { // Entry 146
    0x1.2a5a02d392b54f641a0d88bd4ac6c2e1p-1,
    0x1.a016dd7480a1eea4d49efb585fa49c86p-1,
    -0x1.6a4fbap2,
  },
  { // Entry 147
    -0x1.263123df22d13ed329c665c83c0e71e8p-1,
    0x1.a30a6a3bf4a3e2b7e27666d3a9c3b74bp-1,
    0x1.6af2f0p2,
  },
  { // Entry 148
    0x1.263123df22d13ed329c665c83c0e71e8p-1,
    0x1.a30a6a3bf4a3e2b7e27666d3a9c3b74bp-1,
    -0x1.6af2f0p2,
  },
  { // Entry 149
    -0x1.e18e67b508ffc9e42f6c9e72f8e545f3p-1,
    0x1.5bd625504015ccc101e4f4340d4b762bp-2,
    0x1.43c62ap2,
  },
  { // Entry 150
    0x1.e18e67b508ffc9e42f6c9e72f8e545f3p-1,
    0x1.5bd625504015ccc101e4f4340d4b762bp-2,
    -0x1.43c62ap2,
  },
  { // Entry 151
    -0x1.ee0e80ec9d1562c17a6cf608af9b0ed4p-1,
    -0x1.0cb733448c30ee3ddffb4da69f0b3842p-2,
    0x1.1c9964p2,
  },
  { // Entry 152
    0x1.ee0e80ec9d1562c17a6cf608af9b0ed4p-1,
    -0x1.0cb733448c30ee3ddffb4da69f0b3842p-2,
    -0x1.1c9964p2,
  },
  { // Entry 153
    -0x1.472768637ea8866a652098a43aa688a6p-1,
    -0x1.89d874ad30e3fb46244daa24451690d7p-1,
    0x1.ead93cp1,
  },
  { // Entry 154
    0x1.472768637ea8866a652098a43aa688a6p-1,
    -0x1.89d874ad30e3fb46244daa24451690d7p-1,
    -0x1.ead93cp1,
  },
  { // Entry 155
    -0x1.4ba24f6325f21420e7c48d4f91e28064p-4,
    -0x1.fe51ae09f0d39ed554e68bef3e2f8a03p-1,
    0x1.9c7fb0p1,
  },
  { // Entry 156
    0x1.4ba24f6325f21420e7c48d4f91e28064p-4,
    -0x1.fe51ae09f0d39ed554e68bef3e2f8a03p-1,
    -0x1.9c7fb0p1,
  },
  { // Entry 157
    0x1.034c643295153aaffe5d9a0f29e92844p-1,
    -0x1.b97bf76ae765eb69cf55e80aae977303p-1,
    0x1.4e2624p1,
  },
  { // Entry 158
    -0x1.034c643295153aaffe5d9a0f29e92844p-1,
    -0x1.b97bf76ae765eb69cf55e80aae977303p-1,
    -0x1.4e2624p1,
  },
  { // Entry 159
    0x1.d1e4d96eac917574b948e4ea9e37b36dp-1,
    -0x1.a8ac5793e32629b131984ecffd2d0f31p-2,
    0x1.ff9932p0,
  },
  { // Entry 160
    -0x1.d1e4d96eac917574b948e4ea9e37b36dp-1,
    -0x1.a8ac5793e32629b131984ecffd2d0f31p-2,
    -0x1.ff9932p0,
  },
  { // Entry 161
    0x1.f7501e002bcafb897f931931d3a57afdp-1,
    0x1.77a92ca01bc79c195dda33736807f986p-3,
    0x1.62e61cp0,
  },
  { // Entry 162
    -0x1.f7501e002bcafb897f931931d3a57afdp-1,
    0x1.77a92ca01bc79c195dda33736807f986p-3,
    -0x1.62e61cp0,
  },
  { // Entry 163
    0x1.65f7d66ef6591cfc51ab29dc2086d3e7p-1,
    0x1.6e1060282c1488d9abd83da1d68cd0f7p-1,
    0x1.8c662cp-1,
  },
  { // Entry 164
    -0x1.65f7d66ef6591cfc51ab29dc2086d3e7p-1,
    0x1.6e1060282c1488d9abd83da1d68cd0f7p-1,
    -0x1.8c662cp-1,
  },
  { // Entry 165
    -0x1.fe043f875c6ed4a2c1b8d69a09fcf578p-1,
    -0x1.682f2bb87a8f5011735094176c9b6dacp-4,
    -0x1.a8aa1cp0,
  },
  { // Entry 166
    0x1.fe043f875c6ed4a2c1b8d69a09fcf578p-1,
    -0x1.682f2bb87a8f5011735094176c9b6dacp-4,
    0x1.a8aa1cp0,
  },
  { // Entry 167
    -0x1.fff18f313e66f1ae25f89a5f7a1f84c4p-1,
    -0x1.e665cb2af842be5ba5f65960599a97ecp-7,
    -0x1.95ec8ap0,
  },
  { // Entry 168
    0x1.fff18f313e66f1ae25f89a5f7a1f84c4p-1,
    -0x1.e665cb2af842be5ba5f65960599a97ecp-7,
    0x1.95ec8ap0,
  },
  { // Entry 169
    -0x1.ff20d920b9e9c23154f97e2f342a2884p-1,
    0x1.ddd231501b12fcf2bc20633be4d51e51p-5,
    -0x1.832ef8p0,
  },
  { // Entry 170
    0x1.ff20d920b9e9c23154f97e2f342a2884p-1,
    0x1.ddd231501b12fcf2bc20633be4d51e51p-5,
    0x1.832ef8p0,
  },
  { // Entry 171
    -0x1.fb933b89d7db3286eed0cfeabee98875p-1,
    0x1.0caba6997691ab1970d43c7419ed51e1p-3,
    -0x1.707166p0,
  },
  { // Entry 172
    0x1.fb933b89d7db3286eed0cfeabee98875p-1,
    0x1.0caba6997691ab1970d43c7419ed51e1p-3,
    0x1.707166p0,
  },
  { // Entry 173
    -0x1.f54d95c5058b7dc9972ab6f9928ca043p-1,
    0x1.a072541fd6eaf8b65a874c58d6cc5739p-3,
    -0x1.5db3d4p0,
  },
  { // Entry 174
    0x1.f54d95c5058b7dc9972ab6f9928ca043p-1,
    0x1.a072541fd6eaf8b65a874c58d6cc5739p-3,
    0x1.5db3d4p0,
  },
  { // Entry 175
    -0x1.ec5881a09c46aae020cd9036098a196ep-1,
    0x1.18fef8106bea63b9e96a7adf538c6194p-2,
    -0x1.4af642p0,
  },
  { // Entry 176
    0x1.ec5881a09c46aae020cd9036098a196ep-1,
    0x1.18fef8106bea63b9e96a7adf538c6194p-2,
    0x1.4af642p0,
  },
  { // Entry 177
    -0x1.e0c04795919d961a5e83e505df31c624p-1,
    0x1.60437277d48067e85230bce1883eaabap-2,
    -0x1.3838b0p0,
  },
  { // Entry 178
    0x1.e0c04795919d961a5e83e505df31c624p-1,
    0x1.60437277d48067e85230bce1883eaabap-2,
    0x1.3838b0p0,
  },
  { // Entry 179
    -0x1.d294cdef7cc161633a02d3e62058be0ep-1,
    0x1.a5a4ded492bedfe8cf5c34cadd78df75p-2,
    -0x1.257b1ep0,
  },
  { // Entry 180
    0x1.d294cdef7cc161633a02d3e62058be0ep-1,
    0x1.a5a4ded492bedfe8cf5c34cadd78df75p-2,
    0x1.257b1ep0,
  },
  { // Entry 181
    -0x1.c1e988b95614abd65d3d811f5c88039bp-1,
    0x1.e8c4040678d2ef736333a4537a1113a1p-2,
    -0x1.12bd92p0,
  },
  { // Entry 182
    0x1.c1e988b95614abd65d3d811f5c88039bp-1,
    0x1.e8c4040678d2ef736333a4537a1113a1p-2,
    0x1.12bd92p0,
  },
  { // Entry 183
    -0x1.a2c2895edb0d4ba51cdbd5390cac468fp-1,
    0x1.26976b1b16d19091c09259765c4b3872p-1,
    -0x1.ea5c3ep-1,
  },
  { // Entry 184
    0x1.a2c2895edb0d4ba51cdbd5390cac468fp-1,
    0x1.26976b1b16d19091c09259765c4b3872p-1,
    0x1.ea5c3ep-1,
  },
  { // Entry 185
    -0x1.95f05153644d60b94d2f2e700dfd3a37p-1,
    0x1.3805a2dafda7f8554aec65dab348a714p-1,
    -0x1.d4b87cp-1,
  },
  { // Entry 186
    0x1.95f05153644d60b94d2f2e700dfd3a37p-1,
    0x1.3805a2dafda7f8554aec65dab348a714p-1,
    0x1.d4b87cp-1,
  },
  { // Entry 187
    -0x1.88647d8ad2e41fb7c0af0f64614c9993p-1,
    0x1.48e52ff5bbe794618b85190b86411824p-1,
    -0x1.bf14bap-1,
  },
  { // Entry 188
    0x1.88647d8ad2e41fb7c0af0f64614c9993p-1,
    0x1.48e52ff5bbe794618b85190b86411824p-1,
    0x1.bf14bap-1,
  },
  { // Entry 189
    -0x1.7a253f9f89a7d3e4f9c54638418e97f6p-1,
    0x1.592e5b615ef5ae463976d31141dbacf0p-1,
    -0x1.a970f8p-1,
  },
  { // Entry 190
    0x1.7a253f9f89a7d3e4f9c54638418e97f6p-1,
    0x1.592e5b615ef5ae463976d31141dbacf0p-1,
    0x1.a970f8p-1,
  },
  { // Entry 191
    -0x1.6b391b34aab828fbe7cd7dcaf9ef3bd6p-1,
    0x1.68d9b2d657e4307d331eb7bd35ee1879p-1,
    -0x1.93cd36p-1,
  },
  { // Entry 192
    0x1.6b391b34aab828fbe7cd7dcaf9ef3bd6p-1,
    0x1.68d9b2d657e4307d331eb7bd35ee1879p-1,
    0x1.93cd36p-1,
  },
  { // Entry 193
    -0x1.5ba6e2fb980d482cf00ede80f5597fb2p-1,
    0x1.77e00c3718528c36f722e63096c2646bp-1,
    -0x1.7e2974p-1,
  },
  { // Entry 194
    0x1.5ba6e2fb980d482cf00ede80f5597fb2p-1,
    0x1.77e00c3718528c36f722e63096c2646bp-1,
    0x1.7e2974p-1,
  },
  { // Entry 195
    -0x1.4b75b5954e718020900719e25a7ea93bp-1,
    0x1.863a88d6b064f36f34370722d361ce9fp-1,
    -0x1.6885b2p-1,
  },
  { // Entry 196
    0x1.4b75b5954e718020900719e25a7ea93bp-1,
    0x1.863a88d6b064f36f34370722d361ce9fp-1,
    0x1.6885b2p-1,
  },
  { // Entry 197
    -0x1.3aacfa510810054c52ae0b67d116eb40p-1,
    0x1.93e2989cee6084e34b533b1eb92746dap-1,
    -0x1.52e1f0p-1,
  },
  { // Entry 198
    0x1.3aacfa510810054c52ae0b67d116eb40p-1,
    0x1.93e2989cee6084e34b533b1eb92746dap-1,
    0x1.52e1f0p-1,
  },
  { // Entry 199
    -0x1.2954644ceb8e3a2479c83ae84af57d3ep-1,
    0x1.a0d1f8613ebc60c7ee6502ee183c89e7p-1,
    -0x1.3d3e36p-1,
  },
  { // Entry 200
    0x1.2954644ceb8e3a2479c83ae84af57d3ep-1,
    0x1.a0d1f8613ebc60c7ee6502ee183c89e7p-1,
    0x1.3d3e36p-1,
  },
  { // Entry 201
    -0x1.fc769aecd265cfea08e0ff30c2fbcacdp-2,
    0x1.bc6bd889a8a59dbb56e546a37ae798f5p-1,
    -0x1.0a0b02p-1,
  },
  { // Entry 202
    0x1.fc769aecd265cfea08e0ff30c2fbcacdp-2,
    0x1.bc6bd889a8a59dbb56e546a37ae798f5p-1,
    0x1.0a0b02p-1,
  },
  { // Entry 203
    -0x1.c853c704e3b94322031d6b47aef853c9p-2,
    0x1.ca59c719f96075dd6b7a0ff443ad59dcp-1,
    -0x1.d8f720p-2,
  },
  { // Entry 204
    0x1.c853c704e3b94322031d6b47aef853c9p-2,
    0x1.ca59c719f96075dd6b7a0ff443ad59dcp-1,
    0x1.d8f720p-2,
  },
  { // Entry 205
    -0x1.92aba8981b25deda4cc1817251723a1bp-2,
    0x1.d6c0b13df99613a49306b4dc6c57aa03p-1,
    -0x1.9dd83cp-2,
  },
  { // Entry 206
    0x1.92aba8981b25deda4cc1817251723a1bp-2,
    0x1.d6c0b13df99613a49306b4dc6c57aa03p-1,
    0x1.9dd83cp-2,
  },
  { // Entry 207
    -0x1.5bac05e1e0a7c2de280fcb93be67a4dap-2,
    0x1.e1960273a4aaa1bd19ef0ccaa8874183p-1,
    -0x1.62b958p-2,
  },
  { // Entry 208
    0x1.5bac05e1e0a7c2de280fcb93be67a4dap-2,
    0x1.e1960273a4aaa1bd19ef0ccaa8874183p-1,
    0x1.62b958p-2,
  },
  { // Entry 209
    -0x1.2383ca2b249807d95005d96cfdaecd6cp-2,
    0x1.ead07cd2e0f7f19679646362bbc0eb80p-1,
    -0x1.279a74p-2,
  },
  { // Entry 210
    0x1.2383ca2b249807d95005d96cfdaecd6cp-2,
    0x1.ead07cd2e0f7f19679646362bbc0eb80p-1,
    0x1.279a74p-2,
  },
  { // Entry 211
    -0x1.d4c5bb872ea5375834ca0bca088d1d75p-3,
    0x1.f26840efd86ceea23f388b6a1102ea4dp-1,
    -0x1.d8f720p-3,
  },
  { // Entry 212
    0x1.d4c5bb872ea5375834ca0bca088d1d75p-3,
    0x1.f26840efd86ceea23f388b6a1102ea4dp-1,
    0x1.d8f720p-3,
  },
  { // Entry 213
    -0x1.60f3fa460b85811d2ae710cd69ec3690p-3,
    0x1.f856d49251bd37c54f0094270eecbd18p-1,
    -0x1.62b958p-3,
  },
  { // Entry 214
    0x1.60f3fa460b85811d2ae710cd69ec3690p-3,
    0x1.f856d49251bd37c54f0094270eecbd18p-1,
    0x1.62b958p-3,
  },
  { // Entry 215
    -0x1.d7ea3d56e1e6244c8786d74f189d98acp-4,
    0x1.fc97283c4f5bd1f793201972b8db551fp-1,
    -0x1.d8f720p-4,
  },
  { // Entry 216
    0x1.d7ea3d56e1e6244c8786d74f189d98acp-4,
    0x1.fc97283c4f5bd1f793201972b8db551fp-1,
    0x1.d8f720p-4,
  },
  { // Entry 217
    -0x1.d8b3deba6ac493b04b2103a0dbaef02fp-5,
    0x1.ff259b7b3d721edf063a5bf6e7a1f93cp-1,
    -0x1.d8f720p-5,
  },
  { // Entry 218
    0x1.d8b3deba6ac493b04b2103a0dbaef02fp-5,
    0x1.ff259b7b3d721edf063a5bf6e7a1f93cp-1,
    0x1.d8f720p-5,
  },
  { // Entry 219
    0x1.d8b3deba6ac493b04b2103a0dbaef02fp-5,
    0x1.ff259b7b3d721edf063a5bf6e7a1f93cp-1,
    0x1.d8f720p-5,
  },
  { // Entry 220
    -0x1.d8b3deba6ac493b04b2103a0dbaef02fp-5,
    0x1.ff259b7b3d721edf063a5bf6e7a1f93cp-1,
    -0x1.d8f720p-5,
  },
  { // Entry 221
    0x1.d7ea3d56e1e6244c8786d74f189d98acp-4,
    0x1.fc97283c4f5bd1f793201972b8db551fp-1,
    0x1.d8f720p-4,
  },
  { // Entry 222
    -0x1.d7ea3d56e1e6244c8786d74f189d98acp-4,
    0x1.fc97283c4f5bd1f793201972b8db551fp-1,
    -0x1.d8f720p-4,
  },
  { // Entry 223
    0x1.60f3fa460b85811d2ae710cd69ec3690p-3,
    0x1.f856d49251bd37c54f0094270eecbd18p-1,
    0x1.62b958p-3,
  },
  { // Entry 224
    -0x1.60f3fa460b85811d2ae710cd69ec3690p-3,
    0x1.f856d49251bd37c54f0094270eecbd18p-1,
    -0x1.62b958p-3,
  },
  { // Entry 225
    0x1.d4c5bb872ea5375834ca0bca088d1d75p-3,
    0x1.f26840efd86ceea23f388b6a1102ea4dp-1,
    0x1.d8f720p-3,
  },
  { // Entry 226
    -0x1.d4c5bb872ea5375834ca0bca088d1d75p-3,
    0x1.f26840efd86ceea23f388b6a1102ea4dp-1,
    -0x1.d8f720p-3,
  },
  { // Entry 227
    0x1.2383ca2b249807d95005d96cfdaecd6cp-2,
    0x1.ead07cd2e0f7f19679646362bbc0eb80p-1,
    0x1.279a74p-2,
  },
  { // Entry 228
    -0x1.2383ca2b249807d95005d96cfdaecd6cp-2,
    0x1.ead07cd2e0f7f19679646362bbc0eb80p-1,
    -0x1.279a74p-2,
  },
  { // Entry 229
    0x1.5bac05e1e0a7c2de280fcb93be67a4dap-2,
    0x1.e1960273a4aaa1bd19ef0ccaa8874183p-1,
    0x1.62b958p-2,
  },
  { // Entry 230
    -0x1.5bac05e1e0a7c2de280fcb93be67a4dap-2,
    0x1.e1960273a4aaa1bd19ef0ccaa8874183p-1,
    -0x1.62b958p-2,
  },
  { // Entry 231
    0x1.92aba8981b25deda4cc1817251723a1bp-2,
    0x1.d6c0b13df99613a49306b4dc6c57aa03p-1,
    0x1.9dd83cp-2,
  },
  { // Entry 232
    -0x1.92aba8981b25deda4cc1817251723a1bp-2,
    0x1.d6c0b13df99613a49306b4dc6c57aa03p-1,
    -0x1.9dd83cp-2,
  },
  { // Entry 233
    0x1.c853c704e3b94322031d6b47aef853c9p-2,
    0x1.ca59c719f96075dd6b7a0ff443ad59dcp-1,
    0x1.d8f720p-2,
  },
  { // Entry 234
    -0x1.c853c704e3b94322031d6b47aef853c9p-2,
    0x1.ca59c719f96075dd6b7a0ff443ad59dcp-1,
    -0x1.d8f720p-2,
  },
  { // Entry 235
    0x1.fc769aecd265cfea08e0ff30c2fbcacdp-2,
    0x1.bc6bd889a8a59dbb56e546a37ae798f5p-1,
    0x1.0a0b02p-1,
  },
  { // Entry 236
    -0x1.fc769aecd265cfea08e0ff30c2fbcacdp-2,
    0x1.bc6bd889a8a59dbb56e546a37ae798f5p-1,
    -0x1.0a0b02p-1,
  },
  { // Entry 237
    0x1.2954644ceb8e3a2479c83ae84af57d3ep-1,
    0x1.a0d1f8613ebc60c7ee6502ee183c89e7p-1,
    0x1.3d3e36p-1,
  },
  { // Entry 238
    -0x1.2954644ceb8e3a2479c83ae84af57d3ep-1,
    0x1.a0d1f8613ebc60c7ee6502ee183c89e7p-1,
    -0x1.3d3e36p-1,
  },
  { // Entry 239
    0x1.3aad00a09268a39df1653c70db91f157p-1,
    0x1.93e293b23a6aa1ae5373214eb8fb9e96p-1,
    0x1.52e1f8p-1,
  },
  { // Entry 240
    -0x1.3aad00a09268a39df1653c70db91f157p-1,
    0x1.93e293b23a6aa1ae5373214eb8fb9e96p-1,
    -0x1.52e1f8p-1,
  },
  { // Entry 241
    0x1.4b75bbae388a7f3466e7f2d6bdcf72bbp-1,
    0x1.863a83a8d9826c6135509406ebe05c0fp-1,
    0x1.6885bap-1,
  },
  { // Entry 242
    -0x1.4b75bbae388a7f3466e7f2d6bdcf72bbp-1,
    0x1.863a83a8d9826c6135509406ebe05c0fp-1,
    -0x1.6885bap-1,
  },
  { // Entry 243
    0x1.5ba6e8db1833475712b9a42a1ad0d2c2p-1,
    0x1.77e006c87cbaded66ec5b960c93c568ap-1,
    0x1.7e297cp-1,
  },
  { // Entry 244
    -0x1.5ba6e8db1833475712b9a42a1ad0d2c2p-1,
    0x1.77e006c87cbaded66ec5b960c93c568ap-1,
    -0x1.7e297cp-1,
  },
  { // Entry 245
    0x1.6b3920d8117828928fe10ac70ba69e76p-1,
    0x1.68d9ad29736c1704caea6a2db6e71223p-1,
    0x1.93cd3ep-1,
  },
  { // Entry 246
    -0x1.6b3920d8117828928fe10ac70ba69e76p-1,
    0x1.68d9ad29736c1704caea6a2db6e71223p-1,
    -0x1.93cd3ep-1,
  },
  { // Entry 247
    0x1.7a25450443098836c5202375db4b8462p-1,
    0x1.592e5578c9ec66acceddd4dc6ce66b26p-1,
    0x1.a971p-1,
  },
  { // Entry 248
    -0x1.7a25450443098836c5202375db4b8462p-1,
    0x1.592e5578c9ec66acceddd4dc6ce66b26p-1,
    -0x1.a971p-1,
  },
  { // Entry 249
    0x1.886482ae6797b38364f5c72ce9a3b76fp-1,
    0x1.48e529d429e721ec8bb1e014f94d48f1p-1,
    0x1.bf14c2p-1,
  },
  { // Entry 250
    -0x1.886482ae6797b38364f5c72ce9a3b76fp-1,
    0x1.48e529d429e721ec8bb1e014f94d48f1p-1,
    -0x1.bf14c2p-1,
  },
  { // Entry 251
    0x1.95f056337acc1d2d557525232e915467p-1,
    0x1.38059c833c58ea970f7b96d6ada3d9c4p-1,
    0x1.d4b884p-1,
  },
  { // Entry 252
    -0x1.95f056337acc1d2d557525232e915467p-1,
    0x1.38059c833c58ea970f7b96d6ada3d9c4p-1,
    -0x1.d4b884p-1,
  },
  { // Entry 253
    0x1.a2c2895edb0d4ba51cdbd5390cac468fp-1,
    0x1.26976b1b16d19091c09259765c4b3872p-1,
    0x1.ea5c3ep-1,
  },
  { // Entry 254
    -0x1.a2c2895edb0d4ba51cdbd5390cac468fp-1,
    0x1.26976b1b16d19091c09259765c4b3872p-1,
    -0x1.ea5c3ep-1,
  },
  { // Entry 255
    0x1.c1e988b95614abd65d3d811f5c88039bp-1,
    0x1.e8c4040678d2ef736333a4537a1113a1p-2,
    0x1.12bd92p0,
  },
  { // Entry 256
    -0x1.c1e988b95614abd65d3d811f5c88039bp-1,
    0x1.e8c4040678d2ef736333a4537a1113a1p-2,
    -0x1.12bd92p0,
  },
  { // Entry 257
    0x1.d294d2e06b3d10a4de263172d50f4497p-1,
    0x1.a5a4c8f598fa0078971316eb4907f97bp-2,
    0x1.257b24p0,
  },
  { // Entry 258
    -0x1.d294d2e06b3d10a4de263172d50f4497p-1,
    0x1.a5a4c8f598fa0078971316eb4907f97bp-2,
    -0x1.257b24p0,
  },
  { // Entry 259
    0x1.e0c04bb65bd33012be72a340df2c044bp-1,
    0x1.60435beed10ca05769f0a3d86a5a20f3p-2,
    0x1.3838b6p0,
  },
  { // Entry 260
    -0x1.e0c04bb65bd33012be72a340df2c044bp-1,
    0x1.60435beed10ca05769f0a3d86a5a20f3p-2,
    -0x1.3838b6p0,
  },
  { // Entry 261
    0x1.ec5884eb990c3deaaeebd3f0f84d6962p-1,
    0x1.18fee0fc45c31a79b2b9478b1f72a9ebp-2,
    0x1.4af648p0,
  },
  { // Entry 262
    -0x1.ec5884eb990c3deaaeebd3f0f84d6962p-1,
    0x1.18fee0fc45c31a79b2b9478b1f72a9ebp-2,
    -0x1.4af648p0,
  },
  { // Entry 263
    0x1.f54d9835b0e66e17612160272521f3b0p-1,
    0x1.a072252090c33828767aee3e040ccddfp-3,
    0x1.5db3dap0,
  },
  { // Entry 264
    -0x1.f54d9835b0e66e17612160272521f3b0p-1,
    0x1.a072252090c33828767aee3e040ccddfp-3,
    -0x1.5db3dap0,
  },
  { // Entry 265
    0x1.fb933d1cd931685e902e403a1baaecfdp-1,
    0x1.0cab7703a8e9dacc4ad01188b443cfeep-3,
    0x1.70716cp0,
  },
  { // Entry 266
    -0x1.fb933d1cd931685e902e403a1baaecfdp-1,
    0x1.0cab7703a8e9dacc4ad01188b443cfeep-3,
    -0x1.70716cp0,
  },
  { // Entry 267
    0x1.ff20d9d3e8984fec33982e42f5884f2cp-1,
    0x1.ddd171a3c9851e7819b5e4f6f90e763dp-5,
    0x1.832efep0,
  },
  { // Entry 268
    -0x1.ff20d9d3e8984fec33982e42f5884f2cp-1,
    0x1.ddd171a3c9851e7819b5e4f6f90e763dp-5,
    -0x1.832efep0,
  },
  { // Entry 269
    0x1.fff18f03a4b7e6aacf51f83931e85042p-1,
    -0x1.e668cb154eea68bbc7f8154f46b2e536p-7,
    0x1.95ec90p0,
  },
  { // Entry 270
    -0x1.fff18f03a4b7e6aacf51f83931e85042p-1,
    -0x1.e668cb154eea68bbc7f8154f46b2e536p-7,
    -0x1.95ec90p0,
  },
  { // Entry 271
    0x1.fe043f875c6ed4a2c1b8d69a09fcf578p-1,
    -0x1.682f2bb87a8f5011735094176c9b6dacp-4,
    0x1.a8aa1cp0,
  },
  { // Entry 272
    -0x1.fe043f875c6ed4a2c1b8d69a09fcf578p-1,
    -0x1.682f2bb87a8f5011735094176c9b6dacp-4,
    -0x1.a8aa1cp0,
  },
  { // Entry 273
    0x1.b3d36a96880cf69d9884a49f5381e917p-1,
    0x1.0cb3449a0d0a9e0643d41f4a5b0f7db7p-1,
    0x1.04aff8p0,
  },
  { // Entry 274
    -0x1.b3d36a96880cf69d9884a49f5381e917p-1,
    0x1.0cb3449a0d0a9e0643d41f4a5b0f7db7p-1,
    -0x1.04aff8p0,
  },
  { // Entry 275
    0x1.b3d41aebcf391c30c3d2f1ee7b79710cp-1,
    0x1.0cb22697153bcf1f8a63acddd96c54cbp-1,
    0x1.04b0a0p0,
  },
  { // Entry 276
    -0x1.b3d41aebcf391c30c3d2f1ee7b79710cp-1,
    0x1.0cb22697153bcf1f8a63acddd96c54cbp-1,
    -0x1.04b0a0p0,
  },
  { // Entry 277
    0x1.b3d4cb405ab3292be7df5b1b98032fbep-1,
    0x1.0cb10893a9b5471a44356072cb33b395p-1,
    0x1.04b148p0,
  },
  { // Entry 278
    -0x1.b3d4cb405ab3292be7df5b1b98032fbep-1,
    0x1.0cb10893a9b5471a44356072cb33b395p-1,
    -0x1.04b148p0,
  },
  { // Entry 279
    0x1.b3d57b942a7ad19e9b9892c9319e1be6p-1,
    0x1.0cafea8fca7781236a57e5b1c8aed39cp-1,
    0x1.04b1f0p0,
  },
  { // Entry 280
    -0x1.b3d57b942a7ad19e9b9892c9319e1be6p-1,
    0x1.0cafea8fca7781236a57e5b1c8aed39cp-1,
    -0x1.04b1f0p0,
  },
  { // Entry 281
    0x1.b3d62be73e8fc998c6c2df6590425613p-1,
    0x1.0caecc8b7782f86827af92b0b2374510p-1,
    0x1.04b298p0,
  },
  { // Entry 282
    -0x1.b3d62be73e8fc998c6c2df6590425613p-1,
    0x1.0caecc8b7782f86827af92b0b2374510p-1,
    -0x1.04b298p0,
  },
  { // Entry 283
    0x1.b3d6dc3996f1c52aa1f83bdee1d0e023p-1,
    0x1.0cadae86b0d82815d8f632e67c7e1a99p-1,
    0x1.04b340p0,
  },
  { // Entry 284
    -0x1.b3d6dc3996f1c52aa1f83bdee1d0e023p-1,
    0x1.0cadae86b0d82815d8f632e67c7e1a99p-1,
    -0x1.04b340p0,
  },
  { // Entry 285
    0x1.b3d78c8b33a07864b6a878573db34bcap-1,
    0x1.0cac908176778b5a0cbad21ee75ce765p-1,
    0x1.04b3e8p0,
  },
  { // Entry 286
    -0x1.b3d78c8b33a07864b6a878573db34bcap-1,
    0x1.0cac908176778b5a0cbad21ee75ce765p-1,
    -0x1.04b3e8p0,
  },
  { // Entry 287
    0x1.b3d83cdc149b9757df195ad885ab5201p-1,
    0x1.0cab727bc8619d628361876e1f30a633p-1,
    0x1.04b490p0,
  },
  { // Entry 288
    -0x1.b3d83cdc149b9757df195ad885ab5201p-1,
    0x1.0cab727bc8619d628361876e1f30a633p-1,
    -0x1.04b490p0,
  },
  { // Entry 289
    0x1.b3d8e8f9908360b38cd13fcbf6224d93p-1,
    0x1.0caa5b450a4324f204a556b072da124ap-1,
    0x1.04b534p0,
  },
  { // Entry 290
    -0x1.b3d8e8f9908360b38cd13fcbf6224d93p-1,
    0x1.0caa5b450a4324f204a556b072da124ap-1,
    -0x1.04b534p0,
  },
  { // Entry 291
    -0.0f,
    0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.p-149,
  },
  { // Entry 292
    0.0f,
    0x1.ffffffffffffffffffffffffffffffffp-1,
    0x1.p-149,
  },
  { // Entry 293
    0.0,
    0x1.p0,
    0.0,
  },
  { // Entry 294
    0.0f,
    0x1.ffffffffffffffffffffffffffffffffp-1,
    0x1.p-149,
  },
  { // Entry 295
    -0.0f,
    0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.p-149,
  },
  { // Entry 296
    0x1.1773d36a64df61d6715e60af063559f4p-1,
    0x1.ad02c8b9cc93f448ef4eb068a88922a3p-1,
    0x1.279a72p-1,
  },
  { // Entry 297
    -0x1.1773d36a64df61d6715e60af063559f4p-1,
    0x1.ad02c8b9cc93f448ef4eb068a88922a3p-1,
    -0x1.279a72p-1,
  },
  { // Entry 298
    0x1.1773d51767a78fe91b55f6b7e5fd44c2p-1,
    0x1.ad02c7a258bfb362abbe86fb48f4e98bp-1,
    0x1.279a74p-1,
  },
  { // Entry 299
    -0x1.1773d51767a78fe91b55f6b7e5fd44c2p-1,
    0x1.ad02c7a258bfb362abbe86fb48f4e98bp-1,
    -0x1.279a74p-1,
  },
  { // Entry 300
    0x1.1773d6c46a6ea687f03625194d25bb52p-1,
    0x1.ad02c68ae4e9c579a08c04ce59be4002p-1,
    0x1.279a76p-1,
  },
  { // Entry 301
    -0x1.1773d6c46a6ea687f03625194d25bb52p-1,
    0x1.ad02c68ae4e9c579a08c04ce59be4002p-1,
    -0x1.279a76p-1,
  },
  { // Entry 302
    0x1.f95b8f40501057ac49acef13993b0c55p-1,
    -0x1.48d1c9e98b6c08784f10040f47a12191p-3,
    0x1.bb67acp0,
  },
  { // Entry 303
    -0x1.f95b8f40501057ac49acef13993b0c55p-1,
    -0x1.48d1c9e98b6c08784f10040f47a12191p-3,
    -0x1.bb67acp0,
  },
  { // Entry 304
    0x1.f95b8e9be727702f7595ae1000a14a1ap-1,
    -0x1.48d1d9b467e37955337311decd09fc74p-3,
    0x1.bb67aep0,
  },
  { // Entry 305
    -0x1.f95b8e9be727702f7595ae1000a14a1ap-1,
    -0x1.48d1d9b467e37955337311decd09fc74p-3,
    -0x1.bb67aep0,
  },
  { // Entry 306
    0x1.f95b8df77e36a344670ed07149191a58p-1,
    -0x1.48d1e97f4455c6eab1048022238b2bd0p-3,
    0x1.bb67b0p0,
  },
  { // Entry 307
    -0x1.f95b8df77e36a344670ed07149191a58p-1,
    -0x1.48d1e97f4455c6eab1048022238b2bd0p-3,
    -0x1.bb67b0p0,
  },
  { // Entry 308
    0x1.b1d82e835a918de18f5fdadc8b1240cfp-2,
    0x1.cfc6d011a0e5d0fcebb54b5fed672940p-1,
    0x1.bffffep-2,
  },
  { // Entry 309
    -0x1.b1d82e835a918de18f5fdadc8b1240cfp-2,
    0x1.cfc6d011a0e5d0fcebb54b5fed672940p-1,
    -0x1.bffffep-2,
  },
  { // Entry 310
    0x1.b1d83053216169476f4d1982b9b14ab1p-2,
    0x1.cfc6cfa52ad9f62d6d5423ca8339a00ap-1,
    0x1.c0p-2,
  },
  { // Entry 311
    -0x1.b1d83053216169476f4d1982b9b14ab1p-2,
    0x1.cfc6cfa52ad9f62d6d5423ca8339a00ap-1,
    -0x1.c0p-2,
  },
  { // Entry 312
    0x1.b1d83222e830d83743258fd09040ee56p-2,
    0x1.cfc6cf38b4cda76c3b09b17e9deb19eap-1,
    0x1.c00002p-2,
  },
  { // Entry 313
    -0x1.b1d83222e830d83743258fd09040ee56p-2,
    0x1.cfc6cf38b4cda76c3b09b17e9deb19eap-1,
    -0x1.c00002p-2,
  },
  { // Entry 314
    0x1.44eb3691428062b27925c585ad59d62ap-1,
    0x1.8bb106eac7c75d33fbb19446313ecc2fp-1,
    0x1.5ffffep-1,
  },
  { // Entry 315
    -0x1.44eb3691428062b27925c585ad59d62ap-1,
    0x1.8bb106eac7c75d33fbb19446313ecc2fp-1,
    -0x1.5ffffep-1,
  },
  { // Entry 316
    0x1.44eb381cf386ab04a4f8656abea80b83p-1,
    0x1.8bb105a5dc900618f80fa51d303c69p-1,
    0x1.60p-1,
  },
  { // Entry 317
    -0x1.44eb381cf386ab04a4f8656abea80b83p-1,
    0x1.8bb105a5dc900618f80fa51d303c69p-1,
    -0x1.60p-1,
  },
  { // Entry 318
    0x1.44eb39a8a48bae6b98ae11c9400535e5p-1,
    0x1.8bb10460f157234ceec7d9644a1a78e5p-1,
    0x1.600002p-1,
  },
  { // Entry 319
    -0x1.44eb39a8a48bae6b98ae11c9400535e5p-1,
    0x1.8bb10460f157234ceec7d9644a1a78e5p-1,
    -0x1.600002p-1,
  },
  { // Entry 320
    0x1.dad9017b96408c375d4faf0e4776d1fcp-1,
    0x1.7ef48b9a6fd5c24f5ec39839e1729b78p-2,
    0x1.2ffffep0,
  },
  { // Entry 321
    -0x1.dad9017b96408c375d4faf0e4776d1fcp-1,
    0x1.7ef48b9a6fd5c24f5ec39839e1729b78p-2,
    -0x1.2ffffep0,
  },
  { // Entry 322
    0x1.dad902fa8ac870f52f1b843ac83bc3edp-1,
    0x1.7ef4842f0bccd60d4a501dc8bc4b57b3p-2,
    0x1.30p0,
  },
  { // Entry 323
    -0x1.dad902fa8ac870f52f1b843ac83bc3edp-1,
    0x1.7ef4842f0bccd60d4a501dc8bc4b57b3p-2,
    -0x1.30p0,
  },
  { // Entry 324
    0x1.dad904797f48ea4ef4fd2e47fe4d52bdp-1,
    0x1.7ef47cc3a7bdedf9252074263d8a4596p-2,
    0x1.300002p0,
  },
  { // Entry 325
    -0x1.dad904797f48ea4ef4fd2e47fe4d52bdp-1,
    0x1.7ef47cc3a7bdedf9252074263d8a4596p-2,
    -0x1.300002p0,
  },
  { // Entry 326
    0x1.4b708093c9cb45355e7821e5aad98ce8p-1,
    -0x1.863ef5085bcc358d2ae8525bf39f0c40p-1,
    0x1.37fffep1,
  },
  { // Entry 327
    -0x1.4b708093c9cb45355e7821e5aad98ce8p-1,
    -0x1.863ef5085bcc358d2ae8525bf39f0c40p-1,
    -0x1.37fffep1,
  },
  { // Entry 328
    0x1.4b707a7acdecc84239463e78b312fa10p-1,
    -0x1.863efa361dc252bca1eaeed39749bed7p-1,
    0x1.38p1,
  },
  { // Entry 329
    -0x1.4b707a7acdecc84239463e78b312fa10p-1,
    -0x1.863efa361dc252bca1eaeed39749bed7p-1,
    -0x1.38p1,
  },
  { // Entry 330
    0x1.4b707461d1f994476c677c5ad5ddb264p-1,
    -0x1.863eff63dfa00bfc758baf469469d741p-1,
    0x1.380002p1,
  },
  { // Entry 331
    -0x1.4b707461d1f994476c677c5ad5ddb264p-1,
    -0x1.863eff63dfa00bfc758baf469469d741p-1,
    -0x1.380002p1,
  },
  { // Entry 332
    0x1.066e7f705a6ca2b9e107f7dc9f3b26e6p-4,
    0x1.fef2b2d0a10e2739c566936480a1479bp-1,
    0x1.069c8cp-4,
  },
  { // Entry 333
    -0x1.066e7f705a6ca2b9e107f7dc9f3b26e6p-4,
    0x1.fef2b2d0a10e2739c566936480a1479bp-1,
    -0x1.069c8cp-4,
  },
  { // Entry 334
    0x1.05e476d27febc8b7e9690009b367c327p-3,
    0x1.fbcbe68dd10bad0a229ccbb580cc5436p-1,
    0x1.069c8cp-3,
  },
  { // Entry 335
    -0x1.05e476d27febc8b7e9690009b367c327p-3,
    0x1.fbcbe68dd10bad0a229ccbb580cc5436p-1,
    -0x1.069c8cp-3,
  },
  { // Entry 336
    0x1.877e2de5c9a066b8db595adc149af0c0p-3,
    0x1.f68eebef72e7f6126b3f3dde646a755cp-1,
    0x1.89ead2p-3,
  },
  { // Entry 337
    -0x1.877e2de5c9a066b8db595adc149af0c0p-3,
    0x1.f68eebef72e7f6126b3f3dde646a755cp-1,
    -0x1.89ead2p-3,
  },
  { // Entry 338
    0x1.03be07acb9dab719b4343a33b9fa6afep-2,
    0x1.ef41459d2e90ea1b7faad7fabd1fd444p-1,
    0x1.069c8cp-2,
  },
  { // Entry 339
    -0x1.03be07acb9dab719b4343a33b9fa6afep-2,
    0x1.ef41459d2e90ea1b7faad7fabd1fd444p-1,
    -0x1.069c8cp-2,
  },
  { // Entry 340
    0x1.42abbc5b3b2f91e8ece46e5effd28369p-2,
    0x1.e5eaa23a27fe8d6890a3edace1c61998p-1,
    0x1.4843b0p-2,
  },
  { // Entry 341
    -0x1.42abbc5b3b2f91e8ece46e5effd28369p-2,
    0x1.e5eaa23a27fe8d6890a3edace1c61998p-1,
    -0x1.4843b0p-2,
  },
  { // Entry 342
    0x1.804601411d93f4750919670061de07d9p-2,
    0x1.da94d4b99c3a9a5e0d1fc86d53369a84p-1,
    0x1.89ead4p-2,
  },
  { // Entry 343
    -0x1.804601411d93f4750919670061de07d9p-2,
    0x1.da94d4b99c3a9a5e0d1fc86d53369a84p-1,
    -0x1.89ead4p-2,
  },
  { // Entry 344
    0x1.bc4c08af356088b1694995bfaf8a297bp-2,
    0x1.cd4bc9afc01230b2f982f6968dab7f05p-1,
    0x1.cb91f8p-2,
  },
  { // Entry 345
    -0x1.bc4c08af356088b1694995bfaf8a297bp-2,
    0x1.cd4bc9afc01230b2f982f6968dab7f05p-1,
    -0x1.cb91f8p-2,
  },
  { // Entry 346
    0x1.f67eae34dc0b42b465fd2a3fb07564a4p-2,
    0x1.be1d7adf077def2a360fec23dbbcef09p-1,
    0x1.069c8ep-1,
  },
  { // Entry 347
    -0x1.f67eae34dc0b42b465fd2a3fb07564a4p-2,
    0x1.be1d7adf077def2a360fec23dbbcef09p-1,
    -0x1.069c8ep-1,
  },
  { // Entry 348
    0x1.17505c86231898fd86b18d2282d93eedp-1,
    0x1.ad19e0847d25f3aa142289dab557bf96p-1,
    0x1.277020p-1,
  },
  { // Entry 349
    -0x1.17505c86231898fd86b18d2282d93eedp-1,
    0x1.ad19e0847d25f3aa142289dab557bf96p-1,
    -0x1.277020p-1,
  },
  { // Entry 350
    0x1.323b8e40d16575e50dc7b6e567bb5084p-1,
    0x1.9a52e08b191bd55512c8365074f1987fp-1,
    0x1.4843b2p-1,
  },
  { // Entry 351
    -0x1.323b8e40d16575e50dc7b6e567bb5084p-1,
    0x1.9a52e08b191bd55512c8365074f1987fp-1,
    -0x1.4843b2p-1,
  },
  { // Entry 352
    0x1.4be49b08a1e1629cbdaa507e18255cd8p-1,
    0x1.85dc3bb7c2e9abb5cccb6d96d12d39c4p-1,
    0x1.691744p-1,
  },
  { // Entry 353
    -0x1.4be49b08a1e1629cbdaa507e18255cd8p-1,
    0x1.85dc3bb7c2e9abb5cccb6d96d12d39c4p-1,
    -0x1.691744p-1,
  },
  { // Entry 354
    0x1.6430847dbbbfd46cbebbc6d5f51c7c49p-1,
    0x1.6fcb78e1cd65d2e4fde7118caac79d6dp-1,
    0x1.89ead6p-1,
  },
  { // Entry 355
    -0x1.6430847dbbbfd46cbebbc6d5f51c7c49p-1,
    0x1.6fcb78e1cd65d2e4fde7118caac79d6dp-1,
    -0x1.89ead6p-1,
  },
  { // Entry 356
    0x1.7b05bb87b38844e56003c41ef804b273p-1,
    0x1.5837ce4dc835d4a5454ec0a1bb394081p-1,
    0x1.aabe68p-1,
  },
  { // Entry 357
    -0x1.7b05bb87b38844e56003c41ef804b273p-1,
    0x1.5837ce4dc835d4a5454ec0a1bb394081p-1,
    -0x1.aabe68p-1,
  },
  { // Entry 358
    0x1.904c3b389d55d3deddb39d05eb366571p-1,
    0x1.3f3a09427966e9518802dee3bf443a95p-1,
    0x1.cb91fap-1,
  },
  { // Entry 359
    -0x1.904c3b389d55d3deddb39d05eb366571p-1,
    0x1.3f3a09427966e9518802dee3bf443a95p-1,
    -0x1.cb91fap-1,
  },
  { // Entry 360
    0x1.a3eda211798a82697d62431f9ae46cc4p-1,
    0x1.24ec73f1aeef4940bb8da19a82bbc49fp-1,
    0x1.ec658cp-1,
  },
  { // Entry 361
    -0x1.a3eda211798a82697d62431f9ae46cc4p-1,
    0x1.24ec73f1aeef4940bb8da19a82bbc49fp-1,
    -0x1.ec658cp-1,
  },
  { // Entry 362
    0x1.b5d54883fcb6123bc28aac91f085e4eep-1,
    0x1.096abb862f9bd5515982c2818c332ff9p-1,
    0x1.069c8ep0,
  },
  { // Entry 363
    -0x1.b5d54883fcb6123bc28aac91f085e4eep-1,
    0x1.096abb862f9bd5515982c2818c332ff9p-1,
    -0x1.069c8ep0,
  },
  { // Entry 364
    0x1.c5f05a0135d4882c768cdf18e2e1112cp-1,
    0x1.d9a39c0dddc654c717e3036da5dd685cp-2,
    0x1.170656p0,
  },
  { // Entry 365
    -0x1.c5f05a0135d4882c768cdf18e2e1112cp-1,
    0x1.d9a39c0dddc654c717e3036da5dd685cp-2,
    -0x1.170656p0,
  },
  { // Entry 366
    0x1.d42de53e315c839ce188e201205e99dep-1,
    0x1.9e7f81840c0bbd0f1b13733061062d34p-2,
    0x1.27701ep0,
  },
  { // Entry 367
    -0x1.d42de53e315c839ce188e201205e99dep-1,
    0x1.9e7f81840c0bbd0f1b13733061062d34p-2,
    -0x1.27701ep0,
  },
  { // Entry 368
    0x1.e07eef45d91eea8a6cc7369aa0e55388p-1,
    0x1.61a75e2deb596731c8cd45e3d9794526p-2,
    0x1.37d9e6p0,
  },
  { // Entry 369
    -0x1.e07eef45d91eea8a6cc7369aa0e55388p-1,
    0x1.61a75e2deb596731c8cd45e3d9794526p-2,
    -0x1.37d9e6p0,
  },
  { // Entry 370
    0x1.ead6833b2aa002baa1c2b19a38dc9b79p-1,
    0x1.235b337b091cdd8ac06390abc6816b82p-2,
    0x1.4843aep0,
  },
  { // Entry 371
    -0x1.ead6833b2aa002baa1c2b19a38dc9b79p-1,
    0x1.235b337b091cdd8ac06390abc6816b82p-2,
    -0x1.4843aep0,
  },
  { // Entry 372
    0x1.f329bffa6a208591eecb6905d7594e3bp-1,
    0x1.c7b9146d6d10824ff652dc390ba2d7f9p-3,
    0x1.58ad76p0,
  },
  { // Entry 373
    -0x1.f329bffa6a208591eecb6905d7594e3bp-1,
    0x1.c7b9146d6d10824ff652dc390ba2d7f9p-3,
    -0x1.58ad76p0,
  },
  { // Entry 374
    0x1.f96fe38afbd95b5fcd08608110e9381fp-1,
    0x1.46dc5b2f1de977efff7c278b5adb2a75p-3,
    0x1.69173ep0,
  },
  { // Entry 375
    -0x1.f96fe38afbd95b5fcd08608110e9381fp-1,
    0x1.46dc5b2f1de977efff7c278b5adb2a75p-3,
    -0x1.69173ep0,
  },
  { // Entry 376
    0x1.fda25455d9567772f20f25d15efc6775p-1,
    0x1.894f93ef49c4575800bbd646a3a31d2ap-4,
    0x1.798106p0,
  },
  { // Entry 377
    -0x1.fda25455d9567772f20f25d15efc6775p-1,
    0x1.894f93ef49c4575800bbd646a3a31d2ap-4,
    -0x1.798106p0,
  },
  { // Entry 378
    0x1.ffbca816f1f1516ec5d757b0db54ae34p-1,
    0x1.069164e3f5cee94d865fb52e316dff6bp-5,
    0x1.89eacep0,
  },
  { // Entry 379
    -0x1.ffbca816f1f1516ec5d757b0db54ae34p-1,
    0x1.069164e3f5cee94d865fb52e316dff6bp-5,
    -0x1.89eacep0,
  },
  { // Entry 380
    0x1.ffbca88228b163189ab8d637db99bd2dp-1,
    -0x1.069093eec0ed066ec83dd034498ef8bfp-5,
    0x1.9a5496p0,
  },
  { // Entry 381
    -0x1.ffbca88228b163189ab8d637db99bd2dp-1,
    -0x1.069093eec0ed066ec83dd034498ef8bfp-5,
    -0x1.9a5496p0,
  },
  { // Entry 382
    0x1.fda255970ccddb9d127ecf63403c2bf7p-1,
    -0x1.894f2be2979dd9ced83ccc60cf49cd44p-4,
    0x1.aabe5ep0,
  },
  { // Entry 383
    -0x1.fda255970ccddb9d127ecf63403c2bf7p-1,
    -0x1.894f2be2979dd9ced83ccc60cf49cd44p-4,
    -0x1.aabe5ep0,
  },
  { // Entry 384
    0x1.f96fe5a0da244489fb2f4b97b3e48757p-1,
    -0x1.46dc2796735195a15c80e5b719e2fc42p-3,
    0x1.bb2826p0,
  },
  { // Entry 385
    -0x1.f96fe5a0da244489fb2f4b97b3e48757p-1,
    -0x1.46dc2796735195a15c80e5b719e2fc42p-3,
    -0x1.bb2826p0,
  },
  { // Entry 386
    0x1.f329c2e2c1a39bad8ecdcb87961ba44ap-1,
    -0x1.c7b8e178b7e8c01d9f320466cc7a68d4p-3,
    0x1.cb91eep0,
  },
  { // Entry 387
    -0x1.f329c2e2c1a39bad8ecdcb87961ba44ap-1,
    -0x1.c7b8e178b7e8c01d9f320466cc7a68d4p-3,
    -0x1.cb91eep0,
  },
  { // Entry 388
    0x1.ead686f2ec572c83ed34a01f764d193ep-1,
    -0x1.235b1a6d767e4b362c64571ac97b4a1cp-2,
    0x1.dbfbb6p0,
  },
  { // Entry 389
    -0x1.ead686f2ec572c83ed34a01f764d193ep-1,
    -0x1.235b1a6d767e4b362c64571ac97b4a1cp-2,
    -0x1.dbfbb6p0,
  },
  { // Entry 390
    0x1.e07ef3c91bd500a0de230ad573163163p-1,
    -0x1.61a745a77b7e83c2f8a2f9b091e89aaap-2,
    0x1.ec657ep0,
  },
  { // Entry 391
    -0x1.e07ef3c91bd500a0de230ad573163163p-1,
    -0x1.61a745a77b7e83c2f8a2f9b091e89aaap-2,
    -0x1.ec657ep0,
  },
  { // Entry 392
    0x1.d42dea8835c88adb9cde17347f934e25p-1,
    -0x1.9e7f699e8b9aaf8ed51c71c8f73b0b74p-2,
    0x1.fccf46p0,
  },
  { // Entry 393
    -0x1.d42dea8835c88adb9cde17347f934e25p-1,
    -0x1.9e7f699e8b9aaf8ed51c71c8f73b0b74p-2,
    -0x1.fccf46p0,
  },
  { // Entry 394
    0x1.c5f05e32c80fb0fe603033ec028a4c32p-1,
    -0x1.d9a38bfa3195ba1caa7fb69bc1d04e42p-2,
    0x1.069c88p1,
  },
  { // Entry 395
    -0x1.c5f05e32c80fb0fe603033ec028a4c32p-1,
    -0x1.d9a38bfa3195ba1caa7fb69bc1d04e42p-2,
    -0x1.069c88p1,
  },
  { // Entry 396
    0x1.b5d54d3732d3b2e79d4907e115401ddap-1,
    -0x1.096ab3c55c91f36e2359ed1c5a8342dfp-1,
    0x1.0ed16cp1,
  },
  { // Entry 397
    -0x1.b5d54d3732d3b2e79d4907e115401ddap-1,
    -0x1.096ab3c55c91f36e2359ed1c5a8342dfp-1,
    -0x1.0ed16cp1,
  },
  { // Entry 398
    0x1.a3eda74161d06b83ec2c8dc396d813b9p-1,
    -0x1.24ec6c8206e744322d99f47e9e41becep-1,
    0x1.170650p1,
  },
  { // Entry 399
    -0x1.a3eda74161d06b83ec2c8dc396d813b9p-1,
    -0x1.24ec6c8206e744322d99f47e9e41becep-1,
    -0x1.170650p1,
  },
  { // Entry 400
    0x1.904c421efce58f4e8170d36dcda8e02cp-1,
    -0x1.3f3a009b82b5b8234e1296dd73cff49dp-1,
    0x1.1f3b34p1,
  },
  { // Entry 401
    -0x1.904c421efce58f4e8170d36dcda8e02cp-1,
    -0x1.3f3a009b82b5b8234e1296dd73cff49dp-1,
    -0x1.1f3b34p1,
  },
  { // Entry 402
    0x1.7b05c45093944d6afb0c90d2f9cb217fp-1,
    -0x1.5837c4a184ccf7ed57c189f2addf32c5p-1,
    0x1.277018p1,
  },
  { // Entry 403
    -0x1.7b05c45093944d6afb0c90d2f9cb217fp-1,
    -0x1.5837c4a184ccf7ed57c189f2addf32c5p-1,
    -0x1.277018p1,
  },
  { // Entry 404
    0x1.64308f506ffdaf1326d10b3380278e98p-1,
    -0x1.6fcb6e6685e72fb4074e70cd3162d3bap-1,
    0x1.2fa4fcp1,
  },
  { // Entry 405
    -0x1.64308f506ffdaf1326d10b3380278e98p-1,
    -0x1.6fcb6e6685e72fb4074e70cd3162d3bap-1,
    -0x1.2fa4fcp1,
  },
  { // Entry 406
    0x1.4be4a8076c135a48f3f1a1aaa362475fp-1,
    -0x1.85dc30a79f26754ab1370338ee7bfd11p-1,
    0x1.37d9e0p1,
  },
  { // Entry 407
    -0x1.4be4a8076c135a48f3f1a1aaa362475fp-1,
    -0x1.85dc30a79f26754ab1370338ee7bfd11p-1,
    -0x1.37d9e0p1,
  },
  { // Entry 408
    0x1.323b9d888d4da77a610893735eeed1cbp-1,
    -0x1.9a52d523b1532e4ed477e27dc6051c12p-1,
    0x1.400ec4p1,
  },
  { // Entry 409
    -0x1.323b9d888d4da77a610893735eeed1cbp-1,
    -0x1.9a52d523b1532e4ed477e27dc6051c12p-1,
    -0x1.400ec4p1,
  },
  { // Entry 410
    0x1.17506e2dfb603d34b9af39b12c1db735p-1,
    -0x1.ad19d50664abf0c0141137d2ca509f21p-1,
    0x1.4843a8p1,
  },
  { // Entry 411
    -0x1.17506e2dfb603d34b9af39b12c1db735p-1,
    -0x1.ad19d50664abf0c0141137d2ca509f21p-1,
    -0x1.4843a8p1,
  },
  { // Entry 412
    0x1.f67ed667352d4827450013f15e321bfbp-2,
    -0x1.be1d6f8d517db5c2cf7de0faf0808d30p-1,
    0x1.50788cp1,
  },
  { // Entry 413
    -0x1.f67ed667352d4827450013f15e321bfbp-2,
    -0x1.be1d6f8d517db5c2cf7de0faf0808d30p-1,
    -0x1.50788cp1,
  },
  { // Entry 414
    0x1.bc4c35da51e34b776e5e04da58f23441p-2,
    -0x1.cd4bbecf7f2705d4fd00dd463780f45ep-1,
    0x1.58ad70p1,
  },
  { // Entry 415
    -0x1.bc4c35da51e34b776e5e04da58f23441p-2,
    -0x1.cd4bbecf7f2705d4fd00dd463780f45ep-1,
    -0x1.58ad70p1,
  },
  { // Entry 416
    0x1.8046336e68427cf756056d3f4edbb662p-2,
    -0x1.da94ca915da3cdd1fff839d85eec39e2p-1,
    0x1.60e254p1,
  },
  { // Entry 417
    -0x1.8046336e68427cf756056d3f4edbb662p-2,
    -0x1.da94ca915da3cdd1fff839d85eec39e2p-1,
    -0x1.60e254p1,
  },
  { // Entry 418
    0x1.42abf3872905e632f204c41b24af90b6p-2,
    -0x1.e5ea99116b39361ac926dd9fdc2089d1p-1,
    0x1.691738p1,
  },
  { // Entry 419
    -0x1.42abf3872905e632f204c41b24af90b6p-2,
    -0x1.e5ea99116b39361ac926dd9fdc2089d1p-1,
    -0x1.691738p1,
  },
  { // Entry 420
    0x1.03be43c699f3536990dcf5a6665ac239p-2,
    -0x1.ef413dbbda2859ffb0d1ab84342fd235p-1,
    0x1.714c1cp1,
  },
  { // Entry 421
    -0x1.03be43c699f3536990dcf5a6665ac239p-2,
    -0x1.ef413dbbda2859ffb0d1ab84342fd235p-1,
    -0x1.714c1cp1,
  },
  { // Entry 422
    0x1.877eadc2fdfc2f0db1e8b78cd3fbfbd2p-3,
    -0x1.f68ee5b5bf356b10230944a18e70925cp-1,
    0x1.7981p1,
  },
  { // Entry 423
    -0x1.877eadc2fdfc2f0db1e8b78cd3fbfbd2p-3,
    -0x1.f68ee5b5bf356b10230944a18e70925cp-1,
    -0x1.7981p1,
  },
  { // Entry 424
    0x1.05e4fdf846632a8208d90de72d3a6da8p-3,
    -0x1.fbcbe23296fc61b96f382f35ea15c768p-1,
    0x1.81b5e4p1,
  },
  { // Entry 425
    -0x1.05e4fdf846632a8208d90de72d3a6da8p-3,
    -0x1.fbcbe23296fc61b96f382f35ea15c768p-1,
    -0x1.81b5e4p1,
  },
  { // Entry 426
    0x1.066f9b630b72dff16450e89afdf7e048p-4,
    -0x1.fef2b08943197cd3a8ba861095227c48p-1,
    0x1.89eac8p1,
  },
  { // Entry 427
    -0x1.066f9b630b72dff16450e89afdf7e048p-4,
    -0x1.fef2b08943197cd3a8ba861095227c48p-1,
    -0x1.89eac8p1,
  },
  { // Entry 428
    0x1.03bdf0b79ccf739529d54d422861046cp-2,
    0x1.ef41489fc2fe801a6fc8ae791438eb78p-1,
    -0x1.81b5eep2,
  },
  { // Entry 429
    -0x1.03bdf0b79ccf739529d54d422861046cp-2,
    0x1.ef41489fc2fe801a6fc8ae791438eb78p-1,
    0x1.81b5eep2,
  },
  { // Entry 430
    0x1.f67e8b95f5460ea369a803837b721abdp-2,
    0x1.be1d849ec649b797320e985d0b82ae85p-1,
    -0x1.714c26p2,
  },
  { // Entry 431
    -0x1.f67e8b95f5460ea369a803837b721abdp-2,
    0x1.be1d849ec649b797320e985d0b82ae85p-1,
    0x1.714c26p2,
  },
  { // Entry 432
    0x1.643070791751dc0636d1854d2bdbc5d4p-1,
    0x1.6fcb8c44bd30dd668148605969b1c161p-1,
    -0x1.60e25ep2,
  },
  { // Entry 433
    -0x1.643070791751dc0636d1854d2bdbc5d4p-1,
    0x1.6fcb8c44bd30dd668148605969b1c161p-1,
    0x1.60e25ep2,
  },
  { // Entry 434
    0x1.b5d536f59113a43af30e8c9db8a951a5p-1,
    0x1.096ad87c326622c42de34f92814cfa84p-1,
    -0x1.507896p2,
  },
  { // Entry 435
    -0x1.b5d536f59113a43af30e8c9db8a951a5p-1,
    0x1.096ad87c326622c42de34f92814cfa84p-1,
    0x1.507896p2,
  },
  { // Entry 436
    0x1.ead679985549140318349f512dca7a6bp-1,
    0x1.235b746a2a2eff2bf640dd8c04d35a5bp-2,
    -0x1.400ecep2,
  },
  { // Entry 437
    -0x1.ead679985549140318349f512dca7a6bp-1,
    0x1.235b746a2a2eff2bf640dd8c04d35a5bp-2,
    0x1.400ecep2,
  },
  { // Entry 438
    0x1.ffbca7010e0b0452f56075cfd5982880p-1,
    0x1.0693827b46cee3b661ac17114b5fe0fbp-5,
    -0x1.2fa506p2,
  },
  { // Entry 439
    -0x1.ffbca7010e0b0452f56075cfd5982880p-1,
    0x1.0693827b46cee3b661ac17114b5fe0fbp-5,
    0x1.2fa506p2,
  },
  { // Entry 440
    0x1.f329ca6bfc7425d89c2b4b9ad73ab108p-1,
    -0x1.c7b85d668e2abcc46542ca8527f0b801p-3,
    -0x1.1f3b3ep2,
  },
  { // Entry 441
    -0x1.f329ca6bfc7425d89c2b4b9ad73ab108p-1,
    -0x1.c7b85d668e2abcc46542ca8527f0b801p-3,
    0x1.1f3b3ep2,
  },
  { // Entry 442
    0x1.c5f06fb69427ac0f2d69428d82b5e669p-1,
    -0x1.d9a348d4f4363ba4562110db01ee84e8p-2,
    -0x1.0ed176p2,
  },
  { // Entry 443
    -0x1.c5f06fb69427ac0f2d69428d82b5e669p-1,
    -0x1.d9a348d4f4363ba4562110db01ee84e8p-2,
    0x1.0ed176p2,
  },
  { // Entry 444
    0x1.7b05d864ec9802adbc4b5577c233836ap-1,
    -0x1.5837ae8569c95846e6164d9636546120p-1,
    -0x1.fccf5ap1,
  },
  { // Entry 445
    -0x1.7b05d864ec9802adbc4b5577c233836ap-1,
    -0x1.5837ae8569c95846e6164d9636546120p-1,
    0x1.fccf5ap1,
  },
  { // Entry 446
    0x1.1750808185a998bbcecc3a6ac0cb2907p-1,
    -0x1.ad19c918883000b0b702ec080cf0122ep-1,
    -0x1.dbfbc8p1,
  },
  { // Entry 447
    -0x1.1750808185a998bbcecc3a6ac0cb2907p-1,
    -0x1.ad19c918883000b0b702ec080cf0122ep-1,
    0x1.dbfbc8p1,
  },
  { // Entry 448
    0x1.42ac0dd9495211816bf04ca53bce4beap-2,
    -0x1.e5ea94b2cf07add3d0d95ab3a30ad4abp-1,
    -0x1.bb2836p1,
  },
  { // Entry 449
    -0x1.42ac0dd9495211816bf04ca53bce4beap-2,
    -0x1.e5ea94b2cf07add3d0d95ab3a30ad4abp-1,
    0x1.bb2836p1,
  },
  { // Entry 450
    0x1.066fca39a70b52d06f2cd7eab69c31f2p-4,
    -0x1.fef2b02908559f92de892d240a2b0b49p-1,
    -0x1.9a54a4p1,
  },
  { // Entry 451
    -0x1.066fca39a70b52d06f2cd7eab69c31f2p-4,
    -0x1.fef2b02908559f92de892d240a2b0b49p-1,
    0x1.9a54a4p1,
  },
  { // Entry 452
    -0x1.877d931298e6fbc654f065536cff2b54p-3,
    -0x1.f68ef3792e592c3cefbce1d5ded64a92p-1,
    -0x1.798112p1,
  },
  { // Entry 453
    0x1.877d931298e6fbc654f065536cff2b54p-3,
    -0x1.f68ef3792e592c3cefbce1d5ded64a92p-1,
    0x1.798112p1,
  },
  { // Entry 454
    -0x1.bc4bc2875eb6d38eda3b49cb2320b561p-2,
    -0x1.cd4bda943eea13630f8e508f8744f2f2p-1,
    -0x1.58ad80p1,
  },
  { // Entry 455
    0x1.bc4bc2875eb6d38eda3b49cb2320b561p-2,
    -0x1.cd4bda943eea13630f8e508f8744f2f2p-1,
    0x1.58ad80p1,
  },
  { // Entry 456
    -0x1.4be47d6354c4ced53780b1b519acdec2p-1,
    -0x1.85dc54f49f324bdfc71d5749483b3318p-1,
    -0x1.37d9eep1,
  },
  { // Entry 457
    0x1.4be47d6354c4ced53780b1b519acdec2p-1,
    -0x1.85dc54f49f324bdfc71d5749483b3318p-1,
    0x1.37d9eep1,
  },
  { // Entry 458
    -0x1.a3ed8bcb35cbcf8c6089f82a91c31d5bp-1,
    -0x1.24ec93e04d4bdb54e20beaf383519af8p-1,
    -0x1.17065cp1,
  },
  { // Entry 459
    0x1.a3ed8bcb35cbcf8c6089f82a91c31d5bp-1,
    -0x1.24ec93e04d4bdb54e20beaf383519af8p-1,
    0x1.17065cp1,
  },
  { // Entry 460
    -0x1.e07ee496ea109654c42e171fdc4537c4p-1,
    -0x1.61a7983d4c16c451b68bf2f5b70f3b6ap-2,
    -0x1.ec6594p0,
  },
  { // Entry 461
    0x1.e07ee496ea109654c42e171fdc4537c4p-1,
    -0x1.61a7983d4c16c451b68bf2f5b70f3b6ap-2,
    0x1.ec6594p0,
  },
  { // Entry 462
    -0x1.fda2522219689d0e8069d90f5c969b92p-1,
    -0x1.89504a8de6c9ecac663e67583cab47e8p-4,
    -0x1.aabe70p0,
  },
  { // Entry 463
    0x1.fda2522219689d0e8069d90f5c969b92p-1,
    -0x1.89504a8de6c9ecac663e67583cab47e8p-4,
    0x1.aabe70p0,
  },
  { // Entry 464
    -0x1.f96fe802fe570372d0fcb6e934b43061p-1,
    0x1.46dbec9ea3a5f08ba73aa69e7e22de1cp-3,
    -0x1.69174cp0,
  },
  { // Entry 465
    0x1.f96fe802fe570372d0fcb6e934b43061p-1,
    0x1.46dbec9ea3a5f08ba73aa69e7e22de1cp-3,
    0x1.69174cp0,
  },
  { // Entry 466
    -0x1.d42ded56ae88a6e1cf270af27e6f1804p-1,
    0x1.9e7f5cf075d1ec4ef69c9c67b62c27cbp-2,
    -0x1.277028p0,
  },
  { // Entry 467
    0x1.d42ded56ae88a6e1cf270af27e6f1804p-1,
    0x1.9e7f5cf075d1ec4ef69c9c67b62c27cbp-2,
    0x1.277028p0,
  },
  { // Entry 468
    -0x1.904c45326d6dde224381d1d590ada41cp-1,
    0x1.3f39fcc017653d2636837a55fdf6d2d4p-1,
    -0x1.cb920ap-1,
  },
  { // Entry 469
    0x1.904c45326d6dde224381d1d590ada41cp-1,
    0x1.3f39fcc017653d2636837a55fdf6d2d4p-1,
    0x1.cb920ap-1,
  },
  { // Entry 470
    -0x1.323b9cadbb19e75a44483fb64ad8ddf6p-1,
    0x1.9a52d5c700daa3dc8cf8f5a71f2df289p-1,
    -0x1.4843c4p-1,
  },
  { // Entry 471
    0x1.323b9cadbb19e75a44483fb64ad8ddf6p-1,
    0x1.9a52d5c700daa3dc8cf8f5a71f2df289p-1,
    0x1.4843c4p-1,
  },
  { // Entry 472
    -0x1.80462654bde766faf47f3140e290996dp-2,
    0x1.da94cd383dd7a3b91a2fc88ff905a6a0p-1,
    -0x1.89eafcp-2,
  },
  { // Entry 473
    0x1.80462654bde766faf47f3140e290996dp-2,
    0x1.da94cd383dd7a3b91a2fc88ff905a6a0p-1,
    0x1.89eafcp-2,
  },
  { // Entry 474
    -0x1.05e4ca21f386a82bc2e4efcdebb1962bp-3,
    0x1.fbcbe3de58e66c3283bc810d16c45833p-1,
    -0x1.069ce0p-3,
  },
  { // Entry 475
    0x1.05e4ca21f386a82bc2e4efcdebb1962bp-3,
    0x1.fbcbe3de58e66c3283bc810d16c45833p-1,
    0x1.069ce0p-3,
  },
  { // Entry 476
    0x1.05e423830be01f9fe3c57d06867e0056p-3,
    0x1.fbcbe93d48563d51b6e9d6efdb62495cp-1,
    0x1.069c38p-3,
  },
  { // Entry 477
    -0x1.05e423830be01f9fe3c57d06867e0056p-3,
    0x1.fbcbe93d48563d51b6e9d6efdb62495cp-1,
    -0x1.069c38p-3,
  },
  { // Entry 478
    0x1.8045d87852f1307fea6dc751c4d15992p-2,
    0x1.da94dcfb1cd15853ce848ffb0264ad08p-1,
    0x1.89eaa8p-2,
  },
  { // Entry 479
    -0x1.8045d87852f1307fea6dc751c4d15992p-2,
    0x1.da94dcfb1cd15853ce848ffb0264ad08p-1,
    -0x1.89eaa8p-2,
  },
  { // Entry 480
    0x1.323b7b04ee88cff98b2a1620e1f61a01p-1,
    0x1.9a52eee5e35377d554ace881bdc4725bp-1,
    0x1.48439ap-1,
  },
  { // Entry 481
    -0x1.323b7b04ee88cff98b2a1620e1f61a01p-1,
    0x1.9a52eee5e35377d554ace881bdc4725bp-1,
    -0x1.48439ap-1,
  },
  { // Entry 482
    0x1.904c2b02aa59528ce044bf2213c96859p-1,
    0x1.3f3a1d9657ff6aa498c46f6faaf03b90p-1,
    0x1.cb91e0p-1,
  },
  { // Entry 483
    -0x1.904c2b02aa59528ce044bf2213c96859p-1,
    0x1.3f3a1d9657ff6aa498c46f6faaf03b90p-1,
    -0x1.cb91e0p-1,
  },
  { // Entry 484
    0x1.d42ddd25b3797e6a679f76e05e6c3e08p-1,
    0x1.9e7fa617a1a3a400a7f59aa879088e31p-2,
    0x1.277014p0,
  },
  { // Entry 485
    -0x1.d42ddd25b3797e6a679f76e05e6c3e08p-1,
    0x1.9e7fa617a1a3a400a7f59aa879088e31p-2,
    -0x1.277014p0,
  },
  { // Entry 486
    0x1.f96fe1a0b12d0ad4fa8c82d8af989c5ap-1,
    0x1.46dc8a919b27840cda6e18a079da459cp-3,
    0x1.691738p0,
  },
  { // Entry 487
    -0x1.f96fe1a0b12d0ad4fa8c82d8af989c5ap-1,
    0x1.46dc8a919b27840cda6e18a079da459cp-3,
    -0x1.691738p0,
  },
  { // Entry 488
    0x1.fda255f96094d8fe4e859c4cf0dd68a5p-1,
    -0x1.894f0c0872415663b7f9e4e4801deaf0p-4,
    0x1.aabe5cp0,
  },
  { // Entry 489
    -0x1.fda255f96094d8fe4e859c4cf0dd68a5p-1,
    -0x1.894f0c0872415663b7f9e4e4801deaf0p-4,
    -0x1.aabe5cp0,
  },
  { // Entry 490
    0x1.e07ef267748b982778f8d50d2981bb3ap-1,
    -0x1.61a74d29774ae4e3bc5533a2ea08a14ap-2,
    0x1.ec6580p0,
  },
  { // Entry 491
    -0x1.e07ef267748b982778f8d50d2981bb3ap-1,
    -0x1.61a74d29774ae4e3bc5533a2ea08a14ap-2,
    -0x1.ec6580p0,
  },
  { // Entry 492
    0x1.a3eda2adb01143fb21453b20bd1748fep-1,
    -0x1.24ec7311bd7b2255f9b890b3ff5899f4p-1,
    0x1.170652p1,
  },
  { // Entry 493
    -0x1.a3eda2adb01143fb21453b20bd1748fep-1,
    -0x1.24ec7311bd7b2255f9b890b3ff5899f4p-1,
    -0x1.170652p1,
  },
  { // Entry 494
    0x1.4be49bd88a64a0bb414ddacac4fa8de9p-1,
    -0x1.85dc3b06c435f524c873d9b5eba3def8p-1,
    0x1.37d9e4p1,
  },
  { // Entry 495
    -0x1.4be49bd88a64a0bb414ddacac4fa8de9p-1,
    -0x1.85dc3b06c435f524c873d9b5eba3def8p-1,
    -0x1.37d9e4p1,
  },
  { // Entry 496
    0x1.bc4c0a9b3782e220ae55786369ccf190p-2,
    -0x1.cd4bc93947e86671ac7f0eacd9521377p-1,
    0x1.58ad76p1,
  },
  { // Entry 497
    -0x1.bc4c0a9b3782e220ae55786369ccf190p-2,
    -0x1.cd4bc93947e86671ac7f0eacd9521377p-1,
    -0x1.58ad76p1,
  },
  { // Entry 498
    0x1.877e301f43cafffe6644a8958f108729p-3,
    -0x1.f68eebd3b8f12f9433e6d7224989c10ep-1,
    0x1.798108p1,
  },
  { // Entry 499
    -0x1.877e301f43cafffe6644a8958f108729p-3,
    -0x1.f68eebd3b8f12f9433e6d7224989c10ep-1,
    -0x1.798108p1,
  },
  { // Entry 500
    -0x1.066e8ae1f824a69817e6a806e6317e28p-4,
    -0x1.fef2b2b91e40021a2fee74fc61812157p-1,
    0x1.9a549ap1,
  },
  { // Entry 501
    0x1.066e8ae1f824a69817e6a806e6317e28p-4,
    -0x1.fef2b2b91e40021a2fee74fc61812157p-1,
    -0x1.9a549ap1,
  },
  { // Entry 502
    -0x1.42abc1eca11a0ad12ca6eeff197318aap-2,
    -0x1.e5eaa14d86168b69918c22f3716a67eap-1,
    0x1.bb282cp1,
  },
  { // Entry 503
    0x1.42abc1eca11a0ad12ca6eeff197318aap-2,
    -0x1.e5eaa14d86168b69918c22f3716a67eap-1,
    -0x1.bb282cp1,
  },
  { // Entry 504
    -0x1.17505efb8119773c647468be1dfee45ep-1,
    -0x1.ad19deead0eae2f72d04165e09e4a43dp-1,
    0x1.dbfbbep1,
  },
  { // Entry 505
    0x1.17505efb8119773c647468be1dfee45ep-1,
    -0x1.ad19deead0eae2f72d04165e09e4a43dp-1,
    -0x1.dbfbbep1,
  },
  { // Entry 506
    -0x1.7b05bd8091cd79dff359c8412b0de1a9p-1,
    -0x1.5837cc21dda44f3ab7fd96f57c014e19p-1,
    0x1.fccf50p1,
  },
  { // Entry 507
    0x1.7b05bd8091cd79dff359c8412b0de1a9p-1,
    -0x1.5837cc21dda44f3ab7fd96f57c014e19p-1,
    -0x1.fccf50p1,
  },
  { // Entry 508
    -0x1.c5f05982eabf022748960961666d540dp-1,
    -0x1.d9a39df207139f99ebe9b56dafb234b7p-2,
    0x1.0ed170p2,
  },
  { // Entry 509
    0x1.c5f05982eabf022748960961666d540dp-1,
    -0x1.d9a39df207139f99ebe9b56dafb234b7p-2,
    -0x1.0ed170p2,
  },
  { // Entry 510
    -0x1.f329bfbda8122f83e3a1ea0242eb76aap-1,
    -0x1.c7b9189638128bc0ae33fdf2729cc987p-3,
    0x1.1f3b38p2,
  },
  { // Entry 511
    0x1.f329bfbda8122f83e3a1ea0242eb76aap-1,
    -0x1.c7b9189638128bc0ae33fdf2729cc987p-3,
    -0x1.1f3b38p2,
  },
  { // Entry 512
    -0x1.ffbca88ae90f0900b6d3ad89eddd2c80p-1,
    0x1.069082e04b25e9d2ea9e263b50d08b34p-5,
    0x1.2fa5p2,
  },
  { // Entry 513
    0x1.ffbca88ae90f0900b6d3ad89eddd2c80p-1,
    0x1.069082e04b25e9d2ea9e263b50d08b34p-5,
    -0x1.2fa5p2,
  },
  { // Entry 514
    -0x1.ead687409c95dcaf61af98513517f507p-1,
    0x1.235b1861f21aa86dce259e4e5b4ef395p-2,
    0x1.400ec8p2,
  },
  { // Entry 515
    0x1.ead687409c95dcaf61af98513517f507p-1,
    0x1.235b1861f21aa86dce259e4e5b4ef395p-2,
    -0x1.400ec8p2,
  },
  { // Entry 516
    -0x1.b5d54fd79372b90d5d4c7acf7adaed42p-1,
    0x1.096aaf70341485062f443c80a90a3be3p-1,
    0x1.507890p2,
  },
  { // Entry 517
    0x1.b5d54fd79372b90d5d4c7acf7adaed42p-1,
    0x1.096aaf70341485062f443c80a90a3be3p-1,
    -0x1.507890p2,
  },
  { // Entry 518
    -0x1.643092f42ae797375531420c005ca2cfp-1,
    0x1.6fcb6ae03107be458d07361371efabb4p-1,
    0x1.60e258p2,
  },
  { // Entry 519
    0x1.643092f42ae797375531420c005ca2cfp-1,
    0x1.6fcb6ae03107be458d07361371efabb4p-1,
    -0x1.60e258p2,
  },
  { // Entry 520
    -0x1.f67edf3b7bee8554d54d84ea83f6cb21p-2,
    0x1.be1d6d10d5c8ceeb8bf9aeb7a9f690b9p-1,
    0x1.714c20p2,
  },
  { // Entry 521
    0x1.f67edf3b7bee8554d54d84ea83f6cb21p-2,
    0x1.be1d6d10d5c8ceeb8bf9aeb7a9f690b9p-1,
    -0x1.714c20p2,
  },
  { // Entry 522
    -0x1.03be4d93d949325340b2f464201545a7p-2,
    0x1.ef413c72d988bb53937975e4fd4fcc7ap-1,
    0x1.81b5e8p2,
  },
  { // Entry 523
    0x1.03be4d93d949325340b2f464201545a7p-2,
    0x1.ef413c72d988bb53937975e4fd4fcc7ap-1,
    -0x1.81b5e8p2,
  },
  { // Entry 524
    0x1.efb26cfa20f2098ff7e9e42f0260eb01p-5,
    0x1.ff0fd2cb5a9228cfa1e01605d0626c84p-1,
    0x1.effffep-5,
  },
  { // Entry 525
    -0x1.efb26cfa20f2098ff7e9e42f0260eb01p-5,
    0x1.ff0fd2cb5a9228cfa1e01605d0626c84p-1,
    -0x1.effffep-5,
  },
  { // Entry 526
    0x1.efb26ef930c4d3f2b0dbe1931ba5ae64p-5,
    0x1.ff0fd2c96adfbad5f904a71b2d210a2ap-1,
    0x1.f0p-5,
  },
  { // Entry 527
    -0x1.efb26ef930c4d3f2b0dbe1931ba5ae64p-5,
    0x1.ff0fd2c96adfbad5f904a71b2d210a2ap-1,
    -0x1.f0p-5,
  },
  { // Entry 528
    0x1.efb270f840979c65b75ee5c67016a866p-5,
    0x1.ff0fd2c77b2d4add40566ec5aa24fc6ep-1,
    0x1.f00002p-5,
  },
  { // Entry 529
    -0x1.efb270f840979c65b75ee5c67016a866p-5,
    0x1.ff0fd2c77b2d4add40566ec5aa24fc6ep-1,
    -0x1.f00002p-5,
  },
  { // Entry 530
    0x1.f6baa816fce5ea5a60d8c9fd2a289380p-4,
    0x1.fc21005d216a89de55b192096fc6b7bap-1,
    0x1.f7fffep-4,
  },
  { // Entry 531
    -0x1.f6baa816fce5ea5a60d8c9fd2a289380p-4,
    0x1.fc21005d216a89de55b192096fc6b7bap-1,
    -0x1.f7fffep-4,
  },
  { // Entry 532
    0x1.f6baaa131de6438e5611279864fe7663p-4,
    0x1.fc210055467fe58a20193399b3bc0dd2p-1,
    0x1.f8p-4,
  },
  { // Entry 533
    -0x1.f6baaa131de6438e5611279864fe7663p-4,
    0x1.fc210055467fe58a20193399b3bc0dd2p-1,
    -0x1.f8p-4,
  },
  { // Entry 534
    0x1.f6baac0f3ee694e760a138bc06c8be3dp-4,
    0x1.fc21004d6b953945667f800ff81de0ebp-1,
    0x1.f80002p-4,
  },
  { // Entry 535
    -0x1.f6baac0f3ee694e760a138bc06c8be3dp-4,
    0x1.fc21004d6b953945667f800ff81de0ebp-1,
    -0x1.f80002p-4,
  },
  { // Entry 536
    0x1.4a8c395552fb432af31780e883c98f71p-3,
    0x1.f94984c6fdf1be6168509ff1e35f62dep-1,
    0x1.4bfffep-3,
  },
  { // Entry 537
    -0x1.4a8c395552fb432af31780e883c98f71p-3,
    0x1.f94984c6fdf1be6168509ff1e35f62dep-1,
    -0x1.4bfffep-3,
  },
  { // Entry 538
    0x1.4a8c3b4e9c7fffd48305f44a42f5f50fp-3,
    0x1.f94984b2552e1941ec766c6a82ece4a3p-1,
    0x1.4cp-3,
  },
  { // Entry 539
    -0x1.4a8c3b4e9c7fffd48305f44a42f5f50fp-3,
    0x1.f94984b2552e1941ec766c6a82ece4a3p-1,
    -0x1.4cp-3,
  },
  { // Entry 540
    0x1.4a8c3d47e604a7d54f3f7de402409e2cp-3,
    0x1.f949849dac6a548dd851139041106316p-1,
    0x1.4c0002p-3,
  },
  { // Entry 541
    -0x1.4a8c3d47e604a7d54f3f7de402409e2cp-3,
    0x1.f949849dac6a548dd851139041106316p-1,
    -0x1.4c0002p-3,
  },
  { // Entry 542
    0x1.2e9cd83630eb35c12efcfb8413583998p-2,
    0x1.e921dd7054ef5d4f727d938ce10a053cp-1,
    0x1.333332p-2,
  },
  { // Entry 543
    -0x1.2e9cd83630eb35c12efcfb8413583998p-2,
    0x1.e921dd7054ef5d4f727d938ce10a053cp-1,
    -0x1.333332p-2,
  },
  { // Entry 544
    0x1.2e9cda1f52c88042833f236ff0f9d486p-2,
    0x1.e921dd24adb9129efc053f9acd4d2444p-1,
    0x1.333334p-2,
  },
  { // Entry 545
    -0x1.2e9cda1f52c88042833f236ff0f9d486p-2,
    0x1.e921dd24adb9129efc053f9acd4d2444p-1,
    -0x1.333334p-2,
  },
  { // Entry 546
    0x1.2e9cdc0874a57f1ca0f976a9b01e4a71p-2,
    0x1.e921dcd906824da60e43c03a7774b171p-1,
    0x1.333336p-2,
  },
  { // Entry 547
    -0x1.2e9cdc0874a57f1ca0f976a9b01e4a71p-2,
    0x1.e921dcd906824da60e43c03a7774b171p-1,
    -0x1.333336p-2,
  },
  { // Entry 548
    0x1.3faefb2b68e6786eb692bd4e4045213ep-1,
    0x1.8feedc92764bfbdcb41389e82063ed6ep-1,
    0x1.594316p-1,
  },
  { // Entry 549
    -0x1.3faefb2b68e6786eb692bd4e4045213ep-1,
    0x1.8feedc92764bfbdcb41389e82063ed6ep-1,
    -0x1.594316p-1,
  },
  { // Entry 550
    0x1.3faefcbb57c26b0d84b63dbfb72b413bp-1,
    0x1.8feedb52c750087c5f8727e0279e5f66p-1,
    0x1.594318p-1,
  },
  { // Entry 551
    -0x1.3faefcbb57c26b0d84b63dbfb72b413bp-1,
    0x1.8feedb52c750087c5f8727e0279e5f66p-1,
    -0x1.594318p-1,
  },
  { // Entry 552
    0x1.3faefe4b469d1dfd561e666edda7c6e6p-1,
    0x1.8feeda131852852d2fa7fe8847b05973p-1,
    0x1.59431ap-1,
  },
  { // Entry 553
    -0x1.3faefe4b469d1dfd561e666edda7c6e6p-1,
    0x1.8feeda131852852d2fa7fe8847b05973p-1,
    -0x1.59431ap-1,
  },
  { // Entry 554
    0x1.6888a375ab228c1e031c4005769509f9p-1,
    0x1.6b8991127859fd9b43ca1d08b92aa401p-1,
    0x1.8ffffep-1,
  },
  { // Entry 555
    -0x1.6888a375ab228c1e031c4005769509f9p-1,
    0x1.6b8991127859fd9b43ca1d08b92aa401p-1,
    -0x1.8ffffep-1,
  },
  { // Entry 556
    0x1.6888a4e134b2ea520b226eca8694b3a2p-1,
    0x1.6b898fa9efb5d22b58f0d99e9634931ap-1,
    0x1.90p-1,
  },
  { // Entry 557
    -0x1.6888a4e134b2ea520b226eca8694b3a2p-1,
    0x1.6b898fa9efb5d22b58f0d99e9634931ap-1,
    -0x1.90p-1,
  },
  { // Entry 558
    0x1.6888a64cbe41dffd6e4768dcca4db53bp-1,
    0x1.6b898e4167103b31de6da67ebf5e9fe6p-1,
    0x1.900002p-1,
  },
  { // Entry 559
    -0x1.6888a64cbe41dffd6e4768dcca4db53bp-1,
    0x1.6b898e4167103b31de6da67ebf5e9fe6p-1,
    -0x1.900002p-1,
  },
  { // Entry 560
    -0.0f,
    0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.p-149,
  },
  { // Entry 561
    0.0f,
    0x1.ffffffffffffffffffffffffffffffffp-1,
    0x1.p-149,
  },
  { // Entry 562
    0.0,
    0x1.p0,
    0.0,
  },
  { // Entry 563
    0.0f,
    0x1.ffffffffffffffffffffffffffffffffp-1,
    0x1.p-149,
  },
  { // Entry 564
    -0.0f,
    0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.p-149,
  },
  { // Entry 565
    0x1.91f65dccfead353d8db9c32f12262730p-5,
    0x1.ff621e38956a3b3be920256ddb6034cdp-1,
    0x1.921fb4p-5,
  },
  { // Entry 566
    -0x1.91f65dccfead353d8db9c32f12262730p-5,
    0x1.ff621e38956a3b3be920256ddb6034cdp-1,
    -0x1.921fb4p-5,
  },
  { // Entry 567
    0x1.91f65fcc60cb6d09fcc5c35dd6a798c8p-5,
    0x1.ff621e370373dc6f3963d42896ede078p-1,
    0x1.921fb6p-5,
  },
  { // Entry 568
    -0x1.91f65fcc60cb6d09fcc5c35dd6a798c8p-5,
    0x1.ff621e370373dc6f3963d42896ede078p-1,
    -0x1.921fb6p-5,
  },
  { // Entry 569
    0x1.91f661cbc2e9a3447571f72bcfbc21e2p-5,
    0x1.ff621e35717d7ba327894bdfde9f4787p-1,
    0x1.921fb8p-5,
  },
  { // Entry 570
    -0x1.91f661cbc2e9a3447571f72bcfbc21e2p-5,
    0x1.ff621e35717d7ba327894bdfde9f4787p-1,
    -0x1.921fb8p-5,
  },
  { // Entry 571
    0x1.917a6a7fe8297bf0a1125fb02b2038aep-4,
    0x1.fd88da410b61cd55221d9beb996d4d99p-1,
    0x1.921fb4p-4,
  },
  { // Entry 572
    -0x1.917a6a7fe8297bf0a1125fb02b2038aep-4,
    0x1.fd88da410b61cd55221d9beb996d4d99p-1,
    -0x1.921fb4p-4,
  },
  { // Entry 573
    0x1.917a6c7d7103b9d90e09615164449c6bp-4,
    0x1.fd88da3ac5781f5a6fc32e40ed5122b0p-1,
    0x1.921fb6p-4,
  },
  { // Entry 574
    -0x1.917a6c7d7103b9d90e09615164449c6bp-4,
    0x1.fd88da3ac5781f5a6fc32e40ed5122b0p-1,
    -0x1.921fb6p-4,
  },
  { // Entry 575
    0x1.917a6e7af9ddf17b914e6d2e8e83b33ep-4,
    0x1.fd88da347f8e696999ffd58060ba3569p-1,
    0x1.921fb8p-4,
  },
  { // Entry 576
    -0x1.917a6e7af9ddf17b914e6d2e8e83b33ep-4,
    0x1.fd88da347f8e696999ffd58060ba3569p-1,
    -0x1.921fb8p-4,
  },
  { // Entry 577
    0x1.8f8b82889296b5cf7904db1e74b3466bp-3,
    0x1.f6297d0f4671da580dfecdd4db29f473p-1,
    0x1.921fb4p-3,
  },
  { // Entry 578
    -0x1.8f8b82889296b5cf7904db1e74b3466bp-3,
    0x1.f6297d0f4671da580dfecdd4db29f473p-1,
    -0x1.921fb4p-3,
  },
  { // Entry 579
    0x1.8f8b847ebc13b8998ec5b37e7065341ep-3,
    0x1.f6297cf64db9a21d98ab3940fc8a86f0p-1,
    0x1.921fb6p-3,
  },
  { // Entry 580
    -0x1.8f8b847ebc13b8998ec5b37e7065341ep-3,
    0x1.f6297cf64db9a21d98ab3940fc8a86f0p-1,
    -0x1.921fb6p-3,
  },
  { // Entry 581
    0x1.8f8b8674e590a26aec3ea01d30aed486p-3,
    0x1.f6297cdd55014a808b883fd183f318acp-1,
    0x1.921fb8p-3,
  },
  { // Entry 582
    -0x1.8f8b8674e590a26aec3ea01d30aed486p-3,
    0x1.f6297cdd55014a808b883fd183f318acp-1,
    -0x1.921fb8p-3,
  },
  { // Entry 583
    0x1.87de293f569717a42a3bdb01aeae2063p-2,
    0x1.d906bd313443007dcb0bd4e3d63284c0p-1,
    0x1.921fb4p-2,
  },
  { // Entry 584
    -0x1.87de293f569717a42a3bdb01aeae2063p-2,
    0x1.d906bd313443007dcb0bd4e3d63284c0p-1,
    -0x1.921fb4p-2,
  },
  { // Entry 585
    0x1.87de2b185d5417dca800b85ca1319043p-2,
    0x1.d906bccf3cb875874da3da4c01104bafp-1,
    0x1.921fb6p-2,
  },
  { // Entry 586
    -0x1.87de2b185d5417dca800b85ca1319043p-2,
    0x1.d906bccf3cb875874da3da4c01104bafp-1,
    -0x1.921fb6p-2,
  },
  { // Entry 587
    0x1.87de2cf16410b61d9aff7e628fc853b2p-2,
    0x1.d906bc6d452d744f210810861102f2dap-1,
    0x1.921fb8p-2,
  },
  { // Entry 588
    -0x1.87de2cf16410b61d9aff7e628fc853b2p-2,
    0x1.d906bc6d452d744f210810861102f2dap-1,
    -0x1.921fb8p-2,
  },
  { // Entry 589
    0x1.6a09e582aa3945461b5a8a0787d5ab5bp-1,
    0x1.6a09e74d3d3fbb94d94274f37769f6eap-1,
    0x1.921fb4p-1,
  },
  { // Entry 590
    -0x1.6a09e582aa3945461b5a8a0787d5ab5bp-1,
    0x1.6a09e74d3d3fbb94d94274f37769f6eap-1,
    -0x1.921fb4p-1,
  },
  { // Entry 591
    0x1.6a09e6ecb41fdd7e681872c854887019p-1,
    0x1.6a09e5e3335983e5ac92e733e3f24b42p-1,
    0x1.921fb6p-1,
  },
  { // Entry 592
    -0x1.6a09e6ecb41fdd7e681872c854887019p-1,
    0x1.6a09e5e3335983e5ac92e733e3f24b42p-1,
    -0x1.921fb6p-1,
  },
  { // Entry 593
    0x1.6a09e856be050baccde9a76961e84aa7p-1,
    0x1.6a09e4792971e22c9a00261aeac070dap-1,
    0x1.921fb8p-1,
  },
  { // Entry 594
    -0x1.6a09e856be050baccde9a76961e84aa7p-1,
    0x1.6a09e4792971e22c9a00261aeac070dap-1,
    -0x1.921fb8p-1,
  },
  { // Entry 595
    0x1.fffffffffffe6546cc38211c26dabeebp-1,
    0x1.4442d18469893610281a0f9b0e8d0eefp-24,
    0x1.921fb4p0,
  },
  { // Entry 596
    -0x1.fffffffffffe6546cc38211c26dabeebp-1,
    0x1.4442d18469893610281a0f9b0e8d0eefp-24,
    -0x1.921fb4p0,
  },
  { // Entry 597
    0x1.ffffffffffff76521249c7422930ed82p-1,
    -0x1.777a5cf72cecc4cde3a31e7d5a026142p-25,
    0x1.921fb6p0,
  },
  { // Entry 598
    -0x1.ffffffffffff76521249c7422930ed82p-1,
    -0x1.777a5cf72cecc4cde3a31e7d5a026142p-25,
    -0x1.921fb6p0,
  },
  { // Entry 599
    0x1.fffffffffff8875d585b6d6cfce97d9cp-1,
    -0x1.5dde973dcb3985f4a8e76a1feca29e1dp-23,
    0x1.921fb8p0,
  },
  { // Entry 600
    -0x1.fffffffffff8875d585b6d6cfce97d9cp-1,
    -0x1.5dde973dcb3985f4a8e76a1feca29e1dp-23,
    -0x1.921fb8p0,
  },
  { // Entry 601
    0x1.4442d184698831f15b6315bfa6b5ae75p-23,
    -0x1.fffffffffff9951b30e084732e60bb85p-1,
    0x1.921fb4p1,
  },
  { // Entry 602
    -0x1.4442d184698831f15b6315bfa6b5ae75p-23,
    -0x1.fffffffffff9951b30e084732e60bb85p-1,
    -0x1.921fb4p1,
  },
  { // Entry 603
    -0x1.777a5cf72cec5fd61896cb4f40d1de79p-24,
    -0x1.fffffffffffdd94849271d08eecf54a1p-1,
    0x1.921fb6p1,
  },
  { // Entry 604
    0x1.777a5cf72cec5fd61896cb4f40d1de79p-24,
    -0x1.fffffffffffdd94849271d08eecf54a1p-1,
    -0x1.921fb6p1,
  },
  { // Entry 605
    -0x1.5dde973dcb346afa46203cddc6f7fe97p-22,
    -0x1.ffffffffffe21d75616db5ebc56405f5p-1,
    0x1.921fb8p1,
  },
  { // Entry 606
    0x1.5dde973dcb346afa46203cddc6f7fe97p-22,
    -0x1.ffffffffffe21d75616db5ebc56405f5p-1,
    -0x1.921fb8p1,
  },
  { // Entry 607
    -0x1.4442d1846984217628872e56eb58b4c1p-22,
    0x1.ffffffffffe6546cc38211f5e8deeb97p-1,
    0x1.921fb4p2,
  },
  { // Entry 608
    0x1.4442d1846984217628872e56eb58b4c1p-22,
    0x1.ffffffffffe6546cc38211f5e8deeb97p-1,
    -0x1.921fb4p2,
  },
  { // Entry 609
    0x1.777a5cf72ceacbf6ec657e977ef771f1p-23,
    0x1.fffffffffff76521249c74285bf73c07p-1,
    0x1.921fb6p2,
  },
  { // Entry 610
    -0x1.777a5cf72ceacbf6ec657e977ef771f1p-23,
    0x1.fffffffffff76521249c74285bf73c07p-1,
    -0x1.921fb6p2,
  },
  { // Entry 611
    0x1.5dde973dcb1fff10bb0388479e82f4bbp-21,
    0x1.ffffffffff8875d585b6db2c31711004p-1,
    0x1.921fb8p2,
  },
  { // Entry 612
    -0x1.5dde973dcb1fff10bb0388479e82f4bbp-21,
    0x1.ffffffffff8875d585b6db2c31711004p-1,
    -0x1.921fb8p2,
  },
  { // Entry 613
    -0x1.4442d1846973df895d1791023ded513cp-21,
    0x1.ffffffffff9951b30e084a6a993b8675p-1,
    0x1.921fb4p3,
  },
  { // Entry 614
    0x1.4442d1846973df895d1791023ded513cp-21,
    0x1.ffffffffff9951b30e084a6a993b8675p-1,
    -0x1.921fb4p3,
  },
  { // Entry 615
    0x1.777a5cf72ce47c7a3ba04bc2a607a9a7p-22,
    0x1.ffffffffffdd94849271d0eb7b7b884bp-1,
    0x1.921fb6p3,
  },
  { // Entry 616
    -0x1.777a5cf72ce47c7a3ba04bc2a607a9a7p-22,
    0x1.ffffffffffdd94849271d0eb7b7b884bp-1,
    -0x1.921fb6p3,
  },
  { // Entry 617
    0x1.5dde973dcace4f6a8e90bd15e00610f8p-20,
    0x1.fffffffffe21d75616dba48283d3c2f7p-1,
    0x1.921fb8p3,
  },
  { // Entry 618
    -0x1.5dde973dcace4f6a8e90bd15e00610f8p-20,
    0x1.fffffffffe21d75616dba48283d3c2f7p-1,
    -0x1.921fb8p3,
  },
  { // Entry 619
    -0x1.4442d1846932d7d62f59209388c7f7cap-20,
    0x1.fffffffffe6546cc382152d9c0eb9b47p-1,
    0x1.921fb4p4,
  },
  { // Entry 620
    0x1.4442d1846932d7d62f59209388c7f7cap-20,
    0x1.fffffffffe6546cc382152d9c0eb9b47p-1,
    -0x1.921fb4p4,
  },
  { // Entry 621
    0x1.777a5cf72ccb3e87788b811229e725bcp-21,
    0x1.ffffffffff76521249c7484ea7d7a409p-1,
    0x1.921fb6p4,
  },
  { // Entry 622
    -0x1.777a5cf72ccb3e87788b811229e725bcp-21,
    0x1.ffffffffff76521249c7484ea7d7a409p-1,
    -0x1.921fb6p4,
  },
  { // Entry 623
    0x1.5dde973dc98790d1dcc602bd1b86bccap-19,
    0x1.fffffffff8875d585b720f25f0473943p-1,
    0x1.921fb8p4,
  },
  { // Entry 624
    -0x1.5dde973dc98790d1dcc602bd1b86bccap-19,
    0x1.fffffffff8875d585b720f25f0473943p-1,
    -0x1.921fb8p4,
  },
  { // Entry 625
    -0x1.4442d184682eb909785fad18bcb5dbfcp-19,
    0x1.fffffffff9951b30e087de5cc38683b8p-1,
    0x1.921fb4p5,
  },
  { // Entry 626
    0x1.4442d184682eb909785fad18bcb5dbfcp-19,
    0x1.fffffffff9951b30e087de5cc38683b8p-1,
    -0x1.921fb4p5,
  },
  { // Entry 627
    0x1.777a5cf72c6646bc6c38607eb34eea13p-20,
    0x1.fffffffffdd94849271d6b463df6bddfp-1,
    0x1.921fb6p5,
  },
  { // Entry 628
    -0x1.777a5cf72c6646bc6c38607eb34eea13p-20,
    0x1.fffffffffdd94849271d6b463df6bddfp-1,
    -0x1.921fb6p5,
  },
  { // Entry 629
    0x1.5dde973dc46c966f15a2403d60cd14d0p-18,
    0x1.ffffffffe21d75616e000e55d09f8757p-1,
    0x1.921fb8p5,
  },
  { // Entry 630
    -0x1.5dde973dc46c966f15a2403d60cd14d0p-18,
    0x1.ffffffffe21d75616e000e55d09f8757p-1,
    -0x1.921fb8p5,
  },
  { // Entry 631
    -0x1.4442d184641e3dd69c7ec32e14a209a5p-18,
    0x1.ffffffffe6546cc38248a8cf0b9b5795p-1,
    0x1.921fb4p6,
  },
  { // Entry 632
    0x1.4442d184641e3dd69c7ec32e14a209a5p-18,
    0x1.ffffffffe6546cc38248a8cf0b9b5795p-1,
    -0x1.921fb4p6,
  },
  { // Entry 633
    0x1.777a5cf72ad267903aec8118778b3b5ap-19,
    0x1.fffffffff76521249c7a4dd2e15dd1c4p-1,
    0x1.921fb6p6,
  },
  { // Entry 634
    -0x1.777a5cf72ad267903aec8118778b3b5ap-19,
    0x1.fffffffff76521249c7a4dd2e15dd1c4p-1,
    -0x1.921fb6p6,
  },
  { // Entry 635
    0x1.5dde973db000ace3f985a473ea1fc039p-17,
    0x1.ffffffff8875d585bb7d55383a9b39a4p-1,
    0x1.921fb8p6,
  },
  { // Entry 636
    -0x1.5dde973db000ace3f985a473ea1fc039p-17,
    0x1.ffffffff8875d585bb7d55383a9b39a4p-1,
    -0x1.921fb8p6,
  },
  { // Entry 637
    -0x1.4442d18453dc510b2d495b8bf79bd1cep-17,
    0x1.ffffffff9951b30e0bb598fc0679a6f7p-1,
    0x1.921fb4p7,
  },
  { // Entry 638
    0x1.4442d18453dc510b2d495b8bf79bd1cep-17,
    0x1.ffffffff9951b30e0bb598fc0679a6f7p-1,
    -0x1.921fb4p7,
  },
  { // Entry 639
    0x1.777a5cf72482eadf75c731f972507718p-18,
    0x1.ffffffffdd948492723342ea1da49bacp-1,
    0x1.921fb6p7,
  },
  { // Entry 640
    -0x1.777a5cf72482eadf75c731f972507718p-18,
    0x1.ffffffffdd948492723342ea1da49bacp-1,
    -0x1.921fb6p7,
  },
  { // Entry 641
    0x1.5dde973d5e5106b7903a18a552aefc6ep-16,
    0x1.fffffffe21d7561725c712f068fc9718p-1,
    0x1.921fb8p7,
  },
  { // Entry 642
    -0x1.5dde973d5e5106b7903a18a552aefc6ep-16,
    0x1.fffffffe21d7561725c712f068fc9718p-1,
    -0x1.921fb8p7,
  },
  { // Entry 643
    0x1.6a09f1940b80c8e25cfc8c10d42576c6p-1,
    -0x1.6a09db3bdba0868a31e766359a8406cap-1,
    0x1.2d97c4p1,
  },
  { // Entry 644
    -0x1.6a09f1940b80c8e25cfc8c10d42576c6p-1,
    -0x1.6a09db3bdba0868a31e766359a8406cap-1,
    -0x1.2d97c4p1,
  },
  { // Entry 645
    0x1.6a09ebebe40889245d57c5c9d90a6d82p-1,
    -0x1.6a09e0e4035b86694c16534e42fbe111p-1,
    0x1.2d97c6p1,
  },
  { // Entry 646
    -0x1.6a09ebebe40889245d57c5c9d90a6d82p-1,
    -0x1.6a09e0e4035b86694c16534e42fbe111p-1,
    -0x1.2d97c6p1,
  },
  { // Entry 647
    0x1.6a09e643bc79a8c79ef4bf187727e269p-1,
    -0x1.6a09e68c2affe5aa58050accb05c6248p-1,
    0x1.2d97c8p1,
  },
  { // Entry 648
    -0x1.6a09e643bc79a8c79ef4bf187727e269p-1,
    -0x1.6a09e68c2affe5aa58050accb05c6248p-1,
    -0x1.2d97c8p1,
  },
  { // Entry 649
    -0x1.6a09df19704cf14108e09000ff6374bfp-1,
    -0x1.6a09edb67706e0997121d12a0c87bae8p-1,
    0x1.f6a7a0p1,
  },
  { // Entry 650
    0x1.6a09df19704cf14108e09000ff6374bfp-1,
    -0x1.6a09edb67706e0997121d12a0c87bae8p-1,
    -0x1.f6a7a0p1,
  },
  { // Entry 651
    -0x1.6a09e4c197f87ace1c81b43022be39b1p-1,
    -0x1.6a09e80e4f7f2a88debed37faa93e8c8p-1,
    0x1.f6a7a2p1,
  },
  { // Entry 652
    0x1.6a09e4c197f87ace1c81b43022be39b1p-1,
    -0x1.6a09e80e4f7f2a88debed37faa93e8c8p-1,
    -0x1.f6a7a2p1,
  },
  { // Entry 653
    -0x1.6a09ea69bf8d63bce40958f5c4b4f155p-1,
    -0x1.6a09e26627e0d3d9cb76de00cb902becp-1,
    0x1.f6a7a4p1,
  },
  { // Entry 654
    0x1.6a09ea69bf8d63bce40958f5c4b4f155p-1,
    -0x1.6a09e26627e0d3d9cb76de00cb902becp-1,
    -0x1.f6a7a4p1,
  },
  { // Entry 655
    -0x1.ffffffffff065cb240bb8f9519d2c6f1p-1,
    -0x1.f9990e91a74168b90bd68dfab775c9cap-21,
    0x1.2d97c4p2,
  },
  { // Entry 656
    0x1.ffffffffff065cb240bb8f9519d2c6f1p-1,
    -0x1.f9990e91a74168b90bd68dfab775c9cap-21,
    -0x1.2d97c4p2,
  },
  { // Entry 657
    -0x1.ffffffffffc32939898f464aafc2e74ap-1,
    -0x1.f3321d234f1363d187dd09528b67b215p-22,
    0x1.2d97c6p2,
  },
  { // Entry 658
    0x1.ffffffffffc32939898f464aafc2e74ap-1,
    -0x1.f3321d234f1363d187dd09528b67b215p-22,
    -0x1.2d97c6p2,
  },
  { // Entry 659
    -0x1.fffffffffffff5c0d2630ee0a1fb4e7bp-1,
    0x1.99bc5b961b1acaca18d971f68ae99da9p-27,
    0x1.2d97c8p2,
  },
  { // Entry 660
    0x1.fffffffffffff5c0d2630ee0a1fb4e7bp-1,
    0x1.99bc5b961b1acaca18d971f68ae99da9p-27,
    -0x1.2d97c8p2,
  },
  { // Entry 661
    -0x1.6a09f529316cde5a190d235cc4ccb825p-1,
    0x1.6a09d7a6b572c2c824d137d0405d8188p-1,
    0x1.5fdbbcp2,
  },
  { // Entry 662
    0x1.6a09f529316cde5a190d235cc4ccb825p-1,
    0x1.6a09d7a6b572c2c824d137d0405d8188p-1,
    -0x1.5fdbbcp2,
  },
  { // Entry 663
    -0x1.6a09e9d8e2826770567ea818b2e89960p-1,
    0x1.6a09e2f704eecb181e3f5ece9be0ca0fp-1,
    0x1.5fdbbep2,
  },
  { // Entry 664
    0x1.6a09e9d8e2826770567ea818b2e89960p-1,
    0x1.6a09e2f704eecb181e3f5ece9be0ca0fp-1,
    -0x1.5fdbbep2,
  },
  { // Entry 665
    -0x1.6a09de88933d6e0c1db78e1d7cd15173p-1,
    0x1.6a09ee47541050ef59ec4bfce935cc1ap-1,
    0x1.5fdbc0p2,
  },
  { // Entry 666
    0x1.6a09de88933d6e0c1db78e1d7cd15173p-1,
    0x1.6a09ee47541050ef59ec4bfce935cc1ap-1,
    -0x1.5fdbc0p2,
  },
  { // Entry 667
    0x1.6a09d033fa715a407a6f03d01b91113fp-1,
    0x1.6a09fc9bebaba208c81ec0b1cd307589p-1,
    0x1.c463a8p2,
  },
  { // Entry 668
    -0x1.6a09d033fa715a407a6f03d01b91113fp-1,
    0x1.6a09fc9bebaba208c81ec0b1cd307589p-1,
    -0x1.c463a8p2,
  },
  { // Entry 669
    0x1.6a09db844a28f8635851fdf8818515efp-1,
    0x1.6a09f14b9cfcc0f6227d386cc3704a05p-1,
    0x1.c463aap2,
  },
  { // Entry 670
    -0x1.6a09db844a28f8635851fdf8818515efp-1,
    0x1.6a09f14b9cfcc0f6227d386cc3704a05p-1,
    -0x1.c463aap2,
  },
  { // Entry 671
    0x1.6a09e6d49986140f55226fc58672612cp-1,
    0x1.6a09e5fb4df35d6729f472da3413e404p-1,
    0x1.c463acp2,
  },
  { // Entry 672
    -0x1.6a09e6d49986140f55226fc58672612cp-1,
    0x1.6a09e5fb4df35d6729f472da3413e404p-1,
    -0x1.c463acp2,
  },
  { // Entry 673
    0x1.ffffffffff95397934cac1f28532d3d3p-1,
    0x1.4aa9c2f2c1defb8728f0d2da1217aae1p-21,
    0x1.f6a7a0p2,
  },
  { // Entry 674
    -0x1.ffffffffff95397934cac1f28532d3d3p-1,
    0x1.4aa9c2f2c1defb8728f0d2da1217aae1p-21,
    -0x1.f6a7a0p2,
  },
  { // Entry 675
    0x1.fffffffffffa8e5aae2bb93ae590f984p-1,
    0x1.2aa70bcb07d6d0f36b777cb380a845d9p-23,
    0x1.f6a7a2p2,
  },
  { // Entry 676
    -0x1.fffffffffffa8e5aae2bb93ae590f984p-1,
    0x1.2aa70bcb07d6d0f36b777cb380a845d9p-23,
    -0x1.f6a7a2p2,
  },
  { // Entry 677
    0x1.ffffffffffdfe33c278cb48a59ee3ef2p-1,
    -0x1.6aac7a1a7c0c7afc5fcb2313a7eca229p-22,
    0x1.f6a7a4p2,
  },
  { // Entry 678
    -0x1.ffffffffffdfe33c278cb48a59ee3ef2p-1,
    -0x1.6aac7a1a7c0c7afc5fcb2313a7eca229p-22,
    -0x1.f6a7a4p2,
  },
  { // Entry 679
    0x1.6a0a040ea5c32ba4afbeb86a614c5d16p-1,
    -0x1.6a09c8c13f48b7aad851f9d6474bcb31p-1,
    0x1.1475cap3,
  },
  { // Entry 680
    -0x1.6a0a040ea5c32ba4afbeb86a614c5d16p-1,
    -0x1.6a09c8c13f48b7aad851f9d6474bcb31p-1,
    -0x1.1475cap3,
  },
  { // Entry 681
    0x1.6a09ed6e088212b1e260a5132d6959b7p-1,
    -0x1.6a09df61ded49d1ee4fca4ba6140d179p-1,
    0x1.1475ccp3,
  },
  { // Entry 682
    -0x1.6a09ed6e088212b1e260a5132d6959b7p-1,
    -0x1.6a09df61ded49d1ee4fca4ba6140d179p-1,
    -0x1.1475ccp3,
  },
  { // Entry 683
    0x1.6a09d6cd69d6efd1a6fa2dd4c617cbbep-1,
    -0x1.6a09f6027cf678b38fc8992cd9990302p-1,
    0x1.1475cep3,
  },
  { // Entry 684
    -0x1.6a09d6cd69d6efd1a6fa2dd4c617cbbep-1,
    -0x1.6a09f6027cf678b38fc8992cd9990302p-1,
    -0x1.1475cep3,
  },
  { // Entry 685
    0x1.f9990e91a64ae486757878bdfee0f703p-20,
    -0x1.fffffffffc1972c902ef31c37cb54817p-1,
    0x1.2d97c4p3,
  },
  { // Entry 686
    -0x1.f9990e91a64ae486757878bdfee0f703p-20,
    -0x1.fffffffffc1972c902ef31c37cb54817p-1,
    -0x1.2d97c4p3,
  },
  { // Entry 687
    0x1.f3321d234ed8128aabb0499a43b4def2p-21,
    -0x1.ffffffffff0ca4e6263d27a0204389dfp-1,
    0x1.2d97c6p3,
  },
  { // Entry 688
    -0x1.f3321d234ed8128aabb0499a43b4def2p-21,
    -0x1.ffffffffff0ca4e6263d27a0204389dfp-1,
    -0x1.2d97c6p3,
  },
  { // Entry 689
    -0x1.99bc5b961b1ac296dbe1980fd2c890a0p-26,
    -0x1.ffffffffffffd703498c3b8288563915p-1,
    0x1.2d97c8p3,
  },
  { // Entry 690
    0x1.99bc5b961b1ac296dbe1980fd2c890a0p-26,
    -0x1.ffffffffffffd703498c3b8288563915p-1,
    -0x1.2d97c8p3,
  },
  { // Entry 691
    -0x1.6a09c14e83f8db080d1223f887cc12ecp-1,
    -0x1.6a0a0b815fb37b2d01551e07cb3009d1p-1,
    0x1.46b9c0p3,
  },
  { // Entry 692
    0x1.6a09c14e83f8db080d1223f887cc12ecp-1,
    -0x1.6a0a0b815fb37b2d01551e07cb3009d1p-1,
    -0x1.46b9c0p3,
  },
  { // Entry 693
    -0x1.6a09d7ef23fbec1ed812e807beb0492fp-1,
    -0x1.6a09f4e0c2e98deb78642b6032a73d46p-1,
    0x1.46b9c2p3,
  },
  { // Entry 694
    0x1.6a09d7ef23fbec1ed812e807beb0492fp-1,
    -0x1.6a09f4e0c2e98deb78642b6032a73d46p-1,
    -0x1.46b9c2p3,
  },
  { // Entry 695
    -0x1.6a09ee8fc294f35db3efce565365af89p-1,
    -0x1.6a09de4024b596b50eb06d562db8c777p-1,
    0x1.46b9c4p3,
  },
  { // Entry 696
    0x1.6a09ee8fc294f35db3efce565365af89p-1,
    -0x1.6a09de4024b596b50eb06d562db8c777p-1,
    -0x1.46b9c4p3,
  },
  { // Entry 697
    -0x1.fffffffffe4c96b397d951cb21861c95p-1,
    -0x1.4ddd3ba9edcd898b9946fdd20af22a68p-20,
    0x1.5fdbbcp3,
  },
  { // Entry 698
    0x1.fffffffffe4c96b397d951cb21861c95p-1,
    -0x1.4ddd3ba9edcd898b9946fdd20af22a68p-20,
    -0x1.5fdbbcp3,
  },
  { // Entry 699
    -0x1.ffffffffffe8512aebb56c9e75b41941p-1,
    -0x1.3774eea7b8abe8fa8c380142b97af4b6p-22,
    0x1.5fdbbep3,
  },
  { // Entry 700
    0x1.ffffffffffe8512aebb56c9e75b41941p-1,
    -0x1.3774eea7b8abe8fa8c380142b97af4b6p-22,
    -0x1.5fdbbep3,
  },
  { // Entry 701
    -0x1.ffffffffff840ba23f91c9cb49a10b27p-1,
    0x1.644588ac238ae493fa32435ba51329bfp-21,
    0x1.5fdbc0p3,
  },
  { // Entry 702
    0x1.ffffffffff840ba23f91c9cb49a10b27p-1,
    0x1.644588ac238ae493fa32435ba51329bfp-21,
    -0x1.5fdbc0p3,
  },
  { // Entry 703
    -0x1.6a0a12f4197c90a0ee4a094b6377aa23p-1,
    0x1.6a09b9dbc881c458e747908caf2aa5e1p-1,
    0x1.78fdb6p3,
  },
  { // Entry 704
    0x1.6a0a12f4197c90a0ee4a094b6377aa23p-1,
    0x1.6a09b9dbc881c458e747908caf2aa5e1p-1,
    -0x1.78fdb6p3,
  },
  { // Entry 705
    -0x1.6a09fc537d29cf131d6710991bebabedp-1,
    0x1.6a09d07c68fc010ffcfd3b19f1ee4f44p-1,
    0x1.78fdb8p3,
  },
  { // Entry 706
    0x1.6a09fc537d29cf131d6710991bebabedp-1,
    0x1.6a09d07c68fc010ffcfd3b19f1ee4f44p-1,
    -0x1.78fdb8p3,
  },
  { // Entry 707
    -0x1.6a09e5b2df6d0388f9070c4340f3e669p-1,
    0x1.6a09e71d080c33f6964a07d1a0bf5980p-1,
    0x1.78fdbap3,
  },
  { // Entry 708
    0x1.6a09e5b2df6d0388f9070c4340f3e669p-1,
    0x1.6a09e71d080c33f6964a07d1a0bf5980p-1,
    -0x1.78fdbap3,
  },
  { // Entry 709
    0x1.6a09c909add4dbf32253a39d5c306308p-1,
    0x1.6a0a03c63742d62802d163d5cfb3b7d5p-1,
    0x1.ab41aep3,
  },
  { // Entry 710
    -0x1.6a09c909add4dbf32253a39d5c306308p-1,
    0x1.6a0a03c63742d62802d163d5cfb3b7d5p-1,
    -0x1.ab41aep3,
  },
  { // Entry 711
    0x1.6a09dfaa4d5c3a7f056f3e61a365b29ep-1,
    0x1.6a09ed2599fd364c97660cca6652c0a3p-1,
    0x1.ab41b0p3,
  },
  { // Entry 712
    -0x1.6a09dfaa4d5c3a7f056f3e61a365b29ep-1,
    0x1.6a09ed2599fd364c97660cca6652c0a3p-1,
    -0x1.ab41b0p3,
  },
  { // Entry 713
    0x1.6a09f64aeb798f2b3e3d9b16e8e3c412p-1,
    0x1.6a09d684fb4d8c840660d6b42ec83039p-1,
    0x1.ab41b2p3,
  },
  { // Entry 714
    -0x1.6a09f64aeb798f2b3e3d9b16e8e3c412p-1,
    0x1.6a09d684fb4d8c840660d6b42ec83039p-1,
    -0x1.ab41b2p3,
  },
  { // Entry 715
    0x1.fffffffffc260d6ffb8f4cd8ab3fd020p-1,
    0x1.f66595da7a1ae308d26a18de4c2ed3a3p-20,
    0x1.c463a8p3,
  },
  { // Entry 716
    -0x1.fffffffffc260d6ffb8f4cd8ab3fd020p-1,
    0x1.f66595da7a1ae308d26a18de4c2ed3a3p-20,
    -0x1.c463a8p3,
  },
  { // Entry 717
    0x1.ffffffffff12d89bb084dd762848b3d6p-1,
    0x1.eccb2bb4f66ea861241fa09ca9d8a034p-21,
    0x1.c463aap3,
  },
  { // Entry 718
    -0x1.ffffffffff12d89bb084dd762848b3d6p-1,
    0x1.eccb2bb4f66ea861241fa09ca9d8a034p-21,
    -0x1.c463aap3,
  },
  { // Entry 719
    0x1.ffffffffffffa3c7657b85e5b44bbd44p-1,
    -0x1.334d44b0945407b118b361ab78171f67p-25,
    0x1.c463acp3,
  },
  { // Entry 720
    -0x1.ffffffffffffa3c7657b85e5b44bbd44p-1,
    -0x1.334d44b0945407b118b361ab78171f67p-25,
    -0x1.c463acp3,
  },
  { // Entry 721
    0x1.6a0a0b38f134a3295a0b386e42f1ca7ap-1,
    -0x1.6a09c196f2867cc916ae2b7e6c9d99c1p-1,
    0x1.dd85a4p3,
  },
  { // Entry 722
    -0x1.6a0a0b38f134a3295a0b386e42f1ca7ap-1,
    -0x1.6a09c196f2867cc916ae2b7e6c9d99c1p-1,
    -0x1.dd85a4p3,
  },
  { // Entry 723
    0x1.6a09f49854662eff1b35755a129044a7p-1,
    -0x1.6a09d837928506f7cff76f094b4e0377p-1,
    0x1.dd85a6p3,
  },
  { // Entry 724
    -0x1.6a09f49854662eff1b35755a129044a7p-1,
    -0x1.6a09d837928506f7cff76f094b4e0377p-1,
    -0x1.dd85a6p3,
  },
  { // Entry 725
    0x1.6a09ddf7b62db0e0440b6a4262203b11p-1,
    -0x1.6a09eed83119874e51ae4bb8aeddc1f2p-1,
    0x1.dd85a8p3,
  },
  { // Entry 726
    -0x1.6a09ddf7b62db0e0440b6a4262203b11p-1,
    -0x1.6a09eed83119874e51ae4bb8aeddc1f2p-1,
    -0x1.dd85a8p3,
  },
  { // Entry 727
    0x1.4aa9c2f2c19a062b18a017bcd5424feap-20,
    -0x1.fffffffffe54e5e4d32b3453166060b3p-1,
    0x1.f6a7a0p3,
  },
  { // Entry 728
    -0x1.4aa9c2f2c19a062b18a017bcd5424feap-20,
    -0x1.fffffffffe54e5e4d32b3453166060b3p-1,
    -0x1.f6a7a0p3,
  },
  { // Entry 729
    0x1.2aa70bcb07d3a40781510d213652e43ap-22,
    -0x1.ffffffffffea396ab8aee509392c755dp-1,
    0x1.f6a7a2p3,
  },
  { // Entry 730
    -0x1.2aa70bcb07d3a40781510d213652e43ap-22,
    -0x1.ffffffffffea396ab8aee509392c755dp-1,
    -0x1.f6a7a2p3,
  },
  { // Entry 731
    -0x1.6aac7a1a7bf5bbd49572ffb8d7749922p-21,
    -0x1.ffffffffff7f8cf09e32d6309bea85cap-1,
    0x1.f6a7a4p3,
  },
  { // Entry 732
    0x1.6aac7a1a7bf5bbd49572ffb8d7749922p-21,
    -0x1.ffffffffff7f8cf09e32d6309bea85cap-1,
    -0x1.f6a7a4p3,
  },
  { // Entry 733
    -0x1.6a09a383953124096898340f9168b9d5p-1,
    -0x1.6a0a294c45ec747a47711a4994d2c5e4p-1,
    0x1.07e4ccp4,
  },
  { // Entry 734
    0x1.6a09a383953124096898340f9168b9d5p-1,
    -0x1.6a0a294c45ec747a47711a4994d2c5e4p-1,
    -0x1.07e4ccp4,
  },
  { // Entry 735
    -0x1.6a09d0c4d7869961c47a9b0b968cc910p-1,
    -0x1.6a09fc0b0ea7ed9fb5dd50a0c8af19cbp-1,
    0x1.07e4cep4,
  },
  { // Entry 736
    0x1.6a09d0c4d7869961c47a9b0b968cc910p-1,
    -0x1.6a09fc0b0ea7ed9fb5dd50a0c8af19cbp-1,
    -0x1.07e4cep4,
  },
  { // Entry 737
    -0x1.6a09fe061433e7770d00ca59d5a56251p-1,
    -0x1.6a09cec9d1bb3ed4f810c9f9786d610ep-1,
    0x1.07e4d0p4,
  },
  { // Entry 738
    0x1.6a09fe061433e7770d00ca59d5a56251p-1,
    -0x1.6a09cec9d1bb3ed4f810c9f9786d610ep-1,
    -0x1.07e4d0p4,
  },
  { // Entry 739
    -0x1.fffffffff9219dae5feda1b539335803p-1,
    -0x1.4f76f80582c73fc0cc0903ed8ca7d6b3p-19,
    0x1.1475cap4,
  },
  { // Entry 740
    0x1.fffffffff9219dae5feda1b539335803p-1,
    -0x1.4f76f80582c73fc0cc0903ed8ca7d6b3p-19,
    -0x1.1475cap4,
  },
  { // Entry 741
    -0x1.ffffffffff9d556e8c0bf0a80d610808p-1,
    -0x1.3ddbe0161108b690eed70a7f59de751cp-21,
    0x1.1475ccp4,
  },
  { // Entry 742
    0x1.ffffffffff9d556e8c0bf0a80d610808p-1,
    -0x1.3ddbe0161108b690eed70a7f59de751cp-21,
    -0x1.1475ccp4,
  },
  { // Entry 743
    -0x1.fffffffffe190d2eb82e74efd2093215p-1,
    0x1.61120ff4f70180b0d55c3ae0f69585cap-20,
    0x1.1475cep4,
  },
  { // Entry 744
    0x1.fffffffffe190d2eb82e74efd2093215p-1,
    0x1.61120ff4f70180b0d55c3ae0f69585cap-20,
    -0x1.1475cep4,
  },
  { // Entry 745
    -0x1.6a0a1a1e64a28eee238dc852846aacd5p-1,
    0x1.6a09b2b17b741050a6cfd64b81c76485p-1,
    0x1.2106c8p4,
  },
  { // Entry 746
    0x1.6a0a1a1e64a28eee238dc852846aacd5p-1,
    0x1.6a09b2b17b741050a6cfd64b81c76485p-1,
    -0x1.2106c8p4,
  },
  { // Entry 747
    -0x1.6a09ecdd2b784b699034ee8102670e27p-1,
    0x1.6a09dff2bbe3c9616a3576c55e773207p-1,
    0x1.2106cap4,
  },
  { // Entry 748
    0x1.6a09ecdd2b784b699034ee8102670e27p-1,
    0x1.6a09dff2bbe3c9616a3576c55e773207p-1,
    -0x1.2106cap4,
  },
  { // Entry 749
    -0x1.6a09bf9beca5e03188301639c09ed574p-1,
    0x1.6a0a0d33f6ab5af262ad6ad18ac1ce9fp-1,
    0x1.2106ccp4,
  },
  { // Entry 750
    0x1.6a09bf9beca5e03188301639c09ed574p-1,
    0x1.6a0a0d33f6ab5af262ad6ad18ac1ce9fp-1,
    -0x1.2106ccp4,
  },
  { // Entry 751
    -0x1.f9990e91a270d3bc1c02f4f69f48e675p-19,
    0x1.fffffffff065cb240bcbfdff4977ddf8p-1,
    0x1.2d97c4p4,
  },
  { // Entry 752
    0x1.f9990e91a270d3bc1c02f4f69f48e675p-19,
    0x1.fffffffff065cb240bcbfdff4977ddf8p-1,
    -0x1.2d97c4p4,
  },
  { // Entry 753
    -0x1.f3321d234deacd6f3afd75039685012fp-20,
    0x1.fffffffffc32939898f585d6948cf2d1p-1,
    0x1.2d97c6p4,
  },
  { // Entry 754
    0x1.f3321d234deacd6f3afd75039685012fp-20,
    0x1.fffffffffc32939898f585d6948cf2d1p-1,
    -0x1.2d97c6p4,
  },
  { // Entry 755
    0x1.99bc5b961b1aa1c9e8023074f3406fd9p-25,
    0x1.ffffffffffff5c0d2630ee0a27e8d6d1p-1,
    0x1.2d97c8p4,
  },
  { // Entry 756
    -0x1.99bc5b961b1aa1c9e8023074f3406fd9p-25,
    0x1.ffffffffffff5c0d2630ee0a27e8d6d1p-1,
    -0x1.2d97c8p4,
  },
  { // Entry 757
    0x1.6a09949e1ce1ec501afcb35d731bf62cp-1,
    0x1.6a0a3831b81d94966ad8df4d378824f9p-1,
    0x1.3a28c2p4,
  },
  { // Entry 758
    -0x1.6a09949e1ce1ec501afcb35d731bf62cp-1,
    0x1.6a0a3831b81d94966ad8df4d378824f9p-1,
    -0x1.3a28c2p4,
  },
  { // Entry 759
    0x1.6a09c1df6114100c65d1ff6c55755e72p-1,
    0x1.6a0a0af082b5bca7f5569f4da6883f64p-1,
    0x1.3a28c4p4,
  },
  { // Entry 760
    -0x1.6a09c1df6114100c65d1ff6c55755e72p-1,
    0x1.6a0a0af082b5bca7f5569f4da6883f64p-1,
    -0x1.3a28c4p4,
  },
  { // Entry 761
    0x1.6a09ef209f9e0cc13324ddf2b361553fp-1,
    0x1.6a09ddaf47a5bc8dbdcb6b13844902aep-1,
    0x1.3a28c6p4,
  },
  { // Entry 762
    -0x1.6a09ef209f9e0cc13324ddf2b361553fp-1,
    0x1.6a09ddaf47a5bc8dbdcb6b13844902aep-1,
    -0x1.3a28c6p4,
  },
  { // Entry 763
    0x1.fffffffff53f476ec4f59f26c4bcdfa0p-1,
    0x1.a3bb251dc7efaa1e2137bb37ed6654dbp-19,
    0x1.46b9c0p4,
  },
  { // Entry 764
    -0x1.fffffffff53f476ec4f59f26c4bcdfa0p-1,
    0x1.a3bb251dc7efaa1e2137bb37ed6654dbp-19,
    -0x1.46b9c0p4,
  },
  { // Entry 765
    0x1.fffffffffe5d2097b34334ad679dd7a4p-1,
    0x1.47764a3b9566758e5baa2e3029f1abbap-20,
    0x1.46b9c2p4,
  },
  { // Entry 766
    -0x1.fffffffffe5d2097b34334ad679dd7a4p-1,
    0x1.47764a3b9566758e5baa2e3029f1abbap-20,
    -0x1.46b9c2p4,
  },
  { // Entry 767
    0x1.ffffffffff7af9c0a19a005c565c6af7p-1,
    -0x1.71136b88d4608490f2ddfe90101112aep-21,
    0x1.46b9c4p4,
  },
  { // Entry 768
    -0x1.ffffffffff7af9c0a19a005c565c6af7p-1,
    -0x1.71136b88d4608490f2ddfe90101112aep-21,
    -0x1.46b9c4p4,
  },
  { // Entry 769
    0x1.6a0a2903d773925b052fb006ac670c23p-1,
    -0x1.6a09a3cc03c4bbad2222dfe5be317565p-1,
    0x1.534abep4,
  },
  { // Entry 770
    -0x1.6a0a2903d773925b052fb006ac670c23p-1,
    -0x1.6a09a3cc03c4bbad2222dfe5be317565p-1,
    -0x1.534abep4,
  },
  { // Entry 771
    0x1.6a09fbc2a025fdae918466fa00142143p-1,
    -0x1.6a09d10d46112335d0e43d738387de8cp-1,
    0x1.534ac0p4,
  },
  { // Entry 772
    -0x1.6a09fbc2a025fdae918466fa00142143p-1,
    -0x1.6a09d10d46112335d0e43d738387de8cp-1,
    -0x1.534ac0p4,
  },
  { // Entry 773
    0x1.6a09ce8163304113135a68ae93d3fa0ep-1,
    -0x1.6a09fe4e82b5637a4a8f392c3301be94p-1,
    0x1.534ac2p4,
  },
  { // Entry 774
    -0x1.6a09ce8163304113135a68ae93d3fa0ep-1,
    -0x1.6a09fe4e82b5637a4a8f392c3301be94p-1,
    -0x1.534ac2p4,
  },
  { // Entry 775
    0x1.4ddd3ba9ecb19d6bb6ea161120e447b9p-19,
    -0x1.fffffffff9325ace5f682bbb8b122a09p-1,
    0x1.5fdbbcp4,
  },
  { // Entry 776
    -0x1.4ddd3ba9ecb19d6bb6ea161120e447b9p-19,
    -0x1.fffffffff9325ace5f682bbb8b122a09p-1,
    -0x1.5fdbbcp4,
  },
  { // Entry 777
    0x1.3774eea7b89d80df7816fe208ec69fc0p-21,
    -0x1.ffffffffffa144abaed5b4aab880635dp-1,
    0x1.5fdbbep4,
  },
  { // Entry 778
    -0x1.3774eea7b89d80df7816fe208ec69fc0p-21,
    -0x1.ffffffffffa144abaed5b4aab880635dp-1,
    -0x1.5fdbbep4,
  },
  { // Entry 779
    -0x1.644588ac2334a3d5452d9960282cf80dp-20,
    -0x1.fffffffffe102e88fe476331e1ddefafp-1,
    0x1.5fdbc0p4,
  },
  { // Entry 780
    0x1.644588ac2334a3d5452d9960282cf80dp-20,
    -0x1.fffffffffe102e88fe476331e1ddefafp-1,
    -0x1.5fdbc0p4,
  },
  { // Entry 781
    -0x1.6a09b2f9ea049e855e35ca9ce7e0d89ap-1,
    -0x1.6a0a19d5f626a35ee112a34638e07808p-1,
    0x1.6c6cbap4,
  },
  { // Entry 782
    0x1.6a09b2f9ea049e855e35ca9ce7e0d89ap-1,
    -0x1.6a0a19d5f626a35ee112a34638e07808p-1,
    -0x1.6c6cbap4,
  },
  { // Entry 783
    -0x1.6a09e03b2a6b49c6134c67b42baee668p-1,
    -0x1.6a09ec94bcf35208ccd030684d5ddd9cp-1,
    0x1.6c6cbcp4,
  },
  { // Entry 784
    0x1.6a09e03b2a6b49c6134c67b42baee668p-1,
    -0x1.6a09ec94bcf35208ccd030684d5ddd9cp-1,
    -0x1.6c6cbcp4,
  },
  { // Entry 785
    -0x1.6a0a0d7c6529cd85dbbb3a5c2cd3fae5p-1,
    -0x1.6a09bf537e17d900659bd2fa24c3a8c8p-1,
    0x1.6c6cbep4,
  },
  { // Entry 786
    0x1.6a0a0d7c6529cd85dbbb3a5c2cd3fae5p-1,
    -0x1.6a09bf537e17d900659bd2fa24c3a8c8p-1,
    -0x1.6c6cbep4,
  },
  { // Entry 787
    -0x1.fffffffff07f0ab12aa8f41f29c15392p-1,
    -0x1.f7ff52360c622b3f94d9c7250bfad8d4p-19,
    0x1.78fdb6p4,
  },
  { // Entry 788
    0x1.fffffffff07f0ab12aa8f41f29c15392p-1,
    -0x1.f7ff52360c622b3f94d9c7250bfad8d4p-19,
    -0x1.78fdb6p4,
  },
  { // Entry 789
    -0x1.fffffffffc3f0542db21dcbcb847dac3p-1,
    -0x1.effea46c21baa3da7c266c953a013598p-20,
    0x1.78fdb8p4,
  },
  { // Entry 790
    0x1.fffffffffc3f0542db21dcbcb847dac3p-1,
    -0x1.effea46c21baa3da7c266c953a013598p-20,
    -0x1.78fdb8p4,
  },
  { // Entry 791
    -0x1.fffffffffffeffd48bac73efe60c7fcfp-1,
    0x1.0015b93dd0f095be1eb0a5b87fe5e33ep-24,
    0x1.78fdbap4,
  },
  { // Entry 792
    0x1.fffffffffffeffd48bac73efe60c7fcfp-1,
    0x1.0015b93dd0f095be1eb0a5b87fe5e33ep-24,
    -0x1.78fdbap4,
  },
  { // Entry 793
    -0x1.6a0a37e949a7ad698a32234c73e5afbap-1,
    0x1.6a0994e68b787ee4fd6830b288225745p-1,
    0x1.858eb4p4,
  },
  { // Entry 794
    0x1.6a0a37e949a7ad698a32234c73e5afbap-1,
    0x1.6a0994e68b787ee4fd6830b288225745p-1,
    -0x1.858eb4p4,
  },
  { // Entry 795
    -0x1.6a0a0aa81436c7a8d33a38d704030d14p-1,
    0x1.6a09c227cfa194d1fa7ab9909de5083cp-1,
    0x1.858eb6p4,
  },
  { // Entry 796
    0x1.6a0a0aa81436c7a8d33a38d704030d14p-1,
    0x1.6a09c227cfa194d1fa7ab9909de5083cp-1,
    -0x1.858eb6p4,
  },
  { // Entry 797
    -0x1.6a09dd66d91db9bd7bf355faff08f194p-1,
    0x1.6a09ef690e2283b658509ed319483839p-1,
    0x1.858eb8p4,
  },
  { // Entry 798
    0x1.6a09dd66d91db9bd7bf355faff08f194p-1,
    0x1.6a09ef690e2283b658509ed319483839p-1,
    -0x1.858eb8p4,
  },
  { // Entry 799
    0x1.c048b38a8bbf59f414fec7079209926ep-3,
    -0x1.f3957bad70e0741f1d3d6751246ce21ap-1,
    0x1.fffffep62,
  },
  { // Entry 800
    -0x1.c048b38a8bbf59f414fec7079209926ep-3,
    -0x1.f3957bad70e0741f1d3d6751246ce21ap-1,
    -0x1.fffffep62,
  },
  { // Entry 801
    0x1.fff6dfd42dc54430bc0576b00a88bd94p-1,
    0x1.82aa375b3c33e70663731bab4beb6ed3p-7,
    0x1.p63,
  },
  { // Entry 802
    -0x1.fff6dfd42dc54430bc0576b00a88bd94p-1,
    0x1.82aa375b3c33e70663731bab4beb6ed3p-7,
    -0x1.p63,
  },
  { // Entry 803
    -0x1.d6637d070347ee94e830445e76486727p-1,
    0x1.945e6c69a580fb7bb27d02c0fe0f8a71p-2,
    0x1.000002p63,
  },
  { // Entry 804
    0x1.d6637d070347ee94e830445e76486727p-1,
    0x1.945e6c69a580fb7bb27d02c0fe0f8a71p-2,
    -0x1.000002p63,
  },
  { // Entry 805
    -0x1.0e5283661df0ca0f55ab6167e14514a1p-1,
    -0x1.b2d255f2bd0423e29e2a548728f034abp-1,
    0x1.fffffep26,
  },
  { // Entry 806
    0x1.0e5283661df0ca0f55ab6167e14514a1p-1,
    -0x1.b2d255f2bd0423e29e2a548728f034abp-1,
    -0x1.fffffep26,
  },
  { // Entry 807
    -0x1.86dcc9babb0a40ee875cab3b9e892757p-1,
    0x1.4ab6511a7d39ad3cc88ded1e775ca147p-1,
    0x1.p27,
  },
  { // Entry 808
    0x1.86dcc9babb0a40ee875cab3b9e892757p-1,
    0x1.4ab6511a7d39ad3cc88ded1e775ca147p-1,
    -0x1.p27,
  },
  { // Entry 809
    0x1.171999b629fd5b6357c6dff4d7827d95p-1,
    -0x1.ad3d80c82f4452b076581de24648435bp-1,
    0x1.000002p27,
  },
  { // Entry 810
    -0x1.171999b629fd5b6357c6dff4d7827d95p-1,
    -0x1.ad3d80c82f4452b076581de24648435bp-1,
    -0x1.000002p27,
  },
  { // Entry 811
    -0x1.e57ec09221973550d1e5798dcf0cd25dp-1,
    -0x1.4532c3721ed4343ad88eea8908a988cbp-2,
    0x1.fffffep23,
  },
  { // Entry 812
    0x1.e57ec09221973550d1e5798dcf0cd25dp-1,
    -0x1.4532c3721ed4343ad88eea8908a988cbp-2,
    -0x1.fffffep23,
  },
  { // Entry 813
    -0x1.8f22f8433d6edfe9a4aff9622517caa9p-1,
    0x1.40ad67f3f0c9a143963c9c96dbce3f8ap-1,
    0x1.p24,
  },
  { // Entry 814
    0x1.8f22f8433d6edfe9a4aff9622517caa9p-1,
    0x1.40ad67f3f0c9a143963c9c96dbce3f8ap-1,
    -0x1.p24,
  },
  { // Entry 815
    0x1.c9b0c7265c543f80faf01741c6458560p-1,
    0x1.caf8537c3e442ca8aca86c156773853ap-2,
    0x1.000002p24,
  },
  { // Entry 816
    -0x1.c9b0c7265c543f80faf01741c6458560p-1,
    0x1.caf8537c3e442ca8aca86c156773853ap-2,
    -0x1.000002p24,
  },
  { // Entry 817
    -0x1.837b98a3185d1466d852f0a7dc1d248ep-1,
    -0x1.4eaa667ba0b90dfb05ab3d9c247cdee7p-1,
    0x1.fffffep1,
  },
  { // Entry 818
    0x1.837b98a3185d1466d852f0a7dc1d248ep-1,
    -0x1.4eaa667ba0b90dfb05ab3d9c247cdee7p-1,
    -0x1.fffffep1,
  },
  { // Entry 819
    -0x1.837b9dddc1eae70ce98055a0e450d93cp-1,
    -0x1.4eaa606db24c0c466da1c2dc7baa2b32p-1,
    0x1.p2,
  },
  { // Entry 820
    0x1.837b9dddc1eae70ce98055a0e450d93cp-1,
    -0x1.4eaa606db24c0c466da1c2dc7baa2b32p-1,
    -0x1.p2,
  },
  { // Entry 821
    -0x1.837ba85314bde52b1e9c2c8ed2712c72p-1,
    -0x1.4eaa5451d53348eb89dc478d4d11be02p-1,
    0x1.000002p2,
  },
  { // Entry 822
    0x1.837ba85314bde52b1e9c2c8ed2712c72p-1,
    -0x1.4eaa5451d53348eb89dc478d4d11be02p-1,
    -0x1.000002p2,
  },
  { // Entry 823
    0x1.d18f70573da63012fa1c0e3d2ebbe59cp-1,
    -0x1.aa225e2ef96241915b6fd217522814f5p-2,
    0x1.fffffep0,
  },
  { // Entry 824
    -0x1.d18f70573da63012fa1c0e3d2ebbe59cp-1,
    -0x1.aa225e2ef96241915b6fd217522814f5p-2,
    -0x1.fffffep0,
  },
  { // Entry 825
    0x1.d18f6ead1b445dfab848188009c9bb95p-1,
    -0x1.aa22657537204a4332f8acbb72b0d768p-2,
    0x1.p1,
  },
  { // Entry 826
    -0x1.d18f6ead1b445dfab848188009c9bb95p-1,
    -0x1.aa22657537204a4332f8acbb72b0d768p-2,
    -0x1.p1,
  },
  { // Entry 827
    0x1.d18f6b58d66ae7110b2b6f7cffba6ec1p-1,
    -0x1.aa227401b288620a0372d5a96084915dp-2,
    0x1.000002p1,
  },
  { // Entry 828
    -0x1.d18f6b58d66ae7110b2b6f7cffba6ec1p-1,
    -0x1.aa227401b288620a0372d5a96084915dp-2,
    -0x1.000002p1,
  },
  { // Entry 829
    0x1.aed547dbee4d0d8680d0813d1e4e21d0p-1,
    0x1.14a282aa25b11f6312a7a65180e7c3d4p-1,
    0x1.fffffep-1,
  },
  { // Entry 830
    -0x1.aed547dbee4d0d8680d0813d1e4e21d0p-1,
    0x1.14a282aa25b11f6312a7a65180e7c3d4p-1,
    -0x1.fffffep-1,
  },
  { // Entry 831
    0x1.aed548f090cee0418dd3d2138a1e7865p-1,
    0x1.14a280fb5068b923848cdb2ed0e37a53p-1,
    0x1.p0,
  },
  { // Entry 832
    -0x1.aed548f090cee0418dd3d2138a1e7865p-1,
    0x1.14a280fb5068b923848cdb2ed0e37a53p-1,
    -0x1.p0,
  },
  { // Entry 833
    0x1.aed54b19d5cd7937cbf41ed408ca0a52p-1,
    0x1.14a27d9da5d4aebce71428f9057b08dap-1,
    0x1.000002p0,
  },
  { // Entry 834
    -0x1.aed54b19d5cd7937cbf41ed408ca0a52p-1,
    0x1.14a27d9da5d4aebce71428f9057b08dap-1,
    -0x1.000002p0,
  },
  { // Entry 835
    0x1.eaee85835dde5b71beec7d8d98052112p-2,
    0x1.c15280e0737692dd436908fdc8e6e2e1p-1,
    0x1.fffffep-2,
  },
  { // Entry 836
    -0x1.eaee85835dde5b71beec7d8d98052112p-2,
    0x1.c15280e0737692dd436908fdc8e6e2e1p-1,
    -0x1.fffffep-2,
  },
  { // Entry 837
    0x1.eaee8744b05efe8764bc364fd837b666p-2,
    0x1.c1528065b7d4f9db7bbb3b45f5f5b30ap-1,
    0x1.p-1,
  },
  { // Entry 838
    -0x1.eaee8744b05efe8764bc364fd837b666p-2,
    0x1.c1528065b7d4f9db7bbb3b45f5f5b30ap-1,
    -0x1.p-1,
  },
  { // Entry 839
    0x1.eaee8ac7555ed47fca77ceed174c8ea0p-2,
    0x1.c1527f70409076da0c3204df1e099a83p-1,
    0x1.000002p-1,
  },
  { // Entry 840
    -0x1.eaee8ac7555ed47fca77ceed174c8ea0p-2,
    0x1.c1527f70409076da0c3204df1e099a83p-1,
    -0x1.000002p-1,
  },
  { // Entry 841
    0x1.faaeeb5f1c0d63f43c6f3ec46011690fp-3,
    0x1.f0154a1789d8dcc172cd2092d05f6394p-1,
    0x1.fffffep-3,
  },
  { // Entry 842
    -0x1.faaeeb5f1c0d63f43c6f3ec46011690fp-3,
    0x1.f0154a1789d8dcc172cd2092d05f6394p-1,
    -0x1.fffffep-3,
  },
  { // Entry 843
    0x1.faaeed4f31576ba89debdc7351e8b1aep-3,
    0x1.f01549f7deea174f07a67972bf29f148p-1,
    0x1.p-2,
  },
  { // Entry 844
    -0x1.faaeed4f31576ba89debdc7351e8b1aep-3,
    0x1.f01549f7deea174f07a67972bf29f148p-1,
    -0x1.p-2,
  },
  { // Entry 845
    0x1.faaef12f5beb1c1094473d3c3365b9e1p-3,
    0x1.f01549b8890c2f66337cac15a7237c8ep-1,
    0x1.000002p-2,
  },
  { // Entry 846
    -0x1.faaef12f5beb1c1094473d3c3365b9e1p-3,
    0x1.f01549b8890c2f66337cac15a7237c8ep-1,
    -0x1.000002p-2,
  },
  { // Entry 847
    0x1.feaaecec6d8e30cd56950eb2ebdcebd4p-4,
    0x1.fc01552fd068ee83f5b742c05245e8b2p-1,
    0x1.fffffep-4,
  },
  { // Entry 848
    -0x1.feaaecec6d8e30cd56950eb2ebdcebd4p-4,
    0x1.fc01552fd068ee83f5b742c05245e8b2p-1,
    -0x1.fffffep-4,
  },
  { // Entry 849
    0x1.feaaeee86ee35ca069a86721f89f85a5p-4,
    0x1.fc015527d5bd36da3cd4253bede319cap-1,
    0x1.p-3,
  },
  { // Entry 850
    -0x1.feaaeee86ee35ca069a86721f89f85a5p-4,
    0x1.fc015527d5bd36da3cd4253bede319cap-1,
    -0x1.p-3,
  },
  { // Entry 851
    0x1.feaaf2e0718d9c568c9442c81545cd62p-4,
    0x1.fc015517e065afb6bb102c18f5919820p-1,
    0x1.000002p-3,
  },
  { // Entry 852
    -0x1.feaaf2e0718d9c568c9442c81545cd62p-4,
    0x1.fc015517e065afb6bb102c18f5919820p-1,
    -0x1.000002p-3,
  },
  { // Entry 853
    0x1.ffaaacefd4d855ac8227799f3e263d7ap-5,
    0x1.ff0015569ef7e2b96301e6f752c019d4p-1,
    0x1.fffffep-5,
  },
  { // Entry 854
    -0x1.ffaaacefd4d855ac8227799f3e263d7ap-5,
    0x1.ff0015569ef7e2b96301e6f752c019d4p-1,
    -0x1.fffffep-5,
  },
  { // Entry 855
    0x1.ffaaaeeed4edab4ba4b365ed25a9595fp-5,
    0x1.ff0015549f4d34ca0e1ee6509bc42b71p-1,
    0x1.p-4,
  },
  { // Entry 856
    -0x1.ffaaaeeed4edab4ba4b365ed25a9595fp-5,
    0x1.ff0015549f4d34ca0e1ee6509bc42b71p-1,
    -0x1.p-4,
  },
  { // Entry 857
    0x1.ffaab2ecd518508ae9bc730a165a8eadp-5,
    0x1.ff0015509ff7d2ee6418e924f0de5e97p-1,
    0x1.000002p-4,
  },
  { // Entry 858
    -0x1.ffaab2ecd518508ae9bc730a165a8eadp-5,
    0x1.ff0015509ff7d2ee6418e924f0de5e97p-1,
    -0x1.000002p-4,
  },
  { // Entry 859
    0x1.ffeaa8ef2e85933883c0dc33462387b5p-6,
    0x1.ffc00155d277d58e727cd95c43f759cfp-1,
    0x1.fffffep-6,
  },
  { // Entry 860
    -0x1.ffeaa8ef2e85933883c0dc33462387b5p-6,
    0x1.ffc00155d277d58e727cd95c43f759cfp-1,
    -0x1.fffffep-6,
  },
  { // Entry 861
    0x1.ffeaaaeeee86e8cafe41376d47919579p-6,
    0x1.ffc00155527d2b12aedb49d92928df72p-1,
    0x1.p-5,
  },
  { // Entry 862
    -0x1.ffeaaaeeee86e8cafe41376d47919579p-6,
    0x1.ffc00155527d2b12aedb49d92928df72p-1,
    -0x1.p-5,
  },
  { // Entry 863
    0x1.ffeaaeee6e89927003413abe64e9dc21p-6,
    0x1.ffc001545287d49b57972af5145663a0p-1,
    0x1.000002p-5,
  },
  { // Entry 864
    -0x1.ffeaaeee6e89927003413abe64e9dc21p-6,
    0x1.ffc001545287d49b57972af5145663a0p-1,
    -0x1.000002p-5,
  },
  { // Entry 865
    0x1.fffaa8aefeed396ffffc636313d0ba6dp-7,
    0x1.fff0001575499f3d7996e2da11cdeb24p-1,
    0x1.fffffep-7,
  },
  { // Entry 866
    -0x1.fffaa8aefeed396ffffc636313d0ba6dp-7,
    0x1.fff0001575499f3d7996e2da11cdeb24p-1,
    -0x1.fffffep-7,
  },
  { // Entry 867
    0x1.fffaaaaeeeed4ed549c6560f889ee531p-7,
    0x1.fff000155549f4a28a280e97bcd59c8ap-1,
    0x1.p-6,
  },
  { // Entry 868
    -0x1.fffaaaaeeeed4ed549c6560f889ee531p-7,
    0x1.fff000155549f4a28a280e97bcd59c8ap-1,
    -0x1.p-6,
  },
  { // Entry 869
    0x1.fffaaeaeceed793fde5a1a9ca5bb1ee6p-7,
    0x1.fff00015154a9f0cae4a62151501cd0ap-1,
    0x1.000002p-6,
  },
  { // Entry 870
    -0x1.fffaaeaeceed793fde5a1a9ca5bb1ee6p-7,
    0x1.fff00015154a9f0cae4a62151501cd0ap-1,
    -0x1.000002p-6,
  },
  { // Entry 871
    0x1.fffffdfaaaaabaaeeeded997feffa35ap-15,
    0x1.fffffff0000020155544fff49fca38e6p-1,
    0x1.fffffep-15,
  },
  { // Entry 872
    -0x1.fffffdfaaaaabaaeeeded997feffa35ap-15,
    0x1.fffffff0000020155544fff49fca38e6p-1,
    -0x1.fffffep-15,
  },
  { // Entry 873
    0x1.fffffffaaaaaaaaeeeeeeeed4ed4ed4fp-15,
    0x1.fffffff00000001555555549f49f49f7p-1,
    0x1.p-14,
  },
  { // Entry 874
    -0x1.fffffffaaaaaaaaeeeeeeeed4ed4ed4fp-15,
    0x1.fffffff00000001555555549f49f49f7p-1,
    -0x1.p-14,
  },
  { // Entry 875
    0x1.000001fd5555455777578ccbe7bfc09cp-14,
    0x1.ffffffefffffc0155515fff4a1496c1cp-1,
    0x1.000002p-14,
  },
  { // Entry 876
    -0x1.000001fd5555455777578ccbe7bfc09cp-14,
    0x1.ffffffefffffc0155515fff4a1496c1cp-1,
    -0x1.000002p-14,
  },
  { // Entry 877
    0x1.fffffdfffffffeaaaaaeaaaaa6aaeef0p-28,
    0x1.fffffffffffffc000007fffffc015555p-1,
    0x1.fffffep-28,
  },
  { // Entry 878
    -0x1.fffffdfffffffeaaaaaeaaaaa6aaeef0p-28,
    0x1.fffffffffffffc000007fffffc015555p-1,
    -0x1.fffffep-28,
  },
  { // Entry 879
    0x1.fffffffffffffeaaaaaaaaaaaaaaeeeep-28,
    0x1.fffffffffffffc000000000000015555p-1,
    0x1.p-27,
  },
  { // Entry 880
    -0x1.fffffffffffffeaaaaaaaaaaaaaaeeeep-28,
    0x1.fffffffffffffc000000000000015555p-1,
    -0x1.p-27,
  },
  { // Entry 881
    0x1.000001ffffffff55555155554d557772p-27,
    0x1.fffffffffffffbffffeffffff0015555p-1,
    0x1.000002p-27,
  },
  { // Entry 882
    -0x1.000001ffffffff55555155554d557772p-27,
    0x1.fffffffffffffbffffeffffff0015555p-1,
    -0x1.000002p-27,
  },
  { // Entry 883
    0x1.fffffdfffffffffaaaaabaaaaa9aaaaep-31,
    0x1.fffffffffffffff000001ffffff00015p-1,
    0x1.fffffep-31,
  },
  { // Entry 884
    -0x1.fffffdfffffffffaaaaabaaaaa9aaaaep-31,
    0x1.fffffffffffffff000001ffffff00015p-1,
    -0x1.fffffep-31,
  },
  { // Entry 885
    0x1.fffffffffffffffaaaaaaaaaaaaaaaaep-31,
    0x1.fffffffffffffff00000000000000015p-1,
    0x1.p-30,
  },
  { // Entry 886
    -0x1.fffffffffffffffaaaaaaaaaaaaaaaaep-31,
    0x1.fffffffffffffff00000000000000015p-1,
    -0x1.p-30,
  },
  { // Entry 887
    0x1.000001fffffffffd5555455555355557p-30,
    0x1.ffffffffffffffefffffbfffffc00015p-1,
    0x1.000002p-30,
  },
  { // Entry 888
    -0x1.000001fffffffffd5555455555355557p-30,
    0x1.ffffffffffffffefffffbfffffc00015p-1,
    -0x1.000002p-30,
  },
  { // Entry 889
    0x1.0b3366508957520d9dc88d7c09337e24p-1,
    0x1.b4bf2c79bdfcdaa53ed6c013f65e0963p-1,
    -0x1.fffffep127,
  },
  { // Entry 890
    -0x1.0b3366508957520d9dc88d7c09337e24p-1,
    0x1.b4bf2c79bdfcdaa53ed6c013f65e0963p-1,
    0x1.fffffep127,
  },
  { // Entry 891
    -0x1.0b3366508957520d9dc88d7c09337e24p-1,
    0x1.b4bf2c79bdfcdaa53ed6c013f65e0963p-1,
    0x1.fffffep127,
  },
  { // Entry 892
    0x1.0b3366508957520d9dc88d7c09337e24p-1,
    0x1.b4bf2c79bdfcdaa53ed6c013f65e0963p-1,
    -0x1.fffffep127,
  },
  { // Entry 893
    -0x1.0b3366508957520d9dc88d7c09337e24p-1,
    0x1.b4bf2c79bdfcdaa53ed6c013f65e0963p-1,
    0x1.fffffep127,
  },
  { // Entry 894
    0x1.0b3366508957520d9dc88d7c09337e24p-1,
    0x1.b4bf2c79bdfcdaa53ed6c013f65e0963p-1,
    -0x1.fffffep127,
  },
  { // Entry 895
    -0x1.48ce575202efd93c62f7b88106ea1d4dp-1,
    -0x1.8877a29e3d7b6defcb528e86f4c3e09ap-1,
    0x1.fffffcp127,
  },
  { // Entry 896
    0x1.48ce575202efd93c62f7b88106ea1d4dp-1,
    -0x1.8877a29e3d7b6defcb528e86f4c3e09ap-1,
    -0x1.fffffcp127,
  },
  { // Entry 897
    -0x1.777a5cf72cec5fd61896cb4f40d1de79p-24,
    -0x1.fffffffffffdd94849271d08eecf54a1p-1,
    0x1.921fb6p1,
  },
  { // Entry 898
    0x1.777a5cf72cec5fd61896cb4f40d1de79p-24,
    -0x1.fffffffffffdd94849271d08eecf54a1p-1,
    -0x1.921fb6p1,
  },
  { // Entry 899
    0x1.ffffffffffff76521249c7422930ed82p-1,
    -0x1.777a5cf72cecc4cde3a31e7d5a026142p-25,
    0x1.921fb6p0,
  },
  { // Entry 900
    -0x1.ffffffffffff76521249c7422930ed82p-1,
    -0x1.777a5cf72cecc4cde3a31e7d5a026142p-25,
    -0x1.921fb6p0,
  },
  { // Entry 901
    0x1.aed54b19d5cd7937cbf41ed408ca0a52p-1,
    0x1.14a27d9da5d4aebce71428f9057b08dap-1,
    0x1.000002p0,
  },
  { // Entry 902
    -0x1.aed54b19d5cd7937cbf41ed408ca0a52p-1,
    0x1.14a27d9da5d4aebce71428f9057b08dap-1,
    -0x1.000002p0,
  },
  { // Entry 903
    0x1.aed548f090cee0418dd3d2138a1e7865p-1,
    0x1.14a280fb5068b923848cdb2ed0e37a53p-1,
    0x1.p0,
  },
  { // Entry 904
    -0x1.aed548f090cee0418dd3d2138a1e7865p-1,
    0x1.14a280fb5068b923848cdb2ed0e37a53p-1,
    -0x1.p0,
  },
  { // Entry 905
    0x1.aed547dbee4d0d8680d0813d1e4e21d0p-1,
    0x1.14a282aa25b11f6312a7a65180e7c3d4p-1,
    0x1.fffffep-1,
  },
  { // Entry 906
    -0x1.aed547dbee4d0d8680d0813d1e4e21d0p-1,
    0x1.14a282aa25b11f6312a7a65180e7c3d4p-1,
    -0x1.fffffep-1,
  },
  { // Entry 907
    0x1.6a09e6ecb41fdd7e681872c854887019p-1,
    0x1.6a09e5e3335983e5ac92e733e3f24b42p-1,
    0x1.921fb6p-1,
  },
  { // Entry 908
    -0x1.6a09e6ecb41fdd7e681872c854887019p-1,
    0x1.6a09e5e3335983e5ac92e733e3f24b42p-1,
    -0x1.921fb6p-1,
  },
  { // Entry 909
    0x1.000001ffffffffffffffffffffffffffp-126,
    0x1.ffffffffffffffffffffffffffffffffp-1,
    0x1.000002p-126,
  },
  { // Entry 910
    -0x1.000001ffffffffffffffffffffffffffp-126,
    0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.000002p-126,
  },
  { // Entry 911
    0x1.ffffffffffffffffffffffffffffffffp-127,
    0x1.ffffffffffffffffffffffffffffffffp-1,
    0x1.p-126,
  },
  { // Entry 912
    -0x1.ffffffffffffffffffffffffffffffffp-127,
    0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.p-126,
  },
  { // Entry 913
    0x1.fffffbffffffffffffffffffffffffffp-127,
    0x1.ffffffffffffffffffffffffffffffffp-1,
    0x1.fffffcp-127,
  },
  { // Entry 914
    -0x1.fffffbffffffffffffffffffffffffffp-127,
    0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.fffffcp-127,
  },
  { // Entry 915
    0x1.fffff7ffffffffffffffffffffffffffp-127,
    0x1.ffffffffffffffffffffffffffffffffp-1,
    0x1.fffff8p-127,
  },
  { // Entry 916
    -0x1.fffff7ffffffffffffffffffffffffffp-127,
    0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.fffff8p-127,
  },
  { // Entry 917
    0x1.ffffffffffffffffffffffffffffffffp-149,
    0x1.ffffffffffffffffffffffffffffffffp-1,
    0x1.p-148,
  },
  { // Entry 918
    -0x1.ffffffffffffffffffffffffffffffffp-149,
    0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.p-148,
  },
  { // Entry 919
    0.0f,
    0x1.ffffffffffffffffffffffffffffffffp-1,
    0x1.p-149,
  },
  { // Entry 920
    -0.0f,
    0x1.ffffffffffffffffffffffffffffffffp-1,
    -0x1.p-149,
  },
  { // Entry 921
    0.0,
    0x1.p0,
    0.0f,
  },
  { // Entry 922
    -0.0,
    0x1.p0,
    -0.0f,
  },
};
