ifneq ($(TARGET_USE_PRIVATE_LIBM),true)
LOCAL_PATH:= $(call my-dir)

# TODO: this comes from from upstream's libc, not libm, but it's an
# implementation detail that should have hidden visibility, so it needs
# to be in whatever library the math code is in.
libm_common_src_files := \
    digittoint.c  \

# TODO: this is not in the BSDs.
libm_common_src_files += \
    significandl.c \
    sincos.c \

libm_common_src_files += \
    upstream-freebsd/lib/msun/bsdsrc/b_exp.c \
    upstream-freebsd/lib/msun/bsdsrc/b_log.c \
    upstream-freebsd/lib/msun/bsdsrc/b_tgamma.c \
    upstream-freebsd/lib/msun/src/e_acos.c \
    upstream-freebsd/lib/msun/src/e_acosf.c \
    upstream-freebsd/lib/msun/src/e_acosh.c \
    upstream-freebsd/lib/msun/src/e_acoshf.c \
    upstream-freebsd/lib/msun/src/e_asin.c \
    upstream-freebsd/lib/msun/src/e_asinf.c \
    upstream-freebsd/lib/msun/src/e_atan2.c \
    upstream-freebsd/lib/msun/src/e_atan2f.c \
    upstream-freebsd/lib/msun/src/e_atanh.c \
    upstream-freebsd/lib/msun/src/e_atanhf.c \
    upstream-freebsd/lib/msun/src/e_cosh.c \
    upstream-freebsd/lib/msun/src/e_coshf.c \
    upstream-freebsd/lib/msun/src/e_exp.c \
    upstream-freebsd/lib/msun/src/e_expf.c \
    upstream-freebsd/lib/msun/src/e_fmod.c \
    upstream-freebsd/lib/msun/src/e_fmodf.c \
    upstream-freebsd/lib/msun/src/e_gamma.c \
    upstream-freebsd/lib/msun/src/e_gammaf.c \
    upstream-freebsd/lib/msun/src/e_gammaf_r.c \
    upstream-freebsd/lib/msun/src/e_gamma_r.c \
    upstream-freebsd/lib/msun/src/e_hypot.c \
    upstream-freebsd/lib/msun/src/e_hypotf.c \
    upstream-freebsd/lib/msun/src/e_j0.c \
    upstream-freebsd/lib/msun/src/e_j0f.c \
    upstream-freebsd/lib/msun/src/e_j1.c \
    upstream-freebsd/lib/msun/src/e_j1f.c \
    upstream-freebsd/lib/msun/src/e_jn.c \
    upstream-freebsd/lib/msun/src/e_jnf.c \
    upstream-freebsd/lib/msun/src/e_lgamma.c \
    upstream-freebsd/lib/msun/src/e_lgammaf.c \
    upstream-freebsd/lib/msun/src/e_lgammaf_r.c \
    upstream-freebsd/lib/msun/src/e_lgamma_r.c \
    upstream-freebsd/lib/msun/src/e_log10.c \
    upstream-freebsd/lib/msun/src/e_log10f.c \
    upstream-freebsd/lib/msun/src/e_log2.c \
    upstream-freebsd/lib/msun/src/e_log2f.c \
    upstream-freebsd/lib/msun/src/e_log.c \
    upstream-freebsd/lib/msun/src/e_logf.c \
    upstream-freebsd/lib/msun/src/e_pow.c \
    upstream-freebsd/lib/msun/src/e_powf.c \
    upstream-freebsd/lib/msun/src/e_remainder.c \
    upstream-freebsd/lib/msun/src/e_remainderf.c \
    upstream-freebsd/lib/msun/src/e_rem_pio2.c \
    upstream-freebsd/lib/msun/src/e_rem_pio2f.c \
    upstream-freebsd/lib/msun/src/e_scalb.c \
    upstream-freebsd/lib/msun/src/e_scalbf.c \
    upstream-freebsd/lib/msun/src/e_sinh.c \
    upstream-freebsd/lib/msun/src/e_sinhf.c \
    upstream-freebsd/lib/msun/src/e_sqrt.c \
    upstream-freebsd/lib/msun/src/e_sqrtf.c \
    upstream-freebsd/lib/msun/src/imprecise.c \
    upstream-freebsd/lib/msun/src/k_cos.c \
    upstream-freebsd/lib/msun/src/k_cosf.c \
    upstream-freebsd/lib/msun/src/k_exp.c \
    upstream-freebsd/lib/msun/src/k_expf.c \
    upstream-freebsd/lib/msun/src/k_rem_pio2.c \
    upstream-freebsd/lib/msun/src/k_sin.c \
    upstream-freebsd/lib/msun/src/k_sinf.c \
    upstream-freebsd/lib/msun/src/k_tan.c \
    upstream-freebsd/lib/msun/src/k_tanf.c \
    upstream-freebsd/lib/msun/src/s_asinh.c \
    upstream-freebsd/lib/msun/src/s_asinhf.c \
    upstream-freebsd/lib/msun/src/s_atan.c \
    upstream-freebsd/lib/msun/src/s_atanf.c \
    upstream-freebsd/lib/msun/src/s_carg.c \
    upstream-freebsd/lib/msun/src/s_cargf.c \
    upstream-freebsd/lib/msun/src/s_cbrt.c \
    upstream-freebsd/lib/msun/src/s_cbrtf.c \
    upstream-freebsd/lib/msun/src/s_ccosh.c \
    upstream-freebsd/lib/msun/src/s_ccoshf.c \
    upstream-freebsd/lib/msun/src/s_ceil.c \
    upstream-freebsd/lib/msun/src/s_ceilf.c \
    upstream-freebsd/lib/msun/src/s_cexp.c \
    upstream-freebsd/lib/msun/src/s_cexpf.c \
    upstream-freebsd/lib/msun/src/s_cimag.c \
    upstream-freebsd/lib/msun/src/s_cimagf.c \
    upstream-freebsd/lib/msun/src/s_conj.c \
    upstream-freebsd/lib/msun/src/s_conjf.c \
    upstream-freebsd/lib/msun/src/s_copysign.c \
    upstream-freebsd/lib/msun/src/s_copysignf.c \
    upstream-freebsd/lib/msun/src/s_cos.c \
    upstream-freebsd/lib/msun/src/s_cosf.c \
    upstream-freebsd/lib/msun/src/s_cproj.c \
    upstream-freebsd/lib/msun/src/s_cprojf.c \
    upstream-freebsd/lib/msun/src/s_creal.c \
    upstream-freebsd/lib/msun/src/s_crealf.c \
    upstream-freebsd/lib/msun/src/s_csinh.c \
    upstream-freebsd/lib/msun/src/s_csinhf.c \
    upstream-freebsd/lib/msun/src/s_csqrt.c \
    upstream-freebsd/lib/msun/src/s_csqrtf.c \
    upstream-freebsd/lib/msun/src/s_ctanh.c \
    upstream-freebsd/lib/msun/src/s_ctanhf.c \
    upstream-freebsd/lib/msun/src/s_erf.c \
    upstream-freebsd/lib/msun/src/s_erff.c \
    upstream-freebsd/lib/msun/src/s_exp2.c \
    upstream-freebsd/lib/msun/src/s_exp2f.c \
    upstream-freebsd/lib/msun/src/s_expm1.c \
    upstream-freebsd/lib/msun/src/s_expm1f.c \
    upstream-freebsd/lib/msun/src/s_fabs.c \
    upstream-freebsd/lib/msun/src/s_fabsf.c \
    upstream-freebsd/lib/msun/src/s_fdim.c \
    upstream-freebsd/lib/msun/src/s_finite.c \
    upstream-freebsd/lib/msun/src/s_finitef.c \
    upstream-freebsd/lib/msun/src/s_floor.c \
    upstream-freebsd/lib/msun/src/s_floorf.c \
    upstream-freebsd/lib/msun/src/s_fma.c \
    upstream-freebsd/lib/msun/src/s_fmaf.c \
    upstream-freebsd/lib/msun/src/s_fmax.c \
    upstream-freebsd/lib/msun/src/s_fmaxf.c \
    upstream-freebsd/lib/msun/src/s_fmin.c \
    upstream-freebsd/lib/msun/src/s_fminf.c \
    upstream-freebsd/lib/msun/src/s_frexp.c \
    upstream-freebsd/lib/msun/src/s_frexpf.c \
    upstream-freebsd/lib/msun/src/s_ilogb.c \
    upstream-freebsd/lib/msun/src/s_ilogbf.c \
    upstream-freebsd/lib/msun/src/s_llrint.c \
    upstream-freebsd/lib/msun/src/s_llrintf.c \
    upstream-freebsd/lib/msun/src/s_llround.c \
    upstream-freebsd/lib/msun/src/s_llroundf.c \
    upstream-freebsd/lib/msun/src/s_log1p.c \
    upstream-freebsd/lib/msun/src/s_log1pf.c \
    upstream-freebsd/lib/msun/src/s_logb.c \
    upstream-freebsd/lib/msun/src/s_logbf.c \
    upstream-freebsd/lib/msun/src/s_lrint.c \
    upstream-freebsd/lib/msun/src/s_lrintf.c \
    upstream-freebsd/lib/msun/src/s_lround.c \
    upstream-freebsd/lib/msun/src/s_lroundf.c \
    upstream-freebsd/lib/msun/src/s_modf.c \
    upstream-freebsd/lib/msun/src/s_modff.c \
    upstream-freebsd/lib/msun/src/s_nan.c \
    upstream-freebsd/lib/msun/src/s_nearbyint.c \
    upstream-freebsd/lib/msun/src/s_nextafter.c \
    upstream-freebsd/lib/msun/src/s_nextafterf.c \
    upstream-freebsd/lib/msun/src/s_remquo.c \
    upstream-freebsd/lib/msun/src/s_remquof.c \
    upstream-freebsd/lib/msun/src/s_rint.c \
    upstream-freebsd/lib/msun/src/s_rintf.c \
    upstream-freebsd/lib/msun/src/s_round.c \
    upstream-freebsd/lib/msun/src/s_roundf.c \
    upstream-freebsd/lib/msun/src/s_scalbln.c \
    upstream-freebsd/lib/msun/src/s_scalbn.c \
    upstream-freebsd/lib/msun/src/s_scalbnf.c \
    upstream-freebsd/lib/msun/src/s_signgam.c \
    upstream-freebsd/lib/msun/src/s_significand.c \
    upstream-freebsd/lib/msun/src/s_significandf.c \
    upstream-freebsd/lib/msun/src/s_sin.c \
    upstream-freebsd/lib/msun/src/s_sinf.c \
    upstream-freebsd/lib/msun/src/s_tan.c \
    upstream-freebsd/lib/msun/src/s_tanf.c \
    upstream-freebsd/lib/msun/src/s_tanh.c \
    upstream-freebsd/lib/msun/src/s_tanhf.c \
    upstream-freebsd/lib/msun/src/s_tgammaf.c \
    upstream-freebsd/lib/msun/src/s_trunc.c \
    upstream-freebsd/lib/msun/src/s_truncf.c \
    upstream-freebsd/lib/msun/src/w_cabs.c \
    upstream-freebsd/lib/msun/src/w_cabsf.c \
    upstream-freebsd/lib/msun/src/w_drem.c \
    upstream-freebsd/lib/msun/src/w_dremf.c \

libm_common_src_files += \
    fake_long_double.c \
    signbit.c \

libm_ld_src_files = \
    upstream-freebsd/lib/msun/src/e_acosl.c \
    upstream-freebsd/lib/msun/src/e_acoshl.c \
    upstream-freebsd/lib/msun/src/e_asinl.c \
    upstream-freebsd/lib/msun/src/e_atan2l.c \
    upstream-freebsd/lib/msun/src/e_atanhl.c \
    upstream-freebsd/lib/msun/src/e_fmodl.c \
    upstream-freebsd/lib/msun/src/e_hypotl.c \
    upstream-freebsd/lib/msun/src/e_remainderl.c \
    upstream-freebsd/lib/msun/src/e_sqrtl.c \
    upstream-freebsd/lib/msun/src/s_asinhl.c \
    upstream-freebsd/lib/msun/src/s_atanl.c \
    upstream-freebsd/lib/msun/src/s_cbrtl.c \
    upstream-freebsd/lib/msun/src/s_ceill.c \
    upstream-freebsd/lib/msun/src/s_copysignl.c \
    upstream-freebsd/lib/msun/src/s_cosl.c \
    upstream-freebsd/lib/msun/src/s_fabsl.c \
    upstream-freebsd/lib/msun/src/s_floorl.c \
    upstream-freebsd/lib/msun/src/s_fmal.c \
    upstream-freebsd/lib/msun/src/s_fmaxl.c \
    upstream-freebsd/lib/msun/src/s_fminl.c \
    upstream-freebsd/lib/msun/src/s_modfl.c \
    upstream-freebsd/lib/msun/src/s_frexpl.c \
    upstream-freebsd/lib/msun/src/s_ilogbl.c \
    upstream-freebsd/lib/msun/src/s_llrintl.c \
    upstream-freebsd/lib/msun/src/s_llroundl.c \
    upstream-freebsd/lib/msun/src/s_logbl.c \
    upstream-freebsd/lib/msun/src/s_lrintl.c \
    upstream-freebsd/lib/msun/src/s_lroundl.c \
    upstream-freebsd/lib/msun/src/s_nextafterl.c \
    upstream-freebsd/lib/msun/src/s_nexttoward.c \
    upstream-freebsd/lib/msun/src/s_nexttowardf.c \
    upstream-freebsd/lib/msun/src/s_remquol.c \
    upstream-freebsd/lib/msun/src/s_rintl.c \
    upstream-freebsd/lib/msun/src/s_roundl.c \
    upstream-freebsd/lib/msun/src/s_scalbnl.c \
    upstream-freebsd/lib/msun/src/s_sinl.c \
    upstream-freebsd/lib/msun/src/s_tanl.c \
    upstream-freebsd/lib/msun/src/s_truncl.c \

libm_ld_src_files += \
    upstream-freebsd/lib/msun/ld128/invtrig.c \
    upstream-freebsd/lib/msun/ld128/k_cosl.c \
    upstream-freebsd/lib/msun/ld128/k_sinl.c \
    upstream-freebsd/lib/msun/ld128/k_tanl.c \
    upstream-freebsd/lib/msun/ld128/s_exp2l.c \
    upstream-freebsd/lib/msun/ld128/s_expl.c \
    upstream-freebsd/lib/msun/ld128/s_logl.c \
    upstream-freebsd/lib/msun/ld128/s_nanl.c \

# TODO: re-enable i387/e_sqrtf.S for x86, and maybe others.

libm_common_cflags := \
    -DFLT_EVAL_METHOD=0 \
    -std=c99 \
    -include $(LOCAL_PATH)/freebsd-compat.h \
    -Wno-missing-braces \
    -Wno-parentheses \
    -Wno-sign-compare \
    -Wno-uninitialized \
    -Wno-unknown-pragmas \
    -fvisibility=hidden \

# Workaround the GCC "(long)fn -> lfn" optimization bug which will result in
# self recursions for lrint, lrintf, and lrintl.
# BUG: 14225968
libm_common_cflags += -fno-builtin-rint -fno-builtin-rintf -fno-builtin-rintl

libm_common_includes := $(LOCAL_PATH)/upstream-freebsd/lib/msun/src/

libm_ld_includes := $(LOCAL_PATH)/upstream-freebsd/lib/msun/ld128/

#
# libm.a for target.
#
include $(CLEAR_VARS)
LOCAL_MODULE:= libm
LOCAL_ADDITIONAL_DEPENDENCIES := $(LOCAL_PATH)/Android.mk
LOCAL_ARM_MODE := arm
LOCAL_CFLAGS := $(libm_common_cflags)
LOCAL_C_INCLUDES += $(libm_common_includes)
LOCAL_SRC_FILES := $(libm_common_src_files)
LOCAL_SYSTEM_SHARED_LIBRARIES := libc

# arch-specific settings
LOCAL_C_INCLUDES_arm := $(LOCAL_PATH)/arm
LOCAL_SRC_FILES_arm := arm/fenv.c

LOCAL_C_INCLUDES_arm64 := $(libm_ld_includes)
LOCAL_SRC_FILES_arm64 := arm64/fenv.c $(libm_ld_src_files)

LOCAL_C_INCLUDES_x86 := $(LOCAL_PATH)/i387
LOCAL_SRC_FILES_x86 := i387/fenv.c

LOCAL_C_INCLUDES_x86_64 := $(libm_ld_includes)
LOCAL_SRC_FILES_x86_64 := amd64/fenv.c $(libm_ld_src_files)

LOCAL_SRC_FILES_mips := mips/fenv.c

LOCAL_C_INCLUDES_mips64 := $(libm_ld_includes)
LOCAL_SRC_FILES_mips64 := mips/fenv.c $(libm_ld_src_files)

include $(BUILD_STATIC_LIBRARY)

#
# libm.so for target.
#
include $(CLEAR_VARS)
LOCAL_MODULE:= libm
LOCAL_ADDITIONAL_DEPENDENCIES := $(LOCAL_PATH)/Android.mk
LOCAL_SYSTEM_SHARED_LIBRARIES := libc
LOCAL_WHOLE_STATIC_LIBRARIES := libm

# We'd really like to do this for all architectures, but since this wasn't done
# before, these symbols must continue to be exported on LP32 for binary
# compatibility.
LOCAL_LDFLAGS_arm64 := -Wl,--exclude-libs,libgcc.a
LOCAL_LDFLAGS_mips64 := -Wl,--exclude-libs,libgcc.a
LOCAL_LDFLAGS_x86_64 := -Wl,--exclude-libs,libgcc.a
include $(BUILD_SHARED_LIBRARY)
endif
