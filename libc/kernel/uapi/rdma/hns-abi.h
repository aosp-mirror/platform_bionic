/*
 * This file is auto-generated. Modifications will be lost.
 *
 * See https://android.googlesource.com/platform/bionic/+/master/libc/kernel/
 * for more information.
 */
#ifndef HNS_ABI_USER_H
#define HNS_ABI_USER_H
#include <linux/types.h>
struct hns_roce_ib_create_cq {
  __aligned_u64 buf_addr;
  __aligned_u64 db_addr;
  __u32 cqe_size;
  __u32 reserved;
};
enum hns_roce_cq_cap_flags {
  HNS_ROCE_CQ_FLAG_RECORD_DB = 1 << 0,
};
struct hns_roce_ib_create_cq_resp {
  __aligned_u64 cqn;
  __aligned_u64 cap_flags;
};
enum hns_roce_srq_cap_flags {
  HNS_ROCE_SRQ_CAP_RECORD_DB = 1 << 0,
};
enum hns_roce_srq_cap_flags_resp {
  HNS_ROCE_RSP_SRQ_CAP_RECORD_DB = 1 << 0,
};
struct hns_roce_ib_create_srq {
  __aligned_u64 buf_addr;
  __aligned_u64 db_addr;
  __aligned_u64 que_addr;
  __u32 req_cap_flags;
  __u32 reserved;
};
struct hns_roce_ib_create_srq_resp {
  __u32 srqn;
  __u32 cap_flags;
};
struct hns_roce_ib_create_qp {
  __aligned_u64 buf_addr;
  __aligned_u64 db_addr;
  __u8 log_sq_bb_count;
  __u8 log_sq_stride;
  __u8 sq_no_prefetch;
  __u8 reserved[5];
  __aligned_u64 sdb_addr;
};
enum hns_roce_qp_cap_flags {
  HNS_ROCE_QP_CAP_RQ_RECORD_DB = 1 << 0,
  HNS_ROCE_QP_CAP_SQ_RECORD_DB = 1 << 1,
  HNS_ROCE_QP_CAP_OWNER_DB = 1 << 2,
  HNS_ROCE_QP_CAP_DIRECT_WQE = 1 << 5,
};
struct hns_roce_ib_create_qp_resp {
  __aligned_u64 cap_flags;
  __aligned_u64 dwqe_mmap_key;
};
enum {
  HNS_ROCE_EXSGE_FLAGS = 1 << 0,
  HNS_ROCE_RQ_INLINE_FLAGS = 1 << 1,
  HNS_ROCE_CQE_INLINE_FLAGS = 1 << 2,
};
enum {
  HNS_ROCE_RSP_EXSGE_FLAGS = 1 << 0,
  HNS_ROCE_RSP_RQ_INLINE_FLAGS = 1 << 1,
  HNS_ROCE_RSP_CQE_INLINE_FLAGS = 1 << 2,
};
struct hns_roce_ib_alloc_ucontext_resp {
  __u32 qp_tab_size;
  __u32 cqe_size;
  __u32 srq_tab_size;
  __u32 reserved;
  __u32 config;
  __u32 max_inline_data;
};
struct hns_roce_ib_alloc_ucontext {
  __u32 config;
  __u32 reserved;
};
struct hns_roce_ib_alloc_pd_resp {
  __u32 pdn;
};
struct hns_roce_ib_create_ah_resp {
  __u8 dmac[6];
  __u8 reserved[2];
};
#endif
