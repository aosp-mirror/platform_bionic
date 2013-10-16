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
#ifndef IB_USER_VERBS_H
#define IB_USER_VERBS_H
#include <linux/types.h>
#define IB_USER_VERBS_ABI_VERSION 6
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
enum {
 IB_USER_VERBS_CMD_GET_CONTEXT,
 IB_USER_VERBS_CMD_QUERY_DEVICE,
 IB_USER_VERBS_CMD_QUERY_PORT,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 IB_USER_VERBS_CMD_ALLOC_PD,
 IB_USER_VERBS_CMD_DEALLOC_PD,
 IB_USER_VERBS_CMD_CREATE_AH,
 IB_USER_VERBS_CMD_MODIFY_AH,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 IB_USER_VERBS_CMD_QUERY_AH,
 IB_USER_VERBS_CMD_DESTROY_AH,
 IB_USER_VERBS_CMD_REG_MR,
 IB_USER_VERBS_CMD_REG_SMR,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 IB_USER_VERBS_CMD_REREG_MR,
 IB_USER_VERBS_CMD_QUERY_MR,
 IB_USER_VERBS_CMD_DEREG_MR,
 IB_USER_VERBS_CMD_ALLOC_MW,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 IB_USER_VERBS_CMD_BIND_MW,
 IB_USER_VERBS_CMD_DEALLOC_MW,
 IB_USER_VERBS_CMD_CREATE_COMP_CHANNEL,
 IB_USER_VERBS_CMD_CREATE_CQ,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 IB_USER_VERBS_CMD_RESIZE_CQ,
 IB_USER_VERBS_CMD_DESTROY_CQ,
 IB_USER_VERBS_CMD_POLL_CQ,
 IB_USER_VERBS_CMD_PEEK_CQ,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 IB_USER_VERBS_CMD_REQ_NOTIFY_CQ,
 IB_USER_VERBS_CMD_CREATE_QP,
 IB_USER_VERBS_CMD_QUERY_QP,
 IB_USER_VERBS_CMD_MODIFY_QP,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 IB_USER_VERBS_CMD_DESTROY_QP,
 IB_USER_VERBS_CMD_POST_SEND,
 IB_USER_VERBS_CMD_POST_RECV,
 IB_USER_VERBS_CMD_ATTACH_MCAST,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 IB_USER_VERBS_CMD_DETACH_MCAST,
 IB_USER_VERBS_CMD_CREATE_SRQ,
 IB_USER_VERBS_CMD_MODIFY_SRQ,
 IB_USER_VERBS_CMD_QUERY_SRQ,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 IB_USER_VERBS_CMD_DESTROY_SRQ,
 IB_USER_VERBS_CMD_POST_SRQ_RECV,
 IB_USER_VERBS_CMD_OPEN_XRCD,
 IB_USER_VERBS_CMD_CLOSE_XRCD,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 IB_USER_VERBS_CMD_CREATE_XSRQ,
 IB_USER_VERBS_CMD_OPEN_QP
};
struct ib_uverbs_async_event_desc {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u64 element;
 __u32 event_type;
 __u32 reserved;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct ib_uverbs_comp_event_desc {
 __u64 cq_handle;
};
struct ib_uverbs_cmd_hdr {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 command;
 __u16 in_words;
 __u16 out_words;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct ib_uverbs_get_context {
 __u64 response;
 __u64 driver_data[0];
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct ib_uverbs_get_context_resp {
 __u32 async_fd;
 __u32 num_comp_vectors;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct ib_uverbs_query_device {
 __u64 response;
 __u64 driver_data[0];
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct ib_uverbs_query_device_resp {
 __u64 fw_ver;
 __be64 node_guid;
 __be64 sys_image_guid;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u64 max_mr_size;
 __u64 page_size_cap;
 __u32 vendor_id;
 __u32 vendor_part_id;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 hw_ver;
 __u32 max_qp;
 __u32 max_qp_wr;
 __u32 device_cap_flags;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 max_sge;
 __u32 max_sge_rd;
 __u32 max_cq;
 __u32 max_cqe;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 max_mr;
 __u32 max_pd;
 __u32 max_qp_rd_atom;
 __u32 max_ee_rd_atom;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 max_res_rd_atom;
 __u32 max_qp_init_rd_atom;
 __u32 max_ee_init_rd_atom;
 __u32 atomic_cap;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 max_ee;
 __u32 max_rdd;
 __u32 max_mw;
 __u32 max_raw_ipv6_qp;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 max_raw_ethy_qp;
 __u32 max_mcast_grp;
 __u32 max_mcast_qp_attach;
 __u32 max_total_mcast_qp_attach;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 max_ah;
 __u32 max_fmr;
 __u32 max_map_per_fmr;
 __u32 max_srq;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 max_srq_wr;
 __u32 max_srq_sge;
 __u16 max_pkeys;
 __u8 local_ca_ack_delay;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u8 phys_port_cnt;
 __u8 reserved[4];
};
struct ib_uverbs_query_port {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u64 response;
 __u8 port_num;
 __u8 reserved[7];
 __u64 driver_data[0];
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct ib_uverbs_query_port_resp {
 __u32 port_cap_flags;
 __u32 max_msg_sz;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 bad_pkey_cntr;
 __u32 qkey_viol_cntr;
 __u32 gid_tbl_len;
 __u16 pkey_tbl_len;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u16 lid;
 __u16 sm_lid;
 __u8 state;
 __u8 max_mtu;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u8 active_mtu;
 __u8 lmc;
 __u8 max_vl_num;
 __u8 sm_sl;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u8 subnet_timeout;
 __u8 init_type_reply;
 __u8 active_width;
 __u8 active_speed;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u8 phys_state;
 __u8 link_layer;
 __u8 reserved[2];
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct ib_uverbs_alloc_pd {
 __u64 response;
 __u64 driver_data[0];
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct ib_uverbs_alloc_pd_resp {
 __u32 pd_handle;
};
struct ib_uverbs_dealloc_pd {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 pd_handle;
};
struct ib_uverbs_open_xrcd {
 __u64 response;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 fd;
 __u32 oflags;
 __u64 driver_data[0];
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct ib_uverbs_open_xrcd_resp {
 __u32 xrcd_handle;
};
struct ib_uverbs_close_xrcd {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 xrcd_handle;
};
struct ib_uverbs_reg_mr {
 __u64 response;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u64 start;
 __u64 length;
 __u64 hca_va;
 __u32 pd_handle;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 access_flags;
 __u64 driver_data[0];
};
struct ib_uverbs_reg_mr_resp {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 mr_handle;
 __u32 lkey;
 __u32 rkey;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct ib_uverbs_dereg_mr {
 __u32 mr_handle;
};
struct ib_uverbs_alloc_mw {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u64 response;
 __u32 pd_handle;
 __u8 mw_type;
 __u8 reserved[3];
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct ib_uverbs_alloc_mw_resp {
 __u32 mw_handle;
 __u32 rkey;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct ib_uverbs_dealloc_mw {
 __u32 mw_handle;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct ib_uverbs_create_comp_channel {
 __u64 response;
};
struct ib_uverbs_create_comp_channel_resp {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 fd;
};
struct ib_uverbs_create_cq {
 __u64 response;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u64 user_handle;
 __u32 cqe;
 __u32 comp_vector;
 __s32 comp_channel;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 reserved;
 __u64 driver_data[0];
};
struct ib_uverbs_create_cq_resp {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 cq_handle;
 __u32 cqe;
};
struct ib_uverbs_resize_cq {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u64 response;
 __u32 cq_handle;
 __u32 cqe;
 __u64 driver_data[0];
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct ib_uverbs_resize_cq_resp {
 __u32 cqe;
 __u32 reserved;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u64 driver_data[0];
};
struct ib_uverbs_poll_cq {
 __u64 response;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 cq_handle;
 __u32 ne;
};
struct ib_uverbs_wc {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u64 wr_id;
 __u32 status;
 __u32 opcode;
 __u32 vendor_err;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 byte_len;
 union {
 __u32 imm_data;
 __u32 invalidate_rkey;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 } ex;
 __u32 qp_num;
 __u32 src_qp;
 __u32 wc_flags;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u16 pkey_index;
 __u16 slid;
 __u8 sl;
 __u8 dlid_path_bits;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u8 port_num;
 __u8 reserved;
};
struct ib_uverbs_poll_cq_resp {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 count;
 __u32 reserved;
 struct ib_uverbs_wc wc[0];
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct ib_uverbs_req_notify_cq {
 __u32 cq_handle;
 __u32 solicited_only;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct ib_uverbs_destroy_cq {
 __u64 response;
 __u32 cq_handle;
 __u32 reserved;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct ib_uverbs_destroy_cq_resp {
 __u32 comp_events_reported;
 __u32 async_events_reported;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct ib_uverbs_global_route {
 __u8 dgid[16];
 __u32 flow_label;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u8 sgid_index;
 __u8 hop_limit;
 __u8 traffic_class;
 __u8 reserved;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct ib_uverbs_ah_attr {
 struct ib_uverbs_global_route grh;
 __u16 dlid;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u8 sl;
 __u8 src_path_bits;
 __u8 static_rate;
 __u8 is_global;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u8 port_num;
 __u8 reserved;
};
struct ib_uverbs_qp_attr {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 qp_attr_mask;
 __u32 qp_state;
 __u32 cur_qp_state;
 __u32 path_mtu;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 path_mig_state;
 __u32 qkey;
 __u32 rq_psn;
 __u32 sq_psn;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 dest_qp_num;
 __u32 qp_access_flags;
 struct ib_uverbs_ah_attr ah_attr;
 struct ib_uverbs_ah_attr alt_ah_attr;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 max_send_wr;
 __u32 max_recv_wr;
 __u32 max_send_sge;
 __u32 max_recv_sge;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 max_inline_data;
 __u16 pkey_index;
 __u16 alt_pkey_index;
 __u8 en_sqd_async_notify;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u8 sq_draining;
 __u8 max_rd_atomic;
 __u8 max_dest_rd_atomic;
 __u8 min_rnr_timer;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u8 port_num;
 __u8 timeout;
 __u8 retry_cnt;
 __u8 rnr_retry;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u8 alt_port_num;
 __u8 alt_timeout;
 __u8 reserved[5];
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct ib_uverbs_create_qp {
 __u64 response;
 __u64 user_handle;
 __u32 pd_handle;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 send_cq_handle;
 __u32 recv_cq_handle;
 __u32 srq_handle;
 __u32 max_send_wr;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 max_recv_wr;
 __u32 max_send_sge;
 __u32 max_recv_sge;
 __u32 max_inline_data;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u8 sq_sig_all;
 __u8 qp_type;
 __u8 is_srq;
 __u8 reserved;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u64 driver_data[0];
};
struct ib_uverbs_open_qp {
 __u64 response;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u64 user_handle;
 __u32 pd_handle;
 __u32 qpn;
 __u8 qp_type;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u8 reserved[7];
 __u64 driver_data[0];
};
struct ib_uverbs_create_qp_resp {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 qp_handle;
 __u32 qpn;
 __u32 max_send_wr;
 __u32 max_recv_wr;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 max_send_sge;
 __u32 max_recv_sge;
 __u32 max_inline_data;
 __u32 reserved;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct ib_uverbs_qp_dest {
 __u8 dgid[16];
 __u32 flow_label;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u16 dlid;
 __u16 reserved;
 __u8 sgid_index;
 __u8 hop_limit;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u8 traffic_class;
 __u8 sl;
 __u8 src_path_bits;
 __u8 static_rate;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u8 is_global;
 __u8 port_num;
};
struct ib_uverbs_query_qp {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u64 response;
 __u32 qp_handle;
 __u32 attr_mask;
 __u64 driver_data[0];
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct ib_uverbs_query_qp_resp {
 struct ib_uverbs_qp_dest dest;
 struct ib_uverbs_qp_dest alt_dest;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 max_send_wr;
 __u32 max_recv_wr;
 __u32 max_send_sge;
 __u32 max_recv_sge;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 max_inline_data;
 __u32 qkey;
 __u32 rq_psn;
 __u32 sq_psn;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 dest_qp_num;
 __u32 qp_access_flags;
 __u16 pkey_index;
 __u16 alt_pkey_index;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u8 qp_state;
 __u8 cur_qp_state;
 __u8 path_mtu;
 __u8 path_mig_state;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u8 sq_draining;
 __u8 max_rd_atomic;
 __u8 max_dest_rd_atomic;
 __u8 min_rnr_timer;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u8 port_num;
 __u8 timeout;
 __u8 retry_cnt;
 __u8 rnr_retry;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u8 alt_port_num;
 __u8 alt_timeout;
 __u8 sq_sig_all;
 __u8 reserved[5];
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u64 driver_data[0];
};
struct ib_uverbs_modify_qp {
 struct ib_uverbs_qp_dest dest;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct ib_uverbs_qp_dest alt_dest;
 __u32 qp_handle;
 __u32 attr_mask;
 __u32 qkey;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 rq_psn;
 __u32 sq_psn;
 __u32 dest_qp_num;
 __u32 qp_access_flags;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u16 pkey_index;
 __u16 alt_pkey_index;
 __u8 qp_state;
 __u8 cur_qp_state;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u8 path_mtu;
 __u8 path_mig_state;
 __u8 en_sqd_async_notify;
 __u8 max_rd_atomic;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u8 max_dest_rd_atomic;
 __u8 min_rnr_timer;
 __u8 port_num;
 __u8 timeout;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u8 retry_cnt;
 __u8 rnr_retry;
 __u8 alt_port_num;
 __u8 alt_timeout;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u8 reserved[2];
 __u64 driver_data[0];
};
struct ib_uverbs_modify_qp_resp {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct ib_uverbs_destroy_qp {
 __u64 response;
 __u32 qp_handle;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 reserved;
};
struct ib_uverbs_destroy_qp_resp {
 __u32 events_reported;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct ib_uverbs_sge {
 __u64 addr;
 __u32 length;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 lkey;
};
struct ib_uverbs_send_wr {
 __u64 wr_id;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 num_sge;
 __u32 opcode;
 __u32 send_flags;
 union {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 imm_data;
 __u32 invalidate_rkey;
 } ex;
 union {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct {
 __u64 remote_addr;
 __u32 rkey;
 __u32 reserved;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 } rdma;
 struct {
 __u64 remote_addr;
 __u64 compare_add;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u64 swap;
 __u32 rkey;
 __u32 reserved;
 } atomic;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct {
 __u32 ah;
 __u32 remote_qpn;
 __u32 remote_qkey;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 reserved;
 } ud;
 } wr;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct ib_uverbs_post_send {
 __u64 response;
 __u32 qp_handle;
 __u32 wr_count;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 sge_count;
 __u32 wqe_size;
 struct ib_uverbs_send_wr send_wr[0];
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct ib_uverbs_post_send_resp {
 __u32 bad_wr;
};
struct ib_uverbs_recv_wr {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u64 wr_id;
 __u32 num_sge;
 __u32 reserved;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct ib_uverbs_post_recv {
 __u64 response;
 __u32 qp_handle;
 __u32 wr_count;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 sge_count;
 __u32 wqe_size;
 struct ib_uverbs_recv_wr recv_wr[0];
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct ib_uverbs_post_recv_resp {
 __u32 bad_wr;
};
struct ib_uverbs_post_srq_recv {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u64 response;
 __u32 srq_handle;
 __u32 wr_count;
 __u32 sge_count;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 wqe_size;
 struct ib_uverbs_recv_wr recv[0];
};
struct ib_uverbs_post_srq_recv_resp {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 bad_wr;
};
struct ib_uverbs_create_ah {
 __u64 response;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u64 user_handle;
 __u32 pd_handle;
 __u32 reserved;
 struct ib_uverbs_ah_attr attr;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct ib_uverbs_create_ah_resp {
 __u32 ah_handle;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct ib_uverbs_destroy_ah {
 __u32 ah_handle;
};
struct ib_uverbs_attach_mcast {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u8 gid[16];
 __u32 qp_handle;
 __u16 mlid;
 __u16 reserved;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u64 driver_data[0];
};
struct ib_uverbs_detach_mcast {
 __u8 gid[16];
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 qp_handle;
 __u16 mlid;
 __u16 reserved;
 __u64 driver_data[0];
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct ib_uverbs_create_srq {
 __u64 response;
 __u64 user_handle;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 pd_handle;
 __u32 max_wr;
 __u32 max_sge;
 __u32 srq_limit;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u64 driver_data[0];
};
struct ib_uverbs_create_xsrq {
 __u64 response;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u64 user_handle;
 __u32 srq_type;
 __u32 pd_handle;
 __u32 max_wr;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 max_sge;
 __u32 srq_limit;
 __u32 reserved;
 __u32 xrcd_handle;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 cq_handle;
 __u64 driver_data[0];
};
struct ib_uverbs_create_srq_resp {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 srq_handle;
 __u32 max_wr;
 __u32 max_sge;
 __u32 srqn;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct ib_uverbs_modify_srq {
 __u32 srq_handle;
 __u32 attr_mask;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 max_wr;
 __u32 srq_limit;
 __u64 driver_data[0];
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct ib_uverbs_query_srq {
 __u64 response;
 __u32 srq_handle;
 __u32 reserved;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u64 driver_data[0];
};
struct ib_uverbs_query_srq_resp {
 __u32 max_wr;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 max_sge;
 __u32 srq_limit;
 __u32 reserved;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct ib_uverbs_destroy_srq {
 __u64 response;
 __u32 srq_handle;
 __u32 reserved;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct ib_uverbs_destroy_srq_resp {
 __u32 events_reported;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#endif
