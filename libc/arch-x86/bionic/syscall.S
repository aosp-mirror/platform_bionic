/*
 * Generic syscall call.
 * Upon entry
 *	%eax: system call number
 *	%ebx: arg0 to system call
 *	%ecx: arg..
 *	%edx: arg..
 *	%esi: arg..
 *	%edi: arg..
 * We push these (to save them) load them up with the
 * values from the calling frame (not all will actually be valid)
 * and make the syscall.
 */

#include <sys/linux-syscalls.h>

    .text
    .type syscall, @function
    .globl syscall
    .align 4

syscall:
    push    %ebx
    push    %esi
    push    %edi
    mov     16(%esp),%eax
    mov     20(%esp),%ebx
    mov     24(%esp),%ecx
    mov     28(%esp),%edx
    mov     32(%esp),%esi
    mov     36(%esp),%edi

    int     $0x80

    cmpl    $-4095, %eax
    jb      1f
    negl    %eax
    pushl   %eax
    call    __set_errno
    addl    $4, %esp
    orl     $-1, %eax
1:
    pop    %edi
    pop    %esi
    pop    %ebx
    ret
