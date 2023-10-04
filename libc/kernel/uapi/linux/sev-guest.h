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
#ifndef __UAPI_LINUX_SEV_GUEST_H_
#define __UAPI_LINUX_SEV_GUEST_H_
#include <linux/types.h>
struct snp_report_req {
  __u8 user_data[64];
  __u32 vmpl;
  __u8 rsvd[28];
};
struct snp_report_resp {
  __u8 data[4000];
};
struct snp_derived_key_req {
  __u32 root_key_select;
  __u32 rsvd;
  __u64 guest_field_select;
  __u32 vmpl;
  __u32 guest_svn;
  __u64 tcb_version;
};
struct snp_derived_key_resp {
  __u8 data[64];
};
struct snp_guest_request_ioctl {
  __u8 msg_version;
  __u64 req_data;
  __u64 resp_data;
  union {
    __u64 exitinfo2;
    struct {
      __u32 fw_error;
      __u32 vmm_error;
    };
  };
};
struct snp_ext_report_req {
  struct snp_report_req data;
  __u64 certs_address;
  __u32 certs_len;
};
#define SNP_GUEST_REQ_IOC_TYPE 'S'
#define SNP_GET_REPORT _IOWR(SNP_GUEST_REQ_IOC_TYPE, 0x0, struct snp_guest_request_ioctl)
#define SNP_GET_DERIVED_KEY _IOWR(SNP_GUEST_REQ_IOC_TYPE, 0x1, struct snp_guest_request_ioctl)
#define SNP_GET_EXT_REPORT _IOWR(SNP_GUEST_REQ_IOC_TYPE, 0x2, struct snp_guest_request_ioctl)
#define SNP_GUEST_FW_ERR_MASK GENMASK_ULL(31, 0)
#define SNP_GUEST_VMM_ERR_SHIFT 32
#define SNP_GUEST_VMM_ERR(x) (((u64) x) << SNP_GUEST_VMM_ERR_SHIFT)
#define SNP_GUEST_VMM_ERR_INVALID_LEN 1
#define SNP_GUEST_VMM_ERR_BUSY 2
#endif
