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
#ifndef LINUX_MMC_CARD_H
#define LINUX_MMC_CARD_H
#include <linux/mmc/mmc.h>
struct mmc_cid {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 unsigned int manfid;
 char prod_name[8];
 unsigned int serial;
 unsigned short oemid;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 unsigned short year;
 unsigned char hwrev;
 unsigned char fwrev;
 unsigned char month;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct mmc_csd {
 unsigned char mmca_vsn;
 unsigned short cmdclass;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 unsigned short tacc_clks;
 unsigned int tacc_ns;
 unsigned int r2w_factor;
 unsigned int max_dtr;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 unsigned int read_blkbits;
 unsigned int write_blkbits;
 unsigned int capacity;
 unsigned int read_partial:1,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 read_misalign:1,
 write_partial:1,
 write_misalign:1;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct sd_scr {
 unsigned char sda_vsn;
 unsigned char bus_widths;
#define SD_SCR_BUS_WIDTH_1 (1<<0)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define SD_SCR_BUS_WIDTH_4 (1<<2)
};
struct mmc_host;
struct mmc_card {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct list_head node;
 struct mmc_host *host;
 struct device dev;
 unsigned int rca;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 unsigned int state;
#define MMC_STATE_PRESENT (1<<0)
#define MMC_STATE_DEAD (1<<1)
#define MMC_STATE_BAD (1<<2)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define MMC_STATE_SDCARD (1<<3)
#define MMC_STATE_READONLY (1<<4)
 u32 raw_cid[4];
 u32 raw_csd[4];
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 u32 raw_scr[2];
 struct mmc_cid cid;
 struct mmc_csd csd;
 struct sd_scr scr;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
#define mmc_card_present(c) ((c)->state & MMC_STATE_PRESENT)
#define mmc_card_dead(c) ((c)->state & MMC_STATE_DEAD)
#define mmc_card_bad(c) ((c)->state & MMC_STATE_BAD)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define mmc_card_sd(c) ((c)->state & MMC_STATE_SDCARD)
#define mmc_card_readonly(c) ((c)->state & MMC_STATE_READONLY)
#define mmc_card_set_present(c) ((c)->state |= MMC_STATE_PRESENT)
#define mmc_card_set_dead(c) ((c)->state |= MMC_STATE_DEAD)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define mmc_card_set_bad(c) ((c)->state |= MMC_STATE_BAD)
#define mmc_card_set_sd(c) ((c)->state |= MMC_STATE_SDCARD)
#define mmc_card_set_readonly(c) ((c)->state |= MMC_STATE_READONLY)
#define mmc_card_name(c) ((c)->cid.prod_name)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define mmc_card_id(c) ((c)->dev.bus_id)
#define mmc_list_to_card(l) container_of(l, struct mmc_card, node)
#define mmc_get_drvdata(c) dev_get_drvdata(&(c)->dev)
#define mmc_set_drvdata(c,d) dev_set_drvdata(&(c)->dev, d)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct mmc_driver {
 struct device_driver drv;
 int (*probe)(struct mmc_card *);
 void (*remove)(struct mmc_card *);
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 int (*suspend)(struct mmc_card *, pm_message_t);
 int (*resume)(struct mmc_card *);
};
#define mmc_card_release_host(c) mmc_release_host((c)->host)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#endif
