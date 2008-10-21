	.text
	.align 4
	.type _start,#function
	.globl _start

_start:
	mov	r0, sp
	mov	r1, #0
	bl	__linker_init

	/* linker init returns the _entry address in the main image */
	mov	pc, r0

	.section .ctors, "wa"
	.globl __CTOR_LIST__
__CTOR_LIST__:
	.long -1
