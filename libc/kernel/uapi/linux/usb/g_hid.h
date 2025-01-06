/*
 * This file is auto-generated. Modifications will be lost.
 *
 * See https://android.googlesource.com/platform/bionic/+/master/libc/kernel/
 * for more information.
 */
#ifndef __UAPI_LINUX_USB_G_HID_H
#define __UAPI_LINUX_USB_G_HID_H
#include <linux/types.h>
#define MAX_REPORT_LENGTH 64
struct usb_hidg_report {
  __u8 report_id;
  __u8 userspace_req;
  __u16 length;
  __u8 data[MAX_REPORT_LENGTH];
  __u8 padding[4];
};
#define GADGET_HID_READ_GET_REPORT_ID _IOR('g', 0x41, __u8)
#define GADGET_HID_WRITE_GET_REPORT _IOW('g', 0x42, struct usb_hidg_report)
#endif
