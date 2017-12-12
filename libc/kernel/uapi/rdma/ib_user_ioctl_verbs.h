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
#ifndef IB_USER_IOCTL_VERBS_H
#define IB_USER_IOCTL_VERBS_H
#include <rdma/rdma_user_ioctl.h>
#define UVERBS_UDATA_DRIVER_DATA_NS 1
#define UVERBS_UDATA_DRIVER_DATA_FLAG (1UL << UVERBS_ID_NS_SHIFT)
enum uverbs_default_objects {
  UVERBS_OBJECT_DEVICE,
  UVERBS_OBJECT_PD,
  UVERBS_OBJECT_COMP_CHANNEL,
  UVERBS_OBJECT_CQ,
  UVERBS_OBJECT_QP,
  UVERBS_OBJECT_SRQ,
  UVERBS_OBJECT_AH,
  UVERBS_OBJECT_MR,
  UVERBS_OBJECT_MW,
  UVERBS_OBJECT_FLOW,
  UVERBS_OBJECT_XRCD,
  UVERBS_OBJECT_RWQ_IND_TBL,
  UVERBS_OBJECT_WQ,
  UVERBS_OBJECT_LAST,
};
enum {
  UVERBS_UHW_IN = UVERBS_UDATA_DRIVER_DATA_FLAG,
  UVERBS_UHW_OUT,
};
enum uverbs_create_cq_cmd_attr_ids {
  CREATE_CQ_HANDLE,
  CREATE_CQ_CQE,
  CREATE_CQ_USER_HANDLE,
  CREATE_CQ_COMP_CHANNEL,
  CREATE_CQ_COMP_VECTOR,
  CREATE_CQ_FLAGS,
  CREATE_CQ_RESP_CQE,
};
enum uverbs_destroy_cq_cmd_attr_ids {
  DESTROY_CQ_HANDLE,
  DESTROY_CQ_RESP,
};
enum uverbs_actions_cq_ops {
  UVERBS_CQ_CREATE,
  UVERBS_CQ_DESTROY,
};
#endif
