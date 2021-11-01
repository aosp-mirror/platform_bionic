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
#ifndef __VENDOR_CMD_INTEL_H__
#define __VENDOR_CMD_INTEL_H__
#define INTEL_OUI 0x001735
enum iwl_mvm_vendor_cmd {
  IWL_MVM_VENDOR_CMD_GET_CSME_CONN_INFO = 0x2d,
  IWL_MVM_VENDOR_CMD_HOST_GET_OWNERSHIP = 0x30,
  IWL_MVM_VENDOR_CMD_ROAMING_FORBIDDEN_EVENT = 0x32,
};
enum iwl_vendor_auth_akm_mode {
  IWL_VENDOR_AUTH_OPEN,
  IWL_VENDOR_AUTH_RSNA = 0x6,
  IWL_VENDOR_AUTH_RSNA_PSK,
  IWL_VENDOR_AUTH_SAE = 0x9,
  IWL_VENDOR_AUTH_MAX,
};
enum iwl_mvm_vendor_attr {
  __IWL_MVM_VENDOR_ATTR_INVALID = 0x00,
  IWL_MVM_VENDOR_ATTR_VIF_ADDR = 0x02,
  IWL_MVM_VENDOR_ATTR_ADDR = 0x0a,
  IWL_MVM_VENDOR_ATTR_SSID = 0x3d,
  IWL_MVM_VENDOR_ATTR_STA_CIPHER = 0x51,
  IWL_MVM_VENDOR_ATTR_ROAMING_FORBIDDEN = 0x64,
  IWL_MVM_VENDOR_ATTR_AUTH_MODE = 0x65,
  IWL_MVM_VENDOR_ATTR_CHANNEL_NUM = 0x66,
  IWL_MVM_VENDOR_ATTR_BAND = 0x69,
  IWL_MVM_VENDOR_ATTR_COLLOC_CHANNEL = 0x70,
  IWL_MVM_VENDOR_ATTR_COLLOC_ADDR = 0x71,
  NUM_IWL_MVM_VENDOR_ATTR,
  MAX_IWL_MVM_VENDOR_ATTR = NUM_IWL_MVM_VENDOR_ATTR - 1,
};
#endif
