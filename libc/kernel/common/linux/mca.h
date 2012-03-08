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
#ifndef _LINUX_MCA_H
#define _LINUX_MCA_H
#include <linux/device.h>
#define MCA_bus 0
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
typedef int (*MCA_ProcFn)(char* buf, int slot, void* dev);
enum MCA_AdapterStatus {
 MCA_ADAPTER_NORMAL = 0,
 MCA_ADAPTER_NONE = 1,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 MCA_ADAPTER_DISABLED = 2,
 MCA_ADAPTER_ERROR = 3
};
struct mca_device {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 u64 dma_mask;
 int pos_id;
 int slot;
 int index;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 int driver_loaded;
 unsigned char pos[8];
 short pos_register;
 enum MCA_AdapterStatus status;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct device dev;
 char name[32];
};
#define to_mca_device(mdev) container_of(mdev, struct mca_device, dev)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct mca_bus_accessor_functions {
 unsigned char (*mca_read_pos)(struct mca_device *, int reg);
 void (*mca_write_pos)(struct mca_device *, int reg,
 unsigned char byte);
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 int (*mca_transform_irq)(struct mca_device *, int irq);
 int (*mca_transform_ioport)(struct mca_device *,
 int region);
 void * (*mca_transform_memory)(struct mca_device *,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 void *memory);
};
struct mca_bus {
 u64 default_dma_mask;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 int number;
 struct mca_bus_accessor_functions f;
 struct device dev;
 char name[32];
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
#define to_mca_bus(mdev) container_of(mdev, struct mca_bus, dev)
struct mca_driver {
 const short *id_table;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 void *driver_data;
 struct device_driver driver;
};
#define to_mca_driver(mdriver) container_of(mdriver, struct mca_driver, driver)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#endif
