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
#ifndef _UAPI_LINUX_BATMAN_ADV_H_
#define _UAPI_LINUX_BATMAN_ADV_H_
#define BATADV_NL_NAME "batadv"
#define BATADV_NL_MCAST_GROUP_TPMETER "tpmeter"
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
enum batadv_nl_attrs {
  BATADV_ATTR_UNSPEC,
  BATADV_ATTR_VERSION,
  BATADV_ATTR_ALGO_NAME,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  BATADV_ATTR_MESH_IFINDEX,
  BATADV_ATTR_MESH_IFNAME,
  BATADV_ATTR_MESH_ADDRESS,
  BATADV_ATTR_HARD_IFINDEX,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  BATADV_ATTR_HARD_IFNAME,
  BATADV_ATTR_HARD_ADDRESS,
  BATADV_ATTR_ORIG_ADDRESS,
  BATADV_ATTR_TPMETER_RESULT,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  BATADV_ATTR_TPMETER_TEST_TIME,
  BATADV_ATTR_TPMETER_BYTES,
  BATADV_ATTR_TPMETER_COOKIE,
  BATADV_ATTR_PAD,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  __BATADV_ATTR_AFTER_LAST,
  NUM_BATADV_ATTR = __BATADV_ATTR_AFTER_LAST,
  BATADV_ATTR_MAX = __BATADV_ATTR_AFTER_LAST - 1
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
enum batadv_nl_commands {
  BATADV_CMD_UNSPEC,
  BATADV_CMD_GET_MESH_INFO,
  BATADV_CMD_TP_METER,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  BATADV_CMD_TP_METER_CANCEL,
  __BATADV_CMD_AFTER_LAST,
  BATADV_CMD_MAX = __BATADV_CMD_AFTER_LAST - 1
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
enum batadv_tp_meter_reason {
  BATADV_TP_REASON_COMPLETE = 3,
  BATADV_TP_REASON_CANCEL = 4,
  BATADV_TP_REASON_DST_UNREACHABLE = 128,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  BATADV_TP_REASON_RESEND_LIMIT = 129,
  BATADV_TP_REASON_ALREADY_ONGOING = 130,
  BATADV_TP_REASON_MEMORY_ERROR = 131,
  BATADV_TP_REASON_CANT_SEND = 132,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  BATADV_TP_REASON_TOO_MANY = 133,
};
#endif
