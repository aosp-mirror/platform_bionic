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
#ifndef _IPT_SCTP_H_
#define _IPT_SCTP_H_
#define IPT_SCTP_SRC_PORTS 0x01
#define IPT_SCTP_DEST_PORTS 0x02
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define IPT_SCTP_CHUNK_TYPES 0x04
#define IPT_SCTP_VALID_FLAGS 0x07
struct ipt_sctp_flag_info {
 u_int8_t chunktype;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 u_int8_t flag;
 u_int8_t flag_mask;
};
#define IPT_NUM_SCTP_FLAGS 4
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct ipt_sctp_info {
 u_int16_t dpts[2];
 u_int16_t spts[2];
 u_int32_t chunkmap[256 / sizeof (u_int32_t)];
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define SCTP_CHUNK_MATCH_ANY 0x01
#define SCTP_CHUNK_MATCH_ALL 0x02
#define SCTP_CHUNK_MATCH_ONLY 0x04
 u_int32_t chunk_match_type;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct ipt_sctp_flag_info flag_info[IPT_NUM_SCTP_FLAGS];
 int flag_count;
 u_int32_t flags;
 u_int32_t invflags;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
#define bytes(type) (sizeof(type) * 8)
#define SCTP_CHUNKMAP_SET(chunkmap, type)   do {   chunkmap[type / bytes(u_int32_t)] |=   1 << (type % bytes(u_int32_t));   } while (0)
#define SCTP_CHUNKMAP_CLEAR(chunkmap, type)   do {   chunkmap[type / bytes(u_int32_t)] &=   ~(1 << (type % bytes(u_int32_t)));   } while (0)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define SCTP_CHUNKMAP_IS_SET(chunkmap, type)  ({   (chunkmap[type / bytes (u_int32_t)] &   (1 << (type % bytes (u_int32_t)))) ? 1: 0;  })
#define SCTP_CHUNKMAP_RESET(chunkmap)   do {   int i;   for (i = 0; i < ARRAY_SIZE(chunkmap); i++)   chunkmap[i] = 0;   } while (0)
#define SCTP_CHUNKMAP_SET_ALL(chunkmap)   do {   int i;   for (i = 0; i < ARRAY_SIZE(chunkmap); i++)   chunkmap[i] = ~0;   } while (0)
#define SCTP_CHUNKMAP_COPY(destmap, srcmap)   do {   int i;   for (i = 0; i < ARRAY_SIZE(chunkmap); i++)   destmap[i] = srcmap[i];   } while (0)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define SCTP_CHUNKMAP_IS_CLEAR(chunkmap)  ({   int i;   int flag = 1;   for (i = 0; i < ARRAY_SIZE(chunkmap); i++) {   if (chunkmap[i]) {   flag = 0;   break;   }   }   flag;  })
#define SCTP_CHUNKMAP_IS_ALL_SET(chunkmap)  ({   int i;   int flag = 1;   for (i = 0; i < ARRAY_SIZE(chunkmap); i++) {   if (chunkmap[i] != ~0) {   flag = 0;   break;   }   }   flag;  })
#endif
