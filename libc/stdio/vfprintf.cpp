/*	$OpenBSD: vfprintf.c,v 1.71 2016/01/04 15:47:47 schwarze Exp $	*/
/*-
 * Copyright (c) 1990 The Regents of the University of California.
 * All rights reserved.
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

#define CHAR_TYPE char
#define FUNCTION_NAME __vfprintf
#define CHAR_TYPE_STRLEN strlen
#define CHAR_TYPE_STRNLEN strnlen
#define CHAR_TYPE_INF "INF"
#define CHAR_TYPE_inf "inf"
#define CHAR_TYPE_NAN "NAN"
#define CHAR_TYPE_nan "nan"
#define CHAR_TYPE_ORIENTATION -1
#include "printf_common.h"

int FUNCTION_NAME(FILE* fp, const CHAR_TYPE* fmt0, va_list ap) {
  int caller_errno = errno;
  int n, n2;
  CHAR_TYPE* cp;            /* handy char pointer (short term usage) */
  CHAR_TYPE sign;           /* sign prefix (' ', '+', '-', or \0) */
  int flags;           /* flags as above */
  int ret;             /* return value accumulator */
  int width;           /* width from format (%8d), or 0 */
  int prec;            /* precision from format; <0 for N/A */
  /*
   * We can decompose the printed representation of floating
   * point numbers into several parts, some of which may be empty:
   *
   * [+|-| ] [0x|0X] MMM . NNN [e|E|p|P] [+|-] ZZ
   *    A       B     ---C---      D       E   F
   *
   * A:	'sign' holds this value if present; '\0' otherwise
   * B:	ox[1] holds the 'x' or 'X'; '\0' if not hexadecimal
   * C:	cp points to the string MMMNNN.  Leading and trailing
   *	zeros are not in the string and must be added.
   * D:	expchar holds this character; '\0' if no exponent, e.g. %f
   * F:	at least two digits for decimal, at least one digit for hex
   */
  char* decimal_point = nullptr;
  int signflag; /* true if float is negative */
  union {       /* floating point arguments %[aAeEfFgG] */
    double dbl;
    long double ldbl;
  } fparg;
  int expt;                   /* integer value of exponent */
  char expchar;               /* exponent character: [eEpP\0] */
  char* dtoaend;              /* pointer to end of converted digits */
  int expsize;                /* character count for expstr */
  int lead;                   /* sig figs before decimal or group sep */
  int ndig;                   /* actual number of digits returned by dtoa */
  CHAR_TYPE expstr[MAXEXPDIG + 2]; /* buffer for exponent string: e+ZZZ */
  char* dtoaresult = nullptr;

  uintmax_t _umax;             /* integer arguments %[diouxX] */
  enum { OCT, DEC, HEX } base; /* base for %[diouxX] conversion */
  int dprec;                   /* a copy of prec if %[diouxX], 0 otherwise */
  int realsz;                  /* field size expanded by dprec */
  int size;                    /* size of converted field or string */
  const char* xdigs;           /* digits for %[xX] conversion */
#define NIOV 8
  struct __suio uio;       /* output information: summary */
  struct __siov iov[NIOV]; /* ... and individual io vectors */
  struct __siov* iovp; /* for PRINT macro */
  CHAR_TYPE buf[BUF];           /* buffer with space for digits of uintmax_t */
  CHAR_TYPE ox[2];              /* space for 0x; ox[1] is either x, X, or \0 */
  union arg* argtable;     /* args, built due to positional arg */
  union arg statargtable[STATIC_ARG_TBL_SIZE];
  size_t argtablesiz;
  int nextarg;   /* 1-based argument index */
  va_list orgap; /* original argument pointer */
  CHAR_TYPE* convbuf; /* buffer for wide/multibyte conversion */

  /*
   * Choose PADSIZE to trade efficiency vs. size.  If larger printf
   * fields occur frequently, increase PADSIZE and make the initialisers
   * below longer.
   */
#define PADSIZE 16 /* pad chunk size */
  static CHAR_TYPE blanks[PADSIZE] = {
    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '
  };
  static CHAR_TYPE zeroes[PADSIZE] = {
    '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0'
  };

  static const char xdigs_lower[] = "0123456789abcdef";
  static const char xdigs_upper[] = "0123456789ABCDEF";

#define PRINT(ptr, len)                   \
  do {                                    \
    iovp->iov_base = (ptr);               \
    iovp->iov_len = (len);                \
    uio.uio_resid += (len);               \
    iovp++;                               \
    if (++uio.uio_iovcnt >= NIOV) {       \
      if (helpers::sprint(fp, &uio)) goto error; \
      iovp = iov;                         \
    }                                     \
  } while (0)
#define FLUSH()                                          \
  do {                                                   \
    if (uio.uio_resid && helpers::sprint(fp, &uio)) goto error; \
    uio.uio_iovcnt = 0;                                  \
    iovp = iov;                                          \
  } while (0)

  _SET_ORIENTATION(fp, CHAR_TYPE_ORIENTATION);

  // Writing "" to a read only file returns EOF, not 0.
  if (cantwrite(fp)) {
    errno = EBADF;
    return EOF;
  }

  // Optimize writes to stderr and other unbuffered files).
  if ((fp->_flags & (__SNBF | __SWR | __SRW)) == (__SNBF | __SWR) && fp->_file >= 0) {
    return (__sbprintf(fp, fmt0, ap));
  }

  CHAR_TYPE* fmt = const_cast<CHAR_TYPE*>(fmt0);
  argtable = nullptr;
  nextarg = 1;
  va_copy(orgap, ap);
  uio.uio_iov = iovp = iov;
  uio.uio_resid = 0;
  uio.uio_iovcnt = 0;
  ret = 0;
  convbuf = nullptr;

  /*
   * Scan the format for conversions (`%' character).
   */
  for (;;) {
    int ch;
    for (cp = fmt; (ch = *fmt) != '\0' && ch != '%'; fmt++) continue;
    if (fmt != cp) {
      ptrdiff_t m = fmt - cp;
      if (m < 0 || m > INT_MAX - ret) goto overflow;
      PRINT(cp, m);
      ret += m;
    }
    if (ch == '\0') goto done;
    fmt++; /* skip over '%' */

    flags = 0;
    dprec = 0;
    width = 0;
    prec = -1;
    sign = '\0';
    ox[1] = '\0';

  rflag:
    ch = *fmt++;
  reswitch:
    switch (ch) {
      case ' ':
        /*
         * ``If the space and + flags both appear, the space
         * flag will be ignored.''
         *	-- ANSI X3J11
         */
        if (!sign) sign = ' ';
        goto rflag;
      case '#':
        flags |= ALT;
        goto rflag;
      case '\'':
        /* grouping not implemented */
        goto rflag;
      case '*':
        /*
         * ``A negative field width argument is taken as a
         * - flag followed by a positive field width.''
         *	-- ANSI X3J11
         * They don't exclude field widths read from args.
         */
        GETASTER(width);
        if (width >= 0) goto rflag;
        if (width == INT_MIN) goto overflow;
        width = -width;
        __BIONIC_FALLTHROUGH;
      case '-':
        flags |= LADJUST;
        goto rflag;
      case '+':
        sign = '+';
        goto rflag;
      case '.':
        if ((ch = *fmt++) == '*') {
          GETASTER(n);
          prec = n < 0 ? -1 : n;
          goto rflag;
        }
        n = 0;
        while (is_digit(ch)) {
          APPEND_DIGIT(n, ch);
          ch = *fmt++;
        }
        if (ch == '$') {
          nextarg = n;
          if (argtable == nullptr) {
            argtable = statargtable;
            if (__find_arguments(fmt0, orgap, &argtable, &argtablesiz) == -1) {
              ret = -1;
              goto error;
            }
          }
          goto rflag;
        }
        prec = n;
        goto reswitch;
      case '0':
        /*
         * ``Note that 0 is taken as a flag, not as the
         * beginning of a field width.''
         *	-- ANSI X3J11
         */
        flags |= ZEROPAD;
        goto rflag;
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
        n = 0;
        do {
          APPEND_DIGIT(n, ch);
          ch = *fmt++;
        } while (is_digit(ch));
        if (ch == '$') {
          nextarg = n;
          if (argtable == nullptr) {
            argtable = statargtable;
            if (__find_arguments(fmt0, orgap, &argtable, &argtablesiz) == -1) {
              ret = -1;
              goto error;
            }
          }
          goto rflag;
        }
        width = n;
        goto reswitch;
      case 'L':
        flags |= LONGDBL;
        goto rflag;
      case 'h':
        if (*fmt == 'h') {
          fmt++;
          flags |= CHARINT;
        } else {
          flags |= SHORTINT;
        }
        goto rflag;
      case 'j':
        flags |= MAXINT;
        goto rflag;
      case 'l':
        if (*fmt == 'l') {
          fmt++;
          flags |= LLONGINT;
        } else {
          flags |= LONGINT;
        }
        goto rflag;
      case 'q':
        flags |= LLONGINT;
        goto rflag;
      case 't':
        flags |= PTRINT;
        goto rflag;
      case 'z':
        flags |= SIZEINT;
        goto rflag;
      case 'C':
        flags |= LONGINT;
        __BIONIC_FALLTHROUGH;
      case 'c':
        if (flags & LONGINT) {
          mbstate_t mbs;
          size_t mbseqlen;

          memset(&mbs, 0, sizeof(mbs));
          mbseqlen = wcrtomb(buf, (wchar_t)GETARG(wint_t), &mbs);
          if (mbseqlen == (size_t)-1) {
            ret = -1;
            goto error;
          }
          cp = buf;
          size = (int)mbseqlen;
        } else {
          *(cp = buf) = GETARG(int);
          size = 1;
        }
        sign = '\0';
        break;
      case 'D':
        flags |= LONGINT;
        __BIONIC_FALLTHROUGH;
      case 'd':
      case 'i':
        _umax = SARG();
        if ((intmax_t)_umax < 0) {
          _umax = -_umax;
          sign = '-';
        }
        base = DEC;
        goto number;
      case 'a':
      case 'A':
        if (ch == 'a') {
          ox[1] = 'x';
          xdigs = xdigs_lower;
          expchar = 'p';
        } else {
          ox[1] = 'X';
          xdigs = xdigs_upper;
          expchar = 'P';
        }
        if (prec >= 0) prec++;
        if (dtoaresult) __freedtoa(dtoaresult);
        if (flags & LONGDBL) {
          fparg.ldbl = GETARG(long double);
          dtoaresult = cp = __hldtoa(fparg.ldbl, xdigs, prec, &expt, &signflag, &dtoaend);
          if (dtoaresult == nullptr) {
            errno = ENOMEM;
            goto error;
          }
        } else {
          fparg.dbl = GETARG(double);
          dtoaresult = cp = __hdtoa(fparg.dbl, xdigs, prec, &expt, &signflag, &dtoaend);
          if (dtoaresult == nullptr) {
            errno = ENOMEM;
            goto error;
          }
        }
        if (prec < 0) prec = dtoaend - dtoaresult;
        if (expt == INT_MAX) ox[1] = '\0';
        goto fp_common;
      case 'e':
      case 'E':
        expchar = ch;
        if (prec < 0) /* account for digit before decpt */
          prec = DEFPREC + 1;
        else
          prec++;
        goto fp_begin;
      case 'f':
      case 'F':
        expchar = '\0';
        goto fp_begin;
      case 'g':
      case 'G':
        expchar = ch - ('g' - 'e');
        if (prec == 0) prec = 1;
      fp_begin:
        if (prec < 0) prec = DEFPREC;
        if (dtoaresult) __freedtoa(dtoaresult);
        if (flags & LONGDBL) {
          fparg.ldbl = GETARG(long double);
          dtoaresult = cp = __ldtoa(&fparg.ldbl, expchar ? 2 : 3, prec, &expt, &signflag, &dtoaend);
          if (dtoaresult == nullptr) {
            errno = ENOMEM;
            goto error;
          }
        } else {
          fparg.dbl = GETARG(double);
          dtoaresult = cp = __dtoa(fparg.dbl, expchar ? 2 : 3, prec, &expt, &signflag, &dtoaend);
          if (dtoaresult == nullptr) {
            errno = ENOMEM;
            goto error;
          }
          if (expt == 9999) expt = INT_MAX;
        }
      fp_common:
        if (signflag) sign = '-';
        if (expt == INT_MAX) { /* inf or nan */
          if (*cp == 'N') {
            cp = const_cast<CHAR_TYPE*>((ch >= 'a') ? CHAR_TYPE_nan : CHAR_TYPE_NAN);
          } else {
            cp = const_cast<CHAR_TYPE*>((ch >= 'a') ? CHAR_TYPE_inf : CHAR_TYPE_INF);
          }
          size = 3;
          flags &= ~ZEROPAD;
          break;
        }
        flags |= FPT;
        ndig = dtoaend - cp;
        if (ch == 'g' || ch == 'G') {
          if (expt > -4 && expt <= prec) {
            /* Make %[gG] smell like %[fF] */
            expchar = '\0';
            if (flags & ALT)
              prec -= expt;
            else
              prec = ndig - expt;
            if (prec < 0) prec = 0;
          } else {
            /*
             * Make %[gG] smell like %[eE], but
             * trim trailing zeroes if no # flag.
             */
            if (!(flags & ALT)) prec = ndig;
          }
        }
        if (expchar) {
          expsize = exponent(expstr, expt - 1, expchar);
          size = expsize + prec;
          if (prec > 1 || flags & ALT) ++size;
        } else {
          /* space for digits before decimal point */
          if (expt > 0)
            size = expt;
          else /* "0" */
            size = 1;
          /* space for decimal pt and following digits */
          if (prec || flags & ALT) size += prec + 1;
          lead = expt;
        }
        break;
      case 'n':
        __fortify_fatal("%%n not allowed on Android");
      case 'm':
        cp = strerror_r(caller_errno, buf, sizeof(buf));
        goto string;
      case 'O':
        flags |= LONGINT;
        __BIONIC_FALLTHROUGH;
      case 'o':
        _umax = UARG();
        base = OCT;
        goto nosign;
      case 'p':
        /*
         * ``The argument shall be a pointer to void.  The
         * value of the pointer is converted to a sequence
         * of printable characters, in an implementation-
         * defined manner.''
         *	-- ANSI X3J11
         */
        _umax = (u_long)GETARG(void*);
        base = HEX;
        xdigs = xdigs_lower;
        ox[1] = 'x';
        goto nosign;
      case 'S':
        flags |= LONGINT;
        __BIONIC_FALLTHROUGH;
      case 's':
        if (flags & LONGINT) {
          wchar_t* wcp;

          free(convbuf);
          convbuf = nullptr;
          if ((wcp = GETARG(wchar_t*)) == nullptr) {
            cp = const_cast<char*>("(null)");
          } else {
            convbuf = helpers::wcsconv(wcp, prec);
            if (convbuf == nullptr) {
              ret = -1;
              goto error;
            }
            cp = convbuf;
          }
        } else if ((cp = GETARG(char*)) == nullptr) {
          cp = const_cast<char*>("(null)");
        }
  string:
        if (prec >= 0) {
          size = CHAR_TYPE_STRNLEN(cp, prec);
        } else {
          size_t len;

          if ((len = CHAR_TYPE_STRLEN(cp)) > INT_MAX) goto overflow;
          size = (int)len;
        }
        sign = '\0';
        break;
      case 'U':
        flags |= LONGINT;
        __BIONIC_FALLTHROUGH;
      case 'u':
        _umax = UARG();
        base = DEC;
        goto nosign;
      case 'X':
        xdigs = xdigs_upper;
        goto hex;
      case 'x':
        xdigs = xdigs_lower;
      hex:
        _umax = UARG();
        base = HEX;
        /* leading 0x/X only if non-zero */
        if (flags & ALT && _umax != 0) ox[1] = ch;

        /* unsigned conversions */
      nosign:
        sign = '\0';
        /*
         * ``... diouXx conversions ... if a precision is
         * specified, the 0 flag will be ignored.''
         *	-- ANSI X3J11
         */
      number:
        if ((dprec = prec) >= 0) flags &= ~ZEROPAD;

        /*
         * ``The result of converting a zero value with an
         * explicit precision of zero is no characters.''
         *	-- ANSI X3J11
         */
        cp = buf + BUF;
        if (_umax != 0 || prec != 0) {
          /*
           * Unsigned mod is hard, and unsigned mod
           * by a constant is easier than that by
           * a variable; hence this switch.
           */
          switch (base) {
            case OCT:
              do {
                *--cp = to_char(_umax & 7);
                _umax >>= 3;
              } while (_umax);
              /* handle octal leading 0 */
              if (flags & ALT && *cp != '0') *--cp = '0';
              break;

            case DEC:
              /* many numbers are 1 digit */
              while (_umax >= 10) {
                *--cp = to_char(_umax % 10);
                _umax /= 10;
              }
              *--cp = to_char(_umax);
              break;

            case HEX:
              do {
                *--cp = xdigs[_umax & 15];
                _umax >>= 4;
              } while (_umax);
              break;

            default:
              abort();
          }
        }
        size = buf + BUF - cp;
        if (size > BUF) abort(); /* should never happen */
        break;
      default: /* "%?" prints ?, unless ? is NUL */
        if (ch == '\0') goto done;
        /* pretend it was %c with argument ch */
        cp = buf;
        *cp = ch;
        size = 1;
        sign = '\0';
        break;
    }

    /*
     * All reasonable formats wind up here.  At this point, `cp'
     * points to a string which (if not flags&LADJUST) should be
     * padded out to `width' places.  If flags&ZEROPAD, it should
     * first be prefixed by any sign or other prefix; otherwise,
     * it should be blank padded before the prefix is emitted.
     * After any left-hand padding and prefixing, emit zeroes
     * required by a decimal %[diouxX] precision, then print the
     * string proper, then emit zeroes required by any leftover
     * floating precision; finally, if LADJUST, pad with blanks.
     *
     * Compute actual size, so we know how much to pad.
     * size excludes decimal prec; realsz includes it.
     */
    realsz = dprec > size ? dprec : size;
    if (sign) realsz++;
    if (ox[1]) realsz += 2;

    /* right-adjusting blank padding */
    if ((flags & (LADJUST | ZEROPAD)) == 0) PAD(width - realsz, blanks);

    /* prefix */
    if (sign) PRINT(&sign, 1);
    if (ox[1]) { /* ox[1] is either x, X, or \0 */
      ox[0] = '0';
      PRINT(ox, 2);
    }

    /* right-adjusting zero padding */
    if ((flags & (LADJUST | ZEROPAD)) == ZEROPAD) PAD(width - realsz, zeroes);

    /* leading zeroes from decimal precision */
    PAD(dprec - size, zeroes);

    /* the string or number proper */
    if ((flags & FPT) == 0) {
      PRINT(cp, size);
    } else { /* glue together f_p fragments */
      if (decimal_point == nullptr) decimal_point = nl_langinfo(RADIXCHAR);
      if (!expchar) { /* %[fF] or sufficiently short %[gG] */
        if (expt <= 0) {
          PRINT(zeroes, 1);
          if (prec || flags & ALT) PRINT(decimal_point, 1);
          PAD(-expt, zeroes);
          /* already handled initial 0's */
          prec += expt;
        } else {
          PRINTANDPAD(cp, dtoaend, lead, zeroes);
          cp += lead;
          if (prec || flags & ALT) PRINT(decimal_point, 1);
        }
        PRINTANDPAD(cp, dtoaend, prec, zeroes);
      } else { /* %[eE] or sufficiently long %[gG] */
        if (prec > 1 || flags & ALT) {
          buf[0] = *cp++;
          buf[1] = *decimal_point;
          PRINT(buf, 2);
          PRINT(cp, ndig - 1);
          PAD(prec - ndig, zeroes);
        } else { /* XeYYY */
          PRINT(cp, 1);
        }
        PRINT(expstr, expsize);
      }
    }
    /* left-adjusting padding (always blank) */
    if (flags & LADJUST) PAD(width - realsz, blanks);

    /* finally, adjust ret */
    if (width < realsz) width = realsz;
    if (width > INT_MAX - ret) goto overflow;
    ret += width;

    FLUSH(); /* copy out the I/O vectors */
  }
done:
  FLUSH();
error:
  va_end(orgap);
  if (__sferror(fp)) ret = -1;
  goto finish;

overflow:
  errno = ENOMEM;
  ret = -1;

finish:
  free(convbuf);
  if (dtoaresult) __freedtoa(dtoaresult);
  if (argtable != nullptr && argtable != statargtable) {
    munmap(argtable, argtablesiz);
    argtable = nullptr;
  }
  return (ret);
}
