/****************************************************************************
 ****************************************************************************
 ***
 ***   This header was automatically generated from a Linux kernel header
 ***   of the same name, to make information necessary for userspace to
 ***   call into the kernel available to libc.  It contains only constants,
 ***   structures, and macros generated from the original header, and thus,
 ***   contains no copyrightable information.
 ***
 ****************************************************************************
 ****************************************************************************/
#ifndef __LINUX_USB_F_MTP_H
#define __LINUX_USB_F_MTP_H

#define MTP_INTERFACE_MODE_MTP 0
#define MTP_INTERFACE_MODE_PTP 1

struct mtp_file_range {

 int fd;

 loff_t offset;

 size_t length;
};

struct mtp_event {

 size_t length;

 void *data;
};

#define MTP_SEND_FILE _IOW('M', 0, struct mtp_file_range)

#define MTP_RECEIVE_FILE _IOW('M', 1, struct mtp_file_range)

#define MTP_SET_INTERFACE_MODE _IOW('M', 2, int)

#define MTP_SEND_EVENT _IOW('M', 3, struct mtp_event)

#endif
