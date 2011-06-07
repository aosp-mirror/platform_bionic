.section .init_array, "aw"
.align 4
.type __INIT_ARRAY__, @object
.globl __INIT_ARRAY__
__INIT_ARRAY__:
    .long -1
    .long frame_dummy

.section .fini_array, "aw"
.align 4
.type __FINI_ARRAY__, @object
.globl __FINI_ARRAY__
__FINI_ARRAY__:
    .long -1
    .long __do_global_dtors_aux

	.section	.eh_frame,"a",@progbits
	.align 4
	.type	__EH_FRAME_BEGIN__, @object
__EH_FRAME_BEGIN__:
	.text
	.p2align 4,,15
	.type	__do_global_dtors_aux, @function
__do_global_dtors_aux:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	call	__x86.get_pc_thunk.bx
	addl	$_GLOBAL_OFFSET_TABLE_, %ebx
	subl	$20, %esp
	cmpb	$0, completed.4454@GOTOFF(%ebx)
	jne	.L5
	movl	__dso_handle@GOTOFF(%ebx), %eax
	movl	%eax, (%esp)
	call	__cxa_finalize@PLT
	movl	__deregister_frame_info_bases@GOT(%ebx), %eax
	testl	%eax, %eax
	je	.L4
	leal	__EH_FRAME_BEGIN__@GOTOFF(%ebx), %eax
	movl	%eax, (%esp)
	call	__deregister_frame_info_bases@PLT
.L4:
	movb	$1, completed.4454@GOTOFF(%ebx)
.L5:
	addl	$20, %esp
	popl	%ebx
	popl	%ebp
	ret
	.text
	.p2align 4,,15
	.type	frame_dummy, @function
frame_dummy:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	call	__x86.get_pc_thunk.bx
	addl	$_GLOBAL_OFFSET_TABLE_, %ebx
	subl	$20, %esp
	movl	__register_frame_info_bases@GOT(%ebx), %eax
	testl	%eax, %eax
	je	.L8
	leal	object.4469@GOTOFF(%ebx), %eax
	movl	%eax, 4(%esp)
	leal	__EH_FRAME_BEGIN__@GOTOFF(%ebx), %eax
	movl	%ebx, 12(%esp)
	movl	$0, 8(%esp)
	movl	%eax, (%esp)
	call	__register_frame_info_bases@PLT
.L8:
	addl	$20, %esp
	popl	%ebx
	popl	%ebp
	ret
	.local	completed.4454
	.comm	completed.4454,1,1
	.local	object.4469
	.comm	object.4469,24,4
	.weak	__register_frame_info_bases
	.weak	__deregister_frame_info_bases

	.section	.text.__x86.get_pc_thunk.bx,"axG",@progbits,__x86.get_pc_thunk.bx,comdat
	.globl	__x86.get_pc_thunk.bx
	.hidden	__x86.get_pc_thunk.bx
	.type	__x86.get_pc_thunk.bx, @function
__x86.get_pc_thunk.bx:
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	movl	(%esp), %ebx
	ret

#include "__dso_handle.S"
