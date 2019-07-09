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
#ifndef __INCLUDE_UAPI_SOUND_SOF_USER_MANIFEST_H__
#define __INCLUDE_UAPI_SOUND_SOF_USER_MANIFEST_H__
#define SOF_MAN_ELF_TEXT_OFFSET 0x2000
#define SOF_MAN_EXT_HEADER_MAGIC 0x31454124
#define SOF_MAN_MOD_TYPE_BUILTIN 0
#define SOF_MAN_MOD_TYPE_MODULE 1
struct sof_man_module_type {
  uint32_t load_type : 4;
  uint32_t auto_start : 1;
  uint32_t domain_ll : 1;
  uint32_t domain_dp : 1;
  uint32_t rsvd_ : 25;
};
#define SOF_MAN_SEGMENT_TEXT 0
#define SOF_MAN_SEGMENT_RODATA 1
#define SOF_MAN_SEGMENT_DATA 1
#define SOF_MAN_SEGMENT_BSS 2
#define SOF_MAN_SEGMENT_EMPTY 15
union sof_man_segment_flags {
  uint32_t ul;
  struct {
    uint32_t contents : 1;
    uint32_t alloc : 1;
    uint32_t load : 1;
    uint32_t readonly : 1;
    uint32_t code : 1;
    uint32_t data : 1;
    uint32_t _rsvd0 : 2;
    uint32_t type : 4;
    uint32_t _rsvd1 : 4;
    uint32_t length : 16;
  } r;
} __packed;
struct sof_man_segment_desc {
  union sof_man_segment_flags flags;
  uint32_t v_base_addr;
  uint32_t file_offset;
} __packed;
#define SOF_MAN_MOD_ID_LEN 4
#define SOF_MAN_MOD_NAME_LEN 8
#define SOF_MAN_MOD_SHA256_LEN 32
#define SOF_MAN_MOD_ID { '$', 'A', 'M', 'E' }
struct sof_man_module {
  uint8_t struct_id[SOF_MAN_MOD_ID_LEN];
  uint8_t name[SOF_MAN_MOD_NAME_LEN];
  uint8_t uuid[16];
  struct sof_man_module_type type;
  uint8_t hash[SOF_MAN_MOD_SHA256_LEN];
  uint32_t entry_point;
  uint16_t cfg_offset;
  uint16_t cfg_count;
  uint32_t affinity_mask;
  uint16_t instance_max_count;
  uint16_t instance_bss_size;
  struct sof_man_segment_desc segment[3];
} __packed;
struct sof_man_mod_config {
  uint32_t par[4];
  uint32_t is_pages;
  uint32_t cps;
  uint32_t ibs;
  uint32_t obs;
  uint32_t module_flags;
  uint32_t cpc;
  uint32_t obls;
} __packed;
#define SOF_MAN_FW_HDR_FW_NAME_LEN 8
#define SOF_MAN_FW_HDR_ID { '$', 'A', 'M', '1' }
#define SOF_MAN_FW_HDR_NAME "ADSPFW"
#define SOF_MAN_FW_HDR_FLAGS 0x0
#define SOF_MAN_FW_HDR_FEATURES 0xff
struct sof_man_fw_header {
  uint8_t header_id[4];
  uint32_t header_len;
  uint8_t name[SOF_MAN_FW_HDR_FW_NAME_LEN];
  uint32_t preload_page_count;
  uint32_t fw_image_flags;
  uint32_t feature_mask;
  uint16_t major_version;
  uint16_t minor_version;
  uint16_t hotfix_version;
  uint16_t build_version;
  uint32_t num_module_entries;
  uint32_t hw_buf_base_addr;
  uint32_t hw_buf_length;
  uint32_t load_offset;
} __packed;
struct sof_man_fw_desc {
  struct sof_man_fw_header header;
} __packed;
struct sof_man_component_desc {
  uint32_t reserved[2];
  uint32_t version;
  uint8_t hash[SOF_MAN_MOD_SHA256_LEN];
  uint32_t base_offset;
  uint32_t limit_offset;
  uint32_t attributes[4];
} __packed;
struct sof_man_adsp_meta_file_ext {
  uint32_t ext_type;
  uint32_t ext_len;
  uint32_t imr_type;
  uint8_t reserved[16];
  struct sof_man_component_desc comp_desc[1];
} __packed;
struct sof_man_module_manifest {
  struct sof_man_module module;
  uint32_t text_size;
} __packed;
#endif
