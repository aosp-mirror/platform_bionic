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
#ifndef _ANDROID_PMEM_H_
#define _ANDROID_PMEM_H_
#define PMEM_IOCTL_MAGIC 'p'
#define PMEM_GET_PHYS _IOW(PMEM_IOCTL_MAGIC, 1, unsigned int)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define PMEM_MAP _IOW(PMEM_IOCTL_MAGIC, 2, unsigned int)
#define PMEM_GET_SIZE _IOW(PMEM_IOCTL_MAGIC, 3, unsigned int)
#define PMEM_UNMAP _IOW(PMEM_IOCTL_MAGIC, 4, unsigned int)
#define PMEM_ALLOCATE _IOW(PMEM_IOCTL_MAGIC, 5, unsigned int)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define PMEM_CONNECT _IOW(PMEM_IOCTL_MAGIC, 6, unsigned int)
#define PMEM_GET_TOTAL_SIZE _IOW(PMEM_IOCTL_MAGIC, 7, unsigned int)
#define PMEM_CACHE_FLUSH _IOW(PMEM_IOCTL_MAGIC, 8, unsigned int)
struct android_pmem_platform_data
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
{
 const char* name;
 unsigned long start;
 unsigned long size;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 unsigned no_allocator;
 unsigned cached;
 unsigned buffered;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct pmem_region {
 unsigned long offset;
 unsigned long len;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#endif
