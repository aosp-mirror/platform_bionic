#include <private/bionic_asm.h>

#define FUTEX_WAIT 0
#define FUTEX_WAKE 1

// int __futex_wait(volatile void* ftx, int val, const struct timespec* timeout)
ENTRY(__futex_wait)
    pushl   %ebx
    pushl   %esi
    mov     12(%esp), %ebx           /* ftx */
    movl    $FUTEX_WAIT, %ecx
    mov     16(%esp), %edx           /* val */
    mov     20(%esp), %esi           /* timeout */
    movl    $__NR_futex, %eax
    int     $0x80
    popl    %esi
    popl    %ebx
    ret
END(__futex_wait)

// int __futex_wake(volatile void* ftx, int count)
ENTRY(__futex_wake)
    pushl   %ebx
    mov     8(%esp), %ebx            /* ftx */
    movl    $FUTEX_WAKE, %ecx
    mov     12(%esp), %edx           /* count */
    movl    $__NR_futex, %eax
    int     $0x80
    popl    %ebx
    ret
END(__futex_wake)

// int __futex_syscall3(volatile void* ftx, int op, int count)
ENTRY(__futex_syscall3)
    pushl   %ebx
    movl    8(%esp), %ebx      /* ftx */
    movl    12(%esp), %ecx      /* op */
    movl    16(%esp), %edx      /* value */
    movl    $__NR_futex, %eax
    int     $0x80
    popl    %ebx
    ret
END(__futex_syscall3)

// int __futex_syscall4(volatile void* ftx, int op, int val, const struct timespec* timeout)
ENTRY(__futex_syscall4)
    pushl   %ebx
    pushl   %esi
    movl    12(%esp), %ebx      /* ftx */
    movl    16(%esp), %ecx      /* op */
    movl    20(%esp), %edx      /* val */
    movl    24(%esp), %esi      /* timeout */
    movl    $__NR_futex, %eax
    int     $0x80
    popl    %esi
    popl    %ebx
    ret
END(__futex_syscall4)
