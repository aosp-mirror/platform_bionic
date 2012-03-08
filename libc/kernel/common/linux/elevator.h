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
#ifndef _LINUX_ELEVATOR_H
#define _LINUX_ELEVATOR_H
typedef int (elevator_merge_fn) (request_queue_t *, struct request **,
 struct bio *);
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
typedef void (elevator_merge_req_fn) (request_queue_t *, struct request *, struct request *);
typedef void (elevator_merged_fn) (request_queue_t *, struct request *);
typedef int (elevator_dispatch_fn) (request_queue_t *, int);
typedef void (elevator_add_req_fn) (request_queue_t *, struct request *);
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
typedef int (elevator_queue_empty_fn) (request_queue_t *);
typedef struct request *(elevator_request_list_fn) (request_queue_t *, struct request *);
typedef void (elevator_completed_req_fn) (request_queue_t *, struct request *);
typedef int (elevator_may_queue_fn) (request_queue_t *, int, struct bio *);
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
typedef int (elevator_set_req_fn) (request_queue_t *, struct request *, struct bio *, gfp_t);
typedef void (elevator_put_req_fn) (request_queue_t *, struct request *);
typedef void (elevator_activate_req_fn) (request_queue_t *, struct request *);
typedef void (elevator_deactivate_req_fn) (request_queue_t *, struct request *);
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
typedef void *(elevator_init_fn) (request_queue_t *, elevator_t *);
typedef void (elevator_exit_fn) (elevator_t *);
struct elevator_ops
{
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 elevator_merge_fn *elevator_merge_fn;
 elevator_merged_fn *elevator_merged_fn;
 elevator_merge_req_fn *elevator_merge_req_fn;
 elevator_dispatch_fn *elevator_dispatch_fn;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 elevator_add_req_fn *elevator_add_req_fn;
 elevator_activate_req_fn *elevator_activate_req_fn;
 elevator_deactivate_req_fn *elevator_deactivate_req_fn;
 elevator_queue_empty_fn *elevator_queue_empty_fn;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 elevator_completed_req_fn *elevator_completed_req_fn;
 elevator_request_list_fn *elevator_former_req_fn;
 elevator_request_list_fn *elevator_latter_req_fn;
 elevator_set_req_fn *elevator_set_req_fn;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 elevator_put_req_fn *elevator_put_req_fn;
 elevator_may_queue_fn *elevator_may_queue_fn;
 elevator_init_fn *elevator_init_fn;
 elevator_exit_fn *elevator_exit_fn;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 void (*trim)(struct io_context *);
};
#define ELV_NAME_MAX (16)
struct elv_fs_entry {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct attribute attr;
 ssize_t (*show)(elevator_t *, char *);
 ssize_t (*store)(elevator_t *, const char *, size_t);
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct elevator_type
{
 struct list_head list;
 struct elevator_ops ops;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct elevator_type *elevator_type;
 struct elv_fs_entry *elevator_attrs;
 char elevator_name[ELV_NAME_MAX];
 struct module *elevator_owner;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct elevator_queue
{
 struct elevator_ops *ops;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 void *elevator_data;
 struct kobject kobj;
 struct elevator_type *elevator_type;
 struct mutex sysfs_lock;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
#define ELEVATOR_NO_MERGE 0
#define ELEVATOR_FRONT_MERGE 1
#define ELEVATOR_BACK_MERGE 2
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define ELEVATOR_INSERT_FRONT 1
#define ELEVATOR_INSERT_BACK 2
#define ELEVATOR_INSERT_SORT 3
#define ELEVATOR_INSERT_REQUEUE 4
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
enum {
 ELV_MQUEUE_MAY,
 ELV_MQUEUE_NO,
 ELV_MQUEUE_MUST,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
#define rq_end_sector(rq) ((rq)->sector + (rq)->nr_sectors)
#endif
