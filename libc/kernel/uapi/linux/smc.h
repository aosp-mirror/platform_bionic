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
#ifndef _UAPI_LINUX_SMC_H_
#define _UAPI_LINUX_SMC_H_
enum {
  SMC_PNETID_UNSPEC,
  SMC_PNETID_NAME,
  SMC_PNETID_ETHNAME,
  SMC_PNETID_IBNAME,
  SMC_PNETID_IBPORT,
  __SMC_PNETID_MAX,
  SMC_PNETID_MAX = __SMC_PNETID_MAX - 1
};
enum {
  SMC_PNETID_GET = 1,
  SMC_PNETID_ADD,
  SMC_PNETID_DEL,
  SMC_PNETID_FLUSH
};
#define SMCR_GENL_FAMILY_NAME "SMC_PNETID"
#define SMCR_GENL_FAMILY_VERSION 1
#define SMC_GENL_FAMILY_NAME "SMC_GEN_NETLINK"
#define SMC_GENL_FAMILY_VERSION 1
#define SMC_PCI_ID_STR_LEN 16
enum {
  SMC_NETLINK_GET_SYS_INFO = 1,
  SMC_NETLINK_GET_LGR_SMCR,
  SMC_NETLINK_GET_LINK_SMCR,
  SMC_NETLINK_GET_LGR_SMCD,
  SMC_NETLINK_GET_DEV_SMCD,
  SMC_NETLINK_GET_DEV_SMCR,
};
enum {
  SMC_GEN_UNSPEC,
  SMC_GEN_SYS_INFO,
  SMC_GEN_LGR_SMCR,
  SMC_GEN_LINK_SMCR,
  SMC_GEN_LGR_SMCD,
  SMC_GEN_DEV_SMCD,
  SMC_GEN_DEV_SMCR,
  __SMC_GEN_MAX,
  SMC_GEN_MAX = __SMC_GEN_MAX - 1
};
enum {
  SMC_NLA_SYS_UNSPEC,
  SMC_NLA_SYS_VER,
  SMC_NLA_SYS_REL,
  SMC_NLA_SYS_IS_ISM_V2,
  SMC_NLA_SYS_LOCAL_HOST,
  SMC_NLA_SYS_SEID,
  __SMC_NLA_SYS_MAX,
  SMC_NLA_SYS_MAX = __SMC_NLA_SYS_MAX - 1
};
enum {
  SMC_NLA_LGR_V2_VER,
  SMC_NLA_LGR_V2_REL,
  SMC_NLA_LGR_V2_OS,
  SMC_NLA_LGR_V2_NEG_EID,
  SMC_NLA_LGR_V2_PEER_HOST,
};
enum {
  SMC_NLA_LGR_R_UNSPEC,
  SMC_NLA_LGR_R_ID,
  SMC_NLA_LGR_R_ROLE,
  SMC_NLA_LGR_R_TYPE,
  SMC_NLA_LGR_R_PNETID,
  SMC_NLA_LGR_R_VLAN_ID,
  SMC_NLA_LGR_R_CONNS_NUM,
  __SMC_NLA_LGR_R_MAX,
  SMC_NLA_LGR_R_MAX = __SMC_NLA_LGR_R_MAX - 1
};
enum {
  SMC_NLA_LINK_UNSPEC,
  SMC_NLA_LINK_ID,
  SMC_NLA_LINK_IB_DEV,
  SMC_NLA_LINK_IB_PORT,
  SMC_NLA_LINK_GID,
  SMC_NLA_LINK_PEER_GID,
  SMC_NLA_LINK_CONN_CNT,
  SMC_NLA_LINK_NET_DEV,
  SMC_NLA_LINK_UID,
  SMC_NLA_LINK_PEER_UID,
  SMC_NLA_LINK_STATE,
  __SMC_NLA_LINK_MAX,
  SMC_NLA_LINK_MAX = __SMC_NLA_LINK_MAX - 1
};
enum {
  SMC_NLA_LGR_D_UNSPEC,
  SMC_NLA_LGR_D_ID,
  SMC_NLA_LGR_D_GID,
  SMC_NLA_LGR_D_PEER_GID,
  SMC_NLA_LGR_D_VLAN_ID,
  SMC_NLA_LGR_D_CONNS_NUM,
  SMC_NLA_LGR_D_PNETID,
  SMC_NLA_LGR_D_CHID,
  SMC_NLA_LGR_D_PAD,
  SMC_NLA_LGR_V2,
  __SMC_NLA_LGR_D_MAX,
  SMC_NLA_LGR_D_MAX = __SMC_NLA_LGR_D_MAX - 1
};
enum {
  SMC_NLA_DEV_PORT_UNSPEC,
  SMC_NLA_DEV_PORT_PNET_USR,
  SMC_NLA_DEV_PORT_PNETID,
  SMC_NLA_DEV_PORT_NETDEV,
  SMC_NLA_DEV_PORT_STATE,
  SMC_NLA_DEV_PORT_VALID,
  SMC_NLA_DEV_PORT_LNK_CNT,
  __SMC_NLA_DEV_PORT_MAX,
  SMC_NLA_DEV_PORT_MAX = __SMC_NLA_DEV_PORT_MAX - 1
};
enum {
  SMC_NLA_DEV_UNSPEC,
  SMC_NLA_DEV_USE_CNT,
  SMC_NLA_DEV_IS_CRIT,
  SMC_NLA_DEV_PCI_FID,
  SMC_NLA_DEV_PCI_CHID,
  SMC_NLA_DEV_PCI_VENDOR,
  SMC_NLA_DEV_PCI_DEVICE,
  SMC_NLA_DEV_PCI_ID,
  SMC_NLA_DEV_PORT,
  SMC_NLA_DEV_PORT2,
  SMC_NLA_DEV_IB_NAME,
  __SMC_NLA_DEV_MAX,
  SMC_NLA_DEV_MAX = __SMC_NLA_DEV_MAX - 1
};
#endif
