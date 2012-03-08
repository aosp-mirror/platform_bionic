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
#ifndef __LINUX_FILE_H
#define __LINUX_FILE_H
#include <asm/atomic.h>
#include <linux/posix_types.h>
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#include <linux/compiler.h>
#include <linux/spinlock.h>
#include <linux/rcupdate.h>
#include <linux/types.h>
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define NR_OPEN_DEFAULT BITS_PER_LONG
struct embedded_fd_set {
 unsigned long fds_bits[1];
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define EMBEDDED_FD_SET_SIZE (BITS_PER_BYTE * sizeof(struct embedded_fd_set))
struct fdtable {
 unsigned int max_fds;
 int max_fdset;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct file ** fd;
 fd_set *close_on_exec;
 fd_set *open_fds;
 struct rcu_head rcu;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct files_struct *free_files;
 struct fdtable *next;
};
struct files_struct {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 atomic_t count;
 struct fdtable *fdt;
 struct fdtable fdtab;
 spinlock_t file_lock ____cacheline_aligned_in_smp;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 int next_fd;
 struct embedded_fd_set close_on_exec_init;
 struct embedded_fd_set open_fds_init;
 struct file * fd_array[NR_OPEN_DEFAULT];
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
#define files_fdtable(files) (rcu_dereference((files)->fdt))
struct kmem_cache;
#define fcheck(fd) fcheck_files(current->files, fd)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct task_struct;
struct files_struct *get_files_struct(struct task_struct *);
#endif
