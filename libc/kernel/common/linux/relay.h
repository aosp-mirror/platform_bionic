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
#ifndef _LINUX_RELAY_H
#define _LINUX_RELAY_H
#include <linux/types.h>
#include <linux/sched.h>
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#include <linux/wait.h>
#include <linux/list.h>
#include <linux/fs.h>
#include <linux/poll.h>
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#include <linux/kref.h>
#define FIX_SIZE(x) ((((x) - 1) & PAGE_MASK) + PAGE_SIZE)
#define RELAYFS_CHANNEL_VERSION 6
struct rchan_buf
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
{
 void *start;
 void *data;
 size_t offset;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 size_t subbufs_produced;
 size_t subbufs_consumed;
 struct rchan *chan;
 wait_queue_head_t read_wait;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct work_struct wake_readers;
 struct dentry *dentry;
 struct kref kref;
 struct page **page_array;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 unsigned int page_count;
 unsigned int finalized;
 size_t *padding;
 size_t prev_padding;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 size_t bytes_consumed;
 unsigned int cpu;
} ____cacheline_aligned;
struct rchan
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
{
 u32 version;
 size_t subbuf_size;
 size_t n_subbufs;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 size_t alloc_size;
 struct rchan_callbacks *cb;
 struct kref kref;
 void *private_data;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 size_t last_toobig;
 struct rchan_buf *buf[NR_CPUS];
};
struct rchan_callbacks
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
{
 int (*subbuf_start) (struct rchan_buf *buf,
 void *subbuf,
 void *prev_subbuf,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 size_t prev_padding);
 void (*buf_mapped)(struct rchan_buf *buf,
 struct file *filp);
 void (*buf_unmapped)(struct rchan_buf *buf,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct file *filp);
 struct dentry *(*create_buf_file)(const char *filename,
 struct dentry *parent,
 int mode,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct rchan_buf *buf,
 int *is_global);
 int (*remove_buf_file)(struct dentry *dentry);
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct rchan *relay_open(const char *base_filename,
 struct dentry *parent,
 size_t subbuf_size,
 size_t n_subbufs,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct rchan_callbacks *cb);
#endif
