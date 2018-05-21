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

#define op_t        unsigned long int

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

#if __mips64
typedef struct
{
  op_t B0:8, B1:8, B2:8, B3:8, B4:8, B5:8, B6:8, B7:8;
} bits_t;
#else
typedef struct
{
  op_t B0:8, B1:8, B2:8, B3:8;
} bits_t;
#endif

typedef union
{
  op_t v;
  bits_t b;
} bitfields_t;

#if !HW_UNALIGNED_SUPPORT && UNALIGNED_INSTR_SUPPORT
/* for MIPS GCC, there are no unaligned builtins - so this struct forces
   the compiler to treat the pointer access as unaligned.  */
struct ulw
{
  op_t uli;
} __attribute__ ((packed));
#endif /* !HW_UNALIGNED_SUPPORT && UNALIGNED_INSTR_SUPPORT */

#define DO_BYTE(i, ptdst) {  \
  *(ptdst+i) = a.b.B##i;     \
  if(a.b.B##i == '\0')       \
    return ret;              \
}

#if __mips64
#define DO_BYTES(val, dst) {   \
  bitfields_t a;               \
  char *tdst = (char *)(dst);  \
  a.v = val;                   \
  DO_BYTE(0, tdst)             \
  DO_BYTE(1, tdst)             \
  DO_BYTE(2, tdst)             \
  DO_BYTE(3, tdst)             \
  DO_BYTE(4, tdst)             \
  DO_BYTE(5, tdst)             \
  DO_BYTE(6, tdst)             \
  DO_BYTE(7, tdst)             \
}
#else
#define DO_BYTES(val, dst) {   \
  bitfields_t a;               \
  char *tdst = (char *)(dst);  \
  a.v = val;                   \
  DO_BYTE(0, tdst)             \
  DO_BYTE(1, tdst)             \
  DO_BYTE(2, tdst)             \
  DO_BYTE(3, tdst)             \
}
#endif

#define DO_WORD_ALIGNED(dst, src) {                 \
  op_t val = *(src);                                \
  if ((((val - mask_1) & ~val) & mask_128) != 0) {  \
    DO_BYTES(val, dst);                             \
  } else *(dst) = val;                              \
}

#if !HW_UNALIGNED_SUPPORT
#if UNALIGNED_INSTR_SUPPORT
#define DO_WORD_UNALIGNED(dst, src) {               \
  op_t val = *(src);                                \
  if ((((val - mask_1) & ~val) & mask_128) != 0) {  \
    DO_BYTES(val, dst);                             \
  } else {                                          \
    struct ulw *a = (struct ulw *)(dst);            \
    a->uli = val;                                   \
  }                                                 \
}
#else
#define DO_WORD_UNALIGNED(dst, src) {                 \
  op_t val = *(src);                                  \
  if ((((val - mask_1) & ~val) & mask_128) != 0) {    \
    DO_BYTES(val, dst);                               \
  } else {                                            \
    char *pdst = (char *) dst;                        \
    const char *psrc = (const char *) src;            \
    for (; (*pdst = *psrc) != '\0'; ++psrc, ++pdst);  \
    return ret;                                       \
  }                                                   \
}
#endif /* UNALIGNED_INSTR_SUPPORT */

#define PROCESS_UNALIGNED_WORDS(a, b) { \
  while (1) {                           \
    DO_WORD_UNALIGNED(a, b);            \
    DO_WORD_UNALIGNED(a + 1, b + 1);    \
    DO_WORD_UNALIGNED(a + 2, b + 2);    \
    DO_WORD_UNALIGNED(a + 3, b + 3);    \
    a += 4;                             \
    b += 4;                             \
  }                                     \
}
#endif /* HW_UNALIGNED_SUPPORT */

#define PROCESS_ALIGNED_WORDS(a, b) {  \
  while (1) {                          \
    DO_WORD_ALIGNED(a, b);             \
    DO_WORD_ALIGNED(a + 1, b + 1);     \
    DO_WORD_ALIGNED(a + 2, b + 2);     \
    DO_WORD_ALIGNED(a + 3, b + 3);     \
    a += 4;                            \
    b += 4;                            \
  }                                    \
}

char *
strcpy (char *to, const char *from)
{
  char *ret = to;
  op_t mask_1, mask_128;
  const op_t *src;
  op_t *dst;

  for (; (*to = *from) != '\0' && ((size_t) from % sizeof (op_t)) != 0; ++from, ++to);

  if(*to != '\0') {
    __asm__ volatile (
      "li %0, 0x01010101 \n\t"
      : "=r" (mask_1)
    );
#if __mips64
    mask_1 |= mask_1 << 32;
#endif
    mask_128 = mask_1 << 7;

    src = (const op_t *) from;
    dst = (op_t *) to;

#if HW_UNALIGNED_SUPPORT
    PROCESS_ALIGNED_WORDS(dst, src);
#else
    if (((unsigned long) dst) % sizeof (op_t) == 0) {
      PROCESS_ALIGNED_WORDS(dst, src);
    } else {
      PROCESS_UNALIGNED_WORDS(dst, src);
    }
#endif
  }

  return ret;
}
