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
#ifndef _PGTABLE_NOPUD_H
#define _PGTABLE_NOPUD_H
#ifndef __ASSEMBLY__
#define __PAGETABLE_PUD_FOLDED
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
typedef struct { pgd_t pgd; } pud_t;
#define PUD_SHIFT PGDIR_SHIFT
#define PTRS_PER_PUD 1
#define PUD_SIZE (1UL << PUD_SHIFT)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define PUD_MASK (~(PUD_SIZE-1))
#define pud_ERROR(pud) (pgd_ERROR((pud).pgd))
#define pgd_populate(mm, pgd, pud) do { } while (0)
#define set_pgd(pgdptr, pgdval) set_pud((pud_t *)(pgdptr), (pud_t) { pgdval })
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define pud_val(x) (pgd_val((x).pgd))
#define __pud(x) ((pud_t) { __pgd(x) } )
#define pgd_page(pgd) (pud_page((pud_t){ pgd }))
#define pgd_page_kernel(pgd) (pud_page_kernel((pud_t){ pgd }))
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define pud_alloc_one(mm, address) NULL
#define pud_free(x) do { } while (0)
#define __pud_free_tlb(tlb, x) do { } while (0)
#undef pud_addr_end
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define pud_addr_end(addr, end) (end)
#endif
#endif
