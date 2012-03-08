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
#define PN544_MAGIC 0xE9

#define PN544_SET_PWR _IOW(0xE9, 0x01, unsigned int)

struct pn544_i2c_platform_data {
 unsigned int irq_gpio;
 unsigned int ven_gpio;
 unsigned int firm_gpio;
};

