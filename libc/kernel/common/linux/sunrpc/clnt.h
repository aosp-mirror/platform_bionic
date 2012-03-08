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
#ifndef _LINUX_SUNRPC_CLNT_H
#define _LINUX_SUNRPC_CLNT_H
#include <linux/sunrpc/msg_prot.h>
#include <linux/sunrpc/sched.h>
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#include <linux/sunrpc/xprt.h>
#include <linux/sunrpc/auth.h>
#include <linux/sunrpc/stats.h>
#include <linux/sunrpc/xdr.h>
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#include <linux/sunrpc/timer.h>
#include <asm/signal.h>
struct rpc_portmap {
 __u32 pm_prog;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 pm_vers;
 __u32 pm_prot;
 __u16 pm_port;
 unsigned char pm_binding : 1;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct rpc_wait_queue pm_bindwait;
};
struct rpc_inode;
struct rpc_clnt {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 atomic_t cl_count;
 atomic_t cl_users;
 struct rpc_xprt * cl_xprt;
 struct rpc_procinfo * cl_procinfo;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 u32 cl_maxproc;
 char * cl_server;
 char * cl_protname;
 struct rpc_auth * cl_auth;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct rpc_stat * cl_stats;
 struct rpc_iostats * cl_metrics;
 unsigned int cl_softrtry : 1,
 cl_intr : 1,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 cl_autobind : 1,
 cl_oneshot : 1,
 cl_dead : 1;
 struct rpc_rtt * cl_rtt;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct rpc_portmap * cl_pmap;
 int cl_nodelen;
 char cl_nodename[UNX_MAXNODENAME];
 char cl_pathname[30];
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct vfsmount * cl_vfsmnt;
 struct dentry * cl_dentry;
 struct rpc_clnt * cl_parent;
 struct rpc_rtt cl_rtt_default;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct rpc_portmap cl_pmap_default;
 char cl_inline_name[32];
};
#define cl_timeout cl_xprt->timeout
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define cl_prog cl_pmap->pm_prog
#define cl_vers cl_pmap->pm_vers
#define cl_port cl_pmap->pm_port
#define cl_prot cl_pmap->pm_prot
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define RPC_MAXVERSION 4
struct rpc_program {
 char * name;
 u32 number;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 unsigned int nrvers;
 struct rpc_version ** version;
 struct rpc_stat * stats;
 char * pipe_dir_name;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct rpc_version {
 u32 number;
 unsigned int nrprocs;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct rpc_procinfo * procs;
};
struct rpc_procinfo {
 u32 p_proc;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 kxdrproc_t p_encode;
 kxdrproc_t p_decode;
 unsigned int p_bufsiz;
 unsigned int p_count;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 unsigned int p_timer;
 u32 p_statidx;
 char * p_name;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define RPC_CONGESTED(clnt) (RPCXPRT_CONGESTED((clnt)->cl_xprt))
#define RPC_PEERADDR(clnt) (&(clnt)->cl_xprt->addr)
#endif
