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
#ifndef _LINUX_PAGEMAP_H
#define _LINUX_PAGEMAP_H
#include <linux/mm.h>
#include <linux/fs.h>
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#include <linux/list.h>
#include <linux/highmem.h>
#include <linux/compiler.h>
#include <asm/uaccess.h>
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#include <linux/gfp.h>
#define AS_EIO (__GFP_BITS_SHIFT + 0)
#define AS_ENOSPC (__GFP_BITS_SHIFT + 1)
#define PAGE_CACHE_SHIFT PAGE_SHIFT
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define PAGE_CACHE_SIZE PAGE_SIZE
#define PAGE_CACHE_MASK PAGE_MASK
#define PAGE_CACHE_ALIGN(addr) (((addr)+PAGE_CACHE_SIZE-1)&PAGE_CACHE_MASK)
#define page_cache_get(page) get_page(page)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define page_cache_release(page) put_page(page)
typedef int filler_t(void *, struct page *);
#endif
