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
#ifndef _LINUX_BACKING_DEV_H
#define _LINUX_BACKING_DEV_H
#include <asm/atomic.h>
enum bdi_state {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 BDI_pdflush,
 BDI_write_congested,
 BDI_read_congested,
 BDI_unused,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
typedef int (congested_fn)(void *, int);
struct backing_dev_info {
 unsigned long ra_pages;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 unsigned long state;
 unsigned int capabilities;
 congested_fn *congested_fn;
 void *congested_data;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 void (*unplug_io_fn)(struct backing_dev_info *, struct page *);
 void *unplug_io_data;
};
#define BDI_CAP_NO_ACCT_DIRTY 0x00000001
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define BDI_CAP_NO_WRITEBACK 0x00000002
#define BDI_CAP_MAP_COPY 0x00000004
#define BDI_CAP_MAP_DIRECT 0x00000008
#define BDI_CAP_READ_MAP 0x00000010
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define BDI_CAP_WRITE_MAP 0x00000020
#define BDI_CAP_EXEC_MAP 0x00000040
#define BDI_CAP_VMFLAGS   (BDI_CAP_READ_MAP | BDI_CAP_WRITE_MAP | BDI_CAP_EXEC_MAP)
#if defined(VM_MAYREAD) && BDI_CAP_READ_MAP != (VM_MAYREAD || BDI_CAP_WRITE_MAP != (VM_MAYWRITE || BDI_CAP_EXEC_MAP != VM_MAYEXEC))
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#error please change backing_dev_info::capabilities flags
#endif
#define bdi_cap_writeback_dirty(bdi)   (!((bdi)->capabilities & BDI_CAP_NO_WRITEBACK))
#define bdi_cap_account_dirty(bdi)   (!((bdi)->capabilities & BDI_CAP_NO_ACCT_DIRTY))
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define mapping_cap_writeback_dirty(mapping)   bdi_cap_writeback_dirty((mapping)->backing_dev_info)
#define mapping_cap_account_dirty(mapping)   bdi_cap_account_dirty((mapping)->backing_dev_info)
#endif
