/*
 * Copyright (C) 2016 The Android Open Source Project
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

extern char __adddf3;
extern char __addsf3;
extern char __aeabi_cdcmpeq;
extern char __aeabi_cdcmple;
extern char __aeabi_cdrcmple;
extern char __aeabi_cfcmpeq;
extern char __aeabi_cfcmple;
extern char __aeabi_cfrcmple;
extern char __aeabi_d2f;
extern char __aeabi_d2iz;
extern char __aeabi_d2uiz;
extern char __aeabi_dadd;
extern char __aeabi_dcmpeq;
extern char __aeabi_dcmpge;
extern char __aeabi_dcmpgt;
extern char __aeabi_dcmple;
extern char __aeabi_dcmplt;
extern char __aeabi_dcmpun;
extern char __aeabi_ddiv;
extern char __aeabi_dmul;
extern char __aeabi_drsub;
extern char __aeabi_dsub;
extern char __aeabi_f2d;
extern char __aeabi_f2iz;
extern char __aeabi_f2uiz;
extern char __aeabi_fadd;
extern char __aeabi_fcmpeq;
extern char __aeabi_fcmpge;
extern char __aeabi_fcmpgt;
extern char __aeabi_fcmple;
extern char __aeabi_fcmplt;
extern char __aeabi_fcmpun;
extern char __aeabi_fdiv;
extern char __aeabi_fmul;
extern char __aeabi_frsub;
extern char __aeabi_fsub;
extern char __aeabi_i2d;
extern char __aeabi_i2f;
extern char __aeabi_idiv;
extern char __aeabi_idivmod;
extern char __aeabi_l2d;
extern char __aeabi_l2f;
extern char __aeabi_lasr;
extern char __aeabi_ldivmod;
extern char __aeabi_llsl;
extern char __aeabi_llsr;
extern char __aeabi_lmul;
extern char __aeabi_ui2d;
extern char __aeabi_ui2f;
extern char __aeabi_uidiv;
extern char __aeabi_uidivmod;
extern char __aeabi_ul2d;
extern char __aeabi_ul2f;
extern char __aeabi_uldivmod;
extern char __aeabi_unwind_cpp_pr0;
extern char __aeabi_unwind_cpp_pr1;
extern char __cmpdf2;
extern char __cmpsf2;
extern char __divdf3;
extern char __divsf3;
extern char __eqdf2;
extern char __eqsf2;
extern char __extendsfdf2;
extern char __fixdfsi;
extern char __fixsfsi;
extern char __fixunsdfsi;
extern char __floatdidf;
extern char __floatdisf;
extern char __floatsidf;
extern char __floatsisf;
extern char __floatundidf;
extern char __floatundisf;
extern char __floatunsidf;
extern char __floatunsisf;
extern char __gedf2;
extern char __gesf2;
extern char __gtdf2;
extern char __gtsf2;
extern char __gnu_ldivmod_helper;
extern char __gnu_uldivmod_helper;
extern char __ledf2;
extern char __lesf2;
extern char __ltdf2;
extern char __ltsf2;
extern char __muldf3;
extern char __muldi3;
extern char __mulsf3;
extern char __nedf2;
extern char __nesf2;
extern char __popcount_tab;
extern char __popcountsi2;
extern char __subdf3;
extern char __subsf3;
extern char __truncdfsf2;
extern char __udivdi3;
extern char __unorddf2;
extern char __unordsf2;

void* __bionic_libcrt_compat_symbols[] = {
    &__adddf3,
    &__addsf3,
    &__aeabi_cdcmpeq,
    &__aeabi_cdcmple,
    &__aeabi_cdrcmple,
    &__aeabi_cfcmpeq,
    &__aeabi_cfcmple,
    &__aeabi_cfrcmple,
    &__aeabi_d2f,
    &__aeabi_d2iz,
    &__aeabi_d2uiz,
    &__aeabi_dadd,
    &__aeabi_dcmpeq,
    &__aeabi_dcmpge,
    &__aeabi_dcmpgt,
    &__aeabi_dcmple,
    &__aeabi_dcmplt,
    &__aeabi_dcmpun,
    &__aeabi_ddiv,
    &__aeabi_dmul,
    &__aeabi_drsub,
    &__aeabi_dsub,
    &__aeabi_f2d,
    &__aeabi_f2iz,
    &__aeabi_f2uiz,
    &__aeabi_fadd,
    &__aeabi_fcmpeq,
    &__aeabi_fcmpge,
    &__aeabi_fcmpgt,
    &__aeabi_fcmple,
    &__aeabi_fcmplt,
    &__aeabi_fcmpun,
    &__aeabi_fdiv,
    &__aeabi_fmul,
    &__aeabi_frsub,
    &__aeabi_fsub,
    &__aeabi_i2d,
    &__aeabi_i2f,
    &__aeabi_idiv,
    &__aeabi_idivmod,
    &__aeabi_l2d,
    &__aeabi_l2f,
    &__aeabi_lasr,
    &__aeabi_ldivmod,
    &__aeabi_llsl,
    &__aeabi_llsr,
    &__aeabi_lmul,
    &__aeabi_ui2d,
    &__aeabi_ui2f,
    &__aeabi_uidiv,
    &__aeabi_uidivmod,
    &__aeabi_ul2d,
    &__aeabi_ul2f,
    &__aeabi_uldivmod,
    &__aeabi_unwind_cpp_pr0,
    &__aeabi_unwind_cpp_pr1,
    &__cmpdf2,
    &__cmpsf2,
    &__divdf3,
    &__divsf3,
    &__eqdf2,
    &__eqsf2,
    &__extendsfdf2,
    &__fixdfsi,
    &__fixsfsi,
    &__fixunsdfsi,
    &__floatdidf,
    &__floatdisf,
    &__floatsidf,
    &__floatsisf,
    &__floatundidf,
    &__floatundisf,
    &__floatunsidf,
    &__floatunsisf,
    &__gedf2,
    &__gesf2,
    &__gtdf2,
    &__gtsf2,
    &__gnu_ldivmod_helper,
    &__gnu_uldivmod_helper,
    &__ledf2,
    &__lesf2,
    &__ltdf2,
    &__ltsf2,
    &__muldf3,
    &__muldi3,
    &__mulsf3,
    &__nedf2,
    &__nesf2,
    &__popcount_tab,
    &__popcountsi2,
    &__subdf3,
    &__subsf3,
    &__truncdfsf2,
    &__udivdi3,
    &__unorddf2,
    &__unordsf2,
};
