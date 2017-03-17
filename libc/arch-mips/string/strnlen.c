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

#define op_t                unsigned long int
#define op_size             sizeof (op_t)

#if __mips64 || __mips_isa_rev >= 2
static inline size_t __attribute__ ((always_inline))
do_bytes (const char *base, const char *p, op_t inval)
{
  op_t outval = 0;
#if __mips64
  __asm__ volatile (
    "dsbh %1, %0 \n\t"
    "dshd %0, %1 \n\t"
    "dclz %1, %0 \n\t"
    : "+r" (inval), "+r" (outval)
  );
#else
  __asm__ volatile (
    "wsbh %1, %0 \n\t"
    "rotr %0, %1, 16 \n\t"
    "clz %1, %0 \n\t"
    : "+r" (inval), "+r" (outval)
  );
#endif
  p += (outval >> 3);
  return (size_t) (p - base);
}

#define DO_WORD(in, val) {                          \
  op_t tmp = ((val - mask_1) & ~val) & mask_128;    \
  if (tmp)                                          \
    return do_bytes(str, (const char *)(in), tmp);  \
}
#else
static inline size_t __attribute__ ((always_inline))
do_bytes (const char *base, const char *p)
{
  for (; *p; ++p);
  return (size_t) (p - base);
}

#define DO_WORD(in, val) {                     \
  if (((val - mask_1) & ~val) & mask_128) {    \
    return do_bytes(str, (const char *)(in));  \
  }                                            \
}
#endif

size_t strnlen (const char *str, size_t n) {
  if (n != 0) {
    const char *p = (const char *) str;
    const op_t *w;
    op_t mask_1, mask_128;

    for (; n > 0 && ((size_t) p % op_size) != 0; --n, ++p) {
      if (!(*p))
        return (p - str);
    }

    w = (const op_t *) p;

    __asm__ volatile (
      "li %0, 0x01010101 \n\t"
      : "=r" (mask_1)
    );
#if __mips64
    mask_1 |= mask_1 << 32;
#endif
    mask_128 = mask_1 << 7;

    /*
     * Check op_size byteswize after initial alignment
     */
    while (n >= 4 * op_size) {
      const op_t w0 = w[0];
      const op_t w1 = w[1];
      const op_t w2 = w[2];
      const op_t w3 = w[3];
      DO_WORD(w + 0, w0)
      DO_WORD(w + 1, w1)
      DO_WORD(w + 2, w2)
      DO_WORD(w + 3, w3)
      w += 4;
      n -= 4 * op_size;
    }

    while (n >= op_size) {
      DO_WORD(w, w[0]);
      w++;
      n -= op_size;
    }

    /*
     * Check bytewize for remaining bytes
     */
    p = (const char *) w;
    for (; n > 0; --n, ++p) {
      if (!(*p))
        return (p - str);
    }

    return (p - str);
  }

  return 0;
}
