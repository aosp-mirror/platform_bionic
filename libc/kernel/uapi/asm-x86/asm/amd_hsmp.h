/*
 * This file is auto-generated. Modifications will be lost.
 *
 * See https://android.googlesource.com/platform/bionic/+/master/libc/kernel/
 * for more information.
 */
#ifndef _UAPI_ASM_X86_AMD_HSMP_H_
#define _UAPI_ASM_X86_AMD_HSMP_H_
#include <linux/types.h>
#pragma pack(4)
#define HSMP_MAX_MSG_LEN 8
enum hsmp_message_ids {
  HSMP_TEST = 1,
  HSMP_GET_SMU_VER,
  HSMP_GET_PROTO_VER,
  HSMP_GET_SOCKET_POWER,
  HSMP_SET_SOCKET_POWER_LIMIT,
  HSMP_GET_SOCKET_POWER_LIMIT,
  HSMP_GET_SOCKET_POWER_LIMIT_MAX,
  HSMP_SET_BOOST_LIMIT,
  HSMP_SET_BOOST_LIMIT_SOCKET,
  HSMP_GET_BOOST_LIMIT,
  HSMP_GET_PROC_HOT,
  HSMP_SET_XGMI_LINK_WIDTH,
  HSMP_SET_DF_PSTATE,
  HSMP_SET_AUTO_DF_PSTATE,
  HSMP_GET_FCLK_MCLK,
  HSMP_GET_CCLK_THROTTLE_LIMIT,
  HSMP_GET_C0_PERCENT,
  HSMP_SET_NBIO_DPM_LEVEL,
  HSMP_GET_NBIO_DPM_LEVEL,
  HSMP_GET_DDR_BANDWIDTH,
  HSMP_GET_TEMP_MONITOR,
  HSMP_GET_DIMM_TEMP_RANGE,
  HSMP_GET_DIMM_POWER,
  HSMP_GET_DIMM_THERMAL,
  HSMP_GET_SOCKET_FREQ_LIMIT,
  HSMP_GET_CCLK_CORE_LIMIT,
  HSMP_GET_RAILS_SVI,
  HSMP_GET_SOCKET_FMAX_FMIN,
  HSMP_GET_IOLINK_BANDWITH,
  HSMP_GET_XGMI_BANDWITH,
  HSMP_SET_GMI3_WIDTH,
  HSMP_SET_PCI_RATE,
  HSMP_SET_POWER_MODE,
  HSMP_SET_PSTATE_MAX_MIN,
  HSMP_MSG_ID_MAX,
};
struct hsmp_message {
  __u32 msg_id;
  __u16 num_args;
  __u16 response_sz;
  __u32 args[HSMP_MAX_MSG_LEN];
  __u16 sock_ind;
};
enum hsmp_msg_type {
  HSMP_RSVD = - 1,
  HSMP_SET = 0,
  HSMP_GET = 1,
};
struct hsmp_msg_desc {
  int num_args;
  int response_sz;
  enum hsmp_msg_type type;
};
static const struct hsmp_msg_desc hsmp_msg_desc_table[] = {
 {
    0, 0, HSMP_RSVD
  }
 , {
    1, 1, HSMP_GET
  }
 , {
    0, 1, HSMP_GET
  }
 , {
    0, 1, HSMP_GET
  }
 , {
    0, 1, HSMP_GET
  }
 , {
    1, 0, HSMP_SET
  }
 , {
    0, 1, HSMP_GET
  }
 , {
    0, 1, HSMP_GET
  }
 , {
    1, 0, HSMP_SET
  }
 , {
    1, 0, HSMP_SET
  }
 , {
    1, 1, HSMP_GET
  }
 , {
    0, 1, HSMP_GET
  }
 , {
    1, 0, HSMP_SET
  }
 , {
    1, 0, HSMP_SET
  }
 , {
    0, 0, HSMP_SET
  }
 , {
    0, 2, HSMP_GET
  }
 , {
    0, 1, HSMP_GET
  }
 , {
    0, 1, HSMP_GET
  }
 , {
    1, 0, HSMP_SET
  }
 , {
    1, 1, HSMP_GET
  }
 , {
    0, 1, HSMP_GET
  }
 , {
    0, 1, HSMP_GET
  }
 , {
    1, 1, HSMP_GET
  }
 , {
    1, 1, HSMP_GET
  }
 , {
    1, 1, HSMP_GET
  }
 , {
    0, 1, HSMP_GET
  }
 , {
    1, 1, HSMP_GET
  }
 , {
    0, 1, HSMP_GET
  }
 , {
    0, 1, HSMP_GET
  }
 , {
    1, 1, HSMP_GET
  }
 , {
    1, 1, HSMP_GET
  }
 , {
    1, 0, HSMP_SET
  }
 , {
    1, 1, HSMP_SET
  }
 , {
    1, 0, HSMP_SET
  }
 , {
    1, 0, HSMP_SET
  }
 ,
};
#pragma pack()
#define HSMP_BASE_IOCTL_NR 0xF8
#define HSMP_IOCTL_CMD _IOWR(HSMP_BASE_IOCTL_NR, 0, struct hsmp_message)
#endif
