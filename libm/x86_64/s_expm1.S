/*
Copyright (c) 2014, Intel Corporation
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice,
    * this list of conditions and the following disclaimer.

    * Redistributions in binary form must reproduce the above copyright notice,
    * this list of conditions and the following disclaimer in the documentation
    * and/or other materials provided with the distribution.

    * Neither the name of Intel Corporation nor the names of its contributors
    * may be used to endorse or promote products derived from this software
    * without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/******************************************************************************/
//                     ALGORITHM DESCRIPTION
//                     ---------------------
//
// Description:
//  Let K = 64 (table size).
//
//  Four sub-domains:
//    1. |x| < 1/(2*K)
//      expm1(x) ~ P(x)
//    2. 1/(2*K) <= |x| <= 56*log(2)
//       x       x/log(2)    n
//      e - 1 = 2         = 2 * T[j] * (1 + P(y)) - 1
//    3. 56*log(2) < x < MAX_LOG
//       x       x   x/log(2)    n
//      e - 1 ~ e = 2         = 2 * T[j] * (1 + P(y))
//    4. x < -56*log(2)
//       x            x
//      e - 1 = -1 + e ~ -1
//    where
//       x = m*log(2)/K + y,    y in [-log(2)/K..log(2)/K]
//       m = n*K + j,           m,n,j - signed integer, j in [-K/2..K/2]
//                  j/K
//       values of 2   are tabulated as T[j] = T_hi[j] ( 1 + T_lo[j]).
//
//       P(y) is a minimax polynomial approximation of exp(x)-1
//       on small interval [-log(2)/K..log(2)/K] (were calculated by Maple V).
//
//    In case 3, to avoid problems with arithmetic overflow and underflow,
//              n                        n1  n2
//    value of 2  is safely computed as 2 * 2 where n1 in [-BIAS/2..BIAS/2]
//    and BIAS is a value of exponent bias.
//
// Special cases:
//  expm1(NaN) is NaN
//  expm1(+INF) is +INF
//  expm1(-INF) is -1
//  expm1(x) is x for subnormals
//  for finite argument, only expm1(0)=0 is exact.
//  For IEEE double
//    if x > 709.782712893383973096 then expm1(x) overflow
//
/******************************************************************************/

#include <private/bionic_asm.h>
# -- Begin  expm1
ENTRY(expm1)
# parameter 1: %xmm0
..B1.1:
..___tag_value_expm1.1:
        subq      $56, %rsp
..___tag_value_expm1.3:
        movsd     %xmm0, 32(%rsp)
..B1.2:
        unpcklpd  %xmm0, %xmm0
        movapd    cv(%rip), %xmm1
        movapd    Shifter(%rip), %xmm6
        movapd    16+cv(%rip), %xmm2
        movapd    32+cv(%rip), %xmm3
        pextrw    $3, %xmm0, %eax
        andl      $32767, %eax
        movl      $16527, %edx
        subl      %eax, %edx
        subl      $16304, %eax
        orl       %eax, %edx
        cmpl      $-2147483648, %edx
        jae       .L_2TAG_PACKET_0.0.2
        mulpd     %xmm0, %xmm1
        addpd     %xmm6, %xmm1
        movapd    %xmm1, %xmm7
        subpd     %xmm6, %xmm1
        mulpd     %xmm1, %xmm2
        movapd    48+cv(%rip), %xmm4
        mulpd     %xmm1, %xmm3
        movapd    64+cv(%rip), %xmm5
        subpd     %xmm2, %xmm0
        movd      %xmm7, %eax
        movl      %eax, %ecx
        andl      $63, %ecx
        shll      $4, %ecx
        sarl      $6, %eax
        movl      %eax, %edx
        subpd     %xmm3, %xmm0
        lea       Tbl_addr(%rip), %r11
        movapd    (%rcx,%r11), %xmm2
        movq      80+cv(%rip), %xmm3
        mulpd     %xmm0, %xmm4
        movapd    %xmm0, %xmm1
        mulpd     %xmm0, %xmm0
        mulsd     %xmm0, %xmm3
        addpd     %xmm4, %xmm5
        mulsd     %xmm0, %xmm0
        movq      %xmm2, %xmm4
        unpckhpd  %xmm2, %xmm2
        movdqa    mmask(%rip), %xmm6
        pand      %xmm6, %xmm7
        movdqa    bias(%rip), %xmm6
        paddq     %xmm6, %xmm7
        psllq     $46, %xmm7
        mulsd     %xmm0, %xmm3
        mulpd     %xmm5, %xmm0
        addl      $894, %edx
        cmpl      $1916, %edx
        ja        .L_2TAG_PACKET_1.0.2
        addsd     %xmm3, %xmm0
        xorpd     %xmm3, %xmm3
        movl      $16368, %eax
        pinsrw    $3, %eax, %xmm3
        orpd      %xmm7, %xmm2
        mulsd     %xmm4, %xmm7
        movq      %xmm3, %xmm6
        addsd     %xmm1, %xmm3
        pextrw    $3, %xmm2, %edx
        pshufd    $238, %xmm0, %xmm5
        psrlq     $38, %xmm3
        psllq     $38, %xmm3
        movq      %xmm2, %xmm4
        subsd     %xmm3, %xmm6
        addsd     %xmm5, %xmm0
        addsd     %xmm6, %xmm1
        addsd     %xmm7, %xmm4
        mulsd     %xmm3, %xmm7
        mulsd     %xmm2, %xmm3
        xorpd     %xmm5, %xmm5
        movl      $16368, %eax
        pinsrw    $3, %eax, %xmm5
        addsd     %xmm1, %xmm0
        movl      $17184, %ecx
        subl      %edx, %ecx
        subl      $16256, %edx
        orl       %edx, %ecx
        jl        .L_2TAG_PACKET_2.0.2
        mulsd     %xmm4, %xmm0
        subsd     %xmm5, %xmm3
        addsd     %xmm7, %xmm0
        addsd     %xmm3, %xmm0
.L_2TAG_PACKET_3.0.2:
        jmp       ..B1.5
.L_2TAG_PACKET_2.0.2:
        cmpl      $0, %edx
        jl        .L_2TAG_PACKET_4.0.2
        mulsd     %xmm4, %xmm0
        subsd     %xmm5, %xmm7
        addsd     %xmm7, %xmm0
        addsd     %xmm3, %xmm0
        jmp       ..B1.5
.L_2TAG_PACKET_4.0.2:
        mulsd     %xmm4, %xmm0
        addsd     %xmm7, %xmm0
        addsd     %xmm3, %xmm0
        subsd     %xmm5, %xmm0
        jmp       ..B1.5
.L_2TAG_PACKET_1.0.2:
        movl      36(%rsp), %ecx
        addsd     %xmm0, %xmm1
        unpckhpd  %xmm0, %xmm0
        addsd     %xmm1, %xmm0
        cmpl      $0, %ecx
        jl        .L_2TAG_PACKET_5.0.2
        fstcw     (%rsp)
        movw      (%rsp), %dx
        orw       $768, %dx
        movw      %dx, 4(%rsp)
        fldcw     4(%rsp)
        movl      %eax, %edx
        sarl      $1, %eax
        subl      %eax, %edx
        movdqa    emask(%rip), %xmm6
        pandn     %xmm2, %xmm6
        addl      $1023, %eax
        movd      %eax, %xmm3
        psllq     $52, %xmm3
        orpd      %xmm3, %xmm6
        mulsd     %xmm3, %xmm4
        movsd     %xmm0, 16(%rsp)
        fldl      16(%rsp)
        movsd     %xmm6, 24(%rsp)
        fldl      24(%rsp)
        movsd     %xmm4, 16(%rsp)
        fldl      16(%rsp)
        addl      $1023, %edx
        movd      %edx, %xmm4
        psllq     $52, %xmm4
        faddp     %st, %st(1)
        fmul      %st, %st(1)
        faddp     %st, %st(1)
        movsd     %xmm4, 24(%rsp)
        fldl      24(%rsp)
        fmulp     %st, %st(1)
        fstpl     16(%rsp)
        movsd     16(%rsp), %xmm0
        fldcw     (%rsp)
        pextrw    $3, %xmm0, %ecx
        andl      $32752, %ecx
        cmpl      $32752, %ecx
        jae       .L_2TAG_PACKET_6.0.2
        jmp       ..B1.5
        cmpl      $-2147483648, %ecx
        jb        .L_2TAG_PACKET_6.0.2
        jmp       ..B1.5
.L_2TAG_PACKET_6.0.2:
        movl      $41, 8(%rsp)
        jmp       .L_2TAG_PACKET_7.0.2
.L_2TAG_PACKET_8.0.2:
        cmpl      $2146435072, %eax
        jae       .L_2TAG_PACKET_9.0.2
        movsd     XMAX(%rip), %xmm0
        mulsd     %xmm0, %xmm0
        movl      $41, 8(%rsp)
        jmp       .L_2TAG_PACKET_7.0.2
.L_2TAG_PACKET_9.0.2:
        movl      36(%rsp), %eax
        movl      32(%rsp), %edx
        movl      %eax, %ecx
        andl      $2147483647, %eax
        cmpl      $2146435072, %eax
        ja        .L_2TAG_PACKET_10.0.2
        cmpl      $0, %edx
        jne       .L_2TAG_PACKET_10.0.2
        cmpl      $0, %ecx
        jl        .L_2TAG_PACKET_11.0.2
        movq      INF(%rip), %xmm0
        jmp       ..B1.5
.L_2TAG_PACKET_11.0.2:
        jmp       .L_2TAG_PACKET_5.0.2
.L_2TAG_PACKET_10.0.2:
        movsd     32(%rsp), %xmm0
        addsd     %xmm0, %xmm0
        jmp       ..B1.5
.L_2TAG_PACKET_12.0.2:
        addl      $16304, %eax
        cmpl      $15504, %eax
        jb        .L_2TAG_PACKET_13.0.2
        movapd    cvl(%rip), %xmm2
        pshufd    $68, %xmm0, %xmm1
        movapd    16+cvl(%rip), %xmm3
        movapd    32+cvl(%rip), %xmm4
        movq      48+cvl(%rip), %xmm5
        mulsd     %xmm1, %xmm1
        xorpd     %xmm6, %xmm6
        movl      $16352, %eax
        pinsrw    $3, %eax, %xmm6
        mulpd     %xmm0, %xmm2
        xorpd     %xmm7, %xmm7
        movl      $16368, %edx
        pinsrw    $3, %edx, %xmm7
        addpd     %xmm3, %xmm2
        mulsd     %xmm1, %xmm5
        pshufd    $228, %xmm1, %xmm3
        mulpd     %xmm1, %xmm1
        mulsd     %xmm0, %xmm6
        mulpd     %xmm0, %xmm2
        addpd     %xmm4, %xmm2
        movq      %xmm7, %xmm4
        addsd     %xmm6, %xmm7
        mulpd     %xmm3, %xmm1
        psrlq     $27, %xmm7
        psllq     $27, %xmm7
        movq      HIGHMASK(%rip), %xmm3
        subsd     %xmm7, %xmm4
        mulpd     %xmm1, %xmm2
        addsd     %xmm4, %xmm6
        pshufd    $238, %xmm2, %xmm1
        addsd     %xmm2, %xmm6
        andpd     %xmm0, %xmm3
        movq      %xmm0, %xmm4
        addsd     %xmm6, %xmm1
        subsd     %xmm3, %xmm0
        addsd     %xmm5, %xmm1
        mulsd     %xmm7, %xmm3
        mulsd     %xmm7, %xmm0
        mulsd     %xmm1, %xmm4
        addsd     %xmm4, %xmm0
        addsd     %xmm3, %xmm0
        jmp       ..B1.5
.L_2TAG_PACKET_13.0.2:
        cmpl      $16, %eax
        jae       .L_2TAG_PACKET_3.0.2
        movq      %xmm0, %xmm2
        movd      %xmm0, %eax
        psrlq     $31, %xmm2
        movd      %xmm2, %ecx
        orl       %ecx, %eax
        je        .L_2TAG_PACKET_3.0.2
        movl      $16, %edx
        xorpd     %xmm1, %xmm1
        pinsrw    $3, %edx, %xmm1
        mulsd     %xmm1, %xmm1
        movl      $42, 8(%rsp)
        jmp       .L_2TAG_PACKET_7.0.2
.L_2TAG_PACKET_0.0.2:
        cmpl      $0, %eax
        jl        .L_2TAG_PACKET_12.0.2
        movl      36(%rsp), %eax
        cmpl      $1083179008, %eax
        jge       .L_2TAG_PACKET_8.0.2
        cmpl      $-1048576, %eax
        jae       .L_2TAG_PACKET_9.0.2
.L_2TAG_PACKET_5.0.2:
        xorpd     %xmm0, %xmm0
        movl      $49136, %eax
        pinsrw    $3, %eax, %xmm0
        jmp       ..B1.5
.L_2TAG_PACKET_7.0.2:
        movq      %xmm0, 40(%rsp)
..B1.3:
        movq      40(%rsp), %xmm0
.L_2TAG_PACKET_14.0.2:
..B1.5:
        addq      $56, %rsp
..___tag_value_expm1.4:
        ret       
..___tag_value_expm1.5:
END(expm1)
# -- End  expm1
	.section .rodata, "a"
	.align 16
	.align 16
cv:
	.long	1697350398
	.long	1079448903
	.long	1697350398
	.long	1079448903
	.long	4277796864
	.long	1065758274
	.long	4277796864
	.long	1065758274
	.long	3164486458
	.long	1025308570
	.long	3164486458
	.long	1025308570
	.long	1963358694
	.long	1065423121
	.long	1431655765
	.long	1069897045
	.long	1431655765
	.long	1067799893
	.long	0
	.long	1071644672
	.long	381774871
	.long	1062650220
	.long	381774871
	.long	1062650220
	.type	cv,@object
	.size	cv,96
	.align 16
Shifter:
	.long	0
	.long	1127743488
	.long	0
	.long	1127743488
	.type	Shifter,@object
	.size	Shifter,16
	.align 16
Tbl_addr:
	.long	0
	.long	0
	.long	0
	.long	0
	.long	1000070955
	.long	1042145304
	.long	1040187392
	.long	11418
	.long	988267849
	.long	1039500660
	.long	3539992576
	.long	22960
	.long	36755401
	.long	1042114290
	.long	402653184
	.long	34629
	.long	3634769483
	.long	1042178627
	.long	1820327936
	.long	46424
	.long	2155991225
	.long	1041560680
	.long	847249408
	.long	58348
	.long	2766913307
	.long	1039293264
	.long	3489660928
	.long	70401
	.long	3651174602
	.long	1040488175
	.long	2927624192
	.long	82586
	.long	3073892131
	.long	1042240606
	.long	1006632960
	.long	94904
	.long	1328391742
	.long	1042019037
	.long	3942645760
	.long	107355
	.long	2650893825
	.long	1041903210
	.long	822083584
	.long	119943
	.long	2397289153
	.long	1041802037
	.long	2281701376
	.long	132667
	.long	430997175
	.long	1042110606
	.long	1845493760
	.long	145530
	.long	1230936525
	.long	1041801015
	.long	1702887424
	.long	158533
	.long	740675935
	.long	1040178913
	.long	4110417920
	.long	171677
	.long	3489810261
	.long	1041825986
	.long	2793406464
	.long	184965
	.long	2532600530
	.long	1040767882
	.long	167772160
	.long	198398
	.long	3542557060
	.long	1041827263
	.long	2986344448
	.long	211976
	.long	1401563777
	.long	1041061093
	.long	922746880
	.long	225703
	.long	3129406026
	.long	1041852413
	.long	880803840
	.long	239579
	.long	900993572
	.long	1039283234
	.long	1275068416
	.long	253606
	.long	2115029358
	.long	1042140042
	.long	562036736
	.long	267786
	.long	1086643152
	.long	1041785419
	.long	1610612736
	.long	282120
	.long	82864366
	.long	1041256244
	.long	3045064704
	.long	296610
	.long	2392968152
	.long	1040913683
	.long	3573547008
	.long	311258
	.long	2905856183
	.long	1040002214
	.long	1988100096
	.long	326066
	.long	3742008261
	.long	1040011137
	.long	1451229184
	.long	341035
	.long	863393794
	.long	1040880621
	.long	914358272
	.long	356167
	.long	1446136837
	.long	1041372426
	.long	3707764736
	.long	371463
	.long	927855201
	.long	1040617636
	.long	360710144
	.long	386927
	.long	1492679939
	.long	1041050306
	.long	2952790016
	.long	402558
	.long	608827001
	.long	1041582217
	.long	2181038080
	.long	418360
	.long	606260204
	.long	1042271987
	.long	1711276032
	.long	434334
	.long	3163044019
	.long	1041843851
	.long	1006632960
	.long	450482
	.long	4148747325
	.long	1041962972
	.long	3900702720
	.long	466805
	.long	802924201
	.long	1041275378
	.long	1442840576
	.long	483307
	.long	3052749833
	.long	1041940577
	.long	1937768448
	.long	499988
	.long	2216116399
	.long	1041486744
	.long	914358272
	.long	516851
	.long	2729697836
	.long	1041445764
	.long	2566914048
	.long	533897
	.long	540608356
	.long	1041310907
	.long	2600468480
	.long	551129
	.long	2916344493
	.long	1040535661
	.long	1107296256
	.long	568549
	.long	731391814
	.long	1039497014
	.long	2566914048
	.long	586158
	.long	1024722704
	.long	1041461625
	.long	2961178624
	.long	603959
	.long	3806831748
	.long	1041732499
	.long	2675965952
	.long	621954
	.long	238953304
	.long	1040316488
	.long	2189426688
	.long	640145
	.long	749123235
	.long	1041725785
	.long	2063597568
	.long	658534
	.long	1168187977
	.long	1041175214
	.long	2986344448
	.long	677123
	.long	3506096399
	.long	1042186095
	.long	1426063360
	.long	695915
	.long	1470221620
	.long	1041675499
	.long	2566914048
	.long	714911
	.long	3182425146
	.long	1041483134
	.long	3087007744
	.long	734114
	.long	3131698208
	.long	1042208657
	.long	4068474880
	.long	753526
	.long	2300504125
	.long	1041428596
	.long	2415919104
	.long	773150
	.long	2290297931
	.long	1037388400
	.long	3716153344
	.long	792987
	.long	3532148223
	.long	1041626194
	.long	771751936
	.long	813041
	.long	1161884404
	.long	1042015258
	.long	3699376128
	.long	833312
	.long	876383176
	.long	1037968878
	.long	1241513984
	.long	853805
	.long	3379986796
	.long	1042213153
	.long	3699376128
	.long	874520
	.long	1545797737
	.long	1041681569
	.long	58720256
	.long	895462
	.long	2925146801
	.long	1042212567
	.long	855638016
	.long	916631
	.long	1316627971
	.long	1038516204
	.long	3883925504
	.long	938030
	.long	3267869137
	.long	1040337004
	.long	2726297600
	.long	959663
	.long	3720868999
	.long	1041782409
	.long	3992977408
	.long	981531
	.long	433316142
	.long	1041994064
	.long	1526726656
	.long	1003638
	.long	781232103
	.long	1040093400
	.long	2172649472
	.long	1025985
	.type	Tbl_addr,@object
	.size	Tbl_addr,1024
	.align 16
mmask:
	.long	4294967232
	.long	0
	.long	4294967232
	.long	0
	.type	mmask,@object
	.size	mmask,16
	.align 16
bias:
	.long	65472
	.long	0
	.long	65472
	.long	0
	.type	bias,@object
	.size	bias,16
	.align 16
emask:
	.long	0
	.long	4293918720
	.long	0
	.long	4293918720
	.type	emask,@object
	.size	emask,16
	.align 16
cvl:
	.long	2773927732
	.long	1053236707
	.long	381774871
	.long	1062650220
	.long	379653899
	.long	1056571845
	.long	286331153
	.long	1065423121
	.long	436314138
	.long	1059717536
	.long	1431655765
	.long	1067799893
	.long	1431655765
	.long	1069897045
	.long	0
	.long	1071644672
	.type	cvl,@object
	.size	cvl,64
	.align 8
XMAX:
	.long	4294967295
	.long	2146435071
	.type	XMAX,@object
	.size	XMAX,8
	.align 8
INF:
	.long	0
	.long	2146435072
	.type	INF,@object
	.size	INF,8
	.align 8
HIGHMASK:
	.long	4227858432
	.long	4294967295
	.type	HIGHMASK,@object
	.size	HIGHMASK,8
	.data
	.section .note.GNU-stack, ""
// -- Begin DWARF2 SEGMENT .eh_frame
	.section .eh_frame,"a",@progbits
.eh_frame_seg:
	.align 1
	.4byte 0x00000014
	.8byte 0x00527a0100000000
	.8byte 0x08070c1b01107801
	.4byte 0x00000190
	.4byte 0x0000001c
	.4byte 0x0000001c
	.4byte ..___tag_value_expm1.1-.
	.4byte ..___tag_value_expm1.5-..___tag_value_expm1.1
	.2byte 0x0400
	.4byte ..___tag_value_expm1.3-..___tag_value_expm1.1
	.2byte 0x400e
	.byte 0x04
	.4byte ..___tag_value_expm1.4-..___tag_value_expm1.3
	.2byte 0x080e
	.byte 0x00
# End
