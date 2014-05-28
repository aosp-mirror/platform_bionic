#include <private/bionic_asm.h>

// int __futex_syscall4(volatile void* ftx, int op, int val, const struct timespec* timeout)
ENTRY_PRIVATE(__futex_syscall4)
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
