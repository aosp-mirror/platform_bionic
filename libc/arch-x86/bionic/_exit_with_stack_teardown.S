#include <private/bionic_asm.h>

// void _exit_with_stack_teardown(void* stackBase, size_t stackSize)
ENTRY_PRIVATE(_exit_with_stack_teardown)
  // We can trash registers because this function never returns.
  mov 4(%esp), %ebx             // stackBase
  mov 8(%esp), %ecx             // stackSize
  mov $__NR_munmap, %eax
  int $0x80
  // If munmap failed, we ignore the failure and exit anyway.

  mov $0, %ebx                  // status
  movl $__NR_exit, %eax
  int $0x80
  // The exit syscall does not return.
END(_exit_with_stack_teardown)
