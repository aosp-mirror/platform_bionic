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
#ifndef _MD_K_H
#define _MD_K_H
#include "../../../drivers/md/dm-bio-list.h"
#define LEVEL_MULTIPATH (-4)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define LEVEL_LINEAR (-1)
#define LEVEL_FAULTY (-5)
#define LEVEL_NONE (-1000000)
#define MaxSector (~(sector_t)0)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define MD_THREAD_NAME_MAX 14
typedef struct mddev_s mddev_t;
typedef struct mdk_rdev_s mdk_rdev_t;
#define MAX_MD_DEVS 256
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define MAX_CHUNK_SIZE (1<<30)
struct mdk_rdev_s
{
 struct list_head same_set;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 sector_t size;
 mddev_t *mddev;
 unsigned long last_events;
 struct block_device *bdev;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct page *sb_page;
 int sb_loaded;
 __u64 sb_events;
 sector_t data_offset;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 sector_t sb_offset;
 int sb_size;
 int preferred_minor;
 struct kobject kobj;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 unsigned long flags;
#define Faulty 1
#define In_sync 2
#define WriteMostly 4
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define BarriersNotsupp 5
 int desc_nr;
 int raid_disk;
 int saved_raid_disk;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 sector_t recovery_offset;
 atomic_t nr_pending;
 atomic_t read_errors;
 atomic_t corrected_errors;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct mddev_s
{
 void *private;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct mdk_personality *pers;
 dev_t unit;
 int md_minor;
 struct list_head disks;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 int sb_dirty;
 int ro;
 struct gendisk *gendisk;
 struct kobject kobj;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 int major_version,
 minor_version,
 patch_version;
 int persistent;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 int chunk_size;
 time_t ctime, utime;
 int level, layout;
 char clevel[16];
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 int raid_disks;
 int max_disks;
 sector_t size;
 sector_t array_size;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u64 events;
 char uuid[16];
 sector_t reshape_position;
 int delta_disks, new_level, new_layout, new_chunk;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct mdk_thread_s *thread;
 struct mdk_thread_s *sync_thread;
 sector_t curr_resync;
 unsigned long resync_mark;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 sector_t resync_mark_cnt;
 sector_t curr_mark_cnt;
 sector_t resync_max_sectors;
 sector_t resync_mismatches;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 sector_t suspend_lo;
 sector_t suspend_hi;
 int sync_speed_min;
 int sync_speed_max;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 int ok_start_degraded;
#define MD_RECOVERY_RUNNING 0
#define MD_RECOVERY_SYNC 1
#define MD_RECOVERY_ERR 2
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define MD_RECOVERY_INTR 3
#define MD_RECOVERY_DONE 4
#define MD_RECOVERY_NEEDED 5
#define MD_RECOVERY_REQUESTED 6
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define MD_RECOVERY_CHECK 7
#define MD_RECOVERY_RESHAPE 8
#define MD_RECOVERY_FROZEN 9
 unsigned long recovery;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 int in_sync;
 struct mutex reconfig_mutex;
 atomic_t active;
 int changed;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 int degraded;
 int barriers_work;
 struct bio *biolist;
 atomic_t recovery_active;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 wait_queue_head_t recovery_wait;
 sector_t recovery_cp;
 spinlock_t write_lock;
 wait_queue_head_t sb_wait;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 atomic_t pending_writes;
 unsigned int safemode;
 unsigned int safemode_delay;
 struct timer_list safemode_timer;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 atomic_t writes_pending;
 request_queue_t *queue;
 atomic_t write_behind;
 unsigned int max_write_behind;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct bitmap *bitmap;
 struct file *bitmap_file;
 long bitmap_offset;
 long default_bitmap_offset;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct list_head all_mddevs;
};
struct mdk_personality
{
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 char *name;
 int level;
 struct list_head list;
 struct module *owner;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 int (*make_request)(request_queue_t *q, struct bio *bio);
 int (*run)(mddev_t *mddev);
 int (*stop)(mddev_t *mddev);
 void (*status)(struct seq_file *seq, mddev_t *mddev);
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 void (*error_handler)(mddev_t *mddev, mdk_rdev_t *rdev);
 int (*hot_add_disk) (mddev_t *mddev, mdk_rdev_t *rdev);
 int (*hot_remove_disk) (mddev_t *mddev, int number);
 int (*spare_active) (mddev_t *mddev);
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 sector_t (*sync_request)(mddev_t *mddev, sector_t sector_nr, int *skipped, int go_faster);
 int (*resize) (mddev_t *mddev, sector_t sectors);
 int (*check_reshape) (mddev_t *mddev);
 int (*start_reshape) (mddev_t *mddev);
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 int (*reconfig) (mddev_t *mddev, int layout, int chunk_size);
 void (*quiesce) (mddev_t *mddev, int state);
};
struct md_sysfs_entry {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct attribute attr;
 ssize_t (*show)(mddev_t *, char *);
 ssize_t (*store)(mddev_t *, const char *, size_t);
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define ITERATE_RDEV_GENERIC(head,rdev,tmp)     for ((tmp) = (head).next;   (rdev) = (list_entry((tmp), mdk_rdev_t, same_set)),   (tmp) = (tmp)->next, (tmp)->prev != &(head)   ; )
#define ITERATE_RDEV(mddev,rdev,tmp)   ITERATE_RDEV_GENERIC((mddev)->disks,rdev,tmp)
#define ITERATE_RDEV_PENDING(rdev,tmp)   ITERATE_RDEV_GENERIC(pending_raid_disks,rdev,tmp)
typedef struct mdk_thread_s {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 void (*run) (mddev_t *mddev);
 mddev_t *mddev;
 wait_queue_head_t wqueue;
 unsigned long flags;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct task_struct *tsk;
 unsigned long timeout;
} mdk_thread_t;
#define THREAD_WAKEUP 0
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define __wait_event_lock_irq(wq, condition, lock, cmd)  do {   wait_queue_t __wait;   init_waitqueue_entry(&__wait, current);     add_wait_queue(&wq, &__wait);   for (;;) {   set_current_state(TASK_UNINTERRUPTIBLE);   if (condition)   break;   spin_unlock_irq(&lock);   cmd;   schedule();   spin_lock_irq(&lock);   }   current->state = TASK_RUNNING;   remove_wait_queue(&wq, &__wait);  } while (0)
#define wait_event_lock_irq(wq, condition, lock, cmd)  do {   if (condition)   break;   __wait_event_lock_irq(wq, condition, lock, cmd);  } while (0)
#endif
