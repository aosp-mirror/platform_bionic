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
#ifndef _DRM_MODE_H
#define _DRM_MODE_H
#include <linux/types.h>
#define DRM_DISPLAY_INFO_LEN 32
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define DRM_CONNECTOR_NAME_LEN 32
#define DRM_DISPLAY_MODE_LEN 32
#define DRM_PROP_NAME_LEN 32
#define DRM_MODE_TYPE_BUILTIN (1<<0)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define DRM_MODE_TYPE_CLOCK_C ((1<<1) | DRM_MODE_TYPE_BUILTIN)
#define DRM_MODE_TYPE_CRTC_C ((1<<2) | DRM_MODE_TYPE_BUILTIN)
#define DRM_MODE_TYPE_PREFERRED (1<<3)
#define DRM_MODE_TYPE_DEFAULT (1<<4)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define DRM_MODE_TYPE_USERDEF (1<<5)
#define DRM_MODE_TYPE_DRIVER (1<<6)
#define DRM_MODE_FLAG_PHSYNC (1<<0)
#define DRM_MODE_FLAG_NHSYNC (1<<1)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define DRM_MODE_FLAG_PVSYNC (1<<2)
#define DRM_MODE_FLAG_NVSYNC (1<<3)
#define DRM_MODE_FLAG_INTERLACE (1<<4)
#define DRM_MODE_FLAG_DBLSCAN (1<<5)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define DRM_MODE_FLAG_CSYNC (1<<6)
#define DRM_MODE_FLAG_PCSYNC (1<<7)
#define DRM_MODE_FLAG_NCSYNC (1<<8)
#define DRM_MODE_FLAG_HSKEW (1<<9)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define DRM_MODE_FLAG_BCAST (1<<10)
#define DRM_MODE_FLAG_PIXMUX (1<<11)
#define DRM_MODE_FLAG_DBLCLK (1<<12)
#define DRM_MODE_FLAG_CLKDIV2 (1<<13)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define DRM_MODE_DPMS_ON 0
#define DRM_MODE_DPMS_STANDBY 1
#define DRM_MODE_DPMS_SUSPEND 2
#define DRM_MODE_DPMS_OFF 3
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define DRM_MODE_SCALE_NONE 0
#define DRM_MODE_SCALE_FULLSCREEN 1
#define DRM_MODE_SCALE_CENTER 2
#define DRM_MODE_SCALE_ASPECT 3
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define DRM_MODE_DITHERING_OFF 0
#define DRM_MODE_DITHERING_ON 1
#define DRM_MODE_DITHERING_AUTO 2
#define DRM_MODE_DIRTY_OFF 0
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define DRM_MODE_DIRTY_ON 1
#define DRM_MODE_DIRTY_ANNOTATE 2
struct drm_mode_modeinfo {
 __u32 clock;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u16 hdisplay, hsync_start, hsync_end, htotal, hskew;
 __u16 vdisplay, vsync_start, vsync_end, vtotal, vscan;
 __u32 vrefresh;
 __u32 flags;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 type;
 char name[DRM_DISPLAY_MODE_LEN];
};
struct drm_mode_card_res {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u64 fb_id_ptr;
 __u64 crtc_id_ptr;
 __u64 connector_id_ptr;
 __u64 encoder_id_ptr;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 count_fbs;
 __u32 count_crtcs;
 __u32 count_connectors;
 __u32 count_encoders;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 min_width, max_width;
 __u32 min_height, max_height;
};
struct drm_mode_crtc {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u64 set_connectors_ptr;
 __u32 count_connectors;
 __u32 crtc_id;
 __u32 fb_id;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 x, y;
 __u32 gamma_size;
 __u32 mode_valid;
 struct drm_mode_modeinfo mode;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
#define DRM_MODE_PRESENT_TOP_FIELD (1<<0)
#define DRM_MODE_PRESENT_BOTTOM_FIELD (1<<1)
struct drm_mode_set_plane {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 plane_id;
 __u32 crtc_id;
 __u32 fb_id;
 __u32 flags;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __s32 crtc_x, crtc_y;
 __u32 crtc_w, crtc_h;
 __u32 src_x, src_y;
 __u32 src_h, src_w;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct drm_mode_get_plane {
 __u32 plane_id;
 __u32 crtc_id;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 fb_id;
 __u32 possible_crtcs;
 __u32 gamma_size;
 __u32 count_format_types;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u64 format_type_ptr;
};
struct drm_mode_get_plane_res {
 __u64 plane_id_ptr;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 count_planes;
};
#define DRM_MODE_ENCODER_NONE 0
#define DRM_MODE_ENCODER_DAC 1
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define DRM_MODE_ENCODER_TMDS 2
#define DRM_MODE_ENCODER_LVDS 3
#define DRM_MODE_ENCODER_TVDAC 4
#define DRM_MODE_ENCODER_VIRTUAL 5
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct drm_mode_get_encoder {
 __u32 encoder_id;
 __u32 encoder_type;
 __u32 crtc_id;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 possible_crtcs;
 __u32 possible_clones;
};
#define DRM_MODE_SUBCONNECTOR_Automatic 0
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define DRM_MODE_SUBCONNECTOR_Unknown 0
#define DRM_MODE_SUBCONNECTOR_DVID 3
#define DRM_MODE_SUBCONNECTOR_DVIA 4
#define DRM_MODE_SUBCONNECTOR_Composite 5
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define DRM_MODE_SUBCONNECTOR_SVIDEO 6
#define DRM_MODE_SUBCONNECTOR_Component 8
#define DRM_MODE_SUBCONNECTOR_SCART 9
#define DRM_MODE_CONNECTOR_Unknown 0
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define DRM_MODE_CONNECTOR_VGA 1
#define DRM_MODE_CONNECTOR_DVII 2
#define DRM_MODE_CONNECTOR_DVID 3
#define DRM_MODE_CONNECTOR_DVIA 4
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define DRM_MODE_CONNECTOR_Composite 5
#define DRM_MODE_CONNECTOR_SVIDEO 6
#define DRM_MODE_CONNECTOR_LVDS 7
#define DRM_MODE_CONNECTOR_Component 8
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define DRM_MODE_CONNECTOR_9PinDIN 9
#define DRM_MODE_CONNECTOR_DisplayPort 10
#define DRM_MODE_CONNECTOR_HDMIA 11
#define DRM_MODE_CONNECTOR_HDMIB 12
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define DRM_MODE_CONNECTOR_TV 13
#define DRM_MODE_CONNECTOR_eDP 14
#define DRM_MODE_CONNECTOR_VIRTUAL 15
struct drm_mode_get_connector {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u64 encoders_ptr;
 __u64 modes_ptr;
 __u64 props_ptr;
 __u64 prop_values_ptr;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 count_modes;
 __u32 count_props;
 __u32 count_encoders;
 __u32 encoder_id;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 connector_id;
 __u32 connector_type;
 __u32 connector_type_id;
 __u32 connection;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 mm_width, mm_height;
 __u32 subpixel;
};
#define DRM_MODE_PROP_PENDING (1<<0)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define DRM_MODE_PROP_RANGE (1<<1)
#define DRM_MODE_PROP_IMMUTABLE (1<<2)
#define DRM_MODE_PROP_ENUM (1<<3)
#define DRM_MODE_PROP_BLOB (1<<4)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define DRM_MODE_PROP_BITMASK (1<<5)
struct drm_mode_property_enum {
 __u64 value;
 char name[DRM_PROP_NAME_LEN];
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct drm_mode_get_property {
 __u64 values_ptr;
 __u64 enum_blob_ptr;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 prop_id;
 __u32 flags;
 char name[DRM_PROP_NAME_LEN];
 __u32 count_values;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 count_enum_blobs;
};
struct drm_mode_connector_set_property {
 __u64 value;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 prop_id;
 __u32 connector_id;
};
struct drm_mode_obj_get_properties {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u64 props_ptr;
 __u64 prop_values_ptr;
 __u32 count_props;
 __u32 obj_id;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 obj_type;
};
struct drm_mode_obj_set_property {
 __u64 value;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 prop_id;
 __u32 obj_id;
 __u32 obj_type;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct drm_mode_get_blob {
 __u32 blob_id;
 __u32 length;
 __u64 data;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct drm_mode_fb_cmd {
 __u32 fb_id;
 __u32 width, height;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 pitch;
 __u32 bpp;
 __u32 depth;
 __u32 handle;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
#define DRM_MODE_FB_INTERLACED (1<<0)
struct drm_mode_fb_cmd2 {
 __u32 fb_id;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 width, height;
 __u32 pixel_format;
 __u32 flags;
 __u32 handles[4];
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 pitches[4];
 __u32 offsets[4];
};
#define DRM_MODE_FB_DIRTY_ANNOTATE_COPY 0x01
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define DRM_MODE_FB_DIRTY_ANNOTATE_FILL 0x02
#define DRM_MODE_FB_DIRTY_FLAGS 0x03
#define DRM_MODE_FB_DIRTY_MAX_CLIPS 256
struct drm_mode_fb_dirty_cmd {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 fb_id;
 __u32 flags;
 __u32 color;
 __u32 num_clips;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u64 clips_ptr;
};
struct drm_mode_mode_cmd {
 __u32 connector_id;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct drm_mode_modeinfo mode;
};
#define DRM_MODE_CURSOR_BO 0x01
#define DRM_MODE_CURSOR_MOVE 0x02
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define DRM_MODE_CURSOR_FLAGS 0x03
struct drm_mode_cursor {
 __u32 flags;
 __u32 crtc_id;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __s32 x;
 __s32 y;
 __u32 width;
 __u32 height;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 handle;
};
struct drm_mode_crtc_lut {
 __u32 crtc_id;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 gamma_size;
 __u64 red;
 __u64 green;
 __u64 blue;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
#define DRM_MODE_PAGE_FLIP_EVENT 0x01
#define DRM_MODE_PAGE_FLIP_FLAGS DRM_MODE_PAGE_FLIP_EVENT
struct drm_mode_crtc_page_flip {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 crtc_id;
 __u32 fb_id;
 __u32 flags;
 __u32 reserved;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u64 user_data;
};
struct drm_mode_create_dumb {
 uint32_t height;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 uint32_t width;
 uint32_t bpp;
 uint32_t flags;
 uint32_t handle;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 uint32_t pitch;
 uint64_t size;
};
struct drm_mode_map_dumb {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 handle;
 __u32 pad;
 __u64 offset;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct drm_mode_destroy_dumb {
 uint32_t handle;
};
#endif
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
