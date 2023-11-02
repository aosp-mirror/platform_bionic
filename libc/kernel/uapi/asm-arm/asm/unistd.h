/*
 * This file is auto-generated. Modifications will be lost.
 *
 * See https://android.googlesource.com/platform/bionic/+/master/libc/kernel/
 * for more information.
 */
#ifndef _UAPI__ASM_ARM_UNISTD_H
#define _UAPI__ASM_ARM_UNISTD_H
#define __NR_OABI_SYSCALL_BASE 0x900000
#define __NR_SYSCALL_MASK 0x0fffff
#define __NR_SYSCALL_BASE 0
#include <asm/unistd-eabi.h>
#define __NR_sync_file_range2 __NR_arm_sync_file_range
#define __ARM_NR_BASE (__NR_SYSCALL_BASE + 0x0f0000)
#define __ARM_NR_breakpoint (__ARM_NR_BASE + 1)
#define __ARM_NR_cacheflush (__ARM_NR_BASE + 2)
#define __ARM_NR_usr26 (__ARM_NR_BASE + 3)
#define __ARM_NR_usr32 (__ARM_NR_BASE + 4)
#define __ARM_NR_set_tls (__ARM_NR_BASE + 5)
#define __ARM_NR_get_tls (__ARM_NR_BASE + 6)
#endif
