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
#ifndef SUNRPC_SVC_H
#define SUNRPC_SVC_H
#include <linux/in.h>
#include <linux/sunrpc/types.h>
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#include <linux/sunrpc/xdr.h>
#include <linux/sunrpc/svcauth.h>
#include <linux/wait.h>
#include <linux/mm.h>
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct svc_serv {
 struct list_head sv_threads;
 struct list_head sv_sockets;
 struct svc_program * sv_program;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct svc_stat * sv_stats;
 spinlock_t sv_lock;
 unsigned int sv_nrthreads;
 unsigned int sv_bufsz;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 unsigned int sv_xdrsize;
 struct list_head sv_permsocks;
 struct list_head sv_tempsocks;
 int sv_tmpcnt;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 char * sv_name;
};
#define RPCSVC_MAXPAYLOAD (64*1024u)
#define RPCSVC_MAXPAGES ((RPCSVC_MAXPAYLOAD+PAGE_SIZE-1)/PAGE_SIZE + 2)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct svc_rqst {
 struct list_head rq_list;
 struct svc_sock * rq_sock;
 struct sockaddr_in rq_addr;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 int rq_addrlen;
 struct svc_serv * rq_server;
 struct svc_procedure * rq_procinfo;
 struct auth_ops * rq_authop;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct svc_cred rq_cred;
 struct sk_buff * rq_skbuff;
 struct svc_deferred_req*rq_deferred;
 struct xdr_buf rq_arg;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct xdr_buf rq_res;
 struct page * rq_argpages[RPCSVC_MAXPAGES];
 struct page * rq_respages[RPCSVC_MAXPAGES];
 int rq_restailpage;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 short rq_argused;
 short rq_arghi;
 short rq_resused;
 u32 rq_xid;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 u32 rq_prog;
 u32 rq_vers;
 u32 rq_proc;
 u32 rq_prot;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 unsigned short
 rq_secure : 1;
 __u32 rq_daddr;
 void * rq_argp;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 void * rq_resp;
 void * rq_auth_data;
 int rq_reserved;
 struct cache_req rq_chandle;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct auth_domain * rq_client;
 struct svc_cacherep * rq_cacherep;
 struct knfsd_fh * rq_reffh;
 int rq_sendfile_ok;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 wait_queue_head_t rq_wait;
};
struct svc_deferred_req {
 u32 prot;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct sockaddr_in addr;
 struct svc_sock *svsk;
 u32 daddr;
 struct cache_deferred_req handle;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 int argslen;
 u32 args[0];
};
struct svc_program {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct svc_program * pg_next;
 u32 pg_prog;
 unsigned int pg_lovers;
 unsigned int pg_hivers;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 unsigned int pg_nvers;
 struct svc_version ** pg_vers;
 char * pg_name;
 char * pg_class;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct svc_stat * pg_stats;
 int (*pg_authenticate)(struct svc_rqst *);
};
struct svc_version {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 u32 vs_vers;
 u32 vs_nproc;
 struct svc_procedure * vs_proc;
 u32 vs_xdrsize;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 int (*vs_dispatch)(struct svc_rqst *, u32 *);
};
typedef int (*svc_procfunc)(struct svc_rqst *, void *argp, void *resp);
struct svc_procedure {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 svc_procfunc pc_func;
 kxdrproc_t pc_decode;
 kxdrproc_t pc_encode;
 kxdrproc_t pc_release;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 unsigned int pc_argsize;
 unsigned int pc_ressize;
 unsigned int pc_count;
 unsigned int pc_cachetype;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 unsigned int pc_xdrressize;
};
typedef void (*svc_thread_fn)(struct svc_rqst *);
struct svc_serv * svc_create(struct svc_program *, unsigned int);
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#endif
