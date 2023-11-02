/*
 * This file is auto-generated. Modifications will be lost.
 *
 * See https://android.googlesource.com/platform/bionic/+/master/libc/kernel/
 * for more information.
 */
#ifndef __INCLUDE_UAPI_SOUND_SOF_USER_HEADER_H__
#define __INCLUDE_UAPI_SOUND_SOF_USER_HEADER_H__
#include <linux/types.h>
struct sof_abi_hdr {
  __u32 magic;
  __u32 type;
  __u32 size;
  __u32 abi;
  __u32 reserved[4];
  __u32 data[];
} __attribute__((__packed__));
#define SOF_MANIFEST_DATA_TYPE_NHLT 1
struct sof_manifest_tlv {
  __le32 type;
  __le32 size;
  __u8 data[];
};
struct sof_manifest {
  __le16 abi_major;
  __le16 abi_minor;
  __le16 abi_patch;
  __le16 count;
  struct sof_manifest_tlv items[];
};
#endif
