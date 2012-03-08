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
#ifndef _LINUX_PERF_EVENT_H
#define _LINUX_PERF_EVENT_H

#include <linux/types.h>
#include <linux/ioctl.h>
#include <asm/byteorder.h>

enum perf_type_id {
 PERF_TYPE_HARDWARE = 0,
 PERF_TYPE_SOFTWARE = 1,
 PERF_TYPE_TRACEPOINT = 2,
 PERF_TYPE_HW_CACHE = 3,
 PERF_TYPE_RAW = 4,
 PERF_TYPE_BREAKPOINT = 5,

 PERF_TYPE_MAX,
};

enum perf_hw_id {

 PERF_COUNT_HW_CPU_CYCLES = 0,
 PERF_COUNT_HW_INSTRUCTIONS = 1,
 PERF_COUNT_HW_CACHE_REFERENCES = 2,
 PERF_COUNT_HW_CACHE_MISSES = 3,
 PERF_COUNT_HW_BRANCH_INSTRUCTIONS = 4,
 PERF_COUNT_HW_BRANCH_MISSES = 5,
 PERF_COUNT_HW_BUS_CYCLES = 6,

 PERF_COUNT_HW_MAX,
};

enum perf_hw_cache_id {
 PERF_COUNT_HW_CACHE_L1D = 0,
 PERF_COUNT_HW_CACHE_L1I = 1,
 PERF_COUNT_HW_CACHE_LL = 2,
 PERF_COUNT_HW_CACHE_DTLB = 3,
 PERF_COUNT_HW_CACHE_ITLB = 4,
 PERF_COUNT_HW_CACHE_BPU = 5,

 PERF_COUNT_HW_CACHE_MAX,
};

enum perf_hw_cache_op_id {
 PERF_COUNT_HW_CACHE_OP_READ = 0,
 PERF_COUNT_HW_CACHE_OP_WRITE = 1,
 PERF_COUNT_HW_CACHE_OP_PREFETCH = 2,

 PERF_COUNT_HW_CACHE_OP_MAX,
};

enum perf_hw_cache_op_result_id {
 PERF_COUNT_HW_CACHE_RESULT_ACCESS = 0,
 PERF_COUNT_HW_CACHE_RESULT_MISS = 1,

 PERF_COUNT_HW_CACHE_RESULT_MAX,
};

enum perf_sw_ids {
 PERF_COUNT_SW_CPU_CLOCK = 0,
 PERF_COUNT_SW_TASK_CLOCK = 1,
 PERF_COUNT_SW_PAGE_FAULTS = 2,
 PERF_COUNT_SW_CONTEXT_SWITCHES = 3,
 PERF_COUNT_SW_CPU_MIGRATIONS = 4,
 PERF_COUNT_SW_PAGE_FAULTS_MIN = 5,
 PERF_COUNT_SW_PAGE_FAULTS_MAJ = 6,
 PERF_COUNT_SW_ALIGNMENT_FAULTS = 7,
 PERF_COUNT_SW_EMULATION_FAULTS = 8,

 PERF_COUNT_SW_MAX,
};

enum perf_event_sample_format {
 PERF_SAMPLE_IP = 1U << 0,
 PERF_SAMPLE_TID = 1U << 1,
 PERF_SAMPLE_TIME = 1U << 2,
 PERF_SAMPLE_ADDR = 1U << 3,
 PERF_SAMPLE_READ = 1U << 4,
 PERF_SAMPLE_CALLCHAIN = 1U << 5,
 PERF_SAMPLE_ID = 1U << 6,
 PERF_SAMPLE_CPU = 1U << 7,
 PERF_SAMPLE_PERIOD = 1U << 8,
 PERF_SAMPLE_STREAM_ID = 1U << 9,
 PERF_SAMPLE_RAW = 1U << 10,

 PERF_SAMPLE_MAX = 1U << 11,
};

enum perf_event_read_format {
 PERF_FORMAT_TOTAL_TIME_ENABLED = 1U << 0,
 PERF_FORMAT_TOTAL_TIME_RUNNING = 1U << 1,
 PERF_FORMAT_ID = 1U << 2,
 PERF_FORMAT_GROUP = 1U << 3,

 PERF_FORMAT_MAX = 1U << 4,
};

#define PERF_ATTR_SIZE_VER0 64  

struct perf_event_attr {

 __u32 type;

 __u32 size;

 __u64 config;

 union {
 __u64 sample_period;
 __u64 sample_freq;
 };

 __u64 sample_type;
 __u64 read_format;

 __u64 disabled : 1,
 inherit : 1,
 pinned : 1,
 exclusive : 1,
 exclude_user : 1,
 exclude_kernel : 1,
 exclude_hv : 1,
 exclude_idle : 1,
 mmap : 1,
 comm : 1,
 freq : 1,
 inherit_stat : 1,
 enable_on_exec : 1,
 task : 1,
 watermark : 1,

 precise_ip : 2,

 __reserved_1 : 47;

 union {
 __u32 wakeup_events;
 __u32 wakeup_watermark;
 };

 __u32 bp_type;
 __u64 bp_addr;
 __u64 bp_len;
};

#define PERF_EVENT_IOC_ENABLE _IO ('$', 0)
#define PERF_EVENT_IOC_DISABLE _IO ('$', 1)
#define PERF_EVENT_IOC_REFRESH _IO ('$', 2)
#define PERF_EVENT_IOC_RESET _IO ('$', 3)
#define PERF_EVENT_IOC_PERIOD _IOW('$', 4, __u64)
#define PERF_EVENT_IOC_SET_OUTPUT _IO ('$', 5)
#define PERF_EVENT_IOC_SET_FILTER _IOW('$', 6, char *)

enum perf_event_ioc_flags {
 PERF_IOC_FLAG_GROUP = 1U << 0,
};

struct perf_event_mmap_page {
 __u32 version;
 __u32 compat_version;

 __u32 lock;
 __u32 index;
 __s64 offset;
 __u64 time_enabled;
 __u64 time_running;

 __u64 __reserved[123];

 __u64 data_head;
 __u64 data_tail;
};

#define PERF_RECORD_MISC_CPUMODE_MASK (7 << 0)
#define PERF_RECORD_MISC_CPUMODE_UNKNOWN (0 << 0)
#define PERF_RECORD_MISC_KERNEL (1 << 0)
#define PERF_RECORD_MISC_USER (2 << 0)
#define PERF_RECORD_MISC_HYPERVISOR (3 << 0)
#define PERF_RECORD_MISC_GUEST_KERNEL (4 << 0)
#define PERF_RECORD_MISC_GUEST_USER (5 << 0)

#define PERF_RECORD_MISC_EXACT_IP (1 << 14)

#define PERF_RECORD_MISC_EXT_RESERVED (1 << 15)

struct perf_event_header {
 __u32 type;
 __u16 misc;
 __u16 size;
};

enum perf_event_type {

 PERF_RECORD_MMAP = 1,

 PERF_RECORD_LOST = 2,

 PERF_RECORD_COMM = 3,

 PERF_RECORD_EXIT = 4,

 PERF_RECORD_THROTTLE = 5,
 PERF_RECORD_UNTHROTTLE = 6,

 PERF_RECORD_FORK = 7,

 PERF_RECORD_READ = 8,

 PERF_RECORD_SAMPLE = 9,

 PERF_RECORD_MAX,
};

enum perf_callchain_context {
 PERF_CONTEXT_HV = (__u64)-32,
 PERF_CONTEXT_KERNEL = (__u64)-128,
 PERF_CONTEXT_USER = (__u64)-512,

 PERF_CONTEXT_GUEST = (__u64)-2048,
 PERF_CONTEXT_GUEST_KERNEL = (__u64)-2176,
 PERF_CONTEXT_GUEST_USER = (__u64)-2560,

 PERF_CONTEXT_MAX = (__u64)-4095,
};

#define PERF_FLAG_FD_NO_GROUP (1U << 0)
#define PERF_FLAG_FD_OUTPUT (1U << 1)

#endif

