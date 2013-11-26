#include <asm/unistd.h>
#include <machine/asm.h>

// void _exit_with_stack_teardown(void* stackBase, size_t stackSize, int status)
ENTRY(_exit_with_stack_teardown)
    // We can trash registers because this function never returns.
    // We can also take advantage of the fact that the linux syscall trap
    // handler saves all the registers, so we don't need a stack to keep
    // the status argument for exit while doing the munmap.
    mov     4(%esp), %ebx             // stackBase
    mov     8(%esp), %ecx             // stackSize
    mov     12(%esp), %edx            // Also copy 'status' off the stack we're about to unmap.
    mov     $__NR_munmap, %eax
    int     $0x80
    // If munmap failed, we ignore the failure and exit anyway.

    mov     %edx, %ebx                // status
    movl    $__NR_exit, %eax
    int     $0x80
    // The exit syscall does not return.
END(_exit_with_stack_teardown)
