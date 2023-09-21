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
#ifndef _UAPI_SPI_H
#define _UAPI_SPI_H
#include <linux/const.h>
#define SPI_CPHA _BITUL(0)
#define SPI_CPOL _BITUL(1)
#define SPI_MODE_0 (0 | 0)
#define SPI_MODE_1 (0 | SPI_CPHA)
#define SPI_MODE_2 (SPI_CPOL | 0)
#define SPI_MODE_3 (SPI_CPOL | SPI_CPHA)
#define SPI_MODE_X_MASK (SPI_CPOL | SPI_CPHA)
#define SPI_CS_HIGH _BITUL(2)
#define SPI_LSB_FIRST _BITUL(3)
#define SPI_3WIRE _BITUL(4)
#define SPI_LOOP _BITUL(5)
#define SPI_NO_CS _BITUL(6)
#define SPI_READY _BITUL(7)
#define SPI_TX_DUAL _BITUL(8)
#define SPI_TX_QUAD _BITUL(9)
#define SPI_RX_DUAL _BITUL(10)
#define SPI_RX_QUAD _BITUL(11)
#define SPI_CS_WORD _BITUL(12)
#define SPI_TX_OCTAL _BITUL(13)
#define SPI_RX_OCTAL _BITUL(14)
#define SPI_3WIRE_HIZ _BITUL(15)
#define SPI_RX_CPHA_FLIP _BITUL(16)
#define SPI_MOSI_IDLE_LOW _BITUL(17)
#define SPI_MODE_USER_MASK (_BITUL(18) - 1)
#endif
