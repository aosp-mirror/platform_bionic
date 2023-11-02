/*
 * This file is auto-generated. Modifications will be lost.
 *
 * See https://android.googlesource.com/platform/bionic/+/master/libc/kernel/
 * for more information.
 */
#ifndef _UAPI_LINUX_SURFACE_AGGREGATOR_DTX_H
#define _UAPI_LINUX_SURFACE_AGGREGATOR_DTX_H
#include <linux/ioctl.h>
#include <linux/types.h>
#define SDTX_CATEGORY_STATUS 0x0000
#define SDTX_CATEGORY_RUNTIME_ERROR 0x1000
#define SDTX_CATEGORY_HARDWARE_ERROR 0x2000
#define SDTX_CATEGORY_UNKNOWN 0xf000
#define SDTX_CATEGORY_MASK 0xf000
#define SDTX_CATEGORY(value) ((value) & SDTX_CATEGORY_MASK)
#define SDTX_STATUS(code) ((code) | SDTX_CATEGORY_STATUS)
#define SDTX_ERR_RT(code) ((code) | SDTX_CATEGORY_RUNTIME_ERROR)
#define SDTX_ERR_HW(code) ((code) | SDTX_CATEGORY_HARDWARE_ERROR)
#define SDTX_UNKNOWN(code) ((code) | SDTX_CATEGORY_UNKNOWN)
#define SDTX_SUCCESS(value) (SDTX_CATEGORY(value) == SDTX_CATEGORY_STATUS)
#define SDTX_LATCH_CLOSED SDTX_STATUS(0x00)
#define SDTX_LATCH_OPENED SDTX_STATUS(0x01)
#define SDTX_BASE_DETACHED SDTX_STATUS(0x00)
#define SDTX_BASE_ATTACHED SDTX_STATUS(0x01)
#define SDTX_DETACH_NOT_FEASIBLE SDTX_ERR_RT(0x01)
#define SDTX_DETACH_TIMEDOUT SDTX_ERR_RT(0x02)
#define SDTX_ERR_FAILED_TO_OPEN SDTX_ERR_HW(0x01)
#define SDTX_ERR_FAILED_TO_REMAIN_OPEN SDTX_ERR_HW(0x02)
#define SDTX_ERR_FAILED_TO_CLOSE SDTX_ERR_HW(0x03)
#define SDTX_DEVICE_TYPE_HID 0x0100
#define SDTX_DEVICE_TYPE_SSH 0x0200
#define SDTX_DEVICE_TYPE_MASK 0x0f00
#define SDTX_DEVICE_TYPE(value) ((value) & SDTX_DEVICE_TYPE_MASK)
#define SDTX_BASE_TYPE_HID(id) ((id) | SDTX_DEVICE_TYPE_HID)
#define SDTX_BASE_TYPE_SSH(id) ((id) | SDTX_DEVICE_TYPE_SSH)
enum sdtx_device_mode {
  SDTX_DEVICE_MODE_TABLET = 0x00,
  SDTX_DEVICE_MODE_LAPTOP = 0x01,
  SDTX_DEVICE_MODE_STUDIO = 0x02,
};
struct sdtx_event {
  __u16 length;
  __u16 code;
  __u8 data[];
} __attribute__((__packed__));
enum sdtx_event_code {
  SDTX_EVENT_REQUEST = 1,
  SDTX_EVENT_CANCEL = 2,
  SDTX_EVENT_BASE_CONNECTION = 3,
  SDTX_EVENT_LATCH_STATUS = 4,
  SDTX_EVENT_DEVICE_MODE = 5,
};
struct sdtx_base_info {
  __u16 state;
  __u16 base_id;
} __attribute__((__packed__));
#define SDTX_IOCTL_EVENTS_ENABLE _IO(0xa5, 0x21)
#define SDTX_IOCTL_EVENTS_DISABLE _IO(0xa5, 0x22)
#define SDTX_IOCTL_LATCH_LOCK _IO(0xa5, 0x23)
#define SDTX_IOCTL_LATCH_UNLOCK _IO(0xa5, 0x24)
#define SDTX_IOCTL_LATCH_REQUEST _IO(0xa5, 0x25)
#define SDTX_IOCTL_LATCH_CONFIRM _IO(0xa5, 0x26)
#define SDTX_IOCTL_LATCH_HEARTBEAT _IO(0xa5, 0x27)
#define SDTX_IOCTL_LATCH_CANCEL _IO(0xa5, 0x28)
#define SDTX_IOCTL_GET_BASE_INFO _IOR(0xa5, 0x29, struct sdtx_base_info)
#define SDTX_IOCTL_GET_DEVICE_MODE _IOR(0xa5, 0x2a, __u16)
#define SDTX_IOCTL_GET_LATCH_STATUS _IOR(0xa5, 0x2b, __u16)
#endif
