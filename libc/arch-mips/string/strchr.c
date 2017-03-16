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
#define op_size     sizeof (op_t)

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

#define DO_BYTE(i)                  \
  if (a.b.B##i != ch) {             \
    if(a.b.B##i == '\0') return 0;  \
    p++;                            \
  } else                            \
    return (char *)p;

#define DO_WORD(w, cnt) {                            \
  op_t val = w[cnt] ^ mask_c;                        \
  if ((((w[cnt] - mask_1) & ~w[cnt]) & mask_128) ||  \
    (((val - mask_1) & ~val) & mask_128)) {          \
    return do_bytes(w + cnt, ch);                    \
  }                                                  \
}

static inline char * __attribute__ ((always_inline))
do_bytes (const op_t* w, unsigned char ch)
{
  bitfields_t a;
  unsigned char* p = (unsigned char *) w;
  a.v = *w;
#if __mips64
  DO_BYTE(0)
  DO_BYTE(1)
  DO_BYTE(2)
  DO_BYTE(3)
  DO_BYTE(4)
  DO_BYTE(5)
  DO_BYTE(6)
  DO_BYTE(7)
#else
  DO_BYTE(0)
  DO_BYTE(1)
  DO_BYTE(2)
  DO_BYTE(3)
#endif
  return (char *)p;
}

char* strchr(const char* s, int c) __overloadable
{
  const op_t *w;
  op_t mask_1, mask_128, mask_c;
  const unsigned char ch = c;
  unsigned char* p = (unsigned char *) s;

  /*
   * Check byte by byte till initial alignment
   */
  for ( ; *p != ch && ((size_t) p % op_size) != 0; p++)
    if (*p == '\0')
      return 0;

  if (*p != ch) {
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
     * Check word/dword wize after initial alignment till character match
     * or end of string
     */
    while (1) {
      DO_WORD(w, 0)
      DO_WORD(w, 1)
      DO_WORD(w, 2)
      DO_WORD(w, 3)
      w += 4;
    }
  }

  return (char *)p;
}
