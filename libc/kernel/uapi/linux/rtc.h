/*
 * This file is auto-generated. Modifications will be lost.
 *
 * See https://android.googlesource.com/platform/bionic/+/master/libc/kernel/
 * for more information.
 */
#ifndef _UAPI_LINUX_RTC_H_
#define _UAPI_LINUX_RTC_H_
#include <linux/const.h>
#include <linux/ioctl.h>
#include <linux/types.h>
struct rtc_time {
  int tm_sec;
  int tm_min;
  int tm_hour;
  int tm_mday;
  int tm_mon;
  int tm_year;
  int tm_wday;
  int tm_yday;
  int tm_isdst;
};
struct rtc_wkalrm {
  unsigned char enabled;
  unsigned char pending;
  struct rtc_time time;
};
struct rtc_pll_info {
  int pll_ctrl;
  int pll_value;
  int pll_max;
  int pll_min;
  int pll_posmult;
  int pll_negmult;
  long pll_clock;
};
struct rtc_param {
  __u64 param;
  union {
    __u64 uvalue;
    __s64 svalue;
    __u64 ptr;
  };
  __u32 index;
  __u32 __pad;
};
#define RTC_AIE_ON _IO('p', 0x01)
#define RTC_AIE_OFF _IO('p', 0x02)
#define RTC_UIE_ON _IO('p', 0x03)
#define RTC_UIE_OFF _IO('p', 0x04)
#define RTC_PIE_ON _IO('p', 0x05)
#define RTC_PIE_OFF _IO('p', 0x06)
#define RTC_WIE_ON _IO('p', 0x0f)
#define RTC_WIE_OFF _IO('p', 0x10)
#define RTC_ALM_SET _IOW('p', 0x07, struct rtc_time)
#define RTC_ALM_READ _IOR('p', 0x08, struct rtc_time)
#define RTC_RD_TIME _IOR('p', 0x09, struct rtc_time)
#define RTC_SET_TIME _IOW('p', 0x0a, struct rtc_time)
#define RTC_IRQP_READ _IOR('p', 0x0b, unsigned long)
#define RTC_IRQP_SET _IOW('p', 0x0c, unsigned long)
#define RTC_EPOCH_READ _IOR('p', 0x0d, unsigned long)
#define RTC_EPOCH_SET _IOW('p', 0x0e, unsigned long)
#define RTC_WKALM_SET _IOW('p', 0x0f, struct rtc_wkalrm)
#define RTC_WKALM_RD _IOR('p', 0x10, struct rtc_wkalrm)
#define RTC_PLL_GET _IOR('p', 0x11, struct rtc_pll_info)
#define RTC_PLL_SET _IOW('p', 0x12, struct rtc_pll_info)
#define RTC_PARAM_GET _IOW('p', 0x13, struct rtc_param)
#define RTC_PARAM_SET _IOW('p', 0x14, struct rtc_param)
#define RTC_VL_DATA_INVALID _BITUL(0)
#define RTC_VL_BACKUP_LOW _BITUL(1)
#define RTC_VL_BACKUP_EMPTY _BITUL(2)
#define RTC_VL_ACCURACY_LOW _BITUL(3)
#define RTC_VL_BACKUP_SWITCH _BITUL(4)
#define RTC_VL_READ _IOR('p', 0x13, unsigned int)
#define RTC_VL_CLR _IO('p', 0x14)
#define RTC_IRQF 0x80
#define RTC_PF 0x40
#define RTC_AF 0x20
#define RTC_UF 0x10
#define RTC_FEATURE_ALARM 0
#define RTC_FEATURE_ALARM_RES_MINUTE 1
#define RTC_FEATURE_NEED_WEEK_DAY 2
#define RTC_FEATURE_ALARM_RES_2S 3
#define RTC_FEATURE_UPDATE_INTERRUPT 4
#define RTC_FEATURE_CORRECTION 5
#define RTC_FEATURE_BACKUP_SWITCH_MODE 6
#define RTC_FEATURE_ALARM_WAKEUP_ONLY 7
#define RTC_FEATURE_CNT 8
#define RTC_PARAM_FEATURES 0
#define RTC_PARAM_CORRECTION 1
#define RTC_PARAM_BACKUP_SWITCH_MODE 2
#define RTC_BSM_DISABLED 0
#define RTC_BSM_DIRECT 1
#define RTC_BSM_LEVEL 2
#define RTC_BSM_STANDBY 3
#define RTC_MAX_FREQ 8192
#endif
