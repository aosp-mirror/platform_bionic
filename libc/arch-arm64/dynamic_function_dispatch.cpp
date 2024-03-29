/*
 * Copyright (C) 2019 The Android Open Source Project
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

#include <private/bionic_ifuncs.h>
#include <stddef.h>
#include <sys/auxv.h>

#define MIDR_IMPL_ID_SHIFT 24u
#define MIDR_IMPL_ID_MASK 0xFF
#define CPU_VARIANT_SHIFT 20u
#define CPU_VARIANT_MASK 0xF

/* Macro to identify CPU implementer */
#define QCOM_IMPL_ID 0x51

/* Macro to indentify qualcomm CPU variants which supports
 * __memcpy_aarch64_nt routine
 */
#define QCOM_ORYON_CPU_VARIANTS 0x5

extern "C" {

typedef void* memchr_func(const void*, int, size_t);
DEFINE_IFUNC_FOR(memchr) {
    if (arg->_hwcap2 & HWCAP2_MTE) {
        RETURN_FUNC(memchr_func, __memchr_aarch64_mte);
    } else {
        RETURN_FUNC(memchr_func, __memchr_aarch64);
    }
}

typedef int memcmp_func(const void*, const void*, size_t);
DEFINE_IFUNC_FOR(memcmp) {
    // TODO: enable the SVE version.
    RETURN_FUNC(memcmp_func, __memcmp_aarch64);
}

typedef void* memcpy_func(void*, const void*, size_t);
DEFINE_IFUNC_FOR(memcpy) {
  unsigned long midr;
  unsigned int impl_id, cpu_variant;

  /* Check if hardware capability CPUID is available */
  if (arg->_hwcap & HWCAP_CPUID) {
    /* Read the MIDR register */
    asm("mrs %0, MIDR_EL1 \n\t" : "=r"(midr));

    /* Extract the CPU Implementer ID */
    impl_id = (midr >> MIDR_IMPL_ID_SHIFT) & (MIDR_IMPL_ID_MASK);

    /* Check for Qualcomm implementer ID */
    if (impl_id == QCOM_IMPL_ID) {
      cpu_variant = (midr >> CPU_VARIANT_SHIFT) & CPU_VARIANT_MASK;

      /* Check for Qualcomm Oryon CPU variants: 0x1, 0x2, 0x3, 0x4, 0x5 */
      if (cpu_variant <= QCOM_ORYON_CPU_VARIANTS) {
        RETURN_FUNC(memcpy_func, __memcpy_aarch64_nt);
      } else {
        RETURN_FUNC(memcpy_func, __memcpy_aarch64);
      }
    }
  }
  /* If CPU implementer is not Qualcomm, choose the custom
   * implementation based on CPU architecture feature
   * */
  if (arg->_hwcap & HWCAP_ASIMD) {
    RETURN_FUNC(memcpy_func, __memcpy_aarch64_simd);
  } else {
    RETURN_FUNC(memcpy_func, __memcpy_aarch64);
  }
}

typedef void* memmove_func(void*, const void*, size_t);
DEFINE_IFUNC_FOR(memmove) {
  unsigned long midr;
  unsigned int impl_id, cpu_variant;

  /* Check if hardware capability CPUID is available */
  if (arg->_hwcap & HWCAP_CPUID) {
    /* Read the MIDR register */
    asm("mrs %0, MIDR_EL1 \n\t" : "=r"(midr));

    /* Extract the CPU Implementer ID */
    impl_id = (midr >> MIDR_IMPL_ID_SHIFT) & (MIDR_IMPL_ID_MASK);

    /* Check for Qualcomm implementer ID */
    if (impl_id == QCOM_IMPL_ID) {
      cpu_variant = (midr >> CPU_VARIANT_SHIFT) & CPU_VARIANT_MASK;

      /* Check for Qualcomm Oryon CPU variants: 0x1, 0x2, 0x3, 0x4, 0x5 */
      if (cpu_variant <= QCOM_ORYON_CPU_VARIANTS) {
        RETURN_FUNC(memcpy_func, __memmove_aarch64_nt);
      } else {
        RETURN_FUNC(memcpy_func, __memmove_aarch64);
      }
    }
  }
  /* If CPU implementer is not Qualcomm, choose the custom
   * implementation based on CPU architecture feature
   * */
  if (arg->_hwcap & HWCAP_ASIMD) {
    RETURN_FUNC(memmove_func, __memmove_aarch64_simd);
  } else {
    RETURN_FUNC(memmove_func, __memmove_aarch64);
  }
}

typedef int memrchr_func(const void*, int, size_t);
DEFINE_IFUNC_FOR(memrchr) {
    RETURN_FUNC(memrchr_func, __memrchr_aarch64);
}

typedef int memset_func(void*, int, size_t);
DEFINE_IFUNC_FOR(memset) {
    RETURN_FUNC(memset_func, __memset_aarch64);
}

typedef char* stpcpy_func(char*, const char*, size_t);
DEFINE_IFUNC_FOR(stpcpy) {
    // TODO: enable the SVE version.
    RETURN_FUNC(stpcpy_func, __stpcpy_aarch64);
}

typedef char* strchr_func(const char*, int);
DEFINE_IFUNC_FOR(strchr) {
    if (arg->_hwcap2 & HWCAP2_MTE) {
        RETURN_FUNC(strchr_func, __strchr_aarch64_mte);
    } else {
        RETURN_FUNC(strchr_func, __strchr_aarch64);
    }
}

typedef char* strchrnul_func(const char*, int);
DEFINE_IFUNC_FOR(strchrnul) {
    if (arg->_hwcap2 & HWCAP2_MTE) {
        RETURN_FUNC(strchrnul_func, __strchrnul_aarch64_mte);
    } else {
        RETURN_FUNC(strchrnul_func, __strchrnul_aarch64);
    }
}

typedef int strcmp_func(const char*, const char*);
DEFINE_IFUNC_FOR(strcmp) {
    // TODO: enable the SVE version.
    RETURN_FUNC(strcmp_func, __strcmp_aarch64);
}

typedef char* strcpy_func(char*, const char*);
DEFINE_IFUNC_FOR(strcpy) {
    // TODO: enable the SVE version.
    RETURN_FUNC(strcpy_func, __strcpy_aarch64);
}

typedef size_t strlen_func(const char*);
DEFINE_IFUNC_FOR(strlen) {
    if (arg->_hwcap2 & HWCAP2_MTE) {
        RETURN_FUNC(strlen_func, __strlen_aarch64_mte);
    } else {
        RETURN_FUNC(strlen_func, __strlen_aarch64);
    }
}

typedef int strncmp_func(const char*, const char*, size_t);
DEFINE_IFUNC_FOR(strncmp) {
    // TODO: enable the SVE version.
    RETURN_FUNC(strncmp_func, __strncmp_aarch64);
}

typedef size_t strnlen_func(const char*, size_t);
DEFINE_IFUNC_FOR(strnlen) {
    // TODO: enable the SVE version.
    RETURN_FUNC(strnlen_func, __strnlen_aarch64);
}

typedef char* strrchr_func(const char*, int);
DEFINE_IFUNC_FOR(strrchr) {
    if (arg->_hwcap2 & HWCAP2_MTE) {
        RETURN_FUNC(strrchr_func, __strrchr_aarch64_mte);
    } else {
        RETURN_FUNC(strrchr_func, __strrchr_aarch64);
    }
}

}  // extern "C"
