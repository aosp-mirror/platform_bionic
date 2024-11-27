/*
 * This file is auto-generated. Modifications will be lost.
 *
 * See https://android.googlesource.com/platform/bionic/+/master/libc/kernel/
 * for more information.
 */
#ifndef _UAPI__ASM_ARM_SWAB_H
#define _UAPI__ASM_ARM_SWAB_H
#include <linux/compiler.h>
#include <linux/types.h>
#ifndef __STRICT_ANSI__
#define __SWAB_64_THRU_32__
#endif
static inline __attribute__((__const__)) __u32 __arch_swab32(__u32 x) {
  __u32 t;
#ifndef __thumb__
  if(! __builtin_constant_p(x)) {
    asm("eor\t%0, %1, %1, ror #16" : "=r" (t) : "r" (x));
  } else
#endif
  t = x ^ ((x << 16) | (x >> 16));
  x = (x << 24) | (x >> 8);
  t &= ~0x00FF0000;
  x ^= (t >> 8);
  return x;
}
#define __arch_swab32 __arch_swab32
#endif
