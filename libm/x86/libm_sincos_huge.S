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

# -- Begin  __libm_sincos_huge
	.text
       .align    16,0x90
	.hidden __libm_sincos_huge
	.globl __libm_sincos_huge
__libm_sincos_huge:
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
        movl      20(%ebp), %edx
        movl      %eax, 32(%esp)
        movl      %edx, 36(%esp)
..B1.2:
        fnstcw    30(%esp)
..B1.3:
        call      ..L2
..L2:
        popl      %edi
        lea       _GLOBAL_OFFSET_TABLE_+[. - ..L2](%edi), %edi
        movsd     8(%ebp), %xmm1
        movl      12(%ebp), %esi
        movl      %esi, %eax
        andl      $2147483647, %eax
        andps     .L_2il0floatpacket.0@GOTOFF(%edi), %xmm1
        shrl      $31, %esi
        movl      %eax, 40(%esp)
        cmpl      $1104150528, %eax
        movsd     %xmm1, 8(%ebp)
        jae       ..B1.11
..B1.4:
        movsd     _Pi4Inv@GOTOFF(%edi), %xmm0
        mulsd     %xmm1, %xmm0
        movzwl    30(%esp), %edx
        movl      %edx, %eax
        andl      $768, %eax
        movsd     %xmm0, (%esp)
        cmpl      $768, %eax
        je        ..B1.42
..B1.5:
        orl       $-64768, %edx
        movw      %dx, 28(%esp)
..B1.6:
        fldcw     28(%esp)
..B1.7:
        movsd     8(%ebp), %xmm1
        movl      $1, %ebx
..B1.8:
        movl      %ebx, 12(%esp)
        movl      4(%esp), %ebx
        movl      %ebx, %eax
        movl      %esi, 8(%esp)
        movl      %ebx, %esi
        shrl      $20, %esi
        andl      $1048575, %eax
        movl      %esi, %ecx
        orl       $1048576, %eax
        negl      %ecx
        movl      %eax, %edx
        addl      $19, %ecx
        addl      $13, %esi
        movl      %ecx, 24(%esp)
        shrl      %cl, %edx
        movl      %esi, %ecx
        shll      %cl, %eax
        movl      24(%esp), %ecx
        movl      (%esp), %esi
        shrl      %cl, %esi
        orl       %esi, %eax
        cmpl      $1094713344, %ebx
        movsd     %xmm1, 16(%esp)
        fldl      16(%esp)
        cmovb     %edx, %eax
        movl      8(%esp), %esi
        lea       1(%eax), %edx
        movl      %edx, %ebx
        andl      $-2, %ebx
        movl      %ebx, 16(%esp)
        fildl     16(%esp)
        movl      12(%esp), %ebx
        cmpl      $1094713344, 40(%esp)
        jae       ..B1.10
..B1.9:
        fldl      _Pi4x3@GOTOFF(%edi)
        fmul      %st(1), %st
        faddp     %st, %st(2)
        fldl      8+_Pi4x3@GOTOFF(%edi)
        fmul      %st(1), %st
        faddp     %st, %st(2)
        fldl      16+_Pi4x3@GOTOFF(%edi)
        fmulp     %st, %st(1)
        faddp     %st, %st(1)
        jmp       ..B1.17
..B1.10:
        fldl      _Pi4x4@GOTOFF(%edi)
        fmul      %st(1), %st
        faddp     %st, %st(2)
        fldl      8+_Pi4x4@GOTOFF(%edi)
        fmul      %st(1), %st
        faddp     %st, %st(2)
        fldl      16+_Pi4x4@GOTOFF(%edi)
        fmul      %st(1), %st
        faddp     %st, %st(2)
        fldl      24+_Pi4x4@GOTOFF(%edi)
        fmulp     %st, %st(1)
        faddp     %st, %st(1)
        jmp       ..B1.17
..B1.11:
        movzwl    30(%esp), %edx
        movl      %edx, %eax
        andl      $768, %eax
        cmpl      $768, %eax
        je        ..B1.43
..B1.12:
        orl       $-64768, %edx
        movw      %dx, 28(%esp)
..B1.13:
        fldcw     28(%esp)
..B1.14:
        movsd     8(%ebp), %xmm1
        movl      $1, %ebx
..B1.15:
        movsd     %xmm1, 16(%esp)
        fldl      16(%esp)
        addl      $-32, %esp
        lea       32(%esp), %eax
        fstpt     (%esp)
        movl      $0, 12(%esp)
        movl      %eax, 16(%esp)
        call      __libm_reduce_pi04l
..B1.46:
        addl      $32, %esp
..B1.16:
        fldl      (%esp)
        lea       1(%eax), %edx
        fldl      8(%esp)
        faddp     %st, %st(1)
..B1.17:
        movl      %edx, %ecx
        addl      $3, %eax
        shrl      $2, %ecx
        andl      $1, %ecx
        shrl      $2, %eax
        xorl      %ecx, %esi
        movl      36(%esp), %ecx
        andl      $1, %eax
        andl      $3, %ecx
        cmpl      $3, %ecx
        jne       ..B1.25
..B1.18:
        fldt      84+_SP@GOTOFF(%edi)
        fld       %st(1)
        fmul      %st(2), %st
        testb     $2, %dl
        fmul      %st, %st(1)
        fldt      72+_SP@GOTOFF(%edi)
        faddp     %st, %st(2)
        fmul      %st, %st(1)
        fldt      60+_SP@GOTOFF(%edi)
        faddp     %st, %st(2)
        fmul      %st, %st(1)
        fldt      48+_SP@GOTOFF(%edi)
        faddp     %st, %st(2)
        fmul      %st, %st(1)
        fldt      36+_SP@GOTOFF(%edi)
        faddp     %st, %st(2)
        fmul      %st, %st(1)
        fldt      24+_SP@GOTOFF(%edi)
        faddp     %st, %st(2)
        fmul      %st, %st(1)
        fldt      12+_SP@GOTOFF(%edi)
        faddp     %st, %st(2)
        fmul      %st, %st(1)
        fldt      _SP@GOTOFF(%edi)
        faddp     %st, %st(2)
        fmul      %st, %st(1)
        fldt      84+_CP@GOTOFF(%edi)
        fmul      %st(1), %st
        fldt      72+_CP@GOTOFF(%edi)
        faddp     %st, %st(1)
        fmul      %st(1), %st
        fldt      60+_CP@GOTOFF(%edi)
        faddp     %st, %st(1)
        fmul      %st(1), %st
        fldt      48+_CP@GOTOFF(%edi)
        faddp     %st, %st(1)
        fmul      %st(1), %st
        fldt      36+_CP@GOTOFF(%edi)
        faddp     %st, %st(1)
        fmul      %st(1), %st
        fldt      24+_CP@GOTOFF(%edi)
        faddp     %st, %st(1)
        fmul      %st(1), %st
        fldt      12+_CP@GOTOFF(%edi)
        faddp     %st, %st(1)
        fmul      %st(1), %st
        fldt      _CP@GOTOFF(%edi)
        faddp     %st, %st(1)
        fmulp     %st, %st(1)
        fldl      _ones@GOTOFF(%edi,%esi,8)
        fldl      _ones@GOTOFF(%edi,%eax,8)
        je        ..B1.22
..B1.19:
        fmulp     %st, %st(4)
        testl     %ebx, %ebx
        fxch      %st(2)
        fmul      %st(3), %st
        movl      32(%esp), %eax
        faddp     %st, %st(3)
        fxch      %st(2)
        fstpl     (%eax)
        fmul      %st, %st(1)
        faddp     %st, %st(1)
        fstpl     8(%eax)
        je        ..B1.21
..B1.20:
        fldcw     30(%esp)
..B1.21:
        addl      $52, %esp
        popl      %ebx
        popl      %edi
        popl      %esi
        movl      %ebp, %esp
        popl      %ebp
        ret       
..B1.22:
        fxch      %st(1)
        fmulp     %st, %st(4)
        testl     %ebx, %ebx
        fxch      %st(2)
        fmul      %st(3), %st
        movl      32(%esp), %eax
        faddp     %st, %st(3)
        fxch      %st(2)
        fstpl     8(%eax)
        fmul      %st, %st(1)
        faddp     %st, %st(1)
        fstpl     (%eax)
        je        ..B1.24
..B1.23:
        fldcw     30(%esp)
..B1.24:
        addl      $52, %esp
        popl      %ebx
        popl      %edi
        popl      %esi
        movl      %ebp, %esp
        popl      %ebp
        ret       
..B1.25:
        testb     $2, 36(%esp)
        je        ..B1.33
..B1.26:
        fld       %st(0)
        testb     $2, %dl
        fmul      %st(1), %st
        fld       %st(0)
        fmul      %st(1), %st
        je        ..B1.30
..B1.27:
        fstp      %st(2)
        fldt      84+_CP@GOTOFF(%edi)
        testl     %ebx, %ebx
        fmul      %st(2), %st
        fldt      72+_CP@GOTOFF(%edi)
        fmul      %st(3), %st
        fldt      60+_CP@GOTOFF(%edi)
        movl      32(%esp), %eax
        faddp     %st, %st(2)
        fxch      %st(1)
        fmul      %st(3), %st
        fldt      48+_CP@GOTOFF(%edi)
        faddp     %st, %st(2)
        fxch      %st(1)
        fmul      %st(3), %st
        fldt      36+_CP@GOTOFF(%edi)
        faddp     %st, %st(2)
        fxch      %st(1)
        fmul      %st(3), %st
        fldt      24+_CP@GOTOFF(%edi)
        faddp     %st, %st(2)
        fxch      %st(1)
        fmul      %st(3), %st
        fldt      12+_CP@GOTOFF(%edi)
        faddp     %st, %st(2)
        fxch      %st(1)
        fmulp     %st, %st(3)
        fldt      _CP@GOTOFF(%edi)
        faddp     %st, %st(1)
        fmulp     %st, %st(1)
        faddp     %st, %st(1)
        fldl      _ones@GOTOFF(%edi,%esi,8)
        fmul      %st, %st(1)
        faddp     %st, %st(1)
        fstpl     8(%eax)
        je        ..B1.29
..B1.28:
        fldcw     30(%esp)
..B1.29:
        addl      $52, %esp
        popl      %ebx
        popl      %edi
        popl      %esi
        movl      %ebp, %esp
        popl      %ebp
        ret       
..B1.30:
        fldt      84+_SP@GOTOFF(%edi)
        testl     %ebx, %ebx
        fmul      %st(1), %st
        fldt      72+_SP@GOTOFF(%edi)
        fmul      %st(2), %st
        fldt      60+_SP@GOTOFF(%edi)
        movl      32(%esp), %eax
        faddp     %st, %st(2)
        fxch      %st(1)
        fmul      %st(2), %st
        fldt      48+_SP@GOTOFF(%edi)
        faddp     %st, %st(2)
        fxch      %st(1)
        fmul      %st(2), %st
        fldt      36+_SP@GOTOFF(%edi)
        faddp     %st, %st(2)
        fxch      %st(1)
        fmul      %st(2), %st
        fldt      24+_SP@GOTOFF(%edi)
        faddp     %st, %st(2)
        fxch      %st(1)
        fmul      %st(2), %st
        fldt      12+_SP@GOTOFF(%edi)
        faddp     %st, %st(2)
        fxch      %st(1)
        fmulp     %st, %st(2)
        fldt      _SP@GOTOFF(%edi)
        faddp     %st, %st(1)
        fmulp     %st, %st(2)
        faddp     %st, %st(1)
        fldl      _ones@GOTOFF(%edi,%esi,8)
        fmulp     %st, %st(2)
        fmul      %st(1), %st
        faddp     %st, %st(1)
        fstpl     8(%eax)
        je        ..B1.32
..B1.31:
        fldcw     30(%esp)
..B1.32:
        addl      $52, %esp
        popl      %ebx
        popl      %edi
        popl      %esi
        movl      %ebp, %esp
        popl      %ebp
        ret       
..B1.33:
        testb     $1, 36(%esp)
        je        ..B1.41
..B1.34:
        fld       %st(0)
        testb     $2, %dl
        fmul      %st(1), %st
        fld       %st(0)
        fmul      %st(1), %st
        je        ..B1.38
..B1.35:
        fldt      84+_SP@GOTOFF(%edi)
        testl     %ebx, %ebx
        fmul      %st(1), %st
        fldt      72+_SP@GOTOFF(%edi)
        fmul      %st(2), %st
        fldt      60+_SP@GOTOFF(%edi)
        faddp     %st, %st(2)
        fxch      %st(1)
        fmul      %st(2), %st
        fldt      48+_SP@GOTOFF(%edi)
        faddp     %st, %st(2)
        fxch      %st(1)
        fmul      %st(2), %st
        fldt      36+_SP@GOTOFF(%edi)
        faddp     %st, %st(2)
        fxch      %st(1)
        fmul      %st(2), %st
        fldt      24+_SP@GOTOFF(%edi)
        faddp     %st, %st(2)
        fxch      %st(1)
        fmul      %st(2), %st
        fldt      12+_SP@GOTOFF(%edi)
        faddp     %st, %st(2)
        fxch      %st(1)
        fmulp     %st, %st(2)
        fldt      _SP@GOTOFF(%edi)
        faddp     %st, %st(1)
        fmulp     %st, %st(2)
        faddp     %st, %st(1)
        fldl      _ones@GOTOFF(%edi,%eax,8)
        fmulp     %st, %st(2)
        fmul      %st(1), %st
        movl      32(%esp), %eax
        faddp     %st, %st(1)
        fstpl     (%eax)
        je        ..B1.37
..B1.36:
        fldcw     30(%esp)
..B1.37:
        addl      $52, %esp
        popl      %ebx
        popl      %edi
        popl      %esi
        movl      %ebp, %esp
        popl      %ebp
        ret       
..B1.38:
        fstp      %st(2)
        fldt      84+_CP@GOTOFF(%edi)
        testl     %ebx, %ebx
        fmul      %st(2), %st
        fldt      72+_CP@GOTOFF(%edi)
        fmul      %st(3), %st
        fldt      60+_CP@GOTOFF(%edi)
        faddp     %st, %st(2)
        fxch      %st(1)
        fmul      %st(3), %st
        fldt      48+_CP@GOTOFF(%edi)
        faddp     %st, %st(2)
        fxch      %st(1)
        fmul      %st(3), %st
        fldt      36+_CP@GOTOFF(%edi)
        faddp     %st, %st(2)
        fxch      %st(1)
        fmul      %st(3), %st
        fldt      24+_CP@GOTOFF(%edi)
        faddp     %st, %st(2)
        fxch      %st(1)
        fmul      %st(3), %st
        fldt      12+_CP@GOTOFF(%edi)
        faddp     %st, %st(2)
        fxch      %st(1)
        fmulp     %st, %st(3)
        fldt      _CP@GOTOFF(%edi)
        faddp     %st, %st(1)
        fmulp     %st, %st(1)
        faddp     %st, %st(1)
        fldl      _ones@GOTOFF(%edi,%eax,8)
        fmul      %st, %st(1)
        movl      32(%esp), %eax
        faddp     %st, %st(1)
        fstpl     (%eax)
        je        ..B1.40
..B1.39:
        fldcw     30(%esp)
..B1.40:
        addl      $52, %esp
        popl      %ebx
        popl      %edi
        popl      %esi
        movl      %ebp, %esp
        popl      %ebp
        ret       
..B1.41:
        fstp      %st(0)
        addl      $52, %esp
        popl      %ebx
        popl      %edi
        popl      %esi
        movl      %ebp, %esp
        popl      %ebp
        ret       
..B1.42:
        xorl      %ebx, %ebx
        jmp       ..B1.8
..B1.43:
        xorl      %ebx, %ebx
        jmp       ..B1.15
        .align    16,0x90
	.type	__libm_sincos_huge,@function
	.size	__libm_sincos_huge,.-__libm_sincos_huge
	.data
# -- End  __libm_sincos_huge
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
_SP:
	.word	43691
	.word	43690
	.word	43690
	.word	43690
	.word	49148
	.word	0
	.word	34951
	.word	34952
	.word	34952
	.word	34952
	.word	16376
	.word	0
	.word	50471
	.word	3328
	.word	208
	.word	53261
	.word	49138
	.word	0
	.word	17910
	.word	46614
	.word	7466
	.word	47343
	.word	16364
	.word	0
	.word	33371
	.word	14743
	.word	11071
	.word	55090
	.word	49125
	.word	0
	.word	48947
	.word	35764
	.word	12250
	.word	45202
	.word	16350
	.word	0
	.word	17574
	.word	60698
	.word	10735
	.word	55102
	.word	49110
	.word	0
	.word	34320
	.word	12415
	.word	25249
	.word	51489
	.word	16334
	.word	0
	.type	_SP,@object
	.size	_SP,96
	.align 16
_CP:
	.word	0
	.word	0
	.word	0
	.word	32768
	.word	49150
	.word	0
	.word	43685
	.word	43690
	.word	43690
	.word	43690
	.word	16378
	.word	0
	.word	39983
	.word	2912
	.word	24758
	.word	46603
	.word	49141
	.word	0
	.word	61476
	.word	3244
	.word	208
	.word	53261
	.word	16367
	.word	0
	.word	1022
	.word	16229
	.word	32187
	.word	37874
	.word	49129
	.word	0
	.word	55373
	.word	44526
	.word	50840
	.word	36726
	.word	16354
	.word	0
	.word	55994
	.word	65145
	.word	59958
	.word	51657
	.word	49114
	.word	0
	.word	15046
	.word	2976
	.word	1998
	.word	54661
	.word	16338
	.word	0
	.type	_CP,@object
	.size	_CP,96
	.align 16
_ones:
	.long	0
	.long	1072693248
	.long	0
	.long	3220176896
	.type	_ones,@object
	.size	_ones,16
	.data
	.hidden __libm_reduce_pi04l
	.section .note.GNU-stack, ""
# End
