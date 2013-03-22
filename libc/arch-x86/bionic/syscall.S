/*
 * Generic syscall call.
 * Upon entry:
 *	%eax: system call number  - caller save
 *	%ebx: arg0 to system call -   callee save
 *	%ecx: arg1                - caller save
 *	%edx: arg2                - caller save
 *	%esi: arg3                -   callee save
 *	%edi: arg4                -   callee save
 *	%ebp: arg5                -   callee save
 */

#include <asm/unistd.h>
#include <machine/asm.h>

ENTRY(syscall)
    # Push the callee save registers.
    push    %ebx
    push    %esi
    push    %edi
    push    %ebp

    # Load all the arguments from the calling frame.
    # (Not all will be valid, depending on the syscall.)
    mov     20(%esp),%eax
    mov     24(%esp),%ebx
    mov     28(%esp),%ecx
    mov     32(%esp),%edx
    mov     36(%esp),%esi
    mov     40(%esp),%edi
    mov     44(%esp),%ebp

    # Make the system call.
    int     $0x80

    # Error?
    cmpl    $-4095, %eax
    jb      1f
    # Yes, so set errno.
    negl    %eax
    pushl   %eax
    call    __set_errno
    addl    $4, %esp
    orl     $-1, %eax
1:
    # Restore the callee save registers.
    pop    %ebp
    pop    %edi
    pop    %esi
    pop    %ebx
    ret
END(syscall)
