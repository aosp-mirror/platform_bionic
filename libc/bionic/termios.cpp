/*
 * Copyright (C) 2008 The Android Open Source Project
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

#include <errno.h>
#include <termios.h>
#include <unistd.h>

static speed_t cfgetspeed(const termios* s) {
  return (s->c_cflag & CBAUD);
}

speed_t cfgetispeed(const termios* s) {
  return cfgetspeed(s);
}

speed_t cfgetospeed(const termios* s) {
  return cfgetspeed(s);
}

void cfmakeraw(termios* s) {
  s->c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL|IXON);
  s->c_oflag &= ~OPOST;
  s->c_lflag &= ~(ECHO|ECHONL|ICANON|ISIG|IEXTEN);
  s->c_cflag &= ~(CSIZE|PARENB);
  s->c_cflag |= CS8;
}

int cfsetispeed(termios* s, speed_t speed) {
  return cfsetspeed(s, speed);
}

int cfsetospeed(termios* s, speed_t speed) {
  return cfsetspeed(s, speed);
}

int cfsetspeed(termios* s, speed_t speed) {
  // TODO: check 'speed' is valid.
  s->c_cflag = (s->c_cflag & ~CBAUD) | (speed & CBAUD);
  return 0;
}

int tcdrain(int fd) {
  // A non-zero argument to TCSBRK means "don't send a break".
  // The drain is a side-effect of the ioctl!
  return ioctl(fd, TCSBRK, static_cast<unsigned long>(1));
}

int tcflow(int fd, int action) {
  return ioctl(fd, TCXONC, static_cast<unsigned long>(action));
}

int tcflush(int fd, int queue) {
  return ioctl(fd, TCFLSH, static_cast<unsigned long>(queue));
}

int tcgetattr(int fd, termios* s) {
  return ioctl(fd, TCGETS, s);
}

pid_t tcgetsid(int fd) {
  pid_t sid;
  if (ioctl(fd, TIOCGSID, &sid) == -1) {
    return -1;
  }
  return sid;
}

int tcsendbreak(int fd, int duration) {
  return ioctl(fd, TCSBRKP, static_cast<unsigned long>(duration));
}

int tcsetattr(int fd, int optional_actions, const termios* s) {
  int cmd;
  switch (optional_actions) {
    case TCSANOW: cmd = TCSETS; break;
    case TCSADRAIN: cmd = TCSETSW; break;
    case TCSAFLUSH: cmd = TCSETSF; break;
    default: errno = EINVAL; return -1;
  }
  return ioctl(fd, cmd, s);
}

pid_t tcgetpgrp(int fd) {
  pid_t pid;
  if (ioctl(fd, TIOCGPGRP, &pid) == -1) {
    return -1;
  }
  return pid;
}

int tcsetpgrp(int fd, pid_t pid) {
  return ioctl(fd, TIOCSPGRP, &pid);
}
