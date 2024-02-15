/*
 * This file is auto-generated. Modifications will be lost.
 *
 * See https://android.googlesource.com/platform/bionic/+/master/libc/kernel/
 * for more information.
 */
#ifndef SCSI_BSG_MPI3MR_H_INCLUDED
#define SCSI_BSG_MPI3MR_H_INCLUDED
#include <linux/types.h>
#define MPI3MR_IOCTL_VERSION 0x06
#define MPI3MR_APP_DEFAULT_TIMEOUT (60)
#define MPI3MR_BSG_ADPTYPE_UNKNOWN 0
#define MPI3MR_BSG_ADPTYPE_AVGFAMILY 1
#define MPI3MR_BSG_ADPSTATE_UNKNOWN 0
#define MPI3MR_BSG_ADPSTATE_OPERATIONAL 1
#define MPI3MR_BSG_ADPSTATE_FAULT 2
#define MPI3MR_BSG_ADPSTATE_IN_RESET 3
#define MPI3MR_BSG_ADPSTATE_UNRECOVERABLE 4
#define MPI3MR_BSG_ADPRESET_UNKNOWN 0
#define MPI3MR_BSG_ADPRESET_SOFT 1
#define MPI3MR_BSG_ADPRESET_DIAG_FAULT 2
#define MPI3MR_BSG_LOGDATA_MAX_ENTRIES 400
#define MPI3MR_BSG_LOGDATA_ENTRY_HEADER_SZ 4
#define MPI3MR_DRVBSG_OPCODE_UNKNOWN 0
#define MPI3MR_DRVBSG_OPCODE_ADPINFO 1
#define MPI3MR_DRVBSG_OPCODE_ADPRESET 2
#define MPI3MR_DRVBSG_OPCODE_ALLTGTDEVINFO 4
#define MPI3MR_DRVBSG_OPCODE_GETCHGCNT 5
#define MPI3MR_DRVBSG_OPCODE_LOGDATAENABLE 6
#define MPI3MR_DRVBSG_OPCODE_PELENABLE 7
#define MPI3MR_DRVBSG_OPCODE_GETLOGDATA 8
#define MPI3MR_DRVBSG_OPCODE_QUERY_HDB 9
#define MPI3MR_DRVBSG_OPCODE_REPOST_HDB 10
#define MPI3MR_DRVBSG_OPCODE_UPLOAD_HDB 11
#define MPI3MR_DRVBSG_OPCODE_REFRESH_HDB_TRIGGERS 12
#define MPI3MR_BSG_BUFTYPE_UNKNOWN 0
#define MPI3MR_BSG_BUFTYPE_RAIDMGMT_CMD 1
#define MPI3MR_BSG_BUFTYPE_RAIDMGMT_RESP 2
#define MPI3MR_BSG_BUFTYPE_DATA_IN 3
#define MPI3MR_BSG_BUFTYPE_DATA_OUT 4
#define MPI3MR_BSG_BUFTYPE_MPI_REPLY 5
#define MPI3MR_BSG_BUFTYPE_ERR_RESPONSE 6
#define MPI3MR_BSG_BUFTYPE_MPI_REQUEST 0xFE
#define MPI3MR_BSG_MPI_REPLY_BUFTYPE_UNKNOWN 0
#define MPI3MR_BSG_MPI_REPLY_BUFTYPE_STATUS 1
#define MPI3MR_BSG_MPI_REPLY_BUFTYPE_ADDRESS 2
#define MPI3MR_HDB_BUFTYPE_UNKNOWN 0
#define MPI3MR_HDB_BUFTYPE_TRACE 1
#define MPI3MR_HDB_BUFTYPE_FIRMWARE 2
#define MPI3MR_HDB_BUFTYPE_RESERVED 3
#define MPI3MR_HDB_BUFSTATUS_UNKNOWN 0
#define MPI3MR_HDB_BUFSTATUS_NOT_ALLOCATED 1
#define MPI3MR_HDB_BUFSTATUS_POSTED_UNPAUSED 2
#define MPI3MR_HDB_BUFSTATUS_POSTED_PAUSED 3
#define MPI3MR_HDB_BUFSTATUS_RELEASED 4
#define MPI3MR_HDB_TRIGGER_TYPE_UNKNOWN 0
#define MPI3MR_HDB_TRIGGER_TYPE_DIAGFAULT 1
#define MPI3MR_HDB_TRIGGER_TYPE_ELEMENT 2
#define MPI3MR_HDB_TRIGGER_TYPE_MASTER 3
enum command {
  MPI3MR_DRV_CMD = 1,
  MPI3MR_MPT_CMD = 2,
};
struct mpi3_driver_info_layout {
  __le32 information_length;
  __u8 driver_signature[12];
  __u8 os_name[16];
  __u8 os_version[12];
  __u8 driver_name[20];
  __u8 driver_version[32];
  __u8 driver_release_date[20];
  __le32 driver_capabilities;
};
struct mpi3mr_bsg_in_adpinfo {
  __u32 adp_type;
  __u32 rsvd1;
  __u32 pci_dev_id;
  __u32 pci_dev_hw_rev;
  __u32 pci_subsys_dev_id;
  __u32 pci_subsys_ven_id;
  __u32 pci_dev : 5;
  __u32 pci_func : 3;
  __u32 pci_bus : 8;
  __u16 rsvd2;
  __u32 pci_seg_id;
  __u32 app_intfc_ver;
  __u8 adp_state;
  __u8 rsvd3;
  __u16 rsvd4;
  __u32 rsvd5[2];
  struct mpi3_driver_info_layout driver_info;
};
struct mpi3mr_bsg_adp_reset {
  __u8 reset_type;
  __u8 rsvd1;
  __u16 rsvd2;
};
struct mpi3mr_change_count {
  __u16 change_count;
  __u16 rsvd;
};
struct mpi3mr_device_map_info {
  __u16 handle;
  __u16 perst_id;
  __u32 target_id;
  __u8 bus_id;
  __u8 rsvd1;
  __u16 rsvd2;
};
struct mpi3mr_all_tgt_info {
  __u16 num_devices;
  __u16 rsvd1;
  __u32 rsvd2;
  struct mpi3mr_device_map_info dmi[1];
};
struct mpi3mr_logdata_enable {
  __u16 max_entries;
  __u16 rsvd;
};
struct mpi3mr_bsg_out_pel_enable {
  __u16 pel_locale;
  __u8 pel_class;
  __u8 rsvd;
};
struct mpi3mr_logdata_entry {
  __u8 valid_entry;
  __u8 rsvd1;
  __u16 rsvd2;
  __u8 data[1];
};
struct mpi3mr_bsg_in_log_data {
  struct mpi3mr_logdata_entry entry[1];
};
struct mpi3mr_hdb_entry {
  __u8 buf_type;
  __u8 status;
  __u8 trigger_type;
  __u8 rsvd1;
  __u16 size;
  __u16 rsvd2;
  __u64 trigger_data;
  __u32 rsvd3;
  __u32 rsvd4;
};
struct mpi3mr_bsg_in_hdb_status {
  __u8 num_hdb_types;
  __u8 rsvd1;
  __u16 rsvd2;
  __u32 rsvd3;
  struct mpi3mr_hdb_entry entry[1];
};
struct mpi3mr_bsg_out_repost_hdb {
  __u8 buf_type;
  __u8 rsvd1;
  __u16 rsvd2;
};
struct mpi3mr_bsg_out_upload_hdb {
  __u8 buf_type;
  __u8 rsvd1;
  __u16 rsvd2;
  __u32 start_offset;
  __u32 length;
};
struct mpi3mr_bsg_out_refresh_hdb_triggers {
  __u8 page_type;
  __u8 rsvd1;
  __u16 rsvd2;
};
struct mpi3mr_bsg_drv_cmd {
  __u8 mrioc_id;
  __u8 opcode;
  __u16 rsvd1;
  __u32 rsvd2[4];
};
struct mpi3mr_bsg_in_reply_buf {
  __u8 mpi_reply_type;
  __u8 rsvd1;
  __u16 rsvd2;
  __u8 reply_buf[1];
};
struct mpi3mr_buf_entry {
  __u8 buf_type;
  __u8 rsvd1;
  __u16 rsvd2;
  __u32 buf_len;
};
struct mpi3mr_buf_entry_list {
  __u8 num_of_entries;
  __u8 rsvd1;
  __u16 rsvd2;
  __u32 rsvd3;
  struct mpi3mr_buf_entry buf_entry[1];
};
struct mpi3mr_bsg_mptcmd {
  __u8 mrioc_id;
  __u8 rsvd1;
  __u16 timeout;
  __u32 rsvd2;
  struct mpi3mr_buf_entry_list buf_entry_list;
};
struct mpi3mr_bsg_packet {
  __u8 cmd_type;
  __u8 rsvd1;
  __u16 rsvd2;
  __u32 rsvd3;
  union {
    struct mpi3mr_bsg_drv_cmd drvrcmd;
    struct mpi3mr_bsg_mptcmd mptcmd;
  } cmd;
};
struct mpi3_nvme_encapsulated_request {
  __le16 host_tag;
  __u8 ioc_use_only02;
  __u8 function;
  __le16 ioc_use_only04;
  __u8 ioc_use_only06;
  __u8 msg_flags;
  __le16 change_count;
  __le16 dev_handle;
  __le16 encapsulated_command_length;
  __le16 flags;
  __le32 data_length;
  __le32 reserved14[3];
  __le32 command[];
};
struct mpi3_nvme_encapsulated_error_reply {
  __le16 host_tag;
  __u8 ioc_use_only02;
  __u8 function;
  __le16 ioc_use_only04;
  __u8 ioc_use_only06;
  __u8 msg_flags;
  __le16 ioc_use_only08;
  __le16 ioc_status;
  __le32 ioc_log_info;
  __le32 nvme_completion_entry[4];
};
#define MPI3MR_NVME_PRP_SIZE 8
#define MPI3MR_NVME_CMD_PRP1_OFFSET 24
#define MPI3MR_NVME_CMD_PRP2_OFFSET 32
#define MPI3MR_NVME_CMD_SGL_OFFSET 24
#define MPI3MR_NVME_DATA_FORMAT_PRP 0
#define MPI3MR_NVME_DATA_FORMAT_SGL1 1
#define MPI3MR_NVME_DATA_FORMAT_SGL2 2
struct mpi3_scsi_task_mgmt_request {
  __le16 host_tag;
  __u8 ioc_use_only02;
  __u8 function;
  __le16 ioc_use_only04;
  __u8 ioc_use_only06;
  __u8 msg_flags;
  __le16 change_count;
  __le16 dev_handle;
  __le16 task_host_tag;
  __u8 task_type;
  __u8 reserved0f;
  __le16 task_request_queue_id;
  __le16 reserved12;
  __le32 reserved14;
  __u8 lun[8];
};
#define MPI3_SCSITASKMGMT_MSGFLAGS_DO_NOT_SEND_TASK_IU (0x08)
#define MPI3_SCSITASKMGMT_TASKTYPE_ABORT_TASK (0x01)
#define MPI3_SCSITASKMGMT_TASKTYPE_ABORT_TASK_SET (0x02)
#define MPI3_SCSITASKMGMT_TASKTYPE_TARGET_RESET (0x03)
#define MPI3_SCSITASKMGMT_TASKTYPE_LOGICAL_UNIT_RESET (0x05)
#define MPI3_SCSITASKMGMT_TASKTYPE_CLEAR_TASK_SET (0x06)
#define MPI3_SCSITASKMGMT_TASKTYPE_QUERY_TASK (0x07)
#define MPI3_SCSITASKMGMT_TASKTYPE_CLEAR_ACA (0x08)
#define MPI3_SCSITASKMGMT_TASKTYPE_QUERY_TASK_SET (0x09)
#define MPI3_SCSITASKMGMT_TASKTYPE_QUERY_ASYNC_EVENT (0x0a)
#define MPI3_SCSITASKMGMT_TASKTYPE_I_T_NEXUS_RESET (0x0b)
struct mpi3_scsi_task_mgmt_reply {
  __le16 host_tag;
  __u8 ioc_use_only02;
  __u8 function;
  __le16 ioc_use_only04;
  __u8 ioc_use_only06;
  __u8 msg_flags;
  __le16 ioc_use_only08;
  __le16 ioc_status;
  __le32 ioc_log_info;
  __le32 termination_count;
  __le32 response_data;
  __le32 reserved18;
};
#define MPI3_SCSITASKMGMT_RSPCODE_TM_COMPLETE (0x00)
#define MPI3_SCSITASKMGMT_RSPCODE_INVALID_FRAME (0x02)
#define MPI3_SCSITASKMGMT_RSPCODE_TM_FUNCTION_NOT_SUPPORTED (0x04)
#define MPI3_SCSITASKMGMT_RSPCODE_TM_FAILED (0x05)
#define MPI3_SCSITASKMGMT_RSPCODE_TM_SUCCEEDED (0x08)
#define MPI3_SCSITASKMGMT_RSPCODE_TM_INVALID_LUN (0x09)
#define MPI3_SCSITASKMGMT_RSPCODE_TM_OVERLAPPED_TAG (0x0a)
#define MPI3_SCSITASKMGMT_RSPCODE_IO_QUEUED_ON_IOC (0x80)
#define MPI3_SCSITASKMGMT_RSPCODE_TM_NVME_DENIED (0x81)
#define MPI3_PEL_LOCALE_FLAGS_NON_BLOCKING_BOOT_EVENT (0x0200)
#define MPI3_PEL_LOCALE_FLAGS_BLOCKING_BOOT_EVENT (0x0100)
#define MPI3_PEL_LOCALE_FLAGS_PCIE (0x0080)
#define MPI3_PEL_LOCALE_FLAGS_CONFIGURATION (0x0040)
#define MPI3_PEL_LOCALE_FLAGS_CONTROLER (0x0020)
#define MPI3_PEL_LOCALE_FLAGS_SAS (0x0010)
#define MPI3_PEL_LOCALE_FLAGS_EPACK (0x0008)
#define MPI3_PEL_LOCALE_FLAGS_ENCLOSURE (0x0004)
#define MPI3_PEL_LOCALE_FLAGS_PD (0x0002)
#define MPI3_PEL_LOCALE_FLAGS_VD (0x0001)
#define MPI3_PEL_CLASS_DEBUG (0x00)
#define MPI3_PEL_CLASS_PROGRESS (0x01)
#define MPI3_PEL_CLASS_INFORMATIONAL (0x02)
#define MPI3_PEL_CLASS_WARNING (0x03)
#define MPI3_PEL_CLASS_CRITICAL (0x04)
#define MPI3_PEL_CLASS_FATAL (0x05)
#define MPI3_PEL_CLASS_FAULT (0x06)
#define MPI3_BSG_FUNCTION_MGMT_PASSTHROUGH (0x0a)
#define MPI3_BSG_FUNCTION_SCSI_IO (0x20)
#define MPI3_BSG_FUNCTION_SCSI_TASK_MGMT (0x21)
#define MPI3_BSG_FUNCTION_SMP_PASSTHROUGH (0x22)
#define MPI3_BSG_FUNCTION_NVME_ENCAPSULATED (0x24)
#endif
