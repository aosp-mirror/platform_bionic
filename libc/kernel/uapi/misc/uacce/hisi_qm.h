/*
 * This file is auto-generated. Modifications will be lost.
 *
 * See https://android.googlesource.com/platform/bionic/+/master/libc/kernel/
 * for more information.
 */
#ifndef _UAPI_HISI_QM_H
#define _UAPI_HISI_QM_H
#include <linux/types.h>
struct hisi_qp_ctx {
  __u16 id;
  __u16 qc_type;
};
struct hisi_qp_info {
  __u32 sqe_size;
  __u16 sq_depth;
  __u16 cq_depth;
  __u64 reserved;
};
#define HISI_QM_API_VER_BASE "hisi_qm_v1"
#define HISI_QM_API_VER2_BASE "hisi_qm_v2"
#define HISI_QM_API_VER3_BASE "hisi_qm_v3"
#define UACCE_CMD_QM_SET_QP_CTX _IOWR('H', 10, struct hisi_qp_ctx)
#define UACCE_CMD_QM_SET_QP_INFO _IOWR('H', 11, struct hisi_qp_info)
#endif
