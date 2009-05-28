/*
 * Copyright (C) 2008 The Android Open Source Project
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */
	.text
	.align 4
	.type _start,#function
	.globl _start

# This is the small startup code that is first run when
# any static executable runs. A static executable is one
# that is started directly by the Linux kernel, not from
# the dynamic linker, it thus cannot depend on any shared
# library.
#
# It's purpose is to call __libc_init as defined in
# bionic/libc_init_static.c with appropriate
# arguments, which are:
#
#    - the address of the raw data block setup by the Linux
#      kernel ELF loader
#
#    - address of an "onexit" function (not used on any
#      platform supported by Bionic).
#
#    - address of the "main" function of the program.
#
#    - address of the constructors table, i.e. a table
#      that points to various initialization and
#      finalization sections for the program.
#
_start:	
	mov	r0, sp
	mov	r1, #0
	adr r2, 0f
	adr r3, 1f
	b	__libc_init

0:  b   main

# The "C++ ABI for ARM" document that static C++ constructors
# shall only be called from the .init_array section.
#
# Do we really need a .ctors section on ARM platforms ?
# It looks like it will always be empty.
#

1:  .long   __PREINIT_ARRAY__
    .long   __INIT_ARRAY__
    .long   __FINI_ARRAY__
    .long   __CTOR_LIST__

# the .ctors section contains a list of pointers to "constructor"
# functions that need to be called in order during C library initialization,
# just before the program is being run. This is a C++ requirement
#
# the last entry shall be 0, and is defined in crtend.S
#
	.section .preinit_array, "aw"
	.globl __PREINIT_ARRAY__
__PREINIT_ARRAY__:
	.long -1

	.section .init_array, "aw"
	.globl __INIT_ARRAY__
__INIT_ARRAY__:
	.long -1

	.section .fini_array, "aw"
	.globl __FINI_ARRAY__
__FINI_ARRAY__:
	.long -1

	.section .ctors, "aw"
	.globl __CTOR_LIST__
__CTOR_LIST__:
	.long -1

