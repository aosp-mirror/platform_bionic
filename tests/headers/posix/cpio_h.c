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

#include <cpio.h>

#include "header_checks.h"

static void cpio_h() {
  MACRO_VALUE(C_IRUSR, 0400);
  MACRO_VALUE(C_IWUSR, 0200);
  MACRO_VALUE(C_IXUSR, 0100);

  MACRO_VALUE(C_IRGRP, 040);
  MACRO_VALUE(C_IWGRP, 020);
  MACRO_VALUE(C_IXGRP, 010);

  MACRO_VALUE(C_IROTH, 04);
  MACRO_VALUE(C_IWOTH, 02);
  MACRO_VALUE(C_IXOTH, 01);

  MACRO_VALUE(C_ISUID, 04000);
  MACRO_VALUE(C_ISGID, 02000);
  MACRO_VALUE(C_ISVTX, 01000);

  MACRO_VALUE(C_ISDIR, 040000);
  MACRO_VALUE(C_ISFIFO, 010000);
  MACRO_VALUE(C_ISREG, 0100000);
  MACRO_VALUE(C_ISBLK, 060000);
  MACRO_VALUE(C_ISCHR, 020000);

  MACRO_VALUE(C_ISCTG, 0110000);
  MACRO_VALUE(C_ISLNK, 0120000);
  MACRO_VALUE(C_ISSOCK, 0140000);

#if !defined(MAGIC)
#error MAGIC
#endif
}
