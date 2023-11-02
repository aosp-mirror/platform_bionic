/*
 * This file is auto-generated. Modifications will be lost.
 *
 * See https://android.googlesource.com/platform/bionic/+/master/libc/kernel/
 * for more information.
 */
#ifndef CXGB4_ABI_USER_H
#define CXGB4_ABI_USER_H
#include <linux/types.h>
#define C4IW_UVERBS_ABI_VERSION 3
enum {
  C4IW_64B_CQE = (1 << 0)
};
struct c4iw_create_cq {
  __u32 flags;
  __u32 reserved;
};
struct c4iw_create_cq_resp {
  __aligned_u64 key;
  __aligned_u64 gts_key;
  __aligned_u64 memsize;
  __u32 cqid;
  __u32 size;
  __u32 qid_mask;
  __u32 flags;
};
enum {
  C4IW_QPF_ONCHIP = (1 << 0),
  C4IW_QPF_WRITE_W_IMM = (1 << 1)
};
struct c4iw_create_qp_resp {
  __aligned_u64 ma_sync_key;
  __aligned_u64 sq_key;
  __aligned_u64 rq_key;
  __aligned_u64 sq_db_gts_key;
  __aligned_u64 rq_db_gts_key;
  __aligned_u64 sq_memsize;
  __aligned_u64 rq_memsize;
  __u32 sqid;
  __u32 rqid;
  __u32 sq_size;
  __u32 rq_size;
  __u32 qid_mask;
  __u32 flags;
};
struct c4iw_create_srq_resp {
  __aligned_u64 srq_key;
  __aligned_u64 srq_db_gts_key;
  __aligned_u64 srq_memsize;
  __u32 srqid;
  __u32 srq_size;
  __u32 rqt_abs_idx;
  __u32 qid_mask;
  __u32 flags;
  __u32 reserved;
};
enum {
  T4_SRQ_LIMIT_SUPPORT = 1 << 0,
};
struct c4iw_alloc_ucontext_resp {
  __aligned_u64 status_page_key;
  __u32 status_page_size;
  __u32 reserved;
};
struct c4iw_alloc_pd_resp {
  __u32 pdid;
};
#endif
