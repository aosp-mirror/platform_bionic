/*	$OpenBSD: vfscanf.c,v 1.31 2014/03/19 05:17:01 guenther Exp $ */
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

static const unsigned char* __sccl(char*, const unsigned char*);

/*
 * Internal, unlocked version of vfscanf
 */
int __svfscanf(FILE* fp, const char* fmt0, va_list ap) {
  const unsigned char* fmt = reinterpret_cast<const unsigned char*>(fmt0);
  int c;            /* character from format, or conversion */
  size_t width;     /* field width, or 0 */
  char* p;
  wchar_t* wcp;
  size_t n;
  int flags;        /* flags as defined above */
  int nassigned;    /* number of fields assigned */
  int nread;        /* number of characters consumed from fp */
  int base;         /* base argument to strtoimax/strtouimax */
  char ccltab[256]; /* character class table for %[...] */
  char buf[BUF];    /* buffer for numeric conversions */
  size_t nconv;     /* length of multibyte sequence converted */
  mbstate_t mbs;
  void* allocation = nullptr; // Allocated but unassigned result for %mc/%ms/%m[.
  size_t capacity = 0; // Number of char/wchar_t units allocated in `allocation`.

  _SET_ORIENTATION(fp, ORIENT_BYTES);

  nassigned = 0;
  nread = 0;
  for (;;) {
    c = *fmt++;
    if (c == 0) return nassigned;
    if (isspace(c)) {
      while ((fp->_r > 0 || __srefill(fp) == 0) && isspace(*fp->_p)) nread++, fp->_r--, fp->_p++;
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
        if (fp->_r <= 0 && __srefill(fp)) goto input_failure;
        if (*fp->_p != c) goto match_failure;
        fp->_r--, fp->_p++;
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
      case 'm':
        flags |= ALLOCATE;
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
        memset(ccltab, 1, 256);
        ccltab['\t'] = ccltab['\n'] = ccltab['\v'] = ccltab['\f'] = ccltab['\r'] = ccltab[' '] = 0;
        c = CT_STRING;
        break;

      case '[':
        fmt = __sccl(ccltab, fmt);
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
        if (flags & SUPPRESS) continue;
        if (flags & SHORTSHORT) {
          *va_arg(ap, signed char*) = nread;
        } else if (flags & SHORT) {
          *va_arg(ap, short*) = nread;
        } else if (flags & LONG) {
          *va_arg(ap, long*) = nread;
        } else if (flags & SIZEINT) {
          *va_arg(ap, ssize_t*) = nread;
        } else if (flags & PTRINT) {
          *va_arg(ap, ptrdiff_t*) = nread;
        } else if (flags & LLONG) {
          *va_arg(ap, long long*) = nread;
        } else if (flags & MAXINT) {
          *va_arg(ap, intmax_t*) = nread;
        } else {
          *va_arg(ap, int*) = nread;
        }
        continue;

      /*
       * Disgusting backwards compatibility hacks.	XXX
       */
      case '\0': /* compat */
        return EOF;

      default: /* compat */
        if (isupper(c)) flags |= LONG;
        c = CT_INT;
        base = 10;
        break;
    }

    if ((flags & ALLOCATE) != 0 && c > CT_STRING) {
      __fortify_fatal("scanf 'm' only works with %%c/%%s/%%[");
    }
    if ((flags & (ALLOCATE|SUPPRESS)) == (ALLOCATE|SUPPRESS)) {
      __fortify_fatal("scanf 'm' makes no sense with '*'");
    }

    /*
     * We have a conversion that requires input.
     */
    if (fp->_r <= 0 && __srefill(fp)) goto input_failure;

    /*
     * Consume leading white space, except for formats
     * that suppress this.
     */
    if ((flags & NOSKIP) == 0) {
      while (isspace(*fp->_p)) {
        nread++;
        if (--fp->_r > 0) {
          fp->_p++;
        } else if (__srefill(fp)) {
          goto input_failure;
        }
      }
      /*
       * Note that there is at least one character in
       * the buffer, so conversions that do not set NOSKIP
       * ca no longer result in an input failure.
       */
    }

    /*
     * Do the conversion.
     */
    switch (c) {
      case CT_CHAR:
        /* scan arbitrary characters (sets NOSKIP) */
        if (width == 0) width = 1;
        if (flags & LONG) {
          if (flags & ALLOCATE) {
            allocation = wcp = reinterpret_cast<wchar_t*>(malloc(width * sizeof(wchar_t)));
            if (allocation == nullptr) goto allocation_failure;
          } else if (flags & SUPPRESS) {
            wcp = nullptr;
          } else {
            wcp = va_arg(ap, wchar_t*);
          }
          size_t bytes = 0;
          while (width != 0) {
            if (bytes == MB_CUR_MAX) {
              fp->_flags |= __SERR;
              goto input_failure;
            }
            buf[bytes++] = *fp->_p;
            fp->_p++;
            fp->_r--;
            memset(&mbs, 0, sizeof(mbs));
            nconv = mbrtowc(wcp, buf, bytes, &mbs);
            if (nconv == BIONIC_MULTIBYTE_RESULT_ILLEGAL_SEQUENCE) {
              fp->_flags |= __SERR;
              goto input_failure;
            }
            if (nconv == 0 && !(flags & SUPPRESS)) *wcp = L'\0';
            if (nconv != BIONIC_MULTIBYTE_RESULT_INCOMPLETE_SEQUENCE) {
              nread += bytes;
              width--;
              if (!(flags & SUPPRESS)) wcp++;
              bytes = 0;
            }
            if (fp->_r <= 0 && __srefill(fp)) {
              if (bytes != 0) {
                fp->_flags |= __SERR;
                goto input_failure;
              }
              break;
            }
          }
          if (allocation != nullptr) {
            *va_arg(ap, wchar_t**) = reinterpret_cast<wchar_t*>(allocation);
            allocation = nullptr;
          }
          if (!(flags & SUPPRESS)) nassigned++;
        } else if (flags & SUPPRESS) {
          size_t sum = 0;
          for (;;) {
            if ((n = fp->_r) < width) {
              sum += n;
              width -= n;
              fp->_p += n;
              if (__srefill(fp)) {
                if (sum == 0) goto input_failure;
                break;
              }
            } else {
              sum += width;
              fp->_r -= width;
              fp->_p += width;
              break;
            }
          }
          nread += sum;
        } else {
          if (flags & ALLOCATE) {
            allocation = p = reinterpret_cast<char*>(malloc(width));
            if (allocation == nullptr) goto allocation_failure;
          } else {
            p = va_arg(ap, char*);
          }
          size_t r = fread(p, 1, width, fp);
          if (r == 0) goto input_failure;
          if (allocation != nullptr) {
            *va_arg(ap, char**) = reinterpret_cast<char*>(allocation);
            allocation = nullptr;
          }
          nread += r;
          nassigned++;
        }
        break;

      case CT_CCL:
      case CT_STRING:
        // CT_CCL: scan a (nonempty) character class (sets NOSKIP).
        // CT_STRING: like CCL, but zero-length string OK, & no NOSKIP.
        if (width == 0) width = SIZE_MAX;
        if (flags & LONG) {
          // TODO: since no-one cares, replace this with a simple fgetwc loop?
          n = 0;
          if (flags & ALLOCATE) {
            capacity = MIN(width, 32);
            allocation = wcp = reinterpret_cast<wchar_t*>(malloc(sizeof(wchar_t) * capacity));
            if (allocation == nullptr) goto allocation_failure;
          } else if (flags & SUPPRESS) {
            wcp = nullptr;
          } else {
            wcp = va_arg(ap, wchar_t*);
          }
          size_t bytes = 0;
          while ((c == CT_CCL || !isspace(*fp->_p)) && width != 0) {
            if (bytes == MB_CUR_MAX) {
              fp->_flags |= __SERR;
              goto input_failure;
            }
            buf[bytes++] = *fp->_p;
            fp->_p++;
            fp->_r--;
            wchar_t wc = L'\0';
            memset(&mbs, 0, sizeof(mbs));
            nconv = mbrtowc(&wc, buf, bytes, &mbs);
            if (nconv == BIONIC_MULTIBYTE_RESULT_ILLEGAL_SEQUENCE) {
              fp->_flags |= __SERR;
              goto input_failure;
            }
            if (nconv != BIONIC_MULTIBYTE_RESULT_INCOMPLETE_SEQUENCE) {
              if ((c == CT_CCL && wctob(wc) != EOF && !ccltab[wctob(wc)]) || (c == CT_STRING && iswspace(wc))) {
                while (bytes != 0) {
                  bytes--;
                  ungetc(buf[bytes], fp);
                }
                break;
              }
              if (wcp) wcp[n] = wc;
              n++;
              if (allocation != nullptr && n == capacity) {
                capacity *= 2;
                wchar_t* new_allocation =
                    reinterpret_cast<wchar_t*>(realloc(allocation, sizeof(wchar_t) * capacity));
                if (new_allocation == nullptr) goto allocation_failure;
                allocation = wcp = new_allocation;
              }
              nread += bytes;
              width--;
              bytes = 0;
            }
            if (fp->_r <= 0 && __srefill(fp)) {
              if (bytes != 0) {
                fp->_flags |= __SERR;
                goto input_failure;
              }
              break;
            }
          }
          if (c == CT_CCL && bytes != 0) {
            fp->_flags |= __SERR;
            goto input_failure;
          }
          if (allocation != nullptr) {
            *va_arg(ap, wchar_t**) = reinterpret_cast<wchar_t*>(allocation);
            allocation = nullptr;
          }
        } else if (flags & SUPPRESS) {
          n = 0;
          while (ccltab[*fp->_p]) {
            n++, fp->_r--, fp->_p++;
            if (--width == 0) break;
            if (fp->_r <= 0 && __srefill(fp)) {
              if (c == CT_CCL && n == 0) goto input_failure;
              break;
            }
          }
          nread += n;
        } else {
          if (flags & ALLOCATE) {
            capacity = MIN(width, 32);
            allocation = p = reinterpret_cast<char*>(malloc(capacity));
            if (allocation == nullptr) goto allocation_failure;
          } else {
            p = va_arg(ap, char*);
          }
          n = 0;
          while (ccltab[*fp->_p]) {
            fp->_r--;
            p[n++] = *fp->_p++;
            if (allocation != nullptr && n == capacity) {
              capacity *= 2;
              char* new_allocation = reinterpret_cast<char*>(realloc(allocation, capacity));
              if (new_allocation == nullptr) goto allocation_failure;
              allocation = p = new_allocation;
            }
            if (--width == 0) break;
            if (fp->_r <= 0 && __srefill(fp)) {
              if (c == CT_CCL && n == 0) goto input_failure;
              break;
            }
          }
          nread += n;
          if (allocation != nullptr) {
            *va_arg(ap, char**) = reinterpret_cast<char*>(allocation);
            allocation = nullptr;
          }
        }
        if (c == CT_CCL && n == 0) goto match_failure;
        if (!(flags & SUPPRESS)) {
          if (flags & LONG) {
            wcp[n] = L'\0';
          } else {
            p[n] = '\0';
          }
          ++nassigned;
        }
        break;

      case CT_INT:
        /* scan an integer as if by strtoimax/strtoumax */
#ifdef hardway
        if (width == 0 || width > sizeof(buf) - 1) width = sizeof(buf) - 1;
#else
        /* size_t is unsigned, hence this optimisation */
        if (--width > sizeof(buf) - 2) width = sizeof(buf) - 2;
        width++;
#endif
        flags |= SIGNOK | NDIGITS | NZDIGITS;
        for (p = buf; width; width--) {
          c = *fp->_p;
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
            case 'B':
            case 'b':
              // Is this 'b' or 'B' potentially part of an "0b" prefix?
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
              if (base != 16 && (c - '0') >= base) break; /* not legal here */
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
          break;
        ok:
          /*
           * c is legal: store it and look at the next.
           */
          *p++ = c;
          if (--fp->_r > 0)
            fp->_p++;
          else if (__srefill(fp))
            break; /* EOF */
        }
        /*
         * If we had only a sign, it is no good; push back the sign.
         * If the number was `[-+]0[BbXx]`, push back and treat it
         * as `[-+]0`.
         */
        if (flags & NDIGITS) {
          if (p > buf) ungetc(*reinterpret_cast<u_char*>(--p), fp);
          goto match_failure;
        }
        c = reinterpret_cast<u_char*>(p)[-1];
        if ((base == 2 && (c == 'b' || c == 'B')) || c == 'x' || c == 'X') {
          --p;
          (void)ungetc(c, fp);
        }
        if ((flags & SUPPRESS) == 0) {
          uintmax_t res;

          *p = '\0';
          if (flags & UNSIGNED) {
            res = strtoumax(buf, nullptr, base);
          } else {
            res = strtoimax(buf, nullptr, base);
          }
          if (flags & POINTER) {
            *va_arg(ap, void**) = reinterpret_cast<void*>(res);
          } else if (flags & MAXINT) {
            *va_arg(ap, intmax_t*) = res;
          } else if (flags & LLONG) {
            *va_arg(ap, long long*) = res;
          } else if (flags & SIZEINT) {
            *va_arg(ap, ssize_t*) = res;
          } else if (flags & PTRINT) {
            *va_arg(ap, ptrdiff_t*) = res;
          } else if (flags & LONG) {
            *va_arg(ap, long*) = res;
          } else if (flags & SHORT) {
            *va_arg(ap, short*) = res;
          } else if (flags & SHORTSHORT) {
            *va_arg(ap, signed char*) = res;
          } else {
            *va_arg(ap, int*) = res;
          }
          nassigned++;
        }
        nread += p - buf;
        break;

      case CT_FLOAT:
        /* scan a floating point number as if by strtod */
        if (width == 0 || width > sizeof(buf) - 1) width = sizeof(buf) - 1;
        if ((width = parsefloat(fp, buf, buf + width)) == 0) goto match_failure;
        if ((flags & SUPPRESS) == 0) {
          if (flags & LONGDBL) {
            long double res = strtold(buf, &p);
            *va_arg(ap, long double*) = res;
          } else if (flags & LONG) {
            double res = strtod(buf, &p);
            *va_arg(ap, double*) = res;
          } else {
            float res = strtof(buf, &p);
            *va_arg(ap, float*) = res;
          }
          if (static_cast<size_t>(p - buf) != width) abort();
          nassigned++;
        }
        nread += width;
        break;
    }
  }
allocation_failure:
input_failure:
  free(allocation);
  if (nassigned == 0) nassigned = -1;
match_failure:
  return nassigned;
}

/*
 * Fill in the given table from the scanset at the given format
 * (just after `[').  Return a pointer to the character past the
 * closing `]'.  The table has a 1 wherever characters should be
 * considered part of the scanset.
 */
static const unsigned char* __sccl(char* tab, const unsigned char* fmt) {
  int c, n, v;

  /* first `clear' the whole table */
  c = *fmt++; /* first char hat => negated scanset */
  if (c == '^') {
    v = 1;      /* default => accept */
    c = *fmt++; /* get new first char */
  } else {
    v = 0; /* default => reject */
  }
  memset(tab, v, 256);
  if (c == 0) return (fmt - 1); /* format ended before closing ] */

  /*
   * Now set the entries corresponding to the actual scanset
   * to the opposite of the above.
   *
   * The first character may be ']' (or '-') without being special;
   * the last character may be '-'.
   */
  v = 1 - v;
  for (;;) {
    tab[c] = v; /* take character c */
  doswitch:
    n = *fmt++; /* and examine the next */
    switch (n) {
      case 0: /* format ended too soon */
        return (fmt - 1);

      case '-':
        /*
         * A scanset of the form
         *	[01+-]
         * is defined as `the digit 0, the digit 1,
         * the character +, the character -', but
         * the effect of a scanset such as
         *	[a-zA-Z0-9]
         * is implementation defined.  The V7 Unix
         * scanf treats `a-z' as `the letters a through
         * z', but treats `a-a' as `the letter a, the
         * character -, and the letter a'.
         *
         * For compatibility, the `-' is not considerd
         * to define a range if the character following
         * it is either a close bracket (required by ANSI)
         * or is not numerically greater than the character
         * we just stored in the table (c).
         */
        n = *fmt;
        if (n == ']' || n < c) {
          c = '-';
          break; /* resume the for(;;) */
        }
        fmt++;
        do { /* fill in the range */
          tab[++c] = v;
        } while (c < n);
#if 1 /* XXX another disgusting compatibility hack */
        /*
         * Alas, the V7 Unix scanf also treats formats
         * such as [a-c-e] as `the letters a through e'.
         * This too is permitted by the standard....
         */
        goto doswitch;
#else
        c = *fmt++;
        if (c == 0) return (fmt - 1);
        if (c == ']') return (fmt);
#endif
        break;

      case ']': /* end of scanset */
        return fmt;

      default: /* just another character */
        c = n;
        break;
    }
  }
  /* NOTREACHED */
}
