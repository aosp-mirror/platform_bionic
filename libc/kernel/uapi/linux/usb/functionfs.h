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
#ifndef _UAPI__LINUX_FUNCTIONFS_H__
#define _UAPI__LINUX_FUNCTIONFS_H__
#include <linux/types.h>
#include <linux/ioctl.h>
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#include <linux/usb/ch9.h>
enum {
 FUNCTIONFS_DESCRIPTORS_MAGIC = 1,
 FUNCTIONFS_STRINGS_MAGIC = 2,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 FUNCTIONFS_DESCRIPTORS_MAGIC_V2 = 3,
};
enum functionfs_flags {
 FUNCTIONFS_HAS_FS_DESC = 1,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 FUNCTIONFS_HAS_HS_DESC = 2,
 FUNCTIONFS_HAS_SS_DESC = 4,
};
struct usb_endpoint_descriptor_no_audio {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u8 bLength;
 __u8 bDescriptorType;
 __u8 bEndpointAddress;
 __u8 bmAttributes;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __le16 wMaxPacketSize;
 __u8 bInterval;
} __attribute__((packed));
struct usb_functionfs_descs_head {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __le32 magic;
 __le32 length;
 __le32 fs_count;
 __le32 hs_count;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
} __attribute__((packed, deprecated));
struct usb_functionfs_strings_head {
 __le32 magic;
 __le32 length;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __le32 str_count;
 __le32 lang_count;
} __attribute__((packed));
enum usb_functionfs_event_type {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 FUNCTIONFS_BIND,
 FUNCTIONFS_UNBIND,
 FUNCTIONFS_ENABLE,
 FUNCTIONFS_DISABLE,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 FUNCTIONFS_SETUP,
 FUNCTIONFS_SUSPEND,
 FUNCTIONFS_RESUME
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct usb_functionfs_event {
 union {
 struct usb_ctrlrequest setup;
 } __attribute__((packed)) u;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u8 type;
 __u8 _pad[3];
} __attribute__((packed));
#define FUNCTIONFS_FIFO_STATUS _IO('g', 1)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define FUNCTIONFS_FIFO_FLUSH _IO('g', 2)
#define FUNCTIONFS_CLEAR_HALT _IO('g', 3)
#define FUNCTIONFS_INTERFACE_REVMAP _IO('g', 128)
#define FUNCTIONFS_ENDPOINT_REVMAP _IO('g', 129)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#endif
