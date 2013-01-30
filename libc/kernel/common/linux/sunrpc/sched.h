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
#ifndef _LINUX_SUNRPC_SCHED_H_
#define _LINUX_SUNRPC_SCHED_H_
#include <linux/timer.h>
#include <linux/sunrpc/types.h>
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#include <linux/spinlock.h>
#include <linux/wait.h>
#include <linux/workqueue.h>
#include <linux/sunrpc/xdr.h>
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct rpc_procinfo;
struct rpc_message {
 struct rpc_procinfo * rpc_proc;
 void * rpc_argp;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 void * rpc_resp;
 struct rpc_cred * rpc_cred;
};
struct rpc_call_ops;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct rpc_wait_queue;
struct rpc_wait {
 struct list_head list;
 struct list_head links;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct rpc_wait_queue * rpc_waitq;
};
struct rpc_task {
#ifdef RPC_DEBUG
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 unsigned long tk_magic;
#endif
 atomic_t tk_count;
 struct list_head tk_task;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct rpc_clnt * tk_client;
 struct rpc_rqst * tk_rqstp;
 int tk_status;
 struct rpc_message tk_msg;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u8 tk_garb_retry;
 __u8 tk_cred_retry;
 unsigned long tk_cookie;
 void (*tk_timeout_fn)(struct rpc_task *);
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 void (*tk_callback)(struct rpc_task *);
 void (*tk_action)(struct rpc_task *);
 const struct rpc_call_ops *tk_ops;
 void * tk_calldata;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct timer_list tk_timer;
 unsigned long tk_timeout;
 unsigned short tk_flags;
 unsigned char tk_priority : 2;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 unsigned long tk_runstate;
 struct workqueue_struct *tk_workqueue;
 union {
 struct work_struct tk_work;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct rpc_wait tk_wait;
 } u;
 unsigned short tk_timeouts;
 size_t tk_bytes_sent;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 unsigned long tk_start;
 long tk_rtt;
#ifdef RPC_DEBUG
 unsigned short tk_pid;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#endif
};
#define tk_auth tk_client->cl_auth
#define tk_xprt tk_client->cl_xprt
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define task_for_each(task, pos, head)   list_for_each(pos, head)   if ((task=list_entry(pos, struct rpc_task, u.tk_wait.list)),1)
#define task_for_first(task, head)   if (!list_empty(head) &&   ((task=list_entry((head)->next, struct rpc_task, u.tk_wait.list)),1))
#define alltask_for_each(task, pos, head)   list_for_each(pos, head)   if ((task=list_entry(pos, struct rpc_task, tk_task)),1)
typedef void (*rpc_action)(struct rpc_task *);
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct rpc_call_ops {
 void (*rpc_call_prepare)(struct rpc_task *, void *);
 void (*rpc_call_done)(struct rpc_task *, void *);
 void (*rpc_release)(void *);
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
#define RPC_TASK_ASYNC 0x0001
#define RPC_TASK_SWAPPER 0x0002
#define RPC_TASK_CHILD 0x0008
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define RPC_CALL_MAJORSEEN 0x0020
#define RPC_TASK_ROOTCREDS 0x0040
#define RPC_TASK_DYNAMIC 0x0080
#define RPC_TASK_KILLED 0x0100
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define RPC_TASK_SOFT 0x0200
#define RPC_TASK_NOINTR 0x0400
#define RPC_IS_ASYNC(t) ((t)->tk_flags & RPC_TASK_ASYNC)
#define RPC_IS_CHILD(t) ((t)->tk_flags & RPC_TASK_CHILD)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define RPC_IS_SWAPPER(t) ((t)->tk_flags & RPC_TASK_SWAPPER)
#define RPC_DO_ROOTOVERRIDE(t) ((t)->tk_flags & RPC_TASK_ROOTCREDS)
#define RPC_ASSASSINATED(t) ((t)->tk_flags & RPC_TASK_KILLED)
#define RPC_DO_CALLBACK(t) ((t)->tk_callback != NULL)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define RPC_IS_SOFT(t) ((t)->tk_flags & RPC_TASK_SOFT)
#define RPC_TASK_UNINTERRUPTIBLE(t) ((t)->tk_flags & RPC_TASK_NOINTR)
#define RPC_TASK_RUNNING 0
#define RPC_TASK_QUEUED 1
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define RPC_TASK_WAKEUP 2
#define RPC_TASK_HAS_TIMER 3
#define RPC_TASK_ACTIVE 4
#define RPC_IS_RUNNING(t) (test_bit(RPC_TASK_RUNNING, &(t)->tk_runstate))
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define rpc_set_running(t) (set_bit(RPC_TASK_RUNNING, &(t)->tk_runstate))
#define rpc_test_and_set_running(t)   (test_and_set_bit(RPC_TASK_RUNNING, &(t)->tk_runstate))
#define rpc_clear_running(t)   do {   smp_mb__before_clear_bit();   clear_bit(RPC_TASK_RUNNING, &(t)->tk_runstate);   smp_mb__after_clear_bit();   } while (0)
#define RPC_IS_QUEUED(t) (test_bit(RPC_TASK_QUEUED, &(t)->tk_runstate))
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define rpc_set_queued(t) (set_bit(RPC_TASK_QUEUED, &(t)->tk_runstate))
#define rpc_clear_queued(t)   do {   smp_mb__before_clear_bit();   clear_bit(RPC_TASK_QUEUED, &(t)->tk_runstate);   smp_mb__after_clear_bit();   } while (0)
#define rpc_start_wakeup(t)   (test_and_set_bit(RPC_TASK_WAKEUP, &(t)->tk_runstate) == 0)
#define rpc_finish_wakeup(t)   do {   smp_mb__before_clear_bit();   clear_bit(RPC_TASK_WAKEUP, &(t)->tk_runstate);   smp_mb__after_clear_bit();   } while (0)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define RPC_IS_ACTIVATED(t) (test_bit(RPC_TASK_ACTIVE, &(t)->tk_runstate))
#define rpc_set_active(t) (set_bit(RPC_TASK_ACTIVE, &(t)->tk_runstate))
#define rpc_clear_active(t)   do {   smp_mb__before_clear_bit();   clear_bit(RPC_TASK_ACTIVE, &(t)->tk_runstate);   smp_mb__after_clear_bit();   } while(0)
#define RPC_PRIORITY_LOW 0
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define RPC_PRIORITY_NORMAL 1
#define RPC_PRIORITY_HIGH 2
#define RPC_NR_PRIORITY (RPC_PRIORITY_HIGH+1)
struct rpc_wait_queue {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 spinlock_t lock;
 struct list_head tasks[RPC_NR_PRIORITY];
 unsigned long cookie;
 unsigned char maxpriority;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 unsigned char priority;
 unsigned char count;
 unsigned char nr;
 unsigned short qlen;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#ifdef RPC_DEBUG
 const char * name;
#endif
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define RPC_BATCH_COUNT 16
#ifndef RPC_DEBUG
#define RPC_WAITQ_INIT(var,qname) {   .lock = SPIN_LOCK_UNLOCKED,   .tasks = {   [0] = LIST_HEAD_INIT(var.tasks[0]),   [1] = LIST_HEAD_INIT(var.tasks[1]),   [2] = LIST_HEAD_INIT(var.tasks[2]),   },   }
#else
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define RPC_WAITQ_INIT(var,qname) {   .lock = SPIN_LOCK_UNLOCKED,   .tasks = {   [0] = LIST_HEAD_INIT(var.tasks[0]),   [1] = LIST_HEAD_INIT(var.tasks[1]),   [2] = LIST_HEAD_INIT(var.tasks[2]),   },   .name = qname,   }
#endif
#define RPC_WAITQ(var,qname) struct rpc_wait_queue var = RPC_WAITQ_INIT(var,qname)
#define RPC_IS_PRIORITY(q) ((q)->maxpriority > 0)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct rpc_task *rpc_new_task(struct rpc_clnt *, int flags,
 const struct rpc_call_ops *ops, void *data);
struct rpc_task *rpc_run_task(struct rpc_clnt *clnt, int flags,
 const struct rpc_call_ops *ops, void *data);
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct rpc_task *rpc_new_child(struct rpc_clnt *, struct rpc_task *parent);
struct rpc_task *rpc_wake_up_next(struct rpc_wait_queue *);
#ifdef RPC_DEBUG
#endif
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#ifdef RPC_DEBUG
#endif
#endif
