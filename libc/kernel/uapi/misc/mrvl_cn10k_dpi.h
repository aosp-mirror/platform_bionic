/*
 * This file is auto-generated. Modifications will be lost.
 *
 * See https://android.googlesource.com/platform/bionic/+/master/libc/kernel/
 * for more information.
 */
#ifndef __MRVL_CN10K_DPI_H__
#define __MRVL_CN10K_DPI_H__
#include <linux/types.h>
#define DPI_MAX_ENGINES 6
struct dpi_mps_mrrs_cfg {
  __u16 max_read_req_sz;
  __u16 max_payload_sz;
  __u16 port;
  __u16 reserved;
};
struct dpi_engine_cfg {
  __u64 fifo_mask;
  __u16 molr[DPI_MAX_ENGINES];
  __u16 update_molr;
  __u16 reserved;
};
#define DPI_MAGIC_NUM 0xB8
#define DPI_MPS_MRRS_CFG _IOW(DPI_MAGIC_NUM, 1, struct dpi_mps_mrrs_cfg)
#define DPI_ENGINE_CFG _IOW(DPI_MAGIC_NUM, 2, struct dpi_engine_cfg)
#endif
