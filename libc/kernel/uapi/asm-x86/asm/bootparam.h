/*
 * This file is auto-generated. Modifications will be lost.
 *
 * See https://android.googlesource.com/platform/bionic/+/master/libc/kernel/
 * for more information.
 */
#ifndef _ASM_X86_BOOTPARAM_H
#define _ASM_X86_BOOTPARAM_H
#include <asm/setup_data.h>
#define RAMDISK_IMAGE_START_MASK 0x07FF
#define RAMDISK_PROMPT_FLAG 0x8000
#define RAMDISK_LOAD_FLAG 0x4000
#define LOADED_HIGH (1 << 0)
#define KASLR_FLAG (1 << 1)
#define QUIET_FLAG (1 << 5)
#define KEEP_SEGMENTS (1 << 6)
#define CAN_USE_HEAP (1 << 7)
#define XLF_KERNEL_64 (1 << 0)
#define XLF_CAN_BE_LOADED_ABOVE_4G (1 << 1)
#define XLF_EFI_HANDOVER_32 (1 << 2)
#define XLF_EFI_HANDOVER_64 (1 << 3)
#define XLF_EFI_KEXEC (1 << 4)
#define XLF_5LEVEL (1 << 5)
#define XLF_5LEVEL_ENABLED (1 << 6)
#define XLF_MEM_ENCRYPTION (1 << 7)
#ifndef __ASSEMBLY__
#include <linux/types.h>
#include <linux/screen_info.h>
#include <linux/apm_bios.h>
#include <linux/edd.h>
#include <asm/ist.h>
#include <video/edid.h>
struct setup_header {
  __u8 setup_sects;
  __u16 root_flags;
  __u32 syssize;
  __u16 ram_size;
  __u16 vid_mode;
  __u16 root_dev;
  __u16 boot_flag;
  __u16 jump;
  __u32 header;
  __u16 version;
  __u32 realmode_swtch;
  __u16 start_sys_seg;
  __u16 kernel_version;
  __u8 type_of_loader;
  __u8 loadflags;
  __u16 setup_move_size;
  __u32 code32_start;
  __u32 ramdisk_image;
  __u32 ramdisk_size;
  __u32 bootsect_kludge;
  __u16 heap_end_ptr;
  __u8 ext_loader_ver;
  __u8 ext_loader_type;
  __u32 cmd_line_ptr;
  __u32 initrd_addr_max;
  __u32 kernel_alignment;
  __u8 relocatable_kernel;
  __u8 min_alignment;
  __u16 xloadflags;
  __u32 cmdline_size;
  __u32 hardware_subarch;
  __u64 hardware_subarch_data;
  __u32 payload_offset;
  __u32 payload_length;
  __u64 setup_data;
  __u64 pref_address;
  __u32 init_size;
  __u32 handover_offset;
  __u32 kernel_info_offset;
} __attribute__((packed));
struct sys_desc_table {
  __u16 length;
  __u8 table[14];
};
struct olpc_ofw_header {
  __u32 ofw_magic;
  __u32 ofw_version;
  __u32 cif_handler;
  __u32 irq_desc_table;
} __attribute__((packed));
struct efi_info {
  __u32 efi_loader_signature;
  __u32 efi_systab;
  __u32 efi_memdesc_size;
  __u32 efi_memdesc_version;
  __u32 efi_memmap;
  __u32 efi_memmap_size;
  __u32 efi_systab_hi;
  __u32 efi_memmap_hi;
};
#define E820_MAX_ENTRIES_ZEROPAGE 128
#define JAILHOUSE_SETUP_REQUIRED_VERSION 1
struct boot_params {
  struct screen_info screen_info;
  struct apm_bios_info apm_bios_info;
  __u8 _pad2[4];
  __u64 tboot_addr;
  struct ist_info ist_info;
  __u64 acpi_rsdp_addr;
  __u8 _pad3[8];
  __u8 hd0_info[16];
  __u8 hd1_info[16];
  struct sys_desc_table sys_desc_table;
  struct olpc_ofw_header olpc_ofw_header;
  __u32 ext_ramdisk_image;
  __u32 ext_ramdisk_size;
  __u32 ext_cmd_line_ptr;
  __u8 _pad4[112];
  __u32 cc_blob_address;
  struct edid_info edid_info;
  struct efi_info efi_info;
  __u32 alt_mem_k;
  __u32 scratch;
  __u8 e820_entries;
  __u8 eddbuf_entries;
  __u8 edd_mbr_sig_buf_entries;
  __u8 kbd_status;
  __u8 secure_boot;
  __u8 _pad5[2];
  __u8 sentinel;
  __u8 _pad6[1];
  struct setup_header hdr;
  __u8 _pad7[0x290 - 0x1f1 - sizeof(struct setup_header)];
  __u32 edd_mbr_sig_buffer[EDD_MBR_SIG_MAX];
  struct boot_e820_entry e820_table[E820_MAX_ENTRIES_ZEROPAGE];
  __u8 _pad8[48];
  struct edd_info eddbuf[EDDMAXNR];
  __u8 _pad9[276];
} __attribute__((packed));
enum x86_hardware_subarch {
  X86_SUBARCH_PC = 0,
  X86_SUBARCH_LGUEST,
  X86_SUBARCH_XEN,
  X86_SUBARCH_INTEL_MID,
  X86_SUBARCH_CE4100,
  X86_NR_SUBARCHS,
};
#endif
#endif
