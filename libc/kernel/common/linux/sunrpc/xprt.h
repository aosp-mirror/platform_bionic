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
#ifndef _LINUX_SUNRPC_XPRT_H
#define _LINUX_SUNRPC_XPRT_H
#include <linux/uio.h>
#include <linux/socket.h>
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#include <linux/in.h>
#include <linux/sunrpc/sched.h>
#include <linux/sunrpc/xdr.h>
#define RPC_MIN_SLOT_TABLE (2U)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define RPC_DEF_SLOT_TABLE (16U)
#define RPC_MAX_SLOT_TABLE (128U)
#define RPC_CALLHDRSIZE 6
#define RPC_REPHDRSIZE 4
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define RPC_MIN_RESVPORT (1U)
#define RPC_MAX_RESVPORT (65535U)
#define RPC_DEF_MIN_RESVPORT (665U)
#define RPC_DEF_MAX_RESVPORT (1023U)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct rpc_timeout {
 unsigned long to_initval,
 to_maxval,
 to_increment;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 unsigned int to_retries;
 unsigned char to_exponential;
};
struct rpc_task;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct rpc_xprt;
struct seq_file;
struct rpc_rqst {
 struct rpc_xprt * rq_xprt;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct xdr_buf rq_snd_buf;
 struct xdr_buf rq_rcv_buf;
 struct rpc_task * rq_task;
 __u32 rq_xid;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 int rq_cong;
 int rq_received;
 u32 rq_seqno;
 int rq_enc_pages_num;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct page **rq_enc_pages;
 void (*rq_release_snd_buf)(struct rpc_rqst *);
 struct list_head rq_list;
 __u32 * rq_buffer;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 size_t rq_bufsize;
 struct xdr_buf rq_private_buf;
 unsigned long rq_majortimeo;
 unsigned long rq_timeout;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 unsigned int rq_retries;
 u32 rq_bytes_sent;
 unsigned long rq_xtime;
 int rq_ntrans;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
#define rq_svec rq_snd_buf.head
#define rq_slen rq_snd_buf.len
struct rpc_xprt_ops {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 void (*set_buffer_size)(struct rpc_xprt *xprt, size_t sndsize, size_t rcvsize);
 int (*reserve_xprt)(struct rpc_task *task);
 void (*release_xprt)(struct rpc_xprt *xprt, struct rpc_task *task);
 void (*set_port)(struct rpc_xprt *xprt, unsigned short port);
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 void (*connect)(struct rpc_task *task);
 void * (*buf_alloc)(struct rpc_task *task, size_t size);
 void (*buf_free)(struct rpc_task *task);
 int (*send_request)(struct rpc_task *task);
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 void (*set_retrans_timeout)(struct rpc_task *task);
 void (*timer)(struct rpc_task *task);
 void (*release_request)(struct rpc_task *task);
 void (*close)(struct rpc_xprt *xprt);
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 void (*destroy)(struct rpc_xprt *xprt);
 void (*print_stats)(struct rpc_xprt *xprt, struct seq_file *seq);
};
struct rpc_xprt {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct rpc_xprt_ops * ops;
 struct socket * sock;
 struct sock * inet;
 struct rpc_timeout timeout;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct sockaddr_in addr;
 int prot;
 unsigned long cong;
 unsigned long cwnd;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 size_t rcvsize,
 sndsize;
 size_t max_payload;
 unsigned int tsh_size;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct rpc_wait_queue sending;
 struct rpc_wait_queue resend;
 struct rpc_wait_queue pending;
 struct rpc_wait_queue backlog;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct list_head free;
 struct rpc_rqst * slot;
 unsigned int max_reqs;
 unsigned long state;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 unsigned char shutdown : 1,
 resvport : 1;
 __u32 xid;
 u32 tcp_recm,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 tcp_xid,
 tcp_reclen,
 tcp_offset;
 unsigned long tcp_copied,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 tcp_flags;
 unsigned long connect_timeout,
 bind_timeout,
 reestablish_timeout;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct work_struct connect_worker;
 unsigned short port;
 struct work_struct task_cleanup;
 struct timer_list timer;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 unsigned long last_used,
 idle_timeout;
 spinlock_t transport_lock;
 spinlock_t reserve_lock;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct rpc_task * snd_task;
 struct list_head recv;
 struct {
 unsigned long bind_count,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 connect_count,
 connect_start,
 connect_time,
 sends,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 recvs,
 bad_xids;
 unsigned long long req_u,
 bklog_u;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 } stat;
 void (*old_data_ready)(struct sock *, int);
 void (*old_state_change)(struct sock *);
 void (*old_write_space)(struct sock *);
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
#define XPRT_LAST_FRAG (1 << 0)
#define XPRT_COPY_RECM (1 << 1)
#define XPRT_COPY_XID (1 << 2)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define XPRT_COPY_DATA (1 << 3)
#endif
