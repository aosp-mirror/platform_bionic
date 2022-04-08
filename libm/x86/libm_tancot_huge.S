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

# -- Begin  __libm_tancot_huge
	.text
       .align    16,0x90
	.hidden __libm_tancot_huge
	.globl __libm_tancot_huge
__libm_tancot_huge:
# parameter 1: 8 + %ebp
# parameter 2: 16 + %ebp
# parameter 3: 20 + %ebp
..B1.1:
        pushl     %ebp
        movl      %esp, %ebp
        andl      $-64, %esp
        pushl     %esi
        pushl     %edi
        pushl     %ebx
        subl      $52, %esp
        movl      16(%ebp), %eax
        movl      20(%ebp), %ebx
        movl      %eax, 40(%esp)
..B1.2:
        fnstcw    38(%esp)
..B1.3:
        movl      12(%ebp), %edx
        movl      %edx, %eax
        andl      $2147483647, %eax
        shrl      $31, %edx
        movl      %edx, 44(%esp)
        cmpl      $1104150528, %eax
        call      ..L2
..L2:
        popl      %esi
        lea       _GLOBAL_OFFSET_TABLE_+[. - ..L2](%esi), %esi
        jae       ..B1.11
..B1.4:
        movsd     8(%ebp), %xmm1
        movzwl    38(%esp), %ecx
        movl      %ecx, %edx
        andl      $768, %edx
        andps     .L_2il0floatpacket.0@GOTOFF(%esi), %xmm1
        cmpl      $768, %edx
        movsd     _Pi4Inv@GOTOFF(%esi), %xmm0
        mulsd     %xmm1, %xmm0
        movsd     %xmm1, 8(%ebp)
        movsd     %xmm0, (%esp)
        je        ..B1.39
..B1.5:
        orl       $-64768, %ecx
        movw      %cx, 36(%esp)
..B1.6:
        fldcw     36(%esp)
..B1.7:
        movsd     8(%ebp), %xmm1
        movl      $1, %edi
..B1.8:
        movl      %esi, 12(%esp)
        movl      4(%esp), %esi
        movl      %esi, %edx
        movl      %edi, 24(%esp)
        movl      %esi, %edi
        shrl      $20, %edi
        andl      $1048575, %edx
        movl      %edi, %ecx
        orl       $1048576, %edx
        negl      %ecx
        addl      $13, %edi
        movl      %ebx, 8(%esp)
        addl      $19, %ecx
        movl      %edx, %ebx
        movl      %ecx, 28(%esp)
        shrl      %cl, %ebx
        movl      %edi, %ecx
        shll      %cl, %edx
        movl      28(%esp), %ecx
        movl      (%esp), %edi
        shrl      %cl, %edi
        orl       %edi, %edx
        cmpl      $1094713344, %esi
        movsd     %xmm1, 16(%esp)
        fldl      16(%esp)
        cmovb     %ebx, %edx
        movl      24(%esp), %edi
        movl      12(%esp), %esi
        lea       1(%edx), %ebx
        andl      $-2, %ebx
        movl      %ebx, 16(%esp)
        cmpl      $1094713344, %eax
        fildl     16(%esp)
        movl      8(%esp), %ebx
        jae       ..B1.10
..B1.9:
        fldl      _Pi4x3@GOTOFF(%esi)
        fmul      %st(1), %st
        faddp     %st, %st(2)
        fldl      8+_Pi4x3@GOTOFF(%esi)
        fmul      %st(1), %st
        faddp     %st, %st(2)
        fldl      16+_Pi4x3@GOTOFF(%esi)
        fmulp     %st, %st(1)
        faddp     %st, %st(1)
        jmp       ..B1.17
..B1.10:
        fldl      _Pi4x4@GOTOFF(%esi)
        fmul      %st(1), %st
        faddp     %st, %st(2)
        fldl      8+_Pi4x4@GOTOFF(%esi)
        fmul      %st(1), %st
        faddp     %st, %st(2)
        fldl      16+_Pi4x4@GOTOFF(%esi)
        fmul      %st(1), %st
        faddp     %st, %st(2)
        fldl      24+_Pi4x4@GOTOFF(%esi)
        fmulp     %st, %st(1)
        faddp     %st, %st(1)
        jmp       ..B1.17
..B1.11:
        movzwl    38(%esp), %edx
        movl      %edx, %eax
        andl      $768, %eax
        cmpl      $768, %eax
        je        ..B1.40
..B1.12:
        orl       $-64768, %edx
        movw      %dx, 36(%esp)
..B1.13:
        fldcw     36(%esp)
..B1.14:
        movl      $1, %edi
..B1.15:
        movsd     8(%ebp), %xmm0
        addl      $-32, %esp
        andps     .L_2il0floatpacket.0@GOTOFF(%esi), %xmm0
        lea       32(%esp), %eax
        movsd     %xmm0, 16(%eax)
        fldl      16(%eax)
        fstpt     (%esp)
        movl      $0, 12(%esp)
        movl      %eax, 16(%esp)
        call      __libm_reduce_pi04l
..B1.43:
        movl      %eax, %edx
        addl      $32, %esp
..B1.16:
        fldl      (%esp)
        fldl      8(%esp)
        faddp     %st, %st(1)
..B1.17:
        movl      %ebx, %eax
        andl      $3, %eax
        cmpl      $3, %eax
        jne       ..B1.24
..B1.18:
        fldl      _ones@GOTOFF(%esi)
        incl      %edx
        fdiv      %st(1), %st
        testb     $2, %dl
        fstpt     24(%esp)
        fld       %st(0)
        fmul      %st(1), %st
        fld       %st(0)
        fmul      %st(1), %st
        fldt      36+_TP@GOTOFF(%esi)
        fmul      %st(2), %st
        fldt      24+_TP@GOTOFF(%esi)
        faddp     %st, %st(1)
        fmul      %st(2), %st
        fldt      12+_TP@GOTOFF(%esi)
        faddp     %st, %st(1)
        fmul      %st(2), %st
        fldt      36+_TQ@GOTOFF(%esi)
        fmul      %st(3), %st
        fldt      24+_TQ@GOTOFF(%esi)
        faddp     %st, %st(1)
        fmul      %st(3), %st
        fldt      12+_TQ@GOTOFF(%esi)
        faddp     %st, %st(1)
        fmul      %st(3), %st
        fldt      _TQ@GOTOFF(%esi)
        faddp     %st, %st(1)
        fldt      _TP@GOTOFF(%esi)
        faddp     %st, %st(2)
        fldt      132+_GP@GOTOFF(%esi)
        fmul      %st(3), %st
        fldt      120+_GP@GOTOFF(%esi)
        fmul      %st(4), %st
        fldt      108+_GP@GOTOFF(%esi)
        faddp     %st, %st(2)
        fxch      %st(1)
        fmul      %st(4), %st
        fldt      96+_GP@GOTOFF(%esi)
        faddp     %st, %st(2)
        fxch      %st(1)
        fmul      %st(4), %st
        fldt      84+_GP@GOTOFF(%esi)
        faddp     %st, %st(2)
        fxch      %st(1)
        fmul      %st(4), %st
        fldt      72+_GP@GOTOFF(%esi)
        faddp     %st, %st(2)
        fxch      %st(1)
        fmul      %st(4), %st
        fldt      60+_GP@GOTOFF(%esi)
        faddp     %st, %st(2)
        fxch      %st(1)
        fmul      %st(4), %st
        fldt      48+_GP@GOTOFF(%esi)
        faddp     %st, %st(2)
        fxch      %st(1)
        fmul      %st(4), %st
        fldt      36+_GP@GOTOFF(%esi)
        faddp     %st, %st(2)
        fxch      %st(1)
        fmul      %st(4), %st
        fldt      24+_GP@GOTOFF(%esi)
        faddp     %st, %st(2)
        fxch      %st(1)
        fmulp     %st, %st(4)
        fldt      12+_GP@GOTOFF(%esi)
        faddp     %st, %st(1)
        fmul      %st(4), %st
        fmul      %st(5), %st
        fldt      _GP@GOTOFF(%esi)
        faddp     %st, %st(4)
        fxch      %st(3)
        fmul      %st(5), %st
        faddp     %st, %st(3)
        je        ..B1.20
..B1.19:
        fldt      24(%esp)
        fxch      %st(1)
        fdivrp    %st, %st(2)
        fxch      %st(1)
        fmulp     %st, %st(3)
        movl      44(%esp), %eax
        xorl      $1, %eax
        fxch      %st(2)
        fmul      %st(3), %st
        fldl      _ones@GOTOFF(%esi,%eax,8)
        fmul      %st, %st(2)
        fmul      %st, %st(3)
        fxch      %st(3)
        faddp     %st, %st(2)
        fxch      %st(1)
        fstpl     16(%esp)
        fmul      %st(1), %st
        fxch      %st(1)
        fmulp     %st, %st(2)
        movsd     16(%esp), %xmm0
        faddp     %st, %st(1)
        fstpl     16(%esp)
        movsd     16(%esp), %xmm1
        jmp       ..B1.21
..B1.20:
        fdivrp    %st, %st(1)
        fmulp     %st, %st(2)
        fxch      %st(1)
        fmul      %st(2), %st
        movl      44(%esp), %eax
        fldl      _ones@GOTOFF(%esi,%eax,8)
        fmul      %st, %st(1)
        fmul      %st, %st(3)
        fxch      %st(3)
        faddp     %st, %st(1)
        fstpl     16(%esp)
        fmul      %st(1), %st
        fldt      24(%esp)
        fmulp     %st, %st(2)
        movsd     16(%esp), %xmm0
        faddp     %st, %st(1)
        fstpl     16(%esp)
        movsd     16(%esp), %xmm1
..B1.21:
        testl     %edi, %edi
        je        ..B1.23
..B1.22:
        fldcw     38(%esp)
..B1.23:
        movl      40(%esp), %eax
        movsd     %xmm0, (%eax)
        movsd     %xmm1, 8(%eax)
        addl      $52, %esp
        popl      %ebx
        popl      %edi
        popl      %esi
        movl      %ebp, %esp
        popl      %ebp
        ret       
..B1.24:
        testb     $2, %bl
        je        ..B1.31
..B1.25:
        incl      %edx
        fld       %st(0)
        fmul      %st(1), %st
        testb     $2, %dl
        je        ..B1.27
..B1.26:
        fldl      _ones@GOTOFF(%esi)
        fdiv      %st(2), %st
        fld       %st(1)
        fmul      %st(2), %st
        fldt      132+_GP@GOTOFF(%esi)
        fmul      %st(1), %st
        fldt      120+_GP@GOTOFF(%esi)
        fmul      %st(2), %st
        fldt      108+_GP@GOTOFF(%esi)
        movl      44(%esp), %eax
        faddp     %st, %st(2)
        fxch      %st(1)
        fmul      %st(2), %st
        xorl      $1, %eax
        fldt      96+_GP@GOTOFF(%esi)
        faddp     %st, %st(2)
        fxch      %st(1)
        fmul      %st(2), %st
        fldt      84+_GP@GOTOFF(%esi)
        faddp     %st, %st(2)
        fxch      %st(1)
        fmul      %st(2), %st
        fldt      72+_GP@GOTOFF(%esi)
        faddp     %st, %st(2)
        fxch      %st(1)
        fmul      %st(2), %st
        fldt      60+_GP@GOTOFF(%esi)
        faddp     %st, %st(2)
        fxch      %st(1)
        fmul      %st(2), %st
        fldt      48+_GP@GOTOFF(%esi)
        faddp     %st, %st(2)
        fxch      %st(1)
        fmul      %st(2), %st
        fldt      36+_GP@GOTOFF(%esi)
        faddp     %st, %st(2)
        fxch      %st(1)
        fmul      %st(2), %st
        fldt      24+_GP@GOTOFF(%esi)
        faddp     %st, %st(2)
        fxch      %st(1)
        fmulp     %st, %st(2)
        fldt      12+_GP@GOTOFF(%esi)
        faddp     %st, %st(1)
        fmulp     %st, %st(3)
        fldt      _GP@GOTOFF(%esi)
        faddp     %st, %st(1)
        fmul      %st(3), %st
        fxch      %st(2)
        fmulp     %st, %st(3)
        fxch      %st(1)
        faddp     %st, %st(2)
        fldl      _ones@GOTOFF(%esi,%eax,8)
        fmul      %st, %st(2)
        fmulp     %st, %st(1)
        faddp     %st, %st(1)
        fstpl     16(%esp)
        movsd     16(%esp), %xmm0
        jmp       ..B1.28
..B1.27:
        fldt      36+_TP@GOTOFF(%esi)
        fmul      %st(1), %st
        fldt      24+_TP@GOTOFF(%esi)
        movl      44(%esp), %eax
        faddp     %st, %st(1)
        fmul      %st(1), %st
        fldt      36+_TQ@GOTOFF(%esi)
        fmul      %st(2), %st
        fldt      24+_TQ@GOTOFF(%esi)
        faddp     %st, %st(1)
        fmul      %st(2), %st
        fldt      12+_TQ@GOTOFF(%esi)
        faddp     %st, %st(1)
        fmul      %st(2), %st
        fldt      _TQ@GOTOFF(%esi)
        faddp     %st, %st(1)
        fldt      12+_TP@GOTOFF(%esi)
        faddp     %st, %st(2)
        fxch      %st(1)
        fmul      %st(2), %st
        fldt      _TP@GOTOFF(%esi)
        faddp     %st, %st(1)
        fdivp     %st, %st(1)
        fmulp     %st, %st(1)
        fmul      %st(1), %st
        fldl      _ones@GOTOFF(%esi,%eax,8)
        fmul      %st, %st(1)
        fmulp     %st, %st(2)
        faddp     %st, %st(1)
        fstpl     16(%esp)
        movsd     16(%esp), %xmm0
..B1.28:
        testl     %edi, %edi
        je        ..B1.30
..B1.29:
        fldcw     38(%esp)
..B1.30:
        movl      40(%esp), %eax
        movsd     %xmm0, (%eax)
        addl      $52, %esp
        popl      %ebx
        popl      %edi
        popl      %esi
        movl      %ebp, %esp
        popl      %ebp
        ret       
..B1.31:
        testb     $1, %bl
        je        ..B1.38
..B1.32:
        incl      %edx
        fld       %st(0)
        fmul      %st(1), %st
        testb     $2, %dl
        je        ..B1.34
..B1.33:
        fldt      36+_TP@GOTOFF(%esi)
        fmul      %st(1), %st
        fldt      24+_TP@GOTOFF(%esi)
        movl      44(%esp), %eax
        faddp     %st, %st(1)
        fmul      %st(1), %st
        xorl      $1, %eax
        fldt      36+_TQ@GOTOFF(%esi)
        fmul      %st(2), %st
        fldt      24+_TQ@GOTOFF(%esi)
        faddp     %st, %st(1)
        fmul      %st(2), %st
        fldt      12+_TQ@GOTOFF(%esi)
        faddp     %st, %st(1)
        fmul      %st(2), %st
        fldt      _TQ@GOTOFF(%esi)
        faddp     %st, %st(1)
        fldt      12+_TP@GOTOFF(%esi)
        faddp     %st, %st(2)
        fxch      %st(1)
        fmul      %st(2), %st
        fldt      _TP@GOTOFF(%esi)
        faddp     %st, %st(1)
        fdivp     %st, %st(1)
        fmulp     %st, %st(1)
        fmul      %st(1), %st
        fldl      _ones@GOTOFF(%esi,%eax,8)
        fmul      %st, %st(1)
        fmulp     %st, %st(2)
        faddp     %st, %st(1)
        fstpl     16(%esp)
        movsd     16(%esp), %xmm0
        jmp       ..B1.35
..B1.34:
        fldl      _ones@GOTOFF(%esi)
        fdiv      %st(2), %st
        fld       %st(1)
        fmul      %st(2), %st
        fldt      132+_GP@GOTOFF(%esi)
        fmul      %st(1), %st
        fldt      120+_GP@GOTOFF(%esi)
        fmul      %st(2), %st
        fldt      108+_GP@GOTOFF(%esi)
        movl      44(%esp), %eax
        faddp     %st, %st(2)
        fxch      %st(1)
        fmul      %st(2), %st
        fldt      96+_GP@GOTOFF(%esi)
        faddp     %st, %st(2)
        fxch      %st(1)
        fmul      %st(2), %st
        fldt      84+_GP@GOTOFF(%esi)
        faddp     %st, %st(2)
        fxch      %st(1)
        fmul      %st(2), %st
        fldt      72+_GP@GOTOFF(%esi)
        faddp     %st, %st(2)
        fxch      %st(1)
        fmul      %st(2), %st
        fldt      60+_GP@GOTOFF(%esi)
        faddp     %st, %st(2)
        fxch      %st(1)
        fmul      %st(2), %st
        fldt      48+_GP@GOTOFF(%esi)
        faddp     %st, %st(2)
        fxch      %st(1)
        fmul      %st(2), %st
        fldt      36+_GP@GOTOFF(%esi)
        faddp     %st, %st(2)
        fxch      %st(1)
        fmul      %st(2), %st
        fldt      24+_GP@GOTOFF(%esi)
        faddp     %st, %st(2)
        fxch      %st(1)
        fmulp     %st, %st(2)
        fldt      12+_GP@GOTOFF(%esi)
        faddp     %st, %st(1)
        fmulp     %st, %st(3)
        fldt      _GP@GOTOFF(%esi)
        faddp     %st, %st(1)
        fmul      %st(3), %st
        fxch      %st(2)
        fmulp     %st, %st(3)
        fxch      %st(1)
        faddp     %st, %st(2)
        fldl      _ones@GOTOFF(%esi,%eax,8)
        fmul      %st, %st(2)
        fmulp     %st, %st(1)
        faddp     %st, %st(1)
        fstpl     16(%esp)
        movsd     16(%esp), %xmm0
..B1.35:
        testl     %edi, %edi
        je        ..B1.37
..B1.36:
        fldcw     38(%esp)
..B1.37:
        movl      40(%esp), %eax
        movsd     %xmm0, 8(%eax)
        addl      $52, %esp
        popl      %ebx
        popl      %edi
        popl      %esi
        movl      %ebp, %esp
        popl      %ebp
        ret       
..B1.38:
        fstp      %st(0)
        addl      $52, %esp
        popl      %ebx
        popl      %edi
        popl      %esi
        movl      %ebp, %esp
        popl      %ebp
        ret       
..B1.39:
        xorl      %edi, %edi
        jmp       ..B1.8
..B1.40:
        xorl      %edi, %edi
        jmp       ..B1.15
        .align    16,0x90
	.type	__libm_tancot_huge,@function
	.size	__libm_tancot_huge,.-__libm_tancot_huge
	.data
# -- End  __libm_tancot_huge
	.section .rodata, "a"
	.align 16
	.align 16
.L_2il0floatpacket.0:
	.long	0xffffffff,0x7fffffff,0x00000000,0x00000000
	.type	.L_2il0floatpacket.0,@object
	.size	.L_2il0floatpacket.0,16
	.align 16
_Pi4Inv:
	.long	1841940611
	.long	1072979760
	.type	_Pi4Inv,@object
	.size	_Pi4Inv,8
	.space 8, 0x00 	# pad
	.align 16
_Pi4x3:
	.long	1413754880
	.long	3219726843
	.long	993632256
	.long	1027030475
	.long	3773204808
	.long	3129236486
	.type	_Pi4x3,@object
	.size	_Pi4x3,24
	.space 8, 0x00 	# pad
	.align 16
_Pi4x4:
	.long	1413480448
	.long	3219726843
	.long	442499072
	.long	3183522913
	.long	771751936
	.long	3146979722
	.long	622873025
	.long	3110831002
	.type	_Pi4x4,@object
	.size	_Pi4x4,32
	.align 16
_ones:
	.long	0
	.long	1072693248
	.long	0
	.long	3220176896
	.type	_ones,@object
	.size	_ones,16
	.align 16
_TP:
	.word	19670
	.word	44908
	.word	50960
	.word	50786
	.word	49149
	.word	0
	.word	19206
	.word	45228
	.word	54194
	.word	52268
	.word	16377
	.word	0
	.word	227
	.word	51280
	.word	43560
	.word	38195
	.word	49139
	.word	0
	.word	12272
	.word	18029
	.word	6715
	.word	45670
	.word	16357
	.word	0
	.type	_TP,@object
	.size	_TP,48
	.align 16
_TQ:
	.word	14748
	.word	33681
	.word	5452
	.word	38090
	.word	49151
	.word	0
	.word	46755
	.word	50026
	.word	17634
	.word	35372
	.word	16382
	.word	0
	.word	46863
	.word	53352
	.word	42702
	.word	59869
	.word	49145
	.word	0
	.word	33295
	.word	20942
	.word	32118
	.word	39935
	.word	16371
	.word	0
	.type	_TQ,@object
	.size	_TQ,48
	.align 16
_GP:
	.word	43691
	.word	43690
	.word	43690
	.word	43690
	.word	49149
	.word	0
	.word	46639
	.word	2912
	.word	24758
	.word	46603
	.word	49145
	.word	0
	.word	57255
	.word	2218
	.word	21984
	.word	35507
	.word	49142
	.word	0
	.word	34208
	.word	43033
	.word	48281
	.word	56811
	.word	49138
	.word	0
	.word	28773
	.word	27191
	.word	31071
	.word	45908
	.word	49135
	.word	0
	.word	43257
	.word	33777
	.word	11976
	.word	37184
	.word	49132
	.word	0
	.word	62410
	.word	35990
	.word	36363
	.word	60269
	.word	49128
	.word	0
	.word	13659
	.word	55568
	.word	26569
	.word	48851
	.word	49125
	.word	0
	.word	10347
	.word	46238
	.word	47188
	.word	39576
	.word	49122
	.word	0
	.word	2161
	.word	6703
	.word	25719
	.word	64708
	.word	49118
	.word	0
	.word	42329
	.word	7593
	.word	44754
	.word	47734
	.word	49115
	.word	0
	.word	163
	.word	32746
	.word	39875
	.word	61957
	.word	49112
	.word	0
	.type	_GP,@object
	.size	_GP,144
	.data
	.hidden __libm_reduce_pi04l
	.section .note.GNU-stack, ""
# End
