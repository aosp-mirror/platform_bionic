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
#ifndef _MSM_MDP_H_
#define _MSM_MDP_H_
#include <linux/types.h>
#include <linux/fb.h>
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define MSMFB_IOCTL_MAGIC 'm'
#define MSMFB_GRP_DISP _IOW(MSMFB_IOCTL_MAGIC, 1, unsigned int)
#define MSMFB_BLIT _IOW(MSMFB_IOCTL_MAGIC, 2, unsigned int)
#define MSMFB_SUSPEND_SW_REFRESHER _IOW(MSMFB_IOCTL_MAGIC, 128, unsigned int)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define MSMFB_RESUME_SW_REFRESHER _IOW(MSMFB_IOCTL_MAGIC, 129, unsigned int)
#define MSMFB_CURSOR _IOW(MSMFB_IOCTL_MAGIC, 130, struct fb_cursor)
#define MSMFB_SET_LUT _IOW(MSMFB_IOCTL_MAGIC, 131, struct fb_cmap)
#define MSMFB_HISTOGRAM _IOWR(MSMFB_IOCTL_MAGIC, 132, struct mdp_histogram_data)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define MSMFB_GET_CCS_MATRIX _IOWR(MSMFB_IOCTL_MAGIC, 133, struct mdp_ccs)
#define MSMFB_SET_CCS_MATRIX _IOW(MSMFB_IOCTL_MAGIC, 134, struct mdp_ccs)
#define MSMFB_OVERLAY_SET _IOWR(MSMFB_IOCTL_MAGIC, 135,   struct mdp_overlay)
#define MSMFB_OVERLAY_UNSET _IOW(MSMFB_IOCTL_MAGIC, 136, unsigned int)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define MSMFB_OVERLAY_PLAY _IOW(MSMFB_IOCTL_MAGIC, 137,   struct msmfb_overlay_data)
#define MSMFB_OVERLAY_QUEUE MSMFB_OVERLAY_PLAY
#define MSMFB_GET_PAGE_PROTECTION _IOR(MSMFB_IOCTL_MAGIC, 138,   struct mdp_page_protection)
#define MSMFB_SET_PAGE_PROTECTION _IOW(MSMFB_IOCTL_MAGIC, 139,   struct mdp_page_protection)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define MSMFB_OVERLAY_GET _IOR(MSMFB_IOCTL_MAGIC, 140,   struct mdp_overlay)
#define MSMFB_OVERLAY_PLAY_ENABLE _IOW(MSMFB_IOCTL_MAGIC, 141, unsigned int)
#define MSMFB_OVERLAY_BLT _IOWR(MSMFB_IOCTL_MAGIC, 142,   struct msmfb_overlay_blt)
#define MSMFB_OVERLAY_BLT_OFFSET _IOW(MSMFB_IOCTL_MAGIC, 143, unsigned int)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define MSMFB_HISTOGRAM_START _IOR(MSMFB_IOCTL_MAGIC, 144,   struct mdp_histogram_start_req)
#define MSMFB_HISTOGRAM_STOP _IOR(MSMFB_IOCTL_MAGIC, 145, unsigned int)
#define MSMFB_NOTIFY_UPDATE _IOW(MSMFB_IOCTL_MAGIC, 146, unsigned int)
#define MSMFB_OVERLAY_3D _IOWR(MSMFB_IOCTL_MAGIC, 147,   struct msmfb_overlay_3d)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define MSMFB_MIXER_INFO _IOWR(MSMFB_IOCTL_MAGIC, 148,   struct msmfb_mixer_info_req)
#define MSMFB_OVERLAY_PLAY_WAIT _IOWR(MSMFB_IOCTL_MAGIC, 149,   struct msmfb_overlay_data)
#define MSMFB_WRITEBACK_INIT _IO(MSMFB_IOCTL_MAGIC, 150)
#define MSMFB_WRITEBACK_START _IO(MSMFB_IOCTL_MAGIC, 151)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define MSMFB_WRITEBACK_STOP _IO(MSMFB_IOCTL_MAGIC, 152)
#define MSMFB_WRITEBACK_QUEUE_BUFFER _IOW(MSMFB_IOCTL_MAGIC, 153,   struct msmfb_data)
#define MSMFB_WRITEBACK_DEQUEUE_BUFFER _IOW(MSMFB_IOCTL_MAGIC, 154,   struct msmfb_data)
#define MSMFB_WRITEBACK_TERMINATE _IO(MSMFB_IOCTL_MAGIC, 155)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define MSMFB_MDP_PP _IOWR(MSMFB_IOCTL_MAGIC, 156, struct msmfb_mdp_pp)
#define MSMFB_OVERLAY_VSYNC_CTRL _IOW(MSMFB_IOCTL_MAGIC, 160, unsigned int)
#define MSMFB_VSYNC_CTRL _IOW(MSMFB_IOCTL_MAGIC, 161, unsigned int)
#define MSMFB_BUFFER_SYNC _IOW(MSMFB_IOCTL_MAGIC, 162, struct mdp_buf_sync)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define MSMFB_OVERLAY_COMMIT _IOW(MSMFB_IOCTL_MAGIC, 163, unsigned int)
#define MSMFB_DISPLAY_COMMIT _IOW(MSMFB_IOCTL_MAGIC, 164,   struct mdp_display_commit)
#define FB_TYPE_3D_PANEL 0x10101010
#define MDP_IMGTYPE2_START 0x10000
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define MSMFB_DRIVER_VERSION 0xF9E8D701
enum {
 NOTIFY_UPDATE_START,
 NOTIFY_UPDATE_STOP,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
enum {
 MDP_RGB_565,
 MDP_XRGB_8888,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 MDP_Y_CBCR_H2V2,
 MDP_Y_CBCR_H2V2_ADRENO,
 MDP_ARGB_8888,
 MDP_RGB_888,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 MDP_Y_CRCB_H2V2,
 MDP_YCRYCB_H2V1,
 MDP_Y_CRCB_H2V1,
 MDP_Y_CBCR_H2V1,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 MDP_Y_CRCB_H1V2,
 MDP_Y_CBCR_H1V2,
 MDP_RGBA_8888,
 MDP_BGRA_8888,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 MDP_RGBX_8888,
 MDP_Y_CRCB_H2V2_TILE,
 MDP_Y_CBCR_H2V2_TILE,
 MDP_Y_CR_CB_H2V2,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 MDP_Y_CR_CB_GH2V2,
 MDP_Y_CB_CR_H2V2,
 MDP_Y_CRCB_H1V1,
 MDP_Y_CBCR_H1V1,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 MDP_YCRCB_H1V1,
 MDP_YCBCR_H1V1,
 MDP_BGR_565,
 MDP_IMGTYPE_LIMIT,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 MDP_RGB_BORDERFILL,
 MDP_FB_FORMAT = MDP_IMGTYPE2_START,
 MDP_IMGTYPE_LIMIT2
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
enum {
 PMEM_IMG,
 FB_IMG,
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
enum {
 HSIC_HUE = 0,
 HSIC_SAT,
 HSIC_INT,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 HSIC_CON,
 NUM_HSIC_PARAM,
};
#define MDSS_MDP_ROT_ONLY 0x80
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define MDSS_MDP_RIGHT_MIXER 0x100
#define MDP_ROT_NOP 0
#define MDP_FLIP_LR 0x1
#define MDP_FLIP_UD 0x2
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define MDP_ROT_90 0x4
#define MDP_ROT_180 (MDP_FLIP_UD|MDP_FLIP_LR)
#define MDP_ROT_270 (MDP_ROT_90|MDP_FLIP_UD|MDP_FLIP_LR)
#define MDP_DITHER 0x8
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define MDP_BLUR 0x10
#define MDP_BLEND_FG_PREMULT 0x20000
#define MDP_DEINTERLACE 0x80000000
#define MDP_SHARPENING 0x40000000
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define MDP_NO_DMA_BARRIER_START 0x20000000
#define MDP_NO_DMA_BARRIER_END 0x10000000
#define MDP_NO_BLIT 0x08000000
#define MDP_BLIT_WITH_DMA_BARRIERS 0x000
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define MDP_BLIT_WITH_NO_DMA_BARRIERS   (MDP_NO_DMA_BARRIER_START | MDP_NO_DMA_BARRIER_END)
#define MDP_BLIT_SRC_GEM 0x04000000
#define MDP_BLIT_DST_GEM 0x02000000
#define MDP_BLIT_NON_CACHED 0x01000000
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define MDP_OV_PIPE_SHARE 0x00800000
#define MDP_DEINTERLACE_ODD 0x00400000
#define MDP_OV_PLAY_NOWAIT 0x00200000
#define MDP_SOURCE_ROTATED_90 0x00100000
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define MDP_OVERLAY_PP_CFG_EN 0x00080000
#define MDP_BACKEND_COMPOSITION 0x00040000
#define MDP_BORDERFILL_SUPPORTED 0x00010000
#define MDP_SECURE_OVERLAY_SESSION 0x00008000
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define MDP_MEMORY_ID_TYPE_FB 0x00001000
#define MDP_TRANSP_NOP 0xffffffff
#define MDP_ALPHA_NOP 0xff
#define MDP_FB_PAGE_PROTECTION_NONCACHED (0)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define MDP_FB_PAGE_PROTECTION_WRITECOMBINE (1)
#define MDP_FB_PAGE_PROTECTION_WRITETHROUGHCACHE (2)
#define MDP_FB_PAGE_PROTECTION_WRITEBACKCACHE (3)
#define MDP_FB_PAGE_PROTECTION_WRITEBACKWACACHE (4)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define MDP_FB_PAGE_PROTECTION_INVALID (5)
#define MDP_NUM_FB_PAGE_PROTECTION_VALUES (5)
struct mdp_rect {
 uint32_t x;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 uint32_t y;
 uint32_t w;
 uint32_t h;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct mdp_img {
 uint32_t width;
 uint32_t height;
 uint32_t format;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 uint32_t offset;
 int memory_id;
 uint32_t priv;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define MDP_CCS_RGB2YUV 0
#define MDP_CCS_YUV2RGB 1
#define MDP_CCS_SIZE 9
#define MDP_BV_SIZE 3
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct mdp_ccs {
 int direction;
 uint16_t ccs[MDP_CCS_SIZE];
 uint16_t bv[MDP_BV_SIZE];
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct mdp_csc {
 int id;
 uint32_t csc_mv[9];
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 uint32_t csc_pre_bv[3];
 uint32_t csc_post_bv[3];
 uint32_t csc_pre_lv[6];
 uint32_t csc_post_lv[6];
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
#define MDP_BLIT_REQ_VERSION 2
struct mdp_blit_req {
 struct mdp_img src;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct mdp_img dst;
 struct mdp_rect src_rect;
 struct mdp_rect dst_rect;
 uint32_t alpha;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 uint32_t transp_mask;
 uint32_t flags;
 int sharpening_strength;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct mdp_blit_req_list {
 uint32_t count;
 struct mdp_blit_req req[];
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define MSMFB_DATA_VERSION 2
struct msmfb_data {
 uint32_t offset;
 int memory_id;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 int id;
 uint32_t flags;
 uint32_t priv;
 uint32_t iova;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
#define MSMFB_NEW_REQUEST -1
struct msmfb_overlay_data {
 uint32_t id;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct msmfb_data data;
 uint32_t version_key;
 struct msmfb_data plane1_data;
 struct msmfb_data plane2_data;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct msmfb_data dst_data;
};
struct msmfb_img {
 uint32_t width;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 uint32_t height;
 uint32_t format;
};
#define MSMFB_WRITEBACK_DEQUEUE_BLOCKING 0x1
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct msmfb_writeback_data {
 struct msmfb_data buf_info;
 struct msmfb_img img;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define MDP_PP_OPS_READ 0x2
#define MDP_PP_OPS_WRITE 0x4
struct mdp_qseed_cfg {
 uint32_t table_num;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 uint32_t ops;
 uint32_t len;
 uint32_t *data;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct mdp_qseed_cfg_data {
 uint32_t block;
 struct mdp_qseed_cfg qseed_data;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define MDP_OVERLAY_PP_CSC_CFG 0x1
#define MDP_OVERLAY_PP_QSEED_CFG 0x2
#define MDP_CSC_FLAG_ENABLE 0x1
#define MDP_CSC_FLAG_YUV_IN 0x2
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define MDP_CSC_FLAG_YUV_OUT 0x4
struct mdp_csc_cfg {
 uint32_t flags;
 uint32_t csc_mv[9];
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 uint32_t csc_pre_bv[3];
 uint32_t csc_post_bv[3];
 uint32_t csc_pre_lv[6];
 uint32_t csc_post_lv[6];
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct mdp_csc_cfg_data {
 uint32_t block;
 struct mdp_csc_cfg csc_data;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct mdp_overlay_pp_params {
 uint32_t config_ops;
 struct mdp_csc_cfg csc_cfg;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct mdp_qseed_cfg qseed_cfg[2];
};
struct mdp_overlay {
 struct msmfb_img src;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct mdp_rect src_rect;
 struct mdp_rect dst_rect;
 uint32_t z_order;
 uint32_t is_fg;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 uint32_t alpha;
 uint32_t transp_mask;
 uint32_t flags;
 uint32_t id;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 uint32_t user_data[8];
 struct mdp_overlay_pp_params overlay_pp_cfg;
};
struct msmfb_overlay_3d {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 uint32_t is_3d;
 uint32_t width;
 uint32_t height;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct msmfb_overlay_blt {
 uint32_t enable;
 uint32_t offset;
 uint32_t width;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 uint32_t height;
 uint32_t bpp;
};
struct mdp_histogram {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 uint32_t frame_cnt;
 uint32_t bin_cnt;
 uint32_t *r;
 uint32_t *g;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 uint32_t *b;
};
enum {
 MDP_BLOCK_RESERVED = 0,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 MDP_BLOCK_OVERLAY_0,
 MDP_BLOCK_OVERLAY_1,
 MDP_BLOCK_VG_1,
 MDP_BLOCK_VG_2,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 MDP_BLOCK_RGB_1,
 MDP_BLOCK_RGB_2,
 MDP_BLOCK_DMA_P,
 MDP_BLOCK_DMA_S,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 MDP_BLOCK_DMA_E,
 MDP_BLOCK_OVERLAY_2,
 MDP_BLOCK_MAX,
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct mdp_histogram_start_req {
 uint32_t block;
 uint8_t frame_cnt;
 uint8_t bit_mask;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 uint8_t num_bins;
};
struct mdp_histogram_data {
 uint32_t block;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 uint8_t bin_cnt;
 uint32_t *c0;
 uint32_t *c1;
 uint32_t *c2;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 uint32_t *extra_info;
};
struct mdp_pcc_coeff {
 uint32_t c, r, g, b, rr, gg, bb, rg, gb, rb, rgb_0, rgb_1;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct mdp_pcc_cfg_data {
 uint32_t block;
 uint32_t ops;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct mdp_pcc_coeff r, g, b;
};
enum {
 mdp_lut_igc,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 mdp_lut_pgc,
 mdp_lut_hist,
 mdp_lut_max,
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct mdp_igc_lut_data {
 uint32_t block;
 uint32_t len, ops;
 uint32_t *c0_c1_data;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 uint32_t *c2_data;
};
struct mdp_ar_gc_lut_data {
 uint32_t x_start;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 uint32_t slope;
 uint32_t offset;
};
struct mdp_pgc_lut_data {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 uint32_t block;
 uint32_t flags;
 uint8_t num_r_stages;
 uint8_t num_g_stages;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 uint8_t num_b_stages;
 struct mdp_ar_gc_lut_data *r_data;
 struct mdp_ar_gc_lut_data *g_data;
 struct mdp_ar_gc_lut_data *b_data;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct mdp_hist_lut_data {
 uint32_t block;
 uint32_t ops;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 uint32_t len;
 uint32_t *data;
};
struct mdp_lut_cfg_data {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 uint32_t lut_type;
 union {
 struct mdp_igc_lut_data igc_lut_data;
 struct mdp_pgc_lut_data pgc_lut_data;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct mdp_hist_lut_data hist_lut_data;
 } data;
};
struct mdp_bl_scale_data {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 uint32_t min_lvl;
 uint32_t scale;
};
enum {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 mdp_op_pcc_cfg,
 mdp_op_csc_cfg,
 mdp_op_lut_cfg,
 mdp_op_qseed_cfg,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 mdp_bl_scale_cfg,
 mdp_op_max,
};
struct msmfb_mdp_pp {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 uint32_t op;
 union {
 struct mdp_pcc_cfg_data pcc_cfg_data;
 struct mdp_csc_cfg_data csc_cfg_data;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct mdp_lut_cfg_data lut_cfg_data;
 struct mdp_qseed_cfg_data qseed_cfg_data;
 struct mdp_bl_scale_data bl_scale_data;
 } data;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
#define MDP_MAX_FENCE_FD 10
#define MDP_BUF_SYNC_FLAG_WAIT 1
struct mdp_buf_sync {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 uint32_t flags;
 uint32_t acq_fen_fd_cnt;
 int *acq_fen_fd;
 int *rel_fen_fd;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct mdp_buf_fence {
 uint32_t flags;
 uint32_t acq_fen_fd_cnt;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 int acq_fen_fd[MDP_MAX_FENCE_FD];
 int rel_fen_fd[MDP_MAX_FENCE_FD];
};
struct mdp_display_commit {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 uint32_t flags;
 uint32_t wait_for_finish;
 struct fb_var_screeninfo var;
 struct mdp_buf_fence buf_fence;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct mdp_page_protection {
 uint32_t page_protection;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct mdp_mixer_info {
 int pndx;
 int pnum;
 int ptype;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 int mixer_num;
 int z_order;
};
#define MAX_PIPE_PER_MIXER 4
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct msmfb_mixer_info_req {
 int mixer_num;
 int cnt;
 struct mdp_mixer_info info[MAX_PIPE_PER_MIXER];
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
enum {
 DISPLAY_SUBSYSTEM_ID,
 ROTATOR_SUBSYSTEM_ID,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
#endif

