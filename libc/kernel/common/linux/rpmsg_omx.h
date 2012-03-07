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
#ifndef RPMSG_OMX_H
#define RPMSG_OMX_H
#include <linux/ioctl.h>
#define OMX_IOC_MAGIC 'X'
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define OMX_IOCCONNECT _IOW(OMX_IOC_MAGIC, 1, char *)
#define OMX_IOCIONREGISTER _IOWR(OMX_IOC_MAGIC, 2, struct ion_fd_data)
#define OMX_IOCIONUNREGISTER _IOWR(OMX_IOC_MAGIC, 3, struct ion_fd_data)
#define OMX_IOC_MAXNR (3)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct omx_conn_req {
 char name[48];
} __packed;
struct omx_packet {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 uint16_t desc;
 uint16_t msg_id;
 uint32_t flags;
 uint32_t fxn_idx;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 int32_t result;
 uint32_t data_size;
 uint32_t data[0];
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#endif
