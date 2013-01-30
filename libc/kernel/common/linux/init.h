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
#ifndef _LINUX_INIT_H
#define _LINUX_INIT_H
#include <linux/compiler.h>
#define __init __attribute__ ((__section__ (".init.text")))
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define __initdata __attribute__ ((__section__ (".init.data")))
#define __exitdata __attribute__ ((__section__(".exit.data")))
#define __exit_call __attribute_used__ __attribute__ ((__section__ (".exitcall.exit")))
#ifdef MODULE
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define __exit __attribute__ ((__section__(".exit.text")))
#else
#define __exit __attribute_used__ __attribute__ ((__section__(".exit.text")))
#endif
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define __INIT .section ".init.text","ax"
#define __FINIT .previous
#define __INITDATA .section ".init.data","aw"
#ifndef __ASSEMBLY__
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
typedef int (*initcall_t)(void);
typedef void (*exitcall_t)(void);
#endif
#ifndef MODULE
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#ifndef __ASSEMBLY__
#define __define_initcall(level,fn)   static initcall_t __initcall_##fn __attribute_used__   __attribute__((__section__(".initcall" level ".init"))) = fn
#define core_initcall(fn) __define_initcall("1",fn)
#define postcore_initcall(fn) __define_initcall("2",fn)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define arch_initcall(fn) __define_initcall("3",fn)
#define subsys_initcall(fn) __define_initcall("4",fn)
#define fs_initcall(fn) __define_initcall("5",fn)
#define device_initcall(fn) __define_initcall("6",fn)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define late_initcall(fn) __define_initcall("7",fn)
#define __initcall(fn) device_initcall(fn)
#define __exitcall(fn)   static exitcall_t __exitcall_##fn __exit_call = fn
#define console_initcall(fn)   static initcall_t __initcall_##fn   __attribute_used__ __attribute__((__section__(".con_initcall.init")))=fn
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define security_initcall(fn)   static initcall_t __initcall_##fn   __attribute_used__ __attribute__((__section__(".security_initcall.init"))) = fn
struct obs_kernel_param {
 const char *str;
 int (*setup_func)(char *);
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 int early;
};
#define __setup_param(str, unique_id, fn, early)   static char __setup_str_##unique_id[] __initdata = str;   static struct obs_kernel_param __setup_##unique_id   __attribute_used__   __attribute__((__section__(".init.setup")))   __attribute__((aligned((sizeof(long)))))   = { __setup_str_##unique_id, fn, early }
#define __setup_null_param(str, unique_id)   __setup_param(str, unique_id, NULL, 0)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define __setup(str, fn)   __setup_param(str, fn, fn, 0)
#define __obsolete_setup(str)   __setup_null_param(str, __LINE__)
#define early_param(str, fn)   __setup_param(str, fn, fn, 1)
#endif
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define module_init(x) __initcall(x);
#define module_exit(x) __exitcall(x);
#else
#define core_initcall(fn) module_init(fn)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define postcore_initcall(fn) module_init(fn)
#define arch_initcall(fn) module_init(fn)
#define subsys_initcall(fn) module_init(fn)
#define fs_initcall(fn) module_init(fn)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define device_initcall(fn) module_init(fn)
#define late_initcall(fn) module_init(fn)
#define security_initcall(fn) module_init(fn)
#define module_init(initfn)   static inline initcall_t __inittest(void)   { return initfn; }   int init_module(void) __attribute__((alias(#initfn)));
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define module_exit(exitfn)   static inline exitcall_t __exittest(void)   { return exitfn; }   void cleanup_module(void) __attribute__((alias(#exitfn)));
#define __setup_param(str, unique_id, fn)
#define __setup_null_param(str, unique_id)
#define __setup(str, func)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define __obsolete_setup(str)
#endif
#define __nosavedata __attribute__ ((__section__ (".data.nosave")))
#define __init_or_module __init
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define __initdata_or_module __initdata
#define __devinit __init
#define __devinitdata __initdata
#define __devexit __exit
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define __devexitdata __exitdata
#define __cpuinit __init
#define __cpuinitdata __initdata
#define __cpuexit __exit
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define __cpuexitdata __exitdata
#define __meminit __init
#define __meminitdata __initdata
#define __memexit __exit
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define __memexitdata __exitdata
#ifdef MODULE
#define __devexit_p(x) x
#else
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define __devexit_p(x) NULL
#endif
#ifdef MODULE
#define __exit_p(x) x
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#else
#define __exit_p(x) NULL
#endif
#endif
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
