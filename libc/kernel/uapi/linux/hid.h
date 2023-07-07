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
#ifndef _UAPI__HID_H
#define _UAPI__HID_H
#define USB_INTERFACE_CLASS_HID 3
#define USB_INTERFACE_SUBCLASS_BOOT 1
#define USB_INTERFACE_PROTOCOL_KEYBOARD 1
#define USB_INTERFACE_PROTOCOL_MOUSE 2
enum hid_report_type {
  HID_INPUT_REPORT = 0,
  HID_OUTPUT_REPORT = 1,
  HID_FEATURE_REPORT = 2,
  HID_REPORT_TYPES,
};
enum hid_class_request {
  HID_REQ_GET_REPORT = 0x01,
  HID_REQ_GET_IDLE = 0x02,
  HID_REQ_GET_PROTOCOL = 0x03,
  HID_REQ_SET_REPORT = 0x09,
  HID_REQ_SET_IDLE = 0x0A,
  HID_REQ_SET_PROTOCOL = 0x0B,
};
#define HID_DT_HID (USB_TYPE_CLASS | 0x01)
#define HID_DT_REPORT (USB_TYPE_CLASS | 0x02)
#define HID_DT_PHYSICAL (USB_TYPE_CLASS | 0x03)
#define HID_MAX_DESCRIPTOR_SIZE 4096
#endif
