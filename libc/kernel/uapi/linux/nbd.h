/*
 * This file is auto-generated. Modifications will be lost.
 *
 * See https://android.googlesource.com/platform/bionic/+/master/libc/kernel/
 * for more information.
 */
#ifndef _UAPILINUX_NBD_H
#define _UAPILINUX_NBD_H
#include <linux/types.h>
#define NBD_SET_SOCK _IO(0xab, 0)
#define NBD_SET_BLKSIZE _IO(0xab, 1)
#define NBD_SET_SIZE _IO(0xab, 2)
#define NBD_DO_IT _IO(0xab, 3)
#define NBD_CLEAR_SOCK _IO(0xab, 4)
#define NBD_CLEAR_QUE _IO(0xab, 5)
#define NBD_PRINT_DEBUG _IO(0xab, 6)
#define NBD_SET_SIZE_BLOCKS _IO(0xab, 7)
#define NBD_DISCONNECT _IO(0xab, 8)
#define NBD_SET_TIMEOUT _IO(0xab, 9)
#define NBD_SET_FLAGS _IO(0xab, 10)
enum {
  NBD_CMD_READ = 0,
  NBD_CMD_WRITE = 1,
  NBD_CMD_DISC = 2,
  NBD_CMD_FLUSH = 3,
  NBD_CMD_TRIM = 4,
  NBD_CMD_WRITE_ZEROES = 6,
};
#define NBD_FLAG_HAS_FLAGS (1 << 0)
#define NBD_FLAG_READ_ONLY (1 << 1)
#define NBD_FLAG_SEND_FLUSH (1 << 2)
#define NBD_FLAG_SEND_FUA (1 << 3)
#define NBD_FLAG_ROTATIONAL (1 << 4)
#define NBD_FLAG_SEND_TRIM (1 << 5)
#define NBD_FLAG_SEND_WRITE_ZEROES (1 << 6)
#define NBD_FLAG_CAN_MULTI_CONN (1 << 8)
#define NBD_CMD_FLAG_FUA (1 << 16)
#define NBD_CMD_FLAG_NO_HOLE (1 << 17)
#define NBD_CFLAG_DESTROY_ON_DISCONNECT (1 << 0)
#define NBD_CFLAG_DISCONNECT_ON_CLOSE (1 << 1)
#define NBD_REQUEST_MAGIC 0x25609513
#define NBD_REPLY_MAGIC 0x67446698
struct nbd_request {
  __be32 magic;
  __be32 type;
  union {
    __be64 cookie;
    char handle[8];
  };
  __be64 from;
  __be32 len;
} __attribute__((packed));
struct nbd_reply {
  __be32 magic;
  __be32 error;
  union {
    __be64 cookie;
    char handle[8];
  };
};
#endif
