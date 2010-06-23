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
#ifndef _MSM_VIDC_ENC_H_
#define _MSM_VIDC_ENC_H_

#include <linux/types.h>
#include <linux/ioctl.h>

#define VEN_S_BASE 0x00000000
#define VEN_S_SUCCESS (VEN_S_BASE) 
#define VEN_S_EFAIL (VEN_S_BASE+1) 
#define VEN_S_EFATAL (VEN_S_BASE+2) 
#define VEN_S_EBADPARAM (VEN_S_BASE+3) 

#define VEN_S_EINVALSTATE (VEN_S_BASE+4)
#define VEN_S_ENOSWRES (VEN_S_BASE+5) 
#define VEN_S_ENOHWRES (VEN_S_BASE+6) 
#define VEN_S_EBUFFREQ (VEN_S_BASE+7) 
#define VEN_S_EINVALCMD (VEN_S_BASE+8) 
#define VEN_S_ETIMEOUT (VEN_S_BASE+9) 

#define VEN_S_ENOREATMPT (VEN_S_BASE+10)
#define VEN_S_ENOPREREQ (VEN_S_BASE+11) 
#define VEN_S_ECMDQFULL (VEN_S_BASE+12) 
#define VEN_S_ENOTSUPP (VEN_S_BASE+13) 
#define VEN_S_ENOTIMPL (VEN_S_BASE+14) 
#define VEN_S_ENOTPMEM (VEN_S_BASE+15) 
#define VEN_S_EFLUSHED (VEN_S_BASE+16) 
#define VEN_S_EINSUFBUF (VEN_S_BASE+17) 
#define VEN_S_ESAMESTATE (VEN_S_BASE+18)
#define VEN_S_EINVALTRANS (VEN_S_BASE+19)

#define VEN_INTF_VER 1

#define VEN_MSG_INDICATION 0
#define VEN_MSG_INPUT_BUFFER_DONE 1
#define VEN_MSG_OUTPUT_BUFFER_DONE 2
#define VEN_MSG_NEED_OUTPUT_BUFFER 3
#define VEN_MSG_FLUSH_INPUT_DONE 4
#define VEN_MSG_FLUSH_OUPUT_DONE 5
#define VEN_MSG_START 6
#define VEN_MSG_STOP 7
#define VEN_MSG_PAUSE 8
#define VEN_MSG_RESUME 9
#define VEN_MSG_STOP_READING_MSG 10

#define VEN_BUFFLAG_EOS 0x00000001
#define VEN_BUFFLAG_ENDOFFRAME 0x00000010
#define VEN_BUFFLAG_SYNCFRAME 0x00000020
#define VEN_BUFFLAG_EXTRADATA 0x00000040
#define VEN_BUFFLAG_CODECCONFIG 0x00000080

#define VEN_FRAME_TYPE_I 1 
#define VEN_FRAME_TYPE_P 2 
#define VEN_FRAME_TYPE_B 3 

#define VEN_CODEC_MPEG4 1 
#define VEN_CODEC_H264 2 
#define VEN_CODEC_H263 3 

#define VEN_PROFILE_MPEG4_SP 1 
#define VEN_PROFILE_MPEG4_ASP 2 
#define VEN_PROFILE_H264_BASELINE 3 
#define VEN_PROFILE_H264_MAIN 4 
#define VEN_PROFILE_H264_HIGH 5 
#define VEN_PROFILE_H263_BASELINE 6 

#define VEN_LEVEL_MPEG4_0 0x1 
#define VEN_LEVEL_MPEG4_1 0x2 
#define VEN_LEVEL_MPEG4_2 0x3 
#define VEN_LEVEL_MPEG4_3 0x4 
#define VEN_LEVEL_MPEG4_4 0x5 
#define VEN_LEVEL_MPEG4_5 0x6 
#define VEN_LEVEL_MPEG4_3b 0x7 
#define VEN_LEVEL_MPEG4_6 0x8 

#define VEN_LEVEL_H264_1 0x9 
#define VEN_LEVEL_H264_1b 0xA 
#define VEN_LEVEL_H264_1p1 0xB 
#define VEN_LEVEL_H264_1p2 0xC 
#define VEN_LEVEL_H264_1p3 0xD 
#define VEN_LEVEL_H264_2 0xE 
#define VEN_LEVEL_H264_2p1 0xF 
#define VEN_LEVEL_H264_2p2 0x10 
#define VEN_LEVEL_H264_3 0x11 
#define VEN_LEVEL_H264_3p1 0x12 

#define VEN_LEVEL_H263_10 0x13 
#define VEN_LEVEL_H263_20 0x14 
#define VEN_LEVEL_H263_30 0x15 
#define VEN_LEVEL_H263_40 0x16 
#define VEN_LEVEL_H263_45 0x17 
#define VEN_LEVEL_H263_50 0x18 
#define VEN_LEVEL_H263_60 0x19 
#define VEN_LEVEL_H263_70 0x1A 

#define VEN_ENTROPY_MODEL_CAVLC 1
#define VEN_ENTROPY_MODEL_CABAC 2

#define VEN_CABAC_MODEL_0 1 
#define VEN_CABAC_MODEL_1 2 
#define VEN_CABAC_MODEL_2 3 

#define VEN_DB_DISABLE 1 
#define VEN_DB_ALL_BLKG_BNDRY 2 
#define VEN_DB_SKIP_SLICE_BNDRY 3 

#define VEN_MSLICE_OFF 1
#define VEN_MSLICE_CNT_MB 2  
#define VEN_MSLICE_CNT_BYTE 3  
#define VEN_MSLICE_GOB 4  

#define VEN_RC_OFF 1
#define VEN_RC_VBR_VFR 2
#define VEN_RC_VBR_CFR 3
#define VEN_RC_CBR_VFR 4

#define VEN_FLUSH_INPUT 1
#define VEN_FLUSH_OUTPUT 2
#define VEN_FLUSH_ALL 3

#define VEN_INPUTFMT_NV12 1 
#define VEN_INPUTFMT_NV21 2 

#define VEN_ROTATION_0 1 
#define VEN_ROTATION_90 2 
#define VEN_ROTATION_180 3 
#define VEN_ROTATION_270 4 

#define VEN_TIMEOUT_INFINITE 0xffffffff

#define VEN_IR_OFF 1
#define VEN_IR_CYCLIC 2
#define VEN_IR_RANDOM 3

#define VEN_IOCTLBASE_NENC 0x800

#define VEN_IOCTLBASE_ENC 0x850

struct venc_ioctl_msg{
 void *inputparam;
 void *outputparam;
};

#define VEN_IOCTL_SET_INTF_VERSION   _IOW(VEN_IOCTLBASE_NENC, 0, struct venc_ioctl_msg)

#define VEN_IOCTL_CMD_READ_NEXT_MSG   _IOWR(VEN_IOCTLBASE_NENC, 1, struct venc_ioctl_msg)

#define VEN_IOCTL_CMD_STOP_READ_MSG _IO(VEN_IOCTLBASE_NENC, 2)

#define VEN_IOCTL_SET_INPUT_BUFFER_REQ   _IOW(VEN_IOCTLBASE_NENC, 3, struct venc_ioctl_msg)
#define VEN_IOCTL_GET_INPUT_BUFFER_REQ   _IOR(VEN_IOCTLBASE_NENC, 4, struct venc_ioctl_msg)

#define VEN_IOCTL_CMD_ALLOC_INPUT_BUFFER   _IOW(VEN_IOCTLBASE_NENC, 5, struct venc_ioctl_msg)

#define VEN_IOCTL_SET_INPUT_BUFFER   _IOW(VEN_IOCTLBASE_NENC, 6, struct venc_ioctl_msg)

#define VEN_IOCTL_CMD_FREE_INPUT_BUFFER   _IOW(VEN_IOCTLBASE_NENC, 7, struct venc_ioctl_msg)

#define VEN_IOCTL_SET_OUTPUT_BUFFER_REQ   _IOW(VEN_IOCTLBASE_NENC, 8, struct venc_ioctl_msg)
#define VEN_IOCTL_GET_OUTPUT_BUFFER_REQ   _IOR(VEN_IOCTLBASE_NENC, 9, struct venc_ioctl_msg)

#define VEN_IOCTL_CMD_ALLOC_OUTPUT_BUFFER   _IOW(VEN_IOCTLBASE_NENC, 10, struct venc_ioctl_msg)

#define VEN_IOCTL_SET_OUTPUT_BUFFER   _IOW(VEN_IOCTLBASE_NENC, 11, struct venc_ioctl_msg)

#define VEN_IOCTL_CMD_FREE_OUTPUT_BUFFER   _IOW(VEN_IOCTLBASE_NENC, 12, struct venc_ioctl_msg)

#define VEN_IOCTL_CMD_START _IO(VEN_IOCTLBASE_NENC, 13)

#define VEN_IOCTL_CMD_ENCODE_FRAME   _IOW(VEN_IOCTLBASE_NENC, 14, struct venc_ioctl_msg)

#define VEN_IOCTL_CMD_FILL_OUTPUT_BUFFER   _IOW(VEN_IOCTLBASE_NENC, 15, struct venc_ioctl_msg)

#define VEN_IOCTL_CMD_FLUSH   _IOW(VEN_IOCTLBASE_NENC, 16, struct venc_ioctl_msg)

#define VEN_IOCTL_CMD_PAUSE _IO(VEN_IOCTLBASE_NENC, 17)

#define VEN_IOCTL_CMD_RESUME _IO(VEN_IOCTLBASE_NENC, 18)

#define VEN_IOCTL_CMD_STOP _IO(VEN_IOCTLBASE_NENC, 19)

#define VEN_IOCTL_SET_BASE_CFG   _IOW(VEN_IOCTLBASE_ENC, 1, struct venc_ioctl_msg)
#define VEN_IOCTL_GET_BASE_CFG   _IOR(VEN_IOCTLBASE_ENC, 2, struct venc_ioctl_msg)

#define VEN_IOCTL_SET_LIVE_MODE   _IOW(VEN_IOCTLBASE_ENC, 3, struct venc_ioctl_msg)
#define VEN_IOCTL_GET_LIVE_MODE   _IOR(VEN_IOCTLBASE_ENC, 4, struct venc_ioctl_msg)

#define VEN_IOCTL_SET_CODEC_PROFILE   _IOW(VEN_IOCTLBASE_ENC, 5, struct venc_ioctl_msg)
#define VEN_IOCTL_GET_CODEC_PROFILE   _IOR(VEN_IOCTLBASE_ENC, 6, struct venc_ioctl_msg)

#define VEN_IOCTL_SET_PROFILE_LEVEL   _IOW(VEN_IOCTLBASE_ENC, 7, struct venc_ioctl_msg)

#define VEN_IOCTL_GET_PROFILE_LEVEL   _IOR(VEN_IOCTLBASE_ENC, 8, struct venc_ioctl_msg)

#define VEN_IOCTL_SET_SHORT_HDR   _IOW(VEN_IOCTLBASE_ENC, 9, struct venc_ioctl_msg)
#define VEN_IOCTL_GET_SHORT_HDR   _IOR(VEN_IOCTLBASE_ENC, 10, struct venc_ioctl_msg)

#define VEN_IOCTL_SET_SESSION_QP   _IOW(VEN_IOCTLBASE_ENC, 11, struct venc_ioctl_msg)
#define VEN_IOCTL_GET_SESSION_QP   _IOR(VEN_IOCTLBASE_ENC, 12, struct venc_ioctl_msg)

#define VEN_IOCTL_SET_INTRA_PERIOD   _IOW(VEN_IOCTLBASE_ENC, 13, struct venc_ioctl_msg)
#define VEN_IOCTL_GET_INTRA_PERIOD   _IOR(VEN_IOCTLBASE_ENC, 14, struct venc_ioctl_msg)

#define VEN_IOCTL_CMD_REQUEST_IFRAME _IO(VEN_IOCTLBASE_ENC, 15)

#define VEN_IOCTL_GET_CAPABILITY   _IOR(VEN_IOCTLBASE_ENC, 16, struct venc_ioctl_msg)

#define VEN_IOCTL_GET_SEQUENCE_HDR   _IOR(VEN_IOCTLBASE_ENC, 17, struct venc_ioctl_msg)

#define VEN_IOCTL_SET_ENTROPY_CFG   _IOW(VEN_IOCTLBASE_ENC, 18, struct venc_ioctl_msg)
#define VEN_IOCTL_GET_ENTROPY_CFG   _IOR(VEN_IOCTLBASE_ENC, 19, struct venc_ioctl_msg)

#define VEN_IOCTL_SET_DEBLOCKING_CFG   _IOW(VEN_IOCTLBASE_ENC, 20, struct venc_ioctl_msg)
#define VEN_IOCTL_GET_DEBLOCKING_CFG   _IOR(VEN_IOCTLBASE_ENC, 21, struct venc_ioctl_msg)

#define VEN_IOCTL_SET_INTRA_REFRESH   _IOW(VEN_IOCTLBASE_ENC, 22, struct venc_ioctl_msg)
#define VEN_IOCTL_GET_INTRA_REFRESH   _IOR(VEN_IOCTLBASE_ENC, 23, struct venc_ioctl_msg)

#define VEN_IOCTL_SET_MULTI_SLICE_CFG   _IOW(VEN_IOCTLBASE_ENC, 24, struct venc_ioctl_msg)
#define VEN_IOCTL_GET_MULTI_SLICE_CFG   _IOR(VEN_IOCTLBASE_ENC, 25, struct venc_ioctl_msg)

#define VEN_IOCTL_SET_RATE_CTRL_CFG   _IOW(VEN_IOCTLBASE_ENC, 26, struct venc_ioctl_msg)
#define VEN_IOCTL_GET_RATE_CTRL_CFG   _IOR(VEN_IOCTLBASE_ENC, 27, struct venc_ioctl_msg)

#define VEN_IOCTL_SET_VOP_TIMING_CFG   _IOW(VEN_IOCTLBASE_ENC, 28, struct venc_ioctl_msg)
#define VEN_IOCTL_GET_VOP_TIMING_CFG   _IOR(VEN_IOCTLBASE_ENC, 29, struct venc_ioctl_msg)

#define VEN_IOCTL_SET_FRAME_RATE   _IOW(VEN_IOCTLBASE_ENC, 30, struct venc_ioctl_msg)
#define VEN_IOCTL_GET_FRAME_RATE   _IOR(VEN_IOCTLBASE_ENC, 31, struct venc_ioctl_msg)

#define VEN_IOCTL_SET_TARGET_BITRATE   _IOW(VEN_IOCTLBASE_ENC, 32, struct venc_ioctl_msg)
#define VEN_IOCTL_GET_TARGET_BITRATE   _IOR(VEN_IOCTLBASE_ENC, 33, struct venc_ioctl_msg)

#define VEN_IOCTL_SET_ROTATION   _IOW(VEN_IOCTLBASE_ENC, 34, struct venc_ioctl_msg)
#define VEN_IOCTL_GET_ROTATION   _IOR(VEN_IOCTLBASE_ENC, 35, struct venc_ioctl_msg)

#define VEN_IOCTL_SET_HEC   _IOW(VEN_IOCTLBASE_ENC, 36, struct venc_ioctl_msg)
#define VEN_IOCTL_GET_HEC   _IOR(VEN_IOCTLBASE_ENC, 37, struct venc_ioctl_msg)

#define VEN_IOCTL_SET_DATA_PARTITION   _IOW(VEN_IOCTLBASE_ENC, 38, struct venc_ioctl_msg)
#define VEN_IOCTL_GET_DATA_PARTITION   _IOR(VEN_IOCTLBASE_ENC, 39, struct venc_ioctl_msg)

#define VEN_IOCTL_SET_RVLC   _IOW(VEN_IOCTLBASE_ENC, 40, struct venc_ioctl_msg)
#define VEN_IOCTL_GET_RVLC   _IOR(VEN_IOCTLBASE_ENC, 41, struct venc_ioctl_msg)

#define VEN_IOCTL_SET_AC_PREDICTION   _IOW(VEN_IOCTLBASE_ENC, 42, struct venc_ioctl_msg)
#define VEN_IOCTL_GET_AC_PREDICTION   _IOR(VEN_IOCTLBASE_ENC, 43, struct venc_ioctl_msg)

#define VEN_IOCTL_SET_QP_RANGE   _IOW(VEN_IOCTLBASE_ENC, 44, struct venc_ioctl_msg)
#define VEN_IOCTL_GET_QP_RANGE   _IOR(VEN_IOCTLBASE_ENC, 45, struct venc_ioctl_msg)

struct venc_switch{
 unsigned char status;
};

struct venc_allocatorproperty{
 unsigned long mincount;
 unsigned long maxcount;
 unsigned long actualcount;
 unsigned long datasize;
 unsigned long suffixsize;
 unsigned long alignment;
 unsigned long bufpoolid;
};

struct venc_bufferpayload{
 unsigned char *pbuffer;
 unsigned long nsize;
 int fd;
 unsigned int offset;
 unsigned int maped_size;
 unsigned long filled_len;
};

struct venc_buffer{
 unsigned char *ptrbuffer;
 unsigned long size;
 unsigned long len;
 unsigned long offset;
 long long timestamp;
 unsigned long flags;
 void *clientdata;
};

struct venc_basecfg{
 unsigned long input_width;
 unsigned long input_height;
 unsigned long dvs_width;
 unsigned long dvs_height;
 unsigned long codectype;
 unsigned long fps_num;
 unsigned long fps_den;
 unsigned long targetbitrate;
 unsigned long inputformat;
};

struct venc_profile{
 unsigned long profile;
};
struct ven_profilelevel{
 unsigned long level;
};

struct venc_sessionqp{
 unsigned long iframeqp;
 unsigned long pframqp;
};

struct venc_qprange{
 unsigned long maxqp;
 unsigned long minqp;
};
struct venc_intraperiod{
 unsigned long num_pframes;
};
struct venc_seqheader{
 unsigned char *hdrbufptr;
 unsigned long bufsize;
 unsigned long hdrlen;
};

struct venc_capability{
 unsigned long codec_types;
 unsigned long maxframe_width;
 unsigned long maxframe_height;
 unsigned long maxtarget_bitrate;
 unsigned long maxframe_rate;
 unsigned long input_formats;
 unsigned char dvs;
};

struct venc_entropycfg{
 unsigned longentropysel;
 unsigned long cabacmodel;
};

struct venc_dbcfg{
 unsigned long db_mode;
 unsigned long slicealpha_offset;
 unsigned long slicebeta_offset;
};

struct venc_intrarefresh{
 unsigned long irmode;
 unsigned long mbcount;
};

struct venc_multiclicecfg{
 unsigned long mslice_mode;
 unsigned long mslice_size;
};

struct venc_bufferflush{
 unsigned long flush_mode;
};

struct venc_ratectrlcfg{
 unsigned long rcmode;
};

struct venc_voptimingcfg{
 unsigned long voptime_resolution;
};
struct venc_framerate{
 unsigned long fps_denominator;
 unsigned long fps_numerator;
};

struct venc_targetbitrate{
 unsigned long target_bitrate;
};

struct venc_rotation{
 unsigned long rotation;
};

struct venc_timeout{
 unsigned long millisec;
};

struct venc_headerextension{
 unsigned long header_extension;
};

struct venc_msg{
 unsigned long statuscode;
 unsigned long msgcode;
 struct venc_buffer buf;
 unsigned long msgdata_size;
};
#endif

