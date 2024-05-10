/*
 * Copyright (C) 2017 The Android Open Source Project
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <termios.h>

#include "header_checks.h"

static void termios_h() {
  TYPE(cc_t);
  TYPE(speed_t);
  TYPE(tcflag_t);

  TYPE(struct termios);
  STRUCT_MEMBER(struct termios, tcflag_t, c_iflag);
  STRUCT_MEMBER(struct termios, tcflag_t, c_oflag);
  STRUCT_MEMBER(struct termios, tcflag_t, c_cflag);
  STRUCT_MEMBER(struct termios, tcflag_t, c_lflag);
  STRUCT_MEMBER_ARRAY(struct termios, cc_t/*[]*/, c_cc);

#if !defined(__GLIBC__)  // Our glibc is too old.
  TYPE(struct winsize);
  STRUCT_MEMBER(struct winsize, unsigned short, ws_row);
  STRUCT_MEMBER(struct winsize, unsigned short, ws_col);
#endif

  MACRO(NCCS);

  MACRO(VEOF);
  MACRO(VEOL);
  MACRO(VERASE);
  MACRO(VINTR);
  MACRO(VKILL);
  MACRO(VMIN);
  MACRO(VQUIT);
  MACRO(VSTART);
  MACRO(VSTOP);
  MACRO(VSUSP);
  MACRO(VTIME);

  MACRO(BRKINT);
  MACRO(ICRNL);
  MACRO(IGNBRK);
  MACRO(IGNCR);
  MACRO(IGNPAR);
  MACRO(INLCR);
  MACRO(INPCK);
  MACRO(ISTRIP);
  MACRO(IXANY);
  MACRO(IXOFF);
  MACRO(IXON);
  MACRO(PARMRK);

  MACRO(OPOST);
  MACRO(ONLCR);
  MACRO(OCRNL);
  MACRO(ONOCR);
  MACRO(ONLRET);
  MACRO(OFDEL);
  MACRO(OFILL);
  MACRO(NLDLY);
  MACRO(NL0);
  MACRO(NL1);
  MACRO(CRDLY);
  MACRO(CR0);
  MACRO(CR1);
  MACRO(CR2);
  MACRO(CR3);
  MACRO(TABDLY);
  MACRO(TAB0);
  MACRO(TAB1);
  MACRO(TAB2);
  MACRO(TAB3);
  MACRO(BSDLY);
  MACRO(BS0);
  MACRO(BS1);
  MACRO(VTDLY);
  MACRO(VT0);
  MACRO(VT1);
  MACRO(FFDLY);
  MACRO(FF0);
  MACRO(FF1);

  MACRO(B0);
  MACRO(B50);
  MACRO(B75);
  MACRO(B110);
  MACRO(B134);
  MACRO(B150);
  MACRO(B200);
  MACRO(B300);
  MACRO(B600);
  MACRO(B1200);
  MACRO(B1800);
  MACRO(B2400);
  MACRO(B4800);
  MACRO(B9600);
  MACRO(B19200);
  MACRO(B38400);

  MACRO(CSIZE);
  MACRO(CS5);
  MACRO(CS6);
  MACRO(CS7);
  MACRO(CS8);
  MACRO(CSTOPB);
  MACRO(CREAD);
  MACRO(PARENB);
  MACRO(PARODD);
  MACRO(HUPCL);
  MACRO(CLOCAL);

  MACRO(ECHO);
  MACRO(ECHOE);
  MACRO(ECHOK);
  MACRO(ECHONL);
  MACRO(ICANON);
  MACRO(IEXTEN);
  MACRO(ISIG);
  MACRO(NOFLSH);
  MACRO(TOSTOP);

  MACRO(TCSANOW);
  MACRO(TCSADRAIN);
  MACRO(TCSAFLUSH);

  MACRO(TCIFLUSH);
  MACRO(TCIOFLUSH);
  MACRO(TCOFLUSH);

  MACRO(TCIOFF);
  MACRO(TCION);
  MACRO(TCOOFF);
  MACRO(TCOON);

  TYPE(pid_t);

  FUNCTION(cfgetispeed, speed_t (*f)(const struct termios*));
  FUNCTION(cfgetospeed, speed_t (*f)(const struct termios*));
  FUNCTION(cfsetispeed, int (*f)(struct termios*, speed_t));
  FUNCTION(cfsetospeed, int (*f)(struct termios*, speed_t));
  FUNCTION(tcdrain, int (*f)(int));
  FUNCTION(tcflow, int (*f)(int, int));
  FUNCTION(tcflush, int (*f)(int, int));
  FUNCTION(tcgetattr, int (*f)(int, struct termios*));
  FUNCTION(tcgetsid, pid_t (*f)(int));
#if !defined(__GLIBC__)  // Our glibc is too old.
  FUNCTION(tcgetwinsize, int (*f)(int, struct winsize*));
#endif
  FUNCTION(tcsendbreak, int (*f)(int, int));
  FUNCTION(tcsetattr, int (*f)(int, int, const struct termios*));
#if !defined(__GLIBC__)  // Our glibc is too old.
  FUNCTION(tcsetwinsize, int (*f)(int, const struct winsize*));
#endif
}
