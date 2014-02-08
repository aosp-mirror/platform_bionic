/*
 * Copyright (c) 2010 MIPS Technologies, Inc.
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
 *      * Neither the name of MIPS Technologies Inc. nor the names of its
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
#include "mips-string-ops.h"

#define do_strlen_word(__av) {\
    if (detect_zero(x,x,_01s,_80s)) break;\
    x = __av;\
    cnt += sizeof (unsigned);\
    }

#define do_strlen_byte(__x) {\
  if ((bx.b.B##__x) == 0) break;\
  ++cnt;\
  }

#if SMOKE_TEST_MIPS_STRLEN
#define strlen my_strlen
#endif

size_t
strlen (const char *_a)
{
  int cnt = 0;
  unsigned long x;

  /* align the string to word boundary so we can do word at a time.  */
  if ((cvt_ptr_to (unsigned long, _a) & (sizeof (unsigned long) - 1)) != 0)
    {
      if ((cvt_ptr_to (unsigned long, _a) & 1) != 0)
	{
	  if (get_byte (_a, 0) == 0)
	    return cnt;
	  /* set bit 1 so 2-bytes are checked and incremented. */
	  inc_ptr_as (char *, _a, 1);
	  ++cnt;
	}
      if ((cvt_ptr_to (unsigned long, _a) & 2) != 0)
	{
	  if (get_byte (_a, 0) == 0)
	    return cnt + 0;
	  if (get_byte (_a, 1) == 0)
	    return cnt + 1;
	  inc_ptr_as (char *, _a, 2);
	  cnt += 2;
	}
    }

#if __mips64
#error strlen: mips64 check for 4-byte alignment not implemented.
#endif

  if (1)
    {
      def_and_set_01 (_01s);
      def_and_set_80 (_80s);

      /* as advantagous as it is to performance, this code cannot pre-load
         the following word, nor can it prefetch the next line at the start
         of the loop since the string can be at the end of a page with the
         following page unmapped. There are tests in the suite to catch
         any attempt to go beyond the current word. */
      x = get_word (_a, 0);
      while (1)
	{
	  /* doing 8 words should cover most strings.  */
	  do_strlen_word (get_word (_a, 1));
	  do_strlen_word (get_word (_a, 2));
	  do_strlen_word (get_word (_a, 3));
	  do_strlen_word (get_word (_a, 4));
	  do_strlen_word (get_word (_a, 5));
	  do_strlen_word (get_word (_a, 6));
	  do_strlen_word (get_word (_a, 7));
	  do_strlen_word (get_word (_a, 8));
	  inc_ptr_as (unsigned long*, _a, 8);
	}
    }
  while (1)
    {
      /* pull apart the last word processed and find the zero.  */
      bitfields_t bx;
      bx.v = x;
#if __mips64
      do_strlen_byte (0);
      do_strlen_byte (1);
      do_strlen_byte (2);
      do_strlen_byte (3);
      do_strlen_byte (4);
      do_strlen_byte (5);
      do_strlen_byte (6);
#else
      do_strlen_byte (0);
      do_strlen_byte (1);
      do_strlen_byte (2);
#endif
      /* last byte is zero */
      break;
    }
  return cnt;
}

#undef do_strlen_byte
#undef do_strlen_word

#if SMOKE_TEST_MIPS_STRLEN
#include <stdio.h>
char str1[] = "DHRYSTONE PROGRAM, 1'ST STRING";
char str2[] = "DHRYSTONE PROGRAM, 2'ST STRING";

char str3[] = "another string";
char str4[] = "another";

char str5[] = "somes tring";
char str6[] = "somes_tring";

char str7[16], str8[16];

static char *
chk (unsigned long mine, unsigned long libs, int *errors)
{
  static char answer[1024];
  char *result = mine == libs ? "PASS" : "FAIL";
  sprintf (answer, "new_strlen=%d: lib_strlen=%d: %s!", mine, libs, result);
  if (mine != libs)
    (*errors)++;
  return answer;
}

int
main (int argc, char **argv)
{
  int errors = 0;
  /* set -1 in one position */
  str6[5] = 0xff;
  /* set zero in same position with junk in following 3 */
  str7[0] = str8[0] = 0;
  str7[1] = 0xff;
  str7[2] = 'a';
  str7[3] = 2;
  str8[1] = 's';
  str8[2] = -2;
  str8[3] = 0;

  fprintf (stderr, "========== mips_strlen%s test...\n",
	   argv[0] ? argv[0] : "unknown strlen");
#define P(__x,__y) {\
    int a = my_strlen(__x + __y);\
    int b = (strlen)(__x + __y) /* library version */;\
    fprintf(stderr,"%s+%d: %s\n",#__x,__y,chk(a,b,&errors));\
    }

  P (str1, 0);
  P (str1, 1);
  P (str1, 2);
  P (str1, 3);

  P (str2, 0);
  P (str2, 1);
  P (str2, 2);
  P (str2, 3);

  P (str3, 0);
  P (str3, 1);
  P (str3, 2);
  P (str3, 3);

  P (str4, 0);
  P (str4, 1);
  P (str4, 2);
  P (str4, 3);

  P (str5, 0);
  P (str5, 1);
  P (str5, 2);
  P (str5, 3);

  P (str6, 0);
  P (str6, 1);
  P (str6, 2);
  P (str6, 3);

  P (str7, 0);
  P (str7, 1);
  P (str7, 2);
  P (str7, 3);

  P (str8, 0);
  P (str8, 1);
  P (str8, 2);
  P (str8, 3);

  return errors;
}
#endif
