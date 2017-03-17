/*
 * Copyright (c) 2017 Imagination Technologies.
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *      * Redistributions of source code must retain the above copyright
 *        notice, this list of conditions and the following disclaimer.
 *      * Redistributions in binary form must reproduce the above copyright
 *        notice, this list of conditions and the following disclaimer
 *        in the documentation and/or other materials provided with
 *        the distribution.
 *      * Neither the name of Imagination Technologies nor the names of its
 *        contributors may be used to endorse or promote products derived
 *        from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <string.h>
#include <stdint.h>

#define ENABLE_PREFETCH 1

#define STRNG(X) #X
#define PREFETCH(src_ptr, offset)  \
  asm("pref 0, " STRNG(offset) "(%[src]) \n\t" : : [src] "r" (src_ptr));

#if !defined(UNALIGNED_INSTR_SUPPORT)
/* does target have unaligned lw/ld/ualw/uald instructions? */
#define UNALIGNED_INSTR_SUPPORT 0
#if __mips_isa_rev < 6 && !__mips1
#undef UNALIGNED_INSTR_SUPPORT
#define UNALIGNED_INSTR_SUPPORT 1
#endif
#endif

#if !defined(HW_UNALIGNED_SUPPORT)
/* Does target have hardware support for unaligned accesses?  */
#define HW_UNALIGNED_SUPPORT 0
#if __mips_isa_rev >= 6
#undef HW_UNALIGNED_SUPPORT
#define HW_UNALIGNED_SUPPORT 1
#endif
#endif

#define SIZEOF_reg_t 4
#if _MIPS_SIM == _ABIO32
typedef unsigned long reg_t;
typedef struct bits
{
  reg_t B0:8, B1:8, B2:8, B3:8;
} bits_t;
#else
#undef SIZEOF_reg_t
#define SIZEOF_reg_t 8
typedef unsigned long long reg_t;
typedef struct bits
{
    reg_t B0:8, B1:8, B2:8, B3:8, B4:8, B5:8, B6:8, B7:8;
} bits_t;
#endif

/* This union assumes that small structures can be in registers.  If
   not, then memory accesses will be done - not optimal, but ok.  */
typedef union
{
  reg_t v;
  bits_t b;
} bitfields_t;

#define do_bitfield(__i) \
  if (x.b.B##__i != y.b.B##__i) return x.b.B##__i - y.b.B##__i;

/* pull apart the words to find the first differing unsigned byte.  */
static int __attribute__ ((noinline)) do_by_bitfields (reg_t a, reg_t b)
{
  bitfields_t x, y;
  x.v = a;
  y.v = b;
  do_bitfield (0);
  do_bitfield (1);
  do_bitfield (2);
#if SIZEOF_reg_t == 4
  return x.b.B3 - y.b.B3;
#else
  do_bitfield (3);
  do_bitfield (4);
  do_bitfield (5);
  do_bitfield (6);
  return x.b.B7 - y.b.B7;
#endif
}

/* This code is called when aligning a pointer, there are remaining bytes
   after doing word compares, or architecture does not have some form
   of unaligned support.  */
static inline int __attribute__ ((always_inline))
do_bytes (const void *a, const void *b, unsigned long len)
{
  unsigned char *x = (unsigned char *) a;
  unsigned char *y = (unsigned char *) b;
  unsigned long i;

  /* 'len' might be zero here, so preloading the first two values
     before the loop may access unallocated memory.  */
  for (i = 0; i < len; i++) {
    if (*x != *y)
      return *x - *y;
    x++;
    y++;
  }
  return 0;
}

#if !HW_UNALIGNED_SUPPORT
#if UNALIGNED_INSTR_SUPPORT
/* for MIPS GCC, there are no unaligned builtins - so this struct forces
   the compiler to treat the pointer access as unaligned.  */
struct ulw
{
  reg_t uli;
} __attribute__ ((packed));

/* first pointer is not aligned while second pointer is.  */
static int unaligned_words (const struct ulw *a, const reg_t *b,
                            unsigned long words, unsigned long bytes)
{
#if ENABLE_PREFETCH
  /* prefetch pointer aligned to 32 byte boundary */
  const reg_t *pref_ptr = (const reg_t *) (((uintptr_t) b + 31) & ~31);
  const reg_t *pref_ptr_a = (const reg_t *) (((uintptr_t) a + 31) & ~31);
#endif
  for (; words >= 16; words -= 8) {
#if ENABLE_PREFETCH
    pref_ptr += 8;
    PREFETCH(pref_ptr, 0);
    PREFETCH(pref_ptr, 32);

    pref_ptr_a += 8;
    PREFETCH(pref_ptr_a, 0);
    PREFETCH(pref_ptr_a, 32);
#endif
    reg_t x0 = a[0].uli, x1 = a[1].uli;
    reg_t x2 = a[2].uli, x3 = a[3].uli;
    reg_t y0 = b[0], y1 = b[1], y2 = b[2], y3 = b[3];
    if (x0 != y0)
      return do_by_bitfields (x0, y0);
    if (x1 != y1)
      return do_by_bitfields (x1, y1);
    if (x2 != y2)
      return do_by_bitfields (x2, y2);
    if (x3 != y3)
      return do_by_bitfields (x3, y3);

    x0 = a[4].uli; x1 = a[5].uli;
    x2 = a[6].uli; x3 = a[7].uli;
    y0 = b[4]; y1 = b[5]; y2 = b[6]; y3 = b[7];
    if (x0 != y0)
      return do_by_bitfields (x0, y0);
    if (x1 != y1)
      return do_by_bitfields (x1, y1);
    if (x2 != y2)
      return do_by_bitfields (x2, y2);
    if (x3 != y3)
      return do_by_bitfields (x3, y3);

    a += 8;
    b += 8;
  }

  for (; words >= 4; words -= 4) {
    reg_t x0 = a[0].uli, x1 = a[1].uli;
    reg_t x2 = a[2].uli, x3 = a[3].uli;
    reg_t y0 = b[0], y1 = b[1], y2 = b[2], y3 = b[3];
    if (x0 != y0)
      return do_by_bitfields (x0, y0);
    if (x1 != y1)
      return do_by_bitfields (x1, y1);
    if (x2 != y2)
      return do_by_bitfields (x2, y2);
    if (x3 != y3)
      return do_by_bitfields (x3, y3);
    a += 4;
    b += 4;
  }

  /* do remaining words.  */
  while (words--) {
    reg_t x0 = a->uli;
    reg_t y0 = *b;
    a += 1;
    b += 1;
    if (x0 != y0)
      return do_by_bitfields (x0, y0);
  }

  /* mop up any remaining bytes.  */
  return do_bytes (a, b, bytes);
}
#else
/* no HW support or unaligned lw/ld/ualw/uald instructions.  */
static int unaligned_words (const reg_t *a, const reg_t *b,
                            unsigned long words, unsigned long bytes)
{
  return do_bytes (a, b, (sizeof (reg_t) * words) + bytes);
}
#endif /* UNALIGNED_INSTR_SUPPORT */
#endif /* HW_UNALIGNED_SUPPORT */

/* both pointers are aligned, or first isn't and HW support for unaligned.  */
static int aligned_words (const reg_t *a, const reg_t *b,
                          unsigned long words, unsigned long bytes)
{
#if ENABLE_PREFETCH
  /* prefetch pointer aligned to 32 byte boundary */
  const reg_t *pref_ptr = (const reg_t *) (((uintptr_t) b + 31) & ~31);
  const reg_t *pref_ptr_a = (const reg_t *) (((uintptr_t) a + 31) & ~31);
#endif

  for (; words >= 24; words -= 12) {
#if ENABLE_PREFETCH
    pref_ptr += 12;
    PREFETCH(pref_ptr, 0);
    PREFETCH(pref_ptr, 32);
    PREFETCH(pref_ptr, 64);

    pref_ptr_a += 12;
    PREFETCH(pref_ptr_a, 0);
    PREFETCH(pref_ptr_a, 32);
    PREFETCH(pref_ptr_a, 64);
#endif
    reg_t x0 = a[0], x1 = a[1], x2 = a[2], x3 = a[3];
    reg_t y0 = b[0], y1 = b[1], y2 = b[2], y3 = b[3];
    if (x0 != y0)
      return do_by_bitfields (x0, y0);
    if (x1 != y1)
      return do_by_bitfields (x1, y1);
    if (x2 != y2)
      return do_by_bitfields (x2, y2);
    if (x3 != y3)
      return do_by_bitfields (x3, y3);

    x0 = a[4]; x1 = a[5]; x2 = a[6]; x3 = a[7];
    y0 = b[4]; y1 = b[5]; y2 = b[6]; y3 = b[7];
    if (x0 != y0)
      return do_by_bitfields (x0, y0);
    if (x1 != y1)
      return do_by_bitfields (x1, y1);
    if (x2 != y2)
      return do_by_bitfields (x2, y2);
    if (x3 != y3)
      return do_by_bitfields (x3, y3);

    x0 = a[8]; x1 = a[9]; x2 = a[10]; x3 = a[11];
    y0 = b[8]; y1 = b[9]; y2 = b[10]; y3 = b[11];
    if (x0 != y0)
      return do_by_bitfields (x0, y0);
    if (x1 != y1)
      return do_by_bitfields (x1, y1);
    if (x2 != y2)
      return do_by_bitfields (x2, y2);
    if (x3 != y3)
      return do_by_bitfields (x3, y3);

    a += 12;
    b += 12;
  }

  for (; words >= 4; words -= 4) {
    reg_t x0 = a[0], x1 = a[1], x2 = a[2], x3 = a[3];
    reg_t y0 = b[0], y1 = b[1], y2 = b[2], y3 = b[3];
    if (x0 != y0)
      return do_by_bitfields (x0, y0);
    if (x1 != y1)
      return do_by_bitfields (x1, y1);
    if (x2 != y2)
      return do_by_bitfields (x2, y2);
    if (x3 != y3)
      return do_by_bitfields (x3, y3);
    a += 4;
    b += 4;
  }

  /* do remaining words.  */
  while (words--) {
    reg_t x0 = *a;
    reg_t y0 = *b;
    a += 1;
    b += 1;
    if (x0 != y0)
      return do_by_bitfields (x0, y0);
  }

  /* mop up any remaining bytes.  */
  return do_bytes (a, b, bytes);
}

int memcmp (const void *a, const void *b, size_t len)
{
  unsigned long bytes, words;

  /* shouldn't hit that often.  */
  if (len < sizeof (reg_t) * 4) {
    return do_bytes (a, b, len);
  }

  /* Align the second pointer to word/dword alignment.
     Note that the pointer is only 32-bits for o32/n32 ABIs. For
     n32, loads are done as 64-bit while address remains 32-bit.   */
  bytes = ((unsigned long) b) % sizeof (reg_t);
  if (bytes) {
    int res;
    bytes = sizeof (reg_t) - bytes;
    if (bytes > len)
      bytes = len;
    res = do_bytes (a, b, bytes);
    if (res || len == bytes)
      return res;
    len -= bytes;
    a = (const void *) (((unsigned char *) a) + bytes);
    b = (const void *) (((unsigned char *) b) + bytes);
  }

  /* Second pointer now aligned.  */
  words = len / sizeof (reg_t);
  bytes = len % sizeof (reg_t);

#if HW_UNALIGNED_SUPPORT
  /* treat possible unaligned first pointer as aligned.  */
  return aligned_words (a, b, words, bytes);
#else
  if (((unsigned long) a) % sizeof (reg_t) == 0) {
    return aligned_words (a, b, words, bytes);
  }
  /* need to use unaligned instructions on first pointer.  */
  return unaligned_words (a, b, words, bytes);
#endif
}
