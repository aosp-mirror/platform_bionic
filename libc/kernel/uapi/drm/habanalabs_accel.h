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
#ifndef HABANALABS_H_
#define HABANALABS_H_
#include <linux/types.h>
#include <linux/ioctl.h>
#define GOYA_KMD_SRAM_RESERVED_SIZE_FROM_START 0x8000
#define GAUDI_DRIVER_SRAM_RESERVED_SIZE_FROM_START 0x80
#define GAUDI_FIRST_AVAILABLE_W_S_SYNC_OBJECT 144
#define GAUDI_FIRST_AVAILABLE_W_S_MONITOR 72
#define TS_MAX_ELEMENTS_NUM (1 << 20)
enum goya_queue_id {
  GOYA_QUEUE_ID_DMA_0 = 0,
  GOYA_QUEUE_ID_DMA_1 = 1,
  GOYA_QUEUE_ID_DMA_2 = 2,
  GOYA_QUEUE_ID_DMA_3 = 3,
  GOYA_QUEUE_ID_DMA_4 = 4,
  GOYA_QUEUE_ID_CPU_PQ = 5,
  GOYA_QUEUE_ID_MME = 6,
  GOYA_QUEUE_ID_TPC0 = 7,
  GOYA_QUEUE_ID_TPC1 = 8,
  GOYA_QUEUE_ID_TPC2 = 9,
  GOYA_QUEUE_ID_TPC3 = 10,
  GOYA_QUEUE_ID_TPC4 = 11,
  GOYA_QUEUE_ID_TPC5 = 12,
  GOYA_QUEUE_ID_TPC6 = 13,
  GOYA_QUEUE_ID_TPC7 = 14,
  GOYA_QUEUE_ID_SIZE
};
enum gaudi_queue_id {
  GAUDI_QUEUE_ID_DMA_0_0 = 0,
  GAUDI_QUEUE_ID_DMA_0_1 = 1,
  GAUDI_QUEUE_ID_DMA_0_2 = 2,
  GAUDI_QUEUE_ID_DMA_0_3 = 3,
  GAUDI_QUEUE_ID_DMA_1_0 = 4,
  GAUDI_QUEUE_ID_DMA_1_1 = 5,
  GAUDI_QUEUE_ID_DMA_1_2 = 6,
  GAUDI_QUEUE_ID_DMA_1_3 = 7,
  GAUDI_QUEUE_ID_CPU_PQ = 8,
  GAUDI_QUEUE_ID_DMA_2_0 = 9,
  GAUDI_QUEUE_ID_DMA_2_1 = 10,
  GAUDI_QUEUE_ID_DMA_2_2 = 11,
  GAUDI_QUEUE_ID_DMA_2_3 = 12,
  GAUDI_QUEUE_ID_DMA_3_0 = 13,
  GAUDI_QUEUE_ID_DMA_3_1 = 14,
  GAUDI_QUEUE_ID_DMA_3_2 = 15,
  GAUDI_QUEUE_ID_DMA_3_3 = 16,
  GAUDI_QUEUE_ID_DMA_4_0 = 17,
  GAUDI_QUEUE_ID_DMA_4_1 = 18,
  GAUDI_QUEUE_ID_DMA_4_2 = 19,
  GAUDI_QUEUE_ID_DMA_4_3 = 20,
  GAUDI_QUEUE_ID_DMA_5_0 = 21,
  GAUDI_QUEUE_ID_DMA_5_1 = 22,
  GAUDI_QUEUE_ID_DMA_5_2 = 23,
  GAUDI_QUEUE_ID_DMA_5_3 = 24,
  GAUDI_QUEUE_ID_DMA_6_0 = 25,
  GAUDI_QUEUE_ID_DMA_6_1 = 26,
  GAUDI_QUEUE_ID_DMA_6_2 = 27,
  GAUDI_QUEUE_ID_DMA_6_3 = 28,
  GAUDI_QUEUE_ID_DMA_7_0 = 29,
  GAUDI_QUEUE_ID_DMA_7_1 = 30,
  GAUDI_QUEUE_ID_DMA_7_2 = 31,
  GAUDI_QUEUE_ID_DMA_7_3 = 32,
  GAUDI_QUEUE_ID_MME_0_0 = 33,
  GAUDI_QUEUE_ID_MME_0_1 = 34,
  GAUDI_QUEUE_ID_MME_0_2 = 35,
  GAUDI_QUEUE_ID_MME_0_3 = 36,
  GAUDI_QUEUE_ID_MME_1_0 = 37,
  GAUDI_QUEUE_ID_MME_1_1 = 38,
  GAUDI_QUEUE_ID_MME_1_2 = 39,
  GAUDI_QUEUE_ID_MME_1_3 = 40,
  GAUDI_QUEUE_ID_TPC_0_0 = 41,
  GAUDI_QUEUE_ID_TPC_0_1 = 42,
  GAUDI_QUEUE_ID_TPC_0_2 = 43,
  GAUDI_QUEUE_ID_TPC_0_3 = 44,
  GAUDI_QUEUE_ID_TPC_1_0 = 45,
  GAUDI_QUEUE_ID_TPC_1_1 = 46,
  GAUDI_QUEUE_ID_TPC_1_2 = 47,
  GAUDI_QUEUE_ID_TPC_1_3 = 48,
  GAUDI_QUEUE_ID_TPC_2_0 = 49,
  GAUDI_QUEUE_ID_TPC_2_1 = 50,
  GAUDI_QUEUE_ID_TPC_2_2 = 51,
  GAUDI_QUEUE_ID_TPC_2_3 = 52,
  GAUDI_QUEUE_ID_TPC_3_0 = 53,
  GAUDI_QUEUE_ID_TPC_3_1 = 54,
  GAUDI_QUEUE_ID_TPC_3_2 = 55,
  GAUDI_QUEUE_ID_TPC_3_3 = 56,
  GAUDI_QUEUE_ID_TPC_4_0 = 57,
  GAUDI_QUEUE_ID_TPC_4_1 = 58,
  GAUDI_QUEUE_ID_TPC_4_2 = 59,
  GAUDI_QUEUE_ID_TPC_4_3 = 60,
  GAUDI_QUEUE_ID_TPC_5_0 = 61,
  GAUDI_QUEUE_ID_TPC_5_1 = 62,
  GAUDI_QUEUE_ID_TPC_5_2 = 63,
  GAUDI_QUEUE_ID_TPC_5_3 = 64,
  GAUDI_QUEUE_ID_TPC_6_0 = 65,
  GAUDI_QUEUE_ID_TPC_6_1 = 66,
  GAUDI_QUEUE_ID_TPC_6_2 = 67,
  GAUDI_QUEUE_ID_TPC_6_3 = 68,
  GAUDI_QUEUE_ID_TPC_7_0 = 69,
  GAUDI_QUEUE_ID_TPC_7_1 = 70,
  GAUDI_QUEUE_ID_TPC_7_2 = 71,
  GAUDI_QUEUE_ID_TPC_7_3 = 72,
  GAUDI_QUEUE_ID_NIC_0_0 = 73,
  GAUDI_QUEUE_ID_NIC_0_1 = 74,
  GAUDI_QUEUE_ID_NIC_0_2 = 75,
  GAUDI_QUEUE_ID_NIC_0_3 = 76,
  GAUDI_QUEUE_ID_NIC_1_0 = 77,
  GAUDI_QUEUE_ID_NIC_1_1 = 78,
  GAUDI_QUEUE_ID_NIC_1_2 = 79,
  GAUDI_QUEUE_ID_NIC_1_3 = 80,
  GAUDI_QUEUE_ID_NIC_2_0 = 81,
  GAUDI_QUEUE_ID_NIC_2_1 = 82,
  GAUDI_QUEUE_ID_NIC_2_2 = 83,
  GAUDI_QUEUE_ID_NIC_2_3 = 84,
  GAUDI_QUEUE_ID_NIC_3_0 = 85,
  GAUDI_QUEUE_ID_NIC_3_1 = 86,
  GAUDI_QUEUE_ID_NIC_3_2 = 87,
  GAUDI_QUEUE_ID_NIC_3_3 = 88,
  GAUDI_QUEUE_ID_NIC_4_0 = 89,
  GAUDI_QUEUE_ID_NIC_4_1 = 90,
  GAUDI_QUEUE_ID_NIC_4_2 = 91,
  GAUDI_QUEUE_ID_NIC_4_3 = 92,
  GAUDI_QUEUE_ID_NIC_5_0 = 93,
  GAUDI_QUEUE_ID_NIC_5_1 = 94,
  GAUDI_QUEUE_ID_NIC_5_2 = 95,
  GAUDI_QUEUE_ID_NIC_5_3 = 96,
  GAUDI_QUEUE_ID_NIC_6_0 = 97,
  GAUDI_QUEUE_ID_NIC_6_1 = 98,
  GAUDI_QUEUE_ID_NIC_6_2 = 99,
  GAUDI_QUEUE_ID_NIC_6_3 = 100,
  GAUDI_QUEUE_ID_NIC_7_0 = 101,
  GAUDI_QUEUE_ID_NIC_7_1 = 102,
  GAUDI_QUEUE_ID_NIC_7_2 = 103,
  GAUDI_QUEUE_ID_NIC_7_3 = 104,
  GAUDI_QUEUE_ID_NIC_8_0 = 105,
  GAUDI_QUEUE_ID_NIC_8_1 = 106,
  GAUDI_QUEUE_ID_NIC_8_2 = 107,
  GAUDI_QUEUE_ID_NIC_8_3 = 108,
  GAUDI_QUEUE_ID_NIC_9_0 = 109,
  GAUDI_QUEUE_ID_NIC_9_1 = 110,
  GAUDI_QUEUE_ID_NIC_9_2 = 111,
  GAUDI_QUEUE_ID_NIC_9_3 = 112,
  GAUDI_QUEUE_ID_SIZE
};
enum gaudi2_queue_id {
  GAUDI2_QUEUE_ID_PDMA_0_0 = 0,
  GAUDI2_QUEUE_ID_PDMA_0_1 = 1,
  GAUDI2_QUEUE_ID_PDMA_0_2 = 2,
  GAUDI2_QUEUE_ID_PDMA_0_3 = 3,
  GAUDI2_QUEUE_ID_PDMA_1_0 = 4,
  GAUDI2_QUEUE_ID_PDMA_1_1 = 5,
  GAUDI2_QUEUE_ID_PDMA_1_2 = 6,
  GAUDI2_QUEUE_ID_PDMA_1_3 = 7,
  GAUDI2_QUEUE_ID_DCORE0_EDMA_0_0 = 8,
  GAUDI2_QUEUE_ID_DCORE0_EDMA_0_1 = 9,
  GAUDI2_QUEUE_ID_DCORE0_EDMA_0_2 = 10,
  GAUDI2_QUEUE_ID_DCORE0_EDMA_0_3 = 11,
  GAUDI2_QUEUE_ID_DCORE0_EDMA_1_0 = 12,
  GAUDI2_QUEUE_ID_DCORE0_EDMA_1_1 = 13,
  GAUDI2_QUEUE_ID_DCORE0_EDMA_1_2 = 14,
  GAUDI2_QUEUE_ID_DCORE0_EDMA_1_3 = 15,
  GAUDI2_QUEUE_ID_DCORE0_MME_0_0 = 16,
  GAUDI2_QUEUE_ID_DCORE0_MME_0_1 = 17,
  GAUDI2_QUEUE_ID_DCORE0_MME_0_2 = 18,
  GAUDI2_QUEUE_ID_DCORE0_MME_0_3 = 19,
  GAUDI2_QUEUE_ID_DCORE0_TPC_0_0 = 20,
  GAUDI2_QUEUE_ID_DCORE0_TPC_0_1 = 21,
  GAUDI2_QUEUE_ID_DCORE0_TPC_0_2 = 22,
  GAUDI2_QUEUE_ID_DCORE0_TPC_0_3 = 23,
  GAUDI2_QUEUE_ID_DCORE0_TPC_1_0 = 24,
  GAUDI2_QUEUE_ID_DCORE0_TPC_1_1 = 25,
  GAUDI2_QUEUE_ID_DCORE0_TPC_1_2 = 26,
  GAUDI2_QUEUE_ID_DCORE0_TPC_1_3 = 27,
  GAUDI2_QUEUE_ID_DCORE0_TPC_2_0 = 28,
  GAUDI2_QUEUE_ID_DCORE0_TPC_2_1 = 29,
  GAUDI2_QUEUE_ID_DCORE0_TPC_2_2 = 30,
  GAUDI2_QUEUE_ID_DCORE0_TPC_2_3 = 31,
  GAUDI2_QUEUE_ID_DCORE0_TPC_3_0 = 32,
  GAUDI2_QUEUE_ID_DCORE0_TPC_3_1 = 33,
  GAUDI2_QUEUE_ID_DCORE0_TPC_3_2 = 34,
  GAUDI2_QUEUE_ID_DCORE0_TPC_3_3 = 35,
  GAUDI2_QUEUE_ID_DCORE0_TPC_4_0 = 36,
  GAUDI2_QUEUE_ID_DCORE0_TPC_4_1 = 37,
  GAUDI2_QUEUE_ID_DCORE0_TPC_4_2 = 38,
  GAUDI2_QUEUE_ID_DCORE0_TPC_4_3 = 39,
  GAUDI2_QUEUE_ID_DCORE0_TPC_5_0 = 40,
  GAUDI2_QUEUE_ID_DCORE0_TPC_5_1 = 41,
  GAUDI2_QUEUE_ID_DCORE0_TPC_5_2 = 42,
  GAUDI2_QUEUE_ID_DCORE0_TPC_5_3 = 43,
  GAUDI2_QUEUE_ID_DCORE0_TPC_6_0 = 44,
  GAUDI2_QUEUE_ID_DCORE0_TPC_6_1 = 45,
  GAUDI2_QUEUE_ID_DCORE0_TPC_6_2 = 46,
  GAUDI2_QUEUE_ID_DCORE0_TPC_6_3 = 47,
  GAUDI2_QUEUE_ID_DCORE1_EDMA_0_0 = 48,
  GAUDI2_QUEUE_ID_DCORE1_EDMA_0_1 = 49,
  GAUDI2_QUEUE_ID_DCORE1_EDMA_0_2 = 50,
  GAUDI2_QUEUE_ID_DCORE1_EDMA_0_3 = 51,
  GAUDI2_QUEUE_ID_DCORE1_EDMA_1_0 = 52,
  GAUDI2_QUEUE_ID_DCORE1_EDMA_1_1 = 53,
  GAUDI2_QUEUE_ID_DCORE1_EDMA_1_2 = 54,
  GAUDI2_QUEUE_ID_DCORE1_EDMA_1_3 = 55,
  GAUDI2_QUEUE_ID_DCORE1_MME_0_0 = 56,
  GAUDI2_QUEUE_ID_DCORE1_MME_0_1 = 57,
  GAUDI2_QUEUE_ID_DCORE1_MME_0_2 = 58,
  GAUDI2_QUEUE_ID_DCORE1_MME_0_3 = 59,
  GAUDI2_QUEUE_ID_DCORE1_TPC_0_0 = 60,
  GAUDI2_QUEUE_ID_DCORE1_TPC_0_1 = 61,
  GAUDI2_QUEUE_ID_DCORE1_TPC_0_2 = 62,
  GAUDI2_QUEUE_ID_DCORE1_TPC_0_3 = 63,
  GAUDI2_QUEUE_ID_DCORE1_TPC_1_0 = 64,
  GAUDI2_QUEUE_ID_DCORE1_TPC_1_1 = 65,
  GAUDI2_QUEUE_ID_DCORE1_TPC_1_2 = 66,
  GAUDI2_QUEUE_ID_DCORE1_TPC_1_3 = 67,
  GAUDI2_QUEUE_ID_DCORE1_TPC_2_0 = 68,
  GAUDI2_QUEUE_ID_DCORE1_TPC_2_1 = 69,
  GAUDI2_QUEUE_ID_DCORE1_TPC_2_2 = 70,
  GAUDI2_QUEUE_ID_DCORE1_TPC_2_3 = 71,
  GAUDI2_QUEUE_ID_DCORE1_TPC_3_0 = 72,
  GAUDI2_QUEUE_ID_DCORE1_TPC_3_1 = 73,
  GAUDI2_QUEUE_ID_DCORE1_TPC_3_2 = 74,
  GAUDI2_QUEUE_ID_DCORE1_TPC_3_3 = 75,
  GAUDI2_QUEUE_ID_DCORE1_TPC_4_0 = 76,
  GAUDI2_QUEUE_ID_DCORE1_TPC_4_1 = 77,
  GAUDI2_QUEUE_ID_DCORE1_TPC_4_2 = 78,
  GAUDI2_QUEUE_ID_DCORE1_TPC_4_3 = 79,
  GAUDI2_QUEUE_ID_DCORE1_TPC_5_0 = 80,
  GAUDI2_QUEUE_ID_DCORE1_TPC_5_1 = 81,
  GAUDI2_QUEUE_ID_DCORE1_TPC_5_2 = 82,
  GAUDI2_QUEUE_ID_DCORE1_TPC_5_3 = 83,
  GAUDI2_QUEUE_ID_DCORE2_EDMA_0_0 = 84,
  GAUDI2_QUEUE_ID_DCORE2_EDMA_0_1 = 85,
  GAUDI2_QUEUE_ID_DCORE2_EDMA_0_2 = 86,
  GAUDI2_QUEUE_ID_DCORE2_EDMA_0_3 = 87,
  GAUDI2_QUEUE_ID_DCORE2_EDMA_1_0 = 88,
  GAUDI2_QUEUE_ID_DCORE2_EDMA_1_1 = 89,
  GAUDI2_QUEUE_ID_DCORE2_EDMA_1_2 = 90,
  GAUDI2_QUEUE_ID_DCORE2_EDMA_1_3 = 91,
  GAUDI2_QUEUE_ID_DCORE2_MME_0_0 = 92,
  GAUDI2_QUEUE_ID_DCORE2_MME_0_1 = 93,
  GAUDI2_QUEUE_ID_DCORE2_MME_0_2 = 94,
  GAUDI2_QUEUE_ID_DCORE2_MME_0_3 = 95,
  GAUDI2_QUEUE_ID_DCORE2_TPC_0_0 = 96,
  GAUDI2_QUEUE_ID_DCORE2_TPC_0_1 = 97,
  GAUDI2_QUEUE_ID_DCORE2_TPC_0_2 = 98,
  GAUDI2_QUEUE_ID_DCORE2_TPC_0_3 = 99,
  GAUDI2_QUEUE_ID_DCORE2_TPC_1_0 = 100,
  GAUDI2_QUEUE_ID_DCORE2_TPC_1_1 = 101,
  GAUDI2_QUEUE_ID_DCORE2_TPC_1_2 = 102,
  GAUDI2_QUEUE_ID_DCORE2_TPC_1_3 = 103,
  GAUDI2_QUEUE_ID_DCORE2_TPC_2_0 = 104,
  GAUDI2_QUEUE_ID_DCORE2_TPC_2_1 = 105,
  GAUDI2_QUEUE_ID_DCORE2_TPC_2_2 = 106,
  GAUDI2_QUEUE_ID_DCORE2_TPC_2_3 = 107,
  GAUDI2_QUEUE_ID_DCORE2_TPC_3_0 = 108,
  GAUDI2_QUEUE_ID_DCORE2_TPC_3_1 = 109,
  GAUDI2_QUEUE_ID_DCORE2_TPC_3_2 = 110,
  GAUDI2_QUEUE_ID_DCORE2_TPC_3_3 = 111,
  GAUDI2_QUEUE_ID_DCORE2_TPC_4_0 = 112,
  GAUDI2_QUEUE_ID_DCORE2_TPC_4_1 = 113,
  GAUDI2_QUEUE_ID_DCORE2_TPC_4_2 = 114,
  GAUDI2_QUEUE_ID_DCORE2_TPC_4_3 = 115,
  GAUDI2_QUEUE_ID_DCORE2_TPC_5_0 = 116,
  GAUDI2_QUEUE_ID_DCORE2_TPC_5_1 = 117,
  GAUDI2_QUEUE_ID_DCORE2_TPC_5_2 = 118,
  GAUDI2_QUEUE_ID_DCORE2_TPC_5_3 = 119,
  GAUDI2_QUEUE_ID_DCORE3_EDMA_0_0 = 120,
  GAUDI2_QUEUE_ID_DCORE3_EDMA_0_1 = 121,
  GAUDI2_QUEUE_ID_DCORE3_EDMA_0_2 = 122,
  GAUDI2_QUEUE_ID_DCORE3_EDMA_0_3 = 123,
  GAUDI2_QUEUE_ID_DCORE3_EDMA_1_0 = 124,
  GAUDI2_QUEUE_ID_DCORE3_EDMA_1_1 = 125,
  GAUDI2_QUEUE_ID_DCORE3_EDMA_1_2 = 126,
  GAUDI2_QUEUE_ID_DCORE3_EDMA_1_3 = 127,
  GAUDI2_QUEUE_ID_DCORE3_MME_0_0 = 128,
  GAUDI2_QUEUE_ID_DCORE3_MME_0_1 = 129,
  GAUDI2_QUEUE_ID_DCORE3_MME_0_2 = 130,
  GAUDI2_QUEUE_ID_DCORE3_MME_0_3 = 131,
  GAUDI2_QUEUE_ID_DCORE3_TPC_0_0 = 132,
  GAUDI2_QUEUE_ID_DCORE3_TPC_0_1 = 133,
  GAUDI2_QUEUE_ID_DCORE3_TPC_0_2 = 134,
  GAUDI2_QUEUE_ID_DCORE3_TPC_0_3 = 135,
  GAUDI2_QUEUE_ID_DCORE3_TPC_1_0 = 136,
  GAUDI2_QUEUE_ID_DCORE3_TPC_1_1 = 137,
  GAUDI2_QUEUE_ID_DCORE3_TPC_1_2 = 138,
  GAUDI2_QUEUE_ID_DCORE3_TPC_1_3 = 139,
  GAUDI2_QUEUE_ID_DCORE3_TPC_2_0 = 140,
  GAUDI2_QUEUE_ID_DCORE3_TPC_2_1 = 141,
  GAUDI2_QUEUE_ID_DCORE3_TPC_2_2 = 142,
  GAUDI2_QUEUE_ID_DCORE3_TPC_2_3 = 143,
  GAUDI2_QUEUE_ID_DCORE3_TPC_3_0 = 144,
  GAUDI2_QUEUE_ID_DCORE3_TPC_3_1 = 145,
  GAUDI2_QUEUE_ID_DCORE3_TPC_3_2 = 146,
  GAUDI2_QUEUE_ID_DCORE3_TPC_3_3 = 147,
  GAUDI2_QUEUE_ID_DCORE3_TPC_4_0 = 148,
  GAUDI2_QUEUE_ID_DCORE3_TPC_4_1 = 149,
  GAUDI2_QUEUE_ID_DCORE3_TPC_4_2 = 150,
  GAUDI2_QUEUE_ID_DCORE3_TPC_4_3 = 151,
  GAUDI2_QUEUE_ID_DCORE3_TPC_5_0 = 152,
  GAUDI2_QUEUE_ID_DCORE3_TPC_5_1 = 153,
  GAUDI2_QUEUE_ID_DCORE3_TPC_5_2 = 154,
  GAUDI2_QUEUE_ID_DCORE3_TPC_5_3 = 155,
  GAUDI2_QUEUE_ID_NIC_0_0 = 156,
  GAUDI2_QUEUE_ID_NIC_0_1 = 157,
  GAUDI2_QUEUE_ID_NIC_0_2 = 158,
  GAUDI2_QUEUE_ID_NIC_0_3 = 159,
  GAUDI2_QUEUE_ID_NIC_1_0 = 160,
  GAUDI2_QUEUE_ID_NIC_1_1 = 161,
  GAUDI2_QUEUE_ID_NIC_1_2 = 162,
  GAUDI2_QUEUE_ID_NIC_1_3 = 163,
  GAUDI2_QUEUE_ID_NIC_2_0 = 164,
  GAUDI2_QUEUE_ID_NIC_2_1 = 165,
  GAUDI2_QUEUE_ID_NIC_2_2 = 166,
  GAUDI2_QUEUE_ID_NIC_2_3 = 167,
  GAUDI2_QUEUE_ID_NIC_3_0 = 168,
  GAUDI2_QUEUE_ID_NIC_3_1 = 169,
  GAUDI2_QUEUE_ID_NIC_3_2 = 170,
  GAUDI2_QUEUE_ID_NIC_3_3 = 171,
  GAUDI2_QUEUE_ID_NIC_4_0 = 172,
  GAUDI2_QUEUE_ID_NIC_4_1 = 173,
  GAUDI2_QUEUE_ID_NIC_4_2 = 174,
  GAUDI2_QUEUE_ID_NIC_4_3 = 175,
  GAUDI2_QUEUE_ID_NIC_5_0 = 176,
  GAUDI2_QUEUE_ID_NIC_5_1 = 177,
  GAUDI2_QUEUE_ID_NIC_5_2 = 178,
  GAUDI2_QUEUE_ID_NIC_5_3 = 179,
  GAUDI2_QUEUE_ID_NIC_6_0 = 180,
  GAUDI2_QUEUE_ID_NIC_6_1 = 181,
  GAUDI2_QUEUE_ID_NIC_6_2 = 182,
  GAUDI2_QUEUE_ID_NIC_6_3 = 183,
  GAUDI2_QUEUE_ID_NIC_7_0 = 184,
  GAUDI2_QUEUE_ID_NIC_7_1 = 185,
  GAUDI2_QUEUE_ID_NIC_7_2 = 186,
  GAUDI2_QUEUE_ID_NIC_7_3 = 187,
  GAUDI2_QUEUE_ID_NIC_8_0 = 188,
  GAUDI2_QUEUE_ID_NIC_8_1 = 189,
  GAUDI2_QUEUE_ID_NIC_8_2 = 190,
  GAUDI2_QUEUE_ID_NIC_8_3 = 191,
  GAUDI2_QUEUE_ID_NIC_9_0 = 192,
  GAUDI2_QUEUE_ID_NIC_9_1 = 193,
  GAUDI2_QUEUE_ID_NIC_9_2 = 194,
  GAUDI2_QUEUE_ID_NIC_9_3 = 195,
  GAUDI2_QUEUE_ID_NIC_10_0 = 196,
  GAUDI2_QUEUE_ID_NIC_10_1 = 197,
  GAUDI2_QUEUE_ID_NIC_10_2 = 198,
  GAUDI2_QUEUE_ID_NIC_10_3 = 199,
  GAUDI2_QUEUE_ID_NIC_11_0 = 200,
  GAUDI2_QUEUE_ID_NIC_11_1 = 201,
  GAUDI2_QUEUE_ID_NIC_11_2 = 202,
  GAUDI2_QUEUE_ID_NIC_11_3 = 203,
  GAUDI2_QUEUE_ID_NIC_12_0 = 204,
  GAUDI2_QUEUE_ID_NIC_12_1 = 205,
  GAUDI2_QUEUE_ID_NIC_12_2 = 206,
  GAUDI2_QUEUE_ID_NIC_12_3 = 207,
  GAUDI2_QUEUE_ID_NIC_13_0 = 208,
  GAUDI2_QUEUE_ID_NIC_13_1 = 209,
  GAUDI2_QUEUE_ID_NIC_13_2 = 210,
  GAUDI2_QUEUE_ID_NIC_13_3 = 211,
  GAUDI2_QUEUE_ID_NIC_14_0 = 212,
  GAUDI2_QUEUE_ID_NIC_14_1 = 213,
  GAUDI2_QUEUE_ID_NIC_14_2 = 214,
  GAUDI2_QUEUE_ID_NIC_14_3 = 215,
  GAUDI2_QUEUE_ID_NIC_15_0 = 216,
  GAUDI2_QUEUE_ID_NIC_15_1 = 217,
  GAUDI2_QUEUE_ID_NIC_15_2 = 218,
  GAUDI2_QUEUE_ID_NIC_15_3 = 219,
  GAUDI2_QUEUE_ID_NIC_16_0 = 220,
  GAUDI2_QUEUE_ID_NIC_16_1 = 221,
  GAUDI2_QUEUE_ID_NIC_16_2 = 222,
  GAUDI2_QUEUE_ID_NIC_16_3 = 223,
  GAUDI2_QUEUE_ID_NIC_17_0 = 224,
  GAUDI2_QUEUE_ID_NIC_17_1 = 225,
  GAUDI2_QUEUE_ID_NIC_17_2 = 226,
  GAUDI2_QUEUE_ID_NIC_17_3 = 227,
  GAUDI2_QUEUE_ID_NIC_18_0 = 228,
  GAUDI2_QUEUE_ID_NIC_18_1 = 229,
  GAUDI2_QUEUE_ID_NIC_18_2 = 230,
  GAUDI2_QUEUE_ID_NIC_18_3 = 231,
  GAUDI2_QUEUE_ID_NIC_19_0 = 232,
  GAUDI2_QUEUE_ID_NIC_19_1 = 233,
  GAUDI2_QUEUE_ID_NIC_19_2 = 234,
  GAUDI2_QUEUE_ID_NIC_19_3 = 235,
  GAUDI2_QUEUE_ID_NIC_20_0 = 236,
  GAUDI2_QUEUE_ID_NIC_20_1 = 237,
  GAUDI2_QUEUE_ID_NIC_20_2 = 238,
  GAUDI2_QUEUE_ID_NIC_20_3 = 239,
  GAUDI2_QUEUE_ID_NIC_21_0 = 240,
  GAUDI2_QUEUE_ID_NIC_21_1 = 241,
  GAUDI2_QUEUE_ID_NIC_21_2 = 242,
  GAUDI2_QUEUE_ID_NIC_21_3 = 243,
  GAUDI2_QUEUE_ID_NIC_22_0 = 244,
  GAUDI2_QUEUE_ID_NIC_22_1 = 245,
  GAUDI2_QUEUE_ID_NIC_22_2 = 246,
  GAUDI2_QUEUE_ID_NIC_22_3 = 247,
  GAUDI2_QUEUE_ID_NIC_23_0 = 248,
  GAUDI2_QUEUE_ID_NIC_23_1 = 249,
  GAUDI2_QUEUE_ID_NIC_23_2 = 250,
  GAUDI2_QUEUE_ID_NIC_23_3 = 251,
  GAUDI2_QUEUE_ID_ROT_0_0 = 252,
  GAUDI2_QUEUE_ID_ROT_0_1 = 253,
  GAUDI2_QUEUE_ID_ROT_0_2 = 254,
  GAUDI2_QUEUE_ID_ROT_0_3 = 255,
  GAUDI2_QUEUE_ID_ROT_1_0 = 256,
  GAUDI2_QUEUE_ID_ROT_1_1 = 257,
  GAUDI2_QUEUE_ID_ROT_1_2 = 258,
  GAUDI2_QUEUE_ID_ROT_1_3 = 259,
  GAUDI2_QUEUE_ID_CPU_PQ = 260,
  GAUDI2_QUEUE_ID_SIZE
};
enum goya_engine_id {
  GOYA_ENGINE_ID_DMA_0 = 0,
  GOYA_ENGINE_ID_DMA_1,
  GOYA_ENGINE_ID_DMA_2,
  GOYA_ENGINE_ID_DMA_3,
  GOYA_ENGINE_ID_DMA_4,
  GOYA_ENGINE_ID_MME_0,
  GOYA_ENGINE_ID_TPC_0,
  GOYA_ENGINE_ID_TPC_1,
  GOYA_ENGINE_ID_TPC_2,
  GOYA_ENGINE_ID_TPC_3,
  GOYA_ENGINE_ID_TPC_4,
  GOYA_ENGINE_ID_TPC_5,
  GOYA_ENGINE_ID_TPC_6,
  GOYA_ENGINE_ID_TPC_7,
  GOYA_ENGINE_ID_SIZE
};
enum gaudi_engine_id {
  GAUDI_ENGINE_ID_DMA_0 = 0,
  GAUDI_ENGINE_ID_DMA_1,
  GAUDI_ENGINE_ID_DMA_2,
  GAUDI_ENGINE_ID_DMA_3,
  GAUDI_ENGINE_ID_DMA_4,
  GAUDI_ENGINE_ID_DMA_5,
  GAUDI_ENGINE_ID_DMA_6,
  GAUDI_ENGINE_ID_DMA_7,
  GAUDI_ENGINE_ID_MME_0,
  GAUDI_ENGINE_ID_MME_1,
  GAUDI_ENGINE_ID_MME_2,
  GAUDI_ENGINE_ID_MME_3,
  GAUDI_ENGINE_ID_TPC_0,
  GAUDI_ENGINE_ID_TPC_1,
  GAUDI_ENGINE_ID_TPC_2,
  GAUDI_ENGINE_ID_TPC_3,
  GAUDI_ENGINE_ID_TPC_4,
  GAUDI_ENGINE_ID_TPC_5,
  GAUDI_ENGINE_ID_TPC_6,
  GAUDI_ENGINE_ID_TPC_7,
  GAUDI_ENGINE_ID_NIC_0,
  GAUDI_ENGINE_ID_NIC_1,
  GAUDI_ENGINE_ID_NIC_2,
  GAUDI_ENGINE_ID_NIC_3,
  GAUDI_ENGINE_ID_NIC_4,
  GAUDI_ENGINE_ID_NIC_5,
  GAUDI_ENGINE_ID_NIC_6,
  GAUDI_ENGINE_ID_NIC_7,
  GAUDI_ENGINE_ID_NIC_8,
  GAUDI_ENGINE_ID_NIC_9,
  GAUDI_ENGINE_ID_SIZE
};
enum gaudi2_engine_id {
  GAUDI2_DCORE0_ENGINE_ID_EDMA_0 = 0,
  GAUDI2_DCORE0_ENGINE_ID_EDMA_1,
  GAUDI2_DCORE0_ENGINE_ID_MME,
  GAUDI2_DCORE0_ENGINE_ID_TPC_0,
  GAUDI2_DCORE0_ENGINE_ID_TPC_1,
  GAUDI2_DCORE0_ENGINE_ID_TPC_2,
  GAUDI2_DCORE0_ENGINE_ID_TPC_3,
  GAUDI2_DCORE0_ENGINE_ID_TPC_4,
  GAUDI2_DCORE0_ENGINE_ID_TPC_5,
  GAUDI2_DCORE0_ENGINE_ID_DEC_0,
  GAUDI2_DCORE0_ENGINE_ID_DEC_1,
  GAUDI2_DCORE1_ENGINE_ID_EDMA_0,
  GAUDI2_DCORE1_ENGINE_ID_EDMA_1,
  GAUDI2_DCORE1_ENGINE_ID_MME,
  GAUDI2_DCORE1_ENGINE_ID_TPC_0,
  GAUDI2_DCORE1_ENGINE_ID_TPC_1,
  GAUDI2_DCORE1_ENGINE_ID_TPC_2,
  GAUDI2_DCORE1_ENGINE_ID_TPC_3,
  GAUDI2_DCORE1_ENGINE_ID_TPC_4,
  GAUDI2_DCORE1_ENGINE_ID_TPC_5,
  GAUDI2_DCORE1_ENGINE_ID_DEC_0,
  GAUDI2_DCORE1_ENGINE_ID_DEC_1,
  GAUDI2_DCORE2_ENGINE_ID_EDMA_0,
  GAUDI2_DCORE2_ENGINE_ID_EDMA_1,
  GAUDI2_DCORE2_ENGINE_ID_MME,
  GAUDI2_DCORE2_ENGINE_ID_TPC_0,
  GAUDI2_DCORE2_ENGINE_ID_TPC_1,
  GAUDI2_DCORE2_ENGINE_ID_TPC_2,
  GAUDI2_DCORE2_ENGINE_ID_TPC_3,
  GAUDI2_DCORE2_ENGINE_ID_TPC_4,
  GAUDI2_DCORE2_ENGINE_ID_TPC_5,
  GAUDI2_DCORE2_ENGINE_ID_DEC_0,
  GAUDI2_DCORE2_ENGINE_ID_DEC_1,
  GAUDI2_DCORE3_ENGINE_ID_EDMA_0,
  GAUDI2_DCORE3_ENGINE_ID_EDMA_1,
  GAUDI2_DCORE3_ENGINE_ID_MME,
  GAUDI2_DCORE3_ENGINE_ID_TPC_0,
  GAUDI2_DCORE3_ENGINE_ID_TPC_1,
  GAUDI2_DCORE3_ENGINE_ID_TPC_2,
  GAUDI2_DCORE3_ENGINE_ID_TPC_3,
  GAUDI2_DCORE3_ENGINE_ID_TPC_4,
  GAUDI2_DCORE3_ENGINE_ID_TPC_5,
  GAUDI2_DCORE3_ENGINE_ID_DEC_0,
  GAUDI2_DCORE3_ENGINE_ID_DEC_1,
  GAUDI2_DCORE0_ENGINE_ID_TPC_6,
  GAUDI2_ENGINE_ID_PDMA_0,
  GAUDI2_ENGINE_ID_PDMA_1,
  GAUDI2_ENGINE_ID_ROT_0,
  GAUDI2_ENGINE_ID_ROT_1,
  GAUDI2_PCIE_ENGINE_ID_DEC_0,
  GAUDI2_PCIE_ENGINE_ID_DEC_1,
  GAUDI2_ENGINE_ID_NIC0_0,
  GAUDI2_ENGINE_ID_NIC0_1,
  GAUDI2_ENGINE_ID_NIC1_0,
  GAUDI2_ENGINE_ID_NIC1_1,
  GAUDI2_ENGINE_ID_NIC2_0,
  GAUDI2_ENGINE_ID_NIC2_1,
  GAUDI2_ENGINE_ID_NIC3_0,
  GAUDI2_ENGINE_ID_NIC3_1,
  GAUDI2_ENGINE_ID_NIC4_0,
  GAUDI2_ENGINE_ID_NIC4_1,
  GAUDI2_ENGINE_ID_NIC5_0,
  GAUDI2_ENGINE_ID_NIC5_1,
  GAUDI2_ENGINE_ID_NIC6_0,
  GAUDI2_ENGINE_ID_NIC6_1,
  GAUDI2_ENGINE_ID_NIC7_0,
  GAUDI2_ENGINE_ID_NIC7_1,
  GAUDI2_ENGINE_ID_NIC8_0,
  GAUDI2_ENGINE_ID_NIC8_1,
  GAUDI2_ENGINE_ID_NIC9_0,
  GAUDI2_ENGINE_ID_NIC9_1,
  GAUDI2_ENGINE_ID_NIC10_0,
  GAUDI2_ENGINE_ID_NIC10_1,
  GAUDI2_ENGINE_ID_NIC11_0,
  GAUDI2_ENGINE_ID_NIC11_1,
  GAUDI2_ENGINE_ID_PCIE,
  GAUDI2_ENGINE_ID_PSOC,
  GAUDI2_ENGINE_ID_ARC_FARM,
  GAUDI2_ENGINE_ID_KDMA,
  GAUDI2_ENGINE_ID_SIZE
};
enum hl_goya_pll_index {
  HL_GOYA_CPU_PLL = 0,
  HL_GOYA_IC_PLL,
  HL_GOYA_MC_PLL,
  HL_GOYA_MME_PLL,
  HL_GOYA_PCI_PLL,
  HL_GOYA_EMMC_PLL,
  HL_GOYA_TPC_PLL,
  HL_GOYA_PLL_MAX
};
enum hl_gaudi_pll_index {
  HL_GAUDI_CPU_PLL = 0,
  HL_GAUDI_PCI_PLL,
  HL_GAUDI_SRAM_PLL,
  HL_GAUDI_HBM_PLL,
  HL_GAUDI_NIC_PLL,
  HL_GAUDI_DMA_PLL,
  HL_GAUDI_MESH_PLL,
  HL_GAUDI_MME_PLL,
  HL_GAUDI_TPC_PLL,
  HL_GAUDI_IF_PLL,
  HL_GAUDI_PLL_MAX
};
enum hl_gaudi2_pll_index {
  HL_GAUDI2_CPU_PLL = 0,
  HL_GAUDI2_PCI_PLL,
  HL_GAUDI2_SRAM_PLL,
  HL_GAUDI2_HBM_PLL,
  HL_GAUDI2_NIC_PLL,
  HL_GAUDI2_DMA_PLL,
  HL_GAUDI2_MESH_PLL,
  HL_GAUDI2_MME_PLL,
  HL_GAUDI2_TPC_PLL,
  HL_GAUDI2_IF_PLL,
  HL_GAUDI2_VID_PLL,
  HL_GAUDI2_MSS_PLL,
  HL_GAUDI2_PLL_MAX
};
enum hl_goya_dma_direction {
  HL_DMA_HOST_TO_DRAM,
  HL_DMA_HOST_TO_SRAM,
  HL_DMA_DRAM_TO_SRAM,
  HL_DMA_SRAM_TO_DRAM,
  HL_DMA_SRAM_TO_HOST,
  HL_DMA_DRAM_TO_HOST,
  HL_DMA_DRAM_TO_DRAM,
  HL_DMA_SRAM_TO_SRAM,
  HL_DMA_ENUM_MAX
};
enum hl_device_status {
  HL_DEVICE_STATUS_OPERATIONAL,
  HL_DEVICE_STATUS_IN_RESET,
  HL_DEVICE_STATUS_MALFUNCTION,
  HL_DEVICE_STATUS_NEEDS_RESET,
  HL_DEVICE_STATUS_IN_DEVICE_CREATION,
  HL_DEVICE_STATUS_IN_RESET_AFTER_DEVICE_RELEASE,
  HL_DEVICE_STATUS_LAST = HL_DEVICE_STATUS_IN_RESET_AFTER_DEVICE_RELEASE
};
enum hl_server_type {
  HL_SERVER_TYPE_UNKNOWN = 0,
  HL_SERVER_GAUDI_HLS1 = 1,
  HL_SERVER_GAUDI_HLS1H = 2,
  HL_SERVER_GAUDI_TYPE1 = 3,
  HL_SERVER_GAUDI_TYPE2 = 4,
  HL_SERVER_GAUDI2_HLS2 = 5,
  HL_SERVER_GAUDI2_TYPE1 = 7
};
#define HL_NOTIFIER_EVENT_TPC_ASSERT (1ULL << 0)
#define HL_NOTIFIER_EVENT_UNDEFINED_OPCODE (1ULL << 1)
#define HL_NOTIFIER_EVENT_DEVICE_RESET (1ULL << 2)
#define HL_NOTIFIER_EVENT_CS_TIMEOUT (1ULL << 3)
#define HL_NOTIFIER_EVENT_DEVICE_UNAVAILABLE (1ULL << 4)
#define HL_NOTIFIER_EVENT_USER_ENGINE_ERR (1ULL << 5)
#define HL_NOTIFIER_EVENT_GENERAL_HW_ERR (1ULL << 6)
#define HL_NOTIFIER_EVENT_RAZWI (1ULL << 7)
#define HL_NOTIFIER_EVENT_PAGE_FAULT (1ULL << 8)
#define HL_NOTIFIER_EVENT_CRITICL_HW_ERR (1ULL << 9)
#define HL_NOTIFIER_EVENT_CRITICL_FW_ERR (1ULL << 10)
#define HL_INFO_HW_IP_INFO 0
#define HL_INFO_HW_EVENTS 1
#define HL_INFO_DRAM_USAGE 2
#define HL_INFO_HW_IDLE 3
#define HL_INFO_DEVICE_STATUS 4
#define HL_INFO_DEVICE_UTILIZATION 6
#define HL_INFO_HW_EVENTS_AGGREGATE 7
#define HL_INFO_CLK_RATE 8
#define HL_INFO_RESET_COUNT 9
#define HL_INFO_TIME_SYNC 10
#define HL_INFO_CS_COUNTERS 11
#define HL_INFO_PCI_COUNTERS 12
#define HL_INFO_CLK_THROTTLE_REASON 13
#define HL_INFO_SYNC_MANAGER 14
#define HL_INFO_TOTAL_ENERGY 15
#define HL_INFO_PLL_FREQUENCY 16
#define HL_INFO_POWER 17
#define HL_INFO_OPEN_STATS 18
#define HL_INFO_DRAM_REPLACED_ROWS 21
#define HL_INFO_DRAM_PENDING_ROWS 22
#define HL_INFO_LAST_ERR_OPEN_DEV_TIME 23
#define HL_INFO_CS_TIMEOUT_EVENT 24
#define HL_INFO_RAZWI_EVENT 25
#define HL_INFO_DEV_MEM_ALLOC_PAGE_SIZES 26
#define HL_INFO_SECURED_ATTESTATION 27
#define HL_INFO_REGISTER_EVENTFD 28
#define HL_INFO_UNREGISTER_EVENTFD 29
#define HL_INFO_GET_EVENTS 30
#define HL_INFO_UNDEFINED_OPCODE_EVENT 31
#define HL_INFO_ENGINE_STATUS 32
#define HL_INFO_PAGE_FAULT_EVENT 33
#define HL_INFO_USER_MAPPINGS 34
#define HL_INFO_FW_GENERIC_REQ 35
#define HL_INFO_HW_ERR_EVENT 36
#define HL_INFO_FW_ERR_EVENT 37
#define HL_INFO_VERSION_MAX_LEN 128
#define HL_INFO_CARD_NAME_MAX_LEN 16
#define HL_ENGINES_DATA_MAX_SIZE SZ_1M
struct hl_info_hw_ip_info {
  __u64 sram_base_address;
  __u64 dram_base_address;
  __u64 dram_size;
  __u32 sram_size;
  __u32 num_of_events;
  __u32 device_id;
  __u32 module_id;
  __u32 decoder_enabled_mask;
  __u16 first_available_interrupt_id;
  __u16 server_type;
  __u32 cpld_version;
  __u32 psoc_pci_pll_nr;
  __u32 psoc_pci_pll_nf;
  __u32 psoc_pci_pll_od;
  __u32 psoc_pci_pll_div_factor;
  __u8 tpc_enabled_mask;
  __u8 dram_enabled;
  __u8 security_enabled;
  __u8 mme_master_slave_mode;
  __u8 cpucp_version[HL_INFO_VERSION_MAX_LEN];
  __u8 card_name[HL_INFO_CARD_NAME_MAX_LEN];
  __u64 tpc_enabled_mask_ext;
  __u64 dram_page_size;
  __u32 edma_enabled_mask;
  __u16 number_of_user_interrupts;
  __u8 reserved1;
  __u8 reserved2;
  __u64 reserved3;
  __u64 device_mem_alloc_default_page_size;
  __u64 reserved4;
  __u64 reserved5;
  __u32 reserved6;
  __u8 reserved7;
  __u8 revision_id;
  __u16 tpc_interrupt_id;
  __u32 rotator_enabled_mask;
  __u32 reserved9;
  __u64 engine_core_interrupt_reg_addr;
  __u64 reserved_dram_size;
};
struct hl_info_dram_usage {
  __u64 dram_free_mem;
  __u64 ctx_dram_mem;
};
#define HL_BUSY_ENGINES_MASK_EXT_SIZE 4
struct hl_info_hw_idle {
  __u32 is_idle;
  __u32 busy_engines_mask;
  __u64 busy_engines_mask_ext[HL_BUSY_ENGINES_MASK_EXT_SIZE];
};
struct hl_info_device_status {
  __u32 status;
  __u32 pad;
};
struct hl_info_device_utilization {
  __u32 utilization;
  __u32 pad;
};
struct hl_info_clk_rate {
  __u32 cur_clk_rate_mhz;
  __u32 max_clk_rate_mhz;
};
struct hl_info_reset_count {
  __u32 hard_reset_cnt;
  __u32 soft_reset_cnt;
};
struct hl_info_time_sync {
  __u64 device_time;
  __u64 host_time;
};
struct hl_info_pci_counters {
  __u64 rx_throughput;
  __u64 tx_throughput;
  __u64 replay_cnt;
};
enum hl_clk_throttling_type {
  HL_CLK_THROTTLE_TYPE_POWER,
  HL_CLK_THROTTLE_TYPE_THERMAL,
  HL_CLK_THROTTLE_TYPE_MAX
};
#define HL_CLK_THROTTLE_POWER (1 << HL_CLK_THROTTLE_TYPE_POWER)
#define HL_CLK_THROTTLE_THERMAL (1 << HL_CLK_THROTTLE_TYPE_THERMAL)
struct hl_info_clk_throttle {
  __u32 clk_throttling_reason;
  __u32 pad;
  __u64 clk_throttling_timestamp_us[HL_CLK_THROTTLE_TYPE_MAX];
  __u64 clk_throttling_duration_ns[HL_CLK_THROTTLE_TYPE_MAX];
};
struct hl_info_energy {
  __u64 total_energy_consumption;
};
#define HL_PLL_NUM_OUTPUTS 4
struct hl_pll_frequency_info {
  __u16 output[HL_PLL_NUM_OUTPUTS];
};
struct hl_open_stats_info {
  __u64 open_counter;
  __u64 last_open_period_ms;
  __u8 is_compute_ctx_active;
  __u8 compute_ctx_in_release;
  __u8 pad[6];
};
struct hl_power_info {
  __u64 power;
};
struct hl_info_sync_manager {
  __u32 first_available_sync_object;
  __u32 first_available_monitor;
  __u32 first_available_cq;
  __u32 reserved;
};
struct hl_info_cs_counters {
  __u64 total_out_of_mem_drop_cnt;
  __u64 ctx_out_of_mem_drop_cnt;
  __u64 total_parsing_drop_cnt;
  __u64 ctx_parsing_drop_cnt;
  __u64 total_queue_full_drop_cnt;
  __u64 ctx_queue_full_drop_cnt;
  __u64 total_device_in_reset_drop_cnt;
  __u64 ctx_device_in_reset_drop_cnt;
  __u64 total_max_cs_in_flight_drop_cnt;
  __u64 ctx_max_cs_in_flight_drop_cnt;
  __u64 total_validation_drop_cnt;
  __u64 ctx_validation_drop_cnt;
};
struct hl_info_last_err_open_dev_time {
  __s64 timestamp;
};
struct hl_info_cs_timeout_event {
  __s64 timestamp;
  __u64 seq;
};
#define HL_RAZWI_NA_ENG_ID U16_MAX
#define HL_RAZWI_MAX_NUM_OF_ENGINES_PER_RTR 128
#define HL_RAZWI_READ BIT(0)
#define HL_RAZWI_WRITE BIT(1)
#define HL_RAZWI_LBW BIT(2)
#define HL_RAZWI_HBW BIT(3)
#define HL_RAZWI_RR BIT(4)
#define HL_RAZWI_ADDR_DEC BIT(5)
struct hl_info_razwi_event {
  __s64 timestamp;
  __u64 addr;
  __u16 engine_id[HL_RAZWI_MAX_NUM_OF_ENGINES_PER_RTR];
  __u16 num_of_possible_engines;
  __u8 flags;
  __u8 pad[5];
};
#define MAX_QMAN_STREAMS_INFO 4
#define OPCODE_INFO_MAX_ADDR_SIZE 8
struct hl_info_undefined_opcode_event {
  __s64 timestamp;
  __u64 cb_addr_streams[MAX_QMAN_STREAMS_INFO][OPCODE_INFO_MAX_ADDR_SIZE];
  __u64 cq_addr;
  __u32 cq_size;
  __u32 cb_addr_streams_len;
  __u32 engine_id;
  __u32 stream_id;
};
struct hl_info_hw_err_event {
  __s64 timestamp;
  __u16 event_id;
  __u16 pad[3];
};
enum hl_info_fw_err_type {
  HL_INFO_FW_HEARTBEAT_ERR,
  HL_INFO_FW_REPORTED_ERR,
};
struct hl_info_fw_err_event {
  __s64 timestamp;
  __u16 err_type;
  __u16 event_id;
  __u32 pad;
};
struct hl_info_dev_memalloc_page_sizes {
  __u64 page_order_bitmask;
};
#define SEC_PCR_DATA_BUF_SZ 256
#define SEC_PCR_QUOTE_BUF_SZ 510
#define SEC_SIGNATURE_BUF_SZ 255
#define SEC_PUB_DATA_BUF_SZ 510
#define SEC_CERTIFICATE_BUF_SZ 2046
struct hl_info_sec_attest {
  __u32 nonce;
  __u16 pcr_quote_len;
  __u16 pub_data_len;
  __u16 certificate_len;
  __u8 pcr_num_reg;
  __u8 pcr_reg_len;
  __u8 quote_sig_len;
  __u8 pcr_data[SEC_PCR_DATA_BUF_SZ];
  __u8 pcr_quote[SEC_PCR_QUOTE_BUF_SZ];
  __u8 quote_sig[SEC_SIGNATURE_BUF_SZ];
  __u8 public_data[SEC_PUB_DATA_BUF_SZ];
  __u8 certificate[SEC_CERTIFICATE_BUF_SZ];
  __u8 pad0[2];
};
struct hl_page_fault_info {
  __s64 timestamp;
  __u64 addr;
  __u16 engine_id;
  __u8 pad[6];
};
struct hl_user_mapping {
  __u64 dev_va;
  __u64 size;
};
enum gaudi_dcores {
  HL_GAUDI_WS_DCORE,
  HL_GAUDI_WN_DCORE,
  HL_GAUDI_EN_DCORE,
  HL_GAUDI_ES_DCORE
};
struct hl_info_args {
  __u64 return_pointer;
  __u32 return_size;
  __u32 op;
  union {
    __u32 dcore_id;
    __u32 ctx_id;
    __u32 period_ms;
    __u32 pll_index;
    __u32 eventfd;
    __u32 user_buffer_actual_size;
    __u32 sec_attest_nonce;
    __u32 array_size;
    __u32 fw_sub_opcode;
  };
  __u32 pad;
};
#define HL_CB_OP_CREATE 0
#define HL_CB_OP_DESTROY 1
#define HL_CB_OP_INFO 2
#define HL_MAX_CB_SIZE (0x200000 - 32)
#define HL_CB_FLAGS_MAP 0x1
#define HL_CB_FLAGS_GET_DEVICE_VA 0x2
struct hl_cb_in {
  __u64 cb_handle;
  __u32 op;
  __u32 cb_size;
  __u32 ctx_id;
  __u32 flags;
};
struct hl_cb_out {
  union {
    __u64 cb_handle;
    union {
      struct {
        __u32 usage_cnt;
        __u32 pad;
      };
      __u64 device_va;
    };
  };
};
union hl_cb_args {
  struct hl_cb_in in;
  struct hl_cb_out out;
};
#define HL_CS_CHUNK_FLAGS_USER_ALLOC_CB 0x1
struct hl_cs_chunk {
  union {
    __u64 cb_handle;
    __u64 signal_seq_arr;
    __u64 encaps_signal_seq;
  };
  __u32 queue_index;
  union {
    __u32 cb_size;
    __u32 num_signal_seq_arr;
    __u32 encaps_signal_offset;
  };
  __u32 cs_chunk_flags;
  __u32 collective_engine_id;
  __u32 pad[10];
};
#define HL_CS_FLAGS_FORCE_RESTORE 0x1
#define HL_CS_FLAGS_SIGNAL 0x2
#define HL_CS_FLAGS_WAIT 0x4
#define HL_CS_FLAGS_COLLECTIVE_WAIT 0x8
#define HL_CS_FLAGS_TIMESTAMP 0x20
#define HL_CS_FLAGS_STAGED_SUBMISSION 0x40
#define HL_CS_FLAGS_STAGED_SUBMISSION_FIRST 0x80
#define HL_CS_FLAGS_STAGED_SUBMISSION_LAST 0x100
#define HL_CS_FLAGS_CUSTOM_TIMEOUT 0x200
#define HL_CS_FLAGS_SKIP_RESET_ON_TIMEOUT 0x400
#define HL_CS_FLAGS_ENCAP_SIGNALS 0x800
#define HL_CS_FLAGS_RESERVE_SIGNALS_ONLY 0x1000
#define HL_CS_FLAGS_UNRESERVE_SIGNALS_ONLY 0x2000
#define HL_CS_FLAGS_ENGINE_CORE_COMMAND 0x4000
#define HL_CS_FLAGS_FLUSH_PCI_HBW_WRITES 0x8000
#define HL_CS_FLAGS_ENGINES_COMMAND 0x10000
#define HL_CS_STATUS_SUCCESS 0
#define HL_MAX_JOBS_PER_CS 512
enum hl_engine_command {
  HL_ENGINE_CORE_HALT = 1,
  HL_ENGINE_CORE_RUN = 2,
  HL_ENGINE_STALL = 3,
  HL_ENGINE_RESUME = 4,
  HL_ENGINE_COMMAND_MAX
};
struct hl_cs_in {
  union {
    struct {
      __u64 chunks_restore;
      __u64 chunks_execute;
    };
    struct {
      __u64 engine_cores;
      __u32 num_engine_cores;
      __u32 core_command;
    };
    struct {
      __u64 engines;
      __u32 num_engines;
      __u32 engine_command;
    };
  };
  union {
    __u64 seq;
    __u32 encaps_sig_handle_id;
    struct {
      __u32 encaps_signals_count;
      __u32 encaps_signals_q_idx;
    };
  };
  __u32 num_chunks_restore;
  __u32 num_chunks_execute;
  __u32 timeout;
  __u32 cs_flags;
  __u32 ctx_id;
  __u8 pad[4];
};
struct hl_cs_out {
  union {
    __u64 seq;
    struct {
      __u32 handle_id;
      __u32 count;
    };
  };
  __u32 status;
  __u32 sob_base_addr_offset;
  __u16 sob_count_before_submission;
  __u16 pad[3];
};
union hl_cs_args {
  struct hl_cs_in in;
  struct hl_cs_out out;
};
#define HL_WAIT_CS_FLAGS_INTERRUPT 0x2
#define HL_WAIT_CS_FLAGS_INTERRUPT_MASK 0xFFF00000
#define HL_WAIT_CS_FLAGS_ANY_CQ_INTERRUPT 0xFFF00000
#define HL_WAIT_CS_FLAGS_ANY_DEC_INTERRUPT 0xFFE00000
#define HL_WAIT_CS_FLAGS_MULTI_CS 0x4
#define HL_WAIT_CS_FLAGS_INTERRUPT_KERNEL_CQ 0x10
#define HL_WAIT_CS_FLAGS_REGISTER_INTERRUPT 0x20
#define HL_WAIT_MULTI_CS_LIST_MAX_LEN 32
struct hl_wait_cs_in {
  union {
    struct {
      __u64 seq;
      __u64 timeout_us;
    };
    struct {
      union {
        __u64 addr;
        __u64 cq_counters_handle;
      };
      __u64 target;
    };
  };
  __u32 ctx_id;
  __u32 flags;
  union {
    struct {
      __u8 seq_arr_len;
      __u8 pad[7];
    };
    __u64 interrupt_timeout_us;
  };
  __u64 cq_counters_offset;
  __u64 timestamp_handle;
  __u64 timestamp_offset;
};
#define HL_WAIT_CS_STATUS_COMPLETED 0
#define HL_WAIT_CS_STATUS_BUSY 1
#define HL_WAIT_CS_STATUS_TIMEDOUT 2
#define HL_WAIT_CS_STATUS_ABORTED 3
#define HL_WAIT_CS_STATUS_FLAG_GONE 0x1
#define HL_WAIT_CS_STATUS_FLAG_TIMESTAMP_VLD 0x2
struct hl_wait_cs_out {
  __u32 status;
  __u32 flags;
  __s64 timestamp_nsec;
  __u32 cs_completion_map;
  __u32 pad;
};
union hl_wait_cs_args {
  struct hl_wait_cs_in in;
  struct hl_wait_cs_out out;
};
#define HL_MEM_OP_ALLOC 0
#define HL_MEM_OP_FREE 1
#define HL_MEM_OP_MAP 2
#define HL_MEM_OP_UNMAP 3
#define HL_MEM_OP_MAP_BLOCK 4
#define HL_MEM_OP_EXPORT_DMABUF_FD 5
#define HL_MEM_OP_TS_ALLOC 6
#define HL_MEM_CONTIGUOUS 0x1
#define HL_MEM_SHARED 0x2
#define HL_MEM_USERPTR 0x4
#define HL_MEM_FORCE_HINT 0x8
#define HL_MEM_PREFETCH 0x40
struct hl_mem_in {
  union {
    struct {
      __u64 mem_size;
      __u64 page_size;
    } alloc;
    struct {
      __u64 handle;
    } free;
    struct {
      __u64 hint_addr;
      __u64 handle;
    } map_device;
    struct {
      __u64 host_virt_addr;
      __u64 hint_addr;
      __u64 mem_size;
    } map_host;
    struct {
      __u64 block_addr;
    } map_block;
    struct {
      __u64 device_virt_addr;
    } unmap;
    struct {
      __u64 addr;
      __u64 mem_size;
      __u64 offset;
    } export_dmabuf_fd;
  };
  __u32 op;
  __u32 flags;
  __u32 ctx_id;
  __u32 num_of_elements;
};
struct hl_mem_out {
  union {
    __u64 device_virt_addr;
    __u64 handle;
    struct {
      __u64 block_handle;
      __u32 block_size;
      __u32 pad;
    };
    __s32 fd;
  };
};
union hl_mem_args {
  struct hl_mem_in in;
  struct hl_mem_out out;
};
#define HL_DEBUG_MAX_AUX_VALUES 10
struct hl_debug_params_etr {
  __u64 buffer_address;
  __u64 buffer_size;
  __u32 sink_mode;
  __u32 pad;
};
struct hl_debug_params_etf {
  __u64 buffer_address;
  __u64 buffer_size;
  __u32 sink_mode;
  __u32 pad;
};
struct hl_debug_params_stm {
  __u64 he_mask;
  __u64 sp_mask;
  __u32 id;
  __u32 frequency;
};
struct hl_debug_params_bmon {
  __u64 start_addr0;
  __u64 addr_mask0;
  __u64 start_addr1;
  __u64 addr_mask1;
  __u32 bw_win;
  __u32 win_capture;
  __u32 id;
  __u32 control;
  __u64 start_addr2;
  __u64 end_addr2;
  __u64 start_addr3;
  __u64 end_addr3;
};
struct hl_debug_params_spmu {
  __u64 event_types[HL_DEBUG_MAX_AUX_VALUES];
  __u32 event_types_num;
  __u32 pmtrc_val;
  __u32 trc_ctrl_host_val;
  __u32 trc_en_host_val;
};
#define HL_DEBUG_OP_ETR 0
#define HL_DEBUG_OP_ETF 1
#define HL_DEBUG_OP_STM 2
#define HL_DEBUG_OP_FUNNEL 3
#define HL_DEBUG_OP_BMON 4
#define HL_DEBUG_OP_SPMU 5
#define HL_DEBUG_OP_TIMESTAMP 6
#define HL_DEBUG_OP_SET_MODE 7
struct hl_debug_args {
  __u64 input_ptr;
  __u64 output_ptr;
  __u32 input_size;
  __u32 output_size;
  __u32 op;
  __u32 reg_idx;
  __u32 enable;
  __u32 ctx_id;
};
#define HL_IOCTL_INFO _IOWR('H', 0x01, struct hl_info_args)
#define HL_IOCTL_CB _IOWR('H', 0x02, union hl_cb_args)
#define HL_IOCTL_CS _IOWR('H', 0x03, union hl_cs_args)
#define HL_IOCTL_WAIT_CS _IOWR('H', 0x04, union hl_wait_cs_args)
#define HL_IOCTL_MEMORY _IOWR('H', 0x05, union hl_mem_args)
#define HL_IOCTL_DEBUG _IOWR('H', 0x06, struct hl_debug_args)
#define HL_COMMAND_START 0x01
#define HL_COMMAND_END 0x07
#endif
