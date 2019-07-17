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
#ifndef __INCLUDE_UAPI_SOUND_SOF_USER_TRACE_H__
#define __INCLUDE_UAPI_SOUND_SOF_USER_TRACE_H__
struct system_time {
  uint32_t val_l;
  uint32_t val_u;
} __packed;
#define LOG_ENABLE 1
#define LOG_DISABLE 0
#define LOG_LEVEL_CRITICAL 1
#define LOG_LEVEL_VERBOSE 2
struct log_buffer_layout {
  uint32_t read_ptr;
  uint32_t write_ptr;
  uint32_t buffer[0];
} __packed;
struct log_buffer_status {
  uint32_t core_id;
} __packed;
#define TRACE_ID_LENGTH 12
struct log_entry_header {
  uint32_t id_0 : TRACE_ID_LENGTH;
  uint32_t id_1 : TRACE_ID_LENGTH;
  uint32_t core_id : 8;
  uint64_t timestamp;
  uint32_t log_entry_address;
} __packed;
#endif
