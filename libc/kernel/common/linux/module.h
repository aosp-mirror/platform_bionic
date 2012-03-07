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
#ifndef _LINUX_MODULE_H
#define _LINUX_MODULE_H
#include <linux/sched.h>
#include <linux/spinlock.h>
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#include <linux/list.h>
#include <linux/stat.h>
#include <linux/compiler.h>
#include <linux/cache.h>
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#include <linux/kmod.h>
#include <linux/elf.h>
#include <linux/stringify.h>
#include <linux/kobject.h>
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#include <linux/moduleparam.h>
#include <asm/local.h>
#include <asm/module.h>
#define MODULE_SUPPORTED_DEVICE(name)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#ifndef MODULE_SYMBOL_PREFIX
#define MODULE_SYMBOL_PREFIX ""
#endif
#define MODULE_NAME_LEN (64 - sizeof(unsigned long))
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct kernel_symbol
{
 unsigned long value;
 const char *name;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct modversion_info
{
 unsigned long crc;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 char name[MODULE_NAME_LEN];
};
struct module;
struct module_attribute {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct attribute attr;
 ssize_t (*show)(struct module_attribute *, struct module *, char *);
 ssize_t (*store)(struct module_attribute *, struct module *,
 const char *, size_t count);
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 void (*setup)(struct module *, const char *);
 int (*test)(struct module *);
 void (*free)(struct module *);
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct module_kobject
{
 struct kobject kobj;
 struct module *mod;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct exception_table_entry;
#ifdef MODULE
#define MODULE_GENERIC_TABLE(gtype,name)  extern const struct gtype##_id __mod_##gtype##_table   __attribute__ ((unused, alias(__stringify(name))))
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define THIS_MODULE (&__this_module)
#else
#define MODULE_GENERIC_TABLE(gtype,name)
#define THIS_MODULE ((struct module *)0)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#endif
#define MODULE_INFO(tag, info) __MODULE_INFO(tag, tag, info)
#define MODULE_ALIAS(_alias) MODULE_INFO(alias, _alias)
#define MODULE_LICENSE(_license) MODULE_INFO(license, _license)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define MODULE_AUTHOR(_author) MODULE_INFO(author, _author)
#define MODULE_DESCRIPTION(_description) MODULE_INFO(description, _description)
#define MODULE_PARM_DESC(_parm, desc)   __MODULE_INFO(parm, _parm, #_parm ":" desc)
#define MODULE_DEVICE_TABLE(type,name)   MODULE_GENERIC_TABLE(type##_device,name)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define MODULE_VERSION(_version) MODULE_INFO(version, _version)
struct notifier_block;
#define EXPORT_SYMBOL(sym)
#define EXPORT_SYMBOL_GPL(sym)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define EXPORT_SYMBOL_GPL_FUTURE(sym)
#define EXPORT_UNUSED_SYMBOL(sym)
#define EXPORT_UNUSED_SYMBOL_GPL(sym)
#define symbol_get(x) ({ extern typeof(x) x __attribute__((weak)); &(x); })
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define symbol_put(x) do { } while(0)
#define symbol_put_addr(x) do { } while(0)
#define module_name(mod) "kernel"
#define __unsafe(mod)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define module_put_and_exit(code) do_exit(code)
struct device_driver;
struct module;
#define symbol_request(x) try_then_request_module(symbol_get(x), "symbol:" #x)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define __MODULE_STRING(x) __stringify(x)
#endif
