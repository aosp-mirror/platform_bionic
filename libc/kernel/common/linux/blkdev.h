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
#ifndef _LINUX_BLKDEV_H
#define _LINUX_BLKDEV_H
#include <linux/major.h>
#include <linux/genhd.h>
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#include <linux/list.h>
#include <linux/timer.h>
#include <linux/workqueue.h>
#include <linux/pagemap.h>
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#include <linux/backing-dev.h>
#include <linux/wait.h>
#include <linux/mempool.h>
#include <linux/bio.h>
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#include <linux/module.h>
#include <linux/stringify.h>
#include <asm/scatterlist.h>
struct scsi_ioctl_command;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct request_queue;
typedef struct request_queue request_queue_t;
struct elevator_queue;
typedef struct elevator_queue elevator_t;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct request_pm_state;
struct blk_trace;
#define BLKDEV_MIN_RQ 4
#define BLKDEV_MAX_RQ 128
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct as_io_context {
 spinlock_t lock;
 void (*dtor)(struct as_io_context *aic);
 void (*exit)(struct as_io_context *aic);
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 unsigned long state;
 atomic_t nr_queued;
 atomic_t nr_dispatched;
 unsigned long last_end_request;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 unsigned long ttime_total;
 unsigned long ttime_samples;
 unsigned long ttime_mean;
 unsigned int seek_samples;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 sector_t last_request_pos;
 u64 seek_total;
 sector_t seek_mean;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct cfq_queue;
struct cfq_io_context {
 struct rb_node rb_node;
 void *key;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct cfq_queue *cfqq[2];
 struct io_context *ioc;
 unsigned long last_end_request;
 sector_t last_request_pos;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 unsigned long last_queue;
 unsigned long ttime_total;
 unsigned long ttime_samples;
 unsigned long ttime_mean;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 unsigned int seek_samples;
 u64 seek_total;
 sector_t seek_mean;
 struct list_head queue_list;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 void (*dtor)(struct io_context *);
 void (*exit)(struct io_context *);
};
struct io_context {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 atomic_t refcount;
 struct task_struct *task;
 int (*set_ioprio)(struct io_context *, unsigned int);
 unsigned long last_waited;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 int nr_batch_requests;
 struct as_io_context *aic;
 struct rb_root cic_root;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct io_context *current_io_context(gfp_t gfp_flags);
struct io_context *get_io_context(gfp_t gfp_flags);
struct request;
typedef void (rq_end_io_fn)(struct request *, int);
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct request_list {
 int count[2];
 int starved[2];
 int elvpriv;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 mempool_t *rq_pool;
 wait_queue_head_t wait[2];
};
#define BLK_MAX_CDB 16
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct request {
 struct list_head queuelist;
 struct list_head donelist;
 unsigned long flags;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 sector_t sector;
 unsigned long nr_sectors;
 unsigned int current_nr_sectors;
 sector_t hard_sector;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 unsigned long hard_nr_sectors;
 unsigned int hard_cur_sectors;
 struct bio *bio;
 struct bio *biotail;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 void *elevator_private;
 void *completion_data;
 int rq_status;
 int errors;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct gendisk *rq_disk;
 unsigned long start_time;
 unsigned short nr_phys_segments;
 unsigned short nr_hw_segments;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 unsigned short ioprio;
 int tag;
 int ref_count;
 request_queue_t *q;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct request_list *rl;
 struct completion *waiting;
 void *special;
 char *buffer;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 unsigned int cmd_len;
 unsigned char cmd[BLK_MAX_CDB];
 unsigned int data_len;
 unsigned int sense_len;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 void *data;
 void *sense;
 unsigned int timeout;
 int retries;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 rq_end_io_fn *end_io;
 void *end_io_data;
};
enum rq_flag_bits {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __REQ_RW,
 __REQ_FAILFAST,
 __REQ_SORTED,
 __REQ_SOFTBARRIER,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __REQ_HARDBARRIER,
 __REQ_FUA,
 __REQ_CMD,
 __REQ_NOMERGE,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __REQ_STARTED,
 __REQ_DONTPREP,
 __REQ_QUEUED,
 __REQ_ELVPRIV,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __REQ_PC,
 __REQ_BLOCK_PC,
 __REQ_SENSE,
 __REQ_FAILED,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __REQ_QUIET,
 __REQ_SPECIAL,
 __REQ_DRIVE_CMD,
 __REQ_DRIVE_TASK,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __REQ_DRIVE_TASKFILE,
 __REQ_PREEMPT,
 __REQ_PM_SUSPEND,
 __REQ_PM_RESUME,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __REQ_PM_SHUTDOWN,
 __REQ_ORDERED_COLOR,
 __REQ_RW_SYNC,
 __REQ_NR_BITS,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
#define REQ_RW (1 << __REQ_RW)
#define REQ_FAILFAST (1 << __REQ_FAILFAST)
#define REQ_SORTED (1 << __REQ_SORTED)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define REQ_SOFTBARRIER (1 << __REQ_SOFTBARRIER)
#define REQ_HARDBARRIER (1 << __REQ_HARDBARRIER)
#define REQ_FUA (1 << __REQ_FUA)
#define REQ_CMD (1 << __REQ_CMD)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define REQ_NOMERGE (1 << __REQ_NOMERGE)
#define REQ_STARTED (1 << __REQ_STARTED)
#define REQ_DONTPREP (1 << __REQ_DONTPREP)
#define REQ_QUEUED (1 << __REQ_QUEUED)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define REQ_ELVPRIV (1 << __REQ_ELVPRIV)
#define REQ_PC (1 << __REQ_PC)
#define REQ_BLOCK_PC (1 << __REQ_BLOCK_PC)
#define REQ_SENSE (1 << __REQ_SENSE)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define REQ_FAILED (1 << __REQ_FAILED)
#define REQ_QUIET (1 << __REQ_QUIET)
#define REQ_SPECIAL (1 << __REQ_SPECIAL)
#define REQ_DRIVE_CMD (1 << __REQ_DRIVE_CMD)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define REQ_DRIVE_TASK (1 << __REQ_DRIVE_TASK)
#define REQ_DRIVE_TASKFILE (1 << __REQ_DRIVE_TASKFILE)
#define REQ_PREEMPT (1 << __REQ_PREEMPT)
#define REQ_PM_SUSPEND (1 << __REQ_PM_SUSPEND)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define REQ_PM_RESUME (1 << __REQ_PM_RESUME)
#define REQ_PM_SHUTDOWN (1 << __REQ_PM_SHUTDOWN)
#define REQ_ORDERED_COLOR (1 << __REQ_ORDERED_COLOR)
#define REQ_RW_SYNC (1 << __REQ_RW_SYNC)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct request_pm_state
{
 int pm_step;
 u32 pm_state;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 void* data;
};
#include <linux/elevator.h>
typedef int (merge_request_fn) (request_queue_t *, struct request *,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct bio *);
typedef int (merge_requests_fn) (request_queue_t *, struct request *,
 struct request *);
typedef void (request_fn_proc) (request_queue_t *q);
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
typedef int (make_request_fn) (request_queue_t *q, struct bio *bio);
typedef int (prep_rq_fn) (request_queue_t *, struct request *);
typedef void (unplug_fn) (request_queue_t *);
struct bio_vec;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
typedef int (merge_bvec_fn) (request_queue_t *, struct bio *, struct bio_vec *);
typedef void (activity_fn) (void *data, int rw);
typedef int (issue_flush_fn) (request_queue_t *, struct gendisk *, sector_t *);
typedef void (prepare_flush_fn) (request_queue_t *, struct request *);
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
typedef void (softirq_done_fn)(struct request *);
enum blk_queue_state {
 Queue_down,
 Queue_up,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct blk_queue_tag {
 struct request **tag_index;
 unsigned long *tag_map;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct list_head busy_list;
 int busy;
 int max_depth;
 int real_max_depth;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 atomic_t refcnt;
};
struct request_queue
{
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct list_head queue_head;
 struct request *last_merge;
 elevator_t *elevator;
 struct request_list rq;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 request_fn_proc *request_fn;
 merge_request_fn *back_merge_fn;
 merge_request_fn *front_merge_fn;
 merge_requests_fn *merge_requests_fn;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 make_request_fn *make_request_fn;
 prep_rq_fn *prep_rq_fn;
 unplug_fn *unplug_fn;
 merge_bvec_fn *merge_bvec_fn;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 activity_fn *activity_fn;
 issue_flush_fn *issue_flush_fn;
 prepare_flush_fn *prepare_flush_fn;
 softirq_done_fn *softirq_done_fn;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 sector_t end_sector;
 struct request *boundary_rq;
 struct timer_list unplug_timer;
 int unplug_thresh;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 unsigned long unplug_delay;
 struct work_struct unplug_work;
 struct backing_dev_info backing_dev_info;
 void *queuedata;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 void *activity_data;
 unsigned long bounce_pfn;
 gfp_t bounce_gfp;
 unsigned long queue_flags;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 spinlock_t __queue_lock;
 spinlock_t *queue_lock;
 struct kobject kobj;
 unsigned long nr_requests;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 unsigned int nr_congestion_on;
 unsigned int nr_congestion_off;
 unsigned int nr_batching;
 unsigned int max_sectors;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 unsigned int max_hw_sectors;
 unsigned short max_phys_segments;
 unsigned short max_hw_segments;
 unsigned short hardsect_size;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 unsigned int max_segment_size;
 unsigned long seg_boundary_mask;
 unsigned int dma_alignment;
 struct blk_queue_tag *queue_tags;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 unsigned int nr_sorted;
 unsigned int in_flight;
 unsigned int sg_timeout;
 unsigned int sg_reserved_size;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 int node;
 struct blk_trace *blk_trace;
 unsigned int ordered, next_ordered, ordseq;
 int orderr, ordcolor;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct request pre_flush_rq, bar_rq, post_flush_rq;
 struct request *orig_bar_rq;
 unsigned int bi_size;
 struct mutex sysfs_lock;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
#define RQ_INACTIVE (-1)
#define RQ_ACTIVE 1
#define QUEUE_FLAG_CLUSTER 0
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define QUEUE_FLAG_QUEUED 1
#define QUEUE_FLAG_STOPPED 2
#define QUEUE_FLAG_READFULL 3
#define QUEUE_FLAG_WRITEFULL 4
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define QUEUE_FLAG_DEAD 5
#define QUEUE_FLAG_REENTER 6
#define QUEUE_FLAG_PLUGGED 7
#define QUEUE_FLAG_ELVSWITCH 8
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
enum {
 QUEUE_ORDERED_NONE = 0x00,
 QUEUE_ORDERED_DRAIN = 0x01,
 QUEUE_ORDERED_TAG = 0x02,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 QUEUE_ORDERED_PREFLUSH = 0x10,
 QUEUE_ORDERED_POSTFLUSH = 0x20,
 QUEUE_ORDERED_FUA = 0x40,
 QUEUE_ORDERED_DRAIN_FLUSH = QUEUE_ORDERED_DRAIN |
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 QUEUE_ORDERED_PREFLUSH | QUEUE_ORDERED_POSTFLUSH,
 QUEUE_ORDERED_DRAIN_FUA = QUEUE_ORDERED_DRAIN |
 QUEUE_ORDERED_PREFLUSH | QUEUE_ORDERED_FUA,
 QUEUE_ORDERED_TAG_FLUSH = QUEUE_ORDERED_TAG |
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 QUEUE_ORDERED_PREFLUSH | QUEUE_ORDERED_POSTFLUSH,
 QUEUE_ORDERED_TAG_FUA = QUEUE_ORDERED_TAG |
 QUEUE_ORDERED_PREFLUSH | QUEUE_ORDERED_FUA,
 QUEUE_ORDSEQ_STARTED = 0x01,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 QUEUE_ORDSEQ_DRAIN = 0x02,
 QUEUE_ORDSEQ_PREFLUSH = 0x04,
 QUEUE_ORDSEQ_BAR = 0x08,
 QUEUE_ORDSEQ_POSTFLUSH = 0x10,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 QUEUE_ORDSEQ_DONE = 0x20,
};
#define blk_queue_plugged(q) test_bit(QUEUE_FLAG_PLUGGED, &(q)->queue_flags)
#define blk_queue_tagged(q) test_bit(QUEUE_FLAG_QUEUED, &(q)->queue_flags)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define blk_queue_stopped(q) test_bit(QUEUE_FLAG_STOPPED, &(q)->queue_flags)
#define blk_queue_flushing(q) ((q)->ordseq)
#define blk_fs_request(rq) ((rq)->flags & REQ_CMD)
#define blk_pc_request(rq) ((rq)->flags & REQ_BLOCK_PC)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define blk_noretry_request(rq) ((rq)->flags & REQ_FAILFAST)
#define blk_rq_started(rq) ((rq)->flags & REQ_STARTED)
#define blk_account_rq(rq) (blk_rq_started(rq) && blk_fs_request(rq))
#define blk_pm_suspend_request(rq) ((rq)->flags & REQ_PM_SUSPEND)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define blk_pm_resume_request(rq) ((rq)->flags & REQ_PM_RESUME)
#define blk_pm_request(rq)   ((rq)->flags & (REQ_PM_SUSPEND | REQ_PM_RESUME))
#define blk_sorted_rq(rq) ((rq)->flags & REQ_SORTED)
#define blk_barrier_rq(rq) ((rq)->flags & REQ_HARDBARRIER)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define blk_fua_rq(rq) ((rq)->flags & REQ_FUA)
#define list_entry_rq(ptr) list_entry((ptr), struct request, queuelist)
#define rq_data_dir(rq) ((rq)->flags & 1)
#define RQ_NOMERGE_FLAGS   (REQ_NOMERGE | REQ_STARTED | REQ_HARDBARRIER | REQ_SOFTBARRIER)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define rq_mergeable(rq)   (!((rq)->flags & RQ_NOMERGE_FLAGS) && blk_fs_request((rq)))
#define blk_queue_headactive(q, head_active)
#define BLKPREP_OK 0
#define BLKPREP_KILL 1
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define BLKPREP_DEFER 2
#define BLK_BOUNCE_HIGH ((u64)blk_max_low_pfn << PAGE_SHIFT)
#define BLK_BOUNCE_ANY ((u64)blk_max_pfn << PAGE_SHIFT)
#define BLK_BOUNCE_ISA (ISA_DMA_THRESHOLD)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define rq_for_each_bio(_bio, rq)   if ((rq->bio))   for (_bio = (rq)->bio; _bio; _bio = _bio->bi_next)
struct sec_size {
 unsigned block_size;
 unsigned block_size_bits;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
#define end_io_error(uptodate) (unlikely((uptodate) <= 0))
#define blk_queue_tag_depth(q) ((q)->queue_tags->busy)
#define blk_queue_tag_queue(q) ((q)->queue_tags->busy < (q)->queue_tags->max_depth)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define blk_rq_tagged(rq) ((rq)->flags & REQ_QUEUED)
#define MAX_PHYS_SEGMENTS 128
#define MAX_HW_SEGMENTS 128
#define SAFE_MAX_SECTORS 255
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define BLK_DEF_MAX_SECTORS 1024
#define MAX_SEGMENT_SIZE 65536
#define blkdev_entry_to_request(entry) list_entry((entry), struct request, queuelist)
#define blk_finished_io(nsects) do { } while (0)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define blk_started_io(nsects) do { } while (0)
typedef struct {struct page *v;} Sector;
struct work_struct;
#define sector_div(n, b)(  {   int _res;   _res = (n) % (b);   (n) /= (b);   _res;  }  )
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define MODULE_ALIAS_BLOCKDEV(major,minor)   MODULE_ALIAS("block-major-" __stringify(major) "-" __stringify(minor))
#define MODULE_ALIAS_BLOCKDEV_MAJOR(major)   MODULE_ALIAS("block-major-" __stringify(major) "-*")
#endif
