/*
 * This file is auto-generated. Modifications will be lost.
 *
 * See https://android.googlesource.com/platform/bionic/+/master/libc/kernel/
 * for more information.
 */
#ifndef _UAPI__LINUX_FUNCTIONFS_H__
#define _UAPI__LINUX_FUNCTIONFS_H__
#include <linux/const.h>
#include <linux/types.h>
#include <linux/ioctl.h>
#include <linux/usb/ch9.h>
enum {
  FUNCTIONFS_DESCRIPTORS_MAGIC = 1,
  FUNCTIONFS_STRINGS_MAGIC = 2,
  FUNCTIONFS_DESCRIPTORS_MAGIC_V2 = 3,
};
enum functionfs_flags {
  FUNCTIONFS_HAS_FS_DESC = 1,
  FUNCTIONFS_HAS_HS_DESC = 2,
  FUNCTIONFS_HAS_SS_DESC = 4,
  FUNCTIONFS_HAS_MS_OS_DESC = 8,
  FUNCTIONFS_VIRTUAL_ADDR = 16,
  FUNCTIONFS_EVENTFD = 32,
  FUNCTIONFS_ALL_CTRL_RECIP = 64,
  FUNCTIONFS_CONFIG0_SETUP = 128,
};
struct usb_endpoint_descriptor_no_audio {
  __u8 bLength;
  __u8 bDescriptorType;
  __u8 bEndpointAddress;
  __u8 bmAttributes;
  __le16 wMaxPacketSize;
  __u8 bInterval;
} __attribute__((packed));
struct usb_dfu_functional_descriptor {
  __u8 bLength;
  __u8 bDescriptorType;
  __u8 bmAttributes;
  __le16 wDetachTimeOut;
  __le16 wTransferSize;
  __le16 bcdDFUVersion;
} __attribute__((packed));
#define DFU_FUNC_ATT_CAN_DOWNLOAD _BITUL(0)
#define DFU_FUNC_ATT_CAN_UPLOAD _BITUL(1)
#define DFU_FUNC_ATT_MANIFEST_TOLERANT _BITUL(2)
#define DFU_FUNC_ATT_WILL_DETACH _BITUL(3)
struct usb_functionfs_descs_head_v2 {
  __le32 magic;
  __le32 length;
  __le32 flags;
} __attribute__((packed));
struct usb_functionfs_descs_head {
  __le32 magic;
  __le32 length;
  __le32 fs_count;
  __le32 hs_count;
} __attribute__((packed, deprecated));
struct usb_os_desc_header {
  __u8 interface;
  __le32 dwLength;
  __le16 bcdVersion;
  __le16 wIndex;
  union {
    struct {
      __u8 bCount;
      __u8 Reserved;
    };
    __le16 wCount;
  };
} __attribute__((packed));
struct usb_ext_compat_desc {
  __u8 bFirstInterfaceNumber;
  __u8 Reserved1;
  __struct_group(, IDs,, __u8 CompatibleID[8];
  __u8 SubCompatibleID[8];
 );
  __u8 Reserved2[6];
};
struct usb_ext_prop_desc {
  __le32 dwSize;
  __le32 dwPropertyDataType;
  __le16 wPropertyNameLength;
} __attribute__((packed));
#define USB_FFS_DMABUF_TRANSFER_MASK 0x0
struct usb_ffs_dmabuf_transfer_req {
  int fd;
  __u32 flags;
  __u64 length;
} __attribute__((packed));
struct usb_functionfs_strings_head {
  __le32 magic;
  __le32 length;
  __le32 str_count;
  __le32 lang_count;
} __attribute__((packed));
enum usb_functionfs_event_type {
  FUNCTIONFS_BIND,
  FUNCTIONFS_UNBIND,
  FUNCTIONFS_ENABLE,
  FUNCTIONFS_DISABLE,
  FUNCTIONFS_SETUP,
  FUNCTIONFS_SUSPEND,
  FUNCTIONFS_RESUME
};
struct usb_functionfs_event {
  union {
    struct usb_ctrlrequest setup;
  } __attribute__((packed)) u;
  __u8 type;
  __u8 _pad[3];
} __attribute__((packed));
#define FUNCTIONFS_FIFO_STATUS _IO('g', 1)
#define FUNCTIONFS_FIFO_FLUSH _IO('g', 2)
#define FUNCTIONFS_CLEAR_HALT _IO('g', 3)
#define FUNCTIONFS_INTERFACE_REVMAP _IO('g', 128)
#define FUNCTIONFS_ENDPOINT_REVMAP _IO('g', 129)
#define FUNCTIONFS_ENDPOINT_DESC _IOR('g', 130, struct usb_endpoint_descriptor)
#define FUNCTIONFS_DMABUF_ATTACH _IOW('g', 131, int)
#define FUNCTIONFS_DMABUF_DETACH _IOW('g', 132, int)
#define FUNCTIONFS_DMABUF_TRANSFER _IOW('g', 133, struct usb_ffs_dmabuf_transfer_req)
#endif
