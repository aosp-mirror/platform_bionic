/****************************************************************************
 ****************************************************************************
 ***
 ***   This header was automatically generated from a Linux kernel header
 ***   of the same name, to make information necessary for userspace to
 ***   call into the kernel available to libc.  It contains only constants,
 ***   structures, and macros generated from the original header, and thus,
 ***   contains no copyrightable information.
 ***
 ***   To edit the content of this header, modify the corresponding
 ***   source file (e.g. under external/kernel-headers/original/) then
 ***   run bionic/libc/kernel/tools/update_all.py
 ***
 ***   Any manual change here will be lost the next time this script will
 ***   be run. You've been warned!
 ***
 ****************************************************************************
 ****************************************************************************/
#ifndef __ASM_IOCTLS_H
#define __ASM_IOCTLS_H
#include <asm/ioctl.h>
#define TCGETA 0x5401
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define TCSETA 0x5402
#define TCSETAW 0x5403
#define TCSETAF 0x5404
#define TCSBRK 0x5405
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define TCXONC 0x5406
#define TCFLSH 0x5407
#define TCGETS 0x540d
#define TCSETS 0x540e
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define TCSETSW 0x540f
#define TCSETSF 0x5410
#define TIOCEXCL 0x740d
#define TIOCNXCL 0x740e
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define TIOCOUTQ 0x7472
#define TIOCSTI 0x5472
#define TIOCMGET 0x741d
#define TIOCMBIS 0x741b
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define TIOCMBIC 0x741c
#define TIOCMSET 0x741a
#define TIOCPKT 0x5470
#define TIOCPKT_DATA 0x00
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define TIOCPKT_FLUSHREAD 0x01
#define TIOCPKT_FLUSHWRITE 0x02
#define TIOCPKT_STOP 0x04
#define TIOCPKT_START 0x08
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define TIOCPKT_NOSTOP 0x10
#define TIOCPKT_DOSTOP 0x20
#define TIOCPKT_IOCTL 0x40
#define TIOCSWINSZ _IOW('t', 103, struct winsize)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define TIOCGWINSZ _IOR('t', 104, struct winsize)
#define TIOCNOTTY 0x5471
#define TIOCSETD 0x7401
#define TIOCGETD 0x7400
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define FIOCLEX 0x6601
#define FIONCLEX 0x6602
#define FIOASYNC 0x667d
#define FIONBIO 0x667e
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define FIOQSIZE 0x667f
#define TIOCGLTC 0x7474
#define TIOCSLTC 0x7475
#define TIOCSPGRP _IOW('t', 118, int)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define TIOCGPGRP _IOR('t', 119, int)
#define TIOCCONS _IOW('t', 120, int)
#define FIONREAD 0x467f
#define TIOCINQ FIONREAD
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define TIOCGETP 0x7408
#define TIOCSETP 0x7409
#define TIOCSETN 0x740a
#define TIOCSBRK 0x5427
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define TIOCCBRK 0x5428
#define TIOCGSID 0x7416
#define TCGETS2 _IOR('T', 0x2A, struct termios2)
#define TCSETS2 _IOW('T', 0x2B, struct termios2)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define TCSETSW2 _IOW('T', 0x2C, struct termios2)
#define TCSETSF2 _IOW('T', 0x2D, struct termios2)
#define TIOCGPTN _IOR('T', 0x30, unsigned int)
#define TIOCSPTLCK _IOW('T', 0x31, int)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define TIOCGDEV _IOR('T', 0x32, unsigned int)
#define TIOCSIG _IOW('T', 0x36, int)
#define TIOCVHANGUP 0x5437
#define TIOCGPKT _IOR('T', 0x38, int)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define TIOCGPTLCK _IOR('T', 0x39, int)
#define TIOCGEXCL _IOR('T', 0x40, int)
#define TIOCSCTTY 0x5480
#define TIOCGSOFTCAR 0x5481
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define TIOCSSOFTCAR 0x5482
#define TIOCLINUX 0x5483
#define TIOCGSERIAL 0x5484
#define TIOCSSERIAL 0x5485
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define TCSBRKP 0x5486
#define TIOCSERCONFIG 0x5488
#define TIOCSERGWILD 0x5489
#define TIOCSERSWILD 0x548a
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define TIOCGLCKTRMIOS 0x548b
#define TIOCSLCKTRMIOS 0x548c
#define TIOCSERGSTRUCT 0x548d
#define TIOCSERGETLSR 0x548e
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define TIOCSERGETMULTI 0x548f
#define TIOCSERSETMULTI 0x5490
#define TIOCMIWAIT 0x5491
#define TIOCGICOUNT 0x5492
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#endif
