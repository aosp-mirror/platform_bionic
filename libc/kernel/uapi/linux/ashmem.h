/*
 * This file is auto-generated. Modifications will be lost.
 *
 * See https://android.googlesource.com/platform/bionic/+/master/libc/kernel/
 * for more information.
 */
#ifndef _UAPI_LINUX_ASHMEM_H
#define _UAPI_LINUX_ASHMEM_H
#include <linux/ioctl.h>
#include <linux/types.h>
#define ASHMEM_NAME_LEN 256
#define ASHMEM_NAME_DEF "dev/ashmem"
#define ASHMEM_NOT_PURGED 0
#define ASHMEM_WAS_PURGED 1
#define ASHMEM_IS_UNPINNED 0
#define ASHMEM_IS_PINNED 1
struct ashmem_pin {
  __u32 offset;
  __u32 len;
};
#define __ASHMEMIOC 0x77
#define ASHMEM_SET_NAME _IOW(__ASHMEMIOC, 1, char[ASHMEM_NAME_LEN])
#define ASHMEM_GET_NAME _IOR(__ASHMEMIOC, 2, char[ASHMEM_NAME_LEN])
#define ASHMEM_SET_SIZE _IOW(__ASHMEMIOC, 3, size_t)
#define ASHMEM_GET_SIZE _IO(__ASHMEMIOC, 4)
#define ASHMEM_SET_PROT_MASK _IOW(__ASHMEMIOC, 5, unsigned long)
#define ASHMEM_GET_PROT_MASK _IO(__ASHMEMIOC, 6)
#define ASHMEM_PIN _IOW(__ASHMEMIOC, 7, struct ashmem_pin)
#define ASHMEM_UNPIN _IOW(__ASHMEMIOC, 8, struct ashmem_pin)
#define ASHMEM_GET_PIN_STATUS _IO(__ASHMEMIOC, 9)
#define ASHMEM_PURGE_ALL_CACHES _IO(__ASHMEMIOC, 10)
#define ASHMEM_GET_FILE_ID _IOR(__ASHMEMIOC, 11, unsigned long)
#endif
