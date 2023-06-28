/*	$OpenBSD: vfwscanf.c,v 1.4 2014/03/19 05:17:01 guenther Exp $ */
/*-
 * Copyright (c) 1990, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Chris Torek.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include "scanf_common.h"
// An interpretive version of __sccl from vfscanf.c --- a table of all wchar_t values would
// be a little too expensive, and some kind of compressed version isn't worth the trouble.
static inline bool in_ccl(wchar_t wc, const wchar_t* ccl) {
  // Is this a negated set?
  bool member_result = true;
  if (*ccl == '^') {
    member_result = false;
    ++ccl;
  }

  // The first character may be ']' or '-' without being special.
  if (*ccl == '-' || *ccl == ']') {
    // A literal match?
    if (*ccl == wc) return member_result;
    ++ccl;
  }

  while (*ccl && *ccl != ']') {
    // The last character may be '-' without being special.
    if (*ccl == '-' && ccl[1] != '\0' && ccl[1] != ']') {
      wchar_t first = *(ccl - 1);
      wchar_t last = *(ccl + 1);
      if (first <= last) {
        // In the range?
        if (wc >= first && wc <= last) return member_result;
        ccl += 2;
        continue;
      }
      // A '-' is not considered to be part of a range if the character after
      // is not greater than the character before, so fall through...
    }
    // A literal match?
    if (*ccl == wc) return member_result;
    ++ccl;
  }
  return !member_result;
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wframe-larger-than="

/*
 * vfwscanf
 */
int __vfwscanf(FILE* __restrict fp, const wchar_t* __restrict fmt, __va_list ap) {
  wint_t c;               /* character from format, or conversion */
  size_t width;           /* field width, or 0 */
  wchar_t* p;             /* points into all kinds of strings */
  int n;                  /* handy integer */
  int flags;              /* flags as defined above */
  wchar_t* p0;            /* saves original value of p when necessary */
  int nassigned;          /* number of fields assigned */
  int nconversions;       /* number of conversions */
  int nread;              /* number of characters consumed from fp */
  int base;               /* base argument to strtoimax/strtouimax */
  wchar_t buf[BUF];       /* buffer for numeric conversions */
  const wchar_t* ccl;
  wint_t wi;              /* handy wint_t */
  char* mbp;              /* multibyte string pointer for %c %s %[ */
  size_t nconv;           /* number of bytes in mb. conversion */
  char mbbuf[MB_LEN_MAX]; /* temporary mb. character buffer */
  mbstate_t mbs;

  _SET_ORIENTATION(fp, ORIENT_CHARS);

  nassigned = 0;
  nconversions = 0;
  nread = 0;
  base = 0; /* XXX just to keep gcc happy */
  for (;;) {
    c = *fmt++;
    if (c == 0) {
      return (nassigned);
    }
    if (iswspace(c)) {
      while ((c = __fgetwc_unlock(fp)) != WEOF && iswspace(c))
        ;
      if (c != WEOF) __ungetwc(c, fp);
      continue;
    }
    if (c != '%') goto literal;
    width = 0;
    flags = 0;
    /*
     * switch on the format.  continue if done;
     * break once format type is derived.
     */
  again:
    c = *fmt++;
  reswitch:
    switch (c) {
      case '%':
      literal:
        if ((wi = __fgetwc_unlock(fp)) == WEOF) goto input_failure;
        if (wi != c) {
          __ungetwc(wi, fp);
          goto match_failure;
        }
        nread++;
        continue;

      case '*':
        flags |= SUPPRESS;
        goto again;
      case 'j':
        flags |= MAXINT;
        goto again;
      case 'L':
        flags |= LONGDBL;
        goto again;
      case 'h':
        if (*fmt == 'h') {
          fmt++;
          flags |= SHORTSHORT;
        } else {
          flags |= SHORT;
        }
        goto again;
      case 'l':
        if (*fmt == 'l') {
          fmt++;
          flags |= LLONG;
        } else {
          flags |= LONG;
        }
        goto again;
      case 'q':
        flags |= LLONG; /* deprecated */
        goto again;
      case 't':
        flags |= PTRINT;
        goto again;
      case 'z':
        flags |= SIZEINT;
        goto again;

      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
        width = width * 10 + c - '0';
        goto again;

      /*
       * Conversions.
       * Those marked `compat' are for 4.[123]BSD compatibility.
       */
      case 'b':
        c = CT_INT;
        base = 2;
        flags |= PFBOK; /* enable 0b prefixing */
        break;

      case 'D': /* compat */
        flags |= LONG;
        __BIONIC_FALLTHROUGH;
      case 'd':
        c = CT_INT;
        base = 10;
        break;

      case 'i':
        c = CT_INT;
        base = 0;
        break;

      case 'O': /* compat */
        flags |= LONG;
        __BIONIC_FALLTHROUGH;
      case 'o':
        c = CT_INT;
        flags |= UNSIGNED;
        base = 8;
        break;

      case 'u':
        c = CT_INT;
        flags |= UNSIGNED;
        base = 10;
        break;

      case 'w': {
        int size = 0;
        bool fast = false;
        c = *fmt++;
        if (c == 'f') {
          fast = true;
          c = *fmt++;
        }
        while (is_digit(c)) {
          APPEND_DIGIT(size, c);
          c = *fmt++;
        }
        flags |= w_to_flag(size, fast);
        goto reswitch;
      }

      case 'X':
      case 'x':
        flags |= PFXOK; /* enable 0x prefixing */
        c = CT_INT;
        flags |= UNSIGNED;
        base = 16;
        break;

      case 'e':
      case 'E':
      case 'f':
      case 'F':
      case 'g':
      case 'G':
      case 'a':
      case 'A':
        c = CT_FLOAT;
        break;

      case 's':
        c = CT_STRING;
        break;

      case '[':
        ccl = fmt;
        if (*fmt == '^') fmt++;
        if (*fmt == ']') fmt++;
        while (*fmt != '\0' && *fmt != ']') fmt++;
        fmt++;
        flags |= NOSKIP;
        c = CT_CCL;
        break;

      case 'c':
        flags |= NOSKIP;
        c = CT_CHAR;
        break;

      case 'p': /* pointer format is like hex */
        flags |= POINTER | PFXOK;
        c = CT_INT;
        flags |= UNSIGNED;
        base = 16;
        break;

      case 'n':
        nconversions++;
        if (flags & SUPPRESS) continue;
        if (flags & SHORTSHORT)
          *va_arg(ap, signed char*) = nread;
        else if (flags & SHORT)
          *va_arg(ap, short*) = nread;
        else if (flags & LONG)
          *va_arg(ap, long*) = nread;
        else if (flags & SIZEINT)
          *va_arg(ap, ssize_t*) = nread;
        else if (flags & PTRINT)
          *va_arg(ap, ptrdiff_t*) = nread;
        else if (flags & LLONG)
          *va_arg(ap, long long*) = nread;
        else if (flags & MAXINT)
          *va_arg(ap, intmax_t*) = nread;
        else
          *va_arg(ap, int*) = nread;
        continue;

      /*
       * Disgusting backwards compatibility hacks.	XXX
       */
      case '\0': /* compat */
        return (EOF);

      default: /* compat */
        if (iswupper(c)) flags |= LONG;
        c = CT_INT;
        base = 10;
        break;
    }

    /*
     * Consume leading white space, except for formats
     * that suppress this.
     */
    if ((flags & NOSKIP) == 0) {
      while ((wi = __fgetwc_unlock(fp)) != WEOF && iswspace(wi)) nread++;
      if (wi == WEOF) goto input_failure;
      __ungetwc(wi, fp);
    }

    /*
     * Do the conversion.
     */
    switch (c) {
      case CT_CHAR:
        /* scan arbitrary characters (sets NOSKIP) */
        if (width == 0) width = 1;
        if (flags & LONG) {
          if (!(flags & SUPPRESS)) p = va_arg(ap, wchar_t*);
          n = 0;
          while (width-- != 0 && (wi = __fgetwc_unlock(fp)) != WEOF) {
            if (!(flags & SUPPRESS)) *p++ = (wchar_t)wi;
            n++;
          }
          if (n == 0) goto input_failure;
          nread += n;
          if (!(flags & SUPPRESS)) nassigned++;
        } else {
          if (!(flags & SUPPRESS)) mbp = va_arg(ap, char*);
          n = 0;
          memset(&mbs, 0, sizeof(mbs));
          while (width != 0 && (wi = __fgetwc_unlock(fp)) != WEOF) {
            if (width >= MB_CUR_MAX && !(flags & SUPPRESS)) {
              nconv = wcrtomb(mbp, wi, &mbs);
              if (nconv == (size_t)-1) goto input_failure;
            } else {
              nconv = wcrtomb(mbbuf, wi, &mbs);
              if (nconv == (size_t)-1) goto input_failure;
              if (nconv > width) {
                __ungetwc(wi, fp);
                break;
              }
              if (!(flags & SUPPRESS)) memcpy(mbp, mbbuf, nconv);
            }
            if (!(flags & SUPPRESS)) mbp += nconv;
            width -= nconv;
            n++;
          }
          if (n == 0) goto input_failure;
          nread += n;
          if (!(flags & SUPPRESS)) nassigned++;
        }
        nconversions++;
        break;

      case CT_CCL:
      case CT_STRING:
        // CT_CCL: scan a (nonempty) character class (sets NOSKIP).
        // CT_STRING: like CCL, but zero-length string OK, & no NOSKIP.
        if (width == 0) width = (size_t)~0; // 'infinity'.
        if ((flags & SUPPRESS) && (flags & LONG)) {
          n = 0;
          while ((wi = __fgetwc_unlock(fp)) != WEOF && width-- != 0 && ((c == CT_CCL && in_ccl(wi, ccl)) || (c == CT_STRING && !iswspace(wi)))) n++;
          if (wi != WEOF) __ungetwc(wi, fp);
        } else if (flags & LONG) {
          p0 = p = va_arg(ap, wchar_t*);
          while ((wi = __fgetwc_unlock(fp)) != WEOF && width-- != 0 && ((c == CT_CCL && in_ccl(wi, ccl)) || (c == CT_STRING && !iswspace(wi)))) {
            *p++ = (wchar_t)wi;
          }
          if (wi != WEOF) __ungetwc(wi, fp);
          n = p - p0;
        } else {
          if (!(flags & SUPPRESS)) mbp = va_arg(ap, char*);
          n = 0;
          memset(&mbs, 0, sizeof(mbs));
          while ((wi = __fgetwc_unlock(fp)) != WEOF && width != 0 && ((c == CT_CCL && in_ccl(wi, ccl)) || (c == CT_STRING && !iswspace(wi)))) {
            if (width >= MB_CUR_MAX && !(flags & SUPPRESS)) {
              nconv = wcrtomb(mbp, wi, &mbs);
              if (nconv == (size_t)-1) goto input_failure;
            } else {
              nconv = wcrtomb(mbbuf, wi, &mbs);
              if (nconv == (size_t)-1) goto input_failure;
              if (nconv > width) break;
              if (!(flags & SUPPRESS)) memcpy(mbp, mbbuf, nconv);
            }
            if (!(flags & SUPPRESS)) mbp += nconv;
            width -= nconv;
            n++;
          }
          if (wi != WEOF) __ungetwc(wi, fp);
        }
        if (c == CT_CCL && n == 0) goto match_failure;
        if (!(flags & SUPPRESS)) {
          if (flags & LONG) {
            *p = L'\0';
          } else {
            *mbp = '\0';
          }
          ++nassigned;
        }
        nread += n;
        nconversions++;
        break;

      case CT_INT:
        /* scan an integer as if by strtoimax/strtoumax */
        if (width == 0 || width > sizeof(buf) / sizeof(*buf) - 1)
          width = sizeof(buf) / sizeof(*buf) - 1;
        flags |= SIGNOK | NDIGITS | NZDIGITS;
        for (p = buf; width; width--) {
          c = __fgetwc_unlock(fp);
          /*
           * Switch on the character; `goto ok'
           * if we accept it as a part of number.
           */
          switch (c) {
            /*
             * The digit 0 is always legal, but is
             * special.  For %i conversions, if no
             * digits (zero or nonzero) have been
             * scanned (only signs), we will have
             * base==0.  In that case, we should set
             * it to 8 and enable 0b/0x prefixing.
             * Also, if we have not scanned zero digits
             * before this, do not turn off prefixing
             * (someone else will turn it off if we
             * have scanned any nonzero digits).
             */
            case '0':
              if (base == 0) {
                base = 8;
                flags |= PFBOK | PFXOK;
              }
              if (flags & NZDIGITS) {
                flags &= ~(SIGNOK | NZDIGITS | NDIGITS);
              } else {
                flags &= ~(SIGNOK | PFBOK | PFXOK | NDIGITS);
              }
              goto ok;

            /* 1 through 7 always legal */
            case 'B':
            case 'b':
              // Is this 'b' potentially part of an "0b" prefix?
              if ((flags & PFBOK) && p == buf + 1 + !!(flags & HAVESIGN)) {
                base = 2;
                flags &= ~PFBOK;
                goto ok;
              }
              // No? Fall through and see if it's a hex digit instead then...
              __BIONIC_FALLTHROUGH;
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case 'A':
            case 'C':
            case 'D':
            case 'E':
            case 'F':
            case 'a':
            case 'c':
            case 'd':
            case 'e':
            case 'f':
              if (base == 0) base = 10;
              if (base != 16 && (int)(c - '0') >= base) break; /* not legal here */
              flags &= ~(SIGNOK | PFBOK | PFXOK | NDIGITS);
              goto ok;

            /* sign ok only as first character */
            case '+':
            case '-':
              if (flags & SIGNOK) {
                flags &= ~SIGNOK;
                flags |= HAVESIGN;
                goto ok;
              }
              break;

            /*
             * x ok iff flag still set and 2nd char (or
             * 3rd char if we have a sign).
             */
            case 'x':
            case 'X':
              if ((flags & PFXOK) && p == buf + 1 + !!(flags & HAVESIGN)) {
                base = 16; /* if %i */
                flags &= ~PFXOK;
                goto ok;
              }
              break;
          }

          /*
           * If we got here, c is not a legal character
           * for a number.  Stop accumulating digits.
           */
          if (c != WEOF) __ungetwc(c, fp);
          break;
        ok:
          /*
           * c is legal: store it and look at the next.
           */
          *p++ = (wchar_t)c;
        }
        /*
         * If we had only a sign, it is no good; push back the sign.
         * If the number was `[-+]0[BbXx]`, push back and treat it
         * as `[-+]0`.
         */
        if (flags & NDIGITS) {
          if (p > buf) __ungetwc(*--p, fp);
          goto match_failure;
        }
        c = p[-1];
        if ((base == 2 && (c == 'b' || c == 'B')) || c == 'x' || c == 'X') {
          --p;
          __ungetwc(c, fp);
        }
        if ((flags & SUPPRESS) == 0) {
          uintmax_t res;

          *p = '\0';
          if (flags & UNSIGNED)
            res = wcstoimax(buf, NULL, base);
          else
            res = wcstoumax(buf, NULL, base);
          if (flags & POINTER)
            *va_arg(ap, void**) = (void*)(uintptr_t)res;
          else if (flags & MAXINT)
            *va_arg(ap, intmax_t*) = res;
          else if (flags & LLONG)
            *va_arg(ap, long long*) = res;
          else if (flags & SIZEINT)
            *va_arg(ap, ssize_t*) = res;
          else if (flags & PTRINT)
            *va_arg(ap, ptrdiff_t*) = res;
          else if (flags & LONG)
            *va_arg(ap, long*) = res;
          else if (flags & SHORT)
            *va_arg(ap, short*) = res;
          else if (flags & SHORTSHORT)
            *va_arg(ap, signed char*) = res;
          else
            *va_arg(ap, int*) = res;
          nassigned++;
        }
        nread += p - buf;
        nconversions++;
        break;

      case CT_FLOAT:
        /* scan a floating point number as if by strtod */
        if (width == 0 || width > sizeof(buf) / sizeof(*buf) - 1)
          width = sizeof(buf) / sizeof(*buf) - 1;
        if ((width = wparsefloat(fp, buf, buf + width)) == 0) goto match_failure;
        if ((flags & SUPPRESS) == 0) {
          if (flags & LONGDBL) {
            long double res = wcstold(buf, &p);
            *va_arg(ap, long double*) = res;
          } else if (flags & LONG) {
            double res = wcstod(buf, &p);
            *va_arg(ap, double*) = res;
          } else {
            float res = wcstof(buf, &p);
            *va_arg(ap, float*) = res;
          }
          if (p - buf != (ptrdiff_t)width) abort();
          nassigned++;
        }
        nread += width;
        nconversions++;
        break;
    }
  }
input_failure:
  return (nconversions != 0 ? nassigned : EOF);
match_failure:
  return (nassigned);
}
#pragma GCC diagnostic pop
