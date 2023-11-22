/*
 * This file is auto-generated. Modifications will be lost.
 *
 * See https://android.googlesource.com/platform/bionic/+/master/libc/kernel/
 * for more information.
 */
#ifndef _UAPI__LINUX_USB_CHARGER_H
#define _UAPI__LINUX_USB_CHARGER_H
enum usb_charger_type {
  UNKNOWN_TYPE = 0,
  SDP_TYPE = 1,
  DCP_TYPE = 2,
  CDP_TYPE = 3,
  ACA_TYPE = 4,
};
enum usb_charger_state {
  USB_CHARGER_DEFAULT = 0,
  USB_CHARGER_PRESENT = 1,
  USB_CHARGER_ABSENT = 2,
};
#endif
