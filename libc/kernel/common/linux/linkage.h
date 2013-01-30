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
#ifndef _LINUX_LINKAGE_H
#define _LINUX_LINKAGE_H
#include <asm/linkage.h>
#ifdef __cplusplus
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define CPP_ASMLINKAGE extern "C"
#else
#define CPP_ASMLINKAGE
#endif
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#ifndef asmlinkage
#define asmlinkage CPP_ASMLINKAGE
#endif
#ifndef prevent_tail_call
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define prevent_tail_call(ret) do { } while (0)
#endif
#ifndef __ALIGN
#define __ALIGN .align 4,0x90
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define __ALIGN_STR ".align 4,0x90"
#endif
#ifdef __ASSEMBLY__
#define ALIGN __ALIGN
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define ALIGN_STR __ALIGN_STR
#ifndef ENTRY
#define ENTRY(name)   .globl name;   ALIGN;   name:
#endif
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define KPROBE_ENTRY(name)   .section .kprobes.text, "ax";   ENTRY(name)
#ifndef END
#define END(name)   .size name, .-name
#endif
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#ifndef ENDPROC
#define ENDPROC(name)   .type name, @function;   END(name)
#endif
#endif
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define NORET_TYPE
#define ATTRIB_NORET __attribute__((noreturn))
#define NORET_AND noreturn,
#ifndef FASTCALL
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define FASTCALL(x) x
#define fastcall
#endif
#endif
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
