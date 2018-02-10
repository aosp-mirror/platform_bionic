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

#define ENABLE_PREFETCH     1
#define op_t                unsigned long int
#define op_size             sizeof (op_t)

#if ENABLE_PREFETCH
#define PREFETCH(addr)  __builtin_prefetch (addr, 0, 1);
#else
#define PREFETCH(addr)
#endif

#if __mips64 || __mips_isa_rev >= 2
static inline void * __attribute__ ((always_inline))
do_bytes (const op_t* w, op_t inval)
{
  const unsigned char *p = (const unsigned char *) w;
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
  return (void *) p;
}

#define DO_WORD(in, val) {                        \
  op_t tmp = ((val - mask_1) & ~val) & mask_128;  \
  if (tmp != 0)                                   \
    return do_bytes(in, tmp);                     \
}
#else
static inline void * __attribute__ ((always_inline))
do_bytes (const op_t* w, unsigned char ch)
{
  const unsigned char *p = (const unsigned char *) w;
  for (; *p != ch; ++p);
  return (void *) p;
}

#define DO_WORD(in, val) {                        \
  op_t tmp = ((val - mask_1) & ~val) & mask_128;  \
  if (tmp != 0)                                   \
    return do_bytes(in, ch);                      \
}
#endif

#define DO_WORDS(w) {          \
  op_t* w1 = (op_t*) w;        \
  op_t val0 = w1[0] ^ mask_c;  \
  op_t val1 = w1[1] ^ mask_c;  \
  op_t val2 = w1[2] ^ mask_c;  \
  op_t val3 = w1[3] ^ mask_c;  \
  DO_WORD(w1, val0)            \
  DO_WORD(w1 + 1, val1)        \
  DO_WORD(w1 + 2, val2)        \
  DO_WORD(w1 + 3, val3)        \
}

void *
memchr (void const *s, int c_in, size_t n)
{
  if (n != 0) {
    const unsigned char *p = (const unsigned char *) s;
    const op_t *w;
    op_t mask_1, mask_128, mask_c;
    unsigned char ch = (unsigned char) c_in;

    /*
     * Check bytewize till initial alignment
     */
    for (; n > 0 && ((size_t) p % op_size) != 0; --n, ++p) {
      if (*p == ch)
        return (void *) p;
    }

    w = (const op_t *) p;

    mask_c = ch | (ch << 8);
    mask_c |= mask_c << 16;
    __asm__ volatile (
      "li %0, 0x01010101 \n\t"
      : "=r" (mask_1)
    );
#if __mips64
    mask_1 |= mask_1 << 32;
    mask_c |= mask_c << 32;
#endif
    mask_128 = mask_1 << 7;

    /*
     * Check op_size byteswize after initial alignment
     */
#if ((_MIPS_SIM == _ABIO32) || _MIPS_TUNE_I6400)
    PREFETCH (w);
    PREFETCH (w + 8);
    while (n >= 24 * op_size) {
      PREFETCH(w + 16);
      DO_WORDS(w);
      DO_WORDS(w + 4);
      w += 8;
      n -= 8 * op_size;
    }
    while (n >= 8 * op_size) {
      DO_WORDS(w);
      DO_WORDS(w + 4);
      w += 8;
      n -= 8 * op_size;
    }
#else
    PREFETCH (w);
    PREFETCH (w + 4);
    while (n >= 12 * op_size) {
      PREFETCH(w + 8);
      DO_WORDS(w);
      w += 4;
      n -= 4 * op_size;
    }
    while (n >= 4 * op_size) {
      DO_WORDS(w);
      w += 4;
      n -= 4 * op_size;
    }
#endif

    while (n >= op_size) {
      op_t val = *w ^ mask_c;
      DO_WORD(w, val);
      w++;
      n -= op_size;
    }

    /*
     * Check bytewize for remaining bytes
     */
    p = (const unsigned char *) w;
    for (; n > 0; --n, ++p) {
      if (*p == ch)
        return (void *) p;
    }
  }
  return NULL;
}
