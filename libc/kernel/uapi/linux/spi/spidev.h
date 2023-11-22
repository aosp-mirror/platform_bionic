/*
 * This file is auto-generated. Modifications will be lost.
 *
 * See https://android.googlesource.com/platform/bionic/+/master/libc/kernel/
 * for more information.
 */
#ifndef SPIDEV_H
#define SPIDEV_H
#include <linux/types.h>
#include <linux/ioctl.h>
#include <linux/spi/spi.h>
#define SPI_IOC_MAGIC 'k'
struct spi_ioc_transfer {
  __u64 tx_buf;
  __u64 rx_buf;
  __u32 len;
  __u32 speed_hz;
  __u16 delay_usecs;
  __u8 bits_per_word;
  __u8 cs_change;
  __u8 tx_nbits;
  __u8 rx_nbits;
  __u8 word_delay_usecs;
  __u8 pad;
};
#define SPI_MSGSIZE(N) ((((N) * (sizeof(struct spi_ioc_transfer))) < (1 << _IOC_SIZEBITS)) ? ((N) * (sizeof(struct spi_ioc_transfer))) : 0)
#define SPI_IOC_MESSAGE(N) _IOW(SPI_IOC_MAGIC, 0, char[SPI_MSGSIZE(N)])
#define SPI_IOC_RD_MODE _IOR(SPI_IOC_MAGIC, 1, __u8)
#define SPI_IOC_WR_MODE _IOW(SPI_IOC_MAGIC, 1, __u8)
#define SPI_IOC_RD_LSB_FIRST _IOR(SPI_IOC_MAGIC, 2, __u8)
#define SPI_IOC_WR_LSB_FIRST _IOW(SPI_IOC_MAGIC, 2, __u8)
#define SPI_IOC_RD_BITS_PER_WORD _IOR(SPI_IOC_MAGIC, 3, __u8)
#define SPI_IOC_WR_BITS_PER_WORD _IOW(SPI_IOC_MAGIC, 3, __u8)
#define SPI_IOC_RD_MAX_SPEED_HZ _IOR(SPI_IOC_MAGIC, 4, __u32)
#define SPI_IOC_WR_MAX_SPEED_HZ _IOW(SPI_IOC_MAGIC, 4, __u32)
#define SPI_IOC_RD_MODE32 _IOR(SPI_IOC_MAGIC, 5, __u32)
#define SPI_IOC_WR_MODE32 _IOW(SPI_IOC_MAGIC, 5, __u32)
#endif
