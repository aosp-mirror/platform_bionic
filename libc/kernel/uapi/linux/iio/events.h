/*
 * This file is auto-generated. Modifications will be lost.
 *
 * See https://android.googlesource.com/platform/bionic/+/master/libc/kernel/
 * for more information.
 */
#ifndef _UAPI_IIO_EVENTS_H_
#define _UAPI_IIO_EVENTS_H_
#include <linux/ioctl.h>
#include <linux/types.h>
struct iio_event_data {
  __u64 id;
  __s64 timestamp;
};
#define IIO_GET_EVENT_FD_IOCTL _IOR('i', 0x90, int)
#define IIO_EVENT_CODE_EXTRACT_TYPE(mask) ((mask >> 56) & 0xFF)
#define IIO_EVENT_CODE_EXTRACT_DIR(mask) ((mask >> 48) & 0x7F)
#define IIO_EVENT_CODE_EXTRACT_CHAN_TYPE(mask) ((mask >> 32) & 0xFF)
#define IIO_EVENT_CODE_EXTRACT_CHAN(mask) ((__s16) (mask & 0xFFFF))
#define IIO_EVENT_CODE_EXTRACT_CHAN2(mask) ((__s16) (((mask) >> 16) & 0xFFFF))
#define IIO_EVENT_CODE_EXTRACT_MODIFIER(mask) ((mask >> 40) & 0xFF)
#define IIO_EVENT_CODE_EXTRACT_DIFF(mask) (((mask) >> 55) & 0x1)
#endif
