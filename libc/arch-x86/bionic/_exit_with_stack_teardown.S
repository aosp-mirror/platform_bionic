#include <asm/unistd.h>
#include <machine/asm.h>

// void _exit_with_stack_teardown(void* stackBase, size_t stackSize, int status)
ENTRY(_exit_with_stack_teardown)
    // We can trash %ebx here since this call should never return.
    // We can also take advantage of the fact that the linux syscall trap
    // handler saves all the registers, so we don't need a stack to keep
    // the status argument for exit while doing the munmap.
    mov     4(%esp), %ebx             // stackBase
    mov     8(%esp), %ecx             // stackSize
    mov     $__NR_munmap, %eax
    int     $0x80
    // If munmap failed, we ignore the failure and exit anyway.

    mov     %edx, %ebx                // status
    movl    $__NR_exit, %eax
    int     $0x80
    // The exit syscall does not return.
END(_exit_with_stack_teardown)
